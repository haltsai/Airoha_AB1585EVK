
Build/lib/pisplit_skew_ctrl_position_independent.splitload.lib:     file format elf32-xtensa-le


Disassembly of section .text:

00000000 <_init-0xb0>:
       0:	0000010c 00001ac0 00002200 000021e4     ........."...!..
      10:	00000000 000021cc 00000000 00002204     .....!......."..
      20:	0000221c 00002110 00000170 0000212c     ."...!..p...,!..
      30:	00000178 00002144 000001e0 00002168     x...D!......h!..
      40:	00001278 000021f0 21081301 00001ae0     x....!.....!....
      50:	00000654 3fffffff 00001974 40000000     T......?t......@
      60:	55555555 002aaaaa 00400000 0ccd0000     UUUU..*...@.....
      70:	a0000000 00000caa 000010d8 000016b0     ................
      80:	00002180 0000110c 00001224 000006fc     .!......$.......
      90:	0000a000 2aaaaaaa 00004000 00000ccd     .......*.@......
      a0:	00002000 00001fd4 000021d0 000000d0     . .......!......

000000b0 <_init>:
      b0:	008136                                    	entry	a1, 64
      b3:	ffd381                                    	l32r	a8, 0 (10c <frame_dummy>)
      b6:	f03d                                    	nop.n
      b8:	0008e0                                    	callx8	a8
      bb:	ffd281                                    	l32r	a8, 4 (1ac0 <__do_global_ctors_aux>)
      be:	f03d                                    	nop.n
      c0:	0008e0                                    	callx8	a8
      c3:	f01d                                    	retw.n

000000c5 <_init+0x15>:
	...

000000d0 <__do_global_dtors_aux>:
      d0:	004136                                    	entry	a1, 32
      d3:	ffcd31                                    	l32r	a3, 8 (2200 <__bss_start>)
      d6:	000382                                    	l8ui	a8, a3, 0
      d9:	d8ec                                    	bnez.n	a8, 10a <__do_global_dtors_aux+0x3a>
      db:	ffcc21                                    	l32r	a2, c (21e4 <p$7655_3_1>)
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
      fa:	ffc6a1                                    	l32r	a10, 14 (21cc <__EH_FRAME_BEGIN___100>)
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
     117:	ffbfa1                                    	l32r	a10, 14 (21cc <__EH_FRAME_BEGIN___100>)
     11a:	ffc0b1                                    	l32r	a11, 1c (2204 <object$7674_5_1>)
     11d:	ffbe81                                    	l32r	a8, 18 (0 <_text_start>)
     120:	0008e0                                    	callx8	a8

00000123 <frame_dummy+0x17>:
     123:	f01d                                    	retw.n

00000125 <frame_dummy+0x19>:
	...

00000130 <_start>:
     130:	004136                                    	entry	a1, 32
     133:	ffbca1                                    	l32r	a10, 24 (2110 <_DYNAMIC+0x33c>)
     136:	ffba31                                    	l32r	a3, 20 (221c <printf_ptr>)
     139:	0288                                    	l32i.n	a8, a2, 0
     13b:	ffbbb1                                    	l32r	a11, 28 (170 <get_skewctrl_version>)
     13e:	0389                                    	s32i.n	a8, a3, 0
     140:	0008e0                                    	callx8	a8
     143:	0388                                    	l32i.n	a8, a3, 0
     145:	ffb9a1                                    	l32r	a10, 2c (212c <_DYNAMIC+0x358>)
     148:	ffbab1                                    	l32r	a11, 30 (178 <skew_ctrl_init>)
     14b:	0008e0                                    	callx8	a8
     14e:	0388                                    	l32i.n	a8, a3, 0
     150:	ffb9a1                                    	l32r	a10, 34 (2144 <_DYNAMIC+0x370>)
     153:	ffb9b1                                    	l32r	a11, 38 (1e0 <skew_ctrl_set_input_framesize>)
     156:	0008e0                                    	callx8	a8
     159:	0388                                    	l32i.n	a8, a3, 0
     15b:	ffb8a1                                    	l32r	a10, 3c (2168 <_DYNAMIC+0x394>)
     15e:	ffb8b1                                    	l32r	a11, 40 (1278 <skew_ctrl_process>)
     161:	0008e0                                    	callx8	a8
     164:	ffb821                                    	l32r	a2, 44 (21f0 <export_parameter_array>)
     167:	f01d                                    	retw.n

00000169 <_start+0x39>:
     169:	00000000 36000000                                .......

00000170 <get_skewctrl_version>:
     170:	004136                                    	entry	a1, 32
     173:	ffb521                                    	l32r	a2, 48 (21081301 <_end+0x2107f0e1>)
     176:	f01d                                    	retw.n

00000178 <skew_ctrl_init>:
     178:	004136                                    	entry	a1, 32
     17b:	055252                                    	s16i	a5, a2, 10
     17e:	075242                                    	s16i	a4, a2, 14
     181:	080421400080123e                  	{ s16i	a3, a2, 0; movi	a9, 1 }
     189:	065292                                    	s16i	a9, a2, 12
     18c:	060420401080029e                  	{ s16i	a9, a2, 4; movi	a6, 0 }
     194:	045262                                    	s16i	a6, a2, 8
     197:	035262                                    	s16i	a6, a2, 6
     19a:	02042a420880526e                  	{ s16i	a6, a2, 146; movi	a3, 10 }
     1a2:	0206b2421089426e                  	{ s16i	a6, a2, 148; or	a4, a2, a2 }
     1aa:	2606b6420088426e                  	{ s16i	a6, a2, 144; or	a2, a6, a6 }
     1b2:	0020f0                                    	nop
     1b5:	0020f0                                    	nop
     1b8:	1306d24000a9230e                  	{ loop	a3, 1dc <skew_ctrl_init+0x64>; or	a5, a2, a2 }
     1c0:	1302824014e0324e                  	{ addx4	a2, a2, a4; addi	a3, a2, 1 }
     1c8:	933d                                    	ae_zext16	a3, a3
     1ca:	2506a3440098025e                  	{ s32i	a5, a2, 64; addx4	a2, a3, a4 }
     1d2:	1302a3440080025e                  	{ s32i	a5, a2, 64; addi	a2, a3, 1 }
     1da:	922d                                    	ae_zext16	a2, a2

000001dc <skew_ctrl_init+0x64>:
     1dc:	f01d                                    	retw.n

000001de <skew_ctrl_init+0x66>:
	...

000001e0 <skew_ctrl_set_input_framesize>:
     1e0:	014136                                    	entry	a1, 160
     1e3:	18035240008000ce                  	{ movi	a12, 0; l16ui	a8, a2, 2 }
     1eb:	0a044040008000ee                  	{ movi	a14, 0; movi	a10, 0 }
     1f3:	060440400080004e                  	{ movi	a4, 0; movi	a6, 0 }
     1fb:	050440040840b83e                  	{ beq.w15	a8, a3, 496 <skew_ctrl_set_input_framesize+0x2b6>; movi	a5, 0 }
     203:	0e6152                                    	s32i	a5, a1, 56
     206:	0d6162                                    	s32i	a6, a1, 52
     209:	0f6142                                    	s32i	a4, a1, 60
     20c:	080420400880023e                  	{ s16i	a3, a2, 2; movi	a8, 0 }
     214:	09042041008011ae                  	{ s32i	a10, a1, 16; movi	a9, 0 }
     21c:	07042040108011ce                  	{ s32i	a12, a1, 8; movi	a7, 0 }
     224:	0f042042108011ee                  	{ s32i	a14, a1, 40; movi	a15, 0 }
     22c:	0f042044008001fe                  	{ s32i	a15, a1, 64; movi	a14, 0 }
     234:	0d042044088001ee                  	{ s32i	a14, a1, 68; movi	a12, 0 }
     23c:	0b042044108011ce                  	{ s32i	a12, a1, 72; movi	a11, 0 }
     244:	0b042040188001be                  	{ s32i	a11, a1, 12; movi	a10, 0 }
     24c:	0b0332430080117e                  	{ s32i	a7, a1, 48; l16ui	a11, a2, 0 }
     254:	270332410880119e                  	{ s32i	a9, a1, 20; l16ui	a7, a2, 4 }
     25c:	1306b2c21896118e                  	{ s32i	a8, a1, 44; srli	a9, a3, 2 }
     264:	0b02abc204b8513e                  	{ srli	a8, a3, 1; addi	a11, a11, -32 }
     26c:	0b06a9c0089a11ae                  	{ s32i	a10, a1, 4; moveqz	a8, a9, a11 }
     274:	827780                                    	mull	a7, a7, a8
     277:	0462a2                                    	s32i	a10, a2, 16
     27a:	370687411884129e                  	{ l16ui	a9, a2, 14; slli	a3, a7, 9 }
     282:	2d06a74500a5178e                  	{ slli	a4, a7, 8; slli	a6, a7, 3 }
     28a:	1a06e7400c6507be                  	{ addi	a11, a7, 1; slli	a5, a7, 6 }
     292:	3b02a7c0009f01be                  	{ s32i	a11, a1, 0; addi	a10, a7, -1 }
     29a:	3d02a5c2089f01ae                  	{ s32i	a10, a1, 36; addi	a12, a5, -1 }
     2a2:	1f02a545108001ce                  	{ s32i	a12, a1, 88; addi	a14, a5, 1 }
     2aa:	1902a645088001ee                  	{ s32i	a14, a1, 84; addi	a8, a6, 1 }
     2b2:	3f02a6c4189f118e                  	{ s32i	a8, a1, 76; addi	a15, a6, -1 }
     2ba:	1902a445008001fe                  	{ s32i	a15, a1, 80; addi	a8, a4, 1 }
     2c2:	3f02a4c5189f118e                  	{ s32i	a8, a1, 92; addi	a15, a4, -1 }
     2ca:	1f02a346008001fe                  	{ s32i	a15, a1, 96; addi	a14, a3, 1 }
     2d2:	3d02a3c6089f01ee                  	{ s32i	a14, a1, 100; addi	a12, a3, -1 }
     2da:	1a61c2                                    	s32i	a12, a1, 104
     2dd:	0c0440221c20d90e                  	{ beqi.w15	a9, 16, 499 <skew_ctrl_set_input_framesize+0x2b9>; movi	a13, 0 }
     2e5:	0b06fb450c0bf98e                  	{ l32r	a8, 4c (1ae0 <__udivdi3>); or	a12, a11, a11 }
     2ed:	3706b2c210b5172e                  	{ slli	a10, a7, 30; srli	a7, a7, 2 }
     2f5:	3706b741108a11ae                  	{ s32i	a10, a1, 24; or	a11, a7, a7 }
     2fd:	0008e0                                    	callx8	a8
     300:	1c038142148201de                  	{ l32i	a13, a1, 40; l32i	a12, a1, 36 }
     308:	3706f7450c0af18e                  	{ l32r	a8, 4c (1ae0 <__udivdi3>); or	a11, a7, a7 }
     310:	1b06ba46188a01ae                  	{ s32i	a10, a1, 108; or	a9, a10, a10 }
     318:	2b03a1411081029e                  	{ s32i	a9, a2, 24; l32i	a10, a1, 24 }
     320:	0008e0                                    	callx8	a8
     323:	3d03a147008211ae                  	{ s32i	a10, a1, 112; l32i	a13, a1, 44 }
     32b:	3d03a142188402ae                  	{ s32i	a10, a2, 44; l32i	a12, a1, 76 }
     333:	ff4681                                    	l32r	a8, 4c (1ae0 <__udivdi3>)
     336:	2706b2c118b5062e                  	{ slli	a7, a6, 30; srli	a6, a6, 2 }
     33e:	2606b74618eaf66e                  	{ or	a11, a6, a6; or	a10, a7, a7 }
     346:	0008e0                                    	callx8	a8
     349:	0c038145048311ce                  	{ l32i	a12, a1, 80; l32i	a13, a1, 48 }
     351:	3706f6441c0aee8e                  	{ l32r	a8, 4c (1ae0 <__udivdi3>); or	a11, a6, a6 }
     359:	1b06ba47088a01ae                  	{ s32i	a10, a1, 116; or	a9, a10, a10 }
     361:	2706b741188a129e                  	{ s32i	a9, a2, 28; or	a10, a7, a7 }
     369:	0008e0                                    	callx8	a8
     36c:	1d03a147108311ae                  	{ s32i	a10, a1, 120; l32i	a13, a1, 52 }
     374:	1d03a143008502ae                  	{ s32i	a10, a2, 48; l32i	a12, a1, 84 }
     37c:	ff3481                                    	l32r	a8, 4c (1ae0 <__udivdi3>)
     37f:	1506b2c110b5152e                  	{ slli	a6, a5, 30; srli	a5, a5, 2 }
     387:	2606b64618eae55e                  	{ or	a11, a5, a5; or	a10, a6, a6 }
     38f:	0008e0                                    	callx8	a8
     392:	0e21d2                                    	l32i	a13, a1, 56
     395:	3506f544140afd8e                  	{ l32r	a8, 4c (1ae0 <__udivdi3>); or	a11, a5, a5 }
     39d:	2c03a14518e5eaae                  	{ or	a7, a10, a10; l32i	a12, a1, 88 }
     3a5:	2706b642008a027e                  	{ s32i	a7, a2, 32; or	a10, a6, a6 }
     3ad:	0008e0                                    	callx8	a8
     3b0:	f1d8                                    	l32i.n	a13, a1, 60
     3b2:	3c03a14510e5eaae                  	{ or	a6, a10, a10; l32i	a12, a1, 92 }
     3ba:	2306e4441402e48e                  	{ l32r	a8, 4c (1ae0 <__udivdi3>); slli	a10, a4, 30 }
     3c2:	0506b2c1189501ae                  	{ s32i	a10, a1, 28; srli	a4, a4, 2 }
     3ca:	3506b443088a026e                  	{ s32i	a6, a2, 52; or	a11, a4, a4 }
     3d2:	0008e0                                    	callx8	a8
     3d5:	1021d2                                    	l32i	a13, a1, 64
     3d8:	3506f4440c0aed8e                  	{ l32r	a8, 4c (1ae0 <__udivdi3>); or	a11, a4, a4 }
     3e0:	0c03a14508e6eaae                  	{ or	a5, a10, a10; l32i	a12, a1, 96 }
     3e8:	3b03a1420881025e                  	{ s32i	a5, a2, 36; l32i	a10, a1, 28 }
     3f0:	0008e0                                    	callx8	a8
     3f3:	1121d2                                    	l32i	a13, a1, 68
     3f6:	1c03a14500e6eaae                  	{ or	a4, a10, a10; l32i	a12, a1, 100 }
     3fe:	2306e3440c02e38e                  	{ l32r	a8, 4c (1ae0 <__udivdi3>); slli	a10, a3, 30 }
     406:	3306b2c2009411ae                  	{ s32i	a10, a1, 32; srli	a3, a3, 2 }
     40e:	3306b343108a124e                  	{ s32i	a4, a2, 56; or	a11, a3, a3 }
     416:	0008e0                                    	callx8	a8
     419:	1221d2                                    	l32i	a13, a1, 72
     41c:	3306f344040afc8e                  	{ l32r	a8, 4c (1ae0 <__udivdi3>); or	a11, a3, a3 }
     424:	2c03a14418e6eaae                  	{ or	a3, a10, a10; l32i	a12, a1, 104 }
     42c:	0b03a1421082023e                  	{ s32i	a3, a2, 40; l32i	a10, a1, 32 }
     434:	0008e0                                    	callx8	a8
     437:	1d21b2                                    	l32i	a11, a1, 116
     43a:	2c0381461c8711ce                  	{ l32i	a12, a1, 108; l32i	a13, a1, 120 }
     442:	0e03e1471c67e58e                  	{ addi	a8, a5, -1; l32i	a14, a1, 112 }
     44a:	3f02a3c2089f128e                  	{ s32i	a8, a2, 36; addi	a15, a3, -1 }
     452:	3902a7c2109f12fe                  	{ s32i	a15, a2, 40; addi	a9, a7, -1 }
     45a:	3902aac2009f129e                  	{ s32i	a9, a2, 32; addi	a9, a10, -1 }
     462:	3f02aec3189f029e                  	{ s32i	a9, a2, 60; addi	a14, a14, -1 }
     46a:	3d02adc2189f12ee                  	{ s32i	a14, a2, 44; addi	a13, a13, -1 }
     472:	3d02acc3009f02de                  	{ s32i	a13, a2, 48; addi	a12, a12, -1 }
     47a:	3b02abc1109f12ce                  	{ s32i	a12, a2, 24; addi	a11, a11, -1 }
     482:	3d02a6c1189f02be                  	{ s32i	a11, a2, 28; addi	a12, a6, -1 }
     48a:	3b02a4c3089f12ce                  	{ s32i	a12, a2, 52; addi	a11, a4, -1 }
     492:	e2b9                                    	s32i.n	a11, a2, 56
     494:	f01d                                    	retw.n

