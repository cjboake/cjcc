	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.intel_syntax noprefix
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90

_main:                                  ## @main
	push	rbp
	mov	rbp, rsp
	
	mov	dword ptr [rbp - 4], 0
	mov	dword ptr [rbp - 8], edi
	mov	qword ptr [rbp - 16], rsi
	mov	dword ptr [rbp - 20], 3
	
	cmp	dword ptr [rbp - 20], 1
	jne	LBB0_2
	mov	dword ptr [rbp - 20], 5

LBB0_2:
	mov	eax, dword ptr [rbp - 20]
	
	pop	rbp
	ret
