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

#ifndef USBAUDIO_DRV_H
#define USBAUDIO_DRV_H

/* C library */
#include <stdbool.h>
#include <stdint.h>

/* USB Middleware includes */
#include "bmd.h"
#include "usb.h"

/***********************************************
    USB Audio File Definition
************************************************/

/* Used to check the Audio TX/RX interval */
//#define USB_AUDIO_TX_TIME_MEASURE
//#define USB_AUDIO_RX_TIME_MEASURE

/* USB HID Keyboard Enable*/
//#define USB_HID_KEYBOARD_ENABLE

/* USB HID GYROMETER Sensor Enable*/
//#define CUSTOM_GYROMETER

/* USB Audio volume dB convertor */
#ifdef AIR_BLE_ULTRA_LOW_LATENCY_ENABLE
#define USB_AUDIO_SPEAKER_SAMPLESIZE_24BIT
#endif


#define USB_AUDIO_UNUSED_ID         0xFF

/***********************************************
    USB Audio Class Specific Command Definition
************************************************/
/* USB 2.0 : Ch5.6.4 Isochronous Transfer Bus Access Constraints, unit:us */
#define USB_HS_ISO_TRANSFER_DRATION  1000
#define USB_HS_ISO_TRANSFER_DRIFT     125
#define USB_HS_ISO_TRANSFER_UPPER    (USB_HS_ISO_TRANSFER_DRATION + USB_HS_ISO_TRANSFER_DRIFT)
#define USB_HS_ISO_TRANSFER_LOWER    (USB_HS_ISO_TRANSFER_DRATION - USB_HS_ISO_TRANSFER_DRIFT)

/* Version UAC 1.0 class-specific request codes: A.9 */
#define USB_AUDIO_1_REQ_UNDEFINED    0x00
#define USB_AUDIO_1_REQ_SET_CUR      0x01
#define USB_AUDIO_1_REQ_SET_MIN      0x02
#define USB_AUDIO_1_REQ_SET_MAX      0x03
#define USB_AUDIO_1_REQ_SET_RES      0x04
#define USB_AUDIO_1_REQ_SET_MEM      0x05
#define USB_AUDIO_1_REQ_GET_CUR      0x81
#define USB_AUDIO_1_REQ_GET_MIN      0x82
#define USB_AUDIO_1_REQ_GET_MAX      0x83
#define USB_AUDIO_1_REQ_GET_RES      0x84
#define USB_AUDIO_1_REQ_GET_MEM      0X85
#define USB_AUDIO_1_REQ_GET_STAT     0xFF

#define USB_AUDIO_AC_SUBTYPE_UNDEFINED          0x00
#define USB_AUDIO_AC_SUBTYPE_HEADER             0x01
#define USB_AUDIO_AC_SUBTYPE_INPUT_TERMINAL     0x02
#define USB_AUDIO_AC_SUBTYPE_OUTPUT_TERMINAL    0x03
#define USB_AUDIO_AC_SUBTYPE_MIXER_UNIT         0x04
#define USB_AUDIO_AC_SUBTYPE_SELECTOR_UNIT      0x05
#define USB_AUDIO_AC_SUBTYPE_FEATURE_UNIT       0x06
#define USB_AUDIO_AC_SUBTYPE_PROCESSING_UNIT    0x07
#define USB_AUDIO_AC_SUBTYPE_EXTENSION_UNIT     0x08

#define USB_AUDIO_AS_SUBTYPE_UNDEFINED          0x00
#define USB_AUDIO_AS_SUBTYPE_GENERAL            0x01
#define USB_AUDIO_AS_SUBTYPE_FORMAT_TYPE        0x02
#define USB_AUDIO_AS_SUBTYPE_FORMAT_SPECIFIC    0x03

typedef struct __attribute__((__packed__)) {
    uint8_t data[3];
} usb_audio_frequency_t;

/* Interface Descriptor */
typedef struct __attribute__((packed)) {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bInterfaceNumber;
    uint8_t bAlternateSetting;
    uint8_t bNumEndpoints;
    uint8_t bInterfaceClass;
    uint8_t bInterfaceSubClass;
    uint8_t bInterfaceProtocol;
    uint8_t iInterface;
} usb_audio_dscr_interface_t;

/* Class-Specific AC Interface Header Descriptor */
#define USB_AUDIO_DSCR_MAX_INTERFACE_NR 5
typedef struct __attribute__((packed)) {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint16_t bcdADC;
    uint16_t wTotalLength;
    uint8_t bInCollection;
    uint8_t baInterfaceNr[USB_AUDIO_DSCR_MAX_INTERFACE_NR];
} usb_audio_dscr_control_header_t;

/* Input Terminal Descriptor */
typedef struct __attribute__((packed)) {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bTerminalID;
    uint16_t wTerminalType;
    uint8_t bAssocTerminal;
    uint8_t bNrChannels;
    uint16_t wChannelConfig;
    uint8_t iChannelNames;
    uint8_t iTerminal;
} usb_audio_dscr_it_t;

