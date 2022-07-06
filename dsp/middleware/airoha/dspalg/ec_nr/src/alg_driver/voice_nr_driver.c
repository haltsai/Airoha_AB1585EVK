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

#include "config.h"
#include "dsp_feature_interface.h"
#include "dsp_utilities.h"
#include "dsp_buffer.h"
#include "dsp_audio_ctrl.h"
#include "dsp_share_memory.h"
#include "audio_nvdm_common.h"
#include "dsp_dump.h"
#include "string.h"
#include "dsp_memory.h"
#include "dsp_audio_msg_define.h"
#include "dsp_audio_msg.h"
#include "dsp_callback.h"
#include "voice_nr_interface.h"
#include "voice_nr_driver.h"
#include "voice_nr_portable.h"


#define VOICE_SWB_15_MS_FRAME_SIZE      (960)
#define VOICE_WB_15_MS_FRAME_SIZE       (480)
#define VOICE_WB_7P5_MS_FRAME_SIZE      (240)
#define DSP_AEC_NR_SCRATCH_MEMSIZE      (1)

#define DSP_ALG_ECNR_EC_BUFFER_MAGIC_NUMBER         0x6128800
#define DSP_ALG_ECNR_NR_BUFFER_MAGIC_NUMBER         0x6128801
#define DSP_ALG_ECNR_POSTEC_BUFFER_MAGIC_NUMBER     0x6128802
#define DSP_ALG_ECNR_TXEQ_BUFFER_MAGIC_NUMBER       0x6128803
#define DSP_ALG_ECNR_EC_POSTEC_BUFFER_MAGIC_NUMBER  0x6128804
#define DSP_ALG_ECNR_ALL_BUFFER_MAGIC_NUMBER        0x6128805
#define DSP_ALG_ECNR_BUFFER_INVALID_MAGIC_NUMBER    0x6128880

typedef struct {
    uint32_t MemoryCheck;
    uint32_t deinit_process;
    uint32_t init_flag;
} voice_ecnr_common_para_t;

#ifdef AIR_BTA_IC_PREMIUM_G2
DSP_PARA_AEC_NR_STRU *g_voice_ecnr_nvkey_aec;
#elif defined(AIR_BTA_IC_PREMIUM_G3)
DSP_PARA_AEC_STRU *g_voice_ecnr_nvkey_aec;
#endif

static voice_ecnr_status_t voice_ecnr_common_init(void *para, voice_ecnr_common_para_t **p_common, bool alloc_flag, uint32_t alloc_size, uint32_t magic_number)
{
    bool re_init;
    DSP_STREAMING_PARA_PTR stream_ptr;
    voice_ecnr_common_para_t *p_common_curr;

    p_common_curr = *p_common;

    /* Check and alloc working buffer */
    re_init = false;
    if ((p_common_curr == NULL) || (p_common_curr->MemoryCheck != magic_number)) {
#ifdef AIR_DSP_MEMORY_REGION_ENABLE
        uint32_t work_buf_length;
        UNUSED(stream_ptr);
        if (alloc_flag == true) {
            work_buf_length = stream_function_get_working_buffer_length(para);
            AUDIO_ASSERT(work_buf_length >= alloc_size);
            p_common_curr = (voice_ecnr_common_para_t *)stream_function_get_working_buffer(para);
        }
#else
        UNUSED(alloc_flag);
        stream_ptr = DSP_STREAMING_GET_FROM_PRAR(para);
        p_common_curr = (voice_ecnr_common_para_t *)DSPMEM_tmalloc(DAV_TASK_ID, alloc_size, stream_ptr);
#endif
        p_common_curr->MemoryCheck = magic_number;
        p_common_curr->deinit_process = false;
        p_common_curr->init_flag = false;
        re_init = true;
    }
    *p_common = p_common_curr;

    /* Check whether later init is needed */
    if ((re_init == false) && (p_common_curr->deinit_process == false)) {
        return VOICE_ECNR_STATUS_BYPASS;
    }

    return VOICE_ECNR_STATUS_OK;
}

static void voice_ecnr_common_deinit(voice_ecnr_common_para_t **p_common, bool is_nvkey_only, uint32_t magic_number)
{
    voice_ecnr_common_para_t *p_common_curr;

    p_common_curr = *p_common;
    if ((p_common_curr == NULL) || (p_common_curr->MemoryCheck != magic_number)) {
        DSP_MW_LOG_E("[DSP][VOICE_NR] deinit check, working memory check fail", 0);
        return;
    }

    if (is_nvkey_only == true) {
        p_common_curr->deinit_process = true;
    } else {
        p_common_curr->MemoryCheck = DSP_ALG_ECNR_BUFFER_INVALID_MAGIC_NUMBER;
        p_common_curr = NULL;
    }
}

static voice_ecnr_mic_mode_t g_voice_ecnr_mic_mode;

void voice_ecnr_set_mic_mode(voice_ecnr_mic_mode_t mode)
{
    g_voice_ecnr_mic_mode = mode;
}

static bool voice_ecnr_check_running_status_internal(voice_ecnr_common_para_t *common, uint32_t magic_number)
{
    if ((common != NULL) &&
        (common->init_flag == true) &&
        (common->MemoryCheck == magic_number)) {
        return true;
    }

    return false;
}

/************************************
 * For Rx NR part library porting
 ************************************/

#ifdef AIR_BTA_IC_PREMIUM_G2

typedef struct {
    DSP_ALIGN4 DSP_PARA_WB_RX_EQ_STRU rx_eq;
} nvkey_rx_nr_config_t;

static voice_ecnr_status_t voice_rxnr_init(void *p_working_buf, DSP_PARA_AEC_NR_STRU *nvkey_aec_nr, nvkey_rx_nr_config_t *nvkey_rx_config)
{
    if (gDspAlgParameter.EscoMode.Rx == VOICE_WB) {
        nvkey_read_full_key(NVKEY_DSP_PARA_WB_RX_EQ, &nvkey_rx_config->rx_eq, sizeof(DSP_PARA_WB_RX_EQ_STRU));
    } else {
        nvkey_read_full_key(NVKEY_DSP_PARA_NB_RX_EQ, &nvkey_rx_config->rx_eq, sizeof(DSP_PARA_NB_RX_EQ_STRU));
    }

    WB_RX_NR_init(p_working_buf, nvkey_aec_nr, &nvkey_rx_config->rx_eq);

    return VOICE_ECNR_STATUS_OK;
}

#elif defined(AIR_BTA_IC_PREMIUM_G3)

typedef struct {
    DSP_ALIGN4 DSP_PARA_SWB_RX_EQ_STRU rx_eq;
} nvkey_rx_nr_config_t;

static voice_ecnr_status_t voice_rxnr_init(void *p_working_buf, DSP_PARA_NR_STRU *nvkey_nr, DSP_PARA_AEC_NR_SWB_STRU *nvkey_swb_nr, nvkey_rx_nr_config_t *nvkey_rx_config)
{
    if (gDspAlgParameter.EscoMode.Rx == VOICE_SWB) {
        nvkey_read_full_key(NVKEY_DSP_PARA_SWB_RX_EQ, &nvkey_rx_config->rx_eq, sizeof(DSP_PARA_SWB_RX_EQ_STRU));
        SWB_RX_NR_init(p_working_buf, nvkey_nr, nvkey_swb_nr, &nvkey_rx_config->rx_eq);
    } else if (gDspAlgParameter.EscoMode.Rx == VOICE_WB) {
        nvkey_read_full_key(NVKEY_DSP_PARA_WB_RX_EQ, &nvkey_rx_config->rx_eq, sizeof(DSP_PARA_WB_RX_EQ_STRU));
        WB_RX_NR_init(p_working_buf, nvkey_nr, &nvkey_rx_config->rx_eq);
    } else {
        nvkey_read_full_key(NVKEY_DSP_PARA_NB_RX_EQ, &nvkey_rx_config->rx_eq, sizeof(DSP_PARA_NB_RX_EQ_STRU));
        WB_RX_NR_init(p_working_buf, nvkey_nr, &nvkey_rx_config->rx_eq);
    }

    return VOICE_ECNR_STATUS_OK;
}

#endif

