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

#if defined(AIR_ULL_AUDIO_V2_DONGLE_ENABLE)

/* Includes ------------------------------------------------------------------*/
#include "scenario_ull_audio_v2.h"
#include "types.h"
#include "source_inter.h"
#include "sink_inter.h"
#include "dsp_buffer.h"
#include "dsp_memory.h"
#include "dsp_callback.h"
#include "dsp_temp.h"
#include "dsp_dump.h"
#include "dsp_scenario.h"
#include "dsp_audio_msg_define.h"
#include "dsp_audio_process.h"
#include "stream_audio_transmitter.h"
#include "stream_bt_common.h"
#include "bt_types.h"
#include "hal_gpt.h"
#include "hal_sleep_manager.h"
#include "memory_attribute.h"
#include "clk_skew_sw.h"
#include "sw_gain_interface.h"
#include "sw_buffer_interface.h"
#include "lc3plus_enc_interface.h"
#include "lc3plus_dec_interface.h"

/* Private define ------------------------------------------------------------*/
#define ULL_AUDIO_V2_DONGLE_DL_PATH_DEBUG_DUMP                      1
#define ULL_AUDIO_V2_DONGLE_UL_PATH_DEBUG_DUMP                      1
#define ULL_AUDIO_V2_DONGLE_DL_PATH_DEBUG_LOG                       0
#define ULL_AUDIO_V2_DONGLE_UL_PATH_DEBUG_LOG                       0

#define UL_BT_TIMESTAMP_DIFF                                        (2)  /* 0.625ms/0.3125ms */
#define UL_PLAYEN_DELAY_FRAME_5000US                                (10-4) /* procsess time - 1ms */
#define UL_FIRST_PACKET_SAFE_DELAY_FRAME_5000US                     (2)
#define UL_BT_RETRY_WINDOW_FRAME_5000US                             (18)
#define UL_BT_INTERVAL_FRAME_5000US                                 (16)
#define DL_CLOCK_SKEW_CHECK_COUNT                                   (4)
#define UL_CLOCK_SKEW_CHECK_COUNT                                   (4)

#ifndef min
#define min(_a, _b)   (((_a)<(_b))?(_a):(_b))
#endif

#ifndef max
#define max(_a, _b)   (((_a)<(_b))?(_b):(_a))
#endif

/* Private typedef -----------------------------------------------------------*/
typedef struct  {
    U16 DataOffset; /* offset of payload */
    U16 _reserved_word_02h;
    U32 TimeStamp; /* this CIS/BIS link's CLK, Unit:312.5us */
    U16 ConnEvtCnt; /* event count seem on airlog, for debug propose */
    U8 SampleSeq;  /* Sameple sequence of this SDU interval Ex:0,1,2... */
    U8 _reserved_byte_0Bh;
    U8 PduHdrLo;
    // U8 _reserved_byte_0Dh;
    U8 valid_packet; /* valid packet: 0x01, invalid packet 0x00 */
    U8 PduLen ; /* payload size */
    U8 _reserved_byte_0Fh;
    U16 DataLen;
    U16 _reserved_word_12h;
    // U32 _reserved_long_0;
    U32 crc32_value;
    U32 _reserved_long_1;
} ULL_AUDIO_V2_HEADER;

typedef struct {
    uint8_t scenario_type;
    uint8_t scenario_sub_id;
} dl_mixer_param_t;

typedef struct {
    uint32_t vol_ch;
    int32_t  vol_gain;
} vol_gain_t;

typedef struct {
    uint8_t chx_choose;
    uint8_t ch1_connection;
    uint8_t ch2_connection;
    uint8_t ch3_connection;
    uint8_t ch4_connection;
    uint8_t ch5_connection;
    uint8_t ch6_connection;
    uint8_t ch7_connection;
    uint8_t ch8_connection;
} ull_audio_v2_connection_info_t;

typedef union {
    vol_gain_t                vol_gain;
    dl_mixer_param_t          dl_mixer_param;
} ull_audio_v2_dongle_runtime_config_operation_param_t, *ull_audio_v2_dongle_runtime_config_operation_param_p;

typedef struct {
    ull_audio_v2_dongle_runtime_config_operation_t          config_operation;
    ull_audio_v2_dongle_runtime_config_operation_param_t    config_param;
} ull_audio_v2_runtime_config_param_t, *ull_audio_v2_dongle_runtime_config_param_p;

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
} ull_audio_v2_init_play_info_t;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static ull_audio_v2_dongle_bt_info_t ull_audio_v2_dl_bt_info[ULL_AUDIO_V2_DATA_CHANNEL_NUMBER];
static ull_audio_v2_dongle_bt_info_t ull_audio_v2_ul_bt_info[ULL_AUDIO_V2_DATA_CHANNEL_NUMBER];
static uint32_t crc32_tab[] = {
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
    0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
    0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
    0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
    0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
    0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
    0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
    0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
    0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
    0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
    0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
    0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
    0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
    0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
    0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
    0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
    0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
    0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
    0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
    0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
    0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
    0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
    0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
    0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
    0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
    0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
    0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
    0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
    0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
    0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
    0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
    0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
    0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};
static ull_audio_v2_init_play_info_t ull_audio_v2_bt_init_play_info =
{
    .dl_retransmission_window_clk = UL_BT_RETRY_WINDOW_FRAME_5000US,
};

/* Public variables ----------------------------------------------------------*/
ull_audio_v2_dongle_dl_handle_t *ull_audio_v2_dongle_first_dl_handle = NULL;
ull_audio_v2_dongle_ul_handle_t *ull_audio_v2_dongle_first_ul_handle = NULL;

/* Private functions ---------------------------------------------------------*/
EXTERN VOID StreamDSP_HWSemaphoreTake(VOID);
EXTERN VOID StreamDSP_HWSemaphoreGive(VOID);
extern VOID audio_transmitter_share_information_update_read_offset(SOURCE source, U32 ReadOffset);
extern VOID audio_transmitter_share_information_fetch(SOURCE source, SINK sink);
extern VOID audio_transmitter_share_information_update_write_offset(SINK sink, U32 WriteOffset);
extern void bt_common_share_information_fetch(SOURCE source, SINK sink);
extern void bt_common_share_information_update_read_offset(SOURCE source, U32 ReadOffset);
extern VOID audio_transmitter_share_information_fetch(SOURCE source, SINK sink);
extern VOID MCE_GetBtClk(BTCLK* pCurrCLK, BTPHASE* pCurrPhase, BT_CLOCK_OFFSET_SCENARIO type);

static uint32_t ull_audio_v2_codec_get_frame_size(audio_dsp_codec_type_t *codec_type, audio_codec_param_t *codec_param)
{
    uint32_t frame_size = 0;

    switch (*codec_type)
    {
        case AUDIO_DSP_CODEC_TYPE_LC3:
            frame_size = codec_param->lc3.bit_rate * codec_param->lc3.frame_interval / 8 / 1000 / 1000;
            break;

        case AUDIO_DSP_CODEC_TYPE_LC3PLUS:
            frame_size = codec_param->lc3plus.bit_rate * codec_param->lc3plus.frame_interval / 8 / 1000 / 1000;
            break;

        default:
            DSP_MW_LOG_E("[ULL Audio V2][ERROR]This codec is not supported at now, %u\r\n", 1, *codec_type);
            AUDIO_ASSERT(0);
            break;
    }

    return frame_size;
}

static uint32_t usb_audio_get_frame_size(audio_dsp_codec_type_t *usb_type, audio_codec_param_t *usb_param)
{
    uint32_t frame_size = 0;
    uint32_t samples = 0;
    uint32_t channel_num = 0;
    uint32_t resolution_size = 0;

    if (*usb_type == AUDIO_DSP_CODEC_TYPE_PCM)
    {
        frame_size = 1;

        switch (usb_param->pcm.frame_interval)
        {
            case 1000:
                break;

            default:
                AUDIO_ASSERT(0);
                break;
        }

        switch (usb_param->pcm.sample_rate)
        {
            case 44100:
                samples = 44;
                break;

            case 16000:
            case 32000:
            case 48000:
            case 96000:
                samples = usb_param->pcm.sample_rate/1000;
                break;

            default:
                AUDIO_ASSERT(0);
                break;
        }

        switch (usb_param->pcm.channel_mode)
        {
            case 1:
                channel_num = 1;
                break;

            case 2:
                channel_num = 2;
                break;

            case 8:
                channel_num = 8;
                break;

            default:
                AUDIO_ASSERT(0);
                break;
        }

        switch (usb_param->pcm.format)
        {
            case AFE_PCM_FORMAT_S16_LE:
                resolution_size = 2;
                break;

            case AFE_PCM_FORMAT_S24_LE:
                resolution_size = 3;
                break;

            default:
                AUDIO_ASSERT(0);
                break;
        }
    }
    else
    {
        frame_size = 0;
    }

    frame_size = frame_size * samples * channel_num * resolution_size;

    return frame_size;
}

ATTR_TEXT_IN_IRAM static void ShareBufferCopy_I_16bit_to_D_32bit_2ch(uint32_t* src_buf, uint32_t* dest_buf1, uint32_t* dest_buf2, uint32_t samples)
{
    uint32_t data;
    uint32_t i;

    for (i = 0; i < samples; i++)
    {
        data = src_buf[i];
        dest_buf1[i] = ((uint32_t)((uint16_t)(data>> 0)))<<16;
        dest_buf2[i] = ((uint32_t)((uint16_t)(data>>16)))<<16;
    }
}

ATTR_TEXT_IN_IRAM static void ShareBufferCopy_I_24bit_to_D_32bit_2ch(uint8_t* src_buf, uint32_t* dest_buf1, uint32_t* dest_buf2, uint32_t samples)
{
    uint32_t data32;
    uint16_t data16;
    uint32_t i;

    for (i = 0; i < samples; i++)
    {
        if ((i%2) == 0)
        {
            data32 = *((uint32_t *)(src_buf+i*6));
            data16 = *((uint16_t *)(src_buf+i*6+4));
            dest_buf1[i] = (data32&0x00ffffff)<<8;
            dest_buf2[i] = (((uint32_t)data16)<<16) | ((data32&0xff000000)>>16);
        }
        else
        {
            data16 = *((uint16_t *)(src_buf+i*6));
            data32 = *((uint32_t *)(src_buf+i*6+2));
            dest_buf1[i] = (((uint32_t)data16)<<8) | ((data32&0x000000ff)<<24);
            dest_buf2[i] = (data32&0xffffff00);
        }
    }
}

ATTR_TEXT_IN_IRAM static void ShareBufferCopy_I_16bit_to_D_32bit_8ch(uint32_t* src_buf, uint32_t* dest_buf1, uint32_t* dest_buf2, uint32_t* dest_buf3, uint32_t* dest_buf4, uint32_t* dest_buf5, uint32_t* dest_buf6, uint32_t* dest_buf7, uint32_t* dest_buf8, uint32_t samples)
{
    uint32_t data0;
    uint32_t data1;
    uint32_t data2;
    uint32_t data3;
    uint32_t i;

    for (i = 0; i < samples; i++)
    {
        data0 = src_buf[4*i + 0];
        data1 = src_buf[4*i + 1];
        data2 = src_buf[4*i + 2];
        data3 = src_buf[4*i + 3];
        dest_buf1[i] = ((uint32_t)((uint16_t)(data0>> 0)))<<16;
        dest_buf2[i] = ((uint32_t)((uint16_t)(data0>>16)))<<16;
        dest_buf3[i] = ((uint32_t)((uint16_t)(data1>> 0)))<<16;
        dest_buf4[i] = ((uint32_t)((uint16_t)(data1>>16)))<<16;
        dest_buf5[i] = ((uint32_t)((uint16_t)(data2>> 0)))<<16;
        dest_buf6[i] = ((uint32_t)((uint16_t)(data2>>16)))<<16;
        dest_buf7[i] = ((uint32_t)((uint16_t)(data3>> 0)))<<16;
        dest_buf8[i] = ((uint32_t)((uint16_t)(data3>>16)))<<16;
    }
}

ATTR_TEXT_IN_IRAM static void ShareBufferCopy_I_24bit_to_D_32bit_8ch(uint32_t* src_buf, uint32_t* dest_buf1, uint32_t* dest_buf2, uint32_t* dest_buf3, uint32_t* dest_buf4, uint32_t* dest_buf5, uint32_t* dest_buf6, uint32_t* dest_buf7, uint32_t* dest_buf8, uint32_t samples)
{
    uint32_t data0;
    uint32_t data1;
    uint32_t data2;
    uint32_t data3;
    uint32_t data4;
    uint32_t data5;
    uint32_t i;

    for (i = 0; i < samples; i++)
    {
        data0 = src_buf[6*i + 0];
        data1 = src_buf[6*i + 1];
        data2 = src_buf[6*i + 2];
        data3 = src_buf[6*i + 3];
        data4 = src_buf[6*i + 4];
        data5 = src_buf[6*i + 5];
        dest_buf1[i] = (data0&0x00ffffff)<<8;
        dest_buf2[i] = ((data1&0x0000ffff)<<16)|((data0&0xff000000)>>16);
        dest_buf3[i] = ((data2&0x000000ff)<<24)|((data1&0xffff0000)>>8);
        dest_buf4[i] = (data2&0xffffff00);
        dest_buf5[i] = (data3&0x00ffffff)<<8;
        dest_buf6[i] = ((data4&0x0000ffff)<<16)|((data3&0xff000000)>>16);
        dest_buf7[i] = ((data5&0x000000ff)<<24)|((data4&0xffff0000)>>8);
        dest_buf8[i] = (data5&0xffffff00);
    }
}

static void ShareBufferCopy_D_32bit_to_D_24bit_1ch(uint32_t* src_buf, uint8_t* dest_buf1, uint32_t samples)
{
    uint32_t data;
    uint32_t i;

    for (i = 0; i < samples; i++)
    {
        data = src_buf[i];
        *(dest_buf1+i*3+0) = (uint8_t)((data>> 8)&0xff);
        *(dest_buf1+i*3+1) = (uint8_t)((data>>16)&0xff);
        *(dest_buf1+i*3+2) = (uint8_t)((data>>24)&0xff);
    }
}

static void ShareBufferCopy_D_32bit_to_I_24bit_2ch(uint32_t* src_buf1, uint32_t* src_buf2, uint8_t* dest_buf1, uint32_t samples)
{
    uint32_t data1;
    uint32_t data2;
    uint32_t i;

    for (i = 0; i < samples; i++)
    {
        data1 = src_buf1[i];
        data2 = src_buf2[i];
        *(dest_buf1+i*6+0) = (uint8_t)((data1>> 8)&0xff);
        *(dest_buf1+i*6+1) = (uint8_t)((data1>>16)&0xff);
        *(dest_buf1+i*6+2) = (uint8_t)((data1>>24)&0xff);
        *(dest_buf1+i*6+3) = (uint8_t)((data2>> 8)&0xff);
        *(dest_buf1+i*6+4) = (uint8_t)((data2>>16)&0xff);
        *(dest_buf1+i*6+5) = (uint8_t)((data2>>24)&0xff);
    }
}

static void ShareBufferCopy_D_16bit_to_D_16bit_1ch(uint16_t* src_buf, uint16_t* dest_buf1, uint32_t samples)
{
    uint16_t data;
    uint32_t i;

    for (i = 0; i < samples; i++)
    {
        data = src_buf[i];
        *(dest_buf1+i) = data;
    }
}

static void ShareBufferCopy_D_16bit_to_I_16bit_2ch(uint16_t* src_buf1, uint16_t* src_buf2, uint16_t* dest_buf1, uint32_t samples)
{
    uint32_t data1;
    uint32_t data2;
    uint32_t i;

    for (i = 0; i < samples; i++)
    {
        data1 = src_buf1[i];
        data2 = src_buf2[i];
        data1 = (data1) | (data2 << 16);
        *((uint32_t* )dest_buf1+i) = data1;
    }
}

static void ShareBufferCopy_D_32bit_to_D_16bit_1ch(uint32_t* src_buf, uint16_t* dest_buf1, uint32_t samples)
{
    uint32_t data;
    uint32_t i;

    for (i = 0; i < samples; i++)
    {
        data = src_buf[i];
        *(dest_buf1+i) = (uint16_t)((data >> 16)&0xffff);
    }
}

static void ShareBufferCopy_D_32bit_to_I_16bit_2ch(uint32_t* src_buf1, uint32_t* src_buf2, uint16_t* dest_buf1, uint32_t samples)
{
    uint32_t data1;
    uint32_t data2;
    uint32_t i;

    for (i = 0; i < samples; i++)
    {
        data1 = src_buf1[i];
        data2 = src_buf2[i];
        data1 = ((data1 >> 16)&0x0000ffff) | ((data2 >> 0)&0xffff0000);
        *((uint32_t* )dest_buf1+i) = data1;
    }
}

static void ShareBufferCopy_D_16bit_to_I_24bit_2ch(uint16_t* src_buf1, uint16_t* src_buf2, uint8_t* dest_buf1, uint32_t samples)
{
    uint32_t data32;
    uint16_t data16;
    uint32_t i;

    for (i = 0; i < samples; i++)
    {
        if ((i%2) == 0)
        {
            data32 = (src_buf1[i]<<8); // 0x00XXXX00
            data16 = src_buf2[i]; //0xXXXX
            *(uint32_t *)(dest_buf1 + i*6) = data32;
            *(uint16_t *)(dest_buf1 + i*6 + 4) = data16;
        }
        else
        {
            data16 = (src_buf1[i]&0x00ff)<<8; //0xXX00
            data32 = (src_buf2[i]<<16) | ((src_buf1[i]&0xff00)>>8); // 0xXXXX00XX
            *(uint16_t *)(dest_buf1 + i*6) = data16;
            *(uint32_t *)(dest_buf1 + i*6 + 2) = data32;
        }
    }
}

static void ShareBufferCopy_D_16bit_to_D_24bit_1ch(uint16_t* src_buf1, uint8_t* dest_buf1, uint32_t samples)
{
    uint32_t data32;
    uint16_t data16;
    uint32_t i;

    for (i = 0; i < samples; i++)
    {
        if ((i%2) == 0)
        {
            if (i != (samples - 1))
            {
                data32 = (src_buf1[i]<<8); // 0x00XXXX00
                *(uint32_t *)(dest_buf1 + (i/2)*6) = data32;
            }
            else
            {
                /* prevent overflow */
                data32 = (src_buf1[i]<<8); // 0x00XXXX00
                *(uint16_t *)(dest_buf1 + (i/2)*6) = (uint16_t)(data32&0x0000ffff);
                *(uint8_t *)(dest_buf1 + (i/2)*6 + 2) = (uint8_t)((data32&0x00ff0000)>>16);
            }
        }
        else
        {
            data16 = src_buf1[i]; //0xXXXX
            *(uint16_t *)(dest_buf1 + (i/2)*6 + 4) = data16;
        }
    }
}

ATTR_TEXT_IN_IRAM static uint32_t CRC32_Generate(uint8_t *ptr, uint32_t length, uint32_t crc_init)
{
    const uint8_t *p;

    p = ptr;
    crc_init = crc_init ^ ~0U;

    while (length--) {
        crc_init = crc32_tab[(crc_init ^ *p++) & 0xFF] ^ (crc_init >> 8);
    }

    return crc_init ^ ~0U;
}

/******************************************************************************/
/*               ULL audio 2.0 dongle DL path Private Functions               */
/******************************************************************************/
static ull_audio_v2_dongle_dl_handle_t *ull_audio_v2_dongle_dl_get_handle(void *owner)
{
    uint32_t saved_mask;
    int16_t count;
    ull_audio_v2_dongle_dl_handle_t *dongle_handle = NULL;
    ull_audio_v2_dongle_dl_handle_t *c_handle = NULL;

    dongle_handle = malloc(sizeof(ull_audio_v2_dongle_dl_handle_t));
    if (dongle_handle == NULL)
    {
        AUDIO_ASSERT(0);
    }
    memset(dongle_handle, 0, sizeof(ull_audio_v2_dongle_dl_handle_t));

    dongle_handle->total_number = -1;
    dongle_handle->index = -1;
    dongle_handle->owner = owner;
    dongle_handle->next_dl_handle = NULL;

    hal_nvic_save_and_set_interrupt_mask(&saved_mask);

    if (ull_audio_v2_dongle_first_dl_handle == NULL)
    {
        /* there is no item on the handle list */
        dongle_handle->total_number = 1;
        dongle_handle->index = 1;

        /* set this handle as the head of the handle list */
        ull_audio_v2_dongle_first_dl_handle = dongle_handle;
    }
    else
    {
        /* there are other items on the handle list */
        count = 1;
        c_handle = ull_audio_v2_dongle_first_dl_handle;
        while (c_handle != NULL)
        {
            count += 1;

            c_handle->total_number += 1;
            if (c_handle->total_number <= 0)
            {
                /* error status */
                AUDIO_ASSERT(0);
            }

            if (c_handle->next_dl_handle == NULL)
            {
                /* c_handle is the last item on the list now */
                dongle_handle->total_number = c_handle->total_number;
                dongle_handle->index = c_handle->total_number;

                /* dongle_handle is the last item on the list now */
                c_handle->next_dl_handle = dongle_handle;

                break;
            }

            c_handle = c_handle->next_dl_handle;
        }
        if ((c_handle == NULL) || (dongle_handle->total_number != count))
        {
            AUDIO_ASSERT(0);
        }
    }

    hal_nvic_restore_interrupt_mask(saved_mask);

    return dongle_handle;
}

static void ull_audio_v2_dongle_dl_release_handle(ull_audio_v2_dongle_dl_handle_t *handle)
{
    uint32_t saved_mask, i, count;
    ull_audio_v2_dongle_dl_handle_t *l_handle = NULL;
    ull_audio_v2_dongle_dl_handle_t *c_handle = NULL;
    ull_audio_v2_dongle_dl_handle_t *dongle_handle = NULL;

    if ((handle == NULL) || (ull_audio_v2_dongle_first_dl_handle == NULL))
    {
        AUDIO_ASSERT(0);
    }

    hal_nvic_save_and_set_interrupt_mask(&saved_mask);

    if (ull_audio_v2_dongle_first_dl_handle->total_number <= 0)
    {
        /* error status */
        AUDIO_ASSERT(0);
    }
    else if ((ull_audio_v2_dongle_first_dl_handle->total_number == 1) &&
            (ull_audio_v2_dongle_first_dl_handle == handle))
    {
        /* this handle is only one item on handle list */
        if (ull_audio_v2_dongle_first_dl_handle->next_dl_handle != NULL)
        {
            AUDIO_ASSERT(0);
        }

        /* there is no item on the handle list */
        ull_audio_v2_dongle_first_dl_handle = NULL;
    }
    else if ((ull_audio_v2_dongle_first_dl_handle->total_number > 1) &&
            (ull_audio_v2_dongle_first_dl_handle == handle))
    {
        /* this handle is the first item on handle list, but there are other handles on the list */
        c_handle = ull_audio_v2_dongle_first_dl_handle;
        count = ull_audio_v2_dongle_first_dl_handle->total_number;
        for (i=0; i < count; i++)
        {
            c_handle->total_number -= 1;
            c_handle->index -= 1;

            c_handle = c_handle->next_dl_handle;
        }
        if (c_handle != NULL)
        {
            /* error status */
            AUDIO_ASSERT(0);
        }

        /* change the next iten to the head */
        ull_audio_v2_dongle_first_dl_handle = handle->next_dl_handle;
    }
    else
    {
        /* this handle is the middle item on handle list */
        c_handle = ull_audio_v2_dongle_first_dl_handle;
        count = ull_audio_v2_dongle_first_dl_handle->total_number;
        if (count == 1)
        {
            /* error status */
            AUDIO_ASSERT(0);
        }
        for (i=0; i < count; i++)
        {
            if (c_handle == handle)
            {
                if (dongle_handle != NULL)
                {
                    /* error status */
                    AUDIO_ASSERT(0);
                }

                /* find out the handle on the list */
                dongle_handle = handle;
                l_handle->next_dl_handle = c_handle->next_dl_handle;
            }

            if (dongle_handle == NULL)
            {
                /* only the total_number of the handle in front of the released handle needs to be decreased */
                c_handle->total_number -= 1;
            }
            else
            {
                /* Both the total_number and the index of the handle in back of the released handle need to be decreased */
                c_handle->total_number -= 1;
                c_handle->index -= 1;
            }

            l_handle = c_handle;
            c_handle = c_handle->next_dl_handle;
        }
        if ((c_handle != NULL) || (dongle_handle == NULL))
        {
            /* error status */
            AUDIO_ASSERT(0);
        }
    }

    hal_nvic_restore_interrupt_mask(saved_mask);

    free(handle);
}

ATTR_TEXT_IN_IRAM static void ull_audio_v2_dongle_dl_mixer_postcallback(sw_mixer_member_t *member, void *para, uint32_t *out_frame_size)
{
    uint32_t i;
    ull_audio_v2_dongle_dl_handle_t *c_handle;
    SOURCE source;
    bool all_streams_in_mixer = true;
    UNUSED(para);

    /* change this handle's data status */
    source = (SOURCE)(member->owner);
    c_handle = (ull_audio_v2_dongle_dl_handle_t *)source->transform->sink->param.bt_common.scenario_param.dongle_handle;
    c_handle->data_status = ULL_AUDIO_V2_DONGLE_DL_DATA_IN_MIXER;

    /* check if this stream is unmixed or the first packet is not ready */
    if (c_handle->mixer_status == ULL_AUDIO_V2_DONGLE_MIXER_UNMIX)
    {
        /* clear this stream's input buffer */
        stream_function_sw_mixer_member_input_buffers_clean(c_handle->mixer_member, false);

        /* change this handle data status */
        c_handle->data_status = ULL_AUDIO_V2_DONGLE_DL_DATA_EMPTY;

        /* this stream is unmixed now, so we can directly return here */
        return;
    }
    else if (c_handle->first_packet_status == ULL_AUDIO_V2_DONGLE_DL_FIRST_PACKET_NOT_READY)
    {
        /* clear this stream's input buffer */
        stream_function_sw_mixer_member_input_buffers_clean(c_handle->mixer_member, false);

        /* change this handle data status */
        c_handle->data_status = ULL_AUDIO_V2_DONGLE_DL_DATA_EMPTY;

        /* in here, it means it is the first packet, we need to drop the later sink flow */
        *out_frame_size = 0;

        return;
    }

    /* check if all mixed stream data is in this mixer */
    c_handle = ull_audio_v2_dongle_first_dl_handle;
    for (i = 0; i < (uint32_t)ull_audio_v2_dongle_first_dl_handle->total_number; i++)
    {
        if ((c_handle->data_status == ULL_AUDIO_V2_DONGLE_DL_DATA_IN_STREAM) &&
            (c_handle->mixer_status != ULL_AUDIO_V2_DONGLE_MIXER_UNMIX) &&
            (c_handle->first_packet_status != ULL_AUDIO_V2_DONGLE_DL_FIRST_PACKET_NOT_READY))
        {
            /* this stream should be mixed, but its data is not in this mixer now.
               so we need to bypass this mix result in this time */
            all_streams_in_mixer = false;
            break;
        }
        // DSP_MW_LOG_W("[ULL Audio V2][DL][handle 0x%x]all_streams_in_mixer %d, %d, %d, %d\r\n", 5, c_handle, all_streams_in_mixer, c_handle->data_status, c_handle->mixer_status, c_handle->first_packet_status);
        /* switch to the next dl stream */
        c_handle = c_handle->next_dl_handle;
    }
    /* check if the output data in this time includes all stream data */
    if ((*out_frame_size != 0) && (all_streams_in_mixer == true))
    {
        /* all_streams_in_mixer is true, so all stream data is in this mixer.
           So we can clean all mixed streams' input buffers now and the mix result are sent to the sink */
        c_handle = ull_audio_v2_dongle_first_dl_handle;
        for (i = 0; i < (uint32_t)ull_audio_v2_dongle_first_dl_handle->total_number; i++)
        {
            if (c_handle->mixer_status != ULL_AUDIO_V2_DONGLE_MIXER_UNMIX)
            {
                /* clear this stream's input buffer */
                stream_function_sw_mixer_member_input_buffers_clean(c_handle->mixer_member, false);

                /* change this handle data status */
                c_handle->data_status = ULL_AUDIO_V2_DONGLE_DL_DATA_EMPTY;
            }
            /* switch to the next dl stream */
            c_handle = c_handle->next_dl_handle;
        }
    }
    else
    {
        /* all_streams_in_mixer is false, so some stream data is not in this mixer.
           So we need to bypass this mix result */
        *out_frame_size = 0;
    }
}

