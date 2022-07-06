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
#if defined (AIR_DUAL_CHIP_MIXING_MODE_ROLE_MASTER_ENABLE) || defined (AIR_DUAL_CHIP_MIXING_MODE_ROLE_SLAVE_ENABLE)
/******************************************************************************
 * 
 * Include
 * 
 ******************************************************************************/
#include "scenario_dchs.h"
#include "mux.h"
#include "memory_attribute.h"
#include "bt_a2dp.h"
#include "bt_sink_srv_ami.h"
#include "audio_transmitter_playback_port.h"
#include "audio_dump.h"
#include "hal_sleep_manager_platform.h"

/******************************************************************************
 * 
 * Extern Function and Variable
 * 
 ******************************************************************************/
#if defined(MTK_AVM_DIRECT)
extern HAL_DSP_PARA_AU_AFE_CTRL_t audio_nvdm_HW_config;
#endif
extern void bt_sink_srv_am_set_volume(bt_sink_srv_am_stream_type_t in_out, bt_sink_srv_audio_setting_vol_info_t *vol_info);
extern void user_audio_transmitter_callback(audio_transmitter_event_t event, void *data, void *user_data);

/******************************************************************************
 * 
 * Private Macro and Variable Declaration 
 * 
 ******************************************************************************/
#define MUX_UART_TXBUF_MAX_SIZE  (4096)
#define MUX_UART_RXBUF_MAX_SIZE  (8192)
#define MUX_UART_BUF_SLICE       (6)
#define AUDIO_DURATION_TIME      (5)

static bool mux_uart_init_flag = false;
static mux_handle_t audio_cmd_uart_handle = 0;
static bool  audio_cmd_uart_open_flag = false;
static int8_t dl_transmitter_id = -1;
static bool   other_chip_dchs_dl_exist = false;
static bool   local_chip_dchs_dl_exist = false;
static audio_transmitter_config_t transmitter_config;
static bool   bt_lock_sleep_done = true; //[hard code]

ATTR_ZIDATA_IN_NONCACHED_SYSRAM_4BYTE_ALIGN static uint8_t mux_uart_txbuffer[MUX_UART_TXBUF_MAX_SIZE];
ATTR_ZIDATA_IN_NONCACHED_SYSRAM_4BYTE_ALIGN static uint8_t mux_uart_rxbuffer[MUX_UART_RXBUF_MAX_SIZE];

typedef enum
{
    READY_READ,
    WAITING_READ,
}uart_cmd_status_t;

typedef struct
{
    uart_cmd_type_t ctrl_type;
    uint32_t param_size;
    uart_cmd_status_t cmd_status;
}audio_uart_cmd_status_t;

typedef enum
{
    DCHS_DL_OTHER_CHIP_DL_EXIST,
    //extend here
    SYNC_TYPE_MAX = 0xFFFFFFFF,
}dchs_mcu2dsp_sync_type_t;

typedef struct
{
    bool other_chip_dl_exist;
    //dl sync extend here
}dual_chip_dl_sync_msg_t;

typedef union{
    dual_chip_dl_sync_msg_t  dual_chip_dl_sync;
    // ul sync add here
}audio_dchs_mcu2dsp_sync_msg_t;

typedef struct
{
    dchs_mcu2dsp_sync_type_t sync_type;
    audio_dchs_mcu2dsp_sync_msg_t sync_msg;
}audio_dchs_mcu2dsp_cosys_sync_t;

static audio_uart_cmd_status_t cur_uart_cmd_status = {AUDIO_UART_COSYS_CMD_MIN, 0 , READY_READ};
ATTR_SHARE_ZIDATA static audio_dchs_mcu2dsp_cosys_sync_t dchs_cosys_sync;

/******************************************************************************
 * 
 * Private Function Define
 * 
 ******************************************************************************/
void mcu_uart_rx(mux_handle_t handle,U8 * user_rx_buf,U32 buf_size)
{
    mux_status_t mux_status;
    mux_buffer_t uart_rx_buffer;
    U32 rx_size = 0;
    uart_rx_buffer.p_buf    = user_rx_buf;
    uart_rx_buffer.buf_size = buf_size;
    mux_status = mux_rx(handle, &uart_rx_buffer, &rx_size);
    if (mux_status != MUX_STATUS_OK) {
        TRANSMITTER_LOG_E("[DCHS][uart callback] mux uart rx fail,status=%d", 1, mux_status);
        AUDIO_ASSERT(0);
        return;
    }
    if(rx_size != buf_size){
        TRANSMITTER_LOG_E("[DCHS][uart callback] mux uart rx fail,already_rx_size=%d,need_rx_size=%d", 2, rx_size, buf_size);
        AUDIO_ASSERT(0);
        return;
    }
    //LOG_AUDIO_DUMP(user_rx_buf,rx_size,AUDIO_DCHS_UART_DL_SOURCE);
    TRANSMITTER_LOG_I("[DCHS][uart callback] audio cmd rx data size: %d", 1, rx_size);
}

