	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.intel_syntax noprefix
	.globl	_function               
	.p2align	4, 0x90

_function:                              
	push	rbp
	mov	rbp, rsp
	
	mov	dword ptr [rbp - 4], edi
	mov	dword ptr [rbp - 8], esi
	mov	dword ptr [rbp - 12], edx
	mov	dword ptr [rbp - 16], ecx
	
	mov	ecx, dword ptr [rbp - 4]
	add	ecx, dword ptr [rbp - 8]
	
	mov	eax, ecx
	
	pop	rbp
	ret
                                       
	.globl	_caller                 
	.p2align	4, 0x90

_caller:                                
	
	push	rbp
	mov	rbp, rsp
	
	mov	edi, 1
	mov	esi, 2
	mov	edx, 3
	mov	ecx, 4
	
	call	_function
	
	pop	rbp
	ret
