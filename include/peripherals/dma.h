#pragma once

#include "common.h"

#include "base.h"
//* p40 4.2.1.1 control block data structure
typedef struct {
    u32 transfer_info;
    u32 src_addr;
    u32 dest_addr;
    u32 transfer_length;
    u32 mode_2d_stride;
    u32 next_block_addr;
    u32 res[2];   
} dma_control_block;  //* 8*32 = 256 bits

//* p41  DMA Address map 
/*
0x0  0_CS (control) 
0x4  0_CONBLK_ADDR  
0x8  0_TI 
0xc  0_SRC_ADDR
0x10 0_DEST_ADDR
0x14 0_TXFR_LEN
0x18 0_STRIDE
0x1C nextCONBK
0x20 0_DEBUG  
....
0x100 1_CS   for dma channel 1  
*/
typedef struct {
    u32 control;                   //* CS
    u32 control_block_addr;        //* CONBLK_AD
    dma_control_block block;  //? 好像漏了 debug ??   
} dma_channel_regs;
//* 一個 channel memory 位址 隔 0x100  
// channel 1 0x000, channel 2 0x100 ... 
// memory 單位是 byte 所以  一個 channel 256 **bytes**
//todo carefull I've add uintptr_t
#define REGS_DMA(channel) ((dma_channel_regs *)(uintptr_t)(PBASE + 0x00007000 + (channel * 0x100)))
//* channel 1  

//* BCM2835 ARM p47  0x7000+ 0xFE0  INT_STATUS : interrupt status of each DMA channel
#define REGS_DMA_INT_STATUS *((volatile u32 *)(PBASE + 0x00007FE0))
//* 0xFF0  global enable bits for each DMA channel 
#define REGS_DMA_ENABLE *((volatile u32 *)(PBASE + 0x00007FF0))

//defines for differnet bits of the control and transfer info
//* CS (control and status ) p47
#define CS_RESET			(1 << 31)
#define CS_ABORT			(1 << 30)
#define CS_WAIT_FOR_OUTSTANDING_WRITES	(1 << 28)
#define CS_PANIC_PRIORITY_SHIFT		20
    #define DEFAULT_PANIC_PRIORITY		15
#define CS_PRIORITY_SHIFT		16
    #define DEFAULT_PRIORITY		1
#define CS_ERROR			(1 << 8)
#define CS_INT				(1 << 2)
#define CS_END				(1 << 1)
#define CS_ACTIVE			(1 << 0)

//* DMA Transfer Information p50  
#define TI_PERMAP_SHIFT			16
#define TI_BURST_LENGTH_SHIFT		12
#define DEFAULT_BURST_LENGTH		0
#define TI_SRC_IGNORE			(1 << 11)
#define TI_SRC_DREQ			(1 << 10)
#define TI_SRC_WIDTH			(1 << 9)
#define TI_SRC_INC			(1 << 8)
#define TI_DEST_DREQ			(1 << 6)
#define TI_DEST_WIDTH			(1 << 5)
#define TI_DEST_INC			(1 << 4)
#define TI_WAIT_RESP			(1 << 3)
#define TI_TDMODE			(1 << 1)
#define TI_INTEN			(1 << 0)
