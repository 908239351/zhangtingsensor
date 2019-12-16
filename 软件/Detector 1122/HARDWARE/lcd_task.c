#include "lcd_task.h"
#include "usart.h"
#include "gc9306.h"
#include "timer.h"
#include "uart2.h"
#include "SHT2x.H"
#include "adc.h"
#include "w25qxx.h"
#include "key_task.h"
#include "timer.h"










u16 pm25_curve_buf[320];
u16 co2_curve_buf[320];
u16 hcho_curve_buf[320];
u16 lcd_curve_while_ct = 0;
u8 lcd_curve_while_flag = 0;


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


u8 lcd_yemian_flag = 0;
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
	LCD_ShowString(40,150,24,"ug/方",BLACK,pm25_back_color);
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
	LCD_ShowString(239,82,16,"甲醛",BLACK,hcho_back_color);
	LCD_ShowString(235,140,16,"mg/方",BLACK,hcho_back_color);
}
void lcd_updata_vocbmp(void)
{
	LCD_DrawFullCircle(180,194,45,voc_back_color);
	LCD_ShowString(164,157,16,"TVOC",BLACK,voc_back_color);
	LCD_ShowString(160,215,16,"mg/方",BLACK,voc_back_color);
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
/*******************************************************************************
* Function Name  : lcd_show_curve_data
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void lcd_show_curve_data(u8 type)
{
	if(type == 1)
	{
		LCD_ShowChar(8,24,'5',16,BRRED,BLACK);
		LCD_ShowChar(8,64,'4',16,BRRED,BLACK);
		LCD_ShowChar(8,104,'3',16,BRRED,BLACK);
		LCD_ShowChar(8,144,'2',16,BRRED,BLACK);
		LCD_ShowChar(8,184,'1',16,BRRED,BLACK);
		
		LCD_ShowChar(8,0,'x',16,BRRED,BLACK);
		LCD_ShowChar(8+8*1,0,'1',16,BRRED,BLACK);
		LCD_ShowChar(8+8*2,0,'0',16,BRRED,BLACK);
		LCD_ShowChar(8+8*3,0,'0',16,BRRED,BLACK);
	}
	else if(type == 2)
	{
		if(co2_sensor_type == 1)//炜盛的量程到5000
		{
			LCD_ShowChar(8,24,'5',16,BRRED,BLACK);
			LCD_ShowChar(8,64,'4',16,BRRED,BLACK);
			LCD_ShowChar(8,104,'3',16,BRRED,BLACK);
			LCD_ShowChar(8,144,'2',16,BRRED,BLACK);
			LCD_ShowChar(8,184,'1',16,BRRED,BLACK);
			
			LCD_ShowChar(8,0,'x',16,BRRED,BLACK);//X1000
			LCD_ShowChar(8+8*1,0,'1',16,BRRED,BLACK);
			LCD_ShowChar(8+8*2,0,'0',16,BRRED,BLACK);
			LCD_ShowChar(8+8*3,0,'0',16,BRRED,BLACK);
			LCD_ShowChar(8+8*4,0,'0',16,BRRED,BLACK);
		}
		else//S8的量程到2000
		{
			LCD_ShowChar(8,24,'2',16,BRRED,BLACK);LCD_ShowChar(8+8*1,24,'0',16,BRRED,BLACK);
			LCD_ShowChar(8,64,'1',16,BRRED,BLACK);LCD_ShowChar(8+8*1,64,'6',16,BRRED,BLACK);
			LCD_ShowChar(8,104,'1',16,BRRED,BLACK);LCD_ShowChar(8+8*1,104,'2',16,BRRED,BLACK);
			LCD_ShowChar(8,144,'8',16,BRRED,BLACK);
			LCD_ShowChar(8,184,'4',16,BRRED,BLACK);
			
			LCD_ShowChar(8,0,'x',16,BRRED,BLACK);
			LCD_ShowChar(8+8*1,0,'1',16,BRRED,BLACK);
			LCD_ShowChar(8+8*2,0,'0',16,BRRED,BLACK);
			LCD_ShowChar(8+8*3,0,'0',16,BRRED,BLACK);
		}
	}
	else if(type == 3)
	{
		LCD_ShowChar(8,24,'5',16,BRRED,BLACK);
		LCD_ShowChar(8,64,'4',16,BRRED,BLACK);
		LCD_ShowChar(8,104,'3',16,BRRED,BLACK);
		LCD_ShowChar(8,144,'2',16,BRRED,BLACK);
		LCD_ShowChar(8,184,'1',16,BRRED,BLACK);
		
		LCD_ShowChar(8,0,'x',16,BRRED,BLACK);//X0.1
		LCD_ShowChar(8+8*1,0,'0',16,BRRED,BLACK);
		LCD_ShowChar(8+8*2,0,'.',16,BRRED,BLACK);
		LCD_ShowChar(8+8*3,0,'1',16,BRRED,BLACK);
	}
}
///*******************************************************************************
//* Function Name  : lcd_clear_curve
//* Description    : 
//* Input          : sensor_type=1是PM25，sensor_type=2是S8传感器，sensor_type=3是炜盛传感器，sensor_type=4是甲醛传感器
//* Output         : None
//* Return         : None
//*******************************************************************************/
//void lcd_clear_curve(u8 sensor_type)
//{
//	u16 i = 0;
//	u8 y = 0;
//	u8 p = 0;//缩放比例

//	if(sensor_type == 1)	p = 3;
//	else if(sensor_type == 2)	p = 10;
//	else if(sensor_type == 3)	p = 3;
//	for(i = 0;i < lcd_curve_while_ct;i ++)
//	{
//		if(sensor_type == 1)
//			y = pm25_curve_buf[i] / p;
//		else if(sensor_type == 2)
//			y = co2_curve_buf[i] / p;
//		else if(sensor_type == 3)
//			y = hcho_curve_buf[i] / p;
//		
//		if(y > 230)	y = 230;
//		y = 232 - y;
//		//LCD_DrawLine(232,7+i,y,7+i,BLACK);
//		GC9306_DrawPoint(7+i,y,BLACK);
//	}
//}
///*******************************************************************************
//* Function Name  : lcd_show_curve
//* Description    : 
//* Input          : None
//* Output         : None
//* Return         : None
//*******************************************************************************/
//void lcd_show_curve(u8 first,u8 sensor_type)
//{
//	u16 i = 0;
//	u8 y = 0;
//	u8 p = 0;//缩放比例
//	
//	if(sensor_type == 1)	p = 3;
//	else if(sensor_type == 2)	p = 10;
//	else if(sensor_type == 3)	p = 3;
//	if((lcd_curve_while_flag == 0) && (first == 2))
//	{
//		i = lcd_curve_while_ct - 1;
//		if(sensor_type == 1)
//			y = pm25_curve_buf[i] / p;
//		else if(sensor_type == 2)
//			y = co2_curve_buf[i] / p;
//		else if(sensor_type == 3)
//			y = hcho_curve_buf[i] / p;
//		
//		if(y > 230)	y = 230;
//		y = 232 - y;
//		//LCD_DrawLine(232,7+i,y,7+i,Chengse);
//		GC9306_DrawPoint(7+i,y,Chengse);
//	}
//	else
//	{
//		for(i = 0;i < lcd_curve_while_ct;i ++)
//		{
//			if(sensor_type == 1)
//				y = pm25_curve_buf[i] / p;
//			else if(sensor_type == 2)
//				y = co2_curve_buf[i] / p;
//			else if(sensor_type == 3)
//				y = hcho_curve_buf[i] / p;
//			y = 232 - y;
//			//LCD_DrawLine(232,7+i,y,7+i,Chengse);
//			GC9306_DrawPoint(7+i,y,Chengse);
//		}
//	}
//}

/*******************************************************************************
* Function Name  : lcd_clear_curve
* Description    : 
* Input          : sensor_type=1是PM25，sensor_type=2是CO2传感器，sensor_type=3是甲醛传感器
* Output         : None
* Return         : None
*******************************************************************************/
void lcd_clear_curve(u8 sensor_type)
{
	u16 i = 0;
	u8 y = 0;
	u8 p = 0;//缩放比例

	if(sensor_type == 1)	p = 3;
	else if(sensor_type == 2)	
	{
		if(co2_sensor_type == 0)	p = 10;
		else if(co2_sensor_type == 1)	p = 25;
	}
	else if(sensor_type == 3)	p = 3;
	for(i = 0;i < lcd_curve_while_ct;i ++)
	{
		if(sensor_type == 1)
			y = pm25_curve_buf[i] / p;
		else if(sensor_type == 2)
			y = co2_curve_buf[i] / p;
		else if(sensor_type == 3)
			y = hcho_curve_buf[i] / p;
		
		if(y > 230)	y = 230;
		y = 232 - y;
		//LCD_DrawLine(232,7+i,y,7+i,BLACK);
		GC9306_DrawPoint(7+i,y,BLACK);
	}
}
/*******************************************************************************
* Function Name  : lcd_show_curve
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void lcd_show_curve(u8 first,u8 sensor_type)
{
	u16 i = 0;
	u8 y = 0;
	u8 p = 0;//缩放比例
	
	if(sensor_type == 1)	p = 3;
	else if(sensor_type == 2)	
	{
		if(co2_sensor_type == 0)	p = 10;
		else if(co2_sensor_type == 1)	p = 25;
	}
	else if(sensor_type == 3)	p = 3;
	if((lcd_curve_while_flag == 0) && (first == 2))
	{
		i = lcd_curve_while_ct - 1;
		if(sensor_type == 1)
			y = pm25_curve_buf[i] / p;
		else if(sensor_type == 2)
			y = co2_curve_buf[i] / p;
		else if(sensor_type == 3)
			y = hcho_curve_buf[i] / p;
		
		if(y > 230)	y = 230;
		y = 232 - y;
		//LCD_DrawLine(232,7+i,y,7+i,Chengse);
		GC9306_DrawPoint(7+i,y,Chengse);
	}
	else
	{
		for(i = 0;i < lcd_curve_while_ct;i ++)
		{
			if(sensor_type == 1)
				y = pm25_curve_buf[i] / p;
			else if(sensor_type == 2)
				y = co2_curve_buf[i] / p;
			else if(sensor_type == 3)
				y = hcho_curve_buf[i] / p;
			y = 232 - y;
			//LCD_DrawLine(232,7+i,y,7+i,Chengse);
			GC9306_DrawPoint(7+i,y,Chengse);
		}
	}
}
/*******************************************************************************
* Function Name  : lcd_show_zuobiao_point
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void lcd_show_zuobiao_point(void)
{
	u16 x,y = 0;
	for(y = 36;y < 197;)
	{
		for(x = 46;x < 290;)
		{
			GC9306_DrawPoint(x,y,WHITE);
			x = x + 40;
		}
		y = y + 40;
	}
}
/*******************************************************************************
* Function Name  : lcd_show_zuobiao
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void lcd_show_zuobiao(u8 sensor_type)
{
	LCD_Clear(BLACK);
	LCD_DrawLine(5,0,5,239,WHITE);
	LCD_DrawLine(6,0,6,239,WHITE);
	
	LCD_DrawLine(0,234,319,234,WHITE);
	LCD_DrawLine(0,233,319,233,WHITE);
	
	LCD_Fill(46,235,1,2,WHITE);
	LCD_Fill(86,235,1,2,WHITE);
	LCD_Fill(126,235,1,2,WHITE);
	LCD_Fill(166,235,1,2,WHITE);
	LCD_Fill(206,235,1,2,WHITE);
	LCD_Fill(246,235,1,2,WHITE);
	LCD_Fill(286,235,1,2,WHITE);
	
	LCD_Fill(2,194,3,1,WHITE);
	LCD_Fill(2,154,3,1,WHITE);
	LCD_Fill(2,114,3,1,WHITE);
	LCD_Fill(2,74,3,1,WHITE);
	LCD_Fill(2,34,3,1,WHITE);
	
	lcd_show_zuobiao_point();
	
if(sensor_type == 1)
	{
		LCD_ShowString(259,0,16,"PM2.5",WHITE,BLACK);
	}
	if(sensor_type == 2)
	{
		LCD_ShowString(283,0,16,"CO2",WHITE,BLACK);
	}
	if(sensor_type == 3)
	{
		LCD_ShowString(271,0,16,"HCHO",WHITE,BLACK);
	}
	lcd_show_curve_data(sensor_type);
	lcd_show_curve(1,sensor_type);
}
/*******************************************************************************
* Function Name  : lcd_curve_time
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u16 lcd_curve_ct = 800;
void lcd_curve_time(void)
{
	lcd_curve_ct++;
}
/*******************************************************************************
* Function Name  : lcd_curve_task
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void lcd_curve_task(void)
{
	u16 i = 0;
	
	if(lcd_curve_ct > 2000)
	{
		lcd_curve_ct = 0;
		
		if(lcd_curve_while_flag == 0)
		{
			if(lcd_curve_while_ct < 313)
			{
				pm25_curve_buf[lcd_curve_while_ct] = PM2_5_data;
				co2_curve_buf[lcd_curve_while_ct] = CO2_data;
				hcho_curve_buf[lcd_curve_while_ct] = HCHO_data;
				lcd_curve_while_ct ++;
			}
			else
			{
				lcd_curve_while_flag = 1;
			}
		}
		else
		{
			if((lcd_yemian_flag == 1) || (lcd_yemian_flag == 2) || (lcd_yemian_flag == 3))
			{
				lcd_clear_curve(yemian1_sensor_type);
			}
			for(i = 0;i < 312;i++)
			{
				pm25_curve_buf[i] = pm25_curve_buf[i + 1];
				co2_curve_buf[i] = co2_curve_buf[i + 1];
				hcho_curve_buf[i] = hcho_curve_buf[i + 1];
			}
			pm25_curve_buf[312] = PM2_5_data;
			co2_curve_buf[312] = CO2_data;
			hcho_curve_buf[312] = HCHO_data;
		}
		if((lcd_yemian_flag == 1) || (lcd_yemian_flag == 2) || (lcd_yemian_flag == 3))	
		{
			lcd_show_zuobiao_point();
			lcd_show_curve(2,yemian1_sensor_type);
			lcd_show_curve_data(yemian1_sensor_type);
		}
	}
}
/*******************************************************************************
* Function Name  : key_show_klw
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void key_show_klw(void)
{
	char hcho_zero[5];

	LCD_ShowString(20,23,16,"-浓度值-",WHITE,BLACK);
	LCD_ShowString(160,23,16,"-颗粒物个数-",WHITE,BLACK);
	LCD_ShowString(0,55,16,"PM2.5:    ug",Tianlanse,BACK_COLOR1);
	LCD_ShowString(0,87,16,"PM1.0:    ug",Tianlanse,BACK_COLOR1);
	LCD_ShowString(0,119,16," PM10:    ug",Tianlanse,BACK_COLOR1);
	//if(main_driver_sta & 0x04)//有CO2
	{
		LCD_ShowString(0,151,16," CO2:    ppm",Tianlanse,BACK_COLOR1);
	}
	//if(main_driver_sta & 0x08)//有甲醛
	{
		LCD_ShowString(0,183,16,"HCHO:     mg",Tianlanse,BACK_COLOR1);
		LCD_ShowString(0,215,16,"HCHO Zero:",Tianlanse,BACK_COLOR1);
		
		sprintf(hcho_zero,"%d",hcho_jizhun);
		LCD_ShowString(79,215,16,(u8*)hcho_zero,WHITE,BLACK);
	}
	LCD_ShowString(159,55,16,">0.3um:     个",Tianlanse,BACK_COLOR1);
	LCD_ShowString(159,87,16,">0.5um:     个",Tianlanse,BACK_COLOR1);
	LCD_ShowString(159,119,16,">1.0um:     个",Tianlanse,BACK_COLOR1);
	LCD_ShowString(159,151,16,">2.5um:     个",Tianlanse,BACK_COLOR1);
	LCD_ShowString(159,183,16,">5.0um:     个",Tianlanse,BACK_COLOR1);
	LCD_ShowString(159,215,16," >10um:     个",Tianlanse,BACK_COLOR1);
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
		
		if(lcd_yemian_flag == 0)
		{
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
			
			sprintf(lcd_buf0,"%d℃",(int)tem_t);
			LCD_ShowString(35,210,24,(u8*)lcd_buf0,WHITE,BLACK);
			sprintf(lcd_buf0,"%-02d%%",tem_h);
			LCD_ShowString(275,210,24,(u8*)lcd_buf0,WHITE,BLACK);
		}
		else if(lcd_yemian_flag == 4)
		{
			hcho_fdata = (float)HCHO_data;
			hcho_fdata = hcho_fdata / 1000;
			
			sprintf(lcd_buf0,"%d",PM2_5_data);LCD_ShowString(47,55,16,(u8*)lcd_buf0,WHITE,BACK_COLOR1);
			sprintf(lcd_buf0,"%d",PM1_0_data);LCD_ShowString(47,87,16,(u8*)lcd_buf0,WHITE,BACK_COLOR1);
			sprintf(lcd_buf0,"%d",PM10_data);LCD_ShowString(47,119,16,(u8*)lcd_buf0,WHITE,BACK_COLOR1);
			//if(main_driver_sta & 0x04)//有CO2
			{
				sprintf(lcd_buf0,"%d",CO2_data);LCD_ShowString(39,151,16,(u8*)lcd_buf0,WHITE,BACK_COLOR1);
			}
			//if(main_driver_sta & 0x08)//有甲醛
			{
				sprintf(lcd_buf0,"%.3f",hcho_fdata);LCD_ShowString(39,183,16,(u8*)lcd_buf0,WHITE,BACK_COLOR1);
			}
			sprintf(lcd_buf0,"%d",klw_03_data);LCD_ShowString(214,55,16,(u8*)lcd_buf0,WHITE,BACK_COLOR1);
			sprintf(lcd_buf0,"%d",klw_05_data);LCD_ShowString(214,87,16,(u8*)lcd_buf0,WHITE,BACK_COLOR1);
			sprintf(lcd_buf0,"%d",klw_10_data);LCD_ShowString(214,119,16,(u8*)lcd_buf0,WHITE,BACK_COLOR1);
			sprintf(lcd_buf0,"%d",klw_25_data);LCD_ShowString(214,151,16,(u8*)lcd_buf0,WHITE,BACK_COLOR1);
			sprintf(lcd_buf0,"%d",klw_50_data);LCD_ShowString(214,183,16,(u8*)lcd_buf0,WHITE,BACK_COLOR1);
			sprintf(lcd_buf0,"%d",klw_100_data);LCD_ShowString(214,215,16,(u8*)lcd_buf0,WHITE,BACK_COLOR1);
		}
	}
}










