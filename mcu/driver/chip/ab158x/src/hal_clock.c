/* Copyright Statement:
 *
 * (C) 2021  Airoha Technology Corp. All rights reserved.
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


#include "hal_clock_internal.h"
#ifdef HAL_CLOCK_MODULE_ENABLED

#include "hal_nvic_internal.h"
#include "hal_nvic.h"
#include <assert.h>
#include <stdio.h>
#include "hal_gpt.h"
#include "hal_flash_sf.h"
#include "hal.h"

#if defined(__EXT_BOOTLOADER__)
#include "bl_common.h"
#define clk_dbg_log(dbg_msg, param_cnt, ...)
#define clk_dbg_print(dbg_msg, param_cnt, ...) bl_print(LOG_DEBUG, dbg_msg, ##__VA_ARGS__)
#elif defined(__EXT_DA__)
/* need to add da env corresponding debug log function call */
#define clk_dbg_log(dbg_msg, param_cnt, ...)
#define clk_dbg_print(dbg_msg, param_cnt, ...)
#else
#include "syslog.h"
/* dbg_log, added in critical section, only enable when we need to debug stuff */
#define clk_dbg_log(dbg_msg, param_cnt, ...)
#define clk_dbg_print(dbg_msg, param_cnt, ...)     log_hal_msgid_info(dbg_msg, param_cnt, ##__VA_ARGS__)
#endif

/* Definitions to control LPOSC target calibration freq
 * LPOSC1: 312 MHz    (AIR_RFI_SUPPRESS_DISABLE defined)
 *         299.25 MHz (AIR_RFI_SUPPRESS_DISABLE NOT defined)
 * LPOSC2: 520 MHz
 */
#if defined(AIR_RFI_SUPPRESS_DISABLE)
#define LPOSC1_TARGET_KHZ   (104 * 1000 * 3)    /* 312 MHz */
#else
#define LPOSC1_TARGET_KHZ   (99.75 * 1000 * 3)  /* 299.25 MHz */
#endif /* AIR_RFI_SUPPRESS_DISABLE */
#define LPOSC2_TARGET_KHZ   (520000)            /* 520 MHz */

/* Dynamic clock gating enable/disable definition (comment out to disable) */
#define DCM_ENABLE

ATTR_RWDATA_IN_TCM static cali_info_t osc_tune_tbl[OSC_NUM][TUNE_TBL_SIZE] = {{{.scale = lposc_cali_scale, .src = AD_OSC1_D3_CK, .cali_target = (LPOSC1_TARGET_KHZ / 3)}, // OSC1
        {.scale = lposc_ft_scale,   .src = AD_OSC1_CK,   .cali_target = LPOSC1_TARGET_KHZ}
    },
    {   {.scale = lposc_cali_scale, .src = AD_OSC2_D3_CK, .cali_target = (LPOSC2_TARGET_KHZ / 3)}, //OSC2
        {.scale = lposc_ft_scale,   .src = AD_OSC2_CK,   .cali_target = LPOSC2_TARGET_KHZ}
    }
};

#define BYTE_REG(instance)      ((volatile uint8_t *)&instance)
void lposc1_enable(void);
//static void lposc1_disable(void);   /* Comment out lposc1_disable() code (currently not used) */
void lposc2_enable(void);
static void lposc2_disable(void);


#if 1
/* clock CG status debug function */
void dbg_clock_cg(void)
{
    /* XO_PDN_PD_COND0(0x42040B00) */
    clk_dbg_print("[CLK] XO_PDN_PD_COND0: 0x%x\r\n", 1, *XO_PDN_PD_COND0);
    /* XO_PDN_AO_COND0(0x42040B30) */
    clk_dbg_print("[CLK] XO_PDN_AO_COND0: 0x%x\r\n", 1, *XO_PDN_AO_COND0);
    /* XO_PDN_TOP_COND0(0x42040B60) */
    clk_dbg_print("[CLK] XO_PDN_TOP_COND0: 0x%x\r\n", 1, *XO_PDN_TOP_COND0);
    /* XO_PDN_TOP_COND1(0x42040B90) */
    clk_dbg_print("[CLK] XO_PDN_TOP_COND1: 0x%x\r\n", 1, *XO_PDN_TOP_COND1);
    /* PDN_PD_COND0(0x422B0300) */
    clk_dbg_print("[CLK] PDN_PD_COND0: 0x%x\r\n", 1, *PDN_PD_COND0);
    /* PDN_AO_COND0(0x422B0330) */
    clk_dbg_print("[CLK] PDN_AO_COND0: 0x%x\r\n", 1, *PDN_AO_COND0);
    /* PDN_TOP_COND0(0x422B0360) */
    clk_dbg_print("[CLK] PDN_TOP_COND0: 0x%x\r\n", 1, *PDN_TOP_COND0);
}
#endif
const static clock_tbl_t clock_tbl;
/* Psuedo_CG, set to all default Clock gating default enabled for now
 * should be set to enabled after 1st MUX_SEL call
 * Used for indicating if clock source is in-use
 * (must match psuedo cg list sequence defined in hal_clock_cg_id)
 */
ATTR_RWDATA_IN_TCM static uint8_t psuedo_cg_arr[HAL_CLOCK_CG_END - HAL_CLOCK_CG_PSUEDO_CLK_26M] = {
    1, // HAL_CLOCK_CG_PSUEDO_CLK_26M
    1, // HAL_CLOCK_CG_PSUEDO_CLK_OSC_26M
    1, // HAL_CLOCK_CG_PSUEDO_MCLK
};

/* array for keeping psuedo mux select option
 * MCLK: intialized to 2 to represent default clock source DCXO 26MHz
 * (must match clock_mux_sel_id list sequence defined in )
 */

ATTR_RWDATA_IN_TCM static uint8_t psuedo_mux_arr[CLK_PSUEDO_MUX_NUM] = {2, // CLK_MCLK_SEL
                                                                        0, // CLK_SPDIF_SEL
                                                                        0, // CLK_PWM0_SEL
                                                                        0, // CLK_PWM1_SEL
                                                                        0, // CLK_PWM2_SEL
                                                                        0, // CLK_PWM3_SEL
                                                                        0, // CLK_PWM4_SEL
                                                                        0, // CLK_PWM5_SEL
                                                                        0, // CLK_PWM6_SEL
                                                                        0, // CLK_PWM7_SEL
                                                                        0, // CLK_PWM8_SEL
                                                                        0, // CLK_PWM9_SEL
                                                                        0, // CLK_PWM10_SEL
                                                                        0, // CLK_PWM11_SEL
                                                                        0, // CLK_PWM12_SEL
                                                                        0  // CLK_PWM13_SEL
                                                                       };


ATTR_TEXT_IN_TCM static uint8_t clk_pll_on(clock_pll_id pll_id);
ATTR_TEXT_IN_TCM static uint8_t clk_pll_off(clock_pll_id pll_id);
ATTR_RWDATA_IN_TCM static clk_apll_freq apll_freq[NR_APLL] = {CLK_APLL_45M, CLK_APLL_49M};


uint8_t clock_set_pll_on(clock_pll_id pll_id)
{
    return 1;
}

uint8_t clock_set_pll_off(clock_pll_id pll_id)
{
    return 1;
}

void hal_clock_set_running_flags(uint32_t clock_cg_opt_set, bool on_off) {}
void clock_dump_info(void) {};


clk_usr_status clock_get_apll1_status()
{
    //return APLL1_DOMAIN_USR_EXIST;
    return exist;
}

clk_usr_status clock_get_apll2_status()
{
    //return APLL2_DOMAIN_USR_EXIST;
    return exist;
}

clk_usr_status clock_get_upll_status()
{
    //return UPLL_DOMAIN_USR_EXIST;
    return exist;
}

clk_usr_status clock_get_lposc1_status()
{
    // return LPOSC1_DOMAIN_USR_EXIST;
    return exist;
}

clk_usr_status clock_get_lposc2_status()
{
    // return LPOSC2_DOMAIN_USR_EXIST;
    return exist;
}

ATTR_TEXT_IN_TCM static void apll1_enable(void)
{
    clk_pll_on(CLK_APLL1);
}
ATTR_TEXT_IN_TCM static void apll1_disable(void)
{
    clk_pll_off(CLK_APLL1);
}
ATTR_TEXT_IN_TCM static void apll2_enable(void)
{
    clk_pll_on(CLK_APLL2);
}
ATTR_TEXT_IN_TCM static void apll2_disable(void)
{
    clk_pll_off(CLK_APLL2);
}
ATTR_TEXT_IN_TCM static void upll_disable(void)
{
    clk_pll_off(CLK_UPLL);
}
ATTR_TEXT_IN_TCM static void upll_enable(void)
{
    clk_pll_on(CLK_UPLL);
}


/* Performs mux switch for all physical and psuedo mux */
ATTR_TEXT_IN_TCM static void clock_top_mux_ctrl(clock_mux_sel_id mux_id, uint8_t target_sel)
{
    volatile uint8_t *sel = NULL;
    volatile uint8_t *chg = NULL;
    volatile uint8_t *chg_ok = NULL;
    volatile uint8_t *force_on = NULL;
    switch (mux_id) {
        case CLK_SYS_SEL:
            sel =      BYTE_REG(CKSYS_CLK_CFG_REG.CLK_SYS_SEL);
            force_on = BYTE_REG(CKSYS_CLK_FORCE_ON_REG.CLK_SYS_FORCE_ON);
            chg =      BYTE_REG(CKSYS_CLK_UPDATE_REG.CHG_SYS);
            chg_ok =   BYTE_REG(CKSYS_CLK_UPDATE_STATUS_REG.CLK_SYS_OK);
            break;
        case CLK_DSP_SEL:
            sel =      BYTE_REG(CKSYS_CLK_CFG_REG.CLK_DSP_SEL);
            force_on = BYTE_REG(CKSYS_CLK_FORCE_ON_REG.CLK_DSP_FORCE_ON);
            chg =      BYTE_REG(CKSYS_CLK_UPDATE_REG.CHG_DSP);
            chg_ok =   BYTE_REG(CKSYS_CLK_UPDATE_STATUS_REG.CLK_DSP_OK);
            break;
        case CLK_SFC_SEL:
            sel =      BYTE_REG(CKSYS_CLK_CFG_REG.CLK_SFC_SEL);
            force_on = BYTE_REG(CKSYS_CLK_FORCE_ON_REG.CLK_SFC_FORCE_ON);
            chg =      BYTE_REG(CKSYS_CLK_UPDATE_REG.CHG_SFC);
            chg_ok =   BYTE_REG(CKSYS_CLK_UPDATE_STATUS_REG.CLK_SFC_OK);
            break;
        case CLK_ESC_SEL:
            sel =      BYTE_REG(CKSYS_CLK_CFG_REG.CLK_ESC_SEL);
            force_on = BYTE_REG(CKSYS_CLK_FORCE_ON_REG.CLK_ESC_FORCE_ON);
            chg =      BYTE_REG(CKSYS_CLK_UPDATE_REG.CHG_ESC);
            chg_ok =   BYTE_REG(CKSYS_CLK_UPDATE_STATUS_REG.CLK_ESC_OK);
            break;
        case CLK_SPIMST0_SEL:
            sel =      BYTE_REG(CKSYS_CLK_CFG_REG.CLK_SPIMST0_SEL);
            force_on = BYTE_REG(CKSYS_CLK_FORCE_ON_REG.CLK_SPIMST0_FORCE_ON);
            chg =      BYTE_REG(CKSYS_CLK_UPDATE_REG.CHG_SPIMST0);
            chg_ok =   BYTE_REG(CKSYS_CLK_UPDATE_STATUS_REG.CLK_SPIMST0_OK);
            break;
        case CLK_SPIMST1_SEL:
            sel =      BYTE_REG(CKSYS_CLK_CFG_REG.CLK_SPIMST1_SEL);
            force_on = BYTE_REG(CKSYS_CLK_FORCE_ON_REG.CLK_SPIMST1_FORCE_ON);
            chg =      BYTE_REG(CKSYS_CLK_UPDATE_REG.CHG_SPIMST1);
            chg_ok =   BYTE_REG(CKSYS_CLK_UPDATE_STATUS_REG.CLK_SPIMST1_OK);
            break;
        case CLK_SPIMST2_SEL:
            sel =      BYTE_REG(CKSYS_CLK_CFG_REG.CLK_SPIMST2_SEL);
            force_on = BYTE_REG(CKSYS_CLK_FORCE_ON_REG.CLK_SPIMST2_FORCE_ON);
            chg =      BYTE_REG(CKSYS_CLK_UPDATE_REG.CHG_SPIMST2);
            chg_ok =   BYTE_REG(CKSYS_CLK_UPDATE_STATUS_REG.CLK_SPIMST2_OK);
            break;
        case CLK_SPISLV_SEL:
            sel =      BYTE_REG(CKSYS_CLK_CFG_REG.CLK_SPISLV_SEL);
            force_on = BYTE_REG(CKSYS_CLK_FORCE_ON_REG.CLK_SPISLV_FORCE_ON);
            chg =      BYTE_REG(CKSYS_CLK_UPDATE_REG.CHG_SPISLV);
            chg_ok =   BYTE_REG(CKSYS_CLK_UPDATE_STATUS_REG.CLK_SPISLV_OK);
            break;
        case CLK_SDIOMST0_SEL:
            sel =      BYTE_REG(CKSYS_CLK_CFG_REG.CLK_SDIOMST0_SEL);
            force_on = BYTE_REG(CKSYS_CLK_FORCE_ON_REG.CLK_SDIOMST0_FORCE_ON);
            chg =      BYTE_REG(CKSYS_CLK_UPDATE_REG.CHG_SDIOMST0);
            chg_ok =   BYTE_REG(CKSYS_CLK_UPDATE_STATUS_REG.CLK_SDIOMST0_OK);
            break;
        case CLK_USB_SEL:
            sel =      BYTE_REG(CKSYS_CLK_CFG_REG.CLK_USB_SEL);
            force_on = BYTE_REG(CKSYS_CLK_FORCE_ON_REG.CLK_USB_FORCE_ON);
            chg =      BYTE_REG(CKSYS_CLK_UPDATE_REG.CHG_USB);
            chg_ok =   BYTE_REG(CKSYS_CLK_UPDATE_STATUS_REG.CLK_USB_OK);
            break;
        case CLK_I3C_SEL:
            sel =      BYTE_REG(CKSYS_CLK_CFG_REG.CLK_I3C_SEL);
            force_on = BYTE_REG(CKSYS_CLK_FORCE_ON_REG.CLK_I3C_FORCE_ON);
            chg =      BYTE_REG(CKSYS_CLK_UPDATE_REG.CHG_I3C);
            chg_ok =   BYTE_REG(CKSYS_CLK_UPDATE_STATUS_REG.CLK_I3C_OK);
            break;
        case CLK_BT_HOP_SEL:
            sel =      BYTE_REG(CKSYS_CLK_CFG_REG.CLK_BT_HOP_SEL);
            force_on = BYTE_REG(CKSYS_CLK_FORCE_ON_REG.CLK_BT_HOP_FORCE_ON);
            chg =      BYTE_REG(CKSYS_CLK_UPDATE_REG.CHG_BT_HOP);
            chg_ok =   BYTE_REG(CKSYS_CLK_UPDATE_STATUS_REG.CLK_BT_HOP_OK);
            break;
        case CLK_AUD_BUS_SEL:
            sel =      BYTE_REG(CKSYS_CLK_CFG_REG.CLK_AUD_BUS_SEL);
            force_on = BYTE_REG(CKSYS_CLK_FORCE_ON_REG.CLK_AUD_BUS_FORCE_ON);
            chg =      BYTE_REG(CKSYS_CLK_UPDATE_REG.CHG_AUD_BUS);
            chg_ok =   BYTE_REG(CKSYS_CLK_UPDATE_STATUS_REG.CLK_AUD_BUS_OK);
            break;
        case CLK_AUD_GPSRC_SEL:
            sel =      BYTE_REG(CKSYS_CLK_CFG_REG.CLK_AUD_GPSRC_SEL);
            force_on = BYTE_REG(CKSYS_CLK_FORCE_ON_REG.CLK_AUD_GPSRC_FORCE_ON);
            chg =      BYTE_REG(CKSYS_CLK_UPDATE_REG.CHG_AUD_GPSRC);
            chg_ok =   BYTE_REG(CKSYS_CLK_UPDATE_STATUS_REG.CLK_AUD_GPSRC_OK);
            break;
        case CLK_AUD_ULCK_SEL:
            sel =      BYTE_REG(CKSYS_CLK_CFG_REG.CLK_AUD_ULCK_SEL);
            force_on = BYTE_REG(CKSYS_CLK_FORCE_ON_REG.CLK_AUD_ULCK_FORCE_ON);
            chg =      BYTE_REG(CKSYS_CLK_UPDATE_REG.CHG_AUD_ULCK);
            chg_ok =   BYTE_REG(CKSYS_CLK_UPDATE_STATUS_REG.CLK_AUD_ULCK_OK);
            break;
        case CLK_AUD_DLCK_SEL:
            sel =      BYTE_REG(CKSYS_CLK_CFG_REG.CLK_AUD_DLCK_SEL);
            force_on = BYTE_REG(CKSYS_CLK_FORCE_ON_REG.CLK_AUD_DLCK_FORCE_ON);
            chg =      BYTE_REG(CKSYS_CLK_UPDATE_REG.CHG_AUD_DLCK);
            chg_ok =   BYTE_REG(CKSYS_CLK_UPDATE_STATUS_REG.CLK_AUD_DLCK_OK);
            break;

        case CLK_AUD_INTERFACE0_SEL:
            sel =      BYTE_REG(CKSYS_CLK_CFG_REG.CLK_AUD_INTF0_SEL);
            break;
        case CLK_AUD_INTERFACE1_SEL:
            sel =      BYTE_REG(CKSYS_CLK_CFG_REG.CLK_AUD_INTF1_SEL);
            break;
        case CLK_26M_SEL:
            sel =      BYTE_REG(CKSYS_CLK_CFG_REG.CLK_26M_SEL);
            break;
        case CLK_VOW_SEL:
            sel =      BYTE_REG(CKSYS_CLK_CFG_REG.CLK_VOW_SEL);
            break;
        case CLK_AUD_ENGINE_SEL:
            sel =      BYTE_REG(CKSYS_CLK_CFG_REG.CLK_AUD_ENGINE_SEL);
            break;
        case CLK_OSC_26M_SEL:
            sel =      BYTE_REG(CKSYS_CLK_CFG_REG.CLK_OSC_26M_SEL);
            break;

        /* Below are psuedo muxes */
        case CLK_MCLK_SEL:
        case CLK_SPDIF_SEL:
        case CLK_PWM0_SEL:
        case CLK_PWM1_SEL:
        case CLK_PWM2_SEL:
        case CLK_PWM3_SEL:
        case CLK_PWM4_SEL:
        case CLK_PWM5_SEL:
        case CLK_PWM6_SEL:
        case CLK_PWM7_SEL:
        case CLK_PWM8_SEL:
        case CLK_PWM9_SEL:
        case CLK_PWM10_SEL:
        case CLK_PWM11_SEL:
        case CLK_PWM12_SEL:
        case CLK_PWM13_SEL:
            sel =      &psuedo_mux_arr[mux_id - CLK_MCLK_SEL];
            break;
        default:
            return;
    }
    // n to 1
    if (sel) {
        *sel = target_sel;
    }
    if (force_on) {
        *force_on = 1;
    }
#ifndef FPGA_ENV
    if (chg) {
        uint32_t counter = 0; /* counter for debugging when mux switch hangs */
        *chg = 1;
        while (*chg) {
            if (counter > 1000) {
                clk_dbg_print("while chg, counter %d!\r\n", 1, counter);
            }
            counter++;
        }
    }
    if (chg_ok) {
        while (*chg_ok) {
            uint32_t counter = 0; /* counter for debugging when mux switch hangs */
            if (counter > 1000) {
                clk_dbg_print("while chg_ok, counter %d!\r\n", 1, counter);
            }
            counter++;
        }
    }
#endif
    if (force_on) {
        *force_on = 0;
    }

#if !defined(__EXT_BOOTLOADER__) && !defined(__EXT_DA__)
    if (mux_id == CLK_SYS_SEL) {
        /* Notify FreeRTOS CPU's clk freq setting change */
        SystemCoreClockUpdate();
    }
#endif
}

