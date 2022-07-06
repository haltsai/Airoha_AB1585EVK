/* Copyright Statement:
 *
 * (C) 2022  Airoha Technology Corp. All rights reserved.
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
#include <string.h>
#include "config.h"
#include "types.h"
#include "dlist.h"
#include "dsp_temp.h"

#include "dsp_memory_region.h"

#include "preloader_pisplit.h"
#include "syslog.h"


#ifdef AIR_DSP_MEMORY_REGION_ENABLE

log_create_module(DSP_MEMORY, PRINT_LEVEL_DEBUG);

#define DSP_MEMORY_DEBUG_LOG
#if 1
#define DSP_MEMORY_LOG_E(_message, arg_cnt, ...)  LOG_MSGID_E(DSP_MEMORY,_message, arg_cnt, ##__VA_ARGS__)
#define DSP_MEMORY_LOG_W(_message, arg_cnt, ...)  LOG_MSGID_W(DSP_MEMORY,_message, arg_cnt, ##__VA_ARGS__)
#define DSP_MEMORY_LOG_I(_message, arg_cnt, ...)  LOG_MSGID_I(DSP_MEMORY,_message, arg_cnt, ##__VA_ARGS__)
#ifdef DSP_MEMORY_DEBUG_LOG
#define DSP_MEMORY_LOG_D(_message, arg_cnt, ...)  LOG_MSGID_D(DSP_MEMORY,_message, arg_cnt, ##__VA_ARGS__)
#else
#define DSP_MEMORY_LOG_D(_message, arg_cnt, ...)
#endif
#else
#define DSP_MEMORY_LOG_E(_message, arg_cnt, ...)  LOG_E(DSP_MEMORY,_message, ##__VA_ARGS__)
#define DSP_MEMORY_LOG_W(_message, arg_cnt, ...)  LOG_W(DSP_MEMORY,_message, ##__VA_ARGS__)
#define DSP_MEMORY_LOG_I(_message, arg_cnt, ...)  LOG_I(DSP_MEMORY,_message, ##__VA_ARGS__)
#define DSP_MEMORY_LOG_D(_message, arg_cnt, ...)  LOG_D(DSP_MEMORY,_message, ##__VA_ARGS__)
#endif


////////////////////////////////////////////////////////////////////////////////
// Constant Definitions ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////
// Global Variables ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


/**
 * @brief This table is used to configure subcomponent ID by feature type.
 *
 */
