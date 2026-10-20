# Gimbal control loops

The generated MATLAB Coder code is kept under `position/` and `velocity/` so
their generated `struct0_T` types cannot collide. Application code should use
`control_loop.h` instead of including generated headers directly.

Call `control_loop_run()` at the configured control period. Inputs are in
degrees, degrees/second, and
degrees/second2 as named. `actuator[0]` and `actuator[1]` are the generated
actuation values; mapping those values to PWM duty and enabling the drives is
application/hardware policy and is intentionally not done here.

The first call initializes the generated state. Pass a nonzero `reset` when
changing mode or reloading parameters. Parameters must be populated with the
validated plant and controller values before the first call; zero-filled
parameters are not safe operating values.

For application code, call `control_loop_initialize()` once, then run either
`control_loop_run(CONTROL_LOOP_POSITION, reset)` or
`control_loop_run(CONTROL_LOOP_VELOCITY, reset)`, and retrieve the result with
`control_loop_output()`.

`control_loop_run()` automatically reads the encoder and IMU interfaces before
executing the selected controller.

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
