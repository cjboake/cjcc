	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	
	pushq	%rbp
	movq	%rsp, %rbp
	
	movl	$0, -4(%rbp)
	movl	%edi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	
	movl	$3, -20(%rbp)
	movl	$1, -24(%rbp)
	movl	-20(%rbp), %edi
	
	addl	-24(%rbp), %edi
	movl	%edi, %eax
	
	popq	%rbp
	retq
