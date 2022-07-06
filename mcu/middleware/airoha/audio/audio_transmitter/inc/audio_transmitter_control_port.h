/* Copyright Statement:
 *
 * (C) 2020  Airoha Technology Corp. All rights reserved.
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
#ifndef __AUDIO_TRANSMITTER_CONTROL_PORT_H__
#define __AUDIO_TRANSMITTER_CONTROL_PORT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "audio_transmitter_mcu_dsp_common.h"

/*****************************scenario config************************************************/

#if defined (AIR_MULTI_MIC_STREAM_ENABLE)
#include "hal_audio_internal.h"
#define MAX_MULTI_MIC_STREAM_NUMBER 4
typedef struct {
    hal_audio_device_t audio_device;
    hal_audio_interface_t audio_interface;
} audio_mic_channel_config_t;
typedef struct {
    audio_mic_channel_config_t mic_configs[MAX_MULTI_MIC_STREAM_NUMBER];
    hal_audio_mic_config_t *mic_para;
    uint32_t sampling_rate;
    uint16_t frame_size;
    uint8_t frame_number;
    hal_audio_format_t format;
    bool echo_reference;
} audio_transmitter_multi_mic_stream_config_t;
#endif


#if defined (MTK_AUDIO_LOOPBACK_TEST_ENABLE)
typedef struct {
    hal_audio_device_t audio_device;
    hal_audio_interface_t audio_interface;
} audio_transmitter_audio_loopback_test_config_t;
#endif

typedef union {
#if defined (MTK_AUDIO_LOOPBACK_TEST_ENABLE)
    audio_transmitter_audio_loopback_test_config_t    audio_loopback_test_config;
#endif
    uint32_t reserved;
} audio_transmitter_test_config_t;

typedef struct {
    audio_dsp_codec_type_t codec_type;
    audio_codec_param_t codec_param;
} linein_dongle_config_t;

typedef struct {
    audio_dsp_codec_type_t codec_type;
    audio_codec_param_t codec_param;
    audio_dsp_codec_type_t  usb_type;
    audio_codec_param_t     usb_param;
} lineout_dongle_config_t;

typedef struct {
    audio_dsp_codec_type_t       codec_type;
    audio_codec_param_t          codec_param;
    hal_audio_device_t           audio_device;
    hal_audio_interface_t        audio_interface;
    hal_audio_i2s_format_t       i2s_fromat;
    hal_audio_i2s_word_length_t  i2s_word_length;
} i2s_in_dongle_config_t;

#if defined (AIR_BT_ULTRA_LOW_LATENCY_ENABLE)
typedef struct {
    audio_dsp_codec_type_t codec_type;
    audio_codec_param_t codec_param;
} voice_headset_config_t;
typedef struct {
    audio_dsp_codec_type_t codec_type;
    audio_codec_param_t codec_param;
    audio_dsp_codec_type_t  usb_type;
    audio_codec_param_t     usb_param;
} music_dongle_config_t;
typedef struct {
    audio_dsp_codec_type_t codec_type;
    audio_codec_param_t codec_param;
    audio_dsp_codec_type_t  usb_type;
    audio_codec_param_t     usb_param;
} voice_dongle_config_t;
typedef struct {
    union {
        voice_headset_config_t voice_headset_config;
        music_dongle_config_t music_dongle_config;
        voice_dongle_config_t voice_dongle_config;
        linein_dongle_config_t linein_dongle_config;
        lineout_dongle_config_t lineout_dongle_config;
        i2s_in_dongle_config_t i2s_in_dongle_config;
    };
    //uint32_t reserved;
} audio_transmitter_gaming_mode_config_t;
#endif

#if defined (AIR_BLE_AUDIO_DONGLE_ENABLE)
typedef struct {
    bool                    test_mode_enable;
    uint32_t                period;
    uint32_t                channel_enable;
    audio_dsp_codec_type_t codec_type;
    audio_codec_param_t codec_param;
    audio_dsp_codec_type_t  usb_type;
    audio_codec_param_t     usb_param;
} music_ble_audio_dongle_config_t;

typedef struct {
    bool                    test_mode_enable;
    uint32_t                period;
    uint32_t                channel_enable;
    audio_dsp_codec_type_t codec_type;
    audio_codec_param_t codec_param;
    audio_dsp_codec_type_t  usb_type;
    audio_codec_param_t     usb_param;
} voice_ble_audio_dongle_config_t;

