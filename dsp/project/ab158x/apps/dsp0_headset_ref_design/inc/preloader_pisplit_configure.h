/*
 * Copyright (c) 2012-2013 by Tensilica Inc. ALL RIGHTS RESERVED.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef __PRELOADER_PILSI_CONFIGURE_H__
#define __PRELOADER_PILSI_CONFIGURE_H__

#include "xt_library_loader.h"


#ifdef __cplusplus
extern "C" {
#endif

#ifdef MTK_BT_A2DP_LC3_USE_PIC
extern xtlib_packaged_library pisplit_lc3_codec;
extern xtlib_packaged_library pisplit_lc3i_plc_std;
extern xtlib_packaged_library pisplit_lc3i_plc_adv;
extern xtlib_packaged_library pisplit_lc3i_fft7p5ms;
extern xtlib_packaged_library pisplit_lc3i_fft10ms;
extern xtlib_packaged_library pisplit_lc3i_codec;
#define LC3_LIB &pisplit_lc3_codec,
#define LC3I_PLC_STD_LIB &pisplit_lc3i_plc_std,
#define LC3I_PLC_ADV_LIB &pisplit_lc3i_plc_adv,
#define LC3I_FFT7_5MS_LIB &pisplit_lc3i_fft7p5ms,
#define LC3I_FFT10MS_LIB &pisplit_lc3i_fft10ms,
#define LC3I_LIB &pisplit_lc3i_codec,
#else
#define LC3_LIB
#define LC3I_PLC_STD_LIB
#define LC3I_PLC_ADV_LIB
#define LC3I_FFT7_5MS_LIB
#define LC3I_FFT10MS_LIB
#define LC3I_LIB
#endif

#ifdef MTK_BT_A2DP_AAC_USE_PIC
extern xtlib_packaged_library pisplit_AAC_dec_5x;
#define AAC_LIB &pisplit_AAC_dec_5x,
#else
#define AAC_LIB
#endif

#ifdef MTK_BT_PEQ_USE_PIC
extern xtlib_packaged_library pisplit_peq2;// user muts be extern your lib to here!!!
#define PEQ_LIB &pisplit_peq2,
#else
#define PEQ_LIB
#endif

#ifdef MTK_BT_A2DP_SBC_USE_PIC
#define SBC_LIB &pisplit_sbc_dec,
extern xtlib_packaged_library pisplit_sbc_dec;
#else
#define SBC_LIB
#endif

#ifdef MTK_BT_A2DP_MSBC_USE_PIC
extern xtlib_packaged_library pisplit_msbc_dec;// user muts be extern your lib to here!!!
extern xtlib_packaged_library pisplit_msbc_enc;// user muts be extern your lib to here!!!
#define MSBC_DEC_LIB &pisplit_msbc_dec,
#define MSBC_ENC_LIB &pisplit_msbc_enc,
#else
#define MSBC_DEC_LIB
#define MSBC_ENC_LIB
#endif

#ifdef MTK_BT_CLK_SKEW_USE_PIC
extern xtlib_packaged_library pisplit_skew_ctrl;// user muts be extern your lib to here!!!
#define CLK_SKEW_LIB &pisplit_skew_ctrl,
#else
#define CLK_SKEW_LIB
#endif

#ifdef MTK_PLC_USE_PIC
extern xtlib_packaged_library pisplit_plc_pitch;// user muts be extern your lib to here!!!
#define PLC_LIB &pisplit_plc_pitch,
#else
#define PLC_LIB
#endif

#ifdef MTK_BT_A2DP_CPD_USE_PIC
extern xtlib_packaged_library pisplit_cpd;// user muts be extern your lib to here!!!
#define CPD_LIB &pisplit_cpd,
#else
#define CPD_LIB
#endif

#ifdef MTK_BT_AGC_USE_PIC
extern xtlib_packaged_library pisplit_agc;// user muts be extern your lib to here!!!
#define AGC_LIB &pisplit_agc,
#else
#define AGC_LIB
#endif

#ifdef AIR_ADAPITVE_EQ_DETECTION_USE_PIC_ENABLE
extern xtlib_packaged_library pisplit_aeq;// user muts be extern your lib to here!!!
#define AEQ_LIB &pisplit_aeq,
#else
#define AEQ_LIB
#endif

#ifdef MTK_WWE_USE_PIC

#ifdef MTK_WWE_AMA_USE_PIC
extern xtlib_packaged_library pisplit_library_pryon1000;// user muts be extern your lib to here!!!
#define WWE_LIB_AMA &pisplit_library_pryon1000,
#else
#define WWE_LIB_AMA
#endif
#ifdef MTK_WWE_GSOUND_USE_PIC
extern xtlib_packaged_library pisplit_google_hotword_dsp_multi_bank;// user muts be extern your lib to here!!!
#define WWE_LIB_GSOUND &pisplit_google_hotword_dsp_multi_bank,
#else
#define WWE_LIB_GSOUND
#endif
#else
#define WWE_LIB_AMA
#define WWE_LIB_GSOUND
#endif

#ifdef AIR_VOICE_NR_USE_PIC_ENABLE

#if defined(AIR_BTA_IC_PREMIUM_G2)
#if defined(AIR_ECNR_1MIC_INEAR_ENABLE)
extern xtlib_packaged_library pisplit_ecnr_inear;// user muts be extern your lib to here!!!
#define ECNR_LIB &pisplit_ecnr_inear,
#elif defined(AIR_ECNR_2MIC_INEAR_ENABLE)
extern xtlib_packaged_library pisplit_ecnr_inear_v2;// user muts be extern your lib to here!!!
#define ECNR_LIB &pisplit_ecnr_inear_v2,
#elif defined(AIR_ECNR_1_OR_2_MIC_ENABLE)
extern xtlib_packaged_library pisplit_ecnr;// user muts be extern your lib to here!!!
#define ECNR_LIB &pisplit_ecnr,
#elif defined(AIR_3RD_PARTY_NR_ENABLE)
extern xtlib_packaged_library pisplit_ec_rxnr;// user muts be extern your lib to here!!!
#define ECNR_LIB &pisplit_ec_rxnr,
#endif
#else
#if (defined(AIR_ECNR_1_OR_2_MIC_ENABLE) || defined(AIR_ECNR_1MIC_INEAR_ENABLE) || defined(AIR_ECNR_2MIC_INEAR_ENABLE))
extern xtlib_packaged_library pisplit_ecnr_inear_v2_swb;// user muts be extern your lib to here!!!
#define ECNR_LIB &pisplit_ecnr_inear_v2_swb,
#elif defined(AIR_3RD_PARTY_NR_ENABLE)
extern xtlib_packaged_library pisplit_ec_rxnr_swb;// user muts be extern your lib to here!!!
#define ECNR_LIB &pisplit_ec_rxnr_swb,
#else
#define ECNR_LIB
#endif
#endif
#else
#define ECNR_LIB
#endif

#ifdef AIR_VOICE_NR_ENABLE
#ifdef AIR_ECNR_PREV_PART_USE_PIC_ENABLE
extern xtlib_packaged_library pisplit_ec120;// user muts be extern your lib to here!!!
#define EC120_LIB &pisplit_ec120,
#else
#define EC120_LIB
#endif
#else
#define EC120_LIB
#endif

#ifdef AIR_VOICE_NR_ENABLE
#ifdef AIR_ECNR_POST_PART_USE_PIC_ENABLE
     extern xtlib_packaged_library pisplit_post_ec;// user muts be extern your lib to here!!!
     #define POST_EC_LIB &pisplit_post_ec,
#else
     #define POST_EC_LIB
#endif
#else
     #define POST_EC_LIB
#endif

#ifdef AIR_VOICE_NR_USE_PIC_ENABLE
#ifdef AIR_3RD_PARTY_NR_ENABLE
extern xtlib_packaged_library pisplit_fir_eq;// user muts be extern your lib to here!!!
#define TX_EQ_LIB &pisplit_fir_eq,
#else
#define TX_EQ_LIB
#endif
#else
#define TX_EQ_LIB
#endif

#ifdef AIR_VOICE_NR_USE_PIC_ENABLE
#if defined(AIR_3RD_PARTY_NR_ENABLE)
#if defined(AIR_AI_NR_PREMIUM_ENABLE) || defined(AIR_AI_NR_PREMIUM_INEAR_ENABLE)
extern xtlib_packaged_library pisplit_igo_txnr;// user muts be extern your lib to here!!!
#define IGO_TXNR_LIB &pisplit_igo_txnr,
#else
#define IGO_TXNR_LIB
#endif
#else
#define IGO_TXNR_LIB
#endif
#else
#define IGO_TXNR_LIB
#endif

#ifdef AIR_VOICE_NR_USE_PIC_ENABLE
#if defined(AIR_HFP_DNN_PATH_ENABLE) && defined(AIR_DNN_LIB_ENABLE)
extern xtlib_packaged_library pisplit_igo_txnr;// user muts be extern your lib to here!!!
#define DNN_NR_LIB &pisplit_igo_txnr,
#else
#define DNN_NR_LIB
#endif
#else
#define DNN_NR_LIB
#endif

#ifdef AIR_BT_LE_LC3PLUS_USE_PIC
extern xtlib_packaged_library pisplit_lc3plus_codec;
#endif

#ifdef AIR_BT_LE_LC3PLUS_USE_PIC
#define LC3PLUS_LIB &pisplit_lc3plus_codec,
#else
#define LC3PLUS_LIB
#endif

#ifdef AIR_BT_A2DP_CVSD_USE_PIC_ENABLE
extern xtlib_packaged_library pisplit_cvsd_dec;// user muts be extern your lib to here!!!
extern xtlib_packaged_library pisplit_cvsd_enc;// user muts be extern your lib to here!!!
#ifdef MTK_BT_CELT_USE_PIC
extern xtlib_packaged_library pisplit_celt_codec_1565;
#ifdef AIR_CELT_MULTI_VERSIONS_SUPPORT
extern xtlib_packaged_library pisplit_celt_codec_21073101_1565;
#endif /* AIR_CELT_MULTI_VERSIONS_SUPPORT */
#endif
#define CVSD_DEC_LIB &pisplit_cvsd_dec,
#define CVSD_ENC_LIB &pisplit_cvsd_enc,
#else
#define CVSD_DEC_LIB
#define CVSD_ENC_LIB
#endif

