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

/**
 * File: nvdm_config_factory_reset.h
 *
 * Description: This file defines the interface of nvdm_config_factory_reset.c.
 *
 */

#ifndef __NVDM_CONFIG_FACTORY_RST_H__
#define __NVDM_CONFIG_FACTORY_RST_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MTK_NVDM_ENABLE

#define FACTORY_RESET_FLAG      (0x55)    /* Flag write in NVDM to indicate that factory reset is requested, all NVDM items except for those in FACTORY_RST_RESERVED_NVDM_ITEM_LIST will be deleted. */
#define FACTORY_RESET_LINK_KEY  (0x56)    /* Flag write in NVDM to indicate that link key reset is requested, only link related NVkey items will be deleted. */

/*
 * This macro defines table of NVDM items that will be reserved after factory reset. Items not in the table will be deleted.
 * 1. The user should put the NVDM items which need to be reserved after factory reset here with group name and item name.
 * 2. Those reserved NVDM items should be reviewed carefully before put them here.
 * 3. See mcu/middleware/airoha/nvdm/inc/nvkey_id_list.h and mcu/middleware/airoha/nvdm/inc/nvdm_id_list.h for more nvdm item details.
 */
#define FACTORY_RST_RESERVED_NVDM_ITEM_LIST \
{\
    /* NVDM internal use. */ \
    {NVDM_INTERNAL_USE_GROUP, NVDM_USE_SETTING},\
    /* BT relative. */        \
    {NVDM_GROUP_BT, NVDM_BT_ITEM_LOCAL_ADDR},\
    {NVDM_GROUP_BT_APP, NVDM_BT_APP_ITEM_FIX_ADDR},\
    {NVDM_GROUP_CONTEXT_INFO, NVDM_CONTEXT_INFO_ITEM_INDEX},\
    {NVDM_GROUP_CONTEXT_INFO, NVDM_CONTEXT_INFO_ITEM_INFO_0},\
    {NVDM_GROUP_CONTEXT_INFO, NVDM_CONTEXT_INFO_ITEM_INFO_1},\
    {NVDM_GROUP_CONTEXT_INFO, NVDM_CONTEXT_INFO_ITEM_INFO_2},\
    {NVDM_GROUP_HCI_SWITCH, NVDM_HCI_SWITCH_ITEM_SWITCH_ITEM},\
    {NVDM_GROUP_MMI_APP, NVDM_MULTI_ADV_LE_ADDR},\
    {NVDM_GROUP_PORT_SETTING, NVDM_PORT_SETTING_ITEM_PORT_ASSIGN},\
    {NVDM_GROUP_PORT_SETTING, NVDM_PORT_SETTING_ITEM_PORT_CONFIG},\
    {NVDM_INTERNAL_USE_GROUP, NVDM_USE_SETTING},\
};

