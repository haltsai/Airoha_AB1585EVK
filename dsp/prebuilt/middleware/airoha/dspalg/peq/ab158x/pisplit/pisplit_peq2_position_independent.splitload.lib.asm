
Build/lib/pisplit_peq2_position_independent.splitload.lib:     file format elf32-xtensa-le


Disassembly of section .text:

00000000 <_init-0x54>:
   0:	000000ac 00000460 00000788 0000077c     ....`.......|...
  10:	00000000 00000760 00000000 0000078c     ....`...........
  20:	000007a4 00000734 000000f0 00000744     ....4.......D...
  30:	00000434 00000780 0000025c 000001ac     4.......\.......
  40:	00000324 000003ac 00000568 00000764     $.......h...d...
  50:	00000070                                p...

00000054 <_init>:
  54:	008136                                    	entry	a1, 64
  57:	ffea81                                    	l32r	a8, 0 (ac <frame_dummy>)
  5a:	f03d                                    	nop.n
  5c:	0008e0                                    	callx8	a8
  5f:	ffe981                                    	l32r	a8, 4 (460 <__do_global_ctors_aux>)
  62:	f03d                                    	nop.n
  64:	0008e0                                    	callx8	a8
  67:	f01d                                    	retw.n

00000069 <_init+0x15>:
  69:	00000000 36000000                                .......

00000070 <__do_global_dtors_aux>:
  70:	004136                                    	entry	a1, 32
  73:	ffe531                                    	l32r	a3, 8 (788 <__bss_start>)
  76:	000382                                    	l8ui	a8, a3, 0
  79:	d8ec                                    	bnez.n	a8, aa <__do_global_dtors_aux+0x3a>
  7b:	ffe421                                    	l32r	a2, c (77c <p$7655_3_1>)
  7e:	02b8                                    	l32i.n	a11, a2, 0
  80:	0ba8                                    	l32i.n	a10, a11, 0
  82:	ca8c                                    	beqz.n	a10, 92 <__do_global_dtors_aux+0x22>

00000084 <__do_global_dtors_aux+0x14>:
  84:	9b4b                                    	addi.n	a9, a11, 4
  86:	0299                                    	s32i.n	a9, a2, 0
  88:	000ae0                                    	callx8	a10
  8b:	02b8                                    	l32i.n	a11, a2, 0
  8d:	0ba8                                    	l32i.n	a10, a11, 0
  8f:	ff1a56                                    	bnez	a10, 84 <__do_global_dtors_aux+0x14>
  92:	ffdfa1                                    	l32r	a10, 10 (0 <_text_start>)
  95:	0b0c                                    	movi.n	a11, 0
  97:	081ab7                                    	beq	a10, a11, a3 <__do_global_dtors_aux+0x33>
  9a:	ffdea1                                    	l32r	a10, 14 (760 <__EH_FRAME_BEGIN___100>)
  9d:	ffdc81                                    	l32r	a8, 10 (0 <_text_start>)
  a0:	0008e0                                    	callx8	a8
  a3:	1c0c                                    	movi.n	a12, 1
  a5:	0043c2                                    	s8i	a12, a3, 0
  a8:	f01d                                    	retw.n

000000aa <__do_global_dtors_aux+0x3a>:
  aa:	f01d                                    	retw.n

000000ac <frame_dummy>:
  ac:	004136                                    	entry	a1, 32
  af:	ffda81                                    	l32r	a8, 18 (0 <_text_start>)
  b2:	090c                                    	movi.n	a9, 0
  b4:	0b1897                                    	beq	a8, a9, c3 <frame_dummy+0x17>
  b7:	ffd7a1                                    	l32r	a10, 14 (760 <__EH_FRAME_BEGIN___100>)
  ba:	ffd8b1                                    	l32r	a11, 1c (78c <object$7674_5_1>)
  bd:	ffd681                                    	l32r	a8, 18 (0 <_text_start>)
  c0:	0008e0                                    	callx8	a8

000000c3 <frame_dummy+0x17>:
  c3:	f01d                                    	retw.n

000000c5 <frame_dummy+0x19>:
  c5:	000000                                        ...

