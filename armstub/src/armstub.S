.globl _start
_start:
    // load kernel_entry and branch to there
    ldr w0, kernel_entry32
    br x0

//fill some magic number so its a valid arm stub

.ltorg

.org 0xf0
.globl stub_magic
stub_magic:
    .word 0x5afe570b

.org 0xf4
.globl stub_version
stub_version:
    .word 0

//will start from very top of the binary file 

.org 0xfc
.globl kernel_entry32
kernel_entry32:
    .word 0x0