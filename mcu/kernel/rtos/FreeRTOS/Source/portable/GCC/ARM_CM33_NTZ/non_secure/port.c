/*
 * FreeRTOS Kernel V10.4.5
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
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
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers. That should only be done when
 * task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "memory_attribute.h"
#include "hal.h"
#ifdef MTK_SWLA_ENABLE
#include "swla.h"
#endif /* MTK_SWLA_ENABLE */

/* MPU wrappers includes. */
#include "mpu_wrappers.h"

/* Portasm includes. */
#include "portasm.h"

#if ( configENABLE_TRUSTZONE == 1 )
    /* Secure components includes. */
    #include "secure_context.h"
    #include "secure_init.h"
#endif /* configENABLE_TRUSTZONE */

#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE
#ifdef MTK_SYSTEM_HANG_TRACER_ENABLE
#include "systemhang_tracer.h"
#endif /* MTK_SYSTEM_HANG_TRACER_ENABLE */
#ifdef MTK_MEMORY_MONITOR_ENABLE
#include "memory_monitor.h"
#endif /* MTK_MEMORY_MONITOR_ENABLE */

#ifdef SYSTEM_DAEMON_TASK_ENABLE
#include "system_daemon.h"
#endif /* SYSTEM_DAEMON_TASK_ENABLE */

#include "syslog.h"
log_create_module(os, PRINT_LEVEL_INFO);
/**
 * The FreeRTOS Cortex M33 port can be configured to run on the Secure Side only
 * i.e. the processor boots as secure and never jumps to the non-secure side.
 * The Trust Zone support in the port must be disabled in order to run FreeRTOS
 * on the secure side. The following are the valid configuration seetings:
 *
 * 1. Run FreeRTOS on the Secure Side:
 *		configRUN_FREERTOS_SECURE_ONLY = 1 and configENABLE_TRUSTZONE = 0
 *
 * 2. Run FreeRTOS on the Non-Secure Side with Secure Side function call support:
 *		configRUN_FREERTOS_SECURE_ONLY = 0 and configENABLE_TRUSTZONE = 1
 *
 * 3. Run FreeRTOS on the Non-Secure Side only i.e. no Secure Side function call support:
 *		configRUN_FREERTOS_SECURE_ONLY = 0 and configENABLE_TRUSTZONE = 0
 */
#if( ( configRUN_FREERTOS_SECURE_ONLY == 1 ) && ( configENABLE_TRUSTZONE == 1 ) )
	#error TrustZone needs to be disabled in order to run FreeRTOS on the Secure Side.
#endif
/*-----------------------------------------------------------*/

/**
 * @brief Constants required to manipulate the NVIC.
 */
#define portNVIC_SYSTICK_CTRL_REG             ( *( ( volatile uint32_t * ) 0xe000e010 ) )
#define portNVIC_SYSTICK_LOAD_REG             ( *( ( volatile uint32_t * ) 0xe000e014 ) )
#define portNVIC_SYSTICK_CURRENT_VALUE_REG    ( *( ( volatile uint32_t * ) 0xe000e018 ) )
#define portNVIC_INT_CTRL					( ( volatile uint32_t * ) 0xe000ed04 )
#define portNVIC_SHPR3_REG                    ( *( ( volatile uint32_t * ) 0xe000ed20 ) )
#define portNVIC_SYSTICK_ENABLE_BIT           ( 1UL << 0UL )
#define portNVIC_SYSTICK_INT_BIT              ( 1UL << 1UL )
#define portNVIC_SYSTICK_COUNT_FLAG_BIT       ( 1UL << 16UL )
#define portNVIC_PENDSVSET					( 0x10000000 )
#define portMIN_INTERRUPT_PRIORITY			( 255UL )
#define portNVIC_PENDSV_PRI                   ( portMIN_INTERRUPT_PRIORITY << 16UL )
#define portNVIC_SYSTICK_PRI                  ( portMIN_INTERRUPT_PRIORITY << 24UL )
#ifndef configSYSTICK_CLOCK_HZ
    #define configSYSTICK_CLOCK_HZ            configCPU_CLOCK_HZ
    /* Ensure the SysTick is clocked at the same frequency as the core. */
    #define portNVIC_SYSTICK_CLK_BIT          ( 1UL << 2UL )
#else

/* The way the SysTick is clocked is not modified in case it is not the
 * same a the core. */
    #define portNVIC_SYSTICK_CLK_BIT    ( 0 )
#endif

#define portVECTACTIVE_MASK					( 0xFFUL )
/*-----------------------------------------------------------*/

/**
 * @brief Constants required to manipulate the SCB.
 */
#define portSCB_SYS_HANDLER_CTRL_STATE_REG    ( *( volatile uint32_t * ) 0xe000ed24 )
#define portSCB_MEM_FAULT_ENABLE_BIT          ( 1UL << 16UL )
/*-----------------------------------------------------------*/

/**
 * @brief Constants required to manipulate the FPU.
 */
#define portCPACR               ( ( volatile uint32_t * ) 0xe000ed88 )              /* Coprocessor Access Control Register. */
#define portCPACR_CP10_VALUE    ( 3UL )
#define portCPACR_CP11_VALUE    portCPACR_CP10_VALUE
#define portCPACR_CP10_POS      ( 20UL )
#define portCPACR_CP11_POS      ( 22UL )

#define portFPCCR               ( ( volatile uint32_t * ) 0xe000ef34 )              /* Floating Point Context Control Register. */
#define portFPCCR_ASPEN_POS     ( 31UL )
#define portFPCCR_ASPEN_MASK    ( 1UL << portFPCCR_ASPEN_POS )
#define portFPCCR_LSPEN_POS     ( 30UL )
#define portFPCCR_LSPEN_MASK    ( 1UL << portFPCCR_LSPEN_POS )
/*-----------------------------------------------------------*/

/**
 * @brief Constants required to manipulate the MPU.
 */
#define portMPU_TYPE_REG                      ( *( ( volatile uint32_t * ) 0xe000ed90 ) )
#define portMPU_CTRL_REG                      ( *( ( volatile uint32_t * ) 0xe000ed94 ) )
#define portMPU_RNR_REG                       ( *( ( volatile uint32_t * ) 0xe000ed98 ) )

#define portMPU_RBAR_REG                      ( *( ( volatile uint32_t * ) 0xe000ed9c ) )
#define portMPU_RLAR_REG                      ( *( ( volatile uint32_t * ) 0xe000eda0 ) )

#define portMPU_RBAR_A1_REG                   ( *( ( volatile uint32_t * ) 0xe000eda4 ) )
#define portMPU_RLAR_A1_REG                   ( *( ( volatile uint32_t * ) 0xe000eda8 ) )

