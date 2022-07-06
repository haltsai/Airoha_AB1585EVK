
Build/lib/pisplit_plc_pitch_position_independent.splitload.lib:     file format elf32-xtensa-le


Disassembly of section .text:

00000000 <_init-0x8c>:
       0:	000000dc 000021e0 0000285c 00000000     .....!..\(......
      10:	0000282c 00000000 00002844 00001e5c     ,(......D(..\...
      20:	00001728 0000015c 00000100 00002870     (...\.......p(..
      30:	00002890 000028b0 00001730 00002260     .(...(..0...`"..
      40:	00002210 00001ec8 0000213c 8000ffff     ."......<!......
      50:	000e0008 000f000a 0001000a 00010001     ................
      60:	000028c0 00002990 0000293c 00002098     .(...)..<)... ..
      70:	00002955 0000013c 0000296f 000003f8     U)..<...o)......
      80:	00002930 00002834 000000a4              0)..4(......

0000008c <_init>:
      8c:	008136                                    	entry	a1, 64
      8f:	ffdc81                                    	l32r	a8, 0 (dc <frame_dummy>)
      92:	f03d                                    	nop.n
      94:	0008e0                                    	callx8	a8
      97:	ffdb81                                    	l32r	a8, 4 (21e0 <__do_global_ctors_aux>)
      9a:	f03d                                    	nop.n
      9c:	0008e0                                    	callx8	a8
      9f:	f01d                                    	retw.n

000000a1 <_init+0x15>:
      a1:	000000                                        ...

000000a4 <__do_global_dtors_aux>:
      a4:	004136                                    	entry	a1, 32
      a7:	ffd821                                    	l32r	a2, 8 (285c <__do_global_dtors_aux.completed>)
      aa:	000232                                    	l8ui	a3, a2, 0
      ad:	63ec                                    	bnez.n	a3, d7 <__do_global_dtors_aux+0x33>

000000af <__do_global_dtors_aux+0xb>:
      af:	1248                                    	l32i.n	a4, a2, 4
      b1:	0438                                    	l32i.n	a3, a4, 0
      b3:	04028424000163ae                  	{ beqz.w15	a3, c4 <__do_global_dtors_aux+0x20>; addi	a4, a4, 4 }
      bb:	1249                                    	s32i.n	a4, a2, 4
      bd:	0003e0                                    	callx8	a3
      c0:	fffac6                                    	j	af <__do_global_dtors_aux+0xb>

000000c3 <__do_global_dtors_aux+0x1f>:
	...

000000c4 <__do_global_dtors_aux+0x20>:
      c4:	ffd231                                    	l32r	a3, c (0 <_text_start>)
      c7:	ffd2a1                                    	l32r	a10, 10 (282c <__FRAME_END__>)
      ca:	438c                                    	beqz.n	a3, d2 <__do_global_dtors_aux+0x2e>
      cc:	ffd081                                    	l32r	a8, c (0 <_text_start>)
      cf:	0008e0                                    	callx8	a8
      d2:	130c                                    	movi.n	a3, 1
      d4:	004232                                    	s8i	a3, a2, 0
      d7:	f01d                                    	retw.n

000000d9 <__do_global_dtors_aux+0x35>:
      d9:	000000                                        ...

000000dc <frame_dummy>:
      dc:	004136                                    	entry	a1, 32
      df:	ffcd21                                    	l32r	a2, 14 (0 <_text_start>)
      e2:	ffcba1                                    	l32r	a10, 10 (282c <__FRAME_END__>)
      e5:	ffccb1                                    	l32r	a11, 18 (2844 <frame_dummy.object>)
      e8:	428c                                    	beqz.n	a2, f0 <frame_dummy+0x14>
      ea:	ffca81                                    	l32r	a8, 14 (0 <_text_start>)
      ed:	0008e0                                    	callx8	a8

000000f0 <frame_dummy+0x14>:
      f0:	f01d                                    	retw.n

000000f2 <frame_dummy+0x16>:
	...

00000100 <memset>:
     100:	004136                                    	entry	a1, 32
     103:	025d                                    	mov.n	a5, a2
     105:	052d                                    	mov.n	a2, a5
     107:	d48c                                    	beqz.n	a4, 118 <memset+0x18>
     109:	f03d                                    	nop.n
     10b:	f03d                                    	nop.n
     10d:	078476                                    	loop	a4, 118 <memset+0x18>
     110:	1202a2400480023e                  	{ s8i	a3, a2, 0; addi	a2, a2, 1 }

00000118 <memset+0x18>:
     118:	052d                                    	mov.n	a2, a5
     11a:	f01d                                    	retw.n

0000011c <xt_memset_plc>:
     11c:	004136                                    	entry	a1, 32
     11f:	025d                                    	mov.n	a5, a2
     121:	329c                                    	beqz.n	a2, 138 <xt_memset_plc+0x1c>
     123:	052d                                    	mov.n	a2, a5
     125:	f48c                                    	beqz.n	a4, 138 <xt_memset_plc+0x1c>
     127:	0020f0                                    	nop
     12a:	0020f0                                    	nop
     12d:	078476                                    	loop	a4, 138 <xt_memset_plc+0x1c>
     130:	2202a2400080023e                  	{ s16i	a3, a2, 0; addi	a2, a2, 2 }

00000138 <xt_memset_plc+0x1c>:
     138:	052d                                    	mov.n	a2, a5
     13a:	f01d                                    	retw.n

0000013c <SMART_PITCH_PLC_INI>:
     13c:	004136                                    	entry	a1, 32
     13f:	ffb741                                    	l32r	a4, 1c (1e5c <plc_gainctrl_ini>)
     142:	02ad                                    	mov.n	a10, a2
     144:	0004e0                                    	callx8	a4
     147:	ffb641                                    	l32r	a4, 20 (1728 <find_pitch_ini>)
     14a:	0004e0                                    	callx8	a4
     14d:	ffb541                                    	l32r	a4, 24 (15c <cvsd_a2plc_ini>)
     150:	03ad                                    	mov.n	a10, a3
     152:	02bd                                    	mov.n	a11, a2
     154:	0004e0                                    	callx8	a4
     157:	f01d                                    	retw.n

00000159 <SMART_PITCH_PLC_INI+0x1d>:
     159:	000000                                        ...

0000015c <cvsd_a2plc_ini>:
     15c:	006136                                    	entry	a1, 48
     15f:	4b23e2                                    	l32i	a14, a3, 0x12c
     162:	036d                                    	mov.n	a6, a3
     164:	a0a042                                    	movi	a4, 160
     167:	ffb071                                    	l32r	a7, 28 (100 <memset>)
     16a:	3923d2                                    	l32i	a13, a3, 228
     16d:	3129                                    	s32i.n	a2, a1, 12
     16f:	3a23f2                                    	l32i	a15, a3, 232
     172:	e3d9                                    	s32i.n	a13, a3, 56
     174:	f3f9                                    	s32i.n	a15, a3, 60
     176:	cd3c                                    	movi.n	a13, 60
     178:	6ea0f2                                    	movi	a15, 110
     17b:	064e16                                    	beqz	a14, 1e3 <cvsd_a2plc_ini+0x87>
     17e:	6649                                    	s32i.n	a4, a6, 24
     180:	3a0c                                    	movi.n	a10, 3
     182:	6c4c                                    	movi.n	a12, 70
     184:	08a2b2                                    	movi	a11, 0x208
     187:	30a2e2                                    	movi	a14, 0x230
     18a:	021c                                    	movi.n	a2, 16
     18c:	030c                                    	movi.n	a3, 0
     18e:	140c                                    	movi.n	a4, 1
     190:	26d9                                    	s32i.n	a13, a6, 8
     192:	283c                                    	movi.n	a8, 50
     194:	253c                                    	movi.n	a5, 50
     196:	892c                                    	movi.n	a9, 40
     198:	76f9                                    	s32i.n	a15, a6, 28
     19a:	e6a0d2                                    	movi	a13, 230
     19d:	96a9                                    	s32i.n	a10, a6, 36
     19f:	08a2f2                                    	movi	a15, 0x208
     1a2:	b6c9                                    	s32i.n	a12, a6, 44
     1a4:	a6ea                                    	add.n	a10, a6, a14
     1a6:	a6b9                                    	s32i.n	a11, a6, 40
     1a8:	0b0c                                    	movi.n	a11, 0
     1aa:	3d6632                                    	s32i	a3, a6, 244
     1ad:	10a4c2                                    	movi	a12, 0x410
     1b0:	1629                                    	s32i.n	a2, a6, 4
     1b2:	0629                                    	s32i.n	a2, a6, 0
     1b4:	68a632                                    	movi	a3, 0x668
     1b7:	3629                                    	s32i.n	a2, a6, 12
     1b9:	4649                                    	s32i.n	a4, a6, 16
     1bb:	2159                                    	s32i.n	a5, a1, 8
     1bd:	5689                                    	s32i.n	a8, a6, 20
     1bf:	8699                                    	s32i.n	a9, a6, 32
     1c1:	11f9                                    	s32i.n	a15, a1, 4
     1c3:	c6d9                                    	s32i.n	a13, a6, 48
     1c5:	0007e0                                    	callx8	a7
     1c8:	0b0c                                    	movi.n	a11, 0
     1ca:	a63a                                    	add.n	a10, a6, a3
     1cc:	0c5c                                    	movi.n	a12, 80
     1ce:	e0a632                                    	movi	a3, 0x6e0
     1d1:	0007e0                                    	callx8	a7
     1d4:	0b0c                                    	movi.n	a11, 0
     1d6:	a63a                                    	add.n	a10, a6, a3
     1d8:	10a4c2                                    	movi	a12, 0x410
     1db:	0007e0                                    	callx8	a7
     1de:	06bd                                    	mov.n	a11, a6
     1e0:	001946                                    	j	249 <cvsd_a2plc_ini+0xed>

000001e3 <cvsd_a2plc_ini+0x87>:
     1e3:	150c                                    	movi.n	a5, 1
     1e5:	0a5c                                    	movi.n	a10, 80
     1e7:	eb1c                                    	movi.n	a11, 30
     1e9:	a0a0c2                                    	movi	a12, 160
     1ec:	30a2e2                                    	movi	a14, 0x230
     1ef:	e30c                                    	movi.n	a3, 14
     1f1:	820c                                    	movi.n	a2, 8
     1f3:	882c                                    	movi.n	a8, 40
     1f5:	4659                                    	s32i.n	a5, a6, 16
     1f7:	641c                                    	movi.n	a4, 22
     1f9:	76a9                                    	s32i.n	a10, a6, 28
     1fb:	78a092                                    	movi	a9, 120
     1fe:	86b9                                    	s32i.n	a11, a6, 32
     200:	86a152                                    	movi	a5, 0x186
     203:	86a1d2                                    	movi	a13, 0x186
     206:	8f2c                                    	movi.n	a15, 40
     208:	c6c9                                    	s32i.n	a12, a6, 48
     20a:	a6ea                                    	add.n	a10, a6, a14
     20c:	0b0c                                    	movi.n	a11, 0
     20e:	0ca3c2                                    	movi	a12, 0x30c
     211:	1639                                    	s32i.n	a3, a6, 4
     213:	0629                                    	s32i.n	a2, a6, 0
     215:	2649                                    	s32i.n	a4, a6, 8
     217:	3639                                    	s32i.n	a3, a6, 12
     219:	68a632                                    	movi	a3, 0x668
     21c:	5689                                    	s32i.n	a8, a6, 20
     21e:	6699                                    	s32i.n	a9, a6, 24
     220:	11d9                                    	s32i.n	a13, a1, 4
     222:	a659                                    	s32i.n	a5, a6, 40
     224:	0261f2                                    	s32i	a15, a1, 8
     227:	0b6682                                    	s32i	a8, a6, 44
     22a:	0007e0                                    	callx8	a7
     22d:	0b0c                                    	movi.n	a11, 0
     22f:	a63a                                    	add.n	a10, a6, a3
     231:	cc3c                                    	movi.n	a12, 60
     233:	e0a632                                    	movi	a3, 0x6e0
     236:	0007e0                                    	callx8	a7
     239:	0b0c                                    	movi.n	a11, 0
     23b:	a63a                                    	add.n	a10, a6, a3
     23d:	0ca3c2                                    	movi	a12, 0x30c
     240:	330c                                    	movi.n	a3, 3
     242:	0007e0                                    	callx8	a7
     245:	9639                                    	s32i.n	a3, a6, 36
     247:	06bd                                    	mov.n	a11, a6
     249:	f0c242                                    	addi	a4, a2, -16
     24c:	080c                                    	movi.n	a8, 0
     24e:	624840                                    	saltu	a4, a8, a4
     251:	ff7631                                    	l32r	a3, 2c (2870 <rc_win_right_16s>)
     254:	ff7751                                    	l32r	a5, 30 (2890 <rc_win_left_16s>)
     257:	f0a162                                    	movi	a6, 0x1f0
     25a:	149020                                    	extui	a9, a2, 0, 2
     25d:	401400                                    	ssl	a4
     260:	417220                                    	srli	a7, a2, 2
     263:	4b6a                                    	add.n	a4, a11, a6
     265:	159976                                    	loopnez	a9, 27e <cvsd_a2plc_ini+0x122>
     268:	a12800                                    	sll	a2, a8
     26b:	881b                                    	addi.n	a8, a8, 1
     26d:	906230                                    	addx2	a6, a2, a3
     270:	90a250                                    	addx2	a10, a2, a5
     273:	266d                                    	ae_l16si.n	a6, a6, 0
     275:	105462                                    	s16i	a6, a4, 32
     278:	2a2d                                    	ae_l16si.n	a2, a10, 0
     27a:	642d                                    	ae_s16i.n	a2, a4, 0
     27c:	442b                                    	addi.n	a4, a4, 2

0000027e <cvsd_a2plc_ini+0x122>:
     27e:	a12800                                    	sll	a2, a8
     281:	0eb716                                    	beqz	a7, 370 <cvsd_a2plc_ini+0x214>
     284:	906230                                    	addx2	a6, a2, a3
     287:	d81b                                    	addi.n	a13, a8, 1
     289:	266d                                    	ae_l16si.n	a6, a6, 0
     28b:	902250                                    	addx2	a2, a2, a5
     28e:	a1ed00                                    	sll	a14, a13
     291:	105462                                    	s16i	a6, a4, 32
     294:	222d                                    	ae_l16si.n	a2, a2, 0
     296:	90fe30                                    	addx2	a15, a14, a3
     299:	642d                                    	ae_s16i.n	a2, a4, 0
     29b:	8d1b                                    	addi.n	a8, a13, 1
     29d:	2f2d                                    	ae_l16si.n	a2, a15, 0
     29f:	906e50                                    	addx2	a6, a14, a5
     2a2:	115422                                    	s16i	a2, a4, 34
     2a5:	a1a800                                    	sll	a10, a8
     2a8:	262d                                    	ae_l16si.n	a2, a6, 0
     2aa:	90ca30                                    	addx2	a12, a10, a3
     2ad:	944b                                    	addi.n	a9, a4, 4
     2af:	742d                                    	ae_s16i.n	a2, a4, 2
     2b1:	2c4d                                    	ae_l16si.n	a4, a12, 0
     2b3:	881b                                    	addi.n	a8, a8, 1
     2b5:	90da50                                    	addx2	a13, a10, a5
     2b8:	105942                                    	s16i	a4, a9, 32
     2bb:	a1e800                                    	sll	a14, a8
     2be:	2d4d                                    	ae_l16si.n	a4, a13, 0
     2c0:	90fe30                                    	addx2	a15, a14, a3
     2c3:	694d                                    	ae_s16i.n	a4, a9, 0
     2c5:	2f6d                                    	ae_l16si.n	a6, a15, 0
     2c7:	770b                                    	addi.n	a7, a7, -1
     2c9:	90ae50                                    	addx2	a10, a14, a5
     2cc:	115962                                    	s16i	a6, a9, 34
     2cf:	032d                                    	mov.n	a2, a3
     2d1:	203550                                    	or	a3, a5, a5
     2d4:	206aa0                                    	or	a6, a10, a10
     2d7:	01c842                                    	addi	a4, a8, 1
     2da:	205990                                    	or	a5, a9, a9
     2dd:	879776                                    	loopnez	a7, 368 <cvsd_a2plc_ini+0x20c>
     2e0:	0703464110a034ce                  	{ sll	a6, a4; l16si	a7, a6, 0 }
     2e8:	3206a6400895057e                  	{ s16i	a7, a5, 2; addx2	a7, a6, a2 }
     2f0:	080285400081075e                  	{ l16si	a5, a7, 0; addi	a8, a5, 4 }
     2f8:	0402a8440081185e                  	{ s16i	a5, a8, 32; addi	a5, a8, 4 }
     300:	1402844514e0263e                  	{ addx2	a6, a6, a3; addi	a4, a4, 1 }
     308:	300704c00085066e                  	{ l16si	a6, a6, 0; sll	a7, a4 }
     310:	2206a7400095086e                  	{ s16i	a6, a8, 0; addx2	a6, a7, a2 }
     318:	140284400080066e                  	{ l16si	a6, a6, 0; addi	a4, a4, 1 }
     320:	2206a7440895186e                  	{ s16i	a6, a8, 34; addx2	a6, a7, a3 }
     328:	200704c00085067e                  	{ l16si	a7, a6, 0; sll	a6, a4 }
     330:	3206a6400895087e                  	{ s16i	a7, a8, 2; addx2	a7, a6, a2 }
     338:	140284400080077e                  	{ l16si	a7, a7, 0; addi	a4, a4, 1 }
     340:	2206a6440095157e                  	{ s16i	a7, a5, 32; addx2	a6, a6, a3 }
     348:	200704c00085067e                  	{ l16si	a7, a6, 0; sll	a6, a4 }
     350:	3206a6400095057e                  	{ s16i	a7, a5, 0; addx2	a7, a6, a2 }
     358:	140284400080077e                  	{ l16si	a7, a7, 0; addi	a4, a4, 1 }
     360:	2206a6440895157e                  	{ s16i	a7, a5, 34; addx2	a6, a6, a3 }
     368:	26ed                                    	ae_l16si.n	a14, a6, 0
     36a:	06ad                                    	mov.n	a10, a6
     36c:	f54b                                    	addi.n	a15, a5, 4
     36e:	75ed                                    	ae_s16i.n	a14, a5, 2

00000370 <cvsd_a2plc_ini+0x214>:
     370:	3178                                    	l32i.n	a7, a1, 12
     372:	e21c                                    	movi.n	a2, 30
     374:	130c                                    	movi.n	a3, 1
     376:	63a1f2                                    	movi	a15, 0x163
     379:	113310                                    	slli	a3, a3, 15
     37c:	ff2b61                                    	l32r	a6, 28 (100 <memset>)
     37f:	824720                                    	mull	a4, a7, a2
     382:	106b32                                    	s32i	a3, a11, 64
     385:	020c                                    	movi.n	a2, 0
     387:	113fd0                                    	slli	a3, a15, 3
     38a:	1ec442                                    	addi	a4, a4, 30
     38d:	db29                                    	s32i.n	a2, a11, 52
     38f:	ab3a                                    	add.n	a10, a11, a3
     391:	1a6b42                                    	s32i	a4, a11, 104
     394:	0b5d                                    	mov.n	a5, a11
     396:	0b0c                                    	movi.n	a11, 0
     398:	0c5c                                    	movi.n	a12, 80
     39a:	0006e0                                    	callx8	a6
     39d:	ff2531                                    	l32r	a3, 34 (28b0 <zc_diff_size_tab>)
     3a0:	ffa042                                    	movi	a4, 255
     3a3:	70a1f2                                    	movi	a15, 0x170
     3a6:	7fd442                                    	addmi	a4, a4, 0x7f00
     3a9:	a5fa                                    	add.n	a10, a5, a15
     3ab:	0b0c                                    	movi.n	a11, 0
     3ad:	a03730                                    	addx4	a3, a7, a3
     3b0:	80a0c2                                    	movi	a12, 128
     3b3:	0338                                    	l32i.n	a3, a3, 0
     3b5:	1178                                    	l32i.n	a7, a1, 4
     3b7:	196522                                    	s32i	a2, a5, 100
     3ba:	146532                                    	s32i	a3, a5, 80
     3bd:	1e6522                                    	s32i	a2, a5, 120
     3c0:	216522                                    	s32i	a2, a5, 132
     3c3:	130c                                    	movi.n	a3, 1
     3c5:	206522                                    	s32i	a2, a5, 128
     3c8:	1f6522                                    	s32i	a2, a5, 124
     3cb:	186572                                    	s32i	a7, a5, 96
     3ce:	1b6542                                    	s32i	a4, a5, 108
     3d1:	1c6542                                    	s32i	a4, a5, 112
     3d4:	1d6522                                    	s32i	a2, a5, 116
     3d7:	0006e0                                    	callx8	a6
     3da:	2148                                    	l32i.n	a4, a1, 8
     3dc:	2e0c                                    	movi.n	a14, 2
     3de:	226542                                    	s32i	a4, a5, 136
     3e1:	126522                                    	s32i	a2, a5, 72
     3e4:	c0f740                                    	sub	a15, a7, a4
     3e7:	166542                                    	s32i	a4, a5, 88
     3ea:	136522                                    	s32i	a2, a5, 76
     3ed:	116532                                    	s32i	a3, a5, 68
     3f0:	1565e2                                    	s32i	a14, a5, 84
     3f3:	1765f2                                    	s32i	a15, a5, 92
     3f6:	f01d                                    	retw.n

000003f8 <SMART_PITCH_PLC_PROCESS>:
     3f8:	012136                                    	entry	a1, 144
     3fb:	2521f2                                    	l32i	a15, a1, 148
     3fe:	03dd                                    	mov.n	a13, a3
     400:	02bd                                    	mov.n	a11, a2
     402:	2421c2                                    	l32i	a12, a1, 144
     405:	78a022                                    	movi	a2, 120
     408:	0f78                                    	l32i.n	a7, a15, 0
     40a:	192f32                                    	l32i	a3, a15, 100
     40d:	afe8                                    	l32i.n	a14, a15, 40
     40f:	2f68                                    	l32i.n	a6, a15, 8
     411:	1b8d16                                    	beqz	a13, 5cd <SMART_PITCH_PLC_PROCESS+0x1d5>
     414:	220720a40525b3ae                  	{ beqz.w15	a3, d8d <SMART_PITCH_PLC_PROCESS+0x995>; nop }
     41c:	4b2f32                                    	l32i	a3, a15, 0x12c
     41f:	61c9                                    	s32i.n	a12, a1, 24
     421:	b1e9                                    	s32i.n	a14, a1, 44
     423:	0f5d                                    	mov.n	a5, a15
     425:	51d9                                    	s32i.n	a13, a1, 20
     427:	78a0d2                                    	movi	a13, 120
     42a:	230760861825fc2e                  	{ bne.w15	a12, a2, 60d <SMART_PITCH_PLC_PROCESS+0x215>; nop }
     432:	172fc2                                    	l32i	a12, a15, 92
     435:	1d4316                                    	beqz	a3, 60d <SMART_PITCH_PLC_PROCESS+0x215>
     438:	30a232                                    	movi	a3, 0x230
     43b:	0b7d                                    	mov.n	a7, a11
     43d:	2f3a                                    	add.n	a2, a15, a3
     43f:	162fb2                                    	l32i	a11, a15, 88
     442:	feff41                                    	l32r	a4, 40 (2210 <getfespeech>)
     445:	02ad                                    	mov.n	a10, a2
     447:	02ed                                    	mov.n	a14, a2
     449:	0004e0                                    	callx8	a4
     44c:	1c2542                                    	l32i	a4, a5, 112
     44f:	fea0e2                                    	movi	a14, 254
     452:	05fd                                    	mov.n	a15, a5
     454:	7fde52                                    	addmi	a5, a14, 0x7f00
     457:	07bd                                    	mov.n	a11, a7
     459:	3f04084004a1e44e                  	{ ae_cvtp24a16x2.ll	aed0, a4, a4; movi	a14, 120 }
     461:	230740841805d54e                  	{ blt.w15	a5, a4, 500 <SMART_PITCH_PLC_PROCESS+0x108>; nop }
     469:	100472400c8293fe                  	{ ae_l16m.x	aed1, a15, a3; ae_l16m.iu	aed2, a2, 2 }
     471:	0e1d225b0d240c14442b6d0a1c2031ef 	{ srli	a3, a14, 1; nop; ae_mulf32s.ll	aed1, aed1, aed0; ae_mulf32s.ll	aed2, aed2, aed0 }
     481:	0ead217b0c6d0cf0c204213a1c01f33f 	{ addi	a3, a3, -1; nop; nop; ae_roundsp16q48x2sym	aed1, aed1, aed1 }
     491:	220740c00cc51f2e                  	{ ae_s16m.l.i	aed1, a2, -2; nop }
     499:	230760c00c85712e                  	{ ae_l16m.iu	aed1, a2, 2; nop }
     4a1:	02ad                                    	mov.n	a10, a2
     4a3:	230720c318e5747e                  	{ nop; nop }
     4ab:	f03d                                    	nop.n
     4ad:	378376                                    	loop	a3, 4e8 <SMART_PITCH_PLC_PROCESS+0xf0>
     4b0:	0e9d225b0d540cf0c3044d100c20015f 	{ nop; nop; ae_mulf32s.ll	aed2, aed1, aed0; ae_roundsp16q48x2sym	aed1, aed2, aed2 }
     4c0:	220740c00cc51fae                  	{ ae_s16m.l.i	aed1, a10, -2; nop }
     4c8:	0ead217b0d350cf0c2044138080181af 	{ ae_l16m.iu	aed2, a10, 2; nop; nop; ae_roundsp16q48x2sym	aed1, aed2, aed2 }
     4d8:	2282aa1728401fae                  	{ ae_s16m.l.i	aed1, a10, -2; nop; ae_mulf32s.ll	aed2, aed2, aed0 }
     4e0:	230760c00c8571ae                  	{ ae_l16m.iu	aed1, a10, 2; nop }

000004e8 <SMART_PITCH_PLC_PROCESS+0xf0>:
     4e8:	0ead217b0d540cf0c30441380c01015f 	{ nop; nop; nop; ae_roundsp16q48x2sym	aed1, aed2, aed2 }
     4f8:	220740c00cc51fae                  	{ ae_s16m.l.i	aed1, a10, -2; nop }

00000500 <SMART_PITCH_PLC_PROCESS+0x108>:
     500:	61c8                                    	l32i.n	a12, a1, 24
     502:	b1e8                                    	l32i.n	a14, a1, 44
     504:	51d8                                    	l32i.n	a13, a1, 20
     506:	1916a6                                    	blti	a6, 1, 523 <SMART_PITCH_PLC_PROCESS+0x12b>
     509:	30a222                                    	movi	a2, 0x230
     50c:	0b4d                                    	mov.n	a4, a11
     50e:	3f2a                                    	add.n	a3, a15, a2
     510:	0f8676                                    	loop	a6, 523 <SMART_PITCH_PLC_PROCESS+0x12b>
     513:	220283400080132e                  	{ l16si	a2, a3, 0; addi	a3, a3, 2 }
     51b:	2402a4400080042e                  	{ s16i	a2, a4, 0; addi	a4, a4, 2 }
     523:	140c                                    	movi.n	a4, 1
     525:	78a022                                    	movi	a2, 120
     528:	014490                                    	slli	a4, a4, 23
     52b:	c02260                                    	sub	a2, a2, a6
     52e:	77a032                                    	movi	a3, 119
     531:	9076f0                                    	addx2	a7, a6, a15
     534:	d22420                                    	quos	a2, a4, a2
     537:	30a282                                    	movi	a8, 0x230
     53a:	230720841905736e                  	{ blt.w15	a3, a6, d8d <SMART_PITCH_PLC_PROCESS+0x995>; nop }
     542:	9036b0                                    	addx2	a3, a6, a11
     545:	c0a420                                    	sub	a10, a4, a2
     548:	000473400482887e                  	{ ae_l16m.x	aed0, a7, a8; ae_l16m.i	aed2, a3, 0 }
     550:	0e94e7a4452364d0c5009f1700601a2f 	{ ae_movda32x2	aed1, a10, a2; addi	a4, a6, -120; ae_sel16i.n	aed3, aed0, aed2, 3; nop }
     560:	2a83192a3023578e                  	{ add	a5, a7, a8; nop; ae_mulzaafd32s.hh.ll	aed5, aed1, aed3 }
     568:	606040                                    	neg	a6, a4
     56b:	609020                                    	neg	a9, a2
     56e:	fec382                                    	addi	a8, a3, -2
     571:	0eace571252b51f0c308a13f0001092f 	{ ae_movda32x2	aed0, a9, a2; ae_l16m.iu	aed4, a5, 2; nop; ae_roundsp16q48x2sym	aed2, aed5, aed5 }
     581:	0ea4e407fd2366d0c400810208c1cd3f 	{ ae_l16m.iu	aed31, a3, 2; addi	a6, a6, -1; ae_add24s	aed1, aed1, aed0; nop }
     591:	3e0032c410e3e88e                  	{ or	a2, a8, a8; ae_sel16i	aed3, aed4, aed31, 3 }
     599:	054d                                    	mov.n	a4, a5
     59b:	f03d                                    	nop.n
     59d:	1f9676                                    	loopnez	a6, 5c0 <SMART_PITCH_PLC_PROCESS+0x1c8>
     5a0:	2a82b9273023212e                  	{ ae_s16m.l.iu	aed2, a2, 2; nop; ae_mulzaafd32s.hh.ll	aed4, aed1, aed3 }
     5a8:	110473401c82714e                  	{ ae_l16m.iu	aed3, a4, 2; ae_l16m.iu	aed2, a3, 2 }
     5b0:	0ea5041b0be40cf0c208810018c1332f 	{ ae_sel16i	aed3, aed3, aed2, 3; nop; ae_add24s	aed1, aed1, aed0; ae_roundsp16q48x2sym	aed2, aed4, aed4 }

000005c0 <SMART_PITCH_PLC_PROCESS+0x1c8>:
     5c0:	02ad                                    	mov.n	a10, a2
     5c2:	230740c014c511ae                  	{ ae_s16m.l.iu	aed2, a10, 2; nop }
     5ca:	01efc6                                    	j	d8d <SMART_PITCH_PLC_PROCESS+0x995>

000005cd <SMART_PITCH_PLC_PROCESS+0x1d5>:
     5cd:	4f28                                    	l32i.n	a2, a15, 16
     5cf:	27a316                                    	beqz	a3, 84d <SMART_PITCH_PLC_PROCESS+0x455>
     5d2:	112f42                                    	l32i	a4, a15, 68
     5d5:	220760a40c25126e                  	{ bnei.w15	a2, 1, 7ba <SMART_PITCH_PLC_PROCESS+0x3c2>; nop }
     5dd:	152f22                                    	l32i	a2, a15, 84
     5e0:	071347                                    	beq	a3, a4, 5eb <SMART_PITCH_PLC_PROCESS+0x1f3>
     5e3:	230760861825932e                  	{ bne.w15	a3, a2, 7ba <SMART_PITCH_PLC_PROCESS+0x3c2>; nop }
     5eb:	ef28                                    	l32i.n	a2, a15, 56
     5ed:	0e6d                                    	mov.n	a6, a14
     5ef:	ffa032                                    	movi	a3, 255
     5f2:	61c9                                    	s32i.n	a12, a1, 24
     5f4:	ff48                                    	l32i.n	a4, a15, 60
     5f6:	91b9                                    	s32i.n	a11, a1, 36
     5f8:	1c2f52                                    	l32i	a5, a15, 112
     5fb:	220720a40d05526e                  	{ bnei.w15	a2, 1, e68 <SMART_PITCH_PLC_PROCESS+0xa70>; nop }
     603:	822540                                    	mull	a2, a5, a4
     606:	212f20                                    	srai	a2, a2, 15
     609:	021786                                    	j	e6b <SMART_PITCH_PLC_PROCESS+0xa73>

0000060c <SMART_PITCH_PLC_PROCESS+0x214>:
	...

0000060d <SMART_PITCH_PLC_PROCESS+0x215>:
     60d:	30a222                                    	movi	a2, 0x230
     610:	0b4d                                    	mov.n	a4, a11
     612:	af2a                                    	add.n	a10, a15, a2
     614:	162fb2                                    	l32i	a11, a15, 88
     617:	172fc2                                    	l32i	a12, a15, 92
     61a:	d67a                                    	add.n	a13, a6, a7
     61c:	fe8921                                    	l32r	a2, 40 (2210 <getfespeech>)
     61f:	0aed                                    	mov.n	a14, a10
     621:	0f3d                                    	mov.n	a3, a15
     623:	0002e0                                    	callx8	a2
     626:	04bd                                    	mov.n	a11, a4
     628:	03fd                                    	mov.n	a15, a3
     62a:	f0a122                                    	movi	a2, 0x1f0
     62d:	220740a00c0557ce                  	{ blti.w15	a7, 1, 6d9 <SMART_PITCH_PLC_PROCESS+0x2e1>; nop }
     635:	b0a132                                    	movi	a3, 0x1b0
     638:	9056b0                                    	addx2	a5, a6, a11
     63b:	4f3a                                    	add.n	a4, a15, a3
     63d:	2f2a                                    	add.n	a2, a15, a2
     63f:	fec532                                    	addi	a3, a5, -2
     642:	fec252                                    	addi	a5, a2, -2
     645:	278776                                    	loop	a7, 670 <SMART_PITCH_PLC_PROCESS+0x278>
     648:	110473400c80715e                  	{ ae_l16m.iu	aed1, a5, 2; ae_l16m.iu	aed0, a3, 2 }
     650:	1a8355033420b13e                  	{ nop; nop; ae_mul32.ll	aed0, aed1, aed0 }
     658:	0ead257b0d240cd0c400813b0c01620f 	{ ae_trunca32q48	a2, aed0; nop; nop; nop }
     668:	0502a4400081042e                  	{ s32i	a2, a4, 0; addi	a4, a4, 4 }

00000670 <SMART_PITCH_PLC_PROCESS+0x278>:
     670:	30a222                                    	movi	a2, 0x230
     673:	70a1d2                                    	movi	a13, 0x170
     676:	4f2a                                    	add.n	a4, a15, a2
     678:	1126f0                                    	slli	a2, a6, 1
     67b:	220474400460e43e                  	{ addi	a3, a4, -32; ae_l16m.x	aed0, a4, a2 }
     683:	3e02e7c00c9f603e                  	{ ae_l16m.i	aed1, a3, 0; addi	a14, a7, -1 }
     68b:	1a8335002c20877e                  	{ or	a8, a7, a7; nop; ae_mul32.ll	aed0, aed1, aed0 }
     693:	3fda                                    	add.n	a3, a15, a13
     695:	02c442                                    	addi	a4, a4, 2
     698:	000003f3020200fad1e3030534601e2f 	{ loopnez	a14, 6d2 <SMART_PITCH_PLC_PROCESS+0x2da>; ae_trunca32q48	a5, aed0; nop }
     6a8:	0502a4c00098135e                  	{ s32i	a5, a3, 0; addi	a5, a4, -32 }
     6b0:	220474400c80605e                  	{ ae_l16m.i	aed1, a5, 0; ae_l16m.x	aed0, a4, a2 }
     6b8:	1a8355033420b13e                  	{ nop; nop; ae_mul32.ll	aed0, aed1, aed0 }
     6c0:	334b                                    	addi.n	a3, a3, 4
     6c2:	0eace560152244d0c400813b0c01650f 	{ ae_trunca32q48	a5, aed0; addi	a4, a4, 2; nop; nop }
     6d2:	0359                                    	s32i.n	a5, a3, 0
     6d4:	087d                                    	mov.n	a7, a8
     6d6:	04c3e2                                    	addi	a14, a3, 4

000006d9 <SMART_PITCH_PLC_PROCESS+0x2e1>:
     6d9:	1d2f22                                    	l32i	a2, a15, 116
     6dc:	61c8                                    	l32i.n	a12, a1, 24
     6de:	b1e8                                    	l32i.n	a14, a1, 44
     6e0:	51d8                                    	l32i.n	a13, a1, 20
     6e2:	6158                                    	l32i.n	a5, a1, 24
     6e4:	fccb32                                    	addi	a3, a11, -4
     6e7:	214160                                    	srai	a4, a6, 1
     6ea:	230760a408a5f26e                  	{ bnei.w15	a2, 1, ccd <SMART_PITCH_PLC_PROCESS+0x8d5>; nop }
     6f2:	b1e8                                    	l32i.n	a14, a1, 44
     6f4:	51d8                                    	l32i.n	a13, a1, 20
     6f6:	05cd                                    	mov.n	a12, a5
     6f8:	2416a6                                    	blti	a6, 1, 720 <SMART_PITCH_PLC_PROCESS+0x328>
     6fb:	30a232                                    	movi	a3, 0x230
     6fe:	1c2f22                                    	l32i	a2, a15, 112
     701:	3f3a                                    	add.n	a3, a15, a3
     703:	0b4d                                    	mov.n	a4, a11
     705:	178676                                    	loop	a6, 720 <SMART_PITCH_PLC_PROCESS+0x328>
     708:	220283400080135e                  	{ l16si	a5, a3, 0; addi	a3, a3, 2 }
     710:	825250                                    	mull	a5, a2, a5
     713:	f03d                                    	nop.n
     715:	415f50                                    	srli	a5, a5, 15
     718:	2402a4400080045e                  	{ s16i	a5, a4, 0; addi	a4, a4, 2 }
     720:	70a122                                    	movi	a2, 0x170
     723:	230720a00cc537ce                  	{ blti.w15	a7, 1, d8d <SMART_PITCH_PLC_PROCESS+0x995>; nop }
     72b:	5f2a                                    	add.n	a5, a15, a2
     72d:	10c132                                    	addi	a3, a1, 16
     730:	05a8                                    	l32i.n	a10, a5, 0
     732:	70cf22                                    	addi	a2, a15, 112
     735:	9046b0                                    	addx2	a4, a6, a11
     738:	102562                                    	l32i	a6, a5, 64
     73b:	554b                                    	addi.n	a5, a5, 4
     73d:	0104324208a02fae                  	{ srai	a9, a10, 15; ae_l16m.i	aed0, a2, 0 }
     745:	4199                                    	s32i.n	a9, a1, 16
     747:	3a02e7c00c9f603e                  	{ ae_l16m.i	aed1, a3, 0; addi	a10, a7, -1 }
     74f:	0020f0                                    	nop
     752:	0020f0                                    	nop
     755:	0020f0                                    	nop
     758:	1a83550230204a2e                  	{ loopnez	a10, 7a0 <SMART_PITCH_PLC_PROCESS+0x3a8>; nop; ae_mul32.ll	aed0, aed1, aed0 }
     760:	0eace568052458d0c400813b0c01670f 	{ ae_trunca32q48	a7, aed0; l32i	a8, a5, 0; nop; nop }
     770:	3706a74110b12f8e                  	{ srai	a6, a8, 15; add	a7, a7, a6 }
     778:	2706bfc10095116e                  	{ s32i	a6, a1, 16; srli	a6, a7, 15 }
     780:	000433400081046e                  	{ s16i	a6, a4, 0; ae_l16m.i	aed1, a3, 0 }
     788:	220760c00485702e                  	{ ae_l16m.i	aed0, a2, 0; nop }
     790:	1a81b5042020244e                  	{ addi	a4, a4, 2; nop; ae_mul32.ll	aed0, aed1, aed0 }
     798:	040285440481156e                  	{ l32i	a6, a5, 64; addi	a5, a5, 4 }

000007a0 <SMART_PITCH_PLC_PROCESS+0x3a8>:
     7a0:	0eace56015224ad0c400813b0c01620f 	{ ae_trunca32q48	a2, aed0; addi	a10, a4, 2; nop; nop }
     7b0:	226a                                    	add.n	a2, a2, a6
     7b2:	412f20                                    	srli	a2, a2, 15
     7b5:	642d                                    	ae_s16i.n	a2, a4, 0
     7b7:	017486                                    	j	d8d <SMART_PITCH_PLC_PROCESS+0x995>

000007ba <SMART_PITCH_PLC_PROCESS+0x3c2>:
     7ba:	1d2f22                                    	l32i	a2, a15, 116
     7bd:	5179                                    	s32i.n	a7, a1, 20
     7bf:	0b6d                                    	mov.n	a6, a11
     7c1:	0e7d                                    	mov.n	a7, a14
     7c3:	ef48                                    	l32i.n	a4, a15, 56
     7c5:	0c5d                                    	mov.n	a5, a12
     7c7:	021226                                    	beqi	a2, 1, 7cd <SMART_PITCH_PLC_PROCESS+0x3d5>
     7ca:	241466                                    	bnei	a4, 1, 7f2 <SMART_PITCH_PLC_PROCESS+0x3fa>
     7cd:	30a242                                    	movi	a4, 0x230
     7d0:	112f22                                    	l32i	a2, a15, 68
     7d3:	af4a                                    	add.n	a10, a15, a4
     7d5:	162fb2                                    	l32i	a11, a15, 88
     7d8:	0aed                                    	mov.n	a14, a10
     7da:	172fc2                                    	l32i	a12, a15, 92
     7dd:	51d8                                    	l32i.n	a13, a1, 20
     7df:	0fa237                                    	bge	a2, a3, 7f2 <SMART_PITCH_PLC_PROCESS+0x3fa>
     7e2:	fe1731                                    	l32r	a3, 40 (2210 <getfespeech>)
     7e5:	0f4d                                    	mov.n	a4, a15
     7e7:	122f22                                    	l32i	a2, a15, 72
     7ea:	0003e0                                    	callx8	a3
     7ed:	126422                                    	s32i	a2, a4, 72
     7f0:	04fd                                    	mov.n	a15, a4
     7f2:	30a222                                    	movi	a2, 0x230
     7f5:	e0a632                                    	movi	a3, 0x6e0
     7f8:	af2a                                    	add.n	a10, a15, a2
     7fa:	162fb2                                    	l32i	a11, a15, 88
     7fd:	ef3a                                    	add.n	a14, a15, a3
     7ff:	172fc2                                    	l32i	a12, a15, 92
     802:	fe0f21                                    	l32r	a2, 40 (2210 <getfespeech>)
     805:	05dd                                    	mov.n	a13, a5
     807:	0f3d                                    	mov.n	a3, a15
     809:	0002e0                                    	callx8	a2
     80c:	1d2322                                    	l32i	a2, a3, 116
     80f:	20d330                                    	or	a13, a3, a3
     812:	0e2d32                                    	l32i	a3, a13, 56
     815:	071226                                    	beqi	a2, 1, 820 <SMART_PITCH_PLC_PROCESS+0x428>
     818:	230700a408c5736e                  	{ bnei.w15	a3, 1, e2b <SMART_PITCH_PLC_PROCESS+0xa33>; nop }
     820:	192d22                                    	l32i	a2, a13, 100
     823:	ffa0a2                                    	movi	a10, 255
     826:	112d32                                    	l32i	a3, a13, 68
     829:	7fdaa2                                    	addmi	a10, a10, 0x7f00
     82c:	ed48                                    	l32i.n	a4, a13, 56
     82e:	220760860ca5d32e                  	{ bge.w15	a3, a2, e2b <SMART_PITCH_PLC_PROCESS+0xa33>; nop }
     836:	fd28                                    	l32i.n	a2, a13, 60
     838:	220760a00d45746e                  	{ bnei.w15	a4, 1, 1328 <SMART_PITCH_PLC_PROCESS+0xf30>; nop }
     840:	1c2d32                                    	l32i	a3, a13, 112
     843:	822320                                    	mull	a2, a3, a2
     846:	21af20                                    	srai	a10, a2, 15
     849:	02b6c6                                    	j	1328 <SMART_PITCH_PLC_PROCESS+0xf30>

0000084c <SMART_PITCH_PLC_PROCESS+0x454>:
	...

0000084d <SMART_PITCH_PLC_PROCESS+0x455>:
     84d:	3f88                                    	l32i.n	a8, a15, 12
     84f:	61c9                                    	s32i.n	a12, a1, 24
     851:	6fd8                                    	l32i.n	a13, a15, 24
     853:	bfc8                                    	l32i.n	a12, a15, 44
     855:	c03e80                                    	sub	a3, a14, a8
     858:	1198f0                                    	slli	a9, a8, 1
     85b:	e0a642                                    	movi	a4, 0x6e0
     85e:	4a13a6                                    	blti	a3, 1, 8ac <SMART_PITCH_PLC_PROCESS+0x4b4>
     861:	5f4a                                    	add.n	a5, a15, a4
     863:	412130                                    	srli	a2, a3, 1
     866:	459a                                    	add.n	a4, a5, a9
     868:	056307                                    	bbci	a3, 0, 871 <SMART_PITCH_PLC_PROCESS+0x479>
     86b:	244d                                    	ae_l16si.n	a4, a4, 0
     86d:	654d                                    	ae_s16i.n	a4, a5, 0
     86f:	552b                                    	addi.n	a5, a5, 2
     871:	459a                                    	add.n	a4, a5, a9
     873:	52bc                                    	beqz.n	a2, 8ac <SMART_PITCH_PLC_PROCESS+0x4b4>
     875:	352b                                    	addi.n	a3, a5, 2
     877:	244d                                    	ae_l16si.n	a4, a4, 0
     879:	a39a                                    	add.n	a10, a3, a9
     87b:	654d                                    	ae_s16i.n	a4, a5, 0
     87d:	620b                                    	addi.n	a6, a2, -1
     87f:	2a4d                                    	ae_l16si.n	a4, a10, 0
     881:	532b                                    	addi.n	a5, a3, 2
     883:	092d                                    	mov.n	a2, a9
     885:	1f9676                                    	loopnez	a6, 8a8 <SMART_PITCH_PLC_PROCESS+0x4b0>
     888:	3206a5400090034e                  	{ s16i	a4, a3, 0; add	a3, a5, a2 }
     890:	220285400080134e                  	{ l16si	a4, a3, 0; addi	a3, a5, 2 }
     898:	0206a3400091054e                  	{ s16i	a4, a5, 0; add	a4, a3, a2 }
     8a0:	240283400080144e                  	{ l16si	a4, a4, 0; addi	a5, a3, 2 }

000008a8 <SMART_PITCH_PLC_PROCESS+0x4b0>:
     8a8:	029d                                    	mov.n	a9, a2
     8aa:	634d                                    	ae_s16i.n	a4, a3, 0

000008ac <SMART_PITCH_PLC_PROCESS+0x4b4>:
     8ac:	d02e90                                    	subx2	a2, a14, a9
     8af:	1d18a6                                    	blti	a8, 1, 8d0 <SMART_PITCH_PLC_PROCESS+0x4d8>
     8b2:	e0a632                                    	movi	a3, 0x6e0
     8b5:	22fa                                    	add.n	a2, a2, a15
     8b7:	204bb0                                    	or	a4, a11, a11
     8ba:	803230                                    	add	a3, a2, a3
     8bd:	0f8876                                    	loop	a8, 8d0 <SMART_PITCH_PLC_PROCESS+0x4d8>
     8c0:	240284400080042e                  	{ l16si	a2, a4, 0; addi	a4, a4, 2 }
     8c8:	2202a3400080132e                  	{ s16i	a2, a3, 0; addi	a3, a3, 2 }
     8d0:	30a222                                    	movi	a2, 0x230
     8d3:	2ca232                                    	movi	a3, 0x22c
     8d6:	af2a                                    	add.n	a10, a15, a2
     8d8:	2141e0                                    	srai	a4, a14, 1
     8db:	dca652                                    	movi	a5, 0x6dc
     8de:	162ea6                                    	blti	a14, 2, 8f8 <SMART_PITCH_PLC_PROCESS+0x500>
     8e1:	2f5a                                    	add.n	a2, a15, a5
     8e3:	3f3a                                    	add.n	a3, a15, a3
     8e5:	0f8476                                    	loop	a4, 8f8 <SMART_PITCH_PLC_PROCESS+0x500>
     8e8:	220760c00485d12e                  	{ ae_l16x2m.iu	aed0, a2, 4; nop }
     8f0:	220740c004c5713e                  	{ ae_s16x2m.iu	aed0, a3, 4; nop }
     8f8:	0e4d                                    	mov.n	a4, a14
     8fa:	51f9                                    	s32i.n	a15, a1, 20
     8fc:	5128                                    	l32i.n	a2, a1, 20
     8fe:	91b9                                    	s32i.n	a11, a1, 36
     900:	cfb8                                    	l32i.n	a11, a15, 48
     902:	7fe8                                    	l32i.n	a14, a15, 28
     904:	8199                                    	s32i.n	a9, a1, 32
     906:	1822f2                                    	l32i	a15, a2, 96
     909:	076182                                    	s32i	a8, a1, 28
     90c:	fdcb21                                    	l32r	a2, 38 (1730 <find_pitch>)
     90f:	0a61a2                                    	s32i	a10, a1, 40
     912:	0002e0                                    	callx8	a2
     915:	5128                                    	l32i.n	a2, a1, 20
     917:	0a6d                                    	mov.n	a6, a10
     919:	61c8                                    	l32i.n	a12, a1, 24
     91b:	51f8                                    	l32i.n	a15, a1, 20
     91d:	9228                                    	l32i.n	a2, a2, 36
     91f:	2131c0                                    	srai	a3, a12, 1
     922:	31dfc0                                    	srai	a13, a12, 31
     925:	226fa2                                    	s32i	a10, a15, 136
     928:	8222a0                                    	mull	a2, a2, a10
     92b:	a23a                                    	add.n	a10, a2, a3
     92d:	fdc321                                    	l32r	a2, 3c (2260 <__divdi3>)
     930:	31bfa0                                    	srai	a11, a10, 31
     933:	130c                                    	movi.n	a3, 1
     935:	0002e0                                    	callx8	a2
     938:	51d8                                    	l32i.n	a13, a1, 20
     93a:	220c                                    	movi.n	a2, 2
     93c:	72fa20                                    	salt	a15, a10, a2
     93f:	115af0                                    	slli	a5, a10, 1
     942:	833af0                                    	moveqz	a3, a10, a15
     945:	9352f0                                    	movnez	a5, a2, a15
     948:	116d32                                    	s32i	a3, a13, 68
     94b:	04ed                                    	mov.n	a14, a4
     94d:	156d52                                    	s32i	a5, a13, 84
     950:	1127f0                                    	slli	a2, a7, 1
     953:	30a232                                    	movi	a3, 0x230
     956:	1e17a6                                    	blti	a7, 1, 978 <SMART_PITCH_PLC_PROCESS+0x580>
     959:	d02e20                                    	subx2	a2, a14, a2
     95c:	68a652                                    	movi	a5, 0x668
     95f:	22da                                    	add.n	a2, a2, a13
     961:	423a                                    	add.n	a4, a2, a3
     963:	3d5a                                    	add.n	a3, a13, a5
     965:	0f8776                                    	loop	a7, 978 <SMART_PITCH_PLC_PROCESS+0x580>
     968:	240284400080042e                  	{ l16si	a2, a4, 0; addi	a4, a4, 2 }
     970:	2202a3400080132e                  	{ s16i	a2, a3, 0; addi	a3, a3, 2 }
     978:	182d32                                    	l32i	a3, a13, 96
     97b:	71a8                                    	l32i.n	a10, a1, 28
     97d:	020c                                    	movi.n	a2, 0
     97f:	166d62                                    	s32i	a6, a13, 88
     982:	126d22                                    	s32i	a2, a13, 72
     985:	06fd                                    	mov.n	a15, a6
     987:	c0c360                                    	sub	a12, a3, a6
     98a:	371b                                    	addi.n	a3, a7, 1
     98c:	64a642                                    	movi	a4, 0x664
     98f:	176dc2                                    	s32i	a12, a13, 92
     992:	b33770                                    	movgez	a3, a7, a7
     995:	220700a4102597ce                  	{ blti.w15	a7, 2, aaa <SMART_PITCH_PLC_PROCESS+0x6b2>; nop }
     99d:	0ca252                                    	movi	a5, 0x20c
     9a0:	216130                                    	srai	a6, a3, 1
     9a3:	3d5a                                    	add.n	a3, a13, a5
     9a5:	30c122                                    	addi	a2, a1, 48
     9a8:	4d4a                                    	add.n	a4, a13, a4
     9aa:	415160                                    	srli	a5, a6, 1
     9ad:	2d6607                                    	bbci	a6, 0, 9de <SMART_PITCH_PLC_PROCESS+0x5e6>
     9b0:	200474c00482d13e                  	{ ae_l16x2m.iu	aed0, a3, 4; ae_l16x2m.iu	aed2, a4, 4 }
     9b8:	0e9523db0d540c008111b1080c02015f 	{ nop; nop; ae_mul32.hh	aed1, aed0, aed2; ae_mul32.ll	aed0, aed0, aed2 }
     9c8:	000003f3008300fc562303002c60b3bf 	{ ae_trunca32q48	a11, aed1; ae_trunca32q48	a8, aed0; nop }
     9d8:	02b9                                    	s32i.n	a11, a2, 0
     9da:	1289                                    	s32i.n	a8, a2, 4
     9dc:	228b                                    	addi.n	a2, a2, 8
     9de:	079d                                    	mov.n	a9, a7
     9e0:	0c6516                                    	beqz	a5, aaa <SMART_PITCH_PLC_PROCESS+0x6b2>
     9e3:	200474c00481d13e                  	{ ae_l16x2m.iu	aed0, a3, 4; ae_l16x2m.iu	aed1, a4, 4 }
     9eb:	0e94e3d7152b41008991b11a00018d3f 	{ ae_l16x2m.iu	aed30, a3, 4; ae_l16x2m.iu	aed2, a4, 4; ae_mul32.hh	aed3, aed0, aed1; ae_mul32.ll	aed4, aed0, aed1 }
     9fb:	1a81b3fd3fc2f58e                  	{ addi	a8, a5, -1; nop; ae_mul32.hh	aed31, aed30, aed2 }
     a03:	000003d3108300fdd63b00003c62b36f 	{ ae_trunca32q48	a6, aed3; ae_trunca32q48	a11, aed4; ae_mul32.ll	aed0, aed30, aed2 }
     a13:	12b9                                    	s32i.n	a11, a2, 4
     a15:	065d                                    	mov.n	a5, a6
     a17:	0020f0                                    	nop
     a1a:	0020f0                                    	nop
     a1d:	0020f0                                    	nop
     a20:	000003f37a0200fbf1e303032460382f 	{ loopnez	a8, a90 <SMART_PITCH_PLC_PROCESS+0x698>; ae_trunca32q48	a7, aed31; nop }
     a30:	0eace577052231d0c400813b0c01660f 	{ ae_trunca32q48	a6, aed0; ae_l16x2m.iu	aed0, a3, 4; nop; nop }
     a40:	210434c00081125e                  	{ s32i	a5, a2, 0; ae_l16x2m.iu	aed1, a4, 4 }
     a48:	0e94c3d719ea31008311b1051401027f 	{ s32i	a7, a2, 8; ae_l16x2m.iu	aed3, a3, 4; ae_mul32.hh	aed0, aed0, aed1; ae_mul32.ll	aed1, aed0, aed1 }
     a58:	210434c01882126e                  	{ s32i	a6, a2, 12; ae_l16x2m.iu	aed2, a4, 4 }
     a60:	0e94e7c0852222d0c400910b0c62651f 	{ ae_trunca32q48	a5, aed1; addi	a2, a2, 16; ae_mul32.hh	aed1, aed3, aed2; nop }
     a70:	00000073038300fad6bb00002c62125f 	{ s32i	a5, a2, 4; ae_trunca32q48	a5, aed0; ae_mul32.ll	aed0, aed3, aed2 }
     a80:	0ead257b0d240cd0c400813b0c01671f 	{ ae_trunca32q48	a7, aed1; nop; nop; nop }

00000a90 <SMART_PITCH_PLC_PROCESS+0x698>:
     a90:	000003f3038300fc56a303002460125f 	{ s32i	a5, a2, 0; ae_trunca32q48	a8, aed0; nop }
     aa0:	2279                                    	s32i.n	a7, a2, 8
     aa2:	097d                                    	mov.n	a7, a9
     aa4:	036282                                    	s32i	a8, a2, 12
     aa7:	10c2b2                                    	addi	a11, a2, 16

00000aaa <SMART_PITCH_PLC_PROCESS+0x6b2>:
     aaa:	c02c70                                    	sub	a2, a12, a7
     aad:	230700a40c2577ce                  	{ blti.w15	a7, 1, be0 <SMART_PITCH_PLC_PROCESS+0x7e8>; nop }
     ab5:	f0a152                                    	movi	a5, 0x1f0
     ab8:	30a232                                    	movi	a3, 0x230
     abb:	9022d0                                    	addx2	a2, a2, a13
     abe:	30c142                                    	addi	a4, a1, 48
     ac1:	bd5a                                    	add.n	a11, a13, a5
     ac3:	223a                                    	add.n	a2, a2, a3
     ac5:	fcc452                                    	addi	a5, a4, -4
     ac8:	fecb32                                    	addi	a3, a11, -2
     acb:	fec242                                    	addi	a4, a2, -2
     ace:	670b                                    	addi.n	a6, a7, -1
     ad0:	110565400482713e                  	{ ae_l16m.iu	aed0, a3, 2; ae_l32m.iu	aed2, a5, 4 }
     ad8:	1104544310a1368e                  	{ loopnez	a6, b0a <SMART_PITCH_PLC_PROCESS+0x712>; ae_l16m.iu	aed1, a4, 2 }
     ae0:	1c8227142801013e                  	{ ae_l16m.iu	aed0, a3, 2; nop; ae_mula32.ll	aed2, aed0, aed1 }
     ae8:	230760c00c85714e                  	{ ae_l16m.iu	aed1, a4, 2; nop }
     af0:	0ead257b0d240cd0c400813b0c01622f 	{ ae_trunca32q48	a2, aed2; nop; nop; nop }
     b00:	0529                                    	s32i.n	a2, a5, 0
     b02:	230720c010c5715e                  	{ ae_l32m.iu	aed2, a5, 4; nop }

00000b0a <SMART_PITCH_PLC_PROCESS+0x712>:
     b0a:	1c830712240187fe                  	{ slli	a8, a7, 1; nop; ae_mula32.ll	aed2, aed0, aed1 }
     b12:	e0a642                                    	movi	a4, 0x6e0
     b15:	30a292                                    	movi	a9, 0x230
     b18:	146070                                    	extui	a6, a7, 0, 2
     b1b:	0eace561852213d0c400813b0c01622f 	{ ae_trunca32q48	a2, aed2; addi	a3, a1, 48; nop; nop }
     b2b:	0529                                    	s32i.n	a2, a5, 0
     b2d:	d02e80                                    	subx2	a2, a14, a8
     b30:	22da                                    	add.n	a2, a2, a13
     b32:	b24a                                    	add.n	a11, a2, a4
     b34:	229a                                    	add.n	a2, a2, a9
     b36:	414270                                    	srli	a4, a7, 2
     b39:	0e9676                                    	loopnez	a6, b4b <SMART_PITCH_PLC_PROCESS+0x753>
     b3c:	0378                                    	l32i.n	a7, a3, 0
     b3e:	334b                                    	addi.n	a3, a3, 4
     b40:	417f70                                    	srli	a7, a7, 15
     b43:	627d                                    	ae_s16i.n	a7, a2, 0
     b45:	222b                                    	addi.n	a2, a2, 2
     b47:	6b7d                                    	ae_s16i.n	a7, a11, 0
     b49:	bb2b                                    	addi.n	a11, a11, 2
     b4b:	824b                                    	addi.n	a8, a2, 4
     b4d:	08f416                                    	beqz	a4, be0 <SMART_PITCH_PLC_PROCESS+0x7e8>
     b50:	0358                                    	l32i.n	a5, a3, 0
     b52:	a40b                                    	addi.n	a10, a4, -1
     b54:	012362                                    	l32i	a6, a3, 4
     b57:	204880                                    	or	a4, a8, a8
     b5a:	032372                                    	l32i	a7, a3, 12
     b5d:	415f50                                    	srli	a5, a5, 15
     b60:	022392                                    	l32i	a9, a3, 8
     b63:	416f60                                    	srli	a6, a6, 15
     b66:	625d                                    	ae_s16i.n	a5, a2, 0
     b68:	6b5d                                    	ae_s16i.n	a5, a11, 0
     b6a:	417f70                                    	srli	a7, a7, 15
     b6d:	726d                                    	ae_s16i.n	a6, a2, 2
     b6f:	10c322                                    	addi	a2, a3, 16
     b72:	7b6d                                    	ae_s16i.n	a6, a11, 2
     b74:	04cbb2                                    	addi	a11, a11, 4
     b77:	416f90                                    	srli	a6, a9, 15
     b7a:	203bb0                                    	or	a3, a11, a11
     b7d:	519a76                                    	loopnez	a10, bd2 <SMART_PITCH_PLC_PROCESS+0x7da>
     b80:	0403a2400080146e                  	{ s16i	a6, a4, 0; l32i	a5, a2, 0 }
     b88:	0602a4400081036e                  	{ s16i	a6, a3, 0; addi	a6, a4, 4 }
     b90:	1403a2400880047e                  	{ s16i	a7, a4, 2; l32i	a4, a2, 4 }
     b98:	1406bfc00895137e                  	{ s16i	a7, a3, 2; srli	a5, a5, 15 }
     ba0:	0202a3400081165e                  	{ s16i	a5, a6, 0; addi	a3, a3, 4 }
     ba8:	3406bfc00095035e                  	{ s16i	a5, a3, 0; srli	a7, a4, 15 }
     bb0:	3248                                    	l32i.n	a4, a2, 12
     bb2:	2403a2400880167e                  	{ s16i	a7, a6, 2; l32i	a5, a2, 8 }
     bba:	0202a3400881137e                  	{ s16i	a7, a3, 2; addi	a3, a3, 4 }
     bc2:	3406ffc10475064e                  	{ addi	a4, a6, 4; srli	a7, a4, 15 }
     bca:	0302a24114a44f5e                  	{ srli	a6, a5, 15; addi	a2, a2, 16 }
     bd2:	71a8                                    	l32i.n	a10, a1, 28
     bd4:	646d                                    	ae_s16i.n	a6, a4, 0
     bd6:	048d                                    	mov.n	a8, a4
     bd8:	636d                                    	ae_s16i.n	a6, a3, 0
     bda:	747d                                    	ae_s16i.n	a7, a4, 2
     bdc:	b34b                                    	addi.n	a11, a3, 4
     bde:	737d                                    	ae_s16i.n	a7, a3, 2

00000be0 <SMART_PITCH_PLC_PROCESS+0x7e8>:
     be0:	9148                                    	l32i.n	a4, a1, 36
     be2:	221aa6                                    	blti	a10, 1, c08 <SMART_PITCH_PLC_PROCESS+0x810>
     be5:	8128                                    	l32i.n	a2, a1, 32
     be7:	e0a632                                    	movi	a3, 0x6e0
     bea:	0020f0                                    	nop
     bed:	d02e20                                    	subx2	a2, a14, a2
     bf0:	22da                                    	add.n	a2, a2, a13
     bf2:	803230                                    	add	a3, a2, a3
     bf5:	0f8a76                                    	loop	a10, c08 <SMART_PITCH_PLC_PROCESS+0x810>
     bf8:	220283400080132e                  	{ l16si	a2, a3, 0; addi	a3, a3, 2 }
     c00:	2402a4400080042e                  	{ s16i	a2, a4, 0; addi	a4, a4, 2 }
     c08:	6128                                    	l32i.n	a2, a1, 24
     c0a:	7aea                                    	add.n	a7, a10, a14
     c0c:	6188                                    	l32i.n	a8, a1, 24
     c0e:	e0a632                                    	movi	a3, 0x6e0
     c11:	082152                                    	l32i	a5, a1, 32
     c14:	c062a0                                    	sub	a6, a2, a10
     c17:	0a21a2                                    	l32i	a10, a1, 40
     c1a:	c02e60                                    	sub	a2, a14, a6
     c1d:	c04780                                    	sub	a4, a7, a8
     c20:	5012a6                                    	blti	a2, 1, c74 <SMART_PITCH_PLC_PROCESS+0x87c>
     c23:	d02850                                    	subx2	a2, a8, a5
     c26:	805d30                                    	add	a5, a13, a3
     c29:	413140                                    	srli	a3, a4, 1
     c2c:	096407                                    	bbci	a4, 0, c39 <SMART_PITCH_PLC_PROCESS+0x841>
     c2f:	804520                                    	add	a4, a5, a2
     c32:	244d                                    	ae_l16si.n	a4, a4, 0
     c34:	654d                                    	ae_s16i.n	a4, a5, 0
     c36:	02c552                                    	addi	a5, a5, 2
     c39:	452a                                    	add.n	a4, a5, a2
     c3b:	53bc                                    	beqz.n	a3, c74 <SMART_PITCH_PLC_PROCESS+0x87c>
     c3d:	852b                                    	addi.n	a8, a5, 2
     c3f:	244d                                    	ae_l16si.n	a4, a4, 0
     c41:	e82a                                    	add.n	a14, a8, a2
     c43:	654d                                    	ae_s16i.n	a4, a5, 0
     c45:	930b                                    	addi.n	a9, a3, -1
     c47:	2e4d                                    	ae_l16si.n	a4, a14, 0
     c49:	582b                                    	addi.n	a5, a8, 2
     c4b:	083d                                    	mov.n	a3, a8
     c4d:	1f9976                                    	loopnez	a9, c70 <SMART_PITCH_PLC_PROCESS+0x878>
     c50:	3206a5400090034e                  	{ s16i	a4, a3, 0; add	a3, a5, a2 }
     c58:	220285400080134e                  	{ l16si	a4, a3, 0; addi	a3, a5, 2 }
     c60:	0206a3400091054e                  	{ s16i	a4, a5, 0; add	a4, a3, a2 }
     c68:	240283400080144e                  	{ l16si	a4, a4, 0; addi	a5, a3, 2 }

00000c70 <SMART_PITCH_PLC_PROCESS+0x878>:
     c70:	634d                                    	ae_s16i.n	a4, a3, 0
     c72:	038d                                    	mov.n	a8, a3

00000c74 <SMART_PITCH_PLC_PROCESS+0x87c>:
     c74:	6da122                                    	movi	a2, 0x16d
     c77:	0fbd                                    	mov.n	a11, a15
     c79:	1122d0                                    	slli	a2, a2, 3
     c7c:	51f8                                    	l32i.n	a15, a1, 20
     c7e:	4d2a                                    	add.n	a4, a13, a2
     c80:	fcf021                                    	l32r	a2, 40 (2210 <getfespeech>)
     c83:	06dd                                    	mov.n	a13, a6
     c85:	04ed                                    	mov.n	a14, a4
     c87:	0002e0                                    	callx8	a2
     c8a:	51d8                                    	l32i.n	a13, a1, 20
     c8c:	91b8                                    	l32i.n	a11, a1, 36
     c8e:	61c8                                    	l32i.n	a12, a1, 24
     c90:	230720a0094596ce                  	{ blti.w15	a6, 1, 16e6 <SMART_PITCH_PLC_PROCESS+0x12ee>; nop }
     c98:	7138                                    	l32i.n	a3, a1, 28
     c9a:	c027c0                                    	sub	a2, a7, a12
     c9d:	e0a652                                    	movi	a5, 0x6e0
     ca0:	9022d0                                    	addx2	a2, a2, a13
     ca3:	04ed                                    	mov.n	a14, a4
     ca5:	225a                                    	add.n	a2, a2, a5
     ca7:	9033b0                                    	addx2	a3, a3, a11
     caa:	198676                                    	loop	a6, cc7 <SMART_PITCH_PLC_PROCESS+0x8cf>
     cad:	2e4d                                    	ae_l16si.n	a4, a14, 0
     caf:	2202a3400080134e                  	{ s16i	a4, a3, 0; addi	a3, a3, 2 }
     cb7:	2e028e4000800e4e                  	{ l16si	a4, a14, 0; addi	a14, a14, 2 }
     cbf:	2202a2400080024e                  	{ s16i	a4, a2, 0; addi	a2, a2, 2 }

00000cc7 <SMART_PITCH_PLC_PROCESS+0x8cf>:
     cc7:	0286c6                                    	j	16e6 <SMART_PITCH_PLC_PROCESS+0x12ee>

00000cca <SMART_PITCH_PLC_PROCESS+0x8d2>:
     cca:	000000                                        ...

00000ccd <SMART_PITCH_PLC_PROCESS+0x8d5>:
     ccd:	2ca222                                    	movi	a2, 0x22c
     cd0:	1426a6                                    	blti	a6, 2, ce8 <SMART_PITCH_PLC_PROCESS+0x8f0>
     cd3:	2f2a                                    	add.n	a2, a15, a2
     cd5:	0f8476                                    	loop	a4, ce8 <SMART_PITCH_PLC_PROCESS+0x8f0>
     cd8:	220760c00485d12e                  	{ ae_l16x2m.iu	aed0, a2, 4; nop }
     ce0:	220740c004c5713e                  	{ ae_s16x2m.iu	aed0, a3, 4; nop }
     ce8:	70a142                                    	movi	a4, 0x170
     ceb:	230740a00805f7ce                  	{ blti.w15	a7, 1, d8d <SMART_PITCH_PLC_PROCESS+0x995>; nop }
     cf3:	412170                                    	srli	a2, a7, 1
     cf6:	9036b0                                    	addx2	a3, a6, a11
     cf9:	4f4a                                    	add.n	a4, a15, a4
     cfb:	116707                                    	bbci	a7, 0, d10 <SMART_PITCH_PLC_PROCESS+0x918>
     cfe:	102452                                    	l32i	a5, a4, 64
     d01:	0498                                    	l32i.n	a9, a4, 0
     d03:	444b                                    	addi.n	a4, a4, 4
     d05:	805950                                    	add	a5, a9, a5
     d08:	418f50                                    	srli	a8, a5, 15
     d0b:	638d                                    	ae_s16i.n	a8, a3, 0
     d0d:	02c332                                    	addi	a3, a3, 2
     d10:	079216                                    	beqz	a2, d8d <SMART_PITCH_PLC_PROCESS+0x995>
     d13:	102472                                    	l32i	a7, a4, 64
     d16:	1498                                    	l32i.n	a9, a4, 4
     d18:	04a8                                    	l32i.n	a10, a4, 0
     d1a:	112452                                    	l32i	a5, a4, 68
     d1d:	448b                                    	addi.n	a4, a4, 8
     d1f:	7a7a                                    	add.n	a7, a10, a7
     d21:	0024a2                                    	l32i	a10, a4, 0
     d24:	805950                                    	add	a5, a9, a5
     d27:	416f70                                    	srli	a6, a7, 15
     d2a:	418f50                                    	srli	a8, a5, 15
     d2d:	5622b6                                    	bltui	a2, 2, d87 <SMART_PITCH_PLC_PROCESS+0x98f>
     d30:	102472                                    	l32i	a7, a4, 64
     d33:	012492                                    	l32i	a9, a4, 4
     d36:	112452                                    	l32i	a5, a4, 68
     d39:	08c442                                    	addi	a4, a4, 8
     d3c:	807a70                                    	add	a7, a10, a7
     d3f:	fec2a2                                    	addi	a10, a2, -2
     d42:	202550                                    	or	a2, a5, a5
     d45:	2f9a76                                    	loopnez	a10, d78 <SMART_PITCH_PLC_PROCESS+0x980>
     d48:	0403a4400084136e                  	{ s16i	a6, a3, 0; l32i	a5, a4, 64 }
     d50:	26069fc00495147e                  	{ l32i	a7, a4, 0; srli	a6, a7, 15 }
     d58:	0206a9400892038e                  	{ s16i	a8, a3, 2; add	a8, a9, a2 }
     d60:	020283440c81142e                  	{ l32i	a2, a4, 68; addi	a3, a3, 4 }
     d68:	08069fc11cf6275e                  	{ add	a7, a7, a5; srli	a8, a8, 15 }
     d70:	040284400c82049e                  	{ l32i	a9, a4, 4; addi	a4, a4, 8 }

00000d78 <SMART_PITCH_PLC_PROCESS+0x980>:
     d78:	636d                                    	ae_s16i.n	a6, a3, 0
     d7a:	592a                                    	add.n	a5, a9, a2
     d7c:	738d                                    	ae_s16i.n	a8, a3, 2
     d7e:	416f70                                    	srli	a6, a7, 15
     d81:	04c332                                    	addi	a3, a3, 4
     d84:	418f50                                    	srli	a8, a5, 15

00000d87 <SMART_PITCH_PLC_PROCESS+0x98f>:
     d87:	636d                                    	ae_s16i.n	a6, a3, 0
     d89:	a34b                                    	addi.n	a10, a3, 4
     d8b:	738d                                    	ae_s16i.n	a8, a3, 2
     d8d:	c04ec0                                    	sub	a4, a14, a12
     d90:	112cf0                                    	slli	a2, a12, 1
     d93:	e0a652                                    	movi	a5, 0x6e0
     d96:	4814a6                                    	blti	a4, 1, de2 <SMART_PITCH_PLC_PROCESS+0x9ea>
     d99:	5f5a                                    	add.n	a5, a15, a5
     d9b:	413140                                    	srli	a3, a4, 1
     d9e:	652a                                    	add.n	a6, a5, a2
     da0:	056407                                    	bbci	a4, 0, da9 <SMART_PITCH_PLC_PROCESS+0x9b1>
     da3:	264d                                    	ae_l16si.n	a4, a6, 0
     da5:	654d                                    	ae_s16i.n	a4, a5, 0
     da7:	552b                                    	addi.n	a5, a5, 2
     da9:	452a                                    	add.n	a4, a5, a2
     dab:	33bc                                    	beqz.n	a3, de2 <SMART_PITCH_PLC_PROCESS+0x9ea>
     dad:	652b                                    	addi.n	a6, a5, 2
     daf:	244d                                    	ae_l16si.n	a4, a4, 0
     db1:	a62a                                    	add.n	a10, a6, a2
     db3:	654d                                    	ae_s16i.n	a4, a5, 0
     db5:	730b                                    	addi.n	a7, a3, -1
     db7:	2a4d                                    	ae_l16si.n	a4, a10, 0
     db9:	063d                                    	mov.n	a3, a6
     dbb:	562b                                    	addi.n	a5, a6, 2
     dbd:	1f9776                                    	loopnez	a7, de0 <SMART_PITCH_PLC_PROCESS+0x9e8>
     dc0:	3206a5400090034e                  	{ s16i	a4, a3, 0; add	a3, a5, a2 }
     dc8:	220285400080134e                  	{ l16si	a4, a3, 0; addi	a3, a5, 2 }
     dd0:	0206a3400091054e                  	{ s16i	a4, a5, 0; add	a4, a3, a2 }
     dd8:	240283400080144e                  	{ l16si	a4, a4, 0; addi	a5, a3, 2 }

00000de0 <SMART_PITCH_PLC_PROCESS+0x9e8>:
     de0:	634d                                    	ae_s16i.n	a4, a3, 0

00000de2 <SMART_PITCH_PLC_PROCESS+0x9ea>:
     de2:	d02e20                                    	subx2	a2, a14, a2
     de5:	1b1ca6                                    	blti	a12, 1, e04 <SMART_PITCH_PLC_PROCESS+0xa0c>
     de8:	e0a632                                    	movi	a3, 0x6e0
     deb:	22fa                                    	add.n	a2, a2, a15
     ded:	0b4d                                    	mov.n	a4, a11
     def:	323a                                    	add.n	a3, a2, a3
     df1:	0f8c76                                    	loop	a12, e04 <SMART_PITCH_PLC_PROCESS+0xa0c>
     df4:	240284400080042e                  	{ l16si	a2, a4, 0; addi	a4, a4, 2 }
     dfc:	2202a3400080132e                  	{ s16i	a2, a3, 0; addi	a3, a3, 2 }
     e04:	020c                                    	movi.n	a2, 0
     e06:	ffa032                                    	movi	a3, 255
     e09:	196f22                                    	s32i	a2, a15, 100
     e0c:	7fd332                                    	addmi	a3, a3, 0x7f00
     e0f:	1d6f22                                    	s32i	a2, a15, 116
     e12:	0dad                                    	mov.n	a10, a13
     e14:	fc8c21                                    	l32r	a2, 44 (1ec8 <plc_gainctrl>)
     e17:	0fdd                                    	mov.n	a13, a15
     e19:	1c6f32                                    	s32i	a3, a15, 112
     e1c:	1b6f32                                    	s32i	a3, a15, 108
     e1f:	0002e0                                    	callx8	a2
     e22:	f01d                                    	retw.n

00000e24 <SMART_PITCH_PLC_PROCESS+0xa2c>:
     e24:	00000000                                 .......

00000e2b <SMART_PITCH_PLC_PROCESS+0xa33>:
     e2b:	ffa022                                    	movi	a2, 255
     e2e:	05cd                                    	mov.n	a12, a5
     e30:	7fd222                                    	addmi	a2, a2, 0x7f00
     e33:	06bd                                    	mov.n	a11, a6
     e35:	1b6d22                                    	s32i	a2, a13, 108
     e38:	07ed                                    	mov.n	a14, a7
     e3a:	dca622                                    	movi	a2, 0x6dc
     e3d:	220760a410c5b5ce                  	{ blti.w15	a5, 2, 1516 <SMART_PITCH_PLC_PROCESS+0x111e>; nop }
     e45:	2141c0                                    	srai	a4, a12, 1
     e48:	fccb32                                    	addi	a3, a11, -4
     e4b:	2d2a                                    	add.n	a2, a13, a2
     e4d:	0f8476                                    	loop	a4, e60 <SMART_PITCH_PLC_PROCESS+0xa68>
     e50:	220760c00485d12e                  	{ ae_l16x2m.iu	aed0, a2, 4; nop }
     e58:	220740c004c5713e                  	{ ae_s16x2m.iu	aed0, a3, 4; nop }

00000e60 <SMART_PITCH_PLC_PROCESS+0xa68>:
     e60:	01ac86                                    	j	1516 <SMART_PITCH_PLC_PROCESS+0x111e>

00000e63 <SMART_PITCH_PLC_PROCESS+0xa6b>:
     e63:	00000000                                 .....

00000e68 <SMART_PITCH_PLC_PROCESS+0xa70>:
     e68:	7fd322                                    	addmi	a2, a3, 0x7f00
     e6b:	30a242                                    	movi	a4, 0x230
     e6e:	51f9                                    	s32i.n	a15, a1, 20
     e70:	af4a                                    	add.n	a10, a15, a4
     e72:	1b6f22                                    	s32i	a2, a15, 108
     e75:	122f32                                    	l32i	a3, a15, 72
     e78:	162fb2                                    	l32i	a11, a15, 88
     e7b:	172fc2                                    	l32i	a12, a15, 92
     e7e:	51f8                                    	l32i.n	a15, a1, 20
     e80:	fc7021                                    	l32r	a2, 40 (2210 <getfespeech>)
     e83:	07dd                                    	mov.n	a13, a7
     e85:	0aed                                    	mov.n	a14, a10
     e87:	71a9                                    	s32i.n	a10, a1, 28
     e89:	0002e0                                    	callx8	a2
     e8c:	5198                                    	l32i.n	a9, a1, 20
     e8e:	0ca2f2                                    	movi	a15, 0x20c
     e91:	21e170                                    	srai	a14, a7, 1
     e94:	06ad                                    	mov.n	a10, a6
     e96:	64a682                                    	movi	a8, 0x664
     e99:	222922                                    	l32i	a2, a9, 136
     e9c:	162942                                    	l32i	a4, a9, 88
     e9f:	182952                                    	l32i	a5, a9, 96
     ea2:	f9fa                                    	add.n	a15, a9, a15
     ea4:	323a                                    	add.n	a3, a2, a3
     ea6:	b42a                                    	add.n	a11, a4, a2
     ea8:	126932                                    	s32i	a3, a9, 72
     eab:	c0c5b0                                    	sub	a12, a5, a11
     eae:	30c122                                    	addi	a2, a1, 48
     eb1:	1669b2                                    	s32i	a11, a9, 88
     eb4:	0f3d                                    	mov.n	a3, a15
     eb6:	1769c2                                    	s32i	a12, a9, 92
     eb9:	4151e0                                    	srli	a5, a14, 1
     ebc:	220760a41405d7ce                  	{ blti.w15	a7, 2, fb9 <SMART_PITCH_PLC_PROCESS+0xbc1>; nop }
     ec4:	498a                                    	add.n	a4, a9, a8
     ec6:	2d6e07                                    	bbci	a14, 0, ef7 <SMART_PITCH_PLC_PROCESS+0xaff>
     ec9:	200474c00482d13e                  	{ ae_l16x2m.iu	aed0, a3, 4; ae_l16x2m.iu	aed2, a4, 4 }
     ed1:	0e9523db0d540c008111b1080c02015f 	{ nop; nop; ae_mul32.hh	aed1, aed0, aed2; ae_mul32.ll	aed0, aed0, aed2 }
     ee1:	000003f3008300fc562303002c60b3df 	{ ae_trunca32q48	a13, aed1; ae_trunca32q48	a8, aed0; nop }
     ef1:	02d9                                    	s32i.n	a13, a2, 0
     ef3:	1289                                    	s32i.n	a8, a2, 4
     ef5:	228b                                    	addi.n	a2, a2, 8
     ef7:	07dd                                    	mov.n	a13, a7
     ef9:	0bc516                                    	beqz	a5, fb9 <SMART_PITCH_PLC_PROCESS+0xbc1>
     efc:	200474c00481d13e                  	{ ae_l16x2m.iu	aed0, a3, 4; ae_l16x2m.iu	aed1, a4, 4 }
     f04:	0e94e3d7152b41008991b11a00018d3f 	{ ae_l16x2m.iu	aed30, a3, 4; ae_l16x2m.iu	aed2, a4, 4; ae_mul32.hh	aed3, aed0, aed1; ae_mul32.ll	aed4, aed0, aed1 }
     f14:	1a81b3fd3fc2f58e                  	{ addi	a8, a5, -1; nop; ae_mul32.hh	aed31, aed30, aed2 }
     f1c:	000003d3108300fbd63b00003c62b36f 	{ ae_trunca32q48	a6, aed3; ae_trunca32q48	a7, aed4; ae_mul32.ll	aed0, aed30, aed2 }
     f2c:	1279                                    	s32i.n	a7, a2, 4
     f2e:	065d                                    	mov.n	a5, a6
     f30:	000003f37a0200fbf1e303032460382f 	{ loopnez	a8, fa0 <SMART_PITCH_PLC_PROCESS+0xba8>; ae_trunca32q48	a7, aed31; nop }
     f40:	0eace577052231d0c400813b0c01660f 	{ ae_trunca32q48	a6, aed0; ae_l16x2m.iu	aed0, a3, 4; nop; nop }
     f50:	210434c00081125e                  	{ s32i	a5, a2, 0; ae_l16x2m.iu	aed1, a4, 4 }
     f58:	0e94c3d719ea31008311b1051401027f 	{ s32i	a7, a2, 8; ae_l16x2m.iu	aed3, a3, 4; ae_mul32.hh	aed0, aed0, aed1; ae_mul32.ll	aed1, aed0, aed1 }
     f68:	210434c01882126e                  	{ s32i	a6, a2, 12; ae_l16x2m.iu	aed2, a4, 4 }
     f70:	0e94e7c0852222d0c400910b0c62651f 	{ ae_trunca32q48	a5, aed1; addi	a2, a2, 16; ae_mul32.hh	aed1, aed3, aed2; nop }
     f80:	00000073038300fad6bb00002c62125f 	{ s32i	a5, a2, 4; ae_trunca32q48	a5, aed0; ae_mul32.ll	aed0, aed3, aed2 }
     f90:	0ead257b0d240cd0c400813b0c01671f 	{ ae_trunca32q48	a7, aed1; nop; nop; nop }

00000fa0 <SMART_PITCH_PLC_PROCESS+0xba8>:
     fa0:	000003f3038300fc56a303002460125f 	{ s32i	a5, a2, 0; ae_trunca32q48	a8, aed0; nop }
     fb0:	2279                                    	s32i.n	a7, a2, 8
     fb2:	0d7d                                    	mov.n	a7, a13
     fb4:	3289                                    	s32i.n	a8, a2, 12
     fb6:	10c282                                    	addi	a8, a2, 16

00000fb9 <SMART_PITCH_PLC_PROCESS+0xbc1>:
     fb9:	eca152                                    	movi	a5, 0x1ec
     fbc:	c02c70                                    	sub	a2, a12, a7
     fbf:	30a232                                    	movi	a3, 0x230
     fc2:	1c17a6                                    	blti	a7, 1, fe2 <SMART_PITCH_PLC_PROCESS+0xbea>
     fc5:	902290                                    	addx2	a2, a2, a9
     fc8:	30a162                                    	movi	a6, 0x130
     fcb:	423a                                    	add.n	a4, a2, a3
     fcd:	396a                                    	add.n	a3, a9, a6
     fcf:	0f8776                                    	loop	a7, fe2 <SMART_PITCH_PLC_PROCESS+0xbea>
     fd2:	240284400080042e                  	{ l16si	a2, a4, 0; addi	a4, a4, 2 }
     fda:	2202a3400080132e                  	{ s16i	a2, a3, 0; addi	a3, a3, 2 }
     fe2:	d95a                                    	add.n	a13, a9, a5
     fe4:	2ca142                                    	movi	a4, 0x12c
     fe7:	2cc122                                    	addi	a2, a1, 44
     fea:	230740a0140517ce                  	{ blti.w15	a7, 2, 1090 <SMART_PITCH_PLC_PROCESS+0xc98>; nop }
     ff2:	0d3d                                    	mov.n	a3, a13
     ff4:	494a                                    	add.n	a4, a9, a4
     ff6:	200474c00481d13e                  	{ ae_l16x2m.iu	aed0, a3, 4; ae_l16x2m.iu	aed1, a4, 4 }
     ffe:	3902aec010df612e                  	{ ae_l32m.iu	aed2, a2, 4; addi	a8, a14, -1 }
    1006:	1c8345133401b13e                  	{ nop; nop; ae_mula32.hh	aed2, aed0, aed1 }
    100e:	0ead257b0d240cd0c400813b0c01652f 	{ ae_trunca32q48	a5, aed2; nop; nop; nop }
    101e:	0259                                    	s32i.n	a5, a2, 0
    1020:	1105424400a2588e                  	{ loopnez	a8, 1074 <SMART_PITCH_PLC_PROCESS+0xc7c>; ae_l32m.iu	aed2, a2, 4 }
    1028:	1c8227153801013e                  	{ ae_l16x2m.iu	aed0, a3, 4; nop; ae_mula32.ll	aed2, aed0, aed1 }
    1030:	220760c00c85d14e                  	{ ae_l16x2m.iu	aed1, a4, 4; nop }
    1038:	0ead257b0d240cd0c400813b0c01652f 	{ ae_trunca32q48	a5, aed2; nop; nop; nop }
    1048:	0259                                    	s32i.n	a5, a2, 0
    104a:	230720c010c5712e                  	{ ae_l32m.iu	aed2, a2, 4; nop }
    1052:	1c8345133401b13e                  	{ nop; nop; ae_mula32.hh	aed2, aed0, aed1 }
    105a:	0ead257b0d240cd0c400813b0c01652f 	{ ae_trunca32q48	a5, aed2; nop; nop; nop }
    106a:	0259                                    	s32i.n	a5, a2, 0
    106c:	230720c010c5712e                  	{ ae_l32m.iu	aed2, a2, 4; nop }

00001074 <SMART_PITCH_PLC_PROCESS+0xc7c>:
    1074:	1c8347133401b13e                  	{ nop; nop; ae_mula32.ll	aed2, aed0, aed1 }
    107c:	f03d                                    	nop.n
    107e:	0ead257b0d240cd0c400813b0c01632f 	{ ae_trunca32q48	a3, aed2; nop; nop; nop }
    108e:	0239                                    	s32i.n	a3, a2, 0

00001090 <SMART_PITCH_PLC_PROCESS+0xc98>:
    1090:	1127f0                                    	slli	a2, a7, 1
    1093:	81e9                                    	s32i.n	a14, a1, 32
    1095:	30a242                                    	movi	a4, 0x230
    1098:	5517a6                                    	blti	a7, 1, 10f1 <SMART_PITCH_PLC_PROCESS+0xcf9>
    109b:	d02a20                                    	subx2	a2, a10, a2
    109e:	30c132                                    	addi	a3, a1, 48
    10a1:	229a                                    	add.n	a2, a2, a9
    10a3:	424a                                    	add.n	a4, a2, a4
    10a5:	412170                                    	srli	a2, a7, 1
    10a8:	0a6707                                    	bbci	a7, 0, 10b6 <SMART_PITCH_PLC_PROCESS+0xcbe>
    10ab:	0368                                    	l32i.n	a6, a3, 0
    10ad:	334b                                    	addi.n	a3, a3, 4
    10af:	416f60                                    	srli	a6, a6, 15
    10b2:	646d                                    	ae_s16i.n	a6, a4, 0
    10b4:	442b                                    	addi.n	a4, a4, 2
    10b6:	820b                                    	addi.n	a8, a2, -1
    10b8:	52bc                                    	beqz.n	a2, 10f1 <SMART_PITCH_PLC_PROCESS+0xcf9>
    10ba:	0358                                    	l32i.n	a5, a3, 0
    10bc:	1368                                    	l32i.n	a6, a3, 4
    10be:	338b                                    	addi.n	a3, a3, 8
    10c0:	415f50                                    	srli	a5, a5, 15
    10c3:	062d                                    	mov.n	a2, a6
    10c5:	1f9876                                    	loopnez	a8, 10e8 <SMART_PITCH_PLC_PROCESS+0xcf0>
    10c8:	0403a3400080145e                  	{ s16i	a5, a4, 0; l32i	a5, a3, 0 }
    10d0:	1303a34114a04f2e                  	{ srli	a6, a2, 15; l32i	a2, a3, 4 }
    10d8:	0402a4400881046e                  	{ s16i	a6, a4, 2; addi	a4, a4, 4 }
    10e0:	0302a3410ca25f5e                  	{ srli	a5, a5, 15; addi	a3, a3, 8 }

000010e8 <SMART_PITCH_PLC_PROCESS+0xcf0>:
    10e8:	416f20                                    	srli	a6, a2, 15
    10eb:	645d                                    	ae_s16i.n	a5, a4, 0
    10ed:	e44b                                    	addi.n	a14, a4, 4
    10ef:	746d                                    	ae_s16i.n	a6, a4, 2

000010f1 <SMART_PITCH_PLC_PROCESS+0xcf9>:
    10f1:	e0a622                                    	movi	a2, 0x6e0
    10f4:	a1d9                                    	s32i.n	a13, a1, 40
    10f6:	073d                                    	mov.n	a3, a7
    10f8:	205ff0                                    	or	a5, a15, a15
    10fb:	207aa0                                    	or	a7, a10, a10
    10fe:	80e920                                    	add	a14, a9, a2
    1101:	0721a2                                    	l32i	a10, a1, 28
    1104:	70a142                                    	movi	a4, 0x170
    1107:	0621d2                                    	l32i	a13, a1, 24
    110a:	2ca262                                    	movi	a6, 0x22c
    110d:	0521f2                                    	l32i	a15, a1, 20
    1110:	fbcc21                                    	l32r	a2, 40 (2210 <getfespeech>)
    1113:	0020f0                                    	nop
    1116:	0002e0                                    	callx8	a2
    1119:	51d8                                    	l32i.n	a13, a1, 20
    111b:	91b8                                    	l32i.n	a11, a1, 36
    111d:	03fd                                    	mov.n	a15, a3
    111f:	220740a0100513ce                  	{ blti.w15	a3, 2, 11a3 <SMART_PITCH_PLC_PROCESS+0xdab>; nop }
    1127:	8188                                    	l32i.n	a8, a1, 32
    1129:	4d4a                                    	add.n	a4, a13, a4
    112b:	6d6a                                    	add.n	a6, a13, a6
    112d:	318876                                    	loop	a8, 1162 <SMART_PITCH_PLC_PROCESS+0xd6a>
    1130:	200476c00c80d15e                  	{ ae_l16x2m.iu	aed1, a5, 4; ae_l16x2m.iu	aed0, a6, 4 }
    1138:	0e9523db0d440c088311b1000c20015f 	{ nop; nop; ae_mul32.hh	aed0, aed1, aed0; ae_mul32.ll	aed1, aed1, aed0 }
    1148:	000003f3008300f9f62303002460b32f 	{ ae_trunca32q48	a2, aed0; ae_trunca32q48	a3, aed1; nop }
    1158:	0429                                    	s32i.n	a2, a4, 0
    115a:	0502a4400882043e                  	{ s32i	a3, a4, 4; addi	a4, a4, 8 }

00001162 <SMART_PITCH_PLC_PROCESS+0xd6a>:
    1162:	b0a122                                    	movi	a2, 0x1b0
    1165:	a158                                    	l32i.n	a5, a1, 40
    1167:	dca632                                    	movi	a3, 0x6dc
    116a:	4d2a                                    	add.n	a4, a13, a2
    116c:	6d3a                                    	add.n	a6, a13, a3
    116e:	318876                                    	loop	a8, 11a3 <SMART_PITCH_PLC_PROCESS+0xdab>
    1171:	200476c00c80d15e                  	{ ae_l16x2m.iu	aed1, a5, 4; ae_l16x2m.iu	aed0, a6, 4 }
    1179:	0e9523db0d440c088311b1000c20015f 	{ nop; nop; ae_mul32.hh	aed0, aed1, aed0; ae_mul32.ll	aed1, aed1, aed0 }
    1189:	000003f3008300f9f62303002460b32f 	{ ae_trunca32q48	a2, aed0; ae_trunca32q48	a3, aed1; nop }
    1199:	0429                                    	s32i.n	a2, a4, 0
    119b:	0502a4400882043e                  	{ s32i	a3, a4, 4; addi	a4, a4, 8 }

000011a3 <SMART_PITCH_PLC_PROCESS+0xdab>:
    11a3:	1d2d22                                    	l32i	a2, a13, 116
    11a6:	ed38                                    	l32i.n	a3, a13, 56
    11a8:	61c8                                    	l32i.n	a12, a1, 24
    11aa:	07ed                                    	mov.n	a14, a7
    11ac:	0b4d                                    	mov.n	a4, a11
    11ae:	0f6d                                    	mov.n	a6, a15
    11b0:	0a1226                                    	beqi	a2, 1, 11be <SMART_PITCH_PLC_PROCESS+0xdc6>
    11b3:	412160                                    	srli	a2, a6, 1
    11b6:	220760a00c65736e                  	{ bnei.w15	a3, 1, 15a6 <SMART_PITCH_PLC_PROCESS+0x11ae>; nop }
    11be:	61c8                                    	l32i.n	a12, a1, 24
    11c0:	07ed                                    	mov.n	a14, a7
    11c2:	207ff0                                    	or	a7, a15, a15
    11c5:	230700a40c251fce                  	{ blti.w15	a15, 1, 12ec <SMART_PITCH_PLC_PROCESS+0xef4>; nop }
    11cd:	70a122                                    	movi	a2, 0x170
    11d0:	208770                                    	or	a8, a7, a7
    11d3:	804d20                                    	add	a4, a13, a2
    11d6:	10c132                                    	addi	a3, a1, 16
    11d9:	0024a2                                    	l32i	a10, a4, 0
    11dc:	6ccd22                                    	addi	a2, a13, 108
    11df:	102462                                    	l32i	a6, a4, 64
    11e2:	205bb0                                    	or	a5, a11, a11
    11e5:	04c442                                    	addi	a4, a4, 4
    11e8:	0104324118a12fae                  	{ srai	a7, a10, 15; ae_l16m.i	aed1, a2, 0 }
    11f0:	0304324110a32f6e                  	{ srai	a6, a6, 15; ae_l16m.i	aed3, a2, 4 }
    11f8:	046172                                    	s32i	a7, a1, 16
    11fb:	010433410080016e                  	{ s32i	a6, a1, 16; ae_l16m.i	aed0, a3, 0 }
    1203:	000453241402c82e                  	{ bltui.w15	a8, 2, 12c0 <SMART_PITCH_PLC_PROCESS+0xec8>; ae_l16m.i	aed2, a3, 0 }
    120b:	002472                                    	l32i	a7, a4, 0
    120e:	fecfa2                                    	addi	a10, a15, -2
    1211:	102462                                    	l32i	a6, a4, 64
    1214:	04c442                                    	addi	a4, a4, 4
    1217:	217f70                                    	srai	a7, a7, 15
    121a:	046172                                    	s32i	a7, a1, 16
    121d:	579a76                                    	loopnez	a10, 1278 <SMART_PITCH_PLC_PROCESS+0xe80>
    1220:	0e9543c639e8f6108511b3080403049f 	{ l32i	a9, a4, 0; srai	a7, a6, 15; ae_mul32.ll	aed1, aed0, aed3; ae_mul32.ll	aed2, aed2, aed1 }
    1230:	220760c00485703e                  	{ ae_l16m.i	aed0, a3, 0; nop }
    1238:	000003f3088300fb562303002c60b38f 	{ ae_trunca32q48	a8, aed1; ae_trunca32q48	a6, aed2; nop }
    1248:	3906af410089117e                  	{ s32i	a7, a1, 16; srai	a7, a9, 15 }
    1250:	010433410082017e                  	{ s32i	a7, a1, 16; ae_l16m.i	aed2, a3, 0 }
    1258:	380686440491046e                  	{ l32i	a6, a4, 64; add	a7, a6, a8 }
    1260:	0502a4411ca14f7e                  	{ srli	a7, a7, 15; addi	a4, a4, 4 }
    1268:	2402a5400080157e                  	{ s16i	a7, a5, 0; addi	a5, a5, 2 }
    1270:	000472401c81622e                  	{ ae_l16m.i	aed3, a2, 4; ae_l16m.i	aed1, a2, 0 }

00001278 <SMART_PITCH_PLC_PROCESS+0xe80>:
    1278:	0e94e3d0052b30108211b31c0c036f6f 	{ srai	a6, a6, 15; ae_l16m.i	aed0, a3, 0; ae_mul32.ll	aed3, aed0, aed3; ae_mul32.ll	aed1, aed2, aed1 }
    1288:	4169                                    	s32i.n	a6, a1, 16
    128a:	000003f3008300fbf62303003c60b34f 	{ ae_trunca32q48	a4, aed3; ae_trunca32q48	a7, aed1; nop }
    129a:	230720c318e5747e                  	{ nop; nop }
    12a2:	0020f0                                    	nop
    12a5:	0020f0                                    	nop
    12a8:	804740                                    	add	a4, a7, a4
    12ab:	41af40                                    	srli	a10, a4, 15
    12ae:	65ad                                    	ae_s16i.n	a10, a5, 0
    12b0:	2402e5401c80722e                  	{ ae_l16m.i	aed3, a2, 4; addi	a5, a5, 2 }
    12b8:	000473400c82602e                  	{ ae_l16m.i	aed1, a2, 0; ae_l16m.i	aed2, a3, 0 }

000012c0 <SMART_PITCH_PLC_PROCESS+0xec8>:
    12c0:	0e9523db0c6c0c108211b318140305af 	{ addi	a10, a5, 2; nop; ae_mul32.ll	aed3, aed0, aed3; ae_mul32.ll	aed1, aed2, aed1 }
    12d0:	000003f3008300fbf62303003c60b34f 	{ ae_trunca32q48	a4, aed3; ae_trunca32q48	a7, aed1; nop }
    12e0:	0020f0                                    	nop
    12e3:	474a                                    	add.n	a4, a7, a4
    12e5:	0f7d                                    	mov.n	a7, a15
    12e7:	413f40                                    	srli	a3, a4, 15
    12ea:	653d                                    	ae_s16i.n	a3, a5, 0
    12ec:	c05c70                                    	sub	a5, a12, a7
    12ef:	9047d0                                    	addx2	a4, a7, a13
    12f2:	e0a662                                    	movi	a6, 0x6e0
    12f5:	230720a40c6555ce                  	{ blti.w15	a5, 1, 1664 <SMART_PITCH_PLC_PROCESS+0x126c>; nop }
    12fd:	1b2d22                                    	l32i	a2, a13, 108
    1300:	9037b0                                    	addx2	a3, a7, a11
    1303:	446a                                    	add.n	a4, a4, a6
    1305:	158576                                    	loop	a5, 131e <SMART_PITCH_PLC_PROCESS+0xf26>
    1308:	240284400080045e                  	{ l16si	a5, a4, 0; addi	a4, a4, 2 }
    1310:	825250                                    	mull	a5, a2, a5
    1313:	415f50                                    	srli	a5, a5, 15
    1316:	2202a3400080135e                  	{ s16i	a5, a3, 0; addi	a3, a3, 2 }

0000131e <SMART_PITCH_PLC_PROCESS+0xf26>:
    131e:	00d086                                    	j	1664 <SMART_PITCH_PLC_PROCESS+0x126c>

00001321 <SMART_PITCH_PLC_PROCESS+0xf29>:
    1321:	00000000                                 .......

00001328 <SMART_PITCH_PLC_PROCESS+0xf30>:
    1328:	06bd                                    	mov.n	a11, a6
    132a:	5168                                    	l32i.n	a6, a1, 20
    132c:	05cd                                    	mov.n	a12, a5
    132e:	07ed                                    	mov.n	a14, a7
    1330:	0ca252                                    	movi	a5, 0x20c
    1333:	1b6da2                                    	s32i	a10, a13, 108
    1336:	70a122                                    	movi	a2, 0x170
    1339:	230740a4100556ce                  	{ blti.w15	a6, 2, 13c8 <SMART_PITCH_PLC_PROCESS+0xfd0>; nop }
    1341:	217160                                    	srai	a7, a6, 1
    1344:	2ca232                                    	movi	a3, 0x22c
    1347:	6d3a                                    	add.n	a6, a13, a3
    1349:	4d2a                                    	add.n	a4, a13, a2
    134b:	5d5a                                    	add.n	a5, a13, a5
    134d:	318776                                    	loop	a7, 1382 <SMART_PITCH_PLC_PROCESS+0xf8a>
    1350:	200476c00c80d15e                  	{ ae_l16x2m.iu	aed1, a5, 4; ae_l16x2m.iu	aed0, a6, 4 }
    1358:	0e9523db0d440c088311b1000c20015f 	{ nop; nop; ae_mul32.hh	aed0, aed1, aed0; ae_mul32.ll	aed1, aed1, aed0 }
    1368:	000003f3008300f9f62303002460b32f 	{ ae_trunca32q48	a2, aed0; ae_trunca32q48	a3, aed1; nop }
    1378:	0429                                    	s32i.n	a2, a4, 0
    137a:	0502a4400882043e                  	{ s32i	a3, a4, 4; addi	a4, a4, 8 }

00001382 <SMART_PITCH_PLC_PROCESS+0xf8a>:
    1382:	b0a122                                    	movi	a2, 0x1b0
    1385:	eca152                                    	movi	a5, 0x1ec
    1388:	dca632                                    	movi	a3, 0x6dc
    138b:	4d2a                                    	add.n	a4, a13, a2
    138d:	6d3a                                    	add.n	a6, a13, a3
    138f:	5d5a                                    	add.n	a5, a13, a5
    1391:	318776                                    	loop	a7, 13c6 <SMART_PITCH_PLC_PROCESS+0xfce>
    1394:	200476c00c80d15e                  	{ ae_l16x2m.iu	aed1, a5, 4; ae_l16x2m.iu	aed0, a6, 4 }
    139c:	0e9523db0d440c088311b1000c20015f 	{ nop; nop; ae_mul32.hh	aed0, aed1, aed0; ae_mul32.ll	aed1, aed1, aed0 }
    13ac:	000003f3008300f9f62303002460b32f 	{ ae_trunca32q48	a2, aed0; ae_trunca32q48	a3, aed1; nop }
    13bc:	0429                                    	s32i.n	a2, a4, 0
    13be:	0502a4400882043e                  	{ s32i	a3, a4, 4; addi	a4, a4, 8 }
    13c6:	5168                                    	l32i.n	a6, a1, 20

000013c8 <SMART_PITCH_PLC_PROCESS+0xfd0>:
    13c8:	70a122                                    	movi	a2, 0x170
    13cb:	230700a40825d6ce                  	{ blti.w15	a6, 1, 14ea <SMART_PITCH_PLC_PROCESS+0x10f2>; nop }
    13d3:	4d2a                                    	add.n	a4, a13, a2
    13d5:	10c132                                    	addi	a3, a1, 16
    13d8:	0478                                    	l32i.n	a7, a4, 0
    13da:	068d                                    	mov.n	a8, a6
    13dc:	1024f2                                    	l32i	a15, a4, 64
    13df:	6ccd22                                    	addi	a2, a13, 108
    13e2:	0b5d                                    	mov.n	a5, a11
    13e4:	0104324118a12f7e                  	{ srai	a7, a7, 15; ae_l16m.i	aed1, a2, 0 }
    13ec:	4179                                    	s32i.n	a7, a1, 16
    13ee:	0304324110a32ffe                  	{ srai	a6, a15, 15; ae_l16m.i	aed3, a2, 4 }
    13f6:	000473410460044e                  	{ addi	a4, a4, 4; ae_l16m.i	aed0, a3, 0 }
    13fe:	0478                                    	l32i.n	a7, a4, 0
    1400:	046162                                    	s32i	a6, a1, 16
    1403:	000453241402c82e                  	{ bltui.w15	a8, 2, 14c0 <SMART_PITCH_PLC_PROCESS+0x10c8>; ae_l16m.i	aed2, a3, 0 }
    140b:	0521f2                                    	l32i	a15, a1, 20
    140e:	102462                                    	l32i	a6, a4, 64
    1411:	217f70                                    	srai	a7, a7, 15
    1414:	04c442                                    	addi	a4, a4, 4
    1417:	046172                                    	s32i	a7, a1, 16
    141a:	fecf72                                    	addi	a7, a15, -2
    141d:	579776                                    	loopnez	a7, 1478 <SMART_PITCH_PLC_PROCESS+0x1080>
    1420:	0e9543c639e8f6108511b3080403049f 	{ l32i	a9, a4, 0; srai	a7, a6, 15; ae_mul32.ll	aed1, aed0, aed3; ae_mul32.ll	aed2, aed2, aed1 }
    1430:	220760c00485703e                  	{ ae_l16m.i	aed0, a3, 0; nop }
    1438:	000003f3088300fb562303002c60b38f 	{ ae_trunca32q48	a8, aed1; ae_trunca32q48	a6, aed2; nop }
    1448:	3906af410089117e                  	{ s32i	a7, a1, 16; srai	a7, a9, 15 }
    1450:	010433410082017e                  	{ s32i	a7, a1, 16; ae_l16m.i	aed2, a3, 0 }
    1458:	380686440491046e                  	{ l32i	a6, a4, 64; add	a7, a6, a8 }
    1460:	0502a4411ca14f7e                  	{ srli	a7, a7, 15; addi	a4, a4, 4 }
    1468:	2402a5400080157e                  	{ s16i	a7, a5, 0; addi	a5, a5, 2 }
    1470:	000472401c81622e                  	{ ae_l16m.i	aed3, a2, 4; ae_l16m.i	aed1, a2, 0 }

00001478 <SMART_PITCH_PLC_PROCESS+0x1080>:
    1478:	0e94e3d0052b30108211b31c0c036f6f 	{ srai	a6, a6, 15; ae_l16m.i	aed0, a3, 0; ae_mul32.ll	aed3, aed0, aed3; ae_mul32.ll	aed1, aed2, aed1 }
    1488:	4169                                    	s32i.n	a6, a1, 16
    148a:	000003f3008300fbf62303003c60b34f 	{ ae_trunca32q48	a4, aed3; ae_trunca32q48	a7, aed1; nop }
    149a:	230720c318e5747e                  	{ nop; nop }
    14a2:	0020f0                                    	nop
    14a5:	0020f0                                    	nop
    14a8:	804740                                    	add	a4, a7, a4
    14ab:	41ff40                                    	srli	a15, a4, 15
    14ae:	65fd                                    	ae_s16i.n	a15, a5, 0
    14b0:	2402e5401c80722e                  	{ ae_l16m.i	aed3, a2, 4; addi	a5, a5, 2 }
    14b8:	000473400c82602e                  	{ ae_l16m.i	aed1, a2, 0; ae_l16m.i	aed2, a3, 0 }

000014c0 <SMART_PITCH_PLC_PROCESS+0x10c8>:
    14c0:	0e94c3c011ea5f108311b3180c03116f 	{ l32i	a6, a1, 20; addi	a15, a5, 2; ae_mul32.ll	aed3, aed0, aed3; ae_mul32.ll	aed1, aed2, aed1 }
    14d0:	000003f3008300fbf62303003c60b34f 	{ ae_trunca32q48	a4, aed3; ae_trunca32q48	a7, aed1; nop }
    14e0:	0020f0                                    	nop
    14e3:	474a                                    	add.n	a4, a7, a4
    14e5:	413f40                                    	srli	a3, a4, 15
    14e8:	653d                                    	ae_s16i.n	a3, a5, 0
    14ea:	c05c60                                    	sub	a5, a12, a6
    14ed:	9026d0                                    	addx2	a2, a6, a13
    14f0:	e0a642                                    	movi	a4, 0x6e0
    14f3:	1f15a6                                    	blti	a5, 1, 1516 <SMART_PITCH_PLC_PROCESS+0x111e>
    14f6:	424a                                    	add.n	a4, a2, a4
    14f8:	9036b0                                    	addx2	a3, a6, a11
    14fb:	0a2d                                    	mov.n	a2, a10
    14fd:	158576                                    	loop	a5, 1516 <SMART_PITCH_PLC_PROCESS+0x111e>
    1500:	240284400080045e                  	{ l16si	a5, a4, 0; addi	a4, a4, 2 }
    1508:	825250                                    	mull	a5, a2, a5
    150b:	415f50                                    	srli	a5, a5, 15
    150e:	2202a3400080135e                  	{ s16i	a5, a3, 0; addi	a3, a3, 2 }
    1516:	c04ec0                                    	sub	a4, a14, a12
    1519:	e0a652                                    	movi	a5, 0x6e0
    151c:	112cf0                                    	slli	a2, a12, 1
    151f:	4f14a6                                    	blti	a4, 1, 1572 <SMART_PITCH_PLC_PROCESS+0x117a>
    1522:	5d5a                                    	add.n	a5, a13, a5
    1524:	413140                                    	srli	a3, a4, 1
    1527:	652a                                    	add.n	a6, a5, a2
    1529:	056407                                    	bbci	a4, 0, 1532 <SMART_PITCH_PLC_PROCESS+0x113a>
    152c:	264d                                    	ae_l16si.n	a4, a6, 0
    152e:	654d                                    	ae_s16i.n	a4, a5, 0
    1530:	552b                                    	addi.n	a5, a5, 2
    1532:	452a                                    	add.n	a4, a5, a2
    1534:	a3bc                                    	beqz.n	a3, 1572 <SMART_PITCH_PLC_PROCESS+0x117a>
    1536:	652b                                    	addi.n	a6, a5, 2
    1538:	244d                                    	ae_l16si.n	a4, a4, 0
    153a:	f62a                                    	add.n	a15, a6, a2
    153c:	654d                                    	ae_s16i.n	a4, a5, 0
    153e:	730b                                    	addi.n	a7, a3, -1
    1540:	2f4d                                    	ae_l16si.n	a4, a15, 0
    1542:	063d                                    	mov.n	a3, a6
    1544:	562b                                    	addi.n	a5, a6, 2
    1546:	0020f0                                    	nop
    1549:	f03d                                    	nop.n
    154b:	f03d                                    	nop.n
    154d:	1f9776                                    	loopnez	a7, 1570 <SMART_PITCH_PLC_PROCESS+0x1178>
    1550:	3206a5400090034e                  	{ s16i	a4, a3, 0; add	a3, a5, a2 }
    1558:	220285400080134e                  	{ l16si	a4, a3, 0; addi	a3, a5, 2 }
    1560:	0206a3400091054e                  	{ s16i	a4, a5, 0; add	a4, a3, a2 }
    1568:	240283400080144e                  	{ l16si	a4, a4, 0; addi	a5, a3, 2 }

00001570 <SMART_PITCH_PLC_PROCESS+0x1178>:
    1570:	634d                                    	ae_s16i.n	a4, a3, 0

00001572 <SMART_PITCH_PLC_PROCESS+0x117a>:
    1572:	112cf0                                    	slli	a2, a12, 1
    1575:	230720a40c253cce                  	{ blti.w15	a12, 1, 16e0 <SMART_PITCH_PLC_PROCESS+0x12e8>; nop }
    157d:	d02e20                                    	subx2	a2, a14, a2
    1580:	e0a632                                    	movi	a3, 0x6e0
    1583:	22da                                    	add.n	a2, a2, a13
    1585:	0b4d                                    	mov.n	a4, a11
    1587:	323a                                    	add.n	a3, a2, a3
    1589:	f03d                                    	nop.n
    158b:	f03d                                    	nop.n
    158d:	0f8c76                                    	loop	a12, 15a0 <SMART_PITCH_PLC_PROCESS+0x11a8>
    1590:	240284400080042e                  	{ l16si	a2, a4, 0; addi	a4, a4, 2 }
    1598:	2202a3400080132e                  	{ s16i	a2, a3, 0; addi	a3, a3, 2 }

000015a0 <SMART_PITCH_PLC_PROCESS+0x11a8>:
    15a0:	004f06                                    	j	16e0 <SMART_PITCH_PLC_PROCESS+0x12e8>

000015a3 <SMART_PITCH_PLC_PROCESS+0x11ab>:
    15a3:	000000                                        ...

000015a6 <SMART_PITCH_PLC_PROCESS+0x11ae>:
    15a6:	70a132                                    	movi	a3, 0x170
    15a9:	230740a408059fce                  	{ blti.w15	a15, 1, 1640 <SMART_PITCH_PLC_PROCESS+0x1248>; nop }
    15b1:	3d3a                                    	add.n	a3, a13, a3
    15b3:	0f6607                                    	bbci	a6, 0, 15c6 <SMART_PITCH_PLC_PROCESS+0x11ce>
    15b6:	0358                                    	l32i.n	a5, a3, 0
    15b8:	102392                                    	l32i	a9, a3, 64
    15bb:	334b                                    	addi.n	a3, a3, 4
    15bd:	595a                                    	add.n	a5, a9, a5
    15bf:	418f50                                    	srli	a8, a5, 15
    15c2:	648d                                    	ae_s16i.n	a8, a4, 0
    15c4:	442b                                    	addi.n	a4, a4, 2
    15c6:	076216                                    	beqz	a2, 1640 <SMART_PITCH_PLC_PROCESS+0x1248>
    15c9:	0378                                    	l32i.n	a7, a3, 0
    15cb:	1023a2                                    	l32i	a10, a3, 64
    15ce:	1358                                    	l32i.n	a5, a3, 4
    15d0:	112392                                    	l32i	a9, a3, 68
    15d3:	338b                                    	addi.n	a3, a3, 8
    15d5:	7a7a                                    	add.n	a7, a10, a7
    15d7:	1023a2                                    	l32i	a10, a3, 64
    15da:	595a                                    	add.n	a5, a9, a5
    15dc:	416f70                                    	srli	a6, a7, 15
    15df:	418f50                                    	srli	a8, a5, 15
    15e2:	5022b6                                    	bltui	a2, 2, 1636 <SMART_PITCH_PLC_PROCESS+0x123e>
    15e5:	0378                                    	l32i.n	a7, a3, 0
    15e7:	1358                                    	l32i.n	a5, a3, 4
    15e9:	112392                                    	l32i	a9, a3, 68
    15ec:	338b                                    	addi.n	a3, a3, 8
    15ee:	7a7a                                    	add.n	a7, a10, a7
    15f0:	fec2a2                                    	addi	a10, a2, -2
    15f3:	052d                                    	mov.n	a2, a5
    15f5:	2f9a76                                    	loopnez	a10, 1628 <SMART_PITCH_PLC_PROCESS+0x1230>
    15f8:	0403a3400080146e                  	{ s16i	a6, a4, 0; l32i	a5, a3, 0 }
    1600:	26069fc40495137e                  	{ l32i	a7, a3, 64; srli	a6, a7, 15 }
    1608:	0206a9400892048e                  	{ s16i	a8, a4, 2; add	a8, a9, a2 }
    1610:	040284400c81032e                  	{ l32i	a2, a3, 4; addi	a4, a4, 4 }
    1618:	08069fc11cf6275e                  	{ add	a7, a7, a5; srli	a8, a8, 15 }
    1620:	020283440c82139e                  	{ l32i	a9, a3, 68; addi	a3, a3, 8 }

00001628 <SMART_PITCH_PLC_PROCESS+0x1230>:
    1628:	646d                                    	ae_s16i.n	a6, a4, 0
    162a:	592a                                    	add.n	a5, a9, a2
    162c:	748d                                    	ae_s16i.n	a8, a4, 2
    162e:	416f70                                    	srli	a6, a7, 15
    1631:	444b                                    	addi.n	a4, a4, 4
    1633:	418f50                                    	srli	a8, a5, 15

00001636 <SMART_PITCH_PLC_PROCESS+0x123e>:
    1636:	646d                                    	ae_s16i.n	a6, a4, 0
    1638:	206ff0                                    	or	a6, a15, a15
    163b:	748d                                    	ae_s16i.n	a8, a4, 2
    163d:	04c4a2                                    	addi	a10, a4, 4
    1640:	c02c60                                    	sub	a2, a12, a6
    1643:	9046d0                                    	addx2	a4, a6, a13
    1646:	e0a652                                    	movi	a5, 0x6e0
    1649:	1712a6                                    	blti	a2, 1, 1664 <SMART_PITCH_PLC_PROCESS+0x126c>
    164c:	445a                                    	add.n	a4, a4, a5
    164e:	9036b0                                    	addx2	a3, a6, a11
    1651:	0f8276                                    	loop	a2, 1664 <SMART_PITCH_PLC_PROCESS+0x126c>
    1654:	240284400080042e                  	{ l16si	a2, a4, 0; addi	a4, a4, 2 }
    165c:	2202a3400080132e                  	{ s16i	a2, a3, 0; addi	a3, a3, 2 }
    1664:	c04ec0                                    	sub	a4, a14, a12
    1667:	e0a652                                    	movi	a5, 0x6e0
    166a:	112cf0                                    	slli	a2, a12, 1
    166d:	4914a6                                    	blti	a4, 1, 16ba <SMART_PITCH_PLC_PROCESS+0x12c2>
    1670:	5d5a                                    	add.n	a5, a13, a5
    1672:	413140                                    	srli	a3, a4, 1
    1675:	652a                                    	add.n	a6, a5, a2
    1677:	066407                                    	bbci	a4, 0, 1681 <SMART_PITCH_PLC_PROCESS+0x1289>
    167a:	264d                                    	ae_l16si.n	a4, a6, 0
    167c:	654d                                    	ae_s16i.n	a4, a5, 0
    167e:	02c552                                    	addi	a5, a5, 2
    1681:	452a                                    	add.n	a4, a5, a2
    1683:	33bc                                    	beqz.n	a3, 16ba <SMART_PITCH_PLC_PROCESS+0x12c2>
    1685:	652b                                    	addi.n	a6, a5, 2
    1687:	244d                                    	ae_l16si.n	a4, a4, 0
    1689:	f62a                                    	add.n	a15, a6, a2
    168b:	654d                                    	ae_s16i.n	a4, a5, 0
    168d:	730b                                    	addi.n	a7, a3, -1
    168f:	2f4d                                    	ae_l16si.n	a4, a15, 0
    1691:	063d                                    	mov.n	a3, a6
    1693:	562b                                    	addi.n	a5, a6, 2
    1695:	1f9776                                    	loopnez	a7, 16b8 <SMART_PITCH_PLC_PROCESS+0x12c0>
    1698:	3206a5400090034e                  	{ s16i	a4, a3, 0; add	a3, a5, a2 }
    16a0:	220285400080134e                  	{ l16si	a4, a3, 0; addi	a3, a5, 2 }
    16a8:	0206a3400091054e                  	{ s16i	a4, a5, 0; add	a4, a3, a2 }
    16b0:	240283400080144e                  	{ l16si	a4, a4, 0; addi	a5, a3, 2 }

000016b8 <SMART_PITCH_PLC_PROCESS+0x12c0>:
    16b8:	634d                                    	ae_s16i.n	a4, a3, 0

000016ba <SMART_PITCH_PLC_PROCESS+0x12c2>:
    16ba:	112cf0                                    	slli	a2, a12, 1
    16bd:	1f1ca6                                    	blti	a12, 1, 16e0 <SMART_PITCH_PLC_PROCESS+0x12e8>
    16c0:	d02e20                                    	subx2	a2, a14, a2
    16c3:	e0a632                                    	movi	a3, 0x6e0
    16c6:	22da                                    	add.n	a2, a2, a13
    16c8:	0b4d                                    	mov.n	a4, a11
    16ca:	803230                                    	add	a3, a2, a3
    16cd:	0f8c76                                    	loop	a12, 16e0 <SMART_PITCH_PLC_PROCESS+0x12e8>
    16d0:	240284400080042e                  	{ l16si	a2, a4, 0; addi	a4, a4, 2 }
    16d8:	2202a3400080132e                  	{ s16i	a2, a3, 0; addi	a3, a3, 2 }
    16e0:	1b2d22                                    	l32i	a2, a13, 108
    16e3:	1c6d22                                    	s32i	a2, a13, 112
    16e6:	192d22                                    	l32i	a2, a13, 100
    16e9:	ed38                                    	l32i.n	a3, a13, 56
    16eb:	0a0c                                    	movi.n	a10, 0
    16ed:	1d6d32                                    	s32i	a3, a13, 116
    16f0:	221b                                    	addi.n	a2, a2, 1
    16f2:	196d22                                    	s32i	a2, a13, 100
    16f5:	fa5321                                    	l32r	a2, 44 (1ec8 <plc_gainctrl>)
    16f8:	0002e0                                    	callx8	a2
    16fb:	f01d                                    	retw.n

000016fd <SMART_PITCH_PLC_PROCESS+0x1305>:
    16fd:	000000                                        ...

00001700 <sqrt_q26>:
    1700:	004136                                    	entry	a1, 32
    1703:	fa5131                                    	l32r	a3, 48 (213c <HIFI_plc_sqrt>)
    1706:	02bd                                    	mov.n	a11, a2
    1708:	5a0c                                    	movi.n	a10, 5
    170a:	05a022                                    	movi	a2, 5
    170d:	0003e0                                    	callx8	a3
    1710:	c022a0                                    	sub	a2, a2, a10
    1713:	630c                                    	movi.n	a3, 6
    1715:	400200                                    	ssr	a2
    1718:	ff7c                                    	movi.n	a15, -1
    171a:	723a30                                    	salt	a3, a10, a3
    171d:	4121f0                                    	srli	a2, a15, 1
    1720:	b140b0                                    	sra	a4, a11
    1723:	932430                                    	movnez	a2, a4, a3
    1726:	f01d                                    	retw.n

00001728 <find_pitch_ini>:
    1728:	004136                                    	entry	a1, 32
    172b:	f01d                                    	retw.n

0000172d <find_pitch_ini+0x5>:
    172d:	000000                                        ...

00001730 <find_pitch>:
    1730:	00e136                                    	entry	a1, 112
    1733:	7169                                    	s32i.n	a6, a1, 28
    1735:	04cd                                    	mov.n	a12, a4
    1737:	a0e440                                    	addx4	a14, a4, a4
    173a:	136152                                    	s32i	a5, a1, 76
    173d:	a0fee0                                    	addx4	a15, a14, a14
    1740:	b139                                    	s32i.n	a3, a1, 44
    1742:	2164f0                                    	srai	a6, a15, 4
    1745:	c129                                    	s32i.n	a2, a1, 48
    1747:	07dd                                    	mov.n	a13, a7
    1749:	51cb                                    	addi.n	a5, a1, 12
    174b:	090c                                    	movi.n	a9, 0
    174d:	10c142                                    	addi	a4, a1, 16
    1750:	030c                                    	movi.n	a3, 0
    1752:	020c                                    	movi.n	a2, 0
    1754:	d169                                    	s32i.n	a6, a1, 52
    1756:	ffcda2                                    	addi	a10, a13, -1
    1759:	0b2182                                    	l32i	a8, a1, 44
    175c:	230760a00805dcce                  	{ blti.w15	a12, 1, 183a <find_pitch+0x10a>; nop }
    1764:	0c21e2                                    	l32i	a14, a1, 48
    1767:	01cc32                                    	addi	a3, a12, 1
    176a:	c07ac0                                    	sub	a7, a10, a12
    176d:	413130                                    	srli	a3, a3, 1
    1770:	c02a80                                    	sub	a2, a10, a8
    1773:	ffc392                                    	addi	a9, a3, -1
    1776:	9077e0                                    	addx2	a7, a7, a14
    1779:	9082e0                                    	addx2	a8, a2, a14
    177c:	27fd                                    	ae_l16si.n	a15, a7, 0
    177e:	00a0a2                                    	movi	a10, 0
    1781:	282d                                    	ae_l16si.n	a2, a8, 0
    1783:	0461f2                                    	s32i	a15, a1, 16
    1786:	036122                                    	s32i	a2, a1, 12
    1789:	04c882                                    	addi	a8, a8, 4
    178c:	2406f5401488705e                  	{ ae_l16m.i	aed2, a5, 0; or	a2, a5, a5 }
    1794:	205770                                    	or	a5, a7, a7
    1797:	00a062                                    	movi	a6, 0
    179a:	3406f4400488604e                  	{ ae_l16m.i	aed0, a4, 0; or	a3, a4, a4 }
    17a2:	204880                                    	or	a4, a8, a8
    17a5:	207aa0                                    	or	a7, a10, a10
    17a8:	1a83550a3442492e                  	{ loopnez	a9, 1800 <find_pitch+0xd0>; nop; ae_mul32.ll	aed1, aed2, aed2 }
    17b0:	0e94e7c0052448d0c40093030c40691f 	{ ae_trunca32q48	a9, aed1; l16si	a8, a4, 0; ae_mul32.ll	aed0, aed2, aed0; nop }
    17c0:	0502a5401881118e                  	{ s32i	a8, a1, 12; addi	a5, a5, 4 }
    17c8:	0eace56005245ad0c400813b0c01680f 	{ ae_trunca32q48	a8, aed0; l16si	a10, a5, 0; nop; nop }
    17d8:	1906a441008a11ae                  	{ s32i	a10, a1, 16; srai	a9, a9, 4 }
    17e0:	3606e9401491702e                  	{ ae_l16m.i	aed2, a2, 0; add	a7, a9, a7 }
    17e8:	0e94e7c0252344d0c400930c0c42848f 	{ srai	a8, a8, 4; addi	a4, a4, 4; ae_mul32.ll	aed1, aed2, aed2; nop }
    17f8:	2606e8400491603e                  	{ ae_l16m.i	aed0, a3, 0; add	a6, a8, a6 }

00001800 <find_pitch+0xd0>:
    1800:	0e9567c7d52655d0c40093030c406b1f 	{ ae_trunca32q48	a11, aed1; or	a10, a5, a5; ae_mul32.ll	aed0, aed2, aed0; nop }
    1810:	025d                                    	mov.n	a5, a2
    1812:	0ead6567a52633d0c400813b0c016e0f 	{ ae_trunca32q48	a14, aed0; or	a4, a3, a3; nop; nop }
    1822:	2124b0                                    	srai	a2, a11, 4
    1825:	927a                                    	add.n	a9, a2, a7
    1827:	2134e0                                    	srai	a3, a14, 4
    182a:	636a                                    	add.n	a6, a3, a6
    182c:	31ff60                                    	srai	a15, a6, 31
    182f:	413f60                                    	srli	a3, a6, 15
    1832:	017ff0                                    	slli	a7, a15, 17
    1835:	0126f0                                    	slli	a2, a6, 17
    1838:	373a                                    	add.n	a3, a7, a3

0000183a <find_pitch+0x10a>:
    183a:	d168                                    	l32i.n	a6, a1, 52
    183c:	5a0c                                    	movi.n	a10, 5
    183e:	5159                                    	s32i.n	a5, a1, 20
    1840:	095d                                    	mov.n	a5, a9
    1842:	6149                                    	s32i.n	a4, a1, 24
    1844:	570c                                    	movi.n	a7, 5
    1846:	53b960                                    	max	a11, a9, a6
    1849:	f9ff61                                    	l32r	a6, 48 (213c <HIFI_plc_sqrt>)
    184c:	a1c9                                    	s32i.n	a12, a1, 40
    184e:	1261d2                                    	s32i	a13, a1, 72
    1851:	0006e0                                    	callx8	a6
    1854:	122192                                    	l32i	a9, a1, 72
    1857:	180c                                    	movi.n	a8, 1
    1859:	a1c8                                    	l32i.n	a12, a1, 40
    185b:	0d0c                                    	movi.n	a13, 0
    185d:	c069c0                                    	sub	a6, a9, a12
    1860:	c047a0                                    	sub	a4, a7, a10
    1863:	e169                                    	s32i.n	a6, a1, 56
    1865:	6e0c                                    	movi.n	a14, 6
    1867:	ff7c                                    	movi.n	a15, -1
    1869:	7168                                    	l32i.n	a6, a1, 28
    186b:	4171f0                                    	srli	a7, a15, 1
    186e:	72eae0                                    	salt	a14, a10, a14
    1871:	230740a41025d6ce                  	{ blti.w15	a6, 2, 1a10 <find_pitch+0x2e0>; nop }
    1879:	02ad                                    	mov.n	a10, a2
    187b:	400400                                    	ssr	a4
    187e:	f9ef21                                    	l32r	a2, 3c (2260 <__divdi3>)
    1881:	b1c0b0                                    	sra	a12, a11
    1884:	83c7e0                                    	moveqz	a12, a7, a14
    1887:	03bd                                    	mov.n	a11, a3
    1889:	31dfc0                                    	srai	a13, a12, 31
    188c:	096d                                    	mov.n	a6, a9
    188e:	0002e0                                    	callx8	a2
    1891:	a1c8                                    	l32i.n	a12, a1, 40
    1893:	e128                                    	l32i.n	a2, a1, 56
    1895:	c178                                    	l32i.n	a7, a1, 48
    1897:	0a9d                                    	mov.n	a9, a10
    1899:	b188                                    	l32i.n	a8, a1, 44
    189b:	2b0c                                    	movi.n	a11, 2
    189d:	e6ca                                    	add.n	a14, a6, a12
    189f:	0d0c                                    	movi.n	a13, 0
    18a1:	902270                                    	addx2	a2, a2, a7
    18a4:	c03680                                    	sub	a3, a6, a8
    18a7:	fec222                                    	addi	a2, a2, -2
    18aa:	c0fe80                                    	sub	a15, a14, a8
    18ad:	f129                                    	s32i.n	a2, a1, 60
    18af:	903370                                    	addx2	a3, a3, a7
    18b2:	902f70                                    	addx2	a2, a15, a7
    18b5:	e32b                                    	addi.n	a14, a3, 2
    18b7:	fec232                                    	addi	a3, a2, -2
    18ba:	1161b2                                    	s32i	a11, a1, 68
    18bd:	232d                                    	ae_l16si.n	a2, a3, 0
    18bf:	51b8                                    	l32i.n	a11, a1, 20
    18c1:	8199                                    	s32i.n	a9, a1, 32
    18c3:	6148                                    	l32i.n	a4, a1, 24
    18c5:	fcce92                                    	addi	a9, a14, -4
    18c8:	29ad                                    	ae_l16si.n	a10, a9, 0
    18ca:	3129                                    	s32i.n	a2, a1, 12
    18cc:	01043b41008001ae                  	{ s32i	a10, a1, 16; ae_l16m.i	aed0, a11, 0 }
    18d4:	1a8235042000104e                  	{ ae_l16m.i	aed1, a4, 0; nop; ae_mul32.ll	aed0, aed0, aed0 }
    18dc:	91d9                                    	s32i.n	a13, a1, 36
    18de:	1a8215f82421013e                  	{ s32i	a3, a1, 64; nop; ae_mul32.ll	aed31, aed1, aed1 }
    18e6:	060c                                    	movi.n	a6, 0
    18e8:	0eace5687d2418d0c400813b0c016d0f 	{ ae_trunca32q48	a13, aed0; l32i	a8, a1, 60; nop; nop }
    18f8:	0eace5600d23c7d0c400813b0c016fff 	{ ae_trunca32q48	a15, aed31; addi	a7, a12, 1; nop; nop }
    1908:	2124d0                                    	srai	a2, a13, 4
    190b:	419170                                    	srli	a9, a7, 1
    190e:	802250                                    	add	a2, a2, a5
    1911:	2134f0                                    	srai	a3, a15, 4
    1914:	c0a230                                    	sub	a10, a2, a3
    1917:	ffc992                                    	addi	a9, a9, -1
    191a:	04c852                                    	addi	a5, a8, 4
    191d:	7a1ca6                                    	blti	a12, 1, 199b <find_pitch+0x26b>
    1920:	2efd                                    	ae_l16si.n	a15, a14, 0
    1922:	00a062                                    	movi	a6, 0
    1925:	283d                                    	ae_l16si.n	a3, a8, 0
    1927:	0461f2                                    	s32i	a15, a1, 16
    192a:	010434401880013e                  	{ s32i	a3, a1, 12; ae_l16m.i	aed0, a4, 0 }
    1932:	3406f4400c8860be                  	{ ae_l16m.i	aed1, a11, 0; or	a3, a4, a4 }
    193a:	1a8335002c202bbe                  	{ or	a2, a11, a11; nop; ae_mul32.ll	aed0, aed1, aed0 }
    1942:	204ee0                                    	or	a4, a14, a14
    1945:	399976                                    	loopnez	a9, 1982 <find_pitch+0x252>
    1948:	040284400081057e                  	{ l16si	a7, a5, 0; addi	a4, a4, 4 }
    1950:	000003f3038300fbd6a303003c60117f 	{ s32i	a7, a1, 12; ae_trunca32q48	a7, aed0; nop }
    1960:	248d                                    	ae_l16si.n	a8, a4, 0
    1962:	010432410081018e                  	{ s32i	a8, a1, 16; ae_l16m.i	aed1, a2, 0 }
    196a:	3606e4400489703e                  	{ ae_l16m.i	aed0, a3, 0; srai	a7, a7, 4 }
    1972:	0e94e7c0252355d0c50093070820676f 	{ add	a6, a7, a6; addi	a5, a5, 4; ae_mul32.ll	aed0, aed1, aed0; nop }

00001982 <find_pitch+0x252>:
    1982:	0ead6567dd2622d0c400813b0c016f0f 	{ ae_trunca32q48	a15, aed0; or	a11, a2, a2; nop; nop }
    1992:	204330                                    	or	a4, a3, a3
    1995:	2124f0                                    	srai	a2, a15, 4
    1998:	806260                                    	add	a6, a2, a6

0000199b <find_pitch+0x26b>:
    199b:	d128                                    	l32i.n	a2, a1, 52
    199d:	51b9                                    	s32i.n	a11, a1, 20
    199f:	6149                                    	s32i.n	a4, a1, 24
    19a1:	0a5d                                    	mov.n	a5, a10
    19a3:	0e4d                                    	mov.n	a4, a14
    19a5:	313f60                                    	srai	a3, a6, 31
    19a8:	53ba20                                    	max	a11, a10, a2
    19ab:	f9a721                                    	l32r	a2, 48 (213c <HIFI_plc_sqrt>)
    19ae:	5a0c                                    	movi.n	a10, 5
    19b0:	417f60                                    	srli	a7, a6, 15
    19b3:	0002e0                                    	callx8	a2
    19b6:	520c                                    	movi.n	a2, 5
    19b8:	f87c                                    	movi.n	a8, -1
    19ba:	c022a0                                    	sub	a2, a2, a10
    19bd:	6f0c                                    	movi.n	a15, 6
    19bf:	01e6f0                                    	slli	a14, a6, 17
    19c2:	400200                                    	ssr	a2
    19c5:	726af0                                    	salt	a6, a10, a15
    19c8:	b1c0b0                                    	sra	a12, a11
    19cb:	418180                                    	srli	a8, a8, 1
    19ce:	0133f0                                    	slli	a3, a3, 17
    19d1:	f99a21                                    	l32r	a2, 3c (2260 <__divdi3>)
    19d4:	83c860                                    	moveqz	a12, a8, a6
    19d7:	b37a                                    	add.n	a11, a3, a7
    19d9:	31dfc0                                    	srai	a13, a12, 31
    19dc:	0ead                                    	mov.n	a10, a14
    19de:	0002e0                                    	callx8	a2
    19e1:	8198                                    	l32i.n	a9, a1, 32
    19e3:	e44b                                    	addi.n	a14, a4, 4
    19e5:	91d8                                    	l32i.n	a13, a1, 36
    19e7:	102132                                    	l32i	a3, a1, 64
    19ea:	1121b2                                    	l32i	a11, a1, 68
    19ed:	a1c8                                    	l32i.n	a12, a1, 40
    19ef:	71f8                                    	l32i.n	a15, a1, 28
    19f1:	722a90                                    	salt	a2, a10, a9
    19f4:	5399a0                                    	max	a9, a9, a10
    19f7:	334b                                    	addi.n	a3, a3, 4
    19f9:	83db20                                    	moveqz	a13, a11, a2
    19fc:	bb2b                                    	addi.n	a11, a11, 2
    19fe:	2207409a0fc5dfbe                  	{ bge.w15	a15, a11, 18ba <find_pitch+0x18a>; nop }
    1a06:	122192                                    	l32i	a9, a1, 72
    1a09:	120c                                    	movi.n	a2, 1
    1a0b:	7168                                    	l32i.n	a6, a1, 28
    1a0d:	538d20                                    	max	a8, a13, a2
    1a10:	3d1b                                    	addi.n	a3, a13, 1
    1a12:	722d60                                    	salt	a2, a13, a6
    1a15:	833620                                    	moveqz	a3, a6, a2
    1a18:	489a                                    	add.n	a4, a8, a9
    1a1a:	780b                                    	addi.n	a7, a8, -1
    1a1c:	9189                                    	s32i.n	a8, a1, 36
    1a1e:	7139                                    	s32i.n	a3, a1, 28
    1a20:	020c                                    	movi.n	a2, 0
    1a22:	8149                                    	s32i.n	a4, a1, 32
    1a24:	030c                                    	movi.n	a3, 0
    1a26:	5158                                    	l32i.n	a5, a1, 20
    1a28:	230740a00c25dcce                  	{ blti.w15	a12, 1, 1be6 <find_pitch+0x4b6>; nop }
    1a30:	b1e8                                    	l32i.n	a14, a1, 44
    1a32:	5158                                    	l32i.n	a5, a1, 20
    1a34:	c188                                    	l32i.n	a8, a1, 48
    1a36:	07dd                                    	mov.n	a13, a7
    1a38:	e128                                    	l32i.n	a2, a1, 56
    1a3a:	060c                                    	movi.n	a6, 0
    1a3c:	c0f4e0                                    	sub	a15, a4, a14
    1a3f:	040c                                    	movi.n	a4, 0
    1a41:	61b8                                    	l32i.n	a11, a1, 24
    1a43:	903280                                    	addx2	a3, a2, a8
    1a46:	907f80                                    	addx2	a7, a15, a8
    1a49:	4121c0                                    	srli	a2, a12, 1
    1a4c:	fec382                                    	addi	a8, a3, -2
    1a4f:	fcc792                                    	addi	a9, a7, -4
    1a52:	476c07                                    	bbci	a12, 0, 1a9d <find_pitch+0x36d>
    1a55:	297d                                    	ae_l16si.n	a7, a9, 0
    1a57:	283d                                    	ae_l16si.n	a3, a8, 0
    1a59:	3179                                    	s32i.n	a7, a1, 12
    1a5b:	010435410082013e                  	{ s32i	a3, a1, 16; ae_l16m.i	aed2, a5, 0 }
    1a63:	1a82350c204200be                  	{ ae_l16m.i	aed0, a11, 0; nop; ae_mul32.ll	aed1, aed2, aed2 }
    1a6b:	1a81b5042040288e                  	{ addi	a8, a8, 2; nop; ae_mul32.ll	aed0, aed2, aed0 }
    1a73:	0eace560152299d0c400813b0c016c1f 	{ ae_trunca32q48	a12, aed1; addi	a9, a9, 2; nop; nop }
    1a83:	0ead257b0d240cd0c400813b0c016a0f 	{ ae_trunca32q48	a10, aed0; nop; nop; nop }
    1a93:	21e4c0                                    	srai	a14, a12, 4
    1a96:	21f4a0                                    	srai	a15, a10, 4
    1a99:	4e4a                                    	add.n	a4, a14, a4
    1a9b:	6f6a                                    	add.n	a6, a15, a6
    1a9d:	61e8                                    	l32i.n	a14, a1, 24
    1a9f:	130216                                    	beqz	a2, 1bd3 <find_pitch+0x4a3>
    1aa2:	293d                                    	ae_l16si.n	a3, a9, 0
    1aa4:	28ad                                    	ae_l16si.n	a10, a8, 0
    1aa6:	3139                                    	s32i.n	a3, a1, 12
    1aa8:	01043541008001ae                  	{ s32i	a10, a1, 16; ae_l16m.i	aed0, a5, 0 }
    1ab0:	1a82350c200020ee                  	{ ae_l16m.i	aed2, a14, 0; nop; ae_mul32.ll	aed1, aed0, aed0 }
    1ab8:	397d                                    	ae_l16si.n	a7, a9, 2
    1aba:	1a81d506200218be                  	{ l16si	a11, a8, 2; nop; ae_mul32.ll	aed0, aed0, aed2 }
    1ac2:	3179                                    	s32i.n	a7, a1, 12
    1ac4:	0eace570ed2250d0c400813b0c016c1f 	{ ae_trunca32q48	a12, aed1; ae_l16m.i	aed29, a5, 0; nop; nop }
    1ad4:	000003b3038300fd56bb00f1247d11bf 	{ s32i	a11, a1, 16; ae_trunca32q48	a10, aed0; ae_mul32.ll	aed30, aed29, aed29 }
    1ae4:	0802e9471c8170ee                  	{ ae_l16m.i	aed31, a14, 0; addi	a9, a9, 4 }
    1aec:	1a8315022bbff4ce                  	{ srai	a15, a12, 4; nop; ae_mul32.ll	aed0, aed29, aed31 }
    1af4:	c20b                                    	addi.n	a12, a2, -1
    1af6:	0ead65665d23f4d0c400813b0c0164ef 	{ ae_trunca32q48	a4, aed30; add	a11, a15, a4; nop; nop }
    1b06:	0ead65667d204ad0c400813b0c016a0f 	{ ae_trunca32q48	a10, aed0; srai	a15, a10, 4; nop; nop }
    1b16:	6f6a                                    	add.n	a6, a15, a6
    1b18:	884b                                    	addi.n	a8, a8, 4
    1b1a:	217440                                    	srai	a7, a4, 4
    1b1d:	052d                                    	mov.n	a2, a5
    1b1f:	094d                                    	mov.n	a4, a9
    1b21:	209660                                    	or	a9, a6, a6
    1b24:	203ee0                                    	or	a3, a14, a14
    1b27:	205880                                    	or	a5, a8, a8
    1b2a:	206bb0                                    	or	a6, a11, a11
    1b2d:	979c76                                    	loopnez	a12, 1bc8 <find_pitch+0x498>
    1b30:	060687400092046e                  	{ l16si	a6, a4, 0; add	a8, a7, a6 }
    1b38:	2a068440008a057e                  	{ l16si	a7, a5, 0; srai	a10, a10, 4 }
    1b40:	18068a40089215ae                  	{ l16si	a10, a5, 2; add	a9, a10, a9 }
    1b48:	170324401880016e                  	{ s32i	a6, a1, 12; l16si	a6, a4, 2 }
    1b50:	010432410080017e                  	{ s32i	a7, a1, 16; ae_l16m.i	aed0, a2, 0 }
    1b58:	0e94c7d011e230d0c500930d1c00016f 	{ s32i	a6, a1, 12; ae_l16m.i	aed2, a3, 0; ae_mul32.ll	aed1, aed0, aed0; nop }
    1b68:	0e94c7d019e220d0c5009305040211af 	{ s32i	a10, a1, 16; ae_l16m.i	aed3, a2, 0; ae_mul32.ll	aed0, aed0, aed2; nop }
    1b78:	0e94e7d00d2230d0c40093130c63671f 	{ ae_trunca32q48	a7, aed1; ae_l16m.i	aed1, a3, 0; ae_mul32.ll	aed2, aed3, aed3; nop }
    1b88:	0e94e7c0252255d0c40093030c61660f 	{ ae_trunca32q48	a6, aed0; addi	a5, a5, 4; ae_mul32.ll	aed0, aed3, aed1; nop }
    1b98:	0eace560252244d0c400813b0c016b2f 	{ ae_trunca32q48	a11, aed2; addi	a4, a4, 4; nop; nop }
    1ba8:	0ead65663d2047d0c400813b0c016a0f 	{ ae_trunca32q48	a10, aed0; srai	a7, a7, 4; nop; nop }
    1bb8:	3606844114e9278e                  	{ add	a6, a7, a8; srai	a7, a6, 4 }
    1bc0:	3a0684420ce9379e                  	{ add	a9, a7, a9; srai	a7, a11, 4 }

00001bc8 <find_pitch+0x498>:
    1bc8:	21f4a0                                    	srai	a15, a10, 4
    1bcb:	6139                                    	s32i.n	a3, a1, 24
    1bcd:	476a                                    	add.n	a4, a7, a6
    1bcf:	025d                                    	mov.n	a5, a2
    1bd1:	6f9a                                    	add.n	a6, a15, a9

00001bd3 <find_pitch+0x4a3>:
    1bd3:	312f60                                    	srai	a2, a6, 31
    1bd6:	413f60                                    	srli	a3, a6, 15
    1bd9:	0172f0                                    	slli	a7, a2, 17
    1bdc:	0126f0                                    	slli	a2, a6, 17
    1bdf:	373a                                    	add.n	a3, a7, a3
    1be1:	0d7d                                    	mov.n	a7, a13
    1be3:	000046                                    	j	1be8 <find_pitch+0x4b8>

00001be6 <find_pitch+0x4b6>:
    1be6:	040c                                    	movi.n	a4, 0
    1be8:	d168                                    	l32i.n	a6, a1, 52
    1bea:	5a0c                                    	movi.n	a10, 5
    1bec:	53b460                                    	max	a11, a4, a6
    1bef:	f91661                                    	l32r	a6, 48 (213c <HIFI_plc_sqrt>)
    1bf2:	0006e0                                    	callx8	a6
    1bf5:	7168                                    	l32i.n	a6, a1, 28
    1bf7:	590c                                    	movi.n	a9, 5
    1bf9:	6c0c                                    	movi.n	a12, 6
    1bfb:	c089a0                                    	sub	a8, a9, a10
    1bfe:	72eac0                                    	salt	a14, a10, a12
    1c01:	fd7c                                    	movi.n	a13, -1
    1c03:	02ad                                    	mov.n	a10, a2
    1c05:	230700820c45576e                  	{ bge.w15	a7, a6, 1e33 <find_pitch+0x703>; nop }
    1c0d:	400800                                    	ssr	a8
    1c10:	5179                                    	s32i.n	a7, a1, 20
    1c12:	b1c0b0                                    	sra	a12, a11
    1c15:	4171d0                                    	srli	a7, a13, 1
    1c18:	f90921                                    	l32r	a2, 3c (2260 <__divdi3>)
    1c1b:	83c7e0                                    	moveqz	a12, a7, a14
    1c1e:	31dfc0                                    	srai	a13, a12, 31
    1c21:	03bd                                    	mov.n	a11, a3
    1c23:	0002e0                                    	callx8	a2
    1c26:	c178                                    	l32i.n	a7, a1, 48
    1c28:	8198                                    	l32i.n	a9, a1, 32
    1c2a:	e128                                    	l32i.n	a2, a1, 56
    1c2c:	a168                                    	l32i.n	a6, a1, 40
    1c2e:	b188                                    	l32i.n	a8, a1, 44
    1c30:	0abd                                    	mov.n	a11, a10
    1c32:	902270                                    	addx2	a2, a2, a7
    1c35:	696a                                    	add.n	a6, a9, a6
    1c37:	fec222                                    	addi	a2, a2, -2
    1c3a:	c03980                                    	sub	a3, a9, a8
    1c3d:	c0f680                                    	sub	a15, a6, a8
    1c40:	f129                                    	s32i.n	a2, a1, 60
    1c42:	903370                                    	addx2	a3, a3, a7
    1c45:	902f70                                    	addx2	a2, a15, a7
    1c48:	5178                                    	l32i.n	a7, a1, 20
    1c4a:	9168                                    	l32i.n	a6, a1, 36
    1c4c:	fec382                                    	addi	a8, a3, -2
    1c4f:	fcc292                                    	addi	a9, a2, -4
    1c52:	fec832                                    	addi	a3, a8, -2
    1c55:	292d                                    	ae_l16si.n	a2, a9, 0
    1c57:	23dd                                    	ae_l16si.n	a13, a3, 0
    1c59:	3129                                    	s32i.n	a2, a1, 12
    1c5b:	01043540188001de                  	{ s32i	a13, a1, 12; ae_l16m.i	aed0, a5, 0 }
    1c63:	1a8235042000105e                  	{ ae_l16m.i	aed1, a5, 0; nop; ae_mul32.ll	aed0, aed0, aed0 }
    1c6b:	5179                                    	s32i.n	a7, a1, 20
    1c6d:	1a8215f8202191be                  	{ s32i	a11, a1, 36; nop; ae_mul32.ll	aed31, aed1, aed1 }
    1c75:	070c                                    	movi.n	a7, 0
    1c77:	000003f3038300ff56a303022460118f 	{ s32i	a8, a1, 32; ae_trunca32q48	a14, aed0; nop }
    1c87:	000003f37b8300fff6a303042c60119f 	{ s32i	a9, a1, 68; ae_trunca32q48	a15, aed31; nop }
    1c97:	106162                                    	s32i	a6, a1, 64
    1c9a:	086d                                    	mov.n	a6, a8
    1c9c:	61a8                                    	l32i.n	a10, a1, 24
    1c9e:	2124e0                                    	srai	a2, a14, 4
    1ca1:	224a                                    	add.n	a2, a2, a4
    1ca3:	2134f0                                    	srai	a3, a15, 4
    1ca6:	c0c230                                    	sub	a12, a2, a3
    1ca9:	a138                                    	l32i.n	a3, a1, 40
    1cab:	0a4d                                    	mov.n	a4, a10
    1cad:	f198                                    	l32i.n	a9, a1, 60
    1caf:	61d8                                    	l32i.n	a13, a1, 24
    1cb1:	412130                                    	srli	a2, a3, 1
    1cb4:	220720a00c25d3ce                  	{ blti.w15	a3, 1, 1e30 <find_pitch+0x700>; nop }
    1cbc:	0d4d                                    	mov.n	a4, a13
    1cbe:	326307                                    	bbci	a3, 0, 1cf4 <find_pitch+0x5c4>
    1cc1:	293d                                    	ae_l16si.n	a3, a9, 0
    1cc3:	268d                                    	ae_l16si.n	a8, a6, 0
    1cc5:	3139                                    	s32i.n	a3, a1, 12
    1cc7:	010435410080018e                  	{ s32i	a8, a1, 16; ae_l16m.i	aed0, a5, 0 }
    1ccf:	2602e6400c80604e                  	{ ae_l16m.i	aed1, a4, 0; addi	a6, a6, 2 }
    1cd7:	1a81b5042001299e                  	{ addi	a9, a9, 2; nop; ae_mul32.ll	aed0, aed0, aed1 }
    1cdf:	0ead257b0d240cd0c400813b0c016e0f 	{ ae_trunca32q48	a14, aed0; nop; nop; nop }
    1cef:	21f4e0                                    	srai	a15, a14, 4
    1cf2:	7f7a                                    	add.n	a7, a15, a7
    1cf4:	e64b                                    	addi.n	a14, a6, 4
    1cf6:	0cc216                                    	beqz	a2, 1dc6 <find_pitch+0x696>
    1cf9:	293d                                    	ae_l16si.n	a3, a9, 0
    1cfb:	f94b                                    	addi.n	a15, a9, 4
    1cfd:	268d                                    	ae_l16si.n	a8, a6, 0
    1cff:	3139                                    	s32i.n	a3, a1, 12
    1d01:	39bd                                    	ae_l16si.n	a11, a9, 2
    1d03:	4189                                    	s32i.n	a8, a1, 16
    1d05:	1c0366401480705e                  	{ ae_l16m.i	aed2, a5, 0; l16si	a13, a6, 2 }
    1d0d:	01043440188301be                  	{ s32i	a11, a1, 12; ae_l16m.i	aed3, a4, 0 }
    1d15:	1a8235142043005e                  	{ ae_l16m.i	aed0, a5, 0; nop; ae_mul32.ll	aed2, aed2, aed3 }
    1d1d:	41d9                                    	s32i.n	a13, a1, 16
    1d1f:	3a02e2c00c9f704e                  	{ ae_l16m.i	aed1, a4, 0; addi	a11, a2, -1 }
    1d27:	0e9567c7952655d0c40093030c016a2f 	{ ae_trunca32q48	a10, aed2; or	a2, a5, a5; ae_mul32.ll	aed0, aed0, aed1; nop }
    1d37:	043d                                    	mov.n	a3, a4
    1d39:	0e4d                                    	mov.n	a4, a14
    1d3b:	0ead6567ad26ffd0c400813b0c01680f 	{ ae_trunca32q48	a8, aed0; or	a5, a15, a15; nop; nop }
    1d4b:	0a6d                                    	mov.n	a6, a10
    1d4d:	679b76                                    	loopnez	a11, 1db8 <find_pitch+0x688>
    1d50:	260684400089059e                  	{ l16si	a9, a5, 0; srai	a6, a6, 4 }
    1d58:	090324401880119e                  	{ s32i	a9, a1, 12; l16si	a9, a4, 0 }
    1d60:	1b0325410080019e                  	{ s32i	a9, a1, 16; l16si	a10, a5, 2 }
    1d68:	180364400480702e                  	{ ae_l16m.i	aed0, a2, 0; l16si	a9, a4, 2 }
    1d70:	01043340188101ae                  	{ s32i	a10, a1, 12; ae_l16m.i	aed1, a3, 0 }
    1d78:	0e94c7d011e220d0c50093050401119f 	{ s32i	a9, a1, 16; ae_l16m.i	aed2, a2, 0; ae_mul32.ll	aed0, aed0, aed1; nop }
    1d88:	0402e4400c81603e                  	{ ae_l16m.i	aed1, a3, 0; addi	a4, a4, 4 }
    1d90:	0e9567c63d2267d0c40093030c41660f 	{ ae_trunca32q48	a6, aed0; add	a7, a6, a7; ae_mul32.ll	aed0, aed2, aed1; nop }
    1da0:	0502a54200a1348e                  	{ srai	a8, a8, 4; addi	a5, a5, 4 }
    1da8:	0ead65663d2287d0c400813b0c01680f 	{ ae_trunca32q48	a8, aed0; add	a7, a8, a7; nop; nop }

00001db8 <find_pitch+0x688>:
    1db8:	21f480                                    	srai	a15, a8, 4
    1dbb:	21e460                                    	srai	a14, a6, 4
    1dbe:	025d                                    	mov.n	a5, a2
    1dc0:	2e7a                                    	add.n	a2, a14, a7
    1dc2:	7f2a                                    	add.n	a7, a15, a2
    1dc4:	034d                                    	mov.n	a4, a3

00001dc6 <find_pitch+0x696>:
    1dc6:	d128                                    	l32i.n	a2, a1, 52
    1dc8:	5a0c                                    	movi.n	a10, 5
    1dca:	6149                                    	s32i.n	a4, a1, 24
    1dcc:	0c4d                                    	mov.n	a4, a12
    1dce:	416f70                                    	srli	a6, a7, 15
    1dd1:	53bc20                                    	max	a11, a12, a2
    1dd4:	f89d21                                    	l32r	a2, 48 (213c <HIFI_plc_sqrt>)
    1dd7:	0002e0                                    	callx8	a2
    1dda:	520c                                    	movi.n	a2, 5
    1ddc:	f87c                                    	movi.n	a8, -1
    1dde:	c022a0                                    	sub	a2, a2, a10
    1de1:	6f0c                                    	movi.n	a15, 6
    1de3:	400200                                    	ssr	a2
    1de6:	313f70                                    	srai	a3, a7, 31
    1de9:	01e7f0                                    	slli	a14, a7, 17
    1dec:	727af0                                    	salt	a7, a10, a15
    1def:	b1c0b0                                    	sra	a12, a11
    1df2:	418180                                    	srli	a8, a8, 1
    1df5:	0133f0                                    	slli	a3, a3, 17
    1df8:	83c870                                    	moveqz	a12, a8, a7
    1dfb:	f89021                                    	l32r	a2, 3c (2260 <__divdi3>)
    1dfe:	b36a                                    	add.n	a11, a3, a6
    1e00:	31dfc0                                    	srai	a13, a12, 31
    1e03:	0ead                                    	mov.n	a10, a14
    1e05:	0002e0                                    	callx8	a2
    1e08:	91b8                                    	l32i.n	a11, a1, 36
    1e0a:	102132                                    	l32i	a3, a1, 64
    1e0d:	8188                                    	l32i.n	a8, a1, 32
    1e0f:	112192                                    	l32i	a9, a1, 68
    1e12:	5178                                    	l32i.n	a7, a1, 20
    1e14:	71f8                                    	l32i.n	a15, a1, 28
    1e16:	722ab0                                    	salt	a2, a10, a11
    1e19:	631b                                    	addi.n	a6, a3, 1
    1e1b:	53bba0                                    	max	a11, a11, a10
    1e1e:	882b                                    	addi.n	a8, a8, 2
    1e20:	992b                                    	addi.n	a9, a9, 2
    1e22:	837320                                    	moveqz	a7, a3, a2
    1e25:	2207009c1fc553fe                  	{ blt.w15	a3, a15, 1c52 <find_pitch+0x522>; nop }
    1e2d:	000086                                    	j	1e33 <find_pitch+0x703>

00001e30 <find_pitch+0x700>:
    1e30:	ffe486                                    	j	1dc6 <find_pitch+0x696>

00001e33 <find_pitch+0x703>:
    1e33:	132122                                    	l32i	a2, a1, 76
    1e36:	c02270                                    	sub	a2, a2, a7
    1e39:	f01d                                    	retw.n

00001e3b <find_pitch+0x70b>:
    1e3b:	00000000                                 .....

00001e40 <Q_ubitlength>:
    1e40:	004136                                    	entry	a1, 32
    1e43:	f37c                                    	movi.n	a3, -1
    1e45:	f47c                                    	movi.n	a4, -1
    1e47:	f43030                                    	extui	a3, a3, 0, 16
    1e4a:	114400                                    	slli	a4, a4, 16
    1e4d:	323a                                    	add.n	a3, a2, a3
    1e4f:	b33220                                    	movgez	a3, a2, a2
    1e52:	103340                                    	and	a3, a3, a4
    1e55:	c02230                                    	sub	a2, a2, a3
    1e58:	f01d                                    	retw.n

00001e5a <Q_ubitlength+0x1a>:
	...

00001e5c <plc_gainctrl_ini>:
    1e5c:	004136                                    	entry	a1, 32
    1e5f:	3222e2                                    	l32i	a14, a2, 200
    1e62:	031c                                    	movi.n	a3, 16
    1e64:	332282                                    	l32i	a8, a2, 204
    1e67:	f47c                                    	movi.n	a4, -1
    1e69:	f87851                                    	l32r	a5, 4c (8000ffff <_end+0x8000d66b>)
    1e6c:	f67c                                    	movi.n	a6, -1
    1e6e:	401e00                                    	ssl	a14
    1e71:	236232                                    	s32i	a3, a2, 140
    1e74:	a13400                                    	sll	a3, a4
    1e77:	401800                                    	ssl	a8
    1e7a:	a14400                                    	sll	a4, a4
    1e7d:	f46060                                    	extui	a6, a6, 0, 16
    1e80:	303350                                    	xor	a3, a3, a5
    1e83:	304450                                    	xor	a4, a4, a5
    1e86:	f77c                                    	movi.n	a7, -1
    1e88:	d36a                                    	add.n	a13, a3, a6
    1e8a:	546a                                    	add.n	a5, a4, a6
    1e8c:	117700                                    	slli	a7, a7, 16
    1e8f:	b3d330                                    	movgez	a13, a3, a3
    1e92:	b35440                                    	movgez	a5, a4, a4
    1e95:	3522f2                                    	l32i	a15, a2, 212
    1e98:	10ed70                                    	and	a14, a13, a7
    1e9b:	342292                                    	l32i	a9, a2, 208
    1e9e:	105570                                    	and	a5, a5, a7
    1ea1:	2762f2                                    	s32i	a15, a2, 156
    1ea4:	3622a2                                    	l32i	a10, a2, 216
    1ea7:	3722b2                                    	l32i	a11, a2, 220
    1eaa:	c033e0                                    	sub	a3, a3, a14
    1ead:	3822c2                                    	l32i	a12, a2, 224
    1eb0:	c0f450                                    	sub	a15, a4, a5
    1eb3:	266292                                    	s32i	a9, a2, 152
    1eb6:	2862a2                                    	s32i	a10, a2, 160
    1eb9:	2962b2                                    	s32i	a11, a2, 164
    1ebc:	2a62c2                                    	s32i	a12, a2, 168
    1ebf:	246232                                    	s32i	a3, a2, 144
    1ec2:	2562f2                                    	s32i	a15, a2, 148
    1ec5:	f01d                                    	retw.n

00001ec7 <plc_gainctrl_ini+0x6b>:
	...

00001ec8 <plc_gainctrl>:
    1ec8:	004136                                    	entry	a1, 32
    1ecb:	d578                                    	l32i.n	a7, a5, 52
    1ecd:	160c                                    	movi.n	a6, 1
    1ecf:	116600                                    	slli	a6, a6, 16
    1ed2:	fe7c                                    	movi.n	a14, -1
    1ed4:	2525f2                                    	l32i	a15, a5, 148
    1ed7:	d02720                                    	subx2	a2, a7, a2
    1eda:	117e00                                    	slli	a7, a14, 16
    1edd:	626a                                    	add.n	a6, a2, a6
    1edf:	221b                                    	addi.n	a2, a2, 1
    1ee1:	b36220                                    	movgez	a6, a2, a2
    1ee4:	2325b2                                    	l32i	a11, a5, 140
    1ee7:	106670                                    	and	a6, a6, a7
    1eea:	242592                                    	l32i	a9, a5, 144
    1eed:	c08260                                    	sub	a8, a2, a6
    1ef0:	020c                                    	movi.n	a2, 0
    1ef2:	060c                                    	movi.n	a6, 0
    1ef4:	d589                                    	s32i.n	a8, a5, 52
    1ef6:	fa7c                                    	movi.n	a10, -1
    1ef8:	230760831c051f8e                  	{ bnone.w15	a15, a8, 1fde <plc_gainctrl+0x116>; nop }
    1f00:	f4a0a0                                    	extui	a10, a10, 0, 16
    1f03:	230760a00805fbce                  	{ blti.w15	a11, 1, 1fe5 <plc_gainctrl+0x11d>; nop }
    1f0b:	00a062                                    	movi	a6, 0
    1f0e:	00a022                                    	movi	a2, 0
    1f11:	258b76                                    	loop	a11, 1f3a <plc_gainctrl+0x72>
    1f14:	10b980                                    	and	a11, a9, a8
    1f17:	c98c                                    	beqz.n	a9, 1f27 <plc_gainctrl+0x5f>
    1f19:	04b0b0                                    	extui	a11, a11, 0, 1
    1f1c:	0ac897                                    	bnall	a8, a9, 1f2a <plc_gainctrl+0x62>
    1f1f:	fdc222                                    	addi	a2, a2, -3
    1f22:	061c                                    	movi.n	a6, 16
    1f24:	000106                                    	j	1f2c <plc_gainctrl+0x64>

00001f27 <plc_gainctrl+0x5f>:
    1f27:	04b080                                    	extui	a11, a8, 0, 1
    1f2a:	6b6a                                    	add.n	a6, a11, a6
    1f2c:	218180                                    	srai	a8, a8, 1
    1f2f:	b8aa                                    	add.n	a11, a8, a10
    1f31:	b3b880                                    	movgez	a11, a8, a8
    1f34:	10bb70                                    	and	a11, a11, a7
    1f37:	c088b0                                    	sub	a8, a8, a11

00001f3a <plc_gainctrl+0x72>:
    1f3a:	262572                                    	l32i	a7, a5, 152
    1f3d:	0c0c                                    	movi.n	a12, 0
    1f3f:	272592                                    	l32i	a9, a5, 156
    1f42:	2825b2                                    	l32i	a11, a5, 160
    1f45:	727670                                    	salt	a7, a6, a7
    1f48:	118710                                    	slli	a8, a7, 15
    1f4b:	72a6b0                                    	salt	a10, a6, a11
    1f4e:	0ba967                                    	bge	a9, a6, 1f5d <plc_gainctrl+0x95>
    1f51:	83cba0                                    	moveqz	a12, a11, a10
    1f54:	c066c0                                    	sub	a6, a6, a12
    1f57:	400600                                    	ssr	a6
    1f5a:	918080                                    	srl	a8, a8

00001f5d <plc_gainctrl+0x95>:
    1f5d:	170c                                    	movi.n	a7, 1
    1f5f:	7914a6                                    	blti	a4, 1, 1fdc <plc_gainctrl+0x114>
    1f62:	102592                                    	l32i	a9, a5, 64
    1f65:	a4a0a2                                    	movi	a10, 164
    1f68:	2d0c                                    	movi.n	a13, 2
    1f6a:	f40b                                    	addi.n	a15, a4, -1
    1f6c:	72d6d0                                    	salt	a13, a6, a13
    1f6f:	fdc262                                    	addi	a6, a2, -3
    1f72:	c0c890                                    	sub	a12, a8, a9
    1f75:	8362d0                                    	moveqz	a6, a2, a13
    1f78:	72bc70                                    	salt	a11, a12, a7
    1f7b:	0d0c                                    	movi.n	a13, 0
    1f7d:	a0bba0                                    	addx4	a11, a11, a10
    1f80:	074d                                    	mov.n	a4, a7
    1f82:	b5ba                                    	add.n	a11, a5, a11
    1f84:	0d7d                                    	mov.n	a7, a13
    1f86:	0be8                                    	l32i.n	a14, a11, 0
    1f88:	2e6a                                    	add.n	a2, a14, a6
    1f8a:	5322d0                                    	max	a2, a2, a13
    1f8d:	400200                                    	ssr	a2
    1f90:	b120c0                                    	sra	a2, a12
    1f93:	929a                                    	add.n	a9, a2, a9
    1f95:	062d                                    	mov.n	a2, a6
    1f97:	0a6d                                    	mov.n	a6, a10
    1f99:	329f76                                    	loopnez	a15, 1fcf <plc_gainctrl+0x107>
    1f9c:	c0c890                                    	sub	a12, a8, a9
    1f9f:	72ac40                                    	salt	a10, a12, a4
    1fa2:	a0aa60                                    	addx4	a10, a10, a6
    1fa5:	0a0303421ce025ae                  	{ add	a11, a5, a10; l16si	a10, a3, 0 }
    1fad:	0bb8                                    	l32i.n	a11, a11, 0
    1faf:	82a9a0                                    	mull	a10, a9, a10
    1fb2:	bb2a                                    	add.n	a11, a11, a2
    1fb4:	3b06bfc210b66b7e                  	{ max	a10, a11, a7; srli	a11, a10, 15 }
    1fbc:	32072ac0008413be                  	{ s16i	a11, a3, 0; ssr	a10 }
    1fc4:	b1a0c0                                    	sra	a10, a12
    1fc7:	220283420ce03a9e                  	{ add	a9, a10, a9; addi	a3, a3, 2 }

00001fcf <plc_gainctrl+0x107>:
    1fcf:	23ed                                    	ae_l16si.n	a14, a3, 0
    1fd1:	106592                                    	s32i	a9, a5, 64
    1fd4:	8229e0                                    	mull	a2, a9, a14
    1fd7:	412f20                                    	srli	a2, a2, 15
    1fda:	632d                                    	ae_s16i.n	a2, a3, 0

00001fdc <plc_gainctrl+0x114>:
    1fdc:	f01d                                    	retw.n

00001fde <plc_gainctrl+0x116>:
    1fde:	060c                                    	movi.n	a6, 0
    1fe0:	d27c                                    	movi.n	a2, -3
    1fe2:	ffd506                                    	j	1f3a <plc_gainctrl+0x72>

00001fe5 <plc_gainctrl+0x11d>:
    1fe5:	ffd446                                    	j	1f3a <plc_gainctrl+0x72>

00001fe8 <plcpitch_sim_init>:
    1fe8:	004136                                    	entry	a1, 32
    1feb:	840c                                    	movi.n	a4, 8
    1fed:	a80c                                    	movi.n	a8, 10
    1fef:	190c                                    	movi.n	a9, 1
    1ff1:	326242                                    	s32i	a4, a2, 200
    1ff4:	114920                                    	slli	a4, a9, 14
    1ff7:	cca2b2                                    	movi	a11, 0x2cc
    1ffa:	6c1c                                    	movi.n	a12, 22
    1ffc:	f81531                                    	l32r	a3, 50 (e0008 <_end+0xdd674>)
    1fff:	346282                                    	s32i	a8, a2, 208
    2002:	590c                                    	movi.n	a9, 5
    2004:	376282                                    	s32i	a8, a2, 220
    2007:	e50c                                    	movi.n	a5, 14
    2009:	386282                                    	s32i	a8, a2, 224
    200c:	0fd282                                    	addmi	a8, a2, 0xf00
    200f:	f70c                                    	movi.n	a7, 15
    2011:	0a2c                                    	movi.n	a10, 32
    2013:	4462b2                                    	s32i	a11, a2, 0x110
    2016:	4d4c                                    	movi.n	a13, 68
    2018:	4562c2                                    	s32i	a12, a2, 0x114
    201b:	99a0e2                                    	movi	a14, 153
    201e:	f80df1                                    	l32r	a15, 54 (f000a <_end+0xed676>)
    2021:	5b0c                                    	movi.n	a11, 5
    2023:	2b6832                                    	s32i	a3, a8, 172
    2026:	0139f0                                    	slli	a3, a9, 17
    2029:	1c0c                                    	movi.n	a12, 1
    202b:	1ea562                                    	movi	a6, 0x51e
    202e:	336252                                    	s32i	a5, a2, 204
    2031:	19de52                                    	addmi	a5, a14, 0x1900
    2034:	356272                                    	s32i	a7, a2, 212
    2037:	1e0c                                    	movi.n	a14, 1
    2039:	3a6242                                    	s32i	a4, a2, 232
    203c:	070c                                    	movi.n	a7, 0
    203e:	4362a2                                    	s32i	a10, a2, 0x10c
    2041:	114c00                                    	slli	a4, a12, 16
    2044:	4662d2                                    	s32i	a13, a2, 0x118
    2047:	f805d1                                    	l32r	a13, 5c (10001 <_end+0xd66d>)
    204a:	2c68f2                                    	s32i	a15, a8, 176
    204d:	f802a1                                    	l32r	a10, 58 (1000a <_end+0xd676>)
    2050:	2d6832                                    	s32i	a3, a8, 180
    2053:	113b20                                    	slli	a3, a11, 14
    2056:	486252                                    	s32i	a5, a2, 0x120
    2059:	496262                                    	s32i	a6, a2, 0x124
    205c:	4a6262                                    	s32i	a6, a2, 0x128
    205f:	366272                                    	s32i	a7, a2, 216
    2062:	3c6272                                    	s32i	a7, a2, 240
    2065:	4b6272                                    	s32i	a7, a2, 0x12c
    2068:	2e68a2                                    	s32i	a10, a8, 184
    206b:	2f6832                                    	s32i	a3, a8, 188
    206e:	306842                                    	s32i	a4, a8, 192
    2071:	3962e2                                    	s32i	a14, a2, 228
    2074:	3b62e2                                    	s32i	a14, a2, 236
    2077:	3d62e2                                    	s32i	a14, a2, 244
    207a:	3e62e2                                    	s32i	a14, a2, 248
    207d:	3f62e2                                    	s32i	a14, a2, 252
    2080:	4062e2                                    	s32i	a14, a2, 0x100
    2083:	4162e2                                    	s32i	a14, a2, 0x104
    2086:	4262e2                                    	s32i	a14, a2, 0x108
    2089:	4762e2                                    	s32i	a14, a2, 0x11c
    208c:	3168d2                                    	s32i	a13, a8, 196
    208f:	3268d2                                    	s32i	a13, a8, 200
    2092:	6658e2                                    	s16i	a14, a8, 204
    2095:	f01d                                    	retw.n

00002097 <plcpitch_sim_init+0xaf>:
	...

00002098 <plcpitch_para_init>:
    2098:	004136                                    	entry	a1, 32
    209b:	032c                                    	movi.n	a3, 32
    209d:	cca242                                    	movi	a4, 0x2cc
    20a0:	0fd2a2                                    	addmi	a10, a2, 0xf00
    20a3:	99a072                                    	movi	a7, 153
    20a6:	436232                                    	s32i	a3, a2, 0x10c
    20a9:	454c                                    	movi.n	a5, 68
    20ab:	446242                                    	s32i	a4, a2, 0x110
    20ae:	19d742                                    	addmi	a4, a7, 0x1900
    20b1:	579a32                                    	l16si	a3, a10, 174
    20b4:	6f1c                                    	movi.n	a15, 22
    20b6:	090c                                    	movi.n	a9, 0
    20b8:	466252                                    	s32i	a5, a2, 0x118
    20bb:	486242                                    	s32i	a4, a2, 0x120
    20be:	569ab2                                    	l16si	a11, a10, 172
    20c1:	336232                                    	s32i	a3, a2, 204
    20c4:	599a42                                    	l16si	a4, a10, 178
    20c7:	4562f2                                    	s32i	a15, a2, 0x114
    20ca:	5b9a32                                    	l16si	a3, a10, 182
    20cd:	589ad2                                    	l16si	a13, a10, 176
    20d0:	4b6292                                    	s32i	a9, a2, 0x12c
    20d3:	5a9af2                                    	l16si	a15, a10, 180
    20d6:	3262b2                                    	s32i	a11, a2, 200
    20d9:	356242                                    	s32i	a4, a2, 212
    20dc:	5d9a42                                    	l16si	a4, a10, 186
    20df:	376232                                    	s32i	a3, a2, 220
    20e2:	5f9a32                                    	l16si	a3, a10, 190
    20e5:	0fd272                                    	addmi	a7, a2, 0xf00
    20e8:	1ea582                                    	movi	a8, 0x51e
    20eb:	160c                                    	movi.n	a6, 1
    20ed:	3462d2                                    	s32i	a13, a2, 208
    20f0:	6297c2                                    	l16si	a12, a7, 196
    20f3:	3662f2                                    	s32i	a15, a2, 216
    20f6:	396242                                    	s32i	a4, a2, 228
    20f9:	619a42                                    	l16si	a4, a10, 194
    20fc:	3b6232                                    	s32i	a3, a2, 236
    20ff:	639a32                                    	l16si	a3, a10, 198
    2102:	476262                                    	s32i	a6, a2, 0x11c
    2105:	5c9a62                                    	l16si	a6, a10, 184
    2108:	3d6242                                    	s32i	a4, a2, 244
    210b:	609aa2                                    	l16si	a10, a10, 192
    210e:	3f6232                                    	s32i	a3, a2, 252
    2111:	6497e2                                    	l16si	a14, a7, 200
    2114:	496282                                    	s32i	a8, a2, 0x124
    2117:	659742                                    	l16si	a4, a7, 202
    211a:	4a6282                                    	s32i	a8, a2, 0x128
    211d:	669732                                    	l16si	a3, a7, 204
    2120:	5e9782                                    	l16si	a8, a7, 188
    2123:	386262                                    	s32i	a6, a2, 224
    2126:	3a6282                                    	s32i	a8, a2, 232
    2129:	3c62a2                                    	s32i	a10, a2, 240
    212c:	3e62c2                                    	s32i	a12, a2, 248
    212f:	4062e2                                    	s32i	a14, a2, 0x100
    2132:	416242                                    	s32i	a4, a2, 0x104
    2135:	426232                                    	s32i	a3, a2, 0x108
    2138:	f01d                                    	retw.n

0000213a <plcpitch_para_init+0xa2>:
	...

0000213c <HIFI_plc_sqrt>:
    213c:	004136                                    	entry	a1, 32
    213f:	070c                                    	movi.n	a7, 0
    2141:	6113a6                                    	blti	a3, 1, 21a6 <HIFI_plc_sqrt+0x6a>
    2144:	40e340                                    	nsa	a4, a3
    2147:	c02240                                    	sub	a2, a2, a4
    214a:	401400                                    	ssl	a4
    214d:	044020                                    	extui	a4, a2, 0, 1
    2150:	a1d300                                    	sll	a13, a3
    2153:	400400                                    	ssr	a4
    2156:	f7c251                                    	l32r	a5, 60 (28c0 <TABLE_PLC_SQRT>)
    2159:	b130d0                                    	sra	a3, a13
    215c:	242a                                    	add.n	a2, a4, a2
    215e:	656930                                    	extui	a6, a3, 25, 7
    2161:	413930                                    	srli	a3, a3, 9
    2164:	070286c004bce33e                  	{ ae_cvtp24a16x2.ll	aed0, a3, a3; addi	a6, a6, -16 }
    216c:	536670                                    	max	a6, a6, a7
    216f:	905650                                    	addx2	a5, a6, a5
    2172:	456d                                    	ae_l16ui.n	a6, a5, 0
    2174:	355d                                    	ae_l16si.n	a5, a5, 2
    2176:	11f600                                    	slli	a15, a6, 16
    2179:	21070f4314e1456e                  	{ sub	a14, a5, a6; ae_cvt48a32	aed1, a15 }
    2181:	113e00                                    	slli	a3, a14, 16
    2184:	2107234010a2212e                  	{ srai	a2, a2, 1; ae_cvt48a32	aed2, a3 }
    218c:	20834f0b3440b13e                  	{ nop; nop; ae_mulaq32sp16u.l	aed1, aed2, aed0 }
    2194:	0ead257b0d240cd0c400813b0c01631f 	{ ae_trunca32q48	a3, aed1; nop; nop; nop }
    21a4:	f01d                                    	retw.n

000021a6 <HIFI_plc_sqrt+0x6a>:
    21a6:	020c                                    	movi.n	a2, 0
    21a8:	030c                                    	movi.n	a3, 0
    21aa:	f01d                                    	retw.n

000021ac <_start>:
    21ac:	004136                                    	entry	a1, 32
    21af:	f7ad31                                    	l32r	a3, 64 (2990 <printf_ptr>)
    21b2:	0228                                    	l32i.n	a2, a2, 0
    21b4:	f7ada1                                    	l32r	a10, 68 (293c <export_parameter_array+0xc>)
    21b7:	f7adb1                                    	l32r	a11, 6c (2098 <plcpitch_para_init>)
    21ba:	006322                                    	s32i	a2, a3, 0
    21bd:	0002e0                                    	callx8	a2
    21c0:	002322                                    	l32i	a2, a3, 0
    21c3:	f7aba1                                    	l32r	a10, 70 (2955 <export_parameter_array+0x25>)
    21c6:	f7abb1                                    	l32r	a11, 74 (13c <SMART_PITCH_PLC_INI>)
    21c9:	0002e0                                    	callx8	a2
    21cc:	002322                                    	l32i	a2, a3, 0
    21cf:	f7aaa1                                    	l32r	a10, 78 (296f <export_parameter_array+0x3f>)
    21d2:	f7aab1                                    	l32r	a11, 7c (3f8 <SMART_PITCH_PLC_PROCESS>)
    21d5:	0002e0                                    	callx8	a2
    21d8:	f7aa21                                    	l32r	a2, 80 (2930 <export_parameter_array>)
    21db:	f01d                                    	retw.n

000021dd <_start+0x31>:
    21dd:	000000                                        ...

000021e0 <__do_global_ctors_aux>:
    21e0:	004136                                    	entry	a1, 32
    21e3:	f7a831                                    	l32r	a3, 84 (2834 <__CTOR_END__>)
    21e6:	fcc322                                    	addi	a2, a3, -4
    21e9:	002222                                    	l32i	a2, a2, 0
    21ec:	020283a4001e920e                  	{ beqi.w15	a2, -1, 2201 <__do_global_ctors_aux+0x21>; addi	a3, a3, -8 }

000021f4 <__do_global_ctors_aux+0x14>:
    21f4:	0002e0                                    	callx8	a2
    21f7:	0328                                    	l32i.n	a2, a3, 0
    21f9:	0302e3bc07ffb26e                  	{ bnei.w15	a2, -1, 21f4 <__do_global_ctors_aux+0x14>; addi	a3, a3, -4 }
    2201:	f01d                                    	retw.n

00002203 <__do_global_ctors_aux+0x23>:
	...

00002210 <getfespeech>:
    2210:	004136                                    	entry	a1, 32
    2213:	07bd                                    	mov.n	a11, a7
    2215:	122772                                    	l32i	a7, a7, 72
    2218:	208550                                    	or	a8, a5, a5
    221b:	052737                                    	blt	a7, a3, 2224 <getfespeech+0x14>
    221e:	c07730                                    	sub	a7, a7, a3
    2221:	126b72                                    	s32i	a7, a11, 72

00002224 <getfespeech+0x14>:
    2224:	050c                                    	movi.n	a5, 0
    2226:	3118a6                                    	blti	a8, 1, 225b <getfespeech+0x4b>
    2229:	f03d                                    	nop.n
    222b:	f03d                                    	nop.n
    222d:	278876                                    	loop	a8, 2258 <getfespeech+0x48>
    2230:	180287411ce0374e                  	{ add	a7, a7, a4; addi	a9, a7, 1 }
    2238:	3206934604e9372e                  	{ addx2	a8, a7, a2; or	a7, a3, a3 }
    2240:	02069940008e18ae                  	{ l16si	a10, a8, 0; salt	a8, a9, a3 }
    2248:	3806b540008506ae                  	{ s16i	a10, a6, 0; movnez	a7, a5, a8 }
    2250:	270286411ce0497e                  	{ sub	a7, a9, a7; addi	a6, a6, 2 }

00002258 <getfespeech+0x48>:
    2258:	126b72                                    	s32i	a7, a11, 72

0000225b <getfespeech+0x4b>:
    225b:	f01d                                    	retw.n

0000225d <getfespeech+0x4d>:
    225d:	000000                                        ...

00002260 <__divdi3>:
    2260:	004136                                    	entry	a1, 32
    2263:	220742c00081007e                  	{ movi	a7, 0; neg	a6, a2 }
    226b:	1206974314f2474e                  	{ saltu	a14, a7, a4; saltu	a9, a7, a2 }
    2273:	3f06a54510f3a23e                  	{ movgez	a6, a2, a3; add	a15, a5, a14 }
    227b:	240702c21ce0239e                  	{ add	a11, a3, a9; neg	a2, a4 }
    2283:	1f0742c718827f8e                  	{ movi	a8, -1; neg	a9, a15 }
    228b:	2506a4c610bc3f3e                  	{ srai	a10, a3, 31; movgez	a2, a4, a5 }
    2293:	0b0702c104e27a8e                  	{ xor	a4, a10, a8; neg	a8, a11 }
    229b:	0506aac608fdb55e                  	{ movgez	a9, a5, a5; movgez	a4, a10, a5 }
    22a3:	0504204600e0b33e                  	{ movgez	a8, a3, a3; movi	a5, 0 }
    22ab:	22046024040099ae                  	{ beqz.w15	a9, 23a0 <__divdi3+0x140>; movi	a3, 32 }
    22b3:	2306b6070c52e89e                  	{ bltu.w15	a8, a9, 2536 <__divdi3+0x2d6>; saltu	a10, a6, a2 }
    22bb:	40f950                                    	nsau	a5, a9
    22be:	3506f320043a35ae                  	{ beqz.w15	a5, 24a8 <__divdi3+0x248>; sub	a11, a3, a5 }
    22c6:	401500                                    	ssl	a5
    22c9:	33074bc310a439ce                  	{ sll	a14, a9; ssr	a11 }
    22d1:	230745c71ca4712e                  	{ srl	a15, a2; ssl	a5 }
    22d9:	200722c418e4efee                  	{ or	a3, a15, a14; sll	a2, a2 }
    22e1:	130230c318fe7b6e                  	{ ssr	a11; extui	a9, a3, 16, 16 }
    22e9:	230745c70ca4718e                  	{ srl	a13, a8; ssl	a5 }
    22f1:	33074bc200a438ce                  	{ sll	a8, a8; ssr	a11 }
    22f9:	c2bd90                                    	quou	a11, a13, a9
    22fc:	82fb90                                    	mull	a15, a11, a9
    22ff:	230745c714a4716e                  	{ srl	a14, a6; ssl	a5 }
    2307:	1e06bd4600fbfe8e                  	{ or	a8, a14, a8; sub	a13, a13, a15 }
    230f:	93ad                                    	ae_zext16	a10, a3
    2311:	82cba0                                    	mull	a12, a11, a10
    2314:	1106ed471067e08e                  	{ extui	a14, a8, 16, 16; slli	a13, a13, 16 }
    231c:	1a069b430ce93dee                  	{ add	a13, a13, a14; or	a5, a11, a11 }
    2324:	14bdc7                                    	bgeu	a13, a12, 233c <__divdi3+0xdc>
    2327:	dd3a                                    	add.n	a13, a13, a3
    2329:	35028b87081f7d3e                  	{ bltu.w15	a13, a3, 233c <__divdi3+0xdc>; addi	a5, a11, -1 }
    2331:	07bdc7                                    	bgeu	a13, a12, 233c <__divdi3+0xdc>
    2334:	24028bc30cff3d3e                  	{ add	a13, a13, a3; addi	a5, a11, -2 }

0000233c <__divdi3+0xdc>:
    233c:	c0cdc0                                    	sub	a12, a13, a12
    233f:	c2bc90                                    	quou	a11, a12, a9
    2342:	829b90                                    	mull	a9, a11, a9
    2345:	98fd                                    	ae_zext16	a15, a8
    2347:	c09c90                                    	sub	a9, a12, a9
    234a:	119900                                    	slli	a9, a9, 16
    234d:	828ba0                                    	mull	a8, a11, a10
    2350:	2a069b420cea39fe                  	{ add	a9, a9, a15; or	a10, a11, a11 }
    2358:	14b987                                    	bgeu	a9, a8, 2370 <__divdi3+0x110>
    235b:	993a                                    	add.n	a9, a9, a3
    235d:	3b028b87081f693e                  	{ bltu.w15	a9, a3, 2370 <__divdi3+0x110>; addi	a10, a11, -1 }
    2365:	07b987                                    	bgeu	a9, a8, 2370 <__divdi3+0x110>
    2368:	2a028bc20cff293e                  	{ add	a9, a9, a3; addi	a10, a11, -2 }
    2370:	0906b94418ba050e                  	{ slli	a3, a5, 16; sub	a8, a9, a8 }
    2378:	205a30                                    	or	a5, a10, a3
    237b:	a23520                                    	muluh	a3, a5, a2
    237e:	822520                                    	mull	a2, a5, a2
    2381:	200706830805a83e                  	{ bltu.w15	a8, a3, 2399 <__divdi3+0x139>; sll	a6, a6 }
    2389:	220740850c25562e                  	{ bgeu.w15	a6, a2, 2536 <__divdi3+0x2d6>; nop }
    2391:	220740861c25183e                  	{ bne.w15	a8, a3, 2536 <__divdi3+0x2d6>; nop }
    2399:	550b                                    	addi.n	a5, a5, -1
    239b:	0065c6                                    	j	2536 <__divdi3+0x2d6>

0000239e <__divdi3+0x13e>:
	...

000023a0 <__divdi3+0x140>:
    23a0:	40f250                                    	nsau	a5, a2
    23a3:	220420050800382e                  	{ bgeu.w15	a8, a2, 23ec <__divdi3+0x18c>; movi	a3, 32 }
    23ab:	23040024040035ae                  	{ beqz.w15	a5, 23d6 <__divdi3+0x176>; movi	a3, 32 }
    23b3:	3506b34318f8755e                  	{ ssl	a5; sub	a3, a3, a5 }
    23bb:	010748c010a622ce                  	{ sll	a2, a2; sll	a8, a8 }
    23c3:	400300                                    	ssr	a3
    23c6:	230745c41ca4716e                  	{ srl	a3, a6; ssl	a5 }
    23ce:	200726c600e5e38e                  	{ or	a8, a3, a8; sll	a6, a6 }
    23d6:	370270c4187fe02e                  	{ extui	a3, a2, 16, 16; extui	a15, a6, 16, 16 }
    23de:	c29830                                    	quou	a9, a8, a3
    23e1:	82b930                                    	mull	a11, a9, a3
    23e4:	925d                                    	ae_zext16	a5, a2
    23e6:	c0e8b0                                    	sub	a14, a8, a11
    23e9:	003a46                                    	j	24d6 <__divdi3+0x276>

000023ec <__divdi3+0x18c>:
    23ec:	1406f320001a75ae                  	{ beqz.w15	a5, 24bc <__divdi3+0x25c>; sub	a9, a3, a5 }
    23f4:	37042ec318ff755e                  	{ ssl	a5; movi	a7, -2 }
    23fc:	330749c010a432ce                  	{ sll	a2, a2; ssr	a9 }
    2404:	330250c61cbc618e                  	{ srl	a11, a8; extui	a3, a2, 16, 16 }
    240c:	370230c318f9755e                  	{ ssl	a5; extui	a7, a7, 0, 16 }
    2414:	c2ab30                                    	quou	a10, a11, a3
    2417:	330749c318a438ce                  	{ sll	a15, a8; ssr	a9 }
    241f:	82da30                                    	mull	a13, a10, a3
    2422:	3706c2c704a5716e                  	{ srl	a12, a6; and	a7, a2, a7 }
    242a:	3c06bb4600fafcfe                  	{ or	a8, a12, a15; sub	a11, a11, a13 }
    2432:	829a70                                    	mull	a9, a10, a7
    2435:	3106eb470066e08e                  	{ extui	a12, a8, 16, 16; slli	a11, a11, 16 }
    243d:	3d06ab4318f2655e                  	{ ssl	a5; add	a11, a11, a12 }
    2445:	0a5d                                    	mov.n	a5, a10
    2447:	15bb97                                    	bgeu	a11, a9, 2460 <__divdi3+0x200>
    244a:	34028ac21cff3b2e                  	{ add	a11, a11, a2; addi	a5, a10, -1 }
    2452:	0a3b27                                    	bltu	a11, a2, 2460 <__divdi3+0x200>
    2455:	07bb97                                    	bgeu	a11, a9, 2460 <__divdi3+0x200>
    2458:	3206eb471472ea5e                  	{ addi	a5, a10, -2; add	a11, a11, a2 }
    2460:	c0ab90                                    	sub	a10, a11, a9
    2463:	c29a30                                    	quou	a9, a10, a3
    2466:	82f930                                    	mull	a15, a9, a3
    2469:	988d                                    	ae_zext16	a8, a8
    246b:	c0aaf0                                    	sub	a10, a10, a15
    246e:	11aa00                                    	slli	a10, a10, 16
    2471:	827970                                    	mull	a7, a9, a7
    2474:	2806994204ea3a8e                  	{ add	a8, a10, a8; or	a10, a9, a9 }
    247c:	14b877                                    	bgeu	a8, a7, 2494 <__divdi3+0x234>
    247f:	882a                                    	add.n	a8, a8, a2
    2481:	3b028987081f682e                  	{ bltu.w15	a8, a2, 2494 <__divdi3+0x234>; addi	a10, a9, -1 }
    2489:	07b877                                    	bgeu	a8, a7, 2494 <__divdi3+0x234>
    248c:	2a0289c204ff282e                  	{ add	a8, a8, a2; addi	a10, a9, -2 }
    2494:	0706b84508ba150e                  	{ slli	a5, a5, 16; sub	a8, a8, a7 }
    249c:	200726c518e5ea5e                  	{ or	a7, a10, a5; sll	a6, a6 }
    24a4:	000786                                    	j	24c6 <__divdi3+0x266>

000024a7 <__divdi3+0x247>:
	...

000024a8 <__divdi3+0x248>:
    24a8:	150c                                    	movi.n	a5, 1
    24aa:	3906994014f06a5e                  	{ xor	a2, a10, a5; saltu	a3, a9, a8 }
    24b2:	120693400069008e                  	{ j	2536 <__divdi3+0x2d6>; or	a5, a3, a2 }

000024ba <__divdi3+0x25a>:
	...

000024bc <__divdi3+0x25c>:
    24bc:	330210c204fc482e                  	{ sub	a8, a8, a2; extui	a3, a2, 16, 16 }
    24c4:	170c                                    	movi.n	a7, 1
    24c6:	c29830                                    	quou	a9, a8, a3
    24c9:	82b930                                    	mull	a11, a9, a3
    24cc:	925d                                    	ae_zext16	a5, a2
    24ce:	370210c314ff48be                  	{ sub	a14, a8, a11; extui	a15, a6, 16, 16 }
    24d6:	118e00                                    	slli	a8, a14, 16
    24d9:	82a950                                    	mull	a10, a9, a5
    24dc:	080699421cea38fe                  	{ add	a11, a8, a15; or	a8, a9, a9 }
    24e4:	15bba7                                    	bgeu	a11, a10, 24fd <__divdi3+0x29d>
    24e7:	380289c21cff2b2e                  	{ add	a11, a11, a2; addi	a8, a9, -1 }
    24ef:	0a3b27                                    	bltu	a11, a2, 24fd <__divdi3+0x29d>
    24f2:	07bba7                                    	bgeu	a11, a10, 24fd <__divdi3+0x29d>
    24f5:	3206eb471472e98e                  	{ addi	a8, a9, -2; add	a11, a11, a2 }
    24fd:	c09ba0                                    	sub	a9, a11, a10
    2500:	96dd                                    	ae_zext16	a13, a6
    2502:	c26930                                    	quou	a6, a9, a3
    2505:	82e630                                    	mull	a14, a6, a3
    2508:	823650                                    	mull	a3, a6, a5
    250b:	39040fc31cff59ee                  	{ sub	a15, a9, a14; movi	a9, -1 }
    2513:	115f00                                    	slli	a5, a15, 16
    2516:	55da                                    	add.n	a5, a5, a13
    2518:	120685010811a53e                  	{ bgeu.w15	a5, a3, 2530 <__divdi3+0x2d0>; add	a5, a5, a2 }
    2520:	220695401cf0453e                  	{ saltu	a3, a5, a3; saltu	a2, a5, a2 }
    2528:	c03930                                    	sub	a3, a9, a3
    252b:	933920                                    	movnez	a3, a9, a2
    252e:	636a                                    	add.n	a6, a3, a6
    2530:	112800                                    	slli	a2, a8, 16
    2533:	205620                                    	or	a5, a6, a2
    2536:	020c                                    	movi.n	a2, 0
    2538:	622250                                    	saltu	a2, a2, a5
    253b:	240702c01ce0372e                  	{ add	a3, a7, a2; neg	a2, a5 }
    2543:	250685c418f8cf3e                  	{ neg	a3, a3; moveqz	a2, a5, a4 }
    254b:	833740                                    	moveqz	a3, a7, a4
    254e:	f01d                                    	retw.n

00002550 <_fini>:
    2550:	008136                                    	entry	a1, 64
    2553:	f6cd81                                    	l32r	a8, 88 (a4 <__do_global_dtors_aux>)
    2556:	f03d                                    	nop.n
    2558:	0008e0                                    	callx8	a8

0000255b <_fini+0xb>:
    255b:	f01d                                    	retw.n
