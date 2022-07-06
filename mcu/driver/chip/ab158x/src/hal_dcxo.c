/* Copyright Statement:
 *
 * (C) 2017  Airoha Technology Corp. All rights reserved.
 *
 * This software/firmware and related documentation ("Airoha Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to Airoha Technology Corp. ("Airoha") and/or its licensors.
 * Without the prior written permission of Airoha and/or its licensors,
 * any reproduction, modification, use or disclosure of Airoha Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 * You may only use, reproduce, modify, or distribute (as applicable) Airoha Software
 * if you have agreed to and been bound by the applicable license agreement with
 * Airoha ("License Agreement") and been granted explicit permission to do so within
 * the License Agreement ("Permitted User").  If you are not a Permitted User,
 * please cease any access or use of Airoha Software immediately.
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT AIROHA SOFTWARE RECEIVED FROM AIROHA AND/OR ITS REPRESENTATIVES
 * ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. AIROHA EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES AIROHA PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH AIROHA SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN AIROHA SOFTWARE. AIROHA SHALL ALSO NOT BE RESPONSIBLE FOR ANY AIROHA
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND AIROHA'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO AIROHA SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT AIROHA'S OPTION, TO REVISE OR REPLACE AIROHA SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * AIROHA FOR SUCH AIROHA SOFTWARE AT ISSUE.
 */

#include <stdio.h>
#include <stdlib.h>
#include "hal_feature_config.h"
#include "hal.h"
#ifdef HAL_SLEEP_MANAGER_ENABLED
#include "hal_sleep_manager.h"
#include "hal_sleep_manager_internal.h"
#endif
#ifdef HAL_DCXO_MODULE_ENABLED
#include "hal_clock_internal.h"
#define __EOSC_32K__
unsigned int dcxo_capid = 0;

#if !defined(__EXT_BOOTLOADER__) && !defined(__EXT_DA__)
#include "dcxo_capid.h"  /* function only defined in mainbin */
#endif

void set_capid(void)
{
    /* Note: CAPID settings can only take affect during DCXO FPM */
    if (dcxo_capid) {
        DCXO_PCON_REG.DCXO_CAPID_EFUSE = dcxo_capid;
        DCXO_PCON_REG.DCXO_CAPID_EFUSE_SEL = 0x1;
        log_hal_msgid_info("[DCXO] Load FPM CAPID done, CAPID RG = 0x%x\r\n", 1, dcxo_capid);
    }
}

uint32_t get_capid(void)
{
    return dcxo_capid;
}

/**
 * internal DCXO pwr mode switch (FPM/LPM)
 * some configs require FPM/LPM switch for settings to take affect
 * this function is for this specific purpose (dcxo driver internal use only)
 */
static void _internal_dcxo_lp_mode(dcxo_mode_t mode)
{
    DCXO_PCON_REG.EN_26M_FPM = mode;
    DCXO_PCON_REG.BT_26M_EN = mode;
}

/**
 * DCXO pwr mode switch (FPM/LPM)
 * (for AIR_BTA_IC_PREMIUM_G3, maintaining dcxo as FPM is feasible)
 * -> when user calls this function, it always sets DCXO to FPM
 */
void dcxo_lp_mode(dcxo_mode_t mode)
{
    /* fixed to FPM for PREMIUM_G3
     * change parameter to "mode" for dynamic mode switching
     */
    _internal_dcxo_lp_mode(DCXO_NORMAL_MODE);
}

#if 0
/* internal API that returns current dcxo mode */
static dcxo_mode_t dcxo_current_mode(void)
{
    if (DCXO_PCON_REG.EN_26M_FPM == 0 && DCXO_PCON_REG.BT_26M_EN == 0) {
        return DCXO_LP_MODE;
    } else {
        return DCXO_NORMAL_MODE;
    }
}
#endif

/* API that checks if VBG_CAL calibration value exists, and apply it to VBC */
void set_vref_v2i_sel(void)
{
    uint8_t efuse_vbg_val = EFUSE->M_ANA_CFG_FT_BTRF1_b.VBG_CAL;
    if (efuse_vbg_val != 0) {
        /* value exists, apply setting to register (truncate efuse value to 6 bits) */
        DCXO_CFG->BGCORE_CTRL0_b.VREF_V2I_SEL = (efuse_vbg_val & 0x3F);
        log_hal_msgid_info("[DCXO] VBG_CAL value applied to VREF_V2I_SEL = 0x%x\r\n", 1, DCXO_CFG->BGCORE_CTRL0_b.VREF_V2I_SEL);
    } else {
        log_hal_msgid_info("[DCXO] Error, VBG_CAL value does not exist!\r\n", 0);
    }
}