00000496 <skew_ctrl_set_input_framesize+0x2b6>:
     496:	f01d                                    	retw.n

00000498 <skew_ctrl_set_input_framesize+0x2b8>:
	...

00000499 <skew_ctrl_set_input_framesize+0x2b9>:
     499:	11d8                                    	l32i.n	a13, a1, 4
     49b:	feec81                                    	l32r	a8, 4c (1ae0 <__udivdi3>)
     49e:	1306aa421ca242ae                  	{ srli	a11, a10, 2; slli	a9, a10, 30 }
     4a6:	2806b94700eaf77e                  	{ or	a12, a7, a7; or	a10, a9, a9 }
     4ae:	0008e0                                    	callx8	a8
     4b1:	07cd                                    	mov.n	a12, a7
     4b3:	0b03a146188011ae                  	{ s32i	a10, a1, 108; l32i	a11, a1, 0 }
     4bb:	1b06fa43140ae48e                  	{ l32r	a8, 4c (1ae0 <__udivdi3>); or	a9, a10, a10 }
     4c3:	1d03a1411080129e                  	{ s32i	a9, a2, 24; l32i	a13, a1, 4 }

000004cb <skew_ctrl_set_input_framesize+0x2eb>:
     4cb:	3b06b2c210b61b2e                  	{ slli	a10, a11, 30; srli	a11, a11, 2 }
     4d3:	0008e0                                    	callx8	a8
     4d6:	21d8                                    	l32i.n	a13, a1, 8
     4d8:	0b03a147008511ae                  	{ s32i	a10, a1, 112; l32i	a11, a1, 80 }
     4e0:	1b06fa430c0aeb8e                  	{ l32r	a8, 4c (1ae0 <__udivdi3>); or	a9, a10, a10 }
     4e8:	0706b642188b029e                  	{ s32i	a9, a2, 44; or	a12, a6, a6 }
     4f0:	3b06b2c210b61b2e                  	{ slli	a10, a11, 30; srli	a11, a11, 2 }
     4f8:	0008e0                                    	callx8	a8
     4fb:	06cd                                    	mov.n	a12, a6
     4fd:	3b03a147088411ae                  	{ s32i	a10, a1, 116; l32i	a11, a1, 76 }
     505:	1b06fa430c0ae18e                  	{ l32r	a8, 4c (1ae0 <__udivdi3>); or	a9, a10, a10 }
     50d:	2d03a1411880129e                  	{ s32i	a9, a2, 28; l32i	a13, a1, 8 }
     515:	3b06b2c210b61b2e                  	{ slli	a10, a11, 30; srli	a11, a11, 2 }
     51d:	0008e0                                    	callx8	a8
     520:	31d8                                    	l32i.n	a13, a1, 12
     522:	2b03a147108511ae                  	{ s32i	a10, a1, 120; l32i	a11, a1, 88 }
     52a:	1b06fa43040ae88e                  	{ l32r	a8, 4c (1ae0 <__udivdi3>); or	a9, a10, a10 }
     532:	0506b543008b129e                  	{ s32i	a9, a2, 48; or	a12, a5, a5 }
     53a:	3b06b2c210b61b2e                  	{ slli	a10, a11, 30; srli	a11, a11, 2 }
     542:	0008e0                                    	callx8	a8
     545:	040695450c8b11be                  	{ l32i	a11, a1, 84; or	a12, a5, a5 }
     54d:	3b06fa421c09ef8e                  	{ l32r	a8, 4c (1ae0 <__udivdi3>); or	a7, a10, a10 }
     555:	3d03a1420080127e                  	{ s32i	a7, a2, 32; l32i	a13, a1, 12 }
     55d:	3b06b2c210b61b2e                  	{ slli	a10, a11, 30; srli	a11, a11, 2 }
     565:	0008e0                                    	callx8	a8
     568:	0c038146048111be                  	{ l32i	a11, a1, 96; l32i	a13, a1, 16 }
     570:	2b06fa421c09e78e                  	{ l32r	a8, 4c (1ae0 <__udivdi3>); or	a6, a10, a10 }
     578:	0506b443088b026e                  	{ s32i	a6, a2, 52; or	a12, a4, a4 }
     580:	3b06b2c210b61b2e                  	{ slli	a10, a11, 30; srli	a11, a11, 2 }
     588:	0008e0                                    	callx8	a8
     58b:	040694451c8b01be                  	{ l32i	a11, a1, 92; or	a12, a4, a4 }
     593:	1b06fa421409ee8e                  	{ l32r	a8, 4c (1ae0 <__udivdi3>); or	a5, a10, a10 }
     59b:	0d03a1420881125e                  	{ s32i	a5, a2, 36; l32i	a13, a1, 16 }
     5a3:	3b06b2c210b61b2e                  	{ slli	a10, a11, 30; srli	a11, a11, 2 }
     5ab:	0008e0                                    	callx8	a8
     5ae:	1c038146148111be                  	{ l32i	a11, a1, 104; l32i	a13, a1, 20 }
     5b6:	0b06fa421409e58e                  	{ l32r	a8, 4c (1ae0 <__udivdi3>); or	a4, a10, a10 }
     5be:	0306b343108b124e                  	{ s32i	a4, a2, 56; or	a12, a3, a3 }
     5c6:	3b06b2c210b61b2e                  	{ slli	a10, a11, 30; srli	a11, a11, 2 }
     5ce:	0008e0                                    	callx8	a8
     5d1:	1a03a14700e6f33e                  	{ or	a12, a3, a3; l32i	a11, a1, 100 }
     5d9:	3b06fa420c08ec8e                  	{ l32r	a8, 4c (1ae0 <__udivdi3>); or	a3, a10, a10 }
     5e1:	1d03a1421081123e                  	{ s32i	a3, a2, 40; l32i	a13, a1, 20 }
     5e9:	3b06b2c210b61b2e                  	{ slli	a10, a11, 30; srli	a11, a11, 2 }
     5f1:	0008e0                                    	callx8	a8
     5f4:	1d21d2                                    	l32i	a13, a1, 116
     5f7:	2e0381461c8711ee                  	{ l32i	a14, a1, 108; l32i	a15, a1, 120 }
     5ff:	0803e1471c67e5be                  	{ addi	a11, a5, -1; l32i	a8, a1, 112 }
     607:	3902a3c2089f12be                  	{ s32i	a11, a2, 36; addi	a9, a3, -1 }
     60f:	3d02a7c2109f029e                  	{ s32i	a9, a2, 40; addi	a12, a7, -1 }
     617:	3d02aac2009f02ce                  	{ s32i	a12, a2, 32; addi	a12, a10, -1 }
     61f:	3902a8c3189f02ce                  	{ s32i	a12, a2, 60; addi	a8, a8, -1 }
     627:	3f02afc2189f128e                  	{ s32i	a8, a2, 44; addi	a15, a15, -1 }
     62f:	3f02aec3009f02fe                  	{ s32i	a15, a2, 48; addi	a14, a14, -1 }
     637:	3d02adc1109f12ee                  	{ s32i	a14, a2, 24; addi	a13, a13, -1 }
     63f:	3f02a6c1189f02de                  	{ s32i	a13, a2, 28; addi	a14, a6, -1 }
     647:	3d02a4c3089f12ee                  	{ s32i	a14, a2, 52; addi	a13, a4, -1 }
     64f:	e2d9                                    	s32i.n	a13, a2, 56
     651:	f01d                                    	retw.n

00000653 <skew_ctrl_set_input_framesize+0x473>:
	...

00000654 <Get_Src_Len>:
     654:	004136                                    	entry	a1, 32
     657:	061272                                    	l16ui	a7, a2, 12
     65a:	0a044440008006be                  	{ movi	a11, 6; movi	a10, 4 }
     662:	090425201c00870e                  	{ beqi.w15	a7, 3, 6d8 <Get_Src_Len+0x84>; movi	a8, 5 }
     66a:	050429211000970e                  	{ beqi.w15	a7, 6, 6c0 <Get_Src_Len+0x6c>; movi	a5, 9 }
     672:	4a1757                                    	beq	a7, a5, 6c0 <Get_Src_Len+0x6c>
     675:	07042f261000270e                  	{ beqi.w15	a7, 12, 6c0 <Get_Src_Len+0x6c>; movi	a6, 15 }
     67d:	3f1767                                    	beq	a7, a6, 6c0 <Get_Src_Len+0x6c>
     680:	4e2726                                    	beqi	a7, 2, 6d2 <Get_Src_Len+0x7e>
     683:	455726                                    	beqi	a7, 5, 6cc <Get_Src_Len+0x78>
     686:	09042b220000170e                  	{ beqi.w15	a7, 8, 6cc <Get_Src_Len+0x78>; movi	a9, 11 }
     68e:	0d040e000c00c79e                  	{ beq.w15	a7, a9, 6cc <Get_Src_Len+0x78>; movi	a12, 14 }
     696:	3217c7                                    	beq	a7, a12, 6cc <Get_Src_Len+0x78>
     699:	220322250012070e                  	{ beqi.w15	a7, 4, 6de <Get_Src_Len+0x8a>; l16si	a2, a2, 148 }
     6a1:	2e02a7a5181d670e                  	{ beqi.w15	a7, 7, 6f2 <Get_Src_Len+0x9e>; addi	a14, a7, -10 }
     6a9:	06044044008800de                  	{ movi	a13, 0x100; movi	a6, 0x200 }
     6b1:	836de0                                    	moveqz	a6, a13, a14

000006b4 <Get_Src_Len+0x60>:
     6b4:	f2f260                                    	rems	a15, a2, a6
     6b7:	2b1fa6                                    	blti	a15, 1, 6e6 <Get_Src_Len+0x92>
     6ba:	638d                                    	ae_s16i.n	a8, a3, 0
     6bc:	648d                                    	ae_s16i.n	a8, a4, 0
     6be:	f01d                                    	retw.n

000006c0 <Get_Src_Len+0x6c>:
     6c0:	4a9252                                    	l16si	a5, a2, 148
     6c3:	011596                                    	bltz	a5, 6d8 <Get_Src_Len+0x84>
     6c6:	638d                                    	ae_s16i.n	a8, a3, 0
     6c8:	648d                                    	ae_s16i.n	a8, a4, 0
     6ca:	f01d                                    	retw.n

000006cc <Get_Src_Len+0x78>:
     6cc:	4a9292                                    	l16si	a9, a2, 148
     6cf:	1919a6                                    	blti	a9, 1, 6ec <Get_Src_Len+0x98>

000006d2 <Get_Src_Len+0x7e>:
     6d2:	638d                                    	ae_s16i.n	a8, a3, 0
     6d4:	648d                                    	ae_s16i.n	a8, a4, 0
     6d6:	f01d                                    	retw.n

000006d8 <Get_Src_Len+0x84>:
     6d8:	63bd                                    	ae_s16i.n	a11, a3, 0
     6da:	64ad                                    	ae_s16i.n	a10, a4, 0
     6dc:	f01d                                    	retw.n

000006de <Get_Src_Len+0x8a>:
     6de:	8c0c                                    	movi.n	a12, 8
     6e0:	f2c2c0                                    	rems	a12, a2, a12
     6e3:	d31ce6                                    	bgei	a12, 1, 6ba <Get_Src_Len+0x66>
     6e6:	63bd                                    	ae_s16i.n	a11, a3, 0
     6e8:	64ad                                    	ae_s16i.n	a10, a4, 0
     6ea:	f01d                                    	retw.n

000006ec <Get_Src_Len+0x98>:
     6ec:	63bd                                    	ae_s16i.n	a11, a3, 0
     6ee:	64ad                                    	ae_s16i.n	a10, a4, 0
     6f0:	f01d                                    	retw.n

000006f2 <Get_Src_Len+0x9e>:
     6f2:	070400471c61eebe                  	{ j	6b4 <Get_Src_Len+0x60>; movi	a6, 64 }

000006fa <Get_Src_Len+0xa6>:
	...

000006fc <skew_ctrl_32b>:
     6fc:	00a136                                    	entry	a1, 80
     6ff:	b129                                    	s32i.n	a2, a1, 44
     701:	9139                                    	s32i.n	a3, a1, 36
     703:	7159                                    	s32i.n	a5, a1, 28
     705:	a169                                    	s32i.n	a6, a1, 40
     707:	0a02c2400095059e                  	{ movi	a9, 5; addi	a10, a2, 84 }
     70f:	0306d34500aa190e                  	{ loop	a9, 727 <skew_ctrl_32b+0x2b>; or	a8, a3, a3 }
     717:	080288400481089e                  	{ l32i	a9, a8, 0; addi	a8, a8, 4 }
     71f:	0b02aa4000810a9e                  	{ s32i	a9, a10, 0; addi	a10, a10, 4 }

00000727 <skew_ctrl_32b+0x2b>:
     727:	fe4a81                                    	l32r	a8, 50 (654 <Get_Src_Len>)
     72a:	b1a8                                    	l32i.n	a10, a1, 44
     72c:	3006f140146a11ce                  	{ addi	a12, a1, 2; or	a11, a1, a1 }
     734:	0008e0                                    	callx8	a8
     737:	0b2182                                    	l32i	a8, a1, 44
     73a:	1b0381400082016e                  	{ l16ui	a6, a1, 0; l32i	a10, a1, 36 }
     742:	fe4451                                    	l32r	a5, 54 (3fffffff <_end+0x3fffdddf>)
     745:	051892                                    	l16ui	a9, a8, 10
     748:	0b028a4114e4268e                  	{ addx4	a6, a6, a8; addi	a10, a10, 16 }
     750:	19033142008001ae                  	{ s32i	a10, a1, 32; l16ui	a8, a1, 2 }
     758:	066182                                    	s32i	a8, a1, 24
     75b:	0702e6201410490e                  	{ beqi.w15	a9, 2, 849 <skew_ctrl_32b+0x14d>; addi	a6, a6, 64 }
     763:	060420240000d8ae                  	{ beqz.w15	a8, 7c0 <skew_ctrl_32b+0xc4>; movi	a7, 0 }

0000076b <skew_ctrl_32b+0x6f>:
     76b:	b1b8                                    	l32i.n	a11, a1, 44
     76d:	0020f0                                    	nop
     770:	0020f0                                    	nop
     773:	08038b410c811bce                  	{ l32i	a12, a11, 20; l32i	a9, a11, 16 }
     77b:	1d06e9401c12e78e                  	{ l32r	a8, 58 (1974 <intp_filter_32b>); add	a9, a9, a12 }
     783:	2b06fb47086a1e9e                  	{ extui	a13, a9, 30, 2; or	a10, a11, a11 }
     78b:	0d0401430ce02d6e                  	{ addx4	a13, a13, a6; movi	a12, 1 }
     793:	2706bd4608a5595e                  	{ and	a9, a9, a5; movnez	a6, a13, a7 }

0000079b <skew_ctrl_32b+0x9f>:
     79b:	3706b641008a0b9e                  	{ s32i	a9, a11, 16; or	a11, a6, a6 }
     7a3:	0008e0                                    	callx8	a8
     7a6:	1d033140008014ae                  	{ s32i	a10, a4, 0; l16ui	a13, a1, 2 }
     7ae:	1702a741108011de                  	{ s32i	a13, a1, 24; addi	a7, a7, 1 }
     7b6:	977d                                    	ae_zext16	a7, a7
     7b8:	0502c41f0fe167de                  	{ bltu.w15	a7, a13, 76b <skew_ctrl_32b+0x6f>; addi	a4, a4, 4 }

000007c0 <skew_ctrl_32b+0xc4>:
     7c0:	280381421481019e                  	{ l32i	a9, a1, 40; l32i	a8, a1, 24 }
     7c8:	8168                                    	l32i.n	a6, a1, 32
     7ca:	3806b8010809489e                  	{ bgeu.w15	a8, a9, 816 <skew_ctrl_32b+0x11a>; or	a7, a8, a8 }
     7d2:	b1b8                                    	l32i.n	a11, a1, 44
     7d4:	08038b410c811bce                  	{ l32i	a12, a11, 20; l32i	a9, a11, 16 }
     7dc:	fe1f81                                    	l32r	a8, 58 (1974 <intp_filter_32b>)
     7df:	2a069b420cea39ce                  	{ add	a9, a9, a12; or	a10, a11, a11 }
     7e7:	0d04614708600e9e                  	{ extui	a13, a9, 30, 2; movi	a12, 1 }
     7ef:	2706ad4608b9495e                  	{ and	a9, a9, a5; addx4	a6, a13, a6 }

