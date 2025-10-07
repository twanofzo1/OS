global setGdt
global reloadSegments

section .data
gdtr: dw 0          ; limit
      dq 0          ; base

section .text
setGdt:
    mov [gdtr], di
    mov [gdtr+2], rsi
    lgdt [gdtr]
    ret

reloadSegments:
    push 0x08          ; code segment selector
    lea rax, [rel .reload_cs]
    push rax
    retfq              ; far return into 64-bit code segment
.reload_cs:
    mov ax, 0x10       ; data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret
