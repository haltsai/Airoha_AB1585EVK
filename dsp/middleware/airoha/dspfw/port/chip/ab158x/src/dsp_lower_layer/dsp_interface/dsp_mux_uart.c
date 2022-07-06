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


/******************************************************************************
 * Include
 ******************************************************************************/
#include "dsp_mux_uart.h"
#include "dsp_temp.h"
#include "dsp_dump.h"


/******************************************************************************
 * Private Macro 
 ******************************************************************************/
#define DSP_TEST_SOURCE_DEBUG         (0)
#define DSP_UART_SOURCE_ONLY_DEBUG    (0)

/******************************************************************************
 * Private Global Variables
 ******************************************************************************/
static mux_handle_t        dl_uart_handle = 0;
static mux_handle_t        ul_uart_handle = 0;
static bool                dl_uart_open_flag = false;
static bool                ul_uart_open_flag = false;
static BUFFER_INFO_PTR     dl_rx_buffer = NULL;
static BUFFER_INFO_PTR     ul_rx_buffer = NULL;
static bool                other_chip_scenario_exist_flag = false;

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

/******************************************************************************
 * Private Function Define
 ******************************************************************************/
void dsp_uart_rx(uart_type_t type, U8 * user_rx_buf,U32 buf_size)
{
    if(type == UART_DL){
        mux_status_t mux_status;
        mux_buffer_t uart_rx_buffer;
        U32 rx_size = 0;
        uart_rx_buffer.p_buf    = user_rx_buf;
        uart_rx_buffer.buf_size = buf_size;
        mux_status = mux_rx(dl_uart_handle, &uart_rx_buffer, &rx_size);
        if (mux_status != MUX_STATUS_OK) {
            DSP_MW_LOG_E("[DCHS][uart callback] mux uart rx fail,status=%d", 1, mux_status);
            AUDIO_ASSERT(0);
            return;
        }
        if(rx_size != buf_size){
            DSP_MW_LOG_E("[DCHS][uart callback] mux uart rx fail,already_rx_size=%d,need_rx_size=%d", 2, rx_size, buf_size);
            AUDIO_ASSERT(0);
            return;
        }
    }
    //LOG_AUDIO_DUMP(user_rx_buf,rx_size,AUDIO_DCHS_UART_DL_SOURCE);
    //DSP_MW_LOG_I("[DCHS][uart callback] rx data size: %d", 1, rx_size);
}


ATTR_TEXT_IN_IRAM void dsp_mux_uart_callback(mux_handle_t handle, mux_event_t event, uint32_t data_len, void *user_data)
{
    DSP_MW_LOG_I("[DCHS][uart callback] enter rx data api", 0);
    UNUSED(user_data);
    uint32_t need_rx_size = 0;
    switch (event) {
        case MUX_EVENT_READY_TO_READ:
            DSP_MW_LOG_I("[DCHS][uart callback] enter rx data event api 0x%x 0x%x", 2, dl_uart_handle, handle);
            if(handle == dl_uart_handle){
                #if DSP_UART_SOURCE_ONLY_DEBUG
                static U8 test_buff[1024];
                dsp_uart_rx(UART_DL,test_buff,data_len);
                return;
                #endif
                if(!dl_rx_buffer){
                    DSP_MW_LOG_E("[MUX UART][uart callback] no register dl_rx_buffer",0);
                    AUDIO_ASSERT(0);
                    return;
                }
                if(dl_rx_buffer->bBufferIsFull){
                    DSP_MW_LOG_W("[DCHS][uart callback] uart dl_rx_buffer is Full!", 0);
                    return;
                }
                U32 writeOffset = dl_rx_buffer->WriteOffset;
                U32 readOffset  = dl_rx_buffer->ReadOffset;
                U32 length      = dl_rx_buffer->length;
                U32 available_length = (writeOffset >= readOffset) ? (length + readOffset - writeOffset) : (readOffset - writeOffset);
                //DSP_MW_LOG_W("[DCHS][uart callback] available_length=%d!", 1,available_length);
                need_rx_size = MIN(available_length, data_len);
                if (length - writeOffset >= need_rx_size){
                    dsp_uart_rx(UART_DL, dl_rx_buffer->startaddr[0] + writeOffset, need_rx_size);
                }else{
                    //read part1
                    dsp_uart_rx(UART_DL, dl_rx_buffer->startaddr[0] + writeOffset, length - writeOffset);
                    //read part2
                    dsp_uart_rx(UART_DL, dl_rx_buffer->startaddr[0], need_rx_size - (length - writeOffset));
                }
                dl_rx_buffer->WriteOffset = (dl_rx_buffer->WriteOffset + need_rx_size) % (dl_rx_buffer->length);
                if (dl_rx_buffer->WriteOffset == dl_rx_buffer->ReadOffset) {
                    dl_rx_buffer->bBufferIsFull = TRUE;
                }
                //DSP_MW_LOG_I("[DCHS][uart callback] dl_rx_buffer->WriteOffset=%d", 1,dl_rx_buffer->WriteOffset);
            }
            break;
        case MUX_EVENT_READY_TO_WRITE:
            DSP_MW_LOG_I("[DCHS][uart callback] MUX_EVENT_READY_TO_WRITE", 0);
            break;
        default:
            DSP_MW_LOG_I("[DCHS][uart callback] no event: %d", 1, event);
    }
}

