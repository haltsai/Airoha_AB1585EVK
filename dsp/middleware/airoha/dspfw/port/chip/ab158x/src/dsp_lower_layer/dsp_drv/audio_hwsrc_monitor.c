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

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "hal_audio_control.h"
#include "hal_audio_clock.h"
#include "hal_audio_driver.h"
#include "hal_audio_volume.h"
#include "hal_audio_register.h"
#include "sink_inter.h"
#include "source_inter.h"
#include "transform_.h"
#include "stream_config.h"
#include "transform_.h"
#include "sink_.h"
#include "source_.h"
#include "common.h"
#include "audio_hwsrc_monitor.h"
#include "hal_audio.h"


HWSRC_UNDERRUN_RECOVER_t recover_para = {0,0,0,0,0};
HWSRC_COMPEN_MONITOR_t compen_para;

void hal_audio_src_underrun_monitor_reset(void)
{
    recover_para.src_monitor_state = OBSERVE_MODE;
    recover_para.underrun_recovery_size = 0;
    recover_para.src_cnt = 0;
    recover_para.timer_flag = FALSE;
}

static void hwsrc_detect_timer_callback(void *user_data)
{
    SINK sink = (SINK)user_data;
    U32 src_out_size = ((sink->param.audio.AfeBlkControl.u4asrc_buffer_size) + AFE_GET_REG(ASM_CH01_OBUF_WRPNT) - AFE_GET_REG(ASM_CH01_OBUF_RDPNT)) % (sink->param.audio.AfeBlkControl.u4asrc_buffer_size);
    DSP_MW_LOG_E("[HWSRC]HWSRC output buffer underrun, trigger error handling, out size:%d",1,src_out_size);
    recover_para.src_monitor_state = ERROR_HANDLING_MODE;
}

void hal_audio_src_underrun_recover(SINK sink, U32 input_size)
{
    U32 hw_output_current_write_idx = AFE_READ(ASM_CH01_OBUF_WRPNT)-AFE_GET_REG(ASM_OBUF_SADR);
    U32 hw_output_read_idx = AFE_READ(ASM_CH01_OBUF_RDPNT)-AFE_GET_REG(ASM_OBUF_SADR);
    U32 src_out_data_size = (hw_output_current_write_idx >= hw_output_read_idx) ?  (hw_output_current_write_idx - hw_output_read_idx) : (hw_output_current_write_idx + AFE_GET_REG(ASM_OBUF_SIZE) - hw_output_read_idx);
    U32 gpt_timer1,gpt_timer2;
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &gpt_timer1);
    if (recover_para.underrun_recovery_size == 0) {
        hw_output_read_idx += 256;
        hw_output_read_idx %= AFE_GET_REG(ASM_OBUF_SIZE);
        recover_para.underrun_recovery_size = AFE_GET_REG(ASM_OBUF_SIZE) - src_out_data_size + 256;
        while (AFE_READ(ASM_CH01_OBUF_WRPNT) != hw_output_read_idx + AFE_GET_REG(ASM_OBUF_SADR))
        {
            hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &gpt_timer2);
            if ((gpt_timer2 - gpt_timer1) > 800)
            {
                recover_para.underrun_recovery_size = 0;
                break;
            }
            AFE_WRITE(ASM_CH01_OBUF_WRPNT,hw_output_read_idx + AFE_GET_REG(ASM_OBUF_SADR));
        }
        DSP_MW_LOG_W("[HWSRC] UNDERFLOW detected owo:%d oro:%d,  modify owo:%d recover size:%d time:%d", 5,hw_output_current_write_idx,hw_output_read_idx,AFE_READ(ASM_CH01_OBUF_WRPNT)-AFE_GET_REG(ASM_OBUF_SADR),recover_para.underrun_recovery_size,(gpt_timer2 - gpt_timer1));
        recover_para.src_cnt = 0;
    } else {
        U16 recover_size = MIN(recover_para.underrun_recovery_size,384);
        recover_size = MIN(recover_size,src_out_data_size);
        hw_output_current_write_idx += (AFE_GET_REG(ASM_OBUF_SIZE) - recover_size);
        hw_output_current_write_idx %= AFE_GET_REG(ASM_OBUF_SIZE);
        recover_para.underrun_recovery_size -= recover_size;
        while (AFE_READ(ASM_CH01_OBUF_WRPNT) != hw_output_current_write_idx + AFE_GET_REG(ASM_OBUF_SADR))
        {
            hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &gpt_timer2);
            if ((gpt_timer2 - gpt_timer1) > 800)
            {
                recover_para.underrun_recovery_size += recover_size;
                recover_size = 0;
                break;
            }
            AFE_WRITE(ASM_CH01_OBUF_WRPNT,hw_output_current_write_idx + AFE_GET_REG(ASM_OBUF_SADR));
        }
        DSP_MW_LOG_W("[HWSRC] UNDERFLOW modify owo:%d size:%d time:%d", 3,AFE_READ(ASM_CH01_OBUF_WRPNT)-AFE_GET_REG(ASM_OBUF_SADR),recover_size,(gpt_timer2 - gpt_timer1));

    }
    if ((recover_para.src_cnt%1000) < 2) {
        DSP_MW_LOG_W("[HWSRC]src1 irq,DL HWSRC UNDERFLOW debug owo:%d oro:%d size:%d thd:%d", 4,hw_output_current_write_idx,hw_output_read_idx,src_out_data_size,(input_size*sink->param.audio.rate/sink->param.audio.src_rate)*3/2);
    }
    recover_para.src_cnt++;
}