/*************************************************************************
 * Clock mux select API definition part
 * 1. Enable individual clock divider
 * 2. Force clock on th prevent clock can't switch to target clock
 * 3. Set CSW to target clock freq. and set change bit
 * 4. After clock change to target freq. Change bit will be cleared to0 and release clock gating
 * 5. Disable forced on clock
 *************************************************************************/

/* Call corresponding clock source enable functions
 * If clock sources are already enabled, it should return directly
 */
ATTR_TEXT_IN_TCM static hal_clock_status_t clock_src_enable(hal_mux_t sels)
{
    switch (sels.src) {
        case CLK_XO:
            /* DCXO is currently enabled by default */
            break;
        case CLK_OSC1:
            lposc1_enable();
            break;
        case CLK_OSC2:
            lposc2_enable();
            break;
        case CLK_APLL1:
            apll1_enable();
            break;
        case CLK_APLL2:
            apll2_enable();
            break;
        case CLK_UPLL:
            upll_enable();
            break;
        default:
            break;
    }
    return HAL_CLOCK_STATUS_OK;
}


ATTR_TEXT_IN_TCM static hal_clock_status_t clock_div_enable(hal_mux_t sels)
{
    clk_dbg_log("clock_div_enable src %d, div %d\r\n", 2, sels.src, sels.div);
    /* Only LPOSC1, LPOSC2, and UPLL dividers are currently dynamically enable/disabled */
    if (sels.src == CLK_OSC1) {
        CKSYS_CLK_DIV_REG.CLK_OSC1_DIV_EN = 0x1;
        if (sels.div == CLK_DIV_D2) {
            CKSYS_CLK_DIV_REG.CLK_OSC1_D2_EN = 0x1;
        } else if (sels.div == CLK_DIV_D3) {
            CKSYS_CLK_DIV_REG.CLK_OSC1_D3_EN = 0x1;
        } else if (sels.div == CLK_DIV_D5) {
            CKSYS_CLK_DIV_REG.CLK_OSC1_D5_EN = 0x1;
        } else if (sels.div == CLK_DIV_D12) {
            CKSYS_CLK_DIV_REG.CLK_OSC1_D12_EN = 0x1;
        } else if (sels.div == CLK_DIV_D8) {
            CKSYS_CLK_DIV_REG.CLK_OSC1_D8_EN = 0x1;
        }
        /* CLK_DIV_NONE 0xFF means divide by 1 */
    } else if (sels.src == CLK_OSC2) {
        CKSYS_CLK_DIV_REG.CLK_OSC2_DIV_EN = 0x1;
        if (sels.div == CLK_DIV_D2) {
            CKSYS_CLK_DIV_REG.CLK_OSC2_D2_EN = 0x1;
        } else if (sels.div == CLK_DIV_D3) {
            CKSYS_CLK_DIV_REG.CLK_OSC2_D3_EN = 0x1;
        } else if (sels.div == CLK_DIV_D5) {
            CKSYS_CLK_DIV_REG.CLK_OSC2_D5_EN = 0x1;
        } else if (sels.div == CLK_DIV_D12) {
            CKSYS_CLK_DIV_REG.CLK_OSC2_D12_EN = 0x1;
        }
        /* CLK_DIV_NONE 0xFF means divide by 1 */
    } else if (sels.src == CLK_UPLL) {
        CKSYS_CLK_DIV_REG.CLK_PLL1_DIV_EN = 0x1;
        if (sels.div == CLK_DIV_D2) {
            CKSYS_CLK_DIV_REG.CLK_PLL1_D2_EN = 0x1;
        } else if (sels.div == CLK_DIV_D3) {
            CKSYS_CLK_DIV_REG.CLK_PLL1_D3_EN = 0x1;
        } else if (sels.div == CLK_DIV_D5) {
            CKSYS_CLK_DIV_REG.CLK_PLL1_D5_EN = 0x1;
        }
        /* CLK_DIV_NONE 0xFF means UPLL 624M for now */
    }
    return HAL_CLOCK_STATUS_OK;
}


ATTR_TEXT_IN_TCM static hal_clock_status_t clock_div_disable(hal_mux_t sels)
{
    clk_dbg_log("clock_div_disable src %d, div %d\r\n", 2, sels.src, sels.div);
    /* Only LPOSC1, LPOSC2, and UPLL dividers are currently dynamically enable/disabled
     * master divider enable/disable is not disabled for now
     */
    if (sels.src == CLK_OSC1) {
        //CKSYS_CLK_DIV_REG.CLK_OSC1_DIV_EN = 0x0;
        if (sels.div == CLK_DIV_D2) {
            CKSYS_CLK_DIV_REG.CLK_OSC1_D2_EN = 0x0;
        } else if (sels.div == CLK_DIV_D3) {
            CKSYS_CLK_DIV_REG.CLK_OSC1_D3_EN = 0x0;
        } else if (sels.div == CLK_DIV_D5) {
            CKSYS_CLK_DIV_REG.CLK_OSC1_D5_EN = 0x0;
        } else if (sels.div == CLK_DIV_D12) {
            CKSYS_CLK_DIV_REG.CLK_OSC1_D12_EN = 0x0;
        } else if (sels.div == CLK_DIV_D8) {
            CKSYS_CLK_DIV_REG.CLK_OSC1_D8_EN = 0x0;
        }
        /* CLK_DIV_NONE 0xFF means divide by 1 */
    } else if (sels.src == CLK_OSC2) {
        //CKSYS_CLK_DIV_REG.CLK_OSC2_DIV_EN = 0x0;
        if (sels.div == CLK_DIV_D2) {
            CKSYS_CLK_DIV_REG.CLK_OSC2_D2_EN = 0x0;
        } else if (sels.div == CLK_DIV_D3) {
            CKSYS_CLK_DIV_REG.CLK_OSC2_D3_EN = 0x0;
        } else if (sels.div == CLK_DIV_D5) {
            CKSYS_CLK_DIV_REG.CLK_OSC2_D5_EN = 0x0;
        } else if (sels.div == CLK_DIV_D12) {
            CKSYS_CLK_DIV_REG.CLK_OSC2_D12_EN = 0x0;
        }
        /* CLK_DIV_NONE 0xFF means divide by 1 */
    } else if (sels.src == CLK_UPLL) {
        //CKSYS_CLK_DIV_REG.CLK_PLL1_DIV_EN = 0x0;
        if (sels.div == CLK_DIV_D2) {
            CKSYS_CLK_DIV_REG.CLK_PLL1_D2_EN = 0x0;
        } else if (sels.div == CLK_DIV_D3) {
            CKSYS_CLK_DIV_REG.CLK_PLL1_D3_EN = 0x0;
        } else if (sels.div == CLK_DIV_D5) {
            CKSYS_CLK_DIV_REG.CLK_PLL1_D5_EN = 0x0;
        }
        /* CLK_DIV_NONE 0xFF means UPLL 624M for now */
    }
    return HAL_CLOCK_STATUS_OK;
}

/* Make sure current, and next clock source/divder have been enabled */
ATTR_TEXT_IN_TCM static hal_clock_status_t mux_switch_preproc(clock_mux_sel_id mux_id, uint32_t next_mux_sel)
{
    //clk_dbg_log("mux_switch_preproc\r\n", 0);
    /* N-to-1 mux requires both original, and next clock source/dividers both be enabled */
    if (clock_tbl.mux_tbl[mux_id].mux_type == PHYSICAL_N_TO_1_MUX) {
        /* enable current clock source, divider */
        uint8_t curr_sel = *(volatile uint8_t *)(clock_tbl.mux_tbl[mux_id].curr_sel_ptr);
        hal_mux_t curr_mux_info = clock_tbl.mux_tbl[mux_id].sels[curr_sel];
        clock_src_enable(curr_mux_info);
        clock_div_enable(curr_mux_info);
    }

    /* enable next clock source, divider */
    hal_mux_t next_mux_info = clock_tbl.mux_tbl[mux_id].sels[next_mux_sel];
    clock_src_enable(next_mux_info);
    clock_div_enable(next_mux_info);

    /* process psuedo CG */
    if (clock_tbl.mux_tbl[mux_id].cg_info.cg_type == PSUEDO_CG) {
        /* nobody actually calls enable/disable psuedo_cg
         * just used as an indication whether or not clock resource need to be enable/disabled during post process
         * (PSUEDO CG is set to on(clock disabled by default), and will maintain on status for now)
         */
        if (clock_tbl.mux_tbl[mux_id].cg_info.cg_id > HAL_CLOCK_CG_PHYS_NUM && clock_tbl.mux_tbl[mux_id].cg_info.cg_id < HAL_CLOCK_CG_END) {
            uint32_t psuedo_cg_arr_idx = clock_tbl.mux_tbl[mux_id].cg_info.cg_id - HAL_CLOCK_CG_PSUEDO_CLK_26M;
            psuedo_cg_arr[psuedo_cg_arr_idx] = 0; /* disable psuedo cg */
        } else {
            /* check if psuedo cg_id is in expected range (report error if unexpected occurs) */
            clk_dbg_print("unexpected psuedo cg_id %d", 1, clock_tbl.mux_tbl[mux_id].cg_info.cg_id);
        }
    }

    return HAL_CLOCK_STATUS_OK;
}

/* perform mux switch */
ATTR_TEXT_IN_TCM static hal_clock_status_t mux_switch_proc(clock_mux_sel_id mux_id, uint32_t mux_sel)
{
    clk_dbg_log("mux_switch_proc mux_id %d, mux_sel %d", 2, mux_id, mux_sel);
    clock_top_mux_ctrl(mux_id, mux_sel);

    return HAL_CLOCK_STATUS_OK;
}

/* Disable unused resources clock source/divider */
ATTR_TEXT_IN_TCM static hal_clock_status_t mux_switch_postproc(void)
{
    clk_dbg_log("mux_switch_postproc\r\n", 0);
    clock_resource_chk_t curr_chk = { .src_chk = { .lposc1 = 0, .lposc2 = 0, .upll = 0, .apll1 = 0, .apll2 = 0},
                                      .lposc1_div_chk = { .d2 = 0, .d3 = 0, .d5 = 0, .d12 = 0, .d8 = 0},
                                      .lposc2_div_chk = { .d2 = 0, .d3 = 0, .d5 = 0, .d12 = 0, .d8 = 0},
                                      .upll_div_chk = { .d2 = 0, .d3 = 0, .d5 = 0, .d12 = 0, .d8 = 0},
                                    };
    /* loop through whole table, and check if each clk source, divider have any users
     * We only need to know if each divider, clock source have any users (we don't need the actual count)
     * Note: CG status is also checked, if CG is enabled, we wont increment count during reference count
     */
    /* Check if the clock_id has a corresponding mux that need to be switched
     * If one exists -> call mux_sel to trigger clock source/divider enable etc..
     * TODO: find a way to shorten the below code
     */
    for (uint32_t mux_tbl_idx = 0; mux_tbl_idx < TOTAL_MUX_CNT; mux_tbl_idx++) {
        uint8_t curr_sel_num = *((volatile uint8_t *)clock_tbl.mux_tbl[mux_tbl_idx].curr_sel_ptr); /* ptr that points to current selected num */
        hal_cg_t cg_info = clock_tbl.mux_tbl[mux_tbl_idx].cg_info;
        if (hal_clock_is_enabled(cg_info.cg_id)) {
            /* clock is enabled, start to check clock source, divider */
            hal_mux_t sels = clock_tbl.mux_tbl[mux_tbl_idx].sels[curr_sel_num];
            if (sels.src == CLK_OSC1) {
                curr_chk.src_chk.lposc1 = 1;
                //CKSYS_CLK_DIV_REG.CLK_OSC1_DIV_EN = 0x0;
                if (sels.div == CLK_DIV_D2) {
                    curr_chk.lposc1_div_chk.d2 = 1;
                } else if (sels.div == CLK_DIV_D3) {
                    curr_chk.lposc1_div_chk.d3 = 1;
                } else if (sels.div == CLK_DIV_D5) {
                    curr_chk.lposc1_div_chk.d5 = 1;
                } else if (sels.div == CLK_DIV_D12) {
                    curr_chk.lposc1_div_chk.d12 = 1;
                } else if (sels.div == CLK_DIV_D8) {
                    curr_chk.lposc1_div_chk.d8 = 1;
                }
                /* CLK_DIV_NONE 0xFF means divide by 1 */
            } else if (sels.src == CLK_OSC2) {
                curr_chk.src_chk.lposc2 = 1;
                //CKSYS_CLK_DIV_REG.CLK_OSC2_DIV_EN = 0x0;
                if (sels.div == CLK_DIV_D2) {
                    curr_chk.lposc2_div_chk.d2 = 1;
                } else if (sels.div == CLK_DIV_D3) {
                    curr_chk.lposc2_div_chk.d3 = 1;
                } else if (sels.div == CLK_DIV_D5) {
                    curr_chk.lposc2_div_chk.d5 = 1;
                } else if (sels.div == CLK_DIV_D12) {
                    curr_chk.lposc2_div_chk.d12 = 1;
                }
                /* CLK_DIV_NONE 0xFF means divide by 1 */
            } else if (sels.src == CLK_UPLL) {
                curr_chk.src_chk.upll = 1;
                if (sels.div == CLK_DIV_D2) {
                    curr_chk.upll_div_chk.d2 = 1;
                } else if (sels.div == CLK_DIV_D3) {
                    curr_chk.upll_div_chk.d3 = 1;
                } else if (sels.div == CLK_DIV_D5) {
                    curr_chk.upll_div_chk.d5 = 1;
                }
                /* CLK_DIV_NONE 0xFF means UPLL 624M for now */
            } else if (sels.src == CLK_APLL1) {
                curr_chk.src_chk.apll1 = 1;
            } else if (sels.src == CLK_APLL2) {
                curr_chk.src_chk.apll2 = 1;
            }
        }
    }

    /* Use the above resource check results, and disable unused clock resource */
    /* LPOSC1 chk */
    if (curr_chk.src_chk.lposc1 == 0) {
        /* Do not disable LPOSC1 for this generation of chips (for SPM clock control) */
    }
    if (curr_chk.lposc1_div_chk.d2 == 0) {
        clock_div_disable((hal_mux_t) {
            .src = CLK_OSC1, .div = CLK_DIV_D2
        });
    }
    if (curr_chk.lposc1_div_chk.d3 == 0) {
        clock_div_disable((hal_mux_t) {
            .src = CLK_OSC1, .div = CLK_DIV_D3
        });
    }
    if (curr_chk.lposc1_div_chk.d5 == 0) {
        clock_div_disable((hal_mux_t) {
            .src = CLK_OSC1, .div = CLK_DIV_D5
        });
    }
    if (curr_chk.lposc1_div_chk.d12 == 0) {
        clock_div_disable((hal_mux_t) {
            .src = CLK_OSC1, .div = CLK_DIV_D12
        });
    }
    if (curr_chk.lposc1_div_chk.d8 == 0) {
        clock_div_disable((hal_mux_t) {
            .src = CLK_OSC1, .div = CLK_DIV_D8
        });
    }

    /* LPOSC2 chk */
    if (curr_chk.src_chk.lposc2 == 0) {
        lposc2_disable();
    }
    if (curr_chk.lposc2_div_chk.d2 == 0) {
        clock_div_disable((hal_mux_t) {
            .src = CLK_OSC2, .div = CLK_DIV_D2
        });
    }
    if (curr_chk.lposc2_div_chk.d3 == 0) {
        clock_div_disable((hal_mux_t) {
            .src = CLK_OSC2, .div = CLK_DIV_D3
        });
    }
    if (curr_chk.lposc2_div_chk.d5 == 0) {
        clock_div_disable((hal_mux_t) {
            .src = CLK_OSC2, .div = CLK_DIV_D5
        });
    }
    if (curr_chk.lposc2_div_chk.d12 == 0) {
        clock_div_disable((hal_mux_t) {
            .src = CLK_OSC2, .div = CLK_DIV_D12
        });
    }

    /* UPLL chk */
    if (curr_chk.src_chk.upll == 0) {
        upll_disable();
    }
    if (curr_chk.upll_div_chk.d2 == 0) {
        clock_div_disable((hal_mux_t) {
            .src = CLK_UPLL, .div = CLK_DIV_D2
        });
    }
    if (curr_chk.upll_div_chk.d3 == 0) {
        clock_div_disable((hal_mux_t) {
            .src = CLK_UPLL, .div = CLK_DIV_D3
        });
    }
    if (curr_chk.upll_div_chk.d5 == 0) {
        clock_div_disable((hal_mux_t) {
            .src = CLK_UPLL, .div = CLK_DIV_D5
        });
    }
    /* APLL1 chk */
    if (curr_chk.src_chk.apll1 == 0) {
        apll1_disable();
    }
    /* APLL2 chk */
    if (curr_chk.src_chk.apll2 == 0) {
        apll2_disable();
    }
    return HAL_CLOCK_STATUS_OK;
}

