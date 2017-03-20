/*********************************Copyright (c)*********************************                                      
  *                                             
  ******************************************************************************
  * �ļ����ƣ�pwm.c
  * ��    �����ļ�
  * �� �� ��: ����
  * ��������: 
  * �� �� ��:             
  * �޸�����: 
  * ��    ��: V1.0.0
  ******************************************************************************
  * attention
  *	��ʱ��API
  ******************************************************************************
*/

#include "bsp.h"
//Һ��������Ƴ�ʼ��
void TIM5_PWM_Init(uint16_t arr,uint16_t psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	//���ø�����Ϊ�����������,���TIM5 CH1��PWM���岨��	GPIOA.0
	GPIO_InitStructure.GPIO_Pin             = GPIO_Pin_0; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode            = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed           = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
	//��ʼ��TIM5
	TIM_TimeBaseStructure.TIM_Period        = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler     = psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	//��ʼ�� PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode          = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState     = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity      = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM5, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM OC1
	TIM_CtrlPWMOutputs(TIM5,ENABLE);	//MOE �����ʹ��	
	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��
	TIM_ARRPreloadConfig(TIM5, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM5, ENABLE);  //ʹ�ܱ���PWM TIM5
}

////ͨ�ö�ʱ���жϳ�ʼ��
////����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
////arr���Զ���װֵ��
////psc��ʱ��Ԥ��Ƶ��
//void TIM2_Int_Init(uint16_t number,uint16_t psc)
//{
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
//	TIM_DeInit(TIM2);
//	TIM_TimeBaseStructure.TIM_Period                     = number; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
//	TIM_TimeBaseStructure.TIM_Prescaler                  = psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  
//	TIM_TimeBaseStructure.TIM_ClockDivision              = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode                = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
//	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
//	NVIC_InitStructure.NVIC_IRQChannel                   = TIM2_IRQn;            //TIM2�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;					 //��ռ���ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;         			 //�����ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;               //IRQͨ����ʹ��
//	NVIC_Init(&NVIC_InitStructure);
//	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM	
//}
////���Զ�ʱ��ÿms�ж�һ��
//void TIM2_IRQHandler(void)   //TIM3�ж�
//{	
//	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
//	{
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIMx�����жϱ�־ 
//	}
//}