000000c8 <_start>:
  c8:	004136                                    	entry	a1, 32
  cb:	ffd6a1                                    	l32r	a10, 24 (734 <_DYNAMIC+0x29c>)
  ce:	ffd431                                    	l32r	a3, 20 (7a4 <printf_ptr>)
  d1:	0288                                    	l32i.n	a8, a2, 0
  d3:	ffd5b1                                    	l32r	a11, 28 (f0 <PEQ2_INIT>)
  d6:	0389                                    	s32i.n	a8, a3, 0
  d8:	0008e0                                    	callx8	a8
  db:	0388                                    	l32i.n	a8, a3, 0
  dd:	ffd3a1                                    	l32r	a10, 2c (744 <_DYNAMIC+0x2ac>)
  e0:	ffd4b1                                    	l32r	a11, 30 (434 <PEQ2_PROC>)
  e3:	0008e0                                    	callx8	a8
  e6:	ffd321                                    	l32r	a2, 34 (780 <export_parameter_array>)
  e9:	f01d                                    	retw.n

000000eb <_start+0x23>:
  eb:	00000000 00413600                                .....

000000f0 <PEQ2_INIT>:
  f0:	004136                                    	entry	a1, 32
  f3:	22ad                                    	ae_l16si.n	a10, a2, 0
  f5:	63ad                                    	ae_s16i.n	a10, a3, 0
  f7:	222b                                    	addi.n	a2, a2, 2
  f9:	711aa6                                    	blti	a10, 1, 16e <PEQ2_INIT+0x7e>
  fc:	060c                                    	movi.n	a6, 0

000000fe <PEQ2_INIT+0xe>:
  fe:	a08630                                    	addx4	a8, a6, a3
 101:	071292                                    	l16ui	a9, a2, 14
 104:	0312c2                                    	l16ui	a12, a2, 6
 107:	0112d2                                    	l16ui	a13, a2, 2
 10a:	049252                                    	l16si	a5, a2, 8
 10d:	0992b2                                    	l16si	a11, a2, 18
 110:	0092f2                                    	l16si	a15, a2, 0
 113:	089272                                    	l16si	a7, a2, 16
 116:	029242                                    	l16si	a4, a2, 4
 119:	0692e2                                    	l16si	a14, a2, 12
 11c:	9bbd                                    	ae_zext16	a11, a11
 11e:	0512a2                                    	l16ui	a10, a2, 10
 121:	117700                                    	slli	a7, a7, 16
 124:	11ee00                                    	slli	a14, a14, 16
 127:	114400                                    	slli	a4, a4, 16
 12a:	11ff00                                    	slli	a15, a15, 16
 12d:	115500                                    	slli	a5, a5, 16
 130:	20aa50                                    	or	a10, a10, a5
 133:	20ddf0                                    	or	a13, a13, a15
 136:	20cc40                                    	or	a12, a12, a4
 139:	2099e0                                    	or	a9, a9, a14
 13c:	609090                                    	neg	a9, a9
 13f:	21c8c0                                    	srai	a12, a12, 8
 142:	21d8d0                                    	srai	a13, a13, 8
 145:	1068c2                                    	s32i	a12, a8, 64
 148:	21a8a0                                    	srai	a10, a10, 8
 14b:	18d9                                    	s32i.n	a13, a8, 4
 14d:	2077b0                                    	or	a7, a7, a11
 150:	1f68a2                                    	s32i	a10, a8, 124
 153:	607070                                    	neg	a7, a7
 156:	219890                                    	srai	a9, a9, 8
 159:	2e6892                                    	s32i	a9, a8, 184
 15c:	217870                                    	srai	a7, a7, 8
 15f:	3d6872                                    	s32i	a7, a8, 244
 162:	23ad                                    	ae_l16si.n	a10, a3, 0
 164:	661b                                    	addi.n	a6, a6, 1
 166:	866d                                    	ae_sext16	a6, a6
 168:	14c222                                    	addi	a2, a2, 20
 16b:	8f26a7                                    	blt	a6, a10, fe <PEQ2_INIT+0xe>
 16e:	22dd                                    	ae_l16si.n	a13, a2, 0
 170:	52cd                                    	ae_l16ui.n	a12, a2, 2
 172:	11dd00                                    	slli	a13, a13, 16
 175:	20ccd0                                    	or	a12, a12, a13
 178:	21c8c0                                    	srai	a12, a12, 8
 17b:	060c                                    	movi.n	a6, 0
 17d:	4c63c2                                    	s32i	a12, a3, 0x130
 180:	020c                                    	movi.n	a2, 0
 182:	231aa6                                    	blti	a10, 1, 1a9 <PEQ2_INIT+0xb9>
 185:	a04230                                    	addx4	a4, a2, a3
 188:	b66462                                    	s32i	a6, a4, 0x2d8
 18b:	a76462                                    	s32i	a6, a4, 0x29c
 18e:	986462                                    	s32i	a6, a4, 0x260
 191:	896462                                    	s32i	a6, a4, 0x224
 194:	7a6462                                    	s32i	a6, a4, 0x1e8
 197:	6b6462                                    	s32i	a6, a4, 0x1ac
 19a:	5c6462                                    	s32i	a6, a4, 0x170
 19d:	4d6462                                    	s32i	a6, a4, 0x134
 1a0:	23ed                                    	ae_l16si.n	a14, a3, 0
 1a2:	221b                                    	addi.n	a2, a2, 1
 1a4:	822d                                    	ae_sext16	a2, a2
 1a6:	db22e7                                    	blt	a2, a14, 185 <PEQ2_INIT+0x95>
 1a9:	f01d                                    	retw.n