voice_ecnr_status_t voice_rxnr_process(void *para, uint32_t length, int16_t *in_out)
{
    UNUSED(para);

    if (length == 0) {
        return VOICE_ECNR_STATUS_BYPASS;
    }

    ///TODO:: check the flow here
#ifndef AIR_3RD_PARTY_NR_ENABLE
#if 0
    if (gNeedChangeRxEQ) {
        EQ_update(p_ecnr_bss, p_rxeq_NvKey);
        gNeedChangeRxEQ = false;
        DSP_MW_LOG_I("aec_nr change RX EQ param", 0);
    }
#endif
#endif

    LOG_AUDIO_DUMP((uint8_t *)in_out, length, VOICE_RX_NR_IN);

    switch (gDspAlgParameter.EscoMode.Rx) {
        /* Rx side always convert to 16K when NB is configured. */
        case VOICE_NB:
        case VOICE_WB:
            if (length != VOICE_WB_15_MS_FRAME_SIZE) {
                return VOICE_ECNR_STATUS_ERROR;
            }
            Voice_WB_RX_Prcs(in_out);
            break;
        case VOICE_SWB:
#ifdef AIR_BTA_IC_PREMIUM_G3
            if (length != VOICE_SWB_15_MS_FRAME_SIZE) {
                return VOICE_ECNR_STATUS_ERROR;
            }
            Voice_SWB_RX_Prcs(in_out);
#endif
            break;
        default:
            return VOICE_ECNR_STATUS_ERROR;
    }

#ifdef AIR_AUDIO_DUMP_ENABLE
    LOG_AUDIO_DUMP((uint8_t *)in_out, length, VOICE_RX_NR_OUT);
#endif

    return VOICE_ECNR_STATUS_OK;
}


/************************************
 * For EC library porting
 ************************************/
#if defined(AIR_3RD_PARTY_NR_ENABLE)
#if defined(AIR_ECNR_PREV_PART_ENABLE)

typedef struct {
    voice_ecnr_common_para_t common;
    DSP_ALIGN4 DSP_PARA_AEC_STRU nvkey_aec;
#ifdef AIR_BTA_IC_PREMIUM_G2
    DSP_ALIGN4 DSP_PARA_NR_STRU nvkey_nr;
    DSP_ALIGN4 DSP_PARA_INEAR_STRU nvkey_inear;
    DSP_ALIGN4 DSP_PARA_AEC_NR_STRU nvkey_aec_nr;
#endif
    DSP_ALIGN8 uint8_t AecOut[VOICE_SWB_15_MS_FRAME_SIZE];
    DSP_ALIGN16 uint8_t ScratchMemory[DSP_AEC_NR_SCRATCH_MEMSIZE]; //Set at the end of structure for dynamic size
} voice_ecnr_ec_para_t;

static voice_ecnr_ec_para_t *g_voice_ecnr_ec_para = NULL;

bool voice_ecnr_ec_check_running_status(void)
{
    return voice_ecnr_check_running_status_internal((voice_ecnr_common_para_t *)g_voice_ecnr_ec_para, DSP_ALG_ECNR_EC_BUFFER_MAGIC_NUMBER);
}

voice_ecnr_status_t voice_ecnr_ec_init(void *para)
{
    voice_ecnr_status_t ret;

    /* Check whether init is needed */
    ret = voice_ecnr_common_init(para, (voice_ecnr_common_para_t **)&g_voice_ecnr_ec_para, true, sizeof(g_voice_ecnr_ec_para) + get_ec120_memsize(), DSP_ALG_ECNR_EC_BUFFER_MAGIC_NUMBER);
    if (ret == VOICE_ECNR_STATUS_BYPASS) {
        return VOICE_ECNR_STATUS_BYPASS;
    }

    /* load nvkey */
    nvkey_read_full_key(NVKEY_DSP_PARA_AEC, &g_voice_ecnr_ec_para->nvkey_aec, sizeof(DSP_PARA_AEC_STRU));
#ifdef AIR_BTA_IC_PREMIUM_G2
    nvkey_read_full_key(NVKEY_DSP_PARA_NR, &g_voice_ecnr_ec_para->nvkey_nr, sizeof(DSP_PARA_NR_STRU));
    nvkey_read_full_key(NVKEY_DSP_PARA_INEAR, &g_voice_ecnr_ec_para->nvkey_inear, sizeof(DSP_PARA_INEAR_STRU));
    voice_nvkey_aec_nr_porting_layer(&g_voice_ecnr_ec_para->nvkey_aec, &g_voice_ecnr_ec_para->nvkey_nr, &g_voice_ecnr_ec_para->nvkey_inear, &g_voice_ecnr_ec_para->nvkey_aec_nr);
#endif

    /* Init the alg */
#ifdef AIR_BTA_IC_PREMIUM_G2
    Voice_EC120_Init(VOICE_WB, g_voice_ecnr_ec_para->ScratchMemory, &g_voice_ecnr_ec_para->nvkey_aec_nr, (int16_t *)&g_voice_ecnr_ec_para->nvkey_aec_nr.WB_NR_TX_NOISE_GAIN_LIMITER);
#elif defined(AIR_BTA_IC_PREMIUM_G3)
    Voice_EC120_Init(VOICE_WB, g_voice_ecnr_ec_para->ScratchMemory, &g_voice_ecnr_ec_para->nvkey_aec, (int16_t *)&g_voice_ecnr_ec_para->nvkey_aec.WB_NR_TX_NOISE_GAIN_LIMITER);
#endif

    g_voice_ecnr_ec_para->common.init_flag = true;

    /* Query the alg version log */
    DSP_MW_LOG_I("[DSP][VOICE_NR] inhouse EC version %d", 1, get_EC120_SVN());

    return VOICE_ECNR_STATUS_OK;
}

voice_ecnr_status_t voice_ecnr_ec_deinit(bool is_nvkey_only)
{
    voice_ecnr_common_deinit((voice_ecnr_common_para_t **)&g_voice_ecnr_ec_para, is_nvkey_only, DSP_ALG_ECNR_EC_BUFFER_MAGIC_NUMBER);

    return VOICE_ECNR_STATUS_OK;
}

voice_ecnr_status_t voice_ecnr_ec_process(void *para, uint32_t length, int16_t *main_mic, int16_t *ff_mic, int16_t *fb_mic, int16_t *ec_in, int16_t *out)
{
    uint8_t extra_OutBuf[VOICE_WB_7P5_MS_FRAME_SIZE];

    UNUSED(para);
    UNUSED(fb_mic);

    if (length != VOICE_WB_7P5_MS_FRAME_SIZE) {
        return VOICE_ECNR_STATUS_ERROR;
    }

    Voice_EC120_Prcs(main_mic, ff_mic, ec_in, (int16_t *)g_voice_ecnr_ec_para->AecOut, (int16_t *)extra_OutBuf);
    memcpy(out, g_voice_ecnr_ec_para->AecOut, length);

#ifdef AIR_AUDIO_DUMP_ENABLE
    LOG_AUDIO_DUMP((uint8_t *)out, length, AUDIO_ENHANCEMENT_IN_L);
#endif

    /* Get the EC gain for post EC reference */
    gDspAlgParameter.PostEC_Gain = PostEC120_Info(&gDspAlgParameter.AecNr);

    return VOICE_ECNR_STATUS_OK;
}

uint32_t voice_ecnr_ec_get_ref_gain(int16_t *gain_buf)
{
    uint32_t i, length;

    length = 0;
    if (voice_ecnr_ec_check_running_status() == true) {
        EC120_REF_GAIN_READBACK(gain_buf);
        length += 8;
        for (i = 0; i < 8; i++) {
            DSP_MW_LOG_I("[DSP][VOICE_NR] DSP get ref gain, GainAddr:0x%x", 1, gain_buf[i]);
        }
    }

    return length;
}

uint8_t voice_ecnr_ec_get_postec_gain(void)
{
    return gDspAlgParameter.PostEC_Gain;
}

#endif
#endif



/************************************
 * For NR library porting
 ************************************/
#if defined(AIR_3RD_PARTY_NR_ENABLE)

typedef struct {
    voice_ecnr_common_para_t common;
#if defined( AIR_AI_NR_PREMIUM_INEAR_270K_ENABLE)
    DSP_ALIGN4 DSP_PARA_RESERVED_STRU ResKey;
#else
    DSP_ALIGN4 DSP_PARA_RESERVED_STRU ResKey[2];
#endif
#ifdef AIR_BTA_IC_PREMIUM_G2
    DSP_ALIGN4 DSP_PARA_WB_TX_EQ_STRU ff_eq;
#elif defined(AIR_BTA_IC_PREMIUM_G3)
    DSP_ALIGN4 DSP_PARA_SWB_TX_EQ_STRU ff_eq;
#endif
#if defined(AIR_AI_NR_PREMIUM_INEAR_ENABLE)
    DSP_ALIGN4 DSP_PARA_AST_EQ_STRU fb_eq;
#endif
    DSP_ALIGN16 uint8_t ScratchMemory[DSP_AEC_NR_SCRATCH_MEMSIZE]; //Set at the end of structure for dynamic size
} voice_ecnr_nr_para_t;

static uint32_t g_voice_ecnr_nr_memsize;
static voice_ecnr_nr_para_t *g_voice_ecnr_nr_para = NULL;

#ifdef AIR_AI_NR_PREMIUM_INEAR_ENABLE
#ifdef MTK_ANC_ENABLE
#include "anc_api.h"
#endif
static _SWITCH_MODE igo_nr_get_curr_mode(void)
{
    _SWITCH_MODE anc_mode;

#ifdef MTK_ANC_ENABLE
    uint8_t anc_enable;
    dsp_anc_control_type_t anc_type;
    dsp_anc_control_get_status(&anc_enable, &anc_type, NULL);
    if (anc_enable == true) {
        if (anc_type == DSP_ANC_CONTROL_TYPE_PASSTHRU_FF) {
            anc_mode = _PASSTHROUGH;
        } else {
            anc_mode = _ANC_ON;
        }
    } else {
        anc_mode = _ANC_OFF;
    }
#else
    anc_mode = _ANC_OFF;
#endif

    return anc_mode;
}