#define portMPU_RBAR_A2_REG                   ( *( ( volatile uint32_t * ) 0xe000edac ) )
#define portMPU_RLAR_A2_REG                   ( *( ( volatile uint32_t * ) 0xe000edb0 ) )

#define portMPU_RBAR_A3_REG                   ( *( ( volatile uint32_t * ) 0xe000edb4 ) )
#define portMPU_RLAR_A3_REG                   ( *( ( volatile uint32_t * ) 0xe000edb8 ) )

#define portMPU_MAIR0_REG                     ( *( ( volatile uint32_t * ) 0xe000edc0 ) )
#define portMPU_MAIR1_REG                     ( *( ( volatile uint32_t * ) 0xe000edc4 ) )

#define portMPU_RBAR_ADDRESS_MASK             ( 0xffffffe0 ) /* Must be 32-byte aligned. */
#define portMPU_RLAR_ADDRESS_MASK             ( 0xffffffe0 ) /* Must be 32-byte aligned. */

#define portMPU_MAIR_ATTR0_POS                ( 0UL )
#define portMPU_MAIR_ATTR0_MASK               ( 0x000000ff )

#define portMPU_MAIR_ATTR1_POS                ( 8UL )
#define portMPU_MAIR_ATTR1_MASK               ( 0x0000ff00 )

#define portMPU_MAIR_ATTR2_POS                ( 16UL )
#define portMPU_MAIR_ATTR2_MASK               ( 0x00ff0000 )

#define portMPU_MAIR_ATTR3_POS                ( 24UL )
#define portMPU_MAIR_ATTR3_MASK               ( 0xff000000 )

#define portMPU_MAIR_ATTR4_POS                ( 0UL )
#define portMPU_MAIR_ATTR4_MASK               ( 0x000000ff )

#define portMPU_MAIR_ATTR5_POS                ( 8UL )
#define portMPU_MAIR_ATTR5_MASK               ( 0x0000ff00 )

#define portMPU_MAIR_ATTR6_POS                ( 16UL )
#define portMPU_MAIR_ATTR6_MASK               ( 0x00ff0000 )

#define portMPU_MAIR_ATTR7_POS                ( 24UL )
#define portMPU_MAIR_ATTR7_MASK               ( 0xff000000 )

#define portMPU_RLAR_ATTR_INDEX0              ( 0UL << 1UL )
#define portMPU_RLAR_ATTR_INDEX1              ( 1UL << 1UL )
#define portMPU_RLAR_ATTR_INDEX2              ( 2UL << 1UL )
#define portMPU_RLAR_ATTR_INDEX3              ( 3UL << 1UL )
#define portMPU_RLAR_ATTR_INDEX4              ( 4UL << 1UL )
#define portMPU_RLAR_ATTR_INDEX5              ( 5UL << 1UL )
#define portMPU_RLAR_ATTR_INDEX6              ( 6UL << 1UL )
#define portMPU_RLAR_ATTR_INDEX7              ( 7UL << 1UL )

#define portMPU_RLAR_REGION_ENABLE            ( 1UL )

/* Enable privileged access to unmapped region. */
#define portMPU_PRIV_BACKGROUND_ENABLE_BIT    ( 1UL << 2UL )

/* Enable MPU. */
#define portMPU_ENABLE_BIT                    ( 1UL << 0UL )

/* Expected value of the portMPU_TYPE register. */
#define portEXPECTED_MPU_TYPE_VALUE           ( 8UL << 8UL ) /* 8 regions, unified. */
/*-----------------------------------------------------------*/

/**
 * @brief The maximum 24-bit number.
 *
 * It is needed because the systick is a 24-bit counter.
 */
#define portMAX_24_BIT_NUMBER       ( 0xffffffUL )

/**
 * @brief A fiddle factor to estimate the number of SysTick counts that would
 * have occurred while the SysTick counter is stopped during tickless idle
 * calculations.
 */
#define portMISSED_COUNTS_FACTOR    ( 45UL )
/*-----------------------------------------------------------*/

/**
 * @brief Constants required to set up the initial stack.
 */
#define portINITIAL_XPSR    ( 0x01000000 )

#if ( configRUN_FREERTOS_SECURE_ONLY == 1 )

/**
 * @brief Initial EXC_RETURN value.
 *
 *     FF         FF         FF         FD
 * 1111 1111  1111 1111  1111 1111  1111 1101
 *
 * Bit[6] - 1 --> The exception was taken from the Secure state.
 * Bit[5] - 1 --> Do not skip stacking of additional state context.
 * Bit[4] - 1 --> The PE did not allocate space on the stack for FP context.
 * Bit[3] - 1 --> Return to the Thread mode.
 * Bit[2] - 1 --> Restore registers from the process stack.
 * Bit[1] - 0 --> Reserved, 0.
 * Bit[0] - 1 --> The exception was taken to the Secure state.
 */
    #define portINITIAL_EXC_RETURN    ( 0xfffffffd )
#else

/**
 * @brief Initial EXC_RETURN value.
 *
 *     FF         FF         FF         BC
 * 1111 1111  1111 1111  1111 1111  1011 1100
 *
 * Bit[6] - 0 --> The exception was taken from the Non-Secure state.
 * Bit[5] - 1 --> Do not skip stacking of additional state context.
 * Bit[4] - 1 --> The PE did not allocate space on the stack for FP context.
 * Bit[3] - 1 --> Return to the Thread mode.
 * Bit[2] - 1 --> Restore registers from the process stack.
 * Bit[1] - 0 --> Reserved, 0.
 * Bit[0] - 0 --> The exception was taken to the Non-Secure state.
 */
    #define portINITIAL_EXC_RETURN    ( 0xffffffbc )
#endif /* configRUN_FREERTOS_SECURE_ONLY */

/**
 * @brief CONTROL register privileged bit mask.
 *
 * Bit[0] in CONTROL register tells the privilege:
 *  Bit[0] = 0 ==> The task is privileged.
 *  Bit[0] = 1 ==> The task is not privileged.
 */
#define portCONTROL_PRIVILEGED_MASK         ( 1UL << 0UL )

/**
 * @brief Initial CONTROL register values.
 */
#define portINITIAL_CONTROL_UNPRIVILEGED    ( 0x3 )
#define portINITIAL_CONTROL_PRIVILEGED      ( 0x2 )

/**
 * @brief Let the user override the pre-loading of the initial LR with the
 * address of prvTaskExitError() in case it messes up unwinding of the stack
 * in the debugger.
 */
#ifdef configTASK_RETURN_ADDRESS
    #define portTASK_RETURN_ADDRESS    configTASK_RETURN_ADDRESS
