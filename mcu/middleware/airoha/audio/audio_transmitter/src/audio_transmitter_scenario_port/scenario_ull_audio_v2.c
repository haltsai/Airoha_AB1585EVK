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

#if defined(AIR_ULL_AUDIO_V2_DONGLE_ENABLE)

/* Includes ------------------------------------------------------------------*/
#include "scenario_ull_audio_v2.h"
#include "audio_transmitter_playback_port.h"
#include "usbaudio_drv.h"
#include "bt_sink_srv_ami.h"
#include "audio_dump.h"
#include "hal_audio_message_struct_common.h"

/* Private define ------------------------------------------------------------*/
#define ULL_AUDIO_V2_DONGLE_DEBUG_UT                    0
#define ULL_AUDIO_V2_DONGLE_DEBUG_LANTENCY              1
#define ULL_AUDIO_V2_DONGLE_DL_PATH_DEBUG_LOG           0
#define ULL_AUDIO_V2_DONGLE_UL_PATH_DEBUG_LOG           0

#define USB_RX_PORT_TOTAL                               2
#define USB_TX_PORT_TOTAL                               1

#define DONGLE_PAYLOAD_SIZE_USB_TX_PCM_MAX              192    //1ms for 48K/16bit/Stereo
#define DONGLE_USB_TX_SEND_CCNI_FRAMES                  3

#define GAIN_COMPENSATION_STEP                          10

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
#if ULL_AUDIO_V2_DONGLE_DEBUG_LANTENCY
    hal_gpio_pin_t latency_debug_gpio_pin;
    uint16_t latency_debug_enable;
    int16_t latency_debug_last_sample;
    uint16_t latency_debug_last_level;
#endif /* ULL_AUDIO_V2_DONGLE_DEBUG_LANTENCY */
    uint8_t first_time;
    uint8_t stream_is_started;
    uint32_t previous_gpt_count;
    audio_transmitter_block_header_t usb_stream_header;
    uint32_t frame_interval; // uint: us
    uint32_t frame_size;
    audio_dsp_codec_type_t usb_type;
    audio_codec_param_t usb_param;
    n9_dsp_share_info_t *p_dsp_info;
} ull_audio_v2_dongle_usb_handle_t;

typedef struct {
    uint8_t scenario_type;
    uint8_t scenario_sub_id;
} dl_mixer_param_t;

typedef struct {
    uint32_t vol_ch;
    int32_t  vol_gain;
} vol_gain_t;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static ull_audio_v2_dongle_usb_handle_t usb_stream_rx_handle[USB_RX_PORT_TOTAL];
static ull_audio_v2_dongle_usb_handle_t usb_stream_tx_handle[USB_TX_PORT_TOTAL];
static uint32_t ull_audio_v2_dongle_dl_stream_status = 0;
static uint32_t ull_audio_v2_dongle_ul_stream_status = 0;
static uint8_t all_zero_buffer[DONGLE_PAYLOAD_SIZE_USB_TX_PCM_MAX];
const static int16_t gain_compensation_table[GAIN_COMPENSATION_STEP + 1] =
{
    /*
    Ratio |    db    | Compensation
    0%    |  -60db   | 0xE890
    10%   |  -20db   | 0xF830
    20%   | -13.98db | 0xFA8B
    30%   | -10.46db | 0xFBEB
    40%   |  -7.96db | 0xFCE5
    50%   |  -6.02db | 0xFDA6
    60%   |  -4.44db | 0xFE45
    70%   |  -3.1db  | 0xFECB
    80%   |  -1.94db | 0xFF3F
    90%   |  -0.92db | 0xFFA5
    100%  |     0db  | 0
    */
    0xE890,
    0xF830,
    0xFA8B,
    0xFBEB,
    0xFCE5,
    0xFDA6,
    0xFE45,
    0xFECB,
    0xFF3F,
    0xFFA5,
    0x0
};

/* Public variables ----------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static uint32_t ull_audio_v2_codec_get_frame_size(audio_dsp_codec_type_t *codec_type, audio_codec_param_t *codec_param)
{
    uint32_t frame_size = 0;

    switch (*codec_type)
    {
        case AUDIO_DSP_CODEC_TYPE_LC3:
            frame_size = codec_param->lc3.bit_rate * codec_param->lc3.frame_interval / 8 / 1000 / 1000;
            break;

        case AUDIO_DSP_CODEC_TYPE_LC3PLUS:
            frame_size = codec_param->lc3plus.bit_rate * codec_param->lc3plus.frame_interval / 8 / 1000 / 1000;
            break;

        default:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This codec is not supported at now, %u\r\n", 1, *codec_type);
            AUDIO_ASSERT(0);
            break;
    }

    return frame_size;
}

static uint32_t usb_audio_get_frame_size(audio_dsp_codec_type_t *usb_type, audio_codec_param_t *usb_param)
{
    uint32_t frame_size = 0;
    uint32_t samples;
    uint32_t channel_num;
    uint32_t resolution_size;

    if (*usb_type == AUDIO_DSP_CODEC_TYPE_PCM)
    {
        frame_size = 1;

        switch (usb_param->pcm.frame_interval)
        {
            case 1000:
                break;

            default:
                AUDIO_ASSERT(0);
                break;
        }

        switch (usb_param->pcm.sample_rate)
        {
            case 44100:
                samples = 44;
                break;

            case 16000:
            case 32000:
            case 48000:
            case 96000:
                samples = usb_param->pcm.sample_rate/1000;
                break;

            default:
                AUDIO_ASSERT(0);
                break;
        }

        switch (usb_param->pcm.channel_mode)
        {
            case 1:
                channel_num = 1;
                break;

            case 2:
                channel_num = 2;
                break;

            case 8:
                channel_num = 8;
                break;

            default:
                AUDIO_ASSERT(0);
                break;
        }

        switch (usb_param->pcm.format)
        {
            case AFE_PCM_FORMAT_S16_LE:
                resolution_size = 2;
                break;

            case AFE_PCM_FORMAT_S24_LE:
                resolution_size = 3;
                break;

            default:
                AUDIO_ASSERT(0);
                break;
        }
    }
    else
    {
        frame_size = 0;
    }

    frame_size = frame_size * samples * channel_num * resolution_size;

    return frame_size;
}

#if ULL_AUDIO_V2_DONGLE_DEBUG_LANTENCY
static void usb_audio_rx_cb_latency_debug(ull_audio_v2_dongle_usb_handle_t *handle, uint8_t *source_buf)
{
    int16_t *start_address = NULL;
    uint32_t current_level = 0;
    uint32_t i;
    int16_t current_sample;
    int16_t next_sample;

    if (handle->latency_debug_enable)
    {
        if (handle->usb_param.pcm.format == AFE_PCM_FORMAT_S16_LE)
        {
            current_level = handle->latency_debug_last_level;
            start_address = (int16_t *)source_buf;
            if ((*start_address > handle->latency_debug_last_sample) &&
                (*start_address - handle->latency_debug_last_sample > 40000))
            {
                current_level = 1;
            }
            else if ((*start_address < handle->latency_debug_last_sample) &&
                    (handle->latency_debug_last_sample - *start_address > 40000))
            {
                current_level = 0;
            }
            for (i = 0; i < handle->frame_size/(2*handle->usb_param.pcm.channel_mode)-1; i++)
            {
                current_sample  = *((int16_t *)(source_buf+i*(2*handle->usb_param.pcm.channel_mode)));
                next_sample     = *((int16_t *)(source_buf+(i+1)*(2*handle->usb_param.pcm.channel_mode)));
                if ((current_sample > next_sample) &&
                    (current_sample - next_sample > 40000))
                {
                    current_level = 0;
                    break;
                }
                else if ((current_sample < next_sample) &&
                        (next_sample - current_sample > 40000))
                {
                    current_level = 1;
                    break;
                }
                else
                {
                }
            }
            handle->latency_debug_last_sample = *((int16_t *)(source_buf + 2 * handle->usb_param.pcm.channel_mode * (handle->frame_size/(2*handle->usb_param.pcm.channel_mode)-1)));
            if (current_level != handle->latency_debug_last_level)
            {
                hal_gpio_set_output(handle->latency_debug_gpio_pin, current_level);
                handle->latency_debug_last_level = current_level;
            }
        }
        else if (handle->usb_param.pcm.format == AFE_PCM_FORMAT_S24_LE)
        {
            current_level = handle->latency_debug_last_level;
            start_address    = (int16_t *)(source_buf+1); // drop the low 8bit
            if ((*start_address > handle->latency_debug_last_sample) &&
                (*start_address - handle->latency_debug_last_sample > 40000))
            {
                current_level = 1;
            }
            else if ((*start_address < handle->latency_debug_last_sample) &&
                    (handle->latency_debug_last_sample - *start_address > 40000))
            {
                current_level = 0;
            }
            for (i = 0; i < handle->frame_size/(3*handle->usb_param.pcm.channel_mode)-1; i++)
            {
                current_sample  = *((int16_t *)(source_buf+i*(3*handle->usb_param.pcm.channel_mode)+1)); // drop the low 8bit
                next_sample     = *((int16_t *)(source_buf+(i+1)*(3*handle->usb_param.pcm.channel_mode)+1)); // drop the low 8bit
                if ((current_sample > next_sample) &&
                    (current_sample - next_sample) > 40000)
                {
                    current_level = 0;
                    break;
                }
                else if ((current_sample < next_sample) &&
                        (next_sample - current_sample > 40000))
                {
                    current_level = 1;
                    break;
                }
                else
                {
                }
            }
            handle->latency_debug_last_sample = *((int16_t *)(source_buf + 3 * handle->usb_param.pcm.channel_mode * (handle->frame_size/(3*handle->usb_param.pcm.channel_mode)-1) + 1));
            if (current_level != handle->latency_debug_last_level)
            {
                hal_gpio_set_output(handle->latency_debug_gpio_pin, current_level);
                handle->latency_debug_last_level = current_level;
            }
        }
    }
}

static void usb_audio_tx_cb_latency_debug(ull_audio_v2_dongle_usb_handle_t *handle, uint8_t *source_buf)
{
    int16_t *start_address = source_buf;
    uint32_t current_level = 0;
    uint32_t i;
    int16_t sample_value = 0;
    uint16_t frame_samples;
    uint16_t channel_num;
    uint16_t resolution_size;

    if (handle->latency_debug_enable)
    {
        if (handle->usb_param.pcm.channel_mode == 1)
        {
            channel_num = 1;
        }
        else
        {
            channel_num = 2;
        }
        resolution_size = 2;
        frame_samples = handle->frame_size / resolution_size / channel_num;
        for (i = 0; i < frame_samples; i++) {
            sample_value += (*(start_address + i*channel_num) / frame_samples);
        }
        if (sample_value >= 5000)
        {
            current_level = 1;
        }
        else
        {
            current_level = 0;
        }
        if (current_level != handle->latency_debug_last_level) {
            hal_gpio_set_output(handle->latency_debug_gpio_pin, current_level);
            handle->latency_debug_last_level = current_level;
        }
    }
}
#endif /* ULL_AUDIO_V2_DONGLE_DEBUG_LANTENCY */

