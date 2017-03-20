/*********************************Copyright (c)*********************************                                      
  *                                             
  ******************************************************************************
  * �ļ����ƣ�tft.c
  * ��    �����ļ�
  * �� �� ��: ����
  * ��������: 
  * �� �� ��:             
  * �޸�����: 
  * ��    ��: V1.0.0
  ******************************************************************************
  * attention
  *	Һ������
  ******************************************************************************
  */
#include "bsp.h"
//#include "tft_font.h"

//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;    

uint16_t POINT_COLOR=0x0000;//������ɫ
uint16_t BACK_COLOR=BLACK;  //����ɫ 


//д�Ĵ�������
//data:�Ĵ���ֵ
void LCD_WR_REG(uint16_t data)
{ 
	LCD_RS_CLR;
 	LCD_CS_CLR; 
	DATAOUT(data); 
	LCD_WR_CLR; 
	LCD_WR_SET; 
 	LCD_CS_SET;   
}

//д���ݺ���
//�������LCD_WR_DATA��,��ʱ�任�ռ�.
//data:�Ĵ���ֵ
void LCD_WR_DATA(uint16_t data)
{
	LCD_RS_SET;
	LCD_CS_CLR;
	DATAOUT(data);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET;
}
////��LCD����
////����ֵ:������ֵ
uint16_t LCD_RD_DATA(void)
{										   
 	uint16_t r;
 	GPIOB->CRL=0X88888888; //PB0-7  ��������
	GPIOB->CRH=0X88888888; //PB8-15 ��������
	GPIOB->ODR=0X0000;     //ȫ�����0

	LCD_RS_SET;
	LCD_CS_CLR;
	//��ȡ����(���Ĵ���ʱ,������Ҫ��2��)
	LCD_RD_CLR;
	r=DATAIN;  
	LCD_RD_SET;
	LCD_CS_SET; 

	GPIOB->CRL=0X33333333; //PB0-7  �������
	GPIOB->CRH=0X33333333; //PB8-15 �������
	GPIOB->ODR=0XFFFF;    //ȫ�������
	return r;	
}
uint16_t LCD_RD_REG_DATA(void)
{
 	uint16_t r,g,b;
	LCD_WR_REG(0X2e);	//9341/6804/5310���Ͷ�GRAMָ��	
 	GPIOB->CRL=0X88888888; //PB0-7  ��������
	GPIOB->CRH=0X88888888; //PB8-15 ��������
	GPIOB->ODR=0X0000ffff;     //ȫ�����
	LCD_RS_SET;
	LCD_CS_CLR;
	//��ȡ����(���Ĵ���ʱ,������Ҫ��2��)
	LCD_RD_CLR;
  	bsp_delay_us(1);//��ʱ1us					   
	LCD_RD_SET;
	//dummy READ
	LCD_RD_CLR;
  	bsp_delay_us(1);//��ʱ1us					   
	r=DATAIN;  
	LCD_RD_SET;
		LCD_RD_CLR;					   
		b=DATAIN;//��ȡ��ɫֵ  	  
	 	LCD_RD_SET;
		g=r&0XFF;//����9341,��һ�ζ�ȡ����RG��ֵ,R��ǰ,G�ں�,��ռ8λ
		g<<=8;
	LCD_CS_SET; 

	GPIOB->CRL=0X33333333; //PB0-7  �������
	GPIOB->CRH=0X33333333; //PB8-15 �������
	GPIOB->ODR=0X0000FFFF;    //ȫ�������
	return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));//ILI9341/NT35310/NT35510��Ҫ��ʽת��һ��
}

//д�Ĵ���
//LCD_Reg:�Ĵ������
//LCD_RegValue:Ҫд���ֵ
void LCD_WriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue);	    		 
} 

//���Ĵ���
//LCD_Reg:�Ĵ������
//����ֵ:������ֵ
uint16_t LCD_ReadReg(uint16_t LCD_Reg)
{										   
 	LCD_WR_REG(LCD_Reg); //д��Ҫ���ļĴ�����    
	return LCD_RD_DATA(); 
} 

//��ʼдGRAM
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
} 

