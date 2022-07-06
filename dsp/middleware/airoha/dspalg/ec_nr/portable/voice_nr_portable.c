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

#include <string.h>
#include <xtensa/tie/xt_hifi2.h>
#include <xtensa/hal.h>
#include "assert.h"
#include "hal.h"
#include "dsp_feature_interface.h"
#include "xt_library_loader.h"
#include "preloader_pisplit.h"
#include "voice_nr_portable.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "dsp_feature_interface.h"

#if PIC_LOGPRINT
#define IGO_PRINTF_IMPORT_API printf
#else
#define IGO_PRINTF_IMPORT_API fake_printf
#endif

static void *g_exnr_import_parameters[1] = {IGO_PRINTF_IMPORT_API};

static void ecnr_default_function_parameter(void)
{
    DSP_MW_LOG_E("[DSP][VOICE_NR] ECNR export function is not initial", 0);
    AUDIO_ASSERT(0);
}

#if defined(AIR_3RD_PARTY_NR_ENABLE)

#define TX_EQ_EXPORT_PARAM_NUM (4)

static SemaphoreHandle_t g_tx_eq_load_lib_xSemaphore = NULL;
static preloader_pisplit_handle_t g_tx_eq_pisplit_library_handle = NULL;
static uint32_t g_tx_eq_lib_loaded_counter = 0;

void *g_tx_eq_export_parameters[TX_EQ_EXPORT_PARAM_NUM] = {
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter
};