/* Input Terminal Descriptor */
typedef struct __attribute__((packed)) {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bTerminalID;
    uint16_t wTerminalType;
    uint8_t bAssocTerminal;
    uint8_t bSourceID;
    uint8_t iTerminal;
} usb_audio_dscr_ot_t;

/* Mixer Unit Descriptor */
#define USB_DSCR_MIXER_MAX_IN_PINS 5
typedef struct __attribute__((packed)) {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bUnitID;
    uint8_t bNrInPins;
    uint8_t baSourceID[USB_DSCR_MIXER_MAX_IN_PINS];
} usb_audio_dscr_mixer_t;


/* Selector Unit Descriptor (for 2 unit select) */
#define USB_DSCR_SELECTOR_MAX_IN_PINS 5
typedef struct __attribute__((packed)) {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bUnitID;
    uint8_t bNrInPins;
    uint8_t baSourceID[USB_DSCR_SELECTOR_MAX_IN_PINS];
    uint8_t iSelector;
} usb_audio_dscr_selector_t;

/* Feature Unit Descriptor */
#define USB_DSCR_FEATURE_MAX_CONTROLS 5
typedef struct __attribute__((packed)) {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bUnitID;
    uint8_t bSourceID;
    uint8_t bControlSize;
    uint8_t bmaControls[USB_DSCR_FEATURE_MAX_CONTROLS];
    uint8_t iFeature;
    uint8_t control_nr;   /* custom field, record num of bmaControls */
} usb_audio_dscr_feature_t;

/* Class-Specific AS Interface Descriptor */
typedef struct __attribute__((__packed__)) {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bTerminalLink;
    uint8_t bDelay;
    uint16_t wFormatTag;
} usb_audio_dscr_as_general_t;

/* Class-Specific AS Format Type Descriptor */
#define USB_AUDIO_DSCR_MAX_FREQ_NUM 5
typedef struct __attribute__((__packed__)) {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubtype;
    uint8_t bFormatType;
    uint8_t bNrChannels;
    uint8_t bSubFrameSize;
    uint8_t bBitResolution;
    uint8_t bSamFreqType;
    usb_audio_frequency_t tSamFreq[USB_AUDIO_DSCR_MAX_FREQ_NUM];
} usb_audio_dscr_as_format_type_t;

/**
 * USB audio descriptor serialize functions
 */
typedef int16_t (*usb_audio_serial_func_t)(void *, void *, uint16_t);

typedef struct {
    void *dscr;
    usb_audio_serial_func_t serial_func;
} usb_audio_dscr_hdlr_t;

/* usb audio customer definition */
#ifdef USB_AUDIO_SPEAKER_SAMPLESIZE_24BIT
#define USB_AUDIO_RX_BUFFER_SIZE     1728 // (576x3, 288x6)
#else
#define USB_AUDIO_RX_BUFFER_SIZE     1920
#endif
#define USB_AUDIO_TX_BUFFER_SIZE     1920
#define USB_AUDIO_INTERFACE_PROTOCOL 0x00
#define USB_AUDIO_48K_DATA_SIZE       192

/* usb audio ep maxium packet size */
#ifdef USB_AUDIO_SPEAKER_SAMPLESIZE_24BIT
/* 96(K Hz, sample rate) x 2(channel) x 3byte (sample size) */
#define USB_AUDIO_RX_EP_MAX_SIZE      576
#else
#define USB_AUDIO_RX_EP_MAX_SIZE      192
#endif

/* 48(K Hz, sample rate) x 1(channel) x 2byte (sample size) */
#define USB_AUDIO_TX_EP_MAX_SIZE       96


typedef enum {
    USB_AUDIO_VERSION_None = 0,
    USB_AUDIO_VERSION_V1 = 1,
    USB_AUDIO_VERSION_V2 = 2,
    USB_AUDIO_VERSION_V3 = 3
} usb_audio_version_t;


typedef enum {
    USB_AUDIO_MUTE_OFF = 0,
    USB_AUDIO_MUTE_ON  = 1
} usb_audio_mute_t;

typedef void (*AUDIO_RX_FUNC)(void);
typedef void (*AUDIO_TX_FUNC)(void);
typedef void (*AUDIO_SETINTERFACE_FUNC)(uint8_t bInterfaceNumber, uint8_t bAlternateSetting);
typedef void (*AUDIO_SETSAMPLINGRATE_FUNC)(uint8_t ep_number, uint32_t sampling_rate);
typedef void (*AUDIO_UNPLUG_FUNC)(void);
typedef void (*AUDIO_VOLUMECHANGE_FUNC)(uint8_t ep_number, uint8_t channel, uint32_t volume, int32_t dB);
typedef void (*AUDIO_MUTE_FUNC)(uint8_t ep_number, usb_audio_mute_t mute);