#else
    #define portTASK_RETURN_ADDRESS    prvTaskExitError
#endif

/**
 * @brief If portPRELOAD_REGISTERS then registers will be given an initial value
 * when a task is created. This helps in debugging at the cost of code size.
 */
#define portPRELOAD_REGISTERS    1

/**
 * @brief A task is created without a secure context, and must call
 * portALLOCATE_SECURE_CONTEXT() to give itself a secure context before it makes
 * any secure calls.
 */
#define portNO_SECURE_CONTEXT    0
/*-----------------------------------------------------------*/

/**
 * @brief Used to catch tasks that attempt to return from their implementing
 * function.
 */
static void prvTaskExitError( void );

#if ( configENABLE_MPU == 1 )

/**
 * @brief Setup the Memory Protection Unit (MPU).
 */
    static void prvSetupMPU( void ) PRIVILEGED_FUNCTION;
#endif /* configENABLE_MPU */

#if ( configENABLE_FPU == 1 )

/**
 * @brief Setup the Floating Point Unit (FPU).
 */
    static void prvSetupFPU( void ) PRIVILEGED_FUNCTION;
#endif /* configENABLE_FPU */

/**
 * @brief Setup the timer to generate the tick interrupts.
 *
 * The implementation in this file is weak to allow application writers to
 * change the timer used to generate the tick interrupt.
 */
void vPortSetupTimerInterrupt( void ) PRIVILEGED_FUNCTION;

/**
 * @brief Checks whether the current execution context is interrupt.
 *
 * @return pdTRUE if the current execution context is interrupt, pdFALSE
 * otherwise.
 */
BaseType_t xPortIsInsideInterrupt( void );

/**
 * @brief Yield the processor.
 */
void vPortYield( void ) PRIVILEGED_FUNCTION;

/**
 * @brief Enter critical section.
 */
void vPortEnterCritical( void ) PRIVILEGED_FUNCTION;

/**
 * @brief Exit from critical section.
 */
void vPortExitCritical( void ) PRIVILEGED_FUNCTION;

/**
 * @brief SysTick handler.
 */
void SysTick_Handler( void ) PRIVILEGED_FUNCTION;

/**
 * @brief C part of SVC handler.
 */
portDONT_DISCARD void vPortSVCHandler_C( uint32_t * pulCallerStackAddress ) PRIVILEGED_FUNCTION;
/*-----------------------------------------------------------*/

/**
 * @brief Each task maintains its own interrupt status in the critical nesting
 * variable.
 */
PRIVILEGED_DATA static volatile uint32_t ulCriticalNesting = 0xaaaaaaaaUL;

#if ( configENABLE_TRUSTZONE == 1 )

/**
 * @brief Saved as part of the task context to indicate which context the
 * task is using on the secure side.
 */
    PRIVILEGED_DATA portDONT_DISCARD volatile SecureContextHandle_t xSecureContext = portNO_SECURE_CONTEXT;
#endif /* configENABLE_TRUSTZONE */
/*-----------------------------------------------------------*/

__attribute__(( weak )) void vPortSetupTimerInterrupt( void ) /* PRIVILEGED_FUNCTION */
{
    /* Stop and reset the SysTick. */
    portNVIC_SYSTICK_CTRL_REG = 0UL;
    portNVIC_SYSTICK_CURRENT_VALUE_REG = 0UL;

    /* Configure SysTick to interrupt at the requested rate. */
    portNVIC_SYSTICK_LOAD_REG = ( configSYSTICK_CLOCK_HZ / configTICK_RATE_HZ ) - 1UL;
    portNVIC_SYSTICK_CTRL_REG = portNVIC_SYSTICK_CLK_BIT | portNVIC_SYSTICK_INT_BIT | portNVIC_SYSTICK_ENABLE_BIT;
}
/*-----------------------------------------------------------*/

static void prvTaskExitError( void )
{
    volatile uint32_t ulDummy = 0UL;

    /* A function that implements a task must not exit or attempt to return to
     * its caller as there is nothing to return to. If a task wants to exit it
     * should instead call vTaskDelete( NULL ). Artificially force an assert()
     * to be triggered if configASSERT() is defined, then stop here so
     * application writers can catch the error. */
    configASSERT( ulCriticalNesting == ~0UL );
    portDISABLE_INTERRUPTS();

    while( ulDummy == 0 )
    {
        /* This file calls prvTaskExitError() after the scheduler has been
         * started to remove a compiler warning about the function being
         * defined but never called.  ulDummy is used purely to quieten other
         * warnings about code appearing after this function is called - making
         * ulDummy volatile makes the compiler think the function could return
         * and therefore not output an 'unreachable code' warning for code that
         * appears after it. */
    }
}
/*-----------------------------------------------------------*/

