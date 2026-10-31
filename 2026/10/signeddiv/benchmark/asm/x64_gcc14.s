compiler_div7:
	movsx	rax, edi
	imul	rax, rax, -1840700269
	shr	rax, 32
	add	eax, edi
	sar	edi, 31
	sar	eax, 2
	sub	eax, edi
	ret
div32_7:
	movsx	rax, edi
	mov	edx, 2454267027
	shr	edi, 31
	imul	rax, rdx
	sar	rax, 34
	add	eax, edi
	ret
compiler_div1000:
	movsx	rax, edi
	sar	edi, 31
	imul	rax, rax, 274877907
	sar	rax, 38
	sub	eax, edi
	ret
div32_1000:
	movsx	rax, edi
	mov	edx, 2199023256
	shr	edi, 31
	imul	rax, rdx
	sar	rax, 41
	add	eax, edi
	ret
