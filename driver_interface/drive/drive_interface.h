#ifndef DRIVE_INTERFACE_H
#define DRIVE_INTERFACE_H

#include "pwm_interface.h"

/*
 * DRV8432DKD dual full-bridge on top of pwm_interface.
 * Pin mux is in gpio_interface. Complementary PWM and deadband stay
 * in pwm_interface (20 kHz up-down, 1 us). This layer maps motors
 * and DRV reset/status pins. drive_init() is called from system_init.
 * drive_enable() is only used by motor_test.
 *
 * Schematic (drive.md) / SLES242G:
 *   DRIVE_1  OUT_A/OUT_B  PWM_A/B  GPIO10/11  PWM_6  RESET_AB GPIO26
 *   DRIVE_2  OUT_C/OUT_D  PWM_C/D  GPIO9/8    PWM_5  RESET_CD not on DSP
 *   FAULT    GPIO13  OTW GPIO12  chip-wide, active-low (isolators
 *            ACPL-077L treated as non-inverting)
 *
 * RST_AB low Hi-Zs motor 1 only (half-bridges A/B). Motor 2 follows
 * whatever RESET_CD is tied to on the driver sheet.
 *
 * M1/M2/M3 are hardware straps (not on the DSP). All four PWM nets
 * are wired, so Mode 1 or 2 (independent half-bridges), not Mode 4.
 *
 * Complementary pair: 0% and 100% are opposite DC voltages; ~50% is
 * near-zero average. CBC (datasheet): a half-bridge PWM input held
 * DC cannot recover from a CBC event without a PWM edge.
 *
 * OTSD clear needs both RESET_AB and RESET_CD. CD is unavailable, so
 * firmware cannot fully clear an OTSD latch. No 1 s OCSD/OTSD wait
 * here. drive_init asserts RST_AB only; it does not touch PWM duty.
 */

#define DRIVE_1                     0U      /* OUT_A/B, PWM_6, RESET_AB */
#define DRIVE_2                     1U      /* OUT_C/D, PWM_5, RESET_CD N/C */

void drive_init(void);
Uint16 drive_enable(Uint16 motor);              /* 1 = RESET released */
Uint16 drive_disable(Uint16 motor);             /* 1 = RESET asserted */
void drive_set_duty(Uint16 motor, Uint16 duty); /* 0..100 -> pwm_set_duty */
Uint16 drive_fault(void);                       /* 1 = FAULT pin low */
Uint16 drive_otw(void);                         /* 1 = OTW pin low */

#endif /* DRIVE_INTERFACE_H */
