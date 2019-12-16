#include "key_task.h"
#include "key.h"
#include "gc9306.h"
#include "delay.h"
#include "lcd_task.h"









u8 yemian1_sensor_type = 0;

void key1_short_event(void)
{
	LCD_Clear(BLACK);
	switch (lcd_yemian_flag)
  {
  	case 0:
			lcd_yemian_flag = 1;
			lcd_show_zuobiao(1);
			yemian1_sensor_type = 1;
  		break;
  	case 1:
			lcd_yemian_flag = 2;
			lcd_show_zuobiao(2);
			yemian1_sensor_type = 2;
  		break;
		case 2:
			lcd_yemian_flag = 3;
			lcd_show_zuobiao(3);
			yemian1_sensor_type = 3;
  		break;
		case 3:
			lcd_yemian_flag = 4;
			key_show_klw();//显示颗粒物界面
  		break;
		case 4:
			lcd_yemian_flag = 0;
			lcd_show_main();
  		break;
  	default:
  		break;
  }
}
void key2_short_event(void)
{
	
}
void key3_short_event(void)
{
	LCD_Clear(BLACK);
	switch (lcd_yemian_flag)
  {
  	case 0:
			lcd_yemian_flag = 4;
			key_show_klw();//显示颗粒物界面
  		break;
  	case 1:
			lcd_yemian_flag = 0;
			lcd_show_main();
  		break;
		case 2:
			lcd_yemian_flag = 1;
			lcd_show_zuobiao(1);
			yemian1_sensor_type = 1;
  		break;
		case 3:
			lcd_yemian_flag = 2;
			lcd_show_zuobiao(2);
			yemian1_sensor_type = 2;
  		break;
		case 4:
			lcd_yemian_flag = 3;
			lcd_show_zuobiao(3);
			yemian1_sensor_type = 3;
  		break;
  	default:
  		break;
  }
}
void key_up_short_event(void)
{
	
}
void key_up_long_event(void)
{
	LCD_Fill(74,94,170,50,LIGHTBLUE);
	LCD_ShowString(81,107,24,"  POWER OFF  ",WHITE,LIGHTBLUE);
	delay_ms(1000);
	LCD_Clear1(BLACK);
	VCC_EN = 0;
}





















