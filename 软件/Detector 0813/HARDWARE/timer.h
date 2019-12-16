#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"


extern u16 CO2_data;



void TIM3_Int_Init(u16 arr,u16 psc);
void TIM1_Cap_Init(u16 arr,u16 psc);
void tim1_pwm_co2_task(void);





#endif
