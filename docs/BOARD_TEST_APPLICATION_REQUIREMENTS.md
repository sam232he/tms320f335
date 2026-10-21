# Board Test Application — Software Requirements Specification

## 1. Purpose

The Board Test Application (BTA) shall provide a professional desktop interface for commissioning and testing the gimbal control board over a serial UART connection.

The application shall allow an operator to:

- Open and configure a serial port.
- Send direct PWM commands to the two motors.
- Send fixed position commands in degrees.
- Generate configurable two-axis position trajectories.
- Read and display live encoder, motor, gyroscope, and accelerometer information.
- Upload and verify control-loop parameters.
- Calibrate encoder offsets and receive a confirmation from the board.
- Stop motor activity immediately and place the board in a safe state.
- Record telemetry and test actions for later analysis.

The application is intended for engineering and laboratory use. It is not a production operator console.

## 2. Scope

### 2.1 In scope

- Windows desktop application with a clean, modern graphical user interface.
- UART/COM-port discovery, connection, configuration, and reconnect handling.
- A binary, framed, versioned board protocol with CRC and acknowledgements.
- Manual PWM testing.
- Manual position testing.
- Periodic position trajectory generation.
- Parameter read, edit, write, and verification.
- Live telemetry dashboard.
- Encoder calibration workflow.
- Motor-test utilities, limits, alarms, and emergency stop.
- In-memory telemetry display during the active session; file saving and file transfer are not part of this release.

### 2.2 Out of scope for the first release

- Firmware flashing or bootloader management.
- Automatic tuning of controller gains.
- Closed-loop control executed by the PC.
- Safety certification for unattended operation.
- Replacement of the board firmware control loop.

## 3. Product principles

The BTA shall be:

1. Safe by default: motors shall remain disabled until the operator deliberately enables them.
2. Observable: every command shall show its transmission state, acknowledgement, and error state.
3. Deterministic: periodic commands shall use a monotonic scheduler and clearly display timing.
4. Recoverable: communication loss shall automatically stop or disable motor commands according to the configured safety policy.
5. Traceable: the application shall record board responses, firmware information, parameter changes, and test data.
6. Easy to learn: the main workflow shall be understandable without reading the protocol documentation.

## 4. Users and operating environment

### 4.1 Users

- Firmware engineer
- Controls engineer
- Hardware test engineer
- Manufacturing or laboratory technician under engineering supervision

### 4.2 Recommended platform

- Windows 10 or newer, 64-bit.
- USB-to-UART adapter or native USB serial device.
- Configurable UART settings; the initial board profile shall default to:
  - Baud rate: 115200
  - Data bits: 8
  - Parity: None
  - Stop bits: 1
  - Flow control: None

The software team shall keep all serial settings configurable because the board firmware may change them during development.

## 5. Main application layout

The application shall use a clear navigation structure:

1. **Connection** — serial port and board connection state.
2. **Dashboard** — live status, measurements, and quick controls.
3. **Motor Test** — direct PWM and motor test functions.
4. **Position Test** — direct angles and periodic trajectories.
5. **Parameters** — controller and plant parameter editor.
6. **Encoder Calibration** — encoder readings, offsets, and calibration results.
7. **Telemetry** — live charts and in-memory session data.
8. **Diagnostics** — protocol monitor, firmware information, and raw messages.

The emergency stop shall remain visible and accessible from every tab.

## 6. Connection and board identification

### 6.1 Connection controls

The connection area shall contain:

- COM-port drop-down with refresh button.
- Baud rate, data bits, parity, stop bits, and flow-control selectors.
- Connect and Disconnect buttons.
- Connection state indicator:
  - Disconnected
  - Connecting
  - Connected
  - Communication degraded
  - Faulted
- Board name, firmware version, protocol version, and board uptime after connection.
- Last received message time.
- RX/TX frame counters and CRC-error counter.

### 6.2 Connection behavior

- The application shall not send motor commands while disconnected.
- On connection, the application shall perform a handshake.
- The handshake shall request board identity, protocol version, capabilities, current safety state, and parameter checksum.
- If the board protocol version is unsupported, the application shall show a clear compatibility error and shall not enable motor controls.
- A lost connection shall immediately stop periodic command generation. The board shall independently disable motor output after 500 ms without a valid command or heartbeat; the application shall never rely on reconnecting before the board becomes safe.
- The operator shall be able to reconnect without restarting the application.