void user_transmitter_callback(audio_transmitter_event_t event, void *data, void *user_data)
{
    if(event == AUDIO_TRANSMITTER_EVENT_STOP_SUCCESS){
        if(dl_transmitter_id < 0){
            TRANSMITTER_LOG_E("[DCHS][transmitter_callback]: dl uart transmitter deinit false, user id do not use", 0);
            AUDIO_ASSERT(0);
        }
        audio_transmitter_status_t status = audio_transmitter_deinit(dl_transmitter_id);
        if(status != AUDIO_TRANSMITTER_STATUS_SUCCESS){
            TRANSMITTER_LOG_E("[DCHS][transmitter_callback]: dl uart deinit false", 0);
            AUDIO_ASSERT(0);
        }
        dl_transmitter_id = -1;
        TRANSMITTER_LOG_I("[DCHS][transmitter_callback]: transmitter stop and deinit success", 0);
    }
    if (event == AUDIO_TRANSMITTER_EVENT_START_SUCCESS){
        TRANSMITTER_LOG_I("[DCHS][transmitter_callback]: transmitter start done", 0);
    }
}

uint32_t query_uart_buf_remain_size()
{
    mux_ctrl_para_t rx_param;
    mux_user_control(audio_cmd_uart_handle,MUX_CMD_GET_LL_USER_RX_BUFFER_DATA_SIZE,&rx_param);
    return rx_param.mux_ll_user_rx_data_len;
}

void reset_cur_cmd_status()
{
    //clear cur cmd status
    cur_uart_cmd_status.cmd_status = READY_READ;
    cur_uart_cmd_status.ctrl_type  = AUDIO_UART_COSYS_CMD_MIN;
    cur_uart_cmd_status.param_size = 0;
}

void dchs_dl_other_chip_dl_exist_sync(bool exist_flag)
{
    dchs_cosys_sync.sync_type = DCHS_DL_OTHER_CHIP_DL_EXIST;
    dchs_cosys_sync.sync_msg.dual_chip_dl_sync.other_chip_dl_exist = exist_flag;
    hal_audio_dsp_controller_send_message(MSG_DSP2MCU_DCHS_COSYS_SYNC, exist_flag, (uint32_t)&dchs_cosys_sync, false);
    TRANSMITTER_LOG_I("[DCHS][MCU] enter dchs dl ccni msg sync,sync type=%d,exist=%d",2,dchs_cosys_sync.sync_type,dchs_cosys_sync.sync_msg.dual_chip_dl_sync.other_chip_dl_exist);
}

