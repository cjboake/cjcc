	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.intel_syntax noprefix
	.globl	_function               ## -- Begin function function
	.p2align	4, 0x90

_function:                              ## @function
	push	rbp
	mov	rbp, rsp
	mov	dword ptr [rbp - 4], 0
	mov	dword ptr [rbp - 8], 1
	pop	rbp
	ret
                                        ## -- End function

.subsections_via_symbols
