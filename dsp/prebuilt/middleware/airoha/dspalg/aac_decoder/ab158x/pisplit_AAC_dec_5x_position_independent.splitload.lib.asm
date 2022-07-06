
Build/lib/pisplit_AAC_dec_5x_position_independent.splitload.lib:     file format elf32-xtensa-le


Disassembly of section .text:

00000000 <_init-0x19c>:
       0:	000001ec 00003a48 0000455c 00000000     ....H:..\E......
      10:	00004524 00000000 00004544 000002f0     $E......DE......
      20:	00000334 00000344 0000038c 00000540     4...D.......@...
      30:	00003878 0000ac44 000063e0 000003ec     x8..D....c......
      40:	0000222c 00002360 00001af8 000022f4     ,"..`#......."..
      50:	0000230c 000022d0 00001d98 000007a8     .#..."..........
      60:	0000226c 0000066c 000022b4 00000e04     l"..l...."......
      70:	00000ae8 00000c50 0000388c 00001284     ....P....8......
      80:	00001e6c 00002b50 000011c0 000038ac     l...P+.......8..
      90:	000038c0 00003740 000063d0 00001a00     .8..@7...c......
      a0:	000015fc 0000172c 00001850 00004810     ....,...P....H..
      b0:	000027f4 00002b10 00003a6c 00001648     .'...+..l:..H...
      c0:	0000259c 00002838 000023a4 00002458     .%..8(...#..X$..
      d0:	000026a8 00002378 00001acc 000045c0     .&..x#.......E..
      e0:	000002c8 00006350 00001f90 0000206c     ....Pc......l ..
      f0:	00002128 00004800 000058c0 00005f40     (!...H...X..@_..
     100:	00005e20 00004f60 00005130 00004fb0      ^..`O..0Q...O..
     110:	00005d00 00005b20 000052a0 000056d0     .].. [...R...V..
     120:	00005600 000053f0 00006020 00002a44     .V...S.. `..D*..
     130:	00004f50 00004f20 00004fa0 000099d0     PO.. O...O......
     140:	000031d4 000033cc 000075d0 00006410     .1...3...u...d..
     150:	00003554 00003694 000085d0 000073f0     T5...6.......s..
     160:	000099e0 0000457c 00000210 0000458f     ....|E.......E..
     170:	00000280 000045a5 000002e0 00004570     .....E......pE..
     180:	00004df0 00004a20 00004b00 00004a30     .M.. J...K..0J..
     190:	000045e0 0000452c 000001b4              .E..,E......

0000019c <_init>:
     19c:	008136                                    	entry	a1, 64
     19f:	ff9881                                    	l32r	a8, 0 (1ec <frame_dummy>)
     1a2:	f03d                                    	nop.n
     1a4:	0008e0                                    	callx8	a8
     1a7:	ff9781                                    	l32r	a8, 4 (3a48 <__do_global_ctors_aux>)
     1aa:	f03d                                    	nop.n
     1ac:	0008e0                                    	callx8	a8
     1af:	f01d                                    	retw.n

000001b1 <_init+0x15>:
     1b1:	000000                                        ...

000001b4 <__do_global_dtors_aux>:
     1b4:	004136                                    	entry	a1, 32
     1b7:	ff9421                                    	l32r	a2, 8 (455c <__do_global_dtors_aux.completed>)
     1ba:	000232                                    	l8ui	a3, a2, 0
     1bd:	63ec                                    	bnez.n	a3, 1e7 <__do_global_dtors_aux+0x33>

000001bf <__do_global_dtors_aux+0xb>:
     1bf:	1248                                    	l32i.n	a4, a2, 4
     1c1:	0438                                    	l32i.n	a3, a4, 0
     1c3:	04028424000163ae                  	{ beqz.w15	a3, 1d4 <__do_global_dtors_aux+0x20>; addi	a4, a4, 4 }
     1cb:	1249                                    	s32i.n	a4, a2, 4
     1cd:	0003e0                                    	callx8	a3
     1d0:	fffac6                                    	j	1bf <__do_global_dtors_aux+0xb>

000001d3 <__do_global_dtors_aux+0x1f>:
	...

000001d4 <__do_global_dtors_aux+0x20>:
     1d4:	ff8e31                                    	l32r	a3, c (0 <_text_start>)
     1d7:	ff8ea1                                    	l32r	a10, 10 (4524 <__FRAME_END__>)
     1da:	438c                                    	beqz.n	a3, 1e2 <__do_global_dtors_aux+0x2e>
     1dc:	ff8c81                                    	l32r	a8, c (0 <_text_start>)
     1df:	0008e0                                    	callx8	a8
     1e2:	130c                                    	movi.n	a3, 1
     1e4:	004232                                    	s8i	a3, a2, 0
     1e7:	f01d                                    	retw.n

000001e9 <__do_global_dtors_aux+0x35>:
     1e9:	000000                                        ...

000001ec <frame_dummy>:
     1ec:	004136                                    	entry	a1, 32
     1ef:	ff8921                                    	l32r	a2, 14 (0 <_text_start>)
     1f2:	ff87a1                                    	l32r	a10, 10 (4524 <__FRAME_END__>)
     1f5:	ff88b1                                    	l32r	a11, 18 (4544 <frame_dummy.object>)
     1f8:	428c                                    	beqz.n	a2, 200 <frame_dummy+0x14>
     1fa:	ff8681                                    	l32r	a8, 14 (0 <_text_start>)
     1fd:	0008e0                                    	callx8	a8

00000200 <frame_dummy+0x14>:
     200:	f01d                                    	retw.n

00000202 <frame_dummy+0x16>:
	...

00000210 <AIR_AAC_init>:
     210:	006136                                    	entry	a1, 48
     213:	2179                                    	s32i.n	a7, a1, 8
     215:	2b0c                                    	movi.n	a11, 2
     217:	ff8171                                    	l32r	a7, 1c (2f0 <aac_open>)
     21a:	02ad                                    	mov.n	a10, a2
     21c:	0007e0                                    	callx8	a7
     21f:	ff8021                                    	l32r	a2, 20 (334 <aac_get_config>)
     222:	0a7d                                    	mov.n	a7, a10
     224:	0002e0                                    	callx8	a2
     227:	120c                                    	movi.n	a2, 1
     229:	0abd                                    	mov.n	a11, a10
     22b:	094a22                                    	s8i	a2, a10, 9
     22e:	07ad                                    	mov.n	a10, a7
     230:	ff7d21                                    	l32r	a2, 24 (344 <aac_set_config>)
     233:	0002e0                                    	callx8	a2
     236:	000322                                    	l8ui	a2, a3, 0
     239:	d14b                                    	addi.n	a13, a1, 4
     23b:	010382                                    	l8ui	a8, a3, 1
     23e:	e1cb                                    	addi.n	a14, a1, 12
     240:	020392                                    	l8ui	a9, a3, 2
     243:	0303a2                                    	l8ui	a10, a3, 3
     246:	012280                                    	slli	a2, a2, 24
     249:	118800                                    	slli	a8, a8, 16
     24c:	03bd                                    	mov.n	a11, a3
     24e:	11f980                                    	slli	a15, a9, 8
     251:	882a                                    	add.n	a8, a8, a2
     253:	9faa                                    	add.n	a9, a15, a10
     255:	020c                                    	movi.n	a2, 0
     257:	889a                                    	add.n	a8, a8, a9
     259:	07ad                                    	mov.n	a10, a7
     25b:	f3f180                                    	wur.ae_bithead	a8
     25e:	67ef24                                    	wur.ae_bitptr	a2
     261:	ff7181                                    	l32r	a8, 28 (38c <aac_dec_init>)
     264:	04cd                                    	mov.n	a12, a4
     266:	0008e0                                    	callx8	a8
     269:	2148                                    	l32i.n	a4, a1, 8
     26b:	00da96                                    	bltz	a10, 27c <AIR_AAC_init+0x6c>
     26e:	220c                                    	movi.n	a2, 2
     270:	11f8                                    	l32i.n	a15, a1, 4
     272:	652d                                    	ae_s16i.n	a2, a5, 0
     274:	030c                                    	movi.n	a3, 0
     276:	120c                                    	movi.n	a2, 1
     278:	06f9                                    	s32i.n	a15, a6, 0
     27a:	643d                                    	ae_s16i.n	a3, a4, 0

0000027c <AIR_AAC_init+0x6c>:
     27c:	f01d                                    	retw.n

0000027e <AIR_AAC_init+0x6e>:
	...

00000280 <AIR_AAC_decoder>:
     280:	008136                                    	entry	a1, 64
     283:	080c                                    	movi.n	a8, 0
     285:	b18b                                    	addi.n	a11, a1, 8
     287:	02ad                                    	mov.n	a10, a2
     289:	114182                                    	s8i	a8, a1, 17
     28c:	03cd                                    	mov.n	a12, a3
     28e:	0789                                    	s32i.n	a8, a7, 0
     290:	00a432                                    	movi	a3, 0x400
     293:	54ac                                    	beqz.n	a4, 2bc <AIR_AAC_decoder+0x3c>
     295:	84dd                                    	ae_sext16	a13, a4
     297:	ff6541                                    	l32r	a4, 2c (540 <aac_dec_decode>)
     29a:	6259                                    	s32i.n	a5, a2, 24
     29c:	7269                                    	s32i.n	a6, a2, 28
     29e:	0004e0                                    	callx8	a4
     2a1:	110122                                    	l8ui	a2, a1, 17
     2a4:	3148                                    	l32i.n	a4, a1, 12
     2a6:	729c                                    	beqz.n	a2, 2c1 <AIR_AAC_decoder+0x41>
     2a8:	0739                                    	s32i.n	a3, a7, 0
     2aa:	00a442                                    	movi	a4, 0x400
     2ad:	030c                                    	movi.n	a3, 0
     2af:	078476                                    	loop	a4, 2ba <AIR_AAC_decoder+0x3a>
     2b2:	0539                                    	s32i.n	a3, a5, 0
     2b4:	554b                                    	addi.n	a5, a5, 4
     2b6:	0639                                    	s32i.n	a3, a6, 0
     2b8:	664b                                    	addi.n	a6, a6, 4

000002ba <AIR_AAC_decoder+0x3a>:
     2ba:	f01d                                    	retw.n

000002bc <AIR_AAC_decoder+0x3c>:
     2bc:	ffa022                                    	movi	a2, 255
     2bf:	f01d                                    	retw.n

000002c1 <AIR_AAC_decoder+0x41>:
     2c1:	413140                                    	srli	a3, a4, 1
     2c4:	0739                                    	s32i.n	a3, a7, 0
     2c6:	f01d                                    	retw.n

000002c8 <memcpy>:
     2c8:	004136                                    	entry	a1, 32
     2cb:	025d                                    	mov.n	a5, a2
     2cd:	b48c                                    	beqz.n	a4, 2dc <memcpy+0x14>
     2cf:	098476                                    	loop	a4, 2dc <memcpy+0x14>
     2d2:	000342                                    	l8ui	a4, a3, 0
     2d5:	331b                                    	addi.n	a3, a3, 1
     2d7:	004542                                    	s8i	a4, a5, 0
     2da:	551b                                    	addi.n	a5, a5, 1

000002dc <memcpy+0x14>:
     2dc:	f01d                                    	retw.n

000002de <memcpy+0x16>:
	...

000002e0 <get_AACdec_memsize>:
     2e0:	004136                                    	entry	a1, 32
     2e3:	722c                                    	movi.n	a2, 39
     2e5:	112270                                    	slli	a2, a2, 9
     2e8:	f01d                                    	retw.n

000002ea <get_AACdec_memsize+0xa>:
     2ea:	00000000                                 ......

000002f0 <aac_open>:
     2f0:	004136                                    	entry	a1, 32
     2f3:	ff4f41                                    	l32r	a4, 30 (3878 <memset>)
     2f6:	7c2c                                    	movi.n	a12, 39
     2f8:	11cc70                                    	slli	a12, a12, 9
     2fb:	0b0c                                    	movi.n	a11, 0
     2fd:	02ad                                    	mov.n	a10, a2
     2ff:	050c                                    	movi.n	a5, 0
     301:	0004e0                                    	callx8	a4
     304:	40d2d2                                    	addmi	a13, a2, 0x4000
     307:	00a4e2                                    	movi	a14, 0x400
     30a:	140c                                    	movi.n	a4, 1
     30c:	ff4af1                                    	l32r	a15, 34 (ac44 <_end+0x1260>)
     30f:	074232                                    	s8i	a3, a2, 7
     312:	305d42                                    	s16i	a4, a13, 96
     315:	584d42                                    	s8i	a4, a13, 88
     318:	0252e2                                    	s16i	a14, a2, 4
     31b:	064252                                    	s8i	a5, a2, 6
     31e:	0259                                    	s32i.n	a5, a2, 0
     320:	176df2                                    	s32i	a15, a13, 92
     323:	144252                                    	s8i	a5, a2, 20
     326:	134252                                    	s8i	a5, a2, 19
     329:	9259                                    	s32i.n	a5, a2, 36
     32b:	a259                                    	s32i.n	a5, a2, 40
     32d:	b259                                    	s32i.n	a5, a2, 44
     32f:	c259                                    	s32i.n	a5, a2, 48
     331:	f01d                                    	retw.n

00000333 <aac_open+0x43>:
	...

00000334 <aac_get_config>:
     334:	004136                                    	entry	a1, 32
     337:	835c                                    	movi.n	a3, 88
     339:	40d332                                    	addmi	a3, a3, 0x4000
     33c:	323a                                    	add.n	a3, a2, a3
     33e:	932320                                    	movnez	a2, a3, a2
     341:	f01d                                    	retw.n

00000343 <aac_get_config+0xf>:
	...

00000344 <aac_set_config>:
     344:	004136                                    	entry	a1, 32
     347:	024d                                    	mov.n	a4, a2
     349:	020c                                    	movi.n	a2, 0
     34b:	40d462                                    	addmi	a6, a4, 0x4000
     34e:	74bc                                    	beqz.n	a4, 389 <aac_set_config+0x45>
     350:	270c                                    	movi.n	a7, 2
     352:	33bc                                    	beqz.n	a3, 389 <aac_set_config+0x45>
     354:	000352                                    	l8ui	a5, a3, 0
     357:	40d4a2                                    	addmi	a10, a4, 0x4000
     35a:	1388                                    	l32i.n	a8, a3, 4
     35c:	080392                                    	l8ui	a9, a3, 8
     35f:	090332                                    	l8ui	a3, a3, 9
     362:	232566                                    	bnei	a5, 2, 389 <aac_set_config+0x45>
     365:	584672                                    	s8i	a7, a6, 88
     368:	085d                                    	mov.n	a5, a8
     36a:	690b                                    	addi.n	a6, a9, -1
     36c:	989c                                    	beqz.n	a8, 389 <aac_set_config+0x45>
     36e:	176a52                                    	s32i	a5, a10, 92
     371:	40d452                                    	addmi	a5, a4, 0x4000
     374:	b68d                                    	ae_zext8	a8, a6
     376:	096d                                    	mov.n	a6, a9
     378:	0d48f6                                    	bgeui	a8, 4, 389 <aac_set_config+0x45>
     37b:	40d472                                    	addmi	a7, a4, 0x4000
     37e:	604562                                    	s8i	a6, a5, 96
     381:	0423f6                                    	bgeui	a3, 2, 389 <aac_set_config+0x45>
     384:	120c                                    	movi.n	a2, 1
     386:	614732                                    	s8i	a3, a7, 97

00000389 <aac_set_config+0x45>:
     389:	f01d                                    	retw.n

0000038b <aac_set_config+0x47>:
	...

0000038c <aac_dec_init>:
     38c:	006136                                    	entry	a1, 48
     38f:	03bd                                    	mov.n	a11, a3
     391:	0f0c                                    	movi.n	a15, 0
     393:	023d                                    	mov.n	a3, a2
     395:	f27c                                    	movi.n	a2, -1
     397:	31f9                                    	s32i.n	a15, a1, 12
     399:	2b8c                                    	beqz.n	a11, 39f <aac_dec_init+0x13>
     39b:	04cc                                    	bnez.n	a4, 39f <aac_dec_init+0x13>

0000039d <aac_dec_init+0x11>:
     39d:	f01d                                    	retw.n

0000039f <aac_dec_init+0x13>:
     39f:	40d372                                    	addmi	a7, a3, 0x4000
     3a2:	ff7316                                    	beqz	a3, 39d <aac_dec_init+0x11>
     3a5:	1f0c                                    	movi.n	a15, 1
     3a7:	ff2516                                    	beqz	a5, 39d <aac_dec_init+0x11>
     3aa:	03ad                                    	mov.n	a10, a3
     3ac:	fed616                                    	beqz	a6, 39d <aac_dec_init+0x11>
     3af:	ff2281                                    	l32r	a8, 38 (63e0 <sample_rates>)
     3b2:	04cd                                    	mov.n	a12, a4
     3b4:	580722                                    	l8ui	a2, a7, 88
     3b7:	470c                                    	movi.n	a7, 4
     3b9:	094322                                    	s8i	a2, a3, 9
     3bc:	05dd                                    	mov.n	a13, a5
     3be:	48e8                                    	l32i.n	a14, a8, 16
     3c0:	084372                                    	s8i	a7, a3, 8
     3c3:	ff1e21                                    	l32r	a2, 3c (3ec <aac_dec_init_buffer>)
     3c6:	05e9                                    	s32i.n	a14, a5, 0
     3c8:	06ed                                    	mov.n	a14, a6
     3ca:	0046f2                                    	s8i	a15, a6, 0
     3cd:	f1cb                                    	addi.n	a15, a1, 12
     3cf:	0002e0                                    	callx8	a2
     3d2:	030076                                    	bf	b0, 3d9 <aac_dec_init+0x4d>
     3d5:	3128                                    	l32i.n	a2, a1, 12
     3d7:	f01d                                    	retw.n

000003d9 <aac_dec_init+0x4d>:
     3d9:	090342                                    	l8ui	a4, a3, 9
     3dc:	000652                                    	l8ui	a5, a6, 0
     3df:	f27c                                    	movi.n	a2, -1
     3e1:	0a4352                                    	s8i	a5, a3, 10
     3e4:	fec442                                    	addi	a4, a4, -2
     3e7:	832440                                    	moveqz	a2, a4, a4
     3ea:	f01d                                    	retw.n

000003ec <aac_dec_init_buffer>:
     3ec:	00e136                                    	entry	a1, 112
     3ef:	64a0e2                                    	movi	a14, 100
     3f2:	0f1316                                    	beqz	a3, 4e7 <aac_dec_init_buffer+0xfb>
     3f5:	d159                                    	s32i.n	a5, a1, 52
     3f7:	40de52                                    	addmi	a5, a14, 0x4000
     3fa:	c169                                    	s32i.n	a6, a1, 48
     3fc:	40d2f2                                    	addmi	a15, a2, 0x4000
     3ff:	625a                                    	add.n	a6, a2, a5
     401:	106122                                    	s32i	a2, a1, 64
     404:	ff0f21                                    	l32r	a2, 40 (222c <aac_initbits>)
     407:	51cb                                    	addi.n	a5, a1, 12
     409:	03bd                                    	mov.n	a11, a3
     40b:	04cd                                    	mov.n	a12, a4
     40d:	05ad                                    	mov.n	a10, a5
     40f:	e179                                    	s32i.n	a7, a1, 56
     411:	f1f9                                    	s32i.n	a15, a1, 60
     413:	0002e0                                    	callx8	a2
     416:	ff0621                                    	l32r	a2, 30 (3878 <memset>)
     419:	0b0c                                    	movi.n	a11, 0
     41b:	0c5c                                    	movi.n	a12, 80
     41d:	06ad                                    	mov.n	a10, a6
     41f:	0002e0                                    	callx8	a2
     422:	ff0821                                    	l32r	a2, 44 (2360 <aac_used_bytes>)
     425:	05ad                                    	mov.n	a10, a5
     427:	0002e0                                    	callx8	a2
     42a:	5138                                    	l32i.n	a3, a1, 20
     42c:	020c                                    	movi.n	a2, 0
     42e:	844d                                    	ae_sext16	a4, a4
     430:	04dd                                    	mov.n	a13, a4
     432:	18c152                                    	addi	a5, a1, 24
     435:	39aa37                                    	bge	a10, a3, 472 <aac_dec_init_buffer+0x86>
     438:	31cb                                    	addi.n	a3, a1, 12
     43a:	020c                                    	movi.n	a2, 0

0000043c <aac_dec_init_buffer+0x50>:
     43c:	ff0371                                    	l32r	a7, 48 (1af8 <aac_loas_frame>)
     43f:	06ad                                    	mov.n	a10, a6
     441:	03bd                                    	mov.n	a11, a3
     443:	05cd                                    	mov.n	a12, a5
     445:	0007e0                                    	callx8	a7
     448:	ff0181                                    	l32r	a8, 4c (22f4 <aac_get_processed_bits>)
     44b:	0a7d                                    	mov.n	a7, a10
     44d:	03ad                                    	mov.n	a10, a3
     44f:	0008e0                                    	callx8	a8
     452:	ba7a                                    	add.n	a11, a10, a7
     454:	0b0726                                    	beqi	a7, -1, 463 <aac_dec_init_buffer+0x77>
     457:	03ad                                    	mov.n	a10, a3
     459:	221b                                    	addi.n	a2, a2, 1
     45b:	478c                                    	beqz.n	a7, 463 <aac_dec_init_buffer+0x77>
     45d:	fefc71                                    	l32r	a7, 50 (230c <aac_resetbits>)
     460:	0007e0                                    	callx8	a7
     463:	fef871                                    	l32r	a7, 44 (2360 <aac_used_bytes>)
     466:	03ad                                    	mov.n	a10, a3
     468:	0007e0                                    	callx8	a7
     46b:	5178                                    	l32i.n	a7, a1, 20
     46d:	04dd                                    	mov.n	a13, a4
     46f:	c92a77                                    	blt	a10, a7, 43c <aac_dec_init_buffer+0x50>
     472:	f138                                    	l32i.n	a3, a1, 60
     474:	040c                                    	movi.n	a4, 0
     476:	102162                                    	l32i	a6, a1, 64
     479:	a1cb                                    	addi.n	a10, a1, 12
     47b:	0b0c                                    	movi.n	a11, 0
     47d:	644342                                    	s8i	a4, a3, 100
     480:	fef431                                    	l32r	a3, 50 (230c <aac_resetbits>)
     483:	40d6f2                                    	addmi	a15, a6, 0x4000
     486:	355f42                                    	s16i	a4, a15, 106
     489:	40d652                                    	addmi	a5, a6, 0x4000
     48c:	0003e0                                    	callx8	a3
     48f:	054216                                    	beqz	a2, 4e7 <aac_dec_init_buffer+0xfb>
     492:	2c2522                                    	l32i	a2, a5, 176
     495:	e178                                    	l32i.n	a7, a1, 56
     497:	07a452                                    	movi	a5, 0x407
     49a:	fa7c                                    	movi.n	a10, -1
     49c:	144c                                    	movi.n	a4, 65
     49e:	1195c0                                    	slli	a9, a5, 4
     4a1:	130c                                    	movi.n	a3, 1
     4a3:	040216                                    	beqz	a2, 4e7 <aac_dec_init_buffer+0xfb>
     4a6:	227b                                    	addi.n	a2, a2, 7
     4a8:	064632                                    	s8i	a3, a6, 6
     4ab:	418320                                    	srli	a8, a2, 3
     4ae:	69b2b6                                    	bltui	a2, 16, 51b <aac_dec_init_buffer+0x12f>
     4b1:	40d442                                    	addmi	a4, a4, 0x4000
     4b4:	b69a                                    	add.n	a11, a6, a9
     4b6:	88cd                                    	ae_sext16	a12, a8
     4b8:	364a                                    	add.n	a3, a6, a4
     4ba:	44c142                                    	addi	a4, a1, 68
     4bd:	fee021                                    	l32r	a2, 40 (222c <aac_initbits>)
     4c0:	04ad                                    	mov.n	a10, a4
     4c2:	0002e0                                    	callx8	a2
     4c5:	fee321                                    	l32r	a2, 54 (22d0 <aac_byte_align>)
     4c8:	04ad                                    	mov.n	a10, a4
     4ca:	0002e0                                    	callx8	a2
     4cd:	fee221                                    	l32r	a2, 58 (1d98 <aac_specific_config>)
     4d0:	18c1b2                                    	addi	a11, a1, 24
     4d3:	04ad                                    	mov.n	a10, a4
     4d5:	03cd                                    	mov.n	a12, a3
     4d7:	0002e0                                    	callx8	a2
     4da:	200122                                    	l8ui	a2, a1, 32
     4dd:	b28c                                    	beqz.n	a2, 4ec <aac_dec_init_buffer+0x100>
     4df:	c138                                    	l32i.n	a3, a1, 48
     4e1:	004322                                    	s8i	a2, a3, 0
     4e4:	0005c6                                    	j	4ff <aac_dec_init_buffer+0x113>

000004e7 <aac_dec_init_buffer+0xfb>:
     4e7:	020c                                    	movi.n	a2, 0
     4e9:	000dc6                                    	j	524 <aac_dec_init_buffer+0x138>

000004ec <aac_dec_init_buffer+0x100>:
     4ec:	c188                                    	l32i.n	a8, a1, 48
     4ee:	40d632                                    	addmi	a3, a6, 0x4000
     4f1:	440332                                    	l8ui	a3, a3, 68
     4f4:	40d642                                    	addmi	a4, a6, 0x4000
     4f7:	150c                                    	movi.n	a5, 1
     4f9:	004832                                    	s8i	a3, a8, 0
     4fc:	404452                                    	s8i	a5, a4, 64

000004ff <aac_dec_init_buffer+0x113>:
     4ff:	d148                                    	l32i.n	a4, a1, 52
     501:	7138                                    	l32i.n	a3, a1, 28
     503:	1901e2                                    	l8ui	a14, a1, 25
     506:	f57c                                    	movi.n	a5, -1
     508:	1801f2                                    	l8ui	a15, a1, 24
     50b:	0439                                    	s32i.n	a3, a4, 0
     50d:	210132                                    	l8ui	a3, a1, 33
     510:	0846e2                                    	s8i	a14, a6, 8
     513:	0946f2                                    	s8i	a15, a6, 9
     516:	835330                                    	moveqz	a5, a3, a3
     519:	8a9c                                    	beqz.n	a10, 535 <aac_dec_init_buffer+0x149>
     51b:	07a9                                    	s32i.n	a10, a7, 0
     51d:	020c                                    	movi.n	a2, 0
     51f:	064622                                    	s8i	a2, a6, 6
     522:	120c                                    	movi.n	a2, 1
     524:	030430                                    	rsr.br	a3
     527:	413130                                    	srli	a3, a3, 1
     52a:	042020                                    	extui	a2, a2, 0, 1
     52d:	903320                                    	addx2	a3, a3, a2
     530:	130430                                    	wsr.br	a3
     533:	f01d                                    	retw.n

00000535 <aac_dec_init_buffer+0x149>:
     535:	0a4622                                    	s8i	a2, a6, 10
     538:	0759                                    	s32i.n	a5, a7, 0
     53a:	fdf356                                    	bnez	a3, 51d <aac_dec_init_buffer+0x131>
     53d:	fff846                                    	j	522 <aac_dec_init_buffer+0x136>

00000540 <aac_dec_decode>:
     540:	008136                                    	entry	a1, 64
     543:	026d                                    	mov.n	a6, a2
     545:	09e216                                    	beqz	a2, 5e7 <aac_dec_decode+0xa7>
     548:	0b0c                                    	movi.n	a11, 0
     54a:	099316                                    	beqz	a3, 5e7 <aac_dec_decode+0xa7>
     54d:	8c1c                                    	movi.n	a12, 24
     54f:	094416                                    	beqz	a4, 5e7 <aac_dec_decode+0xa7>
     552:	0212f2                                    	l16ui	a15, a2, 4
     555:	03ad                                    	mov.n	a10, a3
     557:	feb621                                    	l32r	a2, 30 (3878 <memset>)
     55a:	070c                                    	movi.n	a7, 0
     55c:	41f9                                    	s32i.n	a15, a1, 16
     55e:	0002e0                                    	callx8	a2
     561:	40d622                                    	addmi	a2, a6, 0x4000
     564:	564272                                    	s8i	a7, a2, 86
     567:	554272                                    	s8i	a7, a2, 85
     56a:	c21c                                    	movi.n	a2, 28
     56c:	07d516                                    	beqz	a5, 5ed <aac_dec_decode+0xad>
     56f:	feb421                                    	l32r	a2, 40 (222c <aac_initbits>)
     572:	14c1a2                                    	addi	a10, a1, 20
     575:	04bd                                    	mov.n	a11, a4
     577:	05cd                                    	mov.n	a12, a5
     579:	0002e0                                    	callx8	a2
     57c:	060622                                    	l8ui	a2, a6, 6
     57f:	feb371                                    	l32r	a7, 4c (22f4 <aac_get_processed_bits>)
     582:	feb141                                    	l32r	a4, 48 (1af8 <aac_loas_frame>)
     585:	e29c                                    	beqz.n	a2, 5a7 <aac_dec_decode+0x67>
     587:	64a022                                    	movi	a2, 100
     58a:	c1cb                                    	addi.n	a12, a1, 12
     58c:	40d222                                    	addmi	a2, a2, 0x4000
     58f:	05dd                                    	mov.n	a13, a5
     591:	a62a                                    	add.n	a10, a6, a2
     593:	14c122                                    	addi	a2, a1, 20
     596:	02bd                                    	mov.n	a11, a2
     598:	0004e0                                    	callx8	a4
     59b:	0a4d                                    	mov.n	a4, a10
     59d:	02ad                                    	mov.n	a10, a2
     59f:	0007e0                                    	callx8	a7
     5a2:	d21c                                    	movi.n	a2, 29
     5a4:	570426                                    	beqi	a4, -1, 5ff <aac_dec_decode+0xbf>

000005a7 <aac_dec_decode+0x67>:
     5a7:	124c                                    	movi.n	a2, 65
     5a9:	14c1c2                                    	addi	a12, a1, 20
     5ac:	40d222                                    	addmi	a2, a2, 0x4000
     5af:	06ad                                    	mov.n	a10, a6
     5b1:	d62a                                    	add.n	a13, a6, a2
     5b3:	feaa21                                    	l32r	a2, 5c (7a8 <raw_data_block>)
     5b6:	03bd                                    	mov.n	a11, a3
     5b8:	40d642                                    	addmi	a4, a6, 0x4000
     5bb:	0002e0                                    	callx8	a2
     5be:	060622                                    	l8ui	a2, a6, 6
     5c1:	429c                                    	beqz.n	a2, 5d9 <aac_dec_decode+0x99>
     5c3:	14c1a2                                    	addi	a10, a1, 20
     5c6:	0007e0                                    	callx8	a7
     5c9:	1b2422                                    	l32i	a2, a4, 108
     5cc:	14c1a2                                    	addi	a10, a1, 20
     5cf:	628c                                    	beqz.n	a2, 5d9 <aac_dec_decode+0x99>
     5d1:	82bd                                    	ae_sext16	a11, a2
     5d3:	fea321                                    	l32r	a2, 60 (226c <aac_getbits>)
     5d6:	0002e0                                    	callx8	a2
     5d9:	090342                                    	l8ui	a4, a3, 9
     5dc:	120c                                    	movi.n	a2, 1
     5de:	c50c                                    	movi.n	a5, 12
     5e0:	14c1a2                                    	addi	a10, a1, 20
     5e3:	948c                                    	beqz.n	a4, 5f0 <aac_dec_decode+0xb0>
     5e5:	f01d                                    	retw.n

000005e7 <aac_dec_decode+0xa7>:
     5e7:	fea022                                    	movi	a2, 254
     5ea:	0004c6                                    	j	601 <aac_dec_decode+0xc1>

000005ed <aac_dec_decode+0xad>:
     5ed:	000406                                    	j	601 <aac_dec_decode+0xc1>

000005f0 <aac_dec_decode+0xb0>:
     5f0:	0d0642                                    	l8ui	a4, a6, 13
     5f3:	440b                                    	addi.n	a4, a4, -1
     5f5:	b44d                                    	ae_zext8	a4, a4
     5f7:	0d24b6                                    	bltui	a4, 2, 608 <aac_dec_decode+0xc8>
     5fa:	094352                                    	s8i	a5, a3, 9
     5fd:	f01d                                    	retw.n

000005ff <aac_dec_decode+0xbf>:
     5ff:	0359                                    	s32i.n	a5, a3, 0
     601:	094322                                    	s8i	a2, a3, 9
     604:	120c                                    	movi.n	a2, 1
     606:	f01d                                    	retw.n

00000608 <aac_dec_decode+0xc8>:
     608:	0007e0                                    	callx8	a7
     60b:	5a7b                                    	addi.n	a5, a10, 7
     60d:	080622                                    	l8ui	a2, a6, 8
     610:	090642                                    	l8ui	a4, a6, 9
     613:	415350                                    	srli	a5, a5, 3
     616:	104342                                    	s8i	a4, a3, 16
     619:	0706c2                                    	l8ui	a12, a6, 7
     61c:	040c                                    	movi.n	a4, 0
     61e:	0359                                    	s32i.n	a5, a3, 0
     620:	0b4642                                    	s8i	a4, a6, 11
     623:	07a2f6                                    	bgeui	a2, 12, 62e <aac_dec_decode+0xee>
     626:	fe8441                                    	l32r	a4, 38 (63e0 <sample_rates>)
     629:	a02240                                    	addx4	a2, a2, a4
     62c:	0248                                    	l32i.n	a4, a2, 0
     62e:	41d8                                    	l32i.n	a13, a1, 16
     630:	060622                                    	l8ui	a2, a6, 6
     633:	069152                                    	l16si	a5, a1, 12
     636:	3349                                    	s32i.n	a4, a3, 12
     638:	0843c2                                    	s8i	a12, a3, 8
     63b:	c1fcd0                                    	mul16u	a15, a12, a13
     63e:	13f9                                    	s32i.n	a15, a3, 4
     640:	728c                                    	beqz.n	a2, 64b <aac_dec_decode+0x10b>
     642:	052d                                    	mov.n	a2, a5
     644:	0345a6                                    	blti	a5, 4, 64b <aac_dec_decode+0x10b>
     647:	922d                                    	ae_zext16	a2, a2
     649:	0329                                    	s32i.n	a2, a3, 0
     64b:	120622                                    	l8ui	a2, a6, 18
     64e:	0212c7                                    	beq	a2, a12, 654 <aac_dec_decode+0x114>
     651:	1246c2                                    	s8i	a12, a6, 18
     654:	fe8421                                    	l32r	a2, 64 (66c <output_to_PCM>)
     657:	24c6b2                                    	addi	a11, a6, 36
     65a:	06ad                                    	mov.n	a10, a6
     65c:	0002e0                                    	callx8	a2
     65f:	0628                                    	l32i.n	a2, a6, 0
     661:	221b                                    	addi.n	a2, a2, 1
     663:	0629                                    	s32i.n	a2, a6, 0
     665:	020c                                    	movi.n	a2, 0
     667:	f01d                                    	retw.n

00000669 <aac_dec_decode+0x129>:
     669:	000000                                        ...

0000066c <output_to_PCM>:
     66c:	004136                                    	entry	a1, 32
     66f:	40d262                                    	addmi	a6, a2, 0x4000
     672:	550672                                    	l8ui	a7, a6, 85
     675:	560662                                    	l8ui	a6, a6, 86
     678:	a06630                                    	addx4	a6, a6, a3
     67b:	a07730                                    	addx4	a7, a7, a3
     67e:	0738                                    	l32i.n	a3, a7, 0
     680:	0668                                    	l32i.n	a6, a6, 0
     682:	217150                                    	srai	a7, a5, 1
     685:	4a25a6                                    	blti	a5, 2, 6d3 <output_to_PCM+0x67>
     688:	6298                                    	l32i.n	a9, a2, 24
     68a:	fcc382                                    	addi	a8, a3, -4
     68d:	fcc992                                    	addi	a9, a9, -4
     690:	3f8776                                    	loop	a7, 6d3 <output_to_PCM+0x67>
     693:	230720c000c5718e                  	{ ae_l32m.iu	aed0, a8, 4; nop }
     69b:	0e6d217b0d340c50078001381001418f 	{ ae_l32m.iu	aed1, a8, 4; nop; nop; ae_slaisq56s	aed0, aed0, 2 }
     6ab:	0ead217b09040cd0c58061380c01421f 	{ ae_slaisq56s	aed1, aed1, 2; nop; nop; ae_roundsq32f48sym	aed0, aed0 }
     6bb:	0ead217b0d2c0cd0c5fc61390801019f 	{ ae_s32m.iu	aed0, a9, 4; nop; nop; ae_roundsq32f48sym	aed31, aed1 }
     6cb:	230760c718c5b19e                  	{ ae_s32m.iu	aed31, a9, 4; nop }

000006d3 <output_to_PCM+0x67>:
     6d3:	6524a6                                    	blti	a4, 2, 73c <output_to_PCM+0xd0>
     6d6:	521367                                    	beq	a3, a6, 72c <output_to_PCM+0xc0>
     6d9:	5f25a6                                    	blti	a5, 2, 73c <output_to_PCM+0xd0>
     6dc:	072232                                    	l32i	a3, a2, 28
     6df:	fcc622                                    	addi	a2, a6, -4
     6e2:	fcc332                                    	addi	a3, a3, -4
     6e5:	3f8776                                    	loop	a7, 728 <output_to_PCM+0xbc>
     6e8:	230720c000c5712e                  	{ ae_l32m.iu	aed0, a2, 4; nop }
     6f0:	0e6d217b0d340c50078001381001412f 	{ ae_l32m.iu	aed1, a2, 4; nop; nop; ae_slaisq56s	aed0, aed0, 2 }
     700:	0ead217b09040cd0c58061380c01421f 	{ ae_slaisq56s	aed1, aed1, 2; nop; nop; ae_roundsq32f48sym	aed0, aed0 }
     710:	0ead217b0d2c0cd0c5fc61390801013f 	{ ae_s32m.iu	aed0, a3, 4; nop; nop; ae_roundsq32f48sym	aed31, aed1 }
     720:	230760c718c5b13e                  	{ ae_s32m.iu	aed31, a3, 4; nop }

00000728 <output_to_PCM+0xbc>:
     728:	000406                                    	j	73c <output_to_PCM+0xd0>

0000072b <output_to_PCM+0xbf>:
	...

0000072c <output_to_PCM+0xc0>:
     72c:	0c15a6                                    	blti	a5, 1, 73c <output_to_PCM+0xd0>
     72f:	7228                                    	l32i.n	a2, a2, 28
     731:	078576                                    	loop	a5, 73c <output_to_PCM+0xd0>
     734:	0348                                    	l32i.n	a4, a3, 0
     736:	334b                                    	addi.n	a3, a3, 4
     738:	0249                                    	s32i.n	a4, a2, 0
     73a:	224b                                    	addi.n	a2, a2, 4

0000073c <output_to_PCM+0xd0>:
     73c:	f01d                                    	retw.n

0000073e <output_to_PCM+0xd2>:
	...

00000740 <check_decode_SCE>:
     740:	004136                                    	entry	a1, 32
     743:	0d0242                                    	l8ui	a4, a2, 13
     746:	053c                                    	movi.n	a5, 48
     748:	0c0222                                    	l8ui	a2, a2, 12
     74b:	0424b6                                    	bltui	a4, 2, 753 <check_decode_SCE+0x13>
     74e:	c20c                                    	movi.n	a2, 12
     750:	000106                                    	j	758 <check_decode_SCE+0x18>

00000753 <check_decode_SCE+0x13>:
     753:	093257                                    	bltu	a2, a5, 760 <check_decode_SCE+0x20>
     756:	d20c                                    	movi.n	a2, 13
     758:	094322                                    	s8i	a2, a3, 9
     75b:	020c                                    	movi.n	a2, 0
     75d:	000046                                    	j	762 <check_decode_SCE+0x22>

00000760 <check_decode_SCE+0x20>:
     760:	120c                                    	movi.n	a2, 1
     762:	030430                                    	rsr.br	a3
     765:	413130                                    	srli	a3, a3, 1
     768:	042020                                    	extui	a2, a2, 0, 1
     76b:	903320                                    	addx2	a3, a3, a2
     76e:	130430                                    	wsr.br	a3
     771:	f01d                                    	retw.n

00000773 <check_decode_SCE+0x33>:
	...

00000774 <check_decode_CPE>:
     774:	004136                                    	entry	a1, 32
     777:	0d0242                                    	l8ui	a4, a2, 13
     77a:	053c                                    	movi.n	a5, 48
     77c:	0c0222                                    	l8ui	a2, a2, 12
     77f:	348c                                    	beqz.n	a4, 786 <check_decode_CPE+0x12>
     781:	c20c                                    	movi.n	a2, 12
     783:	000106                                    	j	78b <check_decode_CPE+0x17>

00000786 <check_decode_CPE+0x12>:
     786:	093257                                    	bltu	a2, a5, 793 <check_decode_CPE+0x1f>
     789:	d20c                                    	movi.n	a2, 13
     78b:	094322                                    	s8i	a2, a3, 9
     78e:	020c                                    	movi.n	a2, 0
     790:	000046                                    	j	795 <check_decode_CPE+0x21>

00000793 <check_decode_CPE+0x1f>:
     793:	120c                                    	movi.n	a2, 1
     795:	030430                                    	rsr.br	a3
     798:	413130                                    	srli	a3, a3, 1
     79b:	042020                                    	extui	a2, a2, 0, 1
     79e:	903320                                    	addx2	a3, a3, a2
     7a1:	130430                                    	wsr.br	a3
     7a4:	f01d                                    	retw.n

000007a6 <check_decode_CPE+0x32>:
	...

000007a8 <raw_data_block>:
     7a8:	00a136                                    	entry	a1, 80
     7ab:	a159                                    	s32i.n	a5, a1, 40
     7ad:	0f0c                                    	movi.n	a15, 0
     7af:	fe2c51                                    	l32r	a5, 60 (226c <aac_getbits>)
     7b2:	3b0c                                    	movi.n	a11, 3
     7b4:	04ad                                    	mov.n	a10, a4
     7b6:	6139                                    	s32i.n	a3, a1, 24
     7b8:	5129                                    	s32i.n	a2, a1, 20
     7ba:	060c                                    	movi.n	a6, 0
     7bc:	0652f2                                    	s16i	a15, a2, 12
     7bf:	047060                                    	extui	a7, a6, 0, 1
     7c2:	0005e0                                    	callx8	a5
     7c5:	ba2d                                    	ae_zext8	a2, a10
     7c7:	1c0c                                    	movi.n	a12, 1
     7c9:	097266                                    	bnei	a2, 7, 7d6 <raw_data_block+0x2e>

000007cc <raw_data_block+0x24>:
     7cc:	fe2221                                    	l32r	a2, 54 (22d0 <aac_byte_align>)
     7cf:	04ad                                    	mov.n	a10, a4
     7d1:	0002e0                                    	callx8	a2
     7d4:	f01d                                    	retw.n

000007d6 <raw_data_block+0x2e>:
     7d6:	030430                                    	rsr.br	a3
     7d9:	413130                                    	srli	a3, a3, 1
     7dc:	5e5c                                    	movi.n	a14, 85
     7de:	903370                                    	addx2	a3, a3, a7
     7e1:	5178                                    	l32i.n	a7, a1, 20
     7e3:	130430                                    	wsr.br	a3
     7e6:	030460                                    	rsr.br	a6
     7e9:	416160                                    	srli	a6, a6, 1
     7ec:	0430c0                                    	extui	a3, a12, 0, 1
     7ef:	fe1e81                                    	l32r	a8, 68 (22b4 <aac_get1bit>)
     7f2:	906630                                    	addx2	a6, a6, a3
     7f5:	0302de461cb0630e                  	{ ae_movab	a11, b0; addmi	a2, a14, 0x4000 }
     7fd:	40d7d2                                    	addmi	a13, a7, 0x4000
     800:	272a                                    	add.n	a2, a7, a2
     802:	130460                                    	wsr.br	a6
     805:	2906d8471ca9630e                  	{ ae_movab	a15, b0; or	a6, a8, a8 }
     80d:	0f41b2                                    	s8i	a11, a1, 15
     810:	81d9                                    	s32i.n	a13, a1, 32
     812:	0e41f2                                    	s8i	a15, a1, 14
     815:	9129                                    	s32i.n	a2, a1, 36
     817:	000446                                    	j	82c <raw_data_block+0x84>

0000081a <raw_data_block+0x72>:
     81a:	022266                                    	bnei	a2, 2, 820 <raw_data_block+0x78>
     81d:	00a8c6                                    	j	ac4 <raw_data_block+0x31c>
     820:	3b0c                                    	movi.n	a11, 3
     822:	04ad                                    	mov.n	a10, a4
     824:	0005e0                                    	callx8	a5
     827:	ba2d                                    	ae_zext8	a2, a10
     829:	9f7226                                    	beqi	a2, 7, 7cc <raw_data_block+0x24>
     82c:	ba2d                                    	ae_zext8	a2, a10
     82e:	4b0c                                    	movi.n	a11, 4
     830:	0242a6                                    	blti	a2, 4, 836 <raw_data_block+0x8e>
     833:	002486                                    	j	8c9 <raw_data_block+0x121>
     836:	3d0c                                    	movi.n	a13, 3
     838:	0222a6                                    	blti	a2, 2, 83e <raw_data_block+0x96>
     83b:	008106                                    	j	a43 <raw_data_block+0x29b>
     83e:	5138                                    	l32i.n	a3, a1, 20
     840:	208216                                    	beqz	a2, a4c <raw_data_block+0x2a4>
     843:	0f0c                                    	movi.n	a15, 0
     845:	d71266                                    	bnei	a2, 1, 820 <raw_data_block+0x78>
     848:	1041f2                                    	s8i	a15, a1, 16
     84b:	c20c                                    	movi.n	a2, 12
     84d:	0d0362                                    	l8ui	a6, a3, 13
     850:	5188                                    	l32i.n	a8, a1, 20
     852:	0e0172                                    	l8ui	a7, a1, 14
     855:	3f04404404a0727e                  	{ ae_movba	b0, a7; movi	a15, 48 }
     85d:	66cc                                    	bnez.n	a6, 867 <raw_data_block+0xbf>
     85f:	0c0832                                    	l8ui	a3, a8, 12
     862:	d20c                                    	movi.n	a2, 13
     864:	0f33f7                                    	bltu	a3, a15, 877 <raw_data_block+0xcf>
     867:	6138                                    	l32i.n	a3, a1, 24
     869:	0f01f2                                    	l8ui	a15, a1, 15
     86c:	230740c404a572fe                  	{ ae_movba	b0, a15; nop }
     874:	094322                                    	s8i	a2, a3, 9
     877:	460076                                    	bf	b0, 8c1 <raw_data_block+0x119>
     87a:	5138                                    	l32i.n	a3, a1, 20
     87c:	270c                                    	movi.n	a7, 2
     87e:	0c0322                                    	l8ui	a2, a3, 12
     881:	232a                                    	add.n	a2, a3, a2
     883:	0e0232                                    	l8ui	a3, a2, 14
     886:	052326                                    	beqi	a3, 2, 88f <raw_data_block+0xe7>
     889:	240356                                    	bnez	a3, acd <raw_data_block+0x325>
     88c:	0e4272                                    	s8i	a7, a2, 14
     88f:	5178                                    	l32i.n	a7, a1, 20
     891:	40d222                                    	addmi	a2, a2, 0x4000
     894:	130c                                    	movi.n	a3, 1
     896:	10c1d2                                    	addi	a13, a1, 16
     899:	534232                                    	s8i	a3, a2, 83
     89c:	04bd                                    	mov.n	a11, a4
     89e:	fdf321                                    	l32r	a2, 6c (e04 <channel_pair_element>)
     8a1:	07ad                                    	mov.n	a10, a7
     8a3:	06cd                                    	mov.n	a12, a6
     8a5:	0002e0                                    	callx8	a2
     8a8:	9188                                    	l32i.n	a8, a1, 36
     8aa:	d61b                                    	addi.n	a13, a6, 1
     8ac:	6138                                    	l32i.n	a3, a1, 24
     8ae:	00a2f2                                    	movi	a15, 0x200
     8b1:	286a                                    	add.n	a2, a8, a6
     8b3:	e8da                                    	add.n	a14, a8, a13
     8b5:	0943a2                                    	s8i	a10, a3, 9
     8b8:	004262                                    	s8i	a6, a2, 0
     8bb:	004ed2                                    	s8i	a13, a14, 0
     8be:	0657f2                                    	s16i	a15, a7, 12
     8c1:	6128                                    	l32i.n	a2, a1, 24
     8c3:	fde961                                    	l32r	a6, 68 (22b4 <aac_get1bit>)
     8c6:	006486                                    	j	a5c <raw_data_block+0x2b4>

000008c9 <raw_data_block+0x121>:
     8c9:	04ad                                    	mov.n	a10, a4
     8cb:	024266                                    	bnei	a2, 4, 8d1 <raw_data_block+0x129>
     8ce:	0064c6                                    	j	a65 <raw_data_block+0x2bd>
     8d1:	f30c                                    	movi.n	a3, 15
     8d3:	025266                                    	bnei	a2, 5, 8d9 <raw_data_block+0x131>
     8d6:	006b86                                    	j	a88 <raw_data_block+0x2e0>
     8d9:	026226                                    	beqi	a2, 6, 8df <raw_data_block+0x137>
     8dc:	ffd006                                    	j	820 <raw_data_block+0x78>
     8df:	0005e0                                    	callx8	a5
     8e2:	0a7d                                    	mov.n	a7, a10
     8e4:	089a37                                    	bne	a10, a3, 8f0 <raw_data_block+0x148>
     8e7:	8b0c                                    	movi.n	a11, 8
     8e9:	04ad                                    	mov.n	a10, a4
     8eb:	0005e0                                    	callx8	a5
     8ee:	7aeb                                    	addi.n	a7, a10, 14
     8f0:	4b0c                                    	movi.n	a11, 4
     8f2:	0217e6                                    	bgei	a7, 1, 8f8 <raw_data_block+0x150>
     8f5:	006706                                    	j	a95 <raw_data_block+0x2ed>
     8f8:	04ad                                    	mov.n	a10, a4
     8fa:	b20c                                    	movi.n	a2, 11
     8fc:	0005e0                                    	callx8	a5
     8ff:	4b0c                                    	movi.n	a11, 4
     901:	021a66                                    	bnei	a10, 1, 907 <raw_data_block+0x15f>
     904:	002e46                                    	j	9c1 <raw_data_block+0x219>
     907:	7179                                    	s32i.n	a7, a1, 28
     909:	130c                                    	movi.n	a3, 1
     90b:	ffa072                                    	movi	a7, 255
     90e:	022a66                                    	bnei	a10, 2, 914 <raw_data_block+0x16c>
     911:	003246                                    	j	9de <raw_data_block+0x236>
     914:	021a27                                    	beq	a10, a2, 91a <raw_data_block+0x172>
     917:	003ec6                                    	j	a16 <raw_data_block+0x26e>
     91a:	04ad                                    	mov.n	a10, a4
     91c:	0006e0                                    	callx8	a6
     91f:	0f6a07                                    	bbci	a10, 0, 932 <raw_data_block+0x18a>
     922:	4b0c                                    	movi.n	a11, 4
     924:	04ad                                    	mov.n	a10, a4
     926:	0005e0                                    	callx8	a5
     929:	4b0c                                    	movi.n	a11, 4
     92b:	04ad                                    	mov.n	a10, a4
     92d:	0005e0                                    	callx8	a5
     930:	230c                                    	movi.n	a3, 2
     932:	04ad                                    	mov.n	a10, a4
     934:	0006e0                                    	callx8	a6
     937:	ba2d                                    	ae_zext8	a2, a10
     939:	7b0c                                    	movi.n	a11, 7
     93b:	281266                                    	bnei	a2, 1, 967 <raw_data_block+0x1bf>
     93e:	04ad                                    	mov.n	a10, a4
     940:	0005e0                                    	callx8	a5
     943:	04ad                                    	mov.n	a10, a4
     945:	0006e0                                    	callx8	a6
     948:	ba2d                                    	ae_zext8	a2, a10
     94a:	021226                                    	beqi	a2, 1, 950 <raw_data_block+0x1a8>
     94d:	003b46                                    	j	a3e <raw_data_block+0x296>
     950:	120c                                    	movi.n	a2, 1
     952:	7b0c                                    	movi.n	a11, 7
     954:	04ad                                    	mov.n	a10, a4
     956:	0005e0                                    	callx8	a5
     959:	04ad                                    	mov.n	a10, a4
     95b:	221b                                    	addi.n	a2, a2, 1
     95d:	0006e0                                    	callx8	a6
     960:	ba7d                                    	ae_zext8	a7, a10
     962:	ec1726                                    	beqi	a7, 1, 952 <raw_data_block+0x1aa>
     965:	323a                                    	add.n	a3, a2, a3
     967:	04ad                                    	mov.n	a10, a4
     969:	170c                                    	movi.n	a7, 1
     96b:	0006e0                                    	callx8	a6
     96e:	276a07                                    	bbci	a10, 0, 999 <raw_data_block+0x1f1>
     971:	4b0c                                    	movi.n	a11, 4
     973:	04ad                                    	mov.n	a10, a4
     975:	0005e0                                    	callx8	a5
     978:	b1a9                                    	s32i.n	a10, a1, 44
     97a:	4b0c                                    	movi.n	a11, 4
     97c:	ba2d                                    	ae_zext8	a2, a10
     97e:	04ad                                    	mov.n	a10, a4
     980:	721b                                    	addi.n	a7, a2, 1
     982:	0005e0                                    	callx8	a5
     985:	072d                                    	mov.n	a2, a7
     987:	8b0c                                    	movi.n	a11, 8
     989:	04ad                                    	mov.n	a10, a4
     98b:	0005e0                                    	callx8	a5
     98e:	220b                                    	addi.n	a2, a2, -1
     990:	ff3256                                    	bnez	a2, 987 <raw_data_block+0x1df>
     993:	b128                                    	l32i.n	a2, a1, 44
     995:	232a                                    	add.n	a2, a3, a2
     997:	322b                                    	addi.n	a3, a2, 2
     999:	04ad                                    	mov.n	a10, a4
     99b:	0006e0                                    	callx8	a6
     99e:	086a07                                    	bbci	a10, 0, 9aa <raw_data_block+0x202>
     9a1:	8b0c                                    	movi.n	a11, 8
     9a3:	04ad                                    	mov.n	a10, a4
     9a5:	0005e0                                    	callx8	a5
     9a8:	331b                                    	addi.n	a3, a3, 1
     9aa:	072d                                    	mov.n	a2, a7
     9ac:	8b0c                                    	movi.n	a11, 8
     9ae:	04ad                                    	mov.n	a10, a4
     9b0:	0005e0                                    	callx8	a5
     9b3:	220b                                    	addi.n	a2, a2, -1
     9b5:	ff3256                                    	bnez	a2, 9ac <raw_data_block+0x204>
     9b8:	237a                                    	add.n	a2, a3, a7
     9ba:	7178                                    	l32i.n	a7, a1, 28
     9bc:	b22d                                    	ae_zext8	a2, a2
     9be:	001b86                                    	j	a30 <raw_data_block+0x288>

000009c1 <raw_data_block+0x219>:
     9c1:	4b0c                                    	movi.n	a11, 4
     9c3:	04ad                                    	mov.n	a10, a4
     9c5:	0005e0                                    	callx8	a5
     9c8:	972d                                    	ae_zext16	a2, a7
     9ca:	320b                                    	addi.n	a3, a2, -1
     9cc:	6022b6                                    	bltui	a2, 2, a30 <raw_data_block+0x288>
     9cf:	8b0c                                    	movi.n	a11, 8
     9d1:	04ad                                    	mov.n	a10, a4
     9d3:	0005e0                                    	callx8	a5
     9d6:	330b                                    	addi.n	a3, a3, -1
     9d8:	ff3356                                    	bnez	a3, 9cf <raw_data_block+0x227>
     9db:	001446                                    	j	a30 <raw_data_block+0x288>

000009de <raw_data_block+0x236>:
     9de:	4b0c                                    	movi.n	a11, 4
     9e0:	04ad                                    	mov.n	a10, a4
     9e2:	0005e0                                    	callx8	a5
     9e5:	080c                                    	movi.n	a8, 0
     9e7:	3087a7                                    	bany	a7, a10, a1b <raw_data_block+0x273>
     9ea:	120c                                    	movi.n	a2, 1
     9ec:	ffa062                                    	movi	a6, 255
     9ef:	8b0c                                    	movi.n	a11, 8
     9f1:	04ad                                    	mov.n	a10, a4
     9f3:	083d                                    	mov.n	a3, a8
     9f5:	221b                                    	addi.n	a2, a2, 1
     9f7:	0005e0                                    	callx8	a5
     9fa:	ba7d                                    	ae_zext8	a7, a10
     9fc:	873a                                    	add.n	a8, a7, a3
     9fe:	ed4a67                                    	ball	a10, a6, 9ef <raw_data_block+0x247>
     a01:	8b0c                                    	movi.n	a11, 8
     a03:	fd9961                                    	l32r	a6, 68 (22b4 <aac_get1bit>)
     a06:	04ad                                    	mov.n	a10, a4
     a08:	f88c                                    	beqz.n	a8, a1b <raw_data_block+0x273>
     a0a:	0005e0                                    	callx8	a5
     a0d:	337a                                    	add.n	a3, a3, a7
     a0f:	7178                                    	l32i.n	a7, a1, 28
     a11:	232a                                    	add.n	a2, a3, a2
     a13:	000646                                    	j	a30 <raw_data_block+0x288>

00000a16 <raw_data_block+0x26e>:
     a16:	04ad                                    	mov.n	a10, a4
     a18:	0005e0                                    	callx8	a5
     a1b:	7178                                    	l32i.n	a7, a1, 28
     a1d:	972d                                    	ae_zext16	a2, a7
     a1f:	320b                                    	addi.n	a3, a2, -1
     a21:	0b22b6                                    	bltui	a2, 2, a30 <raw_data_block+0x288>
     a24:	8b0c                                    	movi.n	a11, 8
     a26:	04ad                                    	mov.n	a10, a4
     a28:	0005e0                                    	callx8	a5
     a2b:	330b                                    	addi.n	a3, a3, -1
     a2d:	ff3356                                    	bnez	a3, a24 <raw_data_block+0x27c>
     a30:	c07720                                    	sub	a7, a7, a2
     a33:	4b0c                                    	movi.n	a11, 4
     a35:	0217a6                                    	blti	a7, 1, a3b <raw_data_block+0x293>
     a38:	ffaf06                                    	j	8f8 <raw_data_block+0x150>
     a3b:	001586                                    	j	a95 <raw_data_block+0x2ed>

00000a3e <raw_data_block+0x296>:
     a3e:	120c                                    	movi.n	a2, 1
     a40:	ffc846                                    	j	965 <raw_data_block+0x1bd>

00000a43 <raw_data_block+0x29b>:
     a43:	023226                                    	beqi	a2, 3, a49 <raw_data_block+0x2a1>
     a46:	ff7406                                    	j	81a <raw_data_block+0x72>
     a49:	000046                                    	j	a4e <raw_data_block+0x2a6>

00000a4c <raw_data_block+0x2a4>:
     a4c:	0d0c                                    	movi.n	a13, 0
     a4e:	6128                                    	l32i.n	a2, a1, 24
     a50:	04cd                                    	mov.n	a12, a4
     a52:	51a8                                    	l32i.n	a10, a1, 20
     a54:	fd8731                                    	l32r	a3, 70 (ae8 <decode_SCE>)
     a57:	02bd                                    	mov.n	a11, a2
     a59:	0003e0                                    	callx8	a3
     a5c:	090222                                    	l8ui	a2, a2, 9
     a5f:	dbd216                                    	beqz	a2, 820 <raw_data_block+0x78>
     a62:	ff5b86                                    	j	7d4 <raw_data_block+0x2c>

00000a65 <raw_data_block+0x2bd>:
     a65:	db0c                                    	movi.n	a11, 13
     a67:	04ad                                    	mov.n	a10, a4
     a69:	0005e0                                    	callx8	a5
     a6c:	ffa022                                    	movi	a2, 255
     a6f:	0a6d                                    	mov.n	a6, a10
     a71:	8b0c                                    	movi.n	a11, 8
     a73:	28ca27                                    	bnall	a10, a2, a9f <raw_data_block+0x2f7>
     a76:	04ad                                    	mov.n	a10, a4
     a78:	ffa032                                    	movi	a3, 255
     a7b:	0005e0                                    	callx8	a5
     a7e:	9a2d                                    	ae_zext16	a2, a10
     a80:	223a                                    	add.n	a2, a2, a3
     a82:	1ee687                                    	bbsi	a6, 8, aa4 <raw_data_block+0x2fc>
     a85:	0008c6                                    	j	aac <raw_data_block+0x304>

00000a88 <raw_data_block+0x2e0>:
     a88:	a1a8                                    	l32i.n	a10, a1, 40
     a8a:	04bd                                    	mov.n	a11, a4
     a8c:	fd7a21                                    	l32r	a2, 74 (c50 <loas_program_config_element>)
     a8f:	0002e0                                    	callx8	a2
     a92:	ff6286                                    	j	820 <raw_data_block+0x78>

00000a95 <raw_data_block+0x2ed>:
     a95:	6128                                    	l32i.n	a2, a1, 24
     a97:	030c                                    	movi.n	a3, 0
     a99:	094232                                    	s8i	a3, a2, 9
     a9c:	ff6006                                    	j	820 <raw_data_block+0x78>

00000a9f <raw_data_block+0x2f7>:
     a9f:	b62d                                    	ae_zext8	a2, a6
     aa1:	076687                                    	bbci	a6, 8, aac <raw_data_block+0x304>
     aa4:	fd6c31                                    	l32r	a3, 54 (22d0 <aac_byte_align>)
     aa7:	04ad                                    	mov.n	a10, a4
     aa9:	0003e0                                    	callx8	a3
     aac:	fd6f61                                    	l32r	a6, 68 (22b4 <aac_get1bit>)
     aaf:	0212e6                                    	bgei	a2, 1, ab5 <raw_data_block+0x30d>
     ab2:	ff5a86                                    	j	820 <raw_data_block+0x78>
     ab5:	8b0c                                    	movi.n	a11, 8
     ab7:	04ad                                    	mov.n	a10, a4
     ab9:	0005e0                                    	callx8	a5
     abc:	220b                                    	addi.n	a2, a2, -1
     abe:	ff3256                                    	bnez	a2, ab5 <raw_data_block+0x30d>
     ac1:	ff56c6                                    	j	820 <raw_data_block+0x78>

00000ac4 <raw_data_block+0x31c>:
     ac4:	6138                                    	l32i.n	a3, a1, 24
     ac6:	620c                                    	movi.n	a2, 6
     ac8:	094322                                    	s8i	a2, a3, 9
     acb:	f01d                                    	retw.n

00000acd <raw_data_block+0x325>:
     acd:	8138                                    	l32i.n	a3, a1, 32
     acf:	020c                                    	movi.n	a2, 0
     ad1:	5148                                    	l32i.n	a4, a1, 20
     ad3:	5e1c                                    	movi.n	a14, 21
     ad5:	61f8                                    	l32i.n	a15, a1, 24
     ad7:	544322                                    	s8i	a2, a3, 84
     ada:	534322                                    	s8i	a2, a3, 83
     add:	075422                                    	s16i	a2, a4, 14
     ae0:	094fe2                                    	s8i	a14, a15, 9
     ae3:	f01d                                    	retw.n

00000ae5 <raw_data_block+0x33d>:
     ae5:	000000                                        ...

00000ae8 <decode_SCE>:
     ae8:	008136                                    	entry	a1, 64
     aeb:	05ad                                    	mov.n	a10, a5
     aed:	0d0252                                    	l8ui	a5, a2, 13
     af0:	0c0282                                    	l8ui	a8, a2, 12
     af3:	073c                                    	movi.n	a7, 48
     af5:	d90c                                    	movi.n	a9, 13
     af7:	0425b6                                    	bltui	a5, 2, aff <decode_SCE+0x17>
     afa:	c90c                                    	movi.n	a9, 12
     afc:	000086                                    	j	b02 <decode_SCE+0x1a>

00000aff <decode_SCE+0x17>:
     aff:	073877                                    	bltu	a8, a7, b0a <decode_SCE+0x22>
     b02:	094392                                    	s8i	a9, a3, 9
     b05:	070c                                    	movi.n	a7, 0
     b07:	000046                                    	j	b0c <decode_SCE+0x24>

00000b0a <decode_SCE+0x22>:
     b0a:	170c                                    	movi.n	a7, 1
     b0c:	030490                                    	rsr.br	a9
     b0f:	419190                                    	srli	a9, a9, 1
     b12:	047070                                    	extui	a7, a7, 0, 1
     b15:	909970                                    	addx2	a9, a9, a7
     b18:	b4a072                                    	movi	a7, 180
     b1b:	130490                                    	wsr.br	a9
     b1e:	021076                                    	bt	b0, b24 <decode_SCE+0x3c>
     b21:	004a46                                    	j	c4e <decode_SCE+0x166>
     b24:	e28a                                    	add.n	a14, a2, a8
     b26:	40d772                                    	addmi	a7, a7, 0x4000
     b29:	7189                                    	s32i.n	a8, a1, 28
     b2b:	40de82                                    	addmi	a8, a14, 0x4000
     b2e:	3139                                    	s32i.n	a3, a1, 12
     b30:	727a                                    	add.n	a7, a2, a7
     b32:	5348a2                                    	s8i	a10, a8, 83
     b35:	40c232                                    	addi	a3, a2, 64
     b38:	fd5081                                    	l32r	a8, 78 (388c <xt_memset8_0>)
     b3b:	f78b                                    	addi.n	a15, a7, 8
     b3d:	00a2b2                                    	movi	a11, 0x200
     b40:	03ad                                    	mov.n	a10, a3
     b42:	61e9                                    	s32i.n	a14, a1, 24
     b44:	40d262                                    	addmi	a6, a2, 0x4000
     b47:	51f9                                    	s32i.n	a15, a1, 20
     b49:	0008e0                                    	callx8	a8
     b4c:	fd4581                                    	l32r	a8, 60 (226c <aac_getbits>)
     b4f:	4b0c                                    	movi.n	a11, 4
     b51:	04ad                                    	mov.n	a10, a4
     b53:	0008e0                                    	callx8	a8
     b56:	f87c                                    	movi.n	a8, -1
     b58:	2159                                    	s32i.n	a5, a1, 8
     b5a:	b44652                                    	s8i	a5, a6, 180
     b5d:	5158                                    	l32i.n	a5, a1, 20
     b5f:	f48080                                    	extui	a8, a8, 0, 16
     b62:	b846a2                                    	s8i	a10, a6, 184
     b65:	5b5682                                    	s16i	a8, a6, 182
     b68:	0e0c                                    	movi.n	a14, 0
     b6a:	fd4481                                    	l32r	a8, 7c (1284 <one_channel_stream>)
     b6d:	02ad                                    	mov.n	a10, a2
     b6f:	07bd                                    	mov.n	a11, a7
     b71:	04cd                                    	mov.n	a12, a4
     b73:	05dd                                    	mov.n	a13, a5
     b75:	03fd                                    	mov.n	a15, a3
     b77:	0008e0                                    	callx8	a8
     b7a:	3188                                    	l32i.n	a8, a1, 12
     b7c:	ffa072                                    	movi	a7, 255
     b7f:	2198                                    	l32i.n	a9, a1, 8
     b81:	0807a7                                    	bnone	a7, a10, b8d <decode_SCE+0xa5>
     b84:	0a4d                                    	mov.n	a4, a10
     b86:	083d                                    	mov.n	a3, a8
     b88:	096d                                    	mov.n	a6, a9
     b8a:	002186                                    	j	c14 <decode_SCE+0x12c>

00000b8d <decode_SCE+0xa5>:
     b8d:	0c0242                                    	l8ui	a4, a2, 12
     b90:	4169                                    	s32i.n	a6, a1, 16
     b92:	1139                                    	s32i.n	a3, a1, 4
     b94:	1a4c                                    	movi.n	a10, 65
     b96:	424a                                    	add.n	a4, a2, a4
     b98:	81a092                                    	movi	a9, 129
     b9b:	0e0472                                    	l8ui	a7, a4, 14
     b9e:	180c                                    	movi.n	a8, 1
     ba0:	1199a0                                    	slli	a9, a9, 6
     ba3:	f29a                                    	add.n	a15, a2, a9
     ba5:	113aa0                                    	slli	a3, a10, 6
     ba8:	17dc                                    	bnez.n	a7, bbd <decode_SCE+0xd5>
     baa:	723a                                    	add.n	a7, a2, a3
     bac:	0e4482                                    	s8i	a8, a4, 14
     baf:	62a8                                    	l32i.n	a10, a2, 24
     bb1:	a279                                    	s32i.n	a7, a2, 40
     bb3:	92a9                                    	s32i.n	a10, a2, 36
     bb5:	b2f9                                    	s32i.n	a15, a2, 44
     bb7:	0e0472                                    	l8ui	a7, a4, 14
     bba:	104482                                    	s8i	a8, a4, 16
     bbd:	2168                                    	l32i.n	a6, a1, 8
     bbf:	041726                                    	beqi	a7, 1, bc7 <decode_SCE+0xdf>
     bc2:	170c                                    	movi.n	a7, 1
     bc4:	0e4472                                    	s8i	a7, a4, 14
     bc7:	1138                                    	l32i.n	a3, a1, 4
     bc9:	45d242                                    	addmi	a4, a2, 0x4500
     bcc:	770442                                    	l8ui	a4, a4, 119
     bcf:	88a072                                    	movi	a7, 136
     bd2:	05ad                                    	mov.n	a10, a5
     bd4:	03ed                                    	mov.n	a14, a3
     bd6:	45d782                                    	addmi	a8, a7, 0x4500
     bd9:	149c                                    	beqz.n	a4, bee <decode_SCE+0x106>
     bdb:	0802c2                                    	l8ui	a12, a2, 8
     bde:	b28a                                    	add.n	a11, a2, a8
     be0:	0902d2                                    	l8ui	a13, a2, 9
     be3:	0212f2                                    	l16ui	a15, a2, 4
     be6:	fd2641                                    	l32r	a4, 80 (1e6c <tns_decode>)
     be9:	0004e0                                    	callx8	a4
     bec:	1138                                    	l32i.n	a3, a1, 4
     bee:	40d252                                    	addmi	a5, a2, 0x4000
     bf1:	fd2471                                    	l32r	a7, 84 (2b50 <filter_bank_LC>)
     bf4:	bd05b2                                    	l8ui	a11, a5, 189
     bf7:	0e0c                                    	movi.n	a14, 0
     bf9:	be05c2                                    	l8ui	a12, a5, 190
     bfc:	02ad                                    	mov.n	a10, a2
     bfe:	03dd                                    	mov.n	a13, a3
     c00:	040c                                    	movi.n	a4, 0
     c02:	0007e0                                    	callx8	a7
     c05:	4138                                    	l32i.n	a3, a1, 16
     c07:	be0552                                    	l8ui	a5, a5, 190
     c0a:	b40332                                    	l8ui	a3, a3, 180
     c0d:	323a                                    	add.n	a3, a2, a3
     c0f:	134352                                    	s8i	a5, a3, 19
     c12:	3138                                    	l32i.n	a3, a1, 12
     c14:	6178                                    	l32i.n	a7, a1, 24
     c16:	094342                                    	s8i	a4, a3, 9
     c19:	f26a                                    	add.n	a15, a2, a6
     c1b:	7158                                    	l32i.n	a5, a1, 28
     c1d:	40df32                                    	addmi	a3, a15, 0x4000
     c20:	861b                                    	addi.n	a8, a6, 1
     c22:	0e0742                                    	l8ui	a4, a7, 14
     c25:	554362                                    	s8i	a6, a3, 85
     c28:	328a                                    	add.n	a3, a2, a8
     c2a:	052466                                    	bnei	a4, 2, c33 <decode_SCE+0x14b>
     c2d:	40d342                                    	addmi	a4, a3, 0x4000
     c30:	554482                                    	s8i	a8, a4, 85
     c33:	0e0732                                    	l8ui	a3, a7, 14
     c36:	451b                                    	addi.n	a4, a5, 1
     c38:	1e0c                                    	movi.n	a14, 1
     c3a:	2f0c                                    	movi.n	a15, 2
     c3c:	b44d                                    	ae_zext8	a4, a4
     c3e:	336a                                    	add.n	a3, a3, a6
     c40:	4344e0                                    	min	a4, a4, a14
     c43:	b33d                                    	ae_zext8	a3, a3
     c45:	4333f0                                    	min	a3, a3, a15
     c48:	0c4242                                    	s8i	a4, a2, 12
     c4b:	0d4232                                    	s8i	a3, a2, 13
     c4e:	f01d                                    	retw.n

00000c50 <loas_program_config_element>:
     c50:	004136                                    	entry	a1, 32
     c53:	fcf741                                    	l32r	a4, 30 (3878 <memset>)
     c56:	0b0c                                    	movi.n	a11, 0
     c58:	2c1c                                    	movi.n	a12, 18
     c5a:	02ad                                    	mov.n	a10, a2
     c5c:	0004e0                                    	callx8	a4
     c5f:	fd0051                                    	l32r	a5, 60 (226c <aac_getbits>)
     c62:	8b1c                                    	movi.n	a11, 24
     c64:	03ad                                    	mov.n	a10, a3
     c66:	0005e0                                    	callx8	a5
     c69:	15b2a0                                    	extui	a11, a10, 18, 2
     c6c:	3144a0                                    	srai	a4, a10, 20
     c6f:	0142b2                                    	s8i	a11, a2, 1
     c72:	34cea0                                    	extui	a12, a10, 14, 4
     c75:	34daa0                                    	extui	a13, a10, 10, 4
     c78:	34e6a0                                    	extui	a14, a10, 6, 4
     c7b:	34f2a0                                    	extui	a15, a10, 2, 4
     c7e:	1460a0                                    	extui	a6, a10, 0, 2
     c81:	8b0c                                    	movi.n	a11, 8
     c83:	03ad                                    	mov.n	a10, a3
     c85:	004242                                    	s8i	a4, a2, 0
     c88:	0242c2                                    	s8i	a12, a2, 2
     c8b:	0442d2                                    	s8i	a13, a2, 4
     c8e:	0542e2                                    	s8i	a14, a2, 5
     c91:	0642f2                                    	s8i	a15, a2, 6
     c94:	074262                                    	s8i	a6, a2, 7
     c97:	0005e0                                    	callx8	a5
     c9a:	4145a0                                    	srli	a4, a10, 5
     c9d:	34e1a0                                    	extui	a14, a10, 1, 4
     ca0:	04f0a0                                    	extui	a15, a10, 0, 1
     ca3:	084242                                    	s8i	a4, a2, 8
     ca6:	0942e2                                    	s8i	a14, a2, 9
     ca9:	4b0c                                    	movi.n	a11, 4
     cab:	0a42f2                                    	s8i	a15, a2, 10
     cae:	046a07                                    	bbci	a10, 0, cb6 <loas_program_config_element+0x66>
     cb1:	03ad                                    	mov.n	a10, a3
     cb3:	0005e0                                    	callx8	a5

00000cb6 <loas_program_config_element+0x66>:
     cb6:	fcec41                                    	l32r	a4, 68 (22b4 <aac_get1bit>)
     cb9:	03ad                                    	mov.n	a10, a3
     cbb:	0004e0                                    	callx8	a4
     cbe:	ba6d                                    	ae_zext8	a6, a10
     cc0:	0b42a2                                    	s8i	a10, a2, 11
     cc3:	4b0c                                    	movi.n	a11, 4
     cc5:	03ad                                    	mov.n	a10, a3
     cc7:	021666                                    	bnei	a6, 1, ccd <loas_program_config_element+0x7d>
     cca:	0005e0                                    	callx8	a5
     ccd:	03ad                                    	mov.n	a10, a3
     ccf:	0004e0                                    	callx8	a4
     cd2:	ba4d                                    	ae_zext8	a4, a10
     cd4:	0c42a2                                    	s8i	a10, a2, 12
     cd7:	3b0c                                    	movi.n	a11, 3
     cd9:	03ad                                    	mov.n	a10, a3
     cdb:	021466                                    	bnei	a4, 1, ce1 <loas_program_config_element+0x91>
     cde:	0005e0                                    	callx8	a5
     ce1:	040242                                    	l8ui	a4, a2, 4
     ce4:	260c                                    	movi.n	a6, 2
     ce6:	74ac                                    	beqz.n	a4, d11 <loas_program_config_element+0xc1>
     ce8:	040c                                    	movi.n	a4, 0
     cea:	5b0c                                    	movi.n	a11, 5
     cec:	03ad                                    	mov.n	a10, a3
     cee:	0005e0                                    	callx8	a5
     cf1:	0e0282                                    	l8ui	a8, a2, 14
     cf4:	170c                                    	movi.n	a7, 1
     cf6:	030292                                    	l8ui	a9, a2, 3
     cf9:	04a4a0                                    	extui	a10, a10, 4, 1
     cfc:	9376a0                                    	movnez	a7, a6, a10
     cff:	0402f2                                    	l8ui	a15, a2, 4
     d02:	878a                                    	add.n	a8, a7, a8
     d04:	441b                                    	addi.n	a4, a4, 1
     d06:	797a                                    	add.n	a7, a9, a7
     d08:	0e4282                                    	s8i	a8, a2, 14
     d0b:	034272                                    	s8i	a7, a2, 3
     d0e:	d834f7                                    	bltu	a4, a15, cea <loas_program_config_element+0x9a>
     d11:	050242                                    	l8ui	a4, a2, 5
     d14:	260c                                    	movi.n	a6, 2
     d16:	74ac                                    	beqz.n	a4, d41 <loas_program_config_element+0xf1>
     d18:	040c                                    	movi.n	a4, 0
     d1a:	5b0c                                    	movi.n	a11, 5
     d1c:	03ad                                    	mov.n	a10, a3
     d1e:	0005e0                                    	callx8	a5
     d21:	0f0282                                    	l8ui	a8, a2, 15
     d24:	170c                                    	movi.n	a7, 1
     d26:	030292                                    	l8ui	a9, a2, 3
     d29:	04a4a0                                    	extui	a10, a10, 4, 1
     d2c:	9376a0                                    	movnez	a7, a6, a10
     d2f:	0502f2                                    	l8ui	a15, a2, 5
     d32:	878a                                    	add.n	a8, a7, a8
     d34:	441b                                    	addi.n	a4, a4, 1
     d36:	797a                                    	add.n	a7, a9, a7
     d38:	0f4282                                    	s8i	a8, a2, 15
     d3b:	034272                                    	s8i	a7, a2, 3
     d3e:	d834f7                                    	bltu	a4, a15, d1a <loas_program_config_element+0xca>
     d41:	060242                                    	l8ui	a4, a2, 6
     d44:	260c                                    	movi.n	a6, 2
     d46:	74ac                                    	beqz.n	a4, d71 <loas_program_config_element+0x121>
     d48:	040c                                    	movi.n	a4, 0
     d4a:	5b0c                                    	movi.n	a11, 5
     d4c:	03ad                                    	mov.n	a10, a3
     d4e:	0005e0                                    	callx8	a5
     d51:	100282                                    	l8ui	a8, a2, 16
     d54:	170c                                    	movi.n	a7, 1
     d56:	030292                                    	l8ui	a9, a2, 3
     d59:	04a4a0                                    	extui	a10, a10, 4, 1
     d5c:	9376a0                                    	movnez	a7, a6, a10
     d5f:	0602f2                                    	l8ui	a15, a2, 6
     d62:	878a                                    	add.n	a8, a7, a8
     d64:	441b                                    	addi.n	a4, a4, 1
     d66:	797a                                    	add.n	a7, a9, a7
     d68:	104282                                    	s8i	a8, a2, 16
     d6b:	034272                                    	s8i	a7, a2, 3
     d6e:	d834f7                                    	bltu	a4, a15, d4a <loas_program_config_element+0xfa>
     d71:	070242                                    	l8ui	a4, a2, 7
     d74:	f49c                                    	beqz.n	a4, d97 <loas_program_config_element+0x147>
     d76:	040c                                    	movi.n	a4, 0
     d78:	4b0c                                    	movi.n	a11, 4
     d7a:	03ad                                    	mov.n	a10, a3
     d7c:	0005e0                                    	callx8	a5
     d7f:	110262                                    	l8ui	a6, a2, 17
     d82:	441b                                    	addi.n	a4, a4, 1
     d84:	030272                                    	l8ui	a7, a2, 3
     d87:	070282                                    	l8ui	a8, a2, 7
     d8a:	661b                                    	addi.n	a6, a6, 1
     d8c:	f71b                                    	addi.n	a15, a7, 1
     d8e:	114262                                    	s8i	a6, a2, 17
     d91:	0342f2                                    	s8i	a15, a2, 3
     d94:	e03487                                    	bltu	a4, a8, d78 <loas_program_config_element+0x128>
     d97:	080242                                    	l8ui	a4, a2, 8
     d9a:	f48c                                    	beqz.n	a4, dad <loas_program_config_element+0x15d>
     d9c:	040c                                    	movi.n	a4, 0
     d9e:	4b0c                                    	movi.n	a11, 4
     da0:	03ad                                    	mov.n	a10, a3
     da2:	0005e0                                    	callx8	a5
     da5:	080262                                    	l8ui	a6, a2, 8
     da8:	441b                                    	addi.n	a4, a4, 1
     daa:	f03467                                    	bltu	a4, a6, d9e <loas_program_config_element+0x14e>
     dad:	090242                                    	l8ui	a4, a2, 9
     db0:	f48c                                    	beqz.n	a4, dc3 <loas_program_config_element+0x173>
     db2:	040c                                    	movi.n	a4, 0
     db4:	5b0c                                    	movi.n	a11, 5
     db6:	03ad                                    	mov.n	a10, a3
     db8:	0005e0                                    	callx8	a5
     dbb:	090262                                    	l8ui	a6, a2, 9
     dbe:	441b                                    	addi.n	a4, a4, 1
     dc0:	f03467                                    	bltu	a4, a6, db4 <loas_program_config_element+0x164>
     dc3:	fca441                                    	l32r	a4, 54 (22d0 <aac_byte_align>)
     dc6:	03ad                                    	mov.n	a10, a3
     dc8:	0004e0                                    	callx8	a4
     dcb:	8b0c                                    	movi.n	a11, 8
     dcd:	03ad                                    	mov.n	a10, a3
     dcf:	0005e0                                    	callx8	a5
     dd2:	0a4d                                    	mov.n	a4, a10
     dd4:	0b1aa6                                    	blti	a10, 1, de3 <loas_program_config_element+0x193>
     dd7:	8b0c                                    	movi.n	a11, 8
     dd9:	03ad                                    	mov.n	a10, a3
     ddb:	0005e0                                    	callx8	a5
     dde:	440b                                    	addi.n	a4, a4, -1
     de0:	ff3456                                    	bnez	a4, dd7 <loas_program_config_element+0x187>
     de3:	030222                                    	l8ui	a2, a2, 3
     de6:	0432b6                                    	bltui	a2, 3, dee <loas_program_config_element+0x19e>
     de9:	120c                                    	movi.n	a2, 1
     deb:	000046                                    	j	df0 <loas_program_config_element+0x1a0>

00000dee <loas_program_config_element+0x19e>:
     dee:	020c                                    	movi.n	a2, 0
     df0:	030430                                    	rsr.br	a3
     df3:	413130                                    	srli	a3, a3, 1
     df6:	042020                                    	extui	a2, a2, 0, 1
     df9:	903320                                    	addx2	a3, a3, a2
     dfc:	130430                                    	wsr.br	a3
     dff:	f01d                                    	retw.n

00000e01 <loas_program_config_element+0x1b1>:
     e01:	000000                                        ...

00000e04 <channel_pair_element>:
     e04:	00c136                                    	entry	a1, 96
     e07:	b4a0c2                                    	movi	a12, 180
     e0a:	027d                                    	mov.n	a7, a2
     e0c:	40dc22                                    	addmi	a2, a12, 0x4000
     e0f:	1d4c                                    	movi.n	a13, 65
     e11:	872a                                    	add.n	a8, a7, a2
     e13:	fc9921                                    	l32r	a2, 78 (388c <xt_memset8_0>)
     e16:	6159                                    	s32i.n	a5, a1, 24
     e18:	035d                                    	mov.n	a5, a3
     e1a:	113da0                                    	slli	a3, a13, 6
     e1d:	40c7a2                                    	addi	a10, a7, 64
     e20:	47d7e2                                    	addmi	a14, a7, 0x4700
     e23:	f88b                                    	addi.n	a15, a8, 8
     e25:	00a2b2                                    	movi	a11, 0x200
     e28:	40d762                                    	addmi	a6, a7, 0x4000
     e2b:	91e9                                    	s32i.n	a14, a1, 36
     e2d:	373a                                    	add.n	a3, a7, a3
     e2f:	c189                                    	s32i.n	a8, a1, 48
     e31:	a1f9                                    	s32i.n	a15, a1, 40
     e33:	71a9                                    	s32i.n	a10, a1, 28
     e35:	0002e0                                    	callx8	a2
     e38:	00a2b2                                    	movi	a11, 0x200
     e3b:	03ad                                    	mov.n	a10, a3
     e3d:	e139                                    	s32i.n	a3, a1, 56
     e3f:	0002e0                                    	callx8	a2
     e42:	241b                                    	addi.n	a2, a4, 1
     e44:	5b0c                                    	movi.n	a11, 5
     e46:	5b5622                                    	s16i	a2, a6, 182
     e49:	05ad                                    	mov.n	a10, a5
     e4b:	fc8521                                    	l32r	a2, 60 (226c <aac_getbits>)
     e4e:	b44642                                    	s8i	a4, a6, 180
     e51:	054d                                    	mov.n	a4, a5
     e53:	0002e0                                    	callx8	a2
     e56:	6158                                    	l32i.n	a5, a1, 24
     e58:	4121a0                                    	srli	a2, a10, 1
     e5b:	0430a0                                    	extui	a3, a10, 0, 1
     e5e:	b84622                                    	s8i	a2, a6, 184
     e61:	5169                                    	s32i.n	a6, a1, 20
     e63:	04cd                                    	mov.n	a12, a4
     e65:	004522                                    	s8i	a2, a5, 0
     e68:	a1b8                                    	l32i.n	a11, a1, 40
     e6a:	b94632                                    	s8i	a3, a6, 185
     e6d:	71f8                                    	l32i.n	a15, a1, 28
     e6f:	a158                                    	l32i.n	a5, a1, 40
     e71:	0aea07                                    	bbsi	a10, 0, e7f <channel_pair_element+0x7b>
     e74:	44d722                                    	addmi	a2, a7, 0x4400
     e77:	030c                                    	movi.n	a3, 0
     e79:	fc4232                                    	s8i	a3, a2, 252
     e7c:	005886                                    	j	fe2 <channel_pair_element+0x1de>

00000e7f <channel_pair_element+0x7b>:
     e7f:	fc8221                                    	l32r	a2, 88 (11c0 <ics_info>)
     e82:	07ad                                    	mov.n	a10, a7
     e84:	0002e0                                    	callx8	a2
     e87:	0a2d                                    	mov.n	a2, a10
     e89:	011aa6                                    	blti	a10, 1, e8e <channel_pair_element+0x8a>
     e8c:	f01d                                    	retw.n

00000e8e <channel_pair_element+0x8a>:
     e8e:	fc7421                                    	l32r	a2, 60 (226c <aac_getbits>)
     e91:	2b0c                                    	movi.n	a11, 2
     e93:	04ad                                    	mov.n	a10, a4
     e95:	44d732                                    	addmi	a3, a7, 0x4400
     e98:	0002e0                                    	callx8	a2
     e9b:	40d722                                    	addmi	a2, a7, 0x4000
     e9e:	fc43a2                                    	s8i	a10, a3, 252
     ea1:	c20262                                    	l8ui	a6, a2, 194
     ea4:	d129                                    	s32i.n	a2, a1, 52
     ea6:	bafd                                    	ae_zext8	a15, a10
     ea8:	fda022                                    	movi	a2, 253
     eab:	021f26                                    	beqi	a15, 1, eb1 <channel_pair_element+0xad>
     eae:	002386                                    	j	f40 <channel_pair_element+0x13c>
     eb1:	51f8                                    	l32i.n	a15, a1, 20
     eb3:	087616                                    	beqz	a6, f3e <channel_pair_element+0x13a>
     eb6:	076d                                    	mov.n	a6, a7
     eb8:	44d222                                    	addmi	a2, a2, 0x4400
     ebb:	8139                                    	s32i.n	a3, a1, 32
     ebd:	051c                                    	movi.n	a5, 16
     ebf:	bc0f72                                    	l8ui	a7, a15, 188
     ec2:	080c                                    	movi.n	a8, 0
     ec4:	362a                                    	add.n	a3, a6, a2
     ec6:	4169                                    	s32i.n	a6, a1, 16
     ec8:	036d                                    	mov.n	a6, a3
     eca:	6189                                    	s32i.n	a8, a1, 24
     ecc:	072d                                    	mov.n	a2, a7
     ece:	2db7b6                                    	bltui	a7, 16, eff <channel_pair_element+0xfb>
     ed1:	0b1c                                    	movi.n	a11, 16
     ed3:	036d                                    	mov.n	a6, a3
     ed5:	fc6281                                    	l32r	a8, 60 (226c <aac_getbits>)
     ed8:	20a440                                    	or	a10, a4, a4
     edb:	0008e0                                    	callx8	a8
     ede:	0fa082                                    	movi	a8, 15
     ee1:	0c8576                                    	loop	a5, ef1 <channel_pair_element+0xed>
     ee4:	0490a0                                    	extui	a9, a10, 0, 1
     ee7:	b68a                                    	add.n	a11, a6, a8
     ee9:	41a1a0                                    	srli	a10, a10, 1
     eec:	880b                                    	addi.n	a8, a8, -1
     eee:	004b92                                    	s8i	a9, a11, 0

00000ef1 <channel_pair_element+0xed>:
     ef1:	f0c222                                    	addi	a2, a2, -16
     ef4:	10c662                                    	addi	a6, a6, 16
     ef7:	0b1c                                    	movi.n	a11, 16
     ef9:	d8b2e6                                    	bgei	a2, 16, ed5 <channel_pair_element+0xd1>
     efc:	347070                                    	extui	a7, a7, 0, 4

00000eff <channel_pair_element+0xfb>:
     eff:	fc5821                                    	l32r	a2, 60 (226c <aac_getbits>)
     f02:	04ad                                    	mov.n	a10, a4
     f04:	07bd                                    	mov.n	a11, a7
     f06:	0002e0                                    	callx8	a2
     f09:	802670                                    	add	a2, a6, a7
     f0c:	f78c                                    	beqz.n	a7, f1f <channel_pair_element+0x11b>
     f0e:	ffc222                                    	addi	a2, a2, -1
     f11:	0a8776                                    	loop	a7, f1f <channel_pair_element+0x11b>
     f14:	0460a0                                    	extui	a6, a10, 0, 1
     f17:	41a1a0                                    	srli	a10, a10, 1
     f1a:	004262                                    	s8i	a6, a2, 0
     f1d:	220b                                    	addi.n	a2, a2, -1

00000f1f <channel_pair_element+0x11b>:
     f1f:	5128                                    	l32i.n	a2, a1, 20
     f21:	d1e8                                    	l32i.n	a14, a1, 52
     f23:	6188                                    	l32i.n	a8, a1, 24
     f25:	bc0272                                    	l8ui	a7, a2, 188
     f28:	c20e62                                    	l8ui	a6, a14, 194
     f2b:	881b                                    	addi.n	a8, a8, 1
     f2d:	b8fd                                    	ae_zext8	a15, a8
     f2f:	337a                                    	add.n	a3, a3, a7
     f31:	933f67                                    	bltu	a15, a6, ec8 <channel_pair_element+0xc4>
     f34:	8128                                    	l32i.n	a2, a1, 32
     f36:	4178                                    	l32i.n	a7, a1, 16
     f38:	fc02a2                                    	l8ui	a10, a2, 252
     f3b:	000046                                    	j	f40 <channel_pair_element+0x13c>

00000f3e <channel_pair_element+0x13a>:
     f3e:	060c                                    	movi.n	a6, 0
     f40:	4bd732                                    	addmi	a3, a7, 0x4b00
     f43:	40d782                                    	addmi	a8, a7, 0x4000
     f46:	5198                                    	l32i.n	a9, a1, 20
     f48:	45d722                                    	addmi	a2, a7, 0x4500
     f4b:	9843a2                                    	s8i	a10, a3, 152
     f4e:	6098b2                                    	l16si	a11, a8, 192
     f51:	c30832                                    	l8ui	a3, a8, 195
     f54:	9188                                    	l32i.n	a8, a1, 36
     f56:	780222                                    	l8ui	a2, a2, 120
     f59:	4cd7f2                                    	addmi	a15, a7, 0x4c00
     f5c:	144f22                                    	s8i	a2, a15, 20
     f5f:	47d752                                    	addmi	a5, a7, 0x4700
     f62:	bc0922                                    	l8ui	a2, a9, 188
     f65:	40d7a2                                    	addmi	a10, a7, 0x4000
     f68:	68a0d2                                    	movi	a13, 104
     f6b:	cca0e2                                    	movi	a14, 204
     f6e:	5f4532                                    	s8i	a3, a5, 95
     f71:	47d7c2                                    	addmi	a12, a7, 0x4700
     f74:	584822                                    	s8i	a2, a8, 88
     f77:	bf0af2                                    	l8ui	a15, a10, 191
     f7a:	5e4562                                    	s8i	a6, a5, 94
     f7d:	47dd32                                    	addmi	a3, a13, 0x4700
     f80:	2e55b2                                    	s16i	a11, a5, 92
     f83:	40de52                                    	addmi	a5, a14, 0x4000
     f86:	fc4121                                    	l32r	a2, 8c (38ac <xt_memcpy4>)
     f89:	a73a                                    	add.n	a10, a7, a3
     f8b:	5b4cf2                                    	s8i	a15, a12, 91
     f8e:	b75a                                    	add.n	a11, a7, a5
     f90:	cc3c                                    	movi.n	a12, 60
     f92:	b149                                    	s32i.n	a4, a1, 44
     f94:	0002e0                                    	callx8	a2
     f97:	835c                                    	movi.n	a3, 88
     f99:	bca042                                    	movi	a4, 188
     f9c:	48d332                                    	addmi	a3, a3, 0x4800
     f9f:	41d442                                    	addmi	a4, a4, 0x4100
     fa2:	a73a                                    	add.n	a10, a7, a3
     fa4:	b74a                                    	add.n	a11, a7, a4
     fa6:	ac1c                                    	movi.n	a12, 26
     fa8:	3ba232                                    	movi	a3, 0x23b
     fab:	0002e0                                    	callx8	a2
     fae:	c4a022                                    	movi	a2, 196
     fb1:	1133b0                                    	slli	a3, a3, 5
     fb4:	40d222                                    	addmi	a2, a2, 0x4000
     fb7:	840c                                    	movi.n	a4, 8
     fb9:	272a                                    	add.n	a2, a7, a2
     fbb:	373a                                    	add.n	a3, a7, a3
     fbd:	098476                                    	loop	a4, fca <channel_pair_element+0x1c6>
     fc0:	000242                                    	l8ui	a4, a2, 0
     fc3:	221b                                    	addi.n	a2, a2, 1
     fc5:	004342                                    	s8i	a4, a3, 0
     fc8:	331b                                    	addi.n	a3, a3, 1

00000fca <channel_pair_element+0x1c6>:
     fca:	40d722                                    	addmi	a2, a7, 0x4000
     fcd:	47d732                                    	addmi	a3, a7, 0x4700
     fd0:	bd0242                                    	l8ui	a4, a2, 189
     fd3:	71f8                                    	l32i.n	a15, a1, 28
     fd5:	594342                                    	s8i	a4, a3, 89
     fd8:	a158                                    	l32i.n	a5, a1, 40
     fda:	be0222                                    	l8ui	a2, a2, 190
     fdd:	b148                                    	l32i.n	a4, a1, 44
     fdf:	5a4322                                    	s8i	a2, a3, 90

00000fe2 <channel_pair_element+0x1de>:
     fe2:	fc2631                                    	l32r	a3, 7c (1284 <one_channel_stream>)
     fe5:	0e0c                                    	movi.n	a14, 0
     fe7:	c1b8                                    	l32i.n	a11, a1, 48
     fe9:	07ad                                    	mov.n	a10, a7
     feb:	04cd                                    	mov.n	a12, a4
     fed:	05dd                                    	mov.n	a13, a5
     fef:	0003e0                                    	callx8	a3
     ff2:	0a2d                                    	mov.n	a2, a10
     ff4:	021aa6                                    	blti	a10, 1, ffa <channel_pair_element+0x1f6>
     ff7:	ffa446                                    	j	e8c <channel_pair_element+0x88>
     ffa:	825c                                    	movi.n	a2, 88
     ffc:	c1b8                                    	l32i.n	a11, a1, 48
     ffe:	47d222                                    	addmi	a2, a2, 0x4700
    1001:	e1f8                                    	l32i.n	a15, a1, 56
    1003:	272a                                    	add.n	a2, a7, a2
    1005:	04cd                                    	mov.n	a12, a4
    1007:	0e0c                                    	movi.n	a14, 0
    1009:	07ad                                    	mov.n	a10, a7
    100b:	02dd                                    	mov.n	a13, a2
    100d:	024d                                    	mov.n	a4, a2
    100f:	0003e0                                    	callx8	a3
    1012:	0a2d                                    	mov.n	a2, a10
    1014:	021aa6                                    	blti	a10, 1, 101a <channel_pair_element+0x216>
    1017:	ff9c46                                    	j	e8c <channel_pair_element+0x88>
    101a:	0c0722                                    	l8ui	a2, a7, 12
    101d:	8149                                    	s32i.n	a4, a1, 32
    101f:	81a052                                    	movi	a5, 129
    1022:	6768                                    	l32i.n	a6, a7, 24
    1024:	7798                                    	l32i.n	a9, a7, 28
    1026:	1f0c                                    	movi.n	a15, 1
    1028:	272a                                    	add.n	a2, a7, a2
    102a:	c1a0e2                                    	movi	a14, 193
    102d:	1145a0                                    	slli	a4, a5, 6
    1030:	100232                                    	l8ui	a3, a2, 16
    1033:	474a                                    	add.n	a4, a7, a4
    1035:	118ea0                                    	slli	a8, a14, 6
    1038:	b3cc                                    	bnez.n	a3, 1047 <channel_pair_element+0x243>
    103a:	378a                                    	add.n	a3, a7, a8
    103c:	9769                                    	s32i.n	a6, a7, 36
    103e:	a799                                    	s32i.n	a9, a7, 40
    1040:	b749                                    	s32i.n	a4, a7, 44
    1042:	c739                                    	s32i.n	a3, a7, 48
    1044:	1042f2                                    	s8i	a15, a2, 16
    1047:	44d722                                    	addmi	a2, a7, 0x4400
    104a:	021732                                    	l16ui	a3, a7, 4
    104d:	fc0222                                    	l8ui	a2, a2, 252
    1050:	4179                                    	s32i.n	a7, a1, 16
    1052:	f139                                    	s32i.n	a3, a1, 60
    1054:	4138                                    	l32i.n	a3, a1, 16
    1056:	bca062                                    	movi	a6, 188
    1059:	841c                                    	movi.n	a4, 24
    105b:	0c0216                                    	beqz	a2, 111f <channel_pair_element+0x31b>
    105e:	40d332                                    	addmi	a3, a3, 0x4000
    1061:	c4a082                                    	movi	a8, 196
    1064:	c20332                                    	l8ui	a3, a3, 194
    1067:	41d662                                    	addmi	a6, a6, 0x4100
    106a:	b139                                    	s32i.n	a3, a1, 44
    106c:	9ca7a2                                    	movi	a10, 0x79c
    106f:	f178                                    	l32i.n	a7, a1, 60
    1071:	91b8                                    	l32i.n	a11, a1, 36
    1073:	5158                                    	l32i.n	a5, a1, 20
    1075:	0a6316                                    	beqz	a3, 111f <channel_pair_element+0x31b>
    1078:	41c8                                    	l32i.n	a12, a1, 16
    107a:	4bd442                                    	addmi	a4, a4, 0x4b00
    107d:	413370                                    	srli	a3, a7, 3
    1080:	40d882                                    	addmi	a8, a8, 0x4000
    1083:	bc0572                                    	l8ui	a7, a5, 188
    1086:	580b52                                    	l8ui	a5, a11, 88
    1089:	6139                                    	s32i.n	a3, a1, 24
    108b:	b03aa0                                    	addx8	a3, a10, a10
    108e:	ec6a                                    	add.n	a14, a12, a6
    1090:	9159                                    	s32i.n	a5, a1, 36
    1092:	090c                                    	movi.n	a9, 0
    1094:	fb0c                                    	movi.n	a11, 15
    1096:	dc3a                                    	add.n	a13, a12, a3
    1098:	fc4a                                    	add.n	a15, a12, a4
    109a:	6c8a                                    	add.n	a6, a12, a8
    109c:	c1e9                                    	s32i.n	a14, a1, 48
    109e:	000642                                    	l8ui	a4, a6, 0
    10a1:	030c                                    	movi.n	a3, 0
    10a3:	067416                                    	beqz	a4, 110e <channel_pair_element+0x30a>
    10a6:	d169                                    	s32i.n	a6, a1, 52
    10a8:	c188                                    	l32i.n	a8, a1, 48
    10aa:	057716                                    	beqz	a7, 1105 <channel_pair_element+0x301>
    10ad:	060c                                    	movi.n	a6, 0
    10af:	4d6a                                    	add.n	a4, a13, a6
    10b1:	042226                                    	beqi	a2, 2, 10b9 <channel_pair_element+0x2b5>
    10b4:	8104e2                                    	l8ui	a14, a4, 129
    10b7:	eebc                                    	beqz.n	a14, 10f9 <channel_pair_element+0x2f5>
    10b9:	ef6a                                    	add.n	a14, a15, a6
    10bb:	150c                                    	movi.n	a5, 1
    10bd:	000ee2                                    	l8ui	a14, a14, 0
    10c0:	da0c                                    	movi.n	a10, 13
    10c2:	20ee50                                    	or	a14, a14, a5
    10c5:	301eb7                                    	beq	a14, a11, 10f9 <channel_pair_element+0x2f5>
    10c8:	000442                                    	l8ui	a4, a4, 0
    10cb:	2a14a7                                    	beq	a4, a10, 10f9 <channel_pair_element+0x2f5>
    10ce:	484d                                    	ae_l16ui.n	a4, a8, 0
    10d0:	71a8                                    	l32i.n	a10, a1, 28
    10d2:	58ed                                    	ae_l16ui.n	a14, a8, 2
    10d4:	594a                                    	add.n	a5, a9, a4
    10d6:	c0ce40                                    	sub	a12, a14, a4
    10d9:	1cb4e7                                    	bgeu	a4, a14, 10f9 <channel_pair_element+0x2f5>
    10dc:	0e21e2                                    	l32i	a14, a1, 56
    10df:	a045e0                                    	addx4	a4, a5, a14
    10e2:	a0e5a0                                    	addx4	a14, a5, a10
    10e5:	108c76                                    	loop	a12, 10f9 <channel_pair_element+0x2f5>
    10e8:	0e58                                    	l32i.n	a5, a14, 0
    10ea:	04c8                                    	l32i.n	a12, a4, 0
    10ec:	ac5a                                    	add.n	a10, a12, a5
    10ee:	c055c0                                    	sub	a5, a5, a12
    10f1:	0ea9                                    	s32i.n	a10, a14, 0
    10f3:	ee4b                                    	addi.n	a14, a14, 4
    10f5:	0459                                    	s32i.n	a5, a4, 0
    10f7:	444b                                    	addi.n	a4, a4, 4

000010f9 <channel_pair_element+0x2f5>:
    10f9:	661b                                    	addi.n	a6, a6, 1
    10fb:	882b                                    	addi.n	a8, a8, 2
    10fd:	ae9767                                    	bne	a7, a6, 10af <channel_pair_element+0x2ab>
    1100:	d168                                    	l32i.n	a6, a1, 52
    1102:	000642                                    	l8ui	a4, a6, 0

00001105 <channel_pair_element+0x301>:
    1105:	6158                                    	l32i.n	a5, a1, 24
    1107:	331b                                    	addi.n	a3, a3, 1
    1109:	995a                                    	add.n	a9, a9, a5
    110b:	993347                                    	bltu	a3, a4, 10a8 <channel_pair_element+0x2a4>
    110e:	b138                                    	l32i.n	a3, a1, 44
    1110:	dd7a                                    	add.n	a13, a13, a7
    1112:	9148                                    	l32i.n	a4, a1, 36
    1114:	661b                                    	addi.n	a6, a6, 1
    1116:	330b                                    	addi.n	a3, a3, -1
    1118:	ff4a                                    	add.n	a15, a15, a4
    111a:	b139                                    	s32i.n	a3, a1, 44
    111c:	f7e356                                    	bnez	a3, 109e <channel_pair_element+0x29a>
    111f:	a158                                    	l32i.n	a5, a1, 40
    1121:	e178                                    	l32i.n	a7, a1, 56
    1123:	7148                                    	l32i.n	a4, a1, 28
    1125:	81b8                                    	l32i.n	a11, a1, 32
    1127:	f1e8                                    	l32i.n	a14, a1, 60
    1129:	fbd921                                    	l32r	a2, 90 (38c0 <is_decode>)
    112c:	05ad                                    	mov.n	a10, a5
    112e:	04cd                                    	mov.n	a12, a4
    1130:	07dd                                    	mov.n	a13, a7
    1132:	0002e0                                    	callx8	a2
    1135:	4168                                    	l32i.n	a6, a1, 16
    1137:	45d622                                    	addmi	a2, a6, 0x4500
    113a:	04ed                                    	mov.n	a14, a4
    113c:	770232                                    	l8ui	a3, a2, 119
    113f:	fbd021                                    	l32r	a2, 80 (1e6c <tns_decode>)
    1142:	439c                                    	beqz.n	a3, 115a <channel_pair_element+0x356>
    1144:	88a032                                    	movi	a3, 136
    1147:	0806c2                                    	l8ui	a12, a6, 8
    114a:	45d332                                    	addmi	a3, a3, 0x4500
    114d:	0906d2                                    	l8ui	a13, a6, 9
    1150:	0216f2                                    	l16ui	a15, a6, 4
    1153:	b63a                                    	add.n	a11, a6, a3
    1155:	05ad                                    	mov.n	a10, a5
    1157:	0002e0                                    	callx8	a2
    115a:	442c                                    	movi.n	a4, 36
    115c:	4cd632                                    	addmi	a3, a6, 0x4c00
    115f:	4cd452                                    	addmi	a5, a4, 0x4c00
    1162:	130332                                    	l8ui	a3, a3, 19
    1165:	b65a                                    	add.n	a11, a6, a5
    1167:	81a8                                    	l32i.n	a10, a1, 32
    1169:	07ed                                    	mov.n	a14, a7
    116b:	a38c                                    	beqz.n	a3, 1179 <channel_pair_element+0x375>
    116d:	0806c2                                    	l8ui	a12, a6, 8
    1170:	0906d2                                    	l8ui	a13, a6, 9
    1173:	0216f2                                    	l16ui	a15, a6, 4
    1176:	0002e0                                    	callx8	a2
    1179:	40d632                                    	addmi	a3, a6, 0x4000
    117c:	fbc241                                    	l32r	a4, 84 (2b50 <filter_bank_LC>)
    117f:	bd03b2                                    	l8ui	a11, a3, 189
    1182:	71d8                                    	l32i.n	a13, a1, 28
    1184:	be03c2                                    	l8ui	a12, a3, 190
    1187:	0e0c                                    	movi.n	a14, 0
    1189:	06ad                                    	mov.n	a10, a6
    118b:	020c                                    	movi.n	a2, 0
    118d:	0004e0                                    	callx8	a4
    1190:	47d652                                    	addmi	a5, a6, 0x4700
    1193:	1e0c                                    	movi.n	a14, 1
    1195:	5905b2                                    	l8ui	a11, a5, 89
    1198:	06ad                                    	mov.n	a10, a6
    119a:	5a05c2                                    	l8ui	a12, a5, 90
    119d:	07dd                                    	mov.n	a13, a7
    119f:	0004e0                                    	callx8	a4
    11a2:	5178                                    	l32i.n	a7, a1, 20
    11a4:	13c662                                    	addi	a6, a6, 19
    11a7:	be0332                                    	l8ui	a3, a3, 190
    11aa:	b40742                                    	l8ui	a4, a7, 180
    11ad:	464a                                    	add.n	a4, a6, a4
    11af:	004432                                    	s8i	a3, a4, 0
    11b2:	5b9732                                    	l16si	a3, a7, 182
    11b5:	5a0542                                    	l8ui	a4, a5, 90
    11b8:	363a                                    	add.n	a3, a6, a3
    11ba:	004342                                    	s8i	a4, a3, 0
    11bd:	f01d                                    	retw.n

000011bf <channel_pair_element+0x3bb>:
	...

000011c0 <ics_info>:
    11c0:	004136                                    	entry	a1, 32
    11c3:	fba771                                    	l32r	a7, 60 (226c <aac_getbits>)
    11c6:	4b0c                                    	movi.n	a11, 4
    11c8:	04ad                                    	mov.n	a10, a4
    11ca:	025d                                    	mov.n	a5, a2
    11cc:	0007e0                                    	callx8	a7
    11cf:	1421a0                                    	extui	a2, a10, 1, 2
    11d2:	0460a0                                    	extui	a6, a10, 0, 1
    11d5:	6b0c                                    	movi.n	a11, 6
    11d7:	014322                                    	s8i	a2, a3, 1
    11da:	04ad                                    	mov.n	a10, a4
    11dc:	024362                                    	s8i	a6, a3, 2
    11df:	122266                                    	bnei	a2, 2, 11f5 <ics_info+0x35>
    11e2:	bb0c                                    	movi.n	a11, 11
    11e4:	04ad                                    	mov.n	a10, a4
    11e6:	0007e0                                    	callx8	a7
    11e9:	6420a0                                    	extui	a2, a10, 0, 7
    11ec:	34a7a0                                    	extui	a10, a10, 7, 4
    11ef:	034322                                    	s8i	a2, a3, 3
    11f2:	000086                                    	j	11f8 <ics_info+0x38>

000011f5 <ics_info+0x35>:
    11f5:	0007e0                                    	callx8	a7
    11f8:	fba721                                    	l32r	a2, 94 (3740 <window_grouping_info>)
    11fb:	03bd                                    	mov.n	a11, a3
    11fd:	0043a2                                    	s8i	a10, a3, 0
    1200:	05ad                                    	mov.n	a10, a5
    1202:	0002e0                                    	callx8	a2
    1205:	0a2d                                    	mov.n	a2, a10
    1207:	741ae6                                    	bgei	a10, 1, 127f <ics_info+0xbf>
    120a:	000382                                    	l8ui	a8, a3, 0
    120d:	62a022                                    	movi	a2, 98
    1210:	050392                                    	l8ui	a9, a3, 5
    1213:	0a6d                                    	mov.n	a6, a10
    1215:	0103b2                                    	l8ui	a11, a3, 1
    1218:	633987                                    	bltu	a9, a8, 127f <ics_info+0xbf>
    121b:	0a2d                                    	mov.n	a2, a10
    121d:	5e2b26                                    	beqi	a11, 2, 127f <ics_info+0xbf>
    1220:	fb9221                                    	l32r	a2, 68 (22b4 <aac_get1bit>)
    1223:	04ad                                    	mov.n	a10, a4
    1225:	0002e0                                    	callx8	a2
    1228:	04d322                                    	addmi	a2, a3, 0x400
    122b:	ffa082                                    	movi	a8, 255
    122e:	bd42a2                                    	s8i	a10, a2, 189
    1231:	062d                                    	mov.n	a2, a6
    1233:	000392                                    	l8ui	a9, a3, 0
    1236:	4508a7                                    	bnone	a8, a10, 127f <ics_info+0xbf>
    1239:	090582                                    	l8ui	a8, a5, 9
    123c:	062d                                    	mov.n	a2, a6
    123e:	080552                                    	l8ui	a5, a5, 8
    1241:	030c                                    	movi.n	a3, 0
    1243:	381866                                    	bnei	a8, 1, 127f <ics_info+0xbf>
    1246:	092d                                    	mov.n	a2, a9
    1248:	07a5f6                                    	bgeui	a5, 12, 1253 <ics_info+0x93>
    124b:	fb9331                                    	l32r	a3, 98 (63d0 <predef_sfb_max>)
    124e:	335a                                    	add.n	a3, a3, a5
    1250:	000332                                    	l8ui	a3, a3, 0
    1253:	433230                                    	min	a3, a2, a3
    1256:	fb8421                                    	l32r	a2, 68 (22b4 <aac_get1bit>)
    1259:	04ad                                    	mov.n	a10, a4
    125b:	0002e0                                    	callx8	a2
    125e:	066a07                                    	bbci	a10, 0, 1268 <ics_info+0xa8>
    1261:	5b0c                                    	movi.n	a11, 5
    1263:	04ad                                    	mov.n	a10, a4
    1265:	0007e0                                    	callx8	a7
    1268:	fb8071                                    	l32r	a7, 68 (22b4 <aac_get1bit>)
    126b:	ffa052                                    	movi	a5, 255
    126e:	062d                                    	mov.n	a2, a6
    1270:	0b0537                                    	bnone	a5, a3, 127f <ics_info+0xbf>
    1273:	04ad                                    	mov.n	a10, a4
    1275:	330b                                    	addi.n	a3, a3, -1
    1277:	0007e0                                    	callx8	a7
    127a:	062d                                    	mov.n	a2, a6
    127c:	ff3356                                    	bnez	a3, 1273 <ics_info+0xb3>
    127f:	f01d                                    	retw.n

00001281 <ics_info+0xc1>:
    1281:	000000                                        ...

00001284 <one_channel_stream>:
    1284:	00c136                                    	entry	a1, 96
    1287:	7179                                    	s32i.n	a7, a1, 28
    1289:	067d                                    	mov.n	a7, a6
    128b:	026d                                    	mov.n	a6, a2
    128d:	fb7421                                    	l32r	a2, 60 (226c <aac_getbits>)
    1290:	8b0c                                    	movi.n	a11, 8
    1292:	04ad                                    	mov.n	a10, a4
    1294:	0002e0                                    	callx8	a2
    1297:	050322                                    	l8ui	a2, a3, 5
    129a:	ffa0f2                                    	movi	a15, 255
    129d:	05bd                                    	mov.n	a11, a5
    129f:	e179                                    	s32i.n	a7, a1, 56
    12a1:	0445a2                                    	s8i	a10, a5, 4
    12a4:	06ad                                    	mov.n	a10, a6
    12a6:	202270                                    	or	a2, a2, a7
    12a9:	2159                                    	s32i.n	a5, a1, 8
    12ab:	04cd                                    	mov.n	a12, a4
    12ad:	028f27                                    	bany	a15, a2, 12b3 <one_channel_stream+0x2f>
    12b0:	004e46                                    	j	13ed <one_channel_stream+0x169>

000012b3 <one_channel_stream+0x2f>:
    12b3:	010b32                                    	l8ui	a3, a11, 1
    12b6:	550c                                    	movi.n	a5, 5
    12b8:	320c                                    	movi.n	a2, 3
    12ba:	6169                                    	s32i.n	a6, a1, 24
    12bc:	d0a2a2                                    	movi	a10, 0x2d0
    12bf:	58a292                                    	movi	a9, 0x258
    12c2:	fec372                                    	addi	a7, a3, -2
    12c5:	f37c                                    	movi.n	a3, -1
    12c7:	932570                                    	movnez	a2, a5, a7
    12ca:	060b52                                    	l8ui	a5, a11, 6
    12cd:	401200                                    	ssl	a2
    12d0:	c0a382                                    	movi	a8, 0x3c0
    12d3:	a17300                                    	sll	a7, a3
    12d6:	38a4c2                                    	movi	a12, 0x438
    12d9:	303730                                    	xor	a3, a7, a3
    12dc:	0f4516                                    	beqz	a5, 13d4 <one_channel_stream+0x150>
    12df:	04dbe2                                    	addmi	a14, a11, 0x400
    12e2:	000b72                                    	l8ui	a7, a11, 0
    12e5:	6b9a                                    	add.n	a6, a11, a9
    12e7:	8b8a                                    	add.n	a8, a11, a8
    12e9:	fbaa                                    	add.n	a15, a11, a10
    12eb:	cbca                                    	add.n	a12, a11, a12
    12ed:	f1e9                                    	s32i.n	a14, a1, 60
    12ef:	0d0c                                    	movi.n	a13, 0
    12f1:	4169                                    	s32i.n	a6, a1, 16
    12f3:	ffa0e2                                    	movi	a14, 255
    12f6:	8189                                    	s32i.n	a8, a1, 32
    12f8:	060c                                    	movi.n	a6, 0
    12fa:	31f9                                    	s32i.n	a15, a1, 12
    12fc:	080c                                    	movi.n	a8, 0
    12fe:	c1c9                                    	s32i.n	a12, a1, 48
    1300:	090c                                    	movi.n	a9, 0
    1302:	0a0c                                    	movi.n	a10, 0
    1304:	028e77                                    	bany	a14, a7, 130a <one_channel_stream+0x86>
    1307:	002c06                                    	j	13bb <one_channel_stream+0x137>
    130a:	4178                                    	l32i.n	a7, a1, 16
    130c:	d1d9                                    	s32i.n	a13, a1, 52
    130e:	090c                                    	movi.n	a9, 0
    1310:	fb5451                                    	l32r	a5, 60 (226c <aac_getbits>)
    1313:	51a9                                    	s32i.n	a10, a1, 20
    1315:	4b0c                                    	movi.n	a11, 4
    1317:	04ad                                    	mov.n	a10, a4
    1319:	b199                                    	s32i.n	a9, a1, 44
    131b:	0005e0                                    	callx8	a5
    131e:	77a052                                    	movi	a5, 119
    1321:	d80c                                    	movi.n	a8, 13
    1323:	190c                                    	movi.n	a9, 1
    1325:	02a567                                    	bge	a5, a6, 132b <one_channel_stream+0xa7>
    1328:	003446                                    	j	13fd <one_channel_stream+0x179>
    132b:	a169                                    	s32i.n	a6, a1, 40
    132d:	4179                                    	s32i.n	a7, a1, 16
    132f:	0047a2                                    	s8i	a10, a7, 0
    1332:	91a9                                    	s32i.n	a10, a1, 36
    1334:	f158                                    	l32i.n	a5, a1, 60
    1336:	029a87                                    	bne	a10, a8, 133c <one_channel_stream+0xb8>
    1339:	b94592                                    	s8i	a9, a5, 185
    133c:	fb4961                                    	l32r	a6, 60 (226c <aac_getbits>)
    133f:	04ad                                    	mov.n	a10, a4
    1341:	02bd                                    	mov.n	a11, a2
    1343:	0006e0                                    	callx8	a6
    1346:	ba7d                                    	ae_zext8	a7, a10
    1348:	0f9737                                    	bne	a7, a3, 135b <one_channel_stream+0xd7>
    134b:	037d                                    	mov.n	a7, a3
    134d:	04ad                                    	mov.n	a10, a4
    134f:	02bd                                    	mov.n	a11, a2
    1351:	0006e0                                    	callx8	a6
    1354:	ba5d                                    	ae_zext8	a5, a10
    1356:	757a                                    	add.n	a7, a5, a7
    1358:	f11537                                    	beq	a5, a3, 134d <one_channel_stream+0xc9>
    135b:	b168                                    	l32i.n	a6, a1, 44
    135d:	77a0f2                                    	movi	a15, 119
    1360:	3198                                    	l32i.n	a9, a1, 12
    1362:	88afb2                                    	movi	a11, -120
    1365:	07cd                                    	mov.n	a12, a7
    1367:	51a8                                    	l32i.n	a10, a1, 20
    1369:	576a                                    	add.n	a5, a7, a6
    136b:	fb3171                                    	l32r	a7, 30 (3878 <memset>)
    136e:	118580                                    	slli	a8, a5, 8
    1371:	886a                                    	add.n	a8, a8, a6
    1373:	698d                                    	ae_s16i.n	a8, a9, 0
    1375:	8188                                    	l32i.n	a8, a1, 32
    1377:	02bf57                                    	bgeu	a15, a5, 137d <one_channel_stream+0xf9>
    137a:	002106                                    	j	1402 <one_channel_stream+0x17e>
    137d:	029ab7                                    	bne	a10, a11, 1383 <one_channel_stream+0xff>
    1380:	0020c6                                    	j	1407 <one_channel_stream+0x183>
    1383:	91b8                                    	l32i.n	a11, a1, 36
    1385:	06b657                                    	bgeu	a6, a5, 138f <one_channel_stream+0x10b>
    1388:	a86a                                    	add.n	a10, a8, a6
    138a:	0007e0                                    	callx8	a7
    138d:	51a8                                    	l32i.n	a10, a1, 20
    138f:	21b8                                    	l32i.n	a11, a1, 8
    1391:	aa0b                                    	addi.n	a10, a10, -1
    1393:	3198                                    	l32i.n	a9, a1, 12
    1395:	a168                                    	l32i.n	a6, a1, 40
    1397:	4178                                    	l32i.n	a7, a1, 16
    1399:	000b82                                    	l8ui	a8, a11, 0
    139c:	992b                                    	addi.n	a9, a9, 2
    139e:	661b                                    	addi.n	a6, a6, 1
    13a0:	3199                                    	s32i.n	a9, a1, 12
    13a2:	771b                                    	addi.n	a7, a7, 1
    13a4:	059d                                    	mov.n	a9, a5
    13a6:	02b587                                    	bgeu	a5, a8, 13ac <one_channel_stream+0x128>
    13a9:	ffd8c6                                    	j	1310 <one_channel_stream+0x8c>
    13ac:	d1d8                                    	l32i.n	a13, a1, 52
    13ae:	060b52                                    	l8ui	a5, a11, 6
    13b1:	c1c8                                    	l32i.n	a12, a1, 48
    13b3:	4179                                    	s32i.n	a7, a1, 16
    13b5:	6090a0                                    	neg	a9, a10
    13b8:	ffa0e2                                    	movi	a14, 255
    13bb:	8178                                    	l32i.n	a7, a1, 32
    13bd:	acda                                    	add.n	a10, a12, a13
    13bf:	004a92                                    	s8i	a9, a10, 0
    13c2:	dd1b                                    	addi.n	a13, a13, 1
    13c4:	778a                                    	add.n	a7, a7, a8
    13c6:	090c                                    	movi.n	a9, 0
    13c8:	8179                                    	s32i.n	a7, a1, 32
    13ca:	087d                                    	mov.n	a7, a8
    13cc:	080c                                    	movi.n	a8, 0
    13ce:	02bd57                                    	bgeu	a13, a5, 13d4 <one_channel_stream+0x150>
    13d1:	ffcb46                                    	j	1302 <one_channel_stream+0x7e>
    13d4:	fb3221                                    	l32r	a2, 9c (1a00 <decode_scale_factors>)
    13d7:	0bad                                    	mov.n	a10, a11
    13d9:	04bd                                    	mov.n	a11, a4
    13db:	0002e0                                    	callx8	a2
    13de:	21b8                                    	l32i.n	a11, a1, 8
    13e0:	6178                                    	l32i.n	a7, a1, 24
    13e2:	e138                                    	l32i.n	a3, a1, 56
    13e4:	0a2d                                    	mov.n	a2, a10
    13e6:	ffa052                                    	movi	a5, 255
    13e9:	fa9c                                    	beqz.n	a10, 140c <one_channel_stream+0x188>
    13eb:	f01d                                    	retw.n

000013ed <one_channel_stream+0x169>:
    13ed:	fb2621                                    	l32r	a2, 88 (11c0 <ics_info>)
    13f0:	0002e0                                    	callx8	a2
    13f3:	21b8                                    	l32i.n	a11, a1, 8
    13f5:	0a2d                                    	mov.n	a2, a10
    13f7:	f01ae6                                    	bgei	a10, 1, 13eb <one_channel_stream+0x167>
    13fa:	ffad46                                    	j	12b3 <one_channel_stream+0x2f>

000013fd <one_channel_stream+0x179>:
    13fd:	65a022                                    	movi	a2, 101
    1400:	f01d                                    	retw.n

00001402 <one_channel_stream+0x17e>:
    1402:	66a022                                    	movi	a2, 102
    1405:	f01d                                    	retw.n

00001407 <one_channel_stream+0x183>:
    1407:	67a022                                    	movi	a2, 103
    140a:	f01d                                    	retw.n

0000140c <one_channel_stream+0x188>:
    140c:	04ad                                    	mov.n	a10, a4
    140e:	161316                                    	beqz	a3, 1573 <one_channel_stream+0x2ef>
    1411:	fb2521                                    	l32r	a2, a8 (1850 <spectral_data>)
    1414:	07ad                                    	mov.n	a10, a7
    1416:	71d8                                    	l32i.n	a13, a1, 28
    1418:	04cd                                    	mov.n	a12, a4
    141a:	0002e0                                    	callx8	a2
    141d:	2198                                    	l32i.n	a9, a1, 8
    141f:	0a2d                                    	mov.n	a2, a10
    1421:	6148                                    	l32i.n	a4, a1, 24
    1423:	c41ae6                                    	bgei	a10, 1, 13eb <one_channel_stream+0x167>
    1426:	04d922                                    	addmi	a2, a9, 0x400
    1429:	04d9c2                                    	addmi	a12, a9, 0x400
    142c:	ba0222                                    	l8ui	a2, a2, 186
    142f:	04d952                                    	addmi	a5, a9, 0x400
    1432:	14c9e2                                    	addi	a14, a9, 20
    1435:	19d216                                    	beqz	a2, 15d6 <one_channel_stream+0x352>
    1438:	010932                                    	l8ui	a3, a9, 1
    143b:	64a022                                    	movi	a2, 100
    143e:	070c                                    	movi.n	a7, 0
    1440:	a72326                                    	beqi	a3, 2, 13eb <one_channel_stream+0x167>
    1443:	021422                                    	l16ui	a2, a4, 4
    1446:	4129                                    	s32i.n	a2, a1, 16
    1448:	bf0c22                                    	l8ui	a2, a12, 191
    144b:	fb18d1                                    	l32r	a13, ac (4810 <aac_pow4_3>)
    144e:	b159                                    	s32i.n	a5, a1, 44
    1450:	90f290                                    	addx2	a15, a2, a9
    1453:	a1e9                                    	s32i.n	a14, a1, 40
    1455:	4d8b                                    	addi.n	a4, a13, 8
    1457:	801f32                                    	l16ui	a3, a15, 0x100
    145a:	020c                                    	movi.n	a2, 0
    145c:	9149                                    	s32i.n	a4, a1, 36
    145e:	492a                                    	add.n	a4, a9, a2
    1460:	4158                                    	l32i.n	a5, a1, 16
    1462:	04d442                                    	addmi	a4, a4, 0x400
    1465:	7168                                    	l32i.n	a6, a1, 28
    1467:	c00442                                    	l8ui	a4, a4, 192
    146a:	902290                                    	addx2	a2, a2, a9
    146d:	334a                                    	add.n	a3, a3, a4
    146f:	04d2f2                                    	addmi	a15, a2, 0x400
    1472:	834d                                    	ae_sext16	a4, a3
    1474:	a0b460                                    	addx4	a11, a4, a6
    1477:	022457                                    	blt	a4, a5, 147d <one_channel_stream+0x1f9>
    147a:	005d86                                    	j	15f4 <one_channel_stream+0x370>
    147d:	091952                                    	l16ui	a5, a9, 18
    1480:	629fa2                                    	l16si	a10, a15, 196
    1483:	0b28                                    	l32i.n	a2, a11, 0
    1485:	a168                                    	l32i.n	a6, a1, 40
    1487:	02a457                                    	bge	a4, a5, 148d <one_channel_stream+0x209>
    148a:	003806                                    	j	156e <one_channel_stream+0x2ea>
    148d:	050c                                    	movi.n	a5, 0
    148f:	468d                                    	ae_l16ui.n	a8, a6, 0
    1491:	662b                                    	addi.n	a6, a6, 2
    1493:	551b                                    	addi.n	a5, a5, 1
    1495:	f6a487                                    	bge	a4, a8, 148f <one_channel_stream+0x20b>
    1498:	854d                                    	ae_sext16	a4, a5
    149a:	904490                                    	addx2	a4, a4, a9
    149d:	b41442                                    	l16ui	a4, a4, 0x168
    14a0:	84fd                                    	ae_sext16	a15, a4
    14a2:	2152f0                                    	srai	a5, a15, 2
    14a5:	144040                                    	extui	a4, a4, 0, 2
    14a8:	e0c562                                    	addi	a6, a5, -32
    14ab:	fb0251                                    	l32r	a5, b4 (2b10 <aac_dequant_small>)
    14ae:	04cd                                    	mov.n	a12, a4
    14b0:	06d216                                    	beqz	a2, 1521 <one_channel_stream+0x29d>
    14b3:	51a9                                    	s32i.n	a10, a1, 20
    14b5:	0a0c                                    	movi.n	a10, 0
    14b7:	31b9                                    	s32i.n	a11, a1, 12
    14b9:	06bd                                    	mov.n	a11, a6
    14bb:	0005e0                                    	callx8	a5
    14be:	605120                                    	abs	a5, a2
    14c1:	072a16                                    	beqz	a10, 1537 <one_channel_stream+0x2b3>
    14c4:	fafd81                                    	l32r	a8, b8 (3a6c <__udivdi3>)
    14c7:	0acd                                    	mov.n	a12, a10
    14c9:	31dfa0                                    	srai	a13, a10, 31
    14cc:	01a5a0                                    	slli	a10, a5, 22
    14cf:	41ba50                                    	srli	a11, a5, 10
    14d2:	0008e0                                    	callx8	a8
    14d5:	090c                                    	movi.n	a9, 0
    14d7:	04cd                                    	mov.n	a12, a4
    14d9:	6289b0                                    	saltu	a8, a9, a11
    14dc:	729a90                                    	salt	a9, a10, a9
    14df:	9398b0                                    	movnez	a9, a8, a11
    14e2:	06bd                                    	mov.n	a11, a6
    14e4:	9188                                    	l32i.n	a8, a1, 36
    14e6:	05c956                                    	bnez	a9, 1546 <one_channel_stream+0x2c2>
    14e9:	190c                                    	movi.n	a9, 1
    14eb:	150c                                    	movi.n	a5, 1
    14ed:	0199b0                                    	slli	a9, a9, 21
    14f0:	0d7c                                    	movi.n	a13, -16
    14f2:	ae0c                                    	movi.n	a14, 10
    14f4:	09fd                                    	mov.n	a15, a9
    14f6:	08b8                                    	l32i.n	a11, a8, 0
    14f8:	884b                                    	addi.n	a8, a8, 4
    14fa:	34c0b0                                    	extui	a12, a11, 0, 4
    14fd:	10bbd0                                    	and	a11, a11, a13
    1500:	c0cec0                                    	sub	a12, a14, a12
    1503:	400c00                                    	ssr	a12
    1506:	9190b0                                    	srl	a9, a11
    1509:	05bd                                    	mov.n	a11, a5
    150b:	c9fa                                    	add.n	a12, a9, a15
    150d:	551b                                    	addi.n	a5, a5, 1
    150f:	09fd                                    	mov.n	a15, a9
    1511:	053ac7                                    	bltu	a10, a12, 151a <one_channel_stream+0x296>
    1514:	6ea0b2                                    	movi	a11, 110
    1517:	db95b7                                    	bne	a5, a11, 14f6 <one_channel_stream+0x272>
    151a:	5158                                    	l32i.n	a5, a1, 20
    151c:	ab5a                                    	add.n	a10, a11, a5
    151e:	0005c6                                    	j	1539 <one_channel_stream+0x2b5>

00001521 <one_channel_stream+0x29d>:
    1521:	fae321                                    	l32r	a2, b0 (27f4 <aac_dequant_medium>)
    1524:	0b5d                                    	mov.n	a5, a11
    1526:	06bd                                    	mov.n	a11, a6
    1528:	04cd                                    	mov.n	a12, a4
    152a:	0002e0                                    	callx8	a2
    152d:	056d                                    	mov.n	a6, a5
    152f:	2198                                    	l32i.n	a9, a1, 8
    1531:	6040a0                                    	neg	a4, a10
    1534:	000886                                    	j	155a <one_channel_stream+0x2d6>

00001537 <one_channel_stream+0x2b3>:
    1537:	51a8                                    	l32i.n	a10, a1, 20
    1539:	04cd                                    	mov.n	a12, a4
    153b:	fadd41                                    	l32r	a4, b0 (27f4 <aac_dequant_medium>)
    153e:	06bd                                    	mov.n	a11, a6
    1540:	0004e0                                    	callx8	a4
    1543:	000246                                    	j	1550 <one_channel_stream+0x2cc>

00001546 <one_channel_stream+0x2c2>:
    1546:	51a8                                    	l32i.n	a10, a1, 20
    1548:	fada41                                    	l32r	a4, b0 (27f4 <aac_dequant_medium>)
    154b:	0004e0                                    	callx8	a4
    154e:	aa5a                                    	add.n	a10, a10, a5
    1550:	6040a0                                    	neg	a4, a10
    1553:	2198                                    	l32i.n	a9, a1, 8
    1555:	3168                                    	l32i.n	a6, a1, 12
    1557:	b34a20                                    	movgez	a4, a10, a2
    155a:	b128                                    	l32i.n	a2, a1, 44
    155c:	771b                                    	addi.n	a7, a7, 1
    155e:	0649                                    	s32i.n	a4, a6, 0
    1560:	be0252                                    	l8ui	a5, a2, 190
    1563:	b72d                                    	ae_zext8	a2, a7
    1565:	023527                                    	bltu	a5, a2, 156b <one_channel_stream+0x2e7>
    1568:	ffbc86                                    	j	145e <one_channel_stream+0x1da>
    156b:	0019c6                                    	j	15d6 <one_channel_stream+0x352>

0000156e <one_channel_stream+0x2ea>:
    156e:	050c                                    	movi.n	a5, 0
    1570:	ffc906                                    	j	1498 <one_channel_stream+0x214>

00001573 <one_channel_stream+0x2ef>:
    1573:	fabd31                                    	l32r	a3, 68 (22b4 <aac_get1bit>)
    1576:	0003e0                                    	callx8	a3
    1579:	2168                                    	l32i.n	a6, a1, 8
    157b:	04d622                                    	addmi	a2, a6, 0x400
    157e:	ba42a2                                    	s8i	a10, a2, 186
    1581:	bea422                                    	movi	a2, 0x4be
    1584:	b62a                                    	add.n	a11, a6, a2
    1586:	1305a7                                    	bnone	a5, a10, 159d <one_channel_stream+0x319>
    1589:	fac521                                    	l32r	a2, a0 (15fc <pulse_data>)
    158c:	06ad                                    	mov.n	a10, a6
    158e:	04cd                                    	mov.n	a12, a4
    1590:	0002e0                                    	callx8	a2
    1593:	2168                                    	l32i.n	a6, a1, 8
    1595:	0a2d                                    	mov.n	a2, a10
    1597:	021aa6                                    	blti	a10, 1, 159d <one_channel_stream+0x319>
    159a:	ff9346                                    	j	13eb <one_channel_stream+0x167>
    159d:	04ad                                    	mov.n	a10, a4
    159f:	04d622                                    	addmi	a2, a6, 0x400
    15a2:	0003e0                                    	callx8	a3
    15a5:	bb42a2                                    	s8i	a10, a2, 187
    15a8:	0e05a7                                    	bnone	a5, a10, 15ba <one_channel_stream+0x336>
    15ab:	cca422                                    	movi	a2, 0x4cc
    15ae:	06ad                                    	mov.n	a10, a6
    15b0:	b62a                                    	add.n	a11, a6, a2
    15b2:	fabc21                                    	l32r	a2, a4 (172c <tns_data>)
    15b5:	04cd                                    	mov.n	a12, a4
    15b7:	0002e0                                    	callx8	a2
    15ba:	04ad                                    	mov.n	a10, a4
    15bc:	04d622                                    	addmi	a2, a6, 0x400
    15bf:	0003e0                                    	callx8	a3
    15c2:	bc42a2                                    	s8i	a10, a2, 188
    15c5:	ffa032                                    	movi	a3, 255
    15c8:	60a022                                    	movi	a2, 96
    15cb:	06bd                                    	mov.n	a11, a6
    15cd:	0203a7                                    	bnone	a3, a10, 15d3 <one_channel_stream+0x34f>
    15d0:	ff85c6                                    	j	13eb <one_channel_stream+0x167>
    15d3:	ff8e86                                    	j	1411 <one_channel_stream+0x18d>

000015d6 <one_channel_stream+0x352>:
    15d6:	010932                                    	l8ui	a3, a9, 1
    15d9:	6148                                    	l32i.n	a4, a1, 24
    15db:	020c                                    	movi.n	a2, 0
    15dd:	09ad                                    	mov.n	a10, a9
    15df:	71b8                                    	l32i.n	a11, a1, 28
    15e1:	022326                                    	beqi	a3, 2, 15e7 <one_channel_stream+0x363>
    15e4:	ff80c6                                    	j	13eb <one_channel_stream+0x167>
    15e7:	0214c2                                    	l16ui	a12, a4, 4
    15ea:	64d8                                    	l32i.n	a13, a4, 24
    15ec:	fab431                                    	l32r	a3, bc (1648 <de_interleave>)
    15ef:	0003e0                                    	callx8	a3
    15f2:	f01d                                    	retw.n

000015f4 <one_channel_stream+0x370>:
    15f4:	61a022                                    	movi	a2, 97
    15f7:	f01d                                    	retw.n

000015f9 <one_channel_stream+0x375>:
    15f9:	000000                                        ...

000015fc <pulse_data>:
    15fc:	004136                                    	entry	a1, 32
    15ff:	fa9851                                    	l32r	a5, 60 (226c <aac_getbits>)
    1602:	8b0c                                    	movi.n	a11, 8
    1604:	04ad                                    	mov.n	a10, a4
    1606:	0005e0                                    	callx8	a5
    1609:	050262                                    	l8ui	a6, a2, 5
    160c:	41f6a0                                    	srli	a15, a10, 6
    160f:	5470a0                                    	extui	a7, a10, 0, 6
    1612:	63a022                                    	movi	a2, 99
    1615:	0043f2                                    	s8i	a15, a3, 0
    1618:	014372                                    	s8i	a7, a3, 1
    161b:	273677                                    	bltu	a6, a7, 1646 <pulse_data+0x4a>
    161e:	160c                                    	movi.n	a6, 1
    1620:	020c                                    	movi.n	a2, 0
    1622:	070c                                    	movi.n	a7, 0

00001624 <pulse_data+0x28>:
    1624:	9b0c                                    	movi.n	a11, 9
    1626:	04ad                                    	mov.n	a10, a4
    1628:	0005e0                                    	callx8	a5
    162b:	837a                                    	add.n	a8, a3, a7
    162d:	0003b2                                    	l8ui	a11, a3, 0
    1630:	909730                                    	addx2	a9, a7, a3
    1633:	41c4a0                                    	srli	a12, a10, 4
    1636:	b67d                                    	ae_zext8	a7, a6
    1638:	34f0a0                                    	extui	a15, a10, 0, 4
    163b:	0248c2                                    	s8i	a12, a8, 2
    163e:	0359f2                                    	s16i	a15, a9, 6
    1641:	661b                                    	addi.n	a6, a6, 1
    1643:	ddbb77                                    	bgeu	a11, a7, 1624 <pulse_data+0x28>
    1646:	f01d                                    	retw.n

00001648 <de_interleave>:
    1648:	008136                                    	entry	a1, 64
    164b:	060292                                    	l8ui	a9, a2, 6
    164e:	00a462                                    	movi	a6, 0x400
    1651:	0502f2                                    	l8ui	a15, a2, 5
    1654:	2129                                    	s32i.n	a2, a1, 8
    1656:	020c                                    	movi.n	a2, 0
    1658:	31f9                                    	s32i.n	a15, a1, 12
    165a:	098676                                    	loop	a6, 1667 <de_interleave+0x1f>
    165d:	632a                                    	add.n	a6, a3, a2
    165f:	752a                                    	add.n	a7, a5, a2
    1661:	0668                                    	l32i.n	a6, a6, 0
    1663:	224b                                    	addi.n	a2, a2, 4
    1665:	0769                                    	s32i.n	a6, a7, 0

00001667 <de_interleave+0x1f>:
    1667:	2178                                    	l32i.n	a7, a1, 8
    1669:	0bd916                                    	beqz	a9, 172a <de_interleave+0xe2>
    166c:	247b                                    	addi.n	a2, a4, 7
    166e:	02a162                                    	movi	a6, 0x102
    1671:	b32440                                    	movgez	a2, a4, a4
    1674:	f87c                                    	movi.n	a8, -1
    1676:	212320                                    	srai	a2, a2, 3
    1679:	f76a                                    	add.n	a15, a7, a6
    167b:	a78b                                    	addi.n	a10, a7, 8
    167d:	f48080                                    	extui	a8, a8, 0, 16
    1680:	11d2e0                                    	slli	a13, a2, 2
    1683:	41f9                                    	s32i.n	a15, a1, 16
    1685:	5129                                    	s32i.n	a2, a1, 20
    1687:	001c86                                    	j	16fd <de_interleave+0xb5>

0000168a <de_interleave+0x42>:
	...

0000168b <de_interleave+0x43>:
    168b:	6199                                    	s32i.n	a9, a1, 24
    168d:	0e4d                                    	mov.n	a4, a14
    168f:	2168                                    	l32i.n	a6, a1, 8
    1691:	058216                                    	beqz	a2, 16ed <de_interleave+0xa5>
    1694:	41c8                                    	l32i.n	a12, a1, 16
    1696:	3178                                    	l32i.n	a7, a1, 12
    1698:	801622                                    	l16ui	a2, a6, 0x100
    169b:	4c6d                                    	ae_l16ui.n	a6, a12, 0
    169d:	7169                                    	s32i.n	a6, a1, 28
    169f:	c09620                                    	sub	a9, a6, a2
    16a2:	ffa062                                    	movi	a6, 255
    16a5:	992d                                    	ae_zext16	a2, a9
    16a7:	330647                                    	bnone	a6, a4, 16de <de_interleave+0x96>
    16aa:	a06f30                                    	addx4	a6, a15, a3
    16ad:	040c                                    	movi.n	a4, 0
    16af:	b23b                                    	addi.n	a11, a2, 3
    16b1:	200897                                    	bnone	a8, a9, 16d5 <de_interleave+0x8d>
    16b4:	41e2b0                                    	srli	a14, a11, 2
    16b7:	06bd                                    	mov.n	a11, a6
    16b9:	158e76                                    	loop	a14, 16d2 <de_interleave+0x8a>
    16bc:	05e8                                    	l32i.n	a14, a5, 0
    16be:	0be9                                    	s32i.n	a14, a11, 0
    16c0:	15e8                                    	l32i.n	a14, a5, 4
    16c2:	1be9                                    	s32i.n	a14, a11, 4
    16c4:	25e8                                    	l32i.n	a14, a5, 8
    16c6:	2be9                                    	s32i.n	a14, a11, 8
    16c8:	35e8                                    	l32i.n	a14, a5, 12
    16ca:	10c552                                    	addi	a5, a5, 16
    16cd:	3be9                                    	s32i.n	a14, a11, 12
    16cf:	10cbb2                                    	addi	a11, a11, 16

000016d2 <de_interleave+0x8a>:
    16d2:	000ae2                                    	l8ui	a14, a10, 0

000016d5 <de_interleave+0x8d>:
    16d5:	441b                                    	addi.n	a4, a4, 1
    16d7:	66da                                    	add.n	a6, a6, a13
    16d9:	0ebd                                    	mov.n	a11, a14
    16db:	d034e7                                    	bltu	a4, a14, 16af <de_interleave+0x67>
    16de:	ff2a                                    	add.n	a15, a15, a2
    16e0:	7128                                    	l32i.n	a2, a1, 28
    16e2:	770b                                    	addi.n	a7, a7, -1
    16e4:	cc2b                                    	addi.n	a12, a12, 2
    16e6:	0b4d                                    	mov.n	a4, a11
    16e8:	0b0c                                    	movi.n	a11, 0
    16ea:	fad756                                    	bnez	a7, 169b <de_interleave+0x53>
    16ed:	5128                                    	l32i.n	a2, a1, 20
    16ef:	6198                                    	l32i.n	a9, a1, 24
    16f1:	8222e0                                    	mull	a2, a2, a14
    16f4:	a03230                                    	addx4	a3, a2, a3
    16f7:	990b                                    	addi.n	a9, a9, -1
    16f9:	aa1b                                    	addi.n	a10, a10, 1
    16fb:	b9ac                                    	beqz.n	a9, 172a <de_interleave+0xe2>
    16fd:	000ae2                                    	l8ui	a14, a10, 0
    1700:	3128                                    	l32i.n	a2, a1, 12
    1702:	042c                                    	movi.n	a4, 32
    1704:	0b0c                                    	movi.n	a11, 0
    1706:	0f0c                                    	movi.n	a15, 0
    1708:	021e26                                    	beqi	a14, 1, 170e <de_interleave+0xc6>
    170b:	ffdf06                                    	j	168b <de_interleave+0x43>
    170e:	158476                                    	loop	a4, 1727 <de_interleave+0xdf>
    1711:	0528                                    	l32i.n	a2, a5, 0
    1713:	0329                                    	s32i.n	a2, a3, 0
    1715:	1528                                    	l32i.n	a2, a5, 4
    1717:	1329                                    	s32i.n	a2, a3, 4
    1719:	2528                                    	l32i.n	a2, a5, 8
    171b:	2329                                    	s32i.n	a2, a3, 8
    171d:	3528                                    	l32i.n	a2, a5, 12
    171f:	10c552                                    	addi	a5, a5, 16
    1722:	3329                                    	s32i.n	a2, a3, 12
    1724:	10c332                                    	addi	a3, a3, 16

00001727 <de_interleave+0xdf>:
    1727:	fff306                                    	j	16f7 <de_interleave+0xaf>

0000172a <de_interleave+0xe2>:
    172a:	f01d                                    	retw.n

0000172c <tns_data>:
    172c:	00a136                                    	entry	a1, 80
    172f:	0702f2                                    	l8ui	a15, a2, 7
    1732:	2129                                    	s32i.n	a2, a1, 8
    1734:	2128                                    	l32i.n	a2, a1, 8
    1736:	5b0c                                    	movi.n	a11, 5
    1738:	fa4ad1                                    	l32r	a13, 60 (226c <aac_getbits>)
    173b:	290c                                    	movi.n	a9, 2
    173d:	350c                                    	movi.n	a5, 3
    173f:	109f16                                    	beqz	a15, 184c <tns_data+0x120>
    1742:	010222                                    	l8ui	a2, a2, 1
    1745:	1a0c                                    	movi.n	a10, 1
    1747:	b70c                                    	movi.n	a7, 11
    1749:	7c0c                                    	movi.n	a12, 7
    174b:	f87c                                    	movi.n	a8, -1
    174d:	ffa0e2                                    	movi	a14, 255
    1750:	fec222                                    	addi	a2, a2, -2
    1753:	90a062                                    	movi	a6, 144
    1756:	83b520                                    	moveqz	a11, a5, a2
    1759:	93a920                                    	movnez	a10, a9, a2
    175c:	401b00                                    	ssl	a11
    175f:	93c720                                    	movnez	a12, a7, a2
    1762:	a1f800                                    	sll	a15, a8
    1765:	0d7d                                    	mov.n	a7, a13
    1767:	41a9                                    	s32i.n	a10, a1, 16
    1769:	70c3a2                                    	addi	a10, a3, 112
    176c:	536a                                    	add.n	a5, a3, a6
    176e:	302fe0                                    	xor	a2, a15, a14
    1771:	11a9                                    	s32i.n	a10, a1, 4
    1773:	090c                                    	movi.n	a9, 0
    1775:	81c9                                    	s32i.n	a12, a1, 32
    1777:	036d                                    	mov.n	a6, a3
    1779:	91b9                                    	s32i.n	a11, a1, 36
    177b:	04ad                                    	mov.n	a10, a4
    177d:	7129                                    	s32i.n	a2, a1, 28

0000177f <tns_data+0x53>:
    177f:	41b8                                    	l32i.n	a11, a1, 16
    1781:	5199                                    	s32i.n	a9, a1, 20
    1783:	0007e0                                    	callx8	a7
    1786:	ffa022                                    	movi	a2, 255
    1789:	0046a2                                    	s8i	a10, a6, 0
    178c:	430c                                    	movi.n	a3, 4
    178e:	0282a7                                    	bany	a2, a10, 1794 <tns_data+0x68>
    1791:	002786                                    	j	1833 <tns_data+0x107>
    1794:	fa3521                                    	l32r	a2, 68 (22b4 <aac_get1bit>)
    1797:	04ad                                    	mov.n	a10, a4
    1799:	0002e0                                    	callx8	a2
    179c:	000622                                    	l8ui	a2, a6, 0
    179f:	0846a2                                    	s8i	a10, a6, 8
    17a2:	ba9d                                    	ae_zext8	a9, a10
    17a4:	3b0c                                    	movi.n	a11, 3
    17a6:	04ad                                    	mov.n	a10, a4
    17a8:	93b390                                    	movnez	a11, a3, a9
    17ab:	084216                                    	beqz	a2, 1833 <tns_data+0x107>
    17ae:	090c                                    	movi.n	a9, 0
    17b0:	1128                                    	l32i.n	a2, a1, 4
    17b2:	61b9                                    	s32i.n	a11, a1, 24
    17b4:	a169                                    	s32i.n	a6, a1, 40
    17b6:	81b8                                    	l32i.n	a11, a1, 32
    17b8:	b199                                    	s32i.n	a9, a1, 44
    17ba:	0007e0                                    	callx8	a7
    17bd:	9138                                    	l32i.n	a3, a1, 36
    17bf:	ffa0f2                                    	movi	a15, 255
    17c2:	71e8                                    	l32i.n	a14, a1, 28
    17c4:	3129                                    	s32i.n	a2, a1, 12
    17c6:	400300                                    	ssr	a3
    17c9:	a0c232                                    	addi	a3, a2, -96
    17cc:	042d                                    	mov.n	a2, a4
    17ce:	9160a0                                    	srl	a6, a10
    17d1:	074d                                    	mov.n	a4, a7
    17d3:	107ae0                                    	and	a7, a10, a14
    17d6:	004362                                    	s8i	a6, a3, 0
    17d9:	2b0c                                    	movi.n	a11, 2
    17db:	02ad                                    	mov.n	a10, a2
    17dd:	204372                                    	s8i	a7, a3, 32
    17e0:	370f77                                    	bnone	a15, a7, 181b <tns_data+0xef>
    17e3:	0004e0                                    	callx8	a4
    17e6:	3198                                    	l32i.n	a9, a1, 12
    17e8:	200372                                    	l8ui	a7, a3, 32
    17eb:	61b8                                    	l32i.n	a11, a1, 24
    17ed:	4181a0                                    	srli	a8, a10, 1
    17f0:	e0c962                                    	addi	a6, a9, -32
    17f3:	004682                                    	s8i	a8, a6, 0
    17f6:	0460a0                                    	extui	a6, a10, 0, 1
    17f9:	004962                                    	s8i	a6, a9, 0
    17fc:	c08b60                                    	sub	a8, a11, a6
    17ff:	160c                                    	movi.n	a6, 1
    1801:	679c                                    	beqz.n	a7, 181b <tns_data+0xef>
    1803:	b87d                                    	ae_zext8	a7, a8
    1805:	02ad                                    	mov.n	a10, a2
    1807:	07bd                                    	mov.n	a11, a7
    1809:	0004e0                                    	callx8	a4
    180c:	0045a2                                    	s8i	a10, a5, 0
    180f:	551b                                    	addi.n	a5, a5, 1
    1811:	200392                                    	l8ui	a9, a3, 32
    1814:	b68d                                    	ae_zext8	a8, a6
    1816:	661b                                    	addi.n	a6, a6, 1
    1818:	e93897                                    	bltu	a8, a9, 1805 <tns_data+0xd9>
    181b:	a168                                    	l32i.n	a6, a1, 40
    181d:	3188                                    	l32i.n	a8, a1, 12
    181f:	b198                                    	l32i.n	a9, a1, 44
    1821:	047d                                    	mov.n	a7, a4
    1823:	024d                                    	mov.n	a4, a2
    1825:	000632                                    	l8ui	a3, a6, 0
    1828:	881b                                    	addi.n	a8, a8, 1
    182a:	991b                                    	addi.n	a9, a9, 1
    182c:	082d                                    	mov.n	a2, a8
    182e:	04ad                                    	mov.n	a10, a4
    1830:	823937                                    	bltu	a9, a3, 17b6 <tns_data+0x8a>
    1833:	2128                                    	l32i.n	a2, a1, 8
    1835:	661b                                    	addi.n	a6, a6, 1
    1837:	1138                                    	l32i.n	a3, a1, 4
    1839:	5198                                    	l32i.n	a9, a1, 20
    183b:	070222                                    	l8ui	a2, a2, 7
    183e:	04ad                                    	mov.n	a10, a4
    1840:	334b                                    	addi.n	a3, a3, 4
    1842:	991b                                    	addi.n	a9, a9, 1
    1844:	1139                                    	s32i.n	a3, a1, 4
    1846:	02b927                                    	bgeu	a9, a2, 184c <tns_data+0x120>
    1849:	ffcc86                                    	j	177f <tns_data+0x53>
    184c:	f01d                                    	retw.n

0000184e <tns_data+0x122>:
	...

00001850 <spectral_data>:
    1850:	00e136                                    	entry	a1, 112
    1853:	0603f2                                    	l8ui	a15, a3, 6
    1856:	8149                                    	s32i.n	a4, a1, 32
    1858:	010392                                    	l8ui	a9, a3, 1
    185b:	58a242                                    	movi	a4, 0x258
    185e:	10c382                                    	addi	a8, a3, 16
    1861:	0a2c                                    	movi.n	a10, 32
    1863:	68a172                                    	movi	a7, 0x168
    1866:	187f16                                    	beqz	a15, 19f1 <spectral_data+0x1a1>
    1869:	021222                                    	l16ui	a2, a2, 4
    186c:	fec9e2                                    	addi	a14, a9, -2
    186f:	38a4d2                                    	movi	a13, 0x438
    1872:	db1c                                    	movi.n	a11, 29
    1874:	9189                                    	s32i.n	a8, a1, 36
    1876:	c38b                                    	addi.n	a12, a3, 8
    1878:	434a                                    	add.n	a4, a3, a4
    187a:	83da                                    	add.n	a8, a3, a13
    187c:	0f0c                                    	movi.n	a15, 0
    187e:	93bae0                                    	movnez	a11, a10, a14
    1881:	737a                                    	add.n	a7, a3, a7
    1883:	412320                                    	srli	a2, a2, 3
    1886:	d0a262                                    	movi	a6, 0x2d0
    1889:	1261c2                                    	s32i	a12, a1, 72
    188c:	b149                                    	s32i.n	a4, a1, 44
    188e:	c36a                                    	add.n	a12, a3, a6
    1890:	090c                                    	movi.n	a9, 0
    1892:	a1f9                                    	s32i.n	a15, a1, 40
    1894:	07ad                                    	mov.n	a10, a7
    1896:	040c                                    	movi.n	a4, 0
    1898:	71b9                                    	s32i.n	a11, a1, 28
    189a:	106122                                    	s32i	a2, a1, 64
    189d:	d139                                    	s32i.n	a3, a1, 52
    189f:	c159                                    	s32i.n	a5, a1, 48
    18a1:	116182                                    	s32i	a8, a1, 68
    18a4:	001046                                    	j	18e9 <spectral_data+0x99>

000018a7 <spectral_data+0x57>:
    18a7:	b128                                    	l32i.n	a2, a1, 44
    18a9:	f198                                    	l32i.n	a9, a1, 60
    18ab:	d138                                    	l32i.n	a3, a1, 52
    18ad:	e1a8                                    	l32i.n	a10, a1, 56
    18af:	225a                                    	add.n	a2, a2, a5
    18b1:	c158                                    	l32i.n	a5, a1, 48
    18b3:	b129                                    	s32i.n	a2, a1, 44
    18b5:	122122                                    	l32i	a2, a1, 72
    18b8:	000382                                    	l8ui	a8, a3, 0
    18bb:	1021b2                                    	l32i	a11, a1, 64
    18be:	a1e8                                    	l32i.n	a14, a1, 40
    18c0:	050362                                    	l8ui	a6, a3, 5
    18c3:	9178                                    	l32i.n	a7, a1, 36
    18c5:	229a                                    	add.n	a2, a2, a9
    18c7:	0603d2                                    	l8ui	a13, a3, 6
    18ca:	000222                                    	l8ui	a2, a2, 0
    18cd:	90a8a0                                    	addx2	a10, a8, a10
    18d0:	907670                                    	addx2	a7, a6, a7
    18d3:	991b                                    	addi.n	a9, a9, 1
    18d5:	f72b                                    	addi.n	a15, a7, 2
    18d7:	d122b0                                    	mul16s	a2, a2, a11
    18da:	91f9                                    	s32i.n	a15, a1, 36
    18dc:	82ea                                    	add.n	a8, a2, a14
    18de:	a189                                    	s32i.n	a8, a1, 40
    18e0:	112182                                    	l32i	a8, a1, 68
    18e3:	0239d7                                    	bltu	a9, a13, 18e9 <spectral_data+0x99>
    18e6:	0041c6                                    	j	19f1 <spectral_data+0x1a1>
    18e9:	289a                                    	add.n	a2, a8, a9
    18eb:	136122                                    	s32i	a2, a1, 76
    18ee:	000222                                    	l8ui	a2, a2, 0
    18f1:	a168                                    	l32i.n	a6, a1, 40
    18f3:	9178                                    	l32i.n	a7, a1, 36
    18f5:	fbc216                                    	beqz	a2, 18b5 <spectral_data+0x65>
    18f8:	a06650                                    	addx4	a6, a6, a5
    18fb:	f199                                    	s32i.n	a9, a1, 60
    18fd:	e1a9                                    	s32i.n	a10, a1, 56
    18ff:	050c                                    	movi.n	a5, 0
    1901:	0a2d                                    	mov.n	a2, a10
    1903:	000ac6                                    	j	1932 <spectral_data+0xe2>

00001906 <spectral_data+0xb6>:
    1906:	fecc                                    	bnez.n	a14, 1919 <spectral_data+0xc9>
    1908:	f9ca81                                    	l32r	a8, 30 (3878 <memset>)
    190b:	11c3e0                                    	slli	a12, a3, 2
    190e:	0b0c                                    	movi.n	a11, 0
    1910:	06ad                                    	mov.n	a10, a6
    1912:	0008e0                                    	callx8	a8
    1915:	5198                                    	l32i.n	a9, a1, 20
    1917:	61c8                                    	l32i.n	a12, a1, 24
    1919:	132182                                    	l32i	a8, a1, 76
    191c:	551b                                    	addi.n	a5, a5, 1
    191e:	902920                                    	addx2	a2, a9, a2
    1921:	907970                                    	addx2	a7, a9, a7
    1924:	cc2b                                    	addi.n	a12, a12, 2
    1926:	a06360                                    	addx4	a6, a3, a6
    1929:	000882                                    	l8ui	a8, a8, 0
    192c:	023587                                    	bltu	a5, a8, 1932 <spectral_data+0xe2>
    192f:	ffdd06                                    	j	18a7 <spectral_data+0x57>
    1932:	4c3d                                    	ae_l16ui.n	a3, a12, 0
    1934:	91a8                                    	l32i.n	a10, a1, 36
    1936:	418830                                    	srli	a8, a3, 8
    1939:	00a4b2                                    	movi	a11, 0x400
    193c:	b39d                                    	ae_zext8	a9, a3
    193e:	90f8a0                                    	addx2	a15, a8, a10
    1941:	90a9a0                                    	addx2	a10, a9, a10
    1944:	4f3d                                    	ae_l16ui.n	a3, a15, 0
    1946:	c09890                                    	sub	a9, a8, a9
    1949:	4aad                                    	ae_l16ui.n	a10, a10, 0
    194b:	c033a0                                    	sub	a3, a3, a10
    194e:	b1a8                                    	l32i.n	a10, a1, 44
    1950:	434a                                    	add.n	a4, a3, a4
    1952:	09f396                                    	bltz	a3, 19f5 <spectral_data+0x1a5>
    1955:	02ab47                                    	bge	a11, a4, 195b <spectral_data+0x10b>
    1958:	002746                                    	j	19f9 <spectral_data+0x1a9>
    195b:	aa5a                                    	add.n	a10, a10, a5
    195d:	61c9                                    	s32i.n	a12, a1, 24
    195f:	000ae2                                    	l8ui	a14, a10, 0
    1962:	5199                                    	s32i.n	a9, a1, 20
    1964:	71d8                                    	l32i.n	a13, a1, 28
    1966:	07bd                                    	mov.n	a11, a7
    1968:	81f8                                    	l32i.n	a15, a1, 32
    196a:	f9ce82                                    	addi	a8, a14, -7
    196d:	197ea6                                    	blti	a14, 7, 198a <spectral_data+0x13a>
    1970:	f3cea2                                    	addi	a10, a14, -13
    1973:	3748b6                                    	bltui	a8, 4, 19ae <spectral_data+0x15e>
    1976:	b80c                                    	movi.n	a8, 11
    1978:	8c3ab6                                    	bltui	a10, 3, 1908 <spectral_data+0xb8>
    197b:	02ad                                    	mov.n	a10, a2
    197d:	989e87                                    	bne	a14, a8, 1919 <spectral_data+0xc9>
    1980:	f9d181                                    	l32r	a8, c4 (2838 <dehuffman_cb11>)
    1983:	06cd                                    	mov.n	a12, a6
    1985:	09ed                                    	mov.n	a14, a9
    1987:	001806                                    	j	19eb <spectral_data+0x19b>

0000198a <spectral_data+0x13a>:
    198a:	fdce82                                    	addi	a8, a14, -3
    198d:	2f3ea6                                    	blti	a14, 3, 19c0 <spectral_data+0x170>
    1990:	fbcea2                                    	addi	a10, a14, -5
    1993:	3f28b6                                    	bltui	a8, 2, 19d6 <spectral_data+0x186>
    1996:	8188                                    	l32i.n	a8, a1, 32
    1998:	022ab6                                    	bltui	a10, 2, 199e <spectral_data+0x14e>
    199b:	ffde86                                    	j	1919 <spectral_data+0xc9>
    199e:	3d03a1400081118e                  	{ s32i	a8, a1, 0; l32i	a13, a1, 28 }
    19a6:	51f8                                    	l32i.n	a15, a1, 20
    19a8:	f9c681                                    	l32r	a8, c0 (259c <dehuffman_cb5_cb6>)
    19ab:	000d86                                    	j	19e5 <spectral_data+0x195>

000019ae <spectral_data+0x15e>:
    19ae:	8188                                    	l32i.n	a8, a1, 32
    19b0:	3d03a1400081118e                  	{ s32i	a8, a1, 0; l32i	a13, a1, 28 }
    19b8:	51f8                                    	l32i.n	a15, a1, 20
    19ba:	f9c581                                    	l32r	a8, d0 (26a8 <dehuffman_cb7_8_9_10>)
    19bd:	000906                                    	j	19e5 <spectral_data+0x195>

000019c0 <spectral_data+0x170>:
    19c0:	81a8                                    	l32i.n	a10, a1, 32
    19c2:	8e0b                                    	addi.n	a8, a14, -1
    19c4:	71d8                                    	l32i.n	a13, a1, 28
    19c6:	0228b6                                    	bltui	a8, 2, 19cc <spectral_data+0x17c>
    19c9:	ffce46                                    	j	1906 <spectral_data+0xb6>
    19cc:	01a9                                    	s32i.n	a10, a1, 0
    19ce:	51f8                                    	l32i.n	a15, a1, 20
    19d0:	f9be81                                    	l32r	a8, c8 (23a4 <dehuffman_cb1_cb2>)
    19d3:	000386                                    	j	19e5 <spectral_data+0x195>

000019d6 <spectral_data+0x186>:
    19d6:	8188                                    	l32i.n	a8, a1, 32
    19d8:	3d03a1400081118e                  	{ s32i	a8, a1, 0; l32i	a13, a1, 28 }
    19e0:	51f8                                    	l32i.n	a15, a1, 20
    19e2:	f9ba81                                    	l32r	a8, cc (2458 <dehuffman_cb3_cb4>)
    19e5:	02ad                                    	mov.n	a10, a2
    19e7:	07bd                                    	mov.n	a11, a7
    19e9:	06cd                                    	mov.n	a12, a6
    19eb:	0008e0                                    	callx8	a8
    19ee:	ffc8c6                                    	j	1915 <spectral_data+0xc5>

000019f1 <spectral_data+0x1a1>:
    19f1:	020c                                    	movi.n	a2, 0
    19f3:	f01d                                    	retw.n

000019f5 <spectral_data+0x1a5>:
    19f5:	725c                                    	movi.n	a2, 87
    19f7:	f01d                                    	retw.n

000019f9 <spectral_data+0x1a9>:
    19f9:	625c                                    	movi.n	a2, 86
    19fb:	f01d                                    	retw.n

000019fd <spectral_data+0x1ad>:
    19fd:	000000                                        ...

00001a00 <decode_scale_factors>:
    1a00:	008136                                    	entry	a1, 64
    1a03:	060252                                    	l8ui	a5, a2, 6
    1a06:	160c                                    	movi.n	a6, 1
    1a08:	0402a2                                    	l8ui	a10, a2, 4
    1a0b:	070c                                    	movi.n	a7, 0
    1a0d:	000282                                    	l8ui	a8, a2, 0
    1a10:	c0a342                                    	movi	a4, 0x3c0
    1a13:	68a192                                    	movi	a9, 0x168
    1a16:	0a6516                                    	beqz	a5, 1ac0 <decode_scale_factors+0xc0>
    1a19:	7169                                    	s32i.n	a6, a1, 28
    1a1b:	a6caf2                                    	addi	a15, a10, -90
    1a1e:	5179                                    	s32i.n	a7, a1, 20
    1a20:	724a                                    	add.n	a7, a2, a4
    1a22:	429a                                    	add.n	a4, a2, a9
    1a24:	060c                                    	movi.n	a6, 0
    1a26:	ffa0b2                                    	movi	a11, 255
    1a29:	61a9                                    	s32i.n	a10, a1, 24
    1a2b:	41f9                                    	s32i.n	a15, a1, 16
    1a2d:	090c                                    	movi.n	a9, 0
    1a2f:	000506                                    	j	1a47 <decode_scale_factors+0x47>

00001a32 <decode_scale_factors+0x32>:
    1a32:	060252                                    	l8ui	a5, a2, 6
    1a35:	51a8                                    	l32i.n	a10, a1, 20
    1a37:	779a                                    	add.n	a7, a7, a9
    1a39:	098d                                    	mov.n	a8, a9
    1a3b:	ffa0b2                                    	movi	a11, 255
    1a3e:	aa1b                                    	addi.n	a10, a10, 1
    1a40:	090c                                    	movi.n	a9, 0
    1a42:	51a9                                    	s32i.n	a10, a1, 20
    1a44:	78ba57                                    	bgeu	a10, a5, 1ac0 <decode_scale_factors+0xc0>
    1a47:	ea0b87                                    	bnone	a11, a8, 1a35 <decode_scale_factors+0x35>
    1a4a:	050c                                    	movi.n	a5, 0
    1a4c:	000746                                    	j	1a6d <decode_scale_factors+0x6d>

00001a4f <decode_scale_factors+0x4f>:
    1a4f:	f9a181                                    	l32r	a8, d4 (2378 <dehuffman_sf>)
    1a52:	03ad                                    	mov.n	a10, a3
    1a54:	0008e0                                    	callx8	a8
    1a57:	4188                                    	l32i.n	a8, a1, 16
    1a59:	090c                                    	movi.n	a9, 0
    1a5b:	7199                                    	s32i.n	a9, a1, 28
    1a5d:	8a8a                                    	add.n	a8, a10, a8
    1a5f:	4189                                    	s32i.n	a8, a1, 16
    1a61:	000292                                    	l8ui	a9, a2, 0
    1a64:	551b                                    	addi.n	a5, a5, 1
    1a66:	648d                                    	ae_s16i.n	a8, a4, 0
    1a68:	442b                                    	addi.n	a4, a4, 2
    1a6a:	c4b597                                    	bgeu	a5, a9, 1a32 <decode_scale_factors+0x32>
    1a6d:	875a                                    	add.n	a8, a7, a5
    1a6f:	da0c                                    	movi.n	a10, 13
    1a71:	000892                                    	l8ui	a9, a8, 0
    1a74:	71b8                                    	l32i.n	a11, a1, 28
    1a76:	f2c982                                    	addi	a8, a9, -14
    1a79:	1e28b6                                    	bltui	a8, 2, 1a9b <decode_scale_factors+0x9b>
    1a7c:	080c                                    	movi.n	a8, 0
    1a7e:	fdf916                                    	beqz	a9, 1a61 <decode_scale_factors+0x61>
    1a81:	080c                                    	movi.n	a8, 0
    1a83:	2399a7                                    	bne	a9, a10, 1aaa <decode_scale_factors+0xaa>
    1a86:	03ad                                    	mov.n	a10, a3
    1a88:	fc3b16                                    	beqz	a11, 1a4f <decode_scale_factors+0x4f>
    1a8b:	f97581                                    	l32r	a8, 60 (226c <aac_getbits>)
    1a8e:	9b0c                                    	movi.n	a11, 9
    1a90:	0008e0                                    	callx8	a8
    1a93:	8a8d                                    	ae_sext16	a8, a10
    1a95:	ffd8a2                                    	addmi	a10, a8, 0xffffff00
    1a98:	ffeec6                                    	j	1a57 <decode_scale_factors+0x57>

00001a9b <decode_scale_factors+0x9b>:
    1a9b:	f98e81                                    	l32r	a8, d4 (2378 <dehuffman_sf>)
    1a9e:	03ad                                    	mov.n	a10, a3
    1aa0:	0008e0                                    	callx8	a8
    1aa3:	8a6a                                    	add.n	a8, a10, a6
    1aa5:	086d                                    	mov.n	a6, a8
    1aa7:	ffed86                                    	j	1a61 <decode_scale_factors+0x61>

00001aaa <decode_scale_factors+0xaa>:
    1aaa:	648d                                    	ae_s16i.n	a8, a4, 0
    1aac:	03ad                                    	mov.n	a10, a3
    1aae:	f98981                                    	l32r	a8, d4 (2378 <dehuffman_sf>)
    1ab1:	0008e0                                    	callx8	a8
    1ab4:	6188                                    	l32i.n	a8, a1, 24
    1ab6:	8a8a                                    	add.n	a8, a10, a8
    1ab8:	08f8f6                                    	bgeui	a8, 0x100, 1ac4 <decode_scale_factors+0xc4>
    1abb:	6189                                    	s32i.n	a8, a1, 24
    1abd:	ffe806                                    	j	1a61 <decode_scale_factors+0x61>

00001ac0 <decode_scale_factors+0xc0>:
    1ac0:	020c                                    	movi.n	a2, 0
    1ac2:	f01d                                    	retw.n

00001ac4 <decode_scale_factors+0xc4>:
    1ac4:	68a022                                    	movi	a2, 104
    1ac7:	f01d                                    	retw.n

00001ac9 <decode_scale_factors+0xc9>:
    1ac9:	000000                                        ...

00001acc <loas_get_value>:
    1acc:	004136                                    	entry	a1, 32
    1acf:	f96441                                    	l32r	a4, 60 (226c <aac_getbits>)
    1ad2:	2b0c                                    	movi.n	a11, 2
    1ad4:	02ad                                    	mov.n	a10, a2
    1ad6:	023d                                    	mov.n	a3, a2
    1ad8:	0004e0                                    	callx8	a4
    1adb:	ba5d                                    	ae_zext8	a5, a10
    1add:	020c                                    	movi.n	a2, 0
    1adf:	551b                                    	addi.n	a5, a5, 1

00001ae1 <loas_get_value+0x15>:
    1ae1:	8b0c                                    	movi.n	a11, 8
    1ae3:	03ad                                    	mov.n	a10, a3
    1ae5:	112280                                    	slli	a2, a2, 8
    1ae8:	550b                                    	addi.n	a5, a5, -1
    1aea:	0004e0                                    	callx8	a4
    1aed:	ba6d                                    	ae_zext8	a6, a10
    1aef:	262a                                    	add.n	a2, a6, a2
    1af1:	fec556                                    	bnez	a5, 1ae1 <loas_get_value+0x15>
    1af4:	f01d                                    	retw.n

00001af6 <loas_get_value+0x2a>:
	...

00001af8 <aac_loas_frame>:
    1af8:	010136                                    	entry	a1, 128
    1afb:	ff7c                                    	movi.n	a15, -1
    1afd:	7159                                    	s32i.n	a5, a1, 28
    1aff:	f450f0                                    	extui	a5, a15, 0, 16
    1b02:	a149                                    	s32i.n	a4, a1, 40
    1b04:	645d                                    	ae_s16i.n	a5, a4, 0
    1b06:	03ad                                    	mov.n	a10, a3
    1b08:	f95141                                    	l32r	a4, 4c (22f4 <aac_get_processed_bits>)
    1b0b:	0004e0                                    	callx8	a4
    1b0e:	f94d41                                    	l32r	a4, 44 (2360 <aac_used_bytes>)
    1b11:	03ad                                    	mov.n	a10, a3
    1b13:	0004e0                                    	callx8	a4
    1b16:	f47c                                    	movi.n	a4, -1
    1b18:	2358                                    	l32i.n	a5, a3, 8
    1b1a:	8149                                    	s32i.n	a4, a1, 32
    1b1c:	024d                                    	mov.n	a4, a2
    1b1e:	71e8                                    	l32i.n	a14, a1, 28
    1b20:	f2cb                                    	addi.n	a15, a2, 12
    1b22:	ffa062                                    	movi	a6, 255
    1b25:	022a57                                    	blt	a10, a5, 1b2b <aac_loas_frame+0x33>
    1b28:	0099c6                                    	j	1d93 <aac_loas_frame+0x29b>
    1b2b:	f94d51                                    	l32r	a5, 60 (226c <aac_getbits>)
    1b2e:	112ed0                                    	slli	a2, a14, 3
    1b31:	91f9                                    	s32i.n	a15, a1, 36
    1b33:	b129                                    	s32i.n	a2, a1, 44

00001b35 <aac_loas_frame+0x3d>:
    1b35:	f94721                                    	l32r	a2, 54 (22d0 <aac_byte_align>)
    1b38:	03ad                                    	mov.n	a10, a3
    1b3a:	0002e0                                    	callx8	a2
    1b3d:	f94321                                    	l32r	a2, 4c (22f4 <aac_get_processed_bits>)
    1b40:	03ad                                    	mov.n	a10, a3
    1b42:	0002e0                                    	callx8	a2
    1b45:	0a2d                                    	mov.n	a2, a10
    1b47:	1b0c                                    	movi.n	a11, 1
    1b49:	03ad                                    	mov.n	a10, a3
    1b4b:	070c                                    	movi.n	a7, 0
    1b4d:	0005e0                                    	callx8	a5
    1b50:	0444a2                                    	s8i	a10, a4, 4
    1b53:	c129                                    	s32i.n	a2, a1, 48
    1b55:	2c06a7                                    	bnone	a6, a10, 1b85 <aac_loas_frame+0x8d>
    1b58:	000422                                    	l8ui	a2, a4, 0
    1b5b:	040216                                    	beqz	a2, 1b9f <aac_loas_frame+0xa7>
    1b5e:	030422                                    	l8ui	a2, a4, 3
    1b61:	070c                                    	movi.n	a7, 0
    1b63:	f93f51                                    	l32r	a5, 60 (226c <aac_getbits>)
    1b66:	ffa062                                    	movi	a6, 255
    1b69:	021266                                    	bnei	a2, 1, 1b6f <aac_loas_frame+0x77>
    1b6c:	004e46                                    	j	1ca9 <aac_loas_frame+0x1b1>
    1b6f:	139256                                    	bnez	a2, 1cac <aac_loas_frame+0x1b4>
    1b72:	070c                                    	movi.n	a7, 0
    1b74:	8b0c                                    	movi.n	a11, 8
    1b76:	03ad                                    	mov.n	a10, a3
    1b78:	0005e0                                    	callx8	a5
    1b7b:	ba2d                                    	ae_zext8	a2, a10
    1b7d:	727a                                    	add.n	a7, a2, a7
    1b7f:	f14a67                                    	ball	a10, a6, 1b74 <aac_loas_frame+0x7c>
    1b82:	004986                                    	j	1cac <aac_loas_frame+0x1b4>

00001b85 <aac_loas_frame+0x8d>:
    1b85:	1b0c                                    	movi.n	a11, 1
    1b87:	03ad                                    	mov.n	a10, a3
    1b89:	0005e0                                    	callx8	a5
    1b8c:	0144a2                                    	s8i	a10, a4, 1
    1b8f:	0f06a7                                    	bnone	a6, a10, 1ba2 <aac_loas_frame+0xaa>
    1b92:	1b0c                                    	movi.n	a11, 1
    1b94:	03ad                                    	mov.n	a10, a3
    1b96:	0005e0                                    	callx8	a5
    1b99:	0244a2                                    	s8i	a10, a4, 2
    1b9c:	000146                                    	j	1ba5 <aac_loas_frame+0xad>

00001b9f <aac_loas_frame+0xa7>:
    1b9f:	004246                                    	j	1cac <aac_loas_frame+0x1b4>

00001ba2 <aac_loas_frame+0xaa>:
    1ba2:	0204a2                                    	l8ui	a10, a4, 2
    1ba5:	ffa022                                    	movi	a2, 255
    1ba8:	070c                                    	movi.n	a7, 0
    1baa:	0202a7                                    	bnone	a2, a10, 1bb0 <aac_loas_frame+0xb8>
    1bad:	003ec6                                    	j	1cac <aac_loas_frame+0x1b4>
    1bb0:	010422                                    	l8ui	a2, a4, 1
    1bb3:	03ad                                    	mov.n	a10, a3
    1bb5:	428c                                    	beqz.n	a2, 1bbd <aac_loas_frame+0xc5>
    1bb7:	f94821                                    	l32r	a2, d8 (1acc <loas_get_value>)
    1bba:	0002e0                                    	callx8	a2
    1bbd:	f92861                                    	l32r	a6, 60 (226c <aac_getbits>)
    1bc0:	1b0c                                    	movi.n	a11, 1
    1bc2:	03ad                                    	mov.n	a10, a3
    1bc4:	0006e0                                    	callx8	a6
    1bc7:	51a9                                    	s32i.n	a10, a1, 20
    1bc9:	6b0c                                    	movi.n	a11, 6
    1bcb:	03ad                                    	mov.n	a10, a3
    1bcd:	0006e0                                    	callx8	a6
    1bd0:	0a5d                                    	mov.n	a5, a10
    1bd2:	4b0c                                    	movi.n	a11, 4
    1bd4:	03ad                                    	mov.n	a10, a3
    1bd6:	551b                                    	addi.n	a5, a5, 1
    1bd8:	0006e0                                    	callx8	a6
    1bdb:	0a2d                                    	mov.n	a2, a10
    1bdd:	3b0c                                    	movi.n	a11, 3
    1bdf:	03ad                                    	mov.n	a10, a3
    1be1:	221b                                    	addi.n	a2, a2, 1
    1be3:	0006e0                                    	callx8	a6
    1be6:	6a1b                                    	addi.n	a6, a10, 1
    1be8:	0226a6                                    	blti	a6, 2, 1bee <aac_loas_frame+0xf6>
    1beb:	002f46                                    	j	1cac <aac_loas_frame+0x1b4>
    1bee:	5168                                    	l32i.n	a6, a1, 20
    1bf0:	0225a6                                    	blti	a5, 2, 1bf6 <aac_loas_frame+0xfe>
    1bf3:	002d46                                    	j	1cac <aac_loas_frame+0x1b4>
    1bf6:	03ad                                    	mov.n	a10, a3
    1bf8:	0b0616                                    	beqz	a6, 1cac <aac_loas_frame+0x1b4>
    1bfb:	060c                                    	movi.n	a6, 0
    1bfd:	0222a6                                    	blti	a2, 2, 1c03 <aac_loas_frame+0x10b>
    1c00:	002a06                                    	j	1cac <aac_loas_frame+0x1b4>
    1c03:	010422                                    	l8ui	a2, a4, 1
    1c06:	070c                                    	movi.n	a7, 0
    1c08:	628c                                    	beqz.n	a2, 1c12 <aac_loas_frame+0x11a>
    1c0a:	f93321                                    	l32r	a2, d8 (1acc <loas_get_value>)
    1c0d:	0002e0                                    	callx8	a2
    1c10:	0a6d                                    	mov.n	a6, a10
    1c12:	f90e21                                    	l32r	a2, 4c (22f4 <aac_get_processed_bits>)
    1c15:	03ad                                    	mov.n	a10, a3
    1c17:	0002e0                                    	callx8	a2
    1c1a:	f90f51                                    	l32r	a5, 58 (1d98 <aac_specific_config>)
    1c1d:	0a2d                                    	mov.n	a2, a10
    1c1f:	48c1b2                                    	addi	a11, a1, 72
    1c22:	34c1c2                                    	addi	a12, a1, 52
    1c25:	03ad                                    	mov.n	a10, a3
    1c27:	0005e0                                    	callx8	a5
    1c2a:	07ea96                                    	bltz	a10, 1cac <aac_loas_frame+0x1b4>
    1c2d:	f90751                                    	l32r	a5, 4c (22f4 <aac_get_processed_bits>)
    1c30:	03ad                                    	mov.n	a10, a3
    1c32:	6169                                    	s32i.n	a6, a1, 24
    1c34:	0005e0                                    	callx8	a5
    1c37:	c06a20                                    	sub	a6, a10, a2
    1c3a:	00a252                                    	movi	a5, 0x200
    1c3d:	02bd                                    	mov.n	a11, a2
    1c3f:	5169                                    	s32i.n	a6, a1, 20
    1c41:	03ad                                    	mov.n	a10, a3
    1c43:	2c3567                                    	bltu	a5, a6, 1c73 <aac_loas_frame+0x17b>
    1c46:	f90221                                    	l32r	a2, 50 (230c <aac_resetbits>)
    1c49:	0002e0                                    	callx8	a2
    1c4c:	5128                                    	l32i.n	a2, a1, 20
    1c4e:	880c                                    	movi.n	a8, 8
    1c50:	136422                                    	s32i	a2, a4, 76
    1c53:	9158                                    	l32i.n	a5, a1, 36
    1c55:	5168                                    	l32i.n	a6, a1, 20
    1c57:	829c                                    	beqz.n	a2, 1c73 <aac_loas_frame+0x17b>
    1c59:	432680                                    	min	a2, a6, a8
    1c5c:	f90181                                    	l32r	a8, 60 (226c <aac_getbits>)
    1c5f:	03ad                                    	mov.n	a10, a3
    1c61:	02bd                                    	mov.n	a11, a2
    1c63:	0008e0                                    	callx8	a8
    1c66:	c06620                                    	sub	a6, a6, a2
    1c69:	0045a2                                    	s8i	a10, a5, 0
    1c6c:	880c                                    	movi.n	a8, 8
    1c6e:	551b                                    	addi.n	a5, a5, 1
    1c70:	fe5656                                    	bnez	a6, 1c59 <aac_loas_frame+0x161>
    1c73:	6128                                    	l32i.n	a2, a1, 24
    1c75:	03ad                                    	mov.n	a10, a3
    1c77:	5168                                    	l32i.n	a6, a1, 20
    1c79:	f8f951                                    	l32r	a5, 60 (226c <aac_getbits>)
    1c7c:	c08260                                    	sub	a8, a2, a6
    1c7f:	04b627                                    	bgeu	a6, a2, 1c87 <aac_loas_frame+0x18f>
    1c82:	88bd                                    	ae_sext16	a11, a8
    1c84:	0005e0                                    	callx8	a5
    1c87:	3b0c                                    	movi.n	a11, 3
    1c89:	03ad                                    	mov.n	a10, a3
    1c8b:	0005e0                                    	callx8	a5
    1c8e:	ba2d                                    	ae_zext8	a2, a10
    1c90:	0344a2                                    	s8i	a10, a4, 3
    1c93:	8b0c                                    	movi.n	a11, 8
    1c95:	03ad                                    	mov.n	a10, a3
    1c97:	751226                                    	beqi	a2, 1, 1d10 <aac_loas_frame+0x218>
    1c9a:	056d                                    	mov.n	a6, a5
    1c9c:	c2cc                                    	bnez.n	a2, 1cac <aac_loas_frame+0x1b4>
    1c9e:	020c                                    	movi.n	a2, 0
    1ca0:	035422                                    	s16i	a2, a4, 6
    1ca3:	0005e0                                    	callx8	a5
    1ca6:	002146                                    	j	1d2f <aac_loas_frame+0x237>

00001ca9 <aac_loas_frame+0x1b1>:
    1ca9:	031472                                    	l16ui	a7, a4, 6
    1cac:	f8e821                                    	l32r	a2, 4c (22f4 <aac_get_processed_bits>)
    1caf:	03ad                                    	mov.n	a10, a3
    1cb1:	0002e0                                    	callx8	a2
    1cb4:	67ee64                                    	rur.ae_bitptr	a6
    1cb7:	b1f8                                    	l32i.n	a15, a1, 44
    1cb9:	5169                                    	s32i.n	a6, a1, 20
    1cbb:	0328                                    	l32i.n	a2, a3, 0
    1cbd:	c168                                    	l32i.n	a6, a1, 48
    1cbf:	1388                                    	l32i.n	a8, a3, 4
    1cc1:	0a5d                                    	mov.n	a5, a10
    1cc3:	3189                                    	s32i.n	a8, a1, 12
    1cc5:	2129                                    	s32i.n	a2, a1, 8
    1cc7:	a18b                                    	addi.n	a10, a1, 8
    1cc9:	86ed                                    	ae_sext16	a14, a6
    1ccb:	f8e181                                    	l32r	a8, 50 (230c <aac_resetbits>)
    1cce:	2efa                                    	add.n	a2, a14, a15
    1cd0:	e3cf10                                    	rur.ae_bithead	a12
    1cd3:	23d8                                    	l32i.n	a13, a3, 8
    1cd5:	02bd                                    	mov.n	a11, a2
    1cd7:	61c9                                    	s32i.n	a12, a1, 24
    1cd9:	41d9                                    	s32i.n	a13, a1, 16
    1cdb:	0008e0                                    	callx8	a8
    1cde:	5188                                    	l32i.n	a8, a1, 20
    1ce0:	61f8                                    	l32i.n	a15, a1, 24
    1ce2:	21b8                                    	l32i.n	a11, a1, 8
    1ce4:	67ef84                                    	wur.ae_bitptr	a8
    1ce7:	f3f1f0                                    	wur.ae_bithead	a15
    1cea:	4198                                    	l32i.n	a9, a1, 16
    1cec:	090756                                    	bnez	a7, 1d80 <aac_loas_frame+0x288>
    1cef:	31e8                                    	l32i.n	a14, a1, 12
    1cf1:	2399                                    	s32i.n	a9, a3, 8
    1cf3:	f8d421                                    	l32r	a2, 44 (2360 <aac_used_bytes>)
    1cf6:	03ad                                    	mov.n	a10, a3
    1cf8:	13e9                                    	s32i.n	a14, a3, 4
    1cfa:	ffa062                                    	movi	a6, 255
    1cfd:	03b9                                    	s32i.n	a11, a3, 0
    1cff:	0002e0                                    	callx8	a2
    1d02:	2328                                    	l32i.n	a2, a3, 8
    1d04:	f8d751                                    	l32r	a5, 60 (226c <aac_getbits>)
    1d07:	02aa27                                    	bge	a10, a2, 1d0d <aac_loas_frame+0x215>
    1d0a:	ff89c6                                    	j	1b35 <aac_loas_frame+0x3d>
    1d0d:	002086                                    	j	1d93 <aac_loas_frame+0x29b>

00001d10 <aac_loas_frame+0x218>:
    1d10:	9b0c                                    	movi.n	a11, 9
    1d12:	03ad                                    	mov.n	a10, a3
    1d14:	0005e0                                    	callx8	a5
    1d17:	f27c                                    	movi.n	a2, -1
    1d19:	0354a2                                    	s16i	a10, a4, 6
    1d1c:	f42020                                    	extui	a2, a2, 0, 16
    1d1f:	14ca82                                    	addi	a8, a10, 20
    1d22:	056d                                    	mov.n	a6, a5
    1d24:	8402a7                                    	bnone	a2, a10, 1cac <aac_loas_frame+0x1b4>
    1d27:	1128d0                                    	slli	a2, a8, 3
    1d2a:	035422                                    	s16i	a2, a4, 6
    1d2d:	020c                                    	movi.n	a2, 0
    1d2f:	1b0c                                    	movi.n	a11, 1
    1d31:	03ad                                    	mov.n	a10, a3
    1d33:	2429                                    	s32i.n	a2, a4, 8
    1d35:	0006e0                                    	callx8	a6
    1d38:	caac                                    	beqz.n	a10, 1d68 <aac_loas_frame+0x270>
    1d3a:	010422                                    	l8ui	a2, a4, 1
    1d3d:	03ad                                    	mov.n	a10, a3
    1d3f:	928c                                    	beqz.n	a2, 1d4c <aac_loas_frame+0x254>
    1d41:	f8e521                                    	l32r	a2, d8 (1acc <loas_get_value>)
    1d44:	0002e0                                    	callx8	a2
    1d47:	24a9                                    	s32i.n	a10, a4, 8
    1d49:	0006c6                                    	j	1d68 <aac_loas_frame+0x270>

00001d4c <aac_loas_frame+0x254>:
    1d4c:	1b0c                                    	movi.n	a11, 1
    1d4e:	03ad                                    	mov.n	a10, a3
    1d50:	0006e0                                    	callx8	a6
    1d53:	0a2d                                    	mov.n	a2, a10
    1d55:	8b0c                                    	movi.n	a11, 8
    1d57:	03ad                                    	mov.n	a10, a3
    1d59:	0006e0                                    	callx8	a6
    1d5c:	2458                                    	l32i.n	a5, a4, 8
    1d5e:	115580                                    	slli	a5, a5, 8
    1d61:	55aa                                    	add.n	a5, a5, a10
    1d63:	2459                                    	s32i.n	a5, a4, 8
    1d65:	fe3256                                    	bnez	a2, 1d4c <aac_loas_frame+0x254>
    1d68:	1b0c                                    	movi.n	a11, 1
    1d6a:	03ad                                    	mov.n	a10, a3
    1d6c:	0006e0                                    	callx8	a6
    1d6f:	5a8c                                    	beqz.n	a10, 1d78 <aac_loas_frame+0x280>
    1d71:	8b0c                                    	movi.n	a11, 8
    1d73:	03ad                                    	mov.n	a10, a3
    1d75:	0006e0                                    	callx8	a6
    1d78:	120c                                    	movi.n	a2, 1
    1d7a:	004422                                    	s8i	a2, a4, 0
    1d7d:	ff7746                                    	j	1b5e <aac_loas_frame+0x66>

00001d80 <aac_loas_frame+0x288>:
    1d80:	7148                                    	l32i.n	a4, a1, 28
    1d82:	113600                                    	slli	a3, a6, 16
    1d85:	a1f8                                    	l32i.n	a15, a1, 40
    1d87:	313330                                    	srai	a3, a3, 19
    1d8a:	c02250                                    	sub	a2, a2, a5
    1d8d:	334a                                    	add.n	a3, a3, a4
    1d8f:	8129                                    	s32i.n	a2, a1, 32
    1d91:	6f3d                                    	ae_s16i.n	a3, a15, 0
    1d93:	8128                                    	l32i.n	a2, a1, 32
    1d95:	f01d                                    	retw.n

00001d97 <aac_loas_frame+0x29f>:
	...

00001d98 <aac_specific_config>:
    1d98:	008136                                    	entry	a1, 64
    1d9b:	025d                                    	mov.n	a5, a2
    1d9d:	0c3316                                    	beqz	a3, 1e64 <aac_specific_config+0xcc>
    1da0:	f8a421                                    	l32r	a2, 30 (3878 <memset>)
    1da3:	0b0c                                    	movi.n	a11, 0
    1da5:	8c1c                                    	movi.n	a12, 24
    1da7:	03ad                                    	mov.n	a10, a3
    1da9:	070c                                    	movi.n	a7, 0
    1dab:	0002e0                                    	callx8	a2
    1dae:	f8ac61                                    	l32r	a6, 60 (226c <aac_getbits>)
    1db1:	db0c                                    	movi.n	a11, 13
    1db3:	05ad                                    	mov.n	a10, a5
    1db5:	0006e0                                    	callx8	a6
    1db8:	4128a0                                    	srli	a2, a10, 8
    1dbb:	3494a0                                    	extui	a9, a10, 4, 4
    1dbe:	3480a0                                    	extui	a8, a10, 0, 4
    1dc1:	004322                                    	s8i	a2, a3, 0
    1dc4:	014392                                    	s8i	a9, a3, 1
    1dc7:	084382                                    	s8i	a8, a3, 8
    1dca:	07a9f6                                    	bgeui	a9, 12, 1dd5 <aac_specific_config+0x3d>
    1dcd:	f89a71                                    	l32r	a7, 38 (63e0 <sample_rates>)
    1dd0:	a07970                                    	addx4	a7, a9, a7
    1dd3:	0778                                    	l32i.n	a7, a7, 0

00001dd5 <aac_specific_config+0x3d>:
    1dd5:	f8c1a1                                    	l32r	a10, dc (45c0 <object_types>)
    1dd8:	b29d                                    	ae_zext8	a9, a2
    1dda:	1379                                    	s32i.n	a7, a3, 4
    1ddc:	2a9a                                    	add.n	a2, a10, a9
    1dde:	2b0c                                    	movi.n	a11, 2
    1de0:	0002a2                                    	l8ui	a10, a2, 0
    1de3:	f27c                                    	movi.n	a2, -1
    1de5:	791a66                                    	bnei	a10, 1, 1e62 <aac_specific_config+0xca>
    1de8:	d27c                                    	movi.n	a2, -3
    1dea:	07a716                                    	beqz	a7, 1e68 <aac_specific_config+0xd0>
    1ded:	f77c                                    	movi.n	a7, -1
    1def:	6f88f6                                    	bgeui	a8, 8, 1e62 <aac_specific_config+0xca>
    1df2:	017760                                    	slli	a7, a7, 26
    1df5:	6689f6                                    	bgeui	a9, 8, 1e5f <aac_specific_config+0xc7>
    1df8:	dea022                                    	movi	a2, 222
    1dfb:	05ad                                    	mov.n	a10, a5
    1dfd:	5e5297                                    	bbc	a2, a9, 1e5f <aac_specific_config+0xc7>
    1e00:	0006e0                                    	callx8	a6
    1e03:	d27c                                    	movi.n	a2, -3
    1e05:	fea172                                    	movi	a7, 0x1fe
    1e08:	012280                                    	slli	a2, a2, 24
    1e0b:	4181a0                                    	srli	a8, a10, 1
    1e0e:	109a70                                    	and	a9, a10, a7
    1e11:	027d                                    	mov.n	a7, a2
    1e13:	04b0a0                                    	extui	a11, a10, 0, 1
    1e16:	094382                                    	s8i	a8, a3, 9
    1e19:	422926                                    	beqi	a9, 2, 1e5f <aac_specific_config+0xc7>
    1e1c:	0a43b2                                    	s8i	a11, a3, 10
    1e1f:	eb0c                                    	movi.n	a11, 14
    1e21:	076a07                                    	bbci	a10, 0, 1e2c <aac_specific_config+0x94>
    1e24:	05ad                                    	mov.n	a10, a5
    1e26:	0006e0                                    	callx8	a6
    1e29:	0653a2                                    	s16i	a10, a3, 12
    1e2c:	f88f61                                    	l32r	a6, 68 (22b4 <aac_get1bit>)
    1e2f:	05ad                                    	mov.n	a10, a5
    1e31:	0006e0                                    	callx8	a6
    1e34:	080362                                    	l8ui	a6, a3, 8
    1e37:	070c                                    	movi.n	a7, 0
    1e39:	0e43a2                                    	s8i	a10, a3, 14
    1e3c:	a1cb                                    	addi.n	a10, a1, 12
    1e3e:	f88d31                                    	l32r	a3, 74 (c50 <loas_program_config_element>)
    1e41:	05bd                                    	mov.n	a11, a5
    1e43:	86dc                                    	bnez.n	a6, 1e5f <aac_specific_config+0xc7>
    1e45:	0003e0                                    	callx8	a3
    1e48:	027d                                    	mov.n	a7, a2
    1e4a:	f8a521                                    	l32r	a2, e0 (2c8 <memcpy>)
    1e4d:	0e1076                                    	bt	b0, 1e5f <aac_specific_config+0xc7>
    1e50:	070c                                    	movi.n	a7, 0
    1e52:	948c                                    	beqz.n	a4, 1e5f <aac_specific_config+0xc7>
    1e54:	b1cb                                    	addi.n	a11, a1, 12
    1e56:	2c1c                                    	movi.n	a12, 18
    1e58:	04ad                                    	mov.n	a10, a4
    1e5a:	070c                                    	movi.n	a7, 0
    1e5c:	0002e0                                    	callx8	a2
    1e5f:	312870                                    	srai	a2, a7, 24
    1e62:	f01d                                    	retw.n

00001e64 <aac_specific_config+0xcc>:
    1e64:	827c                                    	movi.n	a2, -8
    1e66:	f01d                                    	retw.n

00001e68 <aac_specific_config+0xd0>:
    1e68:	e27c                                    	movi.n	a2, -2
    1e6a:	f01d                                    	retw.n

00001e6c <tns_decode>:
    1e6c:	016136                                    	entry	a1, 176
    1e6f:	3129                                    	s32i.n	a2, a1, 12
    1e71:	070222                                    	l8ui	a2, a2, 7
    1e74:	a169                                    	s32i.n	a6, a1, 40
    1e76:	160c                                    	movi.n	a6, 1
    1e78:	5139                                    	s32i.n	a3, a1, 20
    1e7a:	2f0c                                    	movi.n	a15, 2
    1e7c:	31e8                                    	l32i.n	a14, a1, 12
    1e7e:	10b216                                    	beqz	a2, 1f8d <tns_decode+0x121>
    1e81:	953d                                    	ae_zext16	a3, a5
    1e83:	f898a1                                    	l32r	a10, e4 (6350 <tns_sbf_max>)
    1e86:	390c                                    	movi.n	a9, 3
    1e88:	010e52                                    	l8ui	a5, a14, 1
    1e8b:	fdc332                                    	addi	a3, a3, -3
    1e8e:	c77b                                    	addi.n	a12, a7, 7
    1e90:	51e8                                    	l32i.n	a14, a1, 20
    1e92:	b0d4a0                                    	addx8	a13, a4, a10
    1e95:	b3c770                                    	movgez	a12, a7, a7
    1e98:	fec552                                    	addi	a5, a5, -2
    1e9b:	90a0b2                                    	movi	a11, 144
    1e9e:	939f50                                    	movnez	a9, a15, a5
    1ea1:	625560                                    	saltu	a5, a5, a6
    1ea4:	939530                                    	movnez	a9, a5, a3
    1ea7:	070c                                    	movi.n	a7, 0
    1ea9:	9039d0                                    	addx2	a3, a9, a13
    1eac:	21f3c0                                    	srai	a15, a12, 3
    1eaf:	233d                                    	ae_l16si.n	a3, a3, 0
    1eb1:	4179                                    	s32i.n	a7, a1, 16
    1eb3:	b139                                    	s32i.n	a3, a1, 44
    1eb5:	050c                                    	movi.n	a5, 0
    1eb7:	0e6d                                    	mov.n	a6, a14
    1eb9:	4eba                                    	add.n	a4, a14, a11
    1ebb:	71f9                                    	s32i.n	a15, a1, 28
    1ebd:	5138                                    	l32i.n	a3, a1, 20
    1ebf:	000706                                    	j	1edf <tns_decode+0x73>

00001ec2 <tns_decode+0x56>:
    1ec2:	3128                                    	l32i.n	a2, a1, 12
    1ec4:	9158                                    	l32i.n	a5, a1, 36
    1ec6:	8168                                    	l32i.n	a6, a1, 32
    1ec8:	070222                                    	l8ui	a2, a2, 7
    1ecb:	4138                                    	l32i.n	a3, a1, 16
    1ecd:	551b                                    	addi.n	a5, a5, 1
    1ecf:	7178                                    	l32i.n	a7, a1, 28
    1ed1:	664b                                    	addi.n	a6, a6, 4
    1ed3:	337a                                    	add.n	a3, a3, a7
    1ed5:	4139                                    	s32i.n	a3, a1, 16
    1ed7:	5138                                    	l32i.n	a3, a1, 20
    1ed9:	023527                                    	bltu	a5, a2, 1edf <tns_decode+0x73>
    1edc:	002b46                                    	j	1f8d <tns_decode+0x121>
    1edf:	335a                                    	add.n	a3, a3, a5
    1ee1:	6139                                    	s32i.n	a3, a1, 24
    1ee3:	000332                                    	l8ui	a3, a3, 0
    1ee6:	3178                                    	l32i.n	a7, a1, 12
    1ee8:	4e1c                                    	movi.n	a14, 20
    1eea:	0f0c                                    	movi.n	a15, 0
    1eec:	fdb316                                    	beqz	a3, 1ecb <tns_decode+0x5f>
    1eef:	9159                                    	s32i.n	a5, a1, 36
    1ef1:	030c                                    	movi.n	a3, 0
    1ef3:	050772                                    	l8ui	a7, a7, 5
    1ef6:	8169                                    	s32i.n	a6, a1, 32
    1ef8:	000546                                    	j	1f11 <tns_decode+0xa5>

00001efb <tns_decode+0x8f>:
    1efb:	f87d21                                    	l32r	a2, f0 (2128 <tns_filter_m1>)
    1efe:	0002e0                                    	callx8	a2
    1f01:	6128                                    	l32i.n	a2, a1, 24
    1f03:	331b                                    	addi.n	a3, a3, 1
    1f05:	661b                                    	addi.n	a6, a6, 1
    1f07:	4e1c                                    	movi.n	a14, 20
    1f09:	000222                                    	l8ui	a2, a2, 0
    1f0c:	0f0c                                    	movi.n	a15, 0
    1f0e:	b0b327                                    	bgeu	a3, a2, 1ec2 <tns_decode+0x56>
    1f11:	100652                                    	l8ui	a5, a6, 16
    1f14:	072d                                    	mov.n	a2, a7
    1f16:	3006d2                                    	l8ui	a13, a6, 48
    1f19:	6198                                    	l32i.n	a9, a1, 24
    1f1b:	7006c2                                    	l8ui	a12, a6, 112
    1f1e:	c08750                                    	sub	a8, a7, a5
    1f21:	435de0                                    	min	a5, a13, a14
    1f24:	5378f0                                    	max	a7, a8, a15
    1f27:	30c1e2                                    	addi	a14, a1, 48
    1f2a:	fd3516                                    	beqz	a5, 1f01 <tns_decode+0x95>
    1f2d:	04dd                                    	mov.n	a13, a4
    1f2f:	05ad                                    	mov.n	a10, a5
    1f31:	080982                                    	l8ui	a8, a9, 8
    1f34:	445a                                    	add.n	a4, a4, a5
    1f36:	b83b                                    	addi.n	a11, a8, 3
    1f38:	f86c81                                    	l32r	a8, e8 (1f90 <tns_decode_coef_mod>)
    1f3b:	0008e0                                    	callx8	a8
    1f3e:	31a8                                    	l32i.n	a10, a1, 12
    1f40:	b1b8                                    	l32i.n	a11, a1, 44
    1f42:	000a82                                    	l8ui	a8, a10, 0
    1f45:	01daa2                                    	addmi	a10, a10, 0x100
    1f48:	4397b0                                    	min	a9, a7, a11
    1f4b:	4322b0                                    	min	a2, a2, a11
    1f4e:	1c0c                                    	movi.n	a12, 1
    1f50:	41d8                                    	l32i.n	a13, a1, 16
    1f52:	439980                                    	min	a9, a9, a8
    1f55:	432280                                    	min	a2, a2, a8
    1f58:	90e9a0                                    	addx2	a14, a9, a10
    1f5b:	90f2a0                                    	addx2	a15, a2, a10
    1f5e:	500692                                    	l8ui	a9, a6, 80
    1f61:	4e2d                                    	ae_l16ui.n	a2, a14, 0
    1f63:	4f8d                                    	ae_l16ui.n	a8, a15, 0
    1f65:	a1f8                                    	l32i.n	a15, a1, 40
    1f67:	fa7c                                    	movi.n	a10, -1
    1f69:	05ed                                    	mov.n	a14, a5
    1f6b:	93ca90                                    	movnez	a12, a10, a9
    1f6e:	c0b820                                    	sub	a11, a8, a2
    1f71:	880b                                    	addi.n	a8, a8, -1
    1f73:	932890                                    	movnez	a2, a8, a9
    1f76:	871ba6                                    	blti	a11, 1, 1f01 <tns_decode+0x95>
    1f79:	2d2a                                    	add.n	a2, a13, a2
    1f7b:	30c1d2                                    	addi	a13, a1, 48
    1f7e:	a0a2f0                                    	addx4	a10, a2, a15
    1f81:	026507                                    	bbci	a5, 0, 1f87 <tns_decode+0x11b>
    1f84:	ffdcc6                                    	j	1efb <tns_decode+0x8f>
    1f87:	f85921                                    	l32r	a2, ec (206c <tns_filter_m0>)
    1f8a:	ffdc06                                    	j	1efe <tns_decode+0x92>

00001f8d <tns_decode+0x121>:
    1f8d:	f01d                                    	retw.n

00001f8f <tns_decode+0x123>:
	...

00001f90 <tns_decode_coef_mod>:
    1f90:	024136                                    	entry	a1, 0x120
    1f93:	180c                                    	movi.n	a8, 1
    1f95:	070c                                    	movi.n	a7, 0
    1f97:	019880                                    	slli	a9, a8, 24
    1f9a:	0212e6                                    	bgei	a2, 1, 1fa0 <tns_decode_coef_mod+0x10>
    1f9d:	003086                                    	j	2063 <tns_decode_coef_mod+0xd3>
    1fa0:	fdc332                                    	addi	a3, a3, -3
    1fa3:	380c                                    	movi.n	a8, 3
    1fa5:	f853a1                                    	l32r	a10, f4 (4800 <tns_coef>)
    1fa8:	290c                                    	movi.n	a9, 2
    1faa:	939830                                    	movnez	a9, a8, a3
    1fad:	627730                                    	saltu	a7, a7, a3
    1fb0:	839740                                    	moveqz	a9, a7, a4
    1fb3:	70c132                                    	addi	a3, a1, 112
    1fb6:	a049a0                                    	addx4	a4, a9, a10
    1fb9:	10c172                                    	addi	a7, a1, 16
    1fbc:	0448                                    	l32i.n	a4, a4, 0
    1fbe:	118276                                    	loop	a2, 1fd3 <tns_decode_coef_mod+0x43>
    1fc1:	000582                                    	l8ui	a8, a5, 0
    1fc4:	551b                                    	addi.n	a5, a5, 1
    1fc6:	908840                                    	addx2	a8, a8, a4
    1fc9:	288d                                    	ae_l16si.n	a8, a8, 0
    1fcb:	0789                                    	s32i.n	a8, a7, 0
    1fcd:	774b                                    	addi.n	a7, a7, 4
    1fcf:	638d                                    	ae_s16i.n	a8, a3, 0
    1fd1:	332b                                    	addi.n	a3, a3, 2

00001fd3 <tns_decode_coef_mod+0x43>:
    1fd3:	140c                                    	movi.n	a4, 1
    1fd5:	4138                                    	l32i.n	a3, a1, 16
    1fd7:	014480                                    	slli	a4, a4, 24
    1fda:	01d172                                    	addmi	a7, a1, 0x100
    1fdd:	0649                                    	s32i.n	a4, a6, 0
    1fdf:	72c182                                    	addi	a8, a1, 114
    1fe2:	11f370                                    	slli	a15, a3, 9
    1fe5:	364b                                    	addi.n	a3, a6, 4
    1fe7:	16f9                                    	s32i.n	a15, a6, 4
    1fe9:	150c                                    	movi.n	a5, 1
    1feb:	a0c762                                    	addi	a6, a7, -96
    1fee:	6122a6                                    	blti	a2, 2, 2053 <tns_decode_coef_mod+0xc3>
    1ff1:	070c                                    	movi.n	a7, 0
    1ff3:	fec882                                    	addi	a8, a8, -2

00001ff6 <tns_decode_coef_mod+0x66>:
    1ff6:	950b                                    	addi.n	a9, a5, -1
    1ff8:	03ad                                    	mov.n	a10, a3
    1ffa:	049090                                    	extui	a9, a9, 0, 1
    1ffd:	064d                                    	mov.n	a4, a6
    1fff:	93a690                                    	movnez	a10, a6, a9
    2002:	934390                                    	movnez	a4, a3, a9
    2005:	ba7a                                    	add.n	a11, a10, a7
    2007:	fcca92                                    	addi	a9, a10, -4
    200a:	0b02e4c0049f618e                  	{ ae_l16m.iu	aed0, a8, 2; addi	a10, a4, -4 }
    2012:	04cbb2                                    	addi	a11, a11, 4
    2015:	178576                                    	loop	a5, 2030 <tns_decode_coef_mod+0xa0>
    2018:	1105294008c27fbe                  	{ ae_l32m.iu	aed1, a11, -4; ae_l32m.iu	aed2, a9, 4 }
    2020:	1e8354133420b13e                  	{ nop; nop; ae_mulaf48q32sp16s.l	aed2, aed1, aed0 }
    2028:	230760c010c5b1ae                  	{ ae_s32m.iu	aed2, a10, 4; nop }

00002030 <tns_decode_coef_mod+0xa0>:
    2030:	1606c44000f2102e                  	{ ae_cvtq56p32s.l	aed0, aed0; add	a9, a4, a7 }
    2038:	0e6ce1600c7a5578060001380401177f 	{ addi	a7, a7, 4; addi	a5, a5, 1; nop; ae_srai64	aed0, aed0, 7 }
    2048:	220760c000c5b19e                  	{ ae_s32m.i	aed0, a9, 4; nop }
    2050:	a29257                                    	bne	a2, a5, 1ff6 <tns_decode_coef_mod+0x66>

00002053 <tns_decode_coef_mod+0xc3>:
    2053:	0ae207                                    	bbsi	a2, 0, 2061 <tns_decode_coef_mod+0xd1>
    2056:	078276                                    	loop	a2, 2061 <tns_decode_coef_mod+0xd1>
    2059:	0428                                    	l32i.n	a2, a4, 0
    205b:	444b                                    	addi.n	a4, a4, 4
    205d:	0329                                    	s32i.n	a2, a3, 0
    205f:	334b                                    	addi.n	a3, a3, 4

00002061 <tns_decode_coef_mod+0xd1>:
    2061:	f01d                                    	retw.n

00002063 <tns_decode_coef_mod+0xd3>:
    2063:	1679                                    	s32i.n	a7, a6, 4
    2065:	0699                                    	s32i.n	a9, a6, 0
    2067:	f01d                                    	retw.n

00002069 <tns_decode_coef_mod+0xd9>:
    2069:	000000                                        ...

0000206c <tns_filter_m0>:
    206c:	024136                                    	entry	a1, 0x120
    206f:	b54b                                    	addi.n	a11, a5, 4
    2071:	1b16a6                                    	blti	a6, 1, 2090 <tns_filter_m0+0x24>
    2074:	f81b71                                    	l32r	a7, e0 (2c8 <memcpy>)
    2077:	1156e0                                    	slli	a5, a6, 2
    207a:	10c1a2                                    	addi	a10, a1, 16
    207d:	05cd                                    	mov.n	a12, a5
    207f:	0007e0                                    	callx8	a7
    2082:	f7eb71                                    	l32r	a7, 30 (3878 <memset>)
    2085:	60c1a2                                    	addi	a10, a1, 96
    2088:	0b0c                                    	movi.n	a11, 0
    208a:	20c550                                    	or	a12, a5, a5
    208d:	0007e0                                    	callx8	a7

00002090 <tns_filter_m0+0x24>:
    2090:	60c152                                    	addi	a5, a1, 96
    2093:	01c6b2                                    	addi	a11, a6, 1
    2096:	a07650                                    	addx4	a7, a6, a5
    2099:	1144e0                                    	slli	a4, a4, 2
    209c:	f3f650                                    	wur.ae_cbegin0	a5
    209f:	f3f770                                    	wur.ae_cend0	a7
    20a2:	b3b660                                    	movgez	a11, a6, a6
    20a5:	7a13a6                                    	blti	a3, 1, 2123 <tns_filter_m0+0xb7>
    20a8:	10c172                                    	addi	a7, a1, 16
    20ab:	04a082                                    	movi	a8, 4
    20ae:	10a092                                    	movi	a9, 16
    20b1:	fcafa2                                    	movi	a10, -4
    20b4:	c02240                                    	sub	a2, a2, a4
    20b7:	21b1b0                                    	srai	a11, a11, 1
    20ba:	0606b74000cbb42e                  	{ ae_l32m.xu	aed0, a2, a4; or	a12, a7, a7 }
    20c2:	0ead257b09040cd0c580813a0c01130f 	{ ae_srai64	aed0, aed0, 7; nop; nop; nop }
    20d2:	1a26a6                                    	blti	a6, 2, 20f0 <tns_filter_m0+0x84>
    20d5:	178b76                                    	loop	a11, 20f0 <tns_filter_m0+0x84>
    20d8:	39063cc014c1585e                  	{ ae_l32.xc	aed2, a5, a8; ae_l32x2.ip	aed1, a12, 8 }
    20e0:	1e8287073041385e                  	{ ae_l32.xc	aed3, a5, a8; nop; ae_mulaf32r.lh	aed0, aed2, aed1 }
    20e8:	1e8348033461b13e                  	{ nop; nop; ae_mulaf32r.ll	aed0, aed3, aed1 }

000020f0 <tns_filter_m0+0x84>:
    20f0:	0eace567f90233d0c58081380c01130f 	{ ae_slaisq56s	aed0, aed0, 7; addi	a3, a3, -1; nop; nop }
    2100:	0ead245b0d250cd0c580800103c0002f 	{ ae_s32m.i	aed0, a2, 0; nop; ae_neg64	aed0, aed0; nop }
    2110:	0a0585400041090e                  	{ ae_trunca32x2f64s	aed0, aed0, aed0, a9; ae_l32.xc	aed1, a5, a10 }
    2118:	230760c004c5d05e                  	{ ae_s32.l.i	aed0, a5, 0; nop }
    2120:	f96356                                    	bnez	a3, 20ba <tns_filter_m0+0x4e>

00002123 <tns_filter_m0+0xb7>:
    2123:	f01d                                    	retw.n

00002125 <tns_filter_m0+0xb9>:
    2125:	000000                                        ...

00002128 <tns_filter_m1>:
    2128:	024136                                    	entry	a1, 0x120
    212b:	b54b                                    	addi.n	a11, a5, 4
    212d:	1c16a6                                    	blti	a6, 1, 214d <tns_filter_m1+0x25>
    2130:	f7ec71                                    	l32r	a7, e0 (2c8 <memcpy>)
    2133:	1156e0                                    	slli	a5, a6, 2
    2136:	10c1a2                                    	addi	a10, a1, 16
    2139:	05cd                                    	mov.n	a12, a5
    213b:	0007e0                                    	callx8	a7
    213e:	f7bc71                                    	l32r	a7, 30 (3878 <memset>)
    2141:	60c1a2                                    	addi	a10, a1, 96
    2144:	00a0b2                                    	movi	a11, 0
    2147:	20c550                                    	or	a12, a5, a5
    214a:	0007e0                                    	callx8	a7

0000214d <tns_filter_m1+0x25>:
    214d:	60c152                                    	addi	a5, a1, 96
    2150:	01c6a2                                    	addi	a10, a6, 1
    2153:	a07650                                    	addx4	a7, a6, a5
    2156:	1144e0                                    	slli	a4, a4, 2
    2159:	f3f650                                    	wur.ae_cbegin0	a5
    215c:	f3f770                                    	wur.ae_cend0	a7
    215f:	b3a660                                    	movgez	a10, a6, a6
    2162:	0213e6                                    	bgei	a3, 1, 2168 <tns_filter_m1+0x40>
    2165:	002086                                    	j	21eb <tns_filter_m1+0xc3>
    2168:	10c172                                    	addi	a7, a1, 16
    216b:	04a082                                    	movi	a8, 4
    216e:	c02240                                    	sub	a2, a2, a4
    2171:	00a092                                    	movi	a9, 0
    2174:	20b770                                    	or	a11, a7, a7
    2177:	21a1a0                                    	srai	a10, a10, 1
    217a:	38063bc000c1b42e                  	{ ae_l32m.xu	aed0, a2, a4; ae_l32x2.ip	aed1, a11, 8 }
    2182:	0ead257b09040cd0c580813a0c01130f 	{ ae_srai64	aed0, aed0, 7; nop; nop; nop }
    2192:	1a26a6                                    	blti	a6, 2, 21b0 <tns_filter_m1+0x88>
    2195:	178a76                                    	loop	a10, 21b0 <tns_filter_m1+0x88>
    2198:	230720c014c5585e                  	{ ae_l32.xc	aed2, a5, a8; nop }
    21a0:	1e8287073041385e                  	{ ae_l32.xc	aed3, a5, a8; nop; ae_mulaf32r.lh	aed0, aed2, aed1 }
    21a8:	1e826800386113be                  	{ ae_l32x2.ip	aed1, a11, 8; nop; ae_mulaf32r.ll	aed0, aed3, aed1 }

000021b0 <tns_filter_m1+0x88>:
    21b0:	3302a3c014df595e                  	{ ae_l32.xc	aed2, a5, a9; addi	a3, a3, -1 }
    21b8:	1e8327002c41b77e                  	{ or	a11, a7, a7; nop; ae_mulaf32r.lh	aed0, aed2, aed1 }
    21c0:	0ead257b09040cd0c58081380c01130f 	{ ae_slaisq56s	aed0, aed0, 7; nop; nop; nop }
    21d0:	0ead245b0d250cd0c580800103c0002f 	{ ae_s32m.i	aed0, a2, 0; nop; ae_neg64	aed0, aed0; nop }
    21e0:	220760c000c5b05e                  	{ ae_s32m.i	aed0, a5, 0; nop }
    21e8:	f8e356                                    	bnez	a3, 217a <tns_filter_m1+0x52>

000021eb <tns_filter_m1+0xc3>:
    21eb:	f01d                                    	retw.n

000021ed <tns_filter_m1+0xc5>:
    21ed:	000000                                        ...

000021f0 <fill_DB>:
    21f0:	004136                                    	entry	a1, 32
    21f3:	430c                                    	movi.n	a3, 4
    21f5:	045020                                    	extui	a5, a2, 0, 1
    21f8:	c04350                                    	sub	a4, a3, a5
    21fb:	1135d0                                    	slli	a3, a5, 3
    21fe:	fcc552                                    	addi	a5, a5, -4
    2201:	749c                                    	beqz.n	a4, 221c <fill_DB+0x2c>
    2203:	607050                                    	neg	a7, a5
    2206:	026d                                    	mov.n	a6, a2
    2208:	050c                                    	movi.n	a5, 0
    220a:	098776                                    	loop	a7, 2217 <fill_DB+0x27>
    220d:	000672                                    	l8ui	a7, a6, 0
    2210:	115580                                    	slli	a5, a5, 8
    2213:	661b                                    	addi.n	a6, a6, 1
    2215:	557a                                    	add.n	a5, a5, a7

00002217 <fill_DB+0x27>:
    2217:	224a                                    	add.n	a2, a2, a4
    2219:	000046                                    	j	221e <fill_DB+0x2e>

0000221c <fill_DB+0x2c>:
    221c:	050c                                    	movi.n	a5, 0
    221e:	fec222                                    	addi	a2, a2, -2
    2221:	67ef34                                    	wur.ae_bitptr	a3
    2224:	f3f150                                    	wur.ae_bithead	a5
    2227:	f01d                                    	retw.n

00002229 <fill_DB+0x39>:
    2229:	000000                                        ...

0000222c <aac_initbits>:
    222c:	004136                                    	entry	a1, 32
    222f:	450c                                    	movi.n	a5, 4
    2231:	046030                                    	extui	a6, a3, 0, 1
    2234:	2249                                    	s32i.n	a4, a2, 8
    2236:	c05560                                    	sub	a5, a5, a6
    2239:	1146d0                                    	slli	a4, a6, 3
    223c:	0239                                    	s32i.n	a3, a2, 0
    223e:	fcc662                                    	addi	a6, a6, -4
    2241:	759c                                    	beqz.n	a5, 225c <aac_initbits+0x30>
    2243:	608060                                    	neg	a8, a6
    2246:	037d                                    	mov.n	a7, a3
    2248:	060c                                    	movi.n	a6, 0
    224a:	098876                                    	loop	a8, 2257 <aac_initbits+0x2b>
    224d:	000782                                    	l8ui	a8, a7, 0
    2250:	116680                                    	slli	a6, a6, 8
    2253:	771b                                    	addi.n	a7, a7, 1
    2255:	668a                                    	add.n	a6, a6, a8

00002257 <aac_initbits+0x2b>:
    2257:	335a                                    	add.n	a3, a3, a5
    2259:	000046                                    	j	225e <aac_initbits+0x32>

0000225c <aac_initbits+0x30>:
    225c:	060c                                    	movi.n	a6, 0
    225e:	fec332                                    	addi	a3, a3, -2
    2261:	67ef44                                    	wur.ae_bitptr	a4
    2264:	f3f160                                    	wur.ae_bithead	a6
    2267:	1239                                    	s32i.n	a3, a2, 4
    2269:	f01d                                    	retw.n

0000226b <aac_initbits+0x3f>:
	...

0000226c <aac_getbits>:
    226c:	004136                                    	entry	a1, 32
    226f:	024d                                    	mov.n	a4, a2
    2271:	051c                                    	movi.n	a5, 16
    2273:	f0c322                                    	addi	a2, a3, -16
    2276:	133537                                    	bltu	a5, a3, 228d <aac_getbits+0x21>
    2279:	220701c00c80145e                  	{ l32i	a5, a4, 4; ae_lb	a2, a3 }
    2281:	230740c00ca5753e                  	{ ae_db	a5, a3; nop }
    2289:	1459                                    	s32i.n	a5, a4, 4
    228b:	f01d                                    	retw.n

0000228d <aac_getbits+0x21>:
    228d:	120701c00c8104fe                  	{ l32i	a15, a4, 4; ae_lb	a5, a2 }
    2295:	1106c5400ca56f2e                  	{ ae_db	a15, a2; slli	a5, a5, 16 }
    229d:	21072fc0088414fe                  	{ s32i	a15, a4, 4; ae_lbi	a2, 16 }
    22a5:	1438                                    	l32i.n	a3, a4, 4
    22a7:	2406a24218f053fe                  	{ ae_dbi	a3, 16; add	a2, a2, a5 }
    22af:	1439                                    	s32i.n	a3, a4, 4
    22b1:	f01d                                    	retw.n

000022b3 <aac_getbits+0x47>:
	...

000022b4 <aac_get1bit>:
    22b4:	004136                                    	entry	a1, 32
    22b7:	1248                                    	l32i.n	a4, a2, 4
    22b9:	023d                                    	mov.n	a3, a2
    22bb:	230740c414a5740e                  	{ ae_lbi	a2, 1; nop }
    22c3:	220720c218e5540e                  	{ ae_dbi	a4, 1; nop }
    22cb:	1349                                    	s32i.n	a4, a3, 4
    22cd:	f01d                                    	retw.n

000022cf <aac_get1bit+0x1b>:
	...

000022d0 <aac_byte_align>:
    22d0:	004136                                    	entry	a1, 32
    22d3:	67ee34                                    	rur.ae_bitptr	a3
    22d6:	840c                                    	movi.n	a4, 8
    22d8:	725430                                    	salt	a5, a4, a3
    22db:	039c                                    	beqz.n	a3, 22ef <aac_byte_align+0x1f>
    22dd:	12e8                                    	l32i.n	a14, a2, 4
    22df:	b04540                                    	addx8	a4, a5, a4
    22e2:	c0f430                                    	sub	a15, a4, a3
    22e5:	230740c00ca57efe                  	{ ae_db	a14, a15; nop }
    22ed:	12e9                                    	s32i.n	a14, a2, 4

000022ef <aac_byte_align+0x1f>:
    22ef:	f01d                                    	retw.n

000022f1 <aac_byte_align+0x21>:
    22f1:	000000                                        ...

000022f4 <aac_get_processed_bits>:
    22f4:	004136                                    	entry	a1, 32
    22f7:	1248                                    	l32i.n	a4, a2, 4
    22f9:	67ee34                                    	rur.ae_bitptr	a3
    22fc:	02f8                                    	l32i.n	a15, a2, 0
    22fe:	c024f0                                    	sub	a2, a4, a15
    2301:	b02230                                    	addx8	a2, a2, a3
    2304:	f0c222                                    	addi	a2, a2, -16
    2307:	f01d                                    	retw.n

00002309 <aac_get_processed_bits+0x15>:
    2309:	000000                                        ...

0000230c <aac_resetbits>:
    230c:	004136                                    	entry	a1, 32
    230f:	0248                                    	l32i.n	a4, a2, 0
    2311:	216330                                    	srai	a6, a3, 3
    2314:	450c                                    	movi.n	a5, 4
    2316:	243030                                    	extui	a3, a3, 0, 3
    2319:	446a                                    	add.n	a4, a4, a6
    231b:	046040                                    	extui	a6, a4, 0, 1
    231e:	048d                                    	mov.n	a8, a4
    2320:	c07560                                    	sub	a7, a5, a6
    2323:	1156d0                                    	slli	a5, a6, 3
    2326:	fcc662                                    	addi	a6, a6, -4
    2329:	679c                                    	beqz.n	a7, 2343 <aac_resetbits+0x37>
    232b:	609060                                    	neg	a9, a6
    232e:	00a062                                    	movi	a6, 0
    2331:	098976                                    	loop	a9, 233e <aac_resetbits+0x32>
    2334:	000892                                    	l8ui	a9, a8, 0
    2337:	116680                                    	slli	a6, a6, 8
    233a:	881b                                    	addi.n	a8, a8, 1
    233c:	669a                                    	add.n	a6, a6, a9

0000233e <aac_resetbits+0x32>:
    233e:	447a                                    	add.n	a4, a4, a7
    2340:	000046                                    	j	2345 <aac_resetbits+0x39>

00002343 <aac_resetbits+0x37>:
    2343:	060c                                    	movi.n	a6, 0
    2345:	fec442                                    	addi	a4, a4, -2
    2348:	1249                                    	s32i.n	a4, a2, 4
    234a:	1248                                    	l32i.n	a4, a2, 4
    234c:	67ef54                                    	wur.ae_bitptr	a5
    234f:	f3f160                                    	wur.ae_bithead	a6
    2352:	230740c00ca5743e                  	{ ae_db	a4, a3; nop }
    235a:	1249                                    	s32i.n	a4, a2, 4
    235c:	f01d                                    	retw.n

0000235e <aac_resetbits+0x52>:
	...

00002360 <aac_used_bytes>:
    2360:	004136                                    	entry	a1, 32
    2363:	1248                                    	l32i.n	a4, a2, 4
    2365:	67ee34                                    	rur.ae_bitptr	a3
    2368:	02f8                                    	l32i.n	a15, a2, 0
    236a:	c024f0                                    	sub	a2, a4, a15
    236d:	b02230                                    	addx8	a2, a2, a3
    2370:	f0c222                                    	addi	a2, a2, -16
    2373:	212320                                    	srai	a2, a2, 3
    2376:	f01d                                    	retw.n

00002378 <dehuffman_sf>:
    2378:	004136                                    	entry	a1, 32
    237b:	630c                                    	movi.n	a3, 6
    237d:	230740c614a57b3e                  	{ ae_vldsht	a3; nop }
    2385:	f75c31                                    	l32r	a3, f8 (58c0 <vld_huffdatasf>)

00002388 <dehuffman_sf+0x10>:
    2388:	1503a24400a0b34e                  	{ ae_vldl16t	b0, a4, a3; l32i	a5, a2, 4 }
    2390:	230740c410a535ce                  	{ ae_vldl16c	a5; nop }
    2398:	1259                                    	s32i.n	a5, a2, 4
    239a:	ea0076                                    	bf	b0, 2388 <dehuffman_sf+0x10>
    239d:	c4c422                                    	addi	a2, a4, -60
    23a0:	f01d                                    	retw.n

000023a2 <dehuffman_sf+0x2a>:
	...

000023a4 <dehuffman_cb1_cb2>:
    23a4:	008136                                    	entry	a1, 64
    23a7:	a60b                                    	addi.n	a10, a6, -1
    23a9:	480c                                    	movi.n	a8, 4
    23ab:	0c0c                                    	movi.n	a12, 0
    23ad:	5b0c                                    	movi.n	a11, 5
    23af:	51c9                                    	s32i.n	a12, a1, 20
    23b1:	93b8a0                                    	movnez	a11, a8, a10
    23b4:	dc6c                                    	movi.n	a12, -19
    23b6:	1506dc4614b97bbe                  	{ ae_vldsht	a11; sub	a5, a12, a5 }
    23be:	0217e6                                    	bgei	a7, 1, 23c4 <dehuffman_cb1_cb2+0x20>
    23c1:	0023c6                                    	j	2454 <dehuffman_cb1_cb2+0xb0>
    23c4:	f74eb1                                    	l32r	a11, fc (5f40 <vld_huffdata2>)
    23c7:	f74e61                                    	l32r	a6, 100 (5e20 <vld_huffdata1>)
    23ca:	102182                                    	l32i	a8, a1, 64
    23cd:	f74d91                                    	l32r	a9, 104 (4f60 <acc_pow_quarter>)
    23d0:	936ba0                                    	movnez	a6, a11, a10
    23d3:	10c1a2                                    	addi	a10, a1, 16
    23d6:	cb0c                                    	movi.n	a11, 12

000023d8 <dehuffman_cb1_cb2+0x34>:
    23d8:	22cd                                    	ae_l16si.n	a12, a2, 0
    23da:	53ed                                    	ae_l16ui.n	a14, a3, 2
    23dc:	21d2c0                                    	srai	a13, a12, 2
    23df:	14c0c0                                    	extui	a12, a12, 0, 2
    23e2:	a0cc90                                    	addx4	a12, a12, a9
    23e5:	d5da                                    	add.n	a13, a5, a13
    23e7:	0c03334000c060ce                  	{ ae_l32m.i	aed0, a12, 0; l16ui	a12, a3, 0 }
    23ef:	2202834010a01d0e                  	{ ae_slaasq56s	aed0, aed0, a13; addi	a3, a3, 2 }
    23f7:	1406f44000cba2ae                  	{ ae_s32m.i	aed0, a10, 8; or	a13, a4, a4 }
    23ff:	61f8                                    	l32i.n	a15, a1, 24
    2401:	c0cec0                                    	sub	a12, a14, a12
    2404:	60f0f0                                    	neg	a15, a15
    2407:	41f9                                    	s32i.n	a15, a1, 16
    2409:	1f03a84400a0a64e                  	{ ae_vldl16t	b0, a4, a6; l32i	a14, a8, 4 }
    2411:	3b06c4c410a73ece                  	{ ae_vldl16c	a14; and	a15, a4, a11 }
    2419:	18e9                                    	s32i.n	a14, a8, 4
    241b:	14e040                                    	extui	a14, a4, 0, 2
    241e:	e70076                                    	bf	b0, 2409 <dehuffman_cb1_cb2+0x65>
    2421:	a0eea0                                    	addx4	a14, a14, a10
    2424:	fafa                                    	add.n	a15, a10, a15
    2426:	0ee8                                    	l32i.n	a14, a14, 0
    2428:	0ff8                                    	l32i.n	a15, a15, 0
    242a:	0de9                                    	s32i.n	a14, a13, 0
    242c:	14e440                                    	extui	a14, a4, 4, 2
    242f:	1df9                                    	s32i.n	a15, a13, 4
    2431:	14f640                                    	extui	a15, a4, 6, 2
    2434:	a0eea0                                    	addx4	a14, a14, a10
    2437:	a0ffa0                                    	addx4	a15, a15, a10
    243a:	0ee8                                    	l32i.n	a14, a14, 0
    243c:	0ff8                                    	l32i.n	a15, a15, 0
    243e:	10cd42                                    	addi	a4, a13, 16
    2441:	fcccc2                                    	addi	a12, a12, -4
    2444:	2de9                                    	s32i.n	a14, a13, 8
    2446:	3df9                                    	s32i.n	a15, a13, 12
    2448:	04dd                                    	mov.n	a13, a4
    244a:	bb1ce6                                    	bgei	a12, 1, 2409 <dehuffman_cb1_cb2+0x65>
    244d:	770b                                    	addi.n	a7, a7, -1
    244f:	222b                                    	addi.n	a2, a2, 2
    2451:	8317e6                                    	bgei	a7, 1, 23d8 <dehuffman_cb1_cb2+0x34>
    2454:	f01d                                    	retw.n

00002456 <dehuffman_cb1_cb2+0xb2>:
	...

00002458 <dehuffman_cb3_cb4>:
    2458:	00a136                                    	entry	a1, 80
    245b:	080c                                    	movi.n	a8, 0
    245d:	9159                                    	s32i.n	a5, a1, 36
    245f:	fdc692                                    	addi	a9, a6, -3
    2462:	4189                                    	s32i.n	a8, a1, 16
    2464:	03cd                                    	mov.n	a12, a3
    2466:	0217e6                                    	bgei	a7, 1, 246c <dehuffman_cb3_cb4+0x14>
    2469:	004a86                                    	j	2597 <dehuffman_cb3_cb4+0x13f>
    246c:	f727b1                                    	l32r	a11, 108 (5130 <vld_huffdata4>)
    246f:	10c1a2                                    	addi	a10, a1, 16
    2472:	f72661                                    	l32r	a6, 10c (4fb0 <vld_huffdata3>)
    2475:	530c                                    	movi.n	a3, 5
    2477:	142182                                    	l32i	a8, a1, 80
    247a:	3d0c                                    	movi.n	a13, 3
    247c:	9158                                    	l32i.n	a5, a1, 36
    247e:	936b90                                    	movnez	a6, a11, a9

00002481 <dehuffman_cb3_cb4+0x29>:
    2481:	229d                                    	ae_l16si.n	a9, a2, 0
    2483:	b129                                    	s32i.n	a2, a1, 44
    2485:	4cbd                                    	ae_l16ui.n	a11, a12, 0
    2487:	5cfd                                    	ae_l16ui.n	a15, a12, 2
    2489:	cc2b                                    	addi.n	a12, a12, 2
    248b:	21e290                                    	srai	a14, a9, 2
    248e:	149090                                    	extui	a9, a9, 0, 2
    2491:	c0ee50                                    	sub	a14, a14, a5
    2494:	f71c51                                    	l32r	a5, 104 (4f60 <acc_pow_quarter>)
    2497:	edcee2                                    	addi	a14, a14, -19
    249a:	a1c9                                    	s32i.n	a12, a1, 40
    249c:	a09950                                    	addx4	a9, a9, a5
    249f:	0405294000df609e                  	{ ae_l32m.i	aed0, a9, 0; ae_l32m.i	aed31, a9, 16 }
    24a7:	220700c010a51e0e                  	{ ae_slaasq56s	aed0, aed0, a14; nop }
    24af:	220760c000c5b1ae                  	{ ae_s32m.i	aed0, a10, 4; nop }
    24b7:	0e04024010a02efe                  	{ ae_slaasq56s	aed0, aed31, a14; movi	a14, 2 }
    24bf:	2a069f4010bb1e0e                  	{ ae_slaasq56s	aed0, aed0, a14; sub	a14, a15, a11 }
    24c7:	3406f44000cba2ae                  	{ ae_s32m.i	aed0, a10, 8; or	a15, a4, a4 }
    24cf:	230740c614a57b3e                  	{ ae_vldsht	a3; nop }
    24d7:	1903a84400a0b64e                  	{ ae_vldl16t	b0, a4, a6; l32i	a9, a8, 4 }
    24df:	230740c410a539ce                  	{ ae_vldl16c	a9; nop }
    24e7:	1899                                    	s32i.n	a9, a8, 4
    24e9:	ea0076                                    	bf	b0, 24d7 <dehuffman_cb3_cb4+0x7f>
    24ec:	149040                                    	extui	a9, a4, 0, 2
    24ef:	18b8                                    	l32i.n	a11, a8, 4
    24f1:	010700c20ce739ae                  	{ addx4	a9, a9, a10; ae_lbi	a12, 1 }
    24f9:	0998                                    	l32i.n	a9, a9, 0
    24fb:	0c0d47                                    	bnone	a13, a4, 250b <dehuffman_cb3_cb4+0xb3>
    24fe:	180722c218e15b0e                  	{ ae_dbi	a11, 1; neg	a5, a9 }
    2506:	9395c0                                    	movnez	a9, a5, a12
    2509:	18b9                                    	s32i.n	a11, a8, 4
    250b:	415240                                    	srli	a5, a4, 2
    250e:	0f99                                    	s32i.n	a9, a15, 0
    2510:	110760c21866105e                  	{ extui	a11, a5, 0, 2; ae_lbi	a9, 1 }
    2518:	a0bba0                                    	addx4	a11, a11, a10
    251b:	0bb8                                    	l32i.n	a11, a11, 0
    251d:	0e0d57                                    	bnone	a13, a5, 252f <dehuffman_cb3_cb4+0xd7>
    2520:	1858                                    	l32i.n	a5, a8, 4
    2522:	60c0b0                                    	neg	a12, a11
    2525:	3806bc4218e6550e                  	{ ae_dbi	a5, 1; movnez	a11, a12, a9 }
    252d:	1859                                    	s32i.n	a5, a8, 4
    252f:	415440                                    	srli	a5, a4, 4
    2532:	1fb9                                    	s32i.n	a11, a15, 4
    2534:	310760c20866105e                  	{ extui	a9, a5, 0, 2; ae_lbi	a11, 1 }
    253c:	a099a0                                    	addx4	a9, a9, a10
    253f:	0998                                    	l32i.n	a9, a9, 0
    2541:	0e0d57                                    	bnone	a13, a5, 2553 <dehuffman_cb3_cb4+0xfb>
    2544:	1858                                    	l32i.n	a5, a8, 4
    2546:	60c090                                    	neg	a12, a9
    2549:	1a06bc4218e6550e                  	{ ae_dbi	a5, 1; movnez	a9, a12, a11 }
    2551:	1859                                    	s32i.n	a5, a8, 4
    2553:	414640                                    	srli	a4, a4, 6
    2556:	2f99                                    	s32i.n	a9, a15, 8
    2558:	210760c10864104e                  	{ extui	a5, a4, 0, 2; ae_lbi	a2, 1 }
    2560:	a055a0                                    	addx4	a5, a5, a10
    2563:	05b8                                    	l32i.n	a11, a5, 0
    2565:	0e0d47                                    	bnone	a13, a4, 2577 <dehuffman_cb3_cb4+0x11f>
    2568:	1848                                    	l32i.n	a4, a8, 4
    256a:	6090b0                                    	neg	a9, a11
    256d:	3206b94218e6440e                  	{ ae_dbi	a4, 1; movnez	a11, a9, a2 }
    2575:	1849                                    	s32i.n	a4, a8, 4
    2577:	fccee2                                    	addi	a14, a14, -4
    257a:	10cf42                                    	addi	a4, a15, 16
    257d:	3fb9                                    	s32i.n	a11, a15, 12
    257f:	04fd                                    	mov.n	a15, a4
    2581:	021ea6                                    	blti	a14, 1, 2587 <dehuffman_cb3_cb4+0x12f>
    2584:	ffd1c6                                    	j	24cf <dehuffman_cb3_cb4+0x77>
    2587:	b128                                    	l32i.n	a2, a1, 44
    2589:	770b                                    	addi.n	a7, a7, -1
    258b:	a1c8                                    	l32i.n	a12, a1, 40
    258d:	9158                                    	l32i.n	a5, a1, 36
    258f:	222b                                    	addi.n	a2, a2, 2
    2591:	0217a6                                    	blti	a7, 1, 2597 <dehuffman_cb3_cb4+0x13f>
    2594:	ffba46                                    	j	2481 <dehuffman_cb3_cb4+0x29>
    2597:	f01d                                    	retw.n

00002599 <dehuffman_cb3_cb4+0x141>:
    2599:	000000                                        ...

0000259c <dehuffman_cb5_cb6>:
    259c:	00c136                                    	entry	a1, 96
    259f:	fbc6a2                                    	addi	a10, a6, -5
    25a2:	580c                                    	movi.n	a8, 5
    25a4:	6b0c                                    	movi.n	a11, 6
    25a6:	0c0c                                    	movi.n	a12, 0
    25a8:	93b8a0                                    	movnez	a11, a8, a10
    25ab:	81c9                                    	s32i.n	a12, a1, 32
    25ad:	0902c14614a87bbe                  	{ ae_vldsht	a11; addi	a9, a1, 32 }
    25b5:	182162                                    	l32i	a6, a1, 96
    25b8:	0217e6                                    	bgei	a7, 1, 25be <dehuffman_cb5_cb6+0x22>
    25bb:	003986                                    	j	26a5 <dehuffman_cb5_cb6+0x109>
    25be:	f6d4b1                                    	l32r	a11, 110 (5d00 <vld_huffdata6>)
    25c1:	f6d481                                    	l32r	a8, 114 (5b20 <vld_huffdata5>)
    25c4:	938ba0                                    	movnez	a8, a11, a10
    25c7:	f6cfa1                                    	l32r	a10, 104 (4f60 <acc_pow_quarter>)
    25ca:	10c1b2                                    	addi	a11, a1, 16

000025cd <dehuffman_cb5_cb6+0x31>:
    25cd:	22cd                                    	ae_l16si.n	a12, a2, 0
    25cf:	21d2c0                                    	srai	a13, a12, 2
    25d2:	14c0c0                                    	extui	a12, a12, 0, 2
    25d5:	c0dd50                                    	sub	a13, a13, a5
    25d8:	a0cca0                                    	addx4	a12, a12, a10
    25db:	1e02adc000db60ce                  	{ ae_l32m.i	aed0, a12, 0; addi	a14, a13, -19 }
    25e3:	3e028dc010bb1e0e                  	{ ae_slaasq56s	aed0, aed0, a14; addi	a15, a13, -17 }
    25eb:	0c02ac4008c664ce                  	{ ae_l32m.i	aed1, a12, 16; addi	a12, a12, 24 }
    25f3:	02050c4010be4f1e                  	{ ae_slaasq56s	aed1, aed1, a15; ae_l32m.i	aed30, a12, 8 }
    25fb:	0eace4478523ddd0c580800103c0019f 	{ ae_s32m.i	aed0, a9, 4; addi	a13, a13, -16; ae_neg64	aed0, aed0; nop }
    260b:	0eace459fd23c6d0c580800903c1429f 	{ ae_s32m.i	aed1, a9, 8; ae_l32m.i	aed31, a12, 24; ae_neg64	aed1, aed1; nop }
    261b:	0e03734000c0af9e                  	{ ae_s32m.i	aed0, a9, -4; l16ui	a14, a3, 0 }
    2623:	1e03134010a03fee                  	{ ae_slaasq56s	aed0, aed30, a15; l16ui	a15, a3, 2 }
    262b:	2202e34008c0be9e                  	{ ae_s32m.i	aed1, a9, -8; addi	a3, a3, 2 }
    2633:	1406944010ab6dfe                  	{ ae_slaasq56s	aed1, aed31, a13; or	a13, a4, a4 }
    263b:	0ead6448e523fed0c580800103c0039f 	{ ae_s32m.i	aed0, a9, 12; sub	a12, a15, a14; ae_neg64	aed0, aed0; nop }
    264b:	0ead245b0d250cd0c580800903c1509f 	{ ae_s32m.i	aed1, a9, 16; nop; ae_neg64	aed1, aed1; nop }
    265b:	220760c000c5bd9e                  	{ ae_s32m.i	aed0, a9, -12; nop }
    2663:	220760c008c5bc9e                  	{ ae_s32m.i	aed1, a9, -16; nop }
    266b:	1f03a64400a0a84e                  	{ ae_vldl16t	b0, a4, a8; l32i	a14, a6, 4 }
    2673:	3502444410bb2ece                  	{ ae_vldl16c	a14; extui	a15, a4, 4, 4 }
    267b:	16e9                                    	s32i.n	a14, a6, 4
    267d:	34e040                                    	extui	a14, a4, 0, 4
    2680:	e70076                                    	bf	b0, 266b <dehuffman_cb5_cb6+0xcf>
    2683:	a0eeb0                                    	addx4	a14, a14, a11
    2686:	a0ffb0                                    	addx4	a15, a15, a11
    2689:	0ee8                                    	l32i.n	a14, a14, 0
    268b:	0ff8                                    	l32i.n	a15, a15, 0
    268d:	4d8b                                    	addi.n	a4, a13, 8
    268f:	feccc2                                    	addi	a12, a12, -2
    2692:	0de9                                    	s32i.n	a14, a13, 0
    2694:	1df9                                    	s32i.n	a15, a13, 4
    2696:	04dd                                    	mov.n	a13, a4
    2698:	cf1ce6                                    	bgei	a12, 1, 266b <dehuffman_cb5_cb6+0xcf>
    269b:	770b                                    	addi.n	a7, a7, -1
    269d:	222b                                    	addi.n	a2, a2, 2
    269f:	0217a6                                    	blti	a7, 1, 26a5 <dehuffman_cb5_cb6+0x109>
    26a2:	ffc9c6                                    	j	25cd <dehuffman_cb5_cb6+0x31>
    26a5:	f01d                                    	retw.n

000026a7 <dehuffman_cb5_cb6+0x10b>:
	...

000026a8 <dehuffman_cb7_8_9_10>:
    26a8:	008136                                    	entry	a1, 64
    26ab:	4159                                    	s32i.n	a5, a1, 16
    26ad:	950c                                    	movi.n	a5, 9
    26af:	5b0c                                    	movi.n	a11, 5
    26b1:	1b7626                                    	beqi	a6, 7, 26d0 <dehuffman_cb7_8_9_10+0x28>
    26b4:	f8c692                                    	addi	a9, a6, -8
    26b7:	1d1657                                    	beq	a6, a5, 26d8 <dehuffman_cb7_8_9_10+0x30>
    26ba:	480c                                    	movi.n	a8, 4
    26bc:	f69851                                    	l32r	a5, 11c (56d0 <vld_huffdata10>)
    26bf:	f698a1                                    	l32r	a10, 120 (5600 <vld_huffdata8>)
    26c2:	938b90                                    	movnez	a8, a11, a9
    26c5:	3189                                    	s32i.n	a8, a1, 12
    26c7:	93a590                                    	movnez	a10, a5, a9
    26ca:	1717e6                                    	bgei	a7, 1, 26e5 <dehuffman_cb7_8_9_10+0x3d>
    26cd:	004846                                    	j	27f2 <dehuffman_cb7_8_9_10+0x14a>

000026d0 <dehuffman_cb7_8_9_10+0x28>:
    26d0:	550c                                    	movi.n	a5, 5
    26d2:	f691a1                                    	l32r	a10, 118 (52a0 <vld_huffdata7>)
    26d5:	000106                                    	j	26dd <dehuffman_cb7_8_9_10+0x35>

000026d8 <dehuffman_cb7_8_9_10+0x30>:
    26d8:	450c                                    	movi.n	a5, 4
    26da:	f692a1                                    	l32r	a10, 124 (53f0 <vld_huffdata9>)
    26dd:	3159                                    	s32i.n	a5, a1, 12
    26df:	0217e6                                    	bgei	a7, 1, 26e5 <dehuffman_cb7_8_9_10+0x3d>
    26e2:	004306                                    	j	27f2 <dehuffman_cb7_8_9_10+0x14a>
    26e5:	102162                                    	l32i	a6, a1, 64
    26e8:	fc0c                                    	movi.n	a12, 15
    26ea:	f670d1                                    	l32r	a13, ac (4810 <aac_pow4_3>)
    26ed:	087c                                    	movi.n	a8, -16
    26ef:	225d                                    	ae_l16si.n	a5, a2, 0
    26f1:	41e8                                    	l32i.n	a14, a1, 16
    26f3:	439d                                    	ae_l16ui.n	a9, a3, 0
    26f5:	7179                                    	s32i.n	a7, a1, 28
    26f7:	53bd                                    	ae_l16ui.n	a11, a3, 2
    26f9:	332b                                    	addi.n	a3, a3, 2
    26fb:	f682f1                                    	l32r	a15, 104 (4f60 <acc_pow_quarter>)
    26fe:	217250                                    	srai	a7, a5, 2
    2701:	5139                                    	s32i.n	a3, a1, 20
    2703:	145050                                    	extui	a5, a5, 0, 2
    2706:	c037e0                                    	sub	a3, a7, a14
    2709:	c0eb90                                    	sub	a14, a11, a9
    270c:	6129                                    	s32i.n	a2, a1, 24
    270e:	a0f5f0                                    	addx4	a15, a5, a15
    2711:	3128                                    	l32i.n	a2, a1, 12
    2713:	eec392                                    	addi	a9, a3, -18
    2716:	3506d44614aa6b2e                  	{ ae_vldsht	a2; or	a11, a4, a4 }
    271e:	1303a64400a0ba4e                  	{ ae_vldl16t	b0, a4, a10; l32i	a3, a6, 4 }
    2726:	230740c410a533ce                  	{ ae_vldl16c	a3; nop }
    272e:	1639                                    	s32i.n	a3, a6, 4
    2730:	ea0076                                    	bf	b0, 271e <dehuffman_cb7_8_9_10+0x76>
    2733:	030c                                    	movi.n	a3, 0
    2735:	050c                                    	movi.n	a5, 0
    2737:	342040                                    	extui	a2, a4, 0, 4
    273a:	480c47                                    	bnone	a12, a4, 2786 <dehuffman_cb7_8_9_10+0xde>
    273d:	a032d0                                    	addx4	a3, a2, a13
    2740:	0f78                                    	l32i.n	a7, a15, 0
    2742:	0338                                    	l32i.n	a3, a3, 0
    2744:	342030                                    	extui	a2, a3, 0, 4
    2747:	103380                                    	and	a3, a3, a8
    274a:	b23370                                    	mulsh	a3, a3, a7
    274d:	2007034014e0292e                  	{ add	a2, a9, a2; ae_cvt48a32	aed0, a3 }
    2755:	0c069d4010aa120e                  	{ ae_slaasq56s	aed0, aed0, a2; or	a8, a13, a13 }
    275d:	0eace5680d2462d0c400813b0c016d0f 	{ ae_trunca32q48	a13, aed0; l32i	a2, a6, 4; nop; nop }
    276d:	230740c51ca5740e                  	{ ae_lbi	a7, 1; nop }
    2775:	3c0722c218e0520e                  	{ ae_dbi	a2, 1; neg	a3, a13 }
    277d:	833d70                                    	moveqz	a3, a13, a7
    2780:	08dd                                    	mov.n	a13, a8
    2782:	1629                                    	s32i.n	a2, a6, 4
    2784:	087c                                    	movi.n	a8, -16
    2786:	414440                                    	srli	a4, a4, 4
    2789:	0b39                                    	s32i.n	a3, a11, 0
    278b:	342040                                    	extui	a2, a4, 0, 4
    278e:	410c47                                    	bnone	a12, a4, 27d3 <dehuffman_cb7_8_9_10+0x12b>
    2791:	a022d0                                    	addx4	a2, a2, a13
    2794:	0f38                                    	l32i.n	a3, a15, 0
    2796:	0228                                    	l32i.n	a2, a2, 0
    2798:	1648                                    	l32i.n	a4, a6, 4
    279a:	345020                                    	extui	a5, a2, 0, 4
    279d:	102280                                    	and	a2, a2, a8
    27a0:	b22230                                    	mulsh	a2, a2, a3
    27a3:	200702411ce0295e                  	{ add	a7, a9, a5; ae_cvt48a32	aed0, a2 }
    27ab:	200700c010a4170e                  	{ ae_slaasq56s	aed0, aed0, a7; ae_lbi	a2, 1 }
    27b3:	0ead257b0d240cd0c400813b0c01670f 	{ ae_trunca32q48	a7, aed0; nop; nop; nop }
    27c3:	220720c218e5540e                  	{ ae_dbi	a4, 1; nop }
    27cb:	1649                                    	s32i.n	a4, a6, 4
    27cd:	605070                                    	neg	a5, a7
    27d0:	835720                                    	moveqz	a5, a7, a2
    27d3:	fecee2                                    	addi	a14, a14, -2
    27d6:	4b8b                                    	addi.n	a4, a11, 8
    27d8:	3128                                    	l32i.n	a2, a1, 12
    27da:	1b59                                    	s32i.n	a5, a11, 4
    27dc:	021ea6                                    	blti	a14, 1, 27e2 <dehuffman_cb7_8_9_10+0x13a>
    27df:	ffccc6                                    	j	2716 <dehuffman_cb7_8_9_10+0x6e>
    27e2:	7178                                    	l32i.n	a7, a1, 28
    27e4:	6128                                    	l32i.n	a2, a1, 24
    27e6:	5138                                    	l32i.n	a3, a1, 20
    27e8:	770b                                    	addi.n	a7, a7, -1
    27ea:	222b                                    	addi.n	a2, a2, 2
    27ec:	0217a6                                    	blti	a7, 1, 27f2 <dehuffman_cb7_8_9_10+0x14a>
    27ef:	ffbf06                                    	j	26ef <dehuffman_cb7_8_9_10+0x47>
    27f2:	f01d                                    	retw.n

000027f4 <aac_dequant_medium>:
    27f4:	004136                                    	entry	a1, 32
    27f7:	f62d51                                    	l32r	a5, ac (4810 <aac_pow4_3>)
    27fa:	0e7c                                    	movi.n	a14, -16
    27fc:	f64261                                    	l32r	a6, 104 (4f60 <acc_pow_quarter>)
    27ff:	a02250                                    	addx4	a2, a2, a5
    2802:	0228                                    	l32i.n	a2, a2, 0
    2804:	a04460                                    	addx4	a4, a4, a6
    2807:	0448                                    	l32i.n	a4, a4, 0
    2809:	34f020                                    	extui	a15, a2, 0, 4
    280c:	1022e0                                    	and	a2, a2, a14
    280f:	33fa                                    	add.n	a3, a3, a15
    2811:	b22240                                    	mulsh	a2, a2, a4
    2814:	200762431460e33e                  	{ addi	a3, a3, -18; ae_cvt48a32	aed0, a2 }
    281c:	220700c010a5130e                  	{ ae_slaasq56s	aed0, aed0, a3; nop }
    2824:	0ead257b0d240cd0c400813b0c01620f 	{ ae_trunca32q48	a2, aed0; nop; nop; nop }
    2834:	f01d                                    	retw.n

00002836 <aac_dequant_medium+0x42>:
	...

00002838 <dehuffman_cb11>:
    2838:	00a136                                    	entry	a1, 80
    283b:	7159                                    	s32i.n	a5, a1, 28
    283d:	039d                                    	mov.n	a9, a3
    283f:	2169                                    	s32i.n	a6, a1, 8
    2841:	71d8                                    	l32i.n	a13, a1, 28
    2843:	0216e6                                    	bgei	a6, 1, 2849 <dehuffman_cb11+0x11>
    2846:	007d46                                    	j	2a3f <dehuffman_cb11+0x207>
    2849:	f63761                                    	l32r	a6, 128 (6020 <vld_huffdata11>)
    284c:	0004c6                                    	j	2863 <dehuffman_cb11+0x2b>

0000284f <dehuffman_cb11+0x17>:
    284f:	2138                                    	l32i.n	a3, a1, 8
    2851:	9128                                    	l32i.n	a2, a1, 36
    2853:	8198                                    	l32i.n	a9, a1, 32
    2855:	330b                                    	addi.n	a3, a3, -1
    2857:	222b                                    	addi.n	a2, a2, 2
    2859:	2139                                    	s32i.n	a3, a1, 8
    285b:	71d8                                    	l32i.n	a13, a1, 28
    285d:	0213e6                                    	bgei	a3, 1, 2863 <dehuffman_cb11+0x2b>
    2860:	0076c6                                    	j	2a3f <dehuffman_cb11+0x207>
    2863:	223d                                    	ae_l16si.n	a3, a2, 0
    2865:	a92b                                    	addi.n	a10, a9, 2
    2867:	f627e1                                    	l32r	a14, 104 (4f60 <acc_pow_quarter>)
    286a:	495d                                    	ae_l16ui.n	a5, a9, 0
    286c:	9129                                    	s32i.n	a2, a1, 36
    286e:	598d                                    	ae_l16ui.n	a8, a9, 2
    2870:	21b230                                    	srai	a11, a3, 2
    2873:	14c030                                    	extui	a12, a3, 0, 2
    2876:	c03bd0                                    	sub	a3, a11, a13
    2879:	a02ce0                                    	addx4	a2, a12, a14
    287c:	eec3f2                                    	addi	a15, a3, -18
    287f:	c05850                                    	sub	a5, a8, a5
    2882:	81a9                                    	s32i.n	a10, a1, 32
    2884:	61c9                                    	s32i.n	a12, a1, 24
    2886:	4129                                    	s32i.n	a2, a1, 16
    2888:	042d                                    	mov.n	a2, a4
    288a:	5139                                    	s32i.n	a3, a1, 20
    288c:	530c                                    	movi.n	a3, 5
    288e:	31f9                                    	s32i.n	a15, a1, 12
    2890:	0012c6                                    	j	28df <dehuffman_cb11+0xa7>

00002893 <dehuffman_cb11+0x5b>:
    2893:	f60641                                    	l32r	a4, ac (4810 <aac_pow4_3>)
    2896:	a03340                                    	addx4	a3, a3, a4
    2899:	0e48                                    	l32i.n	a4, a14, 0
    289b:	0338                                    	l32i.n	a3, a3, 0
    289d:	348030                                    	extui	a8, a3, 0, 4
    28a0:	103390                                    	and	a3, a3, a9
    28a3:	b23340                                    	mulsh	a3, a3, a4
    28a6:	2007034104e02f8e                  	{ add	a4, a15, a8; ae_cvt48a32	aed0, a3 }
    28ae:	220700c010a5140e                  	{ ae_slaasq56s	aed0, aed0, a4; nop }
    28b6:	0ead257b0d240cd0c400813b0c016a0f 	{ ae_trunca32q48	a10, aed0; nop; nop; nop }
    28c6:	b148                                    	l32i.n	a4, a1, 44
    28c8:	6030a0                                    	neg	a3, a10
    28cb:	833a40                                    	moveqz	a3, a10, a4
    28ce:	428b                                    	addi.n	a4, a2, 8
    28d0:	fec552                                    	addi	a5, a5, -2
    28d3:	1239                                    	s32i.n	a3, a2, 4
    28d5:	530c                                    	movi.n	a3, 5
    28d7:	042d                                    	mov.n	a2, a4
    28d9:	0215e6                                    	bgei	a5, 1, 28df <dehuffman_cb11+0xa7>
    28dc:	ffdbc6                                    	j	284f <dehuffman_cb11+0x17>
    28df:	230740c614a57b3e                  	{ ae_vldsht	a3; nop }
    28e7:	1503a74400a0a63e                  	{ ae_vldl16t	b0, a3, a6; l32i	a4, a7, 4 }
    28ef:	230740c410a534ce                  	{ ae_vldl16c	a4; nop }
    28f7:	1749                                    	s32i.n	a4, a7, 4
    28f9:	ea0076                                    	bf	b0, 28e7 <dehuffman_cb11+0xaf>
    28fc:	414530                                    	srli	a4, a3, 5
    28ff:	f83c                                    	movi.n	a8, 63
    2901:	17a8                                    	l32i.n	a10, a7, 4
    2903:	443030                                    	extui	a3, a3, 0, 5
    2906:	100700831806984e                  	{ bnone.w15	a8, a4, 291a <dehuffman_cb11+0xe2>; ae_lbi	a9, 1 }
    290e:	a199                                    	s32i.n	a9, a1, 40
    2910:	220720c218e55a0e                  	{ ae_dbi	a10, 1; nop }
    2918:	17a9                                    	s32i.n	a10, a7, 4
    291a:	110760c10066504e                  	{ extui	a4, a4, 0, 6; ae_lbi	a9, 1 }
    2922:	c38c                                    	beqz.n	a3, 2932 <dehuffman_cb11+0xfa>
    2924:	1788                                    	l32i.n	a8, a7, 4
    2926:	b199                                    	s32i.n	a9, a1, 44
    2928:	220720c218e5580e                  	{ ae_dbi	a8, 1; nop }
    2930:	1789                                    	s32i.n	a8, a7, 4
    2932:	0a7c                                    	movi.n	a10, -16
    2934:	09b416                                    	beqz	a4, 29d3 <dehuffman_cb11+0x19b>
    2937:	41e8                                    	l32i.n	a14, a1, 16
    2939:	59b466                                    	bnei	a4, 16, 2996 <dehuffman_cb11+0x15e>
    293c:	1748                                    	l32i.n	a4, a7, 4
    293e:	f97c                                    	movi.n	a9, -1
    2940:	010724c20ca65b9e                  	{ srli	a9, a9, 11; ae_lbi	a8, 5 }
    2948:	0e04214218e0444e                  	{ ae_dbi	a4, 5; movi	a14, 1 }
    2950:	1b04284604a018fe                  	{ ae_lbki	a8, a8, 16; movi	a11, 24 }
    2958:	1749                                    	s32i.n	a4, a7, 4
    295a:	17d8                                    	l32i.n	a13, a7, 4
    295c:	309890                                    	xor	a9, a8, a9
    295f:	40f990                                    	nsau	a9, a9
    2962:	f9c992                                    	addi	a9, a9, -7
    2965:	11a9f0                                    	slli	a10, a9, 1
    2968:	c0cba0                                    	sub	a12, a11, a10
    296b:	400c00                                    	ssr	a12
    296e:	61c8                                    	l32i.n	a12, a1, 24
    2970:	b14080                                    	sra	a4, a8
    2973:	401900                                    	ssl	a9
    2976:	a18e00                                    	sll	a8, a14
    2979:	9099b0                                    	addx2	a9, a9, a11
    297c:	f80b                                    	addi.n	a15, a8, -1
    297e:	0f06c4c00ca57d9e                  	{ ae_db	a13, a9; and	a4, a4, a15 }
    2986:	51b8                                    	l32i.n	a11, a1, 20
    2988:	20a480                                    	or	a10, a4, a8
    298b:	f5e841                                    	l32r	a4, 12c (2a44 <aac_dequant_large>)
    298e:	17d9                                    	s32i.n	a13, a7, 4
    2990:	0004e0                                    	callx8	a4
    2993:	000d06                                    	j	29cb <dehuffman_cb11+0x193>

00002996 <dehuffman_cb11+0x15e>:
    2996:	f5c581                                    	l32r	a8, ac (4810 <aac_pow4_3>)
    2999:	31f8                                    	l32i.n	a15, a1, 12
    299b:	a04480                                    	addx4	a4, a4, a8
    299e:	0e88                                    	l32i.n	a8, a14, 0
    29a0:	0448                                    	l32i.n	a4, a4, 0
    29a2:	349040                                    	extui	a9, a4, 0, 4
    29a5:	1044a0                                    	and	a4, a4, a10
    29a8:	b24480                                    	mulsh	a4, a4, a8
    29ab:	2007044204e02f9e                  	{ add	a8, a15, a9; ae_cvt48a32	aed0, a4 }
    29b3:	220700c010a5180e                  	{ ae_slaasq56s	aed0, aed0, a8; nop }
    29bb:	0ead257b0d240cd0c400813b0c016a0f 	{ ae_trunca32q48	a10, aed0; nop; nop; nop }
    29cb:	a188                                    	l32i.n	a8, a1, 40
    29cd:	6040a0                                    	neg	a4, a10
    29d0:	834a80                                    	moveqz	a4, a10, a8
    29d3:	0249                                    	s32i.n	a4, a2, 0
    29d5:	097c                                    	movi.n	a9, -16
    29d7:	41e8                                    	l32i.n	a14, a1, 16
    29d9:	ef1316                                    	beqz	a3, 28ce <dehuffman_cb11+0x96>
    29dc:	31f8                                    	l32i.n	a15, a1, 12
    29de:	02b326                                    	beqi	a3, 16, 29e4 <dehuffman_cb11+0x1ac>
    29e1:	ffab86                                    	j	2893 <dehuffman_cb11+0x5b>
    29e4:	000704c00c85173e                  	{ l32i	a3, a7, 4; ae_lbi	a4, 5 }
    29ec:	f87c                                    	movi.n	a8, -1
    29ee:	8a1c                                    	movi.n	a10, 24
    29f0:	418b80                                    	srli	a8, a8, 11
    29f3:	1a03a14218e1534e                  	{ ae_dbi	a3, 5; l32i	a11, a1, 20 }
    29fb:	0f04214504a004fe                  	{ ae_lbki	a4, a4, 16; movi	a14, 1 }
    2a03:	1739                                    	s32i.n	a3, a7, 4
    2a05:	17d8                                    	l32i.n	a13, a7, 4
    2a07:	308480                                    	xor	a8, a4, a8
    2a0a:	40f880                                    	nsau	a8, a8
    2a0d:	f9c882                                    	addi	a8, a8, -7
    2a10:	1198f0                                    	slli	a9, a8, 1
    2a13:	c0ca90                                    	sub	a12, a10, a9
    2a16:	400c00                                    	ssr	a12
    2a19:	61c8                                    	l32i.n	a12, a1, 24
    2a1b:	b13040                                    	sra	a3, a4
    2a1e:	401800                                    	ssl	a8
    2a21:	a14e00                                    	sll	a4, a14
    2a24:	9088a0                                    	addx2	a8, a8, a10
    2a27:	f40b                                    	addi.n	a15, a4, -1
    2a29:	3f06c3c00ca47d8e                  	{ ae_db	a13, a8; and	a3, a3, a15 }
    2a31:	17d9                                    	s32i.n	a13, a7, 4
    2a33:	20a340                                    	or	a10, a3, a4
    2a36:	f5bd31                                    	l32r	a3, 12c (2a44 <aac_dequant_large>)
    2a39:	0003e0                                    	callx8	a3
    2a3c:	ffa186                                    	j	28c6 <dehuffman_cb11+0x8e>

00002a3f <dehuffman_cb11+0x207>:
    2a3f:	f01d                                    	retw.n

00002a41 <dehuffman_cb11+0x209>:
    2a41:	000000                                        ...

00002a44 <aac_dequant_large>:
    2a44:	004136                                    	entry	a1, 32
    2a47:	f59981                                    	l32r	a8, ac (4810 <aac_pow4_3>)
    2a4a:	81a052                                    	movi	a5, 129
    2a4d:	077c                                    	movi.n	a7, -16
    2a4f:	7d2257                                    	blt	a2, a5, 2ad0 <aac_dequant_large+0x8c>
    2a52:	200742401080008e                  	{ movi	a8, 32; ae_cvt48a32	aed0, a2 }
    2a5a:	000003f0010300fad0e303062c60b46f 	{ l32r	a6, ac (4810 <aac_pow4_3>); ae_nsa64	a5, aed0; nop }
    2a6a:	f5b171                                    	l32r	a7, 130 (4f50 <crs_exp>)
    2a6d:	f97c                                    	movi.n	a9, -1
    2a6f:	fb1c                                    	movi.n	a11, 31
    2a71:	0d7c                                    	movi.n	a13, -16
    2a73:	f8c552                                    	addi	a5, a5, -8
    2a76:	f5afe1                                    	l32r	a14, 134 (4f20 <crs_mant>)
    2a79:	c08850                                    	sub	a8, a8, a5
    2a7c:	c05b50                                    	sub	a5, a11, a5
    2a7f:	401800                                    	ssl	a8
    2a82:	905570                                    	addx2	a5, a5, a7
    2a85:	a19900                                    	sll	a9, a9
    2a88:	400800                                    	ssr	a8
    2a8b:	91a020                                    	srl	a10, a2
    2a8e:	109290                                    	and	a9, a2, a9
    2a91:	a06a60                                    	addx4	a6, a10, a6
    2a94:	255d                                    	ae_l16si.n	a5, a5, 0
    2a96:	06a8                                    	l32i.n	a10, a6, 0
    2a98:	302290                                    	xor	a2, a2, a9
    2a9b:	1668                                    	l32i.n	a6, a6, 4
    2a9d:	34b0a0                                    	extui	a11, a10, 0, 4
    2aa0:	10aad0                                    	and	a10, a10, a13
    2aa3:	34c060                                    	extui	a12, a6, 0, 4
    2aa6:	1066d0                                    	and	a6, a6, a13
    2aa9:	c07cb0                                    	sub	a7, a12, a11
    2aac:	10b5d0                                    	and	a11, a5, a13
    2aaf:	400700                                    	ssr	a7
    2ab2:	feba                                    	add.n	a15, a14, a11
    2ab4:	b1a0a0                                    	sra	a10, a10
    2ab7:	400800                                    	ssr	a8
    2aba:	c066a0                                    	sub	a6, a6, a10
    2abd:	345050                                    	extui	a5, a5, 0, 4
    2ac0:	826620                                    	mull	a6, a6, a2
    2ac3:	a024f0                                    	addx4	a2, a4, a15
    2ac6:	b16060                                    	sra	a6, a6
    2ac9:	45ca                                    	add.n	a4, a5, a12
    2acb:	56aa                                    	add.n	a5, a6, a10
    2acd:	000406                                    	j	2ae1 <aac_dequant_large+0x9d>

00002ad0 <aac_dequant_large+0x8c>:
    2ad0:	f58d61                                    	l32r	a6, 104 (4f60 <acc_pow_quarter>)
    2ad3:	a02280                                    	addx4	a2, a2, a8
    2ad6:	0258                                    	l32i.n	a5, a2, 0
    2ad8:	a02460                                    	addx4	a2, a4, a6
    2adb:	344050                                    	extui	a4, a5, 0, 4
    2ade:	105570                                    	and	a5, a5, a7
    2ae1:	0228                                    	l32i.n	a2, a2, 0
    2ae3:	334a                                    	add.n	a3, a3, a4
    2ae5:	eec332                                    	addi	a3, a3, -18
    2ae8:	b22520                                    	mulsh	a2, a5, a2
    2aeb:	2107224318e0647e                  	{ nop; ae_cvt48a32	aed0, a2 }
    2af3:	220700c010a5130e                  	{ ae_slaasq56s	aed0, aed0, a3; nop }
    2afb:	0ead257b0d240cd0c400813b0c01620f 	{ ae_trunca32q48	a2, aed0; nop; nop; nop }
    2b0b:	f01d                                    	retw.n

00002b0d <aac_dequant_large+0xc9>:
    2b0d:	000000                                        ...

00002b10 <aac_dequant_small>:
    2b10:	004136                                    	entry	a1, 32
    2b13:	f58951                                    	l32r	a5, 138 (4fa0 <Q_quarter_exp>)
    2b16:	a0d240                                    	addx4	a13, a2, a4
    2b19:	f57a61                                    	l32r	a6, 104 (4f60 <acc_pow_quarter>)
    2b1c:	905250                                    	addx2	a5, a2, a5
    2b1f:	25ed                                    	ae_l16si.n	a14, a5, 0
    2b21:	a02d60                                    	addx4	a2, a13, a6
    2b24:	000502431ce02e3e                  	{ add	a15, a14, a3; ae_l32m.i	aed0, a2, 0 }
    2b2c:	220700c010a51f0e                  	{ ae_slaasq56s	aed0, aed0, a15; nop }
    2b34:	0ead257b0d240cd0c400813b0c01620f 	{ ae_trunca32q48	a2, aed0; nop; nop; nop }
    2b44:	f01d                                    	retw.n

00002b46 <aac_dequant_small+0x36>:
	...

00002b50 <filter_bank_LC>:
    2b50:	00e136                                    	entry	a1, 112
    2b53:	24c272                                    	addi	a7, a2, 36
    2b56:	b26a                                    	add.n	a11, a2, a6
    2b58:	a0a620                                    	addx4	a10, a6, a2
    2b5b:	a0e670                                    	addx4	a14, a6, a7
    2b5e:	a9a082                                    	movi	a8, 169
    2b61:	130bf2                                    	l8ui	a15, a11, 19
    2b64:	bad8                                    	l32i.n	a13, a10, 44
    2b66:	0ee8                                    	l32i.n	a14, a14, 0
    2b68:	7bd882                                    	addmi	a8, a8, 0x7b00
    2b6b:	00a292                                    	movi	a9, 0x200
    2b6e:	090285c004bfe88e                  	{ ae_cvtp24a16x2.ll	aed0, a8, a8; addi	a8, a5, -4 }
    2b76:	0020f0                                    	nop
    2b79:	f03d                                    	nop.n
    2b7b:	f03d                                    	nop.n
    2b7d:	278976                                    	loop	a9, 2ba8 <filter_bank_LC+0x58>
    2b80:	230720c008c5718e                  	{ ae_l32m.iu	aed1, a8, 4; nop }
    2b88:	2282930b3020218e                  	{ ae_l32m.iu	aed2, a8, 4; nop; ae_mulf48q32sp16s.l	aed1, aed1, aed0 }
    2b90:	228353133440b13e                  	{ nop; nop; ae_mulf48q32sp16s.l	aed2, aed2, aed0 }
    2b98:	220760c008c5bf8e                  	{ ae_s32m.i	aed1, a8, -4; nop }
    2ba0:	220760c010c5b08e                  	{ ae_s32m.i	aed2, a8, 0; nop }

00002ba8 <filter_bank_LC+0x58>:
    2ba8:	40c2c2                                    	addi	a12, a2, 64
    2bab:	568c                                    	beqz.n	a6, 2bb4 <filter_bank_LC+0x64>
    2bad:	28c272                                    	addi	a7, a2, 40
    2bb0:	000086                                    	j	2bb6 <filter_bank_LC+0x66>

00002bb3 <filter_bank_LC+0x63>:
	...

00002bb4 <filter_bank_LC+0x64>:
    2bb4:	a2c8                                    	l32i.n	a12, a2, 40
    2bb6:	f56121                                    	l32r	a2, 13c (99d0 <win_mode>)
    2bb9:	0778                                    	l32i.n	a7, a7, 0
    2bbb:	a06420                                    	addx4	a6, a4, a2
    2bbe:	a02f20                                    	addx4	a2, a15, a2
    2bc1:	0648                                    	l32i.n	a4, a6, 0
    2bc3:	05ad                                    	mov.n	a10, a5
    2bc5:	07bd                                    	mov.n	a11, a7
    2bc7:	2668                                    	l32i.n	a6, a6, 8
    2bc9:	0223a6                                    	blti	a3, 2, 2bcf <filter_bank_LC+0x7f>
    2bcc:	003d06                                    	j	2cc4 <filter_bank_LC+0x174>
    2bcf:	05ad                                    	mov.n	a10, a5
    2bd1:	0228                                    	l32i.n	a2, a2, 0
    2bd3:	07bd                                    	mov.n	a11, a7
    2bd5:	19d316                                    	beqz	a3, 2d76 <filter_bank_LC+0x226>
    2bd8:	0d4d                                    	mov.n	a4, a13
    2bda:	021326                                    	beqi	a3, 1, 2be0 <filter_bank_LC+0x90>
    2bdd:	006486                                    	j	2d73 <filter_bank_LC+0x223>
    2be0:	f55831                                    	l32r	a3, 140 (31d4 <imdct_long>)
    2be3:	0e5d                                    	mov.n	a5, a14
    2be5:	6169                                    	s32i.n	a6, a1, 24
    2be7:	41c9                                    	s32i.n	a12, a1, 16
    2be9:	0003e0                                    	callx8	a3
    2bec:	00a1f2                                    	movi	a15, 0x100
    2bef:	049d                                    	mov.n	a9, a4
    2bf1:	05ad                                    	mov.n	a10, a5
    2bf3:	043d                                    	mov.n	a3, a4
    2bf5:	7c8f76                                    	loop	a15, 2c75 <filter_bank_LC+0x125>
    2bf8:	0748                                    	l32i.n	a4, a7, 0
    2bfa:	225d                                    	ae_l16si.n	a5, a2, 0
    2bfc:	03f8                                    	l32i.n	a15, a3, 0
    2bfe:	0292d2                                    	l16si	a13, a2, 4
    2c01:	826540                                    	mull	a6, a5, a4
    2c04:	b24540                                    	mulsh	a4, a5, a4
    2c07:	41ef60                                    	srli	a14, a6, 15
    2c0a:	325d                                    	ae_l16si.n	a5, a2, 2
    2c0c:	0144f0                                    	slli	a4, a4, 17
    2c0f:	4e4a                                    	add.n	a4, a14, a4
    2c11:	414140                                    	srli	a4, a4, 1
    2c14:	9044f0                                    	addx2	a4, a4, a15
    2c17:	0a49                                    	s32i.n	a4, a10, 0
    2c19:	1768                                    	l32i.n	a6, a7, 4
    2c1b:	13c8                                    	l32i.n	a12, a3, 4
    2c1d:	b24560                                    	mulsh	a4, a5, a6
    2c20:	828560                                    	mull	a8, a5, a6
    2c23:	0144f0                                    	slli	a4, a4, 17
    2c26:	41bf80                                    	srli	a11, a8, 15
    2c29:	4b4a                                    	add.n	a4, a11, a4
    2c2b:	414140                                    	srli	a4, a4, 1
    2c2e:	9044c0                                    	addx2	a4, a4, a12
    2c31:	0392c2                                    	l16si	a12, a2, 6
    2c34:	1a49                                    	s32i.n	a4, a10, 4
    2c36:	228b                                    	addi.n	a2, a2, 8
    2c38:	27e8                                    	l32i.n	a14, a7, 8
    2c3a:	23b8                                    	l32i.n	a11, a3, 8
    2c3c:	b24de0                                    	mulsh	a4, a13, a14
    2c3f:	82fde0                                    	mull	a15, a13, a14
    2c42:	0144f0                                    	slli	a4, a4, 17
    2c45:	418ff0                                    	srli	a8, a15, 15
    2c48:	484a                                    	add.n	a4, a8, a4
    2c4a:	414140                                    	srli	a4, a4, 1
    2c4d:	9044b0                                    	addx2	a4, a4, a11
    2c50:	2a49                                    	s32i.n	a4, a10, 8
    2c52:	37d8                                    	l32i.n	a13, a7, 12
    2c54:	33f8                                    	l32i.n	a15, a3, 12
    2c56:	10c772                                    	addi	a7, a7, 16
    2c59:	10c332                                    	addi	a3, a3, 16
    2c5c:	b24cd0                                    	mulsh	a4, a12, a13
    2c5f:	82ecd0                                    	mull	a14, a12, a13
    2c62:	0144f0                                    	slli	a4, a4, 17
    2c65:	416fe0                                    	srli	a6, a14, 15
    2c68:	464a                                    	add.n	a4, a6, a4
    2c6a:	414140                                    	srli	a4, a4, 1
    2c6d:	9044f0                                    	addx2	a4, a4, a15
    2c70:	3a49                                    	s32i.n	a4, a10, 12
    2c72:	10caa2                                    	addi	a10, a10, 16

00002c75 <filter_bank_LC+0x125>:
    2c75:	4158                                    	l32i.n	a5, a1, 16
    2c77:	c0a142                                    	movi	a4, 0x1c0
    2c7a:	093d                                    	mov.n	a3, a9
    2c7c:	6178                                    	l32i.n	a7, a1, 24
    2c7e:	202550                                    	or	a2, a5, a5
    2c81:	078476                                    	loop	a4, 2c8c <filter_bank_LC+0x13c>
    2c84:	0248                                    	l32i.n	a4, a2, 0
    2c86:	224b                                    	addi.n	a2, a2, 4
    2c88:	0349                                    	s32i.n	a4, a3, 0
    2c8a:	334b                                    	addi.n	a3, a3, 4
    2c8c:	fea042                                    	movi	a4, 254
    2c8f:	80a062                                    	movi	a6, 128
    2c92:	07d532                                    	addmi	a3, a5, 0x700
    2c95:	07d922                                    	addmi	a2, a9, 0x700
    2c98:	474a                                    	add.n	a4, a7, a4
    2c9a:	feaf52                                    	movi	a5, -2
    2c9d:	1d8676                                    	loop	a6, 2cbe <filter_bank_LC+0x16e>
    2ca0:	0368                                    	l32i.n	a6, a3, 0
    2ca2:	247d                                    	ae_l16si.n	a7, a4, 0
    2ca4:	fec442                                    	addi	a4, a4, -2
    2ca7:	334b                                    	addi.n	a3, a3, 4
    2ca9:	828760                                    	mull	a8, a7, a6
    2cac:	b26760                                    	mulsh	a6, a7, a6
    2caf:	41ff80                                    	srli	a15, a8, 15
    2cb2:	0166f0                                    	slli	a6, a6, 17
    2cb5:	6f6a                                    	add.n	a6, a15, a6
    2cb7:	106650                                    	and	a6, a6, a5
    2cba:	0269                                    	s32i.n	a6, a2, 0
    2cbc:	224b                                    	addi.n	a2, a2, 4
    2cbe:	09d9a2                                    	addmi	a10, a9, 0x900
    2cc1:	013fc6                                    	j	31c4 <filter_bank_LC+0x674>

00002cc4 <filter_bank_LC+0x174>:
    2cc4:	2228                                    	l32i.n	a2, a2, 8
    2cc6:	022366                                    	bnei	a3, 2, 2ccc <filter_bank_LC+0x17c>
    2cc9:	006386                                    	j	2e5b <filter_bank_LC+0x30b>
    2ccc:	0e5d                                    	mov.n	a5, a14
    2cce:	023326                                    	beqi	a3, 3, 2cd4 <filter_bank_LC+0x184>
    2cd1:	002786                                    	j	2d73 <filter_bank_LC+0x223>
    2cd4:	f51b31                                    	l32r	a3, 140 (31d4 <imdct_long>)
    2cd7:	c0a162                                    	movi	a6, 0x1c0
    2cda:	5129                                    	s32i.n	a2, a1, 20
    2cdc:	0d2d                                    	mov.n	a2, a13
    2cde:	41c9                                    	s32i.n	a12, a1, 16
    2ce0:	0003e0                                    	callx8	a3
    2ce3:	05cd                                    	mov.n	a12, a5
    2ce5:	02bd                                    	mov.n	a11, a2
    2ce7:	023d                                    	mov.n	a3, a2
    2ce9:	078676                                    	loop	a6, 2cf4 <filter_bank_LC+0x1a4>
    2cec:	0368                                    	l32i.n	a6, a3, 0
    2cee:	334b                                    	addi.n	a3, a3, 4
    2cf0:	0569                                    	s32i.n	a6, a5, 0
    2cf2:	554b                                    	addi.n	a5, a5, 4

00002cf4 <filter_bank_LC+0x1a4>:
    2cf4:	5128                                    	l32i.n	a2, a1, 20
    2cf6:	80a082                                    	movi	a8, 128
    2cf9:	07d752                                    	addmi	a5, a7, 0x700
    2cfc:	07dc32                                    	addmi	a3, a12, 0x700
    2cff:	07db62                                    	addmi	a6, a11, 0x700
    2d02:	238876                                    	loop	a8, 2d29 <filter_bank_LC+0x1d9>
    2d05:	0588                                    	l32i.n	a8, a5, 0
    2d07:	229d                                    	ae_l16si.n	a9, a2, 0
    2d09:	06f8                                    	l32i.n	a15, a6, 0
    2d0b:	664b                                    	addi.n	a6, a6, 4
    2d0d:	554b                                    	addi.n	a5, a5, 4
    2d0f:	222b                                    	addi.n	a2, a2, 2
    2d11:	82a980                                    	mull	a10, a9, a8
    2d14:	b28980                                    	mulsh	a8, a9, a8
    2d17:	41afa0                                    	srli	a10, a10, 15
    2d1a:	0188f0                                    	slli	a8, a8, 17
    2d1d:	8a8a                                    	add.n	a8, a10, a8
    2d1f:	418180                                    	srli	a8, a8, 1
    2d22:	9088f0                                    	addx2	a8, a8, a15
    2d25:	0389                                    	s32i.n	a8, a3, 0
    2d27:	334b                                    	addi.n	a3, a3, 4
    2d29:	c0a162                                    	movi	a6, 0x1c0
    2d2c:	4188                                    	l32i.n	a8, a1, 16
    2d2e:	09d732                                    	addmi	a3, a7, 0x900
    2d31:	09dc22                                    	addmi	a2, a12, 0x900
    2d34:	09db52                                    	addmi	a5, a11, 0x900
    2d37:	0d8676                                    	loop	a6, 2d48 <filter_bank_LC+0x1f8>
    2d3a:	0568                                    	l32i.n	a6, a5, 0
    2d3c:	0378                                    	l32i.n	a7, a3, 0
    2d3e:	554b                                    	addi.n	a5, a5, 4
    2d40:	334b                                    	addi.n	a3, a3, 4
    2d42:	676a                                    	add.n	a6, a7, a6
    2d44:	0269                                    	s32i.n	a6, a2, 0
    2d46:	224b                                    	addi.n	a2, a2, 4
    2d48:	fea722                                    	movi	a2, 0x7fe
    2d4b:	00a452                                    	movi	a5, 0x400
    2d4e:	242a                                    	add.n	a2, a4, a2
    2d50:	e37c                                    	movi.n	a3, -2
    2d52:	1d8576                                    	loop	a5, 2d73 <filter_bank_LC+0x223>
    2d55:	0848                                    	l32i.n	a4, a8, 0
    2d57:	225d                                    	ae_l16si.n	a5, a2, 0
    2d59:	884b                                    	addi.n	a8, a8, 4
    2d5b:	fec222                                    	addi	a2, a2, -2
    2d5e:	826540                                    	mull	a6, a5, a4
    2d61:	b24540                                    	mulsh	a4, a5, a4
    2d64:	41ff60                                    	srli	a15, a6, 15
    2d67:	0144f0                                    	slli	a4, a4, 17
    2d6a:	4f4a                                    	add.n	a4, a15, a4
    2d6c:	104430                                    	and	a4, a4, a3
    2d6f:	0b49                                    	s32i.n	a4, a11, 0
    2d71:	bb4b                                    	addi.n	a11, a11, 4

00002d73 <filter_bank_LC+0x223>:
    2d73:	f01d                                    	retw.n

00002d75 <filter_bank_LC+0x225>:
	...

00002d76 <filter_bank_LC+0x226>:
    2d76:	f4f231                                    	l32r	a3, 140 (31d4 <imdct_long>)
    2d79:	05ad                                    	mov.n	a10, a5
    2d7b:	07bd                                    	mov.n	a11, a7
    2d7d:	0d5d                                    	mov.n	a5, a13
    2d7f:	41c9                                    	s32i.n	a12, a1, 16
    2d81:	0e6d                                    	mov.n	a6, a14
    2d83:	0003e0                                    	callx8	a3
    2d86:	41f8                                    	l32i.n	a15, a1, 16
    2d88:	00a182                                    	movi	a8, 0x100
    2d8b:	08d432                                    	addmi	a3, a4, 0x800
    2d8e:	fcc742                                    	addi	a4, a7, -4
    2d91:	fcc222                                    	addi	a2, a2, -4
    2d94:	fcc552                                    	addi	a5, a5, -4
    2d97:	fccf72                                    	addi	a7, a15, -4
    2d9a:	fcc662                                    	addi	a6, a6, -4
    2d9d:	b78876                                    	loop	a8, 2e58 <filter_bank_LC+0x308>
    2da0:	2e0473c00485d12e                  	{ ae_l16x2m.iu	aed0, a2, 4; ae_l16x2m.iu	aed5, a3, -4 }
    2da8:	0e54f3b90d2c4104c594bd001060057f 	{ ae_l32m.iu	aed4, a7, 4; ae_l32m.iu	aed1, a4, 4; ae_sel16i.n	aed3, aed0, aed0, 1; ae_sel16i	aed19, aed5, aed5, 1 }
    2db8:	0e9ce697b52221d0c5808e901085815f 	{ ae_l32m.iu	aed2, a5, 4; ae_l16x2m.iu	aed22, a2, 4; ae_mulf48q32sp16s.l	aed18, aed4, aed5; nop }
    2dc8:	1e8234153c239f3e                  	{ ae_l16x2m.iu	aed25, a3, -4; nop; ae_mulaf48q32sp16s.l	aed2, aed1, aed3 }
    2dd0:	33006cc010ddb16e                  	{ ae_s32m.iu	aed2, a6, 4; ae_sel16i	aed29, aed25, aed25, 1 }
    2dd8:	1005654410c2b05e                  	{ ae_s32m.i	aed18, a5, 0; ae_l32m.iu	aed2, a5, 4 }
    2de0:	1105274008d4714e                  	{ ae_l32m.iu	aed1, a4, 4; ae_l32m.iu	aed20, a7, 4 }
    2de8:	0e1d101b0bfd0ca5ab2b4e111420716f 	{ ae_sel16i	aed23, aed22, aed22, 1; nop; ae_mulaf48q32sp16s.l	aed2, aed1, aed0; ae_mulf48q32sp16s.l	aed21, aed20, aed19 }
    2df8:	230760c010c5b16e                  	{ ae_s32m.iu	aed2, a6, 4; nop }
    2e00:	1005654508d8b05e                  	{ ae_s32m.i	aed21, a5, 0; ae_l32m.iu	aed24, a5, 4 }
    2e08:	1105274010da714e                  	{ ae_l32m.iu	aed2, a4, 4; ae_l32m.iu	aed26, a7, 4 }
    2e10:	0e1d381b0d4c0cd5b72b4ec00c57015f 	{ nop; nop; ae_mulaf48q32sp16s.l	aed24, aed2, aed23; ae_mulf48q32sp16s.l	aed27, aed26, aed25 }
    2e20:	230760c600c5b16e                  	{ ae_s32m.iu	aed24, a6, 4; nop }
    2e28:	1005654618dcb05e                  	{ ae_s32m.i	aed27, a5, 0; ae_l32m.iu	aed28, a5, 4 }
    2e30:	1105274000de714e                  	{ ae_l32m.iu	aed0, a4, 4; ae_l32m.iu	aed30, a7, 4 }
    2e38:	0e1d3c1b0d4c0cf5bf2b4ee00c16015f 	{ nop; nop; ae_mulaf48q32sp16s.l	aed28, aed0, aed22; ae_mulf48q32sp16s.l	aed31, aed30, aed29 }
    2e48:	230760c700c5b16e                  	{ ae_s32m.iu	aed28, a6, 4; nop }
    2e50:	220760c718c5b05e                  	{ ae_s32m.i	aed31, a5, 0; nop }

00002e58 <filter_bank_LC+0x308>:
    2e58:	ffc5c6                                    	j	2d73 <filter_bank_LC+0x223>

00002e5b <filter_bank_LC+0x30b>:
    2e5b:	f4ba41                                    	l32r	a4, 144 (33cc <imdct_short>)
    2e5e:	05ad                                    	mov.n	a10, a5
    2e60:	5129                                    	s32i.n	a2, a1, 20
    2e62:	07bd                                    	mov.n	a11, a7
    2e64:	e1e9                                    	s32i.n	a14, a1, 56
    2e66:	0c2d                                    	mov.n	a2, a12
    2e68:	6169                                    	s32i.n	a6, a1, 24
    2e6a:	71d9                                    	s32i.n	a13, a1, 28
    2e6c:	3159                                    	s32i.n	a5, a1, 12
    2e6e:	0004e0                                    	callx8	a4
    2e71:	3138                                    	l32i.n	a3, a1, 12
    2e73:	04d7b2                                    	addmi	a11, a7, 0x400
    2e76:	02d3a2                                    	addmi	a10, a3, 0x200
    2e79:	0004e0                                    	callx8	a4
    2e7c:	3138                                    	l32i.n	a3, a1, 12
    2e7e:	08d7b2                                    	addmi	a11, a7, 0x800
    2e81:	04d3a2                                    	addmi	a10, a3, 0x400
    2e84:	b1a9                                    	s32i.n	a10, a1, 44
    2e86:	0004e0                                    	callx8	a4
    2e89:	3138                                    	l32i.n	a3, a1, 12
    2e8b:	0cd7b2                                    	addmi	a11, a7, 0xc00
    2e8e:	06d3a2                                    	addmi	a10, a3, 0x600
    2e91:	a1a9                                    	s32i.n	a10, a1, 40
    2e93:	0004e0                                    	callx8	a4
    2e96:	3138                                    	l32i.n	a3, a1, 12
    2e98:	02bd                                    	mov.n	a11, a2
    2e9a:	08d3a2                                    	addmi	a10, a3, 0x800
    2e9d:	0004e0                                    	callx8	a4
    2ea0:	3138                                    	l32i.n	a3, a1, 12
    2ea2:	04d2b2                                    	addmi	a11, a2, 0x400
    2ea5:	d1b9                                    	s32i.n	a11, a1, 52
    2ea7:	0ad3a2                                    	addmi	a10, a3, 0xa00
    2eaa:	c1a9                                    	s32i.n	a10, a1, 48
    2eac:	0004e0                                    	callx8	a4
    2eaf:	3138                                    	l32i.n	a3, a1, 12
    2eb1:	08d2b2                                    	addmi	a11, a2, 0x800
    2eb4:	1361b2                                    	s32i	a11, a1, 76
    2eb7:	0cd3a2                                    	addmi	a10, a3, 0xc00
    2eba:	91a9                                    	s32i.n	a10, a1, 36
    2ebc:	0004e0                                    	callx8	a4
    2ebf:	3138                                    	l32i.n	a3, a1, 12
    2ec1:	0cd2b2                                    	addmi	a11, a2, 0xc00
    2ec4:	4129                                    	s32i.n	a2, a1, 16
    2ec6:	00a462                                    	movi	a6, 0x400
    2ec9:	1261b2                                    	s32i	a11, a1, 72
    2ecc:	0ed3a2                                    	addmi	a10, a3, 0xe00
    2ecf:	1161a2                                    	s32i	a10, a1, 68
    2ed2:	0004e0                                    	callx8	a4
    2ed5:	31e8                                    	l32i.n	a14, a1, 12
    2ed7:	0e5d                                    	mov.n	a5, a14
    2ed9:	078676                                    	loop	a6, 2ee4 <filter_bank_LC+0x394>
    2edc:	0768                                    	l32i.n	a6, a7, 0
    2ede:	774b                                    	addi.n	a7, a7, 4
    2ee0:	0569                                    	s32i.n	a6, a5, 0
    2ee2:	554b                                    	addi.n	a5, a5, 4

00002ee4 <filter_bank_LC+0x394>:
    2ee4:	7128                                    	l32i.n	a2, a1, 28
    2ee6:	e148                                    	l32i.n	a4, a1, 56
    2ee8:	c0a172                                    	movi	a7, 0x1c0
    2eeb:	6138                                    	l32i.n	a3, a1, 24
    2eed:	025d                                    	mov.n	a5, a2
    2eef:	046d                                    	mov.n	a6, a4
    2ef1:	078776                                    	loop	a7, 2efc <filter_bank_LC+0x3ac>
    2ef4:	0578                                    	l32i.n	a7, a5, 0
    2ef6:	554b                                    	addi.n	a5, a5, 4
    2ef8:	0679                                    	s32i.n	a7, a6, 0
    2efa:	664b                                    	addi.n	a6, a6, 4
    2efc:	0dd4d2                                    	addmi	a13, a4, 0xd00
    2eff:	0dd2f2                                    	addmi	a15, a2, 0xd00
    2f02:	1061d2                                    	s32i	a13, a1, 64
    2f05:	fea052                                    	movi	a5, 254
    2f08:	3b03a143188201fe                  	{ s32i	a15, a1, 60; l32i	a10, a1, 44 }
    2f10:	80a062                                    	movi	a6, 128
    2f13:	a1c8                                    	l32i.n	a12, a1, 40
    2f15:	07d272                                    	addmi	a7, a2, 0x700
    2f18:	c1f8                                    	l32i.n	a15, a1, 48
    2f1a:	0bd2b2                                    	addmi	a11, a2, 0xb00
    2f1d:	91d8                                    	l32i.n	a13, a1, 36
    2f1f:	032d                                    	mov.n	a2, a3
    2f21:	07d482                                    	addmi	a8, a4, 0x700
    2f24:	0bd492                                    	addmi	a9, a4, 0xb00
    2f27:	335a                                    	add.n	a3, a3, a5
    2f29:	fa8676                                    	loop	a6, 3027 <filter_bank_LC+0x4d7>
    2f2c:	225d                                    	ae_l16si.n	a5, a2, 0
    2f2e:	0a48                                    	l32i.n	a4, a10, 0
    2f30:	a129                                    	s32i.n	a2, a1, 40
    2f32:	802e22                                    	l32i	a2, a14, 0x200
    2f35:	b199                                    	s32i.n	a9, a1, 44
    2f37:	236d                                    	ae_l16si.n	a6, a3, 0
    2f39:	802792                                    	l32i	a9, a7, 0x200
    2f3c:	9139                                    	s32i.n	a3, a1, 36
    2f3e:	8199                                    	s32i.n	a9, a1, 32
    2f40:	829540                                    	mull	a9, a5, a4
    2f43:	b24540                                    	mulsh	a4, a5, a4
    2f46:	823620                                    	mull	a3, a6, a2
    2f49:	b22620                                    	mulsh	a2, a6, a2
    2f4c:	0144f0                                    	slli	a4, a4, 17
    2f4f:	419f90                                    	srli	a9, a9, 15
    2f52:	494a                                    	add.n	a4, a9, a4
    2f54:	0122f0                                    	slli	a2, a2, 17
    2f57:	8198                                    	l32i.n	a9, a1, 32
    2f59:	413f30                                    	srli	a3, a3, 15
    2f5c:	232a                                    	add.n	a2, a3, a2
    2f5e:	413140                                    	srli	a3, a4, 1
    2f61:	412120                                    	srli	a2, a2, 1
    2f64:	c1f9                                    	s32i.n	a15, a1, 48
    2f66:	902290                                    	addx2	a2, a2, a9
    2f69:	902320                                    	addx2	a2, a3, a2
    2f6c:	0a3d                                    	mov.n	a3, a10
    2f6e:	806822                                    	s32i	a2, a8, 0x200
    2f71:	0c48                                    	l32i.n	a4, a12, 0
    2f73:	802c22                                    	l32i	a2, a12, 0x200
    2f76:	0b98                                    	l32i.n	a9, a11, 0
    2f78:	cc4b                                    	addi.n	a12, a12, 4
    2f7a:	8199                                    	s32i.n	a9, a1, 32
    2f7c:	bb4b                                    	addi.n	a11, a11, 4
    2f7e:	82a460                                    	mull	a10, a4, a6
    2f81:	b24460                                    	mulsh	a4, a4, a6
    2f84:	829250                                    	mull	a9, a2, a5
    2f87:	b22250                                    	mulsh	a2, a2, a5
    2f8a:	0144f0                                    	slli	a4, a4, 17
    2f8d:	41afa0                                    	srli	a10, a10, 15
    2f90:	4a4a                                    	add.n	a4, a10, a4
    2f92:	03ad                                    	mov.n	a10, a3
    2f94:	8138                                    	l32i.n	a3, a1, 32
    2f96:	0122f0                                    	slli	a2, a2, 17
    2f99:	419f90                                    	srli	a9, a9, 15
    2f9c:	414140                                    	srli	a4, a4, 1
    2f9f:	292a                                    	add.n	a2, a9, a2
    2fa1:	b198                                    	l32i.n	a9, a1, 44
    2fa3:	412120                                    	srli	a2, a2, 1
    2fa6:	903430                                    	addx2	a3, a4, a3
    2fa9:	902230                                    	addx2	a2, a2, a3
    2fac:	aa4b                                    	addi.n	a10, a10, 4
    2fae:	0929                                    	s32i.n	a2, a9, 0
    2fb0:	994b                                    	addi.n	a9, a9, 4
    2fb2:	0f28                                    	l32i.n	a2, a15, 0
    2fb4:	0d48                                    	l32i.n	a4, a13, 0
    2fb6:	dd4b                                    	addi.n	a13, a13, 4
    2fb8:	82f260                                    	mull	a15, a2, a6
    2fbb:	b22260                                    	mulsh	a2, a2, a6
    2fbe:	826450                                    	mull	a6, a4, a5
    2fc1:	b23450                                    	mulsh	a3, a4, a5
    2fc4:	414ff0                                    	srli	a4, a15, 15
    2fc7:	f1f8                                    	l32i.n	a15, a1, 60
    2fc9:	0122f0                                    	slli	a2, a2, 17
    2fcc:	0133f0                                    	slli	a3, a3, 17
    2fcf:	242a                                    	add.n	a2, a4, a2
    2fd1:	416f60                                    	srli	a6, a6, 15
    2fd4:	0f48                                    	l32i.n	a4, a15, 0
    2fd6:	363a                                    	add.n	a3, a6, a3
    2fd8:	102152                                    	l32i	a5, a1, 64
    2fdb:	412120                                    	srli	a2, a2, 1
    2fde:	5168                                    	l32i.n	a6, a1, 20
    2fe0:	413130                                    	srli	a3, a3, 1
    2fe3:	902240                                    	addx2	a2, a2, a4
    2fe6:	ff4b                                    	addi.n	a15, a15, 4
    2fe8:	902320                                    	addx2	a2, a3, a2
    2feb:	f1f9                                    	s32i.n	a15, a1, 60
    2fed:	0529                                    	s32i.n	a2, a5, 0
    2fef:	263d                                    	ae_l16si.n	a3, a6, 0
    2ff1:	0e28                                    	l32i.n	a2, a14, 0
    2ff3:	554b                                    	addi.n	a5, a5, 4
    2ff5:	662b                                    	addi.n	a6, a6, 2
    2ff7:	106152                                    	s32i	a5, a1, 64
    2ffa:	ee4b                                    	addi.n	a14, a14, 4
    2ffc:	5169                                    	s32i.n	a6, a1, 20
    2ffe:	824320                                    	mull	a4, a3, a2
    3001:	b22320                                    	mulsh	a2, a3, a2
    3004:	9138                                    	l32i.n	a3, a1, 36
    3006:	414f40                                    	srli	a4, a4, 15
    3009:	0122f0                                    	slli	a2, a2, 17
    300c:	242a                                    	add.n	a2, a4, a2
    300e:	0748                                    	l32i.n	a4, a7, 0
    3010:	412120                                    	srli	a2, a2, 1
    3013:	fec332                                    	addi	a3, a3, -2
    3016:	774b                                    	addi.n	a7, a7, 4
    3018:	902240                                    	addx2	a2, a2, a4
    301b:	a148                                    	l32i.n	a4, a1, 40
    301d:	0829                                    	s32i.n	a2, a8, 0
    301f:	884b                                    	addi.n	a8, a8, 4
    3021:	242b                                    	addi.n	a2, a4, 2
    3023:	c148                                    	l32i.n	a4, a1, 48
    3025:	f44b                                    	addi.n	a15, a4, 4
    3027:	e128                                    	l32i.n	a2, a1, 56
    3029:	71f8                                    	l32i.n	a15, a1, 28
    302b:	41d8                                    	l32i.n	a13, a1, 16
    302d:	1121c2                                    	l32i	a12, a1, 68
    3030:	61e8                                    	l32i.n	a14, a1, 24
    3032:	fea062                                    	movi	a6, 254
    3035:	074c                                    	movi.n	a7, 64
    3037:	0fd222                                    	addmi	a2, a2, 0xf00
    303a:	0d4d                                    	mov.n	a4, a13
    303c:	0fdf32                                    	addmi	a3, a15, 0xf00
    303f:	6e6a                                    	add.n	a6, a14, a6
    3041:	0e5d                                    	mov.n	a5, a14
    3043:	408776                                    	loop	a7, 3087 <filter_bank_LC+0x537>
    3046:	0c78                                    	l32i.n	a7, a12, 0
    3048:	268d                                    	ae_l16si.n	a8, a6, 0
    304a:	0498                                    	l32i.n	a9, a4, 0
    304c:	25ad                                    	ae_l16si.n	a10, a5, 0
    304e:	cc4b                                    	addi.n	a12, a12, 4
    3050:	fec662                                    	addi	a6, a6, -2
    3053:	82b870                                    	mull	a11, a8, a7
    3056:	b27870                                    	mulsh	a7, a8, a7
    3059:	828a90                                    	mull	a8, a10, a9
    305c:	b29a90                                    	mulsh	a9, a10, a9
    305f:	0177f0                                    	slli	a7, a7, 17
    3062:	41bfb0                                    	srli	a11, a11, 15
    3065:	7b7a                                    	add.n	a7, a11, a7
    3067:	03b8                                    	l32i.n	a11, a3, 0
    3069:	0199f0                                    	slli	a9, a9, 17
    306c:	418f80                                    	srli	a8, a8, 15
    306f:	889a                                    	add.n	a8, a8, a9
    3071:	417170                                    	srli	a7, a7, 1
    3074:	418180                                    	srli	a8, a8, 1
    3077:	9077b0                                    	addx2	a7, a7, a11
    307a:	907870                                    	addx2	a7, a8, a7
    307d:	334b                                    	addi.n	a3, a3, 4
    307f:	0279                                    	s32i.n	a7, a2, 0
    3081:	444b                                    	addi.n	a4, a4, 4
    3083:	552b                                    	addi.n	a5, a5, 2
    3085:	224b                                    	addi.n	a2, a2, 4
    3087:	3148                                    	l32i.n	a4, a1, 12
    3089:	80a072                                    	movi	a7, 128
    308c:	084c                                    	movi.n	a8, 64
    308e:	01dd22                                    	addmi	a2, a13, 0x100
    3091:	7ece32                                    	addi	a3, a14, 126
    3094:	e57c                                    	movi.n	a5, -2
    3096:	0fd442                                    	addmi	a4, a4, 0xf00
    3099:	0f6d                                    	mov.n	a6, a15
    309b:	7e7a                                    	add.n	a7, a14, a7
    309d:	398876                                    	loop	a8, 30da <filter_bank_LC+0x58a>
    30a0:	0488                                    	l32i.n	a8, a4, 0
    30a2:	239d                                    	ae_l16si.n	a9, a3, 0
    30a4:	02a8                                    	l32i.n	a10, a2, 0
    30a6:	27bd                                    	ae_l16si.n	a11, a7, 0
    30a8:	444b                                    	addi.n	a4, a4, 4
    30aa:	fec332                                    	addi	a3, a3, -2
    30ad:	82c980                                    	mull	a12, a9, a8
    30b0:	b28980                                    	mulsh	a8, a9, a8
    30b3:	829ba0                                    	mull	a9, a11, a10
    30b6:	b2aba0                                    	mulsh	a10, a11, a10
    30b9:	0188f0                                    	slli	a8, a8, 17
    30bc:	41cfc0                                    	srli	a12, a12, 15
    30bf:	01aaf0                                    	slli	a10, a10, 17
    30c2:	419f90                                    	srli	a9, a9, 15
    30c5:	99aa                                    	add.n	a9, a9, a10
    30c7:	8c8a                                    	add.n	a8, a12, a8
    30c9:	109950                                    	and	a9, a9, a5
    30cc:	418180                                    	srli	a8, a8, 1
    30cf:	908890                                    	addx2	a8, a8, a9
    30d2:	224b                                    	addi.n	a2, a2, 4
    30d4:	0689                                    	s32i.n	a8, a6, 0
    30d6:	772b                                    	addi.n	a7, a7, 2
    30d8:	664b                                    	addi.n	a6, a6, 4
    30da:	05df52                                    	addmi	a5, a15, 0x500
    30dd:	0f7d                                    	mov.n	a7, a15
    30df:	d168                                    	l32i.n	a6, a1, 52
    30e1:	1321f2                                    	l32i	a15, a1, 76
    30e4:	fea082                                    	movi	a8, 254
    30e7:	122122                                    	l32i	a2, a1, 72
    30ea:	80a092                                    	movi	a9, 128
    30ed:	02dda2                                    	addmi	a10, a13, 0x200
    30f0:	06dd32                                    	addmi	a3, a13, 0x600
    30f3:	0edd42                                    	addmi	a4, a13, 0xe00
    30f6:	808e80                                    	add	a8, a14, a8
    30f9:	c28976                                    	loop	a9, 31bf <filter_bank_LC+0x66f>
    30fc:	289d                                    	ae_l16si.n	a9, a8, 0
    30fe:	0ab8                                    	l32i.n	a11, a10, 0
    3100:	51a9                                    	s32i.n	a10, a1, 20
    3102:	06c8                                    	l32i.n	a12, a6, 0
    3104:	2ead                                    	ae_l16si.n	a10, a14, 0
    3106:	d169                                    	s32i.n	a6, a1, 52
    3108:	82d9b0                                    	mull	a13, a9, a11
    310b:	b2b9b0                                    	mulsh	a11, a9, a11
    310e:	0e6d                                    	mov.n	a6, a14
    3110:	41dfd0                                    	srli	a13, a13, 15
    3113:	b2eac0                                    	mulsh	a14, a10, a12
    3116:	82cac0                                    	mull	a12, a10, a12
    3119:	01bbf0                                    	slli	a11, a11, 17
    311c:	01eef0                                    	slli	a14, a14, 17
    311f:	41cfc0                                    	srli	a12, a12, 15
    3122:	bdba                                    	add.n	a11, a13, a11
    3124:	ccea                                    	add.n	a12, a12, a14
    3126:	ee7c                                    	movi.n	a14, -2
    3128:	10cce0                                    	and	a12, a12, a14
    312b:	41b1b0                                    	srli	a11, a11, 1
    312e:	90bbc0                                    	addx2	a11, a11, a12
    3131:	fec882                                    	addi	a8, a8, -2
    3134:	4067b2                                    	s32i	a11, a7, 0x100
    3137:	03b8                                    	l32i.n	a11, a3, 0
    3139:	0fc8                                    	l32i.n	a12, a15, 0
    313b:	334b                                    	addi.n	a3, a3, 4
    313d:	82db90                                    	mull	a13, a11, a9
    3140:	b2bb90                                    	mulsh	a11, a11, a9
    3143:	82eca0                                    	mull	a14, a12, a10
    3146:	b2cca0                                    	mulsh	a12, a12, a10
    3149:	01bbf0                                    	slli	a11, a11, 17
    314c:	41dfd0                                    	srli	a13, a13, 15
    314f:	bdba                                    	add.n	a11, a13, a11
    3151:	41dfe0                                    	srli	a13, a14, 15
    3154:	01ccf0                                    	slli	a12, a12, 17
    3157:	41b1b0                                    	srli	a11, a11, 1
    315a:	cdca                                    	add.n	a12, a13, a12
    315c:	ed7c                                    	movi.n	a13, -2
    315e:	10ccd0                                    	and	a12, a12, a13
    3161:	e62b                                    	addi.n	a14, a6, 2
    3163:	90bbc0                                    	addx2	a11, a11, a12
    3166:	c067b2                                    	s32i	a11, a7, 0x300
    3169:	774b                                    	addi.n	a7, a7, 4
    316b:	02b8                                    	l32i.n	a11, a2, 0
    316d:	802fc2                                    	l32i	a12, a15, 0x200
    3170:	224b                                    	addi.n	a2, a2, 4
    3172:	ff4b                                    	addi.n	a15, a15, 4
    3174:	82dba0                                    	mull	a13, a11, a10
    3177:	b2aba0                                    	mulsh	a10, a11, a10
    317a:	826c90                                    	mull	a6, a12, a9
    317d:	b2cc90                                    	mulsh	a12, a12, a9
    3180:	01aaf0                                    	slli	a10, a10, 17
    3183:	41dfd0                                    	srli	a13, a13, 15
    3186:	01ccf0                                    	slli	a12, a12, 17
    3189:	41bf60                                    	srli	a11, a6, 15
    318c:	bbca                                    	add.n	a11, a11, a12
    318e:	adaa                                    	add.n	a10, a13, a10
    3190:	ec7c                                    	movi.n	a12, -2
    3192:	41b1b0                                    	srli	a11, a11, 1
    3195:	10aac0                                    	and	a10, a10, a12
    3198:	90aba0                                    	addx2	a10, a11, a10
    319b:	51b8                                    	l32i.n	a11, a1, 20
    319d:	05a9                                    	s32i.n	a10, a5, 0
    319f:	04a8                                    	l32i.n	a10, a4, 0
    31a1:	444b                                    	addi.n	a4, a4, 4
    31a3:	b2da90                                    	mulsh	a13, a10, a9
    31a6:	829a90                                    	mull	a9, a10, a9
    31a9:	016df0                                    	slli	a6, a13, 17
    31ac:	d1d8                                    	l32i.n	a13, a1, 52
    31ae:	419f90                                    	srli	a9, a9, 15
    31b1:	ab4b                                    	addi.n	a10, a11, 4
    31b3:	996a                                    	add.n	a9, a9, a6
    31b5:	1099c0                                    	and	a9, a9, a12
    31b8:	6d4b                                    	addi.n	a6, a13, 4
    31ba:	806592                                    	s32i	a9, a5, 0x200
    31bd:	554b                                    	addi.n	a5, a5, 4
    31bf:	7128                                    	l32i.n	a2, a1, 28
    31c1:	09d2a2                                    	addmi	a10, a2, 0x900

000031c4 <filter_bank_LC+0x674>:
    31c4:	f39b21                                    	l32r	a2, 30 (3878 <memset>)
    31c7:	0b0c                                    	movi.n	a11, 0
    31c9:	00a7c2                                    	movi	a12, 0x700
    31cc:	0002e0                                    	callx8	a2
    31cf:	f01d                                    	retw.n

000031d1 <filter_bank_LC+0x681>:
    31d1:	000000                                        ...

000031d4 <imdct_long>:
    31d4:	004136                                    	entry	a1, 32
    31d7:	ffa062                                    	movi	a6, 255
    31da:	f3db51                                    	l32r	a5, 148 (75d0 <mdct_twi2048_L>)
    31dd:	1166c0                                    	slli	a6, a6, 4
    31e0:	00a192                                    	movi	a9, 0x100
    31e3:	626a                                    	add.n	a6, a2, a6
    31e5:	037d                                    	mov.n	a7, a3
    31e7:	028d                                    	mov.n	a8, a2
    31e9:	f03d                                    	nop.n
    31eb:	f03d                                    	nop.n
    31ed:	678976                                    	loop	a9, 3258 <imdct_long+0x84>
    31f0:	000080e350a06f0058031320a800808f 	{ ae_l32x2x2.ip	aed0, aed1, a8, 16; ae_l32x2x2.ip	aed2, aed3, a6, -16; nop }
    3200:	0e5533bb0d350c0045045e1c1c00165f 	{ ae_l32x2.ip	aed4, a5, 16; nop; ae_sel16i.n	aed0, aed0, aed3, 2; ae_sel16i	aed1, aed1, aed2, 2 }
    3210:	0e1d24db0d250c00862bd90718047e5f 	{ ae_l32x2.i	aed29, a5, -8; nop; ae_mulzaafd32s.hl.lh	aed0, aed0, aed4; ae_mulzsafd32s.hh.ll	aed3, aed0, aed4 }
    3220:	0e1d3cdb0d4c0c08bd2bd9080c3d015f 	{ nop; nop; ae_mulzaafd32s.hl.lh	aed1, aed1, aed29; ae_mulzsafd32s.hh.ll	aed30, aed1, aed29 }
    3230:	0ea5241b0d440cd0c50083000fc0015f 	{ nop; nop; ae_round32x2f64ssym	aed0, aed3, aed0; nop }
    3240:	0ea5241b0d250cd0c4009e091fdf017f 	{ ae_s32x2.ip	aed0, a7, 8; nop; ae_round32x2f64ssym	aed31, aed30, aed1; nop }
    3250:	230700c71ce5997e                  	{ ae_s32x2.ip	aed31, a7, 8; nop }

00003258 <imdct_long+0x84>:
    3258:	f3bd71                                    	l32r	a7, 14c (6410 <fft_twi512_L>)
    325b:	08d352                                    	addmi	a5, a3, 0x800
    325e:	00a1b2                                    	movi	a11, 0x100
    3261:	08d262                                    	addmi	a6, a2, 0x800
    3264:	209330                                    	or	a9, a3, a3
    3267:	208220                                    	or	a8, a2, a2
    326a:	20a550                                    	or	a10, a5, a5
    326d:	2f8b76                                    	loop	a11, 32a0 <imdct_long+0xcc>
    3270:	390639c014a0d97e                  	{ ae_l32x2.ip	aed2, a7, 8; ae_l32x2.ip	aed0, a9, 8 }
    3278:	050021400ca2c9ae                  	{ ae_l32x2.ip	aed1, a10, 8; ae_sel16i	aed2, aed2, aed2, 0 }
    3280:	0ead257b0a240cd0c40081380801200f 	{ ae_addandsub32s	aed0, aed1, aed0, aed1; nop; nop; nop }
    3290:	2282760a2822038e                  	{ ae_s32x2.ip	aed0, a8, 8; nop; ae_mulfc32ras	aed1, aed1, aed2 }
    3298:	230700c00ce5996e                  	{ ae_s32x2.ip	aed1, a6, 8; nop }
    32a0:	f3ab61                                    	l32r	a6, 14c (6410 <fft_twi512_L>)
    32a3:	01a172                                    	movi	a7, 0x101
    32a6:	1177d0                                    	slli	a7, a7, 3
    32a9:	0a4c                                    	movi.n	a10, 64
    32ab:	2b0c                                    	movi.n	a11, 2
    32ad:	00a2f2                                    	movi	a15, 0x200
    32b0:	e67a                                    	add.n	a14, a6, a7
    32b2:	f3a771                                    	l32r	a7, 150 (3554 <aac_passf4pos>)
    32b5:	02cd                                    	mov.n	a12, a2
    32b7:	03dd                                    	mov.n	a13, a3
    32b9:	0007e0                                    	callx8	a7
    32bc:	c1a182                                    	movi	a8, 0x1c1
    32bf:	0a1c                                    	movi.n	a10, 16
    32c1:	1188d0                                    	slli	a8, a8, 3
    32c4:	8b0c                                    	movi.n	a11, 8
    32c6:	e68a                                    	add.n	a14, a6, a8
    32c8:	00a2f2                                    	movi	a15, 0x200
    32cb:	03cd                                    	mov.n	a12, a3
    32cd:	02dd                                    	mov.n	a13, a2
    32cf:	0007e0                                    	callx8	a7
    32d2:	f1a182                                    	movi	a8, 0x1f1
    32d5:	4a0c                                    	movi.n	a10, 4
    32d7:	1188d0                                    	slli	a8, a8, 3
    32da:	0b2c                                    	movi.n	a11, 32
    32dc:	e68a                                    	add.n	a14, a6, a8
    32de:	00a2f2                                    	movi	a15, 0x200
    32e1:	02cd                                    	mov.n	a12, a2
    32e3:	03dd                                    	mov.n	a13, a3
    32e5:	0007e0                                    	callx8	a7
    32e8:	f39b61                                    	l32r	a6, 154 (3694 <aac_passf4pos_ido1>)
    32eb:	80a0a2                                    	movi	a10, 128
    32ee:	03bd                                    	mov.n	a11, a3
    32f0:	02cd                                    	mov.n	a12, a2
    32f2:	0006e0                                    	callx8	a6
    32f5:	f39461                                    	l32r	a6, 148 (75d0 <mdct_twi2048_L>)
    32f8:	fca792                                    	movi	a9, 0x7fc
    32fb:	00a1a2                                    	movi	a10, 0x100
    32fe:	839a                                    	add.n	a8, a3, a9
    3300:	08d472                                    	addmi	a7, a4, 0x800
    3303:	949a                                    	add.n	a9, a4, a9
    3305:	578a76                                    	loop	a10, 3360 <imdct_long+0x18c>
    3308:	390636c004a1d92e                  	{ ae_l32x2.ip	aed0, a2, 8; ae_l32x2.ip	aed1, a6, 8 }
    3310:	0e1d20fb0d4c0c01052bf7180c01015f 	{ nop; nop; ae_mulzasfd32s.hl.lh	aed3, aed0, aed1; ae_mulzssfd32s.hh.ll	aed2, aed0, aed1 }
    3320:	2c8348033420b13e                  	{ nop; nop; ae_mulzasfd32s.hl.lh	aed0, aed1, aed0 }
    3328:	0ea5241b0d440cd0c50082180fc2015f 	{ nop; nop; ae_round32x2f64ssym	aed2, aed2, aed3; nop }
    3338:	0ea5241b0c440cd0c58080010fc0827f 	{ ae_s32.h.ip	aed2, a7, 8; nop; ae_round32x2f64ssym	aed0, aed0, aed0; nop }
    3348:	230760c014c59e9e                  	{ ae_s32.h.ip	aed2, a9, -8; nop }
    3350:	220760c014c5f25e                  	{ ae_s32.l.ip	aed2, a5, 8; nop }
    3358:	220760c004c5fe8e                  	{ ae_s32.l.ip	aed0, a8, -8; nop }
    3360:	ffa372                                    	movi	a7, 0x3ff
    3363:	00a182                                    	movi	a8, 0x100
    3366:	1177e0                                    	slli	a7, a7, 2
    3369:	547a                                    	add.n	a5, a4, a7
    336b:	737a                                    	add.n	a7, a3, a7
    336d:	578876                                    	loop	a8, 33c8 <imdct_long+0x1f4>
    3370:	390636c004a1d92e                  	{ ae_l32x2.ip	aed0, a2, 8; ae_l32x2.ip	aed1, a6, 8 }
    3378:	0e1d213b0d4c0c07c52f12180c01015f 	{ nop; nop; ae_mulzssfd32s.hh.ll	aed3, aed0, aed1; ae_mulzaafd32s.hh.ll	aed2, aed0, aed1 }
    3388:	2c8348033401b13e                  	{ nop; nop; ae_mulzasfd32s.hl.lh	aed0, aed0, aed1 }
    3390:	0ea5241b0d440cd0c50082180fdf015f 	{ nop; nop; ae_round32x2f64ssym	aed31, aed2, aed3; nop }
    33a0:	0ea5241b0c440cd0c58080030fc0ce3f 	{ ae_s32.h.ip	aed31, a3, 8; nop; ae_round32x2f64ssym	aed0, aed0, aed0; nop }
    33b0:	220760c71cc5fe7e                  	{ ae_s32.l.ip	aed31, a7, -8; nop }
    33b8:	220760c004c5f24e                  	{ ae_s32.l.ip	aed0, a4, 8; nop }
    33c0:	220760c004c5fe5e                  	{ ae_s32.l.ip	aed0, a5, -8; nop }
    33c8:	f01d                                    	retw.n

000033ca <imdct_long+0x1f6>:
	...

000033cc <imdct_short>:
    33cc:	004136                                    	entry	a1, 32
    33cf:	f0a152                                    	movi	a5, 0x1f0
    33d2:	f36141                                    	l32r	a4, 158 (85d0 <mdct_twi256_L>)
    33d5:	082c                                    	movi.n	a8, 32
    33d7:	525a                                    	add.n	a5, a2, a5
    33d9:	027d                                    	mov.n	a7, a2
    33db:	036d                                    	mov.n	a6, a3
    33dd:	678876                                    	loop	a8, 3448 <imdct_short+0x7c>
    33e0:	000080e350a05f0058031320a800807f 	{ ae_l32x2x2.ip	aed0, aed1, a7, 16; ae_l32x2x2.ip	aed2, aed3, a5, -16; nop }
    33f0:	0e5533bb0d350c0045045e1c1c00164f 	{ ae_l32x2.ip	aed4, a4, 16; nop; ae_sel16i.n	aed0, aed0, aed3, 2; ae_sel16i	aed1, aed1, aed2, 2 }
    3400:	0e1d24db0d250c00862bd90718047e4f 	{ ae_l32x2.i	aed29, a4, -8; nop; ae_mulzaafd32s.hl.lh	aed0, aed0, aed4; ae_mulzsafd32s.hh.ll	aed3, aed0, aed4 }
    3410:	0e1d3cdb0d4c0c08bd2bd9080c3d015f 	{ nop; nop; ae_mulzaafd32s.hl.lh	aed1, aed1, aed29; ae_mulzsafd32s.hh.ll	aed30, aed1, aed29 }
    3420:	0ea5241b0d440cd0c50083000fc0015f 	{ nop; nop; ae_round32x2f64ssym	aed0, aed3, aed0; nop }
    3430:	0ea5241b0d250cd0c4009e091fdf016f 	{ ae_s32x2.ip	aed0, a6, 8; nop; ae_round32x2f64ssym	aed31, aed30, aed1; nop }
    3440:	230700c71ce5996e                  	{ ae_s32x2.ip	aed31, a6, 8; nop }

00003448 <imdct_short+0x7c>:
    3448:	f34541                                    	l32r	a4, 15c (73f0 <fft_twi64_L>)
    344b:	0a1c                                    	movi.n	a10, 16
    344d:	f34051                                    	l32r	a5, 150 (3554 <aac_passf4pos>)
    3450:	1b0c                                    	movi.n	a11, 1
    3452:	0f4c                                    	movi.n	a15, 64
    3454:	03cd                                    	mov.n	a12, a3
    3456:	e48b                                    	addi.n	a14, a4, 8
    3458:	02dd                                    	mov.n	a13, a2
    345a:	0005e0                                    	callx8	a5
    345d:	88a162                                    	movi	a6, 0x188
    3460:	4a0c                                    	movi.n	a10, 4
    3462:	e46a                                    	add.n	a14, a4, a6
    3464:	4b0c                                    	movi.n	a11, 4
    3466:	0f4c                                    	movi.n	a15, 64
    3468:	02cd                                    	mov.n	a12, a2
    346a:	03dd                                    	mov.n	a13, a3
    346c:	fca072                                    	movi	a7, 252
    346f:	0005e0                                    	callx8	a5
    3472:	f33841                                    	l32r	a4, 154 (3694 <aac_passf4pos_ido1>)
    3475:	0a1c                                    	movi.n	a10, 16
    3477:	03bd                                    	mov.n	a11, a3
    3479:	02cd                                    	mov.n	a12, a2
    347b:	0004e0                                    	callx8	a4
    347e:	f33641                                    	l32r	a4, 158 (85d0 <mdct_twi256_L>)
    3481:	20a082                                    	movi	a8, 32
    3484:	01d352                                    	addmi	a5, a3, 0x100
    3487:	00a262                                    	movi	a6, 0x200
    348a:	807370                                    	add	a7, a3, a7
    348d:	578876                                    	loop	a8, 34e8 <imdct_short+0x11c>
    3490:	390634c004a1d92e                  	{ ae_l32x2.ip	aed0, a2, 8; ae_l32x2.ip	aed1, a4, 8 }
    3498:	0e1d20fb0d4c0c01052bf7180c01015f 	{ nop; nop; ae_mulzasfd32s.hl.lh	aed3, aed0, aed1; ae_mulzssfd32s.hh.ll	aed2, aed0, aed1 }
    34a8:	2c8348033420b13e                  	{ nop; nop; ae_mulzasfd32s.hl.lh	aed0, aed1, aed0 }
    34b0:	0ea5241b0d440cd0c50082180fc2015f 	{ nop; nop; ae_round32x2f64ssym	aed2, aed2, aed3; nop }
    34c0:	0ea5241b0c450cd0c58080010fc0925f 	{ ae_s32.h.x	aed2, a5, a6; nop; ae_round32x2f64ssym	aed0, aed0, aed0; nop }
    34d0:	220760c014c5b67e                  	{ ae_s32.h.x	aed2, a7, a6; nop }
    34d8:	220760c014c5f25e                  	{ ae_s32.l.ip	aed2, a5, 8; nop }
    34e0:	220760c004c5fe7e                  	{ ae_s32.l.ip	aed0, a7, -8; nop }
    34e8:	fca362                                    	movi	a6, 0x3fc
    34eb:	072c                                    	movi.n	a7, 32
    34ed:	02d352                                    	addmi	a5, a3, 0x200
    34f0:	336a                                    	add.n	a3, a3, a6
    34f2:	00ae62                                    	movi	a6, 0xfffffe00
    34f5:	578776                                    	loop	a7, 3550 <imdct_short+0x184>
    34f8:	390634c004a1d92e                  	{ ae_l32x2.ip	aed0, a2, 8; ae_l32x2.ip	aed1, a4, 8 }
    3500:	0e1d213b0d4c0c07c52f12180c01015f 	{ nop; nop; ae_mulzssfd32s.hh.ll	aed3, aed0, aed1; ae_mulzaafd32s.hh.ll	aed2, aed0, aed1 }
    3510:	2c8348033401b13e                  	{ nop; nop; ae_mulzasfd32s.hl.lh	aed0, aed0, aed1 }
    3518:	0ea5241b0d440cd0c50082180fdf015f 	{ nop; nop; ae_round32x2f64ssym	aed31, aed2, aed3; nop }
    3528:	0ea5241b0c450cd0c58080030fc0de5f 	{ ae_s32.h.x	aed31, a5, a6; nop; ae_round32x2f64ssym	aed0, aed0, aed0; nop }
    3538:	230760c71cc5f63e                  	{ ae_s32.l.x	aed31, a3, a6; nop }
    3540:	220760c004c5f25e                  	{ ae_s32.l.ip	aed0, a5, 8; nop }
    3548:	220760c004c5fe3e                  	{ ae_s32.l.ip	aed0, a3, -8; nop }
    3550:	f01d                                    	retw.n

00003552 <imdct_short+0x186>:
	...

00003554 <aac_passf4pos>:
    3554:	004136                                    	entry	a1, 32
    3557:	41a170                                    	srli	a10, a7, 1
    355a:	0213e6                                    	bgei	a3, 1, 3560 <aac_passf4pos+0xc>
    355d:	004bc6                                    	j	3690 <aac_passf4pos+0x13c>
    3560:	1172d0                                    	slli	a7, a2, 3
    3563:	908220                                    	addx2	a8, a2, a2
    3566:	11aae0                                    	slli	a10, a10, 2
    3569:	ffc292                                    	addi	a9, a2, -1

0000356c <aac_passf4pos+0x18>:
    356c:	b0b240                                    	addx8	a11, a2, a4
    356f:	390634c00cc017be                  	{ ae_l32x2.xp	aed1, a11, a7; ae_l32x2.ip	aed0, a4, 8 }
    3577:	230720c014c517be                  	{ ae_l32x2.xp	aed2, a11, a7; nop }
    357f:	0e9566865d235ad0c40080150040c2bf 	{ ae_l32x2.i	aed3, a11, 0; add	a11, a5, a10; ae_addandsub32s	aed0, aed2, aed0, aed2; nop }
    358f:	0ead257b0a240cd0c40081381801611f 	{ ae_addandsub32s	aed1, aed3, aed1, aed3; nop; nop; nop }
    359f:	0e95069b09e40cd0c5808008182003df 	{ ae_mul32js	aed3, aed3; nop; ae_addandsub32s	aed0, aed1, aed0, aed1; nop }
    35af:	0e95269b0d250cd0c40082191c62015f 	{ ae_s32x2.ip	aed0, a5, 8; nop; ae_addandsub32s	aed2, aed3, aed2, aed3; nop }
    35bf:	230760c014c57abe                  	{ ae_s32x2.xp	aed2, a11, a10; nop }
    35c7:	230760c00cc57abe                  	{ ae_s32x2.xp	aed1, a11, a10; nop }
    35cf:	3606964018ea82be                  	{ ae_s32x2.i	aed3, a11, 0; or	a11, a6, a6 }
    35d7:	0222e6                                    	bgei	a2, 2, 35dd <aac_passf4pos+0x89>
    35da:	002a86                                    	j	3688 <aac_passf4pos+0x134>
    35dd:	a78976                                    	loop	a9, 3688 <aac_passf4pos+0x134>
    35e0:	390614c704e3324e                  	{ addx8	a12, a2, a4; ae_l32x2.ip	aed3, a4, 8 }
    35e8:	1b06a54004d307ce                  	{ ae_l32x2.xp	aed0, a12, a7; add	a13, a5, a10 }
    35f0:	230720c00cc517ce                  	{ ae_l32x2.xp	aed1, a12, a7; nop }
    35f8:	0e956686e5252bd0c400830d006182cf 	{ ae_l32x2.i	aed2, a12, 0; addx8	a12, a2, a11; ae_addandsub32s	aed1, aed3, aed3, aed1; nop }
    3608:	0e95269b0d250cd0c50080141c4007bf 	{ ae_l32x2.ip	aed4, a11, 8; nop; ae_addandsub32s	aed0, aed2, aed0, aed2; nop }
    3618:	0e55129b09e40c0105908100102002df 	{ ae_mul32js	aed2, aed2; nop; ae_addandsub32s	aed0, aed1, aed1, aed0; ae_sel16i	aed4, aed4, aed4, 0 }
    3628:	0e95269b0d250cd0c40083111c62015f 	{ ae_s32x2.ip	aed0, a5, 8; nop; ae_addandsub32s	aed2, aed3, aed3, aed2; nop }
    3638:	228356133444b13e                  	{ nop; nop; ae_mulfc32ras	aed2, aed2, aed4 }
    3640:	230760c014c57ade                  	{ ae_s32x2.xp	aed2, a13, a10; nop }
    3648:	230720c004c517ce                  	{ ae_l32x2.xp	aed0, a12, a7; nop }
    3650:	0100204318e0647e                  	{ nop; ae_sel16i	aed0, aed0, aed0, 0 }
    3658:	228356033420b13e                  	{ nop; nop; ae_mulfc32ras	aed0, aed1, aed0 }
    3660:	230760c004c57ade                  	{ ae_s32x2.xp	aed0, a13, a10; nop }
    3668:	220700c000e512ce                  	{ ae_l32x2.i	aed0, a12, 0; nop }
    3670:	0100204318e0647e                  	{ nop; ae_sel16i	aed0, aed0, aed0, 0 }
    3678:	228356033460b13e                  	{ nop; nop; ae_mulfc32ras	aed0, aed3, aed0 }
    3680:	220700c000e592de                  	{ ae_s32x2.i	aed0, a13, 0; nop }

00003688 <aac_passf4pos+0x134>:
    3688:	330b                                    	addi.n	a3, a3, -1
    368a:	b04840                                    	addx8	a4, a8, a4
    368d:	edb356                                    	bnez	a3, 356c <aac_passf4pos+0x18>

00003690 <aac_passf4pos+0x13c>:
    3690:	f01d                                    	retw.n

00003692 <aac_passf4pos+0x13e>:
	...

00003694 <aac_passf4pos_ido1>:
    3694:	004136                                    	entry	a1, 32
    3697:	906220                                    	addx2	a6, a2, a2
    369a:	6112a6                                    	blti	a2, 1, 36ff <aac_passf4pos_ido1+0x6b>
    369d:	1172c0                                    	slli	a7, a2, 4
    36a0:	b05240                                    	addx8	a5, a2, a4
    36a3:	b06640                                    	addx8	a6, a6, a4
    36a6:	807470                                    	add	a7, a4, a7
    36a9:	528276                                    	loop	a2, 36ff <aac_passf4pos_ido1+0x6b>
    36ac:	1328                                    	l32i.n	a2, a3, 4
    36ae:	5388                                    	l32i.n	a8, a3, 20
    36b0:	7398                                    	l32i.n	a9, a3, 28
    36b2:	33a8                                    	l32i.n	a10, a3, 12
    36b4:	23b8                                    	l32i.n	a11, a3, 8
    36b6:	63c8                                    	l32i.n	a12, a3, 24
    36b8:	d82a                                    	add.n	a13, a8, a2
    36ba:	c02280                                    	sub	a2, a2, a8
    36bd:	ea9a                                    	add.n	a14, a10, a9
    36bf:	c099a0                                    	sub	a9, a9, a10
    36c2:	feda                                    	add.n	a15, a14, a13
    36c4:	c08de0                                    	sub	a8, a13, a14
    36c7:	c0ebc0                                    	sub	a14, a11, a12
    36ca:	14f9                                    	s32i.n	a15, a4, 4
    36cc:	f2ea                                    	add.n	a15, a2, a14
    36ce:	c022e0                                    	sub	a2, a2, a14
    36d1:	1789                                    	s32i.n	a8, a7, 4
    36d3:	ccba                                    	add.n	a12, a12, a11
    36d5:	15f9                                    	s32i.n	a15, a5, 4
    36d7:	1629                                    	s32i.n	a2, a6, 4
    36d9:	0328                                    	l32i.n	a2, a3, 0
    36db:	4388                                    	l32i.n	a8, a3, 16
    36dd:	20c332                                    	addi	a3, a3, 32
    36e0:	d82a                                    	add.n	a13, a8, a2
    36e2:	c02280                                    	sub	a2, a2, a8
    36e5:	edca                                    	add.n	a14, a13, a12
    36e7:	f29a                                    	add.n	a15, a2, a9
    36e9:	c0adc0                                    	sub	a10, a13, a12
    36ec:	c02290                                    	sub	a2, a2, a9
    36ef:	04e9                                    	s32i.n	a14, a4, 0
    36f1:	448b                                    	addi.n	a4, a4, 8
    36f3:	07a9                                    	s32i.n	a10, a7, 0
    36f5:	778b                                    	addi.n	a7, a7, 8
    36f7:	05f9                                    	s32i.n	a15, a5, 0
    36f9:	558b                                    	addi.n	a5, a5, 8
    36fb:	0629                                    	s32i.n	a2, a6, 0
    36fd:	668b                                    	addi.n	a6, a6, 8

000036ff <aac_passf4pos_ido1+0x6b>:
    36ff:	f01d                                    	retw.n

00003701 <aac_passf4pos_ido1+0x6d>:
    3701:	000000                                        ...

00003704 <_start>:
    3704:	004136                                    	entry	a1, 32
    3707:	f29631                                    	l32r	a3, 160 (99e0 <printf_ptr>)
    370a:	0228                                    	l32i.n	a2, a2, 0
    370c:	f296a1                                    	l32r	a10, 164 (457c <export_parameter_array+0xc>)
    370f:	f296b1                                    	l32r	a11, 168 (210 <AIR_AAC_init>)
    3712:	0329                                    	s32i.n	a2, a3, 0
    3714:	0002e0                                    	callx8	a2
    3717:	0328                                    	l32i.n	a2, a3, 0
    3719:	f294a1                                    	l32r	a10, 16c (458f <export_parameter_array+0x1f>)
    371c:	f295b1                                    	l32r	a11, 170 (280 <AIR_AAC_decoder>)
    371f:	0002e0                                    	callx8	a2
    3722:	0328                                    	l32i.n	a2, a3, 0
    3724:	f294a1                                    	l32r	a10, 174 (45a5 <export_parameter_array+0x35>)
    3727:	f294b1                                    	l32r	a11, 178 (2e0 <get_AACdec_memsize>)
    372a:	0002e0                                    	callx8	a2
    372d:	f29321                                    	l32r	a2, 17c (4570 <export_parameter_array>)
    3730:	f01d                                    	retw.n

00003732 <_start+0x2e>:
	...

00003740 <window_grouping_info>:
    3740:	008136                                    	entry	a1, 64
    3743:	010372                                    	l8ui	a7, a3, 1
    3746:	080262                                    	l8ui	a6, a2, 8
    3749:	024d                                    	mov.n	a4, a2
    374b:	10c352                                    	addi	a5, a3, 16
    374e:	f28f81                                    	l32r	a8, 18c (4a30 <num_swb_128_window>)
    3751:	aba2f2                                    	movi	a15, 0x2ab
    3754:	120c                                    	movi.n	a2, 1
    3756:	0b27b6                                    	bltui	a7, 2, 3765 <window_grouping_info+0x25>
    3759:	909ff0                                    	addx2	a9, a15, a15
    375c:	482726                                    	beqi	a7, 2, 37a8 <window_grouping_info+0x68>
    375f:	023726                                    	beqi	a7, 3, 3765 <window_grouping_info+0x25>
    3762:	004406                                    	j	3876 <window_grouping_info+0x136>

00003765 <window_grouping_info+0x25>:
    3765:	f28621                                    	l32r	a2, 180 (4df0 <swb_offset_1024_window>)
    3768:	01a182                                    	movi	a8, 0x101
    376b:	f28671                                    	l32r	a7, 184 (4a20 <num_swb_1024_window>)
    376e:	1f0c                                    	movi.n	a15, 1
    3770:	a02620                                    	addx4	a2, a6, a2
    3773:	776a                                    	add.n	a7, a7, a6
    3775:	0268                                    	l32i.n	a6, a2, 0
    3777:	000722                                    	l8ui	a2, a7, 0
    377a:	035382                                    	s16i	a8, a3, 6
    377d:	0843f2                                    	s8i	a15, a3, 8
    3780:	054322                                    	s8i	a2, a3, 5
    3783:	e28c                                    	beqz.n	a2, 3795 <window_grouping_info+0x55>
    3785:	0c8276                                    	loop	a2, 3795 <window_grouping_info+0x55>
    3788:	267d                                    	ae_l16si.n	a7, a6, 0
    378a:	657d                                    	ae_s16i.n	a7, a5, 0
    378c:	267d                                    	ae_l16si.n	a7, a6, 0
    378e:	662b                                    	addi.n	a6, a6, 2
    3790:	785572                                    	s16i	a7, a5, 240
    3793:	552b                                    	addi.n	a5, a5, 2

00003795 <window_grouping_info+0x55>:
    3795:	903230                                    	addx2	a3, a2, a3
    3798:	029462                                    	l16si	a6, a4, 4
    379b:	020c                                    	movi.n	a2, 0
    379d:	656d                                    	ae_s16i.n	a6, a5, 0
    379f:	0294f2                                    	l16si	a15, a4, 4
    37a2:	8053f2                                    	s16i	a15, a3, 0x100
    37a5:	f01d                                    	retw.n

000037a7 <window_grouping_info+0x67>:
	...

000037a8 <window_grouping_info+0x68>:
    37a8:	f27821                                    	l32r	a2, 188 (4b00 <swb_offset_128_window>)
    37ab:	e86a                                    	add.n	a14, a8, a6
    37ad:	170c                                    	movi.n	a7, 1
    37af:	01d382                                    	addmi	a8, a3, 0x100
    37b2:	a02620                                    	addx4	a2, a6, a2
    37b5:	000e62                                    	l8ui	a6, a14, 0
    37b8:	0228                                    	l32i.n	a2, a2, 0
    37ba:	035392                                    	s16i	a9, a3, 6
    37bd:	4129                                    	s32i.n	a2, a1, 16
    37bf:	4198                                    	l32i.n	a9, a1, 16
    37c1:	084372                                    	s8i	a7, a3, 8
    37c4:	054362                                    	s8i	a6, a3, 5
    37c7:	968c                                    	beqz.n	a6, 37d4 <window_grouping_info+0x94>
    37c9:	078676                                    	loop	a6, 37d4 <window_grouping_info+0x94>
    37cc:	29ad                                    	ae_l16si.n	a10, a9, 0
    37ce:	992b                                    	addi.n	a9, a9, 2
    37d0:	68ad                                    	ae_s16i.n	a10, a8, 0
    37d2:	882b                                    	addi.n	a8, a8, 2

000037d4 <window_grouping_info+0x94>:
    37d4:	021482                                    	l16ui	a8, a4, 4
    37d7:	030392                                    	l8ui	a9, a3, 3
    37da:	90b630                                    	addx2	a11, a6, a3
    37dd:	0a0c                                    	movi.n	a10, 0
    37df:	418380                                    	srli	a8, a8, 3
    37e2:	805b82                                    	s16i	a8, a11, 0x100
    37e5:	6b0c                                    	movi.n	a11, 6
    37e7:	180c                                    	movi.n	a8, 1
    37e9:	0005c6                                    	j	3804 <window_grouping_info+0xc4>

000037ec <window_grouping_info+0xac>:
    37ec:	881b                                    	addi.n	a8, a8, 1
    37ee:	b8cd                                    	ae_zext8	a12, a8
    37f0:	cc3a                                    	add.n	a12, a12, a3
    37f2:	064382                                    	s8i	a8, a3, 6
    37f5:	074c72                                    	s8i	a7, a12, 7
    37f8:	0703c2                                    	l8ui	a12, a3, 7
    37fb:	aa1b                                    	addi.n	a10, a10, 1
    37fd:	bb0b                                    	addi.n	a11, a11, -1
    37ff:	cc0b                                    	addi.n	a12, a12, -1
    3801:	11aac7                                    	bge	a10, a12, 3816 <window_grouping_info+0xd6>
    3804:	e459b7                                    	bbc	a9, a11, 37ec <window_grouping_info+0xac>
    3807:	b8cd                                    	ae_zext8	a12, a8
    3809:	cc3a                                    	add.n	a12, a12, a3
    380b:	070cd2                                    	l8ui	a13, a12, 7
    380e:	dd1b                                    	addi.n	a13, a13, 1
    3810:	074cd2                                    	s8i	a13, a12, 7
    3813:	fff846                                    	j	37f8 <window_grouping_info+0xb8>

00003816 <window_grouping_info+0xd6>:
    3816:	ffa072                                    	movi	a7, 255
    3819:	f38b                                    	addi.n	a15, a3, 8
    381b:	090c                                    	movi.n	a9, 0
    381d:	530787                                    	bnone	a7, a8, 3874 <window_grouping_info+0x134>
    3820:	b82d                                    	ae_zext8	a2, a8
    3822:	6129                                    	s32i.n	a2, a1, 24
    3824:	1cc182                                    	addi	a8, a1, 28
    3827:	51f9                                    	s32i.n	a15, a1, 20
    3829:	444b                                    	addi.n	a4, a4, 4
    382b:	0a0c                                    	movi.n	a10, 0
    382d:	66bc                                    	beqz.n	a6, 3867 <window_grouping_info+0x127>
    382f:	5128                                    	l32i.n	a2, a1, 20
    3831:	0a0c                                    	movi.n	a10, 0
    3833:	41d8                                    	l32i.n	a13, a1, 16
    3835:	06cd                                    	mov.n	a12, a6
    3837:	b29a                                    	add.n	a11, a2, a9
    3839:	2a8676                                    	loop	a6, 3867 <window_grouping_info+0x127>
    383c:	ec0b                                    	addi.n	a14, a12, -1
    383e:	04fd                                    	mov.n	a15, a4
    3840:	93f8e0                                    	movnez	a15, a8, a14
    3843:	2d2b                                    	addi.n	a2, a13, 2
    3845:	4ffd                                    	ae_l16ui.n	a15, a15, 0
    3847:	8328e0                                    	moveqz	a2, a8, a14
    384a:	422d                                    	ae_l16ui.n	a2, a2, 0
    384c:	000b72                                    	l8ui	a7, a11, 0
    384f:	4d3d                                    	ae_l16ui.n	a3, a13, 0
    3851:	cc0b                                    	addi.n	a12, a12, -1
    3853:	41f3f0                                    	srli	a15, a15, 3
    3856:	65ad                                    	ae_s16i.n	a10, a5, 0
    3858:	93f2e0                                    	movnez	a15, a2, a14
    385b:	dd2b                                    	addi.n	a13, a13, 2
    385d:	c02f30                                    	sub	a2, a15, a3
    3860:	552b                                    	addi.n	a5, a5, 2
    3862:	822270                                    	mull	a2, a2, a7
    3865:	a2aa                                    	add.n	a10, a2, a10

00003867 <window_grouping_info+0x127>:
    3867:	61f8                                    	l32i.n	a15, a1, 24
    3869:	252b                                    	addi.n	a2, a5, 2
    386b:	65ad                                    	ae_s16i.n	a10, a5, 0
    386d:	991b                                    	addi.n	a9, a9, 1
    386f:	025d                                    	mov.n	a5, a2
    3871:	b639f7                                    	bltu	a9, a15, 382b <window_grouping_info+0xeb>

00003874 <window_grouping_info+0x134>:
    3874:	020c                                    	movi.n	a2, 0
    3876:	f01d                                    	retw.n

00003878 <memset>:
    3878:	004136                                    	entry	a1, 32
    387b:	025d                                    	mov.n	a5, a2
    387d:	648c                                    	beqz.n	a4, 3887 <memset+0xf>
    387f:	048476                                    	loop	a4, 3887 <memset+0xf>
    3882:	004532                                    	s8i	a3, a5, 0
    3885:	551b                                    	addi.n	a5, a5, 1

00003887 <memset+0xf>:
    3887:	f01d                                    	retw.n

00003889 <memset+0x11>:
    3889:	000000                                        ...

0000388c <xt_memset8_0>:
    388c:	004136                                    	entry	a1, 32
    388f:	f8c222                                    	addi	a2, a2, -8
    3892:	339c                                    	beqz.n	a3, 38a9 <xt_memset8_0+0x1d>
    3894:	0302c24004a24c0e                  	{ ae_movi	aed0, 0; addi	a2, a2, 8 }
    389c:	f03d                                    	nop.n
    389e:	078376                                    	loop	a3, 38a9 <xt_memset8_0+0x1d>
    38a1:	220700c004e5992e                  	{ ae_s32x2f24.ip	aed0, a2, 8; nop }

000038a9 <xt_memset8_0+0x1d>:
    38a9:	f01d                                    	retw.n

000038ab <xt_memset8_0+0x1f>:
	...

000038ac <xt_memcpy4>:
    38ac:	004136                                    	entry	a1, 32
    38af:	948c                                    	beqz.n	a4, 38bc <xt_memcpy4+0x10>
    38b1:	078476                                    	loop	a4, 38bc <xt_memcpy4+0x10>
    38b4:	0348                                    	l32i.n	a4, a3, 0
    38b6:	334b                                    	addi.n	a3, a3, 4
    38b8:	0249                                    	s32i.n	a4, a2, 0
    38ba:	224b                                    	addi.n	a2, a2, 4

000038bc <xt_memcpy4+0x10>:
    38bc:	f01d                                    	retw.n

000038be <xt_memcpy4+0x12>:
	...

000038c0 <is_decode>:
    38c0:	00e136                                    	entry	a1, 112
    38c3:	0603f2                                    	l8ui	a15, a3, 6
    38c6:	d159                                    	s32i.n	a5, a1, 52
    38c8:	c149                                    	s32i.n	a4, a1, 48
    38ca:	867b                                    	addi.n	a8, a6, 7
    38cc:	41a442                                    	movi	a4, 0x441
    38cf:	6139                                    	s32i.n	a3, a1, 24
    38d1:	a1f9                                    	s32i.n	a15, a1, 40
    38d3:	04d2c2                                    	addmi	a12, a2, 0x400
    38d6:	6178                                    	l32i.n	a7, a1, 24
    38d8:	169f16                                    	beqz	a15, 3a45 <is_decode+0x185>
    38db:	68a1f2                                    	movi	a15, 0x168
    38de:	b38660                                    	movgez	a8, a6, a6
    38e1:	1261c2                                    	s32i	a12, a1, 72
    38e4:	a24a                                    	add.n	a10, a2, a4
    38e6:	000792                                    	l8ui	a9, a7, 0
    38e9:	e78b                                    	addi.n	a14, a7, 8
    38eb:	0002d2                                    	l8ui	a13, a2, 0
    38ee:	c7fa                                    	add.n	a12, a7, a15
    38f0:	81e9                                    	s32i.n	a14, a1, 32
    38f2:	21e380                                    	srai	a14, a8, 3
    38f5:	c0a352                                    	movi	a5, 0x3c0
    38f8:	11f9f0                                    	slli	a15, a9, 1
    38fb:	91d9                                    	s32i.n	a13, a1, 36
    38fd:	1d0c                                    	movi.n	a13, 1
    38ff:	51a9                                    	s32i.n	a10, a1, 20
    3901:	fb0c                                    	movi.n	a11, 15
    3903:	040c                                    	movi.n	a4, 0
    3905:	080c                                    	movi.n	a8, 0
    3907:	a75a                                    	add.n	a10, a7, a5
    3909:	41c9                                    	s32i.n	a12, a1, 16
    390b:	f1e9                                    	s32i.n	a14, a1, 60
    390d:	71f9                                    	s32i.n	a15, a1, 28
    390f:	8128                                    	l32i.n	a2, a1, 32
    3911:	0007c6                                    	j	3934 <is_decode+0x74>

00003914 <is_decode+0x54>:
    3914:	b148                                    	l32i.n	a4, a1, 44
    3916:	5138                                    	l32i.n	a3, a1, 20
    3918:	9128                                    	l32i.n	a2, a1, 36
    391a:	41c8                                    	l32i.n	a12, a1, 16
    391c:	71e8                                    	l32i.n	a14, a1, 28
    391e:	aa9a                                    	add.n	a10, a10, a9
    3920:	a1f8                                    	l32i.n	a15, a1, 40
    3922:	441b                                    	addi.n	a4, a4, 1
    3924:	332a                                    	add.n	a3, a3, a2
    3926:	2cea                                    	add.n	a2, a12, a14
    3928:	5139                                    	s32i.n	a3, a1, 20
    392a:	4129                                    	s32i.n	a2, a1, 16
    392c:	8128                                    	l32i.n	a2, a1, 32
    392e:	0234f7                                    	bltu	a4, a15, 3934 <is_decode+0x74>
    3931:	004406                                    	j	3a45 <is_decode+0x185>
    3934:	b149                                    	s32i.n	a4, a1, 44
    3936:	030c                                    	movi.n	a3, 0
    3938:	224a                                    	add.n	a2, a2, a4
    393a:	000242                                    	l8ui	a4, a2, 0
    393d:	e129                                    	s32i.n	a2, a1, 56
    393f:	fd1416                                    	beqz	a4, 3914 <is_decode+0x54>
    3942:	0004c6                                    	j	3959 <is_decode+0x99>

00003945 <is_decode+0x85>:
    3945:	e128                                    	l32i.n	a2, a1, 56
    3947:	112182                                    	l32i	a8, a1, 68
    394a:	102132                                    	l32i	a3, a1, 64
    394d:	000242                                    	l8ui	a4, a2, 0
    3950:	f128                                    	l32i.n	a2, a1, 60
    3952:	331b                                    	addi.n	a3, a3, 1
    3954:	882a                                    	add.n	a8, a8, a2
    3956:	bab347                                    	bgeu	a3, a4, 3914 <is_decode+0x54>
    3959:	c128                                    	l32i.n	a2, a1, 48
    395b:	ff1916                                    	beqz	a9, 3950 <is_decode+0x90>
    395e:	d1f8                                    	l32i.n	a15, a1, 52
    3960:	106132                                    	s32i	a3, a1, 64
    3963:	060c                                    	movi.n	a6, 0
    3965:	116182                                    	s32i	a8, a1, 68
    3968:	a02820                                    	addx4	a2, a8, a2
    396b:	3a6a                                    	add.n	a3, a10, a6
    396d:	a078f0                                    	addx4	a7, a8, a15
    3970:	136122                                    	s32i	a2, a1, 76
    3973:	000a46                                    	j	39a0 <is_decode+0xe0>

00003976 <is_decode+0xb6>:
    3976:	400200                                    	ssr	a2
    3979:	fb0c                                    	movi.n	a11, 15
    397b:	855d                                    	ae_sext16	a5, a5
    397d:	188876                                    	loop	a8, 3999 <is_decode+0xd9>
    3980:	0f28                                    	l32i.n	a2, a15, 0
    3982:	ff4b                                    	addi.n	a15, a15, 4
    3984:	b12020                                    	sra	a2, a2
    3987:	b23250                                    	mulsh	a3, a2, a5
    398a:	822250                                    	mull	a2, a2, a5
    398d:	113300                                    	slli	a3, a3, 16
    3990:	f52020                                    	extui	a2, a2, 16, 16
    3993:	223a                                    	add.n	a2, a2, a3
    3995:	0629                                    	s32i.n	a2, a6, 0
    3997:	664b                                    	addi.n	a6, a6, 4

00003999 <is_decode+0xd9>:
    3999:	0c6d                                    	mov.n	a6, a12
    399b:	3a6a                                    	add.n	a3, a10, a6
    399d:	a4bc97                                    	bgeu	a12, a9, 3945 <is_decode+0x85>

000039a0 <is_decode+0xe0>:
    39a0:	0003f2                                    	l8ui	a15, a3, 0
    39a3:	c61b                                    	addi.n	a12, a6, 1
    39a5:	4128                                    	l32i.n	a2, a1, 16
    39a7:	fe7c                                    	movi.n	a14, -1
    39a9:	6188                                    	l32i.n	a8, a1, 24
    39ab:	040c                                    	movi.n	a4, 0
    39ad:	203fd0                                    	or	a3, a15, a13
    39b0:	0213b7                                    	beq	a3, a11, 39b6 <is_decode+0xf6>
    39b3:	0022c6                                    	j	3a42 <is_decode+0x182>
    39b6:	51b8                                    	l32i.n	a11, a1, 20
    39b8:	c61b                                    	addi.n	a12, a6, 1
    39ba:	01d852                                    	addmi	a5, a8, 0x100
    39bd:	903620                                    	addx2	a3, a6, a2
    39c0:	f2cf82                                    	addi	a8, a15, -14
    39c3:	f1cff2                                    	addi	a15, a15, -15
    39c6:	93e480                                    	movnez	a14, a4, a8
    39c9:	2b6a                                    	add.n	a2, a11, a6
    39cb:	122182                                    	l32i	a8, a1, 72
    39ce:	23bd                                    	ae_l16si.n	a11, a3, 0
    39d0:	83edf0                                    	moveqz	a14, a13, a15
    39d3:	f1eff1                                    	l32r	a15, 190 (45e0 <pow05_table_m>)
    39d6:	906650                                    	addx2	a6, a6, a5
    39d9:	000222                                    	l8ui	a2, a2, 0
    39dc:	905c50                                    	addx2	a5, a12, a5
    39df:	400832                                    	l8ui	a3, a8, 64
    39e2:	454d                                    	ae_l16ui.n	a4, a5, 0
    39e4:	1480b0                                    	extui	a8, a11, 0, 2
    39e7:	a058f0                                    	addx4	a5, a8, a15
    39ea:	1122f0                                    	slli	a2, a2, 1
    39ed:	0588                                    	l32i.n	a8, a5, 0
    39ef:	330b                                    	addi.n	a3, a3, -1
    39f1:	466d                                    	ae_l16ui.n	a6, a6, 0
    39f3:	c02d20                                    	sub	a2, a13, a2
    39f6:	1321f2                                    	l32i	a15, a1, 76
    39f9:	932d30                                    	movnez	a2, a13, a3
    39fc:	c02e20                                    	sub	a2, a14, a2
    39ff:	605080                                    	neg	a5, a8
    3a02:	835820                                    	moveqz	a5, a8, a2
    3a05:	2132b0                                    	srai	a3, a11, 2
    3a08:	a0f6f0                                    	addx4	a15, a6, a15
    3a0b:	c08460                                    	sub	a8, a4, a6
    3a0e:	230b                                    	addi.n	a2, a3, -1
    3a10:	a06670                                    	addx4	a6, a6, a7
    3a13:	c04d30                                    	sub	a4, a13, a3
    3a16:	024ba6                                    	blti	a11, 4, 3a1c <is_decode+0x15c>
    3a19:	ffd646                                    	j	3976 <is_decode+0xb6>
    3a1c:	401400                                    	ssl	a4
    3a1f:	fb0c                                    	movi.n	a11, 15
    3a21:	855d                                    	ae_sext16	a5, a5
    3a23:	188876                                    	loop	a8, 3a3f <is_decode+0x17f>
    3a26:	0f28                                    	l32i.n	a2, a15, 0
    3a28:	ff4b                                    	addi.n	a15, a15, 4
    3a2a:	a12200                                    	sll	a2, a2
    3a2d:	b23250                                    	mulsh	a3, a2, a5
    3a30:	822250                                    	mull	a2, a2, a5
    3a33:	113300                                    	slli	a3, a3, 16
    3a36:	f52020                                    	extui	a2, a2, 16, 16
    3a39:	223a                                    	add.n	a2, a2, a3
    3a3b:	0629                                    	s32i.n	a2, a6, 0
    3a3d:	664b                                    	addi.n	a6, a6, 4

00003a3f <is_decode+0x17f>:
    3a3f:	ffd586                                    	j	3999 <is_decode+0xd9>

00003a42 <is_decode+0x182>:
    3a42:	ffd4c6                                    	j	3999 <is_decode+0xd9>

00003a45 <is_decode+0x185>:
    3a45:	f01d                                    	retw.n

00003a47 <is_decode+0x187>:
	...

00003a48 <__do_global_ctors_aux>:
    3a48:	004136                                    	entry	a1, 32
    3a4b:	f1d231                                    	l32r	a3, 194 (452c <__CTOR_END__>)
    3a4e:	fcc322                                    	addi	a2, a3, -4
    3a51:	002222                                    	l32i	a2, a2, 0
    3a54:	020283a4001e920e                  	{ beqi.w15	a2, -1, 3a69 <__do_global_ctors_aux+0x21>; addi	a3, a3, -8 }

00003a5c <__do_global_ctors_aux+0x14>:
    3a5c:	0002e0                                    	callx8	a2
    3a5f:	0328                                    	l32i.n	a2, a3, 0
    3a61:	0302e3bc07ffb26e                  	{ bnei.w15	a2, -1, 3a5c <__do_global_ctors_aux+0x14>; addi	a3, a3, -4 }
    3a69:	f01d                                    	retw.n

00003a6b <__do_global_ctors_aux+0x23>:
	...

00003a6c <__udivdi3>:
    3a6c:	004136                                    	entry	a1, 32
    3a6f:	3206b24510e9e33e                  	{ or	a6, a3, a3; or	a7, a2, a2 }
    3a77:	03046020040055ae                  	{ beqz.w15	a5, 3b65 <__udivdi3+0xf9>; movi	a3, 0 }
    3a7f:	020460030c00065e                  	{ bltu.w15	a6, a5, 3b63 <__udivdi3+0xf7>; movi	a2, 0 }
    3a87:	40f5a0                                    	nsau	a10, a5
    3a8a:	2304202000402aae                  	{ beqz.w15	a10, 3cd4 <__udivdi3+0x268>; movi	a2, 32 }
    3a92:	3b06b24318fa6a5e                  	{ ssl	a10; sub	a11, a2, a10 }
    3a9a:	33074bc308a435ce                  	{ sll	a13, a5; ssr	a11 }
    3aa2:	23074ac714a4714e                  	{ srl	a14, a4; ssl	a10 }
    3aaa:	000724c410e5eede                  	{ or	a2, a14, a13; sll	a4, a4 }
    3ab2:	030230c318fe6b6e                  	{ ssr	a11; extui	a8, a2, 16, 16 }
    3aba:	23074ac50ca4716e                  	{ srl	a5, a6; ssl	a10 }
    3ac2:	33074bc318a436ce                  	{ sll	a15, a6; ssr	a11 }
    3aca:	c2b580                                    	quou	a11, a5, a8
    3acd:	82eb80                                    	mull	a14, a11, a8
    3ad0:	91d070                                    	srl	a13, a7
    3ad3:	1e06b54510f9edfe                  	{ or	a6, a13, a15; sub	a5, a5, a14 }
    3adb:	929d                                    	ae_zext16	a9, a2
    3add:	82cb90                                    	mull	a12, a11, a9
    3ae0:	1106e5470865e06e                  	{ extui	a13, a6, 16, 16; slli	a5, a5, 16 }
    3ae8:	1a069b430ce935de                  	{ add	a13, a5, a13; or	a5, a11, a11 }
    3af0:	23070a810804ddce                  	{ bgeu.w15	a13, a12, 3b0e <__udivdi3+0xa2>; ssl	a10 }
    3af8:	34028bc30cff3d2e                  	{ add	a13, a13, a2; addi	a5, a11, -1 }
    3b00:	0a3d27                                    	bltu	a13, a2, 3b0e <__udivdi3+0xa2>
    3b03:	07bdc7                                    	bgeu	a13, a12, 3b0e <__udivdi3+0xa2>
    3b06:	1206ed471473eb5e                  	{ addi	a5, a11, -2; add	a13, a13, a2 }

00003b0e <__udivdi3+0xa2>:
    3b0e:	c0bdc0                                    	sub	a11, a13, a12
    3b11:	c2ab80                                    	quou	a10, a11, a8
    3b14:	828a80                                    	mull	a8, a10, a8
    3b17:	96fd                                    	ae_zext16	a15, a6
    3b19:	c08b80                                    	sub	a8, a11, a8
    3b1c:	118800                                    	slli	a8, a8, 16
    3b1f:	826a90                                    	mull	a6, a10, a9
    3b22:	1a069a4204ea28fe                  	{ add	a8, a8, a15; or	a9, a10, a10 }
    3b2a:	14b867                                    	bgeu	a8, a6, 3b42 <__udivdi3+0xd6>
    3b2d:	882a                                    	add.n	a8, a8, a2
    3b2f:	39028a87081f782e                  	{ bltu.w15	a8, a2, 3b42 <__udivdi3+0xd6>; addi	a9, a10, -1 }
    3b37:	07b867                                    	bgeu	a8, a6, 3b42 <__udivdi3+0xd6>
    3b3a:	28028ac204ff382e                  	{ add	a8, a8, a2; addi	a9, a10, -2 }
    3b42:	2706b84410b9050e                  	{ slli	a2, a5, 16; sub	a6, a8, a6 }
    3b4a:	202920                                    	or	a2, a9, a2
    3b4d:	a25240                                    	muluh	a5, a2, a4
    3b50:	824240                                    	mull	a4, a2, a4
    3b53:	310707830805465e                  	{ bltu.w15	a6, a5, 3b61 <__udivdi3+0xf5>; sll	a7, a7 }
    3b5b:	04b747                                    	bgeu	a7, a4, 3b63 <__udivdi3+0xf7>
    3b5e:	019657                                    	bne	a6, a5, 3b63 <__udivdi3+0xf7>
    3b61:	220b                                    	addi.n	a2, a2, -1
    3b63:	f01d                                    	retw.n

00003b65 <__udivdi3+0xf9>:
    3b65:	40f430                                    	nsau	a3, a4
    3b68:	220440010c00664e                  	{ bgeu.w15	a6, a4, 3c18 <__udivdi3+0x1ac>; movi	a2, 32 }
    3b70:	23040024040023ae                  	{ beqz.w15	a3, 3b9b <__udivdi3+0x12f>; movi	a2, 32 }
    3b78:	2306b24318f8735e                  	{ ssl	a3; sub	a2, a2, a3 }
    3b80:	110746c100a524ce                  	{ sll	a4, a4; sll	a5, a6 }
    3b88:	400200                                    	ssr	a2
    3b8b:	230743c414a4717e                  	{ srl	a2, a7; ssl	a3 }
    3b93:	300727c510e5e25e                  	{ or	a6, a2, a5; sll	a7, a7 }
    3b9b:	370270c4107ff04e                  	{ extui	a2, a4, 16, 16; extui	a15, a7, 16, 16 }
    3ba3:	c28620                                    	quou	a8, a6, a2
    3ba6:	825820                                    	mull	a5, a8, a2
    3ba9:	943d                                    	ae_zext16	a3, a4
    3bab:	c05650                                    	sub	a5, a6, a5
    3bae:	115500                                    	slli	a5, a5, 16
    3bb1:	829830                                    	mull	a9, a8, a3
    3bb4:	1806984114e925fe                  	{ add	a6, a5, a15; or	a5, a8, a8 }
    3bbc:	15b697                                    	bgeu	a6, a9, 3bd5 <__udivdi3+0x169>
    3bbf:	340288c114ff364e                  	{ add	a6, a6, a4; addi	a5, a8, -1 }
    3bc7:	0a3647                                    	bltu	a6, a4, 3bd5 <__udivdi3+0x169>
    3bca:	07b697                                    	bgeu	a6, a9, 3bd5 <__udivdi3+0x169>
    3bcd:	2406e6471471e85e                  	{ addi	a5, a8, -2; add	a6, a6, a4 }
    3bd5:	c08690                                    	sub	a8, a6, a9
    3bd8:	c26820                                    	quou	a6, a8, a2
    3bdb:	82e620                                    	mull	a14, a6, a2
    3bde:	977d                                    	ae_zext16	a7, a7
    3be0:	3b040fc31cff48ee                  	{ sub	a15, a8, a14; movi	a10, -1 }
    3be8:	822630                                    	mull	a2, a6, a3
    3beb:	113f00                                    	slli	a3, a15, 16
    3bee:	337a                                    	add.n	a3, a3, a7
    3bf0:	340683010810a32e                  	{ bgeu.w15	a3, a2, 3c08 <__udivdi3+0x19c>; add	a3, a3, a4 }
    3bf8:	3406934014f0432e                  	{ saltu	a2, a3, a2; saltu	a3, a3, a4 }
    3c00:	c02a20                                    	sub	a2, a10, a2
    3c03:	932a30                                    	movnez	a2, a10, a3
    3c06:	626a                                    	add.n	a6, a2, a6
    3c08:	0304204410a0150e                  	{ slli	a2, a5, 16; movi	a3, 0 }
    3c10:	202620                                    	or	a2, a6, a2
    3c13:	f01d                                    	retw.n

00003c15 <__udivdi3+0x1a9>:
    3c15:	000000                                        ...

00003c18 <__udivdi3+0x1ac>:
    3c18:	0206f220001a93ae                  	{ beqz.w15	a3, 3cec <__udivdi3+0x280>; sub	a8, a2, a3 }
    3c20:	35042ec318ff735e                  	{ ssl	a3; movi	a5, -2 }
    3c28:	330748c100a434ce                  	{ sll	a4, a4; ssr	a8 }
    3c30:	250250c614bc616e                  	{ srl	a10, a6; extui	a2, a4, 16, 16 }
    3c38:	150230c318f9735e                  	{ ssl	a3; extui	a5, a5, 0, 16 }
    3c40:	c29a20                                    	quou	a9, a10, a2
    3c43:	330748c318a436ce                  	{ sll	a15, a6; ssr	a8 }
    3c4b:	82c920                                    	mull	a12, a9, a2
    3c4e:	1506c4c61ca5717e                  	{ srl	a11, a7; and	a5, a4, a5 }
    3c56:	2c06ba4510faebfe                  	{ or	a6, a11, a15; sub	a10, a10, a12 }
    3c5e:	828950                                    	mull	a8, a9, a5
    3c61:	2106ea461866e06e                  	{ extui	a11, a6, 16, 16; slli	a10, a10, 16 }
    3c69:	2b06aa4318f2735e                  	{ ssl	a3; add	a10, a10, a11 }
    3c71:	093d                                    	mov.n	a3, a9
    3c73:	15ba87                                    	bgeu	a10, a8, 3c8c <__udivdi3+0x220>
    3c76:	320289c214ff3a4e                  	{ add	a10, a10, a4; addi	a3, a9, -1 }
    3c7e:	0a3a47                                    	bltu	a10, a4, 3c8c <__udivdi3+0x220>
    3c81:	07ba87                                    	bgeu	a10, a8, 3c8c <__udivdi3+0x220>
    3c84:	2406ea471472e93e                  	{ addi	a3, a9, -2; add	a10, a10, a4 }
    3c8c:	c09a80                                    	sub	a9, a10, a8
    3c8f:	c28920                                    	quou	a8, a9, a2
    3c92:	82f820                                    	mull	a15, a8, a2
    3c95:	966d                                    	ae_zext16	a6, a6
    3c97:	c099f0                                    	sub	a9, a9, a15
    3c9a:	119900                                    	slli	a9, a9, 16
    3c9d:	825850                                    	mull	a5, a8, a5
    3ca0:	1806984114ea296e                  	{ add	a6, a9, a6; or	a9, a8, a8 }
    3ca8:	14b657                                    	bgeu	a6, a5, 3cc0 <__udivdi3+0x254>
    3cab:	664a                                    	add.n	a6, a6, a4
    3cad:	39028887081f764e                  	{ bltu.w15	a6, a4, 3cc0 <__udivdi3+0x254>; addi	a9, a8, -1 }
    3cb5:	07b657                                    	bgeu	a6, a5, 3cc0 <__udivdi3+0x254>
    3cb8:	2406e6471471e89e                  	{ addi	a9, a8, -2; add	a6, a6, a4 }
    3cc0:	310727c418a5030e                  	{ slli	a3, a3, 16; sll	a7, a7 }
    3cc8:	3306994114e8565e                  	{ sub	a6, a6, a5; or	a3, a9, a3 }
    3cd0:	000886                                    	j	3cf6 <__udivdi3+0x28a>

00003cd3 <__udivdi3+0x267>:
	...

00003cd4 <__udivdi3+0x268>:
    3cd4:	0e04014104e0574e                  	{ saltu	a4, a7, a4; movi	a15, 1 }
    3cdc:	0e0694c014e9556e                  	{ saltu	a2, a5, a6; xor	a4, a4, a15 }
    3ce4:	202420                                    	or	a2, a4, a2
    3ce7:	f01d                                    	retw.n

00003ce9 <__udivdi3+0x27d>:
    3ce9:	000000                                        ...

00003cec <__udivdi3+0x280>:
    3cec:	250210c114fc464e                  	{ sub	a6, a6, a4; extui	a2, a4, 16, 16 }
    3cf4:	130c                                    	movi.n	a3, 1
    3cf6:	c28620                                    	quou	a8, a6, a2
    3cf9:	82a820                                    	mull	a10, a8, a2
    3cfc:	945d                                    	ae_zext16	a5, a4
    3cfe:	370210c314ff56ae                  	{ sub	a14, a6, a10; extui	a15, a7, 16, 16 }
    3d06:	116e00                                    	slli	a6, a14, 16
    3d09:	829850                                    	mull	a9, a8, a5
    3d0c:	2806984214e926fe                  	{ add	a10, a6, a15; or	a6, a8, a8 }
    3d14:	15ba97                                    	bgeu	a10, a9, 3d2d <__udivdi3+0x2c1>
    3d17:	360288c214ff2a4e                  	{ add	a10, a10, a4; addi	a6, a8, -1 }
    3d1f:	0a3a47                                    	bltu	a10, a4, 3d2d <__udivdi3+0x2c1>
    3d22:	07ba97                                    	bgeu	a10, a9, 3d2d <__udivdi3+0x2c1>
    3d25:	2406ea471472e86e                  	{ addi	a6, a8, -2; add	a10, a10, a4 }
    3d2d:	c08a90                                    	sub	a8, a10, a9
    3d30:	97dd                                    	ae_zext16	a13, a7
    3d32:	c27820                                    	quou	a7, a8, a2
    3d35:	82e720                                    	mull	a14, a7, a2
    3d38:	822750                                    	mull	a2, a7, a5
    3d3b:	39040fc31cff48ee                  	{ sub	a15, a8, a14; movi	a8, -1 }
    3d43:	115f00                                    	slli	a5, a15, 16
    3d46:	55da                                    	add.n	a5, a5, a13
    3d48:	140685010811a52e                  	{ bgeu.w15	a5, a2, 3d60 <__udivdi3+0x2f4>; add	a5, a5, a4 }
    3d50:	0406954014f1452e                  	{ saltu	a2, a5, a2; saltu	a4, a5, a4 }
    3d58:	c02820                                    	sub	a2, a8, a2
    3d5b:	932840                                    	movnez	a2, a8, a4
    3d5e:	727a                                    	add.n	a7, a2, a7
    3d60:	112600                                    	slli	a2, a6, 16
    3d63:	202720                                    	or	a2, a7, a2
    3d66:	f01d                                    	retw.n

00003d68 <_fini>:
    3d68:	008136                                    	entry	a1, 64
    3d6b:	f10b81                                    	l32r	a8, 198 (1b4 <__do_global_dtors_aux>)
    3d6e:	f03d                                    	nop.n
    3d70:	0008e0                                    	callx8	a8

00003d73 <_fini+0xb>:
    3d73:	f01d                                    	retw.n
