	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.intel_syntax noprefix
	.globl	_test                   ## -- Begin function test
	.p2align	4, 0x90
_test:                                  ## @test
	
	push	rbp
	mov	rbp, rsp
	
	mov	dword ptr [rbp - 4], edi
	mov	dword ptr [rbp - 8], esi
	
	mov	esi, dword ptr [rbp - 4]
	add	esi, dword ptr [rbp - 8]
	
	mov	eax, esi
	pop	rbp
	ret
                                        ## -- End function
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90

_main:                                  ## @main
	
	push	rbp
	mov	rbp, rsp
	
	mov	dword ptr [rbp - 4], 0
	mov	dword ptr [rbp - 8], edi
	mov	qword ptr [rbp - 16], rsi
	
	mov	dword ptr [rbp - 20], 2
	mov	dword ptr [rbp - 24], 3
	
	mov	edi, dword ptr [rbp - 24]
	mov	esi, dword ptr [rbp - 20]
	
	call	_test
	pop	rbp
	ret
                                        ## -- End function

.subsections_via_symbols