const dsp_memory_feature_subcomponent_table_t g_dsp_memory_feature_subcomponent_table[DSP_FEATURE_MAX_NUM] = {
    {DSP_MEMORY_NO_USE},                      /* 0x00    CODEC_PCM_COPY*/
    {DSP_MEMORY_NO_USE},                      /* 0x01    CODEC_DECODER_CVSD*/
    {SubComponent_HFP_MSBC_DEC},              /* 0x02    CODEC_DECODER_MSBC*/
    {SubComponent_A2DP_SBC},                  /* 0x03    CODEC_DECODER_SBC*/
    {SubComponent_A2DP_AAC},                  /* 0x04    CODEC_DECODER_AAC*/
    {DSP_MEMORY_NO_USE},                      /* 0x05    CODEC_DECODER_MP3*/
    {DSP_MEMORY_NO_USE},                      /* 0x06    CODEC_DECODER_EC*/
    {DSP_MEMORY_NO_USE},                      /* 0x07    CODEC_DECODER_UART*/
    {DSP_MEMORY_NO_USE},                      /* 0x08    CODEC_DECODER_UART16BIT*/
    {DSP_MEMORY_NO_USE},                      /* 0x09    CODEC_DECODER_CELT_HD*/
    {SubComponent_A2DP_VENDOR},               /* 0x0A    CODEC_DECODER_VENDOR*/
    {DSP_MEMORY_NO_USE},                      /* 0x0B    CODEC_DECODER_VENDOR_1*/
    {DSP_MEMORY_NO_USE},                      /* 0x0C    CODEC_DECODER_VENDOR_2*/
    {DSP_MEMORY_NO_USE},                      /* 0x0D    CODEC_DECODER_VENDOR_3*/
    {DSP_MEMORY_COMMON_SUBCOMPONENT_ID},      /* 0x0E    CODEC_DECODER_LC3*/
    {DSP_MEMORY_NO_USE},                      /* 0x0F    CODEC_DECODER_SAMPLE*/
    {DSP_MEMORY_NO_USE},                      /* 0x10    CODEC_ENCODER_CVSD*/
    {SubComponent_HFP_MSBC_ENC},              /* 0x11    CODEC_ENCODER_MSBC*/
    {DSP_MEMORY_NO_USE},                      /* 0x12    CODEC_ENCODER_OPUS*/
    {DSP_MEMORY_COMMON_SUBCOMPONENT_ID},      /* 0x13    CODEC_ENCODER_LC3*/
    {DSP_MEMORY_NO_USE},                      /* 0x14    CODEC_ENCODER_LC3_BRANCH*/
    {DSP_MEMORY_NO_USE},                      /* 0x15    */
    {DSP_MEMORY_NO_USE},                      /* 0x16    */
    {DSP_MEMORY_NO_USE},                      /* 0x17    */
    {DSP_MEMORY_NO_USE},                      /* 0x18    */
    {DSP_MEMORY_NO_USE},                      /* 0x19    */
    {DSP_MEMORY_NO_USE},                      /* 0x1A    */
    {DSP_MEMORY_NO_USE},                      /* 0x1B    */
    {DSP_MEMORY_NO_USE},                      /* 0x1C    */
    {DSP_MEMORY_NO_USE},                      /* 0x1D    */
    {DSP_MEMORY_NO_USE},                      /* 0x1E    */
    {DSP_MEMORY_COMMON_SUBCOMPONENT_ID},      /* 0x1F    DSP_SRC*/
    {DSP_MEMORY_NO_USE},                      /* 0x20    FUNC_END*/
    {DSP_MEMORY_COMMON_SUBCOMPONENT_ID},      /* 0x21    FUNC_RX_WB_DRC*/
    {DSP_MEMORY_NO_USE},                      /* 0x22    FUNC_RX_NB_DRC*/
    {DSP_MEMORY_COMMON_SUBCOMPONENT_ID},      /* 0x23    FUNC_TX_WB_DRC*/
    {DSP_MEMORY_NO_USE},                      /* 0x24    FUNC_TX_NB_DRC*/
    {DSP_MEMORY_COMMON_SUBCOMPONENT_ID},      /* 0x25    FUNC_RX_NR*/
    {DSP_MEMORY_COMMON_SUBCOMPONENT_ID},      /* 0x26    FUNC_TX_NR*/
    {DSP_MEMORY_COMMON_SUBCOMPONENT_ID},      /* 0x27    FUNC_CLK_SKEW_HFP_UL*/
    {SubComponent_A2DP_CLK_SKEW},             /* 0x28    FUNC_CLK_SKEW_A2DP_DL*/
    {DSP_MEMORY_COMMON_SUBCOMPONENT_ID},      /* 0x29    FUNC_MIC_SW_GAIN*/
    {DSP_MEMORY_COMMON_SUBCOMPONENT_ID},      /* 0x2A    FUNC_PLC*/
    {DSP_MEMORY_COMMON_SUBCOMPONENT_ID},      /* 0x2B    FUNC_CLK_SKEW_HFP_DL*/
    {DSP_MEMORY_NO_USE},                      /* 0x2C    FUNC_PROC_SIZE_CONV*/
    {DSP_MEMORY_NO_USE},                      /* 0x2D    FUNC_JOINT*/
    {DSP_MEMORY_NO_USE},                      /* 0x2E    FUNC_BRANCH*/
    {DSP_MEMORY_NO_USE},                      /* 0x2F    FUNC_MUTE*/
    {DSP_MEMORY_COMMON_SUBCOMPONENT_ID},      /* 0x30    FUNC_DRC*/
    {DSP_MEMORY_COMMON_SUBCOMPONENT_ID},      /* 0x31    FUNC_PEQ*/
    {DSP_MEMORY_NO_USE},                      /* 0x32    FUNC_LPF*/
    {DSP_MEMORY_NO_USE},                      /* 0x33    FUNC_CH_SEL*/
    {DSP_MEMORY_NO_USE},                      /* 0x34    FUNC_MUTE_SMOOTHER*/
    {DSP_MEMORY_COMMON_SUBCOMPONENT_ID},      /* 0x35    FUNC_PEQ2*/
    {DSP_MEMORY_COMMON_SUBCOMPONENT_ID},      /* 0x36    FUNC_DRC2*/
    {DSP_MEMORY_NO_USE},                      /* 0x37    FUNC_CH_SEL_HFP*/
    {DSP_MEMORY_NO_USE},                      /* 0x38    FUNC_LEAKAGE_COMPENSATION*/
    {DSP_MEMORY_NO_USE},                      /* 0x39    */
    {SubComponent_WWE_PREPROC},               /* 0x3A    FUNC_WWE_PREPROC*/
    {SubComponent_WWE_PROC},                  /* 0x3B    FUNC_WWE_PROC*/
    {DSP_MEMORY_NO_USE},                      /* 0x3C    FUNC_RX_WB_AGC*/
    {DSP_MEMORY_NO_USE},                      /* 0x3D    FUNC_RX_NB_AGC*/
    {DSP_MEMORY_NO_USE},                      /* 0x3E    FUNC_TX_AGC*/
    {DSP_MEMORY_NO_USE},                      /* 0x3F    FUNC_GSENSOR_MOTION_DETECT*/
    {SubComponent_A2DP_PLC},                  /* 0x40    FUNC_AUDIO_PLC*/
    {DSP_MEMORY_NO_USE},                      /* 0x41    FUNC_INS*/
    {DSP_MEMORY_NO_USE},                      /* 0x42    FUNC_PEQ_INSTANT*/
    {DSP_MEMORY_NO_USE},                      /* 0x43    FUNC_FIXED_SW_GAIN*/
    {SubComponent_MULTIMIC_FUNC_A},           /* 0x44    FUNC_FUNCTION_A*/
    {SubComponent_MULTIMIC_FUNC_B},           /* 0x45    FUNC_FUNCTION_B*/
    {SubComponent_MULTIMIC_FUNC_C},           /* 0x46    FUNC_FUNCTION_C*/
    {SubComponent_MULTIMIC_FUNC_D},           /* 0x47    FUNC_FUNCTION_D*/
    {SubComponent_MULTIMIC_FUNC_E},           /* 0x48    FUNC_FUNCTION_E*/
    {SubComponent_ESA_FUNC_F},                /* 0x49    FUNC_FUNCTION_F*/
    {DSP_MEMORY_NO_USE},                      /* 0x4A    FUNC_FUNCTION_G*/
    {DSP_MEMORY_NO_USE},                      /* 0x4B    FUNC_FUNCTION_H*/
    {DSP_MEMORY_COMMON_SUBCOMPONENT_ID},      /* 0x4C    FUNC_TX_EQ*/
    {DSP_MEMORY_NO_USE},                      /* 0x4D    FUNC_USER_TRIGGER_FF_SZ*/
    {DSP_MEMORY_NO_USE},                      /* 0x4E    FUNC_USER_TRIGGER_FF_PZ*/
    {DSP_MEMORY_NO_USE},                      /* 0x4F    FUNC_USER_TRIGGER_FF_PZ_FIR*/
    {DSP_MEMORY_NO_USE},                      /* 0x50    FUNC_AUDIO_LOOPBACK_TEST*/
    {DSP_MEMORY_COMMON_SUBCOMPONENT_ID},      /* 0x51    FUNC_DRC3*/
    {DSP_MEMORY_COMMON_SUBCOMPONENT_ID},      /* 0x52    FUNC_SW_SRC*/
    {DSP_MEMORY_COMMON_SUBCOMPONENT_ID},      /* 0x53    FUNC_SW_CLK_SKEW*/
    {DSP_MEMORY_NO_USE},                      /* 0x54    FUNC_SW_GAIN*/
    {DSP_MEMORY_NO_USE},                      /* 0x55    FUNC_SW_MIXER*/
    {DSP_MEMORY_NO_USE},                      /* 0x56    FUNC_SW_BUFFER*/
    {SubComponent_LEAUDIO_DL_SKEW},           /* 0x57    FUNC_CLK_SKEW_BLE_MUSIC_DL*/
    {DSP_MEMORY_NO_USE},                      /* 0x58    FUNC_WIND_DETECT*/
    {DSP_MEMORY_NO_USE},                      /* 0x59    FUNC_CH_SEL_VP*/
    {DSP_MEMORY_NO_USE},                      /* 0x5A    FUNC_PEQ3*/
    {DSP_MEMORY_NO_USE},                      /* 0x5B    FUNC_AFC*/
    {DSP_MEMORY_NO_USE},                      /* 0x5C    FUNC_LD_NR*/
    {DSP_MEMORY_NO_USE},                      /* 0x5D    FUNC_AT_AGC*/
    {DSP_MEMORY_NO_USE},                      /* 0x5E    FUNC_PEQ4*/
    {DSP_MEMORY_NO_USE},                      /* 0x5F    FUNC_DNN_NR*/
    {DSP_MEMORY_NO_USE},                      /* 0x60    FUNC_EC120*/
    {DSP_MEMORY_NO_USE},                      /* 0x61    FUNC_SRC_FIXED_RATIO*/
    {DSP_MEMORY_NO_USE},                      /* 0x62    FUNC_FIXED_SW_GAIN_TDM*/
    {DSP_MEMORY_NO_USE},                      /* 0x63    FUNC_FULL_ADAPT_ANC */
    {DSP_MEMORY_NO_USE},                      /* 0x64    FUNC_ADAPTIVE_EQ*/
    {DSP_MEMORY_NO_USE},                      /* 0x65    FUNC_ADAPTIVE_EQ_DRC */
    {DSP_MEMORY_COMMON_SUBCOMPONENT_ID},      /* 0x66    FUNC_RX_SWB_DRC*/
    {DSP_MEMORY_COMMON_SUBCOMPONENT_ID},      /* 0x67    FUNC_TX_SWB_DRC */
    {DSP_MEMORY_NO_USE},                      /* 0x68    FUNC_RX_SWB_AGC*/
    {DSP_MEMORY_NO_USE},                      /* 0x69    FUNC_USER_UNAWARE*/
    {DSP_MEMORY_NO_USE},                      /* 0x6A    FUNC_ENVIRONMENT_DETECTION */
    {DSP_MEMORY_NO_USE},                      /* 0x6B    FUNC_ADAPTIVE_EQ_DETECT*/
};