#if ( configENABLE_MPU == 1 )
    static void prvSetupMPU( void ) /* PRIVILEGED_FUNCTION */
    {
        #if defined( __ARMCC_VERSION )

            /* Declaration when these variable are defined in code instead of being
             * exported from linker scripts. */
            extern uint32_t * __privileged_functions_start__;
            extern uint32_t * __privileged_functions_end__;
            extern uint32_t * __syscalls_flash_start__;
            extern uint32_t * __syscalls_flash_end__;
            extern uint32_t * __unprivileged_flash_start__;
            extern uint32_t * __unprivileged_flash_end__;
            extern uint32_t * __privileged_sram_start__;
            extern uint32_t * __privileged_sram_end__;
        #else /* if defined( __ARMCC_VERSION ) */
            /* Declaration when these variable are exported from linker scripts. */
            extern uint32_t __privileged_functions_start__[];
            extern uint32_t __privileged_functions_end__[];
            extern uint32_t __syscalls_flash_start__[];
            extern uint32_t __syscalls_flash_end__[];
            extern uint32_t __unprivileged_flash_start__[];
            extern uint32_t __unprivileged_flash_end__[];
            extern uint32_t __privileged_sram_start__[];
            extern uint32_t __privileged_sram_end__[];
        #endif /* defined( __ARMCC_VERSION ) */

        /* Check that the MPU is present. */
        if( portMPU_TYPE_REG == portEXPECTED_MPU_TYPE_VALUE )
        {
            /* MAIR0 - Index 0. */
            portMPU_MAIR0_REG |= ( ( portMPU_NORMAL_MEMORY_BUFFERABLE_CACHEABLE << portMPU_MAIR_ATTR0_POS ) & portMPU_MAIR_ATTR0_MASK );
            /* MAIR0 - Index 1. */
            portMPU_MAIR0_REG |= ( ( portMPU_DEVICE_MEMORY_nGnRE << portMPU_MAIR_ATTR1_POS ) & portMPU_MAIR_ATTR1_MASK );

            /* Setup privileged flash as Read Only so that privileged tasks can
             * read it but not modify. */
            portMPU_RNR_REG = portPRIVILEGED_FLASH_REGION;
            portMPU_RBAR_REG = ( ( ( uint32_t ) __privileged_functions_start__ ) & portMPU_RBAR_ADDRESS_MASK ) |
                               ( portMPU_REGION_NON_SHAREABLE ) |
                               ( portMPU_REGION_PRIVILEGED_READ_ONLY );
            portMPU_RLAR_REG = ( ( ( uint32_t ) __privileged_functions_end__ ) & portMPU_RLAR_ADDRESS_MASK ) |
                               ( portMPU_RLAR_ATTR_INDEX0 ) |
                               ( portMPU_RLAR_REGION_ENABLE );

            /* Setup unprivileged flash as Read Only by both privileged and
             * unprivileged tasks. All tasks can read it but no-one can modify. */
            portMPU_RNR_REG = portUNPRIVILEGED_FLASH_REGION;
            portMPU_RBAR_REG = ( ( ( uint32_t ) __unprivileged_flash_start__ ) & portMPU_RBAR_ADDRESS_MASK ) |
                               ( portMPU_REGION_NON_SHAREABLE ) |
                               ( portMPU_REGION_READ_ONLY );
            portMPU_RLAR_REG = ( ( ( uint32_t ) __unprivileged_flash_end__ ) & portMPU_RLAR_ADDRESS_MASK ) |
                               ( portMPU_RLAR_ATTR_INDEX0 ) |
                               ( portMPU_RLAR_REGION_ENABLE );

            /* Setup unprivileged syscalls flash as Read Only by both privileged
             * and unprivileged tasks. All tasks can read it but no-one can modify. */
            portMPU_RNR_REG = portUNPRIVILEGED_SYSCALLS_REGION;
            portMPU_RBAR_REG = ( ( ( uint32_t ) __syscalls_flash_start__ ) & portMPU_RBAR_ADDRESS_MASK ) |
                               ( portMPU_REGION_NON_SHAREABLE ) |
                               ( portMPU_REGION_READ_ONLY );
            portMPU_RLAR_REG = ( ( ( uint32_t ) __syscalls_flash_end__ ) & portMPU_RLAR_ADDRESS_MASK ) |
                               ( portMPU_RLAR_ATTR_INDEX0 ) |
                               ( portMPU_RLAR_REGION_ENABLE );

            /* Setup RAM containing kernel data for privileged access only. */
            portMPU_RNR_REG = portPRIVILEGED_RAM_REGION;
            portMPU_RBAR_REG = ( ( ( uint32_t ) __privileged_sram_start__ ) & portMPU_RBAR_ADDRESS_MASK ) |
                               ( portMPU_REGION_NON_SHAREABLE ) |
                               ( portMPU_REGION_PRIVILEGED_READ_WRITE ) |
                               ( portMPU_REGION_EXECUTE_NEVER );
            portMPU_RLAR_REG = ( ( ( uint32_t ) __privileged_sram_end__ ) & portMPU_RLAR_ADDRESS_MASK ) |
                               ( portMPU_RLAR_ATTR_INDEX0 ) |
                               ( portMPU_RLAR_REGION_ENABLE );

            /* Enable mem fault. */
            portSCB_SYS_HANDLER_CTRL_STATE_REG |= portSCB_MEM_FAULT_ENABLE_BIT;

            /* Enable MPU with privileged background access i.e. unmapped
             * regions have privileged access. */
            portMPU_CTRL_REG |= ( portMPU_PRIV_BACKGROUND_ENABLE_BIT | portMPU_ENABLE_BIT );
        }
    }
#endif /* configENABLE_MPU */
/*-----------------------------------------------------------*/

#if ( configENABLE_FPU == 1 )
    static void prvSetupFPU( void ) /* PRIVILEGED_FUNCTION */
    {
        #if ( configENABLE_TRUSTZONE == 1 )
            {
                /* Enable non-secure access to the FPU. */
                SecureInit_EnableNSFPUAccess();
            }
        #endif /* configENABLE_TRUSTZONE */

        /* CP10 = 11 ==> Full access to FPU i.e. both privileged and
         * unprivileged code should be able to access FPU. CP11 should be
         * programmed to the same value as CP10. */
        *( portCPACR ) |= ( ( portCPACR_CP10_VALUE << portCPACR_CP10_POS ) |
                            ( portCPACR_CP11_VALUE << portCPACR_CP11_POS )
                            );

        /* ASPEN = 1 ==> Hardware should automatically preserve floating point
         * context on exception entry and restore on exception return.
         * LSPEN = 1 ==> Enable lazy context save of FP state. */
        *( portFPCCR ) |= ( portFPCCR_ASPEN_MASK | portFPCCR_LSPEN_MASK );
    }
#endif /* configENABLE_FPU */
/*-----------------------------------------------------------*/

void vPortYield( void ) /* PRIVILEGED_FUNCTION */
{
    /* Set a PendSV to request a context switch. */
    portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT;

	/* Barriers are normally not required but do ensure the code is
	 * completely within the specified behaviour for the architecture. */
	__asm volatile( "dsb" ::: "memory" );
	__asm volatile( "isb" );
}
/*-----------------------------------------------------------*/
#ifdef configCheck_CRITICAL_SECTION_LENGTH
ATTR_ZIDATA_IN_TCM static uint32_t mask_int_start = 0;
ATTR_ZIDATA_IN_TCM static uint32_t mask_int_end = 0;
ATTR_ZIDATA_IN_TCM static uint32_t mask_int_time = 0;
ATTR_ZIDATA_IN_TCM static uint32_t mask_int_lr;
#endif /* configCheck_CRITICAL_SECTION_LENGTH */