/* Audio device structure */
typedef struct {
    bool                spk_feature_en;
    bool                mic_feature_en;
    /* Speaker device settings */
    uint16_t            spk_terminal_type;
    uint16_t            spk_cur;
    uint16_t            spk_min;
    uint16_t            spk_max;
    uint16_t            spk_res;
    /* Microphone device settings */
    uint16_t            mic_terminal_type;
    uint16_t            mic_cur;
    uint16_t            mic_min;
    uint16_t            mic_max;
    uint16_t            mic_res;
    uint8_t             control_interface_id;
    uint8_t             stream_interface_id;
    uint8_t             stream_ep_out_id;
    Usb_EpBOut_Status   *rxpipe; /* bulk out EP, Data interface */
    Usb_Ep_Info         *stream_ep_out_info;
    Usb_Interface_Info  *control_if_info;
    Usb_Interface_Info  *stream_if_info;
    Usb_IAD_Dscr        *iad_desc;
    /* buffer for read DMA operation*/
    uint8_t             *rx_dma_buffer;
    uint32_t            rx_dma_buffer_len;
    volatile uint32_t   rx_dma_buffer_is_full;
    volatile uint32_t   rx_dma_buffer_read;
    volatile uint32_t   rx_dma_buffer_write;
    volatile uint32_t   msg_notify;
    uint8_t             stream_microphone_interface_id;
    uint8_t             stream_ep_in_id;
    Usb_EpBOut_Status   *txpipe; /* bulk in EP, Data interface */
    Usb_Ep_Info         *stream_ep_in_info;
    Usb_Interface_Info  *stream_microphone_if_info;
    /* buffer for read DMA operation*/
    uint8_t             *tx_dma_buffer;
    uint32_t            tx_dma_buffer_len;
    volatile uint32_t   tx_dma_buffer_is_full;
    volatile uint32_t   tx_dma_buffer_read;
    volatile uint32_t   tx_dma_buffer_write;
} UsbAudio_Struct;

typedef struct {
    usb_audio_version_t         audio_version;
    bool                        initialized;
    AUDIO_RX_FUNC               rx_cb;
    AUDIO_TX_FUNC               tx_cb;
    AUDIO_SETINTERFACE_FUNC     setinterface_cb;
    AUDIO_SETSAMPLINGRATE_FUNC  setsamplingrate_cb;
    AUDIO_UNPLUG_FUNC           unplug_cb;
    AUDIO_VOLUMECHANGE_FUNC     volumechange_cb;
    AUDIO_MUTE_FUNC             mute_cb;
    AUDIO_SETINTERFACE_FUNC     setinterface_cb_mic;
    AUDIO_SETSAMPLINGRATE_FUNC  setsamplingrate_cb_mic;
    AUDIO_UNPLUG_FUNC           unplug_cb_mic;
    AUDIO_VOLUMECHANGE_FUNC     volumechange_cb_mic;
    AUDIO_MUTE_FUNC             mute_cb_mic;
} UsbAudioStruct;

/***********************************************
    function and global variable
************************************************/
extern UsbAudioStruct USB_Audio[2];
extern UsbAudio_Struct g_UsbAudio[2];

/* USB Audio 1 : Speaker */
extern bool g_usb_audio_spk1_enable;
extern bool g_usb_audio_spk1_status;
/* USB Audio 2 : Speaker */
extern bool g_usb_audio_spk2_feature;
extern bool g_usb_audio_spk2_enable;
extern bool g_usb_audio_spk2_status;

extern int32_t g_usb_audio_spk1_volume_dB[2];
extern int32_t g_usb_audio_spk2_volume_dB[2];
#ifdef MTK_USB_AUDIO_MICROPHONE
extern int32_t g_usb_audio_mic_volume_dB[2];
#endif

extern void USB_Audio_Setting_By_NVKey();

void USB_Audio1_ControlIf_Create(void *ifname);
void USB_Audio2_ControlIf_Create(void *ifname);
void USB_Audio1_ControlIf_Reset(void);
void USB_Audio2_ControlIf_Reset(void);

void USB_Audio1_ControlIf_Enable(void);
void USB_Audio2_ControlIf_Enable(void);

void USB_Audio1_ControlIf_Speed_Reset(bool b_other_speed);
void USB_Audio2_ControlIf_Speed_Reset(bool b_other_speed);
void USB_Audio1_StreamIf_Create(void *ifname);
void USB_Audio2_StreamIf_Create(void *ifname);

void USB_Audio1_StreamIf_Reset(void);
void USB_Audio2_StreamIf_Reset(void);

void USB_Audio1_StreamIf_Enable(void);
void USB_Audio2_StreamIf_Enable(void);
void USB_Audio1_StreamIf_Speed_Reset(bool b_other_speed);
void USB_Audio2_StreamIf_Speed_Reset(bool b_other_speed);

