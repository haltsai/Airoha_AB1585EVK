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

#ifndef _SCENARIO_ULL_AUDIO_V2_H_
#define _SCENARIO_ULL_AUDIO_V2_H_

#if defined(AIR_ULL_AUDIO_V2_DONGLE_ENABLE)

/* Includes ------------------------------------------------------------------*/
#include "config.h"
#include "types.h"
#include "source_inter.h"
#include "sink_inter.h"
#include "source.h"
#include "sink.h"
#include "transform_inter.h"
#include "stream_audio_hardware.h"
#include "stream_audio_setting.h"
#include "stream_audio_driver.h"
#include "dsp_audio_msg_define.h"
#include "dsp_audio_msg.h"
#include "hal_hw_semaphore.h"
#include "hal_resource_assignment.h"
#include "clk_skew_sw.h"
#include "sw_gain_interface.h"
#include "sw_buffer_interface.h"
#include "sw_mixer_interface.h"
#include "sw_src_interface.h"
#include "src_fixed_ratio_interface.h"
#include "common.h"

/* Public define -------------------------------------------------------------*/
#define ULL_AUDIO_V2_DATA_CHANNEL_NUMBER            4

/* Public typedef ------------------------------------------------------------*/
typedef enum {
    ULL_AUDIO_V2_DONGLE_STREAM_DEINIT = 0,
    ULL_AUDIO_V2_DONGLE_STREAM_INIT,
    ULL_AUDIO_V2_DONGLE_STREAM_START,
    ULL_AUDIO_V2_DONGLE_STREAM_RUNNING,
} ull_audio_v2_dongle_stream_status_t;

typedef enum {
    ULL_AUDIO_V2_DONGLE_DL_DATA_EMPTY = 0,
    ULL_AUDIO_V2_DONGLE_DL_DATA_IN_STREAM,
    ULL_AUDIO_V2_DONGLE_DL_DATA_IN_MIXER,
    ULL_AUDIO_V2_DONGLE_UL_DATA_EMPTY,
    ULL_AUDIO_V2_DONGLE_UL_DATA_NORMAL,
    ULL_AUDIO_V2_DONGLE_UL_DATA_PLC,
    ULL_AUDIO_V2_DONGLE_UL_DATA_BYPASS_DECODER,
} ull_audio_v2_dongle_data_status_t;

typedef enum {
    ULL_AUDIO_V2_DONGLE_DL_FIRST_PACKET_NOT_READY = 0,
    ULL_AUDIO_V2_DONGLE_DL_FIRST_PACKET_READY,
    ULL_AUDIO_V2_DONGLE_DL_FIRST_PACKET_TIMEOUT,
    ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_NOT_READY,
    ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_READY,
    ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_TIMEOUT,
    ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_PLAYED,
} ull_audio_v2_dongle_first_packet_status_t;

typedef enum {
    ULL_AUDIO_V2_DONGLE_MIXER_UNMIX = 0,
    ULL_AUDIO_V2_DONGLE_MIXER_MIX,
} ull_audio_v2_dongle_mixer_status_t;

typedef enum {
    ULL_AUDIO_V2_COMPENSATORY_METHOD_DISABLE = 0,
    ULL_AUDIO_V2_COMPENSATORY_METHOD_SW_CLK_SKEW,
} ull_audio_v2_dongle_compensatory_method_t;

typedef struct  {
    uint8_t  seq_num;
    uint8_t  user_count;
    uint16_t blk_index;
    uint16_t blk_index_previous;
    uint32_t crc32_init;
    ull_audio_v2_dongle_bt_link_info_t bt_link_info;
} ull_audio_v2_dongle_bt_info_t;

typedef struct {
    uint32_t channel_num;
    uint32_t sample_rate;
    hal_audio_format_t sample_format;
    uint32_t frame_size;
    uint32_t frame_samples;
    uint32_t frame_interval;
    uint32_t frame_max_num;
} ull_audio_v2_dongle_usb_in_info_t;