## 7. Safety requirements

### 7.1 General safety

- Motor output shall be disabled by default at application startup and after connection.
- The application shall display a prominent warning before enabling motor output.
- The operator shall explicitly acknowledge the warning before the first enable operation.
- There is no physical emergency-stop input in this release; the application shall clearly warn that UART emergency stop is not a personnel-protection device.
- Direct PWM and trajectory controls shall be disabled unless:
  - The board is connected.
  - The handshake has succeeded.
  - The board reports no blocking fault.
  - The operator has enabled motor testing.

### 7.2 Emergency stop

The emergency-stop action shall:

- Stop the PC trajectory scheduler immediately.
- Send a safe-stop command ahead of normal queued commands.
- Command neutral/holding output and disable motor enable.
- Disable motor enable.
- Cancel pending PWM and position commands.
- Remain latched until the operator explicitly resets the stop state and re-enables motor testing.

The UI shall also provide the `Esc` keyboard shortcut for emergency stop. The board firmware shall independently enforce the 500 ms watchdog and output limits; the PC application shall not be the only safety mechanism.

### 7.3 Limits

The application shall support configurable application-side limits for:

- Inner angle range: -50 to +50 degrees.
- Outer angle range: -50 to +50 degrees.
- Signed actuation command range: -100 to +100.
- Maximum trajectory rate.
- Maximum trajectory duration.

The application shall reject values outside the active limits before transmitting them.

## 8. Motor Test tab

### 8.1 Direct PWM mode

The Motor Test tab shall provide independent controls for inner and outer motors:

- Enable/disable control.
- Signed actuation command from -100 to +100. Negative and positive values shall represent the two motor directions; zero shall command neutral/holding output.
- Numeric entry field.
- Slider limited to -100 to +100.
- Direction indicator.
- Actual command sent to the board.
- Last acknowledgement and timestamp.

The UI shall distinguish clearly between:

- Requested command
- Limited command
- Applied command reported by the board
- Measured motor status

The operator shall be able to command both axes together or test one axis independently.

### 8.2 Mandatory motor-test functions

The first release shall include:

- Center/zero command.
- Enable and disable motors.
- Positive and negative step command.
- Configurable ramp from zero to a target PWM.
- Configurable hold time.
- Automatic ramp-down to zero.
- Sine-wave or triangle-wave test with configurable amplitude and frequency.
- Inner-only, outer-only, and both-axis selection.
- Repeat count and test-duration limit.
- Manual pause/resume.

### 8.3 Deferred motor test sequence

The reusable sequence editor is postponed to a later release. Its planned steps are:

- Set motor enable state
- Set PWM/actuation
- Set position reference
- Wait for a duration
- Wait for a specified encoder condition
- Record a marker
- Stop

This feature shall not block the first release.

## 9. Position Test tab

### 9.1 Direct position command

The tab shall provide numeric position fields for:

- Inner angle in degrees
- Outer angle in degrees

The operator shall be able to send the two values once, with an optional command rate and acknowledgement requirement.

The UI shall display the requested position and the latest measured encoder position together.

### 9.2 Periodic degree command

The application shall support a configurable periodic two-axis trajectory. The trajectory editor shall include:

- Inner-axis start angle, in degrees.
- Inner-axis end angle, in degrees.
- Inner-axis step, in degrees.
- Outer-axis start angle, in degrees.
- Outer-axis end angle, in degrees.
- Outer-axis step, in degrees.
- Command update period, in seconds. The minimum supported period shall be defined by the scheduler and displayed by the UI; it shall be safely below the 500 ms watchdog timeout.
- Optional initial delay.
- Optional hold time at the start and end.
- Number of cycles or continuous mode.
- Direction:
  - Start to end
  - End to start
  - Forward and reverse
- Axis selection:
  - Inner only
  - Outer only
  - Both axes

When the operator presses Start, the application shall transmit the configured start point immediately. At every update period thereafter, it shall update the active reference by the configured step. The trajectory shall stop when the end value is reached. If the configured step does not land exactly on the end value, the application shall clamp and transmit the final command at the end value and display a warning.

When one axis reaches its endpoint before the other, that axis shall hold its endpoint while the other axis continues. In forward-and-reverse mode, each endpoint shall be transmitted twice at the direction change.