#if !defined( AIR_AI_NR_PREMIUM_INEAR_270K_ENABLE)
static uint16_t g_voice_ecnr_ec_postec_fb_gain;
#endif

#endif

#if defined(AIR_AI_NR_PREMIUM_INEAR_ENABLE) || defined(AIR_AI_NR_PREMIUM_ENABLE)
#if !defined(AIR_AI_NR_PREMIUM_INEAR_270K_ENABLE)
static _NR_LEVEL g_igo_nr_level = _NR_LEVEL1;
#endif
#endif

bool voice_ecnr_nr_check_running_status(void)
{
    return voice_ecnr_check_running_status_internal((voice_ecnr_common_para_t *)g_voice_ecnr_nr_para, DSP_ALG_ECNR_NR_BUFFER_MAGIC_NUMBER);
}

voice_ecnr_status_t voice_ecnr_nr_init(void *para)
{
    uint32_t work_buf_size;
    voice_ecnr_status_t ret;

#if defined(AIR_AI_NR_PREMIUM_INEAR_ENABLE) || defined(AIR_AI_NR_PREMIUM_ENABLE)
    work_buf_size = IGO_NR_MEMSIZE;
#else
    work_buf_size = 0;
#endif

    /* Check whether init is needed */
    g_voice_ecnr_nr_memsize = sizeof(voice_ecnr_nr_para_t) + work_buf_size;
    ret = voice_ecnr_common_init(para, (voice_ecnr_common_para_t **)&g_voice_ecnr_nr_para, true, g_voice_ecnr_nr_memsize, DSP_ALG_ECNR_NR_BUFFER_MAGIC_NUMBER);
    if (ret == VOICE_ECNR_STATUS_BYPASS) {
        return VOICE_ECNR_STATUS_BYPASS;
    }

    /* load nvkey */
#if defined(AIR_AI_NR_PREMIUM_INEAR_270K_ENABLE)
    nvkey_read_full_key(NVKEY_DSP_PARA_RESERVED, &g_voice_ecnr_nr_para->ResKey, sizeof(DSP_PARA_RESERVED_STRU));
#else
    nvkey_read_full_key(NVKEY_DSP_PARA_RESERVED, &g_voice_ecnr_nr_para->ResKey[0], sizeof(DSP_PARA_RESERVED_STRU));
    nvkey_read_full_key(NVKEY_DSP_PARA_RESERVED_1, &g_voice_ecnr_nr_para->ResKey[1], sizeof(DSP_PARA_RESERVED_STRU));
#endif
    if (gDspAlgParameter.EscoMode.Tx == VOICE_SWB) {
#ifdef AIR_BTA_IC_PREMIUM_G3
        nvkey_read_full_key(NVKEY_DSP_PARA_SWB_TX_EQ, &g_voice_ecnr_nr_para->ff_eq, sizeof(DSP_PARA_SWB_TX_EQ_STRU));
#endif
    } else if (gDspAlgParameter.EscoMode.Tx == VOICE_WB) {
        nvkey_read_full_key(NVKEY_DSP_PARA_WB_TX_EQ, &g_voice_ecnr_nr_para->ff_eq, sizeof(DSP_PARA_WB_TX_EQ_STRU));
    } else {
        nvkey_read_full_key(NVKEY_DSP_PARA_NB_TX_EQ, &g_voice_ecnr_nr_para->ff_eq, sizeof(DSP_PARA_NB_TX_EQ_STRU));
    }
#if defined(AIR_AI_NR_PREMIUM_INEAR_ENABLE)
    nvkey_read_full_key(NVKEY_DSP_PARA_AST_EQ, &g_voice_ecnr_nr_para->fb_eq, sizeof(DSP_PARA_AST_EQ_STRU));
#endif

    /* Init the alg */
#if defined(AIR_AI_NR_PREMIUM_ENABLE)

#if defined(AIR_AI_NR_PREMIUM_200K_ENABLE)
    if (gDspAlgParameter.EscoMode.Tx == VOICE_NB) {
        IGO_NR_Init(g_voice_ecnr_nr_para->ScratchMemory, (uint32_t *)(g_voice_ecnr_nr_para->ResKey), &g_voice_ecnr_nr_para->ff_eq, g_igo_nr_level, _NB);
    } else {
        IGO_NR_Init(g_voice_ecnr_nr_para->ScratchMemory, (uint32_t *)(g_voice_ecnr_nr_para->ResKey), &g_voice_ecnr_nr_para->ff_eq, g_igo_nr_level, _WB);
    }
    DSP_MW_LOG_I("[DSP][VOICE_NR] NR init nr level %d", 1, g_igo_nr_level);
#endif

#elif defined(AIR_AI_NR_PREMIUM_INEAR_ENABLE)

    _SWITCH_MODE igo_mode;
    _BAND_MODE baud_mode;

    igo_mode = igo_nr_get_curr_mode();
    if (gDspAlgParameter.EscoMode.Tx == VOICE_SWB) {
        baud_mode = _SWB;
    } else {
        baud_mode = _WB;
    }
#if defined( AIR_AI_NR_PREMIUM_INEAR_270K_ENABLE)
    IGO_1MIC_NR_PARAMS *p_params;

    p_params = (IGO_1MIC_NR_PARAMS *)&g_voice_ecnr_nr_para->ResKey;
    DSP_MW_LOG_I("[DSP][VOICE_NR] NR init nr_strength %d, baud_mode %d", 2, p_params->nr_strength, baud_mode);
    UNUSED(igo_mode);
    IGO_NR_Init(g_voice_ecnr_nr_para->ScratchMemory, work_buf_size, p_params, &g_voice_ecnr_nr_para->ff_eq, &g_voice_ecnr_nr_para->fb_eq, baud_mode);
#else
    IGO_NR_Init(g_voice_ecnr_nr_para->ScratchMemory, (uint32_t *)g_voice_ecnr_nr_para->ResKey, &g_voice_ecnr_nr_para->ff_eq, &g_voice_ecnr_nr_para->fb_eq, (uint8_t)igo_mode, g_igo_nr_level, baud_mode);
    DSP_MW_LOG_I("[DSP][VOICE_NR] NR init igo_mode %d, g_igo_nr_level %d, baud_mode %d", 3, igo_mode, g_igo_nr_level, baud_mode);
#endif

#endif

    g_voice_ecnr_nr_para->common.init_flag = true;

    /* Query the alg version log */
    DSP_MW_LOG_I("[DSP][VOICE_NR] NR init done", 0);

    return VOICE_ECNR_STATUS_OK;
}

voice_ecnr_status_t voice_ecnr_nr_deinit(bool is_nvkey_only)
{
    voice_ecnr_common_deinit((voice_ecnr_common_para_t **)&g_voice_ecnr_nr_para, is_nvkey_only, DSP_ALG_ECNR_NR_BUFFER_MAGIC_NUMBER);

    return VOICE_ECNR_STATUS_OK;
}

voice_ecnr_status_t voice_ecnr_nr_process(void *para, uint32_t length, int16_t *main_mic, int16_t *fb_mic, int16_t *ref, int16_t *out)
{
    uint32_t gpt_begin, gpt_end, gpt_during;

    UNUSED(para);

    if (length == 0) {
        return VOICE_ECNR_STATUS_BYPASS;
    }
    if (((gDspAlgParameter.EscoMode.Tx == VOICE_SWB) && (length != VOICE_SWB_15_MS_FRAME_SIZE)) ||
        ((gDspAlgParameter.EscoMode.Tx != VOICE_SWB) && (length != VOICE_WB_15_MS_FRAME_SIZE))) {
        DSP_MW_LOG_E("[DSP][VOICE_NR] frame size invalid, %d", 1, length);
        return VOICE_ECNR_STATUS_ERROR;
    }

    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &gpt_begin);

#if defined(AIR_AI_NR_PREMIUM_ENABLE)

#if defined(AIR_AI_NR_PREMIUM_200K_ENABLE)
    UNUSED(fb_mic);
    UNUSED(ref);
    IGO_NR_Prcs(main_mic, out, (uint32_t *)g_voice_ecnr_nr_para->ResKey, g_igo_nr_level, g_voice_ecnr_nr_para->ScratchMemory);
#else
    UNUSED(main_mic);
    UNUSED(fb_mic);
    UNUSED(ref);
#endif

#elif defined(AIR_AI_NR_PREMIUM_INEAR_ENABLE)

    _SWITCH_MODE igo_mode;
    igo_mode = igo_nr_get_curr_mode();
