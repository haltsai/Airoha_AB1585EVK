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

#ifndef __MUX_RINGBUFFER_H__
#define __MUX_RINGBUFFER_H__
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
// #include <stdatomic.h>
#include "mux.h"
#include "hal_pdma_internal.h"
#include "memory_attribute.h"

#define atomic_add(addr, val) __sync_fetch_and_add(addr, val)
#define atomic_sub(addr, val) __sync_fetch_and_sub(addr, val)

#if defined(CORE_DSP0)
#define cas(ptr, old, new) ({\
    uint32_t *p = ptr;\
    *p = new;\
    true;\
})
#elif defined(CORE_CM33)
#define cas(ptr, old, new) __sync_bool_compare_and_swap(ptr, old, new)
#else
#error "CORE ERROR"
#endif

extern void _hexdump(const char *funcname, const void *data, unsigned int len, const void *data1, unsigned int len1);
#define hexdump(data, len)                  _hexdump(__FUNCTION__, data, len, NULL, 0)
#define hexdump2(data, len, data1, len1)    _hexdump(__FUNCTION__, data, len, data1, len1)
#undef __FORCE_INLINE__
#define __FORCE_INLINE__ __attribute__((always_inline)) inline

#if defined(CORE_DSP0)
#define MUX_RB_SWITCH_START_MEMVIEW(rb)   rb->dsp_view_buffer
#define MUX_RB_SWITCH_END_MEMVIEW(rb)     rb->dsp_view_buffer_end
#elif defined(CORE_CM33)
#define MUX_RB_SWITCH_START_MEMVIEW(rb)   rb->mcu_view_buffer
#define MUX_RB_SWITCH_END_MEMVIEW(rb)     rb->mcu_view_buffer_end
#else
#error "CORE ERROR"
#endif

#define MEM_ATTR_NONSHARED_ALIGN(n) ATTR_ZIDATA_IN_NONCACHED_SYSRAM ATTR_ALIGN(n)
#define MEM_ATTR_SHARED_ALIGN(n)    ATTR_SHARE_ZIDATA ATTR_ALIGN(n)

#if defined(CORE_DSP0)
#define ATTR_TEXT_IN_FAST_MEM   ATTR_TEXT_IN_IRAM
#define ATTR_RWDATA_IN_FAST_MEM ATTR_RWDATA_IN_DRAM
#define ATTR_ZIDATA_IN_FAST_MEM ATTR_ZIDATA_IN_DRAM
#elif defined(CORE_CM33)
#define ATTR_TEXT_IN_FAST_MEM   ATTR_TEXT_IN_TCM
#define ATTR_RWDATA_IN_FAST_MEM ATTR_RWDATA_IN_TCM
#define ATTR_ZIDATA_IN_FAST_MEM ATTR_ZIDATA_IN_TCM
#else
#error "CORE ERROR"
#endif

#define RB_LOG_D(fmt, cnt, ...) LOG_MSGID_D(MUX_PORT, fmt, cnt, ##__VA_ARGS__)
#define RB_LOG_I(fmt, cnt, ...) LOG_MSGID_I(MUX_PORT, fmt, cnt, ##__VA_ARGS__)
#define RB_LOG_W(fmt, cnt, ...) LOG_MSGID_W(MUX_PORT, fmt, cnt, ##__VA_ARGS__)
#define RB_LOG_E(fmt, cnt, ...) LOG_MSGID_E(MUX_PORT, fmt, cnt, ##__VA_ARGS__)

/*
__builtin_expect (long exp, long c)
__builtin_constant_p (exp)
__builtin_prefetch (const void *addr, ...)
*/
#define RB_ADDRESS_ALIGN_TO_LEN_ENABLE
// #define RB_DIR_TYPE_CHECK_ENABLE
#define RB_WATER_MARK_ENABLE

struct mux_ringbuffer;

typedef struct {
    uint32_t (*rb_get_rptr)(struct mux_ringbuffer *rb);
    uint32_t (*rb_get_wptr)(struct mux_ringbuffer *rb);
    void (*rb_set_tx_wptr)(struct mux_ringbuffer *rb, uint32_t move_bytes);
    void (*rb_set_rx_rptr)(struct mux_ringbuffer *rb, uint32_t move_bytes);
    bool (*rb_full)(struct mux_ringbuffer *rb);
    bool (*rb_empty)(struct mux_ringbuffer *rb);
    uint32_t (*rb_data_size)(struct mux_ringbuffer *rb);
    uint32_t (*rb_free_space)(struct mux_ringbuffer *rb);
} mux_ringbuffer_ops_t;