//����LCD���Զ�ɨ�跽��
//ע��:�����������ܻ��ܵ��˺������õ�Ӱ��(������9341/6804����������),
//����,һ������ΪL2R_U2D����,�������Ϊ����ɨ�跽ʽ,���ܵ�����ʾ������.
//dir:0~7,����8������(���嶨���lcd.h)
//����LCD���Զ�ɨ�跽��
//ע��:�����������ܻ��ܵ��˺������õ�Ӱ��(������9341/6804����������),
//����,һ������ΪL2R_U2D����,�������Ϊ����ɨ�跽ʽ,���ܵ�����ʾ������.
//dir:0~7,����8������(���嶨���lcd.h)
//9320/9325/9328/4531/4535/1505/b505/8989/5408/9341/5310/5510��IC�Ѿ�ʵ�ʲ���	   	   
void LCD_Scan_Dir(uint8_t dir)
{
	uint16_t regval=0;
	uint16_t dirreg=0;
	uint16_t temp;  
	switch(dir)
	{
		case L2R_U2D://������,���ϵ���
			regval|=(0<<7)|(0<<6)|(0<<5); 
			break;
		case L2R_D2U://������,���µ���
			regval|=(1<<7)|(0<<6)|(0<<5); 
			break;
		case R2L_U2D://���ҵ���,���ϵ���
			regval|=(0<<7)|(1<<6)|(0<<5); 
			break;
		case R2L_D2U://���ҵ���,���µ���
			regval|=(1<<7)|(1<<6)|(0<<5); 
			break;	 
		case U2D_L2R://���ϵ���,������
			regval|=(0<<7)|(0<<6)|(1<<5); 
			break;
		case U2D_R2L://���ϵ���,���ҵ���
			regval|=(0<<7)|(1<<6)|(1<<5); 
			break;
		case D2U_L2R://���µ���,������
			regval|=(1<<7)|(0<<6)|(1<<5); 
			break;
		case D2U_R2L://���µ���,���ҵ���
			regval|=(1<<7)|(1<<6)|(1<<5); 
			break;	 
	}
	dirreg=0X36;
	regval|=0X08;   	   
	LCD_WriteReg(dirreg,regval);
	if(regval&0X20)
	{
		if(lcddev.width<lcddev.height)//����X,Y
		{
			temp=lcddev.width;
			lcddev.width=lcddev.height;
			lcddev.height=temp;
		}
	}else  
	{
		if(lcddev.width>lcddev.height)//����X,Y
		{
			temp=lcddev.width;
			lcddev.width=lcddev.height;
			lcddev.height=temp;
		}
	}  
	LCD_WR_REG(lcddev.setxcmd); 
	LCD_WR_DATA(0);LCD_WR_DATA(0);
	LCD_WR_DATA((lcddev.width-1)>>8);LCD_WR_DATA((lcddev.width-1)&0XFF);
	LCD_WR_REG(lcddev.setycmd); 
	LCD_WR_DATA(0);LCD_WR_DATA(0);
	LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF);  
}
//���ù��λ��
//Xpos:������
//Ypos:������
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
	LCD_WR_REG(lcddev.setxcmd); 
	LCD_WR_DATA(Xpos>>8); 
	LCD_WR_DATA(Xpos&0XFF);	 
	LCD_WR_REG(lcddev.setycmd); 
	LCD_WR_DATA(Ypos>>8); 
	LCD_WR_DATA(Ypos&0XFF);
}

////����LCD��ʾ����
////dir:0,������1,����
void LCD_Display_Dir(uint8_t dir)
{
	if(dir==0)			//����
	{
		lcddev.dir=0;	//����
		lcddev.width=240;
		lcddev.height=320;
	}else 				//����
	{	  				
		lcddev.dir=1;	//����
		lcddev.width=320;
		lcddev.height=240;
	} 
	lcddev.wramcmd=0X2C;
	lcddev.setxcmd=0X2A;
	lcddev.setycmd=0X2B;  	 
	//DFT_SCAN_DIR//9341����
	//D2U_L2R//9341����
	LCD_Scan_Dir(D2U_L2R);	//Ĭ��ɨ�跽��
}