// user muts be add your lib to here!!!
#if defined(AIR_ANC_USER_UNAWARE_ENABLE) && defined(AIR_USER_UNAWARE_USE_PIC)
extern xtlib_packaged_library pisplit_UserUnaware;// user muts be extern your lib to here!!!
#define UNAWARE_LIB &pisplit_UserUnaware,
#else
#define UNAWARE_LIB
#endif

#if defined(AIR_AFC_ENABLE) && defined(AIR_AFC_USE_PIC)
extern xtlib_packaged_library pisplit_lp_afc_1568;// user muts be extern your lib to here!!!
#define AFC_LIB &pisplit_lp_afc_1568,
#else
#define AFC_LIB
#endif

#if defined(AIR_LD_NR_ENABLE) && defined(AIR_LD_NR_USE_PIC)
extern xtlib_packaged_library pisplit_ld_nr_1565;// user muts be extern your lib to here!!!
#define LD_NR_LIB &pisplit_ld_nr_1565,
#else
#define LD_NR_LIB
#endif

#if defined(AIR_AT_AGC_ENABLE) && defined(AIR_AT_AGC_USE_PIC)
extern xtlib_packaged_library pisplit_agc_1565;// user muts be extern your lib to here!!!
#define AT_AGC_LIB &pisplit_agc_1565,
#else
#define AT_AGC_LIB
#endif

