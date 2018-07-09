	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.intel_syntax noprefix
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90

_main:                                  ## @main
	
	
	push	rbp
	mov	rbp, rsp
	
	lea	rdi, [rip + L_.str]
	call	_printf
	
	pop	rbp
	ret

	L_.str:		.asciz	"hello world\n"