000001ab <PEQ2_INIT+0xbb>:
	...

000001ac <APPLY_PEQ_GAIN>:
 1ac:	004136                                    	entry	a1, 32
 1af:	220720c004c5303e                  	{ ae_l32.i	aed0, a3, 0; nop }
 1b7:	213140                                    	srai	a3, a4, 1
 1ba:	04f556                                    	bnez	a5, 20d <APPLY_PEQ_GAIN+0x61>
 1bd:	0020f0                                    	nop
 1c0:	0e6d6969052629d08700013a0401134f 	{ loopgtz	a3, 208 <APPLY_PEQ_GAIN+0x5c>; ae_l32x2.ip	aed0, a2, 8; nop; ae_slai32	aed2, aed0, 8 }
 1d0:	0e1d221b0d540c038d2b6d280c02015f 	{ nop; nop; ae_mulf32ra.hh	aed5, aed0, aed2; ae_mulf32ra.ll	aed6, aed0, aed2 }
 1e0:	0e6d217b092c0c58c780a13a0401036f 	{ ae_slai64s	aed4, aed6, 3; nop; nop; ae_slai64s	aed3, aed5, 3 }
 1f0:	0ead217b0d3d0ca8c304813c1c01032f 	{ ae_l32x2.ip	aed0, a2, 8; nop; nop; ae_round32x2f48sasym	aed1, aed3, aed4 }
 200:	230700c008e5ba2e                  	{ ae_s32x2.i	aed1, a2, -16; nop }

00000208 <APPLY_PEQ_GAIN+0x5c>:
 208:	f01d                                    	retw.n

0000020a <APPLY_PEQ_GAIN+0x5e>:
 20a:	000000                                        ...

0000020d <APPLY_PEQ_GAIN+0x61>:
 20d:	0020f0                                    	nop

00000210 <APPLY_PEQ_GAIN+0x64>:
 210:	0e6d6969052629d08700013a0401134f 	{ loopgtz	a3, 258 <APPLY_PEQ_GAIN+0xac>; ae_l32x2.ip	aed0, a2, 8; nop; ae_slai32	aed2, aed0, 8 }
 220:	0e1d221b0d540c03992b6d580c02015f 	{ nop; nop; ae_mulf32ra.hh	aed11, aed0, aed2; ae_mulf32ra.ll	aed12, aed0, aed2 }
 230:	0e6d6169514e297a0781613a040153cf 	{ ae_slai64s	aed9, aed12, 7; ae_l32x2.ip	aed10, a2, 8; nop; ae_slai64s	aed8, aed11, 7 }
 240:	0ead297b0d250ca8c31d213c1c01032f 	{ ae_l32x2.ip	aed0, a2, 8; nop; nop; ae_round32x2f48sasym	aed7, aed8, aed9 }
 250:	230700c118e5ba2e                  	{ ae_s32x2.i	aed7, a2, -16; nop }

00000258 <APPLY_PEQ_GAIN+0xac>:
 258:	f01d                                    	retw.n

0000025a <APPLY_PEQ_GAIN+0xae>:
	...

