#define PAGE_SHIFT 12  
#define TABLE_SHIFT 9 
#define SECTION_SHIFT (PAGE_SHIFT + TABLE_SHIFT)  //12+9 = 21
#define PAGE_SIZE (1 << PAGE_SHIFT)               //2^12 = 4096
#define SECTION_SIZE (1 << SECTION_SHIFT)  //2^21

#define LOW_MEMORY (2 * SECTION_SIZE)  // 2*2^21 = 2^22 = 4194304
#include "stdio.h"

int main(){
    printf("low memory %d\n", LOW_MEMORY);  //2^21
    return 0;
}