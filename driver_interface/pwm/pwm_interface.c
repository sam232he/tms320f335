#include "pwm_interface.h"
#include "DSP28x_Project.h"

#define PWM_TBCLK_HZ \
    ((SYSCLK_MHZ * 1000000UL) / (PWM_HSP_FACTOR * PWM_CLK_FACTOR))

#if (PWM_COUNT_MODE == TB_COUNT_UPDOWN)
#define PWM_TBPRD_CALC  (PWM_TBCLK_HZ / (2UL * PWM_FREQ_HZ))
#else
#define PWM_TBPRD_CALC  ((PWM_TBCLK_HZ / PWM_FREQ_HZ) - 1UL)
#endif

#define PWM_TBPRD       ((Uint16)PWM_TBPRD_CALC)

#define PWM_DB_TICKS_CALC \
    ((PWM_DB_NS * (PWM_TBCLK_HZ / 1000UL)) / 1000000UL)
#define PWM_DB_TICKS    ((Uint16)PWM_DB_TICKS_CALC)

#if (PWM_TBPRD_CALC == 0UL) || (PWM_TBPRD_CALC > 65535UL)
#error "PWM period out of range. Change PWM_FREQ_HZ or clock dividers."
#endif
#if (PWM_DB_TICKS_CALC > 0x03FFUL)
#error "Deadband ticks exceed 10 bits. Lower PWM_DB_NS."
#endif

typedef struct
{
    pwm_module_t module;
    Uint16 enable;
    Uint16 is_master;
    Uint16 complementary;
    Uint16 phs;
    Uint16 phsdir;
    Uint16 soca_en;
    Uint16 soca_sel;
    Uint16 soca_prd;
} pwm_ch_cfg_t;

static const pwm_ch_cfg_t pwm_ch_table[] =
{
    { PWM_1, PWM1_ENABLE, PWM1_MASTER, PWM1_COMPLEMENTARY, PWM1_PHS, PWM1_PHSDIR, PWM1_SOCA_ENABLE, PWM1_SOCASEL, PWM1_SOCAPRD },
    { PWM_2, PWM2_ENABLE, PWM2_MASTER, PWM2_COMPLEMENTARY, PWM2_PHS, PWM2_PHSDIR, PWM2_SOCA_ENABLE, PWM2_SOCASEL, PWM2_SOCAPRD },
    { PWM_3, PWM3_ENABLE, PWM3_MASTER, PWM3_COMPLEMENTARY, PWM3_PHS, PWM3_PHSDIR, PWM3_SOCA_ENABLE, PWM3_SOCASEL, PWM3_SOCAPRD },
    { PWM_4, PWM4_ENABLE, PWM4_MASTER, PWM4_COMPLEMENTARY, PWM4_PHS, PWM4_PHSDIR, PWM4_SOCA_ENABLE, PWM4_SOCASEL, PWM4_SOCAPRD },
    { PWM_5, PWM5_ENABLE, PWM5_MASTER, PWM5_COMPLEMENTARY, PWM5_PHS, PWM5_PHSDIR, PWM5_SOCA_ENABLE, PWM5_SOCASEL, PWM5_SOCAPRD },
    { PWM_6, PWM6_ENABLE, PWM6_MASTER, PWM6_COMPLEMENTARY, PWM6_PHS, PWM6_PHSDIR, PWM6_SOCA_ENABLE, PWM6_SOCASEL, PWM6_SOCAPRD }
};

static volatile struct EPWM_REGS *pwm_regs(pwm_module_t mod)
{
    switch (mod)
    {
        case PWM_1: return &EPwm1Regs;
        case PWM_2: return &EPwm2Regs;
        case PWM_3: return &EPwm3Regs;
        case PWM_4: return &EPwm4Regs;
        case PWM_5: return &EPwm5Regs;
        case PWM_6: return &EPwm6Regs;
        default:    return 0;
    }
}

static void pwm_apply_trip(volatile struct EPWM_REGS *epwm)
{
    EALLOW;
    epwm->TZSEL.bit.CBC1 = PWM_TZ_CBC1;
    epwm->TZSEL.bit.CBC2 = PWM_TZ_CBC2;
    epwm->TZSEL.bit.CBC3 = PWM_TZ_CBC3;
    epwm->TZSEL.bit.OSHT1 = PWM_TZ_OSHT1;
    epwm->TZSEL.bit.OSHT2 = PWM_TZ_OSHT2;
    epwm->TZSEL.bit.OSHT3 = PWM_TZ_OSHT3;
    epwm->TZCTL.bit.TZA = PWM_TZ_TZA;
    epwm->TZCTL.bit.TZB = PWM_TZ_TZB;
    epwm->TZEINT.bit.CBC = PWM_TZ_CBC_INT;
    epwm->TZEINT.bit.OST = PWM_TZ_OST_INT;
    EDIS;
}

