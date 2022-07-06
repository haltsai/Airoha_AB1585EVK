/* Copyright Statement:
 *
 * (C) 2019  Airoha Technology Corp. All rights reserved.
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

#ifdef MTK_USB_DEMO_ENABLED

#if defined(MTK_USB_AUDIO_V2_ENABLE)
#error "MTK_USB_AUDIO_V2_ENABLE, USB-Audio version is not supported !"
#endif

/* C library */
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* USB Middleware includes */
#include "usb.h"
#include "usb_comm.h"
#include "usb_host_detect.h"
#include "usb_nvkey_struct.h"
#include "usb_resource.h"
#include "usbaudio_drv.h"

/* Other Middleware includes */
#ifdef AIR_NVDM_ENABLE
#include "nvkey.h"
#include "nvkey_id_list.h"
#endif /* AIR_NVDM_ENABLE */

/* Hal includes */
#include "hal.h"
#include "hal_usb.h"

/* Other includes */
#include "memory_attribute.h"

/* Syslog create module for usbaudio_drv.c */
#include "syslog.h"
log_create_module_variant(USBAUDIO_DRV, DEBUG_LOG_ON, PRINT_LEVEL_INFO);

/* Syslog create module for usb debug */
log_create_module_variant(USB_DEBUG, DEBUG_LOG_OFF, PRINT_LEVEL_INFO);

#if defined(MTK_USB_AUDIO_V1_ENABLE)

UsbAudioStruct USB_Audio[2];
UsbAudio_Struct g_UsbAudio[2] = {
    /* Audio 1 device settings */
    {
        .spk_feature_en    = false,
        .mic_feature_en    = false,
        .spk_terminal_type = 0x0402,
        .spk_cur           = 0xF59A,
        .spk_min           = 0xB600,
        .spk_max           = 0xFFFF,
        .spk_res           = 0x0100,
        .mic_terminal_type = 0x0402,
        .mic_cur           = 0xF59A,
        .mic_min           = 0xB600,
        .mic_max           = 0xFFFF,
        .mic_res           = 0x0100,
    },
    /* Audio 2 device settings */
    {
        .spk_feature_en    = false,
        .mic_feature_en    = false,
        .spk_terminal_type = 0x0402,
        .spk_cur           = 0xF59A,
        .spk_min           = 0xB600,
        .spk_max           = 0xFFFF,
        .spk_res           = 0x0100,
        .mic_terminal_type = 0x0402,
        .mic_cur           = 0xF59A,
        .mic_min           = 0xB600,
        .mic_max           = 0xFFFF,
        .mic_res           = 0x0100,
    },
};

/* static functions */
static int16_t usb_audio_dscr_interface_serialize(void *dscr, void *out, uint16_t ava_size) __unused;
static int16_t usb_audio_dscr_control_header_serialize(void *dscr, void *out, uint16_t ava_size) __unused;
static int16_t usb_audio_dscr_it_serialize(void *dscr, void *out, uint16_t ava_size) __unused;
static int16_t usb_audio_dscr_ot_serialize(void *dscr, void *out, uint16_t ava_size) __unused;
static int16_t usb_audio_dscr_mixer_serialize(void *dscr, void *out, uint16_t ava_size) __unused;
static int16_t usb_audio_dscr_selector_serialize(void *dscr, void *out, uint16_t ava_size) __unused;
static int16_t usb_audio_dscr_feature_serialize(void *dscr, void *out, uint16_t ava_size) __unused;
static int16_t usb_audio_dscr_as_general_serialize(void *dscr, void *out, uint16_t ava_size) __unused;
static int16_t usb_audio_dscr_as_format_type_serialize(void *dscr, void *out, uint16_t ava_size) __unused;
static int16_t dscr_list_serialize(usb_audio_dscr_hdlr_t *dscr_list, uint8_t len, uint8_t *out, uint16_t ava_len) __unused;

/******************************************  Configuration Start    *********************************************/
static usb_audio_dscr_interface_t audio1_interface_dscr = {
    .bLength            = USB_IFDSC_LENGTH,
    .bDescriptorType    = USB_INTERFACE,
    .bInterfaceNumber   = 0x00, /* Overwrite by USB_Audio1_ControlIf_Create */
    .bAlternateSetting  = 0x00,
    .bNumEndpoints      = 0x00,
    .bInterfaceClass    = 0x01,
    .bInterfaceSubClass = 0x01,
    .bInterfaceProtocol = USB_AUDIO_INTERFACE_PROTOCOL,
    .iInterface         = 0x00, /* Overwrite by USB_Audio1_ControlIf_Create */
};

static usb_audio_dscr_control_header_t audio1_ach_dscr = {
    .bLength            = 0x00, /* Overwrite by USB_Audio1_ControlIf_Create */
    .bDescriptorType    = 0x24,
    .bDescriptorSubtype = USB_AUDIO_AC_SUBTYPE_HEADER,
    .bcdADC             = 0x0100,
    .wTotalLength       = 0x0047, /* Overwrite by USB_Audio1_ControlIf_Create */
    .bInCollection      = 0x02,   /* Overwrite by USB_Audio1_ControlIf_Create */
    .baInterfaceNr      =         /* Overwrite by USB_Audio1_ControlIf_Create */
    {
        0x01,
        0x02,
    },
};

static usb_audio_dscr_it_t audio1_spk_it_dscr = {
    .bLength            = 0x0C,
    .bDescriptorType    = 0x24,
    .bDescriptorSubtype = USB_AUDIO_AC_SUBTYPE_INPUT_TERMINAL,
    .bTerminalID        = 0x01,
    .wTerminalType      = 0x0101,
    .bAssocTerminal     = 0x00,
    .bNrChannels        = 0x02,
    .wChannelConfig     = 0x0003, /* (L, R) */
    .iChannelNames      = 0x00,
    .iTerminal          = 0x00,
};

static usb_audio_dscr_feature_t audio1_spk_feature_dscr = {
    .bLength            = 0x0A,
    .bDescriptorType    = 0x24,
    .bDescriptorSubtype = USB_AUDIO_AC_SUBTYPE_FEATURE_UNIT,
    .bUnitID            = 0x02,
    .bSourceID          = 0x01,
    .bControlSize       = 0x01,
    .bmaControls =
        {
            0x01, /* bmaControls(0); Master Channel support Mute      */
            0x02, /* bmaControls(1); Logical Channel 1 support Volume */
            0x02, /* bmaControls(2); Logical Channel 2 support Volume */
        },
    .iFeature   = 0x00,
    .control_nr = 0x03, /* num of bmaControls */
};

static usb_audio_dscr_ot_t audio1_spk_ot_dscr = {
    .bLength            = 0x09,
    .bDescriptorType    = 0x24,
    .bDescriptorSubtype = USB_AUDIO_AC_SUBTYPE_OUTPUT_TERMINAL,
    .bTerminalID        = 0x03,
    .wTerminalType      = 0x0402,
    .bAssocTerminal     = 0x00,
    .bSourceID          = 0x02,
    .iTerminal          = 0x00,
};

static usb_audio_dscr_it_t audio1_mic_it_dscr = {
    .bLength            = 0x0C,
    .bDescriptorType    = 0x24,
    .bDescriptorSubtype = USB_AUDIO_AC_SUBTYPE_INPUT_TERMINAL,
    .bTerminalID        = 0x05,
    .wTerminalType      = 0x0201,
    .bAssocTerminal     = 0x0B,
    .bNrChannels        = 0x01,
    .wChannelConfig     = 0x0000,
    .iChannelNames      = 0x00,
    .iTerminal          = 0x00,
};

static usb_audio_dscr_feature_t audio1_mic_feature_dscr = {
    .bLength            = 0x09,
    .bDescriptorType    = 0x24,
    .bDescriptorSubtype = USB_AUDIO_AC_SUBTYPE_FEATURE_UNIT,
    .bUnitID            = 0x06,
    .bSourceID          = 0x05,
    .bControlSize       = 0x01,
    .bmaControls =
        {
            0x01, /* bmaControls(0); Master Channel support Mute      */
            0x02, /* bmaControls(1); Logical Channel 1 support Volume */
        },
    .iFeature   = 0x00,
    .control_nr = 0x02, /* num of bmaControls */
};

static usb_audio_dscr_selector_t audio1_selector_dscr = {
    .bLength            = 0x07,
    .bDescriptorType    = 0x24,
    .bDescriptorSubtype = USB_AUDIO_AC_SUBTYPE_SELECTOR_UNIT,
    .bUnitID            = 0x04,
    .bNrInPins          = 0x01,
    .baSourceID =
        {
            0x06,
        },
    .iSelector = 0x00,
};

static usb_audio_dscr_ot_t audio1_mic_ot_dscr = {
    .bLength            = 0x09,
    .bDescriptorType    = 0x24,
    .bDescriptorSubtype = USB_AUDIO_AC_SUBTYPE_OUTPUT_TERMINAL,
    .bTerminalID        = 0x07,
    .wTerminalType      = 0x0101,
    .bAssocTerminal     = 0x00,
    .bSourceID          = 0x04,
    .iTerminal          = 0x00,
};

static usb_audio_dscr_hdlr_t audio1_aci_dscrs[] = {
    {&audio1_interface_dscr, usb_audio_dscr_interface_serialize},
    {&audio1_ach_dscr, usb_audio_dscr_control_header_serialize},
};

static usb_audio_dscr_hdlr_t audio1_spk_dscrs[] = {
    {&audio1_spk_it_dscr, usb_audio_dscr_it_serialize},
    {&audio1_spk_feature_dscr, usb_audio_dscr_feature_serialize},
    {&audio1_spk_ot_dscr, usb_audio_dscr_ot_serialize},
};

static usb_audio_dscr_hdlr_t audio1_mic_dscrs[] = {
    {&audio1_mic_it_dscr, usb_audio_dscr_it_serialize},
    {&audio1_mic_feature_dscr, usb_audio_dscr_feature_serialize},
    {&audio1_selector_dscr, usb_audio_dscr_selector_serialize},
    {&audio1_mic_ot_dscr, usb_audio_dscr_ot_serialize},
};

static usb_audio_dscr_interface_t audio2_interface_dscr = {
    .bLength            = USB_IFDSC_LENGTH,
    .bDescriptorType    = USB_INTERFACE,
    .bInterfaceNumber   = 0x00, /* Overwrite by USB_Audio2_ControlIf_Create */
    .bAlternateSetting  = 0x00,
    .bNumEndpoints      = 0x00,
    .bInterfaceClass    = 0x01,
    .bInterfaceSubClass = 0x01,
    .bInterfaceProtocol = USB_AUDIO_INTERFACE_PROTOCOL,
    .iInterface         = 0x00, /* Overwrite by USB_Audio2_ControlIf_Create */
};

static usb_audio_dscr_control_header_t audio2_ach_dscr = {
    .bLength            = 0x00, /* Overwrite by USB_Audio2_ControlIf_Create */
    .bDescriptorType    = 0x24,
    .bDescriptorSubtype = USB_AUDIO_AC_SUBTYPE_HEADER,
    .bcdADC             = 0x0100,
    .wTotalLength       = 0x0047, /* Overwrite by USB_Audio2_ControlIf_Create */
    .bInCollection      = 0x02,   /* Overwrite by USB_Audio2_ControlIf_Create */
    .baInterfaceNr      =         /* Overwrite by USB_Audio2_ControlIf_Create */
    {
        0x01,
        0x02,
    },
};

static usb_audio_dscr_it_t audio2_spk_it_dscr = {
    .bLength            = 0x0C,
    .bDescriptorType    = 0x24,
    .bDescriptorSubtype = USB_AUDIO_AC_SUBTYPE_INPUT_TERMINAL,
    .bTerminalID        = 0x09,
    .wTerminalType      = 0x0101,
    .bAssocTerminal     = 0x00,
    .bNrChannels        = 0x02,
    .wChannelConfig     = 0x0003, /* (L, R) */
    .iChannelNames      = 0x00,
    .iTerminal          = 0x00,
};

static usb_audio_dscr_feature_t audio2_spk_feature_dscr = {
    .bLength            = 0x0A,
    .bDescriptorType    = 0x24,
    .bDescriptorSubtype = USB_AUDIO_AC_SUBTYPE_FEATURE_UNIT,
    .bUnitID            = 0x0A,
    .bSourceID          = 0x09,
    .bControlSize       = 0x01,
    .bmaControls =
        {
            0x01, /* bmaControls(0); Master Channel support Mute      */
            0x02, /* bmaControls(1); Logical Channel 1 support Volume */
            0x02, /* bmaControls(2); Logical Channel 2 support Volume */
        },
    .iFeature   = 0x00,
    .control_nr = 0x03, /* num of bmaControls */
};

static usb_audio_dscr_ot_t audio2_spk_ot_dscr = {
    .bLength            = 0x09,
    .bDescriptorType    = 0x24,
    .bDescriptorSubtype = USB_AUDIO_AC_SUBTYPE_OUTPUT_TERMINAL,
    .bTerminalID        = 0x0B,
    .wTerminalType      = 0x0302,
    .bAssocTerminal     = 0x00,
    .bSourceID          = 0x0A,
    .iTerminal          = 0x00,
};

static usb_audio_dscr_it_t audio2_mic_it_dscr = {
    .bLength            = 0x0C,
    .bDescriptorType    = 0x24,
    .bDescriptorSubtype = USB_AUDIO_AC_SUBTYPE_INPUT_TERMINAL,
    .bTerminalID        = 0x0C,
    .wTerminalType      = 0x0201,
    .bAssocTerminal     = 0x0B,
    .bNrChannels        = 0x01,
    .wChannelConfig     = 0x0000,
    .iChannelNames      = 0x00,
    .iTerminal          = 0x00,
};

static usb_audio_dscr_feature_t audio2_mic_feature_dscr = {
    .bLength            = 0x09,
    .bDescriptorType    = 0x24,
    .bDescriptorSubtype = USB_AUDIO_AC_SUBTYPE_FEATURE_UNIT,
    .bUnitID            = 0x0D,
    .bSourceID          = 0x0C,
    .bControlSize       = 0x01,
    .bmaControls =
        {
            0x01, /* bmaControls(0); Master Channel support Mute      */
            0x02, /* bmaControls(1); Logical Channel 1 support Volume */
        },
    .iFeature   = 0x00,
    .control_nr = 0x02, /* num of bmaControls */
};

static usb_audio_dscr_selector_t audio2_selector_dscr = {
    .bLength            = 0x07,
    .bDescriptorType    = 0x24,
    .bDescriptorSubtype = USB_AUDIO_AC_SUBTYPE_SELECTOR_UNIT,
    .bUnitID            = 0x0E,
    .bNrInPins          = 0x01,
    .baSourceID =
        {
            0x0D,
        },
    .iSelector = 0x00,
};

static usb_audio_dscr_ot_t audio2_mic_ot_dscr = {
    .bLength            = 0x09,
    .bDescriptorType    = 0x24,
    .bDescriptorSubtype = USB_AUDIO_AC_SUBTYPE_OUTPUT_TERMINAL,
    .bTerminalID        = 0x0F,
    .wTerminalType      = 0x0101,
    .bAssocTerminal     = 0x00,
    .bSourceID          = 0x0E,
    .iTerminal          = 0x00,
};

static usb_audio_dscr_hdlr_t audio2_aci_dscrs[] = {
    {&audio2_interface_dscr, usb_audio_dscr_interface_serialize},
    {&audio2_ach_dscr, usb_audio_dscr_control_header_serialize},
};

static usb_audio_dscr_hdlr_t audio2_spk_dscrs[] = {
    {&audio2_spk_it_dscr, usb_audio_dscr_it_serialize},
    {&audio2_spk_feature_dscr, usb_audio_dscr_feature_serialize},
    {&audio2_spk_ot_dscr, usb_audio_dscr_ot_serialize},
};

static usb_audio_dscr_hdlr_t audio2_mic_dscrs[] = {
    {&audio2_mic_it_dscr, usb_audio_dscr_it_serialize},
    {&audio2_mic_feature_dscr, usb_audio_dscr_feature_serialize},
    {&audio2_selector_dscr, usb_audio_dscr_selector_serialize},
    {&audio2_mic_ot_dscr, usb_audio_dscr_ot_serialize},
};

/****************************************** Configuration End *********************************************/
static usb_audio_dscr_interface_t audio1_spk_stream_if_dscr = {
    .bLength            = USB_IFDSC_LENGTH,
    .bDescriptorType    = USB_INTERFACE,
    .bInterfaceNumber   = 0x00, /* Overwrite by USB_Audio1_StreamIf_Create */
    .bAlternateSetting  = 0x00,
    .bNumEndpoints      = 0x00,
    .bInterfaceClass    = 0x01,
    .bInterfaceSubClass = 0x02,
    .bInterfaceProtocol = USB_AUDIO_INTERFACE_PROTOCOL,
    .iInterface         = 0x00, /* Overwrite by USB_Audio1_StreamIf_Create */
};

static usb_audio_dscr_interface_t audio1_spk_stream_alt_if_dscr = {
    .bLength            = USB_IFDSC_LENGTH,
    .bDescriptorType    = USB_INTERFACE,
    .bInterfaceNumber   = 0x00, /* Overwrite by USB_Audio1_StreamIf_Create */
    .bAlternateSetting  = 0x01,
    .bNumEndpoints      = 0x01,
    .bInterfaceClass    = 0x01,
    .bInterfaceSubClass = 0x02,
    .bInterfaceProtocol = USB_AUDIO_INTERFACE_PROTOCOL,
    .iInterface         = 0x00, /* Overwrite by USB_Audio1_StreamIf_Create */
};

static usb_audio_dscr_as_general_t audio1_spk_stream_general_dscr = {
    .bLength            = 0x07,
    .bDescriptorType    = 0x24,
    .bDescriptorSubtype = USB_AUDIO_AS_SUBTYPE_GENERAL,
    .bTerminalLink      = 0x01, /* Overwrite by USB_Audio1_StreamIf_Create */
    .bDelay             = 0x01,
    .wFormatTag         = 0x0001, /* PCM */
};

#ifdef USB_AUDIO_SPEAKER_SAMPLESIZE_24BIT
static usb_audio_dscr_as_format_type_t audio1_spk_stream_format_dscr = {
    .bLength            = 0x08 + 3 * 2, /* 8 + freq_size(3) * num_of_freq */
    .bDescriptorType    = 0x24,
    .bDescriptorSubtype = USB_AUDIO_AS_SUBTYPE_FORMAT_TYPE,
    .bFormatType        = 0x01, /* FORMAT_TYPE_I        */
    .bNrChannels        = 0x02, /* 2 channels           */
    .bSubFrameSize      = 0x03, /* 3 bytes per subframe */
    .bBitResolution     = 0x18, /* 24 bits per sample   */
    .bSamFreqType       = 0x02, /* num of tSamFreq      */
    .tSamFreq =
        {
            {.data = {0x80, 0xBB, 0x00}}, /* tSamFreq[1]; 48000 Hz */
            {.data = {0x00, 0x77, 0x01}}, /* tSamFreq[2]; 96000 Hz */
        },
};
#else
static usb_audio_dscr_as_format_type_t audio1_spk_stream_format_dscr = {
    .bLength            = 0x08 + 3 * 1, /* 8 + freq_size(3) * num_of_freq */
    .bDescriptorType    = 0x24,
    .bDescriptorSubtype = USB_AUDIO_AS_SUBTYPE_FORMAT_TYPE,
    .bFormatType        = 0x01, /* FORMAT_TYPE_I        */
    .bNrChannels        = 0x02, /* 2 channels           */
    .bSubFrameSize      = 0x02, /* 2 bytes per subframe */
    .bBitResolution     = 0x10, /* 16 bits per sample   */
    .bSamFreqType       = 0x01, /* num of tSamFreq      */
    .tSamFreq =
        {
            {.data = {0x80, 0xBB, 0x00}}, /* tSamFreq[1]; 48000 Hz */
        },
};
#endif

static usb_audio_dscr_hdlr_t audio1_spk_as_if_dscrs[] = {
    {&audio1_spk_stream_if_dscr, usb_audio_dscr_interface_serialize},
};

static usb_audio_dscr_hdlr_t audio1_spk_as_if_alt_dscrs[] = {
    {&audio1_spk_stream_alt_if_dscr, usb_audio_dscr_interface_serialize},
    {&audio1_spk_stream_general_dscr, usb_audio_dscr_as_general_serialize},
    {&audio1_spk_stream_format_dscr, usb_audio_dscr_as_format_type_serialize},
};

#ifdef MTK_USB_AUDIO_MICROPHONE
static usb_audio_dscr_interface_t audio1_mic_stream_if_dscr = {
    .bLength            = USB_IFDSC_LENGTH,
    .bDescriptorType    = USB_INTERFACE,
    .bInterfaceNumber   = 0x00, /* Overwrite by USB_Audio_StreamIf_Microphone_Create */
    .bAlternateSetting  = 0x00,
    .bNumEndpoints      = 0x00,
    .bInterfaceClass    = 0x01,
    .bInterfaceSubClass = 0x02,
    .bInterfaceProtocol = USB_AUDIO_INTERFACE_PROTOCOL,
    .iInterface         = 0x00, /* Overwrite by USB_Audio_StreamIf_Microphone_Create */
};

static usb_audio_dscr_interface_t audio1_mic_stream_alt_if_dscr = {
    .bLength            = USB_IFDSC_LENGTH,
    .bDescriptorType    = USB_INTERFACE,
    .bInterfaceNumber   = 0x00, /* Overwrite by USB_Audio_StreamIf_Microphone_Create */
    .bAlternateSetting  = 0x01,
    .bNumEndpoints      = 0x01,
    .bInterfaceClass    = 0x01,
    .bInterfaceSubClass = 0x02,
    .bInterfaceProtocol = USB_AUDIO_INTERFACE_PROTOCOL,
    .iInterface         = 0x00, /* Overwrite by USB_Audio_StreamIf_Microphone_Create */
};

static usb_audio_dscr_as_general_t audio1_mic_stream_general_dscr = {
    .bLength            = 0x07,
    .bDescriptorType    = 0x24,
    .bDescriptorSubtype = USB_AUDIO_AS_SUBTYPE_GENERAL,
    .bTerminalLink      = 0x07, /* Overwrite by USB_Audio_StreamIf_Microphone_Create */
    .bDelay             = 0x01,
    .wFormatTag         = 0x0001, /* PCM */
};

static usb_audio_dscr_as_format_type_t audio1_mic_stream_format_dscr = {
    .bLength            = 0x08 + 3 * 2, /* 8 + freq_size(3) * num_of_freq */
    .bDescriptorType    = 0x24,
    .bDescriptorSubtype = USB_AUDIO_AS_SUBTYPE_FORMAT_TYPE,
    .bFormatType        = 0x01, /* FORMAT_TYPE_I        */
    .bNrChannels        = 0x01, /* 1 channels           */
    .bSubFrameSize      = 0x02, /* 2 bytes per subframe */
    .bBitResolution     = 0x10, /* 16 bits per sample   */
    .bSamFreqType       = 0x02, /* num of tSamFreq      */
    .tSamFreq =
        {
            {.data = {0x80, 0xBB, 0x00}}, /* tSamFreq[2]; 48000 Hz */
            {.data = {0x80, 0x3e, 0x00}}, /* tSamFreq[1]; 16000 Hz */
        },
};

static usb_audio_dscr_hdlr_t audio1_mic_as_if_dscrs[] = {
    {&audio1_mic_stream_if_dscr, usb_audio_dscr_interface_serialize},
};

static usb_audio_dscr_hdlr_t audio1_mic_as_if_alt_dscrs[] = {
    {&audio1_mic_stream_alt_if_dscr, usb_audio_dscr_interface_serialize},
    {&audio1_mic_stream_general_dscr, usb_audio_dscr_as_general_serialize},
    {&audio1_mic_stream_format_dscr, usb_audio_dscr_as_format_type_serialize},
};
#endif /* MTK_USB_AUDIO_MICROPHONE */

static usb_audio_dscr_interface_t audio2_spk_stream_if_dscr = {
    .bLength            = USB_IFDSC_LENGTH,
    .bDescriptorType    = USB_INTERFACE,
    .bInterfaceNumber   = 0x00, /* Overwrite by USB_Audio2_StreamIf_Create */
    .bAlternateSetting  = 0x00,
    .bNumEndpoints      = 0x00,
    .bInterfaceClass    = 0x01,
    .bInterfaceSubClass = 0x02,
    .bInterfaceProtocol = USB_AUDIO_INTERFACE_PROTOCOL,
    .iInterface         = 0x00, /* Overwrite by USB_Audio2_StreamIf_Create */
};

