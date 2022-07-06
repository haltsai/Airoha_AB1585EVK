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

/******************************************************************************
 * 
 * Include
 * 
 ******************************************************************************/
#include "dsp_mux_uart.h"
#include "preloader_pisplit.h"
#include "source_inter.h"
//#include "sw_gain_interface.h"

#ifdef AIR_SOFTWARE_MIXER_ENABLE
#include "sw_mixer_interface.h"
#endif

#include "dsp_callback.h"
#include "transform_inter.h"
#include "sink_inter.h"
#include "hal_dwt.h"
#include "dsp_dump.h"

/******************************************************************************
 * 
 * Extern Function
 * 
 ******************************************************************************/
EXTERN VOID DSP_C2D_BufferCopy(VOID *DestBuf,
                               VOID *SrcBuf,
                               U32 CopySize,
                               VOID *SrcCBufStart,
                               U32 SrcCBufSize);

/******************************************************************************
 * 
 * Private Macro and Variable Declaration 
 * 
 ******************************************************************************/
#ifdef AIR_SOFTWARE_MIXER_ENABLE
static sw_mixer_member_t *mixer_member = NULL;
#endif
//static sw_gain_port_t *dchs_gain_port = NULL;

/******************************************************************************
 * 
 * Private Function Define
 * 
 ******************************************************************************/
#if 0
void SourceUartSwGainInit(SOURCE source)
{
#ifdef AIR_SOFTWARE_GAIN_ENABLE
    /* sw gain config */
    int32_t default_gain;
    sw_gain_config_t default_config;
    default_config.resolution = RESOLUTION_32BIT;
    default_config.target_gain = 0;
    default_config.up_step = 1;
    default_config.up_samples_per_step = 2;
    default_config.down_step = -1;
    default_config.down_samples_per_step = 2;
    dchs_gain_port = stream_function_sw_gain_get_port(source);
    stream_function_sw_gain_init(dchs_gain_port, 1, &default_config);
    default_gain = 0;
    stream_function_sw_gain_configure_gain_target(dongle_handle->gain_port, 1, default_gain);
    DSP_MW_LOG_I("[DCHS][SW Gain]change channel %d gain to %d*0.01dB", 2, 1, default_gain);
#endif /* AIR_SOFTWARE_GAIN_ENABLE */
}
#endif

