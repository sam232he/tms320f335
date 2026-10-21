# Board Test Application — Phased Implementation Plan

## 1. Purpose and implementation target

This plan is for agents implementing a Windows desktop application that tests the gimbal board over SCI-B UART.

Technology decisions:

- C# with .NET 10 LTS.
- WPF with XAML.
- MVVM architecture.
- Visual Studio 2026.
- SCI-B, 115200 baud, 8 data bits, no parity, 1 stop bit, no flow control.
- Dark theme only for the first release.
- No file saving, loading, export, or file transfer.

Agents shall implement phases in order. A later phase may begin only when its dependencies and exit criteria are satisfied.

## 2. Confirmed product requirements

- Direct actuation is signed `-100.0 ... +100.0`.
- Zero actuation commands neutral/holding output.
- Motor enable is separate from actuation.
- Inner and outer position commands are limited to `-50.0 ... +50.0` degrees.
- The board disables motor output after 500 ms without a valid command or heartbeat.
- The PC generates position trajectories.
- Trajectory period is the interval between transmitted commands.
- The start point is transmitted immediately when Start is pressed.
- An axis holds its endpoint while the other axis continues.
- A non-exact final step is clamped to the endpoint.
- In forward/reverse mode, endpoints are transmitted twice at direction changes.
- Parameters use little-endian IEEE-754 `float32` on the wire.
- Parameters are volatile in the first release.
- Each parameter has a Set button and there is a Set All button.
- Telemetry display rate is 10 Hz.
- Gyroscope and accelerometer values are displayed raw, without scaling.
- Encoder calibration offsets are entered in degrees.
- Corrected angle is `measured_angle + offset`.
- Calibration requires motors disabled, the board stationary, and no trajectory active.
- Calibration state is shown by a red/green indicator visible on every tab.
- The reusable motor sequence editor is deferred.
- Nonvolatile parameter/calibration storage is deferred.
- There is no physical emergency-stop input; UART stop is not personnel protection.

## 3. Project structure

Create a separate desktop solution under a desktop-app directory. Do not place C# projects in the embedded C folders.

```text
board_test_app/
├── BoardTestApp.sln
├── src/
│   ├── BoardTestApp.UI/          # WPF views, XAML, ViewModels, styles
│   ├── BoardTestApp.Core/        # models, limits, safety, trajectories
│   ├── BoardTestApp.Protocol/    # frames, messages, CRC, serialization
│   ├── BoardTestApp.Serial/      # COM port and asynchronous transport
│   └── BoardTestApp.Diagnostics/ # in-memory logs and protocol trace
└── tests/
    ├── BoardTestApp.Core.Tests/
    ├── BoardTestApp.Protocol.Tests/
    ├── BoardTestApp.Serial.Tests/
    └── BoardTestApp.UI.Tests/
```

Required separation:

- Views shall not open serial ports or construct frames.
- ViewModels shall not calculate CRC or parse raw bytes.
- Safety rules shall live in Core, not in individual buttons.
- Protocol code shall not depend on WPF.
- Serial code shall not depend on view classes.

## 4. Engineering rules for agents

1. Enable nullable reference types.
2. Treat all received bytes and values as untrusted input.
3. Validate frame length before reading a field.
4. Reject NaN, infinity, out-of-range angles, and out-of-range actuation.
5. Never block the UI thread on serial I/O.
6. Use `async/await` and cancellation tokens for communication and tests.
7. Use a monotonic clock for trajectories and timeouts.
8. Do not blindly retry motor commands after uncertain timeouts.
9. Do not hide unresolved firmware assumptions; mark them `TODO(FIRMWARE-CONTRACT)`.
10. Add unit tests for every protocol, safety, validation, and trajectory rule.
11. Do not add file persistence or file transfer in release 1.
12. Do not add the deferred sequence editor in release 1.

## 5. Core interfaces

Create high-level board operations so the UI never constructs protocol frames directly:

