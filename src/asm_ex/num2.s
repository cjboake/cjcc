	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.intel_syntax noprefix
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90

_main:                                  ## @main
	
	push	rbp
	mov	rbp, rsp
	
	xor	eax, eax
	
	mov	dword ptr [rbp - 4], 2
	mov	ecx, dword ptr [rbp - 4]
	
	add	ecx, 1
	mov	dword ptr [rbp - 4], ecx
	
	pop	rbp
	ret
                                        ## -- End function

.subsections_via_symbols