typedef struct {
    uint32_t head;
    uint32_t tail;
} mux_ringbuffer_node_t;

#define MUX_RB_FLAG_TRX_BUF_MASK   (0x1)
#define MUX_RB_FLAG_TX_BUF         (0x0)
#define MUX_RB_FLAG_RX_BUF         (0x1)

#define MUX_RB_FLAG_DATA_4TOH3      (0x02)
#define MUX_RB_FLAG_DATA_4TOL3      (0x04)
#define MUX_RB_FLAG_DATA_3TOH4      (0x08)
#define MUX_RB_FLAG_DATA_3TOL4      (0x10)
#define MUX_RB_FLAG_DATA_3TO4       (MUX_RB_FLAG_DATA_3TOH4 | MUX_RB_FLAG_DATA_3TOL4)
#define MUX_RB_FLAG_DATA_4TO3       (MUX_RB_FLAG_DATA_4TOH3 | MUX_RB_FLAG_DATA_4TOL3)
#define MUX_RB_FLAG_UART_LATCH      (0x20)
#define MUX_RB_FLAG_SHARED_BUFFER   (0x40)

#define RB_SET_FLAG(rb, flag) do {(rb)->flags |=  (flag);} while (0)
#define RB_CLR_FLAG(rb, flag) do {(rb)->flags &= ~(flag);} while (0)
#define RB_CHK_FLAG(rb, flag) ((rb)->flags & (flag))

#define MUX_RB_MAX_NAME_LEN 6

typedef struct mux_ringbuffer {
    char name[MUX_RB_MAX_NAME_LEN]; /*fifo name for debug*/
#ifdef RB_WATER_MARK_ENABLE
    uint16_t water_mark;
#endif
    union {
        mux_ringbuffer_node_t wptr; //for multiple user
        uint32_t swptr; //for single user
    };

    union {
        mux_ringbuffer_node_t rptr; //for multiple user
        uint32_t srptr; //for single user
    };

    /* for hw fifo*/
    mux_ringbuffer_ops_t *ops;

    uint32_t port_idx;
    uint32_t flags;
    uint32_t size; /*data length in ring buffer, but not used now */
    uint32_t mask; /*wptr & rptr mask*/
    uint32_t capacity; /*ring buffer size*/

    uint8_t *dsp_view_buffer;
    uint8_t *dsp_view_buffer_end;

    uint8_t *mcu_view_buffer;
    uint8_t *mcu_view_buffer_end;
    uint8_t uid;
} mux_ringbuffer_t;

#define RB_ASSERT(x)
#define RB_MALLOC(x) malloc(x)
#define RB_FREE(x) free(x)
#define RB_MIN(a, b) (((a) < (b)) ? (a) : (b))

#define MUX_RB_ASSERT(cond)

#ifdef FREERTOS_ENABLE
#include "FreeRTOS.h"

extern void vPortFreeNC( void *pv );
extern void *pvPortMallocNC( size_t xWantedSize );

#define MUX_RB_PORTABLE_FREE(ptr)     vPortFreeNC(ptr)
#define MUX_RB_PORTABLE_MALLOC(size)   pvPortMallocNC(size)
#else
#define MUX_RB_PORTABLE_FREE(ptr)     free(ptr)
#define MUX_RB_PORTABLE_MALLOC(size)   malloc(size)
#endif

#define RB_ADDRESS_POWER_OF_2 1
#if RB_ADDRESS_POWER_OF_2

#define RB_START_ADDRESS(rb)  (MUX_RB_SWITCH_START_MEMVIEW(rb))
#define RB_END_ADDRESS(rb)  (MUX_RB_SWITCH_END_MEMVIEW(rb))

//physical rptr
#if 0
#define RB_PHY_RPTR_HEAD(rb) ((rb->ops)? rb->ops->rb_get_rptr(rb):(rb->rptr.head & rb->mask))
#define RB_PHY_RPTR_TAIL(rb) ((rb->ops)? rb->ops->rb_get_rptr(rb):(rb->rptr.tail & rb->mask))
#else
#define RB_PHY_RPTR_HEAD(rb) (rb->rptr.head & rb->mask)
#define RB_PHY_RPTR_TAIL(rb) (rb->rptr.tail & rb->mask)
#endif
#define RB_PHY_SW_RPTR_ADDR(rb) (RB_START_ADDRESS(rb) + (rb->rptr.head & rb->mask))

