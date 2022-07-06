
Build/lib/pisplit_fir_eq_position_independent.splitload.lib:     file format elf32-xtensa-le


Disassembly of section .text:

00000000 <_init-0x4c>:
   0:	0000009c 000001c8 0000046c 00000000     ........l.......
  10:	00000430 00000000 00000454 000000ec     0.......T.......
  20:	000004f4 00000490 000000c0 000004ab     ................
  30:	000000c8 000004c1 00000178 000004d7     ........x.......
  40:	00000480 00000438 00000064              ....8...d...

0000004c <_init>:
  4c:	008136                                    	entry	a1, 64
  4f:	ffec81                                    	l32r	a8, 0 (9c <frame_dummy>)
  52:	f03d                                    	nop.n
  54:	0008e0                                    	callx8	a8
  57:	ffeb81                                    	l32r	a8, 4 (1c8 <__do_global_ctors_aux>)
  5a:	f03d                                    	nop.n
  5c:	0008e0                                    	callx8	a8
  5f:	f01d                                    	retw.n

00000061 <_init+0x15>:
  61:	000000                                        ...

00000064 <__do_global_dtors_aux>:
  64:	004136                                    	entry	a1, 32
  67:	ffe821                                    	l32r	a2, 8 (46c <__do_global_dtors_aux.completed>)
  6a:	000232                                    	l8ui	a3, a2, 0
  6d:	63ec                                    	bnez.n	a3, 97 <__do_global_dtors_aux+0x33>

0000006f <__do_global_dtors_aux+0xb>:
  6f:	1248                                    	l32i.n	a4, a2, 4
  71:	0438                                    	l32i.n	a3, a4, 0
  73:	04028424000163ae                  	{ beqz.w15	a3, 84 <__do_global_dtors_aux+0x20>; addi	a4, a4, 4 }
  7b:	1249                                    	s32i.n	a4, a2, 4
  7d:	0003e0                                    	callx8	a3
  80:	fffac6                                    	j	6f <__do_global_dtors_aux+0xb>

00000083 <__do_global_dtors_aux+0x1f>:
	...

00000084 <__do_global_dtors_aux+0x20>:
  84:	ffe231                                    	l32r	a3, c (0 <_text_start>)
  87:	ffe2a1                                    	l32r	a10, 10 (430 <__FRAME_END__>)
  8a:	438c                                    	beqz.n	a3, 92 <__do_global_dtors_aux+0x2e>
  8c:	ffe081                                    	l32r	a8, c (0 <_text_start>)
  8f:	0008e0                                    	callx8	a8
  92:	130c                                    	movi.n	a3, 1
  94:	004232                                    	s8i	a3, a2, 0
  97:	f01d                                    	retw.n

00000099 <__do_global_dtors_aux+0x35>:
  99:	000000                                        ...

0000009c <frame_dummy>:
  9c:	004136                                    	entry	a1, 32
  9f:	ffdd21                                    	l32r	a2, 14 (0 <_text_start>)
  a2:	ffdba1                                    	l32r	a10, 10 (430 <__FRAME_END__>)
  a5:	ffdcb1                                    	l32r	a11, 18 (454 <frame_dummy.object>)
  a8:	428c                                    	beqz.n	a2, b0 <frame_dummy+0x14>
  aa:	ffda81                                    	l32r	a8, 14 (0 <_text_start>)
  ad:	0008e0                                    	callx8	a8

000000b0 <frame_dummy+0x14>:
  b0:	f01d                                    	retw.n

000000b2 <frame_dummy+0x16>:
	...

000000c0 <get_fir_wbeq_memsize>:
  c0:	004136                                    	entry	a1, 32
  c3:	70a122                                    	movi	a2, 0x170
  c6:	f01d                                    	retw.n

000000c8 <Voice_TxEQ_init>:
  c8:	004136                                    	entry	a1, 32
  cb:	baa062                                    	movi	a6, 186
  ce:	b75c                                    	movi.n	a7, 91
  d0:	532b                                    	addi.n	a5, a3, 2
  d2:	326a                                    	add.n	a3, a2, a6
  d4:	040c                                    	movi.n	a4, 0
  d6:	206220                                    	or	a6, a2, a2
  d9:	0b8776                                    	loop	a7, e8 <Voice_TxEQ_init+0x20>
  dc:	257d                                    	ae_l16si.n	a7, a5, 0
  de:	5d5642                                    	s16i	a4, a6, 186
  e1:	025672                                    	s16i	a7, a6, 4
  e4:	552b                                    	addi.n	a5, a5, 2
  e6:	662b                                    	addi.n	a6, a6, 2

000000e8 <Voice_TxEQ_init+0x20>:
  e8:	0239                                    	s32i.n	a3, a2, 0
  ea:	f01d                                    	retw.n

000000ec <Voice_TxEQ_Prcs_Length>:
  ec:	004136                                    	entry	a1, 32
  ef:	baa072                                    	movi	a7, 186
  f2:	70a182                                    	movi	a8, 0x170
  f5:	737a                                    	add.n	a7, a3, a7
  f7:	838a                                    	add.n	a8, a3, a8
  f9:	f3f670                                    	wur.ae_cbegin0	a7
  fc:	0378                                    	l32i.n	a7, a3, 0
  fe:	2da052                                    	movi	a5, 45
 101:	f3f780                                    	wur.ae_cend0	a8
 104:	fec222                                    	addi	a2, a2, -2
 107:	02a062                                    	movi	a6, 2
 10a:	04c382                                    	addi	a8, a3, 4

