#pragma once

/*
readme:
https://wiki.osdev.org/Global_Descriptor_Table
This is the Global Descriptor Table (GDT) implementation for the x86 architecture.
It provides a mechanism for defining the characteristics of various memory segments
and is essential for implementing memory protection and multitasking in the operating system.

The GDT is used to define the various segments of memory, including code segments, data segments, and stack segments. 
Each segment has its own set of attributes, such as base address, limit, and access rights.

Access Byte:
bit :       7	6	5	4	3	2	1	0
function:   P	DPL	    S	E	DC	RW	A

P: Present (1 = segment is present in memory, 0 = segment is not present)
DPL: 0 = highest privilege (kernel), 3 = lowest privilege (user applications).
S: Descriptor Type (0 = system, 1 = code or data) 
E: Executable (1 = code segment, 0 = data segment) if the code is executable
DC: Direction/Conforming 
    0 = cant be executed from a lesser privilege level, 
    1 = can be executed from a lesser privilege level 
RW:
    read/write
    if code segment: 0 = read-only, 1 = read and execute
    if data segment: 0 = read-only, 1 = read and write

A = Accessed (set by CPU when segment is accessed) default 0


access byte for each segment:
segment 0 null segment:        
    P   : 0         not present
    DPL : 00 (0)    highest privilege level (kernel)
    S   : 0         system segment         
    E   : 0         not executable (data segment)
    DC  : 0         can be executed from a lesser privilege level
    RW  : 0         read only
    A   : 0         not accessed
    binary: 0b00000000
    hex: 0x00

segment 1 kernel code segment:
    P   : 1         is present
    DPL : 00 (0)    highest privilege level (kernel)
    S   : 1         (code/data) -> code segment
    E   : 1         executable (code segment)
    DC  : 1         can be executed from a lesser privilege level  
    RW  : 1         read and execute
    A   : 0         not accessed
    binary: 0b10011110
    hex: 0x9E

segment 2 kernel data segment: 
    P   : 1         is present
    DPL : 00 (0)    highest privilege level (kernel)
    S   : 1         (code/data) -> data segment
    E   : 0         not executable (data segment)
    DC  : 0         cant be executed from a lesser privilege level (not code segment)
    RW  : 1         read/write
    A   : 0         not accessed
    binary: 0b10010010
    hex: 0x92

segment 3 user code segment:   
    P   : 1         is present
    DPL : 11 (3)    user privilege level 
    S   : 1         (code/data) -> code segment
    E   : 1         executable (code segment)
    DC  : 1         can be executed from a lesser privilege level (doesnt matter for user code segment)
    RW  : 1         read and execute
    A   : 0         not accessed
    binary: 0b11111110
    hex: 0xFE

segment 4 user data segment:
    P   : 1         is present
    DPL : 11 (3)    user privilege level 
    S   : 1         (code/data) -> data segment
    E   : 0         not executable (data segment)
    DC  : 0         cant be executed from a lesser privilege level (not code segment)
    RW  : 1         read and write
    A   : 0         not accessed
    binary: 0b11110010
    hex: 0xF2

segment 5 TSS segment:
    P   : 1         is present
    DPL : 00 (0)    highest privilege level (kernel)
    S   : 0         system segment         
    E   : 1         executable 
    DC  : 0         can be executed from a lesser privilege level
    RW  : 0         read only
    A   : 1         accessed
    binary: 0b10001001
    hex: 0x89
*/
//https://wiki.osdev.org/GDT_Tutorial




// TODO implement GDT


#include <stdint.h>

