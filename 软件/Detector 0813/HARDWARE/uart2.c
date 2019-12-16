#include "delay.h"
#include "uart2.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	   
	   




const u8 interface_head3_byte[2] = {0xff,0x17};//甲醛帧头
u16 HCHO_data = 0;
float hcho_fdata = 0;
u16 hcho_jizhun = 0;


/*******************************************************************************
* Function Name  : uart2_init
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_Init(u32 bound)
{
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  
	USART_DeInit(USART2);  //复位串口1
	//USART2_TX   PA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA2
   
  //USART2_RX	  PA.3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA3

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
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

  USART_Init(USART2, &USART_InitStructure);
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  USART_Cmd(USART2, ENABLE);
}
/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void interface_receive(u8 data);
void USART2_IRQHandler(void)
{
	u8 Res2;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		Res2 = USART_ReceiveData(USART2);
		interface_receive(Res2);
	}
}
/*******************************************************************************
* Function Name  : usart2_send_byte
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void usart2_send_byte(u8 byte)
{
	while((USART2 -> SR & 0X40) == 0);
  USART2->DR = byte;
}
/*******************************************************************************
* Function Name  : FucCheckSum
* Description    : 甲醛校验方法
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u8 FucCheckSum(u8 *i,u8 ln)
{
	u8 j,tempq = 0;
	i += 1;
	for(j = 0;j < (ln - 2);j ++)
	{
		tempq += *i;
		i++;
	}
	tempq = (~tempq) + 1;
	return tempq;
}
u8 interface_rx_index = 0;
u8 interface_rx_clear_count = 0;
u8 interface_receive_rx_length = 0;
u8 interface_rx_buffer[50];
/*******************************************************************************
* Function Name  : interface_receive
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern u8 main_driver_sta;
void interface_receive(u8 data)
{
	switch(interface_rx_index)
	{
		case 0:
			if(interface_head3_byte[interface_rx_index] == data)
			{
				interface_rx_buffer[interface_rx_index] = data;
				interface_rx_index++;
			}
			else interface_rx_index = 0;
		break;
		case 1:
			if(interface_head3_byte[interface_rx_index] == data)
			{
				interface_rx_buffer[interface_rx_index] = data;
				interface_rx_index++;
			}
			else interface_rx_index = 0;
		break;
		default:
			interface_rx_buffer[interface_rx_index] = data;
			interface_rx_index++;
			if(interface_rx_index >= 9)
			{
				interface_rx_index = 0;//接收完毕
				if(interface_rx_buffer[8] == FucCheckSum(interface_rx_buffer,9))//校验
				{
					HCHO_data = (interface_rx_buffer[4] << 8) + interface_rx_buffer[5];//原始值单位ppb
					
					hcho_fdata = (float)HCHO_data;
					hcho_fdata = hcho_fdata / 1000;//单位是ppm
					hcho_fdata *= 1.25;//单位是mg/m3
//					if((main_driver_sta & 0x80) == 0)
//						main_driver_sta |= 0x08;
//					if(key_hcho_jiaozheng_flag == 2)
//					{
//						hcho_jizhun = HCHO_data;
//						key_hcho_jiaozheng_flag = 3;
//						Test_Write(FLASH_SAVE_ADDR,hcho_jizhun);
//					}
//					if(HCHO_data > hcho_jizhun)
//						HCHO_data = HCHO_data - hcho_jizhun;
//					else
//						HCHO_data = 0;
				}
			}
		break;
	}
  interface_rx_clear_count = 0;
}
/*******************************************************************************
* Function Name  : interface_receive_check
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void interface_receive_check(void)
{
  interface_rx_clear_count++;
  if(interface_rx_clear_count > 200)
  {
    interface_rx_clear_count = 0;
    interface_rx_index = 0;
  }
}