000007f7 <skew_ctrl_32b+0xfb>:
     7f7:	3706b641008a0b9e                  	{ s32i	a9, a11, 16; or	a11, a6, a6 }
     7ff:	0008e0                                    	callx8	a8
     802:	a1d8                                    	l32i.n	a13, a1, 40
     804:	1702a740008014ae                  	{ s32i	a10, a4, 0; addi	a7, a7, 1 }
     80c:	977d                                    	ae_zext16	a7, a7
     80e:	0402e41b0be107de                  	{ bltu.w15	a7, a13, 7d2 <skew_ctrl_32b+0xd6>; addi	a4, a4, 4 }

00000816 <skew_ctrl_32b+0x11a>:
     816:	380381420c8111ae                  	{ l32i	a10, a1, 36; l32i	a9, a1, 28 }
     81e:	b188                                    	l32i.n	a8, a1, 44

00000820 <skew_ctrl_32b+0x124>:
     820:	1a06c940009a05ae                  	{ movi	a10, 5; addx4	a9, a9, a10 }
     828:	0902c9c500bb1a0e                  	{ loop	a10, 840 <skew_ctrl_32b+0x144>; addi	a9, a9, -20 }
     830:	0a03e9410460088e                  	{ addi	a8, a8, 4; l32i	a10, a9, 0 }
     838:	0902a943188118ae                  	{ s32i	a10, a8, 60; addi	a9, a9, 4 }

00000840 <skew_ctrl_32b+0x144>:
     840:	f01d                                    	retw.n

00000842 <skew_ctrl_32b+0x146>:
     842:	00000000 c8000000                                .......

00000849 <skew_ctrl_32b+0x14d>:
     849:	b1c8                                    	l32i.n	a12, a1, 44
     84b:	fe06a1                                    	l32r	a10, 64 (2aaaaa <_end+0x2a888a>)
     84e:	61b8                                    	l32i.n	a11, a1, 24

00000850 <skew_ctrl_32b+0x154>:
     850:	1c038c4104810c8e                  	{ l32i	a8, a12, 16; l32i	a12, a12, 20 }
     858:	03042141088001ce                  	{ s32i	a12, a1, 20; movi	a2, 1 }
     860:	33042ca0009f7bae                  	{ beqz.w15	a11, cb2 <skew_ctrl_32b+0x5b6>; movi	a3, -4 }
     868:	fe00b1                                    	l32r	a11, 68 (400000 <_end+0x3fdde0>)
     86b:	0c06e8471c12ecee                  	{ l32r	a14, 5c (40000000 <_end+0x3fffdde0>); add	a8, a8, a12 }
     873:	1d06bc4600ab485e                  	{ and	a8, a8, a5; or	a13, a12, a12 }
     87b:	1e06e8471c06f9ce                  	{ l32r	a12, 60 (55555555 <_end+0x55553335>); slli	a9, a8, 1 }
     883:	21072e40088d019e                  	{ s32i	a9, a1, 4; ae_cvt48a32	aed13, a14 }
     88b:	3004d14300ab92de                  	{ ae_slaaq56	aed18, aed13, a2; ae_l32f24.i	aed11, a1, 4 }
     893:	0b067ac21cced21e                  	{ ae_s32.l.i	aed11, a1, 8; ae_movda32x2	aed14, a10, a11 }
     89b:	0ead650b7981c3d0c58081211d32821f 	{ ae_l32.i	aed2, a1, 8; ae_cvt48a32	aed15, a12; ae_mov	aed9, aed18; nop }
     8ab:	0e5d345b0d3c0c2204bc304d084e79ef 	{ ae_mov	aed7, aed13; nop; ae_mulsf32s.lh	aed9, aed2, aed14; ae_mov	aed8, aed15 }
     8bb:	0e9d2f3b0d340c10911aab3c104eb9ef 	{ ae_mov	aed11, aed18; nop; ae_mulaf32s.ll	aed7, aed2, aed14; ae_mulaf32s.lh	aed8, aed2, aed14 }
     8cb:	0e5d145b08f40c228588315a084e109f 	{ ae_round24x2f48ssym	aed4, aed9, aed9; nop; ae_mulsf32s.ll	aed11, aed2, aed14; ae_cvtq56p32s.l	aed10, aed2 }
     8db:	0ea5043b08fc0ca0c38cf252008a118f 	{ ae_round24x2f48ssym	aed5, aed8, aed8; nop; ae_sub64	aed10, aed18, aed10; ae_round24x2f48ssym	aed3, aed7, aed7 }
     8eb:	0e9d0a5b08fc0ca0c3996df210651daf 	{ ae_round24x2f48ssym	aed29, aed10, aed10; nop; ae_mulf32s.ll	aed30, aed3, aed5; ae_round24x2f48ssym	aed6, aed11, aed11 }
     8fb:	0e1d3e5b0d4c0c247f2b6d000cdd015f 	{ nop; nop; ae_mulf32s.ll	aed0, aed6, aed29; ae_mulf32s.ll	aed31, aed4, aed29 }
     90b:	0e1d4647f1ef661474ab6d090486998f 	{ ae_movda32	aed25, a9; or	a14, a6, a6; ae_mulf32s.ll	aed1, aed4, aed6; ae_mulf32s.ll	aed26, aed2, aed5 }
     91b:	00000053008300ffd62707e73c03b39f 	{ ae_trunca32q48	a9, aed31; ae_trunca32q48	a15, aed0; ae_mulf32s.ll	aed28, aed2, aed3 }
     92b:	0eace17e0529e3a0c260213b0c016caf 	{ ae_trunca32q48	a12, aed26; ae_l32.xp	aed0, a14, a3; nop; ae_round24x2f48ssym	aed24, aed1, aed1 }
     93b:	00000313788300fbd62707df241db3af 	{ ae_trunca32q48	a10, aed28; ae_trunca32q48	a7, aed30; ae_mulf32s.ll	aed27, aed24, aed29 }
     94b:	3107474418f10fee                  	{ ae_movda32	aed19, a15; ae_movda32	aed17, a7 }
     953:	00000233690300fdf0e606cf2419f7ff 	{ l32r	a15, 70 (a0000000 <_end+0x9fffdde0>); ae_trunca32q48	a11, aed27; ae_mulf32r.hh	aed25, aed17, aed25 }
     963:	31074b4510f50aee                  	{ ae_movda32	aed22, a10; ae_movda32	aed21, a11 }
     96b:	0e9d7a2bd529c4a0c2672cb30ed5679f 	{ ae_trunca32q48	a7, aed25; ae_movda32	aed26, a12; ae_mulf32r.hh	aed22, aed22, aed21; ae_round24x2f48ssym	aed25, aed25, aed25 }
     97b:	0e1d564ba1e994d3f42b4d671f19b89f 	{ l32r	a9, 6c (ccd0000 <_end+0xcccdde0>); ae_movda32	aed20, a9; ae_mulf32s.ll	aed12, aed24, aed25; ae_mulf32r.hh	aed26, aed26, aed21 }
     98b:	0e9d662bbd2174d0c4008c8b0e356b6f 	{ ae_trunca32q48	a11, aed22; ae_movda32	aed23, a7; ae_mulf32r.hh	aed17, aed17, aed21; nop }
     99b:	000002736c8000fe5686069b2017d20f 	{ ae_slaaq56	aed16, aed12, a2; ae_trunca32q48	a12, aed26; ae_mulf32r.hh	aed19, aed19, aed23 }
     9ab:	0e25540b81f994a3e8ab4c81016c7c8f 	{ ae_movda32	aed7, a12; ae_movda32	aed16, a9; ae_add64	aed12, aed12, aed16; ae_mulf32r.hh	aed20, aed20, aed23 }
     9bb:	0ead656b5521b4d0c400813b0c016a3f 	{ ae_trunca32q48	a10, aed19; ae_movda32	aed10, a11; nop; nop }
     9cb:	0e6d616b5d29a44cc602213b0c01674f 	{ ae_trunca32q48	a7, aed20; ae_movda32	aed11, a10; nop; ae_slai64s	aed19, aed17, 1 }
     9db:	0ea5440b41e174d0c480919905891f8f 	{ ae_movda32	aed17, a15; ae_movda32	aed8, a7; ae_add64s	aed9, aed17, aed19; nop }
     9eb:	00000153208300fe762606532410b3bf 	{ ae_trunca32q48	a11, aed12; ae_trunca32q48	a12, aed9; ae_mulf32r.hh	aed10, aed10, aed16 }
     9fb:	0e1d522b31e1c45bd6ab4c4101119b8f 	{ ae_movda32	aed9, a11; ae_movda32	aed6, a12; ae_mulf32r.hh	aed8, aed8, aed17; ae_mulf32r.hh	aed11, aed11, aed16 }
     a0b:	92cd                                    	ae_zext16	a12, a2
     a0d:	000003f3288300fd762303023460b3ff 	{ ae_trunca32q48	a15, aed10; ae_trunca32q48	a10, aed11; nop }
     a1d:	0e9d662b0d20a4d0c4008c3b0cf1698f 	{ ae_trunca32q48	a9, aed8; ae_movda32	aed1, a10; ae_mulf32r.hh	aed7, aed7, aed17; nop }
     a2d:	0e9cc63e59e1e3d0c4808c010001c98f 	{ ae_movda32	aed12, a9; ae_l32.xp	aed11, a14, a3; ae_mulf32r.hh	aed0, aed0, aed1; nop }
     a3d:	0e9ce69e4520e3d0c4008b030d6c677f 	{ ae_trunca32q48	a7, aed7; ae_l32.xp	aed8, a14, a3; ae_mulaf32r.hh	aed0, aed11, aed12; nop }
     a4d:	0e9ce68831e419d0c5808b011d0947ef 	{ ae_l32.xp	aed5, a14, a3; l32i	a9, a1, 24; ae_mulaf32r.hh	aed0, aed8, aed9; nop }
     a5d:	0e9d668b21e074d0c5808b011ca6c3ef 	{ ae_l32.xp	aed3, a14, a3; ae_movda32	aed4, a7; ae_mulaf32r.hh	aed0, aed5, aed6; nop }
     a6d:	0e9d668b11e0f4d0c5808b011c6443ef 	{ ae_l32.xp	aed1, a14, a3; ae_movda32	aed2, a15; ae_mulaf32r.hh	aed0, aed3, aed4; nop }
     a7d:	1e81a6053c22f99e                  	{ addi	a9, a9, -1; nop; ae_mulaf32r.hh	aed0, aed1, aed2 }
     a85:	0c06bd4710e9f44e                  	{ or	a14, a4, a4; or	a4, a13, a13 }
     a8d:	0020f0                                    	nop
     a90:	f03d                                    	nop.n
     a92:	000003f3058200fd5603030338605b5f 	{ nop; ae_trunca32q48	a10, aed0; nop }
     aa2:	204916                                    	beqz	a9, caa <skew_ctrl_32b+0x5ae>
     aa5:	179976                                    	loopnez	a9, ac0 <skew_ctrl_32b+0x3c4>
     aa8:	030190                                    	rsr.lend	a9
     aab:	130090                                    	wsr.lbeg	a9
     aae:	fd7191                                    	l32r	a9, 74 (caa <skew_ctrl_32b+0x5ae>)
     ab1:	0020f0                                    	nop
     ab4:	130190                                    	wsr.lend	a9
     ab7:	002000                                    	isync
     aba:	030290                                    	rsr.lcount	a9
     abd:	01c992                                    	addi	a9, a9, 1
     ac0:	0e6d75064d3a842084c82125082f09ef 	{ ae_mov	aed0, aed13; add	a9, a8, a4; ae_mov	aed1, aed15; ae_mov	aed2, aed18 }
     ad0:	0506a9c000861eae                  	{ s32i	a10, a14, 0; and	a8, a9, a5 }
     ad8:	0f02ae4610a108fe                  	{ slli	a10, a8, 1; addi	a14, a14, 4 }
     ae0:	19022e40088e11ae                  	{ s32i	a10, a1, 4; extui	a9, a9, 30, 2 }
     ae8:	0ead65064d2596d0c50081201062051f 	{ ae_l32f24.i	aed4, a1, 4; addx4	a9, a9, a6; ae_mov	aed3, aed2; nop }
     af8:	31076a4104c4c21e                  	{ ae_s32.l.i	aed4, a1, 8; ae_movda32	aed4, a10 }
     b00:	2c06b9410cc5221e                  	{ ae_l32.i	aed5, a1, 8; movnez	a6, a9, a12 }
     b08:	0e1d6c47cd36662b462f511508ae69af 	{ ae_cvtq56p32s.l	aed6, aed5; or	a9, a6, a6; ae_mulsf32s.ll	aed2, aed5, aed14; ae_mulsf32s.lh	aed3, aed5, aed14 }
     b18:	0e9ccf1e60f19328819eab0910aef22f 	{ ae_sub64	aed6, aed18, aed6; ae_l32.xp	aed12, a9, a3; ae_mulaf32s.lh	aed1, aed5, aed14; ae_mulaf32s.ll	aed0, aed5, aed14 }
     b28:	0eacc17e18f893a0c39c61381001022f 	{ ae_round24x2f48ssym	aed2, aed2, aed2; ae_l32.xp	aed3, a9, a3; nop; ae_round24x2f48ssym	aed7, aed3, aed3 }
     b38:	0e9cc25e08e093a0c3a00d4808e2001f 	{ ae_round24x2f48ssym	aed0, aed1, aed1; ae_l32.xp	aed1, a9, a3; ae_mulf32s.ll	aed9, aed7, aed2; ae_round24x2f48ssym	aed8, aed0, aed0 }
     b48:	0e1cca5e40e0932c57ab6d511100126f 	{ ae_round24x2f48ssym	aed6, aed6, aed6; ae_l32.xp	aed8, a9, a3; ae_mulf32s.ll	aed10, aed8, aed0; ae_mulf32s.ll	aed11, aed5, aed8 }
     b58:	0e1cc64008f2ca1453ab6d3a08e6029f 	{ ae_round24x2f48ssym	aed2, aed9, aed9; addi	a10, a12, 1; ae_mulf32s.ll	aed7, aed7, aed6; ae_mulf32s.ll	aed9, aed2, aed6 }
     b68:	00000053288300fef62707323406b3cf 	{ ae_trunca32q48	a12, aed10; ae_trunca32q48	a13, aed11; ae_mulf32s.ll	aed6, aed2, aed6 }
     b78:	0e9d664b0520c4d0c4008d2b0ca06c7f 	{ ae_trunca32q48	a12, aed7; ae_movda32	aed0, a12; ae_mulf32s.ll	aed5, aed5, aed0; nop }
     b88:	0e9d662b2520d4d0c4008c3b0c046d9f 	{ ae_trunca32q48	a13, aed9; ae_movda32	aed4, a13; ae_mulf32r.hh	aed7, aed0, aed4; nop }
     b98:	0ead656b3520d4d0c400813b0c016d6f 	{ ae_trunca32q48	a13, aed6; ae_movda32	aed6, a13; nop; nop }
     ba8:	0ead656b3d38d4a0c224e13b0c016d7f 	{ ae_trunca32q48	a13, aed7; ae_movda32	aed7, a13; nop; ae_round24x2f48ssym	aed9, aed7, aed7 }
     bb8:	0e1d6a2b1528d4144a2b6c230c876d5f 	{ ae_trunca32q48	a13, aed5; ae_movda32	aed2, a13; ae_mulf32r.hh	aed4, aed4, aed7; ae_mulf32s.ll	aed5, aed2, aed9 }
     bc8:	0e1d422b31f1c433d4ab4c4900070d8f 	{ ae_movda32	aed0, a13; ae_movda32	aed6, a12; ae_mulf32r.hh	aed9, aed0, aed7; ae_mulf32r.hh	aed10, aed6, aed2 }
     bd8:	0e9ce63e152093d0c4008c230cc26c4f 	{ ae_trunca32q48	a12, aed4; ae_l32.xp	aed2, a9, a3; ae_mulf32r.hh	aed4, aed6, aed2; nop }
     be8:	000000132c0000fed68606312807d20f 	{ ae_slaaq56	aed0, aed5, a2; ae_trunca32q48	a13, aed10; ae_mulf32r.hh	aed6, aed0, aed7 }
     bf8:	0e65600b0528d449460125030d646d4f 	{ ae_trunca32q48	a13, aed4; ae_movda32	aed0, a13; ae_add64	aed4, aed5, aed0; ae_slai64s	aed5, aed9, 1 }
     c08:	0e25700b0520d403cc2b492b0d856d6f 	{ ae_trunca32q48	a13, aed6; ae_movda32	aed0, a13; ae_add64s	aed5, aed9, aed5; ae_mulf32r.hh	aed6, aed0, aed16 }
     c18:	0e9d462b39e1c4d0c4808c4900110d8f 	{ ae_movda32	aed0, a13; ae_movda32	aed7, a12; ae_mulf32r.hh	aed9, aed0, aed17; nop }
     c28:	00000013188300fed62606012411b3cf 	{ ae_trunca32q48	a12, aed4; ae_trunca32q48	a13, aed6; ae_mulf32r.hh	aed0, aed0, aed17 }
     c38:	0e9d662b2520d4d0c4008c330cf06d9f 	{ ae_trunca32q48	a13, aed9; ae_movda32	aed4, a13; ae_mulf32r.hh	aed6, aed7, aed16; nop }
     c48:	0e9d662b2520d4d0c4008c2b0d846d5f 	{ ae_trunca32q48	a13, aed5; ae_movda32	aed4, a13; ae_mulf32r.hh	aed5, aed12, aed4; nop }
     c58:	0e9d468b19e1c4d0c4808b2900644d8f 	{ ae_movda32	aed4, a13; ae_movda32	aed3, a12; ae_mulaf32r.hh	aed5, aed3, aed4; nop }
     c68:	00000033008300fed63301293443b3cf 	{ ae_trunca32q48	a12, aed6; ae_trunca32q48	a13, aed0; ae_mulaf32r.hh	aed5, aed1, aed3 }
     c78:	0e9d468b09e1c4d0c4808b2901040d8f 	{ ae_movda32	aed0, a13; ae_movda32	aed1, a12; ae_mulaf32r.hh	aed5, aed8, aed4; nop }
     c88:	1e82a6282040039e                  	{ ae_l32.xp	aed0, a9, a3; nop; ae_mulaf32r.hh	aed5, aed2, aed0 }
     c90:	1e83462b3401b13e                  	{ nop; nop; ae_mulaf32r.hh	aed5, aed0, aed1 }
     c98:	9acd                                    	ae_zext16	a12, a10
     c9a:	0ead257b0d240cd0c400813b0c016a5f 	{ ae_trunca32q48	a10, aed5; nop; nop; nop }

