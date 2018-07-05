	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.intel_syntax noprefix
	.globl	_function               ## -- Begin function function
	.p2align	4, 0x90

_function:                              ## @function
	
	push	rbp
	mov	rbp, rsp
	
	mov	dword ptr [rbp - 4], edi
	mov	dword ptr [rbp - 8], esi
	mov	dword ptr [rbp - 12], edx
	
	mov	edx, dword ptr [rbp - 4]
	add	edx, dword ptr [rbp - 8]
	
	mov	eax, edx
	
	pop	rbp
	ret
                                        ## -- End function
_caller:                                ## @caller
	
	push	rbp
	mov	rbp, rsp
	
	mov	edi, 1
	mov	esi, 2
	mov	edx, 3
	
	call	_function
	
	pop	rbp
	ret
