
Build/lib/pisplit_msbc_dec_position_independent.splitload.lib:     file format elf32-xtensa-le


Disassembly of section .text:

00000000 <_init-0x90>:
       0:	000000e0 00002178 000024cc 00000000     ....x!...$......
      10:	00002498 00000000 000024b4 01503205     .$.......$...2P.
      20:	00000108 000033a0 000009a0 00000e10     .....3..........
      30:	000016e0 00001ca0 000034c0 000034a0     .........4...4..
      40:	00002540 00002590 00002950 00001c8f     @%...%..P)......
      50:	00002650 00002cc0 0000357c 00003508     P&...,..|5...5..
      60:	0000214c 0000351c 00001f80 00003500     L!...5.......5..
      70:	00000410 00010008 00780008 00000254     ..........x.T...
      80:	0000014c 0000097c 000024a0 000000a8     L...|....$......

00000090 <_init>:
      90:	008136                                    	entry	a1, 64
      93:	ffdb81                                    	l32r	a8, 0 (e0 <frame_dummy>)
      96:	f03d                                    	nop.n
      98:	0008e0                                    	callx8	a8
      9b:	ffda81                                    	l32r	a8, 4 (2178 <__do_global_ctors_aux>)
      9e:	f03d                                    	nop.n
      a0:	0008e0                                    	callx8	a8
      a3:	f01d                                    	retw.n

000000a5 <_init+0x15>:
      a5:	000000                                        ...

000000a8 <__do_global_dtors_aux>:
      a8:	004136                                    	entry	a1, 32
      ab:	ffd721                                    	l32r	a2, 8 (24cc <__do_global_dtors_aux.completed>)
      ae:	000232                                    	l8ui	a3, a2, 0
      b1:	63ec                                    	bnez.n	a3, db <__do_global_dtors_aux+0x33>

000000b3 <__do_global_dtors_aux+0xb>:
      b3:	1248                                    	l32i.n	a4, a2, 4
      b5:	0438                                    	l32i.n	a3, a4, 0
      b7:	04028424000163ae                  	{ beqz.w15	a3, c8 <__do_global_dtors_aux+0x20>; addi	a4, a4, 4 }
      bf:	1249                                    	s32i.n	a4, a2, 4
      c1:	0003e0                                    	callx8	a3
      c4:	fffac6                                    	j	b3 <__do_global_dtors_aux+0xb>

000000c7 <__do_global_dtors_aux+0x1f>:
	...

000000c8 <__do_global_dtors_aux+0x20>:
      c8:	ffd131                                    	l32r	a3, c (0 <_text_start>)
      cb:	ffd1a1                                    	l32r	a10, 10 (2498 <__FRAME_END__>)
      ce:	438c                                    	beqz.n	a3, d6 <__do_global_dtors_aux+0x2e>
      d0:	ffcf81                                    	l32r	a8, c (0 <_text_start>)
      d3:	0008e0                                    	callx8	a8
      d6:	130c                                    	movi.n	a3, 1
      d8:	004232                                    	s8i	a3, a2, 0
      db:	f01d                                    	retw.n

000000dd <__do_global_dtors_aux+0x35>:
      dd:	000000                                        ...

000000e0 <frame_dummy>:
      e0:	004136                                    	entry	a1, 32
      e3:	ffcc21                                    	l32r	a2, 14 (0 <_text_start>)
      e6:	ffcaa1                                    	l32r	a10, 10 (2498 <__FRAME_END__>)
      e9:	ffcbb1                                    	l32r	a11, 18 (24b4 <frame_dummy.object>)
      ec:	428c                                    	beqz.n	a2, f4 <frame_dummy+0x14>
      ee:	ffc981                                    	l32r	a8, 14 (0 <_text_start>)
      f1:	0008e0                                    	callx8	a8

000000f4 <frame_dummy+0x14>:
      f4:	f01d                                    	retw.n

000000f6 <frame_dummy+0x16>:
	...

00000100 <get_SBC_version>:
     100:	004136                                    	entry	a1, 32
     103:	ffc621                                    	l32r	a2, 1c (1503205 <_end+0x14ffc85>)
     106:	f01d                                    	retw.n

00000108 <memset>:
     108:	004136                                    	entry	a1, 32
     10b:	205220                                    	or	a5, a2, a2
     10e:	e28c                                    	beqz.n	a2, 120 <memset+0x18>
     110:	202550                                    	or	a2, a5, a5
     113:	948c                                    	beqz.n	a4, 120 <memset+0x18>
     115:	078476                                    	loop	a4, 120 <memset+0x18>
     118:	2202a2400080023e                  	{ s16i	a3, a2, 0; addi	a2, a2, 2 }

00000120 <memset+0x18>:
     120:	052d                                    	mov.n	a2, a5
     122:	f01d                                    	retw.n

00000124 <sbc_init>:
     124:	004136                                    	entry	a1, 32
     127:	ffbe31                                    	l32r	a3, 20 (108 <memset>)
     12a:	03a2c2                                    	movi	a12, 0x203
     12d:	11cce0                                    	slli	a12, a12, 2
     130:	0b0c                                    	movi.n	a11, 0
     132:	02ad                                    	mov.n	a10, a2
     134:	040c                                    	movi.n	a4, 0
     136:	0003e0                                    	callx8	a3
     139:	10d2f2                                    	addmi	a15, a2, 0x1000
     13c:	04d232                                    	addmi	a3, a2, 0x400
     13f:	00a252                                    	movi	a5, 0x200
     142:	085f42                                    	s16i	a4, a15, 16
     145:	4c5352                                    	s16i	a5, a3, 152
     148:	f01d                                    	retw.n

0000014a <sbc_init+0x26>:
	...

0000014c <sbc_crc8>:
     14c:	004136                                    	entry	a1, 32
     14f:	426d                                    	ae_l16ui.n	a6, a2, 0
     151:	024d                                    	mov.n	a4, a2
     153:	037d                                    	mov.n	a7, a3
     155:	f20c                                    	movi.n	a2, 15
     157:	218370                                    	srai	a8, a7, 3
     15a:	3b83a6                                    	blti	a3, 8, 199 <sbc_crc8+0x4d>
     15d:	542b                                    	addi.n	a5, a4, 2
     15f:	ffb131                                    	l32r	a3, 24 (33a0 <SBC_crc_table>)
     162:	f20c                                    	movi.n	a2, 15
     164:	064d                                    	mov.n	a4, a6
     166:	0020f0                                    	nop
     169:	f03d                                    	nop.n
     16b:	f03d                                    	nop.n
     16d:	268876                                    	loop	a8, 197 <sbc_crc8+0x4b>
     170:	0503154014e0642e                  	{ xor	a2, a4, a2; l16ui	a4, a5, 0 }
     178:	2206f1c11074002e                  	{ extui	a6, a2, 0, 1; srli	a2, a2, 1 }
     180:	2306a24410b516de                  	{ slli	a2, a6, 3; addx2	a6, a2, a3 }
     188:	330702c00084162e                  	{ l16ui	a2, a6, 0; ssr	a2 }
     190:	552b                                    	addi.n	a5, a5, 2
     192:	912020                                    	srl	a2, a2
     195:	b22d                                    	ae_zext8	a2, a2

00000197 <sbc_crc8+0x4b>:
     197:	046d                                    	mov.n	a6, a4

00000199 <sbc_crc8+0x4d>:
     199:	730c                                    	movi.n	a3, 7
     19b:	245070                                    	extui	a5, a7, 0, 3
     19e:	064d                                    	mov.n	a4, a6
     1a0:	220377                                    	bnone	a3, a7, 1c6 <sbc_crc8+0x7a>
     1a3:	d31c                                    	movi.n	a3, 29
     1a5:	1d8576                                    	loop	a5, 1c6 <sbc_crc8+0x7a>
     1a8:	305240                                    	xor	a5, a2, a4
     1ab:	015580                                    	slli	a5, a5, 24
     1ae:	130220c410b12f5e                  	{ srai	a2, a5, 31; extui	a5, a2, 0, 7 }
     1b6:	1306a2c410a515fe                  	{ slli	a2, a5, 1; and	a5, a2, a3 }
     1be:	0f06844014e5752e                  	{ xor	a2, a5, a2; slli	a4, a4, 1 }
     1c6:	f01d                                    	retw.n

000001c8 <sbc_extract_scale_factors_joint_ch4>:
     1c8:	004136                                    	entry	a1, 32
     1cb:	1a12f2                                    	l16ui	a15, a2, 52
     1ce:	20b220                                    	or	a11, a2, a2
     1d1:	70c2c2                                    	addi	a12, a2, 112
     1d4:	08a032                                    	movi	a3, 8
     1d7:	ff2c72                                    	l32i	a7, a12, 0x3fc
     1da:	38cb62                                    	addi	a6, a11, 56
     1dd:	1f0691c010f7df3e                  	{ ae_lb	a2, a3; srli	a13, a15, 1 }
     1e5:	18cb52                                    	addi	a5, a11, 24
     1e8:	05ad16                                    	beqz	a13, 246 <sbc_extract_scale_factors_joint_ch4+0x7e>
     1eb:	ffcd42                                    	addi	a4, a13, -1
     1ee:	230244400cba673e                  	{ ae_db	a7, a3; extui	a10, a2, 4, 4 }
     1f6:	349020                                    	extui	a9, a2, 0, 4
     1f9:	66ad                                    	ae_s16i.n	a10, a6, 0
     1fb:	fec522                                    	addi	a2, a5, -2
     1fe:	769d                                    	ae_s16i.n	a9, a6, 2
     200:	04c662                                    	addi	a6, a6, 4
     203:	228d                                    	ae_l16si.n	a8, a2, 0
     205:	339476                                    	loopnez	a4, 23c <sbc_extract_scale_factors_joint_ch4+0x74>
     208:	1d06894100e6cf3e                  	{ ae_lb	a4, a3; slli	a9, a9, 4 }
     210:	0a06b840008a059e                  	{ s16i	a9, a5, 0; or	a8, a8, a10 }
     218:	0202a5400080028e                  	{ s16i	a8, a2, 0; addi	a2, a5, 0 }
     220:	150240400cba673e                  	{ ae_db	a7, a3; extui	a9, a4, 0, 4 }
     228:	24022440089a069e                  	{ s16i	a9, a6, 2; extui	a10, a4, 4, 4 }
     230:	66ad                                    	ae_s16i.n	a10, a6, 0
     232:	240285400080128e                  	{ l16si	a8, a2, 0; addi	a5, a5, 2 }
     23a:	664b                                    	addi.n	a6, a6, 4

0000023c <sbc_extract_scale_factors_joint_ch4+0x74>:
     23c:	1139c0                                    	slli	a3, a9, 4
     23f:	2048a0                                    	or	a4, a8, a10
     242:	653d                                    	ae_s16i.n	a3, a5, 0
     244:	624d                                    	ae_s16i.n	a4, a2, 0

00000246 <sbc_extract_scale_factors_joint_ch4+0x7e>:
     246:	421c                                    	movi.n	a2, 20
     248:	ff6c72                                    	s32i	a7, a12, 0x3fc
     24b:	b02d20                                    	addx8	a2, a13, a2
     24e:	155b22                                    	s16i	a2, a11, 42
     251:	f01d                                    	retw.n

00000253 <sbc_extract_scale_factors_joint_ch4+0x8b>:
	...

00000254 <sbc_extract_scale_factors_joint_ch8_non>:
     254:	004136                                    	entry	a1, 32
     257:	1a12f2                                    	l16ui	a15, a2, 52
     25a:	02ad                                    	mov.n	a10, a2
     25c:	70c2c2                                    	addi	a12, a2, 112
     25f:	03bd                                    	mov.n	a11, a3
     261:	ff2c42                                    	l32i	a4, a12, 0x3fc
     264:	903ba0                                    	addx2	a3, a11, a10
     267:	41d1f0                                    	srli	a13, a15, 1
     26a:	38ca52                                    	addi	a5, a10, 56
     26d:	820c                                    	movi.n	a2, 8
     26f:	09dd16                                    	beqz	a13, 310 <sbc_extract_scale_factors_joint_ch8_non+0xbc>
     272:	4161d0                                    	srli	a6, a13, 1
     275:	12c332                                    	addi	a3, a3, 18
     278:	120701840002cd0e                  	{ bbci.w15	a13, 0, 295 <sbc_extract_scale_factors_joint_ch8_non+0x41>; ae_lb	a9, a2 }
     280:	390244400cb9742e                  	{ ae_db	a4, a2; extui	a7, a9, 4, 4 }
     288:	34e090                                    	extui	a14, a9, 0, 4
     28b:	657d                                    	ae_s16i.n	a7, a5, 0
     28d:	75ed                                    	ae_s16i.n	a14, a5, 2
     28f:	554b                                    	addi.n	a5, a5, 4
     291:	639d                                    	ae_s16i.n	a9, a3, 0
     293:	332b                                    	addi.n	a3, a3, 2

00000295 <sbc_extract_scale_factors_joint_ch8_non+0x41>:
     295:	330721a40401a6ae                  	{ beqz.w15	a6, 310 <sbc_extract_scale_factors_joint_ch8_non+0xbc>; ae_lb	a7, a2 }
     29d:	230740c00ca5742e                  	{ ae_db	a4, a2; nop }
     2a5:	130761c20062247e                  	{ extui	a8, a7, 4, 4; ae_lb	a9, a2 }
     2ad:	34f070                                    	extui	a15, a7, 0, 4
     2b0:	658d                                    	ae_s16i.n	a8, a5, 0
     2b2:	75fd                                    	ae_s16i.n	a15, a5, 2
     2b4:	f60b                                    	addi.n	a15, a6, -1
     2b6:	637d                                    	ae_s16i.n	a7, a3, 0
     2b8:	554b                                    	addi.n	a5, a5, 4
     2ba:	290244400cb9742e                  	{ ae_db	a4, a2; extui	a6, a9, 4, 4 }
     2c2:	347090                                    	extui	a7, a9, 0, 4
     2c5:	419f76                                    	loopnez	a15, 30a <sbc_extract_scale_factors_joint_ch8_non+0xb6>
     2c8:	320721c00881057e                  	{ s16i	a7, a5, 2; ae_lb	a7, a2 }
     2d0:	656d                                    	ae_s16i.n	a6, a5, 0
     2d2:	0802a5400881039e                  	{ s16i	a9, a3, 2; addi	a8, a5, 4 }
     2da:	170244400cb9742e                  	{ ae_db	a4, a2; extui	a5, a7, 4, 4 }
     2e2:	120721c00082085e                  	{ s16i	a5, a8, 0; ae_lb	a9, a2 }
     2ea:	0502e8411061307e                  	{ extui	a6, a7, 0, 4; addi	a5, a8, 4 }
     2f2:	0202a3400881186e                  	{ s16i	a6, a8, 2; addi	a3, a3, 4 }
     2fa:	380220400099137e                  	{ s16i	a7, a3, 0; extui	a7, a9, 0, 4 }
     302:	290244400cb9742e                  	{ ae_db	a4, a2; extui	a6, a9, 4, 4 }

0000030a <sbc_extract_scale_factors_joint_ch8_non+0xb6>:
     30a:	757d                                    	ae_s16i.n	a7, a5, 2
     30c:	656d                                    	ae_s16i.n	a6, a5, 0
     30e:	739d                                    	ae_s16i.n	a9, a3, 2

00000310 <sbc_extract_scale_factors_joint_ch8_non+0xbc>:
     310:	2dba                                    	add.n	a2, a13, a11
     312:	ff6c42                                    	s32i	a4, a12, 0x3fc
     315:	1122d0                                    	slli	a2, a2, 3
     318:	155a22                                    	s16i	a2, a10, 42
     31b:	f01d                                    	retw.n

0000031d <sbc_extract_scale_factors_joint_ch8_non+0xc9>:
     31d:	000000                                        ...

00000320 <sbc_get_frame_length>:
     320:	004136                                    	entry	a1, 32
     323:	04d222                                    	addmi	a2, a2, 0x400
     326:	4c1222                                    	l16ui	a2, a2, 152
     329:	f01d                                    	retw.n

0000032b <sbc_get_frame_length+0xb>:
	...

0000032c <sbc_get_sample_freq>:
     32c:	004136                                    	entry	a1, 32
     32f:	021222                                    	l16ui	a2, a2, 4
     332:	f01d                                    	retw.n

00000334 <sbc_frame_header_decode>:
     334:	004136                                    	entry	a1, 32
     337:	0cd352                                    	addmi	a5, a3, 0xc00
     33a:	040c                                    	movi.n	a4, 0
     33c:	3578                                    	l32i.n	a7, a5, 12
     33e:	624d                                    	ae_s16i.n	a4, a2, 0
     340:	9ca062                                    	movi	a6, 156
     343:	271b                                    	addi.n	a2, a7, 1
     345:	0d8076                                    	loop	a0, 356 <sbc_frame_header_decode+0x22>
     348:	024d                                    	mov.n	a4, a2
     34a:	f20b                                    	addi.n	a15, a2, -1
     34c:	3529                                    	s32i.n	a2, a5, 12
     34e:	221b                                    	addi.n	a2, a2, 1
     350:	000f72                                    	l8ui	a7, a15, 0
     353:	021767                                    	beq	a7, a6, 359 <sbc_frame_header_decode+0x25>

00000356 <sbc_frame_header_decode+0x22>:
     356:	fffac6                                    	j	345 <sbc_frame_header_decode+0x11>

00000359 <sbc_frame_header_decode+0x25>:
     359:	620b                                    	addi.n	a6, a2, -1
     35b:	3529                                    	s32i.n	a2, a5, 12
     35d:	000662                                    	l8ui	a6, a6, 0
     360:	a21b                                    	addi.n	a10, a2, 1
     362:	c80c                                    	movi.n	a8, 12
     364:	290c                                    	movi.n	a9, 2
     366:	35a9                                    	s32i.n	a10, a5, 12
     368:	170c                                    	movi.n	a7, 1
     36a:	41c260                                    	srli	a12, a6, 2
     36d:	04d160                                    	extui	a13, a6, 1, 1
     370:	04e060                                    	extui	a14, a6, 0, 1
     373:	108c80                                    	and	a8, a12, a8
     376:	0553d2                                    	s16i	a13, a3, 10
     379:	14a0c0                                    	extui	a10, a12, 0, 2
     37c:	11dee0                                    	slli	a13, a14, 2
     37f:	9379a0                                    	movnez	a7, a9, a10
     382:	884b                                    	addi.n	a8, a8, 4
     384:	9d4b                                    	addi.n	a9, a13, 4
     386:	41f660                                    	srli	a15, a6, 6
     389:	095362                                    	s16i	a6, a3, 18
     38c:	0453a2                                    	s16i	a10, a3, 8
     38f:	016640                                    	slli	a6, a6, 28
     392:	0253f2                                    	s16i	a15, a3, 4
     395:	fecaa2                                    	addi	a10, a10, -2
     398:	035382                                    	s16i	a8, a3, 6
     39b:	316e60                                    	srai	a6, a6, 30
     39e:	222b                                    	addi.n	a2, a2, 2
     3a0:	075372                                    	s16i	a7, a3, 14
     3a3:	065392                                    	s16i	a9, a3, 12
     3a6:	04d3b2                                    	addmi	a11, a3, 0x400
     3a9:	0104f2                                    	l8ui	a15, a4, 1
     3ac:	3529                                    	s32i.n	a2, a5, 12
     3ae:	d1c790                                    	mul16s	a12, a7, a9
     3b1:	d17780                                    	mul16s	a7, a7, a8
     3b4:	d1e980                                    	mul16s	a14, a9, a8
     3b7:	d1dc80                                    	mul16s	a13, a12, a8
     3ba:	d18f80                                    	mul16s	a8, a15, a8
     3bd:	d177f0                                    	mul16s	a7, a7, a15
     3c0:	998a                                    	add.n	a9, a9, a8
     3c2:	887b                                    	addi.n	a8, a8, 7
     3c4:	997b                                    	addi.n	a9, a9, 7
     3c6:	777b                                    	addi.n	a7, a7, 7
     3c8:	1853e2                                    	s16i	a14, a3, 48
     3cb:	8398a0                                    	moveqz	a9, a8, a10
     3ce:	41e1c0                                    	srli	a14, a12, 1
     3d1:	b39760                                    	movgez	a9, a7, a6
     3d4:	2e4b                                    	addi.n	a2, a14, 4
     3d6:	416390                                    	srli	a6, a9, 3
     3d9:	262a                                    	add.n	a2, a6, a2
     3db:	1a53c2                                    	s16i	a12, a3, 52
     3de:	4c5b22                                    	s16i	a2, a11, 152
     3e1:	020c                                    	movi.n	a2, 0
     3e3:	0853f2                                    	s16i	a15, a3, 16
     3e6:	0a53f2                                    	s16i	a15, a3, 20
     3e9:	1b53d2                                    	s16i	a13, a3, 54
     3ec:	0204f2                                    	l8ui	a15, a4, 2
     3ef:	1653f2                                    	s16i	a15, a3, 44
     3f2:	f01d                                    	retw.n

000003f4 <change_endian16x>:
     3f4:	004136                                    	entry	a1, 32
     3f7:	ffa032                                    	movi	a3, 255
     3fa:	114280                                    	slli	a4, a2, 8
     3fd:	113300                                    	slli	a3, a3, 16
     400:	01f280                                    	slli	a15, a2, 24
     403:	103430                                    	and	a3, a4, a3
     406:	2f3a                                    	add.n	a2, a15, a3
     408:	312020                                    	srai	a2, a2, 16
     40b:	f01d                                    	retw.n

0000040d <change_endian16x+0x19>:
     40d:	000000                                        ...

