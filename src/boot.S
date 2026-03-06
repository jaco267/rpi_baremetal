#include "mm.h"
#include "sysregs.h"

.section .text.startup
.global _start
.extern main

_start:
    mrs x0, mpidr_el1
    and x0, x0, #0xFF
    cbz x0, master
    b hang

master:
    ldr x0, =SCTLR_VALUE_MMU_DISABLED
    msr sctlr_el1, x0

    ldr x0, =HCR_VALUE
    msr hcr_el2, x0

    ldr x0, =SCR_VALUE
    msr scr_el3, x0

    ldr x0, =SPSR_VALUE
    msr spsr_el3, x0

    adr x0, el1_entry
    msr elr_el3, x0

    eret

el1_entry:
    adr x0, bss_begin
    adr x1, bss_end
    sub x1, x1, x0
    bl memzero

    // 設定 stack pointer sp=low_memory
    ldr x0, =0x400000
    mov sp, x0

    // 呼叫 C main
    bl main

hang:
    wfe
    b hang
    