void vPortEnterCritical( void ) /* PRIVILEGED_FUNCTION */
{
	/* The interrupt should not be masked when this API is 1st called.
	in order to check the error usage, typical case is:
		hal_nvic_save_and_set_interrupt_mask()
		xQueueGenericSend() // Calling an FreeRTOS API function from within a critical section
		hal_nvic_restore_interrupt_mask()
	*/
	configASSERT(ulCriticalNesting || !__get_BASEPRI());

	portDISABLE_INTERRUPTS();
	ulCriticalNesting++;

	/* Barriers are normally not required but do ensure the code is
	 * completely within the specified behaviour for the architecture. */
	__asm volatile( "dsb" ::: "memory" );
	__asm volatile( "isb" );

	/* This is not the interrupt safe version of the enter critical function so
	assert() if it is being called from an interrupt context.  Only API
	functions that end in "FromISR" can be used in an interrupt.  Only assert if
	the critical nesting count is 1 to protect against recursive calls if the
	assert function also uses a critical section. */
	if( ulCriticalNesting == 1 )
	{
		#ifdef configCheck_CRITICAL_SECTION_LENGTH
		mask_int_lr = (uint32_t)__builtin_return_address(0);
		hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &mask_int_start);
		#endif /* configCheck_CRITICAL_SECTION_LENGTH */

		configASSERT( ( portNVIC_INT_CTRL_REG & portVECTACTIVE_MASK ) == 0 );
	}
}
/*-----------------------------------------------------------*/

void vPortExitCritical( void ) /* PRIVILEGED_FUNCTION */
{
	#ifdef configCheck_CRITICAL_SECTION_LENGTH
	uint32_t mask_int_check_fail = 0x0;
	uint32_t mask_int_too_long_time = 0x0;
	uint32_t mask_int_too_long_lr = 0x0;
	uint32_t mask_int_too_long_clk = 0x0;
	#endif /* configCheck_CRITICAL_SECTION_LENGTH */

	configASSERT( ulCriticalNesting );
	ulCriticalNesting--;

	if( ulCriticalNesting == 0 )
	{
		#ifdef configCheck_CRITICAL_SECTION_LENGTH
		#define CLK_CM4_FREQ_26M      ((uint32_t) 26000000)
		hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &mask_int_end);
		hal_gpt_get_duration_count(mask_int_start, mask_int_end, &mask_int_time);

		if (mask_int_time > configMASK_IRQ_DURATION_TIME) {
			extern uint32_t SystemCoreClock;
			if(SystemCoreClock > CLK_CM4_FREQ_26M) {
				/* skip 26M clock */
				//configASSERT(0);
				mask_int_check_fail = 1;
				mask_int_too_long_time = mask_int_time;
				mask_int_too_long_lr = mask_int_lr;
				mask_int_too_long_clk = SystemCoreClock;
			}
		}

		mask_int_start = 0;
		mask_int_end = 0;
		mask_int_time = 0;
		#endif /* configCheck_CRITICAL_SECTION_LENGTH */

		portENABLE_INTERRUPTS();
        #ifdef configCheck_CRITICAL_SECTION_LENGTH
		(void)mask_int_check_fail;
		(void)mask_int_too_long_time;
		(void)mask_int_too_long_lr;
		(void)mask_int_too_long_clk;
        #endif /* configCheck_CRITICAL_SECTION_LENGTH */
		#ifdef configCheck_CRITICAL_SECTION_LENGTH
		//if(0){
		//	LOG_E(os,"ERROR!!!!!! disable irq time too long = %d us, mask_int_check_fail = %d,mask_irq_lr = 0x%x, task is = %s, clock = %d\n\r\n",
		//			 5,mask_int_too_long_time,
		//			 mask_int_check_fail,
		//			 mask_int_too_long_lr,
		//			 pcTaskGetTaskName(xTaskGetCurrentTaskHandle()),
		//			 mask_int_too_long_clk);
		//}
		#endif /* configCheck_CRITICAL_SECTION_LENGTH */
	}
}
/*-----------------------------------------------------------*/

ATTR_TEXT_IN_RAM void SysTick_Handler( void ) /* PRIVILEGED_FUNCTION */
{
    uint32_t ulPreviousMask;

    ulPreviousMask = portSET_INTERRUPT_MASK_FROM_ISR();
    {
		/* must suspend flash before fetch code from flash */
		extern void Flash_ReturnReady(void);
		Flash_ReturnReady();

        #ifdef MTK_SWLA_ENABLE
        //SLA_CustomLogging("tik",SA_LABEL);
        #endif /* MTK_SWLA_ENABLE */

        /* Increment the RTOS tick. */
        if( xTaskIncrementTick() != pdFALSE )
        {
            /* Pend a context switch. */
            portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT;
        }
    }
    portCLEAR_INTERRUPT_MASK_FROM_ISR( ulPreviousMask );
}

/*-----------------------------------------------------------*/
#if (configCHECK_FOR_STACK_OVERFLOW > 0)
#ifdef HAL_DWT_MODULE_ENABLED
#include "core_cm33.h"
#include "hal_dwt.h"
void vPortCurrentTaskStackOverflowCheck(void)
{
	uint32_t stack_start_address;
	int32_t ret;

	stack_start_address = uxTaskGetBottomOfStack(NULL);

	/* enable debug monitor mode    */
	if (!(CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)) {
		if (!(CoreDebug->DEMCR & (CoreDebug_DEMCR_MON_EN_Msk | CoreDebug_DEMCR_TRCENA_Msk))) {
			CoreDebug->DEMCR |= (CoreDebug_DEMCR_MON_EN_Msk | CoreDebug_DEMCR_TRCENA_Msk) ;
		}
	}

	/* check the last 2words */
	ret = hal_dwt_request_watchpoint(HAL_DWT_3, stack_start_address, 0x2, WDE_DATA_WO);
	//printf("comparator:%d, check address: 0x%x\r\n",HAL_DWT_3,stack_end_address);

	/* Just to avoid compiler warnings. */
	( void ) ret;
}
#else
	#error please enable HAL_DWT_MODULE_ENABLED in project inc/hal_feature_config.h for task stack overflow check.
#endif /* HAL_DWT_MODULE_ENABLED */
#endif /* (configCHECK_FOR_STACK_OVERFLOW > 0) */

/*-----------------------------------------------------------*/