static void usb_audio_tx_trigger_dsp_flow(uint32_t gpt_count, uint32_t frame_num)
{
    hal_ccni_message_t ccni_msg;

    ccni_msg.ccni_message[0] = gpt_count;
    ccni_msg.ccni_message[1] = frame_num;
    if (hal_ccni_set_event(CCNI_CM4_TO_DSP0_BT_COMMON, &ccni_msg) != HAL_CCNI_STATUS_OK)
    {
        TRANSMITTER_LOG_E("[ULL Audio V2][UL][ERROR][usb_audio_tx_trigger_dsp_flow]send ccni fail\r\n", 0);
        AUDIO_ASSERT(0);
    }

    #if ULL_AUDIO_V2_DONGLE_UL_PATH_DEBUG_LOG
    TRANSMITTER_LOG_I("[ULL Audio V2][UL][DEBUG][usb_audio_tx_trigger_dsp_flow]send ccni", 0);
    #endif /* ULL_AUDIO_V2_DONGLE_UL_PATH_DEBUG_LOG */
}

static void usb_audio_tx_cb_ull_audio_v2_dongle_0(void)
{
    uint32_t gpt_count, duration_count;
    uint32_t data_size;
    n9_dsp_share_info_t *p_dsp_info;
    uint32_t blk_size;
    uint32_t data_size_total;
    uint8_t *p_source_buf;

    /* get current gpt count */
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &gpt_count);
    #if BLE_AUDIO_DONGLE_VOICE_PATH_DEBUG_LOG
    TRANSMITTER_LOG_I("[ULL Audio V2][UL]usb_audio_tx_cb_ble_audio_dongle_0 callback = %u", 1, gpt_count);
    #endif /* BLE_AUDIO_DONGLE_VOICE_PATH_DEBUG_LOG */

    if ((ull_audio_v2_dongle_ul_stream_status & 0x1) == 0)
    {
        /* workaround: If the stream is not started, need to send zero data to usb to avoid usb host error */
        hal_usb_write_endpoint_fifo(0x01, usb_stream_tx_handle[0].frame_size, (void*)all_zero_buffer);
        hal_usb_set_endpoint_tx_ready(1);
        return;
    }

    /* get share buffer info */
    p_dsp_info = usb_stream_tx_handle[0].p_dsp_info;

    /* check usb irq duration */
    if (usb_stream_tx_handle[0].first_time == 0)
    {
        usb_stream_tx_handle[0].previous_gpt_count = gpt_count;
        usb_stream_tx_handle[0].first_time  = 1;

        TRANSMITTER_LOG_I("[ULL Audio V2][UL][USB_TX_DEBUG 0]usb_audio_tx_cb_ble_audio_dongle_0 callback first time = %u", 1, gpt_count);
    }
    else
    {
        hal_gpt_get_duration_count(usb_stream_tx_handle[0].previous_gpt_count, gpt_count, &duration_count);
        if ((duration_count > 1500) || (duration_count < 500))
        {
            TRANSMITTER_LOG_E("[ULL Audio V2][UL][ERROR][USB_TX_DEBUG 0]usb_audio_tx_cb_ble_audio_dongle_0 duration = %d", 1, duration_count);
        }
        usb_stream_tx_handle[0].previous_gpt_count = gpt_count;
    }

    /* get data info */
    hal_audio_buf_mgm_get_data_buffer(p_dsp_info, &p_source_buf, &data_size_total);
    if (data_size_total == 0)
    {
        data_size = 0;
        p_source_buf = NULL;
    }
    else
    {
        data_size = ((audio_transmitter_block_header_t *)p_source_buf)->data_length;
        /* check data size */
        if (data_size != usb_stream_tx_handle[0].frame_size)
        {
            TRANSMITTER_LOG_E("[ULL Audio V2][UL][ERROR]usb_audio_tx_cb_ble_audio_dongle_0 data_size is not right %u, %u", 2, data_size, usb_stream_tx_handle[0].frame_size);
            AUDIO_ASSERT(0);
        }
        p_source_buf += sizeof(audio_transmitter_block_header_t);
    }

    /* check if needs to send ccni to trigger dsp flow */
    if (data_size_total == (usb_stream_tx_handle[0].frame_size+sizeof(audio_transmitter_block_header_t))*DONGLE_USB_TX_SEND_CCNI_FRAMES)
    {
        /* send ccni to trigger dsp flow if there are only 3ms data in share buffer, so dsp has 2ms to process new data */
        usb_audio_tx_trigger_dsp_flow(gpt_count, DONGLE_USB_TX_SEND_CCNI_FRAMES);
    }
    else if (data_size_total == 0)
    {
        /* send ccni to trigger dsp flow if there is no data in share buffer */
        usb_audio_tx_trigger_dsp_flow(gpt_count, 0);
    }

    /* send usb data */
    if (data_size == 0)
    {
        if (usb_stream_tx_handle[0].stream_is_started == 0)
        {
            /* the stream is not started, so send slience data */
            hal_usb_write_endpoint_fifo(0x01, usb_stream_tx_handle[0].frame_size, (void*)all_zero_buffer);
        }
        else
        {
            hal_usb_write_endpoint_fifo(0x01, usb_stream_tx_handle[0].frame_size, (void*)all_zero_buffer);
            TRANSMITTER_LOG_E("[ULL Audio V2][UL][ERROR]usb_audio_tx_cb_ble_audio_dongle_0 data is not enough", 0);
            // AUDIO_ASSERT(0);
        }
    }
    else
    {
        /* set data from share buffer into USB FIFO */
        hal_usb_write_endpoint_fifo(0x01, usb_stream_tx_handle[0].frame_size, (void*)p_source_buf);

        /* drop this packet */
        blk_size = p_dsp_info->sub_info.block_info.block_size;
        hal_audio_buf_mgm_get_read_data_done(p_dsp_info, blk_size);

        if (usb_stream_tx_handle[0].stream_is_started == 0)
        {
            /* set that the stream is started */
            usb_stream_tx_handle[0].stream_is_started = 1;
        }
    }
    hal_usb_set_endpoint_tx_ready(1);

    #if ULL_AUDIO_V2_DONGLE_DEBUG_LANTENCY
    if (data_size == 0)
    {
        usb_audio_tx_cb_latency_debug(&usb_stream_tx_handle[0], all_zero_buffer);
    }
    else
    {
        usb_audio_tx_cb_latency_debug(&usb_stream_tx_handle[0], p_source_buf);
    }
    #endif /* ULL_AUDIO_V2_DONGLE_DEBUG_LANTENCY */
}

static void usb_audio_rx_cb_ull_audio_v2_dongle_0(void)
{
    uint32_t gpt_count, duration_count;
    uint32_t available_data_size;
    n9_dsp_share_info_t *p_dsp_info;
    uint32_t buf_size;
    uint8_t *p_source_buf;

    /* get current gpt count */
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &gpt_count);
    #if ULL_AUDIO_V2_DONGLE_DL_PATH_DEBUG_LOG
    TRANSMITTER_LOG_I("[ULL Audio V2][DL][DEBUG][usb_audio_rx_cb_ull_audio_v2_dongle_0]cur gpt = %u", 1, gpt_count);
    #endif /* ULL_AUDIO_V2_DONGLE_DL_PATH_DEBUG_LOG */

    /* get share buffer info */
    p_dsp_info = usb_stream_rx_handle[0].p_dsp_info;

    /* check if is the first time */
    if (usb_stream_rx_handle[0].first_time == 0)
    {
        /* this is the first irq, we need to drop all relict USB data */
        available_data_size = USB_Audio_Get_Len_Received_Data(0);
        USB_Audio_Rx_Buffer_Drop_Bytes(0, available_data_size);
        usb_stream_rx_handle[0].first_time = 1;
        usb_stream_rx_handle[0].previous_gpt_count = gpt_count;

        TRANSMITTER_LOG_I("[ULL Audio V2][DL][usb_audio_rx_cb_ull_audio_v2_dongle_0]first time = %u", 1, gpt_count);
    }
    else
    {
        /* this is not the first irq, we need to copy usb into share buffer */
        hal_gpt_get_duration_count(usb_stream_rx_handle[0].previous_gpt_count, gpt_count, &duration_count);
        if ((duration_count > 1500) || (duration_count < 500))
        {
            TRANSMITTER_LOG_E("[ULL Audio V2][DL][ERROR][usb_audio_rx_cb_ull_audio_v2_dongle_0]duration = %d", 1, duration_count);
        }
        usb_stream_rx_handle[0].previous_gpt_count = gpt_count;

        /* get usb data size */
        available_data_size = USB_Audio_Get_Len_Received_Data(0);
        if (available_data_size > usb_stream_rx_handle[0].frame_size)
        {
            TRANSMITTER_LOG_E("[ULL Audio V2][DL][ERROR][usb_audio_rx_cb_ull_audio_v2_dongle_0]Too much data in USB buffer %d\r\n", 1, available_data_size);
        }

        /* copy usb data into share buffer block one by one */
        while (available_data_size >= usb_stream_rx_handle[0].frame_size)
        {
            /* get share buffer block info */
            hal_audio_buf_mgm_get_free_buffer(p_dsp_info, &p_source_buf, &buf_size);
            if (buf_size < (usb_stream_rx_handle[0].frame_size + BLK_HEADER_SIZE))
            {
                TRANSMITTER_LOG_I("[ULL Audio V2][DL][ERROR][usb_audio_rx_cb_ull_audio_v2_dongle_0]Not enough share buffer space, %u", 1, buf_size);
                USB_Audio_Rx_Buffer_Drop_Bytes(0, usb_stream_rx_handle[0].frame_size);
                available_data_size = USB_Audio_Get_Len_Received_Data(0);
                continue;
            }

            /* update usb data header */
            usb_stream_rx_handle[0].usb_stream_header.sequence_number++;
            usb_stream_rx_handle[0].usb_stream_header.data_length = usb_stream_rx_handle[0].frame_size;

            /* write usb data into share buffer block */
            // memcpy(p_source_buf, &usb_stream_rx_handle[0].usb_stream_header, BLK_HEADER_SIZE);
            ((audio_transmitter_block_header_t *)p_source_buf)->sequence_number = usb_stream_rx_handle[0].usb_stream_header.sequence_number;
            ((audio_transmitter_block_header_t *)p_source_buf)->data_length     = usb_stream_rx_handle[0].usb_stream_header.data_length;
            USB_Audio_Read_Data(0, p_source_buf + BLK_HEADER_SIZE, usb_stream_rx_handle[0].frame_size);
            hal_audio_buf_mgm_get_write_data_done(p_dsp_info, usb_stream_rx_handle[0].frame_size + BLK_HEADER_SIZE);

            #if ULL_AUDIO_V2_DONGLE_DL_PATH_DEBUG_LOG
            TRANSMITTER_LOG_I("[ULL Audio V2][DL][DEBUG][usb_audio_rx_cb_ull_audio_v2_dongle_0]r_offset = %u, w_offset = %u", 2, p_dsp_info->read_offset, p_dsp_info->write_offset);
            #endif /* ULL_AUDIO_V2_DONGLE_DL_PATH_DEBUG_LOG */

            /* audio dump */
            // LOG_AUDIO_DUMP(p_source_buf + BLK_HEADER_SIZE, usb_stream_rx_handle[0].frame_size, SOURCE_IN4);

            /* get residual usb data size */
            available_data_size = USB_Audio_Get_Len_Received_Data(0);

            #if ULL_AUDIO_V2_DONGLE_DEBUG_LANTENCY
            usb_audio_rx_cb_latency_debug(&usb_stream_rx_handle[0], (p_source_buf + BLK_HEADER_SIZE));
            #endif /* ULL_AUDIO_V2_DONGLE_DEBUG_LANTENCY */
        }
    }
}

