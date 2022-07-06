/*
 * FreeRTOS Kernel V10.1.1
 * Copyright (C) 2018 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */


/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the ARM CM4F port.
 *----------------------------------------------------------*/

#include "hal.h"
#include "FreeRTOS.h"
#include "port_tick.h"
#include "hal_clock.h"
#include "hal_nvic.h"
#include "timers.h"
#include "hal_gpt_internal.h"
#include "hal_resource_assignment.h"
#include <assert.h>

#if configUSE_TICKLESS_IDLE == 2
#include "task.h"
#include "memory_attribute.h"
#include "hal_sleep_manager.h"
#include "hal_sleep_manager_internal.h"
#include "hal_sleep_manager_platform.h"
#include "hal_gpt.h"
#include "hal_log.h"
#include "hal_eint.h"
#include "hal_spm.h"

#endif

#ifdef AIR_ICE_DEBUG_ENABLE
#include "hal_core_status.h"
#endif

#define MaximumIdleTime 10  //ms
#define DEEP_SLEEP_SW_BACKUP_RESTORE_TIME 2
#define REMAINDER_LIMIT 100*1000 //100ms

//float RTC_Freq = 32.768; /* RTC 32.768KHz Freq*/
uint32_t RTC_Freq = 32;
uint32_t nvic_mask;
#ifdef  SLEEP_MANAGEMENT_DEBUG_ENABLE
extern uint32_t eint_get_status(uint32_t);
uint32_t wakeup_eint, wakeup_eint1, wakeup_eint2;
#endif

volatile uint32_t systick_change_period = 0, systick_change_period_dsp = 0;

//#define TICKLESS_DEEBUG_ENABLE
#ifdef  TICKLESS_DEEBUG_ENABLE
#define log_debug_tickless(_message,...) printf(_message, ##__VA_ARGS__)
#else
#define log_debug_tickless(_message,...)
#endif

static uint32_t count_idle_time;
static uint32_t count_sleep_time;
volatile uint32_t tick_remainder = 0;
int spm_latency_time_checking(void);
static uint32_t hw_latency_time_ms = 0;

#include "hal_nvic.h"
#include <assert.h>
#include <xtensa/hal.h>

GPT_REGISTER_T *os_gpt1 = OS_GPT1;
OS_GPT_REGISTER_GLOABL_T *os_gpt_glb = OS_GPTGLB;
#define OST_COUNT_CLEAR         0x1
#define OST_COUNT_START         0x1
#define OST_IRQ_ENABLE          0x1
#define OST_IRQ_FLAG_ACK        0x1
#define OST_CLOCK_32KHZ         (0x10)
#define OST_CLOCK_13MHZ         (0x00)

#if configUSE_TICKLESS_IDLE == 2
extern ATTR_TEXT_IN_IRAM hal_nvic_status_t hal_nvic_restore_interrupt_mask_special(uint32_t mask);
extern ATTR_TEXT_IN_IRAM hal_nvic_status_t hal_nvic_save_and_set_interrupt_mask_special(uint32_t *mask);
#endif

typedef enum {
    OS_GPT_TIMER_TYPE_ONE_SHOT = 0,                /**< Set the GPT oneshot mode.  */
    OS_GPT_TIMER_TYPE_REPEAT   = 1                 /**< Set the GPT repeat  mode.  */
} os_gpt_timer_type_t;

typedef enum {
    OS_GPT_CLOCK_SOURCE_32K = 0,            /**< Set the GPT clock source to 32kHz, 1 tick = 1/32768 seconds. */
    OS_GPT_CLOCK_SOURCE_1M  = 1             /**< Set the GPT clock source to 1MHz, 1 tick = 1 microsecond.*/
} os_gpt_clock_source_t;

void os_gpt_interrupt_handle(hal_nvic_irq_t irq_number);
void os_gpt_init(void *callback)
{
    hal_nvic_disable_irq(OS_GPT_IRQn);
    hal_nvic_register_isr_handler(OS_GPT_IRQn, callback);
}