00000caa <skew_ctrl_32b+0x5ae>:
     caa:	0502ae4000810eae                  	{ s32i	a10, a14, 0; addi	a4, a14, 4 }

00000cb2 <skew_ctrl_32b+0x5b6>:
     cb2:	28038142148111ae                  	{ l32i	a10, a1, 40; l32i	a9, a1, 24 }
     cba:	fcebf1                                    	l32r	a15, 68 (400000 <_end+0x3fdde0>)
     cbd:	1c0381420481016e                  	{ l32i	a6, a1, 32; l32i	a12, a1, 20 }
     cc5:	380699050c8999ae                  	{ bgeu.w15	a9, a10, 10fa <skew_ctrl_32b+0x9fe>; or	a7, a9, a9 }
     ccd:	fce5e1                                    	l32r	a14, 64 (2aaaaa <_end+0x2a888a>)
     cd0:	fce4d1                                    	l32r	a13, 60 (55555555 <_end+0x55553335>)
     cd3:	fce2a1                                    	l32r	a10, 5c (40000000 <_end+0x3fffdde0>)
     cd6:	20070a421ced28ce                  	{ add	a11, a8, a12; ae_cvt48a32	aed13, a10 }
     cde:	20070d42148f01ae                  	{ l32i	a10, a1, 40; ae_cvt48a32	aed15, a13 }
     ce6:	0e067ec3140ef1de                  	{ l32r	a13, 6c (ccd0000 <_end+0xcccdde0>); ae_movda32x2	aed14, a14, a15 }
     cee:	0406ebc31406f0ee                  	{ l32r	a14, 70 (a0000000 <_end+0x9fffdde0>); and	a8, a11, a5 }
     cf6:	1e06c84300a692de                  	{ ae_slaaq56	aed18, aed13, a2; slli	a9, a8, 1 }
     cfe:	31072e401891019e                  	{ s32i	a9, a1, 12; ae_movda32	aed17, a14 }
     d06:	3b022e4010ce131e                  	{ ae_l32f24.i	aed2, a1, 12; extui	a11, a11, 30, 2 }
     d0e:	00008144d645b600098313f9a3e0c21f 	{ ae_s32.l.i	aed2, a1, 16; addx4	a11, a11, a6; ae_mov	aed31, aed15 }
     d1e:	0e6d7507f99fbb2045c821231fcd981f 	{ ae_l32.i	aed26, a1, 16; or	a15, a11, a11; ae_mov	aed30, aed13; ae_mov	aed1, aed18 }
     d2e:	0e5cf45e6538f32005c8310c174ed9af 	{ ae_cvtq56p32s.l	aed29, aed26; ae_l32.xp	aed12, a15, a3; ae_mulsf32s.ll	aed1, aed26, aed14; ae_mov	aed0, aed18 }
     d3e:	0e9ccf3ea8f1f3d0bf9aabf70b4efd2f 	{ ae_sub64	aed29, aed18, aed29; ae_l32.xp	aed21, a15, a3; ae_mulaf32s.ll	aed30, aed26, aed14; ae_mulaf32s.lh	aed31, aed26, aed14 }
     d4e:	0e9cc05eb8e9f3a0c3cc30070b4e13df 	{ ae_round24x2f48ssym	aed7, aed29, aed29; ae_l32.xp	aed23, a15, a3; ae_mulsf32s.lh	aed0, aed26, aed14; ae_round24x2f48ssym	aed19, aed1, aed1 }
     d5e:	0eacdd7ea0f1f3a0c3efc13f18011cff 	{ ae_round24x2f48ssym	aed28, aed31, aed31; ae_l32.xp	aed20, a15, a3; nop; ae_round24x2f48ssym	aed27, aed30, aed30 }
     d6e:	0e1cda5e10f8f3d457ab6dc0037c0d0f 	{ ae_round24x2f48ssym	aed25, aed0, aed0; ae_l32.xp	aed2, a15, a3; ae_mulf32s.ll	aed24, aed27, aed28; ae_mulf32s.ll	aed11, aed26, aed27 }
     d7e:	0e1d664b31f894cc55ab6db11f33c3ff 	{ ae_l32.xp	aed3, a15, a3; ae_movda32	aed6, a9; ae_mulf32s.ll	aed22, aed25, aed19; ae_mulf32s.ll	aed10, aed25, aed7 }
     d8e:	00000353608300ff562707423c1cb3ff 	{ ae_trunca32q48	a15, aed11; ae_trunca32q48	a14, aed24; ae_mulf32s.ll	aed8, aed26, aed28 }
     d9e:	0e9d764be530e4a0c212cd4b0e6769af 	{ ae_trunca32q48	a9, aed10; ae_movda32	aed28, a14; ae_mulf32s.ll	aed9, aed19, aed7; ae_round24x2f48ssym	aed4, aed22, aed22 }

00000dae <skew_ctrl_32b+0x6b2>:
     dae:	0e1d664b8530d4e3cc2b4d0b0c876d8f 	{ ae_trunca32q48	a13, aed8; ae_movda32	aed16, a13; ae_mulf32s.ll	aed1, aed4, aed7; ae_mulf32r.hh	aed6, aed28, aed6 }
     dbe:	0ead656bcd20d4d0c400813b0c016e9f 	{ ae_trunca32q48	a14, aed9; ae_movda32	aed25, a13; nop; nop }
     dce:	0ead656b0530e4a0c214c13b0c016e1f 	{ ae_trunca32q48	a14, aed1; ae_movda32	aed0, a14; nop; ae_round24x2f48ssym	aed5, aed6, aed6 }
     dde:	0e9d664bed20e4d0c4008dd30c856d6f 	{ ae_trunca32q48	a13, aed6; ae_movda32	aed29, a14; ae_mulf32s.ll	aed26, aed4, aed5; nop }
     dee:	0e1d5e2bf9e9d4e3f8ab4cc9073de98f 	{ ae_movda32	aed30, a9; ae_movda32	aed31, a13; ae_mulf32r.hh	aed25, aed25, aed29; ae_mulf32r.hh	aed28, aed28, aed29 }
     dfe:	0e1d7e2bf9d9f4f3fdab4c001c1fe2af 	{ ae_slaaq56	aed27, aed26, a2; ae_movda32	aed31, a15; ae_mulf32r.hh	aed0, aed0, aed31; ae_mulf32r.hh	aed30, aed30, aed31 }
     e0e:	0e5d6228d52ba74f46038cfb0ffd6e9f 	{ ae_trunca32q48	a14, aed25; sub	a10, a10, a7; ae_mulf32r.hh	aed31, aed31, aed29; ae_slai64s	aed29, aed28, 1 }
     e1e:	0ea5640bb520e4d0c4009ceb0d996d0f 	{ ae_trunca32q48	a13, aed0; ae_movda32	aed22, a14; ae_add64s	aed25, aed28, aed29; nop }
     e2e:	000003f3608300ff762303073460b39f 	{ ae_trunca32q48	a9, aed30; ae_trunca32q48	a14, aed25; nop }
     e3e:	0ea5440bf1e1d4d0c4809ad9057a898f 	{ ae_movda32	aed24, a9; ae_movda32	aed30, a13; ae_add64	aed26, aed26, aed27; nop }
     e4e:	0e1cf227fd2baac3f02b4cf30fd06fff 	{ ae_trunca32q48	a15, aed31; addi	a10, a10, -1; ae_mulf32r.hh	aed30, aed30, aed16; ae_mulf32r.hh	aed24, aed24, aed17 }
     e5e:	0e9d662b9d21f4d0c4008cb30ed16faf 	{ ae_trunca32q48	a15, aed26; ae_movda32	aed19, a15; ae_mulf32r.hh	aed22, aed22, aed17; nop }
     e6e:	00000273788300fed626069e2410b39f 	{ ae_trunca32q48	a9, aed24; ae_trunca32q48	a13, aed30; ae_mulf32r.hh	aed19, aed19, aed16 }
     e7e:	0ead656bc521d4d0c400813b0c016d6f 	{ ae_trunca32q48	a13, aed22; ae_movda32	aed24, a13; nop; nop }
     e8e:	0e9d662bc52194d0c4008c630d98693f 	{ ae_trunca32q48	a9, aed19; ae_movda32	aed24, a9; ae_mulf32r.hh	aed12, aed12, aed24; nop }
     e9e:	0e9d468ba9e1e4d0c4808b6102b84d8f 	{ ae_movda32	aed4, a13; ae_movda32	aed21, a14; ae_mulaf32r.hh	aed12, aed21, aed24; nop }
     eae:	2506d54600eb1fee                  	{ ae_movda32	aed24, a15; or	a14, a5, a5 }
     eb6:	0e9d6687952733d0c5008b6712f8f22f 	{ or	a15, a2, a2; or	a2, a3, a3; ae_mulaf32r.hh	aed12, aed23, aed24; nop }
     ec6:	230720c318e5747e                  	{ nop; nop }
     ece:	f03d                                    	nop.n
     ed0:	f03d                                    	nop.n
     ed2:	0e9d66879d46ccd0c5008b600e95015f 	{ nop; or	a3, a12, a12; ae_mulaf32r.hh	aed12, aed20, aed21; nop }
     ee2:	1f2a16                                    	beqz	a10, 10d8 <skew_ctrl_32b+0x9dc>
     ee5:	179a76                                    	loopnez	a10, f00 <skew_ctrl_32b+0x804>
     ee8:	0301a0                                    	rsr.lend	a10
     eeb:	1300a0                                    	wsr.lbeg	a10
     eee:	fc62a1                                    	l32r	a10, 78 (10d8 <skew_ctrl_32b+0x9dc>)
     ef1:	0020f0                                    	nop
     ef4:	1301a0                                    	wsr.lend	a10
     ef7:	002000                                    	isync
     efa:	0302a0                                    	rsr.lcount	a10
     efd:	01caa2                                    	addi	a10, a10, 1
     f00:	0ead650b0d219410991a8127080d983f 	{ add	a9, a8, a3; ae_movda32	aed1, a9; ae_mov	aed0, aed13; ae_mulaf32r.hh	aed12, aed2, aed4 }
     f10:	0ead6106452e9e18989a8125185219ef 	{ ae_mov	aed1, aed15; and	a8, a9, a14; ae_mov	aed2, aed18; ae_mulaf32r.hh	aed12, aed3, aed1 }
     f20:	00008121d440e9000a830329a060a3ff 	{ slli	a9, a8, 1; extui	a10, a9, 30, 2; ae_mov	aed3, aed2 }
     f30:	000003f3338300fe56a303003c60119f 	{ s32i	a9, a1, 12; ae_trunca32q48	a12, aed12; nop }
     f40:	3304b140008414ce                  	{ s32i	a12, a4, 0; ae_l32f24.i	aed4, a1, 12 }
     f48:	3b06ea4104dad41e                  	{ ae_s32.l.i	aed4, a1, 16; addx4	a11, a10, a11 }
     f50:	3007294104ca241e                  	{ ae_l32.i	aed4, a1, 16; ae_movda32	aed10, a9 }
     f58:	0e9d6f07cd36bb20801eab0d008e59af 	{ ae_cvtq56p32s.l	aed5, aed4; or	a9, a11, a11; ae_mulaf32s.lh	aed1, aed4, aed14; ae_mulaf32s.ll	aed0, aed4, aed14 }
     f68:	0e1ccc5e60f1922347af5111088ef12f 	{ ae_sub64	aed5, aed18, aed5; ae_l32.xp	aed12, a9, a2; ae_mulsf32s.ll	aed2, aed4, aed14; ae_mulsf32s.lh	aed3, aed4, aed14 }
     f78:	0eacc17e00e092a0c39c01380801011f 	{ ae_round24x2f48ssym	aed1, aed1, aed1; ae_l32.xp	aed0, a9, a2; nop; ae_round24x2f48ssym	aed7, aed0, aed0 }
     f88:	0e9cc65e28e892a0c398ad4010e1022f 	{ ae_round24x2f48ssym	aed2, aed2, aed2; ae_l32.xp	aed5, a9, a2; ae_mulf32s.ll	aed8, aed7, aed1; ae_round24x2f48ssym	aed6, aed5, aed5 }
     f98:	0e1cc65e38f8922457ab6d481846033f 	{ ae_round24x2f48ssym	aed3, aed3, aed3; ae_l32.xp	aed7, a9, a2; ae_mulf32s.ll	aed9, aed2, aed6; ae_mulf32s.ll	aed11, aed4, aed7 }
     fa8:	0e1ce25e1530921c502b6d1b0c666a8f 	{ ae_trunca32q48	a10, aed8; ae_l32.xp	aed2, a9, a2; ae_mulf32s.ll	aed3, aed3, aed6; ae_mulf32s.ll	aed8, aed3, aed2 }
     fb8:	0e9d664b0d20a4d0c4008d230c816a9f 	{ ae_trunca32q48	a10, aed9; ae_movda32	aed1, a10; ae_mulf32s.ll	aed4, aed4, aed1; nop }
     fc8:	0e9d6a2b1d20a4a0c2210c530c2a6a3f 	{ ae_trunca32q48	a10, aed3; ae_movda32	aed3, a10; ae_mulf32r.hh	aed10, aed1, aed10; ae_round24x2f48ssym	aed8, aed8, aed8 }
     fd8:	0e9d664b3520a4d0c4008d4b0d066abf 	{ ae_trunca32q48	a10, aed11; ae_movda32	aed6, a10; ae_mulf32s.ll	aed9, aed8, aed6; nop }
     fe8:	0ead696b5530a4a0c22d413b0c016aaf 	{ ae_trunca32q48	a10, aed10; ae_movda32	aed10, a10; nop; ae_round24x2f48ssym	aed11, aed10, aed10 }
     ff8:	0e9d664b4520a4d0c4008d4b0d0b6a9f 	{ ae_trunca32q48	a10, aed9; ae_movda32	aed8, a10; ae_mulf32s.ll	aed9, aed8, aed11; nop }
    1008:	0e1d6a2b1d20a433d02b4c230c686a4f 	{ ae_trunca32q48	a10, aed4; ae_movda32	aed3, a10; ae_mulf32r.hh	aed4, aed3, aed8; ae_mulf32r.hh	aed8, aed6, aed8 }
    1018:	0e9d662b3140a4d0c5808c581d43af9f 	{ ae_slaaq56	aed10, aed9, a15; ae_movda32	aed6, a10; ae_mulf32r.hh	aed11, aed10, aed3; nop }
    1028:	00000033208300fe562606092403b3af 	{ ae_trunca32q48	a10, aed4; ae_trunca32q48	a12, aed8; ae_mulf32r.hh	aed1, aed1, aed3 }
    1038:	0e9d4a2b21e9a4d8c0a14c5100c33c8f 	{ ae_movda32	aed3, a12; ae_movda32	aed4, a10; ae_mulf32r.hh	aed10, aed6, aed3; ae_add64	aed8, aed9, aed10 }
    1048:	0e1cf23e1d209223d02b4c230c716a8f 	{ ae_trunca32q48	a10, aed8; ae_l32.xp	aed3, a9, a2; ae_mulf32r.hh	aed4, aed3, aed17; ae_mulf32r.hh	aed8, aed4, aed16 }
    1058:	0e6d616b3528a44a4600213b0c0169bf 	{ ae_trunca32q48	a9, aed11; ae_movda32	aed6, a10; nop; ae_slai64s	aed9, aed1, 1 }
    1068:	000003f3108300fe562303023460b3af 	{ ae_trunca32q48	a10, aed10; ae_trunca32q48	a12, aed4; nop }
    1078:	0ea5640b0d20a4d0c400814b0d846a8f 	{ ae_trunca32q48	a10, aed8; ae_movda32	aed1, a10; ae_add64s	aed4, aed1, aed9; nop }
    1088:	0e9d662b0d20a4d0c4008c230c316a4f 	{ ae_trunca32q48	a10, aed4; ae_movda32	aed1, a10; ae_mulf32r.hh	aed4, aed1, aed17; nop }
    1098:	0e9d462b51e1c4d0c4808c610181198f 	{ ae_movda32	aed1, a9; ae_movda32	aed10, a12; ae_mulf32r.hh	aed12, aed12, aed1; nop }
    10a8:	0e9d6a2b0d30a400981a8c030c30694f 	{ ae_trunca32q48	a9, aed4; ae_movda32	aed1, a10; ae_mulf32r.hh	aed0, aed1, aed16; ae_mulaf32r.hh	aed12, aed0, aed10 }
    10b8:	0e9cc68021e344d0c4808b6100a6498f 	{ ae_movda32	aed4, a9; addi	a4, a4, 4; ae_mulaf32r.hh	aed12, aed5, aed6; nop }
    10c8:	0e9d269b0d240cd0c4008b630ce1690f 	{ ae_trunca32q48	a9, aed0; nop; ae_mulaf32r.hh	aed12, aed7, aed1; nop }

