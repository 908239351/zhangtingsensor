#ifndef __UART2_H
#define __UART2_H	 
#include "sys.h"  


extern u16 hcho_jizhun;
extern u16 HCHO_data;
extern float hcho_fdata;

void USART2_Init(u32 bound);
void interface_receive_check(void);




#endif













