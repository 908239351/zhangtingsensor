#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 


extern u16 PM2_5_data;
extern u16 PM1_0_data;
extern u16 PM10_data;

extern u16 klw_03_data;
extern u16 klw_05_data;
extern u16 klw_10_data;
extern u16 klw_25_data;
extern u16 klw_50_data;
extern u16 klw_100_data;




void USART1_Init(u32 bound);
void PMinterface_receive_check(void);





#endif


