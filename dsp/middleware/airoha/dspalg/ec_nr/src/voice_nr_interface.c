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
#include "voice_nr_interface.h"
#include "audio_nvdm_common.h"
#include "dsp_dump.h"
#include "string.h"
#include "dsp_memory.h"
#include "dsp_audio_msg_define.h"
#include "dsp_audio_msg.h"
#include "dsp_callback.h"
#include "voice_nr_driver.h"

typedef enum {
    AEC_NR_MODE_INHOUSE_1_2_MIC,
    AEC_NR_MODE_INHOUSE_1_PLUS_1_MIC,
    AEC_NR_MODE_INHOUSE_2_PLUS_1_MIC,
    AEC_NR_MODE_3RD_PARTY,
    AEC_NR_MODE_IGO_1_2_MIC,
    AEC_NR_MODE_IGO_PLUS_1_MIC,
    AEC_NR_MODE_OFFLOAD_PREV,
    AEC_NR_MODE_OFFLOAD_POST,
    AEC_NR_MODE_MAX
} aec_nr_mode_t;

typedef enum {
    ECNR_REINIT_MODE_NONE,
    ECNR_REINIT_MODE_SINGLE,
    ECNR_REINIT_MODE_FULL,
    ECNR_REINIT_MODE_MAX
} ecnr_reinit_mode_t;

static ecnr_reinit_mode_t g_ecnr_reinit_mode = ECNR_REINIT_MODE_NONE;

static void AEC_NR_Input_Buffer_Fetch(void *para, uint32_t length, int16_t **mic_1, int16_t **mic_2, int16_t **mic_3, int16_t **ec_path)
{
    uint8_t Channel_num;

    Channel_num = stream_function_get_channel_number(para);
    *mic_1 = (int16_t *)stream_function_get_1st_inout_buffer(para);
    switch (Channel_num) {
        case 2:
            *mic_2 = NULL;
            *mic_3 = NULL;
            *ec_path = (int16_t *)stream_function_get_2nd_inout_buffer(para);
            break;
        case 3:
            *mic_2 = (int16_t *)stream_function_get_2nd_inout_buffer(para);
            *mic_3 = NULL;
            *ec_path = (int16_t *)stream_function_get_3rd_inout_buffer(para);
            break;
#ifdef AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
        case 4:
            *mic_2 = (int16_t *)stream_function_get_2nd_inout_buffer(para);
            *mic_3 = (int16_t *)stream_function_get_3rd_inout_buffer(para);
            *ec_path = (int16_t *)stream_function_get_4th_inout_buffer(para);
            break;
        default:
            DSP_MW_LOG_E("[DSP][VOICE_NR] MIC number un-support %d", 1, Channel_num);
            // configASSERT(0);
#endif
    }

#ifdef AIR_AUDIO_DUMP_ENABLE
    switch (Channel_num) {
        case 2:
            LOG_AUDIO_DUMP((uint8_t *)*mic_1, (uint32_t)length, VOICE_TX_MIC_0);
            break;
        case 3:
            LOG_AUDIO_DUMP((uint8_t *)*mic_1, (uint32_t)length, VOICE_TX_MIC_0);
            LOG_AUDIO_DUMP((uint8_t *)*mic_2, (uint32_t)length, VOICE_TX_MIC_1);
            break;
#ifdef AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
        case 4:
            LOG_AUDIO_DUMP((uint8_t *)*mic_1, (uint32_t)length, VOICE_TX_MIC_0);
            LOG_AUDIO_DUMP((uint8_t *)*mic_2, (uint32_t)length, VOICE_TX_MIC_1);
            LOG_AUDIO_DUMP((uint8_t *)*mic_3, (uint32_t)length, VOICE_TX_MIC_2);
            break;
#endif
    }
    LOG_AUDIO_DUMP((uint8_t *)*ec_path, (uint32_t)length, VOICE_TX_REF);
#endif
}