#if defined( AIR_AI_NR_PREMIUM_INEAR_270K_ENABLE)
    IGO_NR_Prcs(main_mic, fb_mic, ref, out, g_voice_ecnr_nr_para->ScratchMemory, igo_mode);
#else
    IGO_NR_Prcs(main_mic, fb_mic, ref, out, (uint32_t *)g_voice_ecnr_nr_para->ResKey, (uint8_t)igo_mode, g_igo_nr_level, g_voice_ecnr_ec_postec_fb_gain, g_voice_ecnr_nr_para->ScratchMemory);
#endif

#else

    UNUSED(main_mic);
    UNUSED(fb_mic);
    UNUSED(ref);

#endif

    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &gpt_end);
    hal_gpt_get_duration_count(gpt_begin, gpt_end, &gpt_during);
    //DSP_MW_LOG_W("[DSP][VOICE_NR] the 3rd party NR process, time %d us", 1, gpt_during);

#ifdef AIR_AUDIO_DUMP_ENABLE
    LOG_AUDIO_DUMP((uint8_t *)out, length, AUDIO_ENHANCEMENT_OUT_L);
#endif

    return VOICE_ECNR_STATUS_OK;
}

uint32_t voice_ecnr_nr_query_version(uint8_t *version)
{
    uint32_t length;

    if (voice_ecnr_nr_check_running_status() == true) {
#if defined(AIR_AI_NR_PREMIUM_INEAR_200K_ENABLE)
        IGO_NR_Get_Lib_Version((version_info_t *)version);
        length = sizeof(version_info_t);
#else
        UNUSED(version);
        length = 0;
#endif
    } else {
        length = 0;
    }

    return length;
}

void voice_ecnr_nr_set_nr_level(uint32_t nr_level)
{
#if (defined(AIR_AI_NR_PREMIUM_INEAR_ENABLE) || defined(AIR_AI_NR_PREMIUM_ENABLE))
#if !defined( AIR_AI_NR_PREMIUM_INEAR_270K_ENABLE)
    if (nr_level < _NR_LEVEL_MAX) {
        g_igo_nr_level = nr_level;
        DSP_MW_LOG_I("[DSP][AEC/NR] IGO NR level set to %d", 1, nr_level);
    } else {
        DSP_MW_LOG_E("[DSP][AEC/NR] IGO NR level invalid %d, max %d", 2, nr_level, _NR_LEVEL_MAX);
    }
#else
    UNUSED(nr_level);
#endif
#else
    UNUSED(nr_level);
#endif
}

#endif


/************************************
 * For PostEC library porting
 ************************************/
#if defined(AIR_3RD_PARTY_NR_ENABLE)
#if defined(AIR_ECNR_POST_PART_ENABLE)

typedef struct {
    voice_ecnr_common_para_t common;
    DSP_ALIGN4 DSP_PARA_AEC_STRU nvkey_aec;
#ifdef AIR_BTA_IC_PREMIUM_G2
    DSP_ALIGN4 DSP_PARA_NR_STRU nvkey_nr;
    DSP_ALIGN4 DSP_PARA_INEAR_STRU nvkey_inear;
    DSP_ALIGN4 DSP_PARA_AEC_NR_STRU nvkey_aec_nr;
#endif
    DSP_ALIGN16 uint8_t ScratchMemory[DSP_AEC_NR_SCRATCH_MEMSIZE]; //Set at the end of structure for dynamic size
} voice_ecnr_postec_para_t;

static voice_ecnr_postec_para_t *g_voice_ecnr_postec_para = NULL;

extern uint8_t AEC_NR_Port_Get_PostEC_Gain(void);

bool voice_ecnr_postec_check_running_status(void)
{
    return voice_ecnr_check_running_status_internal((voice_ecnr_common_para_t *)g_voice_ecnr_postec_para, DSP_ALG_ECNR_POSTEC_BUFFER_MAGIC_NUMBER);
}

voice_ecnr_status_t voice_ecnr_postec_init(void *para)
{
    voice_ecnr_status_t ret;
    uint32_t alloc_size;

    alloc_size = sizeof(voice_ecnr_postec_para_t) + get_post_ec_memsize();
    DSP_MW_LOG_I("[DSP][VOICE_NR] woeking buffer check, expect %d + %d, actual %d", 3, g_voice_ecnr_nr_memsize, alloc_size, stream_function_get_working_buffer_length(para));
    //AUDIO_ASSERT((g_voice_ecnr_nr_memsize + alloc_size) <= stream_function_get_working_buffer_length(para));

    /* Check whether init is needed */
#ifdef AIR_DSP_MEMORY_REGION_ENABLE
    uint8_t *p_voice_nr_buf;
    p_voice_nr_buf = (uint8_t *)g_voice_ecnr_nr_para;
    g_voice_ecnr_postec_para = (voice_ecnr_postec_para_t *)(p_voice_nr_buf + ((g_voice_ecnr_nr_memsize + 7) / 8) * 8);
    ret = voice_ecnr_common_init(para, (voice_ecnr_common_para_t **)&g_voice_ecnr_postec_para, false, alloc_size, DSP_ALG_ECNR_POSTEC_BUFFER_MAGIC_NUMBER);
#else
    ret = voice_ecnr_common_init(para, (voice_ecnr_common_para_t **)&g_voice_ecnr_postec_para, true, alloc_size, DSP_ALG_ECNR_POSTEC_BUFFER_MAGIC_NUMBER);
#endif
    if (ret == VOICE_ECNR_STATUS_BYPASS) {
        return VOICE_ECNR_STATUS_BYPASS;
    }

    /* load nvkey */
    nvkey_read_full_key(NVKEY_DSP_PARA_AEC, &g_voice_ecnr_postec_para->nvkey_aec, sizeof(DSP_PARA_AEC_STRU));
#ifdef AIR_BTA_IC_PREMIUM_G2
    nvkey_read_full_key(NVKEY_DSP_PARA_NR, &g_voice_ecnr_postec_para->nvkey_nr, sizeof(DSP_PARA_NR_STRU));
    nvkey_read_full_key(NVKEY_DSP_PARA_INEAR, &g_voice_ecnr_postec_para->nvkey_inear, sizeof(DSP_PARA_INEAR_STRU));
    voice_nvkey_aec_nr_porting_layer(&g_voice_ecnr_postec_para->nvkey_aec, &g_voice_ecnr_postec_para->nvkey_nr, &g_voice_ecnr_postec_para->nvkey_inear, &g_voice_ecnr_postec_para->nvkey_aec_nr);
#endif

    /* Init the alg */
#ifdef AIR_BTA_IC_PREMIUM_G2
    EXT_POST_EC_Init(g_voice_ecnr_postec_para->ScratchMemory, &g_voice_ecnr_postec_para->nvkey_aec_nr);
#elif defined(AIR_BTA_IC_PREMIUM_G3)
    EXT_POST_EC_Init(g_voice_ecnr_postec_para->ScratchMemory, &g_voice_ecnr_postec_para->nvkey_aec);
#endif

    g_voice_ecnr_postec_para->common.init_flag = true;

    /* Query the alg version log */
    DSP_MW_LOG_I("[DSP][VOICE_NR] Post EC init done", 0);

    return VOICE_ECNR_STATUS_OK;
}

voice_ecnr_status_t voice_ecnr_postec_deinit(bool is_nvkey_only)
{
    voice_ecnr_common_deinit((voice_ecnr_common_para_t **)&g_voice_ecnr_postec_para, is_nvkey_only, DSP_ALG_ECNR_POSTEC_BUFFER_MAGIC_NUMBER);

    return VOICE_ECNR_STATUS_OK;
}

voice_ecnr_status_t voice_ecnr_postec_process(void *para, uint32_t length, int16_t *in, int16_t *out)
{
    uint8_t post_ec_gain;

    UNUSED(para);

    if (length == 0) {
        return VOICE_ECNR_STATUS_BYPASS;
    }
    if (length != VOICE_WB_15_MS_FRAME_SIZE) {
        DSP_MW_LOG_E("[DSP][VOICE_NR] frame size invalid, expect: 480, actual: %d", 1, length);
        return VOICE_ECNR_STATUS_ERROR;
    }

    /* Using the end of stream buffer for extra data transmit */
    post_ec_gain = AEC_NR_Port_Get_PostEC_Gain();
    EXT_POST_EC_PRCS(g_voice_ecnr_postec_para->ScratchMemory, in, out, post_ec_gain, &(gDspAlgParameter.AecNr));

#ifdef AIR_AUDIO_DUMP_ENABLE
    LOG_AUDIO_DUMP((uint8_t *)out, length, VOICE_TX_NR_OUT);
#endif

    return VOICE_ECNR_STATUS_OK;
}

#endif
#endif


/************************************
 * For TX EQ library porting
 ************************************/
#if defined(AIR_3RD_PARTY_NR_ENABLE)