/**
 * @brief This table is used to configure subcomponent_type by component for common feature type.
 *
 * @param feature_type is the feature type for comparison
 * @param component_type is the component ID for comparison
 * @param subcomponent_type is dedicated sub-component ID for the component_type
 * @param separated_instance_type is dedicated sub-component ID for separating working buffer from PIC
 *
 */
const dsp_memory_common_feature_subcomponent_map_t g_dsp_memory_feature_common_feature_subcomponent_table[] = {
    //feature_type                  //component_type                             //subcomponent_type                          //separated_instance_type
    {CODEC_DECODER_LC3,   Component_LE_AUDIO,         SubComponent_LEAUDIO_LC3_DEC,                    DSP_MEMORY_NO_USE},
    {CODEC_DECODER_LC3,    Component_LE_CALL,          SubComponent_LECALL_LC3_DEC,                    DSP_MEMORY_NO_USE},
    {CODEC_ENCODER_LC3,    Component_LE_CALL,          SubComponent_LECALL_LC3_DEC,                    DSP_MEMORY_NO_USE},
    {FUNC_RX_NR,               Component_HFP,            SubComponent_HFP_ECNR_LIB,             SubComponent_HFP_ECNR_DL},
    {FUNC_RX_NR,           Component_LE_CALL,            SubComponent_LECALL_NR_LIB,           SubComponent_LECALL_RX_NR},

    {FUNC_PLC,                 Component_HFP,                 SubComponent_HFP_PLC,                    DSP_MEMORY_NO_USE},
    {FUNC_RX_WB_DRC,           Component_HFP,       SubComponent_HFP_VOICE_DRC_LIB,        SubComponent_HFP_VOICE_DRC_DL},
    {FUNC_CLK_SKEW_HFP_DL,     Component_HFP,        SubComponent_HFP_CLK_SKEW_LIB,          SubComponent_HFP_CLKSKEW_DL},
    {FUNC_CLK_SKEW_HFP_DL, Component_LE_CALL,          SubComponent_LECALL_SKEW_LIB,          SubComponent_LECALL_DL_SKEW},

    {FUNC_TX_NR,               Component_HFP,            SubComponent_HFP_ECNR_LIB,                    DSP_MEMORY_NO_USE},
    {FUNC_TX_NR,           Component_LE_CALL,            SubComponent_LECALL_NR_LIB,           SubComponent_LECALL_TX_NR},

    {FUNC_TX_EQ,               Component_HFP,               SubComponent_HFP_TX_EQ,                    DSP_MEMORY_NO_USE},
    {FUNC_TX_WB_DRC,           Component_HFP,       SubComponent_HFP_VOICE_DRC_LIB,        SubComponent_HFP_VOICE_DRC_UL},

    {FUNC_RX_SWB_DRC,      Component_LE_CALL,       SubComponent_LECALL_SWB_DRC_LIB,        SubComponent_LECALL_RX_SWB_DRC},
    {FUNC_TX_SWB_DRC,      Component_LE_CALL,       SubComponent_LECALL_SWB_DRC_LIB,        SubComponent_LECALL_TX_SWB_DRC},

    {FUNC_CLK_SKEW_HFP_UL,     Component_HFP,        SubComponent_HFP_CLK_SKEW_LIB,          SubComponent_HFP_CLKSKEW_UL},
    {FUNC_CLK_SKEW_HFP_UL, Component_LE_CALL,          SubComponent_LECALL_SKEW_LIB,         SubComponent_LECALL_UL_SKEW},

    {FUNC_MIC_SW_GAIN,         Component_HFP,         SubComponent_HFP_MIC_SW_GAIN,                    DSP_MEMORY_NO_USE},
    {FUNC_MIC_SW_GAIN,   Component_MULTI_MIC,           SubComponent_MULTIMIC_GAIN,                    DSP_MEMORY_NO_USE},
    {FUNC_MIC_SW_GAIN,     Component_LE_CALL,      SubComponent_LECALL_MIC_SW_GAIN,                    DSP_MEMORY_NO_USE},

    {FUNC_PEQ,                Component_A2DP,                SubComponent_A2DP_PEQ,                    DSP_MEMORY_NO_USE},

    {FUNC_DRC,                Component_A2DP,                SubComponent_A2DP_DRC,                    DSP_MEMORY_NO_USE},

    {FUNC_PEQ2,               Component_A2DP,               SubComponent_A2DP_PEQ2,                    DSP_MEMORY_NO_USE},

    {FUNC_MIC_SW_GAIN,      Component_LINEIN,      SubComponent_LINEIN_MIC_SW_GAIN,             DSP_MEMORY_NO_USE},

    {FUNC_MIC_SW_GAIN,         Component_WWE,         SubComponent_WWE_MIC_SW_GAIN,                    DSP_MEMORY_NO_USE},

    {FUNC_MIC_SW_GAIN,      Component_RECORD,      SubComponent_RECORD_MIC_SW_GAIN,                    DSP_MEMORY_NO_USE},

    /* ============================================    For vender   ============================================*/


    /* ====================================================================================================*/

};