bool stream_function_aec_nr_initialize(void *para)
{
    DSP_MW_LOG_I("[DSP][VOICE_NR] stream_function_aec_nr_initialize start", 0);

#if defined(AIR_ECNR_1_OR_2_MIC_ENABLE)
    voice_ecnr_set_mic_mode(VOICE_ECNR_MIC_MODE_2);
#endif
#if defined(AIR_ECNR_1MIC_INEAR_ENABLE)
    voice_ecnr_set_mic_mode(VOICE_ECNR_MIC_MODE_1_1);
#endif
#if defined(AIR_ECNR_2MIC_INEAR_ENABLE)
    voice_ecnr_set_mic_mode(VOICE_ECNR_MIC_MODE_2_1);
#endif
#if defined(AIR_AI_NR_PREMIUM_INEAR_ENABLE)
    voice_ecnr_set_mic_mode(VOICE_ECNR_MIC_MODE_2_1);
#endif

#if defined(AIR_3RD_PARTY_NR_ENABLE)
    voice_ecnr_nr_init(para);
    voice_ecnr_ec_postec_init(para);
#endif
#if defined(AIR_ECNR_1MIC_INEAR_ENABLE) || defined(AIR_ECNR_2MIC_INEAR_ENABLE) || defined(AIR_ECNR_1_OR_2_MIC_ENABLE)
    voice_ecnr_all_init(para);
#endif

    return false;
}

void stream_function_aec_nr_deinitialize(bool is_aec_only)
{
    DSP_MW_LOG_I("[DSP][VOICE_NR] stream_function_aec_nr_deinitialize start", 0);

#if defined(AIR_3RD_PARTY_NR_ENABLE)
    voice_ecnr_ec_postec_deinit(is_aec_only);
    voice_ecnr_nr_deinit(is_aec_only);
#endif
#if defined(AIR_ECNR_1MIC_INEAR_ENABLE) || defined(AIR_ECNR_2MIC_INEAR_ENABLE) || defined(AIR_ECNR_1_OR_2_MIC_ENABLE)
    voice_ecnr_all_deinit(is_aec_only);
#endif

    if (is_aec_only == true) {
        g_ecnr_reinit_mode = ECNR_REINIT_MODE_SINGLE;
    } else {
        g_ecnr_reinit_mode = ECNR_REINIT_MODE_FULL;
    }
}


bool stream_function_aec_process(void *para)
{
    uint16_t Length, i2PreAvcVol, i2PostAvcVol;
    int16_t *InBufLeft, *InBufRight, *InBufMic3, *EC_path_buf;

    if (g_ecnr_reinit_mode == ECNR_REINIT_MODE_FULL) {
        stream_feature_reinitialize(para);
        g_ecnr_reinit_mode = ECNR_REINIT_MODE_NONE;
        DSP_MW_LOG_I("[DSP][VOICE_NR] reinitialize all", 0);
        return false;
    } else if (g_ecnr_reinit_mode == ECNR_REINIT_MODE_SINGLE) {
        stream_function_aec_nr_initialize(para);
        g_ecnr_reinit_mode = ECNR_REINIT_MODE_NONE;
        audio_nvdm_update_status(AUDIO_NVDM_USER_HFP, AUDIO_NVDM_STATUS_POST_CHANGE);
        DSP_MW_LOG_I("[DSP][VOICE_NR] reinitialize only voice nr", 0);
    }

    if (voice_nr_check_enable() == false) {
        return false;
    }

#ifdef AIR_AIRDUMP_ENABLE
    voice_nr_airdump();
#endif

    i2PreAvcVol = gDspAlgParameter.AecNr.avc_vol;

    Length = (uint16_t)stream_function_get_output_size(para);
    AEC_NR_Input_Buffer_Fetch(para, Length, &InBufLeft, &InBufRight, &InBufMic3, &EC_path_buf);

#ifdef AIR_3RD_PARTY_NR_ENABLE
    /* SWB: 480 sample for 15ms, WB: 240 sample for 15ms */
    int16_t EC_out1_buf[480];
    int16_t EC_out2_buf[480];
    int16_t EC_fb_buf[480];
    int16_t NR_out_buf[480];

    voice_ecnr_ec_postec_ec_process(para, Length, InBufLeft, InBufRight, InBufMic3, EC_path_buf, EC_out1_buf, EC_out2_buf, EC_fb_buf);
    voice_ecnr_nr_process(para, Length, EC_out1_buf, EC_fb_buf, EC_path_buf, NR_out_buf);
    voice_ecnr_ec_postec_process(para, Length, NR_out_buf, InBufLeft);
#else
    int16_t out_buf[480];
    voice_ecnr_all_process(para, Length, InBufLeft, InBufRight, InBufMic3, EC_path_buf, out_buf);
    memcpy(InBufLeft, out_buf, Length);
#endif

#ifdef AIR_AUDIO_DUMP_ENABLE
    LOG_AUDIO_DUMP((uint8_t *)InBufLeft, Length, VOICE_TX_NR_OUT);
#endif

    /* Check whether need to update AVC to other side */
    i2PostAvcVol = gDspAlgParameter.AecNr.avc_vol;
    voice_avc_vol_send_update(i2PreAvcVol, i2PostAvcVol);

    return false;
}