00000410 <SBC_decode_preload>:
     410:	004136                                    	entry	a1, 32
     413:	0cd2f2                                    	addmi	a15, a2, 0xc00
     416:	026d                                    	mov.n	a6, a2
     418:	032f72                                    	l32i	a7, a15, 12
     41b:	04a032                                    	movi	a3, 4
     41e:	042070                                    	extui	a2, a7, 0, 1
     421:	c09320                                    	sub	a9, a3, a2
     424:	1182d0                                    	slli	a8, a2, 3
     427:	fcc222                                    	addi	a2, a2, -4
     42a:	057916                                    	beqz	a9, 485 <SBC_decode_preload+0x75>
     42d:	604020                                    	neg	a4, a2
     430:	203770                                    	or	a3, a7, a7
     433:	00a022                                    	movi	a2, 0
     436:	415140                                    	srli	a5, a4, 1
     439:	11b280                                    	slli	a11, a2, 8
     43c:	086407                                    	bbci	a4, 0, 448 <SBC_decode_preload+0x38>
     43f:	0003a2                                    	l8ui	a10, a3, 0
     442:	01c332                                    	addi	a3, a3, 1
     445:	802ba0                                    	add	a2, a11, a10

00000448 <SBC_decode_preload+0x38>:
     448:	11c280                                    	slli	a12, a2, 8
     44b:	15bc                                    	beqz.n	a5, 480 <SBC_decode_preload+0x70>
     44d:	000342                                    	l8ui	a4, a3, 0
     450:	b50b                                    	addi.n	a11, a5, -1
     452:	0103a2                                    	l8ui	a10, a3, 1
     455:	232b                                    	addi.n	a2, a3, 2
     457:	4c4a                                    	add.n	a4, a12, a4
     459:	043d                                    	mov.n	a3, a4
     45b:	0a5d                                    	mov.n	a5, a10
     45d:	199b76                                    	loopnez	a11, 47a <SBC_decode_preload+0x6a>
     460:	090683400485023e                  	{ l8ui	a3, a2, 0; slli	a4, a3, 8 }
     468:	445a                                    	add.n	a4, a4, a5
     46a:	1503b24500a0148e                  	{ slli	a4, a4, 8; l8ui	a5, a2, 1 }
     472:	220282401ce0243e                  	{ add	a3, a4, a3; addi	a2, a2, 2 }

0000047a <SBC_decode_preload+0x6a>:
     47a:	114380                                    	slli	a4, a3, 8
     47d:	802450                                    	add	a2, a4, a5

00000480 <SBC_decode_preload+0x70>:
     480:	779a                                    	add.n	a7, a7, a9
     482:	000046                                    	j	487 <SBC_decode_preload+0x77>

00000485 <SBC_decode_preload+0x75>:
     485:	020c                                    	movi.n	a2, 0
     487:	fec7f2                                    	addi	a15, a7, -2
     48a:	70c632                                    	addi	a3, a6, 112
     48d:	67ef84                                    	wur.ae_bitptr	a8
     490:	f3f120                                    	wur.ae_bithead	a2
     493:	ff63f2                                    	s32i	a15, a3, 0x3fc
     496:	f01d                                    	retw.n

00000498 <sbc_frame_decode>:
     498:	006136                                    	entry	a1, 48
     49b:	0cd2e2                                    	addmi	a14, a2, 0xc00
     49e:	02fd                                    	mov.n	a15, a2
     4a0:	032e62                                    	l32i	a6, a14, 12
     4a3:	04a0d2                                    	movi	a13, 4
     4a6:	026142                                    	s32i	a4, a1, 8
     4a9:	036132                                    	s32i	a3, a1, 12
     4ac:	042060                                    	extui	a2, a6, 0, 1
     4af:	203660                                    	or	a3, a6, a6
     4b2:	c08d20                                    	sub	a8, a13, a2
     4b5:	1172d0                                    	slli	a7, a2, 3
     4b8:	fcc222                                    	addi	a2, a2, -4
     4bb:	05a816                                    	beqz	a8, 519 <sbc_frame_decode+0x81>
     4be:	604020                                    	neg	a4, a2
     4c1:	00a022                                    	movi	a2, 0
     4c4:	415140                                    	srli	a5, a4, 1
     4c7:	096407                                    	bbci	a4, 0, 4d4 <sbc_frame_decode+0x3c>
     4ca:	000392                                    	l8ui	a9, a3, 0
     4cd:	114280                                    	slli	a4, a2, 8
     4d0:	331b                                    	addi.n	a3, a3, 1
     4d2:	249a                                    	add.n	a2, a4, a9

000004d4 <sbc_frame_decode+0x3c>:
     4d4:	11b280                                    	slli	a11, a2, 8
     4d7:	65bc                                    	beqz.n	a5, 511 <sbc_frame_decode+0x79>
     4d9:	000342                                    	l8ui	a4, a3, 0
     4dc:	a50b                                    	addi.n	a10, a5, -1
     4de:	010392                                    	l8ui	a9, a3, 1
     4e1:	02c322                                    	addi	a2, a3, 2
     4e4:	804b40                                    	add	a4, a11, a4
     4e7:	205990                                    	or	a5, a9, a9
     4ea:	203440                                    	or	a3, a4, a4
     4ed:	199a76                                    	loopnez	a10, 50a <sbc_frame_decode+0x72>
     4f0:	090683400485023e                  	{ l8ui	a3, a2, 0; slli	a4, a3, 8 }
     4f8:	445a                                    	add.n	a4, a4, a5
     4fa:	1503b24500a0148e                  	{ slli	a4, a4, 8; l8ui	a5, a2, 1 }
     502:	220282401ce0243e                  	{ add	a3, a4, a3; addi	a2, a2, 2 }

0000050a <sbc_frame_decode+0x72>:
     50a:	114380                                    	slli	a4, a3, 8
     50d:	059d                                    	mov.n	a9, a5
     50f:	245a                                    	add.n	a2, a4, a5

00000511 <sbc_frame_decode+0x79>:
     511:	668a                                    	add.n	a6, a6, a8
     513:	000106                                    	j	51b <sbc_frame_decode+0x83>

00000516 <sbc_frame_decode+0x7e>:
     516:	000000                                        ...

00000519 <sbc_frame_decode+0x81>:
     519:	020c                                    	movi.n	a2, 0
     51b:	041f32                                    	l16ui	a3, a15, 8
     51e:	fec652                                    	addi	a5, a6, -2
     521:	70cfa2                                    	addi	a10, a15, 112
     524:	040c                                    	movi.n	a4, 0
     526:	67ef74                                    	wur.ae_bitptr	a7
     529:	f3f120                                    	wur.ae_bithead	a2
     52c:	061f22                                    	l16ui	a2, a15, 12
     52f:	ff6a52                                    	s32i	a5, a10, 0x3fc
     532:	061f82                                    	l16ui	a8, a15, 12
     535:	6d3366                                    	bnei	a3, 3, 5a6 <sbc_frame_decode+0x10e>
     538:	220701c71c80ea5e                  	{ l32i	a5, a10, 0x3fc; ae_lb	a2, a2 }
     540:	061fd2                                    	l16ui	a13, a15, 12
     543:	175f22                                    	s16i	a2, a15, 46
     546:	171f42                                    	l16ui	a4, a15, 46
     549:	fcc832                                    	addi	a3, a8, -4
     54c:	27035f400ca165de                  	{ ae_db	a5, a13; l16ui	a6, a15, 12 }
     554:	58a472                                    	movi	a7, 0x458
     557:	ff6a52                                    	s32i	a5, a10, 0x3fc
     55a:	34e040                                    	extui	a14, a4, 0, 4
     55d:	fb7c                                    	movi.n	a11, -1
     55f:	112ec0                                    	slli	a2, a14, 4
     562:	960b                                    	addi.n	a9, a6, -1
     564:	932430                                    	movnez	a2, a4, a3
     567:	3f7a                                    	add.n	a3, a15, a7
     569:	0b5f22                                    	s16i	a2, a15, 22
     56c:	6ccf22                                    	addi	a2, a15, 108
     56f:	180c                                    	movi.n	a8, 1
     571:	44bc                                    	beqz.n	a4, 5a9 <sbc_frame_decode+0x111>
     573:	ff6232                                    	s32i	a3, a2, 0x3fc
     576:	f470b0                                    	extui	a7, a11, 0, 16
     579:	c6ac                                    	beqz.n	a6, 5a9 <sbc_frame_decode+0x111>
     57b:	401900                                    	ssl	a9
     57e:	a19800                                    	sll	a9, a8
     581:	1c8076                                    	loop	a0, 5a1 <sbc_frame_decode+0x109>
     584:	104490                                    	and	a4, a4, a9
     587:	b80b                                    	addi.n	a11, a8, -1
     589:	090747                                    	bnone	a7, a4, 596 <sbc_frame_decode+0xfe>
     58c:	63bd                                    	ae_s16i.n	a11, a3, 0
     58e:	332b                                    	addi.n	a3, a3, 2
     590:	061f62                                    	l16ui	a6, a15, 12
     593:	ff6232                                    	s32i	a3, a2, 0x3fc
     596:	419190                                    	srli	a9, a9, 1
     599:	0cb867                                    	bgeu	a8, a6, 5a9 <sbc_frame_decode+0x111>
     59c:	171f42                                    	l16ui	a4, a15, 46
     59f:	881b                                    	addi.n	a8, a8, 1

000005a1 <sbc_frame_decode+0x109>:
     5a1:	fff706                                    	j	581 <sbc_frame_decode+0xe9>

000005a4 <sbc_frame_decode+0x10c>:
	...

000005a6 <sbc_frame_decode+0x10e>:
     5a6:	175f42                                    	s16i	a4, a15, 46
     5a9:	041f22                                    	l16ui	a2, a15, 8
     5ac:	1a1f32                                    	l16ui	a3, a15, 52
     5af:	1a1f42                                    	l16ui	a4, a15, 52
     5b2:	38cf62                                    	addi	a6, a15, 56
     5b5:	061f72                                    	l16ui	a7, a15, 12
     5b8:	0c1c                                    	movi.n	a12, 16
     5ba:	230720a01c05f26e                  	{ bnei.w15	a2, 3, 63c <sbc_frame_decode+0x1a4>; nop }
     5c2:	41b140                                    	srli	a11, a4, 1
     5c5:	8c1c                                    	movi.n	a12, 24
     5c7:	18cf32                                    	addi	a3, a15, 24
     5ca:	230700a50045f76e                  	{ bnei.w15	a7, 4, 7ed <sbc_frame_decode+0x355>; nop }
     5d2:	4c1c                                    	movi.n	a12, 20
     5d4:	820c                                    	movi.n	a2, 8
     5d6:	38cf42                                    	addi	a4, a15, 56
     5d9:	107b16                                    	beqz	a11, 6e4 <sbc_frame_decode+0x24c>
     5dc:	330741c00ca0652e                  	{ ae_db	a5, a2; ae_lb	a3, a2 }
     5e4:	9b0b                                    	addi.n	a9, a11, -1
     5e6:	348430                                    	extui	a8, a3, 4, 4
     5e9:	346030                                    	extui	a6, a3, 0, 4
     5ec:	16cf32                                    	addi	a3, a15, 22
     5ef:	648d                                    	ae_s16i.n	a8, a4, 0
     5f1:	746d                                    	ae_s16i.n	a6, a4, 2
     5f3:	237d                                    	ae_l16si.n	a7, a3, 0
     5f5:	319976                                    	loopnez	a9, 62a <sbc_frame_decode+0x192>
     5f8:	2d06864208e5cf2e                  	{ ae_lb	a9, a2; slli	a6, a6, 4 }
     600:	2806b7400889036e                  	{ s16i	a6, a3, 2; or	a6, a7, a8 }
     608:	0402a4400081036e                  	{ s16i	a6, a3, 0; addi	a4, a4, 4 }
     610:	090244400cba752e                  	{ ae_db	a5, a2; extui	a8, a9, 4, 4 }
     618:	280220400099148e                  	{ s16i	a8, a4, 0; extui	a6, a9, 0, 4 }
     620:	746d                                    	ae_s16i.n	a6, a4, 2
     622:	220283400880137e                  	{ l16si	a7, a3, 2; addi	a3, a3, 2 }

0000062a <sbc_frame_decode+0x192>:
     62a:	1126c0                                    	slli	a2, a6, 4
     62d:	20d780                                    	or	a13, a7, a8
     630:	732d                                    	ae_s16i.n	a2, a3, 2
     632:	e32b                                    	addi.n	a14, a3, 2
     634:	63dd                                    	ae_s16i.n	a13, a3, 0
     636:	002a86                                    	j	6e4 <sbc_frame_decode+0x24c>

00000639 <sbc_frame_decode+0x1a1>:
     639:	000000                                        ...

0000063c <sbc_frame_decode+0x1a4>:
     63c:	41b130                                    	srli	a11, a3, 1
     63f:	16cf32                                    	addi	a3, a15, 22
     642:	820c                                    	movi.n	a2, 8
     644:	09cb16                                    	beqz	a11, 6e4 <sbc_frame_decode+0x24c>
     647:	4141b0                                    	srli	a4, a11, 1
     64a:	176b07                                    	bbci	a11, 0, 665 <sbc_frame_decode+0x1cd>
     64d:	130741c00ca2652e                  	{ ae_db	a5, a2; ae_lb	a9, a2 }
     655:	347490                                    	extui	a7, a9, 4, 4
     658:	34d090                                    	extui	a13, a9, 0, 4
     65b:	667d                                    	ae_s16i.n	a7, a6, 0
     65d:	76dd                                    	ae_s16i.n	a13, a6, 2
     65f:	664b                                    	addi.n	a6, a6, 4
     661:	639d                                    	ae_s16i.n	a9, a3, 0
     663:	332b                                    	addi.n	a3, a3, 2
     665:	330721a40401c4ae                  	{ beqz.w15	a4, 6e4 <sbc_frame_decode+0x24c>; ae_lb	a7, a2 }
     66d:	0d02c6400ca1752e                  	{ ae_db	a5, a2; addi	a13, a6, 4 }
     675:	130761c20062247e                  	{ extui	a8, a7, 4, 4; ae_lb	a9, a2 }
     67d:	34e070                                    	extui	a14, a7, 0, 4
     680:	668d                                    	ae_s16i.n	a8, a6, 0
     682:	76ed                                    	ae_s16i.n	a14, a6, 2
     684:	e40b                                    	addi.n	a14, a4, -1
     686:	637d                                    	ae_s16i.n	a7, a3, 0
     688:	346490                                    	extui	a6, a9, 4, 4
     68b:	390240400cb9752e                  	{ ae_db	a5, a2; extui	a7, a9, 0, 4 }
     693:	0d4d                                    	mov.n	a4, a13
     695:	419e76                                    	loopnez	a14, 6da <sbc_frame_decode+0x242>
     698:	320721c00881047e                  	{ s16i	a7, a4, 2; ae_lb	a7, a2 }
     6a0:	646d                                    	ae_s16i.n	a6, a4, 0
     6a2:	0802a4400881039e                  	{ s16i	a9, a3, 2; addi	a8, a4, 4 }
     6aa:	070244400cb9752e                  	{ ae_db	a5, a2; extui	a4, a7, 4, 4 }
     6b2:	120721c00082084e                  	{ s16i	a4, a8, 0; ae_lb	a9, a2 }
     6ba:	0502e8411061207e                  	{ extui	a6, a7, 0, 4; addi	a4, a8, 4 }
     6c2:	0202a3400881186e                  	{ s16i	a6, a8, 2; addi	a3, a3, 4 }
     6ca:	380220400099137e                  	{ s16i	a7, a3, 0; extui	a7, a9, 0, 4 }
     6d2:	290244400cb9752e                  	{ ae_db	a5, a2; extui	a6, a9, 4, 4 }

000006da <sbc_frame_decode+0x242>:
     6da:	747d                                    	ae_s16i.n	a7, a4, 2
     6dc:	d44b                                    	addi.n	a13, a4, 4
     6de:	e34b                                    	addi.n	a14, a3, 4
     6e0:	646d                                    	ae_s16i.n	a6, a4, 0
     6e2:	739d                                    	ae_s16i.n	a9, a3, 2

000006e4 <sbc_frame_decode+0x24c>:
     6e4:	b02bc0                                    	addx8	a2, a11, a12
     6e7:	091f32                                    	l16ui	a3, a15, 18
     6ea:	927d                                    	ae_zext16	a7, a2
     6ec:	ff6a52                                    	s32i	a5, a10, 0x3fc
     6ef:	f40c                                    	movi.n	a4, 15
     6f1:	155f22                                    	s16i	a2, a15, 42
     6f4:	416370                                    	srli	a6, a7, 3
     6f7:	14cf52                                    	addi	a5, a15, 20
     6fa:	1d87f6                                    	bgeui	a7, 8, 71b <sbc_frame_decode+0x283>
     6fd:	4ce727                                    	bbsi	a7, 2, 74d <sbc_frame_decode+0x2b5>
     700:	161f22                                    	l16ui	a2, a15, 44
     703:	759427                                    	bne	a4, a2, 77c <sbc_frame_decode+0x2e4>
     706:	fe4821                                    	l32r	a2, 28 (9a0 <sbc_bit_allocation>)
     709:	0fad                                    	mov.n	a10, a15
     70b:	0f4d                                    	mov.n	a4, a15
     70d:	0002e0                                    	callx8	a2
     710:	fe4721                                    	l32r	a2, 2c (e10 <sbc_reconstruction>)
     713:	04ad                                    	mov.n	a10, a4
     715:	0002e0                                    	callx8	a2
     718:	001c06                                    	j	78c <sbc_frame_decode+0x2f4>

0000071b <sbc_frame_decode+0x283>:
     71b:	f40c                                    	movi.n	a4, 15
     71d:	fe4121                                    	l32r	a2, 24 (33a0 <SBC_crc_table>)
     720:	268676                                    	loop	a6, 74a <sbc_frame_decode+0x2b2>
     723:	0303154104e0743e                  	{ xor	a4, a4, a3; l16ui	a3, a5, 0 }
     72b:	0406f1c11075004e                  	{ extui	a6, a4, 0, 1; srli	a4, a4, 1 }
     733:	2306a44500b506de                  	{ slli	a4, a6, 3; addx2	a6, a4, a2 }
     73b:	330704c00084164e                  	{ l16ui	a4, a6, 0; ssr	a4 }
     743:	552b                                    	addi.n	a5, a5, 2
     745:	914040                                    	srl	a4, a4
     748:	b44d                                    	ae_zext8	a4, a4

0000074a <sbc_frame_decode+0x2b2>:
     74a:	b26727                                    	bbci	a7, 2, 700 <sbc_frame_decode+0x268>

0000074d <sbc_frame_decode+0x2b5>:
     74d:	420c                                    	movi.n	a2, 4
     74f:	105720                                    	and	a5, a7, a2
     752:	1da022                                    	movi	a2, 29
     755:	1d8576                                    	loop	a5, 776 <sbc_frame_decode+0x2de>
     758:	305340                                    	xor	a5, a3, a4
     75b:	015580                                    	slli	a5, a5, 24
     75e:	150220c500b12f5e                  	{ srai	a4, a5, 31; extui	a5, a4, 0, 7 }
     766:	1306a4c500a505fe                  	{ slli	a4, a5, 1; and	a5, a4, a2 }
     76e:	3f06834104e4754e                  	{ xor	a4, a5, a4; slli	a3, a3, 1 }

00000776 <sbc_frame_decode+0x2de>:
     776:	161f22                                    	l16ui	a2, a15, 44
     779:	891427                                    	beq	a4, a2, 706 <sbc_frame_decode+0x26e>

0000077c <sbc_frame_decode+0x2e4>:
     77c:	fe2921                                    	l32r	a2, 20 (108 <memset>)
     77f:	58cfa2                                    	addi	a10, a15, 88
     782:	0b0c                                    	movi.n	a11, 0
     784:	00a4c2                                    	movi	a12, 0x400
     787:	0f4d                                    	mov.n	a4, a15
     789:	0002e0                                    	callx8	a2
     78c:	071422                                    	l16ui	a2, a4, 14
     78f:	030c                                    	movi.n	a3, 0
     791:	1b14f2                                    	l16ui	a15, a4, 54
     794:	10d4e2                                    	addmi	a14, a4, 0x1000
     797:	04cd                                    	mov.n	a12, a4
     799:	a0a442                                    	movi	a4, 0x4a0
     79c:	220b                                    	addi.n	a2, a2, -1
     79e:	061c52                                    	l16ui	a5, a12, 12
     7a1:	622320                                    	saltu	a2, a3, a2
     7a4:	081e32                                    	l16ui	a3, a14, 16
     7a7:	6c4a                                    	add.n	a6, a12, a4
     7a9:	400200                                    	ssr	a2
     7ac:	0bdc42                                    	addmi	a4, a12, 0xb00
     7af:	9120f0                                    	srl	a2, a15
     7b2:	20a772                                    	movi	a7, 0x720
     7b5:	0d4316                                    	beqz	a3, 88d <sbc_frame_decode+0x3f5>
     7b8:	061c82                                    	l16ui	a8, a12, 12
     7bb:	220760a41405930e                  	{ beqi.w15	a3, 2, 8b0 <sbc_frame_decode+0x418>; nop }
     7c3:	230740a40c25736e                  	{ bnei.w15	a3, 1, 976 <sbc_frame_decode+0x4de>; nop }
     7cb:	0bdc32                                    	addmi	a3, a12, 0xb00
     7ce:	fe1871                                    	l32r	a7, 30 (16e0 <SBC_synthesis4_per_channel>)
     7d1:	5393f2                                    	l16si	a15, a3, 166
     7d4:	286362                                    	s32i	a6, a3, 160
     7d7:	5253f2                                    	s16i	a15, a3, 164
     7da:	220760a10405756e                  	{ bnei.w15	a5, 4, 8ca <sbc_frame_decode+0x432>; nop }
     7e2:	075d                                    	mov.n	a5, a7
     7e4:	003946                                    	j	8cd <sbc_frame_decode+0x435>