typedef struct {
    voice_ecnr_common_para_t common;
    DSP_ALIGN4 DSP_PARA_WB_TX_FIR_EQ_STRU TxFirEq;
    DSP_ALIGN16 uint8_t ScratchMemory[DSP_AEC_NR_SCRATCH_MEMSIZE]; //Set at the end of structure for dynamic size
} voice_ecnr_tx_eq_para_t;

static voice_ecnr_tx_eq_para_t *g_voice_ecnr_tx_eq_para = NULL;

bool voice_ecnr_tx_eq_check_running_status(void)
{
    return voice_ecnr_check_running_status_internal((voice_ecnr_common_para_t *)g_voice_ecnr_tx_eq_para, DSP_ALG_ECNR_TXEQ_BUFFER_MAGIC_NUMBER);
}

voice_ecnr_status_t voice_ecnr_tx_eq_init(void *para)
{
    uint16_t key_id;
    voice_ecnr_status_t ret;

    /* Check whether init is needed */
    ret = voice_ecnr_common_init(para, (voice_ecnr_common_para_t **)&g_voice_ecnr_tx_eq_para, true, sizeof(voice_ecnr_tx_eq_para_t) + get_fir_wbeq_memsize(), DSP_ALG_ECNR_TXEQ_BUFFER_MAGIC_NUMBER);
    if (ret == VOICE_ECNR_STATUS_BYPASS) {
        return VOICE_ECNR_STATUS_BYPASS;
    }

    /* load nvkey */
    if (gDspAlgParameter.EscoMode.Tx == VOICE_WB) {
        key_id = NVKEY_DSP_PARA_WB_TX_FIR_EQ;
        nvkey_read_full_key(key_id, &g_voice_ecnr_tx_eq_para->TxFirEq, sizeof(DSP_PARA_WB_TX_FIR_EQ_STRU));
    } else if (gDspAlgParameter.EscoMode.Tx == VOICE_NB) {
        key_id = NVKEY_DSP_PARA_NB_TX_FIR_EQ;
        nvkey_read_full_key(key_id, &g_voice_ecnr_tx_eq_para->TxFirEq, sizeof(DSP_PARA_NB_TX_FIR_EQ_STRU));
    }

    /* Init the alg */
    Voice_TxEQ_init(&g_voice_ecnr_tx_eq_para->ScratchMemory, &g_voice_ecnr_tx_eq_para->TxFirEq);

    g_voice_ecnr_tx_eq_para->common.init_flag = true;

    /* Query the alg version log */
    DSP_MW_LOG_I("[DSP][VOICE_NR] inhouse TX EQ version", 0);

    return VOICE_ECNR_STATUS_OK;
}

voice_ecnr_status_t voice_ecnr_tx_eq_deinit(bool is_nvkey_only)
{
    voice_ecnr_common_deinit((voice_ecnr_common_para_t **)&g_voice_ecnr_tx_eq_para, is_nvkey_only, DSP_ALG_ECNR_TXEQ_BUFFER_MAGIC_NUMBER);

    return VOICE_ECNR_STATUS_OK;
}

voice_ecnr_status_t voice_ecnr_tx_eq_process(void *para, uint32_t length, int16_t *in_out)
{
    UNUSED(para);

    Voice_TxEQ_Prcs_Length(in_out, &g_voice_ecnr_tx_eq_para->ScratchMemory, length >> 1);

    return VOICE_ECNR_STATUS_OK;
}

#endif



/************************************
 * For EC/PostEC library porting
 ************************************/
#if defined(AIR_3RD_PARTY_NR_ENABLE)

typedef struct {
    voice_ecnr_common_para_t common;
    DSP_ALIGN4 nvkey_rx_nr_config_t nvkey_rx;
    DSP_ALIGN4 DSP_PARA_AEC_STRU nvkey_aec;
    DSP_ALIGN4 DSP_PARA_NR_STRU nvkey_nr;
    DSP_ALIGN4 DSP_PARA_INEAR_STRU nvkey_inear;
#ifdef AIR_BTA_IC_PREMIUM_G2
    DSP_ALIGN4 DSP_PARA_AEC_NR_STRU nvkey_aec_nr;
#elif defined(AIR_BTA_IC_PREMIUM_G3)
    DSP_ALIGN4 DSP_PARA_AEC_NR_SWB_STRU nvkey_nr_swb;
#endif
    DSP_ALIGN16 uint8_t ScratchMemory[DSP_AEC_NR_SCRATCH_MEMSIZE]; //Set at the end of structure for dynamic size
} voice_ecnr_ec_postec_para_t;

static voice_ecnr_ec_postec_para_t *g_voice_ecnr_ec_postec_para = NULL;

bool voice_ecnr_ec_postec_check_running_status(void)
{
    return voice_ecnr_check_running_status_internal((voice_ecnr_common_para_t *)g_voice_ecnr_ec_postec_para, DSP_ALG_ECNR_EC_POSTEC_BUFFER_MAGIC_NUMBER);
}

voice_ecnr_status_t voice_ecnr_ec_postec_init(void *para)
{
    uint32_t alloc_size;
    voice_ecnr_status_t ret;

    /* Check whether init is needed */
    if (g_voice_ecnr_mic_mode <= VOICE_ECNR_MIC_MODE_2) {
        alloc_size = get_aec_nr_memsize() + sizeof(voice_ecnr_ec_postec_para_t);
    } else {
        alloc_size = get_aec_nr_inear_memsize() + sizeof(voice_ecnr_ec_postec_para_t);
    }

    DSP_MW_LOG_I("[DSP][VOICE_NR] working buffer check, expect %d + %d, actual %d", 3, g_voice_ecnr_nr_memsize, alloc_size, stream_function_get_working_buffer_length(para));
    //AUDIO_ASSERT((g_voice_ecnr_nr_memsize + alloc_size) <= stream_function_get_working_buffer_length(para));

#ifdef AIR_DSP_MEMORY_REGION_ENABLE
    uint8_t *p_voice_nr_buf;
    p_voice_nr_buf = (uint8_t *)g_voice_ecnr_nr_para;
    g_voice_ecnr_ec_postec_para = (voice_ecnr_ec_postec_para_t *)(p_voice_nr_buf + ((g_voice_ecnr_nr_memsize + 7) / 8) * 8);
    ret = voice_ecnr_common_init(para, (voice_ecnr_common_para_t **)&g_voice_ecnr_ec_postec_para, false, alloc_size, DSP_ALG_ECNR_EC_POSTEC_BUFFER_MAGIC_NUMBER);
#else
    ret = voice_ecnr_common_init(para, (voice_ecnr_common_para_t **)&g_voice_ecnr_ec_postec_para, true, alloc_size, DSP_ALG_ECNR_EC_POSTEC_BUFFER_MAGIC_NUMBER);
#endif
    if (ret == VOICE_ECNR_STATUS_BYPASS) {
        return VOICE_ECNR_STATUS_BYPASS;
    }

    /* load nvkey */
    nvkey_read_full_key(NVKEY_DSP_PARA_AEC, &g_voice_ecnr_ec_postec_para->nvkey_aec, sizeof(DSP_PARA_AEC_STRU));
    nvkey_read_full_key(NVKEY_DSP_PARA_NR, &g_voice_ecnr_ec_postec_para->nvkey_nr, sizeof(DSP_PARA_NR_STRU));
    nvkey_read_full_key(NVKEY_DSP_PARA_INEAR, &g_voice_ecnr_ec_postec_para->nvkey_inear, sizeof(DSP_PARA_INEAR_STRU));
#ifdef AIR_BTA_IC_PREMIUM_G2
    voice_nvkey_aec_nr_porting_layer(&g_voice_ecnr_ec_postec_para->nvkey_aec, &g_voice_ecnr_ec_postec_para->nvkey_nr, &g_voice_ecnr_ec_postec_para->nvkey_inear, &g_voice_ecnr_ec_postec_para->nvkey_aec_nr);
#elif defined(AIR_BTA_IC_PREMIUM_G3)
    if (gDspAlgParameter.EscoMode.Tx == VOICE_SWB) {
        nvkey_read_full_key(NVKEY_DSP_PARA_AEC_NR_SWB, &g_voice_ecnr_ec_postec_para->nvkey_nr_swb, sizeof(DSP_PARA_AEC_NR_SWB_STRU));
    }
#endif

    /* Init the alg */
    if (g_voice_ecnr_mic_mode <= VOICE_ECNR_MIC_MODE_2) {
        if (gDspAlgParameter.EscoMode.Tx == VOICE_SWB) {
#ifdef AIR_BTA_IC_PREMIUM_G3
            Voice_SWB_EC_Init(&g_voice_ecnr_ec_postec_para->ScratchMemory,
                              &g_voice_ecnr_ec_postec_para->nvkey_aec, &g_voice_ecnr_ec_postec_para->nvkey_nr,
                              &g_voice_ecnr_ec_postec_para->nvkey_inear, &g_voice_ecnr_ec_postec_para->nvkey_nr_swb);
#endif
        } else {
#ifdef AIR_BTA_IC_PREMIUM_G2
            Voice_EC_Init(gDspAlgParameter.EscoMode.Tx, g_voice_ecnr_ec_postec_para->ScratchMemory, &(g_voice_ecnr_ec_postec_para->nvkey_aec_nr), (int16_t *)(&g_voice_ecnr_ec_postec_para->nvkey_aec_nr.WB_NR_TX_NOISE_GAIN_LIMITER));
#elif defined(AIR_BTA_IC_PREMIUM_G3)
            Voice_EC_Init(gDspAlgParameter.EscoMode.Tx, &g_voice_ecnr_ec_postec_para->ScratchMemory,
                          &g_voice_ecnr_ec_postec_para->nvkey_aec, &g_voice_ecnr_ec_postec_para->nvkey_nr,
                          &g_voice_ecnr_ec_postec_para->nvkey_inear, (int16_t *)&g_voice_ecnr_ec_postec_para->nvkey_nr.WB_NR_TX_NOISE_GAIN_LIMITER);
#endif
        }
    } else {
        if (gDspAlgParameter.EscoMode.Tx == VOICE_SWB) {
#ifdef AIR_BTA_IC_PREMIUM_G3
            Voice_SWB_EC_Inear_Init(&g_voice_ecnr_ec_postec_para->ScratchMemory,
                                    &g_voice_ecnr_ec_postec_para->nvkey_aec, &g_voice_ecnr_ec_postec_para->nvkey_nr,
                                    &g_voice_ecnr_ec_postec_para->nvkey_inear, &g_voice_ecnr_ec_postec_para->nvkey_nr_swb);
#endif
        } else {
#ifdef AIR_BTA_IC_PREMIUM_G2
            Voice_EC_Inear_Init(gDspAlgParameter.EscoMode.Tx, g_voice_ecnr_ec_postec_para->ScratchMemory, &(g_voice_ecnr_ec_postec_para->nvkey_aec_nr), (int16_t *)(&g_voice_ecnr_ec_postec_para->nvkey_aec_nr.WB_NR_TX_NOISE_GAIN_LIMITER));
#elif defined(AIR_BTA_IC_PREMIUM_G3)
            Voice_EC_Inear_Init(gDspAlgParameter.EscoMode.Tx, &g_voice_ecnr_ec_postec_para->ScratchMemory,
                                &g_voice_ecnr_ec_postec_para->nvkey_aec, &g_voice_ecnr_ec_postec_para->nvkey_nr,
                                &g_voice_ecnr_ec_postec_para->nvkey_inear, (int16_t *)&g_voice_ecnr_ec_postec_para->nvkey_nr.WB_NR_TX_NOISE_GAIN_LIMITER);
#endif
        }
    }

#ifdef AIR_BTA_IC_PREMIUM_G2
    g_voice_ecnr_nvkey_aec = &g_voice_ecnr_ec_postec_para->nvkey_aec_nr;
#elif defined(AIR_BTA_IC_PREMIUM_G3)
    g_voice_ecnr_nvkey_aec = &g_voice_ecnr_ec_postec_para->nvkey_aec;
#endif

    /* Init RX NR part */
#ifdef AIR_BTA_IC_PREMIUM_G2
    voice_rxnr_init(&g_voice_ecnr_ec_postec_para->ScratchMemory, &g_voice_ecnr_ec_postec_para->nvkey_aec_nr, &g_voice_ecnr_ec_postec_para->nvkey_rx);
#elif defined(AIR_BTA_IC_PREMIUM_G3)
    voice_rxnr_init(&g_voice_ecnr_ec_postec_para->ScratchMemory, &g_voice_ecnr_ec_postec_para->nvkey_nr,
                    &g_voice_ecnr_ec_postec_para->nvkey_nr_swb, &g_voice_ecnr_ec_postec_para->nvkey_rx);
#endif

    g_voice_ecnr_ec_postec_para->common.init_flag = true;

    /* Query the alg version log */
    DSP_MW_LOG_I("[DSP][VOICE_NR] inhouse EC / POST_EC version %d", 1, get_ECNR_SVN());

    return VOICE_ECNR_STATUS_OK;
}