void TFT_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOB|	//ʹ�ܶ˿�E/Bʱ�Ӻ�AFIOʱ��
							RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);//����SWD��ʧ��JTAG

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure); //E11-15 ->LCD���ƿ�
	GPIO_ResetBits(GPIOE,GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;	
	GPIO_Init(GPIOB, &GPIO_InitStructure); //B ->LCD���ݿ�
	GPIO_SetBits(GPIOB,GPIO_Pin_All);

	//Ӳ����λ  ������ܳ�ʼ��ʧ��
	GPIO_SetBits(GPIOE,GPIO_Pin_15);
	bsp_delay_ms(50);
	GPIO_ResetBits(GPIOE,GPIO_Pin_15);
	bsp_delay_ms(50);
	GPIO_SetBits(GPIOE,GPIO_Pin_15);
	bsp_delay_ms(50);
	
	LCD_WriteReg(0x0000,0x0001);
	bsp_delay_ms(5); // delay 50 ms 
	lcddev.id = LCD_ReadReg(0x0000); 
	if(lcddev.id<0XFF||lcddev.id==0XFFFF||lcddev.id==0X9300)//����ID����ȷ
	{
		//����9341 ID�Ķ�ȡ		
		LCD_WR_REG(0xD3);				   
		LCD_RD_DATA(); 				//dummy read 	
		LCD_RD_DATA();   	    	//����0X00
		lcddev.id=LCD_RD_DATA();   	//��ȡ93								   
		lcddev.id<<=8;
		lcddev.id|=LCD_RD_DATA();  	//��ȡ41 	   			   
	}	

//	��ӡLCD ID��Ϊ��ʶ��LCD�������ͺ���	
// 	printf("LCD ID:%x\r\n",lcddev.id); //��ӡLCD ID  
	{
	LCD_WR_REG(0xCF);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0xC1); 
	LCD_WR_DATA(0X30); 
	LCD_WR_REG(0xED);  
	LCD_WR_DATA(0x64); 
	LCD_WR_DATA(0x03); 
	LCD_WR_DATA(0X12); 
	LCD_WR_DATA(0X81); 
	LCD_WR_REG(0xE8);  
	LCD_WR_DATA(0x85); 
	LCD_WR_DATA(0x10); 
	LCD_WR_DATA(0x7A); 
	LCD_WR_REG(0xCB);  
	LCD_WR_DATA(0x39); 
	LCD_WR_DATA(0x2C); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x34); 
	LCD_WR_DATA(0x02); 
	LCD_WR_REG(0xF7);  
	LCD_WR_DATA(0x20); 
	LCD_WR_REG(0xEA);  
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_REG(0xC0);    //Power control 
	LCD_WR_DATA(0x1B);   //VRH[5:0] 
	LCD_WR_REG(0xC1);    //Power control 
	LCD_WR_DATA(0x01);   //SAP[2:0];BT[3:0] 
	LCD_WR_REG(0xC5);    //VCM control 
	LCD_WR_DATA(0x30); 	 //3F
	LCD_WR_DATA(0x30); 	 //3C
	LCD_WR_REG(0xC7);    //VCM control2 
	LCD_WR_DATA(0XB7); 
	LCD_WR_REG(0x36);    // Memory Access Control 
	LCD_WR_DATA(0x48); 
	LCD_WR_REG(0x3A);   
	LCD_WR_DATA(0x55); 
	LCD_WR_REG(0xB1);   
	LCD_WR_DATA(0x00);   
	LCD_WR_DATA(0x1A); 
	LCD_WR_REG(0xB6);    // Display Function Control 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0xA2); 
	LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
	LCD_WR_DATA(0x00); 
	LCD_WR_REG(0x26);    //Gamma curve selected 
	LCD_WR_DATA(0x01); 
	LCD_WR_REG(0xE0);    //Set Gamma 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x2A); 
	LCD_WR_DATA(0x28); 
	LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0x0E); 
	LCD_WR_DATA(0x08); 
	LCD_WR_DATA(0x54); 
	LCD_WR_DATA(0XA9); 
	LCD_WR_DATA(0x43); 
	LCD_WR_DATA(0x0A); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x00); 		 
	LCD_WR_REG(0XE1);    //Set Gamma 
	LCD_WR_DATA(0x00); 
	LCD_WR_DATA(0x15); 
	LCD_WR_DATA(0x17); 
	LCD_WR_DATA(0x07); 
	LCD_WR_DATA(0x11); 
	LCD_WR_DATA(0x06); 
	LCD_WR_DATA(0x2B); 
	LCD_WR_DATA(0x56); 
	LCD_WR_DATA(0x3C); 
	LCD_WR_DATA(0x05); 
	LCD_WR_DATA(0x10); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_DATA(0x3F); 
	LCD_WR_DATA(0x3F); 
	LCD_WR_DATA(0x0F); 
	LCD_WR_REG(0x2B); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x01);
	LCD_WR_DATA(0x3f);
	LCD_WR_REG(0x2A); 
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0x00);
	LCD_WR_DATA(0xef);	 
	LCD_WR_REG(0x11); //Exit Sleep
	bsp_delay_ms(120);
	LCD_WR_REG(0x29); //display on		
	}
	LCD_Display_Dir(0);//���ú������������������õģ�����
}
//���ٻ���
//x,y:����
//color:��ɫ
void LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{	   
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(x>>8); 
		LCD_WR_DATA(x&0XFF);	 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(y>>8); 
		LCD_WR_DATA(y&0XFF);
	LCD_RS_CLR;
 	LCD_CS_CLR; 
	DATAOUT(lcddev.wramcmd);//дָ��  
	LCD_WR_CLR; 
	LCD_WR_SET; 
 	LCD_CS_SET; 
	LCD_WR_DATA(color);		//д����
}