000007e7 <sbc_frame_decode+0x34f>:
     7e7:	00000000                                 ......

000007ed <sbc_frame_decode+0x355>:
     7ed:	38cf62                                    	addi	a6, a15, 56
     7f0:	ef0b16                                    	beqz	a11, 6e4 <sbc_frame_decode+0x24c>
     7f3:	820c                                    	movi.n	a2, 8
     7f5:	4141b0                                    	srli	a4, a11, 1
     7f8:	120701840002cb0e                  	{ bbci.w15	a11, 0, 815 <sbc_frame_decode+0x37d>; ae_lb	a9, a2 }
     800:	390244400cb9752e                  	{ ae_db	a5, a2; extui	a7, a9, 4, 4 }
     808:	34d090                                    	extui	a13, a9, 0, 4
     80b:	667d                                    	ae_s16i.n	a7, a6, 0
     80d:	76dd                                    	ae_s16i.n	a13, a6, 2
     80f:	664b                                    	addi.n	a6, a6, 4
     811:	639d                                    	ae_s16i.n	a9, a3, 0
     813:	332b                                    	addi.n	a3, a3, 2
     815:	330761bc03c144ae                  	{ beqz.w15	a4, 6e4 <sbc_frame_decode+0x24c>; ae_lb	a7, a2 }
     81d:	0d02c6400ca1752e                  	{ ae_db	a5, a2; addi	a13, a6, 4 }
     825:	130761c20062247e                  	{ extui	a8, a7, 4, 4; ae_lb	a9, a2 }
     82d:	34e070                                    	extui	a14, a7, 0, 4
     830:	668d                                    	ae_s16i.n	a8, a6, 0
     832:	76ed                                    	ae_s16i.n	a14, a6, 2
     834:	e40b                                    	addi.n	a14, a4, -1
     836:	637d                                    	ae_s16i.n	a7, a3, 0
     838:	346490                                    	extui	a6, a9, 4, 4
     83b:	390240400cb9752e                  	{ ae_db	a5, a2; extui	a7, a9, 0, 4 }
     843:	0d4d                                    	mov.n	a4, a13
     845:	419e76                                    	loopnez	a14, 88a <sbc_frame_decode+0x3f2>
     848:	320721c00881047e                  	{ s16i	a7, a4, 2; ae_lb	a7, a2 }
     850:	646d                                    	ae_s16i.n	a6, a4, 0
     852:	0802a4400881039e                  	{ s16i	a9, a3, 2; addi	a8, a4, 4 }
     85a:	070244400cb9752e                  	{ ae_db	a5, a2; extui	a4, a7, 4, 4 }
     862:	120721c00082084e                  	{ s16i	a4, a8, 0; ae_lb	a9, a2 }
     86a:	0502e8411061207e                  	{ extui	a6, a7, 0, 4; addi	a4, a8, 4 }
     872:	0202a3400881186e                  	{ s16i	a6, a8, 2; addi	a3, a3, 4 }
     87a:	380220400099137e                  	{ s16i	a7, a3, 0; extui	a7, a9, 0, 4 }
     882:	290244400cb9752e                  	{ ae_db	a5, a2; extui	a6, a9, 4, 4 }

0000088a <sbc_frame_decode+0x3f2>:
     88a:	ff9306                                    	j	6da <sbc_frame_decode+0x242>

0000088d <sbc_frame_decode+0x3f5>:
     88d:	a0a452                                    	movi	a5, 0x4a0
     890:	061c62                                    	l16ui	a6, a12, 12
     893:	0bdc42                                    	addmi	a4, a12, 0xb00
     896:	5c5a                                    	add.n	a5, a12, a5
     898:	5394f2                                    	l16si	a15, a4, 166
     89b:	286452                                    	s32i	a5, a4, 160
     89e:	0bdc32                                    	addmi	a3, a12, 0xb00
     8a1:	5254f2                                    	s16i	a15, a4, 164
     8a4:	0bdc22                                    	addmi	a2, a12, 0xb00
     8a7:	4d4666                                    	bnei	a6, 4, 8f8 <sbc_frame_decode+0x460>
     8aa:	fde151                                    	l32r	a5, 30 (16e0 <SBC_synthesis4_per_channel>)
     8ad:	001286                                    	j	8fb <sbc_frame_decode+0x463>

000008b0 <sbc_frame_decode+0x418>:
     8b0:	0bdc32                                    	addmi	a3, a12, 0xb00
     8b3:	6c7a                                    	add.n	a6, a12, a7
     8b5:	5493f2                                    	l16si	a15, a3, 168
     8b8:	286362                                    	s32i	a6, a3, 160
     8bb:	5253f2                                    	s16i	a15, a3, 164
     8be:	0bdc42                                    	addmi	a4, a12, 0xb00
     8c1:	6b4866                                    	bnei	a8, 4, 930 <sbc_frame_decode+0x498>
     8c4:	fddb51                                    	l32r	a5, 30 (16e0 <SBC_synthesis4_per_channel>)
     8c7:	001a06                                    	j	933 <sbc_frame_decode+0x49b>

000008ca <sbc_frame_decode+0x432>:
     8ca:	fdda51                                    	l32r	a5, 34 (1ca0 <SBC_synthesis8_per_channel>)
     8cd:	31b8                                    	l32i.n	a11, a1, 12
     8cf:	0a0c                                    	movi.n	a10, 0
     8d1:	0005e0                                    	callx8	a5
     8d4:	529442                                    	l16si	a4, a4, 164
     8d7:	535342                                    	s16i	a4, a3, 166
     8da:	3138                                    	l32i.n	a3, a1, 12
     8dc:	2148                                    	l32i.n	a4, a1, 8
     8de:	094216                                    	beqz	a2, 976 <sbc_frame_decode+0x4de>
     8e1:	0f8276                                    	loop	a2, 8f4 <sbc_frame_decode+0x45c>
     8e4:	220283400080132e                  	{ l16si	a2, a3, 0; addi	a3, a3, 2 }
     8ec:	2402a4400080042e                  	{ s16i	a2, a4, 0; addi	a4, a4, 2 }

000008f4 <sbc_frame_decode+0x45c>:
     8f4:	020c                                    	movi.n	a2, 0
     8f6:	f01d                                    	retw.n

000008f8 <sbc_frame_decode+0x460>:
     8f8:	fdcf51                                    	l32r	a5, 34 (1ca0 <SBC_synthesis8_per_channel>)
     8fb:	31b8                                    	l32i.n	a11, a1, 12
     8fd:	0a0c                                    	movi.n	a10, 0
     8ff:	0c7d                                    	mov.n	a7, a12
     901:	0005e0                                    	callx8	a5
     904:	529252                                    	l16si	a5, a2, 164
     907:	071762                                    	l16ui	a6, a7, 14
     90a:	535452                                    	s16i	a5, a4, 166
     90d:	061782                                    	l16ui	a8, a7, 12
     910:	20a792                                    	movi	a9, 0x720
     913:	0bd742                                    	addmi	a4, a7, 0xb00
     916:	579a                                    	add.n	a5, a7, a9
     918:	5a2666                                    	bnei	a6, 2, 976 <sbc_frame_decode+0x4de>
     91b:	5494f2                                    	l16si	a15, a4, 168
     91e:	07cd                                    	mov.n	a12, a7
     920:	286352                                    	s32i	a5, a3, 160
     923:	5252f2                                    	s16i	a15, a2, 164
     926:	3c4866                                    	bnei	a8, 4, 966 <sbc_frame_decode+0x4ce>
     929:	fdc131                                    	l32r	a3, 30 (16e0 <SBC_synthesis4_per_channel>)
     92c:	000e46                                    	j	969 <sbc_frame_decode+0x4d1>

0000092f <sbc_frame_decode+0x497>:
	...

00000930 <sbc_frame_decode+0x498>:
     930:	fdc151                                    	l32r	a5, 34 (1ca0 <SBC_synthesis8_per_channel>)
     933:	21b8                                    	l32i.n	a11, a1, 8
     935:	1a0c                                    	movi.n	a10, 1
     937:	0005e0                                    	callx8	a5
     93a:	529442                                    	l16si	a4, a4, 164
     93d:	545342                                    	s16i	a4, a3, 168
     940:	3138                                    	l32i.n	a3, a1, 12
     942:	2148                                    	l32i.n	a4, a1, 8
     944:	e2ac                                    	beqz.n	a2, 976 <sbc_frame_decode+0x4de>
     946:	0020f0                                    	nop
     949:	f03d                                    	nop.n
     94b:	f03d                                    	nop.n
     94d:	0f8276                                    	loop	a2, 960 <sbc_frame_decode+0x4c8>
     950:	240284400080042e                  	{ l16si	a2, a4, 0; addi	a4, a4, 2 }
     958:	2202a3400080132e                  	{ s16i	a2, a3, 0; addi	a3, a3, 2 }

00000960 <sbc_frame_decode+0x4c8>:
     960:	020c                                    	movi.n	a2, 0
     962:	f01d                                    	retw.n

00000964 <sbc_frame_decode+0x4cc>:
	...

00000966 <sbc_frame_decode+0x4ce>:
     966:	fdb331                                    	l32r	a3, 34 (1ca0 <SBC_synthesis8_per_channel>)
     969:	21b8                                    	l32i.n	a11, a1, 8
     96b:	1a0c                                    	movi.n	a10, 1
     96d:	0003e0                                    	callx8	a3
     970:	529222                                    	l16si	a2, a2, 164
     973:	545422                                    	s16i	a2, a4, 168
     976:	020c                                    	movi.n	a2, 0
     978:	f01d                                    	retw.n

0000097a <sbc_frame_decode+0x4e2>:
	...

0000097c <sbc_zero_sb_samples>:
     97c:	004136                                    	entry	a1, 32
     97f:	58c2a2                                    	addi	a10, a2, 88
     982:	fda721                                    	l32r	a2, 20 (108 <memset>)
     985:	0b0c                                    	movi.n	a11, 0
     987:	00a4c2                                    	movi	a12, 0x400
     98a:	0002e0                                    	callx8	a2
     98d:	f01d                                    	retw.n

0000098f <sbc_zero_sb_samples+0x13>:
	...

00000990 <pseudo_stereo>:
     990:	004136                                    	entry	a1, 32
     993:	f01d                                    	retw.n

00000995 <pseudo_stereo+0x5>:
     995:	000000                                        ...

00000998 <pseudo_stereo_setup>:
     998:	004136                                    	entry	a1, 32
     99b:	f01d                                    	retw.n

0000099d <pseudo_stereo_setup+0x5>:
     99d:	000000                                        ...

000009a0 <sbc_bit_allocation>:
     9a0:	00c136                                    	entry	a1, 96
     9a3:	041242                                    	l16ui	a4, a2, 8
     9a6:	4f3c                                    	movi.n	a15, 52
     9a8:	9149                                    	s32i.n	a4, a1, 36
     9aa:	c30c                                    	movi.n	a3, 12
     9ac:	051282                                    	l16ui	a8, a2, 10
     9af:	02ed                                    	mov.n	a14, a2
     9b1:	044140                                    	extui	a4, a4, 1, 1
     9b4:	050c                                    	movi.n	a5, 0
     9b6:	933f40                                    	movnez	a3, a15, a4
     9b9:	fd9f41                                    	l32r	a4, 38 (34c0 <SBC_OFFSET8>)
     9bc:	223a                                    	add.n	a2, a2, a3
     9be:	021e32                                    	l16ui	a3, a14, 4
     9c1:	42cd                                    	ae_l16ui.n	a12, a2, 0
     9c3:	061e22                                    	l16ui	a2, a14, 12
     9c6:	78a472                                    	movi	a7, 0x478
     9c9:	3d1826                                    	beqi	a8, 1, a0a <sbc_bit_allocation+0x6a>
     9cc:	9c0b                                    	addi.n	a9, a12, -1
     9ce:	8cbc                                    	beqz.n	a12, a0a <sbc_bit_allocation+0x6a>
     9d0:	fd9b61                                    	l32r	a6, 3c (34a0 <SBC_OFFSET4>)
     9d3:	fcc2a2                                    	addi	a10, a2, -4
     9d6:	c1d320                                    	mul16u	a13, a3, a2
     9d9:	320b                                    	addi.n	a3, a2, -1
     9db:	9364a0                                    	movnez	a6, a4, a10
     9de:	10f530                                    	and	a15, a5, a3
     9e1:	902d60                                    	addx2	a2, a13, a6
     9e4:	4e7a                                    	add.n	a4, a14, a7
     9e6:	906f20                                    	addx2	a6, a15, a2
     9e9:	551b                                    	addi.n	a5, a5, 1
     9eb:	266d                                    	ae_l16si.n	a6, a6, 0
     9ed:	179976                                    	loopnez	a9, a08 <sbc_bit_allocation+0x68>
     9f0:	1502a54518a0553e                  	{ and	a7, a5, a3; addi	a5, a5, 1 }
     9f8:	2206a7400095046e                  	{ s16i	a6, a4, 0; addx2	a6, a7, a2 }
     a00:	240284400080066e                  	{ l16si	a6, a6, 0; addi	a4, a4, 2 }

00000a08 <sbc_bit_allocation+0x68>:
     a08:	646d                                    	ae_s16i.n	a6, a4, 0

00000a0a <sbc_bit_allocation+0x6a>:
     a0a:	d5a522                                    	movi	a2, 0x5d5
     a0d:	38ce32                                    	addi	a3, a14, 56
     a10:	78a4f2                                    	movi	a15, 0x478
     a13:	1122f0                                    	slli	a2, a2, 1
     a16:	7139                                    	s32i.n	a3, a1, 28
     a18:	041c                                    	movi.n	a4, 16
     a1a:	050c                                    	movi.n	a5, 0
     a1c:	3efa                                    	add.n	a3, a14, a15
     a1e:	086142                                    	s32i	a4, a1, 32
     a21:	802e20                                    	add	a2, a14, a2
     a24:	00a042                                    	movi	a4, 0
     a27:	00a092                                    	movi	a9, 0
     a2a:	046152                                    	s32i	a5, a1, 16
     a2d:	0a6132                                    	s32i	a3, a1, 40
     a30:	5129                                    	s32i.n	a2, a1, 20
     a32:	b1e9                                    	s32i.n	a14, a1, 44
     a34:	061ea2                                    	l16ui	a10, a14, 12
     a37:	f149                                    	s32i.n	a4, a1, 60
     a39:	5128                                    	l32i.n	a2, a1, 20
     a3b:	050c                                    	movi.n	a5, 0
     a3d:	7138                                    	l32i.n	a3, a1, 28
     a3f:	82b4a0                                    	mull	a11, a4, a10
     a42:	a148                                    	l32i.n	a4, a1, 40
     a44:	5e1866                                    	bnei	a8, 1, aa6 <sbc_bit_allocation+0x106>
     a47:	5128                                    	l32i.n	a2, a1, 20
     a49:	286c16                                    	beqz	a12, cd3 <sbc_bit_allocation+0x333>
     a4c:	71f8                                    	l32i.n	a15, a1, 28
     a4e:	050c                                    	movi.n	a5, 0
     a50:	8042b0                                    	add	a4, a2, a11
     a53:	4121c0                                    	srli	a2, a12, 1
     a56:	803fb0                                    	add	a3, a15, a11
     a59:	0c6c07                                    	bbci	a12, 0, a69 <sbc_bit_allocation+0xc9>
     a5c:	237d                                    	ae_l16si.n	a7, a3, 0
     a5e:	02c332                                    	addi	a3, a3, 2
     a61:	647d                                    	ae_s16i.n	a7, a4, 0
     a63:	02c442                                    	addi	a4, a4, 2
     a66:	535570                                    	max	a5, a5, a7
     a69:	f20b                                    	addi.n	a15, a2, -1
     a6b:	063216                                    	beqz	a2, ad2 <sbc_bit_allocation+0x132>
     a6e:	236d                                    	ae_l16si.n	a6, a3, 0
     a70:	646d                                    	ae_s16i.n	a6, a4, 0
     a72:	337d                                    	ae_l16si.n	a7, a3, 2
     a74:	334b                                    	addi.n	a3, a3, 4
     a76:	536560                                    	max	a6, a5, a6
     a79:	747d                                    	ae_s16i.n	a7, a4, 2
     a7b:	072d                                    	mov.n	a2, a7
     a7d:	1f9f76                                    	loopnez	a15, aa0 <sbc_bit_allocation+0x100>
     a80:	040284400081035e                  	{ l16si	a5, a3, 0; addi	a4, a4, 4 }
     a88:	2206a6c00089045e                  	{ s16i	a5, a4, 0; max	a6, a6, a2 }
     a90:	020283400881132e                  	{ l16si	a2, a3, 2; addi	a3, a3, 4 }
     a98:	2406a6c00889142e                  	{ s16i	a2, a4, 2; max	a6, a6, a5 }

00000aa0 <sbc_bit_allocation+0x100>:
     aa0:	535620                                    	max	a5, a6, a2
     aa3:	000ac6                                    	j	ad2 <sbc_bit_allocation+0x132>

00000aa6 <sbc_bit_allocation+0x106>:
     aa6:	22ba                                    	add.n	a2, a2, a11
     aa8:	227c16                                    	beqz	a12, cd3 <sbc_bit_allocation+0x333>
     aab:	33ba                                    	add.n	a3, a3, a11
     aad:	218c76                                    	loop	a12, ad2 <sbc_bit_allocation+0x132>
     ab0:	436d                                    	ae_l16ui.n	a6, a3, 0
     ab2:	f68c                                    	beqz.n	a6, ac5 <sbc_bit_allocation+0x125>
     ab4:	247d                                    	ae_l16si.n	a7, a4, 0
     ab6:	c06670                                    	sub	a6, a6, a7
     ab9:	727960                                    	salt	a7, a9, a6
     abc:	400700                                    	ssr	a7
     abf:	b16060                                    	sra	a6, a6
     ac2:	000046                                    	j	ac7 <sbc_bit_allocation+0x127>

00000ac5 <sbc_bit_allocation+0x125>:
     ac5:	b67c                                    	movi.n	a6, -5
     ac7:	626d                                    	ae_s16i.n	a6, a2, 0
     ac9:	535650                                    	max	a5, a6, a5
     acc:	222b                                    	addi.n	a2, a2, 2
     ace:	332b                                    	addi.n	a3, a3, 2
     ad0:	442b                                    	addi.n	a4, a4, 2

00000ad2 <sbc_bit_allocation+0x132>:
     ad2:	4128                                    	l32i.n	a2, a1, 16
     ad4:	081ed2                                    	l16ui	a13, a14, 16
     ad7:	51f8                                    	l32i.n	a15, a1, 20
     ad9:	6199                                    	s32i.n	a9, a1, 24
     adb:	351b                                    	addi.n	a3, a5, 1
     add:	d1a9                                    	s32i.n	a10, a1, 52
     adf:	8222a0                                    	mull	a2, a2, a10
     ae2:	efba                                    	add.n	a14, a15, a11
     ae4:	c129                                    	s32i.n	a2, a1, 48
     ae6:	0f0c                                    	movi.n	a15, 0
     ae8:	e1b9                                    	s32i.n	a11, a1, 56
     aea:	080c                                    	movi.n	a8, 0

00000aec <sbc_bit_allocation+0x14c>:
     aec:	f8fa                                    	add.n	a15, a8, a15
     aee:	080c                                    	movi.n	a8, 0
     af0:	032d                                    	mov.n	a2, a3
     af2:	050c                                    	movi.n	a5, 0
     af4:	32fb                                    	addi.n	a3, a2, 15
     af6:	0adc16                                    	beqz	a12, ba7 <sbc_bit_allocation+0x207>
     af9:	0e4d                                    	mov.n	a4, a14
     afb:	4161c0                                    	srli	a6, a12, 1
     afe:	b52b                                    	addi.n	a11, a5, 2
     b00:	196c07                                    	bbci	a12, 0, b1d <sbc_bit_allocation+0x17d>
     b03:	24ad                                    	ae_l16si.n	a10, a4, 0
     b05:	851b                                    	addi.n	a8, a5, 1
     b07:	442b                                    	addi.n	a4, a4, 2
     b09:	c072a0                                    	sub	a7, a2, a10
     b0c:	729a30                                    	salt	a9, a10, a3
     b0f:	93b570                                    	movnez	a11, a5, a7
     b12:	72a2a0                                    	salt	a10, a2, a10
     b15:	838b90                                    	moveqz	a8, a11, a9
     b18:	838ba0                                    	moveqz	a8, a11, a10
     b1b:	085d                                    	mov.n	a5, a8
     b1d:	b60b                                    	addi.n	a11, a6, -1
     b1f:	084616                                    	beqz	a6, ba7 <sbc_bit_allocation+0x207>
     b22:	247d                                    	ae_l16si.n	a7, a4, 0
     b24:	058d                                    	mov.n	a8, a5
     b26:	652b                                    	addi.n	a6, a5, 2
     b28:	34ad                                    	ae_l16si.n	a10, a4, 2
     b2a:	551b                                    	addi.n	a5, a5, 1
     b2c:	04c442                                    	addi	a4, a4, 4
     b2f:	c09270                                    	sub	a9, a2, a7
     b32:	936890                                    	movnez	a6, a8, a9
     b35:	728730                                    	salt	a8, a7, a3
     b38:	729270                                    	salt	a9, a2, a7
     b3b:	835680                                    	moveqz	a5, a6, a8
     b3e:	835690                                    	moveqz	a5, a6, a9
     b41:	c072a0                                    	sub	a7, a2, a10
     b44:	01c582                                    	addi	a8, a5, 1
     b47:	729a30                                    	salt	a9, a10, a3
     b4a:	02c562                                    	addi	a6, a5, 2
     b4d:	4a9b76                                    	loopnez	a11, b9b <sbc_bit_allocation+0x1fb>
     b50:	260695400085147e                  	{ l16si	a7, a4, 0; movnez	a6, a5, a7 }
     b58:	1a06b24600eda69e                  	{ moveqz	a8, a6, a9; salt	a5, a2, a10 }
     b60:	040686c0089a14ae                  	{ l16si	a10, a4, 2; moveqz	a8, a6, a5 }
     b68:	2602e8400c60085e                  	{ addi	a5, a8, 1; addi	a6, a8, 2 }
     b70:	c09270                                    	sub	a9, a2, a7
     b73:	0502a44510e1c89e                  	{ movnez	a6, a8, a9; addi	a4, a4, 4 }
     b7b:	1306ba4600eef73e                  	{ salt	a8, a7, a3; salt	a9, a10, a3 }
     b83:	3606b24508edb68e                  	{ moveqz	a5, a6, a8; salt	a7, a2, a7 }
     b8b:	3a06b24508f9a67e                  	{ moveqz	a5, a6, a7; sub	a7, a2, a10 }
     b93:	2602e5400c60058e                  	{ addi	a8, a5, 1; addi	a6, a5, 2 }