The software team shall implement an explicit trajectory policy for the following cases:

- Start equals end: hold the value.
- Step equals zero: reject the trajectory.
- Step sign does not move toward the end: reject or automatically correct only if the operator confirms.
- Period less than the supported scheduler period: reject and show the minimum supported period.
- A value outside angle limits: reject before starting.

### 9.3 Trajectory preview

Before starting, the application shall show:

- A table of generated commands.
- Estimated duration.
- Number of points.
- Maximum angle and estimated rate.
- A plot of inner and outer references versus time.

The operator shall be able to edit and reuse a trajectory during the active session; file saving and loading are excluded.

### 9.4 Trajectory runtime controls

During execution, the UI shall provide:

- Start, pause, resume, and stop.
- Current point number and total point count.
- Elapsed and remaining time.
- Current target values.
- Last command acknowledgement.
- Scheduler timing statistics.
- Immediate emergency stop.

## 10. Control-loop parameter tab

The Parameters tab shall expose the following parameters with engineering units, descriptions, valid ranges, and editable values:

| Parameter | Wire type | Unit / meaning | Initial default |
|---|---:|---|---:|
| `max_angle_inner_deg` | float32 | Maximum inner angle, degrees | 180.0 |
| `max_angle_outer_deg` | float32 | Maximum outer angle, degrees | 180.0 |
| `max_actuation` | float32 | Maximum signed actuation, -100 to +100 | 100.0 |
| `dt_s` | float32 | Control-loop sample period, seconds | 0.001 |
| `inner_motor_kt` | float32 | Inner motor torque constant; engineering unit TBD | 1.0 |
| `inner_motor_resistance` | float32 | Inner motor resistance; engineering unit TBD | 1.0 |
| `outer_motor_kt` | float32 | Outer motor torque constant; engineering unit TBD | 1.0 |
| `outer_motor_resistance` | float32 | Outer motor resistance; engineering unit TBD | 1.0 |
| `bus_voltage` | float32 | DC bus voltage, volts | 24.0 |
| `ae` | float32 | Plant/model coefficient `Ae`; unit TBD | 1.0 |
| `ar` | float32 | Plant/model coefficient `Ar`; unit TBD | 1.0 |
| `ad` | float32 | Plant/model coefficient `Ad`; unit TBD | 1.0 |
| `ard` | float32 | Plant/model coefficient `Ard`; unit TBD | 1.0 |
| `bk` | float32 | Plant/model coefficient `Bk`; unit TBD | 1.0 |
| `rate_kob` | float32 | Rate-controller parameter; unit TBD | 1.0 |
| `rate_tsettle_s` | float32 | Rate-controller settling time, seconds | 0.1 |
| `position_kob` | float32 | Position-controller parameter; unit TBD | 1.0 |
| `position_tsettle_s` | float32 | Position-controller settling time, seconds | 0.1 |
| `min_jeq` | float32 | Minimum equivalent inertia; unit TBD | 0.1 |
| `min_abs_cos_epsilon` | float32 | Minimum absolute cosine protection threshold | 0.1 |

The controller defaults above are copied from `control_loop.c`. Independently of those controller parameter values, this board-test application shall restrict all operator position commands and trajectories to -50 to +50 degrees on both axes.

### 10.1 Parameter operations

The tab shall support:

- Read parameters from the board.
- Edit one or more parameters locally.
- Validate values before transmission.
- Write parameters to volatile board memory.
- Read back and compare every value.
- Keep the edited parameter set in memory for the active session; file saving is intentionally excluded from this release.
- Show modified, validated, transmitted, acknowledged, and verified states.
- Display a parameter checksum or revision number.
- Reset to the last verified set held in memory.
- Reset to a board-provided safe/default set, subject to confirmation.

Each parameter shall have its own Set button. A Set All button shall transmit the complete in-memory parameter set. The application shall warn the operator that parameters can cause unstable or unsafe control behavior. It shall not silently transmit a partially filled parameter set.

### 10.2 Parameter validation

Parameter writes shall be volatile for the first release. Nonvolatile parameter storage is deferred. The Set All operation shall not report success until all values have been acknowledged and read back.

The board shall remain the final authority for parameter validity. The application shall nevertheless validate:

- Finite numeric values; reject NaN and infinity.
- Positive timing values.
- Positive resistance, voltage, inertia, and settling-time values where applicable.
- Angle and actuation limits greater than zero.
- `min_abs_cos_epsilon` within a documented safe interval.
- Parameter relationships defined by the controls team.

The board shall report authoritative ranges where possible. The application shall use the defaults listed above until the controls team supplies final engineering units and limits for the fields marked TBD.

## 11. Dashboard and status area

The Dashboard shall show a continuously updated status area with clear units and alarm colors.

### 11.1 Encoder status

Display:

- Encoder 1 angle, degrees, floating point with exactly two decimal places.
- Encoder 2 angle, degrees, floating point with exactly two decimal places.
- Raw encoder counts, optionally expandable.
- Encoder validity/status.
- Encoder offset currently applied.
- Timestamp and update rate.

### 11.2 Motor status

For each motor, display:

- Enable state.
- Requested PWM/actuation.
- Applied PWM/actuation.
- Direction.
- Current.
- PWM output.
- Fault state.
- Temperature.
- Saturation/limit state.
- Last command acknowledgement.

### 11.3 IMU status

Display:

- Raw gyro X/Y/Z values, each with configurable decimal precision.
- Raw accelerometer X/Y/Z values, each with configurable decimal precision.
- IMU connection/validity state.
- Sample timestamp and update rate.
- Temperature, if available.

### 11.4 Board and communication status

Display:

- Board state: safe, enabled, running, faulted, calibrating.
- Firmware and protocol versions.
- Board uptime.
- UART round-trip latency.
- RX/TX frame rates.
- CRC errors, rejected commands, timeouts, and dropped frames.
- Last error with human-readable explanation.

## 12. Encoder Calibration tab

The Encoder Calibration tab shall display the same live encoder values as the Dashboard:

- Encoder 1 angle with two decimal places.
- Encoder 2 angle with two decimal places.
- Raw counts and current offset.
- Encoder validity and update timestamp.

It shall provide two editable fields:

- Encoder 1 calibration offset in degrees.
- Encoder 2 calibration offset in degrees.

The tab shall include:

- Read current offsets.
- Apply offset values.
- Calibrate button.
- Cancel/reset edit button.
- Nonvolatile storage is deferred; the first release shall keep offsets volatile.
- Restore previous verified offsets.
- Calibration result and response details.

### 12.1 Calibration behavior

When the operator presses Calibrate:

1. The application shall validate both offsets.
2. The application shall stop or inhibit active trajectories.
3. The application shall request a board-side calibration operation.
4. The application shall wait for an acknowledgement with a timeout.
5. The application shall read the resulting encoder values and offsets back.
6. The application shall compare the response with the requested values.
7. The UI shall show success, rejection, timeout, or mismatch.

The board shall accept calibration only while motor output is disabled, the board is stationary, and no trajectory is active. The corrected angle shall be `measured_angle + offset`.

The application shall display a calibration indicator on every tab: green when calibration is successfully completed and verified, red when calibration is missing, invalid, or failed.

## 13. Telemetry and test recording

The application shall support:

- Start/stop in-memory telemetry capture.
- A selectable telemetry period, defaulting to 10 Hz.
- Recording of all displayed measurements.
- Recording of requested commands and board acknowledgements.
- Recording of parameter changes and calibration operations.
- Test name, operator, board identity, firmware version, protocol version, and start time for the active session.
- Automatic event markers for start, stop, pause, fault, calibration, and parameter update.

The application shall provide live charts for:

- Commanded versus measured inner angle.
- Commanded versus measured outer angle.
- PWM/actuation versus time.
- Gyro values versus time.
- Accelerometer values versus time.
- Communication latency and frame errors.

File saving, CSV export, and file transfer are excluded from the first release.

## 14. UART protocol requirements

The application shall use SCI-B at 115200 baud, 8N1, with no flow control.
The existing position-command payload is:

```text
0xAA | 0xBB | mode | inner_value_L | inner_value_H | outer_value_L | outer_value_H
```

The firmware telemetry stream shall use the compact CRC-protected frame
defined below. Command framing for motor control, parameters, calibration,
acknowledgements, and faults shall be finalized as a separate command contract;
the application protocol layer shall keep command and telemetry parsers
separate.

### 14.1 Telemetry frame format