ATTR_TEXT_IN_TCM void mcu_mux_uart_callback(mux_handle_t handle, mux_event_t event, uint32_t data_len, void *user_data)
{
    uint32_t need_rx_size = 0;
    switch (event) {
        case MUX_EVENT_READY_TO_READ:
            if(handle == audio_cmd_uart_handle){
                if (cur_uart_cmd_status.cmd_status == READY_READ){
                    uint32_t uart_remain_size =  query_uart_buf_remain_size();
                    if(uart_remain_size >= sizeof(audio_uart_cmd_header_t)){
                        audio_uart_cmd_header_t cmd_header;
                        mcu_uart_rx(audio_cmd_uart_handle, &cmd_header, sizeof(audio_uart_cmd_header_t));
                        cur_uart_cmd_status.ctrl_type  = cmd_header.ctrl_type;
                        cur_uart_cmd_status.param_size = cmd_header.param_size;
                        if(cur_uart_cmd_status.ctrl_type <= AUDIO_UART_COSYS_CMD_MIN || cur_uart_cmd_status.ctrl_type >= AUDIO_UART_COSYS_CMD_MAX){
                            TRANSMITTER_LOG_E("[DCHS][uart callback]  rx invalid header, param_size = %d", 1, cur_uart_cmd_status.param_size);
                            uint8_t test_buf[uart_remain_size];
                            mcu_uart_rx(audio_cmd_uart_handle,test_buf,sizeof(test_buf));//clear uart rx buf
                            AUDIO_ASSERT(0);
                            return;
                        }
                        cur_uart_cmd_status.cmd_status = WAITING_READ;
                    }
                }
                if (cur_uart_cmd_status.cmd_status == WAITING_READ){
                    uint32_t uart_remain_size =  query_uart_buf_remain_size();
                    if(uart_remain_size >= cur_uart_cmd_status.param_size){
                        audio_dchs_cosys_ctrl_t cosys_ctrl;
                        cosys_ctrl.ctrl_type = cur_uart_cmd_status.ctrl_type;
                        uint8_t header_size  = sizeof(audio_uart_cmd_header_t);
                        
                        if(cur_uart_cmd_status.ctrl_type == AUDIO_UART_COSYS_DL_OPEN){
                            audio_dchs_dl_open_param_t dl_open_ctrl_param;
                            mcu_uart_rx(audio_cmd_uart_handle, (uint8_t*)&dl_open_ctrl_param + header_size, sizeof(audio_dchs_dl_open_param_t) - header_size);
                            cosys_ctrl.ctrl_param.dchs_dl_open_param = dl_open_ctrl_param;
                            //DCHS_pka_lock_bt_sleep();//disable controller sleep
                            TRANSMITTER_LOG_I("[DCHS][rx cmd] scenario_type=%d, ctrl type = DL_OPEN,", 1 , dl_open_ctrl_param.scenario_type);
                        }else if (cur_uart_cmd_status.ctrl_type == AUDIO_UART_COSYS_DL_START){
                            audio_dchs_dl_start_param_t dl_start_ctrl_param;
                            mcu_uart_rx(audio_cmd_uart_handle, (uint8_t*)&dl_start_ctrl_param + header_size, sizeof(audio_dchs_dl_start_param_t) - header_size);
                            other_chip_dchs_dl_exist = true;
                            dchs_dl_other_chip_dl_exist_sync(true);//send ccni sync msg to dsp
                            cosys_ctrl.ctrl_param.dchs_dl_start_param = dl_start_ctrl_param;
                            TRANSMITTER_LOG_I("[DCHS][rx cmd] scenario_type=%d, ctrl type = DL_START,", 1 , dl_start_ctrl_param.scenario_type);
                            if(dl_transmitter_id != -1){
                                TRANSMITTER_LOG_W("[DCHS][rx cmd] dl already started", 0);
                                reset_cur_cmd_status();
                                return;
                            }
                            if(bt_lock_sleep_done == false){
                                TRANSMITTER_LOG_E("[DCHS][rx cmd] bt lock sleep failed", 0);
                                AUDIO_ASSERT(0);
                            }
                        }else if(cur_uart_cmd_status.ctrl_type == AUDIO_UART_COSYS_DL_CLOSE){
                            other_chip_dchs_dl_exist = false;
                            dchs_dl_other_chip_dl_exist_sync(false);//send ccni sync msg to dsp
                            audio_dchs_dl_close_param_t dl_close_ctrl_param;
                            mcu_uart_rx(audio_cmd_uart_handle, (uint8_t*)&dl_close_ctrl_param + header_size, sizeof(audio_dchs_dl_close_param_t) - header_size);
                            TRANSMITTER_LOG_I("[DCHS][rx cmd] scenario_type=%d, ctrl type = DL_CLOSE,", 1 , dl_close_ctrl_param.scenario_type);
                            if(local_chip_dchs_dl_exist){
                                TRANSMITTER_LOG_W("[DCHS][rx cmd] the local chip dl exist, dchs dl ignore close!",0);
                                reset_cur_cmd_status();
                                return; //will igore the other chip close cmd when local dl exist.
                            }
                            cosys_ctrl.ctrl_param.dchs_dl_close_param = dl_close_ctrl_param;
                        }
                        /***************
                         *   ul extend here
                         * ************/
                        //clear cur cmd status
                        reset_cur_cmd_status();
                        //send to am task to do flow from irq
                        bt_sink_srv_am_feature_t feature_param;
                        feature_param.type_mask = AM_UART_COSYS_CONTROL;
                        feature_param.feature_param.cosys_ctrl = cosys_ctrl;
                        am_audio_set_feature_ISR(FEATURE_NO_NEED_ID, &feature_param);
                        //mcu_dchs_cosys_ctrl_cmd_execute(cosys_ctrl);
                    }
                }
            }
            break;
        case MUX_EVENT_READY_TO_WRITE:
            TRANSMITTER_LOG_I("[DCHS][uart callback] MUX_EVENT_READY_TO_WRITE", 0);
            break;
        default:
            TRANSMITTER_LOG_I("[DCHS][uart callback] no event: %d", 1, event);
    }
}

/******************************************************************************
 * 
 * Public Function Define
 * 
 ******************************************************************************/
/*------------------------------------------------PORT----AIR_DCHS_ENABLE------------------------------------------------------------------*/