/**
 * @brief This table is used to configure mapping ID by Scenario ID.
 *
 * @param component_type is memory management component ID
 * @param fw_subcomponent_type is stream_feature_table_t DRAM ID
 * @param stream_in_subcomponent_type is stream input buffer DRAM ID,
 * @param stream_out_subcomponent_type is stream output buffer DRAM ID
 *
 */
const dsp_memory_middleware_id_t g_dsp_memory_middleware_table[AUDIO_SCEANRIO_TYPE_MAX] = {
          //component_type,                       //fw_subcomponent_type,               //stream_in_subcomponent_type,               //stream_out_subcomponent_type,                                                                         //scenario_type,
/*  0  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /* AUDIO_SCENARIO_TYPE_COMMON */
/*  1  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                    AUDIO_SCENARIO_TYPE_AMP},    /* AUDIO_SCENARIO_TYPE_AMP*/
/*  2  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_VP_PRE},    /* AUDIO_SCENARIO_TYPE_VP_PRE */
/*  3  */   {Component_VP,                    SubComponent_VP_FW,            SubComponent_VP_STREAM_IN,                    DSP_MEMORY_NO_USE,                                     AUDIO_SCENARIO_TYPE_VP},    /* AUDIO_SCENARIO_TYPE_VP */
/*  4  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                           AUDIO_SCENARIO_TYPE_VP_DUMMY_PRE},    /* AUDIO_SCENARIO_TYPE_VP_DUMMY_PRE */
/*  5  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                               AUDIO_SCENARIO_TYPE_VP_DUMMY},    /* AUDIO_SCENARIO_TYPE_VP_DUMMY */
/*  6  */   {Component_HFP,               SubComponent_HFP_TX_FW,        SubComponent_HFP_TX_STREAM_IN,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_HFP_UL},    /* AUDIO_SCENARIO_TYPE_HFP_UL */
/*  7  */   {Component_HFP,               SubComponent_HFP_RX_FW,        SubComponent_HFP_RX_STREAM_IN,       SubComponent_HFP_RX_STREAM_OUT,                                 AUDIO_SCENARIO_TYPE_HFP_DL},    /* AUDIO_SCENARIO_TYPE_HFP_DL */
/*  8  */   {Component_RECORD,            SubComponent_RECORD_FW,        SubComponent_RECORD_STREAM_IN,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_RECORD},    /* AUDIO_SCENARIO_TYPE_RECORD */
/*  9  */   {Component_WWE,                  SubComponent_WWE_FW,           SubComponent_WWE_STREAM_IN,                    DSP_MEMORY_NO_USE,                                    AUDIO_SCENARIO_TYPE_WWE},    /* AUDIO_SCENARIO_TYPE_WWE */
/* 10  */   {Component_A2DP,                SubComponent_A2DP_FW,          SubComponent_A2DP_STREAM_IN,         SubComponent_A2DP_STREAM_OUT,                                   AUDIO_SCENARIO_TYPE_A2DP},    /* AUDIO_SCENARIO_TYPE_A2DP */
/* 11  */   {Component_LE_CALL,        SubComponent_LECALL_UL_FW,     SubComponent_LECALL_UL_STREAM_IN,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_BLE_UL},    /* AUDIO_SCENARIO_TYPE_BLE_UL */
/* 12  */   {Component_LE_CALL,        SubComponent_LECALL_DL_FW,     SubComponent_LECALL_DL_STREAM_IN,    SubComponent_LECALL_DL_STREAM_OUT,                                 AUDIO_SCENARIO_TYPE_BLE_DL},    /* AUDIO_SCENARIO_TYPE_BLE_DL  */
/* 13  */   {Component_LE_AUDIO,      SubComponent_LEAUDIO_DL_FW,    SubComponent_LEAUDIO_DL_STREAM_IN,   SubComponent_LEAUDIO_DL_STREAM_OUT,                           AUDIO_SCENARIO_TYPE_BLE_MUSIC_DL},    /* AUDIO_SCENARIO_TYPE_BLE_MUSIC_DL*/
/* 14  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                    AUDIO_SCENARIO_TYPE_ANC},    /* AUDIO_SCENARIO_TYPE_ANC */
/* 15  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                               AUDIO_SCENARIO_TYPE_SIDETONE},    /* AUDIO_SCENARIO_TYPE_SIDETONE */
/* 16  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                    AUDIO_SCENARIO_TYPE_VOW},    /* AUDIO_SCENARIO_TYPE_VOW */
/* 17  */   {Component_LINEIN,            SubComponent_LINEIN_FW,        SubComponent_LINEIN_STREAM_IN,                    DSP_MEMORY_NO_USE,                                AUDIO_SCENARIO_TYPE_LINE_IN},    /* AUDIO_SCENARIO_TYPE_LINE_IN */
/* 18  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                               AUDIO_SCENARIO_TYPE_LINE_OUT},    /* AUDIO_SCENARIO_TYPE_LINE_OUT */
/* 19  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                  AUDIO_SCENARIO_TYPE_SPDIF},    /* AUDIO_SCENARIO_TYPE_SPDIF */
/* 20  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                   AUDIO_SCENARIO_TYPE_MCLK},    /* AUDIO_SCENARIO_TYPE_MCLK */
/* 21  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                     AUDIO_SCENARIO_TYPE_USB_AUDIO_PLAYBACK},    /* AUDIO_SCENARIO_TYPE_USB_AUDIO_PLAYBACK */
/* 22  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                               AUDIO_SCENARIO_TYPE_PLAYBACK},    /* AUDIO_SCENARIO_TYPE_PLAYBACK */
/* 22  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                        AUDIO_SCENARIO_TYPE_DC_COMPENSATION},    /* AUDIO_SCENARIO_TYPE_DC_COMPENSATION */
/* 24  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 25  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 26  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 27  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 28  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 29  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 30  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                     AUDIO_SCENARIO_TYPE_GSENSOR_FUNCTION_D},    /* AUDIO_SCENARIO_TYPE_GSENSOR_FUNCTION_D */
/* 31  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                     AUDIO_SCENARIO_TYPE_GSENSOR_FUNCTION_F},    /* AUDIO_SCENARIO_TYPE_GSENSOR_FUNCTION_F */
/* 32  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                     AUDIO_SCENARIO_TYPE_GSENSOR_FUNCTION_G},    /* AUDIO_SCENARIO_TYPE_GSENSOR_FUNCTION_G */
/* 33  */   {Component_MULTI_MIC,      SubComponent_MULTIMIC_FW1,  SubComponent_MULTIMIC_FW1_STREAM_IN,                    DSP_MEMORY_NO_USE,            AUDIO_SCENARIO_TYPE_MULTI_MIC_STREAM_FUNCTION_A},    /* AUDIO_SCENARIO_TYPE_MULTI_MIC_STREAM_FUNCTION_A */
/* 34  */   {Component_ESA,                  SubComponent_ESA_FW,           SubComponent_ESA_STREAM_IN,                    DSP_MEMORY_NO_USE,            AUDIO_SCENARIO_TYPE_MULTI_MIC_STREAM_FUNCTION_B},    /* AUDIO_SCENARIO_TYPE_MULTI_MIC_STREAM_FUNCTION_B */
/* 35  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,            AUDIO_SCENARIO_TYPE_MULTI_MIC_STREAM_FUNCTION_C},    /* AUDIO_SCENARIO_TYPE_MULTI_MIC_STREAM_FUNCTION_C */
/* 36  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,            AUDIO_SCENARIO_TYPE_MULTI_MIC_STREAM_FUNCTION_F},    /* AUDIO_SCENARIO_TYPE_MULTI_MIC_STREAM_FUNCTION_F */
/* 37  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,              AUDIO_SCENARIO_TYPE_GAMING_MODE_VOICE_HEADSET},    /* AUDIO_SCENARIO_TYPE_GAMING_MODE_VOICE_HEADSET */
/* 38  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,       AUDIO_SCENARIO_TYPE_GAMING_MODE_VOICE_DONGLE_USB_OUT},    /* AUDIO_SCENARIO_TYPE_GAMING_MODE_VOICE_DONGLE_USB_OUT */
/* 39  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,      AUDIO_SCENARIO_TYPE_GAMING_MODE_MUSIC_DONGLE_USB_IN_0},    /* AUDIO_SCENARIO_TYPE_GAMING_MODE_MUSIC_DONGLE_USB_IN_0 */
/* 40  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,      AUDIO_SCENARIO_TYPE_GAMING_MODE_MUSIC_DONGLE_USB_IN_1},    /* AUDIO_SCENARIO_TYPE_GAMING_MODE_MUSIC_DONGLE_USB_IN_1 */
/* 41  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,      AUDIO_SCENARIO_TYPE_GAMING_MODE_VOICE_DONGLE_LINE_OUT},    /* AUDIO_SCENARIO_TYPE_GAMING_MODE_VOICE_DONGLE_LINE_OUT */
/* 42  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,       AUDIO_SCENARIO_TYPE_GAMING_MODE_MUSIC_DONGLE_LINE_IN},    /* AUDIO_SCENARIO_TYPE_GAMING_MODE_MUSIC_DONGLE_LINE_IN */
/* 43  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                     AUDIO_SCENARIO_TYPE_ANC_MONITOR_STREAM},    /* AUDIO_SCENARIO_TYPE_ANC_MONITOR_STREAM */
/* 44  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    AUDIO_SCENARIO_TYPE_TEST_AUDIO_LOOPBACK},    /* AUDIO_SCENARIO_TYPE_TEST_AUDIO_LOOPBACK */
/* 45  */   {Component_TDM,                  SubComponent_TDM_FW,           SubComponent_TDM_STREAM_IN,          SubComponent_TDM_STREAM_OUT,                                    AUDIO_SCENARIO_TYPE_TDM},    /* AUDIO_SCENARIO_TYPE_TDM */
/* 46  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    AUDIO_SCENARIO_TYPE_WIRED_AUDIO_USB_OUT},    /* AUDIO_SCENARIO_TYPE_WIRED_AUDIO_USB_OUT */
/* 47  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                   AUDIO_SCENARIO_TYPE_WIRED_AUDIO_USB_IN_0},    /* AUDIO_SCENARIO_TYPE_WIRED_AUDIO_USB_IN_0 */
/* 48  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                   AUDIO_SCENARIO_TYPE_WIRED_AUDIO_USB_IN_1},    /* AUDIO_SCENARIO_TYPE_WIRED_AUDIO_USB_IN_1 */
/* 49  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                   AUDIO_SCENARIO_TYPE_WIRED_AUDIO_LINE_OUT},    /* AUDIO_SCENARIO_TYPE_WIRED_AUDIO_LINE_OUT */
/* 50  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    AUDIO_SCENARIO_TYPE_WIRED_AUDIO_LINE_IN},    /* AUDIO_SCENARIO_TYPE_WIRED_AUDIO_LINE_IN */
/* 51  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,   AUDIO_SCENARIO_TYPE_WIRED_AUDIO_DUAL_CHIP_LINE_IN_MASTER},    /* AUDIO_SCENARIO_TYPE_WIRED_AUDIO_DUAL_CHIP_LINE_IN_MASTER */
/* 52  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,    AUDIO_SCENARIO_TYPE_WIRED_AUDIO_DUAL_CHIP_LINE_IN_SLAVE},    /* AUDIO_SCENARIO_TYPE_WIRED_AUDIO_DUAL_CHIP_LINE_IN_SLAVE */
/* 53  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,  AUDIO_SCENARIO_TYPE_WIRED_AUDIO_DUAL_CHIP_LINE_OUT_MASTER},    /* AUDIO_SCENARIO_TYPE_WIRED_AUDIO_DUAL_CHIP_LINE_OUT_MASTER */
/* 54  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,       AUDIO_SCENARIO_TYPE_ADVANCED_PASSTHROUGH_HEARING_AID},    /* AUDIO_SCENARIO_TYPE_ADVANCED_PASSTHROUGH_HEARING_AID */
/* 55  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,         AUDIO_SCENARIO_TYPE_BLE_AUDIO_DONGLE_VOICE_USB_OUT},    /* AUDIO_SCENARIO_TYPE_BLE_AUDIO_DONGLE_VOICE_USB_OUT */
/* 56  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,        AUDIO_SCENARIO_TYPE_BLE_AUDIO_DONGLE_MUSIC_USB_IN_0},    /* AUDIO_SCENARIO_TYPE_BLE_AUDIO_DONGLE_MUSIC_USB_IN_0 */
/* 57  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,        AUDIO_SCENARIO_TYPE_BLE_AUDIO_DONGLE_MUSIC_USB_IN_1},    /* AUDIO_SCENARIO_TYPE_BLE_AUDIO_DONGLE_MUSIC_USB_IN_1 */
/* 58  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,        AUDIO_SCENARIO_TYPE_BLE_AUDIO_DONGLE_VOICE_LINE_OUT},    /* AUDIO_SCENARIO_TYPE_BLE_AUDIO_DONGLE_VOICE_LINE_OUT */
/* 59  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,         AUDIO_SCENARIO_TYPE_BLE_AUDIO_DONGLE_MUSIC_LINE_IN},    /* AUDIO_SCENARIO_TYPE_BLE_AUDIO_DONGLE_MUSIC_LINE_IN */
/* 60  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,          AUDIO_SCENARIO_TYPE_AUDIO_HW_LOOPBACK_I2S0_TO_DAC},    /* AUDIO_SCENARIO_TYPE_AUDIO_HW_LOOPBACK_I2S0_TO_DAC */
/* 61  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,          AUDIO_SCENARIO_TYPE_AUDIO_HW_LOOPBACK_ADC_TO_I2S0},    /* AUDIO_SCENARIO_TYPE_AUDIO_HW_LOOPBACK_ADC_TO_I2S0 */
/* 62  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,       AUDIO_SCENARIO_TYPE_AUDIO_HW_LOOPBACK_LINEIN_TO_I2S2},    /* AUDIO_SCENARIO_TYPE_AUDIO_HW_LOOPBACK_LINEIN_TO_I2S2 */
/* 63  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,             AUDIO_SCENARIO_TYPE_ADAPTIVE_EQ_MONITOR_STREAM},    /* AUDIO_SCENARIO_TYPE_ADAPTIVE_EQ_MONITOR_STREAM*/
/* 64  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 65  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 66  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 67  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 68  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 69  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 70  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 71  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 72  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 73  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 74  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 75  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 76  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 77  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 78  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 79  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 80  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 81  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 82  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 83  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 84  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 85  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 86  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 87  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 88  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 89  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 90  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 91  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 92  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 93  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 94  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 95  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 96  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 97  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 98  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/* 99  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/*100  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/*101  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/*102  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/*103  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/*104  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/*105  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/*106  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/*107  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/*108  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/*109  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/*110  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/*111  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/*112  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/*113  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/*114  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/*115  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/*116  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/*117  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/*118  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/*119  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/*120  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/*121  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/*122  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/*123  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/*124  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/*125  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
/*126  */   {DSP_COMPONENT_NO_USE,             DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                    DSP_MEMORY_NO_USE,                                 AUDIO_SCENARIO_TYPE_COMMON},    /*  */
};

