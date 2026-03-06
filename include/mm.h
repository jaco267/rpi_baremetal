#pragma once


#define PAGE_SHIFT 12  
#define TABLE_SHIFT 9 
#define SECTION_SHIFT (PAGE_SHIFT + TABLE_SHIFT) //12+9 = 21
#define PAGE_SIZE (1 << PAGE_SHIFT)              //2^12 = 4096
#define SECTION_SIZE (1 << SECTION_SHIFT)       //2^21

#define LOW_MEMORY (2 * SECTION_SIZE)  // 2*2^21 = 2^22 = 4194304=0x 40_0000

#ifndef __ASSEMBLER__

void memzero(unsigned long src, unsigned int n);

#endif

/*
24       16    8  0
       4  0   00 00
     100   
     22 20

          16  8 4
0x        8  00 00 
        1000                    2^19=524288    free memory = 4194304-524288

        100 0000  40
            1000   8
*/

/*
PAGE_SHIFT = 12 → 頁大小 4 KB (1 << 12)
TABLE_SHIFT = 9 → 每層頁表 512 entry
SECTION_SHIFT = PAGE_SHIFT + TABLE_SHIFT = 21 → 每個 section 大小 = 1 << 21 = 2 MB

LOW_MEMORY = 2 * SECTION_SIZE = 4 MB

stack pointer 被設到 0x400000 (4 MB)。


高位址            
0x40_0000        stack 往下長
                     |
                     v
---------------------------------------

code memory      Unused code memory (unused sram) 
(flash)          
---------------------------------------
                   heap
--------------------------------------
                 .bss (un initialized vaar)
                 .data (initialized global and static var)
                 .rodata  
                 .text  
0x0             vector table      
低位址         
*/