/* This function assumes mux_id is already checked and valid */
ATTR_TEXT_IN_TCM static hal_clock_status_t clock_mux_allow_chk(clock_mux_sel_id mux_id, uint32_t mux_sel)
{
    uint32_t mux_tbl_idx = mux_id; /* tbl_idx actually equals mux_id */

    if (clock_tbl.mux_tbl[mux_tbl_idx].sels[mux_sel].mux_allow == MUX_NOT_ALLOW) {
        return HAL_CLOCK_STATUS_ERROR;
    } else {
        return HAL_CLOCK_STATUS_OK;
    }
}

ATTR_TEXT_IN_TCM hal_clock_status_t clock_mux_sel(clock_mux_sel_id mux_id, uint32_t mux_sel)
{

    hal_clock_status_t result = HAL_CLOCK_STATUS_OK;

    clk_dbg_log("clock_mux_sel mux_id %d, mux_sel %d\r\n", 2, mux_id, mux_sel);

    /* Need to check if next mux is allowed for switching */

    result = clock_mux_allow_chk(mux_id, mux_sel);
    if (result != HAL_CLOCK_STATUS_OK) {
        clk_dbg_print("clock_mux_sel error!\r\n mux_id %d, mux_sel %d not allowed", 2, mux_id, mux_sel);
        return result;
    }
#if !defined(__EXT_BOOTLOADER__) && !defined(__EXT_DA__)
    uint32_t irq_mask = 0;
    hal_nvic_save_and_set_interrupt_mask_special(&irq_mask);
    /* ================ Critical Section Start ======================== */
#endif
    /* Make sure current, and next clock source/divder have been enabled */
    mux_switch_preproc(mux_id, mux_sel);

    /* perform mux switch */
    mux_switch_proc(mux_id, mux_sel);

    /* Disable unused resources clock source/divider */
    mux_switch_postproc();

#if !defined(__EXT_BOOTLOADER__) && !defined(__EXT_DA__)
    /* ================ Critical Section End ======================== */
    hal_nvic_restore_interrupt_mask_special(irq_mask);
#endif
    return result;
}


/* returns current selected mux num */
ATTR_TEXT_IN_TCM uint8_t clock_mux_cur_sel(clock_mux_sel_id mux_id)
{
    if (mux_id < TOTAL_MUX_CNT) {
        uint32_t mux_tbl_idx = mux_id; /* tbl_idx actually equals mux_id */
        uint8_t curr_sel = *(volatile uint8_t *)(clock_tbl.mux_tbl[mux_tbl_idx].curr_sel_ptr);
        return curr_sel;
    } else {
        clk_dbg_print("clock_mux_cur_sel() incorrect parameter, mux_id %d\r\n", 1, mux_id);
        return 0;
    }
}




ATTR_TEXT_IN_TCM uint32_t clock_freq_meter(ref_clock REF_CLK, hal_src_clock SRC_CLK, uint32_t winset, uint32_t REF_CLK_frequency)
{
    PLL_ABIST_FQMTR_COM_REG.FQMTR_EN = 0x0;
    PLL_ABIST_FQMTR_COM_REG.FQMTR_RST = 0x1;
    PLL_ABIST_FQMTR_COM_REG.FQMTR_RST = 0x0;
    *CKSYS_TST_SEL_1 = (REF_CLK << 8) + SRC_CLK;
    PLL_ABIST_FQMTR_COM_REG.FQMTR_WINSET = winset;
    PLL_ABIST_FQMTR_COM_REG.FQMTR_EN = 0x1;
    hal_gpt_delay_us(5);
#ifndef FPGA_ENV
    while (PLL_ABIST_FQMTR_COM_REG.PLL_ABIST_FQMTR_CON1_ & 0x80);
#endif
    uint32_t freq_meter  = REF_CLK_frequency * (*PLL_ABIST_FQMTR_DATA) / (winset + 1);
    clk_dbg_log("freq_meter_data 0x%x(%d)\r\n", 1, *PLL_ABIST_FQMTR_DATA, *PLL_ABIST_FQMTR_DATA);
    return freq_meter;
}

/* Estimate winset setting (currently returns accuracy of 0.01%) target winset value
 * Function is currently designed for 26MHz as reference clk
 */
ATTR_TEXT_IN_TCM static uint32_t fqmtr_winset_estimate(uint32_t target_freq_hz)
{
    uint32_t winset;
    /* data 40000: targets fqmtr to give an frequency measurement accuracy of 0.01% */
    const uint32_t data = 40000, src_freq_hz = 26 * 1000 * 1000;
    /* typecasted to 64 bit uint (to avoid multiplication overflow) */
    winset = (((uint64_t)src_freq_hz) * data) / target_freq_hz;
    winset--;

    return winset;
}

ATTR_TEXT_IN_TCM uint32_t hal_clock_get_freq_meter(hal_src_clock SRC_CLK, uint32_t winset)
{
    uint32_t ret_val = 0;
    static const uint32_t src_khz = 26000; /* DCXO 26MHz */

    if (SRC_CLK < FQMTR_SYS_DBG) {
        /* normal freq_meter API request */
        ret_val = clock_freq_meter(XO_CK, SRC_CLK, winset, src_khz);
    } else if (SRC_CLK == FQMTR_SYS_DBG) {
        /* debug use, prints CPU, DSP, SFC freqs */
        clk_dbg_print("CM33 [%d]KHz, DSP [%d] KHz, SFC [%d]KHz\r\n", 3,
                      (int)clock_freq_meter(XO_CK, hf_fsys_ck, winset, src_khz),
                      (int)clock_freq_meter(XO_CK, hf_fdsp_ck, winset, src_khz),
                      (int)clock_freq_meter(XO_CK, hf_fsfc_ck, winset, src_khz));
    }

    return ret_val;
}

#define LPOSC_CON_REG(rg)  (rg->field)


/* LPOSC calibration code, designed according to programming guide
 * Calibration actual freq will be smaller than target freq
 */
#define RBANK_CALI_MSB   (0x20)
#define RBANK_CALI_FT_MSB (0x8)
#define LPOSC1_RBANK_CALI_DEFAULT 0x28
#define LPOSC2_RBANK_CALI_DEFAULT 0x28
#define LPOSC1_RBANK_FT 0x0
#define LPOSC2_RBANK_FT 0x0
/**
 * This function should only be used for 1st time boot LPOSC calibration (when system is running off DCXO)
 * System will hang (due to LPOSC gating during calibration) if LPOSC is in use
 * Note: code flow assumes coarse cali, and FT cali are both called in order (will reset FT value in coarse cali flow)
 */
ATTR_TEXT_IN_TCM static void lposc_cali(osc_id osc, uint8_t task)
{
    uint8_t i, lposc_cali_ft_val;
    uint32_t fqmtr_data, target_khz, winset;
    volatile lposc_con *p_lposc_reg = NULL;
    volatile uint8_t *lposc_rdy = NULL, *lposc_cali_ft_reg;
    cali_info_t *cali_ctx = NULL;

    /* register address/field selection */
    if (osc == LPOSC1) {
        p_lposc_reg = ((lposc_con *)LPOSC1_CON0);
        lposc_rdy = (volatile uint8_t *)&CLKSQ_CON_REG.RG_LPOSC1_RDY;
    } else {
        p_lposc_reg = ((lposc_con *)LPOSC2_CON0);
        lposc_rdy = (volatile uint8_t *)&CLKSQ_CON_REG.RG_LPOSC2_RDY;
    }

    /* specify task related value settings */
    if (task == COARSE_CALI) {
        i = RBANK_CALI_MSB;
        lposc_cali_ft_val = RBANK_CALI_MSB;
        lposc_cali_ft_reg = &LPOSC_CON_REG(p_lposc_reg).DA_LPOSC_RBANK_CALI;
        LPOSC_CON_REG(p_lposc_reg).DA_LPOSC_RBANK_FT = 0; /* reset FT register field before coarse calibration */
        cali_ctx = &osc_tune_tbl[osc][0]; /* retrieve coarse calibration configs */
    } else { /* FINE_CALI */
        i = RBANK_CALI_FT_MSB;
        lposc_cali_ft_val = RBANK_CALI_FT_MSB;
        lposc_cali_ft_reg = &LPOSC_CON_REG(p_lposc_reg).DA_LPOSC_RBANK_FT;
        cali_ctx = &osc_tune_tbl[osc][1];  /* retrieve fine tune calibration configs */
    }

    target_khz = cali_ctx->cali_target;
    winset = fqmtr_winset_estimate(target_khz * 1000);
    clk_dbg_log("winset val %d\r\n", 1, winset);

    /*
     * actual calibration (selects a bit for each loop, starting from MSB to LSB)
     * algorithm originated from programming guide
     */
    while (i > 0) {
        *lposc_rdy = 0x0; /* enable LPOSC clock gating */

        *lposc_cali_ft_reg = lposc_cali_ft_val; /* assign value to cali or ft register field */

        hal_gpt_delay_us(20); /* Wait 20us for LPOSC stable */

        *lposc_rdy = 0x1; /* disable LPOSC clock gating */

        /* Use fqmtr to obtain OSC_D3_CK or OSC_CK */
        fqmtr_data = hal_clock_get_freq_meter(cali_ctx->src, winset);
        clk_dbg_log("[CLK] lposc%d cali target = %d kHz, fqmtr_data = %d kHz, cal_val(hex) %x\r\n", 4, (osc + 1), target_khz, fqmtr_data, lposc_cali_ft_val);
        if (fqmtr_data >= target_khz) {
            lposc_cali_ft_val = lposc_cali_ft_val - i + (i >> 1);
        } else {
            lposc_cali_ft_val = lposc_cali_ft_val + (i >> 1);
        }

        i = i >> 1; /* select next bit position */
    }
    /* assign value to cali or ft register field */
    *lposc_cali_ft_reg = lposc_cali_ft_val; /* assign value to cali or ft register field */

    /* print LPOSC calibration result */
    /* Use fqmtr to obtain OSC_CK */
    fqmtr_data = hal_clock_get_freq_meter(cali_ctx->src, winset);
    clk_dbg_log("[CLK] coarse 0x%x fine tune 0x%x\r\n", 2, LPOSC_CON_REG(p_lposc_reg).DA_LPOSC_RBANK_CALI, LPOSC_CON_REG(p_lposc_reg).DA_LPOSC_RBANK_FT);
    clk_dbg_log("[CLK] Final lposc%d target = %d kHz, fqmtr_data = %d kHz\r\n", 3, (osc + 1), target_khz, fqmtr_data);
}

ATTR_TEXT_IN_TCM static void lposc1_init(void)
{
    lposc_cali(LPOSC1, COARSE_CALI);
    lposc_cali(LPOSC1, COARSE_FT);
}

ATTR_TEXT_IN_TCM static void lposc2_init(void)
{
    lposc_cali(LPOSC2, COARSE_CALI);
    lposc_cali(LPOSC2, COARSE_FT);
}

/**
 * Setting for enable LPOSC1
 */
ATTR_TEXT_IN_TCM void lposc1_enable(void)
{
    static uint8_t lposc1_calibrated = 0;

    if (LPOSC1_CON_REG.DA_LPOSC_EN == 1) {
        /* TODO: perform recalibration */
        lposc1_calibrated = 1;  /* Since LPOSC1 is in-use, it is expected to already be calibrated */
        return;
    }

#if 0
    /* disable resetting LPOSC calibration values */
    LPOSC1_CON_REG.DA_LPOSC_RBANK_CALI = 0x28;
    LPOSC1_CON_REG.DA_LPOSC_RBANK_FT  = 0x0;
#endif
    LPOSC1_CON_REG.RG_LPOSC_CBANK_SEL = 0x1;
    LPOSC1_CON_REG.RG_LPOSC_KVCO_SEL = 0x2;
    LPOSC1_CON_REG.DA_LPOSC_EN  = 0x1;         /* enable LPOSC1 */
    LPOSC1_CON_REG.RG_LPOSC_CK_EN  = 0x1;
    LPOSC1_CON_REG.RG_LPOSC_DIV3_CK_EN  = 0x1; /* used in calibration flow */
    LPOSC1_CON_REG.RG_LPOSC_DIV48_CK_EN  = 0x1; /* for hopping, SPM clock control */
    hal_gpt_delay_us(20);                      /* wait 20us for LPOSC1 stable */
    CLKSQ_CON_REG.RG_LPOSC1_RDY = 0x1;         /* release LPOSC1 clock gating */

    if (lposc1_calibrated == 0) {
        lposc1_init();
        lposc1_calibrated = 1;
    }
}

/**
 * Setting for enable LPOSC2
 */
ATTR_TEXT_IN_TCM void lposc2_enable(void)
{
    static uint8_t lposc2_calibrated = 0;

    if (LPOSC2_CON_REG.DA_LPOSC_EN == 1) {
        /* TODO: perform recalibration */
        lposc2_calibrated = 1;  /* Since LPOSC2 is in-use, it is expected to already be calibrated */
        return;
    }

#if 0
    /* disable resetting LPOSC calibration values */
    LPOSC2_CON_REG.DA_LPOSC_RBANK_CALI = 0x28;
    LPOSC2_CON_REG.DA_LPOSC_RBANK_FT  = 0x0;
#endif
    LPOSC2_CON_REG.RG_LPOSC_CBANK_SEL = 0x2;
    LPOSC2_CON_REG.DA_LPOSC_EN  = 0x1;          /* enable LPOSC2 */
    LPOSC2_CON_REG.RG_LPOSC_CK_EN  = 0x1;
    LPOSC2_CON_REG.RG_LPOSC_DIV3_CK_EN  = 0x1; /* used in calibration flow */
    LPOSC2_CON_REG.RG_LPOSC_DIV48_CK_EN  = 0x1; /* for hopping, SPM clock control */
    hal_gpt_delay_us(20);                      /* wait 20us for LPOSC2 stable */
    CLKSQ_CON_REG.RG_LPOSC2_RDY = 0x1;         /* release LPOSC2 clock gating */

    if (lposc2_calibrated == 0) {
        lposc2_init();
        lposc2_calibrated = 1;
    }
}

#if 0
/* Comment out lposc1_disable() code (currently not used) */
ATTR_TEXT_IN_TCM static void lposc1_disable(void)
{
#if 0
    /* LPOSC1 should not be disabled
     * (for SPM clock control function to work)
     */
    CLKSQ_CON_REG.RG_LPOSC1_RDY = 0x0;
    LPOSC1_CON_REG.RG_LPOSC_DIV3_CK_EN  = 0x0;
    LPOSC1_CON_REG.RG_LPOSC_DIV48_CK_EN = 0x0;
    LPOSC1_CON_REG.RG_LPOSC_CK_EN  = 0x0;
    LPOSC1_CON_REG.DA_LPOSC_EN  = 0x0;
#endif
}
#endif

ATTR_TEXT_IN_TCM static void lposc2_disable(void)
{
    CLKSQ_CON_REG.RG_LPOSC2_RDY = 0x0;
    LPOSC2_CON_REG.RG_LPOSC_DIV3_CK_EN  = 0x0;
    LPOSC2_CON_REG.RG_LPOSC_DIV48_CK_EN = 0x0;
    LPOSC2_CON_REG.RG_LPOSC_CK_EN  = 0x0;
    LPOSC2_CON_REG.DA_LPOSC_EN  = 0x0;
}

#if defined(__EXT_BOOTLOADER__) || defined(__EXT_DA__)
/* Settings to allow SPM to control clock on/off
 * currently called/enabled in bootloader
 */
