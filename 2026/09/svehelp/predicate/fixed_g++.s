	.arch armv9-a+crc
	.file	"fixedpred.cpp"
	.text
	.align	2
	.p2align 5,,15
	.global	_Z15mask16_noinlineu10__SVBool_t
	.variant_pcs	_Z15mask16_noinlineu10__SVBool_t
	.type	_Z15mask16_noinlineu10__SVBool_t, %function
_Z15mask16_noinlineu10__SVBool_t:
.LFB3950:
	.cfi_startproc
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	str	p0, [sp, #7, mul vl]
	ldrh	w0, [sp, 14]
	add	sp, sp, 16
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE3950:
	.size	_Z15mask16_noinlineu10__SVBool_t, .-_Z15mask16_noinlineu10__SVBool_t
	.align	2
	.p2align 5,,15
	.global	_Z8classifyPKh
	.type	_Z8classifyPKh, %function
_Z8classifyPKh:
.LFB3952:
	.cfi_startproc
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	adrp	x1, .LANCHOR0
	ptrue	p7.b, vl16
	ldp	q2, q1, [x0]
	ldr	q30, [x1, #:lo12:.LANCHOR0]
	match	p15.b, p7/z, z2.b, z30.b
	str	p15, [sp, #4, mul vl]
	match	p15.b, p7/z, z1.b, z30.b
	ldrh	w2, [sp, 8]
	str	p15, [sp, #4, mul vl]
	ldp	q0, q31, [x0, 32]
	match	p15.b, p7/z, z0.b, z30.b
	ldr	w1, [sp, 8]
	str	p15, [sp, #4, mul vl]
	match	p15.b, p7/z, z31.b, z30.b
	ldr	x0, [sp, 8]
	str	p15, [sp, #4, mul vl]
	lsl	w1, w1, 16
	ubfiz	x0, x0, 32, 16
	orr	x1, x1, x0
	ldr	x0, [sp, 8]
	add	sp, sp, 16
	.cfi_def_cfa_offset 0
	orr	x0, x2, x0, lsl 48
	orr	x0, x1, x0
	ret
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
	cmp	x1, 63
	bls	.L9
	adrp	x2, .LANCHOR0
	mov	x3, x0
	ptrue	p7.b, vl16
	mov	x6, 64
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	ldr	q31, [x2, #:lo12:.LANCHOR0]
	sub	x6, x6, x3
	mov	x0, 0
	.p2align 5,,15
.L8:
	ldp	q2, q1, [x3]
	match	p15.b, p7/z, z2.b, z31.b
	str	p15, [sp, #4, mul vl]
	match	p15.b, p7/z, z1.b, z31.b
	ldrh	w4, [sp, 8]
	str	p15, [sp, #4, mul vl]
	ldp	q0, q30, [x3, 32]
	match	p15.b, p7/z, z0.b, z31.b
	add	x3, x3, 64
	ldr	w2, [sp, 8]
	str	p15, [sp, #4, mul vl]
	match	p15.b, p7/z, z30.b, z31.b
	ldr	x5, [sp, 8]
	str	p15, [sp, #4, mul vl]
	ldr	x7, [sp, 8]
	lsl	w2, w2, 16
	ubfiz	x5, x5, 32, 16
	orr	x2, x2, x5
	orr	x4, x4, x7, lsl 48
	orr	x2, x2, x4
	add	x4, x6, x3
	eor	x0, x0, x2
	cmp	x1, x4
	bcs	.L8
	add	sp, sp, 16
	.cfi_def_cfa_offset 0
	ret
	.p2align 2,,3
.L9:
	mov	x0, 0
	ret
	.cfi_endproc
.LFE3953:
	.size	_Z12classify_allPKhm, .-_Z12classify_allPKhm
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align	3
.LC1:
	.string	"OK"
	.align	3
.LC2:
	.string	"WRONG"
	.align	3
.LC3:
	.string	"compiled for %d-bit SVE, running at %u bits, sizeof(fixed_pred)=%zu\n"
	.align	3
.LC5:
	.string	",:[]{}"
	.align	3
.LC6:
	.string	"classify: %016llx expected %016llx %s\n"
	.align	3
.LC7:
	.string	"classify_all (two identical blocks, xor): %016llx expected 0 %s\n"
	.section	.text.startup,"ax",@progbits
	.align	2
	.p2align 5,,15
	.global	main
	.type	main, %function
main:
.LFB3954:
	.cfi_startproc
	sub	sp, sp, #272
	.cfi_def_cfa_offset 272
	mov	w3, 128
	adrp	x0, :got:__stack_chk_guard
	ldr	x0, [x0, :got_lo12:__stack_chk_guard]
	mov	x4, 2
	stp	x29, x30, [sp, 192]
	.cfi_offset 29, -80
	.cfi_offset 30, -72
	add	x29, sp, 192
	adrp	x1, .LC3
	add	x1, x1, :lo12:.LC3
	stp	x19, x20, [sp, 208]
	stp	x21, x22, [sp, 224]
	stp	x23, x24, [sp, 240]
	.cfi_offset 19, -64
	.cfi_offset 20, -56
	.cfi_offset 21, -48
	.cfi_offset 22, -40
	.cfi_offset 23, -32
	.cfi_offset 24, -24
	add	x23, sp, 63
	and	x23, x23, -64
	str	x25, [sp, 256]
	.cfi_offset 25, -16
	mov	x22, x23
	ldr	x2, [x0]
	str	x2, [sp, 184]
	mov	x2, 0
	mov	w0, w4
	mov	w2, w3
	bl	__printf_chk
	adrp	x2, .LC4
	add	x0, x23, 1
	add	x3, x23, 128
	add	x2, x2, :lo12:.LC4
	mov	w1, 97
	strb	w1, [x23]
	.p2align 5,,15
.L16:
	and	w1, w0, 15
	ldrb	w1, [x2, w1, sxtw]
	strb	w1, [x0], 1
	cmp	x3, x0
	bne	.L16
	adrp	x24, .LC5
	add	x24, x24, :lo12:.LC5
	mov	w19, 0
	mov	x20, 0
	mov	x25, 1
	.p2align 5,,15
.L18:
	ldrb	w21, [x22], 1
	mov	x0, x24
	mov	w1, w21
	bl	strchr
	cmp	x0, 0
	lsl	x2, x25, x19
	ccmp	w21, 0, 4, ne
	orr	x2, x20, x2
	add	w19, w19, 1
	csel	x20, x20, x2, eq
	cmp	w19, 64
	bne	.L18
	mov	x0, x23
	bl	_Z8classifyPKh
	mov	x1, 128
	mov	x8, x0
	mov	x0, x23
	bl	_Z12classify_allPKhm
	adrp	x4, .LC2
	mov	x19, x0
	add	x4, x4, :lo12:.LC2
	cmp	x20, x8
	bne	.L19
	adrp	x4, .LC1
	add	x4, x4, :lo12:.LC1
.L19:
	adrp	x1, .LC6
	mov	x3, x20
	mov	x2, x8
	add	x1, x1, :lo12:.LC6
	mov	w0, 2
	bl	__printf_chk
	cbnz	x19, .L23
	adrp	x3, .LC1
	add	x3, x3, :lo12:.LC1
.L20:
	mov	x2, x19
	adrp	x1, .LC7
	mov	w0, 2
	add	x1, x1, :lo12:.LC7
	bl	__printf_chk
	adrp	x0, :got:__stack_chk_guard
	ldr	x0, [x0, :got_lo12:__stack_chk_guard]
	ldr	x2, [sp, 184]
	ldr	x1, [x0]
	subs	x2, x2, x1
	mov	x1, 0
	bne	.L29
	ldr	x25, [sp, 256]
	mov	w0, 0
	ldp	x29, x30, [sp, 192]
	ldp	x19, x20, [sp, 208]
	ldp	x21, x22, [sp, 224]
	ldp	x23, x24, [sp, 240]
	add	sp, sp, 272
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
.L23:
	.cfi_restore_state
	adrp	x3, .LC2
	add	x3, x3, :lo12:.LC2
	b	.L20
.L29:
	bl	__stack_chk_fail
	.cfi_endproc
.LFE3954:
	.size	main, .-main
	.section	.rodata.str1.8
	.align	3
.LC4:
	.string	"ab,c:d[e]f{g}h  "
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
