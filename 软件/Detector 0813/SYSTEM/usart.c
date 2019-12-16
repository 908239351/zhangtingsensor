#include "sys.h"
#include "usart.h"	  
#include "uart3.h"
#include "stdio.h"
#include "delay.h"


const u8 PMinterface_head_byte[2] = {0x42,0x4d};//PM25帧头
u16 PM2_5_data = 0;
u16 PM1_0_data = 0;
u16 PM10_data = 0;

u16 klw_03_data = 0;
u16 klw_05_data = 0;
u16 klw_10_data = 0;
u16 klw_25_data = 0;
u16 klw_50_data = 0;
u16 klw_100_data = 0;

float pmsensor_xishu = 1.0;

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}


 /*******************************************************************************
* Function Name  : uart_init
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
void USART1_Init(u32 bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}
/*******************************************************************************
* Function Name  : uart_send_byte
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void uart_send_byte(u8 data)
{
	while((USART1->SR & 0X40) == 0);
	USART1->DR = data;
}
u8 PMinterface_rx_index = 0;
u8 PMinterface_rx_clear_count = 0;
u8 PMinterface_receive_rx_length = 0;

u8 PMinterface_rx_buffer[50];
/*------------------------------------------------------------------------------*
 * 函数名：rx_data_check
 * 描述  ：接收数据校验
 * 输入  ：无
 * 输出  ：校验正确返回0，校验错误返回1
 * 调用  ：
 *-----------------------------------------------------------------------------*/
u8 PMrx_data_check(u8 *buf,u16 length)
{
	u16 check_data = 0;
	u16 rx_data = 0;
	u16 i = 0;
	for(i = 0;i < length-2;i ++)
	{
		check_data += buf[i];
	}
	rx_data = (buf[length-2] << 8) + buf[length-1];
	if(check_data == rx_data)
		return 0;
	else
		return 1;
}
/*******************************************************************************
* Function Name  : interface_receive
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PMinterface_receive(u8 data)
{
	switch(PMinterface_rx_index)
	{
		case 0:
			if(PMinterface_head_byte[PMinterface_rx_index] == data)
			{
				PMinterface_rx_buffer[PMinterface_rx_index] = data;
				PMinterface_rx_index++;
			}
			else PMinterface_rx_index = 0;
			break;
		case 1:
			if(PMinterface_head_byte[PMinterface_rx_index] == data)
			{
				PMinterface_rx_buffer[PMinterface_rx_index] = data;
				PMinterface_rx_index++;
			}
			else PMinterface_rx_index = 0;
			break;
		default:
			PMinterface_rx_buffer[PMinterface_rx_index] = data;
			PMinterface_rx_index++;
			//帧头2字节，帧长度2字节，数据n字节
			PMinterface_receive_rx_length = (PMinterface_rx_buffer[2]<<8) + PMinterface_rx_buffer[3];
			
			if(PMinterface_rx_index >= 4 + PMinterface_receive_rx_length)
			{
				PMinterface_rx_index = 0;//接收完毕
				if(PMrx_data_check(PMinterface_rx_buffer,PMinterface_receive_rx_length+4) == 0)//校验
				{
					PM1_0_data = (PMinterface_rx_buffer[10] << 8) + PMinterface_rx_buffer[11];PM1_0_data *= pmsensor_xishu;
					PM2_5_data = (PMinterface_rx_buffer[12] << 8) + PMinterface_rx_buffer[13];PM2_5_data *= pmsensor_xishu;
					PM10_data = (PMinterface_rx_buffer[14] << 8) + PMinterface_rx_buffer[15];PM10_data *= pmsensor_xishu;
					
					if(PM2_5_data > 5000)
						PM2_5_data = 5000;
					
					klw_03_data = (PMinterface_rx_buffer[16] << 8) + PMinterface_rx_buffer[17];klw_03_data *= pmsensor_xishu;if(klw_03_data > 9999)	klw_03_data = 9999;
					klw_05_data = (PMinterface_rx_buffer[18] << 8) + PMinterface_rx_buffer[19];klw_05_data *= pmsensor_xishu;if(klw_05_data > 9999)	klw_05_data = 9999;
					klw_10_data = (PMinterface_rx_buffer[20] << 8) + PMinterface_rx_buffer[21];klw_10_data *= pmsensor_xishu;if(klw_10_data > 9999)	klw_10_data = 9999;
					klw_25_data = (PMinterface_rx_buffer[22] << 8) + PMinterface_rx_buffer[23];klw_25_data *= pmsensor_xishu;if(klw_25_data > 9999)	klw_25_data = 9999;
					klw_50_data = (PMinterface_rx_buffer[24] << 8) + PMinterface_rx_buffer[25];klw_50_data *= pmsensor_xishu;if(klw_50_data > 9999)	klw_50_data = 9999;
					klw_100_data = (PMinterface_rx_buffer[26] << 8) + PMinterface_rx_buffer[27];klw_100_data *= pmsensor_xishu;if(klw_100_data > 9999)	klw_100_data = 9999;
				}
			}
			break;
	}
  PMinterface_rx_clear_count = 0;
}
/*******************************************************************************
* Function Name  : interface_receive_check
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PMinterface_receive_check(void)
{
  PMinterface_rx_clear_count++;
  if(PMinterface_rx_clear_count > 200)
  {
    PMinterface_rx_clear_count = 0;
    PMinterface_rx_index = 0;
  }
}
/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_IRQHandler(void)
{
	u8 Res1;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		Res1 =USART_ReceiveData(USART1);
		PMinterface_receive(Res1);
  }
}
































