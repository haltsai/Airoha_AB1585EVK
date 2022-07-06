/* Copyright Statement:
 *
 * (C) 2018  Airoha Technology Corp. All rights reserved.
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


/* Includes ------------------------------------------------------------------*/

#include "bt_sink_srv_ami.h"
#include "linein_playback.h"
#include "hal_audio_message_struct.h"
#include "hal_audio_internal.h"
#include "hal_audio_cm4_dsp_message.h"
#include "syslog.h"
#include "hal_audio.h"
//#include "hal_clock_platform.h"
extern hal_audio_interface_t hal_audio_convert_linein_interface(uint8_t Mic_NVkey, bool is_input_device);
#if defined(MTK_AVM_DIRECT)
extern HAL_DSP_PARA_AU_AFE_CTRL_t audio_nvdm_HW_config;
#endif

log_create_module(linein, PRINT_LEVEL_INFO);
#define LINEIN_PLAYBACK_USE_MSGID_LOG
#ifdef LINEIN_PLAYBACK_USE_MSGID_LOG
#define LINEIN_PLAYBACK_LOG_E(fmt,arg...)   LOG_MSGID_E(linein, "Linein : "fmt,##arg)
#define LINEIN_PLAYBACK_LOG_W(fmt,arg...)   LOG_MSGID_W(linein, "Linein : "fmt,##arg)
#define LINEIN_PLAYBACK_LOG_I(fmt,arg...)   LOG_MSGID_I(linein ,"Linein : "fmt,##arg)
#else
#define LINEIN_PLAYBACK_LOG_E(fmt,arg...)   LOG_E(linein, "Linein : "fmt,##arg)
#define LINEIN_PLAYBACK_LOG_W(fmt,arg...)   LOG_W(linein, "Linein : "fmt,##arg)
#define LINEIN_PLAYBACK_LOG_I(fmt,arg...)   LOG_I(linein ,"Linein : "fmt,##arg)
#endif

static linein_playback_state_t linein_status;

#if 0
static uint32_t sampling_rate_enum_to_value(hal_audio_sampling_rate_t hal_audio_sampling_rate_enum)
{
    switch (hal_audio_sampling_rate_enum) {
        case HAL_AUDIO_SAMPLING_RATE_8KHZ:
            return   8000;
        case HAL_AUDIO_SAMPLING_RATE_11_025KHZ:
            return  11025;
        case HAL_AUDIO_SAMPLING_RATE_12KHZ:
            return  12000;
        case HAL_AUDIO_SAMPLING_RATE_16KHZ:
            return  16000;
        case HAL_AUDIO_SAMPLING_RATE_22_05KHZ:
            return  22050;
        case HAL_AUDIO_SAMPLING_RATE_24KHZ:
            return  24000;
        case HAL_AUDIO_SAMPLING_RATE_32KHZ:
            return  32000;
        case HAL_AUDIO_SAMPLING_RATE_44_1KHZ:
            return  44100;
        case HAL_AUDIO_SAMPLING_RATE_48KHZ:
            return  48000;
        case HAL_AUDIO_SAMPLING_RATE_88_2KHZ:
            return  88200;
        case HAL_AUDIO_SAMPLING_RATE_96KHZ:
            return  96000;
        case HAL_AUDIO_SAMPLING_RATE_176_4KHZ:
            return 176400;
        case HAL_AUDIO_SAMPLING_RATE_192KHZ:
            return 192000;

        default:
            return 8000;
    }
}
#endif

#ifdef AIR_LE_AUDIO_ENABLE
linein_result_t linein_playback_le_audio_open(hal_audio_sampling_rate_t linein_sample_rate, hal_audio_sampling_rate_t dacout_sample_rate, uint16_t frame_sample_count)
{
    hal_audio_device_t in_audio_device  = HAL_AUDIO_DEVICE_LINEINPLAYBACK_DUAL;
    hal_audio_device_t out_audio_device = HAL_AUDIO_DEVICE_DAC_DUAL;

    LINEIN_PLAYBACK_LOG_I("enter linein_playback_le_audio_open, in rate=%d, out rate=%d, in device=0x%x, out device=0x%x, sample count=%d\n",
                          5, hal_audio_sampling_rate_enum_to_value(linein_sample_rate), hal_audio_sampling_rate_enum_to_value(dacout_sample_rate), in_audio_device, out_audio_device, frame_sample_count);

    if (linein_status != LINEIN_STATE_IDLE) {
        LINEIN_PLAYBACK_LOG_I("cannot open because state(%d)\n", 1, linein_status);
        return LINEIN_EXECUTION_FAIL;
    }

//    if (linein_sample_rate != HAL_AUDIO_SAMPLING_RATE_48KHZ && linein_sample_rate != HAL_AUDIO_SAMPLING_RATE_96KHZ && linein_sample_rate != HAL_AUDIO_SAMPLING_RATE_192KHZ) {
//        LINEIN_PLAYBACK_LOG_I("not support sample rate=%d\n", 1, hal_audio_sampling_rate_enum_to_value(linein_sample_rate));
//        return LINEIN_EXECUTION_FAIL;
//    }

    mcu2dsp_open_param_t open_param = {{0}};
    void *p_param_share;

    open_param.param.stream_in = STREAM_IN_AFE;
    open_param.param.stream_out = STREAM_OUT_AFE;
    open_param.audio_scenario_type = AUDIO_SCENARIO_TYPE_LINE_IN;


//#if defined (HAL_DVFS_MODULE_ENABLED)
//    hal_dvfs_lock_control(HAL_DVFS_MAX_SPEED, HAL_DVFS_LOCK);
//#endif

    open_param.stream_in_param.afe.audio_device = in_audio_device;
    open_param.stream_in_param.afe.stream_channel = HAL_AUDIO_DIRECT;
    open_param.stream_in_param.afe.memory = HAL_AUDIO_MEM1 ;
    open_param.stream_in_param.afe.audio_interface = HAL_AUDIO_INTERFACE_1;
    open_param.stream_in_param.afe.format = AFE_PCM_FORMAT_S16_LE;
    open_param.stream_in_param.afe.sampling_rate = hal_audio_sampling_rate_enum_to_value(linein_sample_rate);
    open_param.stream_in_param.afe.irq_period = 0;
    open_param.stream_in_param.afe.frame_size = frame_sample_count;
    open_param.stream_in_param.afe.frame_number = 4;
    open_param.stream_in_param.afe.hw_gain = false;
    open_param.stream_in_param.afe.adc_mode = HAL_AUDIO_ANALOG_INPUT_ACC10K;
    open_param.stream_in_param.afe.misc_parms      = MICBIAS_SOURCE_ALL | MICBIAS3V_OUTVOLTAGE_2p4v;
    open_param.stream_in_param.afe.ul_adc_mode[0] = 0;// ACC_10k

    open_param.stream_out_param.afe.audio_device = out_audio_device;
    open_param.stream_out_param.afe.stream_channel = HAL_AUDIO_DIRECT;
    open_param.stream_out_param.afe.memory = HAL_AUDIO_MEM1;
    open_param.stream_out_param.afe.audio_interface = HAL_AUDIO_INTERFACE_1;
    open_param.stream_out_param.afe.format = AFE_PCM_FORMAT_S16_LE;
    open_param.stream_out_param.afe.stream_out_sampling_rate = hal_audio_sampling_rate_enum_to_value(dacout_sample_rate);
#if defined (FIXED_SAMPLING_RATE_TO_48KHZ)
    open_param.stream_out_param.afe.sampling_rate   = HAL_AUDIO_FIXED_AFE_48K_SAMPLE_RATE;
#elif defined (AIR_FIXED_DL_SAMPLING_RATE_TO_96KHZ)
    open_param.stream_out_param.afe.sampling_rate   = HAL_AUDIO_FIXED_AFE_96K_SAMPLE_RATE;
#else
    open_param.stream_out_param.afe.sampling_rate = hal_audio_sampling_rate_enum_to_value(dacout_sample_rate);
#endif
    open_param.stream_out_param.afe.irq_period = 0;
    open_param.stream_out_param.afe.frame_size = frame_sample_count;
    open_param.stream_out_param.afe.frame_number = 4;
    open_param.stream_out_param.afe.hw_gain = true;
    open_param.stream_out_param.afe.adc_mode = HAL_AUDIO_ANALOG_OUTPUT_CLASSAB;
    open_param.stream_out_param.afe.misc_parms      = DOWNLINK_PERFORMANCE_NORMAL;
    open_param.stream_out_param.afe.dl_dac_mode = 0;//Class_G

#ifdef AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
    open_param.stream_in_param.afe.audio_device1 = in_audio_device;
    open_param.stream_in_param.afe.audio_interface1 = HAL_AUDIO_INTERFACE_1;
    open_param.stream_out_param.afe.audio_device1 = out_audio_device;
    open_param.stream_out_param.afe.audio_interface1 = HAL_AUDIO_INTERFACE_1;
#endif
#endif


#ifdef ENABLE_HWSRC_CLKSKEW
    open_param.stream_in_param.afe.clkskew_mode  = CLK_SKEW_V1;
    open_param.stream_out_param.afe.clkskew_mode  = CLK_SKEW_V1;
#endif

#if defined(MTK_EXTERNAL_DSP_NEED_SUPPORT)
    ami_set_afe_param(STREAM_OUT, linein_sample_rate, true);
#endif
    p_param_share = hal_audio_dsp_controller_put_paramter(&open_param, sizeof(mcu2dsp_open_param_t), AUDIO_MESSAGE_TYPE_LINEIN);
    ami_hal_audio_status_set_running_flag(AUDIO_SCENARIO_TYPE_LINE_IN, &open_param, true);
    hal_audio_dsp_controller_send_message(MSG_MCU2DSP_LINEIN_PLAYBACK_OPEN, 0, (uint32_t)p_param_share, true);

    hal_audio_set_stream_in_volume(0, 0);
    hal_audio_set_stream_out_volume(HAL_AUDIO_STREAM_OUT1, 0, 0);

    linein_status = LINEIN_STATE_READY;
    return LINEIN_EXECUTION_SUCCESS;
}
#endif