static void usb_audio_rx_cb_ull_audio_v2_dongle_1(void)
{
    uint32_t gpt_count, duration_count;
    uint32_t available_data_size;
    n9_dsp_share_info_t *p_dsp_info;
    uint32_t buf_size;
    uint8_t *p_source_buf;

    /* get current gpt count */
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &gpt_count);
    #if ULL_AUDIO_V2_DONGLE_DL_PATH_DEBUG_LOG
    TRANSMITTER_LOG_I("[ULL Audio V2][DL][DEBUG][usb_audio_rx_cb_ull_audio_v2_dongle_1]cur gpt = %u", 1, gpt_count);
    #endif /* ULL_AUDIO_V2_DONGLE_DL_PATH_DEBUG_LOG */

    /* get share buffer info */
    p_dsp_info = usb_stream_rx_handle[1].p_dsp_info;

    /* check if is the first time */
    if (usb_stream_rx_handle[1].first_time == 0)
    {
        /* this is the first irq, we need to drop all relict USB data */
        available_data_size = USB_Audio_Get_Len_Received_Data(1);
        USB_Audio_Rx_Buffer_Drop_Bytes(1, available_data_size);
        usb_stream_rx_handle[1].first_time = 1;
        usb_stream_rx_handle[1].previous_gpt_count = gpt_count;

        TRANSMITTER_LOG_I("[ULL Audio V2][DL][usb_audio_rx_cb_ull_audio_v2_dongle_1]first time = %u", 1, gpt_count);
    }
    else
    {
        /* this is not the first irq, we need to copy usb into share buffer */
        hal_gpt_get_duration_count(usb_stream_rx_handle[1].previous_gpt_count, gpt_count, &duration_count);
        if ((duration_count > 1500) || (duration_count < 500))
        {
            TRANSMITTER_LOG_E("[ULL Audio V2][DL][ERROR][usb_audio_rx_cb_ull_audio_v2_dongle_1]duration = %d", 1, duration_count);
        }
        usb_stream_rx_handle[1].previous_gpt_count = gpt_count;

        /* get usb data size */
        available_data_size = USB_Audio_Get_Len_Received_Data(1);
        if (available_data_size > usb_stream_rx_handle[1].frame_size)
        {
            TRANSMITTER_LOG_E("[ULL Audio V2][DL][ERROR][usb_audio_rx_cb_ull_audio_v2_dongle_1]Too much data in USB buffer %d\r\n", 1, available_data_size);
        }

        /* copy usb data into share buffer block one by one */
        while (available_data_size >= usb_stream_rx_handle[1].frame_size)
        {
            /* get share buffer block info */
            hal_audio_buf_mgm_get_free_buffer(p_dsp_info, &p_source_buf, &buf_size);
            if (buf_size < (usb_stream_rx_handle[1].frame_size + BLK_HEADER_SIZE))
            {
                TRANSMITTER_LOG_I("[ULL Audio V2][DL][ERROR][usb_audio_rx_cb_ull_audio_v2_dongle_1]Not enough share buffer space, %u", 1, buf_size);
                USB_Audio_Rx_Buffer_Drop_Bytes(1, usb_stream_rx_handle[1].frame_size);
                available_data_size = USB_Audio_Get_Len_Received_Data(1);
                continue;
            }

            /* update usb data header */
            usb_stream_rx_handle[1].usb_stream_header.sequence_number++;
            usb_stream_rx_handle[1].usb_stream_header.data_length = usb_stream_rx_handle[1].frame_size;

            /* write usb data into share buffer block */
            // memcpy(p_source_buf, &usb_stream_rx_handle[1].usb_stream_header, BLK_HEADER_SIZE);
            ((audio_transmitter_block_header_t *)p_source_buf)->sequence_number = usb_stream_rx_handle[1].usb_stream_header.sequence_number;
            ((audio_transmitter_block_header_t *)p_source_buf)->data_length     = usb_stream_rx_handle[1].usb_stream_header.data_length;
            USB_Audio_Read_Data(1, p_source_buf + BLK_HEADER_SIZE, usb_stream_rx_handle[1].frame_size);
            hal_audio_buf_mgm_get_write_data_done(p_dsp_info, usb_stream_rx_handle[1].frame_size + BLK_HEADER_SIZE);

            #if ULL_AUDIO_V2_DONGLE_DL_PATH_DEBUG_LOG
            TRANSMITTER_LOG_I("[ULL Audio V2][DL][DEBUG][usb_audio_rx_cb_ull_audio_v2_dongle_1]r_offset = %u, w_offset = %u", 2, p_dsp_info->read_offset, p_dsp_info->write_offset);
            #endif /* ULL_AUDIO_V2_DONGLE_DL_PATH_DEBUG_LOG */

            /* audio dump */
            //LOG_AUDIO_DUMP(p_source_buf + BLK_HEADER_SIZE, usb_stream_rx_handle[1].frame_size, 13);

            /* get residual usb data size */
            available_data_size = USB_Audio_Get_Len_Received_Data(1);

            #if ULL_AUDIO_V2_DONGLE_DEBUG_LANTENCY
            usb_audio_rx_cb_latency_debug(&usb_stream_rx_handle[1], (p_source_buf + BLK_HEADER_SIZE));
            #endif /* ULL_AUDIO_V2_DONGLE_DEBUG_LANTENCY */
        }
    }
}

/* Public functions ----------------------------------------------------------*/
#if ULL_AUDIO_V2_DONGLE_DEBUG_UT
void ull_audio_v2_dongle_ut_music_callback(audio_transmitter_event_t event, void *data, void *user_data)
{
    switch (event)
    {
        case AUDIO_TRANSMITTER_EVENT_START_SUCCESS:
            TRANSMITTER_LOG_I("[ull_audio_v2_dongle_ut_music_callback]: start successfully.", 0);
            break;

        case AUDIO_TRANSMITTER_EVENT_STOP_SUCCESS:
            TRANSMITTER_LOG_I("[ull_audio_v2_dongle_ut_music_callback]: stop successfully.", 0);
            break;

        default:
            TRANSMITTER_LOG_I("[ull_audio_v2_dongle_ut_music_callback]: error event = %d.", 1, event);
            break;
    }
}

void ull_audio_v2_dongle_ut_gpt_callback(void *user_data)
{
    hal_ccni_message_t msg;
    uint32_t gpt_count;
    uint32_t gpt_count0;
    uint32_t duration_count;
    n9_dsp_share_info_t *p_avm_share_buf_info0;
    n9_dsp_share_info_t *p_avm_share_buf_info1;
    uint8_t *src_buf;
    uint8_t *dst_buf;
    uint32_t write_index = 0;
    uint32_t write_index_0 = 0;
    uint32_t write_index_1 = 0;
    static uint32_t count = 0;

    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &gpt_count);

    p_avm_share_buf_info0 = hal_audio_query_audio_transmitter_share_info(AUDIO_TRANSMITTER_SHARE_INFO_INDEX_ULL_AUDIO_V2_DONGLE_BT_SEND_TO_AIR_0);
    write_index = p_avm_share_buf_info0->write_offset / p_avm_share_buf_info0->sub_info.block_info.block_size;

    /* prepare message */
    msg.ccni_message[0] = (gpt_count*2/625)&0x0fffffff;
    msg.ccni_message[1] = write_index;

    /* send ccni messge act as the bt controller */
    hal_ccni_set_event(CCNI_CM4_TO_DSP0_AUDIO_TRANSMITTER, &msg);

    /* loopback packet: copy DL packet from DL packet share buffer into UL packet share buffer */
    if ((ull_audio_v2_dongle_ul_stream_status & 0x1) != 0)
    {
        p_avm_share_buf_info0 = (n9_dsp_share_info_t *)hal_audio_query_audio_transmitter_share_info(AUDIO_TRANSMITTER_SHARE_INFO_INDEX_ULL_AUDIO_V2_DONGLE_BT_SEND_TO_AIR_0);
        p_avm_share_buf_info1 = (n9_dsp_share_info_t *)hal_audio_query_audio_transmitter_share_info(AUDIO_TRANSMITTER_SHARE_INFO_INDEX_ULL_AUDIO_V2_DONGLE_BT_RECEIVE_FROM_AIR_0);
        if (p_avm_share_buf_info0->read_offset != p_avm_share_buf_info0->write_offset)
        {
            if (count == 0)
            {
                count = (gpt_count*2/625+32)&0x0fffffff;
            }
            src_buf = p_avm_share_buf_info0->read_offset  + p_avm_share_buf_info0->start_addr;
            dst_buf = p_avm_share_buf_info1->write_offset + p_avm_share_buf_info1->start_addr;
            write_index_0 = p_avm_share_buf_info1->write_offset / p_avm_share_buf_info1->sub_info.block_info.block_size;
            memcpy(dst_buf, src_buf, p_avm_share_buf_info0->sub_info.block_info.block_size);
            ((ULL_AUDIO_V2_HEADER *)dst_buf)->TimeStamp = (count)&0x0fffffff;
            ((ULL_AUDIO_V2_HEADER *)dst_buf)->_reserved_byte_0Dh = 1;
            p_avm_share_buf_info0->read_offset  = (p_avm_share_buf_info0->read_offset+p_avm_share_buf_info0->sub_info.block_info.block_size) % (p_avm_share_buf_info0->sub_info.block_info.block_size*p_avm_share_buf_info0->sub_info.block_info.block_num);
            p_avm_share_buf_info1->write_offset = (p_avm_share_buf_info1->write_offset+p_avm_share_buf_info1->sub_info.block_info.block_size)% (p_avm_share_buf_info1->sub_info.block_info.block_size*p_avm_share_buf_info1->sub_info.block_info.block_num);
        }

        p_avm_share_buf_info0 = (n9_dsp_share_info_t *)hal_audio_query_audio_transmitter_share_info(AUDIO_TRANSMITTER_SHARE_INFO_INDEX_ULL_AUDIO_V2_DONGLE_BT_SEND_TO_AIR_1);
        p_avm_share_buf_info1 = (n9_dsp_share_info_t *)hal_audio_query_audio_transmitter_share_info(AUDIO_TRANSMITTER_SHARE_INFO_INDEX_ULL_AUDIO_V2_DONGLE_BT_RECEIVE_FROM_AIR_1);
        if (p_avm_share_buf_info0->read_offset != p_avm_share_buf_info0->write_offset)
        {
            src_buf = p_avm_share_buf_info0->read_offset  + p_avm_share_buf_info0->start_addr;
            dst_buf = p_avm_share_buf_info1->write_offset + p_avm_share_buf_info1->start_addr;
            write_index_1 = p_avm_share_buf_info1->write_offset / p_avm_share_buf_info1->sub_info.block_info.block_size;
            memcpy(dst_buf, src_buf, p_avm_share_buf_info0->sub_info.block_info.block_size);
            ((ULL_AUDIO_V2_HEADER *)dst_buf)->TimeStamp = (count)&0x0fffffff;
            ((ULL_AUDIO_V2_HEADER *)dst_buf)->_reserved_byte_0Dh = 1;
            p_avm_share_buf_info0->read_offset  = (p_avm_share_buf_info0->read_offset+p_avm_share_buf_info0->sub_info.block_info.block_size) % (p_avm_share_buf_info0->sub_info.block_info.block_size*p_avm_share_buf_info0->sub_info.block_info.block_num);
            p_avm_share_buf_info1->write_offset = (p_avm_share_buf_info1->write_offset+p_avm_share_buf_info1->sub_info.block_info.block_size)% (p_avm_share_buf_info1->sub_info.block_info.block_size*p_avm_share_buf_info1->sub_info.block_info.block_num);

            count = count + 16;
        }
    }

    TRANSMITTER_LOG_I("[ULL Audio V2][DL][DEBUG][ull_audio_v2_dongle_ut_gpt_callback]: 0x%x, 0x%x, %u, 0x%x, %u, %u.", 6, gpt_count, (gpt_count*2/625+32)&0x0fffffff, write_index, count-16, write_index_0, write_index_1);
}
#endif /* ULL_AUDIO_V2_DONGLE_DEBUG_UT */

