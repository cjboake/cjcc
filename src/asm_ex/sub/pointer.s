	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.intel_syntax noprefix
	.globl	_function               ## -- Begin function function
	.p2align	4, 0x90
_function:                              ## @function
## BB#0:
	push	rbp
	mov	rbp, rsp
	lea	rax, [rbp - 8]
	mov	dword ptr [rbp - 8], 1
	mov	qword ptr [rbp - 16], rax
	mov	eax, dword ptr [rbp - 4]
	pop	rbp
	ret
                                        ## -- End function

.subsections_via_symbols