#ifdef MTK_USB_AUDIO_MICROPHONE
void USB_Audio_StreamIf_Microphone_Create(void *ifname);
void USB_Audio_StreamIf_Microphone_Reset(void);
void USB_Audio_StreamIf_Microphone_Enable(void);
void USB_Audio_StreamIf_Microphone_Speed_Reset(bool b_other_speed);
#endif
void USB_Audio1_Ep0_Command(Usb_Ep0_Status *pep0state, Usb_Command *pcmd);
void USB_Audio2_Ep0_Command(Usb_Ep0_Status *pep0state, Usb_Command *pcmd);

void USB_Audio1_IsoOut_Hdr(void);
void USB_Audio2_IsoOut_Hdr(void);

void USB_Audio1_IsoOut_Reset(void);
void USB_Audio2_IsoOut_Reset(void);
#ifdef MTK_USB_AUDIO_MICROPHONE
void USB_Audio_IsoIn_Hdr(void);
void USB_Audio_IsoIn_Reset(void);
#endif
void USB_Init_Audio_Status(void);
void USB_Release_Audio_Status(void);
void USB_Audio_Get_Chat_Info(uint8_t *get_audio1_rx_if_id, uint8_t *get_audio1_rx_ep_id, uint8_t *get_audio1_tx_if_id, uint8_t *get_audio1_tx_ep_id);
void USB_Audio_Get_Game_Info(uint8_t *get_audio2_rx_if_id, uint8_t *get_audio2_rx_ep_id);
void USB_Audio_Register_Rx_Callback(uint32_t port, AUDIO_RX_FUNC rx_cb);
void USB_Audio_Register_Tx_Callback(uint32_t port, AUDIO_TX_FUNC tx_cb);
void USB_Audio_Register_SetInterface_Callback(uint32_t port, AUDIO_SETINTERFACE_FUNC setinterface_cb);
void USB_Audio_Register_SetSamplingRate_Callback(uint32_t port, AUDIO_SETSAMPLINGRATE_FUNC setsamplingrate_cb);
void USB_Audio_Register_Unplug_Callback(uint32_t port, AUDIO_UNPLUG_FUNC unplug_cb);
void USB_Audio_Register_VolumeChange_Callback(uint32_t port, AUDIO_VOLUMECHANGE_FUNC volumechange_cb);
void USB_Audio_Register_Mute_Callback(uint32_t port, AUDIO_MUTE_FUNC mute_cb);
#ifdef MTK_USB_AUDIO_MICROPHONE
void USB_Audio_Register_Mic_SetInterface_Callback(uint32_t port, AUDIO_SETINTERFACE_FUNC setinterface_cb);
void USB_Audio_Register_Mic_SetSamplingRate_Callback(uint32_t port, AUDIO_SETSAMPLINGRATE_FUNC setsamplingrate_cb);
void USB_Audio_Register_Mic_Unplug_Callback(uint32_t port, AUDIO_UNPLUG_FUNC unplug_cb);
void USB_Audio_Register_Mic_VolumeChange_Callback(uint32_t port, AUDIO_VOLUMECHANGE_FUNC volumechange_cb);
void USB_Audio_Register_Mic_Mute_Callback(uint32_t port, AUDIO_MUTE_FUNC mute_cb);
#endif
uint32_t USB_Audio_Get_CurSamplingRate(uint32_t port);
#ifdef MTK_USB_AUDIO_MICROPHONE
uint32_t USB_Audio_Get_Mic_CurSamplingRate(uint32_t port);
#endif
void *USB_Audio_Rx_Buffer_Get_Read_Address(uint32_t port);
uint32_t USB_Audio_Rx_Buffer_Get_Bytes(uint32_t port);
void USB_Audio_Rx_Buffer_Drop_Bytes(uint32_t port, uint32_t bytes);
uint32_t USB_Audio_Get_Len_Received_Data(uint32_t port);
uint32_t USB_Audio_Read_Data(uint32_t port, void *dst, uint32_t len);

#ifdef MTK_USB_AUDIO_HID_ENABLE
/* The length of HID descriptort report length */
#define USB_AUDIO_HID_DESCRIPTOR_LENGTH            9

/* Align naming to race and mux  */
#define USB_MUX_PORT1                            0x00
#define USB_MUX_PORT2                            0x01
#define USB_MUX_PORT_UNUSE                       0xFF

#define USB_AUDIO_HID_GET_REPORT                 0x01
#define USB_AUDIO_HID_SET_REPORT                 0x09

#define USB_AUDIO_HID_REPORT_64BYTES             0x40  /* 64-byte for definition and 4-byte algnment*/
#define USB_AUDIO_HID_REPORT_62BYTES             0x3E  /* In/Out HID Report is 62-byte*/
#define USB_AUDIO_HID_REPORT_DATA_LENGTH         0x3B  /* HID Report size of  tool is 62-byte, Available data is 62-3 = 59 (0x3B)(Report ID, Length, Target Device)*/

/* In/Out Report */
#define USB_AUDIO_HID_OUT_REPORT_ID              0x06
#define USB_AUDIO_HID_IN_REPORT_ID               0x07