typedef struct {
    union {
        music_ble_audio_dongle_config_t music_ble_audio_dongle_config;
        voice_ble_audio_dongle_config_t voice_ble_audio_dongle_config;
    };
    //uint32_t reserved;
} audio_transmitter_ble_audio_dongle_config_t;
#endif /* AIR_BLE_AUDIO_DONGLE_ENABLE */

#if defined (AIR_AUDIO_I2S_SLAVE_TDM_ENABLE)
typedef struct {
    hal_audio_memory_t in_memory;
    hal_audio_memory_t out_memory;
    hal_audio_interface_t in_interface;
    hal_audio_interface_t out_interface;
    hal_audio_sampling_rate_t sampling_rate;
    uint32_t frame_size;
    hal_audio_i2s_tdm_channel_setting_t in_channel;
    hal_audio_i2s_tdm_channel_setting_t out_channel;
} audio_transmitter_tdm_config_t;
#endif

#if defined (AIR_WIRED_AUDIO_ENABLE)
typedef struct {
    audio_dsp_codec_type_t codec_type;
    audio_codec_param_t codec_param;
    audio_dsp_codec_type_t  usb_codec_type;
    audio_codec_param_t     usb_codec_param;
} usb_out_config_t;
typedef struct {
    union {
        usb_out_config_t usb_out_config;
    };
} audio_transmitter_wired_audio_config_t;

#endif

#if defined (AIR_ULL_AUDIO_V2_DONGLE_ENABLE)
typedef struct {
    audio_dsp_codec_type_t  codec_type;
    audio_codec_param_t     codec_param;
} ull_audio_v2_dongle_usb_in_param_t;

typedef struct {
    audio_dsp_codec_type_t  codec_type;
    audio_codec_param_t     codec_param;
} ull_audio_v2_dongle_line_in_param_t;

typedef struct {
    audio_dsp_codec_type_t  codec_type;
    audio_codec_param_t     codec_param;
} ull_audio_v2_dongle_i2s_mst_in_param_t;

typedef struct {
    audio_dsp_codec_type_t  codec_type;
    audio_codec_param_t     codec_param;
} ull_audio_v2_dongle_i2s_slv_in_param_t;

typedef struct {
    union {
        ull_audio_v2_dongle_usb_in_param_t      usb_in_param;
        ull_audio_v2_dongle_line_in_param_t     line_in_param;
        ull_audio_v2_dongle_i2s_mst_in_param_t  i2s_mst_in_param;
        ull_audio_v2_dongle_i2s_slv_in_param_t  i2s_slv_in_param;
    };
} audio_transmitter_ull_audio_v2_dl_source_param_t;

typedef struct {
    bool                    enable;
    void *                  share_info;
    audio_dsp_codec_type_t  codec_type;
    audio_codec_param_t     codec_param;
} ull_audio_v2_dongle_bt_link_param_t;

typedef struct {
    uint32_t                                link_num;
    ull_audio_v2_dongle_bt_link_param_t     link_param[4];
} ull_audio_v2_dongle_bt_out_param_t;

typedef struct {
    union {
        ull_audio_v2_dongle_bt_out_param_t      bt_out_param;
    };
} audio_transmitter_ull_audio_v2_dl_sink_param_t;

typedef struct {
    audio_transmitter_ull_audio_v2_dl_source_param_t source_param;
    audio_transmitter_ull_audio_v2_dl_sink_param_t   sink_param;
} ull_audio_v2_dongle_dl_config_t;

typedef struct {
    uint32_t                                link_num;
    ull_audio_v2_dongle_bt_link_param_t     link_param[4];
} ull_audio_v2_dongle_bt_in_param_t;

typedef struct {
    union {
        ull_audio_v2_dongle_bt_in_param_t      bt_in_param;
    };
} audio_transmitter_ull_audio_v2_ul_source_param_t;

typedef struct {
    audio_dsp_codec_type_t  codec_type;
    audio_codec_param_t     codec_param;
} ull_audio_v2_dongle_usb_out_param_t;

