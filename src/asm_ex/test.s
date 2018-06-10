	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.intel_syntax noprefix
	.globl	_main                
	.p2align	4, 0x90

_main:                              
	
	push 	rbp
	mov		rax, 1
	mov		rbx, 2
	add		rax, rbx
	pop		rbp
	ret