#define RB_FETCH_BYTE_ADDRESS(rb, offset) (uint8_t*)(RB_START_ADDRESS(rb) + ((rb->rptr.head + (offset)) & rb->mask))
#define RB_WRITE_BYTE_ADDRESS(rb, offset) (uint8_t*)(RB_START_ADDRESS(rb) + ((rb->wptr.head + (offset)) & rb->mask))
#define RB_FETCH_BYTE_DATA(rb, offset) (*RB_FETCH_BYTE_ADDRESS(rb, offset))

//physical wptr
#if 0
#define RB_PHY_WPTR_HEAD(rb) ((rb->ops)? rb->ops->rb_get_wptr(rb):(rb->wptr.head & rb->mask))
#define RB_PHY_WPTR_TAIL(rb) ((rb->ops)? rb->ops->rb_get_wptr(rb):(rb->wptr.tail & rb->mask))
#else
#define RB_PHY_WPTR_HEAD(rb) (rb->wptr.head & rb->mask)
#define RB_PHY_WPTR_TAIL(rb) (rb->wptr.tail & rb->mask)
#endif

#define RB_UPPDATE_RPTR(rb, len) (rb->rptr += len) //Each time the accumulation, after reaching the maximum overflow, automatically converted to 0
#define RB_UPPDATE_WPTR(rb, len) (rb->wptr += len) //Each time the accumulation, after reaching the maximum overflow, automatically converted to 0

//the total data in ringbuffer, write done - not read yet
#define RB_TOTAL_DATA_SIZE(rb) (rb->wptr.tail - rb->rptr.head)


#define RB_IS_EMPTY(rb) ((rb->wptr.tail == rb->rptr.tail) ? true : false)
#define RB_IS_FULL(rb) ((rb->capacity == mux_ringbuffer_data_length(rb)) ? true : false)

#else //RB_ADDRESS_POWER_OF_2
#error "RB_ADDRESS_POWER_OF_2 must be 1 now!!"
//physical rptr
#define RB_PHY_RPTR_HEAD(rb) (rb->rptr.head)
#define RB_PHY_RPTR_TAIL(rb) (rb->rptr.tail)
//physical wptr
#define RB_PHY_WPTR_HEAD(rb) (rb->wptr.head)
#define RB_PHY_WPTR_TAIL(rb) (rb->wptr.tail)

#define RB_UPPDATE_RPTR(rb, len) (rb->rptr = (rb->rptr + len) % rb->capacity)
#define RB_UPPDATE_WPTR(rb, len) (rb->wptr = (rb->wptr + len) % rb->capacity)
//the total data in ringbuffer
#define RB_TOTAL_DATA_SIZE(rb) ((rb->wptr.tail - rb->rptr.head + rb->capacity) % rb->capacity)

#endif //RB_ADDRESS_POWER_OF_2

//the total free space size in ringbuffer
#define RB_TOTAL_FREE_SPACE_SIZE(rb) (rb->capacity - mux_ringbuffer_data_length(rb))

//In ringbuffer, starting with RPTR, the length of consecutive data in the address
#define RB_CONTIGUOUS_DATA_SIZE(rb) ((RB_PHY_WPTR_TAIL(rb) >= RB_PHY_RPTR_HEAD(rb)) ? \
    RB_PHY_WPTR_TAIL(rb) - RB_PHY_RPTR_HEAD(rb) : (rb->capacity - RB_PHY_RPTR_HEAD(rb)))
//In ringBuffer, starting with WPTR, the length of contigual free space of the address

#define RB_CONTIGUOUS_FREE_SPACE_SIZE(rb) ((RB_PHY_WPTR_TAIL(rb) > RB_PHY_RPTR_HEAD(rb)) ? \
    (rb->capacity - RB_PHY_WPTR_TAIL(rb)) : RB_PHY_RPTR_HEAD(rb) - RB_PHY_WPTR_TAIL(rb))

//ringbuffer rptr address
#define RB_CONTIGUOUS_DATA_START_ADDR(rb) (MUX_RB_SWITCH_START_MEMVIEW(rb) + RB_PHY_RPTR_HEAD(rb))
//ringbuffer wptr address
#define RB_CONTIGUOUS_FREE_SPACE_START_ADDR(rb) (MUX_RB_SWITCH_START_MEMVIEW(rb) + RB_PHY_WPTR_TAIL(rb))
#define mux_ringbuffer_hexdump(rb, is_dump_data) mux_ringbuffer_dump(rb, __func__, is_dump_data)