/* API that checks if VBG_CAL calibration value exists, and apply it accordingly */
void set_vref_dcxo_sel(void)
{
    /* Apply VBG calibration results (if calibration results exist) */
    uint8_t efuse_vbg_val = EFUSE->M_ANA_CFG_FT_BTRF1_b.VBG_CAL;
    if (efuse_vbg_val != 0) {
        /* value exists, apply setting to register (truncate efuse value to 6 bits)
         * Need to switch from LPM -> FPM once for settings to apply
         */
        DCXO_CFG->BGCORE_CTRL1_b.VREF_DCXO_SEL = (efuse_vbg_val & 0x3F);
        _internal_dcxo_lp_mode(DCXO_LP_MODE);
        hal_gpt_delay_us(200);
        _internal_dcxo_lp_mode(DCXO_NORMAL_MODE);
        hal_gpt_delay_us(300);

        log_hal_msgid_info("[DCXO] VBG_CAL value applied to VREF_DCXO_SEL = 0x%x\r\n", 1, DCXO_CFG->BGCORE_CTRL1_b.VREF_DCXO_SEL);
    } else {
        log_hal_msgid_info("[DCXO] Error, VBG_CAL value does not exist!\r\n", 0);
    }
}

#if 0
/* function for debugging DCXO macro rg settings */
static void dcxo_macro_rg_dbg_dump(void)
{
    log_hal_msgid_info("RG dump 12:0x%x\r\n", 1, *(volatile uint32_t *)(DCXO_CFG_BASE + 12));
    log_hal_msgid_info("RG dump 40:0x%x\r\n", 1, *(volatile uint32_t *)(DCXO_CFG_BASE + 40));
    log_hal_msgid_info("RG dump 44:0x%x\r\n", 1, *(volatile uint32_t *)(DCXO_CFG_BASE + 44));
    log_hal_msgid_info("RG dump 48:0x%x\r\n", 1, *(volatile uint32_t *)(DCXO_CFG_BASE + 48));
    log_hal_msgid_info("RG dump 52:0x%x\r\n", 1, *(volatile uint32_t *)(DCXO_CFG_BASE + 52));
    log_hal_msgid_info("RG dump 64:0x%x\r\n", 1, *(volatile uint32_t *)(DCXO_CFG_BASE + 64));
    log_hal_msgid_info("RG dump 68:0x%x\r\n", 1, *(volatile uint32_t *)(DCXO_CFG_BASE + 68));
    log_hal_msgid_info("RG dump 72:0x%x\r\n", 1, *(volatile uint32_t *)(DCXO_CFG_BASE + 72));
    log_hal_msgid_info("RG dump 76:0x%x\r\n", 1, *(volatile uint32_t *)(DCXO_CFG_BASE + 76));
    log_hal_msgid_info("RG dump 84:0x%x\r\n", 1, *(volatile uint32_t *)(DCXO_CFG_BASE + 84));
    log_hal_msgid_info("RG dump 92:0x%x\r\n", 1, *(volatile uint32_t *)(DCXO_CFG_BASE + 92));
    log_hal_msgid_info("RG dump 156:0x%x\r\n", 1, *(volatile uint32_t *)(DCXO_CFG_BASE + 156));
}
#endif

/* DCXO MACRO RG settings update
 * (some may be overridden in later init settings)
 */
static void set_dcxo_macro_rg(void)
{
    /* DCXO_MACRO RG settings */
    if (((*(volatile uint32_t *)0x420C0208 >> 5) & 0x7) == 0x2) {
        /* E1/E2 settings */
        DCXO_CFG->DCXO_CTUNE_LPM = 0x0040;
        DCXO_CFG->DCXO_CORE_ISEL_1 = 0xFFFF;
        DCXO_CFG->DCXO_CORE_ISEL_2 = 0xFFA0;
        DCXO_CFG->DCXO_FPM_LDO2 = 0x7776;
        DCXO_CFG->DCXO_FPM_LDO3 = 0x7675;
        DCXO_CFG->DCXO_FPM_LDO4 = 0x7576;
        DCXO_CFG->BBPLL_XOPS = 0x0087;
        DCXO_CFG->DCXO_LPM_LDO = 0x0005;
        DCXO_CFG->DCXO_32KDIV_LPM = 0x1E83;
        DCXO_CFG->DCXO_MAN2 = 0x75F1;
    } else {
        /* E3 settings */
        DCXO_CFG->DCXO_CTUNE_LPM = 0x0040;
        DCXO_CFG->DCXO_CORE_ISEL_1 = 0xFFFF;
        DCXO_CFG->DCXO_CORE_ISEL_2 = 0xFFA0;
        DCXO_CFG->BGCORE_CTRL0_b.BG_FC_OFF = 0x1; /* bit default value was changed to 0 */
        DCXO_CFG->DCXO_FPM_LDO2 = 0x7676;
        DCXO_CFG->DCXO_FPM_LDO3 = 0x7675;
        DCXO_CFG->DCXO_FPM_LDO4 = 0x7575;
        DCXO_CFG->DCXO_LPM_LDO = 0x0005;
        DCXO_CFG->DCXO_32KDIV_LPM = 0x1E83;
        DCXO_CFG->DCXO_MAN2 = 0x75F1;
    }
    log_hal_msgid_info("[DCXO] LPM CAPID = 0x%x\r\n", 1, DCXO_CFG->DCXO_CTUNE_LPM);
    //dcxo_macro_rg_dbg_dump();
}