#if DSP_TEST_SOURCE_DEBUG
#include "hal_gpt.h"
U8 test_data_buf[512];
static uint32_t timer_handle;
static void dsp_uart_source_timer_callback_test(void *user_data)
{
    uint32_t need_rx_size = 0;
    if(!dl_rx_buffer){
        DSP_MW_LOG_E("[MUX UART][uart callback] no register dl_rx_buffer",0);
        AUDIO_ASSERT(0);
        return;
    }
    U32 writeOffset = dl_rx_buffer->WriteOffset;
    U32 readOffset  = dl_rx_buffer->ReadOffset;
    U32 length      = dl_rx_buffer->length;
    U32 available_length = (writeOffset >= readOffset) ? (length + readOffset - writeOffset) : (readOffset - writeOffset);
    if(dl_rx_buffer->bBufferIsFull){
        DSP_MW_LOG_W("[DCHS][uart callback] bBufferIsFull ", 0);
        hal_gpt_sw_start_timer_ms(timer_handle, 3, dsp_uart_source_timer_callback_test, NULL);
        return;
    }
    need_rx_size = MIN(available_length, 512);//[hard code]
    if (length - writeOffset >= need_rx_size){
        memcpy(dl_rx_buffer->startaddr[0] + writeOffset,test_data_buf, need_rx_size);
    }else{
        //read part1
        memcpy(dl_rx_buffer->startaddr[0] + writeOffset, test_data_buf,length - writeOffset);
        //read part2
        memcpy( dl_rx_buffer->startaddr[0],(test_data_buf+(length - writeOffset)) ,need_rx_size - (length - writeOffset));
    }
    dl_rx_buffer->WriteOffset = (dl_rx_buffer->WriteOffset + need_rx_size) % (dl_rx_buffer->length);
    if (dl_rx_buffer->WriteOffset == dl_rx_buffer->ReadOffset) {
        dl_rx_buffer->bBufferIsFull = TRUE;
    }
    //DSP_MW_LOG_W("[DCHS][uart callback] dl_rx_buffer->WriteOffset=%d", 1,dl_rx_buffer->WriteOffset);
    hal_gpt_sw_start_timer_ms(timer_handle, 3, dsp_uart_source_timer_callback_test, NULL);
}
#endif

/******************************************************************************
 * Public Function Define
 ******************************************************************************/
void dsp_set_other_chip_scenario_exist(bool is_relaying)
{
    other_chip_scenario_exist_flag = is_relaying;
}

void dsp_dchs_mcu2dsp_msg_sync_callback(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    UNUSED(ack);
    audio_dchs_mcu2dsp_cosys_sync_t * sync_msg = (audio_dchs_mcu2dsp_cosys_sync_t *)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);
    DSP_MW_LOG_I("[DCHS][DSP] rx dchs dl ccni msg sync,sync type=%d,exist=%d",2,sync_msg->sync_type,sync_msg->sync_msg.dual_chip_dl_sync.other_chip_dl_exist);
    if(sync_msg->sync_type == DCHS_DL_OTHER_CHIP_DL_EXIST){
        bool other_chip_dl_exist = sync_msg->sync_msg.dual_chip_dl_sync.other_chip_dl_exist;
        dsp_set_other_chip_scenario_exist(other_chip_dl_exist);
        DSP_MW_LOG_I("[DCHS][mcu2dsp msg sync] other_chip_dl_exist = %d", 1, other_chip_dl_exist);
    }
}