void audio_transmitter_dchs_open_playback(audio_transmitter_config_t *config, mcu2dsp_open_param_t *open_param)
{
    if(config->scenario_sub_id == AUDIO_TRANSMITTER_DCHS_UART_DL){
        //reuse a2dp hw gain level
        bt_sink_srv_audio_setting_vol_info_t vol_info;
        // vol_info.type = VOL_A2DP;
        // vol_info.vol_info.usb_audio_vol_info.dev = HAL_AUDIO_DEVICE_DAC_DUAL;
        // vol_info.vol_info.usb_audio_vol_info.lev = audio_get_max_sound_level_out(VOL_A2DP)/2;
        // bt_sink_srv_am_set_volume(STREAM_OUT_3, &vol_info);
        //hal_audio_set_stream_out_volume(HAL_AUDIO_STREAM_OUT3, 0, 0x7FFF);

        open_param->param.stream_in     = STREAM_IN_UART;
        open_param->param.stream_out    = STREAM_OUT_AFE;
        open_param->audio_scenario_type = AUDIO_SCENARIO_TYPE_DCHS_UART_DL;
        //stream in
        open_param->stream_in_param.afe.format           = config->scenario_config.dchs_config.format;
        open_param->stream_in_param.afe.frame_size       = AUDIO_DURATION_TIME * config->scenario_config.dchs_config.sampling_rate / MUX_UART_BUF_SLICE / 1000;//config->scenario_config.dchs_config.frame_size;
        open_param->stream_in_param.afe.frame_number     = config->scenario_config.dchs_config.frame_number;
        open_param->stream_in_param.afe.sampling_rate    = config->scenario_config.dchs_config.sampling_rate;
        //stream out
        open_param->stream_out_param.afe.audio_device    = HAL_AUDIO_DEVICE_DAC_DUAL;
        open_param->stream_out_param.afe.stream_channel  = HAL_AUDIO_DIRECT;
        open_param->stream_out_param.afe.memory          = (config->scenario_config.dchs_config.out_memory == HAL_AUDIO_MEM1) ? HAL_AUDIO_MEM3 : HAL_AUDIO_MEM1;
        open_param->stream_out_param.afe.audio_interface = config->scenario_config.dchs_config.out_interface;
        open_param->stream_out_param.afe.format          = config->scenario_config.dchs_config.format;
        open_param->stream_out_param.afe.sampling_rate   = config->scenario_config.dchs_config.sampling_rate;//48k or 96k
        open_param->stream_out_param.afe.irq_period      = 0;//config->scenario_config.dchs_config.irq_period;
        open_param->stream_out_param.afe.frame_size      = open_param->stream_in_param.afe.frame_size;
        open_param->stream_out_param.afe.frame_number    = config->scenario_config.dchs_config.frame_number;
        open_param->stream_out_param.afe.hw_gain         = false;

        #ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
        open_param->stream_out_param.afe.performance = audio_nvdm_HW_config.adc_dac_config.ADDA_DAC_Class_AB_G_Performance_Sel;
        switch (audio_nvdm_HW_config.adc_dac_config.ADDA_DAC_Mode_Sel) {
            case 0x00: //Class G
            case 0x01: //Class AB
                open_param->stream_out_param.afe.dl_dac_mode = HAL_AUDIO_ANALOG_OUTPUT_CLASSAB;
                break;
            case 0x02: //Class D
                open_param->stream_out_param.afe.dl_dac_mode = HAL_AUDIO_ANALOG_OUTPUT_CLASSD;
                break;
            default:
                open_param->stream_out_param.afe.dl_dac_mode = HAL_AUDIO_ANALOG_OUTPUT_CLASSAB;
                break;
        }
        #endif
        #if defined(MTK_EXTERNAL_DSP_NEED_SUPPORT)
        ami_set_afe_param(STREAM_OUT, HAL_AUDIO_SAMPLING_RATE_48KHZ, true);
        #endif
        TRANSMITTER_LOG_I("[MCU][DCHS] dchs dl scenario_sub_id %d open, sink memory=%d", 2, config->scenario_sub_id,open_param->stream_out_param.afe.memory);
    }
    if(config->scenario_sub_id == AUDIO_TRANSMITTER_DCHS_UART_UL)
    {
        memset(open_param, 0, sizeof(mcu2dsp_open_param_t));

        open_param->param.stream_in = STREAM_IN_AFE;
        open_param->param.stream_out = STREAM_OUT_VIRTUAL;

        // open_param->stream_in_param.afe.audio_device = HAL_AUDIO_DEVICE_MAIN_MIC_DUAL;

        // open_param->stream_in_param.afe.stream_channel = HAL_AUDIO_MONO;
        // open_param->stream_in_param.afe.memory = HAL_AUDIO_MEM3;
        // open_param->stream_in_param.afe.audio_interface = HAL_AUDIO_INTERFACE_1;   
    }
}