typedef struct {
    audio_dsp_codec_type_t codec_type;
    uint32_t channel_num;
    uint32_t sample_rate;
    hal_audio_format_t sample_format;
    uint32_t frame_size;
    uint32_t frame_samples;
    uint32_t frame_interval;
    uint32_t bit_rate;
    uint32_t bt_retry_window;
    uint32_t bt_interval;
    uint32_t bt_channel_anchor_diff;
    uint16_t play_en_delay;
    uint16_t play_en_first_packet_safe_delay;
    uint32_t play_en_bt_clk;
    uint32_t first_packet_bt_clk;
    uint32_t first_anchor_bt_clk;
    uint32_t fetch_anchor_previous;
    uint32_t fetch_anchor;
    uint32_t channel_data_status;
    ull_audio_v2_dongle_bt_info_t *bt_info[ULL_AUDIO_V2_DATA_CHANNEL_NUMBER];
} ull_audio_v2_dongle_bt_in_info_t;

typedef union {
    uint32_t reserved;
    ull_audio_v2_dongle_usb_in_info_t usb_in;
    ull_audio_v2_dongle_bt_in_info_t bt_in;
} ull_audio_v2_dongle_source_info_t;

typedef struct {
    audio_dsp_codec_type_t codec_type;
    uint32_t channel_num;
    uint32_t sample_rate;
    hal_audio_format_t sample_format;
    uint32_t frame_size;
    uint32_t frame_samples;
    uint32_t frame_interval;
    uint32_t bit_rate;
    ull_audio_v2_dongle_bt_info_t *bt_info[ULL_AUDIO_V2_DATA_CHANNEL_NUMBER];
} ull_audio_v2_dongle_bt_out_info_t;

typedef struct {
    uint32_t channel_num;
    uint32_t sample_rate;
    hal_audio_format_t sample_format;
    uint32_t frame_size;
    uint32_t frame_samples;
    uint32_t frame_interval;
    uint32_t frame_max_num;
    uint16_t seq_num;
} ull_audio_v2_dongle_usb_out_info_t;

typedef union {
    uint32_t reserved;
    ull_audio_v2_dongle_bt_out_info_t bt_out;
    ull_audio_v2_dongle_usb_out_info_t usb_out;
} ull_audio_v2_dongle_sink_info_t;

typedef struct ull_audio_v2_dongle_dl_handle_t ull_audio_v2_dongle_dl_handle_t;
struct ull_audio_v2_dongle_dl_handle_t {
    void *owner;
    int16_t total_number;
    int16_t index;
    ull_audio_v2_dongle_dl_handle_t *next_dl_handle;
    audio_transmitter_scenario_sub_id_ullaudiov2dongle_t sub_id;
    SOURCE source;
    SINK   sink;
    ull_audio_v2_dongle_stream_status_t stream_status;
    ull_audio_v2_dongle_data_status_t data_status;
    ull_audio_v2_dongle_first_packet_status_t first_packet_status;
    ull_audio_v2_dongle_source_info_t source_info;
    ull_audio_v2_dongle_sink_info_t sink_info;
    uint32_t ccni_in_bt_count;
    uint32_t ccni_in_gpt_count;
    uint32_t data_in_gpt_count;
    uint32_t data_out_gpt_count;
    uint32_t data_out_bt_count;
    uint16_t fetch_count;
    uint16_t ccni_blk_index;
    uint32_t process_frames;
    sw_gain_port_t *gain_port;
    uint32_t buffer_default_output_size;
    uint32_t buffer0_output_size;
    sw_buffer_port_t *buffer_port_0;
    sw_clk_skew_port_t *clk_skew_port;
    ull_audio_v2_dongle_compensatory_method_t compen_method;
    int16_t compen_samples;
    int16_t clk_skew_count;
    int32_t clk_skew_watermark_samples;
    int32_t clk_skew_compensation_mode;
    src_fixed_ratio_port_t *src_port;
    uint16_t src_in_frame_size;
    uint16_t src_in_frame_samples;
    uint16_t src_out_frame_size;
    uint16_t src_out_frame_samples;
    ull_audio_v2_dongle_mixer_status_t mixer_status;
    sw_mixer_member_t *mixer_member;
};