void os_gpt_start(uint32_t time_out_us , os_gpt_timer_type_t timer_type , os_gpt_clock_source_t clock_source , uint32_t rtc_freq)
{
    os_gpt_timer_type_t ost_type = timer_type;
    os_gpt_clock_source_t ost_source = clock_source;
    uint32_t time_out_us_reload;

    time_out_us_reload = (ost_type == OS_GPT_TIMER_TYPE_ONE_SHOT) ? time_out_us : (time_out_us - 1);

    if ((time_out_us_reload / 1000) > HAL_GPT_MAXIMUM_MS_TIMER_TIME) {
        assert(0);
    }
    //  os_gpt1->GPT_IRQ_ACK = 0x1;                   //clear interrupt status
    os_gpt1->GPT_CON_UNION.GPT_CON |= (1 << 16);  // disabled the clock source

    /* set 13 divided with 13M source */
    if (ost_source == OS_GPT_CLOCK_SOURCE_32K) {       //set clock source
        os_gpt1->GPT_CON_UNION.GPT_CON |= (1 << 16);   // disable clock before config
        os_gpt1->GPT_CLK = 0x10;      //set 32k divide 1
        os_gpt1->GPT_CON_UNION.GPT_CON &= ~(1 << 16);   // enable clock
        //workround:
        time_out_us_reload = time_out_us_reload / 1000;
        os_gpt1->GPT_COMPARE = (uint32_t)(time_out_us_reload * 32 + (7 * time_out_us_reload) / 10 + (6 * time_out_us_reload) / 100 + (8 * time_out_us_reload) / 1000);
        (void)rtc_freq;

        //os_gpt1->GPT_COMPARE = (uint32_t)(((float)time_out_us_reload/1000.0) *rtc_freq);
        while (os_gpt_glb->OS_GPT_WCOMPSTA & (1 << 1));
        os_gpt1->GPT_CLR      = 0x1;                    // clear the count
        while (os_gpt_glb->OS_GPT_CLRSTA & (1 << 1));
    } else {
        os_gpt1->GPT_CON_UNION.GPT_CON |= (1 << 16);   // disable clock before config
        os_gpt1->GPT_CLK = 0xc;     //set  13M divide 13
        os_gpt1->GPT_CON_UNION.GPT_CON &= ~(1 << 16);   // enable clock

        os_gpt1->GPT_COMPARE = time_out_us_reload;
        while (os_gpt_glb->OS_GPT_WCOMPSTA & (1 << 1));
        os_gpt1->GPT_CLR      = 0x1;                    // clear the count
        while (os_gpt_glb->OS_GPT_CLRSTA & (1 << 1));
        (void)rtc_freq;
    }

    if (ost_type == OS_GPT_TIMER_TYPE_ONE_SHOT) {
        os_gpt1->GPT_CON_UNION.GPT_CON &= ~(3 << 8);    //set one-shot mode
        os_gpt_glb->OS_GPT_IRQMSK_DSP &= 0x1;           //DSP IRQ enable
        os_gpt_glb->OS_GPT_WAKEUPMSK_DSP &= 0x1;        //DSP wakeup enable
    } else {
        os_gpt1->GPT_CON_UNION.GPT_CON |= 0x101;        // set repeat mode
        os_gpt_glb->OS_GPT_IRQMSK_DSP &= 0x1;           //DSP IRQ enable
        os_gpt_glb->OS_GPT_WAKEUPMSK_DSP &= 0x1;        //DSP wakeup enable
    }

    /* enable IRQ */
    os_gpt1->GPT_IRQ_EN = 0x1;

    /* register and enable IRQ */
    hal_nvic_enable_irq(OS_GPT_IRQn);

    os_gpt1->GPT_CON_UNION.GPT_CON |= 0x01;
    return;
}

