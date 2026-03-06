#pragma once

//D13.2.113 (page 3396)   M, bit [0] 在 page 3409 (MMU enable) //* done
//設定 MMU disable, little_endian 等等
#define SCTLR_RESERVED                  (3 << 28) | (3 << 22) | (1 << 20) | (1 << 11)
#define SCTLR_EE_LITTLE_ENDIAN          (0 << 25)
#define SCTLR_EOE_LITTLE_ENDIAN         (0 << 24)
#define SCTLR_I_CACHE_DISABLED          (0 << 12)
#define SCTLR_D_CACHE_DISABLED          (0 << 2)
#define SCTLR_MMU_DISABLED              (0 << 0)
#define SCTLR_MMU_ENABLED               (1 << 0)

#define SCTLR_VALUE_MMU_DISABLED (SCTLR_RESERVED | SCTLR_EE_LITTLE_ENDIAN | SCTLR_I_CACHE_DISABLED | SCTLR_D_CACHE_DISABLED | SCTLR_MMU_DISABLED)

//D13.2.47  to set to EL1  (exception level)
//HCR_RW=1   讓 EL1 用 AArch64 (64-bit) 執行
#define HCR_RW                          (1 << 31)
#define HCR_VALUE                       HCR_RW

//D13.2.112  EL3 assignment   el0 el1 will be in secure state (so el0, el1 cant access secure memory)
//RW=1: 下一層 EL 用 aarch64
//NS=1:  Exception levels lower than EL3 are in Non-secure state, and so memory accesses from those Exception levels cannot access Secure memory.
#define SCR_RESERVED                    (3 << 4)
#define SCR_RW                          (1 << 10)
#define SCR_NS                          (1 << 0)
#define SCR_VALUE                       (SCR_RESERVED | SCR_RW | SCR_NS)

//C5.2.19
//   6 54   3210 
//0111 00   0101
#define SPSR_MASK_ALL                   (7 << 6)
#define SPSR_EL1h                       (5 << 0)
#define SPSR_EL2h                       (9 << 0)
#define SPSR_VALUE                      (SPSR_MASK_ALL | SPSR_EL1h)

/*
在 EL1 有兩種 stack pointer：
EL1t	use SP_EL0
EL1h	use SP_EL1
*/