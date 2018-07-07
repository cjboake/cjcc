.section    __TEXT,__text,regular,pure_instructions
.macosx_version_min 10, 13
.intel_syntax noprefix
.globl  _main
.p2align    4, 0x90

_main:
	push  rbp
	mov   rbp, rsp
	mov dword ptr [rbp - 4], edi
	mov dword ptr [rbp - 8], esi
	mov  dword ptr [rbp - 12], 2
	mov  dword ptr [rbp - 16], 1
	mov eax, dword ptr [rbp - 12]
	add eax, dword ptr [rbp - 16]
	pop rbp
	ret