#define USB_AUDIO_HID_REPORT_ID                  0x00
#define USB_AUDIO_HID_REPORT_LENGTH              0x01
#define USB_AUDIO_HID_REPORT_TARGET              0x02
#define USB_AUDIO_HID_REPORT_DATA_BEGIN          0x03
#define USB_AUDIO_HID_REPORT_DATA_NULL           0x00

#define TARGET_LOCAL_DEVICE                      0x00
#define TARGET_REMOTE_DEVICE                     0x80
#define TARGET_REMOTE_MULTIDEVICE1               0x81
#define TARGET_REMOTE_MULTIDEVICE2               0x82
#define TARGET_REMOTE_MULTIDEVICE3               0x83
#define TARGET_REMOTE_MULTIDEVICE4               0x84
#define TARGET_EXTERNAL_DEVICE                   0x90
#define TARGET_INVALID_DEVICE                    0xFF

typedef enum {
    HID_EVENT_READY_TO_READ = 0,        /**< Ready to read event. */
    HID_EVENT_READY_TO_WRITE,           /**< Ready to write event. */
    HID_EVENT_USB_CONNECTION,           /**< USB connection event. */
    HID_EVENT_USB_DISCONNECTION,        /**< USB disconnection event. */
} hid_callback_event_t;

typedef void (*mux_usb_hid_callback)(uint8_t usb_port, hid_callback_event_t event, void *parameter);


#define USB_AUDIO_HID_IF_LENGTH                     0x09   /* standard class */

/* Class-Specfic Codes ,EP0*/
#define USB_AUDIO_HID_GET_REPORT                    0x01
#define USB_AUDIO_HID_GET_IDLE                      0x02
#define USB_AUDIO_HID_SET_REPORT                    0x09
#define USB_AUDIO_HID_SET_IDLE                      0x0A
#define USB_AUDIO_HID_SET_PROTOCOL                  0x0B

#define USB_AUDIO_HID_SEND_ENCAPSULATED_COMMAND     0x00
#define USB_AUDIO_HID_GET_ENCAPSULATED_RESPONSE     0x01
#define USB_AUDIO_HID_SET_COMM_FEATURE              0x02
#define USB_AUDIO_HID_GET_COMM_FEATURE              0x03
#define USB_AUDIO_HID_CLEAR_COMM_FEATURE            0x04
#define USB_AUDIO_HID_SET_LINE_CODING               0x20
#define USB_AUDIO_HID_GET_LINE_CODING               0x21
#define USB_AUDIO_HID_SET_CONTROL_LINE_STATE        0x22
#define USB_AUDIO_HID_SEND_BREAK                    0x23

/*Notifications, interrupt pipe*/
#define USB_AUDIO_HID_NOTIF_NETWORK_CONNECTION      0x00
#define USB_AUDIO_HID_NOTIF_RESPONSE_AVAILABLE      0x01
#define USB_AUDIO_HID_NOTIF_SERIAL_STATE            0x20

/* Interface Descriptor */
#define USB_AUDIO_HID_ALTERNATE_SETTING_NONE        0x00
// bInterfaceClass
#define USB_AUDIO_HID_INTERFACE_CLASS_HID           0x03
// bInterfaceSubClass
#define USB_AUDIO_HID_INTERFACE_SUBCLASS_NONE       0x00
#define USB_AUDIO_HID_INTERFACE_SUBCLASS_BOOT       0x01
// bInterfaceProtocol
#define USB_AUDIO_HID_INTERFACE_PROTOCOL_NONE       0x00
#define USB_AUDIO_HID_INTERFACE_PROTOCOL_KEYBOARD   0x01
#define USB_AUDIO_HID_INTERFACE_PROTOCOL_MOUSE      0x02
// iInterface
#define USB_AUDIO_HID_INTERFACE_INDEX_NONE          0x00
#define USB_AUDIO_HID_INTERFACE_INDEX_ONE           0x01

/* HID Report of Audio Setting*/
#define USB_HID_AUDIO_REPORT_ID                     0x0C
/* HID Report of Gyroscope Data*/
#define USB_HID_Gyroscope_DATA_REPORT_ID            0x03


#define USB_HID_NONE                                0x00
#define USB_HID_VOLUME_UP                           0x01
#define USB_HID_VOLUME_DOWN                         0x02
#define USB_HID_VOLUME_MUTE                         0x04
#define USB_HID_PLAY_PAUSE                          0x08
#define USB_HID_SCAN_NEXT_TRACK                     0x10
#define USB_HID_SCAN_PREVIOUS_TRACK                 0x20

#define USB_AUDIO_HID_TX_EP_NUMBER             1  /*interrupt pipe*/

/* USB HID */
#define USB_HID_INPUT_REPORT_TYPE                0x01
#define USB_HID_OUTPUT_REPORT_TYPE               0x02
#define USB_HID_FEATURE_REPORT_TYPE              0x03

/* USB HID Report Length*/
#define USB_HID_DFU_REPORT_LEN                     33

