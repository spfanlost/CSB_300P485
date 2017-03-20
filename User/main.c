/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    22-September-2016
  * @brief   This file provides main program functions
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
// ����汾����
//#define CSB300P232
//#define CSB300P485
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* ���ر������� ---------------------------------------------------------------*/ 
uint16_t     BacklightTime = 0; 
uint16_t     UpdateBatTime = 0;
uint16_t     ShutdownFlag  = 0;
uint16_t     ShutdownCount = 0;

uint16_t     SaveGearsValue = 1;     //��λ
uint16_t	 PowrOffTime    = 10;

/* �ⲿ�������� ---------------------------------------------------------------*/ 

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */ 
int main(void)
{
	/*** �����ڲ��������� ***/
	uint16_t main_temp = 0;
	/*** �����ⲿ�������� ***/
	extern volatile float frequency;
	extern volatile float ultrasonic_sound;
	extern uint8_t AutoGearFlag ; //Ĭ����0
	
	/* Initilize the BSP layer */
	BSP_Init();
	TFT_Init();
	GUI_Init();
	CreatPageStartWin();
	TIM5_PWM_Init(299,0);//����PWM
	BacklightOn();
	MainTask();
	if(WM_IsWindow(WinPara.hWinStatus))//�жϴ����Ƿ���Ч
	{
		WM_SendMessageNoPara(WinPara.hWinStatus,MY_MSG_BATTERY); /* ���µ�ص���������Ϣ */				
		WM_SendMessageNoPara(WinPara.hWinStatus,MY_MSG_GAUTO);		
	}
	IWDG_Init(); //��ʼ�����Ź�6.553sι��
	while(1)
	{
		IWDG_Feed();//6.553s��ι��
		update_ultrasonic();
		update_frequence();
		if(ultrasonic_sound<0.05f)  //��ʱ����ǿΪ��Ĵ���
		{
			frequency = 0;
			ultrasonic_sound=0;
			main_temp++;		//ֻҪ��һ��Ϊ��Ϳ�ʼ���ǲ���һֱΪ��
			if(main_temp >= 5)  //��ǿһֱΪ�㣬�ſ�ʼ����ػ�����
			{
				main_temp = 5;
				if(PowrOffTime != 0)             //��������ǿ������ʱ��Ϊ�� ��ʼ�ػ�����ʱ
				{
					ShutdownFlag = 1;			  //��ʼ�ػ���ʱ
					if(ShutdownCount >= PowrOffTime*60) //ʱ�䵽
					{
						ShutdownFlag = 0;
						ShutdownCount = 0;
						PowerOffSave();
						BacklightOff();
						TIM_Cmd(TIM5, DISABLE);  //�ر���PWM TIM5
						GPIO_ResetBits(GPIOC,GPIO_Pin_2);   //power off		
					}
				}
			}
		}
		else
		{
			main_temp = 0;	//��ǿ��һֱΪ�㣬�Ͳ��ܹػ�
			ShutdownFlag = 0; //ֻҪ��ǿ��Ϊ���һֱ���ܹػ�
			ShutdownCount = 0;
			if(AutoGearFlag == 1) //ˢ�µ�λ��־
			{
				RefreshGear();
			}
		}
		if(WM_IsWindow(WinPara.hWinOne)) //�жϴ����Ƿ���Ч
		{
			WM_SendMessageNoPara(WinPara.hWinOne,MY_MSG_FREQUENCY); /*	������Ϣ */			
			WM_SendMessageNoPara(WinPara.hWinOne,MY_MSG_ULTRASONIC); /*	������Ϣ */			
		}
		if(WM_IsWindow(WinPara.hWinTwo)) //�жϴ����Ƿ���Ч
		{
			WM_SendMessageNoPara(WinPara.hWinTwo,MY_MSG_ULTRASONIC); /*	������Ϣ */			
			WM_SendMessageNoPara(WinPara.hWinTwo,MY_MSG_ADD_GDATA); /*	������Ϣ */			
		}
		if( UpdateBatTime >= 30 )// 30s���µ�ص���
		{
			UpdateBatTime=0;
			if(WM_IsWindow(WinPara.hWinStatus)) 									//�жϴ����Ƿ���Ч
			{
				WM_SendMessageNoPara(WinPara.hWinStatus,MY_MSG_BATTERY); /* ���µ�ص���������Ϣ */				
			}
		}
		if( BacklightTime >= 20 )//	20s����䰵	
		{
			BacklightTime = 20;
			TIM_SetCompare1(TIM5,10);//�豳�����ȣ�0���299����
		}

//	NumFreeBytes = GUI_ALLOC_GetNumFreeBytes();		
//	printf("GUIFreeBytes = %ldK\r\n", NumFreeBytes/1024);
	GUI_Delay(100);		
	}
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