static usb_audio_dscr_interface_t audio2_spk_stream_alt_if_dscr = {
    .bLength            = USB_IFDSC_LENGTH,
    .bDescriptorType    = USB_INTERFACE,
    .bInterfaceNumber   = 0x00, /* Overwrite by USB_Audio2_StreamIf_Create */
    .bAlternateSetting  = 0x01,
    .bNumEndpoints      = 0x01,
    .bInterfaceClass    = 0x01,
    .bInterfaceSubClass = 0x02,
    .bInterfaceProtocol = USB_AUDIO_INTERFACE_PROTOCOL,
    .iInterface         = 0x00, /* Overwrite by USB_Audio2_StreamIf_Create */
};

static usb_audio_dscr_as_general_t audio2_spk_stream_general_dscr = {
    .bLength            = 0x07,
    .bDescriptorType    = 0x24,
    .bDescriptorSubtype = USB_AUDIO_AS_SUBTYPE_GENERAL,
    .bTerminalLink      = 0x09, /* Overwrite by USB_Audio2_StreamIf_Create */
    .bDelay             = 0x03,
    .wFormatTag         = 0x0001, /* PCM */
};

#ifdef USB_AUDIO_SPEAKER_SAMPLESIZE_24BIT
static usb_audio_dscr_as_format_type_t audio2_spk_stream_format_dscr = {
    .bLength            = 0x08 + 3 * 2, /* 8 + freq_size(3) * num_of_freq */
    .bDescriptorType    = 0x24,
    .bDescriptorSubtype = USB_AUDIO_AS_SUBTYPE_FORMAT_TYPE,
    .bFormatType        = 0x01, /* FORMAT_TYPE_I        */
    .bNrChannels        = 0x02, /* 2 channels           */
    .bSubFrameSize      = 0x03, /* 2 bytes per subframe */
    .bBitResolution     = 0x18, /* 16 bits per sample   */
    .bSamFreqType       = 0x02, /* num of tSamFreq      */
    .tSamFreq =
        {
            {.data = {0x80, 0xBB, 0x00}}, /* tSamFreq[1]; 48000 Hz */
            {.data = {0x00, 0x77, 0x01}}, /* tSamFreq[2]; 96000 Hz */
        },
};
#else
static usb_audio_dscr_as_format_type_t audio2_spk_stream_format_dscr = {
    .bLength            = 0x08 + 3 * 1, /* 8 + freq_size(3) * num_of_freq */
    .bDescriptorType    = 0x24,
    .bDescriptorSubtype = USB_AUDIO_AS_SUBTYPE_FORMAT_TYPE,
    .bFormatType        = 0x01, /* FORMAT_TYPE_I        */
    .bNrChannels        = 0x02, /* 2 channels           */
    .bSubFrameSize      = 0x02, /* 2 bytes per subframe */
    .bBitResolution     = 0x10, /* 16 bits per sample   */
    .bSamFreqType       = 0x01, /* num of tSamFreq      */
    .tSamFreq =
        {
            {.data = {0x80, 0xBB, 0x00}}, /* tSamFreq[1]; 48000 Hz */
        },
};
#endif

static usb_audio_dscr_hdlr_t audio2_spk_as_if_dscrs[] = {
    {&audio2_spk_stream_if_dscr, usb_audio_dscr_interface_serialize},
};

static usb_audio_dscr_hdlr_t audio2_spk_as_if_alt_dscrs[] = {
    {&audio2_spk_stream_alt_if_dscr, usb_audio_dscr_interface_serialize},
    {&audio2_spk_stream_general_dscr, usb_audio_dscr_as_general_serialize},
    {&audio2_spk_stream_format_dscr, usb_audio_dscr_as_format_type_serialize},
};

/****************************************** Speaker Endpoint Start *********************************************/

const uint8_t audio_stream_ep_out_dscr[] = {
    /* Speaker Usage*/
    0x09,             /*bLength;*/
    USB_ENDPOINT,     /*bDescriptorType;*/
    0x00,             /*bEndpointAddress;*/
    USB_EP_ISO | (USB_ISO_ADAPTIVE << 2), /*bmAttributes;*/
    0x02,             /*wMaxPacketSize[2]; Will be replaced by HAL_USB_MAX_PACKET_SIZE_ENDPOINT_ISOCHRONOUS_HIGH_SPEED &*/
    0x00,             /*wMaxPacketSize[2]; HAL_USB_MAX_PACKET_SIZE_ENDPOINT_ISOCHRONOUS_FULL_SPEED.*/
    0x01,             /*bInterval; Will be changed to 0x04 if high-speed @ USB_Audio_StreamIf_Speed_Reset().*/
    0x00,             /*bRefresh;*/
    0x00,             /*bSynchAddress;*/

    0x07,             /*bLength*/
    0x25,             /*bDescriptorType; Audio Endpoint Descriptor*/
    0x01,             /*bDescriptorSubtype; General*/
    0x01,             /*bmAttributes; Sampling Frequency*/
    0x00,             /*bLockDelayUnits*/
    0x00,             /*wLockDelay; LSB*/
    0x00,             /*wLockDelay; MSB*/
};
/****************************************** Speaker Endpoint End *********************************************/

/****************************************** Microphone Endpoint Start *********************************************/
#ifdef MTK_USB_AUDIO_MICROPHONE
const uint8_t audio_stream_ep_in_dscr[] = { /* microphone usage*/
    0x09,             /*bLength;*/
    USB_ENDPOINT,     /*bDescriptorType;*/
    0x00,             /*bEndpointAddress;*/
    USB_EP_ISO | (USB_ISO_ADAPTIVE << 2), /*bmAttributes;*/
    0x02,             /*wMaxPacketSize[2]; Will be replaced by HAL_USB_MAX_PACKET_SIZE_ENDPOINT_ISOCHRONOUS_HIGH_SPEED &*/
    0x00,             /*wMaxPacketSize[2]; HAL_USB_MAX_PACKET_SIZE_ENDPOINT_ISOCHRONOUS_FULL_SPEED.*/
    0x01,             /*bInterval; Will be changed to 0x04 if high-speed @ USB_Audio_StreamIf_Speed_Reset().*/
    0x00,             /*bRefresh;*/
    0x00,             /*bSynchAddress;*/

    0x07,             /*bLength*/
    0x25,             /*bDescriptorType; Audio Endpoint Descriptor*/
    0x01,             /*bDescriptorSubtype; General*/
    0x01,             /*bmAttributes; Sampling Frequency*/
    0x00,             /*bLockDelayUnits*/
    0x00,             /*wLockDelay; LSB*/
    0x00,             /*wLockDelay; MSB*/
};
#endif
/******************************************Microphone Endpoint end*********************************************/

static unsigned int USB_Audio_Stream_Speed_Reset_Packet_Size(uint32_t port, bool b_other_speed)
{
    unsigned int  max_packet;

#if 0
    if (hal_usb_is_high_speed() == true) {

        if (b_other_speed == false) {
            max_packet = HAL_USB_MAX_PACKET_SIZE_ENDPOINT_ISOCHRONOUS_HIGH_SPEED;
        } else {
            max_packet = HAL_USB_MAX_PACKET_SIZE_ENDPOINT_ISOCHRONOUS_FULL_SPEED;
        }
    } else {
        if (b_other_speed == false) {
            max_packet = HAL_USB_MAX_PACKET_SIZE_ENDPOINT_ISOCHRONOUS_FULL_SPEED;
        } else {
            max_packet = HAL_USB_MAX_PACKET_SIZE_ENDPOINT_ISOCHRONOUS_HIGH_SPEED;
        }
    }
#endif

    /**
     * NOTE
     * Each endpoint cost some bus bandwidth. The total cost of each
     * endpoint may not over 100%. If over 100%, Windows will show
     * "Not enough USB controller resources", and usb device can't
     * normally work.
     *
     * The isochronous transaction cost persent could refers to
     * USB2.0 spec 5.6.5.
     */
    max_packet = USB_AUDIO_RX_EP_MAX_SIZE;

    return max_packet;
}

static unsigned int USB_Audio_Stream_Microphone_Speed_Reset_Packet_Size(uint32_t port, bool b_other_speed)
{
    unsigned int  max_packet;

    max_packet = USB_AUDIO_TX_EP_MAX_SIZE;

    return max_packet;
}

/************************************************************
    USB Audio Get/Set interface and endpoint ID
*************************************************************/
uint8_t audio1_rx_if_id, audio1_rx_ep_id;
uint8_t audio1_tx_if_id, audio1_tx_ep_id;
uint8_t audio2_rx_if_id, audio2_rx_ep_id;

void USB_Audio_Init_Chat_Game_Info()
{
    audio1_rx_if_id = USB_AUDIO_UNUSED_ID;
    audio1_rx_ep_id = USB_AUDIO_UNUSED_ID;
    audio1_tx_if_id = USB_AUDIO_UNUSED_ID;
    audio1_tx_ep_id = USB_AUDIO_UNUSED_ID;
    audio2_rx_if_id = USB_AUDIO_UNUSED_ID;
    audio2_rx_ep_id = USB_AUDIO_UNUSED_ID;
}

void USB_Audio_Get_Chat_Info(uint8_t *rx_if_id, uint8_t *rx_ep_id, uint8_t *tx_if_id, uint8_t *tx_ep_id)
{
    *rx_if_id = audio1_rx_if_id;
    *rx_ep_id = audio1_rx_ep_id;
    *tx_if_id = audio1_tx_if_id;
    *tx_ep_id = audio1_tx_ep_id;
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_Get_Chat_Info rx_if_id[%x] rx_ep_id[%x] tx_if_id[%x] tx_ep_id[%x]", 4,
                *rx_if_id, *rx_ep_id, *tx_if_id, *tx_ep_id);
}

void USB_Audio_Get_Game_Info(uint8_t *rx_if_id, uint8_t *rx_ep_id)
{
    *rx_if_id = audio2_rx_if_id;
    *rx_ep_id = audio2_rx_ep_id;
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_Get_Game_Info rx_if_id[%x] rx_ep_id[%x]", 2, *rx_if_id, *rx_ep_id);
}

void USB_Audio_Set_Chat_Info(uint8_t rx_if_id, uint8_t rx_ep_id, uint8_t tx_if_id, uint8_t tx_ep_id)
{
    if (rx_if_id != USB_AUDIO_UNUSED_ID) {
        audio1_rx_if_id = rx_if_id;
    }
    if (rx_ep_id != USB_AUDIO_UNUSED_ID) {
        audio1_rx_ep_id = rx_ep_id;
    }
    if (tx_if_id != USB_AUDIO_UNUSED_ID) {
        audio1_tx_if_id = tx_if_id;
    }
    if (tx_ep_id != USB_AUDIO_UNUSED_ID) {
        audio1_tx_ep_id = tx_ep_id;
    }
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_Set_Chat_Info rx_if_id[%x] rx_ep_id[%x] tx_if_id[%x] tx_ep_id[%x]", 4,
                audio1_rx_if_id, audio1_rx_ep_id, audio1_tx_if_id, audio1_tx_ep_id);
}

void USB_Audio_Set_Game_Info(uint8_t rx_if_id, uint8_t rx_ep_id)
{
    if (rx_if_id != USB_AUDIO_UNUSED_ID) {
        audio2_rx_if_id = rx_if_id;
    }
    if (rx_ep_id != USB_AUDIO_UNUSED_ID) {
        audio2_rx_ep_id = rx_ep_id;
    }
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_Set_Game_Info rx_if_id[%x] rx_ep_id[%x]", 2, audio2_rx_if_id, audio2_rx_ep_id);
}


/************************************************************
    Interface initialization functions
*************************************************************/
uint8_t USB_Audio_Get_StreamIf_Num(uint8_t port)
{
    uint8_t if_id;

    if (g_UsbAudio[port].stream_interface_id == 0xFF) {
        /* Get resource number and register to gUsbDevice */
        g_UsbAudio[port].stream_if_info = USB_Get_Interface_Number(&if_id);
        g_UsbAudio[port].stream_interface_id = if_id;
    }

    return g_UsbAudio[port].stream_interface_id;
}


/************************************************************
    Interface initialization functions
*************************************************************/
/* Audio control interface create function, prepare descriptor */
void USB_Audio_ControlIf_Create(uint32_t port, void *ifname)
{
}

void USB_Audio1_ControlIf_Create(void *ifname)
{
    bool spk_en;
    bool mic_en;
    uint8_t ac_if_id;
    uint8_t spk_if_id;
    uint8_t spk_ep_id;
    uint8_t mic_if_id;
    uint8_t mic_ep_id;
    int16_t ach_length = 0;
    int16_t len        = 0;
    uint8_t *dscr_out;

    g_UsbAudio[0].spk_feature_en = true;
    g_UsbAudio[0].mic_feature_en = true;
    spk_en                       = g_UsbAudio[0].spk_feature_en;
    mic_en                       = g_UsbAudio[0].mic_feature_en;

    g_UsbAudio[0].control_if_info      = USB_Get_Interface_Number(&ac_if_id);
    g_UsbAudio[0].control_interface_id = ac_if_id;

    if (spk_en) {
        g_UsbAudio[0].stream_if_info      = USB_Get_Interface_Number(&spk_if_id);
        g_UsbAudio[0].stream_interface_id = spk_if_id;
        g_UsbAudio[0].stream_ep_out_info  = USB_Get_Iso_Rx_Ep(&spk_ep_id);
        g_UsbAudio[0].stream_ep_out_id    = spk_ep_id;
    }

    if (mic_en) {
        g_UsbAudio[0].stream_microphone_if_info      = USB_Get_Interface_Number(&mic_if_id);
        g_UsbAudio[0].stream_microphone_interface_id = mic_if_id;
        g_UsbAudio[0].stream_ep_in_info              = USB_Get_Iso_Tx_Ep(&mic_ep_id);
        g_UsbAudio[0].stream_ep_in_id                = mic_ep_id;
    }

    audio1_interface_dscr.bInterfaceNumber = g_UsbAudio[0].control_interface_id;
    audio1_interface_dscr.iInterface       = USB_String_Get_Id_By_Usage(USB_STRING_USAGE_AUDIO1);
    audio1_spk_ot_dscr.wTerminalType       = g_UsbAudio[0].spk_terminal_type;
    audio1_mic_it_dscr.wTerminalType       = g_UsbAudio[0].mic_terminal_type;

    audio1_ach_dscr.bInCollection = 0;
    if (spk_en) {
        audio1_ach_dscr.baInterfaceNr[0] = g_UsbAudio[0].stream_interface_id;
        audio1_ach_dscr.bInCollection++;
    }
    if (mic_en) {
        audio1_ach_dscr.baInterfaceNr[1] = g_UsbAudio[0].stream_microphone_interface_id;
        audio1_ach_dscr.bInCollection++;
    }
    audio1_ach_dscr.bLength = 8 + audio1_ach_dscr.bInCollection;

    /* calculate total length of AC Header */
    ach_length += audio1_ach_dscr.bLength;
    if (spk_en) {
        for (int i = 0; i < sizeof(audio1_spk_dscrs) / sizeof(usb_audio_dscr_hdlr_t); i++) {
            /* offset 0 is always length of dscr */
            ach_length += ((uint8_t *)(audio1_spk_dscrs[i].dscr))[0];
        }
    }
    if (mic_en) {
        for (int i = 0; i < sizeof(audio1_mic_dscrs) / sizeof(usb_audio_dscr_hdlr_t); i++) {
            /* offset 0 is always length of dscr */
            ach_length += ((uint8_t *)(audio1_mic_dscrs[i].dscr))[0];
        }
    }
    audio1_ach_dscr.wTotalLength = ach_length;

    /* write dscr to usb interface struct */
    dscr_out = g_UsbAudio[0].control_if_info->ifdscr.classif;
    len += dscr_list_serialize(audio1_aci_dscrs, sizeof(audio1_aci_dscrs) / sizeof(usb_audio_dscr_hdlr_t), dscr_out + len, 1024);
    if (spk_en) {
        len += dscr_list_serialize(audio1_spk_dscrs, sizeof(audio1_spk_dscrs) / sizeof(usb_audio_dscr_hdlr_t), dscr_out + len, 1024);
    }
    if (mic_en) {
        len += dscr_list_serialize(audio1_mic_dscrs, sizeof(audio1_mic_dscrs) / sizeof(usb_audio_dscr_hdlr_t), dscr_out + len, 1024);
    }
    g_UsbAudio[0].control_if_info->ifdscr_size = len;

    /* check if length if correct. If not, assert! */
    if (len != audio1_interface_dscr.bLength + audio1_ach_dscr.wTotalLength) {
        LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio1_ControlIf_Create, the length of descriptor is error[%d]!=[%d], assert!",
                    2, len, audio1_interface_dscr.bLength + audio1_ach_dscr.wTotalLength);
        assert(0);
    }

    /* register interface callback */
    g_UsbAudio[0].control_if_info->if_class_specific_hdlr = USB_Audio1_Ep0_Command;
}

void USB_Audio2_ControlIf_Create(void *ifname)
{
    bool spk_en;
    bool mic_en;
    uint8_t ac_if_id;
    uint8_t spk_if_id;
    uint8_t spk_ep_id;
    uint8_t mic_if_id;
    uint8_t mic_ep_id;
    int16_t ach_length = 0;
    int16_t len        = 0;
    void *dscr_out;

    g_UsbAudio[1].spk_feature_en = true;
    g_UsbAudio[1].mic_feature_en = false;
    spk_en                       = g_UsbAudio[1].spk_feature_en;
    mic_en                       = g_UsbAudio[1].mic_feature_en;

    g_UsbAudio[1].control_if_info      = USB_Get_Interface_Number(&ac_if_id);
    g_UsbAudio[1].control_interface_id = ac_if_id;

    if (spk_en) {
        g_UsbAudio[1].stream_if_info      = USB_Get_Interface_Number(&spk_if_id);
        g_UsbAudio[1].stream_interface_id = spk_if_id;
        g_UsbAudio[1].stream_ep_out_info  = USB_Get_Iso_Rx_Ep(&spk_ep_id);
        g_UsbAudio[1].stream_ep_out_id    = spk_ep_id;
    }

    if (mic_en) {
        g_UsbAudio[1].stream_microphone_if_info      = USB_Get_Interface_Number(&mic_if_id);
        g_UsbAudio[1].stream_microphone_interface_id = mic_if_id;
        g_UsbAudio[1].stream_ep_in_info              = USB_Get_Iso_Tx_Ep(&mic_ep_id);
        g_UsbAudio[1].stream_ep_in_id                = mic_ep_id;
    }

    audio2_interface_dscr.bInterfaceNumber = g_UsbAudio[1].control_interface_id;
    audio2_interface_dscr.iInterface       = USB_String_Get_Id_By_Usage(USB_STRING_USAGE_AUDIO2);
    audio2_spk_ot_dscr.wTerminalType       = g_UsbAudio[1].spk_terminal_type;

    audio2_ach_dscr.bInCollection = 0;
    if (spk_en) {
        audio2_ach_dscr.baInterfaceNr[0] = g_UsbAudio[1].stream_interface_id;
        audio2_ach_dscr.bInCollection++;
    }
    if (mic_en) {
        audio2_ach_dscr.baInterfaceNr[1] = g_UsbAudio[1].stream_microphone_interface_id;
        audio2_ach_dscr.bInCollection++;
    }
    audio2_ach_dscr.bLength = 8 + audio2_ach_dscr.bInCollection;

    /* calculate total length of audio2 AC Header */
    ach_length += audio2_ach_dscr.bLength;
    if (spk_en) {
        for (int i = 0; i < sizeof(audio2_spk_dscrs) / sizeof(usb_audio_dscr_hdlr_t); i++) {
            /* offset 0 is always length of dscr */
            ach_length += ((uint8_t *)(audio2_spk_dscrs[i].dscr))[0];
        }
    }
    if (mic_en) {
        for (int i = 0; i < sizeof(audio2_mic_dscrs) / sizeof(usb_audio_dscr_hdlr_t); i++) {
            /* offset 0 is always length of dscr */
            ach_length += ((uint8_t *)(audio2_mic_dscrs[i].dscr))[0];
        }
    }
    audio2_ach_dscr.wTotalLength = ach_length;

    /* write dscr to usb interface struct */
    dscr_out = g_UsbAudio[1].control_if_info->ifdscr.classif;
    len += dscr_list_serialize(audio2_aci_dscrs, sizeof(audio2_aci_dscrs) / sizeof(usb_audio_dscr_hdlr_t), dscr_out + len, 1024);
    if (spk_en) {
        len += dscr_list_serialize(audio2_spk_dscrs, sizeof(audio2_spk_dscrs) / sizeof(usb_audio_dscr_hdlr_t), dscr_out + len, 1024);
    }
    if (mic_en) {
        len += dscr_list_serialize(audio2_mic_dscrs, sizeof(audio2_mic_dscrs) / sizeof(usb_audio_dscr_hdlr_t), dscr_out + len, 1024);
    }
    g_UsbAudio[1].control_if_info->ifdscr_size = len;

    /* check if length if correct. If not, assert! */
    if (len != audio2_interface_dscr.bLength + audio2_ach_dscr.wTotalLength) {
        LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio2_ControlIf_Create, the length of descriptor is error[%d]!=[%d], assert!",
                    2, len, audio2_interface_dscr.bLength + audio2_ach_dscr.wTotalLength);
        assert(0);
    }

    /* register interface callback */
    g_UsbAudio[1].control_if_info->if_class_specific_hdlr = USB_Audio2_Ep0_Command;
}

void USB_Audio_ControlIf_Reset(uint32_t port)
{
}

void USB_Audio1_ControlIf_Reset(void)
{
}

void USB_Audio2_ControlIf_Reset(void)
{
}

void USB_Audio_ControlIf_Enable(uint32_t port)
{
}

void USB_Audio1_ControlIf_Enable(void)
{
}
void USB_Audio2_ControlIf_Enable(void)
{
}

/* Audio stream interface speed reset function, enable EP's speed-specific descriptor */
void USB_Audio_ControlIf_Speed_Reset(uint32_t port, bool b_other_speed)
{
}

void USB_Audio1_ControlIf_Speed_Reset(bool b_other_speed)
{
}

void USB_Audio2_ControlIf_Speed_Reset(bool b_other_speed)
{
}

/* Audio stream interface create function, prepare descriptor */
void USB_Audio_StreamIf_Create(uint32_t port, void *ifname)
{
}

