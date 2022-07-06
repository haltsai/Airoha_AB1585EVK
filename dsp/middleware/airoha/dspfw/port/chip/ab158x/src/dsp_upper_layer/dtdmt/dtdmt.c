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

#include "config.h"
#include "dsp_task.h"
#include "dsp_interface.h"
#include "dsp_memory.h"
#include "transform.h"
#include "audio_config.h"
#include "stream_audio.h"
#include "stream.h"
#include "dtdmt.h"
#include "dsp_feature_interface.h"
#include "dsp_update_para.h"



/******************************************************************************
 * Function Declaration
 ******************************************************************************/
VOID                DSP_TDM_Task(VOID);
VOID                DTDMT_DefaultEntry(VOID);
VOID                DTDMT_TestEntry(VOID);
VOID                DTDMT_StartEntry(VOID);
VOID                DTDMT_ProcessEntry(VOID);
VOID                DTDMPT_DeInitEntry(VOID);
#if DspIpcEnable
VOID                DTDMT_CmdHdlr(DSP_CMD_PTR_t DspCmdPtr);
#endif
VOID                DTDMT_Init(VOID);
VOID                DTDMT_DeInit(VOID *para);
STATIC INLINE VOID  DTDMT_Process(VOID);
VOID                DTDMT_StreamingInit(VOID);
BOOL                DTDMT_ChkCallbackStatus(VOID);
BOOL                DTDMT_ChkCallbackStatusDisable(VOID);
VOID                DTDMT_SuspendRequest(VOID *para);



/******************************************************************************
 * External Function Prototypes
 ******************************************************************************/


/******************************************************************************
 * Type Definitions
 ******************************************************************************/
#if  DspIpcEnable
typedef VOID (*DTDMT_CMD_HDLR)(DSP_CMD_PTR_t DspCmdPtr);
#endif
typedef VOID (*DTDMT_ENTRY)(VOID);


/******************************************************************************
 * Constants
 ******************************************************************************/
#define NO_OF_TDM_TASK_HANDLER (sizeof(DspTDMTaskHdlr)/sizeof(DspTDMTaskHdlr[0]))

#ifdef MTK_MULTI_MIC_STREAM_ENABLE
#define NO_OF_TDM_STREAM         4
#else
#define NO_OF_TDM_STREAM         1
#endif

#if  DspIpcEnable
DTDMT_CMD_HDLR CODE DspTDMTaskHdlr[] = {
    NULL,
};
#endif
/******************************************************************************
 * Variables
 ******************************************************************************/
STATIC DTDMT_ENTRY DTDMT_Entry;

DSP_STREAMING_PARA TDMStreaming[NO_OF_TDM_STREAM];


/**
 * DSP_TDM_Task
 *
 * Main Process for DSP TDM task application
 *
 */
VOID DSP_TDM_Task(VOID)
{
#if  DspIpcEnable
    DSP_CMD_PTR_t DspCmdPtr;
#endif
    /* Do Initialization */
    DTDMT_Init();

    //DAVT_3wire();
    while (1) {
#if  DspIpcEnable
        if ((DspCmdPtr = osTaskGetIPCMsg()) != NULL) {
            DTDMT_CmdHdlr(DspCmdPtr);
        }
#endif
        DTDMT_Entry();

        PL_CRITICAL(DTDMT_SuspendRequest, NULL);

        portYIELD();
    }
}

/**
 * DTDMT_DefaultEntry
 *
 * Default Entry for DTDMT
 *
 */
VOID DTDMT_DefaultEntry(VOID)
{
    if (!DTDMT_ChkCallbackStatusDisable()) {
        DTDMT_Entry = DTDMT_ProcessEntry;
    }
}


/**
 * DTDMT_TestEntry
 *
 * Test Entry for DTDMT
 *
 */
VOID DTDMT_TestEntry(VOID)
{
    return;
}

/**
 * DTDMT_StartEntry
 *
 * Start Entry for DTDMT
 *
 */
VOID DTDMT_StartEntry(VOID)
{
    DTDMT_Process();
    if (DTDMT_ChkCallbackStatus()) {
        DTDMT_Entry = DTDMT_ProcessEntry;
    }

}


/**
 * DTDMT_ProcessEntry
 *
 * Active Entry for DTDMT background process
 *
 *
 */
VOID DTDMT_ProcessEntry(VOID)
{
#if 0
    if (DTDMT_ChkCallbackStatusDisable()) {
        DTDMT_Entry = DTDMT_DeInitEntry;
    } else
#endif
        DTDMT_Process();
}

/**
 * DTDMT_DeInitEntry
 *
 * Initialization Entry for DTDMT
 *
 */
VOID DTDMT_DeInitEntry(VOID)
{
    PL_CRITICAL(DTDMT_DeInit, NULL);
}

#if  DspIpcEnable

/**
 * DTDMT_CmdHdlr
 *
 * DTDMT Command Handler handles all Commands towards DTDMT task.
 *
 */
