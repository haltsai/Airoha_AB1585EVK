
Build/lib/pisplit_msbc_enc_position_independent.splitload.lib:     file format elf32-xtensa-le


Disassembly of section .text:

00000000 <_init-0x84>:
       0:	000000d4 00001294 000015dc 00000000     ................
      10:	000015ac 00000000 000015c4 000010b0     ................
      20:	000009b8 00001a40 00001600 00001617     ....@...........
      30:	00000944 0000162d 00001648 00000900     D...-...H.......
      40:	000015f0 00001660 00000690 008f0001     ....`...........
      50:	00000498 00000180 00001820 00000840     ........ ...@...
      60:	0000c801 00001a20 0000019c 000010e0     .... ...........
      70:	00000ae4 0000066c 000017a0 000015b4     ....l...........
      80:	0000009c                                ....

00000084 <_init>:
      84:	008136                                    	entry	a1, 64
      87:	ffde81                                    	l32r	a8, 0 (d4 <frame_dummy>)
      8a:	f03d                                    	nop.n
      8c:	0008e0                                    	callx8	a8
      8f:	ffdd81                                    	l32r	a8, 4 (1294 <__do_global_ctors_aux>)
      92:	f03d                                    	nop.n
      94:	0008e0                                    	callx8	a8
      97:	f01d                                    	retw.n

00000099 <_init+0x15>:
      99:	000000                                        ...

0000009c <__do_global_dtors_aux>:
      9c:	004136                                    	entry	a1, 32
      9f:	ffda21                                    	l32r	a2, 8 (15dc <__do_global_dtors_aux.completed>)
      a2:	000232                                    	l8ui	a3, a2, 0
      a5:	63ec                                    	bnez.n	a3, cf <__do_global_dtors_aux+0x33>

000000a7 <__do_global_dtors_aux+0xb>:
      a7:	1248                                    	l32i.n	a4, a2, 4
      a9:	0438                                    	l32i.n	a3, a4, 0
      ab:	04028424000163ae                  	{ beqz.w15	a3, bc <__do_global_dtors_aux+0x20>; addi	a4, a4, 4 }
      b3:	1249                                    	s32i.n	a4, a2, 4
      b5:	0003e0                                    	callx8	a3
      b8:	fffac6                                    	j	a7 <__do_global_dtors_aux+0xb>

000000bb <__do_global_dtors_aux+0x1f>:
	...

000000bc <__do_global_dtors_aux+0x20>:
      bc:	ffd431                                    	l32r	a3, c (0 <_text_start>)
      bf:	ffd4a1                                    	l32r	a10, 10 (15ac <__FRAME_END__>)
      c2:	438c                                    	beqz.n	a3, ca <__do_global_dtors_aux+0x2e>
      c4:	ffd281                                    	l32r	a8, c (0 <_text_start>)
      c7:	0008e0                                    	callx8	a8
      ca:	130c                                    	movi.n	a3, 1
      cc:	004232                                    	s8i	a3, a2, 0
      cf:	f01d                                    	retw.n

000000d1 <__do_global_dtors_aux+0x35>:
      d1:	000000                                        ...

000000d4 <frame_dummy>:
      d4:	004136                                    	entry	a1, 32
      d7:	ffcf21                                    	l32r	a2, 14 (0 <_text_start>)
      da:	ffcda1                                    	l32r	a10, 10 (15ac <__FRAME_END__>)
      dd:	ffceb1                                    	l32r	a11, 18 (15c4 <frame_dummy.object>)
      e0:	428c                                    	beqz.n	a2, e8 <frame_dummy+0x14>
      e2:	ffcc81                                    	l32r	a8, 14 (0 <_text_start>)
      e5:	0008e0                                    	callx8	a8

000000e8 <frame_dummy+0x14>:
      e8:	f01d                                    	retw.n

000000ea <frame_dummy+0x16>:
	...

000000ec <SBC_GSENC_INIT>:
      ec:	004136                                    	entry	a1, 32
      ef:	ffcb51                                    	l32r	a5, 1c (10b0 <SBC_Encoder_Init>)
      f2:	20a440                                    	or	a10, a4, a4
      f5:	0005e0                                    	callx8	a5
      f8:	1e0c                                    	movi.n	a14, 1
      fa:	04d442                                    	addmi	a4, a4, 0x400
      fd:	051c                                    	movi.n	a5, 16
      ff:	1f3c                                    	movi.n	a15, 49
     101:	9033e0                                    	addx2	a3, a3, a14
     104:	236452                                    	s32i	a5, a4, 140
     107:	256422                                    	s32i	a2, a4, 148
     10a:	2a64f2                                    	s32i	a15, a4, 168
     10d:	316432                                    	s32i	a3, a4, 196
     110:	f01d                                    	retw.n

00000112 <SBC_GSENC_INIT+0x26>:
	...

00000114 <mSBC_GSENC_Process>:
     114:	004136                                    	entry	a1, 32
     117:	ffc271                                    	l32r	a7, 20 (9b8 <mSBC_Encoder_Process>)
     11a:	04d452                                    	addmi	a5, a4, 0x400
     11d:	02ad                                    	mov.n	a10, a2
     11f:	312562                                    	l32i	a6, a5, 196
     122:	03bd                                    	mov.n	a11, a3
     124:	04cd                                    	mov.n	a12, a4
     126:	0007e0                                    	callx8	a7
     129:	2b2522                                    	l32i	a2, a5, 172
     12c:	316562                                    	s32i	a6, a5, 196
     12f:	f01d                                    	retw.n

00000131 <mSBC_GSENC_Process+0x1d>:
     131:	000000                                        ...

00000134 <_start>:
     134:	004136                                    	entry	a1, 32
     137:	ffbb31                                    	l32r	a3, 24 (1a40 <printf_ptr>)
     13a:	0228                                    	l32i.n	a2, a2, 0
     13c:	ffbba1                                    	l32r	a10, 28 (1600 <export_parameter_array+0x10>)
     13f:	ffb7b1                                    	l32r	a11, 1c (10b0 <SBC_Encoder_Init>)
     142:	006322                                    	s32i	a2, a3, 0
     145:	0002e0                                    	callx8	a2
     148:	002322                                    	l32i	a2, a3, 0
     14b:	ffb8a1                                    	l32r	a10, 2c (1617 <export_parameter_array+0x27>)
     14e:	ffb8b1                                    	l32r	a11, 30 (944 <mSBC_SW_SN_init>)
     151:	0002e0                                    	callx8	a2
     154:	002322                                    	l32i	a2, a3, 0
     157:	ffb7a1                                    	l32r	a10, 34 (162d <export_parameter_array+0x3d>)
     15a:	ffb1b1                                    	l32r	a11, 20 (9b8 <mSBC_Encoder_Process>)
     15d:	0002e0                                    	callx8	a2
     160:	002322                                    	l32i	a2, a3, 0
     163:	ffb5a1                                    	l32r	a10, 38 (1648 <export_parameter_array+0x58>)
     166:	ffb5b1                                    	l32r	a11, 3c (900 <mSBC_SW_SN>)
     169:	0002e0                                    	callx8	a2
     16c:	ffb521                                    	l32r	a2, 40 (15f0 <export_parameter_array>)
     16f:	f01d                                    	retw.n

00000171 <_start+0x3d>:
	...

00000180 <memset>:
     180:	004136                                    	entry	a1, 32
     183:	205220                                    	or	a5, a2, a2
     186:	e28c                                    	beqz.n	a2, 198 <memset+0x18>
     188:	202550                                    	or	a2, a5, a5
     18b:	948c                                    	beqz.n	a4, 198 <memset+0x18>
     18d:	078476                                    	loop	a4, 198 <memset+0x18>
     190:	2202a2400080023e                  	{ s16i	a3, a2, 0; addi	a2, a2, 2 }

00000198 <memset+0x18>:
     198:	052d                                    	mov.n	a2, a5
     19a:	f01d                                    	retw.n

0000019c <SbcAnalysisFilter8>:
     19c:	01c136                                    	entry	a1, 224
     19f:	04d232                                    	addmi	a3, a2, 0x400
     1a2:	026d                                    	mov.n	a6, a2
     1a4:	2c2352                                    	l32i	a5, a3, 176
     1a7:	6139                                    	s32i.n	a3, a1, 24
     1a9:	232322                                    	l32i	a2, a3, 140
     1ac:	80a272                                    	movi	a7, 0x280
     1af:	4129                                    	s32i.n	a2, a1, 16
     1b1:	04d632                                    	addmi	a3, a6, 0x400
     1b4:	858d                                    	ae_sext16	a8, a5
     1b6:	b67a                                    	add.n	a11, a6, a7
     1b8:	220740a00c8552ce                  	{ blti.w15	a2, 1, 664 <SbcAnalysisFilter8+0x4c8>; nop }
     1c0:	282372                                    	l32i	a7, a3, 160
     1c3:	fca132                                    	movi	a3, 0x1fc
     1c6:	363a                                    	add.n	a3, a6, a3
     1c8:	eca1a2                                    	movi	a10, 0x1ec
     1cb:	2a6132                                    	s32i	a3, a1, 168
     1ce:	a6aa                                    	add.n	a10, a6, a10
     1d0:	f8a132                                    	movi	a3, 0x1f8
     1d3:	e8a192                                    	movi	a9, 0x1e8
     1d6:	363a                                    	add.n	a3, a6, a3
     1d8:	d0a122                                    	movi	a2, 0x1d0
     1db:	296132                                    	s32i	a3, a1, 164
     1de:	f4a132                                    	movi	a3, 0x1f4
     1e1:	363a                                    	add.n	a3, a6, a3
     1e3:	e0a1d2                                    	movi	a13, 0x1e0
     1e6:	286132                                    	s32i	a3, a1, 160
     1e9:	f0a132                                    	movi	a3, 0x1f0
     1ec:	2661a2                                    	s32i	a10, a1, 152
     1ef:	a69a                                    	add.n	a10, a6, a9
     1f1:	d8a1f2                                    	movi	a15, 0x1d8
     1f4:	363a                                    	add.n	a3, a6, a3
     1f6:	2561a2                                    	s32i	a10, a1, 148
     1f9:	a6da                                    	add.n	a10, a6, a13
     1fb:	d6fa                                    	add.n	a13, a6, a15
     1fd:	262a                                    	add.n	a2, a6, a2
     1ff:	cca1f2                                    	movi	a15, 0x1cc
     202:	e4a1c2                                    	movi	a12, 0x1e4
     205:	276132                                    	s32i	a3, a1, 156
     208:	dca1e2                                    	movi	a14, 0x1dc
     20b:	1f6122                                    	s32i	a2, a1, 124
     20e:	26fa                                    	add.n	a2, a6, a15
     210:	d4a142                                    	movi	a4, 0x1d4
     213:	c8a132                                    	movi	a3, 0x1c8
     216:	96ca                                    	add.n	a9, a6, a12
     218:	c6ea                                    	add.n	a12, a6, a14
     21a:	1e6122                                    	s32i	a2, a1, 120
     21d:	e64a                                    	add.n	a14, a6, a4
     21f:	263a                                    	add.n	a2, a6, a3
     221:	c4a142                                    	movi	a4, 0x1c4
     224:	246192                                    	s32i	a9, a1, 144
     227:	c0a192                                    	movi	a9, 0x1c0
     22a:	1d6122                                    	s32i	a2, a1, 116
     22d:	264a                                    	add.n	a2, a6, a4
     22f:	2361a2                                    	s32i	a10, a1, 140
     232:	bca1a2                                    	movi	a10, 0x1bc
     235:	1c6122                                    	s32i	a2, a1, 112
     238:	269a                                    	add.n	a2, a6, a9
     23a:	2261c2                                    	s32i	a12, a1, 136
     23d:	b8a1c2                                    	movi	a12, 0x1b8
     240:	1b6122                                    	s32i	a2, a1, 108
     243:	26aa                                    	add.n	a2, a6, a10
     245:	2161d2                                    	s32i	a13, a1, 132
     248:	b4a1d2                                    	movi	a13, 0x1b4
     24b:	1a6122                                    	s32i	a2, a1, 104
     24e:	26ca                                    	add.n	a2, a6, a12
     250:	2061e2                                    	s32i	a14, a1, 128
     253:	b0a1e2                                    	movi	a14, 0x1b0
     256:	196122                                    	s32i	a2, a1, 100
     259:	26da                                    	add.n	a2, a6, a13
     25b:	186122                                    	s32i	a2, a1, 96
     25e:	26ea                                    	add.n	a2, a6, a14
     260:	aca1f2                                    	movi	a15, 0x1ac
     263:	176122                                    	s32i	a2, a1, 92
     266:	26fa                                    	add.n	a2, a6, a15
     268:	a8a132                                    	movi	a3, 0x1a8
     26b:	166122                                    	s32i	a2, a1, 88
     26e:	263a                                    	add.n	a2, a6, a3
     270:	a4a142                                    	movi	a4, 0x1a4
     273:	156122                                    	s32i	a2, a1, 84
     276:	264a                                    	add.n	a2, a6, a4
     278:	a0a192                                    	movi	a9, 0x1a0
     27b:	146122                                    	s32i	a2, a1, 80
     27e:	269a                                    	add.n	a2, a6, a9
     280:	9ca1a2                                    	movi	a10, 0x19c
     283:	136122                                    	s32i	a2, a1, 76
     286:	26aa                                    	add.n	a2, a6, a10
     288:	98a1c2                                    	movi	a12, 0x198
     28b:	126122                                    	s32i	a2, a1, 72
     28e:	26ca                                    	add.n	a2, a6, a12
     290:	94a1d2                                    	movi	a13, 0x194
     293:	116122                                    	s32i	a2, a1, 68
     296:	26da                                    	add.n	a2, a6, a13
     298:	90a1e2                                    	movi	a14, 0x190
     29b:	106122                                    	s32i	a2, a1, 64
     29e:	26ea                                    	add.n	a2, a6, a14
     2a0:	8ca1f2                                    	movi	a15, 0x18c
     2a3:	f129                                    	s32i.n	a2, a1, 60
     2a5:	26fa                                    	add.n	a2, a6, a15
     2a7:	88a132                                    	movi	a3, 0x188
     2aa:	e129                                    	s32i.n	a2, a1, 56
     2ac:	263a                                    	add.n	a2, a6, a3
     2ae:	84a142                                    	movi	a4, 0x184
     2b1:	d129                                    	s32i.n	a2, a1, 52
     2b3:	264a                                    	add.n	a2, a6, a4
     2b5:	80a192                                    	movi	a9, 0x180
     2b8:	c129                                    	s32i.n	a2, a1, 48
     2ba:	269a                                    	add.n	a2, a6, a9
     2bc:	7ca1a2                                    	movi	a10, 0x17c
     2bf:	b129                                    	s32i.n	a2, a1, 44
     2c1:	26aa                                    	add.n	a2, a6, a10
     2c3:	78a1c2                                    	movi	a12, 0x178
     2c6:	a129                                    	s32i.n	a2, a1, 40
     2c8:	26ca                                    	add.n	a2, a6, a12
     2ca:	74a1d2                                    	movi	a13, 0x174
     2cd:	9129                                    	s32i.n	a2, a1, 36
     2cf:	26da                                    	add.n	a2, a6, a13
     2d1:	70a1e2                                    	movi	a14, 0x170
     2d4:	8129                                    	s32i.n	a2, a1, 32
     2d6:	26ea                                    	add.n	a2, a6, a14
     2d8:	40c6f2                                    	addi	a15, a6, 64
     2db:	7129                                    	s32i.n	a2, a1, 28
     2dd:	2feb                                    	addi.n	a2, a15, 14
     2df:	113500                                    	slli	a3, a5, 16
     2e2:	90a092                                    	movi	a9, 144
     2e5:	2c61f2                                    	s32i	a15, a1, 176
     2e8:	2b6122                                    	s32i	a2, a1, 172
     2eb:	2b2142                                    	l32i	a4, a1, 172
     2ee:	000886                                    	j	314 <SbcAnalysisFilter8+0x178>

000002f1 <SbcAnalysisFilter8+0x155>:
     2f1:	00000000                                 .....

000002f6 <SbcAnalysisFilter8+0x15a>:
     2f6:	548b                                    	addi.n	a5, a4, 8
     2f8:	4128                                    	l32i.n	a2, a1, 16
     2fa:	113500                                    	slli	a3, a5, 16
     2fd:	51b8                                    	l32i.n	a11, a1, 20
     2ff:	10c772                                    	addi	a7, a7, 16
     302:	858d                                    	ae_sext16	a8, a5
     304:	220b                                    	addi.n	a2, a2, -1
     306:	90a092                                    	movi	a9, 144
     309:	4129                                    	s32i.n	a2, a1, 16
     30b:	20cbb2                                    	addi	a11, a11, 32
     30e:	2b2142                                    	l32i	a4, a1, 172
     311:	34f216                                    	beqz	a2, 664 <SbcAnalysisFilter8+0x4c8>
     314:	2f6132                                    	s32i	a3, a1, 188
     317:	c03980                                    	sub	a3, a9, a8
     31a:	27ed                                    	ae_l16si.n	a14, a7, 0
     31c:	2d6182                                    	s32i	a8, a1, 180
     31f:	90f340                                    	addx2	a15, a3, a4
     322:	2e6152                                    	s32i	a5, a1, 184
     325:	fccf92                                    	addi	a9, a15, -4
     328:	6fed                                    	ae_s16i.n	a14, a15, 0
     32a:	378d                                    	ae_l16si.n	a8, a7, 2
     32c:	f8cfc2                                    	addi	a12, a15, -8
     32f:	798d                                    	ae_s16i.n	a8, a9, 2
     331:	f6cfd2                                    	addi	a13, a15, -10
     334:	029722                                    	l16si	a2, a7, 4
     337:	f4cfe2                                    	addi	a14, a15, -12
     33a:	692d                                    	ae_s16i.n	a2, a9, 0
     33c:	2c2192                                    	l32i	a9, a1, 176
     33f:	039742                                    	l16si	a4, a7, 6
     342:	f2cf52                                    	addi	a5, a15, -14
     345:	7c4d                                    	ae_s16i.n	a4, a12, 2
     347:	4f0c                                    	movi.n	a15, 4
     349:	049722                                    	l16si	a2, a7, 8
     34c:	903390                                    	addx2	a3, a3, a9
     34f:	6c2d                                    	ae_s16i.n	a2, a12, 0
     351:	059742                                    	l16si	a4, a7, 10
     354:	6d4d                                    	ae_s16i.n	a4, a13, 0
     356:	069722                                    	l16si	a2, a7, 12
     359:	6e2d                                    	ae_s16i.n	a2, a14, 0
     35b:	062d                                    	mov.n	a2, a6
     35d:	ff3941                                    	l32r	a4, 44 (1660 <gas32CoeffFor8SBs>)
     360:	079782                                    	l16si	a8, a7, 14
     363:	658d                                    	ae_s16i.n	a8, a5, 0
     365:	178f76                                    	loop	a15, 380 <SbcAnalysisFilter8+0x1e4>
     368:	0301f0                                    	rsr.lend	a15
     36b:	1300f0                                    	wsr.lbeg	a15
     36e:	ff38f1                                    	l32r	a15, 50 (498 <SbcAnalysisFilter8+0x2fc>)
     371:	0020f0                                    	nop
     374:	1301f0                                    	wsr.lend	a15
     377:	002000                                    	isync
     37a:	0302f0                                    	rsr.lcount	a15
     37d:	01cff2                                    	addi	a15, a15, 1
     380:	0ead25700c604cd0c40081380401835f 	{ addi	a5, a3, 32; ae_la128.pp	u0, a4; nop; nop }
     390:	0000814957c60f0046031323b000804f 	{ ae_la32x2x2_ip	aed0, aed6, u0, a4; nop; nop }
     3a0:	210534c014a5b13e                  	{ ae_l16x4.ip	aed2, a3, 8; ae_l32.ip	aed5, a4, 4 }
     3a8:	0ead25700d214cd0c500813c0401e65f 	{ ae_l16x4.ip	aed7, a5, 32; ae_la128.pp	u0, a4; nop; nop }
     3b8:	0e6d3963252947004508e13c04016a5f 	{ ae_l16x4.ip	aed9, a5, 32; ae_la32x2x2_ip	aed4, aed3, u0, a4; nop; ae_sel16i	aed1, aed2, aed7, 9 }
     3c8:	0e0111c200e2005e                  	{ ae_l16x4.i	aed8, a5, 0; ae_sel16i	aed2, aed2, aed7, 11 }
     3d0:	0e1d00a43be85c2700af1e080801498f 	{ ae_sel16i	aed4, aed9, aed8, 9; ae_l16x4.i	aed7, a5, 32; ae_mulzaad32x16.h3.l2	aed1, aed0, aed1; ae_mulzaad32x16.h1.l0	aed0, aed4, aed1 }
     3e0:	0e14c7fd23e0411b00ab2e0808c43b8f 	{ ae_sel16i	aed3, aed9, aed8, 11; ae_l32.ip	aed4, a4, 4; ae_mulaad32x16.h3.l2	aed1, aed6, aed4; ae_mulaad32x16.h1.l0	aed0, aed3, aed4 }
     3f0:	0e1527fb0d5c0c20412b22080ca7015f 	{ nop; nop; ae_mula32x16.l3	aed1, aed5, aed7; ae_mula32x16.l2	aed0, aed4, aed7 }
     400:	0ead217b0d4c0cb0c30001380c01015f 	{ nop; nop; nop; ae_round32x2f48ssym	aed0, aed1, aed0 }
     410:	230700c004e5992e                  	{ ae_s32x2.ip	aed0, a2, 8; nop }
     418:	0ead25700d404cd0c50081380c01015f 	{ nop; ae_la128.pp	u0, a4; nop; nop }
     428:	0000814957c60f0046031323a000804f 	{ ae_la32x2x2_ip	aed0, aed4, u0, a4; nop; nop }
     438:	230720c00cc5314e                  	{ ae_l32.ip	aed1, a4, 4; nop }
     440:	0e9d24b00d404cd0c5009e000c02015f 	{ nop; ae_la128.pp	u0, a4; ae_mulzaad32x16.h3.l2	aed0, aed0, aed2; nop }
     450:	0e9527fb0c240cd0c4008e050083534f 	{ ae_la32x2x2_ip	aed5, aed4, u0, a4; nop; ae_mulaad32x16.h3.l2	aed0, aed4, aed3; nop }
     460:	1c8342033427b13e                  	{ nop; nop; ae_mula32x16.l1	aed0, aed1, aed7 }
     468:	2a834d0b34a2b13e                  	{ nop; nop; ae_mulzaad32x16.h1.l0	aed1, aed5, aed2 }
     470:	1c828e0f2083214e                  	{ ae_l32.ip	aed2, a4, 4; nop; ae_mulaad32x16.h1.l0	aed1, aed4, aed3 }
     478:	1c83410b3447b13e                  	{ nop; nop; ae_mula32x16.l0	aed1, aed2, aed7 }
     480:	0ead217b0d440cb0c30021380c01015f 	{ nop; nop; nop; ae_round32x2f48ssym	aed0, aed0, aed1 }
     490:	230700c004e5992e                  	{ ae_s32x2.ip	aed0, a2, 8; nop }

00000498 <SbcAnalysisFilter8+0x2fc>:
     498:	feec21                                    	l32r	a2, 48 (690 <SBC_FastIDCT8>)
     49b:	06ad                                    	mov.n	a10, a6
     49d:	51b9                                    	s32i.n	a11, a1, 20
     49f:	0002e0                                    	callx8	a2
     4a2:	feea21                                    	l32r	a2, 4c (8f0001 <_end+0x8ee571>)
     4a5:	c87c                                    	movi.n	a8, -4
     4a7:	2f2132                                    	l32i	a3, a1, 188
     4aa:	2a21a2                                    	l32i	a10, a1, 168
     4ad:	2d2142                                    	l32i	a4, a1, 180
     4b0:	292192                                    	l32i	a9, a1, 164
     4b3:	2e2152                                    	l32i	a5, a1, 184
     4b6:	2821b2                                    	l32i	a11, a1, 160
     4b9:	2621e2                                    	l32i	a14, a1, 152
     4bc:	230700981fc5b32e                  	{ blt.w15	a3, a2, 2f6 <SbcAnalysisFilter8+0x15a>; nop }
     4c4:	2721c2                                    	l32i	a12, a1, 156
     4c7:	e42150                                    	extui	a2, a5, 1, 15
     4ca:	2521f2                                    	l32i	a15, a1, 148
     4cd:	c02820                                    	sub	a2, a8, a2
     4d0:	242182                                    	l32i	a8, a1, 144
     4d3:	a032a0                                    	addx4	a3, a2, a10
     4d6:	a04290                                    	addx4	a4, a2, a9
     4d9:	0338                                    	l32i.n	a3, a3, 0
     4db:	0448                                    	l32i.n	a4, a4, 0
     4dd:	7f6632                                    	s32i	a3, a6, 0x1fc
     4e0:	a032b0                                    	addx4	a3, a2, a11
     4e3:	0338                                    	l32i.n	a3, a3, 0
     4e5:	7e6642                                    	s32i	a4, a6, 0x1f8
     4e8:	7d6632                                    	s32i	a3, a6, 0x1f4
     4eb:	a032e0                                    	addx4	a3, a2, a14
     4ee:	a042c0                                    	addx4	a4, a2, a12
     4f1:	0338                                    	l32i.n	a3, a3, 0
     4f3:	04d8                                    	l32i.n	a13, a4, 0
     4f5:	2221b2                                    	l32i	a11, a1, 136
     4f8:	7b6632                                    	s32i	a3, a6, 0x1ec
     4fb:	a03280                                    	addx4	a3, a2, a8
     4fe:	a042f0                                    	addx4	a4, a2, a15
     501:	232192                                    	l32i	a9, a1, 140
     504:	0338                                    	l32i.n	a3, a3, 0
     506:	2021e2                                    	l32i	a14, a1, 128
     509:	0503a4460880663e                  	{ s32i	a3, a6, 0x1e4; l32i	a4, a4, 0 }
     511:	a032b0                                    	addx4	a3, a2, a11
     514:	2121c2                                    	l32i	a12, a1, 132
     517:	7a6642                                    	s32i	a4, a6, 0x1e8
     51a:	a04290                                    	addx4	a4, a2, a9
     51d:	0338                                    	l32i.n	a3, a3, 0
     51f:	1e2182                                    	l32i	a8, a1, 120
     522:	0b03a4451880663e                  	{ s32i	a3, a6, 0x1dc; l32i	a10, a4, 0 }
     52a:	a032e0                                    	addx4	a3, a2, a14
     52d:	1f21f2                                    	l32i	a15, a1, 124
     530:	0338                                    	l32i.n	a3, a3, 0
     532:	a042c0                                    	addx4	a4, a2, a12
     535:	7c66d2                                    	s32i	a13, a6, 0x1f0
     538:	1c21b2                                    	l32i	a11, a1, 112
     53b:	0d03a4450880763e                  	{ s32i	a3, a6, 0x1d4; l32i	a13, a4, 0 }
     543:	a03280                                    	addx4	a3, a2, a8
     546:	1d2192                                    	l32i	a9, a1, 116
     549:	a042f0                                    	addx4	a4, a2, a15
     54c:	0338                                    	l32i.n	a3, a3, 0
     54e:	0448                                    	l32i.n	a4, a4, 0
     550:	1a21e2                                    	l32i	a14, a1, 104
     553:	736632                                    	s32i	a3, a6, 0x1cc
     556:	a032b0                                    	addx4	a3, a2, a11
     559:	746642                                    	s32i	a4, a6, 0x1d0
     55c:	1b21c2                                    	l32i	a12, a1, 108
     55f:	0338                                    	l32i.n	a3, a3, 0
     561:	a04290                                    	addx4	a4, a2, a9
     564:	7866a2                                    	s32i	a10, a6, 0x1e0
     567:	182182                                    	l32i	a8, a1, 96
     56a:	0b03a4440880663e                  	{ s32i	a3, a6, 0x1c4; l32i	a10, a4, 0 }
     572:	a032e0                                    	addx4	a3, a2, a14
     575:	1921f2                                    	l32i	a15, a1, 100
     578:	0338                                    	l32i.n	a3, a3, 0
     57a:	a042c0                                    	addx4	a4, a2, a12
     57d:	7666d2                                    	s32i	a13, a6, 0x1d8
     580:	1621b2                                    	l32i	a11, a1, 88
     583:	0d03a4431880763e                  	{ s32i	a3, a6, 0x1bc; l32i	a13, a4, 0 }
     58b:	a03280                                    	addx4	a3, a2, a8
     58e:	172192                                    	l32i	a9, a1, 92
     591:	a042f0                                    	addx4	a4, a2, a15
     594:	0338                                    	l32i.n	a3, a3, 0
     596:	0448                                    	l32i.n	a4, a4, 0
     598:	1421e2                                    	l32i	a14, a1, 80
     59b:	6d6632                                    	s32i	a3, a6, 0x1b4
     59e:	a032b0                                    	addx4	a3, a2, a11
     5a1:	6e6642                                    	s32i	a4, a6, 0x1b8
     5a4:	1521c2                                    	l32i	a12, a1, 84
     5a7:	0338                                    	l32i.n	a3, a3, 0
     5a9:	a04290                                    	addx4	a4, a2, a9
     5ac:	7266a2                                    	s32i	a10, a6, 0x1c8
     5af:	122182                                    	l32i	a8, a1, 72
     5b2:	0b03a4421880663e                  	{ s32i	a3, a6, 0x1ac; l32i	a10, a4, 0 }
     5ba:	a032e0                                    	addx4	a3, a2, a14
     5bd:	1321f2                                    	l32i	a15, a1, 76
     5c0:	0338                                    	l32i.n	a3, a3, 0
     5c2:	a042c0                                    	addx4	a4, a2, a12
     5c5:	7066d2                                    	s32i	a13, a6, 0x1c0
     5c8:	1021b2                                    	l32i	a11, a1, 64
     5cb:	0d03a4420880763e                  	{ s32i	a3, a6, 0x1a4; l32i	a13, a4, 0 }
     5d3:	a03280                                    	addx4	a3, a2, a8
     5d6:	112192                                    	l32i	a9, a1, 68
     5d9:	a042f0                                    	addx4	a4, a2, a15
     5dc:	0338                                    	l32i.n	a3, a3, 0
     5de:	0448                                    	l32i.n	a4, a4, 0
     5e0:	e1e8                                    	l32i.n	a14, a1, 56
     5e2:	676632                                    	s32i	a3, a6, 0x19c
     5e5:	a032b0                                    	addx4	a3, a2, a11
     5e8:	686642                                    	s32i	a4, a6, 0x1a0
     5eb:	f1c8                                    	l32i.n	a12, a1, 60
     5ed:	0338                                    	l32i.n	a3, a3, 0
     5ef:	a04290                                    	addx4	a4, a2, a9
     5f2:	6c66a2                                    	s32i	a10, a6, 0x1b0
     5f5:	c188                                    	l32i.n	a8, a1, 48
     5f7:	0b03a4410880663e                  	{ s32i	a3, a6, 0x194; l32i	a10, a4, 0 }
     5ff:	a032e0                                    	addx4	a3, a2, a14
     602:	d1f8                                    	l32i.n	a15, a1, 52
     604:	0338                                    	l32i.n	a3, a3, 0
     606:	a042c0                                    	addx4	a4, a2, a12
     609:	6a66d2                                    	s32i	a13, a6, 0x1a8
     60c:	a1b8                                    	l32i.n	a11, a1, 40
     60e:	0d03a4401880763e                  	{ s32i	a3, a6, 0x18c; l32i	a13, a4, 0 }
     616:	a03280                                    	addx4	a3, a2, a8
     619:	b198                                    	l32i.n	a9, a1, 44
     61b:	a042f0                                    	addx4	a4, a2, a15
     61e:	0338                                    	l32i.n	a3, a3, 0
     620:	0448                                    	l32i.n	a4, a4, 0
     622:	91c8                                    	l32i.n	a12, a1, 36
     624:	616632                                    	s32i	a3, a6, 0x184
     627:	81e8                                    	l32i.n	a14, a1, 32
     629:	a032b0                                    	addx4	a3, a2, a11
     62c:	71f8                                    	l32i.n	a15, a1, 28
     62e:	626642                                    	s32i	a4, a6, 0x188
     631:	a04290                                    	addx4	a4, a2, a9
     634:	0338                                    	l32i.n	a3, a3, 0
     636:	6666a2                                    	s32i	a10, a6, 0x198
     639:	0b03a4471880463e                  	{ s32i	a3, a6, 0x17c; l32i	a10, a4, 0 }
     641:	a032e0                                    	addx4	a3, a2, a14
     644:	a042c0                                    	addx4	a4, a2, a12
     647:	6466d2                                    	s32i	a13, a6, 0x190
     64a:	a022f0                                    	addx4	a2, a2, a15
     64d:	04d8                                    	l32i.n	a13, a4, 0
     64f:	0338                                    	l32i.n	a3, a3, 0
     651:	0228                                    	l32i.n	a2, a2, 0
     653:	050c                                    	movi.n	a5, 0
     655:	6066a2                                    	s32i	a10, a6, 0x180
     658:	5e66d2                                    	s32i	a13, a6, 0x178
     65b:	5d6632                                    	s32i	a3, a6, 0x174
     65e:	5c6622                                    	s32i	a2, a6, 0x170
     661:	ff24c6                                    	j	2f8 <SbcAnalysisFilter8+0x15c>

00000664 <SbcAnalysisFilter8+0x4c8>:
     664:	6128                                    	l32i.n	a2, a1, 24
     666:	2c6282                                    	s32i	a8, a2, 176
     669:	f01d                                    	retw.n

0000066b <SbcAnalysisFilter8+0x4cf>:
	...

0000066c <SbcAnalysisInit>:
     66c:	004136                                    	entry	a1, 32
     66f:	fe7931                                    	l32r	a3, 54 (180 <memset>)
     672:	40c2a2                                    	addi	a10, a2, 64
     675:	0b0c                                    	movi.n	a11, 0
     677:	e0a0c2                                    	movi	a12, 224
     67a:	040c                                    	movi.n	a4, 0
     67c:	04d222                                    	addmi	a2, a2, 0x400
     67f:	0003e0                                    	callx8	a3
     682:	2c6242                                    	s32i	a4, a2, 176
     685:	f01d                                    	retw.n

00000687 <SbcAnalysisInit+0x1b>:
	...

00000690 <SBC_FastIDCT8>:
     690:	004136                                    	entry	a1, 32
     693:	fe7141                                    	l32r	a4, 58 (1820 <NEWgas16AnalDCTcoeff8>)
     696:	08a062                                    	movi	a6, 8
     699:	fcc222                                    	addi	a2, a2, -4
     69c:	ffc6f2                                    	addi	a15, a6, -1
     69f:	206220                                    	or	a6, a2, a2
     6a2:	f8c442                                    	addi	a4, a4, -8
     6a5:	1205144000e2164e                  	{ ae_l32x2.i	aed0, a4, 8; ae_l32x2.i	aed2, a4, 24 }
     6ad:	0e6d29641d214ec00600013d1001424f 	{ ae_l32x2.i	aed1, a4, 16; ae_l32x2.i	aed3, a4, 32; nop; ae_slai24	aed0, aed0, 8 }
     6bd:	0e552ba74d214ec306003d0509a0164f 	{ ae_l32x2.i	aed4, a4, 40; ae_l32x2.i	aed9, a4, 56; ae_sel16i.n	aed13, aed0, aed0, 1; ae_slai24	aed12, aed1, 8 }
     6cd:	0e552ba63c604ec206007d62056c045f 	{ addi	a5, a4, 64; ae_l32x2.i	aed7, a4, 48; ae_sel16i.n	aed11, aed12, aed12, 1; ae_slai24	aed8, aed3, 8 }
     6dd:	0e6d29602b205ec28600413804018b4f 	{ ae_slai24	aed6, aed4, 8; ae_l32x2.i	aed5, a5, 0; nop; ae_slai24	aed10, aed2, 8 }
     6ed:	0e54eba2030255c10600fd4004e8cb9f 	{ ae_slai24	aed3, aed9, 8; addi	a5, a5, 64; ae_sel16i.n	aed7, aed8, aed8, 1; ae_slai24	aed4, aed7, 8 }
     6fd:	150025c318e9647e                  	{ nop; ae_sel16i	aed9, aed10, aed10, 1 }
     705:	178f76                                    	loop	a15, 720 <SBC_FastIDCT8+0x90>
     708:	0301f0                                    	rsr.lend	a15
     70b:	1300f0                                    	wsr.lbeg	a15
     70e:	fe53f1                                    	l32r	a15, 5c (840 <SBC_FastIDCT8+0x1b0>)
     711:	0020f0                                    	nop
     714:	1301f0                                    	wsr.lend	a15
     717:	002000                                    	isync
     71a:	0302f0                                    	rsr.lcount	a15
     71d:	01cff2                                    	addi	a15, a15, 1
     720:	0e6ce9790d2561c08600a13d0801ae5f 	{ ae_l32x2.i	aed14, a5, -56; ae_l32m.iu	aed1, a6, 4; nop; ae_slai24	aed2, aed5, 8 }
     730:	0e5d339b0d2c0c014598cc08102d4d6f 	{ ae_l32m.iu	aed13, a6, 4; nop; ae_mulq32sp16s.l	aed1, aed1, aed13; ae_sel16i	aed5, aed6, aed6, 1 }
     740:	0e9ce4f96b0461d0c4008d0805a00bef 	{ ae_slai24	aed0, aed14, 8; ae_l32m.iu	aed13, a6, 4; ae_mulaq32sp16s.l	aed1, aed13, aed0; nop }
     750:	0e9cc4f95be461d0c4008d0801abd10f 	{ ae_sel16i	aed13, aed0, aed0, 1; ae_l32m.iu	aed11, a6, 4; ae_mulaq32sp16s.l	aed1, aed13, aed11; nop }
     760:	20828e0b316cb16e                  	{ ae_l32m.iu	aed11, a6, 4; nop; ae_mulaq32sp16s.l	aed1, aed11, aed12 }
     768:	20828e0b3169916e                  	{ ae_l32m.iu	aed9, a6, 4; nop; ae_mulaq32sp16s.l	aed1, aed11, aed9 }
     770:	0e5cf0f95d2d610244108d0d112aaa5f 	{ ae_l32x2.i	aed10, a5, -48; ae_l32m.iu	aed11, a6, 4; ae_mulaq32sp16s.l	aed1, aed9, aed10; ae_sel16i	aed9, aed4, aed4, 1 }
     780:	0e9ce4f93b6461d0c4008d0805670baf 	{ ae_slai24	aed12, aed10, 8; ae_l32m.iu	aed7, a6, 4; ae_mulaq32sp16s.l	aed1, aed11, aed7; nop }
     790:	0e9cc4f93be461d0c4808d0900e8b1cf 	{ ae_sel16i	aed11, aed12, aed12, 1; ae_l32m.iu	aed7, a6, 4; ae_mulaq32sp16s.l	aed1, aed7, aed8; nop }
     7a0:	20828e0b30e5516e                  	{ ae_l32m.iu	aed5, a6, 4; nop; ae_mulaq32sp16s.l	aed1, aed7, aed5 }
     7a8:	0e9ce4f9352561d0c4008d0d18a6665f 	{ ae_l32x2.i	aed5, a5, -40; ae_l32m.iu	aed6, a6, 4; ae_mulaq32sp16s.l	aed1, aed5, aed6; nop }
     7b8:	0e5cf0f9334c6101440c6d0804c98b5f 	{ ae_slai24	aed10, aed5, 8; ae_l32m.iu	aed6, a6, 4; ae_mulaq32sp16s.l	aed1, aed6, aed9; ae_sel16i	aed5, aed3, aed3, 1 }
     7c8:	0e9cc4f923e461d0c4808d0810c491af 	{ ae_sel16i	aed9, aed10, aed10, 1; ae_l32m.iu	aed4, a6, 4; ae_mulaq32sp16s.l	aed1, aed6, aed4; nop }
     7d8:	20828e0b3085416e                  	{ ae_l32m.iu	aed4, a6, 4; nop; ae_mulaq32sp16s.l	aed1, aed4, aed5 }
     7e0:	0e5cf0f91d2d610104084d0d0083465f 	{ ae_l32x2.i	aed5, a5, 0; ae_l32m.iu	aed3, a6, 4; ae_mulaq32sp16s.l	aed1, aed4, aed3; ae_sel16i	aed4, aed2, aed2, 1 }
     7f0:	0e9ce4f9252561d0c4008d0d0064f25f 	{ ae_l32x2.i	aed3, a5, -32; ae_l32m.iu	aed4, a6, 4; ae_mulaq32sp16s.l	aed1, aed3, aed4; nop }
     800:	0e5d28ef15215ec207006d0f1082622f 	{ or	a6, a2, a2; ae_l32x2.i	aed2, a5, -8; ae_mulaq32sp16s.l	aed1, aed4, aed2; ae_slai24	aed8, aed3, 8 }
     810:	0e552bae15215ec0c6005d4508e8365f 	{ ae_l32x2.i	aed4, a5, -24; ae_l32x2.i	aed2, a5, -16; ae_sel16i.n	aed7, aed8, aed8, 1; ae_slai24	aed3, aed2, 8 }
     820:	0e6ce962052255c18600813b0c01641f 	{ ae_trunca32q48	a4, aed1; addi	a5, a5, 64; nop; ae_slai24	aed6, aed4, 8 }
     830:	0e6cc96021e233c10700413d0401034f 	{ s32i	a4, a3, 0; addi	a3, a3, 4; nop; ae_slai24	aed4, aed2, 8 }

00000840 <SBC_FastIDCT8+0x1b0>:
     840:	0e54d3b90bec6103c40c7d2111c4216f 	{ ae_sel16i	aed2, aed6, aed6, 1; ae_l32m.iu	aed1, a6, 4; ae_sel16i.n	aed14, aed4, aed4, 1; ae_sel16i	aed15, aed3, aed3, 1 }
     850:	0e5d2b9b0d240cc14780ac0a102d416f 	{ ae_l32m.iu	aed17, a6, 4; nop; ae_mulq32sp16s.l	aed1, aed1, aed13; ae_slai24	aed5, aed5, 8 }
     860:	0e5d30fb0d2c0c040594ad0a1220816f 	{ ae_l32m.iu	aed18, a6, 4; nop; ae_mulaq32sp16s.l	aed1, aed17, aed0; ae_sel16i	aed16, aed5, aed5, 1 }
     870:	20828e0b364b316e                  	{ ae_l32m.iu	aed19, a6, 4; nop; ae_mulaq32sp16s.l	aed1, aed18, aed11 }
     878:	20828e0b366c416e                  	{ ae_l32m.iu	aed20, a6, 4; nop; ae_mulaq32sp16s.l	aed1, aed19, aed12 }
     880:	20828e0b3689516e                  	{ ae_l32m.iu	aed21, a6, 4; nop; ae_mulaq32sp16s.l	aed1, aed20, aed9 }
     888:	20828e0b36aa616e                  	{ ae_l32m.iu	aed22, a6, 4; nop; ae_mulaq32sp16s.l	aed1, aed21, aed10 }
     890:	20828e0b36c7716e                  	{ ae_l32m.iu	aed23, a6, 4; nop; ae_mulaq32sp16s.l	aed1, aed22, aed7 }
     898:	20828e0b36e8816e                  	{ ae_l32m.iu	aed24, a6, 4; nop; ae_mulaq32sp16s.l	aed1, aed23, aed8 }
     8a0:	20828e0b3702916e                  	{ ae_l32m.iu	aed25, a6, 4; nop; ae_mulaq32sp16s.l	aed1, aed24, aed2 }
     8a8:	20828e0b3726a16e                  	{ ae_l32m.iu	aed26, a6, 4; nop; ae_mulaq32sp16s.l	aed1, aed25, aed6 }
     8b0:	20828e0b374eb16e                  	{ ae_l32m.iu	aed27, a6, 4; nop; ae_mulaq32sp16s.l	aed1, aed26, aed14 }
     8b8:	20828e0b3764c16e                  	{ ae_l32m.iu	aed28, a6, 4; nop; ae_mulaq32sp16s.l	aed1, aed27, aed4 }
     8c0:	20828e0b378fd16e                  	{ ae_l32m.iu	aed29, a6, 4; nop; ae_mulaq32sp16s.l	aed1, aed28, aed15 }
     8c8:	20828e0b37a3e16e                  	{ ae_l32m.iu	aed30, a6, 4; nop; ae_mulaq32sp16s.l	aed1, aed29, aed3 }
     8d0:	20828e0b37d0f16e                  	{ ae_l32m.iu	aed31, a6, 4; nop; ae_mulaq32sp16s.l	aed1, aed30, aed16 }
     8d8:	20834e0b37e5b13e                  	{ nop; nop; ae_mulaq32sp16s.l	aed1, aed31, aed5 }
     8e0:	0ead257b0d240cd0c400813b0c01621f 	{ ae_trunca32q48	a2, aed1; nop; nop; nop }
     8f0:	0329                                    	s32i.n	a2, a3, 0
     8f2:	f01d                                    	retw.n

000008f4 <SBC_FastIDCT8+0x264>:
	...

00000900 <mSBC_SW_SN>:
     900:	004136                                    	entry	a1, 32
     903:	434d                                    	ae_l16ui.n	a4, a3, 0
     905:	f57c                                    	movi.n	a5, -1
     907:	aba282                                    	movi	a8, 0x2ab
     90a:	f45050                                    	extui	a5, a5, 0, 16
     90d:	909880                                    	addx2	a9, a8, a8
     910:	641b                                    	addi.n	a6, a4, 1
     912:	fdc442                                    	addi	a4, a4, -3
     915:	934640                                    	movnez	a4, a6, a4
     918:	39a662                                    	movi	a6, 0x639
     91b:	634d                                    	ae_s16i.n	a4, a3, 0
     91d:	b07660                                    	addx8	a7, a6, a6
     920:	110547                                    	bnone	a5, a4, 935 <mSBC_SW_SN+0x35>
     923:	943d                                    	ae_zext16	a3, a4
     925:	01a842                                    	movi	a4, 0xfffff801
     928:	0d2326                                    	beqi	a3, 2, 939 <mSBC_SW_SN+0x39>
     92b:	f45040                                    	extui	a5, a4, 0, 16
     92e:	0e1366                                    	bnei	a3, 1, 940 <mSBC_SW_SN+0x40>
     931:	627d                                    	ae_s16i.n	a7, a2, 0
     933:	f01d                                    	retw.n

00000935 <mSBC_SW_SN+0x35>:
     935:	629d                                    	ae_s16i.n	a9, a2, 0
     937:	f01d                                    	retw.n

00000939 <mSBC_SW_SN+0x39>:
     939:	fdc931                                    	l32r	a3, 60 (c801 <_end+0xad71>)
     93c:	623d                                    	ae_s16i.n	a3, a2, 0
     93e:	f01d                                    	retw.n

00000940 <mSBC_SW_SN+0x40>:
     940:	625d                                    	ae_s16i.n	a5, a2, 0
     942:	f01d                                    	retw.n

00000944 <mSBC_SW_SN_init>:
     944:	004136                                    	entry	a1, 32
     947:	030c                                    	movi.n	a3, 0
     949:	623d                                    	ae_s16i.n	a3, a2, 0
     94b:	f01d                                    	retw.n

0000094d <mSBC_SW_SN_init+0x9>:
     94d:	000000                                        ...

00000950 <mybit_pack>:
     950:	004136                                    	entry	a1, 32
     953:	0258                                    	l32i.n	a5, a2, 0
     955:	061c                                    	movi.n	a6, 16
     957:	021272                                    	l16ui	a7, a2, 4
     95a:	938d                                    	ae_zext16	a8, a3
     95c:	c06650                                    	sub	a6, a6, a5
     95f:	32b8                                    	l32i.n	a11, a2, 12
     961:	c09460                                    	sub	a9, a4, a6
     964:	14a467                                    	bge	a4, a6, 97c <mybit_pack+0x2c>
     967:	021262                                    	l16ui	a6, a2, 4
     96a:	401400                                    	ssl	a4
     96d:	f54a                                    	add.n	a15, a5, a4
     96f:	a16600                                    	sll	a6, a6
     972:	203630                                    	or	a3, a6, a3
     975:	025232                                    	s16i	a3, a2, 4
     978:	02f9                                    	s32i.n	a15, a2, 0
     97a:	f01d                                    	retw.n

0000097c <mybit_pack+0x2c>:
     97c:	400900                                    	ssr	a9
     97f:	fdb9d1                                    	l32r	a13, 64 (1a20 <MASK_bitp>)
     982:	915080                                    	srl	a5, a8
     985:	401600                                    	ssl	a6
     988:	a16700                                    	sll	a6, a7
     98b:	22e8                                    	l32i.n	a14, a2, 8
     98d:	205650                                    	or	a5, a6, a5
     990:	7b2b                                    	addi.n	a7, a11, 2
     992:	956d                                    	ae_zext16	a6, a5
     994:	118680                                    	slli	a8, a6, 8
     997:	416860                                    	srli	a6, a6, 8
     99a:	c86a                                    	add.n	a12, a8, a6
     99c:	025252                                    	s16i	a5, a2, 4
     99f:	9069d0                                    	addx2	a6, a9, a13
     9a2:	6bcd                                    	ae_s16i.n	a12, a11, 0
     9a4:	266d                                    	ae_l16si.n	a6, a6, 0
     9a6:	5e1b                                    	addi.n	a5, a14, 1
     9a8:	3279                                    	s32i.n	a7, a2, 12
     9aa:	2259                                    	s32i.n	a5, a2, 8
     9ac:	10f630                                    	and	a15, a6, a3
     9af:	0252f2                                    	s16i	a15, a2, 4
     9b2:	0299                                    	s32i.n	a9, a2, 0
     9b4:	f01d                                    	retw.n

000009b6 <mybit_pack+0x66>:
	...

000009b8 <mSBC_Encoder_Process>:
     9b8:	004136                                    	entry	a1, 32
     9bb:	04d452                                    	addmi	a5, a4, 0x400
     9be:	060c                                    	movi.n	a6, 0
     9c0:	286522                                    	s32i	a2, a5, 160
     9c3:	04ad                                    	mov.n	a10, a4
     9c5:	fda821                                    	l32r	a2, 68 (19c <SbcAnalysisFilter8>)
     9c8:	296532                                    	s32i	a3, a5, 164
     9cb:	232532                                    	l32i	a3, a5, 140
     9ce:	2f6562                                    	s32i	a6, a5, 188
     9d1:	5c5562                                    	s16i	a6, a5, 184
     9d4:	2d6562                                    	s32i	a6, a5, 180
     9d7:	0002e0                                    	callx8	a2
     9da:	190c                                    	movi.n	a9, 1
     9dc:	1a0c                                    	movi.n	a10, 1
     9de:	40a222                                    	movi	a2, 0x240
     9e1:	03fd                                    	mov.n	a15, a3
     9e3:	0e0c                                    	movi.n	a14, 0
     9e5:	880c                                    	movi.n	a8, 8
     9e7:	019990                                    	slli	a9, a9, 23
     9ea:	11aa10                                    	slli	a10, a10, 15
     9ed:	04dd                                    	mov.n	a13, a4
     9ef:	fb0c                                    	movi.n	a11, 15
     9f1:	c42a                                    	add.n	a12, a4, a2
     9f3:	040c                                    	movi.n	a4, 0
     9f5:	020c                                    	movi.n	a2, 0
     9f7:	000546                                    	j	a10 <mSBC_Encoder_Process+0x58>

000009fa <mSBC_Encoder_Process+0x42>:
	...

000009fc <mSBC_Encoder_Process+0x44>:
     9fc:	220b                                    	addi.n	a2, a2, -1
     9fe:	3c4b                                    	addi.n	a3, a12, 4
     a00:	0c29                                    	s32i.n	a2, a12, 0
     a02:	880b                                    	addi.n	a8, a8, -1
     a04:	73e2e0                                    	maxu	a14, a2, a14
     a07:	03cd                                    	mov.n	a12, a3
     a09:	020c                                    	movi.n	a2, 0
     a0b:	040c                                    	movi.n	a4, 0
     a0d:	0b0816                                    	beqz	a8, ac1 <mSBC_Encoder_Process+0x109>
     a10:	1430f0                                    	extui	a3, a15, 0, 2
     a13:	230740a008055fce                  	{ blti.w15	a15, 1, aa1 <mSBC_Encoder_Process+0xe9>; nop }
     a1b:	40cc62                                    	addi	a6, a12, 64
     a1e:	040c                                    	movi.n	a4, 0
     a20:	4122f0                                    	srli	a2, a15, 2
     a23:	0a9376                                    	loopnez	a3, a31 <mSBC_Encoder_Process+0x79>
     a26:	0658                                    	l32i.n	a5, a6, 0
     a28:	20c662                                    	addi	a6, a6, 32
     a2b:	603150                                    	abs	a3, a5
     a2e:	534430                                    	max	a4, a4, a3

00000a31 <mSBC_Encoder_Process+0x79>:
     a31:	066216                                    	beqz	a2, a9b <mSBC_Encoder_Process+0xe3>
     a34:	0638                                    	l32i.n	a3, a6, 0
     a36:	082652                                    	l32i	a5, a6, 32
     a39:	40c662                                    	addi	a6, a6, 64
     a3c:	002672                                    	l32i	a7, a6, 0
     a3f:	603130                                    	abs	a3, a3
     a42:	605150                                    	abs	a5, a5
     a45:	533430                                    	max	a3, a4, a3
     a48:	533350                                    	max	a3, a3, a5
     a4b:	082652                                    	l32i	a5, a6, 32
     a4e:	604170                                    	abs	a4, a7
     a51:	ffc272                                    	addi	a7, a2, -1
     a54:	534340                                    	max	a4, a3, a4
     a57:	40c622                                    	addi	a2, a6, 64
     a5a:	603150                                    	abs	a3, a5
     a5d:	379776                                    	loopnez	a7, a98 <mSBC_Encoder_Process+0xe0>
     a60:	220684c00489123e                  	{ l32i	a3, a2, 0; max	a6, a4, a3 }
     a68:	060282420490125e                  	{ l32i	a5, a2, 32; addi	a7, a2, 64 }
     a70:	020287400490074e                  	{ l32i	a4, a7, 0; addi	a2, a7, 64 }
     a78:	0703874018e2de3e                  	{ abs	a3, a3; l32i	a7, a7, 32 }
     a80:	350720c108a0763e                  	{ max	a5, a6, a3; abs	a3, a5 }
     a88:	350720c100a0653e                  	{ max	a4, a5, a3; abs	a3, a4 }
     a90:	370720c100a0743e                  	{ max	a4, a4, a3; abs	a3, a7 }
     a98:	534430                                    	max	a4, a4, a3

00000a9b <mSBC_Encoder_Process+0xe3>:
     a9b:	722940                                    	salt	a2, a9, a4
     a9e:	b02220                                    	addx8	a2, a2, a2
     aa1:	198076                                    	loop	a0, abe <mSBC_Encoder_Process+0x106>
     aa4:	401200                                    	ssl	a2
     aa7:	023d                                    	mov.n	a3, a2
     aa9:	a15a00                                    	sll	a5, a10
     aac:	221b                                    	addi.n	a2, a2, 1
     aae:	2307209a0be5554e                  	{ bge.w15	a5, a4, 9fc <mSBC_Encoder_Process+0x44>; nop }
     ab6:	230720990be513be                  	{ bgeu.w15	a3, a11, 9fc <mSBC_Encoder_Process+0x44>; nop }

00000abe <mSBC_Encoder_Process+0x106>:
     abe:	fff7c6                                    	j	aa1 <mSBC_Encoder_Process+0xe9>

00000ac1 <mSBC_Encoder_Process+0x109>:
     ac1:	fd6a21                                    	l32r	a2, 6c (10e0 <sbc_enc_bit_alloc_mono>)
     ac4:	04dd32                                    	addmi	a3, a13, 0x400
     ac7:	0dad                                    	mov.n	a10, a13
     ac9:	0d4d                                    	mov.n	a4, a13
     acb:	2763e2                                    	s32i	a14, a3, 156
     ace:	0002e0                                    	callx8	a2
     ad1:	fd6721                                    	l32r	a2, 70 (ae4 <EncPacking>)
     ad4:	04ad                                    	mov.n	a10, a4
     ad6:	0002e0                                    	callx8	a2
     ad9:	040c                                    	movi.n	a4, 0
     adb:	2b2322                                    	l32i	a2, a3, 172
     ade:	316342                                    	s32i	a4, a3, 196
     ae1:	f01d                                    	retw.n

00000ae3 <mSBC_Encoder_Process+0x12b>:
	...

00000ae4 <EncPacking>:
     ae4:	008136                                    	entry	a1, 64
     ae7:	04d232                                    	addmi	a3, a2, 0x400
     aea:	02fd                                    	mov.n	a15, a2
     aec:	312322                                    	l32i	a2, a3, 196
     aef:	2223b2                                    	l32i	a11, a3, 136
     af2:	2923e2                                    	l32i	a14, a3, 164
     af5:	5129                                    	s32i.n	a2, a1, 20
     af7:	2d2342                                    	l32i	a4, a3, 180
     afa:	3063e2                                    	s32i	a14, a3, 192
     afd:	04df92                                    	addmi	a9, a15, 0x400
     b00:	02bc                                    	beqz.n	a2, b34 <EncPacking+0x50>
     b02:	0e7d                                    	mov.n	a7, a14
     b04:	230760a01c45126e                  	{ bnei.w15	a2, 3, dea <EncPacking+0x306>; nop }
     b0c:	fd5681                                    	l32r	a8, 64 (1a20 <MASK_bitp>)
     b0f:	021c                                    	movi.n	a2, 16
     b11:	f8c452                                    	addi	a5, a4, -8
     b14:	230760a20005d44e                  	{ bgei.w15	a4, 8, bf2 <EncPacking+0x10e>; nop }
     b1c:	04df62                                    	addmi	a6, a15, 0x400
     b1f:	9ca052                                    	movi	a5, 156
     b22:	5c9622                                    	l16si	a2, a6, 184
     b25:	0e7d                                    	mov.n	a7, a14
     b27:	112280                                    	slli	a2, a2, 8
     b2a:	225a                                    	add.n	a2, a2, a5
     b2c:	548b                                    	addi.n	a5, a4, 8
     b2e:	5c5622                                    	s16i	a2, a6, 184
     b31:	003f06                                    	j	c31 <EncPacking+0x14d>

00000b34 <EncPacking+0x50>:
     b34:	2d2362                                    	l32i	a6, a3, 180
     b37:	04df82                                    	addmi	a8, a15, 0x400
     b3a:	ada042                                    	movi	a4, 173
     b3d:	fd49a1                                    	l32r	a10, 64 (1a20 <MASK_bitp>)
     b40:	04df92                                    	addmi	a9, a15, 0x400
     b43:	ada052                                    	movi	a5, 173
     b46:	021c                                    	movi.n	a2, 16
     b48:	230700a20025764e                  	{ bgei.w15	a6, 8, c5a <EncPacking+0x176>; nop }
     b50:	5c9942                                    	l16si	a4, a9, 184
     b53:	0e7d                                    	mov.n	a7, a14
     b55:	114480                                    	slli	a4, a4, 8
     b58:	445a                                    	add.n	a4, a4, a5
     b5a:	568b                                    	addi.n	a5, a6, 8
     b5c:	5c5942                                    	s16i	a4, a9, 184
     b5f:	2d6352                                    	s32i	a5, a3, 180
     b62:	230700a20425f5ce                  	{ blti.w15	a5, 8, ca4 <EncPacking+0x1c0>; nop }
     b6a:	021c                                    	movi.n	a2, 16
     b6c:	ffa062                                    	movi	a6, 255
     b6f:	c02250                                    	sub	a2, a2, a5
     b72:	04df82                                    	addmi	a8, a15, 0x400
     b75:	944d                                    	ae_zext16	a4, a4
     b77:	401200                                    	ssl	a2
     b7a:	2f28c2                                    	l32i	a12, a8, 188
     b7d:	a12400                                    	sll	a2, a4
     b80:	116600                                    	slli	a6, a6, 16
     b83:	114280                                    	slli	a4, a2, 8
     b86:	012280                                    	slli	a2, a2, 24
     b89:	104460                                    	and	a4, a4, a6
     b8c:	972b                                    	addi.n	a9, a7, 2
     b8e:	224a                                    	add.n	a2, a2, a4
     b90:	dc1b                                    	addi.n	a13, a12, 1
     b92:	312020                                    	srai	a2, a2, 16
     b95:	040c                                    	movi.n	a4, 0
     b97:	672d                                    	ae_s16i.n	a2, a7, 0
     b99:	f8c552                                    	addi	a5, a5, -8
     b9c:	097d                                    	mov.n	a7, a9
     b9e:	306392                                    	s32i	a9, a3, 192
     ba1:	2f68d2                                    	s32i	a13, a8, 188
     ba4:	5c5842                                    	s16i	a4, a8, 184
     ba7:	2d6352                                    	s32i	a5, a3, 180
     baa:	220700a2002575ce                  	{ blti.w15	a5, 8, cba <EncPacking+0x1d6>; nop }
     bb2:	021c                                    	movi.n	a2, 16
     bb4:	ffa062                                    	movi	a6, 255
     bb7:	c02250                                    	sub	a2, a2, a5
     bba:	04df82                                    	addmi	a8, a15, 0x400
     bbd:	944d                                    	ae_zext16	a4, a4
     bbf:	401200                                    	ssl	a2
     bc2:	2f28a2                                    	l32i	a10, a8, 188
     bc5:	a12400                                    	sll	a2, a4
     bc8:	116600                                    	slli	a6, a6, 16
     bcb:	114280                                    	slli	a4, a2, 8
     bce:	012280                                    	slli	a2, a2, 24
     bd1:	104460                                    	and	a4, a4, a6
     bd4:	972b                                    	addi.n	a9, a7, 2
     bd6:	224a                                    	add.n	a2, a2, a4
     bd8:	ca1b                                    	addi.n	a12, a10, 1
     bda:	312020                                    	srai	a2, a2, 16
     bdd:	0d0c                                    	movi.n	a13, 0
     bdf:	672d                                    	ae_s16i.n	a2, a7, 0
     be1:	f8c542                                    	addi	a4, a5, -8
     be4:	097d                                    	mov.n	a7, a9
     be6:	306392                                    	s32i	a9, a3, 192
     be9:	2f68c2                                    	s32i	a12, a8, 188
     bec:	5c58d2                                    	s16i	a13, a8, 184
     bef:	003446                                    	j	cc4 <EncPacking+0x1e0>

00000bf2 <EncPacking+0x10e>:
     bf2:	5c19d2                                    	l16ui	a13, a9, 184
     bf5:	9ca062                                    	movi	a6, 156
     bf8:	400500                                    	ssr	a5
     bfb:	c02240                                    	sub	a2, a2, a4
     bfe:	917060                                    	srl	a7, a6
     c01:	401200                                    	ssl	a2
     c04:	a12d00                                    	sll	a2, a13
     c07:	90c580                                    	addx2	a12, a5, a8
     c0a:	202270                                    	or	a2, a2, a7
     c0d:	2f29a2                                    	l32i	a10, a9, 188
     c10:	5c5922                                    	s16i	a2, a9, 184
     c13:	922d                                    	ae_zext16	a2, a2
     c15:	117280                                    	slli	a7, a2, 8
     c18:	412820                                    	srli	a2, a2, 8
     c1b:	272a                                    	add.n	a2, a7, a2
     c1d:	7e2b                                    	addi.n	a7, a14, 2
     c1f:	6e2d                                    	ae_s16i.n	a2, a14, 0
     c21:	da1b                                    	addi.n	a13, a10, 1
     c23:	4c2d                                    	ae_l16ui.n	a2, a12, 0
     c25:	306372                                    	s32i	a7, a3, 192
     c28:	2f69d2                                    	s32i	a13, a9, 188
     c2b:	102260                                    	and	a2, a2, a6
     c2e:	5c5922                                    	s16i	a2, a9, 184
     c31:	04df42                                    	addmi	a4, a15, 0x400
     c34:	2d6352                                    	s32i	a5, a3, 180
     c37:	2a24a2                                    	l32i	a10, a4, 168
     c3a:	061c                                    	movi.n	a6, 16
     c3c:	924d                                    	ae_zext16	a4, a2
     c3e:	f8c592                                    	addi	a9, a5, -8
     c41:	220740a20005b54e                  	{ bgei.w15	a5, 8, cd9 <EncPacking+0x1f5>; nop }
     c49:	112480                                    	slli	a2, a4, 8
     c4c:	04dfd2                                    	addmi	a13, a15, 0x400
     c4f:	2022a0                                    	or	a2, a2, a10
     c52:	958b                                    	addi.n	a9, a5, 8
     c54:	5c5d22                                    	s16i	a2, a13, 184
     c57:	002f06                                    	j	d17 <EncPacking+0x233>

00000c5a <EncPacking+0x176>:
     c5a:	5c1892                                    	l16ui	a9, a8, 184
     c5d:	f8c652                                    	addi	a5, a6, -8
     c60:	400500                                    	ssr	a5
     c63:	c02260                                    	sub	a2, a2, a6
     c66:	917040                                    	srl	a7, a4
     c69:	401200                                    	ssl	a2
     c6c:	a16900                                    	sll	a6, a9
     c6f:	2f2892                                    	l32i	a9, a8, 188
     c72:	206670                                    	or	a6, a6, a7
     c75:	7e2b                                    	addi.n	a7, a14, 2
     c77:	5c5862                                    	s16i	a6, a8, 184
     c7a:	9025a0                                    	addx2	a2, a5, a10
     c7d:	966d                                    	ae_zext16	a6, a6
     c7f:	11c680                                    	slli	a12, a6, 8
     c82:	416860                                    	srli	a6, a6, 8
     c85:	6c6a                                    	add.n	a6, a12, a6
     c87:	d91b                                    	addi.n	a13, a9, 1
     c89:	6e6d                                    	ae_s16i.n	a6, a14, 0
     c8b:	422d                                    	ae_l16ui.n	a2, a2, 0
     c8d:	306372                                    	s32i	a7, a3, 192
     c90:	2f68d2                                    	s32i	a13, a8, 188
     c93:	104240                                    	and	a4, a2, a4
     c96:	2d6352                                    	s32i	a5, a3, 180
     c99:	5c5842                                    	s16i	a4, a8, 184
     c9c:	230760ba03c5554e                  	{ bgei.w15	a5, 8, b6a <EncPacking+0x86>; nop }
     ca4:	04df22                                    	addmi	a2, a15, 0x400
     ca7:	114480                                    	slli	a4, a4, 8
     caa:	558b                                    	addi.n	a5, a5, 8
     cac:	5c5242                                    	s16i	a4, a2, 184
     caf:	2d6352                                    	s32i	a5, a3, 180
     cb2:	220760ba07c5f54e                  	{ bgei.w15	a5, 8, bb2 <EncPacking+0xce>; nop }
     cba:	04df22                                    	addmi	a2, a15, 0x400
     cbd:	060c                                    	movi.n	a6, 0
     cbf:	458b                                    	addi.n	a4, a5, 8
     cc1:	5c5262                                    	s16i	a6, a2, 184
     cc4:	04df52                                    	addmi	a5, a15, 0x400
     cc7:	2d6342                                    	s32i	a4, a3, 180
     cca:	020c                                    	movi.n	a2, 0
     ccc:	6884e6                                    	bgei	a4, 8, d38 <EncPacking+0x254>
     ccf:	060c                                    	movi.n	a6, 0
     cd1:	248b                                    	addi.n	a2, a4, 8
     cd3:	5c5562                                    	s16i	a6, a5, 184
     cd6:	001cc6                                    	j	d4d <EncPacking+0x269>

00000cd9 <EncPacking+0x1f5>:
     cd9:	400900                                    	ssr	a9
     cdc:	c06650                                    	sub	a6, a6, a5
     cdf:	9a2d                                    	ae_zext16	a2, a10
     ce1:	912020                                    	srl	a2, a2
     ce4:	401600                                    	ssl	a6
     ce7:	a14400                                    	sll	a4, a4
     cea:	04df52                                    	addmi	a5, a15, 0x400
     ced:	202420                                    	or	a2, a4, a2
     cf0:	2f25d2                                    	l32i	a13, a5, 188
     cf3:	5c5522                                    	s16i	a2, a5, 184
     cf6:	922d                                    	ae_zext16	a2, a2
     cf8:	11c280                                    	slli	a12, a2, 8
     cfb:	412820                                    	srli	a2, a2, 8
     cfe:	2c2a                                    	add.n	a2, a12, a2
     d00:	90c980                                    	addx2	a12, a9, a8
     d03:	672d                                    	ae_s16i.n	a2, a7, 0
     d05:	772b                                    	addi.n	a7, a7, 2
     d07:	2c2d                                    	ae_l16si.n	a2, a12, 0
     d09:	dd1b                                    	addi.n	a13, a13, 1
     d0b:	306372                                    	s32i	a7, a3, 192
     d0e:	2f65d2                                    	s32i	a13, a5, 188
     d11:	1022a0                                    	and	a2, a2, a10
     d14:	5c5522                                    	s16i	a2, a5, 184
     d17:	04df42                                    	addmi	a4, a15, 0x400
     d1a:	2d6392                                    	s32i	a9, a3, 180
     d1d:	2524a2                                    	l32i	a10, a4, 148
     d20:	061c                                    	movi.n	a6, 16
     d22:	924d                                    	ae_zext16	a4, a2
     d24:	f8c952                                    	addi	a5, a9, -8
     d27:	2a89e6                                    	bgei	a9, 8, d55 <EncPacking+0x271>
     d2a:	112480                                    	slli	a2, a4, 8
     d2d:	04df62                                    	addmi	a6, a15, 0x400
     d30:	2042a0                                    	or	a4, a2, a10
     d33:	598b                                    	addi.n	a5, a9, 8
     d35:	0015c6                                    	j	d90 <EncPacking+0x2ac>

00000d38 <EncPacking+0x254>:
     d38:	2f2562                                    	l32i	a6, a5, 188
     d3b:	672d                                    	ae_s16i.n	a2, a7, 0
     d3d:	5c5522                                    	s16i	a2, a5, 184
     d40:	772b                                    	addi.n	a7, a7, 2
     d42:	f8c422                                    	addi	a2, a4, -8
     d45:	306372                                    	s32i	a7, a3, 192
     d48:	661b                                    	addi.n	a6, a6, 1
     d4a:	2f6562                                    	s32i	a6, a5, 188
     d4d:	2d6322                                    	s32i	a2, a3, 180
     d50:	002586                                    	j	dea <EncPacking+0x306>

00000d53 <EncPacking+0x26f>:
	...

00000d55 <EncPacking+0x271>:
     d55:	9a2d                                    	ae_zext16	a2, a10
     d57:	400500                                    	ssr	a5
     d5a:	c06690                                    	sub	a6, a6, a9
     d5d:	912020                                    	srl	a2, a2
     d60:	401600                                    	ssl	a6
     d63:	a14400                                    	sll	a4, a4
     d66:	04df62                                    	addmi	a6, a15, 0x400
     d69:	202420                                    	or	a2, a4, a2
     d6c:	2f26d2                                    	l32i	a13, a6, 188
     d6f:	5c5622                                    	s16i	a2, a6, 184
     d72:	922d                                    	ae_zext16	a2, a2
     d74:	11c280                                    	slli	a12, a2, 8
     d77:	412820                                    	srli	a2, a2, 8
     d7a:	2c2a                                    	add.n	a2, a12, a2
     d7c:	90c580                                    	addx2	a12, a5, a8
     d7f:	672d                                    	ae_s16i.n	a2, a7, 0
     d81:	772b                                    	addi.n	a7, a7, 2
     d83:	2c2d                                    	ae_l16si.n	a2, a12, 0
     d85:	dd1b                                    	addi.n	a13, a13, 1
     d87:	306372                                    	s32i	a7, a3, 192
     d8a:	2f66d2                                    	s32i	a13, a6, 188
     d8d:	1042a0                                    	and	a4, a2, a10
     d90:	5c5642                                    	s16i	a4, a6, 184
     d93:	021c                                    	movi.n	a2, 16
     d95:	2d6352                                    	s32i	a5, a3, 180
     d98:	ffa062                                    	movi	a6, 255
     d9b:	c02250                                    	sub	a2, a2, a5
     d9e:	1085e6                                    	bgei	a5, 8, db2 <EncPacking+0x2ce>
     da1:	558b                                    	addi.n	a5, a5, 8
     da3:	04df22                                    	addmi	a2, a15, 0x400
     da6:	114480                                    	slli	a4, a4, 8
     da9:	2d6352                                    	s32i	a5, a3, 180
     dac:	5c5242                                    	s16i	a4, a2, 184
     daf:	000dc6                                    	j	dea <EncPacking+0x306>

00000db2 <EncPacking+0x2ce>:
     db2:	f8c582                                    	addi	a8, a5, -8
     db5:	401200                                    	ssl	a2
     db8:	944d                                    	ae_zext16	a4, a4
     dba:	04df52                                    	addmi	a5, a15, 0x400
     dbd:	a14400                                    	sll	a4, a4
     dc0:	2f25d2                                    	l32i	a13, a5, 188
     dc3:	119480                                    	slli	a9, a4, 8
     dc6:	116600                                    	slli	a6, a6, 16
     dc9:	014480                                    	slli	a4, a4, 24
     dcc:	106960                                    	and	a6, a9, a6
     dcf:	a72b                                    	addi.n	a10, a7, 2
     dd1:	446a                                    	add.n	a4, a4, a6
     dd3:	2d1b                                    	addi.n	a2, a13, 1
     dd5:	31c040                                    	srai	a12, a4, 16
     dd8:	0d0c                                    	movi.n	a13, 0
     dda:	67cd                                    	ae_s16i.n	a12, a7, 0
     ddc:	0a7d                                    	mov.n	a7, a10
     dde:	2d6382                                    	s32i	a8, a3, 180
     de1:	3063a2                                    	s32i	a10, a3, 192
     de4:	2f6522                                    	s32i	a2, a5, 188
     de7:	5c55d2                                    	s16i	a13, a5, 184
     dea:	40a222                                    	movi	a2, 0x240
     ded:	21b9                                    	s32i.n	a11, a1, 8
     def:	2f2a                                    	add.n	a2, a15, a2
     df1:	04df82                                    	addmi	a8, a15, 0x400
     df4:	4129                                    	s32i.n	a2, a1, 16
     df6:	0a0c                                    	movi.n	a10, 0
     df8:	2128                                    	l32i.n	a2, a1, 8
     dfa:	7b1ba6                                    	blti	a11, 1, e79 <EncPacking+0x395>
     dfd:	41c8                                    	l32i.n	a12, a1, 16
     dff:	0b1c                                    	movi.n	a11, 16
     e01:	040c                                    	movi.n	a4, 0
     e03:	fc9891                                    	l32r	a9, 64 (1a20 <MASK_bitp>)
     e06:	3450a0                                    	extui	a5, a10, 0, 4
     e09:	6c8276                                    	loop	a2, e79 <EncPacking+0x395>
     e0c:	0c28                                    	l32i.n	a2, a12, 0
     e0e:	1155c0                                    	slli	a5, a5, 4
     e11:	20a250                                    	or	a10, a2, a5
     e14:	120c                                    	movi.n	a2, 1
     e16:	581466                                    	bnei	a4, 1, e72 <EncPacking+0x38e>
     e19:	2d23d2                                    	l32i	a13, a3, 180
     e1c:	ba4d                                    	ae_zext8	a4, a10
     e1e:	f8cd62                                    	addi	a6, a13, -8
     e21:	0c8de6                                    	bgei	a13, 8, e31 <EncPacking+0x34d>
     e24:	5c9822                                    	l16si	a2, a8, 184
     e27:	6d8b                                    	addi.n	a6, a13, 8
     e29:	112280                                    	slli	a2, a2, 8
     e2c:	224a                                    	add.n	a2, a2, a4
     e2e:	000e06                                    	j	e6a <EncPacking+0x386>

00000e31 <EncPacking+0x34d>:
     e31:	5c1852                                    	l16ui	a5, a8, 184
     e34:	c02bd0                                    	sub	a2, a11, a13
     e37:	400600                                    	ssr	a6
     e3a:	91d040                                    	srl	a13, a4
     e3d:	401200                                    	ssl	a2
     e40:	a12500                                    	sll	a2, a5
     e43:	2022d0                                    	or	a2, a2, a13
     e46:	2f28d2                                    	l32i	a13, a8, 188
     e49:	5c5822                                    	s16i	a2, a8, 184
     e4c:	922d                                    	ae_zext16	a2, a2
     e4e:	115280                                    	slli	a5, a2, 8
     e51:	412820                                    	srli	a2, a2, 8
     e54:	252a                                    	add.n	a2, a5, a2
     e56:	905690                                    	addx2	a5, a6, a9
     e59:	672d                                    	ae_s16i.n	a2, a7, 0
     e5b:	772b                                    	addi.n	a7, a7, 2
     e5d:	452d                                    	ae_l16ui.n	a2, a5, 0
     e5f:	dd1b                                    	addi.n	a13, a13, 1
     e61:	306372                                    	s32i	a7, a3, 192
     e64:	2f68d2                                    	s32i	a13, a8, 188
     e67:	102240                                    	and	a2, a2, a4
     e6a:	5c5822                                    	s16i	a2, a8, 184
     e6d:	020c                                    	movi.n	a2, 0
     e6f:	2d6362                                    	s32i	a6, a3, 180
     e72:	cc4b                                    	addi.n	a12, a12, 4
     e74:	024d                                    	mov.n	a4, a2
     e76:	3450a0                                    	extui	a5, a10, 0, 4

00000e79 <EncPacking+0x395>:
     e79:	04df22                                    	addmi	a2, a15, 0x400
     e7c:	61e9                                    	s32i.n	a14, a1, 24
     e7e:	232252                                    	l32i	a5, a2, 140
     e81:	31f9                                    	s32i.n	a15, a1, 12
     e83:	3148                                    	l32i.n	a4, a1, 12
     e85:	fc7c                                    	movi.n	a12, -1
     e87:	0b1c                                    	movi.n	a11, 16
     e89:	80a262                                    	movi	a6, 0x280
     e8c:	f4c0c0                                    	extui	a12, a12, 0, 16
     e8f:	230760a4080515ce                  	{ blti.w15	a5, 1, f56 <EncPacking+0x472>; nop }
     e97:	04d4a2                                    	addmi	a10, a4, 0x400
     e9a:	1e0c                                    	movi.n	a14, 1
     e9c:	2128                                    	l32i.n	a2, a1, 8
     e9e:	80f460                                    	add	a15, a4, a6

00000ea1 <EncPacking+0x3bd>:
     ea1:	550b                                    	addi.n	a5, a5, -1
     ea3:	7159                                    	s32i.n	a5, a1, 28
     ea5:	2148                                    	l32i.n	a4, a1, 8
     ea7:	230740a40805f2ce                  	{ blti.w15	a2, 1, f4a <EncPacking+0x466>; nop }
     eaf:	4188                                    	l32i.n	a8, a1, 16
     eb1:	958476                                    	loop	a4, f4a <EncPacking+0x466>
     eb4:	8848                                    	l32i.n	a4, a8, 32
     eb6:	c05b40                                    	sub	a5, a11, a4
     eb9:	089416                                    	beqz	a4, f46 <EncPacking+0x462>
     ebc:	0828                                    	l32i.n	a2, a8, 0
     ebe:	400500                                    	ssr	a5
     ec1:	9190c0                                    	srl	a9, a12
     ec4:	0f68                                    	l32i.n	a6, a15, 0
     ec6:	2d2352                                    	l32i	a5, a3, 180
     ec9:	119910                                    	slli	a9, a9, 15
     ecc:	d2fb                                    	addi.n	a13, a2, 15
     ece:	401d00                                    	ssl	a13
     ed1:	a1de00                                    	sll	a13, a14
     ed4:	400200                                    	ssr	a2
     ed7:	6d6a                                    	add.n	a6, a13, a6
     ed9:	c02b50                                    	sub	a2, a11, a5
     edc:	82d690                                    	mull	a13, a6, a9
     edf:	b26690                                    	mulsh	a6, a6, a9
     ee2:	05dfd0                                    	extui	a13, a13, 31, 1
     ee5:	9066d0                                    	addx2	a6, a6, a13
     ee8:	b16060                                    	sra	a6, a6
     eeb:	13a427                                    	bge	a4, a2, f02 <EncPacking+0x41e>
     eee:	5c1a22                                    	l16ui	a2, a10, 184
     ef1:	401400                                    	ssl	a4
     ef4:	454a                                    	add.n	a4, a5, a4
     ef6:	a12200                                    	sll	a2, a2
     ef9:	202260                                    	or	a2, a2, a6
     efc:	001006                                    	j	f40 <EncPacking+0x45c>

00000eff <EncPacking+0x41b>:
     eff:	000000                                        ...

00000f02 <EncPacking+0x41e>:
     f02:	5c1a92                                    	l16ui	a9, a10, 184
     f05:	c04420                                    	sub	a4, a4, a2
     f08:	965d                                    	ae_zext16	a5, a6
     f0a:	400400                                    	ssr	a4
     f0d:	915050                                    	srl	a5, a5
     f10:	401200                                    	ssl	a2
     f13:	a12900                                    	sll	a2, a9
     f16:	2f2a92                                    	l32i	a9, a10, 188
     f19:	202250                                    	or	a2, a2, a5
     f1c:	5c5a22                                    	s16i	a2, a10, 184
     f1f:	922d                                    	ae_zext16	a2, a2
     f21:	11d280                                    	slli	a13, a2, 8
     f24:	412820                                    	srli	a2, a2, 8
     f27:	2d2a                                    	add.n	a2, a13, a2
     f29:	fc4ed1                                    	l32r	a13, 64 (1a20 <MASK_bitp>)
     f2c:	672d                                    	ae_s16i.n	a2, a7, 0
     f2e:	772b                                    	addi.n	a7, a7, 2
     f30:	306372                                    	s32i	a7, a3, 192
     f33:	9054d0                                    	addx2	a5, a4, a13
     f36:	d91b                                    	addi.n	a13, a9, 1
     f38:	252d                                    	ae_l16si.n	a2, a5, 0
     f3a:	2f6ad2                                    	s32i	a13, a10, 188
     f3d:	102260                                    	and	a2, a2, a6
     f40:	5c5a22                                    	s16i	a2, a10, 184
     f43:	2d6342                                    	s32i	a4, a3, 180
     f46:	884b                                    	addi.n	a8, a8, 4
     f48:	ff4b                                    	addi.n	a15, a15, 4

00000f4a <EncPacking+0x466>:
     f4a:	7158                                    	l32i.n	a5, a1, 28
     f4c:	2128                                    	l32i.n	a2, a1, 8
     f4e:	230720bc0be5754e                  	{ bgei.w15	a5, 1, ea1 <EncPacking+0x3bd>; nop }

00000f56 <EncPacking+0x472>:
     f56:	31b8                                    	l32i.n	a11, a1, 12
     f58:	2d2392                                    	l32i	a9, a3, 180
     f5b:	04db42                                    	addmi	a4, a11, 0x400
     f5e:	04db52                                    	addmi	a5, a11, 0x400
     f61:	2f2462                                    	l32i	a6, a4, 188
     f64:	04dbb2                                    	addmi	a11, a11, 0x400
     f67:	021c                                    	movi.n	a2, 16
     f69:	1186f0                                    	slli	a8, a6, 1
     f6c:	c02290                                    	sub	a2, a2, a9
     f6f:	2b6482                                    	s32i	a8, a4, 172
     f72:	ffa0a2                                    	movi	a10, 255
     f75:	19bc                                    	beqz.n	a9, faa <EncPacking+0x4c6>
     f77:	5c1bf2                                    	l16ui	a15, a11, 184
     f7a:	401200                                    	ssl	a2
     f7d:	661b                                    	addi.n	a6, a6, 1
     f7f:	11aa00                                    	slli	a10, a10, 16
     f82:	0d0c                                    	movi.n	a13, 0
     f84:	a19f00                                    	sll	a9, a15
     f87:	2f6462                                    	s32i	a6, a4, 188
     f8a:	11e980                                    	slli	a14, a9, 8
     f8d:	01f980                                    	slli	a15, a9, 24
     f90:	104ea0                                    	and	a4, a14, a10
     f93:	881b                                    	addi.n	a8, a8, 1
     f95:	4f4a                                    	add.n	a4, a15, a4
     f97:	c72b                                    	addi.n	a12, a7, 2
     f99:	314040                                    	srai	a4, a4, 16
     f9c:	3063c2                                    	s32i	a12, a3, 192
     f9f:	674d                                    	ae_s16i.n	a4, a7, 0
     fa1:	2d63d2                                    	s32i	a13, a3, 180
     fa4:	2b6582                                    	s32i	a8, a5, 172
     fa7:	5c5bd2                                    	s16i	a13, a11, 184
     faa:	2158                                    	l32i.n	a5, a1, 8
     fac:	61f8                                    	l32i.n	a15, a1, 24
     fae:	c27c                                    	movi.n	a2, -4
     fb0:	f40c                                    	movi.n	a4, 15
     fb2:	160c                                    	movi.n	a6, 1
     fb4:	217150                                    	srai	a7, a5, 1
     fb7:	230760801805d52e                  	{ blt.w15	a5, a2, 1095 <EncPacking+0x5b1>; nop }
     fbf:	573b                                    	addi.n	a5, a7, 3
     fc1:	4f2d                                    	ae_l16ui.n	a2, a15, 0
     fc3:	535560                                    	max	a5, a5, a6
     fc6:	f40c                                    	movi.n	a4, 15
     fc8:	070c                                    	movi.n	a7, 0
     fca:	a63c                                    	movi.n	a6, 58
     fcc:	0f9d                                    	mov.n	a9, a15
     fce:	41a820                                    	srli	a10, a2, 8
     fd1:	07bd                                    	mov.n	a11, a7
     fd3:	be8576                                    	loop	a5, 1095 <EncPacking+0x5b1>
     fd6:	771b                                    	addi.n	a7, a7, 1
     fd8:	220740a414059b0e                  	{ beqi.w15	a11, 2, 108d <EncPacking+0x5a9>; nop }
     fe0:	ba5d                                    	ae_zext8	a5, a10
     fe2:	30b450                                    	xor	a11, a4, a5
     fe5:	44c040                                    	extui	a12, a4, 0, 5
     fe8:	0440a0                                    	extui	a4, a10, 0, 1
     feb:	04a7b0                                    	extui	a10, a11, 7, 1
     fee:	f0eaa0                                    	subx8	a14, a10, a10
     ff1:	04d5b0                                    	extui	a13, a11, 5, 1
     ff4:	a0aea0                                    	addx4	a10, a14, a10
     ff7:	04b6b0                                    	extui	a11, a11, 6, 1
     ffa:	f0edd0                                    	subx8	a14, a13, a13
     ffd:	11ccf0                                    	slli	a12, a12, 1
    1000:	d1bb60                                    	mul16s	a11, a11, a6
    1003:	a0ded0                                    	addx4	a13, a14, a13
    1006:	30aac0                                    	xor	a10, a10, a12
    1009:	30bbd0                                    	xor	a11, a11, a13
    100c:	11dae0                                    	slli	a13, a10, 2
    100f:	04e450                                    	extui	a14, a5, 4, 1
    1012:	41a5a0                                    	srli	a10, a10, 5
    1015:	30aea0                                    	xor	a10, a14, a10
    1018:	30bdb0                                    	xor	a11, a13, a11
    101b:	f0eaa0                                    	subx8	a14, a10, a10
    101e:	64b0b0                                    	extui	a11, a11, 0, 7
    1021:	a0aea0                                    	addx4	a10, a14, a10
    1024:	11bbf0                                    	slli	a11, a11, 1
    1027:	30aab0                                    	xor	a10, a10, a11
    102a:	41c350                                    	srli	a12, a5, 3
    102d:	41b7a0                                    	srli	a11, a10, 7
    1030:	44e0a0                                    	extui	a14, a10, 0, 5
    1033:	30bcb0                                    	xor	a11, a12, a11
    1036:	41d250                                    	srli	a13, a5, 2
    1039:	41c6a0                                    	srli	a12, a10, 6
    103c:	415150                                    	srli	a5, a5, 1
    103f:	41a5a0                                    	srli	a10, a10, 5
    1042:	30cdc0                                    	xor	a12, a13, a12
    1045:	3055a0                                    	xor	a5, a5, a10
    1048:	04a0b0                                    	extui	a10, a11, 0, 1
    104b:	04d0c0                                    	extui	a13, a12, 0, 1
    104e:	f0caa0                                    	subx8	a12, a10, a10
    1051:	045050                                    	extui	a5, a5, 0, 1
    1054:	11eef0                                    	slli	a14, a14, 1
    1057:	a0aca0                                    	addx4	a10, a12, a10
    105a:	d1bd60                                    	mul16s	a11, a13, a6
    105d:	f0d550                                    	subx8	a13, a5, a5
    1060:	30aae0                                    	xor	a10, a10, a14
    1063:	a05d50                                    	addx4	a5, a13, a5
    1066:	41d5a0                                    	srli	a13, a10, 5
    1069:	305b50                                    	xor	a5, a11, a5
    106c:	11aae0                                    	slli	a10, a10, 2
    106f:	304d40                                    	xor	a4, a13, a4
    1072:	305a50                                    	xor	a5, a10, a5
    1075:	f0e440                                    	subx8	a14, a4, a4
    1078:	645050                                    	extui	a5, a5, 0, 7
    107b:	a04e40                                    	addx4	a4, a14, a4
    107e:	1155f0                                    	slli	a5, a5, 1
    1081:	41a820                                    	srli	a10, a2, 8
    1084:	304450                                    	xor	a4, a4, a5
    1087:	02e707                                    	bbsi	a7, 0, 108d <EncPacking+0x5a9>
    108a:	000146                                    	j	1093 <EncPacking+0x5af>

0000108d <EncPacking+0x5a9>:
    108d:	592d                                    	ae_l16ui.n	a2, a9, 2
    108f:	992b                                    	addi.n	a9, a9, 2
    1091:	02ad                                    	mov.n	a10, a2
    1093:	07bd                                    	mov.n	a11, a7

00001095 <EncPacking+0x5b1>:
    1095:	5128                                    	l32i.n	a2, a1, 20
    1097:	013226                                    	beqi	a2, 3, 109c <EncPacking+0x5b8>
    109a:	82cc                                    	bnez.n	a2, 10a6 <EncPacking+0x5c2>

0000109c <EncPacking+0x5b8>:
    109c:	3f2d                                    	ae_l16si.n	a2, a15, 2
    109e:	114480                                    	slli	a4, a4, 8
    10a1:	202240                                    	or	a2, a2, a4
    10a4:	7f2d                                    	ae_s16i.n	a2, a15, 2
    10a6:	9028f0                                    	addx2	a2, a8, a15
    10a9:	296322                                    	s32i	a2, a3, 164
    10ac:	f01d                                    	retw.n

000010ae <EncPacking+0x5ca>:
	...

000010b0 <SBC_Encoder_Init>:
    10b0:	004136                                    	entry	a1, 32
    10b3:	030c                                    	movi.n	a3, 0
    10b5:	04d242                                    	addmi	a4, a2, 0x400
    10b8:	206432                                    	s32i	a3, a4, 128
    10bb:	850c                                    	movi.n	a5, 8
    10bd:	216432                                    	s32i	a3, a4, 132
    10c0:	fd0c                                    	movi.n	a13, 15
    10c2:	246432                                    	s32i	a3, a4, 144
    10c5:	ae1c                                    	movi.n	a14, 26
    10c7:	fbeb31                                    	l32r	a3, 74 (66c <SbcAnalysisInit>)
    10ca:	1f2c                                    	movi.n	a15, 33
    10cc:	02ad                                    	mov.n	a10, a2
    10ce:	226452                                    	s32i	a5, a4, 136
    10d1:	2364d2                                    	s32i	a13, a4, 140
    10d4:	2564e2                                    	s32i	a14, a4, 148
    10d7:	2a64f2                                    	s32i	a15, a4, 168
    10da:	0003e0                                    	callx8	a3
    10dd:	f01d                                    	retw.n

000010df <SBC_Encoder_Init+0x2f>:
	...

000010e0 <sbc_enc_bit_alloc_mono>:
    10e0:	004136                                    	entry	a1, 32
    10e3:	04d2f2                                    	addmi	a15, a2, 0x400
    10e6:	028d                                    	mov.n	a8, a2
    10e8:	222f92                                    	l32i	a9, a15, 136
    10eb:	02d2a2                                    	addmi	a10, a2, 0x200
    10ee:	04d842                                    	addmi	a4, a8, 0x400
    10f1:	fbe131                                    	l32r	a3, 78 (17a0 <sbc_enc_as16Offset8>)
    10f4:	120c                                    	movi.n	a2, 1
    10f6:	220760a0080539ce                  	{ blti.w15	a9, 1, 11be <sbc_enc_bit_alloc_mono+0xde>; nop }
    10fe:	202422                                    	l32i	a2, a4, 128
    1101:	1142b0                                    	slli	a4, a2, 5
    1104:	020c                                    	movi.n	a2, 0
    1106:	334a                                    	add.n	a3, a3, a4
    1108:	0a4d                                    	mov.n	a4, a10
    110a:	1d8976                                    	loop	a9, 112b <sbc_enc_bit_alloc_mono+0x4b>
    110d:	102452                                    	l32i	a5, a4, 64
    1110:	f58c                                    	beqz.n	a5, 1123 <sbc_enc_bit_alloc_mono+0x43>
    1112:	0368                                    	l32i.n	a6, a3, 0
    1114:	c05560                                    	sub	a5, a5, a6
    1117:	726250                                    	salt	a6, a2, a5
    111a:	400600                                    	ssr	a6
    111d:	915050                                    	srl	a5, a5
    1120:	000046                                    	j	1125 <sbc_enc_bit_alloc_mono+0x45>

00001123 <sbc_enc_bit_alloc_mono+0x43>:
    1123:	b57c                                    	movi.n	a5, -5
    1125:	0459                                    	s32i.n	a5, a4, 0
    1127:	444b                                    	addi.n	a4, a4, 4
    1129:	334b                                    	addi.n	a3, a3, 4

0000112b <sbc_enc_bit_alloc_mono+0x4b>:
    112b:	244090                                    	extui	a4, a9, 0, 3
    112e:	050c                                    	movi.n	a5, 0
    1130:	0a2d                                    	mov.n	a2, a10
    1132:	413390                                    	srli	a3, a9, 3
    1135:	069476                                    	loopnez	a4, 113f <sbc_enc_bit_alloc_mono+0x5f>
    1138:	0278                                    	l32i.n	a7, a2, 0
    113a:	224b                                    	addi.n	a2, a2, 4
    113c:	535750                                    	max	a5, a7, a5
    113f:	f30b                                    	addi.n	a15, a3, -1
    1141:	077316                                    	beqz	a3, 11bc <sbc_enc_bit_alloc_mono+0xdc>
    1144:	0248                                    	l32i.n	a4, a2, 0
    1146:	42b8                                    	l32i.n	a11, a2, 16
    1148:	1268                                    	l32i.n	a6, a2, 4
    114a:	52e8                                    	l32i.n	a14, a2, 20
    114c:	022272                                    	l32i	a7, a2, 8
    114f:	0322d2                                    	l32i	a13, a2, 12
    1152:	534450                                    	max	a4, a4, a5
    1155:	534640                                    	max	a4, a6, a4
    1158:	062252                                    	l32i	a5, a2, 24
    115b:	534740                                    	max	a4, a7, a4
    115e:	072272                                    	l32i	a7, a2, 28
    1161:	534d40                                    	max	a4, a13, a4
    1164:	20c222                                    	addi	a2, a2, 32
    1167:	534b40                                    	max	a4, a11, a4
    116a:	203ee0                                    	or	a3, a14, a14
    116d:	429f76                                    	loopnez	a15, 11b3 <sbc_enc_bit_alloc_mono+0xd3>
    1170:	040683c00489023e                  	{ l32i	a3, a2, 0; max	a4, a3, a4 }
    1178:	140685c00c89024e                  	{ l32i	a4, a2, 4; max	a5, a5, a4 }
    1180:	535750                                    	max	a5, a7, a5
    1183:	2703a24018a0635e                  	{ max	a3, a3, a5; l32i	a6, a2, 8 }
    118b:	3503a24018a0743e                  	{ max	a3, a4, a3; l32i	a5, a2, 12 }
    1193:	360382410481124e                  	{ l32i	a4, a2, 16; l32i	a7, a2, 28 }
    119b:	1303a24110a1763e                  	{ max	a6, a6, a3; l32i	a3, a2, 20 }
    11a3:	2503a24110a1756e                  	{ max	a6, a5, a6; l32i	a5, a2, 24 }
    11ab:	0302a24100a8646e                  	{ max	a4, a4, a6; addi	a2, a2, 32 }
    11b3:	532340                                    	max	a2, a3, a4
    11b6:	532520                                    	max	a2, a5, a2
    11b9:	535720                                    	max	a5, a7, a2

000011bc <sbc_enc_bit_alloc_mono+0xdc>:
    11bc:	251b                                    	addi.n	a2, a5, 1
    11be:	04d832                                    	addmi	a3, a8, 0x400
    11c1:	604020                                    	neg	a4, a2
    11c4:	252332                                    	l32i	a3, a3, 148
    11c7:	060c                                    	movi.n	a6, 0
    11c9:	e50c                                    	movi.n	a5, 14
    11cb:	000786                                    	j	11ed <sbc_enc_bit_alloc_mono+0x10d>

000011ce <sbc_enc_bit_alloc_mono+0xee>:
    11ce:	060c                                    	movi.n	a6, 0
    11d0:	118976                                    	loop	a9, 11e5 <sbc_enc_bit_alloc_mono+0x105>
    11d3:	0278                                    	l32i.n	a7, a2, 0
    11d5:	747a                                    	add.n	a7, a4, a7
    11d7:	083577                                    	bltu	a5, a7, 11e3 <sbc_enc_bit_alloc_mono+0x103>
    11da:	378c                                    	beqz.n	a7, 11e1 <sbc_enc_bit_alloc_mono+0x101>
    11dc:	661b                                    	addi.n	a6, a6, 1
    11de:	000046                                    	j	11e3 <sbc_enc_bit_alloc_mono+0x103>

000011e1 <sbc_enc_bit_alloc_mono+0x101>:
    11e1:	662b                                    	addi.n	a6, a6, 2
    11e3:	224b                                    	addi.n	a2, a2, 4

000011e5 <sbc_enc_bit_alloc_mono+0x105>:
    11e5:	c02360                                    	sub	a2, a3, a6
    11e8:	441b                                    	addi.n	a4, a4, 1
    11ea:	0c12a6                                    	blti	a2, 1, 11fa <sbc_enc_bit_alloc_mono+0x11a>

000011ed <sbc_enc_bit_alloc_mono+0x10d>:
    11ed:	c03360                                    	sub	a3, a3, a6
    11f0:	060c                                    	movi.n	a6, 0
    11f2:	0a2d                                    	mov.n	a2, a10
    11f4:	d619e6                                    	bgei	a9, 1, 11ce <sbc_enc_bit_alloc_mono+0xee>
    11f7:	fffa86                                    	j	11e5 <sbc_enc_bit_alloc_mono+0x105>

000011fa <sbc_enc_bit_alloc_mono+0x11a>:
    11fa:	f67c                                    	movi.n	a6, -1
    11fc:	230740a0080599ce                  	{ blti.w15	a9, 1, 1292 <sbc_enc_bit_alloc_mono+0x1b2>; nop }
    1204:	605040                                    	neg	a5, a4
    1207:	30f460                                    	xor	a15, a4, a6
    120a:	60a272                                    	movi	a7, 0x260
    120d:	835f30                                    	moveqz	a5, a15, a3
    1210:	932330                                    	movnez	a2, a3, a3
    1213:	387a                                    	add.n	a3, a8, a7
    1215:	0a6d                                    	mov.n	a6, a10
    1217:	041c                                    	movi.n	a4, 16
    1219:	752b                                    	addi.n	a7, a5, 2
    121b:	168976                                    	loop	a9, 1235 <sbc_enc_bit_alloc_mono+0x155>
    121e:	06b8                                    	l32i.n	a11, a6, 0
    1220:	080c                                    	movi.n	a8, 0
    1222:	c0cb50                                    	sub	a12, a11, a5
    1225:	072b77                                    	blt	a11, a7, 1230 <sbc_enc_bit_alloc_mono+0x150>
    1228:	8c8d                                    	ae_sext16	a8, a12
    122a:	72bc40                                    	salt	a11, a12, a4
    122d:	8384b0                                    	moveqz	a8, a4, a11
    1230:	186682                                    	s32i	a8, a6, 96
    1233:	664b                                    	addi.n	a6, a6, 4

00001235 <sbc_enc_bit_alloc_mono+0x155>:
    1235:	01c542                                    	addi	a4, a5, 1
    1238:	3612a6                                    	blti	a2, 1, 1272 <sbc_enc_bit_alloc_mono+0x192>
    123b:	00a052                                    	movi	a5, 0
    123e:	0da062                                    	movi	a6, 13
    1241:	2a8076                                    	loop	a0, 126f <sbc_enc_bit_alloc_mono+0x18f>
    1244:	182a72                                    	l32i	a7, a10, 96
    1247:	fec782                                    	addi	a8, a7, -2
    124a:	063687                                    	bltu	a6, a8, 1254 <sbc_enc_bit_alloc_mono+0x174>
    124d:	871b                                    	addi.n	a8, a7, 1
    124f:	f77c                                    	movi.n	a7, -1
    1251:	0002c6                                    	j	1260 <sbc_enc_bit_alloc_mono+0x180>

00001254 <sbc_enc_bit_alloc_mono+0x174>:
    1254:	280c                                    	movi.n	a8, 2
    1256:	0b22a6                                    	blti	a2, 2, 1265 <sbc_enc_bit_alloc_mono+0x185>
    1259:	0a78                                    	l32i.n	a7, a10, 0
    125b:	069747                                    	bne	a7, a4, 1265 <sbc_enc_bit_alloc_mono+0x185>
    125e:	e77c                                    	movi.n	a7, -2
    1260:	186a82                                    	s32i	a8, a10, 96
    1263:	227a                                    	add.n	a2, a2, a7
    1265:	551b                                    	addi.n	a5, a5, 1
    1267:	aa4b                                    	addi.n	a10, a10, 4
    1269:	05a597                                    	bge	a5, a9, 1272 <sbc_enc_bit_alloc_mono+0x192>
    126c:	0212a6                                    	blti	a2, 1, 1272 <sbc_enc_bit_alloc_mono+0x192>

0000126f <sbc_enc_bit_alloc_mono+0x18f>:
    126f:	fff386                                    	j	1241 <sbc_enc_bit_alloc_mono+0x161>

00001272 <sbc_enc_bit_alloc_mono+0x192>:
    1272:	040c                                    	movi.n	a4, 0
    1274:	1a12a6                                    	blti	a2, 1, 1292 <sbc_enc_bit_alloc_mono+0x1b2>
    1277:	148076                                    	loop	a0, 128f <sbc_enc_bit_alloc_mono+0x1af>
    127a:	0358                                    	l32i.n	a5, a3, 0
    127c:	651b                                    	addi.n	a6, a5, 1
    127e:	03b5e6                                    	bgei	a5, 16, 1285 <sbc_enc_bit_alloc_mono+0x1a5>
    1281:	0369                                    	s32i.n	a6, a3, 0
    1283:	220b                                    	addi.n	a2, a2, -1
    1285:	441b                                    	addi.n	a4, a4, 1
    1287:	334b                                    	addi.n	a3, a3, 4
    1289:	05a497                                    	bge	a4, a9, 1292 <sbc_enc_bit_alloc_mono+0x1b2>
    128c:	0212a6                                    	blti	a2, 1, 1292 <sbc_enc_bit_alloc_mono+0x1b2>

0000128f <sbc_enc_bit_alloc_mono+0x1af>:
    128f:	fff906                                    	j	1277 <sbc_enc_bit_alloc_mono+0x197>

00001292 <sbc_enc_bit_alloc_mono+0x1b2>:
    1292:	f01d                                    	retw.n

00001294 <__do_global_ctors_aux>:
    1294:	004136                                    	entry	a1, 32
    1297:	fb7931                                    	l32r	a3, 7c (15b4 <__CTOR_END__>)
    129a:	fcc322                                    	addi	a2, a3, -4
    129d:	002222                                    	l32i	a2, a2, 0
    12a0:	020283a4001e920e                  	{ beqi.w15	a2, -1, 12b5 <__do_global_ctors_aux+0x21>; addi	a3, a3, -8 }

000012a8 <__do_global_ctors_aux+0x14>:
    12a8:	0002e0                                    	callx8	a2
    12ab:	0328                                    	l32i.n	a2, a3, 0
    12ad:	0302e3bc07ffb26e                  	{ bnei.w15	a2, -1, 12a8 <__do_global_ctors_aux+0x14>; addi	a3, a3, -4 }
    12b5:	f01d                                    	retw.n

000012b7 <__do_global_ctors_aux+0x23>:
	...

000012b8 <_fini>:
    12b8:	008136                                    	entry	a1, 64
    12bb:	fb7181                                    	l32r	a8, 80 (9c <__do_global_dtors_aux>)
    12be:	f03d                                    	nop.n
    12c0:	0008e0                                    	callx8	a8

000012c3 <_fini+0xb>:
    12c3:	f01d                                    	retw.n