#ifdef MTK_USB_AUDIO_V1_ENABLE
#define USB_HID_AUDIO_REPORT_LEN                   37
#else
#define USB_HID_AUDIO_REPORT_LEN                    0
#endif

#ifdef AIR_MS_TEAMS_ENABLE
#define USB_HID_TEAMS_REPORT_LEN                  120
#else
#define USB_HID_TEAMS_REPORT_LEN                    0
#endif

#ifdef AIR_CFU_ENABLE
#define USB_HID_CFU_REPORT_LEN                     76
#else
#define USB_HID_CFU_REPORT_LEN                      0
#endif

#ifdef USB_HID_KEYBOARD_ENABLE
#define USB_HID_KEYBOARD_REPORT_LEN                65
#else
#define USB_HID_KEYBOARD_REPORT_LEN                 0
#endif

#ifdef CUSTOM_GYROMETER
#define USB_HID_SENSOR_REPORT_LEN                 172
#else
#define USB_HID_SENSOR_REPORT_LEN                   0
#endif


#define USB_HID_REPORT_LEN  (USB_HID_DFU_REPORT_LEN + \
                             USB_HID_AUDIO_REPORT_LEN + \
                             USB_HID_TEAMS_REPORT_LEN + \
                             USB_HID_CFU_REPORT_LEN + \
                             USB_HID_KEYBOARD_REPORT_LEN + \
                             USB_HID_SENSOR_REPORT_LEN)


#define USB_AUDIO_TEAMS_FEATURE_REPORT_ID        0x9A
#define USB_AUDIO_TEAMS_IN_REPORT_ID             0x9B
#define USB_HID_CALL_CONTROL_REPORT_ID           0x05

/* CFU report ID*/
#define USB_CFU_DUMMY_IN_REPORT_ID             0x20
#define USB_CFU_CONTENT_IN_REPORT_ID           0x22
#define USB_CFU_OFFER_IN_REPORT_ID             0x25
#define USB_CFU_CONTENT_OUT_REPORT_ID          0x20
#define USB_CFU_OFFER_OUT_REPORT_ID            0x25
#define USB_CFU_VERSION_FEATURE_REPORT_ID      0x20

/* CFU report length, +1 means report ID */
#define USB_CFU_DUMMY_IN_REPORT_LENGTH          1+1
#define USB_CFU_CONTENT_IN_REPORT_LENGTH       1+16
#define USB_CFU_OFFER_IN_REPORT_LENGTH         1+16
#define USB_CFU_CONTENT_OUT_REPORT_LENGTH      1+60
#define USB_CFU_OFFER_OUT_REPORT_LENGTH        1+16
#define USB_CFU_VERSION_FEATURE_REPORT_LENGTH  1+60

/* TEAMS - in report (0x05) for call control */
#define USB_HID_HOOK_SWITCH           0x01
#define USB_HID_LINE_BUSY_TONE        0x02
#define USB_HID_LINE                  0x04
#define USB_HID_PHONE_MUTE            0x08
#define USB_HID_FLASH                 0x10
#define USB_HID_REDIAL                0x20
#define USB_HID_SPEED_DIAL            0x40

#define USB_AUDIO_TEAMS_FEATURE_REPORT_LENGTH      32  /* Include Report ID*/
#define USB_AUDIO_TEAMS_IN_REPORT_LENGTH            2


/* HID interrupt packet structure */
typedef struct {
    uint8_t     bmRequestType;
    uint8_t     bRequest;
    uint16_t    wValue;
    uint16_t    wIndex;
    uint16_t    wLength;
    uint16_t    Data;
} Usb_Audio_Hid_Intr_Pkt;

/*  HID line coding element */
typedef struct {
    uint32_t  dwDTERate;
    uint8_t   bCharFormat;
    uint8_t   bParityType;
    uint8_t   bDataBits;
} UsbAudio_HID_Line_Coding;

typedef enum {
    AUDIO_HID_INTR_IDLE = 0,
    AUDIO_HID_INTR_SEND_ONE_PKT,
    AUDIO_HID_INTR_SEND_LAST_PKT
} USB_AUDIO_HID_INTR_PIPE_STATE;


