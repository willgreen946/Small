global _start

section .rodata

KERNEL: equ 0x80

; System call numbers
SYS_WRITE: equ 4
SYS_READ: equ 3
SYS_EXIT: equ 1

; File descriptors
STDOUT_FILENO: equ 1
STDIN_FILENO: equ 0

section .data

; The prompt that is printed
prompt: db '>'
prompt_len: equ $-prompt

instring: db 256

section .text

_io_loop:
																		; Print prompt to stdout
	mov eax, SYS_WRITE								; Load write syscall
	push dword prompt_len							; Load output length
	push dword prompt									; Load string pointer
	push dword STDOUT_FILENO					; Write to stdout
	push eax
	int KERNEL												; Interupt
	add esp, 16												; Move back the stack pointer

																		; Read from stdin
	mov eax, SYS_READ									; Load read syscall
	push dword 255										; Length in bytes to read
	push dword instring								; Load variable pointer
	push dword STDIN_FILENO						; stdin
	push eax
	int KERNEL												; Interupt kernel
	add esp, 16												; Move back the stack pointer

	jnz _io_loop											; Loop back to the top

_start:
	call _io_loop
																		; Exiting program
	mov eax, SYS_EXIT 								; Exit syscall
	push dword 0											; Return status
	push eax
	int KERNEL												; Interupt