// static __FORCE_INLINE__ uint32_t mux_ringbuffer_write_st(mux_ringbuffer_t *rb, uint8_t *buffer, uint32_t len);
// static __FORCE_INLINE__ uint32_t mux_ringbuffer_write_data_st(mux_ringbuffer_t *rb, uint8_t *buffer, uint32_t len);
// static __FORCE_INLINE__ void mux_ringbuffer_write_move_hw_tail_st(mux_ringbuffer_t *rb, uint32_t move_bytes);

// static __FORCE_INLINE__ uint32_t mux_ringbuffer_read_st(mux_ringbuffer_t *rb, uint8_t *buffer, uint32_t len);
// static __FORCE_INLINE__ uint32_t mux_ringbuffer_read_data_st(mux_ringbuffer_t *rb, uint8_t *buffer, uint32_t len);
// static __FORCE_INLINE__ void mux_ringbuffer_read_move_hw_tail_ht_st(mux_ringbuffer_t *rb, uint32_t move_bytes);

//move sw wptr head/tail of src_rb and dest_rb ,except for hw wptr/rptr
uint32_t mux_ringbuffer_write_buffer_st(mux_ringbuffer_t *src_rb, mux_ringbuffer_t *dest_rb, uint32_t len, uint16_t *crc);

bool mux_ringbuffer_init(mux_ringbuffer_t *rb, uint8_t *buffer, uint32_t capacity, uint32_t flags, const char *name);
bool mux_ringbuffer_alloc(mux_ringbuffer_t *rb, uint32_t capacity, uint32_t flags, const char *name);

uint32_t mux_ringbuffer_write(mux_ringbuffer_t *rb, uint8_t *buffer, uint32_t len);
uint32_t mux_ringbuffer_write_try(mux_ringbuffer_t *rb, uint8_t *buffer, uint32_t len);
uint32_t mux_ringbuffer_write_buffer(mux_ringbuffer_t *src_rb, mux_ringbuffer_t *dest_rb, uint32_t len, uint16_t *crc);
uint32_t mux_ringbuffer_read(mux_ringbuffer_t *rb, uint8_t *buffer, uint32_t len);
uint32_t mux_ringbuffer_peek(mux_ringbuffer_t *rb, uint8_t *buffer, uint32_t len);

// bool mux_ringbuffer_get_data_buffer(mux_ringbuffer_t *rb, mux_buffer_t *buffer, uint32_t *buffer_count);

void mux_ringbuffer_reset(mux_ringbuffer_t *rb);
void mux_ringbuffer_free(mux_ringbuffer_t *rb);
void mux_ringbuffer_dump(mux_ringbuffer_t *rb, const char *file_name, bool is_dump_data);
// int mux_ringbuffer_register_ops(mux_ringbuffer_t *rb, mux_ringbuffer_ops_t *ops);

#define RB_TRXBUF_IDX(rb) (rb->flags&MUX_RB_FLAG_TRX_BUF_MASK)
#define RB_IS_RXBUF(rb) (!!(RB_TRXBUF_IDX(rb) == MUX_RB_FLAG_RX_BUF))
#define RB_IS_TXBUF(rb) (!!(RB_TRXBUF_IDX(rb) == MUX_RB_FLAG_TX_BUF))


ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ bool mux_ringbuffer_full(mux_ringbuffer_t *rb)
{
    mux_ringbuffer_ops_t *ops = rb->ops;
    if (ops) {
        return ops->rb_full(rb);
    } else {
        return ((rb->capacity == RB_TOTAL_DATA_SIZE(rb)) ? true : false);
    }
}

ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ bool mux_ringbuffer_empty(mux_ringbuffer_t *rb)
{
    mux_ringbuffer_ops_t *ops = rb->ops;
    if (ops) {
        return ops->rb_empty(rb);
    } else {
        return  ((rb->wptr.tail == rb->rptr.tail) ? true : false);
    }
}

ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ uint32_t mux_ringbuffer_data_length(mux_ringbuffer_t *rb)
{
    mux_ringbuffer_ops_t *ops = rb->ops;

    if (!!ops) {
        return ops->rb_data_size(rb);
    } else {
        return RB_TOTAL_DATA_SIZE(rb);
    }
}

ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ uint32_t mux_ringbuffer_free_space(mux_ringbuffer_t *rb)
{
    mux_ringbuffer_ops_t *ops = rb->ops;

    if (ops) {
        return ops->rb_free_space(rb);
    } else {
        return RB_TOTAL_FREE_SPACE_SIZE(rb);
    }
}

