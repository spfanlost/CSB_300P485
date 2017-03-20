#ifndef __TFT_H
#define __TFT_H		
#include "stm32f10x.h"                  // Device header

//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F//���ɫ
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF//��ɫ
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ

#define R32            0x20
#define R33            0x21
#define R34            0x22


//ɨ�跽����
#define L2R_U2D  0 //������,���ϵ���
#define L2R_D2U  1 //������,���µ���
#define R2L_U2D  2 //���ҵ���,���ϵ���
#define R2L_D2U  3 //���ҵ���,���µ���

#define U2D_L2R  4 //���ϵ���,������
#define U2D_R2L  5 //���ϵ���,���ҵ���
#define D2U_L2R  6 //���µ���,������
#define D2U_R2L  7 //���µ���,���ҵ���

#define DFT_SCAN_DIR  L2R_U2D  //Ĭ�ϵ�ɨ�跽��

#define	LCD_CS_SET  GPIOE->BSRR=1<<11    //Ƭѡ�˿�  		
#define	LCD_RS_SET	GPIOE->BSRR=1<<12    //����/���� 		 
#define	LCD_WR_SET	GPIOE->BSRR=1<<13    //д����			
#define	LCD_RD_SET	GPIOE->BSRR=1<<14    //������			
								    
#define	LCD_CS_CLR  GPIOE->BRR=1<<11     //Ƭѡ�˿�  		
#define	LCD_RS_CLR	GPIOE->BRR=1<<12     //����/����		  
#define	LCD_WR_CLR	GPIOE->BRR=1<<13     //д����			
#define	LCD_RD_CLR	GPIOE->BRR=1<<14     //������			  	

//PB0~15,��Ϊ������
#define DATAOUT(x) GPIOB->ODR=x; //�������
#define DATAIN     GPIOB->IDR;   //��������		 

//LCD��Ҫ������
typedef struct  
{										    
	uint16_t width;			//LCD ���
	uint16_t height;		//LCD �߶�
	uint16_t id;			//LCD ID
	uint8_t  dir;			//���������������ƣ�0��������1��������	
	uint16_t wramcmd;		//��ʼдgramָ��
	uint16_t setxcmd;		//����x����ָ��
	uint16_t setycmd;		//����y����ָ��	 
}_lcd_dev; 	  

extern _lcd_dev lcddev;    
//LCD�Ļ�����ɫ�ͱ���ɫ	   
extern uint16_t  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern uint16_t  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ

void TFT_Init(void);	
void LCD_WriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue);
void LCD_WR_REG(uint16_t data);
void LCD_WR_DATA(uint16_t data);
uint16_t LCD_ReadReg(uint16_t LCD_Reg);
uint16_t LCD_RD_DATA(void);
uint16_t LCD_RD_REG_DATA(void);
void LCD_WriteRAM_Prepare(void);
void LCD_Display_Dir(uint8_t dir);
void LCD_Scan_Dir(uint8_t dir);

void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void LCD_Clear(uint16_t color);
void LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint16_t color);
uint16_t LCD_ReadPoint(uint16_t x,uint16_t y);

//void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode);						//��ʾһ���ַ�
//void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);  						//��ʾһ������
//void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode);				//��ʾ ����
//void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p);		//��ʾһ���ַ���,12/16����

#endif