void hal_audio_src_underrun_monitor(BUFFER_INFO *buffer_info, SINK sink, U32 input_size)
{
    U32 output_rate = (sink->param.audio.rate) / 100;
    U32 input_rate = (sink->param.audio.src_rate) / 100;
    U8  channel_num = sink->param.audio.channel_num;
    U32 BytesPerSample = sink->param.audio.format_bytes;
    U32 remaing_time = 0;
    U32 src_out_size,src_convert_size,consumable_samples,duration_us;
    hal_gpt_status_t gpt_status;
    U32 WO = AFE_GET_REG(ASM_CH01_OBUF_WRPNT) - AFE_GET_REG(ASM_OBUF_SADR);
    U32 RO = AFE_GET_REG(ASM_CH01_OBUF_RDPNT) - AFE_GET_REG(ASM_OBUF_SADR);

    switch (recover_para.src_monitor_state)
    {
        case OBSERVE_MODE:
            hal_gpt_sw_get_remaining_time_us(recover_para.src_detect_handler, &remaing_time);
            if((WO == RO) && (((1000000/100) / output_rate) < remaing_time)){//Troubleshoot buffer full
                src_out_size = sink->param.audio.AfeBlkControl.u4asrc_buffer_size;
            }else{
                src_out_size = ((sink->param.audio.AfeBlkControl.u4asrc_buffer_size) + WO - RO) % (sink->param.audio.AfeBlkControl.u4asrc_buffer_size);
            }
            src_convert_size = (input_size * output_rate) / input_rate;
            BytesPerSample = sink->param.audio.format_bytes;
            consumable_samples = ((src_out_size + src_convert_size) / (channel_num * BytesPerSample));
            duration_us = (U32)(((U64)consumable_samples * (1000000/100)) / (U64)output_rate);

            if (recover_para.src_detect_handler == NULL) {
                gpt_status = hal_gpt_sw_get_timer(&(recover_para.src_detect_handler));
                if (gpt_status != HAL_GPT_STATUS_OK) {
                    DSP_MW_LOG_E("[HWSRC]src_detect_handler get timer fail:%d",1 , gpt_status);
                    AUDIO_ASSERT(0);
                }
            }else {
                if(remaing_time){
                    hal_gpt_sw_stop_timer_us(recover_para.src_detect_handler);
                }
            }

            hal_gpt_sw_start_timer_us(recover_para.src_detect_handler, duration_us, hwsrc_detect_timer_callback, (void*)sink);
            break;

        case ERROR_HANDLING_MODE:

            /*if(sink->transform->source->type == SOURCE_TYPE_A2DP) {
                sink->param.audio.afe_wait_play_en_cnt = PLAY_EN_TRIGGER_REINIT_MAGIC_NUM;
            }else if(sink->transform->source->type == SOURCE_TYPE_N9BLE) {
                hal_audio_src_underrun_recover(sink ,input_size);
            }*/
            sink->param.audio.afe_wait_play_en_cnt = PLAY_EN_TRIGGER_REINIT_MAGIC_NUM;
            break;

        default:
            break;
    }

}

