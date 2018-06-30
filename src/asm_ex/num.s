	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.intel_syntax noprefix
	.globl	_main                   
	.p2align	4, 0x90

_main:                                  
	
	push	rbp
	mov	rbp, rsp

	// allocate for both vars
	mov	dword ptr [rbp - 4], 1
	mov	dword ptr [rbp - 8], 2

	// move first var to return reg
	mov	eax, dword ptr [rbp - 8]
	// add second var to it
	add	eax, dword ptr [rbp - 12]

	// return
	pop	rbp
	ret
