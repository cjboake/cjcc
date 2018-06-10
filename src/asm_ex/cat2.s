	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.intel_syntax noprefix
	.globl	_main                   ## -- Begin function test
	.p2align	4, 0x90

_main:                       
	push	rbp
	
	mov		rbp, rsp
	mov		dword ptr [rbp - 4], 3
	mov		eax, dword ptr [rbp - 4]
	
	pop		rbp
	ret
