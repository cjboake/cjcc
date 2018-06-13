	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.intel_syntax noprefix
	.globl	_test                   ## -- Begin function test
	.p2align	4, 0x90
_test:                                  ## @test
## BB#0:
	push	rbp
	mov	rbp, rsp
	mov	dword ptr [rbp - 4], 1
	mov	dword ptr [rbp - 8], 2
	mov	dword ptr [rbp - 12], 3
	mov	dword ptr [rbp - 16], 4
	mov	dword ptr [rbp - 20], 5
	mov	eax, dword ptr [rbp - 16]
	add	eax, dword ptr [rbp - 20]
	pop	rbp
	ret
                                        ## -- End function

.subsections_via_symbols