0000025c <BIQUARD_FRAMEBASE>:
 25c:	004136                                    	entry	a1, 32
 25f:	027d                                    	mov.n	a7, a2
 261:	1005134208e6063e                  	{ ae_l32x2.i	aed9, a3, 8; ae_l32x2.i	aed6, a3, 0 }
 269:	0e6d29623b203ed24601213a04018b6f 	{ ae_slai32	aed6, aed6, 8; ae_l32x2.i	aed7, a3, 16; nop; ae_slai32	aed9, aed9, 8 }
 279:	0e552ba00d214ed1c600fe340d09615f 	{ srai	a6, a5, 1; ae_l32x2.i	aed1, a4, 0; ae_sel16i.n	aed8, aed9, aed6, 2; ae_slai32	aed7, aed7, 8 }
 289:	0020f0                                    	nop
 28c:	f03d                                    	nop.n
 28e:	f03d                                    	nop.n
 290:	0e5533a105304e01c51d3c3204c9f64f 	{ loopgtz	a6, 310 <BIQUARD_FRAMEBASE+0xb4>; ae_l32x2.i	aed0, a4, 8; ae_sel16i.n	aed6, aed9, aed6, 0; ae_sel16i	aed7, aed7, aed9, 2 }
 2a0:	0e1d2a1b09a40c0f97af4f611c284d7f 	{ ae_l32.ip	aed13, a7, 4; nop; ae_mulf32ra.ll	aed12, aed1, aed8; ae_mulzaafd32ra.hh.ll	aed11, aed1, aed8 }
 2b0:	0dd527fb0d540c030521b6580c07015f 	{ nop; nop; ae_mulaafd32ra.hh.ll	aed11, aed0, aed7; ae_mulafd32x2ra.fir.h	aed2, aed12, aed0, aed13, aed6 }
 2c0:	0e9507bb08e40cd0c4809c6b002106bf 	{ ae_add64s	aed10, aed11, aed12; nop; ae_sel16i.n	aed1, aed1, aed13, 0; nop }
 2d0:	0e1cea1d1d20710f8aaf4f4c1028006f 	{ ae_pksr32	aed0, aed10, 0; ae_l32.ip	aed3, a7, 4; ae_mulf32ra.ll	aed9, aed1, aed8; ae_mulzaafd32ra.hh.ll	aed5, aed1, aed8 }
 2e0:	0dd527fb0d540c02452076280c07015f 	{ nop; nop; ae_mulaafd32ra.hh.ll	aed5, aed0, aed7; ae_mulafd32x2ra.fir.h	aed2, aed9, aed0, aed3, aed6 }
 2f0:	0ead057b08e40cd0c480813a0801105f 	{ ae_add64s	aed4, aed5, aed9; nop; nop; nop }
 300:	2e82709120000c8e                  	{ ae_pksr32	aed0, aed4, 0; nop; nop }
 308:	0700004004e1992e                  	{ ae_s32x2.ip	aed0, a2, 8; ae_sel16i	aed1, aed1, aed3, 0 }

00000310 <BIQUARD_FRAMEBASE+0xb4>:
 310:	220700c008e5924e                  	{ ae_s32x2.i	aed1, a4, 0; nop }
 318:	220700c000e5964e                  	{ ae_s32x2.i	aed0, a4, 8; nop }
 320:	f01d                                    	retw.n

00000322 <BIQUARD_FRAMEBASE+0xc6>:
	...

00000324 <PEQ_IIR_L>:
 324:	00a136                                    	entry	a1, 80
 327:	029d                                    	mov.n	a9, a2
 329:	08a376                                    	loopgtz	a3, 335 <PEQ_IIR_L+0x11>
 32c:	0988                                    	l32i.n	a8, a9, 0
 32e:	218280                                    	srai	a8, a8, 2
 331:	0989                                    	s32i.n	a8, a9, 0
 333:	994b                                    	addi.n	a9, a9, 4

00000335 <PEQ_IIR_L+0x11>:
 335:	249d                                    	ae_l16si.n	a9, a4, 0
 337:	046d                                    	mov.n	a6, a4
 339:	070c                                    	movi.n	a7, 0
 33b:	5719a6                                    	blti	a9, 1, 396 <PEQ_IIR_L+0x72>

