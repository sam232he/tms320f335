//
// FILE:   DSP2833x_ECan.c
//
// TITLE:  DSP2833x eCAN Initialization & Support Functions.
//
// Sequence matches C2000Ware DSP2833x_ECan.c / DSP2803x_ECan.c:
// TXFUNC/RXFUNC, SCB, clear MSGCTRL, clear TA/RMP/GIF,
// CCR=1, wait CCE=1, write CANBTC, CCR=0, wait CCE=0, CANME=0.
//

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"

static void InitECanModule(volatile struct ECAN_REGS *regs,
                           volatile struct ECAN_MBOXES *mboxes)
{
    struct ECAN_REGS shadow;
    volatile struct MBOX *mbox;
    Uint16 i;

    EALLOW;

    shadow.CANTIOC.all = regs->CANTIOC.all;
    shadow.CANTIOC.bit.TXFUNC = 1;
    regs->CANTIOC.all = shadow.CANTIOC.all;

    shadow.CANRIOC.all = regs->CANRIOC.all;
    shadow.CANRIOC.bit.RXFUNC = 1;
    regs->CANRIOC.all = shadow.CANRIOC.all;

    /* HECC mode: mailboxes 16-31 and time stamps. */
    shadow.CANMC.all = regs->CANMC.all;
    shadow.CANMC.bit.SCB = 1;
    regs->CANMC.all = shadow.CANMC.all;

    /* MSGCTRL can power up unknown. TI zeros all 32 mailboxes first. */
    mbox = &mboxes->MBOX0;
    for (i = 0U; i < 32U; i++)
    {
        mbox[i].MSGCTRL.all = 0UL;
    }

    regs->CANTA.all = 0xFFFFFFFFUL;
    regs->CANRMP.all = 0xFFFFFFFFUL;
    regs->CANGIF0.all = 0xFFFFFFFFUL;
    regs->CANGIF1.all = 0xFFFFFFFFUL;

    shadow.CANMC.all = regs->CANMC.all;
    shadow.CANMC.bit.CCR = 1;
    regs->CANMC.all = shadow.CANMC.all;

    do
    {
        shadow.CANES.all = regs->CANES.all;
    } while (shadow.CANES.bit.CCE != 1);

    /*
     * CAN clock is SYSCLKOUT/2. 500 kbit/s.
     * Ntq = (TSEG1REG+1) + (TSEG2REG+1) + 1 sync.
     *   150 MHz -> 75 MHz CAN: BRP=10, 15 tq  ->  75e6 / (10 * 15) = 500 k
     *   125 MHz -> 62.5 MHz CAN: BRP=5, 25 tq ->  62.5e6 / (5 * 25) = 500 k
     *   100 MHz -> 50 MHz CAN: BRP=10, 10 tq  ->  50e6 / (10 * 10) = 500 k
     */
    shadow.CANBTC.all = 0UL;
#if (SYSCLK_MHZ == 150U)
    shadow.CANBTC.bit.BRPREG = 9;
    shadow.CANBTC.bit.TSEG2REG = 2;
    shadow.CANBTC.bit.TSEG1REG = 10;
#elif (SYSCLK_MHZ == 125U)
    shadow.CANBTC.bit.BRPREG = 4;
    shadow.CANBTC.bit.TSEG2REG = 7;
    shadow.CANBTC.bit.TSEG1REG = 15;
#elif (SYSCLK_MHZ == 100U)
    shadow.CANBTC.bit.BRPREG = 9;
    shadow.CANBTC.bit.TSEG2REG = 1;
    shadow.CANBTC.bit.TSEG1REG = 6;
#else
#error "No 500 kbit/s CANBTC for this SYSCLK_MHZ."
#endif
    /*
     * SPRUEU1 / SPRU074 CANBTC.SAM: triple-sample only if BRP > 4
     * (BRP = BRPREG+1). 500 kbit/s tables here are BRP 10, 5, 10.
     */
    if ((shadow.CANBTC.bit.BRPREG + 1U) > 4U)
    {
        shadow.CANBTC.bit.SAM = 1;
    }
    regs->CANBTC.all = shadow.CANBTC.all;

    shadow.CANMC.all = regs->CANMC.all;
    shadow.CANMC.bit.CCR = 0;
    regs->CANMC.all = shadow.CANMC.all;

    do
    {
        shadow.CANES.all = regs->CANES.all;
    } while (shadow.CANES.bit.CCE != 0);

    regs->CANME.all = 0UL;

    EDIS;
}

void InitECana(void)
{
    InitECanModule(&ECanaRegs, &ECanaMboxes);
}

#if DSP28_ECANB
void InitECanb(void)
{
    InitECanModule(&ECanbRegs, &ECanbMboxes);
}
#endif

void InitECan(void)
{
    InitECana();
#if DSP28_ECANB
    InitECanb();
#endif
}
