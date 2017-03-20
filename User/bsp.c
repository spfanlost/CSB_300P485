/**
  ******************************************************************************
  * @file    bsp.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    22-September-2016
  * @brief   This file provides targets hardware configuration 
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

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
* @brief  Initializes the target hardware.
* @param  None
* @retval None
*/
uint32_t BSP_Init (void)
{
	extern uint16_t ModbusDeviceID;
	/* Enable the CRC Module */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);//STemWin�Ŀ���Ҫʹ��CRC
	JATG_Disable_SW_Enable();//��sw����ģʽ  jtag��ֹ���أ�
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	//����NVIC�жϷ���4:16����ռ���ȼ����������ȼ�
	bsp_delay_init();
	bsp_usart_init(115200);  //���ڳ�ʼ��
	ADC1_Channel_1_init();   //Ƶ�ʵ�ѹ�ɼ�
	ADC2_Channel_2_init();   //��ǿ��ѹad���� 
	ADC2_Channel_10_init();  //��ص�ѹad����
	bsp_Init_Key();          //��Ĥ������ʼ��
	Read_Stored_Data();      //��ȡ�洢���ڲ�����
	bsp_Relay_Init();        //�̵�����ʼ��
	while(RTC_Init());       //RTC��ʼ��,һ��Ҫ��ʼ���ɹ�
	bsp_Power_init();        //������ʼ��
	SaveTabRead();           //��ȡ�洢�ļ�¼����
	bsp_OnOffKey_init();     //����������ʼ��	
	TIM4_Configuration();	 //MODBUSר�ö�ʱ��
	ModInit(ModbusDeviceID);          //MODBUS����

	return 0;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
