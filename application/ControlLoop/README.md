# Gimbal control loops

The generated MATLAB Coder code is kept under `position/` and `velocity/` so
their generated `struct0_T` types cannot collide. Application code should use
`control_loop.h` instead of including generated headers directly.

Call `control_loop_run()` at the configured control period. Inputs are in
degrees, degrees/second, and degrees/second2 as named. The public output is
already converted to H-bridge PWM duty in `pwm_duty[2]`.

The first call initializes the generated state. Pass a nonzero `reset` when
changing mode or reloading parameters. Parameters must be populated with the
validated plant and controller values before the first call; zero-filled
parameters are not safe operating values.

For application code, call `control_loop_initialize()` once. Call
`control_command_update()` once per cycle to feed the nonblocking SCI-B parser,
then call `control_loop_run()` and retrieve the result with
`control_loop_output()`. The command app owns the latest command; the control
loop reads that shared command data each cycle.

`control_loop_run()` automatically reads the encoder and IMU interfaces before
executing the selected controller.

SCI-B commands are parsed by the separate `application/control_command` app.
Call `control_command_update()` once per application cycle before
`control_loop_run()`. The command app uses this binary frame:

```text
0xAA | 0xBB | mode | inner_value_L | inner_value_H | outer_value_L | outer_value_H
```

`mode = 0` selects position and `mode = 1` selects velocity. The two values are
signed 16-bit little-endian values scaled by 10: transmit `123` for `12.3`
degrees or degrees/second. Do not run `sci_test()` on SCI-B at the same time,
because it consumes the same RX queue. The parser consumes at most one byte per
call and contains no blocking `while` loop.

The output contains the converted H-bridge duty in `pwm_duty[2]`. The private
signed controller command is used internally for conversion. The conversion is
`duty = 50 + actuator / 2`, clamped to 0..100. The loop does not call
`drive_set_duty()` directly.

`control_loop_input()` currently uses real encoder/IMU values and test
references (10 degrees position, 5 degrees/second velocity). Controller
constants are initialized to test values and can be edited through
`control_loop_parameters()` before running the loop.

The gyro and accelerometer scale constants are currently explicit macros in
`control_loop.c`. Replace them with the verified UNO/DUE sensor scale from the
IMU documentation. The driver interface does not define that scale.