linein_result_t linein_playback_open(hal_audio_sampling_rate_t linein_sample_rate, hal_audio_device_t in_audio_device, hal_audio_device_t out_audio_device)
{
    LINEIN_PLAYBACK_LOG_I("enter linein_playback_open, sample_rate=%d, in_audio_device=0x%x, out_audio_device=0x%x\n", 3, hal_audio_sampling_rate_enum_to_value(linein_sample_rate), in_audio_device, out_audio_device);
    if (linein_status != LINEIN_STATE_IDLE) {
        LINEIN_PLAYBACK_LOG_I("cannot open because state(%d)\n", 1, linein_status);
        return LINEIN_EXECUTION_FAIL;
    }

    if (linein_sample_rate != HAL_AUDIO_SAMPLING_RATE_48KHZ && linein_sample_rate != HAL_AUDIO_SAMPLING_RATE_96KHZ && linein_sample_rate != HAL_AUDIO_SAMPLING_RATE_192KHZ) {
        LINEIN_PLAYBACK_LOG_I("not support sample rate=%d\n", 1, hal_audio_sampling_rate_enum_to_value(linein_sample_rate));
        return LINEIN_EXECUTION_FAIL;
    }
    if (in_audio_device != HAL_AUDIO_DEVICE_I2S_MASTER && in_audio_device != HAL_AUDIO_DEVICE_I2S_MASTER_L && in_audio_device != HAL_AUDIO_DEVICE_I2S_MASTER_R
        && in_audio_device != HAL_AUDIO_DEVICE_I2S_SLAVE && ((in_audio_device & HAL_AUDIO_DEVICE_MAIN_MIC_DUAL) == false)
        && ((in_audio_device & HAL_AUDIO_DEVICE_LINEINPLAYBACK_DUAL) == false) && ((in_audio_device & HAL_AUDIO_DEVICE_DIGITAL_MIC_DUAL) == false)) {
        LINEIN_PLAYBACK_LOG_I("not support in device=%d\n", 1, in_audio_device);
        return LINEIN_EXECUTION_FAIL;
    }
    if (out_audio_device != HAL_AUDIO_DEVICE_I2S_MASTER && out_audio_device != HAL_AUDIO_DEVICE_I2S_SLAVE && ((out_audio_device & HAL_AUDIO_DEVICE_DAC_DUAL) == false)) {
        LINEIN_PLAYBACK_LOG_I("not support out device=%d\n", 1, out_audio_device);
        return LINEIN_EXECUTION_FAIL;
    }

    mcu2dsp_open_param_t open_param;
    memset(&open_param, 0, sizeof(mcu2dsp_open_param_t));
    void *p_param_share;

    open_param.param.stream_in = STREAM_IN_AFE;
    open_param.param.stream_out = STREAM_OUT_AFE;
    open_param.audio_scenario_type = AUDIO_SCENARIO_TYPE_LINE_IN;

    open_param.stream_in_param.afe.audio_device = in_audio_device;
#ifdef AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
    open_param.stream_in_param.afe.audio_device1 = in_audio_device;
#endif
#endif

#ifndef AIR_BTA_IC_PREMIUM_G2
    if (in_audio_device & HAL_AUDIO_DEVICE_LINEINPLAYBACK_DUAL) {
        open_param.stream_in_param.afe.audio_device = HAL_AUDIO_DEVICE_LINEINPLAYBACK_DUAL;
#ifdef AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
        open_param.stream_in_param.afe.audio_device1 = HAL_AUDIO_DEVICE_LINEINPLAYBACK_DUAL;
#endif
#endif

    } else if (in_audio_device & (HAL_AUDIO_DEVICE_I2S_MASTER|HAL_AUDIO_DEVICE_I2S_MASTER_L|HAL_AUDIO_DEVICE_I2S_MASTER_R)) {
        open_param.stream_in_param.afe.audio_device = HAL_AUDIO_DEVICE_I2S_MASTER_L;
#ifdef AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
        open_param.stream_in_param.afe.audio_device1 = HAL_AUDIO_DEVICE_I2S_MASTER_R;
#endif
#endif
    }
#endif

    open_param.stream_in_param.afe.stream_channel = HAL_AUDIO_DIRECT;
    open_param.stream_in_param.afe.memory = HAL_AUDIO_MEM1 ;
#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
    open_param.stream_in_param.afe.audio_interface = hal_audio_convert_linein_interface(audio_nvdm_HW_config.audio_scenario.Audio_Linein_Input_Path, true);
#else
    open_param.stream_in_param.afe.audio_interface = HAL_AUDIO_INTERFACE_2;
#endif

#ifdef AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
    open_param.stream_in_param.afe.audio_interface1 = hal_audio_convert_linein_interface(audio_nvdm_HW_config.audio_scenario.Audio_Linein_Input_Path, true);
#endif
#endif
    open_param.stream_in_param.afe.format = AFE_PCM_FORMAT_S32_LE;
    open_param.stream_in_param.afe.sampling_rate = hal_audio_sampling_rate_enum_to_value(linein_sample_rate);
    //open_param.stream_in_param.afe.irq_period = 512 / (hal_audio_sampling_rate_enum_to_value(linein_sample_rate)/1000);
    open_param.stream_in_param.afe.irq_period = 0;
    open_param.stream_in_param.afe.frame_size = 512;
    open_param.stream_in_param.afe.frame_number = 4;
    open_param.stream_in_param.afe.hw_gain = false;

#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
    //Linein parameters
    open_param.stream_in_param.afe.performance = audio_nvdm_HW_config.audio_scenario.Audio_Analog_LineIn_Performance_Sel;
    open_param.stream_in_param.afe.iir_filter[0] = audio_nvdm_HW_config.adc_dac_config.ADDA_Audio_IIR_Filter;
    open_param.stream_in_param.afe.ul_adc_mode[0] = audio_nvdm_HW_config.adc_dac_config.ADDA_Analog_MIC0_Mode;

    //I2S Slave parameters
    open_param.stream_in_param.afe.i2s_format = audio_nvdm_HW_config.I2SS_config.I2S_Slave_Format;
    open_param.stream_in_param.afe.i2s_word_length = audio_nvdm_HW_config.I2SS_config.I2S_Slave_Word_length;

    //I2S Master parameters
    open_param.stream_in_param.afe.i2s_master_sampling_rate[0] = audio_nvdm_HW_config.audio_scenario.Audio_I2S0_Master_Sampling_Rate;
    open_param.stream_in_param.afe.i2s_master_sampling_rate[1] = audio_nvdm_HW_config.audio_scenario.Audio_I2S1_Master_Sampling_Rate;
    open_param.stream_in_param.afe.i2s_master_sampling_rate[2] = audio_nvdm_HW_config.audio_scenario.Audio_I2S2_Master_Sampling_Rate;
    open_param.stream_in_param.afe.i2s_master_sampling_rate[3] = audio_nvdm_HW_config.audio_scenario.Audio_I2S3_Master_Sampling_Rate;
    open_param.stream_in_param.afe.is_low_jitter[0] = audio_nvdm_HW_config.I2SM_config.I2S0_Master_Low_jitter;
    open_param.stream_in_param.afe.is_low_jitter[1] = audio_nvdm_HW_config.I2SM_config.I2S1_Master_Low_jitter;
    open_param.stream_in_param.afe.is_low_jitter[2] = audio_nvdm_HW_config.I2SM_config.I2S2_Master_Low_jitter;
    open_param.stream_in_param.afe.is_low_jitter[3] = audio_nvdm_HW_config.I2SM_config.I2S3_Master_Low_jitter;
    open_param.stream_in_param.afe.i2s_master_format[0] = audio_nvdm_HW_config.I2SM_config.I2S0_Master_Format;
    open_param.stream_in_param.afe.i2s_master_format[1] = audio_nvdm_HW_config.I2SM_config.I2S1_Master_Format;
    open_param.stream_in_param.afe.i2s_master_format[2] = audio_nvdm_HW_config.I2SM_config.I2S2_Master_Format;
    open_param.stream_in_param.afe.i2s_master_format[3] = audio_nvdm_HW_config.I2SM_config.I2S3_Master_Format;
    open_param.stream_in_param.afe.i2s_master_word_length[0] = audio_nvdm_HW_config.I2SM_config.I2S0_Master_Word_length;
    open_param.stream_in_param.afe.i2s_master_word_length[1] = audio_nvdm_HW_config.I2SM_config.I2S1_Master_Word_length;
    open_param.stream_in_param.afe.i2s_master_word_length[2] = audio_nvdm_HW_config.I2SM_config.I2S2_Master_Word_length;
    open_param.stream_in_param.afe.i2s_master_word_length[3] = audio_nvdm_HW_config.I2SM_config.I2S3_Master_Word_length;

#endif

    if (open_param.stream_in_param.afe.audio_device == HAL_AUDIO_DEVICE_I2S_MASTER) {
        open_param.stream_in_param.afe.misc_parms      = I2S_CLK_SOURCE_DCXO;
        if (linein_sample_rate > HAL_AUDIO_SAMPLING_RATE_48KHZ) {
            open_param.stream_in_param.afe.misc_parms  = I2S_CLK_SOURCE_APLL;
        }
    } else {
        if (in_audio_device & HAL_AUDIO_DEVICE_LINEINPLAYBACK_DUAL) {
            open_param.stream_in_param.afe.misc_parms      = MICBIAS_SOURCE_ALL | MICBIAS3V_OUTVOLTAGE_2p4v;
        } else {
            open_param.stream_in_param.afe.misc_parms      = MICBIAS_SOURCE_ALL | MICBIAS3V_OUTVOLTAGE_1p85v;
        }
    }

    open_param.stream_out_param.afe.audio_device = out_audio_device;
    open_param.stream_out_param.afe.stream_channel = HAL_AUDIO_DIRECT;
    open_param.stream_out_param.afe.memory = HAL_AUDIO_MEM1;
    open_param.stream_out_param.afe.audio_interface = HAL_AUDIO_INTERFACE_1;
    open_param.stream_out_param.afe.format = AFE_PCM_FORMAT_S32_LE;
    open_param.stream_out_param.afe.stream_out_sampling_rate = hal_audio_sampling_rate_enum_to_value(linein_sample_rate);
#if defined (FIXED_SAMPLING_RATE_TO_48KHZ)
    open_param.stream_out_param.afe.sampling_rate   = HAL_AUDIO_FIXED_AFE_48K_SAMPLE_RATE;
#elif defined (AIR_FIXED_DL_SAMPLING_RATE_TO_96KHZ)
    open_param.stream_out_param.afe.sampling_rate   = HAL_AUDIO_FIXED_AFE_96K_SAMPLE_RATE;
#else
    open_param.stream_out_param.afe.sampling_rate   = hal_audio_sampling_rate_enum_to_value(linein_sample_rate);
#endif
    //open_param.stream_out_param.afe.irq_period = 512 / (hal_audio_sampling_rate_enum_to_value(linein_sample_rate)/1000);
    open_param.stream_out_param.afe.irq_period = 0;
    open_param.stream_out_param.afe.frame_size = 512;
    open_param.stream_out_param.afe.frame_number = 4;
    open_param.stream_out_param.afe.hw_gain = true;
#ifdef ENABLE_HWSRC_CLKSKEW
    open_param.stream_out_param.afe.clkskew_mode = CLK_SKEW_V1;
#endif

#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
    open_param.stream_out_param.afe.performance = audio_nvdm_HW_config.adc_dac_config.ADDA_DAC_Class_AB_G_Performance_Sel;
    switch (audio_nvdm_HW_config.adc_dac_config.ADDA_DAC_Mode_Sel) {
        case 0x00: //Class G
        case 0x01: //Class AB
            open_param.stream_out_param.afe.dl_dac_mode = HAL_AUDIO_ANALOG_OUTPUT_CLASSAB;
            break;
        case 0x02: //Class D
            open_param.stream_out_param.afe.dl_dac_mode = HAL_AUDIO_ANALOG_OUTPUT_CLASSD;
            break;
        default:
            open_param.stream_out_param.afe.dl_dac_mode = HAL_AUDIO_ANALOG_OUTPUT_CLASSAB;
            break;
    }

    //I2S Slave parameters
    open_param.stream_out_param.afe.i2s_format = audio_nvdm_HW_config.I2SS_config.I2S_Slave_Format;
    open_param.stream_out_param.afe.i2s_word_length = audio_nvdm_HW_config.I2SS_config.I2S_Slave_Word_length;

    //I2S Master parameters
    open_param.stream_out_param.afe.i2s_master_sampling_rate[0] = audio_nvdm_HW_config.audio_scenario.Audio_I2S0_Master_Sampling_Rate;
    open_param.stream_out_param.afe.i2s_master_sampling_rate[1] = audio_nvdm_HW_config.audio_scenario.Audio_I2S1_Master_Sampling_Rate;
    open_param.stream_out_param.afe.i2s_master_sampling_rate[2] = audio_nvdm_HW_config.audio_scenario.Audio_I2S2_Master_Sampling_Rate;
    open_param.stream_out_param.afe.i2s_master_sampling_rate[3] = audio_nvdm_HW_config.audio_scenario.Audio_I2S3_Master_Sampling_Rate;
    open_param.stream_out_param.afe.is_low_jitter[0] = audio_nvdm_HW_config.I2SM_config.I2S0_Master_Low_jitter;
    open_param.stream_out_param.afe.is_low_jitter[1] = audio_nvdm_HW_config.I2SM_config.I2S1_Master_Low_jitter;
    open_param.stream_out_param.afe.is_low_jitter[2] = audio_nvdm_HW_config.I2SM_config.I2S2_Master_Low_jitter;
    open_param.stream_out_param.afe.is_low_jitter[3] = audio_nvdm_HW_config.I2SM_config.I2S3_Master_Low_jitter;
    open_param.stream_out_param.afe.i2s_master_format[0] = audio_nvdm_HW_config.I2SM_config.I2S0_Master_Format;
    open_param.stream_out_param.afe.i2s_master_format[1] = audio_nvdm_HW_config.I2SM_config.I2S1_Master_Format;
    open_param.stream_out_param.afe.i2s_master_format[2] = audio_nvdm_HW_config.I2SM_config.I2S2_Master_Format;
    open_param.stream_out_param.afe.i2s_master_format[3] = audio_nvdm_HW_config.I2SM_config.I2S3_Master_Format;
    open_param.stream_out_param.afe.i2s_master_word_length[0] = audio_nvdm_HW_config.I2SM_config.I2S0_Master_Word_length;
    open_param.stream_out_param.afe.i2s_master_word_length[1] = audio_nvdm_HW_config.I2SM_config.I2S1_Master_Word_length;
    open_param.stream_out_param.afe.i2s_master_word_length[2] = audio_nvdm_HW_config.I2SM_config.I2S2_Master_Word_length;
    open_param.stream_out_param.afe.i2s_master_word_length[3] = audio_nvdm_HW_config.I2SM_config.I2S3_Master_Word_length;
#endif
    if (open_param.stream_out_param.afe.audio_device == HAL_AUDIO_DEVICE_I2S_MASTER) {
        open_param.stream_out_param.afe.misc_parms      = I2S_CLK_SOURCE_DCXO;
        if (linein_sample_rate > HAL_AUDIO_SAMPLING_RATE_48KHZ) {
            open_param.stream_out_param.afe.misc_parms  = I2S_CLK_SOURCE_APLL;
        }
    } else {
        open_param.stream_out_param.afe.misc_parms      = DOWNLINK_PERFORMANCE_NORMAL;
    }

#if defined(MTK_EXTERNAL_DSP_NEED_SUPPORT)
    ami_set_afe_param(STREAM_OUT, linein_sample_rate, true);
#endif
    p_param_share = hal_audio_dsp_controller_put_paramter(&open_param, sizeof(mcu2dsp_open_param_t), AUDIO_MESSAGE_TYPE_LINEIN);
    ami_hal_audio_status_set_running_flag(AUDIO_SCENARIO_TYPE_LINE_IN, &open_param, true);
    hal_audio_dsp_controller_send_message(MSG_MCU2DSP_LINEIN_PLAYBACK_OPEN, 0, (uint32_t)p_param_share, true);

    linein_status = LINEIN_STATE_READY;
    return LINEIN_EXECUTION_SUCCESS;
}

