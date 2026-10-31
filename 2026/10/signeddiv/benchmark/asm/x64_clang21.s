compiler_div7:
	movsxd	rax, edi
	imul	rcx, rax, -1840700269
	shr	rcx, 32
	add	eax, ecx
	mov	ecx, eax
	shr	ecx, 31
	sar	eax, 2
	add	eax, ecx
	ret
div32_7:
	movsxd	rax, edi
	mov	ecx, 2454267027
	imul	rcx, rax
	sar	rcx, 34
	shr	eax, 31
	add	eax, ecx
	ret
compiler_div1000:
	movsxd	rax, edi
	imul	rax, rax, 274877907
	mov	rcx, rax
	shr	rcx, 63
	sar	rax, 38
	add	eax, ecx
	ret
div32_1000:
	movsxd	rax, edi
	mov	ecx, 2199023256
	imul	rcx, rax
	sar	rcx, 41
	shr	eax, 31
	add	eax, ecx
	ret
