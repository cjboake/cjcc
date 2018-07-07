	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.intel_syntax noprefix
	.globl	_function               ## -- Begin function function
	.p2align	4, 0x90
_function:                              ## @function
	push	rbp
	mov	rbp, rsp
	
	sub	rsp, 32
	mov	rax, qword ptr [rip + ___stack_chk_guard@GOTPCREL]
	mov	rax, qword ptr [rax]
	
	mov	qword ptr [rbp - 8], rax
	mov	eax, dword ptr [rbp - 20]
	
	mov	rcx, qword ptr [rip + ___stack_chk_guard@GOTPCREL]
	mov	rcx, qword ptr [rcx]
	
	mov	rdx, qword ptr [rbp - 8]
	cmp	rcx, rdx
	mov	dword ptr [rbp - 24], eax ## 4-byte Spill
	jne	LBB0_2
## BB#1:
	mov	eax, dword ptr [rbp - 24] ## 4-byte Reload
	add	rsp, 32
	pop	rbp
	ret
LBB0_2:
	call	___stack_chk_fail
                                        ## -- End function

.subsections_via_symbols