ATTR_TEXT_IN_IRAM static void ull_audio_v2_dongle_dl_ccni_handler(hal_ccni_event_t event, void * msg)
{
    SINK sink;
    uint32_t saved_mask;
    uint32_t i;
    uint32_t gpt_count;
    uint32_t bt_count;
    uint32_t blk_index;
    hal_ccni_message_t *ccni_msg = msg;
    ull_audio_v2_dongle_dl_handle_t *c_handle;
    static uint32_t error_count = 0;
    UNUSED(event);

    /* check if there is any DL stream */
    if (ull_audio_v2_dongle_first_dl_handle == NULL)
    {
        if ((error_count%1000) == 0)
        {
            DSP_MW_LOG_W("[ULL Audio V2][DL][WARNNING]dl dongle_handle is NULL, %d\r\n", 1, error_count);
        }
        error_count += 1;
        goto _ccni_return;
    }
    else
    {
        error_count = 0;
    }

    hal_nvic_save_and_set_interrupt_mask(&saved_mask);
    /* get timestamp for debug */
    bt_count = ccni_msg->ccni_message[0];
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &gpt_count);
    /* BT controller will send write index info on ccni msg */
    blk_index = (uint16_t)(ccni_msg->ccni_message[1]);
    hal_nvic_restore_interrupt_mask(saved_mask);

    /* trigger all started DL stream one by one */
    c_handle = ull_audio_v2_dongle_first_dl_handle;
    for (i = 0; i < (uint32_t)ull_audio_v2_dongle_first_dl_handle->total_number; i++)
    {
        if ((c_handle->stream_status == ULL_AUDIO_V2_DONGLE_STREAM_START) || (c_handle->stream_status == ULL_AUDIO_V2_DONGLE_STREAM_RUNNING))
        {
            switch (c_handle->sub_id)
            {
                case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0:
                case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_1:
                    /* get sink */
                    sink = (SINK)c_handle->owner;
                    if ((sink == NULL) || (sink->transform == NULL))
                    {
                        break;
                    }
                    /* set timestamp for debug */
                    c_handle->ccni_in_bt_count  = bt_count;
                    c_handle->ccni_in_gpt_count = gpt_count;
                    /* set blk index */
                    c_handle->ccni_blk_index    = blk_index;
                    /* increase fetch count */
                    hal_nvic_save_and_set_interrupt_mask(&saved_mask);
                    c_handle->fetch_count += 1;
                    hal_nvic_restore_interrupt_mask(saved_mask);
                    /* Handler the stream */
                    AudioCheckTransformHandle(sink->transform);
                    break;

                default:
                    AUDIO_ASSERT(0);
                    break;
            }
        }

        /* switch to the next dl stream */
        c_handle = c_handle->next_dl_handle;
    }
    if (c_handle != NULL)
    {
        AUDIO_ASSERT(0);
    }

_ccni_return:
    return;
}

static void ull_audio_v2_dongle_dl_bt_out_init(ull_audio_v2_dongle_dl_handle_t *dongle_handle, audio_transmitter_open_param_p audio_transmitter_open_param, bt_common_open_param_p bt_common_open_param)
{
    uint32_t i, j;
    uint32_t frame_interval;
    uint32_t sample_rate;
    hal_audio_format_t sample_format;
    uint32_t frame_samples;
    uint32_t frame_size;
    uint32_t saved_mask;
    uint32_t bit_rate;
    audio_dsp_codec_type_t codec_type;
    uint32_t channel_num = 0;
    UNUSED(audio_transmitter_open_param);

    for (i = 0; i < ULL_AUDIO_V2_DATA_CHANNEL_NUMBER; i++)
    {
        if (bt_common_open_param->scenario_param.ull_audio_v2_dongle_param.bt_link_info[i].share_info == NULL)
        {
            continue;
        }
        else
        {
            /* update channel num */
            channel_num += 1;
            /* update bt link settings to global state machine */
            for (j = 0; j < ULL_AUDIO_V2_DATA_CHANNEL_NUMBER; j++)
            {
                if (hal_memview_cm4_to_dsp0((uint32_t)(bt_common_open_param->scenario_param.ull_audio_v2_dongle_param.bt_link_info[i].share_info)) == (uint32_t)(ull_audio_v2_dl_bt_info[j].bt_link_info.share_info))
                {
                    /* check bt link settings */
                    if (memcmp(&(bt_common_open_param->scenario_param.ull_audio_v2_dongle_param.bt_link_info[i].codec_type), &(ull_audio_v2_dl_bt_info[j].bt_link_info.codec_type), sizeof(ull_audio_v2_dongle_bt_link_info_t)-sizeof(void *)) != 0)
                    {
                        /* same share buffer, same codec settings */
                        DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR]codec setting is different\r\n", 0);
                        AUDIO_ASSERT(0);
                    }
                    /* in here, it means this bt link's setting has been used */
                    hal_nvic_save_and_set_interrupt_mask(&saved_mask);
                    ull_audio_v2_dl_bt_info[j].user_count += 1;
                    hal_nvic_restore_interrupt_mask(saved_mask);
                    dongle_handle->sink_info.bt_out.bt_info[i] = &ull_audio_v2_dl_bt_info[j];
                }
            }
            if (dongle_handle->sink_info.bt_out.bt_info[i] == NULL)
            {
                /* in here, it means this bt link's setting has not been used */
                for (j = 0; j < ULL_AUDIO_V2_DATA_CHANNEL_NUMBER; j++)
                {
                    hal_nvic_save_and_set_interrupt_mask(&saved_mask);
                    if (ull_audio_v2_dl_bt_info[j].bt_link_info.share_info == NULL)
                    {
                        if (ull_audio_v2_dl_bt_info[j].user_count != 0)
                        {
                            DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR]user count is error, %u\r\n", 1, ull_audio_v2_dl_bt_info[j].user_count);
                            AUDIO_ASSERT(0);
                        }
                        ull_audio_v2_dl_bt_info[j].user_count += 1;
                        ull_audio_v2_dl_bt_info[j].seq_num = 0;
                        ull_audio_v2_dl_bt_info[j].blk_index = 0;
                        ull_audio_v2_dl_bt_info[j].blk_index_previous = 0;
                        ull_audio_v2_dl_bt_info[j].crc32_init = (0xfffffff0 | i);
                        memcpy(&(ull_audio_v2_dl_bt_info[j].bt_link_info), &(bt_common_open_param->scenario_param.ull_audio_v2_dongle_param.bt_link_info[i]), sizeof(ull_audio_v2_dongle_bt_link_info_t));
                        ull_audio_v2_dl_bt_info[j].bt_link_info.share_info = (void *)hal_memview_cm4_to_dsp0((uint32_t)(ull_audio_v2_dl_bt_info[j].bt_link_info.share_info));
                        dongle_handle->sink_info.bt_out.bt_info[i] = &ull_audio_v2_dl_bt_info[j];
                        hal_nvic_restore_interrupt_mask(saved_mask);
                        break;
                    }
                    hal_nvic_restore_interrupt_mask(saved_mask);
                }
                if (dongle_handle->sink_info.bt_out.bt_info[i] == NULL)
                {
                    /* not found suitable bt info channel */
                    DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR]all bt info are used\r\n", 0);
                    AUDIO_ASSERT(0);
                }
            }
            if ((dongle_handle->sink_info.bt_out.bt_info[i])->bt_link_info.codec_type == AUDIO_DSP_CODEC_TYPE_LC3PLUS)
            {
                codec_type     = AUDIO_DSP_CODEC_TYPE_LC3PLUS;
                frame_interval = (dongle_handle->sink_info.bt_out.bt_info[i])->bt_link_info.codec_param.lc3plus.frame_interval;
                sample_rate    = (dongle_handle->sink_info.bt_out.bt_info[i])->bt_link_info.codec_param.lc3plus.sample_rate;
                sample_format  = (dongle_handle->sink_info.bt_out.bt_info[i])->bt_link_info.codec_param.lc3plus.sample_format;
                bit_rate       = (dongle_handle->sink_info.bt_out.bt_info[i])->bt_link_info.codec_param.lc3plus.bit_rate;
                frame_samples  = sample_rate/1000*frame_interval/1000;
                frame_size     = ull_audio_v2_codec_get_frame_size(&((dongle_handle->sink_info.bt_out.bt_info[i])->bt_link_info.codec_type), &((dongle_handle->sink_info.bt_out.bt_info[i])->bt_link_info.codec_param));
            }
            else
            {
                DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR]codec is not support, %u\r\n", 1, (dongle_handle->sink_info.bt_out.bt_info[i])->bt_link_info.codec_type);
                AUDIO_ASSERT(0);
            }
            /* update codec type */
            if (dongle_handle->sink_info.bt_out.codec_type == 0)
            {
                dongle_handle->sink_info.bt_out.codec_type = codec_type;
            }
            else if (dongle_handle->sink_info.bt_out.codec_type != codec_type)
            {
                DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR]codec type is not right, %u, %u\r\n", 2, dongle_handle->sink_info.bt_out.codec_type, codec_type);
                AUDIO_ASSERT(0);
            }
            /* update frame interval */
            if (dongle_handle->sink_info.bt_out.frame_interval == 0)
            {
                dongle_handle->sink_info.bt_out.frame_interval = frame_interval;
            }
            else if (dongle_handle->sink_info.bt_out.frame_interval != frame_interval)
            {
                DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR]frame interval is not right, %u, %u\r\n", 2, dongle_handle->sink_info.bt_out.frame_interval, frame_interval);
                AUDIO_ASSERT(0);
            }
            /* update frame sample rate */
            if (dongle_handle->sink_info.bt_out.sample_rate == 0)
            {
                dongle_handle->sink_info.bt_out.sample_rate = sample_rate;
            }
            else if (dongle_handle->sink_info.bt_out.sample_rate != sample_rate)
            {
                DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR]sample rate is not right, %u, %u\r\n", 2, dongle_handle->sink_info.bt_out.sample_rate, sample_rate);
                AUDIO_ASSERT(0);
            }
            /* update frame sample format */
            if (dongle_handle->sink_info.bt_out.sample_format == 0)
            {
                dongle_handle->sink_info.bt_out.sample_format = sample_format;
            }
            else if (dongle_handle->sink_info.bt_out.sample_format != sample_format)
            {
                DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR]sample format is not right, %u, %u\r\n", 2, dongle_handle->sink_info.bt_out.sample_format, sample_format);
                AUDIO_ASSERT(0);
            }
            /* update frame samples */
            if (dongle_handle->sink_info.bt_out.frame_samples == 0)
            {
                dongle_handle->sink_info.bt_out.frame_samples = frame_samples;
            }
            else if (dongle_handle->sink_info.bt_out.frame_samples != frame_samples)
            {
                DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR]sample is not right, %u, %u\r\n", 2, dongle_handle->sink_info.bt_out.frame_samples, frame_samples);
                AUDIO_ASSERT(0);
            }
            /* update frame size */
            if (dongle_handle->sink_info.bt_out.frame_size == 0)
            {
                dongle_handle->sink_info.bt_out.frame_size = frame_size;
            }
            else if (dongle_handle->sink_info.bt_out.frame_size != frame_size)
            {
                DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR]frame size is not right, %u, %u\r\n", 2, dongle_handle->sink_info.bt_out.frame_size, frame_size);
                AUDIO_ASSERT(0);
            }
            /* update codec bit rate */
            if (dongle_handle->sink_info.bt_out.bit_rate == 0)
            {
                dongle_handle->sink_info.bt_out.bit_rate = bit_rate;
            }
            else if (dongle_handle->sink_info.bt_out.bit_rate != bit_rate)
            {
                DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR]bit rate is not right, %u, %u\r\n", 2, dongle_handle->sink_info.bt_out.bit_rate, bit_rate);
                AUDIO_ASSERT(0);
            }
        }
    }
    dongle_handle->sink_info.bt_out.channel_num = channel_num;
}

static void ull_audio_v2_dongle_dl_bt_out_deinit(ull_audio_v2_dongle_dl_handle_t *dongle_handle)
{
    uint32_t i;
    uint32_t saved_mask;

    for (i = 0; i < ULL_AUDIO_V2_DATA_CHANNEL_NUMBER; i++)
    {
        if (dongle_handle->sink_info.bt_out.bt_info[i] == NULL)
        {
            continue;
        }
        else
        {
            if ((dongle_handle->sink_info.bt_out.bt_info[i])->user_count == 0)
            {
                DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR]user count is error, %u\r\n", 1, (dongle_handle->sink_info.bt_out.bt_info[i])->user_count);
                AUDIO_ASSERT(0);
            }
            hal_nvic_save_and_set_interrupt_mask(&saved_mask);
            (dongle_handle->sink_info.bt_out.bt_info[i])->user_count -= 1;
            if ((dongle_handle->sink_info.bt_out.bt_info[i])->user_count == 0)
            {
                memset(dongle_handle->sink_info.bt_out.bt_info[i], 0, sizeof(ull_audio_v2_dongle_bt_info_t));
            }
            dongle_handle->sink_info.bt_out.bt_info[i] = NULL;
            hal_nvic_restore_interrupt_mask(saved_mask);
        }
    }
}

static void ull_audio_v2_dongle_dl_usb_in_init(ull_audio_v2_dongle_dl_handle_t *dongle_handle, audio_transmitter_open_param_p audio_transmitter_open_param, bt_common_open_param_p bt_common_open_param)
{
    uint32_t i, j, k;
    ull_audio_v2_dongle_dl_handle_t *c_handle = NULL;
    uint32_t saved_mask;
    n9_dsp_share_info_ptr p_share_info;

    /* bt out info init */
    ull_audio_v2_dongle_dl_bt_out_init(dongle_handle, audio_transmitter_open_param, bt_common_open_param);
    DSP_MW_LOG_I("[ULL Audio V2][DL][scenario %d-%d][handle 0x%x]bt out info, %u, %u, %u, %u, %u, %u, %u\r\n", 10,
                audio_transmitter_open_param->scenario_type,
                audio_transmitter_open_param->scenario_sub_id,
                dongle_handle,
                dongle_handle->sink_info.bt_out.channel_num,
                dongle_handle->sink_info.bt_out.sample_rate,
                dongle_handle->sink_info.bt_out.sample_format,
                dongle_handle->sink_info.bt_out.frame_size,
                dongle_handle->sink_info.bt_out.frame_samples,
                dongle_handle->sink_info.bt_out.frame_interval,
                dongle_handle->sink_info.bt_out.bit_rate);
    for (i = 0; i < ULL_AUDIO_V2_DATA_CHANNEL_NUMBER; i++)
    {
        if (dongle_handle->sink_info.bt_out.bt_info[i] != NULL)
        {
            p_share_info = (n9_dsp_share_info_ptr)((dongle_handle->sink_info.bt_out.bt_info[i])->bt_link_info.share_info);
            DSP_MW_LOG_I("[ULL Audio V2][DL][scenario %d-%d][handle 0x%x]bt out channel %u info, %u, %u, %u, %u, 0x%x, 0x%x, 0x%x\r\n", 11,
                        audio_transmitter_open_param->scenario_type,
                        audio_transmitter_open_param->scenario_sub_id,
                        dongle_handle,
                        i+1,
                        (dongle_handle->sink_info.bt_out.bt_info[i])->seq_num,
                        (dongle_handle->sink_info.bt_out.bt_info[i])->user_count,
                        (dongle_handle->sink_info.bt_out.bt_info[i])->blk_index,
                        (dongle_handle->sink_info.bt_out.bt_info[i])->blk_index_previous,
                        (dongle_handle->sink_info.bt_out.bt_info[i])->crc32_init,
                        p_share_info,
                        hal_memview_infrasys_to_dsp0(p_share_info->start_addr));
        }
    }

    /* usb in info init */
    dongle_handle->source_info.usb_in.channel_num       = audio_transmitter_open_param->scenario_param.ull_audio_v2_dongle_param.codec_param.pcm.channel_mode;
    dongle_handle->source_info.usb_in.sample_rate       = audio_transmitter_open_param->scenario_param.ull_audio_v2_dongle_param.codec_param.pcm.sample_rate;
    dongle_handle->source_info.usb_in.sample_format     = audio_transmitter_open_param->scenario_param.ull_audio_v2_dongle_param.codec_param.pcm.format;
    dongle_handle->source_info.usb_in.frame_size        = usb_audio_get_frame_size(&audio_transmitter_open_param->scenario_param.ull_audio_v2_dongle_param.codec_type, &audio_transmitter_open_param->scenario_param.ull_audio_v2_dongle_param.codec_param);
    dongle_handle->source_info.usb_in.frame_samples     = audio_transmitter_open_param->scenario_param.ull_audio_v2_dongle_param.codec_param.pcm.sample_rate/audio_transmitter_open_param->scenario_param.ull_audio_v2_dongle_param.codec_param.pcm.frame_interval;
    dongle_handle->source_info.usb_in.frame_interval    = audio_transmitter_open_param->scenario_param.ull_audio_v2_dongle_param.codec_param.pcm.frame_interval;
    dongle_handle->source_info.usb_in.frame_max_num     = (dongle_handle->sink_info.bt_out.frame_interval+500) / 1000 + 1;
    DSP_MW_LOG_I("[ULL Audio V2][DL][scenario %d-%d][handle 0x%x]usb in info, %u, %u, %u, %u, %u, %u, %u\r\n", 10,
                audio_transmitter_open_param->scenario_type,
                audio_transmitter_open_param->scenario_sub_id,
                dongle_handle,
                dongle_handle->source_info.usb_in.channel_num,
                dongle_handle->source_info.usb_in.sample_rate,
                dongle_handle->source_info.usb_in.sample_format,
                dongle_handle->source_info.usb_in.frame_size,
                dongle_handle->source_info.usb_in.frame_samples,
                dongle_handle->source_info.usb_in.frame_interval,
                dongle_handle->source_info.usb_in.frame_max_num);

    /* sw gain init */
    int32_t default_gain;
    sw_gain_config_t default_config;
    default_config.resolution               = RESOLUTION_32BIT;
    default_config.target_gain              = audio_transmitter_open_param->scenario_param.ull_audio_v2_dongle_param.gain_default_L;
    default_config.up_step                  = 1;
    default_config.up_samples_per_step      = 2;
    default_config.down_step                = -1;
    default_config.down_samples_per_step    = 2;
    dongle_handle->gain_port                = stream_function_sw_gain_get_port(dongle_handle->source);
    stream_function_sw_gain_init(dongle_handle->gain_port, 2, &default_config);
    default_gain = audio_transmitter_open_param->scenario_param.ull_audio_v2_dongle_param.gain_default_L;
    stream_function_sw_gain_configure_gain_target(dongle_handle->gain_port, 1, default_gain);
    default_gain = audio_transmitter_open_param->scenario_param.ull_audio_v2_dongle_param.gain_default_R;
    stream_function_sw_gain_configure_gain_target(dongle_handle->gain_port, 2, default_gain);
    DSP_MW_LOG_I("[ULL Audio V2][DL][scenario %d-%d][handle 0x%x]sw gain 0x%x info, %d, %d, %d, %d, %d, %d, %d, %d\r\n", 12,
                audio_transmitter_open_param->scenario_type,
                audio_transmitter_open_param->scenario_sub_id,
                dongle_handle,
                dongle_handle->gain_port,
                default_config.resolution,
                default_config.target_gain,
                default_config.up_step,
                default_config.up_samples_per_step,
                default_config.down_step,
                default_config.down_samples_per_step,
                audio_transmitter_open_param->scenario_param.ull_audio_v2_dongle_param.gain_default_L,
                audio_transmitter_open_param->scenario_param.ull_audio_v2_dongle_param.gain_default_R);

    /* sw buffer init */
    sw_buffer_config_t buffer_config;
    buffer_config.mode                  = SW_BUFFER_MODE_FIXED_OUTPUT_LENGTH;
    buffer_config.total_channels        = 2;
    buffer_config.watermark_max_size    = 8*dongle_handle->source_info.usb_in.frame_samples*dongle_handle->sink_info.bt_out.frame_interval/dongle_handle->source_info.usb_in.frame_interval*sizeof(int32_t);
    buffer_config.watermark_min_size    = 0;
    buffer_config.output_size           = dongle_handle->source_info.usb_in.frame_samples*dongle_handle->sink_info.bt_out.frame_interval/dongle_handle->source_info.usb_in.frame_interval*sizeof(int32_t);
    dongle_handle->buffer_port_0        = stream_function_sw_buffer_get_unused_port(dongle_handle->source);
    stream_function_sw_buffer_init(dongle_handle->buffer_port_0, &buffer_config);
    dongle_handle->buffer_default_output_size = dongle_handle->source_info.usb_in.frame_samples*dongle_handle->sink_info.bt_out.frame_interval/dongle_handle->source_info.usb_in.frame_interval*sizeof(int32_t);
    dongle_handle->buffer0_output_size = dongle_handle->buffer_default_output_size;
    DSP_MW_LOG_I("[ULL Audio V2][DL][scenario %d-%d][handle 0x%x]sw buffer 0x%x info, %d, %d, %d, %d, %d, %d, %d\r\n", 11,
                audio_transmitter_open_param->scenario_type,
                audio_transmitter_open_param->scenario_sub_id,
                dongle_handle,
                dongle_handle->buffer_port_0,
                buffer_config.mode,
                buffer_config.total_channels,
                buffer_config.watermark_max_size,
                buffer_config.watermark_min_size,
                buffer_config.output_size,
                dongle_handle->buffer_default_output_size,
                dongle_handle->buffer0_output_size);

    /* sw clk skew init */
    sw_clk_skew_config_t sw_clk_skew_config;
    dongle_handle->clk_skew_port                = stream_function_sw_clk_skew_get_port(dongle_handle->source);
    sw_clk_skew_config.channel                  = 2;
    sw_clk_skew_config.bits                     = 32;
    sw_clk_skew_config.order                    = C_Flp_Ord_1;
    sw_clk_skew_config.skew_io_mode             = C_Skew_Inp;
    sw_clk_skew_config.skew_compensation_mode   = SW_CLK_SKEW_COMPENSATION_1_SAMPLE_IN_8_FRAME;
    sw_clk_skew_config.skew_work_mode           = SW_CLK_SKEW_CONTINUOUS;
    sw_clk_skew_config.max_output_size          = (dongle_handle->source_info.usb_in.frame_samples*dongle_handle->sink_info.bt_out.frame_interval/dongle_handle->source_info.usb_in.frame_interval+1)*sizeof(int32_t);
    sw_clk_skew_config.continuous_frame_size    = dongle_handle->source_info.usb_in.frame_samples*dongle_handle->sink_info.bt_out.frame_interval/dongle_handle->source_info.usb_in.frame_interval*sizeof(int32_t);
    stream_function_sw_clk_skew_init(dongle_handle->clk_skew_port, &sw_clk_skew_config);
    dongle_handle->compen_method                = ULL_AUDIO_V2_COMPENSATORY_METHOD_SW_CLK_SKEW;
    dongle_handle->clk_skew_watermark_samples   = dongle_handle->source_info.usb_in.frame_samples*1000/dongle_handle->source_info.usb_in.frame_interval;
    dongle_handle->clk_skew_compensation_mode   = sw_clk_skew_config.skew_compensation_mode;
    DSP_MW_LOG_I("[ULL Audio V2][DL][scenario %d-%d][handle 0x%x]sw clk skew 0x%x info, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\r\n", 14,
                audio_transmitter_open_param->scenario_type,
                audio_transmitter_open_param->scenario_sub_id,
                dongle_handle,
                dongle_handle->clk_skew_port,
                sw_clk_skew_config.channel,
                sw_clk_skew_config.bits,
                sw_clk_skew_config.order,
                sw_clk_skew_config.skew_io_mode,
                sw_clk_skew_config.skew_compensation_mode,
                sw_clk_skew_config.skew_work_mode,
                sw_clk_skew_config.max_output_size,
                sw_clk_skew_config.continuous_frame_size,
                dongle_handle->compen_method,
                dongle_handle->clk_skew_watermark_samples);

    /* sw src init */
    src_fixed_ratio_config_t sw_src_config;
    dongle_handle->src_in_frame_samples     = dongle_handle->source_info.usb_in.frame_samples*dongle_handle->sink_info.bt_out.frame_interval/dongle_handle->source_info.usb_in.frame_interval;
    dongle_handle->src_in_frame_size        = dongle_handle->src_in_frame_size * sizeof(int32_t);
    dongle_handle->src_out_frame_samples    = dongle_handle->sink_info.bt_out.sample_rate/1000*dongle_handle->sink_info.bt_out.frame_interval/1000;
    dongle_handle->src_out_frame_size       = dongle_handle->src_out_frame_samples * sizeof(int32_t);
    sw_src_config.channel_number            = 2;
    sw_src_config.in_sampling_rate          = dongle_handle->source_info.usb_in.sample_rate;
    sw_src_config.out_sampling_rate         = dongle_handle->sink_info.bt_out.sample_rate;
    sw_src_config.resolution                = RESOLUTION_32BIT;
    sw_src_config.multi_cvt_mode            = SRC_FIXED_RATIO_PORT_MUTI_CVT_MODE_SINGLE;
    sw_src_config.cvt_num                   = 1;
    sw_src_config.with_codec                = false;
    dongle_handle->src_port                 = stream_function_src_fixed_ratio_get_port(dongle_handle->source);
    stream_function_src_fixed_ratio_init(dongle_handle->src_port, &sw_src_config);
    DSP_MW_LOG_I("[ULL Audio V2][DL][scenario %d-%d][handle 0x%x]sw src 0x%x info, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\r\n", 15,
                audio_transmitter_open_param->scenario_type,
                audio_transmitter_open_param->scenario_sub_id,
                dongle_handle,
                dongle_handle->src_port,
                sw_src_config.multi_cvt_mode,
                sw_src_config.cvt_num,
                sw_src_config.with_codec,
                sw_src_config.channel_number,
                sw_src_config.resolution,
                sw_src_config.in_sampling_rate,
                sw_src_config.out_sampling_rate,
                dongle_handle->src_in_frame_samples,
                dongle_handle->src_in_frame_size,
                dongle_handle->src_out_frame_samples,
                dongle_handle->src_out_frame_size);

    /* sw mixer init */
    sw_mixer_callback_config_t       callback_config;
    sw_mixer_input_channel_config_t  in_ch_config;
    sw_mixer_output_channel_config_t out_ch_config;
    stream_function_sw_mixer_init(SW_MIXER_PORT_0);
    callback_config.preprocess_callback     = NULL;
    callback_config.postprocess_callback    = ull_audio_v2_dongle_dl_mixer_postcallback;
    in_ch_config.total_channel_number       = 2;
    in_ch_config.resolution                 = RESOLUTION_32BIT;
    in_ch_config.input_mode                 = SW_MIXER_CHANNEL_MODE_FORCED_ENOUGH;
    in_ch_config.buffer_size                = dongle_handle->sink_info.bt_out.sample_rate/1000*dongle_handle->sink_info.bt_out.frame_interval/1000*sizeof(int32_t);
    out_ch_config.total_channel_number      = 2;
    out_ch_config.resolution                = RESOLUTION_32BIT;
    dongle_handle->mixer_member             = stream_function_sw_mixer_member_create((void *)dongle_handle->source, SW_MIXER_MEMBER_MODE_NO_BYPASS, &callback_config, &in_ch_config, &out_ch_config);
    if (dongle_handle->index == 1)
    {
        /* it is the first dl stream */
        stream_function_sw_mixer_member_register(SW_MIXER_PORT_0, dongle_handle->mixer_member, true);
        /* update mixer status */
        hal_nvic_save_and_set_interrupt_mask(&saved_mask);
        dongle_handle->mixer_status = ULL_AUDIO_V2_DONGLE_MIXER_UNMIX;
        hal_nvic_restore_interrupt_mask(saved_mask);
    }
    else
    {
        /* it is not the first dl stream, needs to do mixer connection based on BT link info */
        stream_function_sw_mixer_member_register(SW_MIXER_PORT_0, dongle_handle->mixer_member, true);
        c_handle = ull_audio_v2_dongle_first_dl_handle;
        for (i=0; i < (uint32_t)ull_audio_v2_dongle_first_dl_handle->total_number; i++)
        {
            if (c_handle != dongle_handle)
            {
                for (j = 0; j < ULL_AUDIO_V2_DATA_CHANNEL_NUMBER; j++)
                {
                    if (dongle_handle->sink_info.bt_out.bt_info[j] != NULL)
                    {
                        for (k = 0; k < ULL_AUDIO_V2_DATA_CHANNEL_NUMBER; k++)
                        {
                            if (c_handle->sink_info.bt_out.bt_info[k] != NULL)
                            {
                                if (dongle_handle->sink_info.bt_out.bt_info[j] == c_handle->sink_info.bt_out.bt_info[k])
                                {
                                    /* in here, it means the currnet dongle_handle ch_k should be mixed with the new dongle_handle ch_j */
                                    stream_function_sw_mixer_channel_connect(c_handle->mixer_member, k+1, SW_MIXER_CHANNEL_ATTRIBUTE_NORMAL, dongle_handle->mixer_member, j+1);
                                    stream_function_sw_mixer_channel_connect(dongle_handle->mixer_member, j+1, SW_MIXER_CHANNEL_ATTRIBUTE_NORMAL, c_handle->mixer_member, k+1);
                                    /* update mixer status */
                                    hal_nvic_save_and_set_interrupt_mask(&saved_mask);
                                    c_handle->mixer_status      = ULL_AUDIO_V2_DONGLE_MIXER_MIX;
                                    dongle_handle->mixer_status = ULL_AUDIO_V2_DONGLE_MIXER_MIX;
                                    hal_nvic_restore_interrupt_mask(saved_mask);
                                    DSP_MW_LOG_I("[ULL Audio V2][DL][scenario %d-%d][handle 0x%x]sw mixer member 0x%x ch%d connect to [handle 0x%x]sw mixer member 0x%x ch%d\r\n", 8,
                                                audio_transmitter_open_param->scenario_type,
                                                audio_transmitter_open_param->scenario_sub_id,
                                                dongle_handle,
                                                dongle_handle->mixer_member,
                                                j+1,
                                                c_handle,
                                                c_handle->mixer_member,
                                                k+1);
                                }
                            }
                        }
                    }
                }
            }
            /* switch to the next dl stream */
            c_handle = c_handle->next_dl_handle;
        }
    }
    DSP_MW_LOG_I("[ULL Audio V2][DL][scenario %d-%d][handle 0x%x]sw mixer 0x%x info, %d, %d, %d, %d, %d, %d, %d\r\n", 11,
                audio_transmitter_open_param->scenario_type,
                audio_transmitter_open_param->scenario_sub_id,
                dongle_handle,
                dongle_handle->mixer_member,
                in_ch_config.total_channel_number,
                in_ch_config.resolution,
                in_ch_config.input_mode,
                in_ch_config.buffer_size,
                out_ch_config.total_channel_number,
                out_ch_config.resolution,
                dongle_handle->mixer_status);

    /* codec init */
    if (dongle_handle->sink_info.bt_out.codec_type == AUDIO_DSP_CODEC_TYPE_LC3PLUS)
    {
        lc3plus_enc_config_t lc3plus_enc_config;
        lc3plus_enc_config.sample_bits      = 24;
        lc3plus_enc_config.channel_num      = dongle_handle->sink_info.bt_out.channel_num;
        lc3plus_enc_config.sample_rate      = dongle_handle->sink_info.bt_out.sample_rate;
        lc3plus_enc_config.bit_rate         = dongle_handle->sink_info.bt_out.bit_rate;
        lc3plus_enc_config.frame_interval   = dongle_handle->sink_info.bt_out.frame_interval;
        lc3plus_enc_config.frame_samples    = dongle_handle->sink_info.bt_out.frame_samples;
        lc3plus_enc_config.in_frame_size    = dongle_handle->sink_info.bt_out.frame_samples*sizeof(int32_t);
        lc3plus_enc_config.out_frame_size   = dongle_handle->sink_info.bt_out.frame_size;
        lc3plus_enc_config.process_frame_num   = 1;
        lc3plus_enc_config.codec_mode       = LC3PLUS_ARCH_FX;
        stream_codec_encoder_lc3plus_init(LC3PLUS_ENC_PORT_0, dongle_handle->sink, &lc3plus_enc_config);
        DSP_MW_LOG_I("[ULL Audio V2][DL][scenario %d-%d][handle 0x%x]lc3plus info, %d, %d, %d, %d, %d, %d, %d, %d, %d\r\n", 12,
                    audio_transmitter_open_param->scenario_type,
                    audio_transmitter_open_param->scenario_sub_id,
                    dongle_handle,
                    lc3plus_enc_config.sample_bits,
                    lc3plus_enc_config.channel_num,
                    lc3plus_enc_config.sample_rate,
                    lc3plus_enc_config.bit_rate,
                    lc3plus_enc_config.frame_interval,
                    lc3plus_enc_config.frame_samples,
                    lc3plus_enc_config.in_frame_size,
                    lc3plus_enc_config.out_frame_size,
                    lc3plus_enc_config.codec_mode);
    }
}