00000b9b <sbc_bit_allocation+0x1fb>:
     b9b:	936570                                    	movnez	a6, a5, a7
     b9e:	72b2a0                                    	salt	a11, a2, a10
     ba1:	838690                                    	moveqz	a8, a6, a9
     ba4:	8386b0                                    	moveqz	a8, a6, a11

00000ba7 <sbc_bit_allocation+0x207>:
     ba7:	a8fa                                    	add.n	a10, a8, a15
     ba9:	320b                                    	addi.n	a3, a2, -1
     bab:	2207009c1fe5fade                  	{ blt.w15	a10, a13, aec <sbc_bit_allocation+0x14c>; nop }
     bb3:	fec222                                    	addi	a2, a2, -2
     bb6:	c0ead0                                    	sub	a14, a10, a13
     bb9:	9323e0                                    	movnez	a2, a3, a14
     bbc:	5138                                    	l32i.n	a3, a1, 20
     bbe:	e148                                    	l32i.n	a4, a1, 56
     bc0:	ccbc                                    	beqz.n	a12, c00 <sbc_bit_allocation+0x260>
     bc2:	9c0b                                    	addi.n	a9, a12, -1
     bc4:	8158                                    	l32i.n	a5, a1, 32
     bc6:	734a                                    	add.n	a7, a3, a4
     bc8:	322b                                    	addi.n	a3, a2, 2
     bca:	274d                                    	ae_l16si.n	a4, a7, 0
     bcc:	c08420                                    	sub	a8, a4, a2
     bcf:	726430                                    	salt	a6, a4, a3
     bd2:	062142                                    	l32i	a4, a1, 24
     bd5:	1a9976                                    	loopnez	a9, bf3 <sbc_bit_allocation+0x253>
     bd8:	040688c00896179e                  	{ l16si	a9, a7, 2; minu	a8, a8, a5 }
     be0:	938460                                    	movnez	a8, a4, a6
     be3:	2602a7400080378e                  	{ s16i	a8, a7, 64; addi	a7, a7, 2 }
     beb:	2306994204ed592e                  	{ sub	a8, a9, a2; salt	a6, a9, a3 }

00000bf3 <sbc_bit_allocation+0x253>:
     bf3:	633850                                    	minu	a3, a8, a5
     bf6:	8159                                    	s32i.n	a5, a1, 32
     bf8:	933460                                    	movnez	a3, a4, a6
     bfb:	6149                                    	s32i.n	a4, a1, 24
     bfd:	205732                                    	s16i	a3, a7, 64

00000c00 <sbc_bit_allocation+0x260>:
     c00:	4138                                    	l32i.n	a3, a1, 16
     c02:	9148                                    	l32i.n	a4, a1, 36
     c04:	93afe0                                    	movnez	a10, a15, a14
     c07:	b1e8                                    	l32i.n	a14, a1, 44
     c09:	f5a5b2                                    	movi	a11, 0x5f5
     c0c:	df0c                                    	movi.n	a15, 13
     c0e:	11bbf0                                    	slli	a11, a11, 1
     c11:	331b                                    	addi.n	a3, a3, 1
     c13:	d158                                    	l32i.n	a5, a1, 52
     c15:	230740860005d41e                  	{ bbsi.w15	a4, 1, cb4 <sbc_bit_allocation+0x314>; nop }
     c1d:	4148                                    	l32i.n	a4, a1, 16
     c1f:	52aad7                                    	bge	a10, a13, c75 <sbc_bit_allocation+0x2d5>
     c22:	c168                                    	l32i.n	a6, a1, 48
     c24:	441b                                    	addi.n	a4, a4, 1
     c26:	825450                                    	mull	a5, a4, a5
     c29:	9076e0                                    	addx2	a7, a6, a14
     c2c:	45b657                                    	bgeu	a6, a5, c75 <sbc_bit_allocation+0x2d5>
     c2f:	521b                                    	addi.n	a5, a2, 1
     c31:	67ba                                    	add.n	a6, a7, a11
     c33:	3b8076                                    	loop	a0, c72 <sbc_bit_allocation+0x2d2>
     c36:	467d                                    	ae_l16ui.n	a7, a6, 0
     c38:	9a1b                                    	addi.n	a9, a10, 1
     c3a:	fec782                                    	addi	a8, a7, -2
     c3d:	771b                                    	addi.n	a7, a7, 1
     c3f:	988d                                    	ae_zext16	a8, a8
     c41:	043f87                                    	bltu	a15, a8, c49 <sbc_bit_allocation+0x2a9>
     c44:	180c                                    	movi.n	a8, 1
     c46:	000386                                    	j	c58 <sbc_bit_allocation+0x2b8>

00000c49 <sbc_bit_allocation+0x2a9>:
     c49:	c0c672                                    	addi	a7, a6, -64
     c4c:	0ca9d7                                    	bge	a9, a13, c5c <sbc_bit_allocation+0x2bc>
     c4f:	279d                                    	ae_l16si.n	a9, a7, 0
     c51:	280c                                    	movi.n	a8, 2
     c53:	270c                                    	movi.n	a7, 2
     c55:	039597                                    	bne	a5, a9, c5c <sbc_bit_allocation+0x2bc>
     c58:	667d                                    	ae_s16i.n	a7, a6, 0
     c5a:	aa8a                                    	add.n	a10, a10, a8
     c5c:	081ed2                                    	l16ui	a13, a14, 16
     c5f:	662b                                    	addi.n	a6, a6, 2
     c61:	061e72                                    	l16ui	a7, a14, 12
     c64:	0daad7                                    	bge	a10, a13, c75 <sbc_bit_allocation+0x2d5>
     c67:	827470                                    	mull	a7, a4, a7
     c6a:	9077e0                                    	addx2	a7, a7, a14
     c6d:	77ba                                    	add.n	a7, a7, a11
     c6f:	02b677                                    	bgeu	a6, a7, c75 <sbc_bit_allocation+0x2d5>

00000c72 <sbc_bit_allocation+0x2d2>:
     c72:	ffef46                                    	j	c33 <sbc_bit_allocation+0x293>

00000c75 <sbc_bit_allocation+0x2d5>:
     c75:	4158                                    	l32i.n	a5, a1, 16
     c77:	39aad7                                    	bge	a10, a13, cb4 <sbc_bit_allocation+0x314>
     c7a:	061e62                                    	l16ui	a6, a14, 12
     c7d:	451b                                    	addi.n	a4, a5, 1
     c7f:	825560                                    	mull	a5, a5, a6
     c82:	826460                                    	mull	a6, a4, a6
     c85:	9075e0                                    	addx2	a7, a5, a14
     c88:	28b567                                    	bgeu	a5, a6, cb4 <sbc_bit_allocation+0x314>
     c8b:	57ba                                    	add.n	a5, a7, a11
     c8d:	208076                                    	loop	a0, cb1 <sbc_bit_allocation+0x311>
     c90:	456d                                    	ae_l16ui.n	a6, a5, 0
     c92:	761b                                    	addi.n	a7, a6, 1
     c94:	06b6f6                                    	bgeui	a6, 16, c9e <sbc_bit_allocation+0x2fe>
     c97:	aa1b                                    	addi.n	a10, a10, 1
     c99:	657d                                    	ae_s16i.n	a7, a5, 0
     c9b:	081ed2                                    	l16ui	a13, a14, 16
     c9e:	552b                                    	addi.n	a5, a5, 2
     ca0:	10aad7                                    	bge	a10, a13, cb4 <sbc_bit_allocation+0x314>
     ca3:	061e62                                    	l16ui	a6, a14, 12
     ca6:	826460                                    	mull	a6, a4, a6
     ca9:	9066e0                                    	addx2	a6, a6, a14
     cac:	66ba                                    	add.n	a6, a6, a11
     cae:	02b567                                    	bgeu	a5, a6, cb4 <sbc_bit_allocation+0x314>

00000cb1 <sbc_bit_allocation+0x311>:
     cb1:	fff606                                    	j	c8d <sbc_bit_allocation+0x2ed>

00000cb4 <sbc_bit_allocation+0x314>:
     cb4:	041e72                                    	l16ui	a7, a14, 8
     cb7:	240c                                    	movi.n	a4, 2
     cb9:	f1b8                                    	l32i.n	a11, a1, 60
     cbb:	160c                                    	movi.n	a6, 1
     cbd:	051e82                                    	l16ui	a8, a14, 10
     cc0:	6198                                    	l32i.n	a9, a1, 24
     cc2:	570b                                    	addi.n	a5, a7, -1
     cc4:	934650                                    	movnez	a4, a6, a5
     cc7:	0fb347                                    	bgeu	a3, a4, cda <sbc_bit_allocation+0x33a>
     cca:	4b2b                                    	addi.n	a4, a11, 2
     ccc:	9179                                    	s32i.n	a7, a1, 36
     cce:	4139                                    	s32i.n	a3, a1, 16
     cd0:	ff5806                                    	j	a34 <sbc_bit_allocation+0x94>

00000cd3 <sbc_bit_allocation+0x333>:
     cd3:	050c                                    	movi.n	a5, 0
     cd5:	ff7e46                                    	j	ad2 <sbc_bit_allocation+0x132>

00000cd8 <sbc_bit_allocation+0x338>:
	...

00000cda <sbc_bit_allocation+0x33a>:
     cda:	221b                                    	addi.n	a2, a2, 1
     cdc:	220740800005f71e                  	{ bbci.w15	a7, 1, d7c <sbc_bit_allocation+0x3dc>; nop }
     ce4:	061e72                                    	l16ui	a7, a14, 12
     ce7:	56aad7                                    	bge	a10, a13, d41 <sbc_bit_allocation+0x3a1>
     cea:	d40c                                    	movi.n	a4, 13
     cec:	150c                                    	movi.n	a5, 1
     cee:	060c                                    	movi.n	a6, 0
     cf0:	030c                                    	movi.n	a3, 0
     cf2:	04b716                                    	beqz	a7, d41 <sbc_bit_allocation+0x3a1>
     cf5:	458076                                    	loop	a0, d3e <sbc_bit_allocation+0x39e>
     cf8:	827370                                    	mull	a7, a3, a7
     cfb:	9077e0                                    	addx2	a7, a7, a14
     cfe:	ca1b                                    	addi.n	a12, a10, 1
     d00:	908670                                    	addx2	a8, a6, a7
     d03:	0bd872                                    	addmi	a7, a8, 0xb00
     d06:	0bd882                                    	addmi	a8, a8, 0xb00
     d09:	751792                                    	l16ui	a9, a7, 234
     d0c:	fec9b2                                    	addi	a11, a9, -2
     d0f:	9bbd                                    	ae_zext16	a11, a11
     d11:	0634b7                                    	bltu	a4, a11, d1b <sbc_bit_allocation+0x37b>
     d14:	891b                                    	addi.n	a8, a9, 1
     d16:	190c                                    	movi.n	a9, 1
     d18:	000306                                    	j	d28 <sbc_bit_allocation+0x388>

00000d1b <sbc_bit_allocation+0x37b>:
     d1b:	290c                                    	movi.n	a9, 2
     d1d:	0cacd7                                    	bge	a12, a13, d2d <sbc_bit_allocation+0x38d>
     d20:	5598b2                                    	l16si	a11, a8, 170
     d23:	280c                                    	movi.n	a8, 2
     d25:	0492b7                                    	bne	a2, a11, d2d <sbc_bit_allocation+0x38d>
     d28:	755782                                    	s16i	a8, a7, 234
     d2b:	aa9a                                    	add.n	a10, a10, a9
     d2d:	081ed2                                    	l16ui	a13, a14, 16
     d30:	663a                                    	add.n	a6, a6, a3
     d32:	061e72                                    	l16ui	a7, a14, 12
     d35:	303350                                    	xor	a3, a3, a5
     d38:	05aad7                                    	bge	a10, a13, d41 <sbc_bit_allocation+0x3a1>
     d3b:	02b677                                    	bgeu	a6, a7, d41 <sbc_bit_allocation+0x3a1>

00000d3e <sbc_bit_allocation+0x39e>:
     d3e:	ffecc6                                    	j	cf5 <sbc_bit_allocation+0x355>

00000d41 <sbc_bit_allocation+0x3a1>:
     d41:	020c                                    	movi.n	a2, 0
     d43:	35aad7                                    	bge	a10, a13, d7c <sbc_bit_allocation+0x3dc>
     d46:	130c                                    	movi.n	a3, 1
     d48:	050c                                    	movi.n	a5, 0
     d4a:	2b8076                                    	loop	a0, d79 <sbc_bit_allocation+0x3d9>
     d4d:	061e62                                    	l16ui	a6, a14, 12
     d50:	054d                                    	mov.n	a4, a5
     d52:	26b267                                    	bgeu	a2, a6, d7c <sbc_bit_allocation+0x3dc>
     d55:	82f560                                    	mull	a15, a5, a6
     d58:	905fe0                                    	addx2	a5, a15, a14
     d5b:	905250                                    	addx2	a5, a2, a5
     d5e:	0bd552                                    	addmi	a5, a5, 0xb00
     d61:	751562                                    	l16ui	a6, a5, 234
     d64:	761b                                    	addi.n	a7, a6, 1
     d66:	07b6f6                                    	bgeui	a6, 16, d71 <sbc_bit_allocation+0x3d1>
     d69:	755572                                    	s16i	a7, a5, 234
     d6c:	aa1b                                    	addi.n	a10, a10, 1
     d6e:	081ed2                                    	l16ui	a13, a14, 16
     d71:	305430                                    	xor	a5, a4, a3
     d74:	224a                                    	add.n	a2, a2, a4
     d76:	02aad7                                    	bge	a10, a13, d7c <sbc_bit_allocation+0x3dc>

00000d79 <sbc_bit_allocation+0x3d9>:
     d79:	fff346                                    	j	d4a <sbc_bit_allocation+0x3aa>

00000d7c <sbc_bit_allocation+0x3dc>:
     d7c:	f01d                                    	retw.n

00000d7e <sbc_bit_allocation+0x3de>:
	...

00000d80 <SAT_ADD>:
     d80:	004136                                    	entry	a1, 32
     d83:	0ead456b09e133d0c48081390001020f 	{ ae_cvt48a32	aed0, a2; ae_cvt48a32	aed1, a3; nop; nop }
     d93:	0ead057b08e40cd0c48081380801c00f 	{ ae_addsq56s	aed0, aed0, aed1; nop; nop; nop }
     da3:	0ead217b0d440cd0c50061380c01015f 	{ nop; nop; nop; ae_roundsq32f48sym	aed0, aed0 }
     db3:	0ead257b0d240cd0c400813b0c01620f 	{ ae_trunca32q48	a2, aed0; nop; nop; nop }
     dc3:	f01d                                    	retw.n

00000dc5 <SAT_ADD+0x45>:
     dc5:	000000                                        ...

00000dc8 <SAT_SUB>:
     dc8:	004136                                    	entry	a1, 32
     dcb:	0ead456b09e133d0c48081390001020f 	{ ae_cvt48a32	aed0, a2; ae_cvt48a32	aed1, a3; nop; nop }
     ddb:	0ead057b08e40cd0c58081380c01600f 	{ ae_subsq56s	aed0, aed0, aed1; nop; nop; nop }
     deb:	0ead217b0d440cd0c50061380c01015f 	{ nop; nop; nop; ae_roundsq32f48sym	aed0, aed0 }
     dfb:	0ead257b0d240cd0c400813b0c01620f 	{ ae_trunca32q48	a2, aed0; nop; nop; nop }
     e0b:	f01d                                    	retw.n

00000e0d <SAT_SUB+0x45>:
     e0d:	000000                                        ...

00000e10 <sbc_reconstruction>:
     e10:	006136                                    	entry	a1, 48
     e13:	025d                                    	mov.n	a5, a2
     e15:	1a1222                                    	l16ui	a2, a2, 52
     e18:	e5a572                                    	movi	a7, 0x5e5
     e1b:	f5a532                                    	movi	a3, 0x5f5
     e1e:	1177f0                                    	slli	a7, a7, 1
     e21:	38c582                                    	addi	a8, a5, 56
     e24:	1133f0                                    	slli	a3, a3, 1
     e27:	657a                                    	add.n	a6, a5, a7
     e29:	79a192                                    	movi	a9, 0x179
     e2c:	068216                                    	beqz	a2, e98 <sbc_reconstruction+0x88>
     e2f:	040464400c80608e                  	{ ae_l16m.i	aed1, a8, 0; movi	a4, 4 }
     e37:	0e02e540148eb53e                  	{ ae_l16m.xu	aed2, a3, a5; addi	a15, a5, 56 }
     e3f:	0ea56425cd279dd0c50081150023044f 	{ ae_cvtp24a16x2.ll	aed0, a4, a4; slli	a9, a9, 3; ae_sub24s	aed3, aed1, aed2; nop }
     e4f:	0ea4e407fd232ad0c400830008c341ff 	{ ae_l16m.iu	aed1, a15, 2; addi	a10, a2, -1; ae_add24s	aed3, aed3, aed0; nop }
     e5f:	459a                                    	add.n	a4, a5, a9
     e61:	0f2d                                    	mov.n	a2, a15
     e63:	0020f0                                    	nop
     e66:	f03d                                    	nop.n
     e68:	1104534100a23a8e                  	{ loopnez	a10, e90 <sbc_reconstruction+0x80>; ae_l16m.iu	aed2, a3, 2 }
     e70:	0ea5243b0c450cd0c50081110421c14f 	{ ae_s16m.l.iu	aed3, a4, 2; nop; ae_sub24s	aed1, aed1, aed2; nop }
     e80:	0ea4e411152331d0c400810008c3412f 	{ ae_l16m.iu	aed1, a2, 2; ae_l16m.iu	aed2, a3, 2; ae_add24s	aed3, aed1, aed0; nop }

00000e90 <sbc_reconstruction+0x80>:
     e90:	230740c01cc5114e                  	{ ae_s16m.l.iu	aed3, a4, 2; nop }

00000e98 <sbc_reconstruction+0x88>:
     e98:	1a1522                                    	l16ui	a2, a5, 52
     e9b:	54c542                                    	addi	a4, a5, 84
     e9e:	1b1592                                    	l16ui	a9, a5, 54
     ea1:	f3f660                                    	wur.ae_cbegin0	a6
     ea4:	902250                                    	addx2	a2, a2, a5
     ea7:	fc6681                                    	l32r	a8, 40 (2540 <S_ARRAY>)
     eaa:	327a                                    	add.n	a3, a2, a7
     eac:	0c0760440461652e                  	{ addi	a2, a5, 112; ae_movi	aed1, 0 }
     eb4:	f3f730                                    	wur.ae_cend0	a3
     eb7:	ff2232                                    	l32i	a3, a2, 0x3fc
     eba:	270c                                    	movi.n	a7, 2
     ebc:	063916                                    	beqz	a9, f23 <sbc_reconstruction+0x113>
     ebf:	608976                                    	loop	a9, f23 <sbc_reconstruction+0x113>
     ec2:	370416440080169e                  	{ l16ui	a9, a6, 32; ae_l16m.xc	aed0, a6, a7 }
     eca:	290701c000e3dbfe                  	{ ae_trunca16p24s.l	a15, aed0; ae_lb	a14, a9 }
     ed2:	11a9e0                                    	slli	a10, a9, 2
     ed5:	042916                                    	beqz	a9, f1b <sbc_reconstruction+0x10b>
     ed8:	3b0458c00ca1639e                  	{ ae_db	a3, a9; ae_l16x2m.x	aed1, a8, a10 }
     ee0:	0ead257b0d240cd0c400813b0c01cb1f 	{ ae_trunca16p24s.h	a11, aed1; nop; nop; nop }
     ef0:	c0aeb0                                    	sub	a10, a14, a11
     ef3:	230700c71ca5faae                  	{ ae_cvtp24a16x2.ll	aed31, a10, a10; nop }
     efb:	1a835403343fb13e                  	{ nop; nop; ae_mul32.lh	aed0, aed1, aed31 }
     f03:	220700c010a51f0e                  	{ ae_slaasq56s	aed0, aed0, a15; nop }
     f0b:	0ead217b0d440cd0c50461380c01015f 	{ nop; nop; nop; ae_roundsq32f48sym	aed1, aed0 }
     f1b:	0d07604008c1a14e                  	{ ae_s32m.iu	aed1, a4, 4; ae_movi	aed1, 0 }