////��������
////color:Ҫ���������ɫ
//void LCD_Clear(uint16_t color)
//{
//	uint32_t index=0;      
//	uint32_t totalpoint=lcddev.width;
//	totalpoint*=lcddev.height; 			//�õ��ܵ���
//	{
//		LCD_SetCursor(0x00,0x0000);	//���ù��λ�� 	
//	}
//	LCD_WriteRAM_Prepare();     		//��ʼд��GRAM	  	  
//	for(index=0;index<totalpoint;index++)
//		LCD_WR_DATA(color);	
//}

////��ָ��λ����ʾһ���ַ�
////x,y:��ʼ����
////num:Ҫ��ʾ���ַ�:" "--->"~"
////size:�����С 12/16/24
////mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
//void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
//{  							  
//    uint8_t temp,t1,t;
//	uint16_t y0=y;
//	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
//	//���ô���		   
//	num=num-' ';//�õ�ƫ�ƺ��ֵ
//	for(t=0;t<csize;t++)
//	{   
//		if(size==12)temp=asc2_1206[num][t]; 	 	//����1206����
//		else return;								//û�е��ֿ�
//		for(t1=0;t1<8;t1++)
//		{			    
//			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
//			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);
//			temp<<=1;
//			y++;
//			if(x>=lcddev.width)return;		//��������
//			if((y-y0)==size)
//			{
//				y=y0;
//				x++;
//				if(x>=lcddev.width)return;	//��������
//				break;
//			}
//		}  	 
//	}  	
//}   
////m^n����
////����ֵ:m^n�η�.
//uint32_t LCD_Pow(uint8_t m,uint8_t n)
//{
//	uint32_t result=1;	 
//	while(n--)result*=m;    
//	return result;
//}			 
////��ʾ����,��λΪ0,����ʾ
////x,y :�������	 
////len :���ֵ�λ��
////size:�����С
////color:��ɫ 
////num:��ֵ(0~4294967295);	 
//void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
//{         	
//	uint8_t t,temp;
//	uint8_t enshow=0;						   
//	for(t=0;t<len;t++)
//	{
//		temp=(num/LCD_Pow(10,len-t-1))%10;
//		if(enshow==0&&t<(len-1))
//		{
//			if(temp==0)
//			{
//				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
//				continue;
//			}else enshow=1; 
//		 	 
//		}
//	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
//	}
//} 
////��ʾ����,��λΪ0,������ʾ
////x,y:�������
////num:��ֵ(0~999999999);	 
////len:����(��Ҫ��ʾ��λ��)
////size:�����С
////mode:
////[7]:0,�����;1,���0.
////[6:1]:����
////[0]:0,�ǵ�����ʾ;1,������ʾ.
//void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
//{  
//	uint8_t t,temp;
//	uint8_t enshow=0;						   
//	for(t=0;t<len;t++)
//	{
//		temp=(num/LCD_Pow(10,len-t-1))%10;
//		if(enshow==0&&t<(len-1))
//		{
//			if(temp==0)
//			{
//				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
//				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
// 				continue;
//			}else enshow=1; 
//		 	 
//		}
//	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
//	}
//} 
////��ʾ�ַ���
////x,y:�������
////width,height:�����С  
////size:�����С
////*p:�ַ�����ʼ��ַ		  
//void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p)
//{         
//	uint8_t x0=x;
//	width+=x;
//	height+=y;
//    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
//    {       
//        if(x>=width){x=x0;y+=size;}
//        if(y>=height)break;//�˳�
//        LCD_ShowChar(x,y,*p,size,0);
//        x+=size/2;
//        p++;
//    }  
//}




