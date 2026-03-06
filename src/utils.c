#include "utils.h"
void delay(unsigned long count){
    while (count--) {  asm volatile("nop");}
}