#if !defined(MTK_SINGLE_CPU_ENV) && defined(MULTI_CORE_ENABLE)
#define MULTI_CORE_ENTER_CRITICAL_SECTION()                                    \
  while (1) {                                                                  \
    if (hal_hw_semaphore_give(HW_SEMAPHORE_MUX) !=                             \
        HAL_HW_SEMAPHORE_STATUS_OK) {                                          \
      assert(0);                                                               \
    }                                                                          \
  }
#define MULTI_CORE_EXIT_CRITICAL_SECTION()                                     \
  do {                                                                         \
    if (hal_hw_semaphore_give(HW_SEMAPHORE_MUX) !=                             \
        HAL_HW_SEMAPHORE_STATUS_OK) {                                          \
      assert(0);                                                               \
    }                                                                          \
  } while (0)
#else
#define MULTI_CORE_ENTER_CRITICAL_SECTION()
#define MULTI_CORE_EXIT_CRITICAL_SECTION()
#endif
ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ void mux_ringbuffer_read_rptr_update(mux_ringbuffer_t *rb, uint32_t len) {
  mux_ringbuffer_ops_t *ops = rb->ops;
  if (ops) {
    ops->rb_set_rx_rptr(rb, len);
  }
}

ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ void mux_ringbuffer_write_wptr_update(mux_ringbuffer_t *rb, uint32_t len) {
  mux_ringbuffer_ops_t *ops = rb->ops;
  if (ops) {
    ops->rb_set_tx_wptr(rb, len);
  }
}

ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ uint32_t mux_ringbuffer_read_move_head(mux_ringbuffer_t *rb, uint32_t *old_head, uint32_t *new_head, uint32_t len)
{
  uint32_t read_len;
  uint32_t size = len;
  uint32_t total_len;
  mux_ringbuffer_node_t *node;
#ifdef RB_DIR_TYPE_CHECK_ENABLE
  mux_ringbuffer_ops_t *ops = rb->ops;
  if (!!ops && RB_IS_TXBUF(rb)) {
      RB_LOG_E("[mux_ringbuffer_read_move_head] txbuff of HW FIFO can not be read by SW!!", 0);
      assert(0);
      return 0xFFFFFFFF;
  }
#endif
  node = &rb->rptr;
    //TODO: Hw port txbuf,SW can only write, cannot read, hW automatically read by DMA, txbuf read pointer is managed by HW

  // printf("[%s] get size to read=%d, %d\n", __func__,size, cnt);
  // step1. Use head to take position before reading ring buffer
  do {
    /* First save the old head, which is the position where the last read was completed, and then copy data from the FIFO from here   */
    *old_head = node->head;
    // printf("[%s] get *old_head=%d, %d\n", __func__,*old_head, cnt);
    /* Calculate the length of the data in the buffer and take the minimum value of the length and the data length in the FIFO   */
    total_len = RB_TOTAL_DATA_SIZE(rb);
    read_len = RB_MIN(size, total_len);
    // printf("[%s] get read_len=%d, %d\n", __func__,read_len, cnt);
    /* new position of head, placeholder first */
    *new_head = *old_head + read_len;
    // printf("[%s] get *new_head=%d, %d\n", __func__,*new_head, cnt);
    /* Let's see if head is still equal to *old_head. If not,  it means before current time, this task(Task1) is interrupted by another task(task2) ,
    task2 has updated the head, and this task needs to re-read the head and re-occupy the space  */
  } while (!cas(&node->head, *old_head, *new_head));

  // *old_head = __old_head;
  // *old_head = __new_head;
  return read_len;
}

ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ void mux_ringbuffer_read_move_data(mux_ringbuffer_t *rb, uint8_t *buffer, uint32_t len, uint32_t rptr_old_head)
{
#ifdef RB_DIR_TYPE_CHECK_ENABLE
  if (!!rb->ops && RB_IS_TXBUF(rb)) {
      RB_LOG_E("[mux_ringbuffer_read_move_data] txbuff of HW FIFO can not be read by SW!!", 0);
      assert(0);
      return ;
  }
#endif
  // step2
  // Head placeholder succeeded  ，copy data to fifo
  // TODO:You can optimize with loop unrolling
  // TODO:If direct memcpy is not correct, consider ringbuffer segmentation
  /* Copy RPTR to the end of the buffer and then copy the rest of the beginning of the buffer   */

    uint32_t read_len_head;
    read_len_head = RB_MIN(len, rb->capacity - (rptr_old_head&rb->mask));
    memcpy(buffer, MUX_RB_SWITCH_START_MEMVIEW(rb) + (rptr_old_head&rb->mask), read_len_head);
    memcpy(buffer + read_len_head, MUX_RB_SWITCH_START_MEMVIEW(rb), len - read_len_head);
}
ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ void mux_ringbuffer_write_move_ht_su(mux_ringbuffer_t *rb, uint32_t move_bytes)
{
  RB_LOG_D("[%02X] swptr:%d -> %d, move_bytes=%d", 4, rb->uid, rb->swptr, rb->swptr + move_bytes, move_bytes);
  rb->swptr += move_bytes;
}

ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ void mux_ringbuffer_write_move_ht_st(mux_ringbuffer_t *rb, uint32_t move_bytes)
{
  RB_LOG_D("[%02X] wptr.tail:%d -> %d, move_bytes=%d", 4, rb->uid, rb->wptr.tail, rb->wptr.tail + move_bytes, move_bytes);

  rb->wptr.head += move_bytes;
  rb->wptr.tail = rb->wptr.head;

}

ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ void mux_ringbuffer_read_move_ht_su(mux_ringbuffer_t *rb, uint32_t move_bytes)
{
  RB_LOG_D("[%02X] srptr:%d -> %d, move_bytes=%d", 4, rb->uid, rb->srptr, rb->srptr + move_bytes, move_bytes);

  rb->srptr += move_bytes;

}
ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ void mux_ringbuffer_read_move_ht_st(mux_ringbuffer_t *rb, uint32_t move_bytes)
{
  RB_LOG_D("[%02X] rptr.tail:%d -> %d, move_bytes=%d", 4, rb->uid, rb->rptr.tail, rb->rptr.tail + move_bytes, move_bytes);

  rb->rptr.head += move_bytes;
  rb->rptr.tail = rb->rptr.head;

}

ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ void mux_ringbuffer_read_move_hw_tail_st(mux_ringbuffer_t *rb, uint32_t move_bytes)
{
  mux_ringbuffer_ops_t *ops = rb->ops;
  if (!!ops) {
      ops->rb_set_rx_rptr(rb, move_bytes);
  }
}
//move hw rptr/ sw rptr
ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ void mux_ringbuffer_read_move_hw_tail_ht_st(mux_ringbuffer_t *rb, uint32_t move_bytes)
{
    mux_ringbuffer_read_move_hw_tail_st(rb, move_bytes);
    //we assume that the data is send out when hw tail update done. so, just move the rptr tail
    //mux_ringbuffer_read_move_ht_st(rb, move_bytes);
}

ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ void mux_ringbuffer_read_move_tail(mux_ringbuffer_t *rb, uint32_t old_head, uint32_t new_head)
{
  mux_ringbuffer_node_t *node;
#ifdef RB_DIR_TYPE_CHECK_ENABLE
  if (!!rb->ops && RB_IS_TXBUF(rb)) {
      RB_LOG_E("txbuff of HW FIFO can not be read by SW!!", 0);
      assert(0);
      return ;
  }
#endif
  node = &rb->rptr;
  /* If another task2 preempts this task1 to read data into the FIFO between step2 and step3, Task2 sees that RPTR ->head is already the updated value  :
  1. If task2 succeeds in executing step1 (placeholder)
  2. Task3 then preempts in task2's steps 2 to 3, and task3 executes step1(placeholder) successfully
  3. Task4 then preempts in task3's steps 2 to 3, and task4 executes step1(placeholder) successfully
      .....

  4.
  In the worst case, if n tasks are preempted, the preempted task will resume reading data when the time slice expires, and you will see a lot of holes in the buffer
  Therefore, when Task2 preempts Task1, task2 should wait for Task1 to read the data into buffer before continuing
  */

  // step3，(current task)update rptr ->tail to new_head when rptr ->tail is equal to rptr ->old_head (previous task completed)
  // update tail of rptr
  // Wait for the last task to finish data read, then update tail to the last task's new_head (The old head of this task)
  // TODO:If the previous task has a low priority and will wait for a long time hear, consider adding a timeout to suspend the task  after timeout

  while (!cas(&node->tail, old_head, new_head));
  // mux_ringbuffer_read_move_hw_tail_ht_st(rb, new_head - old_head);
  // printf("[%s] get tail=%d, %d\n", __func__,node->tail, cnt);
}

ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ uint32_t mux_ringbuffer_read_move_head_st(mux_ringbuffer_t *rb, uint32_t *old_head, uint32_t len)
{
    uint32_t read_len;
    uint32_t size = len;
    uint32_t data_len;
    mux_ringbuffer_node_t *node;
#ifdef RB_DIR_TYPE_CHECK_ENABLE
    mux_ringbuffer_ops_t *ops = rb->ops;
    if (!!ops && RB_IS_TXBUF(rb)) {
        RB_LOG_E("txbuff of HW FIFO can not be read by SW!!", 0);
        assert(0);
        return 0xFFFFFFFF;
    }
#endif
    node = &rb->rptr;

    *old_head = node->head;
    data_len = mux_ringbuffer_data_length(rb);
    read_len = RB_MIN(size, data_len);
    //TODO:check if read_len exceed the max packet size?
    node->head = *old_head + read_len;

    return read_len;
}

ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ void mux_ringbuffer_read_move_tail_st(mux_ringbuffer_t *rb, uint32_t move_bytes)
{
#ifdef RB_DIR_TYPE_CHECK_ENABLE
  if (!!rb->ops && RB_IS_TXBUF(rb)) {
      RB_LOG_E("txbuff of HW FIFO can not be read by SW!!",0);
      assert(0);
      return ;
  }
#endif
  RB_LOG_D("[%02X] rptr.tail:%d -> %d, move_bytes=%d", 4, rb->uid, rb->rptr.tail, rb->rptr.tail + move_bytes, move_bytes);

  rb->rptr.tail += move_bytes;
//   mux_ringbuffer_read_move_hw_tail_ht_st(rb, move_bytes);
}

ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ uint32_t mux_ringbuffer_read_data_st(mux_ringbuffer_t *rb, uint8_t *buffer, uint32_t len)
{
    uint32_t read_len;
    uint32_t old_head;

    read_len = mux_ringbuffer_read_move_head_st(rb, &old_head, len);
    mux_ringbuffer_read_move_data(rb, buffer, read_len, old_head);

    return read_len;
}

ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ uint32_t mux_ringbuffer_read_st(mux_ringbuffer_t *rb, uint8_t *buffer, uint32_t len)
{
    uint32_t read_len;

    read_len = mux_ringbuffer_read_data_st(rb, buffer, len);

    mux_ringbuffer_read_move_tail_st(rb, read_len);

    return read_len;
}


ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ uint32_t mux_ringbuffer_write_move_head(mux_ringbuffer_t *rb, uint32_t *old_head, uint32_t *new_head, uint32_t len)
{
  mux_ringbuffer_node_t *node = &rb->wptr;
  uint32_t write_len;
  uint32_t size = len;
  uint32_t free_size;
#ifdef RB_DIR_TYPE_CHECK_ENABLE
  mux_ringbuffer_ops_t *ops = rb->ops;
  if (!!ops && RB_IS_RXBUF(rb)) {
      RB_LOG_E("rxbuff of HW FIFO can not be write by SW!!", 0);
      assert(0);
      return 0xFFFFFFFF;
  }
#endif
  /* step 1: placeholders by wptr->head */
  bool ok;
  int cnt = 0;

  // printf("[%s] get size to write=%d, %d\n", __func__,size, cnt);

  do {
    /* First save the old head, which is the location of the last write done, and then copy data into the FIFO from here */
    *old_head = node->head;
    //RB_LOG_D("[mux_ringbuffer_write_move_head] *old_head=%d size=%d\n", 2,*old_head, size);
    free_size = RB_TOTAL_FREE_SPACE_SIZE(rb);
    write_len = RB_MIN(size, free_size);
    //RB_LOG_D("[mux_ringbuffer_write_move_head] write_len=%d\n", 1,write_len);
    *new_head = *old_head + write_len;
    // printf("[%s] get *new_head=%d, %d\n", __func__,*new_head, cnt);
    ok = cas(&node->head, *old_head, *new_head);
    cnt++;
  } while (!ok);
    //RB_LOG_D("[mux_ringbuffer_write_move_head] write_len=%d, %d\n", 2,write_len, cnt);

  return write_len;
}