0000010d <Voice_TxEQ_Prcs_Length+0x21>:
 10d:	110473c00481712e                  	{ ae_l16m.iu	aed0, a2, 2; ae_l16x2m.i	aed1, a3, 4 }
 115:	0d07404004c0267e                  	{ ae_s16m.l.xc	aed0, a7, a6; ae_movi	aed0, 0 }
 11d:	1906f840148a867e                  	{ ae_l16m.xc	aed2, a7, a6; or	a9, a8, a8 }
 125:	178576                                    	loop	a5, 140 <Voice_TxEQ_Prcs_Length+0x54>
 128:	1e822a043841367e                  	{ ae_l16m.xc	aed3, a7, a6; nop; ae_mulaf32s.lh	aed0, aed2, aed1 }
 130:	0e9ce733152376d0c5808b000023419f 	{ ae_l16x2m.iu	aed1, a9, 4; ae_l16m.xc	aed2, a7, a6; ae_mulaf32s.ll	aed0, aed1, aed3; nop }

00000140 <Voice_TxEQ_Prcs_Length+0x54>:
 140:	1e81aa053c41f44e                  	{ addi	a4, a4, -1; nop; ae_mulaf32s.lh	aed0, aed2, aed1 }
 148:	0ead257b09040cd0c58081380c01030f 	{ ae_slaisq56s	aed0, aed0, 3; nop; nop; nop }
 158:	0ead217b0d440cf0c30001380c01015f 	{ nop; nop; nop; ae_roundsp16q48x2sym	aed0, aed0, aed0 }
 168:	220740c004c5102e                  	{ ae_s16m.l.i	aed0, a2, 0; nop }
 170:	9914e6                                    	bgei	a4, 1, 10d <Voice_TxEQ_Prcs_Length+0x21>
 173:	0379                                    	s32i.n	a7, a3, 0
 175:	f01d                                    	retw.n

00000177 <Voice_TxEQ_Prcs_Length+0x8b>:
	...

00000178 <Voice_TxEQ_Prcs>:
 178:	004136                                    	entry	a1, 32
 17b:	ffa841                                    	l32r	a4, 1c (ec <Voice_TxEQ_Prcs_Length>)
 17e:	f0a0c2                                    	movi	a12, 240
 181:	02ad                                    	mov.n	a10, a2
 183:	03bd                                    	mov.n	a11, a3
 185:	0004e0                                    	callx8	a4
 188:	f01d                                    	retw.n

0000018a <Voice_TxEQ_Prcs+0x12>:
	...

0000018c <_start>:
 18c:	004136                                    	entry	a1, 32
 18f:	ffa431                                    	l32r	a3, 20 (4f4 <printf_ptr>)
 192:	0228                                    	l32i.n	a2, a2, 0
 194:	ffa4a1                                    	l32r	a10, 24 (490 <export_parameter_array+0x10>)
 197:	ffa4b1                                    	l32r	a11, 28 (c0 <get_fir_wbeq_memsize>)
 19a:	0329                                    	s32i.n	a2, a3, 0
 19c:	0002e0                                    	callx8	a2
 19f:	0328                                    	l32i.n	a2, a3, 0
 1a1:	ffa2a1                                    	l32r	a10, 2c (4ab <export_parameter_array+0x2b>)
 1a4:	ffa3b1                                    	l32r	a11, 30 (c8 <Voice_TxEQ_init>)
 1a7:	0002e0                                    	callx8	a2
 1aa:	0328                                    	l32i.n	a2, a3, 0
 1ac:	ffa2a1                                    	l32r	a10, 34 (4c1 <export_parameter_array+0x41>)
 1af:	ffa2b1                                    	l32r	a11, 38 (178 <Voice_TxEQ_Prcs>)
 1b2:	0002e0                                    	callx8	a2
 1b5:	0328                                    	l32i.n	a2, a3, 0
 1b7:	ffa1a1                                    	l32r	a10, 3c (4d7 <export_parameter_array+0x57>)
 1ba:	ff98b1                                    	l32r	a11, 1c (ec <Voice_TxEQ_Prcs_Length>)
 1bd:	0002e0                                    	callx8	a2
 1c0:	ffa021                                    	l32r	a2, 40 (480 <export_parameter_array>)
 1c3:	f01d                                    	retw.n

000001c5 <_start+0x39>:
 1c5:	000000                                        ...

000001c8 <__do_global_ctors_aux>:
 1c8:	004136                                    	entry	a1, 32
 1cb:	ff9e31                                    	l32r	a3, 44 (438 <__CTOR_END__>)
 1ce:	fcc322                                    	addi	a2, a3, -4
 1d1:	002222                                    	l32i	a2, a2, 0
 1d4:	020283a4001e920e                  	{ beqi.w15	a2, -1, 1e9 <__do_global_ctors_aux+0x21>; addi	a3, a3, -8 }

000001dc <__do_global_ctors_aux+0x14>:
 1dc:	0002e0                                    	callx8	a2
 1df:	0328                                    	l32i.n	a2, a3, 0
 1e1:	0302e3bc07ffb26e                  	{ bnei.w15	a2, -1, 1dc <__do_global_ctors_aux+0x14>; addi	a3, a3, -4 }
 1e9:	f01d                                    	retw.n

000001eb <__do_global_ctors_aux+0x23>:
	...

000001ec <_fini>:
 1ec:	008136                                    	entry	a1, 64
 1ef:	ff9681                                    	l32r	a8, 48 (64 <__do_global_dtors_aux>)
 1f2:	f03d                                    	nop.n
 1f4:	0008e0                                    	callx8	a8

000001f7 <_fini+0xb>:
 1f7:	f01d                                    	retw.n
