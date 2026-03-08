#pragma once 
#include "base.h"
#include "common.h"
//* done bcm2835 2.1 aux: overview
struct AuxRegs {//7E215000(AUX_BASE)+
    reg32 irq_status; //7E21 5000
    reg32 enables;    //7E21 5004  (4byte = 32 bit)  AUX_ENABLES=AUX_BASE+0x04
    reg32 reserved[14]; //7E21 5008 ~ 7E21 5040  0x40-08=16*4-8=56 bytes 56/4=14   
    reg32 mu_io;   //7E215040 = AUX_BASE+0x40  (IO_REG)
    reg32 mu_ier;  //0x44     AUX_MU_IER_REG
    reg32 mu_iir;  //0x48
    reg32 mu_lcr;  //0x4C      AUX_MU_LCR_REG
    reg32 mu_mcr;  //0x50
    reg32 mu_lsr;  //0x54      AUX_MU_LSR_REG
    reg32 mu_msr;  //0x58
    reg32 mu_scratch; //0x5C   
    reg32 mu_control; //0x60   AUX_MU_CNTL_REG
    reg32 mu_status;  //0x64
    reg32 mu_baud_rate;//0x68  AUX_MU_BAUD_REG
  };
  
// 剩下的是 SPI controller

//0x 7E215000 = 0x7E000000+0x00215000
#define REGS_AUX ((struct AuxRegs *)(MMIO_BASE + 0x00215000))