uint32_t dsp_query_uart_rx_buf_remain_size(uart_type_t type)
{
    if(type == UART_DL){
        mux_ctrl_para_t rx_param;
        mux_user_control(dl_uart_handle,MUX_CMD_GET_LL_USER_RX_BUFFER_DATA_SIZE,&rx_param);
        return rx_param.mux_ll_user_rx_data_len;
    }
}


bool dsp_check_other_chip_scenario_exist()
{
    return other_chip_scenario_exist_flag;
}

void dsp_uart_rx_buffer_register(uart_type_t type, BUFFER_INFO_PTR rx_buffer)
{
    if(type == UART_DL){
        dl_rx_buffer = rx_buffer;
    }else if(type == UART_UL){
        ul_rx_buffer = rx_buffer;
    }else {
        DSP_MW_LOG_E("[MUX UART][rx buffer register] don't support uart type = %d", 1, type);
    }
    DSP_MW_LOG_I("[MUX UART][rx buffer register] success, buffer len= %d,addr=0x%x", 2, dl_rx_buffer->length,dl_rx_buffer->startaddr[0]);
}



void dsp_uart_open(uart_type_t type)
{
    if(type == UART_DL){
        if(dl_uart_open_flag){
            DSP_MW_LOG_I("[MUX UART][OPEN] dl uart already opened!",0);
            return;
        }
        mux_status_t status = mux_open(MUX_LL_UART_1, "R2L_DL", &dl_uart_handle, dsp_mux_uart_callback, NULL);
        if(status != MUX_STATUS_OK){
            dl_uart_handle = 0;
            DSP_MW_LOG_E("[MUX UART][OPEN] ul uart open failed, status = %d",1,status);
            AUDIO_ASSERT(0);
            return;
        }
        dl_uart_open_flag = true;
        DSP_MW_LOG_I("[MUX UART][OPEN] dl uart open success, uart_handle = 0x%x",1, dl_uart_handle);

        #if DSP_TEST_SOURCE_DEBUG
        U32 i;
        for(i = 0;i < 512;i++){
            test_data_buf[i] = i%0xFF;
        }
        hal_gpt_status_t gpt_status = hal_gpt_sw_get_timer(&timer_handle);
        if(gpt_status != HAL_GPT_STATUS_OK){
            DSP_MW_LOG_I("[MUX UART][OPEN][TEST] timer get fail:%d",1,gpt_status);
        }
        gpt_status = hal_gpt_sw_start_timer_ms(timer_handle, 3, dsp_uart_source_timer_callback_test, NULL);
        if(gpt_status != HAL_GPT_STATUS_OK){
            DSP_MW_LOG_I("[MUX UART][OPEN][TEST] timer start fail:%d",1,gpt_status);
        }
        #endif
    }
    if(type == UART_UL){
        //to do
    }
}

void dsp_uart_tx(uart_type_t type, uint8_t *user_tx_buffer, uint32_t buf_size)
{
    if (type == UART_DL) {
        if(!dl_uart_handle){
            DSP_MW_LOG_W("[MUX UART][Tx] mux dl uart don't open!",0);
            return;
        }
        mux_buffer_t uart_tx_buffer;
        uint32_t tx_size = 0;
        uart_tx_buffer.buf_size = buf_size;
        uart_tx_buffer.p_buf    = user_tx_buffer;
        mux_status_t status = mux_tx(dl_uart_handle, &uart_tx_buffer, 1, &tx_size);
        if (status != MUX_STATUS_OK) {
            DSP_MW_LOG_E("[MUX UART] dl uart tx fail, status=%d, uart_handle = 0x%x", 2, status, dl_uart_handle);
            AUDIO_ASSERT(0);
            return;
        }
        if(tx_size != buf_size) {
            DSP_MW_LOG_E("[MUX UART] dl uart tx fail, uart_handle = 0x%x, buf_size = %d, already send = %d", 3, dl_uart_handle, buf_size, tx_size);
            AUDIO_ASSERT(0);
            return;
        }
        //DSP_MW_LOG_W("[MUX UART][Tx] mux uart tx success! data_size=%d",1,tx_size);
    } else if (type == UART_UL) {
        /* todo */
    } else {
        DSP_MW_LOG_E("[MUX UART] don't support uart type = %d", 1, type);
    } 
}