static void pwm_init_channel(const pwm_ch_cfg_t *cfg)
{
    volatile struct EPWM_REGS *epwm;

    if (cfg->enable == 0U)
    {
        return;
    }

    epwm = pwm_regs(cfg->module);
    if (epwm == 0)
    {
        return;
    }

    epwm->TBCTL.bit.CTRMODE = TB_FREEZE;
    epwm->TBPRD = PWM_TBPRD;
    epwm->TBPHS.half.TBPHS = cfg->phs;
    epwm->TBCTR = 0U;

    epwm->TBCTL.bit.PRDLD = PWM_PRDLD;
    epwm->TBCTL.bit.HSPCLKDIV = PWM_HSPCLKDIV;
    epwm->TBCTL.bit.CLKDIV = PWM_CLKDIV;
    epwm->TBCTL.bit.PHSDIR = cfg->phsdir;
    epwm->TBCTL.bit.FREE_SOFT = PWM_FREE_SOFT;

    if (cfg->is_master != 0U)
    {
        epwm->TBCTL.bit.PHSEN = TB_DISABLE;
        epwm->TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;
    }
    else
    {
        epwm->TBCTL.bit.PHSEN = TB_ENABLE;
        epwm->TBCTL.bit.SYNCOSEL = TB_SYNC_IN;
    }

    epwm->CMPCTL.bit.SHDWAMODE = PWM_CMP_SHADOW;
    epwm->CMPCTL.bit.SHDWBMODE = PWM_CMP_SHADOW;
    epwm->CMPCTL.bit.LOADAMODE = PWM_CMP_LOAD;
    epwm->CMPCTL.bit.LOADBMODE = PWM_CMP_LOAD;

    epwm->CMPA.half.CMPA = 0U;
    epwm->CMPB = 0U;

    epwm->AQCTLA.bit.ZRO = PWM_AQ_ZRO;
    epwm->AQCTLA.bit.PRD = PWM_AQ_PRD;
    epwm->AQCTLA.bit.CAU = PWM_AQ_CAU;
    epwm->AQCTLA.bit.CAD = PWM_AQ_CAD;
    epwm->AQCTLA.bit.CBU = AQ_NO_ACTION;
    epwm->AQCTLA.bit.CBD = AQ_NO_ACTION;
    epwm->AQCTLB.all = 0U;

    if (cfg->complementary != 0U)
    {
        epwm->DBCTL.bit.OUT_MODE = PWM_DB_OUT_MODE;
        epwm->DBCTL.bit.POLSEL = PWM_DB_POLSEL;
        epwm->DBCTL.bit.IN_MODE = PWM_DB_IN_MODE;
        epwm->DBRED = PWM_DB_TICKS;
        epwm->DBFED = PWM_DB_TICKS;
    }
    else
    {
        epwm->DBCTL.bit.OUT_MODE = DB_DISABLE;
        epwm->DBRED = 0U;
        epwm->DBFED = 0U;
    }

    epwm->PCCTL.bit.CHPEN = PWM_CHOPPER;
    pwm_apply_trip(epwm);

    epwm->ETSEL.bit.SOCAEN = cfg->soca_en;
    epwm->ETSEL.bit.SOCASEL = cfg->soca_sel;
    epwm->ETPS.bit.SOCAPRD = cfg->soca_prd;
    epwm->ETSEL.bit.SOCBEN = PWM_SOCB_ENABLE;
    epwm->ETSEL.bit.SOCBSEL = PWM_SOCBSEL;
    epwm->ETPS.bit.SOCBPRD = PWM_SOCBPRD;
    epwm->ETSEL.bit.INTEN = PWM_INT_ENABLE;
    epwm->ETSEL.bit.INTSEL = PWM_INTSEL;
    epwm->ETPS.bit.INTPRD = PWM_INTPRD;

    epwm->TBCTL.bit.CTRMODE = PWM_COUNT_MODE;
}

void pwm_init(void)
{
    Uint16 i;

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;

    for (i = 0U; i < (Uint16)(sizeof(pwm_ch_table) / sizeof(pwm_ch_table[0])); i++)
    {
        pwm_init_channel(&pwm_ch_table[i]);
    }

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;
}

void pwm_set_duty(pwm_module_t pwm_module, Uint16 duty)
{
    volatile struct EPWM_REGS *epwm;
    Uint32 cmpa;

    epwm = pwm_regs(pwm_module);
    if (epwm == 0)
    {
        return;
    }

    if (duty > 100U)
    {
        duty = 100U;
    }

    /* Up-down, CAU=SET / CAD=CLEAR: high time is (TBPRD - CMPA) / TBPRD. */
    cmpa = ((Uint32)epwm->TBPRD * (Uint32)(100U - duty)) / 100UL;
    epwm->CMPA.half.CMPA = (Uint16)cmpa;
}
