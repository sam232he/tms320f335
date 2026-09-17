#ifndef PWM_INTERFACE_H
#define PWM_INTERFACE_H

#include "clock_config.h"
#include "board_pins.h"
#include "DSP2833x_Device.h"
#include "DSP2833x_EPwm_defines.h"

typedef enum
{
    PWM_1 = 1,
    PWM_2 = 2,
    PWM_3 = 3,
    PWM_4 = 4,
    PWM_5 = 5,
    PWM_6 = 6
} pwm_module_t;

/*
 * Generic ePWM driver. Pin mux is in gpio_interface.
 *
 * Schematic motor PWM (drive.md):
 *   DSP_EPWM1_A/B = GPIO10/11 = EPWM6A/B  (DRV PWM_A/B)
 *   DSP_EPWM2_A/B = GPIO9/8   = EPWM5B/A  (DRV PWM_C/D)
 *
 * Complementary polarity so schematic *A is active-high PWM:
 *   ePWM6 HIC: EPWM6A (DSP_EPWM1_A) active, EPWM6B complementary
 *   ePWM5 LOC: EPWM5B (DSP_EPWM2_A) active, EPWM5A complementary
 *
 * TBCLK = SYSCLKOUT / (HSP_FACTOR * CLK_FACTOR)
 * Up-count:  f = TBCLK / (TBPRD + 1)
 * Up-down:   f = TBCLK / (2 * TBPRD)
 *
 * PWM_OUTPUT_ENABLE = 1: GPIO8..11 mux to EPWM5/6 (schematic motor PWM).
 * Complementary channels init at 50% (near-zero H-bridge voltage) while
 * the time-base is frozen. RST_AB stays low until drive_enable().
 */

#define PWM_FREQ_HZ                 20000UL
#define PWM_COUNT_MODE              TB_COUNT_UPDOWN
#define PWM_HSPCLKDIV               TB_DIV1
#define PWM_CLKDIV                  TB_DIV1
#define PWM_HSP_FACTOR              1U
#define PWM_CLK_FACTOR              1U
#define PWM_PRDLD                   TB_SHADOW
#define PWM_FREE_SOFT               2U      /* 0 stop, 2 free run, 3 soft stop */
#define PWM_CMP_SHADOW              CC_SHADOW
#define PWM_CMP_LOAD                CC_CTR_ZERO
#define PWM_AQ_ZRO                  AQ_NO_ACTION
#define PWM_AQ_PRD                  AQ_NO_ACTION
#define PWM_AQ_CAU                  AQ_SET
#define PWM_AQ_CAD                  AQ_CLEAR
#define PWM_CHOPPER                 CHP_DISABLE
#define PWM_INT_ENABLE              0U
#define PWM_INTSEL                  ET_CTR_ZERO
#define PWM_INTPRD                  ET_1ST
#define PWM_DB_OUT_MODE             DB_FULL_ENABLE
#define PWM_DB_POLSEL               DB_ACTV_HIC
#define PWM_DB_IN_MODE              DBA_ALL
#define PWM_DB_NS                   1000UL

#define PWM1_ENABLE                 0U
#define PWM1_MASTER                 1U
#define PWM1_COMPLEMENTARY          0U
#define PWM1_PHS                    0U
#define PWM1_PHSDIR                 TB_UP
#define PWM1_SOCA_ENABLE            0U
#define PWM1_SOCASEL                ET_CTR_ZERO
#define PWM1_SOCAPRD                ET_1ST

#define PWM2_ENABLE                 0U
#define PWM2_MASTER                 0U
#define PWM2_COMPLEMENTARY          0U
#define PWM2_PHS                    0U
#define PWM2_PHSDIR                 TB_UP
#define PWM2_SOCA_ENABLE            0U
#define PWM2_SOCASEL                ET_CTR_ZERO
#define PWM2_SOCAPRD                ET_1ST

#define PWM3_ENABLE                 0U
#define PWM3_MASTER                 0U
#define PWM3_COMPLEMENTARY          0U
#define PWM3_PHS                    0U
#define PWM3_PHSDIR                 TB_UP
#define PWM3_SOCA_ENABLE            0U
#define PWM3_SOCASEL                ET_CTR_ZERO
#define PWM3_SOCAPRD                ET_1ST

#define PWM4_ENABLE                 0U
#define PWM4_MASTER                 0U
#define PWM4_COMPLEMENTARY          0U
#define PWM4_PHS                    0U
#define PWM4_PHSDIR                 TB_UP
#define PWM4_SOCA_ENABLE            0U
#define PWM4_SOCASEL                ET_CTR_ZERO
#define PWM4_SOCAPRD                ET_1ST

#define PWM5_ENABLE                 1U      /* DSP_EPWM2: GPIO9 EPWM5B = 2A, GPIO8 EPWM5A = 2B + ADC SOCA */
#define PWM5_MASTER                 1U
#define PWM5_COMPLEMENTARY          1U
#define PWM5_DB_POLSEL              DB_ACTV_LOC  /* 2A (EPWM5B) active-high, 2B complementary */
#define PWM5_PHS                    0U
#define PWM5_PHSDIR                 TB_UP
#define PWM5_SOCA_ENABLE            1U
#define PWM5_SOCASEL                ET_CTR_ZERO
#define PWM5_SOCAPRD                ET_1ST

#define PWM6_ENABLE                 1U      /* DSP_EPWM1: GPIO10 EPWM6A, GPIO11 EPWM6B */
#define PWM6_MASTER                 0U
#define PWM6_COMPLEMENTARY          1U
#define PWM6_DB_POLSEL              DB_ACTV_HIC  /* 1A (EPWM6A) active-high, 1B complementary */
#define PWM6_PHS                    0U
#define PWM6_PHSDIR                 TB_UP
#define PWM6_SOCA_ENABLE            0U
#define PWM6_SOCASEL                ET_CTR_ZERO
#define PWM6_SOCAPRD                ET_1ST

#define PWM_SOCB_ENABLE             0U
#define PWM_SOCBSEL                 ET_CTR_PRD
#define PWM_SOCBPRD                 ET_1ST

#define PWM_TZ_CBC1                 TZ_DISABLE
#define PWM_TZ_CBC2                 TZ_DISABLE
#define PWM_TZ_CBC3                 TZ_DISABLE
#define PWM_TZ_OSHT1                TZ_DISABLE
#define PWM_TZ_OSHT2                TZ_DISABLE
#define PWM_TZ_OSHT3                TZ_DISABLE
#define PWM_TZ_TZA                  TZ_FORCE_LO
#define PWM_TZ_TZB                  TZ_FORCE_LO
#define PWM_TZ_CBC_INT              0U
#define PWM_TZ_OST_INT              0U

void pwm_init(void);
void pwm_set_duty(pwm_module_t mod, Uint16 duty); /* 0 = 0%, 100 = 100% */

#endif /* PWM_INTERFACE_H */