static void ull_audio_v2_dongle_dl_usb_in_deinit(ull_audio_v2_dongle_dl_handle_t *dongle_handle)
{
    uint32_t i;
    uint32_t mixer_count = 0;
    ull_audio_v2_dongle_dl_handle_t *c_handle;

    /* bt out info deinit */
    ull_audio_v2_dongle_dl_bt_out_deinit(dongle_handle);

    /* sw gain deinit */
    stream_function_sw_gain_deinit(dongle_handle->gain_port);

    /* sw buffer deinit */
    stream_function_sw_buffer_deinit(dongle_handle->buffer_port_0);

    /* sw clk skew deinit */
    stream_function_sw_clk_skew_deinit(dongle_handle->clk_skew_port);

    /* sw src deinit */
    stream_function_src_fixed_ratio_deinit(dongle_handle->src_port);

    /* sw mixer deinit */
    stream_function_sw_mixer_member_unregister(SW_MIXER_PORT_0, dongle_handle->mixer_member);
    stream_function_sw_mixer_member_delete(dongle_handle->mixer_member);
    stream_function_sw_mixer_deinit(SW_MIXER_PORT_0);
    c_handle = ull_audio_v2_dongle_first_dl_handle;
    for (i=0; i < (uint32_t)ull_audio_v2_dongle_first_dl_handle->total_number; i++)
    {
        if (c_handle->mixer_status == ULL_AUDIO_V2_DONGLE_MIXER_MIX)
        {
            mixer_count += 1;
        }
        /* switch to the next dl stream */
        c_handle = c_handle->next_dl_handle;
    }
    if (mixer_count <= 2)
    {
        c_handle = ull_audio_v2_dongle_first_dl_handle;
        for (i=0; i < (uint32_t)ull_audio_v2_dongle_first_dl_handle->total_number; i++)
        {
            if (c_handle->mixer_status == ULL_AUDIO_V2_DONGLE_MIXER_MIX)
            {
                /* reset mixer status */
                c_handle->mixer_status = ULL_AUDIO_V2_DONGLE_MIXER_UNMIX;
            }
            /* switch to the next dl stream */
            c_handle = c_handle->next_dl_handle;
        }
    }
    dongle_handle->mixer_status = ULL_AUDIO_V2_DONGLE_MIXER_UNMIX;

    /* codec deinit */
    if (dongle_handle->sink_info.bt_out.codec_type == AUDIO_DSP_CODEC_TYPE_LC3PLUS)
    {
        stream_codec_encoder_lc3plus_deinit(LC3PLUS_ENC_PORT_0, dongle_handle->sink);
    }
}

static int32_t ull_audio_v2_dongle_dl_usb_clock_skew_check(ull_audio_v2_dongle_dl_handle_t *dongle_handle, uint32_t input_samples, uint32_t *buffer0_output_size)
{
    int32_t compensatory_samples = 0;
    int32_t remain_samples;
    int32_t remain_samples_0;
    int32_t output_samples = dongle_handle->src_in_frame_samples;
    uint32_t buffer_output_size = dongle_handle->buffer_default_output_size;
    int32_t frac_rpt;

    if (dongle_handle->first_packet_status == ULL_AUDIO_V2_DONGLE_DL_FIRST_PACKET_READY)
    {
        /* get remain samples */
        remain_samples_0 = stream_function_sw_buffer_get_channel_used_size(dongle_handle->buffer_port_0, 1) / sizeof(int32_t);

        if ((dongle_handle->clk_skew_count > -DL_CLOCK_SKEW_CHECK_COUNT) && (dongle_handle->clk_skew_count < DL_CLOCK_SKEW_CHECK_COUNT))
        {
            /* in here, clock skew is not started */
            remain_samples = remain_samples_0+input_samples-output_samples;
            if (remain_samples < 0)
            {
                /* reset state machine */
                compensatory_samples = 0;
                dongle_handle->clk_skew_count = 0;
                DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR][handle 0x%x] samples are not enough, %d, %d, %d\r\n", 4, dongle_handle, remain_samples_0, input_samples, output_samples);
            }
            else if (remain_samples < dongle_handle->clk_skew_watermark_samples)
            {
                /* usb clock is slower than bt clock */
                dongle_handle->clk_skew_count += 1;
                if (dongle_handle->clk_skew_count == DL_CLOCK_SKEW_CHECK_COUNT)
                {
                    /* do nothing */
                }
            }
            else if (remain_samples > dongle_handle->clk_skew_watermark_samples)
            {
                /* usb clock is faster than bt clock */
                dongle_handle->clk_skew_count -= 1;
                if (dongle_handle->clk_skew_count == -DL_CLOCK_SKEW_CHECK_COUNT)
                {
                    /* do nothing */
                }
            }
            else
            {
                /* usb clock is as the same as bt clock */
                if (dongle_handle->clk_skew_count > 0)
                {
                    dongle_handle->clk_skew_count -= 1;
                }
                else if (dongle_handle->clk_skew_count < 0)
                {
                    dongle_handle->clk_skew_count += 1;
                }
            }
            /* do not compensatory */
            compensatory_samples = 0;
        }
        else
        {
            /* in here, clock skew is running */
            remain_samples = remain_samples_0+input_samples-output_samples;
            if (remain_samples < 0)
            {
                /* reset state machine */
                compensatory_samples = 0;
                dongle_handle->clk_skew_count = 0;
                DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR][handle 0x%x] samples are not enough, %d, %d, %d\r\n", 4, dongle_handle, remain_samples_0, input_samples, output_samples);
            }
            else if (dongle_handle->clk_skew_count == DL_CLOCK_SKEW_CHECK_COUNT)
            {
                if (remain_samples >= dongle_handle->clk_skew_watermark_samples)
                {
                    /* watermark is ok and stop compensatory */
                    compensatory_samples = 0;
                    dongle_handle->clk_skew_count = 0;
                }
                else
                {
                    /* do +1 sample compensatory */
                    compensatory_samples = 1;
                    stream_function_sw_clk_skew_get_frac_rpt(dongle_handle->clk_skew_port, 1, &frac_rpt);
                    if (frac_rpt == (dongle_handle->clk_skew_compensation_mode - 1))
                    {
                        buffer_output_size = dongle_handle->buffer_default_output_size - sizeof(int32_t);
                    }
                }
            }
            else if (dongle_handle->clk_skew_count == -DL_CLOCK_SKEW_CHECK_COUNT)
            {
                if (remain_samples <= dongle_handle->clk_skew_watermark_samples)
                {
                    /* watermark is ok and stop compensatory */
                    compensatory_samples = 0;
                    dongle_handle->clk_skew_count = 0;
                }
                else
                {
                    /* do -1 sample compensatory */
                    compensatory_samples = -1;
                    stream_function_sw_clk_skew_get_frac_rpt(dongle_handle->clk_skew_port, 1, &frac_rpt);
                    if (frac_rpt == -(dongle_handle->clk_skew_compensation_mode - 1))
                    {
                        buffer_output_size = dongle_handle->buffer_default_output_size + sizeof(int32_t);
                    }
                }
            }
            else
            {
                DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR][handle 0x%x] error clk skew count, %d\r\n", 2, dongle_handle, dongle_handle->clk_skew_count);
                AUDIO_ASSERT(0);
            }
        }
    }

    *buffer0_output_size = buffer_output_size;

    return compensatory_samples;
}
/******************************************************************************/
/*               ULL audio 2.0 dongle UL path Private Functions               */
/******************************************************************************/
static ull_audio_v2_dongle_ul_handle_t *ull_audio_v2_dongle_ul_get_handle(void *owner)
{
    uint32_t saved_mask;
    int16_t count;
    ull_audio_v2_dongle_ul_handle_t *dongle_handle = NULL;
    ull_audio_v2_dongle_ul_handle_t *c_handle = NULL;

    dongle_handle = malloc(sizeof(ull_audio_v2_dongle_ul_handle_t));
    if (dongle_handle == NULL)
    {
        AUDIO_ASSERT(0);
    }
    memset(dongle_handle, 0, sizeof(ull_audio_v2_dongle_ul_handle_t));

    dongle_handle->total_number = -1;
    dongle_handle->index = -1;
    dongle_handle->owner = owner;
    dongle_handle->next_ul_handle = NULL;

    hal_nvic_save_and_set_interrupt_mask(&saved_mask);

    if (ull_audio_v2_dongle_first_ul_handle == NULL)
    {
        /* there is no item on the handle list */
        dongle_handle->total_number = 1;
        dongle_handle->index = 1;

        /* set this handle as the head of the handle list */
        ull_audio_v2_dongle_first_ul_handle = dongle_handle;
    }
    else
    {
        /* there are other items on the handle list */
        count = 1;
        c_handle = ull_audio_v2_dongle_first_ul_handle;
        while (c_handle != NULL)
        {
            count += 1;

            c_handle->total_number += 1;
            if (c_handle->total_number <= 0)
            {
                /* error status */
                AUDIO_ASSERT(0);
            }

            if (c_handle->next_ul_handle == NULL)
            {
                /* c_handle is the last item on the list now */
                dongle_handle->total_number = c_handle->total_number;
                dongle_handle->index = c_handle->total_number;

                /* dongle_handle is the last item on the list now */
                c_handle->next_ul_handle = dongle_handle;

                break;
            }

            c_handle = c_handle->next_ul_handle;
        }
        if ((c_handle == NULL) || (dongle_handle->total_number != count))
        {
            AUDIO_ASSERT(0);
        }
    }

    hal_nvic_restore_interrupt_mask(saved_mask);

    return dongle_handle;
}

static void ull_audio_v2_dongle_ul_release_handle(ull_audio_v2_dongle_ul_handle_t *handle)
{
    uint32_t saved_mask, i, count;
    ull_audio_v2_dongle_ul_handle_t *l_handle = NULL;
    ull_audio_v2_dongle_ul_handle_t *c_handle = NULL;
    ull_audio_v2_dongle_ul_handle_t *dongle_handle = NULL;

    if ((handle == NULL) || (ull_audio_v2_dongle_first_ul_handle == NULL))
    {
        AUDIO_ASSERT(0);
    }

    hal_nvic_save_and_set_interrupt_mask(&saved_mask);

    if (ull_audio_v2_dongle_first_ul_handle->total_number <= 0)
    {
        /* error status */
        AUDIO_ASSERT(0);
    }
    else if ((ull_audio_v2_dongle_first_ul_handle->total_number == 1) &&
            (ull_audio_v2_dongle_first_ul_handle == handle))
    {
        /* this handle is only one item on handle list */
        if (ull_audio_v2_dongle_first_ul_handle->next_ul_handle != NULL)
        {
            AUDIO_ASSERT(0);
        }

        /* there is no item on the handle list */
        ull_audio_v2_dongle_first_ul_handle = NULL;
    }
    else if ((ull_audio_v2_dongle_first_ul_handle->total_number > 1) &&
            (ull_audio_v2_dongle_first_ul_handle == handle))
    {
        /* this handle is the first item on handle list, but there are other handles on the list */
        c_handle = ull_audio_v2_dongle_first_ul_handle;
        count = ull_audio_v2_dongle_first_ul_handle->total_number;
        for (i=0; i < count; i++)
        {
            c_handle->total_number -= 1;
            c_handle->index -= 1;

            c_handle = c_handle->next_ul_handle;
        }
        if (c_handle != NULL)
        {
            /* error status */
            AUDIO_ASSERT(0);
        }

        /* change the next iten to the head */
        ull_audio_v2_dongle_first_ul_handle = handle->next_ul_handle;
    }
    else
    {
        /* this handle is the middle item on handle list */
        c_handle = ull_audio_v2_dongle_first_ul_handle;
        count = ull_audio_v2_dongle_first_ul_handle->total_number;
        if (count == 1)
        {
            /* error status */
            AUDIO_ASSERT(0);
        }
        for (i=0; i < count; i++)
        {
            if (c_handle == handle)
            {
                /* find out the handle on the list */
                dongle_handle = handle;
                l_handle->next_ul_handle = c_handle->next_ul_handle;
            }

            if (dongle_handle == NULL)
            {
                c_handle->total_number -= 1;
            }
            else
            {
                c_handle->total_number -= 1;
                c_handle->index -= 1;
            }

            l_handle = c_handle;
            c_handle = c_handle->next_ul_handle;
        }
        if ((c_handle != NULL) || (dongle_handle == NULL))
        {
            /* error status */
            AUDIO_ASSERT(0);
        }
    }

    hal_nvic_restore_interrupt_mask(saved_mask);

    free(handle);
}

ATTR_TEXT_IN_IRAM static bool ull_audio_v2_dongle_ul_play_time_is_arrived(ull_audio_v2_dongle_ul_handle_t *dongle_handle, uint32_t bt_clk)
{
    bool ret = false;

    if ((dongle_handle->first_packet_status == ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_PLAYED) || (dongle_handle->first_packet_status == ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_READY))
    {
        if (dongle_handle->source_info.bt_in.first_anchor_bt_clk < dongle_handle->source_info.bt_in.play_en_bt_clk)
        {
            if (bt_clk >= dongle_handle->source_info.bt_in.play_en_bt_clk)
            {
                /* --------- ........ --------- anchor ---------- bt_clk ---------- playen --------- ........ -------- */
                ret = true;
            }
            else if ((bt_clk < dongle_handle->source_info.bt_in.play_en_bt_clk) && (bt_clk < dongle_handle->source_info.bt_in.first_anchor_bt_clk))
            {
                /* ---------- bt_clk --------- ........ --------- anchor  --------- ........ ---------- playen -------- */
                ret = true;
            }
        }
        else
        {
            if ((bt_clk >= dongle_handle->source_info.bt_in.play_en_bt_clk) && (bt_clk < dongle_handle->source_info.bt_in.first_anchor_bt_clk))
            {
                /* ---------- playen ---------- bt_clk --------- ........ --------- anchor  --------- ........  -------- */
                ret = true;
            }
        }
    }

    return ret;
}

ATTR_TEXT_IN_IRAM static void ull_audio_v2_dongle_ul_ccni_handler(hal_ccni_event_t event, void * msg)
{
    SOURCE source;
    hal_ccni_message_t *ccni_msg = msg;
    ull_audio_v2_dongle_ul_handle_t *c_handle;
    uint32_t saved_mask;
    uint32_t gpt_count;
    uint32_t mcu_gpt_count;
    uint32_t bt_clk;
    uint16_t bt_phase;
    uint32_t write_offset;
    uint32_t i;
    UNUSED(event);

    /* check if there is any UL stream */
    if (ull_audio_v2_dongle_first_ul_handle == NULL)
    {
        DSP_MW_LOG_W("[ULL Audio V2][UL][WARNNING]ul dongle_handle is NULL\r\n", 0);
        goto _ccni_return;
    }

    hal_nvic_save_and_set_interrupt_mask(&saved_mask);
    /* get timestamp for debug */
    mcu_gpt_count = ccni_msg->ccni_message[0];
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &gpt_count);
    hal_nvic_restore_interrupt_mask(saved_mask);

    /* trigger all started UL stream one by one */
    c_handle = ull_audio_v2_dongle_first_ul_handle;
    for (i = 0; i < (uint32_t)ull_audio_v2_dongle_first_ul_handle->total_number; i++)
    {
        if ((c_handle->stream_status == ULL_AUDIO_V2_DONGLE_STREAM_START) || (c_handle->stream_status == ULL_AUDIO_V2_DONGLE_STREAM_RUNNING))
        {
            switch (c_handle->sub_id)
            {
                case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0:
                    /* get source */
                    source = (SOURCE)c_handle->owner;
                    if ((source == NULL) || (source->transform == NULL))
                    {
                        break;
                    }
                    /* set timestamp for debug */
                    c_handle->ccni_in_bt_count  = mcu_gpt_count;
                    c_handle->ccni_in_gpt_count = gpt_count;
                    hal_nvic_save_and_set_interrupt_mask(&saved_mask);
                    /* increase fetch count */
                    if (c_handle->first_packet_status == ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_NOT_READY)
                    {
                        /* trigger stream to find out the play time */
                        c_handle->fetch_count = 1;
                    }
                    else if (c_handle->first_packet_status == ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_READY)
                    {
                        /* check if the current bt clock is play time */
                        MCE_GetBtClk((BTCLK*)&bt_clk, (BTPHASE*)&bt_phase, BT_CLK_Offset);
                        if (ull_audio_v2_dongle_ul_play_time_is_arrived(c_handle, bt_clk))
                        {
                            c_handle->first_packet_status = ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_PLAYED;
                            /* update share buffer write offset here for more accurate latency */
                            audio_transmitter_share_information_fetch(NULL, source->transform->sink);
                            write_offset = (source->transform->sink->streamBuffer.ShareBufferInfo.write_offset + source->transform->sink->streamBuffer.ShareBufferInfo.sub_info.block_info.block_size * c_handle->process_frames) % (source->transform->sink->streamBuffer.ShareBufferInfo.sub_info.block_info.block_size * source->transform->sink->streamBuffer.ShareBufferInfo.sub_info.block_info.block_num);
                            audio_transmitter_share_information_update_write_offset(source->transform->sink, write_offset);
                            c_handle->process_frames = 0;
                            c_handle->fetch_count = 0;
                            DSP_MW_LOG_I("[ULL Audio V2][UL][handle 0x%x]stream is played, %d, 0x%x, 0x%x\r\n", 4, c_handle, c_handle->process_frames, bt_clk, write_offset);
                        }
                    }
                    else if (c_handle->first_packet_status == ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_PLAYED)
                    {
                        /* trigger stream do something */
                        c_handle->fetch_count = 1;
                    }
                    else
                    {
                        c_handle->fetch_count = 0;
                    }
                    hal_nvic_restore_interrupt_mask(saved_mask);
                    /* Handler the stream */
                    AudioCheckTransformHandle(source->transform);
                    break;

                default:
                    AUDIO_ASSERT(0);
                    break;
            }
        }

        /* switch to the next dl stream */
        c_handle = c_handle->next_ul_handle;
    }
    if (c_handle != NULL)
    {
        AUDIO_ASSERT(0);
    }

_ccni_return:
    return;
}

static void ull_audio_v2_dongle_ul_bt_in_init(ull_audio_v2_dongle_ul_handle_t *dongle_handle, audio_transmitter_open_param_p audio_transmitter_open_param, bt_common_open_param_p bt_common_open_param)
{
    uint32_t i, j;
    uint32_t frame_interval;
    uint32_t sample_rate;
    hal_audio_format_t sample_format;
    uint32_t frame_samples;
    uint32_t frame_size;
    uint32_t saved_mask;
    uint32_t bit_rate;
    uint32_t channel_num = 0;
    audio_dsp_codec_type_t codec_type;
    UNUSED(audio_transmitter_open_param);

    for (i = 0; i < ULL_AUDIO_V2_DATA_CHANNEL_NUMBER; i++)
    {
        if (bt_common_open_param->scenario_param.ull_audio_v2_dongle_param.bt_link_info[i].share_info == NULL)
        {
            continue;
        }
        else
        {
            /* update channel num */
            channel_num += 1;
            /* update bt link settings to global state machine */
            for (j = 0; j < ULL_AUDIO_V2_DATA_CHANNEL_NUMBER; j++)
            {
                if (hal_memview_cm4_to_dsp0((uint32_t)(bt_common_open_param->scenario_param.ull_audio_v2_dongle_param.bt_link_info[i].share_info)) == (uint32_t)(ull_audio_v2_ul_bt_info[j].bt_link_info.share_info))
                {
                    /* check bt link settings */
                    if (memcmp(&(bt_common_open_param->scenario_param.ull_audio_v2_dongle_param.bt_link_info[i].codec_type), &(ull_audio_v2_ul_bt_info[j].bt_link_info.codec_type), sizeof(ull_audio_v2_dongle_bt_link_info_t)-sizeof(void *)) != 0)
                    {
                        /* same share buffer, same codec settings */
                        DSP_MW_LOG_E("[ULL Audio V2][UL][ERROR]codec setting is different\r\n", 0);
                        AUDIO_ASSERT(0);
                    }
                    /* in here, it means this bt link's setting has been used */
                    hal_nvic_save_and_set_interrupt_mask(&saved_mask);
                    ull_audio_v2_ul_bt_info[j].user_count += 1;
                    hal_nvic_restore_interrupt_mask(saved_mask);
                    dongle_handle->source_info.bt_in.bt_info[i] = &ull_audio_v2_ul_bt_info[j];
                }
            }
            if (dongle_handle->source_info.bt_in.bt_info[i] == NULL)
            {
                /* in here, it means this bt link's setting has not been used */
                for (j = 0; j < ULL_AUDIO_V2_DATA_CHANNEL_NUMBER; j++)
                {
                    hal_nvic_save_and_set_interrupt_mask(&saved_mask);
                    if (ull_audio_v2_ul_bt_info[j].bt_link_info.share_info == NULL)
                    {
                        if (ull_audio_v2_ul_bt_info[j].user_count != 0)
                        {
                            DSP_MW_LOG_E("[ULL Audio V2][UL][ERROR]user count is not right, %u\r\n", 1, ull_audio_v2_ul_bt_info[j].user_count);
                            AUDIO_ASSERT(0);
                        }
                        ull_audio_v2_ul_bt_info[j].user_count += 1;
                        ull_audio_v2_ul_bt_info[j].seq_num = 0;
                        ull_audio_v2_ul_bt_info[j].blk_index = 0;
                        ull_audio_v2_ul_bt_info[j].blk_index_previous = 0;
                        memcpy(&(ull_audio_v2_ul_bt_info[j].bt_link_info), &(bt_common_open_param->scenario_param.ull_audio_v2_dongle_param.bt_link_info[i]), sizeof(ull_audio_v2_dongle_bt_link_info_t));
                        ull_audio_v2_ul_bt_info[j].bt_link_info.share_info = (void *)hal_memview_cm4_to_dsp0((uint32_t)(ull_audio_v2_ul_bt_info[j].bt_link_info.share_info));
                        dongle_handle->source_info.bt_in.bt_info[i] = &ull_audio_v2_ul_bt_info[j];
                        hal_nvic_restore_interrupt_mask(saved_mask);
                        break;
                    }
                    hal_nvic_restore_interrupt_mask(saved_mask);
                }
                if (dongle_handle->source_info.bt_in.bt_info[i] == NULL)
                {
                    /* not found suitable bt info channel */
                    DSP_MW_LOG_E("[ULL Audio V2][UL][ERROR]all bt info are used\r\n", 0);
                    AUDIO_ASSERT(0);
                }
            }
            if ((dongle_handle->source_info.bt_in.bt_info[i])->bt_link_info.codec_type == AUDIO_DSP_CODEC_TYPE_LC3PLUS)
            {
                codec_type     = AUDIO_DSP_CODEC_TYPE_LC3PLUS;
                bit_rate       = (dongle_handle->source_info.bt_in.bt_info[i])->bt_link_info.codec_param.lc3plus.bit_rate;
                frame_interval = (dongle_handle->source_info.bt_in.bt_info[i])->bt_link_info.codec_param.lc3plus.frame_interval;
                sample_rate    = (dongle_handle->source_info.bt_in.bt_info[i])->bt_link_info.codec_param.lc3plus.sample_rate;
                sample_format  = (dongle_handle->source_info.bt_in.bt_info[i])->bt_link_info.codec_param.lc3plus.sample_format;
                frame_samples  = sample_rate/1000*frame_interval/1000;
                frame_size     = ull_audio_v2_codec_get_frame_size(&((dongle_handle->source_info.bt_in.bt_info[i])->bt_link_info.codec_type), &((dongle_handle->source_info.bt_in.bt_info[i])->bt_link_info.codec_param));
            }
            else
            {
                DSP_MW_LOG_E("[ULL Audio V2][UL][ERROR]codec is not supported, %d\r\n", 1, (dongle_handle->source_info.bt_in.bt_info[i])->bt_link_info.codec_type);
                AUDIO_ASSERT(0);
            }
            /* update codec type */
            if (dongle_handle->source_info.bt_in.codec_type == 0)
            {
                dongle_handle->source_info.bt_in.codec_type = codec_type;
            }
            else if (dongle_handle->source_info.bt_in.codec_type != codec_type)
            {
                DSP_MW_LOG_E("[ULL Audio V2][UL][ERROR]codec_type is not right, %u, %u\r\n", 2, dongle_handle->source_info.bt_in.codec_type, codec_type);
                AUDIO_ASSERT(0);
            }
            /* update frame interval */
            if (dongle_handle->source_info.bt_in.frame_interval == 0)
            {
                dongle_handle->source_info.bt_in.frame_interval = frame_interval;
            }
            else if (dongle_handle->source_info.bt_in.frame_interval != frame_interval)
            {
                DSP_MW_LOG_E("[ULL Audio V2][UL][ERROR]frame_interval is not right, %u, %u\r\n", 2, dongle_handle->source_info.bt_in.frame_interval, frame_interval);
                AUDIO_ASSERT(0);
            }
            /* update frame sample rate */
            if (dongle_handle->source_info.bt_in.sample_rate == 0)
            {
                dongle_handle->source_info.bt_in.sample_rate = sample_rate;
            }
            else if (dongle_handle->source_info.bt_in.sample_rate != sample_rate)
            {
                DSP_MW_LOG_E("[ULL Audio V2][UL][ERROR]sample_rate is not right, %u, %u\r\n", 2, dongle_handle->source_info.bt_in.sample_rate, sample_rate);
                AUDIO_ASSERT(0);
            }
            /* update frame sample format */
            if (dongle_handle->source_info.bt_in.sample_format == 0)
            {
                dongle_handle->source_info.bt_in.sample_format = sample_format;
            }
            else if (dongle_handle->source_info.bt_in.sample_format != sample_format)
            {
                DSP_MW_LOG_E("[ULL Audio V2][UL][ERROR]sample_format is not right, %u, %u\r\n", 2, dongle_handle->source_info.bt_in.sample_format, sample_format);
                AUDIO_ASSERT(0);
            }
            /* update frame samples */
            if (dongle_handle->source_info.bt_in.frame_samples == 0)
            {
                dongle_handle->source_info.bt_in.frame_samples = frame_samples;
            }
            else if (dongle_handle->source_info.bt_in.frame_samples != frame_samples)
            {
                DSP_MW_LOG_E("[ULL Audio V2][UL][ERROR]frame_samples is not right, %u, %u\r\n", 2, dongle_handle->source_info.bt_in.frame_samples, frame_samples);
                AUDIO_ASSERT(0);
            }
            /* update frame samples */
            if (dongle_handle->source_info.bt_in.frame_size == 0)
            {
                dongle_handle->source_info.bt_in.frame_size = frame_size;
            }
            else if (dongle_handle->source_info.bt_in.frame_size != frame_size)
            {
                DSP_MW_LOG_E("[ULL Audio V2][UL][ERROR]frame_size is not right, %u, %u\r\n", 2, dongle_handle->source_info.bt_in.frame_size, frame_size);
                AUDIO_ASSERT(0);
            }
            /* update bit_rate */
            if (dongle_handle->source_info.bt_in.bit_rate == 0)
            {
                dongle_handle->source_info.bt_in.bit_rate = bit_rate;
            }
            else if (dongle_handle->source_info.bt_in.bit_rate != bit_rate)
            {
                DSP_MW_LOG_E("[ULL Audio V2][UL][ERROR]bit_rate is not right, %u, %u\r\n", 2, dongle_handle->source_info.bt_in.bit_rate, bit_rate);
                AUDIO_ASSERT(0);
            }
        }
    }
    dongle_handle->source_info.bt_in.channel_num = channel_num;
}