```csharp
Task<BoardIdentity> HandshakeAsync(CancellationToken ct);
Task<CommandResult> SetMotorEnableAsync(bool enabled, CancellationToken ct);
Task<CommandResult> SendActuationAsync(MotorAxis axis, float value, CancellationToken ct);
Task<CommandResult> SendPositionAsync(float innerDeg, float outerDeg, CancellationToken ct);
Task<ParameterSet> ReadParametersAsync(CancellationToken ct);
Task<CommandResult> SetParameterAsync(ParameterId id, float value, CancellationToken ct);
Task<CommandResult> SetAllParametersAsync(ParameterSet values, CancellationToken ct);
Task<CalibrationResult> CalibrateEncodersAsync(float innerOffsetDeg, float outerOffsetDeg, CancellationToken ct);
Task<CommandResult> EmergencyStopAsync(CancellationToken ct);
```

Names may change, but the responsibilities shall remain separated.

Required enums include `MotorAxis`, `BoardState`, `MotorEnableState`, `CalibrationState`, `ControlMode`, and `CommandResult`.

## 6. Protocol contract

### 6.1 Existing compatibility payload

The current position/velocity payload is:

```text
0xAA | 0xBB | mode | inner_value_L | inner_value_H | outer_value_L | outer_value_H
```

It uses signed 16-bit little-endian values scaled by 10. The firmware team shall extend the command with CRC protection. The CRC contract must define polynomial, initial value, reflection, final XOR, covered bytes, and CRC byte order.

### 6.2 Current telemetry frame

The firmware telemetry stream uses five independent packets per 100 ms cycle
(10 Hz). The compact frame is:

```text
SYNC_1 | SYNC_2 | PACKET_INDEX | PAYLOAD_LENGTH | PAYLOAD | CRC16_LO | CRC16_HI
```

The current values are:

```text
SYNC_1 = 0xAA
SYNC_2 = 0xBB
PACKET_INDEX = 0..4
```

`PAYLOAD_LENGTH` is the number of payload bytes only. All multibyte numeric
fields are little-endian. CRC is CRC-16/CCITT-FALSE: polynomial `0x1021`,
initial value `0xFFFF`, no reflection, final XOR `0x0000`; it covers
`PACKET_INDEX`, `PAYLOAD_LENGTH`, and every payload byte. The CRC is sent
little-endian. The maximum current frame is 30 bytes.

Telemetry packet definitions:

| Index | Payload |
|---:|---|
| 0 | `tick(u32)`, inner angle `float32`, outer angle `float32`, inner raw encoder `u32`, outer raw encoder `u32`, validity `u16` |
| 1 | `tick(u32)`, gyro X/Y/Z `u16`, temperature `u16`, validity `u16` |
| 2 | `tick(u32)`, accelerometer X/Y/Z `u16`, reserved `u16`, validity `u16` |
| 3 | `tick(u32)`, PWM motor 1/2 `u16`, validity `u16` |
| 4 | `tick(u32)`, current ADC 1/2 raw counts `u16`, current-sensor fault 1/2 `u16`, drive fault `u16`, drive OTW `u16`, validity `u16` |

Current sensors are mapped as follows: sensor 1 current is ADCINA0 (`A0`)
with an active-low fault input on GPIO29; sensor 2 current is ADCINB0 (`B0`)
with an active-low fault input on GPIO30. The firmware reports ADC values as
raw counts and reports each fault as `1` when active.

The receiver shall validate sync, packet index, payload length, and CRC before
interpreting a packet. It shall tolerate partial and concatenated frames,
discard malformed frames, and treat a missing packet index as a partial
telemetry update rather than invalidating the other packets. The current
telemetry format has no sequence field; freshness and packet-index tracking
shall be used to detect loss.

Parameters and calibration commands continue to use little-endian IEEE-754
`float32` fields. Command framing and request/response correlation remain
separate firmware-contract items until command packet IDs are finalized.

Required message categories:

- Handshake, identity, capabilities, and heartbeat.
- Motor enable/disable and emergency stop.
- Signed actuation and two-axis position.
- Telemetry and fault events using the compact telemetry frame above.
- Parameter read, individual write, Set All, and readback.
- Encoder offset read and calibration.
- Acknowledgement, rejection, and board reset.

Every state-changing command shall receive an acknowledgement containing sequence, result, error code, board uptime/timestamp, and applied value where relevant.

### 6.3 Session protection

Reconnect behavior shall be:

1. Stop all schedulers.
2. Clear normal pending commands.
3. Perform a new handshake.
4. Confirm the board is safe.
5. Require the operator to re-arm and re-enable motors.

Use a session ID or connection generation so delayed frames from a previous connection cannot be accepted.

## 7. Safety architecture

Implement this application state machine:

```text
Disconnected → Connecting → ConnectedSafe → TestArmed → MotorsEnabled → TestRunning
```

Faults, disconnects, stale telemetry, invalid encoder data, and emergency stop shall transition toward `ConnectedSafe` or `Disconnected`.

Motor controls shall remain disabled unless:

- Handshake succeeded.
- Board reports no blocking fault.
- Required telemetry is fresh.
- Calibration state is acceptable.
- Operator explicitly armed the test.

Emergency stop shall cancel trajectories, clear normal queued commands, send the highest-priority safe-stop command, disable motors, latch the UI state, and prevent normal commands until explicit reset and re-arm.

The board watchdog shall disable motor output after 500 ms. The application shall send heartbeat frames at a safe interval; use 100 ms initially unless firmware specifies another value.

## 8. Data models

### 8.1 Telemetry

The telemetry snapshot shall include timestamp, board uptime, encoder angles/counts/validity, raw gyro[3], raw accel[3], motor enable, actuation, PWM, current, temperature, faults, board faults, frame counter, and receive time.

Unavailable values shall be represented as unavailable/invalid, never as zero.

Encoder values shall display exactly two decimals, for example `-3.50°`.

### 8.2 Parameters

Use `float` values for:

```text
max_angle_inner_deg, max_angle_outer_deg, max_actuation, dt_s,
inner_motor_kt, inner_motor_resistance, outer_motor_kt,
outer_motor_resistance, bus_voltage, ae, ar, ad, ard, bk,
rate_kob, rate_tsettle_s, position_kob, position_tsettle_s,
min_jeq, min_abs_cos_epsilon
```

Initial defaults shall match `application/ControlLoop/control_loop.c`:

| Parameter | Default |
|---|---:|
| `max_angle_inner_deg` | 180.0 |
| `max_angle_outer_deg` | 180.0 |
| `max_actuation` | 100.0 |
| `dt_s` | 0.001 |
| `inner_motor_kt` | 1.0 |
| `inner_motor_resistance` | 1.0 |
| `outer_motor_kt` | 1.0 |
| `outer_motor_resistance` | 1.0 |
| `bus_voltage` | 24.0 |
| `ae`, `ar`, `ad`, `ard`, `bk` | 1.0 |
| `rate_kob`, `position_kob` | 1.0 |
| `rate_tsettle_s`, `position_tsettle_s` | 0.1 |
| `min_jeq`, `min_abs_cos_epsilon` | 0.1 |

Fields with unknown physical units shall display `Unit TBD`; agents shall not invent scaling. Operator test angles remain limited to `-50 ... +50` independently of controller defaults.

## 9. Trajectory engine

`TrajectoryDefinition` shall contain inner/outer start, end, and step; update period; axis selection; direction; cycle count; continuous flag; initial delay; start hold; and end hold.

Reject NaN/infinity, zero steps when movement is required, wrong step direction, periods below scheduler capability, excessive point count, excessive duration, and angles outside `-50 ... +50`.

Algorithm:

1. Validate the complete definition.
2. Send the start point immediately at Start.
3. Schedule against absolute monotonic deadlines.
4. Add each active-axis step at each update period.
5. Clamp instead of overshooting an endpoint.
6. Hold an axis after it reaches its endpoint.
7. Send endpoint values twice at reversal.
8. Stop or complete only after endpoint and hold requirements finish.

The UI shall show state, point index, total points, target values, elapsed time, last acknowledgement, scheduler lateness, jitter, and stop reason.

## 10. Phased work packages

### Phase 0 — Bootstrap

Tasks:

- Create solution and projects.
- Target .NET 10 WPF.
- Enable nullable reference types and analyzers.
- Add MVVM toolkit or a small tested MVVM implementation.
- Add dark theme resources.
- Add navigation shell and placeholder tabs.
- Add unit-test projects and CI build/test commands.

Exit criteria:

- Clean `dotnet build` succeeds.
- Clean `dotnet test` succeeds.
- UI opens without a COM port.