void USB_Audio1_StreamIf_Create(void *ifname)
{
    uint8_t if_id, ep_rx_id;
    uint8_t port = 0;
    uint8_t *dscr_out;
    int16_t len;

    if(g_UsbAudio[port].spk_feature_en == false) {
        return;
    }

    if_id = g_UsbAudio[port].stream_interface_id;
    ep_rx_id = g_UsbAudio[port].stream_ep_out_id;

    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_StreamIf_Create port[%x] if_id[%x] ep_rx_id[%x]", 3,
                port, g_UsbAudio[port].stream_interface_id, g_UsbAudio[port].stream_ep_out_id);

    audio1_spk_stream_if_dscr.bInterfaceNumber = if_id;
    audio1_spk_stream_alt_if_dscr.bInterfaceNumber = if_id;
    audio1_spk_stream_general_dscr.bTerminalLink = audio1_spk_it_dscr.bTerminalID;

    /* generate audio stream interface */
    g_UsbAudio[port].stream_if_info->interface_name_ptr = (char *)ifname;

    dscr_out = g_UsbAudio[port].stream_if_info->ifdscr.classif;
    len = dscr_list_serialize(audio1_spk_as_if_dscrs, sizeof(audio1_spk_as_if_dscrs) / sizeof(usb_audio_dscr_hdlr_t), dscr_out, USB_MAX_IFDSC_LENGTH);
    g_UsbAudio[port].stream_if_info->ifdscr_size = len;

    /* generate audio stream alternate interface */
    dscr_out = g_UsbAudio[port].stream_if_info->alternate_if_info[0].ifdscr.classif;
    len      = dscr_list_serialize(audio1_spk_as_if_alt_dscrs, sizeof(audio1_spk_as_if_alt_dscrs) / sizeof(usb_audio_dscr_hdlr_t), dscr_out, USB_MAX_IFDSC_LENGTH);
    g_UsbAudio[port].stream_if_info->alternate_if_info[0].ifdscr_size = len;

    /* generate ep of alternate interface */
    g_UsbAudio[port].stream_if_info->alternate_if_info[0].ep_info[0] = g_UsbAudio[port].stream_ep_out_info;
    g_UsbAudio[port].stream_if_info->alternate_if_info[0].ep_info[0]->epdscr_size = sizeof(audio_stream_ep_out_dscr);
    memcpy((uint32_t *) & (g_UsbAudio[port].stream_ep_out_info->epdesc.stdep), audio_stream_ep_out_dscr, sizeof(audio_stream_ep_out_dscr));

    g_UsbAudio[port].stream_if_info->if_class_specific_hdlr = USB_Audio1_Ep0_Command; /* Command: "SetCur" for "Sampling freq".*/

    /* RX Endpoint handler */
    g_UsbAudio[port].stream_ep_out_info->ep_reset = USB_Audio1_IsoOut_Reset;
    hal_usb_register_driver_callback(HAL_USB_DRV_HDLR_EP_RX, ep_rx_id, USB_Audio1_IsoOut_Hdr);

    /* Endpoint descriptor */
    g_UsbAudio[port].stream_ep_out_info->epdesc.stdep.bEndpointAddress = (USB_EP_DIR_OUT | ep_rx_id); /*OutPipe*/
    g_UsbAudio[port].stream_ep_out_info->ep_status.epout_status.byEP = ep_rx_id;

    /* Ep_id include direction */
    if (port == 0) {
        USB_Audio_Set_Chat_Info(if_id, (USB_EP_DIR_OUT | ep_rx_id), USB_AUDIO_UNUSED_ID, USB_AUDIO_UNUSED_ID);
    } else {
        USB_Audio_Set_Game_Info(if_id, (USB_EP_DIR_OUT | ep_rx_id));
    }

    if (g_UsbAudio[port].rx_dma_buffer == NULL) {
        LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_StreamIf_Create port %d rx_dma_buffer size=%d", 2, port, USB_AUDIO_RX_BUFFER_SIZE);
        g_UsbAudio[port].rx_dma_buffer = (uint8_t *)USB_Get_Memory(USB_AUDIO_RX_BUFFER_SIZE);
        g_UsbAudio[port].rx_dma_buffer_len = USB_AUDIO_RX_BUFFER_SIZE;
    }

    hal_usb_get_dma_channel(0, ep_rx_id, HAL_USB_EP_DIRECTION_RX, false);
}

void USB_Audio2_StreamIf_Create(void *ifname)
{
    uint8_t if_id, ep_rx_id;
    uint8_t port = 1;
    uint8_t *dscr_out;
    int16_t len;

    if(g_UsbAudio[port].spk_feature_en == false) {
        return;
    }

    if_id = g_UsbAudio[port].stream_interface_id;
    ep_rx_id = g_UsbAudio[port].stream_ep_out_id;

    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio2_StreamIf_Create port[%x] if_id[%x] ep_rx_id[%x]", 3,
                port, g_UsbAudio[port].stream_interface_id, g_UsbAudio[port].stream_ep_out_id);

    audio2_spk_stream_if_dscr.bInterfaceNumber = if_id;
    audio2_spk_stream_alt_if_dscr.bInterfaceNumber = if_id;
    audio2_spk_stream_general_dscr.bTerminalLink = audio2_spk_it_dscr.bTerminalID;

    /* generate audio stream interface */
    g_UsbAudio[port].stream_if_info->interface_name_ptr = (char *)ifname;

    dscr_out = g_UsbAudio[port].stream_if_info->ifdscr.classif;
    len = dscr_list_serialize(audio2_spk_as_if_dscrs, sizeof(audio2_spk_as_if_dscrs) / sizeof(usb_audio_dscr_hdlr_t), dscr_out, USB_MAX_IFDSC_LENGTH);
    g_UsbAudio[port].stream_if_info->ifdscr_size = len;

    /* generate audio stream alternate interface */
    dscr_out = g_UsbAudio[port].stream_if_info->alternate_if_info[0].ifdscr.classif;
    len      = dscr_list_serialize(audio2_spk_as_if_alt_dscrs, sizeof(audio2_spk_as_if_alt_dscrs) / sizeof(usb_audio_dscr_hdlr_t), dscr_out, USB_MAX_IFDSC_LENGTH);
    g_UsbAudio[port].stream_if_info->alternate_if_info[0].ifdscr_size = len;

    /* generate ep of alternate interface */
    g_UsbAudio[port].stream_if_info->alternate_if_info[0].ep_info[0] = g_UsbAudio[port].stream_ep_out_info;
    g_UsbAudio[port].stream_if_info->alternate_if_info[0].ep_info[0]->epdscr_size = sizeof(audio_stream_ep_out_dscr);
    memcpy((uint32_t *) & (g_UsbAudio[port].stream_ep_out_info->epdesc.stdep), audio_stream_ep_out_dscr, sizeof(audio_stream_ep_out_dscr));

    g_UsbAudio[port].stream_if_info->if_class_specific_hdlr = USB_Audio2_Ep0_Command; /* Command: "SetCur" for "Sampling freq".*/

    /* RX Endpoint handler */
    g_UsbAudio[port].stream_ep_out_info->ep_reset = USB_Audio2_IsoOut_Reset;
    hal_usb_register_driver_callback(HAL_USB_DRV_HDLR_EP_RX, ep_rx_id, USB_Audio2_IsoOut_Hdr);

    /* Endpoint descriptor */
    g_UsbAudio[port].stream_ep_out_info->epdesc.stdep.bEndpointAddress = (USB_EP_DIR_OUT | ep_rx_id); /*OutPipe*/
    g_UsbAudio[port].stream_ep_out_info->ep_status.epout_status.byEP = ep_rx_id;

    /* Ep_id include direction */
    if (port == 0) {
        USB_Audio_Set_Chat_Info(if_id, (USB_EP_DIR_OUT | ep_rx_id), USB_AUDIO_UNUSED_ID, USB_AUDIO_UNUSED_ID);
    } else {
        USB_Audio_Set_Game_Info(if_id, (USB_EP_DIR_OUT | ep_rx_id));
    }

    if (g_UsbAudio[port].rx_dma_buffer == NULL) {
        LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_StreamIf_Create port %d rx_dma_buffer size=%d", 2, port, USB_AUDIO_RX_BUFFER_SIZE);
        g_UsbAudio[port].rx_dma_buffer = (uint8_t *)USB_Get_Memory(USB_AUDIO_RX_BUFFER_SIZE);
        g_UsbAudio[port].rx_dma_buffer_len = USB_AUDIO_RX_BUFFER_SIZE;
    }

    hal_usb_get_dma_channel(0, ep_rx_id, HAL_USB_EP_DIRECTION_RX, false);
}

/* Audio stream interface reset function, enable EP */
void USB_Audio_StreamIf_Reset(uint32_t port)
{
    if(g_UsbAudio[port].spk_feature_en == false) {
        return;
    }
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_StreamIf_Reset, port:%d", 1, port);

    g_UsbAudio[port].rxpipe = &g_UsbAudio[port].stream_ep_out_info->ep_status.epout_status;

    /* Stop DMA */
    hal_usb_stop_dma_channel(g_UsbAudio[port].rxpipe->byEP, HAL_USB_EP_DIRECTION_RX);
}

void USB_Audio1_StreamIf_Reset(void)
{
    USB_Audio_StreamIf_Reset(0);
}
void USB_Audio2_StreamIf_Reset(void)
{
    USB_Audio_StreamIf_Reset(1);
}

void USB_Audio_StreamIf_Enable(uint32_t port)
{
    if(g_UsbAudio[port].spk_feature_en == false) {
        return;
    }
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_StreamIf_Enable port %d", 1, port);

    /* Stop DMA */
    /* Compliance test tool will set configuration , but no reset */
    hal_usb_stop_dma_channel(g_UsbAudio[port].rxpipe->byEP, HAL_USB_EP_DIRECTION_RX);

    /*Non-DMA*/
    hal_usb_enable_rx_endpoint(g_UsbAudio[port].rxpipe->byEP, HAL_USB_EP_TRANSFER_ISO, HAL_USB_EP_USE_NO_DMA, true);
}
void USB_Audio1_StreamIf_Enable(void)
{
    USB_Audio_StreamIf_Enable(0);
}

void USB_Audio2_StreamIf_Enable(void)
{
    USB_Audio_StreamIf_Enable(1);
}


/* Audio stream interface speed reset function, enable EP's speed-specific descriptor */
void USB_Audio_StreamIf_Speed_Reset(uint32_t port, bool b_other_speed)
{
    uint32_t temp_max_size;

    if(g_UsbAudio[port].spk_feature_en == false) {
        return;
    }
    temp_max_size = USB_Audio_Stream_Speed_Reset_Packet_Size(port, (bool)b_other_speed);

    g_UsbAudio[port].stream_ep_out_info->epdesc.stdep.wMaxPacketSize[0] = temp_max_size & 0xff;
    g_UsbAudio[port].stream_ep_out_info->epdesc.stdep.wMaxPacketSize[1] = (temp_max_size >> 8) & 0xff;

    if (hal_usb_is_high_speed() == true) {
        g_UsbAudio[port].stream_ep_out_info->epdesc.stdep.bInterval = 4; /* (2^(4-1))*0.125us=1ms*/
    } else {
        g_UsbAudio[port].stream_ep_out_info->epdesc.stdep.bInterval = 1; /* (2^(1-1))*1ms=1ms*/
    }

    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_StreamIf_Speed_Reset port %d wMaxPacketSize=%d b_other_speed=%d", 3, port, temp_max_size, b_other_speed);
}

void USB_Audio1_StreamIf_Speed_Reset(bool b_other_speed)
{
    if(g_UsbAudio[0].spk_feature_en == false) {
        return;
    }
    USB_Audio_StreamIf_Speed_Reset(0, b_other_speed);
}

void USB_Audio2_StreamIf_Speed_Reset(bool b_other_speed)
{
    if(g_UsbAudio[1].spk_feature_en == false) {
        return;
    }
    USB_Audio_StreamIf_Speed_Reset(1, b_other_speed);
}

#ifdef MTK_USB_AUDIO_MICROPHONE
/* Audio stream interface create function, prepare descriptor */
void USB_Audio_StreamIf_Microphone_Create(void *ifname)
{
    uint8_t if_id, ep_tx_id;
    uint8_t port = 0;
    uint8_t *dscr_out;
    int16_t len;

    if(g_UsbAudio[port].mic_feature_en == false) {
        return;
    }

    if_id = g_UsbAudio[port].stream_microphone_interface_id;
    ep_tx_id = g_UsbAudio[port].stream_ep_in_id;

    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_StreamIf_Microphone_Create port[%x] if_id[%x] ep_tx_id[%x]", 3,
                port, g_UsbAudio[port].stream_microphone_interface_id, g_UsbAudio[port].stream_ep_in_id);

    audio1_mic_stream_if_dscr.bInterfaceNumber = if_id;
    audio1_mic_stream_alt_if_dscr.bInterfaceNumber = if_id;
    audio1_mic_stream_general_dscr.bTerminalLink = audio1_mic_ot_dscr.bTerminalID;

    /* generate audio stream interface */
    g_UsbAudio[port].stream_microphone_if_info->interface_name_ptr = (char *)ifname;

    dscr_out = g_UsbAudio[port].stream_microphone_if_info->ifdscr.classif;
    len = dscr_list_serialize(audio1_mic_as_if_dscrs, sizeof(audio1_mic_as_if_dscrs) / sizeof(usb_audio_dscr_hdlr_t), dscr_out, USB_MAX_IFDSC_LENGTH);
    g_UsbAudio[port].stream_microphone_if_info->ifdscr_size = len;

    /* generate audio stream alternate interface */
    dscr_out = g_UsbAudio[port].stream_microphone_if_info->alternate_if_info[0].ifdscr.classif;
    len      = dscr_list_serialize(audio1_mic_as_if_alt_dscrs, sizeof(audio1_mic_as_if_alt_dscrs) / sizeof(usb_audio_dscr_hdlr_t), dscr_out, USB_MAX_IFDSC_LENGTH);
    g_UsbAudio[port].stream_microphone_if_info->alternate_if_info[0].ifdscr_size = len;

    /* generate ep of alternate interface */
    g_UsbAudio[port].stream_microphone_if_info->alternate_if_info[0].ep_info[0] = g_UsbAudio[port].stream_ep_in_info;
    g_UsbAudio[port].stream_microphone_if_info->alternate_if_info[0].ep_info[0]->epdscr_size = sizeof(audio_stream_ep_in_dscr);
    memcpy((uint32_t *) & (g_UsbAudio[port].stream_ep_in_info->epdesc.stdep), audio_stream_ep_in_dscr, sizeof(audio_stream_ep_in_dscr));

    g_UsbAudio[port].stream_microphone_if_info->if_class_specific_hdlr = USB_Audio1_Ep0_Command; /* Command: "SetCur" for "Sampling freq".*/

    /* TX Endpoint handler */
    g_UsbAudio[port].stream_ep_in_info->ep_reset = USB_Audio_IsoIn_Reset;
    hal_usb_register_driver_callback(HAL_USB_DRV_HDLR_EP_TX, ep_tx_id, USB_Audio_IsoIn_Hdr);

    /* Endpoint descriptor */
    g_UsbAudio[port].stream_ep_in_info->epdesc.stdep.bEndpointAddress = (USB_EP_DIR_IN | ep_tx_id); /*InPipe*/
    g_UsbAudio[port].stream_ep_in_info->ep_status.epout_status.byEP = ep_tx_id;

    /* Ep_id include direction */
    USB_Audio_Set_Chat_Info(USB_AUDIO_UNUSED_ID, USB_AUDIO_UNUSED_ID, if_id, (USB_EP_DIR_IN | ep_tx_id));

    if (g_UsbAudio[port].tx_dma_buffer == NULL) {
        LOG_MSGID_I(USBAUDIO_DRV, "tx_dma_buffer size=%d", 1, USB_AUDIO_TX_BUFFER_SIZE);
        g_UsbAudio[port].tx_dma_buffer = (uint8_t *)USB_Get_Memory(USB_AUDIO_TX_BUFFER_SIZE);
        g_UsbAudio[port].tx_dma_buffer_len = USB_AUDIO_TX_BUFFER_SIZE;
    }

    hal_usb_get_dma_channel(ep_tx_id, 0, HAL_USB_EP_DIRECTION_TX, false);
}

/* Audio stream interface reset function, enable EP */
void USB_Audio_StreamIf_Microphone_Reset(void)
{
    uint32_t port = 0;

    if(g_UsbAudio[port].mic_feature_en == false) {
        return;
    }

    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_StreamIf_Microphone_Reset", 0);

    g_UsbAudio[port].txpipe = &g_UsbAudio[port].stream_ep_in_info->ep_status.epout_status;
    hal_usb_stop_dma_channel(g_UsbAudio[port].txpipe->byEP, HAL_USB_EP_DIRECTION_TX);
}

void USB_Audio_StreamIf_Microphone_Enable(void)
{
    uint32_t port = 0;

    if(g_UsbAudio[port].mic_feature_en == false) {
        return;
    }

    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_StreamIf_Microphone_Enable", 0);

    hal_usb_stop_dma_channel(g_UsbAudio[port].txpipe->byEP, HAL_USB_EP_DIRECTION_TX);
    hal_usb_enable_tx_endpoint(g_UsbAudio[port].txpipe->byEP, HAL_USB_EP_TRANSFER_ISO, HAL_USB_EP_USE_NO_DMA, true);
}


/* Audio stream interface speed reset function, enable EP's speed-specific descriptor */
void USB_Audio_StreamIf_Microphone_Speed_Reset(bool b_other_speed)
{
    uint32_t port = 0;

    if(g_UsbAudio[port].mic_feature_en == false) {
        return;
    }

    uint32_t temp_max_size;
    temp_max_size = USB_Audio_Stream_Microphone_Speed_Reset_Packet_Size(port, (bool)b_other_speed);

    g_UsbAudio[port].stream_ep_in_info->epdesc.stdep.wMaxPacketSize[0] = temp_max_size & 0xff;
    g_UsbAudio[port].stream_ep_in_info->epdesc.stdep.wMaxPacketSize[1] = (temp_max_size >> 8) & 0xff;

    if (hal_usb_is_high_speed() == true) {
        g_UsbAudio[port].stream_ep_in_info->epdesc.stdep.bInterval = 4; /* (2^(4-1))*0.125us=1ms*/
    } else {
        g_UsbAudio[port].stream_ep_in_info->epdesc.stdep.bInterval = 1; /* (2^(1-1))*1ms=1ms*/
    }

    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_StreamIf_Speed_Microphone_Reset() wMaxPacketSize=%d b_other_speed=%d", 2, temp_max_size, b_other_speed);
}
#endif /* MTK_USB_AUDIO_MICROPHONE*/

/*********************************************************************************************
    global variable g_UsbAudio[port] initialize and release functions
*********************************************************************************************/
/* USB Audio 1 : Speaker */
bool g_usb_audio_spk1_enable = 0;
bool g_usb_audio_spk1_status = 0;

/* USB Audio 2 : Speaker */
bool g_usb_audio_spk2_feature= 0;
bool g_usb_audio_spk2_enable = 0;
bool g_usb_audio_spk2_status = 0;

/* Get USB Audio setting from speaker or microphone */
/* USB Audio 1 : Speaker */
uint16_t USB_Audio_Get_Spk1_Cur()
{
    return g_UsbAudio[0].spk_cur;
}

uint16_t USB_Audio_Get_Spk1_Min()
{
    return g_UsbAudio[0].spk_min;
}

uint16_t USB_Audio_Get_Spk1_Max()
{
    return g_UsbAudio[0].spk_max;
}

uint16_t USB_Audio_Get_Spk1_Res()
{
    return g_UsbAudio[0].spk_res;
}

/* USB Audio 1 : Microphone */
uint16_t USB_Audio_Get_Mic1_Cur()
{
    return g_UsbAudio[0].mic_cur;
}

uint16_t USB_Audio_Get_Mic1_Min()
{
    return g_UsbAudio[0].mic_min;
}

uint16_t USB_Audio_Get_Mic1_Max()
{
    return g_UsbAudio[0].mic_max;
}

uint16_t USB_Audio_Get_Mic1_Res()
{
    return g_UsbAudio[0].mic_res;
}

/* USB Audio 2 : Speaker */
uint16_t USB_Audio_Get_Spk2_Cur()
{
    return g_UsbAudio[1].spk_cur;
}

uint16_t USB_Audio_Get_Spk2_Min()
{
    return g_UsbAudio[1].spk_min;
}

uint16_t USB_Audio_Get_Spk2_Max()
{
    return g_UsbAudio[1].spk_max;
}

uint16_t USB_Audio_Get_Spk2_Res()
{
    return g_UsbAudio[1].spk_res;
}

/* Set up USB Audio setting from NVkey or by default */
void USB_Audio_Setting_By_NVKey()
{
    nvkey_status_t nvkey_status;
    uint32_t nvkey_size;
    usb_nvkey_audio_device_cfg_t audio_device_cfg;

    /* USB Audio 1 Speaker */
    nvkey_size   = sizeof(audio_device_cfg);
    nvkey_status = nvkey_read_data(NVID_USB_SETTING_1, (uint8_t *)&audio_device_cfg, &nvkey_size);
    if (nvkey_status == NVKEY_STATUS_OK) {
        g_UsbAudio[0].spk_terminal_type = audio_device_cfg.terminal_type;
        g_UsbAudio[0].spk_cur           = audio_device_cfg.cur;
        g_UsbAudio[0].spk_min           = audio_device_cfg.min;
        g_UsbAudio[0].spk_max           = audio_device_cfg.max;
        g_UsbAudio[0].spk_res           = audio_device_cfg.res;
    }
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_Setting_By_NVKey audio1 spk[%04X] volume cur[%04X] min[%04X] max[%04X] res[%04X] nvkey_status[%d]", 6,
                audio_device_cfg.terminal_type, audio_device_cfg.cur, audio_device_cfg.min, audio_device_cfg.max, audio_device_cfg.res, nvkey_status);

#ifdef MTK_USB_AUDIO_MICROPHONE
    /* USB Audio 1 Microphone */
    nvkey_size   = sizeof(audio_device_cfg);
    nvkey_status = nvkey_read_data(NVID_USB_SETTING_2, (uint8_t *)&audio_device_cfg, &nvkey_size);
    if (nvkey_status == NVKEY_STATUS_OK) {
        g_UsbAudio[0].mic_terminal_type = audio_device_cfg.terminal_type;
        g_UsbAudio[0].mic_cur           = audio_device_cfg.cur;
        g_UsbAudio[0].mic_min           = audio_device_cfg.min;
        g_UsbAudio[0].mic_max           = audio_device_cfg.max;
        g_UsbAudio[0].mic_res           = audio_device_cfg.res;
    }
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_Setting_By_NVKey audio1 mic[%04X] volume cur[%04X] min[%04X] max[%04X] res[%04X] nvkey_status[%d]", 6,
                audio_device_cfg.terminal_type, audio_device_cfg.cur, audio_device_cfg.min, audio_device_cfg.max, audio_device_cfg.res, nvkey_status);
#endif /* MTK_USB_AUDIO_MICROPHONE */

#ifdef MTK_USB_AUDIO_2ND_SPEAKER
    /* USB Audio 2 : Speaker, default no use */
    g_usb_audio_spk2_feature = FALSE;
    usb_nvkey_device_cfg_t device_cfg;

    /* USB Audio 2 Speaker enable */
    nvkey_size   = sizeof(usb_nvkey_device_cfg_t);
    nvkey_status = nvkey_read_data(NVID_USB_SETTING_0, (uint8_t *)&device_cfg, &nvkey_size);
    if (nvkey_status == NVKEY_STATUS_OK) {
        g_usb_audio_spk2_feature = device_cfg.duble_audio_device;
    }
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_Setting_By_NVKey audio2 spk feature_en[%d] nvkey_status[%d]", 2, g_usb_audio_spk2_feature, nvkey_status);

    /* USB Audio 2 Speaker */
    if (g_usb_audio_spk2_feature) {
        nvkey_size   = sizeof(audio_device_cfg);
        nvkey_status = nvkey_read_data(NVID_USB_SETTING_3, (uint8_t *)&audio_device_cfg, &nvkey_size);
        if (nvkey_status == NVKEY_STATUS_OK) {
            g_UsbAudio[1].spk_terminal_type = audio_device_cfg.terminal_type;
            g_UsbAudio[1].spk_cur           = audio_device_cfg.cur;
            g_UsbAudio[1].spk_min           = audio_device_cfg.min;
            g_UsbAudio[1].spk_max           = audio_device_cfg.max;
            g_UsbAudio[1].spk_res           = audio_device_cfg.res;
        }
        LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_Setting_By_NVKey audio2[%04X] spk volume cur[%04X] min[%04X] max[%04X] res[%04X] nvkey_status[%d]", 6,
                    audio_device_cfg.terminal_type, audio_device_cfg.cur, audio_device_cfg.min, audio_device_cfg.max, audio_device_cfg.res, nvkey_status);
    }
#endif /* MTK_USB_AUDIO_2ND_SPEAKER */
}

void USB_Init_Audio_Status(void)
{
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Init_Audio_Status()", 0);

    /* Initilize the Chat/Game if_id and ep_id */
    USB_Audio_Init_Chat_Game_Info();

    /* USB Audio v1 */
    USB_Audio[0].audio_version = USB_AUDIO_VERSION_V1;
    USB_Audio[1].audio_version = USB_AUDIO_VERSION_V1;

    g_UsbAudio[0].rxpipe = NULL;
    g_UsbAudio[1].rxpipe = NULL;
    g_UsbAudio[0].stream_interface_id = 0xFF;
    g_UsbAudio[1].stream_interface_id = 0xFF;
#ifdef MTK_USB_AUDIO_MICROPHONE
    g_UsbAudio[0].txpipe = NULL;
    g_UsbAudio[1].txpipe = NULL;
    g_UsbAudio[0].stream_microphone_interface_id = 0xFF;
    g_UsbAudio[1].stream_microphone_interface_id = 0xFF;
#endif

    USB_Audio[0].initialized = true;
    USB_Audio[1].initialized = true;
}