void audio_transmitter_dchs_start_playback(audio_transmitter_config_t *config, mcu2dsp_start_param_t *start_param)
{
    if(config->scenario_sub_id == AUDIO_TRANSMITTER_DCHS_UART_DL){
        start_param->param.stream_in  = STREAM_IN_UART;
        start_param->param.stream_out = STREAM_OUT_AFE;
        memset((void *)&start_param->stream_in_param,  0, sizeof(mcu2dsp_start_stream_in_param_t));
        memset((void *)&start_param->stream_out_param, 0, sizeof(mcu2dsp_start_stream_out_param_t));

        start_param->stream_in_param.a2dp.start_asi          = config->scenario_config.dchs_config.start_asi;
        start_param->stream_in_param.a2dp.start_bt_clk       = config->scenario_config.dchs_config.start_bt_clk;
        start_param->stream_in_param.a2dp.start_bt_intra_clk = config->scenario_config.dchs_config.start_bt_intra_clk;
        start_param->stream_out_param.afe.mce_flag           = false; //enable play en
        TRANSMITTER_LOG_I("[MCU][DCHS] dchs dl scenario_sub_id %d start", 1, config->scenario_sub_id);
    }
}

void mcu_uart_init(void)
{
    if(mux_uart_init_flag) { //inited
        return;
    }
    hal_sleep_manager_lock_sleep(SLEEP_LOCK_UART1);
    mux_status_t status;
    mux_port_t   mux_port = MUX_LL_UART_1;
    mux_port_setting_t port_setting;
    port_setting.tx_buffer_size = MUX_UART_TXBUF_MAX_SIZE;
    port_setting.rx_buffer_size = MUX_UART_RXBUF_MAX_SIZE;
    port_setting.tx_buffer = (uint32_t)mux_uart_txbuffer;
    port_setting.rx_buffer = (uint32_t)mux_uart_rxbuffer;
    port_setting.dev_setting.uart.uart_config.baudrate = HAL_UART_BAUDRATE_8666000;//HAL_UART_BAUDRATE_8666000;//HAL_UART_BAUDRATE_921600;//;
    port_setting.dev_setting.uart.uart_config.word_length = HAL_UART_WORD_LENGTH_8;
    port_setting.dev_setting.uart.uart_config.stop_bit = HAL_UART_STOP_BIT_1;
    port_setting.dev_setting.uart.uart_config.parity = HAL_UART_PARITY_NONE;
    port_setting.dev_setting.uart.flowcontrol_type = MUX_UART_SW_FLOWCONTROL;
    
    status = mux_init(mux_port, &port_setting, NULL);
    
    if (status != MUX_STATUS_OK) {
        TRANSMITTER_LOG_I("[DCHS][MUX UART INIT] init fail, status=%d", 1, status);
        AUDIO_ASSERT(0);
        return;
    }
    mux_uart_init_flag = true;
    TRANSMITTER_LOG_I("[DCHS][MUX UART INIT] init success, txbuffer=0x%x rxbuffer=0x%x", 2, port_setting.tx_buffer, port_setting.rx_buffer);
}

void mcu_uart_open(uart_type_t type)
{
    if(type == UART_AUDIO_CMD){
        if(audio_cmd_uart_open_flag){
            TRANSMITTER_LOG_I("[DCHS][MUX UART][OPEN] audio cmd uart already opened!",0);
            return;
        }
        mux_status_t status = mux_open(MUX_LL_UART_1, "audio_cmd", &audio_cmd_uart_handle, mcu_mux_uart_callback, NULL);
        if(status != MUX_STATUS_OK){
            audio_cmd_uart_handle = 0;
            TRANSMITTER_LOG_E("[DCHS][MUX UART][OPEN] ul uart open failed, status = %d",1,status);
            AUDIO_ASSERT(0);
            return;
        }
        audio_cmd_uart_open_flag = true;
        TRANSMITTER_LOG_I("[DCHS][MUX UART][OPEN] audio cmd uart open success, uart_handle = 0x%x",1, audio_cmd_uart_handle);
    }
}

void mcu_uart_tx(uart_type_t type, uint8_t *param_buf, uint32_t buf_size)
{
    if (type == UART_AUDIO_CMD) {
        if(!audio_cmd_uart_handle){
            TRANSMITTER_LOG_W("[DCHS][MUX UART][Tx] audio cmd uart don't open!",0);
            return;
        }
        mux_buffer_t uart_tx_buffer;
        uint32_t tx_size = 0;
        uart_tx_buffer.buf_size = buf_size;
        uart_tx_buffer.p_buf    = param_buf;
        mux_status_t status = mux_tx(audio_cmd_uart_handle, &uart_tx_buffer, 1, &tx_size);
        if (status != MUX_STATUS_OK) {
            TRANSMITTER_LOG_E("[DCHS][MUX UART] audio cmd uart tx fail, status=%d, uart_handle = 0x%x", 2, status, audio_cmd_uart_handle);
            AUDIO_ASSERT(0);
            return;
        }
        if(tx_size != buf_size) {
            TRANSMITTER_LOG_E("[DCHS][MUX UART] audio cmd uart tx fail, uart_handle = 0x%x, buf_size = %d, already send = %d", 3, audio_cmd_uart_handle, buf_size, tx_size);
            AUDIO_ASSERT(0);
            return;
        }
        TRANSMITTER_LOG_I("[DCHS][MUX UART][Tx] audio cmd uart tx success! data_size=%d",1,tx_size);
    } else if (type == UART_UL) {
        /* todo */
    } else {
        TRANSMITTER_LOG_E("[DCHS][MUX UART] don't support uart type = %d", 1, type);
    } 
}