#define DSP_MEMORY_COMMON_FEATURE_NUMBER sizeof(g_dsp_memory_feature_common_feature_subcomponent_table)/sizeof(dsp_memory_common_feature_subcomponent_map_t)

dsp_memory_feature_subcomponent_table_info_t g_dsp_memory_feature_subcomponent_table_memory_ifo[DSP_FEATURE_MAX_NUM];
dsp_memory_common_feature_subcomponent_map_info_t g_dsp_memory_feature_common_feature_subcomponent_table_memory_ifo[DSP_MEMORY_COMMON_FEATURE_NUMBER];
dsp_memory_middleware_id_info_t g_dsp_memory_middleware_table_memory_ifo[AUDIO_SCENARIO_TYPE_END];


////////////////////////////////////////////////////////////////////////////////
// DSPMEM FUNCTION DECLARATIONS ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


DspMemoryManagerReturnStatus_t dsp_memory_init()
{

    return DspMemoryManagementInit();
}


TotalComponentType_t dsp_memory_get_component_by_scenario (audio_scenario_type_t scenario_type)
{
    DSP_MEMORY_LOG_D("DSP MEMORY dsp_memory_get_component_by_scenario scenario:%d, com:%d", 2, scenario_type, g_dsp_memory_middleware_table[scenario_type].component_type);
    if (scenario_type < AUDIO_SCENARIO_TYPE_END) {
        if (scenario_type != g_dsp_memory_middleware_table[scenario_type].scenario_type) {
            DSP_MEMORY_LOG_E("DSP MEMORY dsp_memory_get_component_by_scenario SCENARIO MISMATCH %d != %d ", 2, scenario_type, g_dsp_memory_middleware_table[scenario_type].scenario_type);
            assert(0);
        }
        return g_dsp_memory_middleware_table[scenario_type].component_type;
    }
    return DSP_COMPONENT_NO_USE;
}

