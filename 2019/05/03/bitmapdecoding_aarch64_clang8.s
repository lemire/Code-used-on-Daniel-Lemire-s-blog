	.text
	.file	"bitmapdecoding.cpp"
	.globl	_Z12compute_minsSt6vectorIS_IySaIyEESaIS1_EE // -- Begin function _Z12compute_minsSt6vectorIS_IySaIyEESaIS1_EE
	.p2align	2
	.type	_Z12compute_minsSt6vectorIS_IySaIyEESaIS1_EE,@function
_Z12compute_minsSt6vectorIS_IySaIyEESaIS1_EE: // @_Z12compute_minsSt6vectorIS_IySaIyEESaIS1_EE
	.cfi_startproc
// %bb.0:
	stp	x24, x23, [sp, #-64]!   // 16-byte Folded Spill
	stp	x22, x21, [sp, #16]     // 16-byte Folded Spill
	stp	x20, x19, [sp, #32]     // 16-byte Folded Spill
	stp	x29, x30, [sp, #48]     // 16-byte Folded Spill
	add	x29, sp, #48            // =48
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -56
	.cfi_offset w24, -64
	ldp	x22, x9, [x0]
	mov	x19, x8
	cmp	x9, x22
	b.ne	.LBB0_2
// %bb.1:
	stp	xzr, xzr, [x19]
	str	xzr, [x19, #16]
	b	.LBB0_32
.LBB0_2:
	ldp	x8, x9, [x22]
	mov	x21, x0
	subs	x0, x9, x8
	asr	x24, x0, #3
	b.eq	.LBB0_6
// %bb.3:
	lsr	x8, x24, #61
	cbnz	x8, .LBB0_33
// %bb.4:
	bl	_Znwm
	ldp	x1, x8, [x22]
	mov	x20, x0
	add	x22, x0, x24, lsl #3
	subs	x2, x8, x1
	b.eq	.LBB0_7
// %bb.5:
	mov	x0, x20
	asr	x23, x2, #3
	bl	memmove
	mov	w8, wzr
	b	.LBB0_8
.LBB0_6:
	mov	x9, xzr
	mov	x20, xzr
	mov	x23, xzr
	lsl	x22, x24, #3
	orr	w8, wzr, #0x1
	b	.LBB0_9
.LBB0_7:
	mov	x23, xzr
	orr	w8, wzr, #0x1
.LBB0_8:
	mov	x9, x20
.LBB0_9:
	ldp	x10, x11, [x21]
	mov	x12, #-6148914691236517206
	movk	x12, #43691
	sub	x11, x11, x10
	asr	x11, x11, #3
	mul	x12, x11, x12
	cmp	x12, #2                 // =2
	add	x11, x9, x23, lsl #3
	b.lo	.LBB0_31
// %bb.10:
	cmp	x23, #1                 // =1
	csinc	x15, x23, xzr, hi
	add	x13, x9, #16            // =16
	orr	w14, wzr, #0x1
	add	x16, x9, x15, lsl #3
	and	x17, x15, #0xfffffffffffffffc
	orr	w18, wzr, #0x18
	b	.LBB0_22
.LBB0_11:                               //   in Loop: Header=BB0_22 Depth=1
	add	x1, x0, #16             // =16
	mov	x2, x13
	mov	x3, x17
.LBB0_12:                               //   Parent Loop BB0_22 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldp	q1, q0, [x1, #-16]
	ldp	q3, q2, [x2, #-16]
	cmhi	v3.2d, v3.2d, v1.2d
	xtn	v3.2s, v3.2d
	fmov	w4, s3
	tbz	w4, #0, .LBB0_14
// %bb.13:                              //   in Loop: Header=BB0_12 Depth=2
	stur	d1, [x2, #-16]
.LBB0_14:                               //   in Loop: Header=BB0_12 Depth=2
	mov	w4, v3.s[1]
	tbz	w4, #0, .LBB0_16
// %bb.15:                              //   in Loop: Header=BB0_12 Depth=2
	sub	x4, x2, #8              // =8
	st1	{ v1.d }[1], [x4]
.LBB0_16:                               //   in Loop: Header=BB0_12 Depth=2
	cmhi	v1.2d, v2.2d, v0.2d
	xtn	v1.2s, v1.2d
	fmov	w4, s1
	tbz	w4, #0, .LBB0_18
// %bb.17:                              //   in Loop: Header=BB0_12 Depth=2
	str	d0, [x2]
.LBB0_18:                               //   in Loop: Header=BB0_12 Depth=2
	mov	w4, v1.s[1]
	tbz	w4, #0, .LBB0_20
// %bb.19:                              //   in Loop: Header=BB0_12 Depth=2
	add	x4, x2, #8              // =8
	st1	{ v0.d }[1], [x4]
.LBB0_20:                               //   in Loop: Header=BB0_12 Depth=2
	subs	x3, x3, #4              // =4
	add	x2, x2, #32             // =32
	add	x1, x1, #32             // =32
	b.ne	.LBB0_12
// %bb.21:                              //   in Loop: Header=BB0_22 Depth=1
	cmp	x15, x17
	mov	x1, x17
	b.ne	.LBB0_27
	b	.LBB0_30
.LBB0_22:                               // =>This Loop Header: Depth=1
                                        //     Child Loop BB0_12 Depth 2
                                        //     Child Loop BB0_27 Depth 2
	tbnz	w8, #0, .LBB0_30
// %bb.23:                              //   in Loop: Header=BB0_22 Depth=1
	mul	x0, x14, x18
	ldr	x0, [x10, x0]
	cmp	x15, #3                 // =3
	b.ls	.LBB0_26
// %bb.24:                              //   in Loop: Header=BB0_22 Depth=1
	add	x1, x0, x15, lsl #3
	cmp	x20, x1
	b.hs	.LBB0_11
// %bb.25:                              //   in Loop: Header=BB0_22 Depth=1
	cmp	x0, x16
	b.hs	.LBB0_11
.LBB0_26:                               //   in Loop: Header=BB0_22 Depth=1
	mov	x1, xzr
.LBB0_27:                               //   Parent Loop BB0_22 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x3, x1, #3
	ldr	x2, [x0, x3]
	ldr	x3, [x9, x3]
	cmp	x2, x3
	b.hs	.LBB0_29
// %bb.28:                              //   in Loop: Header=BB0_27 Depth=2
	str	x2, [x9, x1, lsl #3]
.LBB0_29:                               //   in Loop: Header=BB0_27 Depth=2
	add	x1, x1, #1              // =1
	cmp	x1, x23
	b.lo	.LBB0_27
.LBB0_30:                               //   in Loop: Header=BB0_22 Depth=1
	add	x14, x14, #1            // =1
	cmp	x14, x12
	b.lo	.LBB0_22
.LBB0_31:
	stp	x9, x11, [x19]
	str	x22, [x19, #16]
.LBB0_32:
	ldp	x29, x30, [sp, #48]     // 16-byte Folded Reload
	ldp	x20, x19, [sp, #32]     // 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]     // 16-byte Folded Reload
	ldp	x24, x23, [sp], #64     // 16-byte Folded Reload
	ret
.LBB0_33:
	bl	_ZSt17__throw_bad_allocv
.Lfunc_end0:
	.size	_Z12compute_minsSt6vectorIS_IySaIyEESaIS1_EE, .Lfunc_end0-_Z12compute_minsSt6vectorIS_IySaIyEESaIS1_EE
	.cfi_endproc
                                        // -- End function
	.globl	_Z16compute_averagesSt6vectorIS_IySaIyEESaIS1_EE // -- Begin function _Z16compute_averagesSt6vectorIS_IySaIyEESaIS1_EE
	.p2align	2
	.type	_Z16compute_averagesSt6vectorIS_IySaIyEESaIS1_EE,@function
_Z16compute_averagesSt6vectorIS_IySaIyEESaIS1_EE: // @_Z16compute_averagesSt6vectorIS_IySaIyEESaIS1_EE
	.cfi_startproc
// %bb.0:
	str	x23, [sp, #-64]!        // 8-byte Folded Spill
	stp	x22, x21, [sp, #16]     // 16-byte Folded Spill
	stp	x20, x19, [sp, #32]     // 16-byte Folded Spill
	stp	x29, x30, [sp, #48]     // 16-byte Folded Spill
	add	x29, sp, #48            // =48
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -64
	ldp	x10, x9, [x0]
	mov	x19, x8
	cmp	x9, x10
	b.ne	.LBB1_2
// %bb.1:
	stp	xzr, xzr, [x19]
	str	xzr, [x19, #16]
	b	.LBB1_24
.LBB1_2:
	ldp	x8, x11, [x10]
	subs	x22, x11, x8
	asr	x23, x22, #3
	b.eq	.LBB1_5
// %bb.3:
	lsr	x8, x23, #61
	cbnz	x8, .LBB1_25
// %bb.4:
	mov	x21, x0
	mov	x0, x22
	bl	_Znwm
	mov	w1, wzr
	mov	x2, x22
	mov	x20, x0
	add	x23, x0, x23, lsl #3
	bl	memset
	ldp	x10, x9, [x21]
	mov	x8, x23
	b	.LBB1_6
.LBB1_5:
	mov	x20, xzr
	mov	x8, xzr
	lsl	x23, x23, #3
.LBB1_6:
	sub	x13, x9, x10
	mov	x9, #-6148914691236517206
	movk	x9, #43691
	sub	x12, x8, x20
	asr	x11, x13, #3
	mul	x11, x11, x9
	asr	x9, x12, #3
	cbz	x13, .LBB1_16
// %bb.7:
	cmp	x9, #1                  // =1
	csinc	x15, x9, xzr, hi
	mov	x13, xzr
	add	x14, x20, #16           // =16
	and	x16, x15, #0xfffffffffffffffc
	orr	w17, wzr, #0x18
.LBB1_8:                                // =>This Loop Header: Depth=1
                                        //     Child Loop BB1_12 Depth 2
                                        //     Child Loop BB1_14 Depth 2
	cbz	x12, .LBB1_15
// %bb.9:                               //   in Loop: Header=BB1_8 Depth=1
	mul	x18, x13, x17
	ldr	x18, [x10, x18]
	cmp	x15, #4                 // =4
	b.hs	.LBB1_11
// %bb.10:                              //   in Loop: Header=BB1_8 Depth=1
	mov	x0, xzr
	b	.LBB1_14
.LBB1_11:                               //   in Loop: Header=BB1_8 Depth=1
	add	x0, x18, #16            // =16
	mov	x1, x16
	mov	x2, x14
.LBB1_12:                               //   Parent Loop BB1_8 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldp	q0, q1, [x0, #-16]
	ldp	q2, q3, [x2, #-16]
	add	x0, x0, #32             // =32
	subs	x1, x1, #4              // =4
	ucvtf	v0.2d, v0.2d
	ucvtf	v1.2d, v1.2d
	fadd	v0.2d, v2.2d, v0.2d
	fadd	v1.2d, v3.2d, v1.2d
	stp	q0, q1, [x2, #-16]
	add	x2, x2, #32             // =32
	b.ne	.LBB1_12
// %bb.13:                              //   in Loop: Header=BB1_8 Depth=1
	cmp	x15, x16
	mov	x0, x16
	b.eq	.LBB1_15
.LBB1_14:                               //   Parent Loop BB1_8 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x1, x0, #3
	ldr	d0, [x18, x1]
	ldr	d1, [x20, x1]
	add	x0, x0, #1              // =1
	cmp	x0, x9
	ucvtf	d0, d0
	fadd	d0, d1, d0
	str	d0, [x20, x1]
	b.lo	.LBB1_14
.LBB1_15:                               //   in Loop: Header=BB1_8 Depth=1
	add	x13, x13, #1            // =1
	cmp	x13, x11
	b.lo	.LBB1_8
.LBB1_16:
	cbz	x12, .LBB1_23
// %bb.17:
	cmp	x9, #1                  // =1
	csinc	x12, x9, xzr, hi
	cmp	x12, #3                 // =3
	ucvtf	d0, x11
	b.hi	.LBB1_19
// %bb.18:
	mov	x10, xzr
	b	.LBB1_22
.LBB1_19:
	and	x10, x12, #0xfffffffffffffffc
	dup	v1.2d, v0.d[0]
	add	x11, x20, #16           // =16
	mov	x13, x10
.LBB1_20:                               // =>This Inner Loop Header: Depth=1
	ldp	q2, q3, [x11, #-16]
	subs	x13, x13, #4            // =4
	fdiv	v2.2d, v2.2d, v1.2d
	fdiv	v3.2d, v3.2d, v1.2d
	stp	q2, q3, [x11, #-16]
	add	x11, x11, #32           // =32
	b.ne	.LBB1_20
// %bb.21:
	cmp	x12, x10
	b.eq	.LBB1_23
.LBB1_22:                               // =>This Inner Loop Header: Depth=1
	lsl	x11, x10, #3
	ldr	d1, [x20, x11]
	add	x10, x10, #1            // =1
	cmp	x10, x9
	fdiv	d1, d1, d0
	str	d1, [x20, x11]
	b.lo	.LBB1_22
.LBB1_23:
	stp	x20, x8, [x19]
	str	x23, [x19, #16]
.LBB1_24:
	ldp	x29, x30, [sp, #48]     // 16-byte Folded Reload
	ldp	x20, x19, [sp, #32]     // 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]     // 16-byte Folded Reload
	ldr	x23, [sp], #64          // 8-byte Folded Reload
	ret
.LBB1_25:
	bl	_ZSt17__throw_bad_allocv
.Lfunc_end1:
	.size	_Z16compute_averagesSt6vectorIS_IySaIyEESaIS1_EE, .Lfunc_end1-_Z16compute_averagesSt6vectorIS_IySaIyEESaIS1_EE
	.cfi_endproc
                                        // -- End function
	.globl	_Z14load_text_filePKcPm // -- Begin function _Z14load_text_filePKcPm
	.p2align	2
	.type	_Z14load_text_filePKcPm,@function
_Z14load_text_filePKcPm:                // @_Z14load_text_filePKcPm
.Lfunc_begin0:
	.cfi_startproc
	.cfi_personality 0, __gxx_personality_v0
	.cfi_lsda 0, .Lexception0
// %bb.0:
	stp	x22, x21, [sp, #-48]!   // 16-byte Folded Spill
	stp	x20, x19, [sp, #16]     // 16-byte Folded Spill
	stp	x29, x30, [sp, #32]     // 16-byte Folded Spill
	add	x29, sp, #32            // =32
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	mov	x19, x1
	adrp	x1, .L.str
	add	x1, x1, :lo12:.L.str
	mov	x21, x0
	bl	fopen
	cbz	x0, .LBB2_4
// %bb.1:
	orr	w2, wzr, #0x2
	mov	x1, xzr
	mov	x20, x0
	bl	fseek
	mov	x0, x20
	bl	ftell
	str	x0, [x19]
	mov	x0, x20
	bl	rewind
	ldr	x22, [x19]
	mov	x0, x22
	bl	_Znam
	orr	w1, wzr, #0x1
	mov	x2, x22
	mov	x3, x20
	mov	x21, x0
	bl	fread_unlocked
	mov	x22, x0
	mov	x0, x20
	bl	fclose
	ldr	x8, [x19]
	cmp	x22, x8
	b.eq	.LBB2_12
// %bb.2:
	orr	w0, wzr, #0x10
	bl	__cxa_allocate_exception
	mov	x19, x0
.Ltmp0:
	adrp	x1, .L.str.1
	add	x1, x1, :lo12:.L.str.1
	bl	_ZNSt13runtime_errorC1EPKc
.Ltmp1:
// %bb.3:
	adrp	x1, _ZTISt13runtime_error
	adrp	x2, _ZNSt13runtime_errorD1Ev
	add	x1, x1, :lo12:_ZTISt13runtime_error
	add	x2, x2, :lo12:_ZNSt13runtime_errorD1Ev
	mov	x0, x19
	bl	__cxa_throw
.LBB2_4:
	adrp	x19, _ZSt4cout
	add	x19, x19, :lo12:_ZSt4cout
	adrp	x1, .L.str.2
	add	x1, x1, :lo12:.L.str.2
	mov	w2, #11
	mov	x0, x19
	bl	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	cbz	x21, .LBB2_6
// %bb.5:
	mov	x0, x21
	bl	strlen
	mov	x2, x0
	adrp	x0, _ZSt4cout
	add	x0, x0, :lo12:_ZSt4cout
	mov	x1, x21
	bl	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	b	.LBB2_7
.LBB2_6:
	ldr	x8, [x19]
	ldur	x8, [x8, #-24]
	add	x0, x19, x8
	ldr	w8, [x0, #32]
	orr	w1, w8, #0x1
	bl	_ZNSt9basic_iosIcSt11char_traitsIcEE5clearESt12_Ios_Iostate
.LBB2_7:
	ldr	x8, [x19]
	ldur	x8, [x8, #-24]
	add	x8, x19, x8
	ldr	x19, [x8, #240]
	cbz	x19, .LBB2_13
// %bb.8:
	ldrb	w8, [x19, #56]
	cbz	w8, .LBB2_10
// %bb.9:
	ldrb	w1, [x19, #67]
	b	.LBB2_11
.LBB2_10:
	mov	x0, x19
	bl	_ZNKSt5ctypeIcE13_M_widen_initEv
	ldr	x8, [x19]
	mov	w1, #10
	mov	x0, x19
	ldr	x8, [x8, #48]
	blr	x8
	mov	w1, w0
.LBB2_11:
	adrp	x0, _ZSt4cout
	add	x0, x0, :lo12:_ZSt4cout
	bl	_ZNSo3putEc
	bl	_ZNSo5flushEv
	mov	x21, xzr
.LBB2_12:
	ldp	x29, x30, [sp, #32]     // 16-byte Folded Reload
	ldp	x20, x19, [sp, #16]     // 16-byte Folded Reload
	mov	x0, x21
	ldp	x22, x21, [sp], #48     // 16-byte Folded Reload
	ret
.LBB2_13:
	bl	_ZSt16__throw_bad_castv
.LBB2_14:
.Ltmp2:
	mov	x20, x0
	mov	x0, x19
	bl	__cxa_free_exception
	mov	x0, x20
	bl	_Unwind_Resume
.Lfunc_end2:
	.size	_Z14load_text_filePKcPm, .Lfunc_end2-_Z14load_text_filePKcPm
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table2:
.Lexception0:
	.byte	255                     // @LPStart Encoding = omit
	.byte	255                     // @TType Encoding = omit
	.byte	1                       // Call site Encoding = uleb128
	.uleb128 .Lcst_end0-.Lcst_begin0
.Lcst_begin0:
	.uleb128 .Lfunc_begin0-.Lfunc_begin0 // >> Call Site 1 <<
	.uleb128 .Ltmp0-.Lfunc_begin0   //   Call between .Lfunc_begin0 and .Ltmp0
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp0-.Lfunc_begin0   // >> Call Site 2 <<
	.uleb128 .Ltmp1-.Ltmp0          //   Call between .Ltmp0 and .Ltmp1
	.uleb128 .Ltmp2-.Lfunc_begin0   //     jumps to .Ltmp2
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp1-.Lfunc_begin0   // >> Call Site 3 <<
	.uleb128 .Lfunc_end2-.Ltmp1     //   Call between .Ltmp1 and .Lfunc_end2
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
.Lcst_end0:
	.p2align	2
                                        // -- End function
	.text
	.globl	_Z12build_bitmapPKccPm  // -- Begin function _Z12build_bitmapPKccPm
	.p2align	2
	.type	_Z12build_bitmapPKccPm,@function
_Z12build_bitmapPKccPm:                 // @_Z12build_bitmapPKccPm
	.cfi_startproc
// %bb.0:
	str	x23, [sp, #-64]!        // 8-byte Folded Spill
	stp	x22, x21, [sp, #16]     // 16-byte Folded Spill
	stp	x20, x19, [sp, #32]     // 16-byte Folded Spill
	stp	x29, x30, [sp, #48]     // 16-byte Folded Spill
	add	x29, sp, #48            // =48
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -64
	mov	w19, w1
	add	x1, sp, #8              // =8
	mov	x21, x2
	bl	_Z14load_text_filePKcPm
	cbz	x0, .LBB3_8
// %bb.1:
	ldr	x23, [sp, #8]
	mov	x20, x0
	lsr	x8, x23, #6
	lsl	x22, x8, #3
	mov	x0, x22
	str	x8, [x21]
	bl	_Znam
	mov	w1, wzr
	mov	x2, x22
	mov	x21, x0
	bl	memset
	ands	x8, x23, #0xffffffffffffffc0
	b.eq	.LBB3_7
// %bb.2:
	mov	x9, xzr
	orr	w10, wzr, #0x1
.LBB3_3:                                // =>This Inner Loop Header: Depth=1
	ldrb	w11, [x20, x9]
	cmp	w11, w19, uxtb
	b.eq	.LBB3_5
// %bb.4:                               //   in Loop: Header=BB3_3 Depth=1
	cmp	w11, #31                // =31
	b.hi	.LBB3_6
.LBB3_5:                                //   in Loop: Header=BB3_3 Depth=1
	lsr	x11, x9, #3
	and	x11, x11, #0x1ffffffffffffff8
	ldr	x12, [x21, x11]
	lsl	x13, x10, x9
	orr	x12, x12, x13
	str	x12, [x21, x11]
.LBB3_6:                                //   in Loop: Header=BB3_3 Depth=1
	add	x9, x9, #1              // =1
	cmp	x8, x9
	b.ne	.LBB3_3
.LBB3_7:
	mov	x0, x20
	bl	_ZdaPv
	b	.LBB3_9
.LBB3_8:
	mov	x21, xzr
.LBB3_9:
	mov	x0, x21
	ldp	x29, x30, [sp, #48]     // 16-byte Folded Reload
	ldp	x20, x19, [sp, #32]     // 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]     // 16-byte Folded Reload
	ldr	x23, [sp], #64          // 8-byte Folded Reload
	ret
.Lfunc_end3:
	.size	_Z12build_bitmapPKccPm, .Lfunc_end3-_Z12build_bitmapPKccPm
	.cfi_endproc
                                        // -- End function
	.globl	_Z13basic_decoderPjRjjm // -- Begin function _Z13basic_decoderPjRjjm
	.p2align	2
	.type	_Z13basic_decoderPjRjjm,@function
_Z13basic_decoderPjRjjm:                // @_Z13basic_decoderPjRjjm
// %bb.0:
	cbz	x3, .LBB4_3
// %bb.1:
	ldr	w8, [x1]
.LBB4_2:                                // =>This Inner Loop Header: Depth=1
	rbit	x9, x3
	clz	x9, x9
	add	w9, w9, w2
	str	w9, [x0, w8, uxtw #2]
	ldr	w8, [x1]
	sub	x9, x3, #1              // =1
	ands	x3, x9, x3
	add	w8, w8, #1              // =1
	str	w8, [x1]
	b.ne	.LBB4_2
.LBB4_3:
	ret
.Lfunc_end4:
	.size	_Z13basic_decoderPjRjjm, .Lfunc_end4-_Z13basic_decoderPjRjjm
                                        // -- End function
	.globl	_Z8init_bufP5buf_sm     // -- Begin function _Z8init_bufP5buf_sm
	.p2align	2
	.type	_Z8init_bufP5buf_sm,@function
_Z8init_bufP5buf_sm:                    // @_Z8init_bufP5buf_sm
	.cfi_startproc
// %bb.0:
	str	x21, [sp, #-48]!        // 8-byte Folded Spill
	stp	x20, x19, [sp, #16]     // 16-byte Folded Spill
	stp	x29, x30, [sp, #32]     // 16-byte Folded Spill
	add	x29, sp, #32            // =32
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -48
	mov	x19, x0
	str	wzr, [x0]
	mov	w0, #520
	mov	x20, x1
	bl	_Znam
	str	x0, [x19, #8]
	mov	w0, #520
	bl	_Znam
	lsl	x8, x20, #4
	cmp	xzr, x20, lsr #60
	csinv	x20, x8, xzr, eq
	orr	w21, wzr, #0x8
	str	x0, [x19, #16]
.LBB5_1:                                // =>This Inner Loop Header: Depth=1
	mov	x0, x20
	bl	_Znam
	ldr	x8, [x19, #8]
	str	x0, [x8, x21]
	ldp	x8, x9, [x19, #8]
	ldr	x8, [x8, x21]
	str	x8, [x9, x21]
	add	x21, x21, #8            // =8
	cmp	x21, #520               // =520
	b.ne	.LBB5_1
// %bb.2:
	ldp	x29, x30, [sp, #32]     // 16-byte Folded Reload
	ldp	x20, x19, [sp, #16]     // 16-byte Folded Reload
	ldr	x21, [sp], #48          // 8-byte Folded Reload
	ret
.Lfunc_end5:
	.size	_Z8init_bufP5buf_sm, .Lfunc_end5-_Z8init_bufP5buf_sm
	.cfi_endproc
                                        // -- End function
	.globl	_Z9reset_bufP5buf_s     // -- Begin function _Z9reset_bufP5buf_s
	.p2align	2
	.type	_Z9reset_bufP5buf_s,@function
_Z9reset_bufP5buf_s:                    // @_Z9reset_bufP5buf_s
// %bb.0:
	orr	w8, wzr, #0x8
	str	wzr, [x0]
.LBB6_1:                                // =>This Inner Loop Header: Depth=1
	ldp	x10, x9, [x0, #8]
	ldr	x9, [x9, x8]
	str	x9, [x10, x8]
	add	x8, x8, #8              // =8
	cmp	x8, #520                // =520
	b.ne	.LBB6_1
// %bb.2:
	ret
.Lfunc_end6:
	.size	_Z9reset_bufP5buf_s, .Lfunc_end6-_Z9reset_bufP5buf_s
                                        // -- End function
	.globl	_Z11destroy_bufP5buf_s  // -- Begin function _Z11destroy_bufP5buf_s
	.p2align	2
	.type	_Z11destroy_bufP5buf_s,@function
_Z11destroy_bufP5buf_s:                 // @_Z11destroy_bufP5buf_s
// %bb.0:
	stp	x20, x19, [sp, #-32]!   // 16-byte Folded Spill
	mov	x19, x0
	orr	w20, wzr, #0x8
	stp	x29, x30, [sp, #16]     // 16-byte Folded Spill
	add	x29, sp, #16            // =16
.LBB7_1:                                // =>This Inner Loop Header: Depth=1
	ldr	x8, [x19, #16]
	ldr	x0, [x8, x20]
	cbz	x0, .LBB7_3
// %bb.2:                               //   in Loop: Header=BB7_1 Depth=1
	bl	_ZdaPv
.LBB7_3:                                //   in Loop: Header=BB7_1 Depth=1
	add	x20, x20, #8            // =8
	cmp	x20, #520               // =520
	b.ne	.LBB7_1
// %bb.4:
	ldr	x0, [x19, #8]
	cbz	x0, .LBB7_6
// %bb.5:
	bl	_ZdaPv
.LBB7_6:
	ldr	x0, [x19, #16]
	cbz	x0, .LBB7_8
// %bb.7:
	ldp	x29, x30, [sp, #16]     // 16-byte Folded Reload
	ldp	x20, x19, [sp], #32     // 16-byte Folded Reload
	b	_ZdaPv
.LBB7_8:
	ldp	x29, x30, [sp, #16]     // 16-byte Folded Reload
	ldp	x20, x19, [sp], #32     // 16-byte Folded Reload
	ret
.Lfunc_end7:
	.size	_Z11destroy_bufP5buf_s, .Lfunc_end7-_Z11destroy_bufP5buf_s
                                        // -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3               // -- Begin function _Z8fasttestPKcc
.LCPI8_0:
	.xword	4636737291354636288     // double 100
.LCPI8_1:
	.xword	4634204016564240384     // double 64
	.text
	.globl	_Z8fasttestPKcc
	.p2align	2
	.type	_Z8fasttestPKcc,@function
_Z8fasttestPKcc:                        // @_Z8fasttestPKcc
.Lfunc_begin1:
	.cfi_startproc
	.cfi_personality 0, __gxx_personality_v0
	.cfi_lsda 0, .Lexception1
// %bb.0:
	sub	sp, sp, #464            // =464
	str	d10, [sp, #336]         // 8-byte Folded Spill
	stp	d9, d8, [sp, #352]      // 16-byte Folded Spill
	stp	x28, x27, [sp, #368]    // 16-byte Folded Spill
	stp	x26, x25, [sp, #384]    // 16-byte Folded Spill
	stp	x24, x23, [sp, #400]    // 16-byte Folded Spill
	stp	x22, x21, [sp, #416]    // 16-byte Folded Spill
	stp	x20, x19, [sp, #432]    // 16-byte Folded Spill
	stp	x29, x30, [sp, #448]    // 16-byte Folded Spill
	add	x29, sp, #448           // =448
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -56
	.cfi_offset w24, -64
	.cfi_offset w25, -72
	.cfi_offset w26, -80
	.cfi_offset w27, -88
	.cfi_offset w28, -96
	.cfi_offset b8, -104
	.cfi_offset b9, -112
	.cfi_offset b10, -128
	mov	w19, w1
	add	x1, sp, #152            // =152
	bl	_Z14load_text_filePKcPm
	cbz	x0, .LBB8_8
// %bb.1:
	ldr	x24, [sp, #152]
	mov	x21, x0
	lsr	x22, x24, #6
	lsl	x23, x22, #3
	mov	x0, x23
	bl	_Znam
	mov	w1, wzr
	mov	x2, x23
	mov	x20, x0
	bl	memset
	ands	x8, x24, #0xffffffffffffffc0
	b.eq	.LBB8_7
// %bb.2:
	mov	x9, xzr
	orr	w10, wzr, #0x1
.LBB8_3:                                // =>This Inner Loop Header: Depth=1
	ldrb	w11, [x21, x9]
	cmp	w11, w19, uxtb
	b.eq	.LBB8_5
// %bb.4:                               //   in Loop: Header=BB8_3 Depth=1
	cmp	w11, #31                // =31
	b.hi	.LBB8_6
.LBB8_5:                                //   in Loop: Header=BB8_3 Depth=1
	lsr	x11, x9, #3
	and	x11, x11, #0x1ffffffffffffff8
	ldr	x12, [x20, x11]
	lsl	x13, x10, x9
	orr	x12, x12, x13
	str	x12, [x20, x11]
.LBB8_6:                                //   in Loop: Header=BB8_3 Depth=1
	add	x9, x9, #1              // =1
	cmp	x8, x9
	b.ne	.LBB8_3
.LBB8_7:
	mov	x0, x21
	bl	_ZdaPv
	b	.LBB8_9
.LBB8_8:
	mov	x20, xzr
                                        // implicit-def: $x22
.LBB8_9:
	lsl	x19, x22, #6
	lsl	x8, x22, #8
	cmp	xzr, x19, lsr #62
	csinv	x0, x8, xzr, eq
	bl	_Znam
	mov	x21, x0
.Ltmp3:
	orr	w0, wzr, #0x4
	bl	_Znwm
.Ltmp4:
// %bb.10:
	mov	x25, x0
	str	wzr, [x0]
.Ltmp6:
	orr	w0, wzr, #0x8
	bl	_Znwm
.Ltmp7:
// %bb.11:
	ldr	w8, [x25]
	orr	w9, wzr, #0x1
	mov	x24, x0
	stp	w8, w9, [x0]
	mov	x0, x25
	bl	_ZdlPv
.Ltmp9:
	orr	w0, wzr, #0x10
	bl	_Znwm
.Ltmp10:
// %bb.12:
	ldr	x8, [x24]
	mov	w9, #5
	mov	x23, x0
	str	w9, [x0, #8]
	str	x8, [x0]
	mov	x0, x24
	bl	_ZdlPv
	orr	w8, wzr, #0x2
	str	w8, [x23, #12]
.Ltmp12:
	orr	w0, wzr, #0x20
	bl	_Znwm
.Ltmp13:
// %bb.13:
	ldr	q0, [x23]
	orr	w8, wzr, #0x3
	mov	x25, x0
	str	w8, [x0, #16]
	str	q0, [x0]
	mov	x0, x23
	bl	_ZdlPv
	stp	xzr, xzr, [sp, #136]
	str	xzr, [sp, #128]
.Ltmp15:
	mov	w0, #20
	bl	_Znwm
.Ltmp16:
// %bb.14:
	ldr	w9, [x25, #16]
	ldr	q0, [x25]
	add	x8, x0, #20             // =20
	stp	x0, x0, [sp, #128]
	stp	x8, x8, [sp, #136]
	str	w9, [x0, #16]
	str	q0, [x0]
.Ltmp18:
	add	x0, sp, #152            // =152
	add	x1, sp, #128            // =128
	bl	_ZN11LinuxEventsILi0EEC2ESt6vectorIiSaIiEE
.Ltmp19:
// %bb.15:
	ldr	x0, [sp, #128]
	cbz	x0, .LBB8_17
// %bb.16:
	bl	_ZdlPv
.LBB8_17:
	stp	xzr, xzr, [sp, #112]
	stp	xzr, xzr, [sp, #96]
	stp	xzr, xzr, [sp, #80]
.Ltmp21:
	mov	w0, #40
	bl	_Znwm
.Ltmp22:
// %bb.18:
	ldp	x24, x8, [sp, #104]
	mov	x23, x0
	movi	v0.2d, #0000000000000000
	str	xzr, [x0, #32]
	subs	x2, x8, x24
	stp	q0, q0, [x0]
	b.eq	.LBB8_20
// %bb.19:
	mov	x0, x23
	mov	x1, x24
	bl	memmove
.LBB8_20:
	cbz	x24, .LBB8_22
// %bb.21:
	mov	x0, x24
	bl	_ZdlPv
.LBB8_22:
	add	x8, x23, #40            // =40
	str	x25, [sp, #16]          // 8-byte Folded Spill
	str	x8, [sp, #120]
	stp	x23, x8, [sp, #104]
.Ltmp24:
	mov	w0, #520
	bl	_Znam
.Ltmp25:
// %bb.23:
	mov	x23, x0
.Ltmp26:
	mov	w0, #520
	bl	_Znam
.Ltmp27:
// %bb.24:
	lsl	x8, x19, #4
	cmp	xzr, x19, lsr #60
	mov	x24, x0
	str	x19, [sp, #8]           // 8-byte Folded Spill
	csinv	x25, x8, xzr, eq
	orr	w19, wzr, #0x8
.LBB8_25:                               // =>This Inner Loop Header: Depth=1
.Ltmp29:
	mov	x0, x25
	bl	_Znam
.Ltmp30:
// %bb.26:                              //   in Loop: Header=BB8_25 Depth=1
	str	x0, [x23, x19]
	str	x0, [x24, x19]
	add	x19, x19, #8            // =8
	cmp	x19, #520               // =520
	b.ne	.LBB8_25
// %bb.27:
	mov	w19, wzr
	add	x26, x24, #8            // =8
	add	x27, x23, #8            // =8
	stur	x26, [x29, #-104]       // 8-byte Folded Spill
	str	x27, [sp, #24]          // 8-byte Folded Spill
.LBB8_28:                               // =>This Loop Header: Depth=1
                                        //     Child Loop BB8_31 Depth 2
                                        //     Child Loop BB8_36 Depth 2
                                        //     Child Loop BB8_38 Depth 2
                                        //     Child Loop BB8_40 Depth 2
                                        //     Child Loop BB8_42 Depth 2
                                        //     Child Loop BB8_44 Depth 2
                                        //     Child Loop BB8_46 Depth 2
                                        //     Child Loop BB8_48 Depth 2
                                        //     Child Loop BB8_50 Depth 2
                                        //     Child Loop BB8_52 Depth 2
                                        //     Child Loop BB8_54 Depth 2
                                        //     Child Loop BB8_56 Depth 2
                                        //     Child Loop BB8_58 Depth 2
                                        //     Child Loop BB8_60 Depth 2
                                        //     Child Loop BB8_62 Depth 2
                                        //     Child Loop BB8_64 Depth 2
                                        //     Child Loop BB8_66 Depth 2
                                        //     Child Loop BB8_68 Depth 2
                                        //     Child Loop BB8_70 Depth 2
                                        //     Child Loop BB8_72 Depth 2
                                        //     Child Loop BB8_74 Depth 2
                                        //     Child Loop BB8_76 Depth 2
                                        //     Child Loop BB8_78 Depth 2
                                        //       Child Loop BB8_79 Depth 3
                                        //     Child Loop BB8_82 Depth 2
                                        //       Child Loop BB8_83 Depth 3
                                        //     Child Loop BB8_86 Depth 2
                                        //       Child Loop BB8_87 Depth 3
                                        //     Child Loop BB8_90 Depth 2
                                        //       Child Loop BB8_91 Depth 3
                                        //     Child Loop BB8_94 Depth 2
                                        //       Child Loop BB8_95 Depth 3
                                        //     Child Loop BB8_98 Depth 2
                                        //       Child Loop BB8_99 Depth 3
                                        //     Child Loop BB8_102 Depth 2
                                        //       Child Loop BB8_103 Depth 3
                                        //     Child Loop BB8_106 Depth 2
                                        //       Child Loop BB8_107 Depth 3
                                        //     Child Loop BB8_110 Depth 2
                                        //       Child Loop BB8_111 Depth 3
                                        //     Child Loop BB8_114 Depth 2
                                        //       Child Loop BB8_115 Depth 3
                                        //     Child Loop BB8_118 Depth 2
                                        //       Child Loop BB8_119 Depth 3
                                        //     Child Loop BB8_122 Depth 2
                                        //       Child Loop BB8_123 Depth 3
                                        //     Child Loop BB8_126 Depth 2
                                        //       Child Loop BB8_127 Depth 3
                                        //     Child Loop BB8_130 Depth 2
                                        //       Child Loop BB8_131 Depth 3
                                        //     Child Loop BB8_134 Depth 2
                                        //       Child Loop BB8_135 Depth 3
                                        //     Child Loop BB8_138 Depth 2
                                        //       Child Loop BB8_139 Depth 3
                                        //     Child Loop BB8_142 Depth 2
                                        //       Child Loop BB8_143 Depth 3
                                        //     Child Loop BB8_146 Depth 2
                                        //       Child Loop BB8_147 Depth 3
                                        //     Child Loop BB8_150 Depth 2
                                        //       Child Loop BB8_151 Depth 3
                                        //     Child Loop BB8_154 Depth 2
                                        //       Child Loop BB8_155 Depth 3
                                        //     Child Loop BB8_158 Depth 2
                                        //       Child Loop BB8_159 Depth 3
                                        //     Child Loop BB8_162 Depth 2
                                        //       Child Loop BB8_163 Depth 3
                                        //     Child Loop BB8_166 Depth 2
                                        //       Child Loop BB8_167 Depth 3
                                        //     Child Loop BB8_170 Depth 2
                                        //       Child Loop BB8_171 Depth 3
                                        //     Child Loop BB8_174 Depth 2
                                        //       Child Loop BB8_175 Depth 3
                                        //     Child Loop BB8_178 Depth 2
                                        //       Child Loop BB8_179 Depth 3
                                        //     Child Loop BB8_182 Depth 2
                                        //       Child Loop BB8_183 Depth 3
                                        //     Child Loop BB8_186 Depth 2
                                        //       Child Loop BB8_187 Depth 3
                                        //     Child Loop BB8_190 Depth 2
                                        //       Child Loop BB8_191 Depth 3
                                        //     Child Loop BB8_194 Depth 2
                                        //       Child Loop BB8_195 Depth 3
                                        //     Child Loop BB8_198 Depth 2
                                        //       Child Loop BB8_199 Depth 3
                                        //     Child Loop BB8_202 Depth 2
                                        //       Child Loop BB8_203 Depth 3
                                        //     Child Loop BB8_206 Depth 2
                                        //       Child Loop BB8_207 Depth 3
                                        //     Child Loop BB8_210 Depth 2
                                        //       Child Loop BB8_211 Depth 3
                                        //     Child Loop BB8_214 Depth 2
                                        //       Child Loop BB8_215 Depth 3
                                        //     Child Loop BB8_218 Depth 2
                                        //       Child Loop BB8_219 Depth 3
                                        //     Child Loop BB8_222 Depth 2
                                        //       Child Loop BB8_223 Depth 3
                                        //     Child Loop BB8_226 Depth 2
                                        //       Child Loop BB8_227 Depth 3
                                        //     Child Loop BB8_230 Depth 2
                                        //       Child Loop BB8_231 Depth 3
                                        //     Child Loop BB8_234 Depth 2
                                        //       Child Loop BB8_235 Depth 3
                                        //     Child Loop BB8_238 Depth 2
                                        //       Child Loop BB8_239 Depth 3
                                        //     Child Loop BB8_242 Depth 2
                                        //       Child Loop BB8_243 Depth 3
                                        //     Child Loop BB8_246 Depth 2
                                        //       Child Loop BB8_247 Depth 3
.Ltmp32:
	add	x0, sp, #152            // =152
	bl	_ZN11LinuxEventsILi0EE5startEv
.Ltmp33:
// %bb.29:                              //   in Loop: Header=BB8_28 Depth=1
	orr	w2, wzr, #0x200
	mov	x0, x27
	mov	x1, x26
	bl	memcpy
	cbz	x22, .LBB8_34
// %bb.30:                              //   in Loop: Header=BB8_28 Depth=1
	mov	x8, xzr
	mov	w25, wzr
.LBB8_31:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldr	x9, [x20, x8, lsl #3]
	cbz	x9, .LBB8_33
// %bb.32:                              //   in Loop: Header=BB8_31 Depth=2
	fmov	d0, x9
	cnt	v0.8b, v0.8b
	uaddlv	h0, v0.8b
	fmov	w10, s0
	lsl	x11, x10, #3
	ldr	x12, [x23, x11]
	stp	w8, w25, [x12, #8]
	str	x9, [x12]
	add	x9, x12, #16            // =16
	add	w25, w25, w10
	str	x9, [x23, x11]
.LBB8_33:                               //   in Loop: Header=BB8_31 Depth=2
	add	x8, x8, #1              // =1
	cmp	x22, x8
	b.ne	.LBB8_31
	b	.LBB8_35
.LBB8_34:                               //   in Loop: Header=BB8_28 Depth=1
	mov	w25, wzr
.LBB8_35:                               //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x26]
	ldr	x9, [x27]
	cmp	x8, x9
	b.eq	.LBB8_37
.LBB8_36:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldr	x10, [x8]
	ldp	w11, w12, [x8, #8]
	add	x8, x8, #16             // =16
	cmp	x8, x9
	rbit	x10, x10
	clz	x10, x10
	add	w10, w11, w10
	str	w10, [x21, x12, lsl #2]
	b.ne	.LBB8_36
.LBB8_37:                               //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #16]
	ldr	x9, [x23, #16]
	cmp	x8, x9
	b.eq	.LBB8_39
.LBB8_38:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldr	x11, [x8]
	ldp	w12, w10, [x8, #8]
	sub	x13, x11, #1            // =1
	and	x13, x13, x11
	rbit	x11, x11
	clz	x11, x11
	add	x10, x21, x10, lsl #2
	add	w11, w12, w11
	str	w11, [x10]
	ldr	w11, [x8, #8]
	rbit	x12, x13
	clz	x12, x12
	add	x8, x8, #16             // =16
	add	w11, w11, w12
	cmp	x8, x9
	str	w11, [x10, #4]
	b.ne	.LBB8_38
.LBB8_39:                               //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #24]
	ldr	x9, [x23, #24]
	cmp	x8, x9
	b.eq	.LBB8_41
.LBB8_40:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldr	x11, [x8]
	ldp	w12, w10, [x8, #8]
	sub	x13, x11, #1            // =1
	and	x13, x13, x11
	rbit	x11, x11
	clz	x11, x11
	add	x10, x21, x10, lsl #2
	add	w11, w12, w11
	str	w11, [x10]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #4]
	ldr	w11, [x8, #8]
	rbit	x12, x12
	clz	x12, x12
	add	x8, x8, #16             // =16
	add	w11, w11, w12
	cmp	x8, x9
	str	w11, [x10, #8]
	b.ne	.LBB8_40
.LBB8_41:                               //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #32]
	ldr	x9, [x23, #32]
	cmp	x8, x9
	b.eq	.LBB8_43
.LBB8_42:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldr	x11, [x8]
	ldp	w12, w10, [x8, #8]
	sub	x13, x11, #1            // =1
	and	x13, x13, x11
	rbit	x11, x11
	clz	x11, x11
	add	x10, x21, x10, lsl #2
	add	w11, w12, w11
	str	w11, [x10]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #4]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #8]
	ldr	w11, [x8, #8]
	rbit	x12, x13
	clz	x12, x12
	add	x8, x8, #16             // =16
	add	w11, w11, w12
	cmp	x8, x9
	str	w11, [x10, #12]
	b.ne	.LBB8_42
.LBB8_43:                               //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #40]
	ldr	x9, [x23, #40]
	cmp	x8, x9
	b.eq	.LBB8_45
.LBB8_44:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldr	x11, [x8]
	ldp	w12, w10, [x8, #8]
	sub	x13, x11, #1            // =1
	and	x13, x13, x11
	rbit	x11, x11
	clz	x11, x11
	add	x10, x21, x10, lsl #2
	add	w11, w12, w11
	str	w11, [x10]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #4]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #8]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #12]
	ldr	w11, [x8, #8]
	rbit	x12, x12
	clz	x12, x12
	add	x8, x8, #16             // =16
	add	w11, w11, w12
	cmp	x8, x9
	str	w11, [x10, #16]
	b.ne	.LBB8_44
.LBB8_45:                               //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #48]
	ldr	x9, [x23, #48]
	cmp	x8, x9
	b.eq	.LBB8_47
.LBB8_46:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldr	x11, [x8]
	ldp	w12, w10, [x8, #8]
	sub	x13, x11, #1            // =1
	and	x13, x13, x11
	rbit	x11, x11
	clz	x11, x11
	add	x10, x21, x10, lsl #2
	add	w11, w12, w11
	str	w11, [x10]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #4]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #8]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #12]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #16]
	ldr	w11, [x8, #8]
	rbit	x12, x13
	clz	x12, x12
	add	x8, x8, #16             // =16
	add	w11, w11, w12
	cmp	x8, x9
	str	w11, [x10, #20]
	b.ne	.LBB8_46
.LBB8_47:                               //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #56]
	ldr	x9, [x23, #56]
	cmp	x8, x9
	b.eq	.LBB8_49
.LBB8_48:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldr	x11, [x8]
	ldp	w12, w10, [x8, #8]
	sub	x13, x11, #1            // =1
	and	x13, x13, x11
	rbit	x11, x11
	clz	x11, x11
	add	x10, x21, x10, lsl #2
	add	w11, w12, w11
	str	w11, [x10]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #4]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #8]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #12]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #16]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #20]
	ldr	w11, [x8, #8]
	rbit	x12, x12
	clz	x12, x12
	add	x8, x8, #16             // =16
	add	w11, w11, w12
	cmp	x8, x9
	str	w11, [x10, #24]
	b.ne	.LBB8_48
.LBB8_49:                               //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #64]
	ldr	x9, [x23, #64]
	cmp	x8, x9
	b.eq	.LBB8_51
.LBB8_50:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldr	x11, [x8]
	ldp	w12, w10, [x8, #8]
	sub	x13, x11, #1            // =1
	and	x13, x13, x11
	rbit	x11, x11
	clz	x11, x11
	add	x10, x21, x10, lsl #2
	add	w11, w12, w11
	str	w11, [x10]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #4]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #8]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #12]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #16]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #20]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #24]
	ldr	w11, [x8, #8]
	rbit	x12, x13
	clz	x12, x12
	add	x8, x8, #16             // =16
	add	w11, w11, w12
	cmp	x8, x9
	str	w11, [x10, #28]
	b.ne	.LBB8_50
.LBB8_51:                               //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #72]
	ldr	x9, [x23, #72]
	cmp	x8, x9
	b.eq	.LBB8_53
.LBB8_52:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldr	x11, [x8]
	ldp	w12, w10, [x8, #8]
	sub	x13, x11, #1            // =1
	and	x13, x13, x11
	rbit	x11, x11
	clz	x11, x11
	add	x10, x21, x10, lsl #2
	add	w11, w12, w11
	str	w11, [x10]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #4]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #8]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #12]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #16]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #20]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #24]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #28]
	ldr	w11, [x8, #8]
	rbit	x12, x12
	clz	x12, x12
	add	x8, x8, #16             // =16
	add	w11, w11, w12
	cmp	x8, x9
	str	w11, [x10, #32]
	b.ne	.LBB8_52
.LBB8_53:                               //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #80]
	ldr	x9, [x23, #80]
	cmp	x8, x9
	b.eq	.LBB8_55
.LBB8_54:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldr	x11, [x8]
	ldp	w12, w10, [x8, #8]
	sub	x13, x11, #1            // =1
	and	x13, x13, x11
	rbit	x11, x11
	clz	x11, x11
	add	x10, x21, x10, lsl #2
	add	w11, w12, w11
	str	w11, [x10]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #4]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #8]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #12]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #16]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #20]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #24]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #28]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #32]
	ldr	w11, [x8, #8]
	rbit	x12, x13
	clz	x12, x12
	add	x8, x8, #16             // =16
	add	w11, w11, w12
	cmp	x8, x9
	str	w11, [x10, #36]
	b.ne	.LBB8_54
.LBB8_55:                               //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #88]
	ldr	x9, [x23, #88]
	cmp	x8, x9
	b.eq	.LBB8_57
.LBB8_56:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldr	x11, [x8]
	ldp	w12, w10, [x8, #8]
	sub	x13, x11, #1            // =1
	and	x13, x13, x11
	rbit	x11, x11
	clz	x11, x11
	add	x10, x21, x10, lsl #2
	add	w11, w12, w11
	str	w11, [x10]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #4]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #8]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #12]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #16]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #20]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #24]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #28]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #32]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #36]
	ldr	w11, [x8, #8]
	rbit	x12, x12
	clz	x12, x12
	add	x8, x8, #16             // =16
	add	w11, w11, w12
	cmp	x8, x9
	str	w11, [x10, #40]
	b.ne	.LBB8_56
.LBB8_57:                               //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #96]
	ldr	x9, [x23, #96]
	cmp	x8, x9
	b.eq	.LBB8_59
.LBB8_58:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldr	x11, [x8]
	ldp	w12, w10, [x8, #8]
	sub	x13, x11, #1            // =1
	and	x13, x13, x11
	rbit	x11, x11
	clz	x11, x11
	add	x10, x21, x10, lsl #2
	add	w11, w12, w11
	str	w11, [x10]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #4]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #8]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #12]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #16]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #20]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #24]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #28]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #32]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #36]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #40]
	ldr	w11, [x8, #8]
	rbit	x12, x13
	clz	x12, x12
	add	x8, x8, #16             // =16
	add	w11, w11, w12
	cmp	x8, x9
	str	w11, [x10, #44]
	b.ne	.LBB8_58
.LBB8_59:                               //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #104]
	ldr	x9, [x23, #104]
	cmp	x8, x9
	b.eq	.LBB8_61
.LBB8_60:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldr	x11, [x8]
	ldp	w12, w10, [x8, #8]
	sub	x13, x11, #1            // =1
	and	x13, x13, x11
	rbit	x11, x11
	clz	x11, x11
	add	x10, x21, x10, lsl #2
	add	w11, w12, w11
	str	w11, [x10]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #4]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #8]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #12]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #16]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #20]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #24]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #28]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #32]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #36]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #40]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #44]
	ldr	w11, [x8, #8]
	rbit	x12, x12
	clz	x12, x12
	add	x8, x8, #16             // =16
	add	w11, w11, w12
	cmp	x8, x9
	str	w11, [x10, #48]
	b.ne	.LBB8_60
.LBB8_61:                               //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #112]
	ldr	x9, [x23, #112]
	cmp	x8, x9
	b.eq	.LBB8_63
.LBB8_62:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldr	x11, [x8]
	ldp	w12, w10, [x8, #8]
	sub	x13, x11, #1            // =1
	and	x13, x13, x11
	rbit	x11, x11
	clz	x11, x11
	add	x10, x21, x10, lsl #2
	add	w11, w12, w11
	str	w11, [x10]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #4]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #8]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #12]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #16]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #20]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #24]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #28]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #32]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #36]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #40]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #44]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #48]
	ldr	w11, [x8, #8]
	rbit	x12, x13
	clz	x12, x12
	add	x8, x8, #16             // =16
	add	w11, w11, w12
	cmp	x8, x9
	str	w11, [x10, #52]
	b.ne	.LBB8_62
.LBB8_63:                               //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #120]
	ldr	x9, [x23, #120]
	cmp	x8, x9
	b.eq	.LBB8_65
.LBB8_64:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldr	x11, [x8]
	ldp	w12, w10, [x8, #8]
	sub	x13, x11, #1            // =1
	and	x13, x13, x11
	rbit	x11, x11
	clz	x11, x11
	add	x10, x21, x10, lsl #2
	add	w11, w12, w11
	str	w11, [x10]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #4]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #8]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #12]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #16]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #20]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #24]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #28]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #32]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #36]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #40]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #44]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #48]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #52]
	ldr	w11, [x8, #8]
	rbit	x12, x12
	clz	x12, x12
	add	x8, x8, #16             // =16
	add	w11, w11, w12
	cmp	x8, x9
	str	w11, [x10, #56]
	b.ne	.LBB8_64
.LBB8_65:                               //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #128]
	ldr	x9, [x23, #128]
	cmp	x8, x9
	b.eq	.LBB8_67
.LBB8_66:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldr	x11, [x8]
	ldp	w12, w10, [x8, #8]
	sub	x13, x11, #1            // =1
	and	x13, x13, x11
	rbit	x11, x11
	clz	x11, x11
	add	x10, x21, x10, lsl #2
	add	w11, w12, w11
	str	w11, [x10]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #4]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #8]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #12]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #16]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #20]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #24]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #28]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #32]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #36]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #40]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #44]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #48]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #52]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #56]
	ldr	w11, [x8, #8]
	rbit	x12, x13
	clz	x12, x12
	add	x8, x8, #16             // =16
	add	w11, w11, w12
	cmp	x8, x9
	str	w11, [x10, #60]
	b.ne	.LBB8_66
.LBB8_67:                               //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #136]
	ldr	x9, [x23, #136]
	cmp	x8, x9
	b.eq	.LBB8_69
.LBB8_68:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldr	x11, [x8]
	ldp	w12, w10, [x8, #8]
	sub	x13, x11, #1            // =1
	and	x13, x13, x11
	rbit	x11, x11
	clz	x11, x11
	add	x10, x21, x10, lsl #2
	add	w11, w12, w11
	str	w11, [x10]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #4]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #8]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #12]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #16]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #20]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #24]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #28]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #32]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #36]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #40]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #44]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #48]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #52]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #56]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #60]
	ldr	w11, [x8, #8]
	rbit	x12, x12
	clz	x12, x12
	add	x8, x8, #16             // =16
	add	w11, w11, w12
	cmp	x8, x9
	str	w11, [x10, #64]
	b.ne	.LBB8_68
.LBB8_69:                               //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #144]
	ldr	x9, [x23, #144]
	cmp	x8, x9
	b.eq	.LBB8_71
.LBB8_70:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldr	x11, [x8]
	ldp	w12, w10, [x8, #8]
	sub	x13, x11, #1            // =1
	and	x13, x13, x11
	rbit	x11, x11
	clz	x11, x11
	add	x10, x21, x10, lsl #2
	add	w11, w12, w11
	str	w11, [x10]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #4]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #8]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #12]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #16]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #20]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #24]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #28]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #32]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #36]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #40]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #44]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #48]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #52]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #56]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #60]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #64]
	ldr	w11, [x8, #8]
	rbit	x12, x13
	clz	x12, x12
	add	x8, x8, #16             // =16
	add	w11, w11, w12
	cmp	x8, x9
	str	w11, [x10, #68]
	b.ne	.LBB8_70
.LBB8_71:                               //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #152]
	ldr	x9, [x23, #152]
	cmp	x8, x9
	b.eq	.LBB8_73
.LBB8_72:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldr	x11, [x8]
	ldp	w12, w10, [x8, #8]
	sub	x13, x11, #1            // =1
	and	x13, x13, x11
	rbit	x11, x11
	clz	x11, x11
	add	x10, x21, x10, lsl #2
	add	w11, w12, w11
	str	w11, [x10]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #4]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #8]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #12]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #16]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #20]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #24]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #28]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #32]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #36]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #40]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #44]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #48]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #52]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #56]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #60]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #64]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #68]
	ldr	w11, [x8, #8]
	rbit	x12, x12
	clz	x12, x12
	add	x8, x8, #16             // =16
	add	w11, w11, w12
	cmp	x8, x9
	str	w11, [x10, #72]
	b.ne	.LBB8_72
.LBB8_73:                               //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #160]
	ldr	x9, [x23, #160]
	cmp	x8, x9
	b.eq	.LBB8_75
.LBB8_74:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldr	x11, [x8]
	ldp	w12, w10, [x8, #8]
	sub	x13, x11, #1            // =1
	and	x13, x13, x11
	rbit	x11, x11
	clz	x11, x11
	add	x10, x21, x10, lsl #2
	add	w11, w12, w11
	str	w11, [x10]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #4]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #8]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #12]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #16]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #20]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #24]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #28]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #32]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #36]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #40]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #44]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #48]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #52]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #56]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #60]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #64]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #68]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #72]
	ldr	w11, [x8, #8]
	rbit	x12, x13
	clz	x12, x12
	add	x8, x8, #16             // =16
	add	w11, w11, w12
	cmp	x8, x9
	str	w11, [x10, #76]
	b.ne	.LBB8_74
.LBB8_75:                               //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #168]
	ldr	x9, [x23, #168]
	cmp	x8, x9
	b.eq	.LBB8_77
.LBB8_76:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldr	x11, [x8]
	ldp	w12, w10, [x8, #8]
	sub	x13, x11, #1            // =1
	and	x13, x13, x11
	rbit	x11, x11
	clz	x11, x11
	add	x10, x21, x10, lsl #2
	add	w11, w12, w11
	str	w11, [x10]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #4]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #8]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #12]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #16]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #20]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #24]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #28]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #32]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #36]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #40]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #44]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #48]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #52]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #56]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #60]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #64]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #68]
	ldr	w11, [x8, #8]
	sub	x13, x12, #1            // =1
	and	x13, x13, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w11, w12
	str	w11, [x10, #72]
	ldr	w11, [x8, #8]
	sub	x12, x13, #1            // =1
	and	x12, x12, x13
	rbit	x13, x13
	clz	x13, x13
	add	w11, w11, w13
	str	w11, [x10, #76]
	ldr	w11, [x8, #8]
	rbit	x12, x12
	clz	x12, x12
	add	x8, x8, #16             // =16
	add	w11, w11, w12
	cmp	x8, x9
	str	w11, [x10, #80]
	b.ne	.LBB8_76
.LBB8_77:                               //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #176]
	ldr	x9, [x23, #176]
	cmp	x8, x9
	b.eq	.LBB8_81
.LBB8_78:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_79 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_79:                               //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_78 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #88                // =88
	and	x11, x15, x11
	b.ne	.LBB8_79
// %bb.80:                              //   in Loop: Header=BB8_78 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_78
.LBB8_81:                               //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #184]
	ldr	x9, [x23, #184]
	cmp	x8, x9
	b.eq	.LBB8_85
.LBB8_82:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_83 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_83:                               //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_82 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #92                // =92
	and	x11, x15, x11
	b.ne	.LBB8_83
// %bb.84:                              //   in Loop: Header=BB8_82 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_82
.LBB8_85:                               //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #192]
	ldr	x9, [x23, #192]
	cmp	x8, x9
	b.eq	.LBB8_89
.LBB8_86:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_87 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_87:                               //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_86 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #96                // =96
	and	x11, x15, x11
	b.ne	.LBB8_87
// %bb.88:                              //   in Loop: Header=BB8_86 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_86
.LBB8_89:                               //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #200]
	ldr	x9, [x23, #200]
	cmp	x8, x9
	b.eq	.LBB8_93
.LBB8_90:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_91 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_91:                               //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_90 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #100               // =100
	and	x11, x15, x11
	b.ne	.LBB8_91
// %bb.92:                              //   in Loop: Header=BB8_90 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_90
.LBB8_93:                               //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #208]
	ldr	x9, [x23, #208]
	cmp	x8, x9
	b.eq	.LBB8_97
.LBB8_94:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_95 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_95:                               //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_94 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #104               // =104
	and	x11, x15, x11
	b.ne	.LBB8_95
// %bb.96:                              //   in Loop: Header=BB8_94 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_94
.LBB8_97:                               //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #216]
	ldr	x9, [x23, #216]
	cmp	x8, x9
	b.eq	.LBB8_101
.LBB8_98:                               //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_99 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_99:                               //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_98 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #108               // =108
	and	x11, x15, x11
	b.ne	.LBB8_99
// %bb.100:                             //   in Loop: Header=BB8_98 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_98
.LBB8_101:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #224]
	ldr	x9, [x23, #224]
	cmp	x8, x9
	b.eq	.LBB8_105
.LBB8_102:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_103 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_103:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_102 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #112               // =112
	and	x11, x15, x11
	b.ne	.LBB8_103
// %bb.104:                             //   in Loop: Header=BB8_102 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_102
.LBB8_105:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #232]
	ldr	x9, [x23, #232]
	cmp	x8, x9
	b.eq	.LBB8_109
.LBB8_106:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_107 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_107:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_106 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #116               // =116
	and	x11, x15, x11
	b.ne	.LBB8_107
// %bb.108:                             //   in Loop: Header=BB8_106 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_106
.LBB8_109:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #240]
	ldr	x9, [x23, #240]
	cmp	x8, x9
	b.eq	.LBB8_113
.LBB8_110:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_111 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_111:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_110 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #120               // =120
	and	x11, x15, x11
	b.ne	.LBB8_111
// %bb.112:                             //   in Loop: Header=BB8_110 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_110
.LBB8_113:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #248]
	ldr	x9, [x23, #248]
	cmp	x8, x9
	b.eq	.LBB8_117
.LBB8_114:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_115 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_115:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_114 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #124               // =124
	and	x11, x15, x11
	b.ne	.LBB8_115
// %bb.116:                             //   in Loop: Header=BB8_114 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_114
.LBB8_117:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #256]
	ldr	x9, [x23, #256]
	cmp	x8, x9
	b.eq	.LBB8_121
.LBB8_118:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_119 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_119:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_118 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #128               // =128
	and	x11, x15, x11
	b.ne	.LBB8_119
// %bb.120:                             //   in Loop: Header=BB8_118 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_118
.LBB8_121:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #264]
	ldr	x9, [x23, #264]
	cmp	x8, x9
	b.eq	.LBB8_125
.LBB8_122:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_123 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_123:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_122 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #132               // =132
	and	x11, x15, x11
	b.ne	.LBB8_123
// %bb.124:                             //   in Loop: Header=BB8_122 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_122
.LBB8_125:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #272]
	ldr	x9, [x23, #272]
	cmp	x8, x9
	b.eq	.LBB8_129
.LBB8_126:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_127 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_127:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_126 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #136               // =136
	and	x11, x15, x11
	b.ne	.LBB8_127
// %bb.128:                             //   in Loop: Header=BB8_126 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_126
.LBB8_129:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #280]
	ldr	x9, [x23, #280]
	cmp	x8, x9
	b.eq	.LBB8_133
.LBB8_130:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_131 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_131:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_130 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #140               // =140
	and	x11, x15, x11
	b.ne	.LBB8_131
// %bb.132:                             //   in Loop: Header=BB8_130 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_130
.LBB8_133:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #288]
	ldr	x9, [x23, #288]
	cmp	x8, x9
	b.eq	.LBB8_137
.LBB8_134:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_135 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_135:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_134 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #144               // =144
	and	x11, x15, x11
	b.ne	.LBB8_135
// %bb.136:                             //   in Loop: Header=BB8_134 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_134
.LBB8_137:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #296]
	ldr	x9, [x23, #296]
	cmp	x8, x9
	b.eq	.LBB8_141
.LBB8_138:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_139 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_139:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_138 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #148               // =148
	and	x11, x15, x11
	b.ne	.LBB8_139
// %bb.140:                             //   in Loop: Header=BB8_138 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_138
.LBB8_141:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #304]
	ldr	x9, [x23, #304]
	cmp	x8, x9
	b.eq	.LBB8_145
.LBB8_142:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_143 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_143:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_142 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #152               // =152
	and	x11, x15, x11
	b.ne	.LBB8_143
// %bb.144:                             //   in Loop: Header=BB8_142 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_142
.LBB8_145:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #312]
	ldr	x9, [x23, #312]
	cmp	x8, x9
	b.eq	.LBB8_149
.LBB8_146:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_147 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_147:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_146 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #156               // =156
	and	x11, x15, x11
	b.ne	.LBB8_147
// %bb.148:                             //   in Loop: Header=BB8_146 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_146
.LBB8_149:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #320]
	ldr	x9, [x23, #320]
	cmp	x8, x9
	b.eq	.LBB8_153
.LBB8_150:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_151 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_151:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_150 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #160               // =160
	and	x11, x15, x11
	b.ne	.LBB8_151
// %bb.152:                             //   in Loop: Header=BB8_150 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_150
.LBB8_153:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #328]
	ldr	x9, [x23, #328]
	cmp	x8, x9
	b.eq	.LBB8_157
.LBB8_154:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_155 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_155:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_154 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #164               // =164
	and	x11, x15, x11
	b.ne	.LBB8_155
// %bb.156:                             //   in Loop: Header=BB8_154 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_154
.LBB8_157:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #336]
	ldr	x9, [x23, #336]
	cmp	x8, x9
	b.eq	.LBB8_161
.LBB8_158:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_159 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_159:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_158 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #168               // =168
	and	x11, x15, x11
	b.ne	.LBB8_159
// %bb.160:                             //   in Loop: Header=BB8_158 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_158
.LBB8_161:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #344]
	ldr	x9, [x23, #344]
	cmp	x8, x9
	b.eq	.LBB8_165
.LBB8_162:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_163 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_163:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_162 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #172               // =172
	and	x11, x15, x11
	b.ne	.LBB8_163
// %bb.164:                             //   in Loop: Header=BB8_162 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_162
.LBB8_165:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #352]
	ldr	x9, [x23, #352]
	cmp	x8, x9
	b.eq	.LBB8_169
.LBB8_166:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_167 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_167:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_166 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #176               // =176
	and	x11, x15, x11
	b.ne	.LBB8_167
// %bb.168:                             //   in Loop: Header=BB8_166 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_166
.LBB8_169:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #360]
	ldr	x9, [x23, #360]
	cmp	x8, x9
	b.eq	.LBB8_173
.LBB8_170:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_171 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_171:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_170 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #180               // =180
	and	x11, x15, x11
	b.ne	.LBB8_171
// %bb.172:                             //   in Loop: Header=BB8_170 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_170
.LBB8_173:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #368]
	ldr	x9, [x23, #368]
	cmp	x8, x9
	b.eq	.LBB8_177
.LBB8_174:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_175 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_175:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_174 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #184               // =184
	and	x11, x15, x11
	b.ne	.LBB8_175
// %bb.176:                             //   in Loop: Header=BB8_174 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_174
.LBB8_177:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #376]
	ldr	x9, [x23, #376]
	cmp	x8, x9
	b.eq	.LBB8_181
.LBB8_178:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_179 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_179:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_178 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #188               // =188
	and	x11, x15, x11
	b.ne	.LBB8_179
// %bb.180:                             //   in Loop: Header=BB8_178 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_178
.LBB8_181:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #384]
	ldr	x9, [x23, #384]
	cmp	x8, x9
	b.eq	.LBB8_185
.LBB8_182:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_183 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_183:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_182 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #192               // =192
	and	x11, x15, x11
	b.ne	.LBB8_183
// %bb.184:                             //   in Loop: Header=BB8_182 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_182
.LBB8_185:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #392]
	ldr	x9, [x23, #392]
	cmp	x8, x9
	b.eq	.LBB8_189
.LBB8_186:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_187 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_187:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_186 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #196               // =196
	and	x11, x15, x11
	b.ne	.LBB8_187
// %bb.188:                             //   in Loop: Header=BB8_186 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_186
.LBB8_189:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #400]
	ldr	x9, [x23, #400]
	cmp	x8, x9
	b.eq	.LBB8_193
.LBB8_190:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_191 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_191:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_190 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #200               // =200
	and	x11, x15, x11
	b.ne	.LBB8_191
// %bb.192:                             //   in Loop: Header=BB8_190 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_190
.LBB8_193:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #408]
	ldr	x9, [x23, #408]
	cmp	x8, x9
	b.eq	.LBB8_197
.LBB8_194:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_195 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_195:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_194 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #204               // =204
	and	x11, x15, x11
	b.ne	.LBB8_195
// %bb.196:                             //   in Loop: Header=BB8_194 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_194
.LBB8_197:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #416]
	ldr	x9, [x23, #416]
	cmp	x8, x9
	b.eq	.LBB8_201
.LBB8_198:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_199 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_199:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_198 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #208               // =208
	and	x11, x15, x11
	b.ne	.LBB8_199
// %bb.200:                             //   in Loop: Header=BB8_198 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_198
.LBB8_201:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #424]
	ldr	x9, [x23, #424]
	cmp	x8, x9
	b.eq	.LBB8_205
.LBB8_202:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_203 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_203:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_202 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #212               // =212
	and	x11, x15, x11
	b.ne	.LBB8_203
// %bb.204:                             //   in Loop: Header=BB8_202 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_202
.LBB8_205:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #432]
	ldr	x9, [x23, #432]
	cmp	x8, x9
	b.eq	.LBB8_209
.LBB8_206:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_207 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_207:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_206 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #216               // =216
	and	x11, x15, x11
	b.ne	.LBB8_207
// %bb.208:                             //   in Loop: Header=BB8_206 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_206
.LBB8_209:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #440]
	ldr	x9, [x23, #440]
	cmp	x8, x9
	b.eq	.LBB8_213
.LBB8_210:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_211 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_211:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_210 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #220               // =220
	and	x11, x15, x11
	b.ne	.LBB8_211
// %bb.212:                             //   in Loop: Header=BB8_210 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_210
.LBB8_213:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #448]
	ldr	x9, [x23, #448]
	cmp	x8, x9
	b.eq	.LBB8_217
.LBB8_214:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_215 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_215:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_214 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #224               // =224
	and	x11, x15, x11
	b.ne	.LBB8_215
// %bb.216:                             //   in Loop: Header=BB8_214 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_214
.LBB8_217:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #456]
	ldr	x9, [x23, #456]
	cmp	x8, x9
	b.eq	.LBB8_221
.LBB8_218:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_219 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_219:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_218 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #228               // =228
	and	x11, x15, x11
	b.ne	.LBB8_219
// %bb.220:                             //   in Loop: Header=BB8_218 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_218
.LBB8_221:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #464]
	ldr	x9, [x23, #464]
	cmp	x8, x9
	b.eq	.LBB8_225
.LBB8_222:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_223 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_223:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_222 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #232               // =232
	and	x11, x15, x11
	b.ne	.LBB8_223
// %bb.224:                             //   in Loop: Header=BB8_222 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_222
.LBB8_225:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #472]
	ldr	x9, [x23, #472]
	cmp	x8, x9
	b.eq	.LBB8_229
.LBB8_226:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_227 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_227:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_226 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #236               // =236
	and	x11, x15, x11
	b.ne	.LBB8_227
// %bb.228:                             //   in Loop: Header=BB8_226 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_226
.LBB8_229:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #480]
	ldr	x9, [x23, #480]
	cmp	x8, x9
	b.eq	.LBB8_233
.LBB8_230:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_231 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_231:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_230 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #240               // =240
	and	x11, x15, x11
	b.ne	.LBB8_231
// %bb.232:                             //   in Loop: Header=BB8_230 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_230
.LBB8_233:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #488]
	ldr	x9, [x23, #488]
	cmp	x8, x9
	b.eq	.LBB8_237
.LBB8_234:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_235 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_235:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_234 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #244               // =244
	and	x11, x15, x11
	b.ne	.LBB8_235
// %bb.236:                             //   in Loop: Header=BB8_234 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_234
.LBB8_237:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #496]
	ldr	x9, [x23, #496]
	cmp	x8, x9
	b.eq	.LBB8_241
.LBB8_238:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_239 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_239:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_238 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #248               // =248
	and	x11, x15, x11
	b.ne	.LBB8_239
// %bb.240:                             //   in Loop: Header=BB8_238 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_238
.LBB8_241:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #504]
	ldr	x9, [x23, #504]
	cmp	x8, x9
	b.eq	.LBB8_245
.LBB8_242:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_243 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_243:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_242 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #252               // =252
	and	x11, x15, x11
	b.ne	.LBB8_243
// %bb.244:                             //   in Loop: Header=BB8_242 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_242
.LBB8_245:                              //   in Loop: Header=BB8_28 Depth=1
	ldr	x8, [x24, #512]
	ldr	x9, [x23, #512]
	cmp	x8, x9
	b.eq	.LBB8_249
.LBB8_246:                              //   Parent Loop BB8_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB8_247 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB8_247:                              //   Parent Loop BB8_28 Depth=1
                                        //     Parent Loop BB8_246 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	ldr	w13, [x8, #8]
	rbit	x14, x11
	clz	x14, x14
	sub	x15, x11, #1            // =1
	add	w13, w13, w14
	str	w13, [x12, x10]
	add	x10, x10, #4            // =4
	cmp	x10, #256               // =256
	and	x11, x15, x11
	b.ne	.LBB8_247
// %bb.248:                             //   in Loop: Header=BB8_246 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB8_246
.LBB8_249:                              //   in Loop: Header=BB8_28 Depth=1
.Ltmp34:
	add	x0, sp, #152            // =152
	add	x1, sp, #104            // =104
	bl	_ZN11LinuxEventsILi0EE3endERSt6vectorIySaIyEE
.Ltmp35:
// %bb.250:                             //   in Loop: Header=BB8_28 Depth=1
	ldp	x28, x8, [sp, #88]
	cmp	x28, x8
	b.eq	.LBB8_255
// %bb.251:                             //   in Loop: Header=BB8_28 Depth=1
	ldp	x9, x8, [sp, #104]
	stp	xzr, xzr, [x28]
	str	xzr, [x28, #16]
	subs	x0, x8, x9
	asr	x26, x0, #3
	b.eq	.LBB8_256
// %bb.252:                             //   in Loop: Header=BB8_28 Depth=1
	lsr	x8, x26, #61
	cbnz	x8, .LBB8_347
// %bb.253:                             //   in Loop: Header=BB8_28 Depth=1
.Ltmp36:
	bl	_Znwm
.Ltmp37:
// %bb.254:                             //   in Loop: Header=BB8_28 Depth=1
	mov	x27, x0
	b	.LBB8_257
.LBB8_255:                              //   in Loop: Header=BB8_28 Depth=1
.Ltmp41:
	add	x0, sp, #80             // =80
	add	x2, sp, #104            // =104
	mov	x1, x28
	bl	_ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_
.Ltmp42:
	b	.LBB8_260
.LBB8_256:                              //   in Loop: Header=BB8_28 Depth=1
	mov	x27, xzr
.LBB8_257:                              //   in Loop: Header=BB8_28 Depth=1
	add	x8, x27, x26, lsl #3
	stp	x27, x0, [x28]
	str	x8, [x28, #16]
	ldp	x1, x8, [sp, #104]
	subs	x2, x8, x1
	asr	x26, x2, #3
	b.eq	.LBB8_259
// %bb.258:                             //   in Loop: Header=BB8_28 Depth=1
	bl	memmove
.LBB8_259:                              //   in Loop: Header=BB8_28 Depth=1
	add	x8, x27, x26, lsl #3
	str	x8, [x28, #8]
	ldr	x8, [sp, #88]
	ldur	x26, [x29, #-104]       // 8-byte Folded Reload
	ldr	x27, [sp, #24]          // 8-byte Folded Reload
	add	x8, x8, #24             // =24
	str	x8, [sp, #88]
.LBB8_260:                              //   in Loop: Header=BB8_28 Depth=1
	add	w19, w19, #1            // =1
	cmp	w19, #10                // =10
	b.lo	.LBB8_28
// %bb.261:
	orr	w19, wzr, #0x8
.LBB8_262:                              // =>This Inner Loop Header: Depth=1
	ldr	x0, [x24, x19]
	cbz	x0, .LBB8_264
// %bb.263:                             //   in Loop: Header=BB8_262 Depth=1
	bl	_ZdaPv
.LBB8_264:                              //   in Loop: Header=BB8_262 Depth=1
	add	x19, x19, #8            // =8
	cmp	x19, #520               // =520
	b.ne	.LBB8_262
// %bb.265:
	mov	x0, x23
	bl	_ZdaPv
	mov	x0, x24
	bl	_ZdaPv
	ldr	w8, [x21]
	cbnz	w8, .LBB8_267
// %bb.266:
	adrp	x0, .L.str.3
	add	x0, x0, :lo12:.L.str.3
	bl	printf
.LBB8_267:
	adrp	x8, .LCPI8_0
	ldr	d0, [x8, :lo12:.LCPI8_0]
	ldr	x8, [sp, #8]            // 8-byte Folded Reload
	ucvtf	d9, w25
	adrp	x0, .L.str.4
	mov	x19, #-6148914691236517206
	ucvtf	d8, x8
	fdiv	d1, d9, d8
	fmul	d0, d1, d0
	add	x0, x0, :lo12:.L.str.4
	mov	w1, w25
	mov	x2, x22
	movk	x19, #2730, lsl #48
	bl	printf
	adrp	x8, .LCPI8_1
	ldr	d0, [x8, :lo12:.LCPI8_1]
	ucvtf	d10, x22
	adrp	x0, .L.str.5
	add	x0, x0, :lo12:.L.str.5
	fmul	d0, d10, d0
	fdiv	d0, d0, d9
	bl	printf
	ldp	x0, x1, [sp, #80]
	subs	x8, x1, x0
	b.eq	.LBB8_271
// %bb.268:
	mov	x10, #-6148914691236517206
	asr	x9, x8, #3
	movk	x10, #43691
	mul	x9, x9, x10
	cmp	x9, x19
	b.hi	.LBB8_349
// %bb.269:
.Ltmp44:
	mov	x0, x8
	bl	_Znwm
.Ltmp45:
// %bb.270:
	mov	x23, x0
	ldp	x0, x1, [sp, #80]
	mov	x24, x23
	b	.LBB8_272
.LBB8_271:
	mov	x23, xzr
	mov	x24, xzr
.LBB8_272:
.Ltmp49:
	mov	x2, x24
	bl	_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_
.Ltmp50:
// %bb.273:
	mov	x25, x0
	subs	x28, x0, x24
	b.eq	.LBB8_279
// %bb.274:
	ldp	x8, x9, [x24]
	subs	x0, x9, x8
	b.eq	.LBB8_280
// %bb.275:
	asr	x8, x0, #3
	lsr	x8, x8, #61
	cbnz	x8, .LBB8_353
// %bb.276:
.Ltmp52:
	bl	_Znwm
.Ltmp53:
// %bb.277:
	ldp	x1, x8, [x24]
	mov	x26, x0
	subs	x2, x8, x1
	b.eq	.LBB8_281
// %bb.278:
	mov	x0, x26
	asr	x27, x2, #3
	bl	memmove
	mov	w8, wzr
	b	.LBB8_282
.LBB8_279:
	mov	x22, xzr
	cbnz	x24, .LBB8_311
	b	.LBB8_312
.LBB8_280:
	mov	x22, xzr
	mov	x26, xzr
	mov	x27, xzr
	orr	w8, wzr, #0x1
	b	.LBB8_283
.LBB8_281:
	mov	x27, xzr
	orr	w8, wzr, #0x1
.LBB8_282:
	mov	x22, x26
.LBB8_283:
	mov	x10, #-6148914691236517206
	asr	x9, x28, #3
	movk	x10, #43691
	mul	x9, x9, x10
	cmp	x9, #2                  // =2
	b.lo	.LBB8_306
// %bb.284:
	cmp	x27, #1                 // =1
	csinc	x12, x27, xzr, hi
	add	x10, x22, #16           // =16
	orr	w11, wzr, #0x1
	add	x13, x22, x12, lsl #3
	and	x14, x12, #0xfffffffffffffffc
	orr	w15, wzr, #0x18
	b	.LBB8_296
.LBB8_285:                              //   in Loop: Header=BB8_296 Depth=1
	add	x17, x16, #16           // =16
	mov	x18, x10
	mov	x0, x14
.LBB8_286:                              //   Parent Loop BB8_296 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldp	q1, q0, [x17, #-16]
	ldp	q3, q2, [x18, #-16]
	cmhi	v3.2d, v3.2d, v1.2d
	xtn	v3.2s, v3.2d
	fmov	w1, s3
	tbz	w1, #0, .LBB8_288
// %bb.287:                             //   in Loop: Header=BB8_286 Depth=2
	stur	d1, [x18, #-16]
.LBB8_288:                              //   in Loop: Header=BB8_286 Depth=2
	mov	w1, v3.s[1]
	tbz	w1, #0, .LBB8_290
// %bb.289:                             //   in Loop: Header=BB8_286 Depth=2
	sub	x1, x18, #8             // =8
	st1	{ v1.d }[1], [x1]
.LBB8_290:                              //   in Loop: Header=BB8_286 Depth=2
	cmhi	v1.2d, v2.2d, v0.2d
	xtn	v1.2s, v1.2d
	fmov	w1, s1
	tbz	w1, #0, .LBB8_292
// %bb.291:                             //   in Loop: Header=BB8_286 Depth=2
	str	d0, [x18]
.LBB8_292:                              //   in Loop: Header=BB8_286 Depth=2
	mov	w1, v1.s[1]
	tbz	w1, #0, .LBB8_294
// %bb.293:                             //   in Loop: Header=BB8_286 Depth=2
	add	x1, x18, #8             // =8
	st1	{ v0.d }[1], [x1]
.LBB8_294:                              //   in Loop: Header=BB8_286 Depth=2
	subs	x0, x0, #4              // =4
	add	x18, x18, #32           // =32
	add	x17, x17, #32           // =32
	b.ne	.LBB8_286
// %bb.295:                             //   in Loop: Header=BB8_296 Depth=1
	cmp	x12, x14
	mov	x17, x14
	b.ne	.LBB8_301
	b	.LBB8_304
.LBB8_296:                              // =>This Loop Header: Depth=1
                                        //     Child Loop BB8_286 Depth 2
                                        //     Child Loop BB8_301 Depth 2
	tbnz	w8, #0, .LBB8_304
// %bb.297:                             //   in Loop: Header=BB8_296 Depth=1
	mul	x16, x11, x15
	ldr	x16, [x24, x16]
	cmp	x12, #3                 // =3
	b.ls	.LBB8_300
// %bb.298:                             //   in Loop: Header=BB8_296 Depth=1
	add	x17, x16, x12, lsl #3
	cmp	x26, x17
	b.hs	.LBB8_285
// %bb.299:                             //   in Loop: Header=BB8_296 Depth=1
	cmp	x16, x13
	b.hs	.LBB8_285
.LBB8_300:                              //   in Loop: Header=BB8_296 Depth=1
	mov	x17, xzr
.LBB8_301:                              //   Parent Loop BB8_296 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x0, x17, #3
	ldr	x18, [x16, x0]
	ldr	x0, [x22, x0]
	cmp	x18, x0
	b.hs	.LBB8_303
// %bb.302:                             //   in Loop: Header=BB8_301 Depth=2
	str	x18, [x22, x17, lsl #3]
.LBB8_303:                              //   in Loop: Header=BB8_301 Depth=2
	add	x17, x17, #1            // =1
	cmp	x17, x27
	b.lo	.LBB8_301
.LBB8_304:                              //   in Loop: Header=BB8_296 Depth=1
	add	x11, x11, #1            // =1
	cmp	x11, x9
	b.ne	.LBB8_296
// %bb.305:
	cmp	x25, x24
	b.eq	.LBB8_310
.LBB8_306:
	mov	x26, x24
.LBB8_307:                              // =>This Inner Loop Header: Depth=1
	ldr	x0, [x26]
	cbz	x0, .LBB8_309
// %bb.308:                             //   in Loop: Header=BB8_307 Depth=1
	bl	_ZdlPv
.LBB8_309:                              //   in Loop: Header=BB8_307 Depth=1
	add	x26, x26, #24           // =24
	cmp	x25, x26
	b.ne	.LBB8_307
.LBB8_310:
	cbz	x24, .LBB8_312
.LBB8_311:
	mov	x0, x23
	bl	_ZdlPv
.LBB8_312:
	ldp	x0, x1, [sp, #80]
	mov	x9, #-6148914691236517206
	movk	x9, #43691
	stp	xzr, xzr, [sp, #32]
	subs	x8, x1, x0
	asr	x10, x8, #3
	mul	x25, x10, x9
	str	xzr, [sp, #48]
	b.eq	.LBB8_316
// %bb.313:
	cmp	x25, x19
	b.hi	.LBB8_351
// %bb.314:
.Ltmp57:
	mov	x0, x8
	bl	_Znwm
.Ltmp58:
// %bb.315:
	mov	x23, x0
	ldp	x0, x1, [sp, #80]
	mov	x24, x23
	b	.LBB8_317
.LBB8_316:
	mov	x23, xzr
	mov	x24, xzr
.LBB8_317:
	orr	w8, wzr, #0x18
	madd	x8, x25, x8, x24
	stp	x23, x23, [sp, #32]
	str	x8, [sp, #48]
.Ltmp62:
	mov	x2, x24
	bl	_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_
.Ltmp63:
// %bb.318:
	mov	x25, x0
	str	x0, [sp, #40]
.Ltmp65:
	add	x8, sp, #56             // =56
	add	x0, sp, #32             // =32
	bl	_Z16compute_averagesSt6vectorIS_IySaIyEESaIS1_EE
.Ltmp66:
// %bb.319:
	cmp	x24, x25
	b.eq	.LBB8_324
// %bb.320:
	mov	x19, x24
.LBB8_321:                              // =>This Inner Loop Header: Depth=1
	ldr	x0, [x19]
	cbz	x0, .LBB8_323
// %bb.322:                             //   in Loop: Header=BB8_321 Depth=1
	bl	_ZdlPv
.LBB8_323:                              //   in Loop: Header=BB8_321 Depth=1
	add	x19, x19, #24           // =24
	cmp	x25, x19
	b.ne	.LBB8_321
.LBB8_324:
	cbz	x24, .LBB8_326
// %bb.325:
	mov	x0, x23
	bl	_ZdlPv
.LBB8_326:
	ldp	d1, d0, [x22]
	adrp	x0, .L.str.6
	add	x0, x0, :lo12:.L.str.6
	ucvtf	d4, d0
	ucvtf	d3, d1
	fdiv	d0, d4, d3
	fdiv	d1, d3, d9
	fdiv	d2, d4, d9
	fdiv	d3, d3, d10
	fdiv	d4, d4, d10
	bl	printf
	ldp	d0, d1, [x22]
	adrp	x0, .L.str.7
	add	x0, x0, :lo12:.L.str.7
	ucvtf	d0, d0
	ucvtf	d1, d1
	fdiv	d0, d0, d8
	fdiv	d1, d1, d8
	bl	printf
	ldp	x1, x2, [x22]
	ldp	x3, x4, [x22, #16]
	ldr	x5, [x22, #32]
	adrp	x0, .L.str.8
	add	x0, x0, :lo12:.L.str.8
	bl	printf
	ldr	x23, [sp, #56]
	adrp	x0, .L.str.9
	add	x0, x0, :lo12:.L.str.9
	ldp	d0, d1, [x23]
	ldp	d2, d3, [x23, #16]
	ldr	d4, [x23, #32]
	bl	printf
	mov	w0, #10
	bl	putchar
	ldr	x24, [sp, #16]          // 8-byte Folded Reload
	cbz	x20, .LBB8_328
// %bb.327:
	mov	x0, x20
	bl	_ZdaPv
.LBB8_328:
	mov	x0, x21
	bl	_ZdaPv
	cbz	x23, .LBB8_330
// %bb.329:
	mov	x0, x23
	bl	_ZdlPv
.LBB8_330:
	cbz	x22, .LBB8_332
// %bb.331:
	mov	x0, x22
	bl	_ZdlPv
.LBB8_332:
	ldp	x20, x19, [sp, #80]
	cmp	x20, x19
	b.eq	.LBB8_337
.LBB8_333:                              // =>This Inner Loop Header: Depth=1
	ldr	x0, [x20]
	cbz	x0, .LBB8_335
// %bb.334:                             //   in Loop: Header=BB8_333 Depth=1
	bl	_ZdlPv
.LBB8_335:                              //   in Loop: Header=BB8_333 Depth=1
	add	x20, x20, #24           // =24
	cmp	x19, x20
	b.ne	.LBB8_333
// %bb.336:
	ldr	x20, [sp, #80]
.LBB8_337:
	cbz	x20, .LBB8_339
// %bb.338:
	mov	x0, x20
	bl	_ZdlPv
.LBB8_339:
	ldr	x0, [sp, #104]
	cbz	x0, .LBB8_341
// %bb.340:
	bl	_ZdlPv
.LBB8_341:
	ldr	w0, [sp, #152]
.Ltmp71:
	bl	close
.Ltmp72:
// %bb.342:
	ldr	x0, [sp, #304]
	cbz	x0, .LBB8_344
// %bb.343:
	bl	_ZdlPv
.LBB8_344:
	ldr	x0, [sp, #280]
	cbz	x0, .LBB8_346
// %bb.345:
	bl	_ZdlPv
.LBB8_346:
	mov	x0, x24
	bl	_ZdlPv
	ldp	x29, x30, [sp, #448]    // 16-byte Folded Reload
	ldp	x20, x19, [sp, #432]    // 16-byte Folded Reload
	ldp	x22, x21, [sp, #416]    // 16-byte Folded Reload
	ldp	x24, x23, [sp, #400]    // 16-byte Folded Reload
	ldp	x26, x25, [sp, #384]    // 16-byte Folded Reload
	ldp	x28, x27, [sp, #368]    // 16-byte Folded Reload
	ldp	d9, d8, [sp, #352]      // 16-byte Folded Reload
	ldr	d10, [sp, #336]         // 8-byte Folded Reload
	add	sp, sp, #464            // =464
	ret
.LBB8_347:
.Ltmp38:
	bl	_ZSt17__throw_bad_allocv
.Ltmp39:
// %bb.348:
.LBB8_349:
.Ltmp46:
	bl	_ZSt17__throw_bad_allocv
.Ltmp47:
// %bb.350:
.LBB8_351:
.Ltmp59:
	bl	_ZSt17__throw_bad_allocv
.Ltmp60:
// %bb.352:
.LBB8_353:
.Ltmp54:
	bl	_ZSt17__throw_bad_allocv
.Ltmp55:
// %bb.354:
.LBB8_355:
.Ltmp40:
	b	.LBB8_390
.LBB8_356:
.Ltmp56:
	mov	x20, x0
	mov	x19, x24
.LBB8_357:                              // =>This Inner Loop Header: Depth=1
	ldr	x0, [x19]
	cbz	x0, .LBB8_359
// %bb.358:                             //   in Loop: Header=BB8_357 Depth=1
	bl	_ZdlPv
.LBB8_359:                              //   in Loop: Header=BB8_357 Depth=1
	add	x19, x19, #24           // =24
	cmp	x25, x19
	b.ne	.LBB8_357
// %bb.360:
	cbnz	x24, .LBB8_376
	b	.LBB8_391
.LBB8_361:
.Ltmp61:
	mov	x20, x0
	cbnz	x22, .LBB8_374
	b	.LBB8_391
.LBB8_362:
.Ltmp48:
	b	.LBB8_390
.LBB8_363:
.Ltmp73:
.LBB8_364:
	add	x20, sp, #152           // =152
	mov	x19, x0
	add	x0, x20, #152           // =152
	bl	_ZNSt6vectorImSaImEED2Ev
	add	x0, x20, #128           // =128
	bl	_ZNSt6vectorImSaImEED2Ev
	mov	x0, x19
	bl	__clang_call_terminate
.LBB8_365:
.Ltmp67:
	cmp	x24, x25
	mov	x20, x0
	b.eq	.LBB8_370
// %bb.366:
	mov	x19, x24
.LBB8_367:                              // =>This Inner Loop Header: Depth=1
	ldr	x0, [x19]
	cbz	x0, .LBB8_369
// %bb.368:                             //   in Loop: Header=BB8_367 Depth=1
	bl	_ZdlPv
.LBB8_369:                              //   in Loop: Header=BB8_367 Depth=1
	add	x19, x19, #24           // =24
	cmp	x25, x19
	b.ne	.LBB8_367
.LBB8_370:
	cbnz	x24, .LBB8_372
	b	.LBB8_373
.LBB8_371:
.Ltmp64:
	mov	x20, x0
	cbz	x24, .LBB8_373
.LBB8_372:
	mov	x0, x23
	bl	_ZdlPv
.LBB8_373:
	cbz	x22, .LBB8_391
.LBB8_374:
	mov	x0, x22
	b	.LBB8_377
.LBB8_375:
.Ltmp51:
	mov	x20, x0
	cbz	x24, .LBB8_391
.LBB8_376:
	mov	x0, x23
.LBB8_377:
	bl	_ZdlPv
	b	.LBB8_391
.LBB8_378:
.Ltmp23:
	mov	x20, x0
	b	.LBB8_392
.LBB8_379:
.Ltmp20:
	ldr	x8, [sp, #128]
	mov	x20, x0
	cbz	x8, .LBB8_406
// %bb.380:
	mov	x0, x8
	b	.LBB8_405
.LBB8_381:
.Ltmp17:
	b	.LBB8_385
.LBB8_382:
.Ltmp14:
	mov	x20, x0
	mov	x25, x23
	b	.LBB8_406
.LBB8_383:
.Ltmp11:
	mov	x20, x0
	mov	x25, x24
	b	.LBB8_406
.LBB8_384:
.Ltmp8:
.LBB8_385:
	mov	x20, x0
	b	.LBB8_406
.LBB8_386:
.Ltmp5:
	bl	_Unwind_Resume
.LBB8_387:
.Ltmp28:
	b	.LBB8_390
.LBB8_388:
.Ltmp31:
	b	.LBB8_390
.LBB8_389:
.Ltmp43:
.LBB8_390:
	mov	x20, x0
.LBB8_391:
	ldr	x25, [sp, #16]          // 8-byte Folded Reload
.LBB8_392:
	ldp	x21, x19, [sp, #80]
	cmp	x21, x19
	b.eq	.LBB8_397
.LBB8_393:                              // =>This Inner Loop Header: Depth=1
	ldr	x0, [x21]
	cbz	x0, .LBB8_395
// %bb.394:                             //   in Loop: Header=BB8_393 Depth=1
	bl	_ZdlPv
.LBB8_395:                              //   in Loop: Header=BB8_393 Depth=1
	add	x21, x21, #24           // =24
	cmp	x19, x21
	b.ne	.LBB8_393
// %bb.396:
	ldr	x21, [sp, #80]
.LBB8_397:
	cbz	x21, .LBB8_399
// %bb.398:
	mov	x0, x21
	bl	_ZdlPv
.LBB8_399:
	ldr	x0, [sp, #104]
	cbz	x0, .LBB8_401
// %bb.400:
	bl	_ZdlPv
.LBB8_401:
	ldr	w0, [sp, #152]
.Ltmp68:
	bl	close
.Ltmp69:
// %bb.402:
	ldr	x0, [sp, #304]
	cbz	x0, .LBB8_404
// %bb.403:
	bl	_ZdlPv
.LBB8_404:
	ldr	x0, [sp, #280]
	cbz	x0, .LBB8_406
.LBB8_405:
	bl	_ZdlPv
.LBB8_406:
	mov	x0, x25
	bl	_ZdlPv
	mov	x0, x20
	bl	_Unwind_Resume
.LBB8_407:
.Ltmp70:
	b	.LBB8_364
.Lfunc_end8:
	.size	_Z8fasttestPKcc, .Lfunc_end8-_Z8fasttestPKcc
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table8:
.Lexception1:
	.byte	255                     // @LPStart Encoding = omit
	.byte	0                       // @TType Encoding = absptr
	.uleb128 .Lttbase0-.Lttbaseref0
.Lttbaseref0:
	.byte	1                       // Call site Encoding = uleb128
	.uleb128 .Lcst_end1-.Lcst_begin1
.Lcst_begin1:
	.uleb128 .Lfunc_begin1-.Lfunc_begin1 // >> Call Site 1 <<
	.uleb128 .Ltmp3-.Lfunc_begin1   //   Call between .Lfunc_begin1 and .Ltmp3
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp3-.Lfunc_begin1   // >> Call Site 2 <<
	.uleb128 .Ltmp4-.Ltmp3          //   Call between .Ltmp3 and .Ltmp4
	.uleb128 .Ltmp5-.Lfunc_begin1   //     jumps to .Ltmp5
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp6-.Lfunc_begin1   // >> Call Site 3 <<
	.uleb128 .Ltmp7-.Ltmp6          //   Call between .Ltmp6 and .Ltmp7
	.uleb128 .Ltmp8-.Lfunc_begin1   //     jumps to .Ltmp8
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp9-.Lfunc_begin1   // >> Call Site 4 <<
	.uleb128 .Ltmp10-.Ltmp9         //   Call between .Ltmp9 and .Ltmp10
	.uleb128 .Ltmp11-.Lfunc_begin1  //     jumps to .Ltmp11
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp12-.Lfunc_begin1  // >> Call Site 5 <<
	.uleb128 .Ltmp13-.Ltmp12        //   Call between .Ltmp12 and .Ltmp13
	.uleb128 .Ltmp14-.Lfunc_begin1  //     jumps to .Ltmp14
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp15-.Lfunc_begin1  // >> Call Site 6 <<
	.uleb128 .Ltmp16-.Ltmp15        //   Call between .Ltmp15 and .Ltmp16
	.uleb128 .Ltmp17-.Lfunc_begin1  //     jumps to .Ltmp17
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp18-.Lfunc_begin1  // >> Call Site 7 <<
	.uleb128 .Ltmp19-.Ltmp18        //   Call between .Ltmp18 and .Ltmp19
	.uleb128 .Ltmp20-.Lfunc_begin1  //     jumps to .Ltmp20
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp21-.Lfunc_begin1  // >> Call Site 8 <<
	.uleb128 .Ltmp22-.Ltmp21        //   Call between .Ltmp21 and .Ltmp22
	.uleb128 .Ltmp23-.Lfunc_begin1  //     jumps to .Ltmp23
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp22-.Lfunc_begin1  // >> Call Site 9 <<
	.uleb128 .Ltmp24-.Ltmp22        //   Call between .Ltmp22 and .Ltmp24
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp24-.Lfunc_begin1  // >> Call Site 10 <<
	.uleb128 .Ltmp27-.Ltmp24        //   Call between .Ltmp24 and .Ltmp27
	.uleb128 .Ltmp28-.Lfunc_begin1  //     jumps to .Ltmp28
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp29-.Lfunc_begin1  // >> Call Site 11 <<
	.uleb128 .Ltmp30-.Ltmp29        //   Call between .Ltmp29 and .Ltmp30
	.uleb128 .Ltmp31-.Lfunc_begin1  //     jumps to .Ltmp31
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp32-.Lfunc_begin1  // >> Call Site 12 <<
	.uleb128 .Ltmp33-.Ltmp32        //   Call between .Ltmp32 and .Ltmp33
	.uleb128 .Ltmp43-.Lfunc_begin1  //     jumps to .Ltmp43
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp33-.Lfunc_begin1  // >> Call Site 13 <<
	.uleb128 .Ltmp34-.Ltmp33        //   Call between .Ltmp33 and .Ltmp34
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp34-.Lfunc_begin1  // >> Call Site 14 <<
	.uleb128 .Ltmp42-.Ltmp34        //   Call between .Ltmp34 and .Ltmp42
	.uleb128 .Ltmp43-.Lfunc_begin1  //     jumps to .Ltmp43
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp42-.Lfunc_begin1  // >> Call Site 15 <<
	.uleb128 .Ltmp44-.Ltmp42        //   Call between .Ltmp42 and .Ltmp44
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp44-.Lfunc_begin1  // >> Call Site 16 <<
	.uleb128 .Ltmp45-.Ltmp44        //   Call between .Ltmp44 and .Ltmp45
	.uleb128 .Ltmp48-.Lfunc_begin1  //     jumps to .Ltmp48
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp49-.Lfunc_begin1  // >> Call Site 17 <<
	.uleb128 .Ltmp50-.Ltmp49        //   Call between .Ltmp49 and .Ltmp50
	.uleb128 .Ltmp51-.Lfunc_begin1  //     jumps to .Ltmp51
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp52-.Lfunc_begin1  // >> Call Site 18 <<
	.uleb128 .Ltmp53-.Ltmp52        //   Call between .Ltmp52 and .Ltmp53
	.uleb128 .Ltmp56-.Lfunc_begin1  //     jumps to .Ltmp56
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp53-.Lfunc_begin1  // >> Call Site 19 <<
	.uleb128 .Ltmp57-.Ltmp53        //   Call between .Ltmp53 and .Ltmp57
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp57-.Lfunc_begin1  // >> Call Site 20 <<
	.uleb128 .Ltmp58-.Ltmp57        //   Call between .Ltmp57 and .Ltmp58
	.uleb128 .Ltmp61-.Lfunc_begin1  //     jumps to .Ltmp61
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp62-.Lfunc_begin1  // >> Call Site 21 <<
	.uleb128 .Ltmp63-.Ltmp62        //   Call between .Ltmp62 and .Ltmp63
	.uleb128 .Ltmp64-.Lfunc_begin1  //     jumps to .Ltmp64
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp65-.Lfunc_begin1  // >> Call Site 22 <<
	.uleb128 .Ltmp66-.Ltmp65        //   Call between .Ltmp65 and .Ltmp66
	.uleb128 .Ltmp67-.Lfunc_begin1  //     jumps to .Ltmp67
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp71-.Lfunc_begin1  // >> Call Site 23 <<
	.uleb128 .Ltmp72-.Ltmp71        //   Call between .Ltmp71 and .Ltmp72
	.uleb128 .Ltmp73-.Lfunc_begin1  //     jumps to .Ltmp73
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp38-.Lfunc_begin1  // >> Call Site 24 <<
	.uleb128 .Ltmp39-.Ltmp38        //   Call between .Ltmp38 and .Ltmp39
	.uleb128 .Ltmp40-.Lfunc_begin1  //     jumps to .Ltmp40
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp46-.Lfunc_begin1  // >> Call Site 25 <<
	.uleb128 .Ltmp47-.Ltmp46        //   Call between .Ltmp46 and .Ltmp47
	.uleb128 .Ltmp48-.Lfunc_begin1  //     jumps to .Ltmp48
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp59-.Lfunc_begin1  // >> Call Site 26 <<
	.uleb128 .Ltmp60-.Ltmp59        //   Call between .Ltmp59 and .Ltmp60
	.uleb128 .Ltmp61-.Lfunc_begin1  //     jumps to .Ltmp61
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp54-.Lfunc_begin1  // >> Call Site 27 <<
	.uleb128 .Ltmp55-.Ltmp54        //   Call between .Ltmp54 and .Ltmp55
	.uleb128 .Ltmp56-.Lfunc_begin1  //     jumps to .Ltmp56
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp55-.Lfunc_begin1  // >> Call Site 28 <<
	.uleb128 .Ltmp68-.Ltmp55        //   Call between .Ltmp55 and .Ltmp68
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp68-.Lfunc_begin1  // >> Call Site 29 <<
	.uleb128 .Ltmp69-.Ltmp68        //   Call between .Ltmp68 and .Ltmp69
	.uleb128 .Ltmp70-.Lfunc_begin1  //     jumps to .Ltmp70
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp69-.Lfunc_begin1  // >> Call Site 30 <<
	.uleb128 .Lfunc_end8-.Ltmp69    //   Call between .Ltmp69 and .Lfunc_end8
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
.Lcst_end1:
	.byte	1                       // >> Action Record 1 <<
                                        //   Catch TypeInfo 1
	.byte	0                       //   No further actions
	.p2align	2
                                        // >> Catch TypeInfos <<
	.xword	0                       // TypeInfo 1
.Lttbase0:
	.p2align	2
                                        // -- End function
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4               // -- Begin function _ZN11LinuxEventsILi0EEC2ESt6vectorIiSaIiEE
.LCPI9_0:
	.xword	12                      // 0xc
	.xword	97                      // 0x61
	.section	.text._ZN11LinuxEventsILi0EEC2ESt6vectorIiSaIiEE,"axG",@progbits,_ZN11LinuxEventsILi0EEC2ESt6vectorIiSaIiEE,comdat
	.weak	_ZN11LinuxEventsILi0EEC2ESt6vectorIiSaIiEE
	.p2align	2
	.type	_ZN11LinuxEventsILi0EEC2ESt6vectorIiSaIiEE,@function
_ZN11LinuxEventsILi0EEC2ESt6vectorIiSaIiEE: // @_ZN11LinuxEventsILi0EEC2ESt6vectorIiSaIiEE
.Lfunc_begin2:
	.cfi_startproc
	.cfi_personality 0, __gxx_personality_v0
	.cfi_lsda 0, .Lexception2
// %bb.0:
	sub	sp, sp, #144            // =144
	stp	x28, x27, [sp, #48]     // 16-byte Folded Spill
	stp	x26, x25, [sp, #64]     // 16-byte Folded Spill
	stp	x24, x23, [sp, #80]     // 16-byte Folded Spill
	stp	x22, x21, [sp, #96]     // 16-byte Folded Spill
	stp	x20, x19, [sp, #112]    // 16-byte Folded Spill
	stp	x29, x30, [sp, #128]    // 16-byte Folded Spill
	add	x29, sp, #128           // =128
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -56
	.cfi_offset w24, -64
	.cfi_offset w25, -72
	.cfi_offset w26, -80
	.cfi_offset w27, -88
	.cfi_offset w28, -96
	orr	w8, wzr, #0x1
	movi	v0.2d, #0000000000000000
	mov	x19, x0
	mov	x22, x0
	adrp	x9, .LCPI9_0
	str	wzr, [x0]
	strb	w8, [x0, #4]
	str	q0, [x19, #128]!
	str	q0, [x22, #8]!
	stur	q0, [x0, #24]
	stur	q0, [x0, #104]
	stur	q0, [x0, #88]
	stur	q0, [x0, #72]
	stur	q0, [x0, #56]
	stp	q0, q0, [x0, #144]
	ldr	q0, [x9, :lo12:.LCPI9_0]
	orr	w8, wzr, #0x70
	str	w8, [x0, #12]
	str	xzr, [x0, #24]
	stur	q0, [x0, #40]
	ldp	x24, x25, [x1]
	mov	x23, x1
	mov	x20, x0
	add	x21, x0, #152           // =152
	subs	x8, x25, x24
	asr	x1, x8, #2
	str	w1, [x0, #120]
	str	x19, [sp, #8]           // 8-byte Folded Spill
	b.eq	.LBB9_3
// %bb.1:
.Ltmp74:
	mov	x0, x21
	bl	_ZNSt6vectorImSaImEE17_M_default_appendEm
.Ltmp75:
// %bb.2:
	ldp	x24, x25, [x23]
.LBB9_3:
	cmp	x24, x25
	b.eq	.LBB9_13
// %bb.4:
	add	x8, sp, #16             // =16
	adrp	x27, .L.str.17
	mov	w19, wzr
	mov	w23, #-1
	add	x27, x27, :lo12:.L.str.17
	add	x26, x8, #16            // =16
.LBB9_5:                                // =>This Inner Loop Header: Depth=1
	ldrsw	x8, [x24]
	mov	w0, #241
	mov	w3, #-1
	mov	x1, x22
	mov	w2, wzr
	mov	w4, w23
	mov	x5, xzr
	str	x8, [x20, #16]
	bl	syscall
	cmn	w0, #1                  // =1
	str	w0, [x20]
	b.ne	.LBB9_10
// %bb.6:                               //   in Loop: Header=BB9_5 Depth=1
	ldur	x8, [x27, #7]
	ldr	x9, [x27]
	stur	x8, [x26, #7]
	orr	w8, wzr, #0xf
	str	x9, [x26]
	stp	x26, x8, [sp, #16]
	strb	wzr, [sp, #47]
.Ltmp77:
	add	x1, sp, #16             // =16
	mov	x0, x20
	bl	_ZN11LinuxEventsILi0EE12report_errorERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
.Ltmp78:
// %bb.7:                               //   in Loop: Header=BB9_5 Depth=1
	ldr	x0, [sp, #16]
	cmp	x0, x26
	b.eq	.LBB9_9
// %bb.8:                               //   in Loop: Header=BB9_5 Depth=1
	bl	_ZdlPv
.LBB9_9:                                //   in Loop: Header=BB9_5 Depth=1
	ldr	w0, [x20]
.LBB9_10:                               //   in Loop: Header=BB9_5 Depth=1
	ldr	x8, [x21]
	mov	w1, #9223
	movk	w1, #32776, lsl #16
	add	w28, w19, #1            // =1
	add	x2, x8, w19, uxtw #3
                                        // kill: def $w0 killed $w0 killed $x0
	bl	ioctl
	cmn	w23, #1                 // =1
	b.ne	.LBB9_12
// %bb.11:                              //   in Loop: Header=BB9_5 Depth=1
	ldr	w23, [x20]
.LBB9_12:                               //   in Loop: Header=BB9_5 Depth=1
	add	x24, x24, #4            // =4
	cmp	x25, x24
	mov	w19, w28
	b.ne	.LBB9_5
.LBB9_13:
	ldrsw	x11, [x20, #120]
	ldp	x10, x8, [x20, #128]
	orr	w9, wzr, #0x1
	bfi	x9, x11, #1, #63
	sub	x11, x8, x10
	asr	x11, x11, #3
	cmp	x11, x9
	b.hs	.LBB9_15
// %bb.14:
	sub	x1, x9, x11
.Ltmp80:
	ldr	x0, [sp, #8]            // 8-byte Folded Reload
	bl	_ZNSt6vectorImSaImEE17_M_default_appendEm
.Ltmp81:
	b	.LBB9_18
.LBB9_15:
	b.ls	.LBB9_18
// %bb.16:
	add	x9, x10, x9, lsl #3
	cmp	x9, x8
	b.eq	.LBB9_18
// %bb.17:
	str	x9, [x20, #136]
.LBB9_18:
	ldp	x29, x30, [sp, #128]    // 16-byte Folded Reload
	ldp	x20, x19, [sp, #112]    // 16-byte Folded Reload
	ldp	x22, x21, [sp, #96]     // 16-byte Folded Reload
	ldp	x24, x23, [sp, #80]     // 16-byte Folded Reload
	ldp	x26, x25, [sp, #64]     // 16-byte Folded Reload
	ldp	x28, x27, [sp, #48]     // 16-byte Folded Reload
	add	sp, sp, #144            // =144
	ret
.LBB9_19:
.Ltmp82:
	b	.LBB9_21
.LBB9_20:
.Ltmp76:
.LBB9_21:
	mov	x20, x0
	ldr	x0, [x21]
	cbnz	x0, .LBB9_25
	b	.LBB9_26
.LBB9_22:
.Ltmp79:
	ldr	x8, [sp, #16]
	mov	x20, x0
	cmp	x8, x26
	b.eq	.LBB9_24
// %bb.23:
	mov	x0, x8
	bl	_ZdlPv
.LBB9_24:
	ldr	x0, [x21]
	cbz	x0, .LBB9_26
.LBB9_25:
	bl	_ZdlPv
.LBB9_26:
	ldr	x8, [sp, #8]            // 8-byte Folded Reload
	ldr	x0, [x8]
	cbz	x0, .LBB9_28
// %bb.27:
	bl	_ZdlPv
.LBB9_28:
	mov	x0, x20
	bl	_Unwind_Resume
.Lfunc_end9:
	.size	_ZN11LinuxEventsILi0EEC2ESt6vectorIiSaIiEE, .Lfunc_end9-_ZN11LinuxEventsILi0EEC2ESt6vectorIiSaIiEE
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table9:
.Lexception2:
	.byte	255                     // @LPStart Encoding = omit
	.byte	255                     // @TType Encoding = omit
	.byte	1                       // Call site Encoding = uleb128
	.uleb128 .Lcst_end2-.Lcst_begin2
.Lcst_begin2:
	.uleb128 .Ltmp74-.Lfunc_begin2  // >> Call Site 1 <<
	.uleb128 .Ltmp75-.Ltmp74        //   Call between .Ltmp74 and .Ltmp75
	.uleb128 .Ltmp76-.Lfunc_begin2  //     jumps to .Ltmp76
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp77-.Lfunc_begin2  // >> Call Site 2 <<
	.uleb128 .Ltmp78-.Ltmp77        //   Call between .Ltmp77 and .Ltmp78
	.uleb128 .Ltmp79-.Lfunc_begin2  //     jumps to .Ltmp79
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp80-.Lfunc_begin2  // >> Call Site 3 <<
	.uleb128 .Ltmp81-.Ltmp80        //   Call between .Ltmp80 and .Ltmp81
	.uleb128 .Ltmp82-.Lfunc_begin2  //     jumps to .Ltmp82
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp81-.Lfunc_begin2  // >> Call Site 4 <<
	.uleb128 .Lfunc_end9-.Ltmp81    //   Call between .Ltmp81 and .Lfunc_end9
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
.Lcst_end2:
	.p2align	2
                                        // -- End function
	.section	.text._ZN11LinuxEventsILi0EE5startEv,"axG",@progbits,_ZN11LinuxEventsILi0EE5startEv,comdat
	.weak	_ZN11LinuxEventsILi0EE5startEv // -- Begin function _ZN11LinuxEventsILi0EE5startEv
	.p2align	2
	.type	_ZN11LinuxEventsILi0EE5startEv,@function
_ZN11LinuxEventsILi0EE5startEv:         // @_ZN11LinuxEventsILi0EE5startEv
.Lfunc_begin3:
	.cfi_startproc
	.cfi_personality 0, __gxx_personality_v0
	.cfi_lsda 0, .Lexception3
// %bb.0:
	sub	sp, sp, #80             // =80
	stp	x20, x19, [sp, #48]     // 16-byte Folded Spill
	stp	x29, x30, [sp, #64]     // 16-byte Folded Spill
	add	x29, sp, #64            // =64
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x0
	ldr	w0, [x0]
	mov	w1, #9219
	orr	w2, wzr, #0x1
	bl	ioctl
	cmn	w0, #1                  // =1
	b.ne	.LBB10_5
// %bb.1:
	add	x8, sp, #8              // =8
	mov	w9, #27
	add	x20, x8, #16            // =16
	str	x20, [sp, #8]
	stur	x9, [x29, #-24]
.Ltmp83:
	add	x0, sp, #8              // =8
	sub	x1, x29, #24            // =24
	mov	x2, xzr
	bl	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm
.Ltmp84:
// %bb.2:
	adrp	x9, .L.str.24
	add	x9, x9, :lo12:.L.str.24
	ldur	x8, [x29, #-24]
	ldur	q0, [x9, #11]
	ldr	q1, [x9]
	str	x0, [sp, #8]
	str	x8, [sp, #24]
	stur	q0, [x0, #11]
	str	q1, [x0]
	ldr	x9, [sp, #8]
	str	x8, [sp, #16]
	strb	wzr, [x9, x8]
.Ltmp86:
	add	x1, sp, #8              // =8
	mov	x0, x19
	bl	_ZN11LinuxEventsILi0EE12report_errorERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
.Ltmp87:
// %bb.3:
	ldr	x0, [sp, #8]
	cmp	x0, x20
	b.eq	.LBB10_5
// %bb.4:
	bl	_ZdlPv
.LBB10_5:
	ldr	w0, [x19]
	mov	w1, #9216
	orr	w2, wzr, #0x1
	bl	ioctl
	cmn	w0, #1                  // =1
	b.ne	.LBB10_10
// %bb.6:
	add	x8, sp, #8              // =8
	orr	w9, wzr, #0x1c
	add	x20, x8, #16            // =16
	str	x20, [sp, #8]
	stur	x9, [x29, #-24]
.Ltmp89:
	add	x0, sp, #8              // =8
	sub	x1, x29, #24            // =24
	mov	x2, xzr
	bl	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm
.Ltmp90:
// %bb.7:
	adrp	x9, .L.str.25
	add	x9, x9, :lo12:.L.str.25
	ldur	x8, [x29, #-24]
	ldur	q0, [x9, #12]
	ldr	q1, [x9]
	str	x0, [sp, #8]
	str	x8, [sp, #24]
	stur	q0, [x0, #12]
	str	q1, [x0]
	ldr	x9, [sp, #8]
	str	x8, [sp, #16]
	strb	wzr, [x9, x8]
.Ltmp92:
	add	x1, sp, #8              // =8
	mov	x0, x19
	bl	_ZN11LinuxEventsILi0EE12report_errorERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
.Ltmp93:
// %bb.8:
	ldr	x0, [sp, #8]
	cmp	x0, x20
	b.eq	.LBB10_10
// %bb.9:
	bl	_ZdlPv
.LBB10_10:
	ldp	x29, x30, [sp, #64]     // 16-byte Folded Reload
	ldp	x20, x19, [sp, #48]     // 16-byte Folded Reload
	add	sp, sp, #80             // =80
	ret
.LBB10_11:
.Ltmp94:
	b	.LBB10_14
.LBB10_12:
.Ltmp91:
	bl	_Unwind_Resume
.LBB10_13:
.Ltmp88:
.LBB10_14:
	ldr	x8, [sp, #8]
	mov	x19, x0
	cmp	x8, x20
	b.eq	.LBB10_16
// %bb.15:
	mov	x0, x8
	bl	_ZdlPv
.LBB10_16:
	mov	x0, x19
	bl	_Unwind_Resume
.LBB10_17:
.Ltmp85:
	bl	_Unwind_Resume
.Lfunc_end10:
	.size	_ZN11LinuxEventsILi0EE5startEv, .Lfunc_end10-_ZN11LinuxEventsILi0EE5startEv
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table10:
.Lexception3:
	.byte	255                     // @LPStart Encoding = omit
	.byte	255                     // @TType Encoding = omit
	.byte	1                       // Call site Encoding = uleb128
	.uleb128 .Lcst_end3-.Lcst_begin3
.Lcst_begin3:
	.uleb128 .Ltmp83-.Lfunc_begin3  // >> Call Site 1 <<
	.uleb128 .Ltmp84-.Ltmp83        //   Call between .Ltmp83 and .Ltmp84
	.uleb128 .Ltmp85-.Lfunc_begin3  //     jumps to .Ltmp85
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp86-.Lfunc_begin3  // >> Call Site 2 <<
	.uleb128 .Ltmp87-.Ltmp86        //   Call between .Ltmp86 and .Ltmp87
	.uleb128 .Ltmp88-.Lfunc_begin3  //     jumps to .Ltmp88
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp89-.Lfunc_begin3  // >> Call Site 3 <<
	.uleb128 .Ltmp90-.Ltmp89        //   Call between .Ltmp89 and .Ltmp90
	.uleb128 .Ltmp91-.Lfunc_begin3  //     jumps to .Ltmp91
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp92-.Lfunc_begin3  // >> Call Site 4 <<
	.uleb128 .Ltmp93-.Ltmp92        //   Call between .Ltmp92 and .Ltmp93
	.uleb128 .Ltmp94-.Lfunc_begin3  //     jumps to .Ltmp94
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp93-.Lfunc_begin3  // >> Call Site 5 <<
	.uleb128 .Lfunc_end10-.Ltmp93   //   Call between .Ltmp93 and .Lfunc_end10
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
.Lcst_end3:
	.p2align	2
                                        // -- End function
	.section	.text._ZN11LinuxEventsILi0EE3endERSt6vectorIySaIyEE,"axG",@progbits,_ZN11LinuxEventsILi0EE3endERSt6vectorIySaIyEE,comdat
	.weak	_ZN11LinuxEventsILi0EE3endERSt6vectorIySaIyEE // -- Begin function _ZN11LinuxEventsILi0EE3endERSt6vectorIySaIyEE
	.p2align	2
	.type	_ZN11LinuxEventsILi0EE3endERSt6vectorIySaIyEE,@function
_ZN11LinuxEventsILi0EE3endERSt6vectorIySaIyEE: // @_ZN11LinuxEventsILi0EE3endERSt6vectorIySaIyEE
.Lfunc_begin4:
	.cfi_startproc
	.cfi_personality 0, __gxx_personality_v0
	.cfi_lsda 0, .Lexception4
// %bb.0:
	sub	sp, sp, #80             // =80
	str	x21, [sp, #32]          // 8-byte Folded Spill
	stp	x20, x19, [sp, #48]     // 16-byte Folded Spill
	stp	x29, x30, [sp, #64]     // 16-byte Folded Spill
	add	x29, sp, #64            // =64
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -48
	mov	x20, x0
	ldr	w0, [x0]
	mov	x19, x1
	mov	w1, #9217
	orr	w2, wzr, #0x1
	bl	ioctl
	cmn	w0, #1                  // =1
	b.ne	.LBB11_5
// %bb.1:
	mov	x8, sp
	mov	w9, #29
	add	x21, x8, #16            // =16
	str	x21, [sp]
	stur	x9, [x29, #-24]
.Ltmp95:
	mov	x0, sp
	sub	x1, x29, #24            // =24
	mov	x2, xzr
	bl	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm
.Ltmp96:
// %bb.2:
	adrp	x9, .L.str.26
	add	x9, x9, :lo12:.L.str.26
	ldur	x8, [x29, #-24]
	ldur	q0, [x9, #13]
	ldr	q1, [x9]
	str	x0, [sp]
	str	x8, [sp, #16]
	stur	q0, [x0, #13]
	str	q1, [x0]
	ldr	x9, [sp]
	str	x8, [sp, #8]
	strb	wzr, [x9, x8]
.Ltmp98:
	mov	x1, sp
	mov	x0, x20
	bl	_ZN11LinuxEventsILi0EE12report_errorERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
.Ltmp99:
// %bb.3:
	ldr	x0, [sp]
	cmp	x0, x21
	b.eq	.LBB11_5
// %bb.4:
	bl	_ZdlPv
.LBB11_5:
	ldp	x1, x8, [x20, #128]
	ldr	w0, [x20]
	sub	x2, x8, x1
	bl	read
	cmn	x0, #1                  // =1
	b.ne	.LBB11_9
// %bb.6:
	mov	x8, sp
	mov	w9, #25970
	movk	w9, #25697, lsl #16
	orr	w10, wzr, #0x4
	add	x21, x8, #16            // =16
	str	w9, [sp, #16]
	stp	x21, x10, [sp]
	strb	wzr, [sp, #20]
.Ltmp101:
	mov	x1, sp
	mov	x0, x20
	bl	_ZN11LinuxEventsILi0EE12report_errorERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
.Ltmp102:
// %bb.7:
	ldr	x0, [sp]
	cmp	x0, x21
	b.eq	.LBB11_9
// %bb.8:
	bl	_ZdlPv
.LBB11_9:
	ldp	x8, x9, [x20, #128]
	sub	x9, x9, x8
	asr	x9, x9, #3
	cmp	x9, #2                  // =2
	b.lo	.LBB11_12
// %bb.10:
	ldr	x10, [x19]
	orr	w11, wzr, #0x3
	orr	w12, wzr, #0x1
.LBB11_11:                              // =>This Inner Loop Header: Depth=1
	ldr	x14, [x8, x12, lsl #3]
	sub	w13, w11, #2            // =2
	mov	w12, w11
	lsr	w13, w13, #1
	cmp	x9, x12
	add	w11, w11, #2            // =2
	str	x14, [x10, w13, uxtw #3]
	b.hi	.LBB11_11
.LBB11_12:
	ldp	x29, x30, [sp, #64]     // 16-byte Folded Reload
	ldp	x20, x19, [sp, #48]     // 16-byte Folded Reload
	ldr	x21, [sp, #32]          // 8-byte Folded Reload
	add	sp, sp, #80             // =80
	ret
.LBB11_13:
.Ltmp103:
	b	.LBB11_15
.LBB11_14:
.Ltmp100:
.LBB11_15:
	ldr	x8, [sp]
	mov	x19, x0
	cmp	x8, x21
	b.eq	.LBB11_17
// %bb.16:
	mov	x0, x8
	bl	_ZdlPv
.LBB11_17:
	mov	x0, x19
	bl	_Unwind_Resume
.LBB11_18:
.Ltmp97:
	bl	_Unwind_Resume
.Lfunc_end11:
	.size	_ZN11LinuxEventsILi0EE3endERSt6vectorIySaIyEE, .Lfunc_end11-_ZN11LinuxEventsILi0EE3endERSt6vectorIySaIyEE
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table11:
.Lexception4:
	.byte	255                     // @LPStart Encoding = omit
	.byte	255                     // @TType Encoding = omit
	.byte	1                       // Call site Encoding = uleb128
	.uleb128 .Lcst_end4-.Lcst_begin4
.Lcst_begin4:
	.uleb128 .Ltmp95-.Lfunc_begin4  // >> Call Site 1 <<
	.uleb128 .Ltmp96-.Ltmp95        //   Call between .Ltmp95 and .Ltmp96
	.uleb128 .Ltmp97-.Lfunc_begin4  //     jumps to .Ltmp97
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp98-.Lfunc_begin4  // >> Call Site 2 <<
	.uleb128 .Ltmp99-.Ltmp98        //   Call between .Ltmp98 and .Ltmp99
	.uleb128 .Ltmp100-.Lfunc_begin4 //     jumps to .Ltmp100
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp99-.Lfunc_begin4  // >> Call Site 3 <<
	.uleb128 .Ltmp101-.Ltmp99       //   Call between .Ltmp99 and .Ltmp101
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp101-.Lfunc_begin4 // >> Call Site 4 <<
	.uleb128 .Ltmp102-.Ltmp101      //   Call between .Ltmp101 and .Ltmp102
	.uleb128 .Ltmp103-.Lfunc_begin4 //     jumps to .Ltmp103
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp102-.Lfunc_begin4 // >> Call Site 5 <<
	.uleb128 .Lfunc_end11-.Ltmp102  //   Call between .Ltmp102 and .Lfunc_end11
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
.Lcst_end4:
	.p2align	2
                                        // -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3               // -- Begin function main
.LCPI12_0:
	.xword	4636737291354636288     // double 100
.LCPI12_1:
	.xword	4634204016564240384     // double 64
	.text
	.globl	main
	.p2align	2
	.type	main,@function
main:                                   // @main
.Lfunc_begin5:
	.cfi_startproc
	.cfi_personality 0, __gxx_personality_v0
	.cfi_lsda 0, .Lexception5
// %bb.0:
	str	d12, [sp, #-144]!       // 8-byte Folded Spill
	stp	d11, d10, [sp, #16]     // 16-byte Folded Spill
	stp	d9, d8, [sp, #32]       // 16-byte Folded Spill
	stp	x28, x27, [sp, #48]     // 16-byte Folded Spill
	stp	x26, x25, [sp, #64]     // 16-byte Folded Spill
	stp	x24, x23, [sp, #80]     // 16-byte Folded Spill
	stp	x22, x21, [sp, #96]     // 16-byte Folded Spill
	stp	x20, x19, [sp, #112]    // 16-byte Folded Spill
	stp	x29, x30, [sp, #128]    // 16-byte Folded Spill
	add	x29, sp, #128           // =128
	sub	sp, sp, #624            // =624
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -56
	.cfi_offset w24, -64
	.cfi_offset w25, -72
	.cfi_offset w26, -80
	.cfi_offset w27, -88
	.cfi_offset w28, -96
	.cfi_offset b8, -104
	.cfi_offset b9, -112
	.cfi_offset b10, -120
	.cfi_offset b11, -128
	.cfi_offset b12, -144
	cmp	w0, #2                  // =2
	b.lt	.LBB12_2
// %bb.1:
	adrp	x0, .L.str.11
	add	x0, x0, :lo12:.L.str.11
	mov	w1, #44
	bl	_Z8fasttestPKcc
.LBB12_2:
	adrp	x0, .Lstr
	add	x0, x0, :lo12:.Lstr
	bl	puts
	mov	x20, xzr
	orr	w21, wzr, #0x1
	add	x22, sp, #8             // =8
	add	x23, sp, #264           // =264
.LBB12_3:                               // =>This Loop Header: Depth=1
                                        //     Child Loop BB12_4 Depth 2
                                        //     Child Loop BB12_8 Depth 2
	lsl	x19, x21, x20
	add	x0, sp, #8              // =8
	sub	x1, x29, #160           // =160
	mov	w2, wzr
	mov	x3, x19
	stur	wzr, [x29, #-160]
	bl	_ZL14faster_decoderPjRjjm
	mov	w8, wzr
.LBB12_4:                               //   Parent Loop BB12_3 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	rbit	x9, x19
	sub	x10, x19, #1            // =1
	clz	x9, x9
	ands	x19, x10, x19
	str	w9, [x23, w8, uxtw #2]
	add	w8, w8, #1              // =1
	b.ne	.LBB12_4
// %bb.5:                               //   in Loop: Header=BB12_3 Depth=1
	ldur	w9, [x29, #-160]
	cmp	w9, w8
	b.ne	.LBB12_22
// %bb.6:                               //   in Loop: Header=BB12_3 Depth=1
	cbz	w8, .LBB12_10
// %bb.7:                               //   in Loop: Header=BB12_3 Depth=1
	mov	x9, xzr
	mov	w8, w8
.LBB12_8:                               //   Parent Loop BB12_3 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x10, x9, #2
	ldr	w11, [x22, x10]
	ldr	w10, [x23, x10]
	cmp	w11, w10
	b.ne	.LBB12_22
// %bb.9:                               //   in Loop: Header=BB12_8 Depth=2
	add	x9, x9, #1              // =1
	cmp	x9, x8
	b.lo	.LBB12_8
.LBB12_10:                              //   in Loop: Header=BB12_3 Depth=1
	add	x20, x20, #1            // =1
	cmp	x20, #64                // =64
	b.lo	.LBB12_3
// %bb.11:
	mov	w21, #16960
	mov	x20, xzr
	movk	w21, #15, lsl #16
	add	x22, sp, #8             // =8
	add	x23, sp, #264           // =264
.LBB12_12:                              // =>This Loop Header: Depth=1
                                        //     Child Loop BB12_14 Depth 2
                                        //     Child Loop BB12_18 Depth 2
	bl	rand
	mov	w19, w0
	bl	rand
                                        // kill: def $w0 killed $w0 def $x0
	sxtw	x8, w0
	orr	x19, x8, x19, lsl #32
	add	x0, sp, #8              // =8
	sub	x1, x29, #160           // =160
	mov	w2, wzr
	mov	x3, x19
	stur	wzr, [x29, #-160]
	bl	_ZL14faster_decoderPjRjjm
	fmov	d0, x19
	cnt	v0.8b, v0.8b
	uaddlv	h0, v0.8b
	fmov	w8, s0
	cbz	w8, .LBB12_15
// %bb.13:                              //   in Loop: Header=BB12_12 Depth=1
	add	x9, sp, #264            // =264
	mov	x10, x8
.LBB12_14:                              //   Parent Loop BB12_12 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	rbit	x11, x19
	sub	x12, x19, #1            // =1
	clz	x11, x11
	subs	x10, x10, #1            // =1
	str	w11, [x9], #4
	and	x19, x12, x19
	b.ne	.LBB12_14
.LBB12_15:                              //   in Loop: Header=BB12_12 Depth=1
	ldur	w9, [x29, #-160]
	cmp	w9, w8
	b.ne	.LBB12_22
// %bb.16:                              //   in Loop: Header=BB12_12 Depth=1
	cbz	w8, .LBB12_20
// %bb.17:                              //   in Loop: Header=BB12_12 Depth=1
	mov	x9, xzr
.LBB12_18:                              //   Parent Loop BB12_12 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x10, x9, #2
	ldr	w11, [x22, x10]
	ldr	w10, [x23, x10]
	cmp	w11, w10
	b.ne	.LBB12_22
// %bb.19:                              //   in Loop: Header=BB12_18 Depth=2
	add	x9, x9, #1              // =1
	cmp	x9, x8
	b.lo	.LBB12_18
.LBB12_20:                              //   in Loop: Header=BB12_12 Depth=1
	add	x20, x20, #1            // =1
	cmp	x20, x21
	b.lo	.LBB12_12
// %bb.21:
	adrp	x0, .Lstr.48
	add	x0, x0, :lo12:.Lstr.48
	b	.LBB12_23
.LBB12_22:
	adrp	x0, .Lstr.52
	add	x0, x0, :lo12:.Lstr.52
.LBB12_23:
	bl	puts
	adrp	x0, .L.str.11
	add	x0, x0, :lo12:.L.str.11
	add	x1, sp, #264            // =264
	bl	_Z14load_text_filePKcPm
	cbz	x0, .LBB12_31
// %bb.24:
	ldr	x23, [sp, #264]
	mov	x19, x0
	lsr	x22, x23, #6
	lsl	x21, x22, #3
	mov	x0, x21
	bl	_Znam
	mov	w1, wzr
	mov	x2, x21
	mov	x20, x0
	bl	memset
	ands	x8, x23, #0xffffffffffffffc0
	b.eq	.LBB12_30
// %bb.25:
	mov	x9, xzr
	orr	w10, wzr, #0x1
.LBB12_26:                              // =>This Inner Loop Header: Depth=1
	ldrb	w11, [x19, x9]
	cmp	w11, #44                // =44
	b.eq	.LBB12_28
// %bb.27:                              //   in Loop: Header=BB12_26 Depth=1
	cmp	w11, #31                // =31
	b.hi	.LBB12_29
.LBB12_28:                              //   in Loop: Header=BB12_26 Depth=1
	lsr	x11, x9, #3
	and	x11, x11, #0x1ffffffffffffff8
	ldr	x12, [x20, x11]
	lsl	x13, x10, x9
	orr	x12, x12, x13
	str	x12, [x20, x11]
.LBB12_29:                              //   in Loop: Header=BB12_26 Depth=1
	add	x9, x9, #1              // =1
	cmp	x8, x9
	b.ne	.LBB12_26
.LBB12_30:
	mov	x0, x19
	bl	_ZdaPv
	b	.LBB12_32
.LBB12_31:
	mov	x20, xzr
                                        // implicit-def: $x22
.LBB12_32:
	lsl	x25, x22, #6
	lsl	x8, x22, #8
	cmp	xzr, x25, lsr #62
	csinv	x0, x8, xzr, eq
	bl	_Znam
	mov	x21, x0
.Ltmp104:
	orr	w0, wzr, #0x4
	bl	_Znwm
.Ltmp105:
// %bb.33:
	mov	x19, x0
	str	wzr, [x0]
.Ltmp107:
	orr	w0, wzr, #0x8
	bl	_Znwm
.Ltmp108:
// %bb.34:
	ldr	w8, [x19]
	orr	w9, wzr, #0x1
	mov	x24, x0
	stp	w8, w9, [x0]
	mov	x0, x19
	bl	_ZdlPv
.Ltmp110:
	orr	w0, wzr, #0x10
	bl	_Znwm
.Ltmp111:
// %bb.35:
	ldr	x8, [x24]
	mov	w9, #5
	mov	x23, x0
	str	w9, [x0, #8]
	str	x8, [x0]
	mov	x0, x24
	bl	_ZdlPv
	orr	w8, wzr, #0x2
	str	w8, [x23, #12]
.Ltmp113:
	orr	w0, wzr, #0x20
	bl	_Znwm
.Ltmp114:
// %bb.36:
	ldr	q0, [x23]
	orr	w8, wzr, #0x3
	mov	x19, x0
	str	w8, [x0, #16]
	str	q0, [x0]
	mov	x0, x23
	bl	_ZdlPv
	stp	xzr, xzr, [sp, #16]
	str	xzr, [sp, #8]
.Ltmp116:
	mov	w0, #20
	bl	_Znwm
.Ltmp117:
// %bb.37:
	ldr	w8, [x19, #16]
	ldr	q0, [x19]
	add	x9, x0, #20             // =20
	str	x9, [sp, #24]
	str	w8, [x0, #16]
	str	q0, [x0]
	stp	x0, x9, [sp, #8]
.Ltmp119:
	add	x0, sp, #264            // =264
	add	x1, sp, #8              // =8
	bl	_ZN11LinuxEventsILi0EEC2ESt6vectorIiSaIiEE
.Ltmp120:
// %bb.38:
	ldr	x0, [sp, #8]
	cbz	x0, .LBB12_40
// %bb.39:
	bl	_ZdlPv
.LBB12_40:
	stp	xzr, xzr, [x29, #-152]
	stp	xzr, xzr, [x29, #-168]
	stp	xzr, xzr, [x29, #-184]
.Ltmp122:
	mov	w0, #40
	bl	_Znwm
.Ltmp123:
// %bb.41:
	ldp	x24, x8, [x29, #-160]
	mov	x23, x0
	movi	v0.2d, #0000000000000000
	str	xzr, [x0, #32]
	subs	x2, x8, x24
	stp	q0, q0, [x0]
	b.eq	.LBB12_43
// %bb.42:
	mov	x0, x23
	mov	x1, x24
	bl	memmove
.LBB12_43:
	cbz	x24, .LBB12_45
// %bb.44:
	mov	x0, x24
	bl	_ZdlPv
.LBB12_45:
	mov	w26, wzr
	add	x8, x23, #40            // =40
	stur	x8, [x29, #-144]
	stp	x23, x8, [x29, #-160]
.LBB12_46:                              // =>This Loop Header: Depth=1
                                        //     Child Loop BB12_49 Depth 2
                                        //       Child Loop BB12_54 Depth 3
.Ltmp125:
	add	x0, sp, #264            // =264
	bl	_ZN11LinuxEventsILi0EE5startEv
.Ltmp126:
// %bb.47:                              //   in Loop: Header=BB12_46 Depth=1
	cbz	x22, .LBB12_56
// %bb.48:                              //   in Loop: Header=BB12_46 Depth=1
	mov	x8, xzr
	mov	w9, wzr
.LBB12_49:                              //   Parent Loop BB12_46 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB12_54 Depth 3
	ldr	x11, [x20, x8, lsl #3]
	fmov	d0, x11
	cnt	v0.8b, v0.8b
	uaddlv	h0, v0.8b
	fmov	w10, s0
	cbz	x11, .LBB12_51
// %bb.50:                              //   in Loop: Header=BB12_49 Depth=2
	rbit	x12, x11
	sub	x13, x11, #1            // =1
	clz	x12, x12
	and	x11, x13, x11
	add	w12, w12, w8
	str	w12, [x21, w9, uxtw #2]
	rbit	x12, x11
	clz	x12, x12
	add	w13, w9, #1             // =1
	add	w12, w12, w8
	str	w12, [x21, w13, uxtw #2]
	sub	x13, x11, #1            // =1
	and	x11, x13, x11
	rbit	x13, x11
	clz	x13, x13
	add	w12, w9, #2             // =2
	add	w13, w13, w8
	str	w13, [x21, w12, uxtw #2]
	sub	x13, x11, #1            // =1
	and	x11, x13, x11
	rbit	x13, x11
	clz	x13, x13
	add	w12, w9, #3             // =3
	add	w13, w13, w8
	str	w13, [x21, w12, uxtw #2]
	sub	x12, x11, #1            // =1
	and	x11, x12, x11
.LBB12_51:                              //   in Loop: Header=BB12_49 Depth=2
	cmp	w10, #5                 // =5
	add	w23, w9, w10
	b.lo	.LBB12_55
// %bb.52:                              //   in Loop: Header=BB12_49 Depth=2
	rbit	x12, x11
	clz	x12, x12
	add	w13, w9, #4             // =4
	add	w12, w12, w8
	str	w12, [x21, w13, uxtw #2]
	sub	x12, x11, #1            // =1
	and	x11, x12, x11
	rbit	x12, x11
	clz	x12, x12
	add	w13, w9, #5             // =5
	add	w12, w12, w8
	str	w12, [x21, w13, uxtw #2]
	sub	x13, x11, #1            // =1
	and	x11, x13, x11
	rbit	x13, x11
	clz	x13, x13
	add	w12, w9, #6             // =6
	add	w13, w13, w8
	str	w13, [x21, w12, uxtw #2]
	sub	x13, x11, #1            // =1
	and	x11, x13, x11
	rbit	x13, x11
	clz	x13, x13
	add	w12, w9, #7             // =7
	add	w13, w13, w8
	cmp	w10, #9                 // =9
	str	w13, [x21, w12, uxtw #2]
	b.lo	.LBB12_55
// %bb.53:                              //   in Loop: Header=BB12_49 Depth=2
	sub	x10, x11, #1            // =1
	and	x10, x10, x11
	add	w9, w9, #8              // =8
.LBB12_54:                              //   Parent Loop BB12_46 Depth=1
                                        //     Parent Loop BB12_49 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	rbit	x11, x10
	clz	x11, x11
	sub	x12, x10, #1            // =1
	add	w11, w11, w8
	ands	x10, x12, x10
	str	w11, [x21, w9, uxtw #2]
	add	w9, w9, #1              // =1
	b.ne	.LBB12_54
.LBB12_55:                              //   in Loop: Header=BB12_49 Depth=2
	add	x8, x8, #1              // =1
	cmp	x8, x22
	mov	w9, w23
	b.ne	.LBB12_49
	b	.LBB12_57
.LBB12_56:                              //   in Loop: Header=BB12_46 Depth=1
	mov	w23, wzr
.LBB12_57:                              //   in Loop: Header=BB12_46 Depth=1
.Ltmp127:
	add	x0, sp, #264            // =264
	sub	x1, x29, #160           // =160
	bl	_ZN11LinuxEventsILi0EE3endERSt6vectorIySaIyEE
.Ltmp128:
// %bb.58:                              //   in Loop: Header=BB12_46 Depth=1
	ldp	x24, x8, [x29, #-176]
	cmp	x24, x8
	b.eq	.LBB12_63
// %bb.59:                              //   in Loop: Header=BB12_46 Depth=1
	ldp	x9, x8, [x29, #-160]
	stp	xzr, xzr, [x24]
	str	xzr, [x24, #16]
	subs	x0, x8, x9
	asr	x27, x0, #3
	b.eq	.LBB12_64
// %bb.60:                              //   in Loop: Header=BB12_46 Depth=1
	lsr	x8, x27, #61
	cbnz	x8, .LBB12_434
// %bb.61:                              //   in Loop: Header=BB12_46 Depth=1
.Ltmp129:
	bl	_Znwm
.Ltmp130:
// %bb.62:                              //   in Loop: Header=BB12_46 Depth=1
	mov	x28, x0
	b	.LBB12_65
.LBB12_63:                              //   in Loop: Header=BB12_46 Depth=1
.Ltmp134:
	sub	x0, x29, #184           // =184
	sub	x2, x29, #160           // =160
	mov	x1, x24
	bl	_ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_
.Ltmp135:
	b	.LBB12_68
.LBB12_64:                              //   in Loop: Header=BB12_46 Depth=1
	mov	x28, xzr
.LBB12_65:                              //   in Loop: Header=BB12_46 Depth=1
	add	x8, x28, x27, lsl #3
	stp	x28, x0, [x24]
	str	x8, [x24, #16]
	ldp	x1, x8, [x29, #-160]
	subs	x2, x8, x1
	asr	x27, x2, #3
	b.eq	.LBB12_67
// %bb.66:                              //   in Loop: Header=BB12_46 Depth=1
	bl	memmove
.LBB12_67:                              //   in Loop: Header=BB12_46 Depth=1
	add	x8, x28, x27, lsl #3
	str	x8, [x24, #8]
	ldur	x8, [x29, #-176]
	add	x8, x8, #24             // =24
	stur	x8, [x29, #-176]
.LBB12_68:                              //   in Loop: Header=BB12_46 Depth=1
	add	w26, w26, #1            // =1
	cmp	w26, #10                // =10
	b.lo	.LBB12_46
// %bb.69:
	ldr	w8, [x21]
	cbnz	w8, .LBB12_71
// %bb.70:
	adrp	x0, .L.str.3
	add	x0, x0, :lo12:.L.str.3
	bl	printf
.LBB12_71:
	adrp	x8, .LCPI12_0
	ldr	d8, [x8, :lo12:.LCPI12_0]
	ucvtf	d11, w23
	ucvtf	d10, x25
	fdiv	d0, d11, d10
	adrp	x0, .L.str.4
	mov	x24, #-6148914691236517206
	fmul	d0, d0, d8
	add	x0, x0, :lo12:.L.str.4
	mov	w1, w23
	mov	x2, x22
	movk	x24, #2730, lsl #48
	bl	printf
	adrp	x8, .LCPI12_1
	ldr	d9, [x8, :lo12:.LCPI12_1]
	ucvtf	d12, x22
	adrp	x0, .L.str.5
	add	x0, x0, :lo12:.L.str.5
	fmul	d0, d12, d9
	fdiv	d0, d0, d11
	bl	printf
	ldp	x0, x1, [x29, #-184]
	subs	x8, x1, x0
	b.eq	.LBB12_75
// %bb.72:
	mov	x10, #-6148914691236517206
	asr	x9, x8, #3
	movk	x10, #43691
	mul	x9, x9, x10
	cmp	x9, x24
	b.hi	.LBB12_440
// %bb.73:
.Ltmp137:
	mov	x0, x8
	bl	_Znwm
.Ltmp138:
// %bb.74:
	mov	x23, x0
	ldp	x0, x1, [x29, #-184]
	mov	x24, x23
	b	.LBB12_76
.LBB12_75:
	mov	x23, xzr
	mov	x24, xzr
.LBB12_76:
.Ltmp142:
	mov	x2, x24
	bl	_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_
.Ltmp143:
// %bb.77:
	mov	x25, x0
	subs	x27, x0, x24
	b.eq	.LBB12_83
// %bb.78:
	ldp	x8, x9, [x24]
	subs	x0, x9, x8
	b.eq	.LBB12_84
// %bb.79:
	asr	x8, x0, #3
	lsr	x8, x8, #61
	cbnz	x8, .LBB12_452
// %bb.80:
.Ltmp145:
	bl	_Znwm
.Ltmp146:
// %bb.81:
	ldp	x1, x8, [x24]
	mov	x26, x0
	subs	x2, x8, x1
	b.eq	.LBB12_85
// %bb.82:
	mov	x0, x26
	asr	x28, x2, #3
	bl	memmove
	mov	w8, wzr
	b	.LBB12_86
.LBB12_83:
	mov	x22, xzr
	cbnz	x24, .LBB12_114
	b	.LBB12_115
.LBB12_84:
	mov	x22, xzr
	mov	x26, xzr
	mov	x28, xzr
	orr	w8, wzr, #0x1
	b	.LBB12_87
.LBB12_85:
	mov	x28, xzr
	orr	w8, wzr, #0x1
.LBB12_86:
	mov	x22, x26
.LBB12_87:
	mov	x10, #-6148914691236517206
	asr	x9, x27, #3
	movk	x10, #43691
	mul	x9, x9, x10
	cmp	x9, #1                  // =1
	mov	x27, x24
	b.ls	.LBB12_110
// %bb.88:
	cmp	x28, #1                 // =1
	csinc	x12, x28, xzr, hi
	add	x10, x22, #16           // =16
	orr	w11, wzr, #0x1
	add	x13, x22, x12, lsl #3
	and	x14, x12, #0xfffffffffffffffc
	orr	w15, wzr, #0x18
	b	.LBB12_100
.LBB12_89:                              //   in Loop: Header=BB12_100 Depth=1
	add	x17, x16, #16           // =16
	mov	x18, x10
	mov	x0, x14
.LBB12_90:                              //   Parent Loop BB12_100 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldp	q1, q0, [x17, #-16]
	ldp	q3, q2, [x18, #-16]
	cmhi	v3.2d, v3.2d, v1.2d
	xtn	v3.2s, v3.2d
	fmov	w1, s3
	tbz	w1, #0, .LBB12_92
// %bb.91:                              //   in Loop: Header=BB12_90 Depth=2
	stur	d1, [x18, #-16]
.LBB12_92:                              //   in Loop: Header=BB12_90 Depth=2
	mov	w1, v3.s[1]
	tbz	w1, #0, .LBB12_94
// %bb.93:                              //   in Loop: Header=BB12_90 Depth=2
	sub	x1, x18, #8             // =8
	st1	{ v1.d }[1], [x1]
.LBB12_94:                              //   in Loop: Header=BB12_90 Depth=2
	cmhi	v1.2d, v2.2d, v0.2d
	xtn	v1.2s, v1.2d
	fmov	w1, s1
	tbz	w1, #0, .LBB12_96
// %bb.95:                              //   in Loop: Header=BB12_90 Depth=2
	str	d0, [x18]
.LBB12_96:                              //   in Loop: Header=BB12_90 Depth=2
	mov	w1, v1.s[1]
	tbz	w1, #0, .LBB12_98
// %bb.97:                              //   in Loop: Header=BB12_90 Depth=2
	add	x1, x18, #8             // =8
	st1	{ v0.d }[1], [x1]
.LBB12_98:                              //   in Loop: Header=BB12_90 Depth=2
	subs	x0, x0, #4              // =4
	add	x18, x18, #32           // =32
	add	x17, x17, #32           // =32
	b.ne	.LBB12_90
// %bb.99:                              //   in Loop: Header=BB12_100 Depth=1
	cmp	x12, x14
	mov	x17, x14
	b.ne	.LBB12_105
	b	.LBB12_108
.LBB12_100:                             // =>This Loop Header: Depth=1
                                        //     Child Loop BB12_90 Depth 2
                                        //     Child Loop BB12_105 Depth 2
	tbnz	w8, #0, .LBB12_108
// %bb.101:                             //   in Loop: Header=BB12_100 Depth=1
	mul	x16, x11, x15
	ldr	x16, [x24, x16]
	cmp	x12, #3                 // =3
	b.ls	.LBB12_104
// %bb.102:                             //   in Loop: Header=BB12_100 Depth=1
	add	x17, x16, x12, lsl #3
	cmp	x26, x17
	b.hs	.LBB12_89
// %bb.103:                             //   in Loop: Header=BB12_100 Depth=1
	cmp	x16, x13
	b.hs	.LBB12_89
.LBB12_104:                             //   in Loop: Header=BB12_100 Depth=1
	mov	x17, xzr
.LBB12_105:                             //   Parent Loop BB12_100 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x0, x17, #3
	ldr	x18, [x16, x0]
	ldr	x0, [x22, x0]
	cmp	x18, x0
	b.hs	.LBB12_107
// %bb.106:                             //   in Loop: Header=BB12_105 Depth=2
	str	x18, [x22, x17, lsl #3]
.LBB12_107:                             //   in Loop: Header=BB12_105 Depth=2
	add	x17, x17, #1            // =1
	cmp	x17, x28
	b.lo	.LBB12_105
.LBB12_108:                             //   in Loop: Header=BB12_100 Depth=1
	add	x11, x11, #1            // =1
	cmp	x11, x9
	b.ne	.LBB12_100
// %bb.109:
	mov	x27, x24
.LBB12_110:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x27]
	cbz	x0, .LBB12_112
// %bb.111:                             //   in Loop: Header=BB12_110 Depth=1
	bl	_ZdlPv
.LBB12_112:                             //   in Loop: Header=BB12_110 Depth=1
	add	x27, x27, #24           // =24
	cmp	x25, x27
	b.ne	.LBB12_110
// %bb.113:
	cbz	x24, .LBB12_115
.LBB12_114:
	mov	x0, x23
	bl	_ZdlPv
.LBB12_115:
	ldp	x0, x1, [x29, #-184]
	mov	x9, #-6148914691236517206
	movk	x9, #43691
	stp	xzr, xzr, [x29, #-232]
	subs	x8, x1, x0
	asr	x10, x8, #3
	mul	x25, x10, x9
	stur	xzr, [x29, #-216]
	b.eq	.LBB12_119
// %bb.116:
	mov	x9, #-6148914691236517206
	movk	x9, #2730, lsl #48
	cmp	x25, x9
	b.hi	.LBB12_442
// %bb.117:
.Ltmp150:
	mov	x0, x8
	bl	_Znwm
.Ltmp151:
// %bb.118:
	mov	x23, x0
	ldp	x0, x1, [x29, #-184]
	mov	x24, x23
	b	.LBB12_120
.LBB12_119:
	mov	x23, xzr
	mov	x24, xzr
.LBB12_120:
	orr	w8, wzr, #0x18
	madd	x8, x25, x8, x24
	stp	x23, x23, [x29, #-232]
	stur	x8, [x29, #-216]
.Ltmp155:
	mov	x2, x24
	bl	_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_
.Ltmp156:
// %bb.121:
	mov	x25, x0
	stur	x0, [x29, #-224]
.Ltmp158:
	sub	x8, x29, #208           // =208
	sub	x0, x29, #232           // =232
	bl	_Z16compute_averagesSt6vectorIS_IySaIyEESaIS1_EE
.Ltmp159:
// %bb.122:
	cmp	x24, x25
	b.eq	.LBB12_127
// %bb.123:
	mov	x26, x24
.LBB12_124:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x26]
	cbz	x0, .LBB12_126
// %bb.125:                             //   in Loop: Header=BB12_124 Depth=1
	bl	_ZdlPv
.LBB12_126:                             //   in Loop: Header=BB12_124 Depth=1
	add	x26, x26, #24           // =24
	cmp	x25, x26
	b.ne	.LBB12_124
.LBB12_127:
	cbz	x24, .LBB12_129
// %bb.128:
	mov	x0, x23
	bl	_ZdlPv
.LBB12_129:
	ldp	d1, d0, [x22]
	adrp	x0, .L.str.6
	add	x0, x0, :lo12:.L.str.6
	ucvtf	d4, d0
	ucvtf	d3, d1
	fdiv	d0, d4, d3
	fdiv	d1, d3, d11
	fdiv	d2, d4, d11
	fdiv	d3, d3, d12
	fdiv	d4, d4, d12
	bl	printf
	ldp	d0, d1, [x22]
	adrp	x0, .L.str.7
	add	x0, x0, :lo12:.L.str.7
	ucvtf	d0, d0
	ucvtf	d1, d1
	fdiv	d0, d0, d10
	fdiv	d1, d1, d10
	bl	printf
	ldp	x1, x2, [x22]
	ldp	x3, x4, [x22, #16]
	ldr	x5, [x22, #32]
	adrp	x0, .L.str.8
	add	x0, x0, :lo12:.L.str.8
	bl	printf
	ldur	x23, [x29, #-208]
	adrp	x0, .L.str.9
	add	x0, x0, :lo12:.L.str.9
	ldp	d0, d1, [x23]
	ldp	d2, d3, [x23, #16]
	ldr	d4, [x23, #32]
	bl	printf
	mov	w0, #10
	bl	putchar
	cbz	x20, .LBB12_131
// %bb.130:
	mov	x0, x20
	bl	_ZdaPv
.LBB12_131:
	mov	x0, x21
	bl	_ZdaPv
	cbz	x23, .LBB12_133
// %bb.132:
	mov	x0, x23
	bl	_ZdlPv
.LBB12_133:
	cbz	x22, .LBB12_135
// %bb.134:
	mov	x0, x22
	bl	_ZdlPv
.LBB12_135:
	ldp	x20, x21, [x29, #-184]
	cmp	x20, x21
	b.eq	.LBB12_140
.LBB12_136:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x20]
	cbz	x0, .LBB12_138
// %bb.137:                             //   in Loop: Header=BB12_136 Depth=1
	bl	_ZdlPv
.LBB12_138:                             //   in Loop: Header=BB12_136 Depth=1
	add	x20, x20, #24           // =24
	cmp	x21, x20
	b.ne	.LBB12_136
// %bb.139:
	ldur	x20, [x29, #-184]
.LBB12_140:
	cbz	x20, .LBB12_142
// %bb.141:
	mov	x0, x20
	bl	_ZdlPv
.LBB12_142:
	ldur	x0, [x29, #-160]
	cbz	x0, .LBB12_144
// %bb.143:
	bl	_ZdlPv
.LBB12_144:
	ldr	w0, [sp, #264]
.Ltmp164:
	bl	close
.Ltmp165:
// %bb.145:
	ldr	x0, [sp, #416]
	cbz	x0, .LBB12_147
// %bb.146:
	bl	_ZdlPv
.LBB12_147:
	ldr	x0, [sp, #392]
	cbz	x0, .LBB12_149
// %bb.148:
	bl	_ZdlPv
.LBB12_149:
	mov	x0, x19
	bl	_ZdlPv
	adrp	x0, .Lstr.30
	add	x0, x0, :lo12:.Lstr.30
	bl	puts
	mov	x20, xzr
	orr	w21, wzr, #0x1
	add	x22, sp, #8             // =8
	add	x23, sp, #264           // =264
.LBB12_150:                             // =>This Loop Header: Depth=1
                                        //     Child Loop BB12_151 Depth 2
                                        //     Child Loop BB12_155 Depth 2
	lsl	x19, x21, x20
	add	x0, sp, #8              // =8
	sub	x1, x29, #160           // =160
	mov	w2, wzr
	mov	x3, x19
	stur	wzr, [x29, #-160]
	bl	_ZL14faster_decoderPjRjjm
	mov	w8, wzr
.LBB12_151:                             //   Parent Loop BB12_150 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	rbit	x9, x19
	sub	x10, x19, #1            // =1
	clz	x9, x9
	ands	x19, x10, x19
	str	w9, [x23, w8, uxtw #2]
	add	w8, w8, #1              // =1
	b.ne	.LBB12_151
// %bb.152:                             //   in Loop: Header=BB12_150 Depth=1
	ldur	w9, [x29, #-160]
	cmp	w9, w8
	b.ne	.LBB12_169
// %bb.153:                             //   in Loop: Header=BB12_150 Depth=1
	cbz	w8, .LBB12_157
// %bb.154:                             //   in Loop: Header=BB12_150 Depth=1
	mov	x9, xzr
	mov	w8, w8
.LBB12_155:                             //   Parent Loop BB12_150 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x10, x9, #2
	ldr	w11, [x22, x10]
	ldr	w10, [x23, x10]
	cmp	w11, w10
	b.ne	.LBB12_169
// %bb.156:                             //   in Loop: Header=BB12_155 Depth=2
	add	x9, x9, #1              // =1
	cmp	x9, x8
	b.lo	.LBB12_155
.LBB12_157:                             //   in Loop: Header=BB12_150 Depth=1
	add	x20, x20, #1            // =1
	cmp	x20, #64                // =64
	b.lo	.LBB12_150
// %bb.158:
	mov	w21, #16960
	mov	x20, xzr
	movk	w21, #15, lsl #16
	add	x22, sp, #8             // =8
	add	x23, sp, #264           // =264
.LBB12_159:                             // =>This Loop Header: Depth=1
                                        //     Child Loop BB12_161 Depth 2
                                        //     Child Loop BB12_165 Depth 2
	bl	rand
	mov	w19, w0
	bl	rand
                                        // kill: def $w0 killed $w0 def $x0
	sxtw	x8, w0
	orr	x19, x8, x19, lsl #32
	add	x0, sp, #8              // =8
	sub	x1, x29, #160           // =160
	mov	w2, wzr
	mov	x3, x19
	stur	wzr, [x29, #-160]
	bl	_ZL14faster_decoderPjRjjm
	fmov	d0, x19
	cnt	v0.8b, v0.8b
	uaddlv	h0, v0.8b
	fmov	w8, s0
	cbz	w8, .LBB12_162
// %bb.160:                             //   in Loop: Header=BB12_159 Depth=1
	add	x9, sp, #264            // =264
	mov	x10, x8
.LBB12_161:                             //   Parent Loop BB12_159 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	rbit	x11, x19
	sub	x12, x19, #1            // =1
	clz	x11, x11
	subs	x10, x10, #1            // =1
	str	w11, [x9], #4
	and	x19, x12, x19
	b.ne	.LBB12_161
.LBB12_162:                             //   in Loop: Header=BB12_159 Depth=1
	ldur	w9, [x29, #-160]
	cmp	w9, w8
	b.ne	.LBB12_169
// %bb.163:                             //   in Loop: Header=BB12_159 Depth=1
	cbz	w8, .LBB12_167
// %bb.164:                             //   in Loop: Header=BB12_159 Depth=1
	mov	x9, xzr
.LBB12_165:                             //   Parent Loop BB12_159 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x10, x9, #2
	ldr	w11, [x22, x10]
	ldr	w10, [x23, x10]
	cmp	w11, w10
	b.ne	.LBB12_169
// %bb.166:                             //   in Loop: Header=BB12_165 Depth=2
	add	x9, x9, #1              // =1
	cmp	x9, x8
	b.lo	.LBB12_165
.LBB12_167:                             //   in Loop: Header=BB12_159 Depth=1
	add	x20, x20, #1            // =1
	cmp	x20, x21
	b.lo	.LBB12_159
// %bb.168:
	adrp	x0, .Lstr.48
	add	x0, x0, :lo12:.Lstr.48
	b	.LBB12_170
.LBB12_169:
	adrp	x0, .Lstr.52
	add	x0, x0, :lo12:.Lstr.52
.LBB12_170:
	bl	puts
	adrp	x0, .L.str.11
	add	x0, x0, :lo12:.L.str.11
	add	x1, sp, #264            // =264
	bl	_Z14load_text_filePKcPm
	cbz	x0, .LBB12_178
// %bb.171:
	ldr	x23, [sp, #264]
	mov	x19, x0
	lsr	x22, x23, #6
	lsl	x21, x22, #3
	mov	x0, x21
	bl	_Znam
	mov	w1, wzr
	mov	x2, x21
	mov	x20, x0
	bl	memset
	ands	x8, x23, #0xffffffffffffffc0
	b.eq	.LBB12_177
// %bb.172:
	mov	x9, xzr
	orr	w10, wzr, #0x1
.LBB12_173:                             // =>This Inner Loop Header: Depth=1
	ldrb	w11, [x19, x9]
	cmp	w11, #44                // =44
	b.eq	.LBB12_175
// %bb.174:                             //   in Loop: Header=BB12_173 Depth=1
	cmp	w11, #31                // =31
	b.hi	.LBB12_176
.LBB12_175:                             //   in Loop: Header=BB12_173 Depth=1
	lsr	x11, x9, #3
	and	x11, x11, #0x1ffffffffffffff8
	ldr	x12, [x20, x11]
	lsl	x13, x10, x9
	orr	x12, x12, x13
	str	x12, [x20, x11]
.LBB12_176:                             //   in Loop: Header=BB12_173 Depth=1
	add	x9, x9, #1              // =1
	cmp	x8, x9
	b.ne	.LBB12_173
.LBB12_177:
	mov	x0, x19
	bl	_ZdaPv
	b	.LBB12_179
.LBB12_178:
	mov	x20, xzr
                                        // implicit-def: $x22
.LBB12_179:
	lsl	x25, x22, #6
	lsl	x8, x22, #8
	cmp	xzr, x25, lsr #62
	csinv	x0, x8, xzr, eq
	bl	_Znam
	mov	x21, x0
.Ltmp167:
	orr	w0, wzr, #0x4
	bl	_Znwm
.Ltmp168:
// %bb.180:
	mov	x19, x0
	str	wzr, [x0]
.Ltmp170:
	orr	w0, wzr, #0x8
	bl	_Znwm
.Ltmp171:
// %bb.181:
	ldr	w8, [x19]
	orr	w9, wzr, #0x1
	mov	x24, x0
	stp	w8, w9, [x0]
	mov	x0, x19
	bl	_ZdlPv
.Ltmp173:
	orr	w0, wzr, #0x10
	bl	_Znwm
.Ltmp174:
// %bb.182:
	ldr	x8, [x24]
	mov	w9, #5
	mov	x23, x0
	str	w9, [x0, #8]
	str	x8, [x0]
	mov	x0, x24
	bl	_ZdlPv
	orr	w8, wzr, #0x2
	str	w8, [x23, #12]
.Ltmp176:
	orr	w0, wzr, #0x20
	bl	_Znwm
.Ltmp177:
// %bb.183:
	ldr	q0, [x23]
	orr	w8, wzr, #0x3
	mov	x19, x0
	str	w8, [x0, #16]
	str	q0, [x0]
	mov	x0, x23
	bl	_ZdlPv
	stp	xzr, xzr, [sp, #16]
	str	xzr, [sp, #8]
.Ltmp179:
	mov	w0, #20
	bl	_Znwm
.Ltmp180:
// %bb.184:
	ldr	w8, [x19, #16]
	ldr	q0, [x19]
	add	x9, x0, #20             // =20
	str	x9, [sp, #24]
	str	w8, [x0, #16]
	str	q0, [x0]
	stp	x0, x9, [sp, #8]
.Ltmp182:
	add	x0, sp, #264            // =264
	add	x1, sp, #8              // =8
	bl	_ZN11LinuxEventsILi0EEC2ESt6vectorIiSaIiEE
.Ltmp183:
// %bb.185:
	ldr	x0, [sp, #8]
	cbz	x0, .LBB12_187
// %bb.186:
	bl	_ZdlPv
.LBB12_187:
	stp	xzr, xzr, [x29, #-152]
	stp	xzr, xzr, [x29, #-168]
	stp	xzr, xzr, [x29, #-184]
.Ltmp185:
	mov	w0, #40
	bl	_Znwm
.Ltmp186:
// %bb.188:
	ldp	x24, x8, [x29, #-160]
	mov	x23, x0
	movi	v0.2d, #0000000000000000
	str	xzr, [x0, #32]
	subs	x2, x8, x24
	stp	q0, q0, [x0]
	b.eq	.LBB12_190
// %bb.189:
	mov	x0, x23
	mov	x1, x24
	bl	memmove
.LBB12_190:
	cbz	x24, .LBB12_192
// %bb.191:
	mov	x0, x24
	bl	_ZdlPv
.LBB12_192:
	mov	w26, wzr
	add	x8, x23, #40            // =40
	stur	x8, [x29, #-144]
	stp	x23, x8, [x29, #-160]
.LBB12_193:                             // =>This Loop Header: Depth=1
                                        //     Child Loop BB12_196 Depth 2
                                        //       Child Loop BB12_198 Depth 3
.Ltmp188:
	add	x0, sp, #264            // =264
	bl	_ZN11LinuxEventsILi0EE5startEv
.Ltmp189:
// %bb.194:                             //   in Loop: Header=BB12_193 Depth=1
	cbz	x22, .LBB12_200
// %bb.195:                             //   in Loop: Header=BB12_193 Depth=1
	mov	x8, xzr
	mov	w9, wzr
.LBB12_196:                             //   Parent Loop BB12_193 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB12_198 Depth 3
	ldr	x10, [x20, x8, lsl #3]
	fmov	d0, x10
	cnt	v0.8b, v0.8b
	uaddlv	h0, v0.8b
	fmov	w11, s0
	add	w23, w9, w11
	cbz	x10, .LBB12_199
// %bb.197:                             //   in Loop: Header=BB12_196 Depth=2
	add	w9, w9, #7              // =7
.LBB12_198:                             //   Parent Loop BB12_193 Depth=1
                                        //     Parent Loop BB12_196 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	rbit	x12, x10
	clz	x12, x12
	sub	w11, w9, #7             // =7
	add	w12, w12, w8
	str	w12, [x21, w11, uxtw #2]
	sub	x11, x10, #1            // =1
	and	x10, x11, x10
	rbit	x11, x10
	clz	x11, x11
	sub	w12, w9, #6             // =6
	add	w11, w11, w8
	str	w11, [x21, w12, uxtw #2]
	sub	x12, x10, #1            // =1
	and	x10, x12, x10
	rbit	x12, x10
	clz	x12, x12
	sub	w11, w9, #5             // =5
	add	w12, w12, w8
	str	w12, [x21, w11, uxtw #2]
	sub	x12, x10, #1            // =1
	and	x10, x12, x10
	rbit	x12, x10
	clz	x12, x12
	sub	w11, w9, #4             // =4
	add	w12, w12, w8
	str	w12, [x21, w11, uxtw #2]
	sub	x12, x10, #1            // =1
	and	x10, x12, x10
	rbit	x12, x10
	clz	x12, x12
	sub	w11, w9, #3             // =3
	add	w12, w12, w8
	str	w12, [x21, w11, uxtw #2]
	sub	x12, x10, #1            // =1
	and	x10, x12, x10
	rbit	x12, x10
	clz	x12, x12
	sub	w11, w9, #2             // =2
	add	w12, w12, w8
	str	w12, [x21, w11, uxtw #2]
	sub	x12, x10, #1            // =1
	and	x10, x12, x10
	sub	x12, x10, #1            // =1
	and	x12, x12, x10
	rbit	x10, x10
	clz	x10, x10
	sub	w11, w9, #1             // =1
	add	w10, w10, w8
	str	w10, [x21, w11, uxtw #2]
	rbit	x10, x12
	sub	x11, x12, #1            // =1
	clz	x13, x10
	ands	x10, x11, x12
	add	w11, w13, w8
	str	w11, [x21, w9, uxtw #2]
	add	w9, w9, #8              // =8
	b.ne	.LBB12_198
.LBB12_199:                             //   in Loop: Header=BB12_196 Depth=2
	add	x8, x8, #1              // =1
	cmp	x8, x22
	mov	w9, w23
	b.ne	.LBB12_196
	b	.LBB12_201
.LBB12_200:                             //   in Loop: Header=BB12_193 Depth=1
	mov	w23, wzr
.LBB12_201:                             //   in Loop: Header=BB12_193 Depth=1
.Ltmp190:
	add	x0, sp, #264            // =264
	sub	x1, x29, #160           // =160
	bl	_ZN11LinuxEventsILi0EE3endERSt6vectorIySaIyEE
.Ltmp191:
// %bb.202:                             //   in Loop: Header=BB12_193 Depth=1
	ldp	x24, x8, [x29, #-176]
	cmp	x24, x8
	b.eq	.LBB12_207
// %bb.203:                             //   in Loop: Header=BB12_193 Depth=1
	ldp	x9, x8, [x29, #-160]
	stp	xzr, xzr, [x24]
	str	xzr, [x24, #16]
	subs	x0, x8, x9
	asr	x27, x0, #3
	b.eq	.LBB12_208
// %bb.204:                             //   in Loop: Header=BB12_193 Depth=1
	lsr	x8, x27, #61
	cbnz	x8, .LBB12_436
// %bb.205:                             //   in Loop: Header=BB12_193 Depth=1
.Ltmp192:
	bl	_Znwm
.Ltmp193:
// %bb.206:                             //   in Loop: Header=BB12_193 Depth=1
	mov	x28, x0
	b	.LBB12_209
.LBB12_207:                             //   in Loop: Header=BB12_193 Depth=1
.Ltmp197:
	sub	x0, x29, #184           // =184
	sub	x2, x29, #160           // =160
	mov	x1, x24
	bl	_ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_
.Ltmp198:
	b	.LBB12_212
.LBB12_208:                             //   in Loop: Header=BB12_193 Depth=1
	mov	x28, xzr
.LBB12_209:                             //   in Loop: Header=BB12_193 Depth=1
	add	x8, x28, x27, lsl #3
	stp	x28, x0, [x24]
	str	x8, [x24, #16]
	ldp	x1, x8, [x29, #-160]
	subs	x2, x8, x1
	asr	x27, x2, #3
	b.eq	.LBB12_211
// %bb.210:                             //   in Loop: Header=BB12_193 Depth=1
	bl	memmove
.LBB12_211:                             //   in Loop: Header=BB12_193 Depth=1
	add	x8, x28, x27, lsl #3
	str	x8, [x24, #8]
	ldur	x8, [x29, #-176]
	add	x8, x8, #24             // =24
	stur	x8, [x29, #-176]
.LBB12_212:                             //   in Loop: Header=BB12_193 Depth=1
	add	w26, w26, #1            // =1
	cmp	w26, #10                // =10
	b.lo	.LBB12_193
// %bb.213:
	ldr	w8, [x21]
	cbnz	w8, .LBB12_215
// %bb.214:
	adrp	x0, .L.str.3
	add	x0, x0, :lo12:.L.str.3
	bl	printf
.LBB12_215:
	ucvtf	d11, w23
	ucvtf	d10, x25
	fdiv	d0, d11, d10
	adrp	x0, .L.str.4
	fmul	d0, d0, d8
	add	x0, x0, :lo12:.L.str.4
	mov	w1, w23
	mov	x2, x22
	bl	printf
	ucvtf	d12, x22
	fmul	d0, d12, d9
	adrp	x0, .L.str.5
	fdiv	d0, d0, d11
	add	x0, x0, :lo12:.L.str.5
	bl	printf
	ldp	x0, x1, [x29, #-184]
	subs	x8, x1, x0
	b.eq	.LBB12_219
// %bb.216:
	mov	x10, #-6148914691236517206
	asr	x9, x8, #3
	movk	x10, #43691
	mul	x9, x9, x10
	mov	x10, #-6148914691236517206
	movk	x10, #2730, lsl #48
	cmp	x9, x10
	b.hi	.LBB12_444
// %bb.217:
.Ltmp200:
	mov	x0, x8
	bl	_Znwm
.Ltmp201:
// %bb.218:
	mov	x23, x0
	ldp	x0, x1, [x29, #-184]
	mov	x24, x23
	b	.LBB12_220
.LBB12_219:
	mov	x23, xzr
	mov	x24, xzr
.LBB12_220:
.Ltmp205:
	mov	x2, x24
	bl	_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_
.Ltmp206:
// %bb.221:
	mov	x25, x0
	subs	x27, x0, x24
	b.eq	.LBB12_227
// %bb.222:
	ldp	x8, x9, [x24]
	subs	x0, x9, x8
	b.eq	.LBB12_228
// %bb.223:
	asr	x8, x0, #3
	lsr	x8, x8, #61
	cbnz	x8, .LBB12_454
// %bb.224:
.Ltmp208:
	bl	_Znwm
.Ltmp209:
// %bb.225:
	ldp	x1, x8, [x24]
	mov	x26, x0
	subs	x2, x8, x1
	b.eq	.LBB12_229
// %bb.226:
	mov	x0, x26
	asr	x28, x2, #3
	bl	memmove
	mov	w8, wzr
	b	.LBB12_230
.LBB12_227:
	mov	x22, xzr
	cbnz	x24, .LBB12_258
	b	.LBB12_259
.LBB12_228:
	mov	x22, xzr
	mov	x26, xzr
	mov	x28, xzr
	orr	w8, wzr, #0x1
	b	.LBB12_231
.LBB12_229:
	mov	x28, xzr
	orr	w8, wzr, #0x1
.LBB12_230:
	mov	x22, x26
.LBB12_231:
	mov	x10, #-6148914691236517206
	asr	x9, x27, #3
	movk	x10, #43691
	mul	x9, x9, x10
	cmp	x9, #1                  // =1
	mov	x27, x24
	b.ls	.LBB12_254
// %bb.232:
	cmp	x28, #1                 // =1
	csinc	x12, x28, xzr, hi
	add	x10, x22, #16           // =16
	orr	w11, wzr, #0x1
	add	x13, x22, x12, lsl #3
	and	x14, x12, #0xfffffffffffffffc
	orr	w15, wzr, #0x18
	b	.LBB12_244
.LBB12_233:                             //   in Loop: Header=BB12_244 Depth=1
	add	x17, x16, #16           // =16
	mov	x18, x10
	mov	x0, x14
.LBB12_234:                             //   Parent Loop BB12_244 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldp	q1, q0, [x17, #-16]
	ldp	q3, q2, [x18, #-16]
	cmhi	v3.2d, v3.2d, v1.2d
	xtn	v3.2s, v3.2d
	fmov	w1, s3
	tbz	w1, #0, .LBB12_236
// %bb.235:                             //   in Loop: Header=BB12_234 Depth=2
	stur	d1, [x18, #-16]
.LBB12_236:                             //   in Loop: Header=BB12_234 Depth=2
	mov	w1, v3.s[1]
	tbz	w1, #0, .LBB12_238
// %bb.237:                             //   in Loop: Header=BB12_234 Depth=2
	sub	x1, x18, #8             // =8
	st1	{ v1.d }[1], [x1]
.LBB12_238:                             //   in Loop: Header=BB12_234 Depth=2
	cmhi	v1.2d, v2.2d, v0.2d
	xtn	v1.2s, v1.2d
	fmov	w1, s1
	tbz	w1, #0, .LBB12_240
// %bb.239:                             //   in Loop: Header=BB12_234 Depth=2
	str	d0, [x18]
.LBB12_240:                             //   in Loop: Header=BB12_234 Depth=2
	mov	w1, v1.s[1]
	tbz	w1, #0, .LBB12_242
// %bb.241:                             //   in Loop: Header=BB12_234 Depth=2
	add	x1, x18, #8             // =8
	st1	{ v0.d }[1], [x1]
.LBB12_242:                             //   in Loop: Header=BB12_234 Depth=2
	subs	x0, x0, #4              // =4
	add	x18, x18, #32           // =32
	add	x17, x17, #32           // =32
	b.ne	.LBB12_234
// %bb.243:                             //   in Loop: Header=BB12_244 Depth=1
	cmp	x12, x14
	mov	x17, x14
	b.ne	.LBB12_249
	b	.LBB12_252
.LBB12_244:                             // =>This Loop Header: Depth=1
                                        //     Child Loop BB12_234 Depth 2
                                        //     Child Loop BB12_249 Depth 2
	tbnz	w8, #0, .LBB12_252
// %bb.245:                             //   in Loop: Header=BB12_244 Depth=1
	mul	x16, x11, x15
	ldr	x16, [x24, x16]
	cmp	x12, #3                 // =3
	b.ls	.LBB12_248
// %bb.246:                             //   in Loop: Header=BB12_244 Depth=1
	add	x17, x16, x12, lsl #3
	cmp	x26, x17
	b.hs	.LBB12_233
// %bb.247:                             //   in Loop: Header=BB12_244 Depth=1
	cmp	x16, x13
	b.hs	.LBB12_233
.LBB12_248:                             //   in Loop: Header=BB12_244 Depth=1
	mov	x17, xzr
.LBB12_249:                             //   Parent Loop BB12_244 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x0, x17, #3
	ldr	x18, [x16, x0]
	ldr	x0, [x22, x0]
	cmp	x18, x0
	b.hs	.LBB12_251
// %bb.250:                             //   in Loop: Header=BB12_249 Depth=2
	str	x18, [x22, x17, lsl #3]
.LBB12_251:                             //   in Loop: Header=BB12_249 Depth=2
	add	x17, x17, #1            // =1
	cmp	x17, x28
	b.lo	.LBB12_249
.LBB12_252:                             //   in Loop: Header=BB12_244 Depth=1
	add	x11, x11, #1            // =1
	cmp	x11, x9
	b.ne	.LBB12_244
// %bb.253:
	mov	x27, x24
.LBB12_254:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x27]
	cbz	x0, .LBB12_256
// %bb.255:                             //   in Loop: Header=BB12_254 Depth=1
	bl	_ZdlPv
.LBB12_256:                             //   in Loop: Header=BB12_254 Depth=1
	add	x27, x27, #24           // =24
	cmp	x25, x27
	b.ne	.LBB12_254
// %bb.257:
	cbz	x24, .LBB12_259
.LBB12_258:
	mov	x0, x23
	bl	_ZdlPv
.LBB12_259:
	ldp	x0, x1, [x29, #-184]
	mov	x9, #-6148914691236517206
	movk	x9, #43691
	stp	xzr, xzr, [x29, #-232]
	subs	x8, x1, x0
	asr	x10, x8, #3
	mul	x25, x10, x9
	stur	xzr, [x29, #-216]
	b.eq	.LBB12_263
// %bb.260:
	mov	x9, #-6148914691236517206
	movk	x9, #2730, lsl #48
	cmp	x25, x9
	b.hi	.LBB12_446
// %bb.261:
.Ltmp213:
	mov	x0, x8
	bl	_Znwm
.Ltmp214:
// %bb.262:
	mov	x23, x0
	ldp	x0, x1, [x29, #-184]
	mov	x24, x23
	b	.LBB12_264
.LBB12_263:
	mov	x23, xzr
	mov	x24, xzr
.LBB12_264:
	orr	w8, wzr, #0x18
	madd	x8, x25, x8, x24
	stp	x23, x23, [x29, #-232]
	stur	x8, [x29, #-216]
.Ltmp218:
	mov	x2, x24
	bl	_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_
.Ltmp219:
// %bb.265:
	mov	x25, x0
	stur	x0, [x29, #-224]
.Ltmp221:
	sub	x8, x29, #208           // =208
	sub	x0, x29, #232           // =232
	bl	_Z16compute_averagesSt6vectorIS_IySaIyEESaIS1_EE
.Ltmp222:
// %bb.266:
	cmp	x24, x25
	b.eq	.LBB12_271
// %bb.267:
	mov	x26, x24
.LBB12_268:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x26]
	cbz	x0, .LBB12_270
// %bb.269:                             //   in Loop: Header=BB12_268 Depth=1
	bl	_ZdlPv
.LBB12_270:                             //   in Loop: Header=BB12_268 Depth=1
	add	x26, x26, #24           // =24
	cmp	x25, x26
	b.ne	.LBB12_268
.LBB12_271:
	cbz	x24, .LBB12_273
// %bb.272:
	mov	x0, x23
	bl	_ZdlPv
.LBB12_273:
	ldp	d1, d0, [x22]
	adrp	x0, .L.str.6
	add	x0, x0, :lo12:.L.str.6
	ucvtf	d4, d0
	ucvtf	d3, d1
	fdiv	d0, d4, d3
	fdiv	d1, d3, d11
	fdiv	d2, d4, d11
	fdiv	d3, d3, d12
	fdiv	d4, d4, d12
	bl	printf
	ldp	d0, d1, [x22]
	adrp	x0, .L.str.7
	add	x0, x0, :lo12:.L.str.7
	ucvtf	d0, d0
	ucvtf	d1, d1
	fdiv	d0, d0, d10
	fdiv	d1, d1, d10
	bl	printf
	ldp	x1, x2, [x22]
	ldp	x3, x4, [x22, #16]
	ldr	x5, [x22, #32]
	adrp	x0, .L.str.8
	add	x0, x0, :lo12:.L.str.8
	bl	printf
	ldur	x23, [x29, #-208]
	adrp	x0, .L.str.9
	add	x0, x0, :lo12:.L.str.9
	ldp	d0, d1, [x23]
	ldp	d2, d3, [x23, #16]
	ldr	d4, [x23, #32]
	bl	printf
	mov	w0, #10
	bl	putchar
	cbz	x20, .LBB12_275
// %bb.274:
	mov	x0, x20
	bl	_ZdaPv
.LBB12_275:
	mov	x0, x21
	bl	_ZdaPv
	cbz	x23, .LBB12_277
// %bb.276:
	mov	x0, x23
	bl	_ZdlPv
.LBB12_277:
	cbz	x22, .LBB12_279
// %bb.278:
	mov	x0, x22
	bl	_ZdlPv
.LBB12_279:
	ldp	x20, x21, [x29, #-184]
	cmp	x20, x21
	b.eq	.LBB12_284
.LBB12_280:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x20]
	cbz	x0, .LBB12_282
// %bb.281:                             //   in Loop: Header=BB12_280 Depth=1
	bl	_ZdlPv
.LBB12_282:                             //   in Loop: Header=BB12_280 Depth=1
	add	x20, x20, #24           // =24
	cmp	x21, x20
	b.ne	.LBB12_280
// %bb.283:
	ldur	x20, [x29, #-184]
.LBB12_284:
	cbz	x20, .LBB12_286
// %bb.285:
	mov	x0, x20
	bl	_ZdlPv
.LBB12_286:
	ldur	x0, [x29, #-160]
	cbz	x0, .LBB12_288
// %bb.287:
	bl	_ZdlPv
.LBB12_288:
	ldr	w0, [sp, #264]
.Ltmp227:
	bl	close
.Ltmp228:
// %bb.289:
	ldr	x0, [sp, #416]
	cbz	x0, .LBB12_291
// %bb.290:
	bl	_ZdlPv
.LBB12_291:
	ldr	x0, [sp, #392]
	cbz	x0, .LBB12_293
// %bb.292:
	bl	_ZdlPv
.LBB12_293:
	mov	x0, x19
	bl	_ZdlPv
	adrp	x0, .Lstr.31
	add	x0, x0, :lo12:.Lstr.31
	bl	puts
	bl	_Z4unitIXadL_Z13basic_decoderPjRjjmEEEbv
	adrp	x0, .L.str.11
	add	x0, x0, :lo12:.L.str.11
	mov	w1, #44
	bl	_Z4testIXadL_Z13basic_decoderPjRjjmEEEvPKcc
	adrp	x0, .Lstr.32
	add	x0, x0, :lo12:.Lstr.32
	bl	puts
	mov	x20, xzr
	orr	w21, wzr, #0x1
	add	x22, sp, #8             // =8
	add	x23, sp, #264           // =264
.LBB12_294:                             // =>This Loop Header: Depth=1
                                        //     Child Loop BB12_295 Depth 2
                                        //     Child Loop BB12_299 Depth 2
	lsl	x19, x21, x20
	add	x0, sp, #8              // =8
	sub	x1, x29, #160           // =160
	mov	w2, wzr
	mov	x3, x19
	stur	wzr, [x29, #-160]
	bl	_ZL14faster_decoderPjRjjm
	mov	w8, wzr
.LBB12_295:                             //   Parent Loop BB12_294 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	rbit	x9, x19
	sub	x10, x19, #1            // =1
	clz	x9, x9
	ands	x19, x10, x19
	str	w9, [x23, w8, uxtw #2]
	add	w8, w8, #1              // =1
	b.ne	.LBB12_295
// %bb.296:                             //   in Loop: Header=BB12_294 Depth=1
	ldur	w9, [x29, #-160]
	cmp	w9, w8
	b.ne	.LBB12_313
// %bb.297:                             //   in Loop: Header=BB12_294 Depth=1
	cbz	w8, .LBB12_301
// %bb.298:                             //   in Loop: Header=BB12_294 Depth=1
	mov	x9, xzr
	mov	w8, w8
.LBB12_299:                             //   Parent Loop BB12_294 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x10, x9, #2
	ldr	w11, [x22, x10]
	ldr	w10, [x23, x10]
	cmp	w11, w10
	b.ne	.LBB12_313
// %bb.300:                             //   in Loop: Header=BB12_299 Depth=2
	add	x9, x9, #1              // =1
	cmp	x9, x8
	b.lo	.LBB12_299
.LBB12_301:                             //   in Loop: Header=BB12_294 Depth=1
	add	x20, x20, #1            // =1
	cmp	x20, #64                // =64
	b.lo	.LBB12_294
// %bb.302:
	mov	w21, #16960
	mov	x20, xzr
	movk	w21, #15, lsl #16
	add	x22, sp, #8             // =8
	add	x23, sp, #264           // =264
.LBB12_303:                             // =>This Loop Header: Depth=1
                                        //     Child Loop BB12_305 Depth 2
                                        //     Child Loop BB12_309 Depth 2
	bl	rand
	mov	w19, w0
	bl	rand
                                        // kill: def $w0 killed $w0 def $x0
	sxtw	x8, w0
	orr	x19, x8, x19, lsl #32
	add	x0, sp, #8              // =8
	sub	x1, x29, #160           // =160
	mov	w2, wzr
	mov	x3, x19
	stur	wzr, [x29, #-160]
	bl	_ZL14faster_decoderPjRjjm
	fmov	d0, x19
	cnt	v0.8b, v0.8b
	uaddlv	h0, v0.8b
	fmov	w8, s0
	cbz	w8, .LBB12_306
// %bb.304:                             //   in Loop: Header=BB12_303 Depth=1
	add	x9, sp, #264            // =264
	mov	x10, x8
.LBB12_305:                             //   Parent Loop BB12_303 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	rbit	x11, x19
	sub	x12, x19, #1            // =1
	clz	x11, x11
	subs	x10, x10, #1            // =1
	str	w11, [x9], #4
	and	x19, x12, x19
	b.ne	.LBB12_305
.LBB12_306:                             //   in Loop: Header=BB12_303 Depth=1
	ldur	w9, [x29, #-160]
	cmp	w9, w8
	b.ne	.LBB12_313
// %bb.307:                             //   in Loop: Header=BB12_303 Depth=1
	cbz	w8, .LBB12_311
// %bb.308:                             //   in Loop: Header=BB12_303 Depth=1
	mov	x9, xzr
.LBB12_309:                             //   Parent Loop BB12_303 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x10, x9, #2
	ldr	w11, [x22, x10]
	ldr	w10, [x23, x10]
	cmp	w11, w10
	b.ne	.LBB12_313
// %bb.310:                             //   in Loop: Header=BB12_309 Depth=2
	add	x9, x9, #1              // =1
	cmp	x9, x8
	b.lo	.LBB12_309
.LBB12_311:                             //   in Loop: Header=BB12_303 Depth=1
	add	x20, x20, #1            // =1
	cmp	x20, x21
	b.lo	.LBB12_303
// %bb.312:
	adrp	x0, .Lstr.48
	add	x0, x0, :lo12:.Lstr.48
	b	.LBB12_314
.LBB12_313:
	adrp	x0, .Lstr.52
	add	x0, x0, :lo12:.Lstr.52
.LBB12_314:
	bl	puts
	adrp	x0, .L.str.11
	add	x0, x0, :lo12:.L.str.11
	add	x1, sp, #264            // =264
	bl	_Z14load_text_filePKcPm
	cbz	x0, .LBB12_322
// %bb.315:
	ldr	x23, [sp, #264]
	mov	x19, x0
	lsr	x22, x23, #6
	lsl	x21, x22, #3
	mov	x0, x21
	bl	_Znam
	mov	w1, wzr
	mov	x2, x21
	mov	x20, x0
	bl	memset
	ands	x8, x23, #0xffffffffffffffc0
	b.eq	.LBB12_321
// %bb.316:
	mov	x9, xzr
	orr	w10, wzr, #0x1
.LBB12_317:                             // =>This Inner Loop Header: Depth=1
	ldrb	w11, [x19, x9]
	cmp	w11, #44                // =44
	b.eq	.LBB12_319
// %bb.318:                             //   in Loop: Header=BB12_317 Depth=1
	cmp	w11, #31                // =31
	b.hi	.LBB12_320
.LBB12_319:                             //   in Loop: Header=BB12_317 Depth=1
	lsr	x11, x9, #3
	and	x11, x11, #0x1ffffffffffffff8
	ldr	x12, [x20, x11]
	lsl	x13, x10, x9
	orr	x12, x12, x13
	str	x12, [x20, x11]
.LBB12_320:                             //   in Loop: Header=BB12_317 Depth=1
	add	x9, x9, #1              // =1
	cmp	x8, x9
	b.ne	.LBB12_317
.LBB12_321:
	mov	x0, x19
	bl	_ZdaPv
	b	.LBB12_323
.LBB12_322:
	mov	x20, xzr
                                        // implicit-def: $x22
.LBB12_323:
	lsl	x25, x22, #6
	lsl	x8, x22, #8
	cmp	xzr, x25, lsr #62
	csinv	x0, x8, xzr, eq
	bl	_Znam
	mov	x21, x0
.Ltmp230:
	orr	w0, wzr, #0x4
	bl	_Znwm
.Ltmp231:
// %bb.324:
	mov	x19, x0
	str	wzr, [x0]
.Ltmp233:
	orr	w0, wzr, #0x8
	bl	_Znwm
.Ltmp234:
// %bb.325:
	ldr	w8, [x19]
	orr	w9, wzr, #0x1
	mov	x24, x0
	stp	w8, w9, [x0]
	mov	x0, x19
	bl	_ZdlPv
.Ltmp236:
	orr	w0, wzr, #0x10
	bl	_Znwm
.Ltmp237:
// %bb.326:
	ldr	x8, [x24]
	mov	w9, #5
	mov	x23, x0
	str	w9, [x0, #8]
	str	x8, [x0]
	mov	x0, x24
	bl	_ZdlPv
	orr	w8, wzr, #0x2
	str	w8, [x23, #12]
.Ltmp239:
	orr	w0, wzr, #0x20
	bl	_Znwm
.Ltmp240:
// %bb.327:
	ldr	q0, [x23]
	orr	w8, wzr, #0x3
	mov	x19, x0
	str	w8, [x0, #16]
	str	q0, [x0]
	mov	x0, x23
	bl	_ZdlPv
	stp	xzr, xzr, [sp, #16]
	str	xzr, [sp, #8]
.Ltmp242:
	mov	w0, #20
	bl	_Znwm
.Ltmp243:
// %bb.328:
	ldr	w8, [x19, #16]
	ldr	q0, [x19]
	add	x9, x0, #20             // =20
	str	x9, [sp, #24]
	str	w8, [x0, #16]
	str	q0, [x0]
	stp	x0, x9, [sp, #8]
.Ltmp245:
	add	x0, sp, #264            // =264
	add	x1, sp, #8              // =8
	bl	_ZN11LinuxEventsILi0EEC2ESt6vectorIiSaIiEE
.Ltmp246:
// %bb.329:
	ldr	x0, [sp, #8]
	cbz	x0, .LBB12_331
// %bb.330:
	bl	_ZdlPv
.LBB12_331:
	stp	xzr, xzr, [x29, #-152]
	stp	xzr, xzr, [x29, #-168]
	stp	xzr, xzr, [x29, #-184]
.Ltmp248:
	mov	w0, #40
	bl	_Znwm
.Ltmp249:
// %bb.332:
	ldp	x24, x8, [x29, #-160]
	mov	x23, x0
	movi	v0.2d, #0000000000000000
	str	xzr, [x0, #32]
	subs	x2, x8, x24
	stp	q0, q0, [x0]
	b.eq	.LBB12_334
// %bb.333:
	mov	x0, x23
	mov	x1, x24
	bl	memmove
.LBB12_334:
	cbz	x24, .LBB12_336
// %bb.335:
	mov	x0, x24
	bl	_ZdlPv
.LBB12_336:
	mov	w24, wzr
	add	x8, x23, #40            // =40
	stur	x8, [x29, #-144]
	stp	x23, x8, [x29, #-160]
	stur	wzr, [x29, #-116]
.LBB12_337:                             // =>This Loop Header: Depth=1
                                        //     Child Loop BB12_340 Depth 2
	stur	wzr, [x29, #-116]
.Ltmp251:
	add	x0, sp, #264            // =264
	bl	_ZN11LinuxEventsILi0EE5startEv
.Ltmp252:
// %bb.338:                             //   in Loop: Header=BB12_337 Depth=1
	cbz	x22, .LBB12_341
// %bb.339:                             //   in Loop: Header=BB12_337 Depth=1
	mov	x23, xzr
.LBB12_340:                             //   Parent Loop BB12_337 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldr	x3, [x20, x23, lsl #3]
	sub	x1, x29, #116           // =116
	mov	x0, x21
	mov	w2, w23
	bl	_ZL14faster_decoderPjRjjm
	add	x23, x23, #1            // =1
	cmp	x22, x23
	b.ne	.LBB12_340
.LBB12_341:                             //   in Loop: Header=BB12_337 Depth=1
.Ltmp253:
	add	x0, sp, #264            // =264
	sub	x1, x29, #160           // =160
	bl	_ZN11LinuxEventsILi0EE3endERSt6vectorIySaIyEE
.Ltmp254:
// %bb.342:                             //   in Loop: Header=BB12_337 Depth=1
	ldp	x23, x8, [x29, #-176]
	cmp	x23, x8
	b.eq	.LBB12_347
// %bb.343:                             //   in Loop: Header=BB12_337 Depth=1
	ldp	x9, x8, [x29, #-160]
	stp	xzr, xzr, [x23]
	str	xzr, [x23, #16]
	subs	x0, x8, x9
	asr	x26, x0, #3
	b.eq	.LBB12_348
// %bb.344:                             //   in Loop: Header=BB12_337 Depth=1
	lsr	x8, x26, #61
	cbnz	x8, .LBB12_438
// %bb.345:                             //   in Loop: Header=BB12_337 Depth=1
.Ltmp255:
	bl	_Znwm
.Ltmp256:
// %bb.346:                             //   in Loop: Header=BB12_337 Depth=1
	mov	x27, x0
	b	.LBB12_349
.LBB12_347:                             //   in Loop: Header=BB12_337 Depth=1
.Ltmp260:
	sub	x0, x29, #184           // =184
	sub	x2, x29, #160           // =160
	mov	x1, x23
	bl	_ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_
.Ltmp261:
	b	.LBB12_352
.LBB12_348:                             //   in Loop: Header=BB12_337 Depth=1
	mov	x27, xzr
.LBB12_349:                             //   in Loop: Header=BB12_337 Depth=1
	add	x8, x27, x26, lsl #3
	stp	x27, x0, [x23]
	str	x8, [x23, #16]
	ldp	x1, x8, [x29, #-160]
	subs	x2, x8, x1
	asr	x26, x2, #3
	b.eq	.LBB12_351
// %bb.350:                             //   in Loop: Header=BB12_337 Depth=1
	bl	memmove
.LBB12_351:                             //   in Loop: Header=BB12_337 Depth=1
	add	x8, x27, x26, lsl #3
	str	x8, [x23, #8]
	ldur	x8, [x29, #-176]
	add	x8, x8, #24             // =24
	stur	x8, [x29, #-176]
.LBB12_352:                             //   in Loop: Header=BB12_337 Depth=1
	add	w24, w24, #1            // =1
	cmp	w24, #10                // =10
	b.lo	.LBB12_337
// %bb.353:
	ldr	w8, [x21]
	cbnz	w8, .LBB12_355
// %bb.354:
	adrp	x0, .L.str.3
	add	x0, x0, :lo12:.L.str.3
	bl	printf
.LBB12_355:
	ldur	w1, [x29, #-116]
	ucvtf	d10, x25
	adrp	x0, .L.str.4
	add	x0, x0, :lo12:.L.str.4
	ucvtf	d0, w1
	fdiv	d0, d0, d10
	fmul	d0, d0, d8
	mov	x2, x22
	bl	printf
	ldur	s0, [x29, #-116]
	ucvtf	d8, x22
	fmul	d1, d8, d9
	adrp	x0, .L.str.5
	ucvtf	d0, d0
	fdiv	d0, d1, d0
	add	x0, x0, :lo12:.L.str.5
	bl	printf
	ldp	x0, x1, [x29, #-184]
	subs	x8, x1, x0
	b.eq	.LBB12_359
// %bb.356:
	mov	x10, #-6148914691236517206
	asr	x9, x8, #3
	movk	x10, #43691
	mul	x9, x9, x10
	mov	x10, #-6148914691236517206
	movk	x10, #2730, lsl #48
	cmp	x9, x10
	b.hi	.LBB12_448
// %bb.357:
.Ltmp263:
	mov	x0, x8
	bl	_Znwm
.Ltmp264:
// %bb.358:
	mov	x23, x0
	ldp	x0, x1, [x29, #-184]
	mov	x24, x23
	b	.LBB12_360
.LBB12_359:
	mov	x23, xzr
	mov	x24, xzr
.LBB12_360:
.Ltmp268:
	mov	x2, x24
	bl	_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_
.Ltmp269:
// %bb.361:
	mov	x25, x0
	subs	x27, x0, x24
	b.eq	.LBB12_367
// %bb.362:
	ldp	x8, x9, [x24]
	subs	x0, x9, x8
	b.eq	.LBB12_368
// %bb.363:
	asr	x8, x0, #3
	lsr	x8, x8, #61
	cbnz	x8, .LBB12_456
// %bb.364:
.Ltmp271:
	bl	_Znwm
.Ltmp272:
// %bb.365:
	ldp	x1, x8, [x24]
	mov	x26, x0
	subs	x2, x8, x1
	b.eq	.LBB12_369
// %bb.366:
	mov	x0, x26
	asr	x28, x2, #3
	bl	memmove
	mov	w8, wzr
	b	.LBB12_370
.LBB12_367:
	mov	x22, xzr
	cbnz	x24, .LBB12_398
	b	.LBB12_399
.LBB12_368:
	mov	x22, xzr
	mov	x26, xzr
	mov	x28, xzr
	orr	w8, wzr, #0x1
	b	.LBB12_371
.LBB12_369:
	mov	x28, xzr
	orr	w8, wzr, #0x1
.LBB12_370:
	mov	x22, x26
.LBB12_371:
	mov	x10, #-6148914691236517206
	asr	x9, x27, #3
	movk	x10, #43691
	mul	x9, x9, x10
	cmp	x9, #1                  // =1
	mov	x27, x24
	b.ls	.LBB12_394
// %bb.372:
	cmp	x28, #1                 // =1
	csinc	x12, x28, xzr, hi
	add	x10, x22, #16           // =16
	orr	w11, wzr, #0x1
	add	x13, x22, x12, lsl #3
	and	x14, x12, #0xfffffffffffffffc
	orr	w15, wzr, #0x18
	b	.LBB12_384
.LBB12_373:                             //   in Loop: Header=BB12_384 Depth=1
	add	x17, x16, #16           // =16
	mov	x18, x10
	mov	x0, x14
.LBB12_374:                             //   Parent Loop BB12_384 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldp	q1, q0, [x17, #-16]
	ldp	q3, q2, [x18, #-16]
	cmhi	v3.2d, v3.2d, v1.2d
	xtn	v3.2s, v3.2d
	fmov	w1, s3
	tbz	w1, #0, .LBB12_376
// %bb.375:                             //   in Loop: Header=BB12_374 Depth=2
	stur	d1, [x18, #-16]
.LBB12_376:                             //   in Loop: Header=BB12_374 Depth=2
	mov	w1, v3.s[1]
	tbz	w1, #0, .LBB12_378
// %bb.377:                             //   in Loop: Header=BB12_374 Depth=2
	sub	x1, x18, #8             // =8
	st1	{ v1.d }[1], [x1]
.LBB12_378:                             //   in Loop: Header=BB12_374 Depth=2
	cmhi	v1.2d, v2.2d, v0.2d
	xtn	v1.2s, v1.2d
	fmov	w1, s1
	tbz	w1, #0, .LBB12_380
// %bb.379:                             //   in Loop: Header=BB12_374 Depth=2
	str	d0, [x18]
.LBB12_380:                             //   in Loop: Header=BB12_374 Depth=2
	mov	w1, v1.s[1]
	tbz	w1, #0, .LBB12_382
// %bb.381:                             //   in Loop: Header=BB12_374 Depth=2
	add	x1, x18, #8             // =8
	st1	{ v0.d }[1], [x1]
.LBB12_382:                             //   in Loop: Header=BB12_374 Depth=2
	subs	x0, x0, #4              // =4
	add	x18, x18, #32           // =32
	add	x17, x17, #32           // =32
	b.ne	.LBB12_374
// %bb.383:                             //   in Loop: Header=BB12_384 Depth=1
	cmp	x12, x14
	mov	x17, x14
	b.ne	.LBB12_389
	b	.LBB12_392
.LBB12_384:                             // =>This Loop Header: Depth=1
                                        //     Child Loop BB12_374 Depth 2
                                        //     Child Loop BB12_389 Depth 2
	tbnz	w8, #0, .LBB12_392
// %bb.385:                             //   in Loop: Header=BB12_384 Depth=1
	mul	x16, x11, x15
	ldr	x16, [x24, x16]
	cmp	x12, #3                 // =3
	b.ls	.LBB12_388
// %bb.386:                             //   in Loop: Header=BB12_384 Depth=1
	add	x17, x16, x12, lsl #3
	cmp	x26, x17
	b.hs	.LBB12_373
// %bb.387:                             //   in Loop: Header=BB12_384 Depth=1
	cmp	x16, x13
	b.hs	.LBB12_373
.LBB12_388:                             //   in Loop: Header=BB12_384 Depth=1
	mov	x17, xzr
.LBB12_389:                             //   Parent Loop BB12_384 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x0, x17, #3
	ldr	x18, [x16, x0]
	ldr	x0, [x22, x0]
	cmp	x18, x0
	b.hs	.LBB12_391
// %bb.390:                             //   in Loop: Header=BB12_389 Depth=2
	str	x18, [x22, x17, lsl #3]
.LBB12_391:                             //   in Loop: Header=BB12_389 Depth=2
	add	x17, x17, #1            // =1
	cmp	x17, x28
	b.lo	.LBB12_389
.LBB12_392:                             //   in Loop: Header=BB12_384 Depth=1
	add	x11, x11, #1            // =1
	cmp	x11, x9
	b.ne	.LBB12_384
// %bb.393:
	mov	x27, x24
.LBB12_394:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x27]
	cbz	x0, .LBB12_396
// %bb.395:                             //   in Loop: Header=BB12_394 Depth=1
	bl	_ZdlPv
.LBB12_396:                             //   in Loop: Header=BB12_394 Depth=1
	add	x27, x27, #24           // =24
	cmp	x25, x27
	b.ne	.LBB12_394
// %bb.397:
	cbz	x24, .LBB12_399
.LBB12_398:
	mov	x0, x23
	bl	_ZdlPv
.LBB12_399:
	ldp	x0, x1, [x29, #-184]
	mov	x9, #-6148914691236517206
	movk	x9, #43691
	stp	xzr, xzr, [x29, #-232]
	subs	x8, x1, x0
	asr	x10, x8, #3
	mul	x25, x10, x9
	stur	xzr, [x29, #-216]
	b.eq	.LBB12_403
// %bb.400:
	mov	x9, #-6148914691236517206
	movk	x9, #2730, lsl #48
	cmp	x25, x9
	b.hi	.LBB12_450
// %bb.401:
.Ltmp276:
	mov	x0, x8
	bl	_Znwm
.Ltmp277:
// %bb.402:
	mov	x23, x0
	ldp	x0, x1, [x29, #-184]
	mov	x24, x23
	b	.LBB12_404
.LBB12_403:
	mov	x23, xzr
	mov	x24, xzr
.LBB12_404:
	orr	w8, wzr, #0x18
	madd	x8, x25, x8, x24
	stp	x23, x23, [x29, #-232]
	stur	x8, [x29, #-216]
.Ltmp281:
	mov	x2, x24
	bl	_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_
.Ltmp282:
// %bb.405:
	mov	x25, x0
	stur	x0, [x29, #-224]
.Ltmp284:
	sub	x8, x29, #208           // =208
	sub	x0, x29, #232           // =232
	bl	_Z16compute_averagesSt6vectorIS_IySaIyEESaIS1_EE
.Ltmp285:
// %bb.406:
	cmp	x24, x25
	b.eq	.LBB12_411
// %bb.407:
	mov	x26, x24
.LBB12_408:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x26]
	cbz	x0, .LBB12_410
// %bb.409:                             //   in Loop: Header=BB12_408 Depth=1
	bl	_ZdlPv
.LBB12_410:                             //   in Loop: Header=BB12_408 Depth=1
	add	x26, x26, #24           // =24
	cmp	x25, x26
	b.ne	.LBB12_408
.LBB12_411:
	cbz	x24, .LBB12_413
// %bb.412:
	mov	x0, x23
	bl	_ZdlPv
.LBB12_413:
	ldp	d1, d0, [x22]
	ldur	s2, [x29, #-116]
	adrp	x0, .L.str.6
	add	x0, x0, :lo12:.L.str.6
	ucvtf	d4, d0
	ucvtf	d3, d1
	ucvtf	d2, d2
	fdiv	d0, d4, d3
	fdiv	d1, d3, d2
	fdiv	d2, d4, d2
	fdiv	d3, d3, d8
	fdiv	d4, d4, d8
	bl	printf
	ldp	d0, d1, [x22]
	adrp	x0, .L.str.7
	add	x0, x0, :lo12:.L.str.7
	ucvtf	d0, d0
	ucvtf	d1, d1
	fdiv	d0, d0, d10
	fdiv	d1, d1, d10
	bl	printf
	ldp	x1, x2, [x22]
	ldp	x3, x4, [x22, #16]
	ldr	x5, [x22, #32]
	adrp	x0, .L.str.8
	add	x0, x0, :lo12:.L.str.8
	bl	printf
	ldur	x23, [x29, #-208]
	adrp	x0, .L.str.9
	add	x0, x0, :lo12:.L.str.9
	ldp	d0, d1, [x23]
	ldp	d2, d3, [x23, #16]
	ldr	d4, [x23, #32]
	bl	printf
	mov	w0, #10
	bl	putchar
	cbz	x20, .LBB12_415
// %bb.414:
	mov	x0, x20
	bl	_ZdaPv
.LBB12_415:
	mov	x0, x21
	bl	_ZdaPv
	cbz	x23, .LBB12_417
// %bb.416:
	mov	x0, x23
	bl	_ZdlPv
.LBB12_417:
	cbz	x22, .LBB12_419
// %bb.418:
	mov	x0, x22
	bl	_ZdlPv
.LBB12_419:
	ldp	x20, x21, [x29, #-184]
	cmp	x20, x21
	b.eq	.LBB12_424
.LBB12_420:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x20]
	cbz	x0, .LBB12_422
// %bb.421:                             //   in Loop: Header=BB12_420 Depth=1
	bl	_ZdlPv
.LBB12_422:                             //   in Loop: Header=BB12_420 Depth=1
	add	x20, x20, #24           // =24
	cmp	x21, x20
	b.ne	.LBB12_420
// %bb.423:
	ldur	x20, [x29, #-184]
.LBB12_424:
	cbz	x20, .LBB12_426
// %bb.425:
	mov	x0, x20
	bl	_ZdlPv
.LBB12_426:
	ldur	x0, [x29, #-160]
	cbz	x0, .LBB12_428
// %bb.427:
	bl	_ZdlPv
.LBB12_428:
	ldr	w0, [sp, #264]
.Ltmp290:
	bl	close
.Ltmp291:
// %bb.429:
	ldr	x0, [sp, #416]
	cbz	x0, .LBB12_431
// %bb.430:
	bl	_ZdlPv
.LBB12_431:
	ldr	x0, [sp, #392]
	cbz	x0, .LBB12_433
// %bb.432:
	bl	_ZdlPv
.LBB12_433:
	mov	x0, x19
	bl	_ZdlPv
	mov	w0, #10
	bl	putchar
	mov	w0, wzr
	add	sp, sp, #624            // =624
	ldp	x29, x30, [sp, #128]    // 16-byte Folded Reload
	ldp	x20, x19, [sp, #112]    // 16-byte Folded Reload
	ldp	x22, x21, [sp, #96]     // 16-byte Folded Reload
	ldp	x24, x23, [sp, #80]     // 16-byte Folded Reload
	ldp	x26, x25, [sp, #64]     // 16-byte Folded Reload
	ldp	x28, x27, [sp, #48]     // 16-byte Folded Reload
	ldp	d9, d8, [sp, #32]       // 16-byte Folded Reload
	ldp	d11, d10, [sp, #16]     // 16-byte Folded Reload
	ldr	d12, [sp], #144         // 8-byte Folded Reload
	ret
.LBB12_434:
.Ltmp131:
	bl	_ZSt17__throw_bad_allocv
.Ltmp132:
// %bb.435:
.LBB12_436:
.Ltmp194:
	bl	_ZSt17__throw_bad_allocv
.Ltmp195:
// %bb.437:
.LBB12_438:
.Ltmp257:
	bl	_ZSt17__throw_bad_allocv
.Ltmp258:
// %bb.439:
.LBB12_440:
.Ltmp139:
	bl	_ZSt17__throw_bad_allocv
.Ltmp140:
// %bb.441:
.LBB12_442:
.Ltmp152:
	bl	_ZSt17__throw_bad_allocv
.Ltmp153:
// %bb.443:
.LBB12_444:
.Ltmp202:
	bl	_ZSt17__throw_bad_allocv
.Ltmp203:
// %bb.445:
.LBB12_446:
.Ltmp215:
	bl	_ZSt17__throw_bad_allocv
.Ltmp216:
// %bb.447:
.LBB12_448:
.Ltmp265:
	bl	_ZSt17__throw_bad_allocv
.Ltmp266:
// %bb.449:
.LBB12_450:
.Ltmp278:
	bl	_ZSt17__throw_bad_allocv
.Ltmp279:
// %bb.451:
.LBB12_452:
.Ltmp147:
	bl	_ZSt17__throw_bad_allocv
.Ltmp148:
// %bb.453:
.LBB12_454:
.Ltmp210:
	bl	_ZSt17__throw_bad_allocv
.Ltmp211:
// %bb.455:
.LBB12_456:
.Ltmp273:
	bl	_ZSt17__throw_bad_allocv
.Ltmp274:
// %bb.457:
.LBB12_458:
.Ltmp259:
	b	.LBB12_552
.LBB12_459:
.Ltmp196:
	b	.LBB12_565
.LBB12_460:
.Ltmp133:
	b	.LBB12_578
.LBB12_461:
.Ltmp275:
	mov	x20, x0
	mov	x21, x24
.LBB12_462:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x21]
	cbz	x0, .LBB12_464
// %bb.463:                             //   in Loop: Header=BB12_462 Depth=1
	bl	_ZdlPv
.LBB12_464:                             //   in Loop: Header=BB12_462 Depth=1
	add	x21, x21, #24           // =24
	cmp	x25, x21
	b.ne	.LBB12_462
// %bb.465:
	cbnz	x24, .LBB12_494
	b	.LBB12_553
.LBB12_466:
.Ltmp212:
	mov	x20, x0
	mov	x21, x24
.LBB12_467:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x21]
	cbz	x0, .LBB12_469
// %bb.468:                             //   in Loop: Header=BB12_467 Depth=1
	bl	_ZdlPv
.LBB12_469:                             //   in Loop: Header=BB12_467 Depth=1
	add	x21, x21, #24           // =24
	cmp	x25, x21
	b.ne	.LBB12_467
// %bb.470:
	cbnz	x24, .LBB12_515
	b	.LBB12_566
.LBB12_471:
.Ltmp149:
	mov	x20, x0
	mov	x21, x24
.LBB12_472:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x21]
	cbz	x0, .LBB12_474
// %bb.473:                             //   in Loop: Header=BB12_472 Depth=1
	bl	_ZdlPv
.LBB12_474:                             //   in Loop: Header=BB12_472 Depth=1
	add	x21, x21, #24           // =24
	cmp	x25, x21
	b.ne	.LBB12_472
// %bb.475:
	cbnz	x24, .LBB12_537
	b	.LBB12_579
.LBB12_476:
.Ltmp280:
	mov	x20, x0
	cbnz	x22, .LBB12_492
	b	.LBB12_553
.LBB12_477:
.Ltmp267:
	b	.LBB12_552
.LBB12_478:
.Ltmp217:
	mov	x20, x0
	cbnz	x22, .LBB12_513
	b	.LBB12_566
.LBB12_479:
.Ltmp204:
	b	.LBB12_565
.LBB12_480:
.Ltmp154:
	mov	x20, x0
	cbnz	x22, .LBB12_535
	b	.LBB12_579
.LBB12_481:
.Ltmp141:
	b	.LBB12_578
.LBB12_482:
.Ltmp292:
	b	.LBB12_525
.LBB12_483:
.Ltmp286:
	cmp	x24, x25
	mov	x20, x0
	b.eq	.LBB12_488
// %bb.484:
	mov	x21, x24
.LBB12_485:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x21]
	cbz	x0, .LBB12_487
// %bb.486:                             //   in Loop: Header=BB12_485 Depth=1
	bl	_ZdlPv
.LBB12_487:                             //   in Loop: Header=BB12_485 Depth=1
	add	x21, x21, #24           // =24
	cmp	x25, x21
	b.ne	.LBB12_485
.LBB12_488:
	cbnz	x24, .LBB12_490
	b	.LBB12_491
.LBB12_489:
.Ltmp283:
	mov	x20, x0
	cbz	x24, .LBB12_491
.LBB12_490:
	mov	x0, x23
	bl	_ZdlPv
.LBB12_491:
	cbz	x22, .LBB12_553
.LBB12_492:
	mov	x0, x22
	b	.LBB12_495
.LBB12_493:
.Ltmp270:
	mov	x20, x0
	cbz	x24, .LBB12_553
.LBB12_494:
	mov	x0, x23
.LBB12_495:
	bl	_ZdlPv
	b	.LBB12_553
.LBB12_496:
.Ltmp250:
	b	.LBB12_552
.LBB12_497:
.Ltmp247:
	b	.LBB12_541
.LBB12_498:
.Ltmp244:
	b	.LBB12_549
.LBB12_499:
.Ltmp241:
	b	.LBB12_545
.LBB12_500:
.Ltmp238:
	b	.LBB12_547
.LBB12_501:
.Ltmp235:
	b	.LBB12_549
.LBB12_502:
.Ltmp232:
	bl	_Unwind_Resume
.LBB12_503:
.Ltmp229:
	b	.LBB12_525
.LBB12_504:
.Ltmp223:
	cmp	x24, x25
	mov	x20, x0
	b.eq	.LBB12_509
// %bb.505:
	mov	x21, x24
.LBB12_506:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x21]
	cbz	x0, .LBB12_508
// %bb.507:                             //   in Loop: Header=BB12_506 Depth=1
	bl	_ZdlPv
.LBB12_508:                             //   in Loop: Header=BB12_506 Depth=1
	add	x21, x21, #24           // =24
	cmp	x25, x21
	b.ne	.LBB12_506
.LBB12_509:
	cbnz	x24, .LBB12_511
	b	.LBB12_512
.LBB12_510:
.Ltmp220:
	mov	x20, x0
	cbz	x24, .LBB12_512
.LBB12_511:
	mov	x0, x23
	bl	_ZdlPv
.LBB12_512:
	cbz	x22, .LBB12_566
.LBB12_513:
	mov	x0, x22
	b	.LBB12_516
.LBB12_514:
.Ltmp207:
	mov	x20, x0
	cbz	x24, .LBB12_566
.LBB12_515:
	mov	x0, x23
.LBB12_516:
	bl	_ZdlPv
	b	.LBB12_566
.LBB12_517:
.Ltmp187:
	b	.LBB12_565
.LBB12_518:
.Ltmp184:
	b	.LBB12_541
.LBB12_519:
.Ltmp181:
	b	.LBB12_549
.LBB12_520:
.Ltmp178:
	b	.LBB12_545
.LBB12_521:
.Ltmp175:
	b	.LBB12_547
.LBB12_522:
.Ltmp172:
	b	.LBB12_549
.LBB12_523:
.Ltmp169:
	bl	_Unwind_Resume
.LBB12_524:
.Ltmp166:
.LBB12_525:
	add	x20, sp, #264           // =264
	mov	x19, x0
	add	x0, x20, #152           // =152
	bl	_ZNSt6vectorImSaImEED2Ev
	add	x0, x20, #128           // =128
	bl	_ZNSt6vectorImSaImEED2Ev
	mov	x0, x19
	bl	__clang_call_terminate
.LBB12_526:
.Ltmp160:
	cmp	x24, x25
	mov	x20, x0
	b.eq	.LBB12_531
// %bb.527:
	mov	x21, x24
.LBB12_528:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x21]
	cbz	x0, .LBB12_530
// %bb.529:                             //   in Loop: Header=BB12_528 Depth=1
	bl	_ZdlPv
.LBB12_530:                             //   in Loop: Header=BB12_528 Depth=1
	add	x21, x21, #24           // =24
	cmp	x25, x21
	b.ne	.LBB12_528
.LBB12_531:
	cbnz	x24, .LBB12_533
	b	.LBB12_534
.LBB12_532:
.Ltmp157:
	mov	x20, x0
	cbz	x24, .LBB12_534
.LBB12_533:
	mov	x0, x23
	bl	_ZdlPv
.LBB12_534:
	cbz	x22, .LBB12_579
.LBB12_535:
	mov	x0, x22
	b	.LBB12_538
.LBB12_536:
.Ltmp144:
	mov	x20, x0
	cbz	x24, .LBB12_579
.LBB12_537:
	mov	x0, x23
.LBB12_538:
	bl	_ZdlPv
	b	.LBB12_579
.LBB12_539:
.Ltmp124:
	b	.LBB12_578
.LBB12_540:
.Ltmp121:
.LBB12_541:
	ldr	x8, [sp, #8]
	mov	x20, x0
	cbz	x8, .LBB12_593
// %bb.542:
	mov	x0, x8
	b	.LBB12_592
.LBB12_543:
.Ltmp118:
	b	.LBB12_549
.LBB12_544:
.Ltmp115:
.LBB12_545:
	mov	x20, x0
	mov	x19, x23
	b	.LBB12_593
.LBB12_546:
.Ltmp112:
.LBB12_547:
	mov	x20, x0
	mov	x19, x24
	b	.LBB12_593
.LBB12_548:
.Ltmp109:
.LBB12_549:
	mov	x20, x0
	b	.LBB12_593
.LBB12_550:
.Ltmp106:
	bl	_Unwind_Resume
.LBB12_551:
.Ltmp262:
.LBB12_552:
	mov	x20, x0
.LBB12_553:
	ldp	x21, x22, [x29, #-184]
	cmp	x21, x22
	b.eq	.LBB12_558
.LBB12_554:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x21]
	cbz	x0, .LBB12_556
// %bb.555:                             //   in Loop: Header=BB12_554 Depth=1
	bl	_ZdlPv
.LBB12_556:                             //   in Loop: Header=BB12_554 Depth=1
	add	x21, x21, #24           // =24
	cmp	x22, x21
	b.ne	.LBB12_554
// %bb.557:
	ldur	x21, [x29, #-184]
.LBB12_558:
	cbz	x21, .LBB12_560
// %bb.559:
	mov	x0, x21
	bl	_ZdlPv
.LBB12_560:
	ldur	x0, [x29, #-160]
	cbz	x0, .LBB12_562
// %bb.561:
	bl	_ZdlPv
.LBB12_562:
	ldr	w0, [sp, #264]
.Ltmp287:
	bl	close
.Ltmp288:
	b	.LBB12_589
.LBB12_563:
.Ltmp289:
	b	.LBB12_525
.LBB12_564:
.Ltmp199:
.LBB12_565:
	mov	x20, x0
.LBB12_566:
	ldp	x21, x22, [x29, #-184]
	cmp	x21, x22
	b.eq	.LBB12_571
.LBB12_567:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x21]
	cbz	x0, .LBB12_569
// %bb.568:                             //   in Loop: Header=BB12_567 Depth=1
	bl	_ZdlPv
.LBB12_569:                             //   in Loop: Header=BB12_567 Depth=1
	add	x21, x21, #24           // =24
	cmp	x22, x21
	b.ne	.LBB12_567
// %bb.570:
	ldur	x21, [x29, #-184]
.LBB12_571:
	cbz	x21, .LBB12_573
// %bb.572:
	mov	x0, x21
	bl	_ZdlPv
.LBB12_573:
	ldur	x0, [x29, #-160]
	cbz	x0, .LBB12_575
// %bb.574:
	bl	_ZdlPv
.LBB12_575:
	ldr	w0, [sp, #264]
.Ltmp224:
	bl	close
.Ltmp225:
	b	.LBB12_589
.LBB12_576:
.Ltmp226:
	b	.LBB12_525
.LBB12_577:
.Ltmp136:
.LBB12_578:
	mov	x20, x0
.LBB12_579:
	ldp	x21, x22, [x29, #-184]
	cmp	x21, x22
	b.eq	.LBB12_584
.LBB12_580:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x21]
	cbz	x0, .LBB12_582
// %bb.581:                             //   in Loop: Header=BB12_580 Depth=1
	bl	_ZdlPv
.LBB12_582:                             //   in Loop: Header=BB12_580 Depth=1
	add	x21, x21, #24           // =24
	cmp	x22, x21
	b.ne	.LBB12_580
// %bb.583:
	ldur	x21, [x29, #-184]
.LBB12_584:
	cbz	x21, .LBB12_586
// %bb.585:
	mov	x0, x21
	bl	_ZdlPv
.LBB12_586:
	ldur	x0, [x29, #-160]
	cbz	x0, .LBB12_588
// %bb.587:
	bl	_ZdlPv
.LBB12_588:
	ldr	w0, [sp, #264]
.Ltmp161:
	bl	close
.Ltmp162:
.LBB12_589:
	ldr	x0, [sp, #416]
	cbz	x0, .LBB12_591
// %bb.590:
	bl	_ZdlPv
.LBB12_591:
	ldr	x0, [sp, #392]
	cbz	x0, .LBB12_593
.LBB12_592:
	bl	_ZdlPv
.LBB12_593:
	mov	x0, x19
	bl	_ZdlPv
	mov	x0, x20
	bl	_Unwind_Resume
.LBB12_594:
.Ltmp163:
	b	.LBB12_525
.Lfunc_end12:
	.size	main, .Lfunc_end12-main
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table12:
.Lexception5:
	.byte	255                     // @LPStart Encoding = omit
	.byte	0                       // @TType Encoding = absptr
	.uleb128 .Lttbase1-.Lttbaseref1
.Lttbaseref1:
	.byte	1                       // Call site Encoding = uleb128
	.uleb128 .Lcst_end5-.Lcst_begin5
.Lcst_begin5:
	.uleb128 .Lfunc_begin5-.Lfunc_begin5 // >> Call Site 1 <<
	.uleb128 .Ltmp104-.Lfunc_begin5 //   Call between .Lfunc_begin5 and .Ltmp104
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp104-.Lfunc_begin5 // >> Call Site 2 <<
	.uleb128 .Ltmp105-.Ltmp104      //   Call between .Ltmp104 and .Ltmp105
	.uleb128 .Ltmp106-.Lfunc_begin5 //     jumps to .Ltmp106
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp107-.Lfunc_begin5 // >> Call Site 3 <<
	.uleb128 .Ltmp108-.Ltmp107      //   Call between .Ltmp107 and .Ltmp108
	.uleb128 .Ltmp109-.Lfunc_begin5 //     jumps to .Ltmp109
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp110-.Lfunc_begin5 // >> Call Site 4 <<
	.uleb128 .Ltmp111-.Ltmp110      //   Call between .Ltmp110 and .Ltmp111
	.uleb128 .Ltmp112-.Lfunc_begin5 //     jumps to .Ltmp112
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp113-.Lfunc_begin5 // >> Call Site 5 <<
	.uleb128 .Ltmp114-.Ltmp113      //   Call between .Ltmp113 and .Ltmp114
	.uleb128 .Ltmp115-.Lfunc_begin5 //     jumps to .Ltmp115
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp116-.Lfunc_begin5 // >> Call Site 6 <<
	.uleb128 .Ltmp117-.Ltmp116      //   Call between .Ltmp116 and .Ltmp117
	.uleb128 .Ltmp118-.Lfunc_begin5 //     jumps to .Ltmp118
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp119-.Lfunc_begin5 // >> Call Site 7 <<
	.uleb128 .Ltmp120-.Ltmp119      //   Call between .Ltmp119 and .Ltmp120
	.uleb128 .Ltmp121-.Lfunc_begin5 //     jumps to .Ltmp121
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp122-.Lfunc_begin5 // >> Call Site 8 <<
	.uleb128 .Ltmp123-.Ltmp122      //   Call between .Ltmp122 and .Ltmp123
	.uleb128 .Ltmp124-.Lfunc_begin5 //     jumps to .Ltmp124
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp123-.Lfunc_begin5 // >> Call Site 9 <<
	.uleb128 .Ltmp125-.Ltmp123      //   Call between .Ltmp123 and .Ltmp125
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp125-.Lfunc_begin5 // >> Call Site 10 <<
	.uleb128 .Ltmp135-.Ltmp125      //   Call between .Ltmp125 and .Ltmp135
	.uleb128 .Ltmp136-.Lfunc_begin5 //     jumps to .Ltmp136
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp135-.Lfunc_begin5 // >> Call Site 11 <<
	.uleb128 .Ltmp137-.Ltmp135      //   Call between .Ltmp135 and .Ltmp137
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp137-.Lfunc_begin5 // >> Call Site 12 <<
	.uleb128 .Ltmp138-.Ltmp137      //   Call between .Ltmp137 and .Ltmp138
	.uleb128 .Ltmp141-.Lfunc_begin5 //     jumps to .Ltmp141
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp142-.Lfunc_begin5 // >> Call Site 13 <<
	.uleb128 .Ltmp143-.Ltmp142      //   Call between .Ltmp142 and .Ltmp143
	.uleb128 .Ltmp144-.Lfunc_begin5 //     jumps to .Ltmp144
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp145-.Lfunc_begin5 // >> Call Site 14 <<
	.uleb128 .Ltmp146-.Ltmp145      //   Call between .Ltmp145 and .Ltmp146
	.uleb128 .Ltmp149-.Lfunc_begin5 //     jumps to .Ltmp149
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp146-.Lfunc_begin5 // >> Call Site 15 <<
	.uleb128 .Ltmp150-.Ltmp146      //   Call between .Ltmp146 and .Ltmp150
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp150-.Lfunc_begin5 // >> Call Site 16 <<
	.uleb128 .Ltmp151-.Ltmp150      //   Call between .Ltmp150 and .Ltmp151
	.uleb128 .Ltmp154-.Lfunc_begin5 //     jumps to .Ltmp154
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp155-.Lfunc_begin5 // >> Call Site 17 <<
	.uleb128 .Ltmp156-.Ltmp155      //   Call between .Ltmp155 and .Ltmp156
	.uleb128 .Ltmp157-.Lfunc_begin5 //     jumps to .Ltmp157
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp158-.Lfunc_begin5 // >> Call Site 18 <<
	.uleb128 .Ltmp159-.Ltmp158      //   Call between .Ltmp158 and .Ltmp159
	.uleb128 .Ltmp160-.Lfunc_begin5 //     jumps to .Ltmp160
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp164-.Lfunc_begin5 // >> Call Site 19 <<
	.uleb128 .Ltmp165-.Ltmp164      //   Call between .Ltmp164 and .Ltmp165
	.uleb128 .Ltmp166-.Lfunc_begin5 //     jumps to .Ltmp166
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp165-.Lfunc_begin5 // >> Call Site 20 <<
	.uleb128 .Ltmp167-.Ltmp165      //   Call between .Ltmp165 and .Ltmp167
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp167-.Lfunc_begin5 // >> Call Site 21 <<
	.uleb128 .Ltmp168-.Ltmp167      //   Call between .Ltmp167 and .Ltmp168
	.uleb128 .Ltmp169-.Lfunc_begin5 //     jumps to .Ltmp169
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp170-.Lfunc_begin5 // >> Call Site 22 <<
	.uleb128 .Ltmp171-.Ltmp170      //   Call between .Ltmp170 and .Ltmp171
	.uleb128 .Ltmp172-.Lfunc_begin5 //     jumps to .Ltmp172
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp173-.Lfunc_begin5 // >> Call Site 23 <<
	.uleb128 .Ltmp174-.Ltmp173      //   Call between .Ltmp173 and .Ltmp174
	.uleb128 .Ltmp175-.Lfunc_begin5 //     jumps to .Ltmp175
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp176-.Lfunc_begin5 // >> Call Site 24 <<
	.uleb128 .Ltmp177-.Ltmp176      //   Call between .Ltmp176 and .Ltmp177
	.uleb128 .Ltmp178-.Lfunc_begin5 //     jumps to .Ltmp178
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp179-.Lfunc_begin5 // >> Call Site 25 <<
	.uleb128 .Ltmp180-.Ltmp179      //   Call between .Ltmp179 and .Ltmp180
	.uleb128 .Ltmp181-.Lfunc_begin5 //     jumps to .Ltmp181
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp182-.Lfunc_begin5 // >> Call Site 26 <<
	.uleb128 .Ltmp183-.Ltmp182      //   Call between .Ltmp182 and .Ltmp183
	.uleb128 .Ltmp184-.Lfunc_begin5 //     jumps to .Ltmp184
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp185-.Lfunc_begin5 // >> Call Site 27 <<
	.uleb128 .Ltmp186-.Ltmp185      //   Call between .Ltmp185 and .Ltmp186
	.uleb128 .Ltmp187-.Lfunc_begin5 //     jumps to .Ltmp187
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp186-.Lfunc_begin5 // >> Call Site 28 <<
	.uleb128 .Ltmp188-.Ltmp186      //   Call between .Ltmp186 and .Ltmp188
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp188-.Lfunc_begin5 // >> Call Site 29 <<
	.uleb128 .Ltmp198-.Ltmp188      //   Call between .Ltmp188 and .Ltmp198
	.uleb128 .Ltmp199-.Lfunc_begin5 //     jumps to .Ltmp199
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp198-.Lfunc_begin5 // >> Call Site 30 <<
	.uleb128 .Ltmp200-.Ltmp198      //   Call between .Ltmp198 and .Ltmp200
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp200-.Lfunc_begin5 // >> Call Site 31 <<
	.uleb128 .Ltmp201-.Ltmp200      //   Call between .Ltmp200 and .Ltmp201
	.uleb128 .Ltmp204-.Lfunc_begin5 //     jumps to .Ltmp204
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp205-.Lfunc_begin5 // >> Call Site 32 <<
	.uleb128 .Ltmp206-.Ltmp205      //   Call between .Ltmp205 and .Ltmp206
	.uleb128 .Ltmp207-.Lfunc_begin5 //     jumps to .Ltmp207
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp208-.Lfunc_begin5 // >> Call Site 33 <<
	.uleb128 .Ltmp209-.Ltmp208      //   Call between .Ltmp208 and .Ltmp209
	.uleb128 .Ltmp212-.Lfunc_begin5 //     jumps to .Ltmp212
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp209-.Lfunc_begin5 // >> Call Site 34 <<
	.uleb128 .Ltmp213-.Ltmp209      //   Call between .Ltmp209 and .Ltmp213
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp213-.Lfunc_begin5 // >> Call Site 35 <<
	.uleb128 .Ltmp214-.Ltmp213      //   Call between .Ltmp213 and .Ltmp214
	.uleb128 .Ltmp217-.Lfunc_begin5 //     jumps to .Ltmp217
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp218-.Lfunc_begin5 // >> Call Site 36 <<
	.uleb128 .Ltmp219-.Ltmp218      //   Call between .Ltmp218 and .Ltmp219
	.uleb128 .Ltmp220-.Lfunc_begin5 //     jumps to .Ltmp220
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp221-.Lfunc_begin5 // >> Call Site 37 <<
	.uleb128 .Ltmp222-.Ltmp221      //   Call between .Ltmp221 and .Ltmp222
	.uleb128 .Ltmp223-.Lfunc_begin5 //     jumps to .Ltmp223
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp227-.Lfunc_begin5 // >> Call Site 38 <<
	.uleb128 .Ltmp228-.Ltmp227      //   Call between .Ltmp227 and .Ltmp228
	.uleb128 .Ltmp229-.Lfunc_begin5 //     jumps to .Ltmp229
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp228-.Lfunc_begin5 // >> Call Site 39 <<
	.uleb128 .Ltmp230-.Ltmp228      //   Call between .Ltmp228 and .Ltmp230
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp230-.Lfunc_begin5 // >> Call Site 40 <<
	.uleb128 .Ltmp231-.Ltmp230      //   Call between .Ltmp230 and .Ltmp231
	.uleb128 .Ltmp232-.Lfunc_begin5 //     jumps to .Ltmp232
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp233-.Lfunc_begin5 // >> Call Site 41 <<
	.uleb128 .Ltmp234-.Ltmp233      //   Call between .Ltmp233 and .Ltmp234
	.uleb128 .Ltmp235-.Lfunc_begin5 //     jumps to .Ltmp235
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp236-.Lfunc_begin5 // >> Call Site 42 <<
	.uleb128 .Ltmp237-.Ltmp236      //   Call between .Ltmp236 and .Ltmp237
	.uleb128 .Ltmp238-.Lfunc_begin5 //     jumps to .Ltmp238
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp239-.Lfunc_begin5 // >> Call Site 43 <<
	.uleb128 .Ltmp240-.Ltmp239      //   Call between .Ltmp239 and .Ltmp240
	.uleb128 .Ltmp241-.Lfunc_begin5 //     jumps to .Ltmp241
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp242-.Lfunc_begin5 // >> Call Site 44 <<
	.uleb128 .Ltmp243-.Ltmp242      //   Call between .Ltmp242 and .Ltmp243
	.uleb128 .Ltmp244-.Lfunc_begin5 //     jumps to .Ltmp244
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp245-.Lfunc_begin5 // >> Call Site 45 <<
	.uleb128 .Ltmp246-.Ltmp245      //   Call between .Ltmp245 and .Ltmp246
	.uleb128 .Ltmp247-.Lfunc_begin5 //     jumps to .Ltmp247
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp248-.Lfunc_begin5 // >> Call Site 46 <<
	.uleb128 .Ltmp249-.Ltmp248      //   Call between .Ltmp248 and .Ltmp249
	.uleb128 .Ltmp250-.Lfunc_begin5 //     jumps to .Ltmp250
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp249-.Lfunc_begin5 // >> Call Site 47 <<
	.uleb128 .Ltmp251-.Ltmp249      //   Call between .Ltmp249 and .Ltmp251
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp251-.Lfunc_begin5 // >> Call Site 48 <<
	.uleb128 .Ltmp261-.Ltmp251      //   Call between .Ltmp251 and .Ltmp261
	.uleb128 .Ltmp262-.Lfunc_begin5 //     jumps to .Ltmp262
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp261-.Lfunc_begin5 // >> Call Site 49 <<
	.uleb128 .Ltmp263-.Ltmp261      //   Call between .Ltmp261 and .Ltmp263
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp263-.Lfunc_begin5 // >> Call Site 50 <<
	.uleb128 .Ltmp264-.Ltmp263      //   Call between .Ltmp263 and .Ltmp264
	.uleb128 .Ltmp267-.Lfunc_begin5 //     jumps to .Ltmp267
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp268-.Lfunc_begin5 // >> Call Site 51 <<
	.uleb128 .Ltmp269-.Ltmp268      //   Call between .Ltmp268 and .Ltmp269
	.uleb128 .Ltmp270-.Lfunc_begin5 //     jumps to .Ltmp270
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp271-.Lfunc_begin5 // >> Call Site 52 <<
	.uleb128 .Ltmp272-.Ltmp271      //   Call between .Ltmp271 and .Ltmp272
	.uleb128 .Ltmp275-.Lfunc_begin5 //     jumps to .Ltmp275
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp272-.Lfunc_begin5 // >> Call Site 53 <<
	.uleb128 .Ltmp276-.Ltmp272      //   Call between .Ltmp272 and .Ltmp276
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp276-.Lfunc_begin5 // >> Call Site 54 <<
	.uleb128 .Ltmp277-.Ltmp276      //   Call between .Ltmp276 and .Ltmp277
	.uleb128 .Ltmp280-.Lfunc_begin5 //     jumps to .Ltmp280
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp281-.Lfunc_begin5 // >> Call Site 55 <<
	.uleb128 .Ltmp282-.Ltmp281      //   Call between .Ltmp281 and .Ltmp282
	.uleb128 .Ltmp283-.Lfunc_begin5 //     jumps to .Ltmp283
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp284-.Lfunc_begin5 // >> Call Site 56 <<
	.uleb128 .Ltmp285-.Ltmp284      //   Call between .Ltmp284 and .Ltmp285
	.uleb128 .Ltmp286-.Lfunc_begin5 //     jumps to .Ltmp286
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp290-.Lfunc_begin5 // >> Call Site 57 <<
	.uleb128 .Ltmp291-.Ltmp290      //   Call between .Ltmp290 and .Ltmp291
	.uleb128 .Ltmp292-.Lfunc_begin5 //     jumps to .Ltmp292
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp131-.Lfunc_begin5 // >> Call Site 58 <<
	.uleb128 .Ltmp132-.Ltmp131      //   Call between .Ltmp131 and .Ltmp132
	.uleb128 .Ltmp133-.Lfunc_begin5 //     jumps to .Ltmp133
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp194-.Lfunc_begin5 // >> Call Site 59 <<
	.uleb128 .Ltmp195-.Ltmp194      //   Call between .Ltmp194 and .Ltmp195
	.uleb128 .Ltmp196-.Lfunc_begin5 //     jumps to .Ltmp196
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp257-.Lfunc_begin5 // >> Call Site 60 <<
	.uleb128 .Ltmp258-.Ltmp257      //   Call between .Ltmp257 and .Ltmp258
	.uleb128 .Ltmp259-.Lfunc_begin5 //     jumps to .Ltmp259
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp139-.Lfunc_begin5 // >> Call Site 61 <<
	.uleb128 .Ltmp140-.Ltmp139      //   Call between .Ltmp139 and .Ltmp140
	.uleb128 .Ltmp141-.Lfunc_begin5 //     jumps to .Ltmp141
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp152-.Lfunc_begin5 // >> Call Site 62 <<
	.uleb128 .Ltmp153-.Ltmp152      //   Call between .Ltmp152 and .Ltmp153
	.uleb128 .Ltmp154-.Lfunc_begin5 //     jumps to .Ltmp154
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp202-.Lfunc_begin5 // >> Call Site 63 <<
	.uleb128 .Ltmp203-.Ltmp202      //   Call between .Ltmp202 and .Ltmp203
	.uleb128 .Ltmp204-.Lfunc_begin5 //     jumps to .Ltmp204
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp215-.Lfunc_begin5 // >> Call Site 64 <<
	.uleb128 .Ltmp216-.Ltmp215      //   Call between .Ltmp215 and .Ltmp216
	.uleb128 .Ltmp217-.Lfunc_begin5 //     jumps to .Ltmp217
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp265-.Lfunc_begin5 // >> Call Site 65 <<
	.uleb128 .Ltmp266-.Ltmp265      //   Call between .Ltmp265 and .Ltmp266
	.uleb128 .Ltmp267-.Lfunc_begin5 //     jumps to .Ltmp267
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp278-.Lfunc_begin5 // >> Call Site 66 <<
	.uleb128 .Ltmp279-.Ltmp278      //   Call between .Ltmp278 and .Ltmp279
	.uleb128 .Ltmp280-.Lfunc_begin5 //     jumps to .Ltmp280
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp147-.Lfunc_begin5 // >> Call Site 67 <<
	.uleb128 .Ltmp148-.Ltmp147      //   Call between .Ltmp147 and .Ltmp148
	.uleb128 .Ltmp149-.Lfunc_begin5 //     jumps to .Ltmp149
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp210-.Lfunc_begin5 // >> Call Site 68 <<
	.uleb128 .Ltmp211-.Ltmp210      //   Call between .Ltmp210 and .Ltmp211
	.uleb128 .Ltmp212-.Lfunc_begin5 //     jumps to .Ltmp212
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp273-.Lfunc_begin5 // >> Call Site 69 <<
	.uleb128 .Ltmp274-.Ltmp273      //   Call between .Ltmp273 and .Ltmp274
	.uleb128 .Ltmp275-.Lfunc_begin5 //     jumps to .Ltmp275
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp274-.Lfunc_begin5 // >> Call Site 70 <<
	.uleb128 .Ltmp287-.Ltmp274      //   Call between .Ltmp274 and .Ltmp287
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp287-.Lfunc_begin5 // >> Call Site 71 <<
	.uleb128 .Ltmp288-.Ltmp287      //   Call between .Ltmp287 and .Ltmp288
	.uleb128 .Ltmp289-.Lfunc_begin5 //     jumps to .Ltmp289
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp224-.Lfunc_begin5 // >> Call Site 72 <<
	.uleb128 .Ltmp225-.Ltmp224      //   Call between .Ltmp224 and .Ltmp225
	.uleb128 .Ltmp226-.Lfunc_begin5 //     jumps to .Ltmp226
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp161-.Lfunc_begin5 // >> Call Site 73 <<
	.uleb128 .Ltmp162-.Ltmp161      //   Call between .Ltmp161 and .Ltmp162
	.uleb128 .Ltmp163-.Lfunc_begin5 //     jumps to .Ltmp163
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp162-.Lfunc_begin5 // >> Call Site 74 <<
	.uleb128 .Lfunc_end12-.Ltmp162  //   Call between .Ltmp162 and .Lfunc_end12
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
.Lcst_end5:
	.byte	1                       // >> Action Record 1 <<
                                        //   Catch TypeInfo 1
	.byte	0                       //   No further actions
	.p2align	2
                                        // >> Catch TypeInfos <<
	.xword	0                       // TypeInfo 1
.Lttbase1:
	.p2align	2
                                        // -- End function
	.section	.text._Z4unitIXadL_Z13basic_decoderPjRjjmEEEbv,"axG",@progbits,_Z4unitIXadL_Z13basic_decoderPjRjjmEEEbv,comdat
	.weak	_Z4unitIXadL_Z13basic_decoderPjRjjmEEEbv // -- Begin function _Z4unitIXadL_Z13basic_decoderPjRjjmEEEbv
	.p2align	2
	.type	_Z4unitIXadL_Z13basic_decoderPjRjjmEEEbv,@function
_Z4unitIXadL_Z13basic_decoderPjRjjmEEEbv: // @_Z4unitIXadL_Z13basic_decoderPjRjjmEEEbv
	.cfi_startproc
// %bb.0:
	stp	x28, x23, [sp, #-64]!   // 16-byte Folded Spill
	stp	x22, x21, [sp, #16]     // 16-byte Folded Spill
	stp	x20, x19, [sp, #32]     // 16-byte Folded Spill
	stp	x29, x30, [sp, #48]     // 16-byte Folded Spill
	add	x29, sp, #48            // =48
	sub	sp, sp, #528            // =528
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -56
	.cfi_offset w28, -64
	mov	x20, xzr
	orr	w21, wzr, #0x1
	add	x22, sp, #16            // =16
	add	x23, sp, #272           // =272
.LBB13_1:                               // =>This Loop Header: Depth=1
                                        //     Child Loop BB13_2 Depth 2
                                        //     Child Loop BB13_6 Depth 2
	lsl	x19, x21, x20
	add	x0, sp, #16             // =16
	add	x1, sp, #12             // =12
	mov	w2, wzr
	mov	x3, x19
	str	wzr, [sp, #12]
	bl	_ZL14faster_decoderPjRjjm
	mov	w8, wzr
.LBB13_2:                               //   Parent Loop BB13_1 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	rbit	x9, x19
	sub	x10, x19, #1            // =1
	clz	x9, x9
	ands	x19, x10, x19
	str	w9, [x23, w8, uxtw #2]
	add	w8, w8, #1              // =1
	b.ne	.LBB13_2
// %bb.3:                               //   in Loop: Header=BB13_1 Depth=1
	ldr	w9, [sp, #12]
	cmp	w9, w8
	b.ne	.LBB13_20
// %bb.4:                               //   in Loop: Header=BB13_1 Depth=1
	cbz	w8, .LBB13_8
// %bb.5:                               //   in Loop: Header=BB13_1 Depth=1
	mov	x9, xzr
	mov	w8, w8
.LBB13_6:                               //   Parent Loop BB13_1 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x10, x9, #2
	ldr	w11, [x22, x10]
	ldr	w10, [x23, x10]
	cmp	w11, w10
	b.ne	.LBB13_20
// %bb.7:                               //   in Loop: Header=BB13_6 Depth=2
	add	x9, x9, #1              // =1
	cmp	x9, x8
	b.lo	.LBB13_6
.LBB13_8:                               //   in Loop: Header=BB13_1 Depth=1
	add	x20, x20, #1            // =1
	cmp	x20, #64                // =64
	b.lo	.LBB13_1
// %bb.9:
	mov	w22, #16960
	mov	x20, xzr
	add	x21, sp, #16            // =16
	movk	w22, #15, lsl #16
	add	x23, sp, #272           // =272
.LBB13_10:                              // =>This Loop Header: Depth=1
                                        //     Child Loop BB13_12 Depth 2
                                        //     Child Loop BB13_16 Depth 2
	bl	rand
	mov	w19, w0
	bl	rand
                                        // kill: def $w0 killed $w0 def $x0
	sxtw	x8, w0
	orr	x19, x8, x19, lsl #32
	add	x0, sp, #16             // =16
	add	x1, sp, #12             // =12
	mov	w2, wzr
	mov	x3, x19
	str	wzr, [sp, #12]
	bl	_ZL14faster_decoderPjRjjm
	fmov	d0, x19
	cnt	v0.8b, v0.8b
	uaddlv	h0, v0.8b
	fmov	w8, s0
	cbz	w8, .LBB13_13
// %bb.11:                              //   in Loop: Header=BB13_10 Depth=1
	add	w9, w8, #1              // =1
	add	x10, sp, #272           // =272
.LBB13_12:                              //   Parent Loop BB13_10 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	rbit	x11, x19
	sub	x12, x19, #1            // =1
	sub	w9, w9, #1              // =1
	clz	x11, x11
	cmp	w9, #1                  // =1
	str	w11, [x10], #4
	and	x19, x12, x19
	b.gt	.LBB13_12
.LBB13_13:                              //   in Loop: Header=BB13_10 Depth=1
	ldr	w9, [sp, #12]
	cmp	w9, w8
	b.ne	.LBB13_20
// %bb.14:                              //   in Loop: Header=BB13_10 Depth=1
	cbz	w8, .LBB13_18
// %bb.15:                              //   in Loop: Header=BB13_10 Depth=1
	mov	x9, xzr
.LBB13_16:                              //   Parent Loop BB13_10 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x10, x9, #2
	ldr	w11, [x21, x10]
	ldr	w10, [x23, x10]
	cmp	w11, w10
	b.ne	.LBB13_20
// %bb.17:                              //   in Loop: Header=BB13_16 Depth=2
	add	x9, x9, #1              // =1
	cmp	x9, x8
	b.lo	.LBB13_16
.LBB13_18:                              //   in Loop: Header=BB13_10 Depth=1
	add	x20, x20, #1            // =1
	cmp	x20, x22
	b.lo	.LBB13_10
// %bb.19:
	adrp	x0, .Lstr.48
	add	x0, x0, :lo12:.Lstr.48
	bl	puts
	orr	w0, wzr, #0x1
	b	.LBB13_21
.LBB13_20:
	adrp	x0, .Lstr.52
	add	x0, x0, :lo12:.Lstr.52
	bl	puts
	mov	w0, wzr
.LBB13_21:
	add	sp, sp, #528            // =528
	ldp	x29, x30, [sp, #48]     // 16-byte Folded Reload
	ldp	x20, x19, [sp, #32]     // 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]     // 16-byte Folded Reload
	ldp	x28, x23, [sp], #64     // 16-byte Folded Reload
	ret
.Lfunc_end13:
	.size	_Z4unitIXadL_Z13basic_decoderPjRjjmEEEbv, .Lfunc_end13-_Z4unitIXadL_Z13basic_decoderPjRjjmEEEbv
	.cfi_endproc
                                        // -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3               // -- Begin function _Z4testIXadL_Z13basic_decoderPjRjjmEEEvPKcc
.LCPI14_0:
	.xword	4636737291354636288     // double 100
.LCPI14_1:
	.xword	4634204016564240384     // double 64
	.section	.text._Z4testIXadL_Z13basic_decoderPjRjjmEEEvPKcc,"axG",@progbits,_Z4testIXadL_Z13basic_decoderPjRjjmEEEvPKcc,comdat
	.weak	_Z4testIXadL_Z13basic_decoderPjRjjmEEEvPKcc
	.p2align	2
	.type	_Z4testIXadL_Z13basic_decoderPjRjjmEEEvPKcc,@function
_Z4testIXadL_Z13basic_decoderPjRjjmEEEvPKcc: // @_Z4testIXadL_Z13basic_decoderPjRjjmEEEvPKcc
.Lfunc_begin6:
	.cfi_startproc
	.cfi_personality 0, __gxx_personality_v0
	.cfi_lsda 0, .Lexception6
// %bb.0:
	sub	sp, sp, #432            // =432
	str	d10, [sp, #304]         // 8-byte Folded Spill
	stp	d9, d8, [sp, #320]      // 16-byte Folded Spill
	stp	x28, x27, [sp, #336]    // 16-byte Folded Spill
	stp	x26, x25, [sp, #352]    // 16-byte Folded Spill
	stp	x24, x23, [sp, #368]    // 16-byte Folded Spill
	stp	x22, x21, [sp, #384]    // 16-byte Folded Spill
	stp	x20, x19, [sp, #400]    // 16-byte Folded Spill
	stp	x29, x30, [sp, #416]    // 16-byte Folded Spill
	add	x29, sp, #416           // =416
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -56
	.cfi_offset w24, -64
	.cfi_offset w25, -72
	.cfi_offset w26, -80
	.cfi_offset w27, -88
	.cfi_offset w28, -96
	.cfi_offset b8, -104
	.cfi_offset b9, -112
	.cfi_offset b10, -128
	mov	w19, w1
	add	x1, sp, #120            // =120
	bl	_Z14load_text_filePKcPm
	cbz	x0, .LBB14_8
// %bb.1:
	ldr	x24, [sp, #120]
	mov	x21, x0
	lsr	x22, x24, #6
	lsl	x23, x22, #3
	mov	x0, x23
	bl	_Znam
	mov	w1, wzr
	mov	x2, x23
	mov	x20, x0
	bl	memset
	ands	x8, x24, #0xffffffffffffffc0
	b.eq	.LBB14_7
// %bb.2:
	mov	x9, xzr
	orr	w10, wzr, #0x1
.LBB14_3:                               // =>This Inner Loop Header: Depth=1
	ldrb	w11, [x21, x9]
	cmp	w11, w19, uxtb
	b.eq	.LBB14_5
// %bb.4:                               //   in Loop: Header=BB14_3 Depth=1
	cmp	w11, #31                // =31
	b.hi	.LBB14_6
.LBB14_5:                               //   in Loop: Header=BB14_3 Depth=1
	lsr	x11, x9, #3
	and	x11, x11, #0x1ffffffffffffff8
	ldr	x12, [x20, x11]
	lsl	x13, x10, x9
	orr	x12, x12, x13
	str	x12, [x20, x11]
.LBB14_6:                               //   in Loop: Header=BB14_3 Depth=1
	add	x9, x9, #1              // =1
	cmp	x8, x9
	b.ne	.LBB14_3
.LBB14_7:
	mov	x0, x21
	bl	_ZdaPv
	b	.LBB14_9
.LBB14_8:
	mov	x20, xzr
                                        // implicit-def: $x22
.LBB14_9:
	lsl	x25, x22, #6
	lsl	x8, x22, #8
	cmp	xzr, x25, lsr #62
	csinv	x0, x8, xzr, eq
	bl	_Znam
	mov	x21, x0
.Ltmp293:
	orr	w0, wzr, #0x4
	bl	_Znwm
.Ltmp294:
// %bb.10:
	mov	x19, x0
	str	wzr, [x0]
.Ltmp296:
	orr	w0, wzr, #0x8
	bl	_Znwm
.Ltmp297:
// %bb.11:
	ldr	w8, [x19]
	orr	w9, wzr, #0x1
	mov	x24, x0
	stp	w8, w9, [x0]
	mov	x0, x19
	bl	_ZdlPv
.Ltmp299:
	orr	w0, wzr, #0x10
	bl	_Znwm
.Ltmp300:
// %bb.12:
	ldr	x8, [x24]
	mov	w9, #5
	mov	x23, x0
	str	w9, [x0, #8]
	str	x8, [x0]
	mov	x0, x24
	bl	_ZdlPv
	orr	w8, wzr, #0x2
	str	w8, [x23, #12]
.Ltmp302:
	orr	w0, wzr, #0x20
	bl	_Znwm
.Ltmp303:
// %bb.13:
	ldr	q0, [x23]
	orr	w8, wzr, #0x3
	mov	x19, x0
	str	w8, [x0, #16]
	str	q0, [x0]
	mov	x0, x23
	bl	_ZdlPv
	stp	xzr, xzr, [sp, #104]
	str	xzr, [sp, #96]
.Ltmp305:
	mov	w0, #20
	bl	_Znwm
.Ltmp306:
// %bb.14:
	ldr	w9, [x19, #16]
	ldr	q0, [x19]
	add	x8, x0, #20             // =20
	stp	x0, x0, [sp, #96]
	stp	x8, x8, [sp, #104]
	str	w9, [x0, #16]
	str	q0, [x0]
.Ltmp308:
	add	x0, sp, #120            // =120
	add	x1, sp, #96             // =96
	bl	_ZN11LinuxEventsILi0EEC2ESt6vectorIiSaIiEE
.Ltmp309:
// %bb.15:
	ldr	x0, [sp, #96]
	cbz	x0, .LBB14_17
// %bb.16:
	bl	_ZdlPv
.LBB14_17:
	stp	xzr, xzr, [sp, #80]
	stp	xzr, xzr, [sp, #64]
	stp	xzr, xzr, [sp, #48]
.Ltmp311:
	mov	w0, #40
	bl	_Znwm
.Ltmp312:
// %bb.18:
	ldp	x24, x8, [sp, #72]
	mov	x23, x0
	movi	v0.2d, #0000000000000000
	str	xzr, [x0, #32]
	subs	x2, x8, x24
	stp	q0, q0, [x0]
	b.eq	.LBB14_20
// %bb.19:
	mov	x0, x23
	mov	x1, x24
	bl	memmove
.LBB14_20:
	cbz	x24, .LBB14_22
// %bb.21:
	mov	x0, x24
	bl	_ZdlPv
.LBB14_22:
	mov	w26, wzr
	add	x8, x23, #40            // =40
	str	x8, [sp, #88]
	stp	x23, x8, [sp, #72]
.LBB14_23:                              // =>This Loop Header: Depth=1
                                        //     Child Loop BB14_26 Depth 2
                                        //       Child Loop BB14_28 Depth 3
.Ltmp314:
	add	x0, sp, #120            // =120
	bl	_ZN11LinuxEventsILi0EE5startEv
.Ltmp315:
// %bb.24:                              //   in Loop: Header=BB14_23 Depth=1
	cbz	x22, .LBB14_31
// %bb.25:                              //   in Loop: Header=BB14_23 Depth=1
	mov	x8, xzr
	mov	w23, wzr
.LBB14_26:                              //   Parent Loop BB14_23 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB14_28 Depth 3
	ldr	x9, [x20, x8, lsl #3]
	fmov	d0, x9
	cnt	v0.8b, v0.8b
	uaddlv	h0, v0.8b
	fmov	w11, s0
	cbz	w11, .LBB14_30
// %bb.27:                              //   in Loop: Header=BB14_26 Depth=2
	add	w10, w23, w11
	add	w11, w11, #1            // =1
.LBB14_28:                              //   Parent Loop BB14_23 Depth=1
                                        //     Parent Loop BB14_26 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	rbit	x12, x9
	clz	x12, x12
	sub	x13, x9, #1             // =1
	sub	w11, w11, #1            // =1
	add	w12, w12, w8
	and	x9, x13, x9
	cmp	w11, #1                 // =1
	str	w12, [x21, w23, uxtw #2]
	add	w23, w23, #1            // =1
	b.gt	.LBB14_28
// %bb.29:                              //   in Loop: Header=BB14_26 Depth=2
	mov	w23, w10
.LBB14_30:                              //   in Loop: Header=BB14_26 Depth=2
	add	x8, x8, #1              // =1
	cmp	x8, x22
	b.ne	.LBB14_26
	b	.LBB14_32
.LBB14_31:                              //   in Loop: Header=BB14_23 Depth=1
	mov	w23, wzr
.LBB14_32:                              //   in Loop: Header=BB14_23 Depth=1
.Ltmp316:
	add	x0, sp, #120            // =120
	add	x1, sp, #72             // =72
	bl	_ZN11LinuxEventsILi0EE3endERSt6vectorIySaIyEE
.Ltmp317:
// %bb.33:                              //   in Loop: Header=BB14_23 Depth=1
	ldp	x24, x8, [sp, #56]
	cmp	x24, x8
	b.eq	.LBB14_38
// %bb.34:                              //   in Loop: Header=BB14_23 Depth=1
	ldp	x9, x8, [sp, #72]
	stp	xzr, xzr, [x24]
	str	xzr, [x24, #16]
	subs	x0, x8, x9
	asr	x27, x0, #3
	b.eq	.LBB14_39
// %bb.35:                              //   in Loop: Header=BB14_23 Depth=1
	lsr	x8, x27, #61
	cbnz	x8, .LBB14_127
// %bb.36:                              //   in Loop: Header=BB14_23 Depth=1
.Ltmp318:
	bl	_Znwm
.Ltmp319:
// %bb.37:                              //   in Loop: Header=BB14_23 Depth=1
	mov	x28, x0
	b	.LBB14_40
.LBB14_38:                              //   in Loop: Header=BB14_23 Depth=1
.Ltmp323:
	add	x0, sp, #48             // =48
	add	x2, sp, #72             // =72
	mov	x1, x24
	bl	_ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_
.Ltmp324:
	b	.LBB14_43
.LBB14_39:                              //   in Loop: Header=BB14_23 Depth=1
	mov	x28, xzr
.LBB14_40:                              //   in Loop: Header=BB14_23 Depth=1
	add	x8, x28, x27, lsl #3
	stp	x28, x0, [x24]
	str	x8, [x24, #16]
	ldp	x1, x8, [sp, #72]
	subs	x2, x8, x1
	asr	x27, x2, #3
	b.eq	.LBB14_42
// %bb.41:                              //   in Loop: Header=BB14_23 Depth=1
	bl	memmove
.LBB14_42:                              //   in Loop: Header=BB14_23 Depth=1
	add	x8, x28, x27, lsl #3
	str	x8, [x24, #8]
	ldr	x8, [sp, #56]
	add	x8, x8, #24             // =24
	str	x8, [sp, #56]
.LBB14_43:                              //   in Loop: Header=BB14_23 Depth=1
	add	w26, w26, #1            // =1
	cmp	w26, #10                // =10
	b.lo	.LBB14_23
// %bb.44:
	ldr	w8, [x21]
	cbnz	w8, .LBB14_46
// %bb.45:
	adrp	x0, .L.str.3
	add	x0, x0, :lo12:.L.str.3
	bl	printf
.LBB14_46:
	adrp	x8, .LCPI14_0
	ldr	d0, [x8, :lo12:.LCPI14_0]
	ucvtf	d9, w23
	ucvtf	d8, x25
	fdiv	d1, d9, d8
	adrp	x0, .L.str.4
	mov	x28, #-6148914691236517206
	fmul	d0, d1, d0
	add	x0, x0, :lo12:.L.str.4
	mov	w1, w23
	mov	x2, x22
	movk	x28, #2730, lsl #48
	bl	printf
	adrp	x8, .LCPI14_1
	ldr	d0, [x8, :lo12:.LCPI14_1]
	ucvtf	d10, x22
	adrp	x0, .L.str.5
	add	x0, x0, :lo12:.L.str.5
	fmul	d0, d10, d0
	fdiv	d0, d0, d9
	bl	printf
	ldp	x0, x1, [sp, #48]
	subs	x8, x1, x0
	b.eq	.LBB14_50
// %bb.47:
	mov	x10, #-6148914691236517206
	asr	x9, x8, #3
	movk	x10, #43691
	mul	x9, x9, x10
	cmp	x9, x28
	b.hi	.LBB14_129
// %bb.48:
.Ltmp326:
	mov	x0, x8
	bl	_Znwm
.Ltmp327:
// %bb.49:
	mov	x23, x0
	ldp	x0, x1, [sp, #48]
	mov	x24, x23
	b	.LBB14_51
.LBB14_50:
	mov	x23, xzr
	mov	x24, xzr
.LBB14_51:
.Ltmp331:
	mov	x2, x24
	bl	_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_
.Ltmp332:
// %bb.52:
	mov	x25, x0
	subs	x27, x0, x24
	b.eq	.LBB14_58
// %bb.53:
	ldp	x8, x9, [x24]
	subs	x0, x9, x8
	b.eq	.LBB14_59
// %bb.54:
	asr	x8, x0, #3
	lsr	x8, x8, #61
	cbnz	x8, .LBB14_133
// %bb.55:
.Ltmp334:
	bl	_Znwm
.Ltmp335:
// %bb.56:
	ldp	x1, x8, [x24]
	mov	x26, x0
	subs	x2, x8, x1
	b.eq	.LBB14_60
// %bb.57:
	mov	x0, x26
	asr	x28, x2, #3
	bl	memmove
	mov	w8, wzr
	b	.LBB14_61
.LBB14_58:
	mov	x22, xzr
	cbnz	x24, .LBB14_90
	b	.LBB14_91
.LBB14_59:
	mov	x22, xzr
	mov	x26, xzr
	mov	x28, xzr
	orr	w8, wzr, #0x1
	b	.LBB14_62
.LBB14_60:
	mov	x28, xzr
	orr	w8, wzr, #0x1
.LBB14_61:
	mov	x22, x26
.LBB14_62:
	mov	x10, #-6148914691236517206
	asr	x9, x27, #3
	movk	x10, #43691
	mul	x9, x9, x10
	cmp	x9, #2                  // =2
	b.lo	.LBB14_85
// %bb.63:
	cmp	x28, #1                 // =1
	csinc	x12, x28, xzr, hi
	add	x10, x22, #16           // =16
	orr	w11, wzr, #0x1
	add	x13, x22, x12, lsl #3
	and	x14, x12, #0xfffffffffffffffc
	orr	w15, wzr, #0x18
	b	.LBB14_75
.LBB14_64:                              //   in Loop: Header=BB14_75 Depth=1
	add	x17, x16, #16           // =16
	mov	x18, x10
	mov	x0, x14
.LBB14_65:                              //   Parent Loop BB14_75 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldp	q1, q0, [x17, #-16]
	ldp	q3, q2, [x18, #-16]
	cmhi	v3.2d, v3.2d, v1.2d
	xtn	v3.2s, v3.2d
	fmov	w1, s3
	tbz	w1, #0, .LBB14_67
// %bb.66:                              //   in Loop: Header=BB14_65 Depth=2
	stur	d1, [x18, #-16]
.LBB14_67:                              //   in Loop: Header=BB14_65 Depth=2
	mov	w1, v3.s[1]
	tbz	w1, #0, .LBB14_69
// %bb.68:                              //   in Loop: Header=BB14_65 Depth=2
	sub	x1, x18, #8             // =8
	st1	{ v1.d }[1], [x1]
.LBB14_69:                              //   in Loop: Header=BB14_65 Depth=2
	cmhi	v1.2d, v2.2d, v0.2d
	xtn	v1.2s, v1.2d
	fmov	w1, s1
	tbz	w1, #0, .LBB14_71
// %bb.70:                              //   in Loop: Header=BB14_65 Depth=2
	str	d0, [x18]
.LBB14_71:                              //   in Loop: Header=BB14_65 Depth=2
	mov	w1, v1.s[1]
	tbz	w1, #0, .LBB14_73
// %bb.72:                              //   in Loop: Header=BB14_65 Depth=2
	add	x1, x18, #8             // =8
	st1	{ v0.d }[1], [x1]
.LBB14_73:                              //   in Loop: Header=BB14_65 Depth=2
	subs	x0, x0, #4              // =4
	add	x18, x18, #32           // =32
	add	x17, x17, #32           // =32
	b.ne	.LBB14_65
// %bb.74:                              //   in Loop: Header=BB14_75 Depth=1
	cmp	x12, x14
	mov	x17, x14
	b.ne	.LBB14_80
	b	.LBB14_83
.LBB14_75:                              // =>This Loop Header: Depth=1
                                        //     Child Loop BB14_65 Depth 2
                                        //     Child Loop BB14_80 Depth 2
	tbnz	w8, #0, .LBB14_83
// %bb.76:                              //   in Loop: Header=BB14_75 Depth=1
	mul	x16, x11, x15
	ldr	x16, [x24, x16]
	cmp	x12, #3                 // =3
	b.ls	.LBB14_79
// %bb.77:                              //   in Loop: Header=BB14_75 Depth=1
	add	x17, x16, x12, lsl #3
	cmp	x26, x17
	b.hs	.LBB14_64
// %bb.78:                              //   in Loop: Header=BB14_75 Depth=1
	cmp	x16, x13
	b.hs	.LBB14_64
.LBB14_79:                              //   in Loop: Header=BB14_75 Depth=1
	mov	x17, xzr
.LBB14_80:                              //   Parent Loop BB14_75 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x0, x17, #3
	ldr	x18, [x16, x0]
	ldr	x0, [x22, x0]
	cmp	x18, x0
	b.hs	.LBB14_82
// %bb.81:                              //   in Loop: Header=BB14_80 Depth=2
	str	x18, [x22, x17, lsl #3]
.LBB14_82:                              //   in Loop: Header=BB14_80 Depth=2
	add	x17, x17, #1            // =1
	cmp	x17, x28
	b.lo	.LBB14_80
.LBB14_83:                              //   in Loop: Header=BB14_75 Depth=1
	add	x11, x11, #1            // =1
	cmp	x11, x9
	b.ne	.LBB14_75
// %bb.84:
	cmp	x25, x24
	b.eq	.LBB14_126
.LBB14_85:
	mov	x28, #-6148914691236517206
	mov	x26, x24
	movk	x28, #2730, lsl #48
.LBB14_86:                              // =>This Inner Loop Header: Depth=1
	ldr	x0, [x26]
	cbz	x0, .LBB14_88
// %bb.87:                              //   in Loop: Header=BB14_86 Depth=1
	bl	_ZdlPv
.LBB14_88:                              //   in Loop: Header=BB14_86 Depth=1
	add	x26, x26, #24           // =24
	cmp	x25, x26
	b.ne	.LBB14_86
// %bb.89:
	cbz	x24, .LBB14_91
.LBB14_90:
	mov	x0, x23
	bl	_ZdlPv
.LBB14_91:
	ldp	x0, x1, [sp, #48]
	mov	x9, #-6148914691236517206
	movk	x9, #43691
	stp	xzr, xzr, [sp]
	subs	x8, x1, x0
	asr	x10, x8, #3
	mul	x25, x10, x9
	str	xzr, [sp, #16]
	b.eq	.LBB14_95
// %bb.92:
	cmp	x25, x28
	b.hi	.LBB14_131
// %bb.93:
.Ltmp339:
	mov	x0, x8
	bl	_Znwm
.Ltmp340:
// %bb.94:
	mov	x23, x0
	ldp	x0, x1, [sp, #48]
	mov	x24, x23
	b	.LBB14_96
.LBB14_95:
	mov	x23, xzr
	mov	x24, xzr
.LBB14_96:
	orr	w8, wzr, #0x18
	madd	x8, x25, x8, x24
	stp	x23, x23, [sp]
	str	x8, [sp, #16]
.Ltmp344:
	mov	x2, x24
	bl	_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_
.Ltmp345:
// %bb.97:
	mov	x25, x0
	str	x0, [sp, #8]
.Ltmp347:
	add	x8, sp, #24             // =24
	mov	x0, sp
	bl	_Z16compute_averagesSt6vectorIS_IySaIyEESaIS1_EE
.Ltmp348:
// %bb.98:
	cmp	x24, x25
	b.eq	.LBB14_103
// %bb.99:
	mov	x26, x24
.LBB14_100:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x26]
	cbz	x0, .LBB14_102
// %bb.101:                             //   in Loop: Header=BB14_100 Depth=1
	bl	_ZdlPv
.LBB14_102:                             //   in Loop: Header=BB14_100 Depth=1
	add	x26, x26, #24           // =24
	cmp	x25, x26
	b.ne	.LBB14_100
.LBB14_103:
	cbz	x24, .LBB14_105
// %bb.104:
	mov	x0, x23
	bl	_ZdlPv
.LBB14_105:
	ldp	d1, d0, [x22]
	adrp	x0, .L.str.6
	add	x0, x0, :lo12:.L.str.6
	ucvtf	d4, d0
	ucvtf	d3, d1
	fdiv	d0, d4, d3
	fdiv	d1, d3, d9
	fdiv	d2, d4, d9
	fdiv	d3, d3, d10
	fdiv	d4, d4, d10
	bl	printf
	ldp	d0, d1, [x22]
	adrp	x0, .L.str.7
	add	x0, x0, :lo12:.L.str.7
	ucvtf	d0, d0
	ucvtf	d1, d1
	fdiv	d0, d0, d8
	fdiv	d1, d1, d8
	bl	printf
	ldp	x1, x2, [x22]
	ldp	x3, x4, [x22, #16]
	ldr	x5, [x22, #32]
	adrp	x0, .L.str.8
	add	x0, x0, :lo12:.L.str.8
	bl	printf
	ldr	x23, [sp, #24]
	adrp	x0, .L.str.9
	add	x0, x0, :lo12:.L.str.9
	ldp	d0, d1, [x23]
	ldp	d2, d3, [x23, #16]
	ldr	d4, [x23, #32]
	bl	printf
	mov	w0, #10
	bl	putchar
	cbz	x20, .LBB14_107
// %bb.106:
	mov	x0, x20
	bl	_ZdaPv
.LBB14_107:
	mov	x0, x21
	bl	_ZdaPv
	cbz	x23, .LBB14_109
// %bb.108:
	mov	x0, x23
	bl	_ZdlPv
.LBB14_109:
	cbz	x22, .LBB14_111
// %bb.110:
	mov	x0, x22
	bl	_ZdlPv
.LBB14_111:
	ldp	x20, x21, [sp, #48]
	cmp	x20, x21
	b.eq	.LBB14_116
.LBB14_112:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x20]
	cbz	x0, .LBB14_114
// %bb.113:                             //   in Loop: Header=BB14_112 Depth=1
	bl	_ZdlPv
.LBB14_114:                             //   in Loop: Header=BB14_112 Depth=1
	add	x20, x20, #24           // =24
	cmp	x21, x20
	b.ne	.LBB14_112
// %bb.115:
	ldr	x20, [sp, #48]
.LBB14_116:
	cbz	x20, .LBB14_118
// %bb.117:
	mov	x0, x20
	bl	_ZdlPv
.LBB14_118:
	ldr	x0, [sp, #72]
	cbz	x0, .LBB14_120
// %bb.119:
	bl	_ZdlPv
.LBB14_120:
	ldr	w0, [sp, #120]
.Ltmp353:
	bl	close
.Ltmp354:
// %bb.121:
	ldr	x0, [sp, #272]
	cbz	x0, .LBB14_123
// %bb.122:
	bl	_ZdlPv
.LBB14_123:
	ldr	x0, [sp, #248]
	cbz	x0, .LBB14_125
// %bb.124:
	bl	_ZdlPv
.LBB14_125:
	mov	x0, x19
	bl	_ZdlPv
	ldp	x29, x30, [sp, #416]    // 16-byte Folded Reload
	ldp	x20, x19, [sp, #400]    // 16-byte Folded Reload
	ldp	x22, x21, [sp, #384]    // 16-byte Folded Reload
	ldp	x24, x23, [sp, #368]    // 16-byte Folded Reload
	ldp	x26, x25, [sp, #352]    // 16-byte Folded Reload
	ldp	x28, x27, [sp, #336]    // 16-byte Folded Reload
	ldp	d9, d8, [sp, #320]      // 16-byte Folded Reload
	ldr	d10, [sp, #304]         // 8-byte Folded Reload
	add	sp, sp, #432            // =432
	ret
.LBB14_126:
	mov	x28, #-6148914691236517206
	movk	x28, #2730, lsl #48
	cbnz	x24, .LBB14_90
	b	.LBB14_91
.LBB14_127:
.Ltmp320:
	bl	_ZSt17__throw_bad_allocv
.Ltmp321:
// %bb.128:
.LBB14_129:
.Ltmp328:
	bl	_ZSt17__throw_bad_allocv
.Ltmp329:
// %bb.130:
.LBB14_131:
.Ltmp341:
	bl	_ZSt17__throw_bad_allocv
.Ltmp342:
// %bb.132:
.LBB14_133:
.Ltmp336:
	bl	_ZSt17__throw_bad_allocv
.Ltmp337:
// %bb.134:
.LBB14_135:
.Ltmp322:
	b	.LBB14_168
.LBB14_136:
.Ltmp338:
	mov	x20, x0
	mov	x21, x24
.LBB14_137:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x21]
	cbz	x0, .LBB14_139
// %bb.138:                             //   in Loop: Header=BB14_137 Depth=1
	bl	_ZdlPv
.LBB14_139:                             //   in Loop: Header=BB14_137 Depth=1
	add	x21, x21, #24           // =24
	cmp	x25, x21
	b.ne	.LBB14_137
// %bb.140:
	cbnz	x24, .LBB14_156
	b	.LBB14_169
.LBB14_141:
.Ltmp343:
	mov	x20, x0
	cbnz	x22, .LBB14_154
	b	.LBB14_169
.LBB14_142:
.Ltmp330:
	b	.LBB14_168
.LBB14_143:
.Ltmp355:
.LBB14_144:
	add	x20, sp, #120           // =120
	mov	x19, x0
	add	x0, x20, #152           // =152
	bl	_ZNSt6vectorImSaImEED2Ev
	add	x0, x20, #128           // =128
	bl	_ZNSt6vectorImSaImEED2Ev
	mov	x0, x19
	bl	__clang_call_terminate
.LBB14_145:
.Ltmp349:
	cmp	x24, x25
	mov	x20, x0
	b.eq	.LBB14_150
// %bb.146:
	mov	x21, x24
.LBB14_147:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x21]
	cbz	x0, .LBB14_149
// %bb.148:                             //   in Loop: Header=BB14_147 Depth=1
	bl	_ZdlPv
.LBB14_149:                             //   in Loop: Header=BB14_147 Depth=1
	add	x21, x21, #24           // =24
	cmp	x25, x21
	b.ne	.LBB14_147
.LBB14_150:
	cbnz	x24, .LBB14_152
	b	.LBB14_153
.LBB14_151:
.Ltmp346:
	mov	x20, x0
	cbz	x24, .LBB14_153
.LBB14_152:
	mov	x0, x23
	bl	_ZdlPv
.LBB14_153:
	cbz	x22, .LBB14_169
.LBB14_154:
	mov	x0, x22
	b	.LBB14_157
.LBB14_155:
.Ltmp333:
	mov	x20, x0
	cbz	x24, .LBB14_169
.LBB14_156:
	mov	x0, x23
.LBB14_157:
	bl	_ZdlPv
	b	.LBB14_169
.LBB14_158:
.Ltmp313:
	b	.LBB14_168
.LBB14_159:
.Ltmp310:
	ldr	x8, [sp, #96]
	mov	x20, x0
	cbz	x8, .LBB14_183
// %bb.160:
	mov	x0, x8
	b	.LBB14_182
.LBB14_161:
.Ltmp307:
	b	.LBB14_165
.LBB14_162:
.Ltmp304:
	mov	x20, x0
	mov	x19, x23
	b	.LBB14_183
.LBB14_163:
.Ltmp301:
	mov	x20, x0
	mov	x19, x24
	b	.LBB14_183
.LBB14_164:
.Ltmp298:
.LBB14_165:
	mov	x20, x0
	b	.LBB14_183
.LBB14_166:
.Ltmp295:
	bl	_Unwind_Resume
.LBB14_167:
.Ltmp325:
.LBB14_168:
	mov	x20, x0
.LBB14_169:
	ldp	x21, x22, [sp, #48]
	cmp	x21, x22
	b.eq	.LBB14_174
.LBB14_170:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x21]
	cbz	x0, .LBB14_172
// %bb.171:                             //   in Loop: Header=BB14_170 Depth=1
	bl	_ZdlPv
.LBB14_172:                             //   in Loop: Header=BB14_170 Depth=1
	add	x21, x21, #24           // =24
	cmp	x22, x21
	b.ne	.LBB14_170
// %bb.173:
	ldr	x21, [sp, #48]
.LBB14_174:
	cbz	x21, .LBB14_176
// %bb.175:
	mov	x0, x21
	bl	_ZdlPv
.LBB14_176:
	ldr	x0, [sp, #72]
	cbz	x0, .LBB14_178
// %bb.177:
	bl	_ZdlPv
.LBB14_178:
	ldr	w0, [sp, #120]
.Ltmp350:
	bl	close
.Ltmp351:
// %bb.179:
	ldr	x0, [sp, #272]
	cbz	x0, .LBB14_181
// %bb.180:
	bl	_ZdlPv
.LBB14_181:
	ldr	x0, [sp, #248]
	cbz	x0, .LBB14_183
.LBB14_182:
	bl	_ZdlPv
.LBB14_183:
	mov	x0, x19
	bl	_ZdlPv
	mov	x0, x20
	bl	_Unwind_Resume
.LBB14_184:
.Ltmp352:
	b	.LBB14_144
.Lfunc_end14:
	.size	_Z4testIXadL_Z13basic_decoderPjRjjmEEEvPKcc, .Lfunc_end14-_Z4testIXadL_Z13basic_decoderPjRjjmEEEvPKcc
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table14:
.Lexception6:
	.byte	255                     // @LPStart Encoding = omit
	.byte	0                       // @TType Encoding = absptr
	.uleb128 .Lttbase2-.Lttbaseref2
.Lttbaseref2:
	.byte	1                       // Call site Encoding = uleb128
	.uleb128 .Lcst_end6-.Lcst_begin6
.Lcst_begin6:
	.uleb128 .Lfunc_begin6-.Lfunc_begin6 // >> Call Site 1 <<
	.uleb128 .Ltmp293-.Lfunc_begin6 //   Call between .Lfunc_begin6 and .Ltmp293
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp293-.Lfunc_begin6 // >> Call Site 2 <<
	.uleb128 .Ltmp294-.Ltmp293      //   Call between .Ltmp293 and .Ltmp294
	.uleb128 .Ltmp295-.Lfunc_begin6 //     jumps to .Ltmp295
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp296-.Lfunc_begin6 // >> Call Site 3 <<
	.uleb128 .Ltmp297-.Ltmp296      //   Call between .Ltmp296 and .Ltmp297
	.uleb128 .Ltmp298-.Lfunc_begin6 //     jumps to .Ltmp298
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp299-.Lfunc_begin6 // >> Call Site 4 <<
	.uleb128 .Ltmp300-.Ltmp299      //   Call between .Ltmp299 and .Ltmp300
	.uleb128 .Ltmp301-.Lfunc_begin6 //     jumps to .Ltmp301
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp302-.Lfunc_begin6 // >> Call Site 5 <<
	.uleb128 .Ltmp303-.Ltmp302      //   Call between .Ltmp302 and .Ltmp303
	.uleb128 .Ltmp304-.Lfunc_begin6 //     jumps to .Ltmp304
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp305-.Lfunc_begin6 // >> Call Site 6 <<
	.uleb128 .Ltmp306-.Ltmp305      //   Call between .Ltmp305 and .Ltmp306
	.uleb128 .Ltmp307-.Lfunc_begin6 //     jumps to .Ltmp307
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp308-.Lfunc_begin6 // >> Call Site 7 <<
	.uleb128 .Ltmp309-.Ltmp308      //   Call between .Ltmp308 and .Ltmp309
	.uleb128 .Ltmp310-.Lfunc_begin6 //     jumps to .Ltmp310
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp311-.Lfunc_begin6 // >> Call Site 8 <<
	.uleb128 .Ltmp312-.Ltmp311      //   Call between .Ltmp311 and .Ltmp312
	.uleb128 .Ltmp313-.Lfunc_begin6 //     jumps to .Ltmp313
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp312-.Lfunc_begin6 // >> Call Site 9 <<
	.uleb128 .Ltmp314-.Ltmp312      //   Call between .Ltmp312 and .Ltmp314
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp314-.Lfunc_begin6 // >> Call Site 10 <<
	.uleb128 .Ltmp324-.Ltmp314      //   Call between .Ltmp314 and .Ltmp324
	.uleb128 .Ltmp325-.Lfunc_begin6 //     jumps to .Ltmp325
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp324-.Lfunc_begin6 // >> Call Site 11 <<
	.uleb128 .Ltmp326-.Ltmp324      //   Call between .Ltmp324 and .Ltmp326
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp326-.Lfunc_begin6 // >> Call Site 12 <<
	.uleb128 .Ltmp327-.Ltmp326      //   Call between .Ltmp326 and .Ltmp327
	.uleb128 .Ltmp330-.Lfunc_begin6 //     jumps to .Ltmp330
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp331-.Lfunc_begin6 // >> Call Site 13 <<
	.uleb128 .Ltmp332-.Ltmp331      //   Call between .Ltmp331 and .Ltmp332
	.uleb128 .Ltmp333-.Lfunc_begin6 //     jumps to .Ltmp333
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp334-.Lfunc_begin6 // >> Call Site 14 <<
	.uleb128 .Ltmp335-.Ltmp334      //   Call between .Ltmp334 and .Ltmp335
	.uleb128 .Ltmp338-.Lfunc_begin6 //     jumps to .Ltmp338
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp335-.Lfunc_begin6 // >> Call Site 15 <<
	.uleb128 .Ltmp339-.Ltmp335      //   Call between .Ltmp335 and .Ltmp339
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp339-.Lfunc_begin6 // >> Call Site 16 <<
	.uleb128 .Ltmp340-.Ltmp339      //   Call between .Ltmp339 and .Ltmp340
	.uleb128 .Ltmp343-.Lfunc_begin6 //     jumps to .Ltmp343
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp344-.Lfunc_begin6 // >> Call Site 17 <<
	.uleb128 .Ltmp345-.Ltmp344      //   Call between .Ltmp344 and .Ltmp345
	.uleb128 .Ltmp346-.Lfunc_begin6 //     jumps to .Ltmp346
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp347-.Lfunc_begin6 // >> Call Site 18 <<
	.uleb128 .Ltmp348-.Ltmp347      //   Call between .Ltmp347 and .Ltmp348
	.uleb128 .Ltmp349-.Lfunc_begin6 //     jumps to .Ltmp349
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp353-.Lfunc_begin6 // >> Call Site 19 <<
	.uleb128 .Ltmp354-.Ltmp353      //   Call between .Ltmp353 and .Ltmp354
	.uleb128 .Ltmp355-.Lfunc_begin6 //     jumps to .Ltmp355
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp320-.Lfunc_begin6 // >> Call Site 20 <<
	.uleb128 .Ltmp321-.Ltmp320      //   Call between .Ltmp320 and .Ltmp321
	.uleb128 .Ltmp322-.Lfunc_begin6 //     jumps to .Ltmp322
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp328-.Lfunc_begin6 // >> Call Site 21 <<
	.uleb128 .Ltmp329-.Ltmp328      //   Call between .Ltmp328 and .Ltmp329
	.uleb128 .Ltmp330-.Lfunc_begin6 //     jumps to .Ltmp330
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp341-.Lfunc_begin6 // >> Call Site 22 <<
	.uleb128 .Ltmp342-.Ltmp341      //   Call between .Ltmp341 and .Ltmp342
	.uleb128 .Ltmp343-.Lfunc_begin6 //     jumps to .Ltmp343
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp336-.Lfunc_begin6 // >> Call Site 23 <<
	.uleb128 .Ltmp337-.Ltmp336      //   Call between .Ltmp336 and .Ltmp337
	.uleb128 .Ltmp338-.Lfunc_begin6 //     jumps to .Ltmp338
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp337-.Lfunc_begin6 // >> Call Site 24 <<
	.uleb128 .Ltmp350-.Ltmp337      //   Call between .Ltmp337 and .Ltmp350
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp350-.Lfunc_begin6 // >> Call Site 25 <<
	.uleb128 .Ltmp351-.Ltmp350      //   Call between .Ltmp350 and .Ltmp351
	.uleb128 .Ltmp352-.Lfunc_begin6 //     jumps to .Ltmp352
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp351-.Lfunc_begin6 // >> Call Site 26 <<
	.uleb128 .Lfunc_end14-.Ltmp351  //   Call between .Ltmp351 and .Lfunc_end14
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
.Lcst_end6:
	.byte	1                       // >> Action Record 1 <<
                                        //   Catch TypeInfo 1
	.byte	0                       //   No further actions
	.p2align	2
                                        // >> Catch TypeInfos <<
	.xword	0                       // TypeInfo 1
.Lttbase2:
	.p2align	2
                                        // -- End function
	.section	.text.__clang_call_terminate,"axG",@progbits,__clang_call_terminate,comdat
	.hidden	__clang_call_terminate  // -- Begin function __clang_call_terminate
	.weak	__clang_call_terminate
	.p2align	2
	.type	__clang_call_terminate,@function
__clang_call_terminate:                 // @__clang_call_terminate
// %bb.0:
	str	x30, [sp, #-16]!        // 8-byte Folded Spill
	bl	__cxa_begin_catch
	bl	_ZSt9terminatev
.Lfunc_end15:
	.size	__clang_call_terminate, .Lfunc_end15-__clang_call_terminate
                                        // -- End function
	.section	.text._ZN11LinuxEventsILi0EE12report_errorERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE,"axG",@progbits,_ZN11LinuxEventsILi0EE12report_errorERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE,comdat
	.weak	_ZN11LinuxEventsILi0EE12report_errorERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE // -- Begin function _ZN11LinuxEventsILi0EE12report_errorERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
	.p2align	2
	.type	_ZN11LinuxEventsILi0EE12report_errorERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE,@function
_ZN11LinuxEventsILi0EE12report_errorERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE: // @_ZN11LinuxEventsILi0EE12report_errorERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
.Lfunc_begin7:
	.cfi_startproc
	.cfi_personality 0, __gxx_personality_v0
	.cfi_lsda 0, .Lexception7
// %bb.0:
	sub	sp, sp, #176            // =176
	stp	x24, x23, [sp, #112]    // 16-byte Folded Spill
	stp	x22, x21, [sp, #128]    // 16-byte Folded Spill
	stp	x20, x19, [sp, #144]    // 16-byte Folded Spill
	stp	x29, x30, [sp, #160]    // 16-byte Folded Spill
	add	x29, sp, #160           // =160
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -56
	.cfi_offset w24, -64
	ldrb	w8, [x0, #4]
	mov	x19, x0
	cbz	w8, .LBB16_30
// %bb.1:
	adrp	x9, .L.str.19
	add	x9, x9, :lo12:.L.str.19
	add	x8, sp, #40             // =40
	mov	x0, x1
	mov	x1, x9
	bl	_ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EERKS8_PKS5_
	bl	__errno_location
	ldr	w0, [x0]
	bl	strerror
	add	x8, sp, #8              // =8
	add	x22, x8, #16            // =16
	str	x22, [sp, #8]
	cbz	x0, .LBB16_31
// %bb.2:
	mov	x20, x0
	bl	strlen
	mov	x21, x0
	cmp	x0, #15                 // =15
	stur	x0, [x29, #-56]
	b.ls	.LBB16_7
// %bb.3:
.Ltmp356:
	add	x0, sp, #8              // =8
	sub	x1, x29, #56            // =56
	mov	x2, xzr
	bl	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm
.Ltmp357:
// %bb.4:
	ldur	x8, [x29, #-56]
	str	x0, [sp, #8]
	str	x8, [sp, #24]
	cbz	x21, .LBB16_9
.LBB16_5:
	cmp	x21, #1                 // =1
	b.ne	.LBB16_8
// %bb.6:
	ldrb	w8, [x20]
	strb	w8, [x0]
	b	.LBB16_9
.LBB16_7:
	mov	x0, x22
	cbnz	x21, .LBB16_5
	b	.LBB16_9
.LBB16_8:
	mov	x1, x20
	mov	x2, x21
	bl	memcpy
.LBB16_9:
	ldur	x8, [x29, #-56]
	ldr	x9, [sp, #8]
	add	x10, sp, #40            // =40
	add	x23, x10, #16           // =16
	str	x8, [sp, #16]
	strb	wzr, [x9, x8]
	ldp	x3, x4, [sp, #40]
	ldr	x8, [sp, #56]
	ldp	x1, x2, [sp, #8]
	orr	w11, wzr, #0xf
	cmp	x3, x23
	csel	x9, x11, x8, eq
	add	x8, x2, x4
	cmp	x8, x9
	b.ls	.LBB16_11
// %bb.10:
	ldr	x9, [sp, #24]
	cmp	x1, x22
	orr	w10, wzr, #0xf
	csel	x9, x10, x9, eq
	cmp	x8, x9
	b.ls	.LBB16_12
.LBB16_11:
.Ltmp360:
	add	x0, sp, #40             // =40
	bl	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_appendEPKcm
.Ltmp361:
	b	.LBB16_13
.LBB16_12:
.Ltmp358:
	add	x0, sp, #8              // =8
	mov	x1, xzr
	mov	x2, xzr
	bl	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_replaceEmmPKcm
.Ltmp359:
.LBB16_13:
	add	x8, sp, #72             // =72
	add	x24, x8, #16            // =16
	str	x24, [sp, #72]
	mov	x8, x0
	ldr	x9, [x8], #16
	cmp	x9, x8
	b.eq	.LBB16_15
// %bb.14:
	str	x9, [sp, #72]
	ldr	x9, [x0, #16]
	str	x9, [sp, #88]
	b	.LBB16_16
.LBB16_15:
	ldr	q0, [x9]
	str	q0, [x24]
.LBB16_16:
	ldr	x9, [x0, #8]
	str	x9, [sp, #80]
	stp	x8, xzr, [x0]
	strb	wzr, [x0, #16]
	ldp	x1, x2, [sp, #72]
.Ltmp363:
	adrp	x0, _ZSt4cerr
	add	x0, x0, :lo12:_ZSt4cerr
	bl	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
.Ltmp364:
// %bb.17:
	ldr	x8, [x0]
	mov	x20, x0
	ldur	x8, [x8, #-24]
	add	x8, x0, x8
	ldr	x21, [x8, #240]
	cbz	x21, .LBB16_33
// %bb.18:
	ldrb	w8, [x21, #56]
	cbz	w8, .LBB16_20
// %bb.19:
	ldrb	w1, [x21, #67]
	b	.LBB16_22
.LBB16_20:
.Ltmp365:
	mov	x0, x21
	bl	_ZNKSt5ctypeIcE13_M_widen_initEv
.Ltmp366:
// %bb.21:
	ldr	x8, [x21]
	ldr	x8, [x8, #48]
.Ltmp367:
	mov	w1, #10
	mov	x0, x21
	blr	x8
	mov	w1, w0
.Ltmp368:
.LBB16_22:
.Ltmp369:
	mov	x0, x20
	bl	_ZNSo3putEc
.Ltmp370:
// %bb.23:
.Ltmp371:
	bl	_ZNSo5flushEv
.Ltmp372:
// %bb.24:
	ldr	x0, [sp, #72]
	cmp	x0, x24
	b.eq	.LBB16_26
// %bb.25:
	bl	_ZdlPv
.LBB16_26:
	ldr	x0, [sp, #8]
	cmp	x0, x22
	b.eq	.LBB16_28
// %bb.27:
	bl	_ZdlPv
.LBB16_28:
	ldr	x0, [sp, #40]
	cmp	x0, x23
	b.eq	.LBB16_30
// %bb.29:
	bl	_ZdlPv
.LBB16_30:
	strb	wzr, [x19, #4]
	ldp	x29, x30, [sp, #160]    // 16-byte Folded Reload
	ldp	x20, x19, [sp, #144]    // 16-byte Folded Reload
	ldp	x22, x21, [sp, #128]    // 16-byte Folded Reload
	ldp	x24, x23, [sp, #112]    // 16-byte Folded Reload
	add	sp, sp, #176            // =176
	ret
.LBB16_31:
.Ltmp376:
	adrp	x0, .L.str.22
	add	x0, x0, :lo12:.L.str.22
	bl	_ZSt19__throw_logic_errorPKc
.Ltmp377:
// %bb.32:
.LBB16_33:
.Ltmp373:
	bl	_ZSt16__throw_bad_castv
.Ltmp374:
// %bb.34:
.LBB16_35:
.Ltmp378:
	mov	x19, x0
	b	.LBB16_41
.LBB16_36:
.Ltmp362:
	mov	x19, x0
	b	.LBB16_39
.LBB16_37:
.Ltmp375:
	ldr	x8, [sp, #72]
	mov	x19, x0
	cmp	x8, x24
	b.eq	.LBB16_39
// %bb.38:
	mov	x0, x8
	bl	_ZdlPv
.LBB16_39:
	ldr	x0, [sp, #8]
	cmp	x0, x22
	b.eq	.LBB16_41
// %bb.40:
	bl	_ZdlPv
.LBB16_41:
	ldr	x0, [sp, #40]
	add	x8, sp, #40             // =40
	add	x8, x8, #16             // =16
	cmp	x0, x8
	b.eq	.LBB16_43
// %bb.42:
	bl	_ZdlPv
.LBB16_43:
	mov	x0, x19
	bl	_Unwind_Resume
.Lfunc_end16:
	.size	_ZN11LinuxEventsILi0EE12report_errorERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE, .Lfunc_end16-_ZN11LinuxEventsILi0EE12report_errorERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table16:
.Lexception7:
	.byte	255                     // @LPStart Encoding = omit
	.byte	255                     // @TType Encoding = omit
	.byte	1                       // Call site Encoding = uleb128
	.uleb128 .Lcst_end7-.Lcst_begin7
.Lcst_begin7:
	.uleb128 .Lfunc_begin7-.Lfunc_begin7 // >> Call Site 1 <<
	.uleb128 .Ltmp356-.Lfunc_begin7 //   Call between .Lfunc_begin7 and .Ltmp356
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp356-.Lfunc_begin7 // >> Call Site 2 <<
	.uleb128 .Ltmp357-.Ltmp356      //   Call between .Ltmp356 and .Ltmp357
	.uleb128 .Ltmp378-.Lfunc_begin7 //     jumps to .Ltmp378
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp357-.Lfunc_begin7 // >> Call Site 3 <<
	.uleb128 .Ltmp360-.Ltmp357      //   Call between .Ltmp357 and .Ltmp360
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp360-.Lfunc_begin7 // >> Call Site 4 <<
	.uleb128 .Ltmp359-.Ltmp360      //   Call between .Ltmp360 and .Ltmp359
	.uleb128 .Ltmp362-.Lfunc_begin7 //     jumps to .Ltmp362
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp363-.Lfunc_begin7 // >> Call Site 5 <<
	.uleb128 .Ltmp372-.Ltmp363      //   Call between .Ltmp363 and .Ltmp372
	.uleb128 .Ltmp375-.Lfunc_begin7 //     jumps to .Ltmp375
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp376-.Lfunc_begin7 // >> Call Site 6 <<
	.uleb128 .Ltmp377-.Ltmp376      //   Call between .Ltmp376 and .Ltmp377
	.uleb128 .Ltmp378-.Lfunc_begin7 //     jumps to .Ltmp378
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp373-.Lfunc_begin7 // >> Call Site 7 <<
	.uleb128 .Ltmp374-.Ltmp373      //   Call between .Ltmp373 and .Ltmp374
	.uleb128 .Ltmp375-.Lfunc_begin7 //     jumps to .Ltmp375
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp374-.Lfunc_begin7 // >> Call Site 8 <<
	.uleb128 .Lfunc_end16-.Ltmp374  //   Call between .Ltmp374 and .Lfunc_end16
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
.Lcst_end7:
	.p2align	2
                                        // -- End function
	.section	.text._ZNSt6vectorImSaImEED2Ev,"axG",@progbits,_ZNSt6vectorImSaImEED2Ev,comdat
	.weak	_ZNSt6vectorImSaImEED2Ev // -- Begin function _ZNSt6vectorImSaImEED2Ev
	.p2align	2
	.type	_ZNSt6vectorImSaImEED2Ev,@function
_ZNSt6vectorImSaImEED2Ev:               // @_ZNSt6vectorImSaImEED2Ev
	.cfi_startproc
// %bb.0:
	ldr	x0, [x0]
	cbz	x0, .LBB17_2
// %bb.1:
	b	_ZdlPv
.LBB17_2:
	ret
.Lfunc_end17:
	.size	_ZNSt6vectorImSaImEED2Ev, .Lfunc_end17-_ZNSt6vectorImSaImEED2Ev
	.cfi_endproc
                                        // -- End function
	.section	.text._ZNSt6vectorImSaImEE17_M_default_appendEm,"axG",@progbits,_ZNSt6vectorImSaImEE17_M_default_appendEm,comdat
	.weak	_ZNSt6vectorImSaImEE17_M_default_appendEm // -- Begin function _ZNSt6vectorImSaImEE17_M_default_appendEm
	.p2align	2
	.type	_ZNSt6vectorImSaImEE17_M_default_appendEm,@function
_ZNSt6vectorImSaImEE17_M_default_appendEm: // @_ZNSt6vectorImSaImEE17_M_default_appendEm
	.cfi_startproc
// %bb.0:
	stp	x26, x25, [sp, #-80]!   // 16-byte Folded Spill
	stp	x24, x23, [sp, #16]     // 16-byte Folded Spill
	stp	x22, x21, [sp, #32]     // 16-byte Folded Spill
	stp	x20, x19, [sp, #48]     // 16-byte Folded Spill
	stp	x29, x30, [sp, #64]     // 16-byte Folded Spill
	add	x29, sp, #64            // =64
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -56
	.cfi_offset w24, -64
	.cfi_offset w25, -72
	.cfi_offset w26, -80
	cbz	x1, .LBB18_13
// %bb.1:
	mov	x23, x0
	mov	x25, x0
	ldr	x22, [x23, #8]!
	ldr	x21, [x0]
	ldr	x8, [x25, #16]!
	mov	x19, x1
	mov	x20, x0
	sub	x8, x8, x22
	cmp	x1, x8, asr #3
	b.ls	.LBB18_5
// %bb.2:
	sub	x8, x22, x21
	asr	x24, x8, #3
	orr	x8, xzr, #0x1fffffffffffffff
	sub	x9, x8, x24
	cmp	x9, x19
	b.lo	.LBB18_14
// %bb.3:
	cmp	x24, x19
	csel	x9, x19, x24, lo
	adds	x9, x9, x24
	lsr	x11, x9, #61
	cset	w10, hs
	cmp	x11, #0                 // =0
	cset	w11, ne
	orr	w10, w10, w11
	cmp	w10, #0                 // =0
	csel	x26, x8, x9, ne
	cbz	x26, .LBB18_6
// %bb.4:
	lsl	x0, x26, #3
	bl	_Znwm
	ldp	x21, x22, [x20]
	mov	x23, x0
	b	.LBB18_7
.LBB18_5:
	lsl	x19, x19, #3
	mov	x0, x22
	mov	w1, wzr
	mov	x2, x19
	bl	memset
	add	x8, x22, x19
	mov	x25, x23
	b	.LBB18_12
.LBB18_6:
	mov	x23, xzr
.LBB18_7:
	add	x24, x23, x24, lsl #3
	lsl	x2, x19, #3
	mov	x0, x24
	mov	w1, wzr
	bl	memset
	subs	x2, x22, x21
	b.eq	.LBB18_9
// %bb.8:
	mov	x0, x23
	mov	x1, x21
	bl	memmove
.LBB18_9:
	cbz	x21, .LBB18_11
// %bb.10:
	mov	x0, x21
	bl	_ZdlPv
.LBB18_11:
	add	x8, x24, x19, lsl #3
	stp	x23, x8, [x20]
	add	x8, x23, x26, lsl #3
.LBB18_12:
	str	x8, [x25]
.LBB18_13:
	ldp	x29, x30, [sp, #64]     // 16-byte Folded Reload
	ldp	x20, x19, [sp, #48]     // 16-byte Folded Reload
	ldp	x22, x21, [sp, #32]     // 16-byte Folded Reload
	ldp	x24, x23, [sp, #16]     // 16-byte Folded Reload
	ldp	x26, x25, [sp], #80     // 16-byte Folded Reload
	ret
.LBB18_14:
	adrp	x0, .L.str.18
	add	x0, x0, :lo12:.L.str.18
	bl	_ZSt20__throw_length_errorPKc
.Lfunc_end18:
	.size	_ZNSt6vectorImSaImEE17_M_default_appendEm, .Lfunc_end18-_ZNSt6vectorImSaImEE17_M_default_appendEm
	.cfi_endproc
                                        // -- End function
	.section	.text._ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EERKS8_PKS5_,"axG",@progbits,_ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EERKS8_PKS5_,comdat
	.weak	_ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EERKS8_PKS5_ // -- Begin function _ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EERKS8_PKS5_
	.p2align	2
	.type	_ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EERKS8_PKS5_,@function
_ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EERKS8_PKS5_: // @_ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EERKS8_PKS5_
.Lfunc_begin8:
	.cfi_startproc
	.cfi_personality 0, __gxx_personality_v0
	.cfi_lsda 0, .Lexception8
// %bb.0:
	str	x23, [sp, #-64]!        // 8-byte Folded Spill
	stp	x22, x21, [sp, #16]     // 16-byte Folded Spill
	stp	x20, x19, [sp, #32]     // 16-byte Folded Spill
	stp	x29, x30, [sp, #48]     // 16-byte Folded Spill
	add	x29, sp, #48            // =48
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -64
	add	x23, x8, #16            // =16
	str	x23, [x8]
	ldp	x21, x22, [x0]
	mov	x19, x8
	mov	x20, x1
	cbnz	x21, .LBB19_2
// %bb.1:
	cbnz	x22, .LBB19_11
.LBB19_2:
	cmp	x22, #16                // =16
	str	x22, [sp, #8]
	b.lo	.LBB19_6
// %bb.3:
	add	x1, sp, #8              // =8
	mov	x0, x19
	mov	x2, xzr
	bl	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm
	ldr	x8, [sp, #8]
	str	x0, [x19]
	str	x8, [x19, #16]
	cbz	x22, .LBB19_8
.LBB19_4:
	cmp	x22, #1                 // =1
	b.ne	.LBB19_7
// %bb.5:
	ldrb	w8, [x21]
	strb	w8, [x0]
	b	.LBB19_8
.LBB19_6:
	mov	x0, x23
	cbnz	x22, .LBB19_4
	b	.LBB19_8
.LBB19_7:
	mov	x1, x21
	mov	x2, x22
	bl	memcpy
.LBB19_8:
	ldr	x8, [sp, #8]
	ldr	x9, [x19]
	mov	x0, x20
	str	x8, [x19, #8]
	strb	wzr, [x9, x8]
	bl	strlen
	ldr	x8, [x19, #8]
	orr	x9, xzr, #0x7fffffffffffffff
	sub	x8, x9, x8
	cmp	x8, x0
	b.lo	.LBB19_12
// %bb.9:
	mov	x2, x0
.Ltmp379:
	mov	x0, x19
	mov	x1, x20
	bl	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_appendEPKcm
.Ltmp380:
// %bb.10:
	ldp	x29, x30, [sp, #48]     // 16-byte Folded Reload
	ldp	x20, x19, [sp, #32]     // 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]     // 16-byte Folded Reload
	ldr	x23, [sp], #64          // 8-byte Folded Reload
	ret
.LBB19_11:
	adrp	x0, .L.str.22
	add	x0, x0, :lo12:.L.str.22
	bl	_ZSt19__throw_logic_errorPKc
.LBB19_12:
.Ltmp381:
	adrp	x0, .L.str.23
	add	x0, x0, :lo12:.L.str.23
	bl	_ZSt20__throw_length_errorPKc
.Ltmp382:
// %bb.13:
.LBB19_14:
.Ltmp383:
	ldr	x8, [x19]
	mov	x19, x0
	cmp	x8, x23
	b.eq	.LBB19_16
// %bb.15:
	mov	x0, x8
	bl	_ZdlPv
.LBB19_16:
	mov	x0, x19
	bl	_Unwind_Resume
.Lfunc_end19:
	.size	_ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EERKS8_PKS5_, .Lfunc_end19-_ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EERKS8_PKS5_
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table19:
.Lexception8:
	.byte	255                     // @LPStart Encoding = omit
	.byte	255                     // @TType Encoding = omit
	.byte	1                       // Call site Encoding = uleb128
	.uleb128 .Lcst_end8-.Lcst_begin8
.Lcst_begin8:
	.uleb128 .Lfunc_begin8-.Lfunc_begin8 // >> Call Site 1 <<
	.uleb128 .Ltmp379-.Lfunc_begin8 //   Call between .Lfunc_begin8 and .Ltmp379
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp379-.Lfunc_begin8 // >> Call Site 2 <<
	.uleb128 .Ltmp380-.Ltmp379      //   Call between .Ltmp379 and .Ltmp380
	.uleb128 .Ltmp383-.Lfunc_begin8 //     jumps to .Ltmp383
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp380-.Lfunc_begin8 // >> Call Site 3 <<
	.uleb128 .Ltmp381-.Ltmp380      //   Call between .Ltmp380 and .Ltmp381
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp381-.Lfunc_begin8 // >> Call Site 4 <<
	.uleb128 .Ltmp382-.Ltmp381      //   Call between .Ltmp381 and .Ltmp382
	.uleb128 .Ltmp383-.Lfunc_begin8 //     jumps to .Ltmp383
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp382-.Lfunc_begin8 // >> Call Site 5 <<
	.uleb128 .Lfunc_end19-.Ltmp382  //   Call between .Ltmp382 and .Lfunc_end19
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
.Lcst_end8:
	.p2align	2
                                        // -- End function
	.section	.text._ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_,"axG",@progbits,_ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_,comdat
	.weak	_ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_ // -- Begin function _ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_
	.p2align	2
	.type	_ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_,@function
_ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_: // @_ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_
.Lfunc_begin9:
	.cfi_startproc
	.cfi_personality 0, __gxx_personality_v0
	.cfi_lsda 0, .Lexception9
// %bb.0:
	sub	sp, sp, #112            // =112
	stp	x28, x27, [sp, #16]     // 16-byte Folded Spill
	stp	x26, x25, [sp, #32]     // 16-byte Folded Spill
	stp	x24, x23, [sp, #48]     // 16-byte Folded Spill
	stp	x22, x21, [sp, #64]     // 16-byte Folded Spill
	stp	x20, x19, [sp, #80]     // 16-byte Folded Spill
	stp	x29, x30, [sp, #96]     // 16-byte Folded Spill
	add	x29, sp, #96            // =96
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -56
	.cfi_offset w24, -64
	.cfi_offset w25, -72
	.cfi_offset w26, -80
	.cfi_offset w27, -88
	.cfi_offset w28, -96
	ldp	x20, x24, [x0]
	mov	x8, #-6148914691236517206
	movk	x8, #43691
	mov	x9, #-6148914691236517206
	subs	x10, x24, x20
	asr	x10, x10, #3
	mul	x10, x10, x8
	csinc	x11, x10, xzr, ne
	movk	x9, #2730, lsl #48
	adds	x10, x11, x10
	cset	w11, hs
	cmp	x10, x9
	cset	w12, hi
	orr	w11, w11, w12
	sub	x12, x1, x20
	cmp	w11, #0                 // =0
	csel	x19, x9, x10, ne
	asr	x9, x12, #3
	mov	x23, x2
	mov	x22, x1
	mov	x25, x0
	mul	x26, x9, x8
	cbz	x19, .LBB20_2
// %bb.1:
	add	x8, x19, x19, lsl #1
	lsl	x0, x8, #3
	bl	_Znwm
	mov	x21, x0
	b	.LBB20_3
.LBB20_2:
	mov	x21, xzr
.LBB20_3:
	ldp	x1, x9, [x23]
	orr	w8, wzr, #0x18
	madd	x27, x26, x8, x21
	stp	xzr, xzr, [x27]
	subs	x0, x9, x1
	asr	x28, x0, #3
	str	xzr, [x27, #16]
	str	x25, [sp, #8]           // 8-byte Folded Spill
	b.eq	.LBB20_7
// %bb.4:
	lsr	x8, x28, #61
	cbnz	x8, .LBB20_23
// %bb.5:
.Ltmp384:
	bl	_Znwm
.Ltmp385:
// %bb.6:
	ldp	x1, x8, [x23]
	mov	x25, x0
	sub	x2, x8, x1
	asr	x23, x2, #3
	b	.LBB20_8
.LBB20_7:
	mov	x23, xzr
	mov	x2, xzr
	mov	x25, xzr
.LBB20_8:
	orr	w8, wzr, #0x18
	madd	x26, x26, x8, x21
	str	x25, [x27]
	str	x0, [x26, #8]!
	add	x8, x25, x28, lsl #3
	str	x8, [x26, #8]
	cbz	x2, .LBB20_10
// %bb.9:
	bl	memmove
.LBB20_10:
	add	x8, x25, x23, lsl #3
	cmp	x20, x22
	str	x8, [x26]
	mov	x8, x21
	b.eq	.LBB20_13
// %bb.11:
	mov	x8, x21
	mov	x9, x20
.LBB20_12:                              // =>This Inner Loop Header: Depth=1
	stp	xzr, xzr, [x8]
	str	xzr, [x8, #16]
	ldr	x10, [x9]
	str	x10, [x8]
	str	xzr, [x9]
	ldr	x10, [x9, #8]
	ldr	x11, [x8, #8]
	str	x10, [x8, #8]
	str	x11, [x9, #8]
	ldr	x10, [x9, #16]
	ldr	x11, [x8, #16]
	str	x10, [x8, #16]
	str	x11, [x9, #16]
	add	x9, x9, #24             // =24
	cmp	x9, x22
	add	x8, x8, #24             // =24
	b.ne	.LBB20_12
.LBB20_13:
	cmp	x24, x22
	add	x23, x8, #24            // =24
	b.eq	.LBB20_15
.LBB20_14:                              // =>This Inner Loop Header: Depth=1
	stp	xzr, xzr, [x23]
	str	xzr, [x23, #16]
	ldr	x8, [x22]
	str	x8, [x23]
	str	xzr, [x22]
	ldr	x8, [x22, #8]
	ldr	x9, [x23, #8]
	str	x8, [x23, #8]
	str	x9, [x22, #8]
	ldr	x8, [x22, #16]
	ldr	x9, [x23, #16]
	str	x8, [x23, #16]
	str	x9, [x22, #16]
	add	x22, x22, #24           // =24
	cmp	x22, x24
	add	x23, x23, #24           // =24
	b.ne	.LBB20_14
.LBB20_15:
	cmp	x20, x24
	b.eq	.LBB20_20
// %bb.16:
	mov	x22, x20
.LBB20_17:                              // =>This Inner Loop Header: Depth=1
	ldr	x0, [x22]
	cbz	x0, .LBB20_19
// %bb.18:                              //   in Loop: Header=BB20_17 Depth=1
	bl	_ZdlPv
.LBB20_19:                              //   in Loop: Header=BB20_17 Depth=1
	add	x22, x22, #24           // =24
	cmp	x24, x22
	b.ne	.LBB20_17
.LBB20_20:
	cbz	x20, .LBB20_22
// %bb.21:
	mov	x0, x20
	bl	_ZdlPv
.LBB20_22:
	ldr	x9, [sp, #8]            // 8-byte Folded Reload
	orr	w8, wzr, #0x18
	madd	x8, x19, x8, x21
	stp	x21, x23, [x9]
	str	x8, [x9, #16]
	ldp	x29, x30, [sp, #96]     // 16-byte Folded Reload
	ldp	x20, x19, [sp, #80]     // 16-byte Folded Reload
	ldp	x22, x21, [sp, #64]     // 16-byte Folded Reload
	ldp	x24, x23, [sp, #48]     // 16-byte Folded Reload
	ldp	x26, x25, [sp, #32]     // 16-byte Folded Reload
	ldp	x28, x27, [sp, #16]     // 16-byte Folded Reload
	add	sp, sp, #112            // =112
	ret
.LBB20_23:
.Ltmp386:
	bl	_ZSt17__throw_bad_allocv
.Ltmp387:
// %bb.24:
.LBB20_25:
.Ltmp388:
	bl	__cxa_begin_catch
	cbnz	x21, .LBB20_27
// %bb.26:
	ldr	x0, [x27]
	cbnz	x0, .LBB20_28
	b	.LBB20_29
.LBB20_27:
	mov	x0, x21
.LBB20_28:
	bl	_ZdlPv
.LBB20_29:
.Ltmp389:
	bl	__cxa_rethrow
.Ltmp390:
// %bb.30:
.LBB20_31:
.Ltmp391:
	mov	x19, x0
.Ltmp392:
	bl	__cxa_end_catch
.Ltmp393:
// %bb.32:
	mov	x0, x19
	bl	_Unwind_Resume
.LBB20_33:
.Ltmp394:
	bl	__clang_call_terminate
.Lfunc_end20:
	.size	_ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_, .Lfunc_end20-_ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table20:
.Lexception9:
	.byte	255                     // @LPStart Encoding = omit
	.byte	0                       // @TType Encoding = absptr
	.uleb128 .Lttbase3-.Lttbaseref3
.Lttbaseref3:
	.byte	1                       // Call site Encoding = uleb128
	.uleb128 .Lcst_end9-.Lcst_begin9
.Lcst_begin9:
	.uleb128 .Lfunc_begin9-.Lfunc_begin9 // >> Call Site 1 <<
	.uleb128 .Ltmp384-.Lfunc_begin9 //   Call between .Lfunc_begin9 and .Ltmp384
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp384-.Lfunc_begin9 // >> Call Site 2 <<
	.uleb128 .Ltmp385-.Ltmp384      //   Call between .Ltmp384 and .Ltmp385
	.uleb128 .Ltmp388-.Lfunc_begin9 //     jumps to .Ltmp388
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp385-.Lfunc_begin9 // >> Call Site 3 <<
	.uleb128 .Ltmp386-.Ltmp385      //   Call between .Ltmp385 and .Ltmp386
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp386-.Lfunc_begin9 // >> Call Site 4 <<
	.uleb128 .Ltmp387-.Ltmp386      //   Call between .Ltmp386 and .Ltmp387
	.uleb128 .Ltmp388-.Lfunc_begin9 //     jumps to .Ltmp388
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp387-.Lfunc_begin9 // >> Call Site 5 <<
	.uleb128 .Ltmp389-.Ltmp387      //   Call between .Ltmp387 and .Ltmp389
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp389-.Lfunc_begin9 // >> Call Site 6 <<
	.uleb128 .Ltmp390-.Ltmp389      //   Call between .Ltmp389 and .Ltmp390
	.uleb128 .Ltmp391-.Lfunc_begin9 //     jumps to .Ltmp391
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp392-.Lfunc_begin9 // >> Call Site 7 <<
	.uleb128 .Ltmp393-.Ltmp392      //   Call between .Ltmp392 and .Ltmp393
	.uleb128 .Ltmp394-.Lfunc_begin9 //     jumps to .Ltmp394
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp393-.Lfunc_begin9 // >> Call Site 8 <<
	.uleb128 .Lfunc_end20-.Ltmp393  //   Call between .Ltmp393 and .Lfunc_end20
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
.Lcst_end9:
	.byte	1                       // >> Action Record 1 <<
                                        //   Catch TypeInfo 1
	.byte	0                       //   No further actions
	.p2align	2
                                        // >> Catch TypeInfos <<
	.xword	0                       // TypeInfo 1
.Lttbase3:
	.p2align	2
                                        // -- End function
	.section	.text._ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_,"axG",@progbits,_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_,comdat
	.weak	_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_ // -- Begin function _ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_
	.p2align	2
	.type	_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_,@function
_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_: // @_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_
.Lfunc_begin10:
	.cfi_startproc
	.cfi_personality 0, __gxx_personality_v0
	.cfi_lsda 0, .Lexception10
// %bb.0:
	stp	x26, x25, [sp, #-80]!   // 16-byte Folded Spill
	stp	x24, x23, [sp, #16]     // 16-byte Folded Spill
	stp	x22, x21, [sp, #32]     // 16-byte Folded Spill
	stp	x20, x19, [sp, #48]     // 16-byte Folded Spill
	stp	x29, x30, [sp, #64]     // 16-byte Folded Spill
	add	x29, sp, #64            // =64
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -56
	.cfi_offset w24, -64
	.cfi_offset w25, -72
	.cfi_offset w26, -80
	cmp	x0, x1
	mov	x19, x2
	b.eq	.LBB21_11
// %bb.1:
	mov	x20, x0
	mov	x21, xzr
	sub	x22, x1, x0
.LBB21_2:                               // =>This Inner Loop Header: Depth=1
	add	x24, x20, x21
	ldp	x8, x9, [x24]
	add	x23, x19, x21
	stp	xzr, xzr, [x23]
	str	xzr, [x23, #16]
	subs	x0, x9, x8
	asr	x25, x0, #3
	b.eq	.LBB21_6
// %bb.3:                               //   in Loop: Header=BB21_2 Depth=1
	lsr	x8, x25, #61
	cbnz	x8, .LBB21_12
// %bb.4:                               //   in Loop: Header=BB21_2 Depth=1
.Ltmp395:
	bl	_Znwm
.Ltmp396:
// %bb.5:                               //   in Loop: Header=BB21_2 Depth=1
	mov	x26, x0
	b	.LBB21_7
.LBB21_6:                               //   in Loop: Header=BB21_2 Depth=1
	mov	x26, xzr
.LBB21_7:                               //   in Loop: Header=BB21_2 Depth=1
	add	x8, x26, x25, lsl #3
	stp	x26, x0, [x23]
	str	x8, [x23, #16]
	ldp	x1, x8, [x24]
	subs	x2, x8, x1
	asr	x24, x2, #3
	b.eq	.LBB21_9
// %bb.8:                               //   in Loop: Header=BB21_2 Depth=1
	bl	memmove
.LBB21_9:                               //   in Loop: Header=BB21_2 Depth=1
	add	x21, x21, #24           // =24
	add	x8, x26, x24, lsl #3
	cmp	x22, x21
	str	x8, [x23, #8]
	b.ne	.LBB21_2
// %bb.10:
	add	x19, x19, x21
.LBB21_11:
	mov	x0, x19
	ldp	x29, x30, [sp, #64]     // 16-byte Folded Reload
	ldp	x20, x19, [sp, #48]     // 16-byte Folded Reload
	ldp	x22, x21, [sp, #32]     // 16-byte Folded Reload
	ldp	x24, x23, [sp, #16]     // 16-byte Folded Reload
	ldp	x26, x25, [sp], #80     // 16-byte Folded Reload
	ret
.LBB21_12:
.Ltmp398:
	bl	_ZSt17__throw_bad_allocv
.Ltmp399:
// %bb.13:
.LBB21_14:
.Ltmp400:
	bl	__cxa_begin_catch
	cbnz	x21, .LBB21_16
	b	.LBB21_19
.LBB21_15:
.Ltmp397:
	bl	__cxa_begin_catch
	cbz	x21, .LBB21_19
.LBB21_16:                              // =>This Inner Loop Header: Depth=1
	ldr	x0, [x19]
	cbz	x0, .LBB21_18
// %bb.17:                              //   in Loop: Header=BB21_16 Depth=1
	bl	_ZdlPv
.LBB21_18:                              //   in Loop: Header=BB21_16 Depth=1
	subs	x21, x21, #24           // =24
	add	x19, x19, #24           // =24
	b.ne	.LBB21_16
.LBB21_19:
.Ltmp401:
	bl	__cxa_rethrow
.Ltmp402:
// %bb.20:
.LBB21_21:
.Ltmp403:
	mov	x19, x0
.Ltmp404:
	bl	__cxa_end_catch
.Ltmp405:
// %bb.22:
	mov	x0, x19
	bl	_Unwind_Resume
.LBB21_23:
.Ltmp406:
	bl	__clang_call_terminate
.Lfunc_end21:
	.size	_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_, .Lfunc_end21-_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table21:
.Lexception10:
	.byte	255                     // @LPStart Encoding = omit
	.byte	0                       // @TType Encoding = absptr
	.uleb128 .Lttbase4-.Lttbaseref4
.Lttbaseref4:
	.byte	1                       // Call site Encoding = uleb128
	.uleb128 .Lcst_end10-.Lcst_begin10
.Lcst_begin10:
	.uleb128 .Ltmp395-.Lfunc_begin10 // >> Call Site 1 <<
	.uleb128 .Ltmp396-.Ltmp395      //   Call between .Ltmp395 and .Ltmp396
	.uleb128 .Ltmp397-.Lfunc_begin10 //     jumps to .Ltmp397
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp396-.Lfunc_begin10 // >> Call Site 2 <<
	.uleb128 .Ltmp398-.Ltmp396      //   Call between .Ltmp396 and .Ltmp398
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp398-.Lfunc_begin10 // >> Call Site 3 <<
	.uleb128 .Ltmp399-.Ltmp398      //   Call between .Ltmp398 and .Ltmp399
	.uleb128 .Ltmp400-.Lfunc_begin10 //     jumps to .Ltmp400
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp399-.Lfunc_begin10 // >> Call Site 4 <<
	.uleb128 .Ltmp401-.Ltmp399      //   Call between .Ltmp399 and .Ltmp401
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp401-.Lfunc_begin10 // >> Call Site 5 <<
	.uleb128 .Ltmp402-.Ltmp401      //   Call between .Ltmp401 and .Ltmp402
	.uleb128 .Ltmp403-.Lfunc_begin10 //     jumps to .Ltmp403
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp404-.Lfunc_begin10 // >> Call Site 6 <<
	.uleb128 .Ltmp405-.Ltmp404      //   Call between .Ltmp404 and .Ltmp405
	.uleb128 .Ltmp406-.Lfunc_begin10 //     jumps to .Ltmp406
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp405-.Lfunc_begin10 // >> Call Site 7 <<
	.uleb128 .Lfunc_end21-.Ltmp405  //   Call between .Ltmp405 and .Lfunc_end21
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
.Lcst_end10:
	.byte	1                       // >> Action Record 1 <<
                                        //   Catch TypeInfo 1
	.byte	0                       //   No further actions
	.p2align	2
                                        // >> Catch TypeInfos <<
	.xword	0                       // TypeInfo 1
.Lttbase4:
	.p2align	2
                                        // -- End function
	.text
	.p2align	2               // -- Begin function _ZL14faster_decoderPjRjjm
	.type	_ZL14faster_decoderPjRjjm,@function
_ZL14faster_decoderPjRjjm:              // @_ZL14faster_decoderPjRjjm
// %bb.0:
	cbz	x3, .LBB22_6
// %bb.1:
	ldr	w8, [x1]
	rbit	x9, x3
	clz	x11, x9
	add	w11, w11, w2
	sub	x10, x3, #1             // =1
	str	w11, [x0, x8, lsl #2]
	and	x10, x10, x3
	ldr	w11, [x1]
	sub	x12, x10, #1            // =1
	and	x12, x12, x10
	rbit	x10, x10
	clz	x10, x10
	add	w10, w10, w2
	add	w11, w11, #1            // =1
	str	w10, [x0, w11, uxtw #2]
	ldr	w11, [x1]
	sub	x10, x12, #1            // =1
	and	x10, x10, x12
	rbit	x12, x12
	clz	x12, x12
	add	w12, w12, w2
	add	w11, w11, #2            // =2
	str	w12, [x0, w11, uxtw #2]
	ldr	w12, [x1]
	sub	x11, x10, #1            // =1
	and	x11, x11, x10
	rbit	x10, x10
	clz	x10, x10
	add	w10, w10, w2
	add	w12, w12, #3            // =3
	str	w10, [x0, w12, uxtw #2]
	ldr	w12, [x1]
	sub	x10, x11, #1            // =1
	and	x10, x10, x11
	rbit	x11, x11
	clz	x11, x11
	add	w11, w11, w2
	add	w12, w12, #4            // =4
	str	w11, [x0, w12, uxtw #2]
	ldr	w12, [x1]
	sub	x11, x10, #1            // =1
	and	x11, x11, x10
	rbit	x10, x10
	clz	x10, x10
	add	w10, w10, w2
	add	w12, w12, #5            // =5
	str	w10, [x0, w12, uxtw #2]
	ldr	w12, [x1]
	sub	x10, x11, #1            // =1
	and	x10, x10, x11
	rbit	x11, x11
	clz	x11, x11
	add	w11, w11, w2
	add	w12, w12, #6            // =6
	str	w11, [x0, w12, uxtw #2]
	fmov	d0, x3
	ldr	w11, [x1]
	cnt	v0.8b, v0.8b
	uaddlv	h0, v0.8b
	rbit	x12, x10
	fmov	w9, s0
	clz	x12, x12
	add	w8, w8, w9
	add	w12, w12, w2
	add	w11, w11, #7            // =7
	cmp	w9, #9                  // =9
	str	w12, [x0, w11, uxtw #2]
	b.lo	.LBB22_5
// %bb.2:
	sub	x11, x10, #1            // =1
	ldr	w12, [x1]
	and	x10, x11, x10
	sub	x11, x10, #1            // =1
	and	x11, x11, x10
	rbit	x10, x10
	clz	x10, x10
	add	w10, w10, w2
	add	w12, w12, #8            // =8
	str	w10, [x0, w12, uxtw #2]
	ldr	w12, [x1]
	sub	x10, x11, #1            // =1
	and	x10, x10, x11
	rbit	x11, x11
	clz	x11, x11
	add	w11, w11, w2
	add	w12, w12, #9            // =9
	str	w11, [x0, w12, uxtw #2]
	ldr	w12, [x1]
	sub	x11, x10, #1            // =1
	and	x11, x11, x10
	rbit	x10, x10
	clz	x10, x10
	add	w10, w10, w2
	add	w12, w12, #10           // =10
	str	w10, [x0, w12, uxtw #2]
	ldr	w12, [x1]
	sub	x10, x11, #1            // =1
	and	x10, x10, x11
	rbit	x11, x11
	clz	x11, x11
	add	w11, w11, w2
	add	w12, w12, #11           // =11
	str	w11, [x0, w12, uxtw #2]
	ldr	w12, [x1]
	sub	x11, x10, #1            // =1
	and	x11, x11, x10
	rbit	x10, x10
	clz	x10, x10
	add	w10, w10, w2
	add	w12, w12, #12           // =12
	str	w10, [x0, w12, uxtw #2]
	sub	x10, x11, #1            // =1
	and	x12, x10, x11
	ldr	w10, [x1]
	rbit	x11, x11
	clz	x11, x11
	add	w11, w11, w2
	add	w10, w10, #13           // =13
	str	w11, [x0, w10, uxtw #2]
	ldr	w11, [x1]
	sub	x10, x12, #1            // =1
	and	x10, x10, x12
	rbit	x12, x12
	clz	x12, x12
	add	w12, w12, w2
	add	w11, w11, #14           // =14
	str	w12, [x0, w11, uxtw #2]
	ldr	w11, [x1]
	rbit	x12, x10
	clz	x12, x12
	add	w12, w12, w2
	add	w11, w11, #15           // =15
	cmp	w9, #17                 // =17
	str	w12, [x0, w11, uxtw #2]
	b.lo	.LBB22_5
// %bb.3:
	ldr	w11, [x1]
	sub	x9, x10, #1             // =1
	and	x9, x9, x10
	add	w10, w11, #16           // =16
	str	w10, [x1]
.LBB22_4:                               // =>This Inner Loop Header: Depth=1
	rbit	x11, x9
	clz	x11, x11
	add	w11, w11, w2
	str	w11, [x0, w10, uxtw #2]
	ldr	w10, [x1]
	sub	x11, x9, #1             // =1
	ands	x9, x11, x9
	add	w10, w10, #1            // =1
	str	w10, [x1]
	b.ne	.LBB22_4
.LBB22_5:
	str	w8, [x1]
.LBB22_6:
	ret
.Lfunc_end22:
	.size	_ZL14faster_decoderPjRjjm, .Lfunc_end22-_ZL14faster_decoderPjRjjm
                                        // -- End function
	.section	.text.startup,"ax",@progbits
	.p2align	2               // -- Begin function _GLOBAL__sub_I_bitmapdecoding.cpp
	.type	_GLOBAL__sub_I_bitmapdecoding.cpp,@function
_GLOBAL__sub_I_bitmapdecoding.cpp:      // @_GLOBAL__sub_I_bitmapdecoding.cpp
	.cfi_startproc
// %bb.0:
	str	x19, [sp, #-32]!        // 8-byte Folded Spill
	stp	x29, x30, [sp, #16]     // 16-byte Folded Spill
	add	x29, sp, #16            // =16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -32
	adrp	x19, _ZStL8__ioinit
	add	x19, x19, :lo12:_ZStL8__ioinit
	mov	x0, x19
	bl	_ZNSt8ios_base4InitC1Ev
	ldp	x29, x30, [sp, #16]     // 16-byte Folded Reload
	adrp	x0, _ZNSt8ios_base4InitD1Ev
	adrp	x2, __dso_handle
	add	x0, x0, :lo12:_ZNSt8ios_base4InitD1Ev
	add	x2, x2, :lo12:__dso_handle
	mov	x1, x19
	ldr	x19, [sp], #32          // 8-byte Folded Reload
	b	__cxa_atexit
.Lfunc_end23:
	.size	_GLOBAL__sub_I_bitmapdecoding.cpp, .Lfunc_end23-_GLOBAL__sub_I_bitmapdecoding.cpp
	.cfi_endproc
                                        // -- End function
	.type	_ZStL8__ioinit,@object  // @_ZStL8__ioinit
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.hidden	__dso_handle
	.type	.L.str,@object          // @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"rb"
	.size	.L.str, 3

	.type	.L.str.1,@object        // @.str.1
.L.str.1:
	.asciz	"could not read the data"
	.size	.L.str.1, 24

	.type	.L.str.2,@object        // @.str.2
.L.str.2:
	.asciz	"can't open "
	.size	.L.str.2, 12

	.type	.L.str.3,@object        // @.str.3
.L.str.3:
	.asciz	"bogus\n."
	.size	.L.str.3, 8

	.type	.L.str.4,@object        // @.str.4
.L.str.4:
	.asciz	"matches = %u words = %zu 1-bit density %4.3f %% \n"
	.size	.L.str.4, 50

	.type	.L.str.5,@object        // @.str.5
.L.str.5:
	.asciz	"bytes per index = %4.3f \n"
	.size	.L.str.5, 26

	.type	.L.str.6,@object        // @.str.6
.L.str.6:
	.asciz	"instructions per cycle %4.2f, cycles per value set:  %4.3f, instructions per value set: %4.3f, cycles per word: %4.3f, instructions per word: %4.3f \n"
	.size	.L.str.6, 150

	.type	.L.str.7,@object        // @.str.7
.L.str.7:
	.asciz	" cycles per input byte %4.2f instructions per input byte %4.2f \n"
	.size	.L.str.7, 65

	.type	.L.str.8,@object        // @.str.8
.L.str.8:
	.asciz	"min: %8llu cycles, %8llu instructions, \t%8llu branch mis., %8llu cache ref., %8llu cache mis.\n"
	.size	.L.str.8, 95

	.type	.L.str.9,@object        // @.str.9
.L.str.9:
	.asciz	"avg: %8.1f cycles, %8.1f instructions, \t%8.1f branch mis., %8.1f cache ref., %8.1f cache mis.\n"
	.size	.L.str.9, 95

	.type	.L.str.11,@object       // @.str.11
.L.str.11:
	.asciz	"nfl.csv"
	.size	.L.str.11, 8

	.type	.L.str.17,@object       // @.str.17
.L.str.17:
	.asciz	"perf_event_open"
	.size	.L.str.17, 16

	.type	.L.str.18,@object       // @.str.18
.L.str.18:
	.asciz	"vector::_M_default_append"
	.size	.L.str.18, 26

	.type	.L.str.19,@object       // @.str.19
.L.str.19:
	.asciz	": "
	.size	.L.str.19, 3

	.type	.L.str.22,@object       // @.str.22
.L.str.22:
	.asciz	"basic_string::_M_construct null not valid"
	.size	.L.str.22, 42

	.type	.L.str.23,@object       // @.str.23
.L.str.23:
	.asciz	"basic_string::append"
	.size	.L.str.23, 21

	.type	.L.str.24,@object       // @.str.24
.L.str.24:
	.asciz	"ioctl(PERF_EVENT_IOC_RESET)"
	.size	.L.str.24, 28

	.type	.L.str.25,@object       // @.str.25
.L.str.25:
	.asciz	"ioctl(PERF_EVENT_IOC_ENABLE)"
	.size	.L.str.25, 29

	.type	.L.str.26,@object       // @.str.26
.L.str.26:
	.asciz	"ioctl(PERF_EVENT_IOC_DISABLE)"
	.size	.L.str.26, 30

	.section	.init_array,"aw",@init_array
	.p2align	3
	.xword	_GLOBAL__sub_I_bitmapdecoding.cpp
	.type	.Lstr,@object           // @str
	.section	.rodata.str1.1,"aMS",@progbits,1
.Lstr:
	.asciz	"fast_decoder:"
	.size	.Lstr, 14

	.type	.Lstr.30,@object        // @str.30
.Lstr.30:
	.asciz	"simdjson_decoder:"
	.size	.Lstr.30, 18

	.type	.Lstr.31,@object        // @str.31
.Lstr.31:
	.asciz	"basic_decoder:"
	.size	.Lstr.31, 15

	.type	.Lstr.32,@object        // @str.32
.Lstr.32:
	.asciz	"faster_decoder:"
	.size	.Lstr.32, 16

	.type	.Lstr.48,@object        // @str.48
.Lstr.48:
	.asciz	"Tests passed."
	.size	.Lstr.48, 14

	.type	.Lstr.52,@object        // @str.52
.Lstr.52:
	.asciz	"bug"
	.size	.Lstr.52, 4


	.ident	"clang version 8.0.0-4 (tags/RELEASE_800/final)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym __gxx_personality_v0
	.addrsig_sym _GLOBAL__sub_I_bitmapdecoding.cpp
	.addrsig_sym _Unwind_Resume
	.addrsig_sym _ZStL8__ioinit
	.addrsig_sym __dso_handle
	.addrsig_sym _ZTISt13runtime_error
	.addrsig_sym _ZSt4cout
	.addrsig_sym _ZSt4cerr
