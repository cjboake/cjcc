	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.intel_syntax noprefix

# Declare test function
	.globl	_test
	.p2align	4, 0x90

#function
_test:                                 

	# standard register setup
	push	rbp
	mov		rbp, rsp

	# set up X from EDI, since it's an argument
	mov		dword ptr [rbp - 4], edi
	mov		edi, dword ptr [rbp - 4]

	# add 2 + x
	add		edi, 2
	
	# create y by throwing edi into a new var position 
	mov		dword ptr [rbp - 8], edi
	
	# move y into a general purpose register (eax, the first one)
	mov		eax, dword ptr [rbp - 8]
	
	#pop rbp back off and return
	pop		rbp
	ret
    
# Declare your main
	.globl	_main                  
	.p2align	4, 0x90

# function
_main:                                

	# standard register setup
	push	rbp
	mov		rbp, rsp

	# put the parameter from main into EDI, where test() can access it
	mov		edi, 3
	call	_test
	
	#pop rbp back off and return
	pop		rbp
	ret
