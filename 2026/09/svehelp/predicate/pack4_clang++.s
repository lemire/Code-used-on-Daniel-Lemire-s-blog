	.file	"pack4.cpp"
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4, 0x0                          // -- Begin function _Z8classifyPKh
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
	.globl	_Z8classifyPKh
	.p2align	2
	.type	_Z8classifyPKh,@function
_Z8classifyPKh:                         // @_Z8classifyPKh
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	adrp	x8, .LCPI0_0
	ldp	q1, q2, [x0]
	ptrue	p0.b, vl16
	ldr	q0, [x8, :lo12:.LCPI0_0]
	add	x8, sp, #8
	add	x10, sp, #8
	orr	x9, x8, #0x2
	match	p1.b, p0/z, z1.b, z0.b
	match	p2.b, p0/z, z2.b, z0.b
	ldp	q1, q2, [x0, #32]
	match	p3.b, p0/z, z1.b, z0.b
	match	p0.b, p0/z, z2.b, z0.b
	str	p1, [x10]
	orr	x10, x8, #0x4
	orr	x8, x8, #0x6
	str	p2, [x9]
	str	p3, [x10]
	str	p0, [x8]
	ldr	x0, [sp, #8]
	add	sp, sp, #16
	.cfi_def_cfa_offset 0
	ret
.Lfunc_end0:
	.size	_Z8classifyPKh, .Lfunc_end0-_Z8classifyPKh
	.cfi_endproc
                                        // -- End function
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4, 0x0                          // -- Begin function _Z12classify_allPKhm
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
	.text
	.globl	_Z12classify_allPKhm
	.p2align	2
	.type	_Z12classify_allPKhm,@function
_Z12classify_allPKhm:                   // @_Z12classify_allPKhm
	.cfi_startproc
// %bb.0:
	cmp	x1, #64
	b.hs	.LBB1_2
// %bb.1:
	mov	x0, xzr
	ret
.LBB1_2:
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	add	x10, sp, #8
	adrp	x11, .LCPI1_0
	ptrue	p0.b, vl16
	orr	x10, x10, #0x2
	ldr	q0, [x11, :lo12:.LCPI1_0]
	mov	x9, xzr
	mov	x8, xzr
	add	x11, x10, #2
	add	x12, x10, #4
.LBB1_3:                                // =>This Inner Loop Header: Depth=1
	add	x13, x0, x9
	add	x15, x9, #128
	add	x9, x9, #64
	ldp	q1, q2, [x13]
	match	p1.b, p0/z, z1.b, z0.b
	match	p2.b, p0/z, z2.b, z0.b
	ldp	q1, q2, [x13, #32]
	add	x13, sp, #8
	match	p3.b, p0/z, z1.b, z0.b
	match	p4.b, p0/z, z2.b, z0.b
	cmp	x15, x1
	str	p1, [x13]
	lsl	x13, x8, #5
	str	p2, [x10]
	sub	x8, x13, x8
	str	p3, [x11]
	str	p4, [x12]
	ldr	x14, [sp, #8]
	eor	x8, x14, x8
	b.ls	.LBB1_3
// %bb.4:
	add	sp, sp, #16
	.cfi_def_cfa_offset 0
	mov	x0, x8
	ret
.Lfunc_end1:
	.size	_Z12classify_allPKhm, .Lfunc_end1-_Z12classify_allPKhm
	.cfi_endproc
                                        // -- End function
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4, 0x0                          // -- Begin function _Z15classify_paddedPKh
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
	.text
	.globl	_Z15classify_paddedPKh
	.p2align	2
	.type	_Z15classify_paddedPKh,@function
_Z15classify_paddedPKh:                 // @_Z15classify_paddedPKh
	.cfi_startproc
// %bb.0:
	sub	sp, sp, #48
	.cfi_def_cfa_offset 48
	adrp	x8, .LCPI2_0
	ldp	q1, q2, [x0]
	ptrue	p0.b, vl16
	ldr	q0, [x8, :lo12:.LCPI2_0]
	add	x8, sp, #8
	add	x10, sp, #8
	orr	x9, x8, #0x2
	match	p1.b, p0/z, z1.b, z0.b
	match	p2.b, p0/z, z2.b, z0.b
	ldp	q1, q2, [x0, #32]
	match	p3.b, p0/z, z1.b, z0.b
	match	p0.b, p0/z, z2.b, z0.b
	str	p1, [x10]
	orr	x10, x8, #0x4
	orr	x8, x8, #0x6
	str	p2, [x9]
	str	p3, [x10]
	str	p0, [x8]
	ldr	x0, [sp, #8]
	add	sp, sp, #48
	.cfi_def_cfa_offset 0
	ret
.Lfunc_end2:
	.size	_Z15classify_paddedPKh, .Lfunc_end2-_Z15classify_paddedPKh
	.cfi_endproc
                                        // -- End function
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4, 0x0                          // -- Begin function _Z19classify_padded_allPKhm
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
	.globl	_Z19classify_padded_allPKhm
	.p2align	2
	.type	_Z19classify_padded_allPKhm,@function
_Z19classify_padded_allPKhm:            // @_Z19classify_padded_allPKhm
	.cfi_startproc
// %bb.0:
	cmp	x1, #64
	b.hs	.LBB3_2
// %bb.1:
	mov	x0, xzr
	ret
.LBB3_2:
	sub	sp, sp, #48
	.cfi_def_cfa_offset 48
	add	x10, sp, #8
	adrp	x11, .LCPI3_0
	ptrue	p0.b, vl16
	orr	x10, x10, #0x2
	ldr	q0, [x11, :lo12:.LCPI3_0]
	mov	x9, xzr
	mov	x8, xzr
	add	x11, x10, #2
	add	x12, x10, #4
.LBB3_3:                                // =>This Inner Loop Header: Depth=1
	add	x13, x0, x9
	add	x15, x9, #128
	add	x9, x9, #64
	ldp	q1, q2, [x13]
	match	p1.b, p0/z, z1.b, z0.b
	match	p2.b, p0/z, z2.b, z0.b
	ldp	q1, q2, [x13, #32]
	add	x13, sp, #8
	match	p3.b, p0/z, z1.b, z0.b
	match	p4.b, p0/z, z2.b, z0.b
	cmp	x15, x1
	str	p1, [x13]
	lsl	x13, x8, #5
	str	p2, [x10]
	sub	x8, x13, x8
	str	p3, [x11]
	str	p4, [x12]
	ldr	x14, [sp, #8]
	eor	x8, x14, x8
	b.ls	.LBB3_3
// %bb.4:
	add	sp, sp, #48
	.cfi_def_cfa_offset 0
	mov	x0, x8
	ret
.Lfunc_end3:
	.size	_Z19classify_padded_allPKhm, .Lfunc_end3-_Z19classify_padded_allPKhm
	.cfi_endproc
                                        // -- End function
	.globl	_Z14overflow_probeu10__SVBool_tS_S_S_ // -- Begin function _Z14overflow_probeu10__SVBool_tS_S_S_
	.p2align	2
	.type	_Z14overflow_probeu10__SVBool_tS_S_S_,@function
	.variant_pcs	_Z14overflow_probeu10__SVBool_tS_S_S_
_Z14overflow_probeu10__SVBool_tS_S_S_:  // @_Z14overflow_probeu10__SVBool_tS_S_S_
	.cfi_startproc
// %bb.0:
	mov	w0, wzr
	ret
.Lfunc_end4:
	.size	_Z14overflow_probeu10__SVBool_tS_S_S_, .Lfunc_end4-_Z14overflow_probeu10__SVBool_tS_S_S_
	.cfi_endproc
                                        // -- End function
	.globl	main                            // -- Begin function main
	.p2align	2
	.type	main,@function
main:                                   // @main
	.cfi_startproc
// %bb.0:
	stp	x29, x30, [sp, #-80]!           // 16-byte Folded Spill
	.cfi_def_cfa_offset 80
	str	x28, [sp, #16]                  // 8-byte Folded Spill
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
	.cfi_offset w28, -64
	.cfi_offset w30, -72
	.cfi_offset w29, -80
	sub	x9, sp, #368
	and	sp, x9, #0xffffffffffffffc0
	rdvl	x1, #8
	cntd	x2
	adrp	x0, .L.str
	add	x0, x0, :lo12:.L.str
                                        // kill: def $w1 killed $w1 killed $x1
                                        // kill: def $w2 killed $w2 killed $x2
	bl	printf
	mov	x8, xzr
	adrp	x9, .L.str.1
	add	x9, x9, :lo12:.L.str.1
	add	x10, sp, #64
.LBB5_1:                                // =>This Inner Loop Header: Depth=1
	and	x11, x8, #0x1f
	ldrb	w11, [x9, x11]
	strb	w11, [x10, x8]
	add	x8, x8, #1
	cmp	x8, #256
	b.ne	.LBB5_1
// %bb.2:
	movi	v0.2d, #0000000000000000
	mov	x21, xzr
	adrp	x19, .L.str.2
	add	x19, x19, :lo12:.L.str.2
	add	x22, sp, #64
	mov	w23, #1                         // =0x1
	add	x24, sp, #32
	stp	q0, q0, [sp, #32]
	b	.LBB5_4
.LBB5_3:                                //   in Loop: Header=BB5_4 Depth=1
	add	x21, x21, #1
	cmp	x21, #256
	b.eq	.LBB5_7
.LBB5_4:                                // =>This Inner Loop Header: Depth=1
	ldrb	w20, [x22, x21]
	mov	x0, x19
	mov	w2, #7                          // =0x7
	mov	w1, w20
	bl	memchr
	cbz	x0, .LBB5_3
// %bb.5:                               //   in Loop: Header=BB5_4 Depth=1
	cbz	w20, .LBB5_3
// %bb.6:                               //   in Loop: Header=BB5_4 Depth=1
	lsr	w8, w21, #6
	lsl	x9, x23, x21
	ldr	x10, [x24, x8, lsl #3]
	orr	x9, x10, x9
	str	x9, [x24, x8, lsl #3]
	b	.LBB5_3
.LBB5_7:
	ldp	x20, x8, [sp, #32]
	add	x0, sp, #64
	lsl	x9, x20, #5
	sub	x9, x9, x20
	eor	x8, x8, x9
	lsl	x9, x8, #5
	sub	x8, x9, x8
	ldp	x10, x9, [sp, #48]
	eor	x8, x10, x8
	lsl	x10, x8, #5
	sub	x8, x10, x8
	eor	x21, x9, x8
	bl	_Z8classifyPKh
	adrp	x19, .L.str.5
	add	x19, x19, :lo12:.L.str.5
	cmp	x0, x20
	adrp	x22, .L.str.4
	add	x22, x22, :lo12:.L.str.4
	b.ne	.LBB5_9
// %bb.8:
	add	x0, sp, #64
	mov	w1, #256                        // =0x100
	bl	_Z12classify_allPKhm
	cmp	x0, x21
	csel	x19, x22, x19, eq
.LBB5_9:
	adrp	x0, .L.str.3
	add	x0, x0, :lo12:.L.str.3
	mov	x1, x19
	bl	printf
	add	x0, sp, #64
	bl	_Z15classify_paddedPKh
	cmp	x0, x20
	adrp	x19, .L.str.5
	add	x19, x19, :lo12:.L.str.5
	b.ne	.LBB5_11
// %bb.10:
	add	x0, sp, #64
	mov	w1, #256                        // =0x100
	bl	_Z19classify_padded_allPKhm
	cmp	x0, x21
	csel	x19, x22, x19, eq
.LBB5_11:
	adrp	x0, .L.str.6
	add	x0, x0, :lo12:.L.str.6
	mov	x1, x19
	bl	printf
	bl	_Z14overflow_probeu10__SVBool_tS_S_S_
	mov	w1, w0
	adrp	x0, .L.str.7
	add	x0, x0, :lo12:.L.str.7
	bl	printf
	mov	w0, wzr
	mov	sp, x29
	.cfi_def_cfa wsp, 80
	ldp	x20, x19, [sp, #64]             // 16-byte Folded Reload
	ldr	x28, [sp, #16]                  // 8-byte Folded Reload
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
	.asciz	"running at VL %u bits (predicate = %u bytes)\n"
	.size	.L.str, 46

	.type	.L.str.1,@object                // @.str.1
.L.str.1:
	.asciz	"ab,c:d[e]f{g}h  x,y:z[w]v{u}t \"\\"
	.size	.L.str.1, 33

	.type	.L.str.2,@object                // @.str.2
.L.str.2:
	.asciz	",:[]{}"
	.size	.L.str.2, 7

	.type	.L.str.3,@object                // @.str.3
.L.str.3:
	.asciz	"classify:        %s\n"
	.size	.L.str.3, 21

	.type	.L.str.4,@object                // @.str.4
.L.str.4:
	.asciz	"OK"
	.size	.L.str.4, 3

	.type	.L.str.5,@object                // @.str.5
.L.str.5:
	.asciz	"WRONG"
	.size	.L.str.5, 6

	.type	.L.str.6,@object                // @.str.6
.L.str.6:
	.asciz	"classify_padded: %s\n"
	.size	.L.str.6, 21

	.type	.L.str.7,@object                // @.str.7
.L.str.7:
	.asciz	"overflow past w[3]: %d uint16 slots\n"
	.size	.L.str.7, 37

	.ident	"Ubuntu clang version 21.1.8 (6ubuntu1)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
