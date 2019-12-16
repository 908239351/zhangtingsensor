#ifndef __GC9306_H
#define __GC9306_H	 
#include "sys.h"


//LCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	wramcmd;		//开始写gram指令
	u16 setxcmd;		//设置x坐标指令
	u16 setycmd;		//设置y坐标指令	 
}_lcd_dev; 	  

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数


#define WORD_COLOR  BLACK//字体颜色
#define WORD_COLOR1  WHITE//字体颜色
#define BACK_COLOR  WHITE//背景色 
#define BACK_COLOR1  BLACK//背景色 


//==========================LCD DEFINE
#define RED      0xf800
#define GREEN    0x07e0
#define BLUE     0x001f
#define BLUE0    0x07ff
#define RED1     0xfbe0
#define WHITE    0xffff
#define BLACK    0x0000	
#define YELLOW   0xFff0

#define BROWN 			 	0XBC40 	//棕色
#define BRRED 			 	0XFC07 	//棕红色
#define GRAY  			 	0X8430 	//灰色
#define DARKBLUE      0X01CF 	//深蓝色
#define LIGHTBLUE     0X7D7C 	//浅蓝色  
#define GRAYBLUE      0X5458 	//灰蓝色

#define	Fense					0xe9ce	//粉色
#define Chase			  	0xd5b1	//茶色
#define Hese		      0xa285	//褐色
#define Tianlanse		 	0x05ff	//天蓝色
#define Jialanse		  0x435c	//皇家蓝
#define Bohese			  0xf5ff	//薄荷色
#define Yinbaise			0xc618	//银白色 
#define Qianhuise		 	0x8410	//浅灰色
#define Chengse			  0xfbea	//橙色

#define	LCD_LED PAout(0) //LCD背光

#define LCDDATA_ODR_Addr(data)   GPIOC->ODR=(GPIOC -> ODR & 0xff00)|(data & 0x00ff)
#define DATAIN   GPIO_ReadInputData(GPIOC)



#define	LCD_CS_SET  GPIOA->BSRR=1<<1     //片选端口
#define	LCD_RS_SET	GPIOD->BSRR=1<<0     //数据/命令   
#define	LCD_WR_SET	GPIOC->BSRR=1<<15    //写数据
#define	LCD_RD_SET	GPIOC->BSRR=1<<14    //读数据
#define	LCD_RST_SET	GPIOD->BSRR=1<<1     //复位
#define	LCD_TE_SET	GPIOB->BSRR=1<<13    //TE
								    
#define	LCD_CS_CLR  GPIOA->BRR=1<<1      //片选端口
#define	LCD_RS_CLR	GPIOD->BRR=1<<0      //数据/命令	   
#define	LCD_WR_CLR	GPIOC->BRR=1<<15     //写数据	
#define	LCD_RD_CLR	GPIOC->BRR=1<<14     //读数据	
#define	LCD_RST_CLR	GPIOD->BRR=1<<1      //复位
#define	LCD_TE_CLR	GPIOB->BRR=1<<13     //TE



#define	HZ24_w_point  24 //24汉字字体大小宽度
#define	HZ24_h_point  24 //24汉字字体大小高度
#define	ZF24_w_point  12 //24字母字体大小宽度
#define	ZF24_h_point  24 //24字母字体大小高度

#define	HZ16_w_point  16 //16汉字字体大小宽度
#define	HZ16_h_point  16 //16汉字字体大小高度
#define	ZF16_w_point  8  //16字母字体大小宽度
#define	ZF16_h_point  16 //16字母字体大小高度



#define  w_unm3  42
#define	 h_unm3  72
#define  w_unm4  28
#define	 h_unm4  48
#define  w_unm5  23
#define	 h_unm5  40
#define  w_unm6  18
#define	 h_unm6  32




void LCD_INIT_GC9306(void);
void LCD_Clear1(u16 color);
void LCD_Clear(u16 color);
void LCD_Fill(u16 x,u16 y,u16 w,u16 h,u16 color);
void GC9306_DrawPoint(u16 Xpos,u16 Ypos,u16 RGB_Code);
//u16 LCD_ReadPoint(u16 x,u16 y);
void LCD_DrawLine(u16 sx, u16 sy, u16 ex, u16 ey,u16 cala);
void Draw_pic(u16 x, u16 y, u16 w, u16 h,const u8 *pHz, u16 LineColor,u16 FillColor);
void Draw_pic1(u16 x, u16 y, u16 w, u16 h,const u8 *pHz, u16 LineColor,u16 FillColor);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u16 LineColor,u16 FillColor);
void LCD_ShowWord(u16 x,u16 y,u8 *pHz,u8 size,u16 LineColor,u16 FillColor);
void LCD_ShowString(u16 x,u16 y,u8 size,u8 *p,u16 LineColor,u16 FillColor);
void DrawSingleAscii_big3_s(u16 x, u16 y, u8 *pStr, u16 LineColor,u16 FillColor,u8 first0);
void DrawSingleAscii_big4_s(u16 x, u16 y, u8 *pStr, u16 LineColor,u16 FillColor,u8 first0);
void DrawSingleAscii_big5_s(u16 x, u16 y, u8 *pStr, u16 LineColor,u16 FillColor,u8 first0);
void DrawSingleAscii_big5(u16 y, u16 x, u8 pAscii, u16 LineColor,u16 FillColor);
void DrawSingleAscii_big6_s(u16 x, u16 y, u8 *pStr, u16 LineColor,u16 FillColor,u8 first0);
void DrawSingleAscii_big6(u16 y, u16 x, u8 pAscii, u16 LineColor,u16 FillColor);
void lcd_show_bj_bmp(u16 x,u16 y,u16 w,u16 h);
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r,u16 RGB_Code);
void LCD_DrawFullCircle(u16 Xpos,u16 Ypos,u16 Radius,u16 RGB_Code);
void LCD_DrawFullCircle1(u16 Xpos,u16 Ypos,u16 Radius,u16 Rs1,u16 RGB_Code,float bbp);
void LCD_DrawFullCircle2(u16 Xpos,u16 Ypos,u16 Radius,u16 Rs1,u16 RGB_Code,float bbp);
u16 piclib_alpha_blend(u16 src,u16 dst,u8 alpha);


#endif

