ATTR_TEXT_IN_TCM static void spm_clock_ctrl_enable(void)
{
    /* Required settings_check */
    if (LPOSC1_CON_REG.DA_LPOSC_EN == 0) {
        clk_dbg_log("[CLK] spm_clock_ctrl warning, LPOSC1 was not enabled!\r\n", 0);
        /* LPOSC1 must be enabled */
        lposc1_enable();
    }
    /* TODO: check if DIV48_CK_EN is enabled for the current in-use LPOSC */

    CLKSQ_CON_REG.BP_PLL_DLY = 0x0;
    CLKSQ_CON_REG.UPLL_EN_SEL = 0x0;
    CLKSQ_CON_REG.LPOSC1_EN_SEL = 0x0;
    CLKSQ_CON_REG.LPOSC2_EN_SEL = 0x0;
    /* TODO: move MIXEDSYS CG control to a common function */
    hal_clock_enable(HAL_CLOCK_CG_MIXEDSYS);
}
#endif

/* CKSYS_CLK_CFG_0 register value for targeted DVFS OPP */
ATTR_RWDATA_IN_TCM clk_volt_lv_info dvfs_freq_tbl[VOLT_LV_NUM] = {{.field = { .sys = 3, .dsp = 3, .sfc = 2, .esc = ESC_104M_OSC1_D3}},
    {.field = { .sys = 2, .dsp = 2, .sfc = 2, .esc = ESC_104M_OSC1_D3}},
    {.field = { .sys = 5, .dsp = 5, .sfc = 2, .esc = ESC_104M_OSC1_D3}}
};
/* CKSYS_CLK_CFG_3 register value for targeted DVFS OPP */
ATTR_RWDATA_IN_TCM clk_cfg3_config dvfs_cfg3_tbl[VOLT_LV_NUM] = {{.field = { .aud_intbus = 2, .aud_gpsrc = 4, .aud_uplink = 0, .aud_dwlink = 1}},
    {.field = { .aud_intbus = 1, .aud_gpsrc = 1, .aud_uplink = 1, .aud_dwlink = 1}},
    {.field = { .aud_intbus = 1, .aud_gpsrc = 1, .aud_uplink = 1, .aud_dwlink = 1}}
};


/* API that returns current CPU clk freq in Hz
 * This was designed for usage in SystemCoreClockUpdate()
 */
ATTR_TEXT_IN_TCM uint32_t get_curr_cpu_freq_hz(void)
{
    //CLK_SYS_SEL corresponding frequency meaning definitions (in Hz)
    const uint32_t cpu_hz_arr[MUX_SEL_NUM] = {
        26 * 1000 * 1000,       // 0 : F_FXO_CK     26MHz
        26 * 1000 * 1000,       // 1 : OSC_26       26MHz
#if defined(AIR_RFI_SUPPRESS_DISABLE)
        156 * 1000 * 1000,      // 2 : OSC1_D2      156MHz
        104 * 1000 * 1000,      // 3 : OSC1_D3      104MHz
        78 * 1000 * 1000,       // 4 : OSC1_D2_D2   78MHz
#else
        149.625 * 1000 * 1000,  // 2 : OSC1_D2      149.625MHz
        99.75 * 1000 * 1000,    // 3 : OSC1_D3      99.75MHz
        74.8125 * 1000 * 1000,  // 4 : OSC1_D2_D2   74.8125MHz
#endif
        260 * 1000 * 1000,      // 5 : OSC2_D2      260MHz
        130 * 1000 * 1000,      // 6 : OSC2_D2_D2   130MHz
        156 * 1000 * 1000,      // 7 : UPLL_D2      156MHz
    };
    /* CLK_SYS_SEL represents the current mux select option, and the above defined array represents the corresponding freq definition */
    uint8_t curr_sel_num = *((uint8_t *)BYTE_REG(CKSYS_CLK_CFG_REG.CLK_SYS_SEL));

    return cpu_hz_arr[curr_sel_num];
}

/* Enable the dvfs requested mux switch required resources (before actual mux switch in dvfs_switch_clock_freq() */
ATTR_TEXT_IN_TCM void dvfs_pre_proc(uint8_t next_opp_idx)
{
    mux_switch_preproc(CLK_SYS_SEL,  dvfs_freq_tbl[next_opp_idx].field.sys);
    mux_switch_preproc(CLK_DSP_SEL,  dvfs_freq_tbl[next_opp_idx].field.dsp);
    mux_switch_preproc(CLK_SFC_SEL,  dvfs_freq_tbl[next_opp_idx].field.sfc);
    mux_switch_preproc(CLK_ESC_SEL,  dvfs_freq_tbl[next_opp_idx].field.esc);

    mux_switch_preproc(CLK_AUD_BUS_SEL,  dvfs_cfg3_tbl[next_opp_idx].field.aud_intbus);
    mux_switch_preproc(CLK_AUD_GPSRC_SEL,  dvfs_cfg3_tbl[next_opp_idx].field.aud_gpsrc);
    mux_switch_preproc(CLK_AUD_ULCK_SEL,  dvfs_cfg3_tbl[next_opp_idx].field.aud_uplink);
    mux_switch_preproc(CLK_AUD_DLCK_SEL,  dvfs_cfg3_tbl[next_opp_idx].field.aud_dwlink);
}

/* Check and disable unused divider/clock source accordingly */
ATTR_TEXT_IN_TCM void dvfs_post_proc(void)
{
    mux_switch_postproc();
}


/* all dvfs freq/mux switch is performed in this function */
ATTR_TEXT_IN_TCM void dvfs_switch_clock_freq(uint8_t next_opp_idx)
{
    mux_switch_proc(CLK_SYS_SEL, dvfs_freq_tbl[next_opp_idx].field.sys);
    mux_switch_proc(CLK_DSP_SEL, dvfs_freq_tbl[next_opp_idx].field.dsp);
    mux_switch_proc(CLK_SFC_SEL, dvfs_freq_tbl[next_opp_idx].field.sfc);
    mux_switch_proc(CLK_ESC_SEL, dvfs_freq_tbl[next_opp_idx].field.esc);

    mux_switch_proc(CLK_AUD_BUS_SEL, dvfs_cfg3_tbl[next_opp_idx].field.aud_intbus);
    mux_switch_proc(CLK_AUD_GPSRC_SEL, dvfs_cfg3_tbl[next_opp_idx].field.aud_gpsrc);
    mux_switch_proc(CLK_AUD_ULCK_SEL, dvfs_cfg3_tbl[next_opp_idx].field.aud_uplink);
    mux_switch_proc(CLK_AUD_DLCK_SEL, dvfs_cfg3_tbl[next_opp_idx].field.aud_dwlink);

}

#if !defined(__EXT_BOOTLOADER__) && !defined(__EXT_DA__)
static hal_clock_status_t clk_mux_init_rtos(void)
{
    hal_clock_status_t result = HAL_CLOCK_STATUS_OK;
    clk_dbg_log("clk_mux_init_rtos\r\n", 0);

    /* Trigger LPOSC1, LPOSC2 calibration again during sys_init()
     * mainly to set the the lposc1_calibrated, lposc2_calibrated flags
     * (To avoid re-performing LPOSC full calibration during DVFS)
     */
    lposc1_enable();
    lposc2_enable();

    return result;
}
#else
ATTR_TEXT_IN_TCM static hal_clock_status_t clk_mux_init_baremetal(void)
{
    hal_clock_status_t result = HAL_CLOCK_STATUS_OK;
    lposc1_enable(); /* lposc1 must be enabled for SPM clock control */
    lposc2_enable();

    dvfs_pre_proc(HAL_DVFS_OPP_HIGH);
    dvfs_switch_clock_freq(HAL_DVFS_OPP_HIGH);

    /* set MCU/DSP side used module clock sources */
    clock_mux_sel(CLK_I3C_SEL, 1);
    clock_mux_sel(CLK_SPIMST0_SEL, 1);

    spm_clock_ctrl_enable();

    hal_clock_dcm_ctrl(); /* Enable/Disable DCM feature (controlled by compile option) */

    return result;
}
#endif



hal_clock_status_t clk_mux_init(void)
{
    hal_clock_status_t result = HAL_CLOCK_STATUS_OK;
#if !defined(__EXT_BOOTLOADER__) && !defined(__EXT_DA__)
    clk_mux_init_rtos();
#else
    clk_mux_init_baremetal();
#endif /*__EXT_BOOTLOADER__*/
    return result;
}

#if defined(DCM_ENABLE)
/* Settings for DCM enable */
static void hal_clock_dcm_enable(hal_clk_dcm_id dcm_id)
{
    switch (dcm_id) {
        case clk_aud_bus_dcm:
            /* for audio bus DCM to take affect , there are audio pdn register fields
             * that all need to be set to 1, they are listed as below
             *     pdn_asrc1, pdn_asrc2, pdn_asrc3, pdn_i2s_dma
             */
            AUD_DCM_CON_REG.RG_AUD_SFSEL = 0; // divide by 64
            AUD_DCM_CON_REG.RG_AUD_CLKSLOW_EN = 0x1;
            AUD_DCM_CON_REG.RG_AUD_CLKOFF_EN = 0x1;
            AUD_DCM_CON_REG.RG_AUD_PLLCK_SEL = 0x1;
            break;
        case clk_dsp_dcm :
            DSP_DCM_CON_REG.RG_DSP_SFSEL = 0; // divide by 64
            DSP_DCM_CON_REG.RG_DSP_CLKSLOW_EN = 0x1;
            DSP_DCM_CON_REG.RG_DSP_PLLCK_SEL = 0x1;
            break;
        case clk_bus_dcm : //BUS DCM
            //slow bus dcm
            XO_DCM_CON_REG.RG_XO_SFSEL = 0x2; // divide by 16
            XO_DCM_CON_REG.RG_XO_DCM_DBC_NUM = 0x2;
            XO_DCM_CON_REG.RG_XO_DCM_DBC_EN = 0x1;
            XO_DCM_CON_REG.RG_XO_CLKSLOW_EN = 0x1;
            XO_DCM_CON_REG.RG_XO_CLKOFF_EN = 0x1;
            XO_DCM_CON_REG.RG_XO_PLLCK_SEL = 0x1;

            // fast bus dcm
            BUS_DCM_CON_REG.RG_BUS_SFSEL = 0; // divide by 64
            BUS_DCM_CON_REG.RG_BUS_DCM_DBC_NUM = 0x2;
            BUS_DCM_CON_REG.RG_BUS_DCM_DBC_EN = 0x1;
            BUS_DCM_CON_REG.RG_BUS_CLKSLOW_EN = 0x1;
            BUS_DCM_CON_REG.RG_BUS_CLKOFF_EN = 0x1;
            BUS_DCM_CON_REG.RG_BUS_PLLCK_SEL = 0x1;
            hal_gpt_delay_us(1);
            break;
        case clk_sfc_dcm :
            SFC_DCM_CON_REG.RG_SFC_DCM_DBC_NUM = 0xFF;
            SFC_DCM_CON_REG.RG_SFC_DCM_DBC_EN = 0x1;
            SFC_DCM_CON_REG.RG_SFC_CLK_OFF_EN = 0x1;
            SFC_DCM_CON_REG.RG_SFC_DCM_APB_SEL = 0x6;
            SFC_DCM_CON_REG.RG_SFC_DCM_APB_TOG = ~SFC_DCM_CON_REG.RG_SFC_DCM_APB_TOG;
            break;

        case clk_esc_dcm :
            ESC_DCM_CON_REG.RG_ESC_DCM_DBC_NUM = 0xFF;
            ESC_DCM_CON_REG.RG_ESC_DCM_DBC_EN = 0x1;
            ESC_DCM_CON_REG.RG_ESC_CLK_OFF_EN = 0x1;
            ESC_DCM_CON_REG.RG_ESC_DCM_APB_SEL = 0x6;
            ESC_DCM_CON_REG.RG_ESC_DCM_APB_TOG = ~ESC_DCM_CON_REG.RG_ESC_DCM_APB_TOG;
            break;
        case clk_cm4_dcm :
            CM4_DCM_CON_REG.RG_CM_SFSEL = 0; // divide by 64
            CM4_DCM_CON_REG.RG_CM_CLKSLOW_EN = 0x1;
            CM4_DCM_CON_REG.RG_CM_PLLCK_SEL  = 0x1;
            hal_gpt_delay_us(1);
            break;
        default :
            return;
    }

    hal_gpt_delay_us(1);
}

#else
/* Settings for DCM disable */
static void hal_clock_dcm_disable(hal_clk_dcm_id dcm_id)
{
    switch (dcm_id) {
        case clk_aud_bus_dcm:
            AUD_DCM_CON_REG.RG_AUD_PLLCK_SEL = 0x0;
            AUD_DCM_CON_REG.RG_AUD_CLKSLOW_EN = 0x0;
            AUD_DCM_CON_REG.RG_AUD_CLKOFF_EN = 0x0;
            break;
        case clk_dsp_dcm :
            DSP_DCM_CON_REG.RG_DSP_PLLCK_SEL = 0x0;
            DSP_DCM_CON_REG.RG_DSP_CLKSLOW_EN = 0x0;
            break;
        case clk_bus_dcm : //BUS DCM
            //slow bus dcm
            XO_DCM_CON_REG.RG_XO_PLLCK_SEL = 0x0;
            XO_DCM_CON_REG.RG_XO_CLKSLOW_EN = 0x0;
            XO_DCM_CON_REG.RG_XO_CLKOFF_EN = 0x0;

            // fast bus dcm
            BUS_DCM_CON_REG.RG_BUS_PLLCK_SEL = 0x0;
            BUS_DCM_CON_REG.RG_BUS_CLKSLOW_EN = 0x0;
            BUS_DCM_CON_REG.RG_BUS_CLKOFF_EN = 0x0;
            hal_gpt_delay_us(1);
            break;
        case clk_sfc_dcm :
            SFC_DCM_CON_REG.RG_SFC_DCM_DBC_EN = 0x0;
            SFC_DCM_CON_REG.RG_SFC_CLK_OFF_EN = 0x0;
            SFC_DCM_CON_REG.RG_SFC_DCM_APB_SEL = 0x6;
            SFC_DCM_CON_REG.RG_SFC_DCM_APB_TOG = ~SFC_DCM_CON_REG.RG_SFC_DCM_APB_TOG;
            break;

        case clk_esc_dcm :
            ESC_DCM_CON_REG.RG_ESC_DCM_DBC_EN = 0x0;
            ESC_DCM_CON_REG.RG_ESC_CLK_OFF_EN = 0x0;
            ESC_DCM_CON_REG.RG_ESC_DCM_APB_SEL = 0x6;
            ESC_DCM_CON_REG.RG_ESC_DCM_APB_TOG = ~ESC_DCM_CON_REG.RG_ESC_DCM_APB_TOG;
            break;
        case clk_cm4_dcm :
            CM4_DCM_CON_REG.RG_CM_PLLCK_SEL  = 0x0;
            CM4_DCM_CON_REG.RG_CM_CLKSLOW_EN = 0x0;
            hal_gpt_delay_us(1);
            break;
        default :
            return;
    }

    hal_gpt_delay_us(1);
}
#endif

/* DCM ctrl API (controlled by internal compiled option) */
void hal_clock_dcm_ctrl(void)
{
#if defined(DCM_ENABLE)
    clk_dbg_log("[CLK] Dynamic Clock Management: Enable\r\n", 0);
    /* Enable */
    hal_clock_dcm_enable(clk_sfc_dcm);
    hal_clock_dcm_enable(clk_aud_bus_dcm);
    hal_clock_dcm_enable(clk_dsp_dcm);
    hal_clock_dcm_enable(clk_bus_dcm);
    hal_clock_dcm_enable(clk_esc_dcm);
    hal_clock_dcm_enable(clk_cm4_dcm);
#else
    /* Disable */
    clk_dbg_log("[CLK] Dynamic Clock Management: Disable\r\n", 0);
    hal_clock_dcm_disable(clk_sfc_dcm);
    hal_clock_dcm_disable(clk_aud_bus_dcm);
    hal_clock_dcm_disable(clk_dsp_dcm);
    hal_clock_dcm_disable(clk_bus_dcm);
    hal_clock_dcm_disable(clk_esc_dcm);
    hal_clock_dcm_disable(clk_cm4_dcm);
#endif
}


ATTR_TEXT_IN_TCM static void apll2_cali()
{
    XPLL_DBG_PROB_REG.XPLL_DBG_SEL = 0x8;
    APLL2_CON_REG.RG_APLL1_VTMON_EN = 0x1;
#ifndef FPGA_ENV
    while (!(*XPLL_DBG_PROB_MON & 0x8)) {
        if (*XPLL_DBG_PROB_MON & 0x2) {
            APLL2_CON_REG.RG_APLL1_IBANK_FINETUNE += 1;
            if (APLL2_CON_REG.RG_APLL1_IBANK_FINETUNE == 0xF) {
                log_hal_msgid_info("apll2 cali FT max!(cali fail)\r\n", 0);
                assert(0);
                break;
            }
            hal_gpt_delay_us(50);
        } else if (*XPLL_DBG_PROB_MON & 0x4) {
            APLL2_CON_REG.RG_APLL1_IBANK_FINETUNE -= 1;
            if (!APLL2_CON_REG.RG_APLL1_IBANK_FINETUNE) {
                log_hal_msgid_info("apll2 cali FT min!(cali fail)\r\n", 0);
                assert(0);
                break;
            }
            hal_gpt_delay_us(50);
        } else {
            log_hal_msgid_info("apll cali fail \r\n", 0);
            assert(0);
            break;
        }
    }
#endif
    XPLL_DBG_PROB_REG.XPLL_DBG_SEL = 0x0;
    APLL2_CON_REG.RG_APLL1_VTMON_EN = 0x0;
}

