#include "c8051F340.h"
#include <intrins.h>
#include <absacc.h>
#include <stdlib.h>

#include <main.h>
#include <usb_api.h>
#include <mmc_sd\mmc_sd.h>
#include <lcd\lcd.h>
#include <tp\tp.h>

xdata unsigned char Packet[64];   // Last packet received from host 
unsigned char f_down=0;
unsigned char f_debug=0;
unsigned char f_read=0;
unsigned char f_para=0;
unsigned char f_init=0;
unsigned char f_other=0;
unsigned char pic=0;
unsigned int  bf=0;
unsigned long int sd=1;

unsigned char VerifyCode0=0xaa;
unsigned char VerifyCode1=0x55;
unsigned char VerifyCode2=0xff;

 unsigned char n;
unsigned int  kmxz=0;								
/********************************************************************************/
code const UINT USB_VID = 0x10C4;
code const UINT USB_PID = 0xEA61;
code const BYTE USB_MfrStr[] = {0x1A,0x03,'S',0,'i',0,'l',0,'i',0,'c',0,'o',0,'n',0,' ',0,'L',0,'a',0,'b',0,'s',0};                       // Manufacturer String
code const BYTE USB_ProductStr[] = {0x10,0x03,'U',0,'S',0,'B',0,' ',0,'A',0,'P',0,'I',0}; // Product Desc. String
code const BYTE USB_SerialStr[] = {0x0a,0x03,'F',0,'3',0,'4',0,'0',0};
code const BYTE USB_MaxPower = 15;            // Max current = 30 mA (15 * 2)
code const BYTE USB_PwAttributes = 0x80;      // Bus-powered, remote wakeup not supported
code const UINT USB_bcdDevice = 0x0100;       // Device release number 1.00

/********************************************************************************/
void main(void)
{	 kmxz=2;
	#ifdef SD_DEMO_PIC
	//	unsigned char n;
	#endif

	Devices_Init();
	//MMC_SD_ReadSingleBlock(0);
	while(KEY_STEP)
	{	
	
 	switch (kmxz) 
		{
			case 0 :
				{
			//红
				DispColor(RED);
				WaitKey();

			//绿
				DispColor(GREEN);
				WaitKey();
		    //蓝
				DispColor(BLUE);
				WaitKey();

		    //白
				DispColor(WHITE);
				WaitKey();

			//黑	
			 	DispColor(BLACK);
  	         	WaitKey();

			 //三色彩条	
			 DispBand();	  //多色
  	         WaitKey();

			 //视区框	
			 	DispFrame();
  	         	WaitKey();

				break;		 }

			case 1 :
				{
						//红
				DispColor(RED);
	 			Delay(2000);

			//绿
				DispColor(GREEN);
	 			Delay(2000);

		    //蓝
				DispColor(BLUE);
	 			Delay(2000);

		    //白
				DispColor(WHITE);
	 			Delay(2000);

			//黑	
			 	DispColor(BLACK);
	 			Delay(2000);

			 //三色彩条	
			 	DispBand();	  //多色
	 		 	Delay(2000);

			 //视区框	
			 	DispFrame();
	 			Delay(2000);
				break;
					}
			
			case 2	:
				{
			 	DispBand();	  //多色
	 		 	Delay(2000);
			//黑	
			 	DispColor(BLACK);
	 			Delay(2000);
		    //白
				DispColor(WHITE);
	 			Delay(2000);
				break;	}
			default:	
				DispBand();	  //多色
	 		 	Delay(2000);
			}
		#ifdef READ_REG	         
			//DispRegValue(0xbf,6); //
			//DispRegValue(0xD3,4); //
			//DispRegValue(0xbf,6); //
            DispRegValue(0x04,8); // read ID	   7789

			 Delay(3200);
		 //
		//	  StopDelay(20); 
		//	Delay(2500);
		#endif 
	
		#ifdef USE_SD_INIT
			SD_Init(); 
			Delay(10);
		#endif
		//WaitKey();
	//	Delay(5000);           
	  
		#ifdef SLEEP_TEST
		EnterSLP();
		WaitKey();Delay(200);
		ExitSLP();
		WaitKey();Delay(200);
		#endif
	
	

	
//		DispColor(WHITE);
	//	WaitKey();
//		Delay(2000);	
	

	
	
		
					  
		
		#ifdef DEBUG  
			Debug();
		#endif 
	
		#ifdef EVAL_PIC
			DispPic(pic_eval);
			WaitKey();Delay(200);
		#endif

		 
				
		#ifdef EXTEND_TEST
			DispScaleHor1();
			WaitKey();Delay(200);
	
			DispScaleVer();
			WaitKey();Delay(200);
	
			DispScaleVer_Red();
			WaitKey();Delay(200);
	
			DispScaleVer_Green();
			WaitKey();Delay(200);
	
			DispScaleVer_Blue();
			WaitKey();Delay(200);
	
			DispScaleVer_Gray();
			WaitKey();Delay(200);
	
			DispGrayHor16();
			WaitKey();Delay(200);
	
			DispGrayHor32();
			WaitKey();Delay(200);
	
			DispScaleHor2();
			WaitKey();Delay(200);
	
			DispSnow();
			WaitKey();Delay(200);
	
			DispBlock();
			WaitKey();Delay(200);
		#endif
	
		#ifdef SD_DEMO_PIC
			for(n=0;n<1;n++)
			{
				DispPicFromSD(n);
				//StopDelay(20); 
				//Delay(1000);
		        //DispColor(WHITE);
				WaitKey();
			    //Delay(2500);
			}
			break;
		#endif
	}

	//touch panel draw line test

//	#ifdef TP_TEST
//		TP_DrawLine();
//	#endif


	//DispStr("TEST END",0,0,BLACK,WHITE); //test end
	//while(1); 
}

