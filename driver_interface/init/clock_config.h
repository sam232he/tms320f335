#ifndef CLOCK_CONFIG_H
#define CLOCK_CONFIG_H

/* clang / editor only. TI cl2000 defines _TMS320C28XX and ignores this. */
#if !defined(_TMS320C28XX) && !defined(__TMS320C28XX__)
#define interrupt
#define cregister
#define far
#define asm(x)
#endif

/*
 * Board clock. Change XTAL_MHZ to match the crystal on X1/X2.
 *
 * SYSCLKOUT = XTAL_MHZ * PLL_CR / PLL_CLKDIV
 * F28335 maximum SYSCLKOUT is 150 MHz.
 *
 * Common setups:
 *   30 MHz crystal, PLL_CR=10, PLL_DIVSEL=2  ->  150 MHz
 *   20 MHz crystal, PLL_CR=10, PLL_DIVSEL=2  ->  100 MHz
 */
#define XTAL_MHZ        30U

/* PLLCR.DIV: 0 = PLL bypass, 1..10 = multiply OSCCLK. */
#define PLL_CR          10U

/* PLLSTS.DIVSEL: 0 or 1 = divide by 4, 2 = divide by 2, 3 = divide by 1. */
#define PLL_DIVSEL      2U

#if (PLL_DIVSEL == 3U)
#define PLL_CLKDIV      1U
#elif (PLL_DIVSEL == 2U)
#define PLL_CLKDIV      2U
#else
#define PLL_CLKDIV      4U
#endif

#define SYSCLK_MHZ      ((XTAL_MHZ * PLL_CR) / PLL_CLKDIV)

#if (SYSCLK_MHZ > 150U)
#error "F28335 SYSCLKOUT max is 150 MHz. Lower PLL_CR or increase PLL divide."
#endif

#if (PLL_CR > 10U)
#error "F28335 PLLCR.DIV max is 10."
#endif

/* Names InitSysCtrl() / DELAY_US() / InitFlash() already use. */
#define DSP28_PLLCR     PLL_CR
#define DSP28_DIVSEL    PLL_DIVSEL
#define CPU_RATE        (1000.0L / (long double)SYSCLK_MHZ)

#if (SYSCLK_MHZ > 120U)
#define CPU_FRQ_150MHZ  1
#define CPU_FRQ_100MHZ  0
#else
#define CPU_FRQ_150MHZ  0
#define CPU_FRQ_100MHZ  1
#endif

#endif /* CLOCK_CONFIG_H */