uint32_t os_gpt_stop()
{
    /*diable interrupt*/
    os_gpt1->GPT_IRQ_EN = ~OST_IRQ_ENABLE  ;
    /* stop timer */
    os_gpt1->GPT_CON_UNION.GPT_CON_CELLS.EN &= ~OST_COUNT_START;
    os_gpt1->GPT_IRQ_ACK = OST_IRQ_FLAG_ACK ;                    /* clean interrupt status */
    os_gpt1->GPT_CON_UNION.GPT_CON = 0;            /* disable timer     */

    //NVIC_DisableIRQ(OS_GPT_IRQn);
    hal_nvic_disable_irq(OS_GPT_IRQn);
    //NVIC_ClearPendingIRQ(OS_GPT_IRQn);
    xthal_set_intclear((0x1) << OS_GPT_IRQn);

    return os_gpt1->GPT_COUNT;
}

extern BaseType_t xPortSysTickHandler(void);
void os_gpt_interrupt_handle(hal_nvic_irq_t irq_number)
{
    (void)irq_number;

    os_gpt1->GPT_IRQ_ACK = 0x01;

    if (systick_change_period_dsp != 0) {
        systick_change_period_dsp = 0;
        os_gpt_start(portTICK_PERIOD_MS * 1000 , OS_GPT_TIMER_TYPE_REPEAT , OS_GPT_CLOCK_SOURCE_1M , RTC_Freq);
    }
    /* Run FreeRTOS tick handler*/
    xPortSysTickHandler();
}

#if configUSE_TICKLESS_IDLE == 2
ATTR_TEXT_IN_IRAM void tickless_handler(uint32_t xExpectedIdleTime)
{
    static uint32_t after_idle_time;
    static uint32_t ulCompleteTickPeriods, before_idle_time, remain, ulAST_Reload_ms, last_remain;
    static uint32_t after_sleep_time, before_sleep_time, sleep_time = 0;
    static uint32_t before_time[2], after_time[2], duration_time[2];
    static float calculation;

    hal_nvic_save_and_set_interrupt_mask_special(&nvic_mask);
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, (uint32_t *)&before_time[0]);

#ifdef AIR_ICE_DEBUG_ENABLE
    uint32_t ice_debug_value;
    ice_debug_value = *(volatile uint32_t *)HW_SYSRAM_PRIVATE_MEMORY_DSP_ICE_DEBUG_START;
    if ((ice_debug_value & DSP_ENTER_IDLE_DISADLE) != false) {
        return;
    }
