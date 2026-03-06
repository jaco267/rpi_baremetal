#pragma once
//https://youtu.be/nUW1FB_5vqo?list=PLVxiWMqQvhg9FCteL7I0aohj1_YiUx1x8&t=796
#include "base.h"
#include "common.h"

//6.2.4 in bcm2711 arm  video core interrupts  
/*https://youtu.be/2dlBZoLCMSc?list=PLVxiWMqQvhg9FCteL7I0aohj1_YiUx1x8&t=148
29: aux  
0~3: timer0~3
*/
enum vc_irqs {
    SYS_TIMER_IRQ_0 = 1,
    SYS_TIMER_IRQ_1 = 2,
    SYS_TIMER_IRQ_2 = 4,
    SYS_TIMER_IRQ_3 = 8,
    AUX_IRQ = (1 << 29)
};

struct arm_irq_regs_2837 {
    reg32 irq0_pending_0;
    reg32 irq0_pending_1;
    reg32 irq0_pending_2;
    reg32 fiq_control;
    reg32 irq0_enable_1;
    reg32 irq0_enable_2;
    reg32 irq0_enable_0;
    reg32 res;
    reg32 irq0_disable_1;
    reg32 irq0_disable_2;
    reg32 irq0_disable_0;
};


typedef struct arm_irq_regs_2837 arm_irq_regs;

#define REGS_IRQ ((arm_irq_regs *)(MMIO_BASE + 0x0000B200))