/* HID device structure */
typedef struct {
    uint8_t        data_interface_id;
    bool           send_Txilm;
    bool           send_Rxilm;
    bool           send_UARTilm;
    bool           config_send_Txilm;
    bool           ring_buffer_timer_counting;    /* flag to check ring buffer timer running or not */
    bool           threshold_enable;              /* enable ring buffer threshold and time out mechanism */
    bool           setup_dma;                     /* a flag to record DMA is triggered or not */
    bool           is_intr_pending_pkt;           /* interrupt pipe has pending packet */
    bool           dma_txcb_just_done;            /*DMA TX Callback Function just done. It's clear by putbyte funtion*/
    bool           break_detect;
    bool           put_start;
    uint16_t       dmaf_setmember;                /* Who set dma_false after putbyte and it does not finsih*/
    uint16_t       line_state;
    Usb_Audio_Hid_Intr_Pkt        intr_pkt;
    uint32_t       handle;                        /*GPT handle*/
    uint32_t       dma_handle;                    /*for DMA timeout handle*/
    uint32_t       ring_buffer_handle;            /* ring buffer's GPT handle */
    uint32_t       break_number;
    UsbAudio_HID_Line_Coding line_coding;         /* HID-TX From usbam.h */

    Usb_EpBIn_Status     *txpipe;                 /* bulk in EP, Data interface */
    Usb_EpBOut_Status    *rxpipe;                 /* bulk out EP, Data interface */
    Usb_EpBIn_Status     *intrpipe;               /* interrupt EP, Communication Interface */

    USB_AUDIO_HID_INTR_PIPE_STATE    intr_state;  /* interrupt pipe state machine */
    Usb_Ep_Info                *comm_ep_intr_info;
    Usb_Ep_Info                *data_ep_in_info;
    Usb_Ep_Info                *data_ep_out_info;
    Usb_Interface_Info         *comm_if_info;
    Usb_Interface_Info         *data_if_info;

    Usb_IAD_Dscr               *iad_desc;

#ifdef  __USB_HIGH_SPEED_COM_PORT_ENABLE__
    uint32_t       data_addr;
    uint32_t       rx_buf_data_len[USB_HID_RX_BUFFER_NUM];
    uint16_t       tx_count;          /* To store the TX count, debug only*/
    bool           b_data_left;       // left data due to 512N
    bool           b_is_high_speed_enable;
    bool           b_rc_index_move;
    uint16_t       b_rc_get_count;
    uint8_t        rx_buf_rc_index;  /* read clear pointer */
    uint8_t        rx_buf_r_index;   /* read pointer */
    uint8_t        rx_buf_w_index;   /* write pointer */
    uint8_t        rx_buf_count;     /* the buffer count with data */
#ifdef DRV_USB_DMA_QUEUE_SUPPORT
    bool        ppp_txdmaq_first;
    uint32_t    ppp_rdmaq_idx;
    uint32_t    ppp_tdmaq_idx;
#endif

//#ifdef __USB_TETHERING__
    uint8_t         *rx_buf_addr[RNDIS_RX_BUFFER_NUM + 1];    //9 : 0 ~ 8
    uint32_t        isd_rx_buf_data_len[RNDIS_RX_BUFFER_NUM + 1];
    uint8_t         *rx_flc_buf_addr[RNDIS_RX_FLC_BUFFER_NUM + 1];     //3 //3:  0~2
    uint32_t        rx_flc_buf_data_len[RNDIS_RX_FLC_BUFFER_NUM + 1];
    uint16_t        tx_flc_count;          /* To store the TX count, debug only*/
    uint8_t         *tx_flc_data_addr;
    bool            rx_full_data;
    bool            halt_flag;
    bool            ctrl_data_done;
    bool            send_ctrl_Txilm;  // Ctrl pipe --> TX message
    bool            send_ctrl_Rxilm;  // ctrl pipe --> RX message

//    bool        dma_rx_running;  //use  FIFO Read first
//    bool        dma_tx_running; //the dame as setup_dma

    uint8_t        rx_full_buff_index;      // receive Data
    uint8_t        rx_empty_buff_index;     // empty buffer
    uint8_t        rx_flc_full_buff_index;  // receive Data
    uint8_t        rx_flc_empty_buff_index; // empty buffer
//    bool         b_flc_rc_index_move;
    uint8_t        rx_flc_buf_rc_index;   /* read clear pointer */
    uint8_t        rx_flc_buf_r_index;    /* read pointer */
    uint8_t        rx_flc_buf_w_index;    /* write pointer: data write (bulk out handler) */
    uint8_t        rx_flc_buf_count;      /* the buffer count with data */
    uint8_t        rx_flc_temp_length;    /* for first time PC send data just before buffer push  (initial message) */
    bool           rx_flc_temp;           /* for first time PC send data just before buffer push (initial message) */
    uint8_t        *rx_flc_temp_buf_addr;
#endif

    bool        b_is_tx_direct_path_enable;
} UsbAudioHid_Struct;

typedef struct {
    uint8_t    bLength;
    uint8_t    bDescriptorType;
    uint8_t    bInterfaceNumber;
    uint8_t    bAlternateSetting;
    uint8_t    bNumEndpoints;
    uint8_t    bInterfaceClass;
    uint8_t    bInterfaceSubClass;
    uint8_t    bInterfaceProtocol;
    uint8_t    iInterface;

    uint8_t    HFbFunctionLength;        /*Header Functional Descriptor*/
    uint8_t    HFbDescriptorType;
    uint8_t    HFbDescriptorSubtype;
    uint16_t   bcdCDC;

    uint8_t    HIDFbFunctionLength;      /*Abstract Control Management Functional Descriptor*/
    uint8_t    HIDFbDescriptorType;
    uint8_t    HIDFbDescriptorSubtype;
    uint8_t    HIDFbmCapabilities;

    uint8_t    UFbFunctionLength;        /*Union Functional Descriptor*/
    uint8_t    UFbDescriptorType;
    uint8_t    UFbDescriptorSubtype;
    uint8_t    bMasterInterface;
    uint8_t    bSlaveInterface0;

    uint8_t    CMFbFunctionLength;        /*Call Management Descriptor*/
    uint8_t    CMFbDescriptorType;
    uint8_t    CMFbDescriptorSubtype;
    uint8_t    CMFbmCapabilities;
    uint8_t    bDataInterface;
} Usb_Audio_Hid_If_Dscr;

