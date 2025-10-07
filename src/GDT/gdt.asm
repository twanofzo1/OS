
NULL_DESCRIPTOR:
    db 0
    dd 0x00000000
    db 0x00
    db 0x0

KERNEL_CODE_SEGMENT:
    db 0
    dd 0xFFFFF
    db 0x9A
    db 0xA

KERNEL_DATA_SEGMENT:
    db 0
    dd 0xFFFFF
    db 0x92
    db 0xC

USER_CODE_SEGMENT:
    db 0
    dd 0xFFFFF
    db 0xFA
    db 0xA

USER_DATA_SEGMENT:
    db 0
    dd 0xFFFFF
    db 0xF2
    db 0xC