00000f23 <sbc_reconstruction+0x113>:
     f23:	10d542                                    	addmi	a4, a5, 0x1000
     f26:	ff6232                                    	s32i	a3, a2, 0x3fc
     f29:	081442                                    	l16ui	a4, a4, 16
     f2c:	6cc532                                    	addi	a3, a5, 108
     f2f:	a8ab62                                    	movi	a6, 0xfffffba8
     f32:	10a416                                    	beqz	a4, 1040 <sbc_reconstruction+0x230>
     f35:	230740a41045940e                  	{ beqi.w15	a4, 2, 11cc <sbc_reconstruction+0x3bc>; nop }
     f3d:	230740a408e5d46e                  	{ bnei.w15	a4, 1, 16dc <sbc_reconstruction+0x8cc>; nop }
     f45:	171522                                    	l16ui	a2, a5, 46
     f48:	1b15d2                                    	l16ui	a13, a5, 54
     f4b:	061542                                    	l16ui	a4, a5, 12
     f4e:	78a216                                    	beqz	a2, 16dc <sbc_reconstruction+0x8cc>
     f51:	ff2322                                    	l32i	a2, a3, 0x3fc
     f54:	a8ab32                                    	movi	a3, 0xfffffba8
     f57:	c02250                                    	sub	a2, a2, a5
     f5a:	223a                                    	add.n	a2, a2, a3
     f5c:	56a432                                    	movi	a3, 0x456
     f5f:	21c120                                    	srai	a12, a2, 1
     f62:	1129                                    	s32i.n	a2, a1, 4
     f64:	902c50                                    	addx2	a2, a12, a5
     f67:	230720a31cc55d6e                  	{ bnei.w15	a13, 0x100, 15d5 <sbc_reconstruction+0x7c5>; nop }
     f6f:	823a                                    	add.n	a8, a2, a3
     f71:	00a1d2                                    	movi	a13, 0x100
     f74:	1128                                    	l32i.n	a2, a1, 4
     f76:	230720a200c5f46e                  	{ bnei.w15	a4, 8, 15d8 <sbc_reconstruction+0x7c8>; nop }
     f7e:	00a042                                    	movi	a4, 0

00000f81 <sbc_reconstruction+0x171>:
     f81:	fec872                                    	addi	a7, a8, -2
     f84:	9c0b                                    	addi.n	a9, a12, -1
     f86:	220740a00c0532ce                  	{ blti.w15	a2, 1, 102e <sbc_reconstruction+0x21e>; nop }
     f8e:	482d                                    	ae_l16ui.n	a2, a8, 0
     f90:	242a                                    	add.n	a2, a4, a2
     f92:	a03250                                    	addx4	a3, a2, a5
     f95:	1e2362                                    	l32i	a6, a3, 120
     f98:	162322                                    	l32i	a2, a3, 88
     f9b:	2007224410e1e55e                  	{ or	a2, a5, a5; ae_cvt48a32	aed1, a2 }
     fa3:	075d                                    	mov.n	a5, a7
     fa5:	4a9976                                    	loopnez	a9, ff3 <sbc_reconstruction+0x1e3>
     fa8:	210706400080056e                  	{ l16ui	a6, a5, 0; ae_cvt48a32	aed0, a6 }
     fb0:	0eacc567f0e255d0c48081380001c01f 	{ ae_addsq56s	aed0, aed1, aed0; addi	a5, a5, -2; nop; nop }
     fc0:	0ead217b0d440cd0c50061380c01015f 	{ nop; nop; nop; ae_roundsq32f48sym	aed0, aed0 }
     fd0:	0ead65663d2246d0c400813b0c01660f 	{ ae_trunca32q48	a6, aed0; add	a7, a4, a6; nop; nop }
     fe0:	3306a7451098036e                  	{ s32i	a6, a3, 88; addx4	a3, a7, a2 }
     fe8:	162362                                    	l32i	a6, a3, 88
     feb:	200706471481036e                  	{ l32i	a6, a3, 120; ae_cvt48a32	aed1, a6 }

00000ff3 <sbc_reconstruction+0x1e3>:
     ff3:	2007264508e0e22e                  	{ or	a5, a2, a2; ae_cvt48a32	aed0, a6 }
     ffb:	0ead057b08e40cd0c48081380001c01f 	{ ae_addsq56s	aed0, aed1, aed0; nop; nop; nop }
    100b:	0ead217b0d440cd0c50061380c01015f 	{ nop; nop; nop; ae_roundsq32f48sym	aed0, aed0 }
    101b:	0ead257b0d240cd0c400813b0c016f0f 	{ ae_trunca32q48	a15, aed0; nop; nop; nop }
    102b:	1663f2                                    	s32i	a15, a3, 88

0000102e <sbc_reconstruction+0x21e>:
    102e:	10c442                                    	addi	a4, a4, 16
    1031:	1128                                    	l32i.n	a2, a1, 4
    1033:	230720bb1be5542e                  	{ bltui.w15	a4, 0x100, f81 <sbc_reconstruction+0x171>; nop }
    103b:	01a746                                    	j	16dc <sbc_reconstruction+0x8cc>

0000103e <sbc_reconstruction+0x22e>:
	...

00001040 <sbc_reconstruction+0x230>:
    1040:	6cc532                                    	addi	a3, a5, 108
    1043:	171522                                    	l16ui	a2, a5, 46
    1046:	ff2342                                    	l32i	a4, a3, 0x3fc
    1049:	1b15e2                                    	l16ui	a14, a5, 54
    104c:	a8ab62                                    	movi	a6, 0xfffffba8
    104f:	061572                                    	l16ui	a7, a5, 12
    1052:	56a432                                    	movi	a3, 0x456
    1055:	683216                                    	beqz	a2, 16dc <sbc_reconstruction+0x8cc>
    1058:	c02450                                    	sub	a2, a4, a5
    105b:	226a                                    	add.n	a2, a2, a6
    105d:	21d120                                    	srai	a13, a2, 1
    1060:	1129                                    	s32i.n	a2, a1, 4
    1062:	902d50                                    	addx2	a2, a13, a5
    1065:	220720a71845fe6e                  	{ bnei.w15	a14, 0x100, 12c6 <sbc_reconstruction+0x4b6>; nop }
    106d:	923a                                    	add.n	a9, a2, a3
    106f:	00a1e2                                    	movi	a14, 0x100
    1072:	030c                                    	movi.n	a3, 0
    1074:	220720a60045976e                  	{ bnei.w15	a7, 8, 12c9 <sbc_reconstruction+0x4b9>; nop }
    107c:	1128                                    	l32i.n	a2, a1, 4
    107e:	fec962                                    	addi	a6, a9, -2
    1081:	220700a40c25d2ce                  	{ blti.w15	a2, 1, 11be <sbc_reconstruction+0x3ae>; nop }
    1089:	492d                                    	ae_l16ui.n	a2, a9, 0
    108b:	230720c318e5747e                  	{ nop; nop }
    1093:	0020f0                                    	nop
    1096:	0020f0                                    	nop
    1099:	832a                                    	add.n	a8, a3, a2
    109b:	462d                                    	ae_l16ui.n	a2, a6, 0
    109d:	a04850                                    	addx4	a4, a8, a5
    10a0:	1624f2                                    	l32i	a15, a4, 88
    10a3:	20070f471480047e                  	{ l32i	a7, a4, 120; ae_cvt48a32	aed0, a15 }
    10ab:	2007074204e1232e                  	{ add	a8, a3, a2; ae_cvt48a32	aed1, a7 }
    10b3:	0ea4e027f52366f8c108200f1101255f 	{ or	a2, a5, a5; addi	a6, a6, -2; ae_subsq56s	aed1, aed0, aed1; ae_addsq56s	aed2, aed0, aed1 }
    10c3:	0ead217b0c750cd0c400613a1401fdaf 	{ addi	a10, a13, -2; nop; nop; ae_roundsq32f48sym	aed0, aed2 }
    10d3:	220760a010053d2e                  	{ bltui.w15	a13, 2, 119b <sbc_reconstruction+0x38b>; nop }
    10db:	035d                                    	mov.n	a5, a3
    10dd:	5f9a76                                    	loopnez	a10, 1140 <sbc_reconstruction+0x330>
    10e0:	0eace160052e63d0c400613b0c01670f 	{ ae_trunca32q48	a7, aed0; l16ui	a3, a6, 0; nop; ae_roundsq32f48sym	aed0, aed1 }
    10f0:	000003f3038300fbd6a303053460147f 	{ s32i	a7, a4, 88; ae_trunca32q48	a7, aed0; nop }
    1100:	0306a8471099047e                  	{ s32i	a7, a4, 120; addx4	a4, a8, a2 }
    1108:	280384451487047e                  	{ l32i	a7, a4, 88; l32i	a8, a4, 120 }
    1110:	0ead456b09e183d0c48081390001270f 	{ ae_cvt48a32	aed2, a7; ae_cvt48a32	aed1, a8; nop; nop }
    1120:	0ea5241b0d250cd0c500820f09a0853f 	{ add	a8, a5, a3; nop; ae_addsq56s	aed0, aed2, aed1; nop }
    1130:	0eacc167f0e266d0c58061380c01612f 	{ ae_subsq56s	aed1, aed2, aed1; addi	a6, a6, -2; nop; ae_roundsq32f48sym	aed0, aed0 }

00001140 <sbc_reconstruction+0x330>:
    1140:	0ead61679d2e55d0c404613b0c016f0f 	{ ae_trunca32q48	a15, aed0; or	a3, a5, a5; nop; ae_roundsq32f48sym	aed1, aed1 }
    1150:	025d                                    	mov.n	a5, a2
    1152:	000003f3038300fb76a30305346014ff 	{ s32i	a15, a4, 88; ae_trunca32q48	a6, aed1; nop }
    1162:	1e6462                                    	s32i	a6, a4, 120
    1165:	a04820                                    	addx4	a4, a8, a2
    1168:	162422                                    	l32i	a2, a4, 88
    116b:	20070247149e047e                  	{ l32i	a7, a4, 120; ae_cvt48a32	aed30, a2 }
    1173:	2107274318ff647e                  	{ nop; ae_cvt48a32	aed31, a7 }
    117b:	0ea5041b08e50cd0c5809eff1da261ef 	{ ae_subsq56s	aed1, aed30, aed31; nop; ae_addsq56s	aed2, aed30, aed31; nop }
    118b:	0ead217b0d540cd0c50061380c01015f 	{ nop; nop; nop; ae_roundsq32f48sym	aed0, aed2 }

0000119b <sbc_reconstruction+0x38b>:
    119b:	0ead217b0d2c0cd0c404613b0c01620f 	{ ae_trunca32q48	a2, aed0; nop; nop; ae_roundsq32f48sym	aed1, aed1 }
    11ab:	000003f3038300fb76a303053460142f 	{ s32i	a2, a4, 88; ae_trunca32q48	a6, aed1; nop }
    11bb:	1e6462                                    	s32i	a6, a4, 120
    11be:	10c332                                    	addi	a3, a3, 16
    11c1:	230740bf1fc5b32e                  	{ bltui.w15	a3, 0x100, 107c <sbc_reconstruction+0x26c>; nop }
    11c9:	0143c6                                    	j	16dc <sbc_reconstruction+0x8cc>

000011cc <sbc_reconstruction+0x3bc>:
    11cc:	171522                                    	l16ui	a2, a5, 46
    11cf:	6cc532                                    	addi	a3, a5, 108
    11d2:	ff2342                                    	l32i	a4, a3, 0x3fc
    11d5:	1b15d2                                    	l16ui	a13, a5, 54
    11d8:	061572                                    	l16ui	a7, a5, 12
    11db:	56a432                                    	movi	a3, 0x456
    11de:	4fa216                                    	beqz	a2, 16dc <sbc_reconstruction+0x8cc>
    11e1:	c02450                                    	sub	a2, a4, a5
    11e4:	040c                                    	movi.n	a4, 0
    11e6:	226a                                    	add.n	a2, a2, a6
    11e8:	21c120                                    	srai	a12, a2, 1
    11eb:	1129                                    	s32i.n	a2, a1, 4
    11ed:	902c50                                    	addx2	a2, a12, a5
    11f0:	220760a718453d6e                  	{ bnei.w15	a13, 0x100, 14b9 <sbc_reconstruction+0x6a9>; nop }
    11f8:	808230                                    	add	a8, a2, a3
    11fb:	00a1d2                                    	movi	a13, 0x100
    11fe:	012122                                    	l32i	a2, a1, 4
    1201:	230740a60445b76e                  	{ bnei.w15	a7, 8, 14bc <sbc_reconstruction+0x6ac>; nop }
    1209:	fec872                                    	addi	a7, a8, -2
    120c:	9c0b                                    	addi.n	a9, a12, -1
    120e:	220740a00c0532ce                  	{ blti.w15	a2, 1, 12b6 <sbc_reconstruction+0x4a6>; nop }
    1216:	482d                                    	ae_l16ui.n	a2, a8, 0
    1218:	242a                                    	add.n	a2, a4, a2
    121a:	a03250                                    	addx4	a3, a2, a5
    121d:	1e2362                                    	l32i	a6, a3, 120
    1220:	162322                                    	l32i	a2, a3, 88
    1223:	2007224410e1e55e                  	{ or	a2, a5, a5; ae_cvt48a32	aed1, a2 }
    122b:	075d                                    	mov.n	a5, a7
    122d:	4a9976                                    	loopnez	a9, 127b <sbc_reconstruction+0x46b>
    1230:	210706400080056e                  	{ l16ui	a6, a5, 0; ae_cvt48a32	aed0, a6 }
    1238:	0eacc567f0e255d0c58081380401601f 	{ ae_subsq56s	aed0, aed1, aed0; addi	a5, a5, -2; nop; nop }
    1248:	0ead217b0d440cd0c50061380c01015f 	{ nop; nop; nop; ae_roundsq32f48sym	aed0, aed0 }
    1258:	0ead65663d2246d0c400813b0c01660f 	{ ae_trunca32q48	a6, aed0; add	a7, a4, a6; nop; nop }
    1268:	3306a7471098036e                  	{ s32i	a6, a3, 120; addx4	a3, a7, a2 }
    1270:	162362                                    	l32i	a6, a3, 88
    1273:	200706471481036e                  	{ l32i	a6, a3, 120; ae_cvt48a32	aed1, a6 }

0000127b <sbc_reconstruction+0x46b>:
    127b:	2007264508e0e22e                  	{ or	a5, a2, a2; ae_cvt48a32	aed0, a6 }
    1283:	0ead057b08e40cd0c58081380401601f 	{ ae_subsq56s	aed0, aed1, aed0; nop; nop; nop }
    1293:	0ead217b0d440cd0c50061380c01015f 	{ nop; nop; nop; ae_roundsq32f48sym	aed0, aed0 }
    12a3:	0ead257b0d240cd0c400813b0c016f0f 	{ ae_trunca32q48	a15, aed0; nop; nop; nop }
    12b3:	1e63f2                                    	s32i	a15, a3, 120

000012b6 <sbc_reconstruction+0x4a6>:
    12b6:	10c442                                    	addi	a4, a4, 16
    12b9:	1128                                    	l32i.n	a2, a1, 4
    12bb:	230720bb1be5542e                  	{ bltui.w15	a4, 0x100, 1209 <sbc_reconstruction+0x3f9>; nop }
    12c3:	010546                                    	j	16dc <sbc_reconstruction+0x8cc>

000012c6 <sbc_reconstruction+0x4b6>:
    12c6:	412e16                                    	beqz	a14, 16dc <sbc_reconstruction+0x8cc>
    12c9:	061572                                    	l16ui	a7, a5, 12
    12cc:	902d50                                    	addx2	a2, a13, a5
    12cf:	56a442                                    	movi	a4, 0x456
    12d2:	060c                                    	movi.n	a6, 0
    12d4:	224a                                    	add.n	a2, a2, a4
    12d6:	58c532                                    	addi	a3, a5, 88
    12d9:	11f7f0                                    	slli	a15, a7, 1
    12dc:	2129                                    	s32i.n	a2, a1, 8
    12de:	3179                                    	s32i.n	a7, a1, 12
    12e0:	1128                                    	l32i.n	a2, a1, 4
    12e2:	4171d0                                    	srli	a7, a13, 1
    12e5:	3148                                    	l32i.n	a4, a1, 12
    12e7:	2198                                    	l32i.n	a9, a1, 8
    12e9:	230740a40c25f2ce                  	{ blti.w15	a2, 1, 14ac <sbc_reconstruction+0x69c>; nop }
    12f1:	246a                                    	add.n	a2, a4, a6
    12f3:	556d07                                    	bbci	a13, 0, 134c <sbc_reconstruction+0x53c>
    12f6:	495d                                    	ae_l16ui.n	a5, a9, 0
    12f8:	465a                                    	add.n	a4, a6, a5
    12fa:	525a                                    	add.n	a5, a2, a5
    12fc:	a0b430                                    	addx4	a11, a4, a3
    12ff:	a0a530                                    	addx4	a10, a5, a3
    1302:	00052a4000c160be                  	{ ae_l32m.i	aed0, a11, 0; ae_l32m.i	aed1, a10, 0 }
    130a:	0ea4c407f0e299d0c58080080da2600f 	{ ae_subsq56s	aed0, aed0, aed1; addi	a9, a9, -2; ae_addsq56s	aed2, aed0, aed1; nop }
    131a:	0ead217b0d540cd0c57c61380c01015f 	{ nop; nop; nop; ae_roundsq32f48sym	aed31, aed2 }
    132a:	0ead217b0d250cd0c400613b0c0168ff 	{ ae_trunca32q48	a8, aed31; nop; nop; ae_roundsq32f48sym	aed0, aed0 }
    133a:	000003f3038300fe56a3030024601b8f 	{ s32i	a8, a11, 0; ae_trunca32q48	a12, aed0; nop }
    134a:	0ac9                                    	s32i.n	a12, a10, 0
    134c:	fec982                                    	addi	a8, a9, -2
    134f:	159716                                    	beqz	a7, 14ac <sbc_reconstruction+0x69c>
    1352:	494d                                    	ae_l16ui.n	a4, a9, 0
    1354:	c70b                                    	addi.n	a12, a7, -1
    1356:	485d                                    	ae_l16ui.n	a5, a8, 0
    1358:	fec892                                    	addi	a9, a8, -2
    135b:	a64a                                    	add.n	a10, a6, a4
    135d:	424a                                    	add.n	a4, a2, a4
    135f:	a08430                                    	addx4	a8, a4, a3
    1362:	a07a30                                    	addx4	a7, a10, a3
    1365:	0005284000c1607e                  	{ ae_l32m.i	aed0, a7, 0; ae_l32m.i	aed1, a8, 0 }
    136d:	0ea560262d2325f8c108200f0900b65f 	{ add	a11, a6, a5; add	a5, a2, a5; ae_subsq56s	aed0, aed0, aed1; ae_addsq56s	aed2, aed0, aed1 }
    137d:	0ead61665d35b3d0c504613f1001466f 	{ or	a4, a6, a6; addx4	a11, a11, a3; nop; ae_roundsq32f48sym	aed1, aed2 }
    138d:	a0a530                                    	addx4	a10, a5, a3
    1390:	086d                                    	mov.n	a6, a8
    1392:	230720c318e5747e                  	{ nop; nop }
    139a:	0020f0                                    	nop
    139d:	9f9c76                                    	loopnez	a12, 1440 <sbc_reconstruction+0x630>
    13a0:	0eace160052698d0c400613b0c01651f 	{ ae_trunca32q48	a5, aed1; l16ui	a8, a9, 0; nop; ae_roundsq32f48sym	aed0, aed0 }
    13b0:	000003f3038300fad6a303002460175f 	{ s32i	a5, a7, 0; ae_trunca32q48	a5, aed0; nop }
    13c0:	2502a9c0009f165e                  	{ s32i	a5, a6, 0; addi	a5, a9, -2 }
    13c8:	00052a4000c160be                  	{ ae_l32m.i	aed0, a11, 0; ae_l32m.i	aed1, a10, 0 }
    13d0:	0ea560063d232890c500200f09a1648f 	{ add	a6, a4, a8; add	a7, a2, a8; ae_addsq56s	aed1, aed0, aed1; ae_subsq56s	aed0, aed0, aed1 }
    13e0:	0ead6166352c73d0c404613a0401763f 	{ addx4	a7, a6, a3; addx4	a6, a7, a3; nop; ae_roundsq32f48sym	aed1, aed1 }
    13f0:	0eace160052658d0c400613b0c01691f 	{ ae_trunca32q48	a9, aed1; l16ui	a8, a5, 0; nop; ae_roundsq32f48sym	aed0, aed0 }
    1400:	000003f3038300fcd6a3030024601b9f 	{ s32i	a9, a11, 0; ae_trunca32q48	a9, aed0; nop }
    1410:	2902a5c0009f1a9e                  	{ s32i	a9, a10, 0; addi	a9, a5, -2 }
    1418:	0005264000c1607e                  	{ ae_l32m.i	aed0, a7, 0; ae_l32m.i	aed1, a6, 0 }
    1420:	0ea5600645232890c500200f09a1548f 	{ add	a5, a4, a8; add	a8, a2, a8; ae_addsq56s	aed1, aed0, aed1; ae_subsq56s	aed0, aed0, aed1 }
    1430:	0ead6166552c83d0c404613a0401b53f 	{ addx4	a11, a5, a3; addx4	a10, a8, a3; nop; ae_roundsq32f48sym	aed1, aed1 }