void SourceUartSwMixerInit(SOURCE source)
{
    #ifdef AIR_SOFTWARE_MIXER_ENABLE
    /* sw mixer config */
    sw_mixer_callback_config_t       callback_config;
    sw_mixer_input_channel_config_t  in_ch_config;
    sw_mixer_output_channel_config_t out_ch_config;
    stream_function_sw_mixer_init(SW_MIXER_PORT_0);
    callback_config.preprocess_callback = NULL;
    callback_config.postprocess_callback = NULL;
    in_ch_config.total_channel_number = 2;
    in_ch_config.resolution = (source->param.audio.format_bytes == 4) ? RESOLUTION_32BIT : RESOLUTION_16BIT;
    in_ch_config.input_mode = SW_MIXER_CHANNEL_MODE_NORMAL;
    in_ch_config.buffer_size = source->param.audio.frame_size * source->param.audio.format_bytes;
    out_ch_config.total_channel_number = 1;
    out_ch_config.resolution = (source->param.audio.format_bytes == 4) ? RESOLUTION_32BIT : RESOLUTION_16BIT;
    mixer_member = stream_function_sw_mixer_member_create((void *)source,
                                                                         SW_MIXER_MEMBER_MODE_NO_BYPASS,
                                                                         &callback_config,
                                                                         &in_ch_config,
                                                                         &out_ch_config);

    stream_function_sw_mixer_member_register(SW_MIXER_PORT_0, mixer_member, false);
    /* do connections */
    #ifdef AIR_DUAL_CHIP_MIXING_MODE_ROLE_SLAVE_ENABLE
    #ifdef AIR_BT_CODEC_BLE_ENABLED
    if(Source_blks[SOURCE_TYPE_A2DP] || Source_blks[SOURCE_TYPE_N9SCO] || Source_blks[SOURCE_TYPE_N9BLE]){//[hard code]
    #else
    if(Source_blks[SOURCE_TYPE_A2DP] || Source_blks[SOURCE_TYPE_N9SCO]){
    #endif
        stream_function_sw_mixer_channel_connect(mixer_member, 2, SW_MIXER_CHANNEL_ATTRIBUTE_MAIN,   mixer_member  , 1);
        stream_function_sw_mixer_channel_connect(mixer_member, 1, SW_MIXER_CHANNEL_ATTRIBUTE_NORMAL, mixer_member, 1);   
    }else{
        stream_function_sw_mixer_channel_connect(mixer_member, 1, SW_MIXER_CHANNEL_ATTRIBUTE_MAIN, mixer_member  , 1);
        stream_function_sw_mixer_channel_connect(mixer_member, 2, SW_MIXER_CHANNEL_ATTRIBUTE_NORMAL, mixer_member, 1);
    }
    #else
    #ifdef AIR_BT_CODEC_BLE_ENABLED
    if(Source_blks[SOURCE_TYPE_A2DP] || Source_blks[SOURCE_TYPE_N9SCO] || Source_blks[SOURCE_TYPE_N9BLE]){//[hard code]
    #else
    if(Source_blks[SOURCE_TYPE_A2DP] || Source_blks[SOURCE_TYPE_N9SCO]){
    #endif
        stream_function_sw_mixer_channel_connect(mixer_member, 1, SW_MIXER_CHANNEL_ATTRIBUTE_MAIN, mixer_member  , 1);
        stream_function_sw_mixer_channel_connect(mixer_member, 2, SW_MIXER_CHANNEL_ATTRIBUTE_NORMAL, mixer_member, 1);
    }else{
        stream_function_sw_mixer_channel_connect(mixer_member, 2, SW_MIXER_CHANNEL_ATTRIBUTE_MAIN, mixer_member, 1);
        stream_function_sw_mixer_channel_connect(mixer_member, 1, SW_MIXER_CHANNEL_ATTRIBUTE_NORMAL, mixer_member  , 1);
    }
    #endif//AIR_DUAL_CHIP_MIXING_MODE_ROLE_SLAVE_ENABLE
    #endif//AIR_SOFTWARE_MIXER_ENABLE
}

U32 SourceSizeUart(SOURCE source)
{
    static U32 log_print_freq = 0;
    log_print_freq ++;
    U32 local_write_offset = source->streamBuffer.BufferInfo.WriteOffset;//local scenario ch
    U32 local_read_offset  = source->streamBuffer.BufferInfo.ReadOffset;
    U32 local_length       = source->streamBuffer.BufferInfo.length;

    U32 audio_frame_size = source->param.audio.frame_size * source->param.audio.format_bytes;
    U32 uart_data_size  = dsp_query_uart_rx_buf_remain_size(UART_DL);

    U32 local_data_size = (local_write_offset >= local_read_offset) ? (local_write_offset - local_read_offset) : (local_length - local_read_offset + local_write_offset);

    SOURCE a2dp_source = Source_blks[SOURCE_TYPE_A2DP];
    #ifdef AIR_BT_CODEC_BLE_ENABLED
    if((a2dp_source && a2dp_source->transform) || Source_blks[SOURCE_TYPE_N9SCO] || Source_blks[SOURCE_TYPE_N9BLE]){
    #else
    if((a2dp_source && a2dp_source->transform) || Source_blks[SOURCE_TYPE_N9SCO]){
    #endif
        if(dsp_check_other_chip_scenario_exist()){
            if ((source->streamBuffer.BufferInfo.bBufferIsFull) || (local_data_size >= audio_frame_size) && (uart_data_size >= audio_frame_size)) {
                if(log_print_freq % 1000 == 0){
                    DSP_MW_LOG_I("[SourceSizeUart] local and uart return size=%d",1,audio_frame_size);
                }
                return audio_frame_size;
            }
        }else{
            if ((source->streamBuffer.BufferInfo.bBufferIsFull) || (local_data_size >= audio_frame_size)) {
                if(log_print_freq % 1000 == 0){
                    DSP_MW_LOG_I("[SourceSizeUart] local return size=%d",1,audio_frame_size);
                }
                return audio_frame_size;
            }
        }
    }else{
        if (uart_data_size >= audio_frame_size) {
            if(log_print_freq % 1000 == 0){
                DSP_MW_LOG_I("[SourceSizeUart] uart return size=%d",1,audio_frame_size);
            }
            return audio_frame_size;
        }
    }
    //DSP_MW_LOG_W("[DCHS][Source] buf no enough data, frame bytes:%d, uart size:%d, local size:%d,local_wo:%d, local_ro:%d, local_length:%d", 9, audio_frame_size, uart_data_size,local_data_size, local_write_offset, local_read_offset, local_length);
    return 0;
}

VOID SourceDropUart(SOURCE source, U32 amount)
{
    if (amount == 0) {
        return;
    } else if (source->streamBuffer.BufferInfo.bBufferIsFull == TRUE) {
        source->streamBuffer.BufferInfo.bBufferIsFull = FALSE;
    }
    source->streamBuffer.BufferInfo.ReadOffset
        = (source->streamBuffer.BufferInfo.ReadOffset + amount)
            % (source->streamBuffer.BufferInfo.length);
}

BOOL SourceCloseUart(SOURCE source)
{
    preloader_pisplit_free_memory(source->streamBuffer.BufferInfo.startaddr[0]);
    #ifdef AIR_SOFTWARE_MIXER_ENABLE
    if(mixer_member){
        stream_function_sw_mixer_member_unregister(SW_MIXER_PORT_0, mixer_member);
        stream_function_sw_mixer_member_delete(mixer_member);
        stream_function_sw_mixer_deinit(SW_MIXER_PORT_0);
        mixer_member = NULL;
    }
    #endif /* AIR_SOFTWARE_MIXER_ENABLE */  
    return TRUE;
}

BOOL SourceReadBufUart(SOURCE source, U8 *dst_addr, U32 length)
{
    TRANSFORM transform =  source->transform;
    DSP_CALLBACK_PTR callback_ptr = DSP_Callback_Get(source, transform->sink);
    U8 * local_ch_addr = NULL;
    U8 * uart_ch_addr  = NULL;
    U32 local_ch_read_offset = source->streamBuffer.BufferInfo.ReadOffset;
    U32 local_ch_buffer_len  = source->streamBuffer.BufferInfo.length;

    #ifdef AIR_DUAL_CHIP_MIXING_MODE_ROLE_MASTER_ENABLE
    local_ch_addr = callback_ptr->EntryPara.in_ptr[0];
    uart_ch_addr  = callback_ptr->EntryPara.in_ptr[1];
    #else
    local_ch_addr = callback_ptr->EntryPara.in_ptr[1];
    uart_ch_addr  = callback_ptr->EntryPara.in_ptr[0];
    #endif

    DSP_C2D_BufferCopy(local_ch_addr,
                            source->streamBuffer.BufferInfo.startaddr[0] + local_ch_read_offset,
                            length,
                            source->streamBuffer.BufferInfo.startaddr[0],
                            local_ch_buffer_len);

    if(dsp_query_uart_rx_buf_remain_size(UART_DL) >= length){
        dsp_uart_rx(UART_DL,uart_ch_addr,length);
    }else{
        memset(uart_ch_addr, 0, length);
    }
    
}

/******************************************************************************
 * 
 * Public Function Define
 * 
 ******************************************************************************/

SOURCE SourceInit_Uart(SOURCE source)
{
    U8 *mem_ptr = NULL;
    U32 relay_buf_size = 32*1024;//uart relay buf,for align A2DP Sink Audio SRAM buf 32k
    source->streamBuffer.BufferInfo.length  = relay_buf_size; //local chip scenario buf

    mem_ptr = preloader_pisplit_malloc_memory(PRELOADER_D_HIGH_PERFORMANCE, relay_buf_size);
    if(!mem_ptr){
        DSP_MW_LOG_E("[DCHS]][Source] malloc source buffer fail!", 0);
        AUDIO_ASSERT(0);
    }
    memset(mem_ptr, 0, relay_buf_size);
    DSP_MW_LOG_I("[DCHS]][Source] malloc source buffer :0x%x", 1,mem_ptr);
    source->streamBuffer.BufferInfo.startaddr[0]  = mem_ptr;
    source->streamBuffer.BufferInfo.bBufferIsFull = FALSE;

    //open uart
    //dsp_uart_rx_buffer_register(UART_DL,&(source->streamBuffer.BufferInfo));
    dsp_uart_open(UART_DL);
    //init sw gain
    //SourceUartSwGainInit(source);
    //init sw mixer
    SourceUartSwMixerInit(source);

    /* interface init */
    source->sif.SourceSize        = SourceSizeUart;
    source->sif.SourceDrop        = SourceDropUart;
    source->sif.SourceClose       = SourceCloseUart;
    source->sif.SourceReadBuf     = SourceReadBufUart;
    return source;
}