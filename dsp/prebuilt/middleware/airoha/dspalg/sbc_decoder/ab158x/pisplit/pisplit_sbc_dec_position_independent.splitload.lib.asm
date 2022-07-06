
Build/lib/pisplit_sbc_dec_position_independent.splitload.lib:     file format elf32-xtensa-le


Disassembly of section .text:

00000000 <_init-0x88>:
       0:	000000d8 00001f84 000022ec 00000000     ........."......
      10:	000022b4 00000000 000022d4 01503205     ."......."...2P.
      20:	000000f8 000031c0 00000990 00000e00     .....1..........
      30:	000016d0 00001c90 000032e0 000032c0     .........2...2..
      40:	00002360 000023b0 00002770 00001c7f     `#...#..p'......
      50:	00002470 00002ae0 0000338c 00003330     p$...*...3..03..
      60:	00000114 0000333f 00000324 0000335d     ....?3..$...]3..
      70:	00000488 00003374 000000f0 00003320     ....t3...... 3..
      80:	000022bc 000000a0                       ."......

00000088 <_init>:
      88:	008136                                    	entry	a1, 64
      8b:	ffdd81                                    	l32r	a8, 0 (d8 <frame_dummy>)
      8e:	f03d                                    	nop.n
      90:	0008e0                                    	callx8	a8
      93:	ffdc81                                    	l32r	a8, 4 (1f84 <__do_global_ctors_aux>)
      96:	f03d                                    	nop.n
      98:	0008e0                                    	callx8	a8
      9b:	f01d                                    	retw.n

0000009d <_init+0x15>:
      9d:	000000                                        ...

000000a0 <__do_global_dtors_aux>:
      a0:	004136                                    	entry	a1, 32
      a3:	ffd921                                    	l32r	a2, 8 (22ec <__do_global_dtors_aux.completed>)
      a6:	000232                                    	l8ui	a3, a2, 0
      a9:	63ec                                    	bnez.n	a3, d3 <__do_global_dtors_aux+0x33>

000000ab <__do_global_dtors_aux+0xb>:
      ab:	1248                                    	l32i.n	a4, a2, 4
      ad:	0438                                    	l32i.n	a3, a4, 0
      af:	04028424000163ae                  	{ beqz.w15	a3, c0 <__do_global_dtors_aux+0x20>; addi	a4, a4, 4 }
      b7:	1249                                    	s32i.n	a4, a2, 4
      b9:	0003e0                                    	callx8	a3
      bc:	fffac6                                    	j	ab <__do_global_dtors_aux+0xb>

000000bf <__do_global_dtors_aux+0x1f>:
	...

000000c0 <__do_global_dtors_aux+0x20>:
      c0:	ffd331                                    	l32r	a3, c (0 <_text_start>)
      c3:	ffd3a1                                    	l32r	a10, 10 (22b4 <__FRAME_END__>)
      c6:	438c                                    	beqz.n	a3, ce <__do_global_dtors_aux+0x2e>
      c8:	ffd181                                    	l32r	a8, c (0 <_text_start>)
      cb:	0008e0                                    	callx8	a8
      ce:	130c                                    	movi.n	a3, 1
      d0:	004232                                    	s8i	a3, a2, 0
      d3:	f01d                                    	retw.n

000000d5 <__do_global_dtors_aux+0x35>:
      d5:	000000                                        ...

000000d8 <frame_dummy>:
      d8:	004136                                    	entry	a1, 32
      db:	ffce21                                    	l32r	a2, 14 (0 <_text_start>)
      de:	ffcca1                                    	l32r	a10, 10 (22b4 <__FRAME_END__>)
      e1:	ffcdb1                                    	l32r	a11, 18 (22d4 <frame_dummy.object>)
      e4:	428c                                    	beqz.n	a2, ec <frame_dummy+0x14>
      e6:	ffcb81                                    	l32r	a8, 14 (0 <_text_start>)
      e9:	0008e0                                    	callx8	a8

000000ec <frame_dummy+0x14>:
      ec:	f01d                                    	retw.n

000000ee <frame_dummy+0x16>:
	...

000000f0 <get_SBC_version>:
      f0:	004136                                    	entry	a1, 32
      f3:	ffca21                                    	l32r	a2, 1c (1503205 <_end+0x14ffe75>)
      f6:	f01d                                    	retw.n

000000f8 <memset>:
      f8:	004136                                    	entry	a1, 32
      fb:	205220                                    	or	a5, a2, a2
      fe:	e28c                                    	beqz.n	a2, 110 <memset+0x18>
     100:	202550                                    	or	a2, a5, a5
     103:	948c                                    	beqz.n	a4, 110 <memset+0x18>
     105:	078476                                    	loop	a4, 110 <memset+0x18>
     108:	2202a2400080023e                  	{ s16i	a3, a2, 0; addi	a2, a2, 2 }

00000110 <memset+0x18>:
     110:	052d                                    	mov.n	a2, a5
     112:	f01d                                    	retw.n

00000114 <sbc_init>:
     114:	004136                                    	entry	a1, 32
     117:	ffc231                                    	l32r	a3, 20 (f8 <memset>)
     11a:	03a2c2                                    	movi	a12, 0x203
     11d:	11cce0                                    	slli	a12, a12, 2
     120:	0b0c                                    	movi.n	a11, 0
     122:	02ad                                    	mov.n	a10, a2
     124:	040c                                    	movi.n	a4, 0
     126:	0003e0                                    	callx8	a3
     129:	10d2f2                                    	addmi	a15, a2, 0x1000
     12c:	04d232                                    	addmi	a3, a2, 0x400
     12f:	00a252                                    	movi	a5, 0x200
     132:	085f42                                    	s16i	a4, a15, 16
     135:	4c5352                                    	s16i	a5, a3, 152
     138:	f01d                                    	retw.n

0000013a <sbc_init+0x26>:
	...

0000013c <sbc_crc8>:
     13c:	004136                                    	entry	a1, 32
     13f:	426d                                    	ae_l16ui.n	a6, a2, 0
     141:	024d                                    	mov.n	a4, a2
     143:	037d                                    	mov.n	a7, a3
     145:	f20c                                    	movi.n	a2, 15
     147:	218370                                    	srai	a8, a7, 3
     14a:	3b83a6                                    	blti	a3, 8, 189 <sbc_crc8+0x4d>
     14d:	542b                                    	addi.n	a5, a4, 2
     14f:	ffb531                                    	l32r	a3, 24 (31c0 <SBC_crc_table>)
     152:	f20c                                    	movi.n	a2, 15
     154:	064d                                    	mov.n	a4, a6
     156:	0020f0                                    	nop
     159:	f03d                                    	nop.n
     15b:	f03d                                    	nop.n
     15d:	268876                                    	loop	a8, 187 <sbc_crc8+0x4b>
     160:	0503154014e0642e                  	{ xor	a2, a4, a2; l16ui	a4, a5, 0 }
     168:	2206f1c11074002e                  	{ extui	a6, a2, 0, 1; srli	a2, a2, 1 }
     170:	2306a24410b516de                  	{ slli	a2, a6, 3; addx2	a6, a2, a3 }
     178:	330702c00084162e                  	{ l16ui	a2, a6, 0; ssr	a2 }
     180:	552b                                    	addi.n	a5, a5, 2
     182:	912020                                    	srl	a2, a2
     185:	b22d                                    	ae_zext8	a2, a2

00000187 <sbc_crc8+0x4b>:
     187:	046d                                    	mov.n	a6, a4

00000189 <sbc_crc8+0x4d>:
     189:	730c                                    	movi.n	a3, 7
     18b:	245070                                    	extui	a5, a7, 0, 3
     18e:	064d                                    	mov.n	a4, a6
     190:	220377                                    	bnone	a3, a7, 1b6 <sbc_crc8+0x7a>
     193:	d31c                                    	movi.n	a3, 29
     195:	1d8576                                    	loop	a5, 1b6 <sbc_crc8+0x7a>
     198:	305240                                    	xor	a5, a2, a4
     19b:	015580                                    	slli	a5, a5, 24
     19e:	130220c410b12f5e                  	{ srai	a2, a5, 31; extui	a5, a2, 0, 7 }
     1a6:	1306a2c410a515fe                  	{ slli	a2, a5, 1; and	a5, a2, a3 }
     1ae:	0f06844014e5752e                  	{ xor	a2, a5, a2; slli	a4, a4, 1 }
     1b6:	f01d                                    	retw.n

000001b8 <sbc_extract_scale_factors_joint_ch4>:
     1b8:	004136                                    	entry	a1, 32
     1bb:	1a12f2                                    	l16ui	a15, a2, 52
     1be:	20b220                                    	or	a11, a2, a2
     1c1:	70c2c2                                    	addi	a12, a2, 112
     1c4:	08a032                                    	movi	a3, 8
     1c7:	ff2c72                                    	l32i	a7, a12, 0x3fc
     1ca:	38cb62                                    	addi	a6, a11, 56
     1cd:	1f0691c010f7df3e                  	{ ae_lb	a2, a3; srli	a13, a15, 1 }
     1d5:	18cb52                                    	addi	a5, a11, 24
     1d8:	05ad16                                    	beqz	a13, 236 <sbc_extract_scale_factors_joint_ch4+0x7e>
     1db:	ffcd42                                    	addi	a4, a13, -1
     1de:	230244400cba673e                  	{ ae_db	a7, a3; extui	a10, a2, 4, 4 }
     1e6:	349020                                    	extui	a9, a2, 0, 4
     1e9:	66ad                                    	ae_s16i.n	a10, a6, 0
     1eb:	fec522                                    	addi	a2, a5, -2
     1ee:	769d                                    	ae_s16i.n	a9, a6, 2
     1f0:	04c662                                    	addi	a6, a6, 4
     1f3:	228d                                    	ae_l16si.n	a8, a2, 0
     1f5:	339476                                    	loopnez	a4, 22c <sbc_extract_scale_factors_joint_ch4+0x74>
     1f8:	1d06894100e6cf3e                  	{ ae_lb	a4, a3; slli	a9, a9, 4 }
     200:	0a06b840008a059e                  	{ s16i	a9, a5, 0; or	a8, a8, a10 }
     208:	0202a5400080028e                  	{ s16i	a8, a2, 0; addi	a2, a5, 0 }
     210:	150240400cba673e                  	{ ae_db	a7, a3; extui	a9, a4, 0, 4 }
     218:	24022440089a069e                  	{ s16i	a9, a6, 2; extui	a10, a4, 4, 4 }
     220:	66ad                                    	ae_s16i.n	a10, a6, 0
     222:	240285400080128e                  	{ l16si	a8, a2, 0; addi	a5, a5, 2 }
     22a:	664b                                    	addi.n	a6, a6, 4

0000022c <sbc_extract_scale_factors_joint_ch4+0x74>:
     22c:	1139c0                                    	slli	a3, a9, 4
     22f:	2048a0                                    	or	a4, a8, a10
     232:	653d                                    	ae_s16i.n	a3, a5, 0
     234:	624d                                    	ae_s16i.n	a4, a2, 0

00000236 <sbc_extract_scale_factors_joint_ch4+0x7e>:
     236:	421c                                    	movi.n	a2, 20
     238:	ff6c72                                    	s32i	a7, a12, 0x3fc
     23b:	b02d20                                    	addx8	a2, a13, a2
     23e:	155b22                                    	s16i	a2, a11, 42
     241:	f01d                                    	retw.n

00000243 <sbc_extract_scale_factors_joint_ch4+0x8b>:
	...

00000244 <sbc_extract_scale_factors_joint_ch8_non>:
     244:	004136                                    	entry	a1, 32
     247:	1a12f2                                    	l16ui	a15, a2, 52
     24a:	02ad                                    	mov.n	a10, a2
     24c:	70c2c2                                    	addi	a12, a2, 112
     24f:	03bd                                    	mov.n	a11, a3
     251:	ff2c42                                    	l32i	a4, a12, 0x3fc
     254:	903ba0                                    	addx2	a3, a11, a10
     257:	41d1f0                                    	srli	a13, a15, 1
     25a:	38ca52                                    	addi	a5, a10, 56
     25d:	820c                                    	movi.n	a2, 8
     25f:	09dd16                                    	beqz	a13, 300 <sbc_extract_scale_factors_joint_ch8_non+0xbc>
     262:	4161d0                                    	srli	a6, a13, 1
     265:	12c332                                    	addi	a3, a3, 18
     268:	120701840002cd0e                  	{ bbci.w15	a13, 0, 285 <sbc_extract_scale_factors_joint_ch8_non+0x41>; ae_lb	a9, a2 }
     270:	390244400cb9742e                  	{ ae_db	a4, a2; extui	a7, a9, 4, 4 }
     278:	34e090                                    	extui	a14, a9, 0, 4
     27b:	657d                                    	ae_s16i.n	a7, a5, 0
     27d:	75ed                                    	ae_s16i.n	a14, a5, 2
     27f:	554b                                    	addi.n	a5, a5, 4
     281:	639d                                    	ae_s16i.n	a9, a3, 0
     283:	332b                                    	addi.n	a3, a3, 2

00000285 <sbc_extract_scale_factors_joint_ch8_non+0x41>:
     285:	330721a40401a6ae                  	{ beqz.w15	a6, 300 <sbc_extract_scale_factors_joint_ch8_non+0xbc>; ae_lb	a7, a2 }
     28d:	230740c00ca5742e                  	{ ae_db	a4, a2; nop }
     295:	130761c20062247e                  	{ extui	a8, a7, 4, 4; ae_lb	a9, a2 }
     29d:	34f070                                    	extui	a15, a7, 0, 4
     2a0:	658d                                    	ae_s16i.n	a8, a5, 0
     2a2:	75fd                                    	ae_s16i.n	a15, a5, 2
     2a4:	f60b                                    	addi.n	a15, a6, -1
     2a6:	637d                                    	ae_s16i.n	a7, a3, 0
     2a8:	554b                                    	addi.n	a5, a5, 4
     2aa:	290244400cb9742e                  	{ ae_db	a4, a2; extui	a6, a9, 4, 4 }
     2b2:	347090                                    	extui	a7, a9, 0, 4
     2b5:	419f76                                    	loopnez	a15, 2fa <sbc_extract_scale_factors_joint_ch8_non+0xb6>
     2b8:	320721c00881057e                  	{ s16i	a7, a5, 2; ae_lb	a7, a2 }
     2c0:	656d                                    	ae_s16i.n	a6, a5, 0
     2c2:	0802a5400881039e                  	{ s16i	a9, a3, 2; addi	a8, a5, 4 }
     2ca:	170244400cb9742e                  	{ ae_db	a4, a2; extui	a5, a7, 4, 4 }
     2d2:	120721c00082085e                  	{ s16i	a5, a8, 0; ae_lb	a9, a2 }
     2da:	0502e8411061307e                  	{ extui	a6, a7, 0, 4; addi	a5, a8, 4 }
     2e2:	0202a3400881186e                  	{ s16i	a6, a8, 2; addi	a3, a3, 4 }
     2ea:	380220400099137e                  	{ s16i	a7, a3, 0; extui	a7, a9, 0, 4 }
     2f2:	290244400cb9742e                  	{ ae_db	a4, a2; extui	a6, a9, 4, 4 }

000002fa <sbc_extract_scale_factors_joint_ch8_non+0xb6>:
     2fa:	757d                                    	ae_s16i.n	a7, a5, 2
     2fc:	656d                                    	ae_s16i.n	a6, a5, 0
     2fe:	739d                                    	ae_s16i.n	a9, a3, 2

00000300 <sbc_extract_scale_factors_joint_ch8_non+0xbc>:
     300:	2dba                                    	add.n	a2, a13, a11
     302:	ff6c42                                    	s32i	a4, a12, 0x3fc
     305:	1122d0                                    	slli	a2, a2, 3
     308:	155a22                                    	s16i	a2, a10, 42
     30b:	f01d                                    	retw.n

0000030d <sbc_extract_scale_factors_joint_ch8_non+0xc9>:
     30d:	000000                                        ...

00000310 <sbc_get_frame_length>:
     310:	004136                                    	entry	a1, 32
     313:	04d222                                    	addmi	a2, a2, 0x400
     316:	4c1222                                    	l16ui	a2, a2, 152
     319:	f01d                                    	retw.n

0000031b <sbc_get_frame_length+0xb>:
	...

0000031c <sbc_get_sample_freq>:
     31c:	004136                                    	entry	a1, 32
     31f:	021222                                    	l16ui	a2, a2, 4
     322:	f01d                                    	retw.n

00000324 <sbc_frame_header_decode>:
     324:	004136                                    	entry	a1, 32
     327:	0cd352                                    	addmi	a5, a3, 0xc00
     32a:	040c                                    	movi.n	a4, 0
     32c:	3578                                    	l32i.n	a7, a5, 12
     32e:	624d                                    	ae_s16i.n	a4, a2, 0
     330:	9ca062                                    	movi	a6, 156
     333:	271b                                    	addi.n	a2, a7, 1
     335:	0d8076                                    	loop	a0, 346 <sbc_frame_header_decode+0x22>
     338:	024d                                    	mov.n	a4, a2
     33a:	f20b                                    	addi.n	a15, a2, -1
     33c:	3529                                    	s32i.n	a2, a5, 12
     33e:	221b                                    	addi.n	a2, a2, 1
     340:	000f72                                    	l8ui	a7, a15, 0
     343:	021767                                    	beq	a7, a6, 349 <sbc_frame_header_decode+0x25>

00000346 <sbc_frame_header_decode+0x22>:
     346:	fffac6                                    	j	335 <sbc_frame_header_decode+0x11>

00000349 <sbc_frame_header_decode+0x25>:
     349:	620b                                    	addi.n	a6, a2, -1
     34b:	3529                                    	s32i.n	a2, a5, 12
     34d:	000662                                    	l8ui	a6, a6, 0
     350:	a21b                                    	addi.n	a10, a2, 1
     352:	c80c                                    	movi.n	a8, 12
     354:	290c                                    	movi.n	a9, 2
     356:	35a9                                    	s32i.n	a10, a5, 12
     358:	170c                                    	movi.n	a7, 1
     35a:	41c260                                    	srli	a12, a6, 2
     35d:	04d160                                    	extui	a13, a6, 1, 1
     360:	04e060                                    	extui	a14, a6, 0, 1
     363:	108c80                                    	and	a8, a12, a8
     366:	0553d2                                    	s16i	a13, a3, 10
     369:	14a0c0                                    	extui	a10, a12, 0, 2
     36c:	11dee0                                    	slli	a13, a14, 2
     36f:	9379a0                                    	movnez	a7, a9, a10
     372:	884b                                    	addi.n	a8, a8, 4
     374:	9d4b                                    	addi.n	a9, a13, 4
     376:	41f660                                    	srli	a15, a6, 6
     379:	095362                                    	s16i	a6, a3, 18
     37c:	0453a2                                    	s16i	a10, a3, 8
     37f:	016640                                    	slli	a6, a6, 28
     382:	0253f2                                    	s16i	a15, a3, 4
     385:	fecaa2                                    	addi	a10, a10, -2
     388:	035382                                    	s16i	a8, a3, 6
     38b:	316e60                                    	srai	a6, a6, 30
     38e:	222b                                    	addi.n	a2, a2, 2
     390:	075372                                    	s16i	a7, a3, 14
     393:	065392                                    	s16i	a9, a3, 12
     396:	04d3b2                                    	addmi	a11, a3, 0x400
     399:	0104f2                                    	l8ui	a15, a4, 1
     39c:	3529                                    	s32i.n	a2, a5, 12
     39e:	d1c790                                    	mul16s	a12, a7, a9
     3a1:	d17780                                    	mul16s	a7, a7, a8
     3a4:	d1e980                                    	mul16s	a14, a9, a8
     3a7:	d1dc80                                    	mul16s	a13, a12, a8
     3aa:	d18f80                                    	mul16s	a8, a15, a8
     3ad:	d177f0                                    	mul16s	a7, a7, a15
     3b0:	998a                                    	add.n	a9, a9, a8
     3b2:	887b                                    	addi.n	a8, a8, 7
     3b4:	997b                                    	addi.n	a9, a9, 7
     3b6:	777b                                    	addi.n	a7, a7, 7
     3b8:	1853e2                                    	s16i	a14, a3, 48
     3bb:	8398a0                                    	moveqz	a9, a8, a10
     3be:	41e1c0                                    	srli	a14, a12, 1
     3c1:	b39760                                    	movgez	a9, a7, a6
     3c4:	2e4b                                    	addi.n	a2, a14, 4
     3c6:	416390                                    	srli	a6, a9, 3
     3c9:	262a                                    	add.n	a2, a6, a2
     3cb:	1a53c2                                    	s16i	a12, a3, 52
     3ce:	4c5b22                                    	s16i	a2, a11, 152
     3d1:	020c                                    	movi.n	a2, 0
     3d3:	0853f2                                    	s16i	a15, a3, 16
     3d6:	0a53f2                                    	s16i	a15, a3, 20
     3d9:	1b53d2                                    	s16i	a13, a3, 54
     3dc:	0204f2                                    	l8ui	a15, a4, 2
     3df:	1653f2                                    	s16i	a15, a3, 44
     3e2:	f01d                                    	retw.n

000003e4 <change_endian16x>:
     3e4:	004136                                    	entry	a1, 32
     3e7:	ffa032                                    	movi	a3, 255
     3ea:	114280                                    	slli	a4, a2, 8
     3ed:	113300                                    	slli	a3, a3, 16
     3f0:	01f280                                    	slli	a15, a2, 24
     3f3:	103430                                    	and	a3, a4, a3
     3f6:	2f3a                                    	add.n	a2, a15, a3
     3f8:	312020                                    	srai	a2, a2, 16
     3fb:	f01d                                    	retw.n

000003fd <change_endian16x+0x19>:
     3fd:	000000                                        ...

00000400 <SBC_decode_preload>:
     400:	004136                                    	entry	a1, 32
     403:	0cd2f2                                    	addmi	a15, a2, 0xc00
     406:	026d                                    	mov.n	a6, a2
     408:	032f72                                    	l32i	a7, a15, 12
     40b:	04a032                                    	movi	a3, 4
     40e:	042070                                    	extui	a2, a7, 0, 1
     411:	c09320                                    	sub	a9, a3, a2
     414:	1182d0                                    	slli	a8, a2, 3
     417:	fcc222                                    	addi	a2, a2, -4
     41a:	057916                                    	beqz	a9, 475 <SBC_decode_preload+0x75>
     41d:	604020                                    	neg	a4, a2
     420:	203770                                    	or	a3, a7, a7
     423:	00a022                                    	movi	a2, 0
     426:	415140                                    	srli	a5, a4, 1
     429:	11b280                                    	slli	a11, a2, 8
     42c:	086407                                    	bbci	a4, 0, 438 <SBC_decode_preload+0x38>
     42f:	0003a2                                    	l8ui	a10, a3, 0
     432:	01c332                                    	addi	a3, a3, 1
     435:	802ba0                                    	add	a2, a11, a10

00000438 <SBC_decode_preload+0x38>:
     438:	11c280                                    	slli	a12, a2, 8
     43b:	15bc                                    	beqz.n	a5, 470 <SBC_decode_preload+0x70>
     43d:	000342                                    	l8ui	a4, a3, 0
     440:	b50b                                    	addi.n	a11, a5, -1
     442:	0103a2                                    	l8ui	a10, a3, 1
     445:	232b                                    	addi.n	a2, a3, 2
     447:	4c4a                                    	add.n	a4, a12, a4
     449:	043d                                    	mov.n	a3, a4
     44b:	0a5d                                    	mov.n	a5, a10
     44d:	199b76                                    	loopnez	a11, 46a <SBC_decode_preload+0x6a>
     450:	090683400485023e                  	{ l8ui	a3, a2, 0; slli	a4, a3, 8 }
     458:	445a                                    	add.n	a4, a4, a5
     45a:	1503b24500a0148e                  	{ slli	a4, a4, 8; l8ui	a5, a2, 1 }
     462:	220282401ce0243e                  	{ add	a3, a4, a3; addi	a2, a2, 2 }

0000046a <SBC_decode_preload+0x6a>:
     46a:	114380                                    	slli	a4, a3, 8
     46d:	802450                                    	add	a2, a4, a5

00000470 <SBC_decode_preload+0x70>:
     470:	779a                                    	add.n	a7, a7, a9
     472:	000046                                    	j	477 <SBC_decode_preload+0x77>

00000475 <SBC_decode_preload+0x75>:
     475:	020c                                    	movi.n	a2, 0
     477:	fec7f2                                    	addi	a15, a7, -2
     47a:	70c632                                    	addi	a3, a6, 112
     47d:	67ef84                                    	wur.ae_bitptr	a8
     480:	f3f120                                    	wur.ae_bithead	a2
     483:	ff63f2                                    	s32i	a15, a3, 0x3fc
     486:	f01d                                    	retw.n

00000488 <sbc_frame_decode>:
     488:	006136                                    	entry	a1, 48
     48b:	0cd2e2                                    	addmi	a14, a2, 0xc00
     48e:	02fd                                    	mov.n	a15, a2
     490:	032e62                                    	l32i	a6, a14, 12
     493:	04a0d2                                    	movi	a13, 4
     496:	026142                                    	s32i	a4, a1, 8
     499:	036132                                    	s32i	a3, a1, 12
     49c:	042060                                    	extui	a2, a6, 0, 1
     49f:	203660                                    	or	a3, a6, a6
     4a2:	c08d20                                    	sub	a8, a13, a2
     4a5:	1172d0                                    	slli	a7, a2, 3
     4a8:	fcc222                                    	addi	a2, a2, -4
     4ab:	05a816                                    	beqz	a8, 509 <sbc_frame_decode+0x81>
     4ae:	604020                                    	neg	a4, a2
     4b1:	00a022                                    	movi	a2, 0
     4b4:	415140                                    	srli	a5, a4, 1
     4b7:	096407                                    	bbci	a4, 0, 4c4 <sbc_frame_decode+0x3c>
     4ba:	000392                                    	l8ui	a9, a3, 0
     4bd:	114280                                    	slli	a4, a2, 8
     4c0:	331b                                    	addi.n	a3, a3, 1
     4c2:	249a                                    	add.n	a2, a4, a9

000004c4 <sbc_frame_decode+0x3c>:
     4c4:	11b280                                    	slli	a11, a2, 8
     4c7:	65bc                                    	beqz.n	a5, 501 <sbc_frame_decode+0x79>
     4c9:	000342                                    	l8ui	a4, a3, 0
     4cc:	a50b                                    	addi.n	a10, a5, -1
     4ce:	010392                                    	l8ui	a9, a3, 1
     4d1:	02c322                                    	addi	a2, a3, 2
     4d4:	804b40                                    	add	a4, a11, a4
     4d7:	205990                                    	or	a5, a9, a9
     4da:	203440                                    	or	a3, a4, a4
     4dd:	199a76                                    	loopnez	a10, 4fa <sbc_frame_decode+0x72>
     4e0:	090683400485023e                  	{ l8ui	a3, a2, 0; slli	a4, a3, 8 }
     4e8:	445a                                    	add.n	a4, a4, a5
     4ea:	1503b24500a0148e                  	{ slli	a4, a4, 8; l8ui	a5, a2, 1 }
     4f2:	220282401ce0243e                  	{ add	a3, a4, a3; addi	a2, a2, 2 }

000004fa <sbc_frame_decode+0x72>:
     4fa:	114380                                    	slli	a4, a3, 8
     4fd:	059d                                    	mov.n	a9, a5
     4ff:	245a                                    	add.n	a2, a4, a5

00000501 <sbc_frame_decode+0x79>:
     501:	668a                                    	add.n	a6, a6, a8
     503:	000106                                    	j	50b <sbc_frame_decode+0x83>

00000506 <sbc_frame_decode+0x7e>:
     506:	000000                                        ...

00000509 <sbc_frame_decode+0x81>:
     509:	020c                                    	movi.n	a2, 0
     50b:	041f32                                    	l16ui	a3, a15, 8
     50e:	fec652                                    	addi	a5, a6, -2
     511:	70cfa2                                    	addi	a10, a15, 112
     514:	040c                                    	movi.n	a4, 0
     516:	67ef74                                    	wur.ae_bitptr	a7
     519:	f3f120                                    	wur.ae_bithead	a2
     51c:	061f22                                    	l16ui	a2, a15, 12
     51f:	ff6a52                                    	s32i	a5, a10, 0x3fc
     522:	061f82                                    	l16ui	a8, a15, 12
     525:	6d3366                                    	bnei	a3, 3, 596 <sbc_frame_decode+0x10e>
     528:	220701c71c80ea5e                  	{ l32i	a5, a10, 0x3fc; ae_lb	a2, a2 }
     530:	061fd2                                    	l16ui	a13, a15, 12
     533:	175f22                                    	s16i	a2, a15, 46
     536:	171f42                                    	l16ui	a4, a15, 46
     539:	fcc832                                    	addi	a3, a8, -4
     53c:	27035f400ca165de                  	{ ae_db	a5, a13; l16ui	a6, a15, 12 }
     544:	58a472                                    	movi	a7, 0x458
     547:	ff6a52                                    	s32i	a5, a10, 0x3fc
     54a:	34e040                                    	extui	a14, a4, 0, 4
     54d:	fb7c                                    	movi.n	a11, -1
     54f:	112ec0                                    	slli	a2, a14, 4
     552:	960b                                    	addi.n	a9, a6, -1
     554:	932430                                    	movnez	a2, a4, a3
     557:	3f7a                                    	add.n	a3, a15, a7
     559:	0b5f22                                    	s16i	a2, a15, 22
     55c:	6ccf22                                    	addi	a2, a15, 108
     55f:	180c                                    	movi.n	a8, 1
     561:	44bc                                    	beqz.n	a4, 599 <sbc_frame_decode+0x111>
     563:	ff6232                                    	s32i	a3, a2, 0x3fc
     566:	f470b0                                    	extui	a7, a11, 0, 16
     569:	c6ac                                    	beqz.n	a6, 599 <sbc_frame_decode+0x111>
     56b:	401900                                    	ssl	a9
     56e:	a19800                                    	sll	a9, a8
     571:	1c8076                                    	loop	a0, 591 <sbc_frame_decode+0x109>
     574:	104490                                    	and	a4, a4, a9
     577:	b80b                                    	addi.n	a11, a8, -1
     579:	090747                                    	bnone	a7, a4, 586 <sbc_frame_decode+0xfe>
     57c:	63bd                                    	ae_s16i.n	a11, a3, 0
     57e:	332b                                    	addi.n	a3, a3, 2
     580:	061f62                                    	l16ui	a6, a15, 12
     583:	ff6232                                    	s32i	a3, a2, 0x3fc
     586:	419190                                    	srli	a9, a9, 1
     589:	0cb867                                    	bgeu	a8, a6, 599 <sbc_frame_decode+0x111>
     58c:	171f42                                    	l16ui	a4, a15, 46
     58f:	881b                                    	addi.n	a8, a8, 1

00000591 <sbc_frame_decode+0x109>:
     591:	fff706                                    	j	571 <sbc_frame_decode+0xe9>

00000594 <sbc_frame_decode+0x10c>:
	...

00000596 <sbc_frame_decode+0x10e>:
     596:	175f42                                    	s16i	a4, a15, 46
     599:	041f22                                    	l16ui	a2, a15, 8
     59c:	1a1f32                                    	l16ui	a3, a15, 52
     59f:	1a1f42                                    	l16ui	a4, a15, 52
     5a2:	38cf62                                    	addi	a6, a15, 56
     5a5:	061f72                                    	l16ui	a7, a15, 12
     5a8:	0c1c                                    	movi.n	a12, 16
     5aa:	230720a01c05f26e                  	{ bnei.w15	a2, 3, 62c <sbc_frame_decode+0x1a4>; nop }
     5b2:	41b140                                    	srli	a11, a4, 1
     5b5:	8c1c                                    	movi.n	a12, 24
     5b7:	18cf32                                    	addi	a3, a15, 24
     5ba:	230700a50045f76e                  	{ bnei.w15	a7, 4, 7dd <sbc_frame_decode+0x355>; nop }
     5c2:	4c1c                                    	movi.n	a12, 20
     5c4:	820c                                    	movi.n	a2, 8
     5c6:	38cf42                                    	addi	a4, a15, 56
     5c9:	107b16                                    	beqz	a11, 6d4 <sbc_frame_decode+0x24c>
     5cc:	330741c00ca0652e                  	{ ae_db	a5, a2; ae_lb	a3, a2 }
     5d4:	9b0b                                    	addi.n	a9, a11, -1
     5d6:	348430                                    	extui	a8, a3, 4, 4
     5d9:	346030                                    	extui	a6, a3, 0, 4
     5dc:	16cf32                                    	addi	a3, a15, 22
     5df:	648d                                    	ae_s16i.n	a8, a4, 0
     5e1:	746d                                    	ae_s16i.n	a6, a4, 2
     5e3:	237d                                    	ae_l16si.n	a7, a3, 0
     5e5:	319976                                    	loopnez	a9, 61a <sbc_frame_decode+0x192>
     5e8:	2d06864208e5cf2e                  	{ ae_lb	a9, a2; slli	a6, a6, 4 }
     5f0:	2806b7400889036e                  	{ s16i	a6, a3, 2; or	a6, a7, a8 }
     5f8:	0402a4400081036e                  	{ s16i	a6, a3, 0; addi	a4, a4, 4 }
     600:	090244400cba752e                  	{ ae_db	a5, a2; extui	a8, a9, 4, 4 }
     608:	280220400099148e                  	{ s16i	a8, a4, 0; extui	a6, a9, 0, 4 }
     610:	746d                                    	ae_s16i.n	a6, a4, 2
     612:	220283400880137e                  	{ l16si	a7, a3, 2; addi	a3, a3, 2 }

0000061a <sbc_frame_decode+0x192>:
     61a:	1126c0                                    	slli	a2, a6, 4
     61d:	20d780                                    	or	a13, a7, a8
     620:	732d                                    	ae_s16i.n	a2, a3, 2
     622:	e32b                                    	addi.n	a14, a3, 2
     624:	63dd                                    	ae_s16i.n	a13, a3, 0
     626:	002a86                                    	j	6d4 <sbc_frame_decode+0x24c>

00000629 <sbc_frame_decode+0x1a1>:
     629:	000000                                        ...

0000062c <sbc_frame_decode+0x1a4>:
     62c:	41b130                                    	srli	a11, a3, 1
     62f:	16cf32                                    	addi	a3, a15, 22
     632:	820c                                    	movi.n	a2, 8
     634:	09cb16                                    	beqz	a11, 6d4 <sbc_frame_decode+0x24c>
     637:	4141b0                                    	srli	a4, a11, 1
     63a:	176b07                                    	bbci	a11, 0, 655 <sbc_frame_decode+0x1cd>
     63d:	130741c00ca2652e                  	{ ae_db	a5, a2; ae_lb	a9, a2 }
     645:	347490                                    	extui	a7, a9, 4, 4
     648:	34d090                                    	extui	a13, a9, 0, 4
     64b:	667d                                    	ae_s16i.n	a7, a6, 0
     64d:	76dd                                    	ae_s16i.n	a13, a6, 2
     64f:	664b                                    	addi.n	a6, a6, 4
     651:	639d                                    	ae_s16i.n	a9, a3, 0
     653:	332b                                    	addi.n	a3, a3, 2
     655:	330721a40401c4ae                  	{ beqz.w15	a4, 6d4 <sbc_frame_decode+0x24c>; ae_lb	a7, a2 }
     65d:	0d02c6400ca1752e                  	{ ae_db	a5, a2; addi	a13, a6, 4 }
     665:	130761c20062247e                  	{ extui	a8, a7, 4, 4; ae_lb	a9, a2 }
     66d:	34e070                                    	extui	a14, a7, 0, 4
     670:	668d                                    	ae_s16i.n	a8, a6, 0
     672:	76ed                                    	ae_s16i.n	a14, a6, 2
     674:	e40b                                    	addi.n	a14, a4, -1
     676:	637d                                    	ae_s16i.n	a7, a3, 0
     678:	346490                                    	extui	a6, a9, 4, 4
     67b:	390240400cb9752e                  	{ ae_db	a5, a2; extui	a7, a9, 0, 4 }
     683:	0d4d                                    	mov.n	a4, a13
     685:	419e76                                    	loopnez	a14, 6ca <sbc_frame_decode+0x242>
     688:	320721c00881047e                  	{ s16i	a7, a4, 2; ae_lb	a7, a2 }
     690:	646d                                    	ae_s16i.n	a6, a4, 0
     692:	0802a4400881039e                  	{ s16i	a9, a3, 2; addi	a8, a4, 4 }
     69a:	070244400cb9752e                  	{ ae_db	a5, a2; extui	a4, a7, 4, 4 }
     6a2:	120721c00082084e                  	{ s16i	a4, a8, 0; ae_lb	a9, a2 }
     6aa:	0502e8411061207e                  	{ extui	a6, a7, 0, 4; addi	a4, a8, 4 }
     6b2:	0202a3400881186e                  	{ s16i	a6, a8, 2; addi	a3, a3, 4 }
     6ba:	380220400099137e                  	{ s16i	a7, a3, 0; extui	a7, a9, 0, 4 }
     6c2:	290244400cb9752e                  	{ ae_db	a5, a2; extui	a6, a9, 4, 4 }

000006ca <sbc_frame_decode+0x242>:
     6ca:	747d                                    	ae_s16i.n	a7, a4, 2
     6cc:	d44b                                    	addi.n	a13, a4, 4
     6ce:	e34b                                    	addi.n	a14, a3, 4
     6d0:	646d                                    	ae_s16i.n	a6, a4, 0
     6d2:	739d                                    	ae_s16i.n	a9, a3, 2

000006d4 <sbc_frame_decode+0x24c>:
     6d4:	b02bc0                                    	addx8	a2, a11, a12
     6d7:	091f32                                    	l16ui	a3, a15, 18
     6da:	927d                                    	ae_zext16	a7, a2
     6dc:	ff6a52                                    	s32i	a5, a10, 0x3fc
     6df:	f40c                                    	movi.n	a4, 15
     6e1:	155f22                                    	s16i	a2, a15, 42
     6e4:	416370                                    	srli	a6, a7, 3
     6e7:	14cf52                                    	addi	a5, a15, 20
     6ea:	1d87f6                                    	bgeui	a7, 8, 70b <sbc_frame_decode+0x283>
     6ed:	4ce727                                    	bbsi	a7, 2, 73d <sbc_frame_decode+0x2b5>
     6f0:	161f22                                    	l16ui	a2, a15, 44
     6f3:	759427                                    	bne	a4, a2, 76c <sbc_frame_decode+0x2e4>
     6f6:	fe4c21                                    	l32r	a2, 28 (990 <sbc_bit_allocation>)
     6f9:	0fad                                    	mov.n	a10, a15
     6fb:	0f4d                                    	mov.n	a4, a15
     6fd:	0002e0                                    	callx8	a2
     700:	fe4b21                                    	l32r	a2, 2c (e00 <sbc_reconstruction>)
     703:	04ad                                    	mov.n	a10, a4
     705:	0002e0                                    	callx8	a2
     708:	001c06                                    	j	77c <sbc_frame_decode+0x2f4>

0000070b <sbc_frame_decode+0x283>:
     70b:	f40c                                    	movi.n	a4, 15
     70d:	fe4521                                    	l32r	a2, 24 (31c0 <SBC_crc_table>)
     710:	268676                                    	loop	a6, 73a <sbc_frame_decode+0x2b2>
     713:	0303154104e0743e                  	{ xor	a4, a4, a3; l16ui	a3, a5, 0 }
     71b:	0406f1c11075004e                  	{ extui	a6, a4, 0, 1; srli	a4, a4, 1 }
     723:	2306a44500b506de                  	{ slli	a4, a6, 3; addx2	a6, a4, a2 }
     72b:	330704c00084164e                  	{ l16ui	a4, a6, 0; ssr	a4 }
     733:	552b                                    	addi.n	a5, a5, 2
     735:	914040                                    	srl	a4, a4
     738:	b44d                                    	ae_zext8	a4, a4

0000073a <sbc_frame_decode+0x2b2>:
     73a:	b26727                                    	bbci	a7, 2, 6f0 <sbc_frame_decode+0x268>

0000073d <sbc_frame_decode+0x2b5>:
     73d:	420c                                    	movi.n	a2, 4
     73f:	105720                                    	and	a5, a7, a2
     742:	1da022                                    	movi	a2, 29
     745:	1d8576                                    	loop	a5, 766 <sbc_frame_decode+0x2de>
     748:	305340                                    	xor	a5, a3, a4
     74b:	015580                                    	slli	a5, a5, 24
     74e:	150220c500b12f5e                  	{ srai	a4, a5, 31; extui	a5, a4, 0, 7 }
     756:	1306a4c500a505fe                  	{ slli	a4, a5, 1; and	a5, a4, a2 }
     75e:	3f06834104e4754e                  	{ xor	a4, a5, a4; slli	a3, a3, 1 }

00000766 <sbc_frame_decode+0x2de>:
     766:	161f22                                    	l16ui	a2, a15, 44
     769:	891427                                    	beq	a4, a2, 6f6 <sbc_frame_decode+0x26e>

0000076c <sbc_frame_decode+0x2e4>:
     76c:	fe2d21                                    	l32r	a2, 20 (f8 <memset>)
     76f:	58cfa2                                    	addi	a10, a15, 88
     772:	0b0c                                    	movi.n	a11, 0
     774:	00a4c2                                    	movi	a12, 0x400
     777:	0f4d                                    	mov.n	a4, a15
     779:	0002e0                                    	callx8	a2
     77c:	071422                                    	l16ui	a2, a4, 14
     77f:	030c                                    	movi.n	a3, 0
     781:	1b14f2                                    	l16ui	a15, a4, 54
     784:	10d4e2                                    	addmi	a14, a4, 0x1000
     787:	04cd                                    	mov.n	a12, a4
     789:	a0a442                                    	movi	a4, 0x4a0
     78c:	220b                                    	addi.n	a2, a2, -1
     78e:	061c52                                    	l16ui	a5, a12, 12
     791:	622320                                    	saltu	a2, a3, a2
     794:	081e32                                    	l16ui	a3, a14, 16
     797:	6c4a                                    	add.n	a6, a12, a4
     799:	400200                                    	ssr	a2
     79c:	0bdc42                                    	addmi	a4, a12, 0xb00
     79f:	9120f0                                    	srl	a2, a15
     7a2:	20a772                                    	movi	a7, 0x720
     7a5:	0d4316                                    	beqz	a3, 87d <sbc_frame_decode+0x3f5>
     7a8:	061c82                                    	l16ui	a8, a12, 12
     7ab:	220760a41405930e                  	{ beqi.w15	a3, 2, 8a0 <sbc_frame_decode+0x418>; nop }
     7b3:	230740a40c25736e                  	{ bnei.w15	a3, 1, 966 <sbc_frame_decode+0x4de>; nop }
     7bb:	0bdc32                                    	addmi	a3, a12, 0xb00
     7be:	fe1c71                                    	l32r	a7, 30 (16d0 <SBC_synthesis4_per_channel>)
     7c1:	5393f2                                    	l16si	a15, a3, 166
     7c4:	286362                                    	s32i	a6, a3, 160
     7c7:	5253f2                                    	s16i	a15, a3, 164
     7ca:	220760a10405756e                  	{ bnei.w15	a5, 4, 8ba <sbc_frame_decode+0x432>; nop }
     7d2:	075d                                    	mov.n	a5, a7
     7d4:	003946                                    	j	8bd <sbc_frame_decode+0x435>

000007d7 <sbc_frame_decode+0x34f>:
     7d7:	00000000                                 ......

000007dd <sbc_frame_decode+0x355>:
     7dd:	38cf62                                    	addi	a6, a15, 56
     7e0:	ef0b16                                    	beqz	a11, 6d4 <sbc_frame_decode+0x24c>
     7e3:	820c                                    	movi.n	a2, 8
     7e5:	4141b0                                    	srli	a4, a11, 1
     7e8:	120701840002cb0e                  	{ bbci.w15	a11, 0, 805 <sbc_frame_decode+0x37d>; ae_lb	a9, a2 }
     7f0:	390244400cb9752e                  	{ ae_db	a5, a2; extui	a7, a9, 4, 4 }
     7f8:	34d090                                    	extui	a13, a9, 0, 4
     7fb:	667d                                    	ae_s16i.n	a7, a6, 0
     7fd:	76dd                                    	ae_s16i.n	a13, a6, 2
     7ff:	664b                                    	addi.n	a6, a6, 4
     801:	639d                                    	ae_s16i.n	a9, a3, 0
     803:	332b                                    	addi.n	a3, a3, 2
     805:	330761bc03c144ae                  	{ beqz.w15	a4, 6d4 <sbc_frame_decode+0x24c>; ae_lb	a7, a2 }
     80d:	0d02c6400ca1752e                  	{ ae_db	a5, a2; addi	a13, a6, 4 }
     815:	130761c20062247e                  	{ extui	a8, a7, 4, 4; ae_lb	a9, a2 }
     81d:	34e070                                    	extui	a14, a7, 0, 4
     820:	668d                                    	ae_s16i.n	a8, a6, 0
     822:	76ed                                    	ae_s16i.n	a14, a6, 2
     824:	e40b                                    	addi.n	a14, a4, -1
     826:	637d                                    	ae_s16i.n	a7, a3, 0
     828:	346490                                    	extui	a6, a9, 4, 4
     82b:	390240400cb9752e                  	{ ae_db	a5, a2; extui	a7, a9, 0, 4 }
     833:	0d4d                                    	mov.n	a4, a13
     835:	419e76                                    	loopnez	a14, 87a <sbc_frame_decode+0x3f2>
     838:	320721c00881047e                  	{ s16i	a7, a4, 2; ae_lb	a7, a2 }
     840:	646d                                    	ae_s16i.n	a6, a4, 0
     842:	0802a4400881039e                  	{ s16i	a9, a3, 2; addi	a8, a4, 4 }
     84a:	070244400cb9752e                  	{ ae_db	a5, a2; extui	a4, a7, 4, 4 }
     852:	120721c00082084e                  	{ s16i	a4, a8, 0; ae_lb	a9, a2 }
     85a:	0502e8411061207e                  	{ extui	a6, a7, 0, 4; addi	a4, a8, 4 }
     862:	0202a3400881186e                  	{ s16i	a6, a8, 2; addi	a3, a3, 4 }
     86a:	380220400099137e                  	{ s16i	a7, a3, 0; extui	a7, a9, 0, 4 }
     872:	290244400cb9752e                  	{ ae_db	a5, a2; extui	a6, a9, 4, 4 }

0000087a <sbc_frame_decode+0x3f2>:
     87a:	ff9306                                    	j	6ca <sbc_frame_decode+0x242>

0000087d <sbc_frame_decode+0x3f5>:
     87d:	a0a452                                    	movi	a5, 0x4a0
     880:	061c62                                    	l16ui	a6, a12, 12
     883:	0bdc42                                    	addmi	a4, a12, 0xb00
     886:	5c5a                                    	add.n	a5, a12, a5
     888:	5394f2                                    	l16si	a15, a4, 166
     88b:	286452                                    	s32i	a5, a4, 160
     88e:	0bdc32                                    	addmi	a3, a12, 0xb00
     891:	5254f2                                    	s16i	a15, a4, 164
     894:	0bdc22                                    	addmi	a2, a12, 0xb00
     897:	4d4666                                    	bnei	a6, 4, 8e8 <sbc_frame_decode+0x460>
     89a:	fde551                                    	l32r	a5, 30 (16d0 <SBC_synthesis4_per_channel>)
     89d:	001286                                    	j	8eb <sbc_frame_decode+0x463>

000008a0 <sbc_frame_decode+0x418>:
     8a0:	0bdc32                                    	addmi	a3, a12, 0xb00
     8a3:	6c7a                                    	add.n	a6, a12, a7
     8a5:	5493f2                                    	l16si	a15, a3, 168
     8a8:	286362                                    	s32i	a6, a3, 160
     8ab:	5253f2                                    	s16i	a15, a3, 164
     8ae:	0bdc42                                    	addmi	a4, a12, 0xb00
     8b1:	6b4866                                    	bnei	a8, 4, 920 <sbc_frame_decode+0x498>
     8b4:	fddf51                                    	l32r	a5, 30 (16d0 <SBC_synthesis4_per_channel>)
     8b7:	001a06                                    	j	923 <sbc_frame_decode+0x49b>

000008ba <sbc_frame_decode+0x432>:
     8ba:	fdde51                                    	l32r	a5, 34 (1c90 <SBC_synthesis8_per_channel>)
     8bd:	31b8                                    	l32i.n	a11, a1, 12
     8bf:	0a0c                                    	movi.n	a10, 0
     8c1:	0005e0                                    	callx8	a5
     8c4:	529442                                    	l16si	a4, a4, 164
     8c7:	535342                                    	s16i	a4, a3, 166
     8ca:	3138                                    	l32i.n	a3, a1, 12
     8cc:	2148                                    	l32i.n	a4, a1, 8
     8ce:	094216                                    	beqz	a2, 966 <sbc_frame_decode+0x4de>
     8d1:	0f8276                                    	loop	a2, 8e4 <sbc_frame_decode+0x45c>
     8d4:	220283400080132e                  	{ l16si	a2, a3, 0; addi	a3, a3, 2 }
     8dc:	2402a4400080042e                  	{ s16i	a2, a4, 0; addi	a4, a4, 2 }

000008e4 <sbc_frame_decode+0x45c>:
     8e4:	020c                                    	movi.n	a2, 0
     8e6:	f01d                                    	retw.n

000008e8 <sbc_frame_decode+0x460>:
     8e8:	fdd351                                    	l32r	a5, 34 (1c90 <SBC_synthesis8_per_channel>)
     8eb:	31b8                                    	l32i.n	a11, a1, 12
     8ed:	0a0c                                    	movi.n	a10, 0
     8ef:	0c7d                                    	mov.n	a7, a12
     8f1:	0005e0                                    	callx8	a5
     8f4:	529252                                    	l16si	a5, a2, 164
     8f7:	071762                                    	l16ui	a6, a7, 14
     8fa:	535452                                    	s16i	a5, a4, 166
     8fd:	061782                                    	l16ui	a8, a7, 12
     900:	20a792                                    	movi	a9, 0x720
     903:	0bd742                                    	addmi	a4, a7, 0xb00
     906:	579a                                    	add.n	a5, a7, a9
     908:	5a2666                                    	bnei	a6, 2, 966 <sbc_frame_decode+0x4de>
     90b:	5494f2                                    	l16si	a15, a4, 168
     90e:	07cd                                    	mov.n	a12, a7
     910:	286352                                    	s32i	a5, a3, 160
     913:	5252f2                                    	s16i	a15, a2, 164
     916:	3c4866                                    	bnei	a8, 4, 956 <sbc_frame_decode+0x4ce>
     919:	fdc531                                    	l32r	a3, 30 (16d0 <SBC_synthesis4_per_channel>)
     91c:	000e46                                    	j	959 <sbc_frame_decode+0x4d1>

0000091f <sbc_frame_decode+0x497>:
	...

00000920 <sbc_frame_decode+0x498>:
     920:	fdc551                                    	l32r	a5, 34 (1c90 <SBC_synthesis8_per_channel>)
     923:	21b8                                    	l32i.n	a11, a1, 8
     925:	1a0c                                    	movi.n	a10, 1
     927:	0005e0                                    	callx8	a5
     92a:	529442                                    	l16si	a4, a4, 164
     92d:	545342                                    	s16i	a4, a3, 168
     930:	3138                                    	l32i.n	a3, a1, 12
     932:	2148                                    	l32i.n	a4, a1, 8
     934:	e2ac                                    	beqz.n	a2, 966 <sbc_frame_decode+0x4de>
     936:	0020f0                                    	nop
     939:	f03d                                    	nop.n
     93b:	f03d                                    	nop.n
     93d:	0f8276                                    	loop	a2, 950 <sbc_frame_decode+0x4c8>
     940:	240284400080042e                  	{ l16si	a2, a4, 0; addi	a4, a4, 2 }
     948:	2202a3400080132e                  	{ s16i	a2, a3, 0; addi	a3, a3, 2 }

00000950 <sbc_frame_decode+0x4c8>:
     950:	020c                                    	movi.n	a2, 0
     952:	f01d                                    	retw.n

00000954 <sbc_frame_decode+0x4cc>:
	...

00000956 <sbc_frame_decode+0x4ce>:
     956:	fdb731                                    	l32r	a3, 34 (1c90 <SBC_synthesis8_per_channel>)
     959:	21b8                                    	l32i.n	a11, a1, 8
     95b:	1a0c                                    	movi.n	a10, 1
     95d:	0003e0                                    	callx8	a3
     960:	529222                                    	l16si	a2, a2, 164
     963:	545422                                    	s16i	a2, a4, 168
     966:	020c                                    	movi.n	a2, 0
     968:	f01d                                    	retw.n

0000096a <sbc_frame_decode+0x4e2>:
	...

0000096c <sbc_zero_sb_samples>:
     96c:	004136                                    	entry	a1, 32
     96f:	58c2a2                                    	addi	a10, a2, 88
     972:	fdab21                                    	l32r	a2, 20 (f8 <memset>)
     975:	0b0c                                    	movi.n	a11, 0
     977:	00a4c2                                    	movi	a12, 0x400
     97a:	0002e0                                    	callx8	a2
     97d:	f01d                                    	retw.n

0000097f <sbc_zero_sb_samples+0x13>:
	...

00000980 <pseudo_stereo>:
     980:	004136                                    	entry	a1, 32
     983:	f01d                                    	retw.n

00000985 <pseudo_stereo+0x5>:
     985:	000000                                        ...

00000988 <pseudo_stereo_setup>:
     988:	004136                                    	entry	a1, 32
     98b:	f01d                                    	retw.n

0000098d <pseudo_stereo_setup+0x5>:
     98d:	000000                                        ...

00000990 <sbc_bit_allocation>:
     990:	00c136                                    	entry	a1, 96
     993:	041242                                    	l16ui	a4, a2, 8
     996:	4f3c                                    	movi.n	a15, 52
     998:	9149                                    	s32i.n	a4, a1, 36
     99a:	c30c                                    	movi.n	a3, 12
     99c:	051282                                    	l16ui	a8, a2, 10
     99f:	02ed                                    	mov.n	a14, a2
     9a1:	044140                                    	extui	a4, a4, 1, 1
     9a4:	050c                                    	movi.n	a5, 0
     9a6:	933f40                                    	movnez	a3, a15, a4
     9a9:	fda341                                    	l32r	a4, 38 (32e0 <SBC_OFFSET8>)
     9ac:	223a                                    	add.n	a2, a2, a3
     9ae:	021e32                                    	l16ui	a3, a14, 4
     9b1:	42cd                                    	ae_l16ui.n	a12, a2, 0
     9b3:	061e22                                    	l16ui	a2, a14, 12
     9b6:	78a472                                    	movi	a7, 0x478
     9b9:	3d1826                                    	beqi	a8, 1, 9fa <sbc_bit_allocation+0x6a>
     9bc:	9c0b                                    	addi.n	a9, a12, -1
     9be:	8cbc                                    	beqz.n	a12, 9fa <sbc_bit_allocation+0x6a>
     9c0:	fd9f61                                    	l32r	a6, 3c (32c0 <SBC_OFFSET4>)
     9c3:	fcc2a2                                    	addi	a10, a2, -4
     9c6:	c1d320                                    	mul16u	a13, a3, a2
     9c9:	320b                                    	addi.n	a3, a2, -1
     9cb:	9364a0                                    	movnez	a6, a4, a10
     9ce:	10f530                                    	and	a15, a5, a3
     9d1:	902d60                                    	addx2	a2, a13, a6
     9d4:	4e7a                                    	add.n	a4, a14, a7
     9d6:	906f20                                    	addx2	a6, a15, a2
     9d9:	551b                                    	addi.n	a5, a5, 1
     9db:	266d                                    	ae_l16si.n	a6, a6, 0
     9dd:	179976                                    	loopnez	a9, 9f8 <sbc_bit_allocation+0x68>
     9e0:	1502a54518a0553e                  	{ and	a7, a5, a3; addi	a5, a5, 1 }
     9e8:	2206a7400095046e                  	{ s16i	a6, a4, 0; addx2	a6, a7, a2 }
     9f0:	240284400080066e                  	{ l16si	a6, a6, 0; addi	a4, a4, 2 }

000009f8 <sbc_bit_allocation+0x68>:
     9f8:	646d                                    	ae_s16i.n	a6, a4, 0

000009fa <sbc_bit_allocation+0x6a>:
     9fa:	d5a522                                    	movi	a2, 0x5d5
     9fd:	38ce32                                    	addi	a3, a14, 56
     a00:	78a4f2                                    	movi	a15, 0x478
     a03:	1122f0                                    	slli	a2, a2, 1
     a06:	7139                                    	s32i.n	a3, a1, 28
     a08:	041c                                    	movi.n	a4, 16
     a0a:	050c                                    	movi.n	a5, 0
     a0c:	3efa                                    	add.n	a3, a14, a15
     a0e:	086142                                    	s32i	a4, a1, 32
     a11:	802e20                                    	add	a2, a14, a2
     a14:	00a042                                    	movi	a4, 0
     a17:	00a092                                    	movi	a9, 0
     a1a:	046152                                    	s32i	a5, a1, 16
     a1d:	0a6132                                    	s32i	a3, a1, 40
     a20:	5129                                    	s32i.n	a2, a1, 20
     a22:	b1e9                                    	s32i.n	a14, a1, 44
     a24:	061ea2                                    	l16ui	a10, a14, 12
     a27:	f149                                    	s32i.n	a4, a1, 60
     a29:	5128                                    	l32i.n	a2, a1, 20
     a2b:	050c                                    	movi.n	a5, 0
     a2d:	7138                                    	l32i.n	a3, a1, 28
     a2f:	82b4a0                                    	mull	a11, a4, a10
     a32:	a148                                    	l32i.n	a4, a1, 40
     a34:	5e1866                                    	bnei	a8, 1, a96 <sbc_bit_allocation+0x106>
     a37:	5128                                    	l32i.n	a2, a1, 20
     a39:	286c16                                    	beqz	a12, cc3 <sbc_bit_allocation+0x333>
     a3c:	71f8                                    	l32i.n	a15, a1, 28
     a3e:	050c                                    	movi.n	a5, 0
     a40:	8042b0                                    	add	a4, a2, a11
     a43:	4121c0                                    	srli	a2, a12, 1
     a46:	803fb0                                    	add	a3, a15, a11
     a49:	0c6c07                                    	bbci	a12, 0, a59 <sbc_bit_allocation+0xc9>
     a4c:	237d                                    	ae_l16si.n	a7, a3, 0
     a4e:	02c332                                    	addi	a3, a3, 2
     a51:	647d                                    	ae_s16i.n	a7, a4, 0
     a53:	02c442                                    	addi	a4, a4, 2
     a56:	535570                                    	max	a5, a5, a7
     a59:	f20b                                    	addi.n	a15, a2, -1
     a5b:	063216                                    	beqz	a2, ac2 <sbc_bit_allocation+0x132>
     a5e:	236d                                    	ae_l16si.n	a6, a3, 0
     a60:	646d                                    	ae_s16i.n	a6, a4, 0
     a62:	337d                                    	ae_l16si.n	a7, a3, 2
     a64:	334b                                    	addi.n	a3, a3, 4
     a66:	536560                                    	max	a6, a5, a6
     a69:	747d                                    	ae_s16i.n	a7, a4, 2
     a6b:	072d                                    	mov.n	a2, a7
     a6d:	1f9f76                                    	loopnez	a15, a90 <sbc_bit_allocation+0x100>
     a70:	040284400081035e                  	{ l16si	a5, a3, 0; addi	a4, a4, 4 }
     a78:	2206a6c00089045e                  	{ s16i	a5, a4, 0; max	a6, a6, a2 }
     a80:	020283400881132e                  	{ l16si	a2, a3, 2; addi	a3, a3, 4 }
     a88:	2406a6c00889142e                  	{ s16i	a2, a4, 2; max	a6, a6, a5 }

00000a90 <sbc_bit_allocation+0x100>:
     a90:	535620                                    	max	a5, a6, a2
     a93:	000ac6                                    	j	ac2 <sbc_bit_allocation+0x132>

00000a96 <sbc_bit_allocation+0x106>:
     a96:	22ba                                    	add.n	a2, a2, a11
     a98:	227c16                                    	beqz	a12, cc3 <sbc_bit_allocation+0x333>
     a9b:	33ba                                    	add.n	a3, a3, a11
     a9d:	218c76                                    	loop	a12, ac2 <sbc_bit_allocation+0x132>
     aa0:	436d                                    	ae_l16ui.n	a6, a3, 0
     aa2:	f68c                                    	beqz.n	a6, ab5 <sbc_bit_allocation+0x125>
     aa4:	247d                                    	ae_l16si.n	a7, a4, 0
     aa6:	c06670                                    	sub	a6, a6, a7
     aa9:	727960                                    	salt	a7, a9, a6
     aac:	400700                                    	ssr	a7
     aaf:	b16060                                    	sra	a6, a6
     ab2:	000046                                    	j	ab7 <sbc_bit_allocation+0x127>

00000ab5 <sbc_bit_allocation+0x125>:
     ab5:	b67c                                    	movi.n	a6, -5
     ab7:	626d                                    	ae_s16i.n	a6, a2, 0
     ab9:	535650                                    	max	a5, a6, a5
     abc:	222b                                    	addi.n	a2, a2, 2
     abe:	332b                                    	addi.n	a3, a3, 2
     ac0:	442b                                    	addi.n	a4, a4, 2

00000ac2 <sbc_bit_allocation+0x132>:
     ac2:	4128                                    	l32i.n	a2, a1, 16
     ac4:	081ed2                                    	l16ui	a13, a14, 16
     ac7:	51f8                                    	l32i.n	a15, a1, 20
     ac9:	6199                                    	s32i.n	a9, a1, 24
     acb:	351b                                    	addi.n	a3, a5, 1
     acd:	d1a9                                    	s32i.n	a10, a1, 52
     acf:	8222a0                                    	mull	a2, a2, a10
     ad2:	efba                                    	add.n	a14, a15, a11
     ad4:	c129                                    	s32i.n	a2, a1, 48
     ad6:	0f0c                                    	movi.n	a15, 0
     ad8:	e1b9                                    	s32i.n	a11, a1, 56
     ada:	080c                                    	movi.n	a8, 0

00000adc <sbc_bit_allocation+0x14c>:
     adc:	f8fa                                    	add.n	a15, a8, a15
     ade:	080c                                    	movi.n	a8, 0
     ae0:	032d                                    	mov.n	a2, a3
     ae2:	050c                                    	movi.n	a5, 0
     ae4:	32fb                                    	addi.n	a3, a2, 15
     ae6:	0adc16                                    	beqz	a12, b97 <sbc_bit_allocation+0x207>
     ae9:	0e4d                                    	mov.n	a4, a14
     aeb:	4161c0                                    	srli	a6, a12, 1
     aee:	b52b                                    	addi.n	a11, a5, 2
     af0:	196c07                                    	bbci	a12, 0, b0d <sbc_bit_allocation+0x17d>
     af3:	24ad                                    	ae_l16si.n	a10, a4, 0
     af5:	851b                                    	addi.n	a8, a5, 1
     af7:	442b                                    	addi.n	a4, a4, 2
     af9:	c072a0                                    	sub	a7, a2, a10
     afc:	729a30                                    	salt	a9, a10, a3
     aff:	93b570                                    	movnez	a11, a5, a7
     b02:	72a2a0                                    	salt	a10, a2, a10
     b05:	838b90                                    	moveqz	a8, a11, a9
     b08:	838ba0                                    	moveqz	a8, a11, a10
     b0b:	085d                                    	mov.n	a5, a8
     b0d:	b60b                                    	addi.n	a11, a6, -1
     b0f:	084616                                    	beqz	a6, b97 <sbc_bit_allocation+0x207>
     b12:	247d                                    	ae_l16si.n	a7, a4, 0
     b14:	058d                                    	mov.n	a8, a5
     b16:	652b                                    	addi.n	a6, a5, 2
     b18:	34ad                                    	ae_l16si.n	a10, a4, 2
     b1a:	551b                                    	addi.n	a5, a5, 1
     b1c:	04c442                                    	addi	a4, a4, 4
     b1f:	c09270                                    	sub	a9, a2, a7
     b22:	936890                                    	movnez	a6, a8, a9
     b25:	728730                                    	salt	a8, a7, a3
     b28:	729270                                    	salt	a9, a2, a7
     b2b:	835680                                    	moveqz	a5, a6, a8
     b2e:	835690                                    	moveqz	a5, a6, a9
     b31:	c072a0                                    	sub	a7, a2, a10
     b34:	01c582                                    	addi	a8, a5, 1
     b37:	729a30                                    	salt	a9, a10, a3
     b3a:	02c562                                    	addi	a6, a5, 2
     b3d:	4a9b76                                    	loopnez	a11, b8b <sbc_bit_allocation+0x1fb>
     b40:	260695400085147e                  	{ l16si	a7, a4, 0; movnez	a6, a5, a7 }
     b48:	1a06b24600eda69e                  	{ moveqz	a8, a6, a9; salt	a5, a2, a10 }
     b50:	040686c0089a14ae                  	{ l16si	a10, a4, 2; moveqz	a8, a6, a5 }
     b58:	2602e8400c60085e                  	{ addi	a5, a8, 1; addi	a6, a8, 2 }
     b60:	c09270                                    	sub	a9, a2, a7
     b63:	0502a44510e1c89e                  	{ movnez	a6, a8, a9; addi	a4, a4, 4 }
     b6b:	1306ba4600eef73e                  	{ salt	a8, a7, a3; salt	a9, a10, a3 }
     b73:	3606b24508edb68e                  	{ moveqz	a5, a6, a8; salt	a7, a2, a7 }
     b7b:	3a06b24508f9a67e                  	{ moveqz	a5, a6, a7; sub	a7, a2, a10 }
     b83:	2602e5400c60058e                  	{ addi	a8, a5, 1; addi	a6, a5, 2 }

00000b8b <sbc_bit_allocation+0x1fb>:
     b8b:	936570                                    	movnez	a6, a5, a7
     b8e:	72b2a0                                    	salt	a11, a2, a10
     b91:	838690                                    	moveqz	a8, a6, a9
     b94:	8386b0                                    	moveqz	a8, a6, a11

00000b97 <sbc_bit_allocation+0x207>:
     b97:	a8fa                                    	add.n	a10, a8, a15
     b99:	320b                                    	addi.n	a3, a2, -1
     b9b:	2207009c1fe5fade                  	{ blt.w15	a10, a13, adc <sbc_bit_allocation+0x14c>; nop }
     ba3:	fec222                                    	addi	a2, a2, -2
     ba6:	c0ead0                                    	sub	a14, a10, a13
     ba9:	9323e0                                    	movnez	a2, a3, a14
     bac:	5138                                    	l32i.n	a3, a1, 20
     bae:	e148                                    	l32i.n	a4, a1, 56
     bb0:	ccbc                                    	beqz.n	a12, bf0 <sbc_bit_allocation+0x260>
     bb2:	9c0b                                    	addi.n	a9, a12, -1
     bb4:	8158                                    	l32i.n	a5, a1, 32
     bb6:	734a                                    	add.n	a7, a3, a4
     bb8:	322b                                    	addi.n	a3, a2, 2
     bba:	274d                                    	ae_l16si.n	a4, a7, 0
     bbc:	c08420                                    	sub	a8, a4, a2
     bbf:	726430                                    	salt	a6, a4, a3
     bc2:	062142                                    	l32i	a4, a1, 24
     bc5:	1a9976                                    	loopnez	a9, be3 <sbc_bit_allocation+0x253>
     bc8:	040688c00896179e                  	{ l16si	a9, a7, 2; minu	a8, a8, a5 }
     bd0:	938460                                    	movnez	a8, a4, a6
     bd3:	2602a7400080378e                  	{ s16i	a8, a7, 64; addi	a7, a7, 2 }
     bdb:	2306994204ed592e                  	{ sub	a8, a9, a2; salt	a6, a9, a3 }

00000be3 <sbc_bit_allocation+0x253>:
     be3:	633850                                    	minu	a3, a8, a5
     be6:	8159                                    	s32i.n	a5, a1, 32
     be8:	933460                                    	movnez	a3, a4, a6
     beb:	6149                                    	s32i.n	a4, a1, 24
     bed:	205732                                    	s16i	a3, a7, 64

00000bf0 <sbc_bit_allocation+0x260>:
     bf0:	4138                                    	l32i.n	a3, a1, 16
     bf2:	9148                                    	l32i.n	a4, a1, 36
     bf4:	93afe0                                    	movnez	a10, a15, a14
     bf7:	b1e8                                    	l32i.n	a14, a1, 44
     bf9:	f5a5b2                                    	movi	a11, 0x5f5
     bfc:	df0c                                    	movi.n	a15, 13
     bfe:	11bbf0                                    	slli	a11, a11, 1
     c01:	331b                                    	addi.n	a3, a3, 1
     c03:	d158                                    	l32i.n	a5, a1, 52
     c05:	230740860005d41e                  	{ bbsi.w15	a4, 1, ca4 <sbc_bit_allocation+0x314>; nop }
     c0d:	4148                                    	l32i.n	a4, a1, 16
     c0f:	52aad7                                    	bge	a10, a13, c65 <sbc_bit_allocation+0x2d5>
     c12:	c168                                    	l32i.n	a6, a1, 48
     c14:	441b                                    	addi.n	a4, a4, 1
     c16:	825450                                    	mull	a5, a4, a5
     c19:	9076e0                                    	addx2	a7, a6, a14
     c1c:	45b657                                    	bgeu	a6, a5, c65 <sbc_bit_allocation+0x2d5>
     c1f:	521b                                    	addi.n	a5, a2, 1
     c21:	67ba                                    	add.n	a6, a7, a11
     c23:	3b8076                                    	loop	a0, c62 <sbc_bit_allocation+0x2d2>
     c26:	467d                                    	ae_l16ui.n	a7, a6, 0
     c28:	9a1b                                    	addi.n	a9, a10, 1
     c2a:	fec782                                    	addi	a8, a7, -2
     c2d:	771b                                    	addi.n	a7, a7, 1
     c2f:	988d                                    	ae_zext16	a8, a8
     c31:	043f87                                    	bltu	a15, a8, c39 <sbc_bit_allocation+0x2a9>
     c34:	180c                                    	movi.n	a8, 1
     c36:	000386                                    	j	c48 <sbc_bit_allocation+0x2b8>

00000c39 <sbc_bit_allocation+0x2a9>:
     c39:	c0c672                                    	addi	a7, a6, -64
     c3c:	0ca9d7                                    	bge	a9, a13, c4c <sbc_bit_allocation+0x2bc>
     c3f:	279d                                    	ae_l16si.n	a9, a7, 0
     c41:	280c                                    	movi.n	a8, 2
     c43:	270c                                    	movi.n	a7, 2
     c45:	039597                                    	bne	a5, a9, c4c <sbc_bit_allocation+0x2bc>
     c48:	667d                                    	ae_s16i.n	a7, a6, 0
     c4a:	aa8a                                    	add.n	a10, a10, a8
     c4c:	081ed2                                    	l16ui	a13, a14, 16
     c4f:	662b                                    	addi.n	a6, a6, 2
     c51:	061e72                                    	l16ui	a7, a14, 12
     c54:	0daad7                                    	bge	a10, a13, c65 <sbc_bit_allocation+0x2d5>
     c57:	827470                                    	mull	a7, a4, a7
     c5a:	9077e0                                    	addx2	a7, a7, a14
     c5d:	77ba                                    	add.n	a7, a7, a11
     c5f:	02b677                                    	bgeu	a6, a7, c65 <sbc_bit_allocation+0x2d5>

00000c62 <sbc_bit_allocation+0x2d2>:
     c62:	ffef46                                    	j	c23 <sbc_bit_allocation+0x293>

00000c65 <sbc_bit_allocation+0x2d5>:
     c65:	4158                                    	l32i.n	a5, a1, 16
     c67:	39aad7                                    	bge	a10, a13, ca4 <sbc_bit_allocation+0x314>
     c6a:	061e62                                    	l16ui	a6, a14, 12
     c6d:	451b                                    	addi.n	a4, a5, 1
     c6f:	825560                                    	mull	a5, a5, a6
     c72:	826460                                    	mull	a6, a4, a6
     c75:	9075e0                                    	addx2	a7, a5, a14
     c78:	28b567                                    	bgeu	a5, a6, ca4 <sbc_bit_allocation+0x314>
     c7b:	57ba                                    	add.n	a5, a7, a11
     c7d:	208076                                    	loop	a0, ca1 <sbc_bit_allocation+0x311>
     c80:	456d                                    	ae_l16ui.n	a6, a5, 0
     c82:	761b                                    	addi.n	a7, a6, 1
     c84:	06b6f6                                    	bgeui	a6, 16, c8e <sbc_bit_allocation+0x2fe>
     c87:	aa1b                                    	addi.n	a10, a10, 1
     c89:	657d                                    	ae_s16i.n	a7, a5, 0
     c8b:	081ed2                                    	l16ui	a13, a14, 16
     c8e:	552b                                    	addi.n	a5, a5, 2
     c90:	10aad7                                    	bge	a10, a13, ca4 <sbc_bit_allocation+0x314>
     c93:	061e62                                    	l16ui	a6, a14, 12
     c96:	826460                                    	mull	a6, a4, a6
     c99:	9066e0                                    	addx2	a6, a6, a14
     c9c:	66ba                                    	add.n	a6, a6, a11
     c9e:	02b567                                    	bgeu	a5, a6, ca4 <sbc_bit_allocation+0x314>

00000ca1 <sbc_bit_allocation+0x311>:
     ca1:	fff606                                    	j	c7d <sbc_bit_allocation+0x2ed>

00000ca4 <sbc_bit_allocation+0x314>:
     ca4:	041e72                                    	l16ui	a7, a14, 8
     ca7:	240c                                    	movi.n	a4, 2
     ca9:	f1b8                                    	l32i.n	a11, a1, 60
     cab:	160c                                    	movi.n	a6, 1
     cad:	051e82                                    	l16ui	a8, a14, 10
     cb0:	6198                                    	l32i.n	a9, a1, 24
     cb2:	570b                                    	addi.n	a5, a7, -1
     cb4:	934650                                    	movnez	a4, a6, a5
     cb7:	0fb347                                    	bgeu	a3, a4, cca <sbc_bit_allocation+0x33a>
     cba:	4b2b                                    	addi.n	a4, a11, 2
     cbc:	9179                                    	s32i.n	a7, a1, 36
     cbe:	4139                                    	s32i.n	a3, a1, 16
     cc0:	ff5806                                    	j	a24 <sbc_bit_allocation+0x94>

00000cc3 <sbc_bit_allocation+0x333>:
     cc3:	050c                                    	movi.n	a5, 0
     cc5:	ff7e46                                    	j	ac2 <sbc_bit_allocation+0x132>

00000cc8 <sbc_bit_allocation+0x338>:
	...

00000cca <sbc_bit_allocation+0x33a>:
     cca:	221b                                    	addi.n	a2, a2, 1
     ccc:	220740800005f71e                  	{ bbci.w15	a7, 1, d6c <sbc_bit_allocation+0x3dc>; nop }
     cd4:	061e72                                    	l16ui	a7, a14, 12
     cd7:	56aad7                                    	bge	a10, a13, d31 <sbc_bit_allocation+0x3a1>
     cda:	d40c                                    	movi.n	a4, 13
     cdc:	150c                                    	movi.n	a5, 1
     cde:	060c                                    	movi.n	a6, 0
     ce0:	030c                                    	movi.n	a3, 0
     ce2:	04b716                                    	beqz	a7, d31 <sbc_bit_allocation+0x3a1>
     ce5:	458076                                    	loop	a0, d2e <sbc_bit_allocation+0x39e>
     ce8:	827370                                    	mull	a7, a3, a7
     ceb:	9077e0                                    	addx2	a7, a7, a14
     cee:	ca1b                                    	addi.n	a12, a10, 1
     cf0:	908670                                    	addx2	a8, a6, a7
     cf3:	0bd872                                    	addmi	a7, a8, 0xb00
     cf6:	0bd882                                    	addmi	a8, a8, 0xb00
     cf9:	751792                                    	l16ui	a9, a7, 234
     cfc:	fec9b2                                    	addi	a11, a9, -2
     cff:	9bbd                                    	ae_zext16	a11, a11
     d01:	0634b7                                    	bltu	a4, a11, d0b <sbc_bit_allocation+0x37b>
     d04:	891b                                    	addi.n	a8, a9, 1
     d06:	190c                                    	movi.n	a9, 1
     d08:	000306                                    	j	d18 <sbc_bit_allocation+0x388>

00000d0b <sbc_bit_allocation+0x37b>:
     d0b:	290c                                    	movi.n	a9, 2
     d0d:	0cacd7                                    	bge	a12, a13, d1d <sbc_bit_allocation+0x38d>
     d10:	5598b2                                    	l16si	a11, a8, 170
     d13:	280c                                    	movi.n	a8, 2
     d15:	0492b7                                    	bne	a2, a11, d1d <sbc_bit_allocation+0x38d>
     d18:	755782                                    	s16i	a8, a7, 234
     d1b:	aa9a                                    	add.n	a10, a10, a9
     d1d:	081ed2                                    	l16ui	a13, a14, 16
     d20:	663a                                    	add.n	a6, a6, a3
     d22:	061e72                                    	l16ui	a7, a14, 12
     d25:	303350                                    	xor	a3, a3, a5
     d28:	05aad7                                    	bge	a10, a13, d31 <sbc_bit_allocation+0x3a1>
     d2b:	02b677                                    	bgeu	a6, a7, d31 <sbc_bit_allocation+0x3a1>

00000d2e <sbc_bit_allocation+0x39e>:
     d2e:	ffecc6                                    	j	ce5 <sbc_bit_allocation+0x355>

00000d31 <sbc_bit_allocation+0x3a1>:
     d31:	020c                                    	movi.n	a2, 0
     d33:	35aad7                                    	bge	a10, a13, d6c <sbc_bit_allocation+0x3dc>
     d36:	130c                                    	movi.n	a3, 1
     d38:	050c                                    	movi.n	a5, 0
     d3a:	2b8076                                    	loop	a0, d69 <sbc_bit_allocation+0x3d9>
     d3d:	061e62                                    	l16ui	a6, a14, 12
     d40:	054d                                    	mov.n	a4, a5
     d42:	26b267                                    	bgeu	a2, a6, d6c <sbc_bit_allocation+0x3dc>
     d45:	82f560                                    	mull	a15, a5, a6
     d48:	905fe0                                    	addx2	a5, a15, a14
     d4b:	905250                                    	addx2	a5, a2, a5
     d4e:	0bd552                                    	addmi	a5, a5, 0xb00
     d51:	751562                                    	l16ui	a6, a5, 234
     d54:	761b                                    	addi.n	a7, a6, 1
     d56:	07b6f6                                    	bgeui	a6, 16, d61 <sbc_bit_allocation+0x3d1>
     d59:	755572                                    	s16i	a7, a5, 234
     d5c:	aa1b                                    	addi.n	a10, a10, 1
     d5e:	081ed2                                    	l16ui	a13, a14, 16
     d61:	305430                                    	xor	a5, a4, a3
     d64:	224a                                    	add.n	a2, a2, a4
     d66:	02aad7                                    	bge	a10, a13, d6c <sbc_bit_allocation+0x3dc>

00000d69 <sbc_bit_allocation+0x3d9>:
     d69:	fff346                                    	j	d3a <sbc_bit_allocation+0x3aa>

00000d6c <sbc_bit_allocation+0x3dc>:
     d6c:	f01d                                    	retw.n

00000d6e <sbc_bit_allocation+0x3de>:
	...

00000d70 <SAT_ADD>:
     d70:	004136                                    	entry	a1, 32
     d73:	0ead456b09e133d0c48081390001020f 	{ ae_cvt48a32	aed0, a2; ae_cvt48a32	aed1, a3; nop; nop }
     d83:	0ead057b08e40cd0c48081380801c00f 	{ ae_addsq56s	aed0, aed0, aed1; nop; nop; nop }
     d93:	0ead217b0d440cd0c50061380c01015f 	{ nop; nop; nop; ae_roundsq32f48sym	aed0, aed0 }
     da3:	0ead257b0d240cd0c400813b0c01620f 	{ ae_trunca32q48	a2, aed0; nop; nop; nop }
     db3:	f01d                                    	retw.n

00000db5 <SAT_ADD+0x45>:
     db5:	000000                                        ...

00000db8 <SAT_SUB>:
     db8:	004136                                    	entry	a1, 32
     dbb:	0ead456b09e133d0c48081390001020f 	{ ae_cvt48a32	aed0, a2; ae_cvt48a32	aed1, a3; nop; nop }
     dcb:	0ead057b08e40cd0c58081380c01600f 	{ ae_subsq56s	aed0, aed0, aed1; nop; nop; nop }
     ddb:	0ead217b0d440cd0c50061380c01015f 	{ nop; nop; nop; ae_roundsq32f48sym	aed0, aed0 }
     deb:	0ead257b0d240cd0c400813b0c01620f 	{ ae_trunca32q48	a2, aed0; nop; nop; nop }
     dfb:	f01d                                    	retw.n

00000dfd <SAT_SUB+0x45>:
     dfd:	000000                                        ...

00000e00 <sbc_reconstruction>:
     e00:	006136                                    	entry	a1, 48
     e03:	025d                                    	mov.n	a5, a2
     e05:	1a1222                                    	l16ui	a2, a2, 52
     e08:	e5a572                                    	movi	a7, 0x5e5
     e0b:	f5a532                                    	movi	a3, 0x5f5
     e0e:	1177f0                                    	slli	a7, a7, 1
     e11:	38c582                                    	addi	a8, a5, 56
     e14:	1133f0                                    	slli	a3, a3, 1
     e17:	657a                                    	add.n	a6, a5, a7
     e19:	79a192                                    	movi	a9, 0x179
     e1c:	068216                                    	beqz	a2, e88 <sbc_reconstruction+0x88>
     e1f:	040464400c80608e                  	{ ae_l16m.i	aed1, a8, 0; movi	a4, 4 }
     e27:	0e02e540148eb53e                  	{ ae_l16m.xu	aed2, a3, a5; addi	a15, a5, 56 }
     e2f:	0ea56425cd279dd0c50081150023044f 	{ ae_cvtp24a16x2.ll	aed0, a4, a4; slli	a9, a9, 3; ae_sub24s	aed3, aed1, aed2; nop }
     e3f:	0ea4e407fd232ad0c400830008c341ff 	{ ae_l16m.iu	aed1, a15, 2; addi	a10, a2, -1; ae_add24s	aed3, aed3, aed0; nop }
     e4f:	459a                                    	add.n	a4, a5, a9
     e51:	0f2d                                    	mov.n	a2, a15
     e53:	0020f0                                    	nop
     e56:	f03d                                    	nop.n
     e58:	1104534100a23a8e                  	{ loopnez	a10, e80 <sbc_reconstruction+0x80>; ae_l16m.iu	aed2, a3, 2 }
     e60:	0ea5243b0c450cd0c50081110421c14f 	{ ae_s16m.l.iu	aed3, a4, 2; nop; ae_sub24s	aed1, aed1, aed2; nop }
     e70:	0ea4e411152331d0c400810008c3412f 	{ ae_l16m.iu	aed1, a2, 2; ae_l16m.iu	aed2, a3, 2; ae_add24s	aed3, aed1, aed0; nop }

00000e80 <sbc_reconstruction+0x80>:
     e80:	230740c01cc5114e                  	{ ae_s16m.l.iu	aed3, a4, 2; nop }

00000e88 <sbc_reconstruction+0x88>:
     e88:	1a1522                                    	l16ui	a2, a5, 52
     e8b:	54c542                                    	addi	a4, a5, 84
     e8e:	1b1592                                    	l16ui	a9, a5, 54
     e91:	f3f660                                    	wur.ae_cbegin0	a6
     e94:	902250                                    	addx2	a2, a2, a5
     e97:	fc6a81                                    	l32r	a8, 40 (2360 <S_ARRAY>)
     e9a:	327a                                    	add.n	a3, a2, a7
     e9c:	0c0760440461652e                  	{ addi	a2, a5, 112; ae_movi	aed1, 0 }
     ea4:	f3f730                                    	wur.ae_cend0	a3
     ea7:	ff2232                                    	l32i	a3, a2, 0x3fc
     eaa:	270c                                    	movi.n	a7, 2
     eac:	063916                                    	beqz	a9, f13 <sbc_reconstruction+0x113>
     eaf:	608976                                    	loop	a9, f13 <sbc_reconstruction+0x113>
     eb2:	370416440080169e                  	{ l16ui	a9, a6, 32; ae_l16m.xc	aed0, a6, a7 }
     eba:	290701c000e3dbfe                  	{ ae_trunca16p24s.l	a15, aed0; ae_lb	a14, a9 }
     ec2:	11a9e0                                    	slli	a10, a9, 2
     ec5:	042916                                    	beqz	a9, f0b <sbc_reconstruction+0x10b>
     ec8:	3b0458c00ca1639e                  	{ ae_db	a3, a9; ae_l16x2m.x	aed1, a8, a10 }
     ed0:	0ead257b0d240cd0c400813b0c01cb1f 	{ ae_trunca16p24s.h	a11, aed1; nop; nop; nop }
     ee0:	c0aeb0                                    	sub	a10, a14, a11
     ee3:	230700c71ca5faae                  	{ ae_cvtp24a16x2.ll	aed31, a10, a10; nop }
     eeb:	1a835403343fb13e                  	{ nop; nop; ae_mul32.lh	aed0, aed1, aed31 }
     ef3:	220700c010a51f0e                  	{ ae_slaasq56s	aed0, aed0, a15; nop }
     efb:	0ead217b0d440cd0c50461380c01015f 	{ nop; nop; nop; ae_roundsq32f48sym	aed1, aed0 }
     f0b:	0d07604008c1a14e                  	{ ae_s32m.iu	aed1, a4, 4; ae_movi	aed1, 0 }

00000f13 <sbc_reconstruction+0x113>:
     f13:	10d542                                    	addmi	a4, a5, 0x1000
     f16:	ff6232                                    	s32i	a3, a2, 0x3fc
     f19:	081442                                    	l16ui	a4, a4, 16
     f1c:	6cc532                                    	addi	a3, a5, 108
     f1f:	a8ab62                                    	movi	a6, 0xfffffba8
     f22:	10a416                                    	beqz	a4, 1030 <sbc_reconstruction+0x230>
     f25:	230740a41045940e                  	{ beqi.w15	a4, 2, 11bc <sbc_reconstruction+0x3bc>; nop }
     f2d:	230740a408e5d46e                  	{ bnei.w15	a4, 1, 16cc <sbc_reconstruction+0x8cc>; nop }
     f35:	171522                                    	l16ui	a2, a5, 46
     f38:	1b15d2                                    	l16ui	a13, a5, 54
     f3b:	061542                                    	l16ui	a4, a5, 12
     f3e:	78a216                                    	beqz	a2, 16cc <sbc_reconstruction+0x8cc>
     f41:	ff2322                                    	l32i	a2, a3, 0x3fc
     f44:	a8ab32                                    	movi	a3, 0xfffffba8
     f47:	c02250                                    	sub	a2, a2, a5
     f4a:	223a                                    	add.n	a2, a2, a3
     f4c:	56a432                                    	movi	a3, 0x456
     f4f:	21c120                                    	srai	a12, a2, 1
     f52:	1129                                    	s32i.n	a2, a1, 4
     f54:	902c50                                    	addx2	a2, a12, a5
     f57:	230720a31cc55d6e                  	{ bnei.w15	a13, 0x100, 15c5 <sbc_reconstruction+0x7c5>; nop }
     f5f:	823a                                    	add.n	a8, a2, a3
     f61:	00a1d2                                    	movi	a13, 0x100
     f64:	1128                                    	l32i.n	a2, a1, 4
     f66:	230720a200c5f46e                  	{ bnei.w15	a4, 8, 15c8 <sbc_reconstruction+0x7c8>; nop }
     f6e:	00a042                                    	movi	a4, 0

00000f71 <sbc_reconstruction+0x171>:
     f71:	fec872                                    	addi	a7, a8, -2
     f74:	9c0b                                    	addi.n	a9, a12, -1
     f76:	220740a00c0532ce                  	{ blti.w15	a2, 1, 101e <sbc_reconstruction+0x21e>; nop }
     f7e:	482d                                    	ae_l16ui.n	a2, a8, 0
     f80:	242a                                    	add.n	a2, a4, a2
     f82:	a03250                                    	addx4	a3, a2, a5
     f85:	1e2362                                    	l32i	a6, a3, 120
     f88:	162322                                    	l32i	a2, a3, 88
     f8b:	2007224410e1e55e                  	{ or	a2, a5, a5; ae_cvt48a32	aed1, a2 }
     f93:	075d                                    	mov.n	a5, a7
     f95:	4a9976                                    	loopnez	a9, fe3 <sbc_reconstruction+0x1e3>
     f98:	210706400080056e                  	{ l16ui	a6, a5, 0; ae_cvt48a32	aed0, a6 }
     fa0:	0eacc567f0e255d0c48081380001c01f 	{ ae_addsq56s	aed0, aed1, aed0; addi	a5, a5, -2; nop; nop }
     fb0:	0ead217b0d440cd0c50061380c01015f 	{ nop; nop; nop; ae_roundsq32f48sym	aed0, aed0 }
     fc0:	0ead65663d2246d0c400813b0c01660f 	{ ae_trunca32q48	a6, aed0; add	a7, a4, a6; nop; nop }
     fd0:	3306a7451098036e                  	{ s32i	a6, a3, 88; addx4	a3, a7, a2 }
     fd8:	162362                                    	l32i	a6, a3, 88
     fdb:	200706471481036e                  	{ l32i	a6, a3, 120; ae_cvt48a32	aed1, a6 }

00000fe3 <sbc_reconstruction+0x1e3>:
     fe3:	2007264508e0e22e                  	{ or	a5, a2, a2; ae_cvt48a32	aed0, a6 }
     feb:	0ead057b08e40cd0c48081380001c01f 	{ ae_addsq56s	aed0, aed1, aed0; nop; nop; nop }
     ffb:	0ead217b0d440cd0c50061380c01015f 	{ nop; nop; nop; ae_roundsq32f48sym	aed0, aed0 }
    100b:	0ead257b0d240cd0c400813b0c016f0f 	{ ae_trunca32q48	a15, aed0; nop; nop; nop }
    101b:	1663f2                                    	s32i	a15, a3, 88

0000101e <sbc_reconstruction+0x21e>:
    101e:	10c442                                    	addi	a4, a4, 16
    1021:	1128                                    	l32i.n	a2, a1, 4
    1023:	230720bb1be5542e                  	{ bltui.w15	a4, 0x100, f71 <sbc_reconstruction+0x171>; nop }
    102b:	01a746                                    	j	16cc <sbc_reconstruction+0x8cc>

0000102e <sbc_reconstruction+0x22e>:
	...

00001030 <sbc_reconstruction+0x230>:
    1030:	6cc532                                    	addi	a3, a5, 108
    1033:	171522                                    	l16ui	a2, a5, 46
    1036:	ff2342                                    	l32i	a4, a3, 0x3fc
    1039:	1b15e2                                    	l16ui	a14, a5, 54
    103c:	a8ab62                                    	movi	a6, 0xfffffba8
    103f:	061572                                    	l16ui	a7, a5, 12
    1042:	56a432                                    	movi	a3, 0x456
    1045:	683216                                    	beqz	a2, 16cc <sbc_reconstruction+0x8cc>
    1048:	c02450                                    	sub	a2, a4, a5
    104b:	226a                                    	add.n	a2, a2, a6
    104d:	21d120                                    	srai	a13, a2, 1
    1050:	1129                                    	s32i.n	a2, a1, 4
    1052:	902d50                                    	addx2	a2, a13, a5
    1055:	220720a71845fe6e                  	{ bnei.w15	a14, 0x100, 12b6 <sbc_reconstruction+0x4b6>; nop }
    105d:	923a                                    	add.n	a9, a2, a3
    105f:	00a1e2                                    	movi	a14, 0x100
    1062:	030c                                    	movi.n	a3, 0
    1064:	220720a60045976e                  	{ bnei.w15	a7, 8, 12b9 <sbc_reconstruction+0x4b9>; nop }
    106c:	1128                                    	l32i.n	a2, a1, 4
    106e:	fec962                                    	addi	a6, a9, -2
    1071:	220700a40c25d2ce                  	{ blti.w15	a2, 1, 11ae <sbc_reconstruction+0x3ae>; nop }
    1079:	492d                                    	ae_l16ui.n	a2, a9, 0
    107b:	230720c318e5747e                  	{ nop; nop }
    1083:	0020f0                                    	nop
    1086:	0020f0                                    	nop
    1089:	832a                                    	add.n	a8, a3, a2
    108b:	462d                                    	ae_l16ui.n	a2, a6, 0
    108d:	a04850                                    	addx4	a4, a8, a5
    1090:	1624f2                                    	l32i	a15, a4, 88
    1093:	20070f471480047e                  	{ l32i	a7, a4, 120; ae_cvt48a32	aed0, a15 }
    109b:	2007074204e1232e                  	{ add	a8, a3, a2; ae_cvt48a32	aed1, a7 }
    10a3:	0ea4e027f52366f8c108200f1101255f 	{ or	a2, a5, a5; addi	a6, a6, -2; ae_subsq56s	aed1, aed0, aed1; ae_addsq56s	aed2, aed0, aed1 }
    10b3:	0ead217b0c750cd0c400613a1401fdaf 	{ addi	a10, a13, -2; nop; nop; ae_roundsq32f48sym	aed0, aed2 }
    10c3:	220760a010053d2e                  	{ bltui.w15	a13, 2, 118b <sbc_reconstruction+0x38b>; nop }
    10cb:	035d                                    	mov.n	a5, a3
    10cd:	5f9a76                                    	loopnez	a10, 1130 <sbc_reconstruction+0x330>
    10d0:	0eace160052e63d0c400613b0c01670f 	{ ae_trunca32q48	a7, aed0; l16ui	a3, a6, 0; nop; ae_roundsq32f48sym	aed0, aed1 }
    10e0:	000003f3038300fbd6a303053460147f 	{ s32i	a7, a4, 88; ae_trunca32q48	a7, aed0; nop }
    10f0:	0306a8471099047e                  	{ s32i	a7, a4, 120; addx4	a4, a8, a2 }
    10f8:	280384451487047e                  	{ l32i	a7, a4, 88; l32i	a8, a4, 120 }
    1100:	0ead456b09e183d0c48081390001270f 	{ ae_cvt48a32	aed2, a7; ae_cvt48a32	aed1, a8; nop; nop }
    1110:	0ea5241b0d250cd0c500820f09a0853f 	{ add	a8, a5, a3; nop; ae_addsq56s	aed0, aed2, aed1; nop }
    1120:	0eacc167f0e266d0c58061380c01612f 	{ ae_subsq56s	aed1, aed2, aed1; addi	a6, a6, -2; nop; ae_roundsq32f48sym	aed0, aed0 }

00001130 <sbc_reconstruction+0x330>:
    1130:	0ead61679d2e55d0c404613b0c016f0f 	{ ae_trunca32q48	a15, aed0; or	a3, a5, a5; nop; ae_roundsq32f48sym	aed1, aed1 }
    1140:	025d                                    	mov.n	a5, a2
    1142:	000003f3038300fb76a30305346014ff 	{ s32i	a15, a4, 88; ae_trunca32q48	a6, aed1; nop }
    1152:	1e6462                                    	s32i	a6, a4, 120
    1155:	a04820                                    	addx4	a4, a8, a2
    1158:	162422                                    	l32i	a2, a4, 88
    115b:	20070247149e047e                  	{ l32i	a7, a4, 120; ae_cvt48a32	aed30, a2 }
    1163:	2107274318ff647e                  	{ nop; ae_cvt48a32	aed31, a7 }
    116b:	0ea5041b08e50cd0c5809eff1da261ef 	{ ae_subsq56s	aed1, aed30, aed31; nop; ae_addsq56s	aed2, aed30, aed31; nop }
    117b:	0ead217b0d540cd0c50061380c01015f 	{ nop; nop; nop; ae_roundsq32f48sym	aed0, aed2 }

0000118b <sbc_reconstruction+0x38b>:
    118b:	0ead217b0d2c0cd0c404613b0c01620f 	{ ae_trunca32q48	a2, aed0; nop; nop; ae_roundsq32f48sym	aed1, aed1 }
    119b:	000003f3038300fb76a303053460142f 	{ s32i	a2, a4, 88; ae_trunca32q48	a6, aed1; nop }
    11ab:	1e6462                                    	s32i	a6, a4, 120
    11ae:	10c332                                    	addi	a3, a3, 16
    11b1:	230740bf1fc5b32e                  	{ bltui.w15	a3, 0x100, 106c <sbc_reconstruction+0x26c>; nop }
    11b9:	0143c6                                    	j	16cc <sbc_reconstruction+0x8cc>

000011bc <sbc_reconstruction+0x3bc>:
    11bc:	171522                                    	l16ui	a2, a5, 46
    11bf:	6cc532                                    	addi	a3, a5, 108
    11c2:	ff2342                                    	l32i	a4, a3, 0x3fc
    11c5:	1b15d2                                    	l16ui	a13, a5, 54
    11c8:	061572                                    	l16ui	a7, a5, 12
    11cb:	56a432                                    	movi	a3, 0x456
    11ce:	4fa216                                    	beqz	a2, 16cc <sbc_reconstruction+0x8cc>
    11d1:	c02450                                    	sub	a2, a4, a5
    11d4:	040c                                    	movi.n	a4, 0
    11d6:	226a                                    	add.n	a2, a2, a6
    11d8:	21c120                                    	srai	a12, a2, 1
    11db:	1129                                    	s32i.n	a2, a1, 4
    11dd:	902c50                                    	addx2	a2, a12, a5
    11e0:	220760a718453d6e                  	{ bnei.w15	a13, 0x100, 14a9 <sbc_reconstruction+0x6a9>; nop }
    11e8:	808230                                    	add	a8, a2, a3
    11eb:	00a1d2                                    	movi	a13, 0x100
    11ee:	012122                                    	l32i	a2, a1, 4
    11f1:	230740a60445b76e                  	{ bnei.w15	a7, 8, 14ac <sbc_reconstruction+0x6ac>; nop }
    11f9:	fec872                                    	addi	a7, a8, -2
    11fc:	9c0b                                    	addi.n	a9, a12, -1
    11fe:	220740a00c0532ce                  	{ blti.w15	a2, 1, 12a6 <sbc_reconstruction+0x4a6>; nop }
    1206:	482d                                    	ae_l16ui.n	a2, a8, 0
    1208:	242a                                    	add.n	a2, a4, a2
    120a:	a03250                                    	addx4	a3, a2, a5
    120d:	1e2362                                    	l32i	a6, a3, 120
    1210:	162322                                    	l32i	a2, a3, 88
    1213:	2007224410e1e55e                  	{ or	a2, a5, a5; ae_cvt48a32	aed1, a2 }
    121b:	075d                                    	mov.n	a5, a7
    121d:	4a9976                                    	loopnez	a9, 126b <sbc_reconstruction+0x46b>
    1220:	210706400080056e                  	{ l16ui	a6, a5, 0; ae_cvt48a32	aed0, a6 }
    1228:	0eacc567f0e255d0c58081380401601f 	{ ae_subsq56s	aed0, aed1, aed0; addi	a5, a5, -2; nop; nop }
    1238:	0ead217b0d440cd0c50061380c01015f 	{ nop; nop; nop; ae_roundsq32f48sym	aed0, aed0 }
    1248:	0ead65663d2246d0c400813b0c01660f 	{ ae_trunca32q48	a6, aed0; add	a7, a4, a6; nop; nop }
    1258:	3306a7471098036e                  	{ s32i	a6, a3, 120; addx4	a3, a7, a2 }
    1260:	162362                                    	l32i	a6, a3, 88
    1263:	200706471481036e                  	{ l32i	a6, a3, 120; ae_cvt48a32	aed1, a6 }

0000126b <sbc_reconstruction+0x46b>:
    126b:	2007264508e0e22e                  	{ or	a5, a2, a2; ae_cvt48a32	aed0, a6 }
    1273:	0ead057b08e40cd0c58081380401601f 	{ ae_subsq56s	aed0, aed1, aed0; nop; nop; nop }
    1283:	0ead217b0d440cd0c50061380c01015f 	{ nop; nop; nop; ae_roundsq32f48sym	aed0, aed0 }
    1293:	0ead257b0d240cd0c400813b0c016f0f 	{ ae_trunca32q48	a15, aed0; nop; nop; nop }
    12a3:	1e63f2                                    	s32i	a15, a3, 120

000012a6 <sbc_reconstruction+0x4a6>:
    12a6:	10c442                                    	addi	a4, a4, 16
    12a9:	1128                                    	l32i.n	a2, a1, 4
    12ab:	230720bb1be5542e                  	{ bltui.w15	a4, 0x100, 11f9 <sbc_reconstruction+0x3f9>; nop }
    12b3:	010546                                    	j	16cc <sbc_reconstruction+0x8cc>

000012b6 <sbc_reconstruction+0x4b6>:
    12b6:	412e16                                    	beqz	a14, 16cc <sbc_reconstruction+0x8cc>
    12b9:	061572                                    	l16ui	a7, a5, 12
    12bc:	902d50                                    	addx2	a2, a13, a5
    12bf:	56a442                                    	movi	a4, 0x456
    12c2:	060c                                    	movi.n	a6, 0
    12c4:	224a                                    	add.n	a2, a2, a4
    12c6:	58c532                                    	addi	a3, a5, 88
    12c9:	11f7f0                                    	slli	a15, a7, 1
    12cc:	2129                                    	s32i.n	a2, a1, 8
    12ce:	3179                                    	s32i.n	a7, a1, 12
    12d0:	1128                                    	l32i.n	a2, a1, 4
    12d2:	4171d0                                    	srli	a7, a13, 1
    12d5:	3148                                    	l32i.n	a4, a1, 12
    12d7:	2198                                    	l32i.n	a9, a1, 8
    12d9:	230740a40c25f2ce                  	{ blti.w15	a2, 1, 149c <sbc_reconstruction+0x69c>; nop }
    12e1:	246a                                    	add.n	a2, a4, a6
    12e3:	556d07                                    	bbci	a13, 0, 133c <sbc_reconstruction+0x53c>
    12e6:	495d                                    	ae_l16ui.n	a5, a9, 0
    12e8:	465a                                    	add.n	a4, a6, a5
    12ea:	525a                                    	add.n	a5, a2, a5
    12ec:	a0b430                                    	addx4	a11, a4, a3
    12ef:	a0a530                                    	addx4	a10, a5, a3
    12f2:	00052a4000c160be                  	{ ae_l32m.i	aed0, a11, 0; ae_l32m.i	aed1, a10, 0 }
    12fa:	0ea4c407f0e299d0c58080080da2600f 	{ ae_subsq56s	aed0, aed0, aed1; addi	a9, a9, -2; ae_addsq56s	aed2, aed0, aed1; nop }
    130a:	0ead217b0d540cd0c57c61380c01015f 	{ nop; nop; nop; ae_roundsq32f48sym	aed31, aed2 }
    131a:	0ead217b0d250cd0c400613b0c0168ff 	{ ae_trunca32q48	a8, aed31; nop; nop; ae_roundsq32f48sym	aed0, aed0 }
    132a:	000003f3038300fe56a3030024601b8f 	{ s32i	a8, a11, 0; ae_trunca32q48	a12, aed0; nop }
    133a:	0ac9                                    	s32i.n	a12, a10, 0
    133c:	fec982                                    	addi	a8, a9, -2
    133f:	159716                                    	beqz	a7, 149c <sbc_reconstruction+0x69c>
    1342:	494d                                    	ae_l16ui.n	a4, a9, 0
    1344:	c70b                                    	addi.n	a12, a7, -1
    1346:	485d                                    	ae_l16ui.n	a5, a8, 0
    1348:	fec892                                    	addi	a9, a8, -2
    134b:	a64a                                    	add.n	a10, a6, a4
    134d:	424a                                    	add.n	a4, a2, a4
    134f:	a08430                                    	addx4	a8, a4, a3
    1352:	a07a30                                    	addx4	a7, a10, a3
    1355:	0005284000c1607e                  	{ ae_l32m.i	aed0, a7, 0; ae_l32m.i	aed1, a8, 0 }
    135d:	0ea560262d2325f8c108200f0900b65f 	{ add	a11, a6, a5; add	a5, a2, a5; ae_subsq56s	aed0, aed0, aed1; ae_addsq56s	aed2, aed0, aed1 }
    136d:	0ead61665d35b3d0c504613f1001466f 	{ or	a4, a6, a6; addx4	a11, a11, a3; nop; ae_roundsq32f48sym	aed1, aed2 }
    137d:	a0a530                                    	addx4	a10, a5, a3
    1380:	086d                                    	mov.n	a6, a8
    1382:	230720c318e5747e                  	{ nop; nop }
    138a:	0020f0                                    	nop
    138d:	9f9c76                                    	loopnez	a12, 1430 <sbc_reconstruction+0x630>
    1390:	0eace160052698d0c400613b0c01651f 	{ ae_trunca32q48	a5, aed1; l16ui	a8, a9, 0; nop; ae_roundsq32f48sym	aed0, aed0 }
    13a0:	000003f3038300fad6a303002460175f 	{ s32i	a5, a7, 0; ae_trunca32q48	a5, aed0; nop }
    13b0:	2502a9c0009f165e                  	{ s32i	a5, a6, 0; addi	a5, a9, -2 }
    13b8:	00052a4000c160be                  	{ ae_l32m.i	aed0, a11, 0; ae_l32m.i	aed1, a10, 0 }
    13c0:	0ea560063d232890c500200f09a1648f 	{ add	a6, a4, a8; add	a7, a2, a8; ae_addsq56s	aed1, aed0, aed1; ae_subsq56s	aed0, aed0, aed1 }
    13d0:	0ead6166352c73d0c404613a0401763f 	{ addx4	a7, a6, a3; addx4	a6, a7, a3; nop; ae_roundsq32f48sym	aed1, aed1 }
    13e0:	0eace160052658d0c400613b0c01691f 	{ ae_trunca32q48	a9, aed1; l16ui	a8, a5, 0; nop; ae_roundsq32f48sym	aed0, aed0 }
    13f0:	000003f3038300fcd6a3030024601b9f 	{ s32i	a9, a11, 0; ae_trunca32q48	a9, aed0; nop }
    1400:	2902a5c0009f1a9e                  	{ s32i	a9, a10, 0; addi	a9, a5, -2 }
    1408:	0005264000c1607e                  	{ ae_l32m.i	aed0, a7, 0; ae_l32m.i	aed1, a6, 0 }
    1410:	0ea5600645232890c500200f09a1548f 	{ add	a5, a4, a8; add	a8, a2, a8; ae_addsq56s	aed1, aed0, aed1; ae_subsq56s	aed0, aed0, aed1 }
    1420:	0ead6166552c83d0c404613a0401b53f 	{ addx4	a11, a5, a3; addx4	a10, a8, a3; nop; ae_roundsq32f48sym	aed1, aed1 }

00001430 <sbc_reconstruction+0x630>:
    1430:	0ead217b0d240cd0c400613b0c01621f 	{ ae_trunca32q48	a2, aed1; nop; nop; ae_roundsq32f48sym	aed0, aed0 }
    1440:	000003f3038300fcd6a303002460172f 	{ s32i	a2, a7, 0; ae_trunca32q48	a9, aed0; nop }
    1450:	0699                                    	s32i.n	a9, a6, 0
    1452:	00052a4708de60be                  	{ ae_l32m.i	aed29, a11, 0; ae_l32m.i	aed30, a10, 0 }
    145a:	0ea54427b0e744d0c4809df71100c2df 	{ ae_addsq56s	aed2, aed29, aed30; or	a6, a4, a4; ae_subsq56s	aed0, aed29, aed30; nop }
    146a:	0ead217b0d540cd0c57c61380c01015f 	{ nop; nop; nop; ae_roundsq32f48sym	aed31, aed2 }
    147a:	0ead217b0d250cd0c400613b0c016cff 	{ ae_trunca32q48	a12, aed31; nop; nop; ae_roundsq32f48sym	aed0, aed0 }
    148a:	000003f3038300fad6a3030024601bcf 	{ s32i	a12, a11, 0; ae_trunca32q48	a5, aed0; nop }
    149a:	0a59                                    	s32i.n	a5, a10, 0

0000149c <sbc_reconstruction+0x69c>:
    149c:	66fa                                    	add.n	a6, a6, a15
    149e:	2307009b0fc576ee                  	{ bltu.w15	a6, a14, 12d0 <sbc_reconstruction+0x4d0>; nop }
    14a6:	008886                                    	j	16cc <sbc_reconstruction+0x8cc>

000014a9 <sbc_reconstruction+0x6a9>:
    14a9:	21fd16                                    	beqz	a13, 16cc <sbc_reconstruction+0x8cc>
    14ac:	0615e2                                    	l16ui	a14, a5, 12
    14af:	902c50                                    	addx2	a2, a12, a5
    14b2:	56a432                                    	movi	a3, 0x456
    14b5:	060c                                    	movi.n	a6, 0
    14b7:	f23a                                    	add.n	a15, a2, a3
    14b9:	58c542                                    	addi	a4, a5, 88
    14bc:	1128                                    	l32i.n	a2, a1, 4
    14be:	11bef0                                    	slli	a11, a14, 1
    14c1:	fecf72                                    	addi	a7, a15, -2
    14c4:	220760a00c0552ce                  	{ blti.w15	a2, 1, 15b0 <sbc_reconstruction+0x7b0>; nop }
    14cc:	4f3d                                    	ae_l16ui.n	a3, a15, 0
    14ce:	2e6a                                    	add.n	a2, a14, a6
    14d0:	230720c318e5747e                  	{ nop; nop }
    14d8:	808630                                    	add	a8, a6, a3
    14db:	803230                                    	add	a3, a2, a3
    14de:	a09840                                    	addx4	a9, a8, a4
    14e1:	a05340                                    	addx4	a5, a3, a4
    14e4:	0005254000c1609e                  	{ ae_l32m.i	aed0, a9, 0; ae_l32m.i	aed1, a5, 0 }
    14ec:	0ea4e427f462cad0c50080090500073f 	{ l16ui	a3, a7, 0; addi	a10, a12, -2; ae_subsq56s	aed0, aed0, aed1; nop }
    14fc:	0ead217b0c650cd0c400613a1401f77f 	{ addi	a7, a7, -2; nop; nop; ae_roundsq32f48sym	aed0, aed0 }
    150c:	220740a410057c2e                  	{ bltui.w15	a12, 2, 159d <sbc_reconstruction+0x79d>; nop }
    1514:	808630                                    	add	a8, a6, a3
    1517:	803230                                    	add	a3, a2, a3
    151a:	a09840                                    	addx4	a9, a8, a4
    151d:	3f9a76                                    	loopnez	a10, 1560 <sbc_reconstruction+0x760>
    1520:	0eace56005267ad0c400813b0c01680f 	{ ae_trunca32q48	a8, aed0; l16ui	a10, a7, 0; nop; nop }
    1530:	1506a3400099058e                  	{ s32i	a8, a5, 0; addx4	a5, a3, a4 }
    1538:	0005254008c0609e                  	{ ae_l32m.i	aed1, a9, 0; ae_l32m.i	aed0, a5, 0 }
    1540:	0ea564261d232ad0c5008107090086af 	{ add	a8, a6, a10; add	a3, a2, a10; ae_subsq56s	aed0, aed1, aed0; nop }
    1550:	0eace167f52277d0c400613a0401984f 	{ addx4	a9, a8, a4; addi	a7, a7, -2; nop; ae_roundsq32f48sym	aed0, aed0 }

00001560 <sbc_reconstruction+0x760>:
    1560:	0ead257b0d240cd0c400813b0c01620f 	{ ae_trunca32q48	a2, aed0; nop; nop; nop }
    1570:	0529                                    	s32i.n	a2, a5, 0
    1572:	a05340                                    	addx4	a5, a3, a4
    1575:	0005254718c1609e                  	{ ae_l32m.i	aed31, a9, 0; ae_l32m.i	aed1, a5, 0 }
    157d:	0ead057b08e50cd0c58081380c0160ff 	{ ae_subsq56s	aed0, aed31, aed1; nop; nop; nop }
    158d:	0ead217b0d440cd0c50061380c01015f 	{ nop; nop; nop; ae_roundsq32f48sym	aed0, aed0 }

0000159d <sbc_reconstruction+0x79d>:
    159d:	0ead257b0d240cd0c400813b0c01620f 	{ ae_trunca32q48	a2, aed0; nop; nop; nop }
    15ad:	006522                                    	s32i	a2, a5, 0
    15b0:	66ba                                    	add.n	a6, a6, a11
    15b2:	1128                                    	l32i.n	a2, a1, 4
    15b4:	2207009f0be556de                  	{ bltu.w15	a6, a13, 14c1 <sbc_reconstruction+0x6c1>; nop }
    15bc:	004306                                    	j	16cc <sbc_reconstruction+0x8cc>

000015bf <sbc_reconstruction+0x7bf>:
    15bf:	00000000                                 ......

000015c5 <sbc_reconstruction+0x7c5>:
    15c5:	103d16                                    	beqz	a13, 16cc <sbc_reconstruction+0x8cc>
    15c8:	0615e2                                    	l16ui	a14, a5, 12
    15cb:	902c50                                    	addx2	a2, a12, a5
    15ce:	56a432                                    	movi	a3, 0x456
    15d1:	060c                                    	movi.n	a6, 0
    15d3:	f23a                                    	add.n	a15, a2, a3
    15d5:	58c542                                    	addi	a4, a5, 88
    15d8:	1128                                    	l32i.n	a2, a1, 4
    15da:	11bef0                                    	slli	a11, a14, 1
    15dd:	fecf72                                    	addi	a7, a15, -2
    15e0:	220760a00805f2ce                  	{ blti.w15	a2, 1, 16c0 <sbc_reconstruction+0x8c0>; nop }
    15e8:	4f3d                                    	ae_l16ui.n	a3, a15, 0
    15ea:	2e6a                                    	add.n	a2, a14, a6
    15ec:	863a                                    	add.n	a8, a6, a3
    15ee:	923a                                    	add.n	a9, a2, a3
    15f0:	a05840                                    	addx4	a5, a8, a4
    15f3:	a09940                                    	addx4	a9, a9, a4
    15f6:	0005294000c1605e                  	{ ae_l32m.i	aed0, a5, 0; ae_l32m.i	aed1, a9, 0 }
    15fe:	0ea4e407f462cad0c500800905a0073f 	{ l16ui	a3, a7, 0; addi	a10, a12, -2; ae_addsq56s	aed0, aed0, aed1; nop }
    160e:	0ead217b0c650cd0c400613a1401f77f 	{ addi	a7, a7, -2; nop; nop; ae_roundsq32f48sym	aed0, aed0 }
    161e:	230740a410055c2e                  	{ bltui.w15	a12, 2, 16ad <sbc_reconstruction+0x8ad>; nop }
    1626:	923a                                    	add.n	a9, a2, a3
    1628:	363a                                    	add.n	a3, a6, a3
    162a:	a09940                                    	addx4	a9, a9, a4
    162d:	3f9a76                                    	loopnez	a10, 1670 <sbc_reconstruction+0x870>
    1630:	0eace56005267ad0c400813b0c01680f 	{ ae_trunca32q48	a8, aed0; l16ui	a10, a7, 0; nop; nop }
    1640:	1506a3400099058e                  	{ s32i	a8, a5, 0; addx4	a5, a3, a4 }
    1648:	0005294008c0605e                  	{ ae_l32m.i	aed1, a5, 0; ae_l32m.i	aed0, a9, 0 }
    1650:	0ea5640645232ad0c500810709a036af 	{ add	a3, a6, a10; add	a8, a2, a10; ae_addsq56s	aed0, aed1, aed0; nop }
    1660:	0eace167f52277d0c400613a0401984f 	{ addx4	a9, a8, a4; addi	a7, a7, -2; nop; ae_roundsq32f48sym	aed0, aed0 }

00001670 <sbc_reconstruction+0x870>:
    1670:	0ead257b0d240cd0c400813b0c01620f 	{ ae_trunca32q48	a2, aed0; nop; nop; nop }
    1680:	0529                                    	s32i.n	a2, a5, 0
    1682:	a05340                                    	addx4	a5, a3, a4
    1685:	0005294718c1605e                  	{ ae_l32m.i	aed31, a5, 0; ae_l32m.i	aed1, a9, 0 }
    168d:	0ead057b08e50cd0c48081380801c0ff 	{ ae_addsq56s	aed0, aed31, aed1; nop; nop; nop }
    169d:	0ead217b0d440cd0c50061380c01015f 	{ nop; nop; nop; ae_roundsq32f48sym	aed0, aed0 }

000016ad <sbc_reconstruction+0x8ad>:
    16ad:	0ead257b0d240cd0c400813b0c01620f 	{ ae_trunca32q48	a2, aed0; nop; nop; nop }
    16bd:	006522                                    	s32i	a2, a5, 0
    16c0:	66ba                                    	add.n	a6, a6, a11
    16c2:	1128                                    	l32i.n	a2, a1, 4
    16c4:	2207009f0be5b6de                  	{ bltu.w15	a6, a13, 15dd <sbc_reconstruction+0x7dd>; nop }
    16cc:	f01d                                    	retw.n

000016ce <sbc_reconstruction+0x8ce>:
	...

000016d0 <SBC_synthesis4_per_channel>:
    16d0:	010136                                    	entry	a1, 128
    16d3:	0b0c                                    	movi.n	a11, 0
    16d5:	031452                                    	l16ui	a5, a4, 6
    16d8:	622b20                                    	saltu	a2, a11, a2
    16db:	fa5a81                                    	l32r	a8, 44 (23b0 <SBC_cos_coef_4>)
    16de:	1122c0                                    	slli	a2, a2, 4
    16e1:	bd0c                                    	movi.n	a13, 11
    16e3:	d92c                                    	movi.n	a9, 45
    16e5:	599516                                    	beqz	a5, 1c82 <SBC_synthesis4_per_channel+0x5b2>
    16e8:	0bd472                                    	addmi	a7, a4, 0xb00
    16eb:	0bd462                                    	addmi	a6, a4, 0xb00
    16ee:	115970                                    	slli	a5, a9, 9
    16f1:	242a                                    	add.n	a2, a4, a2
    16f3:	fec3a2                                    	addi	a10, a3, -2
    16f6:	40c8b2                                    	addi	a11, a8, 64
    16f9:	a0a0c2                                    	movi	a12, 160
    16fc:	08a092                                    	movi	a9, 8
    16ff:	11dd70                                    	slli	a13, a13, 9
    1702:	00a032                                    	movi	a3, 0
    1705:	0f0282421cb6e55e                  	{ ae_cvtp24a16x2.ll	aed11, a5, a5; addi	a14, a2, 88 }
    170d:	156142                                    	s32i	a4, a1, 84
    1710:	146172                                    	s32i	a7, a1, 80
    1713:	116162                                    	s32i	a6, a1, 68
    1716:	1361b2                                    	s32i	a11, a1, 76
    1719:	178076                                    	loop	a0, 1734 <SBC_synthesis4_per_channel+0x64>
    171c:	030100                                    	rsr.lend	a0
    171f:	130000                                    	wsr.lbeg	a0
    1722:	fa4a01                                    	l32r	a0, 4c (1c7f <SBC_synthesis4_per_channel+0x5af>)
    1725:	0020f0                                    	nop
    1728:	130100                                    	wsr.lend	a0
    172b:	002000                                    	isync
    172e:	030200                                    	rsr.lcount	a0
    1731:	01c002                                    	addi	a0, a0, 1
    1734:	112122                                    	l32i	a2, a1, 68
    1737:	10cef2                                    	addi	a15, a14, 16
    173a:	0d0740400cbb4c0e                  	{ ae_movi	aed1, 0; ae_movi	aed27, 0 }
    1742:	1261e2                                    	s32i	a14, a1, 72
    1745:	085d                                    	mov.n	a5, a8
    1747:	1c07404000e9116e                  	{ ae_mov	aed0, aed1; ae_mov	aed9, aed1 }
    174f:	f3f7f0                                    	wur.ae_cend0	a15
    1752:	521262                                    	l16ui	a6, a2, 164
    1755:	1221f2                                    	l32i	a15, a1, 72
    1758:	1c07404400f5116e                  	{ ae_mov	aed16, aed1; ae_mov	aed21, aed1 }
    1760:	f3f6e0                                    	wur.ae_cbegin0	a14
    1763:	282742                                    	l32i	a4, a7, 160
    1766:	166132                                    	s32i	a3, a1, 88
    1769:	1c076040107a106e                  	{ extui	a2, a6, 0, 1; ae_mov	aed26, aed1 }
    1771:	11e690                                    	slli	a14, a6, 7
    1774:	1d063ec508efdb2e                  	{ movnez	a5, a11, a2; ae_truncp24a32x2	aed15, a14, a13 }
    177c:	3406b54010c8d9fe                  	{ ae_l32x2f24.xc	aed2, a15, a9; or	a3, a5, a5 }
    1784:	39052fc01ca5b93e                  	{ ae_l32x2f24.ip	aed3, a3, 8; ae_l32x2f24.xc	aed5, a15, a9 }
    178c:	0e1d62463d3d6416022b2c1509ef055f 	{ ae_l32x2f24.i	aed4, a5, 8; addx4	a7, a6, a4; ae_mulf32s.lh	aed2, aed15, aed15; ae_mulaafd32s.hh.ll	aed1, aed2, aed3 }
    179c:	0e9527e32d213dd0c400980f10a40d5f 	{ ae_l32x2f24.i	aed28, a5, 16; ae_l32x2f24.i	aed5, a3, 24; ae_mulaafd32s.hh.ll	aed1, aed5, aed4; nop }
    17ac:	140503c710e70d5e                  	{ ae_l32x2f24.i	aed30, a5, 24; ae_l32x2f24.i	aed7, a3, 32 }
    17b4:	0a02c3400c82d11e                  	{ ae_round24x2f48ssym	aed1, aed1, aed1; addi	a11, a3, 8 }
    17bc:	0702e74008ca507e                  	{ ae_s32f24.l.i	aed1, a7, 0; addi	a7, a7, 40 }
    17c4:	0e04284708c0c9fe                  	{ ae_l32x2f24.xc	aed29, a15, a9; movi	a14, 8 }
    17cc:	1c829adc3fbcf9fe                  	{ ae_l32x2f24.xc	aed31, a15, a9; nop; ae_mulaafd32s.hh.ll	aed27, aed29, aed28 }
    17d4:	0d1c                                    	movi.n	a13, 16
    17d6:	0e54f7e0091a6527858038d81ffead2f 	{ ae_slaaq56	aed2, aed2, a13; addi	a5, a6, 1; ae_mulaafd32s.hh.ll	aed27, aed31, aed30; ae_mov	aed30, aed0 }
    17e6:	230740c00c85fbbe                  	{ ae_round24x2f48ssym	aed1, aed27, aed27; nop }
    17ee:	0702e74008ca507e                  	{ ae_s32f24.l.i	aed1, a7, 0; addi	a7, a7, 40 }
    17f6:	0202a74110cad9fe                  	{ ae_l32x2f24.xc	aed6, a15, a9; addi	a3, a7, 40 }
    17fe:	1c829a0438c589fe                  	{ ae_l32x2f24.xc	aed8, a15, a9; nop; ae_mulaafd32s.hh.ll	aed0, aed6, aed5 }
    1806:	1c81ba0421078b8e                  	{ addi	a8, a11, 8; nop; ae_mulaafd32s.hh.ll	aed0, aed8, aed7 }
    180e:	0602c3400c8ac00e                  	{ ae_round24x2f48ssym	aed1, aed0, aed0; addi	a6, a3, 40 }
    1816:	000003f30a8300fbd0e303002860907f 	{ ae_s32f24.l.i	aed1, a7, 0; ae_trunca32q48	a7, aed2; nop }
    1826:	14052bc210ccc9fe                  	{ ae_l32x2f24.xc	aed10, a15, a9; ae_l32x2f24.i	aed12, a11, 32 }
    182e:	0e94e7fa7527f9d0c400984d094c5dbf 	{ ae_l32x2f24.i	aed13, a11, 40; ae_l32x2f24.xc	aed14, a15, a9; ae_mulaafd32s.hh.ll	aed9, aed10, aed12; nop }
    183e:	1c81ba4c21cd88be                  	{ addi	a11, a8, 8; nop; ae_mulaafd32s.hh.ll	aed9, aed14, aed13 }
    1846:	0802cb400c82c99e                  	{ ae_round24x2f48ssym	aed1, aed9, aed9; addi	a8, a11, 8 }
    184e:	0304614008c0503e                  	{ ae_s32f24.l.i	aed1, a3, 0; movi	a3, 1 }
    1856:	14052bc408d2c9fe                  	{ ae_l32x2f24.xc	aed17, a15, a9; ae_l32x2f24.i	aed18, a11, 32 }
    185e:	0e94e7faa527f9d0c40098870a32d1bf 	{ ae_l32x2f24.i	aed19, a11, 40; ae_l32x2f24.xc	aed20, a15, a9; ae_mulaafd32s.hh.ll	aed16, aed17, aed18; nop }
    186e:	1c81ba842293888e                  	{ addi	a8, a8, 8; nop; ae_mulaafd32s.hh.ll	aed16, aed20, aed19 }
    1876:	b4ca                                    	add.n	a11, a4, a12
    1878:	f6c592                                    	addi	a9, a5, -10
    187b:	83b420                                    	moveqz	a11, a4, a2
    187e:	0d02c8400c82e00e                  	{ ae_round24x2f48ssym	aed1, aed16, aed16; addi	a12, a8, 8 }
    1886:	939590                                    	movnez	a9, a5, a9
    1889:	0d02ec4008c2406e                  	{ ae_s32f24.l.i	aed1, a6, 0; addi	a12, a12, 8 }
    1891:	140528c510d7cefe                  	{ ae_l32x2f24.xc	aed22, a15, a14; ae_l32x2f24.i	aed23, a8, 32 }
    1899:	0e94e7facd27fed0c40098af0ad7198f 	{ ae_l32x2f24.i	aed24, a8, 40; ae_l32x2f24.xc	aed25, a15, a14; ae_mulaafd32s.hh.ll	aed21, aed22, aed23; nop }
    18a9:	1c833aaa2f38823e                  	{ xor	a8, a2, a3; nop; ae_mulaafd32s.hh.ll	aed21, aed25, aed24 }
    18b1:	28c622                                    	addi	a2, a6, 40
    18b4:	f9e561                                    	l32r	a6, 48 (2770 <SBC_proto_coef_4>)
    18b7:	0302cc400c82f55e                  	{ ae_round24x2f48ssym	aed1, aed21, aed21; addi	a3, a12, 8 }
    18bf:	176192                                    	s32i	a9, a1, 92
    18c2:	0302e24008ca402e                  	{ ae_s32f24.l.i	aed1, a2, 0; addi	a2, a2, 40 }
    18ca:	14052cc008dbcefe                  	{ ae_l32x2f24.xc	aed1, a15, a14; ae_l32x2f24.i	aed27, a12, 32 }
    18d2:	a04760                                    	addx4	a4, a7, a6
    18d5:	1c831ad32c3bc9ce                  	{ ae_l32x2f24.i	aed28, a12, 40; nop; ae_mulaafd32s.hh.ll	aed26, aed1, aed27 }
    18dd:	1406a84708dbcefe                  	{ ae_l32x2f24.xc	aed29, a15, a14; addx4	a13, a8, a4 }
    18e5:	1c833ad02fbccdde                  	{ or	a12, a13, a13; nop; ae_mulaafd32s.hh.ll	aed26, aed29, aed28 }
    18ed:	738b                                    	addi.n	a7, a3, 8
    18ef:	0402a24000ca31ce                  	{ ae_l32f24.ip	aed0, a12, 4; addi	a5, a2, 40 }
    18f7:	0902cc401481eaae                  	{ ae_round24x2f48ssym	aed2, aed26, aed26; addi	a8, a12, 4 }
    18ff:	890c                                    	movi.n	a9, 8
    1901:	0702e84010c1402e                  	{ ae_s32f24.l.i	aed2, a2, 0; addi	a6, a8, 4 }
    1909:	140527c010dfcefe                  	{ ae_l32x2f24.xc	aed2, a15, a14; ae_l32x2f24.i	aed31, a7, 32 }
    1911:	0e94e7e0210363d0c58098f11c5fb6ff 	{ ae_l32x2f24.xc	aed6, a15, a14; addi	a3, a6, 4; ae_mulaafd32s.hh.ll	aed30, aed2, aed31; nop }
    1921:	0f02834008e1157e                  	{ ae_l32x2f24.i	aed1, a7, 40; addi	a15, a3, 4 }
    1929:	0e54e3e02523f700470838f010c1c5df 	{ ae_l32f24.i	aed7, a13, 4; addi	a7, a15, 4; ae_mulaafd32s.hh.ll	aed30, aed6, aed1; ae_movi	aed1, 0 }
    1939:	0e54f7b7147ed22004043c3804c0172f 	{ addi	a2, a7, 4; ae_l32f24.i	aed2, a13, 8; ae_sel16i.n	aed6, aed0, aed7, 0; ae_mov	aed0, aed1 }
    1949:	0f02c2401c81eeee                  	{ ae_round24x2f48ssym	aed3, aed30, aed30; addi	a14, a2, 4 }
    1951:	0502ee4018c1405e                  	{ ae_s32f24.l.i	aed3, a5, 0; addi	a4, a14, 4 }
    1959:	0f02a44708c205ee                  	{ ae_l32f24.i	aed29, a14, 20; addi	a14, a4, 8 }
    1961:	0502ae4710c1054e                  	{ ae_l32f24.i	aed30, a4, 20; addi	a4, a14, 4 }
    1969:	0f02a44718c104ee                  	{ ae_l32f24.i	aed31, a14, 16; addi	a14, a4, 4 }
    1971:	0502ae4018c1044e                  	{ ae_l32f24.i	aed3, a4, 16; addi	a4, a14, 4 }
    1979:	0f02a44100c104ee                  	{ ae_l32f24.i	aed4, a14, 16; addi	a14, a4, 4 }
    1981:	0502ae4200c1044e                  	{ ae_l32f24.i	aed8, a4, 16; addi	a4, a14, 4 }
    1989:	0f02a44208c104ee                  	{ ae_l32f24.i	aed9, a14, 16; addi	a14, a4, 4 }
    1991:	0502ae4210c1044e                  	{ ae_l32f24.i	aed10, a4, 16; addi	a4, a14, 4 }
    1999:	0e94e7a025234ed0c5009c5011299def 	{ ae_l32f24.i	aed14, a14, 20; addi	a14, a4, 4; ae_sel16i.n	aed9, aed9, aed10, 0; nop }
    19a9:	0502ae4410c1054e                  	{ ae_l32f24.i	aed18, a4, 20; addi	a4, a14, 4 }
    19b1:	0f02a44108c205ee                  	{ ae_l32f24.i	aed5, a14, 20; addi	a14, a4, 8 }
    19b9:	0502ae4300c1054e                  	{ ae_l32f24.i	aed12, a4, 20; addi	a4, a14, 4 }
    19c1:	0f02a44308c104ee                  	{ ae_l32f24.i	aed13, a14, 16; addi	a14, a4, 4 }
    19c9:	0502ae4318c1044e                  	{ ae_l32f24.i	aed15, a4, 16; addi	a4, a14, 4 }
    19d1:	0f02a44408c104ee                  	{ ae_l32f24.i	aed17, a14, 16; addi	a14, a4, 4 }
    19d9:	0502ae4500c1044e                  	{ ae_l32f24.i	aed20, a4, 16; addi	a4, a14, 4 }
    19e1:	0f02a44400c104ee                  	{ ae_l32f24.i	aed16, a14, 16; addi	a14, a4, 4 }
    19e9:	0502ae4418c1044e                  	{ ae_l32f24.i	aed19, a4, 16; addi	a4, a14, 4 }
    19f1:	0f02a44508c105ee                  	{ ae_l32f24.i	aed21, a14, 20; addi	a14, a4, 4 }
    19f9:	0502ae4510c1054e                  	{ ae_l32f24.i	aed22, a4, 20; addi	a4, a14, 4 }
    1a01:	0f02a44518c205ee                  	{ ae_l32f24.i	aed23, a14, 20; addi	a14, a4, 8 }
    1a09:	0b06bb4600c9154e                  	{ ae_l32f24.i	aed24, a4, 20; or	a4, a11, a11 }
    1a11:	3504b24614bbb94e                  	{ ae_l32x2f24.ip	aed26, a4, 8; ae_l32f24.i	aed27, a2, 20 }
    1a19:	0e94e7e0452342d0c50098001346c7df 	{ ae_l32f24.i	aed7, a13, 12; addi	a2, a4, 8; ae_mulaafd32s.hh.ll	aed0, aed26, aed6; nop }
    1a29:	3404974100fc9f1e                  	{ ae_s64.i	aed4, a1, 24; ae_l32f24.i	aed28, a7, 20 }
    1a31:	0e54f3a0452327008508fcdc135c471f 	{ ae_s64.i	aed5, a1, 16; addi	a7, a2, 8; ae_sel16i.n	aed26, aed28, aed27, 0; ae_sel16i	aed2, aed2, aed7, 0 }
    1a41:	32049c4210e515be                  	{ ae_l32x2f24.i	aed10, a11, 8; ae_l32f24.i	aed5, a12, 12 }
    1a49:	35049c4708e48f1e                  	{ ae_s64.i	aed29, a1, 56; ae_l32f24.i	aed4, a12, 16 }
    1a51:	0e14e3b7f5376456012b3c2610859f1f 	{ ae_s64.i	aed30, a1, 48; ae_l32f24.i	aed30, a6, 16; ae_sel16i.n	aed4, aed5, aed4, 0; ae_mulaafd32s.hh.ll	aed0, aed10, aed2 }
    1a61:	0702874718e2871e                  	{ ae_s64.i	aed31, a1, 40; addi	a6, a7, 8 }
    1a69:	13052bc718dc048e                  	{ ae_l32f24.i	aed31, a8, 16; ae_l32x2f24.i	aed28, a11, 16 }
    1a71:	0e54d3e043e3680105ba58011b8420ef 	{ ae_sel16i	aed2, aed31, aed30, 0; addi	a8, a6, 8; ae_mulaafd32s.hh.ll	aed0, aed28, aed4; ae_sel16i	aed4, aed14, aed18, 0 }
    1a81:	0ead2507f5211fd0c40081270381914f 	{ ae_l32x2f24.i	aed18, a4, 32; ae_l64.i	aed30, a1, 56; ae_mov	aed28, aed1; nop }
    1a91:	1c831ae32e5afe1e                  	{ ae_l64.i	aed31, a1, 48; nop; ae_mulaafd32s.hh.ll	aed28, aed18, aed26 }
    1a99:	3f002f4708de04fe                  	{ ae_l32f24.i	aed29, a15, 16; ae_sel16i	aed30, aed30, aed31, 0 }
    1aa1:	0f02884018e2931e                  	{ ae_s64.i	aed3, a1, 32; addi	a15, a8, 8 }
    1aa9:	150522c210df043e                  	{ ae_l32f24.i	aed10, a3, 16; ae_l32x2f24.i	aed31, a2, 32 }
    1ab1:	0e54f3e04523fc06842bb8e51bfe8dbf 	{ ae_l32x2f24.i	aed14, a11, 24; addi	a12, a15, 8; ae_mulaafd32s.hh.ll	aed28, aed31, aed30; ae_sel16i	aed26, aed10, aed29, 0 }
    1ac1:	0e9527e5ed211fd0c400980519c2c14f 	{ ae_l32x2f24.i	aed3, a4, 24; ae_l64.i	aed29, a1, 40; ae_mulaafd32s.hh.ll	aed0, aed14, aed2; nop }
    1ad1:	0e9527e435217dd0c4009807007a9f1f 	{ ae_l64.i	aed30, a1, 32; ae_l32x2f24.i	aed6, a7, 32; ae_mulaafd32s.hh.ll	aed0, aed3, aed26; nop }
    1ae1:	0e54f3a04523c4078435fcf5195d8f1f 	{ ae_l64.i	aed14, a1, 24; addi	a4, a12, 8; ae_sel16i.n	aed10, aed29, aed30, 0; ae_sel16i	aed30, aed13, aed15, 0 }
    1af1:	0e5517e43bf86d22048438e110ca608f 	{ ae_sel16i	aed6, aed14, aed8, 0; ae_l32x2f24.i	aed7, a6, 32; ae_mulaafd32s.hh.ll	aed28, aed6, aed10; ae_mov	aed8, aed1 }
    1b01:	0e5533e535214d03844698e700e6d9ff 	{ ae_l32x2f24.i	aed27, a15, 32; ae_l32x2f24.i	aed6, a4, 40; ae_mulaafd32s.hh.ll	aed28, aed7, aed6; ae_sel16i	aed14, aed17, aed20, 0 }
    1b11:	0e54f3e045234206c456d80f1364831f 	{ ae_l64.i	aed18, a1, 16; addi	a2, a4, 8; ae_mulaafd32s.hh.ll	aed1, aed27, aed4; ae_sel16i	aed27, aed21, aed22, 0 }
    1b21:	0e9503a4fbe0cdd0c5007c9812b040cf 	{ ae_sel16i	aed4, aed18, aed12, 0; ae_l32x2f24.i	aed31, a12, 32; ae_sel16i.n	aed21, aed16, aed19, 0; ae_roundsq32f48sym	aed0, aed0 }
    1b31:	0e54f3e045232203045f180d0be4d52f 	{ ae_l32x2f24.i	aed7, a2, 40; addi	a2, a2, 8; ae_mulaafd32s.hh.ll	aed1, aed31, aed4; ae_sel16i	aed12, aed23, aed24, 0 }
    1b41:	0e9ce797d466e5d0c4008c00040b1e5f 	{ addi	a5, a14, 4; ae_l32f24.i	aed26, a14, 20; ae_mulq32sp16s.l	aed0, aed0, aed11; nop }
    1b51:	0302824708e2052e                  	{ ae_l32x2f24.i	aed29, a2, 40; addi	a2, a2, 8 }
    1b59:	0e54e3e0253355500700184213bbdd5f 	{ ae_l32f24.i	aed31, a5, 20; addi	a5, a5, 4; ae_mulaafd32s.hh.ll	aed8, aed29, aed27; ae_slaisq56s	aed0, aed0, 2 }
    1b69:	0ead21641d214df0c200013d0001558f 	{ ae_l32x2f24.i	aed5, a8, 32; ae_l32x2f24.i	aed3, a4, 32; nop; ae_roundsp16q48x2sym	aed0, aed0, aed0 }
    1b79:	0e14ebe0452b222e382b380d087e5d2f 	{ ae_l32x2f24.i	aed13, a2, 40; addi	a2, a2, 8; ae_mulaafd32s.hh.ll	aed1, aed3, aed30; ae_mulaafd32s.hh.ll	aed28, aed5, aed9 }
    1b89:	0e14eff77d27556e112b380a10ce58ef 	{ ae_l32f24.i	aed25, a14, 16; ae_l32f24.i	aed15, a5, 20; ae_mulaafd32s.hh.ll	aed1, aed6, aed14; ae_mulaafd32s.hh.ll	aed8, aed13, aed12 }
    1b99:	0e54f3a025235500c47dfcd708b9512f 	{ ae_l32x2f24.i	aed17, a2, 40; addi	a5, a5, 4; ae_sel16i.n	aed5, aed25, aed26, 0; ae_sel16i	aed3, aed31, aed15, 0 }
    1ba9:	0e14e7e0452b228e112b380a10f5915f 	{ ae_l32f24.i	aed18, a5, 20; addi	a2, a2, 8; ae_mulaafd32s.hh.ll	aed1, aed7, aed21; ae_mulaafd32s.hh.ll	aed8, aed17, aed5 }
    1bb9:	0eacfd60252355d0c45c613f0801152f 	{ ae_l32x2f24.i	aed20, a2, 40; addi	a5, a5, 4; nop; ae_roundsq32f48sym	aed23, aed28 }
    1bc9:	0e94e3e0452b22d0c50478421283955f 	{ ae_l32f24.i	aed22, a5, 20; addi	a2, a2, 8; ae_mulaafd32s.hh.ll	aed8, aed20, aed3; ae_roundsq32f48sym	aed1, aed1 }
    1bd9:	0e14eba88d3d16b8702f7cb70852592f 	{ ae_l32x2f24.i	aed25, a2, 40; l32i	a6, a1, 68; ae_sel16i.n	aed2, aed18, aed22, 0; ae_mulq32sp16s.l	aed24, aed23, aed11 }
    1be9:	0e14ebe8bd3c1f08422f7840032213ef 	{ movi	a14, 11; l32i	a15, a1, 92; ae_mulaafd32s.hh.ll	aed8, aed25, aed2; ae_mulq32sp16s.l	aed1, aed1, aed11 }
    1bf9:	0e6ce168ad351456860301380401de7f 	{ slli	a13, a14, 9; l32i	a4, a1, 84; nop; ae_slaisq56s	aed26, aed24, 2 }
    1c09:	0eacc968a1e417d0c570613e140146ff 	{ s16i	a15, a6, 164; l32i	a7, a1, 80; nop; ae_roundsq32f48sym	aed28, aed8 }
    1c19:	0e5ce388b455165047002cf1078b01af 	{ ae_s16m.l.iu	aed0, a10, 2; l32i	a6, a1, 88; ae_mulq32sp16s.l	aed30, aed28, aed11; ae_slaisq56s	aed1, aed1, 2 }
    1c29:	0eacd97051f00cf0c36f4138140141ef 	{ l32i	a14, a1, 72; movi	a12, 160; nop; ae_roundsp16q48x2sym	aed27, aed26, aed26 }
    1c39:	0e6ce1689c551b57c703c13b0401c9af 	{ ae_s16m.l.iu	aed27, a10, 2; l32i	a11, a1, 76; nop; ae_slaisq56s	aed31, aed30, 2 }
    1c49:	0eacc16009eb6ff0c274213e1c01f28f 	{ l32r	a8, 44 (23b0 <SBC_cos_coef_4>); addi	a15, a6, 1; nop; ae_roundsp16q48x2sym	aed29, aed1, aed1 }
    1c59:	0ead3d7b0c5d0cf0c303e13b04014daf 	{ ae_s16m.l.iu	aed29, a10, 2; nop; nop; ae_roundsp16q48x2sym	aed0, aed31, aed31 }
    1c69:	230740c004c511ae                  	{ ae_s16m.l.iu	aed0, a10, 2; nop }
    1c71:	1a1432                                    	l16ui	a3, a4, 52
    1c74:	031442                                    	l16ui	a4, a4, 6
    1c77:	a0e3e0                                    	addx4	a14, a3, a14
    1c7a:	0f3d                                    	mov.n	a3, a15
    1c7c:	02bf47                                    	bgeu	a15, a4, 1c82 <SBC_synthesis4_per_channel+0x5b2>

00001c7f <SBC_synthesis4_per_channel+0x5af>:
    1c7f:	fea586                                    	j	1719 <SBC_synthesis4_per_channel+0x49>

00001c82 <SBC_synthesis4_per_channel+0x5b2>:
    1c82:	f01d                                    	retw.n

00001c84 <SBC_synthesis4_per_channel+0x5b4>:
	...

00001c90 <SBC_synthesis8_per_channel>:
    1c90:	008136                                    	entry	a1, 64
    1c93:	d52c                                    	movi.n	a5, 45
    1c95:	f8ee61                                    	l32r	a6, 50 (2470 <SBC_cos_coef_8>)
    1c98:	115570                                    	slli	a5, a5, 9
    1c9b:	031482                                    	l16ui	a8, a4, 6
    1c9e:	0bd4c2                                    	addmi	a12, a4, 0xb00
    1ca1:	0bd4d2                                    	addmi	a13, a4, 0xb00
    1ca4:	78c4a2                                    	addi	a10, a4, 120
    1ca7:	01d6e2                                    	addmi	a14, a6, 0x100
    1caa:	58c4f2                                    	addi	a15, a4, 88
    1cad:	870c                                    	movi.n	a7, 8
    1caf:	fec392                                    	addi	a9, a3, -2
    1cb2:	8b2c                                    	movi.n	a11, 40
    1cb4:	33069a410ca7e55e                  	{ ae_cvtp24a16x2.ll	aed5, a5, a5; movnez	a15, a10, a2 }
    1cbc:	0a1c                                    	movi.n	a10, 16
    1cbe:	51c9                                    	s32i.n	a12, a1, 20
    1cc0:	066142                                    	s32i	a4, a1, 24
    1cc3:	0461d2                                    	s32i	a13, a1, 16
    1cc6:	ffcad2                                    	addi	a13, a10, -1
    1cc9:	0361e2                                    	s32i	a14, a1, 12

00001ccc <SBC_synthesis8_per_channel+0x3c>:
    1ccc:	4148                                    	l32i.n	a4, a1, 16
    1cce:	7199                                    	s32i.n	a9, a1, 28
    1cd0:	20cf92                                    	addi	a9, a15, 32
    1cd3:	3168                                    	l32i.n	a6, a1, 12
    1cd5:	5138                                    	l32i.n	a3, a1, 20
    1cd7:	0f5d                                    	mov.n	a5, a15
    1cd9:	521422                                    	l16ui	a2, a4, 164
    1cdc:	40a1c2                                    	movi	a12, 0x140
    1cdf:	f3f6f0                                    	wur.ae_cbegin0	a15
    1ce2:	f3f790                                    	wur.ae_cend0	a9
    1ce5:	221b                                    	addi.n	a2, a2, 1
    1ce7:	2823e2                                    	l32i	a14, a3, 160
    1cea:	f6c2a2                                    	addi	a10, a2, -10
    1ced:	93a2a0                                    	movnez	a10, a2, a10
    1cf0:	f8d821                                    	l32r	a2, 50 (2470 <SBC_cos_coef_8>)
    1cf3:	1a02204000c2c75e                  	{ ae_l32x2f24.xc	aed0, a5, a7; extui	a9, a10, 0, 1 }
    1cfb:	360525c4108154ae                  	{ s16i	a10, a4, 164; ae_l32x2f24.xc	aed1, a5, a7 }
    1d03:	932690                                    	movnez	a2, a6, a9
    1d06:	a06ae0                                    	addx4	a6, a10, a14
    1d09:	360505c010e3112e                  	{ ae_l32x2f24.i	aed2, a2, 0; ae_l32x2f24.xc	aed3, a5, a7 }
    1d11:	2a8319232402e92e                  	{ ae_l32x2f24.i	aed30, a2, 8; nop; ae_mulzaafd32s.hh.ll	aed4, aed0, aed2 }
    1d19:	f8c232                                    	addi	a3, a2, -8
    1d1c:	0e94e7e6c52362d0c4009827103ecd2f 	{ ae_l32x2f24.i	aed31, a2, 16; addi	a2, a6, -40; ae_mulaafd32s.hh.ll	aed4, aed1, aed30; nop }
    1d2c:	1c81ba24287f826e                  	{ addi	a6, a2, 40; nop; ae_mulaafd32s.hh.ll	aed4, aed3, aed31 }
    1d34:	ceca                                    	add.n	a12, a14, a12
    1d36:	370505c000e1113e                  	{ ae_l32x2f24.i	aed0, a3, 32; ae_l32x2f24.xc	aed1, a5, a7 }
    1d3e:	20c342                                    	addi	a4, a3, 32
    1d41:	072d                                    	mov.n	a2, a7
    1d43:	0b3d                                    	mov.n	a3, a11
    1d45:	4f8d76                                    	loop	a13, 1d98 <SBC_synthesis8_per_channel+0x108>
    1d48:	1c831a232020394e                  	{ ae_l32x2f24.i	aed3, a4, 8; nop; ae_mulaafd32s.hh.ll	aed4, aed1, aed0 }
    1d50:	0ead256405214da0c208813d1801414f 	{ ae_l32x2f24.i	aed1, a4, 24; ae_l32x2f24.i	aed0, a4, 32; nop; ae_round24x2f48ssym	aed2, aed4, aed4 }
    1d60:	130564c010c2406e                  	{ ae_s32f24.l.i	aed2, a6, 0; ae_l32x2f24.i	aed2, a4, 16 }
    1d68:	220720c100c5d25e                  	{ ae_l32x2f24.xc	aed4, a5, a2; nop }
    1d70:	2a8299243883325e                  	{ ae_l32x2f24.xc	aed3, a5, a2; nop; ae_mulzaafd32s.hh.ll	aed4, aed4, aed3 }
    1d78:	0e94e7e1010344d0c58098211c62825f 	{ ae_l32x2f24.xc	aed2, a5, a2; addi	a4, a4, 32; ae_mulaafd32s.hh.ll	aed4, aed3, aed2; nop }
    1d88:	0e9567e6310363d0c58098211c41425f 	{ ae_l32x2f24.xc	aed1, a5, a2; add	a6, a6, a3; ae_mulaafd32s.hh.ll	aed4, aed2, aed1; nop }

00001d98 <SBC_synthesis8_per_channel+0x108>:
    1d98:	1c833a202420ec9e                  	{ movnez	a14, a12, a9; nop; ae_mulaafd32s.hh.ll	aed4, aed1, aed0 }
    1da0:	41a1a0                                    	srli	a10, a10, 1
    1da3:	60a1c2                                    	movi	a12, 0x160
    1da6:	027d                                    	mov.n	a7, a2
    1da8:	0c0441400480d44e                  	{ ae_round24x2f48ssym	aed0, aed4, aed4; movi	a13, 1 }
    1db0:	03bd                                    	mov.n	a11, a3
    1db2:	3d06f9c000c8506e                  	{ ae_s32f24.l.i	aed0, a6, 0; xor	a3, a9, a13 }
    1dba:	f8a661                                    	l32r	a6, 54 (2ae0 <SBC_proto_coef_8>)
    1dbd:	850c                                    	movi.n	a5, 8
    1dbf:	c12ac0                                    	mul16u	a2, a10, a12
    1dc2:	12050ec000f901ee                  	{ ae_l32x2f24.i	aed0, a14, 0; ae_l32x2f24.i	aed25, a14, 16 }
    1dca:	262a                                    	add.n	a2, a6, a2
    1dcc:	11050ec014e3332e                  	{ addx4	a2, a3, a2; ae_l32x2f24.i	aed3, a14, 8 }
    1dd4:	f8ce42                                    	addi	a4, a14, -8
    1dd7:	fcc222                                    	addi	a2, a2, -4
    1dda:	550b                                    	addi.n	a5, a5, -1
    1ddc:	24c2e2                                    	addi	a14, a2, 36
    1ddf:	7128                                    	l32i.n	a2, a1, 28
    1de1:	3d04be4610ca08ee                  	{ ae_l32f24.i	aed26, a14, -32; ae_l32f24.i	aed10, a14, -16 }
    1de9:	3d04be4118cb19ee                  	{ ae_l32f24.i	aed7, a14, -28; ae_l32f24.i	aed11, a14, -12 }
    1df1:	0e54f3b7dd27ee0285297c38103a2aef 	{ ae_l32f24.i	aed8, a14, -24; ae_l32f24.i	aed27, a14, -8; ae_sel16i.n	aed1, aed26, aed7, 0; ae_sel16i	aed10, aed10, aed11, 0 }
    1e01:	0e5cf4d7e53fef200514380810016bef 	{ ae_l32f24.i	aed9, a14, -20; ae_l32f24.i	aed28, a14, -4; ae_mulzaafd32s.hh.ll	aed1, aed0, aed1; ae_mov	aed0, aed5 }
    1e11:	0e5533a435214d07c56f9c4a11084cef 	{ ae_l32f24.i	aed29, a14, 0; ae_l32x2f24.i	aed6, a4, 32; ae_sel16i.n	aed8, aed8, aed9, 0; ae_sel16i	aed31, aed27, aed28, 0 }
    1e21:	0e5537e515394d21457c380a10688def 	{ ae_l32f24.i	aed30, a14, 4; ae_l32x2f24.i	aed2, a4, 40; ae_mulaafd32s.hh.ll	aed1, aed3, aed8; ae_mov	aed5, aed31 }
    1e31:	0e54f3e144624401c477d808072abe3f 	{ addi	a3, a14, 44; addi	a4, a4, 40; ae_mulaafd32s.hh.ll	aed1, aed25, aed10; ae_sel16i	aed7, aed29, aed30, 0 }
    1e41:	230720c318e5747e                  	{ nop; nop }
    1e49:	f03d                                    	nop.n
    1e4b:	f03d                                    	nop.n
    1e4d:	878576                                    	loop	a5, 1ed8 <SBC_synthesis8_per_channel+0x248>
    1e50:	0e94e7f725273fd0c500980810c5e03f 	{ ae_l32f24.i	aed3, a3, -32; ae_l32f24.i	aed4, a3, -4; ae_mulaafd32s.hh.ll	aed1, aed6, aed5; nop }
    1e60:	0e94e7f72d273ed0c50098081047a13f 	{ ae_l32f24.i	aed2, a3, -28; ae_l32f24.i	aed5, a3, -8; ae_mulaafd32s.hh.ll	aed1, aed2, aed7; nop }
    1e70:	0e5533a435214d0144149c150863814f 	{ ae_l32x2f24.i	aed2, a4, 8; ae_l32x2f24.i	aed6, a4, 32; ae_sel16i.n	aed3, aed3, aed2, 0; ae_sel16i	aed5, aed5, aed4, 0 }
    1e80:	0e9ce0d71d2f3dd0c51078081043b03f 	{ ae_l32f24.i	aed2, a3, -16; ae_l32f24.i	aed3, a3, -12; ae_mulzaafd32s.hh.ll	aed1, aed2, aed3; ae_roundsq32f48sym	aed4, aed1 }
    1e90:	0e14e3b725273020532f7c1810e22b3f 	{ ae_l32f24.i	aed8, a3, -20; ae_l32f24.i	aed4, a3, 0; ae_sel16i.n	aed7, aed2, aed3, 0; ae_mulq32sp16s.l	aed9, aed4, aed0 }
    1ea0:	3104b34010c31a3e                  	{ ae_l32f24.i	aed2, a3, -24; ae_l32f24.i	aed3, a3, 4 }
    1ea8:	0e54e3a16533335086013c451102494f 	{ ae_l32x2f24.i	aed9, a4, 16; addi	a3, a3, 44; ae_sel16i.n	aed8, aed2, aed8, 0; ae_slaisq56s	aed2, aed9, 2 }
    1eb8:	0e9523e515314df0c224580d1928094f 	{ ae_l32x2f24.i	aed8, a4, 24; ae_l32x2f24.i	aed2, a4, 40; ae_mulaafd32s.hh.ll	aed1, aed9, aed8; ae_roundsp16q48x2sym	aed9, aed2, aed2 }
    1ec8:	0e54f3e144434401c51078090507492f 	{ ae_s16m.l.iu	aed9, a2, 2; addi	a4, a4, 40; ae_mulaafd32s.hh.ll	aed1, aed8, aed7; ae_sel16i	aed7, aed4, aed3, 0 }
    1ed8:	1c07404018e5056e                  	{ ae_mov	aed3, aed5; ae_mov	aed5, aed0 }
    1ee0:	61e8                                    	l32i.n	a14, a1, 24
    1ee2:	029d                                    	mov.n	a9, a2
    1ee4:	1c81ba0d3cc3f88e                  	{ addi	a8, a8, -1; nop; ae_mulaafd32s.hh.ll	aed1, aed6, aed3 }
    1eec:	1c823a0a204710ae                  	{ movi	a10, 16; nop; ae_mulaafd32s.hh.ll	aed1, aed2, aed7 }
    1ef4:	0ead217b0c6d0cd0c47c613a1c01fadf 	{ addi	a13, a10, -1; nop; nop; ae_roundsq32f48sym	aed31, aed1 }
    1f04:	24834f0337e5b13e                  	{ nop; nop; ae_mulq32sp16s.l	aed0, aed31, aed5 }
    1f0c:	0ead257b09040cd0c58081380c01020f 	{ ae_slaisq56s	aed0, aed0, 2; nop; nop; nop }
    1f1c:	0ead217b0d440cf0c30001380c01015f 	{ nop; nop; nop; ae_roundsp16q48x2sym	aed0, aed0, aed0 }
    1f2c:	230740c004c5119e                  	{ ae_s16m.l.iu	aed0, a9, 2; nop }
    1f34:	1a1e22                                    	l16ui	a2, a14, 52
    1f37:	a0f2f0                                    	addx4	a15, a2, a15
    1f3a:	230740b80ba5784e                  	{ bgei.w15	a8, 1, 1ccc <SBC_synthesis8_per_channel+0x3c>; nop }
    1f42:	f01d                                    	retw.n

00001f44 <_start>:
    1f44:	004136                                    	entry	a1, 32
    1f47:	f84431                                    	l32r	a3, 58 (338c <printf_ptr>)
    1f4a:	0228                                    	l32i.n	a2, a2, 0
    1f4c:	f844a1                                    	l32r	a10, 5c (3330 <export_parameter_array+0x10>)
    1f4f:	f844b1                                    	l32r	a11, 60 (114 <sbc_init>)
    1f52:	006322                                    	s32i	a2, a3, 0
    1f55:	0002e0                                    	callx8	a2
    1f58:	002322                                    	l32i	a2, a3, 0
    1f5b:	f842a1                                    	l32r	a10, 64 (333f <export_parameter_array+0x1f>)
    1f5e:	f842b1                                    	l32r	a11, 68 (324 <sbc_frame_header_decode>)
    1f61:	0002e0                                    	callx8	a2
    1f64:	002322                                    	l32i	a2, a3, 0
    1f67:	f841a1                                    	l32r	a10, 6c (335d <export_parameter_array+0x3d>)
    1f6a:	f841b1                                    	l32r	a11, 70 (488 <sbc_frame_decode>)
    1f6d:	0002e0                                    	callx8	a2
    1f70:	002322                                    	l32i	a2, a3, 0
    1f73:	f840a1                                    	l32r	a10, 74 (3374 <export_parameter_array+0x54>)
    1f76:	f840b1                                    	l32r	a11, 78 (f0 <get_SBC_version>)
    1f79:	0002e0                                    	callx8	a2
    1f7c:	f84021                                    	l32r	a2, 7c (3320 <export_parameter_array>)
    1f7f:	f01d                                    	retw.n

00001f81 <_start+0x3d>:
    1f81:	000000                                        ...

00001f84 <__do_global_ctors_aux>:
    1f84:	004136                                    	entry	a1, 32
    1f87:	f83e31                                    	l32r	a3, 80 (22bc <__CTOR_END__>)
    1f8a:	fcc322                                    	addi	a2, a3, -4
    1f8d:	002222                                    	l32i	a2, a2, 0
    1f90:	020283a4001e920e                  	{ beqi.w15	a2, -1, 1fa5 <__do_global_ctors_aux+0x21>; addi	a3, a3, -8 }

00001f98 <__do_global_ctors_aux+0x14>:
    1f98:	0002e0                                    	callx8	a2
    1f9b:	0328                                    	l32i.n	a2, a3, 0
    1f9d:	0302e3bc07ffb26e                  	{ bnei.w15	a2, -1, 1f98 <__do_global_ctors_aux+0x14>; addi	a3, a3, -4 }
    1fa5:	f01d                                    	retw.n

00001fa7 <__do_global_ctors_aux+0x23>:
	...

00001fa8 <_fini>:
    1fa8:	008136                                    	entry	a1, 64
    1fab:	f83681                                    	l32r	a8, 84 (a0 <__do_global_dtors_aux>)
    1fae:	f03d                                    	nop.n
    1fb0:	0008e0                                    	callx8	a8

00001fb3 <_fini+0xb>:
    1fb3:	f01d                                    	retw.n