U32 hal_audio_cal_compen_samples(BUFFER_INFO *buffer_info, SINK sink)
{
    U8 channel_num = sink->param.audio.channel_num;
    U32 output_rate = (sink->param.audio.rate) / 100;
    U32 input_rate = (sink->param.audio.src_rate) / 100;
    U32 BytesPerSample = sink->param.audio.format_bytes;
    U32 remainder = 0;
    U32 mask;

    compen_para.src_channel_num = channel_num;

    compen_para.i_rd_next = AFE_GET_REG(ASM_CH01_IBUF_RDPNT);
    compen_para.o_wr_next = AFE_GET_REG(ASM_CH01_OBUF_WRPNT);

    if (compen_para.i_rd_pre != 0xFFFFFFFF && compen_para.o_wr_pre != 0xFFFFFFFF) {
        compen_para.i_diff_point = (compen_para.i_rd_next - compen_para.i_rd_pre + (buffer_info->length)) % (buffer_info->length);
        compen_para.o_diff_point = (compen_para.o_wr_next - compen_para.o_wr_pre + (sink->param.audio.AfeBlkControl.u4asrc_buffer_size)) % (sink->param.audio.AfeBlkControl.u4asrc_buffer_size);
        //i_diff_point_remainder = (((U64)i_diff_point)*((U64)output_rate))%input_rate;

        compen_para.i_diff_point *= output_rate;
        remainder = compen_para.i_diff_point % input_rate;
        compen_para.i_diff_point -= remainder;
        compen_para.i_diff_point = compen_para.i_diff_point / input_rate;
        compen_para.i_diff_point_remainder += remainder;

        if (compen_para.i_diff_point_remainder >= input_rate) {
            compen_para.i_diff_point += (compen_para.i_diff_point_remainder / input_rate);

            hal_nvic_save_and_set_interrupt_mask(&mask);
            compen_para.irq_compensated_samples += (compen_para.i_diff_point_remainder / input_rate);
            hal_nvic_restore_interrupt_mask(mask);

            compen_para.i_diff_point_remainder = (compen_para.i_diff_point_remainder % input_rate);
        }
        compen_para.io_diff_point += ((S32)compen_para.o_diff_point - (S32)compen_para.i_diff_point);

        compen_para.i_samples += compen_para.i_diff_point / (BytesPerSample * channel_num);
    }

    compen_para.i_rd_pre = compen_para.i_rd_next;
    compen_para.o_wr_pre = compen_para.o_wr_next;

    return compen_para.i_diff_point / (BytesPerSample * channel_num);
}

void hal_audio_cal_compen_samples_reset(BUFFER_INFO *buffer_info, SINK sink)
{
    U32 BytesPerSample = sink->param.audio.format_bytes;
    U8 channel_num = sink->param.audio.channel_num;

    compen_para.i_rd_pre = AFE_GET_REG(ASM_CH01_IBUF_RDPNT);
    compen_para.o_wr_pre = AFE_GET_REG(ASM_CH01_OBUF_WRPNT);
    compen_para.i_rd_next = 0;
    compen_para.o_wr_next = 0;
    compen_para.o_diff_point = 0;
    compen_para.i_diff_point = 0;
    compen_para.i_diff_point_remainder = 0;
    compen_para.io_diff_point = 0;
    compen_para.i_samples = 0;
    compen_para.irq_compensated_samples = 0;

    DSP_MW_LOG_I("[HWSRC_CLK_SKEW]in_buff:%d, out_buff:%d BytesPerSample:%d, channel_num:%d", 4, buffer_info->length, sink->param.audio.AfeBlkControl.u4asrc_buffer_size, BytesPerSample, channel_num);
}

void hal_audio_get_compen_samples(S32 *compen_samples, U32 *input_samples, U8 *channel_num)
{
    *compen_samples = compen_para.io_diff_point;
    *input_samples = compen_para.i_samples;
    *channel_num = compen_para.src_channel_num;
}

U32 hal_audio_get_irq_compen_samples(void* ptr)
{
    SINK sink = (SINK)ptr;
    U32 samples;
    U32 mask;
    U8 channel_num = sink->param.audio.channel_num;
    U32 BytesPerSample = sink->param.audio.format_bytes;

    hal_nvic_save_and_set_interrupt_mask(&mask);
    samples = compen_para.irq_compensated_samples / (BytesPerSample * channel_num);
    compen_para.irq_compensated_samples = compen_para.irq_compensated_samples % (BytesPerSample * channel_num);
    hal_nvic_restore_interrupt_mask(mask);   

    return samples;
}