//----------------------------------------------------------------------
void Delay(unsigned int dly)
{
    unsigned int i,j;

    for(i=0;i<dly;i++)
    	for(j=0;j<255;j++);
}


void StopDelay(unsigned int sdly)
{
    unsigned int i,j,k;
	for(k=0;k<100;k++)
	{
	if(KEY_Stop)
		{
    for(i=0;i<sdly;i++)
    	for(j=0;j<255;j++);
		}
		else
		{
			WaitKey();Delay(20);
		}	
	}
}

#ifdef AUTO_NEXT
void WaitKey(void)
{  
	unsigned int i;

	for(i=0;i<300;i++)
	{
		Delay(1);
		 
		#ifdef CONN_USB
			ConnToUSB();
		#endif	

		while(!KEY_STEP);

	} 
}
#endif

#ifdef MANUA_NEXT
void WaitKey(void)
{  
	while(KEY_STEP)
	{
		#ifdef CONN_USB
		ConnToUSB();
		#endif
	}
		
	Delay(10);
	while(!KEY_STEP);

}
#endif

//f_debug
//0     1      2        3            ，    4	       5	   6	  	   7
//0xaa  0x55  0xff   是否显示在LCD      指令或数据  dataH    dataL   显示的水平位置

//f_read
//0     1      2        3            ，    4	       5	   6	  	  7			     8
//0xff  0x55  0xaa   是否显示在LCD      是否有效    dataH    dataL   读参数的次数    显示的水平位置
//reg_data[]={dataH,dataL,dataH,dataL,dataH,dataL...  } 

//f_down
//0     1      2        3            ，    4	    5	   6	  	  7	   ...  35	     36
//0xaa  0xff  0x55   是否写入SD卡 		  dataH,   dataL, dataH,   dataL,  ... dataH	dataL

//f_init
//0     1      2        3    ，            4	    5	   6	      7    ... 	35	     36
//0xff  0xaa  0x55      -               指令或数据  dataH  dataL      -		
	
