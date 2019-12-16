#include "adc.h"
#include "stdio.h"
#include "lcd.h"

/*******************************************************************************
* Function Name  : usb_power_init
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void usb_power_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/*******************************************************************************
* Function Name  : power_Read_Input
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u8 power_Read_Input(void)
{
  if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9))
    return 1;
  else
    return 0;
}
/*******************************************************************************
* Function Name  : Adc_Init
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Adc_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_ADC1,ENABLE);//使能ADC1通道时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	RCC->APB2ENR|=1<<9;    //ADC1时钟使能	  
	RCC->APB2RSTR|=1<<9;   //ADC1复位
	RCC->APB2RSTR&=~(1<<9);//复位结束	    
	RCC->CFGR&=~(3<<14);   //分频因子清零	
	//SYSCLK/DIV2=12M ADC时钟设置为12M,ADC最大时钟不能超过14M!
	//否则将导致ADC准确度下降! 
	RCC->CFGR|=2<<14;      	 

	ADC1->CR1&=0XF0FFFF;   //工作模式清零
	ADC1->CR1|=0<<16;      //独立工作模式  
	ADC1->CR1&=~(1<<8);    //非扫描模式	  
	ADC1->CR2&=~(1<<1);    //单次转换模式
	ADC1->CR2&=~(7<<17);	   
	ADC1->CR2|=7<<17;	   //软件控制转换  
	ADC1->CR2|=1<<20;      //使用用外部触发(SWSTART)!!!	必须使用一个事件来触发
	ADC1->CR2&=~(1<<11);   //右对齐	 
	ADC1->SQR1&=~(0XF<<20);
	ADC1->SQR1&=0<<20;     //1个转换在规则序列中 也就是只转换规则序列1 		
	
	//设置通道1的采样时间
 ADC1->SMPR2&=0X00000000;//通道0,1,2,3,4,5,6,7,8,9采样时间清空
 
 ADC1->SMPR2|=7<<27; //通道9 239.5周期,提高采样时间可以提高精确度 
 ADC1->SMPR2|=7<<24; //通道8 239.5周期,提高采样时间可以提高精确度 
// ADC1->SMPR2|=7<<21; //通道7 239.5周期,提高采样时间可以提高精确度 
// ADC1->SMPR2|=7<<18; //通道6 239.5周期,提高采样时间可以提高精确度 
// ADC1->SMPR2|=7<<15; //通道5 239.5周期,提高采样时间可以提高精确度 
// ADC1->SMPR2|=7<<12; //通道4 239.5周期,提高采样时间可以提高精确度
// ADC1->SMPR2|=7<<9; //通道3 239.5周期,提高采样时间可以提高精确度 
// ADC1->SMPR2|=7<<6; //通道2 239.5周期,提高采样时间可以提高精确度 
// ADC1->SMPR2|=7<<3; //通道1 239.5周期,提高采样时间可以提高精确度 
// ADC1->SMPR2|=7<<0; //通道0 239.5周期,提高采样时间可以提高精确度 
// ADC1->SMPR1&=0XFFFC0000;//通道10,11,12,13,14,15采样时间清空
// ADC1->SMPR1|=7<<15; //通道15 239.5周期,提高采样时间可以提高精确度 
// ADC1->SMPR1|=7<<12; //通道14 239.5周期,提高采样时间可以提高精确度
// ADC1->SMPR1|=7<<9; //通道13 239.5周期,提高采样时间可以提高精确度 
// ADC1->SMPR1|=7<<6; //通道12 239.5周期,提高采样时间可以提高精确度 
// ADC1->SMPR1|=7<<3; //通道11 239.5周期,提高采样时间可以提高精确度 
// ADC1->SMPR1|=7<<0; //通道10 239.5周期,提高采样时间可以提高精确度

	ADC1->CR2|=1<<0;	    //开启AD转换器	 
	ADC1->CR2|=1<<3;        //使能复位校准  
	while(ADC1->CR2&1<<3);  //等待校准结束 			 
  //该位由软件设置并由硬件清除。在校准寄存器被初始化后该位将被清除。 		 
	ADC1->CR2|=1<<2;        //开启AD校准	   
	while(ADC1->CR2&1<<2);  //等待校准结束
}
/*******************************************************************************
* Function Name  : Get_Adc
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u16 Get_Adc(u8 ch)   
{
  //设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);//ADC1,ADC通道,采样时间为239.5周期
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//使能指定的ADC1的软件转换启动功能	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));//等待转换结束
	return ADC_GetConversionValue(ADC1);//返回最近一次ADC1规则组的转换结果
}
/*******************************************************************************
* Function Name  : Get_Adc_Average
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val += Get_Adc(ch);
		delay_ms(10);
	}
	return temp_val/times;
}
//获得ADC值
//ch:通道值 11
u16 VOC_AD = 0,VOC_data = 0,VOC_ZERO = 500;
float VOC_fdata = 0;
void Get_Adc_voc(void)   
{
	u8 i,k;
	static u16 tempunm[10],tempsum[10],temp;
//	//设置转换序列	  		 
//	ADC1->SQR3&=0XFFFFFFE0;//规则序列1 通道ch
//	ADC1->SQR3|=0;		  			    
//	ADC1->CR2|=1<<22;       //启动规则转换通道 
//	while(!(ADC1->SR&1<<1));//等待转换结束	 

	for(i=0;i<9;i++)
	{
		tempunm[i]=tempunm[i+1];
	}
	tempunm[9] = Get_Adc(9);	
	for(i=0;i<10;i++)
	{
		tempsum[i]=tempunm[i];
	}
	for(k=0;k<9;k++)
	{
		for(i=0;i<9-k;i++)
		{
			if(tempsum[i]>tempsum[i+1])
			{
				temp=tempsum[i];
				tempsum[i]=tempsum[i+1];
				tempsum[i+1]=temp;
			}
		}
	}
	VOC_AD=(tempsum[4]+tempsum[5])/2;	
	if(VOC_AD > VOC_ZERO)
		VOC_data=(float)(VOC_AD - VOC_ZERO)/8;	
	else
		VOC_data=0;

	if(VOC_data>999)
		VOC_data=999;
	VOC_fdata = (float)VOC_data / 100;
}
/*******************************************************************************
* Function Name  : power_check_time
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u16 adc_voc_time = 500;
void adc_check_time(void)
{
	adc_voc_time ++;
}
/*******************************************************************************
* Function Name  : power_check_task
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void adc_check_task(void)
{
	if(adc_voc_time > 2000)
	{
		adc_voc_time = 0;
		Get_Adc_voc();
		//adcc = Get_Adc_Average(4,10);
		//printf("ad=%d \r\n",adcc);
	}
}

/*******************************************************************************
* Function Name  : power_check_time
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u16 power_get_ct = 500;
void power_check_time(void)
{
	power_get_ct ++;
}
/*******************************************************************************
* Function Name  : power_check_task
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern const u8 BAT0[];
extern const u8 BAT1[];
extern const u8 BAT2[];
extern const u8 BAT3[];
extern const u8 BAT4[];
extern const u8 BAT5[];
float powerbuf[20] = {3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8};
void power_check_task(void)
{
	u8 power_updata_flag = 0;
	u8 i = 0;
	float power_aver = 0;
	static u8 power_ct = 0;
	u16 adc_dat = 0;
	float power = 0;
	
	//if((lcd_yemian == 0) || (lcd_yemian == 3))
	{
		if(power_get_ct > 1000)
		{
			power_get_ct = 0;
			adc_dat = Get_Adc_Average(8,10);
			power = adc_dat;
			power = ((power / 4096) * 6.6);
			
			powerbuf[power_ct] = power;
			power_ct ++;
			if(power_ct > 19)
				power_ct = 0;
			
			for(i = 0;i < 20;i ++)
			{
				power_aver += powerbuf[i];
			}
			power = power_aver / 20;
			
			if(power_Read_Input() == 1)
			{
				power_updata_flag = 5;
			}
			else
			{
				if(power > 3.9)	{power_updata_flag = 4;}
				else 	if(power > 3.8)	{power_updata_flag = 3;}
				else 	if(power > 3.7)	{power_updata_flag = 2;}
				else  if(power > 3.6){power_updata_flag = 1;}
				else 	{power_updata_flag = 0;}
			}
			
			if(power_updata_flag == 5)
				Draw_pic(270,4,48,2, BAT5,WHITE,BLACK);
			else if(power_updata_flag == 4)
				Draw_pic(270,4,48,2, BAT4,WHITE,BLACK);
			else if(power_updata_flag == 3)
				Draw_pic(270,4,48,2, BAT3,WHITE,BLACK);
			else if(power_updata_flag == 2)
				Draw_pic(270,4,48,2, BAT2,WHITE,BLACK);
			else if(power_updata_flag == 1)
				Draw_pic(270,4,48,2, BAT1,WHITE,BLACK);
			else if(power_updata_flag == 0)
				Draw_pic(270,4,48,2, BAT0,WHITE,BLACK);
		}
	}
}