#endif

    /* If a context switch is pending or a task is waiting for the scheduler
    to be unsuspended then abandon the low power entry. */
    if (eTaskConfirmSleepModeStatus() != eAbortSleep) {
        if ((xExpectedIdleTime > (MaximumIdleTime / (1000 / configTICK_RATE_HZ))) && (hal_sleep_manager_is_sleep_locked() == 0) && (((*SPM_PCM_RESERVE2 >> 31) & 0x01) != 1)) {
            /* Tickless SLEEP mode*/

            if (systick_change_period_dsp != 0) {
                systick_change_period_dsp = 0;
                tick_remainder += os_gpt_stop();
                tick_remainder += last_remain;
            } else {
                tick_remainder += os_gpt_stop();
            }

            /* Calculate total idle time to ms */
            ulAST_Reload_ms = (xExpectedIdleTime - 1) * (1000 / configTICK_RATE_HZ);
            ulAST_Reload_ms = ulAST_Reload_ms - DEEP_SLEEP_SW_BACKUP_RESTORE_TIME - hw_latency_time_ms;

            os_gpt_start(ulAST_Reload_ms * 1000 , OS_GPT_TIMER_TYPE_ONE_SHOT , OS_GPT_CLOCK_SOURCE_32K , RTC_Freq);
            hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &before_sleep_time);
            hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, (uint32_t *)&after_time[0]);

            /* Enter Sleep mode */
            hal_sleep_manager_enter_sleep_mode(HAL_SLEEP_MODE_SLEEP);

            hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, (uint32_t *)&before_time[1]);
            hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &after_sleep_time);
            /* stop GPT */
            os_gpt_stop();

            hal_gpt_get_duration_count(before_sleep_time, after_sleep_time, &sleep_time);

            sleep_time *= 1000;
            calculation = (float)sleep_time;
            calculation /= RTC_Freq;/*us*/

            sleep_time = (uint32_t)calculation;
            ulCompleteTickPeriods = (sleep_time / 1000) / ((1000 / configTICK_RATE_HZ));
            remain = sleep_time % (portTICK_PERIOD_MS * 1000);
            tick_remainder += remain;

            hal_gpt_get_duration_count(before_time[0], after_time[0], &duration_time[0]);
            hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, (uint32_t *)&after_time[1]);
            hal_gpt_get_duration_count(before_time[1], after_time[1], &duration_time[1]);
            tick_remainder += duration_time[0] + duration_time[1];

            /* Limit OS Tick Compensation Value */
            if (ulCompleteTickPeriods >= (xExpectedIdleTime)) {
                ulCompleteTickPeriods = xExpectedIdleTime;
            } else {
                if (tick_remainder >= (portTICK_PERIOD_MS * 1000)) {
                    tick_remainder -= (portTICK_PERIOD_MS * 1000);
                    ulCompleteTickPeriods++;
                }
            }

            if (tick_remainder > 1000) {
                systick_change_period_dsp = 1;
                last_remain = (tick_remainder % (portTICK_PERIOD_MS * 1000));
                tick_remainder -= last_remain;
                remain = (portTICK_PERIOD_MS * 1000) - last_remain;
                os_gpt_start(remain, OS_GPT_TIMER_TYPE_REPEAT , OS_GPT_CLOCK_SOURCE_1M , RTC_Freq);
            } else {
                systick_change_period_dsp = 0;
                os_gpt_start(portTICK_PERIOD_MS * 1000, OS_GPT_TIMER_TYPE_REPEAT , OS_GPT_CLOCK_SOURCE_1M , RTC_Freq);
            }

            if (tick_remainder >= REMAINDER_LIMIT) {
                tick_remainder = REMAINDER_LIMIT;
            }

            vTaskStepTick(ulCompleteTickPeriods);

            /* Re-enable interrupts */
            hal_nvic_restore_interrupt_mask_special(nvic_mask);

#ifdef  SLEEP_MANAGEMENT_DEBUG_ENABLE
            wakeup_eint = eint_get_status(0);
            wakeup_eint1 = eint_get_status(1);
            wakeup_eint2 = eint_get_status(2);
#endif
            //sleep_management_dump_debug_log(SLEEP_MANAGEMENT_DEBUG_LOG_DUMP);

#ifdef  SLEEP_MANAGEMENT_DEBUG_ENABLE
#ifdef  SLEEP_MANAGEMENT_DEBUG_SLEEP_WAKEUP_LOG_ENABLE
            sleep_management_dump_wakeup_source(sleep_management_dsp_status.wakeup_source, wakeup_eint);
#endif
#endif
            log_debug_tickless("\r\nEIT=%u\r\n"  , (unsigned int)xExpectedIdleTime);
            log_debug_tickless("RL=%u\r\n"       , (unsigned int)ulAST_Reload_ms);
            return;
        } else {
            /* pure IDLE mode*/
            hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &before_idle_time);
            __asm__ __volatile__(" dsync             \n"
                                 " waiti 0           \n"
                                 " isync             \n");
            hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &after_idle_time);

            /* Re-enable interrupts */
            hal_nvic_restore_interrupt_mask_special(nvic_mask);

            count_idle_time += (after_idle_time - before_idle_time) / 1000;
            return;
        }
    }
    /* Re-enable interrupts */
    hal_nvic_restore_interrupt_mask_special(nvic_mask);
}
#endif


TimerHandle_t xTimerofTest;
void log_cup_resource_callback(TimerHandle_t pxTimer);

/*
A peripheral General Purpose Timer is used for OS timer, and the interrupt is handled in GPT handler.
*/
#if configGENERATE_RUN_TIME_STATS == 1
uint32_t task_information_period = 5000;
uint32_t task_information_period_old = 0;
#endif

