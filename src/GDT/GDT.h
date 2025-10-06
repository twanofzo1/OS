#pragma once

/*
readme:
This is the Global Descriptor Table (GDT) implementation for the x86 architecture.
It provides a mechanism for defining the characteristics of various memory segments
and is essential for implementing memory protection and multitasking in the operating system.

The GDT is used to define the various segments of memory, including code segments, data segments, and stack segments. 
Each segment has its own set of attributes, such as base address, limit, and access rights.

*/

// TODO implement GDT