#if ULL_AUDIO_V2_DONGLE_DEBUG_LANTENCY
void ull_audio_v2_dongle_rx_latency_debug_control(uint32_t port, bool enable, uint32_t gpio_num)
{
    uint32_t saved_mask;

    hal_nvic_save_and_set_interrupt_mask(&saved_mask);

    if (enable) {
        usb_stream_rx_handle[port].latency_debug_enable = 1;
        usb_stream_rx_handle[port].latency_debug_last_level = 0;
        usb_stream_rx_handle[port].latency_debug_last_sample = 0;
        usb_stream_rx_handle[port].latency_debug_gpio_pin = gpio_num;
    } else {
        usb_stream_rx_handle[port].latency_debug_enable = 0;
        usb_stream_rx_handle[port].latency_debug_last_level = 0;
        usb_stream_rx_handle[port].latency_debug_last_sample = 0;
        usb_stream_rx_handle[port].latency_debug_gpio_pin = gpio_num;
    }

    hal_nvic_restore_interrupt_mask(saved_mask);

    hal_gpio_set_output(usb_stream_rx_handle[port].latency_debug_gpio_pin, HAL_GPIO_DATA_LOW);
}

void ull_audio_v2_dongle_tx_latency_debug_control(uint32_t port, bool enable, uint32_t gpio_num)
{
    uint32_t saved_mask;

    hal_nvic_save_and_set_interrupt_mask(&saved_mask);

    if (enable) {
        usb_stream_tx_handle[port].latency_debug_enable = 1;
        usb_stream_tx_handle[port].latency_debug_last_level = 0;
        usb_stream_tx_handle[port].latency_debug_last_sample = 0;
        usb_stream_tx_handle[port].latency_debug_gpio_pin = gpio_num;
    } else {
        usb_stream_tx_handle[port].latency_debug_enable = 0;
        usb_stream_tx_handle[port].latency_debug_last_level = 0;
        usb_stream_tx_handle[port].latency_debug_last_sample = 0;
        usb_stream_tx_handle[port].latency_debug_gpio_pin = gpio_num;
    }

    hal_nvic_restore_interrupt_mask(saved_mask);

    hal_gpio_set_output(usb_stream_tx_handle[port].latency_debug_gpio_pin, HAL_GPIO_DATA_LOW);
}
#endif /* ULL_AUDIO_V2_DONGLE_DEBUG_LANTENCY */