000010d8 <skew_ctrl_32b+0x9dc>:
    10d8:	1e8326612044695e                  	{ ae_movda32	aed4, a9; nop; ae_mulaf32r.hh	aed12, aed2, aed4 }

000010e0 <skew_ctrl_32b+0x9e4>:
    10e0:	1e8346633464b13e                  	{ nop; nop; ae_mulaf32r.hh	aed12, aed3, aed4 }
    10e8:	0ead257b0d240cd0c400813b0c016bcf 	{ ae_trunca32q48	a11, aed12; nop; nop; nop }
    10f8:	04b9                                    	s32i.n	a11, a4, 0

000010fa <skew_ctrl_32b+0x9fe>:
    10fa:	b1d8                                    	l32i.n	a13, a1, 44
    10fc:	1d03a14100810d8e                  	{ s32i	a8, a13, 16; l32i	a12, a1, 20 }
    1104:	5dc9                                    	s32i.n	a12, a13, 20
    1106:	fdc306                                    	j	816 <skew_ctrl_32b+0x11a>

00001109 <skew_ctrl_32b+0xa0d>:
    1109:	000000                                        ...

0000110c <skew_ctrl_16b>:
    110c:	006136                                    	entry	a1, 48
    110f:	026122                                    	s32i	a2, a1, 8
    1112:	016132                                    	s32i	a3, a1, 4
    1115:	2a02c2400092059e                  	{ movi	a9, 5; addi	a10, a2, 74 }
    111d:	0306d34500aa190e                  	{ loop	a9, 1135 <skew_ctrl_16b+0x29>; or	a8, a3, a3 }
    1125:	280288400080089e                  	{ l16si	a9, a8, 0; addi	a8, a8, 2 }
    112d:	2a02aa4000800a9e                  	{ s16i	a9, a10, 0; addi	a10, a10, 2 }

00001135 <skew_ctrl_16b+0x29>:
    1135:	fbc681                                    	l32r	a8, 50 (654 <Get_Src_Len>)
    1138:	21a8                                    	l32i.n	a10, a1, 8
    113a:	3006f140146a11ce                  	{ addi	a12, a1, 2; or	a11, a1, a1 }
    1142:	0008e0                                    	callx8	a8
    1145:	290381400080012e                  	{ l16ui	a2, a1, 0; l32i	a8, a1, 8 }
    114d:	51bd                                    	ae_l16ui.n	a11, a1, 2
    114f:	fbc131                                    	l32r	a3, 54 (3fffffff <_end+0x3fffdddf>)
    1152:	0604004414e0328e                  	{ addx2	a2, a2, a8; movi	a7, 0 }
    115a:	0202a22400104bae                  	{ beqz.w15	a11, 11a7 <skew_ctrl_16b+0x9b>; addi	a2, a2, 64 }

00001162 <skew_ctrl_16b+0x56>:
    1162:	21b8                                    	l32i.n	a11, a1, 8
    1164:	08038b410c811bce                  	{ l32i	a12, a11, 20; l32i	a9, a11, 16 }
    116c:	1d06e9470412c48e                  	{ l32r	a8, 7c (16b0 <intp_filter_16b>); add	a9, a9, a12 }
    1174:	2b06fb47086a1e9e                  	{ extui	a13, a9, 30, 2; or	a10, a11, a11 }
    117c:	0c0401470ce02d2e                  	{ addx2	a13, a13, a2; movi	a12, 1 }
    1184:	2706bd4608a4593e                  	{ and	a9, a9, a3; movnez	a2, a13, a7 }

0000118c <skew_ctrl_16b+0x80>:
    118c:	3306b241008a0b9e                  	{ s32i	a9, a11, 16; or	a11, a2, a2 }
    1194:	0008e0                                    	callx8	a8
    1197:	64ad                                    	ae_s16i.n	a10, a4, 0
    1199:	51bd                                    	ae_l16ui.n	a11, a1, 2
    119b:	771b                                    	addi.n	a7, a7, 1
    119d:	977d                                    	ae_zext16	a7, a7
    119f:	2502c41f0fe0e7be                  	{ bltu.w15	a7, a11, 1162 <skew_ctrl_16b+0x56>; addi	a4, a4, 2 }

000011a7 <skew_ctrl_16b+0x9b>:
    11a7:	1128                                    	l32i.n	a2, a1, 4
    11a9:	0b7d                                    	mov.n	a7, a11
    11ab:	0302a20108024b6e                  	{ bgeu.w15	a11, a6, 11f9 <skew_ctrl_16b+0xed>; addi	a2, a2, 8 }
    11b3:	21b8                                    	l32i.n	a11, a1, 8
    11b5:	f03d                                    	nop.n
    11b7:	08038b410c811bce                  	{ l32i	a12, a11, 20; l32i	a9, a11, 16 }
    11bf:	fbaf81                                    	l32r	a8, 7c (16b0 <intp_filter_16b>)
    11c2:	f03d                                    	nop.n
    11c4:	2a069b420cea39ce                  	{ add	a9, a9, a12; or	a10, a11, a11 }
    11cc:	0d04614708600e9e                  	{ extui	a13, a9, 30, 2; movi	a12, 1 }
    11d4:	2306ad4608b4493e                  	{ and	a9, a9, a3; addx2	a2, a13, a2 }

000011dc <skew_ctrl_16b+0xd0>:
    11dc:	3306b241008a0b9e                  	{ s32i	a9, a11, 16; or	a11, a2, a2 }
    11e4:	0008e0                                    	callx8	a8
    11e7:	1602a740008014ae                  	{ s16i	a10, a4, 0; addi	a7, a7, 1 }
    11ef:	977d                                    	ae_zext16	a7, a7
    11f1:	2502c41b0fe0e76e                  	{ bltu.w15	a7, a6, 11b3 <skew_ctrl_16b+0xa7>; addi	a4, a4, 2 }

000011f9 <skew_ctrl_16b+0xed>:
    11f9:	1198                                    	l32i.n	a9, a1, 4
    11fb:	2188                                    	l32i.n	a8, a1, 8
    11fd:	0a0405460ce0259e                  	{ addx2	a9, a5, a9; movi	a10, 5 }
    1205:	0020f0                                    	nop
    1208:	2902c9c500bd1a0e                  	{ loop	a10, 1220 <skew_ctrl_16b+0x114>; addi	a9, a9, -10 }
    1210:	0a0369401460088e                  	{ addi	a8, a8, 2; l16si	a10, a9, 0 }
    1218:	2802a947188018ae                  	{ s16i	a10, a8, 62; addi	a9, a9, 2 }

00001220 <skew_ctrl_16b+0x114>:
    1220:	f01d                                    	retw.n

00001222 <skew_ctrl_16b+0x116>:
	...

00001224 <Check_Phase_Frac>:
    1224:	004136                                    	entry	a1, 32
    1227:	236d                                    	ae_l16si.n	a6, a3, 0
    1229:	2f1226                                    	beqi	a2, 1, 125c <Check_Phase_Frac+0x38>
    122c:	040420201004020e                  	{ beqi.w15	a2, 2, 1270 <Check_Phase_Frac+0x4c>; movi	a4, 0x100 }
    1234:	020460470c68e25e                  	{ addi	a5, a2, -3; movi	a2, 0x200 }
    123c:	832450                                    	moveqz	a2, a4, a5

0000123f <Check_Phase_Frac+0x1b>:
    123f:	822d                                    	ae_sext16	a2, a2
    1241:	820b                                    	addi.n	a8, a2, -1
    1243:	160700851402268e                  	{ bany.w15	a6, a8, 126c <Check_Phase_Frac+0x48>; abs	a9, a6 }
    124b:	899d                                    	ae_sext16	a9, a9
    124d:	1b2927                                    	blt	a9, a2, 126c <Check_Phase_Frac+0x48>
    1250:	0a0c                                    	movi.n	a10, 0
    1252:	02042140008003ae                  	{ s16i	a10, a3, 0; movi	a2, 1 }
    125a:	f01d                                    	retw.n

0000125c <Check_Phase_Frac+0x38>:
    125c:	24b060                                    	extui	a11, a6, 0, 3
    125f:	060700a500034bee                  	{ bnez.w15	a11, 126c <Check_Phase_Frac+0x48>; abs	a12, a6 }
    1267:	8ccd                                    	ae_sext16	a12, a12
    1269:	e38ce6                                    	bgei	a12, 8, 1250 <Check_Phase_Frac+0x2c>
    126c:	020c                                    	movi.n	a2, 0
    126e:	f01d                                    	retw.n

00001270 <Check_Phase_Frac+0x4c>:
    1270:	030400471c61ebce                  	{ j	123f <Check_Phase_Frac+0x1b>; movi	a2, 64 }

00001278 <skew_ctrl_process>:
    1278:	006136                                    	entry	a1, 48
    127b:	0a0424411080027e                  	{ s16i	a7, a2, 12; movi	a10, 4 }
    1283:	0c044240008003be                  	{ movi	a11, 3; movi	a12, 2 }
    128b:	090401221800b78e                  	{ bgeui.w15	a7, 16, 12a5 <skew_ctrl_process+0x2d>; movi	a9, 1 }
    1293:	fb7bd1                                    	l32r	a13, 80 (2180 <__switchjump_table_xp_temp_42_16_36>)
    1296:	1d06e745141bdaee                  	{ l32r	a14, 40 (1278 <skew_ctrl_process>); addx4	a13, a7, a13 }
    129e:	0dd8                                    	l32i.n	a13, a13, 0
    12a0:	ddea                                    	add.n	a13, a13, a14
    12a2:	000da0                                    	jx	a13

000012a5 <skew_ctrl_process+0x2d>:
    12a5:	0f0460451400ddee                  	{ l32r	a14, 5c (40000000 <_end+0x3fffdde0>); movi	a15, 0 }
    12ad:	4852f2                                    	s16i	a15, a2, 144
    12b0:	52e9                                    	s32i.n	a14, a2, 20
    12b2:	148716                                    	beqz	a7, 13fe <skew_ctrl_process+0x186>
    12b5:	0e0429240820270e                  	{ beqi.w15	a7, 1, 13fe <skew_ctrl_process+0x186>; movi	a14, 9 }
    12bd:	42bd                                    	ae_l16ui.n	a11, a2, 0
    12bf:	39031240088102de                  	{ l16ui	a13, a2, 2; l16ui	a8, a2, 14 }
    12c7:	1d06bc40108a01de                  	{ s32i	a13, a1, 8; or	a9, a12, a12 }
    12cf:	0b02abc0089811de                  	{ s32i	a13, a1, 4; addi	a11, a11, -32 }
    12d7:	0c042e4608e0babe                  	{ moveqz	a9, a10, a11; movi	a13, 14 }

000012df <skew_ctrl_process+0x67>:
    12df:	190421400080119e                  	{ s32i	a9, a1, 0; movi	a9, 17 }
    12e7:	0b040b040840b89e                  	{ beq.w15	a8, a9, 1502 <skew_ctrl_process+0x28a>; movi	a11, 11 }
    12ef:	220740a41425f70e                  	{ beqi.w15	a7, 2, 14b0 <skew_ctrl_process+0x238>; nop }
    12f7:	220740a50825970e                  	{ beqi.w15	a7, 5, 148c <skew_ctrl_process+0x214>; nop }
    12ff:	220740a60025570e                  	{ beqi.w15	a7, 8, 148c <skew_ctrl_process+0x214>; nop }
    1307:	22074084082517be                  	{ beq.w15	a7, a11, 148c <skew_ctrl_process+0x214>; nop }
    130f:	220720840c25d7de                  	{ beq.w15	a7, a13, 148c <skew_ctrl_process+0x214>; nop }
    1317:	220760a41825b70e                  	{ beqi.w15	a7, 3, 14f0 <skew_ctrl_process+0x278>; nop }
    131f:	220740a51425570e                  	{ beqi.w15	a7, 6, 14cc <skew_ctrl_process+0x254>; nop }
    1327:	220740840c2517ee                  	{ beq.w15	a7, a14, 14cc <skew_ctrl_process+0x254>; nop }
    132f:	220740a61025d70e                  	{ beqi.w15	a7, 12, 14cc <skew_ctrl_process+0x254>; nop }
    1337:	fc0c                                    	movi.n	a12, 15
    1339:	23074084082577ce                  	{ beq.w15	a7, a12, 14cc <skew_ctrl_process+0x254>; nop }

00001341 <skew_ctrl_process+0xc9>:
    1341:	21d8                                    	l32i.n	a13, a1, 8
    1343:	1e03a140008004de                  	{ s16i	a13, a4, 0; l32i	a14, a1, 4 }
    134b:	66ed                                    	ae_s16i.n	a14, a6, 0
    134d:	42fd                                    	ae_l16ui.n	a15, a2, 0
    134f:	08044040008011be                  	{ movi	a11, 1; movi	a9, 0 }

00001357 <skew_ctrl_process+0xdf>:
    1357:	2f06b1c204b742ee                  	{ srli	a8, a14, 2; srli	a14, a14, 1 }
    135f:	0d02afc214b842de                  	{ srli	a10, a13, 2; addi	a12, a15, -32 }
    1367:	1d06abc30cba41de                  	{ srli	a13, a13, 1; moveqz	a9, a11, a12 }
    136f:	2906b84708e7da9e                  	{ movnez	a13, a10, a9; movnez	a14, a8, a9 }
    1377:	9eed                                    	ae_zext16	a14, a14
    1379:	9ddd                                    	ae_zext16	a13, a13
    137b:	220760a70045bf0e                  	{ beqi.w15	a15, 32, 1654 <skew_ctrl_process+0x3dc>; nop }
    1383:	fb4081                                    	l32r	a8, 84 (110c <skew_ctrl_16b>)
    1386:	05cd                                    	mov.n	a12, a5
    1388:	2206b24618eae33e                  	{ or	a11, a3, a3; or	a10, a2, a2 }
    1390:	0008e0                                    	callx8	a8

00001393 <skew_ctrl_process+0x11b>:
    1393:	489282                                    	l16si	a8, a2, 144
    1396:	190302240c12f80e                  	{ beqi.w15	a8, 1, 13d9 <skew_ctrl_process+0x161>; l16si	a9, a2, 146 }
    139e:	1c0288a0143f080e                  	{ beqi.w15	a8, 2, 14c2 <skew_ctrl_process+0x24a>; addi	a12, a8, -3 }
    13a6:	0a044044008800be                  	{ movi	a11, 0x100; movi	a10, 0x200 }
    13ae:	83abc0                                    	moveqz	a10, a11, a12
    13b1:	8aad                                    	ae_sext16	a10, a10
    13b3:	da0b                                    	addi.n	a13, a10, -1
    13b5:	0e040005100089de                  	{ bany.w15	a9, a13, 13ca <skew_ctrl_process+0x152>; movi	a14, 0 }
    13bd:	42e9                                    	s32i.n	a14, a2, 16
    13bf:	4a52e2                                    	s16i	a14, a2, 148
    13c2:	020400400060070e                  	{ j	13cd <skew_ctrl_process+0x155>; movi	a2, 0 }