Telemetry shall be transmitted as five independent packets every 100 ms
(10 Hz). The frame shall be:

```text
SYNC_1 | SYNC_2 | PACKET_INDEX | PAYLOAD_LENGTH | PAYLOAD | CRC16_LO | CRC16_HI
```

Requirements:

- `SYNC_1` shall be `0xAA` and `SYNC_2` shall be `0xBB`.
- `PACKET_INDEX` shall identify packet 0 through packet 4.
- `PAYLOAD_LENGTH` shall describe payload bytes only and shall be bounded before parsing.
- All multibyte values shall be little-endian.
- CRC shall be CRC-16/CCITT-FALSE with polynomial `0x1021`, initial value `0xFFFF`, no reflection, and final XOR `0x0000`.
- CRC shall cover `PACKET_INDEX`, `PAYLOAD_LENGTH`, and the complete payload.
- CRC bytes shall be transmitted low byte first.
- The maximum telemetry frame shall be 30 bytes.
- The parser shall recover from noise, partial frames, concatenated frames, dropped packets, and CRC errors.
- A dropped packet shall not invalidate successfully received packets with other indexes.

Telemetry payloads shall be:

| Index | Payload |
|---:|---|
| 0 | `tick(u32)`, encoder 1 angle `float32`, encoder 2 angle `float32`, encoder 1 raw count `u32`, encoder 2 raw count `u32`, validity `u16` |
| 1 | `tick(u32)`, gyro X/Y/Z raw values `u16`, temperature `u16`, validity `u16` |
| 2 | `tick(u32)`, accelerometer X/Y/Z raw values `u16`, reserved `u16`, validity `u16` |
| 3 | `tick(u32)`, PWM motor 1/2 `u16`, validity `u16` |
| 4 | `tick(u32)`, motor-current ADC 1/2 raw counts `u16`, current-sensor fault 1/2 `u16`, drive fault `u16`, drive OTW `u16`, validity `u16` |

Current-sensor mapping shall be fixed as follows: sensor 1 current uses ADCINA0
(`A0`) and sensor 1 fault uses active-low GPIO29; sensor 2 current uses ADCINB0
(`B0`) and sensor 2 fault uses active-low GPIO30. The firmware shall report
raw ADC counts and shall report a fault value of `1` when the corresponding
fault input is active.

The current telemetry format does not include a cycle sequence number. The
application shall use packet index, tick, receive time, and freshness timeout
to detect incomplete or stale telemetry cycles. Parameter and calibration
floating-point fields shall use IEEE-754 little-endian `float32`.

### 14.2 Required message categories

The protocol shall support at least:

- Handshake and board identity.
- Heartbeat/watchdog.
- Safe stop and motor enable/disable.
- Direct PWM/actuation command.
- Position command.
- Trajectory status.
- Parameter read/write/verify.
- Encoder offset read/write/calibrate.
- Telemetry subscription and telemetry response.
- Fault and diagnostic event.

### 14.3 Acknowledgements

Every state-changing command shall return an acknowledgement containing:

- Sequence number.
- Accepted/rejected result.
- Error code.
- Board timestamp or uptime.
- Applied or clamped value where relevant.

The application shall retry only commands explicitly defined as safe to retry. Motor commands shall not be blindly retried after an uncertain timeout.

### 14.4 Watchdog

The board shall implement a command watchdog. If valid command or heartbeat frames are not received within a configurable timeout, the board shall enter its defined safe state. The application shall display the active watchdog timeout and warn when communication margin is poor.

## 15. Diagnostics

The Diagnostics tab shall include:

- Raw TX/RX frame monitor.
- Parsed message view.
- Clear filters by message type, direction, and error state.
- In-memory protocol log with filtering.
- Board reset request.
- Board fault list and clear-fault action.
- Firmware capabilities and compile/build information, if available.

Raw protocol tools shall be separated visually from normal test controls to reduce accidental use.

## 16. Error handling

The application shall show actionable errors for:

- Port unavailable or already in use.
- Unsupported board/protocol version.
- Invalid numeric input.
- Command rejected by board.
- Board timeout.
- CRC error.
- Telemetry stream loss.
- Motor fault.
- Encoder or IMU invalid data.
- Safety interlock active.

Each error shall include the recommended operator action. Errors shall not be reported only through a transient pop-up; the persistent status area shall retain the latest active fault.