void ull_audio_v2_dongle_open_playback(audio_transmitter_config_t *config, mcu2dsp_open_param_t *open_param)
{
    uint32_t payload_size = 0;
    uint32_t i;

    switch (config->scenario_sub_id)
    {
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0:
            /* configure stream source */
            /* prepare bt-out parameters to dsp */
            if ((config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_num == 0) ||
                (config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_num > ULL_AUDIO_V2_DATA_CHANNEL_NUMBER))
            {
                TRANSMITTER_LOG_E("[ULL Audio V2][UL][ERROR]Error ul link num %d\r\n", 1, config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_num);
                AUDIO_ASSERT(0);
            }
            open_param->param.stream_in                                         = STREAM_IN_BT_COMMON;
            open_param->stream_in_param.bt_common.scenario_type                 = config->scenario_type;
            open_param->stream_in_param.bt_common.scenario_sub_id               = config->scenario_sub_id;
            open_param->stream_in_param.bt_common.share_info_type               = SHARE_BUFFER_INFO_TYPE;
            open_param->stream_in_param.bt_common.p_share_info                  = hal_audio_query_audio_transmitter_share_info(AUDIO_TRANSMITTER_SHARE_INFO_INDEX_ULL_AUDIO_V2_DONGLE_BT_RECEIVE_FROM_AIR_0);
            open_param->stream_in_param.bt_common.data_notification_frequency   = 0;
            for (i = 0; i < config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_num; i++)
            {
                if (config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_param[i].enable)
                {
                    payload_size = ull_audio_v2_codec_get_frame_size(  &(config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_param[i].codec_type),
                                                                    &(config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_param[i].codec_param));
                    if ((payload_size == 0) || (payload_size != config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_param[i].codec_param.lc3plus.frame_size))
                    {
                        TRANSMITTER_LOG_E("[ULL Audio V2][UL][ERROR]Error codec frame size %d, %d\r\n", 2, payload_size, config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_param[i].codec_param.lc3plus.frame_size);
                        AUDIO_ASSERT(0);
                    }
                    if (open_param->stream_in_param.bt_common.max_payload_size < (payload_size+sizeof(ULL_AUDIO_V2_HEADER)))
                    {
                        open_param->stream_in_param.bt_common.max_payload_size = payload_size+sizeof(ULL_AUDIO_V2_HEADER);
                    }
                    if ((((n9_dsp_share_info_t *)(config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_param[i].share_info))->sub_info.block_info.block_size) != ((payload_size+sizeof(ULL_AUDIO_V2_HEADER)+3)/4*4))
                    {
                        TRANSMITTER_LOG_E("[ULL Audio V2][UL][ERROR]Error packet size %d, %d\r\n", 2, payload_size, (((n9_dsp_share_info_t *)(config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_param[i].share_info))->sub_info.block_info.block_size));
                        AUDIO_ASSERT(0);
                    }
                    open_param->stream_in_param.bt_common.scenario_param.ull_audio_v2_dongle_param.bt_link_info[i].share_info                          = config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_param[i].share_info;
                    open_param->stream_in_param.bt_common.scenario_param.ull_audio_v2_dongle_param.bt_link_info[i].codec_type                          = config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_param[i].codec_type;
                    if (config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_param[i].codec_type == AUDIO_DSP_CODEC_TYPE_LC3PLUS)
                    {
                        open_param->stream_in_param.bt_common.scenario_param.ull_audio_v2_dongle_param.bt_link_info[i].codec_param.lc3plus.sample_rate     = config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_param[i].codec_param.lc3plus.sample_rate;
                        open_param->stream_in_param.bt_common.scenario_param.ull_audio_v2_dongle_param.bt_link_info[i].codec_param.lc3plus.sample_format   = config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_param[i].codec_param.lc3plus.sample_format;
                        open_param->stream_in_param.bt_common.scenario_param.ull_audio_v2_dongle_param.bt_link_info[i].codec_param.lc3plus.channel_mode    = config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_param[i].codec_param.lc3plus.channel_mode;
                        open_param->stream_in_param.bt_common.scenario_param.ull_audio_v2_dongle_param.bt_link_info[i].codec_param.lc3plus.frame_interval  = config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_param[i].codec_param.lc3plus.frame_interval;
                        open_param->stream_in_param.bt_common.scenario_param.ull_audio_v2_dongle_param.bt_link_info[i].codec_param.lc3plus.frame_size      = config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_param[i].codec_param.lc3plus.frame_size;
                        open_param->stream_in_param.bt_common.scenario_param.ull_audio_v2_dongle_param.bt_link_info[i].codec_param.lc3plus.bit_rate        = config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_param[i].codec_param.lc3plus.bit_rate;
                        TRANSMITTER_LOG_I("[ULL Audio V2][UL]BT link codec setting ch%u: %u, %u, %u, %u, %u, %u, %u, %u, 0x%x, 0x%x\r\n", 11,
                                            i+1,
                                            config->scenario_sub_id,
                                            config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_param[i].codec_type,
                                            config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_param[i].codec_param.lc3plus.sample_rate,
                                            config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_param[i].codec_param.lc3plus.sample_format,
                                            config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_param[i].codec_param.lc3plus.channel_mode,
                                            config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_param[i].codec_param.lc3plus.frame_interval,
                                            config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_param[i].codec_param.lc3plus.frame_size,
                                            config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_param[i].codec_param.lc3plus.bit_rate,
                                            config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_param[i].share_info,
                                            ((n9_dsp_share_info_t *)(config->scenario_config.ull_audio_v2_dongle_config.ul_config.source_param.bt_in_param.link_param[i].share_info))->start_addr);
                    }
                }
                else
                {
                    open_param->stream_in_param.bt_common.scenario_param.ull_audio_v2_dongle_param.bt_link_info[i].share_info = NULL;
                }
            }
            /* prepare default gain settings */
            open_param->stream_in_param.bt_common.scenario_param.ull_audio_v2_dongle_param.gain_default_L = -14400; // -144.00dB
            open_param->stream_in_param.bt_common.scenario_param.ull_audio_v2_dongle_param.gain_default_R = -14400; // -144.00dB
            // open_param->stream_in_param.bt_common.scenario_param.ull_audio_v2_dongle_param.gain_default_L = 0; // 0dB
            // open_param->stream_in_param.bt_common.scenario_param.ull_audio_v2_dongle_param.gain_default_R = 0; // 0dB

            /* configure stream sink */
            /* get usb frame size */
            payload_size = usb_audio_get_frame_size(&(config->scenario_config.ull_audio_v2_dongle_config.ul_config.sink_param.usb_out_param.codec_type),
                                                    &(config->scenario_config.ull_audio_v2_dongle_config.ul_config.sink_param.usb_out_param.codec_param));
            if (payload_size == 0)
            {
                TRANSMITTER_LOG_E("[ULL Audio V2][UL][ERROR]Error usb frame size %d\r\n", 1, payload_size);
                AUDIO_ASSERT(0);
            }
            /* prepare usb-out paramters to USB Audio callback */
            usb_stream_tx_handle[config->scenario_sub_id-AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0].frame_size                     = payload_size;
            usb_stream_tx_handle[config->scenario_sub_id-AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0].frame_interval                 = config->scenario_config.ull_audio_v2_dongle_config.ul_config.sink_param.usb_out_param.codec_param.pcm.frame_interval;
            usb_stream_tx_handle[config->scenario_sub_id-AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0].usb_type                       = config->scenario_config.ull_audio_v2_dongle_config.ul_config.sink_param.usb_out_param.codec_type;
            usb_stream_tx_handle[config->scenario_sub_id-AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0].usb_param.pcm.sample_rate      = config->scenario_config.ull_audio_v2_dongle_config.ul_config.sink_param.usb_out_param.codec_param.pcm.sample_rate;
            usb_stream_tx_handle[config->scenario_sub_id-AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0].usb_param.pcm.channel_mode     = config->scenario_config.ull_audio_v2_dongle_config.ul_config.sink_param.usb_out_param.codec_param.pcm.channel_mode;
            usb_stream_tx_handle[config->scenario_sub_id-AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0].usb_param.pcm.format           = config->scenario_config.ull_audio_v2_dongle_config.ul_config.sink_param.usb_out_param.codec_param.pcm.format;
            usb_stream_tx_handle[config->scenario_sub_id-AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0].p_dsp_info                     = hal_audio_query_audio_transmitter_share_info(AUDIO_TRANSMITTER_SHARE_INFO_INDEX_ULL_AUDIO_V2_DONGLE_DSP_SEND_TO_MCU_0);
            /* prepare usb-out parameters to dsp */
            open_param->param.stream_out                                                                                = STREAM_OUT_AUDIO_TRANSMITTER;
            open_param->stream_out_param.data_ul.scenario_type                                                          = config->scenario_type;
            open_param->stream_out_param.data_ul.scenario_sub_id                                                        = config->scenario_sub_id;
            open_param->stream_out_param.data_ul.max_payload_size                                                       = payload_size;
            open_param->stream_out_param.data_ul.data_notification_frequency                                            = 0;
            open_param->stream_out_param.data_ul.scenario_param.ull_audio_v2_dongle_param.codec_type                    = config->scenario_config.ull_audio_v2_dongle_config.ul_config.sink_param.usb_out_param.codec_type;
            open_param->stream_out_param.data_ul.scenario_param.ull_audio_v2_dongle_param.codec_param.pcm.sample_rate   = config->scenario_config.ull_audio_v2_dongle_config.ul_config.sink_param.usb_out_param.codec_param.pcm.sample_rate;
            open_param->stream_out_param.data_ul.scenario_param.ull_audio_v2_dongle_param.codec_param.pcm.channel_mode  = config->scenario_config.ull_audio_v2_dongle_config.ul_config.sink_param.usb_out_param.codec_param.pcm.channel_mode;
            open_param->stream_out_param.data_ul.scenario_param.ull_audio_v2_dongle_param.codec_param.pcm.format        = config->scenario_config.ull_audio_v2_dongle_config.ul_config.sink_param.usb_out_param.codec_param.pcm.format;
            open_param->stream_out_param.data_ul.scenario_param.ull_audio_v2_dongle_param.codec_param.pcm.frame_interval= config->scenario_config.ull_audio_v2_dongle_config.ul_config.sink_param.usb_out_param.codec_param.pcm.frame_interval;
            open_param->stream_out_param.data_ul.p_share_info                                                           = hal_audio_query_audio_transmitter_share_info(AUDIO_TRANSMITTER_SHARE_INFO_INDEX_ULL_AUDIO_V2_DONGLE_DSP_SEND_TO_MCU_0);
            open_param->stream_out_param.data_ul.p_share_info->read_offset                                              = 0;
            open_param->stream_out_param.data_ul.p_share_info->write_offset                                             = 0;
            open_param->stream_out_param.data_ul.p_share_info->bBufferIsFull                                            = false;
            audio_transmitter_modify_share_info_by_block(open_param->stream_out_param.data_ul.p_share_info, payload_size);
            TRANSMITTER_LOG_I("[ULL Audio V2][UL]USB setting: %u, %u, %u, %u, %u, %u, %u, %d, %d, 0x%x, 0x%x\r\n", 11,
                                config->scenario_sub_id,
                                config->scenario_config.ull_audio_v2_dongle_config.ul_config.sink_param.usb_out_param.codec_type,
                                config->scenario_config.ull_audio_v2_dongle_config.ul_config.sink_param.usb_out_param.codec_param.pcm.sample_rate,
                                config->scenario_config.ull_audio_v2_dongle_config.ul_config.sink_param.usb_out_param.codec_param.pcm.channel_mode,
                                config->scenario_config.ull_audio_v2_dongle_config.ul_config.sink_param.usb_out_param.codec_param.pcm.format,
                                config->scenario_config.ull_audio_v2_dongle_config.ul_config.sink_param.usb_out_param.codec_param.pcm.frame_interval,
                                payload_size,
                                open_param->stream_in_param.bt_common.scenario_param.ull_audio_v2_dongle_param.gain_default_L,
                                open_param->stream_in_param.bt_common.scenario_param.ull_audio_v2_dongle_param.gain_default_R,
                                open_param->stream_out_param.data_ul.p_share_info,
                                open_param->stream_out_param.data_ul.p_share_info->start_addr);

            /* update state machine */
            ull_audio_v2_dongle_ul_stream_status |= 0x1<<(config->scenario_sub_id-AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_LINE_OUT:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, config->scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_I2S_MST_OUT_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, config->scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_I2S_SLV_OUT_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, config->scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0:
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_1:
            /* configure stream source */
            /* get usb frame size */
            payload_size = usb_audio_get_frame_size(&(config->scenario_config.ull_audio_v2_dongle_config.dl_config.source_param.usb_in_param.codec_type),
                                                    &(config->scenario_config.ull_audio_v2_dongle_config.dl_config.source_param.usb_in_param.codec_param));
            if (payload_size == 0)
            {
                TRANSMITTER_LOG_E("[ULL Audio V2][DL][ERROR]Error usb frame size %d\r\n", 1, payload_size);
                AUDIO_ASSERT(0);
            }
            /* prepare usb-in paramters to USB Audio callback */
            usb_stream_rx_handle[config->scenario_sub_id-AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0].frame_size                  = payload_size;
            usb_stream_rx_handle[config->scenario_sub_id-AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0].frame_interval              = config->scenario_config.ull_audio_v2_dongle_config.dl_config.source_param.usb_in_param.codec_param.pcm.frame_interval;
            usb_stream_rx_handle[config->scenario_sub_id-AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0].usb_type                    = config->scenario_config.ull_audio_v2_dongle_config.dl_config.source_param.usb_in_param.codec_type;
            usb_stream_rx_handle[config->scenario_sub_id-AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0].usb_param.pcm.sample_rate   = config->scenario_config.ull_audio_v2_dongle_config.dl_config.source_param.usb_in_param.codec_param.pcm.sample_rate;
            usb_stream_rx_handle[config->scenario_sub_id-AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0].usb_param.pcm.channel_mode  = config->scenario_config.ull_audio_v2_dongle_config.dl_config.source_param.usb_in_param.codec_param.pcm.channel_mode;
            usb_stream_rx_handle[config->scenario_sub_id-AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0].usb_param.pcm.format        = config->scenario_config.ull_audio_v2_dongle_config.dl_config.source_param.usb_in_param.codec_param.pcm.format;
            if (config->scenario_sub_id == AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0)
            {
                usb_stream_rx_handle[config->scenario_sub_id-AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0].p_dsp_info = hal_audio_query_audio_transmitter_share_info(AUDIO_TRANSMITTER_SHARE_INFO_INDEX_ULL_AUDIO_V2_DONGLE_DSP_RECEIVE_FROM_MCU_0);
            }
            else
            {
                usb_stream_rx_handle[config->scenario_sub_id-AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0].p_dsp_info = hal_audio_query_audio_transmitter_share_info(AUDIO_TRANSMITTER_SHARE_INFO_INDEX_ULL_AUDIO_V2_DONGLE_DSP_RECEIVE_FROM_MCU_1);
            }
            /* prepare usb-in parameters to dsp */
            open_param->param.stream_in                                                                                 = STREAM_IN_AUDIO_TRANSMITTER;
            open_param->stream_in_param.data_dl.scenario_type                                                           = config->scenario_type;
            open_param->stream_in_param.data_dl.scenario_sub_id                                                         = config->scenario_sub_id;
            open_param->stream_in_param.data_dl.data_notification_frequency                                             = 0;
            open_param->stream_in_param.data_dl.max_payload_size                                                        = payload_size;
            open_param->stream_in_param.data_dl.scenario_param.ull_audio_v2_dongle_param.codec_type                     = config->scenario_config.ull_audio_v2_dongle_config.dl_config.source_param.usb_in_param.codec_type;
            open_param->stream_in_param.data_dl.scenario_param.ull_audio_v2_dongle_param.codec_param.pcm.sample_rate    = config->scenario_config.ull_audio_v2_dongle_config.dl_config.source_param.usb_in_param.codec_param.pcm.sample_rate;
            open_param->stream_in_param.data_dl.scenario_param.ull_audio_v2_dongle_param.codec_param.pcm.channel_mode   = config->scenario_config.ull_audio_v2_dongle_config.dl_config.source_param.usb_in_param.codec_param.pcm.channel_mode;
            open_param->stream_in_param.data_dl.scenario_param.ull_audio_v2_dongle_param.codec_param.pcm.format         = config->scenario_config.ull_audio_v2_dongle_config.dl_config.source_param.usb_in_param.codec_param.pcm.format;
            open_param->stream_in_param.data_dl.scenario_param.ull_audio_v2_dongle_param.codec_param.pcm.frame_interval = config->scenario_config.ull_audio_v2_dongle_config.dl_config.source_param.usb_in_param.codec_param.pcm.frame_interval;
            if (config->scenario_sub_id == AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0)
            {
                open_param->stream_in_param.data_dl.p_share_info = hal_audio_query_audio_transmitter_share_info(AUDIO_TRANSMITTER_SHARE_INFO_INDEX_ULL_AUDIO_V2_DONGLE_DSP_RECEIVE_FROM_MCU_0);
            }
            else
            {
                open_param->stream_in_param.data_dl.p_share_info = hal_audio_query_audio_transmitter_share_info(AUDIO_TRANSMITTER_SHARE_INFO_INDEX_ULL_AUDIO_V2_DONGLE_DSP_RECEIVE_FROM_MCU_1);
            }
            open_param->stream_in_param.data_dl.p_share_info->read_offset = 0;
            open_param->stream_in_param.data_dl.p_share_info->write_offset = 0;
            open_param->stream_in_param.data_dl.p_share_info->bBufferIsFull = false;
            audio_transmitter_modify_share_info_by_block(open_param->stream_in_param.data_dl.p_share_info, payload_size);
            /* prepare default gain settings */
            open_param->stream_in_param.data_dl.scenario_param.ull_audio_v2_dongle_param.gain_default_L = -14400; // -144.00dB
            open_param->stream_in_param.data_dl.scenario_param.ull_audio_v2_dongle_param.gain_default_R = -14400; // -144.00dB
            // open_param->stream_in_param.data_dl.scenario_param.ull_audio_v2_dongle_param.gain_default_L = 0; // 0.00dB
            // open_param->stream_in_param.data_dl.scenario_param.ull_audio_v2_dongle_param.gain_default_R = 0; // 0.00dB
            TRANSMITTER_LOG_I("[ULL Audio V2][DL]USB setting: %u, %u, %u, %u, %u, %u, %u, %d, %d, 0x%x, 0x%x\r\n", 11,
                                config->scenario_sub_id,
                                config->scenario_config.ull_audio_v2_dongle_config.dl_config.source_param.usb_in_param.codec_type,
                                config->scenario_config.ull_audio_v2_dongle_config.dl_config.source_param.usb_in_param.codec_param.pcm.sample_rate,
                                config->scenario_config.ull_audio_v2_dongle_config.dl_config.source_param.usb_in_param.codec_param.pcm.channel_mode,
                                config->scenario_config.ull_audio_v2_dongle_config.dl_config.source_param.usb_in_param.codec_param.pcm.format,
                                config->scenario_config.ull_audio_v2_dongle_config.dl_config.source_param.usb_in_param.codec_param.pcm.frame_interval,
                                payload_size,
                                open_param->stream_in_param.data_dl.scenario_param.ull_audio_v2_dongle_param.gain_default_L,
                                open_param->stream_in_param.data_dl.scenario_param.ull_audio_v2_dongle_param.gain_default_R,
                                open_param->stream_in_param.data_dl.p_share_info,
                                open_param->stream_in_param.data_dl.p_share_info->start_addr);

            /* configure stream sink */
            /* prepare bt-out parameters to dsp */
            if ((config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_num == 0) ||
                (config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_num > ULL_AUDIO_V2_DATA_CHANNEL_NUMBER))
            {
                TRANSMITTER_LOG_E("[ULL Audio V2][DL][ERROR]Error dl link num %d\r\n", 1, config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_num);
                AUDIO_ASSERT(0);
            }
            open_param->param.stream_out                            = STREAM_OUT_BT_COMMON;
            open_param->stream_out_param.bt_common.scenario_type    = config->scenario_type;
            open_param->stream_out_param.bt_common.scenario_sub_id  = config->scenario_sub_id;
            open_param->stream_out_param.bt_common.share_info_type  = SHARE_BUFFER_INFO_TYPE;
            open_param->stream_out_param.bt_common.p_share_info     = hal_audio_query_audio_transmitter_share_info(AUDIO_TRANSMITTER_SHARE_INFO_INDEX_ULL_AUDIO_V2_DONGLE_BT_SEND_TO_AIR_0);
            open_param->stream_out_param.bt_common.data_notification_frequency = 0;
            for (i = 0; i < config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_num; i++)
            {
                if (config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_param[i].enable)
                {
                    payload_size = ull_audio_v2_codec_get_frame_size(  &(config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_param[i].codec_type),
                                                                    &(config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_param[i].codec_param));
                    if ((payload_size == 0) || (payload_size != config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_param[i].codec_param.lc3plus.frame_size))
                    {
                        TRANSMITTER_LOG_E("[ULL Audio V2][DL][ERROR]Error codec frame size %d, %d\r\n", 2, payload_size, config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_param[i].codec_param.lc3plus.frame_size);
                        AUDIO_ASSERT(0);
                    }
                    if (open_param->stream_out_param.bt_common.max_payload_size < (payload_size+sizeof(ULL_AUDIO_V2_HEADER)))
                    {
                        open_param->stream_out_param.bt_common.max_payload_size = payload_size+sizeof(ULL_AUDIO_V2_HEADER);
                    }
                    if ((((n9_dsp_share_info_t *)(config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_param[i].share_info))->sub_info.block_info.block_size) != ((payload_size+sizeof(ULL_AUDIO_V2_HEADER)+3)/4*4))
                    {
                        TRANSMITTER_LOG_E("[ULL Audio V2][DL][ERROR]Error packet size %d, %d\r\n", 2, payload_size, (((n9_dsp_share_info_t *)(config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_param[i].share_info))->sub_info.block_info.block_size));
                        AUDIO_ASSERT(0);
                    }
                    open_param->stream_out_param.bt_common.scenario_param.ull_audio_v2_dongle_param.bt_link_info[i].share_info                          = config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_param[i].share_info;
                    open_param->stream_out_param.bt_common.scenario_param.ull_audio_v2_dongle_param.bt_link_info[i].codec_type                          = config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_param[i].codec_type;
                    if (config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_param[i].codec_type == AUDIO_DSP_CODEC_TYPE_LC3PLUS)
                    {
                        open_param->stream_out_param.bt_common.scenario_param.ull_audio_v2_dongle_param.bt_link_info[i].codec_param.lc3plus.sample_rate     = config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_param[i].codec_param.lc3plus.sample_rate;
                        open_param->stream_out_param.bt_common.scenario_param.ull_audio_v2_dongle_param.bt_link_info[i].codec_param.lc3plus.sample_format   = config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_param[i].codec_param.lc3plus.sample_format;
                        open_param->stream_out_param.bt_common.scenario_param.ull_audio_v2_dongle_param.bt_link_info[i].codec_param.lc3plus.channel_mode    = config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_param[i].codec_param.lc3plus.channel_mode;
                        open_param->stream_out_param.bt_common.scenario_param.ull_audio_v2_dongle_param.bt_link_info[i].codec_param.lc3plus.frame_interval  = config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_param[i].codec_param.lc3plus.frame_interval;
                        open_param->stream_out_param.bt_common.scenario_param.ull_audio_v2_dongle_param.bt_link_info[i].codec_param.lc3plus.frame_size      = config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_param[i].codec_param.lc3plus.frame_size;
                        open_param->stream_out_param.bt_common.scenario_param.ull_audio_v2_dongle_param.bt_link_info[i].codec_param.lc3plus.bit_rate        = config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_param[i].codec_param.lc3plus.bit_rate;
                        TRANSMITTER_LOG_I("[ULL Audio V2][DL]BT link codec setting ch%u: %u, %u, %u, %u, %u, %u, %u, %u, 0x%x, 0x%x\r\n", 11,
                                            i+1,
                                            config->scenario_sub_id,
                                            config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_param[i].codec_type,
                                            config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_param[i].codec_param.lc3plus.sample_rate,
                                            config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_param[i].codec_param.lc3plus.sample_format,
                                            config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_param[i].codec_param.lc3plus.channel_mode,
                                            config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_param[i].codec_param.lc3plus.frame_interval,
                                            config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_param[i].codec_param.lc3plus.bit_rate,
                                            payload_size,
                                            config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_param[i].share_info,
                                            ((n9_dsp_share_info_t *)(config->scenario_config.ull_audio_v2_dongle_config.dl_config.sink_param.bt_out_param.link_param[i].share_info))->start_addr);
                    }
                }
                else
                {
                    open_param->stream_out_param.bt_common.scenario_param.ull_audio_v2_dongle_param.bt_link_info[i].share_info = NULL;
                }
            }
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_LINE_IN:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, config->scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_I2S_MST_IN_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, config->scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_I2S_SLV_IN_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, config->scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        default:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not in the list, %u\r\n", 1, config->scenario_sub_id);
            AUDIO_ASSERT(0);
            break;
    }
}

void ull_audio_v2_dongle_start_playback(audio_transmitter_config_t *config, mcu2dsp_start_param_t *start_param)
{
    switch (config->scenario_sub_id)
    {
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0:
            start_param->param.stream_in  = STREAM_IN_BT_COMMON;
            start_param->param.stream_out = STREAM_OUT_AUDIO_TRANSMITTER;
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_LINE_OUT:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, config->scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_I2S_MST_OUT_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, config->scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_I2S_SLV_OUT_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, config->scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0:
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_1:
            start_param->param.stream_in  = STREAM_IN_AUDIO_TRANSMITTER;
            start_param->param.stream_out = STREAM_OUT_BT_COMMON;
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_LINE_IN:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, config->scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_I2S_MST_IN_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, config->scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_I2S_SLV_IN_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, config->scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        default:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not in the list, %u\r\n", 1, config->scenario_sub_id);
            AUDIO_ASSERT(0);
            break;
    }
}

audio_transmitter_status_t ull_audio_v2_dongle_set_runtime_config_playback(audio_transmitter_config_t *config, audio_transmitter_runtime_config_type_t runtime_config_type, audio_transmitter_runtime_config_t *runtime_config, mcu2dsp_audio_transmitter_runtime_config_param_t *runtime_config_param)
{
    audio_transmitter_status_t ret = AUDIO_TRANSMITTER_STATUS_FAIL;
    uint32_t operation = runtime_config_type;
    vol_gain_t gain;
    uint8_t mix_ratio = 0;
    int32_t vol_gain;
    int32_t vol_level;

    switch (config->scenario_sub_id)
    {
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0:
            switch (operation)
            {
                case ULL_AUDIO_V2_DONGLE_CONFIG_OP_SET_UL_VOL_INFO:
                    vol_gain        = runtime_config->ull_audio_v2_dongle_runtime_config.vol_info.vol_gain/100;
                    gain.vol_ch     = runtime_config->ull_audio_v2_dongle_runtime_config.vol_info.vol_ch;
                    gain.vol_gain   = vol_gain;
                    runtime_config_param->config_operation = operation;
                    memcpy(runtime_config_param->config_param, &gain, sizeof(vol_gain_t));
                    TRANSMITTER_LOG_I("[ULL Audio V2][UL]scenario_sub_id =%d: operation %d :volume gain=%d, volume_ratio = %d, volume channel=%d, finial gain=%d.",
                                        6 ,
                                        config->scenario_sub_id,
                                        operation,
                                        runtime_config->ull_audio_v2_dongle_runtime_config.vol_info.vol_gain,
                                        runtime_config->ull_audio_v2_dongle_runtime_config.vol_info.vol_ratio,
                                        runtime_config->ull_audio_v2_dongle_runtime_config.vol_info.vol_ch,
                                        gain.vol_gain);
                    ret = AUDIO_TRANSMITTER_STATUS_SUCCESS;
                    break;

                case ULL_AUDIO_V2_DONGLE_CONFIG_OP_SET_UL_VOL_LEVEL:
                    vol_level       = runtime_config->ull_audio_v2_dongle_runtime_config.vol_info.vol_level;
                    if (vol_level > bt_sink_srv_ami_get_usb_voice_sw_max_volume_level()) {
                        vol_level = bt_sink_srv_ami_get_usb_voice_sw_max_volume_level();
                        TRANSMITTER_LOG_E("[ULL Audio V2][UL][ERROR]set volume %d level more than max level %d\r\n", 2, vol_level, bt_sink_srv_ami_get_usb_voice_sw_max_volume_level());
                    }
                    vol_gain        = audio_get_gain_in_in_dB(vol_level, GAIN_DIGITAL, VOL_USB_VOICE_SW_OUT);
                    gain.vol_ch     = runtime_config->ull_audio_v2_dongle_runtime_config.vol_info.vol_ch;
                    gain.vol_gain   = vol_gain;
                    runtime_config_param->config_operation = ULL_AUDIO_V2_DONGLE_CONFIG_OP_SET_UL_VOL_INFO;
                    memcpy(runtime_config_param->config_param, &gain, sizeof(vol_gain_t));
                    TRANSMITTER_LOG_I("[ULL Audio V2][UL]scenario_sub_id =%d: operation %d :volume level=%d, volume_ratio = %d, volume channel=%d, finial gain=%d.",
                                        6 ,
                                        config->scenario_sub_id,
                                        operation,
                                        runtime_config->ull_audio_v2_dongle_runtime_config.vol_info.vol_level,
                                        runtime_config->ull_audio_v2_dongle_runtime_config.vol_info.vol_ratio,
                                        runtime_config->ull_audio_v2_dongle_runtime_config.vol_info.vol_ch,
                                        gain.vol_gain);
                    ret = AUDIO_TRANSMITTER_STATUS_SUCCESS;
                    break;

                case ULL_AUDIO_V2_DONGLE_CONFIG_OP_SET_UL_CH_INPUT_SOURCE:
                    runtime_config_param->config_operation = operation;
                    memcpy(runtime_config_param->config_param, &runtime_config->ull_audio_v2_dongle_runtime_config.connection_info, sizeof(ull_audio_v2_connection_info_t));
                    TRANSMITTER_LOG_I("[ULL Audio V2][UL]scenario_sub_id =%d: operation %d :ch choose=%d, ch connection=0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x.",
                                        11,
                                        config->scenario_sub_id,
                                        operation,
                                        runtime_config->ull_audio_v2_dongle_runtime_config.connection_info.chx_choose,
                                        runtime_config->ull_audio_v2_dongle_runtime_config.connection_info.ch1_connection,
                                        runtime_config->ull_audio_v2_dongle_runtime_config.connection_info.ch2_connection,
                                        runtime_config->ull_audio_v2_dongle_runtime_config.connection_info.ch3_connection,
                                        runtime_config->ull_audio_v2_dongle_runtime_config.connection_info.ch4_connection,
                                        runtime_config->ull_audio_v2_dongle_runtime_config.connection_info.ch5_connection,
                                        runtime_config->ull_audio_v2_dongle_runtime_config.connection_info.ch6_connection,
                                        runtime_config->ull_audio_v2_dongle_runtime_config.connection_info.ch7_connection,
                                        runtime_config->ull_audio_v2_dongle_runtime_config.connection_info.ch8_connection);
                    ret = AUDIO_TRANSMITTER_STATUS_SUCCESS;
                    break;

                default:
                    break;
            }
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_LINE_OUT:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, config->scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_I2S_MST_OUT_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, config->scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_I2S_SLV_OUT_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, config->scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0:
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_1:
            switch (operation)
            {
                case ULL_AUDIO_V2_DONGLE_CONFIG_OP_SET_DL_MIX:
                    TRANSMITTER_LOG_I("[ULL Audio V2][DL]scenario_sub_id =%d: DL_MIX id %d.", 2, config->scenario_sub_id, runtime_config->ull_audio_v2_dongle_runtime_config.dl_mixer_id);
                    runtime_config_param->config_operation = operation;
                    dl_mixer_param_t dl_mixer;
                    dl_mixer.scenario_type   = g_audio_transmitter_control[runtime_config->ull_audio_v2_dongle_runtime_config.dl_mixer_id].config.scenario_type;
                    dl_mixer.scenario_sub_id = g_audio_transmitter_control[runtime_config->ull_audio_v2_dongle_runtime_config.dl_mixer_id].config.scenario_sub_id;
                    memcpy(runtime_config_param->config_param, &dl_mixer, sizeof(dl_mixer_param_t));
                    ret = AUDIO_TRANSMITTER_STATUS_SUCCESS;
                    break;

                case ULL_AUDIO_V2_DONGLE_CONFIG_OP_SET_DL_UNMIX:
                    TRANSMITTER_LOG_I("[ULL Audio V2][DL]scenario_sub_id =%d: DL_UNMIX id %d.", 2, config->scenario_sub_id, config->scenario_sub_id);
                    runtime_config_param->config_operation = operation;
                    ret = AUDIO_TRANSMITTER_STATUS_SUCCESS;
                    break;

                case ULL_AUDIO_V2_DONGLE_CONFIG_OP_SET_DL_VOL_INFO:
                    mix_ratio       = runtime_config->ull_audio_v2_dongle_runtime_config.vol_info.vol_ratio;
                    vol_gain        = runtime_config->ull_audio_v2_dongle_runtime_config.vol_info.vol_gain/100;
                    gain.vol_ch     = runtime_config->ull_audio_v2_dongle_runtime_config.vol_info.vol_ch;
                    gain.vol_gain   = vol_gain + gain_compensation_table[mix_ratio/GAIN_COMPENSATION_STEP];
                    runtime_config_param->config_operation = operation;
                    memcpy(runtime_config_param->config_param, &gain, sizeof(vol_gain_t));
                    TRANSMITTER_LOG_I("[ULL Audio V2][DL]scenario_sub_id =%d: operation %d :volume gain=%d, volume_ratio = %d, volume channel=%d, finial gain=%d.",
                                        6 ,
                                        config->scenario_sub_id,
                                        operation,
                                        runtime_config->ull_audio_v2_dongle_runtime_config.vol_info.vol_gain,
                                        runtime_config->ull_audio_v2_dongle_runtime_config.vol_info.vol_ratio,
                                        runtime_config->ull_audio_v2_dongle_runtime_config.vol_info.vol_ch,
                                        gain.vol_gain);
                    ret = AUDIO_TRANSMITTER_STATUS_SUCCESS;
                    break;

                case ULL_AUDIO_V2_DONGLE_CONFIG_OP_SET_DL_VOL_LEVEL:
                    mix_ratio       = runtime_config->ull_audio_v2_dongle_runtime_config.vol_info.vol_ratio;
                    vol_level       = runtime_config->ull_audio_v2_dongle_runtime_config.vol_info.vol_level;
                    if (vol_level > bt_sink_srv_ami_get_usb_music_sw_max_volume_level()) {
                        vol_level = bt_sink_srv_ami_get_usb_music_sw_max_volume_level();
                        TRANSMITTER_LOG_E("[ULL Audio V2][DL][ERROR]set volume %d level more than max level %d\r\n", 2, vol_level, bt_sink_srv_ami_get_usb_music_sw_max_volume_level());
                    }
                    vol_gain        = audio_get_gain_out_in_dB(vol_level, GAIN_DIGITAL, VOL_USB_AUDIO_SW_IN);
                    gain.vol_ch     = runtime_config->ull_audio_v2_dongle_runtime_config.vol_info.vol_ch;
                    gain.vol_gain   = vol_gain + gain_compensation_table[mix_ratio/GAIN_COMPENSATION_STEP];
                    runtime_config_param->config_operation = ULL_AUDIO_V2_DONGLE_CONFIG_OP_SET_DL_VOL_INFO;
                    memcpy(runtime_config_param->config_param, &gain, sizeof(vol_gain_t));
                    TRANSMITTER_LOG_I("[ULL Audio V2][DL]scenario_sub_id =%d: operation %d :volume level=%d, volume_ratio = %d, volume channel=%d, finial gain=%d.",
                                        6 ,
                                        config->scenario_sub_id,
                                        operation,
                                        runtime_config->ull_audio_v2_dongle_runtime_config.vol_info.vol_level,
                                        runtime_config->ull_audio_v2_dongle_runtime_config.vol_info.vol_ratio,
                                        runtime_config->ull_audio_v2_dongle_runtime_config.vol_info.vol_ch,
                                        gain.vol_gain);
                    ret = AUDIO_TRANSMITTER_STATUS_SUCCESS;
                    break;

                default:
                    break;
            }
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_LINE_IN:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, config->scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_I2S_MST_IN_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, config->scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_I2S_SLV_IN_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, config->scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        default:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not in the list, %u\r\n", 1, config->scenario_sub_id);
            AUDIO_ASSERT(0);
            break;
    }

    return ret;
}

audio_transmitter_status_t ull_audio_v2_dongle_get_runtime_config(uint8_t scenario_type, uint8_t scenario_sub_id, audio_transmitter_runtime_config_type_t runtime_config_type, audio_transmitter_runtime_config_t *runtime_config)
{
    audio_transmitter_status_t ret = AUDIO_TRANSMITTER_STATUS_FAIL;

    switch (scenario_sub_id)
    {
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_LINE_OUT:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_I2S_MST_OUT_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_I2S_SLV_OUT_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_1:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_LINE_IN:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_I2S_MST_IN_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_I2S_SLV_IN_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        default:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not in the list, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;
    }

    return ret;
}

void ull_audio_v2_dongle_state_started_handler(uint8_t scenario_sub_id)
{
    uint32_t saved_mask;

    switch (scenario_sub_id)
    {
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0:
            hal_nvic_save_and_set_interrupt_mask(&saved_mask);
            usb_stream_tx_handle[0].first_time = 0;
            usb_stream_tx_handle[0].stream_is_started = 0;
            #if ULL_AUDIO_V2_DONGLE_DEBUG_LANTENCY
            usb_stream_tx_handle[0].latency_debug_enable = 0;
            usb_stream_tx_handle[0].latency_debug_gpio_pin = HAL_GPIO_13;
            #endif /* ULL_AUDIO_V2_DONGLE_DEBUG_LANTENCY */
            ull_audio_v2_dongle_ul_stream_status |= 0x1<<(scenario_sub_id-AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0);
            hal_nvic_restore_interrupt_mask(saved_mask);
            #ifdef MTK_USB_AUDIO_MICROPHONE
            USB_Audio_Register_Tx_Callback(0, usb_audio_tx_cb_ull_audio_v2_dongle_0);
            TRANSMITTER_LOG_I("[ULL Audio V2][USB_TX_DEBUG 0]Register usb_audio_tx_cb 0", 0);
            #endif /* MTK_USB_AUDIO_MICROPHONE */
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_LINE_OUT:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_I2S_MST_OUT_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_I2S_SLV_OUT_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0:
            /* update state machine */
            usb_stream_rx_handle[0].first_time = 0;
            usb_stream_rx_handle[0].stream_is_started = 0;
            #if ULL_AUDIO_V2_DONGLE_DEBUG_LANTENCY
            usb_stream_rx_handle[0].latency_debug_enable = 0;
            usb_stream_rx_handle[0].latency_debug_gpio_pin = HAL_GPIO_13;
            #endif /* ULL_AUDIO_V2_DONGLE_DEBUG_LANTENCY */
            ull_audio_v2_dongle_dl_stream_status |= 0x1<<(scenario_sub_id-AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0);
            #if defined(MTK_USB_DEMO_ENABLED) && (defined(MTK_USB_AUDIO_V1_ENABLE) || defined(MTK_USB_AUDIO_V2_ENABLE))
            USB_Audio_Register_Rx_Callback(0, usb_audio_rx_cb_ull_audio_v2_dongle_0);
            TRANSMITTER_LOG_I("[ULL Audio V2][USB_RX_DEBUG 0]Register usb_audio_rx_cb 0", 0);
            #endif /* MTK_USB_DEMO_ENABLED */
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_1:
            usb_stream_rx_handle[1].first_time = 0;
            usb_stream_rx_handle[1].stream_is_started = 0;
            #if ULL_AUDIO_V2_DONGLE_DEBUG_LANTENCY
            usb_stream_rx_handle[1].latency_debug_enable = 0;
            usb_stream_rx_handle[1].latency_debug_gpio_pin = HAL_GPIO_13;
            #endif /* ULL_AUDIO_V2_DONGLE_DEBUG_LANTENCY */
            ull_audio_v2_dongle_dl_stream_status |= 0x1<<(scenario_sub_id-AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0);
            #if defined(MTK_USB_DEMO_ENABLED) && (defined(MTK_USB_AUDIO_V1_ENABLE) || defined(MTK_USB_AUDIO_V2_ENABLE))
            USB_Audio_Register_Rx_Callback(1, usb_audio_rx_cb_ull_audio_v2_dongle_1);
            TRANSMITTER_LOG_I("[ULL Audio V2][USB_RX_DEBUG 1]Register usb_audio_rx_cb 1", 0);
            #endif /* MTK_USB_DEMO_ENABLED */
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_LINE_IN:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_I2S_MST_IN_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_I2S_SLV_IN_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        default:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not in the list, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;
    }
}

void ull_audio_v2_dongle_state_idle_handler(uint8_t scenario_sub_id)
{
    uint32_t saved_mask;

    switch (scenario_sub_id)
    {
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0:
            hal_nvic_save_and_set_interrupt_mask(&saved_mask);
            usb_stream_tx_handle[0].first_time = 0;
            usb_stream_tx_handle[0].stream_is_started = 0;
            #if ULL_AUDIO_V2_DONGLE_DEBUG_LANTENCY
            usb_stream_tx_handle[0].latency_debug_enable = 0;
            #endif /* ULL_AUDIO_V2_DONGLE_DEBUG_LANTENCY */
            ull_audio_v2_dongle_ul_stream_status &= ~(0x1<<(scenario_sub_id-AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0));
            hal_nvic_restore_interrupt_mask(saved_mask);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_LINE_OUT:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_I2S_MST_OUT_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_I2S_SLV_OUT_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0:
            #if defined(MTK_USB_DEMO_ENABLED) && (defined(MTK_USB_AUDIO_V1_ENABLE) || defined(MTK_USB_AUDIO_V2_ENABLE))
            USB_Audio_Register_Rx_Callback(0, NULL);
            TRANSMITTER_LOG_I("[ULL Audio V2][USB_RX_DEBUG 0]Unregister usb_audio_rx_cb 0", 0);
            #endif /* MTK_USB_DEMO_ENABLED */
            usb_stream_rx_handle[0].first_time = 0;
            usb_stream_rx_handle[0].stream_is_started = 0;
            #if ULL_AUDIO_V2_DONGLE_DEBUG_LANTENCY
            usb_stream_rx_handle[0].latency_debug_enable = 0;
            #endif /* ULL_AUDIO_V2_DONGLE_DEBUG_LANTENCY */
            ull_audio_v2_dongle_dl_stream_status &= ~(0x1<<(scenario_sub_id-AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0));
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_1:
            #if defined(MTK_USB_DEMO_ENABLED) && (defined(MTK_USB_AUDIO_V1_ENABLE) || defined(MTK_USB_AUDIO_V2_ENABLE))
            USB_Audio_Register_Rx_Callback(1, NULL);
            TRANSMITTER_LOG_I("[ULL Audio V2][USB_RX_DEBUG 1]Unregister usb_audio_rx_cb 1", 0);
            #endif /* MTK_USB_DEMO_ENABLED */
            usb_stream_rx_handle[1].first_time = 0;
            usb_stream_rx_handle[1].stream_is_started = 0;
            #if ULL_AUDIO_V2_DONGLE_DEBUG_LANTENCY
            usb_stream_rx_handle[1].latency_debug_enable = 0;
            #endif /* ULL_AUDIO_V2_DONGLE_DEBUG_LANTENCY */
            ull_audio_v2_dongle_dl_stream_status &= ~(0x1<<(scenario_sub_id-AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0));
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_LINE_IN:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_I2S_MST_IN_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_I2S_SLV_IN_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        default:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not in the list, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;
    }
}

void ull_audio_v2_dongle_state_starting_handler(uint8_t scenario_sub_id)
{
    switch (scenario_sub_id)
    {
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0:
            /* there is counter in DVFS API, so do not need add counter here */
            #if defined(AIR_BTA_IC_PREMIUM_G2)
            hal_dvfs_lock_control(HAL_DVFS_HIGH_SPEED_208M, HAL_DVFS_LOCK);
            #else
            hal_dvfs_lock_control(HAL_DVFS_OPP_HIGH, HAL_DVFS_LOCK);
            #endif
            TRANSMITTER_LOG_I("[ULL Audio V2] lock cpu to high", 0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_LINE_OUT:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_I2S_MST_OUT_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_I2S_SLV_OUT_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0:
            /* there is counter in DVFS API, so do not need add counter here */
            #if defined(AIR_BTA_IC_PREMIUM_G2)
            hal_dvfs_lock_control(HAL_DVFS_HIGH_SPEED_208M, HAL_DVFS_LOCK);
            #else
            hal_dvfs_lock_control(HAL_DVFS_OPP_HIGH, HAL_DVFS_LOCK);
            #endif
            TRANSMITTER_LOG_I("[ULL Audio V2] lock cpu to high", 0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_1:
            /* there is counter in DVFS API, so do not need add counter here */
            #if defined(AIR_BTA_IC_PREMIUM_G2)
            hal_dvfs_lock_control(HAL_DVFS_HIGH_SPEED_208M, HAL_DVFS_LOCK);
            #else
            hal_dvfs_lock_control(HAL_DVFS_OPP_HIGH, HAL_DVFS_LOCK);
            #endif
            TRANSMITTER_LOG_I("[ULL Audio V2] lock cpu to high", 0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_LINE_IN:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_I2S_MST_IN_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_I2S_SLV_IN_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        default:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not in the list, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;
    }
}

void ull_audio_v2_dongle_state_stoping_handler(uint8_t scenario_sub_id)
{
    switch (scenario_sub_id)
    {
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0:
            /* there is counter in DVFS API, so do not need add counter here */
            #if defined(AIR_BTA_IC_PREMIUM_G2)
            hal_dvfs_lock_control(HAL_DVFS_HIGH_SPEED_208M, HAL_DVFS_UNLOCK);
            #else
            hal_dvfs_lock_control(HAL_DVFS_OPP_HIGH, HAL_DVFS_UNLOCK);
            #endif
            TRANSMITTER_LOG_I("[ULL Audio V2] lock cpu to high", 0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_LINE_OUT:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_I2S_MST_OUT_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_I2S_SLV_OUT_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0:
            /* there is counter in DVFS API, so do not need add counter here */
            #if defined(AIR_BTA_IC_PREMIUM_G2)
            hal_dvfs_lock_control(HAL_DVFS_HIGH_SPEED_208M, HAL_DVFS_UNLOCK);
            #else
            hal_dvfs_lock_control(HAL_DVFS_OPP_HIGH, HAL_DVFS_UNLOCK);
            #endif
            TRANSMITTER_LOG_I("[ULL Audio V2] lock cpu to high", 0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_1:
            /* there is counter in DVFS API, so do not need add counter here */
            #if defined(AIR_BTA_IC_PREMIUM_G2)
            hal_dvfs_lock_control(HAL_DVFS_HIGH_SPEED_208M, HAL_DVFS_UNLOCK);
            #else
            hal_dvfs_lock_control(HAL_DVFS_OPP_HIGH, HAL_DVFS_UNLOCK);
            #endif
            TRANSMITTER_LOG_I("[ULL Audio V2] lock cpu to high", 0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_LINE_IN:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_I2S_MST_IN_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_I2S_SLV_IN_0:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not supported at now, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;

        default:
            TRANSMITTER_LOG_E("[ULL Audio V2][ERROR]This scenario is not in the list, %u\r\n", 1, scenario_sub_id);
            AUDIO_ASSERT(0);
            break;
    }
}

#endif /* AIR_ULL_AUDIO_V2_DONGLE_ENABLE */