#ifdef AIR_FIXED_RATIO_SRC_USE_PIC
extern xtlib_packaged_library pisplit_sampler_by_n;
#define FIXED_RATIO_SRC_LIB &pisplit_sampler_by_n,
#else
#define FIXED_RATIO_SRC_LIB
#endif /* AIR_FIXED_RATIO_SRC_USE_PIC */

#define PIC_LIB_LIST {\
    MSBC_DEC_LIB\
    MSBC_ENC_LIB\
    PLC_LIB\
    CPD_LIB\
    AGC_LIB\
    AEQ_LIB\
    SBC_LIB\
    CLK_SKEW_LIB\
    PEQ_LIB\
    WWE_LIB_AMA\
    WWE_LIB_GSOUND\
    ECNR_LIB\
    IGO_TXNR_LIB\
    TX_EQ_LIB\
    DNN_NR_LIB\
    EC120_LIB\
    POST_EC_LIB\
    CVSD_DEC_LIB\
    CVSD_ENC_LIB\
    LC3_LIB\
    LC3I_PLC_STD_LIB\
    LC3I_PLC_ADV_LIB\
    LC3I_FFT7_5MS_LIB\
    LC3I_FFT10MS_LIB\
    LC3I_LIB\
    UNAWARE_LIB\
    AFC_LIB\
    LD_NR_LIB\
    AT_AGC_LIB\
    AAC_LIB\
	LC3PLUS_LIB\
    FIXED_RATIO_SRC_LIB\
}


#ifdef __cplusplus
}
#endif

#endif /* __PRELOADER_PILSI_CONFIGURE_H__ */
