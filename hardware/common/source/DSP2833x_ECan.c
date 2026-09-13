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
     * CAN clock is SYSCLKOUT/2 (TI comment in DSP2833x_ECan.c).
     * 500 kbit/s uses the same 15 tq as TI's 1 Mbit/s example, BRP doubled.
     *   150 MHz SYSCLK -> 75 MHz CAN: BRPREG=9, TSEG1=10, TSEG2=2
     *       75e6 / (10 * 15) = 500 kbit/s
     *   100 MHz SYSCLK -> 50 MHz CAN: BRPREG=9, TSEG1=6, TSEG2=1
     *       50e6 / (10 * 10) = 500 kbit/s
     */
    shadow.CANBTC.all = 0UL;
#if (CPU_FRQ_150MHZ)
    shadow.CANBTC.bit.BRPREG = 9;
    shadow.CANBTC.bit.TSEG2REG = 2;
    shadow.CANBTC.bit.TSEG1REG = 10;
#endif
#if (CPU_FRQ_100MHZ)
    shadow.CANBTC.bit.BRPREG = 9;
    shadow.CANBTC.bit.TSEG2REG = 1;
    shadow.CANBTC.bit.TSEG1REG = 6;
#endif
    shadow.CANBTC.bit.SAM = 1;
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