static void ull_audio_v2_dongle_ul_bt_in_deinit(ull_audio_v2_dongle_ul_handle_t *dongle_handle)
{
    uint32_t i;
    uint32_t saved_mask;

    for (i = 0; i < ULL_AUDIO_V2_DATA_CHANNEL_NUMBER; i++)
    {
        if (dongle_handle->source_info.bt_in.bt_info[i] == NULL)
        {
            continue;
        }
        else
        {
            if ((dongle_handle->source_info.bt_in.bt_info[i])->user_count == 0)
            {
                DSP_MW_LOG_E("[ULL Audio V2][UL][ERROR]user_count is not right, %u\r\n", 1, (dongle_handle->source_info.bt_in.bt_info[i])->user_count);
                AUDIO_ASSERT(0);
            }
            hal_nvic_save_and_set_interrupt_mask(&saved_mask);
            (dongle_handle->source_info.bt_in.bt_info[i])->user_count -= 1;
            if ((dongle_handle->source_info.bt_in.bt_info[i])->user_count == 0)
            {
                memset(dongle_handle->source_info.bt_in.bt_info[i], 0, sizeof(ull_audio_v2_dongle_bt_info_t));
            }
            dongle_handle->source_info.bt_in.bt_info[i] = NULL;
            hal_nvic_restore_interrupt_mask(saved_mask);
        }
    }
}

static void ull_audio_v2_dongle_ul_usb_out_init(ull_audio_v2_dongle_ul_handle_t *dongle_handle, audio_transmitter_open_param_p audio_transmitter_open_param, bt_common_open_param_p bt_common_open_param)
{
    uint32_t i;
    uint32_t sample_size = 0;
    n9_dsp_share_info_ptr p_share_info;
    stream_resolution_t stream_resolution = RESOLUTION_16BIT;
    uint32_t saved_mask;

    /* bt in info init */
    ull_audio_v2_dongle_ul_bt_in_init(dongle_handle, audio_transmitter_open_param, bt_common_open_param);
    if (dongle_handle->source_info.bt_in.frame_interval == 5000)
    {
        hal_nvic_save_and_set_interrupt_mask(&saved_mask);
        dongle_handle->source_info.bt_in.play_en_delay                    = UL_PLAYEN_DELAY_FRAME_5000US;
        dongle_handle->source_info.bt_in.play_en_first_packet_safe_delay  = UL_FIRST_PACKET_SAFE_DELAY_FRAME_5000US;
        dongle_handle->source_info.bt_in.bt_retry_window                  = ull_audio_v2_bt_init_play_info.dl_retransmission_window_clk;
        dongle_handle->source_info.bt_in.bt_interval                      = UL_BT_INTERVAL_FRAME_5000US;
        dongle_handle->source_info.bt_in.bt_channel_anchor_diff           = UL_BT_INTERVAL_FRAME_5000US/2;
        hal_nvic_restore_interrupt_mask(saved_mask);
    }
    else
    {
        DSP_MW_LOG_E("[ULL Audio V2][UL][ERROR]frame_interval is not supported, %u\r\n", 1, dongle_handle->source_info.bt_in.frame_interval);
        AUDIO_ASSERT(0);
    }
    DSP_MW_LOG_I("[ULL Audio V2][UL][scenario %d-%d][handle 0x%x]bt in info, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u\r\n", 15,
                audio_transmitter_open_param->scenario_type,
                audio_transmitter_open_param->scenario_sub_id,
                dongle_handle,
                dongle_handle->source_info.bt_in.channel_num,
                dongle_handle->source_info.bt_in.sample_rate,
                dongle_handle->source_info.bt_in.sample_format,
                dongle_handle->source_info.bt_in.frame_size,
                dongle_handle->source_info.bt_in.frame_samples,
                dongle_handle->source_info.bt_in.frame_interval,
                dongle_handle->source_info.bt_in.bit_rate,
                dongle_handle->source_info.bt_in.play_en_delay,
                dongle_handle->source_info.bt_in.play_en_first_packet_safe_delay,
                dongle_handle->source_info.bt_in.bt_retry_window,
                dongle_handle->source_info.bt_in.bt_interval,
                dongle_handle->source_info.bt_in.bt_channel_anchor_diff);
    for (i = 0; i < ULL_AUDIO_V2_DATA_CHANNEL_NUMBER; i++)
    {
        if (dongle_handle->source_info.bt_in.bt_info[i] != NULL)
        {
            p_share_info = (n9_dsp_share_info_ptr)((dongle_handle->source_info.bt_in.bt_info[i])->bt_link_info.share_info);
            DSP_MW_LOG_I("[ULL Audio V2][UL][scenario %d-%d][handle 0x%x]bt in channel %u info, %u, %u, %u, %u, %u, 0x%x, 0x%x\r\n", 11,
                        audio_transmitter_open_param->scenario_type,
                        audio_transmitter_open_param->scenario_sub_id,
                        dongle_handle,
                        i+1,
                        (dongle_handle->source_info.bt_in.bt_info[i])->seq_num,
                        (dongle_handle->source_info.bt_in.bt_info[i])->user_count,
                        (dongle_handle->source_info.bt_in.bt_info[i])->blk_index,
                        (dongle_handle->source_info.bt_in.bt_info[i])->blk_index_previous,
                        (dongle_handle->source_info.bt_in.bt_info[i])->crc32_init,
                        p_share_info,
                        hal_memview_infrasys_to_dsp0(p_share_info->start_addr));
        }
    }

    /* usb in info init */
    dongle_handle->sink_info.usb_out.channel_num       = audio_transmitter_open_param->scenario_param.ull_audio_v2_dongle_param.codec_param.pcm.channel_mode;
    dongle_handle->sink_info.usb_out.sample_rate       = audio_transmitter_open_param->scenario_param.ull_audio_v2_dongle_param.codec_param.pcm.sample_rate;
    dongle_handle->sink_info.usb_out.sample_format     = audio_transmitter_open_param->scenario_param.ull_audio_v2_dongle_param.codec_param.pcm.format;
    dongle_handle->sink_info.usb_out.frame_size        = usb_audio_get_frame_size(&audio_transmitter_open_param->scenario_param.ull_audio_v2_dongle_param.codec_type, &audio_transmitter_open_param->scenario_param.ull_audio_v2_dongle_param.codec_param);
    dongle_handle->sink_info.usb_out.frame_samples     = audio_transmitter_open_param->scenario_param.ull_audio_v2_dongle_param.codec_param.pcm.sample_rate/audio_transmitter_open_param->scenario_param.ull_audio_v2_dongle_param.codec_param.pcm.frame_interval;
    dongle_handle->sink_info.usb_out.frame_interval    = audio_transmitter_open_param->scenario_param.ull_audio_v2_dongle_param.codec_param.pcm.frame_interval;
    dongle_handle->sink_info.usb_out.frame_max_num     = (dongle_handle->source_info.bt_in.frame_interval*2) / audio_transmitter_open_param->scenario_param.ull_audio_v2_dongle_param.codec_param.pcm.frame_interval;
    DSP_MW_LOG_I("[ULL Audio V2][UL][scenario %d-%d][handle 0x%x]usb out info, %u, %u, %u, %u, %u, %u, %u\r\n", 10,
                audio_transmitter_open_param->scenario_type,
                audio_transmitter_open_param->scenario_sub_id,
                dongle_handle,
                dongle_handle->sink_info.usb_out.channel_num,
                dongle_handle->sink_info.usb_out.sample_rate,
                dongle_handle->sink_info.usb_out.sample_format,
                dongle_handle->sink_info.usb_out.frame_size,
                dongle_handle->sink_info.usb_out.frame_samples,
                dongle_handle->sink_info.usb_out.frame_interval,
                dongle_handle->sink_info.usb_out.frame_max_num);

    /* codec init */
    if (dongle_handle->source_info.bt_in.codec_type == AUDIO_DSP_CODEC_TYPE_LC3PLUS)
    {
        lc3plus_dec_port_config_t lc3plus_dec_config;
        if (dongle_handle->source_info.bt_in.sample_format == HAL_AUDIO_PCM_FORMAT_S16_LE)
        {
            lc3plus_dec_config.sample_bits  = 16;
            sample_size = sizeof(uint16_t);
            stream_resolution = RESOLUTION_16BIT;
        }
        else if (dongle_handle->source_info.bt_in.sample_format == HAL_AUDIO_PCM_FORMAT_S24_LE)
        {
            lc3plus_dec_config.sample_bits  = 24;
            sample_size = sizeof(uint32_t);
            stream_resolution = RESOLUTION_32BIT;
        }
        else
        {
            DSP_MW_LOG_E("[ULL Audio V2][UL][ERROR]sample_format is not supported, %u\r\n", 1, dongle_handle->source_info.bt_in.sample_format);
            AUDIO_ASSERT(0);
        }
        lc3plus_dec_config.sample_rate      = dongle_handle->source_info.bt_in.sample_rate;
        lc3plus_dec_config.bit_rate         = dongle_handle->source_info.bt_in.bit_rate;
        lc3plus_dec_config.channel_mode     = LC3PLUS_DEC_CHANNEL_MODE_MULTI;
        lc3plus_dec_config.in_channel_num   = dongle_handle->source_info.bt_in.channel_num;
        lc3plus_dec_config.out_channel_num  = dongle_handle->source_info.bt_in.channel_num;
        lc3plus_dec_config.frame_interval   = dongle_handle->source_info.bt_in.frame_interval;
        lc3plus_dec_config.frame_size       = dongle_handle->source_info.bt_in.frame_size;
        lc3plus_dec_config.frame_samples    = dongle_handle->source_info.bt_in.frame_samples;
        lc3plus_dec_config.plc_enable       = 1;
        lc3plus_dec_config.plc_method       = LC3PLUS_PLCMETH_STD;
        stream_codec_decoder_lc3plus_init(LC3PLUS_DEC_PORT_0, dongle_handle->source, &lc3plus_dec_config);
        DSP_MW_LOG_I("[ULL Audio V2][UL][scenario %d-%d][handle 0x%x]lc3plus info, %d, %d, %d, %d, %d, %d, %d, %d, %d\r\n", 12,
                    audio_transmitter_open_param->scenario_type,
                    audio_transmitter_open_param->scenario_sub_id,
                    dongle_handle,
                    lc3plus_dec_config.sample_bits,
                    lc3plus_dec_config.sample_rate,
                    lc3plus_dec_config.bit_rate,
                    lc3plus_dec_config.channel_mode,
                    lc3plus_dec_config.in_channel_num,
                    lc3plus_dec_config.out_channel_num,
                    lc3plus_dec_config.frame_interval,
                    lc3plus_dec_config.frame_samples,
                    lc3plus_dec_config.frame_size);
    }

    /* sw mxier init */
    sw_mixer_callback_config_t       callback_config;
    sw_mixer_input_channel_config_t  in_ch_config;
    sw_mixer_output_channel_config_t out_ch_config;
    stream_function_sw_mixer_init(SW_MIXER_PORT_0);
    callback_config.preprocess_callback = NULL;
    callback_config.postprocess_callback = NULL;
    in_ch_config.total_channel_number = 2;
    in_ch_config.resolution = stream_resolution;
    in_ch_config.input_mode = SW_MIXER_CHANNEL_MODE_OVERWRITE;
    in_ch_config.buffer_size = dongle_handle->source_info.bt_in.sample_rate/1000*dongle_handle->source_info.bt_in.frame_interval/1000*sample_size;
    out_ch_config.total_channel_number = 2;
    out_ch_config.resolution = stream_resolution;
    dongle_handle->mixer_member= stream_function_sw_mixer_member_create((void *)dongle_handle->source, SW_MIXER_MEMBER_MODE_NO_BYPASS, &callback_config, &in_ch_config, &out_ch_config);
    stream_function_sw_mixer_member_register(SW_MIXER_PORT_0, dongle_handle->mixer_member, true);
    DSP_MW_LOG_I("[ULL Audio V2][UL][scenario %d-%d][handle 0x%x]sw mixer 0x%x info, %d, %d, %d, %d, %d, %d\r\n", 10,
                audio_transmitter_open_param->scenario_type,
                audio_transmitter_open_param->scenario_sub_id,
                dongle_handle,
                dongle_handle->mixer_member,
                in_ch_config.total_channel_number,
                in_ch_config.resolution,
                in_ch_config.input_mode,
                in_ch_config.buffer_size,
                out_ch_config.total_channel_number,
                out_ch_config.resolution);

    /* sw clk skew init */
    sw_clk_skew_config_t sw_clk_skew_config;
    dongle_handle->clk_skew_port = stream_function_sw_clk_skew_get_port(dongle_handle->source);
    sw_clk_skew_config.channel = 2;
    if (stream_resolution == RESOLUTION_16BIT)
    {
        sw_clk_skew_config.bits = 16;
    }
    else
    {
        sw_clk_skew_config.bits = 32;
    }
    sw_clk_skew_config.order = C_Flp_Ord_5;
    sw_clk_skew_config.skew_io_mode = C_Skew_Oup;
    sw_clk_skew_config.skew_compensation_mode = SW_CLK_SKEW_COMPENSATION_1_SAMPLE_IN_8_FRAME;
    sw_clk_skew_config.skew_work_mode = SW_CLK_SKEW_CONTINUOUS;
    sw_clk_skew_config.max_output_size = 2*dongle_handle->source_info.bt_in.sample_rate/1000*dongle_handle->source_info.bt_in.frame_interval/1000*sample_size;
    sw_clk_skew_config.continuous_frame_size = dongle_handle->source_info.bt_in.sample_rate/1000*dongle_handle->source_info.bt_in.frame_interval/1000*sample_size;
    stream_function_sw_clk_skew_init(dongle_handle->clk_skew_port, &sw_clk_skew_config);
    dongle_handle->compen_samples = 0;
    dongle_handle->clk_skew_count = 0;
    dongle_handle->clk_skew_watermark_samples = dongle_handle->source_info.bt_in.sample_rate/1000*dongle_handle->source_info.bt_in.frame_interval/1000;
    DSP_MW_LOG_I("[ULL Audio V2][UL][scenario %d-%d][handle 0x%x]sw clk skew 0x%x info, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\r\n", 15,
                audio_transmitter_open_param->scenario_type,
                audio_transmitter_open_param->scenario_sub_id,
                dongle_handle,
                dongle_handle->clk_skew_port,
                sw_clk_skew_config.channel,
                sw_clk_skew_config.bits,
                sw_clk_skew_config.order,
                sw_clk_skew_config.skew_io_mode,
                sw_clk_skew_config.skew_compensation_mode,
                sw_clk_skew_config.skew_work_mode,
                sw_clk_skew_config.max_output_size,
                sw_clk_skew_config.continuous_frame_size,
                dongle_handle->compen_samples,
                dongle_handle->clk_skew_count,
                dongle_handle->clk_skew_watermark_samples);

    /* sw buffer init */
    sw_buffer_config_t buffer_config;
    buffer_config.mode = SW_BUFFER_MODE_FIXED_OUTPUT_LENGTH;
    buffer_config.total_channels = 2;
    buffer_config.watermark_max_size = 4*dongle_handle->source_info.bt_in.sample_rate/1000*dongle_handle->source_info.bt_in.frame_interval/1000*sample_size;
    buffer_config.watermark_min_size = 0;
    buffer_config.output_size = dongle_handle->source_info.bt_in.sample_rate/1000*dongle_handle->source_info.bt_in.frame_interval/1000*sample_size;
    dongle_handle->buffer_port = stream_function_sw_buffer_get_port(dongle_handle->source);
    stream_function_sw_buffer_init(dongle_handle->buffer_port, &buffer_config);
    stream_function_sw_buffer_config_channel_prefill_size(dongle_handle->buffer_port, 0, dongle_handle->source_info.bt_in.sample_rate/1000*dongle_handle->source_info.bt_in.frame_interval/1000*sample_size, true);
    dongle_handle->buffer_default_output_size = dongle_handle->source_info.bt_in.sample_rate/1000*dongle_handle->source_info.bt_in.frame_interval/1000*sample_size;
    DSP_MW_LOG_I("[ULL Audio V2][UL][scenario %d-%d][handle 0x%x]sw buffer 0x%x info, %d, %d, %d, %d, %d, %d\r\n", 11,
                audio_transmitter_open_param->scenario_type,
                audio_transmitter_open_param->scenario_sub_id,
                dongle_handle,
                dongle_handle->buffer_port,
                buffer_config.mode,
                buffer_config.total_channels,
                buffer_config.watermark_max_size,
                buffer_config.watermark_min_size,
                buffer_config.output_size,
                dongle_handle->buffer_default_output_size);

    /* sw gain init */
    int32_t default_gain;
    sw_gain_config_t default_config;
    default_config.resolution = stream_resolution;
    default_config.target_gain = bt_common_open_param->scenario_param.ull_audio_v2_dongle_param.gain_default_L;
    default_config.up_step = 25;
    default_config.up_samples_per_step = dongle_handle->sink_info.usb_out.sample_rate/1000;
    default_config.down_step = -25;
    default_config.down_samples_per_step = dongle_handle->sink_info.usb_out.sample_rate/1000;
    dongle_handle->gain_port = stream_function_sw_gain_get_port(dongle_handle->source);
    stream_function_sw_gain_init(dongle_handle->gain_port, 2, &default_config);
    default_gain = bt_common_open_param->scenario_param.ull_audio_v2_dongle_param.gain_default_L;
    stream_function_sw_gain_configure_gain_target(dongle_handle->gain_port, 1, default_gain);
    default_gain = bt_common_open_param->scenario_param.ull_audio_v2_dongle_param.gain_default_R;
    stream_function_sw_gain_configure_gain_target(dongle_handle->gain_port, 2, default_gain);
    DSP_MW_LOG_I("[ULL Audio V2][UL][scenario %d-%d][handle 0x%x]sw gain 0x%x info, %d, %d, %d, %d, %d, %d, %d, %d\r\n", 12,
                audio_transmitter_open_param->scenario_type,
                audio_transmitter_open_param->scenario_sub_id,
                dongle_handle,
                dongle_handle->gain_port,
                default_config.resolution,
                default_config.target_gain,
                default_config.up_step,
                default_config.up_samples_per_step,
                default_config.down_step,
                default_config.down_samples_per_step,
                bt_common_open_param->scenario_param.ull_audio_v2_dongle_param.gain_default_L,
                bt_common_open_param->scenario_param.ull_audio_v2_dongle_param.gain_default_R);

    /* sw src init */
    sw_src_config_t sw_src_config;
    dongle_handle->src_in_frame_samples     = dongle_handle->source_info.bt_in.sample_rate/1000*dongle_handle->source_info.bt_in.frame_interval/1000;
    dongle_handle->src_in_frame_size        = dongle_handle->src_in_frame_samples * sample_size;
    dongle_handle->src_out_frame_samples    = dongle_handle->sink_info.usb_out.sample_rate/1000*dongle_handle->source_info.bt_in.frame_interval/1000;
    dongle_handle->src_out_frame_size       = dongle_handle->src_out_frame_samples * sample_size;
    sw_src_config.mode = SW_SRC_MODE_NORMAL;
    sw_src_config.channel_num = 2;
    sw_src_config.in_res = stream_resolution;
    sw_src_config.in_sampling_rate  = dongle_handle->source_info.bt_in.sample_rate;
    sw_src_config.in_frame_size_max = dongle_handle->source_info.bt_in.sample_rate/1000*dongle_handle->source_info.bt_in.frame_interval/1000*sample_size;
    sw_src_config.out_res           = stream_resolution;
    sw_src_config.out_sampling_rate = dongle_handle->sink_info.usb_out.sample_rate;
    sw_src_config.out_frame_size_max= dongle_handle->src_out_frame_size;
    dongle_handle->src_port = stream_function_sw_src_get_port(dongle_handle->source);
    stream_function_sw_src_init(dongle_handle->src_port, &sw_src_config);
    DSP_MW_LOG_I("[ULL Audio V2][UL][scenario %d-%d][handle 0x%x]sw src 0x%x info, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\r\n", 16,
                audio_transmitter_open_param->scenario_type,
                audio_transmitter_open_param->scenario_sub_id,
                dongle_handle,
                dongle_handle->src_port,
                sw_src_config.mode,
                sw_src_config.channel_num,
                sw_src_config.in_res,
                sw_src_config.in_sampling_rate,
                sw_src_config.in_frame_size_max,
                sw_src_config.out_res,
                sw_src_config.out_sampling_rate,
                sw_src_config.out_frame_size_max,
                dongle_handle->src_in_frame_samples,
                dongle_handle->src_in_frame_size,
                dongle_handle->src_out_frame_samples,
                dongle_handle->src_out_frame_size);
}

static void ull_audio_v2_dongle_ul_usb_out_deinit(ull_audio_v2_dongle_ul_handle_t *dongle_handle)
{
    /* bt in info deinit */
    ull_audio_v2_dongle_ul_bt_in_deinit(dongle_handle);

    /* codec deinit */
    stream_codec_decoder_lc3plus_deinit(LC3PLUS_DEC_PORT_0, dongle_handle->source);

    /* sw mixer deinit */
    stream_function_sw_mixer_member_unregister(SW_MIXER_PORT_0, dongle_handle->mixer_member);
    stream_function_sw_mixer_member_delete(dongle_handle->mixer_member);
    stream_function_sw_mixer_deinit(SW_MIXER_PORT_0);

    /* sw clk skew deinit */
    stream_function_sw_clk_skew_deinit(dongle_handle->clk_skew_port);

    /* sw buffer deinit */
    stream_function_sw_buffer_deinit(dongle_handle->buffer_port);

    /* sw gain deinit */
    stream_function_sw_gain_deinit(dongle_handle->gain_port);

    /* sw src deinit */
    stream_function_sw_src_deinit(dongle_handle->src_port);
}

static ull_audio_v2_dongle_first_packet_status_t ull_audio_v2_dongle_ul_first_packet_check(ull_audio_v2_dongle_ul_handle_t *dongle_handle, uint32_t bt_clk)
{
    uint32_t i;
    n9_dsp_share_info_ptr p_share_info;
    uint16_t blk_index = 0;
    uint16_t blk_num;
    uint16_t read_offset;
    uint32_t total_buffer_size;
    ULL_AUDIO_V2_HEADER *p_ull_audio_header;
    ull_audio_v2_dongle_first_packet_status_t first_packet_status = ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_NOT_READY;
    uint32_t anchor = 0;
    uint32_t anchor_timeout = 0;
    uint32_t anchor_playen = 0;
    uint32_t anchor_playen_safe =0;

    for (i = 0; i < ULL_AUDIO_V2_DATA_CHANNEL_NUMBER; i++)
    {
        if (dongle_handle->source_info.bt_in.bt_info[i] == NULL)
        {
            continue;
        }
        else
        {
            /* set read blk to the write blk - 1 and get this read blk' header */
            p_share_info = (n9_dsp_share_info_ptr)((dongle_handle->source_info.bt_in.bt_info[i])->bt_link_info.share_info);
            if (p_share_info->write_offset == 0)
            {
                continue;
            }
            total_buffer_size = p_share_info->sub_info.block_info.block_size * p_share_info->sub_info.block_info.block_num;
            StreamDSP_HWSemaphoreTake();
            read_offset = (p_share_info->write_offset+total_buffer_size-p_share_info->sub_info.block_info.block_size)%total_buffer_size;
            p_ull_audio_header = (ULL_AUDIO_V2_HEADER *)(hal_memview_infrasys_to_dsp0(p_share_info->start_addr) + read_offset);
            StreamDSP_HWSemaphoreGive();
            blk_index = read_offset/p_share_info->sub_info.block_info.block_size;
            blk_num = p_share_info->sub_info.block_info.block_num;

            /* check packet's timestamp */
            if (p_ull_audio_header->valid_packet == 0)
            {
                continue;
            }
            else
            {
                anchor_timeout = p_ull_audio_header->TimeStamp;
                anchor = (anchor_timeout+0x10000000-dongle_handle->source_info.bt_in.bt_retry_window) & 0x0fffffff;
                anchor_playen = (anchor_timeout+dongle_handle->source_info.bt_in.play_en_delay) & 0x0fffffff;
                anchor_playen_safe = (anchor_timeout+dongle_handle->source_info.bt_in.play_en_first_packet_safe_delay) & 0x0fffffff;

                /* check if this packet is suitable for play */
                if (anchor < anchor_playen_safe)
                {
                    if ((bt_clk >= anchor) && (bt_clk <= anchor_playen_safe))
                    {
                        /* --------- ........ --------- anchor ---------- bt_clk ---------- anchor_playen_safe --------- ........ -------- */
                        first_packet_status = ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_READY;
                        goto _find_first_packet;
                    }
                }
                else
                {
                    if ((bt_clk < anchor) && (bt_clk <= anchor_playen_safe))
                    {
                        /* --------- ........ ---------- bt_clk ---------- anchor_playen_safe --------- ........ --------- anchor -------- */
                        first_packet_status = ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_READY;
                        goto _find_first_packet;
                    }
                    else if ((bt_clk >= anchor) && (bt_clk > anchor_playen_safe))
                    {
                        /* --------- ........ ---------- anchor_playen_safe --------- ........ --------- anchor -------- bt_clk ---------- */
                        first_packet_status = ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_READY;
                        goto _find_first_packet;
                    }
                }
            }
        }
    }

_find_first_packet:
    if (first_packet_status == ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_READY)
    {
        /* update play info */
        dongle_handle->source_info.bt_in.play_en_bt_clk = anchor_playen;
        dongle_handle->source_info.bt_in.first_anchor_bt_clk = anchor;
        dongle_handle->source_info.bt_in.first_packet_bt_clk = anchor_timeout;
        dongle_handle->source_info.bt_in.fetch_anchor_previous = (anchor_timeout+0x10000000-dongle_handle->source_info.bt_in.bt_interval) & 0x0fffffff;
        dongle_handle->source_info.bt_in.fetch_anchor = anchor_timeout;
        for (i = 0; i < ULL_AUDIO_V2_DATA_CHANNEL_NUMBER; i++)
        {
            if (dongle_handle->source_info.bt_in.bt_info[i] == NULL)
            {
                continue;
            }
            else
            {
                (dongle_handle->source_info.bt_in.bt_info[i])->blk_index_previous = (blk_index+blk_num-1)%blk_num;
                (dongle_handle->source_info.bt_in.bt_info[i])->blk_index = blk_index;
            }
        }
        DSP_MW_LOG_I("[ULL Audio V2][UL][handle 0x%x] first packet is ready %d, 0x%x, 0x%x, 0x%x, 0x%x", 6,
                        dongle_handle,
                        blk_index,
                        dongle_handle->source_info.bt_in.first_anchor_bt_clk,
                        dongle_handle->source_info.bt_in.first_packet_bt_clk,
                        bt_clk,
                        dongle_handle->source_info.bt_in.play_en_bt_clk);
    }
    else
    {
        DSP_MW_LOG_W("[ULL Audio V2][UL][WARNNING][handle 0x%x] first packet is not ready %d, 0x%x, 0x%x, 0x%x, 0x%x", 6,
                        dongle_handle,
                        blk_index,
                        anchor,
                        anchor_timeout,
                        bt_clk,
                        anchor_playen);
    }

    return first_packet_status;
}

static bool ull_audio_v2_dongle_ul_fetch_time_is_arrived(ull_audio_v2_dongle_ul_handle_t *dongle_handle, uint32_t bt_clk)
{
    bool ret = false;
    uint32_t fetch_anchor_previous_2;

    if ((dongle_handle->first_packet_status == ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_PLAYED) || (dongle_handle->first_packet_status == ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_READY))
    {
        fetch_anchor_previous_2 = (dongle_handle->source_info.bt_in.fetch_anchor_previous+0x10000000-dongle_handle->source_info.bt_in.bt_interval) & 0x0fffffff;
        if (fetch_anchor_previous_2 < dongle_handle->source_info.bt_in.fetch_anchor)
        {
            if (bt_clk >= dongle_handle->source_info.bt_in.fetch_anchor)
            {
                /* --------- ........ --------- fetch_anchor_previous ---------- fetch_anchor ---------- bt_clk --------- ........ -------- */
                ret = true;
            }
            else if ((bt_clk < dongle_handle->source_info.bt_in.fetch_anchor) && (bt_clk < fetch_anchor_previous_2))
            {
                /* --------- ........ ---------- bt_clk --------- ........ --------- fetch_anchor_previous_2   --------- fetch_anchor_previous ---------- fetch_anchor -------- */
                ret = true;
            }
        }
        else
        {
            if ((bt_clk >= dongle_handle->source_info.bt_in.fetch_anchor) && (bt_clk < fetch_anchor_previous_2))
            {
                /* ---------- fetch_anchor ---------- bt_clk --------- ........ --------- fetch_anchor_previous_2 --------- fetch_anchor_previous -------- */
                /* --------- fetch_anchor_previous ---------- fetch_anchor ---------- bt_clk --------- ........ --------- fetch_anchor_previous_2  --------- */
                ret = true;
            }
        }
    }

    return ret;
}

