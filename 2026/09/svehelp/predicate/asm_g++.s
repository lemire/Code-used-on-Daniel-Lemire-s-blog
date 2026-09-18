	.arch armv9-a+crc+sve2-bitperm
	.file	"pred.cpp"
	.text
	.align	2
	.p2align 5,,15
	.global	_Z12classify_asmPKh
	.type	_Z12classify_asmPKh, %function
_Z12classify_asmPKh:
.LFB7677:
	.cfi_startproc
	sub	sp, sp, #160
	.cfi_def_cfa_offset 160
	adrp	x3, .LANCHOR0
	ptrue	p7.b, vl16
	adrp	x1, :got:__stack_chk_guard
	ldr	x1, [x1, :got_lo12:__stack_chk_guard]
	mov	x2, sp
	stp	x29, x30, [sp, 144]
	.cfi_offset 29, -16
	.cfi_offset 30, -8
	add	x29, sp, 144
	ldr	x4, [x1]
	str	x4, [sp, 136]
	mov	x4, 0
	ldr	q31, [x3, #:lo12:.LANCHOR0]
	ldp	q27, q28, [x0]
	match	p4.b, p7/z, z27.b, z31.b
	match	p5.b, p7/z, z28.b, z31.b
	ldp	q29, q30, [x0, 32]
	match	p6.b, p7/z, z29.b, z31.b
	match	p7.b, p7/z, z30.b, z31.b
#APP
// 29 "pred.cpp" 1
	str p4, [x2]
// 0 "" 2
#NO_APP
	add	x0, sp, 32
#APP
// 30 "pred.cpp" 1
	str p5, [x0]
// 0 "" 2
#NO_APP
	add	x0, sp, 64
#APP
// 31 "pred.cpp" 1
	str p6, [x0]
// 0 "" 2
#NO_APP
	add	x0, sp, 96
#APP
// 32 "pred.cpp" 1
	str p7, [x0]
// 0 "" 2
#NO_APP
	ldrh	w3, [sp, 64]
	ldrh	w4, [sp]
	ldrh	w0, [sp, 32]
	ldrh	w1, [sp, 96]
	lsl	x3, x3, 32
	adrp	x2, :got:__stack_chk_guard
	ldr	x2, [x2, :got_lo12:__stack_chk_guard]
	orr	x0, x3, x0, lsl 16
	orr	x1, x4, x1, lsl 48
	orr	x0, x0, x1
	ldr	x4, [sp, 136]
	ldr	x3, [x2]
	subs	x4, x4, x3
	mov	x3, 0
	bne	.L5
	ldp	x29, x30, [sp, 144]
	add	sp, sp, 160
	.cfi_remember_state
	.cfi_restore 29
	.cfi_restore 30
	.cfi_def_cfa_offset 0
	ret
.L5:
	.cfi_restore_state
	bl	__stack_chk_fail
	.cfi_endproc
.LFE7677:
	.size	_Z12classify_asmPKh, .-_Z12classify_asmPKh
	.align	2
	.p2align 5,,15
	.global	_Z15classify_bridgePKh
	.type	_Z15classify_bridgePKh, %function
_Z15classify_bridgePKh:
.LFB7678:
	.cfi_startproc
	adrp	x2, .LANCHOR0
	ptrue	p7.b, vl16
	movi	d30, #0
	ldp	q3, q2, [x0]
	ptrue	p6.b, all
	adrp	x1, .LC1
	ldr	q26, [x2, #:lo12:.LANCHOR0]
	match	p15.b, p7/z, z3.b, z26.b
	add	x1, x1, :lo12:.LC1
	ld1rd	z31.d, p6/z, [x1]
	sel	z0.b, p15, z31.b, z30.b
	match	p15.b, p7/z, z2.b, z26.b
	ldp	q1, q27, [x0, 32]
	sel	z28.b, p15, z31.b, z30.b
	match	p15.b, p7/z, z1.b, z26.b
	match	p7.b, p7/z, z27.b, z26.b
	sel	z29.b, p15, z31.b, z30.b
	addp	v28.16b, v0.16b, v28.16b
	sel	z30.b, p7, z31.b, z30.b
	addp	v30.16b, v29.16b, v30.16b
	addp	v30.16b, v28.16b, v30.16b
	addp	v30.16b, v30.16b, v30.16b
	umov	x0, v30.d[0]
	ret
	.cfi_endproc
.LFE7678:
	.size	_Z15classify_bridgePKh, .-_Z15classify_bridgePKh
	.align	2
	.p2align 5,,15
	.global	_Z13classify_bextPKh
	.type	_Z13classify_bextPKh, %function
_Z13classify_bextPKh:
.LFB7679:
	.cfi_startproc
	adrp	x1, .LANCHOR0
	add	x2, x1, :lo12:.LANCHOR0
	ptrue	p7.b, vl16
	ldp	q3, q2, [x0]
	mov	z0.b, #-128
	ldr	q27, [x2, 16]
	ldr	q25, [x1, #:lo12:.LANCHOR0]
	match	p15.b, p7/z, z3.b, z25.b
	mov	z28.b, p15/z, #-1
	match	p15.b, p7/z, z2.b, z25.b
	bext	z28.d, z28.d, z0.d
	mov	z29.b, p15/z, #-1
	ldp	q1, q26, [x0, 32]
	bext	z29.d, z29.d, z0.d
	match	p15.b, p7/z, z1.b, z25.b
	match	p7.b, p7/z, z26.b, z25.b
	mov	z30.b, p15/z, #-1
	mov	z31.b, p7/z, #-1
	bext	z30.d, z30.d, z0.d
	bext	z31.d, z31.d, z0.d
	tbl	v27.16b, {v28.16b - v31.16b}, v27.16b
	fmov	x0, d27
	ret
	.cfi_endproc
.LFE7679:
	.size	_Z13classify_bextPKh, .-_Z13classify_bextPKh
	.align	2
	.p2align 5,,15
	.global	_Z15classify_memcpyPKh
	.type	_Z15classify_memcpyPKh, %function
_Z15classify_memcpyPKh:
.LFB7680:
	.cfi_startproc
	adrp	x1, .LANCHOR0
	addvl	sp, sp, #-1
	.cfi_escape 0xf,0x8,0x8f,0,0x92,0x2e,0,0x38,0x1e,0x22
	ptrue	p7.b, vl16
	ldp	q2, q1, [x0]
	addpl	x3, sp, #4
	ldr	q30, [x1, #:lo12:.LANCHOR0]
	match	p15.b, p7/z, z2.b, z30.b
	match	p14.b, p7/z, z1.b, z30.b
	addpl	x1, sp, #7
	ldp	q0, q31, [x0, 32]
	str	p14, [sp, #7, mul vl]
	str	p15, [sp, #4, mul vl]
	addpl	x0, sp, #6
	match	p13.b, p7/z, z0.b, z30.b
	match	p7.b, p7/z, z31.b, z30.b
	str	p13, [sp, #6, mul vl]
	str	p7, [sp, #5, mul vl]
	ldrh	w2, [x1]
	addpl	x1, sp, #5
	ldrh	w3, [x3]
	ldrh	w0, [x0]
	ldrh	w1, [x1]
	addvl	sp, sp, #1
	.cfi_def_cfa_offset 0
	lsl	x0, x0, 32
	orr	x2, x0, x2, lsl 16
	orr	x0, x3, x1, lsl 48
	orr	x0, x2, x0
	ret
	.cfi_endproc
.LFE7680:
	.size	_Z15classify_memcpyPKh, .-_Z15classify_memcpyPKh
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align	3
.LC3:
	.string	",:[]{}"
	.text
	.align	2
	.p2align 5,,15
	.global	_Z15classify_scalarPKh
	.type	_Z15classify_scalarPKh, %function
_Z15classify_scalarPKh:
.LFB7681:
	.cfi_startproc
	stp	x29, x30, [sp, -64]!
	.cfi_def_cfa_offset 64
	.cfi_offset 29, -64
	.cfi_offset 30, -56
	mov	x29, sp
	stp	x21, x22, [sp, 32]
	.cfi_offset 21, -32
	.cfi_offset 22, -24
	adrp	x22, .LC3
	add	x22, x22, :lo12:.LC3
	stp	x23, x24, [sp, 48]
	.cfi_offset 23, -16
	.cfi_offset 24, -8
	mov	x24, x0
	mov	x21, 0
	mov	x23, 1
	stp	x19, x20, [sp, 16]
	.cfi_offset 19, -48
	.cfi_offset 20, -40
	mov	x19, 0
	.p2align 5,,15
.L15:
	ldrb	w20, [x24, x19]
	mov	x0, x22
	mov	w1, w20
	bl	strchr
	lsl	x1, x23, x19
	cbz	x0, .L11
	cbz	w20, .L14
	orr	x21, x21, x1
.L14:
	add	x19, x19, 1
	cmp	x19, 64
	bne	.L15
	mov	x0, x21
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x29, x30, [sp], 64
	.cfi_remember_state
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 23
	.cfi_restore 24
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa_offset 0
	ret
	.p2align 2,,3
.L11:
	.cfi_restore_state
	cmp	w20, 255
	bne	.L14
	orr	x21, x21, x1
	b	.L14
	.cfi_endproc
.LFE7681:
	.size	_Z15classify_scalarPKh, .-_Z15classify_scalarPKh
	.section	.rodata.str1.8
	.align	3
.LC4:
	.string	"SVE vector length: %u bits\n"
	.align	3
.LC7:
	.string	"MISMATCH iter %d: ref %016llx asm %016llx bridge %016llx bext %016llx memcpy %016llx\n"
	.align	3
.LC8:
	.string	"all four agree with the scalar reference"
	.section	.text.startup,"ax",@progbits
	.align	2
	.p2align 5,,15
	.global	main
	.type	main, %function
main:
.LFB7682:
	.cfi_startproc
	stp	x29, x30, [sp, -96]!
	.cfi_def_cfa_offset 96
	.cfi_offset 29, -96
	.cfi_offset 30, -88
	mov	x29, sp
	stp	x19, x20, [sp, 16]
	stp	x21, x22, [sp, 32]
	stp	x23, x24, [sp, 48]
	.cfi_offset 19, -80
	.cfi_offset 20, -72
	.cfi_offset 21, -64
	.cfi_offset 22, -56
	.cfi_offset 23, -48
	.cfi_offset 24, -40
	mov	x24, 1
	stp	x25, x26, [sp, 64]
	stp	x27, x28, [sp, 80]
	sub	sp, sp, #2672
	.cfi_def_cfa_offset 2768
	.cfi_offset 25, -32
	.cfi_offset 26, -24
	.cfi_offset 27, -16
	.cfi_offset 28, -8
	add	x23, sp, 79
	adrp	x0, :got:__stack_chk_guard
	ldr	x0, [x0, :got_lo12:__stack_chk_guard]
	and	x1, x23, -64
	str	x1, [sp, 8]
	adrp	x1, .LC4
	add	x1, x1, :lo12:.LC4
	ldr	x2, [x0]
	str	x2, [sp, 2664]
	mov	x2, 0
	mov	w0, 2
	cntb	x2, all, mul #8
	bl	__printf_chk
	mov	x2, 32557
	mov	x3, 42
	movk	x2, 0x4c95, lsl 16
	add	x1, sp, 136
	movk	x2, 0xf42d, lsl 32
	mov	x0, x3
	movk	x2, 0x5851, lsl 48
	str	x3, [sp, 128]
.L22:
	eor	x0, x0, x0, lsr 62
	madd	x0, x0, x2, x24
	add	x24, x24, 1
	str	x0, [x1], 8
	cmp	x24, 312
	bne	.L22
	adrp	x0, .LC5
	add	x0, x0, :lo12:.LC5
	ldr	x1, [sp, 8]
	add	x19, sp, 2632
	ldr	q30, [x0]
	addvl	x25, sp, #-1
	ldr	q31, [x0, 10]
	add	x21, sp, 128
	add	x25, x25, 136
	add	x20, x1, 64
	str	wzr, [sp, 4]
	str	q30, [x19]
	str	q31, [x19, 10]
.L23:
	mov	x13, 3987079168
	ptrue	p6.b, all
	ldr	x10, [sp, 8]
	addvl	x1, x21, #-1
	addvl	x5, x21, #-1
	movk	x13, 0x7fff, lsl 32
	mov	x12, 262645840084992
	mov	w4, 156
	whilelo	p14.d, wzr, w4
	adrp	x14, .LC6
	mov	w3, 155
	whilelo	p15.d, wzr, w3
	add	x14, x14, :lo12:.LC6
	addvl	x2, x21, #-1
	add	x1, x1, 1248
	add	x5, x5, 1256
	mov	x23, x10
	movk	x13, 0x71d6, lsl 48
	movk	x12, 0xfff7, lsl 48
	mov	x11, 25
	b	.L28
	.p2align 2,,3
.L50:
	ldr	x7, [x21, x24, lsl 3]
	add	x24, x24, 1
.L25:
	lsr	x0, x7, 29
	and	x0, x0, 6148914691236517205
	eor	x0, x0, x7
	and	x7, x13, x0, lsl 17
	eor	x0, x0, x7
	and	x7, x12, x0, lsl 37
	eor	x0, x0, x7
	eor	x0, x0, x0, lsr 43
	udiv	x7, x0, x11
	add	x7, x7, x7, lsl 2
	add	x7, x7, x7, lsl 2
	sub	x0, x0, x7
	ldrb	w0, [x19, x0]
	strb	w0, [x10], 1
	cmp	x20, x10
	beq	.L49
.L28:
	cmp	x24, 312
	bne	.L50
	cntd	x0
	mov	p7.b, p14.b
	ld1rd	z31.d, p6/z, [x14]
	.p2align 5,,15
.L26:
	ld1d	z0.d, p7/z, [x25, x0, lsl 3]
	ld1d	z29.d, p7/z, [x2, x0, lsl 3]
	ld1d	z30.d, p7/z, [x1, x0, lsl 3]
	and	z0.d, z0.d, #0x7fffffff
	and	z29.d, z29.d, #0xffffffff80000000
	orr	z29.d, z0.d, z29.d
	movprfx	z28, z29
	and	z28.d, z28.d, #0x1
	lsr	z29.d, z29.d, #1
	mul	z28.d, z28.d, z31.d
	eor3	z30.d, z30.d, z29.d, z28.d
	st1d	z30.d, p7, [x2, x0, lsl 3]
	whilelo	p7.d, w0, w4
	incd	x0
	b.any	.L26
	cntd	x0
	mov	p7.b, p15.b
	ld1rd	z27.d, p6/z, [x14]
	.p2align 5,,15
.L27:
	ld1d	z1.d, p7/z, [x5, x0, lsl 3]
	ld1d	z25.d, p7/z, [x1, x0, lsl 3]
	ld1d	z26.d, p7/z, [x2, x0, lsl 3]
	and	z1.d, z1.d, #0x7fffffff
	and	z25.d, z25.d, #0xffffffff80000000
	orr	z25.d, z1.d, z25.d
	movprfx	z24, z25
	and	z24.d, z24.d, #0x1
	lsr	z25.d, z25.d, #1
	mul	z24.d, z24.d, z27.d
	eor3	z26.d, z26.d, z25.d, z24.d
	st1d	z26.d, p7, [x1, x0, lsl 3]
	whilelo	p7.d, w0, w3
	incd	x0
	b.any	.L27
	ldr	x7, [sp, 128]
	mov	x6, 6633
	ldr	x0, [sp, 2616]
	movk	x6, 0xa966, lsl 16
	ldr	x16, [sp, 1368]
	movk	x6, 0x6f5a, lsl 32
	bfi	x0, x7, 0, 31
	movk	x6, 0xb502, lsl 48
	mov	x24, 1
	sbfx	x15, x0, 0, 1
	eor	x0, x16, x0, lsr 1
	and	x15, x15, x6
	eor	x0, x0, x15
	str	x0, [sp, 2616]
	b	.L25
.L49:
	adrp	x0, .LC3
	mov	x26, 0
	add	x27, x0, :lo12:.LC3
	mov	w22, 0
	b	.L33
	.p2align 2,,3
.L52:
	cbz	w28, .L32
.L30:
	mov	x0, 1
	lsl	x0, x0, x22
	orr	x26, x26, x0
.L32:
	add	w22, w22, 1
	add	x23, x23, 1
	cmp	w22, 64
	beq	.L51
.L33:
	ldrb	w28, [x23]
	mov	x0, x27
	mov	w1, w28
	bl	strchr
	cbnz	x0, .L52
	cmp	w28, 255
	beq	.L30
	add	w22, w22, 1
	add	x23, x23, 1
	cmp	w22, 64
	bne	.L33
.L51:
	ldr	x22, [sp, 8]
	mov	x0, x22
	bl	_Z12classify_asmPKh
	mov	x4, x0
	mov	x0, x22
	bl	_Z15classify_bridgePKh
	mov	x5, x0
	mov	x0, x22
	bl	_Z13classify_bextPKh
	mov	x6, x0
	mov	x0, x22
	bl	_Z15classify_memcpyPKh
	cmp	x4, x26
	ccmp	x5, x26, 0, eq
	bne	.L34
	cmp	x6, x26
	ccmp	x0, x26, 0, eq
	bne	.L34
	ldr	w0, [sp, 4]
	add	w0, w0, 1
	str	w0, [sp, 4]
	sub	w0, w0, #196608
	subs	w0, w0, #3392
	bne	.L23
	adrp	x0, .LC8
	add	x0, x0, :lo12:.LC8
	bl	puts
	mov	w0, 0
	b	.L21
.L34:
	ldr	w2, [sp, 4]
	mov	x7, x0
	mov	x3, x26
	adrp	x1, .LC7
	mov	w0, 2
	add	x1, x1, :lo12:.LC7
	bl	__printf_chk
	mov	w0, 1
.L21:
	adrp	x1, :got:__stack_chk_guard
	ldr	x1, [x1, :got_lo12:__stack_chk_guard]
	ldr	x3, [sp, 2664]
	ldr	x2, [x1]
	subs	x3, x3, x2
	mov	x2, 0
	bne	.L53
	add	sp, sp, 2672
	.cfi_remember_state
	.cfi_def_cfa_offset 96
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldp	x23, x24, [sp, 48]
	ldp	x25, x26, [sp, 64]
	ldp	x27, x28, [sp, 80]
	ldp	x29, x30, [sp], 96
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 27
	.cfi_restore 28
	.cfi_restore 25
	.cfi_restore 26
	.cfi_restore 23
	.cfi_restore 24
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa_offset 0
	ret
.L53:
	.cfi_restore_state
	bl	__stack_chk_fail
	.cfi_endproc
.LFE7682:
	.size	main, .-main
	.section	.rodata.str1.8
	.align	3
.LC5:
	.string	"abc ,:[]{}\"\\\n0123456789\377\200"
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
.LC2:
	.byte	0
	.byte	8
	.byte	16
	.byte	24
	.byte	32
	.byte	40
	.byte	48
	.byte	56
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.byte	0
.LC1:
	.xword	-9205322385119247871
.LC6:
	.xword	-5403634167711393303
	.ident	"GCC: (Ubuntu 16-20260322-1ubuntu1) 16.0.1 20260322 (experimental) [trunk r16-8246-g569ace1fa50]"
	.section	.note.GNU-stack,"",@progbits

	.aeabi_subsection aeabi_feature_and_bits, optional, ULEB128
	.aeabi_attribute Tag_Feature_BTI, 0
	.aeabi_attribute Tag_Feature_PAC, 0
	.aeabi_attribute Tag_Feature_GCS, 0
