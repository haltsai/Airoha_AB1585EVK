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

/******************************************************************************
* Include
******************************************************************************/
#include "string.h"
#include "dsp_scenario.h"
#include "bt_interface.h"
#include "stream.h"
#ifdef AIR_BT_HFP_ENABLE
#include "stream_n9sco.h"
#endif
#include "stream_n9_a2dp.h"
#include "hal_audio_afe_define.h"
#include "hal_audio_afe_control.h"
#include "hal_sleep_manager.h"
#include "dsp_audio_msg_define.h"
#include "dsp_drv_afe.h"
#include "davt.h"
#include "dhpt.h"
#include "dllt.h"
#include "dsp_audio_ctrl.h"
#include "dsp_share_memory.h"
#include "dsp_temp.h"
#include "audio_nvdm_common.h"
#include "clk_skew.h"
#include "compander_interface.h"
#include "long_term_clk_skew.h"
#ifdef AIR_VOICE_NR_ENABLE
#include "voice_nr_interface.h"
#endif
#if defined(MTK_PEQ_ENABLE) || defined(MTK_LINEIN_PEQ_ENABLE)
#include "peq_interface.h"
#include "compander_interface.h"
#endif
#include "dsp_dump.h"
#include "sfr_bt.h"
#ifdef MTK_LEAKAGE_DETECTION_ENABLE
#include "leakage_compensation.h"
#endif
#ifdef MTK_WWE_ENABLE
#include "wwe_interface.h"
#endif
#ifdef MTK_SENSOR_SOURCE_ENABLE
#include "bsp_multi_axis_sensor.h"
#endif
#ifdef MTK_AUDIO_PLC_ENABLE
#include "audio_plc_interface.h"
#endif
#include "dsp_audio_msg_define.h"

#ifdef MTK_BT_A2DP_ENABLE
#define PLAY_EN_CHECK_TIMER
#endif /* MTK_BT_A2DP_ENABLE */

#define TRIGGER_A2DP_PROCSESS_TIMER
#if defined(PLAY_EN_CHECK_TIMER) || defined(TRIGGER_A2DP_PROCSESS_TIMER)
#include "timers.h"
#endif

#ifdef MTK_CELT_DEC_ENABLE
#include "celt_dec_interface.h"
#endif

#ifdef AIR_BT_CODEC_BLE_ENABLED
#include "stream_n9ble.h"
#include "common.h"
#ifdef AIR_BT_LE_LC3_ENABLE
#include "lc3_dec_interface.h"
#include "lc3_enc_interface.h"
#endif
#include "dsp_memory_region.h"

#endif

#ifdef AIR_BT_LE_LC3PLUS_ENABLE
#include "lc3plus_dec_interface.h"
#endif

#ifdef MTK_LINEIN_INS_ENABLE
#include "ins_interface.h"
#endif

#ifdef MTK_AUDIO_LOOPBACK_TEST_ENABLE
#include "audio_loopback_test_interface.h"
#endif

#ifdef AIR_SOFTWARE_SRC_ENABLE
#include "sw_src_interface.h"
#endif
#ifdef AIR_SOFTWARE_CLK_SKEW_ENABLE
#include "clk_skew_sw.h"
#endif
#ifdef AIR_SOFTWARE_GAIN_ENABLE
#include "sw_gain_interface.h"
#endif
#ifdef AIR_ANC_WIND_DETECTION_ENABLE
#include "wind_interface.h"
#endif
#if defined(AIR_WIRED_AUDIO_ENABLE)
#include "scenario_wired_audio.h"
#endif
#ifdef MTK_ANC_ENABLE
#include "anc_api.h"
#endif
#ifdef MTK_USER_TRIGGER_FF_ENABLE
#include "user_trigger_adaptive_ff.h"
#endif
#ifdef AIR_ANC_USER_UNAWARE_ENABLE
#include "user_unaware_interface.h"
#endif

#ifdef AIR_ANC_ENVIRONMENT_DETECTION_ENABLE
#include "environment_detection_interface.h"
#endif
#include "hal_resource_assignment.h"
#ifdef AIR_ADVANCED_PASSTHROUGH_ENABLE
#include "scenario_advanced_passthrough.h"
#endif

#if defined (AIR_DUAL_CHIP_MIXING_MODE_ROLE_MASTER_ENABLE) || defined (AIR_DUAL_CHIP_MIXING_MODE_ROLE_SLAVE_ENABLE)
#include "dsp_mux_uart.h"
bool g_dchs_dl_open_flag = false;
#endif


/******************************************************************************
* Define
******************************************************************************/
#define eSCO_UT_FILL_SHAREBUF
// #define eSCO_UT_WETH_MEMORY


#define eSCO_UT_FILL_SHAREBUF
#define DSP_REMAP_SHARE_INFO(para,type)  ({  \
         para = (type)hal_memview_cm4_to_dsp0((uint32_t)para); \
         /*((n9_dsp_share_info_ptr) para)->startaddr = hal_memview_cm4_to_dsp0(((SHARE_BUFFER_INFO_PTR) para)->startaddr);*/})
#define abs32(x) ( (x >= 0) ? x : (-x) )

EXTERN audio_hardware afe_get_audio_hardware_by_au_afe_open_param(au_afe_open_param_t *afe_open);
EXTERN audio_instance afe_get_audio_instance_by_au_afe_open_param(au_afe_open_param_t *afe_open);
EXTERN audio_channel afe_get_audio_channel_by_au_afe_open_param(au_afe_open_param_t *afe_open);
EXTERN afe_t afe;

/******************************************************************************
* Global variable
******************************************************************************/
CONNECTION_IF playback_if, n9_a2dp_if, record_if, linein_playback_if;
#ifdef AIR_BT_CODEC_BLE_ENABLED
CONNECTION_IF n9_ble_ul_if, n9_ble_dl_if;
#endif
#ifdef MTK_PROMPT_SOUND_ENABLE
CONNECTION_IF playback_vp_if;
#endif
#ifdef MTK_PROMPT_SOUND_DUMMY_SOURCE_ENABLE
CONNECTION_IF playback_vp_dummy_source_if;
#endif
#ifdef MTK_SENSOR_SOURCE_ENABLE
CONNECTION_IF sensor_src_if[AUDIO_TRANSMITTER_GSENSOR_SUB_ID_MAX] = {
    /*source,      sink,  transform, pfeature_table;*/
    {NULL,      NULL,       NULL,     AudioFeatureList_GSensorMotionDetect_virtual},  //no need to send data to MCU
    {NULL,      NULL,       NULL,     AudioFeatureList_GSensorMotionDetect}, //send sensor data to MCU
    {NULL,      NULL,       NULL,     AudioFeatureList_GSensorMotionDetect_virtual},
};
#endif

#if defined(MTK_ANC_SURROUND_MONITOR_ENABLE)
CONNECTION_IF sensor_anc_monitor_if = {NULL,      NULL,       NULL,     stream_featuremulti_anc_monitor};
#endif

#if defined(AIR_ADAPTIVE_EQ_ENABLE)
CONNECTION_IF sensor_adaptive_eq_monitor_if = {NULL,      NULL,       NULL,     stream_featuremulti_adaptive_eq_monitor};
#endif
#if defined (AIR_DUAL_CHIP_MIXING_MODE_ROLE_MASTER_ENABLE) || defined (AIR_DUAL_CHIP_MIXING_MODE_ROLE_SLAVE_ENABLE)
CONNECTION_IF dchs_uart_dl_if = {NULL,      NULL,       NULL,     stream_feature_list_dchs_uart_dl};
CONNECTION_IF dchs_uart_ul_if = {NULL,      NULL,       NULL,     stream_feature_list_dchs_uart_ul};
#endif

void afe_print_reg(void)
{
    DSP_MW_LOG_I("AUDIO_TOP_CON0=%x\r\n", 1, AFE_READ(AUDIO_TOP_CON0));
    DSP_MW_LOG_I("AUDIO_TOP_CON1=%x\r\n", 1, AFE_READ(AUDIO_TOP_CON1));
    DSP_MW_LOG_I("AFE_DAC_CON0=%x\r\n", 1, AFE_READ(AFE_DAC_CON0));
    DSP_MW_LOG_I("AFE_DAC_CON1=%x\r\n", 1, AFE_READ(AFE_DAC_CON1));
    DSP_MW_LOG_I("AFE_DAC_CON2=%x\r\n", 1, AFE_READ(AFE_DAC_CON2));
    DSP_MW_LOG_I("AFE_ADDA_TOP_CON0=%x\r\n", 1, AFE_READ(AFE_ADDA_TOP_CON0));
    DSP_MW_LOG_I("AFE_ADDA_DL_SRC2_CON0=%x\r\n", 1, AFE_READ(AFE_ADDA_DL_SRC2_CON0));
    DSP_MW_LOG_I("AFE_ADDA_DL_SRC2_CON1=%x\r\n", 1, AFE_READ(AFE_ADDA_DL_SRC2_CON1));
    DSP_MW_LOG_I("AFE_ADDA_UL_DL_CON0=%x\r\n", 1, AFE_READ(AFE_ADDA_UL_DL_CON0));
    DSP_MW_LOG_I("AFE_ADDA_PREDIS_CON0=%x\r\n", 1, AFE_READ(AFE_ADDA_PREDIS_CON0));
    DSP_MW_LOG_I("AFE_ADDA_PREDIS_CON1=%x\r\n", 1, AFE_READ(AFE_ADDA_PREDIS_CON1));
    DSP_MW_LOG_I("AFE_CLASSG_LPSRCH_CFG0=%x\r\n", 1, AFE_READ(AFE_CLASSG_LPSRCH_CFG0));
    DSP_MW_LOG_I("ZCD_CON2=%x\r\n", 1, AFE_READ(ZCD_CON2));
    DSP_MW_LOG_I("ZCD_CON0=%x\r\n", 1, AFE_READ(ZCD_CON0));
    //DSP_MW_LOG_I("ZCD_CON1=%x\r\n", 1, AFE_READ(ZCD_CON1));
    DSP_MW_LOG_I("AUDDEC_ANA_CON0=%x\r\n", 1, ANA_READ(AUDDEC_ANA_CON0));
    DSP_MW_LOG_I("AUDDEC_ANA_CON1=%x\r\n", 1, ANA_READ(AUDDEC_ANA_CON1));
    DSP_MW_LOG_I("AUDDEC_ANA_CON2=%x\r\n", 1, ANA_READ(AUDDEC_ANA_CON2));
    DSP_MW_LOG_I("AUDDEC_ANA_CON3=%x\r\n", 1, ANA_READ(AUDDEC_ANA_CON3));
    DSP_MW_LOG_I("AUDDEC_ANA_CON4=%x\r\n", 1, ANA_READ(AUDDEC_ANA_CON4));
    DSP_MW_LOG_I("AUDDEC_ANA_CON5=%x\r\n", 1, ANA_READ(AUDDEC_ANA_CON5));
    DSP_MW_LOG_I("AUDDEC_ANA_CON6=%x\r\n", 1, ANA_READ(AUDDEC_ANA_CON6));
    DSP_MW_LOG_I("AUDDEC_ANA_CON7=%x\r\n", 1, ANA_READ(AUDDEC_ANA_CON7));
    DSP_MW_LOG_I("AUDDEC_ANA_CON8=%x\r\n", 1, ANA_READ(AUDDEC_ANA_CON8));
    DSP_MW_LOG_I("AUDDEC_ANA_CON9=%x\r\n", 1, ANA_READ(AUDDEC_ANA_CON9));
    DSP_MW_LOG_I("AUDDEC_ANA_CON10=%x\r\n", 1, ANA_READ(AUDDEC_ANA_CON10));
    DSP_MW_LOG_I("AUDDEC_ANA_CON11=%x\r\n", 1, ANA_READ(AUDDEC_ANA_CON11));
    DSP_MW_LOG_I("AUDDEC_ANA_CON12=%x\r\n", 1, ANA_READ(AUDDEC_ANA_CON12));
    DSP_MW_LOG_I("AUDDEC_ANA_CON13=%x\r\n", 1, ANA_READ(AUDDEC_ANA_CON13));
    DSP_MW_LOG_I("AUDDEC_ANA_CON14=%x\r\n", 1, ANA_READ(AUDDEC_ANA_CON14));
}
bool BleDlStopFlag;
audio_dsp_ull_start_ctrl_param_t audio_headset_ull_ctrl;
#define ULL_BASIC_LATENCY 20000

/******************************************************************************
* Function
******************************************************************************/
static void dsp_audio_msg_ack(uint32_t msg_id, bool from_isr);
U32 dsp_calcuate_number_of_bit(U32 value)
{
    U32 number_of_bit;
    value = value - ((value >> 1) & 0x55555555);
    value = (value & 0x33333333) + ((value >> 2) & 0x33333333);
    number_of_bit = (((value + (value >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
    return number_of_bit;
}

#ifdef AIR_AUDIO_HARDWARE_ENABLE
SOURCE dsp_open_stream_in_afe(mcu2dsp_open_param_p open_param)
{
    SOURCE source;
    bool echo_path = false;
    //DSP_MW_LOG_I("Stream in afe\r\n", 0);
    DSP_MW_LOG_I("afe in device:%d, channel:%d, memory:%d, interface:%d \r\n", 4, open_param->stream_in_param.afe.audio_device,
                 open_param->stream_in_param.afe.stream_channel,
                 open_param->stream_in_param.afe.memory,
                 open_param->stream_in_param.afe.audio_interface);

    DSP_MW_LOG_I("afe in interface:%d, hw_gain:%d\r\n", 2, open_param->stream_in_param.afe.audio_interface,
                 open_param->stream_in_param.afe.hw_gain);
#ifdef AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
    DSP_MW_LOG_I("[2A2D] afe in audio_device: %d, %d, %d, %d, %d, %d, %d\r\n", 7, open_param->stream_in_param.afe.audio_device, open_param->stream_in_param.afe.audio_device1, open_param->stream_in_param.afe.audio_device2, open_param->stream_in_param.afe.audio_device3, open_param->stream_in_param.afe.audio_device4, open_param->stream_in_param.afe.audio_device5, open_param->stream_in_param.afe.audio_device6);
    DSP_MW_LOG_I("[2A2D] afe in audio_interface: %d, %d, %d, %d, %d, %d, %d\r\n", 7, open_param->stream_in_param.afe.audio_interface, open_param->stream_in_param.afe.audio_interface1, open_param->stream_in_param.afe.audio_interface2, open_param->stream_in_param.afe.audio_interface3, open_param->stream_in_param.afe.audio_device4, open_param->stream_in_param.afe.audio_device5, open_param->stream_in_param.afe.audio_device6);
#endif

    AudioAfeConfiguration(AUDIO_SOURCE_DATA_FORMAT, open_param->stream_in_param.afe.format);

    if (open_param->stream_in_param.afe.sampling_rate != 0) {
        AudioAfeConfiguration(AUDIO_SOURCE_IRQ_RATE, open_param->stream_in_param.afe.sampling_rate);
    }
#if defined AIR_HWSRC_RX_TRACKING_ENABLE || defined AIR_GAMING_MODE_DONGLE_I2S_IN_ENABLE
    if (open_param->stream_in_param.afe.stream_out_sampling_rate != 0) {
        AudioAfeConfiguration(AUDIO_SRC_RATE, open_param->stream_in_param.afe.stream_out_sampling_rate);
        DSP_MW_LOG_I("afe in hwsrc output rate = %d", 1, open_param->stream_in_param.afe.stream_out_sampling_rate);
    }
#endif
    AudioAfeConfiguration(AUDIO_SOURCE_IRQ_PERIOD, open_param->stream_in_param.afe.irq_period);

    /*echo path*/
    echo_path = (open_param->stream_in_param.afe.memory & HAL_AUDIO_MEM3)/* && (dsp_calcuate_number_of_bit(open_param->stream_in_param.afe.audio_interface) == 1)*/;
    if (echo_path) {
        open_param->stream_in_param.afe.memory &= ~HAL_AUDIO_MEM3;//echo path
        DSP_MW_LOG_I("afe in with echo, memory:%d\r\n", 1, open_param->stream_in_param.afe.memory);
    } else {
        DSP_MW_LOG_I("afe in not with echo, memory:%d\r\n", 1, open_param->stream_in_param.afe.memory);
    }


#if 0

    /* 4 A-MIC Verification */
    open_param->stream_in_param.afe.audio_device        = HAL_AUDIO_CONTROL_DEVICE_ANALOG_MIC_L;
    open_param->stream_in_param.afe.audio_device1       = HAL_AUDIO_CONTROL_DEVICE_ANALOG_MIC_R;
    open_param->stream_in_param.afe.audio_device2       = HAL_AUDIO_CONTROL_DEVICE_ANALOG_MIC_L;
    open_param->stream_in_param.afe.audio_device3       = HAL_AUDIO_CONTROL_DEVICE_ANALOG_MIC_R;

    open_param->stream_in_param.afe.audio_interface     = HAL_AUDIO_INTERFACE_1;
    open_param->stream_in_param.afe.audio_interface1    = HAL_AUDIO_INTERFACE_1;
    open_param->stream_in_param.afe.audio_interface2    = HAL_AUDIO_INTERFACE_2;
    open_param->stream_in_param.afe.audio_interface3    = HAL_AUDIO_INTERFACE_2;

#endif

    AudioAfeConfiguration(AUDIO_SOURCE_DEVICE, open_param->stream_in_param.afe.audio_device);
    DSP_MW_LOG_I("audio_device 0x%x", 1, open_param->stream_in_param.afe.audio_device);
#ifdef AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
    AudioAfeConfiguration(AUDIO_SOURCE_DEVICE1, open_param->stream_in_param.afe.audio_device1);
    DSP_MW_LOG_I("audio_device1 0x%x", 1, open_param->stream_in_param.afe.audio_device1);
    AudioAfeConfiguration(AUDIO_SOURCE_DEVICE2, open_param->stream_in_param.afe.audio_device2);
    DSP_MW_LOG_I("audio_device2 0x%x", 1, open_param->stream_in_param.afe.audio_device2);
    AudioAfeConfiguration(AUDIO_SOURCE_DEVICE3, open_param->stream_in_param.afe.audio_device3);
    DSP_MW_LOG_I("audio_device3 0x%x", 1, open_param->stream_in_param.afe.audio_device3);
#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
    AudioAfeConfiguration(AUDIO_SOURCE_DEVICE4, open_param->stream_in_param.afe.audio_device4);
    DSP_MW_LOG_I("audio_device4 0x%x", 1, open_param->stream_in_param.afe.audio_device4);
    AudioAfeConfiguration(AUDIO_SOURCE_DEVICE5, open_param->stream_in_param.afe.audio_device5);
    DSP_MW_LOG_I("audio_device5 0x%x", 1, open_param->stream_in_param.afe.audio_device5);
    AudioAfeConfiguration(AUDIO_SOURCE_DEVICE6, open_param->stream_in_param.afe.audio_device6);
    DSP_MW_LOG_I("audio_device6 0x%x", 1, open_param->stream_in_param.afe.audio_device6);
    AudioAfeConfiguration(AUDIO_SOURCE_DEVICE7, open_param->stream_in_param.afe.audio_device7);
    DSP_MW_LOG_I("audio_device7 0x%x", 1, open_param->stream_in_param.afe.audio_device7);
#endif
#endif
    AudioAfeConfiguration(AUDIO_SOURCE_CHANNEL, open_param->stream_in_param.afe.stream_channel);
    AudioAfeConfiguration(AUDIO_SOURCE_MEMORY, open_param->stream_in_param.afe.memory);
    AudioAfeConfiguration(AUDIO_SOURCE_INTERFACE, open_param->stream_in_param.afe.audio_interface);
    DSP_MW_LOG_I("audio_interface 0x%x", 1, open_param->stream_in_param.afe.audio_interface);
#ifdef AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
    AudioAfeConfiguration(AUDIO_SOURCE_INTERFACE1, open_param->stream_in_param.afe.audio_interface1);
    DSP_MW_LOG_I("audio_interface1 0x%x", 1, open_param->stream_in_param.afe.audio_interface1);
    AudioAfeConfiguration(AUDIO_SOURCE_INTERFACE2, open_param->stream_in_param.afe.audio_interface2);
    DSP_MW_LOG_I("audio_interface2 0x%x", 1, open_param->stream_in_param.afe.audio_interface2);
    AudioAfeConfiguration(AUDIO_SOURCE_INTERFACE3, open_param->stream_in_param.afe.audio_interface3);
    DSP_MW_LOG_I("audio_interface3 0x%x", 1, open_param->stream_in_param.afe.audio_interface3);
#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
    AudioAfeConfiguration(AUDIO_SOURCE_INTERFACE4, open_param->stream_in_param.afe.audio_interface4);
    DSP_MW_LOG_I("audio_interface4 0x%x", 1, open_param->stream_in_param.afe.audio_interface4);
    AudioAfeConfiguration(AUDIO_SOURCE_INTERFACE5, open_param->stream_in_param.afe.audio_interface5);
    DSP_MW_LOG_I("audio_interface5 0x%x", 1, open_param->stream_in_param.afe.audio_interface5);
    AudioAfeConfiguration(AUDIO_SOURCE_INTERFACE6, open_param->stream_in_param.afe.audio_interface6);
    DSP_MW_LOG_I("audio_interface6 0x%x", 1, open_param->stream_in_param.afe.audio_interface6);
    AudioAfeConfiguration(AUDIO_SOURCE_INTERFACE7, open_param->stream_in_param.afe.audio_interface7);
    DSP_MW_LOG_I("audio_interface7 0x%x", 1, open_param->stream_in_param.afe.audio_interface7);
#endif
#endif
#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
    AudioAfeConfiguration(AUDIO_SOURCE_ADC_MODE, open_param->stream_in_param.afe.ul_adc_mode[0]);
    AudioAfeConfiguration(AUDIO_SOURCE_ADC_MODE1, open_param->stream_in_param.afe.ul_adc_mode[1]);
    AudioAfeConfiguration(AUDIO_SOURCE_ADC_MODE2, open_param->stream_in_param.afe.ul_adc_mode[2]);
    AudioAfeConfiguration(AUDIO_SOURCE_ADC_MODE3, open_param->stream_in_param.afe.ul_adc_mode[3]);
    AudioAfeConfiguration(AUDIO_SOURCE_ADC_MODE4, open_param->stream_in_param.afe.ul_adc_mode[4]);
    AudioAfeConfiguration(AUDIO_SOURCE_ADC_MODE5, open_param->stream_in_param.afe.ul_adc_mode[5]);
    AudioAfeConfiguration(AUDIO_SOURCE_ADC_MODE6, open_param->stream_in_param.afe.ul_adc_mode[6]);
    AudioAfeConfiguration(AUDIO_SOURCE_ADC_MODE7, open_param->stream_in_param.afe.ul_adc_mode[7]);

    AudioAfeConfiguration(AUDIO_SOURCE_BIAS_VOLTAGE, open_param->stream_in_param.afe.bias_voltage[0]);
#ifdef AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
    AudioAfeConfiguration(AUDIO_SOURCE_BIAS_VOLTAGE1, open_param->stream_in_param.afe.bias_voltage[1]);
    AudioAfeConfiguration(AUDIO_SOURCE_BIAS_VOLTAGE2, open_param->stream_in_param.afe.bias_voltage[2]);
    AudioAfeConfiguration(AUDIO_SOURCE_BIAS_VOLTAGE3, open_param->stream_in_param.afe.bias_voltage[3]);
    AudioAfeConfiguration(AUDIO_SOURCE_BIAS_VOLTAGE4, open_param->stream_in_param.afe.bias_voltage[4]);
#endif
    AudioAfeConfiguration(AUDIO_SOURCE_BIAS_SELECT, open_param->stream_in_param.afe.bias_select);
    AudioAfeConfiguration(AUDIO_SOURCE_WITH_EXTERNAL_BIAS, open_param->stream_in_param.afe.with_external_bias);
    AudioAfeConfiguration(AUDIO_SOURCE_WITH_BIAS_LOWPOWER, open_param->stream_in_param.afe.with_bias_lowpower);
    AudioAfeConfiguration(AUDIO_SOURCE_BIAS1_BIAS2_WITH_LDO0, open_param->stream_in_param.afe.bias1_2_with_LDO0);
    AudioAfeConfiguration(AUDIO_SOURCE_DMIC_SELECT, open_param->stream_in_param.afe.dmic_selection[0]);
#ifdef AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
    AudioAfeConfiguration(AUDIO_SOURCE_DMIC_SELECT1, open_param->stream_in_param.afe.dmic_selection[1]);
    AudioAfeConfiguration(AUDIO_SOURCE_DMIC_SELECT2, open_param->stream_in_param.afe.dmic_selection[2]);
    AudioAfeConfiguration(AUDIO_SOURCE_DMIC_SELECT3, open_param->stream_in_param.afe.dmic_selection[3]);
    AudioAfeConfiguration(AUDIO_SOURCE_DMIC_SELECT4, open_param->stream_in_param.afe.dmic_selection[4]);
    AudioAfeConfiguration(AUDIO_SOURCE_DMIC_SELECT5, open_param->stream_in_param.afe.dmic_selection[5]);
    AudioAfeConfiguration(AUDIO_SOURCE_DMIC_SELECT6, open_param->stream_in_param.afe.dmic_selection[6]);
    AudioAfeConfiguration(AUDIO_SOURCE_DMIC_SELECT7, open_param->stream_in_param.afe.dmic_selection[7]);
    DSP_MW_LOG_I("dmic_selection=%d,%d,%d,%d,%d,%d,%d,%d", 8, open_param->stream_in_param.afe.dmic_selection[0],
                 open_param->stream_in_param.afe.dmic_selection[1],
                 open_param->stream_in_param.afe.dmic_selection[2],
                 open_param->stream_in_param.afe.dmic_selection[3],
                 open_param->stream_in_param.afe.dmic_selection[4],
                 open_param->stream_in_param.afe.dmic_selection[5],
                 open_param->stream_in_param.afe.dmic_selection[6],
                 open_param->stream_in_param.afe.dmic_selection[7]);
#endif
    AudioAfeConfiguration(AUDIO_SOURCE_UL_IIR, open_param->stream_in_param.afe.iir_filter[0]);
    AudioAfeConfiguration(AUDIO_SOURCE_DMIC_CLOCK, open_param->stream_in_param.afe.dmic_clock_rate[0]);

#ifdef AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
    AudioAfeConfiguration(AUDIO_SOURCE_UL_IIR1, open_param->stream_in_param.afe.iir_filter[1]);
    AudioAfeConfiguration(AUDIO_SOURCE_UL_IIR2, open_param->stream_in_param.afe.iir_filter[2]);
    AudioAfeConfiguration(AUDIO_SOURCE_DMIC_CLOCK1, open_param->stream_in_param.afe.dmic_clock_rate[1]);
    AudioAfeConfiguration(AUDIO_SOURCE_DMIC_CLOCK2, open_param->stream_in_param.afe.dmic_clock_rate[2]);
#endif
    AudioAfeConfiguration(AUDIO_SOURCE_UL_PERFORMANCE, open_param->stream_in_param.afe.performance);
    if ((open_param->stream_in_param.afe.audio_device | open_param->stream_in_param.afe.audio_device1 |
         open_param->stream_in_param.afe.audio_device2 | open_param->stream_in_param.afe.audio_device3 |
         open_param->stream_in_param.afe.audio_device4 | open_param->stream_in_param.afe.audio_device5) &
        (HAL_AUDIO_CONTROL_DEVICE_I2S_MASTER | HAL_AUDIO_CONTROL_DEVICE_I2S_MASTER_L | HAL_AUDIO_CONTROL_DEVICE_I2S_MASTER_R)) {
        AudioAfeConfiguration(AUDIO_SOURCE_I2S_MASTER_SAMPLING_RATE, open_param->stream_in_param.afe.i2s_master_sampling_rate[0]);
        AudioAfeConfiguration(AUDIO_SOURCE_I2S_MASTER_SAMPLING_RATE1, open_param->stream_in_param.afe.i2s_master_sampling_rate[1]);
        AudioAfeConfiguration(AUDIO_SOURCE_I2S_MASTER_SAMPLING_RATE2, open_param->stream_in_param.afe.i2s_master_sampling_rate[2]);
        AudioAfeConfiguration(AUDIO_SOURCE_I2S_MASTER_SAMPLING_RATE3, open_param->stream_in_param.afe.i2s_master_sampling_rate[3]);
        AudioAfeConfiguration(AUDIO_SOURCE_I2S_MASTER_FORMAT, open_param->stream_in_param.afe.i2s_master_format[0]);
        AudioAfeConfiguration(AUDIO_SOURCE_I2S_MASTER_FORMAT1, open_param->stream_in_param.afe.i2s_master_format[1]);
        AudioAfeConfiguration(AUDIO_SOURCE_I2S_MASTER_FORMAT2, open_param->stream_in_param.afe.i2s_master_format[2]);
        AudioAfeConfiguration(AUDIO_SOURCE_I2S_MASTER_FORMAT3, open_param->stream_in_param.afe.i2s_master_format[3]);
        AudioAfeConfiguration(AUDIO_SOURCE_I2S_MASTER_WORD_LENGTH, open_param->stream_in_param.afe.i2s_master_word_length[0]);
        AudioAfeConfiguration(AUDIO_SOURCE_I2S_MASTER_WORD_LENGTH1, open_param->stream_in_param.afe.i2s_master_word_length[1]);
        AudioAfeConfiguration(AUDIO_SOURCE_I2S_MASTER_WORD_LENGTH2, open_param->stream_in_param.afe.i2s_master_word_length[2]);
        AudioAfeConfiguration(AUDIO_SOURCE_I2S_MASTER_WORD_LENGTH3, open_param->stream_in_param.afe.i2s_master_word_length[3]);
        AudioAfeConfiguration(AUDIO_SOURCE_I2S_MASTER_LOW_JITTER, open_param->stream_in_param.afe.is_low_jitter[0]);
        AudioAfeConfiguration(AUDIO_SOURCE_I2S_MASTER_LOW_JITTER1, open_param->stream_in_param.afe.is_low_jitter[1]);
        AudioAfeConfiguration(AUDIO_SOURCE_I2S_MASTER_LOW_JITTER2, open_param->stream_in_param.afe.is_low_jitter[2]);
        AudioAfeConfiguration(AUDIO_SOURCE_I2S_MASTER_LOW_JITTER3, open_param->stream_in_param.afe.is_low_jitter[3]);
        DSP_MW_LOG_I("i2s master configuration=%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", 16,
                     open_param->stream_in_param.afe.i2s_master_sampling_rate[0],
                     open_param->stream_in_param.afe.i2s_master_sampling_rate[1],
                     open_param->stream_in_param.afe.i2s_master_sampling_rate[2],
                     open_param->stream_in_param.afe.i2s_master_sampling_rate[3],
                     open_param->stream_in_param.afe.i2s_master_format[0],
                     open_param->stream_in_param.afe.i2s_master_format[1],
                     open_param->stream_in_param.afe.i2s_master_format[2],
                     open_param->stream_in_param.afe.i2s_master_format[3],
                     open_param->stream_in_param.afe.i2s_master_word_length[0],
                     open_param->stream_in_param.afe.i2s_master_word_length[1],
                     open_param->stream_in_param.afe.i2s_master_word_length[2],
                     open_param->stream_in_param.afe.i2s_master_word_length[3],
                     open_param->stream_in_param.afe.is_low_jitter[0],
                     open_param->stream_in_param.afe.is_low_jitter[1],
                     open_param->stream_in_param.afe.is_low_jitter[2],
                     open_param->stream_in_param.afe.is_low_jitter[3]);
    } else if (open_param->stream_in_param.afe.audio_device == HAL_AUDIO_CONTROL_DEVICE_I2S_SLAVE) {
        AudioAfeConfiguration(AUDIO_SOURCE_I2S_FORMAT, open_param->stream_in_param.afe.i2s_format);
        AudioAfeConfiguration(AUDIO_SOURCE_I2S_SLAVE_TDM, open_param->stream_in_param.afe.i2S_Slave_TDM);
        AudioAfeConfiguration(AUDIO_SOURCE_I2S_WORD_LENGTH, open_param->stream_in_param.afe.i2s_word_length);
    }
#endif

    AudioAfeConfiguration(AUDIO_SOURCE_HW_GAIN, open_param->stream_in_param.afe.hw_gain);
#if AUTO_ERROR_SUPPRESSION
    AudioAfeConfiguration(AUDIO_SOURCE_MISC_PARMS_I2S_CLK, open_param->stream_in_param.afe.misc_parms.I2sClkSourceType);
    AudioAfeConfiguration(AUDIO_SOURCE_MISC_PARMS_MICBIAS, open_param->stream_in_param.afe.misc_parms.MicbiasSourceType);
#endif
    AudioAfeConfiguration(AUDIO_SOURCE_MISC_PARMS, open_param->stream_in_param.afe.misc_parms);
    AudioAfeConfiguration(AUDIO_SOURCE_ECHO_REFERENCE, echo_path);
#ifdef AIR_HFP_DNN_PATH_ENABLE
    AudioAfeConfiguration(AUDIO_SOURCE_DNN_PATH_ENABLE, open_param->stream_in_param.afe.enable_ul_dnn);
#endif

    AudioAfeConfiguration(AUDIO_SOURCE_UPDOWN_SAMPLER_ENABLE, open_param->stream_in_param.afe.with_upwdown_sampler);
    AudioAfeConfiguration(AUDIO_SOURCE_PATH_INPUT_RATE, open_param->stream_in_param.afe.audio_path_input_rate);
    AudioAfeConfiguration(AUDIO_SOURCE_PATH_OUTPUT_RATE, open_param->stream_in_param.afe.audio_path_output_rate);
    DSP_MW_LOG_I("afe in format:%d, sampling rate:%d, IRQ period:%d mise:0x%x\r\n", 4, open_param->stream_in_param.afe.format,
                 gAudioCtrl.Afe.AfeULSetting.rate,
                 open_param->stream_in_param.afe.irq_period,
                 open_param->stream_in_param.afe.misc_parms);

    //TEMP!! should remove Audio_Default_setting_init
    Audio_setting->Audio_source.Buffer_Frame_Num = open_param->stream_in_param.afe.frame_number;
    Audio_setting->Audio_source.Frame_Size       = open_param->stream_in_param.afe.frame_size;

    if (open_param->stream_in_param.afe.memory & HAL_AUDIO_MEM_SUB) {
        //Sub-Source
        source = StreamAudioAfeSubSource(afe_get_audio_hardware_by_au_afe_open_param(&(open_param->stream_in_param.afe)),
                                         afe_get_audio_instance_by_au_afe_open_param(&(open_param->stream_in_param.afe)),
                                         afe_get_audio_channel_by_au_afe_open_param(&(open_param->stream_in_param.afe)));
#ifdef AIR_AUDIO_I2S_SLAVE_TDM_ENABLE
    } else if (open_param->stream_in_param.afe.memory & HAL_AUDIO_MEM7) {
        //Tdm-Source
        source = StreamAudioAfeTdmSource(afe_get_audio_hardware_by_au_afe_open_param(&(open_param->stream_in_param.afe)),
                                         afe_get_audio_instance_by_au_afe_open_param(&(open_param->stream_in_param.afe)),
                                         afe_get_audio_channel_by_au_afe_open_param(&(open_param->stream_in_param.afe)));
#endif
#ifdef AIR_I2S_SLAVE_ENABLE
    } else if (open_param->stream_in_param.afe.memory & HAL_AUDIO_MEM2) {
        DSP_MW_LOG_I("source enter HAL_AUDIO_MEM2", 0);
        source = StreamAudioAfe2Source(afe_get_audio_hardware_by_au_afe_open_param(&(open_param->stream_in_param.afe)),
                                       afe_get_audio_instance_by_au_afe_open_param(&(open_param->stream_in_param.afe)),
                                       afe_get_audio_channel_by_au_afe_open_param(&(open_param->stream_in_param.afe)));
#endif
    } else {
        source = StreamAudioAfeSource(afe_get_audio_hardware_by_au_afe_open_param(&(open_param->stream_in_param.afe)),
                                      afe_get_audio_instance_by_au_afe_open_param(&(open_param->stream_in_param.afe)),
                                      afe_get_audio_channel_by_au_afe_open_param(&(open_param->stream_in_param.afe)));
    }

    if (source != NULL) {

    } else {
        DSP_MW_LOG_E("DSP source create fail\r\n", 0);
    }
    return source;

}
#endif /* AIR_AUDIO_HARDWARE_ENABLE */

#ifdef AIR_BT_HFP_ENABLE

#ifdef AIR_VOICE_NR_ENABLE
#ifdef AIR_AIRDUMP_ENABLE
extern AIRDUMPCTRL_t *rAirDumpCtrl;
#endif
#endif

CONNECTION_IF n9_sco_dl_if;

SOURCE dsp_open_stream_in_hfp(mcu2dsp_open_param_p open_param)
{
    SOURCE source;
    DSP_MW_LOG_I("Stream in hfp\r\n", 0);
    DSP_MW_LOG_I("hfp in Codec:%d\r\n", 1, open_param->stream_in_param.hfp.codec_type);

    if (open_param->stream_in_param.hfp.codec_type == BT_HFP_CODEC_CVSD) {
        DSP_ALG_UpdateEscoRxMode(VOICE_NB);
        n9_sco_dl_if.pfeature_table = (stream_feature_list_ptr_t)&stream_feature_list_hfp_downlink_cvsd;
    } else if (open_param->stream_in_param.hfp.codec_type  == BT_HFP_CODEC_mSBC) {
        DSP_ALG_UpdateEscoRxMode(VOICE_WB);
        n9_sco_dl_if.pfeature_table = (stream_feature_list_ptr_t)&stream_feature_list_hfp_downlink_msbc;
    } else {
        //not support codec type
    }

    source = StreamN9ScoSource(open_param->stream_in_param.hfp.p_share_info);
    if (source != NULL) {
        source->streamBuffer.AVMBufferInfo.SampleRate = 16000;
        source->param.n9sco.share_info_base_addr->SampleRate = 16000;
        AudioAfeConfiguration(AUDIO_SINK_IRQ_RATE, 16000);
    } else {
        DSP_MW_LOG_E("DSP source create fail\r\n", 0);
    }

#ifdef AIR_VOICE_NR_ENABLE
#ifdef AIR_AIRDUMP_ENABLE
    rAirDumpCtrl = (AIRDUMPCTRL_t *)open_param->stream_in_param.hfp.p_air_dump_buf;
#endif
#endif

#ifdef AIR_DUAL_CHIP_MIXING_MODE_ROLE_SLAVE_ENABLE
#ifdef AIR_SOFTWARE_BUFFER_ENABLE
    sw_buffer_config_t buffer_config;
    buffer_config.mode = SW_BUFFER_MODE_FIXED_OUTPUT_LENGTH;
    buffer_config.total_channels = 2;
    buffer_config.watermark_max_size = 240 * 4 * sizeof(int16_t); /* must be 15ms*N */
    buffer_config.watermark_min_size = 0;
    buffer_config.output_size = 240 * sizeof(int16_t);
    source->param.n9sco.sw_buffer_port = stream_function_sw_buffer_get_port(source);
    stream_function_sw_buffer_init(source->param.n9sco.sw_buffer_port, &buffer_config);
    stream_function_sw_buffer_config_channel_prefill_size(source->param.n9sco.sw_buffer_port, 2, 120 * sizeof(int16_t), true);
#endif /* AIR_SOFTWARE_BUFFER_ENABLE */
#endif
    return source;
}
#endif

#ifdef AIR_BT_CODEC_BLE_ENABLED
#ifdef AIR_BT_LE_LC3_ENABLE
LC3I_DL_PIC_Type_T lc3_dl_pic_type;
LC3I_UL_PIC_Type_T lc3_ul_pic_type;
#ifdef AIR_DSP_MEMORY_REGION_ENABLE
GroupMemoryInfo_t lc3_plc_memory_addr;
GroupMemoryInfo_t lc3_fft_memory_addr;
#endif
#endif
SOURCE dsp_open_stream_in_ble(mcu2dsp_open_param_p open_param)
{
    SOURCE source;
#ifdef AIR_DSP_MEMORY_REGION_ENABLE
    DspMemoryManagerReturnStatus_t memory_return_status;
    memset(&lc3_plc_memory_addr, 0, sizeof(GroupMemoryInfo_t));
    memset(&lc3_fft_memory_addr, 0, sizeof(GroupMemoryInfo_t));
#endif
    void *code_addr_plc = NULL;
    void *data_addr_plc = NULL;
    void *code_addr_fft = NULL;
    void *data_addr_fft = NULL;
    uint32_t pic_dram_usage = 0;

    DSP_MW_LOG_I("[BLE] Stream in ble, Codec:%d", 1, open_param->stream_in_param.ble.codec_type);

#ifdef AIR_BT_LE_LC3PLUS_ENABLE
    if(open_param->stream_in_param.ble.context_type == BLE_CONTENT_TYPE_ULL_BLE) {
        n9_ble_dl_if.pfeature_table = AudioFeatureList_ULL_BLE_DL;
        if (open_param->stream_in_param.ble.codec_type == BT_BLE_CODEC_LC3PLUS)
        {
            stream_feature_configure_type(n9_ble_dl_if.pfeature_table, CODEC_DECODER_LC3PLUS, CONFIG_DECODER);
            stream_feature_configure_resolution((stream_feature_list_ptr_t)n9_ble_dl_if.pfeature_table, RESOLUTION_32BIT, CONFIG_DECODER);
        }
    }
    else
#endif
    {
        if (open_param->stream_in_param.ble.codec_type == BT_BLE_CODEC_LC3) {
            #ifdef AIR_BT_LE_LC3_ENABLE
    if (open_param->stream_in_param.ble.decode_mode == CODEC_AUDIO_MODE) {
        n9_ble_dl_if.pfeature_table = AudioFeatureList_BLE_Music_DL;
    } else {
        n9_ble_dl_if.pfeature_table = AudioFeatureList_BLE_Call_DL;
    }
        stream_feature_configure_type(n9_ble_dl_if.pfeature_table, CODEC_DECODER_LC3, CONFIG_DECODER);
        lc3_dual_decode_mode_set(open_param->stream_in_param.ble.channel_num == 2);
        pic_dram_usage = 0;

#ifdef PRELOADER_ENABLE
    //#ifdef AIR_DSP_MEMORY_REGION_ENABLE
        switch (open_param->stream_in_param.ble.context_type) {
            case BLE_CONTEXT_CONVERSATIONAL:
#ifdef AIR_DSP_MEMORY_REGION_ENABLE
                    memory_return_status = DspMemoryManager_AcquireGroupMemory(Component_LE_CALL,SubComponent_LECALL_LC3_PLC_STD,&lc3_plc_memory_addr);
                    code_addr_plc = lc3_plc_memory_addr.IramAddr;
                    data_addr_plc = lc3_plc_memory_addr.DramAddr;
                    memory_return_status = DspMemoryManager_AcquireGroupMemory(Component_LE_CALL,SubComponent_LECALL_LC3_FFT,&lc3_fft_memory_addr);
                    code_addr_fft = lc3_fft_memory_addr.IramAddr;
                    data_addr_fft = lc3_fft_memory_addr.DramAddr;
#endif
                    lc3i_plc_std_library_load(code_addr_plc,data_addr_plc,&pic_dram_usage);

                if (open_param->stream_in_param.ble.frame_ms == 10000) {
                        lc3i_fft10ms_library_load(code_addr_fft,data_addr_fft,&pic_dram_usage);
                    lc3_dl_pic_type = LC3I_DL_CALL_10_MS;
                } else if (open_param->stream_in_param.ble.frame_ms == 7500) {
                        lc3i_fft7_5ms_library_load(code_addr_fft,data_addr_fft,&pic_dram_usage);
                    lc3_dl_pic_type = LC3I_DL_CALL_7P5_MS;
                } else {
                    DSP_MW_LOG_E("[BLE] unsupported DL frame interval %d", 1, open_param->stream_in_param.ble.frame_ms);
                }
                break;

            default:
#ifdef AIR_DSP_MEMORY_REGION_ENABLE
                    memory_return_status = DspMemoryManager_AcquireGroupMemory(Component_LE_AUDIO,SubComponent_LEAUDIO_LC3_PLC_ADV,&lc3_plc_memory_addr);
                    code_addr_plc = lc3_plc_memory_addr.IramAddr;
                    data_addr_plc = lc3_plc_memory_addr.DramAddr;
                    memory_return_status = DspMemoryManager_AcquireGroupMemory(Component_LE_AUDIO,SubComponent_LEAUDIO_LC3_FFT,&lc3_fft_memory_addr);
                    code_addr_fft = lc3_fft_memory_addr.IramAddr;
                    data_addr_fft = lc3_fft_memory_addr.DramAddr;
#endif
                    lc3i_plc_adv_library_load(code_addr_plc,data_addr_plc,&pic_dram_usage);
                if (open_param->stream_in_param.ble.frame_ms == 10000) {
                        lc3i_fft10ms_library_load(code_addr_fft,data_addr_plc,&pic_dram_usage);
                    lc3_dl_pic_type = LC3I_DL_AUDIO_10_MS;
                } else if (open_param->stream_in_param.ble.frame_ms == 7500) {
                        lc3i_fft7_5ms_library_load(code_addr_fft,data_addr_plc,&pic_dram_usage);
                    lc3_dl_pic_type = LC3I_DL_AUDIO_7P5_MS;
                } else {
                    DSP_MW_LOG_E("[BLE] unsupported DL frame interval %d", 1, open_param->stream_in_param.ble.frame_ms);
                }
                break;
        }
#endif
#endif
        }
            
    }

#ifdef AIR_VOICE_NR_ENABLE
#ifdef AIR_AIRDUMP_ENABLE
        rAirDumpCtrl = (AIRDUMPCTRL_t *)open_param->stream_in_param.ble.p_air_dump_buf;
#endif
#endif

    source = StreamN9BleSource(open_param);

    return source;
}
#endif

#ifdef MTK_BT_A2DP_ENABLE
SOURCE dsp_open_stream_in_a2dp(mcu2dsp_open_param_p open_param)
{
    SOURCE source;
    U32 samplerate = 0, channel = 0;

    DSP_MW_LOG_I("Stream In A2DP\r\n", 0);
    source  = StreamN9A2dpSource(&open_param->stream_in_param.a2dp);

    if (source != NULL) {
        /* parse codec info */
        samplerate  = a2dp_get_samplingrate(source);
        channel     = a2dp_get_channel(source);

        DSP_MW_LOG_I("A2DP codec type: %d, sr: %d, SL: %d\r\n", 3, source->param.n9_a2dp.codec_info.codec_cap.type, samplerate, source->param.n9_a2dp.sink_latency);
        n9_a2dp_if.pfeature_table = stream_feature_list_a2dp;
        if (source->param.n9_a2dp.codec_info.codec_cap.type == BT_A2DP_CODEC_VENDOR) {
            DSP_UpdateA2DPCodec(BT_A2DP_CODEC_VENDOR);
            DSP_MW_LOG_I("A2DP codec :%d", 1, source->param.n9_a2dp.codec_info.codec_cap.type);
#if !defined(MTK_BT_A2DP_VENDOR_ENABLE) && !defined(MTK_BT_A2DP_VENDOR_1_ENABLE)
            DSP_MW_LOG_E("A2DP request unsupported codec", 0);
            AUDIO_ASSERT(0);
#endif
#ifdef MTK_BT_A2DP_VENDOR_BC_ENABLE
            if (source->param.n9_a2dp.sink_latency == 0) {
                n9_a2dp_if.pfeature_table = stream_feature_list_vend_a2dp;
            }
#endif
#ifdef MTK_BT_A2DP_VENDOR_1_ENABLE
            stream_feature_configure_type(n9_a2dp_if.pfeature_table, CODEC_DECODER_VENDOR_1, CONFIG_DECODER);
#else
            stream_feature_configure_type(n9_a2dp_if.pfeature_table, CODEC_DECODER_VENDOR, CONFIG_DECODER);
#endif
            //not support codec type
#ifdef MTK_AUDIO_PLC_ENABLE
            Audio_PLC_ctrl(open_param->stream_in_param.a2dp.audio_plc);
#endif
        } else if (source->param.n9_a2dp.codec_info.codec_cap.type == BT_A2DP_CODEC_SBC) {
            DSP_UpdateA2DPCodec(BT_A2DP_CODEC_SBC);
            stream_feature_configure_type(n9_a2dp_if.pfeature_table, CODEC_DECODER_SBC, CONFIG_DECODER);
#ifdef MTK_AUDIO_PLC_ENABLE
            Audio_PLC_ctrl(open_param->stream_in_param.a2dp.audio_plc);
#endif
        } else if (source->param.n9_a2dp.codec_info.codec_cap.type == BT_A2DP_CODEC_AAC) {
            DSP_UpdateA2DPCodec(BT_A2DP_CODEC_AAC);
            stream_feature_configure_type(n9_a2dp_if.pfeature_table, CODEC_DECODER_AAC, CONFIG_DECODER);
#ifdef MTK_AUDIO_PLC_ENABLE
            Audio_PLC_ctrl(open_param->stream_in_param.a2dp.audio_plc);
#endif
        }
#ifdef MTK_GAMING_MODE_HEADSET
        else if (source->param.n9_a2dp.codec_info.codec_cap.type == BT_A2DP_CODEC_AIRO_CELT) {
            samplerate = 48000;
            DSP_UpdateA2DPCodec(BT_A2DP_CODEC_AIRO_CELT);
            stream_feature_configure_type(n9_a2dp_if.pfeature_table, CODEC_DECODER_CELT_HD, CONFIG_DECODER);
        }
#endif

        source->streamBuffer.AVMBufferInfo.SampleRate = samplerate;
        memcpy(&(((n9_dsp_share_info_ptr)(source->param.n9_a2dp.share_info_base_addr))->sampling_rate), &(source->streamBuffer.AVMBufferInfo.SampleRate), 4);/* update sample_rate */
#ifdef AIR_AUDIO_HARDWARE_ENABLE
        AudioAfeConfiguration(AUDIO_SINK_IRQ_RATE, samplerate);
#endif /* AIR_AUDIO_HARDWARE_ENABLE */

#ifdef AIR_BT_CLK_SKEW_ENABLE
        /* long-term clock skew */
        lt_clk_skew_reset_info();
        lt_clk_skew_set_sample_rate(samplerate);
        lt_clk_skew_set_asi_buf(source->param.n9_a2dp.asi_buf);
        lt_clk_skew_set_min_gap_buf(source->param.n9_a2dp.min_gap_buf);
        lt_clk_skew_set_sink_latency(source->param.n9_a2dp.sink_latency);
#endif /* AIR_BT_CLK_SKEW_ENABLE */

        source->param.n9_a2dp.p_afe_buf_report = open_param->stream_in_param.a2dp.p_afe_buf_report;// store afe buffer report instead
#ifdef MTK_PEQ_ENABLE
        PEQ_Reset_Info();
#endif

    } else {
        DSP_MW_LOG_E("DSP source create fail\r\n", 0);
    }

#ifdef AIR_DUAL_CHIP_MIXING_MODE_ROLE_SLAVE_ENABLE
#ifdef AIR_SOFTWARE_BUFFER_ENABLE
    U32 frame_size = 0;
    if (source->param.n9_a2dp.codec_info.codec_cap.type == BT_A2DP_CODEC_SBC){
        frame_size = 128;
    }else if(source->param.n9_a2dp.codec_info.codec_cap.type == BT_A2DP_CODEC_AAC){
        frame_size = 1024;
    }else if(source->param.n9_a2dp.codec_info.codec_cap.type == BT_A2DP_CODEC_VENDOR){
        frame_size = (samplerate <= 48000) ? 128 : 256;
    }
    sw_buffer_config_t buffer_config;
    buffer_config.mode = SW_BUFFER_MODE_FIXED_OUTPUT_LENGTH;
    buffer_config.total_channels = 2;
    buffer_config.watermark_max_size = frame_size *  sizeof(int32_t) * 4; /* must be 15ms*N */
    buffer_config.watermark_min_size = 0;
    buffer_config.output_size = frame_size *  sizeof(int32_t);
    source->param.n9_a2dp.sw_buffer_port = stream_function_sw_buffer_get_port(source);
    stream_function_sw_buffer_init(source->param.n9_a2dp.sw_buffer_port, &buffer_config);
    stream_function_sw_buffer_config_channel_prefill_size(source->param.n9_a2dp.sw_buffer_port, 2, frame_size *  sizeof(int32_t) , true);
    DSP_MW_LOG_I("[DCHS] a2dp sw buffer prefill size=%d\r\n", 1, frame_size *  sizeof(int32_t));

#endif /* AIR_SOFTWARE_BUFFER_ENABLE */
#endif
    return source;
}
#endif /* MTK_BT_A2DP_ENABLE */

SOURCE dsp_open_stream_in_playback(mcu2dsp_open_param_p open_param)
{
    SOURCE source;
    U32 sample_rate;
    DSP_MW_LOG_I("Stream in playback\r\n", 0);

    source = StreamCM4PlaybackSource(&open_param->stream_in_param.playback);

    if (source) {
        sample_rate = source->param.cm4_playback.info.sampling_rate;

        /* Yo: Should switch to VP afe sink later */
        source->streamBuffer.ShareBufferInfo.sampling_rate = sample_rate;
#ifdef AIR_AUDIO_HARDWARE_ENABLE
        AudioAfeConfiguration(AUDIO_SINK_IRQ_RATE, sample_rate);
#endif /* AIR_AUDIO_HARDWARE_ENABLE */
        playback_if.pfeature_table = (stream_feature_list_ptr_t)&stream_feature_list_playback;
        stream_feature_configure_type(playback_if.pfeature_table, CODEC_PCM_COPY, CONFIG_DECODER);
    } else {
        DSP_MW_LOG_E("DSP source create fail\r\n", 0);
    }

    return source;
}

SOURCE dsp_open_stream_in_vp(mcu2dsp_open_param_p open_param)
{
    SOURCE source;
    U32 sample_rate;
    DSP_MW_LOG_I("Stream in vp\r\n", 0);
#ifdef MTK_PROMPT_SOUND_ENABLE
    source = StreamCM4VPPlaybackSource(&open_param->stream_in_param.playback);
#endif
    if (source) {
        sample_rate = source->param.cm4_playback.info.sampling_rate;

        /* Yo: Should switch to VP afe sink later */
        source->streamBuffer.ShareBufferInfo.sampling_rate = sample_rate;
#ifdef AIR_AUDIO_HARDWARE_ENABLE
        AudioAfeConfiguration(AUDIO_SINK_IRQ_RATE, sample_rate);
#endif /* AIR_AUDIO_HARDWARE_ENABLE */

        stream_feature_configure_type(stream_feature_list_prompt, CODEC_PCM_COPY, CONFIG_DECODER);
    } else {
        DSP_MW_LOG_E("DSP source create fail\r\n", 0);
    }

    return source;
}

#ifdef MTK_PROMPT_SOUND_DUMMY_SOURCE_ENABLE
SOURCE dsp_open_stream_in_vp_dummy_source(mcu2dsp_open_param_p open_param)
{
    SOURCE source;
    U32 sample_rate;
    DSP_MW_LOG_I("Stream in VP Dummy Source\r\n", 0);

    source = StreamCM4VPDummySourcePlaybackSource(&open_param->stream_in_param.playback);

    if (source) {
        sample_rate = source->param.cm4_playback.info.sampling_rate;

        /* Should switch to VP afe sink later */
        source->streamBuffer.ShareBufferInfo.sample_rate = sample_rate;
        AudioAfeConfiguration(AUDIO_SINK_IRQ_RATE, sample_rate);

        stream_feature_configure_type(stream_feature_list_prompt_dummy_source, CODEC_PCM_COPY, CONFIG_DECODER);
    } else {
        DSP_MW_LOG_I("DSP source create fail\r\n", 0);
    }

    return source;
}
#endif

#ifdef MTK_SENSOR_SOURCE_ENABLE
SOURCE dsp_open_stream_in_gsensor(mcu2dsp_open_param_p open_param)
{
    SOURCE source;

    open_param = open_param;

    source = StreamGsensorSource();

    return source;
}
#endif

#if defined AIR_DUAL_CHIP_MIXING_MODE_ROLE_MASTER_ENABLE || defined AIR_DUAL_CHIP_MIXING_MODE_ROLE_SLAVE_ENABLE
SOURCE dsp_open_stream_in_uart(mcu2dsp_open_param_p open_param)
{
    SOURCE source = NULL;
    DSP_MW_LOG_I("[DSP][DCHS]Stream in uart\r\n", 0);

    source = StreamUartSource(open_param);
    if(!source){
        DSP_MW_LOG_E("[DSP][DCHS]uart source create fail\r\n", 0);
    }
    g_dchs_dl_open_flag = true;
    return source;
}

void dsp_start_stream_in_uart(mcu2dsp_start_param_p start_param, SOURCE source)
{
    if(Source_blks[SOURCE_TYPE_A2DP]){
        DSP_MW_LOG_I("[DCHS]a2dp exist",0);
        return;//if local chip have a2dp, will use the same play en value
    }
    U32 bt_clk, intra_clk;
    U32 predict_asi = start_param->stream_in_param.a2dp.start_asi;
    MCE_GetBtClk((BTCLK *)&bt_clk, (BTPHASE *)&intra_clk, BT_CLK_Offset);
    DSP_MW_LOG_I("[DCHS]Get host set asi:0x%x b:0x%x i:0x%x", 3, start_param->stream_in_param.a2dp.start_asi, start_param->stream_in_param.a2dp.start_bt_clk, start_param->stream_in_param.a2dp.start_bt_intra_clk);
    DSP_MW_LOG_I("[DCHS]Get local clk, n:0x%x b:0x%x i:0x%x", 3, rBb->rClkCtl.rNativeClock, bt_clk, intra_clk);
    start_param->stream_in_param.a2dp.start_bt_intra_clk = (start_param->stream_in_param.a2dp.start_bt_intra_clk & 0xFFFF) << 1;

    if (abs32((S32)(start_param->stream_in_param.a2dp.start_bt_clk - bt_clk)) >  0x2000) { // Maximum tolerence time difference = 0x2000*312.5(us) = 2.56(s)
        DSP_MW_LOG_I("[DCHS]Host notify play time abnormal, bt_clk: 0x%x play_clk: 0x%x", 2, bt_clk, start_param->stream_in_param.a2dp.start_bt_clk);
        start_param->stream_in_param.a2dp.start_bt_clk = bt_clk + 0x100;
        //AUDIO_ASSERT(0);
    }
    if ((S32)(start_param->stream_in_param.a2dp.start_bt_clk - bt_clk) > 0x10) { // check play time in time
        DSP_MW_LOG_I("[DCHS]Set legal play en", 0);
    } else {
        U32 temp_start_bt_clk = start_param->stream_in_param.a2dp.start_bt_clk;
        while ((S32)(temp_start_bt_clk - bt_clk) < 0x80) {
            predict_asi += 1024;
            temp_start_bt_clk +=  3276800 / (source->param.audio.rate); // roughly calc
        }
        DSP_MW_LOG_E("[DCHS]Set legal play en, rate=%d", 1,source->param.audio.rate);
        start_param->stream_in_param.a2dp.start_bt_clk += ((((predict_asi - start_param->stream_in_param.a2dp.start_asi) << 5) * 100 / (source->param.audio.rate)) & 0xFFFFFFC);
        start_param->stream_in_param.a2dp.start_bt_intra_clk += ((((predict_asi - start_param->stream_in_param.a2dp.start_asi)) * 20000) / (source->param.audio.rate / 100)) % 2500;
        DSP_MW_LOG_I("[DCHS]Host notify play time too early, modify play en b:0x%x i:0x%x", 2, start_param->stream_in_param.a2dp.start_bt_clk, start_param->stream_in_param.a2dp.start_bt_intra_clk);
    }
    MCE_TransBT2NativeClk(start_param->stream_in_param.a2dp.start_bt_clk, start_param->stream_in_param.a2dp.start_bt_intra_clk, &bt_clk, &intra_clk, BT_CLK_Offset);
    DSP_MW_LOG_I("[DCHS]Play en b:0x%x i:0x%x n:0x%x asi: 0x%x", 4, bt_clk, intra_clk, rBb->rClkCtl.rNativeClock, source->param.n9_a2dp.predict_asi);
#ifdef AIR_AUDIO_HARDWARE_ENABLE
    hal_audio_afe_set_play_en(bt_clk, intra_clk);
#endif /* AIR_AUDIO_HARDWARE_ENABLE */
}
#endif

#ifdef AIR_AUDIO_TRANSMITTER_ENABLE
SOURCE dsp_open_stream_in_audio_transmitter(mcu2dsp_open_param_p open_param)
{
    SOURCE source;

    DSP_MW_LOG_I("Stream in aduio transmitter\r\n", 0);

    source = StreamAudioTransmitterSource(&(open_param->stream_in_param.data_dl));
    if (source) {
        DSP_MW_LOG_I("DSP source create successfully\r\n", 0);
    } else {
        DSP_MW_LOG_E("DSP source create fail\r\n", 0);
    }

    return source;
}
#endif /* AIR_AUDIO_TRANSMITTER_ENABLE */

#ifdef AIR_AUDIO_BT_COMMON_ENABLE
SOURCE dsp_open_stream_in_bt_common(mcu2dsp_open_param_p open_param)
{
    SOURCE source;

    DSP_MW_LOG_I("Stream in bt common\r\n", 0);

    source = StreamBTCommonSource(&(open_param->stream_in_param.bt_common));

    if (source) {
        DSP_MW_LOG_I("DSP source create successfully\r\n", 0);
    } else {
        DSP_MW_LOG_E("DSP source create fail\r\n", 0);
    }

    return source;
}
#endif /* AIR_AUDIO_BT_COMMON_ENABLE */

SOURCE dsp_open_stream_in(mcu2dsp_open_param_p open_param)
{
    SOURCE source = NULL;
    if (open_param != NULL) {
#ifdef AIR_AUDIO_HARDWARE_ENABLE
        AudioAfeConfiguration(AUDIO_SOURCE_SCENARIO_TYPE, open_param->audio_scenario_type);
#endif
        switch (open_param->param.stream_in) {
#ifdef AIR_AUDIO_HARDWARE_ENABLE
            case STREAM_IN_AFE:
                source = dsp_open_stream_in_afe(open_param);
                break;
#endif /* AIR_AUDIO_HARDWARE_ENABLE */
#ifdef AIR_BT_HFP_ENABLE
            case STREAM_IN_HFP:
                source = dsp_open_stream_in_hfp(open_param);
                break;
#endif
#ifdef AIR_BT_CODEC_BLE_ENABLED
            case STREAM_IN_BLE:
                source = dsp_open_stream_in_ble(open_param);
                break;
#endif
#ifdef MTK_BT_A2DP_ENABLE
            case STREAM_IN_A2DP:
                source = dsp_open_stream_in_a2dp(open_param);
                break;
#endif /* MTK_BT_A2DP_ENABLE */
            case STREAM_IN_PLAYBACK:
                source = dsp_open_stream_in_playback(open_param);
                break;
            case STREAM_IN_VP:
                source = dsp_open_stream_in_vp(open_param);
                break;
#ifdef MTK_PROMPT_SOUND_DUMMY_SOURCE_ENABLE
            case STREAM_IN_VP_DUMMY_SOURCE:
                source = dsp_open_stream_in_vp_dummy_source(open_param);
                break;
#endif
#ifdef MTK_SENSOR_SOURCE_ENABLE
            case STREAM_IN_GSENSOR:
                source = dsp_open_stream_in_gsensor(open_param);
                break;
#endif
#ifdef AIR_AUDIO_TRANSMITTER_ENABLE
            case STREAM_IN_AUDIO_TRANSMITTER:
                source = dsp_open_stream_in_audio_transmitter(open_param);
                break;
#endif /* AIR_AUDIO_TRANSMITTER_ENABLE */
#ifdef AIR_AUDIO_BT_COMMON_ENABLE
            case STREAM_IN_BT_COMMON:
                source = dsp_open_stream_in_bt_common(open_param);
                break;
#endif /* AIR_AUDIO_BT_COMMON_ENABLE */
#ifdef AIR_FULL_ADAPTIVE_ANC_ENABLE
            case STREAM_IN_ADAPT_ANC:
                source = dsp_open_stream_in_adapt_anc(open_param);
                break;
#endif
#if defined AIR_DUAL_CHIP_MIXING_MODE_ROLE_MASTER_ENABLE || defined AIR_DUAL_CHIP_MIXING_MODE_ROLE_SLAVE_ENABLE
            case STREAM_IN_UART:
                source = dsp_open_stream_in_uart(open_param);
                break;
#endif
            default:
                break;
        }
    }

    if (source) {
        source->scenario_type= open_param->audio_scenario_type;
    }
    return source;
}

#ifdef AIR_BT_HFP_ENABLE
CONNECTION_IF n9_sco_ul_if;

SINK dsp_open_stream_out_hfp(mcu2dsp_open_param_p open_param)
{
    SINK sink;
    DSP_MW_LOG_I("Stream out hfp\r\n", 0);
    DSP_MW_LOG_I("hfp out Codec:%d\r\n", 1, open_param->stream_out_param.hfp.codec_type);

    if (open_param->stream_out_param.hfp.codec_type == BT_HFP_CODEC_CVSD) {
        DSP_ALG_UpdateEscoTxMode(VOICE_NB);
        /* Downsample from 16K to 8K, and encode in controller side */
        stream_feature_configure_type(stream_feature_list_hfp_uplink, FUNC_SRC_FIXED_RATIO, CONFIG_ENCODER_REPLACE);
    } else if (open_param->stream_out_param.hfp.codec_type  == BT_HFP_CODEC_mSBC) {
        DSP_ALG_UpdateEscoTxMode(VOICE_WB);
        stream_feature_configure_type(stream_feature_list_hfp_uplink, CODEC_ENCODER_MSBC, CONFIG_ENCODER_REPLACE);
    } else {
        //not support codec type
    }
    n9_sco_ul_if.pfeature_table = (stream_feature_list_ptr_t)&stream_feature_list_hfp_uplink;

    sink = StreamN9ScoSink(open_param->stream_out_param.hfp.p_share_info);
    if (sink != NULL) {

    } else {
        DSP_MW_LOG_E("DSP sink create fail\r\n", 0);
    }

    return sink;
}
#endif

#ifdef AIR_BT_CODEC_BLE_ENABLED
SINK dsp_open_stream_out_ble(mcu2dsp_open_param_p open_param)
{
    SINK sink;

    DSP_MW_LOG_I("[BLE] Stream out ble, Codec:%d", 1, open_param->stream_out_param.ble.codec_type);

    /*  Tx Rx mode must be aligned otherwise it would cause NVkey assert*/
    /*  Currently Rx mode dominates the initialization process*/
#ifdef AIR_BT_BLE_SWB_ENABLE
    DSP_ALG_UpdateEscoTxMode(VOICE_SWB);         /*32K sample rate. Need SWB algorithm*/
    DSP_ALG_UpdateEscoRxMode(VOICE_SWB);         /*32K sample rate. Need SWB algorithm*/
#else
    DSP_ALG_UpdateEscoTxMode(VOICE_WB);         /*16K sample rate. Need WB algorithm*/
    DSP_ALG_UpdateEscoRxMode(VOICE_WB);         /*16K sample rate. Need WB algorithm*/
#endif

#ifdef AIR_BT_LE_LC3PLUS_ENABLE
    if (open_param->stream_out_param.ble.context_type == BLE_CONTENT_TYPE_ULL_BLE)
    {
        if (open_param->stream_out_param.ble.codec_type == BT_BLE_CODEC_LC3PLUS)
        {
            n9_ble_ul_if.pfeature_table = AudioFeatureList_ULL_BLE_UL;
            stream_feature_configure_type(n9_ble_ul_if.pfeature_table, CODEC_ENCODER_LC3PLUS, CONFIG_ENCODER);
        }
    } else if (open_param->stream_out_param.ble.context_type == BLE_CONTENT_TYPE_WIRELESS_MIC) {
        n9_ble_ul_if.pfeature_table = AudioFeatureList_WirelessMic;
        stream_feature_configure_type(n9_ble_ul_if.pfeature_table, CODEC_ENCODER_LC3PLUS, CONFIG_ENCODER);
    } else
#endif
    {
    if (open_param->stream_out_param.ble.codec_type == BT_BLE_CODEC_LC3) {
            #ifdef AIR_BT_LE_LC3_ENABLE
            n9_ble_ul_if.pfeature_table = AudioFeatureList_BLE_Call_UL;
        stream_feature_configure_type(n9_ble_ul_if.pfeature_table, CODEC_ENCODER_LC3, CONFIG_ENCODER);
#ifdef PRELOADER_ENABLE
        lc3i_plc_std_library_load();
        if(open_param->stream_out_param.ble.frame_ms == 10000){
            lc3i_fft10ms_library_load();
            lc3_ul_pic_type = LC3I_UL_CALL_10_MS;

        }else if(open_param->stream_out_param.ble.frame_ms == 7500){
            lc3i_fft7_5ms_library_load();
            lc3_ul_pic_type = LC3I_UL_CALL_7P5_MS;
        }else{
            DSP_MW_LOG_E("[BLE] unsupported UL frame interval %d",1,open_param->stream_out_param.ble.frame_ms);
        }
            #endif
#endif
    }
    }

    sink = StreamN9BleSink(open_param);

    return sink;
}
#endif

#ifdef AIR_AUDIO_HARDWARE_ENABLE
SINK dsp_open_stream_out_afe(mcu2dsp_open_param_p open_param)
{
    SINK sink = NULL;

    //[TEMP]: Add AT Cmd to switch I2S mode
    if (((*((volatile uint32_t *)(0xA2120B04)) >> 2) & 0x01) == 1) {
        open_param->stream_out_param.afe.audio_device = (hal_audio_device_t)HAL_AUDIO_CONTROL_DEVICE_SPDIF;//HAL_AUDIO_DEVICE_I2S_MASTER;
        open_param->stream_out_param.afe.audio_interface = HAL_AUDIO_INTERFACE_1;
    }

    //DSP_MW_LOG_I("Stream out afe\r\n", 1);
#ifdef ENABLE_HWSRC_CLKSKEW
    DSP_MW_LOG_I("afe out device:%d, channel:%d, memory:%d, interface:%d, hw_gain:%d, adc_mode:%d, performance:%d,clkskew_mode:%d \r\n", 8, open_param->stream_out_param.afe.audio_device,
                 open_param->stream_out_param.afe.stream_channel,
                 open_param->stream_out_param.afe.memory,
                 open_param->stream_out_param.afe.audio_interface,
                 open_param->stream_out_param.afe.hw_gain,
                 open_param->stream_out_param.afe.adc_mode,
                 open_param->stream_out_param.afe.performance,
                 open_param->stream_out_param.afe.clkskew_mode);
#else
    DSP_MW_LOG_I("afe out device:%d, channel:%d, memory:%d, interface:%d, hw_gain:%d, adc_mode:%d, performance:%d \r\n", 8, open_param->stream_out_param.afe.audio_device,
                 open_param->stream_out_param.afe.stream_channel,
                 open_param->stream_out_param.afe.memory,
                 open_param->stream_out_param.afe.audio_interface,
                 open_param->stream_out_param.afe.hw_gain,
                 open_param->stream_out_param.afe.adc_mode,
                 open_param->stream_out_param.afe.performance);
#endif

    AudioAfeConfiguration(AUDIO_SINK_DATA_FORMAT, open_param->stream_out_param.afe.format);
    if (open_param->stream_out_param.afe.sampling_rate != 0) {
        AudioAfeConfiguration(AUDIO_SINK_IRQ_RATE, open_param->stream_out_param.afe.sampling_rate);
    }

#if defined(ENABLE_HWSRC_ON_MAIN_STREAM) || defined(MTK_HWSRC_IN_STREAM)
    AudioAfeConfiguration(AUDIO_SRC_RATE, open_param->stream_out_param.afe.stream_out_sampling_rate);
#endif
    AudioAfeConfiguration(AUDIO_SINK_IRQ_PERIOD, open_param->stream_out_param.afe.irq_period);

    AudioAfeConfiguration(AUDIO_SINK_DEVICE, open_param->stream_out_param.afe.audio_device);
    AudioAfeConfiguration(AUDIO_SINK_CHANNEL, open_param->stream_out_param.afe.stream_channel);
    AudioAfeConfiguration(AUDIO_SINK_MEMORY, open_param->stream_out_param.afe.memory);
    AudioAfeConfiguration(AUDIO_SINK_INTERFACE, open_param->stream_out_param.afe.audio_interface);
    AudioAfeConfiguration(AUDIO_SINK_HW_GAIN, open_param->stream_out_param.afe.hw_gain);
#ifdef ENABLE_HWSRC_CLKSKEW
    if (open_param->stream_out_param.afe.memory == HAL_AUDIO_MEM1) {
        if (ClkSkewMode_isModify_g == TRUE) {
            AudioAfeConfiguration(AUDIO_SINK_CLKSKEW_MODE, ClkSkewMode_g);
        } else {
            AudioAfeConfiguration(AUDIO_SINK_CLKSKEW_MODE, open_param->stream_out_param.afe.clkskew_mode);
            ClkSkewMode_g = open_param->stream_out_param.afe.clkskew_mode;
        }
        DSP_MW_LOG_I("afe clkskew_mode:%d %d", 2, open_param->stream_out_param.afe.clkskew_mode, ClkSkewMode_g);
    }
#endif
#ifdef HAL_AUDIO_ENABLE_PATH_MEM_DEVICE
    AudioAfeConfiguration(AUDIO_SINK_ADC_MODE, open_param->stream_out_param.afe.dl_dac_mode);
    if (open_param->stream_out_param.afe.audio_device & (HAL_AUDIO_CONTROL_DEVICE_I2S_MASTER | HAL_AUDIO_CONTROL_DEVICE_I2S_MASTER_L | HAL_AUDIO_CONTROL_DEVICE_I2S_MASTER_R)) {
        AudioAfeConfiguration(AUDIO_SINK_I2S_MASTER_SAMPLING_RATE, open_param->stream_out_param.afe.i2s_master_sampling_rate[0]);
        AudioAfeConfiguration(AUDIO_SINK_I2S_MASTER_SAMPLING_RATE1, open_param->stream_out_param.afe.i2s_master_sampling_rate[1]);
        AudioAfeConfiguration(AUDIO_SINK_I2S_MASTER_SAMPLING_RATE2, open_param->stream_out_param.afe.i2s_master_sampling_rate[2]);
        AudioAfeConfiguration(AUDIO_SINK_I2S_MASTER_SAMPLING_RATE3, open_param->stream_out_param.afe.i2s_master_sampling_rate[3]);
        AudioAfeConfiguration(AUDIO_SINK_I2S_MASTER_FORMAT, open_param->stream_out_param.afe.i2s_master_format[0]);
        AudioAfeConfiguration(AUDIO_SINK_I2S_MASTER_FORMAT1, open_param->stream_out_param.afe.i2s_master_format[1]);
        AudioAfeConfiguration(AUDIO_SINK_I2S_MASTER_FORMAT2, open_param->stream_out_param.afe.i2s_master_format[2]);
        AudioAfeConfiguration(AUDIO_SINK_I2S_MASTER_FORMAT3, open_param->stream_out_param.afe.i2s_master_format[3]);
        AudioAfeConfiguration(AUDIO_SINK_I2S_MASTER_WORD_LENGTH, open_param->stream_out_param.afe.i2s_master_word_length[0]);
        AudioAfeConfiguration(AUDIO_SINK_I2S_MASTER_WORD_LENGTH1, open_param->stream_out_param.afe.i2s_master_word_length[1]);
        AudioAfeConfiguration(AUDIO_SINK_I2S_MASTER_WORD_LENGTH2, open_param->stream_out_param.afe.i2s_master_word_length[2]);
        AudioAfeConfiguration(AUDIO_SINK_I2S_MASTER_WORD_LENGTH3, open_param->stream_out_param.afe.i2s_master_word_length[3]);
        AudioAfeConfiguration(AUDIO_SINK_I2S_MASTER_LOW_JITTER, open_param->stream_out_param.afe.is_low_jitter[0]);
        AudioAfeConfiguration(AUDIO_SINK_I2S_MASTER_LOW_JITTER1, open_param->stream_out_param.afe.is_low_jitter[1]);
        AudioAfeConfiguration(AUDIO_SINK_I2S_MASTER_LOW_JITTER2, open_param->stream_out_param.afe.is_low_jitter[2]);
        AudioAfeConfiguration(AUDIO_SINK_I2S_MASTER_LOW_JITTER3, open_param->stream_out_param.afe.is_low_jitter[3]);
        DSP_MW_LOG_I("i2s master configuration=%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", 16,
                     open_param->stream_out_param.afe.i2s_master_sampling_rate[0],
                     open_param->stream_out_param.afe.i2s_master_sampling_rate[1],
                     open_param->stream_out_param.afe.i2s_master_sampling_rate[2],
                     open_param->stream_out_param.afe.i2s_master_sampling_rate[3],
                     open_param->stream_out_param.afe.i2s_master_format[0],
                     open_param->stream_out_param.afe.i2s_master_format[1],
                     open_param->stream_out_param.afe.i2s_master_format[2],
                     open_param->stream_out_param.afe.i2s_master_format[3],
                     open_param->stream_out_param.afe.i2s_master_word_length[0],
                     open_param->stream_out_param.afe.i2s_master_word_length[1],
                     open_param->stream_out_param.afe.i2s_master_word_length[2],
                     open_param->stream_out_param.afe.i2s_master_word_length[3],
                     open_param->stream_out_param.afe.is_low_jitter[0],
                     open_param->stream_out_param.afe.is_low_jitter[1],
                     open_param->stream_out_param.afe.is_low_jitter[2],
                     open_param->stream_out_param.afe.is_low_jitter[3]);
    } else if (open_param->stream_out_param.afe.audio_device == HAL_AUDIO_CONTROL_DEVICE_I2S_SLAVE) {
        AudioAfeConfiguration(AUDIO_SINK_I2S_FORMAT, open_param->stream_out_param.afe.i2s_format);
        AudioAfeConfiguration(AUDIO_SINK_I2S_SLAVE_TDM, open_param->stream_out_param.afe.i2S_Slave_TDM);
        AudioAfeConfiguration(AUDIO_SINK_I2S_WORD_LENGTH, open_param->stream_out_param.afe.i2s_word_length);
    }

    AudioAfeConfiguration(AUDIO_SINK_DAC_PERFORMANCE, open_param->stream_out_param.afe.performance);
#endif
#ifdef AIR_LINE_IN_MIX_ENABLE
if(open_param->stream_out_param.afe.audio_device = HAL_AUDIO_CONTROL_DEVICE_I2S_MASTER) {
    AudioAfeConfiguration(AUDIO_SINK_I2S_MASTER_FORMAT, open_param->stream_out_param.afe.i2s_master_format[0]);;
    AudioAfeConfiguration(AUDIO_SINK_I2S_MASTER_WORD_LENGTH, open_param->stream_out_param.afe.i2s_master_word_length[0]);
    AudioAfeConfiguration(AUDIO_SINK_I2S_MASTER_LOW_JITTER, open_param->stream_out_param.afe.is_low_jitter[0]);
}
#endif
#if AUTO_ERROR_SUPPRESSION
    AudioAfeConfiguration(AUDIO_SINK_MISC_PARMS_I2S_CLK, open_param->stream_out_param.afe.misc_parms.I2sClkSourceType);
    AudioAfeConfiguration(AUDIO_SINK_MISC_PARMS_MICBIAS, open_param->stream_out_param.afe.misc_parms.MicbiasSourceType);
#endif
    AudioAfeConfiguration(AUDIO_SINK_ECHO_REFERENCE, false);
#ifdef AIR_HFP_DNN_PATH_ENABLE
    AudioAfeConfiguration(AUDIO_SINK_DNN_PATH_ENABLE, open_param->stream_out_param.afe.enable_ul_dnn);
#endif
#ifdef AIR_WIRED_AUDIO_ENABLE
    AudioAfeConfiguration(AUDIO_SINK_UPDOWN_SAMPLER_ENABLE, open_param->stream_out_param.afe.with_upwdown_sampler);
    AudioAfeConfiguration(AUDIO_SINK_PATH_INPUT_RATE, open_param->stream_out_param.afe.audio_path_input_rate);
    AudioAfeConfiguration(AUDIO_SINK_PATH_OUTPUT_RATE, open_param->stream_out_param.afe.audio_path_output_rate);
#endif
    DSP_MW_LOG_I("afe out format:%d, src out rate:%d, IRQ period:%d src in rate:%d\r\n", 4, open_param->stream_out_param.afe.format,
                 gAudioCtrl.Afe.AfeDLSetting.rate,
                 open_param->stream_out_param.afe.irq_period,
                 gAudioCtrl.Afe.AfeDLSetting.src_rate);


#ifdef AIR_AUDIO_I2S_SLAVE_TDM_ENABLE
    if (open_param->stream_out_param.afe.memory == HAL_AUDIO_MEM1 || open_param->stream_out_param.afe.memory == HAL_AUDIO_MEM6) {
#else
    if (open_param->stream_out_param.afe.memory == HAL_AUDIO_MEM1 || open_param->stream_out_param.afe.memory == HAL_AUDIO_MEM6 || open_param->stream_out_param.afe.memory == HAL_AUDIO_MEM7) {
#endif
        AudioAfeConfiguration(AUDIO_SINK_SW_CHANNELS, open_param->stream_out_param.afe.sw_channels);
        Audio_setting->Audio_sink.Buffer_Frame_Num = open_param->stream_out_param.afe.frame_number;
        Audio_setting->Audio_sink.Frame_Size       = open_param->stream_out_param.afe.frame_size;
        sink = StreamAudioAfeSink(afe_get_audio_hardware_by_au_afe_open_param(&(open_param->stream_out_param.afe)),
                                  afe_get_audio_instance_by_au_afe_open_param(&(open_param->stream_out_param.afe)),
                                  afe_get_audio_channel_by_au_afe_open_param(&(open_param->stream_out_param.afe)));

    } else if (open_param->stream_out_param.afe.memory == HAL_AUDIO_MEM2) {
        Audio_setting->Audio_VP.Buffer_Frame_Num = open_param->stream_out_param.afe.frame_number;
        Audio_setting->Audio_VP.Frame_Size = open_param->stream_out_param.afe.frame_size;
        // VP/RT memory path DL2_data
#ifdef MTK_PROMPT_SOUND_ENABLE
        sink = StreamAudioAfe2Sink(afe_get_audio_hardware_by_au_afe_open_param(&(open_param->stream_out_param.afe)),
                                   afe_get_audio_instance_by_au_afe_open_param(&(open_param->stream_out_param.afe)),
                                   afe_get_audio_channel_by_au_afe_open_param(&(open_param->stream_out_param.afe)));
#endif
#ifdef AIR_AUDIO_I2S_SLAVE_TDM_ENABLE
    } else if (open_param->stream_out_param.afe.memory == HAL_AUDIO_MEM7) {
        //Tdm-Sink
        AudioAfeConfiguration(AUDIO_SINK_SW_CHANNELS, open_param->stream_out_param.afe.sw_channels);
        Audio_setting->Audio_sink.Buffer_Frame_Num = open_param->stream_out_param.afe.frame_number;
        Audio_setting->Audio_sink.Frame_Size       = open_param->stream_out_param.afe.frame_size;
        sink = StreamAudioAfeTdmSink(afe_get_audio_hardware_by_au_afe_open_param(&(open_param->stream_out_param.afe)),
                                     afe_get_audio_instance_by_au_afe_open_param(&(open_param->stream_out_param.afe)),
                                     afe_get_audio_channel_by_au_afe_open_param(&(open_param->stream_out_param.afe)));
#endif
    } else if (open_param->stream_out_param.afe.memory == HAL_AUDIO_MEM3) {
#if defined (AIR_WIRED_AUDIO_ENABLE) || defined (AIR_ADVANCED_PASSTHROUGH_ENABLE) || defined (AIR_PROMPT_SOUND_DUMMY_SOURCE_ENABLE) || defined(AIR_HFP_DNN_PATH_ENABLE) || defined(AIR_DUAL_CHIP_MIXING_MODE_ROLE_MASTER_ENABLE) || defined(AIR_DUAL_CHIP_MIXING_MODE_ROLE_SLAVE_ENABLE)
        AudioAfeConfiguration(AUDIO_SINK_SW_CHANNELS, open_param->stream_out_param.afe.sw_channels);
        Audio_setting->Audio_sink.Buffer_Frame_Num = open_param->stream_out_param.afe.frame_number;
        Audio_setting->Audio_sink.Frame_Size       = open_param->stream_out_param.afe.frame_size;
        // VP/RT memory path DL2_data
        DSP_MW_LOG_I("[TEST] Create DL3 afe sink", 0);
        sink = StreamAudioAfe3Sink(afe_get_audio_hardware_by_au_afe_open_param(&(open_param->stream_out_param.afe)),
                                   afe_get_audio_instance_by_au_afe_open_param(&(open_param->stream_out_param.afe)),
                                   afe_get_audio_channel_by_au_afe_open_param(&(open_param->stream_out_param.afe)));
#endif
    }
#ifdef AIR_ADVANCED_PASSTHROUGH_ENABLE
    else if (open_param->stream_out_param.afe.memory == HAL_AUDIO_MEM4) {
        AudioAfeConfiguration(AUDIO_SINK_SW_CHANNELS, open_param->stream_out_param.afe.sw_channels);
        Audio_setting->Audio_sink.Buffer_Frame_Num = open_param->stream_out_param.afe.frame_number;
        Audio_setting->Audio_sink.Frame_Size       = open_param->stream_out_param.afe.frame_size;
        sink = StreamAudioAfe12Sink(afe_get_audio_hardware_by_au_afe_open_param(&(open_param->stream_out_param.afe)),
                                    afe_get_audio_instance_by_au_afe_open_param(&(open_param->stream_out_param.afe)),
                                    afe_get_audio_channel_by_au_afe_open_param(&(open_param->stream_out_param.afe)));
    }
#endif /* AIR_ADVANCED_PASSTHROUGH_ENABLE */

    if (sink != NULL) {

    } else {
        DSP_MW_LOG_E("DSP sink create fail\r\n", 0);
    }
    return sink;

}
#endif /* AIR_AUDIO_HARDWARE_ENABLE */

SINK dsp_open_stream_out_record(mcu2dsp_open_param_p open_param)
{
    SINK sink;
    DSP_MW_LOG_I("Stream out record\r\n", 0);

    sink = StreamCm4RecordSink(&(open_param->stream_out_param.record));
    if (sink != NULL) {

    } else {
        DSP_MW_LOG_E("DSP sink create fail\r\n", 0);
    }
    return sink;
}

SINK dsp_open_stream_out_virtual(mcu2dsp_open_param_p open_param)
{
    SINK sink;
    DSP_MW_LOG_I("Stream out virtual\r\n", 0);
    UNUSED(open_param);
    sink = StreamVirtualSink(NULL, NULL);
    if (sink != NULL) {

    } else {
        DSP_MW_LOG_E("DSP sink create fail\r\n", 0);
    }
    return sink;
}

#ifdef AIR_AUDIO_TRANSMITTER_ENABLE
SINK dsp_open_stream_out_audio_transmitter(mcu2dsp_open_param_p open_param)
{
    SINK sink;

    sink = StreamAudioTransmitterSink(&(open_param->stream_out_param.data_ul));
    if (sink) {
        DSP_MW_LOG_I("[audio transmitter]: sink create successfully\r\n", 0);
    } else {
        DSP_MW_LOG_E("[audio transmitter]: sink create fail\r\n", 0);
    }

    return sink;
}
#endif /* AIR_AUDIO_TRANSMITTER_ENABLE */

#ifdef AIR_AUDIO_BT_COMMON_ENABLE
SINK dsp_open_stream_out_bt_common(mcu2dsp_open_param_p open_param)
{
    SINK sink;

    DSP_MW_LOG_I("Stream out bt common\r\n", 0);

    sink = StreamBTCommonSink(&(open_param->stream_out_param.bt_common));

    if (sink) {
        DSP_MW_LOG_I("DSP sink create successfully\r\n", 0);
    } else {
        DSP_MW_LOG_E("DSP sink create fail\r\n", 0);
    }

    return sink;
}
#endif /* AIR_AUDIO_BT_COMMON_ENABLE */

SINK dsp_open_stream_out(mcu2dsp_open_param_p open_param)
{
    SINK sink = NULL;
    if (open_param != NULL) {
#ifdef AIR_AUDIO_HARDWARE_ENABLE
        AudioAfeConfiguration(AUDIO_SINK_SCENARIO_TYPE, open_param->audio_scenario_type);
#endif
        switch (open_param->param.stream_out) {
#ifdef AIR_AUDIO_HARDWARE_ENABLE
            case STREAM_OUT_AFE:
                sink = dsp_open_stream_out_afe(open_param);
                break;
#endif /* AIR_AUDIO_HARDWARE_ENABLE */
#ifdef AIR_BT_HFP_ENABLE
            case STREAM_OUT_HFP:
                sink = dsp_open_stream_out_hfp(open_param);
                break;
#endif
#ifdef AIR_BT_CODEC_BLE_ENABLED
            case STREAM_OUT_BLE:
                sink = dsp_open_stream_out_ble(open_param);
                break;
#endif
            case STREAM_OUT_RECORD:
                sink = dsp_open_stream_out_record(open_param);
                break;
#ifdef AIR_AUDIO_TRANSMITTER_ENABLE
            case STREAM_OUT_AUDIO_TRANSMITTER:
                sink = dsp_open_stream_out_audio_transmitter(open_param);
                break;
#endif /* AIR_AUDIO_TRANSMITTER_ENABLE */
            case STREAM_OUT_VIRTUAL:
                sink = dsp_open_stream_out_virtual(open_param);
                break;
#ifdef AIR_AUDIO_BT_COMMON_ENABLE
            case STREAM_OUT_BT_COMMON:
                sink = dsp_open_stream_out_bt_common(open_param);
                break;
#endif /* AIR_AUDIO_BT_COMMON_ENABLE */
            default:
                break;
        }
    }

    if (sink) {
        sink->scenario_type = open_param->audio_scenario_type;
    }
    return sink;
}

#ifdef AIR_AUDIO_HARDWARE_ENABLE
void dsp_start_stream_in_afe(mcu2dsp_start_param_p start_param, SOURCE source)
{
    source->param.audio.AfeBlkControl.u4awsflag = start_param->stream_in_param.afe.mce_flag;
    DSP_MW_LOG_I("Stream in afe start MCE:%d\r\n", 1, source->param.audio.AfeBlkControl.u4awsflag);
}
#endif /* AIR_AUDIO_HARDWARE_ENABLE */

#ifdef PLAY_EN_CHECK_TIMER
#ifdef AIR_CPU_IN_SECURITY_MODE
#define CONN_BT_TIMCON_BASE 0xA0000000
#else
#define CONN_BT_TIMCON_BASE 0xB0000000
#endif

static TimerHandle_t playen_check_timer = NULL;
static void playen_check_timer_callback(TimerHandle_t xTimer)
{
    UNUSED(xTimer);
    DSP_MW_LOG_I("PLANEN_CHECK_TIMER", 0);
    if ((n9_a2dp_if.transform != NULL) && (n9_a2dp_if.sink->param.audio.irq_exist == FALSE)) {
        DSP_MW_LOG_E("AFE wait play en trigger re-sync", 0);
#ifdef MTK_BT_A2DP_ENABLE
        Au_DL_send_reinit_request(MSG2_DSP2CN4_REINIT_AFE_ABNORMAL);
#endif /* MTK_BT_A2DP_ENABLE */
    } else {
        xTimerStop(playen_check_timer, 0);
    }
}
#endif
U32 initial_asi = 0;
#ifdef MTK_BT_A2DP_ENABLE
#ifdef TRIGGER_A2DP_PROCSESS_TIMER
static TimerHandle_t trig_a2dp_proc_timer = NULL;

static void afe_prefill_for_src_out(U16 fill_sample_num, SINK sink)
{
    DSP_MW_LOG_E("prefill samples:%d,remain space:%d", 2, fill_sample_num, SinkSlack(sink));
    if ((fill_sample_num * sink->param.audio.format_bytes) > SinkSlack(sink)) {
        return;
    }
    U32 buffer_per_channel_shift = ((sink->param.audio.channel_num >= 2) && (sink->buftype == BUFFER_TYPE_INTERLEAVED_BUFFER))
                                   ? 1
                                   : 0;
    U32 mask;
    hal_nvic_save_and_set_interrupt_mask(&mask);
    SinkFlush(sink, fill_sample_num * sink->param.audio.format_bytes);
    sink->param.audio.sram_empty_fill_size = (fill_sample_num * sink->param.audio.format_bytes) << buffer_per_channel_shift;
    hal_nvic_restore_interrupt_mask(mask);
}

static void trigger_a2dp_proc_timer_callback(TimerHandle_t xTimer)
{
    UNUSED(xTimer);

    if ((n9_a2dp_if.source != NULL) && (n9_a2dp_if.sink != NULL) && (n9_a2dp_if.transform != NULL) && (n9_a2dp_if.sink->param.audio.irq_exist == FALSE)) {
        DSP_MW_LOG_I("trigger_a2dp_proc_timer_callback start", 0);
        n9_a2dp_if.source->transform->Handler(n9_a2dp_if.source, n9_a2dp_if.sink);
        if (n9_a2dp_if.source->transform->sink->taskid == DPR_TASK_ID) {
            vTaskResume(pDPR_TaskHandler);
        } else if (n9_a2dp_if.source->transform->sink->taskid == DHP_TASK_ID) {
            vTaskResume(pDHP_TaskHandler);
        } else {
            vTaskResume(pDAV_TaskHandler);
        }
    } else {
        DSP_MW_LOG_I("trigger_a2dp_proc_timer_callback stop", 0);
        xTimerStop(trig_a2dp_proc_timer, 0);
    }
}
#endif

void dsp_start_stream_in_a2dp(mcu2dsp_start_param_p start_param, SOURCE source)
{
    source->param.n9_a2dp.cp_exist = FALSE;
    source->param.n9_a2dp.predict_asi = start_param->stream_in_param.a2dp.start_asi;
    source->param.n9_a2dp.latency_monitor = start_param->stream_in_param.a2dp.latency_monitor_enable;
    source->param.n9_a2dp.DspReportStartId = 0xFFFF;
    source->param.n9_a2dp.alc_monitor = !start_param->stream_in_param.a2dp.alc_monitor;// If Host already enabled ALC, DSP not to trigger ALC mode

    //source->param.n9_a2dp.latency_monitor = TRUE;
    U32 bt_clk;
    U16 intra_clk;
#ifdef AIR_A2DP_REINIT_V2_ENABLE
    if(n9_a2dp_if.source != NULL){
        if(start_param->stream_in_param.a2dp.time_stamp_ratio == IS_SPECIAL_DEVICE){
            A2DP_SpeDev_Setting(&(n9_a2dp_if.source->param.n9_a2dp),true); 
        }else{
            A2DP_SpeDev_Setting(&(n9_a2dp_if.source->param.n9_a2dp),false);
        }
    }
#endif

#ifdef MTK_GAMING_MODE_HEADSET
    if (source->param.n9_a2dp.codec_info.codec_cap.type == BT_A2DP_CODEC_AIRO_CELT) {
        /* ULL should wait for play en parameter from controller */
        if (audio_headset_ull_ctrl.is_play_en_ready) {
            audio_headset_ull_ctrl.play_en.bt_clock += ((source->param.n9_a2dp.sink_latency - ULL_BASIC_LATENCY) * 8 / 2500); /* 8/2500 = 312.5 for frame based  */

            a2dp_ull_get_proper_play_en(audio_headset_ull_ctrl.play_en.bt_clock,
                                        audio_headset_ull_ctrl.play_en.sequence_number,
                                        &audio_headset_ull_ctrl.play_en.bt_clock,
                                        (U8 *)&audio_headset_ull_ctrl.play_en.sequence_number);

            a2dp_ull_set_init_seq_no(audio_headset_ull_ctrl.play_en.sequence_number);

            a2dp_ull_drop_expired_pkt(source);

            /* AVM buffer needs to update SampleRate as 48k, this is temp */
            source->streamBuffer.AVMBufferInfo.SampleRate = 48000;
            if ((n9_a2dp_if.sink != NULL) && (n9_a2dp_if.sink->type == SINK_TYPE_AUDIO)) {
                n9_a2dp_if.sink->streamBuffer.BufferInfo.WriteOffset = 720;// temp modify prefill samples for ULL
            }
            MCE_TransBT2NativeClk(audio_headset_ull_ctrl.play_en.bt_clock, 0, &bt_clk, &intra_clk, ULL_CLK_Offset);
#ifdef AIR_AUDIO_HARDWARE_ENABLE
            hal_audio_afe_set_play_en(bt_clk, intra_clk);
#endif /* AIR_AUDIO_HARDWARE_ENABLE */
            DSP_MW_LOG_I("A2DP DL - A2DP Set Play En Clk:0x%x Intra:0x%x Modify AFE prefill offest to 720", 2, (uint32_t)bt_clk, (uint32_t)intra_clk);
        }
        audio_headset_ull_ctrl.is_a2dp_started = TRUE;
    } else
#endif
    {
        MCE_GetBtClk((BTCLK *)&bt_clk, (BTPHASE *)&intra_clk, BT_CLK_Offset);
        DSP_MW_LOG_I("Get host set asi:0x%x b:0x%x i:0x%x", 3, start_param->stream_in_param.a2dp.start_asi, start_param->stream_in_param.a2dp.start_bt_clk, start_param->stream_in_param.a2dp.start_bt_intra_clk);
        DSP_MW_LOG_I("Get local clk, n:0x%x b:0x%x i:0x%x", 3, rBb->rClkCtl.rNativeClock, bt_clk, intra_clk);
        DSP_MW_LOG_I("DSP latency_monitor :%d ALC mode :%d", 2, start_param->stream_in_param.a2dp.latency_monitor_enable, start_param->stream_in_param.a2dp.alc_monitor);
        start_param->stream_in_param.a2dp.start_bt_intra_clk = (start_param->stream_in_param.a2dp.start_bt_intra_clk & 0xFFFF) << 1;

        if (abs32((S32)(start_param->stream_in_param.a2dp.start_bt_clk - bt_clk)) >  0x2000) { // Maximum tolerence time difference = 0x2000*312.5(us) = 2.56(s)
            DSP_MW_LOG_I("Host notify play time abnormal, bt_clk: 0x%x play_clk: 0x%x", 2, bt_clk, start_param->stream_in_param.a2dp.start_bt_clk);
            start_param->stream_in_param.a2dp.start_bt_clk = bt_clk + 0x100;
            //AUDIO_ASSERT(0);
        }

        if ((S32)(start_param->stream_in_param.a2dp.start_bt_clk - bt_clk) > 0x10) { // check play time in time
            DSP_MW_LOG_I("Set legal play en", 0);
        } else {
            U32 temp_start_bt_clk = start_param->stream_in_param.a2dp.start_bt_clk;
            while ((S32)(temp_start_bt_clk - bt_clk) < 0x80) {
                source->param.n9_a2dp.predict_asi += 1024;
                temp_start_bt_clk +=  3276800 / (source->streamBuffer.AVMBufferInfo.SampleRate); // roughly calc
            }
            start_param->stream_in_param.a2dp.start_bt_clk += ((((source->param.n9_a2dp.predict_asi - start_param->stream_in_param.a2dp.start_asi) << 5) * 100 / (source->streamBuffer.AVMBufferInfo.SampleRate)) & 0xFFFFFFC);
            start_param->stream_in_param.a2dp.start_bt_intra_clk += ((((source->param.n9_a2dp.predict_asi - start_param->stream_in_param.a2dp.start_asi)) * 20000) / (source->streamBuffer.AVMBufferInfo.SampleRate / 100)) % 2500;
            DSP_MW_LOG_I("Host notify play time too early, modify play en b:0x%x i:0x%x", 2, start_param->stream_in_param.a2dp.start_bt_clk, start_param->stream_in_param.a2dp.start_bt_intra_clk);
        }
        initial_asi = source->param.n9_a2dp.predict_asi;
        MCE_TransBT2NativeClk(start_param->stream_in_param.a2dp.start_bt_clk, start_param->stream_in_param.a2dp.start_bt_intra_clk, &bt_clk, &intra_clk, BT_CLK_Offset);
        DSP_MW_LOG_I("Play en b:0x%x i:0x%x n:0x%x asi: 0x%x", 4, bt_clk, intra_clk, rBb->rClkCtl.rNativeClock, source->param.n9_a2dp.predict_asi);
#ifdef AIR_AUDIO_HARDWARE_ENABLE
        hal_audio_afe_set_play_en(bt_clk, intra_clk);
#endif /* AIR_AUDIO_HARDWARE_ENABLE */
#ifdef TRIGGER_A2DP_PROCSESS_TIMER
        U32 bt_clk_cur;
        U16 intra_clk_cur;
        MCE_GetBtClk((BTCLK *)&bt_clk_cur, (BTPHASE *)&intra_clk_cur, BT_CLK_Offset);
        U32 bt_clk_duration = start_param->stream_in_param.a2dp.start_bt_clk - bt_clk_cur;
        U32 duration_ms = ((bt_clk_duration >> 1) * 625) / 1000;
        DSP_MW_LOG_I("trigger_a2dp_proc_timer, bclk_play_en:%d, bclk_cur:%d, timer_duration:%d ms", 3, start_param->stream_in_param.a2dp.start_bt_clk, bt_clk_cur, duration_ms);
        if (duration_ms > 10) { /* the tick unit of DSP is 10ms*/
            if (trig_a2dp_proc_timer == NULL) {
                trig_a2dp_proc_timer = xTimerCreate("TRIGGER_A2DP_PROCSESS_TIMER", pdMS_TO_TICKS(duration_ms), pdFALSE, 0, trigger_a2dp_proc_timer_callback);
            }
            if (!trig_a2dp_proc_timer) {
                DSP_MW_LOG_I("trigger_a2dp_proc_timer create timer FAIL", 0);
            } else {
                DSP_MW_LOG_I("trigger_a2dp_proc_timer create timer PASS", 0);
                xTimerStart(trig_a2dp_proc_timer, 0);
            }
        }
        //else if(SourceSize(source) == 0)
        {
            DSP_MW_LOG_E("Add prefill to prevent SRC out underrun on a2dp start", 0);
            afe_prefill_for_src_out(((2 * (U32)n9_a2dp_if.sink->param.audio.period * (n9_a2dp_if.sink->param.audio.rate / 1000) + 256) & 0xFF00), n9_a2dp_if.sink);
        }
#endif /*TRIGGER_A2DP_PROCSESS_TIMER*/
#ifdef PLAY_EN_CHECK_TIMER
        if (playen_check_timer == NULL) {
            playen_check_timer = xTimerCreate("PLANEN_CHECK_TIMER", pdMS_TO_TICKS(700), pdTRUE, 0, playen_check_timer_callback);
        }
        if (!playen_check_timer) {
            DSP_MW_LOG_I("playen_check_timer create timer FAIL", 0);
        } else {
            DSP_MW_LOG_I("playen_check_timer create timer PASS", 0);
            xTimerChangePeriod(playen_check_timer, pdMS_TO_TICKS(700), 0);
        }
#endif
    }
    MCE_Initial_Aud_Cnt_from_Controller();
}
#endif /* MTK_BT_A2DP_ENABLE */

#ifdef AIR_AUDIO_TRANSMITTER_ENABLE
void dsp_start_stream_in_audio_transmitter(mcu2dsp_start_param_p start_param, SOURCE source)
{
    UNUSED(start_param);

    DSP_MW_LOG_I("Stream in audio transmitter start:%d, %d\r\n", 2, source->param.data_dl.scenario_type, source->param.data_dl.scenario_sub_id);

    switch (source->param.data_dl.scenario_type) {
#if defined(MTK_GAMING_MODE_HEADSET) || defined(AIR_GAMING_MODE_DONGLE_ENABLE)
        case AUDIO_TRANSMITTER_GAMING_MODE:
            if ((source->param.data_dl.scenario_sub_id == AUDIO_TRANSMITTER_GAMING_MODE_MUSIC_DONGLE_USB_IN_0) ||
                (source->param.data_dl.scenario_sub_id == AUDIO_TRANSMITTER_GAMING_MODE_MUSIC_DONGLE_USB_IN_1)) {
                /* start the first timer */
                hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &source->param.data_dl.scenario_param.usb_in_broadcast_param.usb_in_param.timer.timestamp);
                // hal_gpt_sw_start_timer_us(source->param.data_dl.scenario_param.usb_in_broadcast_param.usb_in_param.timer.handle,
                //                           source->param.data_dl.scenario_param.usb_in_broadcast_param.usb_in_param.timer.period,
                //                           source->param.data_dl.scenario_param.usb_in_broadcast_param.usb_in_param.timer.callback,
                //                           (void *)source);

                /* set timer start timestamp as the first data timestamp */
                source->param.data_dl.scenario_param.usb_in_broadcast_param.usb_in_param.data_timestamp = source->param.data_dl.scenario_param.usb_in_broadcast_param.usb_in_param.timer.timestamp;
            }

            break;
#endif /* MTK_GAMING_MODE_HEADSET || AIR_GAMING_MODE_DONGLE_ENABLE */

#if defined(AIR_BLE_AUDIO_DONGLE_ENABLE)
        case AUDIO_TRANSMITTER_BLE_AUDIO_DONGLE:
            if ((source->param.data_dl.scenario_sub_id == AUDIO_TRANSMITTER_BLE_AUDIO_DONGLE_MUSIC_USB_IN_0) ||
                (source->param.data_dl.scenario_sub_id == AUDIO_TRANSMITTER_BLE_AUDIO_DONGLE_MUSIC_USB_IN_1)) {
                /* get the first timestamp */
                hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &source->param.data_dl.scenario_param.usb_in_broadcast_param.usb_in_param.data_timestamp);
            }

            break;
#endif /* AIR_BLE_AUDIO_DONGLE_ENABLE */

        default:
            break;
    }
}
#endif /* AIR_AUDIO_TRANSMITTER_ENABLE */

#ifdef AIR_AUDIO_BT_COMMON_ENABLE
void dsp_start_stream_in_bt_common(mcu2dsp_start_param_p start_param, SOURCE source)
{
    UNUSED(start_param);

    DSP_MW_LOG_I("Stream in bt common start:%d, %d\r\n", 2, source->param.bt_common.scenario_type, source->param.bt_common.scenario_sub_id);

    switch (source->param.bt_common.scenario_type) {
#if defined(MTK_GAMING_MODE_HEADSET) || defined(AIR_GAMING_MODE_DONGLE_ENABLE)
        case AUDIO_TRANSMITTER_GAMING_MODE:
            if (source->param.bt_common.scenario_sub_id == AUDIO_TRANSMITTER_GAMING_MODE_VOICE_DONGLE_USB_OUT) {
                /* start the first timer */
                hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &source->param.bt_common.scenario_param.usb_out_broadcast_param.bt_in_param.timer.timestamp);
                // hal_gpt_sw_start_timer_us(source->param.bt_common.scenario_param.usb_out_broadcast_param.bt_in_param.timer.handle,
                //                         source->param.bt_common.scenario_param.usb_out_broadcast_param.bt_in_param.timer.period,
                //                         source->param.bt_common.scenario_param.usb_out_broadcast_param.bt_in_param.timer.callback,
                //                         (void *)source);

                /* set timer start timestamp as the first data timestamp */
                source->param.bt_common.scenario_param.usb_out_broadcast_param.bt_in_param.data_timestamp = source->param.bt_common.scenario_param.usb_out_broadcast_param.bt_in_param.timer.timestamp;
            }
#ifdef AIR_GAMING_MODE_DONGLE_LINE_OUT_ENABLE
            else if (source->scenario_type == AUDIO_SCENARIO_TYPE_GAMING_MODE_VOICE_DONGLE_LINE_OUT) {
                /* start the first timer */
                hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &source->param.bt_common.scenario_param.usb_out_broadcast_param.bt_in_param.timer.timestamp);
                /* set timer start timestamp as the first data timestamp */
                source->param.bt_common.scenario_param.usb_out_broadcast_param.bt_in_param.data_timestamp = source->param.bt_common.scenario_param.usb_out_broadcast_param.bt_in_param.timer.timestamp;
            }
#endif /* AIR_GAMING_MODE_DONGLE_LINE_OUT_ENABLE */
            break;
#endif /* MTK_GAMING_MODE_HEADSET || AIR_GAMING_MODE_DONGLE_ENABLE */

        default:
            break;
    }

}
#endif /* AIR_AUDIO_BT_COMMON_ENABLE */

void dsp_start_stream_in(mcu2dsp_start_param_p start_param, SOURCE source)
{
    if (start_param != NULL) {
        switch (start_param->param.stream_in) {
#ifdef AIR_AUDIO_HARDWARE_ENABLE
            case STREAM_IN_AFE:
                dsp_start_stream_in_afe(start_param, source);
                break;
#endif /* AIR_AUDIO_HARDWARE_ENABLE */
#ifdef AIR_BT_HFP_ENABLE
            case STREAM_IN_HFP:
                break;
#endif
#ifdef AIR_BT_CODEC_BLE_ENABLED
            case STREAM_IN_BLE:
                break;
#endif
#ifdef MTK_BT_A2DP_ENABLE
            case STREAM_IN_A2DP:
                dsp_start_stream_in_a2dp(start_param, source);
                break;
#endif /* MTK_BT_A2DP_ENABLE */
            case STREAM_IN_PLAYBACK:
                break;
            case STREAM_IN_VP:
                break;
#ifdef AIR_AUDIO_TRANSMITTER_ENABLE
            case STREAM_IN_AUDIO_TRANSMITTER:
                dsp_start_stream_in_audio_transmitter(start_param, source);
                break;
#endif /* AIR_AUDIO_TRANSMITTER_ENABLE */
#ifdef AIR_AUDIO_BT_COMMON_ENABLE
            case STREAM_IN_BT_COMMON:
                dsp_start_stream_in_bt_common(start_param, source);
                break;
#endif /* AIR_AUDIO_BT_COMMON_ENABLE */
#if defined AIR_DUAL_CHIP_MIXING_MODE_ROLE_MASTER_ENABLE || defined AIR_DUAL_CHIP_MIXING_MODE_ROLE_SLAVE_ENABLE
            case STREAM_IN_UART:
                dsp_start_stream_in_uart(start_param, source);
                break;
#endif
            default:
                break;
        }
    }
}

#ifdef AIR_AUDIO_HARDWARE_ENABLE
void dsp_start_stream_out_afe(mcu2dsp_start_param_p start_param, SINK sink)
{
    sink->param.audio.AfeBlkControl.u4awsflag = start_param->stream_out_param.afe.mce_flag;
    //sink->param.audio.AfeBlkControl.u4awsflag = TRUE;// synchornize headset project & MCE
    sink->param.audio.aws_sync_request        = start_param->stream_out_param.afe.aws_sync_request;
    sink->param.audio.aws_sync_time           = start_param->stream_out_param.afe.aws_sync_time;
    //DSP_MW_LOG_I("Stream out afe start MCE:%d\r\n", 1, sink->param.audio.AfeBlkControl.u4awsflag);
}
#endif /* AIR_AUDIO_HARDWARE_ENABLE */

#ifdef AIR_AUDIO_TRANSMITTER_ENABLE
void dsp_start_stream_out_audio_transmitter(mcu2dsp_start_param_p start_param, SINK sink)
{
    UNUSED(start_param);

    DSP_MW_LOG_I("Stream out audio transmitter start:%d, %d\r\n", 2, sink->param.data_ul.scenario_type, sink->param.data_ul.scenario_sub_id);
}
#endif /* AIR_AUDIO_TRANSMITTER_ENABLE */

#ifdef AIR_AUDIO_BT_COMMON_ENABLE
void dsp_start_stream_out_bt_common(mcu2dsp_start_param_p start_param, SINK sink)
{
    UNUSED(start_param);

    DSP_MW_LOG_I("Stream out bt common start:%d, %d\r\n", 2, sink->param.bt_common.scenario_type, sink->param.bt_common.scenario_sub_id);
}
#endif /* AIR_AUDIO_BT_COMMON_ENABLE */

void dsp_start_stream_out(mcu2dsp_start_param_p start_param, SINK sink)
{
    if (start_param != NULL) {
        switch (start_param->param.stream_out) {
#ifdef AIR_AUDIO_HARDWARE_ENABLE
            case STREAM_OUT_AFE:
                dsp_start_stream_out_afe(start_param, sink);
                break;
#endif /* AIR_AUDIO_HARDWARE_ENABLE */
#ifdef AIR_BT_HFP_ENABLE
            case STREAM_OUT_HFP:
                break;
#endif
#ifdef AIR_BT_CODEC_BLE_ENABLED
            case STREAM_OUT_BLE:
                break;
#endif
            case STREAM_OUT_RECORD:
                break;
#ifdef AIR_AUDIO_TRANSMITTER_ENABLE
            case STREAM_OUT_AUDIO_TRANSMITTER:
                dsp_start_stream_out_audio_transmitter(start_param, sink);
                break;
#endif /* AIR_AUDIO_TRANSMITTER_ENABLE */
#ifdef AIR_AUDIO_BT_COMMON_ENABLE
            case STREAM_OUT_BT_COMMON:
                dsp_start_stream_out_bt_common(start_param, sink);
                break;
#endif /* AIR_AUDIO_BT_COMMON_ENABLE */
            default:
                break;
        }
    }
}


void dsp_trigger_suspend(SOURCE source, SINK sink)
{
#ifdef AIR_AUDIO_HARDWARE_ENABLE
    afe_amp_keep_enable_state(TRUE);
    if (source != NULL) {
        audio_ops_trigger(source, AFE_PCM_TRIGGER_SUSPEND);
    }

    if (sink != NULL) {
        audio_ops_trigger(sink, AFE_PCM_TRIGGER_SUSPEND);
    }
#else
    UNUSED(source);
    UNUSED(sink);
#endif /* AIR_AUDIO_HARDWARE_ENABLE */
}

void dsp_trigger_resume(SOURCE source, SINK sink)
{
#ifdef AIR_AUDIO_HARDWARE_ENABLE
    if (source != NULL) {
        if (!audio_ops_trigger(source, AFE_PCM_TRIGGER_RESUME)) {
            source->param.audio.mute_flag = TRUE;
            source->param.audio.pop_noise_pkt_num = 0;
        }
    }

    if (sink != NULL) {
        audio_ops_trigger(sink, AFE_PCM_TRIGGER_RESUME);
    }
    afe_amp_keep_enable_state(FALSE);
#else
    UNUSED(source);
    UNUSED(sink);
#endif /* AIR_AUDIO_HARDWARE_ENABLE */
}
#ifdef MTK_BT_A2DP_ENABLE
/* A2DP CCNI callback function */
void CB_N9_A2DP_OPEN(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
#if 0
    //return;
#else
    UNUSED(ack);
    DSP_MW_LOG_I("A2DP OPEN\r\n", 0);

    /* remap to non-cacheable address */

    mcu2dsp_open_param_p open_param;
    open_param = (mcu2dsp_open_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);
    DSP_REMAP_SHARE_INFO(open_param->stream_in_param.a2dp.p_share_info, avm_share_buf_info_ptr);
    DSP_MW_LOG_I("A2DP share info %X\r\n", 1, open_param->stream_in_param.a2dp.p_share_info->StartAddr);
    DSP_REMAP_SHARE_INFO(open_param->stream_in_param.a2dp.p_asi_buf, uint32_t *);
    DSP_REMAP_SHARE_INFO(open_param->stream_in_param.a2dp.p_min_gap_buf, uint32_t *);
    DSP_REMAP_SHARE_INFO(open_param->stream_in_param.a2dp.p_pcdc_anchor_info_buf, uint32_t *);
    DSP_REMAP_SHARE_INFO(open_param->stream_in_param.a2dp.p_current_bit_rate, uint32_t *);
    DSP_REMAP_SHARE_INFO(open_param->stream_in_param.a2dp.bt_inf_address, uint32_t);
    DSP_REMAP_SHARE_INFO(open_param->stream_in_param.a2dp.p_afe_buf_report, uint32_t *);

#ifdef ENABLE_HWSRC_CLKSKEW
#ifdef AIR_HWSRC_TRACKING_ENABLE
    open_param->stream_out_param.afe.clkskew_mode = CLK_SKEW_V1;
#endif
#endif

#ifdef MTK_GAMING_MODE_HEADSET
    DSP_REMAP_SHARE_INFO(open_param->stream_in_param.a2dp.ull_clk_info_address, uint32_t *);
    MCE_Update_BtClkOffsetAddr(open_param->stream_in_param.a2dp.ull_clk_info_address, ULL_CLK_Offset);
    DSP_MW_LOG_I("[TEST]ull_clk_info_address:0x%x", 1, open_param->stream_in_param.a2dp.ull_clk_info_address);
#endif
    n9_a2dp_if.source   = dsp_open_stream_in(open_param);
    n9_a2dp_if.sink     = dsp_open_stream_out(open_param);
    n9_a2dp_if.transform = NULL;

    DSP_Callback_PreloaderConfig(n9_a2dp_if.pfeature_table, open_param->audio_scenario_type);
    DSP_MW_LOG_I("A2DP OPEN Finish\r\n", 0);
    //PIC
#endif
}

void CB_N9_A2DP_START(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
#if 0
    //aud_msg_ack(MSG_MCU2DSP_BT_AUDIO_DL_START | 0x8000, FALSE);
    //return;
#else
    U32 gpt_timer;
    UNUSED(ack);
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &gpt_timer);
    DSP_MW_LOG_I("A2DP START Time:%d\r\n", 1, gpt_timer);

    /* remap to non-cacheable address */
    mcu2dsp_start_param_p start_param;
    start_param = (mcu2dsp_start_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);


    dsp_start_stream_in(start_param, n9_a2dp_if.source);
    dsp_start_stream_out(start_param, n9_a2dp_if.sink);
    n9_a2dp_if.sink->param.audio.AfeBlkControl.u4awsflag = 1;
    DSP_MW_LOG_I("A2DP notify cnt address 0x%x Start Ro :%d", 2, &(n9_a2dp_if.source->streamBuffer.AVMBufferInfo.NotifyCount), n9_a2dp_if.source->streamBuffer.AVMBufferInfo.ReadIndex);
#ifdef MTK_BT_A2DP_VENDOR_BC_ENABLE
    if ((n9_a2dp_if.source->param.n9_a2dp.codec_info.codec_cap.type == BT_A2DP_CODEC_VENDOR) && (n9_a2dp_if.source->param.n9_a2dp.sink_latency == 0)) {
        n9_a2dp_if.sink->param.audio.AfeBlkControl.u4awsflag = FALSE;
        DSP_MW_LOG_I("vendor BC force aws flag to be false", 0);
    }
#endif
    //n9_a2dp_if.sink->param.audio.AfeBlkControl.u4awsflag = FALSE;

    n9_a2dp_if.source->param.n9_a2dp.mce_flag = n9_a2dp_if.sink->param.audio.AfeBlkControl.u4awsflag;


    DSP_MW_LOG_I("A2DP latency :%d", 1, n9_a2dp_if.source->param.n9_a2dp.sink_latency);
    stream_feature_configure_resolution((stream_feature_list_ptr_t)n9_a2dp_if.pfeature_table, RESOLUTION_32BIT, CONFIG_DECODER);
#if MTK_HWSRC_IN_STREAM
    U32 samplerate;//modify for ASRC
    //samplerate = afe_get_audio_device_samplerate(n9_a2dp_if.sink->param.audio.audio_device, n9_a2dp_if.sink->param.audio.audio_interface) /1000 ; //modify for SRC
    samplerate = (n9_a2dp_if.sink->param.audio.rate) / 1000;
    DSP_MW_LOG_I("afe_get_audio_device_samplerate = %d\r\n", 1, samplerate); //modify for ASRC
    stream_feature_configure_src(n9_a2dp_if.pfeature_table, RESOLUTION_32BIT, RESOLUTION_32BIT, samplerate, 4096);
    if (n9_a2dp_if.source->param.n9_a2dp.codec_info.codec_cap.type == BT_A2DP_CODEC_SBC) {
        DSP_MW_LOG_I("BT_A2DP_CODEC_SBC samplerate %d fixed point %d", 2, samplerate, 4096);
    }
    if (n9_a2dp_if.source->param.n9_a2dp.codec_info.codec_cap.type == BT_A2DP_CODEC_AAC) {
        DSP_MW_LOG_I("BT_A2DP_CODEC_AAC samplerate %d fixed point %d", 2, samplerate, 4096);
    }
    if (n9_a2dp_if.source->param.n9_a2dp.codec_info.codec_cap.type == BT_A2DP_CODEC_VENDOR) {
        DSP_MW_LOG_I("BT_A2DP_CODEC_VENDOR samplerate %d fixed point %d", 2, samplerate, 4096);
    }
#endif

    n9_a2dp_if.transform = TrasformAudio2Audio(n9_a2dp_if.source, n9_a2dp_if.sink, (stream_feature_list_ptr_t)n9_a2dp_if.pfeature_table);
    n9_a2dp_if.source->param.n9_a2dp.DspReportStartId = msg.ccni_message[0] >> 16 | 0x8000;
    n9_a2dp_if.sink->param.audio.afe_wait_play_en_cnt = 0;
//#ifdef AIR_AUDIO_HARDWARE_ENABLE
    //afe_enable_audio_irq(afe_irq_request_number(AUDIO_DIGITAL_BLOCK_MEM_DL1), n9_a2dp_if.sink->param.audio.rate, n9_a2dp_if.sink->param.audio.count);
//#endif /* AIR_AUDIO_HARDWARE_ENABLE */
    if (n9_a2dp_if.transform == NULL) {
        DSP_MW_LOG_E("A2DP START transform failed", 0);
    }
    // *(volatile uint32_t *)0x70000F58 = 0x00000208; //ZCD_CON2
#endif
}
#ifdef MTK_GAMING_MODE_HEADSET
static uint32_t g_play_en_gpt_handler = NULL;
#endif


void CB_N9_A2DP_STOP(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("A2DP STOP\r\n", 0);
#ifdef TRIGGER_A2DP_PROCSESS_TIMER
    if (trig_a2dp_proc_timer) {
        xTimerStop(trig_a2dp_proc_timer, 0);
    }
#endif /*TRIGGER_A2DP_PROCSESS_TIMER*/
#ifdef PLAY_EN_CHECK_TIMER
    if (playen_check_timer) {
        xTimerStop(playen_check_timer, 0);
    }
#endif
#ifdef MTK_GAMING_MODE_HEADSET
    if (g_play_en_gpt_handler) {
        hal_gpt_sw_stop_timer_us(g_play_en_gpt_handler);
    }
#endif
    if (n9_a2dp_if.transform != NULL) {
        StreamDSPClose(n9_a2dp_if.transform->source, n9_a2dp_if.transform->sink, msg.ccni_message[0] >> 16 | 0x8000);
    }
    n9_a2dp_if.transform = NULL;
    DSP_MW_LOG_I("A2DP STOP Finish\r\n", 0);
}

void CB_N9_A2DP_CLOSE(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{

    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("A2DP CLOSE\r\n", 0);
    DSP_Callback_UnloaderConfig(n9_a2dp_if.pfeature_table, n9_a2dp_if.source->scenario_type);
    SourceClose(n9_a2dp_if.source);
    SinkClose(n9_a2dp_if.sink);
    DSP_MW_LOG_I("aac feature PIC unload : %d %d", 2, *(n9_a2dp_if.pfeature_table),*(n9_a2dp_if.pfeature_table));
    memset(&n9_a2dp_if,0,sizeof(CONNECTION_IF));

    DSP_MW_LOG_I("A2DP CLOSE Finish\r\n", 0);
}

void CB_N9_A2DP_CLOCKSKEW(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    //clock skew to adjust hw src
}

void CB_N9_A2DP_SUSPEND(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("A2DP SUSPEND\r\n", 0);

    dsp_trigger_suspend(n9_a2dp_if.source, n9_a2dp_if.sink);


}

void CB_N9_A2DP_RESUME(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("A2DP RESUME\r\n", 0);


    dsp_trigger_resume(n9_a2dp_if.source, n9_a2dp_if.sink);

#ifdef AIR_AUDIO_HARDWARE_ENABLE
    afe_amp_keep_enable_state(FALSE);
#endif
}

#ifdef AIR_A2DP_REINIT_V2_ENABLE
void Au_DL_receive_reinit_request(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    UNUSED(msg);
    if(n9_a2dp_if.source != NULL){
        n9_a2dp_if.source->param.n9_a2dp.reinit_request= true;
    }
}
#endif

/* End A2DP CCNI callback function */
#endif /* MTK_BT_A2DP_ENABLE */

#ifdef AIR_AUDIO_DETACHABLE_MIC_ENABLE
extern void Source_device_set_para(hal_audio_device_parameter_t *device_handle);

void dsp_detachable_config(CONNECTION_IF *application_ptr, hal_ccni_message_t msg)
{
    audio_transmitter_scenario_type_t scenario_id;
    audio_transmitter_scenario_sub_id_t sub_id;
    mcu2dsp_open_param_p open_param;
    SOURCE source;
    audio_channel pre_ch, ch;
    au_afe_open_param_t afe_open;
    scenario_id = (msg.ccni_message[0] >> 8) & 0xFF;
    sub_id.scenario_id  = (msg.ccni_message[0] & 0xFF);
    DSP_MW_LOG_I("dsp_detachable_config", 0);
    if (msg.ccni_message[1] != NULL) {
        open_param = (mcu2dsp_open_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);
        open_param->stream_in_param.afe.stream_channel = application_ptr->source->param.audio.stream_channel;
        open_param->stream_in_param.afe.memory = application_ptr->source->param.audio.memory;
        if (application_ptr->source->param.audio.echo_reference) {
            open_param->stream_in_param.afe.memory |= HAL_AUDIO_MEM3;
        }
        // re-use parameters
        open_param->stream_in_param.afe.irq_period               = application_ptr->source->param.audio.period;
        open_param->stream_in_param.afe.sampling_rate            = application_ptr->source->param.audio.rate;
        open_param->stream_in_param.afe.stream_out_sampling_rate = application_ptr->source->param.audio.src_rate;
        open_param->stream_in_param.afe.format                   = application_ptr->source->param.audio.format;
        open_param->stream_in_param.afe.frame_number             = application_ptr->source->param.audio.Buffer_Frame_Num;
        open_param->stream_in_param.afe.frame_size               = application_ptr->source->param.audio.Frame_Size;

        afe_open.audio_device  = open_param->stream_in_param.afe.audio_device;
        afe_open.audio_device1 = open_param->stream_in_param.afe.audio_device1;
        afe_open.audio_device2 = open_param->stream_in_param.afe.audio_device2;
        afe_open.audio_device3 = open_param->stream_in_param.afe.audio_device3;
        afe_open.audio_device4 = open_param->stream_in_param.afe.audio_device4;
        afe_open.audio_device5 = open_param->stream_in_param.afe.audio_device5;
        pre_ch = afe_get_audio_channel_by_au_afe_open_param(&(afe_open));
        afe_open.audio_device  = open_param->stream_in_param.afe.audio_device;
        afe_open.audio_device1 = open_param->stream_in_param.afe.audio_device1;
        afe_open.audio_device2 = open_param->stream_in_param.afe.audio_device2;
        afe_open.audio_device3 = open_param->stream_in_param.afe.audio_device3;
        afe_open.audio_device4 = open_param->stream_in_param.afe.audio_device4;
        afe_open.audio_device5 = open_param->stream_in_param.afe.audio_device5;
        ch = afe_get_audio_channel_by_au_afe_open_param(&(afe_open));
        if (pre_ch != ch) {
            HAL_AUDIO_LOG_ERROR("pre_ch=%d != ch=%d", 2, pre_ch, ch);
            platform_assert("please check open paraters.", __FILE__, __LINE__);
        }
        source = dsp_open_stream_in_afe(open_param);
        hal_audio_path_parameter_t   *path_handle   = &source->param.audio.path_handle;
        hal_audio_device_parameter_t *device_handle = &source->param.audio.device_handle;
        hal_audio_memory_parameter_t *mem_handle    = &source->param.audio.mem_handle;     //modify for ab1568
#ifdef AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
        hal_audio_device_parameter_t *device_handle1        = &source->param.audio.device_handle1;
        hal_audio_device_parameter_t *device_handle2        = &source->param.audio.device_handle2;
        hal_audio_device_parameter_t *device_handle3        = &source->param.audio.device_handle3;
        hal_audio_device_parameter_t *device_handle4        = &source->param.audio.device_handle4;
        hal_audio_device_parameter_t *device_handle5        = &source->param.audio.device_handle5;
        hal_audio_device_parameter_t *device_handle6        = &source->param.audio.device_handle6;
        hal_audio_device_parameter_t *device_handle7        = &source->param.audio.device_handle7;
        AUDIO_PARAMETER              *pAudPara              = &source->param.audio;
        pAudPara->audio_device = gAudioCtrl.Afe.AfeULSetting.audio_device;
#ifdef AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
        pAudPara->audio_device1 = gAudioCtrl.Afe.AfeULSetting.audio_device1;
        pAudPara->audio_device2 = gAudioCtrl.Afe.AfeULSetting.audio_device2;
        pAudPara->audio_device3 = gAudioCtrl.Afe.AfeULSetting.audio_device3;
        pAudPara->audio_device4 = gAudioCtrl.Afe.AfeULSetting.audio_device4;
        pAudPara->audio_device5 = gAudioCtrl.Afe.AfeULSetting.audio_device5;
        pAudPara->audio_device6 = gAudioCtrl.Afe.AfeULSetting.audio_device6;
        pAudPara->audio_device7 = gAudioCtrl.Afe.AfeULSetting.audio_device7;
#endif
        pAudPara->audio_interface = gAudioCtrl.Afe.AfeULSetting.audio_interface;
#ifdef AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
        pAudPara->audio_interface1 = gAudioCtrl.Afe.AfeULSetting.audio_interface1;
        pAudPara->audio_interface2 = gAudioCtrl.Afe.AfeULSetting.audio_interface2;
        pAudPara->audio_interface3 = gAudioCtrl.Afe.AfeULSetting.audio_interface3;
        pAudPara->audio_interface4 = gAudioCtrl.Afe.AfeULSetting.audio_interface4;
        pAudPara->audio_interface5 = gAudioCtrl.Afe.AfeULSetting.audio_interface5;
        pAudPara->audio_interface6 = gAudioCtrl.Afe.AfeULSetting.audio_interface6;
        pAudPara->audio_interface7 = gAudioCtrl.Afe.AfeULSetting.audio_interface7;
#endif
#endif
        uint32_t i;
        hal_audio_path_port_parameter_t input_port_parameters, output_port_parameters;
        input_port_parameters.device_interface = pAudPara->audio_interface;
        output_port_parameters.memory_select = mem_handle->memory_select & (~HAL_AUDIO_MEMORY_UL_AWB2);
#ifdef AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
        hal_audio_device_t path_audio_device[HAL_AUDIO_PATH_SUPPORT_SEQUENCE] = {
            pAudPara->audio_device, pAudPara->audio_device1, pAudPara->audio_device2, pAudPara->audio_device3
            #ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
            , pAudPara->audio_device4, pAudPara->audio_device5, pAudPara->audio_device6, pAudPara->audio_device7
            #endif /* MTK_AUDIO_HW_IO_CONFIG_ENHANCE */
        };
        hal_audio_device_interface_t device_interface[HAL_AUDIO_PATH_SUPPORT_SEQUENCE]= {
            pAudPara->audio_interface, pAudPara->audio_interface1, pAudPara->audio_interface2, pAudPara->audio_interface3
            #ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
            , pAudPara->audio_interface4, pAudPara->audio_interface5, pAudPara->audio_interface6, pAudPara->audio_interface7
            #endif /* MTK_AUDIO_HW_IO_CONFIG_ENHANCE */
        };
        hal_audio_memory_selection_t memory_select[HAL_AUDIO_PATH_SUPPORT_SEQUENCE]= {
            HAL_AUDIO_MEMORY_UL_VUL1, HAL_AUDIO_MEMORY_UL_VUL1, HAL_AUDIO_MEMORY_UL_VUL2, HAL_AUDIO_MEMORY_UL_VUL2
            #ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
            , HAL_AUDIO_MEMORY_UL_VUL3, HAL_AUDIO_MEMORY_UL_VUL3, HAL_AUDIO_MEMORY_UL_AWB, HAL_AUDIO_MEMORY_UL_AWB
            #endif /* MTK_AUDIO_HW_IO_CONFIG_ENHANCE */
        };
#else
        hal_audio_device_t path_audio_device[HAL_AUDIO_PATH_SUPPORT_SEQUENCE] = {
            pAudPara->audio_device, pAudPara->audio_device
        };
        hal_audio_device_interface_t device_interface[HAL_AUDIO_PATH_SUPPORT_SEQUENCE] = {
            pAudPara->audio_interface, pAudPara->audio_interface
        };
        hal_audio_memory_selection_t memory_select[HAL_AUDIO_PATH_SUPPORT_SEQUENCE] = {
            output_port_parameters.memory_select, output_port_parameters.memory_select
        };
#endif
        for (i = 0 ; i < path_handle->connection_number ; i++) {
            input_port_parameters.device_interface    = device_interface[i];
            output_port_parameters.memory_select      = memory_select[i];
            path_handle->input.interconn_sequence [i] = stream_audio_convert_control_to_interconn(path_audio_device[i], input_port_parameters, i, true);
            path_handle->output.interconn_sequence[i] = stream_audio_convert_control_to_interconn(HAL_AUDIO_CONTROL_MEMORY_INTERFACE, output_port_parameters, i, true);
        }
        if ((pAudPara->audio_device) & (HAL_AUDIO_CONTROL_DEVICE_ANALOG_MIC_DUAL |
                                        HAL_AUDIO_CONTROL_DEVICE_LINE_IN_DUAL |
                                        HAL_AUDIO_CONTROL_DEVICE_DIGITAL_MIC_DUAL |
                                        HAL_AUDIO_CONTROL_DEVICE_ANC |
                                        HAL_AUDIO_CONTROL_DEVICE_I2S_MASTER |
                                        HAL_AUDIO_CONTROL_DEVICE_I2S_SLAVE)
) {
            device_handle->common.rate = pAudPara->rate;
            device_handle->common.device_interface = pAudPara->audio_interface;
#ifdef AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
            device_handle1->common.rate = pAudPara->rate;
            device_handle1->common.device_interface = pAudPara->audio_interface1;
            device_handle2->common.rate = pAudPara->rate;
            device_handle2->common.device_interface = pAudPara->audio_interface2;
            device_handle3->common.rate = pAudPara->rate;
            device_handle3->common.device_interface = pAudPara->audio_interface3;
            device_handle4->common.rate = pAudPara->rate;
            device_handle4->common.device_interface = pAudPara->audio_interface4;
            device_handle5->common.rate = pAudPara->rate;
            device_handle5->common.device_interface = pAudPara->audio_interface5;
            device_handle6->common.rate = pAudPara->rate;
            device_handle6->common.device_interface = pAudPara->audio_interface6;
            device_handle7->common.rate = pAudPara->rate;
            device_handle7->common.device_interface = pAudPara->audio_interface7;
#endif
            DSP_MW_LOG_I("set device common.rate %d,source rate %d",2,device_handle->common.rate,pAudPara->rate);
        }

        if (pAudPara->audio_device == HAL_AUDIO_CONTROL_DEVICE_I2S_SLAVE) {
            device_handle->i2s_slave.memory_select = mem_handle->memory_select;
        }

        device_handle->common.audio_device = pAudPara->audio_device;
#ifdef AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
        device_handle1->common.audio_device = pAudPara->audio_device1;
        device_handle2->common.audio_device = pAudPara->audio_device2;
        device_handle3->common.audio_device = pAudPara->audio_device3;
        device_handle4->common.audio_device = pAudPara->audio_device4;
        device_handle5->common.audio_device = pAudPara->audio_device5;
        device_handle6->common.audio_device = pAudPara->audio_device6;
        device_handle7->common.audio_device = pAudPara->audio_device7;
#endif
        Source_device_set_para(device_handle);
#ifdef AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
        Source_device_set_para(device_handle1);
        Source_device_set_para(device_handle2);
        Source_device_set_para(device_handle3);
        Source_device_set_para(device_handle4);
        Source_device_set_para(device_handle5);
        Source_device_set_para(device_handle6);
        Source_device_set_para(device_handle7);
#endif
    }
    DSP_MW_LOG_I("dsp_detachable_config finish", 0);
}
#endif

#ifdef AIR_BT_HFP_ENABLE
bool ScoDlStopFlag;
bool g_esco_dl_open_flag = false;
#ifdef AIR_HFP_DNN_PATH_ENABLE
CONNECTION_IF sco_ul_dnn_if;
#endif

/* eSCO CCNI callback function */
void CB_N9_SCO_UL_OPEN(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    U32 scenario = (msg.ccni_message[0] & 0x00ff);
    DSP_MW_LOG_I("eSCO UL OPEN, scenario:%d", 1, scenario);

    mcu2dsp_open_param_p open_param;
    open_param = (mcu2dsp_open_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);
    DSP_REMAP_SHARE_INFO(open_param->stream_out_param.hfp.p_share_info, avm_share_buf_info_ptr);

    if ((scenario == AUDIO_DSP_CODEC_TYPE_CVSD) || (scenario == AUDIO_DSP_CODEC_TYPE_MSBC)) {
        n9_sco_ul_if.source   = dsp_open_stream_in(open_param);
        n9_sco_ul_if.sink     = dsp_open_stream_out(open_param);
        n9_sco_ul_if.transform = NULL;
        DSP_Callback_PreloaderConfig(n9_sco_ul_if.pfeature_table, open_param->audio_scenario_type);
        //DSP_MW_LOG_I("eSCO UL OPEN Finish", 0);
    }
#ifdef AIR_HFP_DNN_PATH_ENABLE
    else if (scenario == AUDIO_DSP_CODEC_TYPE_DNN) {
        sco_ul_dnn_if.source   = dsp_open_stream_in(open_param);
        sco_ul_dnn_if.sink     = dsp_open_stream_out(open_param);
        sco_ul_dnn_if.transform = NULL;
        sco_ul_dnn_if.pfeature_table = (stream_feature_list_ptr_t)&stream_feature_list_hfp_uplink_dnn;
        DSP_Callback_PreloaderConfig(sco_ul_dnn_if.pfeature_table, open_param->audio_scenario_type);
        //DSP_MW_LOG_I("eSCO UL DNN Open Finish", 0);
        //*((volatile uint32_t *)(0xc00001dc)) = 0x1;
        //*((volatile uint32_t *)(0xc00001f0)) = 0x4A42A42;
    }
#endif
}

void CB_N9_SCO_UL_START(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    U32 scenario = (msg.ccni_message[0] & 0x00ff);
    U32 gpt_timer;
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &gpt_timer);
    DSP_MW_LOG_I("eSCO UL START, scenario:%d", 1, scenario);

    mcu2dsp_start_param_p start_param;
    start_param = (mcu2dsp_start_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);

    if (scenario == 0) {
        dsp_start_stream_in(start_param, n9_sco_ul_if.source);
        dsp_start_stream_out(start_param, n9_sco_ul_if.sink);
        n9_sco_ul_if.source->param.audio.AfeBlkControl.u4awsflag = 1;
        stream_feature_configure_resolution((stream_feature_list_ptr_t)n9_sco_ul_if.pfeature_table, RESOLUTION_16BIT, 0);
        n9_sco_ul_if.transform = TrasformAudio2Audio(n9_sco_ul_if.source, n9_sco_ul_if.sink, n9_sco_ul_if.pfeature_table);
        if (n9_sco_ul_if.transform == NULL) {
            DSP_MW_LOG_E("SCO UL transform failed", 0);
        }
        //DSP_MW_LOG_I("eSCO UL START finish",0);
    }
#ifdef AIR_HFP_DNN_PATH_ENABLE
    else if (scenario == AUDIO_DSP_CODEC_TYPE_DNN) {
        dsp_start_stream_in(start_param, sco_ul_dnn_if.source);
        dsp_start_stream_out(start_param, sco_ul_dnn_if.sink);
        sco_ul_dnn_if.source->param.audio.AfeBlkControl.u4awsflag = 0;
        stream_feature_configure_resolution((stream_feature_list_ptr_t)sco_ul_dnn_if.pfeature_table, RESOLUTION_16BIT, 0);
        sco_ul_dnn_if.transform = TrasformAudio2Audio(sco_ul_dnn_if.source, sco_ul_dnn_if.sink, sco_ul_dnn_if.pfeature_table);
        if (sco_ul_dnn_if.transform == NULL) {
            DSP_MW_LOG_E("SCO UL DNN transform failed", 0);
        }
        //DSP_MW_LOG_I("eSCO UL DNN START Finish", 0);
    }
#endif
}

void CB_N9_SCO_UL_STOP(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    U32 scenario = (msg.ccni_message[0] & 0x00ff);
    DSP_MW_LOG_I("eSCO UL STOP, scenario:%d", 1, scenario);

    if (scenario == 0) {
        if (n9_sco_ul_if.transform != NULL) {
            StreamDSPClose(n9_sco_ul_if.transform->source, n9_sco_ul_if.transform->sink, msg.ccni_message[0] >> 16 | 0x8000);
        }
        n9_sco_ul_if.transform = NULL;
        DSP_MW_LOG_I("eSCO UL STOP Finish", 0);
    }
#ifdef AIR_HFP_DNN_PATH_ENABLE
    else if (scenario == AUDIO_DSP_CODEC_TYPE_DNN) {
        if (sco_ul_dnn_if.transform != NULL) {
            StreamDSPClose(sco_ul_dnn_if.transform->source, sco_ul_dnn_if.transform->sink, msg.ccni_message[0] >> 16 | 0x8000);
        }
        sco_ul_dnn_if.transform = NULL;
        DSP_MW_LOG_I("eSCO UL DNN STOP Finish", 0);
    }
#endif
}

void CB_N9_SCO_UL_CLOSE(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    U32 scenario = (msg.ccni_message[0] & 0x00ff);
    if (scenario == 0) {
        DSP_Callback_UnloaderConfig(n9_sco_ul_if.pfeature_table, n9_sco_ul_if.source->scenario_type);
        SourceClose(n9_sco_ul_if.source);
        SinkClose(n9_sco_ul_if.sink);
        memset(&n9_sco_ul_if,0,sizeof(CONNECTION_IF));
        DSP_MW_LOG_I("eSCO UL CLOSE Finish", 0);
    }
#ifdef AIR_HFP_DNN_PATH_ENABLE
    else if (scenario == AUDIO_DSP_CODEC_TYPE_DNN) {
        DSP_Callback_UnloaderConfig(sco_ul_dnn_if.pfeature_table, sco_ul_dnn_if.source->scenario_type);
        SourceClose(sco_ul_dnn_if.source);
        SinkClose(sco_ul_dnn_if.sink);
        memset(&sco_ul_dnn_if,0,sizeof(CONNECTION_IF));
        DSP_MW_LOG_I("eSCO UL DNN CLOSE Finish", 0);
    }
#endif
}

void CB_N9_SCO_UL_PLAY(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);

    if (n9_sco_ul_if.transform != NULL) {
        DSP_MW_LOG_I("eSCO UL PLAY\r\n", 0);
#if 0//moidfy for ab1568
        afe_set_memory_path_enable(AUDIO_DIGITAL_BLOCK_MEM_VUL1, true, true);
        afe_enable_audio_irq(afe_irq_request_number(AUDIO_DIGITAL_BLOCK_MEM_VUL1), n9_sco_ul_if.source->param.audio.rate, n9_sco_ul_if.source->param.audio.count);

        if (n9_sco_ul_if.source->param.audio.echo_reference) {
            afe_set_memory_path_enable(AUDIO_DIGITAL_BLOCK_MEM_AWB, true, true);
        }
#else
        hal_audio_trigger_start_parameter_t start_parameter;
        start_parameter.memory_select = n9_sco_ul_if.source->param.audio.mem_handle.memory_select;
        start_parameter.enable = true;
        hal_audio_set_value((hal_audio_set_value_parameter_t *)&start_parameter, HAL_AUDIO_SET_TRIGGER_MEMORY_START);
#endif
        hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &(n9_sco_ul_if.sink->param.n9sco.ul_play_gpt));
    } else {
        DSP_MW_LOG_I("eSCO UL PLAY when tansform not exist\r\n", 0);
    }
}

void CB_N9_SCO_UL_SUSPEND(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("eSCO UL SUSPEND\r\n", 0);
    dsp_trigger_suspend(n9_sco_ul_if.source, n9_sco_ul_if.sink);

}


void CB_N9_SCO_UL_RESUME(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("eSCO UL RESUME\r\n", 0);
#ifdef AIR_AUDIO_DETACHABLE_MIC_ENABLE
    dsp_detachable_config(&n9_sco_ul_if, msg);
#endif
    dsp_trigger_resume(n9_sco_ul_if.source, n9_sco_ul_if.sink);
}

void CB_N9_SCO_DL_OPEN(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("eSCO DL OPEN\r\n", 0);

    mcu2dsp_open_param_p open_param;
    open_param = (mcu2dsp_open_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);
    DSP_REMAP_SHARE_INFO(open_param->stream_in_param.hfp.p_share_info, avm_share_buf_info_ptr);
    DSP_MW_LOG_I("eSCO DL share info->StartAddr: 0x%x, forwarder: 0x%x\r\n", 2, open_param->stream_in_param.hfp.p_share_info->StartAddr, open_param->stream_in_param.hfp.p_share_info->ForwarderAddr);
    DSP_REMAP_SHARE_INFO(open_param->stream_in_param.hfp.bt_inf_address, uint32_t);
    DSP_REMAP_SHARE_INFO(open_param->stream_in_param.hfp.clk_info_address, uint32_t *);
#ifdef AIR_AIRDUMP_ENABLE
    DSP_REMAP_SHARE_INFO(open_param->stream_in_param.hfp.p_air_dump_buf, uint32_t *);
#endif

    n9_sco_dl_if.source   = dsp_open_stream_in(open_param);
    g_esco_dl_open_flag = true;
    n9_sco_dl_if.sink     = dsp_open_stream_out(open_param);
    g_esco_dl_open_flag = false;
    n9_sco_dl_if.transform = NULL;
    DSP_Callback_PreloaderConfig(n9_sco_dl_if.pfeature_table, open_param->audio_scenario_type);
    //DSP_MW_LOG_I("eSCO DL OPEN Finish\r\n", 0);
}

void CB_N9_SCO_DL_START(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("eSCO DL START\r\n", 0);

    mcu2dsp_start_param_p start_param;
    start_param = (mcu2dsp_start_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);


    dsp_start_stream_in(start_param, n9_sco_dl_if.source);
    dsp_start_stream_out(start_param, n9_sco_dl_if.sink);


    Forwarder_Rx_Buf_Ctrl(TRUE);
#if 1
    n9_sco_dl_if.sink->param.audio.AfeBlkControl.u4awsflag = TRUE;
#else
    n9_sco_dl_if.sink->param.audio.AfeBlkControl.u4awsflag = FALSE; // Open DL AFE directly
#endif
    n9_sco_dl_if.transform = TrasformAudio2Audio(n9_sco_dl_if.source, n9_sco_dl_if.sink, n9_sco_dl_if.pfeature_table);
    ScoDlStopFlag = FALSE;

    if (n9_sco_dl_if.transform == NULL) {
        DSP_MW_LOG_E("SCO DL transform failed", 0);
    } else {
        n9_sco_dl_if.source->param.n9sco.rx_forwarder_en = TRUE;
        //DSP_MW_LOG_E("SCO DL n9_sco_dl_if.sink->param.audio.rx_forwarder_en: %d ", 1, n9_sco_dl_if.source->param.n9sco.rx_forwarder_en);
        Forwarder_Rx_Intr_HW_Handler();
        Forwarder_Rx_Intr_Ctrl(TRUE);
    }
}

void CB_N9_SCO_DL_STOP(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("eSCO DL STOP\r\n", 0);
    ScoDlStopFlag = TRUE;
    if (n9_sco_dl_if.transform != NULL) {
        StreamDSPClose(n9_sco_dl_if.transform->source, n9_sco_dl_if.transform->sink, msg.ccni_message[0] >> 16 | 0x8000);
    }
    n9_sco_dl_if.transform = NULL;
    DSP_MW_LOG_I("eSCO DL STOP Finish\r\n", 0);
}

void CB_N9_SCO_DL_CLOSE(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("eSCO DL CLOSE\r\n", 0);
    DSP_Callback_UnloaderConfig(n9_sco_dl_if.pfeature_table, n9_sco_dl_if.source->scenario_type);
    SourceClose(n9_sco_dl_if.source);
    SinkClose(n9_sco_dl_if.sink);
    memset(&n9_sco_dl_if,0,sizeof(CONNECTION_IF));
    DSP_MW_LOG_I("eSCO DL CLOSE Finish\r\n", 0);
}

void CB_N9_SCO_CLOCKSKEW(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    //TBD
}

void CB_N9_SCO_DL_SUSPEND(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("eSCO DL SUSPEND\r\n", 0);

    dsp_trigger_suspend(n9_sco_dl_if.source, n9_sco_dl_if.sink);

}


void CB_N9_SCO_DL_RESUME(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("eSCO DL RESUME\r\n", 0);

    dsp_trigger_resume(n9_sco_dl_if.source, n9_sco_dl_if.sink);
}

void CB_N9_SCO_ULIRQ(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    //printf("CB_N9_SCO_ULIRQ\r\n");
    //n9_sco_ul_if.source->transform->Handler(n9_sco_ul_if.source,n9_sco_ul_if.sink);
    //vTaskResume(DAV_TASK_ID);
}

void CB_N9_SCO_DLIRQ(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    if ((Source_blks[SOURCE_TYPE_N9SCO] == NULL) || (n9_sco_dl_if.transform == NULL)) {
        DSP_MW_LOG_I("Unexpected N9 DL IRQ", 0);
        return;
    }
    if (Source_blks[SOURCE_TYPE_N9SCO]->param.n9sco.share_info_base_addr->ReadIndex == Source_blks[SOURCE_TYPE_N9SCO]->param.n9sco.share_info_base_addr->WriteIndex) {
#ifdef PT_bufferfull
        Source_blks[SOURCE_TYPE_N9SCO]->param.n9sco.share_info_base_addr->bBufferIsFull = TRUE;
#endif
        DSP_MW_LOG_I("SCO DL bufferfull, ro:%d, wo:%d", 2, Source_blks[SOURCE_TYPE_N9SCO]->param.n9sco.share_info_base_addr->ReadIndex, Source_blks[SOURCE_TYPE_N9SCO]->param.n9sco.share_info_base_addr->WriteIndex);
    }
    if (n9_sco_dl_if.source->param.n9sco.IsFirstIRQ) {
        n9_sco_dl_if.source->param.n9sco.IsFirstIRQ = FALSE;
        U32 gpt_timer;
        hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &gpt_timer);
        DSP_MW_LOG_I("CB_N9_SCO_DLIRQ, First Wo:%d, Ro:%d,GPT : %d\r\n", 3, Source_blks[SOURCE_TYPE_N9SCO]->param.n9sco.share_info_base_addr->WriteIndex, Source_blks[SOURCE_TYPE_N9SCO]->param.n9sco.share_info_base_addr->ReadIndex, gpt_timer);
        if ((n9_sco_dl_if.source != NULL) && (n9_sco_dl_if.source->transform->Handler != NULL)) {
            n9_sco_dl_if.source->transform->Handler(n9_sco_dl_if.source, n9_sco_dl_if.sink);
            vTaskResume((TaskHandle_t)DAV_TASK_ID);
        }
    }
    if (ScoDlStopFlag == TRUE) {
        vTaskResume((TaskHandle_t)DAV_TASK_ID);
    }
}

void CB_N9_SCO_MICIRQ(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    //printf("CB_N9_SCO_MICIRQ\r\n");
    //n9_sco_ul_if.source->transform->Handler(n9_sco_ul_if.source,n9_sco_ul_if.sink);
    //vTaskResume(DAV_TASK_ID);
}
#endif /* End eSCO CCNI callback function */

void CB_AUDIO_DUMP_INIT(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    uint16_t u2SendID;
    uint32_t i;
    UNUSED(ack);

    u2SendID = (msg.ccni_message[0] & 0x00ff);
    DSP_MW_LOG_I("[CB_AUDIO_DUMP_INIT] CCNI ID: %x, Dump Mask: %x, SendID: %x", 3, msg.ccni_message[0], msg.ccni_message[1], u2SendID);
    if (u2SendID == 0) {
        memcpy(AudioDumpCfgIDs,(void *)msg.ccni_message[1],sizeof(AudioDumpCfgIDs));
        //reset dump ids to accum bytes array
        memset(DumpIDsAccumBytes, 0, sizeof(DumpIDsAccumBytes));
        for(i = 0;i < AUDIO_DUMP_CONFIG_MAX_NUM;i++){
            if(AudioDumpCfgIDs[i] != 0){
                DumpIDsAccumBytes[i].dump_id = AudioDumpCfgIDs[i];
            }
        }
    } else if (u2SendID == 1) {
        DumpIdCfgVersion = msg.ccni_message[1];
        DSP_MW_LOG_I("[Audio Dump] DumpIdCfgVersion = %d",1,DumpIdCfgVersion);
    } else if (u2SendID == 2) {
        AudioDumpDevice = msg.ccni_message[1];
        DSP_MW_LOG_I("[Audio Dump] AudioDumpDevice = %d",1,AudioDumpDevice);
    }

#ifdef AIR_AUDIO_DUMP_BY_SPDIF_ENABLE
    audio_dump_init();
#endif
}

#ifdef AIR_BT_CODEC_BLE_ENABLED
/* BLE CCNI callback function */
void CB_N9_BLE_UL_OPEN(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("[BLE] UL OPEN\r\n", 0);


    mcu2dsp_open_param_p open_param;
    open_param = (mcu2dsp_open_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);
    DSP_REMAP_SHARE_INFO(open_param->stream_out_param.ble.p_share_info, n9_dsp_share_info_ptr);

    n9_ble_ul_if.source   = dsp_open_stream_in(open_param);
    n9_ble_ul_if.sink     = dsp_open_stream_out(open_param);
    n9_ble_ul_if.transform = NULL;

    DSP_Callback_PreloaderConfig(n9_ble_ul_if.pfeature_table, open_param->audio_scenario_type);
    DSP_MW_LOG_I("[BLE] UL OPEN Finish\r\n", 0);
}

void CB_N9_BLE_UL_START(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    U32 gpt_timer;
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &gpt_timer);
    DSP_MW_LOG_I("[BLE] UL START gpt %d\r\n", 1, gpt_timer);

    mcu2dsp_start_param_p start_param;
    start_param = (mcu2dsp_start_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);

    dsp_start_stream_in(start_param, n9_ble_ul_if.source);
    dsp_start_stream_out(start_param, n9_ble_ul_if.sink);

    n9_ble_ul_if.source->param.audio.AfeBlkControl.u4awsflag = 1;
#ifdef AIR_UL_FIX_RESOLUTION_32BIT
    stream_feature_configure_resolution((stream_feature_list_ptr_t)n9_ble_ul_if.pfeature_table, RESOLUTION_32BIT, 0);
#endif
    n9_ble_ul_if.transform = TrasformAudio2Audio(n9_ble_ul_if.source, n9_ble_ul_if.sink, n9_ble_ul_if.pfeature_table);
    if (n9_ble_ul_if.transform == NULL) {
        DSP_MW_LOG_E("[BLE] UL transform failed", 0);
    }
    DSP_MW_LOG_I("[BLE] UL START Finish\r\n", 0);
}

void CB_N9_BLE_UL_STOP(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("[BLE] UL STOP\r\n", 0);
    if (n9_ble_ul_if.transform != NULL) {
        StreamDSPClose(n9_ble_ul_if.transform->source, n9_ble_ul_if.transform->sink, msg.ccni_message[0] >> 16 | 0x8000);
    }
    n9_ble_ul_if.transform = NULL;
    DSP_MW_LOG_I("[BLE] UL STOP Finish\r\n", 0);
}

#ifdef AIR_BT_LE_LC3_ENABLE
EXTERN U8 g_lc3_user_cnt;
//EXTERN void LC3_TabDeinit(void);
//EXTERN void LC3_MemDeinit (void);
EXTERN void LC3_Deinit (void);
#endif

void CB_N9_BLE_UL_CLOSE(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    DSP_Callback_UnloaderConfig(n9_ble_ul_if.pfeature_table, n9_ble_ul_if.source->scenario_type);
    SourceClose(n9_ble_ul_if.source);
    SinkClose(n9_ble_ul_if.sink);
#ifdef PRELOADER_ENABLE
    //DSP_PIC_FeatureDeinit(n9_ble_ul_if.pfeature_table);
#endif
#ifdef AIR_BT_LE_LC3_ENABLE
    g_lc3_user_cnt -= 1;
    DSP_MW_LOG_I("g_lc3_user_cnt %d",1,g_lc3_user_cnt);
    if(!g_lc3_user_cnt){
        //LC3_TabDeinit();
        //LC3_MemDeinit();
        LC3_Deinit();

#ifdef PRELOADER_ENABLE
        lc3i_plc_std_library_unload();
        if(lc3_ul_pic_type == LC3I_UL_CALL_10_MS){
            lc3i_fft10ms_library_unload();
        }else{
            lc3i_fft7_5ms_library_unload();
        }
#endif

#ifdef AIR_DSP_MEMORY_REGION_ENABLE
                DspMemoryManagerReturnStatus_t memory_return_status;
                memory_return_status = DspMemoryManager_ReleaseGroupMemory(Component_LE_CALL, SubComponent_LECALL_LC3_PLC_STD, &lc3_plc_memory_addr);
                if(lc3_ul_pic_type = LC3I_UL_CALL_10_MS){
                    memory_return_status = DspMemoryManager_ReleaseGroupMemory(Component_LE_CALL, SubComponent_LECALL_LC3_FFT, &lc3_fft_memory_addr);
                }else{
                    memory_return_status = DspMemoryManager_ReleaseGroupMemory(Component_LE_CALL, SubComponent_LECALL_LC3_FFT, &lc3_fft_memory_addr);
                }
#endif
/*
#ifdef AIR_DSP_MEMORY_REGION_ENABLE
            DspMemoryManagerReturnStatus_t memory_return_status;
            lc3i_plc_std_library_unload();    
            memory_return_status = DspMemoryManager_ReleaseGroupMemory(Component_LE_CALL, SubComponent_LECALL_LC3_PLC_STD, &lc3_plc_memory_addr);
            if(lc3_ul_pic_type = LC3I_UL_CALL_10_MS){
                lc3i_fft10ms_library_unload();
                memory_return_status = DspMemoryManager_ReleaseGroupMemory(Component_LE_CALL, SubComponent_LECALL_LC3_FFT, &lc3_fft_memory_addr);
            }else{
                lc3i_fft7_5ms_library_unload();
                memory_return_status = DspMemoryManager_ReleaseGroupMemory(Component_LE_CALL, SubComponent_LECALL_LC3_FFT, &lc3_fft_memory_addr);
            }
#endif
*/
    }
#endif
    memset(&n9_ble_ul_if,0,sizeof(CONNECTION_IF));
    DSP_MW_LOG_I("[BLE] UL CLOSE Finish\r\n", 0);
}

void CB_N9_BLE_UL_PLAY(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);

    if (n9_ble_ul_if.transform != NULL) {
        DSP_MW_LOG_I("[BLE] UL PLAY\r\n", 0);
        afe_set_memory_path_enable(AUDIO_DIGITAL_BLOCK_MEM_VUL1, true, true);
        //afe_enable_audio_irq(afe_irq_request_number(AUDIO_DIGITAL_BLOCK_MEM_VUL1), n9_ble_ul_if.source->param.audio.rate, n9_ble_ul_if.source->param.audio.count);

        if (n9_ble_ul_if.source->param.audio.echo_reference) {
            afe_set_memory_path_enable(AUDIO_DIGITAL_BLOCK_MEM_AWB, true, true);
        }
        hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &(n9_ble_ul_if.sink->param.n9ble.ul_play_gpt));
    } else {
        DSP_MW_LOG_I("[BLE] UL PLAY when tansform not exist\r\n", 0);
    }
}

void CB_N9_BLE_UL_SUSPEND(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("[BLE] UL SUSPEND\r\n", 0);
    dsp_trigger_suspend(n9_ble_ul_if.source, n9_ble_ul_if.sink);

}

void CB_N9_BLE_UL_RESUME(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("[BLE] UL RESUME\r\n", 0);
    dsp_trigger_resume(n9_ble_ul_if.source, n9_ble_ul_if.sink);
}

bool g_n9_ble_dl_open_flag = false;
bool g_n9_ble_dl_gaming_flag = false;
bool g_n9_ble_dl_ull_flag = false;


void CB_N9_BLE_DL_OPEN(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("[BLE] DL OPEN\r\n", 0);

    mcu2dsp_open_param_p open_param;
    open_param = (mcu2dsp_open_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);
    DSP_MW_LOG_I("[BLE] share info add: 0x%x\r\n", 1, open_param->stream_in_param.ble.p_share_info);
    DSP_REMAP_SHARE_INFO(open_param->stream_in_param.ble.p_share_info, n9_dsp_share_info_ptr);
    DSP_MW_LOG_I("[BLE] share info add: 0x%x ,share buffer startadd: 0x%x\r\n", 2,open_param->stream_in_param.ble.p_share_info, open_param->stream_in_param.ble.p_share_info->start_addr);
#ifdef AIR_AIRDUMP_ENABLE
        DSP_REMAP_SHARE_INFO(open_param->stream_in_param.ble.p_air_dump_buf, uint32_t *);
        DSP_MW_LOG_I("[BLE] airdump addr: 0x%x", 1,open_param->stream_in_param.ble.p_air_dump_buf);
#endif
    n9_ble_dl_if.source   = dsp_open_stream_in(open_param);
    g_n9_ble_dl_open_flag = true;
    g_n9_ble_dl_gaming_flag = (open_param->stream_in_param.ble.context_type == BLE_CONTEXT_GAME) ? true : false;
    g_n9_ble_dl_ull_flag = (open_param->stream_in_param.ble.context_type == BLE_CONTENT_TYPE_ULL_BLE) ? true : false;
    n9_ble_dl_if.sink     = dsp_open_stream_out(open_param);
    g_n9_ble_dl_open_flag = false;
    n9_ble_dl_if.transform = NULL;

    DSP_Callback_PreloaderConfig(n9_ble_dl_if.pfeature_table, open_param->audio_scenario_type);
    DSP_MW_LOG_I("[BLE] DL OPEN Finish\r\n", 0);
}

void CB_N9_BLE_DL_START(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("[BLE] DL START\r\n", 0);

    mcu2dsp_start_param_p start_param;
    start_param = (mcu2dsp_start_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);


    dsp_start_stream_in(start_param, n9_ble_dl_if.source);
    dsp_start_stream_out(start_param, n9_ble_dl_if.sink);
    n9_ble_dl_if.sink->param.audio.AfeBlkControl.u4awsflag = 1;
    n9_ble_dl_if.transform = TrasformAudio2Audio(n9_ble_dl_if.source, n9_ble_dl_if.sink, n9_ble_dl_if.pfeature_table);
    #ifdef AIR_BT_LE_LC3PLUS_ENABLE
    if (n9_ble_dl_if.source->param.n9ble.codec_type == BT_BLE_CODEC_LC3PLUS)
    {
        n9_ble_dl_if.source->param.n9ble.plc_state_len = sizeof(lc3plus_dec_frame_status_t);
        stream_feature_configure_resolution((stream_feature_list_ptr_t)n9_ble_dl_if.pfeature_table, RESOLUTION_32BIT, CONFIG_DECODER);
    }
    #endif
    BleDlStopFlag = FALSE;
    if (n9_ble_dl_if.transform == NULL) {
        DSP_MW_LOG_E("[BLE] DL transform failed", 0);
    }
    //*(volatile uint32_t *)0x70000F58 = 0x00000208; //ZCD_CON2
    DSP_MW_LOG_I("[BLE] DL START Finished\r\n", 0);
}

void CB_N9_BLE_DL_STOP(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("[BLE] DL STOP\r\n", 0);
    BleDlStopFlag = TRUE;
    if (n9_ble_dl_if.transform != NULL) {
        StreamDSPClose(n9_ble_dl_if.transform->source, n9_ble_dl_if.transform->sink, msg.ccni_message[0] >> 16 | 0x8000);
    }
    n9_ble_dl_if.transform = NULL;
    DSP_MW_LOG_I("[BLE] DL STOP Finish\r\n", 0);
}

void CB_N9_BLE_DL_CLOSE(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("[BLE] DL CLOSE\r\n", 0);
    DSP_Callback_UnloaderConfig(n9_ble_dl_if.pfeature_table, n9_ble_dl_if.source->scenario_type);
    SourceClose(n9_ble_dl_if.source);
    SinkClose(n9_ble_dl_if.sink);
#ifdef PRELOADER_ENABLE
    //DSP_PIC_FeatureDeinit(n9_ble_dl_if.pfeature_table);
#endif
#ifdef AIR_BT_LE_LC3_ENABLE
    g_lc3_user_cnt -= 1;
    DSP_MW_LOG_I("g_lc3_user_cnt %d",1,g_lc3_user_cnt);
    if(!g_lc3_user_cnt){
        //LC3_TabDeinit();
        //LC3_MemDeinit();
        LC3_Deinit();

#ifdef PRELOADER_ENABLE
        switch(lc3_dl_pic_type){
            case LC3I_DL_AUDIO_10_MS:
                lc3i_plc_adv_library_unload();
                lc3i_fft10ms_library_unload();
                break;
            case LC3I_DL_AUDIO_7P5_MS:
                lc3i_plc_adv_library_unload();
                lc3i_fft7_5ms_library_unload();
                break;
            case LC3I_DL_CALL_10_MS:
                lc3i_plc_std_library_unload();
                lc3i_fft10ms_library_unload();
                break;
            case LC3I_DL_CALL_7P5_MS:
                lc3i_plc_std_library_unload();
                lc3i_fft7_5ms_library_unload();
                break;
            default:
                break;
        }
#endif
#ifdef AIR_DSP_MEMORY_REGION_ENABLE
            DspMemoryManagerReturnStatus_t memory_return_status;
    
            switch(lc3_dl_pic_type){
                case LC3I_DL_AUDIO_10_MS:
                    memory_return_status = DspMemoryManager_ReleaseGroupMemory(Component_LE_AUDIO, SubComponent_LEAUDIO_LC3_PLC_ADV, &lc3_plc_memory_addr);
                    memory_return_status = DspMemoryManager_ReleaseGroupMemory(Component_LE_AUDIO, SubComponent_LEAUDIO_LC3_FFT, &lc3_fft_memory_addr);                    
                    break;
                case LC3I_DL_AUDIO_7P5_MS:
                    memory_return_status = DspMemoryManager_ReleaseGroupMemory(Component_LE_AUDIO, SubComponent_LEAUDIO_LC3_PLC_ADV, &lc3_plc_memory_addr);
                    memory_return_status = DspMemoryManager_ReleaseGroupMemory(Component_LE_AUDIO, SubComponent_LEAUDIO_LC3_FFT, &lc3_fft_memory_addr);
                    break;
                case LC3I_DL_CALL_10_MS:
                    memory_return_status = DspMemoryManager_ReleaseGroupMemory(Component_LE_CALL, SubComponent_LECALL_LC3_PLC_STD, &lc3_plc_memory_addr);
                    memory_return_status = DspMemoryManager_ReleaseGroupMemory(Component_LE_CALL, SubComponent_LECALL_LC3_FFT, &lc3_fft_memory_addr);
                    break;
                case LC3I_DL_CALL_7P5_MS:
                    memory_return_status = DspMemoryManager_ReleaseGroupMemory(Component_LE_CALL, SubComponent_LECALL_LC3_PLC_STD, &lc3_plc_memory_addr);
                    memory_return_status = DspMemoryManager_ReleaseGroupMemory(Component_LE_CALL, SubComponent_LECALL_LC3_FFT, &lc3_fft_memory_addr);
                    break;
                default:
                   break;
    }
#endif
/*
#ifdef AIR_DSP_MEMORY_REGION_ENABLE
            DspMemoryManagerReturnStatus_t memory_return_status;

            switch(lc3_dl_pic_type){
                case LC3I_DL_AUDIO_10_MS:
                    lc3i_plc_adv_library_unload(); 
                    memory_return_status = DspMemoryManager_ReleaseGroupMemory(Component_LE_AUDIO, SubComponent_LEAUDIO_LC3_PLC_ADV, &lc3_plc_memory_addr);
                    lc3i_fft10ms_library_unload();               
                    memory_return_status = DspMemoryManager_ReleaseGroupMemory(Component_LE_AUDIO, SubComponent_LEAUDIO_LC3_FFT, &lc3_fft_memory_addr);                    
                    break;
                case LC3I_DL_AUDIO_7P5_MS:
                    lc3i_plc_adv_library_unload(); 
                    memory_return_status = DspMemoryManager_ReleaseGroupMemory(Component_LE_AUDIO, SubComponent_LEAUDIO_LC3_PLC_ADV, &lc3_plc_memory_addr);
                    lc3i_fft7_5ms_library_unload();
                    memory_return_status = DspMemoryManager_ReleaseGroupMemory(Component_LE_AUDIO, SubComponent_LEAUDIO_LC3_FFT, &lc3_fft_memory_addr);
                    break;
                case LC3I_DL_CALL_10_MS:
                    lc3i_plc_std_library_unload();
                    memory_return_status = DspMemoryManager_ReleaseGroupMemory(Component_LE_CALL, SubComponent_LECALL_LC3_PLC_STD, &lc3_plc_memory_addr);
                    lc3i_fft10ms_library_unload();
                    memory_return_status = DspMemoryManager_ReleaseGroupMemory(Component_LE_CALL, SubComponent_LECALL_LC3_FFT, &lc3_fft_memory_addr);
                    break;
                case LC3I_DL_CALL_7P5_MS:
                    
                    lc3i_plc_std_library_unload();                
                    memory_return_status = DspMemoryManager_ReleaseGroupMemory(Component_LE_CALL, SubComponent_LECALL_LC3_PLC_STD, &lc3_plc_memory_addr);
                    lc3i_fft7_5ms_library_unload();
                    memory_return_status = DspMemoryManager_ReleaseGroupMemory(Component_LE_CALL, SubComponent_LECALL_LC3_FFT, &lc3_fft_memory_addr);

                    break;
                default:
                   break;
            }
#endif
*/
    }
#endif
    memset(&n9_ble_dl_if,0,sizeof(CONNECTION_IF));
    DSP_MW_LOG_I("[BLE] DL CLOSE Finish\r\n", 0);
}

void CB_N9_BLE_CLOCKSKEW(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    //TBD
}

void CB_N9_BLE_DL_SUSPEND(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("[BLE] DL SUSPEND\r\n", 0);

    dsp_trigger_suspend(n9_ble_dl_if.source, n9_ble_dl_if.sink);

}

void CB_N9_BLE_DL_RESUME(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("[BLE] DL RESUME\r\n", 0);

    dsp_trigger_resume(n9_ble_dl_if.source, n9_ble_dl_if.sink);
}

void CB_N9_BLE_ULIRQ(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    //printf("CB_N9_SCO_ULIRQ\r\n");
    //n9_sco_ul_if.source->transform->Handler(n9_sco_ul_if.source,n9_sco_ul_if.sink);
    //vTaskResume(DAV_TASK_ID);
}

extern bool N9_BLE_SOURCE_ROUTINE(void);
void CB_N9_BLE_DLIRQ(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    /*hal_gpio_set_output(HAL_GPIO_0, 1);
    hal_gpio_set_output(HAL_GPIO_0, 0);*/
    if (N9_BLE_SOURCE_ROUTINE()) {
        return;
    }

    UNUSED(msg);
    UNUSED(ack);
    if ((Source_blks[SOURCE_TYPE_N9BLE] == NULL) || (n9_ble_dl_if.transform == NULL)) {
        DSP_MW_LOG_I("[BLE] Unexpected N9 DL IRQ", 0);
        return;
    }
    if (Source_blks[SOURCE_TYPE_N9BLE]->param.n9ble.share_info_base_addr->read_offset == Source_blks[SOURCE_TYPE_N9BLE]->param.n9ble.share_info_base_addr->write_offset) {
        Source_blks[SOURCE_TYPE_N9BLE]->param.n9ble.share_info_base_addr->bBufferIsFull = TRUE;
        DSP_MW_LOG_I("[BLE] DL bufferfull, Wo:%d, Ro:%d", 2, Source_blks[SOURCE_TYPE_N9BLE]->param.n9ble.share_info_base_addr->write_offset, Source_blks[SOURCE_TYPE_N9BLE]->param.n9ble.share_info_base_addr->read_offset);
    }
    //DSP_MW_LOG_I("[BLE] CB_N9_BLE_DLIRQ, Wo:%d, Ro:%d\r\n", 3, Source_blks[SOURCE_TYPE_N9BLE]->param.n9ble.share_info_base_addr->WriteOffset, Source_blks[SOURCE_TYPE_N9BLE]->param.n9ble.share_info_base_addr->ReadOffset);
    if (n9_ble_dl_if.source->param.n9ble.IsFirstIRQ) {
        /*hal_gpio_set_output(HAL_GPIO_1, 1);
        hal_gpio_set_output(HAL_GPIO_1, 0);*/

        n9_ble_dl_if.source->param.n9ble.IsFirstIRQ = FALSE;
        memset((U8*)Source_blks[SOURCE_TYPE_N9BLE]->streamBuffer.ShareBufferInfo.start_addr, 0, N9BLE_setting->N9Ble_source.Frame_Size);

        U32 gpt_timer;
        hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &gpt_timer);
        DSP_MW_LOG_I("[BLE] CB_N9_BLE_DLIRQ, First Wo:%d, Ro:%d,GPT : %d\r\n", 3, Source_blks[SOURCE_TYPE_N9BLE]->param.n9ble.share_info_base_addr->write_offset, Source_blks[SOURCE_TYPE_N9BLE]->param.n9ble.share_info_base_addr->read_offset, gpt_timer);
        if ((n9_ble_dl_if.source != NULL) && (n9_ble_dl_if.source->transform->Handler != NULL)) {
            n9_ble_dl_if.source->transform->Handler(n9_ble_dl_if.source, n9_ble_dl_if.sink);
            vTaskResume((TaskHandle_t)DAV_TASK_ID);
        }
    }
    if (BleDlStopFlag == TRUE) {
        vTaskResume((TaskHandle_t)DAV_TASK_ID);
    }
}

void CB_N9_BLE_MICIRQ(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    //printf("[BLE] CB_N9_SCO_MICIRQ\r\n");
    //n9_sco_ul_if.source->transform->Handler(n9_sco_ul_if.source,n9_sco_ul_if.sink);
    //vTaskResume(DAV_TASK_ID);
}

/* Below CB_N9_BLE_INIT_PLAY_INFO is used to control how to start the DL/UL flow  */

typedef struct {
    uint32_t iso_interval; /* Unit with BT clock (312.5us) */
    uint32_t dl_timestamp_clk; /* Unit with BT clock (312.5us), indicate the first anchor of DL */
    uint32_t dl_retransmission_window_clk; /* Unit with BT clock (312.5us), valid bit[27:2] */
    uint16_t dl_timestamp_phase; /* Unit with 0.5us, valid value: 0~2499 */
    uint16_t dl_retransmission_window_phase; /* Unit with 0.5us, valid value: 0~2499 */
    uint8_t  dl_ft;
    uint8_t  dl_packet_counter; /* ISO DL packet counter & 0xFF */
    uint8_t  ul_ft;
    uint8_t  ul_packet_counter; /* ISO UL packet counter & 0xFF */
    uint32_t ul_timestamp; /* Unit with BT clock (312.5us), indicate the first anchor of UL */
    uint32_t iso_anchor_clock; 
    uint16_t iso_anchor_phase; 
    uint32_t ul_avm_info_addr; 
    uint32_t dl_avm_info_addr; 
} ble_init_play_info_t;

#define BLE_UL_TIME_MIC_MUTE        1000
#define BLE_UL_TIME_AVM_MARGIN      1000
#define BLE_UL_TIME_STREAM_PROCESS  6000
#define BLE_TIME_BT_CLOCK           625 /* unit with 312.5us */
#define BLE_UL_TIME_PLAY_EN_MARGIN  1000
#define BLE_DL_TIME_PLAY_EN_MARGIN  16000

#define BIT_MASK(n)         (1UL << (n))
#define BT_CLOCK_TICK_MASK  (0x0FFFFFFC)
#define BT_CLOCK_MAX_WRAP   (0x10000000)

void LC_Get_CurrBtClk(BTCLK *pCurrCLK, BTPHASE *pCurrPhase)
{
    *pCurrCLK = rBb->rAudioCtl.rRxClk;
    *pCurrPhase = rBb->rAudioCtl.rRxPhs;
    if (*((volatile uint32_t *)(CONN_BT_TIMCON_BASE + 0x011C)) != 0)
    {
        DSP_MW_LOG_E("[LC] DSP get current bt clk when sleep, 0x%x", 1,*((volatile uint32_t *)(CONN_BT_TIMCON_BASE + 0x011C)) );
    }
}

/******************************************************************************
 *  LC_IsBTTimeExpired
 *  Descprition :
 *  return TRUE  : CurrentBTTime >= ExpiredBTTime
 *  return FALSE : CurrentBTTime <  ExpiredBTTime
 *  This API had considered wrap around case, because it check MSB of Valid Clock bit.
 *******************************************************************************/
bool LC_BtClock_IsBTTimeExpired(BTTIME_STRU_PTR pExpiredBTTime, BTTIME_STRU_PTR pCurrentBTTime)
{
    BTCLK ExpiredCLK = pExpiredBTTime->period & 0xFFFFFFC;
    BTCLK CurrentCLK = pCurrentBTTime->period & 0xFFFFFFC;
    BTCLK DeltaTime;

    DeltaTime = CurrentCLK - ExpiredCLK;
    if ((DeltaTime & BIT_MASK(27)) > 0) {
        //CurrentCLK <  ExpiredTime
        return FALSE;
    } else {
        //CurrentCLK >= ExpiredTime
        if (CurrentCLK == ExpiredCLK) { //Same CLK, We have to compare Phase
            return (pCurrentBTTime->phase >= pExpiredBTTime->phase);
        } else {
            return TRUE;
        }
    }
}
/**
 * LC_Add_us_FromA
 *
 * Add us from a => b = a + n
 *
 * @Author : Wen <wen.huang@airoha.com.tw>
 *
 * @n : In offset unit us
 * @pa : In
 * @pb : out
 * @Return : b = a + n
 */
VOID LC_Add_us_FromA(U32 n, BTTIME_STRU_PTR pa, BTTIME_STRU_PTR pb)
{
    //get cur clk and phase
    BTCLK a_t0 = pa->period & 0xFFFFFFC;
    U32 m;
    m = pa->phase + (n << 1);
    pb->period = (a_t0 + m / 625) &BT_CLOCK_TICK_MASK;
    pb->phase = m % 2500;
}

/**
 * LC_Subtract_us_Fromb
 *
 * Subtract us from b => a = b - n
 *
 * @Author : Wen <wen.huang@airoha.com.tw>
 *
 * @n : In offset unit us
 * @pa : out
 * @pb : in
 * @Return : b - n = a
 */
VOID LC_Subtract_us_Fromb(U32 n, BTTIME_STRU_PTR pa, BTTIME_STRU_PTR pb)
{
    BTCLK b_t0 = pb->period & 0xFFFFFFC;
    BTCLK a_t0;
    U32 x;

    n = n << 1; //change to unit 0.5us
    if (pb->phase >= n) {
        pa->phase = pb->phase - n;
        pa->period = (b_t0 + pa->phase / 625) &BT_CLOCK_TICK_MASK;
    } else {
        x = (n - pb->phase) % 2500;
        pa->phase = (2500 - x) % 2500;
        a_t0 = (BT_CLOCK_MAX_WRAP + b_t0 - ((((n - pb->phase) / 2500) + (pa->phase > 0)) * 4)) & BT_CLOCK_TICK_MASK;
        pa->period = a_t0 + pa->phase / 625;
    }
}

/**
 * LC_Get_Offset_FromAB
 *
 * get offset us between a and b
 *
 * @Author : Wen <wen.huang@airoha.com.tw>
 *
 * @pa : In Point a
 * @pb : In Point b
 * @Return : b-a = ? us , maximum value 134217726us
 */
U32 LC_Get_Offset_FromAB(BTTIME_STRU_PTR pa, BTTIME_STRU_PTR pb)
{
    BTCLK a_t0 = pa->period & 0xFFFFFFC;
    BTCLK b_t0 = pb->period & 0xFFFFFFC;
    BTCLK CLKOffset;
    U32 Phase;
    if (pa->period <= pb->period) {
        CLKOffset = (b_t0 - a_t0);
    } else {
        CLKOffset = (0xFFFFFFF - a_t0 + b_t0 + 1);
    }
    Phase = (CLKOffset * 625) - pa->phase + pb->phase;
    return (Phase >> 1);
}

static uint32_t g_dl_sw_timer_handle, g_ul_sw_timer_handle;

static void ble_init_ul_timer_callback(void *user_data)
{
    hal_audio_trigger_start_parameter_t start_parameter;

    UNUSED(user_data);

    /* Enable UL AFE path */
    start_parameter.memory_select = n9_ble_ul_if.source->param.audio.mem_handle.memory_select;
    start_parameter.enable = true;
    hal_audio_set_value((hal_audio_set_value_parameter_t *)&start_parameter, HAL_AUDIO_SET_TRIGGER_MEMORY_START);
    DSP_MW_LOG_I("[BLE] ble first sw ul callback, for initial setting", 0);
    if (g_ul_sw_timer_handle != 0) {
        hal_gpt_sw_free_timer(g_ul_sw_timer_handle);
        g_ul_sw_timer_handle = 0;
    }
    if ((n9_ble_ul_if.source != NULL) && (n9_ble_ul_if.source->transform != NULL)) {
        AudioCheckTransformHandle(n9_ble_ul_if.source->transform);
    }

#ifdef MTK_BLE_LATENCY_GPIO_DEBUG
    hal_gpio_toggle_pin(HAL_GPIO_11);
#endif
}

static void ble_init_dl_timer_callback(void *user_data)
{
    UNUSED(user_data);

    if ((n9_ble_dl_if.source != NULL) && (n9_ble_dl_if.source->transform != NULL)) {
        AudioCheckTransformHandle(n9_ble_dl_if.source->transform);
    }

    DSP_MW_LOG_I("[BLE] ble first sw dl callback", 0);

    if (g_dl_sw_timer_handle != 0) {
        hal_gpt_sw_free_timer(g_dl_sw_timer_handle);
        g_dl_sw_timer_handle = 0;
    }


#ifdef MTK_BLE_LATENCY_GPIO_DEBUG
    hal_gpio_toggle_pin(HAL_GPIO_8);
#endif
}
#ifndef ALIGN_4
#define ALIGN_4(_value)             (((_value) + 3) & ~3u)
#endif

static void ble_trigger_ul_stream(ble_init_play_info_t *play_info)
{
    uint16_t curr_intra_clk;
    uint32_t curr_bt_clk, during_time, offset_time, relocate_avm_wptr;
    BTTIME_STRU play_en_time, curr_time, expect_time, first_anchor_time;

    /*
      * decide the time of play en
      *     - margin from frame process done to controller begin to copy
      *     - time of process one frame (AFE buffer -> AVM buffer)
      *     - time of AFE buffer begin to capture one frame data
      *     - time of MIC mute(avoid pop noise) because of AFE HW request
      */
    offset_time = n9_ble_ul_if.source->param.audio.period * 1000 + BLE_UL_TIME_STREAM_PROCESS + BLE_UL_TIME_MIC_MUTE + BLE_UL_TIME_AVM_MARGIN;

    first_anchor_time.period = play_info->ul_timestamp;
    first_anchor_time.phase = 0;
    LC_Subtract_us_Fromb(offset_time, &play_en_time, &first_anchor_time);
    n9_ble_ul_if.sink->param.n9ble.predict_timestamp = play_info->ul_timestamp;
    if ((n9_ble_ul_if.sink->param.n9ble.context_type != BLE_CONTENT_TYPE_ULL_BLE)&&(n9_ble_ul_if.sink->param.n9ble.context_type != BLE_CONTENT_TYPE_WIRELESS_MIC))
    {
        n9_ble_ul_if.sink->param.n9ble.predict_timestamp -= ((n9_ble_ul_if.sink->param.n9ble.frame_interval << 1) / BLE_TIME_BT_CLOCK);
    }
    LC_Get_CurrBtClk(&curr_bt_clk, &curr_intra_clk);
    curr_time.period = curr_bt_clk;
    curr_time.phase = curr_intra_clk;
    LC_Add_us_FromA(BLE_UL_TIME_PLAY_EN_MARGIN, &curr_time, &expect_time);

    relocate_avm_wptr = play_info->ul_packet_counter;
    while (LC_BtClock_IsBTTimeExpired(&play_en_time, &expect_time) == true) {
        LC_Add_us_FromA(n9_ble_ul_if.sink->param.n9ble.frame_interval, &play_en_time, &play_en_time);
        relocate_avm_wptr++;
        n9_ble_ul_if.sink->param.n9ble.predict_timestamp += ((n9_ble_ul_if.sink->param.n9ble.frame_interval << 1) / BLE_TIME_BT_CLOCK);
    }


    /* re-locate the RPTR of first frame */
    relocate_avm_wptr %= N9BLE_setting->N9Ble_sink.Buffer_Frame_Num;
    relocate_avm_wptr *= N9BLE_setting->N9Ble_sink.Frame_Size;
    N9Ble_update_writeoffset_share_information(n9_ble_ul_if.sink, relocate_avm_wptr);
    n9_ble_ul_if.sink->param.n9ble.crc_init = n9_ble_ul_if.sink->param.n9ble.share_info_base_addr->asi_cur;//CRC init value saved in asi_current
    DSP_MW_LOG_I("[BLE][sink] initial writeoffset %d ts %d crc_init %d source irq %d", 4, relocate_avm_wptr, n9_ble_ul_if.sink->param.n9ble.predict_timestamp,n9_ble_ul_if.sink->param.n9ble.crc_init,n9_ble_ul_if.source->param.audio.period);

    DSP_MW_LOG_I("[BLE] UL first_anchor_time %d, %d, curr_time %d, %d, expect_time %d, %d, play_en_time %d, %d", 8,
                 first_anchor_time.period, first_anchor_time.phase,
                 curr_time.period, curr_time.phase,
                 expect_time.period, expect_time.phase,
                 play_en_time.period, play_en_time.phase);

    during_time = LC_Get_Offset_FromAB(&curr_time, &play_en_time);
    DSP_MW_LOG_I("[BLE] ble_trigger_ul_stream(), during_time = %d", 1, during_time);
    hal_gpt_sw_get_timer(&g_ul_sw_timer_handle);
    hal_gpt_sw_start_timer_us(g_ul_sw_timer_handle, during_time, ble_init_ul_timer_callback, NULL);
}
static TimerHandle_t ble_playen_check_timer = NULL;

static void ble_playen_check_timer_callback(TimerHandle_t xTimer)
{
    UNUSED(xTimer);
    DSP_MW_LOG_I("ble PLANEN_CHECK_TIMER", 0);
    if ((Sink_blks[SINK_TYPE_AUDIO] != NULL)&&(Sink_blks[SINK_TYPE_AUDIO]->param.audio.irq_exist == FALSE)){
    //if ((n9_ble_dl_if.transform != NULL) && (n9_ble_dl_if.sink->param.audio.irq_exist == FALSE)) {
        DSP_MW_LOG_I("Play en check, native bt_clk 0x%x :0x%x, play en  bt_clk 0x%x :0x%x, enable :0x%x", 5, rBb->rClkCtl.rNativeClock, rBb->rClkCtl.rNativePhase, *((volatile uint32_t *)(CONN_BT_TIMCON_BASE + 0x0204)), *((volatile uint32_t *)(CONN_BT_TIMCON_BASE + 0x0208)), *((volatile uint8_t *)(CONN_BT_TIMCON_BASE + 0x0200)));

    } else {
        xTimerStop(ble_playen_check_timer, 0);
    }
}

#ifndef ALIGN_4
#define ALIGN_4(_value)             (((_value) + 3) & ~3u)
#endif

static void ble_trigger_dl_stream(ble_init_play_info_t *play_info)
{
    uint16_t native_intra_clk;
    uint32_t during_time, native_bt_clk, retransmission_window, relocate_avm_rptr;
    BTTIME_STRU play_en_time, curr_time, expect_time;
    U8 *read_ptr;
    LE_AUDIO_HEADER *buf_header;

    /*
      * decide the time of play en
      *     - retransmission window
      */
    retransmission_window = ((play_info->dl_retransmission_window_clk) >> 2) * 1250 + play_info->dl_retransmission_window_phase / 2;
    play_en_time.period = play_info->dl_timestamp_clk;
    play_en_time.phase = play_info->dl_timestamp_phase;
    LC_Add_us_FromA(retransmission_window, &play_en_time, &play_en_time);

    LC_Get_CurrBtClk(&curr_time.period, &curr_time.phase);
    LC_Add_us_FromA(BLE_DL_TIME_PLAY_EN_MARGIN, &curr_time, &expect_time);
    if (n9_ble_dl_if.source->param.n9ble.dual_cis_status == DUAL_CIS_WAITING_SUB)
    {
        U32 shift_offset = (play_info->dl_packet_counter%N9BLE_setting->N9Ble_source.Buffer_Frame_Num);
        while ((play_info->dl_timestamp_clk +  ((U32)(play_info->dl_ft)*play_info->iso_interval)) < n9_ble_dl_if.source->param.n9ble.predict_timestamp)
        {
            play_info->dl_timestamp_clk += play_info->iso_interval;
            shift_offset += n9_ble_dl_if.source->param.n9ble.frame_per_iso;
        }
        shift_offset += N9BLE_setting->N9Ble_source.Buffer_Frame_Num - (n9_ble_dl_if.source->streamBuffer.ShareBufferInfo.read_offset/(ALIGN_4(n9_ble_dl_if.source->param.n9ble.frame_length + BLE_AVM_FRAME_HEADER_SIZE)));
        shift_offset %= N9BLE_setting->N9Ble_source.Buffer_Frame_Num;
        n9_ble_dl_if.source->param.n9ble.dual_cis_buffer_offset = (((play_info->dl_timestamp_clk +  ((U32)(play_info->dl_ft)*play_info->iso_interval) - n9_ble_dl_if.source->param.n9ble.predict_timestamp)*625/2/n9_ble_dl_if.source->param.n9ble.frame_interval)*(U32)(ALIGN_4(n9_ble_dl_if.source->param.n9ble.frame_length + BLE_AVM_FRAME_HEADER_SIZE)))%n9_ble_dl_if.source->streamBuffer.ShareBufferInfo.length;
        //Default offset, could be wrong but will be correct later
        //DSP_MW_LOG_I("[BLE] Sub CIS dual_cis_buffer_offset : %d mat : %d %d ", 3,n9_ble_dl_if.source->param.n9ble.dual_cis_buffer_offset,shift_offset,(ALIGN_4(n9_ble_dl_if.source->param.n9ble.frame_length + BLE_AVM_FRAME_HEADER_SIZE)));
        n9_ble_dl_if.source->param.n9ble.dual_cis_buffer_offset = (n9_ble_dl_if.source->streamBuffer.ShareBufferInfo.length - n9_ble_dl_if.source->param.n9ble.dual_cis_buffer_offset + (shift_offset*(U32)(ALIGN_4(n9_ble_dl_if.source->param.n9ble.frame_length + BLE_AVM_FRAME_HEADER_SIZE))))%n9_ble_dl_if.source->streamBuffer.ShareBufferInfo.length;
        DSP_MW_LOG_I("[BLE] Sub CIS start ts : %d offset : %d %d ", 3, play_info->dl_timestamp_clk,n9_ble_dl_if.source->param.n9ble.dual_cis_buffer_offset,n9_ble_dl_if.source->streamBuffer.ShareBufferInfo.read_offset);

        if ((play_info->dl_avm_info_addr&0xffff) == ((U32)n9_ble_dl_if.source->param.n9ble.share_info_base_addr&0xffff))
        {
            U32 swap_offset;
            swap_offset = n9_ble_dl_if.source->streamBuffer.ShareBufferInfo.read_offset;
            n9_ble_dl_if.source->streamBuffer.ShareBufferInfo.read_offset = (swap_offset + n9_ble_dl_if.source->param.n9ble.dual_cis_buffer_offset)%n9_ble_dl_if.source->streamBuffer.ShareBufferInfo.length;
            n9_ble_dl_if.source->param.n9ble.dual_cis_buffer_offset = (n9_ble_dl_if.source->streamBuffer.ShareBufferInfo.length - n9_ble_dl_if.source->param.n9ble.dual_cis_buffer_offset)%n9_ble_dl_if.source->streamBuffer.ShareBufferInfo.length;
            DSP_MW_LOG_I("[BLE] Sub CIS swap offset : %d %d", 2, n9_ble_dl_if.source->param.n9ble.dual_cis_buffer_offset,n9_ble_dl_if.source->streamBuffer.ShareBufferInfo.read_offset);
            N9Ble_update_readoffset_share_information(n9_ble_dl_if.source, n9_ble_dl_if.source->streamBuffer.ShareBufferInfo.read_offset);
        }
        n9_ble_dl_if.source->param.n9ble.dual_cis_status = DUAL_CIS_BOTH_ENABLED;
        return;
    }

    /* Init TimeStamp & _reserved_byte_0Dh */
    DSP_MW_LOG_I("[BLE] Init TimeStamp & _reserved_byte_0Dh", 0);
    for (int i = 0; i < N9BLE_setting->N9Ble_source.Buffer_Frame_Num; i++) {
        read_ptr = (U8 *)(n9_ble_dl_if.source->streamBuffer.ShareBufferInfo.start_addr + i * ALIGN_4(n9_ble_dl_if.source->param.n9ble.frame_length + BLE_AVM_FRAME_HEADER_SIZE));
        buf_header = (LE_AUDIO_HEADER *)read_ptr;
        buf_header->TimeStamp = BLE_AVM_INVALID_TIMESTAMP;
        buf_header->_reserved_byte_0Dh = 0x00;
    }

    n9_ble_dl_if.source->streamBuffer.ShareBufferInfo.read_offset = (play_info->dl_packet_counter%N9BLE_setting->N9Ble_source.Buffer_Frame_Num)*(U32)(ALIGN_4(n9_ble_dl_if.source->param.n9ble.frame_length + BLE_AVM_FRAME_HEADER_SIZE));
    DSP_MW_LOG_I("[BLE] init readoffset : %d %d %d ",3,n9_ble_dl_if.source->streamBuffer.ShareBufferInfo.read_offset,play_info->dl_packet_counter,(play_info->dl_packet_counter%N9BLE_setting->N9Ble_source.Buffer_Frame_Num));
    while (LC_BtClock_IsBTTimeExpired(&play_en_time, &expect_time) == true) {
        LC_Add_us_FromA((play_info->iso_interval * BLE_TIME_BT_CLOCK) >> 1, &play_en_time, &play_en_time);
        n9_ble_dl_if.source->param.n9ble.predict_timestamp += play_info->iso_interval;
        N9Ble_SourceUpdateLocalReadOffset(n9_ble_dl_if.source, n9_ble_dl_if.source->param.n9ble.frame_per_iso);
    }
    N9Ble_update_readoffset_share_information(n9_ble_dl_if.source, n9_ble_dl_if.source->streamBuffer.ShareBufferInfo.read_offset);
    DSP_MW_LOG_I("[BLE] DL start anchor timestamp : %d readoffset : %d", 2,n9_ble_dl_if.source->param.n9ble.predict_timestamp,n9_ble_dl_if.source->streamBuffer.ShareBufferInfo.read_offset);

    /* turn on play_en */
    MCE_TransBT2NativeClk(play_en_time.period, play_en_time.phase, &native_bt_clk, &native_intra_clk, BT_CLK_Offset);
    AFE_SET_REG(AFE_MEMIF_PBUF_SIZE, 0, 3 << 0); //reduce pbuffer size
    AFE_SET_REG(AFE_MEMIF_MINLEN, 1, 0XF);//reduce pbuffer size

    hal_audio_afe_set_play_en(native_bt_clk, native_intra_clk);
    DSP_MW_LOG_I("[BLE] ble_trigger_dl_stream() native_bt_clk %d, native_intra_clk %d", 2, native_bt_clk, native_intra_clk);

    DSP_MW_LOG_I("[BLE] DL first_anchor_time %d, %d, curr_time %d, %d, expect_time %d, %d, play_en_time %d, %d", 8,
                 play_info->dl_timestamp_clk, play_info->dl_timestamp_phase,
                 curr_time.period, curr_time.phase,
                 expect_time.period, expect_time.phase,
                 play_en_time.period, play_en_time.phase);

    /* start timer for wakeup later stream process */
    during_time = LC_Get_Offset_FromAB(&curr_time, &play_en_time);
    DSP_MW_LOG_I("[BLE] ble_trigger_dl_stream(), during_time %d", 1, during_time);
    if (n9_ble_dl_if.source->param.n9ble.dual_cis_status == DUAL_CIS_WAITING_MAIN)
    {
        n9_ble_dl_if.source->param.n9ble.dual_cis_status = DUAL_CIS_WAITING_SUB;
    }
    if (ble_playen_check_timer == NULL) {
        ble_playen_check_timer = xTimerCreate("BLE PLANEN_CHECK_TIMER", pdMS_TO_TICKS(10), pdTRUE, 0, ble_playen_check_timer_callback);
    }
    if (!ble_playen_check_timer) {
        DSP_MW_LOG_I("playen_check_timer create timer FAIL", 0);
    } else {
        DSP_MW_LOG_I("playen_check_timer create timer PASS", 0);
        xTimerChangePeriod(ble_playen_check_timer, pdMS_TO_TICKS(10), 0);
    }
    if (during_time > 6000)
    {
        during_time -= 6000;
    }
    hal_gpt_sw_get_timer(&g_dl_sw_timer_handle);
    hal_gpt_sw_start_timer_us(g_dl_sw_timer_handle, during_time, ble_init_dl_timer_callback, NULL);
}

void CB_N9_BLE_INIT_PLAY_INFO(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    ble_init_play_info_t *play_info;

    UNUSED(ack);

    if (n9_ble_dl_if.source != NULL)
    {
    DSP_MW_LOG_I("[BLE] CB_N9_BLE_INIT_PLAY_INFO called cis status : %d", 1,n9_ble_dl_if.source->param.n9ble.dual_cis_status);
    }
    /* As the play info don't go through the host check flow, so the ble flow may has closed when run here */
    /*if (n9_ble_dl_if.source == NULL) {
        DSP_MW_LOG_W("[BLE] already close ble flow", 0);
        return;
    }*/

    play_info = (ble_init_play_info_t *)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);

    DSP_MW_LOG_I("[BLE] play_info->iso_interval %d", 1, play_info->iso_interval);
    if (n9_ble_dl_if.source == NULL) {
        DSP_MW_LOG_W("[BLE] DL already close ble flow", 0);
    } else {
        if ((n9_ble_dl_if.source->param.n9ble.IsPlayInfoReceived == FALSE) || (n9_ble_dl_if.source->param.n9ble.IsSubPlayInfoReceived == FALSE))
        {
            DSP_MW_LOG_I("[BLE] play_info->dl_timestamp_clk %d", 1, play_info->dl_timestamp_clk);
            DSP_MW_LOG_I("[BLE] play_info->dl_timestamp_phase %d", 1, play_info->dl_timestamp_phase);
            DSP_MW_LOG_I("[BLE] play_info->dl_packet_counter %d", 1, play_info->dl_packet_counter);
            DSP_MW_LOG_I("[BLE] play_info->dl_avm_info_addr %d", 1, play_info->dl_avm_info_addr);
            n9_ble_dl_if.source->param.n9ble.iso_interval = (play_info->iso_interval >> 1) * 625;
            n9_ble_dl_if.source->param.n9ble.ret_window_len = ((play_info->iso_interval + 1) >> 1) * 625;
            n9_ble_dl_if.source->param.n9ble.ft = play_info->dl_ft;
            n9_ble_dl_if.source->param.n9ble.seq_miss_cnt = 0;
            n9_ble_dl_if.source->param.n9ble.predict_frame_counter = 0;
            if (n9_ble_dl_if.source->param.n9ble.dual_cis_status != DUAL_CIS_WAITING_SUB)
            {
                n9_ble_dl_if.source->param.n9ble.predict_timestamp = play_info->dl_timestamp_clk + (play_info->dl_ft*play_info->iso_interval);
                n9_ble_dl_if.source->param.n9ble.frame_per_iso = n9_ble_dl_if.source->param.n9ble.iso_interval / n9_ble_dl_if.source->param.n9ble.frame_interval;
                DSP_MW_LOG_I("[le audio DSP] frame per iso %d",1,n9_ble_dl_if.source->param.n9ble.frame_per_iso) ;
                n9_ble_dl_if.source->param.n9ble.IsPlayInfoReceived = TRUE; // It is better to add avm buffer compare
            }
            else
            {
                n9_ble_dl_if.source->param.n9ble.IsSubPlayInfoReceived = TRUE; // It is better to add avm buffer compare
            }
            ble_trigger_dl_stream(play_info);
        }
    }

    if (n9_ble_ul_if.sink == NULL) {
        DSP_MW_LOG_W("[BLE] UL already close ble flow", 0);
    } else if (((play_info->ul_timestamp | play_info->ul_ft | play_info->ul_packet_counter) && (n9_ble_ul_if.sink != NULL))&&(n9_ble_ul_if.sink->param.n9ble.IsPlayInfoReceived == FALSE))  {
        DSP_MW_LOG_I("[BLE] play_info->ul_timestamp %d", 1, play_info->ul_timestamp);
        DSP_MW_LOG_I("[BLE] play_info->ul_packet_counter %d", 1, play_info->ul_packet_counter);
        ble_trigger_ul_stream(play_info);
        n9_ble_ul_if.sink->param.n9ble.IsPlayInfoReceived = TRUE;
    }

}

/* End BLE CCNI callback function */
#endif //AIR_BT_CODEC_BLE_ENABLED

/* CM4 Playback CCNI callback function */
void CB_CM4_PLAYBACK_OPEN(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("[CM4_PB] Open", 0);


    /* remap to non-cacheable address */
    mcu2dsp_open_param_p open_param;
    open_param = (mcu2dsp_open_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);
    DSP_REMAP_SHARE_INFO(open_param->stream_in_param.playback.p_share_info, n9_dsp_share_info_ptr);
    memset(&playback_if, 0 ,sizeof(CONNECTION_IF) );
    playback_if.source   = dsp_open_stream_in(open_param);
    playback_if.sink     = dsp_open_stream_out(open_param);
    DSP_Callback_PreloaderConfig(playback_if.pfeature_table, open_param->audio_scenario_type);
}


void CB_CM4_PLAYBACK_START(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("[CM4_PB] Start", 0);
    mcu2dsp_start_param_p start_param;
    start_param = (mcu2dsp_start_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);

    dsp_start_stream_in(start_param, playback_if.source);
    dsp_start_stream_out(start_param, playback_if.sink);

    playback_if.transform = TrasformAudio2Audio(playback_if.source, playback_if.sink, playback_if.pfeature_table);
    if (playback_if.transform == NULL) {
        DSP_MW_LOG_E("CM4 Playback transform failed", 0);
    }
}


void CB_CM4_PLAYBACK_STOP(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("[CM4_PB] Stop", 0);
    if (playback_if.transform != NULL) {
        StreamDSPClose(playback_if.transform->source, playback_if.transform->sink, msg.ccni_message[0] >> 16 | 0x8000);
    }
    playback_if.transform = NULL;
}

void CB_CM4_PLAYBACK_CLOSE(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("[CM4_PB] Close", 0);
    DSP_Callback_UnloaderConfig(playback_if.pfeature_table, playback_if.source->scenario_type);
    SourceClose(playback_if.source);
    SinkClose(playback_if.sink);
    memset(&playback_if,0,sizeof(CONNECTION_IF));
}

void CB_CM4_PLAYBACK_SUSPEND(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("[CM4_PB]  SUSPEND\r\n", 0);

    dsp_trigger_suspend(playback_if.source, playback_if.sink);
}


void CB_CM4_PLAYBACK_RESUME(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("[CM4_PB]  RESUME\r\n", 0);

    dsp_trigger_resume(playback_if.source, playback_if.sink);

}

/* CM4 Line-in Playback CCNI callback function */
void CB_CM4_LINEIN_PLAYBACK_OPEN(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("LINEIN PLAYBACK OPEN\r\n", 0);


    /* remap to non-cacheable address */
    mcu2dsp_open_param_p open_param;
    open_param = (mcu2dsp_open_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);
    //DSP_REMAP_SHARE_INFO(open_param->stream_in_param.playback.share_info_base_addr, uint32_t);
    memset(&linein_playback_if, 0, sizeof(CONNECTION_IF));
    linein_playback_if.pfeature_table = (stream_feature_list_ptr_t)&stream_feature_list_linein;
    linein_playback_if.source   = dsp_open_stream_in(open_param);
    linein_playback_if.sink     = dsp_open_stream_out(open_param);

    linein_playback_if.source->param.audio.linein_scenario_flag = 1;
    linein_playback_if.sink->param.audio.linein_scenario_flag = 1;

#ifdef AIR_AUDIO_HARDWARE_ENABLE
    Source_Audio_BufferInfo_Rst(linein_playback_if.source, 0);
    Sink_Audio_BufferInfo_Rst(linein_playback_if.sink, 0);
#endif /* AIR_AUDIO_HARDWARE_ENABLE */

    DSP_Callback_PreloaderConfig(linein_playback_if.pfeature_table, open_param->audio_scenario_type);
    DSP_MW_LOG_I("LINEIN PLAYBACK OPEN Finish\r\n", 0);
}

void CB_CM4_LINEIN_PLAYBACK_START(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("LINEIN PLAYBACK START\r\n", 0);
    mcu2dsp_start_param_p start_param;
    start_param = (mcu2dsp_start_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);

    dsp_start_stream_in(start_param, linein_playback_if.source);
    hal_gpt_delay_us(2000);
    dsp_start_stream_out(start_param, linein_playback_if.sink);

#ifndef AIR_BT_CODEC_BLE_ENABLED
    stream_feature_configure_resolution((stream_feature_list_ptr_t)linein_playback_if.pfeature_table, RESOLUTION_32BIT, CONFIG_DECODER);
#endif
    linein_playback_if.transform = TrasformAudio2Audio(linein_playback_if.source, linein_playback_if.sink, linein_playback_if.pfeature_table);
    if (linein_playback_if.transform == NULL) {
        DSP_MW_LOG_E("CM4 Line-in Playback transform failed", 0);
    }
}

void CB_CM4_LINEIN_PLAYBACK_STOP(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);

    DSP_MW_LOG_I("LINEIN PLAYBACK STOP\r\n", 0);
    if (linein_playback_if.transform != NULL) {
        StreamDSPClose(linein_playback_if.transform->source, linein_playback_if.transform->sink, msg.ccni_message[0] >> 16 | 0x8000);
    }
    linein_playback_if.transform = NULL;
    DSP_MW_LOG_I("LINEIN PLAYBACK STOP Finish\r\n", 0);
}

void CB_CM4_LINEIN_PLAYBACK_CLOSE(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("LINEIN PLAYBACK CLOSE\r\n", 0);

    //to cancel Sink_Audio_FlushBuffer mute flag for issue BTA-9374 Line-in switch to BT no sound
    hal_audio_volume_digital_gain_parameter_t           digital_gain;
    memset(&digital_gain, 0, sizeof(hal_audio_volume_digital_gain_parameter_t));
    digital_gain.memory_select = (hal_audio_memory_selection_t)linein_playback_if.sink->param.audio.memory;
    digital_gain.mute_control = HAL_AUDIO_VOLUME_MUTE_FRAMEWORK;
    digital_gain.mute_enable = false;
    digital_gain.is_mute_control = true;
#ifdef AIR_AUDIO_HARDWARE_ENABLE
    hal_audio_set_value((hal_audio_set_value_parameter_t *)&digital_gain, HAL_AUDIO_SET_VOLUME_HW_DIGITAL_GAIN);
#endif /* AIR_AUDIO_HARDWARE_ENABLE */
    DSP_Callback_UnloaderConfig(linein_playback_if.pfeature_table, linein_playback_if.source->scenario_type);
    SourceClose(linein_playback_if.source);
    SinkClose(linein_playback_if.sink);
    //DSP_MW_LOG_I("feature PIC unload : %d",1,*(linein_playback_if.pfeature_table));
    memset(&linein_playback_if, 0, sizeof(CONNECTION_IF));

    DSP_MW_LOG_I("LINEIN PLAYBACK CLOSE Finish\r\n", 0);
}

void CB_CM4_LINEIN_PLAYBACK_SUSPEND(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("LINEIN PLAYBACK SUSPEND\r\n", 0);

    dsp_trigger_suspend(linein_playback_if.source, linein_playback_if.sink);
}

void CB_CM4_LINEIN_PLAYBACK_RESUME(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("LINEIN PLAYBACK RESUME\r\n", 0);

    dsp_trigger_resume(linein_playback_if.source, linein_playback_if.sink);
}

afe_loopback_param_t dsp_afe_loopack;
//extern hal_audio_device_parameter_vow_t vow_control;

void CB_CM4_TRULY_LINEIN_PLAYBACK_OPEN(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    DSP_MW_LOG_I("Cm4 Truly Line-in Playback Open", 0);
    mcu2dsp_open_param_p open_param;
    UNUSED(ack);

#ifdef AB1568_BRING_UP_DSP_DEFAULT_HW_LOOPBACK
    dsp_afe_loopack.in_device = (hal_audio_device_t)HAL_AUDIO_CONTROL_DEVICE_ANALOG_MIC_DUAL;//HAL_AUDIO_CONTROL_DEVICE_LINE_IN_DUAL;
    dsp_afe_loopack.in_interface = HAL_AUDIO_INTERFACE_1;
    //dsp_afe_loopack.in_misc_parms = 259;
    dsp_afe_loopack.out_device = (hal_audio_device_t)HAL_AUDIO_CONTROL_DEVICE_INTERNAL_DAC_DUAL;//HAL_AUDIO_CONTROL_DEVICE_I2S_MASTER;
    dsp_afe_loopack.out_interface = HAL_AUDIO_INTERFACE_1;
    //dsp_afe_loopack.out_misc_parms = 1;
    dsp_afe_loopack.sample_rate = 48000;
    dsp_afe_loopack.with_hw_gain = true;
    dsp_afe_loopack.stream_channel = HAL_AUDIO_DIRECT;
    dsp_afe_loopack.format = AFE_PCM_FORMAT_S32_LE;
#else
    open_param = (mcu2dsp_open_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);

    dsp_afe_loopack.in_device = (hal_audio_device_t)open_param->stream_in_param.afe.audio_device;
    dsp_afe_loopack.in_interface = (hal_audio_interface_t)open_param->stream_in_param.afe.audio_interface;
    dsp_afe_loopack.in_misc_parms.I2sClkSourceType = open_param->stream_in_param.afe.misc_parms;
    dsp_afe_loopack.out_device = open_param->stream_out_param.afe.audio_device;
    dsp_afe_loopack.out_interface = (hal_audio_interface_t)open_param->stream_out_param.afe.audio_interface;
    dsp_afe_loopack.out_misc_parms.I2sClkSourceType = open_param->stream_out_param.afe.misc_parms;
    dsp_afe_loopack.sample_rate = open_param->stream_out_param.afe.sampling_rate;
    dsp_afe_loopack.with_hw_gain = open_param->stream_out_param.afe.hw_gain;
    dsp_afe_loopack.stream_channel = open_param->stream_out_param.afe.stream_channel;
    dsp_afe_loopack.format = open_param->stream_out_param.afe.format;

#endif
#ifdef LINE_IN_PURE_FOR_AMIC_CLASS_G_HQA
    DSP_MW_LOG_I("dsp_afe_loopack.in_device %d, adc_mode %d, adc_type %d\r\n", 3, dsp_afe_loopack.in_device, open_param->stream_in_param.afe.adc_mode, open_param->stream_in_param.afe.adc_type);
    if (dsp_afe_loopack.in_device & (hal_audio_device_t)HAL_AUDIO_CONTROL_DEVICE_ANALOG_MIC_DUAL) {
        dsp_afe_loopack.device_handle_in.analog_mic.adc_parameter.performance = open_param->stream_in_param.afe.performance;
        dsp_afe_loopack.device_handle_in.analog_mic.bias_voltage[0] = open_param->stream_in_param.afe.bias_voltage[0];
        dsp_afe_loopack.device_handle_in.analog_mic.bias_voltage[1] = open_param->stream_in_param.afe.bias_voltage[1];
        dsp_afe_loopack.device_handle_in.analog_mic.bias_voltage[2] = open_param->stream_in_param.afe.bias_voltage[2];
        dsp_afe_loopack.device_handle_in.analog_mic.bias_voltage[3] = open_param->stream_in_param.afe.bias_voltage[3];
        dsp_afe_loopack.device_handle_in.analog_mic.bias_voltage[4] = open_param->stream_in_param.afe.bias_voltage[4];
        dsp_afe_loopack.device_handle_in.analog_mic.bias_select = open_param->stream_in_param.afe.bias_select;
        dsp_afe_loopack.device_handle_in.analog_mic.iir_filter = open_param->stream_in_param.afe.iir_filter[0];
        dsp_afe_loopack.device_handle_in.analog_mic.adc_parameter.adc_mode = open_param->stream_in_param.afe.adc_mode;
        dsp_afe_loopack.device_handle_in.analog_mic.adc_parameter.adc_type = open_param->stream_in_param.afe.adc_type;
        dsp_afe_loopack.device_handle_in.analog_mic.mic_interface = (hal_audio_device_interface_t)open_param->stream_in_param.afe.audio_interface;
    } else if (dsp_afe_loopack.in_device & (hal_audio_device_t)HAL_AUDIO_CONTROL_DEVICE_DIGITAL_MIC_DUAL) {
        dsp_afe_loopack.device_handle_in.digital_mic.dmic_clock_rate = open_param->stream_in_param.afe.performance;
        dsp_afe_loopack.device_handle_in.digital_mic.bias_voltage[0] = open_param->stream_in_param.afe.bias_voltage[0];
        dsp_afe_loopack.device_handle_in.digital_mic.bias_voltage[1] = open_param->stream_in_param.afe.bias_voltage[1];
        dsp_afe_loopack.device_handle_in.digital_mic.bias_voltage[2] = open_param->stream_in_param.afe.bias_voltage[2];
        dsp_afe_loopack.device_handle_in.digital_mic.bias_voltage[3] = open_param->stream_in_param.afe.bias_voltage[3];
        dsp_afe_loopack.device_handle_in.digital_mic.bias_voltage[4] = open_param->stream_in_param.afe.bias_voltage[4];
        dsp_afe_loopack.device_handle_in.digital_mic.bias_select = open_param->stream_in_param.afe.bias_select;
        dsp_afe_loopack.device_handle_in.digital_mic.iir_filter = open_param->stream_in_param.afe.iir_filter[0];
        dsp_afe_loopack.device_handle_in.digital_mic.mic_interface = (hal_audio_device_interface_t)open_param->stream_in_param.afe.audio_interface;
        dsp_afe_loopack.device_handle_in.digital_mic.dmic_selection = open_param->stream_in_param.afe.dmic_selection[0];
        DSP_MW_LOG_I("dsp_afe_loopack dmic_selection %d, clk %d", 2, dsp_afe_loopack.device_handle_in.digital_mic.dmic_selection,
                     open_param->stream_in_param.afe.adc_mode);
    } else if (dsp_afe_loopack.in_device & (hal_audio_device_t)HAL_AUDIO_CONTROL_DEVICE_LINE_IN_DUAL) {
        dsp_afe_loopack.device_handle_in.linein.adc_parameter.performance = open_param->stream_in_param.afe.performance;
        dsp_afe_loopack.device_handle_in.linein.bias_voltage[0] = open_param->stream_in_param.afe.bias_voltage[0];
        dsp_afe_loopack.device_handle_in.linein.bias_voltage[1] = open_param->stream_in_param.afe.bias_voltage[1];
        dsp_afe_loopack.device_handle_in.linein.bias_voltage[2] = open_param->stream_in_param.afe.bias_voltage[2];
        dsp_afe_loopack.device_handle_in.linein.bias_voltage[3] = open_param->stream_in_param.afe.bias_voltage[3];
        dsp_afe_loopack.device_handle_in.linein.bias_voltage[4] = open_param->stream_in_param.afe.bias_voltage[4];
        dsp_afe_loopack.device_handle_in.linein.bias_select = open_param->stream_in_param.afe.bias_select;
        dsp_afe_loopack.device_handle_in.linein.iir_filter = open_param->stream_in_param.afe.iir_filter[0];
        dsp_afe_loopack.device_handle_in.linein.adc_parameter.adc_mode = open_param->stream_in_param.afe.adc_mode;
        //DSP_MW_LOG_I("dsp_afe_loopack linein performance %d, bias_voltagea %d %d %d %d %d, bias_select %d, iir_filter %d, adc_mode %d",9,dsp_afe_loopack.device_handle_in.linein.adc_parameter.performance,
        //    dsp_afe_loopack.device_handle_in.linein.bias_voltage[0],dsp_afe_loopack.device_handle_in.linein.bias_voltage[1],dsp_afe_loopack.device_handle_in.linein.bias_voltage[2],dsp_afe_loopack.device_handle_in.linein.bias_voltage[3],
        //    dsp_afe_loopack.device_handle_in.linein.bias_voltage[4],dsp_afe_loopack.device_handle_in.linein.bias_select,dsp_afe_loopack.device_handle_in.linein.iir_filter,dsp_afe_loopack.device_handle_in.linein.adc_parameter.adc_mode);
    } else if ((dsp_afe_loopack.in_device == (hal_audio_device_t)HAL_AUDIO_CONTROL_DEVICE_I2S_MASTER) || (dsp_afe_loopack.in_device == (hal_audio_device_t)HAL_AUDIO_CONTROL_DEVICE_I2S_MASTER_L) || (dsp_afe_loopack.in_device == (hal_audio_device_t)HAL_AUDIO_CONTROL_DEVICE_I2S_MASTER_R)) {
        dsp_afe_loopack.device_handle_in.i2s_master.i2s_interface = (hal_audio_device_interface_t)open_param->stream_in_param.afe.audio_interface;
    }
#if defined(AIR_BTA_IC_PREMIUM_G2)
    dsp_afe_loopack.device_handle_out.dac.with_high_performance = open_param->stream_out_param.afe.performance;
#else
    dsp_afe_loopack.device_handle_out.dac.performance = open_param->stream_out_param.afe.performance;
#endif
    DSP_MW_LOG_I("dsp_afe_loopack in_device %d in_interface %d in_misc_parms %d out_device %d out_interface %d out_misc_parms %d sample_rate %d with_hw_gain %d stream_channel %d format %d adc_mode %d adc_type %d", 12,
                 dsp_afe_loopack.in_device, dsp_afe_loopack.in_interface, dsp_afe_loopack.in_misc_parms, dsp_afe_loopack.out_device, dsp_afe_loopack.out_interface,
                 dsp_afe_loopack.out_misc_parms, dsp_afe_loopack.sample_rate, dsp_afe_loopack.with_hw_gain, dsp_afe_loopack.stream_channel, dsp_afe_loopack.format,
                 dsp_afe_loopack.device_handle_in.analog_mic.adc_parameter.adc_mode, dsp_afe_loopack.device_handle_in.analog_mic.adc_parameter.adc_type);
#endif
#ifdef AIR_AUDIO_HARDWARE_ENABLE
    afe_set_loopback_enable(true, &dsp_afe_loopack);
#endif /* AIR_AUDIO_HARDWARE_ENABLE */
#if 0

    memset(&dsp_vow_control, 0, sizeof(hal_audio_device_parameter_vow_t));
    dsp_vow_control.audio_device = HAL_AUDIO_CONTROL_DEVICE_VOW;
    dsp_vow_control.dma_irq_threshold = VOW_SRAM_COPY_SIZE;
    dsp_vow_control.snr_threshold = 0x7373;
    dsp_vow_control.alpha_rise = 0x7;
    dsp_vow_control.dmic_selection = HAL_AUDIO_DMIC_GPIO_DMIC0;
    //dsp_vow_control.suspend_mic = true;
    dsp_vow_control.suspend_mic = false;
    dsp_vow_control.input_device = HAL_AUDIO_CONTROL_DEVICE_VOW;
    //dsp_vow_control.mic_selection = HAL_AUDIO_CONTROL_DEVICE_DIGITAL_MIC_DUAL;
    dsp_vow_control.mic_selection = HAL_AUDIO_CONTROL_DEVICE_ANALOG_MIC_DUAL;
    dsp_vow_control.bias_select = HAL_AUDIO_BIAS_SELECT_ALL;
    dsp_vow_control.bias_voltage = HAL_AUDIO_BIAS_VOLTAGE_1_85V;
    dsp_vow_control.mic_interface = HAL_AUDIO_CONTROL_DEVICE_INTERFACE_1;
    dsp_vow_control.memory_select = HAL_AUDIO_MEMORY_UL_VUL1;
    dsp_vow_control.adc_parameter.performance = AFE_PEROFRMANCE_ULTRA_LOW_POWER_MODE;
    dsp_vow_control.rate = 16000;
    dsp_vow_control.vow_detection_done_entry = dsp_vow_isr_handler;
    //dsp_vow_control.vow_mode = AFE_VOW_PHASE0;
    dsp_vow_control.vow_mode = AFE_VOW_PHASE1;
    dsp_vow_control.vow_with_hpf = true;

    vow_sink(NULL, NULL);
    vow_enable(&dsp_vow_control, NULL);


#endif


}

void CB_CM4_TRULY_LINEIN_PLAYBACK_CLOSE(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    DSP_MW_LOG_I("Cm4 Truly Line-in Playback Close", 0);
    UNUSED(msg);
    UNUSED(ack);
#ifdef AIR_AUDIO_HARDWARE_ENABLE
    afe_set_loopback_enable(false, &dsp_afe_loopack);
#endif /* AIR_AUDIO_HARDWARE_ENABLE */
}
/* End CM4 Line-in Playback CCNI callback function */

/* CM4 Record CCNI callback function */
#ifdef MTK_CM4_RECORD_ENABLE
extern bool CM4_Record_air_dump;
extern U8   CM4_Record_air_dump_scenario;
#ifdef MTK_LEAKAGE_DETECTION_ENABLE
extern bool CM4_Record_leakage_enable;
#endif
#ifdef MTK_USER_TRIGGER_FF_ENABLE
extern bool utff_enable;
#endif
#ifdef MTK_WWE_ENABLE
hal_audio_device_t  wwe_audio_device;
#endif
void CB_CM4_RECORD_OPEN(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("Cm4 record Open", 0);

    /* remap to non-cacheable address */
    mcu2dsp_open_param_p open_param;
    open_param = (mcu2dsp_open_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);
    DSP_REMAP_SHARE_INFO(open_param->stream_out_param.record.p_share_info, n9_dsp_share_info_t *);
    open_param->stream_out_param.record.p_share_info->start_addr = hal_memview_cm4_to_dsp0(open_param->stream_out_param.record.p_share_info->start_addr);
    memset(&record_if, 0, sizeof(CONNECTION_IF));
    audio_dsp_codec_type_t audio_dsp_codec_type;
    audio_dsp_codec_type = msg.ccni_message[0] & 0x0fff;
#ifdef MTK_WWE_ENABLE
    g_wwe_mode = ((msg.ccni_message[0] & 0xf000) >> 12);
    DSP_MW_LOG_I("Cm4 record Open wwe_mode = %d", 1, g_wwe_mode);
    wwe_audio_device = open_param->stream_in_param.afe.audio_device;
#endif
    DSP_MW_LOG_I("Cm4 record Open msg.ccni_message[0]:0x%x", 1, msg.ccni_message[0]);
    DSP_MW_LOG_I("Cm4 record Open codec type:0x%3x", 1, audio_dsp_codec_type);
    switch (audio_dsp_codec_type) {
#ifdef MTK_RECORD_OPUS_ENABLE
        case AUDIO_DSP_CODEC_TYPE_OPUS: {
            record_if.pfeature_table = (stream_feature_list_ptr_t)&stream_feature_list_opus_mic_record;
            break;
        }
#endif
#ifdef MTK_LEAKAGE_DETECTION_ENABLE
        case AUDIO_DSP_CODEC_TYPE_ANC_LC: {
            CM4_Record_leakage_enable = true;
            record_if.pfeature_table = (stream_feature_list_ptr_t)&stream_feature_list_leakage_compensation;
            break;
        }
#endif
#ifdef MTK_USER_TRIGGER_FF_ENABLE
        case AUDIO_DSP_CODEC_TYPE_ANC_USER_TRIGGER_FF_SZ: {
            utff_enable = true;
            record_if.pfeature_table = (stream_feature_list_ptr_t)&stream_feature_list_user_trigger_adaptive_ff_sz;
            DSP_MW_LOG_I("[user_trigger_ff]CB_CM4_RECORD_OPEN, codec type:0x%x", 1, audio_dsp_codec_type);
            DSP_MW_LOG_I("[user_trigger_ff]iir_filter[0]:0x%x, iir_filter[1]:0x%x, iir_filter[2]:0x%x", 3, open_param->stream_in_param.afe.iir_filter[0], open_param->stream_in_param.afe.iir_filter[1], open_param->stream_in_param.afe.iir_filter[2]);
            break;
        }
        case AUDIO_DSP_CODEC_TYPE_ANC_USER_TRIGGER_FF_PZ: {
            utff_enable = true;
            record_if.pfeature_table = (stream_feature_list_ptr_t)&stream_feature_list_user_trigger_adaptive_ff_pz;
            DSP_MW_LOG_I("[user_trigger_ff]CB_CM4_RECORD_OPEN, codec type:0x%x", 1, audio_dsp_codec_type);
            DSP_MW_LOG_I("[user_trigger_ff]iir_filter[0]:0x%x, iir_filter[1]:0x%x, iir_filter[2]:0x%x", 3, open_param->stream_in_param.afe.iir_filter[0], open_param->stream_in_param.afe.iir_filter[1], open_param->stream_in_param.afe.iir_filter[2]);
            break;
        }
        case AUDIO_DSP_CODEC_TYPE_ANC_USER_TRIGGER_FF_PZ_FIR: {
            utff_enable = true;
            record_if.pfeature_table = (stream_feature_list_ptr_t)&stream_feature_list_user_trigger_adaptive_ff_pz_fir;
            DSP_MW_LOG_I("[user_trigger_ff]CB_CM4_RECORD_OPEN, codec type:0x%x", 1, audio_dsp_codec_type);
            DSP_MW_LOG_I("[user_trigger_ff]iir_filter[0]:0x%x, iir_filter[1]:0x%x, iir_filter[2]:0x%x", 3, open_param->stream_in_param.afe.iir_filter[0], open_param->stream_in_param.afe.iir_filter[1], open_param->stream_in_param.afe.iir_filter[2]);
            break;
        }
#endif
        case AUDIO_DSP_CODEC_TYPE_PCM: {
            record_if.pfeature_table = (stream_feature_list_ptr_t)&stream_feature_list_mic_record;
            break;
        }
        default:
            if ((audio_dsp_codec_type & 0xff00) == 0xff00) { /*Record air dump.*/
                CM4_Record_air_dump = true;
                CM4_Record_air_dump_scenario = audio_dsp_codec_type & 0x000f;
                record_if.pfeature_table = (stream_feature_list_ptr_t)&stream_feature_list_mic_record_airdump;
            } else {
                record_if.pfeature_table = (stream_feature_list_ptr_t)&stream_feature_list_mic_record;
            }
            break;
    }
#ifdef MTK_WWE_ENABLE
    if (g_wwe_mode != WWE_MODE_NONE) {
        record_if.pfeature_table = (stream_feature_list_ptr_t)&stream_feature_list_wwe_mic_record;
        g_mcu2dsp_vad_param = (mcu2dsp_vad_param_p)open_param->stream_out_param.record.p_share_info->start_addr;
    }
#endif
    record_if.source   = dsp_open_stream_in(open_param);
    record_if.sink     = dsp_open_stream_out(open_param);

    DSP_Callback_PreloaderConfig(record_if.pfeature_table, open_param->audio_scenario_type);
#ifdef MTK_WWE_ENABLE
    wwe_processing_init();
#endif
}


void CB_CM4_RECORD_START(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("Cm4 record Start", 0);
    mcu2dsp_start_param_p start_param;
    start_param = (mcu2dsp_start_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);

    dsp_start_stream_in(start_param, record_if.source);
    dsp_start_stream_out(start_param, record_if.sink);
#ifdef AIR_UL_FIX_RESOLUTION_32BIT
    stream_feature_configure_resolution((stream_feature_list_ptr_t)record_if.pfeature_table, RESOLUTION_32BIT, 0);
#else
    stream_feature_configure_resolution((stream_feature_list_ptr_t)record_if.pfeature_table, RESOLUTION_16BIT, 0);
#endif
    record_if.source->param.audio.AfeBlkControl.u4awsflag = 1;
    record_if.transform = TrasformAudio2Audio(record_if.source, record_if.sink, record_if.pfeature_table);
    hal_audio_trigger_start_parameter_t sw_trigger_start;
    sw_trigger_start.enable = true;
    sw_trigger_start.memory_select = record_if.source->param.audio.mem_handle.memory_select;
    hal_audio_set_value((hal_audio_set_value_parameter_t *)&sw_trigger_start, HAL_AUDIO_SET_TRIGGER_MEMORY_START);
}


void CB_CM4_RECORD_STOP(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("Cm4 record Stop", 0);
#ifdef MTK_WWE_ENABLE
    /*must deinit hwvad first to resume mic ,then stop the record*/
    wwe_hwvad_deinit();
#endif
    if (record_if.transform != NULL) {
        StreamDSPClose(record_if.transform->source, record_if.transform->sink, msg.ccni_message[0] >> 16 | 0x8000);
    } else {
        DSP_MW_LOG_E("Cm4 record not exit, just ack.", 0);
        aud_msg_ack(msg.ccni_message[0] >> 16 | 0x8000, FALSE);
    }
    record_if.transform = NULL;
}

void CB_CM4_RECORD_CLOSE(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("Cm4 record Close", 0);
#ifdef MTK_WWE_ENABLE
    wwe_processing_deinit();
#endif
    DSP_Callback_UnloaderConfig(record_if.pfeature_table, record_if.source->scenario_type);
    SourceClose(record_if.source);
    SinkClose(record_if.sink);
    memset(&record_if,0,sizeof(CONNECTION_IF));
    CM4_Record_air_dump = false;
#ifdef MTK_LEAKAGE_DETECTION_ENABLE
    CM4_Record_leakage_enable = false;
#endif
#ifdef MTK_USER_TRIGGER_FF_ENABLE
    utff_enable = false;
#endif
}

void CB_CM4_RECORD_SUSPEND(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("Cm4 recordSUSPEND\r\n", 0);
    dsp_trigger_suspend(record_if.source, record_if.sink);

}

void CB_CM4_RECORD_RESUME(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("Cm4 record RESUME\r\n", 0);
#ifdef AIR_AUDIO_DETACHABLE_MIC_ENABLE
    dsp_detachable_config(&record_if, msg);
#endif
    dsp_trigger_resume(record_if.source, record_if.sink);
}
#endif /* MTK_CM4_RECORD_ENABLE */
/* End Playback CCNI callback function */

#ifdef MTK_LEAKAGE_DETECTION_ENABLE
void CB_CM4_RECORD_LC_SET_PARAM_ACK(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    anc_leakage_compensation_parameters_nvdm_p leakage_compensation_param;
    void *share_ptr = (void *)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);
    leakage_detection_load_nvkey(share_ptr);

    leakage_compensation_param = (anc_leakage_compensation_parameters_nvdm_p)share_ptr;
    leakage_detection_set_duration(leakage_compensation_param->report_thd, leakage_compensation_param->no_response_thd);
    DSP_MW_LOG_I("[RECORD_LC]CB_CM4_RECORD_LC_SET_PARAM_ACK, ld_thrd:0x%x, RXIN_TXREF_DELAY:0x%x, DIGITAL_GAIN:0x%x, report_thd:%u, no_response_thd:%u\r\n", 5, leakage_compensation_param->LD_para.ld_thrd, leakage_compensation_param->LD_para.RXIN_TXREF_DELAY, leakage_compensation_param->LD_para.DIGITAL_GAIN, leakage_compensation_param->report_thd, leakage_compensation_param->no_response_thd);
}
#endif

#ifdef AIR_SIDETONE_ENABLE
afe_sidetone_param_t dsp_afe_sidetone;
afe_sidetone_param_extension_t dsp_afe_sidetone_extension;
void dsp_sidetone_start(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);

    U32 gpt_timer;
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &gpt_timer);
    DSP_MW_LOG_I("SideTone Start gpt : %d\r\n", 1, gpt_timer);

    mcu2dsp_sidetone_param_p start_param;
    //mcu2dsp_sidetone_param_t sidetone;


#if 0
    dsp_afe_sidetone.in_device      = HAL_AUDIO_DEVICE_MAIN_MIC_DUAL;
    dsp_afe_sidetone.in_interface   = HAL_AUDIO_INTERFACE_1;
    dsp_afe_sidetone.out_device     = HAL_AUDIO_DEVICE_DAC_DUAL;
    dsp_afe_sidetone.out_interface  = HAL_AUDIO_INTERFACE_1;
    dsp_afe_sidetone.channel        = HAL_AUDIO_DIRECT;
    dsp_afe_sidetone.gain           = 600;
#else
    start_param = (mcu2dsp_sidetone_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);
    memcpy(&dsp_afe_sidetone, start_param, sizeof(mcu2dsp_sidetone_param_t));
#endif
    memset(&dsp_afe_sidetone_extension, 0, sizeof(dsp_afe_sidetone_extension));
    //afe_set_sidetone_enable(true, dsp_afe_sidetone);

#ifdef AIR_AUDIO_HARDWARE_ENABLE
    afe_set_sidetone_enable_flag(true, dsp_afe_sidetone.gain);
#endif /* AIR_AUDIO_HARDWARE_ENABLE */

    DSP_MW_LOG_I("SideTone Start gain %d\r\n", 1, dsp_afe_sidetone.gain);
    DTM_enqueue(DTM_EVENT_ID_SIDETONE_START, 0, false);
}

void dsp_sidetone_stop(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("SideTone Stop\r\n", 0);
#if 0
    mcu2dsp_sidetone_param_p start_param;
    mcu2dsp_sidetone_param_t sidetone;
    start_param = (mcu2dsp_sidetone_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);
    memcpy(&sidetone, start_param, sizeof(mcu2dsp_sidetone_param_t));
#endif
    //afe_set_sidetone_enable(false, dsp_afe_sidetone);
#ifdef AIR_AUDIO_HARDWARE_ENABLE
    afe_set_sidetone_enable_flag(false, dsp_afe_sidetone.gain);
#endif /* AIR_AUDIO_HARDWARE_ENABLE */
#if 0//modify for ab1568
#ifdef ENABLE_SIDETONE_RAMP_TIMER
    fw_sidetone_set_ramp_timer(FW_SIDETONE_MUTE_GAIN);
#else
    DTM_enqueue(DTM_EVENT_ID_SIDETONE_STOP, 0, false);
#endif
#else
    DTM_enqueue(DTM_EVENT_ID_SIDETONE_STOP_RAMP, 0, false);
    //hal_audio_set_device(&(dsp_afe_sidetone.device_handle_in_side_tone), dsp_afe_sidetone.device_handle_in_side_tone.sidetone.audio_device, HAL_AUDIO_CONTROL_OFF);
    DSP_MW_LOG_I("sidetone device 0x%x off\r\n", 1, dsp_afe_sidetone_extension.device_handle_in_side_tone.sidetone.audio_device);
#endif
}

void dsp_sidetone_set_volume(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("SideTone Set_volume\r\n", 0);
    int32_t sidetone_gain = (int32_t)msg.ccni_message[1];
#ifdef AIR_AUDIO_HARDWARE_ENABLE
    afe_set_sidetone_volume(sidetone_gain);
#endif /* AIR_AUDIO_HARDWARE_ENABLE */
}

void dsp_sidetone_start_volume_set(void)
{
    if (afe_get_sidetone_enable_flag() == true) {
        //hal_gpt_delay_ms(200);
        vTaskDelay(pdMS_TO_TICKS(200));
#ifdef AIR_AUDIO_HARDWARE_ENABLE
        afe_set_sidetone_volume(afe_get_sidetone_gain());
#endif /* AIR_AUDIO_HARDWARE_ENABLE */
    }
}
#endif

void dsp_dc_compensation_start(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
#ifdef AIR_AUDIO_HARDWARE_ENABLE
    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("[DC COMPENSATION] Start\r\n", 0);
    hal_audio_device_parameter_t device_handle;
    hal_audio_set_value_parameter_t set_val_handle;
    memset(&device_handle, 0, sizeof(hal_audio_device_parameter_t));
    memset(&set_val_handle, 0, sizeof(hal_audio_set_value_parameter_t));
    mcu2dsp_open_param_p open_param;
    open_param = (mcu2dsp_open_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);
    bool dac_performance = open_param->stream_out_param.afe.performance;
    hal_audio_analog_mdoe_t dac_mode = open_param->stream_out_param.afe.dl_dac_mode;
    set_val_handle.analog_output_mode.dac_mode = dac_mode;
    set_val_handle.analog_output_mode.gain_select = AFE_HW_ANALOG_GAIN_OUTPUT;
    hal_audio_set_value(&set_val_handle, HAL_AUDIO_SET_VOLUME_OUTPUT_ANALOG_SETTING);
#if defined(AIR_BTA_IC_PREMIUM_G2)
    if ((dac_mode == HAL_AUDIO_ANALOG_OUTPUT_CLASSAB) ||
        (dac_mode == HAL_AUDIO_ANALOG_OUTPUT_CLASSG)) {
#else
    if ((dac_mode == HAL_AUDIO_ANALOG_OUTPUT_CLASSAB) ||
        (dac_mode == HAL_AUDIO_ANALOG_OUTPUT_CLASSG2) ||
        (dac_mode == HAL_AUDIO_ANALOG_OUTPUT_CLASSG3)) {
#endif
        device_handle.dac.rate = 48000;
        device_handle.dac.dac_mode = dac_mode;

        device_handle.dac.dc_compensation_value = afe.stream_out.dc_compensation_value;
#if defined(AIR_BTA_IC_PREMIUM_G2)
        device_handle.dac.with_high_performance = dac_performance;
#else
        device_handle.dac.performance = dac_performance;
#endif
        device_handle.dac.with_force_change_rate = false;

        hal_audio_set_device(&device_handle, HAL_AUDIO_CONTROL_DEVICE_INTERNAL_DAC_DUAL, HAL_AUDIO_CONTROL_ON);

        AFE_WRITE(0xC0000EDC, 0x11004089);              // Setting voltage high to 0.9v
#if 1//modfiy for ab1568
        //ANA_SET_REG (0xA207012C, 0x1<<2, 0x1<<2); // Enable trim buffer's reference voltage
        ANA_SET_REG(AUDDEC_ANA_CON11, 0x1 << 2, 0x1 << 2); // Enable trim buffer's reference voltage
        DSP_MW_LOG_I("AUDDEC_ANA_CON11 addr:0x%x val:0x%x\r\n", 2, AUDDEC_ANA_CON11, AFE_READ(AUDDEC_ANA_CON11));
#endif
    } else {
        device_handle.analog_mic.audio_device = HAL_AUDIO_CONTROL_DEVICE_ANALOG_MIC_DUAL;
        device_handle.analog_mic.rate = 48000;
        device_handle.analog_mic.mic_interface = HAL_AUDIO_CONTROL_DEVICE_INTERFACE_1;
        device_handle.analog_mic.bias_select = HAL_AUDIO_BIAS_SELECT_BIAS0;
        device_handle.analog_mic.bias_voltage[0] = HAL_AUDIO_BIAS_VOLTAGE_1_85V;
        device_handle.analog_mic.iir_filter = false;
        device_handle.analog_mic.with_external_bias = false;
        device_handle.analog_mic.with_bias_lowpower = false;
        device_handle.analog_mic.with_phase_inverse = false;
        device_handle.analog_mic.adc_parameter.adc_mode = HAL_AUDIO_ANALOG_INPUT_ACC10K;              //AU_AFE_IN_MODE_t
        device_handle.analog_mic.adc_parameter.performance = dac_performance;  //AU_AFE_POWER_SETTING_t
        device_handle.analog_mic.adc_parameter.is_fifo_sync_clock_ch1 = false;
        device_handle.analog_mic.adc_parameter.with_au_in_swap = false;
        hal_audio_set_device(&device_handle, device_handle.analog_mic.audio_device, HAL_AUDIO_CONTROL_ON);
    }
//#endif
#else
    UNUSED(msg);
    UNUSED(ack);
#endif /* AIR_AUDIO_HARDWARE_ENABLE */
}

void dsp_dc_compensation_stop(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
#ifdef AIR_AUDIO_HARDWARE_ENABLE
    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("[DC COMPENSATION] Stop\r\n", 0);
    afe.stream_out.dc_compensation_value = (uint16_t)msg.ccni_message[0];
    hal_audio_device_parameter_t device_handle;
    memset(&device_handle, 0, sizeof(hal_audio_device_parameter_t));
    mcu2dsp_open_param_p open_param;
    open_param = (mcu2dsp_open_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);
    bool dac_performance = open_param->stream_out_param.afe.performance;
    hal_audio_analog_mdoe_t dac_mode = open_param->stream_out_param.afe.dl_dac_mode;
    bool no_delay_flag = open_param->stream_out_param.afe.misc_parms; // just reuse this parameter
    afe.stream_out.dac_mode = dac_mode;
#if defined(AIR_BTA_IC_PREMIUM_G2)
    if ((dac_mode == HAL_AUDIO_ANALOG_OUTPUT_CLASSAB) ||
        (dac_mode == HAL_AUDIO_ANALOG_OUTPUT_CLASSG)) {
#else
    if ((dac_mode == HAL_AUDIO_ANALOG_OUTPUT_CLASSAB) ||
        (dac_mode == HAL_AUDIO_ANALOG_OUTPUT_CLASSG2) ||
        (dac_mode == HAL_AUDIO_ANALOG_OUTPUT_CLASSG3)) {
#endif
        ANA_WRITE(AUDDEC_ANA_CON6, 0x0070);
        //ANA_SET_REG (AUDDEC_ANA_CON10, 0, 0x1<<2);  // Disable trim buffer's reference voltage
        ANA_SET_REG(AUDDEC_ANA_CON11, 0, 0x1 << 2); // Disable trim buffer's reference voltage
        ANA_WRITE(AUDDEC_ANA_CON6, 0x0030);

        AFE_WRITE(0xC0000EDC, 0x11004001);          // Setting voltage low to 0.42v
        device_handle.dac.rate = 48000;
        device_handle.dac.dac_mode = dac_mode;
#if defined(AIR_BTA_IC_PREMIUM_G2)
        device_handle.dac.with_high_performance = dac_performance;
#else
        device_handle.dac.performance = dac_performance;
#endif
        device_handle.dac.dc_compensation_value = afe.stream_out.dc_compensation_value;
        device_handle.dac.with_force_change_rate = false;
        hal_audio_set_device(&device_handle, HAL_AUDIO_CONTROL_DEVICE_INTERNAL_DAC_DUAL, HAL_AUDIO_CONTROL_OFF);
    } else {
        hal_audio_analog_mdoe_t dl_analog_mode = HAL_AUDIO_ANALOG_OUTPUT_CLASSD;
        device_handle.analog_mic.audio_device = HAL_AUDIO_CONTROL_DEVICE_ANALOG_MIC_DUAL;
        device_handle.analog_mic.rate = 48000;
        device_handle.analog_mic.mic_interface = HAL_AUDIO_CONTROL_DEVICE_INTERFACE_1;
        device_handle.analog_mic.bias_select = HAL_AUDIO_BIAS_SELECT_BIAS0;
        device_handle.analog_mic.bias_voltage[0] = HAL_AUDIO_BIAS_VOLTAGE_1_85V;
        device_handle.analog_mic.iir_filter = false;
        device_handle.analog_mic.with_external_bias = false;
        device_handle.analog_mic.with_bias_lowpower = false;
        device_handle.analog_mic.with_phase_inverse = false;
        device_handle.analog_mic.adc_parameter.adc_mode = HAL_AUDIO_ANALOG_INPUT_ACC10K;              //AU_AFE_IN_MODE_t
        device_handle.analog_mic.adc_parameter.performance = dac_performance;//AU_AFE_POWER_SETTING_t
        device_handle.analog_mic.adc_parameter.is_fifo_sync_clock_ch1 = false;
        device_handle.analog_mic.adc_parameter.with_au_in_swap = false;
        afe.stream_out.dc_compensation_value = hal_audio_get_value((hal_audio_get_value_parameter_t *)&dl_analog_mode, HAL_AUDIO_GET_DEVICE_DAC_COMPENSATION_VALUE);
        hal_audio_set_device(&device_handle, device_handle.analog_mic.audio_device, HAL_AUDIO_CONTROL_OFF);
    }
//#endif

    DSP_MW_LOG_I("DC COMPENSATION val:0x%x, no_delay_flag = 0x%x\r\n", 2, afe.stream_out.dc_compensation_value, no_delay_flag);

    uint32_t value = 0;
    if (no_delay_flag != 0) {
        value = 0;
    } else {
        value = 2000;
    }
    hal_audio_set_value((hal_audio_set_value_parameter_t *)&value, HAL_AUDIO_SET_DEVICE_AMP_DELAY_TIMER_MS);
    if (dac_mode == HAL_AUDIO_ANALOG_OUTPUT_CLASSD) {
        DSP_MW_LOG_I("[DC COMPENSATION] classd unlock amp", 0);
        afe_send_amp_status_ccni(false);
    }
#else
    UNUSED(msg);
    UNUSED(ack);
#endif /* AIR_AUDIO_HARDWARE_ENABLE */
}

#ifdef MTK_BT_A2DP_ENABLE
void dsp_alc_switch(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("a2dp alc switch :%d\r\n", 1, msg.ccni_message[1]);
#ifdef AIR_AUDIO_HARDWARE_ENABLE
    Audio_setting->Audio_sink.alc_enable = (bool)(uint32_t)(msg.ccni_message[1]);
#endif /* AIR_AUDIO_HARDWARE_ENABLE */
}
#endif /* MTK_BT_A2DP_ENABLE */

#if defined(MTK_PEQ_ENABLE) || defined(MTK_LINEIN_PEQ_ENABLE)
void dsp_peq_set_param(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    BOOL BypassTimestamp = FALSE;
    mcu2dsp_peq_param_p peq_param;
    peq_param = (mcu2dsp_peq_param_p)hal_memview_mcu_to_dsp0(msg.ccni_message[1]);

#ifdef MTK_CELT_DEC_ENABLE
    DSP_STREAMING_PARA_PTR  pStream;
    if (peq_param->setting_mode == PEQ_SYNC) {
        if (n9_a2dp_if.source != NULL && n9_a2dp_if.sink != NULL) {
            pStream = DSP_Streaming_Get(n9_a2dp_if.source, n9_a2dp_if.sink);
            if (pStream == NULL) {
                BypassTimestamp = FALSE;
            } else if (pStream->callback.FeatureTablePtr->ProcessEntry == stream_codec_decoder_celt_process) {
                BypassTimestamp = TRUE;
            }
        }
    }
#endif

    DSP_MW_LOG_I("PEQ set param with phase %d\r\n", 1, peq_param->phase_id);
    PEQ_Set_Param(msg, ack, BypassTimestamp);
    if ((msg.ccni_message[0] & 0xFFFF) == 0) {
        Audio_CPD_Enable(peq_param->drc_enable, peq_param->phase_id, peq_param->drc_force_disable, 0, peq_param->peq_nvkey_id);
    } else if ((msg.ccni_message[0] & 0xFFFF) == 1) {
        Audio_CPD_Enable(peq_param->drc_enable, peq_param->phase_id, peq_param->drc_force_disable, 1, peq_param->peq_nvkey_id);
    } else {
        Audio_CPD_Enable(peq_param->drc_enable, peq_param->phase_id, peq_param->drc_force_disable, 3, peq_param->peq_nvkey_id);
    }
}
#endif

#ifdef AIR_ADAPTIVE_EQ_ENABLE
void dsp_adaptive_eq_set_param(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    uint8_t adaptive_eq_number;
    adaptive_eq_number = (msg.ccni_message[0] & 0xFFFF);

    if(adaptive_eq_number == AEQ_GET_INDEX_INFO){
        ADAPTIVE_EQ_Set_Share_Addr(msg, ack);
    }else{
        DSP_MW_LOG_I("adaptive peq set param number %d\r\n", 1, adaptive_eq_number);
        ADAPTIVE_EQ_Set_Param(msg, ack);
    }
}
#endif

#ifdef MTK_GAMING_MODE_HEADSET

static void trigger_a2dp_airoha_proc_timer_callback(void *user_data)
{
    UNUSED(user_data);
//    hal_gpio_set_output(HAL_GPIO_9, 1);//debug for play en decode 1st frame
    U32 bt_clk_cur;
    U16 intra_clk_cur;
    MCE_GetBtClk((BTCLK *)&bt_clk_cur, (BTPHASE *)&intra_clk_cur, ULL_CLK_Offset);
    DSP_MW_LOG_I("[AiroGaming]Play En Timer Callback Decode 1st Frame,Clk:0x%x,phase:0x%x", 2, bt_clk_cur, intra_clk_cur);
    if ((n9_a2dp_if.source != NULL) && (n9_a2dp_if.sink != NULL) && (n9_a2dp_if.transform != NULL) && (n9_a2dp_if.sink->param.audio.irq_exist == FALSE)) {
        n9_a2dp_if.source->transform->Handler(n9_a2dp_if.source, n9_a2dp_if.sink);
        if (n9_a2dp_if.source->transform->sink->taskid == DPR_TASK_ID) {
            xTaskResumeFromISR(pDPR_TaskHandler);
        } else if (n9_a2dp_if.source->transform->sink->taskid == DHP_TASK_ID) {
            xTaskResumeFromISR(pDHP_TaskHandler);
        } else {
            xTaskResumeFromISR(pDAV_TaskHandler);
        }
    } else {
        DSP_MW_LOG_W("[AiroGaming] Trigger 1ST Decode faill", 0);
    }
    hal_gpt_sw_free_timer(g_play_en_gpt_handler);
    g_play_en_gpt_handler = NULL;
//    hal_gpio_set_output(HAL_GPIO_9, 0);//debug for play en decode 1st frame
}

void dsp_a2dp_set_dl_play_en(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    BTCLK bt_clk;
    BTPHASE bt_phase;
    audio_dsp_a2dp_dl_play_en_param_p play_en_param;

    play_en_param = (audio_dsp_a2dp_dl_play_en_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);
    if (n9_a2dp_if.sink->param.audio.channel_num >= 2) {
        n9_a2dp_if.sink->streamBuffer.BufferInfo.WriteOffset = (120 << 2) << 1;
    } else {
        n9_a2dp_if.sink->streamBuffer.BufferInfo.WriteOffset = (120 << 2);
    }
    AFE_SET_REG(AFE_MEMIF_PBUF_SIZE, 0, 3 << 0); //reduce pbuffer size
    AFE_SET_REG(AFE_MEMIF_MINLEN, 1, 0XF);//reduce pbuffer size
    DSP_MW_LOG_I("A2DP DL - Play En SeqNo:%d, BtClk:0x%x", 2, play_en_param->sequence_number, play_en_param->bt_clock);

    audio_headset_ull_ctrl.play_en.sequence_number = play_en_param->sequence_number;
    //audio_headset_ull_ctrl.play_en.bt_clock = ((play_en_param->bt_clock + 28 /* 8.75ms DL reserve time */) & BTCLK_27_0_MASK);
    audio_headset_ull_ctrl.play_en.bt_clock = ((play_en_param->bt_clock + 22 - 2 /* 6.5ms DL reserve time */) & BTCLK_27_0_MASK);
    //================debug test===================
    BTCLK bt_clk_1, bt_clk_2, bt_clk_3, bt_clk_4;
    BTPHASE bt_phase_1, bt_phase_2, bt_phase_3, bt_phase_4;
    MCE_Get_BtClkOffset(&bt_clk_1, &bt_phase_1, ULL_CLK_Offset);
    MCE_Get_BtClkOffset(&bt_clk_2, &bt_phase_2, BT_CLK_Offset);

    MCE_GetBtClk(&bt_clk_3, &bt_phase_3, ULL_CLK_Offset);
    MCE_GetBtClk(&bt_clk_4, &bt_phase_4, BT_CLK_Offset);
    if ((bt_clk_1 != bt_clk_2) && (rBb_ull->rValid)) {
        DSP_MW_LOG_E("[MCE]ULL_CLK_Offset != BT_CLK_Offset , ULL_CLK:0x%x, BT_CLK:0x%x", 2, bt_clk_3, bt_clk_4);
        AUDIO_ASSERT(0);
    } else {
        DSP_MW_LOG_I("[MCE]ULL_CLK_Offset != BT_CLK_Offset , ULL_CLK:0x%x, BT_CLK:0x%x", 2, bt_clk_3, bt_clk_4);
    }
    //==========================================
    if (audio_headset_ull_ctrl.is_a2dp_started) {

        MCE_GetBtClk(&bt_clk, &bt_phase, ULL_CLK_Offset);
        audio_headset_ull_ctrl.play_en.bt_clock += ((n9_a2dp_if.source->param.n9_a2dp.sink_latency - ULL_BASIC_LATENCY) * 8 / 2500); /* 8/2500 = 312.5 for frame based  */

        DSP_MW_LOG_I("A2DP DL - A2DP Start First, Current BtClk:0x%x, Phase:0x%x", 2, bt_clk, bt_phase);

        a2dp_ull_get_proper_play_en(audio_headset_ull_ctrl.play_en.bt_clock,
                                    audio_headset_ull_ctrl.play_en.sequence_number,
                                    &audio_headset_ull_ctrl.play_en.bt_clock,
                                    (U8 *)&audio_headset_ull_ctrl.play_en.sequence_number);
        a2dp_ull_set_init_seq_no(audio_headset_ull_ctrl.play_en.sequence_number);

        MCE_TransBT2NativeClk(audio_headset_ull_ctrl.play_en.bt_clock, 625/*625 1 bt clk,22-1+1 */, &bt_clk, &bt_phase, ULL_CLK_Offset);

        hal_audio_afe_set_play_en(bt_clk, bt_phase);
        DSP_MW_LOG_I("[AiroGaming]AFE Set Val: bt_clk:0x%x, bt_phase:0x%x", 2, bt_clk, bt_phase);
#ifdef TRIGGER_A2DP_PROCSESS_TIMER
        U32 bt_clk_cur;
        U16 intra_clk_cur;

        if (g_play_en_gpt_handler == NULL) {
            if (hal_gpt_sw_get_timer((uint32_t *)&g_play_en_gpt_handler) != HAL_GPT_STATUS_OK) {
                DSP_MW_LOG_W("[AiroGaming]play en get timer fail", 0);
                AUDIO_ASSERT(0);
            } else {
                DSP_MW_LOG_I("[AiroGaming]play en get timer success", 0);
            }
        }
        MCE_GetBtClk((BTCLK *)&bt_clk_cur, (BTPHASE *)&intra_clk_cur, ULL_CLK_Offset);
        U32 FirstDecodeCnt = ((((audio_headset_ull_ctrl.play_en.bt_clock) - (bt_clk_cur)) >> 1) * 625 - (intra_clk_cur >> 1) - 625);
        hal_gpt_sw_start_timer_us(g_play_en_gpt_handler, FirstDecodeCnt, trigger_a2dp_airoha_proc_timer_callback, NULL);
        DSP_MW_LOG_I("[AiroGaming]bclk_play_en:0x%x, bclk_cur:0x%x, timer_duration:%d us", 3, audio_headset_ull_ctrl.play_en.bt_clock, bt_clk_cur, FirstDecodeCnt);

        /*        hal_gpio_init(HAL_GPIO_9);                    //debug for play en decode 1st frame
                hal_pinmux_set_function(HAL_GPIO_9, 0);
                hal_gpio_set_direction(HAL_GPIO_9, HAL_GPIO_DIRECTION_OUTPUT);
                hal_gpio_set_output(HAL_GPIO_9, 0);*/

#endif
    } else {
        DSP_MW_LOG_I("A2DP DL - A2DP Play En First", 0);
    }

    audio_headset_ull_ctrl.is_play_en_ready = TRUE;

    UNUSED(ack);
}
#endif

void dsp_set_sysram_addr(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("DSP NVDM init\r\n", 0);

    U32 sysram_start_addr;

    sysram_start_addr = hal_memview_cm4_to_dsp0(msg.ccni_message[1]);
    dsp0_nvkey_init(sysram_start_addr, NAT_MEM_Size);// SysRam fixed 3k size
}

void dsp_streaming_deinit_all(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    bool enable = (bool)(uint16_t)msg.ccni_message[1];
    audio_stream_deinit_id_t stream_deinit = (audio_stream_deinit_id_t)(msg.ccni_message[0] & 0xFFFF);

    DSP_MW_LOG_I("DSP stream deinit,mute status :%d, ID:%d \r\n", 2, enable, stream_deinit);

    if (stream_deinit == AUDIO_STRAM_DEINIT_VOICE_AEC) {
#ifdef AIR_VOICE_NR_ENABLE
        stream_function_aec_nr_deinitialize(true);
#endif
    } else if (stream_deinit == AUDIO_STRAM_DEINIT_ANC_MONITOR) {
#ifdef AIR_ANC_WIND_DETECTION_ENABLE
        stream_function_wind_reset_status();
#endif
#ifdef AIR_ANC_ENVIRONMENT_DETECTION_ENABLE
        stream_function_environment_detection_reset_status();
#endif
    } else {
        DAVT_StreeamingDeinitAll();
        DHPT_StreeamingDeinitAll();
        #if defined(AIR_ADVANCED_PASSTHROUGH_ENABLE)
        DLLT_StreeamingDeinitAll();
        #endif
        //hal_audio_mute_stream_out(enable);
#ifdef AIR_VOICE_NR_ENABLE
        stream_function_aec_nr_deinitialize(false);
#ifdef AIR_3RD_PARTY_NR_ENABLE
        stream_function_tx_eq_deinitialize(false);
#endif
#endif
#if defined(AIR_DRC_ENABLE) || defined(AIR_VOICE_DRC_ENABLE)
        stream_function_cpd_deinitialize();
#endif
#ifdef MTK_LINEIN_INS_ENABLE
        stream_function_ins_deinitialize();
#endif
    }
}

#ifdef AIR_VOICE_NR_ENABLE
void CB_CM4_SCO_DL_AVC_VOL_UPDATE(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    voice_avc_vol_receive_update(msg.ccni_message[1]);
}

void dsp_get_reference_gain(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("DSP get reference gain\r\n", 0);
    S16 *Refgain_start_addr;
    Refgain_start_addr = (S16 *)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);
    voice_nr_get_ref_gain(Refgain_start_addr);
}

#ifdef AIR_AIRDUMP_ENABLE
void CB_CM4_SCO_AIRDUMP_EN(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    U32 mask;
    hal_nvic_save_and_set_interrupt_mask(&mask);
    if (msg.ccni_message[1] == 1) {
        DSP_MW_LOG_I("[AirDump][DSP] AIRDUMP Start\r\n", 0);
        voice_nr_airdump_on_off(TRUE);
    } else if (msg.ccni_message[1] == 0) {
        DSP_MW_LOG_I("[AirDump][DSP] AIRDUMP Stop\r\n", 0);
        voice_nr_airdump_on_off(FALSE);
    }
    hal_nvic_restore_interrupt_mask(mask);
}
#endif

#endif

typedef struct {
    uint32_t type;
    uint32_t length;
    uint8_t version[32];
} race_dsprealtime_query_lib_version_noti_struct_t;

void dsp_query_lib_version(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    race_dsprealtime_query_lib_version_noti_struct_t *ver_struct;

    UNUSED(ack);

    ver_struct = (race_dsprealtime_query_lib_version_noti_struct_t *)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);
    if (ver_struct->type == DSP_ALG_LIB_TYPE_IGO) {
#ifdef AIR_VOICE_NR_ENABLE
        ver_struct->length = voice_nr_query_version(ver_struct->type, ver_struct->version);
#else
        ver_struct->length = 0;
#endif
    } else {
        ver_struct->length = 0;
        DSP_MW_LOG_I("DSP query lib version fail, type %d\r\n", 1, ver_struct->type);
        return;
    }

    DSP_MW_LOG_I("DSP query lib version success\r\n", 0);
}

void dsp_set_algorithm_param(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    void *share_ptr = (void *)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);
    uint16_t nvkey_id = (uint16_t)(msg.ccni_message[0] & 0xFFFF);
    DSP_MW_LOG_I("DSP set algorithm parameters nvkey_ID:0x%x, ptr:0x%x", 2, nvkey_id, share_ptr);

    if (0) {
#ifdef AIR_ANC_WIND_DETECTION_ENABLE
    } else if (nvkey_id == NVID_DSP_ALG_ANC_WIND_DET) {
        stream_function_wind_load_nvkey(share_ptr);
#endif
#ifdef AIR_ANC_USER_UNAWARE_ENABLE
    } else if (nvkey_id == NVID_DSP_ALG_ANC_USR_UNAWARE) {
        stream_function_user_unaware_load_nvkey(share_ptr);
#endif
#ifdef MTK_USER_TRIGGER_FF_ENABLE
    } else if (nvkey_id == NVKEY_DSP_PARA_ADAPTIVE_FF) {
        stream_function_user_trigger_ff_load_nvkey(share_ptr);
#endif
#ifdef AIR_ANC_ENVIRONMENT_DETECTION_ENABLE
    } else if (nvkey_id == NVID_DSP_ALG_ANC_ENVIRONMENT_DETECTION) {
        stream_function_environment_detection_load_nvkey(share_ptr);
#endif
#ifdef AIR_ADVANCED_PASSTHROUGH_ENABLE
    } else if ((nvkey_id >= NVKEY_DSP_PARA_AFC) && (nvkey_id <= NVKEY_DSP_PARA_ADVANCED_PASSTHROUGH_MISC)){
        advanced_passthrough_hearing_aid_features_set_param(nvkey_id, share_ptr);
#endif
    } else {
        DSP_MW_LOG_E("DSP set algorithm parameters nvkey id is unsupported 0x%x", 1, nvkey_id);
    }
    UNUSED(ack);
}

#ifdef AIR_ANC_USER_UNAWARE_ENABLE
audio_extend_gain_control_t g_audio_extend_gain_ctrl[AUDIO_ANC_CONTROL_EXTEND_RAMP_TYPE_NUM];
#endif
void dsp_anc_monitor_extend_gain_ctrl_init(void)
{
#ifdef AIR_ANC_USER_UNAWARE_ENABLE
    memset(g_audio_extend_gain_ctrl, 0, sizeof(g_audio_extend_gain_ctrl));
#endif
}

void dsp_anc_monitor_extend_gain_ctrl_set(audio_extend_gain_control_ptr_t gain_control)
{
#ifdef AIR_ANC_USER_UNAWARE_ENABLE
    if ((gain_control->gain_type < AUDIO_ANC_CONTROL_EXTEND_RAMP_TYPE_NUM)
        && (gain_control->gain_type != AUDIO_ANC_CONTROL_EXTEND_RAMP_TYPE_USER_UNAWARE)) {
        memcpy(&g_audio_extend_gain_ctrl[gain_control->gain_type], gain_control, sizeof(audio_extend_gain_control_t));
    }
#else
    UNUSED(gain_control);
#endif
}

void dsp_anc_monitor_extend_gain_ctrl_get_total(int16_t *get_total_gain0, int16_t *get_total_gain1)
{
#ifdef AIR_ANC_USER_UNAWARE_ENABLE
    int16_t total_gain[2] = {0, 0};
    for (int i = 0; i < AUDIO_ANC_CONTROL_EXTEND_RAMP_TYPE_NUM; i++) {
        if (i == AUDIO_ANC_CONTROL_EXTEND_RAMP_TYPE_USER_UNAWARE) {
            continue;
        }
        total_gain[0] += g_audio_extend_gain_ctrl[i].gain[0];
        total_gain[1] += g_audio_extend_gain_ctrl[i].gain[1];
    }
    *get_total_gain0 = total_gain[0];
    *get_total_gain1 = total_gain[1];
#else
    UNUSED(get_total_gain0);
    UNUSED(get_total_gain1);
#endif
}

void dsp_ccni_set_extend_gain(audio_extend_gain_control_ptr_t gain_control)
{
    hal_ccni_message_t ccni_msg;
    memset((void *)&ccni_msg, 0, sizeof(hal_ccni_message_t));
    ccni_msg.ccni_message[0] = ((MSG_DSP2MCU_COMMON_AUDIO_ANC_ADAPTIVE << 16) |
                                (gain_control->gain_type & 0xFF) |
                                (((U16)gain_control->misc) << 8));
    ccni_msg.ccni_message[1] = (((gain_control->gain[1] & 0xFFFF) << 16) |
                                (gain_control->gain[0] & 0xFFFF));
    aud_msg_tx_handler(ccni_msg, 0, FALSE);

    dsp_anc_monitor_extend_gain_ctrl_set(gain_control);
}

void dsp_set_adaptive_anc(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    audio_anc_monitor_set_info_t set_type;
    uint32_t data;
    set_type = (audio_anc_monitor_set_info_t)(msg.ccni_message[0] & 0xFFFF);
    data = msg.ccni_message[1];
    UNUSED(ack);
    if (0) {
#ifdef AIR_ANC_USER_UNAWARE_ENABLE
    } else if (set_type < AUDIO_ANC_MONITOR_SET_USER_UNAWARE_MAX) {
        stream_function_user_unaware_inform_handler(set_type, data);
#endif
#ifdef AIR_ANC_ENVIRONMENT_DETECTION_ENABLE
    } else if (set_type < AUDIO_ANC_MONITOR_SET_ENVIRONMENT_DETECTION_MAX) {
        stream_function_environment_detection_set_value(set_type, data);
#endif
#ifdef AIR_ANC_WIND_DETECTION_ENABLE
    } else if (set_type < AUDIO_ANC_MONITOR_SET_WIND_DETECT_MAX) {
        stream_function_wind_detection_set_value(set_type, data);
#endif
    } else {
        DSP_MW_LOG_I("DSP set adaptive anc, not support type:0x%x", 1, set_type);
    }
}

void dsp_get_adaptive_anc(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    audio_anc_monitor_get_info_t get_type;
    uint32_t *data_addr;

    get_type = (audio_anc_monitor_get_info_t)(msg.ccni_message[0] & 0xFFFF);
    data_addr = (uint32_t *)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);
    if (0) {
#ifdef AIR_ANC_USER_UNAWARE_ENABLE
    } else if (get_type == AUDIO_ANC_MONITOR_GET_USER_UNAWARE_GAIN_INFO) {
        stream_function_user_unaware_get_info((int32_t *)data_addr);
    } else if (get_type == AUDIO_ANC_MONITOR_GET_USER_UNAWARE_ENABLE_STAT) {
        *data_addr = (uint32_t)stream_function_user_unaware_get_enable_state();
#endif
#ifdef AIR_ANC_ENVIRONMENT_DETECTION_ENABLE
    } else if (get_type == AUDIO_ANC_MONITOR_GET_ENVIRONMENT_DETECTION_STATIONARY_NOISE) {
        *data_addr = (uint32_t)stream_function_environment_detection_get_value(get_type);
#endif
    } else {
        DSP_MW_LOG_I("DSP get adaptive anc, not support type:0x%x", 1, get_type);
    }
}


#ifdef AIR_ANC_USER_UNAWARE_ENABLE
const uint32_t gUser_Unaware_en = 0x00270027;
void dsp_get_user_unaware_info(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    uint16_t type = (uint16_t)(msg.ccni_message[0] & 0xFFFF);
    S32 *UU_info_start_addr;
    UU_info_start_addr = (S32 *)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);
    if (type == 0) {
        stream_function_user_unaware_get_info(UU_info_start_addr);
    } else if (type == 1) {
        *UU_info_start_addr = (U32)stream_function_user_unaware_get_enable_state();
    }
}
#else
const uint32_t gUser_Unaware_en = 0;

#endif

#ifdef MTK_PROMPT_SOUND_ENABLE
uint32_t gDSP_VP_GPT_TIMER_HANDLE = 0;
uint32_t gDSP_VP_POLLING_TIME = 600; //us
uint32_t gDSP_VP_GPT_TARGET_TIME = 0;
void CB_CM4_VP_PLAYBACK_GPT_CALLBACK(void *user_data)
{
    UNUSED(user_data);
    uint32_t savedmask;
    uint32_t curr_cnt = 0;
    hal_nvic_save_and_set_interrupt_mask(&savedmask);
    //hal_gpio_set_output(HAL_GPIO_2, 0);
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &curr_cnt);
    //DSP_MW_LOG_I("[VPC][SYNC]Enter VP DSP polling!!, curr_cnt = %d.\n", 1, curr_cnt);
    if (gDSP_VP_GPT_TARGET_TIME > curr_cnt) {
        while (1) {
            hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &curr_cnt);
            //DSP_MW_LOG_I("[VPC][SYNC]polling!!, curr_cnt = %d, Target_cnt = %d.\n", 2, curr_cnt, gDSP_VP_GPT_TARGET_TIME);
            if (curr_cnt >= gDSP_VP_GPT_TARGET_TIME) {
                break;
            }
        }
        *((volatile uint32_t *)(0xC0000010)) |= 0x00000004; //Patch test
        //hal_gpio_set_output(HAL_GPIO_2, 1);
#ifndef HAL_AUDIO_ENABLE_PATH_MEM_DEVICE
        audio_digital_block_t memory_block;
        memory_block = hal_audio_afe_get_memory_digital_block(playback_vp_if.sink->param.audio.memory, true);
        //afe_enable_audio_irq(afe_irq_request_number(memory_block), playback_vp_if.sink->param.audio.rate, playback_vp_if.sink->param.audio.count);
        afe_set_memory_path_enable(memory_block, !(playback_vp_if.sink->param.audio.AfeBlkControl.u4awsflag), true);
#else
        hal_audio_trigger_start_parameter_t sw_trigger_start;
        sw_trigger_start.enable = true;
        sw_trigger_start.memory_select = playback_vp_if.sink->param.audio.mem_handle.memory_select;
        hal_audio_set_value((hal_audio_set_value_parameter_t *)&sw_trigger_start, HAL_AUDIO_SET_TRIGGER_MEMORY_START);
#endif
        //hal_gpio_set_output(HAL_GPIO_2, 0);
    } else {
        if ((curr_cnt - gDSP_VP_GPT_TARGET_TIME) > 0x80000000) {
            //gDSP_VP_GPT_TARGET_TIME overflow
            while (1) {
                hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &curr_cnt);
                //DSP_MW_LOG_I("[VPC][SYNC]polling!!, curr_cnt = %d, Target_cnt = %d.\n", 2, curr_cnt, gDSP_VP_GPT_TARGET_TIME);
                if (curr_cnt >= gDSP_VP_GPT_TARGET_TIME) {
                    if ((curr_cnt & 0x80000000) == 0x0) {
                        break;
                    }
                }
            }
            *((volatile uint32_t *)(0xC0000010)) |= 0x00000004; //Patch test
            //hal_gpio_set_output(HAL_GPIO_2, 1);
#ifndef HAL_AUDIO_ENABLE_PATH_MEM_DEVICE
            audio_digital_block_t memory_block;
            memory_block = hal_audio_afe_get_memory_digital_block(playback_vp_if.sink->param.audio.memory, true);
            //afe_enable_audio_irq(afe_irq_request_number(memory_block), playback_vp_if.sink->param.audio.rate, playback_vp_if.sink->param.audio.count);
            afe_set_memory_path_enable(memory_block, !(playback_vp_if.sink->param.audio.AfeBlkControl.u4awsflag), true);
#else
            hal_audio_trigger_start_parameter_t sw_trigger_start;
            sw_trigger_start.enable = true;
            sw_trigger_start.memory_select = playback_vp_if.sink->param.audio.mem_handle.memory_select;
            hal_audio_set_value((hal_audio_set_value_parameter_t *)&sw_trigger_start, HAL_AUDIO_SET_TRIGGER_MEMORY_START);
#endif
            //hal_gpio_set_output(HAL_GPIO_2, 0);
        } else {
            //hal_gpio_set_output(HAL_GPIO_2, 1);
            *((volatile uint32_t *)(0xC0000010)) |= 0x00000004;
#ifndef HAL_AUDIO_ENABLE_PATH_MEM_DEVICE
            audio_digital_block_t memory_block;
            memory_block = hal_audio_afe_get_memory_digital_block(playback_vp_if.sink->param.audio.memory, true);
            //afe_enable_audio_irq(afe_irq_request_number(memory_block), playback_vp_if.sink->param.audio.rate, playback_vp_if.sink->param.audio.count);
            afe_set_memory_path_enable(memory_block, !(playback_vp_if.sink->param.audio.AfeBlkControl.u4awsflag), true);
#else
            hal_audio_trigger_start_parameter_t sw_trigger_start;
            sw_trigger_start.enable = true;
            sw_trigger_start.memory_select = playback_vp_if.sink->param.audio.mem_handle.memory_select;
            hal_audio_set_value((hal_audio_set_value_parameter_t *)&sw_trigger_start, HAL_AUDIO_SET_TRIGGER_MEMORY_START);
#endif
            //hal_gpio_set_output(HAL_GPIO_2, 0);
            //hal_nvic_restore_interrupt_mask(savedmask);
            DSP_MW_LOG_E("[CM4_VP_PB][ERROR] polling callback t(%d) < c(%d) cnt error", 2, gDSP_VP_GPT_TARGET_TIME, curr_cnt);
            //platform_assert("vp sync polling callback target < current GPT count",__FILE__,__LINE__);
        }
    }
    hal_nvic_restore_interrupt_mask(savedmask);

    hal_gpt_status_t gpt_status = hal_gpt_sw_free_timer(gDSP_VP_GPT_TIMER_HANDLE);
    if (HAL_GPT_STATUS_OK != gpt_status) {
        DSP_MW_LOG_I("[VPC][SYNC]VP free one_shot Timer error!!, error id = %d.\n", 1, gpt_status);
    }
    DSP_MW_LOG_I("[VPC][SYNC]Polling end!!, curr_cnt = %d,target_cnt = %d.\n", 2, curr_cnt, gDSP_VP_GPT_TARGET_TIME);
    gDSP_VP_GPT_TIMER_HANDLE = 0;
    gDSP_VP_GPT_TARGET_TIME = 0;
    dsp_audio_msg_ack(MSG_MCU2DSP_VP_SYNC_REQUEST << 8 | SUBMSG_MCU2DSP_SYNC_START, true); // replace with volume sync mechanism
}

/* CM4 VP Playback CCNI callback function */
volatile uint32_t vp_config_flag = 0;
void CB_CM4_VP_PLAYBACK_OPEN(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("[CM4_VP_PB] Open", 0);
    vp_config_flag = 0;

    /* remap to non-cacheable address */
    mcu2dsp_open_param_p open_param;
    open_param = (mcu2dsp_open_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);

#ifdef ENABLE_HWSRC_CLKSKEW
#ifdef AIR_HWSRC_TRACKING_ENABLE
    open_param->stream_out_param.afe.clkskew_mode = CLK_SKEW_V1;
#endif
#endif

    DSP_REMAP_SHARE_INFO(open_param->stream_in_param.playback.p_share_info, n9_dsp_share_info_ptr);

    playback_vp_if.source = dsp_open_stream_in(open_param);//StreamCM4VPPlaybackSource(share_info);
    playback_vp_if.sink   = dsp_open_stream_out(open_param);//StreamAudioAfe2Sink(AUDIO_HARDWARE_PCM, INSTANCE_A, AUDIO_CHANNEL_A_AND_B);
    playback_vp_if.transform = NULL;
    playback_vp_if.pfeature_table = stream_feature_list_prompt;

    DSP_Callback_PreloaderConfig(playback_vp_if.pfeature_table, open_param->audio_scenario_type);
    //stream_feature_configure_type(stream_feature_list_prompt, CODEC_PCM_COPY, CONFIG_DECODER);
}

void CB_CM4_VP_PLAYBACK_START(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("[CM4_VP_PB] Start", 0);
    bool time_out_flag  = 0;
    uint32_t curr_cnt   = 0;
    uint32_t target_cnt = 0;
    uint32_t time_out   = 0;
    uint32_t savedmask;
    hal_gpt_status_t gpt_status;
    mcu2dsp_start_param_p start_param;
    start_param = (mcu2dsp_start_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);

    dsp_start_stream_in(start_param, playback_vp_if.source);
    dsp_start_stream_out(start_param, playback_vp_if.sink);

    playback_vp_if.transform = TrasformAudio2Audio(playback_vp_if.source, playback_vp_if.sink, stream_feature_list_prompt);
    if (playback_vp_if.transform == NULL)
    {
        DSP_MW_LOG_E("[CM4_VP_PB] transform failed", 0);
    }
    hal_nvic_save_and_set_interrupt_mask(&savedmask);
    if (playback_vp_if.sink->param.audio.aws_sync_request) {
        //hal_gpio_set_output(HAL_GPIO_2, 1);
        hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &curr_cnt);
        target_cnt = playback_vp_if.sink->param.audio.aws_sync_time;
        if (target_cnt < curr_cnt) {
            if ((curr_cnt - target_cnt) > 0x80000000) {
                //target_cnt overflow
                time_out = (0xFFFFFFFF - curr_cnt) + target_cnt - gDSP_VP_POLLING_TIME;
            } else {
                DSP_MW_LOG_E("[CM4_VP_PB][ERROR] t(%d) < c(%d) cnt error", 2, target_cnt, curr_cnt);
                //platform_assert("vp sync target < current GPT count",__FILE__,__LINE__);
                time_out_flag = true;
            }
        } else if ((target_cnt - curr_cnt) < 600) {
            DSP_MW_LOG_E("[CM4_VP_PB][ERROR] t(%d) - c(%d) < 600us cnt error", 2, target_cnt, curr_cnt);
            //platform_assert("vp sync (target-current) < gDSP_VP_POLLING_TIME",__FILE__,__LINE__);
            time_out_flag = true;
        } else {
            time_out = (target_cnt - curr_cnt) - gDSP_VP_POLLING_TIME;
        }
        if (!time_out_flag) {
            gDSP_VP_GPT_TARGET_TIME = target_cnt;
            if (gDSP_VP_GPT_TIMER_HANDLE == 0) {
                gpt_status = hal_gpt_sw_get_timer(&gDSP_VP_GPT_TIMER_HANDLE);
                if (HAL_GPT_STATUS_OK != gpt_status) {
                    DSP_MW_LOG_I("[VPC][SYNC]VP get one_shot Timer error!!, error id = %d.\n", 1, gpt_status);
                }
            }
            //hal_gpio_set_output(HAL_GPIO_2, 0);
            gpt_status = hal_gpt_sw_start_timer_us(gDSP_VP_GPT_TIMER_HANDLE, time_out, CB_CM4_VP_PLAYBACK_GPT_CALLBACK, NULL);
            //hal_gpio_set_output(HAL_GPIO_2, 1);
            if (HAL_GPT_STATUS_OK != gpt_status) {
                DSP_MW_LOG_I("[VPC][SYNC]VP start one_shot Timer error!!, error id = %d.\n", 1, gpt_status);
            }
        } else {
            if (afe.dl2_enable) {
                *((volatile uint32_t *)(0xC0000010)) |= 0x00000004;
#ifndef HAL_AUDIO_ENABLE_PATH_MEM_DEVICE
                audio_digital_block_t memory_block;
                memory_block = hal_audio_afe_get_memory_digital_block(playback_vp_if.sink->param.audio.memory, true);
                //afe_enable_audio_irq(afe_irq_request_number(memory_block), playback_vp_if.sink->param.audio.rate, playback_vp_if.sink->param.audio.count);
                afe_set_memory_path_enable(memory_block, !(playback_vp_if.sink->param.audio.AfeBlkControl.u4awsflag), true);
#else
                hal_audio_trigger_start_parameter_t sw_trigger_start;
                sw_trigger_start.enable = true;
                sw_trigger_start.memory_select = playback_vp_if.sink->param.audio.mem_handle.memory_select;
                hal_audio_set_value((hal_audio_set_value_parameter_t *)&sw_trigger_start, HAL_AUDIO_SET_TRIGGER_MEMORY_START);
#endif
            } else {
                DSP_MW_LOG_I("[CM4_VP_PB] VP not exit or ready.", 0);
            }
        }
    }
    hal_nvic_restore_interrupt_mask(savedmask);
    if (time_out_flag) {
        xTaskResumeFromISR((TaskHandle_t)pDPR_TaskHandler);
        portYIELD_FROM_ISR(pdTRUE); // force to do context switch
    }
    DSP_MW_LOG_I("[CM4_VP_PB] Start info request(%d), c_cnt(%d), t_cnt(%d), time_out(%d)", 4, playback_vp_if.sink->param.audio.aws_sync_request, curr_cnt, target_cnt, time_out);
}


void CB_CM4_VP_PLAYBACK_STOP(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    uint32_t savedmask;
    hal_nvic_save_and_set_interrupt_mask(&savedmask);
    if (gDSP_VP_GPT_TIMER_HANDLE != 0) {
        hal_gpt_status_t gpt_status = hal_gpt_sw_stop_timer_us(gDSP_VP_GPT_TIMER_HANDLE);
        if (HAL_GPT_STATUS_OK != gpt_status) {
            DSP_MW_LOG_I("[VPC][SYNC]VP stop one_shot Timer error!!, error id = %d.\n", 1, gpt_status);
        }
        gpt_status = hal_gpt_sw_free_timer(gDSP_VP_GPT_TIMER_HANDLE);
        if (HAL_GPT_STATUS_OK != gpt_status) {
            DSP_MW_LOG_I("[VPC][SYNC]VP free2 one_shot Timer error!!, error id = %d.\n", 1, gpt_status);
        }
        gDSP_VP_GPT_TIMER_HANDLE = 0;
    }
    hal_nvic_restore_interrupt_mask(savedmask);
    DSP_MW_LOG_I("[CM4_VP_PB] Stop", 0);
    if (playback_vp_if.transform != NULL) {
        StreamDSPClose(playback_vp_if.transform->source, playback_vp_if.transform->sink, msg.ccni_message[0] >> 16 | 0x8000);
    }
    playback_vp_if.transform = NULL;
}

void CB_CM4_VP_PLAYBACK_CONFIG(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    uint16_t vp_stop_immediately = hal_memview_cm4_to_dsp0(msg.ccni_message[0]);
    UNUSED(ack);
    /*
      Todo:
      Notify DSP framework to push remained pcm data into AFE.
    */

    /* Set flag to notify APP to stop VP */
    if (playback_vp_if.transform != NULL) {
#if 0 //VP log slim
        DSP_MW_LOG_I("[CM4_VP_PB] Config", 0);
#endif
        vp_config_flag = 1;
        if (vp_stop_immediately == 0x55) {
            // clear audio buffer
            DSP_CALLBACK_PTR callback_ptr = DSP_Callback_Get(playback_vp_if.source, playback_vp_if.sink);
            memset(callback_ptr->EntryPara.in_ptr[0], 0, callback_ptr->EntryPara.in_malloc_size);
            memset(callback_ptr->EntryPara.out_ptr[0], 0, callback_ptr->EntryPara.out_malloc_size);
            // Clear SRC IN
            memset((uint32_t *)(playback_vp_if.sink->param.audio.AfeBlkControl.phys_buffer_addr + playback_vp_if.sink->param.audio.AfeBlkControl.u4asrc_buffer_size), 0, AFE_INTERNAL_SRAM_VP_SIZE - playback_vp_if.sink->param.audio.AfeBlkControl.u4asrc_buffer_size);
            // Clear SRC OUT
            memset((uint32_t *)playback_vp_if.sink->param.audio.AfeBlkControl.phys_buffer_addr, 0, playback_vp_if.sink->param.audio.AfeBlkControl.u4asrc_buffer_size);
            DSP_MW_LOG_I("[CM4_VP_PB] config stop immediately", 0);
        }
    } else {
        DSP_MW_LOG_I("[CM4_VP_PB] Config duplicate.", 0);
    }
}

void CB_CM4_VP_PLAYBACK_CLOSE(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("[CM4_VP_PB] Close", 0);
    DSP_Callback_UnloaderConfig(playback_vp_if.pfeature_table, playback_vp_if.source->scenario_type);
    SourceClose(playback_vp_if.source);
    SinkClose(playback_vp_if.sink);
    memset(&playback_vp_if,0,sizeof(CONNECTION_IF));
}

void CB_CM4_VP_PLAYBACK_TRIGGER(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("[CM4_VP_PB] Trigger Just return", 0);
    return;
#if 0
    if (afe.dl2_enable) {
        //DSP_MW_LOG_I("[CM4_VP_PB] VP trigger memory irq enable", 0);
#ifndef HAL_AUDIO_ENABLE_PATH_MEM_DEVICE
        audio_digital_block_t memory_block;
        memory_block = hal_audio_afe_get_memory_digital_block(playback_vp_if.sink->param.audio.memory, true);
        afe_enable_audio_irq(afe_irq_request_number(memory_block), playback_vp_if.sink->param.audio.rate, playback_vp_if.sink->param.audio.count);
        afe_set_memory_path_enable(memory_block, !(playback_vp_if.sink->param.audio.AfeBlkControl.u4awsflag), true);
#else
        hal_audio_trigger_start_parameter_t sw_trigger_start;
        sw_trigger_start.enable = true;
        sw_trigger_start.memory_select = playback_vp_if.sink->param.audio.mem_handle.memory_select;
        hal_audio_set_value((hal_audio_set_value_parameter_t *)&sw_trigger_start, HAL_AUDIO_SET_TRIGGER_MEMORY_START);
#endif
        xTaskResumeFromISR((TaskHandle_t)pDPR_TaskHandler);
        portYIELD_FROM_ISR(pdTRUE); // force to do context switch
    } else {
        DSP_MW_LOG_I("[CM4_VP_PB] VP not exit or ready.", 0);
    }
#endif
}
#endif

#ifdef MTK_PROMPT_SOUND_DUMMY_SOURCE_ENABLE
/* VP_DUMMY Playback CCNI callback function */
extern n9_dsp_share_info_t DUMMY_buff_info;
void CB_CM4_VP_DUMMY_SOURCE_PLAYBACK_OPEN(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("[CM4_VP_DUMMY] Open", 0);

    /* remap to non-cacheable address */
    mcu2dsp_open_param_p open_param;
    open_param = (mcu2dsp_open_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);
    //DSP_REMAP_SHARE_INFO(open_param->stream_in_param.playback.share_info_base_addr);
    open_param->stream_in_param.playback.share_info_base_addr = &DUMMY_buff_info;
    playback_vp_dummy_source_if.source = dsp_open_stream_in(open_param);
    playback_vp_dummy_source_if.sink   = dsp_open_stream_out(open_param);
    playback_vp_dummy_source_if.transform = NULL;
    DSP_Callback_PreloaderConfig(playback_vp_dummy_source_if.pfeature_table, open_param->audio_scenario_type);
}

void CB_CM4_VP_DUMMY_SOURCE_PLAYBACK_START(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("[CM4_VP_DUMMY] Start", 0);
    mcu2dsp_start_param_p start_param;
    start_param = (mcu2dsp_start_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);

    dsp_start_stream_in(start_param, playback_vp_dummy_source_if.source);
    dsp_start_stream_out(start_param, playback_vp_dummy_source_if.sink);
    //Rdebug- Needcheck
    //DSP_ConfigFeatureListCodecResolution(playback_vp_dummy_source_if.pfeature_table, Res_32bit, CONFIG_DECODER); /*Set Feature Codec 32 Resolution.*/

    playback_vp_dummy_source_if.transform = TrasformAudio2Audio(playback_vp_dummy_source_if.source, playback_vp_dummy_source_if.sink, stream_feature_list_prompt_dummy_source);
    if (playback_vp_dummy_source_if.transform == NULL) {
        DSP_MW_LOG_I("[CM4_VP_DUMMY] transform failed", 0);
    }
}


void CB_CM4_VP_DUMMY_SOURCE_PLAYBACK_STOP(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("[CM4_VP_DUMMY] Stop", 0);
    if (playback_vp_dummy_source_if.transform != NULL) {
        StreamDSPClose(playback_vp_dummy_source_if.transform->source, playback_vp_dummy_source_if.transform->sink, msg.ccni_message[0] >> 16 | 0x8000);
    }
    playback_vp_dummy_source_if.transform = NULL;
}

void CB_CM4_VP_DUMMY_SOURCE_PLAYBACK_CLOSE(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);
    DSP_MW_LOG_I("[CM4_VP_DUMMY] Close", 0);
    DSP_Callback_UnloaderConfig(playback_vp_dummy_source_if.pfeature_table, playback_vp_dummy_source_if.source->scenario_type);
    SourceClose(playback_vp_dummy_source_if.source);
    SinkClose(playback_vp_dummy_source_if.sink);
    memset(&playback_vp_dummy_source_if,0,sizeof(CONNECTION_IF));
}

extern void cm4_vp_dummy_source_playback_set_param(uint8_t mode, uint8_t index);
void CB_CM4_VP_DUMMY_SOURCE_PLAYBACK_CHANGE_FEATURE(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    DSP_MW_LOG_I("[CM4_VP_DUMMY] Change feature", 0);
    uint8_t mode = (uint8_t)msg.ccni_message[1];
    uint8_t index = (uint8_t)(msg.ccni_message[1] >> 16);
    cm4_vp_dummy_source_playback_set_param(mode, index);
}
#endif

#ifdef AIR_BT_CLK_SKEW_ENABLE
void CB_N9_CLK_SKEW_LAG(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    UNUSED(msg);
    //reserve
}

void CB_N9_CLK_SKEW_LEAD(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    UNUSED(msg);
    //reserve
}
#endif

void CB_CM4_AUDIO_AMP_FORCE_CLOSE(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    UNUSED(msg);
    DSP_MW_LOG_I("[CM4_AUDIO] AMP Force Close", 0);
#ifdef ENABLE_AMP_TIMER
    fw_amp_force_close();
#endif
}

#ifdef MTK_SENSOR_SOURCE_ENABLE
#include "bsp_multi_axis_sensor.h"
extern bool bsp_multi_axis_read_register(uint32_t addr, uint8_t *data);
extern bool bsp_multi_axis_write_register(uint32_t addr, uint8_t *data);

void CB_GSENSOR_DETECT_READ_RG(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    uint8_t data = 0;

    ack = ack;

    if (bsp_multi_axis_read_register((uint32_t)msg.ccni_message[0] & 0xffff, &data) == false) {
        log_hal_msgid_error("[G-sensor] CB_CM4_GSENSOR_READ_RG fail", 0);
        return;
    }
    log_hal_msgid_info("[G-sensor]CB_CM4_GSENSOR_READ_RG,addr = 0x%08x,data = 0x%08x", 2, msg.ccni_message[0] & 0xffff, data);
}

void CB_GSENSOR_DETECT_WRITE_RG(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    uint8_t data = 0;

    ack = ack;

    data = (uint8_t)msg.ccni_message[1];
    if (bsp_multi_axis_write_register((uint32_t)msg.ccni_message[0] & 0xffff, &data) == false) {
        log_hal_msgid_error("[G-sensor] CB_CM4_GSENSOR_WRITE_RG fail", 0);
        return;
    }

    log_hal_msgid_info("[G-sensor]CB_CM4_GSENSOR_WRITE_RG,addr = 0x%08x,data = 0x%08x", 2, msg.ccni_message[0] & 0xffff, data);
}
#endif
#ifdef MTK_AUDIO_PLC_ENABLE
void DSP_AUDIO_PLC_CONTROL(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("DSP_AUDIO_PLC_CONTROL\r\n", 0);

    uint32_t enable = (uint32_t)(msg.ccni_message[1]);
    dsp_audio_plc_ctrl_t audio_plc_ctrl;
    audio_plc_ctrl.enable = enable;
    Audio_PLC_ctrl(audio_plc_ctrl);
    DSP_MW_LOG_I("DSP_AUDIO_PLC_CONTROL ENABLE %d\r\n", 1, enable);
}
#endif

#ifdef MTK_DSP_SHUTDOWN_SPECIAL_CONTROL_ENABLE
void DSP_DUMMY_SHUTDOWN(void)
{
    bool ack = false;
    uint32_t savedmask;
    uint16_t count = 1;
    hal_nvic_save_and_set_interrupt_mask(&savedmask);
    //**Special control.
    //**To avoid DSP task HW semaphore when CM4 disable DSP power by using SPM control.
    hal_ccni_message_t msg;
    memset((void *)&msg, 0, sizeof(hal_ccni_message_t));
    msg.ccni_message[0] = 0x804b << 16;
    while (1) {
        count ++;
        if ((count % 1000) == 0) {
            if (!ack) {
                if (hal_ccni_set_event(AUDIO_CM4_TX_EVENT, &msg) == HAL_CCNI_STATUS_OK) { /*Remove all OS API, just call driver.*/
                    ack = true;
                }
            }
        }
    }
}
#endif

#ifdef AIR_AUDIO_TRANSMITTER_ENABLE

#ifdef AIR_MULTI_MIC_STREAM_ENABLE
extern stream_feature_list_t stream_featuremulti_mic_function_a[];
extern stream_feature_list_t stream_featuremulti_mic_function_b[];
extern stream_feature_list_t stream_featuremulti_mic_function_c[];
extern stream_feature_list_t stream_featuremulti_mic_function_f[];

CONNECTION_IF g_multi_mic_streams[AUDIO_TRANSMITTER_MULTI_MIC_STREAM_SUB_ID_MAX] = {
    /* source     sink            transform     pfeature_table */
    {NULL,      NULL,       NULL,        stream_featuremulti_mic_function_a},  //AUDIO_SCENARIO_MULTI_MIC_STREAM_FUNCTION_A
    {NULL,      NULL,       NULL,        stream_featuremulti_mic_function_b},  //AUDIO_SCENARIO_MULTI_MIC_STREAM_FUNCTION_B
    {NULL,      NULL,       NULL,        stream_featuremulti_mic_function_c},  //AUDIO_SCENARIO_MULTI_MIC_STREAM_FUNCTION_C
    {NULL,      NULL,       NULL,        stream_featuremulti_mic_function_f},  //AUDIO_SCENARIO_MULTI_MIC_STREAM_FUNCTION_F
};
#endif

#ifdef MTK_SENSOR_SOURCE_ENABLE
CONNECTION_IF g_sensor_streams = {
    /* source     sink            transform     pfeature_table */
    NULL,        NULL,       NULL,       AudioFeatureList_GSensorMotionDetect
};
#endif

#ifdef MTK_AUDIO_LOOPBACK_TEST_ENABLE
extern stream_feature_list_t stream_feature_list_audio_loopback_test[];
CONNECTION_IF audio_loopback_test_if = {NULL,      NULL,       NULL,     stream_feature_list_audio_loopback_test};
#endif

#if defined(MTK_GAMING_MODE_HEADSET) || defined(AIR_GAMING_MODE_DONGLE_ENABLE)
extern stream_feature_list_t stream_feature_list_usb_in_broadcast_0[];
CONNECTION_IF g_usb_in_broadcast_streams_0 = {
    /* source     sink            transform     pfeature_table */
    NULL,        NULL,       NULL,       stream_feature_list_usb_in_broadcast_0
};

extern stream_feature_list_t stream_feature_list_usb_in_broadcast_1[];
CONNECTION_IF g_usb_in_broadcast_streams_1 = {
    /* source     sink            transform     pfeature_table */
    NULL,        NULL,       NULL,       stream_feature_list_usb_in_broadcast_1
};

extern stream_feature_list_t stream_feature_list_usb_out_broadcast[];
CONNECTION_IF g_usb_out_broadcast_streams = {
    /* source     sink            transform     pfeature_table */
    NULL,        NULL,       NULL,       stream_feature_list_usb_out_broadcast
};

#ifdef AIR_GAMING_MODE_DONGLE_LINE_IN_ENABLE
extern stream_feature_list_t stream_feature_list_game_line_in_broadcast[];
CONNECTION_IF g_gaming_line_in_broadcast_streams = {
    /* source     sink            transform     pfeature_table */
    NULL,        NULL,       NULL,       stream_feature_list_game_line_in_broadcast
};
#endif /* AIR_GAMING_MODE_DONGLE_LINE_IN_ENABLE */

#ifdef AIR_GAMING_MODE_DONGLE_LINE_OUT_ENABLE
extern stream_feature_list_t stream_feature_list_game_line_out_broadcast[];
CONNECTION_IF g_gaming_line_out_broadcast_streams = {
    /* source     sink            transform     pfeature_table */
    NULL,        NULL,       NULL,       stream_feature_list_game_line_out_broadcast
};
#endif /* AIR_GAMING_MODE_DONGLE_LINE_OUT_ENABLE */

#ifdef AIR_GAMING_MODE_DONGLE_I2S_IN_ENABLE
extern stream_feature_list_t stream_feature_list_game_i2s_in_broadcast[];
CONNECTION_IF g_gaming_i2s_in_broadcast_streams = {
    /* source     sink            transform     pfeature_table */
    NULL,        NULL,       NULL,       stream_feature_list_game_i2s_in_broadcast
};
#endif /* AIR_GAMING_MODE_DONGLE_I2S_IN_ENABLE */

#endif /* MTK_GAMING_MODE_HEADSET || AIR_GAMING_MODE_DONGLE_ENABLE */

#if defined(MTK_GAMING_MODE_HEADSET) || defined(AIR_GAMING_MODE_DONGLE_ENABLE)
extern stream_feature_list_t stream_feature_list_game_headset_ul[];
CONNECTION_IF g_game_headset_ul_streams = {
    /* source     sink            transform     pfeature_table */
    NULL,        NULL,       NULL,       stream_feature_list_game_headset_ul
};
#endif /* MTK_GAMING_MODE_HEADSET || AIR_GAMING_MODE_DONGLE_ENABLE */

#ifdef AIR_AUDIO_I2S_SLAVE_TDM_ENABLE
extern stream_feature_list_t stream_feature_list_tdm[];
CONNECTION_IF tdm_if = {NULL,      NULL,       NULL,     stream_feature_list_tdm};
#endif

#if defined(AIR_WIRED_AUDIO_ENABLE)
extern stream_feature_list_t stream_feature_list_usb_in_local_0[];
CONNECTION_IF g_usb_in_local_streams_0 = {
    /* source     sink            transform     pfeature_table */
    NULL,        NULL,       NULL,       stream_feature_list_usb_in_local_0
};
extern stream_feature_list_t stream_feature_list_usb_in_local_1[];
CONNECTION_IF g_usb_in_local_streams_1 = {
    /* source     sink            transform     pfeature_table */
    NULL,        NULL,       NULL,       stream_feature_list_usb_in_local_1
};
extern stream_feature_list_t stream_feature_list_line_in_local[];
CONNECTION_IF g_line_in_local_streams = {
    /* source     sink            transform     pfeature_table */
    NULL,        NULL,       NULL,       stream_feature_list_line_in_local
};
extern stream_feature_list_t stream_feature_list_usb_out_local[];
CONNECTION_IF g_usb_out_local_streams = {
    /* source     sink            transform     pfeature_table */
    NULL,        NULL,       NULL,       stream_feature_list_usb_out_local
};
extern stream_feature_list_t stream_feature_list_line_out_local[];
CONNECTION_IF g_line_out_local_streams = {
    /* source     sink            transform     pfeature_table */
    NULL,        NULL,       NULL,       stream_feature_list_line_out_local
};
#endif /*AIR_WIRED_AUDIO_ENABLE*/

#if defined(AIR_ADVANCED_PASSTHROUGH_ENABLE)
extern stream_feature_list_t stream_feature_list_advanced_passthrough[];
CONNECTION_IF advanced_passthrough_if = {NULL,      NULL,       NULL,     stream_feature_list_advanced_passthrough};
#endif /* AIR_ADVANCED_PASSTHROUGH_ENABLE */

#if defined(AIR_BLE_AUDIO_DONGLE_ENABLE)
extern stream_feature_list_t stream_feature_list_ble_audio_dongle_usb_in_broadcast_0[];
CONNECTION_IF g_ble_audio_dongle_usb_in_broadcast_streams_0 = {
    /* source     sink            transform     pfeature_table */
    NULL,        NULL,       NULL,       stream_feature_list_ble_audio_dongle_usb_in_broadcast_0
};

extern stream_feature_list_t stream_feature_list_ble_audio_dongle_usb_in_broadcast_1[];
CONNECTION_IF g_ble_audio_dongle_usb_in_broadcast_streams_1 = {
    /* source     sink            transform     pfeature_table */
    NULL,        NULL,       NULL,       stream_feature_list_ble_audio_dongle_usb_in_broadcast_1
};

extern stream_feature_list_t stream_feature_list_ble_audio_dongle_usb_out_broadcast[];
CONNECTION_IF g_ble_audio_dongle_usb_out_broadcast_streams = {
    /* source     sink            transform     pfeature_table */
    NULL,        NULL,       NULL,       stream_feature_list_ble_audio_dongle_usb_out_broadcast
};
#endif /* AIR_BLE_AUDIO_DONGLE_ENABLE */

#if defined(AIR_ULL_AUDIO_V2_DONGLE_ENABLE)
extern stream_feature_list_t stream_feature_list_ull_audio_v2_dongle_usb_in_broadcast_0[];
CONNECTION_IF g_ull_audio_v2_dongle_usb_in_broadcast_streams_0 = {
    /* source     sink            transform     pfeature_table */
    NULL,        NULL,       NULL,       stream_feature_list_ull_audio_v2_dongle_usb_in_broadcast_0
};

extern stream_feature_list_t stream_feature_list_ull_audio_v2_dongle_usb_in_broadcast_1[];
CONNECTION_IF g_ull_audio_v2_dongle_usb_in_broadcast_streams_1 = {
    /* source     sink            transform     pfeature_table */
    NULL,        NULL,       NULL,       stream_feature_list_ull_audio_v2_dongle_usb_in_broadcast_1
};

extern stream_feature_list_t stream_feature_list_ull_audio_v2_dongle_usb_out_broadcast[];
CONNECTION_IF g_ull_audio_v2_dongle_usb_out_broadcast_streams = {
    /* source     sink            transform     pfeature_table */
    NULL,        NULL,       NULL,       stream_feature_list_ull_audio_v2_dongle_usb_out_broadcast
};
#endif /* AIR_ULL_AUDIO_V2_DONGLE_ENABLE */

CONNECTION_IF *port_audio_transmitter_get_connection_if(audio_transmitter_scenario_type_t scenario_id, audio_transmitter_scenario_sub_id_t sub_id)
{
    CONNECTION_IF *application_ptr = NULL;

    if (scenario_id == AUDIO_TRANSMITTER_A2DP_SOURCE) {


    }
#ifdef AIR_MULTI_MIC_STREAM_ENABLE
    else if (scenario_id == AUDIO_TRANSMITTER_MULTI_MIC_STREAM) {
        application_ptr = &g_multi_mic_streams[sub_id.multimic_id];
    }
#endif
#ifdef MTK_SENSOR_SOURCE_ENABLE
    else if (scenario_id == AUDIO_TRANSMITTER_GSENSOR) {
        application_ptr = &sensor_src_if[sub_id.gsensor_id];
    }
#endif
#if defined(MTK_GAMING_MODE_HEADSET) || defined(AIR_GAMING_MODE_DONGLE_ENABLE)
    else if (scenario_id == AUDIO_TRANSMITTER_GAMING_MODE) {
        switch (sub_id.gamingmode_id) {
            case AUDIO_TRANSMITTER_GAMING_MODE_MUSIC_DONGLE_USB_IN_0:
            application_ptr = &g_usb_in_broadcast_streams_0;
                break;
            case AUDIO_TRANSMITTER_GAMING_MODE_MUSIC_DONGLE_USB_IN_1:
            application_ptr = &g_usb_in_broadcast_streams_1;
                break;
            case AUDIO_TRANSMITTER_GAMING_MODE_VOICE_DONGLE_USB_OUT:
            application_ptr = &g_usb_out_broadcast_streams;
                break;
            case AUDIO_TRANSMITTER_GAMING_MODE_VOICE_HEADSET:
            application_ptr = &g_game_headset_ul_streams;
                break;
            #ifdef AIR_GAMING_MODE_DONGLE_LINE_IN_ENABLE
                case AUDIO_TRANSMITTER_GAMING_MODE_MUSIC_DONGLE_LINE_IN:
                    application_ptr = &g_gaming_line_in_broadcast_streams;
                    break;
            #endif /* AIR_GAMING_MODE_DONGLE_LINE_IN_ENABLE */
            #ifdef AIR_GAMING_MODE_DONGLE_LINE_OUT_ENABLE
                case AUDIO_TRANSMITTER_GAMING_MODE_VOICE_DONGLE_LINE_OUT:
                    application_ptr = &g_gaming_line_out_broadcast_streams;
                    break;
            #endif /* AIR_GAMING_MODE_DONGLE_LINE_OUT_ENABLE */
            #ifdef AIR_GAMING_MODE_DONGLE_I2S_IN_ENABLE
                case AUDIO_TRANSMITTER_GAMING_MODE_MUSIC_DONGLE_I2S_IN:
                    application_ptr = &g_gaming_i2s_in_broadcast_streams;
                    break;
            #endif
            default:
                DSP_MW_LOG_W("[audio_transmitter] get connection_if not found app_ptr %d-%d", 2, scenario_id, sub_id.gamingmode_id);
                break;
        }
    }
#endif /* MTK_GAMING_MODE_HEADSET || AIR_GAMING_MODE_DONGLE_ENABLE */
#if defined(AIR_BLE_AUDIO_DONGLE_ENABLE)
    else if (scenario_id == AUDIO_TRANSMITTER_BLE_AUDIO_DONGLE) {

        if (sub_id.ble_audio_dongle_id == AUDIO_TRANSMITTER_BLE_AUDIO_DONGLE_MUSIC_USB_IN_0) {
            application_ptr = &g_ble_audio_dongle_usb_in_broadcast_streams_0;
        } else if (sub_id.ble_audio_dongle_id == AUDIO_TRANSMITTER_BLE_AUDIO_DONGLE_MUSIC_USB_IN_1) {
            application_ptr = &g_ble_audio_dongle_usb_in_broadcast_streams_1;
        } else if (sub_id.ble_audio_dongle_id == AUDIO_TRANSMITTER_BLE_AUDIO_DONGLE_VOICE_USB_OUT) {
            application_ptr = &g_ble_audio_dongle_usb_out_broadcast_streams;
        }
    }
#endif /* AIR_BLE_AUDIO_DONGLE_ENABLE */
#if defined(AIR_ULL_AUDIO_V2_DONGLE_ENABLE)
    else if (scenario_id == AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE) {
        if(sub_id.ull_audio_v2_dongle_id == AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0)
        {
            application_ptr = &g_ull_audio_v2_dongle_usb_in_broadcast_streams_0;
        }
        else if(sub_id.ull_audio_v2_dongle_id == AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_1)
        {
            application_ptr = &g_ull_audio_v2_dongle_usb_in_broadcast_streams_1;
        }
        else if(sub_id.ull_audio_v2_dongle_id == AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0)
        {
            application_ptr = &g_ull_audio_v2_dongle_usb_out_broadcast_streams;
        }
    }
#endif /* AIR_ULL_AUDIO_V2_DONGLE_ENABLE */
#if defined(MTK_ANC_SURROUND_MONITOR_ENABLE)
    else if (scenario_id == AUDIO_TRANSMITTER_ANC_MONITOR_STREAM) {
        application_ptr = &sensor_anc_monitor_if;
    }
#endif
    else if (scenario_id == AUDIO_TRANSMITTER_TEST) {
        if (sub_id.test_id == AUDIO_TRANSMITTER_TEST_AUDIO_LOOPBACK) {
#if defined(MTK_AUDIO_LOOPBACK_TEST_ENABLE)
            application_ptr = &audio_loopback_test_if;
            stream_feature_configure_resolution((stream_feature_list_ptr_t)application_ptr->pfeature_table, RESOLUTION_32BIT, CONFIG_DECODER);
#endif
        }
    }
#if defined(AIR_AUDIO_I2S_SLAVE_TDM_ENABLE)
    else if (scenario_id == AUDIO_TRANSMITTER_TDM) {
        application_ptr = &tdm_if;
        stream_feature_configure_resolution((stream_feature_list_ptr_t)application_ptr->pfeature_table, RESOLUTION_32BIT, CONFIG_DECODER);

        if ((tdm_if.source != 0) && (tdm_if.sink != 0)) {
            tdm_if.source->param.audio.linein_scenario_flag = 1;
            tdm_if.sink->param.audio.linein_scenario_flag = 1;
            Source_Audio_BufferInfo_Rst(tdm_if.source, 0);
#ifndef AIR_AUDIO_I2S_SLAVE_TDM_VIRTUAL_OUT_ENABLE
            if (!tdm_if.sink->param.virtual_para.user_count) {
                Sink_Audio_BufferInfo_Rst(tdm_if.sink, 0);
            }
#endif
        }
    }
#endif
#if defined(AIR_WIRED_AUDIO_ENABLE)
    else if (scenario_id == AUDIO_TRANSMITTER_WIRED_AUDIO) {
        //if(sub_id.test_id == AUDIO_TRANSMITTER_WIRED_AUDIO_USB_OUT){
        //    DSP_MW_LOG_E("[audio_transmitter] get connection_if AUDIO_TRANSMITTER_WIRED_AUDIO_USB_OUT", 0);
        //    DSP_ALG_UpdateEscoRxMode(VOICE_WB);
        //    application_ptr = &g_usb_out_local_streams;
        //}
        //else
        if ((sub_id.wiredaudio_id == AUDIO_TRANSMITTER_WIRED_AUDIO_USB_IN_0) || (sub_id.wiredaudio_id == AUDIO_TRANSMITTER_WIRED_AUDIO_USB_IN_1)) {
            if (((sub_id.wiredaudio_id == AUDIO_TRANSMITTER_WIRED_AUDIO_USB_IN_0) && (g_application_ptr_usb_in_0 == NULL))
                || ((sub_id.wiredaudio_id == AUDIO_TRANSMITTER_WIRED_AUDIO_USB_IN_1) && (g_application_ptr_usb_in_1 == NULL))) {
                if (wired_audio_get_usb_audio_start_number() == 0) {
                    DSP_MW_LOG_I("[audio_transmitter] get connection_if AUDIO_TRANSMITTER_WIRED_AUDIO_USB_IN_0", 0);
                    application_ptr = &g_usb_in_local_streams_0;

                } else if (wired_audio_get_usb_audio_start_number() > 0) {
                    DSP_MW_LOG_I("[audio_transmitter] get connection_if AUDIO_TRANSMITTER_WIRED_AUDIO_USB_IN_1", 0);
                    application_ptr = &g_usb_in_local_streams_1;
                }
                if (sub_id.wiredaudio_id == AUDIO_TRANSMITTER_WIRED_AUDIO_USB_IN_0) {
                    g_application_ptr_usb_in_0 = application_ptr;
                } else if (sub_id.wiredaudio_id == AUDIO_TRANSMITTER_WIRED_AUDIO_USB_IN_1) {
                    g_application_ptr_usb_in_1 = application_ptr;
                }
            } else {
                if (sub_id.wiredaudio_id == AUDIO_TRANSMITTER_WIRED_AUDIO_USB_IN_0) {
                    application_ptr = g_application_ptr_usb_in_0;
                } else if (sub_id.wiredaudio_id == AUDIO_TRANSMITTER_WIRED_AUDIO_USB_IN_1) {
                    application_ptr = g_application_ptr_usb_in_1;
                }
            }
        } else if ((sub_id.wiredaudio_id == AUDIO_TRANSMITTER_WIRED_AUDIO_LINE_OUT) || (sub_id.wiredaudio_id == AUDIO_TRANSMITTER_WIRED_AUDIO_USB_OUT)) {
            DSP_MW_LOG_E("[audio_transmitter] get connection_if AUDIO_TRANSMITTER_WIRED_AUDIO_LINE_OUT/AUDIO_TRANSMITTER_WIRED_AUDIO_USB_OUT", 0);
            DSP_ALG_UpdateEscoTxMode(VOICE_WB);
            DSP_ALG_UpdateEscoRxMode(VOICE_WB);
            if (sub_id.wiredaudio_id == AUDIO_TRANSMITTER_WIRED_AUDIO_USB_OUT) {
                application_ptr = &g_usb_out_local_streams;
            } else {
                application_ptr = &g_line_out_local_streams;
            }
        } else if (sub_id.wiredaudio_id == AUDIO_TRANSMITTER_WIRED_AUDIO_LINE_IN) {
            DSP_MW_LOG_E("[audio_transmitter] get connection_if AUDIO_TRANSMITTER_WIRED_AUDIO_LINE_IN", 0);
            application_ptr = &g_line_in_local_streams;
        }
    }
#endif /*AIR_WIRED_AUDIO_ENABLE*/
#if defined(AIR_ADVANCED_PASSTHROUGH_ENABLE)
    else if (scenario_id == AUDIO_TRANSMITTER_ADVANCED_PASSTHROUGH) {
        if (sub_id.advanced_passthrough_id == AUDIO_TRANSMITTER_ADVANCED_PASSTHROUGH_HEARING_AID) {
            application_ptr = &advanced_passthrough_if;
        }
    }
#endif /* AIR_ADVANCED_PASSTHROUGH_ENABLE */
#if defined(AIR_ADAPTIVE_EQ_ENABLE)
    else if (scenario_id == AUDIO_TRANSMITTER_ADAPTIVE_EQ_MONITOR_STREAM){
        application_ptr = &sensor_adaptive_eq_monitor_if;
    }
#endif
#if defined (AIR_DUAL_CHIP_MIXING_MODE_ROLE_MASTER_ENABLE) || defined (AIR_DUAL_CHIP_MIXING_MODE_ROLE_SLAVE_ENABLE)
    else if (scenario_id == AUDIO_TRANSMITTER_DCHS){
        if(sub_id.dchs_id == AUDIO_TRANSMITTER_DCHS_UART_DL){
            application_ptr = &dchs_uart_dl_if;
        }
    }
#endif
    if (!application_ptr) {
        DSP_MW_LOG_E("[audio_transmitter] get connection_if failed, scenario_id %d, sub_id %d", 2, scenario_id, sub_id);
        AUDIO_ASSERT(0);
    }
    return application_ptr;
}

void audio_transmitter_configure_task(CONNECTION_IF *application_ptr, audio_transmitter_scenario_type_t scenario_id, audio_transmitter_scenario_sub_id_t sub_id)
{
    if (0) {
        UNUSED(application_ptr);
        UNUSED(scenario_id);
        UNUSED(sub_id);
#if defined(MTK_ANC_SURROUND_MONITOR_ENABLE) || defined(AIR_ADAPTIVE_EQ_ENABLE)
    } else if ((scenario_id == AUDIO_TRANSMITTER_ANC_MONITOR_STREAM)||(scenario_id == AUDIO_TRANSMITTER_ADAPTIVE_EQ_MONITOR_STREAM)){
        application_ptr->source->taskId = DPR_TASK_ID;
#endif
    }

}

void audio_transmitter_open(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    CONNECTION_IF *application_ptr;
    mcu2dsp_open_param_p open_param;
    audio_transmitter_scenario_type_t scenario_id;
    audio_transmitter_scenario_sub_id_t sub_id;

    UNUSED(ack);

    scenario_id = (msg.ccni_message[0] >> 8) & 0xFF;
    sub_id.scenario_id = (msg.ccni_message[0] & 0xFF);
    DSP_MW_LOG_I("[audio_transmitter] open, scenario type %d, scenario sub id %d\r\n", 2, scenario_id, sub_id.scenario_id);

    application_ptr = port_audio_transmitter_get_connection_if(scenario_id, sub_id);

    open_param = (mcu2dsp_open_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);
    application_ptr->source     = dsp_open_stream_in(open_param);
    application_ptr->sink       = dsp_open_stream_out(open_param);
    application_ptr->transform = NULL;
    audio_transmitter_configure_task(application_ptr, scenario_id, sub_id);


    /* do scenario's portable open opeartion here */
    extern void port_audio_transmitter_open(audio_transmitter_scenario_type_t scenario_id, audio_transmitter_scenario_sub_id_t sub_id, mcu2dsp_open_param_p open_param, SOURCE source, SINK sink);
    port_audio_transmitter_open(scenario_id, sub_id, open_param, application_ptr->source, application_ptr->sink);

    DSP_Callback_PreloaderConfig(application_ptr->pfeature_table, open_param->audio_scenario_type);
    DSP_MW_LOG_I("[audio_transmitter] open finish", 0);
}

void audio_transmitter_start(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    CONNECTION_IF *application_ptr;
    uint32_t gpt_timer;
    mcu2dsp_start_param_p start_param;
    audio_transmitter_scenario_type_t scenario_id;
    audio_transmitter_scenario_sub_id_t sub_id;

    UNUSED(ack);

    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &gpt_timer);

    scenario_id = (msg.ccni_message[0] >> 8) & 0xFF;
    sub_id.scenario_id  = (msg.ccni_message[0] & 0xFF);
    DSP_MW_LOG_I("[audio_transmitter] start, gpt_timer %d, scenario type %d, scenario sub id %d\r\n", 3, gpt_timer, scenario_id, sub_id);

    application_ptr = port_audio_transmitter_get_connection_if(scenario_id, sub_id);

    start_param = (mcu2dsp_start_param_p)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);
    dsp_start_stream_in(start_param, application_ptr->source);
    dsp_start_stream_out(start_param, application_ptr->sink);

    application_ptr->transform = TrasformAudio2Audio(application_ptr->source, application_ptr->sink, application_ptr->pfeature_table);
    if (application_ptr->transform == NULL) {
        DSP_MW_LOG_E("[audio_transmitter] start fail", 0);
        AUDIO_ASSERT(0);
    }

    /* do scenario's portable start opeartion here */
    extern void port_audio_transmitter_start(audio_transmitter_scenario_type_t scenario_id, audio_transmitter_scenario_sub_id_t sub_id, mcu2dsp_start_param_p start_param, SOURCE source, SINK sink);
    port_audio_transmitter_start(scenario_id, sub_id, start_param, application_ptr->source, application_ptr->sink);

    DSP_MW_LOG_I("[audio_transmitter] start finish", 0);
}

void audio_transmitter_stop(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    CONNECTION_IF *application_ptr;
    audio_transmitter_scenario_type_t scenario_id;
    audio_transmitter_scenario_sub_id_t sub_id;

    UNUSED(ack);

    scenario_id = (msg.ccni_message[0] >> 8) & 0xFF;
    sub_id.scenario_id  = (msg.ccni_message[0] & 0xFF);
    DSP_MW_LOG_I("[audio_transmitter] stop, scenario type %d, scenario id %d\r\n", 2, scenario_id, sub_id);

    application_ptr = port_audio_transmitter_get_connection_if(scenario_id, sub_id);

    /* do scenario's portable stop opeartion here */
    extern void port_audio_transmitter_stop(audio_transmitter_scenario_type_t scenario_id, audio_transmitter_scenario_sub_id_t sub_id, SOURCE source, SINK sink);
    port_audio_transmitter_stop(scenario_id, sub_id, application_ptr->source, application_ptr->sink);

    if (application_ptr->transform != NULL) {
        StreamDSPClose(application_ptr->transform->source, application_ptr->transform->sink, (msg.ccni_message[0] >> 16) | 0x8000);
    }
    application_ptr->transform = NULL;

    DSP_MW_LOG_I("[audio_transmitter] stop finish", 0);
}

void audio_transmitter_close(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    CONNECTION_IF *application_ptr;
    audio_transmitter_scenario_type_t scenario_id;
    audio_transmitter_scenario_sub_id_t sub_id;

    UNUSED(ack);

    scenario_id = (msg.ccni_message[0] >> 8) & 0xFF;
    sub_id.scenario_id  = (msg.ccni_message[0] & 0xFF);
    DSP_MW_LOG_I("[audio_transmitter] close, scenario type %d, scenario id %d\r\n", 2, scenario_id, sub_id);

    application_ptr = port_audio_transmitter_get_connection_if(scenario_id, sub_id);

    /* do scenario's portable close opeartion here */
    extern void port_audio_transmitter_close(audio_transmitter_scenario_type_t scenario_id, audio_transmitter_scenario_sub_id_t sub_id, SOURCE source, SINK sink);
    port_audio_transmitter_close(scenario_id, sub_id, application_ptr->source, application_ptr->sink);

    DSP_Callback_UnloaderConfig(application_ptr->pfeature_table, application_ptr->source->scenario_type);
    SourceClose(application_ptr->source);
    application_ptr->source = NULL;
    SinkClose(application_ptr->sink);
    application_ptr->sink = NULL;
    DSP_MW_LOG_I("[audio_transmitter] close finish", 0);
}

void audio_transmitter_config(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    CONNECTION_IF *application_ptr;
    audio_transmitter_scenario_type_t scenario_id;
    audio_transmitter_scenario_sub_id_t sub_id;
    void *config_param;

    UNUSED(ack);

    scenario_id = (msg.ccni_message[0] >> 8) & 0xFF;
    sub_id.scenario_id  = (msg.ccni_message[0] & 0xFF);
    DSP_MW_LOG_I("[audio transmitter]: config, scenario type %d, scenario id %d\r\n", 2, scenario_id, sub_id);

    application_ptr = port_audio_transmitter_get_connection_if(scenario_id, sub_id);

    config_param = (void *)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);

    extern bool port_audio_transmitter_scenario_config(audio_transmitter_scenario_type_t scenario_id, audio_transmitter_scenario_sub_id_t sub_id, void *config_param);
    port_audio_transmitter_scenario_config(scenario_id, sub_id, config_param);

    DSP_MW_LOG_I("[audio transmitter]: config finish", 0);
}

void audio_transmitter_suspend(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    CONNECTION_IF *application_ptr;
    audio_transmitter_scenario_type_t scenario_id;
    audio_transmitter_scenario_sub_id_t sub_id;

    UNUSED(ack);

    scenario_id = (msg.ccni_message[0] >> 8) & 0xFF;
    sub_id.scenario_id  = (msg.ccni_message[0] & 0xFF);
    DSP_MW_LOG_I("[audio_transmitter] suspend, scenario type %d, scenario id %d\r\n", 2, scenario_id, sub_id);

    application_ptr = port_audio_transmitter_get_connection_if(scenario_id, sub_id);
#ifdef AIR_AUDIO_DETACHABLE_MIC_ENABLE
    dsp_trigger_suspend(application_ptr->source, application_ptr->sink);
#endif
    //TODO
    DSP_MW_LOG_I("[audio_transmitter] suspend finish", 0);
}

void audio_transmitter_resume(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    CONNECTION_IF *application_ptr;
    audio_transmitter_scenario_type_t scenario_id;
    audio_transmitter_scenario_sub_id_t sub_id;

    UNUSED(ack);

    scenario_id = (msg.ccni_message[0] >> 8) & 0xFF;
    sub_id.scenario_id  = (msg.ccni_message[0] & 0xFF);
    DSP_MW_LOG_I("[audio_transmitter] resume, scenario type %d, scenario id %d\r\n", 2, scenario_id, sub_id);

    application_ptr = port_audio_transmitter_get_connection_if(scenario_id, sub_id);
#ifdef AIR_AUDIO_DETACHABLE_MIC_ENABLE
    dsp_detachable_config(application_ptr, msg);
    dsp_trigger_resume(application_ptr->source, application_ptr->sink);
#endif
    //TODO
    DSP_MW_LOG_I("[audio_transmitter] resume finish", 0);
}

void audio_transmitter_send_message(audio_transmitter_scenario_type_t scenario_type, audio_transmitter_scenario_sub_id_t scenario_id, uint32_t message)
{
    hal_ccni_message_t msg;

    memset((void *)&msg, 0, sizeof(hal_ccni_message_t));
    msg.ccni_message[0] = (MSG_DSP2MCU_AUDIO_TRANSMITTER_DATA_DIRECT << 16) | (scenario_type << 8) | scenario_id.scenario_id;
    msg.ccni_message[1] = message;
    aud_msg_tx_handler(msg, 0, FALSE);

    DSP_MW_LOG_I("[audio_transmitter] send message, scenario type %d, scenario id %d, message 0x%08x", 3, scenario_type, scenario_id, message);
}

#ifdef MTK_GAMING_MODE_HEADSET
void gaming_headset_uplink_enable_irq(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(msg);
    UNUSED(ack);

    hal_audio_trigger_start_parameter_t start_parameter;
    start_parameter.memory_select = HAL_AUDIO_MEMORY_UL_VUL1;
    start_parameter.enable = true;
    DSP_MW_LOG_I("[audio transmitter] Enable vul interrupt", 0);
    hal_audio_set_value((hal_audio_set_value_parameter_t *)&start_parameter, HAL_AUDIO_SET_TRIGGER_MEMORY_START);
}
#endif /* MTK_GAMING_MODE_HEADSET */
#endif

#ifdef MTK_ANC_ENABLE
extern void anc_passthru_stop_callback(void);
extern void anc_passthru_switch_type_callback(void);
#endif
void DSP_LOCAL_TX_HANDEL(hal_ccni_message_t msg)
{
    switch (msg.ccni_message[0] & 0xFFFF) {
#ifdef MTK_ANC_ENABLE
        case 0x0E06: {
            if (msg.ccni_message[1] == 0x0) {
                anc_passthru_stop_callback();
            } else if (msg.ccni_message[1] == 0x1) {
                anc_passthru_switch_type_callback();
            }
            break;
        }
#endif
        default:
            break;
    }
}

#ifdef MTK_ANC_ENABLE
mcu2dsp_open_adda_param_t g_audio_adda_param;
void DSP_AUDIO_ADDA_NOTIFY(uint32_t memory_addr)
{
    mcu2dsp_open_param_p adda_param_p;
    adda_param_p = (mcu2dsp_open_param_p)hal_memview_cm4_to_dsp0(memory_addr);
    memcpy(&g_audio_adda_param.adc_setting, &(adda_param_p->stream_in_param.afe), sizeof(au_afe_open_param_t));
    memcpy(&g_audio_adda_param.dac_setting, &(adda_param_p->stream_out_param.afe), sizeof(au_afe_open_param_t));
}

hal_audio_device_t DSP_OPEN_DAC(bool enable)
{
    hal_audio_device_t open_device = 0;
    hal_audio_device_parameter_t *device_handle;
    device_handle = (hal_audio_device_parameter_t *)pvPortMalloc(sizeof(hal_audio_device_parameter_t));
    if (device_handle != NULL) {
        memset(device_handle, 0, sizeof(hal_audio_device_parameter_t));
        device_handle->dac.audio_device = (hal_audio_device_t)g_audio_adda_param.dac_setting.audio_device;
        device_handle->dac.rate         = g_audio_adda_param.dac_setting.sampling_rate;
        device_handle->dac.dc_compensation_value  = afe.stream_out.dc_compensation_value;
        device_handle->dac.with_force_change_rate = false;
        device_handle->dac.dac_mode = g_audio_adda_param.dac_setting.dl_dac_mode;
#if defined(AIR_BTA_IC_PREMIUM_G2)
        device_handle->dac.with_high_performance = g_audio_adda_param.dac_setting.performance;
#else
        device_handle->dac.performance = g_audio_adda_param.dac_setting.performance;
#endif
        device_handle->common.scenario_type = AUDIO_SCENARIO_TYPE_ANC;
        hal_audio_set_device(device_handle, device_handle->common.audio_device, enable ? HAL_AUDIO_CONTROL_ON : HAL_AUDIO_CONTROL_OFF);
        if (enable) {
            open_device = device_handle->common.audio_device;
        } else {
            open_device = 0;
        }
        vPortFree(device_handle);
    } else {
        DSP_MW_LOG_E("[ANC API] Error!!! malloc fail for anc_enable_dac", 0);
    }
    return open_device;
}

void DSP_OPEN_ADC(hal_audio_control_t *device, hal_audio_device_interface_t *dev_interface, hal_audio_device_parameter_t *device_handle)
{
    uint8_t index = 0;
    if (*device & HAL_AUDIO_CONTROL_DEVICE_DIGITAL_MIC_DUAL) {
        device_handle->digital_mic.audio_device  = *device;
        device_handle->digital_mic.mic_interface = *dev_interface;
        device_handle->digital_mic.rate            = g_audio_adda_param.adc_setting.sampling_rate;
        device_handle->digital_mic.bias_select     = g_audio_adda_param.adc_setting.bias_select;
        device_handle->digital_mic.bias_voltage[0] = g_audio_adda_param.adc_setting.bias_voltage[0];
        device_handle->digital_mic.bias_voltage[1] = g_audio_adda_param.adc_setting.bias_voltage[1];
        device_handle->digital_mic.bias_voltage[2] = g_audio_adda_param.adc_setting.bias_voltage[2];
        device_handle->digital_mic.bias_voltage[3] = g_audio_adda_param.adc_setting.bias_voltage[3];
        device_handle->digital_mic.bias_voltage[4] = g_audio_adda_param.adc_setting.bias_voltage[4];
        device_handle->digital_mic.with_external_bias = g_audio_adda_param.adc_setting.with_external_bias;
        device_handle->digital_mic.with_bias_lowpower = g_audio_adda_param.adc_setting.with_bias_lowpower;
        device_handle->digital_mic.bias1_2_with_LDO0  = g_audio_adda_param.adc_setting.bias1_2_with_LDO0;
        switch (device_handle->digital_mic.audio_device) {
            case HAL_AUDIO_CONTROL_DEVICE_DIGITAL_MIC_DUAL:
            case HAL_AUDIO_CONTROL_DEVICE_DIGITAL_MIC_L:
                index = (device_handle->analog_mic.mic_interface == HAL_AUDIO_CONTROL_DEVICE_INTERFACE_1) ? 0 : ((device_handle->analog_mic.mic_interface == HAL_AUDIO_CONTROL_DEVICE_INTERFACE_2) ? 2 : 4);
                device_handle->digital_mic.dmic_selection = g_audio_adda_param.adc_setting.dmic_selection[index];
                break;
            case HAL_AUDIO_CONTROL_DEVICE_DIGITAL_MIC_R:
                index = (device_handle->analog_mic.mic_interface == HAL_AUDIO_CONTROL_DEVICE_INTERFACE_1) ? 1 : ((device_handle->analog_mic.mic_interface == HAL_AUDIO_CONTROL_DEVICE_INTERFACE_2) ? 3 : 5);
                device_handle->digital_mic.dmic_selection = g_audio_adda_param.adc_setting.dmic_selection[index];
                break;
            default:
                device_handle->digital_mic.dmic_selection = HAL_AUDIO_DMIC_GPIO_DMIC0;
                break;
        }
        switch (device_handle->digital_mic.mic_interface) {
            case HAL_AUDIO_CONTROL_DEVICE_INTERFACE_1:
                device_handle->digital_mic.iir_filter = g_audio_adda_param.adc_setting.iir_filter[0];
                break;
            case HAL_AUDIO_CONTROL_DEVICE_INTERFACE_2:
                device_handle->digital_mic.iir_filter = g_audio_adda_param.adc_setting.iir_filter[1];
                break;
            case HAL_AUDIO_CONTROL_DEVICE_INTERFACE_3:
                device_handle->digital_mic.iir_filter = g_audio_adda_param.adc_setting.iir_filter[2];
                break;
            default:
                device_handle->digital_mic.iir_filter = HAL_AUDIO_UL_IIR_5HZ_AT_48KHZ;
                break;
        }
    } else {
        device_handle->analog_mic.audio_device  = *device;
        device_handle->analog_mic.mic_interface = *dev_interface;
        device_handle->analog_mic.rate            = g_audio_adda_param.adc_setting.sampling_rate;
        device_handle->analog_mic.bias_select     = g_audio_adda_param.adc_setting.bias_select;
        device_handle->analog_mic.bias_voltage[0] = g_audio_adda_param.adc_setting.bias_voltage[0];
        device_handle->analog_mic.bias_voltage[1] = g_audio_adda_param.adc_setting.bias_voltage[1];
        device_handle->analog_mic.bias_voltage[2] = g_audio_adda_param.adc_setting.bias_voltage[2];
        device_handle->analog_mic.bias_voltage[3] = g_audio_adda_param.adc_setting.bias_voltage[3];
        device_handle->analog_mic.bias_voltage[4] = g_audio_adda_param.adc_setting.bias_voltage[4];
        device_handle->analog_mic.with_external_bias        = g_audio_adda_param.adc_setting.with_external_bias;
        device_handle->analog_mic.adc_parameter.performance = g_audio_adda_param.adc_setting.performance;
        device_handle->analog_mic.with_bias_lowpower        = g_audio_adda_param.adc_setting.with_bias_lowpower;
        device_handle->analog_mic.bias1_2_with_LDO0         = g_audio_adda_param.adc_setting.bias1_2_with_LDO0;
        switch (device_handle->analog_mic.audio_device) {
            case HAL_AUDIO_CONTROL_DEVICE_ANALOG_MIC_DUAL:
            case HAL_AUDIO_CONTROL_DEVICE_ANALOG_MIC_L:
                index = (device_handle->analog_mic.mic_interface == HAL_AUDIO_CONTROL_DEVICE_INTERFACE_1) ? 0 : ((device_handle->analog_mic.mic_interface == HAL_AUDIO_CONTROL_DEVICE_INTERFACE_2) ? 2 : 4);
                device_handle->analog_mic.adc_parameter.adc_mode = g_audio_adda_param.adc_setting.ul_adc_mode[index];
                break;
            case HAL_AUDIO_CONTROL_DEVICE_ANALOG_MIC_R:
                index = (device_handle->analog_mic.mic_interface == HAL_AUDIO_CONTROL_DEVICE_INTERFACE_1) ? 1 : ((device_handle->analog_mic.mic_interface == HAL_AUDIO_CONTROL_DEVICE_INTERFACE_2) ? 3 : 5);
                device_handle->analog_mic.adc_parameter.adc_mode = g_audio_adda_param.adc_setting.ul_adc_mode[index];
                break;
            default:
                device_handle->analog_mic.adc_parameter.adc_mode = HAL_AUDIO_ANALOG_INPUT_ACC10K;
                break;
        }
        switch (device_handle->analog_mic.mic_interface) {
            case HAL_AUDIO_CONTROL_DEVICE_INTERFACE_1:
                device_handle->analog_mic.iir_filter = g_audio_adda_param.adc_setting.iir_filter[0];
                break;
            case HAL_AUDIO_CONTROL_DEVICE_INTERFACE_2:
                device_handle->analog_mic.iir_filter = g_audio_adda_param.adc_setting.iir_filter[1];
                break;
            case HAL_AUDIO_CONTROL_DEVICE_INTERFACE_3:
                device_handle->analog_mic.iir_filter = g_audio_adda_param.adc_setting.iir_filter[2];
                break;
            default:
                device_handle->analog_mic.iir_filter = HAL_AUDIO_UL_IIR_25HZ_AT_48KHZ;
                break;
        }
    }
}
#endif

#ifdef MTK_SLT_AUDIO_HW
void AUDIO_SLT_START(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    uint32_t result_total;
    uint32_t rate;
    //audio_slt_enable(48000);
    DSP_MW_LOG_I("SLT rate= %d", 1, msg.ccni_message[1]);
    rate = msg.ccni_message[1];

    result_total = audio_slt_test_case(rate);

    DSP_MW_LOG_I("AUDIO_SLT_START", 0);
#if 1
    //hal_ccni_message_t msg;
    memset((void *)&msg, 0, sizeof(hal_ccni_message_t));
    msg.ccni_message[0] = 0x70 << 16 | 0x80000000;
    DSP_MW_LOG_I("result_total 0x%x", 1, result_total);
    msg.ccni_message[1] = result_total;//0x123;
    aud_msg_tx_handler(msg, 0, FALSE);
#endif

}
#endif

#ifdef MTK_AUDIO_LOOPBACK_TEST_ENABLE
void DSP_AUDIO_LOOPBACK_TEST_CONFIG(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    DSP_MW_LOG_I("[AUDIO LOOPBACK TEST] CONFIG", 0);
    uint32_t addr = (uint32_t)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);
    audio_loopback_test_inform_result_addr(addr);
    Audio_setting->Audio_source.Pga_mux = HAL_AUDIO_LOOPBACK_TEST_PGA_NULL;
}
#endif

// for volume setting sync
static timer_list_node_t *g_sync_timer_list = NULL;
static aud_msg_cb_node_t *g_dsp_request_sync_cb_front = NULL;
static uint32_t gpt_sync_timer_handle = 0;
hal_gpt_callback_t dsp_audio_request_sync_timer_callback(void *user_data);

// porting function
static aud_msg_cb_node_t *_msg_queue_get_middle(aud_msg_cb_node_t *front, aud_msg_cb_node_t *rear)
{
    if (front == NULL) {
        return NULL;
    }

    aud_msg_cb_node_t *step1 = front;
    aud_msg_cb_node_t *step2 = front->next;

    while (step2 != rear) {
        step2 = step2->next;
        if (step2 != rear) {
            step1 = step1->next;
            step2 = step2->next;
        }
    }

    return step1;
}

static aud_msg_cb_info_t *_msg_queue_search(aud_msg_cb_node_t *front, uint16_t ID)
{
    /* Simple binary search */

    aud_msg_cb_node_t *cb = NULL;
    aud_msg_cb_node_t *rear = NULL;

    do {
        cb = _msg_queue_get_middle(front, rear);

        if (cb == NULL) {
            return NULL;
        } else if (cb->cb_info.msg_id == ID) {
            return &(cb->cb_info);
        } else if (cb->cb_info.msg_id < ID) {
            front = cb->next;
        } else {
            rear = cb;
        }

    } while (front != rear);

    return NULL;
}
static void dsp_audio_msg_ack(uint32_t msg_id, bool from_isr)
{
    // send ACK to cm4 side
    uint32_t ack_msg = 0;
    ack_msg = (uint16_t)msg_id;
    ack_msg |= ((MSG_DSP2MCU_AUDIO_SYNC_DONE | 0x8000) << 16);
    DTM_enqueue(DTM_EVENT_ID_AUDIO_SYNC_END, ack_msg, from_isr);
    DSP_MW_LOG_E("[DSP SYNC] enqueue to dtm task, id [0x%x]\r\n", 1, ack_msg);
}
// msg_callback and ack the msg id
static void dsp_audio_msg_queue_handle(uint32_t msg_id, cm4_dsp_sync_param_t *param)
{
    dsp_sync_callback_t callback;
    // msg.ccni_message[0] structure: [CM42DSP_SYNC_ID 16bit]  [scenario_id 8bit]  [action_id 8bit]
    uint32_t scenario_id = (msg_id & 0xff00) >> 8;
    uint32_t action_id = msg_id & 0xff; // the last 8-bit
    aud_msg_cb_info_t *cb = _msg_queue_search(g_dsp_request_sync_cb_front, scenario_id); // [WARNING]
    if (cb != NULL) {
        if (cb->cb_func != NULL) {
            callback = cb->cb_func;
            callback(action_id, &(param->vol_gain_info));
        } else {
            // error
            DSP_MW_LOG_E("[DSP SYNC] id [0x%x] [0x%x] no related callback\r\n", 2, scenario_id, action_id);
        }
    } else {
        DSP_MW_LOG_E("[DSP SYNC] id [0x%x] [0x%x] no related register\r\n", 2, scenario_id, action_id);
    }
}

// gpt count compare
static int8_t gpt_count_compare(uint32_t count1, uint32_t count2)
{
    uint32_t tmp1 = count1 > count2 ? count1 : count2;
    uint32_t tmp2 = count1 > count2 ? count2 : count1;
    uint32_t delt = tmp1 - tmp2;
    int8_t sign = delt > 0x7fffffff ? -1 : 1;
    if (count1 > count2) {
        return 1 * sign;
    } else if (count1 < count2) {
        return -1 * sign;
    } else {
        return 0;
    }
}

static aud_msg_status_t dsp_audio_request_sync_timer_list_insert(timer_list_info_t *info)
{
    timer_list_node_t *cur = g_sync_timer_list;
    timer_list_node_t *tmp = NULL;
    if (cur == NULL) {
        timer_list_node_t *cb = (timer_list_node_t *)pvPortMalloc(sizeof(timer_list_node_t));
        if (cb == NULL) {
            DSP_MW_LOG_E("[DSP SYNC] timer_list_node_t malloc failed 1!!\r\n", 0);
            return AUDIO_MSG_STATUS_ERROR;
        }
        memcpy(&(cb->info), info, sizeof(timer_list_info_t));
        cb->next = NULL;
        g_sync_timer_list = cb;
        return AUDIO_MSG_STATUS_OK;
    }
    while ((cur->next != NULL) && (info->mirror_count > cur->next->info.mirror_count)) {  /* Search the the closest node */
        cur = cur->next;
    }

    /* Replace or insert the node */
    timer_list_node_t *cb = (timer_list_node_t *)pvPortMalloc(sizeof(timer_list_node_t));
    if (cb == NULL) {
        DSP_MW_LOG_E("[DSP SYNC] timer_list_node_t malloc failed 2!!\r\n", 0);
        return AUDIO_MSG_STATUS_ERROR;
    }
    memcpy(&(cb->info), info, sizeof(timer_list_info_t));

    if (info->mirror_count > cur->info.mirror_count) {
        tmp = cur->next;
        cur->next = cb;
        cb->next = tmp;
    } else {
        tmp = cur;
        cur = cb;
        cb->next = tmp;
    }

    /*  Updated the front of queue pointer */
    if (g_sync_timer_list->info.mirror_count >= cb->info.mirror_count) { // For the same count, latest is the first!
        // And there is memory leak without the modification.
        g_sync_timer_list = cb;
    }
    return AUDIO_MSG_STATUS_OK;
}

// count the item of timer list
static uint32_t dsp_audio_request_sync_timer_list_get_length(void)
{
    uint32_t cnt = 0;
    timer_list_node_t *timer = (timer_list_node_t *)g_sync_timer_list;
    while (timer) {
        cnt++;
        timer = timer->next;
    }
    return cnt;
}

void dsp_audio_request_sync_process(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    uint8_t              trigger_flag = 0; // excute callback immediately
    uint32_t             savedmask    = 0; // interrupt mask
    uint32_t             curr_cnt     = 0;
    uint32_t             curr_cnt_tmp = 0;
    int32_t              time_out     = 0;
    hal_gpt_status_t     gpt_status   = HAL_GPT_STATUS_OK;
    cm4_dsp_sync_param_t *para_addr   = NULL;
    // for show log
    uint32_t             disable_int_time = 0;
    uint32_t             enable_int_time  = 0;
    uint32_t             total_time       = 0;
    uint64_t             deviation        = 0;
    timer_list_info_t info;
    memset(&info, 0, sizeof(timer_list_info_t));
#ifdef MTK_AUDIO_DSP_SYNC_REQUEST_GPIO_DEBUG
    DSP_MW_LOG_I("[DSP SYNC] init gpio test", 0);
    hal_gpio_set_direction(HAL_GPIO_23, 1);
    hal_gpio_disable_pull(HAL_GPIO_23);
    hal_pinmux_set_function(HAL_GPIO_23, 0);
    hal_gpio_set_output(HAL_GPIO_23, 0);
#endif /* MTK_AUDIO_DSP_SYNC_REQUEST_GPIO_DEBUG */
    /* -1- parse msg info --------------------------------------------------------------------*/
    uint32_t msg_id      = (uint32_t)msg.ccni_message[0];
    info.msg_id          = msg_id;
    para_addr            = (cm4_dsp_sync_param_t *)hal_memview_cm4_to_dsp0(msg.ccni_message[1]); // dsp must remap the address
    memcpy(&info.param, para_addr, sizeof(cm4_dsp_sync_param_t));
    // dsp_audio_msg_ack(info.msg_id, false);
    /* -2- check the time --------------------------------------------------------------------*/
//#ifdef MTK_AUDIO_DSP_SYNC_REQUEST_DEBUG
    DSP_MW_LOG_I("[DSP SYNC] dsp receive info: id0 = 0x%x, para_addr = 0x%x, channel = %d, vol_gain = %d, t_cnt = %u\r\n", 5,
                 msg_id, para_addr, info.param.vol_gain_info.gain_select, info.param.vol_gain_info.gain, info.param.gpt_count);
//#endif
    hal_nvic_save_and_set_interrupt_mask(&savedmask);
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &curr_cnt);
    curr_cnt_tmp = curr_cnt;
    disable_int_time = curr_cnt;
    time_out = info.param.gpt_count - curr_cnt;
    if (time_out < AUDIO_DSP_SYNC_MIN_TIME_OUT) { // < 1ms
        // AUDIO_ASSERT(0);
        trigger_flag = 1;
        goto ERROR_HANDLE;
    }
    if (time_out > 5000000) { // 5s, Abnormal Case!
        DSP_MW_LOG_W("[DSP SYNC] ATTENTION: delay time is so big!", 0);
        // do nothing
    }
    deviation = time_out;
    info.mirror_count  = deviation + curr_cnt;
    /* -3- append timer list ------------------------------------------------------------------*/
    if (dsp_audio_request_sync_timer_list_insert(&info) != AUDIO_MSG_STATUS_OK) {
        trigger_flag |= 1 << 1;
        goto ERROR_HANDLE;
    }

    /* -4- re-start timer ---------------------------------------------------------------------*/
    if (info.param.gpt_count == g_sync_timer_list->info.param.gpt_count) {
        // restart timer
        if (gpt_sync_timer_handle == 0) {
            gpt_status = hal_gpt_sw_get_timer(&gpt_sync_timer_handle);
            if (gpt_status != HAL_GPT_STATUS_OK) {
                trigger_flag |= 1 << 2;
                DSP_MW_LOG_W("[DSP SYNC] get gpt_status = %d\r\n", 1, gpt_status);
                goto ERROR_HANDLE;
            }
        } else {
            gpt_status = hal_gpt_sw_stop_timer_us(gpt_sync_timer_handle);
            if (gpt_status != HAL_GPT_STATUS_OK) {
                // trigger_flag |= 1 << 3;
                // goto ERROR_HANDLE;
            }
        }
        hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &curr_cnt);
        time_out = info.param.gpt_count - curr_cnt - AUDIO_DSP_SYNC_FIXED_POLLING_TIME; // [TODO] check over flow
        // avoid negative number
        if (time_out < 0) {
            time_out = 0;
        }
        gpt_status = hal_gpt_sw_start_timer_us(gpt_sync_timer_handle, time_out, (hal_gpt_callback_t)dsp_audio_request_sync_timer_callback, NULL);
        if (gpt_status != HAL_GPT_STATUS_OK) {
            trigger_flag |= 1 << 4;
            DSP_MW_LOG_W("[DSP SYNC] start gpt_status = %d\r\n", 1, gpt_status);
            goto ERROR_HANDLE;
        } else {
            // if (g_sync_timer_list != NULL) {
            //     if (g_sync_timer_list->next == NULL) {
            //         // boost the frequency
            //         dvfs_lock_control("DSP_SYNC", DVFS_78M_SPEED, DVFS_LOCK);
            //     }
            // }
        }
    }
ERROR_HANDLE:
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &enable_int_time);
    hal_nvic_restore_interrupt_mask(savedmask);
    hal_gpt_get_duration_count(disable_int_time, enable_int_time, &total_time);
    DSP_MW_LOG_I("[DSP SYNC] msg process, exit critical region! total time = %d\r\n", 1, total_time);
    /* -5- excute callback immediately -----------------------------------------------------------*/
    if (trigger_flag != 0) {
        // excute callback immediately, search the related scenario's callback
        dsp_audio_msg_queue_handle(info.msg_id, &(info.param));
        dsp_audio_msg_ack(info.msg_id, false);
        DSP_MW_LOG_W("[DSP SYNC] abnormal condition occur, error = %d\r\n", 1, trigger_flag);
        if (trigger_flag & 0x1C) { // avoid memory leak
            timer_list_node_t *tmp = g_sync_timer_list->next;
            vPortFree(g_sync_timer_list);
            g_sync_timer_list = tmp;
        }
    }
    uint32_t number = dsp_audio_request_sync_timer_list_get_length();
    if (g_sync_timer_list != NULL) { // show detail information
        DSP_MW_LOG_I("[DSP SYNC] timer list  list_len = %d, msg0 = 0x%x, vol_ch = %d, vol_gain = %d, t_cnt = %u, c_cnt = %u, t_out = %d, m_cnt = %u\r\n", 8,
                     number, g_sync_timer_list->info.msg_id, g_sync_timer_list->info.param.vol_gain_info.gain_select, g_sync_timer_list->info.param.vol_gain_info.gain,
                     g_sync_timer_list->info.param.gpt_count, curr_cnt_tmp, time_out, g_sync_timer_list->info.mirror_count);
    } else {
        DSP_MW_LOG_I("[DSP SYNC] timer list msg0 = 0x%x, vol_ch = %d, vol_gain = %d, t_cnt = %u, c_cnt = %u, t_out = %d, m_cnt = %u, number = 0\r\n",
                     7, info.msg_id, info.param.vol_gain_info.gain_select, info.param.vol_gain_info.gain,  info.param.gpt_count, curr_cnt_tmp, time_out, info.mirror_count);
    }
}

aud_msg_status_t dsp_audio_request_sync_register_callback(dsp_sync_request_scenario_id_t request_scenario_id, dsp_sync_callback_t *sync_callback)
{
    uint8_t ack_opt = 0; // means ignore this parameter
    aud_msg_cb_node_t *cur = g_dsp_request_sync_cb_front;
    aud_msg_cb_node_t *tmp = NULL;

    if (cur == NULL) {   /* Nothing in the queue */
        aud_msg_cb_node_t *cb = (aud_msg_cb_node_t *)pvPortMalloc(sizeof(aud_msg_cb_node_t));
        if (cb == NULL) {
            DSP_MW_LOG_E("DSP SYNC CB malloc failed 1!!\r\n", 0);
            return AUDIO_MSG_STATUS_ERROR;
        }
        cb->cb_info.msg_id = request_scenario_id;
        cb->cb_info.cb_func = sync_callback;
        cb->cb_info.ack_option = ack_opt;
        cb->next = NULL;
        g_dsp_request_sync_cb_front = cb;
        return AUDIO_MSG_STATUS_OK;
    }

    while ((cur->next != NULL) && (request_scenario_id >= cur->next->cb_info.msg_id)) {  /* Search the the closest node */
        cur = cur->next;
    }

    /* Replace or insert the node */
    if (request_scenario_id == cur->cb_info.msg_id) {
        cur->cb_info.cb_func = sync_callback;
        cur->cb_info.ack_option = ack_opt;
        return AUDIO_MSG_STATUS_OK;
    } else {
        aud_msg_cb_node_t *cb = (aud_msg_cb_node_t *)pvPortMalloc(sizeof(aud_msg_cb_node_t));
        if (cb == NULL) {
            DSP_MW_LOG_E("DSP SYNC CB malloc failed 2!!\r\n", 0);
            return AUDIO_MSG_STATUS_ERROR;
        }
        cb->cb_info.msg_id = request_scenario_id;
        cb->cb_info.cb_func = sync_callback;
        cb->cb_info.ack_option = ack_opt;

        if (request_scenario_id > cur->cb_info.msg_id) {
            tmp = cur->next;
            cur->next = cb;
            cb->next = tmp;
        } else {
            tmp = cur;
            cur = cb;
            cb->next = tmp;
        }

        /*  Updated the front of queue pointer */
        if (g_dsp_request_sync_cb_front->cb_info.msg_id >= cb->cb_info.msg_id) {
            g_dsp_request_sync_cb_front = cb;
        }
        if (g_dsp_request_sync_cb_front->cb_info.msg_id == cb->cb_info.msg_id) { // one id can only has one callback func.
            DSP_MW_LOG_E("[DSP SYNC] ERROR: register callback, ID 0x%x 0x%x", 2, g_dsp_request_sync_cb_front->cb_info.msg_id,
                         cb->cb_info.msg_id);
            AUDIO_ASSERT(0);
            return AUDIO_MSG_STATUS_ERROR;
        }
    }
    return AUDIO_MSG_STATUS_OK;
}

hal_gpt_callback_t dsp_audio_request_sync_timer_callback(void *user_data)
{
    UNUSED(user_data);
    timer_list_info_t info;
    uint32_t savedmask = 0;
    uint32_t curr_cnt  = 0;
    uint32_t time_out  = 0;
    // for show log
    uint32_t disable_int_time = 0;
    uint32_t enable_int_time  = 0;
    uint32_t total_time       = 0;
    timer_list_node_t *tmp    = NULL; // buffer the next timer list pointer before freeing the first item pointer of timer list
    memset(&info, 0, sizeof(timer_list_info_t));
    hal_nvic_save_and_set_interrupt_mask(&savedmask); // enter cirtical code region
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &disable_int_time);
#ifdef MTK_AUDIO_DSP_SYNC_REQUEST_GPIO_DEBUG
    hal_gpio_set_output(HAL_GPIO_2, 0);
#endif /* MTK_AUDIO_DSP_SYNC_REQUEST_GPIO_DEBUG */
LOOP:
#ifdef MTK_AUDIO_DSP_SYNC_REQUEST_DEBUG
    DSP_MW_LOG_I("[DSP SYNC] loop 0x%x %d %d %d\r\n", 4, g_sync_timer_list->info.msg_id, g_sync_timer_list->info.param.vol_gain_info.gain_select,
                 g_sync_timer_list->info.param.vol_gain_info.gain, g_sync_timer_list->info.param.gpt_count);
#endif /* MTK_AUDIO_DSP_SYNC_REQUEST_DEBUG */
    /* -1- free expired timer ----------------------------------------------------------------*/
    tmp = g_sync_timer_list->next;
    memcpy(&info, &(g_sync_timer_list->info), sizeof(timer_list_info_t));
    /* -2- polling tick and excute expired timer's callback  ---------------------------------*/
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &curr_cnt);
    if (info.param.gpt_count > curr_cnt) { // gpt register does not overflow
#ifdef MTK_AUDIO_DSP_SYNC_REQUEST_DEBUG
        DSP_MW_LOG_I("[DSP SYNC] trigger %d curr_cnt %u  tar %d", 3, __LINE__, curr_cnt, info.param.gpt_count);
#endif /* MTK_AUDIO_DSP_SYNC_REQUEST_DEBUG */
        while (1) {
            hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &curr_cnt);
            if (curr_cnt >= info.param.gpt_count) { // expire at time
                break;
            }
        }
    } else if (curr_cnt - info.param.gpt_count > 0x7fffffff) { // gpt register overflow
#ifdef MTK_AUDIO_DSP_SYNC_REQUEST_DEBUG
        DSP_MW_LOG_I("[DSP SYNC] trigger %d curr_cnt %u  tar %d", 3, __LINE__, curr_cnt, info.param.gpt_count);
#endif /* MTK_AUDIO_DSP_SYNC_REQUEST_DEBUG */
        while (1) {
            hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &curr_cnt);
            if (curr_cnt >= info.param.gpt_count) { // expire at time
                if ((curr_cnt & 0x80000000) == 0x0) {
                    break;
                }
            }
        }
    } else {
        DSP_MW_LOG_E("[DSP SYNC]Warning: already expire\r\n", 0);
        // AUDIO_ASSERT(0);
    }
    // callback and ack
    dsp_audio_msg_queue_handle(info.msg_id, &(info.param));
    dsp_audio_msg_ack(info.msg_id, true);
    DSP_MW_LOG_I("[DSP SYNC] Trigger, polling end! c_cnt = %u, t_cnt = %u\r\n", 2, curr_cnt, info.param.gpt_count);
    vPortFree(g_sync_timer_list);
    g_sync_timer_list = tmp;
#ifdef MTK_AUDIO_DSP_SYNC_REQUEST_DEBUG
    if (g_sync_timer_list != NULL) { // show detail information
        DSP_MW_LOG_I("[DSP SYNC] next timer list msg0 = 0x%x, vol_ch = %d, vol_gain = %d, t_cnt = %u, list_len = %d\r\n", 5, g_sync_timer_list->info.msg_id,
                     g_sync_timer_list->info.param.vol_gain_info.gain_select, g_sync_timer_list->info.param.vol_gain_info.gain, g_sync_timer_list->info.param.gpt_count,
                     dsp_audio_request_sync_timer_list_get_length());
    }
#endif /* MTK_AUDIO_DSP_SYNC_REQUEST_DEBUG */
    /* -3- check and start next timer  -------------------------------------------------------*/
    if (g_sync_timer_list != NULL) {
        // while (g_sync_timer_list->next) {
        hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &curr_cnt);
        if (gpt_count_compare(g_sync_timer_list->info.param.gpt_count, curr_cnt) != 1) {
            // trigger immedately
            DSP_MW_LOG_D("[DSP SYNC] Trigger immediately, goto loop\r\n", 0);
            goto LOOP; // about 30-40us delay
        } else {
            hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &curr_cnt);
            time_out = g_sync_timer_list->info.param.gpt_count - curr_cnt - AUDIO_DSP_SYNC_FIXED_POLLING_TIME; // [TODO] check overflow
            int8_t res = gpt_count_compare(g_sync_timer_list->info.param.gpt_count, curr_cnt + AUDIO_DSP_SYNC_REQUEST_MIN_REQUEST_INTERVAL);
            if (res == 1) {
                // start timer
                hal_gpt_status_t gpt_status = hal_gpt_sw_start_timer_us(gpt_sync_timer_handle, time_out, (hal_gpt_callback_t) dsp_audio_request_sync_timer_callback, NULL);
                if (gpt_status != HAL_GPT_STATUS_OK) { // [TODO]
                    DSP_MW_LOG_E("[DSP SYNC] timer start error\r\n", 0);
                    // assert
                }
                // break;
            } else {
                // start polling
                DSP_MW_LOG_D("[DSP SYNC] goto loop\r\n", 0);
                goto LOOP; // ATTENTION: maybe exist some abnormal sitution
            }
        }
        //}
    }
#ifdef MTK_AUDIO_DSP_SYNC_REQUEST_DEBUG
    DSP_MW_LOG_I("[DSP SYNC] exit loop\r\n", 0);
#endif /* MTK_AUDIO_DSP_SYNC_REQUEST_DEBUG */
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &enable_int_time);
    hal_nvic_restore_interrupt_mask(savedmask);
    hal_gpt_get_duration_count(disable_int_time, enable_int_time, &total_time);
    DSP_MW_LOG_I("[DSP SYNC] timer callback, exit citical region! total time = %d\r\n", 1, total_time);
    // release gpt timer handle
    if (g_sync_timer_list == NULL) {
        // down frequency
        // dvfs_lock_control("DSP_SYNC", DVFS_78M_SPEED, DVFS_UNLOCK); // NO API
        if (hal_gpt_sw_free_timer(gpt_sync_timer_handle) != HAL_GPT_STATUS_OK) {
            DSP_MW_LOG_E("[DSP SYNC] gpt timer release error\r\n", 0);
        } else {
            DSP_MW_LOG_I("[DSP SYNC] release timer, timer list len = %d\r\n", 1, dsp_audio_request_sync_timer_list_get_length());
        }
        gpt_sync_timer_handle = 0;
    }
    return 0;
}

#ifdef AIR_AUDIO_HARDWARE_ENABLE
extern void dsp_sync_callback_a2dp(dsp_sync_request_action_id_t request_action_id, void *user_data);
extern void dsp_sync_callback_hfp(dsp_sync_request_action_id_t request_action_id, void *user_data);
extern void dsp_sync_callback_vp(dsp_sync_request_action_id_t request_action_id, void *user_data);
#endif