0000033e <PEQ_IIR_L+0x1a>:
 33e:	ff3e81                                    	l32r	a8, 38 (25c <BIQUARD_FRAMEBASE>)
 341:	16c8                                    	l32i.n	a12, a6, 4
 343:	01c9                                    	s32i.n	a12, a1, 0
 345:	1026b2                                    	l32i	a11, a6, 64
 348:	11b9                                    	s32i.n	a11, a1, 4
 34a:	1f26a2                                    	l32i	a10, a6, 124
 34d:	21a9                                    	s32i.n	a10, a1, 8
 34f:	2e2692                                    	l32i	a9, a6, 184
 352:	3199                                    	s32i.n	a9, a1, 12
 354:	3d26f2                                    	l32i	a15, a6, 244
 357:	41f9                                    	s32i.n	a15, a1, 16
 359:	4d26e2                                    	l32i	a14, a6, 0x134
 35c:	81e9                                    	s32i.n	a14, a1, 32
 35e:	5c26d2                                    	l32i	a13, a6, 0x170
 361:	91d9                                    	s32i.n	a13, a1, 36
 363:	01bd                                    	mov.n	a11, a1
 365:	6b26c2                                    	l32i	a12, a6, 0x1ac
 368:	02ad                                    	mov.n	a10, a2
 36a:	a1c9                                    	s32i.n	a12, a1, 40
 36c:	03dd                                    	mov.n	a13, a3
 36e:	7a2692                                    	l32i	a9, a6, 0x1e8
 371:	20c1c2                                    	addi	a12, a1, 32
 374:	b199                                    	s32i.n	a9, a1, 44
 376:	0008e0                                    	callx8	a8
 379:	8198                                    	l32i.n	a9, a1, 32
 37b:	4d6692                                    	s32i	a9, a6, 0x134
 37e:	9188                                    	l32i.n	a8, a1, 36
 380:	5c6682                                    	s32i	a8, a6, 0x170
 383:	a1f8                                    	l32i.n	a15, a1, 40
 385:	6b66f2                                    	s32i	a15, a6, 0x1ac
 388:	b1e8                                    	l32i.n	a14, a1, 44
 38a:	7a66e2                                    	s32i	a14, a6, 0x1e8
 38d:	24dd                                    	ae_l16si.n	a13, a4, 0
 38f:	771b                                    	addi.n	a7, a7, 1
 391:	664b                                    	addi.n	a6, a6, 4
 393:	a727d7                                    	blt	a7, a13, 33e <PEQ_IIR_L+0x1a>
 396:	05dd                                    	mov.n	a13, a5
 398:	ff2981                                    	l32r	a8, 3c (1ac <APPLY_PEQ_GAIN>)
 39b:	03cd                                    	mov.n	a12, a3
 39d:	01d4b2                                    	addmi	a11, a4, 0x100
 3a0:	02ad                                    	mov.n	a10, a2
 3a2:	30cbb2                                    	addi	a11, a11, 48
 3a5:	0008e0                                    	callx8	a8
 3a8:	f01d                                    	retw.n

000003aa <PEQ_IIR_L+0x86>:
	...

000003ac <PEQ_IIR_R>:
 3ac:	00a136                                    	entry	a1, 80
 3af:	029d                                    	mov.n	a9, a2
 3b1:	08a376                                    	loopgtz	a3, 3bd <PEQ_IIR_R+0x11>
 3b4:	0988                                    	l32i.n	a8, a9, 0
 3b6:	218280                                    	srai	a8, a8, 2
 3b9:	0989                                    	s32i.n	a8, a9, 0
 3bb:	994b                                    	addi.n	a9, a9, 4

000003bd <PEQ_IIR_R+0x11>:
 3bd:	249d                                    	ae_l16si.n	a9, a4, 0
 3bf:	046d                                    	mov.n	a6, a4
 3c1:	070c                                    	movi.n	a7, 0
 3c3:	5719a6                                    	blti	a9, 1, 41e <PEQ_IIR_R+0x72>