/* Release global variable g_UsbAudio[port] */
void USB_Release_Audio_Status(void)
{
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Release_Audio_Status", 0);

    if (g_UsbAudio[0].rxpipe != NULL) {
        hal_usb_release_dma_channel(0, g_UsbAudio[0].rxpipe->byEP, HAL_USB_EP_DIRECTION_RX, false);
    }
    if (g_UsbAudio[1].rxpipe != NULL) {
        hal_usb_release_dma_channel(0, g_UsbAudio[1].rxpipe->byEP, HAL_USB_EP_DIRECTION_RX, false);
    }

#ifdef MTK_USB_AUDIO_MICROPHONE
    if (g_UsbAudio[0].txpipe != NULL) {
        hal_usb_release_dma_channel(g_UsbAudio[0].txpipe->byEP, 0, HAL_USB_EP_DIRECTION_TX, false);
    }
    if (g_UsbAudio[1].txpipe != NULL) {
        hal_usb_release_dma_channel(g_UsbAudio[1].txpipe->byEP, 0, HAL_USB_EP_DIRECTION_TX, false);
    }
#endif

    if (USB_Audio[0].unplug_cb) {
        USB_Send_Message(USB_AUDIO_UNPLUG, NULL);
    }
    if (USB_Audio[1].unplug_cb) {
        USB_Send_Message(USB_AUDIO_UNPLUG, NULL);
    }

    USB_Init_Audio_Status();

    if (g_UsbAudio[0].rx_dma_buffer != NULL) {
        USB_Free_Memory((void *)g_UsbAudio[0].rx_dma_buffer);
        g_UsbAudio[0].rx_dma_buffer = NULL;
    }
    if (g_UsbAudio[1].rx_dma_buffer != NULL) {
        USB_Free_Memory((void *)g_UsbAudio[1].rx_dma_buffer);
        g_UsbAudio[1].rx_dma_buffer = NULL;
    }
#ifdef MTK_USB_AUDIO_MICROPHONE
    if (g_UsbAudio[0].tx_dma_buffer != NULL) {
        USB_Free_Memory((void *)g_UsbAudio[0].tx_dma_buffer);
        g_UsbAudio[0].tx_dma_buffer = NULL;
    }
    if (g_UsbAudio[1].tx_dma_buffer != NULL) {
        USB_Free_Memory((void *)g_UsbAudio[1].tx_dma_buffer);
        g_UsbAudio[1].tx_dma_buffer = NULL;
    }
#endif
    if (gUsbDevice.conf != NULL) {
        USB_Free_Memory((void *)gUsbDevice.conf);
        gUsbDevice.conf = NULL;
    }
}

/************************************************************
    EP0 handle functions
************************************************************/
/* Parse class specific request */
static uint8_t g_usb_audio_mute;
static uint16_t g_usb_audio_volume_cur[2] = {0xF59A, 0xF59A};
static uint16_t g_usb_audio_volume_cur_backup[2] = {0xF59A, 0xF59A};
static uint32_t g_usb_audio_sampling_frequency = 48000; /*44100*/
int32_t g_usb_audio_spk1_volume_dB[2];

#ifdef MTK_USB_AUDIO_MICROPHONE
static uint8_t g_usb_audio_microphone_mute;
static uint16_t g_usb_audio_microphone_volume_cur[2] = {0xF59A, 0xF59A};
static uint16_t g_usb_audio_microphone_volume_cur_backup[2] = {0xF59A, 0xF59A};
static uint32_t g_usb_audio_microphone_sampling_frequency = 16000; /*16000, 44100, 48000*/
int32_t g_usb_audio_mic_volume_dB[2];
#endif
static uint16_t g_usb_audio_set_cur_wIndex;
static uint16_t g_usb_audio_set_cur_wValue;

static uint8_t g_usb_audio2_mute;
static uint16_t g_usb_audio2_volume_cur[2] = {0xF59A, 0xF59A};
static uint16_t g_usb_audio2_volume_cur_backup[2] = {0xF59A, 0xF59A};
static uint32_t g_usb_audio2_sampling_frequency = 48000; /*44100*/
static uint16_t g_usb_audio2_set_cur_wIndex;
static uint16_t g_usb_audio2_set_cur_wValue;
int32_t g_usb_audio_spk2_volume_dB[2];


int32_t USB_Audio_Volume_dB_Convertor(int16_t raw_data)
{
    int32_t dB;

    /* dB = ((int32_t) raw_data) / 256 * 10000 */
    dB = (((int32_t) raw_data) * 625) / 16;

    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_Volume_dB_Convertor raw_data[%04X] dB[%d]", 2, raw_data, dB);

    return dB;
}

uint16_t USB_Audio_Volume_0to100_Convertor(uint16_t raw_data, uint16_t volume_min, uint16_t volume_max)
{
    uint16_t volume, step;

    /* 0~100% = min ~ max */
    step = (volume_max - volume_min) / 100;
    volume = (raw_data - volume_min) / step;

    LOG_MSGID_I(USB_DEBUG, "USB_Audio_Volume_0to100_Convertor volume_max[%04X] volume_min[%04X] step[%d]", 3, volume_max, volume_min, step);
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_Volume_0to100_Convertor raw_data[%04X] volume[%d]", 2, raw_data, volume);

    return volume;
}

static void USB_Audio1_Ep0_DataReceived(void *data)
{
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio1_Ep0_DataReceived", 0);

    bool stall = false;
    uint16_t usb_aduio1_spk_wIndex = (0x02 << 8 | g_UsbAudio[0].control_interface_id);
    uint16_t usb_aduio1_mic_wIndex = (0x06 << 8 | g_UsbAudio[0].control_interface_id);

    uint8_t usb_audio1_spk_ep_addr = (USB_EP_DIR_OUT | g_UsbAudio[0].stream_ep_out_id);
#ifdef MTK_USB_AUDIO_MICROPHONE
    uint8_t usb_audio1_mic_ep_addr = (USB_EP_DIR_IN | g_UsbAudio[0].stream_ep_in_id);
#endif

    if (g_usb_audio_set_cur_wValue == 0x0100 && g_usb_audio_set_cur_wIndex == 0x0400) {
        hal_usb_read_endpoint_fifo(0, 4, &g_usb_audio_sampling_frequency);
        LOG_MSGID_I(USBAUDIO_DRV, "change sampling freq to %d Hz", 1, g_usb_audio_sampling_frequency);
        USB_Send_Message(USB_AUDIO_SET_SAMPLING_FREQ, (void *)(g_usb_audio_sampling_frequency + (usb_audio1_spk_ep_addr << 24)));
    } else if (g_usb_audio_set_cur_wValue == 0x0100 && g_usb_audio_set_cur_wIndex == usb_audio1_spk_ep_addr) {
        hal_usb_read_endpoint_fifo(0, 3, &g_usb_audio_sampling_frequency);
        LOG_MSGID_I(USBAUDIO_DRV, "change sampling freq to %d Hz", 1, g_usb_audio_sampling_frequency);
        USB_Send_Message(USB_AUDIO_SET_SAMPLING_FREQ, (void *)(g_usb_audio_sampling_frequency + (usb_audio1_spk_ep_addr << 24)));
    } else if (g_usb_audio_set_cur_wValue == 0x0100 && g_usb_audio_set_cur_wIndex == usb_aduio1_spk_wIndex) {
        hal_usb_read_endpoint_fifo(0, 1, &g_usb_audio_mute);
        LOG_MSGID_I(USBAUDIO_DRV, "change mute to %d", 1, g_usb_audio_mute);
        USB_Send_Message(USB_AUDIO_SET_MUTE, (void *)(g_usb_audio_mute + (usb_audio1_spk_ep_addr << 24)));
    } else if ((g_usb_audio_set_cur_wValue == 0x0201 || g_usb_audio_set_cur_wValue == 0x0202) && g_usb_audio_set_cur_wIndex == usb_aduio1_spk_wIndex) {
        hal_usb_read_endpoint_fifo(0, 2, &g_usb_audio_volume_cur[g_usb_audio_set_cur_wValue - 0x0201]);
        g_usb_audio_volume_cur_backup[g_usb_audio_set_cur_wValue - 0x0201] = USB_Audio_Volume_0to100_Convertor(g_usb_audio_volume_cur[g_usb_audio_set_cur_wValue - 0x0201], g_UsbAudio[0].spk_min, g_UsbAudio[0].spk_max);
        g_usb_audio_spk1_volume_dB[g_usb_audio_set_cur_wValue - 0x0201] = USB_Audio_Volume_dB_Convertor(g_usb_audio_volume_cur[g_usb_audio_set_cur_wValue - 0x0201]);
        LOG_MSGID_I(USBAUDIO_DRV, "change channel %d volume to %d", 2, g_usb_audio_set_cur_wValue - 0x0200, g_usb_audio_volume_cur_backup[g_usb_audio_set_cur_wValue - 0x0201]);
        USB_Send_Message(USB_AUDIO_SET_VOLUME, (void *)(g_usb_audio_volume_cur_backup[g_usb_audio_set_cur_wValue - 0x0201] + ((g_usb_audio_set_cur_wValue - 0x0200) << 16) + (usb_audio1_spk_ep_addr << 24)));
    }
#ifdef MTK_USB_AUDIO_MICROPHONE
    else if (g_usb_audio_set_cur_wValue == 0x0100 && g_usb_audio_set_cur_wIndex == 0x0800) {
        hal_usb_read_endpoint_fifo(0, 4, &g_usb_audio_microphone_sampling_frequency);
    } else if ((g_usb_audio_set_cur_wValue == 0x0201 || g_usb_audio_set_cur_wValue == 0x0202) && g_usb_audio_set_cur_wIndex == usb_aduio1_mic_wIndex) {
        hal_usb_read_endpoint_fifo(0, 2, &g_usb_audio_microphone_volume_cur[g_usb_audio_set_cur_wValue - 0x0201]);
        g_usb_audio_microphone_volume_cur_backup[g_usb_audio_set_cur_wValue - 0x0201] = USB_Audio_Volume_0to100_Convertor(g_usb_audio_microphone_volume_cur[g_usb_audio_set_cur_wValue - 0x0201], g_UsbAudio[0].mic_min, g_UsbAudio[0].mic_max);
        g_usb_audio_mic_volume_dB[g_usb_audio_set_cur_wValue - 0x0201] = USB_Audio_Volume_dB_Convertor(g_usb_audio_microphone_volume_cur[g_usb_audio_set_cur_wValue - 0x0201]);
        LOG_MSGID_I(USBAUDIO_DRV, "change microphone channel %d volume to %d", 2, g_usb_audio_set_cur_wValue - 0x0200, g_usb_audio_microphone_volume_cur_backup[g_usb_audio_set_cur_wValue - 0x0201]);
        USB_Send_Message(USB_MIC_SET_VOLUME, (void *)(g_usb_audio_microphone_volume_cur_backup[g_usb_audio_set_cur_wValue - 0x0201] + ((g_usb_audio_set_cur_wValue - 0x0200) << 16) + (usb_audio1_mic_ep_addr << 24)));
    } else if (g_usb_audio_set_cur_wValue == 0x0100 && g_usb_audio_set_cur_wIndex == usb_aduio1_mic_wIndex) {
        hal_usb_read_endpoint_fifo(0, 1, &g_usb_audio_microphone_mute);
        LOG_MSGID_I(USBAUDIO_DRV, "change microphone mute to %d", 1, g_usb_audio_microphone_mute);
        USB_Send_Message(USB_MIC_SET_MUTE, (void *)(g_usb_audio_microphone_mute + (usb_audio1_mic_ep_addr << 24)));
    } else if (g_usb_audio_set_cur_wValue == 0x0100 && g_usb_audio_set_cur_wIndex == usb_audio1_mic_ep_addr) {
        hal_usb_read_endpoint_fifo(0, 3, &g_usb_audio_microphone_sampling_frequency);
        LOG_MSGID_I(USBAUDIO_DRV, "change microphone sampling freq to %d Hz", 1, g_usb_audio_microphone_sampling_frequency);
        USB_Send_Message(USB_MIC_SET_SAMPLING_FREQ, (void *)(g_usb_audio_microphone_sampling_frequency + (usb_audio1_mic_ep_addr << 24)));
    }
#endif
    else {
        stall = true;
        LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio1_Ep0_DataReceived stall=true usb_aduio1_wIndex[%x]", 1, usb_aduio1_mic_wIndex);
    }

    gUsbDevice.ep0_state = USB_EP0_RX_STATUS;
    hal_usb_update_endpoint_0_state(HAL_USB_EP0_DRV_STATE_READ_END, stall, true);
}

static void USB_Audio2_Ep0_DataReceived(void *data)
{
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio2_Ep0_DataReceived", 0);

    bool stall = false;
    uint16_t usb_aduio2_spk_wIndex = (0x0A << 8 | g_UsbAudio[1].control_interface_id);

    uint8_t usb_audio2_spk_ep_addr = (USB_EP_DIR_OUT | g_UsbAudio[1].stream_ep_out_id);

    /*wValue: control num  << 8 | channel*/
    /*wIndex: feature_unit << 8 | interface*/
    /*For audio_2,  wIndex =0x0A03*/
    if (g_usb_audio2_set_cur_wValue == 0x0100 && g_usb_audio2_set_cur_wIndex == 0x0400) {
        hal_usb_read_endpoint_fifo(0, 4, &g_usb_audio2_sampling_frequency);
        LOG_MSGID_I(USBAUDIO_DRV, "change sampling freq to %d Hz", 1, g_usb_audio2_sampling_frequency);
        USB_Send_Message(USB_AUDIO_SET_SAMPLING_FREQ, (void *)(g_usb_audio_sampling_frequency + (usb_audio2_spk_ep_addr << 24)));
    } else if (g_usb_audio2_set_cur_wValue == 0x0100 && g_usb_audio2_set_cur_wIndex == usb_audio2_spk_ep_addr) {
        hal_usb_read_endpoint_fifo(0, 3, &g_usb_audio2_sampling_frequency);
        LOG_MSGID_I(USBAUDIO_DRV, "change sampling freq to %d Hz", 1, g_usb_audio2_sampling_frequency);
        USB_Send_Message(USB_AUDIO_SET_SAMPLING_FREQ, (void *)(g_usb_audio2_sampling_frequency + (usb_audio2_spk_ep_addr << 24)));
    } else if (g_usb_audio2_set_cur_wValue == 0x0100 && g_usb_audio2_set_cur_wIndex == usb_aduio2_spk_wIndex) {
        hal_usb_read_endpoint_fifo(0, 1, &g_usb_audio2_mute);
        LOG_MSGID_I(USBAUDIO_DRV, "change mute to %d", 1, g_usb_audio2_mute);
        USB_Send_Message(USB_AUDIO_SET_MUTE, (void *)(g_usb_audio2_mute + (usb_audio2_spk_ep_addr << 24)));
    } else if ((g_usb_audio2_set_cur_wValue == 0x0201 || g_usb_audio2_set_cur_wValue == 0x0202) && g_usb_audio2_set_cur_wIndex == usb_aduio2_spk_wIndex) {
        hal_usb_read_endpoint_fifo(0, 2, &g_usb_audio2_volume_cur[g_usb_audio2_set_cur_wValue - 0x0201]);
        g_usb_audio2_volume_cur_backup[g_usb_audio2_set_cur_wValue - 0x0201] = USB_Audio_Volume_0to100_Convertor(g_usb_audio2_volume_cur[g_usb_audio2_set_cur_wValue - 0x0201], g_UsbAudio[1].spk_min, g_UsbAudio[1].spk_max);
        g_usb_audio_spk2_volume_dB[g_usb_audio2_set_cur_wValue - 0x0201] = USB_Audio_Volume_dB_Convertor(g_usb_audio2_volume_cur[g_usb_audio2_set_cur_wValue - 0x0201]);
        LOG_MSGID_I(USBAUDIO_DRV, "change channel %d volume to %d", 2, g_usb_audio2_set_cur_wValue - 0x0200, g_usb_audio2_volume_cur_backup[g_usb_audio2_set_cur_wValue - 0x0201]);
        USB_Send_Message(USB_AUDIO_SET_VOLUME, (void *)(g_usb_audio2_volume_cur_backup[g_usb_audio2_set_cur_wValue - 0x0201] + ((g_usb_audio2_set_cur_wValue - 0x0200) << 16) + (usb_audio2_spk_ep_addr << 24)));
    } else {
        LOG_MSGID_I(common, "USB_Audio2_Ep0_DataReceived stall=true usb_aduio2_wIndex[%x]", 1, usb_aduio2_spk_wIndex);
        stall = true;
    }

    gUsbDevice.ep0_state = USB_EP0_RX_STATUS;
    hal_usb_update_endpoint_0_state(HAL_USB_EP0_DRV_STATE_READ_END, stall, true);
}

void USB_Audio1_V1_Ep0_Command(Usb_Ep0_Status *pep0state, Usb_Command *pcmd)
{
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio1_V1_Ep0_Command", 0);

    bool  bError = false;
    uint16_t usb_aduio1_spk_wIndex = (0x02 << 8 | g_UsbAudio[0].control_interface_id);
    uint16_t usb_aduio1_mic_wIndex = (0x06 << 8 | g_UsbAudio[0].control_interface_id);

    /*wValue: control num << 8 | channel*/
    /*wIndex: interface << 8 | feature_unit*/
    switch (pcmd->bRequest) {
        case USB_AUDIO_1_REQ_GET_CUR:
            if (pcmd->wValue == 0x0100 && pcmd->wIndex == 0x01) { /* USB out EP for speaker*/
                USB_Generate_EP0Data(pep0state, pcmd, &g_usb_audio_sampling_frequency, pcmd->wLength);
            }
#ifdef MTK_USB_AUDIO_MICROPHONE
            else if (pcmd->wValue == 0x0100 && pcmd->wIndex == 0x81) { /* USB in EP for microphone*/
                USB_Generate_EP0Data(pep0state, pcmd, &g_usb_audio_microphone_sampling_frequency, pcmd->wLength);
            } else if (pcmd->wValue == 0x0100 && pcmd->wIndex == usb_aduio1_mic_wIndex) { /* HSB 0x01: Mute Control; LSB 0x00: Master Channel. HSB 0x06: Feature Unit ID; LSB 0x00: Interface #*/
                USB_Generate_EP0Data(pep0state, pcmd, &g_usb_audio_microphone_mute, pcmd->wLength);
            } else if ((pcmd->wValue == 0x0201 || pcmd->wValue == 0x0202) && (pcmd->wIndex == usb_aduio1_mic_wIndex)) { /* HSB 0x02: Volume Control; LSB 0x01: Channel 1*/
                USB_Generate_EP0Data(pep0state, pcmd, &g_UsbAudio[0].mic_cur, pcmd->wLength);
            }
#endif
            else if (pcmd->wValue == 0x0100 && pcmd->wIndex == usb_aduio1_spk_wIndex) { /* HSB 0x01: Mute Control; LSB 0x00: Master Channel. HSB 0x02: Feature Unit ID; LSB 0x00: Interface #*/
                USB_Generate_EP0Data(pep0state, pcmd, &g_usb_audio_mute, pcmd->wLength);
            }  else if ((pcmd->wValue == 0x0201 || pcmd->wValue == 0x0202) && (pcmd->wIndex == usb_aduio1_spk_wIndex)) { /* HSB 0x02: Volume Control; LSB 0x01: Channel 1*/
                USB_Generate_EP0Data(pep0state, pcmd, &g_UsbAudio[0].spk_cur, pcmd->wLength);
            }  else {
                bError = true;
            }
            break;
        case USB_AUDIO_1_REQ_GET_MIN:
            if ((pcmd->wValue == 0x0201 || pcmd->wValue == 0x0202) && (pcmd->wIndex == usb_aduio1_spk_wIndex)) { /* HSB 0x02: Volume Control; LSB 0x01: Channel 1*/
                USB_Generate_EP0Data(pep0state, pcmd, (void *)(&g_UsbAudio[0].spk_min), pcmd->wLength);
            }
#ifdef MTK_USB_AUDIO_MICROPHONE
            else if ((pcmd->wValue == 0x0201 || pcmd->wValue == 0x0202) && (pcmd->wIndex == usb_aduio1_mic_wIndex)) { /* HSB 0x02: Volume Control; LSB 0x01: Channel 1*/
                USB_Generate_EP0Data(pep0state, pcmd, (void *)(&g_UsbAudio[0].mic_min), pcmd->wLength);
            }
#endif
            else {
                bError = true;
            }
            break;
        case USB_AUDIO_1_REQ_GET_MAX:
            /* tell host the current setting */
            if ((pcmd->wValue == 0x0201 || pcmd->wValue == 0x0202) && (pcmd->wIndex == usb_aduio1_spk_wIndex)) { /* HSB 0x02: Volume Control; LSB 0x01: Channel 1*/
                USB_Generate_EP0Data(pep0state, pcmd, (void *)(&g_UsbAudio[0].spk_max), pcmd->wLength);
            }
#ifdef MTK_USB_AUDIO_MICROPHONE
            else if ((pcmd->wValue == 0x0201 || pcmd->wValue == 0x0202) && (pcmd->wIndex == usb_aduio1_mic_wIndex)) { /* HSB 0x02: Volume Control; LSB 0x01: Channel 1*/
                USB_Generate_EP0Data(pep0state, pcmd, (void *)(&g_UsbAudio[0].mic_max), pcmd->wLength);
            }
#endif
            else {
                bError = true;
            }
            break;
        case USB_AUDIO_1_REQ_GET_RES:
            if ((pcmd->wValue == 0x0201 || pcmd->wValue == 0x0202) && (pcmd->wIndex == usb_aduio1_spk_wIndex)) { /* HSB 0x02: Volume Control; LSB 0x01: Channel 1*/
                USB_Generate_EP0Data(pep0state, pcmd, (void *)(&g_UsbAudio[0].spk_res), pcmd->wLength);
            }
#ifdef MTK_USB_AUDIO_MICROPHONE
            else if ((pcmd->wValue == 0x0201 || pcmd->wValue == 0x0202) && (pcmd->wIndex == usb_aduio1_mic_wIndex)) { /* HSB 0x02: Volume Control; LSB 0x01: Channel 1*/
                USB_Generate_EP0Data(pep0state, pcmd, (void *)(&g_UsbAudio[0].mic_res), pcmd->wLength);
            }
#endif
            else {
                bError = true;
            }
            break;
        case USB_AUDIO_1_REQ_SET_CUR:
            g_usb_audio_set_cur_wValue = pcmd->wValue;
            g_usb_audio_set_cur_wIndex = pcmd->wIndex;
            gUsbDevice.ep0_rx_handler = (usb_ep0_rx_ptr)USB_Audio1_Ep0_DataReceived;
            gUsbDevice.ep0_state = USB_EP0_RX;
            break;
        default:
            bError = true;
            break;
    }

    LOG_MSGID_I(USBAUDIO_DRV, "bmRequestType:0x%02X, bRequest:0x%02X, wValue:0x%04X, wIndex:0x%04X, wLength:0x%04X",
                5, pcmd->bmRequestType, pcmd->bRequest, pcmd->wValue, pcmd->wIndex, pcmd->wLength);

    if (bError) {
        LOG_MSGID_E(common, "[usb][audio1][ep0][error] usb_aduio1_wIndex[%x] usb_aduio1_mic_wIndex[%x]", 2, usb_aduio1_spk_wIndex, usb_aduio1_mic_wIndex);
    }

    /* Stall command if an error occured */
    USB_EP0_Command_Hdlr(bError);
}