typedef struct ull_audio_v2_dongle_ul_handle_t ull_audio_v2_dongle_ul_handle_t;
struct ull_audio_v2_dongle_ul_handle_t {
    void *owner;
    int16_t total_number;
    int16_t index;
    ull_audio_v2_dongle_ul_handle_t *next_ul_handle;
    audio_transmitter_scenario_sub_id_ullaudiov2dongle_t sub_id;
    SOURCE source;
    SINK   sink;
    ull_audio_v2_dongle_stream_status_t stream_status;
    ull_audio_v2_dongle_data_status_t data_status;
    ull_audio_v2_dongle_first_packet_status_t first_packet_status;
    ull_audio_v2_dongle_source_info_t source_info;
    ull_audio_v2_dongle_sink_info_t sink_info;
    uint32_t ccni_in_bt_count;
    uint32_t ccni_in_gpt_count;
    uint32_t data_in_gpt_count;
    uint32_t data_out_gpt_count;
    uint32_t data_out_bt_count;
    uint16_t fetch_count;
    uint16_t process_frames;
    uint16_t drop_frames;
    sw_mixer_member_t *mixer_member;
    sw_clk_skew_port_t *clk_skew_port;
    int16_t compen_samples;
    int16_t clk_skew_count;
    int32_t clk_skew_watermark_samples;
    sw_buffer_port_t *buffer_port;
    uint32_t buffer_output_size;
    uint32_t buffer_default_output_size;
    sw_gain_port_t *gain_port;
    sw_src_port_t *src_port;
    uint16_t src_in_frame_size;
    uint16_t src_in_frame_samples;
    uint16_t src_out_frame_size;
    uint16_t src_out_frame_samples;
};