## 17. Session configuration

The first release shall keep serial settings, safety limits, trajectories, parameters, calibration offsets, and telemetry in memory only. The application shall not save or send test files. A later release may add persistence after explicit approval.

## 18. Non-functional requirements

### 18.1 Usability

- Common actions shall be reachable in no more than two clicks from the relevant tab.
- Units shall be shown beside every engineering value.
- Numeric fields shall support keyboard entry and sensible increment/decrement controls.
- Disabled controls shall explain why they are disabled.
- The application shall use a dark theme; status colors shall remain readable against the dark background.

### 18.2 Performance

- UI updates shall not block on serial I/O.
- Serial parsing shall run asynchronously from the UI thread.
- The application shall remain responsive during telemetry recording and trajectory execution.
- The application shall support the configured 10 Hz telemetry display without visible UI freezing.

### 18.3 Reliability

- The application shall run for at least 8 hours while recording telemetry without uncontrolled memory growth.
- Partial frames, noise, and disconnect/reconnect events shall not crash the application.
- All asynchronous operations shall be cancellable.
- Shutdown shall stop active trajectories and close the serial port safely.

### 18.4 Security and integrity

- Incoming lengths and numeric values shall be validated before use.
- Parameter and calibration writes shall require explicit user action.
- The application shall identify which board produced each recorded test.
- The application shall not execute arbitrary received data as code or configuration.

## 19. Acceptance criteria

The first release shall be accepted when the following can be demonstrated on a real board:

1. The application discovers, opens, and closes the configured UART port.
2. The handshake displays board identity and protocol compatibility.
3. A signed actuation command from -100 to +100 can be sent to either motor and acknowledged.
4. Emergency stop halts a running test and prevents further motor commands until reset.
5. A direct two-axis degree command is accepted and displayed beside measured encoder values.
6. A periodic trajectory can be configured with independent start, end, step, and period values for both axes.
7. The trajectory preview correctly shows its points and duration.
8. The trajectory can be started, paused, resumed, and stopped.
9. Encoder 1 and Encoder 2 are displayed with two decimal places.
10. Raw gyro and accelerometer X/Y/Z values are displayed with timestamps.
11. All listed control-loop parameters can be read, edited, individually set, acknowledged, and read back; Set All performs the complete update.
12. Encoder offsets can be entered for both encoders, sent with the Calibrate action, and verified from the board response; the red/green calibration indicator is visible on every tab.
13. Communication loss stops the PC scheduler and results in the board entering its watchdog-safe state.
14. Telemetry can be captured in memory at 10 Hz with commands, measurements, timestamps, and test metadata; no file export is required.
15. Malformed frames and rejected commands are shown as diagnostics without crashing the application.

## 20. Confirmed design decisions

- The application uses SCI-B at 115200, 8N1, with no flow control.
- The existing command protocol is extended with CRC protection.
- Actuation is signed from -100 to +100; zero commands neutral/holding output.
- Motor enable is controlled separately.
- Angles are limited to -50 to +50 degrees.
- The board disables motor output after 500 ms without a valid command or heartbeat.
- Trajectory period is the command update interval; the start point is sent when Start is pressed.
- The PC generates trajectory points.
- The final endpoint is clamped when necessary, and endpoint values are transmitted twice during reversal.
- Control-loop parameters use little-endian IEEE-754 `float32` on the wire and are volatile in the first release.
- Telemetry is displayed at 10 Hz and uses raw gyro/accelerometer values.
- Encoder offsets are entered and applied in degrees using `measured_angle + offset`.
- Calibration requires disabled, stationary motors and is shown by a red/green indicator on every tab.
- The first release uses a dark theme and does not save or send files.
- The reusable motor sequence editor and nonvolatile storage are deferred.

## 21. Decisions required from the firmware team

Before implementation is finalized, the firmware and software teams shall agree on:

- Final command-frame IDs, command payloads, and command request/response
  correlation fields; telemetry framing and CRC are defined in Section 14.
- Command IDs and payload encoding.
- Inner/outer encoder and motor sign conventions.
- Parameter engineering units and authoritative valid ranges for fields marked TBD.
- Board fault codes and reset behavior.

The software team shall implement the protocol layer from a shared machine-readable protocol definition so the desktop application and firmware cannot silently diverge.
