	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.intel_syntax noprefix
	.globl	_test                   ## -- Begin function test
	.p2align	4, 0x90

_test:                                  ## @test
	
	push	rbp
	mov	rbp, rsp
	
	mov	dword ptr [rbp - 4], edi
	mov	edi, dword ptr [rbp - 4]
	
	add	edi, 2
	mov	dword ptr [rbp - 8], edi
	mov	eax, dword ptr [rbp - 8]
	
	pop	rbp
	ret
                                        ## -- End function
	.globl	_test2                  ## -- Begin function test2
	.p2align	4, 0x90

_test2:                                 ## @test2
	
	push	rbp
	mov	rbp, rsp

	# maintain 16 bit alignment
	sub	rsp, 16
	mov	eax, 3
	
	# allocate argc to EDI
	mov	dword ptr [rbp - 4], edi
	
	# allocate argv to ESI
	mov	dword ptr [rbp - 8], esi
	
	# create the new var by pushing this value to it
	mov	esi, dword ptr [rbp - 4]
	
    # add argv to argc	
	add	esi, dword ptr [rbp - 8]

	# the sum of that is in esi which we will now create the new var w/
	mov	dword ptr [rbp - 12], esi
	mov	edi, eax

	# again with the 16 bit
	call	_test
	add	rsp, 16
	
	pop	rbp
	ret