void _frxt_tick_timer_init(void)
{
    uint32_t freq_32K;
    int hw_latency_time_32k;

    //get rtc real frequency from share memory
    freq_32K = *((volatile uint32_t *)HW_SYSRAM_PRIVATE_MEMORY_RTC_FREQ_START);

    //printf("\r\ndsp hal_rtc_get_f32k_frequency:%d\r\n", freq_32K);
    RTC_Freq = ((float)freq_32K) / 1000;

    os_gpt_init(os_gpt_interrupt_handle);
    os_gpt_start(portTICK_PERIOD_MS * 1000 , OS_GPT_TIMER_TYPE_REPEAT , OS_GPT_CLOCK_SOURCE_1M , RTC_Freq);

    hw_latency_time_32k = spm_latency_time_checking(); //SPM Latency Time
    hw_latency_time_ms = (uint32_t)((((double)hw_latency_time_32k) * (1/32.768))) + 1;

#if configGENERATE_RUN_TIME_STATS == 1
    task_information_period_old = task_information_period;
    //xTimerofTest = xTimerCreate("TimerofTest", (task_information_period / portTICK_PERIOD_MS), pdTRUE, NULL, log_cup_resource_callback);
    //xTimerStart(xTimerofTest, 0);
#endif
}

#if configGENERATE_RUN_TIME_STATS == 1
typedef enum {
    TASK_PERIOD_CONFIG = 0xa0,
    TASK_THRESHOLD_CONFIG,
} option_selector_t;

typedef struct {
    uint32_t task_information_period;
    uint32_t task_threshold[32];
    uint32_t isEnableTaskThreshold;
    uint32_t isEnableTaskInforPeriod;
    uint32_t isEnableTaskMaxTime;
    option_selector_t option_selector;
} dsp_task_info_t;

uint32_t pxTaskMaxTimeArray[32] = {0};

