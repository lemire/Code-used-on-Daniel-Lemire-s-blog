	.file	"pred.cpp"
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4, 0x0                          // -- Begin function _Z12classify_asmPKh
.LCPI0_0:
	.byte	255                             // 0xff
	.byte	44                              // 0x2c
	.byte	58                              // 0x3a
	.byte	91                              // 0x5b
	.byte	93                              // 0x5d
	.byte	123                             // 0x7b
	.byte	125                             // 0x7d
	.byte	255                             // 0xff
	.byte	44                              // 0x2c
	.byte	58                              // 0x3a
	.byte	91                              // 0x5b
	.byte	93                              // 0x5d
	.byte	123                             // 0x7b
	.byte	125                             // 0x7d
	.byte	44                              // 0x2c
	.byte	58                              // 0x3a
	.text
	.globl	_Z12classify_asmPKh
	.p2align	2
	.type	_Z12classify_asmPKh,@function
_Z12classify_asmPKh:                    // @_Z12classify_asmPKh
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #128
	.cfi_def_cfa_offset 128
	adrp	x8, .LCPI0_0
	ldp	q1, q2, [x0]
	ptrue	p0.b, vl16
	ldr	q0, [x8, :lo12:.LCPI0_0]
	mov	x8, sp
	ldp	q3, q4, [x0, #32]
	add	x9, x8, #32
	match	p1.b, p0/z, z1.b, z0.b
	//APP
	str	p1, [x8]
	//NO_APP
	match	p2.b, p0/z, z2.b, z0.b
	//APP
	str	p2, [x9]
	//NO_APP
	add	x9, x8, #64
	match	p3.b, p0/z, z3.b, z0.b
	//APP
	str	p3, [x9]
	//NO_APP
	add	x8, x8, #96
	match	p0.b, p0/z, z4.b, z0.b
	//APP
	str	p0, [x8]
	//NO_APP
	ldrh	w8, [sp]
	ldrh	w9, [sp, #32]
	ldrh	w10, [sp, #64]
	orr	x8, x8, x9, lsl #16
	ldrh	w9, [sp, #96]
	orr	x8, x8, x10, lsl #32
	orr	x0, x8, x9, lsl #48
	add	sp, sp, #128
	.cfi_def_cfa_offset 0
	ret
.Lfunc_end0:
	.size	_Z12classify_asmPKh, .Lfunc_end0-_Z12classify_asmPKh
	.cfi_endproc
                                        // -- End function
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4, 0x0                          // -- Begin function _Z15classify_bridgePKh
.LCPI1_0:
	.byte	255                             // 0xff
	.byte	44                              // 0x2c
	.byte	58                              // 0x3a
	.byte	91                              // 0x5b
	.byte	93                              // 0x5d
	.byte	123                             // 0x7b
	.byte	125                             // 0x7d
	.byte	255                             // 0xff
	.byte	44                              // 0x2c
	.byte	58                              // 0x3a
	.byte	91                              // 0x5b
	.byte	93                              // 0x5d
	.byte	123                             // 0x7b
	.byte	125                             // 0x7d
	.byte	44                              // 0x2c
	.byte	58                              // 0x3a
.LCPI1_1:
	.byte	1                               // 0x1
	.byte	2                               // 0x2
	.byte	4                               // 0x4
	.byte	8                               // 0x8
	.byte	16                              // 0x10
	.byte	32                              // 0x20
	.byte	64                              // 0x40
	.byte	128                             // 0x80
	.byte	1                               // 0x1
	.byte	2                               // 0x2
	.byte	4                               // 0x4
	.byte	8                               // 0x8
	.byte	16                              // 0x10
	.byte	32                              // 0x20
	.byte	64                              // 0x40
	.byte	128                             // 0x80
	.text
	.globl	_Z15classify_bridgePKh
	.p2align	2
	.type	_Z15classify_bridgePKh,@function
_Z15classify_bridgePKh:                 // @_Z15classify_bridgePKh
	.cfi_startproc
// %bb.0:
	adrp	x8, .LCPI1_0
	ldp	q1, q2, [x0]
	ldp	q3, q4, [x0, #32]
	ptrue	p0.b, vl16
	ldr	q0, [x8, :lo12:.LCPI1_0]
	ptrue	p4.b
	adrp	x8, .LCPI1_1
	add	x8, x8, :lo12:.LCPI1_1
	match	p1.b, p0/z, z1.b, z0.b
	ld1rqb	{ z1.b }, p4/z, [x8]
	match	p2.b, p0/z, z2.b, z0.b
	match	p3.b, p0/z, z3.b, z0.b
	match	p0.b, p0/z, z4.b, z0.b
	movi	v0.2d, #0000000000000000
	sel	z2.b, p1, z1.b, z0.b
	sel	z3.b, p2, z1.b, z0.b
	sel	z4.b, p3, z1.b, z0.b
	mov	z0.b, p0/m, z1.b
	addp	v1.16b, v2.16b, v3.16b
	addp	v0.16b, v4.16b, v0.16b
	addp	v0.16b, v1.16b, v0.16b
	addp	v0.16b, v0.16b, v0.16b
	fmov	x0, d0
	ret
.Lfunc_end1:
	.size	_Z15classify_bridgePKh, .Lfunc_end1-_Z15classify_bridgePKh
	.cfi_endproc
                                        // -- End function
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4, 0x0                          // -- Begin function _Z13classify_bextPKh
.LCPI2_0:
	.byte	255                             // 0xff
	.byte	44                              // 0x2c
	.byte	58                              // 0x3a
	.byte	91                              // 0x5b
	.byte	93                              // 0x5d
	.byte	123                             // 0x7b
	.byte	125                             // 0x7d
	.byte	255                             // 0xff
	.byte	44                              // 0x2c
	.byte	58                              // 0x3a
	.byte	91                              // 0x5b
	.byte	93                              // 0x5d
	.byte	123                             // 0x7b
	.byte	125                             // 0x7d
	.byte	44                              // 0x2c
	.byte	58                              // 0x3a
.LCPI2_1:
	.byte	0                               // 0x0
	.byte	8                               // 0x8
	.byte	16                              // 0x10
	.byte	24                              // 0x18
	.byte	32                              // 0x20
	.byte	40                              // 0x28
	.byte	48                              // 0x30
	.byte	56                              // 0x38
	.byte	0                               // 0x0
	.byte	0                               // 0x0
	.byte	0                               // 0x0
	.byte	0                               // 0x0
	.byte	0                               // 0x0
	.byte	0                               // 0x0
	.byte	0                               // 0x0
	.byte	0                               // 0x0
	.text
	.globl	_Z13classify_bextPKh
	.p2align	2
	.type	_Z13classify_bextPKh,@function
_Z13classify_bextPKh:                   // @_Z13classify_bextPKh
	.cfi_startproc
// %bb.0:
	adrp	x8, .LCPI2_0
	ldp	q1, q2, [x0]
	ldr	q0, [x8, :lo12:.LCPI2_0]
	ptrue	p0.b, vl16
	adrp	x8, .LCPI2_1
	match	p1.b, p0/z, z1.b, z0.b
	dupm	z1.b, #0x80
	mov	z3.b, p1/z, #-1                 // =0xffffffffffffffff
	match	p1.b, p0/z, z2.b, z0.b
	bext	z3.d, z3.d, z1.d
	mov	z2.b, p1/z, #-1                 // =0xffffffffffffffff
	bext	z4.d, z2.d, z1.d
	ldp	q2, q7, [x0, #32]
	match	p1.b, p0/z, z2.b, z0.b
	match	p0.b, p0/z, z7.b, z0.b
	mov	z2.b, p1/z, #-1                 // =0xffffffffffffffff
	mov	z0.b, p0/z, #-1                 // =0xffffffffffffffff
	bext	z5.d, z2.d, z1.d
	bext	z6.d, z0.d, z1.d
	ldr	q0, [x8, :lo12:.LCPI2_1]
	tbl	v0.16b, { v3.16b, v4.16b, v5.16b, v6.16b }, v0.16b
	fmov	x0, d0
	ret
.Lfunc_end2:
	.size	_Z13classify_bextPKh, .Lfunc_end2-_Z13classify_bextPKh
	.cfi_endproc
                                        // -- End function
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4, 0x0                          // -- Begin function _Z15classify_memcpyPKh
.LCPI3_0:
	.byte	255                             // 0xff
	.byte	44                              // 0x2c
	.byte	58                              // 0x3a
	.byte	91                              // 0x5b
	.byte	93                              // 0x5d
	.byte	123                             // 0x7b
	.byte	125                             // 0x7d
	.byte	255                             // 0xff
	.byte	44                              // 0x2c
	.byte	58                              // 0x3a
	.byte	91                              // 0x5b
	.byte	93                              // 0x5d
	.byte	123                             // 0x7b
	.byte	125                             // 0x7d
	.byte	44                              // 0x2c
	.byte	58                              // 0x3a
	.text
	.globl	_Z15classify_memcpyPKh
	.p2align	2
	.type	_Z15classify_memcpyPKh,@function
_Z15classify_memcpyPKh:                 // @_Z15classify_memcpyPKh
	.cfi_startproc
// %bb.0:
	str	x30, [sp, #-16]!                // 8-byte Folded Spill
	.cfi_def_cfa_offset 16
	.cfi_offset w30, -16
	addvl	sp, sp, #-1
	.cfi_escape 0x0f, 0x0c, 0x8f, 0x00, 0x11, 0x10, 0x22, 0x11, 0x08, 0x92, 0x2e, 0x00, 0x1e, 0x22 // sp + 16 + 8 * VG
	adrp	x8, .LCPI3_0
	ldp	q1, q2, [x0]
	ptrue	p0.b, vl16
	ldr	q0, [x8, :lo12:.LCPI3_0]
	addpl	x8, sp, #6
	addpl	x9, sp, #4
	addpl	x10, sp, #2
	match	p1.b, p0/z, z1.b, z0.b
	match	p2.b, p0/z, z2.b, z0.b
	ldp	q1, q2, [x0, #32]
	match	p3.b, p0/z, z1.b, z0.b
	match	p0.b, p0/z, z2.b, z0.b
	str	p1, [sp, #6, mul vl]
	str	p2, [sp, #4, mul vl]
	ldrh	w8, [x8]
	ldrh	w9, [x9]
	str	p3, [sp, #2, mul vl]
	orr	x8, x8, x9, lsl #16
	ldrh	w10, [x10]
	str	p0, [sp]
	ldrh	w9, [sp]
	orr	x8, x8, x10, lsl #32
	orr	x0, x8, x9, lsl #48
	addvl	sp, sp, #1
	.cfi_def_cfa wsp, 16
	ldr	x30, [sp], #16                  // 8-byte Folded Reload
	.cfi_def_cfa_offset 0
	.cfi_restore w30
	ret
.Lfunc_end3:
	.size	_Z15classify_memcpyPKh, .Lfunc_end3-_Z15classify_memcpyPKh
	.cfi_endproc
                                        // -- End function
	.globl	_Z15classify_scalarPKh          // -- Begin function _Z15classify_scalarPKh
	.p2align	2
	.type	_Z15classify_scalarPKh,@function
_Z15classify_scalarPKh:                 // @_Z15classify_scalarPKh
	.cfi_startproc
// %bb.0:
	stp	x29, x30, [sp, #-80]!           // 16-byte Folded Spill
	.cfi_def_cfa_offset 80
	str	x25, [sp, #16]                  // 8-byte Folded Spill
	stp	x24, x23, [sp, #32]             // 16-byte Folded Spill
	stp	x22, x21, [sp, #48]             // 16-byte Folded Spill
	stp	x20, x19, [sp, #64]             // 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 80
	.cfi_offset w19, -8
	.cfi_offset w20, -16
	.cfi_offset w21, -24
	.cfi_offset w22, -32
	.cfi_offset w23, -40
	.cfi_offset w24, -48
	.cfi_offset w25, -64
	.cfi_offset w30, -72
	.cfi_offset w29, -80
	mov	x19, x0
	mov	x22, xzr
	mov	x20, xzr
	adrp	x21, .L.str
	add	x21, x21, :lo12:.L.str
	mov	w23, #255                       // =0xff
	mov	w24, #1                         // =0x1
.LBB4_1:                                // =>This Inner Loop Header: Depth=1
	ldrb	w25, [x19, x22]
	mov	x0, x21
	mov	w2, #7                          // =0x7
	ands	w1, w25, #0xff
	bl	memchr
	ands	w8, w25, #0xff
	lsl	x9, x24, x22
	add	x22, x22, #1
	ccmp	x0, #0, #4, ne
	ccmp	w8, w23, #4, eq
	csel	x8, x9, xzr, eq
	cmp	x22, #64
	orr	x20, x8, x20
	b.ne	.LBB4_1
// %bb.2:
	mov	x0, x20
	.cfi_def_cfa wsp, 80
	ldp	x20, x19, [sp, #64]             // 16-byte Folded Reload
	ldr	x25, [sp, #16]                  // 8-byte Folded Reload
	ldp	x22, x21, [sp, #48]             // 16-byte Folded Reload
	ldp	x24, x23, [sp, #32]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #80             // 16-byte Folded Reload
	.cfi_def_cfa_offset 0
	.cfi_restore w19
	.cfi_restore w20
	.cfi_restore w21
	.cfi_restore w22
	.cfi_restore w23
	.cfi_restore w24
	.cfi_restore w25
	.cfi_restore w30
	.cfi_restore w29
	ret
.Lfunc_end4:
	.size	_Z15classify_scalarPKh, .Lfunc_end4-_Z15classify_scalarPKh
	.cfi_endproc
                                        // -- End function
	.globl	main                            // -- Begin function main
	.p2align	2
	.type	main,@function
main:                                   // @main
	.cfi_startproc
// %bb.0:
	stp	x29, x30, [sp, #-96]!           // 16-byte Folded Spill
	.cfi_def_cfa_offset 96
	stp	x28, x27, [sp, #16]             // 16-byte Folded Spill
	stp	x26, x25, [sp, #32]             // 16-byte Folded Spill
	stp	x24, x23, [sp, #48]             // 16-byte Folded Spill
	stp	x22, x21, [sp, #64]             // 16-byte Folded Spill
	stp	x20, x19, [sp, #80]             // 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 96
	.cfi_offset w19, -8
	.cfi_offset w20, -16
	.cfi_offset w21, -24
	.cfi_offset w22, -32
	.cfi_offset w23, -40
	.cfi_offset w24, -48
	.cfi_offset w25, -56
	.cfi_offset w26, -64
	.cfi_offset w27, -72
	.cfi_offset w28, -80
	.cfi_offset w30, -88
	.cfi_offset w29, -96
	sub	x9, sp, #2656
	addvl	x9, x9, #-3
	and	sp, x9, #0xffffffffffffffc0
	rdvl	x1, #8
	adrp	x0, .L.str.1
	add	x0, x0, :lo12:.L.str.1
                                        // kill: def $w1 killed $w1 killed $x1
	bl	printf
	mov	x10, #32557                     // =0x7f2d
	mov	w8, #42                         // =0x2a
	mov	w9, #1                          // =0x1
	movk	x10, #19605, lsl #16
	add	x11, sp, #152
	str	x8, [sp, #152]
	movk	x10, #62509, lsl #32
	movk	x10, #22609, lsl #48
.LBB5_1:                                // =>This Inner Loop Header: Depth=1
	eor	x8, x8, x8, lsr #62
	madd	x8, x8, x10, x9
	str	x8, [x11, x9, lsl #3]
	add	x9, x9, #1
	cmp	x9, #312
	b.ne	.LBB5_1
// %bb.2:
	rdvl	x8, #1
	mov	w9, #-1                         // =0xffffffff
	mov	w18, #252                       // =0xfc
	lsr	x17, x8, #4
	incd	x9
	mov	w12, #-1                        // =0xffffffff
	index	z0.d, #0, #1
	incd	x12
	mov	w10, #156                       // =0x9c
	mul	x8, x17, x18
	ptrue	p2.d
	mov	x25, #6633                      // =0x19e9
	mov	w9, w9
	mov	w11, #-1                        // =0xffffffff
	movk	x25, #43366, lsl #16
	mov	z1.d, x9
	mov	w9, #-1                         // =0xffffffff
	incd	x11
	incd	x9
	movk	x25, #28506, lsl #32
	ptrue	p0.d, vl1
	movk	x25, #46338, lsl #48
	mov	x5, #44565                      // =0xae15
	mov	x3, #3987079168                 // =0xeda60000
	and	x1, x8, x10
	mov	w8, w12
	cmpeq	p3.d, p2/z, z0.d, z1.d
	mov	z1.d, x8
	mov	w8, w9
	mov	z17.d, x25
	whilels	p6.d, xzr, x8
	mov	w8, w11
	movk	x5, #57671, lsl #16
	rev	p4.d, p0.d
	movk	x3, #32767, lsl #32
	mov	x4, #262645840084992            // =0xeee000000000
	cmpeq	p7.d, p2/z, z0.d, z1.d
	ptrue	p5.b
	movk	x5, #5242, lsl #32
	whilels	p0.d, xzr, x8
	mov	w16, wzr
	mov	w23, #312                       // =0x138
	cntd	x0
	mov	w13, #42                        // =0x2a
	add	x19, sp, #152
	mov	x27, #1                         // =0x1
	mov	x28, #156                       // =0x9c
	mov	w2, #152                        // =0x98
	mov	x20, #157                       // =0x9d
	movk	x3, #29142, lsl #48
	movk	x4, #65527, lsl #48
	movk	x5, #18350, lsl #48
	mov	w6, #25                         // =0x19
	adrp	x7, .L__const.main.alphabet
	add	x7, x7, :lo12:.L__const.main.alphabet
	add	x26, sp, #64
	adrp	x24, .L.str
	add	x24, x24, :lo12:.L.str
	str	x23, [sp, #2648]
	str	p0, [x29, #-1, mul vl]          // 2-byte Folded Spill
	stp	x1, x17, [sp, #24]              // 16-byte Folded Spill
	str	p3, [x29, #-2, mul vl]          // 2-byte Folded Spill
	str	p4, [x29, #-3, mul vl]          // 2-byte Folded Spill
	str	z17, [x29, #-2, mul vl]         // 16-byte Folded Spill
	str	p6, [x29, #-17, mul vl]         // 2-byte Folded Spill
	str	p7, [x29, #-18, mul vl]         // 2-byte Folded Spill
.LBB5_3:                                // =>This Loop Header: Depth=1
                                        //     Child Loop BB5_5 Depth 2
                                        //       Child Loop BB5_7 Depth 3
                                        //       Child Loop BB5_10 Depth 3
                                        //       Child Loop BB5_12 Depth 3
                                        //       Child Loop BB5_14 Depth 3
                                        //     Child Loop BB5_17 Depth 2
	mov	x8, xzr
	b	.LBB5_5
.LBB5_4:                                //   in Loop: Header=BB5_5 Depth=2
	mov	x9, x23
	add	x23, x23, #1
	str	x23, [sp, #2648]
	ldr	x9, [x19, x9, lsl #3]
	lsr	x10, x9, #29
	and	x10, x10, #0x5555555555555555
	eor	x9, x10, x9
	and	x10, x3, x9, lsl #17
	eor	x9, x10, x9
	and	x10, x4, x9, lsl #37
	eor	x9, x10, x9
	eor	x9, x9, x9, lsr #43
	umulh	x10, x9, x5
	sub	x11, x9, x10
	add	x10, x10, x11, lsr #1
	lsr	x10, x10, #4
	msub	x9, x10, x6, x9
	ldrb	w9, [x7, x9]
	strb	w9, [x26, x8]
	add	x8, x8, #1
	cmp	x8, #64
	b.eq	.LBB5_16
.LBB5_5:                                //   Parent Loop BB5_3 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB5_7 Depth 3
                                        //       Child Loop BB5_10 Depth 3
                                        //       Child Loop BB5_12 Depth 3
                                        //       Child Loop BB5_14 Depth 3
	cmp	x23, #312
	b.lo	.LBB5_4
// %bb.6:                               //   in Loop: Header=BB5_5 Depth=2
	mov	z2.d, p3/m, x13
	mov	x9, xzr
	neg	x10, x1
	mov	x11, x0
.LBB5_7:                                //   Parent Loop BB5_3 Depth=1
                                        //     Parent Loop BB5_5 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	mov	z0.d, z2.d
	add	x12, x19, x9
	add	x13, x19, x11, lsl #3
	ld1d	{ z1.d }, p2/z, [x12, x27, lsl #3]
	incw	x11
	ld1d	{ z2.d }, p2/z, [x13, x27, lsl #3]
	splice	z3.d, p4, { z0.d, z1.d }
	mov	z5.d, z1.d
	ld1d	{ z7.d }, p2/z, [x13, x28, lsl #3]
	add	x13, x10, x11
	splice	z4.d, p4, { z1.d, z2.d }
	and	z5.d, z5.d, #0x7ffffffe
	mov	z6.d, z2.d
	and	z3.d, z3.d, #0xffffffff80000000
	decd	x13
	and	z6.d, z6.d, #0x7ffffffe
	and	z4.d, z4.d, #0xffffffff80000000
	orr	z3.d, z5.d, z3.d
	ld1d	{ z5.d }, p2/z, [x12, x28, lsl #3]
	orr	z4.d, z6.d, z4.d
	lsr	z3.d, z3.d, #1
	mov	z6.d, z1.d
	and	z6.d, z6.d, #0x1
	eor	z3.d, z3.d, z5.d
	mov	z5.d, z2.d
	lsr	z4.d, z4.d, #1
	and	z5.d, z5.d, #0x1
	cmpeq	p0.d, p2/z, z6.d, #0
	eor	z6.d, z3.d, z17.d
	eor	z4.d, z4.d, z7.d
	cmpeq	p1.d, p2/z, z5.d, #0
	eor	z5.d, z4.d, z17.d
	sel	z3.d, p0, z3.d, z6.d
	sel	z4.d, p1, z4.d, z5.d
	st1b	{ z3.b }, p5, [x19, x9]
	incb	x9, all, mul #2
	str	z4, [x12, #1, mul vl]
	cbnz	x13, .LBB5_7
// %bb.8:                               //   in Loop: Header=BB5_5 Depth=2
	subs	x9, x1, #156
	b.eq	.LBB5_11
// %bb.9:                               //   in Loop: Header=BB5_5 Depth=2
	ldr	p0, [x29, #-1, mul vl]          // 2-byte Folded Reload
	add	x12, x19, x1, lsl #3
	mov	x11, x12
	lastb	x10, p0, z2.d
.LBB5_10:                               //   Parent Loop BB5_3 Depth=1
                                        //     Parent Loop BB5_5 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	and	x13, x10, #0xffffffff80000000
	ldr	x10, [x11, #8]!
	ldr	x15, [x12, #1248]
	adds	x9, x9, #1
	and	x14, x10, #0x7ffffffe
	orr	x13, x14, x13
	sbfx	x14, x10, #0, #1
	eor	x13, x15, x13, lsr #1
	and	x14, x14, x25
	eor	x13, x13, x14
	str	x13, [x12]
	mov	x12, x11
	b.lo	.LBB5_10
.LBB5_11:                               //   in Loop: Header=BB5_5 Depth=2
	mul	x10, x17, x18
	ldr	x11, [sp, #1400]
	mov	x9, xzr
	mov	x12, x0
	mov	z2.d, p7/m, x11
	and	x10, x10, x2
	neg	x11, x10
.LBB5_12:                               //   Parent Loop BB5_3 Depth=1
                                        //     Parent Loop BB5_5 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	mov	z0.d, z2.d
	add	x13, x19, x9
	add	x14, x19, x12, lsl #3
	ld1d	{ z1.d }, p2/z, [x13, x20, lsl #3]
	incw	x12
	ld1d	{ z2.d }, p2/z, [x14, x20, lsl #3]
	splice	z3.d, p4, { z0.d, z1.d }
	mov	z5.d, z1.d
	mov	z7.d, z1.d
	add	x15, x11, x12
	splice	z4.d, p4, { z1.d, z2.d }
	mov	z6.d, z2.d
	and	z5.d, z5.d, #0x7ffffffe
	and	z3.d, z3.d, #0xffffffff80000000
	mov	z16.d, z2.d
	and	z7.d, z7.d, #0x1
	decd	x15
	and	z6.d, z6.d, #0x7ffffffe
	and	z4.d, z4.d, #0xffffffff80000000
	orr	z3.d, z5.d, z3.d
	ld1b	{ z5.b }, p5/z, [x19, x9]
	and	z16.d, z16.d, #0x1
	cmpeq	p0.d, p2/z, z7.d, #0
	incb	x9, all, mul #2
	orr	z4.d, z6.d, z4.d
	lsr	z3.d, z3.d, #1
	ldr	z6, [x13, #1, mul vl]
	cmpeq	p1.d, p2/z, z16.d, #0
	lsr	z4.d, z4.d, #1
	eor	z3.d, z3.d, z5.d
	eor	z4.d, z4.d, z6.d
	eor	z5.d, z3.d, z17.d
	eor	z6.d, z4.d, z17.d
	sel	z3.d, p0, z3.d, z5.d
	sel	z4.d, p1, z4.d, z6.d
	st1d	{ z3.d }, p2, [x13, x28, lsl #3]
	st1d	{ z4.d }, p2, [x14, x28, lsl #3]
	cbnz	x15, .LBB5_12
// %bb.13:                              //   in Loop: Header=BB5_5 Depth=2
	lastb	x9, p6, z2.d
	lsl	x10, x10, #3
.LBB5_14:                               //   Parent Loop BB5_3 Depth=1
                                        //     Parent Loop BB5_5 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	add	x11, x19, x10
	and	x12, x9, #0xffffffff80000000
	add	x10, x10, #8
	ldr	x9, [x11, #1256]
	ldr	x14, [x11]
	cmp	x10, #1240
	and	x13, x9, #0x7ffffffe
	orr	x12, x13, x12
	sbfx	x13, x9, #0, #1
	eor	x12, x14, x12, lsr #1
	and	x13, x13, x25
	eor	x12, x12, x13
	str	x12, [x11, #1248]
	b.ne	.LBB5_14
// %bb.15:                              //   in Loop: Header=BB5_5 Depth=2
	ldr	x9, [sp, #2640]
	ldr	x13, [sp, #152]
	mov	x23, xzr
	ldr	x10, [sp, #1392]
	and	x9, x9, #0xffffffff80000000
	and	x11, x13, #0x7ffffffe
	orr	x9, x11, x9
	sbfx	x11, x13, #0, #1
	eor	x9, x10, x9, lsr #1
	and	x10, x11, x25
	eor	x9, x9, x10
	str	x9, [sp, #2640]
	b	.LBB5_4
.LBB5_16:                               //   in Loop: Header=BB5_3 Depth=1
	mov	x22, xzr
	mov	x21, xzr
	str	x13, [sp, #40]                  // 8-byte Folded Spill
	str	w16, [sp, #60]                  // 4-byte Folded Spill
.LBB5_17:                               //   Parent Loop BB5_3 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	add	x8, sp, #64
	mov	x0, x24
	mov	w2, #7                          // =0x7
	ldrb	w26, [x8, x22]
	ands	w1, w26, #0xff
	bl	memchr
	ands	w8, w26, #0xff
	mov	w9, #1                          // =0x1
	mov	w10, #255                       // =0xff
	ccmp	x0, #0, #4, ne
	lsl	x9, x9, x22
	add	x22, x22, #1
	ccmp	w8, w10, #4, eq
	add	x26, sp, #64
	csel	x8, x9, xzr, eq
	cmp	x22, #64
	orr	x21, x8, x21
	b.ne	.LBB5_17
// %bb.18:                              //   in Loop: Header=BB5_3 Depth=1
	add	x0, sp, #64
	bl	_Z12classify_asmPKh
	mov	x22, x0
	add	x0, sp, #64
	bl	_Z15classify_bridgePKh
	str	x0, [sp, #48]                   // 8-byte Folded Spill
	add	x0, sp, #64
	bl	_Z13classify_bextPKh
	mov	x24, x0
	add	x0, sp, #64
	bl	_Z15classify_memcpyPKh
	ldr	x4, [sp, #48]                   // 8-byte Folded Reload
	cmp	x22, x21
	mov	x6, x0
	b.ne	.LBB5_24
// %bb.19:                              //   in Loop: Header=BB5_3 Depth=1
	cmp	x4, x21
	b.ne	.LBB5_24
// %bb.20:                              //   in Loop: Header=BB5_3 Depth=1
	cmp	x24, x21
	b.ne	.LBB5_24
// %bb.21:                              //   in Loop: Header=BB5_3 Depth=1
	cmp	x6, x21
	b.ne	.LBB5_24
// %bb.22:                              //   in Loop: Header=BB5_3 Depth=1
	ldr	w16, [sp, #60]                  // 4-byte Folded Reload
	mov	x5, #44565                      // =0xae15
	mov	w8, #3392                       // =0xd40
	ldp	x1, x17, [sp, #24]              // 16-byte Folded Reload
	ldr	p3, [x29, #-2, mul vl]          // 2-byte Folded Reload
	ldr	p4, [x29, #-3, mul vl]          // 2-byte Folded Reload
	ldr	z17, [x29, #-2, mul vl]         // 16-byte Folded Reload
	mov	x3, #3987079168                 // =0xeda60000
	ldr	p6, [x29, #-17, mul vl]         // 2-byte Folded Reload
	movk	x5, #57671, lsl #16
	add	w16, w16, #1
	ldr	p7, [x29, #-18, mul vl]         // 2-byte Folded Reload
	movk	w8, #3, lsl #16
	ptrue	p2.d
	ptrue	p5.b
	movk	x3, #32767, lsl #32
	mov	x4, #262645840084992            // =0xeee000000000
	movk	x5, #5242, lsl #32
	ldr	x13, [sp, #40]                  // 8-byte Folded Reload
	cmp	w16, w8
	mov	w18, #252                       // =0xfc
	cntd	x0
	mov	w2, #152                        // =0x98
	movk	x3, #29142, lsl #48
	movk	x4, #65527, lsl #48
	movk	x5, #18350, lsl #48
	mov	w6, #25                         // =0x19
	adrp	x7, .L__const.main.alphabet
	add	x7, x7, :lo12:.L__const.main.alphabet
	adrp	x24, .L.str
	add	x24, x24, :lo12:.L.str
	b.ne	.LBB5_3
// %bb.23:
	adrp	x0, .Lstr
	add	x0, x0, :lo12:.Lstr
	bl	puts
	mov	w0, wzr
	b	.LBB5_25
.LBB5_24:
	ldr	w1, [sp, #60]                   // 4-byte Folded Reload
	adrp	x0, .L.str.2
	add	x0, x0, :lo12:.L.str.2
	mov	x2, x21
	mov	x3, x22
	mov	x5, x24
	bl	printf
	mov	w0, #1                          // =0x1
.LBB5_25:
	mov	sp, x29
	.cfi_def_cfa wsp, 96
	ldp	x20, x19, [sp, #80]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #64]             // 16-byte Folded Reload
	ldp	x24, x23, [sp, #48]             // 16-byte Folded Reload
	ldp	x26, x25, [sp, #32]             // 16-byte Folded Reload
	ldp	x28, x27, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #96             // 16-byte Folded Reload
	.cfi_def_cfa_offset 0
	.cfi_restore w19
	.cfi_restore w20
	.cfi_restore w21
	.cfi_restore w22
	.cfi_restore w23
	.cfi_restore w24
	.cfi_restore w25
	.cfi_restore w26
	.cfi_restore w27
	.cfi_restore w28
	.cfi_restore w30
	.cfi_restore w29
	ret
.Lfunc_end5:
	.size	main, .Lfunc_end5-main
	.cfi_endproc
                                        // -- End function
	.type	.L.str,@object                  // @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	",:[]{}"
	.size	.L.str, 7

	.type	.L.str.1,@object                // @.str.1
.L.str.1:
	.asciz	"SVE vector length: %u bits\n"
	.size	.L.str.1, 28

	.type	.L__const.main.alphabet,@object // @__const.main.alphabet
.L__const.main.alphabet:
	.asciz	"abc ,:[]{}\"\\\n0123456789\377\200"
	.size	.L__const.main.alphabet, 26

	.type	.L.str.2,@object                // @.str.2
.L.str.2:
	.asciz	"MISMATCH iter %d: ref %016llx asm %016llx bridge %016llx bext %016llx memcpy %016llx\n"
	.size	.L.str.2, 86

	.type	.Lstr,@object                   // @str
	.section	.rodata.str1.4,"aMS",@progbits,1
	.p2align	2, 0x0
.Lstr:
	.asciz	"all four agree with the scalar reference"
	.size	.Lstr, 41

	.ident	"Ubuntu clang version 21.1.8 (6ubuntu1)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
