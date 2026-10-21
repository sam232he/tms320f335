#ifndef CONTROL_COMMAND_H
#define CONTROL_COMMAND_H

#include <stdint.h>
#include "../ControlLoop/control_loop.h"

void control_command_initialize(void);
void control_command_update(void);
control_loop_mode_t control_command_mode(void);
double control_command_inner_angle(void);
double control_command_outer_angle(void);

#endif /* CONTROL_COMMAND_H */
