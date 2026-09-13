#include "timer_interface.h"
#include "DSP28x_Project.h"

static volatile Uint32 timer_ticks;

interrupt void timer0_isr(void)
{
    CpuTimer0Regs.TCR.bit.TIF = 1;
    timer_ticks++;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

void timer_init(void)
{
    StopCpuTimer0();
    CpuTimer0Regs.PRD.all = TIMER_PRD;
    CpuTimer0Regs.TPR.all = 0;
    CpuTimer0Regs.TPRH.all = 0;
    CpuTimer0Regs.TCR.bit.TRB = 1;
    CpuTimer0Regs.TCR.bit.SOFT = 0;
    CpuTimer0Regs.TCR.bit.FREE = 1;
    CpuTimer0Regs.TCR.bit.TIE = 1;

    EALLOW;
    PieVectTable.TINT0 = &timer0_isr;
    EDIS;

    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
    IER |= M_INT1;

    StartCpuTimer0();
}

Uint32 timer_tick(void)
{
    return timer_ticks;
}