000013ca <skew_ctrl_process+0x152>:
    13ca:	4a9222                                    	l16si	a2, a2, 148
    13cd:	60f120                                    	abs	a15, a2
    13d0:	8ffd                                    	ae_sext16	a15, a15
    13d2:	142fa7                                    	blt	a15, a10, 13ea <skew_ctrl_process+0x172>
    13d5:	020c                                    	movi.n	a2, 0
    13d7:	f01d                                    	retw.n

000013d9 <skew_ctrl_process+0x161>:
    13d9:	248090                                    	extui	a8, a9, 0, 3
    13dc:	2c1816                                    	beqz	a8, 16a1 <skew_ctrl_process+0x429>
    13df:	4a9222                                    	l16si	a2, a2, 148
    13e2:	609120                                    	abs	a9, a2
    13e5:	899d                                    	ae_sext16	a9, a9
    13e7:	ea89e6                                    	bgei	a9, 8, 13d5 <skew_ctrl_process+0x15d>
    13ea:	f01d                                    	retw.n

000013ec <skew_ctrl_process+0x174>:
    13ec:	000000                                        ...

000013ef <skew_ctrl_process+0x177>:
    13ef:	0d0c                                    	movi.n	a13, 0
    13f1:	2a03a242008152de                  	{ s16i	a13, a2, 144; l32i	a11, a2, 24 }
    13f9:	52b9                                    	s32i.n	a11, a2, 20
    13fb:	eb6756                                    	bnez	a7, 12b5 <skew_ctrl_process+0x3d>

000013fe <skew_ctrl_process+0x186>:
    13fe:	52bd                                    	ae_l16ui.n	a11, a2, 2
    1400:	64bd                                    	ae_s16i.n	a11, a4, 0
    1402:	66bd                                    	ae_s16i.n	a11, a6, 0
    1404:	42dd                                    	ae_l16ui.n	a13, a2, 0
    1406:	051282                                    	l16ui	a8, a2, 10
    1409:	41e2b0                                    	srli	a14, a11, 2
    140c:	0f02adc21cb851be                  	{ srli	a11, a11, 1; addi	a15, a13, -32 }
    1414:	3e06eea0003ad8ae                  	{ beqz.w15	a8, 15f0 <skew_ctrl_process+0x378>; moveqz	a11, a14, a15 }
    141c:	e80b                                    	addi.n	a14, a8, -1
    141e:	83c9e0                                    	moveqz	a12, a9, a14
    1421:	0c8d                                    	mov.n	a8, a12
    1423:	889d                                    	ae_sext16	a9, a8
    1425:	c0cb80                                    	sub	a12, a11, a8
    1428:	8ccd                                    	ae_sext16	a12, a12
    142a:	0a02e22300306d0e                  	{ beqi.w15	a13, 32, 15fa <skew_ctrl_process+0x382>; addi	a10, a2, 64 }
    1432:	11d9f0                                    	slli	a13, a9, 1
    1435:	2b069a4204eb42de                  	{ sub	a8, a2, a13; or	a14, a10, a10 }
    143d:	2902c84500b2094e                  	{ loopgtz	a9, 1455 <skew_ctrl_process+0x1dd>; addi	a8, a8, 74 }

00001445 <skew_ctrl_process+0x1cd>:
    1445:	280288400080089e                  	{ l16si	a9, a8, 0; addi	a8, a8, 2 }
    144d:	2402a5400080159e                  	{ s16i	a9, a5, 0; addi	a5, a5, 2 }
    1455:	0fac76                                    	loopgtz	a12, 1468 <skew_ctrl_process+0x1f0>
    1458:	220283400080139e                  	{ l16si	a9, a3, 0; addi	a3, a3, 2 }
    1460:	2402a5400080159e                  	{ s16i	a9, a5, 0; addi	a5, a5, 2 }

00001468 <skew_ctrl_process+0x1f0>:
    1468:	f6cd92                                    	addi	a9, a13, -10

0000146b <skew_ctrl_process+0x1f3>:
    146b:	0806c3400092159e                  	{ movi	a9, 5; add	a8, a3, a9 }
    1473:	0f8976                                    	loop	a9, 1486 <skew_ctrl_process+0x20e>
    1476:	280288400080089e                  	{ l16si	a9, a8, 0; addi	a8, a8, 2 }
    147e:	2e02ae4000800e9e                  	{ s16i	a9, a14, 0; addi	a14, a14, 2 }

00001486 <skew_ctrl_process+0x20e>:
    1486:	ffc246                                    	j	1393 <skew_ctrl_process+0x11b>

00001489 <skew_ctrl_process+0x211>:
    1489:	000000                                        ...

0000148c <skew_ctrl_process+0x214>:
    148c:	499292                                    	l16si	a9, a2, 146
    148f:	4892a2                                    	l16si	a10, a2, 144
    1492:	1902e9431c00dd8e                  	{ l32r	a8, 88 (1224 <Check_Phase_Frac>); addi	a9, a9, 1 }
    149a:	1a02b2420880529e                  	{ s16i	a9, a2, 146; addmi	a11, a2, 0x100 }
    14a2:	2a02abc21084529e                  	{ s16i	a9, a2, 148; addi	a11, a11, -110 }
    14aa:	0008e0                                    	callx8	a8
    14ad:	e90a16                                    	beqz	a10, 1341 <skew_ctrl_process+0xc9>

000014b0 <skew_ctrl_process+0x238>:
    14b0:	2a038140048001be                  	{ l32i	a11, a1, 0; l32i	a10, a1, 8 }
    14b8:	c0bab0                                    	sub	a11, a10, a11
    14bb:	9bad                                    	ae_zext16	a10, a11
    14bd:	21a9                                    	s32i.n	a10, a1, 8
    14bf:	ff9f86                                    	j	1341 <skew_ctrl_process+0xc9>

000014c2 <skew_ctrl_process+0x24a>:
    14c2:	0b0400471461ebee                  	{ j	13b1 <skew_ctrl_process+0x139>; movi	a10, 64 }

000014ca <skew_ctrl_process+0x252>:
	...

000014cc <skew_ctrl_process+0x254>:
    14cc:	499292                                    	l16si	a9, a2, 146
    14cf:	4892a2                                    	l16si	a10, a2, 144
    14d2:	3902e9c3141fdd8e                  	{ l32r	a8, 88 (1224 <Check_Phase_Frac>); addi	a9, a9, -1 }
    14da:	1a02b2420880529e                  	{ s16i	a9, a2, 146; addmi	a11, a2, 0x100 }
    14e2:	2a02abc21084529e                  	{ s16i	a9, a2, 148; addi	a11, a11, -110 }
    14ea:	0008e0                                    	callx8	a8
    14ed:	e50a16                                    	beqz	a10, 1341 <skew_ctrl_process+0xc9>

000014f0 <skew_ctrl_process+0x278>:
    14f0:	2a038140048001be                  	{ l32i	a11, a1, 0; l32i	a10, a1, 8 }
    14f8:	baba                                    	add.n	a11, a10, a11
    14fa:	9bad                                    	ae_zext16	a10, a11
    14fc:	21a9                                    	s32i.n	a10, a1, 8
    14fe:	ff8fc6                                    	j	1341 <skew_ctrl_process+0xc9>

00001501 <skew_ctrl_process+0x289>:
	...

00001502 <skew_ctrl_process+0x28a>:
    1502:	230740a01025570e                  	{ beqi.w15	a7, 2, 1690 <skew_ctrl_process+0x418>; nop }
    150a:	230720a10825f70e                  	{ beqi.w15	a7, 5, 166c <skew_ctrl_process+0x3f4>; nop }
    1512:	230720a20025b70e                  	{ beqi.w15	a7, 8, 166c <skew_ctrl_process+0x3f4>; nop }
    151a:	23072080082577be                  	{ beq.w15	a7, a11, 166c <skew_ctrl_process+0x3f4>; nop }
    1522:	23072080082537de                  	{ beq.w15	a7, a13, 166c <skew_ctrl_process+0x3f4>; nop }
    152a:	363726                                    	beqi	a7, 3, 1564 <skew_ctrl_process+0x2ec>
    152d:	0f6726                                    	beqi	a7, 6, 1540 <skew_ctrl_process+0x2c8>
    1530:	0c17e7                                    	beq	a7, a14, 1540 <skew_ctrl_process+0x2c8>
    1533:	09a726                                    	beqi	a7, 12, 1540 <skew_ctrl_process+0x2c8>
    1536:	fc0c                                    	movi.n	a12, 15
    1538:	2207009e1bc537ce                  	{ bne.w15	a7, a12, 1341 <skew_ctrl_process+0xc9>; nop }
    1540:	499292                                    	l16si	a9, a2, 146
    1543:	4892a2                                    	l16si	a10, a2, 144
    1546:	3902e9c30c1fd08e                  	{ l32r	a8, 88 (1224 <Check_Phase_Frac>); addi	a9, a9, -1 }
    154e:	1a02b2420880529e                  	{ s16i	a9, a2, 146; addmi	a11, a2, 0x100 }
    1556:	2a02abc21084529e                  	{ s16i	a9, a2, 148; addi	a11, a11, -110 }
    155e:	0008e0                                    	callx8	a8
    1561:	ddca16                                    	beqz	a10, 1341 <skew_ctrl_process+0xc9>
    1564:	1a038140048001be                  	{ l32i	a11, a1, 0; l32i	a10, a1, 4 }
    156c:	c0bab0                                    	sub	a11, a10, a11
    156f:	9bad                                    	ae_zext16	a10, a11
    1571:	11a9                                    	s32i.n	a10, a1, 4
    1573:	ff7286                                    	j	1341 <skew_ctrl_process+0xc9>

00001576 <skew_ctrl_process+0x2fe>:
    1576:	000000                                        ...

00001579 <skew_ctrl_process+0x301>:
    1579:	3c03a242008352ae                  	{ s16i	a10, a2, 144; l32i	a13, a2, 60 }
    1581:	52d9                                    	s32i.n	a13, a2, 20
    1583:	ff4ac6                                    	j	12b2 <skew_ctrl_process+0x3a>

00001586 <skew_ctrl_process+0x30e>:
    1586:	2e03a242008242ae                  	{ s16i	a10, a2, 144; l32i	a14, a2, 40 }
    158e:	52e9                                    	s32i.n	a14, a2, 20
    1590:	ff4786                                    	j	12b2 <skew_ctrl_process+0x3a>

00001593 <skew_ctrl_process+0x31b>:
    1593:	2e03a242008352be                  	{ s16i	a11, a2, 144; l32i	a15, a2, 56 }
    159b:	52f9                                    	s32i.n	a15, a2, 20
    159d:	ff4446                                    	j	12b2 <skew_ctrl_process+0x3a>

000015a0 <skew_ctrl_process+0x328>:
    15a0:	1803a242008242be                  	{ s16i	a11, a2, 144; l32i	a8, a2, 36 }
    15a8:	5289                                    	s32i.n	a8, a2, 20
    15aa:	ff4106                                    	j	12b2 <skew_ctrl_process+0x3a>

000015ad <skew_ctrl_process+0x335>:
    15ad:	1a03a242008352ce                  	{ s16i	a12, a2, 144; l32i	a11, a2, 52 }
    15b5:	52b9                                    	s32i.n	a11, a2, 20
    15b7:	ff3dc6                                    	j	12b2 <skew_ctrl_process+0x3a>

000015ba <skew_ctrl_process+0x342>:
    15ba:	0c03a242008252ce                  	{ s16i	a12, a2, 144; l32i	a13, a2, 32 }
    15c2:	52d9                                    	s32i.n	a13, a2, 20
    15c4:	ff3a86                                    	j	12b2 <skew_ctrl_process+0x3a>

000015c7 <skew_ctrl_process+0x34f>:
    15c7:	0e03a2420083429e                  	{ s16i	a9, a2, 144; l32i	a14, a2, 48 }
    15cf:	52e9                                    	s32i.n	a14, a2, 20
    15d1:	ff3746                                    	j	12b2 <skew_ctrl_process+0x3a>

000015d4 <skew_ctrl_process+0x35c>:
    15d4:	3e03a2420081529e                  	{ s16i	a9, a2, 144; l32i	a15, a2, 28 }
    15dc:	52f9                                    	s32i.n	a15, a2, 20
    15de:	ff3406                                    	j	12b2 <skew_ctrl_process+0x3a>

000015e1 <skew_ctrl_process+0x369>:
    15e1:	0b0c                                    	movi.n	a11, 0
    15e3:	3803a242008242be                  	{ s16i	a11, a2, 144; l32i	a8, a2, 44 }
    15eb:	5289                                    	s32i.n	a8, a2, 20
    15ed:	ff3046                                    	j	12b2 <skew_ctrl_process+0x3a>

000015f0 <skew_ctrl_process+0x378>:
    15f0:	090400471460ef2e                  	{ j	1423 <skew_ctrl_process+0x1ab>; movi	a8, 0 }

000015f8 <skew_ctrl_process+0x380>:
	...

000015fa <skew_ctrl_process+0x382>:
    15fa:	11d9e0                                    	slli	a13, a9, 2
    15fd:	2b069a4204eb42de                  	{ sub	a8, a2, a13; or	a14, a10, a10 }
    1605:	0020f0                                    	nop
    1608:	0902c84500b5094e                  	{ loopgtz	a9, 1620 <skew_ctrl_process+0x3a8>; addi	a8, a8, 84 }

00001610 <skew_ctrl_process+0x398>:
    1610:	080288400481089e                  	{ l32i	a9, a8, 0; addi	a8, a8, 4 }
    1618:	0502a5400081159e                  	{ s32i	a9, a5, 0; addi	a5, a5, 4 }
    1620:	0fac76                                    	loopgtz	a12, 1633 <skew_ctrl_process+0x3bb>
    1623:	020283400481139e                  	{ l32i	a9, a3, 0; addi	a3, a3, 4 }
    162b:	0502a5400081159e                  	{ s32i	a9, a5, 0; addi	a5, a5, 4 }

00001633 <skew_ctrl_process+0x3bb>:
    1633:	eccd92                                    	addi	a9, a13, -20

00001636 <skew_ctrl_process+0x3be>:
    1636:	0806c3400092159e                  	{ movi	a9, 5; add	a8, a3, a9 }
    163e:	0f8976                                    	loop	a9, 1651 <skew_ctrl_process+0x3d9>
    1641:	080288400481089e                  	{ l32i	a9, a8, 0; addi	a8, a8, 4 }
    1649:	0f02ae4000810e9e                  	{ s32i	a9, a14, 0; addi	a14, a14, 4 }

00001651 <skew_ctrl_process+0x3d9>:
    1651:	ff4f86                                    	j	1393 <skew_ctrl_process+0x11b>

00001654 <skew_ctrl_process+0x3dc>:
    1654:	fa8e81                                    	l32r	a8, 8c (6fc <skew_ctrl_32b>)
    1657:	05cd                                    	mov.n	a12, a5
    1659:	2206b24618eae33e                  	{ or	a11, a3, a3; or	a10, a2, a2 }
    1661:	0008e0                                    	callx8	a8
    1664:	ff4ac6                                    	j	1393 <skew_ctrl_process+0x11b>

00001667 <skew_ctrl_process+0x3ef>:
    1667:	00000000 49929200                                .....

0000166c <skew_ctrl_process+0x3f4>:
    166c:	499292                                    	l16si	a9, a2, 146
    166f:	4892a2                                    	l16si	a10, a2, 144
    1672:	1902e9420400d58e                  	{ l32r	a8, 88 (1224 <Check_Phase_Frac>); addi	a9, a9, 1 }
    167a:	1a02b2420880529e                  	{ s16i	a9, a2, 146; addmi	a11, a2, 0x100 }
    1682:	2a02abc21084529e                  	{ s16i	a9, a2, 148; addi	a11, a11, -110 }
    168a:	0008e0                                    	callx8	a8
    168d:	cb0a16                                    	beqz	a10, 1341 <skew_ctrl_process+0xc9>

00001690 <skew_ctrl_process+0x418>:
    1690:	1a038140048001be                  	{ l32i	a11, a1, 0; l32i	a10, a1, 4 }
    1698:	baba                                    	add.n	a11, a10, a11
    169a:	9bad                                    	ae_zext16	a10, a11
    169c:	11a9                                    	s32i.n	a10, a1, 4
    169e:	ff27c6                                    	j	1341 <skew_ctrl_process+0xc9>

000016a1 <skew_ctrl_process+0x429>:
    16a1:	0c0c                                    	movi.n	a12, 0
    16a3:	42c9                                    	s32i.n	a12, a2, 16
    16a5:	4a52c2                                    	s16i	a12, a2, 148
    16a8:	030400470c60e63e                  	{ j	13e2 <skew_ctrl_process+0x16a>; movi	a2, 0 }