void vPortSVCHandler_C( uint32_t *pulCallerStackAddress ) /* PRIVILEGED_FUNCTION portDONT_DISCARD */
{
    #if ( configENABLE_MPU == 1 )
        #if defined( __ARMCC_VERSION )

            /* Declaration when these variable are defined in code instead of being
             * exported from linker scripts. */
            extern uint32_t * __syscalls_flash_start__;
            extern uint32_t * __syscalls_flash_end__;
        #else
            /* Declaration when these variable are exported from linker scripts. */
            extern uint32_t __syscalls_flash_start__[];
            extern uint32_t __syscalls_flash_end__[];
        #endif /* defined( __ARMCC_VERSION ) */
    #endif /* configENABLE_MPU */

    uint32_t ulPC;

    #if ( configENABLE_TRUSTZONE == 1 )
        uint32_t ulR0, ulR1;
        extern TaskHandle_t pxCurrentTCB;
        #if ( configENABLE_MPU == 1 )
            uint32_t ulControl, ulIsTaskPrivileged;
        #endif /* configENABLE_MPU */
    #endif /* configENABLE_TRUSTZONE */
    uint8_t ucSVCNumber;

    /* Register are stored on the stack in the following order - R0, R1, R2, R3,
     * R12, LR, PC, xPSR. */
    ulPC = pulCallerStackAddress[ 6 ];
    ucSVCNumber = ( ( uint8_t * ) ulPC )[ -2 ];

    switch( ucSVCNumber )
    {
        #if ( configENABLE_TRUSTZONE == 1 )
            case portSVC_ALLOCATE_SECURE_CONTEXT:

                /* R0 contains the stack size passed as parameter to the
                 * vPortAllocateSecureContext function. */
                ulR0 = pulCallerStackAddress[ 0 ];

                #if ( configENABLE_MPU == 1 )
                    {
                        /* Read the CONTROL register value. */
                        __asm volatile ( "mrs %0, control"  : "=r" ( ulControl ) );

                        /* The task that raised the SVC is privileged if Bit[0]
                         * in the CONTROL register is 0. */
                        ulIsTaskPrivileged = ( ( ulControl & portCONTROL_PRIVILEGED_MASK ) == 0 );

                        /* Allocate and load a context for the secure task. */
                        xSecureContext = SecureContext_AllocateContext( ulR0, ulIsTaskPrivileged, pxCurrentTCB );
                    }
                #else /* if ( configENABLE_MPU == 1 ) */
                    {
                        /* Allocate and load a context for the secure task. */
                        xSecureContext = SecureContext_AllocateContext( ulR0, pxCurrentTCB );
                    }
                #endif /* configENABLE_MPU */

                configASSERT( xSecureContext != securecontextINVALID_CONTEXT_ID );
                SecureContext_LoadContext( xSecureContext, pxCurrentTCB );
                break;

            case portSVC_FREE_SECURE_CONTEXT:
                /* R0 contains TCB being freed and R1 contains the secure
                 * context handle to be freed. */
                ulR0 = pulCallerStackAddress[ 0 ];
                ulR1 = pulCallerStackAddress[ 1 ];

                /* Free the secure context. */
                SecureContext_FreeContext( ( SecureContextHandle_t ) ulR1, ( void * ) ulR0 );
                break;
        #endif /* configENABLE_TRUSTZONE */

        case portSVC_START_SCHEDULER:
		{
			#ifdef MTK_SWLA_ENABLE
			SLA_Enable();
			#endif /* MTK_SWLA_ENABLE */

            #if ( configENABLE_TRUSTZONE == 1 )
                {
                    /* De-prioritize the non-secure exceptions so that the
                     * non-secure pendSV runs at the lowest priority. */
                    SecureInit_DePrioritizeNSExceptions();

                    /* Initialize the secure context management system. */
                    SecureContext_Init();
                }
            #endif /* configENABLE_TRUSTZONE */

            #if ( configENABLE_FPU == 1 )
                {
                    /* Setup the Floating Point Unit (FPU). */
                    prvSetupFPU();
                }
            #endif /* configENABLE_FPU */
			#if (configCHECK_FOR_STACK_OVERFLOW > 0)/* Enable the stack overflow check by DWT */
				#ifdef HAL_DWT_MODULE_ENABLED
					#include "hal_dwt.h"
					hal_dwt_init();             
					vPortCurrentTaskStackOverflowCheck();
					hal_dwt_request_watchpoint(HAL_DWT_1,0,0x2,WDE_DATA_RW);
				#endif /* HAL_DWT_MODULE_ENABLED */
			#endif /* (configCHECK_FOR_STACK_OVERFLOW > 0)  */
			/* monitor 1st task in swla */
			#ifdef MTK_SWLA_ENABLE
			vTraceTaskSwitchIn(0);
			#endif /* MTK_SWLA_ENABLE */

            /* Setup the context of the first task so that the first task starts
             * executing. */
            vRestoreContextOfFirstTask();
		}
        break;

            #if ( configENABLE_MPU == 1 )
                case portSVC_RAISE_PRIVILEGE:

                    /* Only raise the privilege, if the svc was raised from any of
                     * the system calls. */
                    if( ( ulPC >= ( uint32_t ) __syscalls_flash_start__ ) &&
                        ( ulPC <= ( uint32_t ) __syscalls_flash_end__ ) )
                    {
                        vRaisePrivilege();
                    }
                    break;
            #endif /* configENABLE_MPU */

        default:
            /* Incorrect SVC call. */
            configASSERT( pdFALSE );
    }
}
/*-----------------------------------------------------------*/
/* *INDENT-OFF* */
#if ( configENABLE_MPU == 1 )
    StackType_t * pxPortInitialiseStack( StackType_t * pxTopOfStack,
                                         StackType_t * pxEndOfStack,
                                         TaskFunction_t pxCode,
                                         void * pvParameters,
                                         BaseType_t xRunPrivileged ) /* PRIVILEGED_FUNCTION */
#else
    StackType_t * pxPortInitialiseStack( StackType_t * pxTopOfStack,
                                         StackType_t * pxEndOfStack,
                                         TaskFunction_t pxCode,
                                         void * pvParameters ) /* PRIVILEGED_FUNCTION */
