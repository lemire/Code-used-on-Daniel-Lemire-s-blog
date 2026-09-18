	.arch armv9-a+crc
	.file	"pack4.cpp"
	.text
	.align	2
	.p2align 5,,15
	.global	_Z8classifyPKh
	.type	_Z8classifyPKh, %function
_Z8classifyPKh:
.LFB3952:
	.cfi_startproc
	sub	sp, sp, #32
	.cfi_def_cfa_offset 32
	adrp	x2, .LANCHOR0
	ptrue	p7.b, vl16
	adrp	x1, :got:__stack_chk_guard
	ldr	x1, [x1, :got_lo12:__stack_chk_guard]
	stp	x29, x30, [sp, 16]
	.cfi_offset 29, -16
	.cfi_offset 30, -8
	add	x29, sp, 16
	ldr	x3, [x1]
	str	x3, [sp, 8]
	mov	x3, 0
	add	x1, sp, 2
	ldr	q31, [x2, #:lo12:.LANCHOR0]
	ldp	q29, q30, [x0]
	match	p15.b, p7/z, z29.b, z31.b
	str	p15, [sp]
	match	p15.b, p7/z, z30.b, z31.b
	str	p15, [x1]
	adrp	x1, :got:__stack_chk_guard
	ldr	x1, [x1, :got_lo12:__stack_chk_guard]
	ldp	q29, q30, [x0, 32]
	match	p15.b, p7/z, z29.b, z31.b
	match	p7.b, p7/z, z30.b, z31.b
	add	x0, sp, 4
	str	p15, [x0]
	add	x0, sp, 6
	str	p7, [x0]
	ldr	x0, [sp, 8]
	ldr	x2, [x1]
	subs	x0, x0, x2
	mov	x2, 0
	ldr	x0, [sp]
	bne	.L5
	ldp	x29, x30, [sp, 16]
	add	sp, sp, 32
	.cfi_remember_state
	.cfi_restore 29
	.cfi_restore 30
	.cfi_def_cfa_offset 0
	ret
.L5:
	.cfi_restore_state
	bl	__stack_chk_fail
	.cfi_endproc
.LFE3952:
	.size	_Z8classifyPKh, .-_Z8classifyPKh
	.align	2
	.p2align 5,,15
	.global	_Z12classify_allPKhm
	.type	_Z12classify_allPKhm, %function
_Z12classify_allPKhm:
.LFB3953:
	.cfi_startproc
	sub	sp, sp, #32
	.cfi_def_cfa_offset 32
	adrp	x2, :got:__stack_chk_guard
	ldr	x2, [x2, :got_lo12:__stack_chk_guard]
	stp	x29, x30, [sp, 16]
	.cfi_offset 29, -16
	.cfi_offset 30, -8
	add	x29, sp, 16
	ldr	x3, [x2]
	str	x3, [sp, 8]
	mov	x3, 0
	cmp	x1, 63
	bls	.L10
	adrp	x3, .LANCHOR0
	mov	x2, x0
	ptrue	p7.b, vl16
	mov	x5, 64
	sub	x5, x5, x2
	ldr	q31, [x3, #:lo12:.LANCHOR0]
	mov	x0, 0
	.p2align 5,,15
.L8:
	ldp	q2, q1, [x2]
	match	p15.b, p7/z, z2.b, z31.b
	str	p15, [sp]
	lsl	x3, x0, 5
	match	p15.b, p7/z, z1.b, z31.b
	add	x4, sp, 2
	str	p15, [x4]
	sub	x0, x3, x0
	ldp	q0, q30, [x2, 32]
	match	p15.b, p7/z, z0.b, z31.b
	add	x3, sp, 4
	str	p15, [x3]
	match	p15.b, p7/z, z30.b, z31.b
	add	x3, sp, 6
	str	p15, [x3]
	add	x2, x2, 64
	ldr	x4, [sp]
	add	x3, x5, x2
	eor	x0, x0, x4
	cmp	x1, x3
	bcs	.L8
.L6:
	adrp	x1, :got:__stack_chk_guard
	ldr	x1, [x1, :got_lo12:__stack_chk_guard]
	ldr	x3, [sp, 8]
	ldr	x2, [x1]
	subs	x3, x3, x2
	mov	x2, 0
	bne	.L13
	ldp	x29, x30, [sp, 16]
	add	sp, sp, 32
	.cfi_remember_state
	.cfi_restore 29
	.cfi_restore 30
	.cfi_def_cfa_offset 0
	ret
	.p2align 2,,3
.L10:
	.cfi_restore_state
	mov	x0, 0
	b	.L6
.L13:
	bl	__stack_chk_fail
	.cfi_endproc
.LFE3953:
	.size	_Z12classify_allPKhm, .-_Z12classify_allPKhm
	.align	2
	.p2align 5,,15
	.global	_Z15classify_paddedPKh
	.type	_Z15classify_paddedPKh, %function
_Z15classify_paddedPKh:
.LFB3954:
	.cfi_startproc
	sub	sp, sp, #64
	.cfi_def_cfa_offset 64
	adrp	x2, .LANCHOR0
	ptrue	p7.b, vl16
	adrp	x1, :got:__stack_chk_guard
	ldr	x1, [x1, :got_lo12:__stack_chk_guard]
	stp	x29, x30, [sp, 48]
	.cfi_offset 29, -16
	.cfi_offset 30, -8
	add	x29, sp, 48
	ldr	x3, [x1]
	str	x3, [sp, 40]
	mov	x3, 0
	add	x1, sp, 2
	ldr	q31, [x2, #:lo12:.LANCHOR0]
	ldp	q29, q30, [x0]
	match	p15.b, p7/z, z29.b, z31.b
	str	p15, [sp]
	match	p15.b, p7/z, z30.b, z31.b
	str	p15, [x1]
	adrp	x1, :got:__stack_chk_guard
	ldr	x1, [x1, :got_lo12:__stack_chk_guard]
	ldp	q29, q30, [x0, 32]
	match	p15.b, p7/z, z29.b, z31.b
	match	p7.b, p7/z, z30.b, z31.b
	add	x0, sp, 4
	str	p15, [x0]
	add	x0, sp, 6
	str	p7, [x0]
	ldr	x0, [sp, 40]
	ldr	x2, [x1]
	subs	x0, x0, x2
	mov	x2, 0
	ldr	x0, [sp]
	bne	.L17
	ldp	x29, x30, [sp, 48]
	add	sp, sp, 64
	.cfi_remember_state
	.cfi_restore 29
	.cfi_restore 30
	.cfi_def_cfa_offset 0
	ret
.L17:
	.cfi_restore_state
	bl	__stack_chk_fail
	.cfi_endproc
.LFE3954:
	.size	_Z15classify_paddedPKh, .-_Z15classify_paddedPKh
	.align	2
	.p2align 5,,15
	.global	_Z19classify_padded_allPKhm
	.type	_Z19classify_padded_allPKhm, %function
_Z19classify_padded_allPKhm:
.LFB3955:
	.cfi_startproc
	sub	sp, sp, #64
	.cfi_def_cfa_offset 64
	adrp	x2, :got:__stack_chk_guard
	ldr	x2, [x2, :got_lo12:__stack_chk_guard]
	stp	x29, x30, [sp, 48]
	.cfi_offset 29, -16
	.cfi_offset 30, -8
	add	x29, sp, 48
	ldr	x3, [x2]
	str	x3, [sp, 40]
	mov	x3, 0
	cmp	x1, 63
	bls	.L22
	adrp	x3, .LANCHOR0
	mov	x2, x0
	ptrue	p7.b, vl16
	mov	x5, 64
	sub	x5, x5, x2
	ldr	q31, [x3, #:lo12:.LANCHOR0]
	mov	x0, 0
	.p2align 5,,15
.L20:
	ldp	q2, q1, [x2]
	match	p15.b, p7/z, z2.b, z31.b
	str	p15, [sp]
	lsl	x3, x0, 5
	match	p15.b, p7/z, z1.b, z31.b
	add	x4, sp, 2
	str	p15, [x4]
	sub	x0, x3, x0
	ldp	q0, q30, [x2, 32]
	match	p15.b, p7/z, z0.b, z31.b
	add	x3, sp, 4
	str	p15, [x3]
	match	p15.b, p7/z, z30.b, z31.b
	add	x3, sp, 6
	str	p15, [x3]
	add	x2, x2, 64
	ldr	x4, [sp]
	add	x3, x5, x2
	eor	x0, x0, x4
	cmp	x1, x3
	bcs	.L20
.L18:
	adrp	x1, :got:__stack_chk_guard
	ldr	x1, [x1, :got_lo12:__stack_chk_guard]
	ldr	x3, [sp, 40]
	ldr	x2, [x1]
	subs	x3, x3, x2
	mov	x2, 0
	bne	.L25
	ldp	x29, x30, [sp, 48]
	add	sp, sp, 64
	.cfi_remember_state
	.cfi_restore 29
	.cfi_restore 30
	.cfi_def_cfa_offset 0
	ret
	.p2align 2,,3
.L22:
	.cfi_restore_state
	mov	x0, 0
	b	.L18
.L25:
	bl	__stack_chk_fail
	.cfi_endproc
.LFE3955:
	.size	_Z19classify_padded_allPKhm, .-_Z19classify_padded_allPKhm
	.align	2
	.p2align 5,,15
	.global	_Z14overflow_probeu10__SVBool_tS_S_S_
	.variant_pcs	_Z14overflow_probeu10__SVBool_tS_S_S_
	.type	_Z14overflow_probeu10__SVBool_tS_S_S_, %function
_Z14overflow_probeu10__SVBool_tS_S_S_:
.LFB3956:
	.cfi_startproc
	stp	x29, x30, [sp, -16]!
	.cfi_def_cfa_offset 16
	.cfi_offset 29, -16
	.cfi_offset 30, -8
	movi	v29.2d, #0
	index	z31.s, #0, #1
	mov	x0, 0
	mov	x29, sp
	addvl	sp, sp, #-18
	.cfi_escape 0xf,0xb,0x8f,0,0x92,0x2e,0,0x8,0x90,0x1e,0x23,0x10,0x22
	adrp	x1, :got:__stack_chk_guard
	ldr	x1, [x1, :got_lo12:__stack_chk_guard]
	str	p7, [sp, #3, mul vl]
	str	p5, [sp, #1, mul vl]
	str	p6, [sp, #2, mul vl]
	str	p8, [sp, #4, mul vl]
	str	p9, [sp, #5, mul vl]
	str	p10, [sp, #6, mul vl]
	str	p11, [sp, #7, mul vl]
	str	p12, [sp, #8, mul vl]
	str	p13, [sp, #9, mul vl]
	str	p14, [sp, #10, mul vl]
	str	p15, [sp, #11, mul vl]
	str	z8, [sp, #2, mul vl]
	str	z9, [sp, #3, mul vl]
	str	z10, [sp, #4, mul vl]
	str	z11, [sp, #5, mul vl]
	str	z12, [sp, #6, mul vl]
	str	z13, [sp, #7, mul vl]
	str	z14, [sp, #8, mul vl]
	str	z15, [sp, #9, mul vl]
	str	z16, [sp, #10, mul vl]
	str	z17, [sp, #11, mul vl]
	str	z18, [sp, #12, mul vl]
	str	z19, [sp, #13, mul vl]
	str	z20, [sp, #14, mul vl]
	str	z21, [sp, #15, mul vl]
	str	z22, [sp, #16, mul vl]
	str	p4, [sp]
	str	z23, [sp, #17, mul vl]
	sub	sp, sp, #48
	.cfi_escape 0xf,0xb,0x8f,0,0x92,0x2e,0,0x8,0x90,0x1e,0x23,0x40,0x22
	.cfi_escape 0x10,0x48,0xa,0x8f,0,0x92,0x2e,0,0x40,0x1e,0x23,0x30,0x22
	.cfi_escape 0x10,0x49,0xa,0x8f,0,0x92,0x2e,0,0x48,0x1e,0x23,0x30,0x22
	.cfi_escape 0x10,0x4a,0xb,0x8f,0,0x92,0x2e,0,0x8,0x20,0x1e,0x23,0x30,0x22
	.cfi_escape 0x10,0x4b,0xb,0x8f,0,0x92,0x2e,0,0x8,0x28,0x1e,0x23,0x30,0x22
	.cfi_escape 0x10,0x4c,0xb,0x8f,0,0x92,0x2e,0,0x8,0x30,0x1e,0x23,0x30,0x22
	.cfi_escape 0x10,0x4d,0xb,0x8f,0,0x92,0x2e,0,0x8,0x38,0x1e,0x23,0x30,0x22
	.cfi_escape 0x10,0x4e,0xb,0x8f,0,0x92,0x2e,0,0x8,0x40,0x1e,0x23,0x30,0x22
	.cfi_escape 0x10,0x4f,0xb,0x8f,0,0x92,0x2e,0,0x8,0x48,0x1e,0x23,0x30,0x22
	ldr	x2, [x1]
	str	x2, [sp, 40]
	mov	x2, 0
	add	x3, sp, 2
	add	x2, sp, 8
	mov	w1, 16
	whilelo	p7.s, wzr, w1
	stp	xzr, xzr, [sp]
	stp	xzr, xzr, [sp, 16]
	str	xzr, [sp, 32]
	str	p0, [sp]
	str	p1, [x3]
	add	x3, sp, 4
	str	p2, [x3]
	add	x3, sp, 6
	str	p3, [x3]
.L27:
	ld1h	z0.s, p7/z, [x2, x0, lsl 1]
	cmpne	p6.h, p7/z, z0.h, #0
	movprfx	z30, z31
	add	z30.s, z30.s, #1
	incw	x0
	clastb	s29, p6, s29, z30.s
	whilelo	p7.s, w0, w1
	incw	z31.s
	b.any	.L27
	adrp	x0, :got:__stack_chk_guard
	ldr	x0, [x0, :got_lo12:__stack_chk_guard]
	ldr	x2, [sp, 40]
	ldr	x1, [x0]
	subs	x2, x2, x1
	mov	x1, 0
	bne	.L31
	add	sp, sp, 48
	.cfi_remember_state
	.cfi_escape 0xf,0xb,0x8f,0,0x92,0x2e,0,0x8,0x90,0x1e,0x23,0x10,0x22
	ldr	p4, [sp]
	ldr	z8, [sp, #2, mul vl]
	ldr	z9, [sp, #3, mul vl]
	ldr	z10, [sp, #4, mul vl]
	ldr	z11, [sp, #5, mul vl]
	ldr	z12, [sp, #6, mul vl]
	ldr	z13, [sp, #7, mul vl]
	ldr	z14, [sp, #8, mul vl]
	ldr	z15, [sp, #9, mul vl]
	ldr	z16, [sp, #10, mul vl]
	ldr	z17, [sp, #11, mul vl]
	ldr	z18, [sp, #12, mul vl]
	ldr	z19, [sp, #13, mul vl]
	ldr	z20, [sp, #14, mul vl]
	ldr	z21, [sp, #15, mul vl]
	ldr	z22, [sp, #16, mul vl]
	ldr	z23, [sp, #17, mul vl]
	ldr	p5, [sp, #1, mul vl]
	ldr	p6, [sp, #2, mul vl]
	ldr	p7, [sp, #3, mul vl]
	ldr	p8, [sp, #4, mul vl]
	ldr	p9, [sp, #5, mul vl]
	ldr	p10, [sp, #6, mul vl]
	ldr	p11, [sp, #7, mul vl]
	ldr	p12, [sp, #8, mul vl]
	ldr	p13, [sp, #9, mul vl]
	ldr	p14, [sp, #10, mul vl]
	ldr	p15, [sp, #11, mul vl]
	addvl	sp, sp, #18
	.cfi_def_cfa_offset 16
	fmov	w0, s29
	ldp	x29, x30, [sp], 16
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 79
	.cfi_restore 78
	.cfi_restore 77
	.cfi_restore 76
	.cfi_restore 75
	.cfi_restore 74
	.cfi_restore 73
	.cfi_restore 72
	.cfi_def_cfa_offset 0
	ret
.L31:
	.cfi_restore_state
	bl	__stack_chk_fail
	.cfi_endproc
.LFE3956:
	.size	_Z14overflow_probeu10__SVBool_tS_S_S_, .-_Z14overflow_probeu10__SVBool_tS_S_S_
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align	3
.LC1:
	.string	"OK"
	.align	3
.LC2:
	.string	"WRONG"
	.align	3
.LC3:
	.string	"running at VL %u bits (predicate = %u bytes)\n"
	.align	3
.LC5:
	.string	",:[]{}"
	.align	3
.LC6:
	.string	"classify:        %s\n"
	.align	3
.LC7:
	.string	"classify_padded: %s\n"
	.align	3
.LC8:
	.string	"overflow past w[3]: %d uint16 slots\n"
	.section	.text.startup,"ax",@progbits
	.align	2
	.p2align 5,,15
	.global	main
	.type	main, %function
main:
.LFB3957:
	.cfi_startproc
	sub	sp, sp, #448
	.cfi_def_cfa_offset 448
	cntb	x3
	adrp	x0, :got:__stack_chk_guard
	ldr	x0, [x0, :got_lo12:__stack_chk_guard]
	ubfx	x3, x3, 3, 32
	stp	x29, x30, [sp, 368]
	.cfi_offset 29, -80
	.cfi_offset 30, -72
	add	x29, sp, 368
	cntb	x2, all, mul #8
	stp	x19, x20, [sp, 384]
	stp	x21, x22, [sp, 400]
	stp	x23, x24, [sp, 416]
	.cfi_offset 19, -64
	.cfi_offset 20, -56
	.cfi_offset 21, -48
	.cfi_offset 22, -40
	.cfi_offset 23, -32
	.cfi_offset 24, -24
	add	x24, sp, 79
	and	x24, x24, -64
	str	x25, [sp, 432]
	.cfi_offset 25, -16
	mov	x20, x24
	ldr	x1, [x0]
	str	x1, [sp, 360]
	mov	x1, 0
	mov	w0, 2
	adrp	x1, .LC3
	add	x1, x1, :lo12:.LC3
	bl	__printf_chk
	adrp	x2, .LC4
	add	x0, x24, 1
	add	x3, x24, 256
	add	x2, x2, :lo12:.LC4
	mov	w1, 97
	strb	w1, [x24]
	.p2align 5,,15
.L33:
	and	w1, w0, 31
	ldrb	w1, [x2, w1, sxtw]
	strb	w1, [x0], 1
	cmp	x0, x3
	bne	.L33
	adrp	x22, .LC5
	add	x23, sp, 320
	add	x22, x22, :lo12:.LC5
	mov	w19, 0
	mov	x25, 1
	stp	xzr, xzr, [sp, 320]
	stp	xzr, xzr, [sp, 336]
	.p2align 5,,15
.L35:
	ldrb	w21, [x20]
	mov	x0, x22
	mov	w1, w21
	bl	strchr
	cmp	x0, 0
	ccmp	w21, 0, 4, ne
	beq	.L34
	asr	w0, w19, 6
	lsl	x1, x25, x19
	sbfiz	x0, x0, 3, 32
	ldr	x2, [x23, x0]
	orr	x1, x1, x2
	str	x1, [x23, x0]
.L34:
	add	w19, w19, 1
	add	x20, x20, 1
	cmp	w19, 256
	bne	.L35
	ldp	x19, x1, [sp, 320]
	mov	x0, x24
	ldr	x3, [sp, 336]
	lsl	x2, x19, 5
	sub	x2, x2, x19
	eor	x2, x2, x1
	lsl	x1, x2, 5
	sub	x1, x1, x2
	eor	x1, x1, x3
	ldr	x3, [sp, 344]
	lsl	x2, x1, 5
	sub	x1, x2, x1
	eor	x20, x1, x3
	bl	_Z8classifyPKh
	adrp	x2, .LC2
	add	x2, x2, :lo12:.LC2
	cmp	x0, x19
	bne	.L36
	mov	x1, 256
	mov	x0, x24
	str	x2, [sp, 8]
	bl	_Z12classify_allPKhm
	ldr	x2, [sp, 8]
	cmp	x0, x20
	adrp	x1, .LC1
	add	x1, x1, :lo12:.LC1
	csel	x2, x1, x2, eq
.L36:
	adrp	x1, .LC6
	add	x1, x1, :lo12:.LC6
	mov	w0, 2
	bl	__printf_chk
	mov	x0, x24
	bl	_Z15classify_paddedPKh
	adrp	x2, .LC2
	add	x2, x2, :lo12:.LC2
	cmp	x0, x19
	bne	.L37
	mov	x1, 256
	mov	x0, x24
	str	x2, [sp, 8]
	bl	_Z19classify_padded_allPKhm
	ldr	x2, [sp, 8]
	cmp	x0, x20
	adrp	x1, .LC1
	add	x1, x1, :lo12:.LC1
	csel	x2, x1, x2, eq
.L37:
	adrp	x1, .LC7
	add	x1, x1, :lo12:.LC7
	mov	w0, 2
	bl	__printf_chk
	ptrue	p3.b, all
	mov	p2.b, p3.b
	mov	p1.b, p3.b
	mov	p0.b, p3.b
	bl	_Z14overflow_probeu10__SVBool_tS_S_S_
	mov	w2, w0
	adrp	x1, .LC8
	mov	w0, 2
	add	x1, x1, :lo12:.LC8
	bl	__printf_chk
	adrp	x0, :got:__stack_chk_guard
	ldr	x0, [x0, :got_lo12:__stack_chk_guard]
	ldr	x2, [sp, 360]
	ldr	x1, [x0]
	subs	x2, x2, x1
	mov	x1, 0
	bne	.L49
	ldr	x25, [sp, 432]
	mov	w0, 0
	ldp	x29, x30, [sp, 368]
	ldp	x19, x20, [sp, 384]
	ldp	x21, x22, [sp, 400]
	ldp	x23, x24, [sp, 416]
	add	sp, sp, 448
	.cfi_remember_state
	.cfi_restore 25
	.cfi_restore 23
	.cfi_restore 24
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 19
	.cfi_restore 20
	.cfi_restore 29
	.cfi_restore 30
	.cfi_def_cfa_offset 0
	ret
.L49:
	.cfi_restore_state
	bl	__stack_chk_fail
	.cfi_endproc
.LFE3957:
	.size	main, .-main
	.section	.rodata.str1.8
	.align	3
.LC4:
	.string	"ab,c:d[e]f{g}h  x,y:z[w]v{u}t \"\\"
	.section	.text.startup
	.section	.rodata
	.align	4
	.set	.LANCHOR0,. + 0
.LC0:
	.byte	-1
	.byte	44
	.byte	58
	.byte	91
	.byte	93
	.byte	123
	.byte	125
	.byte	-1
	.byte	44
	.byte	58
	.byte	91
	.byte	93
	.byte	123
	.byte	125
	.byte	44
	.byte	58
	.ident	"GCC: (Ubuntu 16-20260322-1ubuntu1) 16.0.1 20260322 (experimental) [trunk r16-8246-g569ace1fa50]"
	.section	.note.GNU-stack,"",@progbits

	.aeabi_subsection aeabi_feature_and_bits, optional, ULEB128
	.aeabi_attribute Tag_Feature_BTI, 0
	.aeabi_attribute Tag_Feature_PAC, 0
	.aeabi_attribute Tag_Feature_GCS, 0