ATTR_TEXT_IN_TCM static void apll1_cali()
{
    XPLL_DBG_PROB_REG.XPLL_DBG_SEL = 0x1;
    APLL1_CON_REG.RG_APLL1_VTMON_EN = 0x1;
#ifndef FPGA_ENV
    while (!(*XPLL_DBG_PROB_MON & 0x8)) {
        if (*XPLL_DBG_PROB_MON & 0x2) {
            APLL1_CON_REG.RG_APLL1_IBANK_FINETUNE += 1;
            if (APLL1_CON_REG.RG_APLL1_IBANK_FINETUNE == 0xF) {
                log_hal_msgid_info("apll1 cali FT max err!(cali fail)\r\n", 0);
                assert(0);
                break;
            }
            hal_gpt_delay_us(50);
        } else if (*XPLL_DBG_PROB_MON & 0x4) {
            APLL1_CON_REG.RG_APLL1_IBANK_FINETUNE -= 1;
            if (!APLL1_CON_REG.RG_APLL1_IBANK_FINETUNE) {
                log_hal_msgid_info("apll1 cali FT min err!(cali fail)\r\n", 0);
                assert(0);
                break;
            }
            hal_gpt_delay_us(50);
        } else {
            log_hal_msgid_info("apll cali fail \r\n", 0);
            assert(0);
            break;
        }
    }
#endif
    XPLL_DBG_PROB_REG.XPLL_DBG_SEL = 0x0;
    APLL1_CON_REG.RG_APLL1_VTMON_EN = 0x0;
}

/**
 *  SW UPLL calibration flow
 *  - Should only be executed as a fallback solution
 */
ATTR_TEXT_IN_TCM static void upll_cali(void)
{
    SYS_ABIST_MON_CON_REG.ABIST_LMON_SEL = 0x16;
    UPLL_CON_REG.RG_UPLL_VTMON_EN = 0x1;
#ifndef FPGA_ENV
    while (!(*ABIST_MON_DATA0 & 0x4)) {
        if (*ABIST_MON_DATA0 & 0x2) {
            UPLL_CON_REG.RG_UPLL_IBANK_FINETUNE += 1;
            if (UPLL_CON_REG.RG_UPLL_IBANK_FINETUNE == 0xF) {
                log_hal_msgid_info("upll cali FT max err!(cali fail)\r\n", 0);
                assert(0);
                break;
            }
            hal_gpt_delay_us(50);
        } else if (*ABIST_MON_DATA0 & 0x1) {
            UPLL_CON_REG.RG_UPLL_IBANK_FINETUNE -= 1;
            if (!UPLL_CON_REG.RG_UPLL_IBANK_FINETUNE) {
                log_hal_msgid_info("upll cali FT min err!(cali fail)\r\n", 0);
                assert(0);
                break;
            }
            hal_gpt_delay_us(50);
        } else {
            log_hal_msgid_info("upll cali fail \r\n", 0);
            assert(0);
            break;
        }
    }
#endif
    SYS_ABIST_MON_CON_REG.ABIST_LMON_SEL = 0x0;
    UPLL_CON_REG.RG_UPLL_VTMON_EN = 0x0;
}

/* clk_pll_on: enables APLL1, APLL2, UPLL
 * (returns if it is already enabled)
 */
#define APLL_LOW_PWR_MODE /* define this to enable APLL_LOW_PWR_MODE */
ATTR_TEXT_IN_TCM static uint8_t clk_pll_on(clock_pll_id pll_id)
{
    if (pll_id > CLK_UPLL) {
        return 0;
    }

    hal_clock_enable(HAL_CLOCK_CG_MIXEDSYS);
    switch (pll_id) {

        case CLK_APLL1:
            if (APLL1_CON_REG.RG_APLL1_EN) {
                /* already enabled, skip enable flow */
                break;
            }
            APLL1_CON_REG.RG_APLL1_V2I_EN = 0x1;
            APLL1_CON_REG.RG_APLL1_DDS_PWR_ON = 0x1;
            hal_gpt_delay_us(5);
            APLL1_CON_REG.RG_APLL1_DDS_ISO_EN = 0x0;
            APLL1_CON_REG.RG_APLL1_LCDDS_PCW_NCPO        = (apll_freq[CLK_APLL1] == CLK_APLL_45M)  ? 0x0DE517AA + 1 : 0x0F1FAA4D + 1; //? 45M+1 : 49M+1  /*? 0xCEC4EC5 : 0xE276276;    //APLL1_CON_REG[10] 42M : 46M*/
            APLL1_CON_REG.RG_APLL1_LCDDS_TUNER_PCW_NCPO  = (apll_freq[CLK_APLL1] == CLK_APLL_45M)  ? 0x0DE517AA - 1 : 0x0F1FAA4D - 1; //? 45M-1 : 49M-1  /*? 0xEC4EC4F : 0x104EC4EC;   //APLL1_CON_REG[14] 48M : 53M*/
            APLL1_CON_REG.RG_APLL1_DIV16_EN = 0x1;
            APLL1_CON_REG.RG_APLL1_IBANK_EN = 0X1;
#if defined(APLL_LOW_PWR_MODE)
            /* low power mode */
            APLL1_CON_REG.RG_APLL1_LDOOUT2_SEL = 0x2;
            APLL1_CON_REG.RG_APLL1_LDOOUT1_SEL = 0x0;
            APLL1_CON_REG.RG_APLL1_CP_EN = 0x1;
            APLL1_CON_REG.RG_APLL1_RESERVE = 0xC4;
            RSV_CON0_REG.SCAN_RSV0 = 0x20;
#else
            APLL1_CON_REG.RG_APLL1_LDOOUT2_SEL = 0x2;
            APLL1_CON_REG.RG_APLL1_LDOOUT1_SEL = 0x2;
            APLL1_CON_REG.RG_APLL1_CP_EN = 0x1;
            APLL1_CON_REG.RG_APLL1_RESERVE = 0x04;
            RSV_CON0_REG.SCAN_RSV0 = 0x00;
#endif
            APLL1_CON_REG.RG_APLL1_LCDDS_EN = 0x1;
            APLL1_CON_REG.RG_APLL1_EN = 0x1;
            hal_gpt_delay_us(50);
            apll1_cali();

            APLL1_CON_REG.RG_APLL1_LCDDS_TUNER_EN = 0x1;
            break;
        case CLK_APLL2:
            if (APLL2_CON_REG.RG_APLL1_EN) {
                /* already enabled, skip enable flow */
                break;
            }
            APLL2_CON_REG.RG_APLL1_V2I_EN = 0x1;
            APLL2_CON_REG.RG_APLL1_DDS_PWR_ON = 0x1;
            hal_gpt_delay_us(5);
            APLL2_CON_REG.RG_APLL1_DDS_ISO_EN = 0x0;
            APLL2_CON_REG.RG_APLL1_LCDDS_PCW_NCPO        = (apll_freq[CLK_APLL2] == CLK_APLL_45M)  ? 0x0DE517AA + 1 : 0x0F1FAA4D + 1; //? 45M+1 : 49M+1  /*? 0xCEC4EC5 : 0xE276276;    //APLL1_CON_REG[10] 42M : 46M*/
            APLL2_CON_REG.RG_APLL1_LCDDS_TUNER_PCW_NCPO  = (apll_freq[CLK_APLL2] == CLK_APLL_45M)  ? 0x0DE517AA - 1 : 0x0F1FAA4D - 1; //? 45M-1 : 49M-1  /*? 0xEC4EC4F : 0x104EC4EC;   //APLL1_CON_REG[14] 48M : 53M*/
            APLL2_CON_REG.RG_APLL1_DIV16_EN = 0x1;
            APLL2_CON_REG.RG_APLL1_IBANK_EN = 0X1;
#if defined(APLL_LOW_PWR_MODE)
            /* low power mode */
            APLL2_CON_REG.RG_APLL1_LDOOUT2_SEL = 0x2;
            APLL2_CON_REG.RG_APLL1_LDOOUT1_SEL = 0x0;
            APLL2_CON_REG.RG_APLL1_CP_EN = 0x1;
            APLL2_CON_REG.RG_APLL1_RESERVE = 0xC4;
            RSV_CON0_REG.SCAN_RSV0 = 0x20;
#else
            APLL2_CON_REG.RG_APLL1_LDOOUT2_SEL = 0x2;
            APLL2_CON_REG.RG_APLL1_LDOOUT1_SEL = 0x2;
            APLL2_CON_REG.RG_APLL1_CP_EN = 0x1;
            APLL2_CON_REG.RG_APLL1_RESERVE = 0x04;
            RSV_CON0_REG.SCAN_RSV0 = 0x00;
#endif
            APLL2_CON_REG.RG_APLL1_LCDDS_EN = 0x1;
            APLL2_CON_REG.RG_APLL1_EN = 0x1;
            hal_gpt_delay_us(50);
            apll2_cali();
            APLL2_CON_REG.RG_APLL1_LCDDS_TUNER_EN = 0x1;
            break;
        case CLK_UPLL:
            if (UPLL_CON_REG.DA_UPLL_EN) {
                /* already enabled, skip enable flow */
                break;
            }
            UPLL_CON_REG.RG_UPLL_GLOBAL_LDO_BIAS_EN = 0x1;
            hal_gpt_delay_us(5);
            UPLL_CON_REG.RG_UPLL_IBANK_EN = 0x1;
            UPLL_CON_REG.RG_UPLL_CP_EN = 0x1;
            UPLL_CON_REG.RG_UPLL_PREDIV = 0x0;
            UPLL_CON_REG.RG_UPLL_FBDIV = 24;
            UPLL_CON_REG.RG_UPLL_POSTDIV = 0x1;
            UPLL_CON_REG.DA_UPLL_EN = 0x1;
            UPLL_CON_REG.RG_UPLL_48M_EN = 0x1;
            UPLL_CON_REG.RG_UPLL_624M_EN  = 0x1;
            hal_gpt_delay_us(20);
            CLKSQ_CON_REG.RG_UPLL_RDY = 0x1;
            if (EFUSE->M_ANA_CFG_UPLL_IBANK_b.UPLL_IBANK_EN) {
                /* Apply calibrated settings from efuse, or else don't modify HW default values */
                UPLL_CON_REG.RG_UPLL_IBANK_FINETUNE = EFUSE->M_ANA_CFG_UPLL_IBANK_b.UPLL_IBANK_FINETUNE;
                UPLL_CON_REG.RG_UPLL_KVCO_SEL = EFUSE->M_ANA_CFG_UPLL_IBANK_b.UPLL_KVCO_SEL;
                clk_dbg_log("UPLL IBANK_FINETUNE 0x%x, KVCO_SEL 0x%x\r\n", 2, EFUSE->M_ANA_CFG_UPLL_IBANK_b.UPLL_IBANK_FINETUNE, EFUSE->M_ANA_CFG_UPLL_IBANK_b.UPLL_KVCO_SEL);
            } else {
                clk_dbg_print("Warning! UPLL efuse FT not done! Performing upll_cali\r\n", 0);
                upll_cali();
            }
            break;
        default :
            break;
    }
    /* Must wait 20us before disabling MIXEDSYS clock
     * (for UPLL internal DLY_CNT to process)
     */
    hal_gpt_delay_us(20);
    hal_clock_disable(HAL_CLOCK_CG_MIXEDSYS);
    return HAL_CLOCK_STATUS_OK;
}


ATTR_TEXT_IN_TCM static uint8_t clk_pll_off(clock_pll_id pll_id)
{
    if (pll_id >= NR_PLLS) {
        return 0;
    }
    hal_clock_enable(HAL_CLOCK_CG_MIXEDSYS);
    switch (pll_id) {
        case CLK_APLL1:
            APLL1_CON_REG.RG_APLL1_EN = 0x0;
            APLL1_CON_REG.RG_APLL1_LCDDS_EN = 0x0;
            APLL1_CON_REG.RG_APLL1_DDS_ISO_EN = 0x1;
            APLL1_CON_REG.RG_APLL1_DDS_PWR_ON = 0x0;
            hal_gpt_delay_us(5);

            APLL1_CON_REG.RG_APLL1_V2I_EN = 0x0;
            APLL1_CON_REG.RG_APLL1_LCDDS_PCW_NCPO_CHG = 0x0;
            // cksys tuner off
            APLL1_CON_REG.RG_APLL1_LCDDS_PCW_NCPO  = APLL1_CON_REG.DA_APLL1_LCDDS_PCW_NCPO;
            APLL1_CON_REG.RG_APLL1_LCDDS_TUNER_EN = 0x0;
            break;
        case CLK_APLL2:
            APLL2_CON_REG.RG_APLL1_EN = 0x0;
            APLL2_CON_REG.RG_APLL1_LCDDS_EN = 0x0;
            APLL2_CON_REG.RG_APLL1_DDS_ISO_EN = 0x1;
            APLL2_CON_REG.RG_APLL1_DDS_PWR_ON = 0x0;
            hal_gpt_delay_us(5);

            APLL2_CON_REG.RG_APLL1_V2I_EN = 0x0;
            APLL2_CON_REG.RG_APLL1_LCDDS_PCW_NCPO_CHG = 0x0;
            // cksys tuner off
            APLL2_CON_REG.RG_APLL1_LCDDS_PCW_NCPO  = APLL2_CON_REG.DA_APLL1_LCDDS_PCW_NCPO;
            APLL2_CON_REG.RG_APLL1_LCDDS_TUNER_EN = 0x0;
            break;
        case CLK_UPLL:
            CLKSQ_CON_REG.RG_UPLL_RDY = 0x0;
            UPLL_CON_REG.RG_UPLL_48M_EN = 0x0;
            UPLL_CON_REG.DA_UPLL_EN = 0x0;
            UPLL_CON_REG.RG_UPLL_IBANK_EN = 0x0;
            UPLL_CON_REG.RG_UPLL_CP_EN = 0x0;
            UPLL_CON_REG.RG_UPLL_GLOBAL_LDO_BIAS_EN = 0x0;
            break;
    }
    hal_clock_disable(HAL_CLOCK_CG_MIXEDSYS);
    return 1;
}

ATTR_TEXT_IN_TCM hal_clock_status_t clk_set_apll(clock_pll_id pll, clk_apll_freq apll)
{

    switch (apll) {
        case CLK_APLL_45M:
            apll_freq[pll] = CLK_APLL_45M;
            goto pll_rst ;

        case CLK_APLL_49M:
            apll_freq[pll] = CLK_APLL_49M;
            goto pll_rst ;

        default :
            return HAL_CLOCK_STATUS_ERROR;
    }
pll_rst :

    if (pll == CLK_APLL1) {
        clk_pll_off(CLK_APLL1);
        clk_pll_on(CLK_APLL1);
    } else if (pll == CLK_APLL2) {
        clk_pll_off(CLK_APLL2);
        clk_pll_on(CLK_APLL2);
    }

    return HAL_CLOCK_STATUS_OK;

}

/*************************************************************************
 * CG Clock API definition
 *************************************************************************/

/* Check if specified cg_id has a corresponding mux that need processing
 * return -1: no corresponding mux
 * return any positive num: has corresponding mux (returns its corresponding mux_id)
 */
ATTR_TEXT_IN_TCM static int32_t clock_mux_exist(hal_clock_cg_id cg_id)
{
    /* Check if the clock_id has a corresponding mux that need to be switched
     * If one exists -> call mux_sel to trigger clock source/divider enable etc..
     */
    for (uint32_t mux_tbl_idx = 0; mux_tbl_idx < TOTAL_MUX_CNT; mux_tbl_idx++) {
        if (clock_tbl.mux_tbl[mux_tbl_idx].cg_info.cg_id == cg_id) {
            return mux_tbl_idx;
        }
    }

    return HAL_CLOCK_STATUS_INVALID_PARAMETER;
}

/* request
 * 0: status (return status, 1: cg enabled, 0: cg disabled)
 * 1: cg disable (return 0)
 * 2: cg enable (return 0)
 * others: return -1
 */