#endif /* configENABLE_MPU */
/* *INDENT-ON* */
{
    /* Simulate the stack frame as it would be created by a context switch
     * interrupt. */
    #if ( portPRELOAD_REGISTERS == 0 )
        {
            pxTopOfStack--;                                          /* Offset added to account for the way the MCU uses the stack on entry/exit of interrupts. */
            *pxTopOfStack = portINITIAL_XPSR;                        /* xPSR */
            pxTopOfStack--;
            *pxTopOfStack = ( StackType_t ) pxCode;                  /* PC */
            pxTopOfStack--;
            *pxTopOfStack = ( StackType_t ) portTASK_RETURN_ADDRESS; /* LR */
            pxTopOfStack -= 5;                                       /* R12, R3, R2 and R1. */
            *pxTopOfStack = ( StackType_t ) pvParameters;            /* R0 */
            pxTopOfStack -= 9;                                       /* R11..R4, EXC_RETURN. */
            *pxTopOfStack = portINITIAL_EXC_RETURN;

            #if ( configENABLE_MPU == 1 )
                {
                    pxTopOfStack--;

                    if( xRunPrivileged == pdTRUE )
                    {
                        *pxTopOfStack = portINITIAL_CONTROL_PRIVILEGED; /* Slot used to hold this task's CONTROL value. */
                    }
                    else
                    {
                        *pxTopOfStack = portINITIAL_CONTROL_UNPRIVILEGED; /* Slot used to hold this task's CONTROL value. */
                    }
                }
            #endif /* configENABLE_MPU */

            pxTopOfStack--;
            *pxTopOfStack = ( StackType_t ) pxEndOfStack; /* Slot used to hold this task's PSPLIM value. */

            #if ( configENABLE_TRUSTZONE == 1 )
                {
                    pxTopOfStack--;
                    *pxTopOfStack = portNO_SECURE_CONTEXT; /* Slot used to hold this task's xSecureContext value. */
                }
            #endif /* configENABLE_TRUSTZONE */
        }
    #else /* portPRELOAD_REGISTERS */
        {
            pxTopOfStack--;                                          /* Offset added to account for the way the MCU uses the stack on entry/exit of interrupts. */
            *pxTopOfStack = portINITIAL_XPSR;                        /* xPSR */
            pxTopOfStack--;
            *pxTopOfStack = ( StackType_t ) pxCode;                  /* PC */
            pxTopOfStack--;
            *pxTopOfStack = ( StackType_t ) portTASK_RETURN_ADDRESS; /* LR */
            pxTopOfStack--;
            *pxTopOfStack = ( StackType_t ) 0x12121212UL;            /* R12 */
            pxTopOfStack--;
            *pxTopOfStack = ( StackType_t ) 0x03030303UL;            /* R3 */
            pxTopOfStack--;
            *pxTopOfStack = ( StackType_t ) 0x02020202UL;            /* R2 */
            pxTopOfStack--;
            *pxTopOfStack = ( StackType_t ) 0x01010101UL;            /* R1 */
            pxTopOfStack--;
            *pxTopOfStack = ( StackType_t ) pvParameters;            /* R0 */
            pxTopOfStack--;
            *pxTopOfStack = ( StackType_t ) 0x11111111UL;            /* R11 */
            pxTopOfStack--;
            *pxTopOfStack = ( StackType_t ) 0x10101010UL;            /* R10 */
            pxTopOfStack--;
            *pxTopOfStack = ( StackType_t ) 0x09090909UL;            /* R09 */
            pxTopOfStack--;
            *pxTopOfStack = ( StackType_t ) 0x08080808UL;            /* R08 */
            pxTopOfStack--;
            *pxTopOfStack = ( StackType_t ) 0x07070707UL;            /* R07 */
            pxTopOfStack--;
            *pxTopOfStack = ( StackType_t ) 0x06060606UL;            /* R06 */
            pxTopOfStack--;
            *pxTopOfStack = ( StackType_t ) 0x05050505UL;            /* R05 */
            pxTopOfStack--;
            *pxTopOfStack = ( StackType_t ) 0x04040404UL;            /* R04 */
            pxTopOfStack--;
            *pxTopOfStack = portINITIAL_EXC_RETURN;                  /* EXC_RETURN */

            #if ( configENABLE_MPU == 1 )
                {
                    pxTopOfStack--;

                    if( xRunPrivileged == pdTRUE )
                    {
                        *pxTopOfStack = portINITIAL_CONTROL_PRIVILEGED; /* Slot used to hold this task's CONTROL value. */
                    }
                    else
                    {
                        *pxTopOfStack = portINITIAL_CONTROL_UNPRIVILEGED; /* Slot used to hold this task's CONTROL value. */
                    }
                }
            #endif /* configENABLE_MPU */

            pxTopOfStack--;
            *pxTopOfStack = ( StackType_t ) pxEndOfStack; /* Slot used to hold this task's PSPLIM value. */

            #if ( configENABLE_TRUSTZONE == 1 )
                {
                    pxTopOfStack--;
                    *pxTopOfStack = portNO_SECURE_CONTEXT; /* Slot used to hold this task's xSecureContext value. */
                }
            #endif /* configENABLE_TRUSTZONE */
        }
    #endif /* portPRELOAD_REGISTERS */

    return pxTopOfStack;
}
/*-----------------------------------------------------------*/

BaseType_t xPortStartScheduler( void ) /* PRIVILEGED_FUNCTION */
{
    /* Make PendSV, CallSV and SysTick the same priority as the kernel. */
    portNVIC_SHPR3_REG |= portNVIC_PENDSV_PRI;
    portNVIC_SHPR3_REG |= portNVIC_SYSTICK_PRI;

    #if ( configENABLE_MPU == 1 )
        {
            /* Setup the Memory Protection Unit (MPU). */
            prvSetupMPU();
        }
    #endif /* configENABLE_MPU */

    /* Start the timer that generates the tick ISR. Interrupts are disabled
     * here already. */
    vPortSetupTimerInterrupt();

	#ifdef SYSTEM_DAEMON_TASK_ENABLE
	extern TaskHandle_t system_daemon_task_handle;
	configASSERT( xTaskCreate( system_daemon_task, "SYSDEA", SYSTEM_DAEMON_STACK_SIZE/sizeof(StackType_t), ( void * ) NULL, ( (tskIDLE_PRIORITY + 1) | portPRIVILEGE_BIT ), &system_daemon_task_handle ) );
	system_daemon_task_init();
	#endif /* SYSTEM_DAEMON_TASK_ENABLE */

    /* Initialize the critical nesting count ready for the first task. */
    ulCriticalNesting = 0;

    /* Start the first task. */
    vStartFirstTask();

    /* Should never get here as the tasks will now be executing. Call the task
     * exit error function to prevent compiler warnings about a static function
     * not being called in the case that the application writer overrides this
     * functionality by defining configTASK_RETURN_ADDRESS. Call
     * vTaskSwitchContext() so link time optimization does not remove the
     * symbol. */
    vTaskSwitchContext();
    prvTaskExitError();

    /* Should not get here. */
    return 0;
}
/*-----------------------------------------------------------*/

void vPortEndScheduler( void ) /* PRIVILEGED_FUNCTION */
{
    /* Not implemented in ports where there is nothing to return to.
     * Artificially force an assert. */
    configASSERT( ulCriticalNesting == 1000UL );
}
/*-----------------------------------------------------------*/