void USB_Audio2_V1_Ep0_Command(Usb_Ep0_Status *pep0state, Usb_Command *pcmd)
{
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio2_V1_Ep0_Command", 0);
    bool  bError = false;
    uint16_t usb_aduio2_spk_wIndex = (0x0A << 8 | g_UsbAudio[1].control_interface_id);

    /*wValue: control num << 8 | channel*/
    /*wIndex: feature_unit << 8 | interface*/
    /*For audio_2, the wValue = 0x0100,   wIndex =0X0A03*/
    switch (pcmd->bRequest) {
        case USB_AUDIO_1_REQ_GET_CUR:
            if (pcmd->wValue == 0x0100 && pcmd->wIndex == 0x01) { /* USB out EP for speaker*/
                USB_Generate_EP0Data(pep0state, pcmd, &g_usb_audio2_sampling_frequency, pcmd->wLength);
            } else if (pcmd->wValue == 0x0100 && pcmd->wIndex == usb_aduio2_spk_wIndex) { /* HSB 0x01: Mute Control; LSB 0x00: Master Channel. HSB 0x02: Feature Unit ID; LSB 0x00: Interface #*/
                USB_Generate_EP0Data(pep0state, pcmd, &g_usb_audio2_mute, pcmd->wLength);
            } else if ((pcmd->wValue == 0x0201 || pcmd->wValue == 0x0202) && (pcmd->wIndex == usb_aduio2_spk_wIndex)) { /* HSB 0x02: Volume Control; LSB 0x01: Channel 1*/
                USB_Generate_EP0Data(pep0state, pcmd, &g_UsbAudio[1].spk_cur, pcmd->wLength);
            } else {
                bError = true;
            }
            break;
        case USB_AUDIO_1_REQ_GET_MIN:
            if ((pcmd->wValue == 0x0201 || pcmd->wValue == 0x0202) && (pcmd->wIndex == usb_aduio2_spk_wIndex)) { /* HSB 0x02: Volume Control; LSB 0x01: Channel 1*/
                USB_Generate_EP0Data(pep0state, pcmd, (void *)(&g_UsbAudio[1].spk_min), pcmd->wLength);
            } else {
                bError = true;
            }
            break;
        case USB_AUDIO_1_REQ_GET_MAX:
            /* tell host the current setting */
            if ((pcmd->wValue == 0x0201 || pcmd->wValue == 0x0202) && (pcmd->wIndex == usb_aduio2_spk_wIndex)) { /* HSB 0x02: Volume Control; LSB 0x01: Channel 1*/
                USB_Generate_EP0Data(pep0state, pcmd, (void *)(&g_UsbAudio[1].spk_max), pcmd->wLength);
            } else {
                bError = true;
            }
            break;
        case USB_AUDIO_1_REQ_GET_RES:
            if ((pcmd->wValue == 0x0201 || pcmd->wValue == 0x0202) && (pcmd->wIndex == usb_aduio2_spk_wIndex)) { /* HSB 0x02: Volume Control; LSB 0x01: Channel 1*/
                USB_Generate_EP0Data(pep0state, pcmd, (void *)(&g_UsbAudio[1].spk_res), pcmd->wLength);
            } else {
                bError = true;
            }
            break;
        case USB_AUDIO_1_REQ_SET_CUR:
            g_usb_audio2_set_cur_wValue = pcmd->wValue;
            g_usb_audio2_set_cur_wIndex = pcmd->wIndex;
            gUsbDevice.ep0_rx_handler = (usb_ep0_rx_ptr)USB_Audio2_Ep0_DataReceived;
            gUsbDevice.ep0_state = USB_EP0_RX;
            break;
        default:
            bError = true;
            break;
    }

    LOG_MSGID_I(USBAUDIO_DRV, "bmRequestType:0x%02X, bRequest:0x%02X, wValue:0x%04X, wIndex:0x%04X, wLength:0x%04X",
                5, pcmd->bmRequestType, pcmd->bRequest, pcmd->wValue, pcmd->wIndex, pcmd->wLength);

    if (bError) {
        LOG_MSGID_E(common, "[usb][audio2][ep0][error] usb_aduio2_wIndex[%x]", 1, usb_aduio2_spk_wIndex);
    }

    /* Stall command if an error occured */
    USB_EP0_Command_Hdlr(bError);
}

void USB_Audio1_Ep0_Command(Usb_Ep0_Status *pep0state, Usb_Command *pcmd)
{
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio1_Ep0_Command", 0);

    if (USB_Audio[0].audio_version == USB_AUDIO_VERSION_V1) {
        USB_Audio1_V1_Ep0_Command(pep0state, pcmd);
    } else {
        LOG_MSGID_E(USBAUDIO_DRV, "USB_Audio_Ep0_Command version[%d] ERROR!!", 1, (uint8_t)USB_Audio[0].audio_version);
    }
}

void USB_Audio2_Ep0_Command(Usb_Ep0_Status *pep0state, Usb_Command *pcmd)
{
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio2_Ep0_Command", 0);

    if (USB_Audio[1].audio_version == USB_AUDIO_VERSION_V1) {
        USB_Audio2_V1_Ep0_Command(pep0state, pcmd);
    } else {
        LOG_MSGID_E(USBAUDIO_DRV, "USB_Audio_Ep0_Command version[%d] ERROR!!", 1, (uint8_t)USB_Audio[0].audio_version);
    }
}

/************************************************************
    Iso EP OUT handle functions
*************************************************************/
static uint32_t USB_Audio_Get_RX_Bytes(uint32_t port)
{
    /* byte = sample rate x sample size x channel */
    uint32_t rx_byte;

#ifdef USB_AUDIO_SPEAKER_SAMPLESIZE_24BIT
    if(port == 0){
    rx_byte = (g_usb_audio_sampling_frequency/1000) *3 *2;
    } else {
        rx_byte = (g_usb_audio2_sampling_frequency/1000) *3 *2;
    }
#else
    rx_byte = USB_AUDIO_48K_DATA_SIZE;
#endif

    return rx_byte;
}

static uint32_t USB_Audio_Buffer_Space(uint32_t port)
{
    uint32_t byte = USB_Audio_Get_RX_Bytes(port);

    if (g_UsbAudio[port].rx_dma_buffer_is_full) {
        LOG_MSGID_E(USBAUDIO_DRV, "USB_Audio_Buffer_Space RX buffer is full ERROR!!", 0);
        return 0;
    } else {
        if ((g_UsbAudio[port].rx_dma_buffer_write + byte) == g_UsbAudio[port].rx_dma_buffer_len) {
            /*LOG_MSGID_I(USBAUDIO_DRV, "usb error USB_Audio_Buffer_Space() reach full\n", 0);*/
        }
        return byte;
    }
}

void *USB_Audio_Rx_Buffer_Get_Read_Address(uint32_t port)
{
    return g_UsbAudio[port].rx_dma_buffer + g_UsbAudio[port].rx_dma_buffer_read;
}

uint32_t USB_Audio_Rx_Buffer_Get_Bytes(uint32_t port)
{
    uint32_t write_temp, read_temp, rc;

    write_temp = g_UsbAudio[port].rx_dma_buffer_write;
    read_temp = g_UsbAudio[port].rx_dma_buffer_read;

    if (write_temp > read_temp) {
        rc = write_temp - read_temp;
    } else if (write_temp < read_temp || g_UsbAudio[port].rx_dma_buffer_is_full == 1) {
        rc = g_UsbAudio[port].rx_dma_buffer_len - read_temp;
    } else {
        rc = 0;
    }

    return rc;
}

void USB_Audio_Rx_Buffer_Drop_Bytes(uint32_t port, uint32_t bytes)
{
    uint32_t read_temp;

    if (g_UsbAudio[port].rx_dma_buffer_len > g_UsbAudio[port].rx_dma_buffer_read + bytes) {
        read_temp = g_UsbAudio[port].rx_dma_buffer_read + bytes;
    } else {
        read_temp = bytes - (g_UsbAudio[port].rx_dma_buffer_len - g_UsbAudio[port].rx_dma_buffer_read);
    }

    g_UsbAudio[port].rx_dma_buffer_read = read_temp;
    g_UsbAudio[port].rx_dma_buffer_is_full = 0;

    return;
}

#ifdef USB_AUDIO_RX_TIME_MEASURE
uint32_t usb_rx1_curr_time = 0;
uint32_t usb_rx1_last_time = 0;
uint32_t usb_rx2_curr_time = 0;
uint32_t usb_rx2_last_time = 0;
#endif

/* EP Iso Out interrupt handler, called by EP interrupt */
void USB_Audio1_IsoOut_Hdr(void)
{
    uint32_t port = 0;
    uint32_t nCount;

#ifdef USB_HOST_DETECT_PS
    g_usb_audio_spk1_status = TRUE;
#endif

#ifdef USB_AUDIO_RX_TIME_MEASURE
    /* Check the ISO out is expected 1ms/transaction */
    uint32_t time_gap = 0;
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &usb_rx1_curr_time);

    time_gap = usb_rx1_curr_time - usb_rx1_last_time;

    if ((time_gap > USB_HS_ISO_TRANSFER_UPPER) ||
        (time_gap < USB_HS_ISO_TRANSFER_LOWER)) {
        LOG_MSGID_E(USBAUDIO_DRV, "USB_Audio1_IsoOut_Hdr time_gap[%d]us", 1, time_gap);
    }

    usb_rx1_last_time = usb_rx1_curr_time;
#endif

    if (USB_Audio[port].initialized == false) {
        LOG_MSGID_E(USBAUDIO_DRV, "USB_Audio1_IsoOut_Hdr not be initialized", 0);
        return;
    }

    nCount = USB_Audio_Get_RX_Bytes(port); /*hal_usb_get_rx_packet_length(g_UsbAudio[port].rxpipe->byEP);*/

    if (nCount != 0) {
        if (g_UsbAudio[port].rx_dma_buffer_is_full) {
            LOG_MSGID_E(USBAUDIO_DRV, "USB_Audio1_IsoOut_Hdr() rx_dma_buffer is full", 0);
        } else {
            uint32_t nSpace = USB_Audio_Buffer_Space(port);

            if (nSpace < nCount) {
                LOG_MSGID_E(USBAUDIO_DRV, "USB_Audio1_IsoOut_Hdr() no enough space", 0);
                nCount = nSpace;
            }

            if (g_UsbAudio[port].rx_dma_buffer_write >= g_UsbAudio[port].rx_dma_buffer_read) {
                hal_usb_read_endpoint_fifo(g_UsbAudio[port].rxpipe->byEP, nCount, g_UsbAudio[port].rx_dma_buffer + g_UsbAudio[port].rx_dma_buffer_write);
                /*hal_usb_write_endpoint_fifo(g_UsbAudio[port].txpipe->byEP, nCount, g_UsbAudio[port].rx_dma_buffer + g_UsbAudio[port].rx_dma_buffer_write);*/
            } else {
                if ((g_UsbAudio[port].rx_dma_buffer_write + nCount) <= g_UsbAudio[port].rx_dma_buffer_read) {
                    hal_usb_read_endpoint_fifo(g_UsbAudio[port].rxpipe->byEP, nCount, g_UsbAudio[port].rx_dma_buffer + g_UsbAudio[port].rx_dma_buffer_write);
                    /*hal_usb_write_endpoint_fifo(g_UsbAudio[port].txpipe->byEP, nCount, g_UsbAudio[port].rx_dma_buffer + g_UsbAudio[port].rx_dma_buffer_write);*/
                } else {
                    LOG_MSGID_E(USBAUDIO_DRV, "USB_Audio1_IsoOut_Hdr() wptr over rptr", 0);
                }
            }

            g_UsbAudio[port].rx_dma_buffer_write += nCount;

            if (g_UsbAudio[port].rx_dma_buffer_write == g_UsbAudio[port].rx_dma_buffer_len) {
                g_UsbAudio[port].rx_dma_buffer_write = 0;
            }
            /*if(g_UsbAudio[port].rx_dma_buffer_write == g_UsbAudio[port].rx_dma_buffer_read)*/
            /*    g_UsbAudio[port].rx_dma_buffer_is_full = 1;*/
        }

        if (USB_Audio[port].rx_cb) {
            LOG_MSGID_I(USB_DEBUG, "USB_Audio1_IsoOut_Hdr RX callback", 0);
            hal_usb_set_endpoint_rx_ready(g_UsbAudio[port].rxpipe->byEP);
            USB_Audio[port].rx_cb();
        } else {
            LOG_MSGID_E(USB_DEBUG, "USB_Audio1_IsoOut_Hdr RX callback is NULL", 0);
            hal_usb_set_endpoint_rx_ready(g_UsbAudio[port].rxpipe->byEP);
            USB_Audio_Rx_Buffer_Drop_Bytes(port, nCount);
        }
    }
}

/* EP Iso Out interrupt handler, called by EP interrupt */
void USB_Audio2_IsoOut_Hdr(void)
{
    uint32_t port = 1;
    uint32_t nCount;

#ifdef USB_HOST_DETECT_PS
    g_usb_audio_spk2_status = TRUE;
#endif

#ifdef USB_AUDIO_RX_TIME_MEASURE
    /* Check the ISO out is expected 1ms/transaction */
    uint32_t time_gap = 0;

    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &usb_rx2_curr_time);
    time_gap = usb_rx2_curr_time - usb_rx2_last_time;

    if ((time_gap > USB_HS_ISO_TRANSFER_UPPER) ||
        (time_gap < USB_HS_ISO_TRANSFER_LOWER)) {
        LOG_MSGID_E(USBAUDIO_DRV, "USB_Audio2_IsoOut_Hdr time_gap[%d]us", 1, time_gap);
    }

    usb_rx2_last_time = usb_rx2_curr_time;
#endif

    if (USB_Audio[port].initialized == false) {
        return;
    }

    nCount = USB_Audio_Get_RX_Bytes(port); /*hal_usb_get_rx_packet_length(g_UsbAudio[port].rxpipe->byEP);*/

    if (nCount != 0) {
        if (g_UsbAudio[port].rx_dma_buffer_is_full) {
            LOG_MSGID_E(USBAUDIO_DRV, "ERROR!! USB_Audio2_IsoOut_Hdr() rx_dma_buffer_is_full", 0);
        } else {
            uint32_t nSpace = USB_Audio_Buffer_Space(port);

            if (nSpace < nCount) {
                LOG_MSGID_E(USBAUDIO_DRV, "ERROR!! USB_Audio2_IsoOut_Hdr() no enough space", 0);
                nCount = nSpace;
            }

            if (g_UsbAudio[port].rx_dma_buffer_write >= g_UsbAudio[port].rx_dma_buffer_read) {
                hal_usb_read_endpoint_fifo(g_UsbAudio[port].rxpipe->byEP, nCount, g_UsbAudio[port].rx_dma_buffer + g_UsbAudio[port].rx_dma_buffer_write);
                /*hal_usb_write_endpoint_fifo(g_UsbAudio[port].txpipe->byEP, nCount, g_UsbAudio[port].rx_dma_buffer + g_UsbAudio[port].rx_dma_buffer_write);*/
            } else {
                if ((g_UsbAudio[port].rx_dma_buffer_write + nCount) <= g_UsbAudio[port].rx_dma_buffer_read) {
                    hal_usb_read_endpoint_fifo(g_UsbAudio[port].rxpipe->byEP, nCount, g_UsbAudio[port].rx_dma_buffer + g_UsbAudio[port].rx_dma_buffer_write);
                    /*hal_usb_write_endpoint_fifo(g_UsbAudio[port].txpipe->byEP, nCount, g_UsbAudio[port].rx_dma_buffer + g_UsbAudio[port].rx_dma_buffer_write);*/
                } else {
                    LOG_MSGID_E(USBAUDIO_DRV, "USB_Audio2_IsoOut_Hdr() wptr over rptr", 0);
                }
            }

            g_UsbAudio[port].rx_dma_buffer_write += nCount;

            if (g_UsbAudio[port].rx_dma_buffer_write == g_UsbAudio[port].rx_dma_buffer_len) {
                g_UsbAudio[port].rx_dma_buffer_write = 0;
            }
            /*if(g_UsbAudio[port].rx_dma_buffer_write == g_UsbAudio[port].rx_dma_buffer_read)*/
            /*g_UsbAudio[port].rx_dma_buffer_is_full = 1;*/
        }

        if (USB_Audio[port].rx_cb) {
            LOG_MSGID_I(USB_DEBUG, "USB_Audio2_IsoOut_Hdr RX callback", 0);
            hal_usb_set_endpoint_rx_ready(g_UsbAudio[port].rxpipe->byEP);
            USB_Audio[port].rx_cb();
        } else {
            LOG_MSGID_E(USB_DEBUG, "USB_Audio2_IsoOut_Hdr RX callback is NULL", 0);
            hal_usb_set_endpoint_rx_ready(g_UsbAudio[port].rxpipe->byEP);
            USB_Audio_Rx_Buffer_Drop_Bytes(port, nCount);
        }
    }
}


/* EP Iso Out reset handler */
void USB_Audio1_IsoOut_Reset(void)
{
    g_UsbAudio[0].rxpipe = &g_UsbAudio[0].stream_ep_out_info->ep_status.epout_status;
}
void USB_Audio2_IsoOut_Reset(void)
{
    g_UsbAudio[1].rxpipe = &g_UsbAudio[1].stream_ep_out_info->ep_status.epout_status;
}


#ifdef MTK_USB_AUDIO_MICROPHONE

#ifdef USB_AUDIO_TX_TIME_MEASURE
uint32_t usb_tx_curr_time  = 0;
uint32_t usb_tx_last_time  = 0;
#endif

#ifdef USB_AUDIO_TEST_TONE
/* 5000HZ, 48000FS, 1Channels, 16bits */
static const uint16_t test_5khz_tone[48] = {
    0x0000, 0x4c18, 0x78bd, 0x737c, 0x3e7f, 0xefb0, 0xa79d, 0x8412,
    0x93c0, 0xd02b, 0x205a, 0x632b, 0x7d00, 0x632b, 0x205a, 0xd02b,
    0x93c0, 0x8412, 0xa79d, 0xefb0, 0x3e80, 0x737c, 0x78bd, 0x4c18,
    0x0000, 0xb3e8, 0x8743, 0x8c84, 0xc180, 0x1050, 0x5863, 0x7bee,
    0x6c40, 0x2fd5, 0xdfa6, 0x9cd5, 0x8300, 0x9cd5, 0xdfa6, 0x2fd5,
    0x6c40, 0x7bee, 0x5863, 0x1050, 0xc181, 0x8c84, 0x8743, 0xb3e8,
};
#endif

/* Create maxiumn size of tx buffer for mute data */
static const uint8_t mic_mute_data[USB_AUDIO_TX_EP_MAX_SIZE] = {0};

static uint32_t USB_Audio_Get_TX_Bytes()
{
    /* byte = sample rate x sample size x channel */
    uint32_t tx_byte;

    tx_byte = (g_usb_audio_microphone_sampling_frequency / 1000) * 2 * 1;

    return tx_byte;
}

/* Send audio data API by endpoint */
uint32_t USB_Audio_TX_SendData(uint8_t *data, uint8_t len)
{
    uint32_t port = 0;
    uint32_t ep_id = g_UsbAudio[port].stream_ep_in_id;
    uint32_t ret = len;

    LOG_MSGID_I(USB_DEBUG, "USB_XBOX_Audio_TX_SendData ep_in_id[%x] data[%x] len[%x]", 3, ep_id, data, len);

    /* Check the length is valid */
    if (len == 0) {
        LOG_MSGID_E(USBAUDIO_DRV, "USB_Audio_TX_SendData Data length is 0", 0);
        return 0;
    }

    if (len > USB_AUDIO_TX_EP_MAX_SIZE) {
        LOG_MSGID_E(USBAUDIO_DRV, "USB_Audio_TX_SendData Data length (%d) is larger than (%d)byte", 2, len, USB_AUDIO_TX_EP_MAX_SIZE);
        return 0;
    }

    /* Set data from share buffer into USB FIFO */
    hal_usb_write_endpoint_fifo(ep_id, len, (void *)data);

    /* Send usb data */
    hal_usb_set_endpoint_tx_ready(ep_id);

    /* Print debug log */
    LOG_MSGID_I(USB_DEBUG, "USB_Audio_TX_SendData [%02X][%02X][%02X][%02X][%02X][%02X][%02X][%02X]", 8,
                data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);

    return ret;
}

/* EP Iso In interrupt handler, called by EP interrupt */
void USB_Audio_IsoIn_Hdr(void)
{
    uint32_t port = 0;
    uint32_t nCount;

    nCount = USB_Audio_Get_TX_Bytes();

#ifdef USB_AUDIO_TX_TIME_MEASURE
    /* Check the ISO in is expected 1ms/transaction */
    uint32_t time_gap = 0;

    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &usb_tx_curr_time);
    time_gap = usb_tx_curr_time - usb_tx_last_time;

    if ((time_gap > USB_HS_ISO_TRANSFER_UPPER) ||
        (time_gap < USB_HS_ISO_TRANSFER_LOWER)) {
        LOG_MSGID_E(USBAUDIO_DRV, "USB_Audio_IsoIn_Hdr time_gap[%d]us", 1, time_gap);
    }

    usb_tx_last_time = usb_tx_curr_time;
#endif

#ifndef USB_AUDIO_TEST_TONE
    if (USB_Audio[port].tx_cb) {
        LOG_MSGID_I(USB_DEBUG, "USB_Audio_IsoIn_Hdr TX callback", 0);
        USB_Audio[port].tx_cb();
    } else {
        LOG_MSGID_E(USBAUDIO_DRV, "USB_Audio_IsoIn_Hdr TX callback is NULL", 0);
        USB_Audio_TX_SendData(mic_mute_data, nCount);
    }
#else
    USB_Audio_TX_SendData(test_5khz_tone, nCount);
#endif
}

/* EP Iso Out reset handler */
void USB_Audio_IsoIn_Reset(void)
{
    g_UsbAudio[0].txpipe = &g_UsbAudio[0].stream_ep_in_info->ep_status.epout_status;
}
#endif

void USB_Audio_Register_Rx_Callback(uint32_t port, AUDIO_RX_FUNC rx_cb)
{
    if (rx_cb != NULL) {
        LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_Register_Rx_Callback port %d, rx_cb = 0x%X ", 2, port, rx_cb);
        USB_Audio[port].rx_cb = rx_cb;
    } else {
        USB_Audio[port].rx_cb = NULL;
        LOG_MSGID_E(USBAUDIO_DRV, "USB_Audio_Register_Rx_Callback port %d is NULL ERROR!!", 1, port);
    }
}

void USB_Audio_Register_Tx_Callback(uint32_t port, AUDIO_TX_FUNC tx_cb)
{
    if (tx_cb != NULL) {
        LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_Register_Tx_Callback port %d, tx_cb = 0x%X ", 2, port, tx_cb);
        USB_Audio[port].tx_cb = tx_cb;
    } else {
        USB_Audio[port].tx_cb = NULL;
        LOG_MSGID_E(USBAUDIO_DRV, "USB_Audio_Register_Tx_Callback port %d is NULL ERROR!!", 1, port);
    }
}

void USB_Audio_Register_SetInterface_Callback(uint32_t port, AUDIO_SETINTERFACE_FUNC setinterface_cb)
{
    if (setinterface_cb != NULL) {
        LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_Register_SetInterface_Callback port %d, setinterface_cb = 0x%X ", 2, port, setinterface_cb);
        USB_Audio[port].setinterface_cb = setinterface_cb;
    } else {
        LOG_MSGID_E(USBAUDIO_DRV, "USB_Audio_Register_SetInterface_Callback port %d is NULL ERROR!!", 1, port);
    }
}

void USB_Audio_Register_SetSamplingRate_Callback(uint32_t port, AUDIO_SETSAMPLINGRATE_FUNC setsamplingrate_cb)
{
    if (setsamplingrate_cb != NULL) {
        LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_Register_SetSamplingRate_Callback port %d, setsamplingrate_cb = 0x%X ", 2, port, setsamplingrate_cb);
        USB_Audio[port].setsamplingrate_cb = setsamplingrate_cb;
    } else {
        LOG_MSGID_E(USBAUDIO_DRV, "USB_Audio_Register_SetSamplingRate_Callback port %d is NULL ERROR!!", 1, port);
    }
}

void USB_Audio_Register_Unplug_Callback(uint32_t port, AUDIO_UNPLUG_FUNC unplug_cb)
{
    if (unplug_cb != NULL) {
        LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_Register_Unplug_Callback port %d, unplug_cb = 0x%X ", 2, port, unplug_cb);
        USB_Audio[port].unplug_cb = unplug_cb;
    } else {
        LOG_MSGID_E(USBAUDIO_DRV, "USB_Audio_Register_Unplug_Callback port %d is NULL ERROR!!", 1, port);
    }
}

void USB_Audio_Register_VolumeChange_Callback(uint32_t port, AUDIO_VOLUMECHANGE_FUNC volumechange_cb)
{
    if (volumechange_cb != NULL) {
        LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_Register_VolumeChange_Callback register port %d, volumechange_cb = 0x%X ", 2, port, volumechange_cb);
        USB_Audio[port].volumechange_cb = volumechange_cb;
    } else {
        LOG_MSGID_E(USBAUDIO_DRV, "USB_Audio_Register_VolumeChange_Callback register port %d is NULL ERROR!!", 1, port);
    }
}

void USB_Audio_Register_Mute_Callback(uint32_t port, AUDIO_MUTE_FUNC mute_cb)
{
    if (mute_cb != NULL) {
        LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_Register_Mute_Callback register port %d, mute_cb = 0x%X ", 2, port, mute_cb);
        USB_Audio[port].mute_cb = mute_cb;
    } else {
        LOG_MSGID_E(USBAUDIO_DRV, "USB_Audio_Register_Mute_Callback register port %d is NULL ERROR!!", 1, port);
    }
}

#ifdef MTK_USB_AUDIO_MICROPHONE
void USB_Audio_Register_Mic_SetInterface_Callback(uint32_t port, AUDIO_SETINTERFACE_FUNC setinterface_cb)
{
    port = 0;

    USB_Audio[port].setinterface_cb_mic = setinterface_cb;
}

