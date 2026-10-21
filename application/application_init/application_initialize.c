#include "application_initialize.h"
#include "../ControlLoop/control_loop.h"
#include "../control_command/control_command.h"

void application_initialize(void)
{
    control_command_initialize();
    control_loop_initialize();
}