SubComponentType_t dsp_memory_get_fw_subcomponent_by_scenario (audio_scenario_type_t scenario_type)
{
    DSP_MEMORY_LOG_D("DSP MEMORY dsp_memory_get_fw_subcomponent_by_scenario scenario:%d, com:%d, in:%d", 2, scenario_type, g_dsp_memory_middleware_table[scenario_type].fw_dram_id, g_dsp_memory_middleware_table[scenario_type].stream_in_dram_id);
    if (scenario_type < AUDIO_SCENARIO_TYPE_END) {
        if (scenario_type != g_dsp_memory_middleware_table[scenario_type].scenario_type) {
            DSP_MEMORY_LOG_E("DSP MEMORY dsp_memory_get_fw_subcomponent_by_scenario SCENARIO MISMATCH %d != %d ", 2, scenario_type, g_dsp_memory_middleware_table[scenario_type].scenario_type);
            assert(0);
        }
        return g_dsp_memory_middleware_table[scenario_type].fw_subcomponent_type;
    }
    return DSP_MEMORY_NO_USE;
}

SubComponentType_t dsp_memory_get_stream_in_subcomponent_by_scenario (audio_scenario_type_t scenario_type)
{
    DSP_MEMORY_LOG_D("DSP MEMORY dsp_memory_get_stream_in_subcomponent_by_scenario scenario:%d, com:%d", 2, scenario_type, g_dsp_memory_middleware_table[scenario_type].stream_in_dram_id);
    if (scenario_type < AUDIO_SCENARIO_TYPE_END) {
        if (scenario_type != g_dsp_memory_middleware_table[scenario_type].scenario_type) {
            DSP_MEMORY_LOG_E("DSP MEMORY dsp_memory_get_stream_in_subcomponent_by_scenario SCENARIO MISMATCH %d != %d ", 2, scenario_type, g_dsp_memory_middleware_table[scenario_type].scenario_type);
            assert(0);
        }
        return g_dsp_memory_middleware_table[scenario_type].stream_in_subcomponent_type;
    }
    return DSP_MEMORY_NO_USE;
}