linein_result_t linein_playback_start()
{
    LINEIN_PLAYBACK_LOG_I("enter linein_playback_start\n", 0);
    if (linein_status != LINEIN_STATE_READY) {
        LINEIN_PLAYBACK_LOG_I("cannot start because state(%d)\n", 1, linein_status);
        return LINEIN_EXECUTION_FAIL;
    }

    mcu2dsp_start_param_t start_param = {{0}};
    void *p_param_share;

    // Collect parameters
    start_param.param.stream_in     = STREAM_IN_AFE;
    start_param.param.stream_out    = STREAM_OUT_AFE;
    start_param.stream_in_param.afe.aws_flag   =  false;
    start_param.stream_out_param.afe.aws_flag   =  false;
    p_param_share = hal_audio_dsp_controller_put_paramter(&start_param, sizeof(mcu2dsp_start_param_t), AUDIO_MESSAGE_TYPE_LINEIN);
    hal_audio_dsp_controller_send_message(MSG_MCU2DSP_LINEIN_PLAYBACK_START, 0, (uint32_t)p_param_share, true);

    linein_status = LINEIN_STATE_PLAY;
    return LINEIN_EXECUTION_SUCCESS;
}

linein_result_t linein_playback_stop()
{
    LINEIN_PLAYBACK_LOG_I("enter linein_playback_stop\n", 0);
    if (linein_status != LINEIN_STATE_PLAY) {
        LINEIN_PLAYBACK_LOG_I("cannot stop because state(%d)\n", 1, linein_status);
        return LINEIN_EXECUTION_FAIL;
    }
    hal_audio_dsp_controller_send_message(MSG_MCU2DSP_LINEIN_PLAYBACK_STOP, 0, 0, true);

    linein_status = LINEIN_STATE_STOP;
    return LINEIN_EXECUTION_SUCCESS;
}

