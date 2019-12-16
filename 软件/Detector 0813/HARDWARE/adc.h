#ifndef __ADC_H
#define __ADC_H
#include "sys.h" 
#include "delay.h"



extern u16 VOC_data;
extern float VOC_fdata;



void usb_power_init(void);
void Adc_Init(void);
void adc_check_time(void);
void adc_check_task(void);
u16 Get_Adc_Average(u8 ch,u8 times);
void power_check_time(void);
void power_check_task(void);




#endif










