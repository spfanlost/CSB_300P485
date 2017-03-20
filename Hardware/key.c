/*********************************Copyright (c)*********************************                                      
  *                                             
  ******************************************************************************
  * �ļ����ƣ�key.C
  * ��    �����ļ�
  * �� �� ��: ����
  * ��������: 2016.4.10
  * �� �� ��:             
  * �޸�����: 
  * ��    ��: V1.0.0
  ******************************************************************************
  * updata:  16.4.10
  *	�滻��������ΪFIFO���ܡ�
  *	ʵ�ְ��£����𣬳�������������ϰ����ȹ��ܡ�
  *
  * �޸���Ҫע��
  *	key.h�У�������� KEY_COUNT KEY_FILTER_TIME KEY_LONG_TIME KEY_FIFO_SIZE
  *
  *
  ******************************************************************************
  */

#include "bsp.h"

static KEY_T s_tBtn[KEY_COUNT];
static KEY_FIFO_T s_tKey;		/* ����FIFO����,�ṹ�� */

static void InitKeyVar(void);
static void InitKeyHard(void);
static void bsp_DetectKey(uint8_t i);

void bsp_OnOffKey_init(void) 
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;// ON/OFF��������ȥΪ�����أ�������Ϊ����          
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);      
	
	EXTI_ClearITPendingBit(EXTI_Line1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource1); 
	EXTI_InitStructure.EXTI_Line                         = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode                         = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger                      = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd                      = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel                   = EXTI1_IRQn;			 //�������ⲿ�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;	 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;					
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;							
	NVIC_Init(&NVIC_InitStructure); 		

}
void EXTI1_IRQHandler(void)
{
	static uint32_t power_temp=0;
	extern uint16_t BacklightTime; 
	extern uint16_t ShutdownCount ;              
	extern uint16_t ShutdownFlag;
	GPIO_InitTypeDef GPIO_InitStructure;
	bsp_delay_us(1000);			
	if(EXTI_GetFlagStatus(EXTI_Line1))
	{
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1))	 
		{
			power_temp++;
		}
		else                           //һ�������ſ� ��������жϱ�־�͹ػ�����
		{
			power_temp=0;
			BacklightTime = 0;				//���㱳��䰵��ʱ
			ShutdownFlag = 0;				//����ػ���ʱ		
			ShutdownCount = 0;				//����ػ���ʱ
			BacklightOn();					//����
			EXTI_ClearITPendingBit(EXTI_Line1);		 //���жϱ�־λ
		}
		if(power_temp>2000)
		{
			power_temp=0;
			PowerOffSave();
			BacklightOff();
			TIM_Cmd(TIM5, DISABLE);  //�ر���PWM TIM5
			//��ģ��ϱ������Ͽ��Թص����⣬���Ǿ��ǲ��ܣ�����ֻ�����±߷�����
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
			GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;				
			GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);	    
			GPIO_SetBits(GPIOA,GPIO_Pin_0);//�ص�����
			EXTI_ClearITPendingBit(EXTI_Line1);		 
			GPIO_ResetBits(GPIOC,GPIO_Pin_2);   //power off	
		}
	}
}

void bsp_Power_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_8;				
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;				
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	bsp_delay_ms(600); //ֻҪ��ʼ��  ���߾ͻ��ȱ�����  ����Ҫ��ʱ  ��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	    
	GPIO_SetBits(GPIOA,GPIO_Pin_0);//���ȹص�����
	GPIO_SetBits(GPIOC,GPIO_Pin_2);//����
	//�ٵ�һ��
	GPIO_SetBits(GPIOC,GPIO_Pin_3);
	bsp_delay_ms(500);
	GPIO_ResetBits(GPIOC,GPIO_Pin_3);
	bsp_delay_ms(1);
}
void BacklightOn(void)
{
	TIM_SetCompare1(TIM5,299);//������
}
void BacklightOff(void)
{
	TIM_SetCompare1(TIM5,0);//�ر���
}
/********************************************************************************************************
  * @��������		��ȡÿһ�������ӿں���
  * @����˵��   
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*********************************************************************************************************
*/
static uint8_t IsKeyBack(void)  {if ((GPIOC->IDR & GPIO_Pin_6)     == 0) return 1;else return 0;}
static uint8_t IsKeyOk(void) 	{if ((GPIOC->IDR & GPIO_Pin_7)     == 0) return 1;else return 0;}
static uint8_t IsKeyUp(void) 	{if ((GPIOE->IDR & GPIO_Pin_2)     == 0) return 1;else return 0;}
static uint8_t IsKeyDown(void)  {if ((GPIOE->IDR & GPIO_Pin_3)     == 0) return 1;else return 0;}
static uint8_t IsKeyLeft(void)  {if ((GPIOE->IDR & GPIO_Pin_4)     == 0) return 1;else return 0;}
static uint8_t IsKeyRight(void) {if ((GPIOE->IDR & GPIO_Pin_5)     == 0) return 1;else return 0;}