/*Copy the wptr to the end of the buffer and then copy the rest of the beginning of the buffer   */
#define RB_WRITE_DATA(rb, data, write_len, wptr_old_head)\
{\
    uint32_t write_len_head;\
    write_len_head = RB_MIN(write_len, rb->capacity - (wptr_old_head&rb->mask));\
    memcpy(MUX_RB_SWITCH_START_MEMVIEW(rb) + (wptr_old_head&rb->mask), data, write_len_head);\
    memcpy(MUX_RB_SWITCH_START_MEMVIEW(rb), data + write_len_head, write_len - write_len_head);\
}

ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ void mux_ringbuffer_write_move_data(mux_ringbuffer_t *rb, uint8_t *buffer, uint32_t len, uint32_t wptr_old_head)
{
#ifdef RB_DIR_TYPE_CHECK_ENABLE
  if (!!rb->ops && RB_IS_RXBUF(rb)) {
      RB_LOG_E("rxbuff of HW FIFO can not be write by SW!!", 0);
      assert(0);
      return ;
  }
#endif
    uint32_t write_len_head;
    write_len_head = RB_MIN(len, rb->capacity - (wptr_old_head&rb->mask));
    memcpy(MUX_RB_SWITCH_START_MEMVIEW(rb) + (wptr_old_head&rb->mask), buffer, write_len_head);
    memcpy(MUX_RB_SWITCH_START_MEMVIEW(rb), buffer + write_len_head, len - write_len_head);
}


ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ void mux_ringbuffer_write_move_hw_tail_st(mux_ringbuffer_t *rb, uint32_t move_bytes)
{
    mux_ringbuffer_ops_t *ops = rb->ops;
    if (!!ops) {
        ops->rb_set_tx_wptr(rb, move_bytes);
    }
    //we assume that the data is send out when hw tail update done. so, just move the rptr tail
    mux_ringbuffer_read_move_ht_st(rb, move_bytes);
}

ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ void mux_ringbuffer_write_move_tail(mux_ringbuffer_t *rb, uint32_t old_head, uint32_t new_head)
{
    mux_ringbuffer_node_t *node;
#ifdef RB_DIR_TYPE_CHECK_ENABLE
    if (!!rb->ops && RB_IS_RXBUF(rb)) {
        RB_LOG_E("rxbuff of HW FIFO can not be write by SW!!", 0);
        assert(0);
        return;
    }
#endif
    node = &rb->wptr;

    while (!cas(&node->tail, old_head, new_head));
    // mux_ringbuffer_write_move_hw_tail_st(rb, new_head - old_head);
    // printf("[%s] get tail=%d, %d\n", __func__,node->tail, cnt);
}

ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ uint32_t mux_ringbuffer_write_move_head_st(mux_ringbuffer_t *rb, uint32_t *old_head, uint32_t len)
{
    uint32_t write_len;
    uint32_t size = len;
    uint32_t free_size;
    mux_ringbuffer_node_t *node;
    // mux_ringbuffer_ops_t *ops = rb->ops;

    // if (!!ops && RB_IS_RXBUF(rb)) {
    //     RB_LOG_I("rxbuff of HW FIFO can not be write by SW!!", 0);
    //     assert(0);
    //     return 0xFFFFFFFF;
    // }

    node = &rb->wptr;

    *old_head = node->head;
    free_size = RB_TOTAL_FREE_SPACE_SIZE(rb);
    write_len = RB_MIN(size, free_size);
    node->head = *old_head + write_len;

    return write_len;
}

ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ void mux_ringbuffer_write_move_tail_st(mux_ringbuffer_t *rb, uint32_t move_bytes)
{
#ifdef RB_DIR_TYPE_CHECK_ENABLE
    if (!!rb->ops && RB_IS_RXBUF(rb)) {
        RB_LOG_E("rxbuff of HW FIFO can not be write by SW!!", 0);
        assert(0);
        return;
    }
#endif
    RB_LOG_D("[%02X] wptr.tail:%d -> %d, move_bytes=%d", 4, rb->uid, rb->wptr.tail, rb->wptr.tail + move_bytes, move_bytes);
    rb->wptr.tail += move_bytes;

    // mux_ringbuffer_write_move_hw_tail_st(rb, move_bytes);
}

ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ uint32_t mux_ringbuffer_write_data_st(mux_ringbuffer_t *rb, uint8_t *buffer, uint32_t len)
{
    uint32_t write_len;
    uint32_t old_head;

    write_len = mux_ringbuffer_write_move_head_st(rb, &old_head, len);

    mux_ringbuffer_write_move_data(rb, buffer, write_len, old_head);

    return write_len;
}
//move sw wptr head/tail, except hw wptr
ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ uint32_t mux_ringbuffer_write_st(mux_ringbuffer_t *rb, uint8_t *buffer, uint32_t len)
{
    uint32_t write_len;

    write_len = mux_ringbuffer_write_data_st(rb, buffer, len);

    mux_ringbuffer_write_move_tail_st(rb, write_len);

    return write_len;
}

#endif // __MUX_RINGBUFFER_H__
