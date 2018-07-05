	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.intel_syntax noprefix
	.globl	_function               ## -- Begin function function
	.p2align	4, 0x90

_function:                              ## @function
	
	# the normal stuff
	push	rbp
	mov		rbp, rsp

	# maybe allocating for the parameters? 
	mov	dword ptr [rbp - 4], edi
	mov	dword ptr [rbp - 8], esi
	
	# assign the vars their values 
	mov	dword ptr [rbp - 12], 1
	mov	dword ptr [rbp - 16], 2
	
	# move the vars int edi
	# and perform the addition 
	mov	esi, dword ptr [rbp - 12]
	add	esi, dword ptr [rbp - 16]
	
	# move edi into eax to 
	# be returned
	mov	eax, esi
	
	#return
	pop	rbp
	ret
