	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.intel_syntax noprefix
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
## BB#0:
	
	push	rbp
	mov		rbp, rsp
	
	lea		rax, [rbp - 4]
	mov		dword ptr [rbp - 4], 1
	mov		qword ptr [rbp - 16], rax
	mov		rax, qword ptr [rbp - 16]
	
	pop		rbp
	ret
                                        ## -- End function

.subsections_via_symbols