static uint8_t IsKeyDown9(void) {if (IsKeyBack() && IsKeyDown())           return 1;else return 0;}

/*
*********************************************************************************************************
*	�� �� ��: bsp_Init_Key
*	����˵��: ��ʼ������. �ú����� bsp_Init() ���á�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_Init_Key(void)
{
	InitKeyVar();	/* ��ʼ���������� */
	InitKeyHard();	/* ��ʼ������Ӳ�� */
}

//������ʼ������ 
static void InitKeyHard(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOE,ENABLE);
	////////////////////////////////////////////////---key_back---key_ok
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;       //����Ϊ����      
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);      
	////////////////////////////////////////////////key_up---key_down---key_left---key_right
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
} 

/*
*********************************************************************************************************
*	�� �� ��: bsp_PutKey
*	����˵��: ��1����ֵѹ�밴��FIFO��������������ģ��һ��������
*	��    ��:  _KeyCode : ��������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_PutKey(uint8_t _KeyCode)
{
	s_tKey.Buf[s_tKey.Write] = _KeyCode;

	if (++s_tKey.Write  >= KEY_FIFO_SIZE)
	{
		s_tKey.Write = 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_GetKey
*	����˵��: �Ӱ���FIFO��������ȡһ����ֵ��
*	��    ��:  ��
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
uint8_t bsp_GetKey(void)
{
	uint8_t ret;

	if (s_tKey.Read == s_tKey.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_tKey.Buf[s_tKey.Read];

		if (++s_tKey.Read >= KEY_FIFO_SIZE)
		{
			s_tKey.Read = 0;
		}
		return ret;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_GetKey2
*	����˵��: �Ӱ���FIFO��������ȡһ����ֵ�������Ķ�ָ�롣
*	��    ��:  ��
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
uint8_t bsp_GetKey2(void)
{
	uint8_t ret;

	if (s_tKey.Read2 == s_tKey.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_tKey.Buf[s_tKey.Read2];

		if (++s_tKey.Read2 >= KEY_FIFO_SIZE)
		{
			s_tKey.Read2 = 0;
		}
		return ret;
	}
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_GetKeyState
*	����˵��: ��ȡ������״̬
*	��    ��:  _ucKeyID : ����ID����0��ʼ
*	�� �� ֵ: 1 ��ʾ���£� 0 ��ʾδ����
*********************************************************************************************************
*/
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID)
{
	return s_tBtn[_ucKeyID].State;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_SetKeyParam
*	����˵��: ���ð�������
*	��    �Σ�_ucKeyID : ����ID����0��ʼ
*			_LongTime : �����¼�ʱ��
*			 _RepeatSpeed : �����ٶ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed)
{
	s_tBtn[_ucKeyID].LongTime = _LongTime;			/* ����ʱ�� 0 ��ʾ����ⳤ�����¼� */
	s_tBtn[_ucKeyID].RepeatSpeed = _RepeatSpeed;			/* �����������ٶȣ�0��ʾ��֧������ */
	s_tBtn[_ucKeyID].RepeatCount = 0;						/* ���������� */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_ClearKey
*	����˵��: ��հ���FIFO������
*	��    �Σ���
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
void bsp_ClearKey(void)
{
	s_tKey.Read = s_tKey.Write;
}

/*
*********************************************************************************************************
*	�� �� ��: InitKeyVar
*	����˵��: ��ʼ����������
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitKeyVar(void)
{
	uint8_t i;

	/* �԰���FIFO��дָ������ */
	s_tKey.Read = 0;
	s_tKey.Write = 0;
	s_tKey.Read2 = 0;

	/* ��ÿ�������ṹ���Ա������һ��ȱʡֵ */
	for (i = 0; i < KEY_COUNT; i++)
	{
		s_tBtn[i].LongTime = KEY_LONG_TIME;			/* ����ʱ�� 0 ��ʾ����ⳤ�����¼� */
		s_tBtn[i].Count = KEY_FILTER_TIME / 2;		/* ����������Ϊ�˲�ʱ���һ�� */
		s_tBtn[i].State = 0;							/* ����ȱʡ״̬��0Ϊδ���� */
		//s_tBtn[i].KeyCodeDown = 3 * i + 1;				/* �������µļ�ֵ���� */
		//s_tBtn[i].KeyCodeUp   = 3 * i + 2;				/* ��������ļ�ֵ���� */
		//s_tBtn[i].KeyCodeLong = 3 * i + 3;				/* �������������µļ�ֵ���� */
		s_tBtn[i].RepeatSpeed = 0;						/* �����������ٶȣ�0��ʾ��֧������ */
		s_tBtn[i].RepeatCount = 0;						/* ���������� */
	}

	/* �����Ҫ��������ĳ�������Ĳ����������ڴ˵������¸�ֵ */
	/* ���磬����ϣ������1���³���1����Զ��ط���ͬ��ֵ */
//	s_tBtn[KID_JOY_U].LongTime = 100;
//	s_tBtn[KID_JOY_U].RepeatSpeed = 5;	/* ÿ��50ms�Զ����ͼ�ֵ */

//	s_tBtn[KID_JOY_D].LongTime = 100;
//	s_tBtn[KID_JOY_D].RepeatSpeed = 5;	/* ÿ��50ms�Զ����ͼ�ֵ */

//	s_tBtn[KID_JOY_L].LongTime = 100;
//	s_tBtn[KID_JOY_L].RepeatSpeed = 5;	/* ÿ��50ms�Զ����ͼ�ֵ */

//	s_tBtn[KID_JOY_R].LongTime = 100;
//	s_tBtn[KID_JOY_R].RepeatSpeed = 5;	/* ÿ��50ms�Զ����ͼ�ֵ */
	/* �жϰ������µĺ��� */
	s_tBtn[0].IsKeyDownFunc = IsKeyBack;
	s_tBtn[1].IsKeyDownFunc = IsKeyOk;
	s_tBtn[1].LongTime      = 600;
	s_tBtn[2].IsKeyDownFunc = IsKeyUp;
	s_tBtn[3].IsKeyDownFunc = IsKeyDown;
	s_tBtn[4].IsKeyDownFunc = IsKeyLeft;
	s_tBtn[5].IsKeyDownFunc = IsKeyRight;
//	/* ��ϼ� */
	s_tBtn[6].IsKeyDownFunc = IsKeyDown9;
	s_tBtn[6].LongTime      = 600;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_DetectKey
*	����˵��: ���һ��������������״̬�����뱻�����Եĵ��á�
*	��    ��:  �����ṹ����ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_DetectKey(uint8_t i)
{
	KEY_T *pBtn;
	/*
		���û�г�ʼ�������������򱨴�
		if (s_tBtn[i].IsKeyDownFunc == 0)
		{
			printf("Fault : DetectButton(), s_tBtn[i].IsKeyDownFunc undefine");
		}
	*/
//static KEY_T s_tBtn[KEY_COUNT];
	pBtn = &s_tBtn[i];//��ȡ��Ӧ�����Ľṹ���ַ����������ÿ�����������Լ��Ľṹ��
	if (pBtn->IsKeyDownFunc())
	{//�������ִ�е��ǰ������µĴ���
/*
**********************************************************************************
�������if�����Ҫ�����ڰ����˲�ǰ,��Count����һ����ֵ��ǰ��˵������ʼ����ʱ��
�Ѿ�������Count = KEY_FILTER_TIME/2
**********************************************************************************
*/		
		if (pBtn->Count < KEY_FILTER_TIME)
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
/*
**********************************************************************************
����ʵ��KEY_FILTER_TIMEʱ�䳤�ȵ��ӳ�
**********************************************************************************
*/		
		else if(pBtn->Count < 2 * KEY_FILTER_TIME)
		{
			pBtn->Count++;
		}
		else
		{
/*
**********************************************************************************
���State����������ʵ������ģ�������������ˣ�����ͽ�������Ϊ1�����û�а������
������ֵ�ͻ�һֱ��0���������õ�Ŀ�Ŀ�����Ч�ķ�ֹһ������ĳ��֣����簴��K1��ĳ��
ʱ�̼�⵽�˰����а��£���ô���ͻ�����һ�����˲������������˲��Ĺ����У��������
���µ�״̬��ʧ�ˣ����ʱ��ͻ���뵽����ڶ���else������棬Ȼ�������������ּ���˲�
���˲��������ж����State���������ǰ���û�м�⵽���£�����Ͳ����¼��������
**********************************************************************************
*/			if (pBtn->State == 0)
			{
				pBtn->State = 1;

				/* ���Ͱ�ť���µ���Ϣ */
				bsp_PutKey((uint8_t)(3 * i + 1));
			}

			if (pBtn->LongTime > 0)
			{
				if (pBtn->LongCount < pBtn->LongTime)
				{
					/* ���Ͱ�ť�������µ���Ϣ */
					if (++pBtn->LongCount == pBtn->LongTime)
					{
						/* ��ֵ���밴��FIFO */
						bsp_PutKey((uint8_t)(3 * i + 3));
					}
				}
				else
				{
					if (pBtn->RepeatSpeed > 0)
					{
						if (++pBtn->RepeatCount >= pBtn->RepeatSpeed)
						{
							pBtn->RepeatCount = 0;
							/* ��������ÿ��10ms����1������ */
							bsp_PutKey((uint8_t)(3 * i + 1));
						}
					}
				}
			}
		}
	}//�������ִ�е��ǰ������µĴ���
	else
	{//�������ִ�е��ǰ������ֵĴ�����߰���û�а��µĴ���
/*
**********************************************************************************
������Ҫʵ�ְ������ֵ��˲����
**********************************************************************************
*/		if(pBtn->Count > KEY_FILTER_TIME)
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count != 0)
		{
			pBtn->Count--;
		}
		else
		{
			if (pBtn->State == 1)
			{
				pBtn->State = 0;

				/* ���Ͱ�ť�������Ϣ */
				bsp_PutKey((uint8_t)(3 * i + 2));
			}
		}
		pBtn->LongCount = 0;
		pBtn->RepeatCount = 0;
	}//�������ִ�е��ǰ������ֵĴ�����߰���û�а��µĴ���
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_KeyScan
*	����˵��: ɨ�����а���������������systick�ж������Եĵ���
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_KeyScan(void)
{
	uint8_t i;
	for (i = 0; i < KEY_COUNT; i++)
	{
		bsp_DetectKey(i);
	}
}

/*
*********************************************************************************************************
* �� �� ��: GUI_KeyTask
* ����˵��: �˺�����Ҫ���ڵõ������ļ�ֵ��
* �� �� ֵ: ��
*********************************************************************************************************
*/
extern void TIM5_PWM_Init(u16 arr,u16 psc);
void GUI_KeyTask(void)
{
	uint8_t ucKeyCode;
	extern uint16_t BacklightTime; 
	extern uint16_t ShutdownCount;              
	extern uint16_t ShutdownFlag;
	ucKeyCode = bsp_GetKey();
	if(ucKeyCode != KEY_NONE)
	{
		BacklightTime = 0;					//���㱳��䰵��ʱ
		ShutdownFlag  = 0;					//����ػ���ʱ
		ShutdownCount = 0;
		BacklightOn();
		switch (ucKeyCode)
		{
			case JOY_DOWN_U: /* �ϼ����� */
					GUI_StoreKeyMsg(GUI_KEY_UP, 1);			
			break;
			case JOY_LONG_U: /* �ϼ����������� */
					GUI_SendKeyMsg(GUI_KEY_UP_LONG, 1);
			break;
			case JOY_DOWN_D: /* �¼����� */
					GUI_StoreKeyMsg(GUI_KEY_DOWN, 1);
			break;
			case JOY_DOWN_L: /* ������� */
					GUI_StoreKeyMsg(GUI_KEY_LEFT, 1);
			break;
			case JOY_DOWN_R: /* �Ҽ����� */
					GUI_StoreKeyMsg(GUI_KEY_RIGHT, 1);
			break;
			case KEY_DOWN_K2: /* OK������ */
					GUI_StoreKeyMsg(GUI_KEY_ENTER, 1);
			break;
			case KEY_LONG_K2: /* OK���������� */
					GUI_StoreKeyMsg(GUI_KEY_ENTER_LONG, 1);
			break;
			case KEY_DOWN_K1: /* ESC DOWN ������ */
					GUI_StoreKeyMsg(GUI_KEY_ESCAPE, 1);
			break;
			case KEY_LONG_K1D: /* ESC �������� */
					GUI_StoreKeyMsg(GUI_KEY_ESC_D_LONG, 1);
			break;
			default:/* �����ļ�ֵ������ */
			break;
		}
	}
}