dsp_task_info_t *dsp_task_info = NULL;
extern uint32_t *mcu_to_dsp_info;
void log_cup_resource_callback(TimerHandle_t pxTimer)
{
    TaskStatus_t *pxTaskStatusArray;
    UBaseType_t uxArraySize, x;
    uint32_t ulTotalTime, ulStatsAsPercentage;
    static uint32_t ulTotalTime_now = 0, ulTotalTime_last = 0;
    float Percentage;
    float sleep_percentage;
    float idle_percentage;
    float busy_percentage;
    ulTotalTime = 0;
    /* Optionally do something if the pxTimer parameter is NULL. */
    configASSERT(pxTimer);

    /* mcu have send cmd to config task proifling */
    if (mcu_to_dsp_info != NULL) {
        dsp_task_info = (dsp_task_info_t *)mcu_to_dsp_info;
        if (dsp_task_info != NULL) {
            /* disable task proifling show */
            if (dsp_task_info->isEnableTaskInforPeriod != TRUE) {
                return;
            }
            /* set task information period */
            if ( dsp_task_info->option_selector == TASK_PERIOD_CONFIG) {
                task_information_period = dsp_task_info->task_information_period;
            } else {
            }
        }
    }

    uxArraySize = uxTaskGetNumberOfTasks();
    if (uxArraySize >= 32) {
        log_hal_info("[ERROR]task maxtime is not support\r\n");
    }


    pxTaskStatusArray = pvPortMalloc(uxArraySize * sizeof(TaskStatus_t));

    if (pxTaskStatusArray == NULL) {
        return;
    }
    uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, uxArraySize, &ulTotalTime);

    ulTotalTime_now = ulTotalTime;

    if (ulTotalTime_now > ulTotalTime_last) {
        ulTotalTime = ulTotalTime_now - ulTotalTime_last;
    } else {
        ulTotalTime = ulTotalTime_now;
    }
    ulTotalTime_last = ulTotalTime_now;

    if (ulTotalTime > 0UL) {
        /*ulTotalTime is 32k counter*/
        log_hal_info("ulTotalTimeL:%lu\r\n", ulTotalTime);
        log_hal_info("----------------------DSP Dump OS Task Info-----------------------------\r\n");
        for (x = 0; x < uxArraySize; x++) {
            /* store max run time */
            if (pxTaskMaxTimeArray[x] <  pxTaskStatusArray[x].ulRunTimeCounter) {
                pxTaskMaxTimeArray[x] = pxTaskStatusArray[x].ulRunTimeCounter;
            }

            Percentage = ((float)pxTaskStatusArray[x].ulRunTimeCounter) / ((float)ulTotalTime);
            ulStatsAsPercentage = pxTaskStatusArray[x].ulRunTimeCounter / ulTotalTime;
            if ((dsp_task_info != NULL ) && (TRUE == dsp_task_info->isEnableTaskMaxTime)) {
                log_hal_info("Task[%s] State[%lu] Percentage[%lu.%lu] MinStack[%lu] RunTime[%lu]  MaxRunTime[%lu]\r\n"
                             , pxTaskStatusArray[x].pcTaskName
                             , pxTaskStatusArray[x].eCurrentState
                             , (uint32_t)(Percentage * 100)
                             , ((uint32_t)(Percentage * 1000)) % 10
                             , pxTaskStatusArray[x].usStackHighWaterMark
                             , pxTaskStatusArray[x].ulRunTimeCounter
                             , pxTaskMaxTimeArray[x]);
            } else {
                log_hal_info("Task[%s] State[%lu] Percentage[%lu.%lu] MinStack[%lu] RunTime[%lu]\r\n"
                             , pxTaskStatusArray[x].pcTaskName
                             , pxTaskStatusArray[x].eCurrentState
                             , (uint32_t)(Percentage * 100)
                             , ((uint32_t)(Percentage * 1000)) % 10
                             , pxTaskStatusArray[x].usStackHighWaterMark
                             , pxTaskStatusArray[x].ulRunTimeCounter);
            }


            if ((dsp_task_info != NULL ) &&  (TRUE == dsp_task_info->isEnableTaskThreshold)) {
                if ( (pxTaskStatusArray[x].ulRunTimeCounter >= dsp_task_info->task_threshold[x]) && (dsp_task_info->task_threshold[x] != 0) ) {
                    log_hal_info("[WARNING]:%s task has exceeded threshold,runtime:%d,threshold:%d\r\n",
                                 pxTaskStatusArray[x].pcTaskName, pxTaskStatusArray[x].ulRunTimeCounter, dsp_task_info->task_threshold[x]);
                }
            }
        }

        log_hal_info("Idle time : %d ms\r\n", count_idle_time);
        log_hal_info("Sleep time : %d ms\r\n", count_sleep_time);
        idle_percentage = ((float)count_idle_time) / task_information_period;
        sleep_percentage = ((float)count_sleep_time) / task_information_period;
        busy_percentage = 1 - idle_percentage - sleep_percentage;
        log_hal_info("Idle time percentage: [%lu]\r\n", (uint32_t)(idle_percentage * 100));
        log_hal_info("Sleep time percentage: [%lu]\r\n", (uint32_t)(sleep_percentage * 100));
        log_hal_info("Busy time percentage: [%lu]\r\n", (uint32_t)(busy_percentage * 100));
        count_idle_time = 0;
        count_sleep_time = 0;

        log_hal_info("----------------------------------------------------------------------\r\n");
        vPortFree(pxTaskStatusArray);
    } else {
        vPortFree(pxTaskStatusArray);
    }
    vTaskClearTaskRunTimeCounter();
#ifdef  SLEEP_MANAGEMENT_DEBUG_ENABLE
    sleep_management_debug_dump_lock_sleep_time();
#endif

    /* update latest period */
    if (task_information_period_old != task_information_period) {
        task_information_period_old = task_information_period;
        memset(pxTaskMaxTimeArray, 0, 32);
        /* it need change period */
        xTimerChangePeriod(xTimerofTest, (task_information_period / portTICK_PERIOD_MS), 0);
    }



}
#endif