SubComponentType_t dsp_memory_get_stream_out_subcomponent_by_scenario (audio_scenario_type_t scenario_type)
{
    DSP_MEMORY_LOG_D("DSP MEMORY dsp_memory_get_stream_out_subcomponent_by_scenario scenario:%d, com:%d", 2, scenario_type, g_dsp_memory_middleware_table[scenario_type].stream_out_dram_id);
    if (scenario_type < AUDIO_SCENARIO_TYPE_END) {
        if (scenario_type != g_dsp_memory_middleware_table[scenario_type].scenario_type) {
            DSP_MEMORY_LOG_E("DSP MEMORY dsp_memory_get_stream_out_subcomponent_by_scenario SCENARIO MISMATCH %d != %d ", 2, scenario_type, g_dsp_memory_middleware_table[scenario_type].scenario_type);
            assert(0);
        }
        return g_dsp_memory_middleware_table[scenario_type].stream_out_subcomponent_type;
    }
    return DSP_MEMORY_NO_USE;
}

dsp_memory_feature_subcomponent_info_t dsp_memory_get_memory_id_by_feature_type (audio_scenario_type_t scenario_type, stream_feature_type_t feature_type, dsp_memory_feature_subcomponent_info_ptr separated_instance_ram)
{

    dsp_memory_feature_subcomponent_info_t feature_subcomponent_info;
    feature_subcomponent_info.subcomponent_type = DSP_MEMORY_NO_USE;

    separated_instance_ram->subcomponent_type = DSP_MEMORY_NO_USE;
    if (feature_type < DSP_FEATURE_MAX_NUM) {
        feature_subcomponent_info.subcomponent_type = g_dsp_memory_feature_subcomponent_table[feature_type].subcomponent_type;
        feature_subcomponent_info.subcomponent_memory_info_ptr = &g_dsp_memory_feature_subcomponent_table_memory_ifo[feature_type].subcomponent_memory_info;


        //Check common type by scenario
        if (feature_subcomponent_info.subcomponent_type == DSP_MEMORY_COMMON_SUBCOMPONENT_ID) {
            TotalComponentType_t    component_type = dsp_memory_get_component_by_scenario(scenario_type);
            uint32_t common_number = sizeof(g_dsp_memory_feature_common_feature_subcomponent_table)/sizeof(dsp_memory_common_feature_subcomponent_map_t);
            uint32_t i;
            for (i=0 ; i<common_number ; i++) {
                if ((component_type == g_dsp_memory_feature_common_feature_subcomponent_table[i].component_type) &&
                    (feature_type == g_dsp_memory_feature_common_feature_subcomponent_table[i].feature_type)) {
                    feature_subcomponent_info.subcomponent_type = g_dsp_memory_feature_common_feature_subcomponent_table[i].subcomponent_type;
                    feature_subcomponent_info.subcomponent_memory_info_ptr = &g_dsp_memory_feature_common_feature_subcomponent_table_memory_ifo[i].subcomponent_memory_info;

                    separated_instance_ram->subcomponent_type  = g_dsp_memory_feature_common_feature_subcomponent_table[i].separated_instance_type;
                    separated_instance_ram->subcomponent_memory_info_ptr = &g_dsp_memory_feature_common_feature_subcomponent_table_memory_ifo[i].instance_memory_info;
                    break;
                }
            }
            if ((feature_subcomponent_info.subcomponent_type == DSP_MEMORY_COMMON_SUBCOMPONENT_ID) || (feature_subcomponent_info.subcomponent_type == DSP_MEMORY_NO_USE)) {
                DSP_MEMORY_LOG_E("DSP MEMORY Get feature type error, need to apply for common feature :%d. scenario_type : %d", 2, feature_type, scenario_type);
                assert(0);
            }
        }

    }
    return feature_subcomponent_info;
}