static void tx_eq_load_library_callback(preloader_pisplit_handle_t handle)
{
    uint32_t i;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint32_t *p_export_parameters;

    p_export_parameters = preloader_pisplit_get_export_parameter(handle, g_exnr_import_parameters);
    if (p_export_parameters == NULL) {
        DSP_MW_LOG_E("[DSP][VOICE_NR] TXEQ lib export APIs is NULL", 0);
    } else {
        for (i = 0; i < TX_EQ_EXPORT_PARAM_NUM; i++) {
            g_tx_eq_export_parameters[i] = (void *)p_export_parameters[i];
        }
        xSemaphoreGiveFromISR(g_tx_eq_load_lib_xSemaphore, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void tx_eq_library_load(void *code_address, void *data_address, uint32_t *dram_pic_usage)
{
    preloader_pisplit_error_handling_t status;

    if (g_tx_eq_load_lib_xSemaphore == NULL) {
        g_tx_eq_load_lib_xSemaphore = xSemaphoreCreateBinary();
        AUDIO_ASSERT(g_tx_eq_load_lib_xSemaphore != NULL);
    }

    if (g_tx_eq_pisplit_library_handle == NULL) {
        status = preloader_pisplit_get_handle(&g_tx_eq_pisplit_library_handle, &pisplit_fir_eq, tx_eq_load_library_callback);
        AUDIO_ASSERT(status == PRELOADER_PISPLIT_XTLIB_NO_ERR);
    }

    g_tx_eq_lib_loaded_counter++;
    if (g_tx_eq_lib_loaded_counter == 1) {
#ifdef AIR_DSP_MEMORY_REGION_ENABLE
        status = preloader_pisplit_load_v1(g_tx_eq_pisplit_library_handle, code_address, data_address);
#else
        status = preloader_pisplit_load(g_tx_eq_pisplit_library_handle, PRELOADER_I_HIGH_PERFORMANCE, PRELOADER_D_HIGH_PERFORMANCE);
#endif
        if (status != PRELOADER_PISPLIT_XTLIB_NO_ERR) {
            preloader_pisplit_dump_pic_memory_heap_info(PRELOADER_I_HIGH_PERFORMANCE);
            preloader_pisplit_dump_pic_memory_heap_info(PRELOADER_I_LOW_PERFORMANCE);
            preloader_pisplit_dump_pic_memory_heap_info(PRELOADER_D_HIGH_PERFORMANCE);
            preloader_pisplit_dump_pic_memory_heap_info(PRELOADER_D_LOW_PERFORMANCE);
            AUDIO_ASSERT(0);
        }
        AUDIO_ASSERT(xSemaphoreTake(g_tx_eq_load_lib_xSemaphore, portMAX_DELAY) == pdTRUE);
    }

    vSemaphoreDelete(g_tx_eq_load_lib_xSemaphore);
    g_tx_eq_load_lib_xSemaphore = NULL;
#ifdef AIR_DSP_MEMORY_REGION_ENABLE
    preloader_pisplit_get_library_code_and_data_size(g_tx_eq_pisplit_library_handle, NULL, dram_pic_usage);
#else
    UNUSED(code_address);
    UNUSED(data_address);
    UNUSED(dram_pic_usage);
#endif

    DSP_MW_LOG_I("[DSP][VOICE_NR] TXEQ library load ++ finish", 0);

}

void tx_eq_library_unload(void)
{
    uint32_t i;
    preloader_pisplit_error_handling_t status;

    g_tx_eq_lib_loaded_counter--;
    if (g_tx_eq_lib_loaded_counter == 0) {

#ifdef AIR_DSP_MEMORY_REGION_ENABLE
        status = preloader_pisplit_unload_v1(g_tx_eq_pisplit_library_handle);
#else
        status = preloader_pisplit_unload(g_tx_eq_pisplit_library_handle);
#endif
        AUDIO_ASSERT(status == PRELOADER_PISPLIT_XTLIB_NO_ERR);
        status = preloader_pisplit_release_handle(g_tx_eq_pisplit_library_handle);
        AUDIO_ASSERT(status == PRELOADER_PISPLIT_XTLIB_NO_ERR);
        for (i = 0; i < TX_EQ_EXPORT_PARAM_NUM; i++) {
            g_tx_eq_export_parameters[i] = ecnr_default_function_parameter;
        }
        g_tx_eq_pisplit_library_handle = NULL;
    }

    DSP_MW_LOG_I("[DSP][VOICE_NR] TXEQ library unload -- finish", 0);
}

#if defined(AIR_AI_NR_PREMIUM_ENABLE) || defined(AIR_AI_NR_PREMIUM_INEAR_ENABLE)

#ifdef AIR_BTA_IC_PREMIUM_G2
#if defined(AIR_AI_NR_PREMIUM_ENABLE)
static void *g_igo_txnr_import_parameters[] = {IGO_PRINTF_IMPORT_API, hal_trng_init, hal_trng_get_generated_random_number, hal_trng_deinit};
#endif
#if defined(AIR_AI_NR_PREMIUM_INEAR_ENABLE)
static void *g_igo_txnr_import_parameters[] = {IGO_PRINTF_IMPORT_API, hal_trng_init, hal_trng_get_generated_random_number, hal_trng_deinit};
#endif
#elif defined(AIR_BTA_IC_PREMIUM_G3)
#if defined(AIR_AI_NR_PREMIUM_ENABLE)
static void *g_igo_txnr_import_parameters[] = {IGO_PRINTF_IMPORT_API, hal_trng_init, hal_trng_get_generated_random_number, hal_trng_deinit};
#elif defined(AIR_AI_NR_PREMIUM_INEAR_ENABLE)
static void *g_igo_txnr_import_parameters[] = {IGO_PRINTF_IMPORT_API};
#endif
#endif
static SemaphoreHandle_t g_igo_txnr_load_lib_xSemaphore = NULL;
static preloader_pisplit_handle_t g_igo_txnr_pisplit_library_handle = NULL;
static uint32_t g_igo_txnr_lib_loaded_counter = 0;

#ifdef AIR_BTA_IC_PREMIUM_G2
#define IGO_TXNR_EXPORT_PARAM_NUM (3)
void *g_igo_txnr_export_parameters[IGO_TXNR_EXPORT_PARAM_NUM] = {
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
};
#elif defined(AIR_BTA_IC_PREMIUM_G3)
#define IGO_TXNR_EXPORT_PARAM_NUM (2)
void *g_igo_txnr_export_parameters[IGO_TXNR_EXPORT_PARAM_NUM] = {
    ecnr_default_function_parameter,
    ecnr_default_function_parameter
};
#endif

#ifdef AIR_DSP_MEMORY_REGION_ENABLE
static uint32_t g_igo_txnr_pic_code_size;
static uint32_t g_igo_txnr_pic_data_size;
#endif

static void igo_txnr_load_library_callback(preloader_pisplit_handle_t handle)
{
    uint32_t i;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint32_t *p_export_parameters;
    p_export_parameters = preloader_pisplit_get_export_parameter(handle, g_igo_txnr_import_parameters);

    if (p_export_parameters == NULL) {
        DSP_MW_LOG_E("[DSP][VOICE_NR] IGO lib export APIs is NULL", 0);
    } else {
        for (i = 0; i < IGO_TXNR_EXPORT_PARAM_NUM; i++) {
            g_igo_txnr_export_parameters[i] = (void *)p_export_parameters[i];
        }

        xSemaphoreGiveFromISR(g_igo_txnr_load_lib_xSemaphore, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void igo_txnr_library_load(void *code_address, void *data_address)
{
    preloader_pisplit_error_handling_t status;

    if (g_igo_txnr_load_lib_xSemaphore == NULL) {
        g_igo_txnr_load_lib_xSemaphore = xSemaphoreCreateBinary();
        AUDIO_ASSERT(g_igo_txnr_load_lib_xSemaphore != NULL);
    }

    if (g_igo_txnr_pisplit_library_handle == NULL) {
        status = preloader_pisplit_get_handle(&g_igo_txnr_pisplit_library_handle, &pisplit_igo_txnr, igo_txnr_load_library_callback);
        AUDIO_ASSERT(status == PRELOADER_PISPLIT_XTLIB_NO_ERR);
    }

    g_igo_txnr_lib_loaded_counter++;
    if (g_igo_txnr_lib_loaded_counter == 1) {
#ifdef AIR_DSP_MEMORY_REGION_ENABLE
        status = preloader_pisplit_load_v1(g_igo_txnr_pisplit_library_handle, code_address, data_address);
#else
        status = preloader_pisplit_load(g_igo_txnr_pisplit_library_handle, PRELOADER_I_HIGH_PERFORMANCE, PRELOADER_D_HIGH_PERFORMANCE);
#endif
        if (status != PRELOADER_PISPLIT_XTLIB_NO_ERR) {
            preloader_pisplit_dump_pic_memory_heap_info(PRELOADER_I_HIGH_PERFORMANCE);
            preloader_pisplit_dump_pic_memory_heap_info(PRELOADER_I_LOW_PERFORMANCE);
            preloader_pisplit_dump_pic_memory_heap_info(PRELOADER_D_HIGH_PERFORMANCE);
            preloader_pisplit_dump_pic_memory_heap_info(PRELOADER_D_LOW_PERFORMANCE);
            AUDIO_ASSERT(0);
        }
        AUDIO_ASSERT(xSemaphoreTake(g_igo_txnr_load_lib_xSemaphore, portMAX_DELAY) == pdTRUE);
    }

#ifdef AIR_DSP_MEMORY_REGION_ENABLE
    preloader_pisplit_get_library_code_and_data_size(g_igo_txnr_pisplit_library_handle, &g_igo_txnr_pic_code_size, &g_igo_txnr_pic_data_size);
#else
    UNUSED(code_address);
    UNUSED(data_address);
#endif

    vSemaphoreDelete(g_igo_txnr_load_lib_xSemaphore);
    g_igo_txnr_load_lib_xSemaphore = NULL;

    DSP_MW_LOG_I("[DSP][VOICE_NR] IGO library load ++ finish", 0);
}

void igo_txnr_library_unload(void)
{
    uint32_t i;
    preloader_pisplit_error_handling_t status;

    g_igo_txnr_lib_loaded_counter--;
    if (g_igo_txnr_lib_loaded_counter == 0) {
#ifdef AIR_DSP_MEMORY_REGION_ENABLE
        status = preloader_pisplit_unload_v1(g_igo_txnr_pisplit_library_handle);
#else
        status = preloader_pisplit_unload(g_igo_txnr_pisplit_library_handle);
#endif
        AUDIO_ASSERT(status == PRELOADER_PISPLIT_XTLIB_NO_ERR);
        for (i = 0; i < IGO_TXNR_EXPORT_PARAM_NUM; i++) {
            g_igo_txnr_export_parameters[i] = ecnr_default_function_parameter;
        }
    }

    DSP_MW_LOG_I("[DSP][VOICE_NR] IGO library unload -- finish", 0);
}
#endif
#endif /* defined(AIR_3RD_PARTY_NR_ENABLE) && defined(AIR_AI_NR_PREMIUM_ENABLE) */

#ifdef AIR_BTA_IC_PREMIUM_G2
#ifdef AIR_ECNR_1MIC_INEAR_ENABLE
#define ECNR_DECODER_EXPORT_PARAM_NUM (17)
#define ECNR_PIC_LIB pisplit_ecnr_inear
#elif defined(AIR_ECNR_2MIC_INEAR_ENABLE)
#define ECNR_DECODER_EXPORT_PARAM_NUM (18)
#define ECNR_PIC_LIB pisplit_ecnr_inear_v2
#elif defined(AIR_3RD_PARTY_NR_ENABLE)
#define ECNR_DECODER_EXPORT_PARAM_NUM (17)
#define ECNR_PIC_LIB pisplit_ec_rxnr
#elif defined(AIR_ECNR_1_OR_2_MIC_ENABLE)
#define ECNR_DECODER_EXPORT_PARAM_NUM (14)
#define ECNR_PIC_LIB pisplit_ecnr
#endif
#elif defined(AIR_BTA_IC_PREMIUM_G3)
#if (defined(AIR_ECNR_1_OR_2_MIC_ENABLE) || defined(AIR_ECNR_1MIC_INEAR_ENABLE) || defined(AIR_ECNR_2MIC_INEAR_ENABLE))
#define ECNR_DECODER_EXPORT_PARAM_NUM (26)
#define ECNR_PIC_LIB pisplit_ecnr_inear_v2_swb
#elif defined(AIR_3RD_PARTY_NR_ENABLE)
#define ECNR_DECODER_EXPORT_PARAM_NUM (26)
#define ECNR_PIC_LIB pisplit_ec_rxnr_swb
#endif
#endif

static SemaphoreHandle_t g_ecnr_load_lib_xSemaphore = NULL;
static preloader_pisplit_handle_t g_ecnr_pisplit_library_handle = NULL;
static uint32_t g_ecnr_lib_loaded_counter = 0;

#ifdef AIR_BTA_IC_PREMIUM_G2
void *g_ecnr_decoder_export_parameters[ECNR_DECODER_EXPORT_PARAM_NUM] = {
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
#if defined(AIR_3RD_PARTY_NR_ENABLE)
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
#elif defined(AIR_ECNR_1MIC_INEAR_ENABLE)
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
#elif defined(AIR_ECNR_2MIC_INEAR_ENABLE)
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
#endif
};
#elif defined(AIR_BTA_IC_PREMIUM_G3)
void *g_ecnr_decoder_export_parameters[ECNR_DECODER_EXPORT_PARAM_NUM] = {
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
#if (defined(AIR_ECNR_1_OR_2_MIC_ENABLE) || defined(AIR_ECNR_1MIC_INEAR_ENABLE) || defined(AIR_ECNR_2MIC_INEAR_ENABLE))
    ecnr_default_function_parameter,
#elif defined(AIR_3RD_PARTY_NR_ENABLE)
    ecnr_default_function_parameter,
#endif
};
#endif

static void ecnr_load_library_callback(preloader_pisplit_handle_t handle)
{
    uint32_t i;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint32_t *p_export_parameters;

    p_export_parameters = preloader_pisplit_get_export_parameter(handle, g_exnr_import_parameters);
    if (p_export_parameters == NULL) {
        DSP_MW_LOG_E("[DSP][VOICE_NR] ECNR lib export APIs is NULL", 0);
    } else {
        for (i = 0; i < ECNR_DECODER_EXPORT_PARAM_NUM; i++) {
            g_ecnr_decoder_export_parameters[i] = (void *)p_export_parameters[i];
        }
        xSemaphoreGiveFromISR(g_ecnr_load_lib_xSemaphore, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

uint32_t ecnr_library_load(void *code_address, void *data_address, uint32_t *dram_pic_usage)
{
    preloader_pisplit_error_handling_t status;

#if defined(AIR_3RD_PARTY_NR_ENABLE)
#if defined(AIR_AI_NR_PREMIUM_ENABLE) || defined(AIR_AI_NR_PREMIUM_INEAR_ENABLE)
    igo_txnr_library_load(code_address, data_address); /* Load IGO before EC since the memory size of IGO is larger */
#endif
#endif

    if (g_ecnr_load_lib_xSemaphore == NULL) {
        g_ecnr_load_lib_xSemaphore = xSemaphoreCreateBinary();
        AUDIO_ASSERT(g_ecnr_load_lib_xSemaphore != NULL);
    }

    if (g_ecnr_pisplit_library_handle == NULL) {
        status = preloader_pisplit_get_handle(&g_ecnr_pisplit_library_handle, &ECNR_PIC_LIB, ecnr_load_library_callback);
        AUDIO_ASSERT(status == PRELOADER_PISPLIT_XTLIB_NO_ERR);
    }

    g_ecnr_lib_loaded_counter++;
    if (g_ecnr_lib_loaded_counter == 1) {

#ifdef AIR_DSP_MEMORY_REGION_ENABLE
#if defined(AIR_3RD_PARTY_NR_ENABLE) && (defined(AIR_AI_NR_PREMIUM_ENABLE) || defined(AIR_AI_NR_PREMIUM_INEAR_ENABLE))
        status = preloader_pisplit_load_v1(g_ecnr_pisplit_library_handle, (void *)(((uint32_t)code_address + g_igo_txnr_pic_code_size+31)&(~31)), (void *)(((uint32_t)data_address + g_igo_txnr_pic_data_size+31)&(~31)));
#else
        status = preloader_pisplit_load_v1(g_ecnr_pisplit_library_handle, code_address, data_address);
#endif
#else
        status = preloader_pisplit_load(g_ecnr_pisplit_library_handle, PRELOADER_I_HIGH_PERFORMANCE, PRELOADER_D_HIGH_PERFORMANCE);
#endif
        if (status != PRELOADER_PISPLIT_XTLIB_NO_ERR) {
            preloader_pisplit_dump_pic_memory_heap_info(PRELOADER_I_HIGH_PERFORMANCE);
            preloader_pisplit_dump_pic_memory_heap_info(PRELOADER_I_LOW_PERFORMANCE);
            preloader_pisplit_dump_pic_memory_heap_info(PRELOADER_D_HIGH_PERFORMANCE);
            preloader_pisplit_dump_pic_memory_heap_info(PRELOADER_D_LOW_PERFORMANCE);
            AUDIO_ASSERT(0);
        }
        AUDIO_ASSERT(xSemaphoreTake(g_ecnr_load_lib_xSemaphore, portMAX_DELAY) == pdTRUE);
    }

    vSemaphoreDelete(g_ecnr_load_lib_xSemaphore);
    g_ecnr_load_lib_xSemaphore = NULL;
#ifdef AIR_DSP_MEMORY_REGION_ENABLE
#if defined(AIR_3RD_PARTY_NR_ENABLE) && (defined(AIR_AI_NR_PREMIUM_ENABLE) || defined(AIR_AI_NR_PREMIUM_INEAR_ENABLE))
    preloader_pisplit_get_library_code_and_data_size(g_ecnr_pisplit_library_handle, NULL, dram_pic_usage);
    *dram_pic_usage += g_igo_txnr_pic_data_size;
#else
    preloader_pisplit_get_library_code_and_data_size(g_ecnr_pisplit_library_handle, NULL, dram_pic_usage);
#endif
#else
    UNUSED(code_address);
    UNUSED(data_address);
    UNUSED(dram_pic_usage);
#endif

    DSP_MW_LOG_I("[DSP][VOICE_NR] ECNR library load ++ finish", 0);
    return g_ecnr_lib_loaded_counter;

}

uint32_t ecnr_library_unload(void)
{
    uint32_t i;
    preloader_pisplit_error_handling_t status;

#if defined(AIR_3RD_PARTY_NR_ENABLE)
#if defined(AIR_AI_NR_PREMIUM_ENABLE) || defined(AIR_AI_NR_PREMIUM_INEAR_ENABLE)
    igo_txnr_library_unload();
#endif
#endif

    g_ecnr_lib_loaded_counter--;
    if (g_ecnr_lib_loaded_counter == 0) {

#ifdef AIR_DSP_MEMORY_REGION_ENABLE
        status = preloader_pisplit_unload_v1(g_ecnr_pisplit_library_handle);
#else
        status = preloader_pisplit_unload(g_ecnr_pisplit_library_handle);
#endif
        AUDIO_ASSERT(status == PRELOADER_PISPLIT_XTLIB_NO_ERR);
        for (i = 0; i < ECNR_DECODER_EXPORT_PARAM_NUM; i++) {
            g_ecnr_decoder_export_parameters[i] = ecnr_default_function_parameter;
        }
    }

    DSP_MW_LOG_I("[DSP][VOICE_NR] ECNR library unload -- finish", 0);
    return g_ecnr_lib_loaded_counter;
}

#if defined(AIR_3RD_PARTY_NR_ENABLE)
#if defined(AIR_ECNR_PREV_PART_ENABLE)

#define ECNR_PREV_DECODER_EXPORT_PARAM_NUM (13)
#define ECNR_PREV_PIC_LIB pisplit_ec120

static SemaphoreHandle_t g_ecnr_prev_load_lib_xSemaphore = NULL;
static preloader_pisplit_handle_t g_ecnr_prev_pisplit_library_handle = NULL;
static uint32_t g_ecnr_prev_lib_loaded_counter = 0;

void *g_ecnr_prev_decoder_export_parameters[ECNR_PREV_DECODER_EXPORT_PARAM_NUM] = {
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
};

static void ecnr_prev_load_library_callback(preloader_pisplit_handle_t handle)
{
    uint32_t i;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint32_t *p_export_parameters;

    p_export_parameters = preloader_pisplit_get_export_parameter(handle, g_exnr_import_parameters);
    if (p_export_parameters == NULL) {
        DSP_MW_LOG_E("[DSP][VOICE_NR] ECNR lib export APIs is NULL", 0);
    } else {
        for (i = 0; i < ECNR_PREV_DECODER_EXPORT_PARAM_NUM; i++) {
            g_ecnr_prev_decoder_export_parameters[i] = (void *)p_export_parameters[i];
        }
        xSemaphoreGiveFromISR(g_ecnr_prev_load_lib_xSemaphore, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

uint32_t ecnr_prev_part_library_load(void *code_address, void *data_address, uint32_t *dram_pic_usage)
{
    preloader_pisplit_error_handling_t status;

    if (g_ecnr_prev_load_lib_xSemaphore == NULL) {
        g_ecnr_prev_load_lib_xSemaphore = xSemaphoreCreateBinary();
        if (g_ecnr_prev_load_lib_xSemaphore == NULL) {
            assert(0);
        }
    }
    if (g_ecnr_prev_pisplit_library_handle == NULL) {
        if (PRELOADER_PISPLIT_XTLIB_NO_ERR != preloader_pisplit_get_handle(&g_ecnr_prev_pisplit_library_handle, &ECNR_PREV_PIC_LIB, ecnr_prev_load_library_callback)) {
            DSP_MW_LOG_E("[DSP][AEC/NR] pisplit_ec120 preloader_pisplit_get_handle() error!!!!", 0);
            assert(0);
        }
    }
    if (g_ecnr_prev_lib_loaded_counter == 0) {
#ifdef AIR_DSP_MEMORY_REGION_ENABLE
        status = preloader_pisplit_load_v1(g_ecnr_prev_pisplit_library_handle, code_address, data_address);
#else
        status = preloader_pisplit_load(g_ecnr_prev_pisplit_library_handle, PRELOADER_I_HIGH_PERFORMANCE, PRELOADER_D_HIGH_PERFORMANCE);
#endif
        if (status != PRELOADER_PISPLIT_XTLIB_NO_ERR) {
            DSP_MW_LOG_E("[DSP][AEC/NR] pisplit_ec120 preloader_pisplit_load() error!!!!", 0);
            preloader_pisplit_dump_pic_memory_heap_info(PRELOADER_I_HIGH_PERFORMANCE);
            preloader_pisplit_dump_pic_memory_heap_info(PRELOADER_I_LOW_PERFORMANCE);
            preloader_pisplit_dump_pic_memory_heap_info(PRELOADER_D_HIGH_PERFORMANCE);
            preloader_pisplit_dump_pic_memory_heap_info(PRELOADER_D_LOW_PERFORMANCE);
            assert(0);
        }
        g_ecnr_prev_lib_loaded_counter = 1;
        DSP_MW_LOG_I("[DSP][AEC/NR] ec120_library_load finish", 0);
        if (xSemaphoreTake(g_ecnr_prev_load_lib_xSemaphore, portMAX_DELAY) != pdTRUE) {
            assert(0);
        }
    } else {
        g_ecnr_prev_lib_loaded_counter++;
        DSP_MW_LOG_I("[DSP][AEC/NR] ec120_library_load preloader_pisplit_load() had been loaded!!!", 0);
    }

#ifdef AIR_DSP_MEMORY_REGION_ENABLE
    preloader_pisplit_get_library_code_and_data_size(g_ecnr_prev_pisplit_library_handle, NULL, dram_pic_usage);
#else
    UNUSED(code_address);
    UNUSED(data_address);
    UNUSED(dram_pic_usage);
#endif
    return g_ecnr_prev_lib_loaded_counter;
}

uint32_t ecnr_prev_part_library_unload(void)
{
    uint32_t i;
    preloader_pisplit_error_handling_t status;

    if (g_ecnr_prev_lib_loaded_counter == 1) {
#ifdef AIR_DSP_MEMORY_REGION_ENABLE
        status = preloader_pisplit_unload_v1(g_ecnr_prev_pisplit_library_handle);
#else
        status = preloader_pisplit_unload(g_ecnr_prev_pisplit_library_handle);
#endif
        if (status != PRELOADER_PISPLIT_XTLIB_NO_ERR) {
            DSP_MW_LOG_E("[DSP][AEC/NR] pisplit_ec120 preloader_pisplit_unload() error!!!!", 0);
            assert(0);
        }
        for (i = 0; i < ECNR_PREV_DECODER_EXPORT_PARAM_NUM; i++) {
            g_ecnr_prev_decoder_export_parameters[i] = ecnr_default_function_parameter;
        }
        g_ecnr_prev_lib_loaded_counter = 0;
        DSP_MW_LOG_I("[DSP][AEC/NR] ec120_library_unload finish", 0);
    } else {
        g_ecnr_prev_lib_loaded_counter--;
    }
    return g_ecnr_prev_lib_loaded_counter;
}
#endif
#endif

#if defined(AIR_3RD_PARTY_NR_ENABLE)
#if defined(AIR_ECNR_POST_PART_ENABLE)

#define ECNR_POST_DECODER_EXPORT_PARAM_NUM (3)
#define ECNR_POST_PIC_LIB pisplit_post_ec

static SemaphoreHandle_t g_ecnr_post_load_lib_xSemaphore = NULL;
static preloader_pisplit_handle_t g_ecnr_post_pisplit_library_handle = NULL;
static uint32_t g_ecnr_post_lib_loaded_counter = 0;

void *g_ecnr_post_decoder_export_parameters[ECNR_POST_DECODER_EXPORT_PARAM_NUM] = {
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
    ecnr_default_function_parameter,
};

static void ecnr_post_load_library_callback(preloader_pisplit_handle_t handle)
{
    uint32_t i;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint32_t *p_export_parameters;

    p_export_parameters = preloader_pisplit_get_export_parameter(handle, g_exnr_import_parameters);
    if (p_export_parameters == NULL) {
        DSP_MW_LOG_E("[DSP][VOICE_NR] ECNR lib export APIs is NULL", 0);
    } else {
        for (i = 0; i < ECNR_POST_DECODER_EXPORT_PARAM_NUM; i++) {
            g_ecnr_post_decoder_export_parameters[i] = (void *)p_export_parameters[i];
        }
        xSemaphoreGiveFromISR(g_ecnr_post_load_lib_xSemaphore, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

uint32_t ecnr_post_ec_library_load(void *code_address, void *data_address, uint32_t *dram_pic_usage)
{
    preloader_pisplit_error_handling_t status;

#if defined(AIR_AI_NR_PREMIUM_ENABLE) || defined(AIR_AI_NR_PREMIUM_INEAR_ENABLE)
    /* Load IGO before EC since the memory size of IGO is larger */
    igo_txnr_library_load(code_address, data_address);
#endif

    if (g_ecnr_post_load_lib_xSemaphore == NULL) {
        g_ecnr_post_load_lib_xSemaphore = xSemaphoreCreateBinary();
        if (g_ecnr_post_load_lib_xSemaphore == NULL) {
            assert(0);
        }
    }
    if (g_ecnr_post_pisplit_library_handle == NULL) {
        if (preloader_pisplit_get_handle(&g_ecnr_post_pisplit_library_handle, &ECNR_POST_PIC_LIB, ecnr_post_load_library_callback) != PRELOADER_PISPLIT_XTLIB_NO_ERR) {
            DSP_MW_LOG_E("pisplit_ecnr preloader_pisplit_get_handle() error!!!!", 0);
            assert(0);
        }
    }
    if (g_ecnr_post_lib_loaded_counter == 0) {
#ifdef AIR_DSP_MEMORY_REGION_ENABLE
#if defined(AIR_3RD_PARTY_NR_ENABLE) && (defined(AIR_AI_NR_PREMIUM_ENABLE) || defined(AIR_AI_NR_PREMIUM_INEAR_ENABLE))
        status = preloader_pisplit_load_v1(g_ecnr_post_pisplit_library_handle, (void *)((uint32_t)code_address + g_igo_txnr_pic_code_size), (void *)((uint32_t)data_address + g_igo_txnr_pic_data_size));
#else
        status = preloader_pisplit_load_v1(g_ecnr_post_pisplit_library_handle, code_address, data_address);
#endif
#else
        status = preloader_pisplit_load(g_ecnr_post_pisplit_library_handle, PRELOADER_I_HIGH_PERFORMANCE, PRELOADER_D_HIGH_PERFORMANCE);
#endif
        if (status != PRELOADER_PISPLIT_XTLIB_NO_ERR) {
            DSP_MW_LOG_E("pisplit_ecnr preloader_pisplit_load() error!!!!", 0);
            preloader_pisplit_dump_pic_memory_heap_info(PRELOADER_I_HIGH_PERFORMANCE);
            preloader_pisplit_dump_pic_memory_heap_info(PRELOADER_I_LOW_PERFORMANCE);
            preloader_pisplit_dump_pic_memory_heap_info(PRELOADER_D_HIGH_PERFORMANCE);
            preloader_pisplit_dump_pic_memory_heap_info(PRELOADER_D_LOW_PERFORMANCE);
            assert(0);
        }
        g_ecnr_post_lib_loaded_counter = 1;
        DSP_MW_LOG_I("ecnr_library_load finish", 0);
        if (xSemaphoreTake(g_ecnr_post_load_lib_xSemaphore, portMAX_DELAY) != pdTRUE) {
            assert(0);
        }
    } else {
        g_ecnr_post_lib_loaded_counter++;
    }

#ifdef AIR_DSP_MEMORY_REGION_ENABLE
#if defined(AIR_3RD_PARTY_NR_ENABLE) && (defined(AIR_AI_NR_PREMIUM_ENABLE) || defined(AIR_AI_NR_PREMIUM_INEAR_ENABLE))
    preloader_pisplit_get_library_code_and_data_size(g_ecnr_post_pisplit_library_handle, NULL, dram_pic_usage);
    *dram_pic_usage += g_igo_txnr_pic_data_size;
#else
    preloader_pisplit_get_library_code_and_data_size(g_ecnr_post_pisplit_library_handle, NULL, dram_pic_usage);
#endif
#else
    UNUSED(code_address);
    UNUSED(data_address);
    UNUSED(dram_pic_usage);
#endif

    vSemaphoreDelete(g_ecnr_post_load_lib_xSemaphore);
    g_ecnr_post_load_lib_xSemaphore = NULL;
    return g_ecnr_post_lib_loaded_counter;
}

uint32_t ecnr_post_ec_library_unload(void)
{
    uint32_t i;
    preloader_pisplit_error_handling_t status;

#if defined(AIR_AI_NR_PREMIUM_ENABLE) || defined(AIR_AI_NR_PREMIUM_INEAR_ENABLE)
    igo_txnr_library_unload();
#endif

    if (g_ecnr_post_lib_loaded_counter == 1) {
#ifdef AIR_DSP_MEMORY_REGION_ENABLE
        status = preloader_pisplit_unload_v1(g_ecnr_post_pisplit_library_handle);
#else
        status = preloader_pisplit_unload(g_ecnr_post_pisplit_library_handle);
#endif
        if (status != PRELOADER_PISPLIT_XTLIB_NO_ERR) {
            DSP_MW_LOG_E("pisplit_ecnr preloader_pisplit_unload() error!!!!", 0);
            assert(0);
        }
        for (i = 0; i < ECNR_POST_DECODER_EXPORT_PARAM_NUM; i++) {
            g_ecnr_post_decoder_export_parameters[i] = ecnr_default_function_parameter;
        }
        g_ecnr_post_lib_loaded_counter = 0;
        DSP_MW_LOG_I("ecnr_library_unload finish", 0);

    } else {
        g_ecnr_post_lib_loaded_counter--;
    }
    return g_ecnr_post_lib_loaded_counter;
}
#endif
#endif