000003c6 <PEQ_IIR_R+0x1a>:
 3c6:	ff1c81                                    	l32r	a8, 38 (25c <BIQUARD_FRAMEBASE>)
 3c9:	16c8                                    	l32i.n	a12, a6, 4
 3cb:	01c9                                    	s32i.n	a12, a1, 0
 3cd:	1026b2                                    	l32i	a11, a6, 64
 3d0:	11b9                                    	s32i.n	a11, a1, 4
 3d2:	1f26a2                                    	l32i	a10, a6, 124
 3d5:	21a9                                    	s32i.n	a10, a1, 8
 3d7:	2e2692                                    	l32i	a9, a6, 184
 3da:	3199                                    	s32i.n	a9, a1, 12
 3dc:	3d26f2                                    	l32i	a15, a6, 244
 3df:	41f9                                    	s32i.n	a15, a1, 16
 3e1:	8926e2                                    	l32i	a14, a6, 0x224
 3e4:	81e9                                    	s32i.n	a14, a1, 32
 3e6:	9826d2                                    	l32i	a13, a6, 0x260
 3e9:	91d9                                    	s32i.n	a13, a1, 36
 3eb:	01bd                                    	mov.n	a11, a1
 3ed:	a726c2                                    	l32i	a12, a6, 0x29c
 3f0:	02ad                                    	mov.n	a10, a2
 3f2:	a1c9                                    	s32i.n	a12, a1, 40
 3f4:	03dd                                    	mov.n	a13, a3
 3f6:	b62692                                    	l32i	a9, a6, 0x2d8
 3f9:	20c1c2                                    	addi	a12, a1, 32
 3fc:	b199                                    	s32i.n	a9, a1, 44
 3fe:	0008e0                                    	callx8	a8
 401:	8198                                    	l32i.n	a9, a1, 32
 403:	896692                                    	s32i	a9, a6, 0x224
 406:	9188                                    	l32i.n	a8, a1, 36
 408:	986682                                    	s32i	a8, a6, 0x260
 40b:	a1f8                                    	l32i.n	a15, a1, 40
 40d:	a766f2                                    	s32i	a15, a6, 0x29c
 410:	b1e8                                    	l32i.n	a14, a1, 44
 412:	b666e2                                    	s32i	a14, a6, 0x2d8
 415:	24dd                                    	ae_l16si.n	a13, a4, 0
 417:	771b                                    	addi.n	a7, a7, 1
 419:	664b                                    	addi.n	a6, a6, 4
 41b:	a727d7                                    	blt	a7, a13, 3c6 <PEQ_IIR_R+0x1a>
 41e:	05dd                                    	mov.n	a13, a5
 420:	ff0781                                    	l32r	a8, 3c (1ac <APPLY_PEQ_GAIN>)
 423:	03cd                                    	mov.n	a12, a3
 425:	01d4b2                                    	addmi	a11, a4, 0x100
 428:	02ad                                    	mov.n	a10, a2
 42a:	30cbb2                                    	addi	a11, a11, 48
 42d:	0008e0                                    	callx8	a8
 430:	f01d                                    	retw.n

00000432 <PEQ_IIR_R+0x86>:
	...

00000434 <PEQ2_PROC>:
 434:	004136                                    	entry	a1, 32
 437:	c28c                                    	beqz.n	a2, 447 <PEQ2_PROC+0x13>
 439:	ff0181                                    	l32r	a8, 40 (324 <PEQ_IIR_L>)
 43c:	02ad                                    	mov.n	a10, a2
 43e:	04bd                                    	mov.n	a11, a4
 440:	05cd                                    	mov.n	a12, a5
 442:	06dd                                    	mov.n	a13, a6
 444:	0008e0                                    	callx8	a8

00000447 <PEQ2_PROC+0x13>:
 447:	c38c                                    	beqz.n	a3, 457 <PEQ2_PROC+0x23>
 449:	fefe81                                    	l32r	a8, 44 (3ac <PEQ_IIR_R>)
 44c:	06dd                                    	mov.n	a13, a6
 44e:	05cd                                    	mov.n	a12, a5
 450:	04bd                                    	mov.n	a11, a4
 452:	03ad                                    	mov.n	a10, a3
 454:	0008e0                                    	callx8	a8
 457:	f01d                                    	retw.n

00000459 <PEQ2_PROC+0x25>:
 459:	00000000 36000000                                .......

00000460 <__do_global_ctors_aux>:
 460:	004136                                    	entry	a1, 32
 463:	fef981                                    	l32r	a8, 48 (568 <_DYNAMIC+0xd0>)
 466:	7f2882                                    	l32i	a8, a8, 0x1fc
 469:	fef821                                    	l32r	a2, 4c (764 <__CTOR_LIST___103>)
 46c:	0c0826                                    	beqi	a8, -1, 47c <__do_global_ctors_aux+0x1c>
 46f:	02a8                                    	l32i.n	a10, a2, 0

00000471 <__do_global_ctors_aux+0x11>:
 471:	000ae0                                    	callx8	a10
 474:	fcc222                                    	addi	a2, a2, -4
 477:	02a8                                    	l32i.n	a10, a2, 0
 479:	f40a66                                    	bnei	a10, -1, 471 <__do_global_ctors_aux+0x11>
 47c:	f01d                                    	retw.n

0000047e <__do_global_ctors_aux+0x1e>:
	...

00000480 <_fini>:
 480:	008136                                    	entry	a1, 64
 483:	fef381                                    	l32r	a8, 50 (70 <__do_global_dtors_aux>)
 486:	f03d                                    	nop.n
 488:	0008e0                                    	callx8	a8

0000048b <_fini+0xb>:
 48b:	f01d                                    	retw.n
