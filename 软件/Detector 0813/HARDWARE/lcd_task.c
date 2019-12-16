#include "lcd_task.h"
#include "usart.h"
#include "lcd.h"
#include "timer.h"
#include "uart2.h"
#include "SHT2x.H"
#include "adc.h"
#include "w25qxx.h"


extern const u8 wendu_ico[];
extern const u8 shidu_ico[];

u32 lcdcolor24_table[5]={0x00FF00,0xEEEE00,0xEE9A00,0xEE0000,0xEE00EE};


u16 pm25_back_color = GREEN;
u16 co2_back_color = GREEN;
u16 hcho_back_color = GREEN;
u16 voc_back_color = GREEN;

u16 last_pm25_back_color = GREEN;
u16 last_co2_back_color = GREEN;
u16 last_hcho_back_color = GREEN;
u16 last_voc_back_color = GREEN;
/*******************************************************************************
* Function Name  : C24to16
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u16 C24to16(unsigned long dw24bitColor)
{
	unsigned int  r,g,b;
	r=(dw24bitColor&0xff0000)>>19;
	g=(dw24bitColor&0xff00)>>10;
	b=(dw24bitColor&0xff)>>3;

	return ((r<<11)|(g<<5)|b)&0XFFFF;
}
void lcd_updata_pm25bmp(void)
{
	LCD_DrawFullCircle(70,119,70,pm25_back_color);
	LCD_ShowString(40,61,24,"PM2.5",BLACK,pm25_back_color);
	LCD_ShowString(40,150,24,"ug/·½",BLACK,pm25_back_color);
}
void lcd_updata_co2bmp(void)
{
	LCD_DrawFullCircle(180,45,45,co2_back_color);
	LCD_ShowString(168,8,16,"CO2",BLACK,co2_back_color);
	LCD_ShowString(168,66,16,"ppm",BLACK,co2_back_color);
}
void lcd_updata_hchobmp(void)
{
	LCD_DrawFullCircle(255,119,50,hcho_back_color);
	LCD_ShowString(239,82,16,"¼×È©",BLACK,hcho_back_color);
	LCD_ShowString(235,140,16,"mg/·½",BLACK,hcho_back_color);
}
void lcd_updata_vocbmp(void)
{
	LCD_DrawFullCircle(180,194,45,voc_back_color);
	LCD_ShowString(164,157,16,"TVOC",BLACK,voc_back_color);
	LCD_ShowString(160,215,16,"mg/·½",BLACK,voc_back_color);
}
void lcd_show_main(void)
{
	lcd_updata_pm25bmp();
	lcd_updata_co2bmp();
	lcd_updata_hchobmp();
	lcd_updata_vocbmp();
	
	Draw_pic1(10,197,24,5, wendu_ico, BLACK,WHITE);
	Draw_pic1(250,197,24,5, shidu_ico, BLACK,WHITE);
}
void lcd_get_pm25_back_color(void)
{
	if(PM2_5_data < 35)
		pm25_back_color = C24to16(lcdcolor24_table[0]);
	else if((PM2_5_data >= 35) && (PM2_5_data < 75))
		pm25_back_color = C24to16(lcdcolor24_table[1]);
	else if((PM2_5_data >= 75) && (PM2_5_data < 115))
		pm25_back_color = C24to16(lcdcolor24_table[2]);
	else if((PM2_5_data >= 115) && (PM2_5_data < 150))
		pm25_back_color = C24to16(lcdcolor24_table[3]);
	else if(PM2_5_data >= 150)
		pm25_back_color = C24to16(lcdcolor24_table[4]);
}
void lcd_get_co2_back_color(void)
{
	if(CO2_data < 800)
		co2_back_color = C24to16(lcdcolor24_table[0]);
	else if((CO2_data >= 800) && (CO2_data < 1500))
		co2_back_color = C24to16(lcdcolor24_table[1]);
	else if((CO2_data >= 1500) && (CO2_data < 2500))
		co2_back_color = C24to16(lcdcolor24_table[3]);
	else if(CO2_data >= 2500)
		co2_back_color = C24to16(lcdcolor24_table[4]);
}
void lcd_get_hcho_back_color(void)
{
	if(hcho_fdata < 0.02)
		hcho_back_color = C24to16(lcdcolor24_table[0]);
	else if((hcho_fdata >= 0.02) && (hcho_fdata < 0.05))
		hcho_back_color = C24to16(lcdcolor24_table[1]);
	else if((hcho_fdata >= 0.05) && (hcho_fdata < 0.08))
		hcho_back_color = C24to16(lcdcolor24_table[2]);
	else if((hcho_fdata >= 0.08) && (hcho_fdata < 0.15))
		hcho_back_color = C24to16(lcdcolor24_table[3]);
	else if(hcho_fdata >= 0.15)
		hcho_back_color = C24to16(lcdcolor24_table[4]);
}
void lcd_get_voc_back_color(void)
{
	if(VOC_fdata < 0.6)
		voc_back_color = C24to16(lcdcolor24_table[0]);
	else if((VOC_fdata >= 0.6) && (VOC_fdata < 2.0))
		voc_back_color = C24to16(lcdcolor24_table[1]);
	else if((VOC_fdata >= 2.0) && (VOC_fdata < 5.0))
		voc_back_color = C24to16(lcdcolor24_table[3]);
	else if(VOC_fdata >= 5.0)
		voc_back_color = C24to16(lcdcolor24_table[4]);
}
u16 lcd_updata_ct = 0;
void lcd_updata_time(void)
{
	lcd_updata_ct ++;
}
void lcd_updata_task(void)
{
	static u8 pm25_weishu = 0;
	static u8 co2_weishu = 0;
	char lcd_buf0[10];
	
	
	if(lcd_updata_ct > 1000)
	{
		lcd_updata_ct = 0;
		
		lcd_get_pm25_back_color();
		lcd_get_co2_back_color();
		lcd_get_hcho_back_color();
		lcd_get_voc_back_color();
		if(pm25_back_color != last_pm25_back_color)
		{
			last_pm25_back_color = pm25_back_color;
			lcd_updata_pm25bmp();
		}
		if(co2_back_color != last_co2_back_color)
		{
			last_co2_back_color = co2_back_color;
			lcd_updata_co2bmp();
		}
		if(hcho_back_color != last_hcho_back_color)
		{
			last_hcho_back_color = hcho_back_color;
			lcd_updata_hchobmp();
		}
		if(voc_back_color != last_voc_back_color)
		{
			last_voc_back_color = voc_back_color;
			lcd_updata_vocbmp();
		}
		
		if(PM2_5_data < 10)
		{
			if(pm25_weishu == 2)
				DrawSingleAscii_big4_s(42,95,"  ", BLACK,pm25_back_color,0);
			sprintf(lcd_buf0,"%-01d",PM2_5_data);
			DrawSingleAscii_big4_s(56,95,(u8*)lcd_buf0, BLACK,pm25_back_color,0);
			pm25_weishu = 1;
		}
		else if(PM2_5_data < 100)
		{
			if(pm25_weishu == 3)
				DrawSingleAscii_big4_s(28,95,"   ", BLACK,pm25_back_color,0);
			
			sprintf(lcd_buf0,"%-02d",PM2_5_data);
			DrawSingleAscii_big4_s(42,95,(u8*)lcd_buf0, BLACK,pm25_back_color,0);
			
			pm25_weishu = 2;
		}
		else if(PM2_5_data < 1000)
		{
			if(pm25_weishu == 4)
				DrawSingleAscii_big4_s(14,95,"    ", BLACK,pm25_back_color,0);
			
			sprintf(lcd_buf0,"%-03d",PM2_5_data);
			DrawSingleAscii_big4_s(28,95,(u8*)lcd_buf0, BLACK,pm25_back_color,0);
			
			pm25_weishu = 3;
		}
		else
		{
			sprintf(lcd_buf0,"%-04d",PM2_5_data);
			DrawSingleAscii_big4_s(14,95,(u8*)lcd_buf0, BLACK,pm25_back_color,0);
			
			pm25_weishu = 4;
		}
		if(CO2_data < 400)
			CO2_data = 400;
		if(CO2_data < 1000)
		{
			if(co2_weishu == 4)
				DrawSingleAscii_big6_s(144,29,"    ", BLACK,co2_back_color,0);
			sprintf(lcd_buf0,"%-03d",CO2_data);
			DrawSingleAscii_big6_s(153,29,(u8*)lcd_buf0, BLACK,co2_back_color,0);
			co2_weishu = 3;
		}
		else
		{
			sprintf(lcd_buf0,"%-04d",CO2_data);
			DrawSingleAscii_big6_s(144,29,(u8*)lcd_buf0, BLACK,co2_back_color,0);
			co2_weishu = 4;
		}
		
		sprintf(lcd_buf0,"%.3f",hcho_fdata);
		DrawSingleAscii_big6_s(210,103,(u8*)lcd_buf0,BLACK,hcho_back_color,0);
		
		sprintf(lcd_buf0,"%1.2f",VOC_fdata);
		DrawSingleAscii_big6_s(144,178,(u8*)lcd_buf0,BLACK,voc_back_color,0);
		
		sprintf(lcd_buf0,"%d¡æ",(int)tem_t);
		LCD_ShowString(35,210,24,(u8*)lcd_buf0,WHITE,BLACK);
		sprintf(lcd_buf0,"%-02d%%",tem_h);
		LCD_ShowString(275,210,24,(u8*)lcd_buf0,WHITE,BLACK);
	}
}










