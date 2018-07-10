	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.intel_syntax noprefix
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90

_main:                                  ## @main
	push	rbp
	mov	rbp, rsp
	
	mov	eax, 5
	
	pop	rbp
	ret
                                        ## -- End function
	.section	__DATA,__data
	.globl	_n                      ## @n
	.p2align	2

_n:
	.long	6                       ## 0x6
