
Build/lib/pisplit_cvsd_enc_position_independent.splitload.lib:     file format elf32-xtensa-le


Disassembly of section .text:

00000000 <_init-0xb8>:
   0:	0000010c 00000e20 0000137c 00001364     .... ...|...d...
  10:	00000000 0000134c 00000000 00001380     ....L...........
  20:	00001398 00001250 00000160 00001268     ....P...`...h...
  30:	00000418 00001284 00000450 00001370     ........P...p...
  40:	21041201 00000168 00002800 00140000     ...!h....(......
  50:	7c000000 ffe00001 7fe00000 001fffff     ...|............
  60:	00001330 00000184 00000d40 00000da0     0.......@.......
  70:	000001b4 00001300 ffff8000 00007fff     ................
  80:	00000677 00001320 000012a0 000012c0     w... ...........
  90:	000012e0 00000e40 000004b0 00000818     ....@...........
  a0:	00000964 00000aac 00000c50 00001154     d.......P...T...
  b0:	00001350 000000d0                       P.......

000000b8 <_init>:
  b8:	008136                                    	entry	a1, 64
  bb:	ffd181                                    	l32r	a8, 0 (10c <frame_dummy>)
  be:	f03d                                    	nop.n
  c0:	0008e0                                    	callx8	a8
  c3:	ffd081                                    	l32r	a8, 4 (e20 <__do_global_ctors_aux>)
  c6:	f03d                                    	nop.n
  c8:	0008e0                                    	callx8	a8
  cb:	f01d                                    	retw.n

000000cd <_init+0x15>:
  cd:	000000                                        ...

000000d0 <__do_global_dtors_aux>:
  d0:	004136                                    	entry	a1, 32
  d3:	ffcd31                                    	l32r	a3, 8 (137c <__bss_start>)
  d6:	000382                                    	l8ui	a8, a3, 0
  d9:	d8ec                                    	bnez.n	a8, 10a <__do_global_dtors_aux+0x3a>
  db:	ffcc21                                    	l32r	a2, c (1364 <p$7655_3_1>)
  de:	02b8                                    	l32i.n	a11, a2, 0
  e0:	0ba8                                    	l32i.n	a10, a11, 0
  e2:	ca8c                                    	beqz.n	a10, f2 <__do_global_dtors_aux+0x22>

000000e4 <__do_global_dtors_aux+0x14>:
  e4:	9b4b                                    	addi.n	a9, a11, 4
  e6:	0299                                    	s32i.n	a9, a2, 0
  e8:	000ae0                                    	callx8	a10
  eb:	02b8                                    	l32i.n	a11, a2, 0
  ed:	0ba8                                    	l32i.n	a10, a11, 0
  ef:	ff1a56                                    	bnez	a10, e4 <__do_global_dtors_aux+0x14>
  f2:	ffc7a1                                    	l32r	a10, 10 (0 <_text_start>)
  f5:	0b0c                                    	movi.n	a11, 0
  f7:	081ab7                                    	beq	a10, a11, 103 <__do_global_dtors_aux+0x33>
  fa:	ffc6a1                                    	l32r	a10, 14 (134c <__EH_FRAME_BEGIN___100>)
  fd:	ffc481                                    	l32r	a8, 10 (0 <_text_start>)
 100:	0008e0                                    	callx8	a8
 103:	1c0c                                    	movi.n	a12, 1
 105:	0043c2                                    	s8i	a12, a3, 0
 108:	f01d                                    	retw.n

0000010a <__do_global_dtors_aux+0x3a>:
 10a:	f01d                                    	retw.n

0000010c <frame_dummy>:
 10c:	004136                                    	entry	a1, 32
 10f:	ffc281                                    	l32r	a8, 18 (0 <_text_start>)
 112:	090c                                    	movi.n	a9, 0
 114:	0b1897                                    	beq	a8, a9, 123 <frame_dummy+0x17>
 117:	ffbfa1                                    	l32r	a10, 14 (134c <__EH_FRAME_BEGIN___100>)
 11a:	ffc0b1                                    	l32r	a11, 1c (1380 <object$7674_5_1>)
 11d:	ffbe81                                    	l32r	a8, 18 (0 <_text_start>)
 120:	0008e0                                    	callx8	a8

00000123 <frame_dummy+0x17>:
 123:	f01d                                    	retw.n

00000125 <frame_dummy+0x19>:
 125:	000000                                        ...

00000128 <_start>:
 128:	004136                                    	entry	a1, 32
 12b:	ffbea1                                    	l32r	a10, 24 (1250 <_DYNAMIC+0x37c>)
 12e:	ffbc31                                    	l32r	a3, 20 (1398 <printf_ptr>)
 131:	0288                                    	l32i.n	a8, a2, 0
 133:	ffbdb1                                    	l32r	a11, 28 (160 <get_cvsd_version>)
 136:	0389                                    	s32i.n	a8, a3, 0
 138:	0008e0                                    	callx8	a8
 13b:	0388                                    	l32i.n	a8, a3, 0
 13d:	ffbba1                                    	l32r	a10, 2c (1268 <_DYNAMIC+0x394>)
 140:	ffbcb1                                    	l32r	a11, 30 (418 <_CVSD_Encoder_Init>)
 143:	0008e0                                    	callx8	a8
 146:	0388                                    	l32i.n	a8, a3, 0
 148:	ffbba1                                    	l32r	a10, 34 (1284 <_DYNAMIC+0x3b0>)
 14b:	ffbbb1                                    	l32r	a11, 38 (450 <_CVSD_Encoder>)
 14e:	0008e0                                    	callx8	a8
 151:	ffba21                                    	l32r	a2, 3c (1370 <export_parameter_array>)
 154:	f01d                                    	retw.n

00000156 <_start+0x2e>:
	...

00000160 <get_cvsd_version>:
 160:	004136                                    	entry	a1, 32
 163:	ffb721                                    	l32r	a2, 40 (21041201 <_end+0x2103fe65>)
 166:	f01d                                    	retw.n

00000168 <cvsd_codec_init>:
 168:	004136                                    	entry	a1, 32
 16b:	080c                                    	movi.n	a8, 0
 16d:	03042a400080128e                  	{ s32i	a8, a2, 0; movi	a3, 10 }
 175:	1289                                    	s32i.n	a8, a2, 4
 177:	2906b8401088023e                  	{ s32i	a3, a2, 8; or	a2, a8, a8 }
 17f:	f01d                                    	retw.n

00000181 <cvsd_codec_init+0x19>:
 181:	000000                                        ...

00000184 <cvsd_encode_init>:
 184:	004136                                    	entry	a1, 32
 187:	ffaf81                                    	l32r	a8, 44 (168 <cvsd_codec_init>)
 18a:	03ad                                    	mov.n	a10, a3
 18c:	0008e0                                    	callx8	a8
 18f:	2b06ba400088023e                  	{ s32i	a3, a2, 0; or	a2, a10, a10 }
 197:	f01d                                    	retw.n

00000199 <cvsd_encode_init+0x15>:
 199:	000000                                        ...

0000019c <cvsd_decode_init>:
 19c:	004136                                    	entry	a1, 32
 19f:	ffa981                                    	l32r	a8, 44 (168 <cvsd_codec_init>)
 1a2:	03ad                                    	mov.n	a10, a3
 1a4:	0008e0                                    	callx8	a8
 1a7:	2b06ba400088023e                  	{ s32i	a3, a2, 0; or	a2, a10, a10 }
 1af:	f01d                                    	retw.n

000001b1 <cvsd_decode_init+0x15>:
 1b1:	000000                                        ...

000001b4 <cvsd_encode_process>:
 1b4:	006136                                    	entry	a1, 48
 1b7:	ffa6f1                                    	l32r	a15, 50 (7c000000 <_end+0x7bffec64>)
 1ba:	ffa6e1                                    	l32r	a14, 54 (ffe00001 <_end+0xffdfec65>)
 1bd:	0903a4400880116e                  	{ s32i	a6, a1, 4; l32i	a9, a4, 0 }
 1c5:	0606d640008b00be                  	{ movi	a11, 0; or	a12, a6, a6 }
 1cd:	67efb4                                    	wur.ae_bitptr	a11
 1d0:	02069240048a0cae                  	{ l32i	a10, a12, 0; or	a8, a2, a2 }
 1d8:	f3f1b0                                    	wur.ae_bithead	a11
 1db:	1906b5c00096112e                  	{ s32i	a2, a1, 0; srli	a9, a9, 5 }
 1e3:	1b06e9c60c16e92e                  	{ l32r	a2, 48 (2800 <_end+0x1464>); minu	a9, a9, a10 }
 1eb:	3b06e9460c06f86e                  	{ l32r	a6, 4c (140000 <_end+0x13ec64>); slli	a11, a9, 5 }
 1f3:	2b02a5c0009f04be                  	{ s32i	a11, a4, 0; addi	a10, a5, -2 }
 1fb:	3d06a94000850c9e                  	{ s32i	a9, a12, 0; slli	a7, a9, 4 }
 203:	18038840048018be                  	{ l32i	a11, a8, 0; l32i	a9, a8, 4 }
 20b:	2503c84718a0478e                  	{ loopnez	a7, 26e <cvsd_encode_process+0xba>; l32i	a4, a8, 8 }
 213:	23cd                                    	ae_l16si.n	a12, a3, 0
 215:	11dbf0                                    	slli	a13, a11, 1
 218:	b2ddf0                                    	mulsh	a13, a13, a15
 21b:	11cca0                                    	slli	a12, a12, 6
 21e:	c0ccd0                                    	sub	a12, a12, a13
 221:	3d022f4100a6214e                  	{ srai	a4, a4, 1; extui	a11, a12, 31, 1 }
 229:	3d068b4404e57abe                  	{ ae_sbi	a10, a11, 1; slli	a7, a11, 3 }
 231:	0606d44000891f7e                  	{ movi	a7, 15; or	a4, a4, a7 }
 239:	2302c3000800d47e                  	{ beq.w15	a4, a7, 2d7 <cvsd_encode_process+0x123>; addi	a3, a3, 2 }
 241:	0f06c920000694ae                  	{ beqz.w15	a4, 2d7 <cvsd_encode_process+0x123>; slli	a8, a9, 1 }
 249:	ff8371                                    	l32r	a7, 58 (7fe00000 <_end+0x7fdfec64>)
 24c:	b28870                                    	mulsh	a8, a8, a7

0000024f <cvsd_encode_process+0x9b>:
 24f:	439860                                    	min	a9, a8, a6
 252:	539920                                    	max	a9, a9, a2
 255:	3906e4460409f1be                  	{ l32r	a11, 5c (1fffff <_end+0x1fec63>); srai	a7, a9, 4 }

0000025d <cvsd_encode_process+0xa9>:
 25d:	07068d411cf25d7e                  	{ sub	a7, a13, a7; add	a8, a13, a7 }
 265:	b378c0                                    	movgez	a7, a8, a12
 268:	43b7b0                                    	min	a11, a7, a11
 26b:	53bbe0                                    	max	a11, a11, a14

0000026e <cvsd_encode_process+0xba>:
 26e:	11c8                                    	l32i.n	a12, a1, 4
 270:	0cc8                                    	l32i.n	a12, a12, 0
 272:	ff7ba1                                    	l32r	a10, 60 (1330 <cvsd_BitReverseTable16>)
 275:	0178                                    	l32i.n	a7, a1, 0
 277:	f03d                                    	nop.n
 279:	0304404410a04c8e                  	{ loopnez	a12, 2cf <cvsd_encode_process+0x11b>; movi	a2, 0 }
 281:	453d                                    	ae_l16ui.n	a3, a5, 0
 283:	34e830                                    	extui	a14, a3, 8, 4
 286:	13026443187b303e                  	{ extui	a15, a3, 0, 4; extui	a13, a3, 4, 4 }
 28e:	32069cc31cf43afe                  	{ add	a15, a10, a15; srli	a3, a3, 12 }
 296:	2f068a4004930ffe                  	{ l8ui	a15, a15, 0; add	a14, a10, a14 }
 29e:	0e039e401ce02a3e                  	{ add	a3, a10, a3; l8ui	a14, a14, 0 }
 2a6:	1d068a400493133e                  	{ l8ui	a3, a3, 0; add	a13, a10, a13 }
 2ae:	000dd2                                    	l8ui	a13, a13, 0
 2b1:	11ee40                                    	slli	a14, a14, 12
 2b4:	3906a34718a40fce                  	{ slli	a15, a15, 4; slli	a3, a3, 8 }
 2bc:	2206be4708ebfdfe                  	{ or	a13, a13, a15; or	a14, a14, a3 }
 2c4:	20dde0                                    	or	a13, a13, a14

000002c7 <cvsd_encode_process+0x113>:
 2c7:	2402a540008015de                  	{ s16i	a13, a5, 0; addi	a5, a5, 2 }

000002cf <cvsd_encode_process+0x11b>:
 2cf:	07b9                                    	s32i.n	a11, a7, 0
 2d1:	1799                                    	s32i.n	a9, a7, 4
 2d3:	2749                                    	s32i.n	a4, a7, 8
 2d5:	f01d                                    	retw.n

000002d7 <cvsd_encode_process+0x123>:
 2d7:	090299471c6ae47e                  	{ j	24f <cvsd_encode_process+0x9b>; addmi	a8, a9, 0x2800 }

000002df <cvsd_encode_process+0x12b>:
	...

000002e0 <cvsd_decode_process>:
 2e0:	006136                                    	entry	a1, 48
 2e3:	0688                                    	l32i.n	a8, a6, 0
 2e5:	04a8                                    	l32i.n	a10, a4, 0
 2e7:	418580                                    	srli	a8, a8, 5
 2ea:	0b06e8c50c16edee                  	{ l32r	a14, 60 (1330 <cvsd_BitReverseTable16>); minu	a8, a8, a10 }
 2f2:	006182                                    	s32i	a8, a1, 0
 2f5:	0b06a8400086148e                  	{ s32i	a8, a4, 0; slli	a8, a8, 5 }
 2fd:	1306b240008a068e                  	{ s32i	a8, a6, 0; or	a9, a2, a2 }
 305:	220693400488147e                  	{ l32i	a7, a4, 0; or	a2, a3, a3 }
 30d:	2302a3c0089f119e                  	{ s32i	a9, a1, 4; addi	a3, a3, -2 }
 315:	1a0389400480098e                  	{ l32i	a8, a9, 0; l32i	a10, a9, 4 }
 31d:	2503c94410a0478e                  	{ loopnez	a7, 373 <cvsd_decode_process+0x93>; l32i	a4, a9, 8 }
 325:	42fd                                    	ae_l16ui.n	a15, a2, 0
 327:	34c8f0                                    	extui	a12, a15, 8, 4
 32a:	3f026443087a30fe                  	{ extui	a13, a15, 0, 4; extui	a11, a15, 4, 4 }
 332:	3e069cc30cf73ede                  	{ add	a13, a14, a13; srli	a15, a15, 12 }
 33a:	0d068e4004930dde                  	{ l8ui	a13, a13, 0; add	a12, a14, a12 }
 342:	0c039c431ce02efe                  	{ add	a15, a14, a15; l8ui	a12, a12, 0 }
 34a:	3b068e4004921ffe                  	{ l8ui	a15, a15, 0; add	a11, a14, a11 }
 352:	000bb2                                    	l8ui	a11, a11, 0
 355:	11cc40                                    	slli	a12, a12, 12
 358:	3906af4708a70dce                  	{ slli	a13, a13, 4; slli	a15, a15, 8 }
 360:	0e06bc4618ebfbde                  	{ or	a11, a11, a13; or	a12, a12, a15 }
 368:	20bbc0                                    	or	a11, a11, a12
 36b:	2202a240008002be                  	{ s16i	a11, a2, 0; addi	a2, a2, 2 }

00000373 <cvsd_decode_process+0x93>:
 373:	ff3921                                    	l32r	a2, 58 (7fe00000 <_end+0x7fdfec64>)
 376:	ff34e1                                    	l32r	a14, 48 (2800 <_end+0x1464>)
 379:	ff34d1                                    	l32r	a13, 4c (140000 <_end+0x13ec64>)
 37c:	0b0460441c00f5ce                  	{ l32r	a12, 50 (7c000000 <_end+0x7bffec64>); movi	a11, 0 }
 384:	f3f1b0                                    	wur.ae_bithead	a11
 387:	67efb4                                    	wur.ae_bitptr	a11
 38a:	0603a14218e053fe                  	{ ae_dbi	a3, 16; l32i	a7, a1, 0 }
 392:	ff32f1                                    	l32r	a15, 5c (1fffff <_end+0x1fec63>)
 395:	ff2fb1                                    	l32r	a11, 54 (ffe00001 <_end+0xffdfec65>)
 398:	3c06a74218e543fe                  	{ ae_dbi	a3, 16; slli	a7, a7, 4 }
 3a0:	5c9776                                    	loopnez	a7, 400 <cvsd_decode_process+0x120>
 3a3:	1198f0                                    	slli	a9, a8, 1
 3a6:	b299c0                                    	mulsh	a9, a9, a12
 3a9:	0406c14018890f7e                  	{ movi	a7, 63; srai	a4, a4, 1 }
 3b1:	28068624000958ee                  	{ bgez.w15	a8, 3be <cvsd_decode_process+0xde>; srai	a6, a9, 6 }
 3b9:	010797                                    	bnone	a7, a9, 3be <cvsd_decode_process+0xde>
 3bc:	661b                                    	addi.n	a6, a6, 1
 3be:	656d                                    	ae_s16i.n	a6, a5, 0
 3c0:	2e8350903800330e                  	{ ae_lbi_dbi	a3, a6, 1; nop; nop }
 3c8:	1176d0                                    	slli	a7, a6, 3
 3cb:	08042f4500e0e47e                  	{ or	a4, a4, a7; movi	a8, 15 }
 3d3:	240285040c00d48e                  	{ beq.w15	a4, a8, 410 <cvsd_decode_process+0x130>; addi	a5, a5, 2 }
 3db:	0e068a24040694ae                  	{ beqz.w15	a4, 410 <cvsd_decode_process+0x130>; slli	a8, a10, 1 }
 3e3:	b28820                                    	mulsh	a8, a8, a2

000003e6 <cvsd_decode_process+0x106>:
 3e6:	43a8d0                                    	min	a10, a8, a13
 3e9:	53aae0                                    	max	a10, a10, a14
 3ec:	2184a0                                    	srai	a8, a10, 4

000003ef <cvsd_decode_process+0x10f>:
 3ef:	3906894204f1498e                  	{ sub	a8, a9, a8; add	a7, a9, a8 }
 3f7:	838760                                    	moveqz	a8, a7, a6
 3fa:	5388b0                                    	max	a8, a8, a11
 3fd:	4388f0                                    	min	a8, a8, a15

00000400 <cvsd_decode_process+0x120>:
 400:	11b8                                    	l32i.n	a11, a1, 4
 402:	0b89                                    	s32i.n	a8, a11, 0
 404:	1ba9                                    	s32i.n	a10, a11, 4
 406:	0304204010800b4e                  	{ s32i	a4, a11, 8; movi	a2, 0 }
 40e:	f01d                                    	retw.n

00000410 <cvsd_decode_process+0x130>:
 410:	09029a471c6ae2de                  	{ j	3e6 <cvsd_decode_process+0x106>; addmi	a8, a10, 0x2800 }

00000418 <_CVSD_Encoder_Init>:
 418:	004136                                    	entry	a1, 32
 41b:	1a02d24018820c8e                  	{ movi	a8, 60; addmi	a10, a2, 0x900 }
 423:	5a5a82                                    	s16i	a8, a10, 180
 426:	051366                                    	bnei	a3, 1, 42f <_CVSD_Encoder_Init+0x17>
 429:	78a092                                    	movi	a9, 120
 42c:	5a5a92                                    	s16i	a9, a10, 180

0000042f <_CVSD_Encoder_Init+0x17>:
 42f:	ff0d81                                    	l32r	a8, 64 (184 <cvsd_encode_init>)
 432:	2206f242046a02be                  	{ addi	a11, a2, 8; or	a10, a2, a2 }
 43a:	0008e0                                    	callx8	a8
 43d:	ff0a81                                    	l32r	a8, 68 (d40 <cvsd_src_up_sampling_init>)
 440:	0a02e245046102be                  	{ addi	a11, a2, 20; addi	a10, a2, 4 }
 448:	0008e0                                    	callx8	a8
 44b:	f01d                                    	retw.n

0000044d <_CVSD_Encoder_Init+0x35>:
 44d:	000000                                        ...

00000450 <_CVSD_Encoder>:
 450:	006136                                    	entry	a1, 48
 453:	03bd                                    	mov.n	a11, a3
 455:	1b03e2420c60023e                  	{ addmi	a3, a2, 0x900; l32i	a10, a2, 4 }
 45d:	2e036345047642ce                  	{ addi	a12, a2, 84; l16si	a14, a3, 180 }
 465:	2502f2440400f18e                  	{ l32r	a8, 6c (da0 <cvsd_src_up_sampling_process>); addmi	a5, a2, 0x200 }
 46d:	0402e546046db33e                  	{ addi	a3, a3, -72; addi	a5, a5, 52 }
 475:	1506b54710ab1efe                  	{ slli	a14, a14, 1; or	a13, a5, a5 }
 47d:	0008e0                                    	callx8	a8
 480:	14069447108b639e                  	{ l16si	a9, a3, 252; or	a13, a4, a4 }
 488:	34069540048a12ae                  	{ l32i	a10, a2, 0; or	a11, a5, a5 }
 490:	0106f1431c0bf88e                  	{ l32r	a8, 70 (1b4 <cvsd_encode_process>); or	a12, a1, a1 }
 498:	1d06a9400886019e                  	{ s32i	a9, a1, 4; slli	a9, a9, 4 }
 4a0:	0f02a1400081019e                  	{ s32i	a9, a1, 0; addi	a14, a1, 4 }
 4a8:	0008e0                                    	callx8	a8
 4ab:	7e9322                                    	l16si	a2, a3, 252
 4ae:	f01d                                    	retw.n

000004b0 <cvsd_src_up_sampling_mono_process_1>:
 4b0:	008136                                    	entry	a1, 64
 4b3:	f3f640                                    	wur.ae_cbegin0	a4
 4b6:	37046ec3141fef7e                  	{ l32r	a7, 74 (1300 <cvsd_src_up_sampling_coef_1>); movi	a6, -2 }
 4be:	2606e5c70466048e                  	{ addi	a8, a4, 28; and	a10, a5, a6 }
 4c6:	f3f780                                    	wur.ae_cend0	a8
 4c9:	0903e4420483a07e                  	{ ae_l16x2m.i	aed8, a7, 0; l32i	a8, a4, 48 }
 4d1:	130477c11c86a17e                  	{ ae_l16x2m.i	aed7, a7, 4; ae_l16x2m.i	aed6, a7, 8 }
 4d9:	150477c10c84a37e                  	{ ae_l16x2m.i	aed5, a7, 12; ae_l16x2m.i	aed4, a7, 16 }
 4e1:	170477c01c82a57e                  	{ ae_l16x2m.i	aed3, a7, 20; ae_l16x2m.i	aed2, a7, 24 }
 4e9:	070444031820556e                  	{ bnone.w15	a5, a6, 677 <cvsd_src_up_sampling_mono_process_1+0x1c7>; movi	a7, 4 }
 4f1:	2502e2c3141ff19e                  	{ l32r	a9, 78 (ffff8000 <_end+0xffff6c64>); addi	a5, a2, -2 }
 4f9:	030460431400e0be                  	{ l32r	a11, 7c (7fff <_end+0x6c63>); movi	a2, 0 }
 501:	0ead456b01e193d0c480813900011b0f 	{ ae_cvt48a32	aed1, a11; ae_cvt48a32	aed0, a9; nop; nop }
 511:	0904224318e0747e                  	{ nop; movi	a9, 2 }
 519:	15aa16                                    	beqz	a10, 677 <cvsd_src_up_sampling_mono_process_1+0x1c7>
 51c:	179a76                                    	loopnez	a10, 537 <cvsd_src_up_sampling_mono_process_1+0x87>
 51f:	0301a0                                    	rsr.lend	a10
 522:	1300a0                                    	wsr.lbeg	a10
 525:	fed6a1                                    	l32r	a10, 80 (677 <cvsd_src_up_sampling_mono_process_1+0x1c7>)
 528:	0020f0                                    	nop
 52b:	1301a0                                    	wsr.lend	a10
 52e:	002000                                    	isync
 531:	0302a0                                    	rsr.lcount	a10
 534:	01caa2                                    	addi	a10, a10, 1
 537:	06003102002c020e                  	{ bbsi.w15	a2, 0, 67b <cvsd_src_up_sampling_mono_process_1+0x1cb>; ae_sel16i	aed12, aed2, aed3, 2 }
 53f:	220760c31c85f78e                  	{ ae_l16x2m.xc	aed15, a8, a7; nop }
 547:	2a82317e2de2578e                  	{ ae_l16x2m.xc	aed21, a8, a7; nop; ae_mulzaad32.hl.lh	aed15, aed15, aed2 }
 54f:	1c82327e2ea3478e                  	{ ae_l16x2m.xc	aed20, a8, a7; nop; ae_mulaad32.hl.lh	aed15, aed21, aed3 }
 557:	1c82327e2e84378e                  	{ ae_l16x2m.xc	aed19, a8, a7; nop; ae_mulaad32.hl.lh	aed15, aed20, aed4 }
 55f:	1c82327e2e65278e                  	{ ae_l16x2m.xc	aed18, a8, a7; nop; ae_mulaad32.hl.lh	aed15, aed19, aed5 }
 567:	1c82327e2e46178e                  	{ ae_l16x2m.xc	aed17, a8, a7; nop; ae_mulaad32.hl.lh	aed15, aed18, aed6 }
 56f:	1c82327e2e27078e                  	{ ae_l16x2m.xc	aed16, a8, a7; nop; ae_mulaad32.hl.lh	aed15, aed17, aed7 }
 577:	1c83527b3608b13e                  	{ nop; nop; ae_mulaad32.hl.lh	aed15, aed16, aed8 }
 57f:	0ead257b09640cd0c580813a0c01e2ff 	{ ae_srai64	aed15, aed15, 10; nop; nop; nop }
 58f:	0ead057b08e40cd0c48081380c0137ff 	{ ae_min64	aed15, aed15, aed1; nop; nop; nop }
 59f:	0ead057b08e40cd0c4808138040117ff 	{ ae_max64	aed15, aed15, aed0; nop; nop; nop }
 5af:	220700c318e5931e                  	{ ae_s64.i	aed15, a1, 0; nop }
 5b7:	230720c310c5101e                  	{ ae_l32f24.i	aed14, a1, 0; nop }
 5bf:	230760c314c5d21e                  	{ ae_s32.l.i	aed14, a1, 8; nop }
 5c7:	220720c30cc5321e                  	{ ae_l32.i	aed13, a1, 8; nop }
 5cf:	220740c30cc5103e                  	{ ae_s16m.l.i	aed13, a3, 0; nop }
 5d7:	220760c30485f78e                  	{ ae_l16x2m.xc	aed12, a8, a7; nop }
 5df:	2a8230662d88278e                  	{ ae_l16x2m.xc	aed18, a8, a7; nop; ae_mulzaad32.hh.ll	aed12, aed12, aed8 }
 5e7:	1c8231662e47178e                  	{ ae_l16x2m.xc	aed17, a8, a7; nop; ae_mulaad32.hh.ll	aed12, aed18, aed7 }
 5ef:	1c8231662e26078e                  	{ ae_l16x2m.xc	aed16, a8, a7; nop; ae_mulaad32.hh.ll	aed12, aed17, aed6 }
 5f7:	1c8231662a05f78e                  	{ ae_l16x2m.xc	aed15, a8, a7; nop; ae_mulaad32.hh.ll	aed12, aed16, aed5 }
 5ff:	1c82316629e4e78e                  	{ ae_l16x2m.xc	aed14, a8, a7; nop; ae_mulaad32.hh.ll	aed12, aed15, aed4 }
 607:	1c82316629c3d78e                  	{ ae_l16x2m.xc	aed13, a8, a7; nop; ae_mulaad32.hh.ll	aed12, aed14, aed3 }
 60f:	1c83516335a2b13e                  	{ nop; nop; ae_mulaad32.hh.ll	aed12, aed13, aed2 }
 617:	0ead257b09640cd0c580813a0c0122cf 	{ ae_srai64	aed12, aed12, 10; nop; nop; nop }
 627:	0ead057b08e40cd0c48081380c0134cf 	{ ae_min64	aed12, aed12, aed1; nop; nop; nop }
 637:	0ead057b08e40cd0c4808138040114cf 	{ ae_max64	aed12, aed12, aed0; nop; nop; nop }
 647:	1004154300eb931e                  	{ ae_s64.i	aed12, a1, 0; ae_l16m.iu	aed11, a5, 2 }
 64f:	3104d1421cca298e                  	{ ae_s16m.l.xc	aed11, a8, a9; ae_l32f24.i	aed10, a1, 0 }
 657:	230760c214c5d31e                  	{ ae_s32.l.i	aed10, a1, 12; nop }
 65f:	220720c20cc5331e                  	{ ae_l32.i	aed9, a1, 12; nop }
 667:	220740c20cc5113e                  	{ ae_s16m.l.i	aed9, a3, 2; nop }

0000066f <cvsd_src_up_sampling_mono_process_1+0x1bf>:
 66f:	1202e2410460033e                  	{ addi	a3, a3, 4; addi	a2, a2, 1 }
 677:	c489                                    	s32i.n	a8, a4, 48
 679:	f01d                                    	retw.n

0000067b <cvsd_src_up_sampling_mono_process_1+0x1cb>:
 67b:	230760c51485998e                  	{ ae_l16m.xc	aed22, a8, a9; nop }
 683:	1a8235b62ac2b78e                  	{ ae_l16x2m.xc	aed11, a8, a7; nop; ae_mul32.ll	aed22, aed22, aed2 }
 68b:	0e152fbb0c440c5becab3e2105435b8f 	{ ae_l16x2m.xc	aed9, a8, a7; nop; ae_sel16i.n	aed10, aed3, aed4, 2; ae_mulaad32.hh.ll	aed22, aed11, aed12 }
 69b:	0e5533fb0c540c07c490afb3052a9f8f 	{ ae_l16x2m.xc	aed30, a8, a7; nop; ae_mulaad32.hh.ll	aed22, aed9, aed10; ae_sel16i	aed31, aed4, aed5, 2 }
 6ab:	0e153fbb0c5c0cf3ecab3e3307a51f8f 	{ ae_l16x2m.xc	aed28, a8, a7; nop; ae_sel16i.n	aed29, aed5, aed6, 2; ae_mulaad32.hh.ll	aed22, aed30, aed31 }
 6bb:	0e5533fb0c540c06c498efb3079d9b8f 	{ ae_l16x2m.xc	aed26, a8, a7; nop; ae_mulaad32.hh.ll	aed22, aed28, aed29; ae_sel16i	aed27, aed6, aed7, 2 }
 6cb:	0e153bbb0c5c0cd3ecab3e4307271b8f 	{ ae_l16x2m.xc	aed24, a8, a7; nop; ae_sel16i.n	aed25, aed7, aed8, 2; ae_mulaad32.hh.ll	aed22, aed26, aed27 }
 6db:	1c8231b43f19798e                  	{ ae_l16m.xc	aed23, a8, a9; nop; ae_mulaad32.hh.ll	aed22, aed24, aed25 }
 6e3:	1c8345b336e8b13e                  	{ nop; nop; ae_mula32.hh	aed22, aed23, aed8 }
 6eb:	0ead257b09a50cd0c580813a0c01a26f 	{ ae_srai64	aed22, aed22, 10; nop; nop; nop }
 6fb:	0ead057b08e50cd0c48081380c013a6f 	{ ae_min64	aed22, aed22, aed1; nop; nop; nop }
 70b:	0ead057b08e50cd0c480813804011a6f 	{ ae_max64	aed22, aed22, aed0; nop; nop; nop }
 71b:	220700c510e5931e                  	{ ae_s64.i	aed22, a1, 0; nop }
 723:	230720c508c5101e                  	{ ae_l32f24.i	aed21, a1, 0; nop }
 72b:	230760c50cc5d41e                  	{ ae_s32.l.i	aed21, a1, 16; nop }
 733:	220720c504c5341e                  	{ ae_l32.i	aed20, a1, 16; nop }
 73b:	220740c504c5103e                  	{ ae_s16m.l.i	aed20, a3, 0; nop }
 743:	230760c41c85998e                  	{ ae_l16m.xc	aed19, a8, a9; nop }
 74b:	0e5533db0c440c0244a0f29b0668df8f 	{ ae_l16x2m.xc	aed31, a8, a7; nop; ae_mul32.lh	aed19, aed19, aed8; ae_sel16i	aed9, aed8, aed7, 0 }
 75b:	0e152bbb0c4c0cfbe6ab3c3307c75f8f 	{ ae_l16x2m.xc	aed29, a8, a7; nop; ae_sel16i.n	aed30, aed7, aed6, 0; ae_mulaad32.hh.ll	aed19, aed31, aed9 }
 76b:	0e5533fb0c440c070498af9b07bedb8f 	{ ae_l16x2m.xc	aed27, a8, a7; nop; ae_mulaad32.hh.ll	aed19, aed29, aed30; ae_sel16i	aed28, aed6, aed5, 0 }
 77b:	0e153fbb0c440cdbe6ab3c2307455b8f 	{ ae_l16x2m.xc	aed25, a8, a7; nop; ae_sel16i.n	aed26, aed5, aed4, 0; ae_mulaad32.hh.ll	aed19, aed27, aed28 }
 78b:	0e5533fb0c440c0604906f9b073ad78f 	{ ae_l16x2m.xc	aed23, a8, a7; nop; ae_mulaad32.hh.ll	aed19, aed25, aed26; ae_sel16i	aed24, aed4, aed3, 0 }
 79b:	0e153bbb0c440cbbe6ab3c1306c3578f 	{ ae_l16x2m.xc	aed21, a8, a7; nop; ae_sel16i.n	aed22, aed3, aed2, 0; ae_mulaad32.hh.ll	aed19, aed23, aed24 }
 7ab:	1c82319c3eb6498e                  	{ ae_l16m.xc	aed20, a8, a9; nop; ae_mulaad32.hh.ll	aed19, aed21, aed22 }
 7b3:	1c83469b3454b13e                  	{ nop; nop; ae_mula32.lh	aed19, aed2, aed20 }
 7bb:	0ead257b09850cd0c580813a0c01e23f 	{ ae_srai64	aed19, aed19, 10; nop; nop; nop }
 7cb:	0ead057b08e50cd0c48081380c012b3f 	{ ae_min64	aed19, aed19, aed1; nop; nop; nop }
 7db:	0ead057b08e50cd0c480813804010b3f 	{ ae_max64	aed19, aed19, aed0; nop; nop; nop }
 7eb:	1004154418f2931e                  	{ ae_s64.i	aed19, a1, 0; ae_l16m.iu	aed18, a5, 2 }
 7f3:	3104d14414d1298e                  	{ ae_s16m.l.xc	aed18, a8, a9; ae_l32f24.i	aed17, a1, 0 }
 7fb:	230760c40cc5d51e                  	{ ae_s32.l.i	aed17, a1, 20; nop }
 803:	220720c404c5351e                  	{ ae_l32.i	aed16, a1, 20; nop }
 80b:	220740c404c5113e                  	{ ae_s16m.l.i	aed16, a3, 2; nop }
 813:	ff9606                                    	j	66f <cvsd_src_up_sampling_mono_process_1+0x1bf>

00000816 <cvsd_src_up_sampling_mono_process_1+0x366>:
	...

00000818 <cvsd_src_up_sampling_mono_process_2>:
 818:	006136                                    	entry	a1, 48
 81b:	37046ec00c1fea7e                  	{ l32r	a7, 84 (1320 <cvsd_src_up_sampling_coef_2>); movi	a6, -2 }
 823:	0a02e4411468249e                  	{ addi	a9, a4, 38; addi	a10, a4, 32 }
 82b:	f3f6a0                                    	wur.ae_cbegin0	a10
 82e:	f3f790                                    	wur.ae_cend0	a9
 831:	1903a44608a3456e                  	{ and	a9, a5, a6; l32i	a8, a4, 52 }
 839:	110477c10c84b07e                  	{ ae_l16x2m.i	aed5, a7, 0; ae_l16x2m.i	aed4, a7, 4 }
 841:	330689071838d56e                  	{ bnone.w15	a5, a6, 960 <cvsd_src_up_sampling_mono_process_2+0x148>; addx4	a3, a9, a3 }
 849:	2202e2c7047fe33e                  	{ addi	a3, a3, -4; addi	a2, a2, -2 }
 851:	050462400400f9be                  	{ l32r	a11, 78 (ffff8000 <_end+0xffff6c64>); movi	a5, 2 }
 859:	21076b400401e8ce                  	{ l32r	a12, 7c (7fff <_end+0x6c63>); ae_cvt48a32	aed1, a11 }
 861:	0020f0                                    	nop
 864:	f03d                                    	nop.n
 866:	f03d                                    	nop.n
 868:	21074c4500a3e98e                  	{ loopnez	a9, 960 <cvsd_src_up_sampling_mono_process_2+0x148>; ae_cvt48a32	aed3, a12 }
 870:	230760c21485958e                  	{ ae_l16m.xc	aed10, a8, a5; nop }
 878:	1a8233543944c58e                  	{ ae_l16m.xc	aed12, a8, a5; nop; ae_mul32.hh	aed10, aed10, aed4 }
 880:	1c82265438acb58e                  	{ ae_l16m.xc	aed11, a8, a5; nop; ae_mula32.lh	aed10, aed5, aed12 }
 888:	1c8345533565b13e                  	{ nop; nop; ae_mula32.hh	aed10, aed11, aed5 }
 890:	0ead257b09440cd0c580813a0c0191af 	{ ae_srai64	aed10, aed10, 5; nop; nop; nop }
 8a0:	0ead057b08e40cd0c48081381c0126af 	{ ae_min64	aed10, aed10, aed3; nop; nop; nop }
 8b0:	0ead057b08e40cd0c48081380c0106af 	{ ae_max64	aed10, aed10, aed1; nop; nop; nop }
 8c0:	220700c210e5931e                  	{ ae_s64.i	aed10, a1, 0; nop }
 8c8:	230720c208c5101e                  	{ ae_l32f24.i	aed9, a1, 0; nop }
 8d0:	230760c20cc5d21e                  	{ ae_s32.l.i	aed9, a1, 8; nop }
 8d8:	220720c204c5321e                  	{ ae_l32.i	aed8, a1, 8; nop }
 8e0:	230740c204c5123e                  	{ ae_s16m.l.iu	aed8, a3, 4; nop }
 8e8:	230760c11c85958e                  	{ ae_l16m.xc	aed7, a8, a5; nop }
 8f0:	1a82333c38e5958e                  	{ ae_l16m.xc	aed9, a8, a5; nop; ae_mul32.hh	aed7, aed7, aed5 }
 8f8:	1c82263c38a9858e                  	{ ae_l16m.xc	aed8, a8, a5; nop; ae_mula32.lh	aed7, aed5, aed9 }
 900:	1c83453b3504b13e                  	{ nop; nop; ae_mula32.hh	aed7, aed8, aed4 }
 908:	0ead257b09240cd0c580813a0c01d17f 	{ ae_srai64	aed7, aed7, 5; nop; nop; nop }
 918:	0ead057b08e40cd0c48081381c01337f 	{ ae_min64	aed7, aed7, aed3; nop; nop; nop }
 928:	0ead057b08e40cd0c48081380c01137f 	{ ae_max64	aed7, aed7, aed1; nop; nop; nop }
 938:	1004124118e6931e                  	{ ae_s64.i	aed7, a1, 0; ae_l16m.iu	aed6, a2, 2 }
 940:	3104d14114c2258e                  	{ ae_s16m.l.xc	aed6, a8, a5; ae_l32f24.i	aed2, a1, 0 }
 948:	230760c014c5d31e                  	{ ae_s32.l.i	aed2, a1, 12; nop }
 950:	220720c004c5331e                  	{ ae_l32.i	aed0, a1, 12; nop }
 958:	220740c004c5113e                  	{ ae_s16m.l.i	aed0, a3, 2; nop }

00000960 <cvsd_src_up_sampling_mono_process_2+0x148>:
 960:	d489                                    	s32i.n	a8, a4, 52
 962:	f01d                                    	retw.n

00000964 <cvsd_src_up_sampling_mono_process_3>:
 964:	006136                                    	entry	a1, 48
 967:	36046ec7041fe77e                  	{ l32r	a7, 84 (1320 <cvsd_src_up_sampling_coef_2>); movi	a6, -2 }
 96f:	0a02e443146a249e                  	{ addi	a9, a4, 46; addi	a10, a4, 40 }
 977:	f3f6a0                                    	wur.ae_cbegin0	a10
 97a:	f3f790                                    	wur.ae_cend0	a9
 97d:	2903a44608a3456e                  	{ and	a9, a5, a6; l32i	a8, a4, 56 }
 985:	110477c10c84b07e                  	{ ae_l16x2m.i	aed5, a7, 0; ae_l16x2m.i	aed4, a7, 4 }
 98d:	220689031834a56e                  	{ bnone.w15	a5, a6, aa5 <cvsd_src_up_sampling_mono_process_3+0x141>; addx2	a2, a9, a2 }
 995:	2202e2c7047fe33e                  	{ addi	a3, a3, -4; addi	a2, a2, -2 }
 99d:	040462461c00f6be                  	{ l32r	a11, 78 (ffff8000 <_end+0xffff6c64>); movi	a5, 2 }
 9a5:	20076b461c01e5ce                  	{ l32r	a12, 7c (7fff <_end+0x6c63>); ae_cvt48a32	aed1, a11 }
 9ad:	21074c4500a3e98e                  	{ loopnez	a9, aa5 <cvsd_src_up_sampling_mono_process_3+0x141>; ae_cvt48a32	aed3, a12 }
 9b5:	230760c21485958e                  	{ ae_l16m.xc	aed10, a8, a5; nop }
 9bd:	1a8233543944c58e                  	{ ae_l16m.xc	aed12, a8, a5; nop; ae_mul32.hh	aed10, aed10, aed4 }
 9c5:	1c82265438acb58e                  	{ ae_l16m.xc	aed11, a8, a5; nop; ae_mula32.lh	aed10, aed5, aed12 }
 9cd:	1c8345533565b13e                  	{ nop; nop; ae_mula32.hh	aed10, aed11, aed5 }
 9d5:	0ead257b09440cd0c580813a0c0191af 	{ ae_srai64	aed10, aed10, 5; nop; nop; nop }
 9e5:	0ead057b08e40cd0c48081381c0126af 	{ ae_min64	aed10, aed10, aed3; nop; nop; nop }
 9f5:	0ead057b08e40cd0c48081380c0106af 	{ ae_max64	aed10, aed10, aed1; nop; nop; nop }
 a05:	220700c210e5931e                  	{ ae_s64.i	aed10, a1, 0; nop }
 a0d:	230720c208c5101e                  	{ ae_l32f24.i	aed9, a1, 0; nop }
 a15:	230760c20cc5d21e                  	{ ae_s32.l.i	aed9, a1, 8; nop }
 a1d:	220720c204c5321e                  	{ ae_l32.i	aed8, a1, 8; nop }
 a25:	230740c204c5123e                  	{ ae_s16m.l.iu	aed8, a3, 4; nop }
 a2d:	230760c11c85958e                  	{ ae_l16m.xc	aed7, a8, a5; nop }
 a35:	1a82333c38e5958e                  	{ ae_l16m.xc	aed9, a8, a5; nop; ae_mul32.hh	aed7, aed7, aed5 }
 a3d:	1c82263c38a9858e                  	{ ae_l16m.xc	aed8, a8, a5; nop; ae_mula32.lh	aed7, aed5, aed9 }
 a45:	1c83453b3504b13e                  	{ nop; nop; ae_mula32.hh	aed7, aed8, aed4 }
 a4d:	0ead257b09240cd0c580813a0c01d17f 	{ ae_srai64	aed7, aed7, 5; nop; nop; nop }
 a5d:	0ead057b08e40cd0c48081381c01337f 	{ ae_min64	aed7, aed7, aed3; nop; nop; nop }
 a6d:	0ead057b08e40cd0c48081380c01137f 	{ ae_max64	aed7, aed7, aed1; nop; nop; nop }
 a7d:	1004124118e6931e                  	{ ae_s64.i	aed7, a1, 0; ae_l16m.iu	aed6, a2, 2 }
 a85:	3104d14114c2258e                  	{ ae_s16m.l.xc	aed6, a8, a5; ae_l32f24.i	aed2, a1, 0 }
 a8d:	230760c014c5d31e                  	{ ae_s32.l.i	aed2, a1, 12; nop }
 a95:	220720c004c5331e                  	{ ae_l32.i	aed0, a1, 12; nop }
 a9d:	220740c004c5113e                  	{ ae_s16m.l.i	aed0, a3, 2; nop }

00000aa5 <cvsd_src_up_sampling_mono_process_3+0x141>:
 aa5:	e489                                    	s32i.n	a8, a4, 56
 aa7:	f01d                                    	retw.n

00000aa9 <cvsd_src_up_sampling_mono_process_3+0x145>:
 aa9:	000000                                        ...

00000aac <cvsd_src_dn_sampling_mono_process_1>:
 aac:	006136                                    	entry	a1, 48
 aaf:	f3f640                                    	wur.ae_cbegin0	a4
 ab2:	2a0384440c8404be                  	{ l32i	a11, a4, 68; l32i	a10, a4, 72 }
 aba:	0c04444000801cee                  	{ movi	a14, 12; movi	a13, 4 }
 ac2:	0c0462451c00e19e                  	{ l32r	a9, 88 (12a0 <cvsd_src_down_sampling_coef_1>); movi	a12, 2 }
 aca:	2406f1c51415fb7e                  	{ l32r	a7, 78 (ffff8000 <_end+0xffff6c64>); srli	a6, a5, 1 }
 ad2:	0802e4451405eafe                  	{ l32r	a15, 7c (7fff <_end+0x6c63>); addi	a8, a4, 20 }
 ada:	0ead456b01e173d0c480813900011f0f 	{ ae_cvt48a32	aed1, a15; ae_cvt48a32	aed0, a7; nop; nop }
 aea:	2602e3c5141ff8fe                  	{ l32r	a15, 8c (12c0 <cvsd_src_down_sampling_coef_2>); addi	a7, a3, -2 }
 af2:	f3f780                                    	wur.ae_cend0	a8
 af5:	2902e2c1049fa09e                  	{ ae_l16x2m.i	aed4, a9, 0; addi	a8, a2, -2 }
 afd:	130479c01c82a19e                  	{ ae_l16x2m.i	aed3, a9, 4; ae_l16x2m.i	aed2, a9, 8 }
 b05:	0020f0                                    	nop
 b08:	0902c2c700bf768e                  	{ loopnez	a6, b88 <cvsd_src_dn_sampling_mono_process_1+0xdc>; addi	a9, a2, -4 }
 b10:	220760c20485fdbe                  	{ ae_l16x2m.xc	aed8, a11, a13; nop }
 b18:	2a8230462904adbe                  	{ ae_l16x2m.xc	aed10, a11, a13; nop; ae_mulzaad32.hh.ll	aed8, aed8, aed4 }
 b20:	1c82314629439ebe                  	{ ae_l16x2m.xc	aed9, a11, a14; nop; ae_mulaad32.hh.ll	aed8, aed10, aed3 }
 b28:	1c8351433522b13e                  	{ nop; nop; ae_mulaad32.hh.ll	aed8, aed9, aed2 }
 b30:	0ead257b09440cd0c580813a0c01128f 	{ ae_srai64	aed8, aed8, 6; nop; nop; nop }
 b40:	0eacc57148e292d0c48081380c01248f 	{ ae_min64	aed8, aed8, aed1; ae_l16m.iu	aed9, a9, 4; nop; nop }
 b50:	0ead297b0c450c30c3200139140178bf 	{ ae_s16m.l.xc	aed9, a11, a12; nop; nop; ae_max64	aed8, aed8, aed0 }
 b60:	1204184200e7831e                  	{ ae_s64.i	aed8, a1, 0; ae_l16m.iu	aed7, a8, 4 }
 b68:	3104d1411cc62cbe                  	{ ae_s16m.l.xc	aed7, a11, a12; ae_l32f24.i	aed6, a1, 0 }
 b70:	230760c114c5d21e                  	{ ae_s32.l.i	aed6, a1, 8; nop }
 b78:	220720c10cc5321e                  	{ ae_l32.i	aed5, a1, 8; nop }
 b80:	230740c10cc5117e                  	{ ae_s16m.l.iu	aed5, a7, 2; nop }

00000b88 <cvsd_src_dn_sampling_mono_process_1+0xdc>:
 b88:	1164b2                                    	s32i	a11, a4, 68
 b8b:	0802e4471466f37e                  	{ addi	a7, a3, -2; addi	a9, a4, 24 }
 b93:	11047fc10483b0fe                  	{ ae_l16x2m.i	aed4, a15, 0; ae_l16x2m.i	aed3, a15, 4 }
 b9b:	0802e4441c0be86e                  	{ l32r	a6, 7c (7fff <_end+0x6c63>); addi	a8, a4, 44 }
 ba3:	210766401481a2fe                  	{ ae_l16x2m.i	aed2, a15, 8; ae_cvt48a32	aed1, a6 }
 bab:	3406f2c41c17f36e                  	{ l32r	a6, 78 (ffff8000 <_end+0xffff6c64>); srli	a15, a5, 2 }
 bb3:	f3f780                                    	wur.ae_cend0	a8
 bb6:	f3f690                                    	wur.ae_cbegin0	a9
 bb9:	2802e2c7047fe29e                  	{ addi	a9, a2, -4; addi	a8, a2, -2 }
 bc1:	0020f0                                    	nop
 bc4:	f03d                                    	nop.n
 bc6:	f03d                                    	nop.n
 bc8:	2107464700a06f8e                  	{ loopnez	a15, c48 <cvsd_src_dn_sampling_mono_process_1+0x19c>; ae_cvt48a32	aed0, a6 }
 bd0:	220760c30485fdae                  	{ ae_l16x2m.xc	aed12, a10, a13; nop }
 bd8:	2a8230662984edae                  	{ ae_l16x2m.xc	aed14, a10, a13; nop; ae_mulzaad32.hh.ll	aed12, aed12, aed4 }
 be0:	1c82316629c3deae                  	{ ae_l16x2m.xc	aed13, a10, a14; nop; ae_mulaad32.hh.ll	aed12, aed14, aed3 }
 be8:	1c83516335a2b13e                  	{ nop; nop; ae_mulaad32.hh.ll	aed12, aed13, aed2 }
 bf0:	0ead257b09640cd0c580813a0c0112cf 	{ ae_srai64	aed12, aed12, 6; nop; nop; nop }
 c00:	0eacc57168e292d0c48081380c0134cf 	{ ae_min64	aed12, aed12, aed1; ae_l16m.iu	aed13, a9, 4; nop; nop }
 c10:	0ead2d7b0c450c30c330013914017caf 	{ ae_s16m.l.xc	aed13, a10, a12; nop; nop; ae_max64	aed12, aed12, aed0 }
 c20:	1204184300eb831e                  	{ ae_s64.i	aed12, a1, 0; ae_l16m.iu	aed11, a8, 4 }
 c28:	3104d1421cca2cae                  	{ ae_s16m.l.xc	aed11, a10, a12; ae_l32f24.i	aed10, a1, 0 }
 c30:	230760c214c5d31e                  	{ ae_s32.l.i	aed10, a1, 12; nop }
 c38:	220720c20cc5331e                  	{ ae_l32.i	aed9, a1, 12; nop }
 c40:	230740c20cc5117e                  	{ ae_s16m.l.iu	aed9, a7, 2; nop }
 c48:	1264a2                                    	s32i	a10, a4, 72
 c4b:	f01d                                    	retw.n

00000c4d <cvsd_src_dn_sampling_mono_process_1+0x1a1>:
 c4d:	000000                                        ...

00000c50 <cvsd_src_dn_sampling_mono_process_2>:
 c50:	006136                                    	entry	a1, 48
 c53:	2202c3c0009f12fe                  	{ movi	a15, 2; addi	a3, a3, -2 }
 c5b:	080464440400fdae                  	{ l32r	a10, 90 (12e0 <cvsd_src_down_sampling_coef_3>); movi	a9, 4 }
 c63:	2406f1c40415f5be                  	{ l32r	a11, 78 (ffff8000 <_end+0xffff6c64>); srli	a6, a5, 1 }
 c6b:	0c02e4440411f4ce                  	{ l32r	a12, 7c (7fff <_end+0x6c63>); addi	a13, a4, 68 }
 c73:	2602e2c7047ff28e                  	{ addi	a8, a2, -4; addi	a7, a2, -2 }
 c7b:	3203e444046424ee                  	{ addi	a14, a4, 48; l32i	a2, a4, 76 }
 c83:	f3f6e0                                    	wur.ae_cbegin0	a14
 c86:	f3f7d0                                    	wur.ae_cend0	a13
 c89:	0ead456b19e1b3d0c480813900012c0f 	{ ae_cvt48a32	aed2, a12; ae_cvt48a32	aed3, a11; nop; nop }
 c99:	11047ac20487b0ae                  	{ ae_l16x2m.i	aed8, a10, 0; ae_l16x2m.i	aed7, a10, 4 }
 ca1:	13047ac11485b2ae                  	{ ae_l16x2m.i	aed6, a10, 8; ae_l16x2m.i	aed5, a10, 12 }
 ca9:	15045ac300a4868e                  	{ loopnez	a6, d39 <cvsd_src_dn_sampling_mono_process_2+0xe9>; ae_l16x2m.i	aed4, a10, 16 }
 cb1:	220760c21485f92e                  	{ ae_l16x2m.xc	aed10, a2, a9; nop }
 cb9:	2a8230562948e92e                  	{ ae_l16x2m.xc	aed14, a2, a9; nop; ae_mulzaad32.hh.ll	aed10, aed10, aed8 }
 cc1:	1c82315629c7d92e                  	{ ae_l16x2m.xc	aed13, a2, a9; nop; ae_mulaad32.hh.ll	aed10, aed14, aed7 }
 cc9:	1c82315629a6c92e                  	{ ae_l16x2m.xc	aed12, a2, a9; nop; ae_mulaad32.hh.ll	aed10, aed13, aed6 }
 cd1:	1c8231562985b92e                  	{ ae_l16x2m.xc	aed11, a2, a9; nop; ae_mulaad32.hh.ll	aed10, aed12, aed5 }
 cd9:	1c8351533564b13e                  	{ nop; nop; ae_mulaad32.hh.ll	aed10, aed11, aed4 }
 ce1:	0ead257b09440cd0c580813a0c01a3af 	{ ae_srai64	aed10, aed10, 11; nop; nop; nop }
 cf1:	0eacc57158e282d0c4808138140126af 	{ ae_min64	aed10, aed10, aed2; ae_l16m.iu	aed11, a8, 4; nop; nop }
 d01:	0ead297b0c550c30c32861391401fb2f 	{ ae_s16m.l.xc	aed11, a2, a15; nop; nop; ae_max64	aed10, aed10, aed3 }
 d11:	1204174210e9831e                  	{ ae_s64.i	aed10, a1, 0; ae_l16m.iu	aed9, a7, 4 }
 d19:	3104d1420cc12f2e                  	{ ae_s16m.l.xc	aed9, a2, a15; ae_l32f24.i	aed1, a1, 0 }
 d21:	230760c00cc5d21e                  	{ ae_s32.l.i	aed1, a1, 8; nop }
 d29:	220720c004c5321e                  	{ ae_l32.i	aed0, a1, 8; nop }
 d31:	230740c004c5113e                  	{ ae_s16m.l.iu	aed0, a3, 2; nop }

00000d39 <cvsd_src_dn_sampling_mono_process_2+0xe9>:
 d39:	136422                                    	s32i	a2, a4, 76
 d3c:	f01d                                    	retw.n

00000d3e <cvsd_src_dn_sampling_mono_process_2+0xee>:
	...

00000d40 <cvsd_src_up_sampling_init>:
 d40:	004136                                    	entry	a1, 32
 d43:	fcd481                                    	l32r	a8, 94 (e40 <xt_hifi5_cvsd_memset>)
 d46:	03ad                                    	mov.n	a10, a3
 d48:	2c044040008000be                  	{ movi	a11, 0; movi	a12, 32 }
 d50:	0008e0                                    	callx8	a8
 d53:	0b02a343008a033e                  	{ s32i	a3, a3, 48; addi	a10, a3, 40 }
 d5b:	0902a343108813ae                  	{ s32i	a10, a3, 56; addi	a9, a3, 32 }
 d63:	d399                                    	s32i.n	a9, a3, 52
 d65:	030420400080023e                  	{ s32i	a3, a2, 0; movi	a2, 0 }
 d6d:	f01d                                    	retw.n

00000d6f <cvsd_src_up_sampling_init+0x2f>:
	...

00000d70 <cvsd_src_dn_sampling_init>:
 d70:	004136                                    	entry	a1, 32
 d73:	fcc881                                    	l32r	a8, 94 (e40 <xt_hifi5_cvsd_memset>)
 d76:	03ad                                    	mov.n	a10, a3
 d78:	2c044840008000be                  	{ movi	a11, 0; movi	a12, 40 }
 d80:	0008e0                                    	callx8	a8
 d83:	0b02a344088c033e                  	{ s32i	a3, a3, 68; addi	a10, a3, 48 }
 d8b:	0902a344188613ae                  	{ s32i	a10, a3, 76; addi	a9, a3, 24 }
 d93:	126392                                    	s32i	a9, a3, 72
 d96:	030420400080023e                  	{ s32i	a3, a2, 0; movi	a2, 0 }
 d9e:	f01d                                    	retw.n

00000da0 <cvsd_src_up_sampling_process>:
 da0:	004136                                    	entry	a1, 32
 da3:	fcbd81                                    	l32r	a8, 98 (4b0 <cvsd_src_up_sampling_mono_process_1>)
 da6:	3406b44610eae33e                  	{ or	a10, a3, a3; or	a11, a4, a4 }
 dae:	0306b2430cab416e                  	{ srli	a13, a6, 1; or	a12, a2, a2 }
 db6:	0008e0                                    	callx8	a8
 db9:	fcb881                                    	l32r	a8, 9c (818 <cvsd_src_up_sampling_mono_process_2>)
 dbc:	3406b54610eaf44e                  	{ or	a10, a4, a4; or	a11, a5, a5 }
 dc4:	1606b64700ebe22e                  	{ or	a12, a2, a2; or	a13, a6, a6 }
 dcc:	0008e0                                    	callx8	a8
 dcf:	fcb481                                    	l32r	a8, a0 (964 <cvsd_src_up_sampling_mono_process_3>)
 dd2:	0206b24618ebe55e                  	{ or	a11, a5, a5; or	a12, a2, a2 }
 dda:	2506b54708aa16fe                  	{ slli	a13, a6, 1; or	a10, a5, a5 }
 de2:	0008e0                                    	callx8	a8
 de5:	020c                                    	movi.n	a2, 0
 de7:	f01d                                    	retw.n

00000de9 <cvsd_src_up_sampling_process+0x49>:
 de9:	000000                                        ...

00000dec <cvsd_src_dn_sampling_process>:
 dec:	004136                                    	entry	a1, 32
 def:	fcad81                                    	l32r	a8, a4 (aac <cvsd_src_dn_sampling_mono_process_1>)
 df2:	3206b34610eaf33e                  	{ or	a10, a3, a3; or	a11, a3, a3 }
 dfa:	0306b2430cab415e                  	{ srli	a13, a5, 1; or	a12, a2, a2 }
 e02:	0008e0                                    	callx8	a8
 e05:	fca881                                    	l32r	a8, a8 (c50 <cvsd_src_dn_sampling_mono_process_2>)
 e08:	3406b44700eae22e                  	{ or	a12, a2, a2; or	a11, a4, a4 }
 e10:	2306b3430caa535e                  	{ srli	a13, a5, 3; or	a10, a3, a3 }
 e18:	0008e0                                    	callx8	a8
 e1b:	020c                                    	movi.n	a2, 0
 e1d:	f01d                                    	retw.n

00000e1f <cvsd_src_dn_sampling_process+0x33>:
	...

00000e20 <__do_global_ctors_aux>:
 e20:	004136                                    	entry	a1, 32
 e23:	fca281                                    	l32r	a8, ac (1154 <_DYNAMIC+0x280>)
 e26:	7f2882                                    	l32i	a8, a8, 0x1fc
 e29:	fca121                                    	l32r	a2, b0 (1350 <__CTOR_LIST___103>)
 e2c:	0c0826                                    	beqi	a8, -1, e3c <__do_global_ctors_aux+0x1c>
 e2f:	02a8                                    	l32i.n	a10, a2, 0

00000e31 <__do_global_ctors_aux+0x11>:
 e31:	000ae0                                    	callx8	a10
 e34:	fcc222                                    	addi	a2, a2, -4
 e37:	02a8                                    	l32i.n	a10, a2, 0
 e39:	f40a66                                    	bnei	a10, -1, e31 <__do_global_ctors_aux+0x11>
 e3c:	f01d                                    	retw.n

00000e3e <__do_global_ctors_aux+0x1e>:
	...

00000e40 <xt_hifi5_cvsd_memset>:
 e40:	004136                                    	entry	a1, 32
 e43:	23069221000962ee                  	{ bnez.w15	a2, e55 <xt_hifi5_cvsd_memset+0x15>; or	a6, a2, a2 }
 e4b:	020c                                    	movi.n	a2, 0
 e4d:	f01d                                    	retw.n

00000e4f <xt_hifi5_cvsd_memset+0xf>:
 e4f:	00000000 94760000                                ......

00000e55 <xt_hifi5_cvsd_memset+0x15>:
 e55:	079476                                    	loopnez	a4, e60 <xt_hifi5_cvsd_memset+0x20>
 e58:	2602a6400080063e                  	{ s16i	a3, a6, 0; addi	a6, a6, 2 }

00000e60 <xt_hifi5_cvsd_memset+0x20>:
 e60:	f01d                                    	retw.n

00000e62 <xt_hifi5_cvsd_memset+0x22>:
	...

00000e64 <xt_hifi5_cvsd_memcpy>:
 e64:	004136                                    	entry	a1, 32
 e67:	02bc                                    	beqz.n	a2, e9b <xt_hifi5_cvsd_memcpy+0x37>
 e69:	e3ac                                    	beqz.n	a3, e9b <xt_hifi5_cvsd_memcpy+0x37>
 e6b:	230692010809832e                  	{ bgeu.w15	a3, a2, e81 <xt_hifi5_cvsd_memcpy+0x1d>; or	a6, a2, a2 }
 e73:	907430                                    	addx2	a7, a4, a3
 e76:	07b277                                    	bgeu	a2, a7, e81 <xt_hifi5_cvsd_memcpy+0x1d>
 e79:	3206844000740c2e                  	{ j	ea9 <xt_hifi5_cvsd_memcpy+0x45>; addx2	a3, a4, a2 }

00000e81 <xt_hifi5_cvsd_memcpy+0x1d>:
 e81:	0003c6                                    	j	e94 <xt_hifi5_cvsd_memcpy+0x30>

00000e84 <xt_hifi5_cvsd_memcpy+0x20>:
 e84:	040363401460133e                  	{ addi	a3, a3, 2; l16si	a5, a3, 0 }
 e8c:	2602a6400080065e                  	{ s16i	a5, a6, 0; addi	a6, a6, 2 }
 e94:	440b                                    	addi.n	a4, a4, -1
 e96:	ea0466                                    	bnei	a4, -1, e84 <xt_hifi5_cvsd_memcpy+0x20>
 e99:	f01d                                    	retw.n

00000e9b <xt_hifi5_cvsd_memcpy+0x37>:
 e9b:	020c                                    	movi.n	a2, 0
 e9d:	f01d                                    	retw.n

00000e9f <xt_hifi5_cvsd_memcpy+0x3b>:
 e9f:	278d                                    	ae_l16si.n	a8, a7, 0
 ea1:	3606b6400088068e                  	{ s16i	a8, a6, 0; or	a3, a6, a6 }
 ea9:	2602e7c71c7ff44e                  	{ addi	a4, a4, -1; addi	a7, a7, -2 }
 eb1:	2702e3b807ff446e                  	{ bnei.w15	a4, -1, e9f <xt_hifi5_cvsd_memcpy+0x3b>; addi	a6, a3, -2 }
 eb9:	fff706                                    	j	e99 <xt_hifi5_cvsd_memcpy+0x35>

00000ebc <_fini>:
 ebc:	008136                                    	entry	a1, 64
 ebf:	fc7d81                                    	l32r	a8, b4 (d0 <__do_global_dtors_aux>)
 ec2:	f03d                                    	nop.n
 ec4:	0008e0                                    	callx8	a8

00000ec7 <_fini+0xb>:
 ec7:	f01d                                    	retw.n