static void xops_disable(void)
{
    DCXO_CFG->DCXO_XOPS_b.RG_DCXO_XOPS_START = 0;
    DCXO_CFG->BBPLL_XOPS_b.BBPLL_XOPS_EN_PO_MAN = 0;
}

#ifdef HAL_SLEEP_MANAGER_ENABLED
void dcxo_resume_callback(void)
{
    /* Enable clock to BTRF during resume */
    DCXO_PCON_REG.BT_26M_EN = 1;
}

void dcxo_suspend_callback(void)
{
    /* Disable clock to BTRF before sleep */
    DCXO_PCON_REG.BT_26M_EN = 0;
}

#endif

/**
 * dcxo_pwr_ctrl setting
 *
 */
void hal_dcxo_init(void)
{
#if !defined(__EXT_BOOTLOADER__) && !defined(__EXT_DA__)
    dcxo_load_capid(); /* function only defined in mainbin */
#endif

    DCXO_PCON_REG.GSM_DCXO_CTL_EN = 0x1;   /* Enable baseband control */
    DCXO_PCON_REG.EXT_DCXO_CTL_EN = 0x1;   /* Enable external control */

    DCXO_PCON_REG.DCXO_CK_RDY_COMP_VREF_SEL = 0x1;   /* For DCXO DIG rg reset */
    DCXO_PCON_REG.DCXO_26M_RDY_EN = 0x0; /* Skip DCXO wakeup by 26M_DBB_CK_RDY */
    set_capid();
    set_dcxo_macro_rg();

    xops_disable();
    set_vref_v2i_sel();
    set_vref_dcxo_sel();

#ifdef __EOSC_32K__
    /* Does not use DCXO32K wakeup timing config*/
    /* Note: since wakeup timing of DCXO is a bit longer than when DCXO32K is enabled, the longer timing
     *       is also suitable for when DCXO32K is dynamically enabled/disabled
     *       (DCXO32K is currently dynamically enabled/disabled in RTC driver by setting DCXO32K_EN directly)
     */
    DCXO_PCON_REG.DCXO_PWR_EN_TD = 0x1;
    DCXO_PCON_REG.DCXO_EN_TD = 0x1;
    DCXO_PCON_REG.DCXO_BUF_EN_TD = 0x51;
#else //32kless
    /* Use DCXO32K */
    DCXO_PCON_REG.DCXO32K_EN = 0x1;
    DCXO_PCON_REG.DCXO_PWR_EN_TD = 0x1;
    DCXO_PCON_REG.DCXO_EN_TD = 0x1;
    DCXO_PCON_REG.DCXO_BUF_EN_TD = 0x14;
#endif
    DCXO_PCON_REG.DCXO_ISO_EN_TD = 0x6;
    DCXO_PCON_REG.DCXO_SLEEP_TD = 0xA;

    /* Since dcxo_init() is currently the last clock-related init
     * -> call system freq debug log
     */
    hal_clock_get_freq_meter(FQMTR_SYS_DBG, 1000);

#ifdef HAL_SLEEP_MANAGER_ENABLED
    sleep_management_register_all_secure_suspend_callback(SLEEP_BACKUP_RESTORE_DCXO_CFG, (sleep_management_suspend_callback_t)dcxo_suspend_callback, NULL);
    sleep_management_register_all_secure_resume_callback(SLEEP_BACKUP_RESTORE_DCXO_CFG, (sleep_management_resume_callback_t)dcxo_resume_callback, NULL);
#endif
}

#endif /*HAL_DCXO_MODULE_ENABLED*/
