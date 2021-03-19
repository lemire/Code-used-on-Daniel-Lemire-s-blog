	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 11, 0	sdk_version 11, 1
	.globl	__Z5printNSt3__14pairI20performance_countersS1_EE ; -- Begin function _Z5printNSt3__14pairI20performance_countersS1_EE
	.p2align	2
__Z5printNSt3__14pairI20performance_countersS1_EE: ; @_Z5printNSt3__14pairI20performance_countersS1_EE
	.cfi_startproc
; %bb.0:
	stp	d15, d14, [sp, #-128]!  ; 16-byte Folded Spill
	stp	d13, d12, [sp, #16]     ; 16-byte Folded Spill
	stp	d11, d10, [sp, #32]     ; 16-byte Folded Spill
	stp	d9, d8, [sp, #48]       ; 16-byte Folded Spill
	stp	x24, x23, [sp, #64]     ; 16-byte Folded Spill
	stp	x22, x21, [sp, #80]     ; 16-byte Folded Spill
	stp	x20, x19, [sp, #96]     ; 16-byte Folded Spill
	stp	x29, x30, [sp, #112]    ; 16-byte Folded Spill
	add	x29, sp, #112           ; =112
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -56
	.cfi_offset w24, -64
	.cfi_offset b8, -72
	.cfi_offset b9, -80
	.cfi_offset b10, -88
	.cfi_offset b11, -96
	.cfi_offset b12, -104
	.cfi_offset b13, -112
	.cfi_offset b14, -120
	.cfi_offset b15, -128
	ldp	d11, d10, [x0]
	ldp	d9, d8, [x0, #16]
	ldp	d15, d14, [x0, #32]
	ldp	d13, d12, [x0, #48]
Lloh0:
	adrp	x19, __ZNSt3__14coutE@GOTPAGE
Lloh1:
	ldr	x19, [x19, __ZNSt3__14coutE@GOTPAGEOFF]
	ldr	x8, [x19]
	ldur	x9, [x8, #-24]
	add	x9, x19, x9
	ldr	w10, [x9, #8]
	and	w10, w10, #0xfffffeff
	orr	w10, w10, #0x4
	str	w10, [x9, #8]
	ldur	x9, [x8, #-24]
	add	x9, x19, x9
	mov	w22, #4
	str	x22, [x9, #16]
	ldur	x8, [x8, #-24]
	add	x8, x19, x8
	mov	w21, #48
	str	w21, [x8, #144]
	mov	x0, x19
	mov.16b	v0, v11
	bl	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEElsEd
Lloh2:
	adrp	x1, l_.str@PAGE
Lloh3:
	add	x1, x1, l_.str@PAGEOFF
	mov	w2, #15
	bl	__ZNSt3__124__put_character_sequenceIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m
	ldr	x8, [x0]
	ldur	x9, [x8, #-24]
	add	x9, x0, x9
	mov	w23, #2
	str	x23, [x9, #16]
	ldur	x8, [x8, #-24]
	add	x8, x0, x8
	str	w21, [x8, #144]
	fsub	d0, d15, d11
	fdiv	d0, d0, d11
	mov	x8, #4636737291354636288
	fmov	d11, x8
	fmul	d0, d0, d11
	bl	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEElsEd
Lloh4:
	adrp	x20, l_.str.1@PAGE
Lloh5:
	add	x20, x20, l_.str.1@PAGEOFF
	mov	x1, x20
	mov	w2, #4
	bl	__ZNSt3__124__put_character_sequenceIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m
	ldr	x8, [x19]
	ldur	x9, [x8, #-24]
	add	x9, x19, x9
	str	x22, [x9, #16]
	ldur	x8, [x8, #-24]
	add	x8, x19, x8
	str	w21, [x8, #144]
	mov	x0, x19
	mov.16b	v0, v10
	bl	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEElsEd
Lloh6:
	adrp	x1, l_.str.2@PAGE
Lloh7:
	add	x1, x1, l_.str.2@PAGEOFF
	mov	w2, #17
	bl	__ZNSt3__124__put_character_sequenceIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m
	ldr	x8, [x0]
	ldur	x9, [x8, #-24]
	add	x9, x0, x9
	str	x23, [x9, #16]
	ldur	x8, [x8, #-24]
	add	x8, x0, x8
	str	w21, [x8, #144]
	fsub	d0, d14, d10
	fdiv	d0, d0, d10
	fmul	d0, d0, d11
	bl	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEElsEd
	mov	x1, x20
	mov	w2, #4
	bl	__ZNSt3__124__put_character_sequenceIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m
	ldr	x8, [x19]
	ldur	x9, [x8, #-24]
	add	x9, x19, x9
	str	x22, [x9, #16]
	ldur	x8, [x8, #-24]
	add	x8, x19, x8
	str	w21, [x8, #144]
	mov	x0, x19
	mov.16b	v0, v9
	bl	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEElsEd
Lloh8:
	adrp	x1, l_.str.3@PAGE
Lloh9:
	add	x1, x1, l_.str.3@PAGEOFF
	mov	w2, #24
	bl	__ZNSt3__124__put_character_sequenceIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m
	ldr	x8, [x0]
	ldur	x9, [x8, #-24]
	add	x9, x0, x9
	str	x23, [x9, #16]
	ldur	x8, [x8, #-24]
	add	x8, x0, x8
	str	w21, [x8, #144]
	fsub	d0, d13, d9
	fdiv	d0, d0, d9
	fmul	d0, d0, d11
	bl	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEElsEd
	mov	x1, x20
	mov	w2, #4
	bl	__ZNSt3__124__put_character_sequenceIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m
	ldr	x8, [x19]
	ldur	x9, [x8, #-24]
	add	x9, x19, x9
	str	x22, [x9, #16]
	ldur	x8, [x8, #-24]
	add	x8, x19, x8
	str	w21, [x8, #144]
	mov	x0, x19
	mov.16b	v0, v8
	bl	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEElsEd
Lloh10:
	adrp	x1, l_.str.4@PAGE
Lloh11:
	add	x1, x1, l_.str.4@PAGEOFF
	mov	w2, #21
	bl	__ZNSt3__124__put_character_sequenceIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m
	ldr	x8, [x0]
	ldur	x9, [x8, #-24]
	add	x9, x0, x9
	str	x23, [x9, #16]
	ldur	x8, [x8, #-24]
	add	x8, x0, x8
	str	w21, [x8, #144]
	fsub	d0, d12, d8
	fdiv	d0, d0, d8
	fmul	d0, d0, d11
	bl	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEElsEd
Lloh12:
	adrp	x1, l_.str.5@PAGE
Lloh13:
	add	x1, x1, l_.str.5@PAGEOFF
	mov	w2, #3
	ldp	x29, x30, [sp, #112]    ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #96]     ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #80]     ; 16-byte Folded Reload
	ldp	x24, x23, [sp, #64]     ; 16-byte Folded Reload
	ldp	d9, d8, [sp, #48]       ; 16-byte Folded Reload
	ldp	d11, d10, [sp, #32]     ; 16-byte Folded Reload
	ldp	d13, d12, [sp, #16]     ; 16-byte Folded Reload
	ldp	d15, d14, [sp], #128    ; 16-byte Folded Reload
	b	__ZNSt3__124__put_character_sequenceIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m
	.loh AdrpAdd	Lloh12, Lloh13
	.loh AdrpAdd	Lloh10, Lloh11
	.loh AdrpAdd	Lloh8, Lloh9
	.loh AdrpAdd	Lloh6, Lloh7
	.loh AdrpAdd	Lloh4, Lloh5
	.loh AdrpAdd	Lloh2, Lloh3
	.loh AdrpLdrGot	Lloh0, Lloh1
	.cfi_endproc
                                        ; -- End function
	.globl	_main                   ; -- Begin function main
	.p2align	2
_main:                                  ; @main
Lfunc_begin0:
	.cfi_startproc
	.cfi_personality 155, ___gxx_personality_v0
	.cfi_lsda 16, Lexception0
; %bb.0:
	stp	x26, x25, [sp, #-80]!   ; 16-byte Folded Spill
	stp	x24, x23, [sp, #16]     ; 16-byte Folded Spill
	stp	x22, x21, [sp, #32]     ; 16-byte Folded Spill
	stp	x20, x19, [sp, #48]     ; 16-byte Folded Spill
	stp	x29, x30, [sp, #64]     ; 16-byte Folded Spill
	add	x29, sp, #64            ; =64
	mov	w9, #29136
	movk	w9, #2, lsl #16
Lloh14:
	adrp	x16, ___chkstk_darwin@GOTPAGE
Lloh15:
	ldr	x16, [x16, ___chkstk_darwin@GOTPAGEOFF]
	blr	x16
	sub	sp, sp, #39, lsl #12    ; =159744
	sub	sp, sp, #464            ; =464
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
Lloh16:
	adrp	x8, ___stack_chk_guard@GOTPAGE
Lloh17:
	ldr	x8, [x8, ___stack_chk_guard@GOTPAGEOFF]
Lloh18:
	ldr	x8, [x8]
	stur	x8, [x29, #-72]
	bl	__Z26setup_performance_countersv
Lloh19:
	adrp	x19, __ZNSt3__14coutE@GOTPAGE
Lloh20:
	ldr	x19, [x19, __ZNSt3__14coutE@GOTPAGEOFF]
Lloh21:
	adrp	x1, l_.str.6@PAGE
Lloh22:
	add	x1, x1, l_.str.6@PAGEOFF
	mov	x0, x19
	mov	w2, #12
	bl	__ZNSt3__124__put_character_sequenceIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m
	mov	w1, #156
	bl	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEElsEm
Lloh23:
	adrp	x1, l_.str.7@PAGE
Lloh24:
	add	x1, x1, l_.str.7@PAGEOFF
	mov	w2, #3
	bl	__ZNSt3__124__put_character_sequenceIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m
	mov	x20, x0
	ldr	x8, [x0]
	ldur	x8, [x8, #-24]
	add	x0, x0, x8
	add	x8, sp, #200            ; =200
	bl	__ZNKSt3__18ios_base6getlocEv
Ltmp0:
Lloh25:
	adrp	x1, __ZNSt3__15ctypeIcE2idE@GOTPAGE
Lloh26:
	ldr	x1, [x1, __ZNSt3__15ctypeIcE2idE@GOTPAGEOFF]
	add	x0, sp, #200            ; =200
	bl	__ZNKSt3__16locale9use_facetERNS0_2idE
Ltmp1:
; %bb.1:
	ldr	x8, [x0]
	ldr	x8, [x8, #56]
Ltmp2:
	mov	w1, #10
	blr	x8
Ltmp3:
; %bb.2:
	mov	x21, x0
	add	x0, sp, #200            ; =200
	bl	__ZNSt3__16localeD1Ev
	mov	x0, x20
	mov	x1, x21
	bl	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEE3putEc
	mov	x0, x20
	bl	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEE5flushEv
	add	x8, sp, #136            ; =136
	add	x0, sp, #200            ; =200
	mov	w1, #20000
	mov	w2, #40000
	bl	__Z13time_populateI18splitmix_generatorENSt3__14pairI20performance_countersS3_EEPymm
	add	x8, sp, #136            ; =136
	add	x0, sp, #200            ; =200
	mov	w1, #20000
	mov	w2, #40000
	bl	__Z13time_populateI15wyrng_generatorENSt3__14pairI20performance_countersS3_EEPymm
	mov	w25, #4
Lloh27:
	adrp	x20, l_.str.8@PAGE
Lloh28:
	add	x20, x20, l_.str.8@PAGEOFF
Lloh29:
	adrp	x21, __ZNSt3__15ctypeIcE2idE@GOTPAGE
Lloh30:
	ldr	x21, [x21, __ZNSt3__15ctypeIcE2idE@GOTPAGEOFF]
Lloh31:
	adrp	x22, l_.str.9@PAGE
Lloh32:
	add	x22, x22, l_.str.9@PAGEOFF
Lloh33:
	adrp	x23, l_.str.10@PAGE
Lloh34:
	add	x23, x23, l_.str.10@PAGEOFF
LBB1_3:                                 ; =>This Inner Loop Header: Depth=1
	mov	x0, x19
	mov	x1, x20
	mov	w2, #9
	bl	__ZNSt3__124__put_character_sequenceIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m
	add	x8, sp, #72             ; =72
	add	x0, sp, #200            ; =200
	mov	w1, #20000
	mov	w2, #40000
	bl	__Z13time_populateI18splitmix_generatorENSt3__14pairI20performance_countersS3_EEPymm
	add	x0, sp, #72             ; =72
	bl	__Z5printNSt3__14pairI20performance_countersS1_EE
	ldr	x8, [x19]
	ldur	x8, [x8, #-24]
	add	x0, x19, x8
	add	x8, sp, #136            ; =136
	bl	__ZNKSt3__18ios_base6getlocEv
Ltmp5:
	add	x0, sp, #136            ; =136
	mov	x1, x21
	bl	__ZNKSt3__16locale9use_facetERNS0_2idE
Ltmp6:
; %bb.4:                                ;   in Loop: Header=BB1_3 Depth=1
	ldr	x8, [x0]
	ldr	x8, [x8, #56]
Ltmp7:
	mov	w1, #10
	blr	x8
Ltmp8:
; %bb.5:                                ;   in Loop: Header=BB1_3 Depth=1
	mov	x24, x0
	add	x0, sp, #136            ; =136
	bl	__ZNSt3__16localeD1Ev
	mov	x0, x19
	mov	x1, x24
	bl	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEE3putEc
	mov	x0, x19
	bl	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEE5flushEv
	mov	x0, x19
	mov	x1, x22
	mov	w2, #9
	bl	__ZNSt3__124__put_character_sequenceIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m
	add	x8, sp, #8              ; =8
	add	x0, sp, #200            ; =200
	mov	w1, #20000
	mov	w2, #40000
	bl	__Z13time_populateI15wyrng_generatorENSt3__14pairI20performance_countersS3_EEPymm
	add	x0, sp, #8              ; =8
	bl	__Z5printNSt3__14pairI20performance_countersS1_EE
	ldr	x8, [x19]
	ldur	x8, [x8, #-24]
	add	x0, x19, x8
	add	x8, sp, #136            ; =136
	bl	__ZNKSt3__18ios_base6getlocEv
Ltmp10:
	add	x0, sp, #136            ; =136
	mov	x1, x21
	bl	__ZNKSt3__16locale9use_facetERNS0_2idE
Ltmp11:
; %bb.6:                                ;   in Loop: Header=BB1_3 Depth=1
	ldr	x8, [x0]
	ldr	x8, [x8, #56]
Ltmp12:
	mov	w1, #10
	blr	x8
Ltmp13:
; %bb.7:                                ;   in Loop: Header=BB1_3 Depth=1
	mov	x24, x0
	add	x0, sp, #136            ; =136
	bl	__ZNSt3__16localeD1Ev
	mov	x0, x19
	mov	x1, x24
	bl	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEE3putEc
	mov	x0, x19
	bl	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEE5flushEv
	mov	x0, x19
	mov	x1, x23
	mov	w2, #12
	bl	__ZNSt3__124__put_character_sequenceIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m
	subs	x25, x25, #1            ; =1
	b.ne	LBB1_3
; %bb.8:
	ldur	x8, [x29, #-72]
Lloh35:
	adrp	x9, ___stack_chk_guard@GOTPAGE
Lloh36:
	ldr	x9, [x9, ___stack_chk_guard@GOTPAGEOFF]
Lloh37:
	ldr	x9, [x9]
	cmp	x9, x8
	b.ne	LBB1_10
; %bb.9:
	mov	w0, #0
	add	sp, sp, #39, lsl #12    ; =159744
	add	sp, sp, #464            ; =464
	ldp	x29, x30, [sp, #64]     ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #48]     ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #32]     ; 16-byte Folded Reload
	ldp	x24, x23, [sp, #16]     ; 16-byte Folded Reload
	ldp	x26, x25, [sp], #80     ; 16-byte Folded Reload
	ret
LBB1_10:
	bl	___stack_chk_fail
LBB1_11:
Ltmp4:
	mov	x19, x0
	add	x0, sp, #200            ; =200
	b	LBB1_15
LBB1_12:
Ltmp14:
	b	LBB1_14
LBB1_13:
Ltmp9:
LBB1_14:
	mov	x19, x0
	add	x0, sp, #136            ; =136
LBB1_15:
	bl	__ZNSt3__16localeD1Ev
	mov	x0, x19
	bl	__Unwind_Resume
	brk	#0x1
	.loh AdrpLdrGot	Lloh25, Lloh26
	.loh AdrpAdd	Lloh23, Lloh24
	.loh AdrpAdd	Lloh21, Lloh22
	.loh AdrpLdrGot	Lloh19, Lloh20
	.loh AdrpLdrGotLdr	Lloh16, Lloh17, Lloh18
	.loh AdrpLdrGot	Lloh14, Lloh15
	.loh AdrpAdd	Lloh33, Lloh34
	.loh AdrpAdd	Lloh31, Lloh32
	.loh AdrpLdrGot	Lloh29, Lloh30
	.loh AdrpAdd	Lloh27, Lloh28
	.loh AdrpLdrGotLdr	Lloh35, Lloh36, Lloh37
Lfunc_end0:
	.cfi_endproc
	.section	__TEXT,__gcc_except_tab
	.p2align	2
GCC_except_table1:
Lexception0:
	.byte	255                     ; @LPStart Encoding = omit
	.byte	255                     ; @TType Encoding = omit
	.byte	1                       ; Call site Encoding = uleb128
	.uleb128 Lcst_end0-Lcst_begin0
Lcst_begin0:
	.uleb128 Lfunc_begin0-Lfunc_begin0 ; >> Call Site 1 <<
	.uleb128 Ltmp0-Lfunc_begin0     ;   Call between Lfunc_begin0 and Ltmp0
	.byte	0                       ;     has no landing pad
	.byte	0                       ;   On action: cleanup
	.uleb128 Ltmp0-Lfunc_begin0     ; >> Call Site 2 <<
	.uleb128 Ltmp3-Ltmp0            ;   Call between Ltmp0 and Ltmp3
	.uleb128 Ltmp4-Lfunc_begin0     ;     jumps to Ltmp4
	.byte	0                       ;   On action: cleanup
	.uleb128 Ltmp3-Lfunc_begin0     ; >> Call Site 3 <<
	.uleb128 Ltmp5-Ltmp3            ;   Call between Ltmp3 and Ltmp5
	.byte	0                       ;     has no landing pad
	.byte	0                       ;   On action: cleanup
	.uleb128 Ltmp5-Lfunc_begin0     ; >> Call Site 4 <<
	.uleb128 Ltmp8-Ltmp5            ;   Call between Ltmp5 and Ltmp8
	.uleb128 Ltmp9-Lfunc_begin0     ;     jumps to Ltmp9
	.byte	0                       ;   On action: cleanup
	.uleb128 Ltmp8-Lfunc_begin0     ; >> Call Site 5 <<
	.uleb128 Ltmp10-Ltmp8           ;   Call between Ltmp8 and Ltmp10
	.byte	0                       ;     has no landing pad
	.byte	0                       ;   On action: cleanup
	.uleb128 Ltmp10-Lfunc_begin0    ; >> Call Site 6 <<
	.uleb128 Ltmp13-Ltmp10          ;   Call between Ltmp10 and Ltmp13
	.uleb128 Ltmp14-Lfunc_begin0    ;     jumps to Ltmp14
	.byte	0                       ;   On action: cleanup
	.uleb128 Ltmp13-Lfunc_begin0    ; >> Call Site 7 <<
	.uleb128 Lfunc_end0-Ltmp13      ;   Call between Ltmp13 and Lfunc_end0
	.byte	0                       ;     has no landing pad
	.byte	0                       ;   On action: cleanup
Lcst_end0:
	.p2align	2
                                        ; -- End function
	.section	__TEXT,__literal8,8byte_literals
	.p2align	3               ; -- Begin function _Z13time_populateI18splitmix_generatorENSt3__14pairI20performance_countersS3_EEPymm
lCPI2_0:
	.quad	9094988921128908188     ; double 1.0000000000000001E+300
	.section	__TEXT,__text,regular,pure_instructions
	.globl	__Z13time_populateI18splitmix_generatorENSt3__14pairI20performance_countersS3_EEPymm
	.weak_def_can_be_hidden	__Z13time_populateI18splitmix_generatorENSt3__14pairI20performance_countersS3_EEPymm
	.p2align	2
__Z13time_populateI18splitmix_generatorENSt3__14pairI20performance_countersS3_EEPymm: ; @_Z13time_populateI18splitmix_generatorENSt3__14pairI20performance_countersS3_EEPymm
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #176            ; =176
	stp	d15, d14, [sp, #32]     ; 16-byte Folded Spill
	stp	d13, d12, [sp, #48]     ; 16-byte Folded Spill
	stp	d11, d10, [sp, #64]     ; 16-byte Folded Spill
	stp	d9, d8, [sp, #80]       ; 16-byte Folded Spill
	stp	x26, x25, [sp, #96]     ; 16-byte Folded Spill
	stp	x24, x23, [sp, #112]    ; 16-byte Folded Spill
	stp	x22, x21, [sp, #128]    ; 16-byte Folded Spill
	stp	x20, x19, [sp, #144]    ; 16-byte Folded Spill
	stp	x29, x30, [sp, #160]    ; 16-byte Folded Spill
	add	x29, sp, #160           ; =160
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
	.cfi_offset b8, -88
	.cfi_offset b9, -96
	.cfi_offset b10, -104
	.cfi_offset b11, -112
	.cfi_offset b12, -120
	.cfi_offset b13, -128
	.cfi_offset b14, -136
	.cfi_offset b15, -144
	mov	x21, x2
	mov	x20, x1
	mov	x19, x8
	adrp	x8, lCPI2_0@PAGE
	cbz	x2, LBB2_3
; %bb.1:
	mov	x22, x0
	sub	x24, x20, #1            ; =1
	movi.2d	v11, #0000000000000000
	ldr	d13, [x8, lCPI2_0@PAGEOFF]
	mov	w23, #12345
	adrp	x25, _hole@PAGE
	mov	x26, x21
	mov.16b	v15, v13
	mov.16b	v12, v13
	mov.16b	v14, v13
	movi.2d	v8, #0000000000000000
	movi.2d	v10, #0000000000000000
	movi.2d	v9, #0000000000000000
LBB2_2:                                 ; =>This Inner Loop Header: Depth=1
	bl	__Z12get_countersv
	stp	d1, d0, [sp, #16]       ; 16-byte Folded Spill
	stp	d3, d2, [sp]            ; 16-byte Folded Spill
	mov	x0, x22
	mov	x1, x20
	mov	x2, x23
	bl	__Z8populateI18splitmix_generatorEvPymy
	ldr	x8, [x22, x24, lsl #3]
	str	x8, [x25, _hole@PAGEOFF]
	bl	__Z12get_countersv
	ldr	d5, [sp, #24]           ; 8-byte Folded Reload
	fsub	d0, d0, d5
	ldr	d5, [sp, #16]           ; 8-byte Folded Reload
	fsub	d1, d1, d5
	ldr	d5, [sp, #8]            ; 8-byte Folded Reload
	fsub	d2, d2, d5
	ldr	d5, [sp]                ; 8-byte Folded Reload
	fsub	d3, d3, d5
	fadd	d9, d9, d0
	fadd	d10, d10, d1
	fadd	d8, d8, d2
	fadd	d11, d11, d3
	fcmp	d0, d14
	fcsel	d14, d0, d14, mi
	fcmp	d1, d12
	fcsel	d12, d1, d12, mi
	fcmp	d2, d15
	fcsel	d15, d2, d15, mi
	fcmp	d3, d13
	fcsel	d13, d3, d13, mi
	add	x23, x23, #1            ; =1
	subs	x26, x26, #1            ; =1
	b.ne	LBB2_2
	b	LBB2_4
LBB2_3:
	movi.2d	v9, #0000000000000000
	ldr	d14, [x8, lCPI2_0@PAGEOFF]
	movi.2d	v10, #0000000000000000
	movi.2d	v8, #0000000000000000
	movi.2d	v11, #0000000000000000
	mov.16b	v12, v14
	mov.16b	v15, v14
	mov.16b	v13, v14
LBB2_4:
	ucvtf	d0, x21
	fdiv	d1, d9, d0
	fdiv	d2, d10, d0
	fdiv	d3, d8, d0
	fdiv	d0, d11, d0
	ucvtf	d4, x20
	fdiv	d5, d14, d4
	fdiv	d6, d12, d4
	fdiv	d7, d15, d4
	fdiv	d16, d13, d4
	fdiv	d1, d1, d4
	fdiv	d2, d2, d4
	fdiv	d3, d3, d4
	stp	d5, d6, [x19]
	stp	d7, d16, [x19, #16]
	stp	d1, d2, [x19, #32]
	fdiv	d0, d0, d4
	stp	d3, d0, [x19, #48]
	ldp	x29, x30, [sp, #160]    ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #144]    ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #128]    ; 16-byte Folded Reload
	ldp	x24, x23, [sp, #112]    ; 16-byte Folded Reload
	ldp	x26, x25, [sp, #96]     ; 16-byte Folded Reload
	ldp	d9, d8, [sp, #80]       ; 16-byte Folded Reload
	ldp	d11, d10, [sp, #64]     ; 16-byte Folded Reload
	ldp	d13, d12, [sp, #48]     ; 16-byte Folded Reload
	ldp	d15, d14, [sp, #32]     ; 16-byte Folded Reload
	add	sp, sp, #176            ; =176
	ret
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__literal8,8byte_literals
	.p2align	3               ; -- Begin function _Z13time_populateI15wyrng_generatorENSt3__14pairI20performance_countersS3_EEPymm
lCPI3_0:
	.quad	9094988921128908188     ; double 1.0000000000000001E+300
	.section	__TEXT,__text,regular,pure_instructions
	.globl	__Z13time_populateI15wyrng_generatorENSt3__14pairI20performance_countersS3_EEPymm
	.weak_def_can_be_hidden	__Z13time_populateI15wyrng_generatorENSt3__14pairI20performance_countersS3_EEPymm
	.p2align	2
__Z13time_populateI15wyrng_generatorENSt3__14pairI20performance_countersS3_EEPymm: ; @_Z13time_populateI15wyrng_generatorENSt3__14pairI20performance_countersS3_EEPymm
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #176            ; =176
	stp	d15, d14, [sp, #32]     ; 16-byte Folded Spill
	stp	d13, d12, [sp, #48]     ; 16-byte Folded Spill
	stp	d11, d10, [sp, #64]     ; 16-byte Folded Spill
	stp	d9, d8, [sp, #80]       ; 16-byte Folded Spill
	stp	x26, x25, [sp, #96]     ; 16-byte Folded Spill
	stp	x24, x23, [sp, #112]    ; 16-byte Folded Spill
	stp	x22, x21, [sp, #128]    ; 16-byte Folded Spill
	stp	x20, x19, [sp, #144]    ; 16-byte Folded Spill
	stp	x29, x30, [sp, #160]    ; 16-byte Folded Spill
	add	x29, sp, #160           ; =160
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
	.cfi_offset b8, -88
	.cfi_offset b9, -96
	.cfi_offset b10, -104
	.cfi_offset b11, -112
	.cfi_offset b12, -120
	.cfi_offset b13, -128
	.cfi_offset b14, -136
	.cfi_offset b15, -144
	mov	x21, x2
	mov	x20, x1
	mov	x19, x8
	adrp	x8, lCPI3_0@PAGE
	cbz	x2, LBB3_3
; %bb.1:
	mov	x22, x0
	sub	x24, x20, #1            ; =1
	movi.2d	v11, #0000000000000000
	ldr	d13, [x8, lCPI3_0@PAGEOFF]
	mov	w23, #12345
	adrp	x25, _hole@PAGE
	mov	x26, x21
	mov.16b	v15, v13
	mov.16b	v12, v13
	mov.16b	v14, v13
	movi.2d	v8, #0000000000000000
	movi.2d	v10, #0000000000000000
	movi.2d	v9, #0000000000000000
LBB3_2:                                 ; =>This Inner Loop Header: Depth=1
	bl	__Z12get_countersv
	stp	d1, d0, [sp, #16]       ; 16-byte Folded Spill
	stp	d3, d2, [sp]            ; 16-byte Folded Spill
	mov	x0, x22
	mov	x1, x20
	mov	x2, x23
	bl	__Z8populateI15wyrng_generatorEvPymy
	ldr	x8, [x22, x24, lsl #3]
	str	x8, [x25, _hole@PAGEOFF]
	bl	__Z12get_countersv
	ldr	d5, [sp, #24]           ; 8-byte Folded Reload
	fsub	d0, d0, d5
	ldr	d5, [sp, #16]           ; 8-byte Folded Reload
	fsub	d1, d1, d5
	ldr	d5, [sp, #8]            ; 8-byte Folded Reload
	fsub	d2, d2, d5
	ldr	d5, [sp]                ; 8-byte Folded Reload
	fsub	d3, d3, d5
	fadd	d9, d9, d0
	fadd	d10, d10, d1
	fadd	d8, d8, d2
	fadd	d11, d11, d3
	fcmp	d0, d14
	fcsel	d14, d0, d14, mi
	fcmp	d1, d12
	fcsel	d12, d1, d12, mi
	fcmp	d2, d15
	fcsel	d15, d2, d15, mi
	fcmp	d3, d13
	fcsel	d13, d3, d13, mi
	add	x23, x23, #1            ; =1
	subs	x26, x26, #1            ; =1
	b.ne	LBB3_2
	b	LBB3_4
LBB3_3:
	movi.2d	v9, #0000000000000000
	ldr	d14, [x8, lCPI3_0@PAGEOFF]
	movi.2d	v10, #0000000000000000
	movi.2d	v8, #0000000000000000
	movi.2d	v11, #0000000000000000
	mov.16b	v12, v14
	mov.16b	v15, v14
	mov.16b	v13, v14
LBB3_4:
	ucvtf	d0, x21
	fdiv	d1, d9, d0
	fdiv	d2, d10, d0
	fdiv	d3, d8, d0
	fdiv	d0, d11, d0
	ucvtf	d4, x20
	fdiv	d5, d14, d4
	fdiv	d6, d12, d4
	fdiv	d7, d15, d4
	fdiv	d16, d13, d4
	fdiv	d1, d1, d4
	fdiv	d2, d2, d4
	fdiv	d3, d3, d4
	stp	d5, d6, [x19]
	stp	d7, d16, [x19, #16]
	stp	d1, d2, [x19, #32]
	fdiv	d0, d0, d4
	stp	d3, d0, [x19, #48]
	ldp	x29, x30, [sp, #160]    ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #144]    ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #128]    ; 16-byte Folded Reload
	ldp	x24, x23, [sp, #112]    ; 16-byte Folded Reload
	ldp	x26, x25, [sp, #96]     ; 16-byte Folded Reload
	ldp	d9, d8, [sp, #80]       ; 16-byte Folded Reload
	ldp	d11, d10, [sp, #64]     ; 16-byte Folded Reload
	ldp	d13, d12, [sp, #48]     ; 16-byte Folded Reload
	ldp	d15, d14, [sp, #32]     ; 16-byte Folded Reload
	add	sp, sp, #176            ; =176
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	__ZNSt3__124__put_character_sequenceIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m ; -- Begin function _ZNSt3__124__put_character_sequenceIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m
	.weak_def_can_be_hidden	__ZNSt3__124__put_character_sequenceIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m
	.p2align	2
__ZNSt3__124__put_character_sequenceIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m: ; @_ZNSt3__124__put_character_sequenceIcNS_11char_traitsIcEEEERNS_13basic_ostreamIT_T0_EES7_PKS4_m
Lfunc_begin1:
	.cfi_startproc
	.cfi_personality 155, ___gxx_personality_v0
	.cfi_lsda 16, Lexception1
; %bb.0:
	sub	sp, sp, #112            ; =112
	stp	x26, x25, [sp, #32]     ; 16-byte Folded Spill
	stp	x24, x23, [sp, #48]     ; 16-byte Folded Spill
	stp	x22, x21, [sp, #64]     ; 16-byte Folded Spill
	stp	x20, x19, [sp, #80]     ; 16-byte Folded Spill
	stp	x29, x30, [sp, #96]     ; 16-byte Folded Spill
	add	x29, sp, #96            ; =96
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
	mov	x21, x2
	mov	x20, x1
	mov	x19, x0
Ltmp15:
	add	x0, sp, #8              ; =8
	mov	x1, x19
	bl	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEE6sentryC1ERS3_
Ltmp16:
; %bb.1:
	ldrb	w8, [sp, #8]
	cbz	w8, LBB4_10
; %bb.2:
	ldr	x8, [x19]
	ldur	x8, [x8, #-24]
	add	x22, x19, x8
	ldr	x23, [x22, #40]
	ldr	w25, [x22, #8]
	ldr	w24, [x22, #144]
	cmn	w24, #1                 ; =1
	b.ne	LBB4_7
; %bb.3:
Ltmp18:
	add	x8, sp, #24             ; =24
	mov	x0, x22
	bl	__ZNKSt3__18ios_base6getlocEv
Ltmp19:
; %bb.4:
Ltmp20:
Lloh38:
	adrp	x1, __ZNSt3__15ctypeIcE2idE@GOTPAGE
Lloh39:
	ldr	x1, [x1, __ZNSt3__15ctypeIcE2idE@GOTPAGEOFF]
	add	x0, sp, #24             ; =24
	bl	__ZNKSt3__16locale9use_facetERNS0_2idE
Ltmp21:
; %bb.5:
	ldr	x8, [x0]
	ldr	x8, [x8, #56]
Ltmp22:
	mov	w1, #32
	blr	x8
Ltmp23:
; %bb.6:
	mov	x24, x0
	add	x0, sp, #24             ; =24
	bl	__ZNSt3__16localeD1Ev
	str	w24, [x22, #144]
LBB4_7:
	add	x3, x20, x21
	mov	w8, #176
	and	w8, w25, w8
	cmp	w8, #32                 ; =32
	csel	x2, x3, x20, eq
Ltmp25:
	sxtb	w5, w24
	mov	x0, x23
	mov	x1, x20
	mov	x4, x22
	bl	__ZNSt3__116__pad_and_outputIcNS_11char_traitsIcEEEENS_19ostreambuf_iteratorIT_T0_EES6_PKS4_S8_S8_RNS_8ios_baseES4_
Ltmp26:
; %bb.8:
	cbnz	x0, LBB4_10
; %bb.9:
	ldr	x8, [x19]
	ldur	x8, [x8, #-24]
	add	x0, x19, x8
	ldr	w8, [x0, #32]
	mov	w9, #5
	orr	w1, w8, w9
Ltmp28:
	bl	__ZNSt3__18ios_base5clearEj
Ltmp29:
LBB4_10:
	add	x0, sp, #8              ; =8
	bl	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEE6sentryD1Ev
LBB4_11:
	mov	x0, x19
	ldp	x29, x30, [sp, #96]     ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #80]     ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #64]     ; 16-byte Folded Reload
	ldp	x24, x23, [sp, #48]     ; 16-byte Folded Reload
	ldp	x26, x25, [sp, #32]     ; 16-byte Folded Reload
	add	sp, sp, #112            ; =112
	ret
LBB4_12:
Ltmp30:
	b	LBB4_15
LBB4_13:
Ltmp24:
	mov	x20, x0
	add	x0, sp, #24             ; =24
	bl	__ZNSt3__16localeD1Ev
	b	LBB4_16
LBB4_14:
Ltmp27:
LBB4_15:
	mov	x20, x0
LBB4_16:
	add	x0, sp, #8              ; =8
	bl	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEE6sentryD1Ev
	b	LBB4_18
LBB4_17:
Ltmp17:
	mov	x20, x0
LBB4_18:
	mov	x0, x20
	bl	___cxa_begin_catch
	ldr	x8, [x19]
	ldur	x8, [x8, #-24]
	add	x0, x19, x8
Ltmp31:
	bl	__ZNSt3__18ios_base33__set_badbit_and_consider_rethrowEv
Ltmp32:
; %bb.19:
	bl	___cxa_end_catch
	b	LBB4_11
LBB4_20:
Ltmp33:
	mov	x19, x0
Ltmp34:
	bl	___cxa_end_catch
Ltmp35:
; %bb.21:
	mov	x0, x19
	bl	__Unwind_Resume
	brk	#0x1
LBB4_22:
Ltmp36:
	bl	___clang_call_terminate
	.loh AdrpLdrGot	Lloh38, Lloh39
Lfunc_end1:
	.cfi_endproc
	.section	__TEXT,__gcc_except_tab
	.p2align	2
GCC_except_table4:
Lexception1:
	.byte	255                     ; @LPStart Encoding = omit
	.byte	155                     ; @TType Encoding = indirect pcrel sdata4
	.uleb128 Lttbase0-Lttbaseref0
Lttbaseref0:
	.byte	1                       ; Call site Encoding = uleb128
	.uleb128 Lcst_end1-Lcst_begin1
Lcst_begin1:
	.uleb128 Ltmp15-Lfunc_begin1    ; >> Call Site 1 <<
	.uleb128 Ltmp16-Ltmp15          ;   Call between Ltmp15 and Ltmp16
	.uleb128 Ltmp17-Lfunc_begin1    ;     jumps to Ltmp17
	.byte	1                       ;   On action: 1
	.uleb128 Ltmp18-Lfunc_begin1    ; >> Call Site 2 <<
	.uleb128 Ltmp19-Ltmp18          ;   Call between Ltmp18 and Ltmp19
	.uleb128 Ltmp27-Lfunc_begin1    ;     jumps to Ltmp27
	.byte	1                       ;   On action: 1
	.uleb128 Ltmp20-Lfunc_begin1    ; >> Call Site 3 <<
	.uleb128 Ltmp23-Ltmp20          ;   Call between Ltmp20 and Ltmp23
	.uleb128 Ltmp24-Lfunc_begin1    ;     jumps to Ltmp24
	.byte	1                       ;   On action: 1
	.uleb128 Ltmp25-Lfunc_begin1    ; >> Call Site 4 <<
	.uleb128 Ltmp26-Ltmp25          ;   Call between Ltmp25 and Ltmp26
	.uleb128 Ltmp27-Lfunc_begin1    ;     jumps to Ltmp27
	.byte	1                       ;   On action: 1
	.uleb128 Ltmp28-Lfunc_begin1    ; >> Call Site 5 <<
	.uleb128 Ltmp29-Ltmp28          ;   Call between Ltmp28 and Ltmp29
	.uleb128 Ltmp30-Lfunc_begin1    ;     jumps to Ltmp30
	.byte	1                       ;   On action: 1
	.uleb128 Ltmp29-Lfunc_begin1    ; >> Call Site 6 <<
	.uleb128 Ltmp31-Ltmp29          ;   Call between Ltmp29 and Ltmp31
	.byte	0                       ;     has no landing pad
	.byte	0                       ;   On action: cleanup
	.uleb128 Ltmp31-Lfunc_begin1    ; >> Call Site 7 <<
	.uleb128 Ltmp32-Ltmp31          ;   Call between Ltmp31 and Ltmp32
	.uleb128 Ltmp33-Lfunc_begin1    ;     jumps to Ltmp33
	.byte	0                       ;   On action: cleanup
	.uleb128 Ltmp32-Lfunc_begin1    ; >> Call Site 8 <<
	.uleb128 Ltmp34-Ltmp32          ;   Call between Ltmp32 and Ltmp34
	.byte	0                       ;     has no landing pad
	.byte	0                       ;   On action: cleanup
	.uleb128 Ltmp34-Lfunc_begin1    ; >> Call Site 9 <<
	.uleb128 Ltmp35-Ltmp34          ;   Call between Ltmp34 and Ltmp35
	.uleb128 Ltmp36-Lfunc_begin1    ;     jumps to Ltmp36
	.byte	1                       ;   On action: 1
	.uleb128 Ltmp35-Lfunc_begin1    ; >> Call Site 10 <<
	.uleb128 Lfunc_end1-Ltmp35      ;   Call between Ltmp35 and Lfunc_end1
	.byte	0                       ;     has no landing pad
	.byte	0                       ;   On action: cleanup
Lcst_end1:
	.byte	1                       ; >> Action Record 1 <<
                                        ;   Catch TypeInfo 1
	.byte	0                       ;   No further actions
	.p2align	2
                                        ; >> Catch TypeInfos <<
	.long	0                       ; TypeInfo 1
Lttbase0:
	.p2align	2
                                        ; -- End function
	.section	__TEXT,__text,regular,pure_instructions
	.private_extern	__ZNSt3__116__pad_and_outputIcNS_11char_traitsIcEEEENS_19ostreambuf_iteratorIT_T0_EES6_PKS4_S8_S8_RNS_8ios_baseES4_ ; -- Begin function _ZNSt3__116__pad_and_outputIcNS_11char_traitsIcEEEENS_19ostreambuf_iteratorIT_T0_EES6_PKS4_S8_S8_RNS_8ios_baseES4_
	.globl	__ZNSt3__116__pad_and_outputIcNS_11char_traitsIcEEEENS_19ostreambuf_iteratorIT_T0_EES6_PKS4_S8_S8_RNS_8ios_baseES4_
	.weak_def_can_be_hidden	__ZNSt3__116__pad_and_outputIcNS_11char_traitsIcEEEENS_19ostreambuf_iteratorIT_T0_EES6_PKS4_S8_S8_RNS_8ios_baseES4_
	.p2align	2
__ZNSt3__116__pad_and_outputIcNS_11char_traitsIcEEEENS_19ostreambuf_iteratorIT_T0_EES6_PKS4_S8_S8_RNS_8ios_baseES4_: ; @_ZNSt3__116__pad_and_outputIcNS_11char_traitsIcEEEENS_19ostreambuf_iteratorIT_T0_EES6_PKS4_S8_S8_RNS_8ios_baseES4_
Lfunc_begin2:
	.cfi_startproc
	.cfi_personality 155, ___gxx_personality_v0
	.cfi_lsda 16, Lexception2
; %bb.0:
	sub	sp, sp, #112            ; =112
	stp	x26, x25, [sp, #32]     ; 16-byte Folded Spill
	stp	x24, x23, [sp, #48]     ; 16-byte Folded Spill
	stp	x22, x21, [sp, #64]     ; 16-byte Folded Spill
	stp	x20, x19, [sp, #80]     ; 16-byte Folded Spill
	stp	x29, x30, [sp, #96]     ; 16-byte Folded Spill
	add	x29, sp, #96            ; =96
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
	mov	x19, x0
	cbz	x0, LBB5_15
; %bb.1:
	mov	x24, x5
	mov	x20, x4
	mov	x22, x3
	mov	x21, x2
	sub	x8, x3, x1
	ldr	x9, [x4, #24]
	subs	x8, x9, x8
	csel	x23, x8, xzr, gt
	sub	x25, x2, x1
	cmp	x25, #1                 ; =1
	b.lt	LBB5_3
; %bb.2:
	ldr	x8, [x19]
	ldr	x8, [x8, #96]
	mov	x0, x19
	mov	x2, x25
	blr	x8
	cmp	x0, x25
	b.ne	LBB5_14
LBB5_3:
	cmp	x23, #1                 ; =1
	b.lt	LBB5_11
; %bb.4:
	cmp	x23, #23                ; =23
	b.hs	LBB5_6
; %bb.5:
	add	x26, sp, #8             ; =8
	strb	w23, [sp, #31]
	add	x25, sp, #8             ; =8
	b	LBB5_7
LBB5_6:
	add	x8, x23, #16            ; =16
	and	x26, x8, #0xfffffffffffffff0
	mov	x0, x26
	bl	__Znwm
	mov	x25, x0
	orr	x8, x26, #0x8000000000000000
	stp	x23, x8, [sp, #16]
	str	x0, [sp, #8]
	add	x26, sp, #8             ; =8
LBB5_7:
	mov	x0, x25
	mov	x1, x24
	mov	x2, x23
	bl	_memset
	strb	wzr, [x25, x23]
	ldrsb	w8, [x26, #23]
	ldr	x9, [sp, #8]
	cmp	w8, #0                  ; =0
	add	x8, sp, #8              ; =8
	csel	x1, x9, x8, lt
	ldr	x8, [x19]
	ldr	x8, [x8, #96]
Ltmp37:
	mov	x0, x19
	mov	x2, x23
	blr	x8
Ltmp38:
; %bb.8:
	mov	x24, x0
	ldrsb	w8, [x26, #23]
	tbnz	w8, #31, LBB5_10
; %bb.9:
	cmp	x24, x23
	b.ne	LBB5_14
	b	LBB5_11
LBB5_10:
	ldr	x0, [sp, #8]
	bl	__ZdlPv
	cmp	x24, x23
	b.ne	LBB5_14
LBB5_11:
	sub	x22, x22, x21
	cmp	x22, #1                 ; =1
	b.lt	LBB5_13
; %bb.12:
	ldr	x8, [x19]
	ldr	x8, [x8, #96]
	mov	x0, x19
	mov	x1, x21
	mov	x2, x22
	blr	x8
	cmp	x0, x22
	b.ne	LBB5_14
LBB5_13:
	str	xzr, [x20, #24]
	b	LBB5_15
LBB5_14:
	mov	x19, #0
LBB5_15:
	mov	x0, x19
	ldp	x29, x30, [sp, #96]     ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #80]     ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #64]     ; 16-byte Folded Reload
	ldp	x24, x23, [sp, #48]     ; 16-byte Folded Reload
	ldp	x26, x25, [sp, #32]     ; 16-byte Folded Reload
	add	sp, sp, #112            ; =112
	ret
LBB5_16:
Ltmp39:
	mov	x19, x0
	ldrsb	w8, [x26, #23]
	tbz	w8, #31, LBB5_18
; %bb.17:
	ldr	x0, [sp, #8]
	bl	__ZdlPv
LBB5_18:
	mov	x0, x19
	bl	__Unwind_Resume
	brk	#0x1
Lfunc_end2:
	.cfi_endproc
	.section	__TEXT,__gcc_except_tab
	.p2align	2
GCC_except_table5:
Lexception2:
	.byte	255                     ; @LPStart Encoding = omit
	.byte	255                     ; @TType Encoding = omit
	.byte	1                       ; Call site Encoding = uleb128
	.uleb128 Lcst_end2-Lcst_begin2
Lcst_begin2:
	.uleb128 Lfunc_begin2-Lfunc_begin2 ; >> Call Site 1 <<
	.uleb128 Ltmp37-Lfunc_begin2    ;   Call between Lfunc_begin2 and Ltmp37
	.byte	0                       ;     has no landing pad
	.byte	0                       ;   On action: cleanup
	.uleb128 Ltmp37-Lfunc_begin2    ; >> Call Site 2 <<
	.uleb128 Ltmp38-Ltmp37          ;   Call between Ltmp37 and Ltmp38
	.uleb128 Ltmp39-Lfunc_begin2    ;     jumps to Ltmp39
	.byte	0                       ;   On action: cleanup
	.uleb128 Ltmp38-Lfunc_begin2    ; >> Call Site 3 <<
	.uleb128 Lfunc_end2-Ltmp38      ;   Call between Ltmp38 and Lfunc_end2
	.byte	0                       ;     has no landing pad
	.byte	0                       ;   On action: cleanup
Lcst_end2:
	.p2align	2
                                        ; -- End function
	.section	__TEXT,__text,regular,pure_instructions
	.private_extern	___clang_call_terminate ; -- Begin function __clang_call_terminate
	.globl	___clang_call_terminate
	.weak_def_can_be_hidden	___clang_call_terminate
	.p2align	2
___clang_call_terminate:                ; @__clang_call_terminate
; %bb.0:
	stp	x29, x30, [sp, #-16]!   ; 16-byte Folded Spill
	bl	___cxa_begin_catch
	bl	__ZSt9terminatev
                                        ; -- End function
	.globl	__Z8populateI18splitmix_generatorEvPymy ; -- Begin function _Z8populateI18splitmix_generatorEvPymy
	.weak_def_can_be_hidden	__Z8populateI18splitmix_generatorEvPymy
	.p2align	2
__Z8populateI18splitmix_generatorEvPymy: ; @_Z8populateI18splitmix_generatorEvPymy
	.cfi_startproc
; %bb.0:
	cbz	x1, LBB7_3
; %bb.1:
	mov	x8, #31765
	movk	x8, #32586, lsl #16
	movk	x8, #31161, lsl #32
	movk	x8, #40503, lsl #48
	add	x9, x2, x8
	mov	x10, #58809
	movk	x10, #7396, lsl #16
	movk	x10, #18285, lsl #32
	movk	x10, #48984, lsl #48
	mov	x11, #4587
	movk	x11, #4913, lsl #16
	movk	x11, #18875, lsl #32
	movk	x11, #38096, lsl #48
LBB7_2:                                 ; =>This Inner Loop Header: Depth=1
	eor	x12, x9, x9, lsr #30
	mul	x12, x12, x10
	eor	x12, x12, x12, lsr #27
	mul	x12, x12, x11
	eor	x12, x12, x12, lsr #31
	str	x12, [x0], #8
	add	x9, x9, x8
	subs	x1, x1, #1              ; =1
	b.ne	LBB7_2
LBB7_3:
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	__Z8populateI15wyrng_generatorEvPymy ; -- Begin function _Z8populateI15wyrng_generatorEvPymy
	.weak_def_can_be_hidden	__Z8populateI15wyrng_generatorEvPymy
	.p2align	2
__Z8populateI15wyrng_generatorEvPymy:   ; @_Z8populateI15wyrng_generatorEvPymy
	.cfi_startproc
; %bb.0:
	cbz	x1, LBB8_3
; %bb.1:
	mov	x8, #64533
	movk	x8, #57632, lsl #16
	movk	x8, #58046, lsl #32
	movk	x8, #24766, lsl #48
	add	x9, x2, x8
	mov	x10, #46861
	movk	x10, #19001, lsl #16
	movk	x10, #38197, lsl #32
	movk	x10, #41905, lsl #48
	mov	x11, #54729
	movk	x11, #4858, lsl #16
	movk	x11, #29575, lsl #32
	movk	x11, #6915, lsl #48
LBB8_2:                                 ; =>This Inner Loop Header: Depth=1
	mul	x12, x9, x10
	umulh	x13, x9, x10
	eor	x12, x13, x12
	mul	x13, x12, x11
	umulh	x12, x12, x11
	eor	x12, x12, x13
	str	x12, [x0], #8
	add	x9, x9, x8
	subs	x1, x1, #1              ; =1
	b.ne	LBB8_2
LBB8_3:
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_hole                   ; @hole
.zerofill __DATA,__common,_hole,8,3
	.section	__TEXT,__cstring,cstring_literals
l_.str:                                 ; @.str
	.asciz	" cycles/value ("

l_.str.1:                               ; @.str.1
	.asciz	" %)\t"

l_.str.2:                               ; @.str.2
	.asciz	" branches/value ("

l_.str.3:                               ; @.str.3
	.asciz	" missed_branches/value ("

l_.str.4:                               ; @.str.4
	.asciz	" instructions/value ("

l_.str.5:                               ; @.str.5
	.asciz	" %)"

l_.str.6:                               ; @.str.6
	.asciz	"buffer size "

l_.str.7:                               ; @.str.7
	.asciz	" kB"

l_.str.8:                               ; @.str.8
	.asciz	"splitmix "

l_.str.9:                               ; @.str.9
	.asciz	"wyrng    "

l_.str.10:                              ; @.str.10
	.asciz	"===========\n"

.subsections_via_symbols
