_compiler_div7:
	mov	w8, #9363
	movk	w8, #37449, lsl #16
	smull	x8, w0, w8
	lsr	x8, x8, #32
	add	w8, w8, w0
	asr	w9, w8, #2
	add	w0, w9, w8, lsr #31
	ret
_div32_7:
	sxtw	x8, w0
	mov	w9, #9363
	movk	w9, #37449, lsl #16
	mul	x8, x8, x9
	asr	x8, x8, #34
	add	w0, w8, w0, lsr #31
	ret
_compiler_div1000:
	mov	w8, #19923
	movk	w8, #4194, lsl #16
	smull	x8, w0, w8
	lsr	x9, x8, #63
	asr	x8, x8, #38
	add	w0, w8, w9
	ret
_div32_1000:
	sxtw	x8, w0
	mov	w9, #28312
	movk	w9, #33554, lsl #16
	mul	x8, x8, x9
	asr	x8, x8, #41
	add	w0, w8, w0, lsr #31
	ret