linein_result_t linein_playback_close()
{
    LINEIN_PLAYBACK_LOG_I("enter linein_playback_close\n", 0);
    if (linein_status != LINEIN_STATE_STOP) {
        LINEIN_PLAYBACK_LOG_I("cannot close because state(%d)\n", 1, linein_status);
        return LINEIN_EXECUTION_FAIL;
    }
    hal_audio_dsp_controller_send_message(MSG_MCU2DSP_LINEIN_PLAYBACK_CLOSE, 0, 0, true);

    linein_status = LINEIN_STATE_IDLE;
    ami_hal_audio_status_set_running_flag(AUDIO_SCENARIO_TYPE_LINE_IN, NULL, false);
    return LINEIN_EXECUTION_SUCCESS;
}

linein_result_t linein_playback_set_volume(linein_playback_gain_t gain)
{
    LINEIN_PLAYBACK_LOG_I("enter linein_playback_set_volume\n", 0);
    LINEIN_PLAYBACK_LOG_I("input_digital_gain=%d input_analog_gain=%d\n", 2, gain.in_digital_gain, gain.in_analog_gain);
    LINEIN_PLAYBACK_LOG_I("output_digital_gain=%d output_analog_gain=%d\n", 2, gain.out_digital_gain, gain.out_analog_gain);
    hal_audio_set_stream_in_volume(gain.in_digital_gain, gain.in_analog_gain);
    hal_audio_set_stream_out_volume(HAL_AUDIO_STREAM_OUT1, gain.out_digital_gain, gain.out_analog_gain);

    return LINEIN_EXECUTION_SUCCESS;
}
#ifdef LINE_IN_PURE_FOR_AMIC_CLASS_G_HQA
linein_result_t pure_linein_playback_open_HQA(hal_audio_sampling_rate_t linein_sample_rate, hal_audio_device_t in_audio_device, hal_audio_interface_t device_in_interface_HQA, hal_audio_analog_mdoe_t adc_mode_HQA, hal_audio_performance_mode_t mic_performance_HQA, hal_audio_device_t out_audio_device, hal_audio_performance_mode_t dac_performance_HQA)
{
    if (audio_nvdm_HW_config.adc_dac_config.ADDA_DAC_Mode_Sel == 0x0) {
        if (dac_performance_HQA == AFE_PEROFRMANCE_LOW_POWER_MODE) {
            audio_nvdm_HW_config.adc_dac_config.ADDA_DAC_Mode_Sel = 0x3;
        }
    }

    LINEIN_PLAYBACK_LOG_I("enter pure_linein_playback_open_HQA, sample_rate=%d, in_audio_device=0x%x, out_audio_device=0x%x\n", 3, hal_audio_sampling_rate_enum_to_value(linein_sample_rate), in_audio_device, out_audio_device);
    if (linein_status != LINEIN_STATE_IDLE) {
        LINEIN_PLAYBACK_LOG_I("cannot open because state(%d)\n", 1, linein_status);
        return LINEIN_EXECUTION_FAIL;
    }

    if (linein_sample_rate != HAL_AUDIO_SAMPLING_RATE_16KHZ && linein_sample_rate != HAL_AUDIO_SAMPLING_RATE_48KHZ && linein_sample_rate != HAL_AUDIO_SAMPLING_RATE_96KHZ && linein_sample_rate != HAL_AUDIO_SAMPLING_RATE_192KHZ) {
        LINEIN_PLAYBACK_LOG_I("not support sample rate=%d\n", 1, hal_audio_sampling_rate_enum_to_value(linein_sample_rate));
        return LINEIN_EXECUTION_FAIL;
    }
    if (in_audio_device != HAL_AUDIO_DEVICE_I2S_MASTER && in_audio_device != HAL_AUDIO_DEVICE_I2S_SLAVE && in_audio_device != HAL_AUDIO_DEVICE_SPDIF && ((in_audio_device & HAL_AUDIO_DEVICE_MAIN_MIC_DUAL) == false)
        && ((in_audio_device & HAL_AUDIO_DEVICE_LINEINPLAYBACK_DUAL) == false) && ((in_audio_device & HAL_AUDIO_DEVICE_DIGITAL_MIC_DUAL) == false)) {
        LINEIN_PLAYBACK_LOG_I("not support in device=%d\n", 1, in_audio_device);
        return LINEIN_EXECUTION_FAIL;
    }

    if (out_audio_device != HAL_AUDIO_DEVICE_I2S_MASTER && out_audio_device != HAL_AUDIO_DEVICE_I2S_SLAVE && out_audio_device != HAL_AUDIO_DEVICE_SPDIF && ((out_audio_device & HAL_AUDIO_DEVICE_DAC_DUAL) == false)) {
        LINEIN_PLAYBACK_LOG_I("not support out device=%d\n", 1, out_audio_device);
        return LINEIN_EXECUTION_FAIL;
    }

    mcu2dsp_open_param_t open_param;
    void *p_param_share;

    open_param.param.stream_in = STREAM_IN_AFE;
    open_param.param.stream_out = STREAM_OUT_AFE;
    open_param.audio_scenario_type = AUDIO_SCENARIO_TYPE_LINE_IN;

    open_param.stream_in_param.afe.audio_interface = device_in_interface_HQA;

#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
    open_param.stream_in_param.afe.performance = mic_performance_HQA;
    open_param.stream_in_param.afe.bias_voltage[0] = HAL_AUDIO_BIAS_VOLTAGE_2_40V;
    open_param.stream_in_param.afe.bias_voltage[1] = HAL_AUDIO_BIAS_VOLTAGE_2_40V;
    open_param.stream_in_param.afe.bias_voltage[2] = HAL_AUDIO_BIAS_VOLTAGE_2_40V;
    open_param.stream_in_param.afe.bias_voltage[3] = HAL_AUDIO_BIAS_VOLTAGE_2_40V;
    open_param.stream_in_param.afe.bias_voltage[4] = HAL_AUDIO_BIAS_VOLTAGE_2_40V;
    open_param.stream_in_param.afe.bias_select = audio_nvdm_HW_config.voice_scenario.Voice_MIC_Bias_Enable;
    open_param.stream_in_param.afe.iir_filter[0] = HAL_AUDIO_UL_IIR_DISABLE;
#ifdef AIR_BTA_IC_PREMIUM_G2
    if ((adc_mode_HQA == HAL_AUDIO_ANALOG_INPUT_ACC10K)) {
#else //AIR_BTA_IC_PREMIUM_G3
    if ((adc_mode_HQA == HAL_AUDIO_ANALOG_INPUT_ACC10K) || (adc_mode_HQA == HAL_AUDIO_ANALOG_INPUT_ACC10K_SINGLE)) {
#endif
        open_param.stream_in_param.afe.adc_mode = HAL_AUDIO_ANALOG_INPUT_ACC10K;
#ifdef AIR_BTA_IC_PREMIUM_G2
    } else if ((adc_mode_HQA == HAL_AUDIO_ANALOG_INPUT_ACC20K)) {
#else //AIR_BTA_IC_PREMIUM_G3
    } else if ((adc_mode_HQA == HAL_AUDIO_ANALOG_INPUT_ACC20K) || (adc_mode_HQA == HAL_AUDIO_ANALOG_INPUT_ACC20K_SINGLE)) {
#endif
        open_param.stream_in_param.afe.adc_mode = HAL_AUDIO_ANALOG_INPUT_ACC20K;
    }
#ifndef AIR_BTA_IC_PREMIUM_G2 //AIR_BTA_IC_PREMIUM_G3
    if ((adc_mode_HQA == HAL_AUDIO_ANALOG_INPUT_ACC10K) || (adc_mode_HQA == HAL_AUDIO_ANALOG_INPUT_ACC20K)) {
        open_param.stream_in_param.afe.adc_type = HAL_AUDIO_ANALOG_TYPE_DIFF;
    } else if ((adc_mode_HQA == HAL_AUDIO_ANALOG_INPUT_ACC10K_SINGLE) || (adc_mode_HQA == HAL_AUDIO_ANALOG_INPUT_ACC20K_SINGLE)) {
        open_param.stream_in_param.afe.adc_type = HAL_AUDIO_ANALOG_TYPE_SINGLE;
    }
#endif
    open_param.stream_in_param.afe.dmic_selection[0] = HAL_AUDIO_DMIC_GPIO_DMIC0;
    if ((in_audio_device == HAL_AUDIO_DEVICE_DIGITAL_MIC_DUAL)) {
        if ((int)adc_mode_HQA == (int)HAL_AUDIO_DMIC_GPIO_DMIC0) {
            open_param.stream_in_param.afe.dmic_selection[0] = HAL_AUDIO_DMIC_GPIO_DMIC0;
        } else if ((int)adc_mode_HQA == (int)HAL_AUDIO_DMIC_GPIO_DMIC1) {
            open_param.stream_in_param.afe.dmic_selection[0] = HAL_AUDIO_DMIC_GPIO_DMIC1;
        } else if ((int)adc_mode_HQA == (int)HAL_AUDIO_DMIC_ANA_DMIC0) {
            open_param.stream_in_param.afe.dmic_selection[0] = HAL_AUDIO_DMIC_ANA_DMIC0;
        } else if ((int)adc_mode_HQA == (int)HAL_AUDIO_DMIC_ANA_DMIC1) {
            open_param.stream_in_param.afe.dmic_selection[0] = HAL_AUDIO_DMIC_ANA_DMIC1;
        } else if ((int)adc_mode_HQA == (int)HAL_AUDIO_DMIC_ANA_DMIC2) {
            open_param.stream_in_param.afe.dmic_selection[0] = HAL_AUDIO_DMIC_ANA_DMIC2;
        } else if ((int)adc_mode_HQA == (int)HAL_AUDIO_DMIC_ANA_DMIC3) {
            open_param.stream_in_param.afe.dmic_selection[0] = HAL_AUDIO_DMIC_ANA_DMIC3;
        }
        LINEIN_PLAYBACK_LOG_I("[Dmic_HQA] adc_mode/DMIC_pin %d, dmic_selection %d", 2, adc_mode_HQA, open_param.stream_in_param.afe.dmic_selection[0]);
    }
#endif

    open_param.stream_in_param.afe.audio_device = in_audio_device;
    if (open_param.stream_in_param.afe.audio_device == HAL_AUDIO_DEVICE_I2S_MASTER) {
        open_param.stream_in_param.afe.misc_parms      = I2S_CLK_SOURCE_DCXO;
        if (linein_sample_rate > HAL_AUDIO_SAMPLING_RATE_48KHZ) {
            open_param.stream_in_param.afe.misc_parms  = I2S_CLK_SOURCE_APLL;
        }
    } else {
        if (in_audio_device & HAL_AUDIO_DEVICE_LINEINPLAYBACK_DUAL) {
            open_param.stream_in_param.afe.misc_parms      = MICBIAS_SOURCE_ALL | MICBIAS3V_OUTVOLTAGE_2p4v;
        } else {
            open_param.stream_in_param.afe.misc_parms      = MICBIAS_SOURCE_ALL | MICBIAS3V_OUTVOLTAGE_1p85v;
        }
    }

    open_param.stream_out_param.afe.audio_device = out_audio_device;
    //open_param.stream_out_param.afe.audio_interface = HAL_AUDIO_INTERFACE_1;
    open_param.stream_out_param.afe.audio_interface = device_in_interface_HQA;
    open_param.stream_out_param.afe.stream_channel = HAL_AUDIO_DIRECT;
    open_param.stream_out_param.afe.format = AFE_PCM_FORMAT_S32_LE;
    open_param.stream_out_param.afe.stream_out_sampling_rate = hal_audio_sampling_rate_enum_to_value(linein_sample_rate);
#if defined (FIXED_SAMPLING_RATE_TO_48KHZ)
    open_param.stream_out_param.afe.sampling_rate   = HAL_AUDIO_FIXED_AFE_48K_SAMPLE_RATE;
#elif defined (AIR_FIXED_DL_SAMPLING_RATE_TO_96KHZ)
    open_param.stream_out_param.afe.sampling_rate   = HAL_AUDIO_FIXED_AFE_96K_SAMPLE_RATE;
#else
    open_param.stream_out_param.afe.sampling_rate   = hal_audio_sampling_rate_enum_to_value(linein_sample_rate);
#endif
    open_param.stream_in_param.afe.sampling_rate = open_param.stream_out_param.afe.sampling_rate;
    open_param.stream_out_param.afe.hw_gain = true;

    if (open_param.stream_in_param.afe.audio_device & HAL_AUDIO_DEVICE_I2S_MASTER) {
        if (open_param.stream_in_param.afe.audio_interface & HAL_AUDIO_INTERFACE_1) {
            open_param.stream_in_param.afe.i2s_master_sampling_rate[0] = open_param.stream_in_param.afe.sampling_rate;
        } else if (open_param.stream_in_param.afe.audio_interface & HAL_AUDIO_INTERFACE_2) {
            open_param.stream_in_param.afe.i2s_master_sampling_rate[1] = open_param.stream_in_param.afe.sampling_rate;
        } else if (open_param.stream_in_param.afe.audio_interface & HAL_AUDIO_INTERFACE_3) {
            open_param.stream_in_param.afe.i2s_master_sampling_rate[2] = open_param.stream_in_param.afe.sampling_rate;
        } else if (open_param.stream_in_param.afe.audio_interface & HAL_AUDIO_INTERFACE_4) {
            open_param.stream_in_param.afe.i2s_master_sampling_rate[3] = open_param.stream_in_param.afe.sampling_rate;
        }
    }
    if (open_param.stream_out_param.afe.audio_device & HAL_AUDIO_DEVICE_I2S_MASTER) {
        open_param.stream_out_param.afe.misc_parms      = I2S_CLK_SOURCE_DCXO;
        if (linein_sample_rate > HAL_AUDIO_SAMPLING_RATE_48KHZ) {
            open_param.stream_out_param.afe.misc_parms  = I2S_CLK_SOURCE_APLL;
        }
        if (open_param.stream_out_param.afe.audio_interface & HAL_AUDIO_INTERFACE_1) {
            open_param.stream_out_param.afe.i2s_master_sampling_rate[0] = open_param.stream_out_param.afe.sampling_rate;
        } else if (open_param.stream_out_param.afe.audio_interface & HAL_AUDIO_INTERFACE_2) {
            open_param.stream_out_param.afe.i2s_master_sampling_rate[1] = open_param.stream_out_param.afe.sampling_rate;
        } else if (open_param.stream_out_param.afe.audio_interface & HAL_AUDIO_INTERFACE_3) {
            open_param.stream_out_param.afe.i2s_master_sampling_rate[2] = open_param.stream_out_param.afe.sampling_rate;
        } else if (open_param.stream_out_param.afe.audio_interface & HAL_AUDIO_INTERFACE_4) {
            open_param.stream_out_param.afe.i2s_master_sampling_rate[3] = open_param.stream_out_param.afe.sampling_rate;
        }
    } else {
        open_param.stream_out_param.afe.misc_parms      = DOWNLINK_PERFORMANCE_NORMAL;
    }

    open_param.stream_out_param.afe.performance = dac_performance_HQA;
#if 0
    if ((in_audio_device & HAL_AUDIO_DEVICE_I2S_MASTER) || (out_audio_device & HAL_AUDIO_DEVICE_I2S_MASTER)) {
        if (open_param.stream_in_param.afe.audio_interface == HAL_AUDIO_INTERFACE_1) {
            hal_gpio_init(2);
            hal_pinmux_set_function(2, 3);
            hal_gpio_init(3);
            hal_pinmux_set_function(3, 3);
            hal_gpio_init(4);
            hal_pinmux_set_function(4, 3);
            hal_gpio_init(6);
            hal_pinmux_set_function(6, 3);
            hal_gpio_init(5);
            hal_pinmux_set_function(5, 3); //O:I2S_MST0_MCLK
            //hal_audio_mclk_enable(true, AFE_MCLK_PIN_FROM_I2S0, AFE_APLL2, 0);//enable mclk
            LINEIN_PLAYBACK_LOG_I("I2S0 GPIO Set done\r\n", 0);
        } else if (open_param.stream_in_param.afe.audio_interface == HAL_AUDIO_INTERFACE_2) {
            hal_gpio_init(16);
            hal_pinmux_set_function(16, 3);
            hal_gpio_init(26);
            hal_pinmux_set_function(26, 3);
            hal_gpio_init(17);
            hal_pinmux_set_function(17, 3);
            hal_gpio_init(18);
            hal_pinmux_set_function(18, 3);
            hal_gpio_init(15);
            hal_pinmux_set_function(15, 3); //O:I2S_MST1_MCLK
            //hal_audio_mclk_enable(true, AFE_MCLK_PIN_FROM_I2S1, AFE_APLL2, 0);//enable mclk
            LINEIN_PLAYBACK_LOG_I("I2S1 GPIO Set done\r\n", 0);
        } else if (open_param.stream_in_param.afe.audio_interface == HAL_AUDIO_INTERFACE_3) {
            hal_gpio_init(29);
            hal_pinmux_set_function(29, 1);
            hal_gpio_init(32);
            hal_pinmux_set_function(32, 1);
            hal_gpio_init(33);
            hal_pinmux_set_function(33, 1);
            hal_gpio_init(37);
            hal_pinmux_set_function(37, 1);
            hal_gpio_init(31);
            hal_pinmux_set_function(31, 1); //O:I2S_MST2_MCLK
            //hal_audio_mclk_enable(true, AFE_MCLK_PIN_FROM_I2S2, AFE_APLL2, 0);//enable mclk
            LINEIN_PLAYBACK_LOG_I("I2S2 GPIO Set done\r\n", 0);
        } else if (open_param.stream_in_param.afe.audio_interface == HAL_AUDIO_INTERFACE_4) {
            hal_gpio_init(38);
            hal_pinmux_set_function(38, 1);
            hal_gpio_init(39);
            hal_pinmux_set_function(39, 1);
            hal_gpio_init(40);
            hal_pinmux_set_function(40, 1);
            hal_gpio_init(43);
            hal_pinmux_set_function(43, 1);
            hal_gpio_init(41);
            hal_pinmux_set_function(41, 1); //O:I2S_MST3_MCLK
            //hal_audio_mclk_enable(true, AFE_MCLK_PIN_FROM_I2S3, AFE_APLL2, 0);//enable mclk
            LINEIN_PLAYBACK_LOG_I("I2S3 GPIO Set done\r\n", 0);
        }
    }

    if (in_audio_device & HAL_AUDIO_DEVICE_DIGITAL_MIC_DUAL) {
        if ((uint32_t)adc_mode_HQA == (uint32_t)HAL_AUDIO_DMIC_GPIO_DMIC0) {
            //2822
            //GPIO2 AuxFunc.3 DMIC0_CLK
            //GPIO3 AuxFunc.3 DMIC0_DAT
            hal_gpio_init(2);
            hal_pinmux_set_function(2, 3);
            hal_gpio_init(3);
            hal_pinmux_set_function(3, 3);
            LINEIN_PLAYBACK_LOG_I("DMIC0 GPIO Set done dmic sel %d\r\n", 1, adc_mode_HQA);
        } else if ((uint32_t)adc_mode_HQA == (uint32_t)HAL_AUDIO_DMIC_GPIO_DMIC1) {
            //2822
            //GPIO4 AuxFunc.3 DMIC0_CLK
            //GPIO5 AuxFunc.3 DMIC0_DAT
            hal_gpio_init(4);
            hal_pinmux_set_function(4, 3);
            hal_gpio_init(5);
            hal_pinmux_set_function(5, 3);
            LINEIN_PLAYBACK_LOG_I("DMIC1 GPIO Set done dmic sel %d\r\n", 1, adc_mode_HQA);
        }
    }
#endif

#if defined(MTK_EXTERNAL_DSP_NEED_SUPPORT)
    ami_set_afe_param(STREAM_OUT, linein_sample_rate, true);
#endif
    p_param_share = hal_audio_dsp_controller_put_paramter(&open_param, sizeof(mcu2dsp_open_param_t), AUDIO_MESSAGE_TYPE_LINEIN);
    ami_hal_audio_status_set_running_flag(AUDIO_SCENARIO_TYPE_LINE_IN, &open_param, true);
    hal_audio_dsp_controller_send_message(MSG_MCU2DSP_TRULY_LINEIN_PLAYBACK_OPEN, 0, (uint32_t)p_param_share, true);

    linein_status = LINEIN_STATE_PLAY;
    return LINEIN_EXECUTION_SUCCESS;
}
#endif

linein_result_t pure_linein_playback_open(hal_audio_sampling_rate_t linein_sample_rate, hal_audio_device_t in_audio_device, hal_audio_device_t out_audio_device)
{
    LINEIN_PLAYBACK_LOG_I("enter pure_linein_playback_open, sample_rate=%d, in_audio_device=0x%x, out_audio_device=0x%x\n", 3, hal_audio_sampling_rate_enum_to_value(linein_sample_rate), in_audio_device, out_audio_device);
    if (linein_status != LINEIN_STATE_IDLE) {
        LINEIN_PLAYBACK_LOG_I("cannot open because state(%d)\n", 1, linein_status);
        return LINEIN_EXECUTION_FAIL;
    }

    if (linein_sample_rate != HAL_AUDIO_SAMPLING_RATE_48KHZ && linein_sample_rate != HAL_AUDIO_SAMPLING_RATE_96KHZ && linein_sample_rate != HAL_AUDIO_SAMPLING_RATE_192KHZ) {
        LINEIN_PLAYBACK_LOG_I("not support sample rate=%d\n", 1, hal_audio_sampling_rate_enum_to_value(linein_sample_rate));
        return LINEIN_EXECUTION_FAIL;
    }
    if (in_audio_device != HAL_AUDIO_DEVICE_I2S_MASTER && in_audio_device != HAL_AUDIO_DEVICE_I2S_MASTER_L && in_audio_device != HAL_AUDIO_DEVICE_I2S_MASTER_R
        && in_audio_device != HAL_AUDIO_DEVICE_I2S_SLAVE && ((in_audio_device & HAL_AUDIO_DEVICE_MAIN_MIC_DUAL) == false)
        && ((in_audio_device & HAL_AUDIO_DEVICE_LINEINPLAYBACK_DUAL) == false) && ((in_audio_device & HAL_AUDIO_DEVICE_DIGITAL_MIC_DUAL) == false)) {
        LINEIN_PLAYBACK_LOG_I("not support in device=%d\n", 1, in_audio_device);
        return LINEIN_EXECUTION_FAIL;
    }
    if (out_audio_device != HAL_AUDIO_DEVICE_I2S_MASTER && out_audio_device != HAL_AUDIO_DEVICE_I2S_SLAVE && ((out_audio_device & HAL_AUDIO_DEVICE_DAC_DUAL) == false)) {
        LINEIN_PLAYBACK_LOG_I("not support out device=%d\n", 1, out_audio_device);
        return LINEIN_EXECUTION_FAIL;
    }

    mcu2dsp_open_param_t open_param;
    memset(&open_param, 0, sizeof(mcu2dsp_open_param_t));
    void *p_param_share;

    open_param.param.stream_in = STREAM_IN_AFE;
    open_param.param.stream_out = STREAM_OUT_AFE;
    open_param.audio_scenario_type = AUDIO_SCENARIO_TYPE_LINE_IN;

#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
    open_param.stream_in_param.afe.audio_interface = hal_audio_convert_linein_interface(audio_nvdm_HW_config.audio_scenario.Audio_Linein_Input_Path, true);
#else
    open_param.stream_in_param.afe.audio_interface = HAL_AUDIO_INTERFACE_2;
#endif

#ifdef AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
    open_param.stream_in_param.afe.audio_interface1 = hal_audio_convert_linein_interface(audio_nvdm_HW_config.audio_scenario.Audio_Linein_Input_Path, true);
#endif
#endif

#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
    open_param.stream_in_param.afe.performance = audio_nvdm_HW_config.audio_scenario.Audio_Analog_LineIn_Performance_Sel;
    open_param.stream_in_param.afe.iir_filter[0] = audio_nvdm_HW_config.adc_dac_config.ADDA_Audio_IIR_Filter;
    open_param.stream_in_param.afe.adc_mode = audio_nvdm_HW_config.adc_dac_config.ADDA_Analog_MIC0_Mode;
#endif

    open_param.stream_in_param.afe.audio_device = in_audio_device;
#ifdef AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
    open_param.stream_in_param.afe.audio_device1 = in_audio_device;
#endif
#endif

    if (open_param.stream_in_param.afe.audio_device == HAL_AUDIO_DEVICE_I2S_MASTER) {
        open_param.stream_in_param.afe.misc_parms      = I2S_CLK_SOURCE_DCXO;
        if (linein_sample_rate > HAL_AUDIO_SAMPLING_RATE_48KHZ) {
            open_param.stream_in_param.afe.misc_parms  = I2S_CLK_SOURCE_APLL;
        }
    } else {
        if (in_audio_device & HAL_AUDIO_DEVICE_LINEINPLAYBACK_DUAL) {
            open_param.stream_in_param.afe.misc_parms      = MICBIAS_SOURCE_ALL | MICBIAS3V_OUTVOLTAGE_2p4v;
        } else {
            open_param.stream_in_param.afe.misc_parms      = MICBIAS_SOURCE_ALL | MICBIAS3V_OUTVOLTAGE_1p85v;
        }
    }
    //I2S Master parameters
    open_param.stream_in_param.afe.i2s_master_sampling_rate[0] = audio_nvdm_HW_config.audio_scenario.Audio_I2S0_Master_Sampling_Rate;
    open_param.stream_in_param.afe.i2s_master_sampling_rate[1] = audio_nvdm_HW_config.audio_scenario.Audio_I2S1_Master_Sampling_Rate;
    open_param.stream_in_param.afe.i2s_master_sampling_rate[2] = audio_nvdm_HW_config.audio_scenario.Audio_I2S2_Master_Sampling_Rate;
    open_param.stream_in_param.afe.i2s_master_sampling_rate[3] = audio_nvdm_HW_config.audio_scenario.Audio_I2S3_Master_Sampling_Rate;
    open_param.stream_in_param.afe.is_low_jitter[0] = audio_nvdm_HW_config.I2SM_config.I2S0_Master_Low_jitter;
    open_param.stream_in_param.afe.is_low_jitter[1] = audio_nvdm_HW_config.I2SM_config.I2S1_Master_Low_jitter;
    open_param.stream_in_param.afe.is_low_jitter[2] = audio_nvdm_HW_config.I2SM_config.I2S2_Master_Low_jitter;
    open_param.stream_in_param.afe.is_low_jitter[3] = audio_nvdm_HW_config.I2SM_config.I2S3_Master_Low_jitter;
    open_param.stream_in_param.afe.i2s_master_format[0] = audio_nvdm_HW_config.I2SM_config.I2S0_Master_Format;
    open_param.stream_in_param.afe.i2s_master_format[1] = audio_nvdm_HW_config.I2SM_config.I2S1_Master_Format;
    open_param.stream_in_param.afe.i2s_master_format[2] = audio_nvdm_HW_config.I2SM_config.I2S2_Master_Format;
    open_param.stream_in_param.afe.i2s_master_format[3] = audio_nvdm_HW_config.I2SM_config.I2S3_Master_Format;
    open_param.stream_in_param.afe.i2s_master_word_length[0] = audio_nvdm_HW_config.I2SM_config.I2S0_Master_Word_length;
    open_param.stream_in_param.afe.i2s_master_word_length[1] = audio_nvdm_HW_config.I2SM_config.I2S1_Master_Word_length;
    open_param.stream_in_param.afe.i2s_master_word_length[2] = audio_nvdm_HW_config.I2SM_config.I2S2_Master_Word_length;
    open_param.stream_in_param.afe.i2s_master_word_length[3] = audio_nvdm_HW_config.I2SM_config.I2S3_Master_Word_length;

    open_param.stream_out_param.afe.audio_device = out_audio_device;
    open_param.stream_out_param.afe.audio_interface = HAL_AUDIO_INTERFACE_1;
    open_param.stream_out_param.afe.stream_channel = HAL_AUDIO_DIRECT;
    open_param.stream_out_param.afe.format = AFE_PCM_FORMAT_S32_LE;
    open_param.stream_out_param.afe.stream_out_sampling_rate = hal_audio_sampling_rate_enum_to_value(linein_sample_rate);
#if defined (FIXED_SAMPLING_RATE_TO_48KHZ)
    open_param.stream_out_param.afe.sampling_rate   = HAL_AUDIO_FIXED_AFE_48K_SAMPLE_RATE;
#elif defined (AIR_FIXED_DL_SAMPLING_RATE_TO_96KHZ)
    open_param.stream_out_param.afe.sampling_rate   = HAL_AUDIO_FIXED_AFE_96K_SAMPLE_RATE;
#else
    open_param.stream_out_param.afe.sampling_rate   = hal_audio_sampling_rate_enum_to_value(linein_sample_rate);
#endif
    open_param.stream_out_param.afe.hw_gain = true;

#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
    open_param.stream_out_param.afe.performance = audio_nvdm_HW_config.adc_dac_config.ADDA_DAC_Class_AB_G_Performance_Sel;
#endif
    if (open_param.stream_out_param.afe.audio_device == HAL_AUDIO_DEVICE_I2S_MASTER) {
        open_param.stream_out_param.afe.misc_parms      = I2S_CLK_SOURCE_DCXO;
        if (linein_sample_rate > HAL_AUDIO_SAMPLING_RATE_48KHZ) {
            open_param.stream_out_param.afe.misc_parms  = I2S_CLK_SOURCE_APLL;
        }
    } else {
        open_param.stream_out_param.afe.misc_parms      = DOWNLINK_PERFORMANCE_NORMAL;
    }
    //I2S Master parameters
    open_param.stream_out_param.afe.i2s_master_sampling_rate[0] = audio_nvdm_HW_config.audio_scenario.Audio_I2S0_Master_Sampling_Rate;
    open_param.stream_out_param.afe.i2s_master_sampling_rate[1] = audio_nvdm_HW_config.audio_scenario.Audio_I2S1_Master_Sampling_Rate;
    open_param.stream_out_param.afe.i2s_master_sampling_rate[2] = audio_nvdm_HW_config.audio_scenario.Audio_I2S2_Master_Sampling_Rate;
    open_param.stream_out_param.afe.i2s_master_sampling_rate[3] = audio_nvdm_HW_config.audio_scenario.Audio_I2S3_Master_Sampling_Rate;
    open_param.stream_out_param.afe.is_low_jitter[0] = audio_nvdm_HW_config.I2SM_config.I2S0_Master_Low_jitter;
    open_param.stream_out_param.afe.is_low_jitter[1] = audio_nvdm_HW_config.I2SM_config.I2S1_Master_Low_jitter;
    open_param.stream_out_param.afe.is_low_jitter[2] = audio_nvdm_HW_config.I2SM_config.I2S2_Master_Low_jitter;
    open_param.stream_out_param.afe.is_low_jitter[3] = audio_nvdm_HW_config.I2SM_config.I2S3_Master_Low_jitter;
    open_param.stream_out_param.afe.i2s_master_format[0] = audio_nvdm_HW_config.I2SM_config.I2S0_Master_Format;
    open_param.stream_out_param.afe.i2s_master_format[1] = audio_nvdm_HW_config.I2SM_config.I2S1_Master_Format;
    open_param.stream_out_param.afe.i2s_master_format[2] = audio_nvdm_HW_config.I2SM_config.I2S2_Master_Format;
    open_param.stream_out_param.afe.i2s_master_format[3] = audio_nvdm_HW_config.I2SM_config.I2S3_Master_Format;
    open_param.stream_out_param.afe.i2s_master_word_length[0] = audio_nvdm_HW_config.I2SM_config.I2S0_Master_Word_length;
    open_param.stream_out_param.afe.i2s_master_word_length[1] = audio_nvdm_HW_config.I2SM_config.I2S1_Master_Word_length;
    open_param.stream_out_param.afe.i2s_master_word_length[2] = audio_nvdm_HW_config.I2SM_config.I2S2_Master_Word_length;
    open_param.stream_out_param.afe.i2s_master_word_length[3] = audio_nvdm_HW_config.I2SM_config.I2S3_Master_Word_length;

#if defined(MTK_EXTERNAL_DSP_NEED_SUPPORT)
    ami_set_afe_param(STREAM_OUT, linein_sample_rate, true);
#endif
    p_param_share = hal_audio_dsp_controller_put_paramter(&open_param, sizeof(mcu2dsp_open_param_t), AUDIO_MESSAGE_TYPE_LINEIN);
    ami_hal_audio_status_set_running_flag(AUDIO_SCENARIO_TYPE_LINE_IN, &open_param, true);
    hal_audio_dsp_controller_send_message(MSG_MCU2DSP_TRULY_LINEIN_PLAYBACK_OPEN, 0, (uint32_t)p_param_share, true);

    linein_status = LINEIN_STATE_PLAY;
    return LINEIN_EXECUTION_SUCCESS;
}

linein_result_t pure_linein_playback_close()
{
    LINEIN_PLAYBACK_LOG_I("enter pure_linein_playback_close\n", 0);
    if (linein_status != LINEIN_STATE_PLAY) {
        LINEIN_PLAYBACK_LOG_I("cannot close because state(%d)\n", 1, linein_status);
        return LINEIN_EXECUTION_FAIL;
    }
    hal_audio_dsp_controller_send_message(MSG_MCU2DSP_TRULY_LINEIN_PLAYBACK_CLOSE, 0, 0, true);
    linein_status = LINEIN_STATE_IDLE;
    ami_hal_audio_status_set_running_flag(AUDIO_SCENARIO_TYPE_LINE_IN, NULL, false);
    return LINEIN_EXECUTION_SUCCESS;
}

linein_result_t audio_linein_playback_open(hal_audio_sampling_rate_t linein_sample_rate, hal_audio_device_t in_audio_device, hal_audio_device_t out_audio_device)
{
#if defined(MTK_PURE_LINEIN_PLAYBACK_ENABLE)
    return pure_linein_playback_open(linein_sample_rate, in_audio_device, out_audio_device);
#else
#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
    if (0x01 == audio_nvdm_HW_config.audio_scenario.Audio_Pure_Linein_enable) {
        return pure_linein_playback_open(linein_sample_rate, in_audio_device, out_audio_device);
    } else {
        return linein_playback_open(linein_sample_rate, in_audio_device, out_audio_device);
    }
#else
    return linein_playback_open(linein_sample_rate, in_audio_device, out_audio_device);
#endif
#endif
}

linein_result_t audio_linein_playback_start()
{
#if defined(MTK_PURE_LINEIN_PLAYBACK_ENABLE)
    return LINEIN_EXECUTION_SUCCESS;
#else
#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
    if (0x01 == audio_nvdm_HW_config.audio_scenario.Audio_Pure_Linein_enable) {
        return LINEIN_EXECUTION_SUCCESS;
    } else {
        return linein_playback_start();
    }
#else
    return linein_playback_start();
#endif
#endif
}

linein_result_t audio_linein_playback_stop()
{
#if defined(MTK_PURE_LINEIN_PLAYBACK_ENABLE)
    return LINEIN_EXECUTION_SUCCESS;
#else
#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
    if (0x01 == audio_nvdm_HW_config.audio_scenario.Audio_Pure_Linein_enable) {
        return LINEIN_EXECUTION_SUCCESS;
    } else {
        return linein_playback_stop();
    }
#else
    return linein_playback_stop();
#endif
#endif
}

linein_result_t audio_linein_playback_close()
{
#if defined(MTK_PURE_LINEIN_PLAYBACK_ENABLE)
    return pure_linein_playback_close();
#else
#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
    if (0x01 == audio_nvdm_HW_config.audio_scenario.Audio_Pure_Linein_enable) {
        return pure_linein_playback_close();
    } else {
        return linein_playback_close();
    }
#else
    return linein_playback_close();
#endif
#endif
}

linein_result_t audio_linein_playback_set_volume(linein_playback_gain_t gain)
{
    return linein_playback_set_volume(gain);
}

linein_result_t audio_pure_linein_playback_open(hal_audio_sampling_rate_t linein_sample_rate, hal_audio_device_t in_audio_device, hal_audio_device_t out_audio_device)
{
    return pure_linein_playback_open(linein_sample_rate, in_audio_device, out_audio_device);
}

linein_result_t audio_pure_linein_playback_close()
{
    return pure_linein_playback_close();
}

#ifdef LINE_IN_PURE_FOR_AMIC_CLASS_G_HQA
linein_result_t audio_pure_linein_playback_open_HQA(hal_audio_sampling_rate_t linein_sample_rate, hal_audio_device_t in_audio_device, hal_audio_interface_t device_in_interface_HQA, hal_audio_analog_mdoe_t adc_mode_HQA, hal_audio_performance_mode_t mic_performance_HQA, hal_audio_device_t out_audio_device, hal_audio_performance_mode_t dac_performance_HQA)
{
    return pure_linein_playback_open_HQA(linein_sample_rate,  in_audio_device, device_in_interface_HQA, adc_mode_HQA, mic_performance_HQA,  out_audio_device, dac_performance_HQA);
}
#endif