voice_ecnr_status_t voice_ecnr_ec_postec_deinit(bool is_nvkey_only)
{
    voice_ecnr_common_deinit((voice_ecnr_common_para_t **)&g_voice_ecnr_ec_postec_para, is_nvkey_only, DSP_ALG_ECNR_EC_POSTEC_BUFFER_MAGIC_NUMBER);

    return VOICE_ECNR_STATUS_OK;
}

voice_ecnr_status_t voice_ecnr_ec_postec_ec_process(void *para, uint32_t length, int16_t *main_mic, int16_t *ff_mic, int16_t *fb_mic, int16_t *ec_in, int16_t *main_out, int16_t *ff_out, int16_t *fb_out)
{
    UNUSED(para);
    UNUSED(length);
    UNUSED(ff_out);

    if (g_voice_ecnr_mic_mode <= VOICE_ECNR_MIC_MODE_2) {
        if (gDspAlgParameter.EscoMode.Tx == VOICE_SWB) {
#ifdef AIR_BTA_IC_PREMIUM_G3
            Voice_SWB_EC_Prcs(main_mic, ff_mic, ec_in, main_out, ff_out);
#endif
        } else {
            Voice_EC_Prcs(main_mic, ff_mic, ec_in, main_out, ff_out);
        }
    } else {
        if (fb_mic == NULL) {
            fb_mic = ff_mic;
            ff_mic = NULL;
        }
        if (gDspAlgParameter.EscoMode.Tx == VOICE_SWB) {
#ifdef AIR_BTA_IC_PREMIUM_G3
            Voice_SWB_EC_Inear_Prcs(main_mic, ff_mic, fb_mic, ec_in, main_out, ff_out, fb_out);
#endif
        } else {
            Voice_EC_Inear_Prcs(main_mic, ff_mic, fb_mic, ec_in, main_out, ff_out, fb_out);
        }
#if defined(AIR_AI_NR_PREMIUM_INEAR_ENABLE)
#if !defined( AIR_AI_NR_PREMIUM_INEAR_270K_ENABLE)
        g_voice_ecnr_ec_postec_fb_gain = PF_GAIN_FB_READBACK();
#ifdef AIR_AUDIO_DUMP_ENABLE
        uint16_t fb_gain_dump[10];

        fb_gain_dump[0] = 0xACE0;
        fb_gain_dump[1] = length / 2;
        fb_gain_dump[2] = g_voice_ecnr_ec_postec_fb_gain;
        LOG_AUDIO_DUMP((U8*)fb_gain_dump, sizeof(fb_gain_dump), VOICE_FB_ECOUT_INFO);
        if (fb_out != NULL) {
            LOG_AUDIO_DUMP((U8*)fb_out, length, VOICE_FB_ECOUT_INFO);
        }
#endif
#endif
#endif
    }

#ifdef AIR_AUDIO_DUMP_ENABLE
    LOG_AUDIO_DUMP((uint8_t *)main_out, length, AUDIO_ENHANCEMENT_IN_L);
    if (fb_out != NULL) {
        LOG_AUDIO_DUMP((uint8_t *)fb_out, length, AUDIO_ENHANCEMENT_IN_R);
    }
#endif

    return VOICE_ECNR_STATUS_OK;
}

voice_ecnr_status_t voice_ecnr_ec_postec_process(void *para, uint32_t length, int16_t *in, int16_t *out)
{
    UNUSED(para);
    UNUSED(length);

    if (gDspAlgParameter.EscoMode.Tx == VOICE_SWB) {
#ifdef AIR_BTA_IC_PREMIUM_G3
        Voice_SWB_PostEC_Prcs(in, out, &gDspAlgParameter.AecNr);
#endif
    } else {
        Voice_PostEC_Prcs(in, out, &gDspAlgParameter.AecNr);
    }

    return VOICE_ECNR_STATUS_OK;
}

uint32_t voice_ecnr_ec_postec_get_ref_gain(int16_t *gain_buf)
{
    uint32_t i, length;

    length = 0;
    if (voice_ecnr_ec_postec_check_running_status() == true) {
        EC_REF_GAIN_READBACK(gain_buf);
        length += 8;
        for (i = 0; i < 8; i++) {
            DSP_MW_LOG_I("[DSP][VOICE_NR] DSP get ref gain, GainAddr:0x%x", 1, gain_buf[i]);
        }
#if defined(AIR_AI_NR_PREMIUM_INEAR_ENABLE)
        EC_Inear_REF_GAIN_READBACK(gain_buf + length);
        length += 4;
        for (i = 0; i < 4; i++) {
            DSP_MW_LOG_I("[DSP][VOICE_NR] DSP get inear ref gain, GainAddr:0x%x", 1, gain_buf[length + i]);
        }
#endif
        if (gDspAlgParameter.EscoMode.Tx == VOICE_SWB) {
#ifdef AIR_BTA_IC_PREMIUM_G3
            BAND5_EC_REF_GAIN_READBACK(gain_buf + length);
            for (i = 0; i < 3; i++) {
                DSP_MW_LOG_I("[DSP][VOICE_NR] DSP get band5 ref gain, GainAddr:0x%x", 1, gain_buf[length + i]);
            }
#endif
        }

    }

    return length;
}