GroupMemoryInfo_t *dsp_memory_get_fw_memory_info_by_scenario (audio_scenario_type_t scenario_type)
{
    if (scenario_type < AUDIO_SCENARIO_TYPE_END) {
        return &g_dsp_memory_middleware_table_memory_ifo[scenario_type].fw_memory_info;
    }
    return NULL;
}
GroupMemoryInfo_t *dsp_memory_get_stream_in_memory_info_by_scenario (audio_scenario_type_t scenario_type)
{
    if (scenario_type < AUDIO_SCENARIO_TYPE_END) {
        return &g_dsp_memory_middleware_table_memory_ifo[scenario_type].stream_in_memory_info;
    }
    return NULL;
}
GroupMemoryInfo_t *dsp_memory_get_stream_out_memory_info_by_scenario (audio_scenario_type_t scenario_type)
{
    if (scenario_type < AUDIO_SCENARIO_TYPE_END) {
        return &g_dsp_memory_middleware_table_memory_ifo[scenario_type].stream_out_memory_info;
    }
    return NULL;
}


dsp_memory_status_t dsp_memory_get_status_by_scenario (audio_scenario_type_t scenario_type)
{
    if (scenario_type < AUDIO_SCENARIO_TYPE_END) {
        return g_dsp_memory_middleware_table_memory_ifo[scenario_type].status;
    }
    return DSP_MEMORY_STATUS_UNSUPPORTED_TYPE;
}


dsp_memory_status_t dsp_memory_set_status_by_scenario (audio_scenario_type_t scenario_type, dsp_memory_status_t set_status)
{
    uint32_t int_mask;
    if (scenario_type < AUDIO_SCENARIO_TYPE_END) {
        hal_nvic_save_and_set_interrupt_mask(&int_mask);
        g_dsp_memory_middleware_table_memory_ifo[scenario_type].status = set_status;
        hal_nvic_restore_interrupt_mask(int_mask);
        DSP_MEMORY_LOG_I("DSP MEMORY Set Status Change, Scenario_type:%d, Status:%d", 2, scenario_type, set_status);
        return g_dsp_memory_middleware_table_memory_ifo[scenario_type].status;
    }
    return DSP_MEMORY_STATUS_UNSUPPORTED_TYPE;
}

#endif /* AIR_DSP_MEMORY_REGION_ENABLE */