static bool ull_audio_v2_dongle_ul_fetch_time_is_timeout(ull_audio_v2_dongle_ul_handle_t *dongle_handle, uint32_t bt_clk)
{
    bool ret = false;
    uint32_t fetch_anchor_next_2;

    if ((dongle_handle->first_packet_status == ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_PLAYED) || (dongle_handle->first_packet_status == ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_READY))
    {
        fetch_anchor_next_2 = (dongle_handle->source_info.bt_in.fetch_anchor+dongle_handle->source_info.bt_in.bt_interval*2) & 0x0fffffff;
        if (dongle_handle->source_info.bt_in.fetch_anchor < fetch_anchor_next_2)
        {
            if (bt_clk >= fetch_anchor_next_2)
            {
                /* --------- ........ --------- fetch_anchor ---------- fetch_anchor_next ---------- fetch_anchor_next_2 ---------- bt_clk --------- ........ -------- */
                ret = true;
            }
            else if ((bt_clk < dongle_handle->source_info.bt_in.fetch_anchor) && (bt_clk < dongle_handle->source_info.bt_in.fetch_anchor_previous) && (bt_clk < fetch_anchor_next_2))
            {
                /* --------- ........ --------  bt_clk --------- ........ --------- fetch_anchor_previous --------- fetch_anchor ---------- fetch_anchor_next ---------- fetch_anchor_next_2 ---------- */
                ret = true;
            }
        }
        else
        {
            if ((bt_clk >= fetch_anchor_next_2) && (bt_clk < dongle_handle->source_info.bt_in.fetch_anchor_previous))
            {
                /* ---------- fetch_anchor_next_2 --------- ........ ---------- bt_clk --------- ........ --------- fetch_anchor_previous --------- fetch_anchor --------- fetch_anchor_next -------- */
                /* --------- fetch_anchor_next ---------- fetch_anchor_next_2 --------- ........ ---------- bt_clk --------- ........ --------- fetch_anchor_previous --------- fetch_anchor -------- */
                ret = true;
            }
        }
    }

    return ret;
}

static bool ull_audio_v2_dongle_ul_fetch_timestamp_is_correct(ull_audio_v2_dongle_ul_handle_t *dongle_handle, uint32_t timestamp)
{
    bool ret = false;
    uint32_t fetch_anchor_max;
    uint32_t fetch_anchor_min;

    if ((dongle_handle->first_packet_status == ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_PLAYED) || (dongle_handle->first_packet_status == ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_READY))
    {
        if (timestamp == dongle_handle->source_info.bt_in.fetch_anchor)
        {
            ret = true;
        }
        else
        {
            fetch_anchor_max = (dongle_handle->source_info.bt_in.fetch_anchor+dongle_handle->source_info.bt_in.bt_channel_anchor_diff) & 0x0fffffff;
            fetch_anchor_min = (dongle_handle->source_info.bt_in.fetch_anchor+0x10000000-dongle_handle->source_info.bt_in.bt_channel_anchor_diff) & 0x0fffffff;
            if (fetch_anchor_max > fetch_anchor_min)
            {
                if ((timestamp < fetch_anchor_max) && (timestamp > fetch_anchor_min))
                {
                    ret = true;
                }
            }
            else
            {
                if ((timestamp < fetch_anchor_max) || (timestamp > fetch_anchor_min))
                {
                    ret = true;
                }
            }
        }
    }

    return ret;
}

static int32_t ull_audio_v2_dongle_ul_usb_clock_skew_check(ull_audio_v2_dongle_ul_handle_t *dongle_handle)
{
    int32_t compensatory_samples = 0;
    int32_t remain_samples;
    int32_t input_frame = 1;
    int32_t frame_samples = (int32_t)dongle_handle->source_info.bt_in.frame_samples;
    int32_t sample_size = 0;

    if (dongle_handle->source_info.bt_in.sample_format == HAL_AUDIO_PCM_FORMAT_S24_LE)
    {
        sample_size = sizeof(int32_t);
    }
    else
    {
        sample_size = sizeof(int16_t);
    }

    if (dongle_handle->first_packet_status == ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_PLAYED)
    {
        /* get remain samples */
        remain_samples = stream_function_sw_buffer_get_channel_used_size(dongle_handle->buffer_port, 1) / sample_size;

        if (dongle_handle->fetch_count != 0)
        {
            input_frame = 0; /* input 1 frame, then output 1 frame, so input frame is zero */
        }
        if (remain_samples+input_frame*frame_samples > dongle_handle->clk_skew_watermark_samples)
        {
            if (dongle_handle->clk_skew_count > -UL_CLOCK_SKEW_CHECK_COUNT)
            {
                dongle_handle->clk_skew_count -= 1;
            }
        }
        else if (remain_samples+input_frame*frame_samples < dongle_handle->clk_skew_watermark_samples)
        {
            if (dongle_handle->clk_skew_count < UL_CLOCK_SKEW_CHECK_COUNT)
            {
                dongle_handle->clk_skew_count += 1;
            }
        }
        else
        {
            // if ((dongle_handle->clk_skew_count < UL_CLOCK_SKEW_CHECK_COUNT) && (dongle_handle->clk_skew_count > 0))
            // {
            //     dongle_handle->clk_skew_count -= 1;
            // }
            // else if ((dongle_handle->clk_skew_count > UL_CLOCK_SKEW_CHECK_COUNT) && (dongle_handle->clk_skew_count < 0))
            // {
            //     dongle_handle->clk_skew_count += 1;
            // }
            dongle_handle->clk_skew_count = 0;
        }

        if (dongle_handle->clk_skew_count == UL_CLOCK_SKEW_CHECK_COUNT)
        {
            compensatory_samples = 1;
        }
        else if (dongle_handle->clk_skew_count == -UL_CLOCK_SKEW_CHECK_COUNT)
        {
            compensatory_samples = -1;
        }
        else
        {
            compensatory_samples = 0;
        }
    }

    return compensatory_samples;
}
/* Public functions ----------------------------------------------------------*/
/******************************************************************************/
/*               ULL audio 2.0 dongle common Public Functions                 */
/******************************************************************************/
void ull_audio_v2_dongle_init_play_info(hal_ccni_message_t msg, hal_ccni_message_t *ack)
{
    ull_audio_v2_init_play_info_t *play_info;
    ull_audio_v2_dongle_ul_handle_t *c_handle;
    uint32_t i;
    uint32_t saved_mask;
    UNUSED(ack);

    /* save play info to the global variables */
    play_info = (ull_audio_v2_init_play_info_t *)hal_memview_cm4_to_dsp0(msg.ccni_message[1]);
    memcpy(&ull_audio_v2_bt_init_play_info, play_info, sizeof(ull_audio_v2_init_play_info_t));

    /* update uplink BT transmission window clk */
    hal_nvic_save_and_set_interrupt_mask(&saved_mask);
    if (ull_audio_v2_dongle_first_ul_handle != NULL)
    {
        c_handle = ull_audio_v2_dongle_first_ul_handle;
        for (i=0; i < (uint32_t)ull_audio_v2_dongle_first_ul_handle->total_number; i++)
        {
            if ((ull_audio_v2_bt_init_play_info.dl_retransmission_window_clk != 0) && (c_handle->source_info.bt_in.bt_retry_window != ull_audio_v2_bt_init_play_info.dl_retransmission_window_clk))
            {
                c_handle->source_info.bt_in.bt_retry_window = ull_audio_v2_bt_init_play_info.dl_retransmission_window_clk;
            }
            /* switch the next handle */
            c_handle = c_handle->next_ul_handle;
        }
    }
    hal_nvic_restore_interrupt_mask(saved_mask);

    DSP_MW_LOG_I("[ULL Audio V2][UL][config] play_info->dl_timestamp_clk %d",   1, ull_audio_v2_bt_init_play_info.dl_retransmission_window_clk);
    DSP_MW_LOG_I("[ULL Audio V2][UL][config] play_info->dl_timestamp_phase %d", 1, ull_audio_v2_bt_init_play_info.dl_retransmission_window_phase);
}
/******************************************************************************/
/*               ULL audio 2.0 dongle DL path Public Functions                */
/******************************************************************************/
uint32_t ull_audio_v2_dongle_dl_get_stream_in_max_size_each_channel(SOURCE source, SINK sink)
{
    ull_audio_v2_dongle_dl_handle_t *dongle_handle = (ull_audio_v2_dongle_dl_handle_t *)sink->param.bt_common.scenario_param.dongle_handle;
    uint32_t stream_in_size = 0;
    uint32_t frame_samples;
    UNUSED(source);
    UNUSED(sink);

    switch (dongle_handle->sub_id)
    {
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0:
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_1:
            frame_samples = max(dongle_handle->source_info.usb_in.frame_samples, dongle_handle->sink_info.bt_out.frame_samples*1000/dongle_handle->sink_info.bt_out.frame_interval);
            stream_in_size = 2*dongle_handle->source_info.usb_in.frame_max_num*frame_samples*sizeof(uint32_t);
            break;

        default:
            AUDIO_ASSERT(0);
            break;
    }

    return stream_in_size;
}

uint32_t ull_audio_v2_dongle_dl_get_stream_in_channel_number(SOURCE source, SINK sink)
{
    ull_audio_v2_dongle_dl_handle_t *dongle_handle = (ull_audio_v2_dongle_dl_handle_t *)sink->param.bt_common.scenario_param.dongle_handle;
    uint32_t channel_num = 0;
    UNUSED(source);
    UNUSED(sink);

    switch (dongle_handle->sub_id)
    {
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0:
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_1:
            channel_num = dongle_handle->source_info.usb_in.channel_num;
            break;

        default:
            AUDIO_ASSERT(0);
            break;
    }

    return channel_num;
}

stream_samplerate_t ull_audio_v2_dongle_dl_get_stream_in_sampling_rate_each_channel(SOURCE source, SINK sink)
{
    ull_audio_v2_dongle_dl_handle_t *dongle_handle = (ull_audio_v2_dongle_dl_handle_t *)sink->param.bt_common.scenario_param.dongle_handle;
    stream_samplerate_t samplerate = 0;
    UNUSED(source);
    UNUSED(sink);

    switch (dongle_handle->sub_id)
    {
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0:
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_1:
            samplerate = dongle_handle->source_info.usb_in.sample_rate/1000;
            break;

        default:
            AUDIO_ASSERT(0);
            break;
    }

    return samplerate;
}

uint32_t ull_audio_v2_dongle_dl_get_stream_out_max_size_each_channel(SOURCE source, SINK sink)
{
    ull_audio_v2_dongle_dl_handle_t *dongle_handle = (ull_audio_v2_dongle_dl_handle_t *)sink->param.bt_common.scenario_param.dongle_handle;
    uint32_t stream_out_size = 0;
    UNUSED(source);
    UNUSED(sink);

    switch (dongle_handle->sub_id)
    {
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0:
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_1:
            stream_out_size = 2*dongle_handle->sink_info.bt_out.frame_size;
            break;

        default:
            AUDIO_ASSERT(0);
            break;
    }

    return stream_out_size;
}

uint32_t ull_audio_v2_dongle_dl_get_stream_out_channel_number(SOURCE source, SINK sink)
{
    ull_audio_v2_dongle_dl_handle_t *dongle_handle = (ull_audio_v2_dongle_dl_handle_t *)sink->param.bt_common.scenario_param.dongle_handle;
    uint32_t channel_num = 0;
    UNUSED(source);
    UNUSED(sink);

    switch (dongle_handle->sub_id)
    {
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0:
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_1:
            channel_num = dongle_handle->sink_info.bt_out.channel_num;
            break;

        default:
            AUDIO_ASSERT(0);
            break;
    }

    return channel_num;
}

stream_samplerate_t ull_audio_v2_dongle_dl_get_stream_out_sampling_rate_each_channel(SOURCE source, SINK sink)
{
    ull_audio_v2_dongle_dl_handle_t *dongle_handle = (ull_audio_v2_dongle_dl_handle_t *)sink->param.bt_common.scenario_param.dongle_handle;
    stream_samplerate_t samplerate = 0;
    UNUSED(source);
    UNUSED(sink);

    switch (dongle_handle->sub_id)
    {
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0:
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_1:
            samplerate = dongle_handle->sink_info.bt_out.sample_rate/1000;
            break;

        default:
            AUDIO_ASSERT(0);
            break;
    }

    return samplerate;
}

void ull_audio_v2_dongle_dl_init(SOURCE source, SINK sink, audio_transmitter_open_param_p audio_transmitter_open_param, bt_common_open_param_p bt_common_open_param)
{
    /* get handle for application config */
    ull_audio_v2_dongle_dl_handle_t *dongle_handle = ull_audio_v2_dongle_dl_get_handle(sink);
    sink->param.bt_common.scenario_param.dongle_handle = (void *)dongle_handle;
    dongle_handle->source = source;
    dongle_handle->sink   = sink;
    dongle_handle->sub_id = audio_transmitter_open_param->scenario_sub_id;

    /* task config */
    source->taskId = DHP_TASK_ID;
    sink->taskid = DHP_TASK_ID;

    /* init audio info */
    switch (dongle_handle->sub_id)
    {
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0:
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_1:
            ull_audio_v2_dongle_dl_usb_in_init(dongle_handle, audio_transmitter_open_param, bt_common_open_param);
            break;

        default:
            AUDIO_ASSERT(0);
            break;
    }

    /* lock sleep because sleep wake-up time will consume the stream process time */
    //hal_sleep_manager_lock_sleep(SLEEP_LOCK_AUDIO);

    /* register ccni handler */
    audio_transmitter_register_isr_handler(ull_audio_v2_dongle_dl_ccni_handler);

    /* stream status update */
    dongle_handle->stream_status = ULL_AUDIO_V2_DONGLE_STREAM_INIT;
}

void ull_audio_v2_dongle_dl_deinit(SOURCE source, SINK sink)
{
    ull_audio_v2_dongle_dl_handle_t *dongle_handle = (ull_audio_v2_dongle_dl_handle_t *)sink->param.bt_common.scenario_param.dongle_handle;
    uint32_t saved_mask;
    UNUSED(source);
    UNUSED(sink);

    /* unregister ccni handler */
    hal_nvic_save_and_set_interrupt_mask(&saved_mask);
    if (dongle_handle->total_number == 1)
    {
        audio_transmitter_unregister_isr_handler(ull_audio_v2_dongle_dl_ccni_handler);
    }
    hal_nvic_restore_interrupt_mask(saved_mask);

    /* deinit audio info */
    switch (dongle_handle->sub_id)
    {
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0:
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_1:
            ull_audio_v2_dongle_dl_usb_in_deinit(dongle_handle);
            break;

        default:
            AUDIO_ASSERT(0);
            break;
    }

    /* unlock sleep */
    //hal_sleep_manager_unlock_sleep(SLEEP_LOCK_AUDIO);

    /* stream status update */
    dongle_handle->stream_status = ULL_AUDIO_V2_DONGLE_STREAM_DEINIT;

    /* release handle */
    ull_audio_v2_dongle_dl_release_handle(dongle_handle);
    sink->param.bt_common.scenario_param.dongle_handle = NULL;
}

void ull_audio_v2_dongle_dl_start(SOURCE source, SINK sink)
{
    ull_audio_v2_dongle_dl_handle_t *dongle_handle = (ull_audio_v2_dongle_dl_handle_t *)sink->param.bt_common.scenario_param.dongle_handle;
    UNUSED(source);
    UNUSED(sink);

    switch (dongle_handle->sub_id)
    {
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0:
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_1:
            dongle_handle->fetch_count = 0;
            dongle_handle->first_packet_status = ULL_AUDIO_V2_DONGLE_DL_FIRST_PACKET_NOT_READY;
            break;

        default:
            AUDIO_ASSERT(0);
            break;
    }

    /* stream status update */
    dongle_handle->stream_status = ULL_AUDIO_V2_DONGLE_STREAM_START;
}

void ull_audio_v2_dongle_dl_stop(SOURCE source, SINK sink)
{
    ull_audio_v2_dongle_dl_handle_t *dongle_handle = (ull_audio_v2_dongle_dl_handle_t *)sink->param.bt_common.scenario_param.dongle_handle;
    uint32_t saved_mask;
    UNUSED(source);
    UNUSED(sink);

    switch (dongle_handle->sub_id)
    {
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0:
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_1:
            hal_nvic_save_and_set_interrupt_mask(&saved_mask);
            dongle_handle->first_packet_status = ULL_AUDIO_V2_DONGLE_DL_FIRST_PACKET_NOT_READY;
            hal_nvic_restore_interrupt_mask(saved_mask);
            break;

        default:
            AUDIO_ASSERT(0);
            break;
    }
}

bool ull_audio_v2_dongle_dl_config(SOURCE source, stream_config_type type, U32 value)
{
    bool ret = true;
    ull_audio_v2_dongle_runtime_config_param_p config_param = (ull_audio_v2_dongle_runtime_config_param_p)value;
    ull_audio_v2_dongle_dl_handle_t *dongle_handle = (ull_audio_v2_dongle_dl_handle_t *)source->transform->sink->param.bt_common.scenario_param.dongle_handle;
    uint32_t vol_ch;
    int32_t vol_gain;
    sw_gain_config_t old_config;
    UNUSED(source);
    UNUSED(type);
    UNUSED(value);

    switch (config_param->config_operation)
    {
        case ULL_AUDIO_V2_DONGLE_CONFIG_OP_SET_DL_VOL_INFO:
            vol_ch = config_param->config_param.vol_gain.vol_ch;
            vol_gain = config_param->config_param.vol_gain.vol_gain;
            if (vol_ch != 0)
            {
                stream_function_sw_gain_get_config(dongle_handle->gain_port, vol_ch, &old_config);
                DSP_MW_LOG_I("[ULL Audio V2][DL][config][handle 0x%x]scenario %d-%d change channel %d gain from %d*0.01dB to %d*0.01dB\r\n", 6,
                            dongle_handle,
                            source->param.data_dl.scenario_type,
                            source->param.data_dl.scenario_sub_id,
                            vol_ch,
                            old_config.target_gain,
                            vol_gain);
                stream_function_sw_gain_configure_gain_target(dongle_handle->gain_port, vol_ch, vol_gain);
            }
            else
            {
                vol_ch = 1;
                stream_function_sw_gain_get_config(dongle_handle->gain_port, vol_ch, &old_config);
                DSP_MW_LOG_I("[ULL Audio V2][DL][config][handle 0x%x]scenario %d-%d change channel %d gain from %d*0.01dB to %d*0.01dB\r\n", 6,
                            dongle_handle,
                            source->param.data_dl.scenario_type,
                            source->param.data_dl.scenario_sub_id,
                            vol_ch,
                            old_config.target_gain,
                            vol_gain);
                stream_function_sw_gain_configure_gain_target(dongle_handle->gain_port, vol_ch, vol_gain);
                vol_ch = 2;
                stream_function_sw_gain_get_config(dongle_handle->gain_port, vol_ch, &old_config);
                DSP_MW_LOG_I("[ULL Audio V2][DL][config][handle 0x%x]scenario %d-%d change channel %d gain from %d*0.01dB to %d*0.01dB\r\n", 6,
                            dongle_handle,
                            source->param.data_dl.scenario_type,
                            source->param.data_dl.scenario_sub_id,
                            vol_ch,
                            old_config.target_gain,
                            vol_gain);
                stream_function_sw_gain_configure_gain_target(dongle_handle->gain_port, vol_ch, vol_gain);
            }
            break;

        case ULL_AUDIO_V2_DONGLE_CONFIG_OP_SET_DL_MIX:
        case ULL_AUDIO_V2_DONGLE_CONFIG_OP_SET_DL_UNMIX:
        case ULL_AUDIO_V2_DONGLE_CONFIG_OP_SET_DL_BITRATE:
            break;

        default:
            DSP_MW_LOG_E("[ULL Audio V2][DL][config] unknow cmd:%d\r\n", 1, config_param->config_operation);
            break;
    }

    return ret;
}

ATTR_TEXT_IN_IRAM bool ull_audio_v2_dongle_dl_source_get_avail_size(SOURCE source, uint32_t *avail_size)
{
    ull_audio_v2_dongle_dl_handle_t *dongle_handle = (ull_audio_v2_dongle_dl_handle_t *)source->transform->sink->param.bt_common.scenario_param.dongle_handle;
    uint32_t unprocess_frames;
    uint32_t saved_mask;
    int32_t remain_samples_0;

    if (dongle_handle->fetch_count == 0)
    {
        /* there is no fetch requirement, so set avail size as 0 */
        *avail_size = 0;
    }
    else
    {
        /* get actual data size include header size in the share buffer */
        if (source->streamBuffer.ShareBufferInfo.read_offset < source->streamBuffer.ShareBufferInfo.write_offset)
        {
            /* normal case */
            *avail_size = source->streamBuffer.ShareBufferInfo.write_offset - source->streamBuffer.ShareBufferInfo.read_offset;
        }
        else if (source->streamBuffer.ShareBufferInfo.read_offset == source->streamBuffer.ShareBufferInfo.write_offset)
        {
            if(source->streamBuffer.ShareBufferInfo.bBufferIsFull == true)
            {
                /* buffer is full, so read_offset == write_offset */
                *avail_size = source->streamBuffer.ShareBufferInfo.sub_info.block_info.block_size * source->streamBuffer.ShareBufferInfo.sub_info.block_info.block_num;
            }
            else
            {
                /* buffer is empty, so read_offset == write_offset */
                *avail_size = 0;
                DSP_MW_LOG_W("[ULL Audio V2][DL][WARNNING][handle 0x%x]data is 0\r\n", 1, dongle_handle);
            }
        }
        else
        {
            /* buffer wrapper case */
            *avail_size = source->streamBuffer.ShareBufferInfo.sub_info.block_info.block_size * source->streamBuffer.ShareBufferInfo.sub_info.block_info.block_num
                        - source->streamBuffer.ShareBufferInfo.read_offset
                        + source->streamBuffer.ShareBufferInfo.write_offset;
        }

        /* get actual data size exclude header size in the share buffer */
        if (*avail_size != 0)
        {
            /* get unprocess frame number */
            unprocess_frames = *avail_size/source->streamBuffer.ShareBufferInfo.sub_info.block_info.block_size;
            /* if this is the first time, check if the data in usb buffer is larger than starting standard */
            if (dongle_handle->first_packet_status == ULL_AUDIO_V2_DONGLE_DL_FIRST_PACKET_NOT_READY)
            {
                if (unprocess_frames < dongle_handle->source_info.usb_in.frame_max_num)
                {
                    /* the data is not enough, so change avail_size to zero */
                    *avail_size = 0;
                    if (hal_nvic_query_exception_number() > 0)
                    {
                        /* update data state machine */
                        dongle_handle->data_status = ULL_AUDIO_V2_DONGLE_DL_DATA_EMPTY;
                        /* decrease fetch count */
                        hal_nvic_save_and_set_interrupt_mask(&saved_mask);
                        if (dongle_handle->fetch_count == 0)
                        {
                            DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR][handle 0x%x]fetch_count is 0\r\n", 1, dongle_handle);
                            AUDIO_ASSERT(0);
                        }
                        dongle_handle->fetch_count = 0;
                        hal_nvic_restore_interrupt_mask(saved_mask);
                    }
                }
                else
                {
                    /* change avail_size to actual data size in the buffer */
                    *avail_size = *avail_size - unprocess_frames*sizeof(audio_transmitter_frame_header_t);
                    if (hal_nvic_query_exception_number() > 0)
                    {
                        /* update data state machine */
                        dongle_handle->data_status = ULL_AUDIO_V2_DONGLE_DL_DATA_IN_STREAM;
                        /* reset fetch count to 1 */
                        hal_nvic_save_and_set_interrupt_mask(&saved_mask);
                        if (dongle_handle->fetch_count == 0)
                        {
                            DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR][handle 0x%x]fetch_count is 0\r\n", 1, dongle_handle);
                            AUDIO_ASSERT(0);
                        }
                        dongle_handle->fetch_count = 1;
                        hal_nvic_restore_interrupt_mask(saved_mask);
                    }
                }
            }
            else
            {
                /* change avail_size to actual data size in the buffer */
                *avail_size = *avail_size - unprocess_frames*sizeof(audio_transmitter_frame_header_t);
                if (hal_nvic_query_exception_number() > 0)
                {
                    /* get remain samples */
                    remain_samples_0 = stream_function_sw_buffer_get_channel_used_size(dongle_handle->buffer_port_0, 1) / sizeof(int32_t);
                    /* check if usb data is enough for one DL packet */
                    if (unprocess_frames*dongle_handle->source_info.usb_in.frame_samples+remain_samples_0 >= dongle_handle->src_in_frame_samples)
                    {
                        /* update data state machine */
                        dongle_handle->data_status = ULL_AUDIO_V2_DONGLE_DL_DATA_IN_STREAM;
                        /* reset state machine */
                        dongle_handle->process_frames = 0;
                    }
                    else
                    {
                        /* change avail_size to 0 because data is not enough */
                        *avail_size = 0;
                        /* update data state machine */
                        dongle_handle->data_status = ULL_AUDIO_V2_DONGLE_DL_DATA_EMPTY;
                        /* reset state machine */
                        dongle_handle->process_frames = 0;
                        /* decrease fetch count */
                        hal_nvic_save_and_set_interrupt_mask(&saved_mask);
                        if (dongle_handle->fetch_count != 0)
                        {
                            dongle_handle->fetch_count = 0;
                        }
                        hal_nvic_restore_interrupt_mask(saved_mask);
                        DSP_MW_LOG_W("[ULL Audio V2][DL][WARNNING][handle 0x%x]data is not enough, %d, %d, %d\r\n", 4, dongle_handle, unprocess_frames*dongle_handle->source_info.usb_in.frame_samples, remain_samples_0, dongle_handle->src_in_frame_samples);
                    }
                }
            }
        }
        else
        {
            if (hal_nvic_query_exception_number() > 0)
            {
                /* update data state machine */
                dongle_handle->data_status = ULL_AUDIO_V2_DONGLE_DL_DATA_EMPTY;
                /* decrease fetch count */
                hal_nvic_save_and_set_interrupt_mask(&saved_mask);
                if (dongle_handle->fetch_count != 0)
                {
                    dongle_handle->fetch_count = 0;
                }
                hal_nvic_restore_interrupt_mask(saved_mask);
                /* reset state machine */
                dongle_handle->process_frames = 0;
            }
        }
    }

#if ULL_AUDIO_V2_DONGLE_DL_PATH_DEBUG_LOG
    uint32_t current_timestamp = 0;
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &current_timestamp);
    DSP_MW_LOG_I("[ULL Audio V2][DL][DEBUG][handle 0x%x][get_avail_size]:%u, %u, %d, %u, %d, %d", 7, dongle_handle, dongle_handle->data_status, source->streamBuffer.ShareBufferInfo.read_offset, source->streamBuffer.ShareBufferInfo.write_offset, *avail_size, current_timestamp, hal_nvic_query_exception_number());
#endif /* ULL_AUDIO_V2_DONGLE_DL_PATH_DEBUG_LOG */

    return true;
}