00001440 <sbc_reconstruction+0x630>:
    1440:	0ead217b0d240cd0c400613b0c01621f 	{ ae_trunca32q48	a2, aed1; nop; nop; ae_roundsq32f48sym	aed0, aed0 }
    1450:	000003f3038300fcd6a303002460172f 	{ s32i	a2, a7, 0; ae_trunca32q48	a9, aed0; nop }
    1460:	0699                                    	s32i.n	a9, a6, 0
    1462:	00052a4708de60be                  	{ ae_l32m.i	aed29, a11, 0; ae_l32m.i	aed30, a10, 0 }
    146a:	0ea54427b0e744d0c4809df71100c2df 	{ ae_addsq56s	aed2, aed29, aed30; or	a6, a4, a4; ae_subsq56s	aed0, aed29, aed30; nop }
    147a:	0ead217b0d540cd0c57c61380c01015f 	{ nop; nop; nop; ae_roundsq32f48sym	aed31, aed2 }
    148a:	0ead217b0d250cd0c400613b0c016cff 	{ ae_trunca32q48	a12, aed31; nop; nop; ae_roundsq32f48sym	aed0, aed0 }
    149a:	000003f3038300fad6a3030024601bcf 	{ s32i	a12, a11, 0; ae_trunca32q48	a5, aed0; nop }
    14aa:	0a59                                    	s32i.n	a5, a10, 0

000014ac <sbc_reconstruction+0x69c>:
    14ac:	66fa                                    	add.n	a6, a6, a15
    14ae:	2307009b0fc576ee                  	{ bltu.w15	a6, a14, 12e0 <sbc_reconstruction+0x4d0>; nop }
    14b6:	008886                                    	j	16dc <sbc_reconstruction+0x8cc>

000014b9 <sbc_reconstruction+0x6a9>:
    14b9:	21fd16                                    	beqz	a13, 16dc <sbc_reconstruction+0x8cc>
    14bc:	0615e2                                    	l16ui	a14, a5, 12
    14bf:	902c50                                    	addx2	a2, a12, a5
    14c2:	56a432                                    	movi	a3, 0x456
    14c5:	060c                                    	movi.n	a6, 0
    14c7:	f23a                                    	add.n	a15, a2, a3
    14c9:	58c542                                    	addi	a4, a5, 88
    14cc:	1128                                    	l32i.n	a2, a1, 4
    14ce:	11bef0                                    	slli	a11, a14, 1
    14d1:	fecf72                                    	addi	a7, a15, -2
    14d4:	220760a00c0552ce                  	{ blti.w15	a2, 1, 15c0 <sbc_reconstruction+0x7b0>; nop }
    14dc:	4f3d                                    	ae_l16ui.n	a3, a15, 0
    14de:	2e6a                                    	add.n	a2, a14, a6
    14e0:	230720c318e5747e                  	{ nop; nop }
    14e8:	808630                                    	add	a8, a6, a3
    14eb:	803230                                    	add	a3, a2, a3
    14ee:	a09840                                    	addx4	a9, a8, a4
    14f1:	a05340                                    	addx4	a5, a3, a4
    14f4:	0005254000c1609e                  	{ ae_l32m.i	aed0, a9, 0; ae_l32m.i	aed1, a5, 0 }
    14fc:	0ea4e427f462cad0c50080090500073f 	{ l16ui	a3, a7, 0; addi	a10, a12, -2; ae_subsq56s	aed0, aed0, aed1; nop }
    150c:	0ead217b0c650cd0c400613a1401f77f 	{ addi	a7, a7, -2; nop; nop; ae_roundsq32f48sym	aed0, aed0 }
    151c:	220740a410057c2e                  	{ bltui.w15	a12, 2, 15ad <sbc_reconstruction+0x79d>; nop }
    1524:	808630                                    	add	a8, a6, a3
    1527:	803230                                    	add	a3, a2, a3
    152a:	a09840                                    	addx4	a9, a8, a4
    152d:	3f9a76                                    	loopnez	a10, 1570 <sbc_reconstruction+0x760>
    1530:	0eace56005267ad0c400813b0c01680f 	{ ae_trunca32q48	a8, aed0; l16ui	a10, a7, 0; nop; nop }
    1540:	1506a3400099058e                  	{ s32i	a8, a5, 0; addx4	a5, a3, a4 }
    1548:	0005254008c0609e                  	{ ae_l32m.i	aed1, a9, 0; ae_l32m.i	aed0, a5, 0 }
    1550:	0ea564261d232ad0c5008107090086af 	{ add	a8, a6, a10; add	a3, a2, a10; ae_subsq56s	aed0, aed1, aed0; nop }
    1560:	0eace167f52277d0c400613a0401984f 	{ addx4	a9, a8, a4; addi	a7, a7, -2; nop; ae_roundsq32f48sym	aed0, aed0 }

00001570 <sbc_reconstruction+0x760>:
    1570:	0ead257b0d240cd0c400813b0c01620f 	{ ae_trunca32q48	a2, aed0; nop; nop; nop }
    1580:	0529                                    	s32i.n	a2, a5, 0
    1582:	a05340                                    	addx4	a5, a3, a4
    1585:	0005254718c1609e                  	{ ae_l32m.i	aed31, a9, 0; ae_l32m.i	aed1, a5, 0 }
    158d:	0ead057b08e50cd0c58081380c0160ff 	{ ae_subsq56s	aed0, aed31, aed1; nop; nop; nop }
    159d:	0ead217b0d440cd0c50061380c01015f 	{ nop; nop; nop; ae_roundsq32f48sym	aed0, aed0 }

000015ad <sbc_reconstruction+0x79d>:
    15ad:	0ead257b0d240cd0c400813b0c01620f 	{ ae_trunca32q48	a2, aed0; nop; nop; nop }
    15bd:	006522                                    	s32i	a2, a5, 0
    15c0:	66ba                                    	add.n	a6, a6, a11
    15c2:	1128                                    	l32i.n	a2, a1, 4
    15c4:	2207009f0be556de                  	{ bltu.w15	a6, a13, 14d1 <sbc_reconstruction+0x6c1>; nop }
    15cc:	004306                                    	j	16dc <sbc_reconstruction+0x8cc>

000015cf <sbc_reconstruction+0x7bf>:
    15cf:	00000000                                 ......

000015d5 <sbc_reconstruction+0x7c5>:
    15d5:	103d16                                    	beqz	a13, 16dc <sbc_reconstruction+0x8cc>
    15d8:	0615e2                                    	l16ui	a14, a5, 12
    15db:	902c50                                    	addx2	a2, a12, a5
    15de:	56a432                                    	movi	a3, 0x456
    15e1:	060c                                    	movi.n	a6, 0
    15e3:	f23a                                    	add.n	a15, a2, a3
    15e5:	58c542                                    	addi	a4, a5, 88
    15e8:	1128                                    	l32i.n	a2, a1, 4
    15ea:	11bef0                                    	slli	a11, a14, 1
    15ed:	fecf72                                    	addi	a7, a15, -2
    15f0:	220760a00805f2ce                  	{ blti.w15	a2, 1, 16d0 <sbc_reconstruction+0x8c0>; nop }
    15f8:	4f3d                                    	ae_l16ui.n	a3, a15, 0
    15fa:	2e6a                                    	add.n	a2, a14, a6
    15fc:	863a                                    	add.n	a8, a6, a3
    15fe:	923a                                    	add.n	a9, a2, a3
    1600:	a05840                                    	addx4	a5, a8, a4
    1603:	a09940                                    	addx4	a9, a9, a4
    1606:	0005294000c1605e                  	{ ae_l32m.i	aed0, a5, 0; ae_l32m.i	aed1, a9, 0 }
    160e:	0ea4e407f462cad0c500800905a0073f 	{ l16ui	a3, a7, 0; addi	a10, a12, -2; ae_addsq56s	aed0, aed0, aed1; nop }
    161e:	0ead217b0c650cd0c400613a1401f77f 	{ addi	a7, a7, -2; nop; nop; ae_roundsq32f48sym	aed0, aed0 }
    162e:	230740a410055c2e                  	{ bltui.w15	a12, 2, 16bd <sbc_reconstruction+0x8ad>; nop }
    1636:	923a                                    	add.n	a9, a2, a3
    1638:	363a                                    	add.n	a3, a6, a3
    163a:	a09940                                    	addx4	a9, a9, a4
    163d:	3f9a76                                    	loopnez	a10, 1680 <sbc_reconstruction+0x870>
    1640:	0eace56005267ad0c400813b0c01680f 	{ ae_trunca32q48	a8, aed0; l16ui	a10, a7, 0; nop; nop }
    1650:	1506a3400099058e                  	{ s32i	a8, a5, 0; addx4	a5, a3, a4 }
    1658:	0005294008c0605e                  	{ ae_l32m.i	aed1, a5, 0; ae_l32m.i	aed0, a9, 0 }
    1660:	0ea5640645232ad0c500810709a036af 	{ add	a3, a6, a10; add	a8, a2, a10; ae_addsq56s	aed0, aed1, aed0; nop }
    1670:	0eace167f52277d0c400613a0401984f 	{ addx4	a9, a8, a4; addi	a7, a7, -2; nop; ae_roundsq32f48sym	aed0, aed0 }

00001680 <sbc_reconstruction+0x870>:
    1680:	0ead257b0d240cd0c400813b0c01620f 	{ ae_trunca32q48	a2, aed0; nop; nop; nop }
    1690:	0529                                    	s32i.n	a2, a5, 0
    1692:	a05340                                    	addx4	a5, a3, a4
    1695:	0005294718c1605e                  	{ ae_l32m.i	aed31, a5, 0; ae_l32m.i	aed1, a9, 0 }
    169d:	0ead057b08e50cd0c48081380801c0ff 	{ ae_addsq56s	aed0, aed31, aed1; nop; nop; nop }
    16ad:	0ead217b0d440cd0c50061380c01015f 	{ nop; nop; nop; ae_roundsq32f48sym	aed0, aed0 }

000016bd <sbc_reconstruction+0x8ad>:
    16bd:	0ead257b0d240cd0c400813b0c01620f 	{ ae_trunca32q48	a2, aed0; nop; nop; nop }
    16cd:	006522                                    	s32i	a2, a5, 0
    16d0:	66ba                                    	add.n	a6, a6, a11
    16d2:	1128                                    	l32i.n	a2, a1, 4
    16d4:	2207009f0be5b6de                  	{ bltu.w15	a6, a13, 15ed <sbc_reconstruction+0x7dd>; nop }
    16dc:	f01d                                    	retw.n

000016de <sbc_reconstruction+0x8ce>:
	...