bool stream_function_nr_initialize(void *para)
{
    stream_function_aec_nr_initialize(para);

    return false;
}

void stream_function_nr_deinitialize(bool is_aec_only)
{
    UNUSED(is_aec_only);
}

bool stream_function_nr_process(void *para)
{
    int16_t *Buf    = (int16_t *)stream_function_get_1st_inout_buffer(para);
    uint16_t Length  = (uint16_t)stream_function_get_output_size(para);
    uint16_t i;

    if (voice_nr_check_enable() == false) {
        return false;
    }

    voice_rxnr_process(para, Length, Buf);

    for (i = 1; i < stream_function_get_device_channel_number(para); i++) {
        memcpy(stream_function_get_inout_buffer(para, i + 1),
               stream_function_get_inout_buffer(para, 1),
               stream_function_get_output_size(para));
    }

    return false;
}

#if defined(AIR_3RD_PARTY_NR_ENABLE)

bool stream_function_tx_eq_initialize(void *para)
{
    voice_ecnr_tx_eq_init(para);

    return false;
}

void stream_function_tx_eq_deinitialize(bool is_aec_only)
{
    voice_ecnr_tx_eq_deinit(is_aec_only);
}

bool stream_function_tx_eq_process(void *para)
{
    int16_t *Buf = (int16_t *)stream_function_get_1st_inout_buffer(para);
    int16_t Length = (uint16_t)stream_function_get_output_size(para);

    voice_ecnr_tx_eq_process(para, Length, Buf);

    return false;
}

#if defined(AIR_ECNR_PREV_PART_ENABLE)

static ecnr_reinit_mode_t g_ecnr_prev_reinit_mode = ECNR_REINIT_MODE_NONE;

uint8_t stream_function_ecnr_prev_get_postec_gain(void)
{
    return voice_ecnr_ec_get_postec_gain();
}

bool stream_function_ecnr_prev_initialize(void *para)
{
    voice_ecnr_ec_init(para);

    return false;
}

void stream_function_ecnr_prev_deinitialize(bool is_aec_only)
{
    DSP_MW_LOG_I("[DSP][VOICE_NR] stream_function_ecnr_prev_deinitialize start", 0);

    voice_ecnr_ec_deinit(is_aec_only);

    if (is_aec_only == true) {
        g_ecnr_prev_reinit_mode = ECNR_REINIT_MODE_SINGLE;
    } else {
        g_ecnr_prev_reinit_mode = ECNR_REINIT_MODE_FULL;
    }
}

