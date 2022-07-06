
Build/lib/pisplit_cpd_position_independent.splitload.lib:     file format elf32-xtensa-le


Disassembly of section .text:

00000000 <_init-0x14c>:
       0:	000001ac 00001a90 00002220 000021e4     ........ "...!..
      10:	00000000 000021c0 00000000 00002224     .....!......$"..
      20:	0000223c 00002020 00001814 00002038     <"..  ......8 ..
      30:	00001828 00002050 00000594 00002068     (...P ......h ..
      40:	00000fa0 00002080 00000a54 00002098     ..... ..T.... ..
      50:	000016b4 000020b0 00001934 000020c0     ..... ..4.... ..
      60:	000019f0 000020d0 00000f80 000020f4     ..... ....... ..
      70:	00000f90 00002118 00001698 00002138     .....!......8!..
      80:	00000260 000021f0 00002150 00007fff     `....!..P!......
      90:	18000000 00004800 00002800 e8000000     .....H...(......
      a0:	48000000 28000000 000002f0 00001ac8     ...H...(........
      b0:	00001ab0 00000580 00000ccd 00001890     ................
      c0:	0000036c 000018c4 000003c4 20000000     l.............. 
      d0:	00400000 7fff0000 000004e0 00000698     ..@.............
      e0:	00000c44 00000bc8 00001000 00800000     D...............
      f0:	08000000 00001830 7fffffff 40000000     ....0..........@
     100:	00000e64 3fffffff 00040000 00000d70     d......?....p...
     110:	00000eb8 00000e84 0000044c 0000102c     ........L...,...
     120:	000010c4 000011e8 000012ec 0000fffd     ................
     130:	000015e4 00001908 000008a8 00002000     ............. ..
     140:	00001fc8 000021c4 00000170              .....!..p...

0000014c <_init>:
     14c:	008136                                    	entry	a1, 64
     14f:	ffac81                                    	l32r	a8, 0 (1ac <frame_dummy>)
     152:	f03d                                    	nop.n
     154:	0008e0                                    	callx8	a8
     157:	ffab81                                    	l32r	a8, 4 (1a90 <__do_global_ctors_aux>)
     15a:	f03d                                    	nop.n
     15c:	0008e0                                    	callx8	a8
     15f:	f01d                                    	retw.n

00000161 <_init+0x15>:
	...

00000170 <__do_global_dtors_aux>:
     170:	004136                                    	entry	a1, 32
     173:	ffa531                                    	l32r	a3, 8 (2220 <__bss_start>)
     176:	000382                                    	l8ui	a8, a3, 0
     179:	d8ec                                    	bnez.n	a8, 1aa <__do_global_dtors_aux+0x3a>
     17b:	ffa421                                    	l32r	a2, c (21e4 <p$7655_3_1>)
     17e:	02b8                                    	l32i.n	a11, a2, 0
     180:	0ba8                                    	l32i.n	a10, a11, 0
     182:	ca8c                                    	beqz.n	a10, 192 <__do_global_dtors_aux+0x22>

00000184 <__do_global_dtors_aux+0x14>:
     184:	9b4b                                    	addi.n	a9, a11, 4
     186:	0299                                    	s32i.n	a9, a2, 0
     188:	000ae0                                    	callx8	a10
     18b:	02b8                                    	l32i.n	a11, a2, 0
     18d:	0ba8                                    	l32i.n	a10, a11, 0
     18f:	ff1a56                                    	bnez	a10, 184 <__do_global_dtors_aux+0x14>
     192:	ff9fa1                                    	l32r	a10, 10 (0 <_text_start>)
     195:	0b0c                                    	movi.n	a11, 0
     197:	081ab7                                    	beq	a10, a11, 1a3 <__do_global_dtors_aux+0x33>
     19a:	ff9ea1                                    	l32r	a10, 14 (21c0 <__EH_FRAME_BEGIN___100>)
     19d:	ff9c81                                    	l32r	a8, 10 (0 <_text_start>)
     1a0:	0008e0                                    	callx8	a8
     1a3:	1c0c                                    	movi.n	a12, 1
     1a5:	0043c2                                    	s8i	a12, a3, 0
     1a8:	f01d                                    	retw.n

000001aa <__do_global_dtors_aux+0x3a>:
     1aa:	f01d                                    	retw.n

000001ac <frame_dummy>:
     1ac:	004136                                    	entry	a1, 32
     1af:	ff9a81                                    	l32r	a8, 18 (0 <_text_start>)
     1b2:	090c                                    	movi.n	a9, 0
     1b4:	0b1897                                    	beq	a8, a9, 1c3 <frame_dummy+0x17>
     1b7:	ff97a1                                    	l32r	a10, 14 (21c0 <__EH_FRAME_BEGIN___100>)
     1ba:	ff98b1                                    	l32r	a11, 1c (2224 <object$7674_5_1>)
     1bd:	ff9681                                    	l32r	a8, 18 (0 <_text_start>)
     1c0:	0008e0                                    	callx8	a8

000001c3 <frame_dummy+0x17>:
     1c3:	f01d                                    	retw.n

000001c5 <frame_dummy+0x19>:
     1c5:	000000                                        ...

000001c8 <_start>:
     1c8:	004136                                    	entry	a1, 32
     1cb:	ff96a1                                    	l32r	a10, 24 (2020 <_DYNAMIC+0x4f8>)
     1ce:	ff9431                                    	l32r	a3, 20 (223c <printf_ptr>)
     1d1:	0288                                    	l32i.n	a8, a2, 0
     1d3:	ff95b1                                    	l32r	a11, 28 (1814 <get_CPD_memsize>)
     1d6:	0389                                    	s32i.n	a8, a3, 0
     1d8:	0008e0                                    	callx8	a8
     1db:	0388                                    	l32i.n	a8, a3, 0
     1dd:	ff93a1                                    	l32r	a10, 2c (2038 <_DYNAMIC+0x510>)
     1e0:	ff94b1                                    	l32r	a11, 30 (1828 <get_INS_memsize>)
     1e3:	0008e0                                    	callx8	a8
     1e6:	0388                                    	l32i.n	a8, a3, 0
     1e8:	ff93a1                                    	l32r	a10, 34 (2050 <_DYNAMIC+0x528>)
     1eb:	ff93b1                                    	l32r	a11, 38 (594 <compander_VO_init>)
     1ee:	0008e0                                    	callx8	a8
     1f1:	0388                                    	l32i.n	a8, a3, 0
     1f3:	ff92a1                                    	l32r	a10, 3c (2068 <_DYNAMIC+0x540>)
     1f6:	ff92b1                                    	l32r	a11, 40 (fa0 <compander_AU_init>)
     1f9:	0008e0                                    	callx8	a8
     1fc:	0388                                    	l32i.n	a8, a3, 0
     1fe:	ff91a1                                    	l32r	a10, 44 (2080 <_DYNAMIC+0x558>)
     201:	ff91b1                                    	l32r	a11, 48 (a54 <compander_VO_proc>)
     204:	0008e0                                    	callx8	a8
     207:	0388                                    	l32i.n	a8, a3, 0
     209:	ff90a1                                    	l32r	a10, 4c (2098 <_DYNAMIC+0x570>)
     20c:	ff91b1                                    	l32r	a11, 50 (16b4 <compander_AU_proc>)
     20f:	0008e0                                    	callx8	a8
     212:	0388                                    	l32i.n	a8, a3, 0
     214:	ff90a1                                    	l32r	a10, 54 (20b0 <_DYNAMIC+0x588>)
     217:	ff90b1                                    	l32r	a11, 58 (1934 <INS_Init>)
     21a:	0008e0                                    	callx8	a8
     21d:	0388                                    	l32i.n	a8, a3, 0
     21f:	ff8fa1                                    	l32r	a10, 5c (20c0 <_DYNAMIC+0x598>)
     222:	ff8fb1                                    	l32r	a11, 60 (19f0 <INS_Prcs>)
     225:	0008e0                                    	callx8	a8
     228:	0388                                    	l32i.n	a8, a3, 0
     22a:	ff8ea1                                    	l32r	a10, 64 (20d0 <_DYNAMIC+0x5a8>)
     22d:	ff8eb1                                    	l32r	a11, 68 (f80 <compander_AU_SetFrame120_init>)
     230:	0008e0                                    	callx8	a8
     233:	0388                                    	l32i.n	a8, a3, 0
     235:	ff8da1                                    	l32r	a10, 6c (20f4 <_DYNAMIC+0x5cc>)
     238:	ff8eb1                                    	l32r	a11, 70 (f90 <compander_AU_SetFrame8_init>)
     23b:	0008e0                                    	callx8	a8
     23e:	0388                                    	l32i.n	a8, a3, 0
     240:	ff8da1                                    	l32r	a10, 74 (2118 <_DYNAMIC+0x5f0>)
     243:	ff8db1                                    	l32r	a11, 78 (1698 <compander_AU_SetINS_proc>)
     246:	0008e0                                    	callx8	a8
     249:	0388                                    	l32i.n	a8, a3, 0
     24b:	ff8ca1                                    	l32r	a10, 7c (2138 <_DYNAMIC+0x610>)
     24e:	ff8cb1                                    	l32r	a11, 80 (260 <SVN_version>)
     251:	0008e0                                    	callx8	a8
     254:	ff8c21                                    	l32r	a2, 84 (21f0 <export_parameter_array>)
     257:	f01d                                    	retw.n

00000259 <_start+0x91>:
     259:	00000000 36000000                                .......

00000260 <SVN_version>:
     260:	004136                                    	entry	a1, 32
     263:	42a422                                    	movi	a2, 0x442
     266:	f01d                                    	retw.n

00000268 <HIFI_sqrt_cpd>:
     268:	004136                                    	entry	a1, 32
     26b:	0368                                    	l32i.n	a6, a3, 0
     26d:	ff86a1                                    	l32r	a10, 88 (2150 <TABLE_SQRT>)
     270:	050c                                    	movi.n	a5, 0
     272:	0516e6                                    	bgei	a6, 1, 27b <HIFI_sqrt_cpd+0x13>
     275:	0259                                    	s32i.n	a5, a2, 0

00000277 <HIFI_sqrt_cpd+0xf>:
     277:	0359                                    	s32i.n	a5, a3, 0
     279:	f01d                                    	retw.n

0000027b <HIFI_sqrt_cpd+0x13>:
     27b:	0288                                    	l32i.n	a8, a2, 0
     27d:	40e6c0                                    	nsa	a12, a6
     280:	c088c0                                    	sub	a8, a8, a12
     283:	401c00                                    	ssl	a12
     286:	047080                                    	extui	a7, a8, 0, 1
     289:	a1b600                                    	sll	a11, a6
     28c:	400700                                    	ssr	a7
     28f:	b1b0b0                                    	sra	a11, a11
     292:	41b9b0                                    	srli	a11, a11, 9
     295:	3190b0                                    	srai	a9, a11, 16
     298:	f0c992                                    	addi	a9, a9, -16
     29b:	539950                                    	max	a9, a9, a5
     29e:	9099a0                                    	addx2	a9, a9, a10
     2a1:	29ad                                    	ae_l16si.n	a10, a9, 0
     2a3:	399d                                    	ae_l16si.n	a9, a9, 2
     2a5:	1b0699401cbaebbe                  	{ ae_cvtp24a16x2.ll	aed3, a11, a11; sub	a9, a9, a10 }
     2ad:	11aa00                                    	slli	a10, a10, 16
     2b0:	119900                                    	slli	a9, a9, 16
     2b3:	0ead456b11e193d0c480813900011a0f 	{ ae_cvt48a32	aed1, a10; ae_cvt48a32	aed2, a9; nop; nop }
     2c3:	20834f0b3443b13e                  	{ nop; nop; ae_mulaq32sp16u.l	aed1, aed2, aed3 }
     2cb:	0ead217b0d4c0ce8c30021380c01015f 	{ nop; nop; nop; ae_roundsp16q48x2asym	aed0, aed1, aed1 }
     2db:	3906874000f1cb5e                  	{ ae_trunca16p24s.l	a5, aed0; add	a7, a7, a8 }
     2e3:	217170                                    	srai	a7, a7, 1
     2e6:	0279                                    	s32i.n	a7, a2, 0
     2e8:	115500                                    	slli	a5, a5, 16
     2eb:	ffe206                                    	j	277 <HIFI_sqrt_cpd+0xf>

000002ee <HIFI_sqrt_cpd+0x86>:
	...

000002f0 <Xdiv>:
     2f0:	004136                                    	entry	a1, 32
     2f3:	0248                                    	l32i.n	a4, a2, 0
     2f5:	0358                                    	l32i.n	a5, a3, 0
     2f7:	84cc                                    	bnez.n	a4, 303 <Xdiv+0x13>
     2f9:	ff6481                                    	l32r	a8, 8c (7fff <_end+0x5dbf>)
     2fc:	020c                                    	movi.n	a2, 0
     2fe:	832850                                    	moveqz	a2, a8, a5
     301:	f01d                                    	retw.n

00000303 <Xdiv+0x13>:
     303:	35cc                                    	bnez.n	a5, 30a <Xdiv+0x1a>
     305:	ff6121                                    	l32r	a2, 8c (7fff <_end+0x5dbf>)
     308:	f01d                                    	retw.n

0000030a <Xdiv+0x1a>:
     30a:	0005224010c0603e                  	{ ae_l32m.i	aed2, a3, 0; ae_l32m.i	aed0, a2, 0 }
     312:	000003f0088300fa572303002460333f 	{ ae_nsa64	a3, aed0; ae_nsa64	a4, aed2; nop }
     322:	f8c442                                    	addi	a4, a4, -8
     325:	f8c332                                    	addi	a3, a3, -8
     328:	433340                                    	min	a3, a3, a4
     32b:	e9c392                                    	addi	a9, a3, -23
     32e:	220700c010a5592e                  	{ ae_slaasq56s	aed1, aed2, a9; nop }
     336:	0ead257b0d240cd0c400813b0c01621f 	{ ae_trunca32q48	a2, aed1; nop; nop; nop }
     346:	f8c382                                    	addi	a8, a3, -8
     349:	d29c                                    	beqz.n	a2, 36a <Xdiv+0x7a>
     34b:	220700c010a5d80e                  	{ ae_slaasq56s	aed3, aed0, a8; nop }
     353:	0ead257b0d240cd0c400813b0c01643f 	{ ae_trunca32q48	a4, aed3; nop; nop; nop }
     363:	d22420                                    	quos	a2, a4, a2
     366:	a22d                                    	ae_clamps16	a2, a2
     368:	f01d                                    	retw.n

0000036a <Xdiv+0x7a>:
     36a:	f01d                                    	retw.n

0000036c <linear_interp>:
     36c:	006136                                    	entry	a1, 48
     36f:	c09340                                    	sub	a9, a3, a4
     372:	005122                                    	s16i	a2, a1, 0
     375:	200769410483601e                  	{ ae_l16m.i	aed4, a1, 0; ae_cvt48a32	aed3, a9 }
     37d:	2283531b3464b13e                  	{ nop; nop; ae_mulf48q32sp16s.l	aed3, aed3, aed4 }
     385:	280c                                    	movi.n	a8, 2
     387:	2007044300a2c83e                  	{ ae_slaaq56	aed3, aed3, a8; ae_cvt48a32	aed2, a4 }
     38f:	0ead057b08e40cd0c48081381801c12f 	{ ae_addsq56s	aed1, aed2, aed3; nop; nop; nop }
     39f:	0ead217b0d4c0cd0c50041380c01015f 	{ nop; nop; nop; ae_roundsq32f48asym	aed0, aed1 }
     3af:	0ead257b0d240cd0c400813b0c01620f 	{ ae_trunca32q48	a2, aed0; nop; nop; nop }
     3bf:	f01d                                    	retw.n

000003c1 <linear_interp+0x55>:
     3c1:	000000                                        ...

000003c4 <INS_coef_init>:
     3c4:	004136                                    	entry	a1, 32
     3c7:	ff3251                                    	l32r	a5, 90 (18000000 <_end+0x17ffddc0>)
     3ca:	210765441c08e2ae                  	{ l32r	a10, 94 (4800 <_end+0x25c0>); ae_cvt48a32	aed8, a5 }
     3d2:	ff3191                                    	l32r	a9, 98 (2800 <_end+0x5c0>)
     3d5:	1cc282                                    	addi	a8, a2, 28
     3d8:	0eace5103d2380d0c500812500a869af 	{ ae_cvtp24a16x2.ll	aed6, a9, a10; ae_l16m.i	aed7, a8, 0; ae_mov	aed5, aed8; nop }
     3e8:	2880732f3cc72d4e                  	{ l32r	a4, 9c (e8000000 <_end+0xe7ffddc0>); nop; ae_mulss32f48p16s.hh	aed5, aed6, aed7 }
     3f0:	0e9d049b09e50cd0c480934100c7340f 	{ ae_cvt48a32	aed3, a4; nop; ae_mulss32f48p16s.lh	aed8, aed6, aed7; nop }
     400:	0e9d251b0c6c0ce8c210ac190cc7023f 	{ addmi	a3, a2, 0x100; nop; ae_mulas32f48p16s.lh	aed3, aed6, aed7; ae_roundsp16q48x2asym	aed4, aed5, aed5 }
     410:	0ead297b0c650ce8c20901381401333f 	{ addi	a3, a3, -114; nop; nop; ae_roundsp16q48x2asym	aed2, aed8, aed8 }
     420:	0e6d217b0c550c48470061381c01823f 	{ ae_s16m.l.i	aed2, a3, 4; nop; nop; ae_slaisq56s	aed1, aed3, 1 }
     430:	0ead217b0c4d0ce8c30021381c01043f 	{ ae_s16m.l.i	aed4, a3, 0; nop; nop; ae_roundsp16q48x2asym	aed0, aed1, aed1 }
     440:	220740c004c5113e                  	{ ae_s16m.l.i	aed0, a3, 2; nop }
     448:	f01d                                    	retw.n

0000044a <INS_coef_init+0x86>:
	...

0000044c <AU_INS_coef_init>:
     44c:	004136                                    	entry	a1, 32
     44f:	429252                                    	l16si	a5, a2, 132
     452:	ff1391                                    	l32r	a9, a0 (48000000 <_end+0x47ffddc0>)
     455:	01d232                                    	addmi	a3, a2, 0x100
     458:	ff1381                                    	l32r	a8, a4 (28000000 <_end+0x27ffddc0>)
     45b:	090678c40408fd8e                  	{ l32r	a8, 90 (18000000 <_end+0x17ffddc0>); ae_movda32x2	aed8, a8, a9 }
     463:	210768440406ee4e                  	{ l32r	a4, 9c (e8000000 <_end+0xe7ffddc0>); ae_cvt48a32	aed6, a8 }
     46b:	0ead650b252143d0c40081200466550f 	{ slli	a5, a5, 16; ae_cvt48a32	aed4, a4; ae_mov	aed3, aed6; nop }
     47b:	230740c118e515ee                  	{ ae_movda32	aed7, a5; nop }
     483:	0e9d283b0d440c3889169a300ce8015f 	{ nop; nop; ae_mulsf32ra.lh	aed6, aed7, aed8; ae_mulaf32ra.ll	aed4, aed7, aed8 }
     493:	0e9d243b0d440cd0c5009b180ce8015f 	{ nop; nop; ae_mulsf32ra.ll	aed3, aed7, aed8; nop }
     4a3:	0eace564811233d0c59441380c01414f 	{ ae_slaisq56s	aed1, aed4, 1; addi	a3, a3, -112; nop; ae_roundsq32f48asym	aed5, aed6 }
     4b3:	0ead217b0d3d0cd0c58841390001443f 	{ ae_s32m.i	aed5, a3, 0; nop; nop; ae_roundsq32f48asym	aed2, aed3 }
     4c3:	0ead217b0d2d0cd0c58041390001823f 	{ ae_s32m.i	aed2, a3, 8; nop; nop; ae_roundsq32f48asym	aed0, aed1 }
     4d3:	220760c000c5b13e                  	{ ae_s32m.i	aed0, a3, 4; nop }
     4db:	f01d                                    	retw.n

000004dd <AU_INS_coef_init+0x91>:
     4dd:	000000                                        ...

000004e0 <INS_inter_gain>:
     4e0:	006136                                    	entry	a1, 48
     4e3:	0d9292                                    	l16si	a9, a2, 26
     4e6:	fef081                                    	l32r	a8, a8 (2f0 <Xdiv>)
     4e9:	01ad                                    	mov.n	a10, a1
     4eb:	119900                                    	slli	a9, a9, 16
     4ee:	4cc2b2                                    	addi	a11, a2, 76
     4f1:	0199                                    	s32i.n	a9, a1, 0
     4f3:	0008e0                                    	callx8	a8
     4f6:	479292                                    	l16si	a9, a2, 142
     4f9:	b14b                                    	addi.n	a11, a1, 4
     4fb:	feeb81                                    	l32r	a8, a8 (2f0 <Xdiv>)
     4fe:	11aa00                                    	slli	a10, a10, 16
     501:	11a9                                    	s32i.n	a10, a1, 4
     503:	119900                                    	slli	a9, a9, 16
     506:	01ad                                    	mov.n	a10, a1
     508:	0199                                    	s32i.n	a9, a1, 0
     50a:	0008e0                                    	callx8	a8
     50d:	4992d2                                    	l16si	a13, a2, 146
     510:	4892c2                                    	l16si	a12, a2, 144
     513:	11b8                                    	l32i.n	a11, a1, 4
     515:	11cc00                                    	slli	a12, a12, 16
     518:	11dd00                                    	slli	a13, a13, 16
     51b:	b2bbd0                                    	mulsh	a11, a11, a13
     51e:	21072c4708a60a0e                  	{ slli	a13, a10, 16; ae_cvt48a32	aed6, a12 }
     526:	21072d4618a50bfe                  	{ slli	a11, a11, 1; ae_cvt48a32	aed5, a13 }
     52e:	0ead456b20e0b3d0c48081391001c35f 	{ ae_addsq56s	aed3, aed5, aed6; ae_cvt48a32	aed4, a11; nop; nop }
     53e:	0ead057b08e40cd0c48081390001c23f 	{ ae_addsq56s	aed2, aed3, aed4; nop; nop; nop }
     54e:	0ead257b09040cd0c58081380c01422f 	{ ae_slaisq56s	aed1, aed2, 2; nop; nop; nop }
     55e:	0ead217b0d4c0ce8c30021380c01015f 	{ nop; nop; nop; ae_roundsp16q48x2asym	aed0, aed1, aed1 }
     56e:	0ead257b0e240cd0c40081391001320f 	{ ae_cvta32f24s.h	a2, aed0; nop; nop; nop }
     57e:	f01d                                    	retw.n

00000580 <compander_VO_PARA_init>:
     580:	004136                                    	entry	a1, 32
     583:	feca81                                    	l32r	a8, ac (1ac8 <xt_memcpy>)
     586:	03ad                                    	mov.n	a10, a3
     588:	b24b                                    	addi.n	a11, a2, 4
     58a:	0c1c                                    	movi.n	a12, 16
     58c:	0008e0                                    	callx8	a8
     58f:	f01d                                    	retw.n

00000591 <compander_VO_PARA_init+0x11>:
     591:	000000                                        ...

00000594 <compander_VO_init>:
     594:	004136                                    	entry	a1, 32
     597:	fec581                                    	l32r	a8, ac (1ac8 <xt_memcpy>)
     59a:	22c3b2                                    	addi	a11, a3, 34
     59d:	01d2a2                                    	addmi	a10, a2, 0x100
     5a0:	4c0c                                    	movi.n	a12, 4
     5a2:	38caa2                                    	addi	a10, a10, 56
     5a5:	0008e0                                    	callx8	a8
     5a8:	fec281                                    	l32r	a8, b0 (1ab0 <xt_memset>)
     5ab:	02ad                                    	mov.n	a10, a2
     5ad:	0b0c                                    	movi.n	a11, 0
     5af:	9ca0c2                                    	movi	a12, 156
     5b2:	0008e0                                    	callx8	a8
     5b5:	febf81                                    	l32r	a8, b4 (580 <compander_VO_PARA_init>)
     5b8:	03ad                                    	mov.n	a10, a3
     5ba:	02bd                                    	mov.n	a11, a2
     5bc:	0008e0                                    	callx8	a8
     5bf:	4a1c                                    	movi.n	a10, 20
     5c1:	febd91                                    	l32r	a9, b8 (ccd <compander_AU_PARA_init+0x105>)
     5c4:	af0c                                    	movi.n	a15, 10
     5c6:	079456                                    	bnez	a4, 643 <compander_VO_init+0xaf>
     5c9:	1852f2                                    	s16i	a15, a2, 48

000005cc <compander_VO_init+0x38>:
     5cc:	f2a9                                    	s32i.n	a10, a2, 60
     5ce:	195292                                    	s16i	a9, a2, 50
     5d1:	12a8                                    	l32i.n	a10, a2, 4
     5d3:	32c8                                    	l32i.n	a12, a2, 12
     5d5:	22b8                                    	l32i.n	a11, a2, 8
     5d7:	022ca7                                    	blt	a12, a10, 5dd <compander_VO_init+0x49>
     5da:	07aba7                                    	bge	a11, a10, 5e5 <compander_VO_init+0x51>
     5dd:	32a9                                    	s32i.n	a10, a2, 12
     5df:	0acd                                    	mov.n	a12, a10
     5e1:	22a9                                    	s32i.n	a10, a2, 8
     5e3:	0abd                                    	mov.n	a11, a10
     5e5:	feb581                                    	l32r	a8, bc (1890 <calc_slope_1>)
     5e8:	0008e0                                    	callx8	a8
     5eb:	52b8                                    	l32i.n	a11, a2, 20
     5ed:	feb481                                    	l32r	a8, c0 (36c <linear_interp>)
     5f0:	4252a2                                    	s16i	a10, a2, 132
     5f3:	12c8                                    	l32i.n	a12, a2, 4
     5f5:	8aad                                    	ae_sext16	a10, a10
     5f7:	0008e0                                    	callx8	a8
     5fa:	feb281                                    	l32r	a8, c4 (18c4 <calc_slope_2>)
     5fd:	42b8                                    	l32i.n	a11, a2, 16
     5ff:	22c8                                    	l32i.n	a12, a2, 8
     601:	0008e0                                    	callx8	a8
     604:	105242                                    	s16i	a4, a2, 32
     607:	feb081                                    	l32r	a8, c8 (3c4 <INS_coef_init>)
     60a:	feb091                                    	l32r	a9, cc (20000000 <_end+0x1fffddc0>)
     60d:	4352a2                                    	s16i	a10, a2, 134
     610:	0b0c                                    	movi.n	a11, 0
     612:	1752b2                                    	s16i	a11, a2, 46
     615:	1662b2                                    	s32i	a11, a2, 88
     618:	1362b2                                    	s32i	a11, a2, 76
     61b:	01d2a2                                    	addmi	a10, a2, 0x100
     61e:	1652b2                                    	s16i	a11, a2, 44
     621:	126292                                    	s32i	a9, a2, 72
     624:	98caa2                                    	addi	a10, a10, -104
     627:	d2a9                                    	s32i.n	a10, a2, 52
     629:	e2a9                                    	s32i.n	a10, a2, 56
     62b:	02ad                                    	mov.n	a10, a2
     62d:	0008e0                                    	callx8	a8
     630:	fea8d1                                    	l32r	a13, d0 (400000 <_end+0x3fddc0>)
     633:	0003e2                                    	l8ui	a14, a3, 0
     636:	1252e2                                    	s16i	a14, a2, 36
     639:	eca1c2                                    	movi	a12, 0x1ec
     63c:	a2d9                                    	s32i.n	a13, a2, 40
     63e:	1352c2                                    	s16i	a12, a2, 38
     641:	f01d                                    	retw.n

00000643 <compander_VO_init+0xaf>:
     643:	8b2c                                    	movi.n	a11, 40
     645:	66a6c2                                    	movi	a12, 0x666
     648:	33a3e2                                    	movi	a14, 0x333
     64b:	091466                                    	bnei	a4, 1, 658 <compander_VO_init+0xc4>
     64e:	0c9d                                    	mov.n	a9, a12
     650:	1852a2                                    	s16i	a10, a2, 48
     653:	0bad                                    	mov.n	a10, a11
     655:	ffdcc6                                    	j	5cc <compander_VO_init+0x38>

00000658 <compander_VO_init+0xc4>:
     658:	0d5c                                    	movi.n	a13, 80
     65a:	082426                                    	beqi	a4, 2, 666 <compander_VO_init+0xd2>
     65d:	0f4466                                    	bnei	a4, 4, 670 <compander_VO_init+0xdc>
     660:	09ed                                    	mov.n	a14, a9
     662:	0add                                    	mov.n	a13, a10
     664:	0fbd                                    	mov.n	a11, a15
     666:	0e9d                                    	mov.n	a9, a14
     668:	1852b2                                    	s16i	a11, a2, 48
     66b:	0dad                                    	mov.n	a10, a13
     66d:	ffd6c6                                    	j	5cc <compander_VO_init+0x38>

00000670 <compander_VO_init+0xdc>:
     670:	095466                                    	bnei	a4, 5, 67d <compander_VO_init+0xe9>
     673:	1852a2                                    	s16i	a10, a2, 48
     676:	0c9d                                    	mov.n	a9, a12
     678:	0bad                                    	mov.n	a10, a11
     67a:	ffd386                                    	j	5cc <compander_VO_init+0x38>

0000067d <compander_VO_init+0xe9>:
     67d:	096466                                    	bnei	a4, 6, 68a <compander_VO_init+0xf6>
     680:	0ecd                                    	mov.n	a12, a14
     682:	1852b2                                    	s16i	a11, a2, 48
     685:	0dbd                                    	mov.n	a11, a13
     687:	fffac6                                    	j	676 <compander_VO_init+0xe2>

0000068a <compander_VO_init+0xf6>:
     68a:	2307209e1be514ae                  	{ bne.w15	a4, a10, 5d1 <compander_VO_init+0x3d>; nop }
     692:	1852a2                                    	s16i	a10, a2, 48
     695:	fff746                                    	j	676 <compander_VO_init+0xe2>

00000698 <cpd_block_proc>:
     698:	006136                                    	entry	a1, 48
     69b:	4cc2b2                                    	addi	a11, a2, 76
     69e:	189282                                    	l16si	a8, a2, 48
     6a1:	32c252                                    	addi	a5, a2, 50
     6a4:	fec362                                    	addi	a6, a3, -2
     6a7:	0ead6487d52766d0c40081200000c05f 	{ ae_l16m.i	aed3, a5, 0; or	a10, a6, a6; ae_movi	aed0, 0; nop }
     6b7:	000081385461b0000a830309b020301f 	{ loopgtz	a8, 6e7 <cpd_block_proc+0x4f>; ae_l32m.i	aed8, a11, 0; ae_mov	aed1, aed0 }
     6c7:	230760c1048571ae                  	{ ae_l16m.iu	aed4, a10, 2; nop }
     6cf:	220700c014e5d45e                  	{ ae_abs24s	aed2, aed4; nop }
     6d7:	0e9d06fb08e40cd0c4808b001062e11f 	{ ae_max32	aed1, aed1, aed2; nop; ae_mulaf32s.hh	aed0, aed3, aed2; nop }

000006e7 <cpd_block_proc+0x4f>:
     6e7:	0ead217b0d440cd0c52c41380c01015f 	{ nop; nop; nop; ae_roundsq32f48asym	aed11, aed0 }
     6f7:	0ead057b08e40cd0c580813a040166bf 	{ ae_subsq56s	aed10, aed11, aed8; nop; nop; nop }
     707:	0ead257b09440cd0c580813a0c0183af 	{ ae_srai64	aed10, aed10, 3; nop; nop; nop }
     717:	0ead297b0d540cd0c52441380c01015f 	{ nop; nop; nop; ae_roundsq32f48asym	aed9, aed10 }
     727:	0ead057b08e40cd0c480813a0801d38f 	{ ae_addsq56s	aed7, aed8, aed9; nop; nop; nop }
     737:	0a02e24118caa0be                  	{ ae_s32m.i	aed7, a11, 0; addi	a10, a2, 40 }
     73f:	220720c110c570ae                  	{ ae_l32m.i	aed6, a10, 0; nop }
     747:	2283533334c1b13e                  	{ nop; nop; ae_mulf48q32sp16s.l	aed6, aed6, aed1 }
     74f:	0ead257b0d540cd0c51441380c01015f 	{ nop; nop; nop; ae_roundsq32f48asym	aed5, aed6 }
     75f:	220760c108c5b01e                  	{ ae_s32m.i	aed5, a1, 0; nop }
     767:	5298                                    	l32i.n	a9, a2, 20
     769:	01c8                                    	l32i.n	a12, a1, 0
     76b:	180c                                    	movi.n	a8, 1
     76d:	7e0c                                    	movi.n	a14, 7
     76f:	1692a2                                    	l16si	a10, a2, 44
     772:	0c92d2                                    	l16si	a13, a2, 24
     775:	01a9c7                                    	bge	a9, a12, 77a <cpd_block_proc+0xe2>
     778:	52c9                                    	s32i.n	a12, a2, 20

0000077a <cpd_block_proc+0xe2>:
     77a:	5cc292                                    	addi	a9, a2, 92
     77d:	2e9da7                                    	bne	a13, a10, 7af <cpd_block_proc+0x117>
     780:	1722f2                                    	l32i	a15, a2, 92
     783:	165282                                    	s16i	a8, a2, 44
     786:	53ffc0                                    	max	a15, a15, a12
     789:	11f9                                    	s32i.n	a15, a1, 4
     78b:	60c2a2                                    	addi	a10, a2, 96
     78e:	10ae76                                    	loopgtz	a14, 7a2 <cpd_block_proc+0x10a>
     791:	0ae8                                    	l32i.n	a14, a10, 0
     793:	11d8                                    	l32i.n	a13, a1, 4
     795:	53dde0                                    	max	a13, a13, a14
     798:	11d9                                    	s32i.n	a13, a1, 4
     79a:	0ab8                                    	l32i.n	a11, a10, 0
     79c:	aa4b                                    	addi.n	a10, a10, 4
     79e:	09b9                                    	s32i.n	a11, a9, 0
     7a0:	994b                                    	addi.n	a9, a9, 4

000007a2 <cpd_block_proc+0x10a>:
     7a2:	11f8                                    	l32i.n	a15, a1, 4
     7a4:	1662f2                                    	s32i	a15, a2, 88
     7a7:	09c9                                    	s32i.n	a12, a9, 0
     7a9:	1622b2                                    	l32i	a11, a2, 88
     7ac:	000586                                    	j	7c6 <cpd_block_proc+0x12e>

000007af <cpd_block_proc+0x117>:
     7af:	1e22d2                                    	l32i	a13, a2, 120
     7b2:	1622b2                                    	l32i	a11, a2, 88
     7b5:	ea1b                                    	addi.n	a14, a10, 1
     7b7:	1652e2                                    	s16i	a14, a2, 44
     7ba:	53ddc0                                    	max	a13, a13, a12
     7bd:	53bbc0                                    	max	a11, a11, a12
     7c0:	1e62d2                                    	s32i	a13, a2, 120
     7c3:	1662b2                                    	s32i	a11, a2, 88
     7c6:	12c8                                    	l32i.n	a12, a2, 4
     7c8:	122242                                    	l32i	a4, a2, 72
     7cb:	46acb7                                    	bge	a12, a11, 815 <cpd_block_proc+0x17d>
     7ce:	fe3c81                                    	l32r	a8, c0 (36c <linear_interp>)
     7d1:	4292a2                                    	l16si	a10, a2, 132
     7d4:	0008e0                                    	callx8	a8
     7d7:	21a9                                    	s32i.n	a10, a1, 8
     7d9:	2298                                    	l32i.n	a9, a2, 8
     7db:	0702820608569a9e                  	{ bge.w15	a10, a9, 9f2 <cpd_block_proc+0x35a>; addi	a7, a2, 88 }
     7e3:	fe3181                                    	l32r	a8, a8 (2f0 <Xdiv>)
     7e6:	07bd                                    	mov.n	a11, a7
     7e8:	2192a0                                    	srai	a9, a10, 2
     7eb:	a18b                                    	addi.n	a10, a1, 8
     7ed:	2199                                    	s32i.n	a9, a1, 8
     7ef:	0008e0                                    	callx8	a8
     7f2:	11aa00                                    	slli	a10, a10, 16
     7f5:	11a9                                    	s32i.n	a10, a1, 4
     7f7:	1222b2                                    	l32i	a11, a2, 72
     7fa:	21072b4318f3647e                  	{ nop; ae_cvt48a32	aed19, a11 }
     802:	c08ab0                                    	sub	a8, a10, a11
     805:	230740820c051bae                  	{ bge.w15	a11, a10, 8ab <cpd_block_proc+0x213>; nop }
     80d:	00041240006002ae                  	{ j	8b3 <cpd_block_proc+0x21b>; ae_l16m.i	aed0, a2, 0 }

00000815 <cpd_block_proc+0x17d>:
     815:	1ac2c2                                    	addi	a12, a2, 26
     818:	220760c3148570ce                  	{ ae_l16m.i	aed14, a12, 0; nop }
     820:	0ead257b0b640cd0c400813800015fef 	{ ae_slai24s	aed13, aed14, 1; nop; nop; nop }
     830:	0ead257b0d040cd0c480813b140194df 	{ ae_add24s	aed12, aed13, aed14; nop; nop; nop }
     840:	0ead257b0e240cd0c400813910017bcf 	{ ae_cvta32f24s.l	a11, aed12; nop; nop; nop }
     850:	11b9                                    	s32i.n	a11, a1, 4
     852:	1322a2                                    	l32i	a10, a2, 76
     855:	2307408418253abe                  	{ blt.w15	a10, a11, 9e0 <cpd_block_proc+0x348>; nop }
     85d:	fe1da1                                    	l32r	a10, d4 (7fff0000 <_end+0x7ffeddc0>)
     860:	1ec2d2                                    	addi	a13, a2, 30
     863:	21f2a0                                    	srai	a15, a10, 2
     866:	48c2e2                                    	addi	a14, a2, 72
     869:	11f9                                    	s32i.n	a15, a1, 4
     86b:	20072f4318d260ee                  	{ ae_l32m.i	aed15, a14, 0; ae_cvt48a32	aed18, a15 }
     873:	0eacc57088e3d0d0c580813b1c01682f 	{ ae_subsq56s	aed16, aed18, aed15; ae_l16m.i	aed17, a13, 0; nop; nop }
     883:	1e83547b3611b13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed15, aed16, aed17 }
     88b:	0ead2d7b0d5c0cd0c50041380c01015f 	{ nop; nop; nop; ae_roundsq32f48asym	aed0, aed15 }
     89b:	000003f3030000fdd6a303002060144f 	{ j	8e3 <cpd_block_proc+0x24b>; ae_trunca32q48	a11, aed0; nop }

000008ab <cpd_block_proc+0x213>:
     8ab:	220760c00485712e                  	{ ae_l16m.i	aed0, a2, 2; nop }
     8b3:	2107284318f4647e                  	{ nop; ae_cvt48a32	aed20, a8 }
     8bb:	1e83549b3680b13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed19, aed20, aed0 }
     8c3:	0ead317b0d5c0cd0c50041380c01015f 	{ nop; nop; nop; ae_roundsq32f48asym	aed0, aed19 }
     8d3:	000003f3038300fdd6a303002c60118f 	{ s32i	a8, a1, 4; ae_trunca32q48	a11, aed0; nop }
     8e3:	2f0c                                    	movi.n	a15, 2
     8e5:	189292                                    	l16si	a9, a2, 48
     8e8:	a2c8                                    	l32i.n	a12, a2, 40
     8ea:	f2d8                                    	l32i.n	a13, a2, 60
     8ec:	2007244610e1e66e                  	{ or	a10, a6, a6; ae_cvt48a32	aed1, a4 }
     8f4:	d2e8                                    	l32i.n	a14, a2, 52
     8f6:	1262b2                                    	s32i	a11, a2, 72
     8f9:	f3f6e0                                    	wur.ae_cbegin0	a14
     8fc:	0ea4e42875252bd0c400800a0115845f 	{ ae_l16m.i	aed22, a5, 0; l32i	a11, a2, 56; ae_subsq56s	aed21, aed0, aed1; nop }
     90c:	0e9ce6900d2106d0c5008eaf0eb6ddef 	{ addx2	a13, a13, a14; movi	a6, 16; ae_mulf48q32sp16s.l	aed21, aed21, aed22; nop }
     91c:	f3f7d0                                    	wur.ae_cend0	a13

0000091f <cpd_block_proc+0x287>:
     91f:	0ead756b1d29c4d0c510413f1001cbbf 	{ or	a12, a11, a11; ae_movda32	aed3, a12; nop; ae_roundsq32f48asym	aed4, aed21 }
     92f:	1d0c                                    	movi.n	a13, 1
     931:	5d29a6                                    	blti	a9, 2, 992 <cpd_block_proc+0x2fa>
     934:	6e0c                                    	movi.n	a14, 6

00000936 <cpd_block_proc+0x29e>:
     936:	0eacc573c8e2cfd0c48081390001c11f 	{ ae_addsq56s	aed1, aed1, aed4; ae_l16m.xc	aed25, a12, a15; nop; nop }
     946:	228353cb3439b13e                  	{ nop; nop; ae_mulf48q32sp16s.l	aed25, aed1, aed25 }
     94e:	220760c60845369e                  	{ ae_trunca32x2f64s	aed24, aed25, aed25, a6; nop }
     956:	0e9d261b0d440cd0c5008fc00f03015f 	{ nop; nop; ae_mulf32ra.ll	aed24, aed24, aed3; nop }
     966:	0803634300a03e8e                  	{ ae_slaaq56	aed24, aed24, a14; l16si	a9, a3, 0 }
     96e:	0ead197b09e40ce8c35f013c04010b9f 	{ s16i	a9, a11, 0; nop; nop; ae_roundsp16q48x2asym	aed23, aed24, aed24 }
     97e:	230740c51cc511ae                  	{ ae_s16m.l.iu	aed23, a10, 2; nop }
     986:	189282                                    	l16si	a8, a2, 48
     989:	dd1b                                    	addi.n	a13, a13, 1
     98b:	332b                                    	addi.n	a3, a3, 2
     98d:	bb2b                                    	addi.n	a11, a11, 2
     98f:	a32d87                                    	blt	a13, a8, 936 <cpd_block_proc+0x29e>
     992:	230760c704859fce                  	{ ae_l16m.xc	aed28, a12, a15; nop }
     99a:	228353e3341cb13e                  	{ nop; nop; ae_mulf48q32sp16s.l	aed28, aed0, aed28 }
     9a2:	220760c70045f6ce                  	{ ae_trunca32x2f64s	aed27, aed28, aed28, a6; nop }
     9aa:	0e9d261b0d440cd0c5008fd80f63015f 	{ nop; nop; ae_mulf32ra.ll	aed27, aed27, aed3; nop }
     9ba:	6e0c                                    	movi.n	a14, 6
     9bc:	0c03634300a0febe                  	{ ae_slaaq56	aed27, aed27, a14; l16si	a13, a3, 0 }
     9c4:	0ead197b09fc0ce8c36b613c04010bdf 	{ s16i	a13, a11, 0; nop; nop; ae_roundsp16q48x2asym	aed26, aed27, aed27 }
     9d4:	230740c614c511ae                  	{ ae_s16m.l.iu	aed26, a10, 2; nop }
     9dc:	e2c9                                    	s32i.n	a12, a2, 56
     9de:	f01d                                    	retw.n

000009e0 <cpd_block_proc+0x348>:
     9e0:	0d92f2                                    	l16si	a15, a2, 26
     9e3:	11ff00                                    	slli	a15, a15, 16
     9e6:	5f2fa7                                    	blt	a15, a10, a49 <cpd_block_proc+0x3b1>
     9e9:	0e92a2                                    	l16si	a10, a2, 28
     9ec:	11aa00                                    	slli	a10, a10, 16
     9ef:	ff9b46                                    	j	860 <cpd_block_proc+0x1c8>

000009f2 <cpd_block_proc+0x35a>:
     9f2:	fdb381                                    	l32r	a8, c0 (36c <linear_interp>)
     9f5:	4292a2                                    	l16si	a10, a2, 132
     9f8:	52b8                                    	l32i.n	a11, a2, 20
     9fa:	12c8                                    	l32i.n	a12, a2, 4
     9fc:	0008e0                                    	callx8	a8
     9ff:	fdb181                                    	l32r	a8, c4 (18c4 <calc_slope_2>)
     a02:	42b8                                    	l32i.n	a11, a2, 16
     a04:	22c8                                    	l32i.n	a12, a2, 8
     a06:	0008e0                                    	callx8	a8
     a09:	21b8                                    	l32i.n	a11, a1, 8
     a0b:	fdad81                                    	l32r	a8, c0 (36c <linear_interp>)
     a0e:	4352a2                                    	s16i	a10, a2, 134
     a11:	22c8                                    	l32i.n	a12, a2, 8
     a13:	8aad                                    	ae_sext16	a10, a10
     a15:	0008e0                                    	callx8	a8
     a18:	11a9                                    	s32i.n	a10, a1, 4
     a1a:	2298                                    	l32i.n	a9, a2, 8
     a1c:	fda381                                    	l32r	a8, a8 (2f0 <Xdiv>)
     a1f:	b14b                                    	addi.n	a11, a1, 4
     a21:	219290                                    	srai	a9, a9, 2
     a24:	a1cb                                    	addi.n	a10, a1, 12
     a26:	3199                                    	s32i.n	a9, a1, 12
     a28:	0008e0                                    	callx8	a8
     a2b:	2198                                    	l32i.n	a9, a1, 8
     a2d:	07bd                                    	mov.n	a11, a7
     a2f:	fd9e81                                    	l32r	a8, a8 (2f0 <Xdiv>)
     a32:	31a9                                    	s32i.n	a10, a1, 12
     a34:	219290                                    	srai	a9, a9, 2
     a37:	a14b                                    	addi.n	a10, a1, 4
     a39:	1199                                    	s32i.n	a9, a1, 4
     a3b:	0008e0                                    	callx8	a8
     a3e:	31b8                                    	l32i.n	a11, a1, 12
     a40:	d1aab0                                    	mul16s	a10, a10, a11
     a43:	11aad0                                    	slli	a10, a10, 3
     a46:	ff6ac6                                    	j	7f5 <cpd_block_proc+0x15d>

00000a49 <cpd_block_proc+0x3b1>:
     a49:	fda381                                    	l32r	a8, d8 (4e0 <INS_inter_gain>)
     a4c:	02ad                                    	mov.n	a10, a2
     a4e:	0008e0                                    	callx8	a8
     a51:	ff82c6                                    	j	860 <cpd_block_proc+0x1c8>

00000a54 <compander_VO_proc>:
     a54:	00a136                                    	entry	a1, 80
     a57:	121282                                    	l16ui	a8, a2, 36
     a5a:	0e5152                                    	s16i	a5, a1, 28
     a5d:	1cc1a2                                    	addi	a10, a1, 28
     a60:	109292                                    	l16si	a9, a2, 32
     a63:	108816                                    	beqz	a8, b6f <compander_VO_proc+0x11b>
     a66:	9f92c2                                    	l16si	a12, a2, 0x13e
     a69:	9c92b2                                    	l16si	a11, a2, 0x138
     a6c:	9d9282                                    	l16si	a8, a2, 0x13a
     a6f:	0f52b2                                    	s16i	a11, a2, 30
     a72:	9e92f2                                    	l16si	a15, a2, 0x13c
     a75:	79bc                                    	beqz.n	a9, ab0 <compander_VO_proc+0x5c>
     a77:	351926                                    	beqi	a9, 1, ab0 <compander_VO_proc+0x5c>
     a7a:	3ea1b2                                    	movi	a11, 0x13e
     a7d:	2f2926                                    	beqi	a9, 2, ab0 <compander_VO_proc+0x5c>
     a80:	220700a10025990e                  	{ beqi.w15	a9, 4, b94 <compander_VO_proc+0x140>; nop }
     a88:	220700a10825590e                  	{ beqi.w15	a9, 5, b94 <compander_VO_proc+0x140>; nop }
     a90:	1c0292211020190e                  	{ beqi.w15	a9, 6, b94 <compander_VO_proc+0x140>; addmi	a13, a2, 0x100 }
     a98:	00047a4714612dde                  	{ addi	a13, a13, 62; ae_l16m.i	aed1, a10, 0 }
     aa0:	00047d2500006cee                  	{ bnez.w15	a12, b71 <compander_VO_proc+0x11d>; ae_l16m.i	aed0, a13, 0 }
     aa8:	11e500                                    	slli	a14, a5, 16
     aab:	01e9                                    	s32i.n	a14, a1, 0
     aad:	001586                                    	j	b07 <compander_VO_proc+0xb3>

00000ab0 <compander_VO_proc+0x5c>:
     ab0:	26ad                                    	ae_l16si.n	a10, a6, 0
     ab2:	36bd                                    	ae_l16si.n	a11, a6, 2
     ab4:	090c                                    	movi.n	a9, 0
     ab6:	7299b0                                    	salt	a9, a9, a11
     ab9:	11aa00                                    	slli	a10, a10, 16
     abc:	01a9                                    	s32i.n	a10, a1, 0
     abe:	93f890                                    	movnez	a15, a8, a9
     ac1:	0f52f2                                    	s16i	a15, a2, 30
     ac4:	000fc6                                    	j	b07 <compander_VO_proc+0xb3>

00000ac7 <compander_VO_proc+0x73>:
     ac7:	2283483b3420b13e                  	{ nop; nop; ae_mulf32s.hh	aed7, aed1, aed0 }
     acf:	220700c014a5997e                  	{ ae_slaasq56s	aed6, aed7, a9; nop }
     ad7:	0ead057b08e40cd0c48081391401f16f 	{ ae_round24x2f48sasym	aed5, aed6, aed6; nop; nop; nop }
     ae7:	2283482334a2b13e                  	{ nop; nop; ae_mulf32s.hh	aed4, aed5, aed2 }
     aef:	0ead257b0d440cd0c50c41380c01015f 	{ nop; nop; nop; ae_roundsq32f48asym	aed3, aed4 }
     aff:	220760c018c5b01e                  	{ ae_s32m.i	aed3, a1, 0; nop }
     b07:	189292                                    	l16si	a9, a2, 48
     b0a:	26c272                                    	addi	a7, a2, 38
     b0d:	28c2c2                                    	addi	a12, a2, 40
     b10:	d25490                                    	quos	a5, a4, a9
     b13:	1169f0                                    	slli	a6, a9, 1
     b16:	5515a6                                    	blti	a5, 1, b6f <compander_VO_proc+0x11b>
     b19:	040c                                    	movi.n	a4, 0
     b1b:	11c9                                    	s32i.n	a12, a1, 4
     b1d:	11b8                                    	l32i.n	a11, a1, 4
     b1f:	01a8                                    	l32i.n	a10, a1, 0
     b21:	20072a4208cc60be                  	{ ae_l32m.i	aed9, a11, 0; ae_cvt48a32	aed12, a10 }
     b29:	0eacc57058e270d0c580813a0c0166cf 	{ ae_subsq56s	aed10, aed12, aed9; ae_l16m.i	aed11, a7, 0; nop; nop }
     b39:	1e83544b354bb13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed9, aed10, aed11 }
     b41:	0ead097b09ed0cd0c420413f0c016a8f 	{ l32r	a8, dc (698 <cpd_block_proc>); nop; nop; ae_roundsq32f48asym	aed8, aed9 }
     b51:	0ead6567d52622d0c400813b0c01698f 	{ ae_trunca32q48	a9, aed8; or	a10, a2, a2; nop; nop }
     b61:	03bd                                    	mov.n	a11, a3
     b63:	a299                                    	s32i.n	a9, a2, 40
     b65:	0008e0                                    	callx8	a8
     b68:	441b                                    	addi.n	a4, a4, 1
     b6a:	336a                                    	add.n	a3, a3, a6
     b6c:	ad9547                                    	bne	a5, a4, b1d <compander_VO_proc+0xc9>
     b6f:	f01d                                    	retw.n

00000b71 <compander_VO_proc+0x11d>:
     b71:	228348733420b13e                  	{ nop; nop; ae_mulf32s.hh	aed14, aed1, aed0 }
     b79:	0ead2d7b0d540cd0c53441380c01015f 	{ nop; nop; nop; ae_roundsq32f48asym	aed13, aed14 }
     b89:	220760c308c5b01e                  	{ ae_s32m.i	aed13, a1, 0; nop }
     b91:	ffdc86                                    	j	b07 <compander_VO_proc+0xb3>

00000b94 <compander_VO_proc+0x140>:
     b94:	9f92c2                                    	l16si	a12, a2, 0x13e
     b97:	2a0472400c82726e                  	{ ae_l16m.i	aed1, a6, 4; ae_l16m.x	aed2, a2, a11 }
     b9f:	290c                                    	movi.n	a9, 2
     ba1:	cd1c                                    	movi.n	a13, 28
     ba3:	2c04113907e01cee                  	{ bnez.w15	a12, ac7 <compander_VO_proc+0x73>; ae_l16m.x	aed0, a1, a13 }
     bab:	228348833420b13e                  	{ nop; nop; ae_mulf32s.hh	aed16, aed1, aed0 }
     bb3:	220720c014a5f90e                  	{ ae_slaasq56s	aed15, aed16, a9; nop }
     bbb:	220760c318c5b01e                  	{ ae_s32m.i	aed15, a1, 0; nop }
     bc3:	ffd006                                    	j	b07 <compander_VO_proc+0xb3>

00000bc6 <compander_VO_proc+0x172>:
	...

00000bc8 <compander_AU_PARA_init>:
     bc8:	004136                                    	entry	a1, 32
     bcb:	fd4571                                    	l32r	a7, e0 (c44 <compander_AU_PARA_init+0x7c>)
     bce:	14dc                                    	bnez.n	a4, be3 <compander_AU_PARA_init+0x1b>
     bd0:	0c8556                                    	bnez	a5, c9c <compander_AU_PARA_init+0xd4>
     bd3:	fd3681                                    	l32r	a8, ac (1ac8 <xt_memcpy>)
     bd6:	03ad                                    	mov.n	a10, a3
     bd8:	3cc2b2                                    	addi	a11, a2, 60
     bdb:	4c0c                                    	movi.n	a12, 4
     bdd:	0008e0                                    	callx8	a8
     be0:	003146                                    	j	ca9 <compander_AU_PARA_init+0xe1>

00000be3 <compander_AU_PARA_init+0x1b>:
     be3:	07d3a2                                    	addmi	a10, a3, 0x700
     be6:	131466                                    	bnei	a4, 1, bfd <compander_AU_PARA_init+0x35>
     be9:	a0caa2                                    	addi	a10, a10, -96
     bec:	0f5556                                    	bnez	a5, ce5 <compander_AU_PARA_init+0x11d>
     bef:	fd2f81                                    	l32r	a8, ac (1ac8 <xt_memcpy>)
     bf2:	12c2b2                                    	addi	a11, a2, 18
     bf5:	4c0c                                    	movi.n	a12, 4
     bf7:	0008e0                                    	callx8	a8
     bfa:	004586                                    	j	d14 <compander_AU_PARA_init+0x14c>

00000bfd <compander_AU_PARA_init+0x35>:
     bfd:	0cd392                                    	addmi	a9, a3, 0xc00
     c00:	332466                                    	bnei	a4, 2, c37 <compander_AU_PARA_init+0x6f>
     c03:	44c9c2                                    	addi	a12, a9, 68
     c06:	158556                                    	bnez	a5, d62 <compander_AU_PARA_init+0x19a>
     c09:	2d92a2                                    	l16si	a10, a2, 90
     c0c:	2e92b2                                    	l16si	a11, a2, 92
     c0f:	a059a2                                    	s16i	a10, a9, 0x140
     c12:	815cb2                                    	s16i	a11, a12, 0x102
     c15:	fd2581                                    	l32r	a8, ac (1ac8 <xt_memcpy>)
     c18:	0dd3a2                                    	addmi	a10, a3, 0xd00
     c1b:	5ec2b2                                    	addi	a11, a2, 94
     c1e:	4c0c                                    	movi.n	a12, 4
     c20:	48caa2                                    	addi	a10, a10, 72
     c23:	0008e0                                    	callx8	a8
     c26:	fd2181                                    	l32r	a8, ac (1ac8 <xt_memcpy>)
     c29:	32c2b2                                    	addi	a11, a2, 50
     c2c:	0dd3a2                                    	addmi	a10, a3, 0xd00
     c2f:	1c0c                                    	movi.n	a12, 1
     c31:	58caa2                                    	addi	a10, a10, 88
     c34:	0008e0                                    	callx8	a8
     c37:	5c0c                                    	movi.n	a12, 5
     c39:	fd1c81                                    	l32r	a8, ac (1ac8 <xt_memcpy>)
     c3c:	02d2b2                                    	addmi	a11, a2, 0x200
     c3f:	01d3a2                                    	addmi	a10, a3, 0x100
     c42:	80caa2                                    	addi	a10, a10, -128
     c45:	c8cbb2                                    	addi	a11, a11, -56
     c48:	0008e0                                    	callx8	a8
     c4b:	5c0c                                    	movi.n	a12, 5
     c4d:	fd1781                                    	l32r	a8, ac (1ac8 <xt_memcpy>)
     c50:	66c222                                    	addi	a2, a2, 102
     c53:	07d3a2                                    	addmi	a10, a3, 0x700
     c56:	20caa2                                    	addi	a10, a10, 32
     c59:	02bd                                    	mov.n	a11, a2
     c5b:	0008e0                                    	callx8	a8
     c5e:	fd1381                                    	l32r	a8, ac (1ac8 <xt_memcpy>)
     c61:	02bd                                    	mov.n	a11, a2
     c63:	0ed3a2                                    	addmi	a10, a3, 0xe00
     c66:	5c0c                                    	movi.n	a12, 5
     c68:	c0caa2                                    	addi	a10, a10, -64
     c6b:	0008e0                                    	callx8	a8
     c6e:	288666                                    	bnei	a6, 8, c9a <compander_AU_PARA_init+0xd2>
     c71:	2023d2                                    	l32i	a13, a3, 128
     c74:	05d3c2                                    	addmi	a12, a3, 0x500
     c77:	a37a                                    	add.n	a10, a3, a7
     c79:	5f2a92                                    	l32i	a9, a10, 0x17c
     c7c:	e8ccc2                                    	addi	a12, a12, -24
     c7f:	8e2cb2                                    	l32i	a11, a12, 0x238
     c82:	dd8b                                    	addi.n	a13, a13, 8
     c84:	998b                                    	addi.n	a9, a9, 8
     c86:	21d4d0                                    	srai	a13, a13, 4
     c89:	bb8b                                    	addi.n	a11, a11, 8
     c8b:	2063d2                                    	s32i	a13, a3, 128
     c8e:	21b4b0                                    	srai	a11, a11, 4
     c91:	219490                                    	srai	a9, a9, 4
     c94:	8e6cb2                                    	s32i	a11, a12, 0x238
     c97:	5f6a92                                    	s32i	a9, a10, 0x17c
     c9a:	f01d                                    	retw.n

00000c9c <compander_AU_PARA_init+0xd4>:
     c9c:	fd0481                                    	l32r	a8, ac (1ac8 <xt_memcpy>)
     c9f:	03ad                                    	mov.n	a10, a3
     ca1:	34c2b2                                    	addi	a11, a2, 52
     ca4:	4c0c                                    	movi.n	a12, 4
     ca6:	0008e0                                    	callx8	a8
     ca9:	fd0081                                    	l32r	a8, ac (1ac8 <xt_memcpy>)
     cac:	a38b                                    	addi.n	a10, a3, 8
     cae:	44c2b2                                    	addi	a11, a2, 68
     cb1:	9c0c                                    	movi.n	a12, 9
     cb3:	0008e0                                    	callx8	a8
     cb6:	220720be07e5f66e                  	{ bnei.w15	a6, 8, c37 <compander_AU_PARA_init+0x6f>; nop }
     cbe:	5398                                    	l32i.n	a9, a3, 20
     cc0:	23c8                                    	l32i.n	a12, a3, 8
     cc2:	43a8                                    	l32i.n	a10, a3, 16
     cc4:	33b8                                    	l32i.n	a11, a3, 12
     cc6:	998b                                    	addi.n	a9, a9, 8
     cc8:	aa8b                                    	addi.n	a10, a10, 8
     cca:	cc8b                                    	addi.n	a12, a12, 8
     ccc:	bb8b                                    	addi.n	a11, a11, 8
     cce:	21c4c0                                    	srai	a12, a12, 4
     cd1:	21b4b0                                    	srai	a11, a11, 4
     cd4:	23c9                                    	s32i.n	a12, a3, 8
     cd6:	21a4a0                                    	srai	a10, a10, 4
     cd9:	33b9                                    	s32i.n	a11, a3, 12
     cdb:	219490                                    	srai	a9, a9, 4
     cde:	43a9                                    	s32i.n	a10, a3, 16
     ce0:	5399                                    	s32i.n	a9, a3, 20
     ce2:	ffd446                                    	j	c37 <compander_AU_PARA_init+0x6f>

00000ce5 <compander_AU_PARA_init+0x11d>:
     ce5:	0c1566                                    	bnei	a5, 1, cf5 <compander_AU_PARA_init+0x12d>
     ce8:	fcf181                                    	l32r	a8, ac (1ac8 <xt_memcpy>)
     ceb:	b2ab                                    	addi.n	a11, a2, 10
     ced:	4c0c                                    	movi.n	a12, 4
     cef:	0008e0                                    	callx8	a8
     cf2:	000786                                    	j	d14 <compander_AU_PARA_init+0x14c>

00000cf5 <compander_AU_PARA_init+0x12d>:
     cf5:	0d2566                                    	bnei	a5, 2, d06 <compander_AU_PARA_init+0x13e>
     cf8:	fced81                                    	l32r	a8, ac (1ac8 <xt_memcpy>)
     cfb:	1ac2b2                                    	addi	a11, a2, 26
     cfe:	4c0c                                    	movi.n	a12, 4
     d00:	0008e0                                    	callx8	a8
     d03:	000346                                    	j	d14 <compander_AU_PARA_init+0x14c>

00000d06 <compander_AU_PARA_init+0x13e>:
     d06:	fce981                                    	l32r	a8, ac (1ac8 <xt_memcpy>)
     d09:	02d2b2                                    	addmi	a11, a2, 0x200
     d0c:	4c0c                                    	movi.n	a12, 4
     d0e:	c0cbb2                                    	addi	a11, a11, -64
     d11:	0008e0                                    	callx8	a8
     d14:	fce681                                    	l32r	a8, ac (1ac8 <xt_memcpy>)
     d17:	22c2b2                                    	addi	a11, a2, 34
     d1a:	07d3a2                                    	addmi	a10, a3, 0x700
     d1d:	9c0c                                    	movi.n	a12, 9
     d1f:	a8caa2                                    	addi	a10, a10, -88
     d22:	0008e0                                    	callx8	a8
     d25:	230700ba03e5766e                  	{ bnei.w15	a6, 8, c37 <compander_AU_PARA_init+0x6f>; nop }
     d2d:	05d3a2                                    	addmi	a10, a3, 0x500
     d30:	e8caa2                                    	addi	a10, a10, -24
     d33:	732a92                                    	l32i	a9, a10, 0x1cc
     d36:	702ad2                                    	l32i	a13, a10, 0x1c0
     d39:	722ab2                                    	l32i	a11, a10, 0x1c8
     d3c:	712ac2                                    	l32i	a12, a10, 0x1c4
     d3f:	998b                                    	addi.n	a9, a9, 8
     d41:	bb8b                                    	addi.n	a11, a11, 8
     d43:	dd8b                                    	addi.n	a13, a13, 8
     d45:	cc8b                                    	addi.n	a12, a12, 8
     d47:	21d4d0                                    	srai	a13, a13, 4
     d4a:	21c4c0                                    	srai	a12, a12, 4
     d4d:	706ad2                                    	s32i	a13, a10, 0x1c0
     d50:	21b4b0                                    	srai	a11, a11, 4
     d53:	716ac2                                    	s32i	a12, a10, 0x1c4
     d56:	219490                                    	srai	a9, a9, 4
     d59:	726ab2                                    	s32i	a11, a10, 0x1c8
     d5c:	736a92                                    	s32i	a9, a10, 0x1cc
     d5f:	ffb506                                    	j	c37 <compander_AU_PARA_init+0x6f>

00000d62 <compander_AU_PARA_init+0x19a>:
     d62:	2b92b2                                    	l16si	a11, a2, 86
     d65:	a059b2                                    	s16i	a11, a9, 0x140
     d68:	2c92b2                                    	l16si	a11, a2, 88
     d6b:	ffa8c6                                    	j	c12 <compander_AU_PARA_init+0x4a>

00000d6e <compander_AU_PARA_init+0x1a6>:
	...

00000d70 <compander_AU_Sub_init>:
     d70:	004136                                    	entry	a1, 32
     d73:	fccf81                                    	l32r	a8, b0 (1ab0 <xt_memset>)
     d76:	04ad                                    	mov.n	a10, a4
     d78:	0b0c                                    	movi.n	a11, 0
     d7a:	50a3c2                                    	movi	a12, 0x350
     d7d:	0008e0                                    	callx8	a8
     d80:	02ad                                    	mov.n	a10, a2
     d82:	fcd881                                    	l32r	a8, e4 (bc8 <compander_AU_PARA_init>)
     d85:	03bd                                    	mov.n	a11, a3
     d87:	05cd                                    	mov.n	a12, a5
     d89:	06dd                                    	mov.n	a13, a6
     d8b:	07ed                                    	mov.n	a14, a7
     d8d:	0008e0                                    	callx8	a8
     d90:	13d3a2                                    	addmi	a10, a3, 0x1300
     d93:	741a92                                    	l16ui	a9, a10, 232
     d96:	fcd621                                    	l32r	a2, f0 (8000000 <_end+0x7ffddc0>)
     d99:	fcd431                                    	l32r	a3, ec (800000 <_end+0x7fddc0>)
     d9c:	0b02caa6001b696e                  	{ bnei.w15	a9, 8, e2f <compander_AU_Sub_init+0xbf>; addi	a10, a10, -20 }
     da4:	890c                                    	movi.n	a9, 8
     da6:	fcd0b1                                    	l32r	a11, e8 (1000 <compander_AU_init+0x60>)
     da9:	1154b2                                    	s16i	a11, a4, 34
     dac:	8b0c                                    	movi.n	a11, 8

00000dae <compander_AU_Sub_init+0x3e>:
     dae:	105492                                    	s16i	a9, a4, 32
     db1:	230740a41005f50e                  	{ beqi.w15	a5, 2, e54 <compander_AU_Sub_init+0xe4>; nop }
     db9:	7e1a92                                    	l16ui	a9, a10, 252
     dbc:	ef1c                                    	movi.n	a15, 30
     dbe:	60a0c2                                    	movi	a12, 96
     dc1:	fccba1                                    	l32r	a10, f0 (8000000 <_end+0x7ffddc0>)
     dc4:	2802c9a2001d296e                  	{ bnei.w15	a9, 8, e4c <compander_AU_Sub_init+0xdc>; addi	a8, a9, -10 }
     dcc:	8c1c                                    	movi.n	a12, 24
     dce:	891c                                    	movi.n	a9, 24
     dd0:	b499                                    	s32i.n	a9, a4, 44
     dd2:	1194d2                                    	l16si	a13, a4, 34
     dd5:	a429                                    	s32i.n	a2, a4, 40
     dd7:	11ebf0                                    	slli	a14, a11, 1
     dda:	048766                                    	bnei	a7, 8, de2 <compander_AU_Sub_init+0x72>
     ddd:	a439                                    	s32i.n	a3, a4, 40
     ddf:	fcc3a1                                    	l32r	a10, ec (800000 <_end+0x7fddc0>)
     de2:	0d26a6                                    	blti	a6, 2, df3 <compander_AU_Sub_init+0x83>
     de5:	1054e2                                    	s16i	a14, a4, 32
     de8:	21d1d0                                    	srai	a13, a13, 1
     deb:	11fcf0                                    	slli	a15, a12, 1
     dee:	1154d2                                    	s16i	a13, a4, 34
     df1:	b4f9                                    	s32i.n	a15, a4, 44
     df3:	fcc081                                    	l32r	a8, f4 (1830 <calc_slope>)
     df6:	54b8                                    	l32i.n	a11, a4, 20
     df8:	24c8                                    	l32i.n	a12, a4, 8
     dfa:	0008e0                                    	callx8	a8
     dfd:	0d5452                                    	s16i	a5, a4, 26
     e00:	f429                                    	s32i.n	a2, a4, 60
     e02:	3c54a2                                    	s16i	a10, a4, 120
     e05:	0c0c                                    	movi.n	a12, 0
     e07:	0f54c2                                    	s16i	a12, a4, 30
     e0a:	1364c2                                    	s32i	a12, a4, 76
     e0d:	190c                                    	movi.n	a9, 1
     e0f:	1264c2                                    	s32i	a12, a4, 72
     e12:	01d4b2                                    	addmi	a11, a4, 0x100
     e15:	0e5492                                    	s16i	a9, a4, 28
     e18:	fcb8a1                                    	l32r	a10, f8 (7fffffff <_end+0x7fffddbf>)
     e1b:	a0cbb2                                    	addi	a11, a11, -96
     e1e:	c4b9                                    	s32i.n	a11, a4, 48
     e20:	d4b9                                    	s32i.n	a11, a4, 52
     e22:	1064a2                                    	s32i	a10, a4, 64
     e25:	018766                                    	bnei	a7, 8, e2a <compander_AU_Sub_init+0xba>
     e28:	f439                                    	s32i.n	a3, a4, 60
     e2a:	3f5472                                    	s16i	a7, a4, 126
     e2d:	f01d                                    	retw.n

00000e2f <compander_AU_Sub_init+0xbf>:
     e2f:	00a4e2                                    	movi	a14, 0x400
     e32:	0c9966                                    	bnei	a9, 10, e42 <compander_AU_Sub_init+0xd2>
     e35:	fca0d1                                    	l32r	a13, b8 (ccd <compander_AU_PARA_init+0x105>)
     e38:	ab0c                                    	movi.n	a11, 10
     e3a:	1154d2                                    	s16i	a13, a4, 34
     e3d:	a90c                                    	movi.n	a9, 10
     e3f:	ffdac6                                    	j	dae <compander_AU_Sub_init+0x3e>

00000e42 <compander_AU_Sub_init+0xd2>:
     e42:	0b2c                                    	movi.n	a11, 32
     e44:	1154e2                                    	s16i	a14, a4, 34
     e47:	092c                                    	movi.n	a9, 32
     e49:	ffd846                                    	j	dae <compander_AU_Sub_init+0x3e>

00000e4c <compander_AU_Sub_init+0xdc>:
     e4c:	83cf80                                    	moveqz	a12, a15, a8
     e4f:	0c9d                                    	mov.n	a9, a12
     e51:	ffdec6                                    	j	dd0 <compander_AU_Sub_init+0x60>

00000e54 <compander_AU_Sub_init+0xe4>:
     e54:	fcaa91                                    	l32r	a9, fc (40000000 <_end+0x3fffddc0>)
     e57:	11abf0                                    	slli	a10, a11, 1
     e5a:	b4a9                                    	s32i.n	a10, a4, 44
     e5c:	a499                                    	s32i.n	a9, a4, 40
     e5e:	fca7a1                                    	l32r	a10, fc (40000000 <_end+0x3fffddc0>)
     e61:	ffe386                                    	j	df3 <compander_AU_Sub_init+0x83>

00000e64 <DBB_PARA_init>:
     e64:	004136                                    	entry	a1, 32
     e67:	fc9181                                    	l32r	a8, ac (1ac8 <xt_memcpy>)
     e6a:	03ad                                    	mov.n	a10, a3
     e6c:	70c2b2                                    	addi	a11, a2, 112
     e6f:	4c0c                                    	movi.n	a12, 4
     e71:	0008e0                                    	callx8	a8
     e74:	fc8e81                                    	l32r	a8, ac (1ac8 <xt_memcpy>)
     e77:	a38b                                    	addi.n	a10, a3, 8
     e79:	7cc2b2                                    	addi	a11, a2, 124
     e7c:	2c0c                                    	movi.n	a12, 2
     e7e:	0008e0                                    	callx8	a8
     e81:	f01d                                    	retw.n

00000e83 <DBB_PARA_init+0x1f>:
	...

00000e84 <DBB_init>:
     e84:	004136                                    	entry	a1, 32
     e87:	fc8a81                                    	l32r	a8, b0 (1ab0 <xt_memset>)
     e8a:	03ad                                    	mov.n	a10, a3
     e8c:	0b0c                                    	movi.n	a11, 0
     e8e:	ac0c                                    	movi.n	a12, 10
     e90:	0008e0                                    	callx8	a8
     e93:	fc9b81                                    	l32r	a8, 100 (e64 <DBB_PARA_init>)
     e96:	02ad                                    	mov.n	a10, a2
     e98:	03bd                                    	mov.n	a11, a3
     e9a:	0008e0                                    	callx8	a8
     e9d:	03a8                                    	l32i.n	a10, a3, 0
     e9f:	fc9991                                    	l32r	a9, 104 (3fffffff <_end+0x3fffddbf>)
     ea2:	0529a7                                    	blt	a9, a10, eab <DBB_init+0x27>
     ea5:	119af0                                    	slli	a9, a10, 1
     ea8:	000086                                    	j	eae <DBB_init+0x2a>

00000eab <DBB_init+0x27>:
     eab:	fc9391                                    	l32r	a9, f8 (7fffffff <_end+0x7fffddbf>)
     eae:	3399                                    	s32i.n	a9, a3, 12
     eb0:	fc90a1                                    	l32r	a10, f0 (8000000 <_end+0x7ffddc0>)
     eb3:	43a9                                    	s32i.n	a10, a3, 16
     eb5:	f01d                                    	retw.n

00000eb7 <DBB_init+0x33>:
	...

00000eb8 <AU_GBL_init>:
     eb8:	004136                                    	entry	a1, 32
     ebb:	fc7c81                                    	l32r	a8, ac (1ac8 <xt_memcpy>)
     ebe:	03ad                                    	mov.n	a10, a3
     ec0:	02bd                                    	mov.n	a11, a2
     ec2:	2c0c                                    	movi.n	a12, 2
     ec4:	0008e0                                    	callx8	a8
     ec7:	0b0c                                    	movi.n	a11, 0
     ec9:	0c1c                                    	movi.n	a12, 16
     ecb:	fc7981                                    	l32r	a8, b0 (1ab0 <xt_memset>)
     ece:	02d3a2                                    	addmi	a10, a3, 0x200
     ed1:	000392                                    	l8ui	a9, a3, 0
     ed4:	005392                                    	s16i	a9, a3, 0
     ed7:	10caa2                                    	addi	a10, a10, 16
     eda:	0008e0                                    	callx8	a8
     edd:	fc7481                                    	l32r	a8, b0 (1ab0 <xt_memset>)
     ee0:	0b0c                                    	movi.n	a11, 0
     ee2:	02d3a2                                    	addmi	a10, a3, 0x200
     ee5:	0c2c                                    	movi.n	a12, 32
     ee7:	30caa2                                    	addi	a10, a10, 48
     eea:	0008e0                                    	callx8	a8
     eed:	fc6f81                                    	l32r	a8, ac (1ac8 <xt_memcpy>)
     ef0:	a34b                                    	addi.n	a10, a3, 4
     ef2:	78c2b2                                    	addi	a11, a2, 120
     ef5:	2c0c                                    	movi.n	a12, 2
     ef7:	0008e0                                    	callx8	a8
     efa:	1e2292                                    	l32i	a9, a2, 120
     efd:	c0a252                                    	movi	a5, 0x2c0
     f00:	59ec                                    	bnez.n	a9, f29 <AU_GBL_init+0x71>
     f02:	8c2c                                    	movi.n	a12, 40
     f04:	fc6a81                                    	l32r	a8, ac (1ac8 <xt_memcpy>)
     f07:	01d2b2                                    	addmi	a11, a2, 0x100
     f0a:	02d3a2                                    	addmi	a10, a3, 0x200
     f0d:	70caa2                                    	addi	a10, a10, 112
     f10:	80cbb2                                    	addi	a11, a11, -128
     f13:	0008e0                                    	callx8	a8
     f16:	fc6581                                    	l32r	a8, ac (1ac8 <xt_memcpy>)
     f19:	a35a                                    	add.n	a10, a3, a5
     f1b:	01d2b2                                    	addmi	a11, a2, 0x100
     f1e:	8c2c                                    	movi.n	a12, 40
     f20:	d0cbb2                                    	addi	a11, a11, -48
     f23:	0008e0                                    	callx8	a8
     f26:	000986                                    	j	f50 <AU_GBL_init+0x98>

00000f29 <AU_GBL_init+0x71>:
     f29:	231966                                    	bnei	a9, 1, f50 <AU_GBL_init+0x98>
     f2c:	8c2c                                    	movi.n	a12, 40
     f2e:	fc5f81                                    	l32r	a8, ac (1ac8 <xt_memcpy>)
     f31:	01d2b2                                    	addmi	a11, a2, 0x100
     f34:	02d3a2                                    	addmi	a10, a3, 0x200
     f37:	70caa2                                    	addi	a10, a10, 112
     f3a:	20cbb2                                    	addi	a11, a11, 32
     f3d:	0008e0                                    	callx8	a8
     f40:	fc5b81                                    	l32r	a8, ac (1ac8 <xt_memcpy>)
     f43:	a35a                                    	add.n	a10, a3, a5
     f45:	01d2b2                                    	addmi	a11, a2, 0x100
     f48:	8c2c                                    	movi.n	a12, 40
     f4a:	70cbb2                                    	addi	a11, a11, 112
     f4d:	0008e0                                    	callx8	a8
     f50:	041392                                    	l16ui	a9, a3, 8
     f53:	fc5fa1                                    	l32r	a10, d0 (400000 <_end+0x3fddc0>)
     f56:	eca1c2                                    	movi	a12, 0x1ec
     f59:	ad0c                                    	movi.n	a13, 10
     f5b:	f6c9e2                                    	addi	a14, a9, -10
     f5e:	138966                                    	bnei	a9, 8, f75 <AU_GBL_init+0xbd>
     f61:	890c                                    	movi.n	a9, 8
     f63:	045392                                    	s16i	a9, a3, 8
     f66:	33a9                                    	s32i.n	a10, a3, 12
     f68:	048466                                    	bnei	a4, 8, f70 <AU_GBL_init+0xb8>
     f6b:	fc67b1                                    	l32r	a11, 108 (40000 <_end+0x3ddc0>)
     f6e:	33b9                                    	s32i.n	a11, a3, 12
     f70:	0553c2                                    	s16i	a12, a3, 10
     f73:	f01d                                    	retw.n

00000f75 <AU_GBL_init+0xbd>:
     f75:	092c                                    	movi.n	a9, 32
     f77:	839de0                                    	moveqz	a9, a13, a14
     f7a:	fff946                                    	j	f63 <AU_GBL_init+0xab>

00000f7d <AU_GBL_init+0xc5>:
     f7d:	000000                                        ...

00000f80 <compander_AU_SetFrame120_init>:
     f80:	004136                                    	entry	a1, 32
     f83:	13d242                                    	addmi	a4, a2, 0x1300
     f86:	a30c                                    	movi.n	a3, 10
     f88:	745432                                    	s16i	a3, a4, 232
     f8b:	f01d                                    	retw.n

00000f8d <compander_AU_SetFrame120_init+0xd>:
     f8d:	000000                                        ...

00000f90 <compander_AU_SetFrame8_init>:
     f90:	004136                                    	entry	a1, 32
     f93:	13d242                                    	addmi	a4, a2, 0x1300
     f96:	830c                                    	movi.n	a3, 8
     f98:	745432                                    	s16i	a3, a4, 232
     f9b:	f01d                                    	retw.n

00000f9d <compander_AU_SetFrame8_init+0xd>:
     f9d:	000000                                        ...

00000fa0 <compander_AU_init>:
     fa0:	004136                                    	entry	a1, 32
     fa3:	03ad                                    	mov.n	a10, a3
     fa5:	02bd                                    	mov.n	a11, a2
     fa7:	fc5981                                    	l32r	a8, 10c (d70 <compander_AU_Sub_init>)
     faa:	02cd                                    	mov.n	a12, a2
     fac:	0d0c                                    	movi.n	a13, 0
     fae:	05ed                                    	mov.n	a14, a5
     fb0:	04fd                                    	mov.n	a15, a4
     fb2:	0008e0                                    	callx8	a8
     fb5:	03ad                                    	mov.n	a10, a3
     fb7:	02bd                                    	mov.n	a11, a2
     fb9:	1d0c                                    	movi.n	a13, 1
     fbb:	07d262                                    	addmi	a6, a2, 0x700
     fbe:	fc5381                                    	l32r	a8, 10c (d70 <compander_AU_Sub_init>)
     fc1:	05ed                                    	mov.n	a14, a5
     fc3:	a0c662                                    	addi	a6, a6, -96
     fc6:	04fd                                    	mov.n	a15, a4
     fc8:	06cd                                    	mov.n	a12, a6
     fca:	0008e0                                    	callx8	a8
     fcd:	05ed                                    	mov.n	a14, a5
     fcf:	03ad                                    	mov.n	a10, a3
     fd1:	02bd                                    	mov.n	a11, a2
     fd3:	fc4e81                                    	l32r	a8, 10c (d70 <compander_AU_Sub_init>)
     fd6:	2d0c                                    	movi.n	a13, 2
     fd8:	0dd2c2                                    	addmi	a12, a2, 0xd00
     fdb:	04fd                                    	mov.n	a15, a4
     fdd:	40ccc2                                    	addi	a12, a12, 64
     fe0:	0008e0                                    	callx8	a8
     fe3:	fc4b81                                    	l32r	a8, 110 (eb8 <AU_GBL_init>)
     fe6:	04cd                                    	mov.n	a12, a4
     fe8:	14d2b2                                    	addmi	a11, a2, 0x1400
     feb:	03ad                                    	mov.n	a10, a3
     fed:	e0cbb2                                    	addi	a11, a11, -32
     ff0:	0008e0                                    	callx8	a8
     ff3:	fc4881                                    	l32r	a8, 114 (e84 <DBB_init>)
     ff6:	17d2b2                                    	addmi	a11, a2, 0x1700
     ff9:	03ad                                    	mov.n	a10, a3
     ffb:	f0cbb2                                    	addi	a11, a11, -16
     ffe:	0008e0                                    	callx8	a8
    1001:	fc4581                                    	l32r	a8, 118 (44c <AU_INS_coef_init>)
    1004:	02ad                                    	mov.n	a10, a2
    1006:	0008e0                                    	callx8	a8
    1009:	fc4381                                    	l32r	a8, 118 (44c <AU_INS_coef_init>)
    100c:	06ad                                    	mov.n	a10, a6
    100e:	0008e0                                    	callx8	a8
    1011:	05d292                                    	addmi	a9, a2, 0x500
    1014:	0cd2a2                                    	addmi	a10, a2, 0xc00
    1017:	8c29c2                                    	l32i	a12, a9, 0x230
    101a:	8d29b2                                    	l32i	a11, a9, 0x234
    101d:	746ac2                                    	s32i	a12, a10, 0x1d0
    1020:	756ab2                                    	s32i	a11, a10, 0x1d4
    1023:	8e2992                                    	l32i	a9, a9, 0x238
    1026:	766a92                                    	s32i	a9, a10, 0x1d8
    1029:	f01d                                    	retw.n

0000102b <compander_AU_init+0x8b>:
	...

0000102c <F_CPD_Ins_Gain>:
    102c:	006136                                    	entry	a1, 48
    102f:	01ad                                    	mov.n	a10, a1
    1031:	fc1d81                                    	l32r	a8, a8 (2f0 <Xdiv>)
    1034:	01d2b2                                    	addmi	a11, a2, 0x100
    1037:	202292                                    	l32i	a9, a2, 128
    103a:	0199                                    	s32i.n	a9, a1, 0
    103c:	8ccbb2                                    	addi	a11, a11, -116
    103f:	0008e0                                    	callx8	a8
    1042:	fc1981                                    	l32r	a8, a8 (2f0 <Xdiv>)
    1045:	11a9                                    	s32i.n	a10, a1, 4
    1047:	b14b                                    	addi.n	a11, a1, 4
    1049:	2422c2                                    	l32i	a12, a2, 144
    104c:	119a00                                    	slli	a9, a10, 16
    104f:	01c9                                    	s32i.n	a12, a1, 0
    1051:	01ad                                    	mov.n	a10, a1
    1053:	1199                                    	s32i.n	a9, a1, 4
    1055:	0008e0                                    	callx8	a8
    1058:	2522f2                                    	l32i	a15, a2, 148
    105b:	01f9                                    	s32i.n	a15, a1, 0
    105d:	11d8                                    	l32i.n	a13, a1, 4
    105f:	2622e2                                    	l32i	a14, a2, 152
    1062:	b2dde0                                    	mulsh	a13, a13, a14
    1065:	21072f4710a60a0e                  	{ slli	a14, a10, 16; ae_cvt48a32	aed6, a15 }
    106d:	21072e4708a50dfe                  	{ slli	a13, a13, 1; ae_cvt48a32	aed5, a14 }
    1075:	0ead456b20e0d3d0c48081391001c35f 	{ ae_addsq56s	aed3, aed5, aed6; ae_cvt48a32	aed4, a13; nop; nop }
    1085:	0ead057b08e40cd0c48081390001c23f 	{ ae_addsq56s	aed2, aed3, aed4; nop; nop; nop }
    1095:	0ead257b09040cd0c58081380c01422f 	{ ae_slaisq56s	aed1, aed2, 2; nop; nop; nop }
    10a5:	0ead217b0d4c0cd0c50041380c01015f 	{ nop; nop; nop; ae_roundsq32f48asym	aed0, aed1 }
    10b5:	220760c000c5b01e                  	{ ae_s32m.i	aed0, a1, 0; nop }
    10bd:	0128                                    	l32i.n	a2, a1, 0
    10bf:	f01d                                    	retw.n

000010c1 <F_CPD_Ins_Gain+0x95>:
    10c1:	000000                                        ...

000010c4 <F_CPD_Mag_In>:
    10c4:	004136                                    	entry	a1, 32
    10c7:	109282                                    	l16si	a8, a2, 32
    10ca:	22c272                                    	addi	a7, a2, 34
    10cd:	0eace487e52333d0c40081200080c07f 	{ ae_l16m.i	aed3, a7, 0; addi	a3, a3, -4; ae_movi	aed4, 0; nop }
    10dd:	0020f0                                    	nop
    10e0:	0e6cf519153c3120451021200404984f 	{ loopgtz	a8, 1108 <F_CPD_Mag_In+0x44>; ae_l32m.iu	aed2, a3, 4; ae_mov	aed0, aed4; ae_mov	aed1, aed4 }
    10f0:	220700c014e5d23e                  	{ ae_abs64	aed2, aed2; nop }
    10f8:	0e9d201b0d240c30c3804e081043813f 	{ ae_l32m.iu	aed2, a3, 4; nop; ae_mulaf48q32sp16s.l	aed1, aed2, aed3; ae_max64	aed0, aed0, aed2 }

00001108 <F_CPD_Mag_In+0x44>:
    1108:	222566                                    	bnei	a5, 2, 112e <F_CPD_Mag_In+0x6a>
    110b:	fcc432                                    	addi	a3, a4, -4
    110e:	1105434700a2184e                  	{ loopgtz	a8, 112e <F_CPD_Mag_In+0x6a>; ae_l32m.iu	aed2, a3, 4 }
    1116:	220700c10ce5d23e                  	{ ae_abs64	aed5, aed2; nop }
    111e:	0e9cc41910e431d0c4808e090ca3000f 	{ ae_max64	aed0, aed0, aed5; ae_l32m.iu	aed2, a3, 4; ae_mulaf48q32sp16s.l	aed1, aed5, aed3; nop }
    112e:	0ead043b09e50cd0c40080070560f63f 	{ l32r	a3, f8 (7fffffff <_end+0x7fffddbf>); nop; ae_sat48s	aed0, aed0; nop }
    113e:	230740a00c05166e                  	{ bnei.w15	a6, 1, 11e4 <F_CPD_Mag_In+0x120>; nop }
    1146:	0ead257b0d440cd0c50841380c01015f 	{ nop; nop; nop; ae_roundsq32f48asym	aed2, aed4 }
    1156:	0ead217b0d4c0cd0c50441380c01015f 	{ nop; nop; nop; ae_roundsq32f48asym	aed1, aed1 }
    1166:	0ead057b08c40cd0c50081381001041f 	{ ae_lt64	b0, aed1, aed2; nop; nop; nop }
    1176:	fbdc91                                    	l32r	a9, e8 (1000 <compander_AU_init+0x60>)
    1179:	1b0292420ca0e99e                  	{ ae_cvtp24a16x2.ll	aed9, a9, a9; addmi	a10, a2, 0x100 }
    1181:	030480                                    	rsr.br	a8
    1184:	8ccaa2                                    	addi	a10, a10, -116
    1187:	048080                                    	extui	a8, a8, 0, 1
    118a:	00050a20000768ae                  	{ beqz.w15	a8, 119a <F_CPD_Mag_In+0xd6>; ae_l32m.i	aed7, a10, 0 }
    1192:	220740c008e5126e                  	{ ae_mov	aed1, aed2; nop }

0000119a <F_CPD_Mag_In+0xd6>:
    119a:	0ead057b08e40cd0c58081391c01641f 	{ ae_subsq56s	aed8, aed1, aed7; nop; nop; nop }
    11aa:	1e83543b3509b13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed7, aed8, aed9 }
    11b2:	0ead257b0d5c0cd0c51841380c01015f 	{ nop; nop; nop; ae_roundsq32f48asym	aed6, aed7 }
    11c2:	0ead257b0d240cd0c400813b0c01636f 	{ ae_trunca32q48	a3, aed6; nop; nop; nop }
    11d2:	000003f3038300f957a303003c60123f 	{ s32i	a3, a2, 140; ae_trunca32q48	a2, aed0; nop }

000011e2 <F_CPD_Mag_In+0x11e>:
    11e2:	f01d                                    	retw.n

000011e4 <F_CPD_Mag_In+0x120>:
    11e4:	fffa86                                    	j	11d2 <F_CPD_Mag_In+0x10e>

000011e7 <F_CPD_Mag_In+0x123>:
	...

000011e8 <F_CPD_Max_In_Buf>:
    11e8:	006136                                    	entry	a1, 48
    11eb:	a288                                    	l32i.n	a8, a2, 40
    11ed:	1c0c                                    	movi.n	a12, 1
    11ef:	0e92b2                                    	l16si	a11, a2, 28
    11f2:	0c9292                                    	l16si	a9, a2, 24
    11f5:	1222a2                                    	l32i	a10, a2, 72
    11f8:	04a837                                    	bge	a8, a3, 1200 <F_CPD_Max_In_Buf+0x18>
    11fb:	a239                                    	s32i.n	a3, a2, 40
    11fd:	0f52c2                                    	s16i	a12, a2, 30

00001200 <F_CPD_Max_In_Buf+0x18>:
    1200:	5e99b7                                    	bne	a9, a11, 1262 <F_CPD_Max_In_Buf+0x7a>
    1203:	1422d2                                    	l32i	a13, a2, 80
    1206:	50c292                                    	addi	a9, a2, 80
    1209:	0e52c2                                    	s16i	a12, a2, 28
    120c:	53dda0                                    	max	a13, a13, a10
    120f:	7b0c                                    	movi.n	a11, 7
    1211:	01d9                                    	s32i.n	a13, a1, 0
    1213:	54c2a2                                    	addi	a10, a2, 84
    1216:	10ab76                                    	loopgtz	a11, 122a <F_CPD_Max_In_Buf+0x42>
    1219:	0a88                                    	l32i.n	a8, a10, 0
    121b:	01f8                                    	l32i.n	a15, a1, 0
    121d:	53ff80                                    	max	a15, a15, a8
    1220:	01f9                                    	s32i.n	a15, a1, 0
    1222:	0ae8                                    	l32i.n	a14, a10, 0
    1224:	aa4b                                    	addi.n	a10, a10, 4
    1226:	09e9                                    	s32i.n	a14, a9, 0
    1228:	994b                                    	addi.n	a9, a9, 4

0000122a <F_CPD_Max_In_Buf+0x42>:
    122a:	01c8                                    	l32i.n	a12, a1, 0
    122c:	1222b2                                    	l32i	a11, a2, 72
    122f:	1362c2                                    	s32i	a12, a2, 76
    1232:	09b9                                    	s32i.n	a11, a9, 0
    1234:	0f92a2                                    	l16si	a10, a2, 30
    1237:	126232                                    	s32i	a3, a2, 72
    123a:	1e1aa6                                    	blti	a10, 1, 125c <F_CPD_Max_In_Buf+0x74>
    123d:	fbad81                                    	l32r	a8, f4 (1830 <calc_slope>)
    1240:	a2a8                                    	l32i.n	a10, a2, 40
    1242:	52b8                                    	l32i.n	a11, a2, 20
    1244:	22c8                                    	l32i.n	a12, a2, 8
    1246:	0008e0                                    	callx8	a8
    1249:	fb90b1                                    	l32r	a11, 8c (7fff <_end+0x5dbf>)
    124c:	80dac2                                    	addmi	a12, a10, 0xffff8000
    124f:	83abc0                                    	moveqz	a10, a11, a12
    1252:	01a9                                    	s32i.n	a10, a1, 0
    1254:	090c                                    	movi.n	a9, 0
    1256:	3c52a2                                    	s16i	a10, a2, 120
    1259:	0f5292                                    	s16i	a9, a2, 30

0000125c <F_CPD_Max_In_Buf+0x74>:
    125c:	1322a2                                    	l32i	a10, a2, 76
    125f:	0004c6                                    	j	1276 <F_CPD_Max_In_Buf+0x8e>

00001262 <F_CPD_Max_In_Buf+0x7a>:
    1262:	1322c2                                    	l32i	a12, a2, 76
    1265:	db1b                                    	addi.n	a13, a11, 1
    1267:	53aa30                                    	max	a10, a10, a3
    126a:	0e52d2                                    	s16i	a13, a2, 28
    126d:	1262a2                                    	s32i	a10, a2, 72
    1270:	53aac0                                    	max	a10, a10, a12
    1273:	1362a2                                    	s32i	a10, a2, 76
    1276:	22b8                                    	l32i.n	a11, a2, 8
    1278:	49aba7                                    	bge	a11, a10, 12c5 <F_CPD_Max_In_Buf+0xdd>
    127b:	21072b4318e1647e                  	{ nop; ae_cvt48a32	aed1, a11 }
    1283:	c0aab0                                    	sub	a10, a10, a11
    1286:	78c2b2                                    	addi	a11, a2, 120
    1289:	20076a401c8260be                  	{ ae_l16m.i	aed3, a11, 0; ae_cvt48a32	aed2, a10 }
    1291:	1e83540b3443b13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed1, aed2, aed3 }
    1299:	0ead017b09ed0cd0c400413f0401878f 	{ l32r	a8, a8 (2f0 <Xdiv>); nop; nop; ae_roundsq32f48asym	aed0, aed1 }
    12a9:	0eace56045222ad0c400813b0c01690f 	{ ae_trunca32q48	a9, aed0; addi	a10, a2, 8; nop; nop }
    12b9:	01bd                                    	mov.n	a11, a1
    12bb:	0199                                    	s32i.n	a9, a1, 0
    12bd:	0008e0                                    	callx8	a8
    12c0:	112a00                                    	slli	a2, a10, 16
    12c3:	f01d                                    	retw.n

000012c5 <F_CPD_Max_In_Buf+0xdd>:
    12c5:	2022a2                                    	l32i	a10, a2, 128
    12c8:	2322b2                                    	l32i	a11, a2, 140
    12cb:	90caa0                                    	addx2	a12, a10, a10
    12ce:	102bc7                                    	blt	a11, a12, 12e2 <F_CPD_Max_In_Buf+0xfa>
    12d1:	fb8921                                    	l32r	a2, f8 (7fffffff <_end+0x7fffddbf>)
    12d4:	f01d                                    	retw.n

000012d6 <F_CPD_Max_In_Buf+0xee>:
    12d6:	fb9181                                    	l32r	a8, 11c (102c <F_CPD_Ins_Gain>)
    12d9:	02ad                                    	mov.n	a10, a2
    12db:	0008e0                                    	callx8	a8
    12de:	0a2d                                    	mov.n	a2, a10
    12e0:	f01d                                    	retw.n

000012e2 <F_CPD_Max_In_Buf+0xfa>:
    12e2:	f02ab7                                    	blt	a10, a11, 12d6 <F_CPD_Max_In_Buf+0xee>
    12e5:	4292a2                                    	l16si	a10, a2, 132
    12e8:	fff506                                    	j	12c0 <F_CPD_Max_In_Buf+0xd8>

000012eb <F_CPD_Max_In_Buf+0x103>:
	...

000012ec <F_CPD_Intp_Out>:
    12ec:	008136                                    	entry	a1, 64
    12ef:	1022c2                                    	l32i	a12, a2, 64
    12f2:	0a5172                                    	s16i	a7, a1, 20
    12f5:	4f1c                                    	movi.n	a15, 20
    12f7:	2f04114314e555ce                  	{ sub	a14, a5, a12; ae_l16m.x	aed5, a1, a15 }
    12ff:	0ead456b21e1e3d0c480813900013c0f 	{ ae_cvt48a32	aed3, a12; ae_cvt48a32	aed4, a14; nop; nop }
    130f:	1e81f41a2085c2de                  	{ l32i	a13, a2, 48; nop; ae_mulaf48q32sp16s.l	aed3, aed4, aed5 }
    1317:	f3f6d0                                    	wur.ae_cbegin0	a13
    131a:	0eacc16111fa28d0c5084138140132af 	{ l32i	a10, a2, 56; addi	a8, a2, 34; nop; ae_roundsq32f48asym	aed2, aed3 }
    132a:	0eace5687d242bd0c400813b0c01692f 	{ ae_trunca32q48	a9, aed2; l32i	a11, a2, 60; nop; nop }
    133a:	0ead65651d23a9d0c500813f00011bcf 	{ ae_movda32x2	aed1, a11, a12; ae_movda32x2	aed3, a10, a9; nop; nop }
    134a:	0e1cc20869fc271b872b4e0d0421429f 	{ s32i	a9, a2, 64; l32i	a7, a2, 52; ae_mulf32ra.lh	aed1, aed1, aed1; ae_mulf32ra.lh	aed3, aed3, aed3 }
    135a:	3403e2410482708e                  	{ ae_l16m.i	aed4, a8, 0; l32i	a5, a2, 44 }
    1362:	0e64d02861ec2b01051083080c8341af 	{ l32i	a10, a1, 68; l32i	a11, a2, 48; ae_sub64	aed3, aed3, aed1; ae_sel16i	aed4, aed4, aed4, 1 }
    1372:	0000006b300000f945046a1c3004766f 	{ bnei.w15	a6, 2, 1385 <F_CPD_Intp_Out+0x99>; l16si	a9, a2, 126; ae_mulf48q32sp16s.l	aed3, aed3, aed4 }
    1382:	1155f0                                    	slli	a5, a5, 1

00001385 <F_CPD_Intp_Out+0x99>:
    1385:	1021e2                                    	l32i	a14, a1, 64
    1388:	a0b5b0                                    	addx4	a11, a5, a11
    138b:	1092d2                                    	l16si	a13, a2, 32
    138e:	075d                                    	mov.n	a5, a7
    1390:	f3f7b0                                    	wur.ae_cend0	a11
    1393:	b99a                                    	add.n	a11, a9, a9
    1395:	068966                                    	bnei	a9, 8, 139f <F_CPD_Intp_Out+0xb3>
    1398:	939ba0                                    	movnez	a9, a11, a10
    139b:	0001c6                                    	j	13a6 <F_CPD_Intp_Out+0xba>

0000139e <F_CPD_Intp_Out+0xb2>:
	...

0000139f <F_CPD_Intp_Out+0xb3>:
    139f:	4c0c                                    	movi.n	a12, 4
    13a1:	994b                                    	addi.n	a9, a9, 4
    13a3:	839ca0                                    	moveqz	a9, a12, a10
    13a6:	0a1c                                    	movi.n	a10, 16
    13a8:	4b0c                                    	movi.n	a11, 4
    13aa:	3a05452504009eee                  	{ bnez.w15	a14, 145f <F_CPD_Intp_Out+0x173>; ae_l32m.xc	aed0, a5, a11 }
    13b2:	0c02e4a0103fa66e                  	{ bnei.w15	a6, 2, 158a <F_CPD_Intp_Out+0x29e>; addi	a12, a4, -4 }
    13ba:	fcc362                                    	addi	a6, a3, -4
    13bd:	8fad76                                    	loopgtz	a13, 1450 <F_CPD_Intp_Out+0x164>
    13c0:	0ea5241b09440cd0c48081180561fa0f 	{ ae_trunca32x2f64s	aed11, aed0, aed0, a10; nop; ae_add64	aed1, aed1, aed3; nop }
    13d0:	220720c008451a1e                  	{ ae_trunca32x2f64s	aed8, aed1, aed1, a10; nop }
    13d8:	0e9d261b0d440cd0c5008f580d68015f 	{ nop; nop; ae_mulf32ra.ll	aed11, aed11, aed8; nop }
    13e8:	0803a34300a0c9be                  	{ ae_slaaq56	aed11, aed11, a9; l32i	a8, a3, 0 }
    13f0:	0ead097b09fc0cd0c528413d0401078f 	{ s32i	a8, a7, 0; nop; nop; ae_roundsq32f48asym	aed10, aed11 }
    1400:	230760c210c5b16e                  	{ ae_s32m.iu	aed10, a6, 4; nop }
    1408:	230720c208c59b5e                  	{ ae_l32m.xc	aed9, a5, a11; nop }
    1410:	220700c20c45da9e                  	{ ae_trunca32x2f64s	aed7, aed9, aed9, a10; nop }
    1418:	0e9d261b0d440cd0c5008f380ce8015f 	{ nop; nop; ae_mulf32ra.ll	aed7, aed7, aed8; nop }
    1428:	0e03844304a0d97e                  	{ ae_slaaq56	aed7, aed7, a9; l32i	a15, a4, 0 }
    1430:	0eacc56021fa33d0c518413d0c0107ff 	{ s32i	a15, a7, 4; addi	a3, a3, 4; nop; ae_roundsq32f48asym	aed6, aed7 }
    1440:	0502e44110c1a1ce                  	{ ae_s32m.iu	aed6, a12, 4; addi	a4, a4, 4 }
    1448:	0702a74000c29b5e                  	{ ae_l32m.xc	aed0, a5, a11; addi	a7, a7, 8 }

00001450 <F_CPD_Intp_Out+0x164>:
    1450:	004906                                    	j	1578 <F_CPD_Intp_Out+0x28c>

00001453 <F_CPD_Intp_Out+0x167>:
	...

0000145f <F_CPD_Intp_Out+0x173>:
    145f:	fcc4c2                                    	addi	a12, a4, -4
    1462:	220740a01405b66e                  	{ bnei.w15	a6, 2, 151a <F_CPD_Intp_Out+0x22e>; nop }
    146a:	fcc362                                    	addi	a6, a3, -4
    146d:	9fad76                                    	loopgtz	a13, 1510 <F_CPD_Intp_Out+0x224>
    1470:	0ea5241b09840cd0c480811805617a0f 	{ ae_trunca32x2f64s	aed17, aed0, aed0, a10; nop; ae_add64	aed1, aed1, aed3; nop }
    1480:	220720c00c459a1e                  	{ ae_trunca32x2f64s	aed14, aed1, aed1, a10; nop }
    1488:	0e9d261b0d440cd0c5008f880e2e015f 	{ nop; nop; ae_mulf32ra.ll	aed17, aed17, aed14; nop }
    1498:	220740c300a5791e                  	{ ae_slaaq56	aed17, aed17, a9; nop }
    14a0:	0f03c34408e0018e                  	{ ae_neg64	aed17, aed17; l32i	a14, a3, 0 }
    14a8:	0ead117b09ec0cd0c540413d040107ef 	{ s32i	a14, a7, 0; nop; nop; ae_roundsq32f48asym	aed16, aed17 }
    14b8:	230760c400c5b16e                  	{ ae_s32m.iu	aed16, a6, 4; nop }
    14c0:	230720c318c59b5e                  	{ ae_l32m.xc	aed15, a5, a11; nop }
    14c8:	220720c31c455afe                  	{ ae_trunca32x2f64s	aed13, aed15, aed15, a10; nop }
    14d0:	0e9d261b0d440cd0c5008f680dae015f 	{ nop; nop; ae_mulf32ra.ll	aed13, aed13, aed14; nop }
    14e0:	220720c304a559de                  	{ ae_slaaq56	aed13, aed13, a9; nop }
    14e8:	0c03c44308e01d8e                  	{ ae_neg64	aed13, aed13; l32i	a13, a4, 0 }
    14f0:	0eaccd6021ea33d0c530413d0c0107df 	{ s32i	a13, a7, 4; addi	a3, a3, 4; nop; ae_roundsq32f48asym	aed12, aed13 }
    1500:	0502e44300c1a1ce                  	{ ae_s32m.iu	aed12, a12, 4; addi	a4, a4, 4 }
    1508:	0702a74000c29b5e                  	{ ae_l32m.xc	aed0, a5, a11; addi	a7, a7, 8 }

00001510 <F_CPD_Intp_Out+0x224>:
    1510:	001906                                    	j	1578 <F_CPD_Intp_Out+0x28c>

00001513 <F_CPD_Intp_Out+0x227>:
    1513:	00000000 62000000                                .......

0000151a <F_CPD_Intp_Out+0x22e>:
    151a:	fcc362                                    	addi	a6, a3, -4
    151d:	57ad76                                    	loopgtz	a13, 1578 <F_CPD_Intp_Out+0x28c>
    1520:	0ea5241b09840cd0c48081180561fa0f 	{ ae_trunca32x2f64s	aed19, aed0, aed0, a10; nop; ae_add64	aed1, aed1, aed3; nop }
    1530:	220740c00c451a1e                  	{ ae_trunca32x2f64s	aed20, aed1, aed1, a10; nop }
    1538:	0e9d261b0d440cd0c5008f980e74015f 	{ nop; nop; ae_mulf32ra.ll	aed19, aed19, aed20; nop }
    1548:	220740c300a5f93e                  	{ ae_slaaq56	aed19, aed19, a9; nop }
    1550:	0f03c34418e0138e                  	{ ae_neg64	aed19, aed19; l32i	a15, a3, 0 }
    1558:	0ead117b09fc0cd0c548413d040107ff 	{ s32i	a15, a7, 0; nop; nop; ae_roundsq32f48asym	aed18, aed19 }
    1568:	0302e34410c1b16e                  	{ ae_s32m.iu	aed18, a6, 4; addi	a3, a3, 4 }
    1570:	0702a74000c19b5e                  	{ ae_l32m.xc	aed0, a5, a11; addi	a7, a7, 4 }
    1578:	c87c                                    	movi.n	a8, -4
    157a:	230720c508c5985e                  	{ ae_l32m.xc	aed21, a5, a8; nop }
    1582:	d259                                    	s32i.n	a5, a2, 52
    1584:	f01d                                    	retw.n

00001586 <F_CPD_Intp_Out+0x29a>:
    1586:	00000000                                ....

0000158a <F_CPD_Intp_Out+0x29e>:
    158a:	fcc362                                    	addi	a6, a3, -4
    158d:	4fad76                                    	loopgtz	a13, 15e0 <F_CPD_Intp_Out+0x2f4>
    1590:	0ea5241b09a40cd0c48081180561fa0f 	{ ae_trunca32x2f64s	aed23, aed0, aed0, a10; nop; ae_add64	aed1, aed1, aed3; nop }
    15a0:	220760c008451a1e                  	{ ae_trunca32x2f64s	aed24, aed1, aed1, a10; nop }
    15a8:	0e9d261b0d440cd0c5008fb80ef8015f 	{ nop; nop; ae_mulf32ra.ll	aed23, aed23, aed24; nop }
    15b8:	0c03c34304a0e97e                  	{ ae_slaaq56	aed23, aed23, a9; l32i	a12, a3, 0 }
    15c0:	0ead157b09fc0cd0c558413d040107cf 	{ s32i	a12, a7, 0; nop; nop; ae_roundsq32f48asym	aed22, aed23 }
    15d0:	0302e34510c1b16e                  	{ ae_s32m.iu	aed22, a6, 4; addi	a3, a3, 4 }
    15d8:	0702a74000c19b5e                  	{ ae_l32m.xc	aed0, a5, a11; addi	a7, a7, 4 }

000015e0 <F_CPD_Intp_Out+0x2f4>:
    15e0:	ffe506                                    	j	1578 <F_CPD_Intp_Out+0x28c>

000015e3 <F_CPD_Intp_Out+0x2f7>:
	...

000015e4 <multiband_cpd_module>:
    15e4:	006136                                    	entry	a1, 48
    15e7:	06ed                                    	mov.n	a14, a6
    15e9:	facd81                                    	l32r	a8, 120 (10c4 <F_CPD_Mag_In>)
    15ec:	02ad                                    	mov.n	a10, a2
    15ee:	03bd                                    	mov.n	a11, a3
    15f0:	04cd                                    	mov.n	a12, a4
    15f2:	05dd                                    	mov.n	a13, a5
    15f4:	0008e0                                    	callx8	a8
    15f7:	30070a43148202ae                  	{ l32i	a10, a2, 56; ae_movda32	aed2, a10 }
    15ff:	3903424018ef1aee                  	{ ae_movda32	aed3, a10; l16si	a9, a2, 126 }
    1607:	0e9d261b0d440cd0c5008f100c43015f 	{ nop; nop; ae_mulf32ra.ll	aed2, aed2, aed3; nop }
    1617:	fac381                                    	l32r	a8, 124 (11e8 <F_CPD_Max_In_Buf>)
    161a:	220700c010a5592e                  	{ ae_slaasq56s	aed1, aed2, a9; nop }
    1622:	0ead217b0d4c0cd0c50041380c01015f 	{ nop; nop; nop; ae_roundsq32f48asym	aed0, aed1 }
    1632:	0ead6567d52622d0c400813b0c016b0f 	{ ae_trunca32q48	a11, aed0; or	a10, a2, a2; nop; nop }
    1642:	0008e0                                    	callx8	a8
    1645:	1322b2                                    	l32i	a11, a2, 76
    1648:	22c8                                    	l32i.n	a12, a2, 8
    164a:	0392f2                                    	l16si	a15, a2, 6
    164d:	32e8                                    	l32i.n	a14, a2, 12
    164f:	0add                                    	mov.n	a13, a10
    1651:	2322a2                                    	l32i	a10, a2, 140
    1654:	02acb7                                    	bge	a12, a11, 165a <multiband_cpd_module+0x76>
    1657:	000606                                    	j	1673 <multiband_cpd_module+0x8f>

0000165a <multiband_cpd_module+0x76>:
    165a:	05aeb7                                    	bge	a14, a11, 1663 <multiband_cpd_module+0x7f>
    165d:	0092f2                                    	l16si	a15, a2, 0
    1660:	0003c6                                    	j	1673 <multiband_cpd_module+0x8f>

00001663 <multiband_cpd_module+0x7f>:
    1663:	4298                                    	l32i.n	a9, a2, 16
    1665:	019282                                    	l16si	a8, a2, 2
    1668:	0292f2                                    	l16si	a15, a2, 4
    166b:	7299b0                                    	salt	a9, a9, a11
    166e:	93f890                                    	movnez	a15, a8, a9
    1671:	8ffd                                    	ae_sext16	a15, a15
    1673:	2022b2                                    	l32i	a11, a2, 128
    1676:	90bbb0                                    	addx2	a11, a11, a11
    1679:	02aab7                                    	bge	a10, a11, 167f <multiband_cpd_module+0x9b>
    167c:	4492f2                                    	l16si	a15, a2, 136
    167f:	05ed                                    	mov.n	a14, a5
    1681:	04cd                                    	mov.n	a12, a4
    1683:	03bd                                    	mov.n	a11, a3
    1685:	faa881                                    	l32r	a8, 128 (12ec <F_CPD_Intp_Out>)
    1688:	0179                                    	s32i.n	a7, a1, 0
    168a:	c198                                    	l32i.n	a9, a1, 48
    168c:	02ad                                    	mov.n	a10, a2
    168e:	1199                                    	s32i.n	a9, a1, 4
    1690:	0008e0                                    	callx8	a8
    1693:	f01d                                    	retw.n

00001695 <multiband_cpd_module+0xb1>:
    1695:	000000                                        ...

00001698 <compander_AU_SetINS_proc>:
    1698:	004136                                    	entry	a1, 32
    169b:	faa461                                    	l32r	a6, 12c (fffd <_end+0xddbd>)
    169e:	13d222                                    	addmi	a2, a2, 0x1300
    16a1:	711252                                    	l16ui	a5, a2, 226
    16a4:	240c                                    	movi.n	a4, 2
    16a6:	204540                                    	or	a4, a5, a4
    16a9:	105560                                    	and	a5, a5, a6
    16ac:	935430                                    	movnez	a5, a4, a3
    16af:	715252                                    	s16i	a5, a2, 226
    16b2:	f01d                                    	retw.n

000016b4 <compander_AU_proc>:
    16b4:	00c136                                    	entry	a1, 96
    16b7:	05ad                                    	mov.n	a10, a5
    16b9:	06dd                                    	mov.n	a13, a6
    16bb:	13d252                                    	addmi	a5, a2, 0x1300
    16be:	06d262                                    	addmi	a6, a2, 0x600
    16c1:	7115f2                                    	l16ui	a15, a5, 226
    16c4:	079d                                    	mov.n	a9, a7
    16c6:	8f9682                                    	l16si	a8, a6, 0x11e
    16c9:	e4c552                                    	addi	a5, a5, -28
    16cc:	20c662                                    	addi	a6, a6, 32
    16cf:	0ded                                    	mov.n	a14, a13
    16d1:	04c1f0                                    	extui	a12, a15, 1, 1
    16d4:	04b2f0                                    	extui	a11, a15, 2, 1
    16d7:	048866                                    	bnei	a8, 8, 16df <compander_AU_proc+0x2b>
    16da:	ed8b                                    	addi.n	a14, a13, 8
    16dc:	21e4e0                                    	srai	a14, a14, 4

000016df <compander_AU_proc+0x2b>:
    16df:	7e1582                                    	l16ui	a8, a5, 252
    16e2:	821572                                    	l16ui	a7, a5, 0x104
    16e5:	4025d2                                    	l32i	a13, a5, 0x100
    16e8:	c8fc                                    	bnez.n	a8, 1728 <compander_AU_proc+0x74>
    16ea:	339c                                    	beqz.n	a3, 1701 <compander_AU_proc+0x4d>
    16ec:	111aa6                                    	blti	a10, 1, 1701 <compander_AU_proc+0x4d>
    16ef:	3f92b2                                    	l16si	a11, a2, 126
    16f2:	401b00                                    	ssl	a11
    16f5:	08aa76                                    	loopgtz	a10, 1701 <compander_AU_proc+0x4d>
    16f8:	03c8                                    	l32i.n	a12, a3, 0
    16fa:	a1cc00                                    	sll	a12, a12
    16fd:	03c9                                    	s32i.n	a12, a3, 0
    16ff:	334b                                    	addi.n	a3, a3, 4
    1701:	102416                                    	beqz	a4, 1807 <compander_AU_proc+0x153>
    1704:	230760a41405f96e                  	{ bnei.w15	a9, 2, 1807 <compander_AU_proc+0x153>; nop }
    170c:	230760a40c05bace                  	{ blti.w15	a10, 1, 1807 <compander_AU_proc+0x153>; nop }
    1714:	3f92d2                                    	l16si	a13, a2, 126
    1717:	401d00                                    	ssl	a13
    171a:	08aa76                                    	loopgtz	a10, 1726 <compander_AU_proc+0x72>
    171d:	04e8                                    	l32i.n	a14, a4, 0
    171f:	a1ee00                                    	sll	a14, a14
    1722:	04e9                                    	s32i.n	a14, a4, 0
    1724:	444b                                    	addi.n	a4, a4, 4

00001726 <compander_AU_proc+0x72>:
    1726:	f01d                                    	retw.n

00001728 <compander_AU_proc+0x74>:
    1728:	109282                                    	l16si	a8, a2, 32
    172b:	4189                                    	s32i.n	a8, a1, 16
    172d:	0a8766                                    	bnei	a7, 8, 173b <compander_AU_proc+0x87>
    1730:	3a06e4241009adce                  	{ blti.w15	a13, 2, 1809 <compander_AU_proc+0x155>; srai	a7, a10, 4 }
    1738:	000786                                    	j	175a <compander_AU_proc+0xa6>

0000173b <compander_AU_proc+0x87>:
    173b:	0f9766                                    	bnei	a7, 10, 174e <compander_AU_proc+0x9a>
    173e:	220760a410057dce                  	{ blti.w15	a13, 2, 180f <compander_AU_proc+0x15b>; nop }
    1746:	4d1c                                    	movi.n	a13, 20
    1748:	d27ad0                                    	quos	a7, a10, a13
    174b:	0002c6                                    	j	175a <compander_AU_proc+0xa6>

0000174e <compander_AU_proc+0x9a>:
    174e:	2176a0                                    	srai	a7, a10, 6
    1751:	022da6                                    	blti	a13, 2, 1757 <compander_AU_proc+0xa3>
    1754:	000086                                    	j	175a <compander_AU_proc+0xa6>

00001757 <compander_AU_proc+0xa3>:
    1757:	2175a0                                    	srai	a7, a10, 5
    175a:	2207408604055f0e                  	{ bbsi.w15	a15, 0, 1807 <compander_AU_proc+0x153>; nop }
    1762:	220740a40c0517ce                  	{ blti.w15	a7, 1, 1807 <compander_AU_proc+0x153>; nop }
    176a:	71c9                                    	s32i.n	a12, a1, 28
    176c:	61b9                                    	s32i.n	a11, a1, 24
    176e:	5199                                    	s32i.n	a9, a1, 20
    1770:	41f8                                    	l32i.n	a15, a1, 16
    1772:	21076e45046002ae                  	{ addmi	a10, a2, 0x1400; ae_cvt48a32	aed0, a14 }
    177a:	3d02924000e19b1e                  	{ ae_s64.i	aed0, a1, 48; addmi	a13, a2, 0x700 }
    1782:	a0cdd2                                    	addi	a13, a13, -96
    1785:	b1a9                                    	s32i.n	a10, a1, 44
    1787:	eacaa2                                    	addi	a10, a10, -22
    178a:	91d9                                    	s32i.n	a13, a1, 36
    178c:	81a9                                    	s32i.n	a10, a1, 32
    178e:	11ffe0                                    	slli	a15, a15, 2
    1791:	020c                                    	movi.n	a2, 0
    1793:	a1f9                                    	s32i.n	a15, a1, 40
    1795:	b198                                    	l32i.n	a9, a1, 44
    1797:	8188                                    	l32i.n	a8, a1, 32
    1799:	230700c108e51b1e                  	{ ae_l64.i	aed5, a1, 48; nop }
    17a1:	220720c010c57b9e                  	{ ae_l32m.i	aed2, a9, -20; nop }
    17a9:	0eacc57020e280d0c58081381401635f 	{ ae_subsq56s	aed3, aed5, aed2; ae_l16m.i	aed4, a8, 0; nop; nop }
    17b9:	1e8354133464b13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed2, aed3, aed4 }
    17c1:	91a8                                    	l32i.n	a10, a1, 36
    17c3:	03bd                                    	mov.n	a11, a3
    17c5:	0ead4167e1f644d0c50441380c0111df 	{ l32i	a13, a1, 20; or	a12, a4, a4; nop; ae_roundsq32f48asym	aed1, aed2 }
    17d5:	000003f3010300fff1e303012c60d68f 	{ l32r	a8, 130 (15e4 <multiband_cpd_module>); ae_trunca32q48	a15, aed1; nop }
    17e5:	71e8                                    	l32i.n	a14, a1, 28
    17e7:	4265f2                                    	s32i	a15, a5, 0x108
    17ea:	190c                                    	movi.n	a9, 1
    17ec:	2e66f2                                    	s32i	a15, a6, 184
    17ef:	61f8                                    	l32i.n	a15, a1, 24
    17f1:	0199                                    	s32i.n	a9, a1, 0
    17f3:	0008e0                                    	callx8	a8
    17f6:	a1b8                                    	l32i.n	a11, a1, 40
    17f8:	221b                                    	addi.n	a2, a2, 1
    17fa:	2e26a2                                    	l32i	a10, a6, 184
    17fd:	2f66a2                                    	s32i	a10, a6, 188
    1800:	33ba                                    	add.n	a3, a3, a11
    1802:	44ba                                    	add.n	a4, a4, a11
    1804:	8d9727                                    	bne	a7, a2, 1795 <compander_AU_proc+0xe1>
    1807:	f01d                                    	retw.n

00001809 <compander_AU_proc+0x155>:
    1809:	2173a0                                    	srai	a7, a10, 3
    180c:	ffd286                                    	j	175a <compander_AU_proc+0xa6>

0000180f <compander_AU_proc+0x15b>:
    180f:	ad0c                                    	movi.n	a13, 10
    1811:	ffccc6                                    	j	1748 <compander_AU_proc+0x94>

00001814 <get_CPD_memsize>:
    1814:	004136                                    	entry	a1, 32
    1817:	420b                                    	addi.n	a4, a2, -1
    1819:	628c                                    	beqz.n	a2, 1823 <get_CPD_memsize+0xf>
    181b:	40a132                                    	movi	a3, 0x140
    181e:	832340                                    	moveqz	a2, a3, a4
    1821:	f01d                                    	retw.n

00001823 <get_CPD_memsize+0xf>:
    1823:	fa4421                                    	l32r	a2, 134 (1908 <calc_slope_2+0x44>)
    1826:	f01d                                    	retw.n

00001828 <get_INS_memsize>:
    1828:	004136                                    	entry	a1, 32
    182b:	fa4321                                    	l32r	a2, 138 (8a8 <cpd_block_proc+0x210>)
    182e:	f01d                                    	retw.n

00001830 <calc_slope>:
    1830:	006136                                    	entry	a1, 48
    1833:	21b920                                    	srai	a11, a2, 9
    1836:	bb2a                                    	add.n	a11, a11, a2
    1838:	c0ab30                                    	sub	a10, a11, a3
    183b:	120c                                    	movi.n	a2, 1
    183d:	011ae6                                    	bgei	a10, 1, 1842 <calc_slope+0x12>
    1840:	f01d                                    	retw.n

00001842 <calc_slope+0x12>:
    1842:	c0cb40                                    	sub	a12, a11, a4
    1845:	0ead65651d23acd0c500813f0001243f 	{ ae_movda32x2	aed2, a4, a3; ae_movda32x2	aed3, a10, a12; nop; nop }
    1855:	0e2d217b0d5c0c10832ca1380c01015f 	{ nop; nop; nop; ae_mulf32x2ra.hh.ll	aed1, aed2, aed2, aed3 }
    1865:	0ead017b09ed0ca8c200413e0401148f 	{ l32r	a8, a8 (2f0 <Xdiv>); nop; nop; ae_round32x2f48sasym	aed0, aed1, aed2 }
    1875:	2106914000ead3be                  	{ ae_movad32.h	a11, aed0; or	a10, a1, a1 }
    187d:	0906b0c0009c11be                  	{ s32i	a11, a1, 0; ae_movad32.l	a9, aed0 }
    1885:	b14b                                    	addi.n	a11, a1, 4
    1887:	1199                                    	s32i.n	a9, a1, 4
    1889:	0008e0                                    	callx8	a8
    188c:	0a2d                                    	mov.n	a2, a10
    188e:	f01d                                    	retw.n

00001890 <calc_slope_1>:
    1890:	006136                                    	entry	a1, 48
    1893:	21a940                                    	srai	a10, a4, 9
    1896:	aa4a                                    	add.n	a10, a10, a4
    1898:	c0aa20                                    	sub	a10, a10, a2
    189b:	c0b320                                    	sub	a11, a3, a2
    189e:	031ae6                                    	bgei	a10, 1, 18a5 <calc_slope_1+0x15>
    18a1:	120c                                    	movi.n	a2, 1
    18a3:	f01d                                    	retw.n

000018a5 <calc_slope_1+0x15>:
    18a5:	041be6                                    	bgei	a11, 1, 18ad <calc_slope_1+0x1d>
    18a8:	fa2521                                    	l32r	a2, 13c (2000 <_DYNAMIC+0x4d8>)
    18ab:	f01d                                    	retw.n

000018ad <calc_slope_1+0x1d>:
    18ad:	01a9                                    	s32i.n	a10, a1, 0
    18af:	f9fe81                                    	l32r	a8, a8 (2f0 <Xdiv>)
    18b2:	2192b0                                    	srai	a9, a11, 2
    18b5:	a14b                                    	addi.n	a10, a1, 4
    18b7:	01bd                                    	mov.n	a11, a1
    18b9:	1199                                    	s32i.n	a9, a1, 4
    18bb:	0008e0                                    	callx8	a8
    18be:	0a2d                                    	mov.n	a2, a10
    18c0:	f01d                                    	retw.n

000018c2 <calc_slope_1+0x32>:
	...

000018c4 <calc_slope_2>:
    18c4:	006136                                    	entry	a1, 48
    18c7:	21c920                                    	srai	a12, a2, 9
    18ca:	cc2a                                    	add.n	a12, a12, a2
    18cc:	c0dc40                                    	sub	a13, a12, a4
    18cf:	c0cc30                                    	sub	a12, a12, a3
    18d2:	0ead65651d23cdd0c500813f0001243f 	{ ae_movda32x2	aed2, a4, a3; ae_movda32x2	aed3, a12, a13; nop; nop }
    18e2:	0e2d217b0d5c0c10832ca1380c01015f 	{ nop; nop; nop; ae_mulf32x2ra.hh.ll	aed1, aed2, aed2, aed3 }
    18f2:	0ead217b0d4c0ca8c30041380c01015f 	{ nop; nop; nop; ae_round32x2f48sasym	aed0, aed1, aed2 }
    1902:	230700c000e5d39e                  	{ ae_movad32.h	a9, aed0; nop }
    190a:	01ad                                    	mov.n	a10, a1
    190c:	0a06f0c7141cd78e                  	{ l32r	a8, a8 (2f0 <Xdiv>); ae_movad32.l	a11, aed0 }
    1914:	11b9                                    	s32i.n	a11, a1, 4
    1916:	219290                                    	srai	a9, a9, 2
    1919:	b14b                                    	addi.n	a11, a1, 4
    191b:	0199                                    	s32i.n	a9, a1, 0
    191d:	0008e0                                    	callx8	a8
    1920:	0a2d                                    	mov.n	a2, a10
    1922:	f01d                                    	retw.n

00001924 <INS_SetFrame120_Init>:
    1924:	004136                                    	entry	a1, 32
    1927:	08d242                                    	addmi	a4, a2, 0x800
    192a:	a30c                                    	movi.n	a3, 10
    192c:	535432                                    	s16i	a3, a4, 166
    192f:	f01d                                    	retw.n

00001931 <INS_SetFrame120_Init+0xd>:
    1931:	000000                                        ...

00001934 <INS_Init>:
    1934:	004136                                    	entry	a1, 32
    1937:	02ad                                    	mov.n	a10, a2
    1939:	f9dd81                                    	l32r	a8, b0 (1ab0 <xt_memset>)
    193c:	0403c2                                    	l8ui	a12, a3, 4
    193f:	0303e2                                    	l8ui	a14, a3, 3
    1942:	08d2d2                                    	addmi	a13, a2, 0x800
    1945:	050392                                    	l8ui	a9, a3, 5
    1948:	000342                                    	l8ui	a4, a3, 0
    194b:	0203f2                                    	l8ui	a15, a3, 2
    194e:	0b0c                                    	movi.n	a11, 0
    1950:	505d42                                    	s16i	a4, a13, 160
    1953:	119980                                    	slli	a9, a9, 8
    1956:	11ee80                                    	slli	a14, a14, 8
    1959:	20eef0                                    	or	a14, a14, a15
    195c:	2099c0                                    	or	a9, a9, a12
    195f:	899d                                    	ae_sext16	a9, a9
    1961:	8eed                                    	ae_sext16	a14, a14
    1963:	a4cd42                                    	addi	a4, a13, -92
    1966:	515de2                                    	s16i	a14, a13, 162
    1969:	50a3c2                                    	movi	a12, 0x350
    196c:	525d92                                    	s16i	a9, a13, 164
    196f:	0008e0                                    	callx8	a8
    1972:	0b0392                                    	l8ui	a9, a3, 11
    1975:	0a03d2                                    	l8ui	a13, a3, 10
    1978:	0703b2                                    	l8ui	a11, a3, 7
    197b:	00a4e2                                    	movi	a14, 0x400
    197e:	0603c2                                    	l8ui	a12, a3, 6
    1981:	0903a2                                    	l8ui	a10, a3, 9
    1984:	119980                                    	slli	a9, a9, 8
    1987:	11bb80                                    	slli	a11, a11, 8
    198a:	2099d0                                    	or	a9, a9, a13
    198d:	20bbc0                                    	or	a11, a11, a12
    1990:	119980                                    	slli	a9, a9, 8
    1993:	2099a0                                    	or	a9, a9, a10
    1996:	0803a2                                    	l8ui	a10, a3, 8
    1999:	8bbd                                    	ae_sext16	a11, a11
    199b:	819482                                    	l16si	a8, a4, 0x102
    199e:	119980                                    	slli	a9, a9, 8
    19a1:	4252b2                                    	s16i	a11, a2, 132
    19a4:	2099a0                                    	or	a9, a9, a10
    19a7:	0d2c                                    	movi.n	a13, 32
    19a9:	206292                                    	s32i	a9, a2, 128
    19ac:	60a0a2                                    	movi	a10, 96
    19af:	349866                                    	bnei	a8, 10, 19e7 <INS_Init+0xb3>
    19b2:	f9c1b1                                    	l32r	a11, b8 (ccd <compander_AU_PARA_init+0x105>)
    19b5:	aa0c                                    	movi.n	a10, 10
    19b7:	1152b2                                    	s16i	a11, a2, 34
    19ba:	1052a2                                    	s16i	a10, a2, 32
    19bd:	ea1c                                    	movi.n	a10, 30

000019bf <INS_Init+0x8b>:
    19bf:	f9d681                                    	l32r	a8, 118 (44c <AU_INS_coef_init>)
    19c2:	b2a9                                    	s32i.n	a10, a2, 44
    19c4:	f9cbc1                                    	l32r	a12, f0 (8000000 <_end+0x7ffddc0>)
    19c7:	f2c9                                    	s32i.n	a12, a2, 60
    19c9:	490c                                    	movi.n	a9, 4
    19cb:	e2c9                                    	s32i.n	a12, a2, 56
    19cd:	01d2b2                                    	addmi	a11, a2, 0x100
    19d0:	3f5292                                    	s16i	a9, a2, 126
    19d3:	f9c9a1                                    	l32r	a10, f8 (7fffffff <_end+0x7fffddbf>)
    19d6:	a0cbb2                                    	addi	a11, a11, -96
    19d9:	c2b9                                    	s32i.n	a11, a2, 48
    19db:	1062a2                                    	s32i	a10, a2, 64
    19de:	d2b9                                    	s32i.n	a11, a2, 52
    19e0:	02ad                                    	mov.n	a10, a2
    19e2:	0008e0                                    	callx8	a8
    19e5:	f01d                                    	retw.n

000019e7 <INS_Init+0xb3>:
    19e7:	1152e2                                    	s16i	a14, a2, 34
    19ea:	1052d2                                    	s16i	a13, a2, 32
    19ed:	fff386                                    	j	19bf <INS_Init+0x8b>

000019f0 <INS_Prcs>:
    19f0:	008136                                    	entry	a1, 64
    19f3:	08d2a2                                    	addmi	a10, a2, 0x800
    19f6:	509a82                                    	l16si	a8, a10, 160
    19f9:	539ab2                                    	l16si	a11, a10, 166
    19fc:	0902ca840009580e                  	{ bbci.w15	a8, 0, 1a8b <INS_Prcs+0x9b>; addi	a9, a10, -92 }
    1a04:	1092a2                                    	l16si	a10, a2, 32
    1a07:	079b66                                    	bnei	a11, 10, 1a12 <INS_Prcs+0x22>
    1a0a:	a70c                                    	movi.n	a7, 10
    1a0c:	d27570                                    	quos	a7, a5, a7
    1a0f:	000086                                    	j	1a15 <INS_Prcs+0x25>

00001a12 <INS_Prcs+0x22>:
    1a12:	217550                                    	srai	a7, a5, 5
    1a15:	7217a6                                    	blti	a7, 1, 1a8b <INS_Prcs+0x9b>
    1a18:	4199                                    	s32i.n	a9, a1, 16
    1a1a:	118ae0                                    	slli	a8, a10, 2
    1a1d:	050c                                    	movi.n	a5, 0
    1a1f:	5189                                    	s32i.n	a8, a1, 20
    1a21:	02ad                                    	mov.n	a10, a2
    1a23:	f9bf81                                    	l32r	a8, 120 (10c4 <F_CPD_Mag_In>)
    1a26:	03bd                                    	mov.n	a11, a3
    1a28:	04cd                                    	mov.n	a12, a4
    1a2a:	06dd                                    	mov.n	a13, a6
    1a2c:	1e0c                                    	movi.n	a14, 1
    1a2e:	0008e0                                    	callx8	a8
    1a31:	2022a2                                    	l32i	a10, a2, 128
    1a34:	2322b2                                    	l32i	a11, a2, 140
    1a37:	f9b0d1                                    	l32r	a13, f8 (7fffffff <_end+0x7fffddbf>)
    1a3a:	909aa0                                    	addx2	a9, a10, a10
    1a3d:	022b97                                    	blt	a11, a9, 1a43 <INS_Prcs+0x53>
    1a40:	000546                                    	j	1a59 <INS_Prcs+0x69>

00001a43 <INS_Prcs+0x53>:
    1a43:	082ab7                                    	blt	a10, a11, 1a4f <INS_Prcs+0x5f>
    1a46:	4292d2                                    	l16si	a13, a2, 132
    1a49:	11dd00                                    	slli	a13, a13, 16
    1a4c:	000246                                    	j	1a59 <INS_Prcs+0x69>

00001a4f <INS_Prcs+0x5f>:
    1a4f:	f9b381                                    	l32r	a8, 11c (102c <F_CPD_Ins_Gain>)
    1a52:	02ad                                    	mov.n	a10, a2
    1a54:	0008e0                                    	callx8	a8
    1a57:	0add                                    	mov.n	a13, a10
    1a59:	41f8                                    	l32i.n	a15, a1, 16
    1a5b:	03bd                                    	mov.n	a11, a3
    1a5d:	1022a2                                    	l32i	a10, a2, 64
    1a60:	080c                                    	movi.n	a8, 0
    1a62:	7f9f92                                    	l16si	a9, a15, 254
    1a65:	809ff2                                    	l16si	a15, a15, 0x100
    1a68:	04cd                                    	mov.n	a12, a4
    1a6a:	0189                                    	s32i.n	a8, a1, 0
    1a6c:	06ed                                    	mov.n	a14, a6
    1a6e:	1189                                    	s32i.n	a8, a1, 4
    1a70:	72aad0                                    	salt	a10, a10, a13
    1a73:	f9ad81                                    	l32r	a8, 128 (12ec <F_CPD_Intp_Out>)
    1a76:	93f9a0                                    	movnez	a15, a9, a10
    1a79:	02ad                                    	mov.n	a10, a2
    1a7b:	8ffd                                    	ae_sext16	a15, a15
    1a7d:	0008e0                                    	callx8	a8
    1a80:	51b8                                    	l32i.n	a11, a1, 20
    1a82:	551b                                    	addi.n	a5, a5, 1
    1a84:	33ba                                    	add.n	a3, a3, a11
    1a86:	44ba                                    	add.n	a4, a4, a11
    1a88:	959757                                    	bne	a7, a5, 1a21 <INS_Prcs+0x31>
    1a8b:	f01d                                    	retw.n

00001a8d <INS_Prcs+0x9d>:
    1a8d:	000000                                        ...

00001a90 <__do_global_ctors_aux>:
    1a90:	004136                                    	entry	a1, 32
    1a93:	f9ab81                                    	l32r	a8, 140 (1fc8 <_DYNAMIC+0x4a0>)
    1a96:	7f2882                                    	l32i	a8, a8, 0x1fc
    1a99:	f9aa21                                    	l32r	a2, 144 (21c4 <__CTOR_LIST___103>)
    1a9c:	0c0826                                    	beqi	a8, -1, 1aac <__do_global_ctors_aux+0x1c>
    1a9f:	02a8                                    	l32i.n	a10, a2, 0

00001aa1 <__do_global_ctors_aux+0x11>:
    1aa1:	000ae0                                    	callx8	a10
    1aa4:	fcc222                                    	addi	a2, a2, -4
    1aa7:	02a8                                    	l32i.n	a10, a2, 0
    1aa9:	f40a66                                    	bnei	a10, -1, 1aa1 <__do_global_ctors_aux+0x11>
    1aac:	f01d                                    	retw.n

00001aae <__do_global_ctors_aux+0x1e>:
	...

00001ab0 <xt_memset>:
    1ab0:	004136                                    	entry	a1, 32
    1ab3:	026d                                    	mov.n	a6, a2
    1ab5:	42cc                                    	bnez.n	a2, 1abd <xt_memset+0xd>
    1ab7:	020c                                    	movi.n	a2, 0
    1ab9:	f01d                                    	retw.n

00001abb <xt_memset+0xb>:
	...

00001abd <xt_memset+0xd>:
    1abd:	049476                                    	loopnez	a4, 1ac5 <xt_memset+0x15>
    1ac0:	005632                                    	s16i	a3, a6, 0
    1ac3:	662b                                    	addi.n	a6, a6, 2

00001ac5 <xt_memset+0x15>:
    1ac5:	f01d                                    	retw.n

00001ac7 <xt_memset+0x17>:
	...

00001ac8 <xt_memcpy>:
    1ac8:	004136                                    	entry	a1, 32
    1acb:	52ac                                    	beqz.n	a2, 1af4 <xt_memcpy+0x2c>
    1acd:	33ac                                    	beqz.n	a3, 1af4 <xt_memcpy+0x2c>
    1acf:	026d                                    	mov.n	a6, a2
    1ad1:	0bb327                                    	bgeu	a3, a2, 1ae0 <xt_memcpy+0x18>
    1ad4:	907430                                    	addx2	a7, a4, a3
    1ad7:	05b277                                    	bgeu	a2, a7, 1ae0 <xt_memcpy+0x18>
    1ada:	903420                                    	addx2	a3, a4, a2
    1add:	000786                                    	j	1aff <xt_memcpy+0x37>

00001ae0 <xt_memcpy+0x18>:
    1ae0:	000246                                    	j	1aed <xt_memcpy+0x25>

00001ae3 <xt_memcpy+0x1b>:
    1ae3:	009352                                    	l16si	a5, a3, 0
    1ae6:	332b                                    	addi.n	a3, a3, 2
    1ae8:	005652                                    	s16i	a5, a6, 0
    1aeb:	662b                                    	addi.n	a6, a6, 2
    1aed:	440b                                    	addi.n	a4, a4, -1
    1aef:	f00466                                    	bnei	a4, -1, 1ae3 <xt_memcpy+0x1b>
    1af2:	f01d                                    	retw.n

00001af4 <xt_memcpy+0x2c>:
    1af4:	020c                                    	movi.n	a2, 0
    1af6:	f01d                                    	retw.n

00001af8 <xt_memcpy+0x30>:
    1af8:	278d                                    	ae_l16si.n	a8, a7, 0
    1afa:	063d                                    	mov.n	a3, a6
    1afc:	005682                                    	s16i	a8, a6, 0
    1aff:	fec772                                    	addi	a7, a7, -2
    1b02:	440b                                    	addi.n	a4, a4, -1
    1b04:	fec362                                    	addi	a6, a3, -2
    1b07:	ed0466                                    	bnei	a4, -1, 1af8 <xt_memcpy+0x30>
    1b0a:	fff906                                    	j	1af2 <xt_memcpy+0x2a>

00001b0d <xt_memcpy+0x45>:
    1b0d:	000000                                        ...

00001b10 <_fini>:
    1b10:	008136                                    	entry	a1, 64
    1b13:	f98d81                                    	l32r	a8, 148 (170 <__do_global_dtors_aux>)
    1b16:	f03d                                    	nop.n
    1b18:	0008e0                                    	callx8	a8

00001b1b <_fini+0xb>:
    1b1b:	f01d                                    	retw.n
