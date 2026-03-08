//D13.2.137
// 用來 設定 EL1 的 exception vector table 位址
// 告訴 CPU 「發生 exception 時要跳去哪裡」
.globl irq_init_vectors
irq_init_vectors:
    adr x0, vectors
    msr vbar_el1, x0
    ret

//C5.1.3  Instructions for accessing the PSTATE fields
// 11:8 Imm4   MSR DAIFClr #Imm4 
//op2 == 0b110 Selects the MSR DAIFSet instruction, 
  // that sets the specified PSTATE.{D, A, I, F} bits to 1.
//op2 == 0b111 Selects the MSR DAIFClr instruction, 
  // that clears the specified PSTATE.{D, A, I, F} bits to 0
//C5.2.2 daif interrupt mask bit , D(debug)aif
// MSR DAIFClr, #<imm> op2 = 0b111
// daif clr 設成 2  代表  clear i mask = enable interrupt 
.globl irq_enable
irq_enable:
    msr daifclr, #2
    ret

.globl irq_disable
irq_disable:
    msr daifset, #2
    ret
