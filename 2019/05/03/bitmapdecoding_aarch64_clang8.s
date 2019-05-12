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
	.globl	_Z16simdjson_decoderPjRjjm // -- Begin function _Z16simdjson_decoderPjRjjm
	.p2align	2
	.type	_Z16simdjson_decoderPjRjjm,@function
_Z16simdjson_decoderPjRjjm:             // @_Z16simdjson_decoderPjRjjm
// %bb.0:
	fmov	d0, x3
	ldr	w9, [x1]
	cnt	v0.8b, v0.8b
	uaddlv	h0, v0.8b
	fmov	w8, s0
	add	w8, w9, w8
	cbz	x3, .LBB5_2
.LBB5_1:                                // =>This Inner Loop Header: Depth=1
	rbit	x11, x3
	clz	x11, x11
	add	w11, w11, w2
	sub	x10, x3, #1             // =1
	str	w11, [x0, w9, uxtw #2]
	and	x10, x10, x3
	ldr	w11, [x1]
	sub	x9, x10, #1             // =1
	and	x9, x9, x10
	rbit	x10, x10
	clz	x10, x10
	add	w10, w10, w2
	add	w11, w11, #1            // =1
	str	w10, [x0, w11, uxtw #2]
	ldr	w11, [x1]
	sub	x10, x9, #1             // =1
	and	x10, x10, x9
	rbit	x9, x9
	clz	x9, x9
	add	w9, w9, w2
	add	w11, w11, #2            // =2
	str	w9, [x0, w11, uxtw #2]
	ldr	w11, [x1]
	sub	x9, x10, #1             // =1
	and	x9, x9, x10
	rbit	x10, x10
	clz	x10, x10
	add	w10, w10, w2
	add	w11, w11, #3            // =3
	str	w10, [x0, w11, uxtw #2]
	ldr	w11, [x1]
	sub	x10, x9, #1             // =1
	and	x10, x10, x9
	rbit	x9, x9
	clz	x9, x9
	add	w9, w9, w2
	add	w11, w11, #4            // =4
	str	w9, [x0, w11, uxtw #2]
	ldr	w11, [x1]
	sub	x9, x10, #1             // =1
	and	x9, x9, x10
	rbit	x10, x10
	clz	x10, x10
	add	w10, w10, w2
	add	w11, w11, #5            // =5
	str	w10, [x0, w11, uxtw #2]
	ldr	w10, [x1]
	rbit	x11, x9
	clz	x11, x11
	add	w11, w11, w2
	add	w10, w10, #6            // =6
	str	w11, [x0, w10, uxtw #2]
	sub	x10, x9, #1             // =1
	and	x9, x10, x9
	ldr	w10, [x1]
	rbit	x11, x9
	clz	x11, x11
	add	w11, w11, w2
	add	w10, w10, #7            // =7
	str	w11, [x0, w10, uxtw #2]
	ldr	w10, [x1]
	sub	x11, x9, #1             // =1
	ands	x3, x11, x9
	add	w9, w10, #8             // =8
	str	w9, [x1]
	b.ne	.LBB5_1
.LBB5_2:
	str	w8, [x1]
	ret
.Lfunc_end5:
	.size	_Z16simdjson_decoderPjRjjm, .Lfunc_end5-_Z16simdjson_decoderPjRjjm
                                        // -- End function
	.globl	_Z14faster_decoderPjRjjm // -- Begin function _Z14faster_decoderPjRjjm
	.p2align	2
	.type	_Z14faster_decoderPjRjjm,@function
_Z14faster_decoderPjRjjm:               // @_Z14faster_decoderPjRjjm
// %bb.0:
	cbz	x3, .LBB6_6
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
	b.lo	.LBB6_5
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
	b.lo	.LBB6_5
// %bb.3:
	ldr	w11, [x1]
	sub	x9, x10, #1             // =1
	and	x9, x9, x10
	add	w10, w11, #16           // =16
	str	w10, [x1]
.LBB6_4:                                // =>This Inner Loop Header: Depth=1
	rbit	x11, x9
	clz	x11, x11
	add	w11, w11, w2
	str	w11, [x0, w10, uxtw #2]
	ldr	w10, [x1]
	sub	x11, x9, #1             // =1
	ands	x9, x11, x9
	add	w10, w10, #1            // =1
	str	w10, [x1]
	b.ne	.LBB6_4
.LBB6_5:
	str	w8, [x1]
.LBB6_6:
	ret
.Lfunc_end6:
	.size	_Z14faster_decoderPjRjjm, .Lfunc_end6-_Z14faster_decoderPjRjjm
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
.LBB7_1:                                // =>This Inner Loop Header: Depth=1
	mov	x0, x20
	bl	_Znam
	ldr	x8, [x19, #8]
	str	x0, [x8, x21]
	ldp	x8, x9, [x19, #8]
	ldr	x8, [x8, x21]
	str	x8, [x9, x21]
	add	x21, x21, #8            // =8
	cmp	x21, #520               // =520
	b.ne	.LBB7_1
// %bb.2:
	ldp	x29, x30, [sp, #32]     // 16-byte Folded Reload
	ldp	x20, x19, [sp, #16]     // 16-byte Folded Reload
	ldr	x21, [sp], #48          // 8-byte Folded Reload
	ret
.Lfunc_end7:
	.size	_Z8init_bufP5buf_sm, .Lfunc_end7-_Z8init_bufP5buf_sm
	.cfi_endproc
                                        // -- End function
	.globl	_Z9reset_bufP5buf_s     // -- Begin function _Z9reset_bufP5buf_s
	.p2align	2
	.type	_Z9reset_bufP5buf_s,@function
_Z9reset_bufP5buf_s:                    // @_Z9reset_bufP5buf_s
// %bb.0:
	orr	w8, wzr, #0x8
	str	wzr, [x0]
.LBB8_1:                                // =>This Inner Loop Header: Depth=1
	ldp	x10, x9, [x0, #8]
	ldr	x9, [x9, x8]
	str	x9, [x10, x8]
	add	x8, x8, #8              // =8
	cmp	x8, #520                // =520
	b.ne	.LBB8_1
// %bb.2:
	ret
.Lfunc_end8:
	.size	_Z9reset_bufP5buf_s, .Lfunc_end8-_Z9reset_bufP5buf_s
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
.LBB9_1:                                // =>This Inner Loop Header: Depth=1
	ldr	x8, [x19, #16]
	ldr	x0, [x8, x20]
	cbz	x0, .LBB9_3
// %bb.2:                               //   in Loop: Header=BB9_1 Depth=1
	bl	_ZdaPv
.LBB9_3:                                //   in Loop: Header=BB9_1 Depth=1
	add	x20, x20, #8            // =8
	cmp	x20, #520               // =520
	b.ne	.LBB9_1
// %bb.4:
	ldr	x0, [x19, #8]
	cbz	x0, .LBB9_6
// %bb.5:
	bl	_ZdaPv
.LBB9_6:
	ldr	x0, [x19, #16]
	cbz	x0, .LBB9_8
// %bb.7:
	ldp	x29, x30, [sp, #16]     // 16-byte Folded Reload
	ldp	x20, x19, [sp], #32     // 16-byte Folded Reload
	b	_ZdaPv
.LBB9_8:
	ldp	x29, x30, [sp, #16]     // 16-byte Folded Reload
	ldp	x20, x19, [sp], #32     // 16-byte Folded Reload
	ret
.Lfunc_end9:
	.size	_Z11destroy_bufP5buf_s, .Lfunc_end9-_Z11destroy_bufP5buf_s
                                        // -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3               // -- Begin function _Z8fasttestPKcc
.LCPI10_0:
	.xword	4636737291354636288     // double 100
.LCPI10_1:
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
	cbz	x0, .LBB10_8
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
	b.eq	.LBB10_7
// %bb.2:
	mov	x9, xzr
	orr	w10, wzr, #0x1
.LBB10_3:                               // =>This Inner Loop Header: Depth=1
	ldrb	w11, [x21, x9]
	cmp	w11, w19, uxtb
	b.eq	.LBB10_5
// %bb.4:                               //   in Loop: Header=BB10_3 Depth=1
	cmp	w11, #31                // =31
	b.hi	.LBB10_6
.LBB10_5:                               //   in Loop: Header=BB10_3 Depth=1
	lsr	x11, x9, #3
	and	x11, x11, #0x1ffffffffffffff8
	ldr	x12, [x20, x11]
	lsl	x13, x10, x9
	orr	x12, x12, x13
	str	x12, [x20, x11]
.LBB10_6:                               //   in Loop: Header=BB10_3 Depth=1
	add	x9, x9, #1              // =1
	cmp	x8, x9
	b.ne	.LBB10_3
.LBB10_7:
	mov	x0, x21
	bl	_ZdaPv
	b	.LBB10_9
.LBB10_8:
	mov	x20, xzr
                                        // implicit-def: $x22
.LBB10_9:
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
	cbz	x0, .LBB10_17
// %bb.16:
	bl	_ZdlPv
.LBB10_17:
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
	b.eq	.LBB10_20
// %bb.19:
	mov	x0, x23
	mov	x1, x24
	bl	memmove
.LBB10_20:
	cbz	x24, .LBB10_22
// %bb.21:
	mov	x0, x24
	bl	_ZdlPv
.LBB10_22:
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
.LBB10_25:                              // =>This Inner Loop Header: Depth=1
.Ltmp29:
	mov	x0, x25
	bl	_Znam
.Ltmp30:
// %bb.26:                              //   in Loop: Header=BB10_25 Depth=1
	str	x0, [x23, x19]
	str	x0, [x24, x19]
	add	x19, x19, #8            // =8
	cmp	x19, #520               // =520
	b.ne	.LBB10_25
// %bb.27:
	mov	w19, wzr
	add	x26, x24, #8            // =8
	add	x27, x23, #8            // =8
	stur	x26, [x29, #-104]       // 8-byte Folded Spill
	str	x27, [sp, #24]          // 8-byte Folded Spill
.LBB10_28:                              // =>This Loop Header: Depth=1
                                        //     Child Loop BB10_31 Depth 2
                                        //     Child Loop BB10_36 Depth 2
                                        //     Child Loop BB10_38 Depth 2
                                        //     Child Loop BB10_40 Depth 2
                                        //     Child Loop BB10_42 Depth 2
                                        //     Child Loop BB10_44 Depth 2
                                        //     Child Loop BB10_46 Depth 2
                                        //     Child Loop BB10_48 Depth 2
                                        //     Child Loop BB10_50 Depth 2
                                        //     Child Loop BB10_52 Depth 2
                                        //     Child Loop BB10_54 Depth 2
                                        //     Child Loop BB10_56 Depth 2
                                        //     Child Loop BB10_58 Depth 2
                                        //     Child Loop BB10_60 Depth 2
                                        //     Child Loop BB10_62 Depth 2
                                        //     Child Loop BB10_64 Depth 2
                                        //     Child Loop BB10_66 Depth 2
                                        //     Child Loop BB10_68 Depth 2
                                        //     Child Loop BB10_70 Depth 2
                                        //     Child Loop BB10_72 Depth 2
                                        //     Child Loop BB10_74 Depth 2
                                        //     Child Loop BB10_76 Depth 2
                                        //     Child Loop BB10_78 Depth 2
                                        //       Child Loop BB10_79 Depth 3
                                        //     Child Loop BB10_82 Depth 2
                                        //       Child Loop BB10_83 Depth 3
                                        //     Child Loop BB10_86 Depth 2
                                        //       Child Loop BB10_87 Depth 3
                                        //     Child Loop BB10_90 Depth 2
                                        //       Child Loop BB10_91 Depth 3
                                        //     Child Loop BB10_94 Depth 2
                                        //       Child Loop BB10_95 Depth 3
                                        //     Child Loop BB10_98 Depth 2
                                        //       Child Loop BB10_99 Depth 3
                                        //     Child Loop BB10_102 Depth 2
                                        //       Child Loop BB10_103 Depth 3
                                        //     Child Loop BB10_106 Depth 2
                                        //       Child Loop BB10_107 Depth 3
                                        //     Child Loop BB10_110 Depth 2
                                        //       Child Loop BB10_111 Depth 3
                                        //     Child Loop BB10_114 Depth 2
                                        //       Child Loop BB10_115 Depth 3
                                        //     Child Loop BB10_118 Depth 2
                                        //       Child Loop BB10_119 Depth 3
                                        //     Child Loop BB10_122 Depth 2
                                        //       Child Loop BB10_123 Depth 3
                                        //     Child Loop BB10_126 Depth 2
                                        //       Child Loop BB10_127 Depth 3
                                        //     Child Loop BB10_130 Depth 2
                                        //       Child Loop BB10_131 Depth 3
                                        //     Child Loop BB10_134 Depth 2
                                        //       Child Loop BB10_135 Depth 3
                                        //     Child Loop BB10_138 Depth 2
                                        //       Child Loop BB10_139 Depth 3
                                        //     Child Loop BB10_142 Depth 2
                                        //       Child Loop BB10_143 Depth 3
                                        //     Child Loop BB10_146 Depth 2
                                        //       Child Loop BB10_147 Depth 3
                                        //     Child Loop BB10_150 Depth 2
                                        //       Child Loop BB10_151 Depth 3
                                        //     Child Loop BB10_154 Depth 2
                                        //       Child Loop BB10_155 Depth 3
                                        //     Child Loop BB10_158 Depth 2
                                        //       Child Loop BB10_159 Depth 3
                                        //     Child Loop BB10_162 Depth 2
                                        //       Child Loop BB10_163 Depth 3
                                        //     Child Loop BB10_166 Depth 2
                                        //       Child Loop BB10_167 Depth 3
                                        //     Child Loop BB10_170 Depth 2
                                        //       Child Loop BB10_171 Depth 3
                                        //     Child Loop BB10_174 Depth 2
                                        //       Child Loop BB10_175 Depth 3
                                        //     Child Loop BB10_178 Depth 2
                                        //       Child Loop BB10_179 Depth 3
                                        //     Child Loop BB10_182 Depth 2
                                        //       Child Loop BB10_183 Depth 3
                                        //     Child Loop BB10_186 Depth 2
                                        //       Child Loop BB10_187 Depth 3
                                        //     Child Loop BB10_190 Depth 2
                                        //       Child Loop BB10_191 Depth 3
                                        //     Child Loop BB10_194 Depth 2
                                        //       Child Loop BB10_195 Depth 3
                                        //     Child Loop BB10_198 Depth 2
                                        //       Child Loop BB10_199 Depth 3
                                        //     Child Loop BB10_202 Depth 2
                                        //       Child Loop BB10_203 Depth 3
                                        //     Child Loop BB10_206 Depth 2
                                        //       Child Loop BB10_207 Depth 3
                                        //     Child Loop BB10_210 Depth 2
                                        //       Child Loop BB10_211 Depth 3
                                        //     Child Loop BB10_214 Depth 2
                                        //       Child Loop BB10_215 Depth 3
                                        //     Child Loop BB10_218 Depth 2
                                        //       Child Loop BB10_219 Depth 3
                                        //     Child Loop BB10_222 Depth 2
                                        //       Child Loop BB10_223 Depth 3
                                        //     Child Loop BB10_226 Depth 2
                                        //       Child Loop BB10_227 Depth 3
                                        //     Child Loop BB10_230 Depth 2
                                        //       Child Loop BB10_231 Depth 3
                                        //     Child Loop BB10_234 Depth 2
                                        //       Child Loop BB10_235 Depth 3
                                        //     Child Loop BB10_238 Depth 2
                                        //       Child Loop BB10_239 Depth 3
                                        //     Child Loop BB10_242 Depth 2
                                        //       Child Loop BB10_243 Depth 3
                                        //     Child Loop BB10_246 Depth 2
                                        //       Child Loop BB10_247 Depth 3
.Ltmp32:
	add	x0, sp, #152            // =152
	bl	_ZN11LinuxEventsILi0EE5startEv
.Ltmp33:
// %bb.29:                              //   in Loop: Header=BB10_28 Depth=1
	orr	w2, wzr, #0x200
	mov	x0, x27
	mov	x1, x26
	bl	memcpy
	cbz	x22, .LBB10_34
// %bb.30:                              //   in Loop: Header=BB10_28 Depth=1
	mov	x8, xzr
	mov	w25, wzr
.LBB10_31:                              //   Parent Loop BB10_28 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldr	x9, [x20, x8, lsl #3]
	cbz	x9, .LBB10_33
// %bb.32:                              //   in Loop: Header=BB10_31 Depth=2
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
.LBB10_33:                              //   in Loop: Header=BB10_31 Depth=2
	add	x8, x8, #1              // =1
	cmp	x22, x8
	b.ne	.LBB10_31
	b	.LBB10_35
.LBB10_34:                              //   in Loop: Header=BB10_28 Depth=1
	mov	w25, wzr
.LBB10_35:                              //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x26]
	ldr	x9, [x27]
	cmp	x8, x9
	b.eq	.LBB10_37
.LBB10_36:                              //   Parent Loop BB10_28 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldr	x10, [x8]
	ldp	w11, w12, [x8, #8]
	add	x8, x8, #16             // =16
	cmp	x8, x9
	rbit	x10, x10
	clz	x10, x10
	add	w10, w11, w10
	str	w10, [x21, x12, lsl #2]
	b.ne	.LBB10_36
.LBB10_37:                              //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #16]
	ldr	x9, [x23, #16]
	cmp	x8, x9
	b.eq	.LBB10_39
.LBB10_38:                              //   Parent Loop BB10_28 Depth=1
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
	b.ne	.LBB10_38
.LBB10_39:                              //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #24]
	ldr	x9, [x23, #24]
	cmp	x8, x9
	b.eq	.LBB10_41
.LBB10_40:                              //   Parent Loop BB10_28 Depth=1
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
	b.ne	.LBB10_40
.LBB10_41:                              //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #32]
	ldr	x9, [x23, #32]
	cmp	x8, x9
	b.eq	.LBB10_43
.LBB10_42:                              //   Parent Loop BB10_28 Depth=1
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
	b.ne	.LBB10_42
.LBB10_43:                              //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #40]
	ldr	x9, [x23, #40]
	cmp	x8, x9
	b.eq	.LBB10_45
.LBB10_44:                              //   Parent Loop BB10_28 Depth=1
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
	b.ne	.LBB10_44
.LBB10_45:                              //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #48]
	ldr	x9, [x23, #48]
	cmp	x8, x9
	b.eq	.LBB10_47
.LBB10_46:                              //   Parent Loop BB10_28 Depth=1
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
	b.ne	.LBB10_46
.LBB10_47:                              //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #56]
	ldr	x9, [x23, #56]
	cmp	x8, x9
	b.eq	.LBB10_49
.LBB10_48:                              //   Parent Loop BB10_28 Depth=1
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
	b.ne	.LBB10_48
.LBB10_49:                              //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #64]
	ldr	x9, [x23, #64]
	cmp	x8, x9
	b.eq	.LBB10_51
.LBB10_50:                              //   Parent Loop BB10_28 Depth=1
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
	b.ne	.LBB10_50
.LBB10_51:                              //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #72]
	ldr	x9, [x23, #72]
	cmp	x8, x9
	b.eq	.LBB10_53
.LBB10_52:                              //   Parent Loop BB10_28 Depth=1
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
	b.ne	.LBB10_52
.LBB10_53:                              //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #80]
	ldr	x9, [x23, #80]
	cmp	x8, x9
	b.eq	.LBB10_55
.LBB10_54:                              //   Parent Loop BB10_28 Depth=1
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
	b.ne	.LBB10_54
.LBB10_55:                              //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #88]
	ldr	x9, [x23, #88]
	cmp	x8, x9
	b.eq	.LBB10_57
.LBB10_56:                              //   Parent Loop BB10_28 Depth=1
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
	b.ne	.LBB10_56
.LBB10_57:                              //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #96]
	ldr	x9, [x23, #96]
	cmp	x8, x9
	b.eq	.LBB10_59
.LBB10_58:                              //   Parent Loop BB10_28 Depth=1
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
	b.ne	.LBB10_58
.LBB10_59:                              //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #104]
	ldr	x9, [x23, #104]
	cmp	x8, x9
	b.eq	.LBB10_61
.LBB10_60:                              //   Parent Loop BB10_28 Depth=1
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
	b.ne	.LBB10_60
.LBB10_61:                              //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #112]
	ldr	x9, [x23, #112]
	cmp	x8, x9
	b.eq	.LBB10_63
.LBB10_62:                              //   Parent Loop BB10_28 Depth=1
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
	b.ne	.LBB10_62
.LBB10_63:                              //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #120]
	ldr	x9, [x23, #120]
	cmp	x8, x9
	b.eq	.LBB10_65
.LBB10_64:                              //   Parent Loop BB10_28 Depth=1
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
	b.ne	.LBB10_64
.LBB10_65:                              //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #128]
	ldr	x9, [x23, #128]
	cmp	x8, x9
	b.eq	.LBB10_67
.LBB10_66:                              //   Parent Loop BB10_28 Depth=1
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
	b.ne	.LBB10_66
.LBB10_67:                              //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #136]
	ldr	x9, [x23, #136]
	cmp	x8, x9
	b.eq	.LBB10_69
.LBB10_68:                              //   Parent Loop BB10_28 Depth=1
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
	b.ne	.LBB10_68
.LBB10_69:                              //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #144]
	ldr	x9, [x23, #144]
	cmp	x8, x9
	b.eq	.LBB10_71
.LBB10_70:                              //   Parent Loop BB10_28 Depth=1
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
	b.ne	.LBB10_70
.LBB10_71:                              //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #152]
	ldr	x9, [x23, #152]
	cmp	x8, x9
	b.eq	.LBB10_73
.LBB10_72:                              //   Parent Loop BB10_28 Depth=1
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
	b.ne	.LBB10_72
.LBB10_73:                              //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #160]
	ldr	x9, [x23, #160]
	cmp	x8, x9
	b.eq	.LBB10_75
.LBB10_74:                              //   Parent Loop BB10_28 Depth=1
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
	b.ne	.LBB10_74
.LBB10_75:                              //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #168]
	ldr	x9, [x23, #168]
	cmp	x8, x9
	b.eq	.LBB10_77
.LBB10_76:                              //   Parent Loop BB10_28 Depth=1
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
	b.ne	.LBB10_76
.LBB10_77:                              //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #176]
	ldr	x9, [x23, #176]
	cmp	x8, x9
	b.eq	.LBB10_81
.LBB10_78:                              //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_79 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_79:                              //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_78 Depth=2
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
	b.ne	.LBB10_79
// %bb.80:                              //   in Loop: Header=BB10_78 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_78
.LBB10_81:                              //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #184]
	ldr	x9, [x23, #184]
	cmp	x8, x9
	b.eq	.LBB10_85
.LBB10_82:                              //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_83 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_83:                              //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_82 Depth=2
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
	b.ne	.LBB10_83
// %bb.84:                              //   in Loop: Header=BB10_82 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_82
.LBB10_85:                              //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #192]
	ldr	x9, [x23, #192]
	cmp	x8, x9
	b.eq	.LBB10_89
.LBB10_86:                              //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_87 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_87:                              //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_86 Depth=2
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
	b.ne	.LBB10_87
// %bb.88:                              //   in Loop: Header=BB10_86 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_86
.LBB10_89:                              //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #200]
	ldr	x9, [x23, #200]
	cmp	x8, x9
	b.eq	.LBB10_93
.LBB10_90:                              //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_91 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_91:                              //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_90 Depth=2
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
	b.ne	.LBB10_91
// %bb.92:                              //   in Loop: Header=BB10_90 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_90
.LBB10_93:                              //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #208]
	ldr	x9, [x23, #208]
	cmp	x8, x9
	b.eq	.LBB10_97
.LBB10_94:                              //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_95 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_95:                              //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_94 Depth=2
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
	b.ne	.LBB10_95
// %bb.96:                              //   in Loop: Header=BB10_94 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_94
.LBB10_97:                              //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #216]
	ldr	x9, [x23, #216]
	cmp	x8, x9
	b.eq	.LBB10_101
.LBB10_98:                              //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_99 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_99:                              //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_98 Depth=2
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
	b.ne	.LBB10_99
// %bb.100:                             //   in Loop: Header=BB10_98 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_98
.LBB10_101:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #224]
	ldr	x9, [x23, #224]
	cmp	x8, x9
	b.eq	.LBB10_105
.LBB10_102:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_103 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_103:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_102 Depth=2
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
	b.ne	.LBB10_103
// %bb.104:                             //   in Loop: Header=BB10_102 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_102
.LBB10_105:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #232]
	ldr	x9, [x23, #232]
	cmp	x8, x9
	b.eq	.LBB10_109
.LBB10_106:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_107 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_107:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_106 Depth=2
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
	b.ne	.LBB10_107
// %bb.108:                             //   in Loop: Header=BB10_106 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_106
.LBB10_109:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #240]
	ldr	x9, [x23, #240]
	cmp	x8, x9
	b.eq	.LBB10_113
.LBB10_110:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_111 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_111:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_110 Depth=2
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
	b.ne	.LBB10_111
// %bb.112:                             //   in Loop: Header=BB10_110 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_110
.LBB10_113:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #248]
	ldr	x9, [x23, #248]
	cmp	x8, x9
	b.eq	.LBB10_117
.LBB10_114:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_115 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_115:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_114 Depth=2
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
	b.ne	.LBB10_115
// %bb.116:                             //   in Loop: Header=BB10_114 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_114
.LBB10_117:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #256]
	ldr	x9, [x23, #256]
	cmp	x8, x9
	b.eq	.LBB10_121
.LBB10_118:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_119 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_119:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_118 Depth=2
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
	b.ne	.LBB10_119
// %bb.120:                             //   in Loop: Header=BB10_118 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_118
.LBB10_121:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #264]
	ldr	x9, [x23, #264]
	cmp	x8, x9
	b.eq	.LBB10_125
.LBB10_122:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_123 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_123:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_122 Depth=2
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
	b.ne	.LBB10_123
// %bb.124:                             //   in Loop: Header=BB10_122 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_122
.LBB10_125:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #272]
	ldr	x9, [x23, #272]
	cmp	x8, x9
	b.eq	.LBB10_129
.LBB10_126:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_127 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_127:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_126 Depth=2
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
	b.ne	.LBB10_127
// %bb.128:                             //   in Loop: Header=BB10_126 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_126
.LBB10_129:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #280]
	ldr	x9, [x23, #280]
	cmp	x8, x9
	b.eq	.LBB10_133
.LBB10_130:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_131 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_131:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_130 Depth=2
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
	b.ne	.LBB10_131
// %bb.132:                             //   in Loop: Header=BB10_130 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_130
.LBB10_133:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #288]
	ldr	x9, [x23, #288]
	cmp	x8, x9
	b.eq	.LBB10_137
.LBB10_134:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_135 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_135:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_134 Depth=2
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
	b.ne	.LBB10_135
// %bb.136:                             //   in Loop: Header=BB10_134 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_134
.LBB10_137:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #296]
	ldr	x9, [x23, #296]
	cmp	x8, x9
	b.eq	.LBB10_141
.LBB10_138:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_139 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_139:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_138 Depth=2
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
	b.ne	.LBB10_139
// %bb.140:                             //   in Loop: Header=BB10_138 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_138
.LBB10_141:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #304]
	ldr	x9, [x23, #304]
	cmp	x8, x9
	b.eq	.LBB10_145
.LBB10_142:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_143 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_143:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_142 Depth=2
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
	b.ne	.LBB10_143
// %bb.144:                             //   in Loop: Header=BB10_142 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_142
.LBB10_145:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #312]
	ldr	x9, [x23, #312]
	cmp	x8, x9
	b.eq	.LBB10_149
.LBB10_146:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_147 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_147:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_146 Depth=2
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
	b.ne	.LBB10_147
// %bb.148:                             //   in Loop: Header=BB10_146 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_146
.LBB10_149:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #320]
	ldr	x9, [x23, #320]
	cmp	x8, x9
	b.eq	.LBB10_153
.LBB10_150:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_151 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_151:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_150 Depth=2
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
	b.ne	.LBB10_151
// %bb.152:                             //   in Loop: Header=BB10_150 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_150
.LBB10_153:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #328]
	ldr	x9, [x23, #328]
	cmp	x8, x9
	b.eq	.LBB10_157
.LBB10_154:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_155 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_155:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_154 Depth=2
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
	b.ne	.LBB10_155
// %bb.156:                             //   in Loop: Header=BB10_154 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_154
.LBB10_157:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #336]
	ldr	x9, [x23, #336]
	cmp	x8, x9
	b.eq	.LBB10_161
.LBB10_158:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_159 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_159:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_158 Depth=2
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
	b.ne	.LBB10_159
// %bb.160:                             //   in Loop: Header=BB10_158 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_158
.LBB10_161:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #344]
	ldr	x9, [x23, #344]
	cmp	x8, x9
	b.eq	.LBB10_165
.LBB10_162:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_163 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_163:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_162 Depth=2
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
	b.ne	.LBB10_163
// %bb.164:                             //   in Loop: Header=BB10_162 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_162
.LBB10_165:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #352]
	ldr	x9, [x23, #352]
	cmp	x8, x9
	b.eq	.LBB10_169
.LBB10_166:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_167 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_167:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_166 Depth=2
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
	b.ne	.LBB10_167
// %bb.168:                             //   in Loop: Header=BB10_166 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_166
.LBB10_169:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #360]
	ldr	x9, [x23, #360]
	cmp	x8, x9
	b.eq	.LBB10_173
.LBB10_170:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_171 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_171:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_170 Depth=2
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
	b.ne	.LBB10_171
// %bb.172:                             //   in Loop: Header=BB10_170 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_170
.LBB10_173:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #368]
	ldr	x9, [x23, #368]
	cmp	x8, x9
	b.eq	.LBB10_177
.LBB10_174:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_175 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_175:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_174 Depth=2
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
	b.ne	.LBB10_175
// %bb.176:                             //   in Loop: Header=BB10_174 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_174
.LBB10_177:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #376]
	ldr	x9, [x23, #376]
	cmp	x8, x9
	b.eq	.LBB10_181
.LBB10_178:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_179 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_179:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_178 Depth=2
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
	b.ne	.LBB10_179
// %bb.180:                             //   in Loop: Header=BB10_178 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_178
.LBB10_181:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #384]
	ldr	x9, [x23, #384]
	cmp	x8, x9
	b.eq	.LBB10_185
.LBB10_182:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_183 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_183:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_182 Depth=2
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
	b.ne	.LBB10_183
// %bb.184:                             //   in Loop: Header=BB10_182 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_182
.LBB10_185:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #392]
	ldr	x9, [x23, #392]
	cmp	x8, x9
	b.eq	.LBB10_189
.LBB10_186:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_187 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_187:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_186 Depth=2
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
	b.ne	.LBB10_187
// %bb.188:                             //   in Loop: Header=BB10_186 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_186
.LBB10_189:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #400]
	ldr	x9, [x23, #400]
	cmp	x8, x9
	b.eq	.LBB10_193
.LBB10_190:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_191 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_191:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_190 Depth=2
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
	b.ne	.LBB10_191
// %bb.192:                             //   in Loop: Header=BB10_190 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_190
.LBB10_193:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #408]
	ldr	x9, [x23, #408]
	cmp	x8, x9
	b.eq	.LBB10_197
.LBB10_194:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_195 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_195:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_194 Depth=2
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
	b.ne	.LBB10_195
// %bb.196:                             //   in Loop: Header=BB10_194 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_194
.LBB10_197:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #416]
	ldr	x9, [x23, #416]
	cmp	x8, x9
	b.eq	.LBB10_201
.LBB10_198:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_199 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_199:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_198 Depth=2
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
	b.ne	.LBB10_199
// %bb.200:                             //   in Loop: Header=BB10_198 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_198
.LBB10_201:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #424]
	ldr	x9, [x23, #424]
	cmp	x8, x9
	b.eq	.LBB10_205
.LBB10_202:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_203 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_203:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_202 Depth=2
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
	b.ne	.LBB10_203
// %bb.204:                             //   in Loop: Header=BB10_202 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_202
.LBB10_205:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #432]
	ldr	x9, [x23, #432]
	cmp	x8, x9
	b.eq	.LBB10_209
.LBB10_206:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_207 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_207:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_206 Depth=2
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
	b.ne	.LBB10_207
// %bb.208:                             //   in Loop: Header=BB10_206 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_206
.LBB10_209:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #440]
	ldr	x9, [x23, #440]
	cmp	x8, x9
	b.eq	.LBB10_213
.LBB10_210:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_211 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_211:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_210 Depth=2
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
	b.ne	.LBB10_211
// %bb.212:                             //   in Loop: Header=BB10_210 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_210
.LBB10_213:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #448]
	ldr	x9, [x23, #448]
	cmp	x8, x9
	b.eq	.LBB10_217
.LBB10_214:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_215 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_215:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_214 Depth=2
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
	b.ne	.LBB10_215
// %bb.216:                             //   in Loop: Header=BB10_214 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_214
.LBB10_217:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #456]
	ldr	x9, [x23, #456]
	cmp	x8, x9
	b.eq	.LBB10_221
.LBB10_218:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_219 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_219:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_218 Depth=2
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
	b.ne	.LBB10_219
// %bb.220:                             //   in Loop: Header=BB10_218 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_218
.LBB10_221:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #464]
	ldr	x9, [x23, #464]
	cmp	x8, x9
	b.eq	.LBB10_225
.LBB10_222:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_223 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_223:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_222 Depth=2
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
	b.ne	.LBB10_223
// %bb.224:                             //   in Loop: Header=BB10_222 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_222
.LBB10_225:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #472]
	ldr	x9, [x23, #472]
	cmp	x8, x9
	b.eq	.LBB10_229
.LBB10_226:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_227 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_227:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_226 Depth=2
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
	b.ne	.LBB10_227
// %bb.228:                             //   in Loop: Header=BB10_226 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_226
.LBB10_229:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #480]
	ldr	x9, [x23, #480]
	cmp	x8, x9
	b.eq	.LBB10_233
.LBB10_230:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_231 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_231:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_230 Depth=2
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
	b.ne	.LBB10_231
// %bb.232:                             //   in Loop: Header=BB10_230 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_230
.LBB10_233:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #488]
	ldr	x9, [x23, #488]
	cmp	x8, x9
	b.eq	.LBB10_237
.LBB10_234:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_235 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_235:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_234 Depth=2
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
	b.ne	.LBB10_235
// %bb.236:                             //   in Loop: Header=BB10_234 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_234
.LBB10_237:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #496]
	ldr	x9, [x23, #496]
	cmp	x8, x9
	b.eq	.LBB10_241
.LBB10_238:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_239 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_239:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_238 Depth=2
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
	b.ne	.LBB10_239
// %bb.240:                             //   in Loop: Header=BB10_238 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_238
.LBB10_241:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #504]
	ldr	x9, [x23, #504]
	cmp	x8, x9
	b.eq	.LBB10_245
.LBB10_242:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_243 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_243:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_242 Depth=2
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
	b.ne	.LBB10_243
// %bb.244:                             //   in Loop: Header=BB10_242 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_242
.LBB10_245:                             //   in Loop: Header=BB10_28 Depth=1
	ldr	x8, [x24, #512]
	ldr	x9, [x23, #512]
	cmp	x8, x9
	b.eq	.LBB10_249
.LBB10_246:                             //   Parent Loop BB10_28 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB10_247 Depth 3
	ldr	w12, [x8, #12]
	ldr	x11, [x8]
	mov	x10, xzr
	add	x12, x21, x12, lsl #2
.LBB10_247:                             //   Parent Loop BB10_28 Depth=1
                                        //     Parent Loop BB10_246 Depth=2
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
	b.ne	.LBB10_247
// %bb.248:                             //   in Loop: Header=BB10_246 Depth=2
	add	x8, x8, #16             // =16
	cmp	x8, x9
	b.ne	.LBB10_246
.LBB10_249:                             //   in Loop: Header=BB10_28 Depth=1
.Ltmp34:
	add	x0, sp, #152            // =152
	add	x1, sp, #104            // =104
	bl	_ZN11LinuxEventsILi0EE3endERSt6vectorIySaIyEE
.Ltmp35:
// %bb.250:                             //   in Loop: Header=BB10_28 Depth=1
	ldp	x28, x8, [sp, #88]
	cmp	x28, x8
	b.eq	.LBB10_255
// %bb.251:                             //   in Loop: Header=BB10_28 Depth=1
	ldp	x9, x8, [sp, #104]
	stp	xzr, xzr, [x28]
	str	xzr, [x28, #16]
	subs	x0, x8, x9
	asr	x26, x0, #3
	b.eq	.LBB10_256
// %bb.252:                             //   in Loop: Header=BB10_28 Depth=1
	lsr	x8, x26, #61
	cbnz	x8, .LBB10_347
// %bb.253:                             //   in Loop: Header=BB10_28 Depth=1
.Ltmp36:
	bl	_Znwm
.Ltmp37:
// %bb.254:                             //   in Loop: Header=BB10_28 Depth=1
	mov	x27, x0
	b	.LBB10_257
.LBB10_255:                             //   in Loop: Header=BB10_28 Depth=1
.Ltmp41:
	add	x0, sp, #80             // =80
	add	x2, sp, #104            // =104
	mov	x1, x28
	bl	_ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_
.Ltmp42:
	b	.LBB10_260
.LBB10_256:                             //   in Loop: Header=BB10_28 Depth=1
	mov	x27, xzr
.LBB10_257:                             //   in Loop: Header=BB10_28 Depth=1
	add	x8, x27, x26, lsl #3
	stp	x27, x0, [x28]
	str	x8, [x28, #16]
	ldp	x1, x8, [sp, #104]
	subs	x2, x8, x1
	asr	x26, x2, #3
	b.eq	.LBB10_259
// %bb.258:                             //   in Loop: Header=BB10_28 Depth=1
	bl	memmove
.LBB10_259:                             //   in Loop: Header=BB10_28 Depth=1
	add	x8, x27, x26, lsl #3
	str	x8, [x28, #8]
	ldr	x8, [sp, #88]
	ldur	x26, [x29, #-104]       // 8-byte Folded Reload
	ldr	x27, [sp, #24]          // 8-byte Folded Reload
	add	x8, x8, #24             // =24
	str	x8, [sp, #88]
.LBB10_260:                             //   in Loop: Header=BB10_28 Depth=1
	add	w19, w19, #1            // =1
	cmp	w19, #10                // =10
	b.lo	.LBB10_28
// %bb.261:
	orr	w19, wzr, #0x8
.LBB10_262:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x24, x19]
	cbz	x0, .LBB10_264
// %bb.263:                             //   in Loop: Header=BB10_262 Depth=1
	bl	_ZdaPv
.LBB10_264:                             //   in Loop: Header=BB10_262 Depth=1
	add	x19, x19, #8            // =8
	cmp	x19, #520               // =520
	b.ne	.LBB10_262
// %bb.265:
	mov	x0, x23
	bl	_ZdaPv
	mov	x0, x24
	bl	_ZdaPv
	ldr	w8, [x21]
	cbnz	w8, .LBB10_267
// %bb.266:
	adrp	x0, .L.str.3
	add	x0, x0, :lo12:.L.str.3
	bl	printf
.LBB10_267:
	adrp	x8, .LCPI10_0
	ldr	d0, [x8, :lo12:.LCPI10_0]
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
	adrp	x8, .LCPI10_1
	ldr	d0, [x8, :lo12:.LCPI10_1]
	ucvtf	d10, x22
	adrp	x0, .L.str.5
	add	x0, x0, :lo12:.L.str.5
	fmul	d0, d10, d0
	fdiv	d0, d0, d9
	bl	printf
	ldp	x0, x1, [sp, #80]
	subs	x8, x1, x0
	b.eq	.LBB10_271
// %bb.268:
	mov	x10, #-6148914691236517206
	asr	x9, x8, #3
	movk	x10, #43691
	mul	x9, x9, x10
	cmp	x9, x19
	b.hi	.LBB10_349
// %bb.269:
.Ltmp44:
	mov	x0, x8
	bl	_Znwm
.Ltmp45:
// %bb.270:
	mov	x23, x0
	ldp	x0, x1, [sp, #80]
	mov	x24, x23
	b	.LBB10_272
.LBB10_271:
	mov	x23, xzr
	mov	x24, xzr
.LBB10_272:
.Ltmp49:
	mov	x2, x24
	bl	_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_
.Ltmp50:
// %bb.273:
	mov	x25, x0
	subs	x28, x0, x24
	b.eq	.LBB10_279
// %bb.274:
	ldp	x8, x9, [x24]
	subs	x0, x9, x8
	b.eq	.LBB10_280
// %bb.275:
	asr	x8, x0, #3
	lsr	x8, x8, #61
	cbnz	x8, .LBB10_353
// %bb.276:
.Ltmp52:
	bl	_Znwm
.Ltmp53:
// %bb.277:
	ldp	x1, x8, [x24]
	mov	x26, x0
	subs	x2, x8, x1
	b.eq	.LBB10_281
// %bb.278:
	mov	x0, x26
	asr	x27, x2, #3
	bl	memmove
	mov	w8, wzr
	b	.LBB10_282
.LBB10_279:
	mov	x22, xzr
	cbnz	x24, .LBB10_311
	b	.LBB10_312
.LBB10_280:
	mov	x22, xzr
	mov	x26, xzr
	mov	x27, xzr
	orr	w8, wzr, #0x1
	b	.LBB10_283
.LBB10_281:
	mov	x27, xzr
	orr	w8, wzr, #0x1
.LBB10_282:
	mov	x22, x26
.LBB10_283:
	mov	x10, #-6148914691236517206
	asr	x9, x28, #3
	movk	x10, #43691
	mul	x9, x9, x10
	cmp	x9, #2                  // =2
	b.lo	.LBB10_306
// %bb.284:
	cmp	x27, #1                 // =1
	csinc	x12, x27, xzr, hi
	add	x10, x22, #16           // =16
	orr	w11, wzr, #0x1
	add	x13, x22, x12, lsl #3
	and	x14, x12, #0xfffffffffffffffc
	orr	w15, wzr, #0x18
	b	.LBB10_296
.LBB10_285:                             //   in Loop: Header=BB10_296 Depth=1
	add	x17, x16, #16           // =16
	mov	x18, x10
	mov	x0, x14
.LBB10_286:                             //   Parent Loop BB10_296 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldp	q1, q0, [x17, #-16]
	ldp	q3, q2, [x18, #-16]
	cmhi	v3.2d, v3.2d, v1.2d
	xtn	v3.2s, v3.2d
	fmov	w1, s3
	tbz	w1, #0, .LBB10_288
// %bb.287:                             //   in Loop: Header=BB10_286 Depth=2
	stur	d1, [x18, #-16]
.LBB10_288:                             //   in Loop: Header=BB10_286 Depth=2
	mov	w1, v3.s[1]
	tbz	w1, #0, .LBB10_290
// %bb.289:                             //   in Loop: Header=BB10_286 Depth=2
	sub	x1, x18, #8             // =8
	st1	{ v1.d }[1], [x1]
.LBB10_290:                             //   in Loop: Header=BB10_286 Depth=2
	cmhi	v1.2d, v2.2d, v0.2d
	xtn	v1.2s, v1.2d
	fmov	w1, s1
	tbz	w1, #0, .LBB10_292
// %bb.291:                             //   in Loop: Header=BB10_286 Depth=2
	str	d0, [x18]
.LBB10_292:                             //   in Loop: Header=BB10_286 Depth=2
	mov	w1, v1.s[1]
	tbz	w1, #0, .LBB10_294
// %bb.293:                             //   in Loop: Header=BB10_286 Depth=2
	add	x1, x18, #8             // =8
	st1	{ v0.d }[1], [x1]
.LBB10_294:                             //   in Loop: Header=BB10_286 Depth=2
	subs	x0, x0, #4              // =4
	add	x18, x18, #32           // =32
	add	x17, x17, #32           // =32
	b.ne	.LBB10_286
// %bb.295:                             //   in Loop: Header=BB10_296 Depth=1
	cmp	x12, x14
	mov	x17, x14
	b.ne	.LBB10_301
	b	.LBB10_304
.LBB10_296:                             // =>This Loop Header: Depth=1
                                        //     Child Loop BB10_286 Depth 2
                                        //     Child Loop BB10_301 Depth 2
	tbnz	w8, #0, .LBB10_304
// %bb.297:                             //   in Loop: Header=BB10_296 Depth=1
	mul	x16, x11, x15
	ldr	x16, [x24, x16]
	cmp	x12, #3                 // =3
	b.ls	.LBB10_300
// %bb.298:                             //   in Loop: Header=BB10_296 Depth=1
	add	x17, x16, x12, lsl #3
	cmp	x26, x17
	b.hs	.LBB10_285
// %bb.299:                             //   in Loop: Header=BB10_296 Depth=1
	cmp	x16, x13
	b.hs	.LBB10_285
.LBB10_300:                             //   in Loop: Header=BB10_296 Depth=1
	mov	x17, xzr
.LBB10_301:                             //   Parent Loop BB10_296 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x0, x17, #3
	ldr	x18, [x16, x0]
	ldr	x0, [x22, x0]
	cmp	x18, x0
	b.hs	.LBB10_303
// %bb.302:                             //   in Loop: Header=BB10_301 Depth=2
	str	x18, [x22, x17, lsl #3]
.LBB10_303:                             //   in Loop: Header=BB10_301 Depth=2
	add	x17, x17, #1            // =1
	cmp	x17, x27
	b.lo	.LBB10_301
.LBB10_304:                             //   in Loop: Header=BB10_296 Depth=1
	add	x11, x11, #1            // =1
	cmp	x11, x9
	b.ne	.LBB10_296
// %bb.305:
	cmp	x25, x24
	b.eq	.LBB10_310
.LBB10_306:
	mov	x26, x24
.LBB10_307:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x26]
	cbz	x0, .LBB10_309
// %bb.308:                             //   in Loop: Header=BB10_307 Depth=1
	bl	_ZdlPv
.LBB10_309:                             //   in Loop: Header=BB10_307 Depth=1
	add	x26, x26, #24           // =24
	cmp	x25, x26
	b.ne	.LBB10_307
.LBB10_310:
	cbz	x24, .LBB10_312
.LBB10_311:
	mov	x0, x23
	bl	_ZdlPv
.LBB10_312:
	ldp	x0, x1, [sp, #80]
	mov	x9, #-6148914691236517206
	movk	x9, #43691
	stp	xzr, xzr, [sp, #32]
	subs	x8, x1, x0
	asr	x10, x8, #3
	mul	x25, x10, x9
	str	xzr, [sp, #48]
	b.eq	.LBB10_316
// %bb.313:
	cmp	x25, x19
	b.hi	.LBB10_351
// %bb.314:
.Ltmp57:
	mov	x0, x8
	bl	_Znwm
.Ltmp58:
// %bb.315:
	mov	x23, x0
	ldp	x0, x1, [sp, #80]
	mov	x24, x23
	b	.LBB10_317
.LBB10_316:
	mov	x23, xzr
	mov	x24, xzr
.LBB10_317:
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
	b.eq	.LBB10_324
// %bb.320:
	mov	x19, x24
.LBB10_321:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x19]
	cbz	x0, .LBB10_323
// %bb.322:                             //   in Loop: Header=BB10_321 Depth=1
	bl	_ZdlPv
.LBB10_323:                             //   in Loop: Header=BB10_321 Depth=1
	add	x19, x19, #24           // =24
	cmp	x25, x19
	b.ne	.LBB10_321
.LBB10_324:
	cbz	x24, .LBB10_326
// %bb.325:
	mov	x0, x23
	bl	_ZdlPv
.LBB10_326:
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
	cbz	x20, .LBB10_328
// %bb.327:
	mov	x0, x20
	bl	_ZdaPv
.LBB10_328:
	mov	x0, x21
	bl	_ZdaPv
	cbz	x23, .LBB10_330
// %bb.329:
	mov	x0, x23
	bl	_ZdlPv
.LBB10_330:
	cbz	x22, .LBB10_332
// %bb.331:
	mov	x0, x22
	bl	_ZdlPv
.LBB10_332:
	ldp	x20, x19, [sp, #80]
	cmp	x20, x19
	b.eq	.LBB10_337
.LBB10_333:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x20]
	cbz	x0, .LBB10_335
// %bb.334:                             //   in Loop: Header=BB10_333 Depth=1
	bl	_ZdlPv
.LBB10_335:                             //   in Loop: Header=BB10_333 Depth=1
	add	x20, x20, #24           // =24
	cmp	x19, x20
	b.ne	.LBB10_333
// %bb.336:
	ldr	x20, [sp, #80]
.LBB10_337:
	cbz	x20, .LBB10_339
// %bb.338:
	mov	x0, x20
	bl	_ZdlPv
.LBB10_339:
	ldr	x0, [sp, #104]
	cbz	x0, .LBB10_341
// %bb.340:
	bl	_ZdlPv
.LBB10_341:
	ldr	w0, [sp, #152]
.Ltmp71:
	bl	close
.Ltmp72:
// %bb.342:
	ldr	x0, [sp, #304]
	cbz	x0, .LBB10_344
// %bb.343:
	bl	_ZdlPv
.LBB10_344:
	ldr	x0, [sp, #280]
	cbz	x0, .LBB10_346
// %bb.345:
	bl	_ZdlPv
.LBB10_346:
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
.LBB10_347:
.Ltmp38:
	bl	_ZSt17__throw_bad_allocv
.Ltmp39:
// %bb.348:
.LBB10_349:
.Ltmp46:
	bl	_ZSt17__throw_bad_allocv
.Ltmp47:
// %bb.350:
.LBB10_351:
.Ltmp59:
	bl	_ZSt17__throw_bad_allocv
.Ltmp60:
// %bb.352:
.LBB10_353:
.Ltmp54:
	bl	_ZSt17__throw_bad_allocv
.Ltmp55:
// %bb.354:
.LBB10_355:
.Ltmp40:
	b	.LBB10_390
.LBB10_356:
.Ltmp56:
	mov	x20, x0
	mov	x19, x24
.LBB10_357:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x19]
	cbz	x0, .LBB10_359
// %bb.358:                             //   in Loop: Header=BB10_357 Depth=1
	bl	_ZdlPv
.LBB10_359:                             //   in Loop: Header=BB10_357 Depth=1
	add	x19, x19, #24           // =24
	cmp	x25, x19
	b.ne	.LBB10_357
// %bb.360:
	cbnz	x24, .LBB10_376
	b	.LBB10_391
.LBB10_361:
.Ltmp61:
	mov	x20, x0
	cbnz	x22, .LBB10_374
	b	.LBB10_391
.LBB10_362:
.Ltmp48:
	b	.LBB10_390
.LBB10_363:
.Ltmp73:
.LBB10_364:
	add	x20, sp, #152           // =152
	mov	x19, x0
	add	x0, x20, #152           // =152
	bl	_ZNSt6vectorImSaImEED2Ev
	add	x0, x20, #128           // =128
	bl	_ZNSt6vectorImSaImEED2Ev
	mov	x0, x19
	bl	__clang_call_terminate
.LBB10_365:
.Ltmp67:
	cmp	x24, x25
	mov	x20, x0
	b.eq	.LBB10_370
// %bb.366:
	mov	x19, x24
.LBB10_367:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x19]
	cbz	x0, .LBB10_369
// %bb.368:                             //   in Loop: Header=BB10_367 Depth=1
	bl	_ZdlPv
.LBB10_369:                             //   in Loop: Header=BB10_367 Depth=1
	add	x19, x19, #24           // =24
	cmp	x25, x19
	b.ne	.LBB10_367
.LBB10_370:
	cbnz	x24, .LBB10_372
	b	.LBB10_373
.LBB10_371:
.Ltmp64:
	mov	x20, x0
	cbz	x24, .LBB10_373
.LBB10_372:
	mov	x0, x23
	bl	_ZdlPv
.LBB10_373:
	cbz	x22, .LBB10_391
.LBB10_374:
	mov	x0, x22
	b	.LBB10_377
.LBB10_375:
.Ltmp51:
	mov	x20, x0
	cbz	x24, .LBB10_391
.LBB10_376:
	mov	x0, x23
.LBB10_377:
	bl	_ZdlPv
	b	.LBB10_391
.LBB10_378:
.Ltmp23:
	mov	x20, x0
	b	.LBB10_392
.LBB10_379:
.Ltmp20:
	ldr	x8, [sp, #128]
	mov	x20, x0
	cbz	x8, .LBB10_406
// %bb.380:
	mov	x0, x8
	b	.LBB10_405
.LBB10_381:
.Ltmp17:
	b	.LBB10_385
.LBB10_382:
.Ltmp14:
	mov	x20, x0
	mov	x25, x23
	b	.LBB10_406
.LBB10_383:
.Ltmp11:
	mov	x20, x0
	mov	x25, x24
	b	.LBB10_406
.LBB10_384:
.Ltmp8:
.LBB10_385:
	mov	x20, x0
	b	.LBB10_406
.LBB10_386:
.Ltmp5:
	bl	_Unwind_Resume
.LBB10_387:
.Ltmp28:
	b	.LBB10_390
.LBB10_388:
.Ltmp31:
	b	.LBB10_390
.LBB10_389:
.Ltmp43:
.LBB10_390:
	mov	x20, x0
.LBB10_391:
	ldr	x25, [sp, #16]          // 8-byte Folded Reload
.LBB10_392:
	ldp	x21, x19, [sp, #80]
	cmp	x21, x19
	b.eq	.LBB10_397
.LBB10_393:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x21]
	cbz	x0, .LBB10_395
// %bb.394:                             //   in Loop: Header=BB10_393 Depth=1
	bl	_ZdlPv
.LBB10_395:                             //   in Loop: Header=BB10_393 Depth=1
	add	x21, x21, #24           // =24
	cmp	x19, x21
	b.ne	.LBB10_393
// %bb.396:
	ldr	x21, [sp, #80]
.LBB10_397:
	cbz	x21, .LBB10_399
// %bb.398:
	mov	x0, x21
	bl	_ZdlPv
.LBB10_399:
	ldr	x0, [sp, #104]
	cbz	x0, .LBB10_401
// %bb.400:
	bl	_ZdlPv
.LBB10_401:
	ldr	w0, [sp, #152]
.Ltmp68:
	bl	close
.Ltmp69:
// %bb.402:
	ldr	x0, [sp, #304]
	cbz	x0, .LBB10_404
// %bb.403:
	bl	_ZdlPv
.LBB10_404:
	ldr	x0, [sp, #280]
	cbz	x0, .LBB10_406
.LBB10_405:
	bl	_ZdlPv
.LBB10_406:
	mov	x0, x25
	bl	_ZdlPv
	mov	x0, x20
	bl	_Unwind_Resume
.LBB10_407:
.Ltmp70:
	b	.LBB10_364
.Lfunc_end10:
	.size	_Z8fasttestPKcc, .Lfunc_end10-_Z8fasttestPKcc
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table10:
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
	.uleb128 .Lfunc_end10-.Ltmp69   //   Call between .Ltmp69 and .Lfunc_end10
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
.LCPI11_0:
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
	adrp	x9, .LCPI11_0
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
	ldr	q0, [x9, :lo12:.LCPI11_0]
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
	b.eq	.LBB11_3
// %bb.1:
.Ltmp74:
	mov	x0, x21
	bl	_ZNSt6vectorImSaImEE17_M_default_appendEm
.Ltmp75:
// %bb.2:
	ldp	x24, x25, [x23]
.LBB11_3:
	cmp	x24, x25
	b.eq	.LBB11_13
// %bb.4:
	add	x8, sp, #16             // =16
	adrp	x27, .L.str.17
	mov	w19, wzr
	mov	w23, #-1
	add	x27, x27, :lo12:.L.str.17
	add	x26, x8, #16            // =16
.LBB11_5:                               // =>This Inner Loop Header: Depth=1
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
	b.ne	.LBB11_10
// %bb.6:                               //   in Loop: Header=BB11_5 Depth=1
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
// %bb.7:                               //   in Loop: Header=BB11_5 Depth=1
	ldr	x0, [sp, #16]
	cmp	x0, x26
	b.eq	.LBB11_9
// %bb.8:                               //   in Loop: Header=BB11_5 Depth=1
	bl	_ZdlPv
.LBB11_9:                               //   in Loop: Header=BB11_5 Depth=1
	ldr	w0, [x20]
.LBB11_10:                              //   in Loop: Header=BB11_5 Depth=1
	ldr	x8, [x21]
	mov	w1, #9223
	movk	w1, #32776, lsl #16
	add	w28, w19, #1            // =1
	add	x2, x8, w19, uxtw #3
                                        // kill: def $w0 killed $w0 killed $x0
	bl	ioctl
	cmn	w23, #1                 // =1
	b.ne	.LBB11_12
// %bb.11:                              //   in Loop: Header=BB11_5 Depth=1
	ldr	w23, [x20]
.LBB11_12:                              //   in Loop: Header=BB11_5 Depth=1
	add	x24, x24, #4            // =4
	cmp	x25, x24
	mov	w19, w28
	b.ne	.LBB11_5
.LBB11_13:
	ldrsw	x11, [x20, #120]
	ldp	x10, x8, [x20, #128]
	orr	w9, wzr, #0x1
	bfi	x9, x11, #1, #63
	sub	x11, x8, x10
	asr	x11, x11, #3
	cmp	x11, x9
	b.hs	.LBB11_15
// %bb.14:
	sub	x1, x9, x11
.Ltmp80:
	ldr	x0, [sp, #8]            // 8-byte Folded Reload
	bl	_ZNSt6vectorImSaImEE17_M_default_appendEm
.Ltmp81:
	b	.LBB11_18
.LBB11_15:
	b.ls	.LBB11_18
// %bb.16:
	add	x9, x10, x9, lsl #3
	cmp	x9, x8
	b.eq	.LBB11_18
// %bb.17:
	str	x9, [x20, #136]
.LBB11_18:
	ldp	x29, x30, [sp, #128]    // 16-byte Folded Reload
	ldp	x20, x19, [sp, #112]    // 16-byte Folded Reload
	ldp	x22, x21, [sp, #96]     // 16-byte Folded Reload
	ldp	x24, x23, [sp, #80]     // 16-byte Folded Reload
	ldp	x26, x25, [sp, #64]     // 16-byte Folded Reload
	ldp	x28, x27, [sp, #48]     // 16-byte Folded Reload
	add	sp, sp, #144            // =144
	ret
.LBB11_19:
.Ltmp82:
	b	.LBB11_21
.LBB11_20:
.Ltmp76:
.LBB11_21:
	mov	x20, x0
	ldr	x0, [x21]
	cbnz	x0, .LBB11_25
	b	.LBB11_26
.LBB11_22:
.Ltmp79:
	ldr	x8, [sp, #16]
	mov	x20, x0
	cmp	x8, x26
	b.eq	.LBB11_24
// %bb.23:
	mov	x0, x8
	bl	_ZdlPv
.LBB11_24:
	ldr	x0, [x21]
	cbz	x0, .LBB11_26
.LBB11_25:
	bl	_ZdlPv
.LBB11_26:
	ldr	x8, [sp, #8]            // 8-byte Folded Reload
	ldr	x0, [x8]
	cbz	x0, .LBB11_28
// %bb.27:
	bl	_ZdlPv
.LBB11_28:
	mov	x0, x20
	bl	_Unwind_Resume
.Lfunc_end11:
	.size	_ZN11LinuxEventsILi0EEC2ESt6vectorIiSaIiEE, .Lfunc_end11-_ZN11LinuxEventsILi0EEC2ESt6vectorIiSaIiEE
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table11:
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
	.uleb128 .Lfunc_end11-.Ltmp81   //   Call between .Ltmp81 and .Lfunc_end11
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
	b.ne	.LBB12_5
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
	b.eq	.LBB12_5
// %bb.4:
	bl	_ZdlPv
.LBB12_5:
	ldr	w0, [x19]
	mov	w1, #9216
	orr	w2, wzr, #0x1
	bl	ioctl
	cmn	w0, #1                  // =1
	b.ne	.LBB12_10
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
	b.eq	.LBB12_10
// %bb.9:
	bl	_ZdlPv
.LBB12_10:
	ldp	x29, x30, [sp, #64]     // 16-byte Folded Reload
	ldp	x20, x19, [sp, #48]     // 16-byte Folded Reload
	add	sp, sp, #80             // =80
	ret
.LBB12_11:
.Ltmp94:
	b	.LBB12_14
.LBB12_12:
.Ltmp91:
	bl	_Unwind_Resume
.LBB12_13:
.Ltmp88:
.LBB12_14:
	ldr	x8, [sp, #8]
	mov	x19, x0
	cmp	x8, x20
	b.eq	.LBB12_16
// %bb.15:
	mov	x0, x8
	bl	_ZdlPv
.LBB12_16:
	mov	x0, x19
	bl	_Unwind_Resume
.LBB12_17:
.Ltmp85:
	bl	_Unwind_Resume
.Lfunc_end12:
	.size	_ZN11LinuxEventsILi0EE5startEv, .Lfunc_end12-_ZN11LinuxEventsILi0EE5startEv
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table12:
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
	.uleb128 .Lfunc_end12-.Ltmp93   //   Call between .Ltmp93 and .Lfunc_end12
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
	b.ne	.LBB13_5
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
	b.eq	.LBB13_5
// %bb.4:
	bl	_ZdlPv
.LBB13_5:
	ldp	x1, x8, [x20, #128]
	ldr	w0, [x20]
	sub	x2, x8, x1
	bl	read
	cmn	x0, #1                  // =1
	b.ne	.LBB13_9
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
	b.eq	.LBB13_9
// %bb.8:
	bl	_ZdlPv
.LBB13_9:
	ldp	x8, x9, [x20, #128]
	sub	x9, x9, x8
	asr	x9, x9, #3
	cmp	x9, #2                  // =2
	b.lo	.LBB13_12
// %bb.10:
	ldr	x10, [x19]
	orr	w11, wzr, #0x3
	orr	w12, wzr, #0x1
.LBB13_11:                              // =>This Inner Loop Header: Depth=1
	ldr	x14, [x8, x12, lsl #3]
	sub	w13, w11, #2            // =2
	mov	w12, w11
	lsr	w13, w13, #1
	cmp	x9, x12
	add	w11, w11, #2            // =2
	str	x14, [x10, w13, uxtw #3]
	b.hi	.LBB13_11
.LBB13_12:
	ldp	x29, x30, [sp, #64]     // 16-byte Folded Reload
	ldp	x20, x19, [sp, #48]     // 16-byte Folded Reload
	ldr	x21, [sp, #32]          // 8-byte Folded Reload
	add	sp, sp, #80             // =80
	ret
.LBB13_13:
.Ltmp103:
	b	.LBB13_15
.LBB13_14:
.Ltmp100:
.LBB13_15:
	ldr	x8, [sp]
	mov	x19, x0
	cmp	x8, x21
	b.eq	.LBB13_17
// %bb.16:
	mov	x0, x8
	bl	_ZdlPv
.LBB13_17:
	mov	x0, x19
	bl	_Unwind_Resume
.LBB13_18:
.Ltmp97:
	bl	_Unwind_Resume
.Lfunc_end13:
	.size	_ZN11LinuxEventsILi0EE3endERSt6vectorIySaIyEE, .Lfunc_end13-_ZN11LinuxEventsILi0EE3endERSt6vectorIySaIyEE
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table13:
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
	.uleb128 .Lfunc_end13-.Ltmp102  //   Call between .Ltmp102 and .Lfunc_end13
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
.Lcst_end4:
	.p2align	2
                                        // -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3               // -- Begin function main
.LCPI14_0:
	.xword	4636737291354636288     // double 100
.LCPI14_1:
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
	str	d10, [sp, #-128]!       // 8-byte Folded Spill
	stp	d9, d8, [sp, #16]       // 16-byte Folded Spill
	stp	x28, x27, [sp, #32]     // 16-byte Folded Spill
	stp	x26, x25, [sp, #48]     // 16-byte Folded Spill
	stp	x24, x23, [sp, #64]     // 16-byte Folded Spill
	stp	x22, x21, [sp, #80]     // 16-byte Folded Spill
	stp	x20, x19, [sp, #96]     // 16-byte Folded Spill
	stp	x29, x30, [sp, #112]    // 16-byte Folded Spill
	add	x29, sp, #112           // =112
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
	.cfi_offset b10, -128
	cmp	w0, #2                  // =2
	b.lt	.LBB14_2
// %bb.1:
	adrp	x0, .L.str.11
	add	x0, x0, :lo12:.L.str.11
	mov	w1, #44
	bl	_Z8fasttestPKcc
.LBB14_2:
	adrp	x0, .Lstr
	add	x0, x0, :lo12:.Lstr
	bl	puts
	mov	x20, xzr
	orr	w21, wzr, #0x1
	add	x22, sp, #8             // =8
	add	x23, sp, #264           // =264
.LBB14_3:                               // =>This Loop Header: Depth=1
                                        //     Child Loop BB14_4 Depth 2
                                        //     Child Loop BB14_8 Depth 2
	lsl	x19, x21, x20
	add	x0, sp, #8              // =8
	sub	x1, x29, #144           // =144
	mov	w2, wzr
	mov	x3, x19
	stur	wzr, [x29, #-144]
	bl	_Z14faster_decoderPjRjjm
	mov	w8, wzr
.LBB14_4:                               //   Parent Loop BB14_3 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	rbit	x9, x19
	sub	x10, x19, #1            // =1
	clz	x9, x9
	ands	x19, x10, x19
	str	w9, [x23, w8, uxtw #2]
	add	w8, w8, #1              // =1
	b.ne	.LBB14_4
// %bb.5:                               //   in Loop: Header=BB14_3 Depth=1
	ldur	w9, [x29, #-144]
	cmp	w9, w8
	b.ne	.LBB14_22
// %bb.6:                               //   in Loop: Header=BB14_3 Depth=1
	cbz	w8, .LBB14_10
// %bb.7:                               //   in Loop: Header=BB14_3 Depth=1
	mov	x9, xzr
	mov	w8, w8
.LBB14_8:                               //   Parent Loop BB14_3 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x10, x9, #2
	ldr	w11, [x22, x10]
	ldr	w10, [x23, x10]
	cmp	w11, w10
	b.ne	.LBB14_22
// %bb.9:                               //   in Loop: Header=BB14_8 Depth=2
	add	x9, x9, #1              // =1
	cmp	x9, x8
	b.lo	.LBB14_8
.LBB14_10:                              //   in Loop: Header=BB14_3 Depth=1
	add	x20, x20, #1            // =1
	cmp	x20, #64                // =64
	b.lo	.LBB14_3
// %bb.11:
	mov	w22, #16960
	mov	x20, xzr
	add	x21, sp, #8             // =8
	movk	w22, #15, lsl #16
	add	x23, sp, #264           // =264
.LBB14_12:                              // =>This Loop Header: Depth=1
                                        //     Child Loop BB14_14 Depth 2
                                        //     Child Loop BB14_18 Depth 2
	bl	rand
	mov	w19, w0
	bl	rand
                                        // kill: def $w0 killed $w0 def $x0
	sxtw	x8, w0
	orr	x19, x8, x19, lsl #32
	add	x0, sp, #8              // =8
	sub	x1, x29, #144           // =144
	mov	w2, wzr
	mov	x3, x19
	stur	wzr, [x29, #-144]
	bl	_Z14faster_decoderPjRjjm
	fmov	d0, x19
	cnt	v0.8b, v0.8b
	uaddlv	h0, v0.8b
	fmov	w8, s0
	cbz	w8, .LBB14_15
// %bb.13:                              //   in Loop: Header=BB14_12 Depth=1
	add	x9, sp, #264            // =264
	mov	x10, x8
.LBB14_14:                              //   Parent Loop BB14_12 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	rbit	x11, x19
	sub	x12, x19, #1            // =1
	clz	x11, x11
	subs	x10, x10, #1            // =1
	str	w11, [x9], #4
	and	x19, x12, x19
	b.ne	.LBB14_14
.LBB14_15:                              //   in Loop: Header=BB14_12 Depth=1
	ldur	w9, [x29, #-144]
	cmp	w9, w8
	b.ne	.LBB14_22
// %bb.16:                              //   in Loop: Header=BB14_12 Depth=1
	cbz	w8, .LBB14_20
// %bb.17:                              //   in Loop: Header=BB14_12 Depth=1
	mov	x9, xzr
.LBB14_18:                              //   Parent Loop BB14_12 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x10, x9, #2
	ldr	w11, [x21, x10]
	ldr	w10, [x23, x10]
	cmp	w11, w10
	b.ne	.LBB14_22
// %bb.19:                              //   in Loop: Header=BB14_18 Depth=2
	add	x9, x9, #1              // =1
	cmp	x9, x8
	b.lo	.LBB14_18
.LBB14_20:                              //   in Loop: Header=BB14_12 Depth=1
	add	x20, x20, #1            // =1
	cmp	x20, x22
	b.lo	.LBB14_12
// %bb.21:
	adrp	x0, .Lstr.48
	add	x0, x0, :lo12:.Lstr.48
	b	.LBB14_23
.LBB14_22:
	adrp	x0, .Lstr.52
	add	x0, x0, :lo12:.Lstr.52
.LBB14_23:
	bl	puts
	adrp	x0, .L.str.11
	add	x0, x0, :lo12:.L.str.11
	add	x1, sp, #264            // =264
	bl	_Z14load_text_filePKcPm
	cbz	x0, .LBB14_31
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
	b.eq	.LBB14_30
// %bb.25:
	mov	x9, xzr
	orr	w10, wzr, #0x1
.LBB14_26:                              // =>This Inner Loop Header: Depth=1
	ldrb	w11, [x19, x9]
	cmp	w11, #44                // =44
	b.eq	.LBB14_28
// %bb.27:                              //   in Loop: Header=BB14_26 Depth=1
	cmp	w11, #31                // =31
	b.hi	.LBB14_29
.LBB14_28:                              //   in Loop: Header=BB14_26 Depth=1
	lsr	x11, x9, #3
	and	x11, x11, #0x1ffffffffffffff8
	ldr	x12, [x20, x11]
	lsl	x13, x10, x9
	orr	x12, x12, x13
	str	x12, [x20, x11]
.LBB14_29:                              //   in Loop: Header=BB14_26 Depth=1
	add	x9, x9, #1              // =1
	cmp	x8, x9
	b.ne	.LBB14_26
.LBB14_30:
	mov	x0, x19
	bl	_ZdaPv
	b	.LBB14_32
.LBB14_31:
	mov	x20, xzr
                                        // implicit-def: $x22
.LBB14_32:
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
	cbz	x0, .LBB14_40
// %bb.39:
	bl	_ZdlPv
.LBB14_40:
	stp	xzr, xzr, [x29, #-136]
	stp	xzr, xzr, [x29, #-152]
	stp	xzr, xzr, [x29, #-168]
.Ltmp122:
	mov	w0, #40
	bl	_Znwm
.Ltmp123:
// %bb.41:
	ldp	x24, x8, [x29, #-144]
	mov	x23, x0
	movi	v0.2d, #0000000000000000
	str	xzr, [x0, #32]
	subs	x2, x8, x24
	stp	q0, q0, [x0]
	b.eq	.LBB14_43
// %bb.42:
	mov	x0, x23
	mov	x1, x24
	bl	memmove
.LBB14_43:
	cbz	x24, .LBB14_45
// %bb.44:
	mov	x0, x24
	bl	_ZdlPv
.LBB14_45:
	mov	w26, wzr
	add	x8, x23, #40            // =40
	stur	x8, [x29, #-128]
	stp	x23, x8, [x29, #-144]
.LBB14_46:                              // =>This Loop Header: Depth=1
                                        //     Child Loop BB14_49 Depth 2
                                        //       Child Loop BB14_54 Depth 3
.Ltmp125:
	add	x0, sp, #264            // =264
	bl	_ZN11LinuxEventsILi0EE5startEv
.Ltmp126:
// %bb.47:                              //   in Loop: Header=BB14_46 Depth=1
	cbz	x22, .LBB14_56
// %bb.48:                              //   in Loop: Header=BB14_46 Depth=1
	mov	x8, xzr
	mov	w9, wzr
.LBB14_49:                              //   Parent Loop BB14_46 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB14_54 Depth 3
	ldr	x11, [x20, x8, lsl #3]
	fmov	d0, x11
	cnt	v0.8b, v0.8b
	uaddlv	h0, v0.8b
	fmov	w10, s0
	cbz	x11, .LBB14_51
// %bb.50:                              //   in Loop: Header=BB14_49 Depth=2
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
.LBB14_51:                              //   in Loop: Header=BB14_49 Depth=2
	cmp	w10, #5                 // =5
	add	w23, w9, w10
	b.lo	.LBB14_55
// %bb.52:                              //   in Loop: Header=BB14_49 Depth=2
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
	b.lo	.LBB14_55
// %bb.53:                              //   in Loop: Header=BB14_49 Depth=2
	sub	x10, x11, #1            // =1
	and	x10, x10, x11
	add	w9, w9, #8              // =8
.LBB14_54:                              //   Parent Loop BB14_46 Depth=1
                                        //     Parent Loop BB14_49 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	rbit	x11, x10
	clz	x11, x11
	sub	x12, x10, #1            // =1
	add	w11, w11, w8
	ands	x10, x12, x10
	str	w11, [x21, w9, uxtw #2]
	add	w9, w9, #1              // =1
	b.ne	.LBB14_54
.LBB14_55:                              //   in Loop: Header=BB14_49 Depth=2
	add	x8, x8, #1              // =1
	cmp	x8, x22
	mov	w9, w23
	b.ne	.LBB14_49
	b	.LBB14_57
.LBB14_56:                              //   in Loop: Header=BB14_46 Depth=1
	mov	w23, wzr
.LBB14_57:                              //   in Loop: Header=BB14_46 Depth=1
.Ltmp127:
	add	x0, sp, #264            // =264
	sub	x1, x29, #144           // =144
	bl	_ZN11LinuxEventsILi0EE3endERSt6vectorIySaIyEE
.Ltmp128:
// %bb.58:                              //   in Loop: Header=BB14_46 Depth=1
	ldp	x24, x8, [x29, #-160]
	cmp	x24, x8
	b.eq	.LBB14_63
// %bb.59:                              //   in Loop: Header=BB14_46 Depth=1
	ldp	x9, x8, [x29, #-144]
	stp	xzr, xzr, [x24]
	str	xzr, [x24, #16]
	subs	x0, x8, x9
	asr	x27, x0, #3
	b.eq	.LBB14_64
// %bb.60:                              //   in Loop: Header=BB14_46 Depth=1
	lsr	x8, x27, #61
	cbnz	x8, .LBB14_150
// %bb.61:                              //   in Loop: Header=BB14_46 Depth=1
.Ltmp129:
	bl	_Znwm
.Ltmp130:
// %bb.62:                              //   in Loop: Header=BB14_46 Depth=1
	mov	x28, x0
	b	.LBB14_65
.LBB14_63:                              //   in Loop: Header=BB14_46 Depth=1
.Ltmp134:
	sub	x0, x29, #168           // =168
	sub	x2, x29, #144           // =144
	mov	x1, x24
	bl	_ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_
.Ltmp135:
	b	.LBB14_68
.LBB14_64:                              //   in Loop: Header=BB14_46 Depth=1
	mov	x28, xzr
.LBB14_65:                              //   in Loop: Header=BB14_46 Depth=1
	add	x8, x28, x27, lsl #3
	stp	x28, x0, [x24]
	str	x8, [x24, #16]
	ldp	x1, x8, [x29, #-144]
	subs	x2, x8, x1
	asr	x27, x2, #3
	b.eq	.LBB14_67
// %bb.66:                              //   in Loop: Header=BB14_46 Depth=1
	bl	memmove
.LBB14_67:                              //   in Loop: Header=BB14_46 Depth=1
	add	x8, x28, x27, lsl #3
	str	x8, [x24, #8]
	ldur	x8, [x29, #-160]
	add	x8, x8, #24             // =24
	stur	x8, [x29, #-160]
.LBB14_68:                              //   in Loop: Header=BB14_46 Depth=1
	add	w26, w26, #1            // =1
	cmp	w26, #10                // =10
	b.lo	.LBB14_46
// %bb.69:
	ldr	w8, [x21]
	cbnz	w8, .LBB14_71
// %bb.70:
	adrp	x0, .L.str.3
	add	x0, x0, :lo12:.L.str.3
	bl	printf
.LBB14_71:
	adrp	x8, .LCPI14_0
	ldr	d0, [x8, :lo12:.LCPI14_0]
	ucvtf	d9, w23
	ucvtf	d8, x25
	fdiv	d1, d9, d8
	adrp	x0, .L.str.4
	mov	x26, #-6148914691236517206
	fmul	d0, d1, d0
	add	x0, x0, :lo12:.L.str.4
	mov	w1, w23
	mov	x2, x22
	movk	x26, #2730, lsl #48
	bl	printf
	adrp	x8, .LCPI14_1
	ldr	d0, [x8, :lo12:.LCPI14_1]
	ucvtf	d10, x22
	adrp	x0, .L.str.5
	add	x0, x0, :lo12:.L.str.5
	fmul	d0, d10, d0
	fdiv	d0, d0, d9
	bl	printf
	ldp	x0, x1, [x29, #-168]
	subs	x8, x1, x0
	b.eq	.LBB14_75
// %bb.72:
	mov	x10, #-6148914691236517206
	asr	x9, x8, #3
	movk	x10, #43691
	mul	x9, x9, x10
	cmp	x9, x26
	b.hi	.LBB14_152
// %bb.73:
.Ltmp137:
	mov	x0, x8
	bl	_Znwm
.Ltmp138:
// %bb.74:
	mov	x23, x0
	ldp	x0, x1, [x29, #-168]
	mov	x24, x23
	b	.LBB14_76
.LBB14_75:
	mov	x23, xzr
	mov	x24, xzr
.LBB14_76:
.Ltmp142:
	mov	x2, x24
	bl	_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_
.Ltmp143:
// %bb.77:
	mov	x25, x0
	subs	x27, x0, x24
	b.eq	.LBB14_83
// %bb.78:
	ldp	x8, x9, [x24]
	subs	x0, x9, x8
	b.eq	.LBB14_84
// %bb.79:
	asr	x8, x0, #3
	lsr	x8, x8, #61
	cbnz	x8, .LBB14_156
// %bb.80:
.Ltmp145:
	bl	_Znwm
.Ltmp146:
// %bb.81:
	ldp	x1, x8, [x24]
	mov	x26, x0
	subs	x2, x8, x1
	b.eq	.LBB14_85
// %bb.82:
	mov	x0, x26
	asr	x28, x2, #3
	bl	memmove
	mov	w8, wzr
	b	.LBB14_86
.LBB14_83:
	mov	x22, xzr
	cbnz	x24, .LBB14_114
	b	.LBB14_115
.LBB14_84:
	mov	x22, xzr
	mov	x26, xzr
	mov	x28, xzr
	orr	w8, wzr, #0x1
	b	.LBB14_87
.LBB14_85:
	mov	x28, xzr
	orr	w8, wzr, #0x1
.LBB14_86:
	mov	x22, x26
.LBB14_87:
	mov	x10, #-6148914691236517206
	asr	x9, x27, #3
	movk	x10, #43691
	mul	x9, x9, x10
	cmp	x9, #1                  // =1
	mov	x27, x24
	b.ls	.LBB14_110
// %bb.88:
	cmp	x28, #1                 // =1
	csinc	x12, x28, xzr, hi
	add	x10, x22, #16           // =16
	orr	w11, wzr, #0x1
	add	x13, x22, x12, lsl #3
	and	x14, x12, #0xfffffffffffffffc
	orr	w15, wzr, #0x18
	b	.LBB14_100
.LBB14_89:                              //   in Loop: Header=BB14_100 Depth=1
	add	x17, x16, #16           // =16
	mov	x18, x10
	mov	x0, x14
.LBB14_90:                              //   Parent Loop BB14_100 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldp	q1, q0, [x17, #-16]
	ldp	q3, q2, [x18, #-16]
	cmhi	v3.2d, v3.2d, v1.2d
	xtn	v3.2s, v3.2d
	fmov	w1, s3
	tbz	w1, #0, .LBB14_92
// %bb.91:                              //   in Loop: Header=BB14_90 Depth=2
	stur	d1, [x18, #-16]
.LBB14_92:                              //   in Loop: Header=BB14_90 Depth=2
	mov	w1, v3.s[1]
	tbz	w1, #0, .LBB14_94
// %bb.93:                              //   in Loop: Header=BB14_90 Depth=2
	sub	x1, x18, #8             // =8
	st1	{ v1.d }[1], [x1]
.LBB14_94:                              //   in Loop: Header=BB14_90 Depth=2
	cmhi	v1.2d, v2.2d, v0.2d
	xtn	v1.2s, v1.2d
	fmov	w1, s1
	tbz	w1, #0, .LBB14_96
// %bb.95:                              //   in Loop: Header=BB14_90 Depth=2
	str	d0, [x18]
.LBB14_96:                              //   in Loop: Header=BB14_90 Depth=2
	mov	w1, v1.s[1]
	tbz	w1, #0, .LBB14_98
// %bb.97:                              //   in Loop: Header=BB14_90 Depth=2
	add	x1, x18, #8             // =8
	st1	{ v0.d }[1], [x1]
.LBB14_98:                              //   in Loop: Header=BB14_90 Depth=2
	subs	x0, x0, #4              // =4
	add	x18, x18, #32           // =32
	add	x17, x17, #32           // =32
	b.ne	.LBB14_90
// %bb.99:                              //   in Loop: Header=BB14_100 Depth=1
	cmp	x12, x14
	mov	x17, x14
	b.ne	.LBB14_105
	b	.LBB14_108
.LBB14_100:                             // =>This Loop Header: Depth=1
                                        //     Child Loop BB14_90 Depth 2
                                        //     Child Loop BB14_105 Depth 2
	tbnz	w8, #0, .LBB14_108
// %bb.101:                             //   in Loop: Header=BB14_100 Depth=1
	mul	x16, x11, x15
	ldr	x16, [x24, x16]
	cmp	x12, #3                 // =3
	b.ls	.LBB14_104
// %bb.102:                             //   in Loop: Header=BB14_100 Depth=1
	add	x17, x16, x12, lsl #3
	cmp	x26, x17
	b.hs	.LBB14_89
// %bb.103:                             //   in Loop: Header=BB14_100 Depth=1
	cmp	x16, x13
	b.hs	.LBB14_89
.LBB14_104:                             //   in Loop: Header=BB14_100 Depth=1
	mov	x17, xzr
.LBB14_105:                             //   Parent Loop BB14_100 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x0, x17, #3
	ldr	x18, [x16, x0]
	ldr	x0, [x22, x0]
	cmp	x18, x0
	b.hs	.LBB14_107
// %bb.106:                             //   in Loop: Header=BB14_105 Depth=2
	str	x18, [x22, x17, lsl #3]
.LBB14_107:                             //   in Loop: Header=BB14_105 Depth=2
	add	x17, x17, #1            // =1
	cmp	x17, x28
	b.lo	.LBB14_105
.LBB14_108:                             //   in Loop: Header=BB14_100 Depth=1
	add	x11, x11, #1            // =1
	cmp	x11, x9
	b.ne	.LBB14_100
// %bb.109:
	mov	x27, x24
.LBB14_110:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x27]
	cbz	x0, .LBB14_112
// %bb.111:                             //   in Loop: Header=BB14_110 Depth=1
	bl	_ZdlPv
.LBB14_112:                             //   in Loop: Header=BB14_110 Depth=1
	add	x27, x27, #24           // =24
	cmp	x25, x27
	b.ne	.LBB14_110
// %bb.113:
	mov	x26, #-6148914691236517206
	movk	x26, #2730, lsl #48
	cbz	x24, .LBB14_115
.LBB14_114:
	mov	x0, x23
	bl	_ZdlPv
.LBB14_115:
	ldp	x0, x1, [x29, #-168]
	mov	x9, #-6148914691236517206
	movk	x9, #43691
	stp	xzr, xzr, [x29, #-216]
	subs	x8, x1, x0
	asr	x10, x8, #3
	mul	x25, x10, x9
	stur	xzr, [x29, #-200]
	b.eq	.LBB14_119
// %bb.116:
	cmp	x25, x26
	b.hi	.LBB14_154
// %bb.117:
.Ltmp150:
	mov	x0, x8
	bl	_Znwm
.Ltmp151:
// %bb.118:
	mov	x23, x0
	ldp	x0, x1, [x29, #-168]
	mov	x24, x23
	b	.LBB14_120
.LBB14_119:
	mov	x23, xzr
	mov	x24, xzr
.LBB14_120:
	orr	w8, wzr, #0x18
	madd	x8, x25, x8, x24
	stp	x23, x23, [x29, #-216]
	stur	x8, [x29, #-200]
.Ltmp155:
	mov	x2, x24
	bl	_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_
.Ltmp156:
// %bb.121:
	mov	x25, x0
	stur	x0, [x29, #-208]
.Ltmp158:
	sub	x8, x29, #192           // =192
	sub	x0, x29, #216           // =216
	bl	_Z16compute_averagesSt6vectorIS_IySaIyEESaIS1_EE
.Ltmp159:
// %bb.122:
	cmp	x24, x25
	b.eq	.LBB14_127
// %bb.123:
	mov	x26, x24
.LBB14_124:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x26]
	cbz	x0, .LBB14_126
// %bb.125:                             //   in Loop: Header=BB14_124 Depth=1
	bl	_ZdlPv
.LBB14_126:                             //   in Loop: Header=BB14_124 Depth=1
	add	x26, x26, #24           // =24
	cmp	x25, x26
	b.ne	.LBB14_124
.LBB14_127:
	cbz	x24, .LBB14_129
// %bb.128:
	mov	x0, x23
	bl	_ZdlPv
.LBB14_129:
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
	ldur	x23, [x29, #-192]
	adrp	x0, .L.str.9
	add	x0, x0, :lo12:.L.str.9
	ldp	d0, d1, [x23]
	ldp	d2, d3, [x23, #16]
	ldr	d4, [x23, #32]
	bl	printf
	mov	w0, #10
	bl	putchar
	cbz	x20, .LBB14_131
// %bb.130:
	mov	x0, x20
	bl	_ZdaPv
.LBB14_131:
	mov	x0, x21
	bl	_ZdaPv
	cbz	x23, .LBB14_133
// %bb.132:
	mov	x0, x23
	bl	_ZdlPv
.LBB14_133:
	cbz	x22, .LBB14_135
// %bb.134:
	mov	x0, x22
	bl	_ZdlPv
.LBB14_135:
	ldp	x20, x21, [x29, #-168]
	cmp	x20, x21
	b.eq	.LBB14_140
.LBB14_136:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x20]
	cbz	x0, .LBB14_138
// %bb.137:                             //   in Loop: Header=BB14_136 Depth=1
	bl	_ZdlPv
.LBB14_138:                             //   in Loop: Header=BB14_136 Depth=1
	add	x20, x20, #24           // =24
	cmp	x21, x20
	b.ne	.LBB14_136
// %bb.139:
	ldur	x20, [x29, #-168]
.LBB14_140:
	cbz	x20, .LBB14_142
// %bb.141:
	mov	x0, x20
	bl	_ZdlPv
.LBB14_142:
	ldur	x0, [x29, #-144]
	cbz	x0, .LBB14_144
// %bb.143:
	bl	_ZdlPv
.LBB14_144:
	ldr	w0, [sp, #264]
.Ltmp164:
	bl	close
.Ltmp165:
// %bb.145:
	ldr	x0, [sp, #416]
	cbz	x0, .LBB14_147
// %bb.146:
	bl	_ZdlPv
.LBB14_147:
	ldr	x0, [sp, #392]
	cbz	x0, .LBB14_149
// %bb.148:
	bl	_ZdlPv
.LBB14_149:
	mov	x0, x19
	bl	_ZdlPv
	adrp	x0, .Lstr.30
	add	x0, x0, :lo12:.Lstr.30
	bl	puts
	bl	_Z4unitIXadL_Z16simdjson_decoderPjRjjmEEEbv
	adrp	x19, .L.str.11
	add	x19, x19, :lo12:.L.str.11
	mov	w1, #44
	mov	x0, x19
	bl	_Z4testIXadL_Z16simdjson_decoderPjRjjmEEEvPKcc
	adrp	x0, .Lstr.31
	add	x0, x0, :lo12:.Lstr.31
	bl	puts
	bl	_Z4unitIXadL_Z13basic_decoderPjRjjmEEEbv
	mov	w1, #44
	mov	x0, x19
	bl	_Z4testIXadL_Z13basic_decoderPjRjjmEEEvPKcc
	adrp	x0, .Lstr.32
	add	x0, x0, :lo12:.Lstr.32
	bl	puts
	bl	_Z4unitIXadL_Z14faster_decoderPjRjjmEEEbv
	mov	w1, #44
	mov	x0, x19
	bl	_Z4testIXadL_Z14faster_decoderPjRjjmEEEvPKcc
	mov	w0, #10
	bl	putchar
	mov	w0, wzr
	add	sp, sp, #624            // =624
	ldp	x29, x30, [sp, #112]    // 16-byte Folded Reload
	ldp	x20, x19, [sp, #96]     // 16-byte Folded Reload
	ldp	x22, x21, [sp, #80]     // 16-byte Folded Reload
	ldp	x24, x23, [sp, #64]     // 16-byte Folded Reload
	ldp	x26, x25, [sp, #48]     // 16-byte Folded Reload
	ldp	x28, x27, [sp, #32]     // 16-byte Folded Reload
	ldp	d9, d8, [sp, #16]       // 16-byte Folded Reload
	ldr	d10, [sp], #128         // 8-byte Folded Reload
	ret
.LBB14_150:
.Ltmp131:
	bl	_ZSt17__throw_bad_allocv
.Ltmp132:
// %bb.151:
.LBB14_152:
.Ltmp139:
	bl	_ZSt17__throw_bad_allocv
.Ltmp140:
// %bb.153:
.LBB14_154:
.Ltmp152:
	bl	_ZSt17__throw_bad_allocv
.Ltmp153:
// %bb.155:
.LBB14_156:
.Ltmp147:
	bl	_ZSt17__throw_bad_allocv
.Ltmp148:
// %bb.157:
.LBB14_158:
.Ltmp133:
	b	.LBB14_191
.LBB14_159:
.Ltmp149:
	mov	x20, x0
	mov	x21, x24
.LBB14_160:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x21]
	cbz	x0, .LBB14_162
// %bb.161:                             //   in Loop: Header=BB14_160 Depth=1
	bl	_ZdlPv
.LBB14_162:                             //   in Loop: Header=BB14_160 Depth=1
	add	x21, x21, #24           // =24
	cmp	x25, x21
	b.ne	.LBB14_160
// %bb.163:
	cbnz	x24, .LBB14_179
	b	.LBB14_192
.LBB14_164:
.Ltmp154:
	mov	x20, x0
	cbnz	x22, .LBB14_177
	b	.LBB14_192
.LBB14_165:
.Ltmp141:
	b	.LBB14_191
.LBB14_166:
.Ltmp166:
.LBB14_167:
	add	x20, sp, #264           // =264
	mov	x19, x0
	add	x0, x20, #152           // =152
	bl	_ZNSt6vectorImSaImEED2Ev
	add	x0, x20, #128           // =128
	bl	_ZNSt6vectorImSaImEED2Ev
	mov	x0, x19
	bl	__clang_call_terminate
.LBB14_168:
.Ltmp160:
	cmp	x24, x25
	mov	x20, x0
	b.eq	.LBB14_173
// %bb.169:
	mov	x21, x24
.LBB14_170:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x21]
	cbz	x0, .LBB14_172
// %bb.171:                             //   in Loop: Header=BB14_170 Depth=1
	bl	_ZdlPv
.LBB14_172:                             //   in Loop: Header=BB14_170 Depth=1
	add	x21, x21, #24           // =24
	cmp	x25, x21
	b.ne	.LBB14_170
.LBB14_173:
	cbnz	x24, .LBB14_175
	b	.LBB14_176
.LBB14_174:
.Ltmp157:
	mov	x20, x0
	cbz	x24, .LBB14_176
.LBB14_175:
	mov	x0, x23
	bl	_ZdlPv
.LBB14_176:
	cbz	x22, .LBB14_192
.LBB14_177:
	mov	x0, x22
	b	.LBB14_180
.LBB14_178:
.Ltmp144:
	mov	x20, x0
	cbz	x24, .LBB14_192
.LBB14_179:
	mov	x0, x23
.LBB14_180:
	bl	_ZdlPv
	b	.LBB14_192
.LBB14_181:
.Ltmp124:
	b	.LBB14_191
.LBB14_182:
.Ltmp121:
	ldr	x8, [sp, #8]
	mov	x20, x0
	cbz	x8, .LBB14_206
// %bb.183:
	mov	x0, x8
	b	.LBB14_205
.LBB14_184:
.Ltmp118:
	b	.LBB14_188
.LBB14_185:
.Ltmp115:
	mov	x20, x0
	mov	x19, x23
	b	.LBB14_206
.LBB14_186:
.Ltmp112:
	mov	x20, x0
	mov	x19, x24
	b	.LBB14_206
.LBB14_187:
.Ltmp109:
.LBB14_188:
	mov	x20, x0
	b	.LBB14_206
.LBB14_189:
.Ltmp106:
	bl	_Unwind_Resume
.LBB14_190:
.Ltmp136:
.LBB14_191:
	mov	x20, x0
.LBB14_192:
	ldp	x21, x22, [x29, #-168]
	cmp	x21, x22
	b.eq	.LBB14_197
.LBB14_193:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x21]
	cbz	x0, .LBB14_195
// %bb.194:                             //   in Loop: Header=BB14_193 Depth=1
	bl	_ZdlPv
.LBB14_195:                             //   in Loop: Header=BB14_193 Depth=1
	add	x21, x21, #24           // =24
	cmp	x22, x21
	b.ne	.LBB14_193
// %bb.196:
	ldur	x21, [x29, #-168]
.LBB14_197:
	cbz	x21, .LBB14_199
// %bb.198:
	mov	x0, x21
	bl	_ZdlPv
.LBB14_199:
	ldur	x0, [x29, #-144]
	cbz	x0, .LBB14_201
// %bb.200:
	bl	_ZdlPv
.LBB14_201:
	ldr	w0, [sp, #264]
.Ltmp161:
	bl	close
.Ltmp162:
// %bb.202:
	ldr	x0, [sp, #416]
	cbz	x0, .LBB14_204
// %bb.203:
	bl	_ZdlPv
.LBB14_204:
	ldr	x0, [sp, #392]
	cbz	x0, .LBB14_206
.LBB14_205:
	bl	_ZdlPv
.LBB14_206:
	mov	x0, x19
	bl	_ZdlPv
	mov	x0, x20
	bl	_Unwind_Resume
.LBB14_207:
.Ltmp163:
	b	.LBB14_167
.Lfunc_end14:
	.size	main, .Lfunc_end14-main
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table14:
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
	.uleb128 .Ltmp131-.Ltmp165      //   Call between .Ltmp165 and .Ltmp131
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp131-.Lfunc_begin5 // >> Call Site 21 <<
	.uleb128 .Ltmp132-.Ltmp131      //   Call between .Ltmp131 and .Ltmp132
	.uleb128 .Ltmp133-.Lfunc_begin5 //     jumps to .Ltmp133
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp139-.Lfunc_begin5 // >> Call Site 22 <<
	.uleb128 .Ltmp140-.Ltmp139      //   Call between .Ltmp139 and .Ltmp140
	.uleb128 .Ltmp141-.Lfunc_begin5 //     jumps to .Ltmp141
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp152-.Lfunc_begin5 // >> Call Site 23 <<
	.uleb128 .Ltmp153-.Ltmp152      //   Call between .Ltmp152 and .Ltmp153
	.uleb128 .Ltmp154-.Lfunc_begin5 //     jumps to .Ltmp154
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp147-.Lfunc_begin5 // >> Call Site 24 <<
	.uleb128 .Ltmp148-.Ltmp147      //   Call between .Ltmp147 and .Ltmp148
	.uleb128 .Ltmp149-.Lfunc_begin5 //     jumps to .Ltmp149
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp148-.Lfunc_begin5 // >> Call Site 25 <<
	.uleb128 .Ltmp161-.Ltmp148      //   Call between .Ltmp148 and .Ltmp161
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp161-.Lfunc_begin5 // >> Call Site 26 <<
	.uleb128 .Ltmp162-.Ltmp161      //   Call between .Ltmp161 and .Ltmp162
	.uleb128 .Ltmp163-.Lfunc_begin5 //     jumps to .Ltmp163
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp162-.Lfunc_begin5 // >> Call Site 27 <<
	.uleb128 .Lfunc_end14-.Ltmp162  //   Call between .Ltmp162 and .Lfunc_end14
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
	.section	.text._Z4unitIXadL_Z16simdjson_decoderPjRjjmEEEbv,"axG",@progbits,_Z4unitIXadL_Z16simdjson_decoderPjRjjmEEEbv,comdat
	.weak	_Z4unitIXadL_Z16simdjson_decoderPjRjjmEEEbv // -- Begin function _Z4unitIXadL_Z16simdjson_decoderPjRjjmEEEbv
	.p2align	2
	.type	_Z4unitIXadL_Z16simdjson_decoderPjRjjmEEEbv,@function
_Z4unitIXadL_Z16simdjson_decoderPjRjjmEEEbv: // @_Z4unitIXadL_Z16simdjson_decoderPjRjjmEEEbv
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
.LBB15_1:                               // =>This Loop Header: Depth=1
                                        //     Child Loop BB15_2 Depth 2
                                        //     Child Loop BB15_6 Depth 2
	lsl	x19, x21, x20
	add	x0, sp, #16             // =16
	add	x1, sp, #12             // =12
	mov	w2, wzr
	mov	x3, x19
	str	wzr, [sp, #12]
	bl	_Z14faster_decoderPjRjjm
	mov	w8, wzr
.LBB15_2:                               //   Parent Loop BB15_1 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	rbit	x9, x19
	sub	x10, x19, #1            // =1
	clz	x9, x9
	ands	x19, x10, x19
	str	w9, [x23, w8, uxtw #2]
	add	w8, w8, #1              // =1
	b.ne	.LBB15_2
// %bb.3:                               //   in Loop: Header=BB15_1 Depth=1
	ldr	w9, [sp, #12]
	cmp	w9, w8
	b.ne	.LBB15_20
// %bb.4:                               //   in Loop: Header=BB15_1 Depth=1
	cbz	w8, .LBB15_8
// %bb.5:                               //   in Loop: Header=BB15_1 Depth=1
	mov	x9, xzr
	mov	w8, w8
.LBB15_6:                               //   Parent Loop BB15_1 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x10, x9, #2
	ldr	w11, [x22, x10]
	ldr	w10, [x23, x10]
	cmp	w11, w10
	b.ne	.LBB15_20
// %bb.7:                               //   in Loop: Header=BB15_6 Depth=2
	add	x9, x9, #1              // =1
	cmp	x9, x8
	b.lo	.LBB15_6
.LBB15_8:                               //   in Loop: Header=BB15_1 Depth=1
	add	x20, x20, #1            // =1
	cmp	x20, #64                // =64
	b.lo	.LBB15_1
// %bb.9:
	mov	w22, #16960
	mov	x20, xzr
	add	x21, sp, #16            // =16
	movk	w22, #15, lsl #16
	add	x23, sp, #272           // =272
.LBB15_10:                              // =>This Loop Header: Depth=1
                                        //     Child Loop BB15_12 Depth 2
                                        //     Child Loop BB15_16 Depth 2
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
	bl	_Z14faster_decoderPjRjjm
	fmov	d0, x19
	cnt	v0.8b, v0.8b
	uaddlv	h0, v0.8b
	fmov	w8, s0
	cbz	w8, .LBB15_13
// %bb.11:                              //   in Loop: Header=BB15_10 Depth=1
	add	w9, w8, #1              // =1
	add	x10, sp, #272           // =272
.LBB15_12:                              //   Parent Loop BB15_10 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	rbit	x11, x19
	sub	x12, x19, #1            // =1
	sub	w9, w9, #1              // =1
	clz	x11, x11
	cmp	w9, #1                  // =1
	str	w11, [x10], #4
	and	x19, x12, x19
	b.gt	.LBB15_12
.LBB15_13:                              //   in Loop: Header=BB15_10 Depth=1
	ldr	w9, [sp, #12]
	cmp	w9, w8
	b.ne	.LBB15_20
// %bb.14:                              //   in Loop: Header=BB15_10 Depth=1
	cbz	w8, .LBB15_18
// %bb.15:                              //   in Loop: Header=BB15_10 Depth=1
	mov	x9, xzr
.LBB15_16:                              //   Parent Loop BB15_10 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x10, x9, #2
	ldr	w11, [x21, x10]
	ldr	w10, [x23, x10]
	cmp	w11, w10
	b.ne	.LBB15_20
// %bb.17:                              //   in Loop: Header=BB15_16 Depth=2
	add	x9, x9, #1              // =1
	cmp	x9, x8
	b.lo	.LBB15_16
.LBB15_18:                              //   in Loop: Header=BB15_10 Depth=1
	add	x20, x20, #1            // =1
	cmp	x20, x22
	b.lo	.LBB15_10
// %bb.19:
	adrp	x0, .Lstr.48
	add	x0, x0, :lo12:.Lstr.48
	bl	puts
	orr	w0, wzr, #0x1
	b	.LBB15_21
.LBB15_20:
	adrp	x0, .Lstr.52
	add	x0, x0, :lo12:.Lstr.52
	bl	puts
	mov	w0, wzr
.LBB15_21:
	add	sp, sp, #528            // =528
	ldp	x29, x30, [sp, #48]     // 16-byte Folded Reload
	ldp	x20, x19, [sp, #32]     // 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]     // 16-byte Folded Reload
	ldp	x28, x23, [sp], #64     // 16-byte Folded Reload
	ret
.Lfunc_end15:
	.size	_Z4unitIXadL_Z16simdjson_decoderPjRjjmEEEbv, .Lfunc_end15-_Z4unitIXadL_Z16simdjson_decoderPjRjjmEEEbv
	.cfi_endproc
                                        // -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3               // -- Begin function _Z4testIXadL_Z16simdjson_decoderPjRjjmEEEvPKcc
.LCPI16_0:
	.xword	4636737291354636288     // double 100
.LCPI16_1:
	.xword	4634204016564240384     // double 64
	.section	.text._Z4testIXadL_Z16simdjson_decoderPjRjjmEEEvPKcc,"axG",@progbits,_Z4testIXadL_Z16simdjson_decoderPjRjjmEEEvPKcc,comdat
	.weak	_Z4testIXadL_Z16simdjson_decoderPjRjjmEEEvPKcc
	.p2align	2
	.type	_Z4testIXadL_Z16simdjson_decoderPjRjjmEEEvPKcc,@function
_Z4testIXadL_Z16simdjson_decoderPjRjjmEEEvPKcc: // @_Z4testIXadL_Z16simdjson_decoderPjRjjmEEEvPKcc
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
	cbz	x0, .LBB16_8
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
	b.eq	.LBB16_7
// %bb.2:
	mov	x9, xzr
	orr	w10, wzr, #0x1
.LBB16_3:                               // =>This Inner Loop Header: Depth=1
	ldrb	w11, [x21, x9]
	cmp	w11, w19, uxtb
	b.eq	.LBB16_5
// %bb.4:                               //   in Loop: Header=BB16_3 Depth=1
	cmp	w11, #31                // =31
	b.hi	.LBB16_6
.LBB16_5:                               //   in Loop: Header=BB16_3 Depth=1
	lsr	x11, x9, #3
	and	x11, x11, #0x1ffffffffffffff8
	ldr	x12, [x20, x11]
	lsl	x13, x10, x9
	orr	x12, x12, x13
	str	x12, [x20, x11]
.LBB16_6:                               //   in Loop: Header=BB16_3 Depth=1
	add	x9, x9, #1              // =1
	cmp	x8, x9
	b.ne	.LBB16_3
.LBB16_7:
	mov	x0, x21
	bl	_ZdaPv
	b	.LBB16_9
.LBB16_8:
	mov	x20, xzr
                                        // implicit-def: $x22
.LBB16_9:
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
// %bb.10:
	mov	x19, x0
	str	wzr, [x0]
.Ltmp170:
	orr	w0, wzr, #0x8
	bl	_Znwm
.Ltmp171:
// %bb.11:
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
.Ltmp176:
	orr	w0, wzr, #0x20
	bl	_Znwm
.Ltmp177:
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
.Ltmp179:
	mov	w0, #20
	bl	_Znwm
.Ltmp180:
// %bb.14:
	ldr	w9, [x19, #16]
	ldr	q0, [x19]
	add	x8, x0, #20             // =20
	stp	x0, x0, [sp, #96]
	stp	x8, x8, [sp, #104]
	str	w9, [x0, #16]
	str	q0, [x0]
.Ltmp182:
	add	x0, sp, #120            // =120
	add	x1, sp, #96             // =96
	bl	_ZN11LinuxEventsILi0EEC2ESt6vectorIiSaIiEE
.Ltmp183:
// %bb.15:
	ldr	x0, [sp, #96]
	cbz	x0, .LBB16_17
// %bb.16:
	bl	_ZdlPv
.LBB16_17:
	stp	xzr, xzr, [sp, #80]
	stp	xzr, xzr, [sp, #64]
	stp	xzr, xzr, [sp, #48]
.Ltmp185:
	mov	w0, #40
	bl	_Znwm
.Ltmp186:
// %bb.18:
	ldp	x24, x8, [sp, #72]
	mov	x23, x0
	movi	v0.2d, #0000000000000000
	str	xzr, [x0, #32]
	subs	x2, x8, x24
	stp	q0, q0, [x0]
	b.eq	.LBB16_20
// %bb.19:
	mov	x0, x23
	mov	x1, x24
	bl	memmove
.LBB16_20:
	cbz	x24, .LBB16_22
// %bb.21:
	mov	x0, x24
	bl	_ZdlPv
.LBB16_22:
	mov	w26, wzr
	add	x8, x23, #40            // =40
	str	x8, [sp, #88]
	stp	x23, x8, [sp, #72]
.LBB16_23:                              // =>This Loop Header: Depth=1
                                        //     Child Loop BB16_26 Depth 2
                                        //       Child Loop BB16_27 Depth 3
.Ltmp188:
	add	x0, sp, #120            // =120
	bl	_ZN11LinuxEventsILi0EE5startEv
.Ltmp189:
// %bb.24:                              //   in Loop: Header=BB16_23 Depth=1
	cbz	x22, .LBB16_29
// %bb.25:                              //   in Loop: Header=BB16_23 Depth=1
	mov	x8, xzr
	mov	w9, wzr
.LBB16_26:                              //   Parent Loop BB16_23 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB16_27 Depth 3
	ldr	x10, [x20, x8, lsl #3]
	fmov	d0, x10
	cnt	v0.8b, v0.8b
	uaddlv	h0, v0.8b
	fmov	w11, s0
	add	w23, w9, w11
	cbz	x10, .LBB16_28
.LBB16_27:                              //   Parent Loop BB16_23 Depth=1
                                        //     Parent Loop BB16_26 Depth=2
                                        // =>    This Inner Loop Header: Depth=3
	rbit	x11, x10
	sub	x12, x10, #1            // =1
	clz	x11, x11
	and	x10, x12, x10
	add	w11, w11, w8
	str	w11, [x21, w9, uxtw #2]
	rbit	x11, x10
	clz	x11, x11
	add	w12, w9, #1             // =1
	add	w11, w11, w8
	str	w11, [x21, w12, uxtw #2]
	sub	x12, x10, #1            // =1
	and	x10, x12, x10
	rbit	x12, x10
	clz	x12, x12
	add	w11, w9, #2             // =2
	add	w12, w12, w8
	str	w12, [x21, w11, uxtw #2]
	sub	x12, x10, #1            // =1
	and	x10, x12, x10
	rbit	x12, x10
	clz	x12, x12
	add	w11, w9, #3             // =3
	add	w12, w12, w8
	str	w12, [x21, w11, uxtw #2]
	sub	x12, x10, #1            // =1
	and	x10, x12, x10
	rbit	x12, x10
	clz	x12, x12
	add	w11, w9, #4             // =4
	add	w12, w12, w8
	str	w12, [x21, w11, uxtw #2]
	sub	x12, x10, #1            // =1
	and	x10, x12, x10
	rbit	x12, x10
	clz	x12, x12
	add	w11, w9, #5             // =5
	add	w12, w12, w8
	str	w12, [x21, w11, uxtw #2]
	sub	x12, x10, #1            // =1
	and	x10, x12, x10
	rbit	x12, x10
	clz	x12, x12
	add	w11, w9, #6             // =6
	add	w12, w12, w8
	str	w12, [x21, w11, uxtw #2]
	sub	x12, x10, #1            // =1
	and	x12, x12, x10
	sub	x10, x12, #1            // =1
	ands	x10, x10, x12
	rbit	x12, x12
	clz	x12, x12
	add	w11, w9, #7             // =7
	add	w12, w12, w8
	add	w9, w9, #8              // =8
	str	w12, [x21, w11, uxtw #2]
	b.ne	.LBB16_27
.LBB16_28:                              //   in Loop: Header=BB16_26 Depth=2
	add	x8, x8, #1              // =1
	cmp	x8, x22
	mov	w9, w23
	b.ne	.LBB16_26
	b	.LBB16_30
.LBB16_29:                              //   in Loop: Header=BB16_23 Depth=1
	mov	w23, wzr
.LBB16_30:                              //   in Loop: Header=BB16_23 Depth=1
.Ltmp190:
	add	x0, sp, #120            // =120
	add	x1, sp, #72             // =72
	bl	_ZN11LinuxEventsILi0EE3endERSt6vectorIySaIyEE
.Ltmp191:
// %bb.31:                              //   in Loop: Header=BB16_23 Depth=1
	ldp	x24, x8, [sp, #56]
	cmp	x24, x8
	b.eq	.LBB16_36
// %bb.32:                              //   in Loop: Header=BB16_23 Depth=1
	ldp	x9, x8, [sp, #72]
	stp	xzr, xzr, [x24]
	str	xzr, [x24, #16]
	subs	x0, x8, x9
	asr	x27, x0, #3
	b.eq	.LBB16_37
// %bb.33:                              //   in Loop: Header=BB16_23 Depth=1
	lsr	x8, x27, #61
	cbnz	x8, .LBB16_125
// %bb.34:                              //   in Loop: Header=BB16_23 Depth=1
.Ltmp192:
	bl	_Znwm
.Ltmp193:
// %bb.35:                              //   in Loop: Header=BB16_23 Depth=1
	mov	x28, x0
	b	.LBB16_38
.LBB16_36:                              //   in Loop: Header=BB16_23 Depth=1
.Ltmp197:
	add	x0, sp, #48             // =48
	add	x2, sp, #72             // =72
	mov	x1, x24
	bl	_ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_
.Ltmp198:
	b	.LBB16_41
.LBB16_37:                              //   in Loop: Header=BB16_23 Depth=1
	mov	x28, xzr
.LBB16_38:                              //   in Loop: Header=BB16_23 Depth=1
	add	x8, x28, x27, lsl #3
	stp	x28, x0, [x24]
	str	x8, [x24, #16]
	ldp	x1, x8, [sp, #72]
	subs	x2, x8, x1
	asr	x27, x2, #3
	b.eq	.LBB16_40
// %bb.39:                              //   in Loop: Header=BB16_23 Depth=1
	bl	memmove
.LBB16_40:                              //   in Loop: Header=BB16_23 Depth=1
	add	x8, x28, x27, lsl #3
	str	x8, [x24, #8]
	ldr	x8, [sp, #56]
	add	x8, x8, #24             // =24
	str	x8, [sp, #56]
.LBB16_41:                              //   in Loop: Header=BB16_23 Depth=1
	add	w26, w26, #1            // =1
	cmp	w26, #10                // =10
	b.lo	.LBB16_23
// %bb.42:
	ldr	w8, [x21]
	cbnz	w8, .LBB16_44
// %bb.43:
	adrp	x0, .L.str.3
	add	x0, x0, :lo12:.L.str.3
	bl	printf
.LBB16_44:
	adrp	x8, .LCPI16_0
	ldr	d0, [x8, :lo12:.LCPI16_0]
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
	adrp	x8, .LCPI16_1
	ldr	d0, [x8, :lo12:.LCPI16_1]
	ucvtf	d10, x22
	adrp	x0, .L.str.5
	add	x0, x0, :lo12:.L.str.5
	fmul	d0, d10, d0
	fdiv	d0, d0, d9
	bl	printf
	ldp	x0, x1, [sp, #48]
	subs	x8, x1, x0
	b.eq	.LBB16_48
// %bb.45:
	mov	x10, #-6148914691236517206
	asr	x9, x8, #3
	movk	x10, #43691
	mul	x9, x9, x10
	cmp	x9, x28
	b.hi	.LBB16_127
// %bb.46:
.Ltmp200:
	mov	x0, x8
	bl	_Znwm
.Ltmp201:
// %bb.47:
	mov	x23, x0
	ldp	x0, x1, [sp, #48]
	mov	x24, x23
	b	.LBB16_49
.LBB16_48:
	mov	x23, xzr
	mov	x24, xzr
.LBB16_49:
.Ltmp205:
	mov	x2, x24
	bl	_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_
.Ltmp206:
// %bb.50:
	mov	x25, x0
	subs	x27, x0, x24
	b.eq	.LBB16_56
// %bb.51:
	ldp	x8, x9, [x24]
	subs	x0, x9, x8
	b.eq	.LBB16_57
// %bb.52:
	asr	x8, x0, #3
	lsr	x8, x8, #61
	cbnz	x8, .LBB16_131
// %bb.53:
.Ltmp208:
	bl	_Znwm
.Ltmp209:
// %bb.54:
	ldp	x1, x8, [x24]
	mov	x26, x0
	subs	x2, x8, x1
	b.eq	.LBB16_58
// %bb.55:
	mov	x0, x26
	asr	x28, x2, #3
	bl	memmove
	mov	w8, wzr
	b	.LBB16_59
.LBB16_56:
	mov	x22, xzr
	cbnz	x24, .LBB16_88
	b	.LBB16_89
.LBB16_57:
	mov	x22, xzr
	mov	x26, xzr
	mov	x28, xzr
	orr	w8, wzr, #0x1
	b	.LBB16_60
.LBB16_58:
	mov	x28, xzr
	orr	w8, wzr, #0x1
.LBB16_59:
	mov	x22, x26
.LBB16_60:
	mov	x10, #-6148914691236517206
	asr	x9, x27, #3
	movk	x10, #43691
	mul	x9, x9, x10
	cmp	x9, #2                  // =2
	b.lo	.LBB16_83
// %bb.61:
	cmp	x28, #1                 // =1
	csinc	x12, x28, xzr, hi
	add	x10, x22, #16           // =16
	orr	w11, wzr, #0x1
	add	x13, x22, x12, lsl #3
	and	x14, x12, #0xfffffffffffffffc
	orr	w15, wzr, #0x18
	b	.LBB16_73
.LBB16_62:                              //   in Loop: Header=BB16_73 Depth=1
	add	x17, x16, #16           // =16
	mov	x18, x10
	mov	x0, x14
.LBB16_63:                              //   Parent Loop BB16_73 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldp	q1, q0, [x17, #-16]
	ldp	q3, q2, [x18, #-16]
	cmhi	v3.2d, v3.2d, v1.2d
	xtn	v3.2s, v3.2d
	fmov	w1, s3
	tbz	w1, #0, .LBB16_65
// %bb.64:                              //   in Loop: Header=BB16_63 Depth=2
	stur	d1, [x18, #-16]
.LBB16_65:                              //   in Loop: Header=BB16_63 Depth=2
	mov	w1, v3.s[1]
	tbz	w1, #0, .LBB16_67
// %bb.66:                              //   in Loop: Header=BB16_63 Depth=2
	sub	x1, x18, #8             // =8
	st1	{ v1.d }[1], [x1]
.LBB16_67:                              //   in Loop: Header=BB16_63 Depth=2
	cmhi	v1.2d, v2.2d, v0.2d
	xtn	v1.2s, v1.2d
	fmov	w1, s1
	tbz	w1, #0, .LBB16_69
// %bb.68:                              //   in Loop: Header=BB16_63 Depth=2
	str	d0, [x18]
.LBB16_69:                              //   in Loop: Header=BB16_63 Depth=2
	mov	w1, v1.s[1]
	tbz	w1, #0, .LBB16_71
// %bb.70:                              //   in Loop: Header=BB16_63 Depth=2
	add	x1, x18, #8             // =8
	st1	{ v0.d }[1], [x1]
.LBB16_71:                              //   in Loop: Header=BB16_63 Depth=2
	subs	x0, x0, #4              // =4
	add	x18, x18, #32           // =32
	add	x17, x17, #32           // =32
	b.ne	.LBB16_63
// %bb.72:                              //   in Loop: Header=BB16_73 Depth=1
	cmp	x12, x14
	mov	x17, x14
	b.ne	.LBB16_78
	b	.LBB16_81
.LBB16_73:                              // =>This Loop Header: Depth=1
                                        //     Child Loop BB16_63 Depth 2
                                        //     Child Loop BB16_78 Depth 2
	tbnz	w8, #0, .LBB16_81
// %bb.74:                              //   in Loop: Header=BB16_73 Depth=1
	mul	x16, x11, x15
	ldr	x16, [x24, x16]
	cmp	x12, #3                 // =3
	b.ls	.LBB16_77
// %bb.75:                              //   in Loop: Header=BB16_73 Depth=1
	add	x17, x16, x12, lsl #3
	cmp	x26, x17
	b.hs	.LBB16_62
// %bb.76:                              //   in Loop: Header=BB16_73 Depth=1
	cmp	x16, x13
	b.hs	.LBB16_62
.LBB16_77:                              //   in Loop: Header=BB16_73 Depth=1
	mov	x17, xzr
.LBB16_78:                              //   Parent Loop BB16_73 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x0, x17, #3
	ldr	x18, [x16, x0]
	ldr	x0, [x22, x0]
	cmp	x18, x0
	b.hs	.LBB16_80
// %bb.79:                              //   in Loop: Header=BB16_78 Depth=2
	str	x18, [x22, x17, lsl #3]
.LBB16_80:                              //   in Loop: Header=BB16_78 Depth=2
	add	x17, x17, #1            // =1
	cmp	x17, x28
	b.lo	.LBB16_78
.LBB16_81:                              //   in Loop: Header=BB16_73 Depth=1
	add	x11, x11, #1            // =1
	cmp	x11, x9
	b.ne	.LBB16_73
// %bb.82:
	cmp	x25, x24
	b.eq	.LBB16_124
.LBB16_83:
	mov	x28, #-6148914691236517206
	mov	x26, x24
	movk	x28, #2730, lsl #48
.LBB16_84:                              // =>This Inner Loop Header: Depth=1
	ldr	x0, [x26]
	cbz	x0, .LBB16_86
// %bb.85:                              //   in Loop: Header=BB16_84 Depth=1
	bl	_ZdlPv
.LBB16_86:                              //   in Loop: Header=BB16_84 Depth=1
	add	x26, x26, #24           // =24
	cmp	x25, x26
	b.ne	.LBB16_84
// %bb.87:
	cbz	x24, .LBB16_89
.LBB16_88:
	mov	x0, x23
	bl	_ZdlPv
.LBB16_89:
	ldp	x0, x1, [sp, #48]
	mov	x9, #-6148914691236517206
	movk	x9, #43691
	stp	xzr, xzr, [sp]
	subs	x8, x1, x0
	asr	x10, x8, #3
	mul	x25, x10, x9
	str	xzr, [sp, #16]
	b.eq	.LBB16_93
// %bb.90:
	cmp	x25, x28
	b.hi	.LBB16_129
// %bb.91:
.Ltmp213:
	mov	x0, x8
	bl	_Znwm
.Ltmp214:
// %bb.92:
	mov	x23, x0
	ldp	x0, x1, [sp, #48]
	mov	x24, x23
	b	.LBB16_94
.LBB16_93:
	mov	x23, xzr
	mov	x24, xzr
.LBB16_94:
	orr	w8, wzr, #0x18
	madd	x8, x25, x8, x24
	stp	x23, x23, [sp]
	str	x8, [sp, #16]
.Ltmp218:
	mov	x2, x24
	bl	_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_
.Ltmp219:
// %bb.95:
	mov	x25, x0
	str	x0, [sp, #8]
.Ltmp221:
	add	x8, sp, #24             // =24
	mov	x0, sp
	bl	_Z16compute_averagesSt6vectorIS_IySaIyEESaIS1_EE
.Ltmp222:
// %bb.96:
	cmp	x24, x25
	b.eq	.LBB16_101
// %bb.97:
	mov	x26, x24
.LBB16_98:                              // =>This Inner Loop Header: Depth=1
	ldr	x0, [x26]
	cbz	x0, .LBB16_100
// %bb.99:                              //   in Loop: Header=BB16_98 Depth=1
	bl	_ZdlPv
.LBB16_100:                             //   in Loop: Header=BB16_98 Depth=1
	add	x26, x26, #24           // =24
	cmp	x25, x26
	b.ne	.LBB16_98
.LBB16_101:
	cbz	x24, .LBB16_103
// %bb.102:
	mov	x0, x23
	bl	_ZdlPv
.LBB16_103:
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
	cbz	x20, .LBB16_105
// %bb.104:
	mov	x0, x20
	bl	_ZdaPv
.LBB16_105:
	mov	x0, x21
	bl	_ZdaPv
	cbz	x23, .LBB16_107
// %bb.106:
	mov	x0, x23
	bl	_ZdlPv
.LBB16_107:
	cbz	x22, .LBB16_109
// %bb.108:
	mov	x0, x22
	bl	_ZdlPv
.LBB16_109:
	ldp	x20, x21, [sp, #48]
	cmp	x20, x21
	b.eq	.LBB16_114
.LBB16_110:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x20]
	cbz	x0, .LBB16_112
// %bb.111:                             //   in Loop: Header=BB16_110 Depth=1
	bl	_ZdlPv
.LBB16_112:                             //   in Loop: Header=BB16_110 Depth=1
	add	x20, x20, #24           // =24
	cmp	x21, x20
	b.ne	.LBB16_110
// %bb.113:
	ldr	x20, [sp, #48]
.LBB16_114:
	cbz	x20, .LBB16_116
// %bb.115:
	mov	x0, x20
	bl	_ZdlPv
.LBB16_116:
	ldr	x0, [sp, #72]
	cbz	x0, .LBB16_118
// %bb.117:
	bl	_ZdlPv
.LBB16_118:
	ldr	w0, [sp, #120]
.Ltmp227:
	bl	close
.Ltmp228:
// %bb.119:
	ldr	x0, [sp, #272]
	cbz	x0, .LBB16_121
// %bb.120:
	bl	_ZdlPv
.LBB16_121:
	ldr	x0, [sp, #248]
	cbz	x0, .LBB16_123
// %bb.122:
	bl	_ZdlPv
.LBB16_123:
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
.LBB16_124:
	mov	x28, #-6148914691236517206
	movk	x28, #2730, lsl #48
	cbnz	x24, .LBB16_88
	b	.LBB16_89
.LBB16_125:
.Ltmp194:
	bl	_ZSt17__throw_bad_allocv
.Ltmp195:
// %bb.126:
.LBB16_127:
.Ltmp202:
	bl	_ZSt17__throw_bad_allocv
.Ltmp203:
// %bb.128:
.LBB16_129:
.Ltmp215:
	bl	_ZSt17__throw_bad_allocv
.Ltmp216:
// %bb.130:
.LBB16_131:
.Ltmp210:
	bl	_ZSt17__throw_bad_allocv
.Ltmp211:
// %bb.132:
.LBB16_133:
.Ltmp196:
	b	.LBB16_166
.LBB16_134:
.Ltmp212:
	mov	x20, x0
	mov	x21, x24
.LBB16_135:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x21]
	cbz	x0, .LBB16_137
// %bb.136:                             //   in Loop: Header=BB16_135 Depth=1
	bl	_ZdlPv
.LBB16_137:                             //   in Loop: Header=BB16_135 Depth=1
	add	x21, x21, #24           // =24
	cmp	x25, x21
	b.ne	.LBB16_135
// %bb.138:
	cbnz	x24, .LBB16_154
	b	.LBB16_167
.LBB16_139:
.Ltmp217:
	mov	x20, x0
	cbnz	x22, .LBB16_152
	b	.LBB16_167
.LBB16_140:
.Ltmp204:
	b	.LBB16_166
.LBB16_141:
.Ltmp229:
.LBB16_142:
	add	x20, sp, #120           // =120
	mov	x19, x0
	add	x0, x20, #152           // =152
	bl	_ZNSt6vectorImSaImEED2Ev
	add	x0, x20, #128           // =128
	bl	_ZNSt6vectorImSaImEED2Ev
	mov	x0, x19
	bl	__clang_call_terminate
.LBB16_143:
.Ltmp223:
	cmp	x24, x25
	mov	x20, x0
	b.eq	.LBB16_148
// %bb.144:
	mov	x21, x24
.LBB16_145:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x21]
	cbz	x0, .LBB16_147
// %bb.146:                             //   in Loop: Header=BB16_145 Depth=1
	bl	_ZdlPv
.LBB16_147:                             //   in Loop: Header=BB16_145 Depth=1
	add	x21, x21, #24           // =24
	cmp	x25, x21
	b.ne	.LBB16_145
.LBB16_148:
	cbnz	x24, .LBB16_150
	b	.LBB16_151
.LBB16_149:
.Ltmp220:
	mov	x20, x0
	cbz	x24, .LBB16_151
.LBB16_150:
	mov	x0, x23
	bl	_ZdlPv
.LBB16_151:
	cbz	x22, .LBB16_167
.LBB16_152:
	mov	x0, x22
	b	.LBB16_155
.LBB16_153:
.Ltmp207:
	mov	x20, x0
	cbz	x24, .LBB16_167
.LBB16_154:
	mov	x0, x23
.LBB16_155:
	bl	_ZdlPv
	b	.LBB16_167
.LBB16_156:
.Ltmp187:
	b	.LBB16_166
.LBB16_157:
.Ltmp184:
	ldr	x8, [sp, #96]
	mov	x20, x0
	cbz	x8, .LBB16_181
// %bb.158:
	mov	x0, x8
	b	.LBB16_180
.LBB16_159:
.Ltmp181:
	b	.LBB16_163
.LBB16_160:
.Ltmp178:
	mov	x20, x0
	mov	x19, x23
	b	.LBB16_181
.LBB16_161:
.Ltmp175:
	mov	x20, x0
	mov	x19, x24
	b	.LBB16_181
.LBB16_162:
.Ltmp172:
.LBB16_163:
	mov	x20, x0
	b	.LBB16_181
.LBB16_164:
.Ltmp169:
	bl	_Unwind_Resume
.LBB16_165:
.Ltmp199:
.LBB16_166:
	mov	x20, x0
.LBB16_167:
	ldp	x21, x22, [sp, #48]
	cmp	x21, x22
	b.eq	.LBB16_172
.LBB16_168:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x21]
	cbz	x0, .LBB16_170
// %bb.169:                             //   in Loop: Header=BB16_168 Depth=1
	bl	_ZdlPv
.LBB16_170:                             //   in Loop: Header=BB16_168 Depth=1
	add	x21, x21, #24           // =24
	cmp	x22, x21
	b.ne	.LBB16_168
// %bb.171:
	ldr	x21, [sp, #48]
.LBB16_172:
	cbz	x21, .LBB16_174
// %bb.173:
	mov	x0, x21
	bl	_ZdlPv
.LBB16_174:
	ldr	x0, [sp, #72]
	cbz	x0, .LBB16_176
// %bb.175:
	bl	_ZdlPv
.LBB16_176:
	ldr	w0, [sp, #120]
.Ltmp224:
	bl	close
.Ltmp225:
// %bb.177:
	ldr	x0, [sp, #272]
	cbz	x0, .LBB16_179
// %bb.178:
	bl	_ZdlPv
.LBB16_179:
	ldr	x0, [sp, #248]
	cbz	x0, .LBB16_181
.LBB16_180:
	bl	_ZdlPv
.LBB16_181:
	mov	x0, x19
	bl	_ZdlPv
	mov	x0, x20
	bl	_Unwind_Resume
.LBB16_182:
.Ltmp226:
	b	.LBB16_142
.Lfunc_end16:
	.size	_Z4testIXadL_Z16simdjson_decoderPjRjjmEEEvPKcc, .Lfunc_end16-_Z4testIXadL_Z16simdjson_decoderPjRjjmEEEvPKcc
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table16:
.Lexception6:
	.byte	255                     // @LPStart Encoding = omit
	.byte	0                       // @TType Encoding = absptr
	.uleb128 .Lttbase2-.Lttbaseref2
.Lttbaseref2:
	.byte	1                       // Call site Encoding = uleb128
	.uleb128 .Lcst_end6-.Lcst_begin6
.Lcst_begin6:
	.uleb128 .Lfunc_begin6-.Lfunc_begin6 // >> Call Site 1 <<
	.uleb128 .Ltmp167-.Lfunc_begin6 //   Call between .Lfunc_begin6 and .Ltmp167
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp167-.Lfunc_begin6 // >> Call Site 2 <<
	.uleb128 .Ltmp168-.Ltmp167      //   Call between .Ltmp167 and .Ltmp168
	.uleb128 .Ltmp169-.Lfunc_begin6 //     jumps to .Ltmp169
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp170-.Lfunc_begin6 // >> Call Site 3 <<
	.uleb128 .Ltmp171-.Ltmp170      //   Call between .Ltmp170 and .Ltmp171
	.uleb128 .Ltmp172-.Lfunc_begin6 //     jumps to .Ltmp172
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp173-.Lfunc_begin6 // >> Call Site 4 <<
	.uleb128 .Ltmp174-.Ltmp173      //   Call between .Ltmp173 and .Ltmp174
	.uleb128 .Ltmp175-.Lfunc_begin6 //     jumps to .Ltmp175
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp176-.Lfunc_begin6 // >> Call Site 5 <<
	.uleb128 .Ltmp177-.Ltmp176      //   Call between .Ltmp176 and .Ltmp177
	.uleb128 .Ltmp178-.Lfunc_begin6 //     jumps to .Ltmp178
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp179-.Lfunc_begin6 // >> Call Site 6 <<
	.uleb128 .Ltmp180-.Ltmp179      //   Call between .Ltmp179 and .Ltmp180
	.uleb128 .Ltmp181-.Lfunc_begin6 //     jumps to .Ltmp181
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp182-.Lfunc_begin6 // >> Call Site 7 <<
	.uleb128 .Ltmp183-.Ltmp182      //   Call between .Ltmp182 and .Ltmp183
	.uleb128 .Ltmp184-.Lfunc_begin6 //     jumps to .Ltmp184
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp185-.Lfunc_begin6 // >> Call Site 8 <<
	.uleb128 .Ltmp186-.Ltmp185      //   Call between .Ltmp185 and .Ltmp186
	.uleb128 .Ltmp187-.Lfunc_begin6 //     jumps to .Ltmp187
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp186-.Lfunc_begin6 // >> Call Site 9 <<
	.uleb128 .Ltmp188-.Ltmp186      //   Call between .Ltmp186 and .Ltmp188
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp188-.Lfunc_begin6 // >> Call Site 10 <<
	.uleb128 .Ltmp198-.Ltmp188      //   Call between .Ltmp188 and .Ltmp198
	.uleb128 .Ltmp199-.Lfunc_begin6 //     jumps to .Ltmp199
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp198-.Lfunc_begin6 // >> Call Site 11 <<
	.uleb128 .Ltmp200-.Ltmp198      //   Call between .Ltmp198 and .Ltmp200
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp200-.Lfunc_begin6 // >> Call Site 12 <<
	.uleb128 .Ltmp201-.Ltmp200      //   Call between .Ltmp200 and .Ltmp201
	.uleb128 .Ltmp204-.Lfunc_begin6 //     jumps to .Ltmp204
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp205-.Lfunc_begin6 // >> Call Site 13 <<
	.uleb128 .Ltmp206-.Ltmp205      //   Call between .Ltmp205 and .Ltmp206
	.uleb128 .Ltmp207-.Lfunc_begin6 //     jumps to .Ltmp207
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp208-.Lfunc_begin6 // >> Call Site 14 <<
	.uleb128 .Ltmp209-.Ltmp208      //   Call between .Ltmp208 and .Ltmp209
	.uleb128 .Ltmp212-.Lfunc_begin6 //     jumps to .Ltmp212
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp209-.Lfunc_begin6 // >> Call Site 15 <<
	.uleb128 .Ltmp213-.Ltmp209      //   Call between .Ltmp209 and .Ltmp213
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp213-.Lfunc_begin6 // >> Call Site 16 <<
	.uleb128 .Ltmp214-.Ltmp213      //   Call between .Ltmp213 and .Ltmp214
	.uleb128 .Ltmp217-.Lfunc_begin6 //     jumps to .Ltmp217
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp218-.Lfunc_begin6 // >> Call Site 17 <<
	.uleb128 .Ltmp219-.Ltmp218      //   Call between .Ltmp218 and .Ltmp219
	.uleb128 .Ltmp220-.Lfunc_begin6 //     jumps to .Ltmp220
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp221-.Lfunc_begin6 // >> Call Site 18 <<
	.uleb128 .Ltmp222-.Ltmp221      //   Call between .Ltmp221 and .Ltmp222
	.uleb128 .Ltmp223-.Lfunc_begin6 //     jumps to .Ltmp223
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp227-.Lfunc_begin6 // >> Call Site 19 <<
	.uleb128 .Ltmp228-.Ltmp227      //   Call between .Ltmp227 and .Ltmp228
	.uleb128 .Ltmp229-.Lfunc_begin6 //     jumps to .Ltmp229
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp194-.Lfunc_begin6 // >> Call Site 20 <<
	.uleb128 .Ltmp195-.Ltmp194      //   Call between .Ltmp194 and .Ltmp195
	.uleb128 .Ltmp196-.Lfunc_begin6 //     jumps to .Ltmp196
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp202-.Lfunc_begin6 // >> Call Site 21 <<
	.uleb128 .Ltmp203-.Ltmp202      //   Call between .Ltmp202 and .Ltmp203
	.uleb128 .Ltmp204-.Lfunc_begin6 //     jumps to .Ltmp204
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp215-.Lfunc_begin6 // >> Call Site 22 <<
	.uleb128 .Ltmp216-.Ltmp215      //   Call between .Ltmp215 and .Ltmp216
	.uleb128 .Ltmp217-.Lfunc_begin6 //     jumps to .Ltmp217
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp210-.Lfunc_begin6 // >> Call Site 23 <<
	.uleb128 .Ltmp211-.Ltmp210      //   Call between .Ltmp210 and .Ltmp211
	.uleb128 .Ltmp212-.Lfunc_begin6 //     jumps to .Ltmp212
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp211-.Lfunc_begin6 // >> Call Site 24 <<
	.uleb128 .Ltmp224-.Ltmp211      //   Call between .Ltmp211 and .Ltmp224
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp224-.Lfunc_begin6 // >> Call Site 25 <<
	.uleb128 .Ltmp225-.Ltmp224      //   Call between .Ltmp224 and .Ltmp225
	.uleb128 .Ltmp226-.Lfunc_begin6 //     jumps to .Ltmp226
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp225-.Lfunc_begin6 // >> Call Site 26 <<
	.uleb128 .Lfunc_end16-.Ltmp225  //   Call between .Ltmp225 and .Lfunc_end16
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
.LBB17_1:                               // =>This Loop Header: Depth=1
                                        //     Child Loop BB17_2 Depth 2
                                        //     Child Loop BB17_6 Depth 2
	lsl	x19, x21, x20
	add	x0, sp, #16             // =16
	add	x1, sp, #12             // =12
	mov	w2, wzr
	mov	x3, x19
	str	wzr, [sp, #12]
	bl	_Z14faster_decoderPjRjjm
	mov	w8, wzr
.LBB17_2:                               //   Parent Loop BB17_1 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	rbit	x9, x19
	sub	x10, x19, #1            // =1
	clz	x9, x9
	ands	x19, x10, x19
	str	w9, [x23, w8, uxtw #2]
	add	w8, w8, #1              // =1
	b.ne	.LBB17_2
// %bb.3:                               //   in Loop: Header=BB17_1 Depth=1
	ldr	w9, [sp, #12]
	cmp	w9, w8
	b.ne	.LBB17_20
// %bb.4:                               //   in Loop: Header=BB17_1 Depth=1
	cbz	w8, .LBB17_8
// %bb.5:                               //   in Loop: Header=BB17_1 Depth=1
	mov	x9, xzr
	mov	w8, w8
.LBB17_6:                               //   Parent Loop BB17_1 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x10, x9, #2
	ldr	w11, [x22, x10]
	ldr	w10, [x23, x10]
	cmp	w11, w10
	b.ne	.LBB17_20
// %bb.7:                               //   in Loop: Header=BB17_6 Depth=2
	add	x9, x9, #1              // =1
	cmp	x9, x8
	b.lo	.LBB17_6
.LBB17_8:                               //   in Loop: Header=BB17_1 Depth=1
	add	x20, x20, #1            // =1
	cmp	x20, #64                // =64
	b.lo	.LBB17_1
// %bb.9:
	mov	w22, #16960
	mov	x20, xzr
	add	x21, sp, #16            // =16
	movk	w22, #15, lsl #16
	add	x23, sp, #272           // =272
.LBB17_10:                              // =>This Loop Header: Depth=1
                                        //     Child Loop BB17_12 Depth 2
                                        //     Child Loop BB17_16 Depth 2
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
	bl	_Z14faster_decoderPjRjjm
	fmov	d0, x19
	cnt	v0.8b, v0.8b
	uaddlv	h0, v0.8b
	fmov	w8, s0
	cbz	w8, .LBB17_13
// %bb.11:                              //   in Loop: Header=BB17_10 Depth=1
	add	w9, w8, #1              // =1
	add	x10, sp, #272           // =272
.LBB17_12:                              //   Parent Loop BB17_10 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	rbit	x11, x19
	sub	x12, x19, #1            // =1
	sub	w9, w9, #1              // =1
	clz	x11, x11
	cmp	w9, #1                  // =1
	str	w11, [x10], #4
	and	x19, x12, x19
	b.gt	.LBB17_12
.LBB17_13:                              //   in Loop: Header=BB17_10 Depth=1
	ldr	w9, [sp, #12]
	cmp	w9, w8
	b.ne	.LBB17_20
// %bb.14:                              //   in Loop: Header=BB17_10 Depth=1
	cbz	w8, .LBB17_18
// %bb.15:                              //   in Loop: Header=BB17_10 Depth=1
	mov	x9, xzr
.LBB17_16:                              //   Parent Loop BB17_10 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x10, x9, #2
	ldr	w11, [x21, x10]
	ldr	w10, [x23, x10]
	cmp	w11, w10
	b.ne	.LBB17_20
// %bb.17:                              //   in Loop: Header=BB17_16 Depth=2
	add	x9, x9, #1              // =1
	cmp	x9, x8
	b.lo	.LBB17_16
.LBB17_18:                              //   in Loop: Header=BB17_10 Depth=1
	add	x20, x20, #1            // =1
	cmp	x20, x22
	b.lo	.LBB17_10
// %bb.19:
	adrp	x0, .Lstr.48
	add	x0, x0, :lo12:.Lstr.48
	bl	puts
	orr	w0, wzr, #0x1
	b	.LBB17_21
.LBB17_20:
	adrp	x0, .Lstr.52
	add	x0, x0, :lo12:.Lstr.52
	bl	puts
	mov	w0, wzr
.LBB17_21:
	add	sp, sp, #528            // =528
	ldp	x29, x30, [sp, #48]     // 16-byte Folded Reload
	ldp	x20, x19, [sp, #32]     // 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]     // 16-byte Folded Reload
	ldp	x28, x23, [sp], #64     // 16-byte Folded Reload
	ret
.Lfunc_end17:
	.size	_Z4unitIXadL_Z13basic_decoderPjRjjmEEEbv, .Lfunc_end17-_Z4unitIXadL_Z13basic_decoderPjRjjmEEEbv
	.cfi_endproc
                                        // -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3               // -- Begin function _Z4testIXadL_Z13basic_decoderPjRjjmEEEvPKcc
.LCPI18_0:
	.xword	4636737291354636288     // double 100
.LCPI18_1:
	.xword	4634204016564240384     // double 64
	.section	.text._Z4testIXadL_Z13basic_decoderPjRjjmEEEvPKcc,"axG",@progbits,_Z4testIXadL_Z13basic_decoderPjRjjmEEEvPKcc,comdat
	.weak	_Z4testIXadL_Z13basic_decoderPjRjjmEEEvPKcc
	.p2align	2
	.type	_Z4testIXadL_Z13basic_decoderPjRjjmEEEvPKcc,@function
_Z4testIXadL_Z13basic_decoderPjRjjmEEEvPKcc: // @_Z4testIXadL_Z13basic_decoderPjRjjmEEEvPKcc
.Lfunc_begin7:
	.cfi_startproc
	.cfi_personality 0, __gxx_personality_v0
	.cfi_lsda 0, .Lexception7
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
	cbz	x0, .LBB18_8
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
	b.eq	.LBB18_7
// %bb.2:
	mov	x9, xzr
	orr	w10, wzr, #0x1
.LBB18_3:                               // =>This Inner Loop Header: Depth=1
	ldrb	w11, [x21, x9]
	cmp	w11, w19, uxtb
	b.eq	.LBB18_5
// %bb.4:                               //   in Loop: Header=BB18_3 Depth=1
	cmp	w11, #31                // =31
	b.hi	.LBB18_6
.LBB18_5:                               //   in Loop: Header=BB18_3 Depth=1
	lsr	x11, x9, #3
	and	x11, x11, #0x1ffffffffffffff8
	ldr	x12, [x20, x11]
	lsl	x13, x10, x9
	orr	x12, x12, x13
	str	x12, [x20, x11]
.LBB18_6:                               //   in Loop: Header=BB18_3 Depth=1
	add	x9, x9, #1              // =1
	cmp	x8, x9
	b.ne	.LBB18_3
.LBB18_7:
	mov	x0, x21
	bl	_ZdaPv
	b	.LBB18_9
.LBB18_8:
	mov	x20, xzr
                                        // implicit-def: $x22
.LBB18_9:
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
// %bb.10:
	mov	x19, x0
	str	wzr, [x0]
.Ltmp233:
	orr	w0, wzr, #0x8
	bl	_Znwm
.Ltmp234:
// %bb.11:
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
.Ltmp239:
	orr	w0, wzr, #0x20
	bl	_Znwm
.Ltmp240:
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
.Ltmp242:
	mov	w0, #20
	bl	_Znwm
.Ltmp243:
// %bb.14:
	ldr	w9, [x19, #16]
	ldr	q0, [x19]
	add	x8, x0, #20             // =20
	stp	x0, x0, [sp, #96]
	stp	x8, x8, [sp, #104]
	str	w9, [x0, #16]
	str	q0, [x0]
.Ltmp245:
	add	x0, sp, #120            // =120
	add	x1, sp, #96             // =96
	bl	_ZN11LinuxEventsILi0EEC2ESt6vectorIiSaIiEE
.Ltmp246:
// %bb.15:
	ldr	x0, [sp, #96]
	cbz	x0, .LBB18_17
// %bb.16:
	bl	_ZdlPv
.LBB18_17:
	stp	xzr, xzr, [sp, #80]
	stp	xzr, xzr, [sp, #64]
	stp	xzr, xzr, [sp, #48]
.Ltmp248:
	mov	w0, #40
	bl	_Znwm
.Ltmp249:
// %bb.18:
	ldp	x24, x8, [sp, #72]
	mov	x23, x0
	movi	v0.2d, #0000000000000000
	str	xzr, [x0, #32]
	subs	x2, x8, x24
	stp	q0, q0, [x0]
	b.eq	.LBB18_20
// %bb.19:
	mov	x0, x23
	mov	x1, x24
	bl	memmove
.LBB18_20:
	cbz	x24, .LBB18_22
// %bb.21:
	mov	x0, x24
	bl	_ZdlPv
.LBB18_22:
	mov	w26, wzr
	add	x8, x23, #40            // =40
	str	x8, [sp, #88]
	stp	x23, x8, [sp, #72]
.LBB18_23:                              // =>This Loop Header: Depth=1
                                        //     Child Loop BB18_26 Depth 2
                                        //       Child Loop BB18_28 Depth 3
.Ltmp251:
	add	x0, sp, #120            // =120
	bl	_ZN11LinuxEventsILi0EE5startEv
.Ltmp252:
// %bb.24:                              //   in Loop: Header=BB18_23 Depth=1
	cbz	x22, .LBB18_31
// %bb.25:                              //   in Loop: Header=BB18_23 Depth=1
	mov	x8, xzr
	mov	w23, wzr
.LBB18_26:                              //   Parent Loop BB18_23 Depth=1
                                        // =>  This Loop Header: Depth=2
                                        //       Child Loop BB18_28 Depth 3
	ldr	x9, [x20, x8, lsl #3]
	fmov	d0, x9
	cnt	v0.8b, v0.8b
	uaddlv	h0, v0.8b
	fmov	w11, s0
	cbz	w11, .LBB18_30
// %bb.27:                              //   in Loop: Header=BB18_26 Depth=2
	add	w10, w23, w11
	add	w11, w11, #1            // =1
.LBB18_28:                              //   Parent Loop BB18_23 Depth=1
                                        //     Parent Loop BB18_26 Depth=2
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
	b.gt	.LBB18_28
// %bb.29:                              //   in Loop: Header=BB18_26 Depth=2
	mov	w23, w10
.LBB18_30:                              //   in Loop: Header=BB18_26 Depth=2
	add	x8, x8, #1              // =1
	cmp	x8, x22
	b.ne	.LBB18_26
	b	.LBB18_32
.LBB18_31:                              //   in Loop: Header=BB18_23 Depth=1
	mov	w23, wzr
.LBB18_32:                              //   in Loop: Header=BB18_23 Depth=1
.Ltmp253:
	add	x0, sp, #120            // =120
	add	x1, sp, #72             // =72
	bl	_ZN11LinuxEventsILi0EE3endERSt6vectorIySaIyEE
.Ltmp254:
// %bb.33:                              //   in Loop: Header=BB18_23 Depth=1
	ldp	x24, x8, [sp, #56]
	cmp	x24, x8
	b.eq	.LBB18_38
// %bb.34:                              //   in Loop: Header=BB18_23 Depth=1
	ldp	x9, x8, [sp, #72]
	stp	xzr, xzr, [x24]
	str	xzr, [x24, #16]
	subs	x0, x8, x9
	asr	x27, x0, #3
	b.eq	.LBB18_39
// %bb.35:                              //   in Loop: Header=BB18_23 Depth=1
	lsr	x8, x27, #61
	cbnz	x8, .LBB18_127
// %bb.36:                              //   in Loop: Header=BB18_23 Depth=1
.Ltmp255:
	bl	_Znwm
.Ltmp256:
// %bb.37:                              //   in Loop: Header=BB18_23 Depth=1
	mov	x28, x0
	b	.LBB18_40
.LBB18_38:                              //   in Loop: Header=BB18_23 Depth=1
.Ltmp260:
	add	x0, sp, #48             // =48
	add	x2, sp, #72             // =72
	mov	x1, x24
	bl	_ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_
.Ltmp261:
	b	.LBB18_43
.LBB18_39:                              //   in Loop: Header=BB18_23 Depth=1
	mov	x28, xzr
.LBB18_40:                              //   in Loop: Header=BB18_23 Depth=1
	add	x8, x28, x27, lsl #3
	stp	x28, x0, [x24]
	str	x8, [x24, #16]
	ldp	x1, x8, [sp, #72]
	subs	x2, x8, x1
	asr	x27, x2, #3
	b.eq	.LBB18_42
// %bb.41:                              //   in Loop: Header=BB18_23 Depth=1
	bl	memmove
.LBB18_42:                              //   in Loop: Header=BB18_23 Depth=1
	add	x8, x28, x27, lsl #3
	str	x8, [x24, #8]
	ldr	x8, [sp, #56]
	add	x8, x8, #24             // =24
	str	x8, [sp, #56]
.LBB18_43:                              //   in Loop: Header=BB18_23 Depth=1
	add	w26, w26, #1            // =1
	cmp	w26, #10                // =10
	b.lo	.LBB18_23
// %bb.44:
	ldr	w8, [x21]
	cbnz	w8, .LBB18_46
// %bb.45:
	adrp	x0, .L.str.3
	add	x0, x0, :lo12:.L.str.3
	bl	printf
.LBB18_46:
	adrp	x8, .LCPI18_0
	ldr	d0, [x8, :lo12:.LCPI18_0]
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
	adrp	x8, .LCPI18_1
	ldr	d0, [x8, :lo12:.LCPI18_1]
	ucvtf	d10, x22
	adrp	x0, .L.str.5
	add	x0, x0, :lo12:.L.str.5
	fmul	d0, d10, d0
	fdiv	d0, d0, d9
	bl	printf
	ldp	x0, x1, [sp, #48]
	subs	x8, x1, x0
	b.eq	.LBB18_50
// %bb.47:
	mov	x10, #-6148914691236517206
	asr	x9, x8, #3
	movk	x10, #43691
	mul	x9, x9, x10
	cmp	x9, x28
	b.hi	.LBB18_129
// %bb.48:
.Ltmp263:
	mov	x0, x8
	bl	_Znwm
.Ltmp264:
// %bb.49:
	mov	x23, x0
	ldp	x0, x1, [sp, #48]
	mov	x24, x23
	b	.LBB18_51
.LBB18_50:
	mov	x23, xzr
	mov	x24, xzr
.LBB18_51:
.Ltmp268:
	mov	x2, x24
	bl	_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_
.Ltmp269:
// %bb.52:
	mov	x25, x0
	subs	x27, x0, x24
	b.eq	.LBB18_58
// %bb.53:
	ldp	x8, x9, [x24]
	subs	x0, x9, x8
	b.eq	.LBB18_59
// %bb.54:
	asr	x8, x0, #3
	lsr	x8, x8, #61
	cbnz	x8, .LBB18_133
// %bb.55:
.Ltmp271:
	bl	_Znwm
.Ltmp272:
// %bb.56:
	ldp	x1, x8, [x24]
	mov	x26, x0
	subs	x2, x8, x1
	b.eq	.LBB18_60
// %bb.57:
	mov	x0, x26
	asr	x28, x2, #3
	bl	memmove
	mov	w8, wzr
	b	.LBB18_61
.LBB18_58:
	mov	x22, xzr
	cbnz	x24, .LBB18_90
	b	.LBB18_91
.LBB18_59:
	mov	x22, xzr
	mov	x26, xzr
	mov	x28, xzr
	orr	w8, wzr, #0x1
	b	.LBB18_62
.LBB18_60:
	mov	x28, xzr
	orr	w8, wzr, #0x1
.LBB18_61:
	mov	x22, x26
.LBB18_62:
	mov	x10, #-6148914691236517206
	asr	x9, x27, #3
	movk	x10, #43691
	mul	x9, x9, x10
	cmp	x9, #2                  // =2
	b.lo	.LBB18_85
// %bb.63:
	cmp	x28, #1                 // =1
	csinc	x12, x28, xzr, hi
	add	x10, x22, #16           // =16
	orr	w11, wzr, #0x1
	add	x13, x22, x12, lsl #3
	and	x14, x12, #0xfffffffffffffffc
	orr	w15, wzr, #0x18
	b	.LBB18_75
.LBB18_64:                              //   in Loop: Header=BB18_75 Depth=1
	add	x17, x16, #16           // =16
	mov	x18, x10
	mov	x0, x14
.LBB18_65:                              //   Parent Loop BB18_75 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldp	q1, q0, [x17, #-16]
	ldp	q3, q2, [x18, #-16]
	cmhi	v3.2d, v3.2d, v1.2d
	xtn	v3.2s, v3.2d
	fmov	w1, s3
	tbz	w1, #0, .LBB18_67
// %bb.66:                              //   in Loop: Header=BB18_65 Depth=2
	stur	d1, [x18, #-16]
.LBB18_67:                              //   in Loop: Header=BB18_65 Depth=2
	mov	w1, v3.s[1]
	tbz	w1, #0, .LBB18_69
// %bb.68:                              //   in Loop: Header=BB18_65 Depth=2
	sub	x1, x18, #8             // =8
	st1	{ v1.d }[1], [x1]
.LBB18_69:                              //   in Loop: Header=BB18_65 Depth=2
	cmhi	v1.2d, v2.2d, v0.2d
	xtn	v1.2s, v1.2d
	fmov	w1, s1
	tbz	w1, #0, .LBB18_71
// %bb.70:                              //   in Loop: Header=BB18_65 Depth=2
	str	d0, [x18]
.LBB18_71:                              //   in Loop: Header=BB18_65 Depth=2
	mov	w1, v1.s[1]
	tbz	w1, #0, .LBB18_73
// %bb.72:                              //   in Loop: Header=BB18_65 Depth=2
	add	x1, x18, #8             // =8
	st1	{ v0.d }[1], [x1]
.LBB18_73:                              //   in Loop: Header=BB18_65 Depth=2
	subs	x0, x0, #4              // =4
	add	x18, x18, #32           // =32
	add	x17, x17, #32           // =32
	b.ne	.LBB18_65
// %bb.74:                              //   in Loop: Header=BB18_75 Depth=1
	cmp	x12, x14
	mov	x17, x14
	b.ne	.LBB18_80
	b	.LBB18_83
.LBB18_75:                              // =>This Loop Header: Depth=1
                                        //     Child Loop BB18_65 Depth 2
                                        //     Child Loop BB18_80 Depth 2
	tbnz	w8, #0, .LBB18_83
// %bb.76:                              //   in Loop: Header=BB18_75 Depth=1
	mul	x16, x11, x15
	ldr	x16, [x24, x16]
	cmp	x12, #3                 // =3
	b.ls	.LBB18_79
// %bb.77:                              //   in Loop: Header=BB18_75 Depth=1
	add	x17, x16, x12, lsl #3
	cmp	x26, x17
	b.hs	.LBB18_64
// %bb.78:                              //   in Loop: Header=BB18_75 Depth=1
	cmp	x16, x13
	b.hs	.LBB18_64
.LBB18_79:                              //   in Loop: Header=BB18_75 Depth=1
	mov	x17, xzr
.LBB18_80:                              //   Parent Loop BB18_75 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x0, x17, #3
	ldr	x18, [x16, x0]
	ldr	x0, [x22, x0]
	cmp	x18, x0
	b.hs	.LBB18_82
// %bb.81:                              //   in Loop: Header=BB18_80 Depth=2
	str	x18, [x22, x17, lsl #3]
.LBB18_82:                              //   in Loop: Header=BB18_80 Depth=2
	add	x17, x17, #1            // =1
	cmp	x17, x28
	b.lo	.LBB18_80
.LBB18_83:                              //   in Loop: Header=BB18_75 Depth=1
	add	x11, x11, #1            // =1
	cmp	x11, x9
	b.ne	.LBB18_75
// %bb.84:
	cmp	x25, x24
	b.eq	.LBB18_126
.LBB18_85:
	mov	x28, #-6148914691236517206
	mov	x26, x24
	movk	x28, #2730, lsl #48
.LBB18_86:                              // =>This Inner Loop Header: Depth=1
	ldr	x0, [x26]
	cbz	x0, .LBB18_88
// %bb.87:                              //   in Loop: Header=BB18_86 Depth=1
	bl	_ZdlPv
.LBB18_88:                              //   in Loop: Header=BB18_86 Depth=1
	add	x26, x26, #24           // =24
	cmp	x25, x26
	b.ne	.LBB18_86
// %bb.89:
	cbz	x24, .LBB18_91
.LBB18_90:
	mov	x0, x23
	bl	_ZdlPv
.LBB18_91:
	ldp	x0, x1, [sp, #48]
	mov	x9, #-6148914691236517206
	movk	x9, #43691
	stp	xzr, xzr, [sp]
	subs	x8, x1, x0
	asr	x10, x8, #3
	mul	x25, x10, x9
	str	xzr, [sp, #16]
	b.eq	.LBB18_95
// %bb.92:
	cmp	x25, x28
	b.hi	.LBB18_131
// %bb.93:
.Ltmp276:
	mov	x0, x8
	bl	_Znwm
.Ltmp277:
// %bb.94:
	mov	x23, x0
	ldp	x0, x1, [sp, #48]
	mov	x24, x23
	b	.LBB18_96
.LBB18_95:
	mov	x23, xzr
	mov	x24, xzr
.LBB18_96:
	orr	w8, wzr, #0x18
	madd	x8, x25, x8, x24
	stp	x23, x23, [sp]
	str	x8, [sp, #16]
.Ltmp281:
	mov	x2, x24
	bl	_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_
.Ltmp282:
// %bb.97:
	mov	x25, x0
	str	x0, [sp, #8]
.Ltmp284:
	add	x8, sp, #24             // =24
	mov	x0, sp
	bl	_Z16compute_averagesSt6vectorIS_IySaIyEESaIS1_EE
.Ltmp285:
// %bb.98:
	cmp	x24, x25
	b.eq	.LBB18_103
// %bb.99:
	mov	x26, x24
.LBB18_100:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x26]
	cbz	x0, .LBB18_102
// %bb.101:                             //   in Loop: Header=BB18_100 Depth=1
	bl	_ZdlPv
.LBB18_102:                             //   in Loop: Header=BB18_100 Depth=1
	add	x26, x26, #24           // =24
	cmp	x25, x26
	b.ne	.LBB18_100
.LBB18_103:
	cbz	x24, .LBB18_105
// %bb.104:
	mov	x0, x23
	bl	_ZdlPv
.LBB18_105:
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
	cbz	x20, .LBB18_107
// %bb.106:
	mov	x0, x20
	bl	_ZdaPv
.LBB18_107:
	mov	x0, x21
	bl	_ZdaPv
	cbz	x23, .LBB18_109
// %bb.108:
	mov	x0, x23
	bl	_ZdlPv
.LBB18_109:
	cbz	x22, .LBB18_111
// %bb.110:
	mov	x0, x22
	bl	_ZdlPv
.LBB18_111:
	ldp	x20, x21, [sp, #48]
	cmp	x20, x21
	b.eq	.LBB18_116
.LBB18_112:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x20]
	cbz	x0, .LBB18_114
// %bb.113:                             //   in Loop: Header=BB18_112 Depth=1
	bl	_ZdlPv
.LBB18_114:                             //   in Loop: Header=BB18_112 Depth=1
	add	x20, x20, #24           // =24
	cmp	x21, x20
	b.ne	.LBB18_112
// %bb.115:
	ldr	x20, [sp, #48]
.LBB18_116:
	cbz	x20, .LBB18_118
// %bb.117:
	mov	x0, x20
	bl	_ZdlPv
.LBB18_118:
	ldr	x0, [sp, #72]
	cbz	x0, .LBB18_120
// %bb.119:
	bl	_ZdlPv
.LBB18_120:
	ldr	w0, [sp, #120]
.Ltmp290:
	bl	close
.Ltmp291:
// %bb.121:
	ldr	x0, [sp, #272]
	cbz	x0, .LBB18_123
// %bb.122:
	bl	_ZdlPv
.LBB18_123:
	ldr	x0, [sp, #248]
	cbz	x0, .LBB18_125
// %bb.124:
	bl	_ZdlPv
.LBB18_125:
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
.LBB18_126:
	mov	x28, #-6148914691236517206
	movk	x28, #2730, lsl #48
	cbnz	x24, .LBB18_90
	b	.LBB18_91
.LBB18_127:
.Ltmp257:
	bl	_ZSt17__throw_bad_allocv
.Ltmp258:
// %bb.128:
.LBB18_129:
.Ltmp265:
	bl	_ZSt17__throw_bad_allocv
.Ltmp266:
// %bb.130:
.LBB18_131:
.Ltmp278:
	bl	_ZSt17__throw_bad_allocv
.Ltmp279:
// %bb.132:
.LBB18_133:
.Ltmp273:
	bl	_ZSt17__throw_bad_allocv
.Ltmp274:
// %bb.134:
.LBB18_135:
.Ltmp259:
	b	.LBB18_168
.LBB18_136:
.Ltmp275:
	mov	x20, x0
	mov	x21, x24
.LBB18_137:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x21]
	cbz	x0, .LBB18_139
// %bb.138:                             //   in Loop: Header=BB18_137 Depth=1
	bl	_ZdlPv
.LBB18_139:                             //   in Loop: Header=BB18_137 Depth=1
	add	x21, x21, #24           // =24
	cmp	x25, x21
	b.ne	.LBB18_137
// %bb.140:
	cbnz	x24, .LBB18_156
	b	.LBB18_169
.LBB18_141:
.Ltmp280:
	mov	x20, x0
	cbnz	x22, .LBB18_154
	b	.LBB18_169
.LBB18_142:
.Ltmp267:
	b	.LBB18_168
.LBB18_143:
.Ltmp292:
.LBB18_144:
	add	x20, sp, #120           // =120
	mov	x19, x0
	add	x0, x20, #152           // =152
	bl	_ZNSt6vectorImSaImEED2Ev
	add	x0, x20, #128           // =128
	bl	_ZNSt6vectorImSaImEED2Ev
	mov	x0, x19
	bl	__clang_call_terminate
.LBB18_145:
.Ltmp286:
	cmp	x24, x25
	mov	x20, x0
	b.eq	.LBB18_150
// %bb.146:
	mov	x21, x24
.LBB18_147:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x21]
	cbz	x0, .LBB18_149
// %bb.148:                             //   in Loop: Header=BB18_147 Depth=1
	bl	_ZdlPv
.LBB18_149:                             //   in Loop: Header=BB18_147 Depth=1
	add	x21, x21, #24           // =24
	cmp	x25, x21
	b.ne	.LBB18_147
.LBB18_150:
	cbnz	x24, .LBB18_152
	b	.LBB18_153
.LBB18_151:
.Ltmp283:
	mov	x20, x0
	cbz	x24, .LBB18_153
.LBB18_152:
	mov	x0, x23
	bl	_ZdlPv
.LBB18_153:
	cbz	x22, .LBB18_169
.LBB18_154:
	mov	x0, x22
	b	.LBB18_157
.LBB18_155:
.Ltmp270:
	mov	x20, x0
	cbz	x24, .LBB18_169
.LBB18_156:
	mov	x0, x23
.LBB18_157:
	bl	_ZdlPv
	b	.LBB18_169
.LBB18_158:
.Ltmp250:
	b	.LBB18_168
.LBB18_159:
.Ltmp247:
	ldr	x8, [sp, #96]
	mov	x20, x0
	cbz	x8, .LBB18_183
// %bb.160:
	mov	x0, x8
	b	.LBB18_182
.LBB18_161:
.Ltmp244:
	b	.LBB18_165
.LBB18_162:
.Ltmp241:
	mov	x20, x0
	mov	x19, x23
	b	.LBB18_183
.LBB18_163:
.Ltmp238:
	mov	x20, x0
	mov	x19, x24
	b	.LBB18_183
.LBB18_164:
.Ltmp235:
.LBB18_165:
	mov	x20, x0
	b	.LBB18_183
.LBB18_166:
.Ltmp232:
	bl	_Unwind_Resume
.LBB18_167:
.Ltmp262:
.LBB18_168:
	mov	x20, x0
.LBB18_169:
	ldp	x21, x22, [sp, #48]
	cmp	x21, x22
	b.eq	.LBB18_174
.LBB18_170:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x21]
	cbz	x0, .LBB18_172
// %bb.171:                             //   in Loop: Header=BB18_170 Depth=1
	bl	_ZdlPv
.LBB18_172:                             //   in Loop: Header=BB18_170 Depth=1
	add	x21, x21, #24           // =24
	cmp	x22, x21
	b.ne	.LBB18_170
// %bb.173:
	ldr	x21, [sp, #48]
.LBB18_174:
	cbz	x21, .LBB18_176
// %bb.175:
	mov	x0, x21
	bl	_ZdlPv
.LBB18_176:
	ldr	x0, [sp, #72]
	cbz	x0, .LBB18_178
// %bb.177:
	bl	_ZdlPv
.LBB18_178:
	ldr	w0, [sp, #120]
.Ltmp287:
	bl	close
.Ltmp288:
// %bb.179:
	ldr	x0, [sp, #272]
	cbz	x0, .LBB18_181
// %bb.180:
	bl	_ZdlPv
.LBB18_181:
	ldr	x0, [sp, #248]
	cbz	x0, .LBB18_183
.LBB18_182:
	bl	_ZdlPv
.LBB18_183:
	mov	x0, x19
	bl	_ZdlPv
	mov	x0, x20
	bl	_Unwind_Resume
.LBB18_184:
.Ltmp289:
	b	.LBB18_144
.Lfunc_end18:
	.size	_Z4testIXadL_Z13basic_decoderPjRjjmEEEvPKcc, .Lfunc_end18-_Z4testIXadL_Z13basic_decoderPjRjjmEEEvPKcc
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table18:
.Lexception7:
	.byte	255                     // @LPStart Encoding = omit
	.byte	0                       // @TType Encoding = absptr
	.uleb128 .Lttbase3-.Lttbaseref3
.Lttbaseref3:
	.byte	1                       // Call site Encoding = uleb128
	.uleb128 .Lcst_end7-.Lcst_begin7
.Lcst_begin7:
	.uleb128 .Lfunc_begin7-.Lfunc_begin7 // >> Call Site 1 <<
	.uleb128 .Ltmp230-.Lfunc_begin7 //   Call between .Lfunc_begin7 and .Ltmp230
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp230-.Lfunc_begin7 // >> Call Site 2 <<
	.uleb128 .Ltmp231-.Ltmp230      //   Call between .Ltmp230 and .Ltmp231
	.uleb128 .Ltmp232-.Lfunc_begin7 //     jumps to .Ltmp232
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp233-.Lfunc_begin7 // >> Call Site 3 <<
	.uleb128 .Ltmp234-.Ltmp233      //   Call between .Ltmp233 and .Ltmp234
	.uleb128 .Ltmp235-.Lfunc_begin7 //     jumps to .Ltmp235
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp236-.Lfunc_begin7 // >> Call Site 4 <<
	.uleb128 .Ltmp237-.Ltmp236      //   Call between .Ltmp236 and .Ltmp237
	.uleb128 .Ltmp238-.Lfunc_begin7 //     jumps to .Ltmp238
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp239-.Lfunc_begin7 // >> Call Site 5 <<
	.uleb128 .Ltmp240-.Ltmp239      //   Call between .Ltmp239 and .Ltmp240
	.uleb128 .Ltmp241-.Lfunc_begin7 //     jumps to .Ltmp241
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp242-.Lfunc_begin7 // >> Call Site 6 <<
	.uleb128 .Ltmp243-.Ltmp242      //   Call between .Ltmp242 and .Ltmp243
	.uleb128 .Ltmp244-.Lfunc_begin7 //     jumps to .Ltmp244
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp245-.Lfunc_begin7 // >> Call Site 7 <<
	.uleb128 .Ltmp246-.Ltmp245      //   Call between .Ltmp245 and .Ltmp246
	.uleb128 .Ltmp247-.Lfunc_begin7 //     jumps to .Ltmp247
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp248-.Lfunc_begin7 // >> Call Site 8 <<
	.uleb128 .Ltmp249-.Ltmp248      //   Call between .Ltmp248 and .Ltmp249
	.uleb128 .Ltmp250-.Lfunc_begin7 //     jumps to .Ltmp250
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp249-.Lfunc_begin7 // >> Call Site 9 <<
	.uleb128 .Ltmp251-.Ltmp249      //   Call between .Ltmp249 and .Ltmp251
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp251-.Lfunc_begin7 // >> Call Site 10 <<
	.uleb128 .Ltmp261-.Ltmp251      //   Call between .Ltmp251 and .Ltmp261
	.uleb128 .Ltmp262-.Lfunc_begin7 //     jumps to .Ltmp262
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp261-.Lfunc_begin7 // >> Call Site 11 <<
	.uleb128 .Ltmp263-.Ltmp261      //   Call between .Ltmp261 and .Ltmp263
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp263-.Lfunc_begin7 // >> Call Site 12 <<
	.uleb128 .Ltmp264-.Ltmp263      //   Call between .Ltmp263 and .Ltmp264
	.uleb128 .Ltmp267-.Lfunc_begin7 //     jumps to .Ltmp267
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp268-.Lfunc_begin7 // >> Call Site 13 <<
	.uleb128 .Ltmp269-.Ltmp268      //   Call between .Ltmp268 and .Ltmp269
	.uleb128 .Ltmp270-.Lfunc_begin7 //     jumps to .Ltmp270
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp271-.Lfunc_begin7 // >> Call Site 14 <<
	.uleb128 .Ltmp272-.Ltmp271      //   Call between .Ltmp271 and .Ltmp272
	.uleb128 .Ltmp275-.Lfunc_begin7 //     jumps to .Ltmp275
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp272-.Lfunc_begin7 // >> Call Site 15 <<
	.uleb128 .Ltmp276-.Ltmp272      //   Call between .Ltmp272 and .Ltmp276
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp276-.Lfunc_begin7 // >> Call Site 16 <<
	.uleb128 .Ltmp277-.Ltmp276      //   Call between .Ltmp276 and .Ltmp277
	.uleb128 .Ltmp280-.Lfunc_begin7 //     jumps to .Ltmp280
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp281-.Lfunc_begin7 // >> Call Site 17 <<
	.uleb128 .Ltmp282-.Ltmp281      //   Call between .Ltmp281 and .Ltmp282
	.uleb128 .Ltmp283-.Lfunc_begin7 //     jumps to .Ltmp283
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp284-.Lfunc_begin7 // >> Call Site 18 <<
	.uleb128 .Ltmp285-.Ltmp284      //   Call between .Ltmp284 and .Ltmp285
	.uleb128 .Ltmp286-.Lfunc_begin7 //     jumps to .Ltmp286
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp290-.Lfunc_begin7 // >> Call Site 19 <<
	.uleb128 .Ltmp291-.Ltmp290      //   Call between .Ltmp290 and .Ltmp291
	.uleb128 .Ltmp292-.Lfunc_begin7 //     jumps to .Ltmp292
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp257-.Lfunc_begin7 // >> Call Site 20 <<
	.uleb128 .Ltmp258-.Ltmp257      //   Call between .Ltmp257 and .Ltmp258
	.uleb128 .Ltmp259-.Lfunc_begin7 //     jumps to .Ltmp259
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp265-.Lfunc_begin7 // >> Call Site 21 <<
	.uleb128 .Ltmp266-.Ltmp265      //   Call between .Ltmp265 and .Ltmp266
	.uleb128 .Ltmp267-.Lfunc_begin7 //     jumps to .Ltmp267
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp278-.Lfunc_begin7 // >> Call Site 22 <<
	.uleb128 .Ltmp279-.Ltmp278      //   Call between .Ltmp278 and .Ltmp279
	.uleb128 .Ltmp280-.Lfunc_begin7 //     jumps to .Ltmp280
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp273-.Lfunc_begin7 // >> Call Site 23 <<
	.uleb128 .Ltmp274-.Ltmp273      //   Call between .Ltmp273 and .Ltmp274
	.uleb128 .Ltmp275-.Lfunc_begin7 //     jumps to .Ltmp275
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp274-.Lfunc_begin7 // >> Call Site 24 <<
	.uleb128 .Ltmp287-.Ltmp274      //   Call between .Ltmp274 and .Ltmp287
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp287-.Lfunc_begin7 // >> Call Site 25 <<
	.uleb128 .Ltmp288-.Ltmp287      //   Call between .Ltmp287 and .Ltmp288
	.uleb128 .Ltmp289-.Lfunc_begin7 //     jumps to .Ltmp289
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp288-.Lfunc_begin7 // >> Call Site 26 <<
	.uleb128 .Lfunc_end18-.Ltmp288  //   Call between .Ltmp288 and .Lfunc_end18
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
.Lcst_end7:
	.byte	1                       // >> Action Record 1 <<
                                        //   Catch TypeInfo 1
	.byte	0                       //   No further actions
	.p2align	2
                                        // >> Catch TypeInfos <<
	.xword	0                       // TypeInfo 1
.Lttbase3:
	.p2align	2
                                        // -- End function
	.section	.text._Z4unitIXadL_Z14faster_decoderPjRjjmEEEbv,"axG",@progbits,_Z4unitIXadL_Z14faster_decoderPjRjjmEEEbv,comdat
	.weak	_Z4unitIXadL_Z14faster_decoderPjRjjmEEEbv // -- Begin function _Z4unitIXadL_Z14faster_decoderPjRjjmEEEbv
	.p2align	2
	.type	_Z4unitIXadL_Z14faster_decoderPjRjjmEEEbv,@function
_Z4unitIXadL_Z14faster_decoderPjRjjmEEEbv: // @_Z4unitIXadL_Z14faster_decoderPjRjjmEEEbv
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
.LBB19_1:                               // =>This Loop Header: Depth=1
                                        //     Child Loop BB19_2 Depth 2
                                        //     Child Loop BB19_6 Depth 2
	lsl	x19, x21, x20
	add	x0, sp, #16             // =16
	add	x1, sp, #12             // =12
	mov	w2, wzr
	mov	x3, x19
	str	wzr, [sp, #12]
	bl	_Z14faster_decoderPjRjjm
	mov	w8, wzr
.LBB19_2:                               //   Parent Loop BB19_1 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	rbit	x9, x19
	sub	x10, x19, #1            // =1
	clz	x9, x9
	ands	x19, x10, x19
	str	w9, [x23, w8, uxtw #2]
	add	w8, w8, #1              // =1
	b.ne	.LBB19_2
// %bb.3:                               //   in Loop: Header=BB19_1 Depth=1
	ldr	w9, [sp, #12]
	cmp	w9, w8
	b.ne	.LBB19_20
// %bb.4:                               //   in Loop: Header=BB19_1 Depth=1
	cbz	w8, .LBB19_8
// %bb.5:                               //   in Loop: Header=BB19_1 Depth=1
	mov	x9, xzr
	mov	w8, w8
.LBB19_6:                               //   Parent Loop BB19_1 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x10, x9, #2
	ldr	w11, [x22, x10]
	ldr	w10, [x23, x10]
	cmp	w11, w10
	b.ne	.LBB19_20
// %bb.7:                               //   in Loop: Header=BB19_6 Depth=2
	add	x9, x9, #1              // =1
	cmp	x9, x8
	b.lo	.LBB19_6
.LBB19_8:                               //   in Loop: Header=BB19_1 Depth=1
	add	x20, x20, #1            // =1
	cmp	x20, #64                // =64
	b.lo	.LBB19_1
// %bb.9:
	mov	w22, #16960
	mov	x20, xzr
	add	x21, sp, #16            // =16
	movk	w22, #15, lsl #16
	add	x23, sp, #272           // =272
.LBB19_10:                              // =>This Loop Header: Depth=1
                                        //     Child Loop BB19_12 Depth 2
                                        //     Child Loop BB19_16 Depth 2
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
	bl	_Z14faster_decoderPjRjjm
	fmov	d0, x19
	cnt	v0.8b, v0.8b
	uaddlv	h0, v0.8b
	fmov	w8, s0
	cbz	w8, .LBB19_13
// %bb.11:                              //   in Loop: Header=BB19_10 Depth=1
	add	w9, w8, #1              // =1
	add	x10, sp, #272           // =272
.LBB19_12:                              //   Parent Loop BB19_10 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	rbit	x11, x19
	sub	x12, x19, #1            // =1
	sub	w9, w9, #1              // =1
	clz	x11, x11
	cmp	w9, #1                  // =1
	str	w11, [x10], #4
	and	x19, x12, x19
	b.gt	.LBB19_12
.LBB19_13:                              //   in Loop: Header=BB19_10 Depth=1
	ldr	w9, [sp, #12]
	cmp	w9, w8
	b.ne	.LBB19_20
// %bb.14:                              //   in Loop: Header=BB19_10 Depth=1
	cbz	w8, .LBB19_18
// %bb.15:                              //   in Loop: Header=BB19_10 Depth=1
	mov	x9, xzr
.LBB19_16:                              //   Parent Loop BB19_10 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x10, x9, #2
	ldr	w11, [x21, x10]
	ldr	w10, [x23, x10]
	cmp	w11, w10
	b.ne	.LBB19_20
// %bb.17:                              //   in Loop: Header=BB19_16 Depth=2
	add	x9, x9, #1              // =1
	cmp	x9, x8
	b.lo	.LBB19_16
.LBB19_18:                              //   in Loop: Header=BB19_10 Depth=1
	add	x20, x20, #1            // =1
	cmp	x20, x22
	b.lo	.LBB19_10
// %bb.19:
	adrp	x0, .Lstr.48
	add	x0, x0, :lo12:.Lstr.48
	bl	puts
	orr	w0, wzr, #0x1
	b	.LBB19_21
.LBB19_20:
	adrp	x0, .Lstr.52
	add	x0, x0, :lo12:.Lstr.52
	bl	puts
	mov	w0, wzr
.LBB19_21:
	add	sp, sp, #528            // =528
	ldp	x29, x30, [sp, #48]     // 16-byte Folded Reload
	ldp	x20, x19, [sp, #32]     // 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]     // 16-byte Folded Reload
	ldp	x28, x23, [sp], #64     // 16-byte Folded Reload
	ret
.Lfunc_end19:
	.size	_Z4unitIXadL_Z14faster_decoderPjRjjmEEEbv, .Lfunc_end19-_Z4unitIXadL_Z14faster_decoderPjRjjmEEEbv
	.cfi_endproc
                                        // -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3               // -- Begin function _Z4testIXadL_Z14faster_decoderPjRjjmEEEvPKcc
.LCPI20_0:
	.xword	4636737291354636288     // double 100
.LCPI20_1:
	.xword	4634204016564240384     // double 64
	.section	.text._Z4testIXadL_Z14faster_decoderPjRjjmEEEvPKcc,"axG",@progbits,_Z4testIXadL_Z14faster_decoderPjRjjmEEEvPKcc,comdat
	.weak	_Z4testIXadL_Z14faster_decoderPjRjjmEEEvPKcc
	.p2align	2
	.type	_Z4testIXadL_Z14faster_decoderPjRjjmEEEvPKcc,@function
_Z4testIXadL_Z14faster_decoderPjRjjmEEEvPKcc: // @_Z4testIXadL_Z14faster_decoderPjRjjmEEEvPKcc
.Lfunc_begin8:
	.cfi_startproc
	.cfi_personality 0, __gxx_personality_v0
	.cfi_lsda 0, .Lexception8
// %bb.0:
	sub	sp, sp, #432            // =432
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
	mov	w19, w1
	add	x1, sp, #136            // =136
	bl	_Z14load_text_filePKcPm
	cbz	x0, .LBB20_8
// %bb.1:
	ldr	x24, [sp, #136]
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
	b.eq	.LBB20_7
// %bb.2:
	mov	x9, xzr
	orr	w10, wzr, #0x1
.LBB20_3:                               // =>This Inner Loop Header: Depth=1
	ldrb	w11, [x21, x9]
	cmp	w11, w19, uxtb
	b.eq	.LBB20_5
// %bb.4:                               //   in Loop: Header=BB20_3 Depth=1
	cmp	w11, #31                // =31
	b.hi	.LBB20_6
.LBB20_5:                               //   in Loop: Header=BB20_3 Depth=1
	lsr	x11, x9, #3
	and	x11, x11, #0x1ffffffffffffff8
	ldr	x12, [x20, x11]
	lsl	x13, x10, x9
	orr	x12, x12, x13
	str	x12, [x20, x11]
.LBB20_6:                               //   in Loop: Header=BB20_3 Depth=1
	add	x9, x9, #1              // =1
	cmp	x8, x9
	b.ne	.LBB20_3
.LBB20_7:
	mov	x0, x21
	bl	_ZdaPv
	b	.LBB20_9
.LBB20_8:
	mov	x20, xzr
                                        // implicit-def: $x22
.LBB20_9:
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
	stp	xzr, xzr, [sp, #120]
	str	xzr, [sp, #112]
.Ltmp305:
	mov	w0, #20
	bl	_Znwm
.Ltmp306:
// %bb.14:
	ldr	w9, [x19, #16]
	ldr	q0, [x19]
	add	x8, x0, #20             // =20
	stp	x0, x0, [sp, #112]
	stp	x8, x8, [sp, #120]
	str	w9, [x0, #16]
	str	q0, [x0]
.Ltmp308:
	add	x0, sp, #136            // =136
	add	x1, sp, #112            // =112
	bl	_ZN11LinuxEventsILi0EEC2ESt6vectorIiSaIiEE
.Ltmp309:
// %bb.15:
	ldr	x0, [sp, #112]
	cbz	x0, .LBB20_17
// %bb.16:
	bl	_ZdlPv
.LBB20_17:
	stp	xzr, xzr, [sp, #96]
	stp	xzr, xzr, [sp, #80]
	stp	xzr, xzr, [sp, #64]
.Ltmp311:
	mov	w0, #40
	bl	_Znwm
.Ltmp312:
// %bb.18:
	ldp	x24, x8, [sp, #88]
	mov	x23, x0
	movi	v0.2d, #0000000000000000
	str	xzr, [x0, #32]
	subs	x2, x8, x24
	stp	q0, q0, [x0]
	b.eq	.LBB20_20
// %bb.19:
	mov	x0, x23
	mov	x1, x24
	bl	memmove
.LBB20_20:
	cbz	x24, .LBB20_22
// %bb.21:
	mov	x0, x24
	bl	_ZdlPv
.LBB20_22:
	mov	w24, wzr
	add	x8, x23, #40            // =40
	str	x8, [sp, #104]
	stp	x23, x8, [sp, #88]
	str	wzr, [sp, #60]
.LBB20_23:                              // =>This Loop Header: Depth=1
                                        //     Child Loop BB20_26 Depth 2
	str	wzr, [sp, #60]
.Ltmp314:
	add	x0, sp, #136            // =136
	bl	_ZN11LinuxEventsILi0EE5startEv
.Ltmp315:
// %bb.24:                              //   in Loop: Header=BB20_23 Depth=1
	cbz	x22, .LBB20_27
// %bb.25:                              //   in Loop: Header=BB20_23 Depth=1
	mov	x23, xzr
.LBB20_26:                              //   Parent Loop BB20_23 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldr	x3, [x20, x23, lsl #3]
	add	x1, sp, #60             // =60
	mov	x0, x21
	mov	w2, w23
	bl	_Z14faster_decoderPjRjjm
	add	x23, x23, #1            // =1
	cmp	x22, x23
	b.ne	.LBB20_26
.LBB20_27:                              //   in Loop: Header=BB20_23 Depth=1
.Ltmp316:
	add	x0, sp, #136            // =136
	add	x1, sp, #88             // =88
	bl	_ZN11LinuxEventsILi0EE3endERSt6vectorIySaIyEE
.Ltmp317:
// %bb.28:                              //   in Loop: Header=BB20_23 Depth=1
	ldp	x23, x8, [sp, #72]
	cmp	x23, x8
	b.eq	.LBB20_33
// %bb.29:                              //   in Loop: Header=BB20_23 Depth=1
	ldp	x9, x8, [sp, #88]
	stp	xzr, xzr, [x23]
	str	xzr, [x23, #16]
	subs	x0, x8, x9
	asr	x26, x0, #3
	b.eq	.LBB20_34
// %bb.30:                              //   in Loop: Header=BB20_23 Depth=1
	lsr	x8, x26, #61
	cbnz	x8, .LBB20_122
// %bb.31:                              //   in Loop: Header=BB20_23 Depth=1
.Ltmp318:
	bl	_Znwm
.Ltmp319:
// %bb.32:                              //   in Loop: Header=BB20_23 Depth=1
	mov	x27, x0
	b	.LBB20_35
.LBB20_33:                              //   in Loop: Header=BB20_23 Depth=1
.Ltmp323:
	add	x0, sp, #64             // =64
	add	x2, sp, #88             // =88
	mov	x1, x23
	bl	_ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_
.Ltmp324:
	b	.LBB20_38
.LBB20_34:                              //   in Loop: Header=BB20_23 Depth=1
	mov	x27, xzr
.LBB20_35:                              //   in Loop: Header=BB20_23 Depth=1
	add	x8, x27, x26, lsl #3
	stp	x27, x0, [x23]
	str	x8, [x23, #16]
	ldp	x1, x8, [sp, #88]
	subs	x2, x8, x1
	asr	x26, x2, #3
	b.eq	.LBB20_37
// %bb.36:                              //   in Loop: Header=BB20_23 Depth=1
	bl	memmove
.LBB20_37:                              //   in Loop: Header=BB20_23 Depth=1
	add	x8, x27, x26, lsl #3
	str	x8, [x23, #8]
	ldr	x8, [sp, #72]
	add	x8, x8, #24             // =24
	str	x8, [sp, #72]
.LBB20_38:                              //   in Loop: Header=BB20_23 Depth=1
	add	w24, w24, #1            // =1
	cmp	w24, #10                // =10
	b.lo	.LBB20_23
// %bb.39:
	ldr	w8, [x21]
	cbnz	w8, .LBB20_41
// %bb.40:
	adrp	x0, .L.str.3
	add	x0, x0, :lo12:.L.str.3
	bl	printf
.LBB20_41:
	ldr	w1, [sp, #60]
	adrp	x8, .LCPI20_0
	ldr	d0, [x8, :lo12:.LCPI20_0]
	ucvtf	d8, x25
	ucvtf	d1, w1
	fdiv	d1, d1, d8
	adrp	x0, .L.str.4
	mov	x28, #-6148914691236517206
	fmul	d0, d1, d0
	add	x0, x0, :lo12:.L.str.4
	mov	x2, x22
	movk	x28, #2730, lsl #48
	bl	printf
	adrp	x8, .LCPI20_1
	ldr	d0, [x8, :lo12:.LCPI20_1]
	ldr	s1, [sp, #60]
	ucvtf	d9, x22
	adrp	x0, .L.str.5
	fmul	d0, d9, d0
	ucvtf	d1, d1
	fdiv	d0, d0, d1
	add	x0, x0, :lo12:.L.str.5
	bl	printf
	ldp	x0, x1, [sp, #64]
	subs	x8, x1, x0
	b.eq	.LBB20_45
// %bb.42:
	mov	x10, #-6148914691236517206
	asr	x9, x8, #3
	movk	x10, #43691
	mul	x9, x9, x10
	cmp	x9, x28
	b.hi	.LBB20_124
// %bb.43:
.Ltmp326:
	mov	x0, x8
	bl	_Znwm
.Ltmp327:
// %bb.44:
	mov	x23, x0
	ldp	x0, x1, [sp, #64]
	mov	x24, x23
	b	.LBB20_46
.LBB20_45:
	mov	x23, xzr
	mov	x24, xzr
.LBB20_46:
.Ltmp331:
	mov	x2, x24
	bl	_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_
.Ltmp332:
// %bb.47:
	mov	x25, x0
	subs	x27, x0, x24
	b.eq	.LBB20_53
// %bb.48:
	ldp	x8, x9, [x24]
	subs	x0, x9, x8
	b.eq	.LBB20_54
// %bb.49:
	asr	x8, x0, #3
	lsr	x8, x8, #61
	cbnz	x8, .LBB20_128
// %bb.50:
.Ltmp334:
	bl	_Znwm
.Ltmp335:
// %bb.51:
	ldp	x1, x8, [x24]
	mov	x26, x0
	subs	x2, x8, x1
	b.eq	.LBB20_55
// %bb.52:
	mov	x0, x26
	asr	x28, x2, #3
	bl	memmove
	mov	w8, wzr
	b	.LBB20_56
.LBB20_53:
	mov	x22, xzr
	cbnz	x24, .LBB20_85
	b	.LBB20_86
.LBB20_54:
	mov	x22, xzr
	mov	x26, xzr
	mov	x28, xzr
	orr	w8, wzr, #0x1
	b	.LBB20_57
.LBB20_55:
	mov	x28, xzr
	orr	w8, wzr, #0x1
.LBB20_56:
	mov	x22, x26
.LBB20_57:
	mov	x10, #-6148914691236517206
	asr	x9, x27, #3
	movk	x10, #43691
	mul	x9, x9, x10
	cmp	x9, #2                  // =2
	b.lo	.LBB20_80
// %bb.58:
	cmp	x28, #1                 // =1
	csinc	x12, x28, xzr, hi
	add	x10, x22, #16           // =16
	orr	w11, wzr, #0x1
	add	x13, x22, x12, lsl #3
	and	x14, x12, #0xfffffffffffffffc
	orr	w15, wzr, #0x18
	b	.LBB20_70
.LBB20_59:                              //   in Loop: Header=BB20_70 Depth=1
	add	x17, x16, #16           // =16
	mov	x18, x10
	mov	x0, x14
.LBB20_60:                              //   Parent Loop BB20_70 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	ldp	q1, q0, [x17, #-16]
	ldp	q3, q2, [x18, #-16]
	cmhi	v3.2d, v3.2d, v1.2d
	xtn	v3.2s, v3.2d
	fmov	w1, s3
	tbz	w1, #0, .LBB20_62
// %bb.61:                              //   in Loop: Header=BB20_60 Depth=2
	stur	d1, [x18, #-16]
.LBB20_62:                              //   in Loop: Header=BB20_60 Depth=2
	mov	w1, v3.s[1]
	tbz	w1, #0, .LBB20_64
// %bb.63:                              //   in Loop: Header=BB20_60 Depth=2
	sub	x1, x18, #8             // =8
	st1	{ v1.d }[1], [x1]
.LBB20_64:                              //   in Loop: Header=BB20_60 Depth=2
	cmhi	v1.2d, v2.2d, v0.2d
	xtn	v1.2s, v1.2d
	fmov	w1, s1
	tbz	w1, #0, .LBB20_66
// %bb.65:                              //   in Loop: Header=BB20_60 Depth=2
	str	d0, [x18]
.LBB20_66:                              //   in Loop: Header=BB20_60 Depth=2
	mov	w1, v1.s[1]
	tbz	w1, #0, .LBB20_68
// %bb.67:                              //   in Loop: Header=BB20_60 Depth=2
	add	x1, x18, #8             // =8
	st1	{ v0.d }[1], [x1]
.LBB20_68:                              //   in Loop: Header=BB20_60 Depth=2
	subs	x0, x0, #4              // =4
	add	x18, x18, #32           // =32
	add	x17, x17, #32           // =32
	b.ne	.LBB20_60
// %bb.69:                              //   in Loop: Header=BB20_70 Depth=1
	cmp	x12, x14
	mov	x17, x14
	b.ne	.LBB20_75
	b	.LBB20_78
.LBB20_70:                              // =>This Loop Header: Depth=1
                                        //     Child Loop BB20_60 Depth 2
                                        //     Child Loop BB20_75 Depth 2
	tbnz	w8, #0, .LBB20_78
// %bb.71:                              //   in Loop: Header=BB20_70 Depth=1
	mul	x16, x11, x15
	ldr	x16, [x24, x16]
	cmp	x12, #3                 // =3
	b.ls	.LBB20_74
// %bb.72:                              //   in Loop: Header=BB20_70 Depth=1
	add	x17, x16, x12, lsl #3
	cmp	x26, x17
	b.hs	.LBB20_59
// %bb.73:                              //   in Loop: Header=BB20_70 Depth=1
	cmp	x16, x13
	b.hs	.LBB20_59
.LBB20_74:                              //   in Loop: Header=BB20_70 Depth=1
	mov	x17, xzr
.LBB20_75:                              //   Parent Loop BB20_70 Depth=1
                                        // =>  This Inner Loop Header: Depth=2
	lsl	x0, x17, #3
	ldr	x18, [x16, x0]
	ldr	x0, [x22, x0]
	cmp	x18, x0
	b.hs	.LBB20_77
// %bb.76:                              //   in Loop: Header=BB20_75 Depth=2
	str	x18, [x22, x17, lsl #3]
.LBB20_77:                              //   in Loop: Header=BB20_75 Depth=2
	add	x17, x17, #1            // =1
	cmp	x17, x28
	b.lo	.LBB20_75
.LBB20_78:                              //   in Loop: Header=BB20_70 Depth=1
	add	x11, x11, #1            // =1
	cmp	x11, x9
	b.ne	.LBB20_70
// %bb.79:
	cmp	x25, x24
	b.eq	.LBB20_121
.LBB20_80:
	mov	x28, #-6148914691236517206
	mov	x26, x24
	movk	x28, #2730, lsl #48
.LBB20_81:                              // =>This Inner Loop Header: Depth=1
	ldr	x0, [x26]
	cbz	x0, .LBB20_83
// %bb.82:                              //   in Loop: Header=BB20_81 Depth=1
	bl	_ZdlPv
.LBB20_83:                              //   in Loop: Header=BB20_81 Depth=1
	add	x26, x26, #24           // =24
	cmp	x25, x26
	b.ne	.LBB20_81
// %bb.84:
	cbz	x24, .LBB20_86
.LBB20_85:
	mov	x0, x23
	bl	_ZdlPv
.LBB20_86:
	ldp	x0, x1, [sp, #64]
	mov	x9, #-6148914691236517206
	movk	x9, #43691
	stp	xzr, xzr, [sp, #8]
	subs	x8, x1, x0
	asr	x10, x8, #3
	mul	x25, x10, x9
	str	xzr, [sp, #24]
	b.eq	.LBB20_90
// %bb.87:
	cmp	x25, x28
	b.hi	.LBB20_126
// %bb.88:
.Ltmp339:
	mov	x0, x8
	bl	_Znwm
.Ltmp340:
// %bb.89:
	mov	x23, x0
	ldp	x0, x1, [sp, #64]
	mov	x24, x23
	b	.LBB20_91
.LBB20_90:
	mov	x23, xzr
	mov	x24, xzr
.LBB20_91:
	orr	w8, wzr, #0x18
	madd	x8, x25, x8, x24
	stp	x23, x23, [sp, #8]
	str	x8, [sp, #24]
.Ltmp344:
	mov	x2, x24
	bl	_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_
.Ltmp345:
// %bb.92:
	mov	x25, x0
	str	x0, [sp, #16]
.Ltmp347:
	add	x8, sp, #32             // =32
	add	x0, sp, #8              // =8
	bl	_Z16compute_averagesSt6vectorIS_IySaIyEESaIS1_EE
.Ltmp348:
// %bb.93:
	cmp	x24, x25
	b.eq	.LBB20_98
// %bb.94:
	mov	x26, x24
.LBB20_95:                              // =>This Inner Loop Header: Depth=1
	ldr	x0, [x26]
	cbz	x0, .LBB20_97
// %bb.96:                              //   in Loop: Header=BB20_95 Depth=1
	bl	_ZdlPv
.LBB20_97:                              //   in Loop: Header=BB20_95 Depth=1
	add	x26, x26, #24           // =24
	cmp	x25, x26
	b.ne	.LBB20_95
.LBB20_98:
	cbz	x24, .LBB20_100
// %bb.99:
	mov	x0, x23
	bl	_ZdlPv
.LBB20_100:
	ldp	d1, d0, [x22]
	ldr	s2, [sp, #60]
	adrp	x0, .L.str.6
	add	x0, x0, :lo12:.L.str.6
	ucvtf	d4, d0
	ucvtf	d3, d1
	ucvtf	d2, d2
	fdiv	d0, d4, d3
	fdiv	d1, d3, d2
	fdiv	d2, d4, d2
	fdiv	d3, d3, d9
	fdiv	d4, d4, d9
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
	ldr	x23, [sp, #32]
	adrp	x0, .L.str.9
	add	x0, x0, :lo12:.L.str.9
	ldp	d0, d1, [x23]
	ldp	d2, d3, [x23, #16]
	ldr	d4, [x23, #32]
	bl	printf
	mov	w0, #10
	bl	putchar
	cbz	x20, .LBB20_102
// %bb.101:
	mov	x0, x20
	bl	_ZdaPv
.LBB20_102:
	mov	x0, x21
	bl	_ZdaPv
	cbz	x23, .LBB20_104
// %bb.103:
	mov	x0, x23
	bl	_ZdlPv
.LBB20_104:
	cbz	x22, .LBB20_106
// %bb.105:
	mov	x0, x22
	bl	_ZdlPv
.LBB20_106:
	ldp	x20, x21, [sp, #64]
	cmp	x20, x21
	b.eq	.LBB20_111
.LBB20_107:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x20]
	cbz	x0, .LBB20_109
// %bb.108:                             //   in Loop: Header=BB20_107 Depth=1
	bl	_ZdlPv
.LBB20_109:                             //   in Loop: Header=BB20_107 Depth=1
	add	x20, x20, #24           // =24
	cmp	x21, x20
	b.ne	.LBB20_107
// %bb.110:
	ldr	x20, [sp, #64]
.LBB20_111:
	cbz	x20, .LBB20_113
// %bb.112:
	mov	x0, x20
	bl	_ZdlPv
.LBB20_113:
	ldr	x0, [sp, #88]
	cbz	x0, .LBB20_115
// %bb.114:
	bl	_ZdlPv
.LBB20_115:
	ldr	w0, [sp, #136]
.Ltmp353:
	bl	close
.Ltmp354:
// %bb.116:
	ldr	x0, [sp, #288]
	cbz	x0, .LBB20_118
// %bb.117:
	bl	_ZdlPv
.LBB20_118:
	ldr	x0, [sp, #264]
	cbz	x0, .LBB20_120
// %bb.119:
	bl	_ZdlPv
.LBB20_120:
	mov	x0, x19
	bl	_ZdlPv
	ldp	x29, x30, [sp, #416]    // 16-byte Folded Reload
	ldp	x20, x19, [sp, #400]    // 16-byte Folded Reload
	ldp	x22, x21, [sp, #384]    // 16-byte Folded Reload
	ldp	x24, x23, [sp, #368]    // 16-byte Folded Reload
	ldp	x26, x25, [sp, #352]    // 16-byte Folded Reload
	ldp	x28, x27, [sp, #336]    // 16-byte Folded Reload
	ldp	d9, d8, [sp, #320]      // 16-byte Folded Reload
	add	sp, sp, #432            // =432
	ret
.LBB20_121:
	mov	x28, #-6148914691236517206
	movk	x28, #2730, lsl #48
	cbnz	x24, .LBB20_85
	b	.LBB20_86
.LBB20_122:
.Ltmp320:
	bl	_ZSt17__throw_bad_allocv
.Ltmp321:
// %bb.123:
.LBB20_124:
.Ltmp328:
	bl	_ZSt17__throw_bad_allocv
.Ltmp329:
// %bb.125:
.LBB20_126:
.Ltmp341:
	bl	_ZSt17__throw_bad_allocv
.Ltmp342:
// %bb.127:
.LBB20_128:
.Ltmp336:
	bl	_ZSt17__throw_bad_allocv
.Ltmp337:
// %bb.129:
.LBB20_130:
.Ltmp322:
	b	.LBB20_163
.LBB20_131:
.Ltmp338:
	mov	x20, x0
	mov	x21, x24
.LBB20_132:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x21]
	cbz	x0, .LBB20_134
// %bb.133:                             //   in Loop: Header=BB20_132 Depth=1
	bl	_ZdlPv
.LBB20_134:                             //   in Loop: Header=BB20_132 Depth=1
	add	x21, x21, #24           // =24
	cmp	x25, x21
	b.ne	.LBB20_132
// %bb.135:
	cbnz	x24, .LBB20_151
	b	.LBB20_164
.LBB20_136:
.Ltmp343:
	mov	x20, x0
	cbnz	x22, .LBB20_149
	b	.LBB20_164
.LBB20_137:
.Ltmp330:
	b	.LBB20_163
.LBB20_138:
.Ltmp355:
.LBB20_139:
	add	x20, sp, #136           // =136
	mov	x19, x0
	add	x0, x20, #152           // =152
	bl	_ZNSt6vectorImSaImEED2Ev
	add	x0, x20, #128           // =128
	bl	_ZNSt6vectorImSaImEED2Ev
	mov	x0, x19
	bl	__clang_call_terminate
.LBB20_140:
.Ltmp349:
	cmp	x24, x25
	mov	x20, x0
	b.eq	.LBB20_145
// %bb.141:
	mov	x21, x24
.LBB20_142:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x21]
	cbz	x0, .LBB20_144
// %bb.143:                             //   in Loop: Header=BB20_142 Depth=1
	bl	_ZdlPv
.LBB20_144:                             //   in Loop: Header=BB20_142 Depth=1
	add	x21, x21, #24           // =24
	cmp	x25, x21
	b.ne	.LBB20_142
.LBB20_145:
	cbnz	x24, .LBB20_147
	b	.LBB20_148
.LBB20_146:
.Ltmp346:
	mov	x20, x0
	cbz	x24, .LBB20_148
.LBB20_147:
	mov	x0, x23
	bl	_ZdlPv
.LBB20_148:
	cbz	x22, .LBB20_164
.LBB20_149:
	mov	x0, x22
	b	.LBB20_152
.LBB20_150:
.Ltmp333:
	mov	x20, x0
	cbz	x24, .LBB20_164
.LBB20_151:
	mov	x0, x23
.LBB20_152:
	bl	_ZdlPv
	b	.LBB20_164
.LBB20_153:
.Ltmp313:
	b	.LBB20_163
.LBB20_154:
.Ltmp310:
	ldr	x8, [sp, #112]
	mov	x20, x0
	cbz	x8, .LBB20_178
// %bb.155:
	mov	x0, x8
	b	.LBB20_177
.LBB20_156:
.Ltmp307:
	b	.LBB20_160
.LBB20_157:
.Ltmp304:
	mov	x20, x0
	mov	x19, x23
	b	.LBB20_178
.LBB20_158:
.Ltmp301:
	mov	x20, x0
	mov	x19, x24
	b	.LBB20_178
.LBB20_159:
.Ltmp298:
.LBB20_160:
	mov	x20, x0
	b	.LBB20_178
.LBB20_161:
.Ltmp295:
	bl	_Unwind_Resume
.LBB20_162:
.Ltmp325:
.LBB20_163:
	mov	x20, x0
.LBB20_164:
	ldp	x21, x22, [sp, #64]
	cmp	x21, x22
	b.eq	.LBB20_169
.LBB20_165:                             // =>This Inner Loop Header: Depth=1
	ldr	x0, [x21]
	cbz	x0, .LBB20_167
// %bb.166:                             //   in Loop: Header=BB20_165 Depth=1
	bl	_ZdlPv
.LBB20_167:                             //   in Loop: Header=BB20_165 Depth=1
	add	x21, x21, #24           // =24
	cmp	x22, x21
	b.ne	.LBB20_165
// %bb.168:
	ldr	x21, [sp, #64]
.LBB20_169:
	cbz	x21, .LBB20_171
// %bb.170:
	mov	x0, x21
	bl	_ZdlPv
.LBB20_171:
	ldr	x0, [sp, #88]
	cbz	x0, .LBB20_173
// %bb.172:
	bl	_ZdlPv
.LBB20_173:
	ldr	w0, [sp, #136]
.Ltmp350:
	bl	close
.Ltmp351:
// %bb.174:
	ldr	x0, [sp, #288]
	cbz	x0, .LBB20_176
// %bb.175:
	bl	_ZdlPv
.LBB20_176:
	ldr	x0, [sp, #264]
	cbz	x0, .LBB20_178
.LBB20_177:
	bl	_ZdlPv
.LBB20_178:
	mov	x0, x19
	bl	_ZdlPv
	mov	x0, x20
	bl	_Unwind_Resume
.LBB20_179:
.Ltmp352:
	b	.LBB20_139
.Lfunc_end20:
	.size	_Z4testIXadL_Z14faster_decoderPjRjjmEEEvPKcc, .Lfunc_end20-_Z4testIXadL_Z14faster_decoderPjRjjmEEEvPKcc
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table20:
.Lexception8:
	.byte	255                     // @LPStart Encoding = omit
	.byte	0                       // @TType Encoding = absptr
	.uleb128 .Lttbase4-.Lttbaseref4
.Lttbaseref4:
	.byte	1                       // Call site Encoding = uleb128
	.uleb128 .Lcst_end8-.Lcst_begin8
.Lcst_begin8:
	.uleb128 .Lfunc_begin8-.Lfunc_begin8 // >> Call Site 1 <<
	.uleb128 .Ltmp293-.Lfunc_begin8 //   Call between .Lfunc_begin8 and .Ltmp293
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp293-.Lfunc_begin8 // >> Call Site 2 <<
	.uleb128 .Ltmp294-.Ltmp293      //   Call between .Ltmp293 and .Ltmp294
	.uleb128 .Ltmp295-.Lfunc_begin8 //     jumps to .Ltmp295
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp296-.Lfunc_begin8 // >> Call Site 3 <<
	.uleb128 .Ltmp297-.Ltmp296      //   Call between .Ltmp296 and .Ltmp297
	.uleb128 .Ltmp298-.Lfunc_begin8 //     jumps to .Ltmp298
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp299-.Lfunc_begin8 // >> Call Site 4 <<
	.uleb128 .Ltmp300-.Ltmp299      //   Call between .Ltmp299 and .Ltmp300
	.uleb128 .Ltmp301-.Lfunc_begin8 //     jumps to .Ltmp301
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp302-.Lfunc_begin8 // >> Call Site 5 <<
	.uleb128 .Ltmp303-.Ltmp302      //   Call between .Ltmp302 and .Ltmp303
	.uleb128 .Ltmp304-.Lfunc_begin8 //     jumps to .Ltmp304
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp305-.Lfunc_begin8 // >> Call Site 6 <<
	.uleb128 .Ltmp306-.Ltmp305      //   Call between .Ltmp305 and .Ltmp306
	.uleb128 .Ltmp307-.Lfunc_begin8 //     jumps to .Ltmp307
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp308-.Lfunc_begin8 // >> Call Site 7 <<
	.uleb128 .Ltmp309-.Ltmp308      //   Call between .Ltmp308 and .Ltmp309
	.uleb128 .Ltmp310-.Lfunc_begin8 //     jumps to .Ltmp310
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp311-.Lfunc_begin8 // >> Call Site 8 <<
	.uleb128 .Ltmp312-.Ltmp311      //   Call between .Ltmp311 and .Ltmp312
	.uleb128 .Ltmp313-.Lfunc_begin8 //     jumps to .Ltmp313
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp312-.Lfunc_begin8 // >> Call Site 9 <<
	.uleb128 .Ltmp314-.Ltmp312      //   Call between .Ltmp312 and .Ltmp314
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp314-.Lfunc_begin8 // >> Call Site 10 <<
	.uleb128 .Ltmp324-.Ltmp314      //   Call between .Ltmp314 and .Ltmp324
	.uleb128 .Ltmp325-.Lfunc_begin8 //     jumps to .Ltmp325
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp324-.Lfunc_begin8 // >> Call Site 11 <<
	.uleb128 .Ltmp326-.Ltmp324      //   Call between .Ltmp324 and .Ltmp326
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp326-.Lfunc_begin8 // >> Call Site 12 <<
	.uleb128 .Ltmp327-.Ltmp326      //   Call between .Ltmp326 and .Ltmp327
	.uleb128 .Ltmp330-.Lfunc_begin8 //     jumps to .Ltmp330
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp331-.Lfunc_begin8 // >> Call Site 13 <<
	.uleb128 .Ltmp332-.Ltmp331      //   Call between .Ltmp331 and .Ltmp332
	.uleb128 .Ltmp333-.Lfunc_begin8 //     jumps to .Ltmp333
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp334-.Lfunc_begin8 // >> Call Site 14 <<
	.uleb128 .Ltmp335-.Ltmp334      //   Call between .Ltmp334 and .Ltmp335
	.uleb128 .Ltmp338-.Lfunc_begin8 //     jumps to .Ltmp338
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp335-.Lfunc_begin8 // >> Call Site 15 <<
	.uleb128 .Ltmp339-.Ltmp335      //   Call between .Ltmp335 and .Ltmp339
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp339-.Lfunc_begin8 // >> Call Site 16 <<
	.uleb128 .Ltmp340-.Ltmp339      //   Call between .Ltmp339 and .Ltmp340
	.uleb128 .Ltmp343-.Lfunc_begin8 //     jumps to .Ltmp343
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp344-.Lfunc_begin8 // >> Call Site 17 <<
	.uleb128 .Ltmp345-.Ltmp344      //   Call between .Ltmp344 and .Ltmp345
	.uleb128 .Ltmp346-.Lfunc_begin8 //     jumps to .Ltmp346
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp347-.Lfunc_begin8 // >> Call Site 18 <<
	.uleb128 .Ltmp348-.Ltmp347      //   Call between .Ltmp347 and .Ltmp348
	.uleb128 .Ltmp349-.Lfunc_begin8 //     jumps to .Ltmp349
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp353-.Lfunc_begin8 // >> Call Site 19 <<
	.uleb128 .Ltmp354-.Ltmp353      //   Call between .Ltmp353 and .Ltmp354
	.uleb128 .Ltmp355-.Lfunc_begin8 //     jumps to .Ltmp355
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp320-.Lfunc_begin8 // >> Call Site 20 <<
	.uleb128 .Ltmp321-.Ltmp320      //   Call between .Ltmp320 and .Ltmp321
	.uleb128 .Ltmp322-.Lfunc_begin8 //     jumps to .Ltmp322
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp328-.Lfunc_begin8 // >> Call Site 21 <<
	.uleb128 .Ltmp329-.Ltmp328      //   Call between .Ltmp328 and .Ltmp329
	.uleb128 .Ltmp330-.Lfunc_begin8 //     jumps to .Ltmp330
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp341-.Lfunc_begin8 // >> Call Site 22 <<
	.uleb128 .Ltmp342-.Ltmp341      //   Call between .Ltmp341 and .Ltmp342
	.uleb128 .Ltmp343-.Lfunc_begin8 //     jumps to .Ltmp343
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp336-.Lfunc_begin8 // >> Call Site 23 <<
	.uleb128 .Ltmp337-.Ltmp336      //   Call between .Ltmp336 and .Ltmp337
	.uleb128 .Ltmp338-.Lfunc_begin8 //     jumps to .Ltmp338
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp337-.Lfunc_begin8 // >> Call Site 24 <<
	.uleb128 .Ltmp350-.Ltmp337      //   Call between .Ltmp337 and .Ltmp350
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp350-.Lfunc_begin8 // >> Call Site 25 <<
	.uleb128 .Ltmp351-.Ltmp350      //   Call between .Ltmp350 and .Ltmp351
	.uleb128 .Ltmp352-.Lfunc_begin8 //     jumps to .Ltmp352
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp351-.Lfunc_begin8 // >> Call Site 26 <<
	.uleb128 .Lfunc_end20-.Ltmp351  //   Call between .Ltmp351 and .Lfunc_end20
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
.Lcst_end8:
	.byte	1                       // >> Action Record 1 <<
                                        //   Catch TypeInfo 1
	.byte	0                       //   No further actions
	.p2align	2
                                        // >> Catch TypeInfos <<
	.xword	0                       // TypeInfo 1
.Lttbase4:
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
.Lfunc_end21:
	.size	__clang_call_terminate, .Lfunc_end21-__clang_call_terminate
                                        // -- End function
	.section	.text._ZN11LinuxEventsILi0EE12report_errorERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE,"axG",@progbits,_ZN11LinuxEventsILi0EE12report_errorERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE,comdat
	.weak	_ZN11LinuxEventsILi0EE12report_errorERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE // -- Begin function _ZN11LinuxEventsILi0EE12report_errorERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
	.p2align	2
	.type	_ZN11LinuxEventsILi0EE12report_errorERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE,@function
_ZN11LinuxEventsILi0EE12report_errorERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE: // @_ZN11LinuxEventsILi0EE12report_errorERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
.Lfunc_begin9:
	.cfi_startproc
	.cfi_personality 0, __gxx_personality_v0
	.cfi_lsda 0, .Lexception9
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
	cbz	w8, .LBB22_30
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
	cbz	x0, .LBB22_31
// %bb.2:
	mov	x20, x0
	bl	strlen
	mov	x21, x0
	cmp	x0, #15                 // =15
	stur	x0, [x29, #-56]
	b.ls	.LBB22_7
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
	cbz	x21, .LBB22_9
.LBB22_5:
	cmp	x21, #1                 // =1
	b.ne	.LBB22_8
// %bb.6:
	ldrb	w8, [x20]
	strb	w8, [x0]
	b	.LBB22_9
.LBB22_7:
	mov	x0, x22
	cbnz	x21, .LBB22_5
	b	.LBB22_9
.LBB22_8:
	mov	x1, x20
	mov	x2, x21
	bl	memcpy
.LBB22_9:
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
	b.ls	.LBB22_11
// %bb.10:
	ldr	x9, [sp, #24]
	cmp	x1, x22
	orr	w10, wzr, #0xf
	csel	x9, x10, x9, eq
	cmp	x8, x9
	b.ls	.LBB22_12
.LBB22_11:
.Ltmp360:
	add	x0, sp, #40             // =40
	bl	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_appendEPKcm
.Ltmp361:
	b	.LBB22_13
.LBB22_12:
.Ltmp358:
	add	x0, sp, #8              // =8
	mov	x1, xzr
	mov	x2, xzr
	bl	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_replaceEmmPKcm
.Ltmp359:
.LBB22_13:
	add	x8, sp, #72             // =72
	add	x24, x8, #16            // =16
	str	x24, [sp, #72]
	mov	x8, x0
	ldr	x9, [x8], #16
	cmp	x9, x8
	b.eq	.LBB22_15
// %bb.14:
	str	x9, [sp, #72]
	ldr	x9, [x0, #16]
	str	x9, [sp, #88]
	b	.LBB22_16
.LBB22_15:
	ldr	q0, [x9]
	str	q0, [x24]
.LBB22_16:
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
	cbz	x21, .LBB22_33
// %bb.18:
	ldrb	w8, [x21, #56]
	cbz	w8, .LBB22_20
// %bb.19:
	ldrb	w1, [x21, #67]
	b	.LBB22_22
.LBB22_20:
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
.LBB22_22:
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
	b.eq	.LBB22_26
// %bb.25:
	bl	_ZdlPv
.LBB22_26:
	ldr	x0, [sp, #8]
	cmp	x0, x22
	b.eq	.LBB22_28
// %bb.27:
	bl	_ZdlPv
.LBB22_28:
	ldr	x0, [sp, #40]
	cmp	x0, x23
	b.eq	.LBB22_30
// %bb.29:
	bl	_ZdlPv
.LBB22_30:
	strb	wzr, [x19, #4]
	ldp	x29, x30, [sp, #160]    // 16-byte Folded Reload
	ldp	x20, x19, [sp, #144]    // 16-byte Folded Reload
	ldp	x22, x21, [sp, #128]    // 16-byte Folded Reload
	ldp	x24, x23, [sp, #112]    // 16-byte Folded Reload
	add	sp, sp, #176            // =176
	ret
.LBB22_31:
.Ltmp376:
	adrp	x0, .L.str.22
	add	x0, x0, :lo12:.L.str.22
	bl	_ZSt19__throw_logic_errorPKc
.Ltmp377:
// %bb.32:
.LBB22_33:
.Ltmp373:
	bl	_ZSt16__throw_bad_castv
.Ltmp374:
// %bb.34:
.LBB22_35:
.Ltmp378:
	mov	x19, x0
	b	.LBB22_41
.LBB22_36:
.Ltmp362:
	mov	x19, x0
	b	.LBB22_39
.LBB22_37:
.Ltmp375:
	ldr	x8, [sp, #72]
	mov	x19, x0
	cmp	x8, x24
	b.eq	.LBB22_39
// %bb.38:
	mov	x0, x8
	bl	_ZdlPv
.LBB22_39:
	ldr	x0, [sp, #8]
	cmp	x0, x22
	b.eq	.LBB22_41
// %bb.40:
	bl	_ZdlPv
.LBB22_41:
	ldr	x0, [sp, #40]
	add	x8, sp, #40             // =40
	add	x8, x8, #16             // =16
	cmp	x0, x8
	b.eq	.LBB22_43
// %bb.42:
	bl	_ZdlPv
.LBB22_43:
	mov	x0, x19
	bl	_Unwind_Resume
.Lfunc_end22:
	.size	_ZN11LinuxEventsILi0EE12report_errorERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE, .Lfunc_end22-_ZN11LinuxEventsILi0EE12report_errorERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table22:
.Lexception9:
	.byte	255                     // @LPStart Encoding = omit
	.byte	255                     // @TType Encoding = omit
	.byte	1                       // Call site Encoding = uleb128
	.uleb128 .Lcst_end9-.Lcst_begin9
.Lcst_begin9:
	.uleb128 .Lfunc_begin9-.Lfunc_begin9 // >> Call Site 1 <<
	.uleb128 .Ltmp356-.Lfunc_begin9 //   Call between .Lfunc_begin9 and .Ltmp356
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp356-.Lfunc_begin9 // >> Call Site 2 <<
	.uleb128 .Ltmp357-.Ltmp356      //   Call between .Ltmp356 and .Ltmp357
	.uleb128 .Ltmp378-.Lfunc_begin9 //     jumps to .Ltmp378
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp357-.Lfunc_begin9 // >> Call Site 3 <<
	.uleb128 .Ltmp360-.Ltmp357      //   Call between .Ltmp357 and .Ltmp360
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp360-.Lfunc_begin9 // >> Call Site 4 <<
	.uleb128 .Ltmp359-.Ltmp360      //   Call between .Ltmp360 and .Ltmp359
	.uleb128 .Ltmp362-.Lfunc_begin9 //     jumps to .Ltmp362
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp363-.Lfunc_begin9 // >> Call Site 5 <<
	.uleb128 .Ltmp372-.Ltmp363      //   Call between .Ltmp363 and .Ltmp372
	.uleb128 .Ltmp375-.Lfunc_begin9 //     jumps to .Ltmp375
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp376-.Lfunc_begin9 // >> Call Site 6 <<
	.uleb128 .Ltmp377-.Ltmp376      //   Call between .Ltmp376 and .Ltmp377
	.uleb128 .Ltmp378-.Lfunc_begin9 //     jumps to .Ltmp378
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp373-.Lfunc_begin9 // >> Call Site 7 <<
	.uleb128 .Ltmp374-.Ltmp373      //   Call between .Ltmp373 and .Ltmp374
	.uleb128 .Ltmp375-.Lfunc_begin9 //     jumps to .Ltmp375
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp374-.Lfunc_begin9 // >> Call Site 8 <<
	.uleb128 .Lfunc_end22-.Ltmp374  //   Call between .Ltmp374 and .Lfunc_end22
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
.Lcst_end9:
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
	cbz	x0, .LBB23_2
// %bb.1:
	b	_ZdlPv
.LBB23_2:
	ret
.Lfunc_end23:
	.size	_ZNSt6vectorImSaImEED2Ev, .Lfunc_end23-_ZNSt6vectorImSaImEED2Ev
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
	cbz	x1, .LBB24_13
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
	b.ls	.LBB24_5
// %bb.2:
	sub	x8, x22, x21
	asr	x24, x8, #3
	orr	x8, xzr, #0x1fffffffffffffff
	sub	x9, x8, x24
	cmp	x9, x19
	b.lo	.LBB24_14
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
	cbz	x26, .LBB24_6
// %bb.4:
	lsl	x0, x26, #3
	bl	_Znwm
	ldp	x21, x22, [x20]
	mov	x23, x0
	b	.LBB24_7
.LBB24_5:
	lsl	x19, x19, #3
	mov	x0, x22
	mov	w1, wzr
	mov	x2, x19
	bl	memset
	add	x8, x22, x19
	mov	x25, x23
	b	.LBB24_12
.LBB24_6:
	mov	x23, xzr
.LBB24_7:
	add	x24, x23, x24, lsl #3
	lsl	x2, x19, #3
	mov	x0, x24
	mov	w1, wzr
	bl	memset
	subs	x2, x22, x21
	b.eq	.LBB24_9
// %bb.8:
	mov	x0, x23
	mov	x1, x21
	bl	memmove
.LBB24_9:
	cbz	x21, .LBB24_11
// %bb.10:
	mov	x0, x21
	bl	_ZdlPv
.LBB24_11:
	add	x8, x24, x19, lsl #3
	stp	x23, x8, [x20]
	add	x8, x23, x26, lsl #3
.LBB24_12:
	str	x8, [x25]
.LBB24_13:
	ldp	x29, x30, [sp, #64]     // 16-byte Folded Reload
	ldp	x20, x19, [sp, #48]     // 16-byte Folded Reload
	ldp	x22, x21, [sp, #32]     // 16-byte Folded Reload
	ldp	x24, x23, [sp, #16]     // 16-byte Folded Reload
	ldp	x26, x25, [sp], #80     // 16-byte Folded Reload
	ret
.LBB24_14:
	adrp	x0, .L.str.18
	add	x0, x0, :lo12:.L.str.18
	bl	_ZSt20__throw_length_errorPKc
.Lfunc_end24:
	.size	_ZNSt6vectorImSaImEE17_M_default_appendEm, .Lfunc_end24-_ZNSt6vectorImSaImEE17_M_default_appendEm
	.cfi_endproc
                                        // -- End function
	.section	.text._ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EERKS8_PKS5_,"axG",@progbits,_ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EERKS8_PKS5_,comdat
	.weak	_ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EERKS8_PKS5_ // -- Begin function _ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EERKS8_PKS5_
	.p2align	2
	.type	_ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EERKS8_PKS5_,@function
_ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EERKS8_PKS5_: // @_ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EERKS8_PKS5_
.Lfunc_begin10:
	.cfi_startproc
	.cfi_personality 0, __gxx_personality_v0
	.cfi_lsda 0, .Lexception10
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
	cbnz	x21, .LBB25_2
// %bb.1:
	cbnz	x22, .LBB25_11
.LBB25_2:
	cmp	x22, #16                // =16
	str	x22, [sp, #8]
	b.lo	.LBB25_6
// %bb.3:
	add	x1, sp, #8              // =8
	mov	x0, x19
	mov	x2, xzr
	bl	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_createERmm
	ldr	x8, [sp, #8]
	str	x0, [x19]
	str	x8, [x19, #16]
	cbz	x22, .LBB25_8
.LBB25_4:
	cmp	x22, #1                 // =1
	b.ne	.LBB25_7
// %bb.5:
	ldrb	w8, [x21]
	strb	w8, [x0]
	b	.LBB25_8
.LBB25_6:
	mov	x0, x23
	cbnz	x22, .LBB25_4
	b	.LBB25_8
.LBB25_7:
	mov	x1, x21
	mov	x2, x22
	bl	memcpy
.LBB25_8:
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
	b.lo	.LBB25_12
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
.LBB25_11:
	adrp	x0, .L.str.22
	add	x0, x0, :lo12:.L.str.22
	bl	_ZSt19__throw_logic_errorPKc
.LBB25_12:
.Ltmp381:
	adrp	x0, .L.str.23
	add	x0, x0, :lo12:.L.str.23
	bl	_ZSt20__throw_length_errorPKc
.Ltmp382:
// %bb.13:
.LBB25_14:
.Ltmp383:
	ldr	x8, [x19]
	mov	x19, x0
	cmp	x8, x23
	b.eq	.LBB25_16
// %bb.15:
	mov	x0, x8
	bl	_ZdlPv
.LBB25_16:
	mov	x0, x19
	bl	_Unwind_Resume
.Lfunc_end25:
	.size	_ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EERKS8_PKS5_, .Lfunc_end25-_ZStplIcSt11char_traitsIcESaIcEENSt7__cxx1112basic_stringIT_T0_T1_EERKS8_PKS5_
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table25:
.Lexception10:
	.byte	255                     // @LPStart Encoding = omit
	.byte	255                     // @TType Encoding = omit
	.byte	1                       // Call site Encoding = uleb128
	.uleb128 .Lcst_end10-.Lcst_begin10
.Lcst_begin10:
	.uleb128 .Lfunc_begin10-.Lfunc_begin10 // >> Call Site 1 <<
	.uleb128 .Ltmp379-.Lfunc_begin10 //   Call between .Lfunc_begin10 and .Ltmp379
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp379-.Lfunc_begin10 // >> Call Site 2 <<
	.uleb128 .Ltmp380-.Ltmp379      //   Call between .Ltmp379 and .Ltmp380
	.uleb128 .Ltmp383-.Lfunc_begin10 //     jumps to .Ltmp383
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp380-.Lfunc_begin10 // >> Call Site 3 <<
	.uleb128 .Ltmp381-.Ltmp380      //   Call between .Ltmp380 and .Ltmp381
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp381-.Lfunc_begin10 // >> Call Site 4 <<
	.uleb128 .Ltmp382-.Ltmp381      //   Call between .Ltmp381 and .Ltmp382
	.uleb128 .Ltmp383-.Lfunc_begin10 //     jumps to .Ltmp383
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp382-.Lfunc_begin10 // >> Call Site 5 <<
	.uleb128 .Lfunc_end25-.Ltmp382  //   Call between .Ltmp382 and .Lfunc_end25
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
.Lcst_end10:
	.p2align	2
                                        // -- End function
	.section	.text._ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_,"axG",@progbits,_ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_,comdat
	.weak	_ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_ // -- Begin function _ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_
	.p2align	2
	.type	_ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_,@function
_ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_: // @_ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_
.Lfunc_begin11:
	.cfi_startproc
	.cfi_personality 0, __gxx_personality_v0
	.cfi_lsda 0, .Lexception11
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
	cbz	x19, .LBB26_2
// %bb.1:
	add	x8, x19, x19, lsl #1
	lsl	x0, x8, #3
	bl	_Znwm
	mov	x21, x0
	b	.LBB26_3
.LBB26_2:
	mov	x21, xzr
.LBB26_3:
	ldp	x1, x9, [x23]
	orr	w8, wzr, #0x18
	madd	x27, x26, x8, x21
	stp	xzr, xzr, [x27]
	subs	x0, x9, x1
	asr	x28, x0, #3
	str	xzr, [x27, #16]
	str	x25, [sp, #8]           // 8-byte Folded Spill
	b.eq	.LBB26_7
// %bb.4:
	lsr	x8, x28, #61
	cbnz	x8, .LBB26_23
// %bb.5:
.Ltmp384:
	bl	_Znwm
.Ltmp385:
// %bb.6:
	ldp	x1, x8, [x23]
	mov	x25, x0
	sub	x2, x8, x1
	asr	x23, x2, #3
	b	.LBB26_8
.LBB26_7:
	mov	x23, xzr
	mov	x2, xzr
	mov	x25, xzr
.LBB26_8:
	orr	w8, wzr, #0x18
	madd	x26, x26, x8, x21
	str	x25, [x27]
	str	x0, [x26, #8]!
	add	x8, x25, x28, lsl #3
	str	x8, [x26, #8]
	cbz	x2, .LBB26_10
// %bb.9:
	bl	memmove
.LBB26_10:
	add	x8, x25, x23, lsl #3
	cmp	x20, x22
	str	x8, [x26]
	mov	x8, x21
	b.eq	.LBB26_13
// %bb.11:
	mov	x8, x21
	mov	x9, x20
.LBB26_12:                              // =>This Inner Loop Header: Depth=1
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
	b.ne	.LBB26_12
.LBB26_13:
	cmp	x24, x22
	add	x23, x8, #24            // =24
	b.eq	.LBB26_15
.LBB26_14:                              // =>This Inner Loop Header: Depth=1
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
	b.ne	.LBB26_14
.LBB26_15:
	cmp	x20, x24
	b.eq	.LBB26_20
// %bb.16:
	mov	x22, x20
.LBB26_17:                              // =>This Inner Loop Header: Depth=1
	ldr	x0, [x22]
	cbz	x0, .LBB26_19
// %bb.18:                              //   in Loop: Header=BB26_17 Depth=1
	bl	_ZdlPv
.LBB26_19:                              //   in Loop: Header=BB26_17 Depth=1
	add	x22, x22, #24           // =24
	cmp	x24, x22
	b.ne	.LBB26_17
.LBB26_20:
	cbz	x20, .LBB26_22
// %bb.21:
	mov	x0, x20
	bl	_ZdlPv
.LBB26_22:
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
.LBB26_23:
.Ltmp386:
	bl	_ZSt17__throw_bad_allocv
.Ltmp387:
// %bb.24:
.LBB26_25:
.Ltmp388:
	bl	__cxa_begin_catch
	cbnz	x21, .LBB26_27
// %bb.26:
	ldr	x0, [x27]
	cbnz	x0, .LBB26_28
	b	.LBB26_29
.LBB26_27:
	mov	x0, x21
.LBB26_28:
	bl	_ZdlPv
.LBB26_29:
.Ltmp389:
	bl	__cxa_rethrow
.Ltmp390:
// %bb.30:
.LBB26_31:
.Ltmp391:
	mov	x19, x0
.Ltmp392:
	bl	__cxa_end_catch
.Ltmp393:
// %bb.32:
	mov	x0, x19
	bl	_Unwind_Resume
.LBB26_33:
.Ltmp394:
	bl	__clang_call_terminate
.Lfunc_end26:
	.size	_ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_, .Lfunc_end26-_ZNSt6vectorIS_IySaIyEESaIS1_EE17_M_realloc_insertIJRKS1_EEEvN9__gnu_cxx17__normal_iteratorIPS1_S3_EEDpOT_
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table26:
.Lexception11:
	.byte	255                     // @LPStart Encoding = omit
	.byte	0                       // @TType Encoding = absptr
	.uleb128 .Lttbase5-.Lttbaseref5
.Lttbaseref5:
	.byte	1                       // Call site Encoding = uleb128
	.uleb128 .Lcst_end11-.Lcst_begin11
.Lcst_begin11:
	.uleb128 .Lfunc_begin11-.Lfunc_begin11 // >> Call Site 1 <<
	.uleb128 .Ltmp384-.Lfunc_begin11 //   Call between .Lfunc_begin11 and .Ltmp384
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp384-.Lfunc_begin11 // >> Call Site 2 <<
	.uleb128 .Ltmp385-.Ltmp384      //   Call between .Ltmp384 and .Ltmp385
	.uleb128 .Ltmp388-.Lfunc_begin11 //     jumps to .Ltmp388
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp385-.Lfunc_begin11 // >> Call Site 3 <<
	.uleb128 .Ltmp386-.Ltmp385      //   Call between .Ltmp385 and .Ltmp386
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp386-.Lfunc_begin11 // >> Call Site 4 <<
	.uleb128 .Ltmp387-.Ltmp386      //   Call between .Ltmp386 and .Ltmp387
	.uleb128 .Ltmp388-.Lfunc_begin11 //     jumps to .Ltmp388
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp387-.Lfunc_begin11 // >> Call Site 5 <<
	.uleb128 .Ltmp389-.Ltmp387      //   Call between .Ltmp387 and .Ltmp389
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp389-.Lfunc_begin11 // >> Call Site 6 <<
	.uleb128 .Ltmp390-.Ltmp389      //   Call between .Ltmp389 and .Ltmp390
	.uleb128 .Ltmp391-.Lfunc_begin11 //     jumps to .Ltmp391
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp392-.Lfunc_begin11 // >> Call Site 7 <<
	.uleb128 .Ltmp393-.Ltmp392      //   Call between .Ltmp392 and .Ltmp393
	.uleb128 .Ltmp394-.Lfunc_begin11 //     jumps to .Ltmp394
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp393-.Lfunc_begin11 // >> Call Site 8 <<
	.uleb128 .Lfunc_end26-.Ltmp393  //   Call between .Ltmp393 and .Lfunc_end26
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
.Lcst_end11:
	.byte	1                       // >> Action Record 1 <<
                                        //   Catch TypeInfo 1
	.byte	0                       //   No further actions
	.p2align	2
                                        // >> Catch TypeInfos <<
	.xword	0                       // TypeInfo 1
.Lttbase5:
	.p2align	2
                                        // -- End function
	.section	.text._ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_,"axG",@progbits,_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_,comdat
	.weak	_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_ // -- Begin function _ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_
	.p2align	2
	.type	_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_,@function
_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_: // @_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_
.Lfunc_begin12:
	.cfi_startproc
	.cfi_personality 0, __gxx_personality_v0
	.cfi_lsda 0, .Lexception12
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
	b.eq	.LBB27_11
// %bb.1:
	mov	x20, x0
	mov	x21, xzr
	sub	x22, x1, x0
.LBB27_2:                               // =>This Inner Loop Header: Depth=1
	add	x24, x20, x21
	ldp	x8, x9, [x24]
	add	x23, x19, x21
	stp	xzr, xzr, [x23]
	str	xzr, [x23, #16]
	subs	x0, x9, x8
	asr	x25, x0, #3
	b.eq	.LBB27_6
// %bb.3:                               //   in Loop: Header=BB27_2 Depth=1
	lsr	x8, x25, #61
	cbnz	x8, .LBB27_12
// %bb.4:                               //   in Loop: Header=BB27_2 Depth=1
.Ltmp395:
	bl	_Znwm
.Ltmp396:
// %bb.5:                               //   in Loop: Header=BB27_2 Depth=1
	mov	x26, x0
	b	.LBB27_7
.LBB27_6:                               //   in Loop: Header=BB27_2 Depth=1
	mov	x26, xzr
.LBB27_7:                               //   in Loop: Header=BB27_2 Depth=1
	add	x8, x26, x25, lsl #3
	stp	x26, x0, [x23]
	str	x8, [x23, #16]
	ldp	x1, x8, [x24]
	subs	x2, x8, x1
	asr	x24, x2, #3
	b.eq	.LBB27_9
// %bb.8:                               //   in Loop: Header=BB27_2 Depth=1
	bl	memmove
.LBB27_9:                               //   in Loop: Header=BB27_2 Depth=1
	add	x21, x21, #24           // =24
	add	x8, x26, x24, lsl #3
	cmp	x22, x21
	str	x8, [x23, #8]
	b.ne	.LBB27_2
// %bb.10:
	add	x19, x19, x21
.LBB27_11:
	mov	x0, x19
	ldp	x29, x30, [sp, #64]     // 16-byte Folded Reload
	ldp	x20, x19, [sp, #48]     // 16-byte Folded Reload
	ldp	x22, x21, [sp, #32]     // 16-byte Folded Reload
	ldp	x24, x23, [sp, #16]     // 16-byte Folded Reload
	ldp	x26, x25, [sp], #80     // 16-byte Folded Reload
	ret
.LBB27_12:
.Ltmp398:
	bl	_ZSt17__throw_bad_allocv
.Ltmp399:
// %bb.13:
.LBB27_14:
.Ltmp400:
	bl	__cxa_begin_catch
	cbnz	x21, .LBB27_16
	b	.LBB27_19
.LBB27_15:
.Ltmp397:
	bl	__cxa_begin_catch
	cbz	x21, .LBB27_19
.LBB27_16:                              // =>This Inner Loop Header: Depth=1
	ldr	x0, [x19]
	cbz	x0, .LBB27_18
// %bb.17:                              //   in Loop: Header=BB27_16 Depth=1
	bl	_ZdlPv
.LBB27_18:                              //   in Loop: Header=BB27_16 Depth=1
	subs	x21, x21, #24           // =24
	add	x19, x19, #24           // =24
	b.ne	.LBB27_16
.LBB27_19:
.Ltmp401:
	bl	__cxa_rethrow
.Ltmp402:
// %bb.20:
.LBB27_21:
.Ltmp403:
	mov	x19, x0
.Ltmp404:
	bl	__cxa_end_catch
.Ltmp405:
// %bb.22:
	mov	x0, x19
	bl	_Unwind_Resume
.LBB27_23:
.Ltmp406:
	bl	__clang_call_terminate
.Lfunc_end27:
	.size	_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_, .Lfunc_end27-_ZNSt20__uninitialized_copyILb0EE13__uninit_copyIN9__gnu_cxx17__normal_iteratorIPKSt6vectorIySaIyEES4_IS6_SaIS6_EEEEPS6_EET0_T_SE_SD_
	.cfi_endproc
	.section	.gcc_except_table,"a",@progbits
	.p2align	2
GCC_except_table27:
.Lexception12:
	.byte	255                     // @LPStart Encoding = omit
	.byte	0                       // @TType Encoding = absptr
	.uleb128 .Lttbase6-.Lttbaseref6
.Lttbaseref6:
	.byte	1                       // Call site Encoding = uleb128
	.uleb128 .Lcst_end12-.Lcst_begin12
.Lcst_begin12:
	.uleb128 .Ltmp395-.Lfunc_begin12 // >> Call Site 1 <<
	.uleb128 .Ltmp396-.Ltmp395      //   Call between .Ltmp395 and .Ltmp396
	.uleb128 .Ltmp397-.Lfunc_begin12 //     jumps to .Ltmp397
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp396-.Lfunc_begin12 // >> Call Site 2 <<
	.uleb128 .Ltmp398-.Ltmp396      //   Call between .Ltmp396 and .Ltmp398
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp398-.Lfunc_begin12 // >> Call Site 3 <<
	.uleb128 .Ltmp399-.Ltmp398      //   Call between .Ltmp398 and .Ltmp399
	.uleb128 .Ltmp400-.Lfunc_begin12 //     jumps to .Ltmp400
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp399-.Lfunc_begin12 // >> Call Site 4 <<
	.uleb128 .Ltmp401-.Ltmp399      //   Call between .Ltmp399 and .Ltmp401
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp401-.Lfunc_begin12 // >> Call Site 5 <<
	.uleb128 .Ltmp402-.Ltmp401      //   Call between .Ltmp401 and .Ltmp402
	.uleb128 .Ltmp403-.Lfunc_begin12 //     jumps to .Ltmp403
	.byte	0                       //   On action: cleanup
	.uleb128 .Ltmp404-.Lfunc_begin12 // >> Call Site 6 <<
	.uleb128 .Ltmp405-.Ltmp404      //   Call between .Ltmp404 and .Ltmp405
	.uleb128 .Ltmp406-.Lfunc_begin12 //     jumps to .Ltmp406
	.byte	1                       //   On action: 1
	.uleb128 .Ltmp405-.Lfunc_begin12 // >> Call Site 7 <<
	.uleb128 .Lfunc_end27-.Ltmp405  //   Call between .Ltmp405 and .Lfunc_end27
	.byte	0                       //     has no landing pad
	.byte	0                       //   On action: cleanup
.Lcst_end12:
	.byte	1                       // >> Action Record 1 <<
                                        //   Catch TypeInfo 1
	.byte	0                       //   No further actions
	.p2align	2
                                        // >> Catch TypeInfos <<
	.xword	0                       // TypeInfo 1
.Lttbase6:
	.p2align	2
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
.Lfunc_end28:
	.size	_GLOBAL__sub_I_bitmapdecoding.cpp, .Lfunc_end28-_GLOBAL__sub_I_bitmapdecoding.cpp
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
