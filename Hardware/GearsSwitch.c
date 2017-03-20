
/*********************************Copyright (c)*********************************                                      
  *                                             
  ******************************************************************************
  * �ļ����ƣ�GearsSwitch.C
  * ��    �����ļ�
  * �� �� ��: ����
  * ��������: 
  * �� �� ��:             
  * �޸�����: 
  * ��    ��: V1.0.0
  ******************************************************************************
  * attention
  *	�̵�����λ
  ******************************************************************************
  */
#include "bsp.h"

uint16_t GearsValue = 1;//ȫ�ֱ�����.h�ļ�����

void SetGear(uint8_t GValue);

void bsp_Relay_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
	SetGear(GearsValue);
}
/********************************************************************************************************
  * @��������	���õ�λ����
  * @����˵��   
  * @�������   ��
  * @�������   ��Χ��1��2��3
  * @���ز���   ��
*********************************************************************************************************
*/

void SetGear(uint8_t GValue)
{
	
	switch(GValue)
		{
			case 1:
				GPIO_ResetBits(GPIOA,GPIO_Pin_11);
				GPIO_ResetBits(GPIOA,GPIO_Pin_12);
				break;
			case 2:
				GPIO_ResetBits(GPIOA,GPIO_Pin_12); 
				GPIO_SetBits(GPIOA,GPIO_Pin_11);
				break;
			case 3:
				GPIO_ResetBits(GPIOA,GPIO_Pin_11); 
				GPIO_SetBits(GPIOA,GPIO_Pin_12);
				break;
			case 4:
				break;
			default:break;
		}
}

/********************************************************************************************************
  * @��������	̽�⵱ǰ��λ����
  * @����˵��   
  * @�������   ��
  * @�������   ��Χ��1��2��3
  * @���ز���   ��
*********************************************************************************************************
*/
uint16_t DetectGear(void)
{
	uint16_t gear = 1;
	
	if((0      == GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_11))&&(0 == GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_12)))
	{
	gear       = 1;
	
	}
	else if((1 == GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_11))&&(0 == GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_12)))
	{
	gear       = 2;
	}
	else if((0 == GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_11))&&(1 == GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_12)))
	{
	gear       = 3;
	}
	
	return gear;

}
/********************************************************************************************************
  * @��������		�Զ����õ�λ����
  * @����˵��   
  * @�������   ��
  * @�������   ��Χ��1��2��3
  * @���ز���   ��
*********************************************************************************************************
*/
//�����ݶ���
uint16_t RefreshGear(void)
{
	uint16_t Gear = 0;
	extern uint8_t GearDataFlag;
	extern float   GVotage;
	Gear = DetectGear();
	switch(Gear)	
	{
    	case 1:
			if(GVotage <= 1.187f)GearsValue=2; 
			else GearsValue=1; 
    		break;
    	case 2:
			if(GVotage <= 0.67f) GearsValue=3; 
			else if(GVotage >= 2.9f) GearsValue=1; 
			     else GearsValue=2; 
    		break;
    	case 3:
			if(GVotage >= 1.6f) GearsValue=2; 
			else GearsValue=3; 
			break;
    	default: GearsValue=1; 
    		break;
    }
	if(Gear != GearsValue)
	{
		GearDataFlag	= 1;	
		if(WM_IsWindow(WinPara.hWinTwo)) 	//�жϴ����Ƿ���Ч
		{
			WM_SendMessageNoPara(WinPara.hWinTwo,MY_MSG_ADD_GDATA);		
		}
	}
	SetGear(GearsValue);
	if(WM_IsWindow(WinPara.hWinStatus)) 	//�жϴ����Ƿ���Ч
	{
		WM_SendMessageNoPara(WinPara.hWinStatus,MY_MSG_GAUTO);		
	}
	return GearsValue;
}