ATTR_TEXT_IN_TCM static int32_t hal_phys_cg_ctrl(hal_clock_cg_id clock_id, cg_request_t request)
{
    //clk_dbg_log("hal_phys_cg_ctrl\r\n", 0);
    volatile uint32_t *clr_addr = NULL, *status_addr = NULL, *set_addr = NULL;
    uint32_t bit_idx = clock_id & (0x1f);

    /* Set pointer to CG reg ptr */
    if (clock_id >= HAL_CLOCK_CG_UART1 && clock_id <= HAL_CLOCK_CG_AUXADC) {
        clr_addr = XO_PDN_PD_CLRD0;
        status_addr = XO_PDN_PD_COND0;
        set_addr = XO_PDN_PD_SETD0;
    } else if (clock_id >= HAL_CLOCK_CG_PWM0 && clock_id <= HAL_CLOCK_CG_GPTIMER1) {
        clr_addr = XO_PDN_AO_CLRD0;
        status_addr = XO_PDN_AO_COND0;
        set_addr = XO_PDN_AO_SETD0;
    } else if (clock_id >= HAL_CLOCK_CG_SPIMST0 && clock_id <= HAL_CLOCK_CG_EFUSE) {
        clr_addr = XO_PDN_TOP_CLRD0;
        status_addr = XO_PDN_TOP_COND0;
        set_addr = XO_PDN_TOP_SETD0;
    } else if (clock_id >= HAL_CLOCK_CG_OSC1_D12_D2 && clock_id <= HAL_CLOCK_CG_AUD_VOW_BUS) {
        clr_addr = XO_PDN_TOP_CLRD1;
        status_addr = XO_PDN_TOP_COND1;
        set_addr = XO_PDN_TOP_SETD1;
    } else if (clock_id >= HAL_CLOCK_CG_FAST_DMA0 && clock_id <= HAL_CLOCK_CG_SECURITY_PERI) {
        clr_addr = PDN_PD_CLRD0;
        status_addr = PDN_PD_COND0;
        set_addr = PDN_PD_SETD0;
    } else if (clock_id >= HAL_CLOCK_CG_BUS_ERR && clock_id <= HAL_CLOCK_CG_SEC_MON) {
        clr_addr = PDN_AO_CLRD0;
        status_addr = PDN_AO_COND0;
        set_addr = PDN_AO_SETD0;
    } else if (clock_id == HAL_CLOCK_CG_CMSYS_ROM) {
        clr_addr = PDN_TOP_CLRD0;
        status_addr = PDN_TOP_COND0;
        set_addr = PDN_TOP_SETD0;
    } else {
        clk_dbg_log("hal_phys_cg_ctrl(), invalid parameter clock_id %d", 1, clock_id);
        return -1;
    }

    if (request == CG_REQUEST_STATUS) {
        /* status (return cg bit directly*/
        return ((*(status_addr) >> bit_idx) & 0x1);
    } else if (request == CG_REQUEST_CLEAR) {
        /* cg disable (clock on) */
        *(clr_addr) |= (0x1 << bit_idx);
    } else if (request == CG_REQUEST_SET) {
        /* special case handling (do not allow the below CG to be disabled)
         * HAL_CLOCK_CG_ESC: for BT HW design, avoid bus hang when mmap address is accessed when clock is off
         * HAL_CLOCK_CG_AUD_INTBUS: avoid bus hang
         * HAL_CLOCK_CG_I3C: CG_I3C is a master CG for all I2C/I3C HW modules, keep enabled for now
         * others: CG will be used on both and MCU/DSP side
         */
        hal_clock_cg_id clk_always_on_tbl[] = {HAL_CLOCK_CG_UART_DMA_0, HAL_CLOCK_CG_I3C0, HAL_CLOCK_CG_I2C1
                                                , HAL_CLOCK_CG_SPIMST0, HAL_CLOCK_CG_I3C, HAL_CLOCK_CG_ESC, HAL_CLOCK_CG_AUD_INTBUS
                                                , HAL_CLOCK_CG_FAST_DMA1, HAL_CLOCK_CG_SPIMST0_BUS, HAL_CLOCK_CG_I3C_DMA0, HAL_CLOCK_CG_I2C_DMA1, HAL_CLOCK_CG_TRNG
                                                };
        for (int idx = 0; idx < (sizeof(clk_always_on_tbl)/sizeof(clk_always_on_tbl[0])); idx++) {
            if (clock_id == clk_always_on_tbl[idx]) {
                /* do not allow disabling of the CG in the table */
                return HAL_CLOCK_STATUS_OK;
            }
        }

        /* cg enable (clock off) */
        *(set_addr) |= (0x1 << bit_idx);
    }
    return 0;
}

/* CG Enable */
ATTR_TEXT_IN_TCM hal_clock_status_t hal_clock_enable(hal_clock_cg_id clock_id)
{
#if !defined(__EXT_BOOTLOADER__) && !defined(__EXT_DA__)
    uint32_t irq_mask = 0;
    hal_nvic_save_and_set_interrupt_mask_special(&irq_mask);
    /* ================ Critical Section Start ======================== */
#endif

    hal_clock_status_t ret = HAL_CLOCK_STATUS_OK;

    //clk_dbg_log ("hal_clock_enable\r\n", 0);
    /* Below disables clock gating to specified CG register or psuedo CG */
    if (clock_id < HAL_CLOCK_CG_PHYS_NUM) {
        hal_phys_cg_ctrl(clock_id, CG_REQUEST_CLEAR);
    } else if (clock_id > HAL_CLOCK_CG_PHYS_NUM && clock_id < HAL_CLOCK_CG_END) {
        /* Psuedo CG */
        uint32_t psuedo_cg_arr_idx = clock_id - HAL_CLOCK_CG_PSUEDO_CLK_26M;
        psuedo_cg_arr[psuedo_cg_arr_idx] = 0;
    } else if (PWM_MULTI_CTRL_REQUEST(clock_id)) {
        /* Process PWM multi-channel CG control request */
        volatile uint32_t *clr_addr = XO_PDN_AO_CLRD0;
        uint32_t set_bits = clock_id & CG_PWM_MULTI_CTRL_BITMASK;

        //clk_dbg_print("bitmask debug 0x%x, 0x%x", (clock_id & ~(CG_PWM_MULTI_CTRL_BITMASK)), ~(CG_PWM_MULTI_CTRL_BITMASK));
        *(clr_addr) = set_bits;
    } else {
        clk_dbg_log("hal_clock_enable() err! invalid clock_id %d", 1, clock_id);
        ret = HAL_CLOCK_STATUS_INVALID_PARAMETER;
    }

    int32_t clock_mux_idx = clock_mux_exist(clock_id);
    if (clock_mux_idx != (HAL_CLOCK_STATUS_INVALID_PARAMETER)) {
        /* CG's corresponding mux exists, perform mux select (to trigger clock enable etc..) */
        clock_mux_sel_id curr_mux_sel_id = clock_mux_idx;
        clock_mux_sel(curr_mux_sel_id, (int32_t)(*(clock_tbl.mux_tbl[clock_mux_idx].curr_sel_ptr)));
    }

#if !defined(__EXT_BOOTLOADER__) && !defined(__EXT_DA__)
    /* ================ Critical Section End ======================== */
    hal_nvic_restore_interrupt_mask_special(irq_mask);
#endif
    return ret;
}

/* CG Disable */
ATTR_TEXT_IN_TCM hal_clock_status_t hal_clock_disable(hal_clock_cg_id clock_id)
{
#if !defined(__EXT_BOOTLOADER__) && !defined(__EXT_DA__)
    uint32_t irq_mask = 0;
    hal_nvic_save_and_set_interrupt_mask_special(&irq_mask);
    /* ================ Critical Section Start ======================== */
#endif

    hal_clock_status_t ret = HAL_CLOCK_STATUS_OK;

    //clk_dbg_log("hal_clock_disable\r\n", 0);
    /* Below disables clock gating to specified CG register or psuedo CG */
    if (clock_id < HAL_CLOCK_CG_PHYS_NUM) {
        hal_phys_cg_ctrl(clock_id, CG_REQUEST_SET);
    } else if (clock_id > HAL_CLOCK_CG_PHYS_NUM && clock_id < HAL_CLOCK_CG_END) {
        /* Psuedo CG */
        //clk_dbg_print("psuedo_cg\r\n", 0);
        uint32_t psuedo_cg_arr_idx = clock_id - HAL_CLOCK_CG_PSUEDO_CLK_26M;
        psuedo_cg_arr[psuedo_cg_arr_idx] = 1;
    } else if (PWM_MULTI_CTRL_REQUEST(clock_id)) {
        /* Process PWM multi-channel CG control request */
        volatile uint32_t *set_addr = XO_PDN_AO_SETD0;
        uint32_t set_bits = clock_id & CG_PWM_MULTI_CTRL_BITMASK;

        //clk_dbg_print("bitmask debug 0x%x, 0x%x", 2, (clock_id & ~(CG_PWM_MULTI_CTRL_BITMASK)), ~(CG_PWM_MULTI_CTRL_BITMASK));
        *(set_addr) = set_bits;
    } else {
        clk_dbg_print("hal_clock_disable() err! invalid clock_id %d", 1, clock_id);
        ret = HAL_CLOCK_STATUS_INVALID_PARAMETER;
    }


    int32_t clock_mux_idx = clock_mux_exist(clock_id);
    if (clock_mux_idx != (HAL_CLOCK_STATUS_INVALID_PARAMETER)) {
        /* clock gating enabled may cause some clock resource to become able to disable */
        mux_switch_postproc();
    }

#if !defined(__EXT_BOOTLOADER__) && !defined(__EXT_DA__)
    /* ================ Critical Section End ======================== */
    hal_nvic_restore_interrupt_mask_special(irq_mask);
#endif
    return ret;
}


ATTR_TEXT_IN_TCM bool hal_clock_is_enabled(hal_clock_cg_id clock_id)
{
    int32_t cg_status = true;

    //clk_dbg_log("hal_clock_is_enabled\r\n", 0);
    /* Below retrieves clock gating status of specified CG register or psuedo CG */
    if (clock_id < HAL_CLOCK_CG_PHYS_NUM) {
        cg_status = hal_phys_cg_ctrl(clock_id, CG_REQUEST_STATUS);
    } else if (clock_id > HAL_CLOCK_CG_PHYS_NUM && clock_id < HAL_CLOCK_CG_END) {
        /* Psuedo CG */
        uint32_t psuedo_cg_arr_idx = clock_id - HAL_CLOCK_CG_PSUEDO_CLK_26M;
        cg_status = psuedo_cg_arr[psuedo_cg_arr_idx];
    } else {
        clk_dbg_log("hal_clock_disable() err! invalid clock_id %d", 1, clock_id);
        return HAL_CLOCK_STATUS_INVALID_PARAMETER;
    }

    if (cg_status == 0x1) {
        //clk_dbg_log("cg_id %d is disabled\r\n", 1, clock_id);
        return false;
    } else {
        //clk_dbg_log("cg_id %d is enabled\r\n", 1, clock_id);
        return true;
    }

}


/* Clock gating initial registRer settings */
static void _hal_clock_cg_init(void)
{
    struct cg_init_stat {
        uint8_t cg_id; /* hal_clock_cg_id (physical CG bit used)*/
        uint8_t stat;
    };

    /* MCU/DSP side both used CG: default clock enable
     * HAL_CLOCK_CG_UART_DMA_0, HAL_CLOCK_CG_I3C0, HAL_CLOCK_CG_I2C1
     * HAL_CLOCK_CG_SPIMST0, HAL_CLOCK_CG_I3C,
     * HAL_CLOCK_CG_FAST_DMA1, HAL_CLOCK_CG_SPIMST0_BUS, HAL_CLOCK_CG_I3C_DMA0, HAL_CLOCK_CG_I2C_DMA1, HAL_CLOCK_CG_TRNG
     */

    const struct cg_init_stat cg_init_tbl[] = {
        /* XO_PDN_PD_COND0(0x42040B00) */
        {.cg_id = HAL_CLOCK_CG_UART1, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_UART2, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_UART_DMA_0, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_UART_DMA_1, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_UART_DMA_2, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_IRRX, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_IRRX_BUS, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_I3C0, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_I3C1, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_I2C0, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_I2C1, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_I2C2, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_ROSC, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_UART0, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_AUXADC, .stat = CLK_REQ_DISABLE},
        /* XO_PDN_AO_COND0(0x42040B30) */
        {.cg_id = HAL_CLOCK_CG_PWM0, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_PWM1, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_PWM2, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_PWM3, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_PWM4, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_PWM5, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_PWM6, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_PWM7, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_PWM8, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_PWM9, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_PWM10, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_PWM11, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_PWM12, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_PWM13, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_SPM_PCLK, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_BCLK_CM33, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_BCLK_DSP, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_SPM_DIV, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_I2C_AO, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_OSTIMER, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_GPTIMER0, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_GPTIMER1, .stat = CLK_REQ_ENABLE},
        /* XO_PDN_TOP_COND0(0x42040B60) */
        {.cg_id = HAL_CLOCK_CG_SPIMST0, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_SPIMST1, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_SPIMST2, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_SDIOMST, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_I3C, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_BT_HOP, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_AUD_GPSRC, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_AUD_UPLINK, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_AUD_DWLINK, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_SPDIF, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_AUD_INTF0, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_AUD_INTF1, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_AUD_TEST, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_AUD_ANC, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_AUD_CLD, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_SFC, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_ESC, .stat = CLK_REQ_ENABLE}, /* ESC clk always on (avoid bus hang, for BT HW) */
        {.cg_id = HAL_CLOCK_CG_SPISLV, .stat = CLK_REQ_DISABLE},
#ifndef __EXT_DA__
        {.cg_id = HAL_CLOCK_CG_USB, .stat = CLK_REQ_DISABLE},
#endif
        {.cg_id = HAL_CLOCK_CG_AUD_INTBUS, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_DSP, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_SEJ, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_MIXEDSYS, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_EFUSE, .stat = CLK_REQ_ENABLE},
        /* XO_PDN_TOP_COND1(0x42040B90) */
        {.cg_id = HAL_CLOCK_CG_OSC1_D12_D2, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_OSC2_D10_D2, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_CMSYS_BUS, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_RSV_BUS, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_CONN_BUS, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_RSV_RTC_BUS, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_RSV_MCLK_BUS, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_AUD_ENGINE_BUS, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_AUD_VOW_BUS, .stat = CLK_REQ_DISABLE},
        /* PDN_PD_COND0(0x422B0300) */
        {.cg_id = HAL_CLOCK_CG_FAST_DMA0, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_FAST_DMA1, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_SPIMST0_BUS, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_SPIMST1_BUS, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_SPIMST2_BUS, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_I3C_DMA0, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_I3C_DMA1, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_I2C_DMA0, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_I2C_DMA1, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_I2C_DMA2, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_SECURITY_PD, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_AESOTF, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_AESOTF_ESC, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_CRYPTO, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_TRNG, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_SPISLV_BUS, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_SDIOMST0, .stat = CLK_REQ_DISABLE},
#ifndef __EXT_DA__
        {.cg_id = HAL_CLOCK_CG_USB_BUS, .stat = CLK_REQ_DISABLE},
        {.cg_id = HAL_CLOCK_CG_USB_DMA, .stat = CLK_REQ_DISABLE},
#endif
        {.cg_id = HAL_CLOCK_CG_SECURITY_PERI, .stat = CLK_REQ_ENABLE},
        /* PDN_AO_COND0(0x422B0330) */
        {.cg_id = HAL_CLOCK_CG_BUS_ERR, .stat = CLK_REQ_ENABLE},
        {.cg_id = HAL_CLOCK_CG_SEC_MON, .stat = CLK_REQ_ENABLE},
        /* PDN_TOP_COND0(0x422B0360) */
        {.cg_id = HAL_CLOCK_CG_CMSYS_ROM, .stat = CLK_REQ_ENABLE}
    };

    for (uint8_t idx = 0; idx < sizeof(cg_init_tbl) / sizeof(cg_init_tbl[0]); idx++) {
        if (cg_init_tbl[idx].stat == CLK_REQ_DISABLE) {
            hal_clock_disable(cg_init_tbl[idx].cg_id);
        } else {
            hal_clock_enable(cg_init_tbl[idx].cg_id);
        }
    }

}


hal_clock_status_t hal_clock_init(void)
{
    clk_dbg_log("hal_clock_init\r\n", 0);
#if 0
    /* Clock free run */
    *XO_PDN_PD_CLRD0 = 0xffffffff;
    *XO_PDN_AO_CLRD0 = 0xffffffff;
    *XO_PDN_TOP_CLRD0 = 0xffffffff;
    *XO_PDN_TOP_CLRD1 = 0xffffffff;
    *PDN_PD_CLRD0 = 0xffffffff;
    *PDN_TOP_CLRD0 = 0xffffffff;
    *PDN_AO_CLRD0 = 0xffffffff;
#endif
    /* Actual CG init settings */
    _hal_clock_cg_init();

    return clk_mux_init();
}

#if !defined(__EXT_BOOTLOADER__) && !defined(__EXT_DA__)
/**
 * hal_clock_chop_ck(): This function controls chop_ck functionality
 * parameter enable: enable/disable the function
 * div_sel: set divider setting (actual divder = div_sel * 2 + 2)
 *          div_sel valid value range: 0 ~ 2047
 */
hal_clock_status_t hal_clock_chop_ck(uint32_t enable, uint32_t div_sel)
{
    if (enable) {
        //uint32_t divider = div_sel * 2 + 2;
        CKSYS_CLK_DIV_REG.CHOP_DIV_SEL = div_sel;
        CKSYS_CLK_DIV_REG.CHOP_DIV_CHG = ~CKSYS_CLK_DIV_REG.CHOP_DIV_CHG;
        hal_gpt_delay_us(471);
    }

    CKSYS_CLK_DIV_REG.CHOP_DIV_EN = enable;

    clk_dbg_log("hal_clock_chop_ck() enable: %d, div_sel:%d (actual divider: %d)\r\n", 3,
                 (int)enable
                 , CKSYS_CLK_DIV_REG.CHOP_DIV_SEL
                 , CKSYS_CLK_DIV_REG.CHOP_DIV_SEL * 2 + 2);

    return HAL_CLOCK_STATUS_OK;
}
#endif


/* (*): OSC1 related freqs depends on whether AIR_RFI_SUPPRESS_DISABLE is defined
 *      when AIR_RFI_SUPPRESS_DISABLE is     defined: freq is the higher value
 *                                       not defined: freq is the lower value
 *    OSC1 freq. code comment format
 *    -> "defined freq"/"not defined freq"
 */
