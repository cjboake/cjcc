	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.intel_syntax noprefix
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90

_main:                                  ## @main
	
	push	rbp
	mov	rbp, rsp
	
	mov  dword ptr [rbp - 4], 2
	mov  dword ptr [rbp - 8], 1
	
	mov eax, dword ptr [rbp - 4]
	add eax, dword ptr [rbp - 8]	
	
	pop	rbp
	ret
