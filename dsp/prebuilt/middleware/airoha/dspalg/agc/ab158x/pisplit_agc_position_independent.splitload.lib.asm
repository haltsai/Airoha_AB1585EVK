
Build/lib/pisplit_agc_position_independent.splitload.lib:     file format elf32-xtensa-le


Disassembly of section .text:

00000000 <_init-0x68>:
   0:	000000bc 00000980 00000d8c 00000d74     ............t...
  10:	00000000 00000d58 00000000 00000d90     ....X...........
  20:	00000da8 00000d10 00000110 00000d28     ............(...
  30:	0000019c 00000d3c 00000940 00000d80     ....<...@.......
  40:	00007fff 000009c0 000009a0 00000889     ................
  50:	08000000 00000118 00000428 00000b60     ........(...`...
  60:	00000d5c 00000080                       \.......

00000068 <_init>:
  68:	008136                                    	entry	a1, 64
  6b:	ffe581                                    	l32r	a8, 0 (bc <frame_dummy>)
  6e:	f03d                                    	nop.n
  70:	0008e0                                    	callx8	a8
  73:	ffe481                                    	l32r	a8, 4 (980 <__do_global_ctors_aux>)
  76:	f03d                                    	nop.n
  78:	0008e0                                    	callx8	a8
  7b:	f01d                                    	retw.n

0000007d <_init+0x15>:
  7d:	000000                                        ...

00000080 <__do_global_dtors_aux>:
  80:	004136                                    	entry	a1, 32
  83:	ffe131                                    	l32r	a3, 8 (d8c <__bss_start>)
  86:	000382                                    	l8ui	a8, a3, 0
  89:	d8ec                                    	bnez.n	a8, ba <__do_global_dtors_aux+0x3a>
  8b:	ffe021                                    	l32r	a2, c (d74 <p$7655_3_1>)
  8e:	02b8                                    	l32i.n	a11, a2, 0
  90:	0ba8                                    	l32i.n	a10, a11, 0
  92:	ca8c                                    	beqz.n	a10, a2 <__do_global_dtors_aux+0x22>

00000094 <__do_global_dtors_aux+0x14>:
  94:	9b4b                                    	addi.n	a9, a11, 4
  96:	0299                                    	s32i.n	a9, a2, 0
  98:	000ae0                                    	callx8	a10
  9b:	02b8                                    	l32i.n	a11, a2, 0
  9d:	0ba8                                    	l32i.n	a10, a11, 0
  9f:	ff1a56                                    	bnez	a10, 94 <__do_global_dtors_aux+0x14>
  a2:	ffdba1                                    	l32r	a10, 10 (0 <_text_start>)
  a5:	0b0c                                    	movi.n	a11, 0
  a7:	081ab7                                    	beq	a10, a11, b3 <__do_global_dtors_aux+0x33>
  aa:	ffdaa1                                    	l32r	a10, 14 (d58 <__EH_FRAME_BEGIN___100>)
  ad:	ffd881                                    	l32r	a8, 10 (0 <_text_start>)
  b0:	0008e0                                    	callx8	a8
  b3:	1c0c                                    	movi.n	a12, 1
  b5:	0043c2                                    	s8i	a12, a3, 0
  b8:	f01d                                    	retw.n

000000ba <__do_global_dtors_aux+0x3a>:
  ba:	f01d                                    	retw.n

000000bc <frame_dummy>:
  bc:	004136                                    	entry	a1, 32
  bf:	ffd681                                    	l32r	a8, 18 (0 <_text_start>)
  c2:	090c                                    	movi.n	a9, 0
  c4:	0b1897                                    	beq	a8, a9, d3 <frame_dummy+0x17>
  c7:	ffd3a1                                    	l32r	a10, 14 (d58 <__EH_FRAME_BEGIN___100>)
  ca:	ffd4b1                                    	l32r	a11, 1c (d90 <object$7674_5_1>)
  cd:	ffd281                                    	l32r	a8, 18 (0 <_text_start>)
  d0:	0008e0                                    	callx8	a8

000000d3 <frame_dummy+0x17>:
  d3:	f01d                                    	retw.n

000000d5 <frame_dummy+0x19>:
	...

000000e0 <_start>:
  e0:	004136                                    	entry	a1, 32
  e3:	ffd0a1                                    	l32r	a10, 24 (d10 <_DYNAMIC+0x2c0>)
  e6:	ffce31                                    	l32r	a3, 20 (da8 <printf_ptr>)
  e9:	0288                                    	l32i.n	a8, a2, 0
  eb:	ffcfb1                                    	l32r	a11, 28 (110 <get_agc_memsize>)
  ee:	0389                                    	s32i.n	a8, a3, 0
  f0:	0008e0                                    	callx8	a8
  f3:	0388                                    	l32i.n	a8, a3, 0
  f5:	ffcda1                                    	l32r	a10, 2c (d28 <_DYNAMIC+0x2d8>)
  f8:	ffceb1                                    	l32r	a11, 30 (19c <AGC_VO_Init>)
  fb:	0008e0                                    	callx8	a8
  fe:	0388                                    	l32i.n	a8, a3, 0
 100:	ffcda1                                    	l32r	a10, 34 (d3c <_DYNAMIC+0x2ec>)
 103:	ffcdb1                                    	l32r	a11, 38 (940 <AGC_Proc>)
 106:	0008e0                                    	callx8	a8
 109:	ffcc21                                    	l32r	a2, 3c (d80 <export_parameter_array>)
 10c:	f01d                                    	retw.n

0000010e <_start+0x2e>:
	...

00000110 <get_agc_memsize>:
 110:	004136                                    	entry	a1, 32
 113:	28a122                                    	movi	a2, 0x128
 116:	f01d                                    	retw.n

00000118 <AGC_Xdiv>:
 118:	004136                                    	entry	a1, 32
 11b:	0248                                    	l32i.n	a4, a2, 0
 11d:	0358                                    	l32i.n	a5, a3, 0
 11f:	069416                                    	beqz	a4, 18c <AGC_Xdiv+0x74>
 122:	070516                                    	beqz	a5, 196 <AGC_Xdiv+0x7e>
 125:	0005224010c0603e                  	{ ae_l32m.i	aed2, a3, 0; ae_l32m.i	aed0, a2, 0 }
 12d:	000003f0088300fa572303002460333f 	{ ae_nsa64	a3, aed0; ae_nsa64	a4, aed2; nop }
 13d:	0202e3c6047ef44e                  	{ addi	a4, a4, -8; addi	a3, a3, -8 }
 145:	433340                                    	min	a3, a3, a4
 148:	e9c382                                    	addi	a8, a3, -23
 14b:	220700c010a5582e                  	{ ae_slaasq56s	aed1, aed2, a8; nop }
 153:	0ead257b0d240cd0c400813b0c01621f 	{ ae_trunca32q48	a2, aed1; nop; nop; nop }
 163:	090283a4041e02ae                  	{ beqz.w15	a2, 18a <AGC_Xdiv+0x72>; addi	a8, a3, -8 }
 16b:	220700c010a5d80e                  	{ ae_slaasq56s	aed3, aed0, a8; nop }
 173:	0ead257b0d240cd0c400813b0c01643f 	{ ae_trunca32q48	a4, aed3; nop; nop; nop }
 183:	d22420                                    	quos	a2, a4, a2
 186:	a22d                                    	ae_clamps16	a2, a2
 188:	f01d                                    	retw.n

0000018a <AGC_Xdiv+0x72>:
 18a:	f01d                                    	retw.n

0000018c <AGC_Xdiv+0x74>:
 18c:	ffad91                                    	l32r	a9, 40 (7fff <_end+0x7253>)
 18f:	020c                                    	movi.n	a2, 0
 191:	832950                                    	moveqz	a2, a9, a5
 194:	f01d                                    	retw.n

00000196 <AGC_Xdiv+0x7e>:
 196:	ffaa21                                    	l32r	a2, 40 (7fff <_end+0x7253>)
 199:	f01d                                    	retw.n

0000019b <AGC_Xdiv+0x83>:
	...

0000019c <AGC_VO_Init>:
 19c:	004136                                    	entry	a1, 32
 19f:	ffa981                                    	l32r	a8, 44 (9c0 <xt_memcpy>)
 1a2:	20b330                                    	or	a11, a3, a3
 1a5:	2206d240108a06ce                  	{ movi	a12, 38; or	a10, a2, a2 }
 1ad:	0008e0                                    	callx8	a8
 1b0:	ffa681                                    	l32r	a8, 48 (9a0 <xt_memset>)
 1b3:	50c2a2                                    	addi	a10, a2, 80
 1b6:	0a04404110801cce                  	{ movi	a12, 108; movi	a11, 0 }
 1be:	0008e0                                    	callx8	a8
 1c1:	070392450c80025e                  	{ l8ui	a5, a2, 21; l8ui	a6, a2, 0 }
 1c9:	0603b2420085226e                  	{ s16i	a6, a2, 80; l8ui	a6, a2, 20 }
 1d1:	115580                                    	slli	a5, a5, 8
 1d4:	3203b24508e5f56e                  	{ or	a5, a5, a6; l8ui	a3, a2, 23 }
 1dc:	855d                                    	ae_sext16	a5, a5
 1de:	2403b2440085325e                  	{ s16i	a5, a2, 96; l8ui	a5, a2, 22 }
 1e6:	113380                                    	slli	a3, a3, 8
 1e9:	1e03b24418e6f35e                  	{ or	a3, a3, a5; l8ui	a15, a2, 25 }
 1f1:	833d                                    	ae_sext16	a3, a3
 1f3:	0203b2440886323e                  	{ s16i	a3, a2, 98; l8ui	a3, a2, 24 }
 1fb:	11ff80                                    	slli	a15, a15, 8
 1fe:	3e03b24718e6ef3e                  	{ or	a15, a15, a3; l8ui	a14, a2, 27 }
 206:	8ffd                                    	ae_sext16	a15, a15
 208:	2e03b244108632fe                  	{ s16i	a15, a2, 100; l8ui	a15, a2, 26 }
 210:	11ee80                                    	slli	a14, a14, 8
 213:	3c03b24710e2fefe                  	{ or	a14, a14, a15; l8ui	a13, a2, 11 }
 21b:	8eed                                    	ae_sext16	a14, a14
 21d:	2e03b244188222ee                  	{ s16i	a14, a2, 102; l8ui	a14, a2, 10 }
 225:	11dd80                                    	slli	a13, a13, 8
 228:	1f069d420c8b02ee                  	{ l8ui	a14, a2, 9; or	a13, a13, a14 }
 230:	11dd80                                    	slli	a13, a13, 8
 233:	1f069d42048b02ee                  	{ l8ui	a14, a2, 8; or	a13, a13, a14 }
 23b:	11dd80                                    	slli	a13, a13, 8
 23e:	3c03b24708e3edee                  	{ or	a13, a13, a14; l8ui	a12, a2, 15 }
 246:	2d03b245088312de                  	{ s32i	a13, a2, 84; l8ui	a13, a2, 14 }
 24e:	11cc80                                    	slli	a12, a12, 8
 251:	0d069c430c8b12de                  	{ l8ui	a13, a2, 13; or	a12, a12, a13 }
 259:	11cc80                                    	slli	a12, a12, 8
 25c:	0d069c43048b12de                  	{ l8ui	a13, a2, 12; or	a12, a12, a13 }
 264:	11cc80                                    	slli	a12, a12, 8
 267:	3a03b24700e4fcde                  	{ or	a12, a12, a13; l8ui	a11, a2, 19 }
 26f:	2d03b245108402ce                  	{ s32i	a12, a2, 88; l8ui	a12, a2, 18 }
 277:	11bb80                                    	slli	a11, a11, 8
 27a:	3d069b440c8a02ce                  	{ l8ui	a12, a2, 17; or	a11, a11, a12 }
 282:	11bb80                                    	slli	a11, a11, 8
 285:	3d069b44048a02ce                  	{ l8ui	a12, a2, 16; or	a11, a11, a12 }
 28d:	11bb80                                    	slli	a11, a11, 8
 290:	3a03b24618e7ebce                  	{ or	a11, a11, a12; l8ui	a10, a2, 31 }
 298:	2b03b245188712be                  	{ s32i	a11, a2, 92; l8ui	a11, a2, 30 }
 2a0:	11aa80                                    	slli	a10, a10, 8
 2a3:	2b069a470c8a12be                  	{ l8ui	a11, a2, 29; or	a10, a10, a11 }
 2ab:	11aa80                                    	slli	a10, a10, 8
 2ae:	2b069a47048a12be                  	{ l8ui	a11, a2, 28; or	a10, a10, a11 }
 2b6:	11aa80                                    	slli	a10, a10, 8
 2b9:	3803b24610e8fabe                  	{ or	a10, a10, a11; l8ui	a9, a2, 35 }
 2c1:	2b03b246108802ae                  	{ s32i	a10, a2, 104; l8ui	a10, a2, 34 }
 2c9:	030272                                    	l8ui	a7, a2, 3
 2cc:	190689401486028e                  	{ l8ui	a8, a2, 2; slli	a9, a9, 8 }
 2d4:	1b0699400c8a22ae                  	{ l8ui	a10, a2, 33; or	a9, a9, a10 }
 2dc:	117780                                    	slli	a7, a7, 8
 2df:	3906b74608a9098e                  	{ slli	a9, a9, 8; or	a7, a7, a8 }
 2e7:	1b069940048a22ae                  	{ l8ui	a10, a2, 32; or	a9, a9, a10 }
 2ef:	048170                                    	extui	a8, a7, 1, 1
 2f2:	1806a9470086228e                  	{ s16i	a8, a2, 120; slli	a9, a9, 8 }
 2fa:	3803b24608e9e9ae                  	{ or	a9, a9, a10; l8ui	a8, a2, 39 }
 302:	2903b2461889129e                  	{ s32i	a9, a2, 108; l8ui	a9, a2, 38 }
 30a:	118880                                    	slli	a8, a8, 8
 30d:	090698410c8a329e                  	{ l8ui	a9, a2, 37; or	a8, a8, a9 }
 315:	118880                                    	slli	a8, a8, 8
 318:	09069841048a329e                  	{ l8ui	a9, a2, 36; or	a8, a8, a9 }
 320:	047070                                    	extui	a7, a7, 0, 1
 323:	0806a8470886227e                  	{ s16i	a7, a2, 122; slli	a8, a8, 8 }
 32b:	3603b24600eaf89e                  	{ or	a8, a8, a9; l8ui	a7, a2, 43 }
 333:	2903b247008a028e                  	{ s32i	a8, a2, 112; l8ui	a8, a2, 42 }
 33b:	117780                                    	slli	a7, a7, 8
 33e:	390697420c89228e                  	{ l8ui	a8, a2, 41; or	a7, a7, a8 }
 346:	117780                                    	slli	a7, a7, 8
 349:	390697420489228e                  	{ l8ui	a8, a2, 40; or	a7, a7, a8 }
 351:	117780                                    	slli	a7, a7, 8
 354:	3906f7441c09ee8e                  	{ l32r	a8, 4c (889 <agc_block_proc+0x461>); or	a7, a7, a8 }
 35c:	0f0421470880027e                  	{ s32i	a7, a2, 116; movi	a14, 1 }
 364:	0a034224080fa40e                  	{ beqi.w15	a4, 1, 3fd <AGC_VO_Init+0x261>; l16si	a10, a2, 120 }
 36c:	08042f441880928e                  	{ s16i	a8, a2, 0x126; movi	a9, 15 }
 374:	925292                                    	s16i	a9, a2, 0x124

00000377 <AGC_VO_Init+0x1db>:
 377:	230740a008059a0e                  	{ beqi.w15	a10, 1, 40d <AGC_VO_Init+0x271>; nop }
 37f:	0502a2                                    	l8ui	a10, a2, 5
 382:	0402b2                                    	l8ui	a11, a2, 4
 385:	11aa80                                    	slli	a10, a10, 8
 388:	20aab0                                    	or	a10, a10, a11
 38b:	8aad                                    	ae_sext16	a10, a10
 38d:	2952a2                                    	s16i	a10, a2, 82
 390:	8b0c                                    	movi.n	a11, 8

00000392 <AGC_VO_Init+0x1f6>:
 392:	1a042047108032be                  	{ s16i	a11, a2, 124; movi	a11, 16 }
 39a:	02aba7                                    	bge	a11, a10, 3a0 <AGC_VO_Init+0x204>
 39d:	3e52b2                                    	s16i	a11, a2, 124

000003a0 <AGC_VO_Init+0x204>:
 3a0:	2a044040188010de                  	{ movi	a13, 48; movi	a11, 32 }
 3a8:	1d04080608002bae                  	{ bge.w15	a11, a10, 3b3 <AGC_VO_Init+0x217>; movi	a12, 24 }
 3b0:	3e52c2                                    	s16i	a12, a2, 124
 3b3:	0e0400041c001dae                  	{ blt.w15	a13, a10, 3d8 <AGC_VO_Init+0x23c>; movi	a15, 0 }
 3bb:	9152e2                                    	s16i	a14, a2, 0x122
 3be:	9052f2                                    	s16i	a15, a2, 0x120
 3c1:	ff2381                                    	l32r	a8, 50 (8000000 <_end+0x7fff254>)
 3c4:	446282                                    	s32i	a8, a2, 0x110
 3c7:	456282                                    	s32i	a8, a2, 0x114
 3ca:	436282                                    	s32i	a8, a2, 0x10c
 3cd:	466282                                    	s32i	a8, a2, 0x118
 3d0:	476282                                    	s32i	a8, a2, 0x11c
 3d3:	216282                                    	s32i	a8, a2, 132
 3d6:	f01d                                    	retw.n

000003d8 <AGC_VO_Init+0x23c>:
 3d8:	3e52b2                                    	s16i	a11, a2, 124
 3db:	9152e2                                    	s16i	a14, a2, 0x122
 3de:	090460440c00fcae                  	{ l32r	a10, 50 (8000000 <_end+0x7fff254>); movi	a9, 0 }
 3e6:	905292                                    	s16i	a9, a2, 0x120
 3e9:	4462a2                                    	s32i	a10, a2, 0x110
 3ec:	4562a2                                    	s32i	a10, a2, 0x114
 3ef:	4362a2                                    	s32i	a10, a2, 0x10c
 3f2:	4662a2                                    	s32i	a10, a2, 0x118
 3f5:	4762a2                                    	s32i	a10, a2, 0x11c
 3f8:	2162a2                                    	s32i	a10, a2, 132
 3fb:	f01d                                    	retw.n

000003fd <AGC_VO_Init+0x261>:
 3fd:	ec1c                                    	movi.n	a12, 30
 3ff:	0a042444109192ce                  	{ s16i	a12, a2, 0x124; movi	a11, 0x444 }
 407:	9352b2                                    	s16i	a11, a2, 0x126
 40a:	ffda46                                    	j	377 <AGC_VO_Init+0x1db>

0000040d <AGC_VO_Init+0x271>:
 40d:	3092f2                                    	l16si	a15, a2, 96
 410:	3152f2                                    	s16i	a15, a2, 98
 413:	0c042044108132fe                  	{ s16i	a15, a2, 100; movi	a13, 64 }
 41b:	2952d2                                    	s16i	a13, a2, 82
 41e:	0b0400471c61ee6e                  	{ j	390 <AGC_VO_Init+0x1f4>; movi	a10, 64 }

00000426 <AGC_VO_Init+0x28a>:
	...

00000428 <agc_block_proc>:
 428:	006136                                    	entry	a1, 48
 42b:	189282                                    	l16si	a8, a2, 48
 42e:	0c0401240080a8ae                  	{ beqz.w15	a8, 847 <agc_block_proc+0x41f>; movi	a12, 1 }
 436:	418416                                    	beqz	a4, 852 <agc_block_proc+0x42a>
 439:	00a082                                    	movi	a8, 0

0000043c <agc_block_proc+0x14>:
 43c:	0b9292                                    	l16si	a9, a2, 22
 43f:	195282                                    	s16i	a8, a2, 50
 442:	0aa987                                    	bge	a9, a8, 450 <agc_block_proc+0x28>
 445:	0a0c                                    	movi.n	a10, 0
 447:	1952a2                                    	s16i	a10, a2, 50
 44a:	1752a2                                    	s16i	a10, a2, 46
 44d:	1852a2                                    	s16i	a10, a2, 48
 450:	1592b2                                    	l16si	a11, a2, 42
 453:	f03d                                    	nop.n
 455:	0020f0                                    	nop
 458:	1804062000038bae                  	{ beqz.w15	a11, 46c <agc_block_proc+0x44>; movi	a8, 214 }
 460:	1792d2                                    	l16si	a13, a2, 46
 463:	0020f0                                    	nop
 466:	0020f0                                    	nop
 469:	3f0d16                                    	beqz	a13, 85d <agc_block_proc+0x435>
 46c:	6a92a2                                    	l16si	a10, a2, 212
 46f:	220760c10485982e                  	{ ae_l16m.x	aed4, a2, a8; nop }

00000477 <agc_block_proc+0x4f>:
 477:	0eace487f52334d0c50081270840328f 	{ add	a3, a2, a8; addi	a4, a3, -2; ae_movi	aed2, 0; nop }
 487:	0000004a888000ba1403260c2de23acf 	{ blti.w15	a10, 1, 4f0 <agc_block_proc+0xc8>; or	a8, a4, a4; ae_mov	aed3, aed2 }
 497:	230760c00485718e                  	{ ae_l16m.iu	aed0, a8, 2; nop }
 49f:	220700c00ce5d05e                  	{ ae_abs24s	aed1, aed0; nop }
 4a7:	352aa6                                    	blti	a10, 2, 4e0 <agc_block_proc+0xb8>
 4aa:	feca92                                    	addi	a9, a10, -2
 4ad:	230720c318e5747e                  	{ nop; nop }
 4b5:	0020f0                                    	nop
 4b8:	1104584500a0194e                  	{ loopgtz	a9, 4d0 <agc_block_proc+0xa8>; ae_l16m.iu	aed0, a8, 2 }
 4c0:	0e9cc2f101fa8128c38c2b100081014f 	{ ae_abs24s	aed1, aed0; ae_l16m.iu	aed0, a8, 2; ae_mulaf32s.hh	aed2, aed4, aed1; ae_max32	aed3, aed3, aed1 }

000004d0 <agc_block_proc+0xa8>:
 4d0:	0ea5001b09ec0c208596a3080263014f 	{ ae_abs24s	aed1, aed0; nop; ae_max32	aed3, aed3, aed1; ae_mulaf32s.hh	aed2, aed4, aed1 }

000004e0 <agc_block_proc+0xb8>:
 4e0:	0e9d06fb08e40cd0c4808b100881e33f 	{ ae_max32	aed3, aed3, aed1; nop; ae_mulaf32s.hh	aed2, aed4, aed1; nop }
 4f0:	38c282                                    	addi	a8, a2, 56
 4f3:	0ead217b0d350cd0c4a041381801448f 	{ ae_l32m.i	aed5, a8, 0; nop; nop; ae_roundsq32f48asym	aed8, aed2 }
 503:	0ead057b08e40cd0c58081390c01738f 	{ ae_subsq56s	aed7, aed8, aed5; nop; nop; nop }
 513:	0ead257b09240cd0c580813a0c01c37f 	{ ae_srai64	aed7, aed7, 3; nop; nop; nop }
 523:	0ead257b0d5c0cd0c51841380c01015f 	{ nop; nop; nop; ae_roundsq32f48asym	aed6, aed7 }
 533:	0ead057b08e40cd0c48081391001d05f 	{ ae_addsq56s	aed4, aed5, aed6; nop; nop; nop }
 543:	220760c100c5b08e                  	{ ae_s32m.i	aed4, a8, 0; nop }
 54b:	28038242048302fe                  	{ l32i	a15, a2, 32; l32i	a8, a2, 56 }
 553:	220740c008e5132e                  	{ ae_cvtq56p32s.l	aed1, aed3; nop }
 55b:	220760c008c5b01e                  	{ ae_s32m.i	aed1, a1, 0; nop }
 563:	220760841805b8fe                  	{ blt.w15	a8, a15, 63c <agc_block_proc+0x214>; nop }
 56b:	18030240089a029e                  	{ l16si	a9, a2, 2; l16si	a8, a2, 210 }
 573:	169252                                    	l16si	a5, a2, 44
 576:	0603c1021c20c98e                  	{ bne.w15	a9, a8, 732 <agc_block_proc+0x30a>; l32i	a6, a1, 0 }
 57e:	3e03a242088372ce                  	{ s16i	a12, a2, 210; l32i	a15, a2, 60 }
 586:	0b02c220146125ce                  	{ blti.w15	a5, 2, 930 <agc_block_proc+0x508>; addi	a10, a2, 4 }
 58e:	0b06da47188a6f7e                  	{ movi	a7, -1; or	a8, a10, a10 }
 596:	3e046fc71c7fe5be                  	{ addi	a11, a5, -1; movi	a14, -1 }
 59e:	3d044fc000bf3b4e                  	{ loopgtz	a11, 5c2 <agc_block_proc+0x19a>; movi	a13, -1 }
 5a6:	f8c8                                    	l32i.n	a12, a8, 60
 5a8:	0802a8020c012fce                  	{ bge.w15	a15, a12, 610 <agc_block_proc+0x1e8>; addi	a8, a8, 4 }

000005b0 <agc_block_proc+0x188>:
 5b0:	2c06bd4518ebfeee                  	{ or	a7, a14, a14; or	a14, a13, a13 }
 5b8:	3c06bc4708ebeffe                  	{ or	a13, a15, a15; or	a15, a12, a12 }

000005c0 <agc_block_proc+0x198>:
 5c0:	f03d                                    	nop.n
 5c2:	0f06a24208ab32de                  	{ srai	a9, a13, 2; srai	a12, a15, 2 }
 5ca:	0706a24218aa32ee                  	{ srai	a11, a14, 2; srai	a8, a7, 2 }
 5d2:	08068c4204f338be                  	{ add	a8, a8, a11; add	a12, a12, a9 }
 5da:	80cc80                                    	add	a12, a12, a8
 5dd:	0d62c2                                    	s32i	a12, a2, 52
 5e0:	230720a0102535ce                  	{ blti.w15	a5, 2, 72a <agc_block_proc+0x302>; nop }
 5e8:	0206d240008a00ce                  	{ movi	a12, 0; or	a8, a2, a2 }
 5f0:	178076                                    	loop	a0, 60b <agc_block_proc+0x1e3>
 5f3:	1028e2                                    	l32i	a14, a8, 64
 5f6:	f8e9                                    	s32i.n	a14, a8, 60
 5f8:	169252                                    	l16si	a5, a2, 44
 5fb:	3c02e5c00c7f1cce                  	{ addi	a12, a12, 1; addi	a13, a5, -1 }
 603:	090288060c210cde                  	{ bge.w15	a12, a13, 72a <agc_block_proc+0x302>; addi	a8, a8, 4 }

0000060b <agc_block_proc+0x1e3>:
 60b:	fff846                                    	j	5f0 <agc_block_proc+0x1c8>

0000060e <agc_block_proc+0x1e6>:
	...

00000610 <agc_block_proc+0x1e8>:
 610:	0fadc7                                    	bge	a13, a12, 623 <agc_block_proc+0x1fb>

00000613 <agc_block_proc+0x1eb>:
 613:	2c06bd4518ebfeee                  	{ or	a7, a14, a14; or	a14, a13, a13 }
 61b:	1d069c471c6be1ae                  	{ j	5c0 <agc_block_proc+0x198>; or	a13, a12, a12 }

00000623 <agc_block_proc+0x1fb>:
 623:	37068c8208097ece                  	{ bge.w15	a14, a12, 635 <agc_block_proc+0x20d>; max	a7, a12, a7 }
 62b:	0e7d                                    	mov.n	a7, a14
 62d:	2d069c471c6bef8e                  	{ j	5c0 <agc_block_proc+0x198>; or	a14, a12, a12 }

00000635 <agc_block_proc+0x20d>:
 635:	ffe1c6                                    	j	5c0 <agc_block_proc+0x198>

00000638 <agc_block_proc+0x210>:
 638:	00000000                                ....

0000063c <agc_block_proc+0x214>:
 63c:	1492f2                                    	l16si	a15, a2, 40
 63f:	276f16                                    	beqz	a15, 8b9 <agc_block_proc+0x491>
 642:	2f2282                                    	l32i	a8, a2, 188
 645:	01d2d2                                    	addmi	a13, a2, 0x100
 648:	20076840046fcdde                  	{ addi	a13, a13, -64; ae_cvt48a32	aed15, a8 }
 650:	0c02a24300c460de                  	{ ae_l32m.i	aed12, a13, 0; addi	a12, a2, 16 }
 658:	0eacc57070e2c0d0c580813b040175ff 	{ ae_subsq56s	aed13, aed15, aed12; ae_l16m.i	aed14, a12, 0; nop; nop }
 668:	1e83546335aeb13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed12, aed13, aed14 }
 670:	0ead2d7b0d440cd0c52c41380c01015f 	{ nop; nop; nop; ae_roundsq32f48asym	aed11, aed12 }
 680:	0eace5698d242ad0c400813b0c016bbf 	{ ae_trunca32q48	a11, aed11; l32i	a10, a2, 196; nop; nop }
 690:	21072a44008022be                  	{ s32i	a11, a2, 192; ae_cvt48a32	aed0, a10 }
 698:	0ea5243b0d250cd0c4008b000109883f 	{ ae_l16m.i	aed10, a3, 0; nop; ae_subsq56s	aed9, aed11, aed0; nop }
 6a8:	2281d34e392aa29e                  	{ l16si	a9, a2, 212; nop; ae_mulf48q32sp16s.l	aed9, aed9, aed10 }
 6b0:	0ead297b0d2d0cd0c508413f1001844f 	{ or	a8, a4, a4; nop; nop; ae_roundsq32f48asym	aed2, aed9 }
 6c0:	100438200c0139ce                  	{ blti.w15	a9, 1, 728 <agc_block_proc+0x300>; ae_l16m.iu	aed1, a8, 2 }
 6c8:	0a044440008000ce                  	{ movi	a12, 0; movi	a10, 4 }
 6d0:	230720c318e5747e                  	{ nop; nop }
 6d8:	0020f0                                    	nop
 6db:	f03d                                    	nop.n
 6dd:	448076                                    	loop	a0, 725 <agc_block_proc+0x2fd>
 6e0:	0ead257b0d040cd0c58081381401c00f 	{ ae_add64	aed0, aed0, aed2; nop; nop; nop }
 6f0:	2283538b3401b13e                  	{ nop; nop; ae_mulf48q32sp16s.l	aed17, aed0, aed1 }
 6f8:	220740c300a57a1e                  	{ ae_slaaq56	aed17, aed17, a10; nop }
 700:	0ead317b0d4c0ce8c34221380c01015f 	{ nop; nop; nop; ae_roundsp16q48x2asym	aed16, aed17, aed17 }
 710:	220740c404c5108e                  	{ ae_s16m.l.i	aed16, a8, 0; nop }
 718:	6a92e2                                    	l16si	a14, a2, 212
 71b:	cc1b                                    	addi.n	a12, a12, 1
 71d:	1104180608013cee                  	{ bge.w15	a12, a14, 728 <agc_block_proc+0x300>; ae_l16m.iu	aed1, a8, 2 }

00000725 <agc_block_proc+0x2fd>:
 725:	ffe9c6                                    	j	6d0 <agc_block_proc+0x2a8>

00000728 <agc_block_proc+0x300>:
 728:	f01d                                    	retw.n

0000072a <agc_block_proc+0x302>:
 72a:	a0f520                                    	addx4	a15, a5, a2
 72d:	ef69                                    	s32i.n	a6, a15, 56
 72f:	000486                                    	j	745 <agc_block_proc+0x31d>

00000732 <agc_block_proc+0x30a>:
 732:	a81b                                    	addi.n	a10, a8, 1
 734:	0206a542089a62ae                  	{ s16i	a10, a2, 210; addx4	a8, a5, a2 }
 73c:	e898                                    	l32i.n	a9, a8, 56
 73e:	01a967                                    	bge	a9, a6, 743 <agc_block_proc+0x31b>
 741:	e869                                    	s32i.n	a6, a8, 56
 743:	a24b                                    	addi.n	a10, a2, 4
 745:	fe4381                                    	l32r	a8, 54 (118 <AGC_Xdiv>)
 748:	302292                                    	l32i	a9, a2, 192
 74b:	0b02a244088d329e                  	{ s32i	a9, a2, 196; addi	a11, a2, 52 }
 753:	0008e0                                    	callx8	a8
 756:	3322b2                                    	l32i	a11, a2, 204
 759:	118a00                                    	slli	a8, a10, 16
 75c:	c0a8b0                                    	sub	a10, a8, a11
 75f:	20070b2408226ace                  	{ blti.w15	a10, 1, 870 <agc_block_proc+0x448>; ae_cvt48a32	aed2, a11 }
 767:	14c2c2                                    	addi	a12, a2, 20
 76a:	20076a45049360ce                  	{ ae_l16m.i	aed20, a12, 0; ae_cvt48a32	aed19, a10 }
 772:	1e8354133674b13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed2, aed19, aed20 }
 77a:	0ead217b0d540cd0c54841380c01015f 	{ nop; nop; nop; ae_roundsq32f48asym	aed18, aed2 }
 78a:	0eace56815252dd0c400813b0c016c2f 	{ ae_trunca32q48	a12, aed18; l32i	a13, a2, 8; nop; nop }
 79a:	3362c2                                    	s32i	a12, a2, 204
 79d:	072cd7                                    	blt	a12, a13, 7a8 <agc_block_proc+0x380>
 7a0:	0d06bd44188b32de                  	{ s32i	a13, a2, 204; or	a12, a13, a13 }
 7a8:	1492d2                                    	l16si	a13, a2, 40
 7ab:	0020f0                                    	nop
 7ae:	0020f0                                    	nop
 7b1:	220720a40c257d0e                  	{ beqi.w15	a13, 1, 922 <agc_block_proc+0x4fa>; nop }
 7b9:	3222e2                                    	l32i	a14, a2, 200
 7bc:	c0d8e0                                    	sub	a13, a8, a14
 7bf:	31070e2408206dce                  	{ blti.w15	a13, 1, 8d2 <agc_block_proc+0x4aa>; ae_movda32	aed0, a14 }
 7c7:	220720c514c5362e                  	{ ae_l32.i	aed22, a2, 24; nop }
 7cf:	228347b33416b13e                  	{ nop; nop; ae_mulf32r.ll	aed22, aed0, aed22 }
 7d7:	01a0f2                                    	movi	a15, 1
 7da:	220740c304a5bf6e                  	{ ae_slaaq56	aed22, aed22, a15; nop }
 7e2:	0ead357b0d540cd0c55441380c01015f 	{ nop; nop; nop; ae_roundsq32f48asym	aed21, aed22 }
 7f2:	0eace56815252ed0c400813b0c01685f 	{ ae_trunca32q48	a8, aed21; l32i	a14, a2, 8; nop; nop }
 802:	326282                                    	s32i	a8, a2, 200
 805:	0728e7                                    	blt	a8, a14, 810 <agc_block_proc+0x3e8>
 808:	0f06be44108a22ee                  	{ s32i	a14, a2, 200; or	a8, a14, a14 }
 810:	1b0700c218e2cede                  	{ abs	a11, a13; abs	a9, a10 }
 818:	0a030206081a89be                  	{ bge.w15	a9, a11, 82d <agc_block_proc+0x405>; l16si	a10, a2, 208 }
 820:	15bae6                                    	bgei	a10, 16, 839 <agc_block_proc+0x411>
 823:	aa1b                                    	addi.n	a10, a10, 1
 825:	8aad                                    	ae_sext16	a10, a10
 827:	6852a2                                    	s16i	a10, a2, 208
 82a:	0002c6                                    	j	839 <agc_block_proc+0x411>

0000082d <agc_block_proc+0x405>:
 82d:	0b7c                                    	movi.n	a11, -16
 82f:	06aba7                                    	bge	a11, a10, 839 <agc_block_proc+0x411>
 832:	aa0b                                    	addi.n	a10, a10, -1
 834:	8aad                                    	ae_sext16	a10, a10
 836:	6852a2                                    	s16i	a10, a2, 208
 839:	0b0c                                    	movi.n	a11, 0
 83b:	72bba0                                    	salt	a11, a11, a10
 83e:	938cb0                                    	movnez	a8, a12, a11
 841:	2f6282                                    	s32i	a8, a2, 188
 844:	ff7f46                                    	j	645 <agc_block_proc+0x21d>

00000847 <agc_block_proc+0x41f>:
 847:	748c                                    	beqz.n	a4, 852 <agc_block_proc+0x42a>
 849:	1852c2                                    	s16i	a12, a2, 48
 84c:	1752c2                                    	s16i	a12, a2, 46
 84f:	feff46                                    	j	450 <agc_block_proc+0x28>

00000852 <agc_block_proc+0x42a>:
 852:	199282                                    	l16si	a8, a2, 50
 855:	190288461c60e3ee                  	{ j	43c <agc_block_proc+0x14>; addi	a8, a8, 1 }

0000085d <agc_block_proc+0x435>:
 85d:	2f2282                                    	l32i	a8, a2, 188
 860:	1802f2471460f34e                  	{ addi	a4, a3, -2; addmi	a9, a2, 0x100 }
 868:	230289c70c75f9de                  	{ j	645 <agc_block_proc+0x21d>; addi	a3, a9, -42 }

00000870 <agc_block_proc+0x448>:
 870:	12c2b2                                    	addi	a11, a2, 18
 873:	20076a460c9860be                  	{ ae_l16m.i	aed25, a11, 0; ae_cvt48a32	aed24, a10 }
 87b:	1e8354133719b13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed2, aed24, aed25 }
 883:	0ead217b0d540cd0c55c41380c01015f 	{ nop; nop; nop; ae_roundsq32f48asym	aed23, aed2 }
 893:	0eace5681d252dd0c400813b0c016c7f 	{ ae_trunca32q48	a12, aed23; l32i	a13, a2, 12; nop; nop }
 8a3:	3362c2                                    	s32i	a12, a2, 204
 8a6:	230760981fc5fdce                  	{ blt.w15	a13, a12, 7a8 <agc_block_proc+0x380>; nop }
 8ae:	3362d2                                    	s32i	a13, a2, 204
 8b1:	0d069d47146bf3fe                  	{ j	7a8 <agc_block_proc+0x380>; or	a12, a13, a13 }

000008b9 <agc_block_proc+0x491>:
 8b9:	92c8                                    	l32i.n	a12, a2, 36
 8bb:	2307409e0ba518ce                  	{ bge.w15	a8, a12, 642 <agc_block_proc+0x21a>; nop }
 8c3:	fde381                                    	l32r	a8, 50 (8000000 <_end+0x7fff254>)
 8c6:	2f6282                                    	s32i	a8, a2, 188
 8c9:	336282                                    	s32i	a8, a2, 204
 8cc:	326282                                    	s32i	a8, a2, 200
 8cf:	ff5c86                                    	j	645 <agc_block_proc+0x21d>

000008d2 <agc_block_proc+0x4aa>:
 8d2:	220720c61cc5372e                  	{ ae_l32.i	aed27, a2, 28; nop }
 8da:	228347db341bb13e                  	{ nop; nop; ae_mulf32r.ll	aed27, aed0, aed27 }
 8e2:	1e0c                                    	movi.n	a14, 1
 8e4:	220760c300a5febe                  	{ ae_slaaq56	aed27, aed27, a14; nop }
 8ec:	0ead397b0d5c0cd0c56841380c01015f 	{ nop; nop; nop; ae_roundsq32f48asym	aed26, aed27 }
 8fc:	0eace5681d252ed0c400813b0c0168af 	{ ae_trunca32q48	a8, aed26; l32i	a14, a2, 12; nop; nop }
 90c:	326282                                    	s32i	a8, a2, 200
 90f:	2207609c1fc5fe8e                  	{ blt.w15	a14, a8, 810 <agc_block_proc+0x3e8>; nop }
 917:	3262e2                                    	s32i	a14, a2, 200
 91a:	0f069e47146ae2fe                  	{ j	810 <agc_block_proc+0x3e8>; or	a8, a14, a14 }

00000922 <agc_block_proc+0x4fa>:
 922:	2f62c2                                    	s32i	a12, a2, 188
 925:	0d069c470c6aec1e                  	{ j	645 <agc_block_proc+0x21d>; or	a8, a12, a12 }

0000092d <agc_block_proc+0x505>:
 92d:	000000                                        ...

00000930 <agc_block_proc+0x508>:
 930:	3f044fc7189f6f7e                  	{ movi	a7, -1; movi	a14, -1 }
 938:	3d040fc7047ff68e                  	{ j	5c2 <agc_block_proc+0x19a>; movi	a13, -1 }

00000940 <AGC_Proc>:
 940:	004136                                    	entry	a1, 32
 943:	289282                                    	l16si	a8, a2, 80
 946:	f03d                                    	nop.n
 948:	e8ac                                    	beqz.n	a8, 97a <AGC_Proc+0x3a>
 94a:	929292                                    	l16si	a9, a2, 0x124
 94d:	0602c2400094102e                  	{ movi	a2, 0; addi	a7, a2, 80 }
 955:	d25590                                    	quos	a5, a5, a9
 958:	2f0689200805f5ce                  	{ blti.w15	a5, 1, 97a <AGC_Proc+0x3a>; slli	a6, a9, 1 }

00000960 <AGC_Proc+0x20>:
 960:	fdbe81                                    	l32r	a8, 58 (428 <agc_block_proc>)
 963:	04cd                                    	mov.n	a12, a4
 965:	3206b34610eaf77e                  	{ or	a10, a7, a7; or	a11, a3, a3 }
 96d:	0008e0                                    	callx8	a8
 970:	221b                                    	addi.n	a2, a2, 1
 972:	3706e31a1ff0452e                  	{ bne.w15	a5, a2, 960 <AGC_Proc+0x20>; add	a3, a3, a6 }
 97a:	f01d                                    	retw.n

0000097c <AGC_Proc+0x3c>:
 97c:	00000000                                ....

00000980 <__do_global_ctors_aux>:
 980:	004136                                    	entry	a1, 32
 983:	fdb681                                    	l32r	a8, 5c (b60 <_DYNAMIC+0x110>)
 986:	7f2882                                    	l32i	a8, a8, 0x1fc
 989:	fdb521                                    	l32r	a2, 60 (d5c <__CTOR_LIST___103>)
 98c:	0c0826                                    	beqi	a8, -1, 99c <__do_global_ctors_aux+0x1c>
 98f:	02a8                                    	l32i.n	a10, a2, 0

00000991 <__do_global_ctors_aux+0x11>:
 991:	000ae0                                    	callx8	a10
 994:	fcc222                                    	addi	a2, a2, -4
 997:	02a8                                    	l32i.n	a10, a2, 0
 999:	f40a66                                    	bnei	a10, -1, 991 <__do_global_ctors_aux+0x11>
 99c:	f01d                                    	retw.n

0000099e <__do_global_ctors_aux+0x1e>:
	...

000009a0 <xt_memset>:
 9a0:	004136                                    	entry	a1, 32
 9a3:	220692240009a2ae                  	{ beqz.w15	a2, 9bc <xt_memset+0x1c>; or	a6, a2, a2 }
 9ab:	f03d                                    	nop.n
 9ad:	079476                                    	loopnez	a4, 9b8 <xt_memset+0x18>
 9b0:	2202a2400080023e                  	{ s16i	a3, a2, 0; addi	a2, a2, 2 }

000009b8 <xt_memset+0x18>:
 9b8:	062d                                    	mov.n	a2, a6
 9ba:	f01d                                    	retw.n

000009bc <xt_memset+0x1c>:
 9bc:	020c                                    	movi.n	a2, 0
 9be:	f01d                                    	retw.n

000009c0 <xt_memcpy>:
 9c0:	004136                                    	entry	a1, 32
 9c3:	3206b224040962ae                  	{ beqz.w15	a2, a34 <xt_memcpy+0x74>; or	a7, a2, a2 }
 9cb:	065316                                    	beqz	a3, a34 <xt_memcpy+0x74>
 9ce:	390284810c1fa32e                  	{ bgeu.w15	a3, a2, a08 <xt_memcpy+0x48>; addi	a8, a4, -1 }
 9d6:	340284c414ff343e                  	{ addx2	a2, a4, a3; addi	a5, a4, -1 }
 9de:	270684010c15372e                  	{ bgeu.w15	a7, a2, a08 <xt_memcpy+0x48>; addx2	a6, a4, a7 }
 9e6:	fec662                                    	addi	a6, a6, -2
 9e9:	2302a2a4001f050e                  	{ beqi.w15	a5, -1, a30 <xt_memcpy+0x70>; addi	a2, a2, -2 }
 9f1:	0f9476                                    	loopnez	a4, a04 <xt_memcpy+0x44>
 9f4:	220282c0009f023e                  	{ l16si	a3, a2, 0; addi	a2, a2, -2 }
 9fc:	2602a6c0009f063e                  	{ s16i	a3, a6, 0; addi	a6, a6, -2 }

00000a04 <xt_memcpy+0x44>:
 a04:	072d                                    	mov.n	a2, a7
 a06:	f01d                                    	retw.n

00000a08 <xt_memcpy+0x48>:
 a08:	220693200408380e                  	{ beqi.w15	a8, -1, a30 <xt_memcpy+0x70>; or	a2, a3, a3 }
 a10:	043d                                    	mov.n	a3, a4
 a12:	0020f0                                    	nop
 a15:	0020f0                                    	nop
 a18:	0706d74500a9138e                  	{ loopnez	a3, a30 <xt_memcpy+0x70>; or	a4, a7, a7 }
 a20:	220282400080023e                  	{ l16si	a3, a2, 0; addi	a2, a2, 2 }
 a28:	2402a4400080043e                  	{ s16i	a3, a4, 0; addi	a4, a4, 2 }
 a30:	072d                                    	mov.n	a2, a7
 a32:	f01d                                    	retw.n

00000a34 <xt_memcpy+0x74>:
 a34:	020c                                    	movi.n	a2, 0
 a36:	f01d                                    	retw.n

00000a38 <_fini>:
 a38:	008136                                    	entry	a1, 64
 a3b:	fd8a81                                    	l32r	a8, 64 (80 <__do_global_dtors_aux>)
 a3e:	f03d                                    	nop.n
 a40:	0008e0                                    	callx8	a8

00000a43 <_fini+0xb>:
 a43:	f01d                                    	retw.n