#endif






/*****************************************
 * For ECNR(EC/NR/PostEC) library porting
 ****************************************/
#if defined(AIR_ECNR_1MIC_INEAR_ENABLE) || defined(AIR_ECNR_2MIC_INEAR_ENABLE) || defined(AIR_ECNR_1_OR_2_MIC_ENABLE)

typedef struct {
    voice_ecnr_common_para_t common;
    DSP_ALIGN4 nvkey_rx_nr_config_t nvkey_rx;
    DSP_ALIGN4 DSP_PARA_AEC_STRU nvkey_aec;
    DSP_ALIGN4 DSP_PARA_NR_STRU nvkey_nr;
    DSP_ALIGN4 DSP_PARA_INEAR_STRU nvkey_inear;
#ifdef AIR_BTA_IC_PREMIUM_G2
    DSP_ALIGN4 DSP_PARA_AEC_NR_STRU nvkey_aec_nr;
    DSP_ALIGN4 DSP_PARA_WB_TX_EQ_STRU nvkey_tx_eq;
#if defined(AIR_ECNR_1MIC_INEAR_ENABLE) || defined(AIR_ECNR_2MIC_INEAR_ENABLE)
    DSP_ALIGN4 DSP_PARA_InEarEQ_STRU nvkey_inear_eq;
    DSP_ALIGN4 DSP_PARA_AST_EQ_STRU nvkey_ast_eq;
#endif
#elif defined(AIR_BTA_IC_PREMIUM_G3)
    DSP_ALIGN4 DSP_PARA_SWB_TX_EQ_STRU nvkey_tx_eq;
    DSP_ALIGN4 DSP_PARA_AEC_NR_SWB_STRU nvkey_aec_nr_swb;
    DSP_ALIGN4 DSP_PARA_InEarEQ_STRU nvkey_inear_eq;
    DSP_ALIGN4 DSP_PARA_AST_EQ_STRU nvkey_ast_eq;
#endif
    DSP_ALIGN16 uint8_t ScratchMemory[DSP_AEC_NR_SCRATCH_MEMSIZE]; //Set at the end of structure for dynamic size
} voice_ecnr_all_para_t;

static voice_ecnr_all_para_t *g_voice_ecnr_all_para = NULL;

bool voice_ecnr_all_check_running_status(void)
{
    return voice_ecnr_check_running_status_internal((voice_ecnr_common_para_t *)g_voice_ecnr_all_para, DSP_ALG_ECNR_ALL_BUFFER_MAGIC_NUMBER);
}

voice_ecnr_status_t voice_ecnr_all_init(void *para)
{
    bool is_nb_mode;
    uint32_t alloc_size;
    voice_ecnr_status_t ret;

    /* Check whether init is needed */
    alloc_size = get_aec_nr_memsize() + sizeof(voice_ecnr_all_para_t);
    ret = voice_ecnr_common_init(para, (voice_ecnr_common_para_t **)&g_voice_ecnr_all_para, true, alloc_size, DSP_ALG_ECNR_ALL_BUFFER_MAGIC_NUMBER);
    if (ret == VOICE_ECNR_STATUS_BYPASS) {
        return VOICE_ECNR_STATUS_BYPASS;
    }

    /* load nvkey */
    nvkey_read_full_key(NVKEY_DSP_PARA_AEC, &g_voice_ecnr_all_para->nvkey_aec, sizeof(DSP_PARA_AEC_STRU));
    nvkey_read_full_key(NVKEY_DSP_PARA_NR, &g_voice_ecnr_all_para->nvkey_nr, sizeof(DSP_PARA_NR_STRU));
    nvkey_read_full_key(NVKEY_DSP_PARA_INEAR, &g_voice_ecnr_all_para->nvkey_inear, sizeof(DSP_PARA_INEAR_STRU));
#ifdef AIR_BTA_IC_PREMIUM_G2
    voice_nvkey_aec_nr_porting_layer(&g_voice_ecnr_all_para->nvkey_aec, &g_voice_ecnr_all_para->nvkey_nr, &g_voice_ecnr_all_para->nvkey_inear, &g_voice_ecnr_all_para->nvkey_aec_nr);
    if (gDspAlgParameter.EscoMode.Tx == VOICE_NB) {
        nvkey_read_full_key(NVKEY_DSP_PARA_NB_TX_EQ, &g_voice_ecnr_all_para->nvkey_tx_eq, sizeof(DSP_PARA_NB_TX_EQ_STRU));
    } else if (gDspAlgParameter.EscoMode.Tx == VOICE_WB) {
        nvkey_read_full_key(NVKEY_DSP_PARA_WB_TX_EQ, &g_voice_ecnr_all_para->nvkey_tx_eq, sizeof(DSP_PARA_WB_TX_EQ_STRU));
    }
#if defined(AIR_ECNR_1MIC_INEAR_ENABLE) || defined(AIR_ECNR_2MIC_INEAR_ENABLE)
    nvkey_read_full_key(NVKEY_DSP_PARA_INEAR_EQ, &g_voice_ecnr_all_para->nvkey_inear_eq, sizeof(DSP_PARA_InEarEQ_STRU));
    nvkey_read_full_key(NVKEY_DSP_PARA_AST_EQ, &g_voice_ecnr_all_para->nvkey_ast_eq, sizeof(DSP_PARA_AST_EQ_STRU));
#endif
#elif defined(AIR_BTA_IC_PREMIUM_G3)
    if (gDspAlgParameter.EscoMode.Tx == VOICE_NB) {
        nvkey_read_full_key(NVKEY_DSP_PARA_NB_TX_EQ, &g_voice_ecnr_all_para->nvkey_tx_eq, sizeof(DSP_PARA_NB_TX_EQ_STRU));
    } else if (gDspAlgParameter.EscoMode.Tx == VOICE_WB) {
        nvkey_read_full_key(NVKEY_DSP_PARA_WB_TX_EQ, &g_voice_ecnr_all_para->nvkey_tx_eq, sizeof(DSP_PARA_WB_TX_EQ_STRU));
    } else {
        nvkey_read_full_key(NVKEY_DSP_PARA_SWB_TX_EQ, &g_voice_ecnr_all_para->nvkey_tx_eq, sizeof(DSP_PARA_SWB_TX_EQ_STRU));
        nvkey_read_full_key(NVKEY_DSP_PARA_AEC_NR_SWB, &g_voice_ecnr_all_para->nvkey_aec_nr_swb, sizeof(DSP_PARA_AEC_NR_SWB_STRU));
    }
    if (g_voice_ecnr_mic_mode > VOICE_ECNR_MIC_MODE_2) {
        nvkey_read_full_key(NVKEY_DSP_PARA_INEAR_EQ, &g_voice_ecnr_all_para->nvkey_inear_eq, sizeof(DSP_PARA_InEarEQ_STRU));
        nvkey_read_full_key(NVKEY_DSP_PARA_AST_EQ, &g_voice_ecnr_all_para->nvkey_ast_eq, sizeof(DSP_PARA_AST_EQ_STRU));
    }
#endif

    /* Init the alg */
    if (gDspAlgParameter.EscoMode.Tx == VOICE_NB) {
        is_nb_mode = true;
    } else {
        is_nb_mode = false;
    }
    if (g_voice_ecnr_mic_mode <= VOICE_ECNR_MIC_MODE_2) {
        if (gDspAlgParameter.EscoMode.Tx == VOICE_SWB) {
#ifdef AIR_BTA_IC_PREMIUM_G3
            Voice_SWB_TX_Init(NULL, &g_voice_ecnr_all_para->ScratchMemory,
                              &g_voice_ecnr_all_para->nvkey_aec, &g_voice_ecnr_all_para->nvkey_nr,
                              &g_voice_ecnr_all_para->nvkey_inear, &g_voice_ecnr_all_para->nvkey_aec_nr_swb,
                              &g_voice_ecnr_all_para->nvkey_tx_eq);
#endif
        } else {
#ifdef AIR_BTA_IC_PREMIUM_G2
            Voice_WB_TX_Init(gDspAlgParameter.EscoMode.Tx, NULL, &g_voice_ecnr_all_para->ScratchMemory,
                             &(g_voice_ecnr_all_para->nvkey_aec_nr), &g_voice_ecnr_all_para->nvkey_tx_eq);
#elif defined(AIR_BTA_IC_PREMIUM_G3)
            Voice_WB_TX_Init(is_nb_mode, NULL, &g_voice_ecnr_all_para->ScratchMemory,
                             &g_voice_ecnr_all_para->nvkey_aec, &g_voice_ecnr_all_para->nvkey_nr,
                             &g_voice_ecnr_all_para->nvkey_inear, &g_voice_ecnr_all_para->nvkey_tx_eq);

#endif
        }
    } else {
        if (gDspAlgParameter.EscoMode.Tx == VOICE_SWB) {
#ifdef AIR_BTA_IC_PREMIUM_G3
            Voice_SWB_TX_Inear_Init_V2(NULL, &g_voice_ecnr_all_para->ScratchMemory,
                                       &g_voice_ecnr_all_para->nvkey_aec, &g_voice_ecnr_all_para->nvkey_nr,
                                       &g_voice_ecnr_all_para->nvkey_inear, &g_voice_ecnr_all_para->nvkey_aec_nr_swb,
                                       &g_voice_ecnr_all_para->nvkey_tx_eq, &g_voice_ecnr_all_para->nvkey_inear_eq,
                                       &g_voice_ecnr_all_para->nvkey_ast_eq);
#endif
        } else {
#ifdef AIR_BTA_IC_PREMIUM_G2
#ifdef AIR_ECNR_1MIC_INEAR_ENABLE
            Voice_WB_TX_Inear_Init(gDspAlgParameter.EscoMode.Tx, NULL, &g_voice_ecnr_all_para->ScratchMemory,
                                   &(g_voice_ecnr_all_para->nvkey_aec_nr), &g_voice_ecnr_all_para->nvkey_tx_eq,
                                   &g_voice_ecnr_all_para->nvkey_inear_eq, &g_voice_ecnr_all_para->nvkey_ast_eq);
#elif defined(AIR_ECNR_2MIC_INEAR_ENABLE)
            Voice_WB_TX_Inear_Init_V2(gDspAlgParameter.EscoMode.Tx, NULL, &g_voice_ecnr_all_para->ScratchMemory,
                                      &(g_voice_ecnr_all_para->nvkey_aec_nr), &g_voice_ecnr_all_para->nvkey_tx_eq,
                                      &g_voice_ecnr_all_para->nvkey_inear_eq, &g_voice_ecnr_all_para->nvkey_ast_eq);
#endif
#elif defined(AIR_BTA_IC_PREMIUM_G3)
            Voice_WB_TX_Inear_Init_V2(is_nb_mode, NULL, &g_voice_ecnr_all_para->ScratchMemory,
                                      &g_voice_ecnr_all_para->nvkey_aec, &g_voice_ecnr_all_para->nvkey_nr,
                                      &g_voice_ecnr_all_para->nvkey_inear, &g_voice_ecnr_all_para->nvkey_tx_eq,
                                      &g_voice_ecnr_all_para->nvkey_inear_eq, &g_voice_ecnr_all_para->nvkey_ast_eq);

#endif
        }
    }

#ifdef AIR_BTA_IC_PREMIUM_G2
    g_voice_ecnr_nvkey_aec = &g_voice_ecnr_all_para->nvkey_aec_nr;
#elif defined(AIR_BTA_IC_PREMIUM_G3)
    g_voice_ecnr_nvkey_aec = &g_voice_ecnr_all_para->nvkey_aec;
#endif

    /* Init RX NR part */
#ifdef AIR_BTA_IC_PREMIUM_G2
    voice_rxnr_init(&g_voice_ecnr_all_para->ScratchMemory, &g_voice_ecnr_all_para->nvkey_aec_nr, &g_voice_ecnr_all_para->nvkey_rx);
#elif defined(AIR_BTA_IC_PREMIUM_G3)
    voice_rxnr_init(&g_voice_ecnr_all_para->ScratchMemory, &g_voice_ecnr_all_para->nvkey_nr,
                    &g_voice_ecnr_all_para->nvkey_aec_nr_swb, &g_voice_ecnr_all_para->nvkey_rx);
#endif

    g_voice_ecnr_all_para->common.init_flag = true;

    /* Query the alg version log */
#if defined(AIR_ECNR_1MIC_INEAR_ENABLE)
    DSP_MW_LOG_I("[DSP][VOICE_NR] Inhouse 1+1 version %d", 1, get_ECNR_SVN());
#elif defined(AIR_ECNR_2MIC_INEAR_ENABLE)
    DSP_MW_LOG_I("[DSP][VOICE_NR] Inhouse 2+1 version %d", 1, get_ECNR_SVN());
#elif defined(AIR_ECNR_1_OR_2_MIC_ENABLE)
    DSP_MW_LOG_I("[DSP][VOICE_NR] Inhouse 1/2 version %d", 1, get_ECNR_SVN());
#endif

    return VOICE_ECNR_STATUS_OK;
}