void USB_Audio_Register_Mic_SetSamplingRate_Callback(uint32_t port, AUDIO_SETSAMPLINGRATE_FUNC setsamplingrate_cb)
{
    port = 0;

    USB_Audio[port].setsamplingrate_cb_mic = setsamplingrate_cb;
}

void USB_Audio_Register_Mic_Unplug_Callback(uint32_t port, AUDIO_UNPLUG_FUNC unplug_cb)
{
    port = 0;

    USB_Audio[port].unplug_cb_mic = unplug_cb;
}

void USB_Audio_Register_Mic_VolumeChange_Callback(uint32_t port, AUDIO_VOLUMECHANGE_FUNC volumechange_cb)
{
    port = 0;

    USB_Audio[port].volumechange_cb_mic = volumechange_cb;
}

void USB_Audio_Register_Mic_Mute_Callback(uint32_t port, AUDIO_MUTE_FUNC mute_cb)
{
    port = 0;

    USB_Audio[port].mute_cb_mic = mute_cb;
}

#endif

uint32_t USB_Audio_Get_CurSamplingRate(uint32_t port)
{
    uint32_t sampling_rate = 0;
    if (port == 0) {
        sampling_rate = g_usb_audio_sampling_frequency;
    } else if (port == 1) {
        sampling_rate = g_usb_audio2_sampling_frequency;
    }
    return sampling_rate;
}

#ifdef MTK_USB_AUDIO_MICROPHONE
uint32_t USB_Audio_Get_Mic_CurSamplingRate(uint32_t port)
{
    uint32_t sampling_rate = 0;
    if (port == 0) {
        sampling_rate = g_usb_audio_microphone_sampling_frequency;
    }
    return sampling_rate;
}
#endif

uint32_t USB_Audio_Get_Len_Received_Data(uint32_t port)
{
    uint32_t write_temp, read_temp, rc;

    write_temp = g_UsbAudio[port].rx_dma_buffer_write;
    read_temp = g_UsbAudio[port].rx_dma_buffer_read;

    if (write_temp > read_temp) {
        rc = write_temp - read_temp;
    } else if (g_UsbAudio[port].rx_dma_buffer_is_full == 1) {
        rc = g_UsbAudio[port].rx_dma_buffer_len;
    } else if (write_temp < read_temp) {
        rc = g_UsbAudio[port].rx_dma_buffer_len - read_temp + write_temp;
    } else {
        rc = 0;
    }

    return rc;
}

uint32_t USB_Audio_Read_Data(uint32_t port, void *dst, uint32_t len)
{
    uint32_t read_bytes = 0;
    uint32_t data_bytes;

    data_bytes = USB_Audio_Rx_Buffer_Get_Bytes(port);
    if (data_bytes >= len) {
        memcpy(dst,  USB_Audio_Rx_Buffer_Get_Read_Address(port), len);
        USB_Audio_Rx_Buffer_Drop_Bytes(port, len);
        read_bytes = len;
    } else {
        memcpy(dst,  USB_Audio_Rx_Buffer_Get_Read_Address(port), data_bytes);
        USB_Audio_Rx_Buffer_Drop_Bytes(port, data_bytes);
        read_bytes = data_bytes;
        data_bytes = USB_Audio_Rx_Buffer_Get_Bytes(port);
        if (data_bytes >= len - read_bytes) {
            memcpy(dst + read_bytes,  USB_Audio_Rx_Buffer_Get_Read_Address(port), len - read_bytes);
            USB_Audio_Rx_Buffer_Drop_Bytes(port, len - read_bytes);
            read_bytes = len;
        } else {
            memcpy(dst + read_bytes, USB_Audio_Rx_Buffer_Get_Read_Address(port), data_bytes);
            USB_Audio_Rx_Buffer_Drop_Bytes(port, data_bytes);
            read_bytes += data_bytes;
        }
    }

    return read_bytes;
}

static int16_t usb_audio_dscr_interface_serialize(void *dscr, void *out, uint16_t ava_size)
{
    uint16_t size = sizeof(usb_audio_dscr_interface_t);

    if (ava_size < size) {
        assert(0);
        return -1;
    }

    for(int i = 0; i < size; i++) {
        ((uint8_t *)out)[i] = ((uint8_t *)dscr)[i];
    }

    return size;
}

static int16_t usb_audio_dscr_control_header_serialize(void *dscr, void *out, uint16_t ava_size)
{
    uint16_t size = sizeof(usb_audio_dscr_control_header_t);
    size = size - USB_AUDIO_DSCR_MAX_INTERFACE_NR + ((usb_audio_dscr_control_header_t*)dscr)->bInCollection;

    if (ava_size < size) {
        assert(0);
        return -1;
    }

    for(int i = 0; i < size; i++) {
        ((uint8_t *)out)[i] = ((uint8_t *)dscr)[i];
    }

    return size;
}

static int16_t usb_audio_dscr_it_serialize(void *dscr, void *out, uint16_t ava_size)
{
    uint16_t size = sizeof(usb_audio_dscr_it_t);

    if (ava_size < size) {
        assert(0);
        return -1;
    }

    for(int i = 0; i < size; i++) {
        ((uint8_t *)out)[i] = ((uint8_t *)dscr)[i];
    }

    return size;
}

static int16_t usb_audio_dscr_ot_serialize(void *dscr, void *out, uint16_t ava_size)
{
    uint16_t size = sizeof(usb_audio_dscr_ot_t);

    if (ava_size < size) {
        assert(0);
        return -1;
    }

    for(int i = 0; i < size; i++) {
        ((uint8_t *)out)[i] = ((uint8_t *)dscr)[i];
    }

    return size;
}

static int16_t usb_audio_dscr_mixer_serialize(void *dscr, void *out, uint16_t ava_size)
{
    uint16_t size = sizeof(usb_audio_dscr_mixer_t);
    size = size - USB_DSCR_MIXER_MAX_IN_PINS + ((usb_audio_dscr_mixer_t*)dscr)->bNrInPins;

    if (ava_size < size) {
        assert(0);
        return -1;
    }

    for(int i = 0; i < size; i++) {
        ((uint8_t *)out)[i] = ((uint8_t *)dscr)[i];
    }

    return size;
}

static int16_t usb_audio_dscr_selector_serialize(void *dscr, void *out, uint16_t ava_size)
{
    uint16_t size = sizeof(usb_audio_dscr_selector_t);
    size = size - USB_DSCR_SELECTOR_MAX_IN_PINS + ((usb_audio_dscr_selector_t*)dscr)->bNrInPins;

    if (ava_size < size) {
        assert(0);
        return -1;
    }

    for(int i = 0; i < size - 1 ; i++) {
        ((uint8_t *)out)[i] = ((uint8_t *)dscr)[i];
    }
    ((uint8_t *)out)[size - 1] = ((usb_audio_dscr_selector_t*)dscr)->iSelector;

    return size;
}

static int16_t usb_audio_dscr_feature_serialize(void *dscr, void *out, uint16_t ava_size)
{
    uint16_t size = sizeof(usb_audio_dscr_feature_t);
    size = size - USB_DSCR_FEATURE_MAX_CONTROLS - 1 + ((usb_audio_dscr_feature_t*)dscr)->control_nr;

    if (size != ((usb_audio_dscr_feature_t*)dscr)->bLength) {
        assert(0);
        return -1;
    }

    if (ava_size < size) {
        assert(0);
        return -1;
    }

    for(int i = 0; i < size - 1 ; i++) {
        ((uint8_t *)out)[i] = ((uint8_t *)dscr)[i];
    }
    ((uint8_t *)out)[size - 1] = ((usb_audio_dscr_feature_t*)dscr)->iFeature;

    return size;
}

static int16_t usb_audio_dscr_as_general_serialize(void *dscr, void *out, uint16_t ava_size)
{
    uint16_t size = sizeof(usb_audio_dscr_as_general_t);

    if (size != ((usb_audio_dscr_as_general_t*)dscr)->bLength) {
        assert(0);
        return -1;
    }

    if (ava_size < size) {
        assert(0);
        return -1;
    }

    for(int i = 0; i < size ; i++) {
        ((uint8_t *)out)[i] = ((uint8_t *)dscr)[i];
    }

    return size;
}

static int16_t usb_audio_dscr_as_format_type_serialize(void *dscr, void *out, uint16_t ava_size)
{
    uint16_t size = sizeof(usb_audio_dscr_as_format_type_t);
    size -= USB_AUDIO_DSCR_MAX_FREQ_NUM * sizeof(usb_audio_frequency_t);
    size += ((usb_audio_dscr_as_format_type_t*)dscr)->bSamFreqType * sizeof(usb_audio_frequency_t);

    if (size != ((usb_audio_dscr_as_format_type_t*)dscr)->bLength) {
        assert(0);
        return -1;
    }

    if (ava_size < size) {
        assert(0);
        return -1;
    }

    for(int i = 0; i < size ; i++) {
        ((uint8_t *)out)[i] = ((uint8_t *)dscr)[i];
    }

    return size;
}

int16_t dscr_list_serialize(usb_audio_dscr_hdlr_t *dscr_list, uint8_t len, uint8_t *out, uint16_t ava_len)
{
    int16_t tmp;
    int16_t idx = 0;

    for (int i = 0; i < len; i++) {
        tmp = dscr_list[i].serial_func(dscr_list[i].dscr, out + idx, ava_len);
        LOG_MSGID_I(USBAUDIO_DRV, "i[%d] tmp[%d]", 2, i, tmp);

        if (tmp < 0) {
            assert(0);
        }
        idx += tmp;
    }
    return idx;
}

#endif

#ifdef MTK_USB_AUDIO_HID_ENABLE

/* USB Middleware includes */
#include "usb_resource.h"
#include "usbaudio_drv.h"

/* Kernel includes */
#include "mux.h"
#include "mux_port_device.h"

/* Hal includes */
#include "hal_pmu.h"

UsbAudioHid_Struct g_UsbAudioHID;
static uint8_t usb_audio_hid_idle_val[256]; /* Restore idle value for each report */

static const uint8_t audio_hid_if_dscr[] = {
    USB_IFDSC_LENGTH,                            //bLength;
    USB_INTERFACE,                               //bDescriptorType;
    0x01,                                        //bInterfaceNumber;
    USB_AUDIO_HID_ALTERNATE_SETTING_NONE,        //bAlternateSetting;
    USB_AUDIO_HID_TX_EP_NUMBER,                  //bNumEndpoints;
    USB_AUDIO_HID_INTERFACE_CLASS_HID,           //bInterfaceClass;
    USB_AUDIO_HID_INTERFACE_SUBCLASS_NONE,       //bInterfaceSubClass;
    USB_AUDIO_HID_INTERFACE_PROTOCOL_NONE,       //bInterfaceProtocol;
    0x01,                                        //iInterface;

    USB_AUDIO_HID_DESCRIPTOR_LENGTH,             //bLength;
    0x21,                                        //bDescriptorType;
    0x10,                                        //bcdHID;
    0x01,                                        //bcdHID;
    0x00,                                        //bContryCode;
    0x01,                                        //bNumDescriptors;
    0x22,                                        //bDescriptorType;
    (uint8_t)(USB_HID_REPORT_LEN),               //wItemLength;
    (uint8_t)(USB_HID_REPORT_LEN / 256),         //wItemLength;
};

const uint8_t audio_hid_dscr[] = {
    USB_AUDIO_HID_DESCRIPTOR_LENGTH,             //bLength;
    0x21,                                        //bDescriptorType;
    0x10,                                        //bcdHID;
    0x01,                                        //bcdHID;
    0x00,                                        //bContryCode;
    0x01,                                        //bNumDescriptors;
    0x22,                                        //bDescriptorType;
    (uint8_t)(USB_HID_REPORT_LEN),               //wItemLength;
    (uint8_t)(USB_HID_REPORT_LEN / 256),         //wItemLength;
};

const uint8_t audio_hid_ep_dscr[] = {
    USB_EPDSC_LENGTH,    //bLength;
    USB_ENDPOINT,        //bDescriptorType;
    0x00,                //bEndpointAddress;   use endpoint_4, USB_Hid_CommIf_Create set this parameter
    USB_EP_INTR,         //bmAttributes;
    0x40,   //wMaxPacketSize[2];
    0x00,   //wMaxPacketSize[2];
    0x00    //bInterval; USB_Hid_CommIf_Create set this parameter
};

const uint8_t audio_hid_report_dscr[] = {
    /* DFU */
    0x06, 0x13, 0xFF, // Usage Page (Vendor-defined ,0xFF13)
    0x09, 0x01,       // Usage      (Vendor-defined ,0x0001)
    0xA1, 0x01,       // Collection (Application)
    0x15, 0x00,       // Logical minimum (0)
    0x26, 0xFF, 0x00, // Logical maximum (255)
    // Output Report
    0x85, 0x06,       // Report Id (6)
    0x09, 0x00,       // Usage  (Vendor-defined ,0x0000)
    0x75, 0x08,       // Report Size  (8)
    0x95, 0x3D,       // Report Count (61)
    0x91, 0x02,       // Output (Data,Value,Absolute,Bit Field)
    // Input Report
    0x85, 0x07,       // Report Id (7)
    0x09, 0x00,       // Usage  (Vendor-defined ,0x0000)
    0x75, 0x08,       // Report Size  (8)
    0x95, 0x3D,       // Report Count (61)
    0x81, 0x02,       // Input    (Data,Value,Absolute,Bit Field)
    0xC0,             // End Collection

#ifdef MTK_USB_AUDIO_V1_ENABLE
    /* Audio Control */
    0x05, 0x0C,  // Usage Page (Consumer)
    0x09, 0x01,  // Usage      (Consumer Control)
    0xA1, 0x01,  // Collection (Application)
    // Input Report
    0x85, 0x0C,  // Report Id (12)
    0x15, 0x00,  // Logical minimum (0)
    0x25, 0x01,  // Logical maximum (1)
    0x09, 0xE9,  // Usage (Volume Increment)
    0x09, 0xEA,  // Usage (Volume Decrement)
    0x09, 0xE2,  // Usage (Mute)
    0x09, 0xCD,  // Usage (Play/Pause)
    0x09, 0xB5,  // Usage (Scan Next Track)
    0x09, 0xB6,  // Usage (Scan Previous Track)
    0x75, 0x01,  // Report Size  (1)
    0x95, 0x06,  // Report Count (6)
    0x81, 0x02,  // Input (Data,Value,Absolute,Bit Field)
    0x09, 0x00,  // Usage (Unassigned)
    0x95, 0x02,  // Report Count (2)
    0x81, 0x02,  // Input (Data,Value,Absolute,Bit Field)
    0xC0,        // End Collection
#endif

#ifdef AIR_MS_TEAMS_ENABLE
    0x06, 0x99, 0xFF,  // Usage Page (Vendor-defined 0xFF99)
    0x09, 0x01,        // Usage (Vendor-defined 0x0001)
    0xA1, 0x01,        // Collection (Application)
    // Feature Report
    0x85, 0x9A,        // Report Id (154)
    0x15, 0x00,        // Logical minimum (0)
    0x26, 0xFF, 0x00,  // Logical maximum (255)
    0x95, 0x3F,        // Report Count (63)
    0x75, 0x08,        // Report Size  (8)
    0x09, 0x03,        // Usage   (Vendor-defined 0x0003)
    0xB1, 0x02,        // Feature (Data,Value,Absolute,Non-volatile,Buffered Bytes)
    // Input Report
    0x85, 0x9B,        // Report Id (155)
    0x15, 0x00,        // Logical minimum (0)
    0x25, 0x01,        // Logical maximum (1)
    0x95, 0x01,        // Report Count (1)
    0x75, 0x01,        // Report Size  (1)
    0x09, 0x04,        // Usage (Vendor-defined 0x0004)
    0x81, 0x06,        // Input (Data,Value,Relative,Bit Field)
    0x75, 0x01,        // Report Size  (1)
    0x95, 0x07,        // Report Count (7)
    0x81, 0x01,        // Input (Constant,Array,Absolute,Bit Field)
    0xC0,              // End Collection

    /* Telephony */
    0x05, 0x0B,        // Usage Page (Telephony)
    0x09, 0x05,        // Usage (Headset)
    0xA1, 0x01,        // Collection (Application)
    0x15, 0x00,        // Logical minimum (0)
    0x25, 0x01,        // Logical maximum (1)
    0x75, 0x01,        // Report Size (1)
    0x85, 0x05,        // Report Id (5)
    // Input
    0x09, 0x20,        // Usage (Hook Switch)
    0x95, 0x01,        // Report Count (1)
    0x81, 0x22,        // Input (Data,Value,Absolute,Bit Field)
    0x09, 0x2F,        // Usage (Phone Mute)
    0x95, 0x01,        // Report Count (1)
    0x81, 0x06,        // Input (Data,Value,Relative,Bit Field)
    0x09, 0x24,        // Usage (Redial)
    0x09, 0x21,        // Usage (Flash)
    0x09, 0x97,        // Usage (Line Busy Tone)
    0x09, 0x2A,        // Usage (Line)
    0x09, 0x50,        // Usage (Speed Dial)
    0x95, 0x05,        // Report Count (5)
    0x81, 0x06,        // Input (Data,Value,Relative,Bit Field)
    0x09, 0x07,        // Usage (Programmable Button)
    0x05, 0x09,        // Usage Page (Button)
    0x09, 0x01,        // Usage (Button 1)
    0x75, 0x01,        // Report Size (1)
    0x95, 0x01,        // Report Count (1)
    0x81, 0x02,        // Input (Data,Value,Absolute,Bit Field)
    // Output
    0x05, 0x08,        // Usage Page (LEDs)
    0x85, 0x05,        // Report Id (5)
    0x09, 0x17,        // Usage (Off-Hook)
    0x09, 0x09,        // Usage (Mute)
    0x09, 0x18,        // Usage (Ring)
    0x09, 0x20,        // Usage (Hold)
    0x09, 0x21,        // Usage (Microphone)
    0x09, 0x2A,        // Usage (Line)
    0x95, 0x06,        // Report Count (6)
    0x91, 0x22,        // Output (Data,Value,Absolute,Non-volatile,Bit Field)
    0x95, 0x02,        // Report Count (2)
    0x91, 0x01,        // Output (Constant,Array,Absolute,Non-volatile,Bit Field)
    0xC0,              // End Collection
#endif

#ifdef USB_HID_KEYBOARD_ENABLE
    0x05, 0x01,        // Usage Page (Generic Desktop Controls)
    0x09, 0x06,        // Usage         (Keyboard)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x0A,        // Report Id (10)
    0x05, 0x07,        // Usage Page (Key)
    0x19, 0xe0,        // Usage minimum (0xE0)
    0x29, 0xe7,        // Usage minimum (0xE0)
    0x15, 0x00,        // Logical minimum (0)
    0x25, 0x01,        // Logical maximum (1)
    0x75, 0x01,        // Report Size  (1)
    0x95, 0x08,        // Report Count (8)
    0x81, 0x02,        // Input (Data,Value,Absolute,Bit Field)
    0x95, 0x01,        // Report Count (1)
    0x75, 0x08,        // Report Size  (8)
    0x81, 0x01,        // Input (Constant,Array,Absolute,Bit Field)
    0x95, 0x03,        // Report Count (3)
    0x75, 0x01,        // Report Size  (1)
    0x05, 0x08,        // Usage Page (LED)
    0x19, 0x01,        // Usage minimum (0x01)
    0x29, 0x03,        // Usage minimum (0x03)
    0x91, 0x02,        // Output (Data,Value,Absolute,Bit Field)
    0x95, 0x05,        // Report Count (5)
    0x75, 0x01,        // Report Size  (1)
    0x91, 0x01,        // Output (Constant,Array,Absolute,Bit Field)
    0x95, 0x06,        // Report Count (6)
    0x75, 0x08,        // Report Size  (8)
    0x15, 0x00,        // Logical minimum (0)
    0x25, 0x68,        // Logical maximum (104)
    0x05, 0x07,        // Usage Page (Key)
    0x19, 0x00,        // Usage minimum (0x00)
    0x29, 0x68,        // Usage minimum (104)
    0x81, 0x00,        // Input (Data,Array,Absolute,Bit Field)
    0xC0,              // End Collection
#endif

#ifdef CUSTOM_GYROMETER
    // Magic value: "#AndroidHeadTracker#1.0"
    0x05, 0x20,        // HID_USAGE_PAGE_SENSOR
    0x09, 0xE1,        // HID_USAGE_SENSOR_TYPE_OTHER_CUSTOM
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x0E,        // Report Id (14)
    0x0A, 0x08, 0x03,  // HID_USAGE_SENSOR_PROPERTY_SENSOR_DESCRIPTION
    0x15, 0x00,        // Logical minimum (0)
    0x25, 0xFF,        // Logical maximum (255)
    0x75, 0x08,        // Report Size  (8)
    0x95, 0x17,        // Report Count (23)
    0xB1, 0x03,        // Feature (Const_Var_Abs)
    // UUID
    0x0A, 0x02, 0x03,  // HID_USAGE_SENSOR_PROPERTY_PERSISTENT_UNIQUE_ID
    0x15, 0x00,        // Logical minimum (0)
    0x25, 0xFF,        // Logical maximum (255)
    0x75, 0x08,        // Report Size  (8)
    0x95, 0x10,        // Report Count (16)
    0xB1, 0x03,        // Feature (Const_Var_Abs)

    //Feature report 1 (read/write).
    0x85, 0x01,        // Report Id (1)
    // 1-bit on/off reporting state
    0x0A, 0x16, 0x03,  // HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE
    0x15, 0x00,        // Logical minimum (0)
    0x25, 0x01,        // Logical maximum (1)
    0x75, 0x01,        // Report Size  (1)
    0x95, 0x01,        // Report Count (1)
    0xA1, 0x02,        // Collection (Logical)
    0x0A, 0x40, 0x08,  // HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS
    0x0A, 0x41, 0x08,  // HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS
    0xB1, 0x00,        // Feature (DATA_ARR_ABS)
    0xC0,              // End Collection
    // 1-bit on/off power state
    0x0A, 0x19, 0x03,  // HID_USAGE_SENSOR_PROPERTY_POWER_STATE
    0x15, 0x00,        // Logical minimum (0)
    0x25, 0x01,        // Logical maximum (1)
    0x75, 0x01,        // Report Size  (1)
    0x95, 0x01,        // Report Count (1)
    0xA1, 0x02,        // Collection (Logical)
    0x0A, 0x55, 0x08,  // HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D4_POWER_OFF
    0x0A, 0x51, 0x08,  // ID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER
    0xB1, 0x00,        // Feature (DATA_ARR_ABS)
    0xC0,              // End Collection
    // 6-bit reporting interval, with values [0x00..0x3F] corresponding to [10ms..100ms]
    0x0A, 0x0E, 0x03,  // HID_USAGE_SENSOR_PROPERTY_REPORT_INTERVAL
    0x15, 0x00,        // Logical minimum (0)
    0x25, 0x3F,        // Logical maximum (63)
    0x35, 0x0A,        // Physical minimum (10)
    0x45, 0x64,        // Physical maximum (100)
    0x75, 0x06,        // Report Size  (6)
    0x95, 0x01,        // Report Count (1)
    0x66, 0x01, 0x10,  // HID_USAGE_SENSOR_UNITS_SECOND
    0x55, 0x0D,        // HID_UNIT_EXPONENT 10^-3
    0xB1, 0x02,        // Feature (DATA_VAR_ABS)
    // Orientation as rotation vector (scaled to [-pi..pi] rad)
    0x85, 0x03,        // Report Id (3)
    0x0A, 0x44, 0x05,  // HID_USAGE_SENSOR_DATA_CUSTOM_VALUE_1
    0x16, 0x01, 0x80,  // Logical minimum (-32767)
    0x26, 0xFF, 0x7F,  // Logical minimum (32767)
    0x37, 0x60, 0x4F,  // Physical minimum_32 (-314159265)
          0x46, 0xED,
    0x47, 0xA1, 0xB0,  // Physical minimum_32 (314159265)
          0xB9, 0x12,
    0x55, 0x08,        // HID_UNIT_EXPONENT 10^-8
    0x75, 0x10,        // Report Size  (16)
    0x95, 0x03,        // Report Count (3)
    0x81, 0x02,        // Input (Data,Value,Absolute,Bit Field)
    // Angular velocity as rotation vector (scaled to [-32..32] rad/sec)
    0x0A, 0x45, 0x05,  // HID_USAGE_SENSOR_DATA_CUSTOM_VALUE_2
    0x16, 0x01, 0x80,  // Logical minimum (-32767)
    0x26, 0xFF, 0x7F,  // Logical minimum (32767)
    0x35, 0xE0,        // Physical minimum (-32)
    0x45, 0x20,        // Physical maximum (32)
    0x55, 0x00,        // HID_UNIT_EXPONENT 10^0
    0x75, 0x10,        // Report Size  (16)
    0x95, 0x03,        // Report Count (3)
    0x81, 0x02,        // Input (Data,Value,Absolute,Bit Field)
    // Reference frame reset counter
    0x0A, 0x46, 0x05,  // HID_USAGE_SENSOR_DATA_CUSTOM_VALUE_3
    0x15, 0x00,        // Logical minimum (0)
    0x25, 0xFF,        // Logical maximum (255)
    0x35, 0x00,        // Physical minimum (0)
    0x45, 0x00,        // Physical maximum (0)
    0x55, 0x00,        // HID_UNIT_EXPONENT 10^0
    0x75, 0x08,        // Report Size  (8)
    0x95, 0x01,        // Report Count (1)
    0x81, 0x02,        // Input (Data,Value,Absolute,Bit Field)
    0xC0,              // End Collection
#endif
#if 0
    0x05, 0x20,        // HID_USAGE_PAGE_SENSOR
    0x09, 0x01,        // HID_USAGE_SENSOR_TYPE_COLLECTION
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x0E,        // Report Id (2)
    // 1-bit on/off reporting state
    0x0A, 0x16, 0x03,  // HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE
    0x15, 0x00,        // Logical minimum (0)
    0x25, 0x01,        // Logical maximum (1)
    0x75, 0x01,        // Report Size  (1)
    0x95, 0x01,        // Report Count (1)
    0xA1, 0x02,        // Collection (Logical)
    0x0A, 0x40, 0x08,  // HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_NO_EVENTS
    0x0A, 0x41, 0x08,  // HID_USAGE_SENSOR_PROPERTY_REPORTING_STATE_ALL_EVENTS
    0xB1, 0x00,        // Feature (DATA_ARR_ABS)
    0xC0,              // End Collection
    // 1-bit on/off power state
    0x0A, 0x19, 0x03,  // HID_USAGE_SENSOR_PROPERTY_POWER_STATE
    0x15, 0x00,        // Logical minimum (0)
    0x25, 0x01,        // Logical maximum (1)
    0x75, 0x01,        // Report Size  (1)
    0x95, 0x01,        // Report Count (1)
    0xA1, 0x02,        // Collection (Logical)
    0x0A, 0x55, 0x08,  // HID_USAGE_SENSOR_PROPERTY_POWER_STATE_D4_POWER_OFF
    0x0A, 0x51, 0x08,  // ID_USAGE_SENSOR_PROPERTY_POWER_STATE_D0_FULL_POWER
    0xB1, 0x00,        // Feature (DATA_ARR_ABS)
    0xC0,              // End Collection
    // 6-bit reporting interval, with values [0x00..0x3F] corresponding to [10ms..100ms]
    0x0A, 0x0E, 0x03,  // HID_USAGE_SENSOR_PROPERTY_REPORT_INTERVAL
    0x15, 0x00,        // Logical minimum (0)
    0x25, 0x3F,        // Logical maximum (63)
    0x35, 0x0A,        // Physical minimum (10)
    0x45, 0x64,        // Physical maximum (100)
    0x75, 0x06,        // Report Size  (6)
    0x95, 0x01,        // Report Count (1)
    0x66, 0x01, 0x10,  // HID_USAGE_SENSOR_UNITS_SECOND
    0x55, 0x0D,        // HID_UNIT_EXPONENT 10^-3
    0xB1, 0x02,        // Feature (DATA_VAR_ABS)

    // Orientation as rotation vector (scaled to [-pi..pi] rad)
    0x85, 0x03,        // Report Id (3)
    0x0A, 0x44, 0x05,  // HID_USAGE_SENSOR_DATA_CUSTOM_VALUE_1
    0x16, 0x01, 0x80,  // Logical minimum (-32767)
    0x26, 0xFF, 0x7F,  // Logical minimum (32767)
    0x37, 0x60, 0x4F,  // Physical minimum_32 (-314159265)
          0x46, 0xED,
    0x47, 0xA1, 0xB0,  // Physical minimum_32 (314159265)
          0xB9, 0x12,
    0x55, 0x08,        // HID_UNIT_EXPONENT 10^-8
    0x75, 0x10,        // Report Size  (16)
    0x95, 0x03,        // Report Count (3)
    0x81, 0x02,        // Input (Data,Value,Absolute,Bit Field)
    // Angular velocity as rotation vector (scaled to [-32..32] rad/sec)
    0x0A, 0x45, 0x05,  // HID_USAGE_SENSOR_DATA_CUSTOM_VALUE_2
    0x16, 0x01, 0x80,  // Logical minimum (-32767)
    0x26, 0xFF, 0x7F,  // Logical minimum (32767)
    0x35, 0xE0,        // Physical minimum (-32)
    0x45, 0x20,        // Physical maximum (32)
    0x55, 0x00,        // HID_UNIT_EXPONENT 10^0
    0x75, 0x10,        // Report Size  (16)
    0x95, 0x03,        // Report Count (3)
    0x81, 0x02,        // Input (Data,Value,Absolute,Bit Field)
    0xC0,              // End Collection
#endif

#ifdef AIR_CFU_ENABLE
    /* CFU */
    0x06, 0x33, 0xFF,  // Usage Page (Vendor-defined 0xFF33)
    0x09, 0x05,        // Usage (Vendor-defined 0x0005)
    0xA1, 0x01,        // Collection (Application)
    0x15, 0x00,        // Logical minimum (0)
    0x26, 0xFF, 0x00,  // Logical maximum (255)
    // In Report
    0x85, 0x20,        // Report Id (32)
    0x09, 0x52,        // Usage (Vendor-defined 0x0052, DUMMY_INPUT_USAGE)
    0x75, 0x08,        // Report Size  (8)
    0x95, 0x01,        // Report Count (1)
    0x81, 0x02,        // Input (Data,Array,Absolute,Bit Field)
    // In Report
    0x85, 0x22,        // Report Id (34)
    0x09, 0x29,        // Usage (Vendor-defined 0x0029, CONTENT_RESPONSE_INPUT_USAGE)
    0x75, 0x20,        // Report Size  (32)
    0x95, 0x04,        // Report Count (4)
    0x81, 0x02,        // Input (Data,Array,Absolute,Bit Field)
    // In Report
    0x85, 0x25,        // Report Id (37)
    0x09, 0x1D,        // Usage (Vendor-defined 0x001D, OFFER_RESPONSE_INPUT_USAGE)
    0x75, 0x20,        // Report Size  (32)
    0x95, 0x04,        // Report Count (4)
    0x81, 0x02,        // Input (Data,Array,Absolute,Bit Field)
    // Output Report
    0x85, 0x20,        // Report Id (32)
    0x09, 0x31,        // Usage (Vendor-defined 0x0031, CONTENT_OUTPUT_USAGE)
    0x75, 0x08,        // Report Size  (8)
    0x95, 0x3C,        // Report Count (60)
    0x92, 0x02, 0x01,  // Output (Data,Array,Absolute,Buffered Bytes)
    // Output Report
    0x85, 0x25,        // Report Id (37)
    0x09, 0x21,        // Usage (Vendor-defined 0x0021, OFFER_OUTPUT_USAGE)
    0x75, 0x20,        // Report Size  (32)
    0x95, 0x04,        // Report Count (4)
    0x92, 0x02, 0x01,  // Output (Data,Array,Absolute,Buffered Bytes)
    // Feature Report
    0x85, 0x20,        // Report Id (32)
    0x09, 0x42,        // Usage (Vendor-defined 0x0042, VERSIONS_FEATURE_USAGE)
    0x75, 0x08,        // Report Size  (8)
    0x95, 0x3C,        // Report Count (60)
    0xB2, 0x02, 0x01,  // Feature(Data,Array,Absolute,Buffered Bytes)
    0xC0               // End Collection
#endif

#if 0
    /* Customer */
    0x06, 0x14, 0xFF, // Usage Page (Vendor-defined ,0xFF14)
    0x09, 0x01,       // Usage      (Vendor-defined ,0x0001)
    0xA1, 0x01,       // Collection (Application)
    0x15, 0x00,       // Logical minimum (0)
    0x26, 0xFF, 0x00, // Logical maximum (255)
    /* ----- Add more HID report ,begin -----*/
    // Feature Report
    0x85, 0x14,       // Report Id (20)
    0x09, 0x00,       // Usage  (Vendor-defined ,0x0000)
    0x75, 0x08,       // Report Size  (8)
    0x95, 0x05,       // Report Count (5)
    0xB2, 0x02, 0x01, // Feature  (Data,Value,Absolute,Buffered Bytes)
    /* ----- Add more HID report ,end -----*/
    0xC0              // End Collection
#endif

};