000016b0 <intp_filter_16b>:
    16b0:	008136                                    	entry	a1, 64
    16b3:	4258                                    	l32i.n	a5, a2, 16
    16b5:	2f06e4411405d98e                  	{ l32r	a8, 5c (40000000 <_end+0x3fffdde0>); slli	a6, a4, 1 }
    16bd:	866d                                    	ae_sext16	a6, a6
    16bf:	1f06e5411c05d47e                  	{ l32r	a7, 90 (a000 <_end+0x7de0>); slli	a5, a5, 1 }
    16c7:	130332400081015e                  	{ s32i	a5, a1, 0; l16ui	a2, a2, 10 }
    16cf:	230720c000c5101e                  	{ ae_l32f24.i	aed0, a1, 0; nop }
    16d7:	0904614004c0d11e                  	{ ae_s32.l.i	aed0, a1, 4; movi	a9, 1 }
    16df:	100531a00040b2ae                  	{ beqz.w15	a2, 1937 <intp_filter_16b+0x287>; ae_l32.i	aed0, a1, 4 }
    16e7:	200748200839e20e                  	{ beqi.w15	a2, 1, 1887 <intp_filter_16b+0x1d7>; ae_cvt48a32	aed25, a8 }
    16ef:	fa5cd1                                    	l32r	a13, 60 (55555555 <_end+0x55553335>)
    16f2:	220740c304a5399e                  	{ ae_slaaq56	aed20, aed25, a9; nop }
    16fa:	fa67e1                                    	l32r	a14, 98 (4000 <_end+0x1de0>)
    16fd:	fa65f1                                    	l32r	a15, 94 (2aaaaaaa <_end+0x2aaa888a>)
    1700:	31f9                                    	s32i.n	a15, a1, 12
    1702:	330491451cbbfeee                  	{ ae_cvtp24a16x2.ll	aed23, a14, a14; ae_l32f24.i	aed27, a1, 12 }
    170a:	0e6d351b0c5d0c2585d021231e39d81f 	{ ae_s32.l.i	aed27, a1, 16; nop; ae_mov	aed17, aed25; ae_mov	aed22, aed20 }
    171a:	0e9d744bc199d300b39eb1b31c17981f 	{ ae_l32.i	aed26, a1, 16; ae_cvt48a32	aed24, a13; ae_mulsf32s.ll	aed22, aed0, aed23; ae_mulaf32s.ll	aed25, aed0, aed23 }
    172a:	0e9d385b0d350c00b11eb1a5041a577f 	{ ae_cvtp24a16x2.ll	aed21, a7, a7; nop; ae_mulsf32s.ll	aed20, aed0, aed26; ae_mulaf32s.ll	aed24, aed0, aed26 }
    173a:	0e9d145b08f50ca0c3bad18e08170b9f 	{ ae_round24x2f48ssym	aed19, aed25, aed25; nop; ae_mulsf32s.ll	aed17, aed0, aed23; ae_round24x2f48ssym	aed14, aed22, aed22 }
    174a:	0ead157b08e50ca0c3b6813e0001148f 	{ ae_round24x2f48ssym	aed12, aed24, aed24; nop; nop; ae_round24x2f48ssym	aed13, aed20, aed20 }
    175a:	0e1d2e5b09850c9c65ab6d781dae691f 	{ ae_slaaq56	aed17, aed17, a9; nop; ae_mulf32s.ll	aed15, aed13, aed14; ae_mulf32s.ll	aed18, aed19, aed12 }
    176a:	0e9d125b09ed0ca0c22a2d861c1344cf 	{ l32r	a12, 9c (ccd <skew_ctrl_32b+0x5d1>); nop; ae_mulf32s.ll	aed16, aed0, aed19; ae_round24x2f48ssym	aed10, aed17, aed17 }
    177a:	0e9d0e5b08fd0ca0c39ded4c11ca012f 	{ ae_round24x2f48ssym	aed1, aed18, aed18; nop; ae_mulf32s.ll	aed9, aed14, aed10; ae_round24x2f48ssym	aed7, aed15, aed15 }
    178a:	0e1d025b08e50c0c57ab6d3400ea1f0f 	{ ae_round24x2f48ssym	aed31, aed16, aed16; nop; ae_mulf32s.ll	aed6, aed7, aed10; ae_mulf32s.ll	aed11, aed1, aed0 }
    179a:	0e9d2a5b0d2d0ca0c30d2d4501aafccf 	{ ae_cvtp24a16x2.ll	aed15, a12, a12; nop; ae_mulf32s.ll	aed8, aed13, aed10; ae_round24x2f48ssym	aed3, aed9, aed9 }
    17aa:	0e9d065b08f40ca0c3f4cd12180c10bf 	{ ae_round24x2f48ssym	aed4, aed11, aed11; nop; ae_mulf32s.ll	aed2, aed0, aed12; ae_round24x2f48ssym	aed29, aed6, aed6 }
    17ba:	0e1d065b08e40c1c79ab6d6a03a1118f 	{ ae_round24x2f48ssym	aed5, aed8, aed8; nop; ae_mulf32s.ll	aed13, aed29, aed1; ae_mulf32s.ll	aed28, aed3, aed4 }
    17ca:	0e1d1e5b08fc0cec77ab6dd010a41e2f 	{ ae_round24x2f48ssym	aed30, aed2, aed2; nop; ae_mulf32s.ll	aed26, aed5, aed4; ae_mulf32s.ll	aed27, aed29, aed31 }
    17da:	0e1d065b08e50c3c65ab6dc703be0dcf 	{ ae_round24x2f48ssym	aed25, aed28, aed28; nop; ae_mulf32s.ll	aed24, aed29, aed30; ae_mulf32s.ll	aed18, aed7, aed4 }
    17ea:	0e9d5a50d8f126a0c3db4dbe1b2f16bf 	{ ae_round24x2f48ssym	aed14, aed27, aed27; neg	a11, a6; ae_mulf32s.ll	aed23, aed25, aed15; ae_round24x2f48ssym	aed22, aed26, aed26 }
    17fa:	0e9d7a46d18343a0c3d30db01ed5e92f 	{ ae_slaaq56	aed19, aed18, a9; addx2	a10, a4, a3; ae_mulf32s.ll	aed22, aed22, aed21; ae_round24x2f48ssym	aed20, aed24, aed24 }
    180a:	0e24d415a0ebaba455ab729d1972197f 	{ ae_round24x2f48ssym	aed21, aed23, aed23; ae_l16m.xu	aed20, a10, a11; ae_add64	aed18, aed18, aed19; ae_mulf32s.ll	aed10, aed20, aed21 }
    181a:	0e9cf6558192aba0c3c6cd101e95e9df 	{ ae_slaaq56	aed19, aed13, a9; ae_l16m.xu	aed16, a10, a11; ae_mulf32s.ll	aed2, aed20, aed21; ae_round24x2f48ssym	aed17, aed22, aed22 }
    182a:	0e9cf3355d12aba0c3b24b141e11d5df 	{ ae_add64	aed13, aed13, aed19; ae_l16m.xu	aed11, a10, a11; ae_mulaf32s.ll	aed2, aed16, aed17; ae_round24x2f48ssym	aed12, aed18, aed18 }
    183a:	0e9cce5540e2ab58859ead3b09cf05df 	{ ae_round24x2f48ssym	aed9, aed13, aed13; ae_l16m.xu	aed8, a10, a11; ae_mulf32s.ll	aed7, aed14, aed15; ae_mulaf32s.ll	aed2, aed11, aed12 }
    184a:	0e9cc73528e2abd0c5808b12110912af 	{ ae_round24x2f48ssym	aed6, aed10, aed10; ae_l16m.xu	aed5, a10, a11; ae_mulaf32s.ll	aed2, aed8, aed9; nop }
    185a:	0e9cc73518e2abd0c5808b1118a6107f 	{ ae_round24x2f48ssym	aed4, aed7, aed7; ae_l16m.xu	aed3, a10, a11; ae_mulaf32s.ll	aed2, aed5, aed6; nop }
    186a:	1e834b133464b13e                  	{ nop; nop; ae_mulaf32s.ll	aed2, aed3, aed4 }
    1872:	220740c00c85d22e                  	{ ae_round24x2f48ssym	aed1, aed2, aed2; nop }
    187a:	220740c00cc5141e                  	{ ae_s16m.l.i	aed1, a1, 8; nop }
    1882:	049122                                    	l16si	a2, a1, 8
    1885:	f01d                                    	retw.n

00001887 <intp_filter_16b+0x1d7>:
    1887:	fa0651                                    	l32r	a5, a0 (2000 <_DYNAMIC+0x22c>)
    188a:	230700c21ca5f55e                  	{ ae_cvtp24a16x2.ll	aed11, a5, a5; nop }
    1892:	22834a1b3560b13e                  	{ nop; nop; ae_mulf32s.ll	aed3, aed11, aed0 }
    189a:	220740c21485d33e                  	{ ae_round24x2f48ssym	aed10, aed3, aed3; nop }
    18a2:	0e9d065b09e50cd0c4808d210140980f 	{ ae_cvt48a32	aed9, a8; nop; ae_mulf32s.ll	aed4, aed10, aed0; nop }
    18b2:	220720c300a5599e                  	{ ae_slaaq56	aed9, aed9, a9; nop }
    18ba:	0ea5642595274dd0c50089250089577f 	{ ae_cvtp24a16x2.ll	aed5, a7, a7; slli	a2, a4, 3; ae_sub64	aed9, aed9, aed4; nop }
    18ca:	0e9d472890e232d0c5808b4818a0e64f 	{ ae_sub64	aed10, aed4, aed3; sub	a2, a3, a2; ae_mulaf32s.ll	aed9, aed5, aed0; nop }
    18da:	0e64d43538f2262185802322108304af 	{ ae_round24x2f48ssym	aed8, aed10, aed10; ae_l16m.xu	aed7, a2, a6; ae_sub64	aed3, aed3, aed4; ae_cvtq56p32s.l	aed6, aed0 }
    18ea:	0e24c81520e2263c7bab661a0966119f 	{ ae_round24x2f48ssym	aed5, aed9, aed9; ae_l16m.xu	aed4, a2, a6; ae_add64	aed6, aed6, aed3; ae_mulf32s.ll	aed29, aed7, aed8 }
    18fa:	0e9cdb3508ea26c8c58c6be91085026f 	{ ae_round24x2f48ssym	aed2, aed6, aed6; ae_l16m.xu	aed1, a2, a6; ae_mulaf32s.ll	aed29, aed4, aed5; ae_neg64	aed3, aed3 }
    190a:	0e9cc735f0e226d0c5808be818221f3f 	{ ae_round24x2f48ssym	aed31, aed3, aed3; ae_l16m.xu	aed30, a2, a6; ae_mulaf32s.ll	aed29, aed1, aed2; nop }
    191a:	1e834beb37dfb13e                  	{ nop; nop; ae_mulaf32s.ll	aed29, aed30, aed31 }
    1922:	230740c70485fdde                  	{ ae_round24x2f48ssym	aed28, aed29, aed29; nop }
    192a:	220740c704c5141e                  	{ ae_s16m.l.i	aed28, a1, 8; nop }
    1932:	049122                                    	l16si	a2, a1, 8
    1935:	f01d                                    	retw.n

00001937 <intp_filter_16b+0x287>:
    1937:	1184e0                                    	slli	a8, a4, 2
    193a:	c08380                                    	sub	a8, a3, a8
    193d:	220760c31c85b68e                  	{ ae_l16m.xu	aed15, a8, a6; nop }
    1945:	220740c308e51f2e                  	{ ae_cvtq56p32s.l	aed13, aed15; nop }
    194d:	2682326d29e0e68e                  	{ ae_l16m.xu	aed14, a8, a6; nop; ae_mulsf32s.ll	aed13, aed15, aed0 }
    1955:	1e834b6b35c0b13e                  	{ nop; nop; ae_mulaf32s.ll	aed13, aed14, aed0 }
    195d:	220740c30485ddde                  	{ ae_round24x2f48ssym	aed12, aed13, aed13; nop }
    1965:	220740c304c5141e                  	{ ae_s16m.l.i	aed12, a1, 8; nop }
    196d:	049122                                    	l16si	a2, a1, 8
    1970:	f01d                                    	retw.n

00001972 <intp_filter_16b+0x2c2>:
	...

00001974 <intp_filter_32b>:
    1974:	006136                                    	entry	a1, 48
    1977:	4268                                    	l32i.n	a6, a2, 16
    1979:	051222                                    	l16ui	a2, a2, 10
    197c:	1166f0                                    	slli	a6, a6, 1
    197f:	0169                                    	s32i.n	a6, a1, 0
    1981:	0eb216                                    	beqz	a2, 1a70 <intp_filter_32b+0xfc>
    1984:	011226                                    	beqi	a2, 1, 1989 <intp_filter_32b+0x15>
    1987:	f01d                                    	retw.n

00001989 <intp_filter_32b+0x15>:
    1989:	f9c5b1                                    	l32r	a11, a0 (2000 <_DYNAMIC+0x22c>)
    198c:	230720c118c5101e                  	{ ae_l32f24.i	aed7, a1, 0; nop }
    1994:	230760c11cc5d11e                  	{ ae_s32.l.i	aed7, a1, 4; nop }
    199c:	110511c114a7fbbe                  	{ ae_cvtp24a16x2.ll	aed6, a11, a11; ae_l32.i	aed7, a1, 4 }
    19a4:	22834a2334c7b13e                  	{ nop; nop; ae_mulf32s.ll	aed4, aed6, aed7 }
    19ac:	0c0461461c00d9ae                  	{ l32r	a10, 90 (a000 <_end+0x7de0>); movi	a13, 1 }
    19b4:	0ead4566c1e344a0c214813f1401a29f 	{ l32r	a9, 5c (40000000 <_end+0x3fffdde0>); addx2	a8, a4, a4; nop; ae_round24x2f48ssym	aed5, aed4, aed4 }

000019c4 <intp_filter_32b+0x50>:
    19c4:	0e9d664b152193d0c4008d4004a794ef 	{ slli	a9, a4, 2; ae_cvt48a32	aed2, a9; ae_mulf32s.ll	aed8, aed5, aed7; nop }
    19d4:	0e6d7565c1168e20c59c21381c01ad2f 	{ ae_slaaq56	aed2, aed2, a13; slli	a8, a8, 2; nop; ae_cvtq56p32s.l	aed3, aed7 }
    19e4:	0ea56028c5333860c509082500856aaf 	{ ae_cvtp24a16x2.ll	aed6, a10, a10; sub	a8, a3, a8; ae_sub64	aed5, aed8, aed4; ae_sub64	aed2, aed2, aed8 }
    19f4:	0ea4e43e05388930841ea4430c846c5f 	{ ae_trunca32q48	a12, aed5; ae_l32.xp	aed0, a8, a9; ae_sub64	aed4, aed4, aed8; ae_mulaf32s.ll	aed2, aed6, aed7 }
    1a04:	0ea4c41e29e189d0c480832101631c8f 	{ ae_movda32	aed1, a12; ae_l32.xp	aed5, a8, a9; ae_add64	aed3, aed3, aed4; nop }
    1a14:	000003f3088300fdd62303003c60b3af 	{ ae_trunca32q48	a10, aed3; ae_trunca32q48	a11, aed2; nop }
    1a24:	0e2cc05e19e98903c0ab402103c46b8f 	{ ae_movda32	aed6, a11; ae_l32.xp	aed3, a8, a9; ae_neg64	aed4, aed4; ae_mulf32r.hh	aed0, aed0, aed1 }
    1a34:	0e9d668b2520a4d0c4008b030ca6674f 	{ ae_trunca32q48	a7, aed4; ae_movda32	aed4, a10; ae_mulaf32r.hh	aed0, aed5, aed6; nop }
    1a44:	0e9d668b11e074d0c5808b011c64618f 	{ ae_l32.xp	aed1, a8, a9; ae_movda32	aed2, a7; ae_mulaf32r.hh	aed0, aed3, aed4; nop }
    1a54:	1e8346033422b13e                  	{ nop; nop; ae_mulaf32r.hh	aed0, aed1, aed2 }
    1a5c:	0ead257b0d240cd0c400813b0c01620f 	{ ae_trunca32q48	a2, aed0; nop; nop; nop }
    1a6c:	f01d                                    	retw.n

00001a6e <intp_filter_32b+0xfa>:
	...