000016e0 <SBC_synthesis4_per_channel>:
    16e0:	010136                                    	entry	a1, 128
    16e3:	0b0c                                    	movi.n	a11, 0
    16e5:	031452                                    	l16ui	a5, a4, 6
    16e8:	622b20                                    	saltu	a2, a11, a2
    16eb:	fa5681                                    	l32r	a8, 44 (2590 <SBC_cos_coef_4>)
    16ee:	1122c0                                    	slli	a2, a2, 4
    16f1:	bd0c                                    	movi.n	a13, 11
    16f3:	d92c                                    	movi.n	a9, 45
    16f5:	599516                                    	beqz	a5, 1c92 <SBC_synthesis4_per_channel+0x5b2>
    16f8:	0bd472                                    	addmi	a7, a4, 0xb00
    16fb:	0bd462                                    	addmi	a6, a4, 0xb00
    16fe:	115970                                    	slli	a5, a9, 9
    1701:	242a                                    	add.n	a2, a4, a2
    1703:	fec3a2                                    	addi	a10, a3, -2
    1706:	40c8b2                                    	addi	a11, a8, 64
    1709:	a0a0c2                                    	movi	a12, 160
    170c:	08a092                                    	movi	a9, 8
    170f:	11dd70                                    	slli	a13, a13, 9
    1712:	00a032                                    	movi	a3, 0
    1715:	0f0282421cb6e55e                  	{ ae_cvtp24a16x2.ll	aed11, a5, a5; addi	a14, a2, 88 }
    171d:	156142                                    	s32i	a4, a1, 84
    1720:	146172                                    	s32i	a7, a1, 80
    1723:	116162                                    	s32i	a6, a1, 68
    1726:	1361b2                                    	s32i	a11, a1, 76
    1729:	178076                                    	loop	a0, 1744 <SBC_synthesis4_per_channel+0x64>
    172c:	030100                                    	rsr.lend	a0
    172f:	130000                                    	wsr.lbeg	a0
    1732:	fa4601                                    	l32r	a0, 4c (1c8f <SBC_synthesis4_per_channel+0x5af>)
    1735:	0020f0                                    	nop
    1738:	130100                                    	wsr.lend	a0
    173b:	002000                                    	isync
    173e:	030200                                    	rsr.lcount	a0
    1741:	01c002                                    	addi	a0, a0, 1
    1744:	112122                                    	l32i	a2, a1, 68
    1747:	10cef2                                    	addi	a15, a14, 16
    174a:	0d0740400cbb4c0e                  	{ ae_movi	aed1, 0; ae_movi	aed27, 0 }
    1752:	1261e2                                    	s32i	a14, a1, 72
    1755:	085d                                    	mov.n	a5, a8
    1757:	1c07404000e9116e                  	{ ae_mov	aed0, aed1; ae_mov	aed9, aed1 }
    175f:	f3f7f0                                    	wur.ae_cend0	a15
    1762:	521262                                    	l16ui	a6, a2, 164
    1765:	1221f2                                    	l32i	a15, a1, 72
    1768:	1c07404400f5116e                  	{ ae_mov	aed16, aed1; ae_mov	aed21, aed1 }
    1770:	f3f6e0                                    	wur.ae_cbegin0	a14
    1773:	282742                                    	l32i	a4, a7, 160
    1776:	166132                                    	s32i	a3, a1, 88
    1779:	1c076040107a106e                  	{ extui	a2, a6, 0, 1; ae_mov	aed26, aed1 }
    1781:	11e690                                    	slli	a14, a6, 7
    1784:	1d063ec508efdb2e                  	{ movnez	a5, a11, a2; ae_truncp24a32x2	aed15, a14, a13 }
    178c:	3406b54010c8d9fe                  	{ ae_l32x2f24.xc	aed2, a15, a9; or	a3, a5, a5 }
    1794:	39052fc01ca5b93e                  	{ ae_l32x2f24.ip	aed3, a3, 8; ae_l32x2f24.xc	aed5, a15, a9 }
    179c:	0e1d62463d3d6416022b2c1509ef055f 	{ ae_l32x2f24.i	aed4, a5, 8; addx4	a7, a6, a4; ae_mulf32s.lh	aed2, aed15, aed15; ae_mulaafd32s.hh.ll	aed1, aed2, aed3 }
    17ac:	0e9527e32d213dd0c400980f10a40d5f 	{ ae_l32x2f24.i	aed28, a5, 16; ae_l32x2f24.i	aed5, a3, 24; ae_mulaafd32s.hh.ll	aed1, aed5, aed4; nop }
    17bc:	140503c710e70d5e                  	{ ae_l32x2f24.i	aed30, a5, 24; ae_l32x2f24.i	aed7, a3, 32 }
    17c4:	0a02c3400c82d11e                  	{ ae_round24x2f48ssym	aed1, aed1, aed1; addi	a11, a3, 8 }
    17cc:	0702e74008ca507e                  	{ ae_s32f24.l.i	aed1, a7, 0; addi	a7, a7, 40 }
    17d4:	0e04284708c0c9fe                  	{ ae_l32x2f24.xc	aed29, a15, a9; movi	a14, 8 }
    17dc:	1c829adc3fbcf9fe                  	{ ae_l32x2f24.xc	aed31, a15, a9; nop; ae_mulaafd32s.hh.ll	aed27, aed29, aed28 }
    17e4:	0d1c                                    	movi.n	a13, 16
    17e6:	0e54f7e0091a6527858038d81ffead2f 	{ ae_slaaq56	aed2, aed2, a13; addi	a5, a6, 1; ae_mulaafd32s.hh.ll	aed27, aed31, aed30; ae_mov	aed30, aed0 }
    17f6:	230740c00c85fbbe                  	{ ae_round24x2f48ssym	aed1, aed27, aed27; nop }
    17fe:	0702e74008ca507e                  	{ ae_s32f24.l.i	aed1, a7, 0; addi	a7, a7, 40 }
    1806:	0202a74110cad9fe                  	{ ae_l32x2f24.xc	aed6, a15, a9; addi	a3, a7, 40 }
    180e:	1c829a0438c589fe                  	{ ae_l32x2f24.xc	aed8, a15, a9; nop; ae_mulaafd32s.hh.ll	aed0, aed6, aed5 }
    1816:	1c81ba0421078b8e                  	{ addi	a8, a11, 8; nop; ae_mulaafd32s.hh.ll	aed0, aed8, aed7 }
    181e:	0602c3400c8ac00e                  	{ ae_round24x2f48ssym	aed1, aed0, aed0; addi	a6, a3, 40 }
    1826:	000003f30a8300fbd0e303002860907f 	{ ae_s32f24.l.i	aed1, a7, 0; ae_trunca32q48	a7, aed2; nop }
    1836:	14052bc210ccc9fe                  	{ ae_l32x2f24.xc	aed10, a15, a9; ae_l32x2f24.i	aed12, a11, 32 }
    183e:	0e94e7fa7527f9d0c400984d094c5dbf 	{ ae_l32x2f24.i	aed13, a11, 40; ae_l32x2f24.xc	aed14, a15, a9; ae_mulaafd32s.hh.ll	aed9, aed10, aed12; nop }
    184e:	1c81ba4c21cd88be                  	{ addi	a11, a8, 8; nop; ae_mulaafd32s.hh.ll	aed9, aed14, aed13 }
    1856:	0802cb400c82c99e                  	{ ae_round24x2f48ssym	aed1, aed9, aed9; addi	a8, a11, 8 }
    185e:	0304614008c0503e                  	{ ae_s32f24.l.i	aed1, a3, 0; movi	a3, 1 }
    1866:	14052bc408d2c9fe                  	{ ae_l32x2f24.xc	aed17, a15, a9; ae_l32x2f24.i	aed18, a11, 32 }
    186e:	0e94e7faa527f9d0c40098870a32d1bf 	{ ae_l32x2f24.i	aed19, a11, 40; ae_l32x2f24.xc	aed20, a15, a9; ae_mulaafd32s.hh.ll	aed16, aed17, aed18; nop }
    187e:	1c81ba842293888e                  	{ addi	a8, a8, 8; nop; ae_mulaafd32s.hh.ll	aed16, aed20, aed19 }
    1886:	b4ca                                    	add.n	a11, a4, a12
    1888:	f6c592                                    	addi	a9, a5, -10
    188b:	83b420                                    	moveqz	a11, a4, a2
    188e:	0d02c8400c82e00e                  	{ ae_round24x2f48ssym	aed1, aed16, aed16; addi	a12, a8, 8 }
    1896:	939590                                    	movnez	a9, a5, a9
    1899:	0d02ec4008c2406e                  	{ ae_s32f24.l.i	aed1, a6, 0; addi	a12, a12, 8 }
    18a1:	140528c510d7cefe                  	{ ae_l32x2f24.xc	aed22, a15, a14; ae_l32x2f24.i	aed23, a8, 32 }
    18a9:	0e94e7facd27fed0c40098af0ad7198f 	{ ae_l32x2f24.i	aed24, a8, 40; ae_l32x2f24.xc	aed25, a15, a14; ae_mulaafd32s.hh.ll	aed21, aed22, aed23; nop }
    18b9:	1c833aaa2f38823e                  	{ xor	a8, a2, a3; nop; ae_mulaafd32s.hh.ll	aed21, aed25, aed24 }
    18c1:	28c622                                    	addi	a2, a6, 40
    18c4:	f9e161                                    	l32r	a6, 48 (2950 <SBC_proto_coef_4>)
    18c7:	0302cc400c82f55e                  	{ ae_round24x2f48ssym	aed1, aed21, aed21; addi	a3, a12, 8 }
    18cf:	176192                                    	s32i	a9, a1, 92
    18d2:	0302e24008ca402e                  	{ ae_s32f24.l.i	aed1, a2, 0; addi	a2, a2, 40 }
    18da:	14052cc008dbcefe                  	{ ae_l32x2f24.xc	aed1, a15, a14; ae_l32x2f24.i	aed27, a12, 32 }
    18e2:	a04760                                    	addx4	a4, a7, a6
    18e5:	1c831ad32c3bc9ce                  	{ ae_l32x2f24.i	aed28, a12, 40; nop; ae_mulaafd32s.hh.ll	aed26, aed1, aed27 }
    18ed:	1406a84708dbcefe                  	{ ae_l32x2f24.xc	aed29, a15, a14; addx4	a13, a8, a4 }
    18f5:	1c833ad02fbccdde                  	{ or	a12, a13, a13; nop; ae_mulaafd32s.hh.ll	aed26, aed29, aed28 }
    18fd:	738b                                    	addi.n	a7, a3, 8
    18ff:	0402a24000ca31ce                  	{ ae_l32f24.ip	aed0, a12, 4; addi	a5, a2, 40 }
    1907:	0902cc401481eaae                  	{ ae_round24x2f48ssym	aed2, aed26, aed26; addi	a8, a12, 4 }
    190f:	890c                                    	movi.n	a9, 8
    1911:	0702e84010c1402e                  	{ ae_s32f24.l.i	aed2, a2, 0; addi	a6, a8, 4 }
    1919:	140527c010dfcefe                  	{ ae_l32x2f24.xc	aed2, a15, a14; ae_l32x2f24.i	aed31, a7, 32 }
    1921:	0e94e7e0210363d0c58098f11c5fb6ff 	{ ae_l32x2f24.xc	aed6, a15, a14; addi	a3, a6, 4; ae_mulaafd32s.hh.ll	aed30, aed2, aed31; nop }
    1931:	0f02834008e1157e                  	{ ae_l32x2f24.i	aed1, a7, 40; addi	a15, a3, 4 }
    1939:	0e54e3e02523f700470838f010c1c5df 	{ ae_l32f24.i	aed7, a13, 4; addi	a7, a15, 4; ae_mulaafd32s.hh.ll	aed30, aed6, aed1; ae_movi	aed1, 0 }
    1949:	0e54f7b7147ed22004043c3804c0172f 	{ addi	a2, a7, 4; ae_l32f24.i	aed2, a13, 8; ae_sel16i.n	aed6, aed0, aed7, 0; ae_mov	aed0, aed1 }
    1959:	0f02c2401c81eeee                  	{ ae_round24x2f48ssym	aed3, aed30, aed30; addi	a14, a2, 4 }
    1961:	0502ee4018c1405e                  	{ ae_s32f24.l.i	aed3, a5, 0; addi	a4, a14, 4 }
    1969:	0f02a44708c205ee                  	{ ae_l32f24.i	aed29, a14, 20; addi	a14, a4, 8 }
    1971:	0502ae4710c1054e                  	{ ae_l32f24.i	aed30, a4, 20; addi	a4, a14, 4 }
    1979:	0f02a44718c104ee                  	{ ae_l32f24.i	aed31, a14, 16; addi	a14, a4, 4 }
    1981:	0502ae4018c1044e                  	{ ae_l32f24.i	aed3, a4, 16; addi	a4, a14, 4 }
    1989:	0f02a44100c104ee                  	{ ae_l32f24.i	aed4, a14, 16; addi	a14, a4, 4 }
    1991:	0502ae4200c1044e                  	{ ae_l32f24.i	aed8, a4, 16; addi	a4, a14, 4 }
    1999:	0f02a44208c104ee                  	{ ae_l32f24.i	aed9, a14, 16; addi	a14, a4, 4 }
    19a1:	0502ae4210c1044e                  	{ ae_l32f24.i	aed10, a4, 16; addi	a4, a14, 4 }
    19a9:	0e94e7a025234ed0c5009c5011299def 	{ ae_l32f24.i	aed14, a14, 20; addi	a14, a4, 4; ae_sel16i.n	aed9, aed9, aed10, 0; nop }
    19b9:	0502ae4410c1054e                  	{ ae_l32f24.i	aed18, a4, 20; addi	a4, a14, 4 }
    19c1:	0f02a44108c205ee                  	{ ae_l32f24.i	aed5, a14, 20; addi	a14, a4, 8 }
    19c9:	0502ae4300c1054e                  	{ ae_l32f24.i	aed12, a4, 20; addi	a4, a14, 4 }
    19d1:	0f02a44308c104ee                  	{ ae_l32f24.i	aed13, a14, 16; addi	a14, a4, 4 }
    19d9:	0502ae4318c1044e                  	{ ae_l32f24.i	aed15, a4, 16; addi	a4, a14, 4 }
    19e1:	0f02a44408c104ee                  	{ ae_l32f24.i	aed17, a14, 16; addi	a14, a4, 4 }
    19e9:	0502ae4500c1044e                  	{ ae_l32f24.i	aed20, a4, 16; addi	a4, a14, 4 }
    19f1:	0f02a44400c104ee                  	{ ae_l32f24.i	aed16, a14, 16; addi	a14, a4, 4 }
    19f9:	0502ae4418c1044e                  	{ ae_l32f24.i	aed19, a4, 16; addi	a4, a14, 4 }
    1a01:	0f02a44508c105ee                  	{ ae_l32f24.i	aed21, a14, 20; addi	a14, a4, 4 }
    1a09:	0502ae4510c1054e                  	{ ae_l32f24.i	aed22, a4, 20; addi	a4, a14, 4 }
    1a11:	0f02a44518c205ee                  	{ ae_l32f24.i	aed23, a14, 20; addi	a14, a4, 8 }
    1a19:	0b06bb4600c9154e                  	{ ae_l32f24.i	aed24, a4, 20; or	a4, a11, a11 }
    1a21:	3504b24614bbb94e                  	{ ae_l32x2f24.ip	aed26, a4, 8; ae_l32f24.i	aed27, a2, 20 }
    1a29:	0e94e7e0452342d0c50098001346c7df 	{ ae_l32f24.i	aed7, a13, 12; addi	a2, a4, 8; ae_mulaafd32s.hh.ll	aed0, aed26, aed6; nop }
    1a39:	3404974100fc9f1e                  	{ ae_s64.i	aed4, a1, 24; ae_l32f24.i	aed28, a7, 20 }
    1a41:	0e54f3a0452327008508fcdc135c471f 	{ ae_s64.i	aed5, a1, 16; addi	a7, a2, 8; ae_sel16i.n	aed26, aed28, aed27, 0; ae_sel16i	aed2, aed2, aed7, 0 }
    1a51:	32049c4210e515be                  	{ ae_l32x2f24.i	aed10, a11, 8; ae_l32f24.i	aed5, a12, 12 }
    1a59:	35049c4708e48f1e                  	{ ae_s64.i	aed29, a1, 56; ae_l32f24.i	aed4, a12, 16 }
    1a61:	0e14e3b7f5376456012b3c2610859f1f 	{ ae_s64.i	aed30, a1, 48; ae_l32f24.i	aed30, a6, 16; ae_sel16i.n	aed4, aed5, aed4, 0; ae_mulaafd32s.hh.ll	aed0, aed10, aed2 }
    1a71:	0702874718e2871e                  	{ ae_s64.i	aed31, a1, 40; addi	a6, a7, 8 }
    1a79:	13052bc718dc048e                  	{ ae_l32f24.i	aed31, a8, 16; ae_l32x2f24.i	aed28, a11, 16 }
    1a81:	0e54d3e043e3680105ba58011b8420ef 	{ ae_sel16i	aed2, aed31, aed30, 0; addi	a8, a6, 8; ae_mulaafd32s.hh.ll	aed0, aed28, aed4; ae_sel16i	aed4, aed14, aed18, 0 }
    1a91:	0ead2507f5211fd0c40081270381914f 	{ ae_l32x2f24.i	aed18, a4, 32; ae_l64.i	aed30, a1, 56; ae_mov	aed28, aed1; nop }
    1aa1:	1c831ae32e5afe1e                  	{ ae_l64.i	aed31, a1, 48; nop; ae_mulaafd32s.hh.ll	aed28, aed18, aed26 }
    1aa9:	3f002f4708de04fe                  	{ ae_l32f24.i	aed29, a15, 16; ae_sel16i	aed30, aed30, aed31, 0 }
    1ab1:	0f02884018e2931e                  	{ ae_s64.i	aed3, a1, 32; addi	a15, a8, 8 }
    1ab9:	150522c210df043e                  	{ ae_l32f24.i	aed10, a3, 16; ae_l32x2f24.i	aed31, a2, 32 }
    1ac1:	0e54f3e04523fc06842bb8e51bfe8dbf 	{ ae_l32x2f24.i	aed14, a11, 24; addi	a12, a15, 8; ae_mulaafd32s.hh.ll	aed28, aed31, aed30; ae_sel16i	aed26, aed10, aed29, 0 }
    1ad1:	0e9527e5ed211fd0c400980519c2c14f 	{ ae_l32x2f24.i	aed3, a4, 24; ae_l64.i	aed29, a1, 40; ae_mulaafd32s.hh.ll	aed0, aed14, aed2; nop }
    1ae1:	0e9527e435217dd0c4009807007a9f1f 	{ ae_l64.i	aed30, a1, 32; ae_l32x2f24.i	aed6, a7, 32; ae_mulaafd32s.hh.ll	aed0, aed3, aed26; nop }
    1af1:	0e54f3a04523c4078435fcf5195d8f1f 	{ ae_l64.i	aed14, a1, 24; addi	a4, a12, 8; ae_sel16i.n	aed10, aed29, aed30, 0; ae_sel16i	aed30, aed13, aed15, 0 }
    1b01:	0e5517e43bf86d22048438e110ca608f 	{ ae_sel16i	aed6, aed14, aed8, 0; ae_l32x2f24.i	aed7, a6, 32; ae_mulaafd32s.hh.ll	aed28, aed6, aed10; ae_mov	aed8, aed1 }
    1b11:	0e5533e535214d03844698e700e6d9ff 	{ ae_l32x2f24.i	aed27, a15, 32; ae_l32x2f24.i	aed6, a4, 40; ae_mulaafd32s.hh.ll	aed28, aed7, aed6; ae_sel16i	aed14, aed17, aed20, 0 }
    1b21:	0e54f3e045234206c456d80f1364831f 	{ ae_l64.i	aed18, a1, 16; addi	a2, a4, 8; ae_mulaafd32s.hh.ll	aed1, aed27, aed4; ae_sel16i	aed27, aed21, aed22, 0 }
    1b31:	0e9503a4fbe0cdd0c5007c9812b040cf 	{ ae_sel16i	aed4, aed18, aed12, 0; ae_l32x2f24.i	aed31, a12, 32; ae_sel16i.n	aed21, aed16, aed19, 0; ae_roundsq32f48sym	aed0, aed0 }
    1b41:	0e54f3e045232203045f180d0be4d52f 	{ ae_l32x2f24.i	aed7, a2, 40; addi	a2, a2, 8; ae_mulaafd32s.hh.ll	aed1, aed31, aed4; ae_sel16i	aed12, aed23, aed24, 0 }
    1b51:	0e9ce797d466e5d0c4008c00040b1e5f 	{ addi	a5, a14, 4; ae_l32f24.i	aed26, a14, 20; ae_mulq32sp16s.l	aed0, aed0, aed11; nop }
    1b61:	0302824708e2052e                  	{ ae_l32x2f24.i	aed29, a2, 40; addi	a2, a2, 8 }
    1b69:	0e54e3e0253355500700184213bbdd5f 	{ ae_l32f24.i	aed31, a5, 20; addi	a5, a5, 4; ae_mulaafd32s.hh.ll	aed8, aed29, aed27; ae_slaisq56s	aed0, aed0, 2 }
    1b79:	0ead21641d214df0c200013d0001558f 	{ ae_l32x2f24.i	aed5, a8, 32; ae_l32x2f24.i	aed3, a4, 32; nop; ae_roundsp16q48x2sym	aed0, aed0, aed0 }
    1b89:	0e14ebe0452b222e382b380d087e5d2f 	{ ae_l32x2f24.i	aed13, a2, 40; addi	a2, a2, 8; ae_mulaafd32s.hh.ll	aed1, aed3, aed30; ae_mulaafd32s.hh.ll	aed28, aed5, aed9 }
    1b99:	0e14eff77d27556e112b380a10ce58ef 	{ ae_l32f24.i	aed25, a14, 16; ae_l32f24.i	aed15, a5, 20; ae_mulaafd32s.hh.ll	aed1, aed6, aed14; ae_mulaafd32s.hh.ll	aed8, aed13, aed12 }
    1ba9:	0e54f3a025235500c47dfcd708b9512f 	{ ae_l32x2f24.i	aed17, a2, 40; addi	a5, a5, 4; ae_sel16i.n	aed5, aed25, aed26, 0; ae_sel16i	aed3, aed31, aed15, 0 }
    1bb9:	0e14e7e0452b228e112b380a10f5915f 	{ ae_l32f24.i	aed18, a5, 20; addi	a2, a2, 8; ae_mulaafd32s.hh.ll	aed1, aed7, aed21; ae_mulaafd32s.hh.ll	aed8, aed17, aed5 }
    1bc9:	0eacfd60252355d0c45c613f0801152f 	{ ae_l32x2f24.i	aed20, a2, 40; addi	a5, a5, 4; nop; ae_roundsq32f48sym	aed23, aed28 }
    1bd9:	0e94e3e0452b22d0c50478421283955f 	{ ae_l32f24.i	aed22, a5, 20; addi	a2, a2, 8; ae_mulaafd32s.hh.ll	aed8, aed20, aed3; ae_roundsq32f48sym	aed1, aed1 }
    1be9:	0e14eba88d3d16b8702f7cb70852592f 	{ ae_l32x2f24.i	aed25, a2, 40; l32i	a6, a1, 68; ae_sel16i.n	aed2, aed18, aed22, 0; ae_mulq32sp16s.l	aed24, aed23, aed11 }
    1bf9:	0e14ebe8bd3c1f08422f7840032213ef 	{ movi	a14, 11; l32i	a15, a1, 92; ae_mulaafd32s.hh.ll	aed8, aed25, aed2; ae_mulq32sp16s.l	aed1, aed1, aed11 }
    1c09:	0e6ce168ad351456860301380401de7f 	{ slli	a13, a14, 9; l32i	a4, a1, 84; nop; ae_slaisq56s	aed26, aed24, 2 }
    1c19:	0eacc968a1e417d0c570613e140146ff 	{ s16i	a15, a6, 164; l32i	a7, a1, 80; nop; ae_roundsq32f48sym	aed28, aed8 }
    1c29:	0e5ce388b455165047002cf1078b01af 	{ ae_s16m.l.iu	aed0, a10, 2; l32i	a6, a1, 88; ae_mulq32sp16s.l	aed30, aed28, aed11; ae_slaisq56s	aed1, aed1, 2 }
    1c39:	0eacd97051f00cf0c36f4138140141ef 	{ l32i	a14, a1, 72; movi	a12, 160; nop; ae_roundsp16q48x2sym	aed27, aed26, aed26 }
    1c49:	0e6ce1689c551b57c703c13b0401c9af 	{ ae_s16m.l.iu	aed27, a10, 2; l32i	a11, a1, 76; nop; ae_slaisq56s	aed31, aed30, 2 }
    1c59:	0eacc16009eb6ff0c274213e1401f28f 	{ l32r	a8, 44 (2590 <SBC_cos_coef_4>); addi	a15, a6, 1; nop; ae_roundsp16q48x2sym	aed29, aed1, aed1 }
    1c69:	0ead3d7b0c5d0cf0c303e13b04014daf 	{ ae_s16m.l.iu	aed29, a10, 2; nop; nop; ae_roundsp16q48x2sym	aed0, aed31, aed31 }
    1c79:	230740c004c511ae                  	{ ae_s16m.l.iu	aed0, a10, 2; nop }
    1c81:	1a1432                                    	l16ui	a3, a4, 52
    1c84:	031442                                    	l16ui	a4, a4, 6
    1c87:	a0e3e0                                    	addx4	a14, a3, a14
    1c8a:	0f3d                                    	mov.n	a3, a15
    1c8c:	02bf47                                    	bgeu	a15, a4, 1c92 <SBC_synthesis4_per_channel+0x5b2>

00001c8f <SBC_synthesis4_per_channel+0x5af>:
    1c8f:	fea586                                    	j	1729 <SBC_synthesis4_per_channel+0x49>

00001c92 <SBC_synthesis4_per_channel+0x5b2>:
    1c92:	f01d                                    	retw.n

00001c94 <SBC_synthesis4_per_channel+0x5b4>:
	...

00001ca0 <SBC_synthesis8_per_channel>:
    1ca0:	008136                                    	entry	a1, 64
    1ca3:	d52c                                    	movi.n	a5, 45
    1ca5:	f8ea61                                    	l32r	a6, 50 (2650 <SBC_cos_coef_8>)
    1ca8:	115570                                    	slli	a5, a5, 9
    1cab:	031482                                    	l16ui	a8, a4, 6
    1cae:	0bd4c2                                    	addmi	a12, a4, 0xb00
    1cb1:	0bd4d2                                    	addmi	a13, a4, 0xb00
    1cb4:	78c4a2                                    	addi	a10, a4, 120
    1cb7:	01d6e2                                    	addmi	a14, a6, 0x100
    1cba:	58c4f2                                    	addi	a15, a4, 88
    1cbd:	870c                                    	movi.n	a7, 8
    1cbf:	fec392                                    	addi	a9, a3, -2
    1cc2:	8b2c                                    	movi.n	a11, 40
    1cc4:	33069a410ca7e55e                  	{ ae_cvtp24a16x2.ll	aed5, a5, a5; movnez	a15, a10, a2 }
    1ccc:	0a1c                                    	movi.n	a10, 16
    1cce:	51c9                                    	s32i.n	a12, a1, 20
    1cd0:	066142                                    	s32i	a4, a1, 24
    1cd3:	0461d2                                    	s32i	a13, a1, 16
    1cd6:	ffcad2                                    	addi	a13, a10, -1
    1cd9:	0361e2                                    	s32i	a14, a1, 12

00001cdc <SBC_synthesis8_per_channel+0x3c>:
    1cdc:	4148                                    	l32i.n	a4, a1, 16
    1cde:	7199                                    	s32i.n	a9, a1, 28
    1ce0:	20cf92                                    	addi	a9, a15, 32
    1ce3:	3168                                    	l32i.n	a6, a1, 12
    1ce5:	5138                                    	l32i.n	a3, a1, 20
    1ce7:	0f5d                                    	mov.n	a5, a15
    1ce9:	521422                                    	l16ui	a2, a4, 164
    1cec:	40a1c2                                    	movi	a12, 0x140
    1cef:	f3f6f0                                    	wur.ae_cbegin0	a15
    1cf2:	f3f790                                    	wur.ae_cend0	a9
    1cf5:	221b                                    	addi.n	a2, a2, 1
    1cf7:	2823e2                                    	l32i	a14, a3, 160
    1cfa:	f6c2a2                                    	addi	a10, a2, -10
    1cfd:	93a2a0                                    	movnez	a10, a2, a10
    1d00:	f8d421                                    	l32r	a2, 50 (2650 <SBC_cos_coef_8>)
    1d03:	1a02204000c2c75e                  	{ ae_l32x2f24.xc	aed0, a5, a7; extui	a9, a10, 0, 1 }
    1d0b:	360525c4108154ae                  	{ s16i	a10, a4, 164; ae_l32x2f24.xc	aed1, a5, a7 }
    1d13:	932690                                    	movnez	a2, a6, a9
    1d16:	a06ae0                                    	addx4	a6, a10, a14
    1d19:	360505c010e3112e                  	{ ae_l32x2f24.i	aed2, a2, 0; ae_l32x2f24.xc	aed3, a5, a7 }
    1d21:	2a8319232402e92e                  	{ ae_l32x2f24.i	aed30, a2, 8; nop; ae_mulzaafd32s.hh.ll	aed4, aed0, aed2 }
    1d29:	f8c232                                    	addi	a3, a2, -8
    1d2c:	0e94e7e6c52362d0c4009827103ecd2f 	{ ae_l32x2f24.i	aed31, a2, 16; addi	a2, a6, -40; ae_mulaafd32s.hh.ll	aed4, aed1, aed30; nop }
    1d3c:	1c81ba24287f826e                  	{ addi	a6, a2, 40; nop; ae_mulaafd32s.hh.ll	aed4, aed3, aed31 }
    1d44:	ceca                                    	add.n	a12, a14, a12
    1d46:	370505c000e1113e                  	{ ae_l32x2f24.i	aed0, a3, 32; ae_l32x2f24.xc	aed1, a5, a7 }
    1d4e:	20c342                                    	addi	a4, a3, 32
    1d51:	072d                                    	mov.n	a2, a7
    1d53:	0b3d                                    	mov.n	a3, a11
    1d55:	4f8d76                                    	loop	a13, 1da8 <SBC_synthesis8_per_channel+0x108>
    1d58:	1c831a232020394e                  	{ ae_l32x2f24.i	aed3, a4, 8; nop; ae_mulaafd32s.hh.ll	aed4, aed1, aed0 }
    1d60:	0ead256405214da0c208813d1801414f 	{ ae_l32x2f24.i	aed1, a4, 24; ae_l32x2f24.i	aed0, a4, 32; nop; ae_round24x2f48ssym	aed2, aed4, aed4 }
    1d70:	130564c010c2406e                  	{ ae_s32f24.l.i	aed2, a6, 0; ae_l32x2f24.i	aed2, a4, 16 }
    1d78:	220720c100c5d25e                  	{ ae_l32x2f24.xc	aed4, a5, a2; nop }
    1d80:	2a8299243883325e                  	{ ae_l32x2f24.xc	aed3, a5, a2; nop; ae_mulzaafd32s.hh.ll	aed4, aed4, aed3 }
    1d88:	0e94e7e1010344d0c58098211c62825f 	{ ae_l32x2f24.xc	aed2, a5, a2; addi	a4, a4, 32; ae_mulaafd32s.hh.ll	aed4, aed3, aed2; nop }
    1d98:	0e9567e6310363d0c58098211c41425f 	{ ae_l32x2f24.xc	aed1, a5, a2; add	a6, a6, a3; ae_mulaafd32s.hh.ll	aed4, aed2, aed1; nop }