typedef struct {
    audio_dsp_codec_type_t  codec_type;
    audio_codec_param_t     codec_param;
} ull_audio_v2_dongle_line_out_param_t;

typedef struct {
    audio_dsp_codec_type_t  codec_type;
    audio_codec_param_t     codec_param;
} ull_audio_v2_dongle_i2s_mst_out_param_t;

typedef struct {
    audio_dsp_codec_type_t  codec_type;
    audio_codec_param_t     codec_param;
} ull_audio_v2_dongle_i2s_slv_out_param_t;
typedef struct {
    union {
        ull_audio_v2_dongle_usb_out_param_t      usb_out_param;
        ull_audio_v2_dongle_line_out_param_t     line_out_param;
        ull_audio_v2_dongle_i2s_mst_out_param_t  i2s_mst_out_param;
        ull_audio_v2_dongle_i2s_slv_out_param_t  i2s_slv_out_param;
    };
} audio_transmitter_ull_audio_v2_ul_sink_param_t;

typedef struct {
    audio_transmitter_ull_audio_v2_ul_source_param_t source_param;
    audio_transmitter_ull_audio_v2_ul_sink_param_t   sink_param;
} ull_audio_v2_dongle_ul_config_t;

typedef struct {
    union {
        ull_audio_v2_dongle_dl_config_t dl_config;
        ull_audio_v2_dongle_ul_config_t ul_config;
    };
    //uint32_t reserved;
} audio_transmitter_ull_audio_v2_dongle_config_t;
#endif /* AIR_ULL_AUDIO_V2_DONGLE_ENABLE */

#if defined (AIR_DUAL_CHIP_MIXING_MODE_ROLE_MASTER_ENABLE) || defined (AIR_DUAL_CHIP_MIXING_MODE_ROLE_SLAVE_ENABLE)
typedef struct {
    hal_audio_memory_t in_memory;
    hal_audio_memory_t out_memory;
    hal_audio_interface_t in_interface;
    hal_audio_interface_t out_interface;
    hal_audio_format_t format;
    uint32_t sampling_rate;
    uint32_t frame_size;
    uint32_t frame_number;
    uint32_t  irq_period;
    uint32_t  start_asi;
    uint32_t  start_bt_clk;
    uint32_t  start_bt_intra_clk;
} audio_transmitter_dchs_config_t;
#endif

typedef struct {
    union {

#if defined (AIR_MULTI_MIC_STREAM_ENABLE)
        audio_transmitter_multi_mic_stream_config_t    multi_mic_stream_config;
#endif
#if defined (AIR_BT_ULTRA_LOW_LATENCY_ENABLE)
        audio_transmitter_gaming_mode_config_t    gaming_mode_config;
#endif
        audio_transmitter_test_config_t    audio_transmitter_test_config;
#if defined (AIR_AUDIO_I2S_SLAVE_TDM_ENABLE)
        audio_transmitter_tdm_config_t    tdm_config;
#endif
#if defined (AIR_BLE_AUDIO_DONGLE_ENABLE)
        audio_transmitter_ble_audio_dongle_config_t    ble_audio_dongle_config;
#endif /* AIR_BLE_AUDIO_DONGLE_ENABLE */
#if defined (AIR_WIRED_AUDIO_ENABLE)
        audio_transmitter_wired_audio_config_t wired_audio_config;
#endif
#if defined (AIR_ULL_AUDIO_V2_DONGLE_ENABLE)
        audio_transmitter_ull_audio_v2_dongle_config_t ull_audio_v2_dongle_config;
#endif /* AIR_ULL_AUDIO_V2_DONGLE_ENABLE */
#if defined (AIR_DUAL_CHIP_MIXING_MODE_ROLE_MASTER_ENABLE) || defined (AIR_DUAL_CHIP_MIXING_MODE_ROLE_SLAVE_ENABLE)
        audio_transmitter_dchs_config_t dchs_config;
#endif /* AIR_DUAL_CHIP_MIXING_MODE_ROLE_MASTER_ENABLE */
        uint32_t reserved;
    };
} scenario_config_t;

/*****************************scenario runtime config************************************************/
#if defined (AIR_BT_ULTRA_LOW_LATENCY_ENABLE)
typedef struct {
    uint8_t vol_level_l;
    uint8_t vol_level_r;
    uint8_t vol_ratio;
} vol_level_t;
typedef struct {
    union {
        vol_level_t vol_level;
        int8_t dl_mixer_id;
        uint32_t latency_us;
    };
} audio_transmitter_gaming_mode_runtime_config_t;
#endif