void mcu_set_local_chip_dchs_dl_status(bool is_running)
{
    local_chip_dchs_dl_exist = is_running;
}

bool mcu_check_other_chip_dchs_dl_status(void)
{
    return other_chip_dchs_dl_exist;
}

void mcu_dchs_cosys_ctrl_cmd_execute(audio_dchs_cosys_ctrl_t  cosys_ctrl)
{
    if(cosys_ctrl.ctrl_type == AUDIO_UART_COSYS_DL_OPEN){
        memset(&transmitter_config, 0, sizeof(audio_transmitter_config_t));
        transmitter_config.scenario_type = AUDIO_TRANSMITTER_DCHS;
        transmitter_config.scenario_sub_id = AUDIO_TRANSMITTER_DCHS_UART_DL;
        transmitter_config.msg_handler = user_transmitter_callback;

        if(cosys_ctrl.ctrl_param.dchs_dl_open_param.scenario_type == AUDIO_SCENARIO_TYPE_A2DP){
            if(cosys_ctrl.ctrl_param.dchs_dl_open_param.a2dp_codec_type == BT_A2DP_CODEC_SBC){
                transmitter_config.scenario_config.dchs_config.frame_size = 128;
            }else if(cosys_ctrl.ctrl_param.dchs_dl_open_param.a2dp_codec_type == BT_A2DP_CODEC_AAC){
                transmitter_config.scenario_config.dchs_config.frame_size = 1024;
            }else if(cosys_ctrl.ctrl_param.dchs_dl_open_param.a2dp_codec_type == BT_A2DP_CODEC_VENDOR){
                transmitter_config.scenario_config.dchs_config.frame_size = (cosys_ctrl.ctrl_param.dchs_dl_open_param.sampling_rate <= HAL_AUDIO_SAMPLING_RATE_48KHZ) ? 128 : 256;
            }
        }else {
            transmitter_config.scenario_config.dchs_config.frame_size = cosys_ctrl.ctrl_param.dchs_dl_open_param.frame_size;
        }
        transmitter_config.scenario_config.dchs_config.out_interface = cosys_ctrl.ctrl_param.dchs_dl_open_param.out_interface;
        transmitter_config.scenario_config.dchs_config.sampling_rate = cosys_ctrl.ctrl_param.dchs_dl_open_param.sampling_rate;
        transmitter_config.scenario_config.dchs_config.irq_period    = cosys_ctrl.ctrl_param.dchs_dl_open_param.irq_period;
        transmitter_config.scenario_config.dchs_config.frame_number  = cosys_ctrl.ctrl_param.dchs_dl_open_param.frame_number;
        transmitter_config.scenario_config.dchs_config.format        = cosys_ctrl.ctrl_param.dchs_dl_open_param.format;
        transmitter_config.scenario_config.dchs_config.out_memory    = cosys_ctrl.ctrl_param.dchs_dl_open_param.out_memory;
        TRANSMITTER_LOG_I("[DCHS][MCU CMd Execute]: scenario type %d open config done", 1, cosys_ctrl.ctrl_param.dchs_dl_open_param.scenario_type);

    }else if(cosys_ctrl.ctrl_type == AUDIO_UART_COSYS_DL_START){ 
        transmitter_config.scenario_config.dchs_config.start_bt_clk       = cosys_ctrl.ctrl_param.dchs_dl_start_param.start_bt_clk;
        transmitter_config.scenario_config.dchs_config.start_asi          = cosys_ctrl.ctrl_param.dchs_dl_start_param.start_asi;
        transmitter_config.scenario_config.dchs_config.start_bt_intra_clk = cosys_ctrl.ctrl_param.dchs_dl_start_param.start_bt_intra_clk;
        if(dl_transmitter_id != -1){
            TRANSMITTER_LOG_W("[DCHS][MCU CMd Execute]: dl already started", 0);
            return;
        }
        if(!transmitter_config.msg_handler){
            transmitter_config.msg_handler = user_transmitter_callback;
        }
        dl_transmitter_id = audio_transmitter_init(&transmitter_config);
        if(dl_transmitter_id < 0){
            TRANSMITTER_LOG_E("[DCHS][MCU CMd Execute]: dl uart init false", 0);
            AUDIO_ASSERT(0);
        }  
        audio_transmitter_status_t status = audio_transmitter_start(dl_transmitter_id);
        if(status != AUDIO_TRANSMITTER_STATUS_SUCCESS){
            TRANSMITTER_LOG_E("[DCHS][MCU CMd Execute]: dl start false", 0);
            AUDIO_ASSERT(0);
        }
        TRANSMITTER_LOG_I("[DCHS][MCU CMd Execute] dl start success,frame_size=%d,irq_period=%d,frame_number=%d,format=%d,out_memory=%d,sampling_rate=%d,start_asi=0x%x,start_bt_clk=0x%x,start_bt_intra_clk=0x%x", 9,
                            transmitter_config.scenario_config.dchs_config.frame_size,
                            transmitter_config.scenario_config.dchs_config.irq_period,
                            transmitter_config.scenario_config.dchs_config.frame_number,
                            transmitter_config.scenario_config.dchs_config.format,
                            transmitter_config.scenario_config.dchs_config.out_memory,
                            transmitter_config.scenario_config.dchs_config.sampling_rate,
                            transmitter_config.scenario_config.dchs_config.start_asi,
                            transmitter_config.scenario_config.dchs_config.start_bt_clk,
                            transmitter_config.scenario_config.dchs_config.start_bt_intra_clk);

    }else if(cosys_ctrl.ctrl_type == AUDIO_UART_COSYS_DL_CLOSE){
        if(dl_transmitter_id < 0){
            TRANSMITTER_LOG_E("[DCHS][MCU CMd Execute]: dl uart AUDIO_UART_DL_CLOSE false, scenario don't open", 0);
            AUDIO_ASSERT(0);
        }
        TRANSMITTER_LOG_I("[DCHS][MCU CMd Execute]: dl is stopping", 0);
        audio_transmitter_status_t status = audio_transmitter_stop(dl_transmitter_id);
        if(status != AUDIO_TRANSMITTER_STATUS_SUCCESS){
            TRANSMITTER_LOG_E("[DCHS][MCU CMd Execute]: dl stop false", 0);
            AUDIO_ASSERT(0);
        }  
    }
}