#ifdef CONN_USB
void ConnToUSB(void)
{
	unsigned char i;
	xdata unsigned char reg_data[24]; 
	xdata unsigned char para_data[64]; 	
	unsigned char dat;
	unsigned int  reg_index;
	unsigned char data_num;
	unsigned char st[1]={1};
	
	//DispStr("DEBUG FOR USB",0,0,BLACK,WHITE);
	while(f_debug)
	{
	   for(i=1;i<16;i++)
	   {
	       if(Packet[i*4]==0x00)
		   {
		   		WriteComm((Packet[i*4+1]<<8)|Packet[i*4+2]);	
		   }
		   else if(Packet[i*4]==0x01)
		   {
		   		WriteData((Packet[i*4+1]<<8)|Packet[i*4+2]);
	
		   }
		   else if(Packet[i*4]==0x80)
		   {
		   		//	
		   }
	   } 
	   _nop_();_nop_();_nop_();_nop_(); _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	   _nop_();_nop_();_nop_();_nop_(); _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();

	   DispStr("USB DEBUG",0,0,BLACK,WHITE);

	   if((Packet[3])&0x01)
	   {
		   for(i=1;i<16;i++)
		   {
		       if(Packet[i*4]==0x00)
			   {	
					DispInt((Packet[i*4+1]<<8)|Packet[i*4+2],(FONT_W*4+4)*Packet[i*4+3],(FONT_H+1)*i,GREEN,BLACK);	
			   }
			   else if(Packet[i*4]==0x01)
			   {
					DispInt((Packet[i*4+1]<<8)|Packet[i*4+2],(FONT_W*4+4)*Packet[i*4+3],(FONT_H+1)*i,BLUE,WHITE);			
			   }
			   else if(Packet[i*4]==0x80)
			   {
			   		//	
			   }
		   } 
	   }
	   _nop_();_nop_();_nop_();_nop_(); _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	   _nop_();_nop_();_nop_();_nop_(); _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();

	   f_debug=0;  
   }


	//////////////////////////////////////////////////////
	while(f_other)
	{	
       for(i=0;i<64;i+=2)
       {
		   CS0=0;
		   //RD0=1;
 		   RS=1;
	       WriteDispData(Packet[i], Packet[i+1]);	   
		   CS0=1;

	       buffer[bf++]=Packet[i];
	       buffer[bf++]=Packet[i+1];
		   if(bf==512)
           {   	  
               SD_Start();
			   MMC_SD_WriteSingleBlock(sd++); 
			   buffer[0]=pic;
	           MMC_SD_WriteSingleBlock(0);	    //统计图片数量 	buffer[0]=pi
	           bf=0;
           }
       } 

	   f_other=0;  
       Block_Write(st, 1);
   	}

	//////////////////////////////////////////////////////
	while(f_down)
	{
		if(Packet[3]&&0x01)	//download to sd
		{	
	       for(i=4;i<36;i+=2)
	       {
			   CS0=0;
			   //RD0=1;
	 		   RS=1;
		       WriteDispData(Packet[i], Packet[i+1]);	   
			   CS0=1;

			   buffer[bf++]=Packet[i];
		       buffer[bf++]=Packet[i+1];
	
			   if(bf==512)
	           {   	  
	               SD_Start();
				   MMC_SD_WriteSingleBlock(sd++); 
				   buffer[0]=pic;
		           MMC_SD_WriteSingleBlock(0);	    
		           bf=0;
	           }
	
	       } 
		   f_down=0; 
		   Block_Write(st, 1);

	   }
	   else
	   {
	   	   for(i=4;i<36;i+=2)
	       {
			   CS0=0;
			   //RD0=1;
	 		   RS=1;
		       WriteDispData(Packet[i], Packet[i+1]);	   
			   CS0=1;
	
	       } 
		   f_down=0; 
		   Block_Write(st, 1);
	   }
   	}

    //////////////////////////////////////////////////////
	while(f_init)
	{
   	   for(i=4;i<36;i+=4)
       {
	       if(Packet[i]==0x00)
		   {
		   		WriteComm((Packet[i+1]<<8)|Packet[i+2]);	
		   }
		   else if(Packet[i]==0x01)
		   {
		   		WriteData((Packet[i+1]<<8)|Packet[i+2]);	
		   }
		   else if(Packet[i]==0x10)
		   {
				//Delay(Packet[i+1]);	
				//Delay(Packet[i+2]);	
				_nop_();_nop_();_nop_();_nop_(); _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
				_nop_();_nop_();_nop_();_nop_(); _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
		   }
		   else if(Packet[i]==0x80)
		   {
		   		//
		   }
       } 
	   f_init=0;
	   Block_Write(st, 1);
	}

	//////////////////////////////////////////////////////
	while(f_read)
	{
		for(i=0;i<24;i++)
			reg_data[i]=0x00;	

		data_num = Packet[7]*2;	
		reg_index=(Packet[5]<<8)|Packet[6];
			
		if(Packet[4]==0x01)
		{
			WriteComm(reg_index);
			for(i=0;i<data_num;i=i+2)
			{
				dat=ReadData();
				reg_data[i]=dat>>8;
				reg_data[i+1]=dat;
			}
		}

		f_read=0;  
		Block_Write(reg_data,data_num);

		Delay(100);
		DispStr("USB READ",0,0,BLACK,WHITE);
		if((Packet[3])&0x01)
		{
			DispInt(reg_index,(FONT_W*4+4)*(Packet[8]),FONT_H+1,GREEN,BLACK);
			for(i=0;i<data_num;i=i+2)
			{
				DispInt((reg_data[i]<<8)|reg_data[i+1],(FONT_W*4+4)*(Packet[8]),(FONT_H+1)*(i/2+2),BLUE,WHITE);			
			}
		}
   	}

	//////////////////////////////////////////////////////
	while(f_para)
	{
		for(i=0;i<64;i++)
			para_data[i]=0x00;	

		para_data[0]=((unsigned int)ROW)>>8;
		para_data[1]= (unsigned int)ROW;
		para_data[2]=((unsigned int)COL)>>8;
		para_data[3]=((unsigned int)COL);

		f_para=0;  
		Block_Write(para_data,64);
   	}
}
#endif