ATTR_TEXT_IN_IRAM uint32_t ull_audio_v2_dongle_dl_source_copy_payload(SOURCE source, uint8_t *src_buf, uint8_t *dst_buf, uint32_t length)
{
    ull_audio_v2_dongle_dl_handle_t *dongle_handle = (ull_audio_v2_dongle_dl_handle_t *)source->transform->sink->param.bt_common.scenario_param.dongle_handle;
    DSP_STREAMING_PARA_PTR stream = DSP_Streaming_Get(source, source->transform->sink);
    uint32_t unprocess_frames;
    uint32_t total_samples;
    uint32_t avail_size;
    uint32_t read_offset;
    uint32_t current_frame;
    uint32_t current_frame_samples;
    uint32_t total_buffer_size;
    audio_transmitter_frame_header_t *frame_header;
    UNUSED(dst_buf);

    /* get the total buffer size in share buffer */
    total_buffer_size = source->streamBuffer.ShareBufferInfo.sub_info.block_info.block_size * source->streamBuffer.ShareBufferInfo.sub_info.block_info.block_num;

    /* copy the pcm data in share buffer into the stream buffers */
    if (dongle_handle->first_packet_status == ULL_AUDIO_V2_DONGLE_DL_FIRST_PACKET_NOT_READY)
    {
        /* get the newest total frame size and unprocessed frame num */
        ull_audio_v2_dongle_dl_source_get_avail_size(source, &avail_size);
        if(avail_size > length)
        {
            /* at the first time, maybe the actual data size is larger than stream->callback.EntryPara.in_malloc_size */
            /* in dsp_callback.c, DSP_Callback_Processing will cut length to the stream->callback.EntryPara.in_malloc_size */
            /* in here, we need to change length back to the actual data size */
            length = avail_size;
        }
        unprocess_frames = length / (source->streamBuffer.ShareBufferInfo.sub_info.block_info.block_size-sizeof(audio_transmitter_frame_header_t));
        /* check if the unprocessed frame is enough */
        if (unprocess_frames < dongle_handle->source_info.usb_in.frame_max_num)
        {
            /* in here, it means the unprocessed frames is not enough */
            DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR][handle 0x%x]unprocess frame is not enough, %u, %u\r\n", 1, dongle_handle, unprocess_frames, dongle_handle->source_info.usb_in.frame_max_num);
            AUDIO_ASSERT(0);
        }
        else
        {
            /* Only copy the latest x.xms data from the share buffer and the remian data in the head are dropped */
            total_samples = 0;
            read_offset = (source->streamBuffer.ShareBufferInfo.read_offset + source->streamBuffer.ShareBufferInfo.sub_info.block_info.block_size*(unprocess_frames-dongle_handle->source_info.usb_in.frame_max_num)) % total_buffer_size;
            src_buf = (uint8_t *)(source->streamBuffer.ShareBufferInfo.start_addr + read_offset);
            current_frame = 0;
            while(current_frame < dongle_handle->source_info.usb_in.frame_max_num)
            {
                /* get current frame info and current frame samples */
                frame_header = (audio_transmitter_frame_header_t *)src_buf;
                if (frame_header->payload_len != dongle_handle->source_info.usb_in.frame_size)
                {
                    DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR][handle 0x%x]frame size is not matched, %u, %u\r\n", 3, dongle_handle, frame_header->payload_len, dongle_handle->source_info.usb_in.frame_size);
                    AUDIO_ASSERT(0);
                }
                current_frame_samples = dongle_handle->source_info.usb_in.frame_samples;
                /* copy usb audio data from share buffer */
                /* 0.5ms case */
                if ((current_frame == 0) && ((dongle_handle->sink_info.bt_out.frame_interval%500)%2 != 0))
                {
                    /* only copy 0.5ms(for example 48K*0.5ms=24samples) data at the first time */
                    current_frame_samples = dongle_handle->source_info.usb_in.frame_samples/2;
                    /* offset 0.5ms data in src_buf */
                    src_buf = src_buf + dongle_handle->source_info.usb_in.frame_size/2;
                }
                switch (dongle_handle->source_info.usb_in.channel_num)
                {
                    case 2:
                        if (dongle_handle->source_info.usb_in.sample_format == HAL_AUDIO_PCM_FORMAT_S16_LE)
                        {
                            ShareBufferCopy_I_16bit_to_D_32bit_2ch((uint32_t *)(src_buf + sizeof(audio_transmitter_frame_header_t)),
                                                                    (uint32_t *)(stream->callback.EntryPara.in_ptr[0])+total_samples,
                                                                    (uint32_t *)(stream->callback.EntryPara.in_ptr[1])+total_samples,
                                                                    current_frame_samples);
                        }
                        else if (dongle_handle->source_info.usb_in.sample_format == HAL_AUDIO_PCM_FORMAT_S24_LE)
                        {
                            ShareBufferCopy_I_24bit_to_D_32bit_2ch((uint8_t *)(src_buf + sizeof(audio_transmitter_frame_header_t)),
                                                                    (uint32_t *)(stream->callback.EntryPara.in_ptr[0])+total_samples,
                                                                    (uint32_t *)(stream->callback.EntryPara.in_ptr[1])+total_samples,
                                                                    current_frame_samples);
                        }
                        else
                        {
                            DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR][handle 0x%x]sample format is not supported, %u, %u\r\n", 2, dongle_handle, dongle_handle->source_info.usb_in.sample_format);
                            AUDIO_ASSERT(0);
                        }
                        break;

                    case 8:
                        if (dongle_handle->source_info.usb_in.sample_format == HAL_AUDIO_PCM_FORMAT_S16_LE)
                        {
                            ShareBufferCopy_I_16bit_to_D_32bit_8ch((uint32_t *)(src_buf + sizeof(audio_transmitter_frame_header_t)),
                                                                    (uint32_t *)(stream->callback.EntryPara.in_ptr[0])+total_samples,
                                                                    (uint32_t *)(stream->callback.EntryPara.in_ptr[1])+total_samples,
                                                                    (uint32_t *)(stream->callback.EntryPara.in_ptr[2])+total_samples,
                                                                    (uint32_t *)(stream->callback.EntryPara.in_ptr[3])+total_samples,
                                                                    (uint32_t *)(stream->callback.EntryPara.in_ptr[4])+total_samples,
                                                                    (uint32_t *)(stream->callback.EntryPara.in_ptr[5])+total_samples,
                                                                    (uint32_t *)(stream->callback.EntryPara.in_ptr[6])+total_samples,
                                                                    (uint32_t *)(stream->callback.EntryPara.in_ptr[7])+total_samples,
                                                                    current_frame_samples);
                        }
                        else if (dongle_handle->source_info.usb_in.sample_format == HAL_AUDIO_PCM_FORMAT_S24_LE)
                        {
                            ShareBufferCopy_I_24bit_to_D_32bit_8ch((uint8_t *)(src_buf + sizeof(audio_transmitter_frame_header_t)),
                                                                    (uint32_t *)(stream->callback.EntryPara.in_ptr[0])+total_samples,
                                                                    (uint32_t *)(stream->callback.EntryPara.in_ptr[1])+total_samples,
                                                                    (uint32_t *)(stream->callback.EntryPara.in_ptr[2])+total_samples,
                                                                    (uint32_t *)(stream->callback.EntryPara.in_ptr[3])+total_samples,
                                                                    (uint32_t *)(stream->callback.EntryPara.in_ptr[4])+total_samples,
                                                                    (uint32_t *)(stream->callback.EntryPara.in_ptr[5])+total_samples,
                                                                    (uint32_t *)(stream->callback.EntryPara.in_ptr[6])+total_samples,
                                                                    (uint32_t *)(stream->callback.EntryPara.in_ptr[7])+total_samples,
                                                                    current_frame_samples);
                        }
                        else
                        {
                            DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR][handle 0x%x]sample format is not supported, %u, %u\r\n", 2, dongle_handle, dongle_handle->source_info.usb_in.sample_format);
                            AUDIO_ASSERT(0);
                        }
                        break;

                    default:
                        DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR][handle 0x%x]channel num is not supported, %u, %u\r\n", 2, dongle_handle, dongle_handle->source_info.usb_in.channel_num);
                        AUDIO_ASSERT(0);
                }
                /* 0.5ms case */
                if ((current_frame == 0) && ((dongle_handle->sink_info.bt_out.frame_interval%500)%2 != 0))
                {
                    /* offset back src_buf to the original position */
                    src_buf = src_buf - dongle_handle->source_info.usb_in.frame_size/2;
                }
                /* update total samples */
                total_samples += current_frame_samples;
                /* update copied frame number */
                current_frame += 1;
                /* change to the next frame */
                read_offset = (read_offset + source->streamBuffer.ShareBufferInfo.sub_info.block_info.block_size) % total_buffer_size;
                src_buf = (uint8_t *)(source->streamBuffer.ShareBufferInfo.start_addr + read_offset);
            }

            /* update stream data size */
            length = total_samples*sizeof(int32_t);
        }
    }
    else
    {
        /* get the unprocessed frame num and check if the uprocess frame is too much */
        unprocess_frames = length / (source->streamBuffer.ShareBufferInfo.sub_info.block_info.block_size-sizeof(audio_transmitter_frame_header_t));
        if (unprocess_frames > dongle_handle->source_info.usb_in.frame_max_num)
        {
            /* drop oldest data in the share buffer */
            source->streamBuffer.ShareBufferInfo.read_offset = (source->streamBuffer.ShareBufferInfo.write_offset+total_buffer_size-source->streamBuffer.ShareBufferInfo.sub_info.block_info.block_size*dongle_handle->source_info.usb_in.frame_max_num) % total_buffer_size;
            audio_transmitter_share_information_update_read_offset(source, source->streamBuffer.ShareBufferInfo.read_offset);
            audio_transmitter_share_information_fetch(source, NULL);
            DSP_MW_LOG_W("[ULL Audio V2][DL][WARNNING][handle 0x%x]unprocess frame is too much, %u, %u, %u, %u\r\n", 5, dongle_handle, unprocess_frames, dongle_handle->source_info.usb_in.frame_max_num, source->streamBuffer.ShareBufferInfo.read_offset, source->streamBuffer.ShareBufferInfo.write_offset);
            unprocess_frames = dongle_handle->source_info.usb_in.frame_max_num;
        }

        /* Copy all data from the share buffer */
        total_samples = 0;
        read_offset = source->streamBuffer.ShareBufferInfo.read_offset;
        src_buf = (uint8_t *)(source->streamBuffer.ShareBufferInfo.start_addr + read_offset);
        current_frame = 0;
        while(current_frame < unprocess_frames)
        {
            /* get current frame info and current frame samples */
            frame_header = (audio_transmitter_frame_header_t *)src_buf;
            if (frame_header->payload_len != dongle_handle->source_info.usb_in.frame_size)
            {
                DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR][handle 0x%x]frame size is not matched, %u, %u\r\n", 3, dongle_handle, frame_header->payload_len, dongle_handle->source_info.usb_in.frame_size);
                AUDIO_ASSERT(0);
            }
            current_frame_samples = dongle_handle->source_info.usb_in.frame_samples;
            /* copy usb audio data from share buffer one by one frame */
            switch (dongle_handle->source_info.usb_in.channel_num)
            {
                case 2:
                    if (dongle_handle->source_info.usb_in.sample_format == HAL_AUDIO_PCM_FORMAT_S16_LE)
                    {
                        ShareBufferCopy_I_16bit_to_D_32bit_2ch((uint32_t *)(src_buf + sizeof(audio_transmitter_frame_header_t)),
                                                                (uint32_t *)(stream->callback.EntryPara.in_ptr[0])+total_samples,
                                                                (uint32_t *)(stream->callback.EntryPara.in_ptr[1])+total_samples,
                                                                current_frame_samples);
                    }
                    else if (dongle_handle->source_info.usb_in.sample_format == HAL_AUDIO_PCM_FORMAT_S24_LE)
                    {
                        ShareBufferCopy_I_24bit_to_D_32bit_2ch((uint8_t *)(src_buf + sizeof(audio_transmitter_frame_header_t)),
                                                                (uint32_t *)(stream->callback.EntryPara.in_ptr[0])+total_samples,
                                                                (uint32_t *)(stream->callback.EntryPara.in_ptr[1])+total_samples,
                                                                current_frame_samples);
                    }
                    else
                    {
                        DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR][handle 0x%x]sample format is not supported, %u, %u\r\n", 2, dongle_handle, dongle_handle->source_info.usb_in.sample_format);
                        AUDIO_ASSERT(0);
                    }
                    break;

                case 8:
                    if (dongle_handle->source_info.usb_in.sample_format == HAL_AUDIO_PCM_FORMAT_S16_LE)
                    {
                        ShareBufferCopy_I_16bit_to_D_32bit_8ch((uint32_t *)(src_buf + sizeof(audio_transmitter_frame_header_t)),
                                                                (uint32_t *)(stream->callback.EntryPara.in_ptr[0])+total_samples,
                                                                (uint32_t *)(stream->callback.EntryPara.in_ptr[1])+total_samples,
                                                                (uint32_t *)(stream->callback.EntryPara.in_ptr[2])+total_samples,
                                                                (uint32_t *)(stream->callback.EntryPara.in_ptr[3])+total_samples,
                                                                (uint32_t *)(stream->callback.EntryPara.in_ptr[4])+total_samples,
                                                                (uint32_t *)(stream->callback.EntryPara.in_ptr[5])+total_samples,
                                                                (uint32_t *)(stream->callback.EntryPara.in_ptr[6])+total_samples,
                                                                (uint32_t *)(stream->callback.EntryPara.in_ptr[7])+total_samples,
                                                                current_frame_samples);
                    }
                    else if (dongle_handle->source_info.usb_in.sample_format == HAL_AUDIO_PCM_FORMAT_S24_LE)
                    {
                        ShareBufferCopy_I_24bit_to_D_32bit_8ch((uint8_t *)(src_buf + sizeof(audio_transmitter_frame_header_t)),
                                                                (uint32_t *)(stream->callback.EntryPara.in_ptr[0])+total_samples,
                                                                (uint32_t *)(stream->callback.EntryPara.in_ptr[1])+total_samples,
                                                                (uint32_t *)(stream->callback.EntryPara.in_ptr[2])+total_samples,
                                                                (uint32_t *)(stream->callback.EntryPara.in_ptr[3])+total_samples,
                                                                (uint32_t *)(stream->callback.EntryPara.in_ptr[4])+total_samples,
                                                                (uint32_t *)(stream->callback.EntryPara.in_ptr[5])+total_samples,
                                                                (uint32_t *)(stream->callback.EntryPara.in_ptr[6])+total_samples,
                                                                (uint32_t *)(stream->callback.EntryPara.in_ptr[7])+total_samples,
                                                                current_frame_samples);
                    }
                    else
                    {
                        DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR][handle 0x%x]sample format is not supported, %u, %u\r\n", 2, dongle_handle, dongle_handle->source_info.usb_in.sample_format);
                        AUDIO_ASSERT(0);
                    }
                    break;

                default:
                    DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR][handle 0x%x]channel num is not supported, %u, %u\r\n", 2, dongle_handle, dongle_handle->source_info.usb_in.channel_num);
                    AUDIO_ASSERT(0);
            }

            /* update total samples */
            total_samples += current_frame_samples;
            /* update copied frame number */
            current_frame += 1;
            /* change to the next frame */
            read_offset = (read_offset + source->streamBuffer.ShareBufferInfo.sub_info.block_info.block_size) % total_buffer_size;
            src_buf = (uint8_t *)(source->streamBuffer.ShareBufferInfo.start_addr + read_offset);
        }
        /* update stream data size */
        length = total_samples*sizeof(int32_t);
    }

    // LOG_AUDIO_DUMP((uint8_t *)(stream->callback.EntryPara.in_ptr[0]), length, SOURCE_IN5);

    /* update stream status */
    stream->callback.EntryPara.in_size                  = length;
    stream->callback.EntryPara.in_channel_num           = dongle_handle->source_info.usb_in.channel_num;
    stream->callback.EntryPara.in_sampling_rate         = dongle_handle->source_info.usb_in.sample_rate/1000;
    stream->callback.EntryPara.resolution.source_in_res = RESOLUTION_32BIT;
    stream->callback.EntryPara.resolution.feature_res   = RESOLUTION_32BIT;
    stream->callback.EntryPara.resolution.sink_out_res  = RESOLUTION_32BIT;
    stream->callback.EntryPara.resolution.process_res   = RESOLUTION_32BIT;

    /* update state machine */
    dongle_handle->process_frames = unprocess_frames;

    /* configure clock skew settings */
    dongle_handle->compen_samples = ull_audio_v2_dongle_dl_usb_clock_skew_check(dongle_handle, dongle_handle->process_frames*dongle_handle->source_info.usb_in.frame_samples, &dongle_handle->buffer0_output_size);
    stream_function_sw_clk_skew_configure_compensation_samples(dongle_handle->clk_skew_port, dongle_handle->compen_samples);

    /* configure buffer output size, this setting should be fixed */
    stream_function_sw_buffer_config_channel_output_size(dongle_handle->buffer_port_0, 0, dongle_handle->buffer0_output_size);

    /* avoid payload length check error in here */
    length = (length > source->param.data_dl.max_payload_size) ? source->param.data_dl.max_payload_size : length;

    /* update timestamp for debug */
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, (uint32_t *)&dongle_handle->data_in_gpt_count);

#if ULL_AUDIO_V2_DONGLE_DL_PATH_DEBUG_LOG
    uint32_t current_timestamp = 0;
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &current_timestamp);
    DSP_MW_LOG_I("[ULL Audio V2][DL][DEBUG][copy_payload]: %d, %d, %u, %d, %d", 5, unprocess_frames, total_samples, length, current_timestamp, hal_nvic_query_exception_number());
#endif /* ULL_AUDIO_V2_DONGLE_DL_PATH_DEBUG_LOG */

    return length;
}

ATTR_TEXT_IN_IRAM uint32_t ull_audio_v2_dongle_dl_source_get_new_read_offset(SOURCE source, uint32_t amount)
{
    UNUSED(amount);
    ull_audio_v2_dongle_dl_handle_t *dongle_handle = (ull_audio_v2_dongle_dl_handle_t *)source->transform->sink->param.bt_common.scenario_param.dongle_handle;
    uint32_t total_buffer_size;
    n9_dsp_share_info_ptr ShareBufferInfo;
    uint32_t read_offset;

    /* get new read offset */
    ShareBufferInfo = &source->streamBuffer.ShareBufferInfo;
    total_buffer_size = ShareBufferInfo->sub_info.block_info.block_size * ShareBufferInfo->sub_info.block_info.block_num;
    read_offset = (ShareBufferInfo->read_offset+ShareBufferInfo->sub_info.block_info.block_size*dongle_handle->process_frames) % total_buffer_size;

    return read_offset;
}

void ull_audio_v2_dongle_dl_source_drop_postprocess(SOURCE source, uint32_t amount)
{
    ull_audio_v2_dongle_dl_handle_t *dongle_handle = (ull_audio_v2_dongle_dl_handle_t *)source->transform->sink->param.bt_common.scenario_param.dongle_handle;
    uint32_t saved_mask;
    UNUSED(amount);

    /* update first packet state machine */
    if ((dongle_handle->first_packet_status == ULL_AUDIO_V2_DONGLE_DL_FIRST_PACKET_NOT_READY) && (dongle_handle->process_frames != 0))
    {
        dongle_handle->first_packet_status = ULL_AUDIO_V2_DONGLE_DL_FIRST_PACKET_READY;
    }

    /* update stream status */
    if (dongle_handle->stream_status == ULL_AUDIO_V2_DONGLE_STREAM_START)
    {
        dongle_handle->stream_status = ULL_AUDIO_V2_DONGLE_STREAM_RUNNING;
    }

    /* decrease fetch count */
    hal_nvic_save_and_set_interrupt_mask(&saved_mask);
    if (dongle_handle->fetch_count == 0)
    {
        DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR][handle 0x%x]fetch count is 0\r\n", 1, dongle_handle);
        AUDIO_ASSERT(0);
    }
    dongle_handle->fetch_count -= 1;
    hal_nvic_restore_interrupt_mask(saved_mask);
}

bool ull_audio_v2_dongle_dl_source_close(SOURCE source)
{
    UNUSED(source);

    return true;
}

ATTR_TEXT_IN_IRAM bool ull_audio_v2_dongle_dl_sink_get_avail_size(SINK sink, uint32_t *avail_size)
{
    ull_audio_v2_dongle_dl_handle_t *dongle_handle = (ull_audio_v2_dongle_dl_handle_t *)sink->param.bt_common.scenario_param.dongle_handle;

    /* change avail_size to actual frame size */
    *avail_size = dongle_handle->sink_info.bt_out.frame_size;

    return true;
}

ATTR_TEXT_IN_IRAM uint32_t ull_audio_v2_dongle_dl_sink_copy_payload(SINK sink, uint8_t *src_buf, uint32_t length)
{
    ull_audio_v2_dongle_dl_handle_t *dongle_handle = (ull_audio_v2_dongle_dl_handle_t *)sink->param.bt_common.scenario_param.dongle_handle;
    DSP_STREAMING_PARA_PTR stream = DSP_Streaming_Get(sink->transform->source, sink);
    uint32_t bt_clk;
    uint16_t bt_phase;
    uint32_t i, j;
    uint32_t *src_addr;
    uint32_t *des_addr;
    uint32_t blk_size;
    uint32_t blk_num;
    uint32_t blk_index;
    n9_dsp_share_info_ptr p_share_info;
    ULL_AUDIO_V2_HEADER *p_ull_audio_header;
    bool crc32_flag = false;
    uint32_t crc32_init[ULL_AUDIO_V2_DATA_CHANNEL_NUMBER];

    /* check parameter */
    if (length != dongle_handle->sink_info.bt_out.frame_size)
    {
        DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR][handle 0x%x]length is not right, %d, %d\r\n", 3, dongle_handle, length, dongle_handle->sink_info.bt_out.frame_size);
        AUDIO_ASSERT(0);
    }

    /* get current bt clock */
    MCE_GetBtClk((BTCLK*)&bt_clk, (BTPHASE*)&bt_phase, BT_CLK_Offset);

    /* write DL packet to different share buffer one by one */
    for (i = 0; i < ULL_AUDIO_V2_DATA_CHANNEL_NUMBER; i++)
    {
        if (dongle_handle->sink_info.bt_out.bt_info[i] == NULL)
        {
            continue;
        }
        else
        {
            src_addr     = stream_function_get_inout_buffer((void *)(&(stream->callback.EntryPara)), (i+1));
            src_buf      = (uint8_t *)src_addr;
            p_share_info = (n9_dsp_share_info_ptr)((dongle_handle->sink_info.bt_out.bt_info[i])->bt_link_info.share_info);
            blk_size     = p_share_info->sub_info.block_info.block_size;
            blk_num      = p_share_info->sub_info.block_info.block_num;
            blk_index    = dongle_handle->ccni_blk_index;
            crc32_init[i]= p_share_info->asi_cur;
            if (blk_index < blk_num)
            {
                /* get header address and data address */
                des_addr = (uint32_t *)(hal_memview_infrasys_to_dsp0(p_share_info->start_addr) + blk_size * blk_index);
                p_ull_audio_header = (ULL_AUDIO_V2_HEADER *)des_addr;
                des_addr = (uint32_t *)((uint32_t)des_addr + sizeof(ULL_AUDIO_V2_HEADER));

                /* check if blk size is enough */
                if ((dongle_handle->sink_info.bt_out.frame_size+sizeof(ULL_AUDIO_V2_HEADER)) > blk_size)
                {
                    DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR][handle 0x%x]blk size is not right, %d, %d\r\n", 3, dongle_handle, blk_size, dongle_handle->sink_info.bt_out.frame_size+sizeof(ULL_AUDIO_V2_HEADER));
                    AUDIO_ASSERT(0);
                }

                /* write DL packet data into share buffer block */
                for (j = 0; j < (((uint32_t)(dongle_handle->sink_info.bt_out.frame_size+3))/4); j++)
                {
                    /* share buffer block must be 4B aligned, so we can use word access to get better performance */
                    *des_addr++ = *src_addr++;
                }

                /* write DL packet header into share buffer block */
                p_ull_audio_header->DataOffset = sizeof(ULL_AUDIO_V2_HEADER);
                p_ull_audio_header->PduLen     = dongle_handle->sink_info.bt_out.frame_size;
                p_ull_audio_header->TimeStamp  = bt_clk;
                p_ull_audio_header->SampleSeq  = (dongle_handle->sink_info.bt_out.bt_info[i])->seq_num;
                /* check if crc32_value is changed before do CRC generate */
                if ((dongle_handle->sink_info.bt_out.bt_info[i])->crc32_init != crc32_init[i])
                {
                    crc32_flag = true;
                }
                p_ull_audio_header->crc32_value= CRC32_Generate((uint8_t *)src_buf, dongle_handle->sink_info.bt_out.frame_size, crc32_init[i]);

                /* update state machine */
                (dongle_handle->sink_info.bt_out.bt_info[i])->seq_num += 1;
                (dongle_handle->sink_info.bt_out.bt_info[i])->blk_index_previous = (dongle_handle->sink_info.bt_out.bt_info[i])->blk_index;
                (dongle_handle->sink_info.bt_out.bt_info[i])->blk_index = blk_index;
            }
            else
            {
                DSP_MW_LOG_E("[ULL Audio V2][DL][ERROR][handle 0x%x]channel%d blk index is not right, %d, %d\r\n", 4, dongle_handle, i+1, blk_index, blk_num);
            }
        }
    }

    /* get current bt clock */
    MCE_GetBtClk((BTCLK*)&bt_clk, (BTPHASE*)&bt_phase, BT_CLK_Offset);

    /* Output crc32_init value changed log for debug */
    if (crc32_flag)
    {
        for (i = 0; i < ULL_AUDIO_V2_DATA_CHANNEL_NUMBER; i++)
        {
            if (dongle_handle->sink_info.bt_out.bt_info[i] != NULL)
            {
                DSP_MW_LOG_W("[ULL Audio V2][DL][handle 0x%x]bt out channel %u crc32_init value is changed from 0x%x to 0x%x\r\n", 11,
                            dongle_handle,
                            i+1,
                            (dongle_handle->sink_info.bt_out.bt_info[i])->crc32_init,
                            crc32_init[i]);
                (dongle_handle->sink_info.bt_out.bt_info[i])->crc32_init = crc32_init[i];
            }
        }
    }

    /* update time stamp */
    dongle_handle->data_out_bt_count = bt_clk;

    return length;
}

ATTR_TEXT_IN_IRAM bool ull_audio_v2_dongle_dl_sink_get_new_write_offset(SINK sink, U32 amount, uint32_t *new_write_offset)
{
    ull_audio_v2_dongle_dl_handle_t *dongle_handle = (ull_audio_v2_dongle_dl_handle_t *)sink->param.bt_common.scenario_param.dongle_handle;
    uint32_t i, write_offset;
    n9_dsp_share_info_ptr p_share_info;
    uint32_t blk_size;
    uint32_t blk_num;
    uint32_t blk_index;
    UNUSED(amount);
    UNUSED(new_write_offset);

    /* update write index */
    StreamDSP_HWSemaphoreTake();
    for (i = 0; i < ULL_AUDIO_V2_DATA_CHANNEL_NUMBER; i++)
    {
        if (dongle_handle->sink_info.bt_out.bt_info[i] == NULL)
        {
            continue;
        }
        else
        {
            p_share_info = (n9_dsp_share_info_ptr)((dongle_handle->sink_info.bt_out.bt_info[i])->bt_link_info.share_info);
            blk_size     = p_share_info->sub_info.block_info.block_size;
            blk_num      = p_share_info->sub_info.block_info.block_num;
            blk_index    = dongle_handle->ccni_blk_index;
            if (blk_index < blk_num)
            {
                write_offset = (uint32_t)(((blk_index+1) % blk_num) * blk_size);
                p_share_info->write_offset = write_offset;
            }
        }
    }
    StreamDSP_HWSemaphoreGive();

    /* we will update the write offsets of the different share buffers in here directly, so return false to aviod the upper layer update write offset */
    return false;
}

ATTR_TEXT_IN_IRAM bool ull_audio_v2_dongle_dl_sink_query_notification(SINK sink, bool *notification_flag)
{
    UNUSED(sink);

    *notification_flag = false;

    return true;
}

ATTR_TEXT_IN_IRAM bool ull_audio_v2_dongle_dl_sink_send_data_ready_notification(SINK sink)
{
    UNUSED(sink);

    return true;
}

ATTR_TEXT_IN_IRAM void ull_audio_v2_dongle_dl_sink_flush_postprocess(SINK sink, uint32_t amount)
{
    ull_audio_v2_dongle_dl_handle_t *dongle_handle = (ull_audio_v2_dongle_dl_handle_t *)sink->param.bt_common.scenario_param.dongle_handle;
    ull_audio_v2_dongle_dl_handle_t *c_handle;
    uint32_t i;
    uint32_t remain_samples_0;
    int32_t frac_rpt;
    UNUSED(sink);
    UNUSED(amount);

    /* update time stamp */
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, (uint32_t *)&dongle_handle->data_out_gpt_count);

    /* audio dump */
    #ifdef AIR_AUDIO_DUMP_ENABLE
    #if ULL_AUDIO_V2_DONGLE_DL_PATH_DEBUG_DUMP
    uint8_t *codec_in_data_address;
    uint32_t codec_in_data_frame_size;
    uint32_t blk_size;
    uint32_t blk_num;
    uint32_t blk_index;
    n9_dsp_share_info_ptr p_share_info;
    uint8_t *codec_out_data_address;
    uint32_t codec_out_data_frame_size;
    if (dongle_handle->sink_info.bt_out.codec_type == AUDIO_DSP_CODEC_TYPE_LC3PLUS)
    {
        /* dump channel 1 LC3+ codec in data */
        stream_codec_encoder_lc3plus_get_data_info(LC3PLUS_ENC_PORT_0, 1, &codec_in_data_address, &codec_in_data_frame_size);
        LOG_AUDIO_DUMP((uint8_t *)codec_in_data_address, codec_in_data_frame_size, AUDIO_SOURCE_IN_L);
        /* dump channel 2 LC3+ codec in data */
        stream_codec_encoder_lc3plus_get_data_info(LC3PLUS_ENC_PORT_0, 2, &codec_in_data_address, &codec_in_data_frame_size);
        LOG_AUDIO_DUMP((uint8_t *)codec_in_data_address, codec_in_data_frame_size, AUDIO_SOURCE_IN_R);

        /* codec out dump */
        for (i = 0; i < ULL_AUDIO_V2_DATA_CHANNEL_NUMBER; i++)
        {
            if (dongle_handle->sink_info.bt_out.bt_info[i] == NULL)
            {
                continue;
            }
            else
            {
                p_share_info = (n9_dsp_share_info_ptr)((dongle_handle->sink_info.bt_out.bt_info[i])->bt_link_info.share_info);
                blk_size     = p_share_info->sub_info.block_info.block_size;
                blk_num      = p_share_info->sub_info.block_info.block_num;
                blk_index    = dongle_handle->ccni_blk_index;
                if (blk_index < blk_num)
                {
                    codec_out_data_address = (uint8_t *)(hal_memview_infrasys_to_dsp0(p_share_info->start_addr) + blk_size * blk_index);
                    codec_out_data_frame_size = dongle_handle->sink_info.bt_out.frame_size + sizeof(ULL_AUDIO_V2_HEADER);
                    LOG_AUDIO_DUMP((uint8_t *)codec_out_data_address, codec_out_data_frame_size, SINK_OUT1+i);
                }
            }
        }
    }
    #endif /* ULL_AUDIO_V2_DONGLE_DL_PATH_DEBUG_DUMP */
    #endif /* AIR_AUDIO_DUMP_ENABLE */

    /* output debug log */
    c_handle = ull_audio_v2_dongle_first_dl_handle;
    for (i = 0; i < (uint32_t)ull_audio_v2_dongle_first_dl_handle->total_number; i++)
    {
        sink = (SINK)(c_handle->owner);

        if (sink->transform != NULL)
        {
            remain_samples_0 = stream_function_sw_buffer_get_channel_used_size(c_handle->buffer_port_0, 1) / sizeof(int32_t);
            stream_function_sw_clk_skew_get_frac_rpt(c_handle->clk_skew_port, 1, &frac_rpt);
            DSP_MW_LOG_I("[ULL Audio V2][DL][handle 0x%x] %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, 0x%x, 0x%x, 0x%x, 0x%x", 16,
                            c_handle,
                            c_handle->mixer_status,
                            c_handle->first_packet_status,
                            c_handle->data_status,
                            c_handle->ccni_blk_index,
                            c_handle->fetch_count,
                            c_handle->process_frames,
                            c_handle->buffer0_output_size,
                            remain_samples_0,
                            c_handle->clk_skew_count,
                            frac_rpt,
                            c_handle->compen_samples,
                            c_handle->ccni_in_bt_count,
                            c_handle->data_out_bt_count,
                            c_handle->ccni_in_gpt_count,
                            c_handle->data_out_gpt_count);

            /* do usb data audio dump */
            #ifdef AIR_AUDIO_DUMP_ENABLE
            #if ULL_AUDIO_V2_DONGLE_DL_PATH_DEBUG_DUMP
            uint32_t j, address, read_offset, total_buffer_size;
            total_buffer_size = sink->transform->source->streamBuffer.ShareBufferInfo.sub_info.block_info.block_size * sink->transform->source->streamBuffer.ShareBufferInfo.sub_info.block_info.block_num;
            for (j = 0; j < c_handle->process_frames; j++) {
                read_offset = (sink->transform->source->streamBuffer.ShareBufferInfo.read_offset + sink->transform->source->streamBuffer.ShareBufferInfo.sub_info.block_info.block_size * j) % total_buffer_size;
                address = sink->transform->source->streamBuffer.ShareBufferInfo.start_addr + read_offset + sizeof(audio_transmitter_frame_header_t);
                LOG_AUDIO_DUMP((uint8_t *)address, sink->transform->source->streamBuffer.ShareBufferInfo.sub_info.block_info.block_size - sizeof(audio_transmitter_frame_header_t), SOURCE_IN2 + (c_handle->sub_id - AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0));
            }
            #endif /* ULL_AUDIO_V2_DONGLE_DL_PATH_DEBUG_DUMP */
            #endif /* AIR_AUDIO_DUMP_ENABLE */
        }

        /* switch to the next dl stream */
        c_handle = c_handle->next_dl_handle;
    }
}

