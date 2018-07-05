	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.intel_syntax noprefix
	.globl	_main                   
	.p2align	4, 0x90

_main:                                 
	
	# the normal stuff
	push	rbp
	mov		rbp, rsp

	# assign the base var (figure this out)
	mov	dword ptr [rbp - 4], 0
	
	#  
	mov	dword ptr [rbp - 8], edi   # destination index, used for function params and data copy
	mov	qword ptr [rbp - 16], rsi  # register source index?

	# assign the vars their values 
	mov	dword ptr [rbp - 20], 1
	mov	dword ptr [rbp - 24], 2

	# move the vars int edi
	# and perform the addition 
	mov	edi, dword ptr [rbp - 20]
	add	edi, dword ptr [rbp - 24]

	# move edi into eax to 
	# be returned
	mov	eax, edi

	#return
	pop	rbp
	ret