/* Public macro --------------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
/******************************************************************************/
/*               ULL audio 2.0 dongle common Public Functions                 */
/******************************************************************************/
extern void ull_audio_v2_dongle_init_play_info(hal_ccni_message_t msg, hal_ccni_message_t *ack);
/******************************************************************************/
/*               ULL audio 2.0 dongle DL path Public Functions                */
/******************************************************************************/
extern uint32_t ull_audio_v2_dongle_dl_get_stream_in_max_size_each_channel(SOURCE source, SINK sink);
extern uint32_t ull_audio_v2_dongle_dl_get_stream_in_channel_number(SOURCE source, SINK sink);
extern stream_samplerate_t ull_audio_v2_dongle_dl_get_stream_in_sampling_rate_each_channel(SOURCE source, SINK sink);
extern uint32_t ull_audio_v2_dongle_dl_get_stream_out_max_size_each_channel(SOURCE source, SINK sink);
extern uint32_t ull_audio_v2_dongle_dl_get_stream_out_channel_number(SOURCE source, SINK sink);
extern stream_samplerate_t ull_audio_v2_dongle_dl_get_stream_out_sampling_rate_each_channel(SOURCE source, SINK sink);
extern void ull_audio_v2_dongle_dl_init(SOURCE source, SINK sink, audio_transmitter_open_param_p audio_transmitter_open_param, bt_common_open_param_p bt_common_open_param);
extern void ull_audio_v2_dongle_dl_deinit(SOURCE source, SINK sink);
extern void ull_audio_v2_dongle_dl_start(SOURCE source, SINK sink);
extern void ull_audio_v2_dongle_dl_stop(SOURCE source, SINK sink);
extern bool ull_audio_v2_dongle_dl_config(SOURCE source, stream_config_type type, U32 value);
extern bool ull_audio_v2_dongle_dl_source_get_avail_size(SOURCE source, uint32_t *avail_size);
extern uint32_t ull_audio_v2_dongle_dl_source_copy_payload(SOURCE source, uint8_t *src_buf, uint8_t *dst_buf, uint32_t length);
extern uint32_t ull_audio_v2_dongle_dl_source_get_new_read_offset(SOURCE source, uint32_t amount);
extern void ull_audio_v2_dongle_dl_source_drop_postprocess(SOURCE source, uint32_t amount);
extern bool ull_audio_v2_dongle_dl_source_close(SOURCE source);
extern bool ull_audio_v2_dongle_dl_sink_get_avail_size(SINK sink, uint32_t *avail_size);
extern uint32_t ull_audio_v2_dongle_dl_sink_copy_payload(SINK sink, uint8_t *src_buf, uint32_t length);
extern bool ull_audio_v2_dongle_dl_sink_get_new_write_offset(SINK sink, U32 amount, uint32_t *new_write_offset);
extern bool ull_audio_v2_dongle_dl_sink_query_notification(SINK sink, bool *notification_flag);
extern bool ull_audio_v2_dongle_dl_sink_send_data_ready_notification(SINK sink);
extern void ull_audio_v2_dongle_dl_sink_flush_postprocess(SINK sink, uint32_t amount);
extern bool ull_audio_v2_dongle_dl_sink_close(SINK sink);
/******************************************************************************/
/*               ULL audio 2.0 dongle UL path Public Functions                */
/******************************************************************************/
extern uint32_t ull_audio_v2_dongle_ul_get_stream_in_max_size_each_channel(SOURCE source, SINK sink);
extern uint32_t ull_audio_v2_dongle_ul_get_stream_in_channel_number(SOURCE source, SINK sink);
extern stream_samplerate_t ull_audio_v2_dongle_ul_get_stream_in_sampling_rate_each_channel(SOURCE source, SINK sink);
extern uint32_t ull_audio_v2_dongle_ul_get_stream_out_max_size_each_channel(SOURCE source, SINK sink);
extern uint32_t ull_audio_v2_dongle_ul_get_stream_out_channel_number(SOURCE source, SINK sink);
extern stream_samplerate_t ull_audio_v2_dongle_ul_get_stream_out_sampling_rate_each_channel(SOURCE source, SINK sink);
extern void ull_audio_v2_dongle_ul_init(SOURCE source, SINK sink, audio_transmitter_open_param_p audio_transmitter_open_param, bt_common_open_param_p bt_common_open_param);
extern void ull_audio_v2_dongle_ul_deinit(SOURCE source, SINK sink);
extern void ull_audio_v2_dongle_ul_start(SOURCE source, SINK sink);
extern bool ull_audio_v2_dongle_ul_config(SOURCE source, stream_config_type type, U32 value);
extern bool ull_audio_v2_dongle_ul_source_get_avail_size(SOURCE source, uint32_t *avail_size);
extern uint32_t ull_audio_v2_dongle_ul_source_copy_payload(SOURCE source, uint8_t *dst_buf, uint32_t length);
extern bool ull_audio_v2_dongle_ul_source_get_new_read_offset(SOURCE source, U32 amount, U32 *new_read_offset);
extern void ull_audio_v2_dongle_ul_source_drop_postprocess(SOURCE source, uint32_t amount);
extern bool ull_audio_v2_dongle_ul_source_close(SOURCE source);
extern bool ull_audio_v2_dongle_ul_sink_get_avail_size(SINK sink, uint32_t *avail_size);
extern uint32_t ull_audio_v2_dongle_ul_sink_copy_payload(SINK sink, uint8_t *src_buf, uint8_t *dst_buf, uint32_t length);
extern void ull_audio_v2_dongle_ul_sink_query_write_offset(SINK sink, uint32_t *write_offset);
extern bool ull_audio_v2_dongle_ul_sink_close(SINK sink);

#endif /* AIR_ULL_AUDIO_V2_DONGLE_ENABLE */

#endif /* _SCENARIO_ULL_AUDIO_V2_H_ */