bool ull_audio_v2_dongle_dl_sink_close(SINK sink)
{
    UNUSED(sink);

    return true;
}

/******************************************************************************/
/*               ULL audio 2.0 dongle UL path Public Functions                */
/******************************************************************************/
uint32_t ull_audio_v2_dongle_ul_get_stream_in_max_size_each_channel(SOURCE source, SINK sink)
{
    ull_audio_v2_dongle_ul_handle_t *dongle_handle = (ull_audio_v2_dongle_ul_handle_t *)source->param.bt_common.scenario_param.dongle_handle;
    uint32_t stream_in_size = 0;
    UNUSED(source);
    UNUSED(sink);

    switch (dongle_handle->sub_id)
    {
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0:
            stream_in_size = 2*((dongle_handle->source_info.bt_in.frame_size+sizeof(lc3plus_dec_frame_status_t)+3)/4*4);
            break;

        default:
            AUDIO_ASSERT(0);
            break;
    }

    return stream_in_size;
}

uint32_t ull_audio_v2_dongle_ul_get_stream_in_channel_number(SOURCE source, SINK sink)
{
    ull_audio_v2_dongle_ul_handle_t *dongle_handle = (ull_audio_v2_dongle_ul_handle_t *)source->param.bt_common.scenario_param.dongle_handle;
    uint32_t channel_number = 0;
    UNUSED(source);
    UNUSED(sink);

    switch (dongle_handle->sub_id)
    {
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0:
            channel_number = dongle_handle->source_info.bt_in.channel_num;
            break;

        default:
            AUDIO_ASSERT(0);
            break;
    }

    return channel_number;
}

stream_samplerate_t ull_audio_v2_dongle_ul_get_stream_in_sampling_rate_each_channel(SOURCE source, SINK sink)
{
    ull_audio_v2_dongle_ul_handle_t *dongle_handle = (ull_audio_v2_dongle_ul_handle_t *)source->param.bt_common.scenario_param.dongle_handle;
    stream_samplerate_t samplerate = 0;
    UNUSED(source);
    UNUSED(sink);

    switch (dongle_handle->sub_id)
    {
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0:
            samplerate = dongle_handle->source_info.bt_in.sample_rate/1000;
            break;

        default:
            AUDIO_ASSERT(0);
            break;
    }

    return samplerate;
}

uint32_t ull_audio_v2_dongle_ul_get_stream_out_max_size_each_channel(SOURCE source, SINK sink)
{
    ull_audio_v2_dongle_ul_handle_t *dongle_handle = (ull_audio_v2_dongle_ul_handle_t *)source->param.bt_common.scenario_param.dongle_handle;
    uint32_t stream_out_size = 0;
    uint32_t frame_samples;
    uint32_t frame_sample_size;
    UNUSED(source);
    UNUSED(sink);

    switch (dongle_handle->sub_id)
    {
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0:
            frame_samples = max(dongle_handle->sink_info.usb_out.frame_samples, dongle_handle->source_info.bt_in.frame_samples*1000/dongle_handle->source_info.bt_in.frame_interval);
            if ((dongle_handle->sink_info.usb_out.sample_format == HAL_AUDIO_PCM_FORMAT_S24_LE) || (dongle_handle->source_info.bt_in.sample_format == HAL_AUDIO_PCM_FORMAT_S24_LE))
            {
                frame_sample_size = sizeof(int32_t);
            }
            else
            {
                frame_sample_size = sizeof(int16_t);
            }
            stream_out_size = 2*(dongle_handle->sink_info.usb_out.frame_max_num*frame_samples*frame_sample_size);
            break;

        default:
            AUDIO_ASSERT(0);
            break;
    }

    return stream_out_size;
}

uint32_t ull_audio_v2_dongle_ul_get_stream_out_channel_number(SOURCE source, SINK sink)
{
    ull_audio_v2_dongle_ul_handle_t *dongle_handle = (ull_audio_v2_dongle_ul_handle_t *)source->param.bt_common.scenario_param.dongle_handle;
    uint32_t channel_number = 0;
    UNUSED(source);
    UNUSED(sink);

    switch (dongle_handle->sub_id)
    {
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0:
            channel_number = max(dongle_handle->sink_info.usb_out.channel_num, dongle_handle->source_info.bt_in.channel_num);
            break;

        default:
            AUDIO_ASSERT(0);
            break;
    }

    return channel_number;
}

stream_samplerate_t ull_audio_v2_dongle_ul_get_stream_out_sampling_rate_each_channel(SOURCE source, SINK sink)
{
    ull_audio_v2_dongle_ul_handle_t *dongle_handle = (ull_audio_v2_dongle_ul_handle_t *)source->param.bt_common.scenario_param.dongle_handle;
    stream_samplerate_t samplerate = 0;
    UNUSED(source);
    UNUSED(sink);

    switch (dongle_handle->sub_id)
    {
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0:
            samplerate = dongle_handle->sink_info.usb_out.sample_rate/1000;
            break;

        default:
            AUDIO_ASSERT(0);
            break;
    }

    return samplerate;
}

void ull_audio_v2_dongle_ul_init(SOURCE source, SINK sink, audio_transmitter_open_param_p audio_transmitter_open_param, bt_common_open_param_p bt_common_open_param)
{
    /* get handle for application config */
    ull_audio_v2_dongle_ul_handle_t *dongle_handle = ull_audio_v2_dongle_ul_get_handle(source);
    source->param.bt_common.scenario_param.dongle_handle = (void *)dongle_handle;
    dongle_handle->source = source;
    dongle_handle->sink   = sink;
    dongle_handle->sub_id = audio_transmitter_open_param->scenario_sub_id;

    /* task config */
    source->taskId = DAV_TASK_ID;
    sink->taskid = DAV_TASK_ID;

    /* init audio info */
    switch (dongle_handle->sub_id)
    {
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0:
            ull_audio_v2_dongle_ul_usb_out_init(dongle_handle, audio_transmitter_open_param, bt_common_open_param);
            break;

        default:
            AUDIO_ASSERT(0);
            break;
    }

    /* lock sleep because sleep wake-up time will consume the stream process time */
    //hal_sleep_manager_lock_sleep(SLEEP_LOCK_AUDIO);

    /* register ccni handler */
    bt_common_register_isr_handler(ull_audio_v2_dongle_ul_ccni_handler);

    /* stream status update */
    dongle_handle->stream_status = ULL_AUDIO_V2_DONGLE_STREAM_INIT;
}

void ull_audio_v2_dongle_ul_deinit(SOURCE source, SINK sink)
{
    ull_audio_v2_dongle_ul_handle_t *dongle_handle = (ull_audio_v2_dongle_ul_handle_t *)source->param.bt_common.scenario_param.dongle_handle;
    uint32_t saved_mask;
    UNUSED(sink);

    /* unregister ccni handler */
    hal_nvic_save_and_set_interrupt_mask(&saved_mask);
    if (dongle_handle->total_number == 1)
    {
        bt_common_unregister_isr_handler(ull_audio_v2_dongle_ul_ccni_handler);
    }
    hal_nvic_restore_interrupt_mask(saved_mask);

    /* deinit audio info */
    switch (dongle_handle->sub_id)
    {
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0:
            ull_audio_v2_dongle_ul_usb_out_deinit(dongle_handle);
            break;

        default:
            AUDIO_ASSERT(0);
            break;
    }

    /* unlock sleep */
    //hal_sleep_manager_unlock_sleep(SLEEP_LOCK_AUDIO);

    /* stream status update */
    dongle_handle->stream_status = ULL_AUDIO_V2_DONGLE_STREAM_DEINIT;

    /* release handle */
    ull_audio_v2_dongle_ul_release_handle(dongle_handle);
    source->param.bt_common.scenario_param.dongle_handle = NULL;
}

void ull_audio_v2_dongle_ul_start(SOURCE source, SINK sink)
{
    ull_audio_v2_dongle_ul_handle_t *dongle_handle = (ull_audio_v2_dongle_ul_handle_t *)source->param.bt_common.scenario_param.dongle_handle;
    UNUSED(sink);

    switch (dongle_handle->sub_id)
    {
        case AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0:
            dongle_handle->fetch_count = 0;
            dongle_handle->first_packet_status = ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_NOT_READY;
            break;

        default:
            AUDIO_ASSERT(0);
            break;
    }

    /* stream status update */
    dongle_handle->stream_status = ULL_AUDIO_V2_DONGLE_STREAM_START;
}

bool ull_audio_v2_dongle_ul_config(SOURCE source, stream_config_type type, U32 value)
{
    bool ret = true;
    ull_audio_v2_dongle_runtime_config_param_p config_param = (ull_audio_v2_dongle_runtime_config_param_p)value;
    ull_audio_v2_dongle_ul_handle_t *dongle_handle = (ull_audio_v2_dongle_ul_handle_t *)source->param.bt_common.scenario_param.dongle_handle;
    uint32_t vol_ch;
    int32_t vol_gain;
    sw_gain_config_t old_config;
    UNUSED(source);
    UNUSED(type);
    UNUSED(value);

    switch (config_param->config_operation)
    {
        case ULL_AUDIO_V2_DONGLE_CONFIG_OP_SET_UL_VOL_INFO:
            vol_ch = config_param->config_param.vol_gain.vol_ch;
            vol_gain = config_param->config_param.vol_gain.vol_gain;
            if (vol_ch != 0)
            {
                stream_function_sw_gain_get_config(dongle_handle->gain_port, vol_ch, &old_config);
                DSP_MW_LOG_I("[ULL Audio V2][UL][config][handle 0x%x]scenario %d-%d change channel %d gain from %d*0.01dB to %d*0.01dB\r\n", 6,
                            dongle_handle,
                            source->param.bt_common.scenario_type,
                            source->param.bt_common.scenario_sub_id,
                            vol_ch,
                            old_config.target_gain,
                            vol_gain);
                stream_function_sw_gain_configure_gain_target(dongle_handle->gain_port, vol_ch, vol_gain);
            }
            else
            {
                vol_ch = 1;
                stream_function_sw_gain_get_config(dongle_handle->gain_port, vol_ch, &old_config);
                DSP_MW_LOG_I("[ULL Audio V2][UL][config][handle 0x%x]scenario %d-%d change channel %d gain from %d*0.01dB to %d*0.01dB\r\n", 6,
                            dongle_handle,
                            source->param.bt_common.scenario_type,
                            source->param.bt_common.scenario_sub_id,
                            vol_ch,
                            old_config.target_gain,
                            vol_gain);
                stream_function_sw_gain_configure_gain_target(dongle_handle->gain_port, vol_ch, vol_gain);
                vol_ch = 2;
                stream_function_sw_gain_get_config(dongle_handle->gain_port, vol_ch, &old_config);
                DSP_MW_LOG_I("[ULL Audio V2][UL][config][handle 0x%x]scenario %d-%d change channel %d gain from %d*0.01dB to %d*0.01dB\r\n", 6,
                            dongle_handle,
                            source->param.bt_common.scenario_type,
                            source->param.bt_common.scenario_sub_id,
                            vol_ch,
                            old_config.target_gain,
                            vol_gain);
                stream_function_sw_gain_configure_gain_target(dongle_handle->gain_port, vol_ch, vol_gain);
            }
            break;

        case ULL_AUDIO_V2_DONGLE_CONFIG_OP_SET_UL_CH_INPUT_SOURCE:
            break;

        default:
            DSP_MW_LOG_E("[ULL Audio V2][UL][config] unknow cmd:%d\r\n", 1, config_param->config_operation);
            break;
    }

    return ret;
}

ATTR_TEXT_IN_IRAM bool ull_audio_v2_dongle_ul_source_get_avail_size(SOURCE source, uint32_t *avail_size)
{
    ull_audio_v2_dongle_ul_handle_t *dongle_handle = (ull_audio_v2_dongle_ul_handle_t *)source->param.bt_common.scenario_param.dongle_handle;
    uint32_t bt_clk;
    uint16_t bt_phase;

    /* get current bt clk */
    MCE_GetBtClk((BTCLK*)&bt_clk, (BTPHASE*)&bt_phase, BT_CLK_Offset);

    if (dongle_handle->fetch_count != 0)
    {
        /* If there is fetch requirement, we must wake up the stream even there is no packet on the share buffer */
        *avail_size = dongle_handle->source_info.bt_in.frame_size;
    }
    else if (ull_audio_v2_dongle_ul_fetch_time_is_arrived(dongle_handle, bt_clk))
    {
        /* If there is data in the share buffer, we must wake up the stream to process it */
        *avail_size = dongle_handle->source_info.bt_in.frame_size;
    }
    else
    {
        *avail_size = 0;
    }

    return true;
}

uint32_t ull_audio_v2_dongle_ul_source_copy_payload(SOURCE source, uint8_t *dst_buf, uint32_t length)
{
    ull_audio_v2_dongle_ul_handle_t *dongle_handle = (ull_audio_v2_dongle_ul_handle_t *)source->param.bt_common.scenario_param.dongle_handle;
    DSP_STREAMING_PARA_PTR stream = DSP_Streaming_Get(source, source->transform->sink);
    uint32_t saved_mask;
    ull_audio_v2_dongle_first_packet_status_t first_packet_status;
    uint32_t bt_clk;
    uint16_t bt_phase;
    uint32_t remain_samples;
    uint32_t i;
    n9_dsp_share_info_ptr p_share_info;
    uint32_t blk_size;
    uint32_t blk_num;
    uint32_t blk_index;
    uint32_t blk_index_previous;
    uint16_t read_offset;
    ULL_AUDIO_V2_HEADER *p_ull_audio_header;
    uint8_t *src_buf;
    int32_t sample_size = 0;

    /* dongle status check */
    switch (dongle_handle->stream_status)
    {
        /* In this status stage, it means the stream is not ready */
        case ULL_AUDIO_V2_DONGLE_STREAM_DEINIT:
        case ULL_AUDIO_V2_DONGLE_STREAM_INIT:
            hal_nvic_save_and_set_interrupt_mask(&saved_mask);
            dongle_handle->fetch_count = 0;
            hal_nvic_restore_interrupt_mask(saved_mask);
            break;

        /* In this status stage, we will check if the newest packet is suitable for playing */
        case ULL_AUDIO_V2_DONGLE_STREAM_START:
            /* get current bt clock */
            MCE_GetBtClk((BTCLK*)&bt_clk, (BTPHASE*)&bt_phase, BT_CLK_Offset);
            first_packet_status = ull_audio_v2_dongle_ul_first_packet_check(dongle_handle, bt_clk);
            switch (first_packet_status)
            {
                case ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_NOT_READY:
                case ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_TIMEOUT:
                    /* reset fetch count */
                    hal_nvic_save_and_set_interrupt_mask(&saved_mask);
                    dongle_handle->fetch_count = 0;
                    hal_nvic_restore_interrupt_mask(saved_mask);
                    break;

                case ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_READY:
                    hal_nvic_save_and_set_interrupt_mask(&saved_mask);
                    /* update stream status */
                    dongle_handle->first_packet_status = ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_READY;
                    dongle_handle->stream_status = ULL_AUDIO_V2_DONGLE_STREAM_RUNNING;
                    hal_nvic_restore_interrupt_mask(saved_mask);
                    break;

                /* Error status */
                default:
                    AUDIO_ASSERT(0);
                    break;
            }
            break;

        /* In this status stage, the stream is started and we will set flag to fetch a new packet */
        case ULL_AUDIO_V2_DONGLE_STREAM_RUNNING:
            break;

        /* Error status */
        default:
            AUDIO_ASSERT(0);
            break;
    }

    /* process the frame when the stream is running */
    dongle_handle->drop_frames = 0;
    MCE_GetBtClk((BTCLK*)&bt_clk, (BTPHASE*)&bt_phase, BT_CLK_Offset);
    if (dongle_handle->stream_status == ULL_AUDIO_V2_DONGLE_STREAM_RUNNING)
    {
        if (dongle_handle->first_packet_status == ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_READY)
        {
            if (dongle_handle->process_frames == 0)
            {
                /* bypass decoder */
                dongle_handle->data_status = ULL_AUDIO_V2_DONGLE_UL_DATA_BYPASS_DECODER;
                /* output prefill size data but not update write offest */
                dongle_handle->buffer_output_size = dongle_handle->buffer_default_output_size;
            }
            else
            {
                /* first packet must not be timeout, so do not need to if the new packet is timeout here */
                /* fetch first packet data into sw buffer but not play */
                if (ull_audio_v2_dongle_ul_fetch_time_is_arrived(dongle_handle, bt_clk) == true)
                {
                    /* do decode */
                    dongle_handle->data_status = ULL_AUDIO_V2_DONGLE_UL_DATA_NORMAL;
                }
                else
                {
                    /* bypass decoder */
                    dongle_handle->data_status = ULL_AUDIO_V2_DONGLE_UL_DATA_BYPASS_DECODER;
                }
                dongle_handle->buffer_output_size = 0;
            }
        }
        else if (dongle_handle->first_packet_status == ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_PLAYED)
        {
            if (dongle_handle->source_info.bt_in.sample_format == HAL_AUDIO_PCM_FORMAT_S24_LE)
            {
                sample_size = sizeof(int32_t);
            }
            else
            {
                sample_size = sizeof(int16_t);
            }
            remain_samples = stream_function_sw_buffer_get_channel_used_size(dongle_handle->buffer_port, 1) / sample_size;
            if ((dongle_handle->fetch_count != 0) && (remain_samples >= dongle_handle->buffer_default_output_size/sample_size))
            {
                /* the remain samples in sw buffer are enough, so output remain data first then process new frame in later */
                /* bypass decoder */
                dongle_handle->data_status = ULL_AUDIO_V2_DONGLE_UL_DATA_BYPASS_DECODER;
                dongle_handle->buffer_output_size = dongle_handle->buffer_default_output_size;
            }
            else if ((dongle_handle->fetch_count != 0) && (remain_samples < dongle_handle->buffer_default_output_size/sample_size))
            {
                /* check if the new packet is timeout */
                while (ull_audio_v2_dongle_ul_fetch_time_is_timeout(dongle_handle, bt_clk) == true)
                {
                    /* drop this packet */
                    dongle_handle->drop_frames += 1;
                    dongle_handle->source_info.bt_in.fetch_anchor_previous = dongle_handle->source_info.bt_in.fetch_anchor;
                    dongle_handle->source_info.bt_in.fetch_anchor = (dongle_handle->source_info.bt_in.fetch_anchor + dongle_handle->source_info.bt_in.bt_interval) & 0x0fffffff;
                }
                if (dongle_handle->drop_frames != 0)
                {
                    /* drop all timeout packets */
                    for (i = 0; i < ULL_AUDIO_V2_DATA_CHANNEL_NUMBER; i++)
                    {
                        if (dongle_handle->source_info.bt_in.bt_info[i] == NULL)
                        {
                            continue;
                        }
                        else
                        {
                            /* get blk info */
                            p_share_info = (n9_dsp_share_info_ptr)((dongle_handle->source_info.bt_in.bt_info[i])->bt_link_info.share_info);
                            blk_size     = p_share_info->sub_info.block_info.block_size;
                            blk_num      = p_share_info->sub_info.block_info.block_num;
                            /* update blk index */
                            // (dongle_handle->source_info.bt_in.bt_info[i])->blk_index_previous = (dongle_handle->source_info.bt_in.bt_info[i])->blk_index;
                            blk_index_previous = (dongle_handle->source_info.bt_in.bt_info[i])->blk_index;
                            blk_index = ((dongle_handle->source_info.bt_in.bt_info[i])->blk_index + dongle_handle->drop_frames) % blk_num;
                            (dongle_handle->source_info.bt_in.bt_info[i])->blk_index = blk_index;
                        }
                    }
                    DSP_MW_LOG_E("[ULL Audio V2][UL][ERROR][handle 0x%x] packet is timeout %d, %d, %d, 0x%x, 0x%x, 0x%x", 7,
                                    dongle_handle,
                                    dongle_handle->drop_frames,
                                    blk_index_previous,
                                    blk_index,
                                    dongle_handle->source_info.bt_in.fetch_anchor_previous,
                                    dongle_handle->source_info.bt_in.fetch_anchor,
                                    bt_clk);
                }
                if (ull_audio_v2_dongle_ul_fetch_time_is_arrived(dongle_handle, bt_clk) == true)
                {
                    /* do decode */
                    dongle_handle->data_status = ULL_AUDIO_V2_DONGLE_UL_DATA_NORMAL;
                    dongle_handle->buffer_output_size = dongle_handle->buffer_default_output_size;
                }
                else
                {
                    /* do PLC */
                    dongle_handle->data_status = ULL_AUDIO_V2_DONGLE_UL_DATA_PLC;
                    dongle_handle->buffer_output_size = dongle_handle->buffer_default_output_size;
                }
            }
            else
            {
                /* check if the new packet is timeout */
                while (ull_audio_v2_dongle_ul_fetch_time_is_timeout(dongle_handle, bt_clk) == true)
                {
                    /* drop this packet */
                    dongle_handle->drop_frames += 1;
                    dongle_handle->source_info.bt_in.fetch_anchor_previous = dongle_handle->source_info.bt_in.fetch_anchor;
                    dongle_handle->source_info.bt_in.fetch_anchor = (dongle_handle->source_info.bt_in.fetch_anchor + dongle_handle->source_info.bt_in.bt_interval) & 0x0fffffff;
                }
                if (dongle_handle->drop_frames != 0)
                {
                    /* drop all timeout packets */
                    for (i = 0; i < ULL_AUDIO_V2_DATA_CHANNEL_NUMBER; i++)
                    {
                        if (dongle_handle->source_info.bt_in.bt_info[i] == NULL)
                        {
                            continue;
                        }
                        else
                        {
                            /* get blk info */
                            p_share_info = (n9_dsp_share_info_ptr)((dongle_handle->source_info.bt_in.bt_info[i])->bt_link_info.share_info);
                            blk_size     = p_share_info->sub_info.block_info.block_size;
                            blk_num      = p_share_info->sub_info.block_info.block_num;
                            /* update blk index */
                            // (dongle_handle->source_info.bt_in.bt_info[i])->blk_index_previous = (dongle_handle->source_info.bt_in.bt_info[i])->blk_index;
                            blk_index_previous = (dongle_handle->source_info.bt_in.bt_info[i])->blk_index;
                            blk_index = ((dongle_handle->source_info.bt_in.bt_info[i])->blk_index + dongle_handle->drop_frames) % blk_num;
                            (dongle_handle->source_info.bt_in.bt_info[i])->blk_index = blk_index;
                        }
                    }
                    DSP_MW_LOG_E("[ULL Audio V2][UL][ERROR][handle 0x%x] packet is timeout %d, %d, %d, 0x%x, 0x%x, 0x%x", 7,
                                    dongle_handle,
                                    dongle_handle->drop_frames,
                                    blk_index_previous,
                                    blk_index,
                                    dongle_handle->source_info.bt_in.fetch_anchor_previous,
                                    dongle_handle->source_info.bt_in.fetch_anchor,
                                    bt_clk);
                }
                if (ull_audio_v2_dongle_ul_fetch_time_is_arrived(dongle_handle, bt_clk) == true)
                {
                    /* do decode */
                    dongle_handle->data_status = ULL_AUDIO_V2_DONGLE_UL_DATA_NORMAL;
                    dongle_handle->buffer_output_size = 0;
                }
                else
                {
                    /* bypass decoder */
                    dongle_handle->data_status = ULL_AUDIO_V2_DONGLE_UL_DATA_BYPASS_DECODER;
                    dongle_handle->buffer_output_size = 0;
                }
            }
        }
        else
        {
            /* bypass decoder */
            dongle_handle->data_status = ULL_AUDIO_V2_DONGLE_UL_DATA_BYPASS_DECODER;
            dongle_handle->buffer_output_size = 0;
        }
    }
    else
    {
        /* bypass decoder */
        dongle_handle->data_status = ULL_AUDIO_V2_DONGLE_UL_DATA_BYPASS_DECODER;
        dongle_handle->buffer_output_size = 0;
    }
    switch (dongle_handle->data_status)
    {
        case ULL_AUDIO_V2_DONGLE_UL_DATA_BYPASS_DECODER:
            dongle_handle->source_info.bt_in.channel_data_status = 0;
            for (i = 0; i < ULL_AUDIO_V2_DATA_CHANNEL_NUMBER; i++)
            {
                if (dongle_handle->source_info.bt_in.bt_info[i] == NULL)
                {
                    continue;
                }
                else
                {
                    dst_buf = (uint8_t *)stream->callback.EntryPara.in_ptr[i];
                    /* set decoder do bypass flag */
                    *((lc3plus_dec_frame_status_t *)dst_buf) = LC3PLUS_DEC_FRAME_STATUS_BYPASS_DECODER;
                }
            }
            break;

        case ULL_AUDIO_V2_DONGLE_UL_DATA_PLC:
            dongle_handle->source_info.bt_in.channel_data_status = 0;
            for (i = 0; i < ULL_AUDIO_V2_DATA_CHANNEL_NUMBER; i++)
            {
                if (dongle_handle->source_info.bt_in.bt_info[i] == NULL)
                {
                    continue;
                }
                else
                {
                    dst_buf = (uint8_t *)stream->callback.EntryPara.in_ptr[i];
                    /* set decoder do PLC flag */
                    *((lc3plus_dec_frame_status_t *)dst_buf) = LC3PLUS_DEC_FRAME_STATUS_PLC;
                }
            }
            break;

        case ULL_AUDIO_V2_DONGLE_UL_DATA_NORMAL:
            dongle_handle->drop_frames += 1;
            dongle_handle->source_info.bt_in.channel_data_status = 0;
            for (i = 0; i < ULL_AUDIO_V2_DATA_CHANNEL_NUMBER; i++)
            {
                if (dongle_handle->source_info.bt_in.bt_info[i] == NULL)
                {
                    continue;
                }
                else
                {
                    dst_buf = (uint8_t *)stream->callback.EntryPara.in_ptr[i];
                    p_share_info = (n9_dsp_share_info_ptr)((dongle_handle->source_info.bt_in.bt_info[i])->bt_link_info.share_info);
                    read_offset = (dongle_handle->source_info.bt_in.bt_info[i])->blk_index*p_share_info->sub_info.block_info.block_size;
                    p_ull_audio_header = (ULL_AUDIO_V2_HEADER *)(hal_memview_infrasys_to_dsp0(p_share_info->start_addr) + read_offset);
                    src_buf = (uint8_t *)(hal_memview_infrasys_to_dsp0(p_share_info->start_addr) + read_offset + sizeof(ULL_AUDIO_V2_HEADER));
                    if (p_ull_audio_header->valid_packet == 0)
                    {
                        /* set decoder do PLC flag */
                        *((lc3plus_dec_frame_status_t *)dst_buf) = LC3PLUS_DEC_FRAME_STATUS_PLC;
                        // DSP_MW_LOG_E("[ULL Audio V2][UL][ERROR][handle 0x%x] valid flag is not right %d, 0x%x, 0x%x, 0x%x, 0x%x", 6,
                        //                 dongle_handle,
                        //                 (dongle_handle->source_info.bt_in.bt_info[i])->blk_index,
                        //                 dongle_handle->source_info.bt_in.fetch_anchor_previous,
                        //                 dongle_handle->source_info.bt_in.fetch_anchor,
                        //                 p_ull_audio_header->TimeStamp,
                        //                 bt_clk);
                    }
                    else
                    {
                        /* check frame size */
                        if (p_ull_audio_header->PduLen != dongle_handle->source_info.bt_in.frame_size)
                        {
                            DSP_MW_LOG_E("[ULL Audio V2][UL][ERROR][handle 0x%x] frame_size is not right %d, %d", 3,
                                            dongle_handle,
                                            p_ull_audio_header->PduLen,
                                            dongle_handle->source_info.bt_in.frame_size);
                            AUDIO_ASSERT(0);
                        }
                        /* check frame timestamp */
                        if (ull_audio_v2_dongle_ul_fetch_timestamp_is_correct(dongle_handle, p_ull_audio_header->TimeStamp))
                        {
                            /* set decoder do decode flag */
                            *((lc3plus_dec_frame_status_t *)dst_buf) = LC3PLUS_DEC_FRAME_STATUS_NORMAL;
                            /* copy frame data into the stream buffer */
                            memcpy(dst_buf+sizeof(lc3plus_dec_frame_status_t), src_buf, dongle_handle->source_info.bt_in.frame_size);
                            dongle_handle->source_info.bt_in.channel_data_status |= 0x1<<i;
                        }
                        else
                        {
                            /* set decoder do PLC flag */
                            *((lc3plus_dec_frame_status_t *)dst_buf) = LC3PLUS_DEC_FRAME_STATUS_PLC;
                            DSP_MW_LOG_E("[ULL Audio V2][UL][ERROR][handle 0x%x] timestamp is not right %d, 0x%x, 0x%x, 0x%x, 0x%x", 6,
                                            dongle_handle,
                                            (dongle_handle->source_info.bt_in.bt_info[i])->blk_index,
                                            dongle_handle->source_info.bt_in.fetch_anchor_previous,
                                            dongle_handle->source_info.bt_in.fetch_anchor,
                                            p_ull_audio_header->TimeStamp,
                                            bt_clk);
                        }
                    }
                }
            }
            /* update fetch timestamp */
            dongle_handle->source_info.bt_in.fetch_anchor_previous = dongle_handle->source_info.bt_in.fetch_anchor;
            dongle_handle->source_info.bt_in.fetch_anchor = (dongle_handle->source_info.bt_in.fetch_anchor + dongle_handle->source_info.bt_in.bt_interval) & 0x0fffffff;
            break;

        default:
            AUDIO_ASSERT(0);
            break;
    }

    /* update stream status */
    stream->callback.EntryPara.in_size = dongle_handle->source_info.bt_in.frame_size+sizeof(lc3plus_dec_frame_status_t);
    if (dongle_handle->source_info.bt_in.sample_format == HAL_AUDIO_PCM_FORMAT_S16_LE)
    {
        stream->callback.EntryPara.resolution.source_in_res = RESOLUTION_16BIT;
        stream->callback.EntryPara.resolution.feature_res   = RESOLUTION_16BIT;
        stream->callback.EntryPara.resolution.sink_out_res  = RESOLUTION_16BIT;
        stream->callback.EntryPara.resolution.process_res   = RESOLUTION_16BIT;
    }
    else
    {
        stream->callback.EntryPara.resolution.source_in_res = RESOLUTION_32BIT;
        stream->callback.EntryPara.resolution.feature_res   = RESOLUTION_32BIT;
        stream->callback.EntryPara.resolution.sink_out_res  = RESOLUTION_32BIT;
        stream->callback.EntryPara.resolution.process_res   = RESOLUTION_32BIT;
    }

    /* configure clock skew settings */
    if (dongle_handle->data_status != ULL_AUDIO_V2_DONGLE_UL_DATA_BYPASS_DECODER)
    {
        dongle_handle->compen_samples = ull_audio_v2_dongle_ul_usb_clock_skew_check(dongle_handle);
        stream_function_sw_clk_skew_configure_compensation_samples(dongle_handle->clk_skew_port, dongle_handle->compen_samples);
    }
    else
    {
        stream_function_sw_clk_skew_configure_compensation_samples(dongle_handle->clk_skew_port, 0);
    }

    /* configure buffer output size */
    stream_function_sw_buffer_config_channel_output_size(dongle_handle->buffer_port, 0, dongle_handle->buffer_output_size);

    return length;
}

