#ifndef __LCD_TASK_H
#define __LCD_TASK_H
#include "stdio.h"	
#include "sys.h" 


extern u8 lcd_yemian_flag;



void lcd_updata_time(void);
void lcd_updata_task(void);
void lcd_show_main(void);
void lcd_show_zuobiao(u8 sensor_type);
void lcd_curve_time(void);
void lcd_curve_task(void);
void key_show_klw(void);





#endif







