/*********************************Copyright (c)*********************************                                      
  *                                             
  ******************************************************************************
  * �ļ����ƣ�sys.c
  * ��    �����ļ�
  * �� �� ��: ����
  * ��������: 
  * �� �� ��:             
  * �޸�����: 
  * ��    ��: V1.0.0
  ******************************************************************************
  * attention
  *	ϵͳ���ú���
  ******************************************************************************
  */

#include "sys.h"

//���Ź���ʼ��
void IWDG_Init(void) 
{	
 	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //ʹ�ܶԼĴ���IWDG_PR��IWDG_RLR��д����
	IWDG->PR = IWDG_Prescaler_64;
	IWDG->RLR = 0xfff;	
	IWDG_ReloadCounter();  //����IWDG��װ�ؼĴ�����ֵ��װ��IWDG������
	IWDG_Enable();  //ʹ��IWDG
}
//ι�������Ź�
void IWDG_Feed(void)
{   
 	IWDG_ReloadCounter();//reload										   
}

/*��ֹjtag ��sw�������� */
void JATG_Disable_SW_Enable(void)
{
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	 GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);   
	 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);   //�ر�jtaqģʽ  ��swģʽ  �����Ʒֻ�ṩsw���������Լ��������� 
}
