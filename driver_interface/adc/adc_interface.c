#include "adc_interface.h"
#include "DSP28x_Project.h"

static const adc_ch_t adc_slot_ch[] = { ADC_SLOT_LIST };
#define ADC_SLOT_COUNT  ((Uint16)(sizeof(adc_slot_ch) / sizeof(adc_slot_ch[0])))
#define ADC_SLOT_MAX    16U     /* cascaded SEQ: A0-A7 + B0-B7 */

static volatile Uint16 adc_result[sizeof(adc_slot_ch) / sizeof(adc_slot_ch[0])];

static Uint16 adc_n_slots(void)
{
    Uint16 n;

    n = ADC_SLOT_COUNT;
    if (n < 1U)
    {
        n = 1U;
    }
    else if (n > ADC_SLOT_MAX)
    {
        n = ADC_SLOT_MAX;
    }
    return n;
}

static void adc_write_conv(Uint16 index, Uint16 ch)
{
    Uint16 shift;
    Uint16 mask;
    Uint16 field;

    shift = (Uint16)((index & 3U) * 4U);
    mask = (Uint16)(0x000FU << shift);
    field = (Uint16)((ch & 0x000FU) << shift);

    if (index < 4U)
    {
        AdcRegs.ADCCHSELSEQ1.all = (AdcRegs.ADCCHSELSEQ1.all & ~mask) | field;
    }
    else if (index < 8U)
    {
        AdcRegs.ADCCHSELSEQ2.all = (AdcRegs.ADCCHSELSEQ2.all & ~mask) | field;
    }
    else if (index < 12U)
    {
        AdcRegs.ADCCHSELSEQ3.all = (AdcRegs.ADCCHSELSEQ3.all & ~mask) | field;
    }
    else
    {
        AdcRegs.ADCCHSELSEQ4.all = (AdcRegs.ADCCHSELSEQ4.all & ~mask) | field;
    }
}

interrupt void adc_seq1_isr(void)
{
    Uint16 i;
    Uint16 n;

    n = adc_n_slots();
    for (i = 0U; i < n; i++)
    {
        adc_result[i] = *(&AdcMirror.ADCRESULT0 + i);
    }

    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

void adc_init(void)
{
    Uint16 i;
    Uint16 n;

    n = adc_n_slots();

    EALLOW;
    SysCtrlRegs.HISPCP.all = ADC_HISPCP;
    EDIS;

    InitAdc();

    AdcRegs.ADCTRL1.bit.ACQ_PS = ADC_ACQ_PS;
    AdcRegs.ADCTRL1.bit.CPS = 0;
    AdcRegs.ADCTRL1.bit.CONT_RUN = 0;
    AdcRegs.ADCTRL1.bit.SEQ_OVRD = 0;
    AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;

    AdcRegs.ADCTRL3.bit.ADCCLKPS = ADC_ADCCLKPS;
    AdcRegs.ADCTRL3.bit.SMODE_SEL = 0;

    for (i = 0U; i < n; i++)
    {
        adc_write_conv(i, (Uint16)adc_slot_ch[i]);
    }
    AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = (Uint16)(n - 1U);

    AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;
    AdcRegs.ADCTRL2.bit.INT_MOD_SEQ1 = 0;
    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;

    EALLOW;
    PieVectTable.ADCINT = &adc_seq1_isr;
    EDIS;

    PieCtrlRegs.PIEIER1.bit.INTx6 = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
    IER |= M_INT1;
}

Uint16 adc_read(adc_ch_t ch)
{
    Uint16 i;
    Uint16 n;

    n = adc_n_slots();
    for (i = 0U; i < n; i++)
    {
        if (adc_slot_ch[i] == ch)
        {
            return adc_result[i];
        }
    }
    return 0U;
}