00001da8 <SBC_synthesis8_per_channel+0x108>:
    1da8:	1c833a202420ec9e                  	{ movnez	a14, a12, a9; nop; ae_mulaafd32s.hh.ll	aed4, aed1, aed0 }
    1db0:	41a1a0                                    	srli	a10, a10, 1
    1db3:	60a1c2                                    	movi	a12, 0x160
    1db6:	027d                                    	mov.n	a7, a2
    1db8:	0c0441400480d44e                  	{ ae_round24x2f48ssym	aed0, aed4, aed4; movi	a13, 1 }
    1dc0:	03bd                                    	mov.n	a11, a3
    1dc2:	3d06f9c000c8506e                  	{ ae_s32f24.l.i	aed0, a6, 0; xor	a3, a9, a13 }
    1dca:	f8a261                                    	l32r	a6, 54 (2cc0 <SBC_proto_coef_8>)
    1dcd:	850c                                    	movi.n	a5, 8
    1dcf:	c12ac0                                    	mul16u	a2, a10, a12
    1dd2:	12050ec000f901ee                  	{ ae_l32x2f24.i	aed0, a14, 0; ae_l32x2f24.i	aed25, a14, 16 }
    1dda:	262a                                    	add.n	a2, a6, a2
    1ddc:	11050ec014e3332e                  	{ addx4	a2, a3, a2; ae_l32x2f24.i	aed3, a14, 8 }
    1de4:	f8ce42                                    	addi	a4, a14, -8
    1de7:	fcc222                                    	addi	a2, a2, -4
    1dea:	550b                                    	addi.n	a5, a5, -1
    1dec:	24c2e2                                    	addi	a14, a2, 36
    1def:	7128                                    	l32i.n	a2, a1, 28
    1df1:	3d04be4610ca08ee                  	{ ae_l32f24.i	aed26, a14, -32; ae_l32f24.i	aed10, a14, -16 }
    1df9:	3d04be4118cb19ee                  	{ ae_l32f24.i	aed7, a14, -28; ae_l32f24.i	aed11, a14, -12 }
    1e01:	0e54f3b7dd27ee0285297c38103a2aef 	{ ae_l32f24.i	aed8, a14, -24; ae_l32f24.i	aed27, a14, -8; ae_sel16i.n	aed1, aed26, aed7, 0; ae_sel16i	aed10, aed10, aed11, 0 }
    1e11:	0e5cf4d7e53fef200514380810016bef 	{ ae_l32f24.i	aed9, a14, -20; ae_l32f24.i	aed28, a14, -4; ae_mulzaafd32s.hh.ll	aed1, aed0, aed1; ae_mov	aed0, aed5 }
    1e21:	0e5533a435214d07c56f9c4a11084cef 	{ ae_l32f24.i	aed29, a14, 0; ae_l32x2f24.i	aed6, a4, 32; ae_sel16i.n	aed8, aed8, aed9, 0; ae_sel16i	aed31, aed27, aed28, 0 }
    1e31:	0e5537e515394d21457c380a10688def 	{ ae_l32f24.i	aed30, a14, 4; ae_l32x2f24.i	aed2, a4, 40; ae_mulaafd32s.hh.ll	aed1, aed3, aed8; ae_mov	aed5, aed31 }
    1e41:	0e54f3e144624401c477d808072abe3f 	{ addi	a3, a14, 44; addi	a4, a4, 40; ae_mulaafd32s.hh.ll	aed1, aed25, aed10; ae_sel16i	aed7, aed29, aed30, 0 }
    1e51:	230720c318e5747e                  	{ nop; nop }
    1e59:	f03d                                    	nop.n
    1e5b:	f03d                                    	nop.n
    1e5d:	878576                                    	loop	a5, 1ee8 <SBC_synthesis8_per_channel+0x248>
    1e60:	0e94e7f725273fd0c500980810c5e03f 	{ ae_l32f24.i	aed3, a3, -32; ae_l32f24.i	aed4, a3, -4; ae_mulaafd32s.hh.ll	aed1, aed6, aed5; nop }
    1e70:	0e94e7f72d273ed0c50098081047a13f 	{ ae_l32f24.i	aed2, a3, -28; ae_l32f24.i	aed5, a3, -8; ae_mulaafd32s.hh.ll	aed1, aed2, aed7; nop }
    1e80:	0e5533a435214d0144149c150863814f 	{ ae_l32x2f24.i	aed2, a4, 8; ae_l32x2f24.i	aed6, a4, 32; ae_sel16i.n	aed3, aed3, aed2, 0; ae_sel16i	aed5, aed5, aed4, 0 }
    1e90:	0e9ce0d71d2f3dd0c51078081043b03f 	{ ae_l32f24.i	aed2, a3, -16; ae_l32f24.i	aed3, a3, -12; ae_mulzaafd32s.hh.ll	aed1, aed2, aed3; ae_roundsq32f48sym	aed4, aed1 }
    1ea0:	0e14e3b725273020532f7c1810e22b3f 	{ ae_l32f24.i	aed8, a3, -20; ae_l32f24.i	aed4, a3, 0; ae_sel16i.n	aed7, aed2, aed3, 0; ae_mulq32sp16s.l	aed9, aed4, aed0 }
    1eb0:	3104b34010c31a3e                  	{ ae_l32f24.i	aed2, a3, -24; ae_l32f24.i	aed3, a3, 4 }
    1eb8:	0e54e3a16533335086013c451102494f 	{ ae_l32x2f24.i	aed9, a4, 16; addi	a3, a3, 44; ae_sel16i.n	aed8, aed2, aed8, 0; ae_slaisq56s	aed2, aed9, 2 }
    1ec8:	0e9523e515314df0c224580d1928094f 	{ ae_l32x2f24.i	aed8, a4, 24; ae_l32x2f24.i	aed2, a4, 40; ae_mulaafd32s.hh.ll	aed1, aed9, aed8; ae_roundsp16q48x2sym	aed9, aed2, aed2 }
    1ed8:	0e54f3e144434401c51078090507492f 	{ ae_s16m.l.iu	aed9, a2, 2; addi	a4, a4, 40; ae_mulaafd32s.hh.ll	aed1, aed8, aed7; ae_sel16i	aed7, aed4, aed3, 0 }
    1ee8:	1c07404018e5056e                  	{ ae_mov	aed3, aed5; ae_mov	aed5, aed0 }
    1ef0:	61e8                                    	l32i.n	a14, a1, 24
    1ef2:	029d                                    	mov.n	a9, a2
    1ef4:	1c81ba0d3cc3f88e                  	{ addi	a8, a8, -1; nop; ae_mulaafd32s.hh.ll	aed1, aed6, aed3 }
    1efc:	1c823a0a204710ae                  	{ movi	a10, 16; nop; ae_mulaafd32s.hh.ll	aed1, aed2, aed7 }
    1f04:	0ead217b0c6d0cd0c47c613a1c01fadf 	{ addi	a13, a10, -1; nop; nop; ae_roundsq32f48sym	aed31, aed1 }
    1f14:	24834f0337e5b13e                  	{ nop; nop; ae_mulq32sp16s.l	aed0, aed31, aed5 }
    1f1c:	0ead257b09040cd0c58081380c01020f 	{ ae_slaisq56s	aed0, aed0, 2; nop; nop; nop }
    1f2c:	0ead217b0d440cf0c30001380c01015f 	{ nop; nop; nop; ae_roundsp16q48x2sym	aed0, aed0, aed0 }
    1f3c:	230740c004c5119e                  	{ ae_s16m.l.iu	aed0, a9, 2; nop }
    1f44:	1a1e22                                    	l16ui	a2, a14, 52
    1f47:	a0f2f0                                    	addx4	a15, a2, a15
    1f4a:	230740b80ba5784e                  	{ bgei.w15	a8, 1, 1cdc <SBC_synthesis8_per_channel+0x3c>; nop }
    1f52:	f01d                                    	retw.n

00001f54 <_start>:
    1f54:	004136                                    	entry	a1, 32
    1f57:	f84031                                    	l32r	a3, 58 (357c <printf_ptr>)
    1f5a:	002222                                    	l32i	a2, a2, 0
    1f5d:	f83fa1                                    	l32r	a10, 5c (3508 <export_parameter_array+0x8>)
    1f60:	f840b1                                    	l32r	a11, 60 (214c <msbc_dec_init>)
    1f63:	006322                                    	s32i	a2, a3, 0
    1f66:	0002e0                                    	callx8	a2
    1f69:	0328                                    	l32i.n	a2, a3, 0
    1f6b:	f83ea1                                    	l32r	a10, 64 (351c <export_parameter_array+0x1c>)
    1f6e:	f83eb1                                    	l32r	a11, 68 (1f80 <msbc_dec_process>)
    1f71:	0002e0                                    	callx8	a2
    1f74:	f83e21                                    	l32r	a2, 6c (3500 <export_parameter_array>)
    1f77:	f01d                                    	retw.n

00001f79 <_start+0x25>:
    1f79:	00000000                                 .......

00001f80 <msbc_dec_process>:
    1f80:	004136                                    	entry	a1, 32
    1f83:	026d                                    	mov.n	a6, a2
    1f85:	0cd2f2                                    	addmi	a15, a2, 0xc00
    1f88:	f83a21                                    	l32r	a2, 70 (410 <SBC_decode_preload>)
    1f8b:	06ad                                    	mov.n	a10, a6
    1f8d:	3f39                                    	s32i.n	a3, a15, 12
    1f8f:	04d672                                    	addmi	a7, a6, 0x400
    1f92:	0002e0                                    	callx8	a2
    1f95:	38c322                                    	addi	a2, a3, 56
    1f98:	883c                                    	movi.n	a8, 56
    1f9a:	832350                                    	moveqz	a2, a3, a5
    1f9d:	276722                                    	s32i	a2, a7, 156
    1fa0:	222b                                    	addi.n	a2, a2, 2
    1fa2:	259c                                    	beqz.n	a5, 1fb8 <msbc_dec_process+0x38>
    1fa4:	935850                                    	movnez	a5, a8, a5
    1fa7:	276722                                    	s32i	a2, a7, 156
    1faa:	335a                                    	add.n	a3, a3, a5
    1fac:	820c                                    	movi.n	a2, 8
    1fae:	233d                                    	ae_l16si.n	a3, a3, 0
    1fb0:	113380                                    	slli	a3, a3, 8
    1fb3:	663d                                    	ae_s16i.n	a3, a6, 0
    1fb5:	000046                                    	j	1fba <msbc_dec_process+0x3a>

00001fb8 <msbc_dec_process+0x38>:
    1fb8:	020c                                    	movi.n	a2, 0
    1fba:	762d                                    	ae_s16i.n	a2, a6, 2
    1fbc:	273c                                    	movi.n	a7, 50
    1fbe:	70c652                                    	addi	a5, a6, 112
    1fc1:	820c                                    	movi.n	a2, 8
    1fc3:	ada032                                    	movi	a3, 173
    1fc6:	0020f0                                    	nop
    1fc9:	f03d                                    	nop.n
    1fcb:	f03d                                    	nop.n
    1fcd:	178776                                    	loop	a7, 1fe8 <msbc_dec_process+0x68>
    1fd0:	320701c71c81e58e                  	{ l32i	a8, a5, 0x3fc; ae_lb	a7, a2 }
    1fd8:	230740c00ca5782e                  	{ ae_db	a8, a2; nop }
    1fe0:	977d                                    	ae_zext16	a7, a7
    1fe2:	ff6582                                    	s32i	a8, a5, 0x3fc
    1fe5:	0f1737                                    	beq	a7, a3, 1ff8 <msbc_dec_process+0x78>

00001fe8 <msbc_dec_process+0x68>:
    1fe8:	ada022                                    	movi	a2, 173
    1feb:	06ad                                    	mov.n	a10, a6
    1fed:	04d632                                    	addmi	a3, a6, 0x400
    1ff0:	220740861c05372e                  	{ bne.w15	a7, a2, 2099 <msbc_dec_process+0x119>; nop }

00001ff8 <msbc_dec_process+0x78>:
    1ff8:	ff2532                                    	l32i	a3, a5, 0x3fc
    1ffb:	820c                                    	movi.n	a2, 8
    1ffd:	0d040f4118e0cf2e                  	{ ae_lb	a7, a2; movi	a12, 15 }
    2005:	04d6a2                                    	addmi	a10, a6, 0x400
    2008:	390449400ca0732e                  	{ ae_db	a3, a2; movi	a9, 57 }
    2010:	330721c71883e53e                  	{ s32i	a3, a5, 0x3fc; ae_lb	a15, a2 }
    2018:	ff2532                                    	l32i	a3, a5, 0x3fc
    201b:	095672                                    	s16i	a7, a6, 18
    201e:	f815d1                                    	l32r	a13, 74 (10008 <_end+0xca88>)
    2021:	f815e1                                    	l32r	a14, 78 (780008 <_end+0x77ca88>)
    2024:	230740c00ca5732e                  	{ ae_db	a3, a2; nop }
    202c:	330721c71882e53e                  	{ s32i	a3, a5, 0x3fc; ae_lb	a11, a2 }
    2034:	ff2582                                    	l32i	a8, a5, 0x3fc
    2037:	0a56f2                                    	s16i	a15, a6, 20
    203a:	af1c                                    	movi.n	a15, 26
    203c:	12c632                                    	addi	a3, a6, 18
    203f:	2106cc400ca4682e                  	{ ae_db	a8, a2; slli	a2, a12, 16 }
    2047:	ff6582                                    	s32i	a8, a5, 0x3fc
    204a:	f80c51                                    	l32r	a5, 7c (254 <sbc_extract_scale_factors_joint_ch8_non>)
    204d:	4c5a92                                    	s16i	a9, a10, 152
    2050:	06ad                                    	mov.n	a10, a6
    2052:	1656b2                                    	s16i	a11, a6, 44
    2055:	2b0c                                    	movi.n	a11, 2
    2057:	1629                                    	s32i.n	a2, a6, 4
    2059:	020c                                    	movi.n	a2, 0
    205b:	36d9                                    	s32i.n	a13, a6, 12
    205d:	2629                                    	s32i.n	a2, a6, 8
    205f:	0856f2                                    	s16i	a15, a6, 16
    2062:	d6e9                                    	s32i.n	a14, a6, 52
    2064:	0005e0                                    	callx8	a5
    2067:	1516b2                                    	l16ui	a11, a6, 42
    206a:	03ad                                    	mov.n	a10, a3
    206c:	f80551                                    	l32r	a5, 80 (14c <sbc_crc8>)
    206f:	0005e0                                    	callx8	a5
    2072:	161632                                    	l16ui	a3, a6, 44
    2075:	129a37                                    	bne	a10, a3, 208b <msbc_dec_process+0x10b>
    2078:	f7ec31                                    	l32r	a3, 28 (9a0 <sbc_bit_allocation>)
    207b:	06ad                                    	mov.n	a10, a6
    207d:	0003e0                                    	callx8	a3
    2080:	f7eb31                                    	l32r	a3, 2c (e10 <sbc_reconstruction>)
    2083:	06ad                                    	mov.n	a10, a6
    2085:	0003e0                                    	callx8	a3
    2088:	000d06                                    	j	20c0 <msbc_dec_process+0x140>

0000208b <msbc_dec_process+0x10b>:
    208b:	f7fe21                                    	l32r	a2, 84 (97c <sbc_zero_sb_samples>)
    208e:	06ad                                    	mov.n	a10, a6
    2090:	0002e0                                    	callx8	a2
    2093:	120c                                    	movi.n	a2, 1
    2095:	0009c6                                    	j	20c0 <msbc_dec_process+0x140>

00002098 <msbc_dec_process+0x118>:
	...

00002099 <msbc_dec_process+0x119>:
    2099:	f7fa21                                    	l32r	a2, 84 (97c <sbc_zero_sb_samples>)
    209c:	953c                                    	movi.n	a5, 57
    209e:	0002e0                                    	callx8	a2
    20a1:	f20c                                    	movi.n	a2, 15
    20a3:	0c0c                                    	movi.n	a12, 0
    20a5:	112200                                    	slli	a2, a2, 16
    20a8:	ad1c                                    	movi.n	a13, 26
    20aa:	f7f2e1                                    	l32r	a14, 74 (10008 <_end+0xca88>)
    20ad:	f7f2f1                                    	l32r	a15, 78 (780008 <_end+0x77ca88>)
    20b0:	1629                                    	s32i.n	a2, a6, 4
    20b2:	120c                                    	movi.n	a2, 1
    20b4:	4c5352                                    	s16i	a5, a3, 152
    20b7:	26c9                                    	s32i.n	a12, a6, 8
    20b9:	0856d2                                    	s16i	a13, a6, 16
    20bc:	36e9                                    	s32i.n	a14, a6, 12
    20be:	d6f9                                    	s32i.n	a15, a6, 52
    20c0:	a0a432                                    	movi	a3, 0x4a0
    20c3:	0bd672                                    	addmi	a7, a6, 0xb00
    20c6:	f7db51                                    	l32r	a5, 34 (1ca0 <SBC_synthesis8_per_channel>)
    20c9:	363a                                    	add.n	a3, a6, a3
    20cb:	0a0c                                    	movi.n	a10, 0
    20cd:	286732                                    	s32i	a3, a7, 160
    20d0:	5397f2                                    	l16si	a15, a7, 166
    20d3:	04bd                                    	mov.n	a11, a4
    20d5:	06cd                                    	mov.n	a12, a6
    20d7:	5257f2                                    	s16i	a15, a7, 164
    20da:	0005e0                                    	callx8	a5
    20dd:	529732                                    	l16si	a3, a7, 164
    20e0:	535732                                    	s16i	a3, a7, 166
    20e3:	f01d                                    	retw.n

000020e5 <msbc_dec_process+0x165>:
    20e5:	000000                                        ...

000020e8 <msbc_get_byte>:
    20e8:	004136                                    	entry	a1, 32
    20eb:	70c252                                    	addi	a5, a2, 112
    20ee:	830c                                    	movi.n	a3, 8
    20f0:	020701c71c81f56e                  	{ l32i	a6, a5, 0x3fc; ae_lb	a4, a3 }
    20f8:	230740c00ca5763e                  	{ ae_db	a6, a3; nop }
    2100:	942d                                    	ae_zext16	a2, a4
    2102:	ff6562                                    	s32i	a6, a5, 0x3fc
    2105:	f01d                                    	retw.n

00002107 <msbc_get_byte+0x1f>:
	...

00002108 <msbc_change_endian16>:
    2108:	004136                                    	entry	a1, 32
    210b:	ffa032                                    	movi	a3, 255
    210e:	114280                                    	slli	a4, a2, 8
    2111:	113300                                    	slli	a3, a3, 16
    2114:	01f280                                    	slli	a15, a2, 24
    2117:	103430                                    	and	a3, a4, a3
    211a:	2f3a                                    	add.n	a2, a15, a3
    211c:	312020                                    	srai	a2, a2, 16
    211f:	f01d                                    	retw.n

00002121 <msbc_change_endian16+0x19>:
    2121:	000000                                        ...

00002124 <mSBC_memory_clear>:
    2124:	004136                                    	entry	a1, 32
    2127:	f7be31                                    	l32r	a3, 20 (108 <memset>)
    212a:	58c2a2                                    	addi	a10, a2, 88
    212d:	0b0c                                    	movi.n	a11, 0
    212f:	00a4c2                                    	movi	a12, 0x400
    2132:	a0a452                                    	movi	a5, 0x4a0
    2135:	040c                                    	movi.n	a4, 0
    2137:	0003e0                                    	callx8	a3
    213a:	0b0c                                    	movi.n	a11, 0
    213c:	a25a                                    	add.n	a10, a2, a5
    213e:	80a2c2                                    	movi	a12, 0x280
    2141:	0bd222                                    	addmi	a2, a2, 0xb00
    2144:	0003e0                                    	callx8	a3
    2147:	535242                                    	s16i	a4, a2, 166
    214a:	f01d                                    	retw.n

0000214c <msbc_dec_init>:
    214c:	004136                                    	entry	a1, 32
    214f:	f7b431                                    	l32r	a3, 20 (108 <memset>)
    2152:	58c2a2                                    	addi	a10, a2, 88
    2155:	0b0c                                    	movi.n	a11, 0
    2157:	00a4c2                                    	movi	a12, 0x400
    215a:	a0a452                                    	movi	a5, 0x4a0
    215d:	00a042                                    	movi	a4, 0
    2160:	0003e0                                    	callx8	a3
    2163:	0b0c                                    	movi.n	a11, 0
    2165:	a25a                                    	add.n	a10, a2, a5
    2167:	80a2c2                                    	movi	a12, 0x280
    216a:	0bd222                                    	addmi	a2, a2, 0xb00
    216d:	0003e0                                    	callx8	a3
    2170:	535242                                    	s16i	a4, a2, 166
    2173:	f01d                                    	retw.n

00002175 <msbc_dec_init+0x29>:
    2175:	000000                                        ...

00002178 <__do_global_ctors_aux>:
    2178:	004136                                    	entry	a1, 32
    217b:	f7c331                                    	l32r	a3, 88 (24a0 <__CTOR_END__>)
    217e:	fcc322                                    	addi	a2, a3, -4
    2181:	002222                                    	l32i	a2, a2, 0
    2184:	020283a4001e920e                  	{ beqi.w15	a2, -1, 2199 <__do_global_ctors_aux+0x21>; addi	a3, a3, -8 }

0000218c <__do_global_ctors_aux+0x14>:
    218c:	0002e0                                    	callx8	a2
    218f:	0328                                    	l32i.n	a2, a3, 0
    2191:	0302e3bc07ffb26e                  	{ bnei.w15	a2, -1, 218c <__do_global_ctors_aux+0x14>; addi	a3, a3, -4 }
    2199:	f01d                                    	retw.n

0000219b <__do_global_ctors_aux+0x23>:
	...

0000219c <_fini>:
    219c:	008136                                    	entry	a1, 64
    219f:	f7bb81                                    	l32r	a8, 8c (a8 <__do_global_dtors_aux>)
    21a2:	f03d                                    	nop.n
    21a4:	0008e0                                    	callx8	a8

000021a7 <_fini+0xb>:
    21a7:	f01d                                    	retw.n