#if defined (AIR_WIRED_AUDIO_ENABLE)
typedef struct {
    uint8_t vol_level_l;
    uint8_t vol_level_r;
    uint8_t vol_ratio;
} wired_audio_vol_level_t;
typedef struct {
    union {
        wired_audio_vol_level_t vol_level;
        int8_t dl_mixer_id;
    };
} audio_transmitter_wired_audio_runtime_config_t;
#endif

#if defined (AIR_BLE_AUDIO_DONGLE_ENABLE)
typedef struct {
    uint8_t vol_level_l;
    uint8_t vol_level_r;
    uint8_t vol_ratio;
} vol_level_t;
typedef struct {
    union {
        vol_level_t vol_level;
        int8_t dl_mixer_id;
        uint32_t latency_us;
        void *share_info;
        uint32_t channel_enable;
    };
} audio_transmitter_ble_audio_dongle_runtime_config_t;
#endif /* AIR_BLE_AUDIO_DONGLE_ENABLE */

#if defined (AIR_ULL_AUDIO_V2_DONGLE_ENABLE)
typedef struct {
    uint8_t vol_ratio;
    uint8_t vol_ch;
    uint8_t vol_level;
    int32_t vol_gain;
} ull_audio_v2_vol_info_t;

typedef struct {
    uint8_t chx_choose;
    uint8_t ch1_connection;
    uint8_t ch2_connection;
    uint8_t ch3_connection;
    uint8_t ch4_connection;
    uint8_t ch5_connection;
    uint8_t ch6_connection;
    uint8_t ch7_connection;
    uint8_t ch8_connection;
} ull_audio_v2_connection_info_t;

typedef struct {
    uint8_t channel_num;
    uint32_t bitrate;
    uint32_t bt_clk;
} ull_audio_v2_bitrate_info_t;

typedef struct {
    union {
        int8_t dl_mixer_id;
        ull_audio_v2_vol_info_t vol_info;
        ull_audio_v2_connection_info_t connection_info;
        ull_audio_v2_bitrate_info_t bitrate_info;
    };
} audio_transmitter_ull_audio_v2_dongle_runtime_config_t;
#endif /* AIR_ULL_AUDIO_V2_DONGLE_ENABLE */

typedef struct {
    union {
#if defined (AIR_BT_ULTRA_LOW_LATENCY_ENABLE)
        audio_transmitter_gaming_mode_runtime_config_t gaming_mode_runtime_config;
#endif
#if defined (AIR_WIRED_AUDIO_ENABLE)
        audio_transmitter_wired_audio_runtime_config_t wired_audio_runtime_config;
#endif
#if defined (AIR_BLE_AUDIO_DONGLE_ENABLE)
        audio_transmitter_ble_audio_dongle_runtime_config_t ble_audio_dongle_runtime_config;
#endif /* AIR_BLE_AUDIO_DONGLE_ENABLE */
#if defined (AIR_ULL_AUDIO_V2_DONGLE_ENABLE)
        audio_transmitter_ull_audio_v2_dongle_runtime_config_t ull_audio_v2_dongle_runtime_config;
#endif /* AIR_ULL_AUDIO_V2_DONGLE_ENABLE */
        uint32_t reserved;
    };
} audio_transmitter_runtime_config_t;

typedef int8_t (*get_runtime_config_t)(uint8_t scenario_type, uint8_t scenario_sub_id, audio_transmitter_runtime_config_type_t runtime_config_type, audio_transmitter_runtime_config_t *runtime_config);
typedef struct {
    get_runtime_config_t get_runtime_config;
} audio_transmitter_runtime_config_handler_t;

typedef void (*state_change_handler_t)(uint8_t scenario_sub_id);
extern state_change_handler_t audio_transmitter_state_change_handler[AUDIO_TRANSMITTER_SCENARIO_TYPE_MAX][5];//AUDIO_TRANSMITTER_STATE_MAX = 5


#ifdef __cplusplus
}
#endif

#endif/*__AUDIO_TRANSMITTER_CONTROL_PORT_H__*/
