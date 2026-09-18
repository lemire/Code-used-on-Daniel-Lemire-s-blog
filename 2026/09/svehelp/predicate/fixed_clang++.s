	.file	"fixedpred.cpp"
	.text
	.globl	_Z15mask16_noinlineu10__SVBool_t // -- Begin function _Z15mask16_noinlineu10__SVBool_t
	.p2align	2
	.type	_Z15mask16_noinlineu10__SVBool_t,@function
	.variant_pcs	_Z15mask16_noinlineu10__SVBool_t
_Z15mask16_noinlineu10__SVBool_t:       // @_Z15mask16_noinlineu10__SVBool_t
	.cfi_startproc
// %bb.0:
	str	x30, [sp, #-16]!                // 8-byte Folded Spill
	.cfi_def_cfa_offset 16
	.cfi_offset w30, -16
	addvl	sp, sp, #-1
	.cfi_escape 0x0f, 0x0c, 0x8f, 0x00, 0x11, 0x10, 0x22, 0x11, 0x08, 0x92, 0x2e, 0x00, 0x1e, 0x22 // sp + 16 + 8 * VG
	ptrue	p1.d
	str	p0, [sp, #7, mul vl]
	addvl	x8, sp, #1
	ld1b	{ z0.d }, p1/z, [sp, #7, mul vl]
	mov	z1.s, z0.s[2]
	str	b0, [x8, #12]
	addvl	x8, sp, #1
	stur	b1, [x8, #13]
	addvl	x8, sp, #1
	ldrh	w0, [x8, #12]
	addvl	sp, sp, #1
	.cfi_def_cfa wsp, 16
	ldr	x30, [sp], #16                  // 8-byte Folded Reload
	.cfi_def_cfa_offset 0
	.cfi_restore w30
	ret
.Lfunc_end0:
	.size	_Z15mask16_noinlineu10__SVBool_t, .Lfunc_end0-_Z15mask16_noinlineu10__SVBool_t
	.cfi_endproc
                                        // -- End function
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4, 0x0                          // -- Begin function _Z8classifyPKh
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
	.globl	_Z8classifyPKh
	.p2align	2
	.type	_Z8classifyPKh,@function
_Z8classifyPKh:                         // @_Z8classifyPKh
	.cfi_startproc
// %bb.0:
	str	x30, [sp, #-16]!                // 8-byte Folded Spill
	.cfi_def_cfa_offset 16
	.cfi_offset w30, -16
	sub	sp, sp, #16
	.cfi_def_cfa_offset 32
	addvl	sp, sp, #-1
	.cfi_escape 0x0f, 0x0c, 0x8f, 0x00, 0x11, 0x20, 0x22, 0x11, 0x08, 0x92, 0x2e, 0x00, 0x1e, 0x22 // sp + 32 + 8 * VG
	adrp	x8, .LCPI1_0
	ldr	q0, [x0]
	ptrue	p0.b, vl16
	ldr	q1, [x8, :lo12:.LCPI1_0]
	ptrue	p1.d
	add	x8, sp, #16
	add	x9, sp, #16
	add	x10, sp, #16
	add	x11, sp, #16
	match	p2.b, p0/z, z0.b, z1.b
	str	p2, [x8, #7, mul vl]
	ld1b	{ z0.d }, p1/z, [x8, #7, mul vl]
	addvl	x8, sp, #1
	mov	z2.s, z0.s[2]
	str	b0, [x8, #28]
	addvl	x8, sp, #1
	stur	b2, [x8, #29]
	addvl	x8, sp, #1
	ldr	q0, [x0, #16]
	ldrh	w8, [x8, #28]
	match	p2.b, p0/z, z0.b, z1.b
	str	p2, [x9, #6, mul vl]
	ld1b	{ z0.d }, p1/z, [x9, #6, mul vl]
	addvl	x9, sp, #1
	mov	z2.s, z0.s[2]
	str	b0, [x9, #24]
	addvl	x9, sp, #1
	stur	b2, [x9, #25]
	addvl	x9, sp, #1
	ldr	q0, [x0, #32]
	ldrh	w9, [x9, #24]
	match	p2.b, p0/z, z0.b, z1.b
	orr	x8, x8, x9, lsl #16
	str	p2, [x10, #5, mul vl]
	ld1b	{ z0.d }, p1/z, [x10, #5, mul vl]
	mov	z2.s, z0.s[2]
	str	b0, [sp, #12]
	stur	b2, [sp, #13]
	ldr	q0, [x0, #48]
	ldrh	w10, [sp, #12]
	match	p0.b, p0/z, z0.b, z1.b
	orr	x8, x8, x10, lsl #32
	str	p0, [x11, #4, mul vl]
	ld1b	{ z0.d }, p1/z, [x11, #4, mul vl]
	mov	z1.s, z0.s[2]
	str	b0, [sp, #8]
	stur	b1, [sp, #9]
	ldrh	w9, [sp, #8]
	orr	x0, x8, x9, lsl #48
	addvl	sp, sp, #1
	.cfi_def_cfa wsp, 32
	add	sp, sp, #16
	.cfi_def_cfa_offset 16
	ldr	x30, [sp], #16                  // 8-byte Folded Reload
	.cfi_def_cfa_offset 0
	.cfi_restore w30
	ret
.Lfunc_end1:
	.size	_Z8classifyPKh, .Lfunc_end1-_Z8classifyPKh
	.cfi_endproc
                                        // -- End function
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4, 0x0                          // -- Begin function _Z12classify_allPKhm
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
	.globl	_Z12classify_allPKhm
	.p2align	2
	.type	_Z12classify_allPKhm,@function
_Z12classify_allPKhm:                   // @_Z12classify_allPKhm
	.cfi_startproc
// %bb.0:
	cmp	x1, #64
	b.hs	.LBB2_2
// %bb.1:
	mov	x0, xzr
	ret
.LBB2_2:
	str	x30, [sp, #-16]!                // 8-byte Folded Spill
	.cfi_def_cfa_offset 16
	.cfi_offset w30, -16
	sub	sp, sp, #16
	.cfi_def_cfa_offset 32
	addvl	sp, sp, #-1
	.cfi_escape 0x0f, 0x0c, 0x8f, 0x00, 0x11, 0x20, 0x22, 0x11, 0x08, 0x92, 0x2e, 0x00, 0x1e, 0x22 // sp + 32 + 8 * VG
	adrp	x8, .LCPI2_0
	ptrue	p0.b, vl16
	mov	x9, xzr
	ldr	q0, [x8, :lo12:.LCPI2_0]
	ptrue	p1.d
	mov	x8, xzr
.LBB2_3:                                // =>This Inner Loop Header: Depth=1
	add	x10, x0, x9
	add	x11, sp, #16
	add	x12, sp, #16
	ldr	q1, [x10]
	add	x13, sp, #16
	match	p2.b, p0/z, z1.b, z0.b
	str	p2, [x11, #7, mul vl]
	ld1b	{ z1.d }, p1/z, [x11, #7, mul vl]
	addvl	x11, sp, #1
	mov	z2.s, z1.s[2]
	str	b1, [x11, #28]
	addvl	x11, sp, #1
	stur	b2, [x11, #29]
	addvl	x11, sp, #1
	ldr	q1, [x10, #16]
	ldrh	w11, [x11, #28]
	match	p2.b, p0/z, z1.b, z0.b
	str	p2, [x12, #6, mul vl]
	ld1b	{ z1.d }, p1/z, [x12, #6, mul vl]
	addvl	x12, sp, #1
	mov	z2.s, z1.s[2]
	str	b1, [x12, #24]
	addvl	x12, sp, #1
	stur	b2, [x12, #25]
	addvl	x12, sp, #1
	ldr	q1, [x10, #32]
	ldrh	w12, [x12, #24]
	match	p2.b, p0/z, z1.b, z0.b
	orr	x11, x11, x12, lsl #16
	str	p2, [x13, #5, mul vl]
	ld1b	{ z1.d }, p1/z, [x13, #5, mul vl]
	mov	z2.s, z1.s[2]
	str	b1, [sp, #12]
	stur	b2, [sp, #13]
	ldr	q1, [x10, #48]
	ldrh	w10, [sp, #12]
	match	p2.b, p0/z, z1.b, z0.b
	orr	x10, x11, x10, lsl #32
	add	x11, x9, #128
	add	x9, x9, #64
	cmp	x11, x1
	str	p2, [x13, #4, mul vl]
	ld1b	{ z1.d }, p1/z, [x13, #4, mul vl]
	mov	z2.s, z1.s[2]
	str	b1, [sp, #8]
	stur	b2, [sp, #9]
	ldrh	w12, [sp, #8]
	orr	x10, x10, x12, lsl #48
	eor	x8, x10, x8
	b.ls	.LBB2_3
// %bb.4:
	addvl	sp, sp, #1
	.cfi_def_cfa wsp, 32
	add	sp, sp, #16
	.cfi_def_cfa_offset 16
	ldr	x30, [sp], #16                  // 8-byte Folded Reload
	.cfi_def_cfa_offset 0
	.cfi_restore w30
	mov	x0, x8
	ret
.Lfunc_end2:
	.size	_Z12classify_allPKhm, .Lfunc_end2-_Z12classify_allPKhm
	.cfi_endproc
                                        // -- End function
	.globl	main                            // -- Begin function main
	.p2align	2
	.type	main,@function
main:                                   // @main
	.cfi_startproc
// %bb.0:
	stp	x29, x30, [sp, #-64]!           // 16-byte Folded Spill
	.cfi_def_cfa_offset 64
	stp	x24, x23, [sp, #16]             // 16-byte Folded Spill
	stp	x22, x21, [sp, #32]             // 16-byte Folded Spill
	stp	x20, x19, [sp, #48]             // 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 64
	.cfi_offset w19, -8
	.cfi_offset w20, -16
	.cfi_offset w21, -24
	.cfi_offset w22, -32
	.cfi_offset w23, -40
	.cfi_offset w24, -48
	.cfi_offset w30, -56
	.cfi_offset w29, -64
	sub	x9, sp, #128
	and	sp, x9, #0xffffffffffffffc0
	adrp	x0, .L.str
	add	x0, x0, :lo12:.L.str
	mov	w1, #128                        // =0x80
	mov	w2, #128                        // =0x80
	mov	w3, #2                          // =0x2
	bl	printf
	mov	x8, xzr
	adrp	x9, .L.str.1
	add	x9, x9, :lo12:.L.str.1
	mov	x10, sp
.LBB3_1:                                // =>This Inner Loop Header: Depth=1
	and	x11, x8, #0xf
	ldrb	w11, [x9, x11]
	strb	w11, [x10, x8]
	add	x8, x8, #1
	cmp	x8, #128
	b.ne	.LBB3_1
// %bb.2:
	mov	x21, xzr
	mov	x19, xzr
	mov	x22, sp
	adrp	x20, .L.str.2
	add	x20, x20, :lo12:.L.str.2
	mov	w23, #1                         // =0x1
.LBB3_3:                                // =>This Inner Loop Header: Depth=1
	ldrb	w24, [x22, x21]
	mov	x0, x20
	mov	w2, #7                          // =0x7
	ands	w1, w24, #0xff
	bl	memchr
	tst	w24, #0xff
	lsl	x8, x23, x21
	add	x21, x21, #1
	ccmp	x0, #0, #4, ne
	csel	x8, xzr, x8, eq
	cmp	x21, #64
	orr	x19, x8, x19
	b.ne	.LBB3_3
// %bb.4:
	mov	x0, sp
	bl	_Z8classifyPKh
	mov	x20, x0
	mov	x0, sp
	mov	w1, #128                        // =0x80
	bl	_Z12classify_allPKhm
	adrp	x22, .L.str.5
	add	x22, x22, :lo12:.L.str.5
	cmp	x20, x19
	adrp	x23, .L.str.4
	add	x23, x23, :lo12:.L.str.4
	mov	x21, x0
	csel	x3, x23, x22, eq
	adrp	x0, .L.str.3
	add	x0, x0, :lo12:.L.str.3
	mov	x1, x20
	mov	x2, x19
	bl	printf
	cmp	x21, #0
	adrp	x0, .L.str.6
	add	x0, x0, :lo12:.L.str.6
	csel	x2, x23, x22, eq
	mov	x1, x21
	bl	printf
	mov	w0, wzr
	mov	sp, x29
	.cfi_def_cfa wsp, 64
	ldp	x20, x19, [sp, #48]             // 16-byte Folded Reload
	ldp	x22, x21, [sp, #32]             // 16-byte Folded Reload
	ldp	x24, x23, [sp, #16]             // 16-byte Folded Reload
	ldp	x29, x30, [sp], #64             // 16-byte Folded Reload
	.cfi_def_cfa_offset 0
	.cfi_restore w19
	.cfi_restore w20
	.cfi_restore w21
	.cfi_restore w22
	.cfi_restore w23
	.cfi_restore w24
	.cfi_restore w30
	.cfi_restore w29
	ret
.Lfunc_end3:
	.size	main, .Lfunc_end3-main
	.cfi_endproc
                                        // -- End function
	.type	.L.str,@object                  // @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"compiled for %d-bit SVE, running at %u bits, sizeof(fixed_pred)=%zu\n"
	.size	.L.str, 69

	.type	.L.str.1,@object                // @.str.1
.L.str.1:
	.asciz	"ab,c:d[e]f{g}h  "
	.size	.L.str.1, 17

	.type	.L.str.2,@object                // @.str.2
.L.str.2:
	.asciz	",:[]{}"
	.size	.L.str.2, 7

	.type	.L.str.3,@object                // @.str.3
.L.str.3:
	.asciz	"classify: %016llx expected %016llx %s\n"
	.size	.L.str.3, 39

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
	.asciz	"classify_all (two identical blocks, xor): %016llx expected 0 %s\n"
	.size	.L.str.6, 65

	.ident	"Ubuntu clang version 21.1.8 (6ubuntu1)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