uint8_t USB_Audio_HID_Get_DataIf_Num(void)
{
    uint8_t if_id;

    if (g_UsbAudioHID.data_interface_id == 0xFF) {
        /* Get resource number and register to gUsbDevice */
        g_UsbAudioHID.data_if_info = USB_Get_Interface_Number(&if_id);
        g_UsbAudioHID.data_interface_id = if_id;
    }

    return g_UsbAudioHID.data_interface_id;
}

USB_Audio_HID_t USB_Audio_HID_TX(uint8_t *data_buf, uint8_t data_len)
{
    uint32_t i, ep_tx_id, usb_wait;
    bool chrger_detect = FALSE;
    USB_Audio_HID_t ret = USB_Audio_HID_STATUS_OK;

    /* Get the charger status */
    chrger_detect = pmu_get_chr_detect_value();

    /* Check the charger (Vbus) is ready for USB which means the USB cable is connected or not */
    if (chrger_detect != PMU_OK) {
        LOG_MSGID_E(USBAUDIO_DRV, "USB_Audio_HID_TX Detect Charger FAIL!", 0);
        return USB_Audio_HID_CHARGER_DETECT_ERROR;
    }

    /* Get the USB EP num of HID TX*/
    ep_tx_id = g_UsbAudioHID.comm_ep_intr_info->ep_status.epin_status.byEP;

    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_HID_TX ep_tx_id[%x] data_len[%x] chrger_detect[%x]", 3, ep_tx_id, data_len, chrger_detect);

    /* Check the length is valid */
    if (data_len == 0) {
        LOG_MSGID_E(USBAUDIO_DRV, "USB_Audio_HID_TX data length is zero!", 0);
        return USB_Audio_HID_TX_LEN_IS_ZERO;
    }

    if (data_len > USB_AUDIO_HID_REPORT_64BYTES) {
        LOG_MSGID_E(USBAUDIO_DRV, "USB_Audio_HID_TX data length[%d] is larger than 64 byte", 1, data_len);
        return USB_Audio_HID_TX_LEN_TOO_LARGE;
    }

    /* Send data by endpoint*/
    hal_usb_clear_tx_endpoint_fifo(ep_tx_id, HAL_USB_EP_TRANSFER_INTR, true);

    hal_usb_write_endpoint_fifo(ep_tx_id, data_len, data_buf);
    hal_usb_set_endpoint_tx_ready_toggle(ep_tx_id);

    usb_wait = 0;
    while (!hal_usb_is_endpoint_tx_empty(ep_tx_id)) {
        if (usb_wait > 0x1000) {
            LOG_MSGID_E(USBAUDIO_DRV, "USB_Audio_HID_TX send data error!", 0);
            ret = USB_Audio_HID_SEND_DATA_ERROR;
            break;
        }
        usb_wait++;
    }

    for (i = 0; i < data_len; i += 8) {
        LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_HID_TX data [%02X][%02X][%02X][%02X][%02X][%02X][%02X][%02X]", 8,
                    data_buf[i + 0], data_buf[i + 1], data_buf[i + 2], data_buf[i + 3], data_buf[i + 4], data_buf[i + 5], data_buf[i + 6], data_buf[i + 7]);
    }

    return ret;
}

/* Adjust the volume by itself (not from Host)
    Refer to : case USB_AUDIO_SET_VOLUME in usb_main.c
    0x01010040(EP,Channel,Volume)
    Default : 64(0x40)    min : 0 (0x0)    Max : 100 (0x64)*/
uint32_t g_ep1_ch1_volume =  0x01010040;
uint32_t g_ep1_ch2_volume =  0x01020040;

USB_Audio_HID_t USB_Audio_HID_VolumeUp(uint8_t step)
{
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_HID_VolumeUp, step= %d", 1, step);
    uint8_t i = 0;
    USB_Audio_HID_t ret = USB_Audio_HID_STATUS_OK;

    uint8_t  set_data[2] = {USB_HID_AUDIO_REPORT_ID, USB_HID_VOLUME_UP};
    uint8_t  reset_data[2] = {USB_HID_AUDIO_REPORT_ID, USB_HID_NONE};

    for (i = 0; i < step; i++) {
        ret = USB_Audio_HID_TX(&set_data[0], sizeof(set_data));
        ret = USB_Audio_HID_TX(&reset_data[0], sizeof(reset_data));
    }

    if (ret != USB_Audio_HID_STATUS_OK) {
        LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_HID_VolumeUp - Adjust", 0);
        USB_Send_Message(USB_AUDIO_SET_VOLUME, (void *) g_ep1_ch1_volume);
        USB_Send_Message(USB_AUDIO_SET_VOLUME, (void *) g_ep1_ch2_volume);

        /* Make sure the volume Maximun is 100 */
        if ((g_ep1_ch1_volume & 0xFF) < 100) {
            g_ep1_ch1_volume++;
        }
        if ((g_ep1_ch2_volume & 0xFF) < 100) {
            g_ep1_ch2_volume++;
        }
    }

    return ret;
};

USB_Audio_HID_t USB_Audio_HID_VolumeDown(uint8_t step)
{
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_HID_VolumeDown, step= %d", 1, step);
    uint8_t i = 0;
    USB_Audio_HID_t ret = USB_Audio_HID_STATUS_OK;

    uint8_t  set_data[2] = {USB_HID_AUDIO_REPORT_ID, USB_HID_VOLUME_DOWN};
    uint8_t  reset_data[2] = {USB_HID_AUDIO_REPORT_ID, USB_HID_NONE};

    for (i = 0; i < step; i++) {
        ret = USB_Audio_HID_TX(&set_data[0], sizeof(set_data));
        ret = USB_Audio_HID_TX(&reset_data[0], sizeof(reset_data));
    }

    if (ret != USB_Audio_HID_STATUS_OK) {
        LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_HID_VolumeDown - Adjust", 0);
        USB_Send_Message(USB_AUDIO_SET_VOLUME, (void *) g_ep1_ch1_volume);
        USB_Send_Message(USB_AUDIO_SET_VOLUME, (void *) g_ep1_ch2_volume);

        /* Make sure the volume minimum is 0 */
        if (((g_ep1_ch1_volume & 0xFF) > 0) && (g_ep1_ch1_volume != 0)) {
            g_ep1_ch1_volume--;
        }
        if (((g_ep1_ch2_volume & 0xFF) > 0) && (g_ep1_ch2_volume != 0)) {
            g_ep1_ch2_volume--;
        }
    }

    return ret;
}

USB_Audio_HID_t USB_Audio_HID_VolumeMute()
{
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_HID_VolumeMute", 0);
    USB_Audio_HID_t ret = USB_Audio_HID_STATUS_OK;

    uint8_t  set_data[2] = {USB_HID_AUDIO_REPORT_ID, USB_HID_VOLUME_MUTE};
    uint8_t  reset_data[2] = {USB_HID_AUDIO_REPORT_ID, USB_HID_NONE};

    ret = USB_Audio_HID_TX(&set_data[0], sizeof(set_data));
    ret = USB_Audio_HID_TX(&reset_data[0], sizeof(reset_data));

    return ret;
};

USB_Audio_HID_t USB_Audio_HID_PlayPause()
{
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_HID_PlayPause", 0);
    USB_Audio_HID_t ret = USB_Audio_HID_STATUS_OK;

    uint8_t  set_data[2] = {USB_HID_AUDIO_REPORT_ID, USB_HID_PLAY_PAUSE};
    uint8_t  reset_data[2] = {USB_HID_AUDIO_REPORT_ID, USB_HID_NONE};

    ret = USB_Audio_HID_TX(&set_data[0], sizeof(set_data));
    ret = USB_Audio_HID_TX(&reset_data[0], sizeof(reset_data));

    return ret;
};

USB_Audio_HID_t USB_Audio_HID_PlayPause_RejectCall()
{
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_HID_PlayPause_RejectCall", 0);
    USB_Audio_HID_t ret = USB_Audio_HID_STATUS_OK;

    uint8_t  set_data[2] = {USB_HID_AUDIO_REPORT_ID, USB_HID_PLAY_PAUSE};
    uint8_t  reset_data[2] = {USB_HID_AUDIO_REPORT_ID, USB_HID_NONE};

    ret = USB_Audio_HID_TX(&set_data[0], sizeof(set_data));

    /* Reject phone call by pulling high about 2 sec */
    hal_gpt_delay_ms(2000);

    ret = USB_Audio_HID_TX(&reset_data[0], sizeof(reset_data));

    return ret;
};


USB_Audio_HID_t USB_Audio_HID_ScanNextTrack()
{
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_HID_ScanNextTrack", 0);
    USB_Audio_HID_t ret = USB_Audio_HID_STATUS_OK;

    uint8_t  set_data[2] = {USB_HID_AUDIO_REPORT_ID, USB_HID_SCAN_NEXT_TRACK};
    uint8_t  reset_data[2] = {USB_HID_AUDIO_REPORT_ID, USB_HID_NONE};

    ret = USB_Audio_HID_TX(&set_data[0], sizeof(set_data));
    ret = USB_Audio_HID_TX(&reset_data[0], sizeof(reset_data));

    return ret;
};

USB_Audio_HID_t USB_Audio_HID_ScanPreviousTrack()
{
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_HID_ScanPreviousTrack", 0);
    USB_Audio_HID_t ret = USB_Audio_HID_STATUS_OK;

    uint8_t  set_data[2] = {USB_HID_AUDIO_REPORT_ID, USB_HID_SCAN_PREVIOUS_TRACK};
    uint8_t  reset_data[2] = {USB_HID_AUDIO_REPORT_ID, USB_HID_NONE};

    ret = USB_Audio_HID_TX(&set_data[0], sizeof(set_data));
    ret = USB_Audio_HID_TX(&reset_data[0], sizeof(reset_data));

    return ret;
}