#define FACTORY_RST_RESERVED_NVKEY_ITEM_LIST \
{\
    NVID_NVDM_VERSION,\
    NVID_PRODUCT_CATEGORY,\
    NVID_MODEL_NAME,\
    NVID_BT_HOST_AIR_SPP_UUID,\
    NVID_APP_ULL_PEER_BT_ADDRESS,\
    NVID_BT_LEA_CSIS_DATA,\
    NVID_BT_LEA_AICS_GAIN_SET_PROP,\
    NVID_BT_LEA_METADATA,\
    NVID_CAL_CHG_CFG,\
    NVID_CAL_CHG_ADC_CFG,\
    NVID_CAL_INT_CHG_DAC_CFG,\
    NVID_CAL_INT_CHG_TRI_CURR_CFG,\
    NVID_CAL_INT_CHG_CC1_CURR_CFG,\
    NVID_CAL_INT_CHG_CC2_CURR_CFG,\
    NVID_CAL_CV_STOP_CURR_CFG,\
    NVID_CAL_SYS_LDO,\
    NVID_CAL_OCP,\
    NVID_CAL_JEITA,\
    NVID_CAL_JEITA_WARM,\
    NVID_CAL_JEITA_COOL,\
    NVID_CAL_VCORE_DL,\
    NVID_CAL_VIO18_DL,\
    NVID_CAL_BUCK_MV_FREQ,\
    NVID_CAL_VAUD18_DL,\
    NVID_CAL_VRF_DL,\
    NVID_CAL_BUCK_LV_FREQ,\
    NVID_CAL_LDO_VDD33_REG_CFG,\
    NVID_CAL_LDO_VDD33_REG_RET_CFG,\
    NVID_CAL_LDO_VDD33_RET_CFG,\
    NVID_CAL_LDO_VRF_REG_CFG,\
    NVID_CAL_LDO_VRF_REG_RET_CFG,\
    NVID_CAL_LDO_VRF_RET_CFG,\
    NVID_CAL_VDIG18_CFG,\
    NVID_CAL_HPBG_CFG,\
    NVID_CAL_LPBG_CFG,\
    NVID_CAL_VBAT_ADC_CAL_TABLE,\
    NVID_CAL_VBAT_VOLT_CFG,\
    NVID_CAL_BUCK_RIPPLE_CTL,\
    NVID_CAL_BUCK_VOLT_TRIM,\
    NVID_CAL_BUCK_IPEAK,\
    NVID_CAL_BUCK_FREQ,\
    NVID_CAL_SIDO_VOLT,\
    NVID_CAL_SIDO_IPEAK,\
    NVID_CAL_SIDO_FREQ,\
    NVID_CAL_VCORE_VOLT,\
    NVID_CAL_VIO18_VOLT,\
    NVID_CAL_VRF_VOLT,\
    NVID_CAL_VAUD18_VOLT,\
    NVID_CAL_VPA_VOLT,\
    NVID_CAL_VPA_DL,\
    NVID_CAL_BUCK_MV_CFG,\
    NVID_CAL_BUCK_MV_STB_CFG,\
    NVID_CAL_BUCK_LV_CFG,\
    NVID_CAL_BUCK_LV_LPM,\
    NVID_CAL_LPO32_CFG,\
    NVID_CAL_PWR_CTL_MP_K,\
    NVID_CAL_XO_26M_CRTSTAL_TRIM,\
    NVID_CAL_LDO_VSRAM_CFG,\
    NVID_CAL_GENERAL_CFG,\
    NVID_CAL_CHG_CTL_CFG,\
    NVID_RF_BR_DPC_TABLE_K_V12,\
    NVID_RF_EDR_DPC_TABLE_K_V12,\
    NVID_RF_BR_DPC_TABLE_K_V14,\
    NVID_RF_EDR_DPC_TABLE_K_V14,\
    NVID_RF_BR_DPC_TABLE_K_V16,\
    NVID_RF_EDR_DPC_TABLE_K_V16,\
    NVID_RF_BR_DPC_TABLE_K_V18,\
    NVID_RF_EDR_DPC_TABLE_K_V18,\
    NVID_RF_BR_DPC_TABLE_K_V22,\
    NVID_RF_EDR_DPC_TABLE_K_V22,\
    NVID_RF_BR_TX_GAIN_TABLE_K_V12,\
    NVID_RF_EDR_TX_GAIN_TABLE_K_V12,\
    NVID_RF_BR_TX_GAIN_TABLE_K_V14,\
    NVID_RF_EDR_TX_GAIN_TABLE_K_V14,\
    NVID_RF_BR_TX_GAIN_TABLE_K_V16,\
    NVID_RF_EDR_TX_GAIN_TABLE_K_V16,\
    NVID_RF_BR_TX_GAIN_TABLE_K_V18,\
    NVID_RF_EDR_TX_GAIN_TABLE_K_V18,\
    NVID_RF_BR_TX_GAIN_TABLE_K_V22,\
    NVID_RF_EDR_TX_GAIN_TABLE_K_V22,\
    NVID_RF_VPA,\
    NVID_NTC_CFG1,\
    NVID_NTC_CFG2,\
    NVID_NTC_TEMP_TABLE,\
    NVID_NTC_RES_TABLE,\
    NVID_NTC_R2_TABLE,\
    NVID_NTC_RARIO_TABLE,\
    NVID_PMU_RST_PAT_CFG,\
    NVID_RF_MISC_PWR_CTL,\
    NVID_LM_INFO_LMP_FEATURE_PAGE0,\
    NVID_LM_INFO_LMP_FEATURE_PAGE1,\
    NVID_LM_INFO_LMP_FEATURE_PAGE2,\
    NVID_BT_BLE_ADV_NAME,\
    NVID_AIRO_KEY_NUMS,\
    NVID_AIRO_KEY_1,\
    NVID_AIRO_KEY_2,\
    NVID_AIRO_KEY_3,\
    NVID_AIRO_KEY_4,\
    NVID_AIRO_KEY_5,\
    NVID_AIRO_KEY_6,\
    NVID_AIRO_KEY_7,\
    NVID_AIRO_KEY_8,\
    NVID_AIRO_KEY_9,\
    NVID_AIRO_KEY_A,\
    NVID_AIRO_KEY_B,\
    NVID_AIRO_KEY_C,\
    NVID_AIRO_KEY_D,\
    NVID_AIRO_KEY_E,\
    NVID_AIRO_KEY_F,\
    NVID_CPT_CALI_4CHDATA_EXT,\
    NVID_CPT_CALI_4CHDATA,\
    NVID_CPT_SDWU_DATA,\
    NVID_CPT_FEATURE,\
    NVID_CPT_EARDETECT_DATA,\
    NVID_CPT_FORCETOUCH_DATA,\
    NVID_CPT_TUNE_DATA,\
    NVID_CPT_AUTOSUSPEND_DATA,\
    NVID_DSP_FW_DUMP_DATA,\
    NVID_DSP_ALG_AUD_LATENCY,\
    NVID_DSP_FW_VOL_COMP,\
    NVID_DSP_FW_VOW_PARA,\
    NVID_DSP_ALG_RESERVED_PARA,\
    NVID_DSP_ALG_RESERVED_PARA_1,\
    NVID_DSP_FW_VOL_COMP_02,\
    NVID_DSP_FW_AG_TB_COMMON,\
    NVID_DSP_FW_DG_TB_A2DP,\
    NVID_DSP_FW_DG_TB_LINE,\
    NVID_DSP_FW_DG_TB_SCO,\
    NVID_DSP_FW_DG_TB_VC,\
    NVID_DSP_FW_DG_TB_VP,\
    NVID_DSP_FW_DG_TB_RT,\
    NVID_DSP_FW_DG_TB_AT,\
    NVID_DSP_FW_DG_TB_SCONB,\
    NVID_DSP_FW_DG_TB_USB_AUDIO,\
    NVID_DSP_FW_DG_TB_MEDIAPLAYER,\
    NVID_DSP_FW_AG_TB_A2DP,\
    NVID_DSP_FW_AG_TB_LINE,\
    NVID_DSP_FW_AG_TB_SCO,\
    NVID_DSP_FW_AG_TB_VC,\
    NVID_DSP_FW_AG_TB_VP,\
    NVID_DSP_FW_AG_TB_RT,\
    NVID_DSP_FW_AG_TB_AT,\
    NVID_DSP_FW_AG_TB_SCONB,\
    NVID_DSP_FW_AG_TB_USB_AUDIO,\
    NVID_DSP_FW_AG_TB_MEDIAPLAYER,\
    NVID_DSP_FW_IDG_TB_A2DP,\
    NVID_DSP_FW_IDG_TB_LINE,\
    NVID_DSP_FW_IDG_TB_SCO,\
    NVID_DSP_FW_IDG_TB_VC,\
    NVID_DSP_FW_IDG_TB_VP,\
    NVID_DSP_FW_IDG_TB_RT,\
    NVID_DSP_FW_IDG_TB_AT,\
    NVID_DSP_FW_IDG_TB_DETACH_MIC,\
    NVID_DSP_FW_IAG_TB_A2DP,\
    NVID_DSP_FW_IAG_TB_LINE,\
    NVID_DSP_FW_IAG_TB_SCO,\
    NVID_DSP_FW_IAG_TB_VC,\
    NVID_DSP_FW_IAG_TB_VP,\
    NVID_DSP_FW_IAG_TB_RT,\
    NVID_DSP_FW_IAG_TB_AT,\
    NVID_DSP_FW_IAG_TB_SCONB,\
    NVID_DSP_FW_IAG_TB_DETACH_MIC,\
    NVID_DSP_FW_IDGP_TB_SCO,\
    NVID_DSP_FW_IDGP_TB_SCONB,\
    NVID_DSP_FW_IDGP_TB_AT,\
    NVID_DSP_FW_IDGP_TB_SYS_SW,\
    NVID_DSP_FW_IDGP_TB_SCO_MULMIC,\
    NVID_DSP_FW_IAGP_TB_SCO_L,\
    NVID_DSP_FW_IAGP_TB_SCO_R,\
    NVID_DSP_FW_IAGP_TB_SCONB_L,\
    NVID_DSP_FW_IAGP_TB_SCONB_R,\
    NVID_DSP_FW_IAGP_TB_AT_L,\
    NVID_DSP_FW_IAGP_TB_AT_R,\
    NVID_DSP_FW_IAGP_TB_SCO_ML_MIC,\
    NVID_DSP_FW_IDG_TB_WWE,\
    NVID_DSP_FW_IDG_TB_MIC_FUNC,\
    NVID_DSP_FW_IAGP_TB_SCO_MIC2,\
    NVID_DSP_FW_IAGP_TB_SCO_MIC3,\
    NVID_DSP_FW_IAGP_TB_SCO_MIC4,\
    NVID_DSP_FW_IAGP_TB_SCO_MIC5,\
    NVID_DSP_FW_IAGP_TB_SCONB_MIC2,\
    NVID_DSP_FW_IAGP_TB_SCONB_MIC3,\
    NVID_DSP_FW_IAGP_TB_SCONB_MIC4,\
    NVID_DSP_FW_IAGP_TB_SCONB_MIC5,\
    NVID_DSP_FW_VOL_CFG_SCO,\
    NVID_DSP_FW_VOL_CFG_A2DP,\
    NVID_DSP_FW_VOL_CFG_MP3,\
    NVID_DSP_FW_VOL_CFG_SCOMIC,\
    NVID_DSP_FW_VOL_CFG_VPRT,\
    NVID_DSP_FW_VOL_CFG_LINEIN,\
    NVID_DSP_FW_VOL_USB_AUD,\
    NVID_DSP_FW_VOL_MEDIAPLAYER,\
    NVID_DSP_FW_VOL_A2DPSRC,\
    NVID_DSP_FW_VOL_SW_USB_AUD,\
    NVID_DSP_FW_VOL_SW_USB_VOC,\
    NVID_DSP_ALG_CPD_WB_TX_VO,\
    NVID_DSP_ALG_CPD_NB_TX_VO,\
    NVID_DSP_ALG_CPD_WB_RX_VO,\
    NVID_DSP_ALG_CPD_NB_RX_VO,\
    NVID_DSP_ALG_CPD_VP,\
    NVID_DSP_ALG_CPD_A2DP_AU,\
    NVID_DSP_ALG_CPD_LINE_AU,\
    NVID_DSP_ALG_POSITIVE_GAIN,\
    NVID_DSP_ALG_INS,\
    NVID_DSP_ALG_POSITIVE_GAIN_2ND,\
    NVID_DSP_ALG_PLC,\
    NVID_DSP_ALG_AEC,\
    NVID_DSP_ALG_NR,\
    NVID_DSP_ALG_INEAR,\
    NVID_DSP_ALG_EQ_WB_RX,\
    NVID_DSP_ALG_EQ_WB_TX,\
    NVID_DSP_ALG_EQ_NB_RX,\
    NVID_DSP_ALG_EQ_NB_TX,\
    NVID_DSP_ALG_EQ_WB_RX_2ND,\
    NVID_DSP_ALG_EQ_NB_RX_2ND,\
    NVID_DSP_ALG_EQ_INEAR,\
    NVID_DSP_ALG_EQ_AST,\
    NVID_DSP_ALG_EQ_WB_TX_FIR,\
    NVID_DSP_ALG_EQ_NB_TX_FIR,\
    NVID_DSP_ALG_VAD_CMN_PARA       ,\
    NVID_DSP_ALG_VAD_1MIC_V  ,\
    NVID_DSP_ALG_VAD_1MIC_C  ,\
    NVID_DSP_ALG_VAD_2MIC_V  ,\
    NVID_DSP_ALG_VAD_2MIC_C  ,\
    NVID_DSP_ALG_ANC_LKGE_COMP,\
    NVID_DSP_ALG_AGC_WB_RX,\
    NVID_DSP_ALG_AGC_NB_RX,\
    NVID_DSP_ALG_AGC_TX,\
    NVID_DSP_ALG_AEC_BOOMIC,\
    NVID_DSP_ALG_NR_BOOMIC,\
    NVID_DSP_ALG_INEAR_BOOMIC,\
    NVID_DSP_ALG_AEC_NR_SWB_BOOMIC,\
    NVID_DSP_ALG_EQ_NB_TX_BOOMIC,\
    NVID_DSP_ALG_EQ_WB_TX_BOOMIC,\
    NVID_DSP_ALG_EQ_SWB_TX_BOOMIC,\
    NVID_DSP_ALG_CPD_NB_TX_VO_BOOMIC,\
    NVID_DSP_ALG_CPD_WB_TX_VO_BOOMIC,\
    NVID_DSP_ALG_CPD_SWB_TX_VO_BOOMIC,\
    NVID_DSP_ALG_EQ_NB_TX_FIR_BOOMIC,\
    NVID_DSP_ALG_EQ_WB_TX_FIR_BOOMIC,\
    NVID_DSP_ALG_EQ_SWB_TX_FIR_BOOMIC,\
    NVID_DSP_ALG_ANC_L_FILTER_1,\
    NVID_DSP_ALG_ANC_R_FILTER_1,\
    NVID_DSP_ALG_ANC_L_FILTER_2,\
    NVID_DSP_ALG_ANC_R_FILTER_2,\
    NVID_DSP_ALG_ANC_L_FILTER_3,\
    NVID_DSP_ALG_ANC_R_FILTER_3,\
    NVID_DSP_ALG_ANC_L_FILTER_4,\
    NVID_DSP_ALG_ANC_R_FILTER_4,\
    NVID_DSP_ALG_ANC_PATH_SETTING,\
    NVID_DSP_ALG_PT_L_FILTER_1,\
    NVID_DSP_ALG_PT_R_FILTER_1,\
    NVID_DSP_ALG_PT_L_FILTER_2,\
    NVID_DSP_ALG_PT_R_FILTER_2,\
    NVID_DSP_ALG_PT_L_FILTER_3,\
    NVID_DSP_ALG_PT_R_FILTER_3,\
    NVID_DSP_ALG_ANC_CUR_STATUS,\
    NVID_DSP_ALG_ANC_SETTING,\
    NVID_DSP_ALG_PT_SETTING,\
    NVID_DSP_ALG_ANC_PWR_DET,\
    NVID_DSP_ALG_PT_PWR_DET,\
    NVID_DSP_ALG_ANC_CAL_GAIN,\
    NVID_DSP_ALG_ANC_DEF_STATUS,\
    0xE1E7,\
    0xE1E8,\
    0xE1E9,\
    0xE1EA,\
    0xE1EB,\
    0xE1EC,\
    0xE1ED,\
    0xE1EE,\
    0xE1EF,\
    NVID_SYS_EXP_DUMP_MODE,\
    NVID_DSP_ALG_ANC_MULTI_FEATURE,\
    NVID_DSP_ALG_FADP_SHAP_FL_1,\
    NVID_DSP_ALG_FADP_SHAP_FL_2,\
    NVID_DSP_ALG_FADP_SHAP_FL_3,\
    NVID_DSP_ALG_FADP_SHAP_FL_4,\
    NVID_DSP_ALG_FADP_SZ,\
    NVID_DSP_ALG_FADP_SZ2,\
    NVID_DSP_ALG_FADP_DS_REAL_1,\
    NVID_DSP_ALG_FADP_DS_REAL_2,\
    NVID_DSP_ALG_FADP_DS_REAL_3,\
    NVID_DSP_ALG_FADP_DS_REAL_4,\
    NVID_DSP_ALG_FADP_DS_IMG_1,\
    NVID_DSP_ALG_FADP_DS_IMG_2,\
    NVID_DSP_ALG_FADP_DS_IMG_3,\
    NVID_DSP_ALG_FADP_DS_IMG_4,\
    NVID_DSP_ALG_FADP_DP_REAL_1,\
    NVID_DSP_ALG_FADP_DP_REAL_2,\
    NVID_DSP_ALG_FADP_DP_REAL_3,\
    NVID_DSP_ALG_FADP_DP_REAL_4,\
    NVID_DSP_ALG_FADP_DP_IMG_1,\
    NVID_DSP_ALG_FADP_DP_IMG_2,\
    NVID_DSP_ALG_FADP_DP_IMG_3,\
    NVID_DSP_ALG_FADP_DP_IMG_4,\
    NVID_DSP_ALG_FADP_PARA,\
    NVID_DSP_ALG_FADP_ADPFL_1,\
    NVID_DSP_ALG_FADP_ADPFL_2,\
    NVID_DSP_ALG_FADP_ADPFL_3,\
    NVID_PERI_LOG_SETTING,\
    NVID_APP_DEVICE_NAME_DEFAULT,\
    NVID_DSP_ALG_PEQ_MISC_PARA,\
    NVID_DSP_ALG_PEQ_PATH_TB,\
    NVID_DSP_ALG_PEQ_GP_TB_1,\
    NVID_DSP_ALG_PEQ_GP_TB_2,\
    NVID_DSP_ALG_PEQ_GP_TB_3,\
    NVID_DSP_ALG_PEQ_GP_TB_4,\
    NVID_DSP_ALG_PEQ_GP_TB_5,\
    NVID_DSP_ALG_PEQ_GP_TB_6,\
    NVID_DSP_ALG_AEQ_GP_TB,\
    NVID_DSP_ALG_AEQ_DETECT_MISC,\
    NVID_DSP_ALG_PEQ_COF_1,\
    NVID_DSP_ALG_PEQ_COF_2,\
    NVID_DSP_ALG_PEQ_COF_3,\
    NVID_DSP_ALG_PEQ_COF_4,\
    NVID_DSP_ALG_PEQ_COF_5,\
    NVID_DSP_ALG_PEQ_COF_6,\
    NVID_DSP_ALG_PEQ_COF_7,\
    NVID_DSP_ALG_PEQ_COF_8,\
    NVID_DSP_ALG_PEQ_COF_9,\
    NVID_DSP_ALG_PEQ_COF_10,\
    NVID_DSP_ALG_PEQ_COF_11,\
    NVID_DSP_ALG_PEQ_COF_12,\
    NVID_DSP_ALG_PEQ_COF_13,\
    NVID_DSP_ALG_PEQ_COF_14,\
    NVID_DSP_ALG_PEQ_COF_15,\
    NVID_DSP_ALG_PEQ_COF_16,\
    NVID_DSP_ALG_PEQ_COF_17,\
    NVID_DSP_ALG_PEQ_COF_18,\
    NVID_DSP_ALG_PEQ_COF_19,\
    NVID_DSP_ALG_PEQ_COF_20,\
    NVID_DSP_ALG_PEQ_COF_21,\
    NVID_DSP_ALG_PEQ_COF_22,\
    NVID_DSP_ALG_PEQ_COF_23,\
    NVID_DSP_ALG_PEQ_COF_24,\
    NVID_DSP_ALG_PEQ_COF_25,\
    NVID_DSP_ALG_PEQ_COF_26,\
    NVID_DSP_ALG_PEQ_COF_27,\
    NVID_DSP_ALG_PEQ_COF_28,\
    NVID_DSP_ALG_AEQ_COF_1,\
    NVID_DSP_ALG_AEQ_COF_2,\
    NVID_DSP_ALG_AEQ_COF_3,\
    NVID_DSP_ALG_AEQ_COF_4,\
    NVID_DSP_ALG_AEQ_COF_5,\
    NVID_DSP_ALG_AEQ_COF_6,\
    NVID_DSP_ALG_AEQ_COF_7,\
    NVID_DSP_ALG_AEQ_PARA,\
    NVID_DSP_ALG_AEQ_PARA_SZ_1,\
    NVID_DSP_ALG_AEQ_PARA_SZ_2,\
    NVID_DSP_ALG_AEQ_PARA_SZ_3,\
    NVID_DSP_ALG_AEQ_PARA_SZ_4,\
    NVID_DSP_ALG_AEQ_PARA_SZ_5,\
    NVID_DSP_ALG_AEQ_PARA_SZ_6,\
    NVID_DSP_ALG_AEQ_PARA_SZ_7,\
    NVID_APP_LED_PATTERN,\
    NVID_DSP_FW_AUDIO_DVFS_CLK_SET,\
    NVID_DSP_FW_AUDIO_HW_IO_CFG,\
    NVID_BT_HOST_DM_AWS_MCE_CONFIG,\
    NVID_DSP_FW_AUDIO_CH_CFG,\
    NVID_PERI_SMCHG_CFG,\
    NVID_APP_BT_MUSIC_AAC_ENABLE,\
    NVID_APP_BT_DEFAULT_TXPWR,\
    NVID_APP_A2DP_3M_CONFIG,\
    NVID_APP_FAST_PAIR_PROTECTED,\
    NVID_APP_FAST_PAIR_CONFIGURE,\
    NVID_APP_GSOUND_INFO,\
    NVID_DSP_ALG_ANC_WIND_DET,\
    NVID_DSP_ALG_ANC_USR_UNAWARE,\
    NVID_DSP_ALG_ANC_ENVIRONMENT_DETECTION,\
    NVID_DSP_ALG_ANC_ADAPTIVE_FF,\
    NVID_DSP_ALG_ANC_ADAPTIVE_FF_KEEP_FILTER,\
    NVID_DSP_ALG_ANC_ADAPTIVE_SETTING,\
    NVID_DSP_ALG_SIDETONE_FIR_COEF,\
};


/**
* @brief      This function is to check which reset flag is set and do NVDM re-organization.
*/
void factory_rst_reserved_nvdm_item_list_check(void);


#endif

#ifdef __cplusplus
}
#endif

#endif /* __NVDM_CONFIG_FACTRST_H__ */