typedef enum {
    USB_Audio_HID_STATUS_OK                    = 0,     /* Operation completed successfully*/
    USB_Audio_HID_TX_LEN_IS_ZERO               = 1,     /* TX data length is zero */
    USB_Audio_HID_TX_LEN_TOO_LARGE             = 2,     /* TX data length is too large */
    USB_Audio_HID_CHARGER_DETECT_ERROR         = 3,     /* Charger type is wrong */
    USB_Audio_HID_SEND_DATA_ERROR              = 4,     /* HID TX status is error */
    USB_Audio_HID_IS_NOT_READY                 = 5,     /* HID is not ready */
} USB_Audio_HID_t;

typedef enum {
    USB_HID_KEYBOARD_KEY_WIN_KEY_ONLY          = 0x00,  /* Press Win Key Only*/
    USB_HID_KEYBOARD_KEY_C                     = 0x06,  /* Press Win Key + C */
    USB_HID_KEYBOARD_KEY_D                     = 0x07,  /* Press Win Key + D */  /* Minimize All Windows */
    USB_HID_KEYBOARD_KEY_E                     = 0x08,  /* Press Win Key + E */  /* Open My Computer */
    USB_HID_KEYBOARD_KEY_L                     = 0x0F,  /* Press Win Key + L */  /* Lock Computer */
} USB_HID_WINDOWS_KEY_MODIFIER_FUNC_t;

void USB_Audio_HID_Ep0_ReadData(uint8_t *data);
void USB_Audio_HID_Ep0_DispatchData(uint8_t *data);

USB_Audio_HID_t USB_Audio_HID_TX(uint8_t *data, uint8_t data_len);
USB_Audio_HID_t USB_Audio_HID_VolumeUp(uint8_t step);
USB_Audio_HID_t USB_Audio_HID_VolumeDown(uint8_t step);
USB_Audio_HID_t USB_Audio_HID_VolumeMute();
USB_Audio_HID_t USB_Audio_HID_PlayPause();
USB_Audio_HID_t USB_Audio_HID_PlayPause_RejectCall();
USB_Audio_HID_t USB_Audio_HID_ScanNextTrack();
USB_Audio_HID_t USB_Audio_HID_ScanPreviousTrack();
USB_Audio_HID_t USB_Audio_HID_Win_Key_Func(USB_HID_WINDOWS_KEY_MODIFIER_FUNC_t key);
USB_Audio_HID_t USB_Audio_HID_Gyroscope_Send_Rotate_Angle(int16_t rx, int16_t ry, int16_t rz);
USB_Audio_HID_t USB_Audio_HID_Gyroscope_Send_Rotate_Speed(int16_t vx, int16_t  vy, int16_t  vz);
USB_Audio_HID_t USB_Audio_HID_Gyroscope_Send_Frame_Reset(void);

void USB_Audio_HID_Create(void *ifname);
void USB_Audio_HID_Reset(void);
void USB_Audio_HID_Enable(void);
void USB_Audio_HID_Speed_Reset(bool b_other_speed);

void USB_Audio_HID_IntrIn_Hdr(void);
void USB_Audio_HID_IntrIn_Reset(void);

/* HID data handler */
#define USB_HID_HANDLER_ERROR_OK 0
#define USB_HID_HANDLER_ERROR_FAIL -1
#define USB_HID_HANDLER_ERROR_INITED -2

#define USB_HID_HANDER_NUMS 8
#define USB_HID_INVALID_REPORT_ID 0xFF

typedef void (*usb_hid_handler_callback)(uint8_t *data, uint32_t data_length);

typedef struct {
    uint8_t report_id;
    uint8_t report_length;
    usb_hid_handler_callback handler_callback;
} usb_hid_handler_t;

int32_t usb_hid_handler_rx_register(uint8_t report_id, uint8_t report_length, usb_hid_handler_callback *callback);
int32_t usb_hid_handler_tx_register(uint8_t report_id, uint8_t report_length, usb_hid_handler_callback *callback);
int32_t usb_hid_rx_handler_unregister(uint8_t report_id);
int32_t usb_hid_tx_handler_unregister(uint8_t report_id);


#endif /* MTK_USB_AUDIO_HID_ENABLE */

#endif /* USBAUDIO_DRV_H */