bool ull_audio_v2_dongle_ul_source_get_new_read_offset(SOURCE source, U32 amount, U32 *new_read_offset)
{
    ull_audio_v2_dongle_ul_handle_t *dongle_handle = (ull_audio_v2_dongle_ul_handle_t *)source->param.bt_common.scenario_param.dongle_handle;
    uint32_t i;
    n9_dsp_share_info_ptr p_share_info;
    uint32_t blk_size;
    uint32_t blk_num;
    uint32_t blk_index;
    UNUSED(amount);
    UNUSED(new_read_offset);

    if (dongle_handle->drop_frames != 0)
    {
        for (i = 0; i < ULL_AUDIO_V2_DATA_CHANNEL_NUMBER; i++)
        {
            if (dongle_handle->source_info.bt_in.bt_info[i] == NULL)
            {
                continue;
            }
            else
            {
                /* get blk info */
                p_share_info = (n9_dsp_share_info_ptr)((dongle_handle->source_info.bt_in.bt_info[i])->bt_link_info.share_info);
                blk_size     = p_share_info->sub_info.block_info.block_size;
                blk_num      = p_share_info->sub_info.block_info.block_num;
                /* get new blk index */
                // (dongle_handle->source_info.bt_in.bt_info[i])->blk_index_previous = (dongle_handle->source_info.bt_in.bt_info[i])->blk_index;
                blk_index = ((dongle_handle->source_info.bt_in.bt_info[i])->blk_index_previous + 1 + dongle_handle->drop_frames) % blk_num;
                // (dongle_handle->source_info.bt_in.bt_info[i])->blk_index = blk_index;
                /* update read index */
                StreamDSP_HWSemaphoreTake();
                p_share_info->read_offset = blk_index * blk_size;
                StreamDSP_HWSemaphoreGive();
            }
        }
    }

    /* we will update the read offsets of the different share buffers in here directly, so return false to aviod the upper layer update read offset */
    return false;
}

void ull_audio_v2_dongle_ul_source_drop_postprocess(SOURCE source, uint32_t amount)
{
    ull_audio_v2_dongle_ul_handle_t *dongle_handle = (ull_audio_v2_dongle_ul_handle_t *)source->param.bt_common.scenario_param.dongle_handle;
    DSP_STREAMING_PARA_PTR stream = DSP_Streaming_Get(source, source->transform->sink);
    uint32_t i;
    uint32_t read_offset;
    n9_dsp_share_info_ptr p_share_info;
    uint32_t blk_size;
    uint32_t blk_num;
    uint32_t blk_index = 0;
    uint32_t blk_index_previous = 0;
    ULL_AUDIO_V2_HEADER *p_ull_audio_header;
    uint32_t saved_mask;
    uint32_t bt_clk;
    uint16_t bt_phase;
    uint32_t remain_samples;
    uint32_t sample_size = 0;
    UNUSED(amount);
    UNUSED(stream);

    /* update time stamp */
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, (uint32_t *)&dongle_handle->data_out_gpt_count);

    /* get sample size */
    if (dongle_handle->source_info.bt_in.sample_format == HAL_AUDIO_PCM_FORMAT_S24_LE)
    {
        sample_size = sizeof(int32_t);
    }
    else
    {
        sample_size = sizeof(int16_t);
    }

    /* audio dump */
    #ifdef AIR_AUDIO_DUMP_ENABLE
    #if ULL_AUDIO_V2_DONGLE_UL_PATH_DEBUG_DUMP
    uint8_t *read_pointer;
    uint32_t data_size;
    if (dongle_handle->buffer_output_size != 0)
    {
        if (dongle_handle->sink_info.usb_out.channel_num == 2)
        {
            LOG_AUDIO_DUMP((uint8_t *)(stream->callback.EntryPara.out_ptr[0]), dongle_handle->process_frames*dongle_handle->sink_info.usb_out.frame_samples*sample_size, VOICE_TX_MIC_2);
            LOG_AUDIO_DUMP((uint8_t *)(stream->callback.EntryPara.out_ptr[1]), dongle_handle->process_frames*dongle_handle->sink_info.usb_out.frame_samples*sample_size, VOICE_TX_MIC_3);
        }
        else
        {
            LOG_AUDIO_DUMP((uint8_t *)(stream->callback.EntryPara.out_ptr[0]), dongle_handle->process_frames*dongle_handle->sink_info.usb_out.frame_samples*sample_size, VOICE_TX_MIC_2);
        }
    }
    if (dongle_handle->data_status == ULL_AUDIO_V2_DONGLE_UL_DATA_NORMAL)
    {
        LOG_AUDIO_DUMP((uint8_t *)(stream->callback.EntryPara.in_ptr[0])+sizeof(lc3plus_dec_frame_status_t), dongle_handle->source_info.bt_in.frame_size, VOICE_TX_MIC_0);
        LOG_AUDIO_DUMP((uint8_t *)(stream->callback.EntryPara.in_ptr[1])+sizeof(lc3plus_dec_frame_status_t), dongle_handle->source_info.bt_in.frame_size, VOICE_TX_MIC_1);
    }
    if ((dongle_handle->data_status == ULL_AUDIO_V2_DONGLE_UL_DATA_NORMAL) || (dongle_handle->data_status == ULL_AUDIO_V2_DONGLE_UL_DATA_PLC))
    {
        if (dongle_handle->sink_info.usb_out.channel_num == 2)
        {
            /* dump channel 1's decoder output data */
            stream_function_sw_mixer_channel_input_get_data_info(dongle_handle->mixer_member, 1, &read_pointer, NULL, &data_size);
            LOG_AUDIO_DUMP(read_pointer, data_size, VOICE_TX_REF);
            /* dump channel 2's decoder output data */
            stream_function_sw_mixer_channel_input_get_data_info(dongle_handle->mixer_member, 2, &read_pointer, NULL, &data_size);
            LOG_AUDIO_DUMP(read_pointer, data_size, VOICE_TX_NR_OUT);
        }
        else
        {
            /* dump channel 1's decoder output data */
            stream_function_sw_mixer_channel_input_get_data_info(dongle_handle->mixer_member, 1, &read_pointer, NULL, &data_size);
            LOG_AUDIO_DUMP(read_pointer, data_size, VOICE_TX_REF);
        }
    }
    #endif /* ULL_AUDIO_V2_DONGLE_UL_PATH_DEBUG_DUMP */
    #endif /* AIR_AUDIO_DUMP_ENABLE */

    /* add debug log */
    MCE_GetBtClk((BTCLK*)&bt_clk, (BTPHASE*)&bt_phase, BT_CLK_Offset);
    for (i = 0; i < ULL_AUDIO_V2_DATA_CHANNEL_NUMBER; i++)
    {
        if (dongle_handle->source_info.bt_in.bt_info[i] == NULL)
        {
            continue;
        }
        else
        {
            blk_index_previous = (dongle_handle->source_info.bt_in.bt_info[i])->blk_index_previous;
            blk_index = (dongle_handle->source_info.bt_in.bt_info[i])->blk_index;
            break;
        }
    }
    remain_samples = stream_function_sw_buffer_get_channel_used_size(dongle_handle->buffer_port, 1) / sample_size;
    DSP_MW_LOG_I("[ULL Audio V2][UL][handle 0x%x][source_drop_postprocess] %d, %d, %d, %d, %d, %d, %d, 0x%x, 0x%x, 0x%x, 0x%x, %d, %d, %d, %d, 0x%x, 0x%x, 0x%x, 0x%x", 20,
                    dongle_handle,
                    dongle_handle->fetch_count,
                    dongle_handle->stream_status,
                    dongle_handle->first_packet_status,
                    dongle_handle->data_status,
                    dongle_handle->drop_frames,
                    blk_index_previous,
                    blk_index,
                    dongle_handle->source_info.bt_in.fetch_anchor_previous,
                    dongle_handle->source_info.bt_in.fetch_anchor,
                    bt_clk,
                    dongle_handle->source_info.bt_in.channel_data_status,
                    dongle_handle->buffer_output_size/sample_size,
                    remain_samples,
                    dongle_handle->clk_skew_count,
                    dongle_handle->compen_samples,
                    dongle_handle->ccni_in_bt_count,
                    dongle_handle->data_out_bt_count,
                    dongle_handle->ccni_in_gpt_count,
                    dongle_handle->data_out_gpt_count);

    /* drop packets */
    if (dongle_handle->drop_frames != 0)
    {
        for (i = 0; i < ULL_AUDIO_V2_DATA_CHANNEL_NUMBER; i++)
        {
            if (dongle_handle->source_info.bt_in.bt_info[i] == NULL)
            {
                continue;
            }
            else
            {
                /* get blk info */
                p_share_info = (n9_dsp_share_info_ptr)((dongle_handle->source_info.bt_in.bt_info[i])->bt_link_info.share_info);
                blk_size     = p_share_info->sub_info.block_info.block_size;
                blk_num      = p_share_info->sub_info.block_info.block_num;
                /* update blk index */
                (dongle_handle->source_info.bt_in.bt_info[i])->blk_index_previous = (dongle_handle->source_info.bt_in.bt_info[i])->blk_index;
                blk_index_previous = (dongle_handle->source_info.bt_in.bt_info[i])->blk_index;
                blk_index = ((dongle_handle->source_info.bt_in.bt_info[i])->blk_index + dongle_handle->drop_frames) % blk_num;
                (dongle_handle->source_info.bt_in.bt_info[i])->blk_index = blk_index;
                /* clear valid flag */
                while (blk_index != blk_index_previous)
                {
                    /* get packet header */
                    read_offset = blk_index_previous*blk_size;
                    p_ull_audio_header = (ULL_AUDIO_V2_HEADER *)(hal_memview_infrasys_to_dsp0(p_share_info->start_addr) + read_offset);
                    /* clear valid flag */
                    p_ull_audio_header->valid_packet = 0;
                    /* switch the next block */
                    blk_index_previous = (blk_index_previous+1)% blk_num;
                }
            }
        }
        dongle_handle->drop_frames = 0;
    }

    /* decrease fetch count */
    hal_nvic_save_and_set_interrupt_mask(&saved_mask);
    if (dongle_handle->fetch_count != 0)
    {
        dongle_handle->fetch_count -= 1;
    }
    hal_nvic_restore_interrupt_mask(saved_mask);
}

bool ull_audio_v2_dongle_ul_source_close(SOURCE source)
{
    UNUSED(source);

    return true;
}

ATTR_TEXT_IN_IRAM bool ull_audio_v2_dongle_ul_sink_get_avail_size(SINK sink, uint32_t *avail_size)
{
    ull_audio_v2_dongle_ul_handle_t *dongle_handle = (ull_audio_v2_dongle_ul_handle_t *)(sink->transform->source->param.bt_common.scenario_param.dongle_handle);
    uint32_t frame_num;

    if (sink->streamBuffer.ShareBufferInfo.read_offset < sink->streamBuffer.ShareBufferInfo.write_offset)
    {
        /* normal case */
        *avail_size = sink->streamBuffer.ShareBufferInfo.sub_info.block_info.block_size * sink->streamBuffer.ShareBufferInfo.sub_info.block_info.block_num
                    - sink->streamBuffer.ShareBufferInfo.write_offset
                    + sink->streamBuffer.ShareBufferInfo.read_offset;
    }
    else if (sink->streamBuffer.ShareBufferInfo.read_offset == sink->streamBuffer.ShareBufferInfo.write_offset)
    {
        if(sink->streamBuffer.ShareBufferInfo.bBufferIsFull == true)
        {
            /* buffer is full, so read_offset == write_offset */
            *avail_size = 0;
        }
        else
        {
            /* buffer is empty, so read_offset == write_offset */
            *avail_size = sink->streamBuffer.ShareBufferInfo.length;
        }
    }
    else
    {
        /* buffer wrapper case */
        *avail_size = sink->streamBuffer.ShareBufferInfo.read_offset - sink->streamBuffer.ShareBufferInfo.write_offset;
    }

    frame_num = *avail_size / sink->streamBuffer.ShareBufferInfo.sub_info.block_info.block_size;
    if ((frame_num < dongle_handle->sink_info.usb_out.frame_max_num) && (dongle_handle->fetch_count != 0))
    {
        /* in each time, there must be at least 10ms data free space in the share buffer */
        DSP_MW_LOG_E("[ULL Audio V2][UL][ERROR]frame num is not right, %u, %u, %u\r\n", 3, frame_num, dongle_handle->sink_info.usb_out.frame_max_num, dongle_handle->fetch_count);
        AUDIO_ASSERT(0);
    }

    return true;
}

uint32_t ull_audio_v2_dongle_ul_sink_copy_payload(SINK sink, uint8_t *src_buf, uint8_t *dst_buf, uint32_t length)
{
    ull_audio_v2_dongle_ul_handle_t *dongle_handle = (ull_audio_v2_dongle_ul_handle_t *)(sink->transform->source->param.bt_common.scenario_param.dongle_handle);
    DSP_STREAMING_PARA_PTR stream = DSP_Streaming_Get(sink->transform->source, sink);
    uint32_t process_frames;
    uint32_t saved_mask;
    uint32_t i;
    uint32_t payload_size;
    UNUSED(src_buf);

    /* copy pcm data into the share buffer one by one 1ms */
    if (dongle_handle->source_info.bt_in.sample_format == HAL_AUDIO_PCM_FORMAT_S24_LE)
    {
        if (length%(sizeof(int32_t)*dongle_handle->sink_info.usb_out.frame_samples) != 0)
        {
            DSP_MW_LOG_E("[ULL Audio V2][UL][ERROR]length is not right, %u\r\n", 1, length);
            AUDIO_ASSERT(0);
        }
        process_frames = ((length/sizeof(int32_t))/dongle_handle->sink_info.usb_out.frame_samples);
    }
    else
    {
        if (length%(sizeof(int16_t)*dongle_handle->sink_info.usb_out.frame_samples) != 0)
        {
            DSP_MW_LOG_E("[ULL Audio V2][UL][ERROR]length is not right, %u\r\n", 1, length);
            AUDIO_ASSERT(0);
        }
        process_frames = ((length/sizeof(int16_t))/dongle_handle->sink_info.usb_out.frame_samples);
    }
    for (i = 0; i < process_frames; i++)
    {
        /* get dst buffer */
        dst_buf = (uint8_t *)(sink->streamBuffer.ShareBufferInfo.start_addr + ((sink->streamBuffer.ShareBufferInfo.write_offset + sink->streamBuffer.ShareBufferInfo.sub_info.block_info.block_size*i) % (sink->streamBuffer.ShareBufferInfo.sub_info.block_info.block_size * sink->streamBuffer.ShareBufferInfo.sub_info.block_info.block_num)));

        if ((dongle_handle->sink_info.usb_out.sample_format == HAL_AUDIO_PCM_FORMAT_S16_LE) && (dongle_handle->source_info.bt_in.sample_format == HAL_AUDIO_PCM_FORMAT_S16_LE))
        {
            /* copy pcm samples into the share buffer */
            if (dongle_handle->sink_info.usb_out.channel_num == 2)
            {
                // DSP_D2I_BufferCopy_16bit((U16 *)(dst_buf+sizeof(audio_transmitter_frame_header_t)),
                //                         (U16 *)(stream->callback.EntryPara.out_ptr[0])+i*dongle_handle->sink_info.usb_out.frame_samples,
                //                         (U16 *)(stream->callback.EntryPara.out_ptr[1])+i*dongle_handle->sink_info.usb_out.frame_samples,
                //                         dongle_handle->sink_info.usb_out.frame_samples);
                ShareBufferCopy_D_16bit_to_I_16bit_2ch( (uint16_t *)(stream->callback.EntryPara.out_ptr[0])+i*dongle_handle->sink_info.usb_out.frame_samples,
                                                        (uint16_t *)(stream->callback.EntryPara.out_ptr[1])+i*dongle_handle->sink_info.usb_out.frame_samples,
                                                        (uint16_t *)(dst_buf+sizeof(audio_transmitter_frame_header_t)),
                                                        dongle_handle->sink_info.usb_out.frame_samples);
                payload_size = dongle_handle->sink_info.usb_out.frame_samples*2*sizeof(int16_t);
            }
            else
            {
                // memcpy(dst_buf+sizeof(audio_transmitter_frame_header_t), (U16 *)(stream->callback.EntryPara.out_ptr[0])+i*dongle_handle->sink_info.usb_out.frame_samples, dongle_handle->sink_info.usb_out.frame_samples*sizeof(int16_t));
                ShareBufferCopy_D_16bit_to_D_16bit_1ch( (uint16_t *)(stream->callback.EntryPara.out_ptr[0])+i*dongle_handle->sink_info.usb_out.frame_samples,
                                                        (uint16_t *)(dst_buf+sizeof(audio_transmitter_frame_header_t)),
                                                        dongle_handle->sink_info.usb_out.frame_samples);
                payload_size = dongle_handle->sink_info.usb_out.frame_samples*sizeof(int16_t);
            }
        }
        else if ((dongle_handle->sink_info.usb_out.sample_format == HAL_AUDIO_PCM_FORMAT_S16_LE) && (dongle_handle->source_info.bt_in.sample_format == HAL_AUDIO_PCM_FORMAT_S24_LE))
        {
            /* copy pcm samples into the share buffer */
            if (dongle_handle->sink_info.usb_out.channel_num == 2)
            {
                ShareBufferCopy_D_32bit_to_I_16bit_2ch( (uint32_t *)(stream->callback.EntryPara.out_ptr[0])+i*dongle_handle->sink_info.usb_out.frame_samples,
                                                        (uint32_t *)(stream->callback.EntryPara.out_ptr[1])+i*dongle_handle->sink_info.usb_out.frame_samples,
                                                        (uint16_t *)(dst_buf+sizeof(audio_transmitter_frame_header_t)),
                                                        dongle_handle->sink_info.usb_out.frame_samples);
                payload_size = dongle_handle->sink_info.usb_out.frame_samples*2*sizeof(int16_t);
            }
            else
            {
                ShareBufferCopy_D_32bit_to_D_16bit_1ch( (uint32_t *)(stream->callback.EntryPara.out_ptr[0])+i*dongle_handle->sink_info.usb_out.frame_samples,
                                                        (uint16_t *)(dst_buf+sizeof(audio_transmitter_frame_header_t)),
                                                        dongle_handle->sink_info.usb_out.frame_samples);
                payload_size = dongle_handle->sink_info.usb_out.frame_samples*sizeof(int16_t);
            }
        }
        else if ((dongle_handle->sink_info.usb_out.sample_format == HAL_AUDIO_PCM_FORMAT_S24_LE) && (dongle_handle->source_info.bt_in.sample_format == HAL_AUDIO_PCM_FORMAT_S16_LE))
        {
            /* copy pcm samples into the share buffer */
            if (dongle_handle->sink_info.usb_out.channel_num == 2)
            {
                ShareBufferCopy_D_16bit_to_I_24bit_2ch( (uint16_t *)(stream->callback.EntryPara.out_ptr[0])+i*dongle_handle->sink_info.usb_out.frame_samples,
                                                        (uint16_t *)(stream->callback.EntryPara.out_ptr[1])+i*dongle_handle->sink_info.usb_out.frame_samples,
                                                        (uint8_t *)(dst_buf+sizeof(audio_transmitter_frame_header_t)),
                                                        dongle_handle->sink_info.usb_out.frame_samples);
                payload_size = dongle_handle->sink_info.usb_out.frame_samples*2*3;
            }
            else
            {
                ShareBufferCopy_D_16bit_to_D_24bit_1ch( (uint16_t *)(stream->callback.EntryPara.out_ptr[0])+i*dongle_handle->sink_info.usb_out.frame_samples,
                                                        (uint16_t *)(dst_buf+sizeof(audio_transmitter_frame_header_t)),
                                                        dongle_handle->sink_info.usb_out.frame_samples);
                payload_size = dongle_handle->sink_info.usb_out.frame_samples*3;
            }
        }
        else if ((dongle_handle->sink_info.usb_out.sample_format == HAL_AUDIO_PCM_FORMAT_S24_LE) && (dongle_handle->source_info.bt_in.sample_format == HAL_AUDIO_PCM_FORMAT_S24_LE))
        {
            /* copy pcm samples into the share buffer */
            if (dongle_handle->sink_info.usb_out.channel_num == 2)
            {
                ShareBufferCopy_D_32bit_to_I_24bit_2ch( (uint32_t *)(stream->callback.EntryPara.out_ptr[0])+i*dongle_handle->sink_info.usb_out.frame_samples,
                                                        (uint32_t *)(stream->callback.EntryPara.out_ptr[1])+i*dongle_handle->sink_info.usb_out.frame_samples,
                                                        (uint16_t *)(dst_buf+sizeof(audio_transmitter_frame_header_t)),
                                                        dongle_handle->sink_info.usb_out.frame_samples);
                payload_size = dongle_handle->sink_info.usb_out.frame_samples*2*3;
            }
            else
            {
                ShareBufferCopy_D_32bit_to_D_24bit_1ch( (uint32_t *)(stream->callback.EntryPara.out_ptr[0])+i*dongle_handle->sink_info.usb_out.frame_samples,
                                                        (uint16_t *)(dst_buf+sizeof(audio_transmitter_frame_header_t)),
                                                        dongle_handle->sink_info.usb_out.frame_samples);
                payload_size = dongle_handle->sink_info.usb_out.frame_samples*3;
            }
        }
        else
        {
            DSP_MW_LOG_E("[ULL Audio V2][UL][ERROR]sample_format is not supported, %u, %u\r\n", 2, dongle_handle->sink_info.usb_out.sample_format, dongle_handle->source_info.bt_in.sample_format);
            AUDIO_ASSERT(0);
        }

        /* write seq number and payload_len into the share buffer */
        ((audio_transmitter_frame_header_t *)dst_buf)->seq_num      = dongle_handle->sink_info.usb_out.seq_num;
        ((audio_transmitter_frame_header_t *)dst_buf)->payload_len  = payload_size;

        /* update seq number */
        dongle_handle->sink_info.usb_out.seq_num = (dongle_handle->sink_info.usb_out.seq_num+1)&0xffff;
    }

    hal_nvic_save_and_set_interrupt_mask(&saved_mask);
    if (dongle_handle->first_packet_status == ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_PLAYED)
    {
        dongle_handle->process_frames = process_frames;
    }
    else
    {
        /* If stream is not played, only copy data into share buffer but not update write offset */
        dongle_handle->process_frames += process_frames;
    }
    hal_nvic_restore_interrupt_mask(saved_mask);

    return payload_size;
}

void ull_audio_v2_dongle_ul_sink_query_write_offset(SINK sink, uint32_t *write_offset)
{
    ull_audio_v2_dongle_ul_handle_t *dongle_handle = (ull_audio_v2_dongle_ul_handle_t *)(sink->transform->source->param.bt_common.scenario_param.dongle_handle);
    uint32_t saved_mask;

    hal_nvic_save_and_set_interrupt_mask(&saved_mask);
    if (dongle_handle->first_packet_status == ULL_AUDIO_V2_DONGLE_UL_FIRST_PACKET_PLAYED)
    {
        /* If stream is not played, update write offset */
        *write_offset = (sink->streamBuffer.ShareBufferInfo.write_offset + sink->streamBuffer.ShareBufferInfo.sub_info.block_info.block_size * dongle_handle->process_frames) % (sink->streamBuffer.ShareBufferInfo.sub_info.block_info.block_size * sink->streamBuffer.ShareBufferInfo.sub_info.block_info.block_num);
        audio_transmitter_share_information_update_write_offset(sink, *write_offset);
    }
    else
    {
        /* If stream is not played, only copy data into share buffer but not update write offset */
        *write_offset = sink->streamBuffer.ShareBufferInfo.write_offset;
    }
    hal_nvic_restore_interrupt_mask(saved_mask);

    /* update time stamp */
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, (uint32_t *)&dongle_handle->data_out_bt_count);

    #if ULL_AUDIO_V2_DONGLE_UL_PATH_DEBUG_LOG
    uint32_t current_timestamp = 0;
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &current_timestamp);
    DSP_MW_LOG_I("[ULL Audio V2][UL][DEBUG][sink_query_write_offset]: %u, %u, %d, %d", 4, dongle_handle->first_packet_status, *write_offset, current_timestamp, hal_nvic_query_exception_number());
    #endif /* ULL_AUDIO_V2_DONGLE_UL_PATH_DEBUG_LOG */
}

bool ull_audio_v2_dongle_ul_sink_close(SINK sink)
{
    UNUSED(sink);

    return true;
}

#endif /* AIR_ULL_AUDIO_V2_DONGLE_ENABLE */
