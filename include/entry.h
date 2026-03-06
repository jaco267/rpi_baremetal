#pragma once
//* done
//D1.10.2
#define SYNC_INVALID_EL1t		0 
#define IRQ_INVALID_EL1t		1 
#define FIQ_INVALID_EL1t		2 
#define ERROR_INVALID_EL1t		3 

#define SYNC_INVALID_EL1h		4 
#define IRQ_INVALID_EL1h		5 
#define FIQ_INVALID_EL1h		6 
#define ERROR_INVALID_EL1h		7 

#define SYNC_INVALID_EL0_64	    8 
#define IRQ_INVALID_EL0_64	    9 
#define FIQ_INVALID_EL0_64		10 
#define ERROR_INVALID_EL0_64	11 

#define SYNC_INVALID_EL0_32		12 
#define IRQ_INVALID_EL0_32		13 
#define FIQ_INVALID_EL0_32		14 
#define ERROR_INVALID_EL0_32	15 

//stack frame size
#define S_FRAME_SIZE			256

/*

entry.S : .align	11, vector table 總大小 = 2048 bytes, 16 × 128 = 2048

在 EL1 有兩種 stack pointer：
EL1t	use SP_EL0
EL1h	use SP_EL1

0x000   
0x080=0000 1000 0000    = 128 bytes = 2^7 bytes
0x100=0001 0000 0000                = 2*128 bytes
0x180=0001 1000 0000                = 3*128 bytes
*/