ATTR_TEXT_IN_TCM const static clock_tbl_t clock_tbl = {
    .mux_tbl = {
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},     //CLK_SYS_SEL        // 0 : F_FXO_CK     26MHz
                {CLK_OSC1, CLK_DIV_NONE, MUX_NOT_ALLOW},                // 1 : OSC_26       26MHz
                {CLK_OSC1, CLK_DIV_D2, MUX_ALLOW},                      // 2 : OSC1_D2      156/149.625MHz (*)
                {CLK_OSC1, CLK_DIV_D3, MUX_ALLOW},                      // 3 : OSC1_D3      104/99.75MHz   (*)
                {CLK_OSC1, CLK_DIV_D2, MUX_ALLOW},                      // 4 : OSC1_D2_D2   78/74.8125MHz  (*)
                {CLK_OSC2, CLK_DIV_D2, MUX_ALLOW},                      // 5 : OSC2_D2      260MHz
                {CLK_OSC2, CLK_DIV_D2, MUX_ALLOW},                      // 6 : OSC2_D2_D2   130MHz
                {CLK_UPLL, CLK_DIV_D2, MUX_ALLOW}                       // 7 : UPLL_D2      156MHz
            },
            .mux_type = PHYSICAL_N_TO_1_MUX,
            .curr_sel_ptr = BYTE_REG(CKSYS_CLK_CFG_REG.CLK_SYS_SEL),
            .cg_info = { .cg_id = HAL_CLOCK_CG_CMSYS_BUS, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},     //CLK_DSP_SEL        // 0 : F_FXO_CK     26MHz
                {CLK_OSC1, CLK_DIV_NONE, MUX_NOT_ALLOW},                // 1 : OSC_26       26MHz
                {CLK_OSC1, CLK_DIV_NONE, MUX_ALLOW},                    // 2 : OSC1         312/299.25MHz   (*)
                {CLK_OSC1, CLK_DIV_D2, MUX_ALLOW},                      // 3 : OSC1_D2      156/149.625MHz  (*)
                {CLK_OSC1, CLK_DIV_D3, MUX_ALLOW},                      // 4 : OSC1_D3      104/99.75MHz    (*)
                {CLK_OSC2, CLK_DIV_NONE, MUX_ALLOW},                    // 5 : OSC2         520MHz
                {CLK_OSC2, CLK_DIV_D5, MUX_ALLOW},                      // 6 : OSC2_D5      104MHz
                {CLK_UPLL, CLK_DIV_D2, MUX_ALLOW}                       // 7 : UPLL_D2      156MHz
            },
            .mux_type = PHYSICAL_N_TO_1_MUX,
            .curr_sel_ptr = BYTE_REG(CKSYS_CLK_CFG_REG.CLK_DSP_SEL),
            .cg_info = { .cg_id = HAL_CLOCK_CG_DSP, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO,  CLK_DIV_NONE, MUX_ALLOW},//CLK_SFC_SEL            // 0 : F_FXO_CK     26MHz
                {CLK_OSC1, CLK_DIV_NONE, MUX_NOT_ALLOW},                // 1 : OSC_26       26MHz
                {CLK_OSC1, CLK_DIV_D3, MUX_ALLOW},                      // 2 : OSC1_D3      104/99.75MHz   (*)
                {CLK_OSC1, CLK_DIV_D3, MUX_ALLOW},                      // 3 : OSC1_D3_D2   52/49.875MHz   (*)
                {CLK_OSC1, CLK_DIV_D8, MUX_ALLOW},                      // 4 : OSC1_D8      39/37.40625MHz (*)
                {CLK_OSC2, CLK_DIV_D3, MUX_NOT_ALLOW},                  // 5 : OSC2_D3      173.33MHz
                {CLK_OSC2, CLK_DIV_D5, MUX_NOT_ALLOW},                  // 6 : OSC2_D5      104MHz
                {CLK_UPLL, CLK_DIV_D3, MUX_ALLOW}                       // 7 : UPLL_D3      104MHz
            },
            .mux_type = PHYSICAL_N_TO_1_MUX,
            .curr_sel_ptr = BYTE_REG(CKSYS_CLK_CFG_REG.CLK_SFC_SEL),
            .cg_info = { .cg_id = HAL_CLOCK_CG_SFC, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO,  CLK_DIV_NONE, MUX_ALLOW},//CLK_ESC_SEL            // 0 : F_FXO_CK     26MHz
                {CLK_OSC1, CLK_DIV_D3, MUX_ALLOW},                      // 1 : OSC1_D3      104/99.75MHz    (*)
                {CLK_OSC1, CLK_DIV_D2, MUX_ALLOW},                      // 2 : OSC1_D2_D2   78/74.8125MHz   (*)
                {CLK_OSC1, CLK_DIV_D3, MUX_ALLOW},                      // 3 : OSC1_D3_D2   52/49.875MHz    (*)
                {CLK_OSC1, CLK_DIV_D8, MUX_ALLOW},                      // 4 : OSC1_D8      39/37.40625MHz  (*)
                {CLK_OSC2, CLK_DIV_D5, MUX_NOT_ALLOW},                  // 5 : OSC2_D5      104MHz
                {CLK_OSC2, CLK_DIV_D3, MUX_NOT_ALLOW},                  // 6 : OSC2_D3_D2   86.67MHz
                {CLK_UPLL, CLK_DIV_D3, MUX_ALLOW}                       // 7 : UPLL_D3      104MHz
            },
            .mux_type = PHYSICAL_N_TO_1_MUX,
            .curr_sel_ptr = BYTE_REG(CKSYS_CLK_CFG_REG.CLK_ESC_SEL),
            .cg_info = { .cg_id = HAL_CLOCK_CG_ESC, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},    //CLK_SPIMST0_SEL     // 0 : F_FXO_CK     26MHz
                {CLK_OSC1, CLK_DIV_D3, MUX_ALLOW},                      // 1 : OSC1_D3      104/99.75MHz    (*)
                {CLK_OSC1, CLK_DIV_D2, MUX_ALLOW},                      // 2 : OSC1_D2_D2   78/74.8125MHz   (*)
                {CLK_OSC1, CLK_DIV_D3, MUX_ALLOW},                      // 3 : OSC1_D3_D2   52/49.875MHz    (*)
                {CLK_OSC1, CLK_DIV_D8, MUX_ALLOW},                      // 4 : OSC1_D8      39/37.40625MHz  (*)
                {CLK_OSC2, CLK_DIV_D5, MUX_NOT_ALLOW},                  // 5 : OSC2_D5      104MHz
                {CLK_OSC2, CLK_DIV_D3, MUX_NOT_ALLOW},                  // 6 : OSC2_D3_D2   86.67MHz
                {CLK_UPLL, CLK_DIV_D3, MUX_ALLOW}                       // 7 : UPLL_D3      104MHz
            },
            .mux_type = PHYSICAL_N_TO_1_MUX,
            .curr_sel_ptr = BYTE_REG(CKSYS_CLK_CFG_REG.CLK_SPIMST0_SEL),
            .cg_info = { .cg_id = HAL_CLOCK_CG_SPIMST0_BUS, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},    //CLK_SPIMST1_SEL     // 0 : F_FXO_CK     26MHz
                {CLK_OSC1, CLK_DIV_D3, MUX_ALLOW},                      // 1 : OSC1_D3      104/99.75MHz    (*)
                {CLK_OSC1, CLK_DIV_D2, MUX_ALLOW},                      // 2 : OSC1_D2_D2   78/74.8125MHz   (*)
                {CLK_OSC1, CLK_DIV_D3, MUX_ALLOW},                      // 3 : OSC1_D3_D2   52/49.875MHz    (*)
                {CLK_OSC1, CLK_DIV_D8, MUX_ALLOW},                      // 4 : OSC1_D8      39/37.40625MHz  (*)
                {CLK_OSC2, CLK_DIV_D5, MUX_NOT_ALLOW},                  // 5 : OSC2_D5      104MHz
                {CLK_OSC2, CLK_DIV_D3, MUX_NOT_ALLOW},                  // 6 : OSC2_D3_D2   86.67MHz
                {CLK_UPLL, CLK_DIV_D3, MUX_ALLOW}                       // 7 : UPLL_D3      104MHz
            },
            .mux_type = PHYSICAL_N_TO_1_MUX,
            .curr_sel_ptr = BYTE_REG(CKSYS_CLK_CFG_REG.CLK_SPIMST1_SEL),
            .cg_info = { .cg_id = HAL_CLOCK_CG_SPIMST1_BUS, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},    //CLK_SPIMST2_SEL     // 0 : F_FXO_CK     26MHz
                {CLK_OSC1, CLK_DIV_D3, MUX_ALLOW},                      // 1 : OSC1_D3      104/99.75MHz    (*)
                {CLK_OSC1, CLK_DIV_D2, MUX_ALLOW},                      // 2 : OSC1_D2_D2   78/74.8125MHz   (*)
                {CLK_OSC1, CLK_DIV_D3, MUX_ALLOW},                      // 3 : OSC1_D3_D2   52/49.875MHz    (*)
                {CLK_OSC1, CLK_DIV_D8, MUX_ALLOW},                      // 4 : OSC1_D8      39/37.40625MHz  (*)
                {CLK_OSC2, CLK_DIV_D5, MUX_NOT_ALLOW},                  // 5 : OSC2_D5      104MHz
                {CLK_OSC2, CLK_DIV_D3, MUX_NOT_ALLOW},                  // 6 : OSC2_D3_D2   86.67MHz
                {CLK_UPLL, CLK_DIV_D3, MUX_ALLOW}                       // 7 : UPLL_D3      104MHz
            },
            .mux_type = PHYSICAL_N_TO_1_MUX,
            .curr_sel_ptr = BYTE_REG(CKSYS_CLK_CFG_REG.CLK_SPIMST2_SEL),
            .cg_info = { .cg_id = HAL_CLOCK_CG_SPIMST2_BUS, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},    //CLK_SPISLV_SEL     // 0 : F_FXO_CK     26MHz
                {CLK_OSC1, CLK_DIV_D3, MUX_ALLOW},                     // 1 : OSC1_D3      104/99.75MHz    (*)
                {CLK_OSC1, CLK_DIV_D2, MUX_ALLOW},                     // 2 : OSC1_D2_D2   78/74.8125MHz   (*)
                {CLK_OSC1, CLK_DIV_D3, MUX_ALLOW},                     // 3 : OSC1_D3_D2   52/49.875MHz    (*)
                {CLK_OSC1, CLK_DIV_D8, MUX_ALLOW},                     // 4 : OSC1_D8      39/37.40625MHz  (*)
                {CLK_OSC2, CLK_DIV_D5, MUX_NOT_ALLOW},                 // 5 : OSC2_D5      104MHz
                {CLK_OSC2, CLK_DIV_D3, MUX_NOT_ALLOW},                 // 6 : OSC2_D3_D2   86.67MHz
                {CLK_UPLL, CLK_DIV_D3, MUX_ALLOW}                      // 7 : UPLL_D3      104MHz
            },
            .mux_type = PHYSICAL_N_TO_1_MUX,
            .curr_sel_ptr = BYTE_REG(CKSYS_CLK_CFG_REG.CLK_SPISLV_SEL),
            .cg_info = { .cg_id = HAL_CLOCK_CG_SPISLV_BUS, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},    //CLK_SDIOMST0_SEL    // 0 : F_FXO_CK     26MHz
                {CLK_OSC1, CLK_DIV_D3, MUX_ALLOW},                      // 1 : OSC1_D3_D2   52/49.875MHz      (*)
                {CLK_OSC1, CLK_DIV_D8, MUX_ALLOW},                      // 2 : OSC1_D8      39/37.40625MHz    (*)
                {CLK_OSC1, CLK_DIV_D12, MUX_ALLOW},                     // 3 : OSC1_D12     26/24.9375MHz     (*)
                {CLK_OSC1, CLK_DIV_D8, MUX_ALLOW},                      // 4 : OSC1_D8_D2   19.5/18.703125MHz (*)
                {CLK_OSC2, CLK_DIV_D5, MUX_NOT_ALLOW},                  // 5 : OSC2_D5_D2   52MHz
                {CLK_OSC2, CLK_DIV_D12, MUX_NOT_ALLOW},                 // 6 : OSC2_D12     43.33MHz
                {CLK_UPLL, CLK_DIV_D3, MUX_ALLOW}                       // 7 : UPLL_D3      104MHz
            },
            .mux_type = PHYSICAL_N_TO_1_MUX,
            .curr_sel_ptr = BYTE_REG(CKSYS_CLK_CFG_REG.CLK_SDIOMST0_SEL),
            .cg_info = { .cg_id = HAL_CLOCK_CG_SDIOMST0, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},    //CLK_USB_SEL         // 0 : F_FXO_CK     26MHz
                {CLK_OSC1, CLK_DIV_D12, MUX_ALLOW},                     // 1 : OSC1_D12     26/24.9375MHz    (*)
                {CLK_OSC2, CLK_DIV_D5, MUX_NOT_ALLOW},                  // 2 : OSC2_D5_D2   52MHz
                {CLK_UPLL, CLK_DIV_D5, MUX_ALLOW},                      // 3 : UPLL_D5      62.4MHz
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW}
            },
            .mux_type = PHYSICAL_N_TO_1_MUX,
            .curr_sel_ptr = BYTE_REG(CKSYS_CLK_CFG_REG.CLK_USB_SEL),
            .cg_info = { .cg_id = HAL_CLOCK_CG_USB, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},    //CLK_I3C_SEL         // 0 : F_FXO_CK     26MHz
                {CLK_OSC1, CLK_DIV_D3, MUX_ALLOW},                      // 1 : OSC1_D3      104/99.75MHz    (*)
                {CLK_OSC1, CLK_DIV_D3, MUX_NOT_ALLOW},                  // 2 : OSC1_D3_D2   52/49.875MHz    (*)
                {CLK_OSC2, CLK_DIV_D5, MUX_ALLOW},                      // 3 : OSC2_D5      104MHz
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW}
            },
            .mux_type = PHYSICAL_N_TO_1_MUX,
            .curr_sel_ptr = BYTE_REG(CKSYS_CLK_CFG_REG.CLK_I3C_SEL),
            .cg_info = { .cg_id = HAL_CLOCK_CG_I3C, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},    //CLK_BT_HOP_SEL      // 0 : F_FXO_CK     26MHz
                {CLK_OSC1, CLK_DIV_D3, MUX_ALLOW},                      // 1 : OSC1_D3      104/99.75MHz    (*)
                {CLK_OSC1, CLK_DIV_D3, MUX_ALLOW},                      // 2 : OSC1_D3_D2   52/49.875MHz    (*)
                {CLK_OSC2, CLK_DIV_D5, MUX_NOT_ALLOW},                  // 3 : OSC2_D5      104MHz
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW}
            },
            .mux_type = PHYSICAL_N_TO_1_MUX,
            .curr_sel_ptr = BYTE_REG(CKSYS_CLK_CFG_REG.CLK_BT_HOP_SEL),
            .cg_info = { .cg_id = HAL_CLOCK_CG_BT_HOP, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW}, //CLK_AUD_BUS_SEL        // 0 : F_FXO_CK     26MHz
                {CLK_OSC1, CLK_DIV_D2, MUX_ALLOW},                      // 1 : OSC1_D2      156/149.625MHz  (*)
                {CLK_OSC1, CLK_DIV_D3, MUX_ALLOW},                      // 2 : OSC1_D3      104/99.75MHz    (*)
                {CLK_OSC1, CLK_DIV_D2, MUX_ALLOW},                      // 3 : OSC1_D2_D2   78/74.8125MHz   (*)
                {CLK_OSC1, CLK_DIV_D3, MUX_ALLOW},                      // 4 : OSC1_D3_D2   52/49.875MHz    (*)
                {CLK_OSC2, CLK_DIV_D2, MUX_NOT_ALLOW},                  // 5 : OSC2_D2      260MHz
                {CLK_OSC2, CLK_DIV_D3, MUX_NOT_ALLOW},                  // 6 : OSC2_D3      173.33MHz
                {CLK_OSC2, CLK_DIV_D2, MUX_NOT_ALLOW}                   // 7 : OSC2_D2_D2   130MHz
            },
            .mux_type = PHYSICAL_N_TO_1_MUX,
            .curr_sel_ptr = BYTE_REG(CKSYS_CLK_CFG_REG.CLK_AUD_BUS_SEL),
            .cg_info = { .cg_id = HAL_CLOCK_CG_AUD_INTBUS, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},    //CLK_AUD_GPSRC_SEL  // 0 : F_FXO_CK     26MHz
                {CLK_OSC1, CLK_DIV_D2, MUX_ALLOW},                     // 1 : OSC1_D2      156/149.625MHz   (*)
                {CLK_OSC1, CLK_DIV_D3, MUX_ALLOW},                     // 2 : OSC1_D3      104/99.75MHz     (*)
                {CLK_OSC1, CLK_DIV_D2, MUX_ALLOW},                     // 3 : OSC1_D2_D2   78/74.8125MHz    (*)
                {CLK_OSC1, CLK_DIV_D3, MUX_ALLOW},                     // 4 : OSC1_D3_D2   52/49.875MHz     (*)
                {CLK_OSC2, CLK_DIV_D2, MUX_NOT_ALLOW},                 // 5 : OSC2_D2      260MHz
                {CLK_OSC2, CLK_DIV_D3, MUX_NOT_ALLOW},                 // 6 : OSC2_D3      173.33MHz
                {CLK_OSC2, CLK_DIV_D2, MUX_ALLOW}                      // 7 : OSC2_D2_D2   130MHz
            },
            .mux_type = PHYSICAL_N_TO_1_MUX,
            .curr_sel_ptr = BYTE_REG(CKSYS_CLK_CFG_REG.CLK_AUD_GPSRC_SEL),
            .cg_info = { .cg_id = HAL_CLOCK_CG_AUD_GPSRC, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},     //CLK_AUD_ULCK_SEL   // 0 : F_FXO_CK     26MHz
                {CLK_OSC1, CLK_DIV_NONE, MUX_ALLOW},                    // 1 : OSC1         312/299.25MHz   (*)
                {CLK_OSC1, CLK_DIV_D2, MUX_ALLOW},                      // 2 : OSC1_D2      156/149.625MHz  (*)
                {CLK_OSC2, CLK_DIV_D2, MUX_NOT_ALLOW},                  // 3 : OSC2_D2      260MHz
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW}
            },
            .mux_type = PHYSICAL_N_TO_1_MUX,
            .curr_sel_ptr = BYTE_REG(CKSYS_CLK_CFG_REG.CLK_AUD_ULCK_SEL),
            .cg_info = { .cg_id = HAL_CLOCK_CG_AUD_UPLINK, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},     //CLK_AUD_DLCK_SEL   // 0 : F_FXO_CK     26MHz
                {CLK_OSC1, CLK_DIV_D2, MUX_ALLOW},                      // 1 : OSC1_D2      156/149.625MHz  (*)
                {CLK_OSC1, CLK_DIV_D2, MUX_ALLOW},                      // 2 : OSC1_D2_D2   78/74.8125MHz   (*)
                {CLK_OSC2, CLK_DIV_D2, MUX_NOT_ALLOW},                  // 3 : OSC2_D2_D2   130MHz
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW}
            },
            .mux_type = PHYSICAL_N_TO_1_MUX,
            .curr_sel_ptr = BYTE_REG(CKSYS_CLK_CFG_REG.CLK_AUD_DLCK_SEL),
            .cg_info = { .cg_id = HAL_CLOCK_CG_AUD_DWLINK, .cg_type = PHYSICAL_CG}
        },

        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},      //CLK_AUD_INTERFACE0_SEL // 0 : F_FXO_CK     13MHz
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},                       // 1 : Reserved
                {CLK_APLL1, CLK_DIV_NONE, MUX_ALLOW},                        // 2 : APLL1        22.579MHz
                {CLK_APLL2, CLK_DIV_NONE, MUX_ALLOW},                        // 3 : APLL2        24.576MHz
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW}
            },
            .mux_type = PHYSICAL_NORM_MUX,
            .curr_sel_ptr = BYTE_REG(CKSYS_CLK_CFG_REG.CLK_AUD_INTF0_SEL),
            .cg_info = { .cg_id = HAL_CLOCK_CG_AUD_INTF0, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},      //CLK_AUD_INTERFACE1_SEL // 0 : F_FXO_CK     13MHz
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},                       // 1 : Reserved
                {CLK_APLL1, CLK_DIV_NONE, MUX_ALLOW},                        // 2 : APLL1        22.579MHz
                {CLK_APLL2, CLK_DIV_NONE, MUX_ALLOW},                        // 3 : APLL2        24.576MHz
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW}
            },
            .mux_type = PHYSICAL_NORM_MUX,
            .curr_sel_ptr = BYTE_REG(CKSYS_CLK_CFG_REG.CLK_AUD_INTF1_SEL),
            .cg_info = { .cg_id = HAL_CLOCK_CG_AUD_INTF1, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},     // CLK_26M_SEL           // 0 : F_FXO_CK     26MHz
                {CLK_OSC1, CLK_DIV_26M, MUX_NOT_ALLOW},                     // 1 : OSC_26       26/24.9375MHz  (*)
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW}
            },
            .mux_type = PHYSICAL_NORM_MUX,
            .curr_sel_ptr = BYTE_REG(CKSYS_CLK_CFG_REG.CLK_26M_SEL),
            .cg_info = { .cg_id = HAL_CLOCK_CG_PSUEDO_CLK_26M, .cg_type = PSUEDO_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},     //CLK_AUD_ENGINE_SEL 0 : // 0 : F_FXO_CK     26MHz
                {CLK_OSC1, CLK_DIV_26M, MUX_NOT_ALLOW},                     // 1 : OSC_26       26/24.9375MHz  (*)
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW}
            },
            .mux_type = PHYSICAL_NORM_MUX,
            .curr_sel_ptr = BYTE_REG(CKSYS_CLK_CFG_REG.CLK_AUD_ENGINE_SEL),
            .cg_info = { .cg_id = HAL_CLOCK_CG_AUD_ENGINE_BUS, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},    //CLK_AUD_VOW_SEL     // 0 : F_FXO_CK     13MHz
                {CLK_OSC2, CLK_DIV_26M, MUX_NOT_ALLOW},                 // 1 : OSC_26       13/12.46875MHz  (*)
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW}
            },
            .mux_type = PHYSICAL_NORM_MUX,
            .curr_sel_ptr = BYTE_REG(CKSYS_CLK_CFG_REG.CLK_VOW_SEL),
            .cg_info = { .cg_id = HAL_CLOCK_CG_AUD_VOW_BUS, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_OSC1, CLK_DIV_D12, MUX_NOT_ALLOW},   // CLK_OSC_26M_SEL    // 0 : OSC1_D12     26/24.9375MHz (*)
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},                      // 1 : Reserved
                {CLK_OSC2, CLK_DIV_D5, MUX_NOT_ALLOW},                      // 2 : OSC2_D5_D2   26MHz
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW}
            },
            .mux_type = PHYSICAL_NORM_MUX,
            .curr_sel_ptr = BYTE_REG(CKSYS_CLK_CFG_REG.CLK_26M_SEL),
            .cg_info = { .cg_id = HAL_CLOCK_CG_PSUEDO_CLK_OSC_26M, .cg_type = PSUEDO_CG}
        },
        /* MCLK_SEL: special case, enable psuedo CG upon 1st mux_sel call (for I2S?) */
        {
            .sels = {{CLK_APLL1, CLK_DIV_NONE, MUX_ALLOW},     //CLK_MCLK_SEL   // 0 : APLL1        45.1584MHz
                {CLK_APLL2, CLK_DIV_NONE, MUX_ALLOW},                   // 1 : APLL2        49.152MHz
                {CLK_XO, CLK_DIV_NONE, MUX_ALLOW},                      // 2 : F_FXO_CK     26MHz
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW}
            },
            .mux_type = PSUEDO_MUX,
            .curr_sel_ptr = &psuedo_mux_arr[CLK_MCLK_SEL - CLK_MCLK_SEL],
            .cg_info = { .cg_id = HAL_CLOCK_CG_PSUEDO_MCLK, .cg_type = PSUEDO_CG}
        },
        /* SPDIF doesn't have a physical mux, add this definition to enable UPLL 624MHz for SPDIF */
        {
            .sels = {{CLK_UPLL, CLK_DIV_NONE, MUX_ALLOW},          //CLK_SPDIF_SEL   0 : UPLL    624MHz
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW}
            },
            .mux_type = PSUEDO_MUX,
            .curr_sel_ptr = &psuedo_mux_arr[CLK_SPDIF_SEL - CLK_MCLK_SEL],
            .cg_info = { .cg_id = HAL_CLOCK_CG_SPDIF, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},     //CLK_PWM0_SEL      // 0 : F_FXO_CK     26MHz
                {CLK_XO, CLK_DIV_NONE, MUX_ALLOW},                      // 1 : F_FRTC_CK    0.032MHz
                {CLK_OSC1, CLK_DIV_D8, MUX_ALLOW},                      // 2 : OSC1_D8      39/37.40625MHz    (*)
                {CLK_OSC2, CLK_DIV_D3, MUX_ALLOW},                      // 3 : OSC2_D3_D2   86.67MHz
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW}
            },
            .mux_type = PSUEDO_MUX,
            .curr_sel_ptr = &psuedo_mux_arr[CLK_PWM0_SEL - CLK_MCLK_SEL],
            .cg_info = { .cg_id = HAL_CLOCK_CG_PWM0, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},     //CLK_PWM1_SEL      // 0 : F_FXO_CK     26MHz
                {CLK_XO, CLK_DIV_NONE, MUX_ALLOW},                      // 1 : F_FRTC_CK    0.032MHz
                {CLK_OSC1, CLK_DIV_D8, MUX_ALLOW},                      // 2 : OSC1_D8      39/37.40625MHz    (*)
                {CLK_OSC2, CLK_DIV_D3, MUX_ALLOW},                      // 3 : OSC2_D3_D2   86.67MHz
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW}
            },
            .mux_type = PSUEDO_MUX,
            .curr_sel_ptr = &psuedo_mux_arr[CLK_PWM1_SEL - CLK_MCLK_SEL],
            .cg_info = { .cg_id = HAL_CLOCK_CG_PWM1, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},     //CLK_PWM2_SEL      // 0 : F_FXO_CK     26MHz
                {CLK_XO, CLK_DIV_NONE, MUX_ALLOW},                      // 1 : F_FRTC_CK    0.032MHz
                {CLK_OSC1, CLK_DIV_D8, MUX_ALLOW},                      // 2 : OSC1_D8      39/37.40625MHz    (*)
                {CLK_OSC2, CLK_DIV_D3, MUX_ALLOW},                      // 3 : OSC2_D3_D2   86.67MHz
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW}
            },
            .mux_type = PSUEDO_MUX,
            .curr_sel_ptr = &psuedo_mux_arr[CLK_PWM2_SEL - CLK_MCLK_SEL],
            .cg_info = { .cg_id = HAL_CLOCK_CG_PWM2, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},     //CLK_PWM3_SEL      // 0 : F_FXO_CK     26MHz
                {CLK_XO, CLK_DIV_NONE, MUX_ALLOW},                      // 1 : F_FRTC_CK    0.032MHz
                {CLK_OSC1, CLK_DIV_D8, MUX_ALLOW},                      // 2 : OSC1_D8      39/37.40625MHz    (*)
                {CLK_OSC2, CLK_DIV_D3, MUX_ALLOW},                      // 3 : OSC2_D3_D2   86.67MHz
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW}
            },
            .mux_type = PSUEDO_MUX,
            .curr_sel_ptr = &psuedo_mux_arr[CLK_PWM3_SEL - CLK_MCLK_SEL],
            .cg_info = { .cg_id = HAL_CLOCK_CG_PWM3, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},     //CLK_PWM4_SEL      // 0 : F_FXO_CK     26MHz
                {CLK_XO, CLK_DIV_NONE, MUX_ALLOW},                      // 1 : F_FRTC_CK    0.032MHz
                {CLK_OSC1, CLK_DIV_D8, MUX_ALLOW},                      // 2 : OSC1_D8      39/37.40625MHz    (*)
                {CLK_OSC2, CLK_DIV_D3, MUX_ALLOW},                      // 3 : OSC2_D3_D2   86.67MHz
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW}
            },
            .mux_type = PSUEDO_MUX,
            .curr_sel_ptr = &psuedo_mux_arr[CLK_PWM4_SEL - CLK_MCLK_SEL],
            .cg_info = { .cg_id = HAL_CLOCK_CG_PWM4, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},     //CLK_PWM5_SEL      // 0 : F_FXO_CK     26MHz
                {CLK_XO, CLK_DIV_NONE, MUX_ALLOW},                      // 1 : F_FRTC_CK    0.032MHz
                {CLK_OSC1, CLK_DIV_D8, MUX_ALLOW},                      // 2 : OSC1_D8      39/37.40625MHz    (*)
                {CLK_OSC2, CLK_DIV_D3, MUX_ALLOW},                      // 3 : OSC2_D3_D2   86.67MHz
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW}
            },
            .mux_type = PSUEDO_MUX,
            .curr_sel_ptr = &psuedo_mux_arr[CLK_PWM5_SEL - CLK_MCLK_SEL],
            .cg_info = { .cg_id = HAL_CLOCK_CG_PWM5, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},     //CLK_PWM6_SEL      // 0 : F_FXO_CK     26MHz
                {CLK_XO, CLK_DIV_NONE, MUX_ALLOW},                      // 1 : F_FRTC_CK    0.032MHz
                {CLK_OSC1, CLK_DIV_D8, MUX_ALLOW},                      // 2 : OSC1_D8      39/37.40625MHz    (*)
                {CLK_OSC2, CLK_DIV_D3, MUX_ALLOW},                      // 3 : OSC2_D3_D2   86.67MHz
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW}
            },
            .mux_type = PSUEDO_MUX,
            .curr_sel_ptr = &psuedo_mux_arr[CLK_PWM6_SEL - CLK_MCLK_SEL],
            .cg_info = { .cg_id = HAL_CLOCK_CG_PWM6, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},     //CLK_PWM7_SEL      // 0 : F_FXO_CK     26MHz
                {CLK_XO, CLK_DIV_NONE, MUX_ALLOW},                      // 1 : F_FRTC_CK    0.032MHz
                {CLK_OSC1, CLK_DIV_D8, MUX_ALLOW},                      // 2 : OSC1_D8      39/37.40625MHz  (*)
                {CLK_OSC2, CLK_DIV_D3, MUX_ALLOW},                      // 3 : OSC2_D3_D2   86.67MHz
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW}
            },
            .mux_type = PSUEDO_MUX,
            .curr_sel_ptr = &psuedo_mux_arr[CLK_PWM7_SEL - CLK_MCLK_SEL],
            .cg_info = { .cg_id = HAL_CLOCK_CG_PWM7, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},     //CLK_PWM8_SEL      // 0 : F_FXO_CK     26MHz
                {CLK_XO, CLK_DIV_NONE, MUX_ALLOW},                      // 1 : F_FRTC_CK    0.032MHz
                {CLK_OSC1, CLK_DIV_D8, MUX_ALLOW},                      // 2 : OSC1_D8      39/37.40625MHz  (*)
                {CLK_OSC2, CLK_DIV_D3, MUX_ALLOW},                      // 3 : OSC2_D3_D2   86.67MHz
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW}
            },
            .mux_type = PSUEDO_MUX,
            .curr_sel_ptr = &psuedo_mux_arr[CLK_PWM8_SEL - CLK_MCLK_SEL],
            .cg_info = { .cg_id = HAL_CLOCK_CG_PWM8, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},     //CLK_PWM9_SEL      // 0 : F_FXO_CK     26MHz
                {CLK_XO, CLK_DIV_NONE, MUX_ALLOW},                      // 1 : F_FRTC_CK    0.032MHz
                {CLK_OSC1, CLK_DIV_D8, MUX_ALLOW},                      // 2 : OSC1_D8      39/37.40625MHz  (*)
                {CLK_OSC2, CLK_DIV_D3, MUX_ALLOW},                      // 3 : OSC2_D3_D2   86.67MHz
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW}
            },
            .mux_type = PSUEDO_MUX,
            .curr_sel_ptr = &psuedo_mux_arr[CLK_PWM9_SEL - CLK_MCLK_SEL],
            .cg_info = { .cg_id = HAL_CLOCK_CG_PWM9, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},     //CLK_PWM10_SEL     // 0 : F_FXO_CK     26MHz
                {CLK_XO, CLK_DIV_NONE, MUX_ALLOW},                      // 1 : F_FRTC_CK    0.032MHz
                {CLK_OSC1, CLK_DIV_D8, MUX_ALLOW},                      // 2 : OSC1_D8      39/37.40625MHz  (*)
                {CLK_OSC2, CLK_DIV_D3, MUX_ALLOW},                      // 3 : OSC2_D3_D2   86.67MHz
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW}
            },
            .mux_type = PSUEDO_MUX,
            .curr_sel_ptr = &psuedo_mux_arr[CLK_PWM10_SEL - CLK_MCLK_SEL],
            .cg_info = { .cg_id = HAL_CLOCK_CG_PWM10, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},     //CLK_PWM11_SEL     // 0 : F_FXO_CK     26MHz
                {CLK_XO, CLK_DIV_NONE, MUX_ALLOW},                      // 1 : F_FRTC_CK    0.032MHz
                {CLK_OSC1, CLK_DIV_D8, MUX_ALLOW},                      // 2 : OSC1_D8      39/37.40625MHz  (*)
                {CLK_OSC2, CLK_DIV_D3, MUX_ALLOW},                      // 3 : OSC2_D3_D2   86.67MHz
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW}
            },
            .mux_type = PSUEDO_MUX,
            .curr_sel_ptr = &psuedo_mux_arr[CLK_PWM11_SEL - CLK_MCLK_SEL],
            .cg_info = { .cg_id = HAL_CLOCK_CG_PWM11, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},     //CLK_PWM12_SEL     // 0 : F_FXO_CK     26MHz
                {CLK_XO, CLK_DIV_NONE, MUX_ALLOW},                      // 1 : F_FRTC_CK    0.032MHz
                {CLK_OSC1, CLK_DIV_D8, MUX_ALLOW},                      // 2 : OSC1_D8      39/37.40625MHz  (*)
                {CLK_OSC2, CLK_DIV_D3, MUX_ALLOW},                      // 3 : OSC2_D3_D2   86.67MHz
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW}
            },
            .mux_type = PSUEDO_MUX,
            .curr_sel_ptr = &psuedo_mux_arr[CLK_PWM12_SEL - CLK_MCLK_SEL],
            .cg_info = { .cg_id = HAL_CLOCK_CG_PWM12, .cg_type = PHYSICAL_CG}
        },
        {
            .sels = {{CLK_XO, CLK_DIV_NONE, MUX_ALLOW},     //CLK_PWM13_SEL     // 0 : F_FXO_CK     26MHz
                {CLK_XO, CLK_DIV_NONE, MUX_ALLOW},                      // 1 : F_FRTC_CK    0.032MHz
                {CLK_OSC1, CLK_DIV_D8, MUX_ALLOW},                      // 2 : OSC1_D8      39/37.40625MHz  (*)
                {CLK_OSC2, CLK_DIV_D3, MUX_ALLOW},                      // 3 : OSC2_D3_D2   86.67MHz
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW},
                {CLK_XO, CLK_DIV_NONE, MUX_NOT_ALLOW}
            },
            .mux_type = PSUEDO_MUX,
            .curr_sel_ptr = &psuedo_mux_arr[CLK_PWM13_SEL - CLK_MCLK_SEL],
            .cg_info = { .cg_id = HAL_CLOCK_CG_PWM13, .cg_type = PHYSICAL_CG}
        },
    }
};
#endif /* HAL_CLOCK_MODULE_ENABLED */