voice_ecnr_status_t voice_ecnr_all_deinit(bool is_nvkey_only)
{
    voice_ecnr_common_deinit((voice_ecnr_common_para_t **)&g_voice_ecnr_all_para, is_nvkey_only, DSP_ALG_ECNR_ALL_BUFFER_MAGIC_NUMBER);

    return VOICE_ECNR_STATUS_OK;
}

voice_ecnr_status_t voice_ecnr_all_process(void *para, uint32_t length, int16_t *main_mic, int16_t *ff_mic, int16_t *fb_mic, int16_t *ec_in, int16_t *out)
{
    UNUSED(para);
    UNUSED(length);

    if (g_voice_ecnr_mic_mode <= VOICE_ECNR_MIC_MODE_2) {
        if (gDspAlgParameter.EscoMode.Tx == VOICE_SWB) {
#ifdef AIR_BTA_IC_PREMIUM_G3
            Voice_SWB_TX_Prcs(main_mic, ff_mic, ec_in, out, &gDspAlgParameter.AecNr);
#endif
        } else {
            Voice_WB_TX_Prcs(main_mic, ff_mic, ec_in, out, &gDspAlgParameter.AecNr);
        }
    } else {
        if (fb_mic == NULL) {
            fb_mic = ff_mic;
            ff_mic = NULL;
        }
        if (gDspAlgParameter.EscoMode.Tx == VOICE_SWB) {
#ifdef AIR_BTA_IC_PREMIUM_G3
            Voice_SWB_TX_Inear_Prcs_V2(main_mic, ff_mic, fb_mic, ec_in, out, &gDspAlgParameter.AecNr);
#endif
        } else {
#ifdef AIR_BTA_IC_PREMIUM_G2
#if defined(AIR_ECNR_1MIC_INEAR_ENABLE)
            int16_t g_f_wind, mic_status_o;
            Voice_WB_TX_Inear_Prcs(main_mic, ff_mic, ec_in, out, &gDspAlgParameter.AecNr, &g_f_wind, &mic_status_o);
#elif defined(AIR_ECNR_2MIC_INEAR_ENABLE)
            Voice_WB_TX_Inear_Prcs_V2(main_mic, ff_mic, fb_mic, ec_in, out, &gDspAlgParameter.AecNr);
#endif
#elif defined(AIR_BTA_IC_PREMIUM_G3)
            Voice_WB_TX_Inear_Prcs_V2(main_mic, ff_mic, fb_mic, ec_in, out, &gDspAlgParameter.AecNr);
#endif
        }
    }

    return VOICE_ECNR_STATUS_OK;
}

uint32_t voice_ecnr_all_get_ref_gain(int16_t *gain_buf)
{
    uint32_t i, length;

    length = 0;
    if (voice_ecnr_all_check_running_status() == true) {
        /*
         * airoha 1/2 MIC: EC_REF_GAIN_READBACK (16 bytes, for 1MIC case, 0 for gain[4]~gain[7])
         * airoha 1+1 MIC: EC_REF_GAIN_READBACK (16 bytes, 0 for gain[4]~gain[7]), IE_EC_REF_GAIN_READBACK(8 bytes for +1)
         * airoha 2+1 MIC: EC_REF_GAIN_READBACK (16 bytes), IE_EC_REF_GAIN_READBACK(8 bytes for +1)
         *
         */
        EC_REF_GAIN_READBACK(gain_buf);
        length += 8;
        for (i = 0; i < 8; i++) {
            DSP_MW_LOG_I("[DSP][VOICE_NR] DSP get ref gain, GainAddr:0x%x", 1, gain_buf[i]);
        }
        if (g_voice_ecnr_mic_mode > VOICE_ECNR_MIC_MODE_2) {
#if (defined(AIR_BTA_IC_PREMIUM_G2) && (defined(AIR_ECNR_1MIC_INEAR_ENABLE) || defined(AIR_ECNR_2MIC_INEAR_ENABLE))) || defined(AIR_BTA_IC_PREMIUM_G3)
            IE_EC_REF_GAIN_READBACK(gain_buf + length);
            for (i = 0; i < 4; i++) {
                DSP_MW_LOG_I("[DSP][VOICE_NR] DSP get inear ref gain, GainAddr:0x%x", 1, gain_buf[length + i]);
            }
            length += 4;
#endif
        }
        if (gDspAlgParameter.EscoMode.Tx == VOICE_SWB) {
#if defined(AIR_BTA_IC_PREMIUM_G3)
            BAND5_EC_REF_GAIN_READBACK(gain_buf + length);
            length += 3;
#endif
        }
    }

    return length;
}

#endif