void  MCU_Init(void)
{
    unsigned char n;
	PCA0MD &= ~0x40;

	//OSC
    OSCICN = 0x83; 	  //Internal oscillator enabled, SYSCLK = Internal Oscillator = 12MHz
    CLKMUL = 0x00;    //Select internal oscillator as clock multiplier source clock

    CLKMUL |= 0x80;   //Enable clock multiplier

	for(n=0;n<255;n++);

	CLKMUL |= 0xC0;           //Initialize the clock multiplier
	while(!(CLKMUL & 0x20));  //Wait for multiplier to lock
    CLKSEL = 0x00;            //Change SYSCLK to (4x Multiplier    ) = 48MHz  (48MHz is as fast as F34 will run); 0x03;
   
    RSTSRC   |= 0x02;  

	//P0~P4					 //set high impedance ,	skipped by crossbar
	P0MDIN |= 0xFF;          //数字输入(无模拟输入）
	P0MDOUT |= 0xFF; 		 //推挽输出（可选开漏输出，输出设为推挽，输入设为开漏）

    P1MDOUT |= 0xFF;         
    P1MDIN |= 0xFF; 

    P2MDOUT |= 0xFF; 				
	P4MDOUT |= 0xFF;         

	P3MDIN |= 0xFF;
	P3MDOUT |= 0x17;		 //0001 0111

	OE=0;
	DIR=0;

	TP_PENIRQ = 1;
	TP_DOUT = 1;

    XBR0     |= 0x02;
    XBR1     |= 0x40;       // Enable Crossbar

}

void SD_Start(void)
{
	static unsigned char f;
	
	if(f==0)
	{
		#ifdef USE_SD_INIT
			SD_Init();
			Delay(10);
		#endif
		f=1;
	}
}

void Devices_Init(void)
{
    //MCU
	MCU_Init();

	//USB
	USB_Clock_Start();                     // Init USB clock *before* calling USB_Init
    USB_Init(USB_VID,USB_PID,USB_MfrStr,USB_ProductStr,USB_SerialStr,USB_MaxPower,USB_PwAttributes,USB_bcdDevice);
    USB_Int_Enable();

	//SPI
	SPI0CFG   = 0x70;
    SPI0CN    = 0x0f;

	//LCM
    LCD_Init();

 	//SD
	#ifdef USE_SD_INIT
		//SD_Init();
		//Delay(50);
	#endif

}

void Suspend_Device(void)
{
   USB_Suspend();
}

//----------------------------------------------------------------------
void  USB_API_TEST_ISR(void) interrupt 17
{
   unsigned char INTVAL = Get_Interrupt_Source();

   if (INTVAL & RX_COMPLETE)
   {
	   Block_Read(Packet,64);

	   if(Packet[0]==VerifyCode0 && Packet[1]==VerifyCode1 && Packet[2]==VerifyCode2) 
	   {  
	   		f_debug=1;
	   }
	   else if(Packet[0]==VerifyCode2 && Packet[1]==VerifyCode1 && Packet[2]==VerifyCode0)
	   {
	   		f_read=1;
	   }
	   else if(Packet[0]==VerifyCode1 && Packet[1]==VerifyCode0 && Packet[2]==VerifyCode2)
	   {
	   		f_para=1;
	   }
	   else if(Packet[0]==VerifyCode0 && Packet[1]==VerifyCode2 && Packet[2]==VerifyCode1)
	   {
	   		f_down=1;
	   }
	   else if(Packet[0]==VerifyCode2 && Packet[1]==VerifyCode0 && Packet[2]==VerifyCode1)
	   {
	   		f_init=1;
	   }
	   else 
	   {
	   		f_other=1;
	   }
   }

   if (INTVAL & TX_COMPLETE)
   { 			   
		
   }
      
   if (INTVAL & DEV_SUSPEND)
   {
        Suspend_Device();
   }

   if (INTVAL & DEV_CONFIGURED)
   {
       MCU_Init();
   }

   if (INTVAL & DEVICE_OPEN)
   {
       BlockWrite(0,COL-1,0,ROW-1);
   }

   if (INTVAL & DEVICE_CLOSE)
   {
       pic=pic+1;
       BlockWrite(0,COL-1,0,ROW-1);
   }
}
