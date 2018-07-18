	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.intel_syntax noprefix
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
## BB#0:
	push	rbp
	mov	rbp, rsp
	mov	dword ptr [rbp - 4], 0
	mov	dword ptr [rbp - 8], edi
	mov	qword ptr [rbp - 16], rsi
	mov	dword ptr [rbp - 20], 3
	mov	dword ptr [rbp - 24], 1
	mov	edi, dword ptr [rbp - 20]
	add	edi, dword ptr [rbp - 24]
	mov	eax, edi
	pop	rbp
	ret
                                        ## -- End function

.subsections_via_symbols
