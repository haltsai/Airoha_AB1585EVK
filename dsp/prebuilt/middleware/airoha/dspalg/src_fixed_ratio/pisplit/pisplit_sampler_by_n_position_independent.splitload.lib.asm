
Build/lib/pisplit_sampler_by_n_position_independent.splitload.lib:     file format elf32-xtensa-le


Disassembly of section .text:

00000000 <_init-0x70>:
   0:	000000c0 000006a0 00000a8c 00000000     ................
  10:	00000a54 00000000 00000a74 00000ab0     T.......t.......
  20:	000009cc 00000120 000009ec 00000128     .... .......(...
  30:	00000a08 0000061c 00000a28 00000660     ........(...`...
  40:	00000aa0 21082401 000006d0 000006f4     .....$.!........
  50:	000003a2 aaaaaaab 55555556 00000608     ........VUUU....
  60:	00000168 000003b4 00000a5c 00000088     h.......\.......

00000070 <_init>:
  70:	008136                                    	entry	a1, 64
  73:	ffe381                                    	l32r	a8, 0 (c0 <frame_dummy>)
  76:	f03d                                    	nop.n
  78:	0008e0                                    	callx8	a8
  7b:	ffe281                                    	l32r	a8, 4 (6a0 <__do_global_ctors_aux>)
  7e:	f03d                                    	nop.n
  80:	0008e0                                    	callx8	a8
  83:	f01d                                    	retw.n

00000085 <_init+0x15>:
  85:	000000                                        ...

00000088 <__do_global_dtors_aux>:
  88:	004136                                    	entry	a1, 32
  8b:	ffdf21                                    	l32r	a2, 8 (a8c <__do_global_dtors_aux.completed>)
  8e:	000232                                    	l8ui	a3, a2, 0
  91:	63ec                                    	bnez.n	a3, bb <__do_global_dtors_aux+0x33>

00000093 <__do_global_dtors_aux+0xb>:
  93:	1248                                    	l32i.n	a4, a2, 4
  95:	0438                                    	l32i.n	a3, a4, 0
  97:	04028424000163ae                  	{ beqz.w15	a3, a8 <__do_global_dtors_aux+0x20>; addi	a4, a4, 4 }
  9f:	1249                                    	s32i.n	a4, a2, 4
  a1:	0003e0                                    	callx8	a3
  a4:	fffac6                                    	j	93 <__do_global_dtors_aux+0xb>

000000a7 <__do_global_dtors_aux+0x1f>:
	...

000000a8 <__do_global_dtors_aux+0x20>:
  a8:	ffd931                                    	l32r	a3, c (0 <_text_start>)
  ab:	ffd9a1                                    	l32r	a10, 10 (a54 <__FRAME_END__>)
  ae:	438c                                    	beqz.n	a3, b6 <__do_global_dtors_aux+0x2e>
  b0:	ffd781                                    	l32r	a8, c (0 <_text_start>)
  b3:	0008e0                                    	callx8	a8
  b6:	130c                                    	movi.n	a3, 1
  b8:	004232                                    	s8i	a3, a2, 0
  bb:	f01d                                    	retw.n

000000bd <__do_global_dtors_aux+0x35>:
  bd:	000000                                        ...

000000c0 <frame_dummy>:
  c0:	004136                                    	entry	a1, 32
  c3:	ffd421                                    	l32r	a2, 14 (0 <_text_start>)
  c6:	ffd2a1                                    	l32r	a10, 10 (a54 <__FRAME_END__>)
  c9:	ffd3b1                                    	l32r	a11, 18 (a74 <frame_dummy.object>)
  cc:	428c                                    	beqz.n	a2, d4 <frame_dummy+0x14>
  ce:	ffd181                                    	l32r	a8, 14 (0 <_text_start>)
  d1:	0008e0                                    	callx8	a8

000000d4 <frame_dummy+0x14>:
  d4:	f01d                                    	retw.n

000000d6 <frame_dummy+0x16>:
	...

000000e0 <_start>:
  e0:	004136                                    	entry	a1, 32
  e3:	ffcfa1                                    	l32r	a10, 20 (9cc <.got+0x3c>)
  e6:	ffcd31                                    	l32r	a3, 1c (ab0 <printf_ptr>)
  e9:	0288                                    	l32i.n	a8, a2, 0
  eb:	ffceb1                                    	l32r	a11, 24 (120 <get_sampler_by_n_version>)
  ee:	0389                                    	s32i.n	a8, a3, 0
  f0:	0008e0                                    	callx8	a8
  f3:	0388                                    	l32i.n	a8, a3, 0
  f5:	ffcca1                                    	l32r	a10, 28 (9ec <.got+0x5c>)
  f8:	ffcdb1                                    	l32r	a11, 2c (128 <updn_sampling_Init>)
  fb:	0008e0                                    	callx8	a8
  fe:	0388                                    	l32i.n	a8, a3, 0
 100:	ffcca1                                    	l32r	a10, 30 (a08 <.got+0x78>)
 103:	ffccb1                                    	l32r	a11, 34 (61c <updn_sampling_16bit_Proc>)
 106:	0008e0                                    	callx8	a8
 109:	0388                                    	l32i.n	a8, a3, 0
 10b:	ffcba1                                    	l32r	a10, 38 (a28 <.got+0x98>)
 10e:	ffcbb1                                    	l32r	a11, 3c (660 <updn_sampling_32bit_Proc>)
 111:	0008e0                                    	callx8	a8
 114:	ffcb21                                    	l32r	a2, 40 (aa0 <export_parameter_array>)
 117:	f01d                                    	retw.n

00000119 <_start+0x39>:
 119:	00000000                                 .......

00000120 <get_sampler_by_n_version>:
 120:	004136                                    	entry	a1, 32
 123:	ffc821                                    	l32r	a2, 44 (21082401 <_end+0x2108194d>)
 126:	f01d                                    	retw.n

00000128 <updn_sampling_Init>:
 128:	004136                                    	entry	a1, 32
 12b:	0b02e2470410e74e                  	{ l32r	a4, 48 (6d0 <xt_hifi5_sampler_by_n_memset>); addi	a10, a2, 64 }
 133:	0c044440008000be                  	{ movi	a11, 0; movi	a12, 4 }
 13b:	0020f0                                    	nop
 13e:	f03d                                    	nop.n
 140:	0004e0                                    	callx8	a4
 143:	0a046042046052ae                  	{ addi	a10, a2, 72; movi	a11, 0 }
 14b:	cc0c                                    	movi.n	a12, 12
 14d:	0004e0                                    	callx8	a4
 150:	2d0460461c00ef4e                  	{ l32r	a4, 4c (6f4 <xt_hifi5_sampler_by_n_memcpy>); movi	a12, 32 }
 158:	3206b34610eaf22e                  	{ or	a10, a2, a2; or	a11, a3, a3 }
 160:	0004e0                                    	callx8	a4
 163:	f01d                                    	retw.n

00000165 <updn_sampling_Init+0x3d>:
 165:	000000                                        ...

00000168 <updn_sampling_by2_Proc_V2>:
 168:	004136                                    	entry	a1, 32
 16b:	080283a2181fc76e                  	{ bnei.w15	a7, 16, 187 <updn_sampling_by2_Proc_V2+0x1f>; addi	a8, a3, -4 }
 173:	2402e4c7147fe33e                  	{ addi	a3, a3, -2; addi	a4, a4, -2 }
 17b:	0002c6                                    	j	18a <updn_sampling_by2_Proc_V2+0x22>

0000017e <updn_sampling_by2_Proc_V2+0x16>:
	...

00000187 <updn_sampling_by2_Proc_V2+0x1f>:
 187:	fcc492                                    	addi	a9, a4, -4
 18a:	0a0c                                    	movi.n	a10, 0
 18c:	3e0286c21cff5a6e                  	{ saltu	a11, a10, a6; addi	a15, a6, -1 }
 194:	0d04234318e07b5e                  	{ ssl	a11; movi	a13, 3 }
 19c:	a1b500                                    	sll	a11, a5
 19f:	11cb00                                    	slli	a12, a11, 16
 1a2:	0c02e2a00c3fecce                  	{ blti.w15	a12, 1, 3a2 <updn_sampling_by2_Proc_V2+0x23a>; addi	a12, a2, -4 }
 1aa:	0202e2400490c1ce                  	{ ae_l16x2m.iu	aed0, a12, 4; addi	a2, a2, 64 }
 1b2:	000060c00c87c1ce                  	{ ae_l16x2m.iu	aed1, a12, 4; ae_sel16i	aed7, aed0, aed0, 1 }
 1ba:	020060c01488d1ce                  	{ ae_l16x2m.iu	aed2, a12, 4; ae_sel16i	aed8, aed1, aed1, 1 }
 1c2:	040061c01c89c1ce                  	{ ae_l16x2m.iu	aed3, a12, 4; ae_sel16i	aed9, aed2, aed2, 1 }
 1ca:	060061c1048ad1ce                  	{ ae_l16x2m.iu	aed4, a12, 4; ae_sel16i	aed10, aed3, aed3, 1 }
 1d2:	080062c10c8bc1ce                  	{ ae_l16x2m.iu	aed5, a12, 4; ae_sel16i	aed11, aed4, aed4, 1 }
 1da:	0a0062c1148cd1ce                  	{ ae_l16x2m.iu	aed6, a12, 4; ae_sel16i	aed12, aed5, aed5, 1 }
 1e2:	0c0063c3148dc1ce                  	{ ae_l16x2m.iu	aed14, a12, 4; ae_sel16i	aed13, aed6, aed6, 1 }
 1ea:	8bed                                    	ae_sext16	a14, a11
 1ec:	0c044140008002be                  	{ movi	a11, 2; movi	a12, 1 }
 1f4:	1f06ba4618f3dd6e                  	{ movnez	a11, a13, a6; saltu	a13, a10, a15 }
 1fc:	f03d                                    	nop.n
 1fe:	1d0027c318ee647e                  	{ nop; ae_sel16i	aed14, aed14, aed14, 1 }
 206:	178e76                                    	loop	a14, 221 <updn_sampling_by2_Proc_V2+0xb9>
 209:	0301e0                                    	rsr.lend	a14
 20c:	1300e0                                    	wsr.lbeg	a14
 20f:	ff90e1                                    	l32r	a14, 50 (3a2 <updn_sampling_by2_Proc_V2+0x23a>)
 212:	0020f0                                    	nop
 215:	1301e0                                    	wsr.lend	a14
 218:	002000                                    	isync
 21b:	0302e0                                    	rsr.lcount	a14
 21e:	01cee2                                    	addi	a14, a14, 1
 221:	010542440cb04c0e                  	{ ae_movi	aed17, 0; ae_l32m.i	aed16, a2, 0 }
 229:	000502261c0f776e                  	{ bnei.w15	a7, 16, 25a <updn_sampling_by2_Proc_V2+0xf2>; ae_l32m.i	aed15, a2, 4 }
 231:	0d0700240811266e                  	{ bnei.w15	a6, 1, 23c <updn_sampling_by2_Proc_V2+0xd4>; ae_movi	aed17, 0 }
 239:	07ea07                                    	bbsi	a10, 0, 244 <updn_sampling_by2_Proc_V2+0xdc>
 23c:	230760c40c85713e                  	{ ae_l16m.iu	aed17, a3, 2; nop }
 244:	0e6d157b08f40c24454421380001002f 	{ j	268 <updn_sampling_by2_Proc_V2+0x100>; nop; nop; ae_cvtq56p32s.l	aed17, aed17 }

00000254 <updn_sampling_by2_Proc_V2+0xec>:
 254:	00000000                                 ......

0000025a <updn_sampling_by2_Proc_V2+0xf2>:
 25a:	021666                                    	bnei	a6, 1, 260 <updn_sampling_by2_Proc_V2+0xf8>
 25d:	07ea07                                    	bbsi	a10, 0, 268 <updn_sampling_by2_Proc_V2+0x100>
 260:	230720c408c5718e                  	{ ae_l32m.iu	aed17, a8, 4; nop }
 268:	0e6ce16075390a544782213b0001012f 	{ ae_s32m.i	aed16, a2, 4; extui	a14, a10, 0, 1; nop; ae_srai64	aed17, aed17, 2 }
 278:	0e9ce699dd2322d0c5808e930229402f 	{ ae_s32m.i	aed17, a2, 0; ae_l32m.i	aed27, a2, 8; ae_mulf48q32sp16s.l	aed18, aed17, aed9; nop }
 288:	0e9ce419fd2327d0c4808e921a070f2f 	{ ae_l32m.i	aed28, a2, 12; ae_l32m.i	aed31, a2, 28; ae_mulaf48q32sp16s.l	aed18, aed16, aed7; nop }
 298:	0e9d6408f521ecd0c4808e921a07d02f 	{ ae_l32m.i	aed19, a2, 16; saltu	a14, a14, a12; ae_mulaf48q32sp16s.l	aed18, aed16, aed7; nop }
 2a8:	1e8294932de0452e                  	{ ae_l32m.i	aed20, a2, 20; nop; ae_mulaf48q32sp16s.l	aed18, aed15, aed0 }
 2b0:	1e8294932f68e62e                  	{ ae_l32m.i	aed30, a2, 24; nop; ae_mulaf48q32sp16s.l	aed18, aed27, aed8 }
 2b8:	1e82d4972768352e                  	{ ae_s32m.i	aed19, a2, 20; nop; ae_mulaf48q32sp16s.l	aed18, aed27, aed8 }
 2c0:	1e82d4972781e72e                  	{ ae_s32m.i	aed30, a2, 28; nop; ae_mulaf48q32sp16s.l	aed18, aed28, aed1 }
 2c8:	0ead71667537edd0c5f4613b0001cb2f 	{ ae_s32m.i	aed27, a2, 12; and	a14, a14, a13; nop; ae_roundsq32f48sym	aed29, aed18 }
 2d8:	2282d39727a2d22e                  	{ ae_s32m.i	aed29, a2, 8; nop; ae_mulf48q32sp16s.l	aed18, aed29, aed2 }
 2e0:	1e835493376ab13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed18, aed27, aed10 }
 2e8:	1e835493376ab13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed18, aed27, aed10 }
 2f0:	1e8354933783b13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed18, aed28, aed3 }
 2f8:	1e835493366bb13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed18, aed19, aed11 }
 300:	1e835493366bb13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed18, aed19, aed11 }
 308:	1e8354933684b13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed18, aed20, aed4 }
 310:	0ead317b0d540cd0c54861380c01015f 	{ nop; nop; nop; ae_roundsq32f48sym	aed18, aed18 }
 320:	2282d37f264e242e                  	{ ae_s32m.i	aed18, a2, 16; nop; ae_mulf48q32sp16s.l	aed15, aed18, aed14 }
 328:	1e83547b366cb13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed15, aed19, aed12 }
 330:	1e83547b366cb13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed15, aed19, aed12 }
 338:	1e83547b3685b13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed15, aed20, aed5 }
 340:	1e83547b37cdb13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed15, aed30, aed13 }
 348:	1e83547b37cdb13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed15, aed30, aed13 }
 350:	1e83547b37e6b13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed15, aed31, aed6 }
 358:	0ead2d7b0d5c0cd0c54061380c01015f 	{ nop; nop; nop; ae_roundsq32f48sym	aed16, aed15 }
 368:	220760c400c5b62e                  	{ ae_s32m.i	aed16, a2, 24; nop }
 370:	ceec                                    	bnez.n	a14, 3a0 <updn_sampling_by2_Proc_V2+0x238>
 372:	220720c014a5dbfe                  	{ ae_slaasq56s	aed15, aed15, a11; nop }
 37a:	1ab766                                    	bnei	a7, 16, 398 <updn_sampling_by2_Proc_V2+0x230>
 37d:	0ead2d7b0d5c0cf0c33de1380c01015f 	{ nop; nop; nop; ae_roundsp16q48x2sym	aed15, aed15, aed15 }
 38d:	230740c31cc5114e                  	{ ae_s16m.l.iu	aed15, a4, 2; nop }
 395:	0001c6                                    	j	3a0 <updn_sampling_by2_Proc_V2+0x238>

00000398 <updn_sampling_by2_Proc_V2+0x230>:
 398:	230760c318c5b19e                  	{ ae_s32m.iu	aed15, a9, 4; nop }
 3a0:	aa1b                                    	addi.n	a10, a10, 1

000003a2 <updn_sampling_by2_Proc_V2+0x23a>:
 3a2:	3506b1c410b415fe                  	{ slli	a2, a5, 1; srli	a3, a5, 1 }
 3aa:	933260                                    	movnez	a3, a2, a6
 3ad:	832d                                    	ae_sext16	a2, a3
 3af:	f01d                                    	retw.n

000003b1 <updn_sampling_by2_Proc_V2+0x249>:
 3b1:	000000                                        ...

000003b4 <updn_sampling_by3_Proc>:
 3b4:	004136                                    	entry	a1, 32
 3b7:	090283a2181f876e                  	{ bnei.w15	a7, 16, 3cd <updn_sampling_by3_Proc+0x19>; addi	a8, a3, -4 }
 3bf:	2402e4c7147fe33e                  	{ addi	a3, a3, -2; addi	a4, a4, -2 }
 3c7:	000146                                    	j	3d0 <updn_sampling_by3_Proc+0x1c>

000003ca <updn_sampling_by3_Proc+0x16>:
 3ca:	000000                                        ...

000003cd <updn_sampling_by3_Proc+0x19>:
 3cd:	fcc492                                    	addi	a9, a4, -4
 3d0:	3406954614ea355e                  	{ addx2	a10, a5, a5; or	a11, a5, a5 }
 3d8:	0f02a24618e7ca6e                  	{ movnez	a11, a10, a6; addi	a14, a2, 28 }
 3e0:	0d04234700a01b0e                  	{ slli	a12, a11, 16; movi	a13, 3 }
 3e8:	0c0402200840ecce                  	{ blti.w15	a12, 1, 608 <updn_sampling_by3_Proc+0x254>; movi	a12, 2 }
 3f0:	0606fd400487c1ee                  	{ ae_l16x2m.iu	aed0, a14, 4; movnez	a12, a13, a6 }
 3f8:	0202e2400c90c1ee                  	{ ae_l16x2m.iu	aed1, a14, 4; addi	a2, a2, 64 }
 400:	0c0460401480d1ee                  	{ ae_l16x2m.iu	aed2, a14, 4; movi	a13, 0 }
 408:	040061c01c89c1ee                  	{ ae_l16x2m.iu	aed3, a14, 4; ae_sel16i	aed9, aed2, aed2, 1 }
 410:	060061c1048ad1ee                  	{ ae_l16x2m.iu	aed4, a14, 4; ae_sel16i	aed10, aed3, aed3, 1 }
 418:	080062c10c8bc1ee                  	{ ae_l16x2m.iu	aed5, a14, 4; ae_sel16i	aed11, aed4, aed4, 1 }
 420:	0a0062c2048cd1ee                  	{ ae_l16x2m.iu	aed8, a14, 4; ae_sel16i	aed12, aed5, aed5, 1 }
 428:	8bfd                                    	ae_sext16	a15, a11
 42a:	010060c40406eabe                  	{ l32r	a11, 54 (aaaaaaab <_end+0xaaaa9ff7>); ae_sel16i	aed6, aed0, aed0, 1 }
 432:	100064c3148dc1ee                  	{ ae_l16x2m.iu	aed14, a14, 4; ae_sel16i	aed13, aed8, aed8, 1 }
 43a:	0e54f3b005480e01c5043d700dce015f 	{ nop; movi	a14, 0; ae_sel16i.n	aed14, aed14, aed14, 1; ae_sel16i	aed7, aed1, aed1, 1 }
 44a:	178f76                                    	loop	a15, 465 <updn_sampling_by3_Proc+0xb1>
 44d:	0301f0                                    	rsr.lend	a15
 450:	1300f0                                    	wsr.lbeg	a15
 453:	ff02f1                                    	l32r	a15, 5c (608 <updn_sampling_by3_Proc+0x254>)
 456:	0020f0                                    	nop
 459:	1301f0                                    	wsr.lend	a15
 45c:	002000                                    	isync
 45f:	0302f0                                    	rsr.lcount	a15
 462:	01cff2                                    	addi	a15, a15, 1
 465:	a2feb0                                    	muluh	a15, a14, a11
 468:	010542440cb04c0e                  	{ ae_movi	aed17, 0; ae_l32m.i	aed16, a2, 0 }
 470:	010522431caf51fe                  	{ srli	a15, a15, 1; ae_l32m.i	aed15, a2, 4 }
 478:	90fff0                                    	addx2	a15, a15, a15
 47b:	3e068d221c13776e                  	{ bnei.w15	a7, 16, 4ab <updn_sampling_by3_Proc+0xf7>; add	a15, a13, a15 }
 483:	0d0700200811266e                  	{ bnei.w15	a6, 1, 48d <updn_sampling_by3_Proc+0xd9>; ae_movi	aed17, 0 }
 48b:	6fcc                                    	bnez.n	a15, 495 <updn_sampling_by3_Proc+0xe1>
 48d:	230760c40c85713e                  	{ ae_l16m.iu	aed17, a3, 2; nop }
 495:	0e6d157b08f40c24454421381801031f 	{ j	4b8 <updn_sampling_by3_Proc+0x104>; nop; nop; ae_cvtq56p32s.l	aed17, aed17 }

000004a5 <updn_sampling_by3_Proc+0xf1>:
 4a5:	00000000                                 ......

000004ab <updn_sampling_by3_Proc+0xf7>:
 4ab:	011666                                    	bnei	a6, 1, 4b0 <updn_sampling_by3_Proc+0xfc>
 4ae:	6fcc                                    	bnez.n	a15, 4b8 <updn_sampling_by3_Proc+0x104>
 4b0:	230720c408c5718e                  	{ ae_l32m.iu	aed17, a8, 4; nop }
 4b8:	0e6d217b0d3d0c544782213b0001012f 	{ ae_s32m.i	aed16, a2, 4; nop; nop; ae_srai64	aed17, aed17, 2 }
 4c8:	0e9ce699dd2322d0c5808e930229402f 	{ ae_s32m.i	aed17, a2, 0; ae_l32m.i	aed27, a2, 8; ae_mulf48q32sp16s.l	aed18, aed17, aed9; nop }
 4d8:	0e9ce419fd2327d0c4808e921a060f2f 	{ ae_l32m.i	aed28, a2, 12; ae_l32m.i	aed31, a2, 28; ae_mulaf48q32sp16s.l	aed18, aed16, aed6; nop }
 4e8:	1e8294932e06342e                  	{ ae_l32m.i	aed19, a2, 16; nop; ae_mulaf48q32sp16s.l	aed18, aed16, aed6 }
 4f0:	1e8294932de0452e                  	{ ae_l32m.i	aed20, a2, 20; nop; ae_mulaf48q32sp16s.l	aed18, aed15, aed0 }
 4f8:	1e8294932f67e62e                  	{ ae_l32m.i	aed30, a2, 24; nop; ae_mulaf48q32sp16s.l	aed18, aed27, aed7 }
 500:	1e82d4972767e72e                  	{ ae_s32m.i	aed30, a2, 28; nop; ae_mulaf48q32sp16s.l	aed18, aed27, aed7 }
 508:	1e82d4972781352e                  	{ ae_s32m.i	aed19, a2, 20; nop; ae_mulaf48q32sp16s.l	aed18, aed28, aed1 }
 510:	0ead317b0d350cd0c5f4613b0001cb2f 	{ ae_s32m.i	aed27, a2, 12; nop; nop; ae_roundsq32f48sym	aed29, aed18 }
 520:	2282d39727a2d22e                  	{ ae_s32m.i	aed29, a2, 8; nop; ae_mulf48q32sp16s.l	aed18, aed29, aed2 }
 528:	1e835493376ab13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed18, aed27, aed10 }
 530:	1e835493376ab13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed18, aed27, aed10 }
 538:	1e8354933783b13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed18, aed28, aed3 }
 540:	1e835493366bb13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed18, aed19, aed11 }
 548:	1e835493366bb13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed18, aed19, aed11 }
 550:	1e8354933684b13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed18, aed20, aed4 }
 558:	0ead317b0d540cd0c54861380c01015f 	{ nop; nop; nop; ae_roundsq32f48sym	aed18, aed18 }
 568:	2282d37f264e242e                  	{ ae_s32m.i	aed18, a2, 16; nop; ae_mulf48q32sp16s.l	aed15, aed18, aed14 }
 570:	1e83547b366cb13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed15, aed19, aed12 }
 578:	1e83547b366cb13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed15, aed19, aed12 }
 580:	1e83547b3685b13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed15, aed20, aed5 }
 588:	1e83547b37cdb13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed15, aed30, aed13 }
 590:	1e83547b37cdb13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed15, aed30, aed13 }
 598:	1e83547b37e8b13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed15, aed31, aed8 }
 5a0:	0ead2d7b099c0cd0c5c0613a0c0141ff 	{ ae_srai64	aed17, aed15, 1; nop; nop; ae_roundsq32f48sym	aed16, aed15 }
 5b0:	220760c400c5b62e                  	{ ae_s32m.i	aed16, a2, 24; nop }
 5b8:	121666                                    	bnei	a6, 1, 5ce <updn_sampling_by3_Proc+0x21a>
 5bb:	0ead257b0d040cd0c580813c0c01d7ff 	{ ae_add64	aed15, aed15, aed17; nop; nop; nop }
 5cb:	000046                                    	j	5d0 <updn_sampling_by3_Proc+0x21c>

000005ce <updn_sampling_by3_Proc+0x21a>:
 5ce:	efec                                    	bnez.n	a15, 600 <updn_sampling_by3_Proc+0x24c>
 5d0:	220720c014a5dcfe                  	{ ae_slaasq56s	aed15, aed15, a12; nop }
 5d8:	1cb766                                    	bnei	a7, 16, 5f8 <updn_sampling_by3_Proc+0x244>
 5db:	0ead2d7b0d5c0cf0c33de1380c01015f 	{ nop; nop; nop; ae_roundsp16q48x2sym	aed15, aed15, aed15 }
 5eb:	230740c31cc5114e                  	{ ae_s16m.l.iu	aed15, a4, 2; nop }
 5f3:	000246                                    	j	600 <updn_sampling_by3_Proc+0x24c>

000005f6 <updn_sampling_by3_Proc+0x242>:
	...

000005f8 <updn_sampling_by3_Proc+0x244>:
 5f8:	230760c318c5b19e                  	{ ae_s32m.iu	aed15, a9, 4; nop }
 600:	3c02edc00c7f1eee                  	{ addi	a14, a14, 1; addi	a13, a13, -1 }

00000608 <updn_sampling_by3_Proc+0x254>:
 608:	fe9421                                    	l32r	a2, 58 (55555556 <_end+0x55554aa2>)
 60b:	b22520                                    	mulsh	a2, a5, a2
 60e:	053f20                                    	extui	a3, a2, 31, 1
 611:	223a                                    	add.n	a2, a2, a3
 613:	932a60                                    	movnez	a2, a10, a6
 616:	822d                                    	ae_sext16	a2, a2
 618:	f01d                                    	retw.n

0000061a <updn_sampling_by3_Proc+0x266>:
	...

0000061c <updn_sampling_16bit_Proc>:
 61c:	004136                                    	entry	a1, 32
 61f:	3a040fa0181fc40e                  	{ beqi.w15	a4, 3, 63b <updn_sampling_16bit_Proc+0x1f>; movi	a10, -1 }
 627:	2b0210a0141a646e                  	{ bnei.w15	a4, 2, 659 <updn_sampling_16bit_Proc+0x3d>; extui	a10, a10, 0, 16 }
 62f:	83ed                                    	ae_sext16	a14, a3
 631:	fe8b31                                    	l32r	a3, 60 (168 <updn_sampling_by2_Proc_V2>)
 634:	000206                                    	j	640 <updn_sampling_16bit_Proc+0x24>

00000637 <updn_sampling_16bit_Proc+0x1b>:
 637:	00000000                                ....

0000063b <updn_sampling_16bit_Proc+0x1f>:
 63b:	83ed                                    	ae_sext16	a14, a3
 63d:	fe8931                                    	l32r	a3, 64 (3b4 <updn_sampling_by3_Proc>)
 640:	2206d240088a00fe                  	{ movi	a15, 16; or	a10, a2, a2 }
 648:	0606b64618ebe55e                  	{ or	a11, a5, a5; or	a12, a6, a6 }
 650:	07dd                                    	mov.n	a13, a7
 652:	0003e0                                    	callx8	a3
 655:	8a2d                                    	ae_sext16	a2, a10
 657:	f01d                                    	retw.n

00000659 <updn_sampling_16bit_Proc+0x3d>:
 659:	8a2d                                    	ae_sext16	a2, a10
 65b:	f01d                                    	retw.n

0000065d <updn_sampling_16bit_Proc+0x41>:
 65d:	000000                                        ...

00000660 <updn_sampling_32bit_Proc>:
 660:	004136                                    	entry	a1, 32
 663:	3a040fa0181fa40e                  	{ beqi.w15	a4, 3, 67b <updn_sampling_32bit_Proc+0x1b>; movi	a10, -1 }
 66b:	2b0210a0141a446e                  	{ bnei.w15	a4, 2, 699 <updn_sampling_32bit_Proc+0x39>; extui	a10, a10, 0, 16 }
 673:	83ed                                    	ae_sext16	a14, a3
 675:	fe7a31                                    	l32r	a3, 60 (168 <updn_sampling_by2_Proc_V2>)
 678:	000106                                    	j	680 <updn_sampling_32bit_Proc+0x20>

0000067b <updn_sampling_32bit_Proc+0x1b>:
 67b:	83ed                                    	ae_sext16	a14, a3
 67d:	fe7931                                    	l32r	a3, 64 (3b4 <updn_sampling_by3_Proc>)
 680:	2206d240108a00fe                  	{ movi	a15, 32; or	a10, a2, a2 }
 688:	0606b64618ebe55e                  	{ or	a11, a5, a5; or	a12, a6, a6 }
 690:	07dd                                    	mov.n	a13, a7
 692:	0003e0                                    	callx8	a3
 695:	8a2d                                    	ae_sext16	a2, a10
 697:	f01d                                    	retw.n

00000699 <updn_sampling_32bit_Proc+0x39>:
 699:	8a2d                                    	ae_sext16	a2, a10
 69b:	f01d                                    	retw.n

0000069d <updn_sampling_32bit_Proc+0x3d>:
 69d:	000000                                        ...

000006a0 <__do_global_ctors_aux>:
 6a0:	004136                                    	entry	a1, 32
 6a3:	fe7131                                    	l32r	a3, 68 (a5c <__CTOR_END__>)
 6a6:	fcc322                                    	addi	a2, a3, -4
 6a9:	002222                                    	l32i	a2, a2, 0
 6ac:	020283a4001e920e                  	{ beqi.w15	a2, -1, 6c1 <__do_global_ctors_aux+0x21>; addi	a3, a3, -8 }

000006b4 <__do_global_ctors_aux+0x14>:
 6b4:	0002e0                                    	callx8	a2
 6b7:	0328                                    	l32i.n	a2, a3, 0
 6b9:	0302e3bc07ffb26e                  	{ bnei.w15	a2, -1, 6b4 <__do_global_ctors_aux+0x14>; addi	a3, a3, -4 }
 6c1:	f01d                                    	retw.n

000006c3 <__do_global_ctors_aux+0x23>:
	...

000006d0 <xt_hifi5_sampler_by_n_memset>:
 6d0:	004136                                    	entry	a1, 32
 6d3:	23069221000962ee                  	{ bnez.w15	a2, 6e5 <xt_hifi5_sampler_by_n_memset+0x15>; or	a6, a2, a2 }
 6db:	020c                                    	movi.n	a2, 0
 6dd:	f01d                                    	retw.n

000006df <xt_hifi5_sampler_by_n_memset+0xf>:
 6df:	00000000                                 ......

000006e5 <xt_hifi5_sampler_by_n_memset+0x15>:
 6e5:	079476                                    	loopnez	a4, 6f0 <xt_hifi5_sampler_by_n_memset+0x20>
 6e8:	2602a6400080063e                  	{ s16i	a3, a6, 0; addi	a6, a6, 2 }

000006f0 <xt_hifi5_sampler_by_n_memset+0x20>:
 6f0:	f01d                                    	retw.n

000006f2 <xt_hifi5_sampler_by_n_memset+0x22>:
	...

000006f4 <xt_hifi5_sampler_by_n_memcpy>:
 6f4:	004136                                    	entry	a1, 32
 6f7:	02bc                                    	beqz.n	a2, 72b <xt_hifi5_sampler_by_n_memcpy+0x37>
 6f9:	e3ac                                    	beqz.n	a3, 72b <xt_hifi5_sampler_by_n_memcpy+0x37>
 6fb:	230692010809832e                  	{ bgeu.w15	a3, a2, 711 <xt_hifi5_sampler_by_n_memcpy+0x1d>; or	a6, a2, a2 }
 703:	907430                                    	addx2	a7, a4, a3
 706:	07b277                                    	bgeu	a2, a7, 711 <xt_hifi5_sampler_by_n_memcpy+0x1d>
 709:	3206844000740c2e                  	{ j	739 <xt_hifi5_sampler_by_n_memcpy+0x45>; addx2	a3, a4, a2 }

00000711 <xt_hifi5_sampler_by_n_memcpy+0x1d>:
 711:	0003c6                                    	j	724 <xt_hifi5_sampler_by_n_memcpy+0x30>

00000714 <xt_hifi5_sampler_by_n_memcpy+0x20>:
 714:	040363401460133e                  	{ addi	a3, a3, 2; l16si	a5, a3, 0 }
 71c:	2602a6400080065e                  	{ s16i	a5, a6, 0; addi	a6, a6, 2 }
 724:	440b                                    	addi.n	a4, a4, -1
 726:	ea0466                                    	bnei	a4, -1, 714 <xt_hifi5_sampler_by_n_memcpy+0x20>
 729:	f01d                                    	retw.n

0000072b <xt_hifi5_sampler_by_n_memcpy+0x37>:
 72b:	020c                                    	movi.n	a2, 0
 72d:	f01d                                    	retw.n

0000072f <xt_hifi5_sampler_by_n_memcpy+0x3b>:
 72f:	278d                                    	ae_l16si.n	a8, a7, 0
 731:	3606b6400088068e                  	{ s16i	a8, a6, 0; or	a3, a6, a6 }
 739:	2602e7c71c7ff44e                  	{ addi	a4, a4, -1; addi	a7, a7, -2 }
 741:	2702e3b807ff446e                  	{ bnei.w15	a4, -1, 72f <xt_hifi5_sampler_by_n_memcpy+0x3b>; addi	a6, a3, -2 }
 749:	fff706                                    	j	729 <xt_hifi5_sampler_by_n_memcpy+0x35>

0000074c <_fini>:
 74c:	008136                                    	entry	a1, 64
 74f:	fe4781                                    	l32r	a8, 6c (88 <__do_global_dtors_aux>)
 752:	f03d                                    	nop.n
 754:	0008e0                                    	callx8	a8

00000757 <_fini+0xb>:
 757:	f01d                                    	retw.n
