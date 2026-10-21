#include <stdint.h>
#include "control_command.h"
#include "../../driver_interface/sci/sci_interface.h"

#define CONTROL_COMMAND_START       0xAAU
#define CONTROL_COMMAND_SECOND_HEADER 0xBBU
#define CONTROL_COMMAND_FRAME_LEN   7U
#define CONTROL_COMMAND_MODE_POSITION 0U
#define CONTROL_COMMAND_MODE_VELOCITY 1U
#define CONTROL_COMMAND_VALUE_SCALE   10.0

static uint16_t control_command_frame[CONTROL_COMMAND_FRAME_LEN];
static uint16_t control_command_state;
static uint16_t control_command_frame_index;
static control_loop_mode_t control_command_current_mode;
static double control_command_current_inner_angle;
static double control_command_current_outer_angle;

static double control_command_signed16(uint16_t low, uint16_t high)
{
    int32_t value;

    value = (int32_t)low | ((int32_t)high << 8);
    if ((high & 0x80U) != 0U)
    {
        value -= (int32_t)65536;
    }
    return (double)value;
}

void control_command_initialize(void)
{
    control_command_state = 0U;
    control_command_frame_index = 0U;
    control_command_current_mode = CONTROL_LOOP_POSITION;
    control_command_current_inner_angle = 10.0;
    control_command_current_outer_angle = 10.0;
}

void control_command_update(void)
{
    sci_msg_t message;

    /* Nonblocking: consume at most one uint8_t per call. */
    if (sci_rx(SCI_B, &message) == 0U)
    {
        return;
    }

    switch (control_command_state)
    {
        case 0U:
            if (message.data == CONTROL_COMMAND_START)
            {
                control_command_frame[0] = message.data;
                control_command_frame_index = 1U;
                control_command_state = 1U;
            }
            break;

        case 1U:
            if (message.data == CONTROL_COMMAND_SECOND_HEADER)
            {
                control_command_frame[1] = message.data;
                control_command_frame_index = 2U;
                control_command_state = 2U;
            }
            else if (message.data != CONTROL_COMMAND_START)
            {
                control_command_frame_index = 0U;
                control_command_state = 0U;
            }
            break;

        case 2U:
            if ((message.data == CONTROL_COMMAND_MODE_POSITION) ||
                (message.data == CONTROL_COMMAND_MODE_VELOCITY))
            {
                control_command_frame[control_command_frame_index++] = message.data;
                control_command_state = 3U;
            }
            else
            {
                control_command_frame_index = 0U;
                control_command_state = 0U;
            }
            break;

        case 3U:
            control_command_frame[control_command_frame_index++] = message.data;
            if (control_command_frame_index >= CONTROL_COMMAND_FRAME_LEN)
            {
                control_command_current_inner_angle =
                    control_command_signed16(control_command_frame[2],
                                             control_command_frame[3]) /
                    CONTROL_COMMAND_VALUE_SCALE;
                control_command_current_outer_angle =
                    control_command_signed16(control_command_frame[4],
                                             control_command_frame[5]) /
                    CONTROL_COMMAND_VALUE_SCALE;
                if (control_command_frame[2] == CONTROL_COMMAND_MODE_POSITION)
                {
                    control_command_current_mode = CONTROL_LOOP_POSITION;
                }
                else
                {
                    control_command_current_mode = CONTROL_LOOP_VELOCITY;
                }
                control_command_frame_index = 0U;
                control_command_state = 0U;
            }
            break;

        default:
            control_command_frame_index = 0U;
            control_command_state = 0U;
            break;
    }
}

control_loop_mode_t control_command_mode(void)
{
    return control_command_current_mode;
}

double control_command_inner_angle(void)
{
    return control_command_current_inner_angle;
}

double control_command_outer_angle(void)
{
    return control_command_current_outer_angle;
}