00001a70 <intp_filter_32b+0xfc>:
    1a70:	11d4e0                                    	slli	a13, a4, 2
    1a73:	c0c3d0                                    	sub	a12, a3, a13
    1a76:	220720c21cc57dce                  	{ ae_l32.xp	aed11, a12, a13; nop }
    1a7e:	0ead257b0d240cd0c480813c1801892f 	{ ae_cvt64f32.h	aed8, aed11; nop; nop; nop }
    1a8e:	0ead656b514064d0c580813a0c01048f 	{ ae_srai64	aed8, aed8, 16; ae_movda32	aed10, a6; nop; nop }
    1a9e:	2682ad40216a9dce                  	{ ae_l32.xp	aed9, a12, a13; nop; ae_mulsf32r.hh	aed8, aed11, aed10 }
    1aa6:	1e834643352ab13e                  	{ nop; nop; ae_mulaf32r.hh	aed8, aed9, aed10 }
    1aae:	0ead257b0d240cd0c400813b0c01628f 	{ ae_trunca32q48	a2, aed8; nop; nop; nop }
    1abe:	f01d                                    	retw.n

00001ac0 <__do_global_ctors_aux>:
    1ac0:	004136                                    	entry	a1, 32
    1ac3:	f97881                                    	l32r	a8, a4 (1fd4 <_DYNAMIC+0x200>)
    1ac6:	7f2882                                    	l32i	a8, a8, 0x1fc
    1ac9:	f97721                                    	l32r	a2, a8 (21d0 <__CTOR_LIST___103>)
    1acc:	0c0826                                    	beqi	a8, -1, 1adc <__do_global_ctors_aux+0x1c>
    1acf:	02a8                                    	l32i.n	a10, a2, 0

00001ad1 <__do_global_ctors_aux+0x11>:
    1ad1:	000ae0                                    	callx8	a10
    1ad4:	fcc222                                    	addi	a2, a2, -4
    1ad7:	02a8                                    	l32i.n	a10, a2, 0
    1ad9:	f40a66                                    	bnei	a10, -1, 1ad1 <__do_global_ctors_aux+0x11>
    1adc:	f01d                                    	retw.n

00001ade <__do_global_ctors_aux+0x1e>:
	...

00001ae0 <__udivdi3>:
    1ae0:	008136                                    	entry	a1, 64
    1ae3:	049d                                    	mov.n	a9, a4
    1ae5:	0206b34610eaf22e                  	{ or	a10, a2, a2; or	a8, a3, a3 }
    1aed:	2b046024000095ae                  	{ beqz.w15	a5, 1bc4 <__udivdi3+0xe4>; movi	a11, 32 }
    1af5:	09b357                                    	bgeu	a3, a5, 1b02 <__udivdi3+0x22>
    1af8:	020440400080102e                  	{ movi	a2, 0; movi	a3, 0 }
    1b00:	f01d                                    	retw.n

00001b02 <__udivdi3+0x22>:
    1b02:	40f540                                    	nsau	a4, a5
    1b05:	277416                                    	beqz	a4, 1d80 <__udivdi3+0x2a0>
    1b08:	c02b40                                    	sub	a2, a11, a4
    1b0b:	400200                                    	ssr	a2

00001b0e <__udivdi3+0x2e>:
    1b0e:	230744c71ca4713e                  	{ srl	a15, a3; ssl	a4 }
    1b16:	3806954704f653ae                  	{ src	a12, a3, a10; src	a11, a5, a9 }
    1b1e:	9b8d                                    	ae_zext16	a8, a11
    1b20:	1b0250c714bf7a0e                  	{ ssai	16; extui	a13, a11, 16, 16 }
    1b28:	c2efd0                                    	quou	a14, a15, a13
    1b2b:	8238e0                                    	mull	a3, a8, a14
    1b2e:	e2ffd0                                    	remu	a15, a15, a13
    1b31:	1e069e471ce94fce                  	{ src	a15, a15, a12; or	a5, a14, a14 }
    1b39:	9ccd                                    	ae_zext16	a12, a12
    1b3b:	2e069f050808df3e                  	{ bgeu.w15	a15, a3, 1b58 <__udivdi3+0x78>; or	a2, a15, a15 }
    1b43:	2bfa                                    	add.n	a2, a11, a15
    1b45:	35028e87081f72be                  	{ bltu.w15	a2, a11, 1b58 <__udivdi3+0x78>; addi	a5, a14, -1 }
    1b4d:	07b237                                    	bgeu	a2, a3, 1b58 <__udivdi3+0x78>
    1b50:	2206eb471470ee5e                  	{ addi	a5, a14, -2; add	a2, a11, a2 }
    1b58:	3306b24510bb150e                  	{ slli	a6, a5, 16; sub	a15, a2, a3 }
    1b60:	c2efd0                                    	quou	a14, a15, a13
    1b63:	8288e0                                    	mull	a8, a8, a14
    1b66:	e2ffd0                                    	remu	a15, a15, a13
    1b69:	3106af4318e7645e                  	{ ssl	a4; slli	a15, a15, 16 }
    1b71:	2e06be4700e8ecfe                  	{ or	a12, a12, a15; or	a2, a14, a14 }
    1b79:	3c069c050808cc8e                  	{ bgeu.w15	a12, a8, 1b96 <__udivdi3+0xb6>; or	a3, a12, a12 }
    1b81:	3bca                                    	add.n	a3, a11, a12
    1b83:	33028e87081f63be                  	{ bltu.w15	a3, a11, 1b96 <__udivdi3+0xb6>; addi	a2, a14, -1 }
    1b8b:	07b387                                    	bgeu	a3, a8, 1b96 <__udivdi3+0xb6>
    1b8e:	3206eb471470fe2e                  	{ addi	a2, a14, -2; add	a3, a11, a3 }
    1b96:	100729c410e5e26e                  	{ or	a2, a2, a6; sll	a5, a9 }
    1b9e:	829250                                    	mull	a9, a2, a5
    1ba1:	a25250                                    	muluh	a5, a2, a5
    1ba4:	c03380                                    	sub	a3, a3, a8
    1ba7:	0f3357                                    	bltu	a3, a5, 1bba <__udivdi3+0xda>
    1baa:	30074a821c05a35e                  	{ bne.w15	a3, a5, 1c62 <__udivdi3+0x182>; sll	a7, a10 }
    1bb2:	220740810c05779e                  	{ bgeu.w15	a7, a9, 1c62 <__udivdi3+0x182>; nop }

00001bba <__udivdi3+0xda>:
    1bba:	3202c2c0009f003e                  	{ movi	a3, 0; addi	a2, a2, -1 }
    1bc2:	f01d                                    	retw.n

00001bc4 <__udivdi3+0xe4>:
    1bc4:	230740810805f34e                  	{ bgeu.w15	a3, a4, 1c66 <__udivdi3+0x186>; nop }
    1bcc:	40f450                                    	nsau	a5, a4
    1bcf:	230705a4000475ae                  	{ beqz.w15	a5, 1be2 <__udivdi3+0x102>; ssl	a5 }
    1bd7:	a14400                                    	sll	a4, a4

00001bda <__udivdi3+0xfa>:
    1bda:	200702c41ce4432e                  	{ src	a3, a3, a2; sll	a2, a2 }

00001be2 <__udivdi3+0x102>:
    1be2:	92dd                                    	ae_zext16	a13, a2
    1be4:	945d                                    	ae_zext16	a5, a4
    1be6:	150250c714be6a0e                  	{ ssai	16; extui	a9, a4, 16, 16 }
    1bee:	e2c390                                    	remu	a12, a3, a9
    1bf1:	c2b390                                    	quou	a11, a3, a9
    1bf4:	8285b0                                    	mull	a8, a5, a11
    1bf7:	2a069b4704ea5c2e                  	{ src	a12, a12, a2; or	a10, a11, a11 }
    1bff:	3c069c050808cc8e                  	{ bgeu.w15	a12, a8, 1c1c <__udivdi3+0x13c>; or	a3, a12, a12 }
    1c07:	3c4a                                    	add.n	a3, a12, a4
    1c09:	3b028b87081f634e                  	{ bltu.w15	a3, a4, 1c1c <__udivdi3+0x13c>; addi	a10, a11, -1 }
    1c11:	07b387                                    	bgeu	a3, a8, 1c1c <__udivdi3+0x13c>
    1c14:	3206e4471470fbae                  	{ addi	a10, a11, -2; add	a3, a4, a3 }
    1c1c:	c03380                                    	sub	a3, a3, a8
    1c1f:	e26390                                    	remu	a6, a3, a9
    1c22:	c23390                                    	quou	a3, a3, a9
    1c25:	828530                                    	mull	a8, a5, a3
    1c28:	115600                                    	slli	a5, a6, 16
    1c2b:	205d50                                    	or	a5, a13, a5
    1c2e:	230693050c08158e                  	{ bgeu.w15	a5, a8, 1c55 <__udivdi3+0x175>; or	a2, a3, a3 }
    1c36:	554a                                    	add.n	a5, a5, a4
    1c38:	32028387081fc54e                  	{ bltu.w15	a5, a4, 1c55 <__udivdi3+0x175>; addi	a2, a3, -1 }
    1c40:	11b587                                    	bgeu	a5, a8, 1c55 <__udivdi3+0x175>
    1c43:	2302a3c510bf0a0e                  	{ slli	a6, a10, 16; addi	a2, a3, -2 }
    1c4b:	0204204410e0f26e                  	{ or	a2, a2, a6; movi	a3, 0 }
    1c53:	f01d                                    	retw.n

00001c55 <__udivdi3+0x175>:
    1c55:	116a00                                    	slli	a6, a10, 16
    1c58:	0204204410e0f26e                  	{ or	a2, a2, a6; movi	a3, 0 }
    1c60:	f01d                                    	retw.n

00001c62 <__udivdi3+0x182>:
    1c62:	030c                                    	movi.n	a3, 0
    1c64:	f01d                                    	retw.n

00001c66 <__udivdi3+0x186>:
    1c66:	130c                                    	movi.n	a3, 1
    1c68:	c2c340                                    	quou	a12, a3, a4
    1c6b:	839c40                                    	moveqz	a9, a12, a4
    1c6e:	40f950                                    	nsau	a5, a9
    1c71:	080699240429b5ae                  	{ beqz.w15	a5, 1daa <__udivdi3+0x2ca>; or	a4, a9, a9 }
    1c79:	2506bb4318fb755e                  	{ ssl	a5; sub	a14, a11, a5 }

00001c81 <__udivdi3+0x1a1>:
    1c81:	33074ec100a439ce                  	{ sll	a4, a9; ssr	a14 }
    1c89:	230745c70ca4718e                  	{ srl	a13, a8; ssl	a5 }
    1c91:	945d                                    	ae_zext16	a5, a4
    1c93:	3306f8460874e04e                  	{ extui	a9, a4, 16, 16; src	a3, a8, a2 }
    1c9b:	c2cd90                                    	quou	a12, a13, a9
    1c9e:	030740c010a532ce                  	{ sll	a2, a2; ssai	16 }
    1ca6:	e2dd90                                    	remu	a13, a13, a9
    1ca9:	82b5c0                                    	mull	a11, a5, a12
    1cac:	2c069c470cea4d3e                  	{ src	a13, a13, a3; or	a10, a12, a12 }
    1cb4:	0c069d05080addbe                  	{ bgeu.w15	a13, a11, 1cd1 <__udivdi3+0x1f1>; or	a8, a13, a13 }
    1cbc:	8d4a                                    	add.n	a8, a13, a4
    1cbe:	3b028c87081f684e                  	{ bltu.w15	a8, a4, 1cd1 <__udivdi3+0x1f1>; addi	a10, a12, -1 }
    1cc6:	07b8b7                                    	bgeu	a8, a11, 1cd1 <__udivdi3+0x1f1>
    1cc9:	0806e4471472ecae                  	{ addi	a10, a12, -2; add	a8, a4, a8 }
    1cd1:	93dd                                    	ae_zext16	a13, a3
    1cd3:	c0c8b0                                    	sub	a12, a8, a11
    1cd6:	e2ec90                                    	remu	a14, a12, a9
    1cd9:	11ee00                                    	slli	a14, a14, 16
    1cdc:	c2cc90                                    	quou	a12, a12, a9
    1cdf:	82b5c0                                    	mull	a11, a5, a12
    1ce2:	0c06bc4708eaedee                  	{ or	a13, a13, a14; or	a8, a12, a12 }
    1cea:	3c069d050808ddbe                  	{ bgeu.w15	a13, a11, 1d07 <__udivdi3+0x227>; or	a3, a13, a13 }
    1cf2:	3d4a                                    	add.n	a3, a13, a4
    1cf4:	39028c87081f634e                  	{ bltu.w15	a3, a4, 1d07 <__udivdi3+0x227>; addi	a8, a12, -1 }
    1cfc:	07b3b7                                    	bgeu	a3, a11, 1d07 <__udivdi3+0x227>
    1cff:	3206e4471470fc8e                  	{ addi	a8, a12, -2; add	a3, a4, a3 }
    1d07:	11aa00                                    	slli	a10, a10, 16
    1d0a:	3a06b34610f8f8ae                  	{ or	a10, a8, a10; sub	a3, a3, a11 }

00001d12 <__udivdi3+0x232>:
    1d12:	92ed                                    	ae_zext16	a14, a2
    1d14:	404010                                    	ssai	16
    1d17:	e2d390                                    	remu	a13, a3, a9
    1d1a:	c2c390                                    	quou	a12, a3, a9
    1d1d:	8285c0                                    	mull	a8, a5, a12
    1d20:	3c069c470cea4d2e                  	{ src	a13, a13, a2; or	a11, a12, a12 }
    1d28:	3c069d050808dd8e                  	{ bgeu.w15	a13, a8, 1d45 <__udivdi3+0x265>; or	a3, a13, a13 }
    1d30:	3d4a                                    	add.n	a3, a13, a4
    1d32:	3b028c87081f734e                  	{ bltu.w15	a3, a4, 1d45 <__udivdi3+0x265>; addi	a11, a12, -1 }
    1d3a:	07b387                                    	bgeu	a3, a8, 1d45 <__udivdi3+0x265>
    1d3d:	3206e4471470fcbe                  	{ addi	a11, a12, -2; add	a3, a4, a3 }
    1d45:	c03380                                    	sub	a3, a3, a8
    1d48:	e26390                                    	remu	a6, a3, a9
    1d4b:	c23390                                    	quou	a3, a3, a9
    1d4e:	828530                                    	mull	a8, a5, a3
    1d51:	115600                                    	slli	a5, a6, 16
    1d54:	205e50                                    	or	a5, a14, a5
    1d57:	220693050c08b58e                  	{ bgeu.w15	a5, a8, 1d90 <__udivdi3+0x2b0>; or	a2, a3, a3 }
    1d5f:	554a                                    	add.n	a5, a5, a4
    1d61:	330283870c1f454e                  	{ bltu.w15	a5, a4, 1d90 <__udivdi3+0x2b0>; addi	a2, a3, -1 }
    1d69:	23b587                                    	bgeu	a5, a8, 1d90 <__udivdi3+0x2b0>
    1d6c:	2302a3c510bf0b0e                  	{ slli	a6, a11, 16; addi	a2, a3, -2 }
    1d74:	3a06ba4410e8e26e                  	{ or	a2, a2, a6; or	a3, a10, a10 }
    1d7c:	f01d                                    	retw.n

00001d7e <__udivdi3+0x29e>:
	...

00001d80 <__udivdi3+0x2a0>:
    1d80:	023537                                    	bltu	a5, a3, 1d86 <__udivdi3+0x2a6>
    1d83:	193297                                    	bltu	a2, a9, 1da0 <__udivdi3+0x2c0>
    1d86:	020440400080112e                  	{ movi	a2, 1; movi	a3, 0 }
    1d8e:	f01d                                    	retw.n

00001d90 <__udivdi3+0x2b0>:
    1d90:	118b00                                    	slli	a8, a11, 16
    1d93:	3a06ba4410e8e28e                  	{ or	a2, a2, a8; or	a3, a10, a10 }
    1d9b:	f01d                                    	retw.n

00001d9d <__udivdi3+0x2bd>:
    1d9d:	000000                                        ...

00001da0 <__udivdi3+0x2c0>:
    1da0:	020440400080102e                  	{ movi	a2, 0; movi	a3, 0 }
    1da8:	f01d                                    	retw.n

00001daa <__udivdi3+0x2ca>:
    1daa:	995d                                    	ae_zext16	a5, a9

00001dac <__udivdi3+0x2cc>:
    1dac:	230693401cea589e                  	{ sub	a3, a8, a9; or	a10, a3, a3 }
    1db4:	190210c71c7efa5e                  	{ j	1d12 <__udivdi3+0x232>; extui	a9, a9, 16, 16 }

00001dbc <_fini>:
    1dbc:	008136                                    	entry	a1, 64
    1dbf:	f8bb81                                    	l32r	a8, ac (d0 <__do_global_dtors_aux>)
    1dc2:	f03d                                    	nop.n
    1dc4:	0008e0                                    	callx8	a8

00001dc7 <_fini+0xb>:
    1dc7:	f01d                                    	retw.n