VOID DTDMT_CmdHdlr(DSP_CMD_PTR_t DspCmdPtr)
{
    DSP_CMD_MSG_t DspMsg = DspCmdPtr->DspMsg;
    switch (DspMsg) {
        case DSP_MSG_START_LINE:
            DTDMT_Entry = DTDMT_TestEntry;
            break;
        case DSP_UPDATE_PARAMETER:
            DSP_UpdateStreamingPara(&DspCmdPtr->DspCmdPara.UpdatePara);
            break;
        default:
            break;
    }

    OSMEM_Put(DspCmdPtr);
}
#endif
/**
 * DTDMT_Init
 *
 * Initialization for DSP_TDM_Task
 *
 */
VOID DTDMT_Init(VOID)
{
    DSPMEM_Init(DTDM_TASK_ID);
    DTDMT_Entry = DTDMT_DefaultEntry;
    DSP_Callback_StreamingInit(&TDMStreaming[0], NO_OF_TDM_STREAM, DTDM_TASK_ID);
}

/**
 * DTDMT_DeInit
 *
 * De-Initialization for TDMT
 *
 */
VOID DTDMT_DeInit(VOID *para)
{
    UNUSED(para);
    DSPMEM_Flush(DTDM_TASK_ID);
    DSP_Callback_StreamingInit(&TDMStreaming[0], NO_OF_TDM_STREAM, DTDM_TASK_ID);
    DTDMT_Entry = DTDMT_DefaultEntry;
}


/**
 * DTDMT_Process
 *
 * DTDMT background process
 *
 *
 */
STATIC INLINE VOID DTDMT_Process(VOID)
{
    U8 i;
    uint32_t start_time, end_time;
    for (i = 0 ; i < NO_OF_TDM_STREAM ; i++) {
        hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &start_time);
        if (TDMStreaming[i].streamingStatus == STREAMING_END) {
            StreamCloseAll(TDMStreaming[i].source->transform, InstantCloseMode);
        } else {
            DSP_Callback_Processing(&TDMStreaming[i]);
        }
        hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &end_time);
        if ((uint32_t)(end_time - start_time) > DTDM_TASK_MAX_RUNTIME) {
            DSP_MW_LOG_W("[DTDM_TASK] task process time:%d us", 1, (uint32_t)(end_time - start_time));
        }
    }
}


/**
 * DTDMT_StreamingConfig
 *
 * Configuration for DSP_TDM_Task streaming
 *
 */
TaskHandle_t  DTDMT_StreamingConfig(DSP_CALLBACK_STREAM_CONFIG_PTR stream_config_ptr)
{
    printf("enter DTDMT_StreamingConfig");
    stream_config_ptr->stream_ptr = (DSP_STREAMING_PARA_PTR)&TDMStreaming[0];
    stream_config_ptr->stream_number = NO_OF_TDM_STREAM;
    stream_config_ptr->task = DTDM_TASK_ID;
    return DSP_Callback_StreamConfig(stream_config_ptr);
}

/**
 * DTDMT_Callback_Get
 *
 * Get DSP_TDM_Task callback ptr
 *
 */
DSP_CALLBACK_PTR DTDMT_Callback_Get(SOURCE source, SINK sink)
{
    U8 i;
    for (i = 0 ; i < NO_OF_TDM_STREAM ; i++) {
        if ((TDMStreaming[i].streamingStatus != STREAMING_DISABLE) &&
            (TDMStreaming[i].source == source) &&
            (TDMStreaming[i].sink == sink)) {
            return &TDMStreaming[i].callback;
        }
    }
    return NULL;
}

/**
 * DTDMT_ChkCallbackStatus
 *
 * Whether all DSP_TDM_Task callback status SUSPEND/DISABLE
 *
 */
BOOL DTDMT_ChkCallbackStatus(VOID)
{
    U8 i;
    for (i = 0 ; i < NO_OF_TDM_STREAM ; i++) {
        if (((TDMStreaming[i].callback.Status != CALLBACK_SUSPEND) &&
             (TDMStreaming[i].callback.Status != CALLBACK_DISABLE) &&
             (TDMStreaming[i].callback.Status != CALLBACK_WAITEND)) ||
            (TDMStreaming[i].streamingStatus == STREAMING_END)) {
            return FALSE;
        }
    }
    return TRUE;
}

/**
 * DTDMT_ChkCallbackStatusDisable
 *
 * Whether all DSP_TDM_Task callback status DISABLE
 *
 */
BOOL DTDMT_ChkCallbackStatusDisable(VOID)
{
    U8 i;
    for (i = 0 ; i < NO_OF_TDM_STREAM ; i++) {
        if (TDMStreaming[i].callback.Status != CALLBACK_DISABLE) {
            return FALSE;
        }
    }
    return TRUE;
}

/**
 * DTDMT_SuspendRequest
 *
 * DTDMT Suspend Request depend on callback status
 *
 */
VOID DTDMT_SuspendRequest(VOID *para)
{
    UNUSED(para);
    if ((DTDMT_Entry != DTDMT_StartEntry) && (DTDMT_Entry != DTDMT_DeInitEntry) &&
        DTDMT_ChkCallbackStatus()) {
        vTaskSuspend(DTDM_TASK_ID);
    }
}