#if ( configENABLE_MPU == 1 )
    void vPortStoreTaskMPUSettings( xMPU_SETTINGS * xMPUSettings,
                                    const struct xMEMORY_REGION * const xRegions,
                                    StackType_t * pxBottomOfStack,
                                    uint32_t ulStackDepth )
    {
        uint32_t ulRegionStartAddress, ulRegionEndAddress, ulRegionNumber;
        int32_t lIndex = 0;

        #if defined( __ARMCC_VERSION )

            /* Declaration when these variable are defined in code instead of being
             * exported from linker scripts. */
            extern uint32_t * __privileged_sram_start__;
            extern uint32_t * __privileged_sram_end__;
        #else
            /* Declaration when these variable are exported from linker scripts. */
            extern uint32_t __privileged_sram_start__[];
            extern uint32_t __privileged_sram_end__[];
        #endif /* defined( __ARMCC_VERSION ) */

        /* Setup MAIR0. */
        xMPUSettings->ulMAIR0 = ( ( portMPU_NORMAL_MEMORY_BUFFERABLE_CACHEABLE << portMPU_MAIR_ATTR0_POS ) & portMPU_MAIR_ATTR0_MASK );
        xMPUSettings->ulMAIR0 |= ( ( portMPU_DEVICE_MEMORY_nGnRE << portMPU_MAIR_ATTR1_POS ) & portMPU_MAIR_ATTR1_MASK );

        /* This function is called automatically when the task is created - in
         * which case the stack region parameters will be valid.  At all other
         * times the stack parameters will not be valid and it is assumed that
         * the stack region has already been configured. */
        if( ulStackDepth > 0 )
        {
            ulRegionStartAddress = ( uint32_t ) pxBottomOfStack;
            ulRegionEndAddress = ( uint32_t ) pxBottomOfStack + ( ulStackDepth * ( uint32_t ) sizeof( StackType_t ) ) - 1;

            /* If the stack is within the privileged SRAM, do not protect it
             * using a separate MPU region. This is needed because privileged
             * SRAM is already protected using an MPU region and ARMv8-M does
             * not allow overlapping MPU regions. */
            if( ( ulRegionStartAddress >= ( uint32_t ) __privileged_sram_start__ ) &&
                ( ulRegionEndAddress <= ( uint32_t ) __privileged_sram_end__ ) )
            {
                xMPUSettings->xRegionsSettings[ 0 ].ulRBAR = 0;
                xMPUSettings->xRegionsSettings[ 0 ].ulRLAR = 0;
            }
            else
            {
                /* Define the region that allows access to the stack. */
                ulRegionStartAddress &= portMPU_RBAR_ADDRESS_MASK;
                ulRegionEndAddress &= portMPU_RLAR_ADDRESS_MASK;

                xMPUSettings->xRegionsSettings[ 0 ].ulRBAR = ( ulRegionStartAddress ) |
                                                             ( portMPU_REGION_NON_SHAREABLE ) |
                                                             ( portMPU_REGION_READ_WRITE ) |
                                                             ( portMPU_REGION_EXECUTE_NEVER );

                xMPUSettings->xRegionsSettings[ 0 ].ulRLAR = ( ulRegionEndAddress ) |
                                                             ( portMPU_RLAR_ATTR_INDEX0 ) |
                                                             ( portMPU_RLAR_REGION_ENABLE );
            }
        }

        /* User supplied configurable regions. */
        for( ulRegionNumber = 1; ulRegionNumber <= portNUM_CONFIGURABLE_REGIONS; ulRegionNumber++ )
        {
            /* If xRegions is NULL i.e. the task has not specified any MPU
             * region, the else part ensures that all the configurable MPU
             * regions are invalidated. */
            if( ( xRegions != NULL ) && ( xRegions[ lIndex ].ulLengthInBytes > 0UL ) )
            {
                /* Translate the generic region definition contained in xRegions
                 * into the ARMv8 specific MPU settings that are then stored in
                 * xMPUSettings. */
                ulRegionStartAddress = ( ( uint32_t ) xRegions[ lIndex ].pvBaseAddress ) & portMPU_RBAR_ADDRESS_MASK;
                ulRegionEndAddress = ( uint32_t ) xRegions[ lIndex ].pvBaseAddress + xRegions[ lIndex ].ulLengthInBytes - 1;
                ulRegionEndAddress &= portMPU_RLAR_ADDRESS_MASK;

                /* Start address. */
                xMPUSettings->xRegionsSettings[ ulRegionNumber ].ulRBAR = ( ulRegionStartAddress ) |
                                                                          ( portMPU_REGION_NON_SHAREABLE );

                /* RO/RW. */
                if( ( xRegions[ lIndex ].ulParameters & tskMPU_REGION_READ_ONLY ) != 0 )
                {
                    xMPUSettings->xRegionsSettings[ ulRegionNumber ].ulRBAR |= ( portMPU_REGION_READ_ONLY );
                }
                else
                {
                    xMPUSettings->xRegionsSettings[ ulRegionNumber ].ulRBAR |= ( portMPU_REGION_READ_WRITE );
                }

                /* XN. */
                if( ( xRegions[ lIndex ].ulParameters & tskMPU_REGION_EXECUTE_NEVER ) != 0 )
                {
                    xMPUSettings->xRegionsSettings[ ulRegionNumber ].ulRBAR |= ( portMPU_REGION_EXECUTE_NEVER );
                }

                /* End Address. */
                xMPUSettings->xRegionsSettings[ ulRegionNumber ].ulRLAR = ( ulRegionEndAddress ) |
                                                                          ( portMPU_RLAR_REGION_ENABLE );

                /* Normal memory/ Device memory. */
                if( ( xRegions[ lIndex ].ulParameters & tskMPU_REGION_DEVICE_MEMORY ) != 0 )
                {
                    /* Attr1 in MAIR0 is configured as device memory. */
                    xMPUSettings->xRegionsSettings[ ulRegionNumber ].ulRLAR |= portMPU_RLAR_ATTR_INDEX1;
                }
                else
                {
                    /* Attr1 in MAIR0 is configured as normal memory. */
                    xMPUSettings->xRegionsSettings[ ulRegionNumber ].ulRLAR |= portMPU_RLAR_ATTR_INDEX0;
                }
            }
            else
            {
                /* Invalidate the region. */
                xMPUSettings->xRegionsSettings[ ulRegionNumber ].ulRBAR = 0UL;
                xMPUSettings->xRegionsSettings[ ulRegionNumber ].ulRLAR = 0UL;
            }

            lIndex++;
        }
    }
#endif /* configENABLE_MPU */
/*-----------------------------------------------------------*/

BaseType_t xPortIsInsideInterrupt( void )
{
    uint32_t ulCurrentInterrupt;
    BaseType_t xReturn;

    /* Obtain the number of the currently executing interrupt. Interrupt Program
     * Status Register (IPSR) holds the exception number of the currently-executing
     * exception or zero for Thread mode.*/
    __asm volatile ( "mrs %0, ipsr" : "=r" ( ulCurrentInterrupt )::"memory" );

    if( ulCurrentInterrupt == 0 )
    {
        xReturn = pdFALSE;
    }
    else
    {
        xReturn = pdTRUE;
    }

    return xReturn;
}
/*-----------------------------------------------------------*/