void mcu_dchs_cosys_ctrl_cmd_relay(uart_cmd_type_t ctrl_type, audio_scenario_type_t scenario_type, mcu2dsp_open_param_t *open_param, mcu2dsp_start_param_t * start_param)
{
    TRANSMITTER_LOG_I("[DCHS][CMD Relay]ctrl_type:%d, scenario_type:%d",2,ctrl_type,scenario_type);
    if(ctrl_type == AUDIO_UART_COSYS_DL_OPEN){
        if (scenario_type == AUDIO_SCENARIO_TYPE_HFP_DL || scenario_type == AUDIO_SCENARIO_TYPE_A2DP || scenario_type == AUDIO_SCENARIO_TYPE_BLE_DL){ // [hard code]   
            audio_dchs_dl_open_param_t dl_open_cosys_ctrl_param;
            memset(&dl_open_cosys_ctrl_param, 0, sizeof(audio_dchs_dl_open_param_t));

            dl_open_cosys_ctrl_param.scenario_type = scenario_type;
            if(scenario_type == AUDIO_SCENARIO_TYPE_A2DP){
                bt_codec_a2dp_audio_t * codec_info = (bt_codec_a2dp_audio_t*)(&(open_param->stream_in_param.a2dp.codec_info));
                dl_open_cosys_ctrl_param.a2dp_codec_type = codec_info->codec_cap.type;
                TRANSMITTER_LOG_I("[DCHS] cpy done,a2dp codec type=%d", 1, dl_open_cosys_ctrl_param.a2dp_codec_type);
            }
            dl_open_cosys_ctrl_param.in_memory     = open_param->stream_in_param.afe.memory;
            dl_open_cosys_ctrl_param.in_interface  = open_param->stream_in_param.afe.audio_interface;
            dl_open_cosys_ctrl_param.out_memory    = open_param->stream_out_param.afe.memory;
            dl_open_cosys_ctrl_param.out_interface = open_param->stream_out_param.afe.audio_interface;
            dl_open_cosys_ctrl_param.sampling_rate = open_param->stream_out_param.afe.sampling_rate;
            dl_open_cosys_ctrl_param.frame_size    = open_param->stream_out_param.afe.frame_size;
            dl_open_cosys_ctrl_param.frame_number  = open_param->stream_out_param.afe.frame_number;
            dl_open_cosys_ctrl_param.format        = open_param->stream_out_param.afe.format;
            dl_open_cosys_ctrl_param.irq_period    = open_param->stream_out_param.afe.irq_period;
            dl_open_cosys_ctrl_param.header.ctrl_type  = ctrl_type;
            dl_open_cosys_ctrl_param.header.param_size = sizeof(audio_dchs_dl_open_param_t) - sizeof(audio_uart_cmd_header_t);
            //relay dchs dl open param to the other chip
            mcu_uart_tx(UART_AUDIO_CMD, &dl_open_cosys_ctrl_param, sizeof(audio_dchs_dl_open_param_t));
            //config local chip open param
            audio_dchs_cosys_ctrl_t  cosys_ctrl;
            cosys_ctrl.ctrl_type  = ctrl_type;
            cosys_ctrl.ctrl_param.dchs_dl_open_param = dl_open_cosys_ctrl_param;
            mcu_dchs_cosys_ctrl_cmd_execute(cosys_ctrl);
            TRANSMITTER_LOG_I("[DCHS] relay dl open param cmd success ",0);
        }
    }else if (ctrl_type == AUDIO_UART_COSYS_DL_START){
        if (scenario_type == AUDIO_SCENARIO_TYPE_HFP_DL || scenario_type == AUDIO_SCENARIO_TYPE_A2DP || scenario_type == AUDIO_SCENARIO_TYPE_BLE_DL){ // [hard code]
            audio_dchs_dl_start_param_t dl_start_cosys_ctrl_param;
            memset(&dl_start_cosys_ctrl_param, 0, sizeof(audio_dchs_dl_start_param_t));

            dl_start_cosys_ctrl_param.scenario_type      = scenario_type;
            dl_start_cosys_ctrl_param.start_bt_clk       = start_param->stream_in_param.a2dp.start_bt_clk;
            dl_start_cosys_ctrl_param.start_asi          = start_param->stream_in_param.a2dp.start_asi;
            dl_start_cosys_ctrl_param.start_bt_intra_clk = start_param->stream_in_param.a2dp.start_bt_intra_clk;
            dl_start_cosys_ctrl_param.header.ctrl_type   = ctrl_type;
            dl_start_cosys_ctrl_param.header.param_size  = sizeof(audio_dchs_dl_start_param_t) - sizeof(audio_uart_cmd_header_t);
            //relay dchs dl start param to the other chip
            mcu_uart_tx(UART_AUDIO_CMD, &dl_start_cosys_ctrl_param, sizeof(audio_dchs_dl_start_param_t));
            
            //create local chip dchs dl path
            audio_dchs_cosys_ctrl_t  cosys_ctrl;
            cosys_ctrl.ctrl_type  = ctrl_type;
            cosys_ctrl.ctrl_param.dchs_dl_start_param = dl_start_cosys_ctrl_param;
            mcu_dchs_cosys_ctrl_cmd_execute(cosys_ctrl);
            mcu_set_local_chip_dchs_dl_status(true);
        }
    }else if(ctrl_type == AUDIO_UART_COSYS_DL_CLOSE){
        if (scenario_type == AUDIO_SCENARIO_TYPE_HFP_DL || scenario_type == AUDIO_SCENARIO_TYPE_A2DP || scenario_type == AUDIO_SCENARIO_TYPE_BLE_DL){ // [hard code]
            mcu_set_local_chip_dchs_dl_status(false);

            audio_dchs_dl_close_param_t dl_close_cosys_ctrl_param;
            memset(&dl_close_cosys_ctrl_param, 0, sizeof(audio_dchs_dl_close_param_t));

            dl_close_cosys_ctrl_param.scenario_type = scenario_type;
            dl_close_cosys_ctrl_param.header.ctrl_type  = ctrl_type;
            dl_close_cosys_ctrl_param.header.param_size = sizeof(audio_dchs_dl_close_param_t) - sizeof(audio_uart_cmd_header_t);

            mcu_uart_tx(UART_AUDIO_CMD, &dl_close_cosys_ctrl_param, sizeof(audio_dchs_dl_close_param_t));
            TRANSMITTER_LOG_I("[DCHS] relay dl close param cmd success ",0);

            if(mcu_check_other_chip_dchs_dl_status()){
                TRANSMITTER_LOG_W("[DCHS] the other chip dl exist, ignore close local dl",0);
                return;
            }
            //close local chip dchs dl
            audio_dchs_cosys_ctrl_t  cosys_ctrl;
            cosys_ctrl.ctrl_type  = ctrl_type;
            cosys_ctrl.ctrl_param.dchs_dl_close_param = dl_close_cosys_ctrl_param;
            mcu_dchs_cosys_ctrl_cmd_execute(cosys_ctrl);
            TRANSMITTER_LOG_I("[DCHS] close local dl success ",0);
        }  
    }
}

void mcu_bt_lock_sleep_done(void)
{
    bt_lock_sleep_done = true;
    //TRANSMITTER_LOG_I("[DCHS] bt lock sleep done ",0);
}

#endif //(AIR_DUAL_CHIP_MIXING_MODE_ROLE_MASTER_ENABLE) || (AIR_DUAL_CHIP_MIXING_MODE_ROLE_SLAVE_ENABLE)