### Phase 1 — Core validation and safety

Tasks:

- Implement models and enums.
- Implement angle, actuation, parameter, and finite-number validation.
- Implement safety state machine.
- Implement calibration state.
- Implement fake monotonic clock.
- Implement cancellation abstractions.

Tests:

- Boundary values are accepted.
- Out-of-range values are rejected.
- NaN and infinity are rejected.
- Commands are rejected while disconnected or emergency-stopped.
- Calibration is rejected while motors are enabled or trajectory is active.

### Phase 2 — Protocol and CRC

Tasks:

- Implement frame models and message IDs.
- Implement little-endian integer and `float32` serialization.
- Implement CRC behind an interface.
- Implement parser recovery for partial, concatenated, noisy, oversized, and bad-CRC frames.
- Implement acknowledgement and rejection models.
- Use the confirmed telemetry CRC definition; mark only command-frame CRC
  details that are not yet agreed as `TODO(FIRMWARE-CONTRACT)`.

Tests:

- Round-trip encode/decode.
- Known CRC vectors when supplied by firmware.
- Bad sync, length, CRC, and truncation tests.
- Arbitrary byte input never crashes the parser.
- Sequence correlation works.

### Phase 3 — Serial transport

Tasks:

- COM-port discovery and refresh.
- Serial settings and open/close.
- Asynchronous reader and serialized writer.
- High-priority emergency-stop path.
- Cancellation on disconnect.
- Counters for RX, TX, CRC failures, timeouts, and dropped frames.
- Connection generation/session protection.

Tests shall use a fake serial transport and verify cancellation, timeout, write ordering, stop priority, and stale-frame rejection.

### Phase 4 — Handshake and board service

Tasks:

- Implement handshake, board identity, capabilities, safety state, firmware/protocol version, and parameter revision.
- Implement request correlation and result mapping.
- Implement heartbeat scheduler.
- Build Connection view with COM selection, settings, connect/disconnect, state, identity, uptime, and counters.

Exit criteria:

- Real or simulated handshake succeeds.
- Unsupported protocol prevents motor controls.
- Heartbeat loss is visible.

### Phase 5 — Motor Test

Tasks:

- Motor enable/disable.
- Inner/outer actuation fields and sliders limited to `-100 ... +100`.
- Zero/neutral control.
- Current, PWM, temperature, and fault status.
- Positive/negative steps.
- Ramp-up/ramp-down.
- Sine and triangle tests.
- Duration limit, pause, stop, acknowledgement display.

Safety:

- Require explicit arming.
- Require motor-enable acknowledgement.
- Stop on disconnect, stale telemetry, fault, or emergency stop.
- Ramp to neutral at test end.

### Phase 6 — Position and trajectories

Tasks:

- Direct inner/outer position controls.
- Enforce `-50 ... +50`.
- Build trajectory editor, preview plot, point table, start/pause/resume/stop.
- Implement immediate start point, clamping, endpoint hold, endpoint duplication, and jitter display.
- Stop on disconnect or board fault.

Tests shall cover start equals end, wrong step sign, exact endpoint, overshoot clamping, independent axis completion, reversal, pause, resume, and cancellation.

### Phase 7 — Parameters

Tasks:

- Build a row editor for every parameter.
- Show value, wire type, unit, default, and validation state.
- Add individual Set and global Set All.
- Add Read All and readback comparison.
- Show modified, validated, transmitted, acknowledged, and verified states.
- Keep parameters volatile.
- Block unsafe writes while motors are active unless firmware explicitly permits them.

The first release shall not create parameter files.

### Phase 8 — Dashboard and telemetry

Tasks:

- Implement telemetry polling/subscription.
- Display encoder values with exactly two decimals.
- Display raw gyro and accelerometer values.
- Display current, PWM, temperature, fault, and enable state.
- Display telemetry age, update rate, board state, and communication health.
- Display `N/A` for unavailable fields.
- Add in-memory charts and event markers.

The dashboard shall update at 10 Hz without blocking the UI or writing files.

### Phase 9 — Encoder calibration

Tasks:

