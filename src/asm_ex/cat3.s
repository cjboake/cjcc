	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.intel_syntax noprefix
	.globl	_main
	.p2align	4, 0x90

_main:                                 
	
	push	rbp
	mov		rbp, rsp

	# Declare the var x = 1
	mov		dword ptr [rbp - 4], 1
	
	# Declare the var y = 2
	mov		dword ptr [rbp - 8], 2

	# Move X to EAX, and add Y to it in EAX
	mov		eax, dword ptr [rbp - 4]
	add		eax, dword ptr [rbp - 8]

	# Declare Z by moving EAX to it's new position
	mov		dword ptr [rbp - 12], eax
    # Throw that back into EAX (not strictly necessary) 	
	mov		eax, dword ptr [rbp - 12]

	# Return your new value
	pop		rbp
	ret