bool stream_function_ecnr_prev_process(void *para)
{
    uint16_t Length;
    int16_t *InBufLeft, *InBufRight, *InBufMic3, *EC_path_buf;

    if (g_ecnr_prev_reinit_mode == ECNR_REINIT_MODE_FULL) {
        stream_feature_reinitialize(para);
        g_ecnr_prev_reinit_mode = ECNR_REINIT_MODE_NONE;
        DSP_MW_LOG_I("[DSP][VOICE_NR] reinitialize all", 0);
        return false;
    } else if (g_ecnr_prev_reinit_mode == ECNR_REINIT_MODE_SINGLE) {
        stream_function_ecnr_prev_initialize(para);
        g_ecnr_prev_reinit_mode = ECNR_REINIT_MODE_NONE;
        audio_nvdm_update_status(AUDIO_NVDM_USER_HFP, AUDIO_NVDM_STATUS_POST_CHANGE);
        DSP_MW_LOG_I("[DSP][VOICE_NR] reinitialize only voice nr", 0);
    }

    Length = (uint16_t)stream_function_get_output_size(para);
    AEC_NR_Input_Buffer_Fetch(para, Length, &InBufLeft, &InBufRight, &InBufMic3, &EC_path_buf);

    if (voice_nr_check_enable() == false) {
        return false;
    }

    voice_ecnr_ec_process(para, Length, InBufLeft, InBufRight, NULL, EC_path_buf, InBufLeft);

    return false;
}

#endif

#ifdef AIR_ECNR_POST_PART_ENABLE

static ecnr_reinit_mode_t g_ecnr_post_reinit_mode = ECNR_REINIT_MODE_NONE;

bool stream_function_ecnr_post_initialize(void *para)
{
    voice_ecnr_nr_init(para);
    voice_ecnr_postec_init(para);

    return false;
}

void stream_function_ecnr_post_deinitialize(bool is_aec_only)
{
    DSP_MW_LOG_I("[DSP][VOICE_NR] stream_function_ecnr_post_deinitialize start", 0);

    voice_ecnr_postec_deinit(is_aec_only);
    voice_ecnr_nr_deinit(is_aec_only);

    if (is_aec_only == true) {
        g_ecnr_post_reinit_mode = ECNR_REINIT_MODE_SINGLE;
    } else {
        g_ecnr_post_reinit_mode = ECNR_REINIT_MODE_FULL;
    }
}

bool stream_function_ecnr_post_process(void *para)
{
    uint16_t Length;
    int16_t *InBufLeft;
    uint8_t temp_buf[480];
    voice_ecnr_status_t ret;

    if (g_ecnr_post_reinit_mode == ECNR_REINIT_MODE_FULL) {
        stream_feature_reinitialize(para);
        g_ecnr_post_reinit_mode = ECNR_REINIT_MODE_NONE;
        DSP_MW_LOG_I("[DSP][VOICE_NR] reinitialize all", 0);
        return false;
    } else if (g_ecnr_post_reinit_mode == ECNR_REINIT_MODE_SINGLE) {
        stream_function_ecnr_post_initialize(para);
        g_ecnr_post_reinit_mode = ECNR_REINIT_MODE_NONE;
        audio_nvdm_update_status(AUDIO_NVDM_USER_HFP, AUDIO_NVDM_STATUS_POST_CHANGE);
        DSP_MW_LOG_I("[DSP][VOICE_NR] reinitialize only voice nr", 0);
    }

    InBufLeft = (int16_t *)stream_function_get_1st_inout_buffer(para);
    Length = (uint16_t)stream_function_get_output_size(para);

#ifdef AIR_AUDIO_DUMP_ENABLE
    LOG_AUDIO_DUMP((uint8_t *)InBufLeft, Length, AUDIO_ENHANCEMENT_IN_L);
#endif

    if (voice_nr_check_enable() == false) {
        return false;
    }

    ret = voice_ecnr_nr_process(para, Length, InBufLeft, NULL, NULL, (int16_t *)temp_buf);
    if (ret == VOICE_ECNR_STATUS_BYPASS) {
        return false;
    }
    if (ret == VOICE_ECNR_STATUS_ERROR) {
        return true;
    }

    voice_ecnr_postec_process(para, Length, (int16_t *)temp_buf, InBufLeft);

    return false;
}

#endif
#endif