USB_Audio_HID_t USB_Audio_HID_Win_Key_Func(USB_HID_WINDOWS_KEY_MODIFIER_FUNC_t key)
{
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_HID_Win_Key", 0);

    USB_Audio_HID_t ret = USB_Audio_HID_STATUS_OK;

    uint8_t  win_key_data[9] = {0x0A, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t  win_key_modifier_data[9] = {0x0A, 0x08, 0x00, key, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t  reset_data[9] = {0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    if(key == USB_HID_KEYBOARD_KEY_WIN_KEY_ONLY)
    {
        ret = USB_Audio_HID_TX(&win_key_data[0], sizeof(win_key_data));
        hal_gpt_delay_ms(150);
        ret = USB_Audio_HID_TX(&reset_data[0], sizeof(reset_data));
    }
    else
    {
        ret = USB_Audio_HID_TX(&win_key_data[0], sizeof(win_key_data));
        hal_gpt_delay_ms(260);
        ret = USB_Audio_HID_TX(&win_key_modifier_data[0], sizeof(win_key_modifier_data));
        hal_gpt_delay_ms(100);
        ret = USB_Audio_HID_TX(&win_key_data[0], sizeof(win_key_data));
        hal_gpt_delay_ms(100);
        ret = USB_Audio_HID_TX(&reset_data[0], sizeof(reset_data));
    }

    return ret;
}

USB_Audio_HID_t USB_Audio_HID_Gyroscope_Send_Rotate_Angle(int16_t rx, int16_t  ry, int16_t  rz)
{
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_HID_Gyroscope_Send_Rotate_Angle rx = %x, ry = %x, rz = %x", 3, rx, ry, rz);
    USB_Audio_HID_t ret = USB_Audio_HID_STATUS_OK;

    uint8_t  set_data[7] = {USB_HID_Gyroscope_DATA_REPORT_ID, (rx & 0xFF), ((rx & 0xFF00)>>8), (ry & 0xFF), ((ry & 0xFF00)>>8), (rz & 0xFF), ((rz & 0xFF00)>>8)};
    LOG_MSGID_I(USBAUDIO_DRV, "check data [0]=%x, [1]=%x, [2]=%x, [3]=%x, [4]=%x, [5]=%x, [6]=%x", 7, set_data[0], set_data[1], set_data[2], set_data[3], set_data[4], set_data[5], set_data[6]);

    ret = USB_Audio_HID_TX(&set_data[0], sizeof(set_data));

    return ret;
}

USB_Audio_HID_t USB_Audio_HID_Gyroscope_Send_Rotate_Speed(int16_t vx, int16_t  vy, int16_t  vz)
{
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_HID_Gyroscope_Send_Rotate_Speed vx = %x, vx = %x, vx = %x", 3, vx, vy, vz);
    USB_Audio_HID_t ret = USB_Audio_HID_STATUS_OK;

    uint8_t  set_data[7] = {USB_HID_Gyroscope_DATA_REPORT_ID, (vx & 0xFF), ((vx & 0xFF00)>>8), (vy & 0xFF), ((vy & 0xFF00)>>8), (vz & 0xFF), ((vz & 0xFF00)>>8)};
    LOG_MSGID_I(USBAUDIO_DRV, "check data [0]=%x, [1]=%x, [2]=%x, [3]=%x, [4]=%x, [5]=%x, [6]=%x", 7, set_data[0], set_data[1], set_data[2], set_data[3], set_data[4], set_data[5], set_data[6]);

    ret = USB_Audio_HID_TX(&set_data[0], sizeof(set_data));

    return ret;
}

USB_Audio_HID_t USB_Audio_HID_Gyroscope_Send_Frame_Reset(void)
{
    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_HID_Gyroscope_Send_Frame_Reset", 0);
    USB_Audio_HID_t ret = USB_Audio_HID_STATUS_OK;
    static uint8_t Gyroscope_Frame_Count = 0;

    Gyroscope_Frame_Count++;
    uint8_t  set_data[2] = {USB_HID_Gyroscope_DATA_REPORT_ID, Gyroscope_Frame_Count};

    ret = USB_Audio_HID_TX(&set_data[0], sizeof(set_data));

    return ret;
}

ATTR_RWDATA_IN_NONCACHED_RAM_4BYTE_ALIGN uint8_t usb_rx[USB_AUDIO_HID_REPORT_64BYTES] = {0};
ATTR_RWDATA_IN_NONCACHED_RAM_4BYTE_ALIGN uint8_t usb_tx[USB_AUDIO_HID_REPORT_64BYTES] = {0};

extern virtual_read_write_point_t g_mux_usb_r_w_point[];

mux_usb_hid_callback g_mux_usb_hid_callback;
uint8_t usb_report_id = 0;
uint8_t usb_report_type = 0;
uint32_t usb_report_length = 0;

/************************************************************
*   USB HID Handler
*************************************************************/
static usb_hid_handler_t usb_hid_rx_handlers[USB_HID_HANDER_NUMS] = {
    {USB_HID_INVALID_REPORT_ID, 0, NULL},
    {USB_HID_INVALID_REPORT_ID, 0, NULL},
    {USB_HID_INVALID_REPORT_ID, 0, NULL},
    {USB_HID_INVALID_REPORT_ID, 0, NULL},
    {USB_HID_INVALID_REPORT_ID, 0, NULL},
    {USB_HID_INVALID_REPORT_ID, 0, NULL},
    {USB_HID_INVALID_REPORT_ID, 0, NULL},
    {USB_HID_INVALID_REPORT_ID, 0, NULL}
};

static usb_hid_handler_t usb_hid_tx_handlers[USB_HID_HANDER_NUMS] = {
    {USB_HID_INVALID_REPORT_ID, 0, NULL},
    {USB_HID_INVALID_REPORT_ID, 0, NULL},
    {USB_HID_INVALID_REPORT_ID, 0, NULL},
    {USB_HID_INVALID_REPORT_ID, 0, NULL},
    {USB_HID_INVALID_REPORT_ID, 0, NULL},
    {USB_HID_INVALID_REPORT_ID, 0, NULL},
    {USB_HID_INVALID_REPORT_ID, 0, NULL},
    {USB_HID_INVALID_REPORT_ID, 0, NULL}
};

int32_t usb_hid_handler_init()
{
    uint32_t index = 0;

    LOG_MSGID_I(USBAUDIO_DRV, "usb_hid_handler_init", 0);

    for (index = 0; index < USB_HID_HANDER_NUMS; index++) {
        usb_hid_rx_handlers[index].report_id = USB_HID_INVALID_REPORT_ID;
        usb_hid_rx_handlers[index].report_length = 0;
        usb_hid_rx_handlers[index].handler_callback = NULL;

        usb_hid_tx_handlers[index].report_id = USB_HID_INVALID_REPORT_ID;
        usb_hid_tx_handlers[index].report_length = 0;
        usb_hid_tx_handlers[index].handler_callback = NULL;
    }

    return USB_HID_HANDLER_ERROR_OK;
}

int32_t usb_hid_handler_rx_register(uint8_t report_id, uint8_t report_length, usb_hid_handler_callback *callback)
{
    uint32_t index = 0;

    for (index = 0; index < USB_HID_HANDER_NUMS; index++) {
        if (usb_hid_rx_handlers[index].report_id == USB_HID_INVALID_REPORT_ID) {
            usb_hid_rx_handlers[index].report_id = report_id;
            usb_hid_rx_handlers[index].report_length = report_length;
            usb_hid_rx_handlers[index].handler_callback = (void *)callback;
            LOG_MSGID_I(USBAUDIO_DRV, "usb_hid_handler_rx_register success to register index[%x] report_id[%x] report_length[%x] callback[%x]", 4,
                        index, usb_hid_rx_handlers[index].report_id, usb_hid_rx_handlers[index].report_length, usb_hid_rx_handlers[index].handler_callback);
            return USB_HID_HANDLER_ERROR_OK;
        } else if (usb_hid_rx_handlers[index].report_id == report_id) {
            LOG_MSGID_I(USBAUDIO_DRV, "usb_hid_handler_rx_register already registered index[%x] report_id[%x] report_length[%x] callback[%x]", 4,
                        index, usb_hid_rx_handlers[index].report_id, usb_hid_rx_handlers[index].report_length, usb_hid_rx_handlers[index].handler_callback);
            return USB_HID_HANDLER_ERROR_INITED;
        }
    }

    LOG_MSGID_E(USBAUDIO_DRV, "usb_hid_handler_rx_register callback pool is full!!", 0);
    return USB_HID_HANDLER_ERROR_FAIL;
}

int32_t usb_hid_handler_tx_register(uint8_t report_id, uint8_t report_length, usb_hid_handler_callback *callback)
{
    uint32_t index = 0;

    for (index = 0; index < USB_HID_HANDER_NUMS; index++) {
        if (usb_hid_tx_handlers[index].report_id == USB_HID_INVALID_REPORT_ID) {
            usb_hid_tx_handlers[index].report_id = report_id;
            usb_hid_tx_handlers[index].report_length = report_length;
            usb_hid_tx_handlers[index].handler_callback = (void *)callback;
            LOG_MSGID_I(USBAUDIO_DRV, "usb_hid_handler_tx_register success to register index[%x] report_id[%x] report_length[%x] callback[%x]", 4,
                        index, usb_hid_tx_handlers[index].report_id, usb_hid_tx_handlers[index].report_length, usb_hid_tx_handlers[index].handler_callback);
            return USB_HID_HANDLER_ERROR_OK;
        } else if (usb_hid_tx_handlers[index].report_id == report_id) {
            LOG_MSGID_I(USBAUDIO_DRV, "usb_hid_handler_tx_register already registered index[%x] report_id[%x] report_length[%x] callback[%x]", 4,
                        index, usb_hid_tx_handlers[index].report_id, usb_hid_tx_handlers[index].report_length, usb_hid_tx_handlers[index].handler_callback);
            return USB_HID_HANDLER_ERROR_INITED;
        }
    }

    LOG_MSGID_E(USBAUDIO_DRV, "usb_hid_handler_tx_register callback pool is full!!", 0);
    return USB_HID_HANDLER_ERROR_FAIL;
}

int32_t usb_hid_rx_handler_unregister(uint8_t report_id)
{
    uint32_t index = 0;

    for (index = 0; index < USB_HID_HANDER_NUMS; index++) {
        if (usb_hid_rx_handlers[index].report_id == report_id) {
            usb_hid_rx_handlers[index].report_id = USB_HID_INVALID_REPORT_ID;
            usb_hid_rx_handlers[index].report_length = 0;
            usb_hid_rx_handlers[index].handler_callback = NULL;
            LOG_MSGID_I(USBAUDIO_DRV, "usb_hid_rx_handler_unregister success to unregister index[%x] report_id[%x] report_length[%x] callback[%x]", 4,
                        index, usb_hid_rx_handlers[index].report_id, usb_hid_rx_handlers[index].report_length, usb_hid_rx_handlers[index].handler_callback);
            return USB_HID_HANDLER_ERROR_OK;
        }
    }

    LOG_MSGID_E(USBAUDIO_DRV, "usb_hid_rx_handler_unregister fail to unregister report_id[%x] handler", 1, report_id);
    return USB_HID_HANDLER_ERROR_FAIL;
}

int32_t usb_hid_tx_handler_unregister(uint8_t report_id)
{
    uint32_t index = 0;

    for (index = 0; index < USB_HID_HANDER_NUMS; index++) {
        if (usb_hid_tx_handlers[index].report_id == report_id) {
            usb_hid_tx_handlers[index].report_id = USB_HID_INVALID_REPORT_ID;
            usb_hid_tx_handlers[index].report_length = 0;
            usb_hid_tx_handlers[index].handler_callback = NULL;
            LOG_MSGID_I(USBAUDIO_DRV, "usb_hid_tx_handler_unregister success to unregister index[%x] report_id[%x] report_length[%x] callback[%x]", 4,
                        index, usb_hid_tx_handlers[index].report_id, usb_hid_tx_handlers[index].report_length, usb_hid_tx_handlers[index].handler_callback);
            return USB_HID_HANDLER_ERROR_OK;
        }
    }

    LOG_MSGID_E(USBAUDIO_DRV, "usb_hid_tx_handler_unregister fail to unregister report_id[%x] handler ", 1, report_id);
    return USB_HID_HANDLER_ERROR_FAIL;
}

/************************************************************
*   USB MUX Port Select
*************************************************************/
uint8_t USB_MUX_Port_Select(uint8_t report_id)
{
    /* Race */
    if (report_id == USB_AUDIO_HID_OUT_REPORT_ID ||
        report_id == USB_AUDIO_HID_IN_REPORT_ID) {
        LOG_MSGID_I(USB_DEBUG, "USB_MUX_Port_Select usb_mux_port[%x] report_id[%x]", 2, USB_MUX_PORT2, report_id);
        return USB_MUX_PORT2;
    }

    /* CFU */
    if (report_id == USB_CFU_DUMMY_IN_REPORT_ID    ||
        report_id == USB_CFU_CONTENT_IN_REPORT_ID  ||
        report_id == USB_CFU_OFFER_IN_REPORT_ID    ||
        report_id == USB_CFU_CONTENT_OUT_REPORT_ID ||
        report_id == USB_CFU_OFFER_OUT_REPORT_ID   ||
        report_id == USB_CFU_VERSION_FEATURE_REPORT_ID) {
        LOG_MSGID_I(USB_DEBUG, "USB_MUX_Port_Select usb_mux_port[%x] report_id[%x]", 2, USB_MUX_PORT1, report_id);
        return USB_MUX_PORT1;
    }

    LOG_MSGID_E(USBAUDIO_DRV, "USB_MUX_Port_Select Fail!", 0);
    return USB_MUX_PORT_UNUSE;
}

extern uint32_t hal_usb_ep0_pkt_len(void);
void USB_Audio_HID_Ep0_ReadData(uint8_t *data)
{
    uint32_t ep0_len, i;

    /* Check the EP0 data length is enough */
    i = 0;
    ep0_len = hal_usb_ep0_pkt_len();

    while (ep0_len < usb_report_length) {
        /* Add 100us for data ready */
        LOG_MSGID_I(USBAUDIO_DRV, "ep0_len[%x] < usb_report_length[%x] add 100us", 2, ep0_len, usb_report_length);
        hal_gpt_delay_us(100);

        /* Get EP0 length */
        ep0_len = hal_usb_ep0_pkt_len();

        /* Timeout 100 times */
        i++;
        if (i > 100) {
            LOG_MSGID_E(USBAUDIO_DRV, "ep0_len[%x] < usb_report_length[%x] i[%d] timeout", 3, ep0_len, usb_report_length, i);
            break;
        }
    }

    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_HID_Ep0_ReadData ep0_len[%x] usb_report_length[%x]", 2, ep0_len, usb_report_length);

    /* Return if EP0 data length = 0 */
    if (ep0_len == 0) {
        LOG_MSGID_E(USBAUDIO_DRV, "ep0_len == 0", 0);
        return;
    }

    /* Get data of set report by EP0 */
    hal_usb_read_endpoint_fifo(0, usb_report_length, &usb_rx);

    /* Send set report cmd to USB queue */
    USB_Send_Message(USB_HID_SET_REPORT, NULL);

    return;
}


void USB_Audio_HID_Ep0_DispatchData(uint8_t *data)
{
    bool stall = false;
    uint8_t i, usb_mux_port;

    LOG_MSGID_I(USBAUDIO_DRV, "USB_Audio_HID_Ep0_DispatchData", 0);
    LOG_MSGID_I(USBAUDIO_DRV, "USB HID %02X %02X %02X %02X %02X %02X %02X %02X", 8, usb_rx[0], usb_rx[1], usb_rx[2], usb_rx[3], usb_rx[4], usb_rx[5], usb_rx[6], usb_rx[7]);
    LOG_MSGID_I(USBAUDIO_DRV, "USB HID %02X %02X %02X %02X %02X %02X %02X %02X", 8, usb_rx[8], usb_rx[9], usb_rx[10], usb_rx[11], usb_rx[12], usb_rx[13], usb_rx[14], usb_rx[15]);
    LOG_MSGID_I(USBAUDIO_DRV, "USB HID %02X %02X %02X %02X %02X %02X %02X %02X", 8, usb_rx[16], usb_rx[17], usb_rx[18], usb_rx[19], usb_rx[20], usb_rx[21], usb_rx[22], usb_rx[23]);
    LOG_MSGID_I(USBAUDIO_DRV, "USB HID %02X %02X %02X %02X %02X %02X %02X %02X", 8, usb_rx[24], usb_rx[25], usb_rx[26], usb_rx[27], usb_rx[28], usb_rx[29], usb_rx[30], usb_rx[31]);
    LOG_MSGID_I(USBAUDIO_DRV, "USB HID %02X %02X %02X %02X %02X %02X %02X %02X", 8, usb_rx[32], usb_rx[33], usb_rx[34], usb_rx[35], usb_rx[36], usb_rx[37], usb_rx[38], usb_rx[39]);
    LOG_MSGID_I(USBAUDIO_DRV, "USB HID %02X %02X %02X %02X %02X %02X %02X %02X", 8, usb_rx[40], usb_rx[41], usb_rx[42], usb_rx[43], usb_rx[44], usb_rx[45], usb_rx[46], usb_rx[47]);
    LOG_MSGID_I(USBAUDIO_DRV, "USB HID %02X %02X %02X %02X %02X %02X %02X %02X", 8, usb_rx[48], usb_rx[49], usb_rx[50], usb_rx[51], usb_rx[52], usb_rx[53], usb_rx[54], usb_rx[55]);
    LOG_MSGID_I(USBAUDIO_DRV, "USB HID %02X %02X %02X %02X %02X %02X", 8, usb_rx[56], usb_rx[57], usb_rx[58], usb_rx[59], usb_rx[60], usb_rx[61]);

    /* HID handler dispatch the part of report */
    for (i = 0; i < USB_HID_HANDER_NUMS; i++) {
        /* According to report ID and send data to module*/
        if (usb_hid_rx_handlers[i].report_id == usb_rx[0]) {
            LOG_MSGID_I(USBAUDIO_DRV, "report_id[%x] is matched in HID rx handler table", 1, usb_rx[0]);
            usb_hid_rx_handlers[i].handler_callback(&usb_rx[0], usb_hid_rx_handlers[i].report_length);
            break;
        }
    }

    /* No HID report callback found */
    if (i == USB_HID_HANDER_NUMS) {
        /* Depen on report ID to send data to Race or CFU */
        usb_mux_port = USB_MUX_Port_Select(usb_rx[0]);

        if (usb_mux_port == USB_MUX_PORT_UNUSE) {
            LOG_MSGID_E(USBAUDIO_DRV, "No report ID [%x] match in Race or CFU", 1, usb_report_id);
        } else {
            g_mux_usb_hid_callback(usb_mux_port, HID_EVENT_READY_TO_READ, NULL);
        }
    }

    gUsbDevice.ep0_state = USB_EP0_RX_STATUS;
    hal_usb_update_endpoint_0_state(HAL_USB_EP0_DRV_STATE_READ_END, stall, true);

    return;
}

/* Parse class specific request */
void USB_HID_Ep0_Command(uint8_t port, Usb_Ep0_Status *pep0state, Usb_Command *pcmd)
{
    bool  bError = false;
    uint8_t index = 0, usb_mux_port;

    switch (pcmd->bRequest) {
        case USB_AUDIO_HID_SET_LINE_CODING:
            /* register handler to handle the get data*/
            //USB_Register_EP0_RxHdlr(USB_Hid_Ep0_SetLineCoding);
            //gUsbDevice.ep0_rx_handler = (usb_ep0_rx_ptr)USB_Audio1_Ep0_DataReceived;
            //USB_Set_Endpoint0_State(USB_EP0_RX);
            gUsbDevice.ep0_state = USB_EP0_RX;
            break;
        case USB_AUDIO_HID_GET_LINE_CODING:
            /* tell host the current setting */
            USB_Generate_EP0Data(pep0state, pcmd, &g_UsbAudioHID.line_coding, 7);
            break;
        case USB_AUDIO_HID_SET_CONTROL_LINE_STATE:
            g_UsbAudioHID.line_state = pcmd->wValue;
            /* do not need to do anything */
            break;
        case USB_AUDIO_HID_SEND_BREAK:
            /* Break behavior */
            switch (pcmd->wValue) {
                case 0xffff:
                    g_UsbAudioHID.break_detect = true;
                    break;
                case 0x0:
                    g_UsbAudioHID.break_detect = false;
                    g_UsbAudioHID.break_number++;
                    break;
                default:
                    g_UsbAudioHID.break_detect = true;
                    break;
            }
            break;
        case USB_AUDIO_HID_SEND_ENCAPSULATED_COMMAND:
        // case USB_AUDIO_HID_GET_ENCAPSULATED_RESPONSE:
        // case USB_AUDIO_HID_SET_COMM_FEATURE:
        case USB_AUDIO_HID_GET_COMM_FEATURE:
        case USB_AUDIO_HID_CLEAR_COMM_FEATURE:
            break;
        case USB_AUDIO_HID_SET_IDLE:
            LOG_MSGID_I(USB, "USB_AUDIO_HID_SET_IDLE", 0);
            usb_audio_hid_idle_val[gUsbDevice.cmd.wValue & 0x00FF] = (gUsbDevice.cmd.wValue & 0xFF00) >> 8;

            #if defined(USB_HOST_DETECT_PS) && defined(MTK_USB_AUDIO_V1_ENABLE) && defined(MTK_USB_AUDIO_HID_ENABLE)
            USB_Send_Message(USB_PS_DETECT, (void*)gUsbDevice.device_type);
            #endif
            break;
        case USB_AUDIO_HID_GET_IDLE:
            /* LOG_MSGID_I(USB, "USB_AUDIO_HID_GET_IDLE", 0); */
            USB_Generate_EP0Data(&gUsbDevice.ep0info, &gUsbDevice.cmd, &usb_audio_hid_idle_val[gUsbDevice.cmd.wValue & 0x00FF], 1);
            break;
        case USB_AUDIO_HID_SET_REPORT:
            LOG_MSGID_I(USBAUDIO_DRV, "Set Report", 0);
            usb_report_id   = (pcmd->wValue);
            usb_report_type = (pcmd->wValue) >> 8;
            usb_report_length = (pcmd->wLength);

            /* 1. USB interrupt for cmd 2. Wait for USB interrupt of data */
            gUsbDevice.ep0_rx_handler = (usb_ep0_rx_ptr)USB_Audio_HID_Ep0_ReadData;
            gUsbDevice.ep0_state = USB_EP0_RX;
            break;
        case USB_AUDIO_HID_GET_REPORT:
            // LOG_MSGID_I(USBAUDIO_DRV, "Get Report", 0);
            /* Get the target Report ID from low byte of wValue */
            usb_report_id   = (pcmd->wValue);
            usb_report_type = (pcmd->wValue) >> 8;
            usb_report_length = (pcmd->wLength);

            if (usb_report_id == 0x9A) {
                memset(usb_tx, 0, sizeof(usb_tx));
            }
            usb_tx[0] = usb_report_id ;
            /* HID handler dispatch the part of report */
            for (index = 0; index < USB_HID_HANDER_NUMS; index++) {
                /* According to report ID and send data to module */
                if (usb_hid_tx_handlers[index].report_id == usb_report_id) {
                    LOG_MSGID_I(USBAUDIO_DRV, "report_id[%x] is matched in HID tx handler table", 1, usb_report_id);
                    usb_hid_tx_handlers[index].handler_callback(&usb_tx[0], usb_hid_tx_handlers[index].report_length);
                    break;
                }
            }

            /* No HID report callback found */
            if (index == USB_HID_HANDER_NUMS) {
                /* Depend on report ID to receive data from Race or CFU */
                usb_mux_port = USB_MUX_Port_Select(usb_report_id);

                if (usb_mux_port == USB_MUX_PORT_UNUSE) {
                    LOG_MSGID_E(USBAUDIO_DRV, "No report ID [%x] match in Race or CFU", 1, usb_report_id);
                    //break;
                } else {
                    g_mux_usb_hid_callback(usb_mux_port, HID_EVENT_READY_TO_WRITE, NULL);
                }
            }

            //USB_Generate_EP0Data(&gUsbDevice.ep0info, &gUsbDevice.cmd, &usb_tx, USB_AUDIO_HID_REPORT_64BYTES);
            USB_Generate_EP0Data(&gUsbDevice.ep0info, &gUsbDevice.cmd, &usb_tx, usb_report_length);
            break;
        default:
            bError = true;
            break;
    }
    /* Stall command if an error occured */
    USB_EP0_Command_Hdlr(bError);
}

static void USB_Audio_HID_Ep0_Command(Usb_Ep0_Status *pep0state, Usb_Command *pcmd)
{
    USB_HID_Ep0_Command(0, pep0state, pcmd);
}

/* When PC return ACK, handle HID intr pipe state machine */
void USB_Audio_HID_IntrIn_Hdr(void)
{
    if (gUsbDevice.nDevState != DEVSTATE_CONFIG) {
        /* USB reset */
        //USB_Dbg_Trace(USB_HID_INTERRUPT_RESET, 0, 0);
        g_UsbAudioHID.intr_state = AUDIO_HID_INTR_IDLE;
        return;
    }

    //g_mux_usb_hid_callback(0, HID_EVENT_READY_TO_WRITE, NULL);

    switch (g_UsbAudioHID.intr_state) {
        
        case AUDIO_HID_INTR_IDLE:
        default:
            break;
    }

};

void USB_Audio_HID_IntrIn_Reset(void)
{
    g_UsbAudioHID.intr_state = AUDIO_HID_INTR_IDLE;
};

/* Communication interface create function, prepare descriptor */
void USB_Audio_HID_Create(void *ifname)
{
    uint8_t ep_id;
    uint8_t if_id;

    /* Initilize HID TX/RX handler */
    // usb_hid_handler_init();

    /* Get resource number and register to gUsbDevice */
    g_UsbAudioHID.comm_if_info = USB_Get_Interface_Number(&if_id);
    g_UsbAudioHID.comm_ep_intr_info = USB_Get_Intr_Tx_Ep(&ep_id);

    /* Record interface name and interface descriptor length */
    g_UsbAudioHID.comm_if_info->interface_name_ptr = (char *)ifname;
    g_UsbAudioHID.comm_if_info->ifdscr_size = sizeof(audio_hid_if_dscr);
    g_UsbAudioHID.comm_ep_intr_info->epdscr_size = USB_EPDSC_LENGTH;

    /* Related endpoint info and class specific command handler */
    g_UsbAudioHID.comm_if_info->ep_info[0] = g_UsbAudioHID.comm_ep_intr_info;
    g_UsbAudioHID.comm_if_info->if_class_specific_hdlr = USB_Audio_HID_Ep0_Command;

    memcpy(&(g_UsbAudioHID.comm_if_info->ifdscr.classif), audio_hid_if_dscr, sizeof(audio_hid_if_dscr));
    memcpy(&(g_UsbAudioHID.comm_ep_intr_info->epdesc.stdep), audio_hid_ep_dscr, USB_EPDSC_LENGTH);

    /* Standard interface descriptor */
    ((Usb_Audio_Hid_If_Dscr *)g_UsbAudioHID.comm_if_info->ifdscr.classif)->bInterfaceNumber = if_id;
    ((Usb_Audio_Hid_If_Dscr *)g_UsbAudioHID.comm_if_info->ifdscr.classif)->iInterface = USB_String_Get_Id_By_Usage(USB_STRING_USAGE_HID);

    hal_usb_register_driver_callback(HAL_USB_DRV_HDLR_EP_TX, ep_id, USB_Audio_HID_IntrIn_Hdr);
    g_UsbAudioHID.comm_ep_intr_info->ep_reset = USB_Audio_HID_IntrIn_Reset;

    /* Union Functional Descriptor */
    ((Usb_Audio_Hid_If_Dscr *)g_UsbAudioHID.comm_if_info->ifdscr.classif)->bMasterInterface = if_id;
    ((Usb_Audio_Hid_If_Dscr *)g_UsbAudioHID.comm_if_info->ifdscr.classif)->bSlaveInterface0 = USB_Audio_HID_Get_DataIf_Num();
    /* Call Management Descriptor */
    ((Usb_Audio_Hid_If_Dscr *)g_UsbAudioHID.comm_if_info->ifdscr.classif)->bDataInterface = USB_Audio_HID_Get_DataIf_Num();

    /* Endpoint descriptor */
    g_UsbAudioHID.comm_ep_intr_info->epdesc.stdep.bEndpointAddress = (USB_EP_DIR_IN | ep_id);    /*InPipe*/
    g_UsbAudioHID.comm_ep_intr_info->epdesc.stdep.bInterval = 3;
    g_UsbAudioHID.comm_ep_intr_info->ep_status.epin_status.byEP = ep_id;
    g_UsbAudioHID.comm_ep_intr_info->ep_status.epin_status.nBytesLeft = USB_EP_NODATA;

    g_UsbAudioHID.intr_pkt.bmRequestType = USB_CMD_CLASSIFIN;
    g_UsbAudioHID.intr_pkt.bRequest = USB_AUDIO_HID_NOTIF_SERIAL_STATE;
    g_UsbAudioHID.intr_pkt.wValue = 0;
    g_UsbAudioHID.intr_pkt.wIndex = if_id;
    g_UsbAudioHID.intr_pkt.wLength = 2;
};

void USB_Audio_HID_Reset(void)
{
    g_UsbAudioHID.intrpipe = &g_UsbAudioHID.comm_ep_intr_info->ep_status.epin_status;
};

void USB_Audio_HID_Enable(void)
{
    /* Non-DMA */
    hal_usb_stop_dma_channel(g_UsbAudioHID.intrpipe->byEP, HAL_USB_EP_DIRECTION_TX);
    hal_usb_enable_tx_endpoint(g_UsbAudioHID.intrpipe->byEP, HAL_USB_EP_TRANSFER_INTR, HAL_USB_EP_USE_NO_DMA, true);
};

void USB_Audio_HID_Speed_Reset(bool b_other_speed)
{
    g_UsbAudioHID.comm_ep_intr_info->epdesc.stdep.wMaxPacketSize[0] = HAL_USB_MAX_PACKET_SIZE_ENDPOINT_INTERRUPT_HIGH_SPEED & 0xff;
    g_UsbAudioHID.comm_ep_intr_info->epdesc.stdep.wMaxPacketSize[1] = 0x00;
};

#endif /* MTK_USB_AUDIO_HID_ENABLE */

#endif