- Show both encoders as on the Dashboard.
- Add inner/outer degree offset fields.
- Add Read Offsets, Apply/Calibrate, and Cancel/Reset.
- Require disabled, stationary motors and no active trajectory.
- Send little-endian `float32` degree offsets.
- Read back offsets and corrected values.
- Verify `measured + offset`.
- Implement volatile-only behavior.
- Add global red/green calibration indicator.

### Phase 10 — Diagnostics and fault handling

Tasks:

- In-memory raw TX/RX monitor.
- Parsed-frame monitor and filters.
- CRC, timeout, rejection, and dropped-frame counters.
- Board fault list, clear-fault, and reset operations.
- Human-readable error mapping.

The protocol log shall remain in memory only.

### Phase 11 — UX hardening

Tasks:

- Finish dark theme, spacing, typography, and status colors.
- Show units beside every engineering field.
- Add tooltips for advanced parameters.
- Explain why disabled controls are disabled.
- Keep Emergency Stop and calibration indicator visible globally.
- Add `Esc` emergency-stop shortcut.
- Add clean shutdown and uncaught-exception handling.
- Verify keyboard navigation.

### Phase 12 — Hardware-in-the-loop validation

Test with a current-limited supply and physically secured mechanism:

1. Connect and verify handshake.
2. Verify board safe state and telemetry.
3. Verify encoder calibration and red/green indicator.
4. Verify individual Set and Set All parameter behavior.
5. Verify enable/disable and zero holding command.
6. Test small positive/negative actuation.
7. Test `-100` and `+100` limits.
8. Test direct positions at `-50`, `0`, and `+50` degrees.
9. Test trajectory start, pause, resume, stop, and reverse.
10. Disconnect UART during operation.
11. Confirm motor disable within 500 ms.
12. Reconnect and confirm motors remain disabled.
13. Inject CRC errors and confirm diagnosis.
14. Confirm calibration is rejected while motors are enabled.

## 11. Deferred features

The following are explicitly excluded from release 1:

- Reusable motor-test sequence editor.
- Nonvolatile parameter storage.
- Nonvolatile encoder calibration storage.
- Parameter, trajectory, or telemetry file saving.
- File loading, file export, and file transfer.

Agents shall not implement these features unless the project owner reopens the scope.

## 12. Agent handoff format

Every agent shall report:

```text
Phase:
Work package:
Files changed:
Interfaces added:
Safety behavior:
Assumptions:
TODO(FIRMWARE-CONTRACT) items:
Tests added:
Tests executed and results:
Known limitations:
Next-agent prerequisites:
```

An agent shall not claim completion when tests are missing, safety logic is bypassed, or protocol assumptions are hidden.

## 13. Global acceptance criteria

Release 1 is complete only when:

1. A clean checkout builds and all automated tests pass.
2. The dark WPF UI opens without hardware.
3. SCI-B connects with the confirmed serial settings.
4. Handshake and protocol compatibility are visible.
5. Signed actuation `-100 ... +100` works with separate motor enable.
6. Zero actuation produces the defined holding command.
7. Position commands are limited to `-50 ... +50` degrees.
8. Trajectories implement all confirmed endpoint rules.
9. Hardware watchdog behavior disables motors within 500 ms.
10. Every parameter has individual Set and global Set All controls.
11. Parameter values use little-endian `float32` and remain volatile.
12. Dashboard telemetry displays encoder, motor, gyro, and accelerometer data at 10 Hz.
13. Encoder values display with exactly two decimals.
14. Calibration requires disabled, stationary motors and applies `measured + offset`.
15. Calibration status is visible red/green on every tab.
16. No feature saves, loads, exports, or transfers files.
17. CRC errors, timeouts, malformed frames, and board rejections are diagnosable.
18. Emergency stop is globally available and latched until reset.

## 14. Remaining firmware contract checklist

Before hardware integration is complete, firmware shall provide:

- CRC polynomial, initialization, covered bytes, and byte order.
- Message IDs and payload layouts.
- Board identity and capability responses.
- Signed actuation and neutral behavior.
- Telemetry payload definitions for current, PWM, temperature, and faults.
- Parameter field IDs, engineering units, and valid ranges.
- Encoder calibration response and fault codes.
- Board reset behavior and watchdog validation behavior.
- Encoder and motor sign conventions.

These contracts shall be recorded in the protocol source or a shared machine-readable protocol specification.
