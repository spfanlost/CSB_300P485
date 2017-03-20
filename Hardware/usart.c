
/*********************************Copyright (c)*********************************                                      
  *                                             
  ******************************************************************************
  * �ļ����ƣ�usart.c
  * ��    �����ļ�
  * �� �� ��: ����
  * ��������: 
  * �� �� ��:             
  * �޸�����: 
  * ��    ��: V1.0.0
  ******************************************************************************
  * attention
  * 
  ******************************************************************************
*/
#include "bsp.h"

uint8_t	idx = 0;
FlagStatus RXStart;

uint16_t ReadTimeTabFlag = 0;
uint16_t ReadDataTabFlag = 0;

uint16_t ContTimeNum = 0;
uint16_t ContDataNum = 0;
uint16_t TimeN = 0;

uint16_t HoldReg[100] = {0};
uint8_t HaveMes,Tim_Out,Rcv_Complete,Comu_Busy,Rcv_Num,Send_Num;
uint8_t Rcv_Data,Send_Data;
uint8_t *PointToRcvBuf,*PointToSendBuf;
uint8_t Rcv_Buffer[110],Send_Buffer[110];	//Rcv_Buffer�е�һ��Ԫ�ط��յ���Ŀ���ڶ�Ԫ�ؿ�ʼ����modbusЭ������
uint8_t ModbusDeviceID=1;

extern volatile float frequency;
extern volatile float ultrasonic_sound;
extern uint16_t k_value;				//Ĭ��Kֵ---0��0-1000��
extern uint16_t SaveGearsValue ;        //Ĭ�ϵ�λ---1��1��2��3��4��
extern uint16_t GearsValue ;
extern uint8_t  AutoGearFlag ;
extern uint16_t PowrOffTime ;           //Ĭ�Ϲػ�ʱ��---2{0���رգ���1��10min����2��20min����3��30min��}
extern uint16_t LogoOnOff ;             //
extern uint16_t SaveTimeCnt ;
extern uint16_t ConectFrqcyOnOff;          //��ǿ����ʱ���Ƿ����Ƶ��   Ĭ�ϴ�
extern uint16_t RangeMAX; //RangeMAX�Ͻ�Ϊ�㣡����
extern uint16_t SaveTimeJG;
static void Delay(volatile unsigned int nCount);

//////////////////////////////////////////////////////////////////
 /*
 * ��������TIM4_Configuration()
 * ����  ��MODBUSר�ö�ʱ����TIM4��ʼ�� 
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����
 */
	/*
	3.5���ַ�ʱ�����ֲ�ͬ��֡�������յ��������ַ�֮��ʱ����С��3.5���ַ�
	ʱ��ʱ��Ϊ��ͬһ��֡�ģ�����������3.5���ַ�ʱ������Ϊ�ǲ�ͬ֡��
	��һ��Ĵ���ͨ���У�����1���ַ���Ҫ��1λ��ʼλ��8λ����λ��1λУ��λ(����),
	1λֹͣλ,�ܹ� 1+8+1+1 = 11λ��3.5���ַ����� 3.5 * 11 = 38.5λ��
	���粨������115200,��ô����1λ��ʱ����1000/115200 (ms) ,
	������3.5���ַ�ʱ��ʹ�Լ�� (1000/115200)*38.5(ms)=0.334ms ,����ʱ����Ҫ���ж�ʱ��
	����������9600��3.5���ַ�ʱ��ʹ�Լ�� (1000/9600)*38.5(ms)=4.01ms 
	*/
  //Ԥ��Ƶϵ��=7200-1����ô7200/72M = 0.0001,��ÿ100us����ֵ��1
  //����������115200 �Զ���װ��ֵ=10-1����ô100us x 10 = 1ms,��1ms�ж�һ��	
  //����������9600 �Զ���װ��ֵ=50-1����ô100us x 50 = 5ms,��5ms�ж�һ��	
void TIM4_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructure.TIM_Period        = 50-1;              /* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
	/* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
	TIM_TimeBaseStructure.TIM_Prescaler     = 7200 - 1;           /* ʱ��Ԥ��Ƶ�� 72M/72 */
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;        /* ������Ƶ */
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;  /* ���ϼ���ģʽ */
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);                         /* �������жϱ�־ */
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel                   = TIM4_IRQn;            //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;                    //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;                    //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;               //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM4, ENABLE);                                        /* ������ʱ�� */  
}
#define	RS485_TX_EN() GPIO_SetBits(GPIOD,GPIO_Pin_6)
#define	RS485_RX_EN() GPIO_ResetBits(GPIOD,GPIO_Pin_6)
void bsp_usart_init(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//ʹ��USART1��GPIOA,GPIODʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD, ENABLE);	

	USART_DeInit(USART1);  //��λ����1
	
	GPIO_InitStructure.GPIO_Pin                   = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed                 = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode                  = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	GPIO_InitStructure.GPIO_Pin                   = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode                  = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pin                   = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode                  = GPIO_Mode_Out_PP;//RX485���
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
	NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2 ;                   //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;                    //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;               //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);                                             
	
	USART_InitStructure.USART_BaudRate            = bound;                           //һ������Ϊ9600;
	USART_InitStructure.USART_WordLength          = USART_WordLength_9b;             //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;                //һ��ֹͣλ
	USART_InitStructure.USART_Parity              = USART_Parity_Even;               //��żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //��Ӳ������������
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;	 //�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure);                                        //��ʼ������
	USART_Cmd(USART1, ENABLE);
	
	while((USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET));
	Delay(50);
	RS485_RX_EN();
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                                   //���������ж�
	USART_ClearFlag(USART1, USART_FLAG_TC);     /* �巢����ɱ�־��Transmission Complete flag */

}

//CRCУ�����ò���
/* CRC ��λ�ֽ�ֵ��*/
static uint8_t auchCRCHi[] = {
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,
0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,
0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,
0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,
0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,
0x81,0x40,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,
0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,
0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,
0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x01,0xC0,0x80,0x41,0x00,0xC1,0x81,0x40,0x01,0xC0,
0x80,0x41,0x00,0xC1,0x81,0x40,0x00,0xC1,0x81,0x40,
0x01,0xC0,0x80,0x41,0x01,0xC0,0x80,0x41,0x00,0xC1,
0x81,0x40,0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,
0x00,0xC1,0x81,0x40,0x01,0xC0,0x80,0x41,0x01,0xC0,
0x80,0x41,0x00,0xC1,0x81,0x40
} ;
/* CRC��λ�ֽ�ֵ��*/
static uint8_t auchCRCLo[] = {
0x00,0xC0,0xC1,0x01,0xC3,0x03,0x02,0xC2,0xC6,0x06,
0x07,0xC7,0x05,0xC5,0xC4,0x04,0xCC,0x0C,0x0D,0xCD,
0x0F,0xCF,0xCE,0x0E,0x0A,0xCA,0xCB,0x0B,0xC9,0x09,
0x08,0xC8,0xD8,0x18,0x19,0xD9,0x1B,0xDB,0xDA,0x1A,
0x1E,0xDE,0xDF,0x1F,0xDD,0x1D,0x1C,0xDC,0x14,0xD4,
0xD5,0x15,0xD7,0x17,0x16,0xD6,0xD2,0x12,0x13,0xD3,
0x11,0xD1,0xD0,0x10,0xF0,0x30,0x31,0xF1,0x33,0xF3,
0xF2,0x32,0x36,0xF6,0xF7,0x37,0xF5,0x35,0x34,0xF4,
0x3C,0xFC,0xFD,0x3D,0xFF,0x3F,0x3E,0xFE,0xFA,0x3A,
0x3B,0xFB,0x39,0xF9,0xF8,0x38,0x28,0xE8,0xE9,0x29,
0xEB,0x2B,0x2A,0xEA,0xEE,0x2E,0x2F,0xEF,0x2D,0xED,
0xEC,0x2C,0xE4,0x24,0x25,0xE5,0x27,0xE7,0xE6,0x26,
0x22,0xE2,0xE3,0x23,0xE1,0x21,0x20,0xE0,0xA0,0x60,
0x61,0xA1,0x63,0xA3,0xA2,0x62,0x66,0xA6,0xA7,0x67,
0xA5,0x65,0x64,0xA4,0x6C,0xAC,0xAD,0x6D,0xAF,0x6F,
0x6E,0xAE,0xAA,0x6A,0x6B,0xAB,0x69,0xA9,0xA8,0x68,
0x78,0xB8,0xB9,0x79,0xBB,0x7B,0x7A,0xBA,0xBE,0x7E,
0x7F,0xBF,0x7D,0xBD,0xBC,0x7C,0xB4,0x74,0x75,0xB5,
0x77,0xB7,0xB6,0x76,0x72,0xB2,0xB3,0x73,0xB1,0x71,
0x70,0xB0,0x50,0x90,0x91,0x51,0x93,0x53,0x52,0x92,
0x96,0x56,0x57,0x97,0x55,0x95,0x94,0x54,0x9C,0x5C,
0x5D,0x9D,0x5F,0x9F,0x9E,0x5E,0x5A,0x9A,0x9B,0x5B,
0x99,0x59,0x58,0x98,0x88,0x48,0x49,0x89,0x4B,0x8B,
0x8A,0x4A,0x4E,0x8E,0x8F,0x4F,0x8D,0x4D,0x4C,0x8C,
0x44,0x84,0x85,0x45,0x87,0x47,0x46,0x86,0x82,0x42,
0x43,0x83,0x41,0x81,0x80,0x40
} ;

  /* �������ܣ�CRCУ���ú���
 	�������룺puchMsgg��Ҫ����CRCУ�����Ϣ��usDataLen����Ϣ���ֽ���														    
	������������������CRCУ���롣                                                                                      */
uint16_t CRC16(uint8_t *puchMsgg,uint8_t usDataLen)//puchMsgg��Ҫ����CRCУ�����Ϣ��usDataLen����Ϣ���ֽ��� 
{
    uint8_t uchCRCHi = 0xFF ; /* ��CRC�ֽڳ�ʼ��*/
    uint8_t uchCRCLo = 0xFF ; /* ��CRC �ֽڳ�ʼ��*/
    uint8_t uIndex ; /* CRCѭ���е�����*/
    while (usDataLen--) /* ������Ϣ������*/
    {
    uIndex = uchCRCHi ^ *puchMsgg++ ; /* ����CRC */
    uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
    uchCRCLo = auchCRCLo[uIndex] ;
    }
    return ((uchCRCHi << 8) | uchCRCLo) ;
}

/*	  
 * �������ܣ�����Modbus��ʼ��
 * �������룺IdΪModbusվ�š�														    
 * ����������ޡ� 
 */

void ModInit(uint8_t Id)
{
	//modbus������ʼ��
	PointToRcvBuf=Rcv_Buffer;
	PointToSendBuf=Send_Buffer;
	Send_Num=1;//���͵�����˳���������ĵڼ�������
	ModbusDeviceID=Id;//վ������
	Rcv_Buffer[1]=ModbusDeviceID;
	Send_Buffer[1]=ModbusDeviceID;
	Comu_Busy=0;
	HaveMes=0;
	Rcv_Complete=1;
}

/* �������ܣ�����֡��������1,2,3,6�����������1Ϊ���Ϸ������룬2���Ϸ����ݵ�ַ��3���Ϸ����ݣ�6�ӻ��豸æµ��
�������룺��һ������Mode����ָʾ��һ�����
pointer����ָ���������Ϣ֡�����飨��������ĵ�һ��Ԫ�ض����������Ϣ֡����Ч�ֽڸ�����
�����Ԫ�ذ���ModbusЭ����֯��+
����������ޡ�
*/
void  ErrorHandle(uint8_t Mode,uint8_t *Pointer)
{
		uint16_t SendKey;//Ҫ�������ݵ�У��ֵ
		HaveMes=0;//�����Ϣλ
		TIM_Cmd(TIM4,DISABLE);
		TIM_SetCounter(TIM4,0);
		Rcv_Complete=1;
		Comu_Busy=1;
		Rcv_Buffer[0]=Rcv_Num;
		switch(Mode)
	  {
			case 1:*(Pointer+3)=0x01;//��������
			break;			 
			case 2:*(Pointer+3)=0x02;//�����ַ
			break;
			case 3:*(Pointer+3)=0x03;//��������
			break;
			case 6:*(Pointer+3)=0x06;//���豸æ
			break;
	  }
	  *Pointer=0x05;//����Ĵ�����Ч���ݸ���	
	  *(Pointer+2)|=0x80;//���������λ��һ
	  //д��У����
	  SendKey=CRC16(Pointer+1,*Pointer-2);					
	  //�����������У����װ��������ݻ�����
	  *(Pointer+(*Pointer-1))=(uint8_t)(SendKey>>8);
	  *(Pointer+(*Pointer))=(uint8_t)(SendKey&0x00FF);

		RS485_TX_EN();	
		/*��ʱ35uS,�ȴ�Max3485��DE/RE�����ź��ȶ�*/
		Delay(50);
		/*	USART_IT_TXE ENABLE	*/ 
		USART_ITConfig(USART1,USART_IT_TXE,ENABLE);
		/*	USART_IT_RXNE DISABLE	*/ 
		USART_ITConfig(USART1,USART_IT_RXNE,DISABLE); 															   
}
/* �������ܣ�����Modbus��Ϣ����
�������룺�ޡ�														    
�����������*/
void ModRcv(void)
{
	HaveMes=1;//��ʾ���յ�����Ϣ
	Rcv_Data=USART_ReceiveData(USART1);
	if(Comu_Busy!=1)//�����æ�����Խ�����һ֡��Ϣ
	{
		TIM_Cmd(TIM4, DISABLE);
		TIM_SetCounter(TIM4,0);
		//������ʱ�䳬����3.5���ַ���ͬʱ���ܵ��ֽں��Լ���վ��һ�£�����Ϊ���տ�ʼ
		if((Tim_Out!=0)&&(Rcv_Data==ModbusDeviceID))
		{
			Rcv_Buffer[1]=ModbusDeviceID;
			Send_Buffer[1]=ModbusDeviceID;
			Rcv_Complete=0;//��ʾ����֡���տ�ʼ
			Rcv_Num=0;//�������ݸ�����ʼ��
			Rcv_Num++;//ͬʱ������һ
		}
		else if((Tim_Out!=0)&&(Rcv_Data==0))//�㲥��ַ
		{
			Rcv_Buffer[1]=0;
			Send_Buffer[1]=ModbusDeviceID;
			Rcv_Complete=0;//��ʾ����֡���տ�ʼ
			Rcv_Num=0;//�������ݸ�����ʼ��
			Rcv_Num++;//ͬʱ������һ
		}

		if((0==Tim_Out)&&(0==Rcv_Complete))//������ڽ���һ֡������������
		{
			if(Rcv_Num<100)
			{
				Rcv_Buffer[Rcv_Num+1]=Rcv_Data;//�����ݷ������������
				Rcv_Num++;//ͬʱ������һ	
			}
			else
			{
				Rcv_Complete=1;
				Comu_Busy=1;
				Rcv_Buffer[0]=Rcv_Num;
				*(PointToSendBuf+2)=*(PointToRcvBuf+2);//��ȡ������
				ErrorHandle(6,PointToSendBuf);//��ʾ�������ֽ���(�ӻ��豸æµ)
				Rcv_Num=0;
			}
		}
		Tim_Out=0;
		TIM_Cmd(TIM4, ENABLE);
	}
}
/* �������ܣ�����Modbus��Ϣ����
 * �������룺�ޡ�														    
 * ����������ޡ�
 */																
void ModSend(void)	
{
 	Send_Data=*(PointToSendBuf+Send_Num);
	USART_SendData(USART1,Send_Data);
	Send_Num++;
	if(Send_Num>(*PointToSendBuf))//�����Ѿ����
	{
		Comu_Busy=0;
		*PointToSendBuf=0;
		Rcv_Num=0;
		Send_Num=1;

		/*RS485�ɷ����л�������ʱ����صȴ����ݷ�����ɺ����л�������������ݶ�ʧ*/
		while((USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET));
		RS485_RX_EN();	
		/*��ʱ35uS,�ȴ�Max3485��DE/RE�����ź��ȶ�*/
		Delay(50);
		/*	USART_IT_TXE ENABLE	*/ 
		USART_ITConfig(USART1,USART_IT_TXE,DISABLE);
		/*	USART_IT_RXNE ENABLE	*/ 
		USART_ITConfig(USART1,USART_IT_RXNE,ENABLE); 
	}
}

 /* �������ܣ���ȡ�ӻ�ID
 	�������룺����ָ�룬pointer_1ָ���������������Ϣ֡�����飬
			  pointer_2����ָ���������Ϣ֡�����飨��������ĵ�һ��Ԫ�ض����������Ϣ֡����Ч�ֽڸ�����
			  �����Ԫ�ذ���ModbusЭ����֯��														    
	����������ޡ�
	                                                                                      */
void ReadDeviceID(uint8_t *pointer_1,uint8_t *pointer_2)//pointer_1�������룬pointer_2�������
{
	uint16_t SendKey;//Ҫ�������ݵ�У��ֵ
	if(*(pointer_1)==4)	  //������յ����ֽ�������4��������һ������֡
	{
		if(*(pointer_1+1)==00)	//�Ƿ��ǹ㲥֡
		{
			*(pointer_2)=0x06;//��Ч�ֽڸ���
			*(pointer_2+1)=0x00;//�㲥֡ͷ
			*(pointer_2+2)=0x02;//��Ӧ02���ֽ�
			*(pointer_2+3)=ModbusDeviceID;//ModbusDeviceID //232�汾��Ĭ��ID��1
			*(pointer_2+4)=0x00;//����
			//д��У����						   
			SendKey=CRC16(pointer_2+1,*pointer_2-2);					
			//�����������У����װ��������ݻ�����
			*(pointer_2+(*pointer_2-1))=(uint8_t)(SendKey>>8);
			*(pointer_2+(*pointer_2))=(uint8_t)(SendKey&0x00FF);

			RS485_TX_EN();	
			/*��ʱ35uS,�ȴ�Max3485��DE/RE�����ź��ȶ�*/
			Delay(50);
			/*	USART_IT_TXE ENABLE	*/ 
			USART_ITConfig(USART1,USART_IT_TXE,ENABLE);
			/*	USART_IT_RXNE DISABLE	*/ 
			USART_ITConfig(USART1,USART_IT_RXNE,DISABLE); 
		}
		else
		{
			ErrorHandle(2,pointer_2);//������ʼ��ַ
		}
	}
	else
	{
		Comu_Busy=0;
	}
}




 /* �������ܣ���ȡ���ּĴ���
 	�������룺����ָ�룬pointer_1ָ���������������Ϣ֡�����飬
			  pointer_2����ָ���������Ϣ֡�����飨��������ĵ�һ��Ԫ�ض����������Ϣ֡����Ч�ֽڸ�����
			  �����Ԫ�ذ���ModbusЭ����֯��														    
	����������ޡ�
 */
void ReadHoldingReg(uint8_t *pointer_1,uint8_t *pointer_2)//pointer_1�������룬pointer_2�������
{
	uint16_t Address=0;//����ȡ�Ĵ�����ʼ��ַ��HoldReg[i],iΪ0-99��Ӧ��ַ��0��99��
	uint16_t Num=0;//Ҫ��ȡ�ļĴ�������
	uint16_t SendKey;//Ҫ�������ݵ�У��ֵ
	Address=(uint16_t)(*(pointer_1+3))*256+(*(pointer_1+4));//�ȵõ��Ĵ�����ʼ��ַ
	Num=(uint16_t)(*(pointer_1+5))*256+(*(pointer_1+6));//�ȵõ�Ҫ��ȡ�ļĴ�������
	*(pointer_2+2)=0x03;//�������ֽ�Ϊ������
	if(*(pointer_1)==8)	  //������յ����ֽ�������8��������һ������֡
	{
		if(Address<=100) //ֻҪ��ַС��100�����ǺϷ���ַ
		{
			if(Address+Num<=100&&Num>0) //ֻҪ��ַ����������0С��100�����ǺϷ�����
			{
				
 				//����forѭ��
				uint8_t i;
				uint8_t j;

				*(pointer_2+3)=Num*2;//���ĸ��ֽ�ΪҪ���͵��ֽڸ���
				*(pointer_2)=1+1+1+Num*2+2;//��Ч�ֽڸ������ڴԻ���ַ+������+�ֽڸ���+�Ĵ�����Ϣ+CRCУ��
	
	
				for(i=Address,j=4;i<Address+Num;i++,j+=2)
				{
					*(pointer_2+j)=(uint8_t)(HoldReg[i]>>8);//�ȷŸ�λ
					*(pointer_2+j+1)=(uint8_t)(HoldReg[i]&0x00FF);//�ٷŵ�λ
				}
					
				//д��У����						   
				SendKey=CRC16(pointer_2+1,*pointer_2-2);					
				//�����������У����װ��������ݻ�����
				*(pointer_2+(*pointer_2-1))=(uint8_t)(SendKey>>8);
				*(pointer_2+(*pointer_2))=(uint8_t)(SendKey&0x00FF);
	
				RS485_TX_EN();	
				/*��ʱ35uS,�ȴ�Max3485��DE/RE�����ź��ȶ�*/
				Delay(50);
				/*	USART_IT_TXE ENABLE	*/ 
				USART_ITConfig(USART1,USART_IT_TXE,ENABLE);
				/*	USART_IT_RXNE DISABLE	*/ 
				USART_ITConfig(USART1,USART_IT_RXNE,DISABLE); 
						
			}
			else
			{
				 ErrorHandle(3,pointer_2);//�����ȡ����
			}		
		}
		else
		{
			ErrorHandle(2,pointer_2);//������ʼ��ַ
		}
	}
	else
	{
		Comu_Busy=0;
	}
}
 /* �������ܣ�Ԥ�Ƶ����Ĵ���
 	�������룺����ָ�룬pointer_1ָ���������������Ϣ֡�����飬
			  pointer_2����ָ���������Ϣ֡�����飨��������ĵ�һ��Ԫ�ض����������Ϣ֡����Ч�ֽڸ�����
			  �����Ԫ�ذ���ModbusЭ����֯��														    
	����������ޡ�                                                                                      */

void PresetSingleReg(uint8_t *pointer_1,uint8_t *pointer_2)//pointer_1�������룬pointer_2�������
{
 uint16_t Address=0;//��Ԥ�ƼĴ�������ʼ��ַ��GPIO_X,XΪA��B�����˿ڣ�ÿ���˿�16λ����Ӧ��ַ0����31��
	uint16_t PresetValue=0;//Ԥ����ֵ											
	uint16_t SendKey;//Ҫ�������ݵ�У��ֵ
	Address=(uint16_t)(*(pointer_1+3))*256+(*(pointer_1+4));//�ȵõ��Ĵ�����ַ
	PresetValue=(uint16_t)(*(pointer_1+5))*256+(*(pointer_1+6));//�ȵõ�Ԥ��ֵ
	*(pointer_2+2)=0x06;//�������ֽ�Ϊ������

	if(*(pointer_1)==8)	  //������յ����ֽ�������8��������һ������֡
	{
		if(Address<100) //ֻҪ��ַС��100�����ǺϷ���ַ
		{
		
				*(pointer_2)=1+1+2+2+2;//��Ч�ֽڸ������ڴԻ���ַ+������+�Ĵ�����ַ+�Ĵ�����ֵ+CRCУ��
				*(pointer_2+3)=*(pointer_1+3);//����ֵַд������ļĴ�����
				*(pointer_2+4)=*(pointer_1+4);
				*(pointer_2+5)=*(pointer_1+5);//����ֵд������Ĵ�����
				*(pointer_2+6)=*(pointer_1+6);
				HoldReg[Address]=PresetValue;//��Ԥ��ֵд�뱣�ּĴ���
				//д��У����
				SendKey=CRC16(pointer_2+1,*pointer_2-2);					
				//�����������У����װ��������ݻ�����
				*(pointer_2+(*pointer_2-1))=(uint8_t)(SendKey>>8);
				*(pointer_2+(*pointer_2))=(uint8_t)(SendKey&0x00FF);

				RS485_TX_EN();	
				/*��ʱ35uS,�ȴ�Max3485��DE/RE�����ź��ȶ�*/
				Delay(50);
				/*	USART_IT_TXE ENABLE	*/ 
				USART_ITConfig(USART1,USART_IT_TXE,ENABLE);
				/*	USART_IT_RXNE DISABLE	*/ 
				USART_ITConfig(USART1,USART_IT_RXNE,DISABLE); 
		}
		else
		{
			ErrorHandle(2,pointer_2);//������ʼ��ַ
		}

	}
	else
	{
		Comu_Busy=0;
	}
}
/* �������ܣ�Ԥ�ƶ���Ĵ���
 	�������룺����ָ�룬pointer_1ָ���������������Ϣ֡�����飬
			  pointer_2����ָ���������Ϣ֡�����飨��������ĵ�һ��Ԫ�ض����������Ϣ֡����Ч�ֽڸ�����
			  �����Ԫ�ذ���ModbusЭ����֯��														    
	����������ޡ�                                                                                      */

 void PresetMulReg(uint8_t *pointer_1,uint8_t *pointer_2)//pointer_1�������룬pointer_2�������
 {
 	uint16_t Address=0;//��Ԥ�ƼĴ�������ʼ��ַ��HoldReg[i],iΪ0-99��Ӧ��ַ��0��99��
	uint16_t Num=0;//ҪԤ�ƵļĴ�������
	uint8_t  ByteCount;//Ԥ��ֵ���ֽڸ���
	uint16_t PresetValue=0;//Ԥ����ֵ											
	uint16_t SendKey;//Ҫ�������ݵ�У��ֵ

	
	Address=(uint16_t)(*(pointer_1+3))*256+(*(pointer_1+4));//�ȵõ��Ĵ�����ַ
	Num=(uint16_t)(*(pointer_1+5))*256+(*(pointer_1+6));//�ȵõ���Ԥ�ƼĴ�������
	*(pointer_2+2)=0x10;//�������ֽ�Ϊ������
	ByteCount= *(pointer_1+7);//��ʾ����ֵ���ֽ���

	if((*(pointer_1)==9+ByteCount)&&ByteCount>0&&ByteCount<=200&&ByteCount==(uint8_t)(Num*2))//������յ����ֽ�������Ԥ���ĸ��������������ֽ�����������Χ����һ������֡
	{

		if(Address<100) //ֻҪ��ַС��100�����ǺϷ���ַ
		{
		
			if(Address+Num<=100&&Num>0) //ֻҪ��ַ����������0С��100�����ǺϷ�����
			{
				//����forѭ��
				uint8_t i;
				uint8_t j;
					
				*(pointer_2)=1+1+2+2+2;//��Ч�ֽڸ������ڴԻ���ַ+������+�Ĵ�����ַ+�Ĵ�������+CRCУ��
				*(pointer_2+3)=*(pointer_1+3);//����ֵַд������ļĴ�����
				*(pointer_2+4)=*(pointer_1+4);
				*(pointer_2+5)=*(pointer_1+5);//������д������Ĵ�����
				*(pointer_2+6)=*(pointer_1+6);
	
				for(i=0,j=0;i<Num;i++,j+=2)		 	
				{
					PresetValue=(uint16_t)(*(pointer_1+8+j))*256+(*(pointer_1+9+j));//�ȵõ�Ԥ��ֵ
				 	HoldReg[Address+i]=PresetValue;//��Ԥ��ֵд�뱣�ּĴ���
				}
				
	
				//д��У����
				SendKey=CRC16(pointer_2+1,*pointer_2-2);					
				//�����������У����װ��������ݻ�����
				*(pointer_2+(*pointer_2-1))=(uint8_t)(SendKey>>8);
				*(pointer_2+(*pointer_2))=(uint8_t)(SendKey&0x00FF);
	
				RS485_TX_EN();	
				/*��ʱ35uS,�ȴ�Max3485��DE/RE�����ź��ȶ�*/
				Delay(50);
				/*	USART_IT_TXE ENABLE	*/ 
				USART_ITConfig(USART1,USART_IT_TXE,ENABLE);
				/*	USART_IT_RXNE DISABLE	*/ 
				USART_ITConfig(USART1,USART_IT_RXNE,DISABLE); 
	
			}
			else
			{
				 ErrorHandle(3,pointer_2);//�����ȡ����
			}
	
		}
		else
		{
			ErrorHandle(2,pointer_2);//������ʼ��ַ
		}
	}
	else
	{
		Comu_Busy=0;
	}
}


 /* �������ܣ���ȡ����ʱ�������
 	�������룺����ָ�룬pointer_1ָ���������������Ϣ֡�����飬
			  pointer_2����ָ���������Ϣ֡�����飨��������ĵ�һ��Ԫ�ض����������Ϣ֡����Ч�ֽڸ�����
			  �����Ԫ�ذ���ModbusЭ����֯��														    
	����������ޡ�
	                                                                                      */
void ReadAllTimeTab(uint8_t *pointer_1,uint8_t *pointer_2)//pointer_1�������룬pointer_2�������
{
	uint16_t SendKey;//Ҫ�������ݵ�У��ֵ
	uint16_t *pTabTemp = 0;
	uint16_t RecordWinCntNum=0;
	*(pointer_2+2)=0x41;//�������ֽ�Ϊ������
	if(*(pointer_1)==4)	  //������յ����ֽ�������4��������һ������֡
	{
			//����forѭ��
			uint8_t i=0;
			uint8_t j=0;
			RecordWinCntNum = 0;
			for(i = 0; i < 25; i++)
			{
				if(Get_TimeTab_num(i) != 0)
				{
					RecordWinCntNum ++;
				}
			}	
			if(RecordWinCntNum !=0 )
			{
					*(pointer_2)  =1+1+RecordWinCntNum*7+2;//��Ч�ֽڸ������ڴԻ���ַ+������+һ��ʱ���+CRCУ��
					for(i=0,j=3;i<RecordWinCntNum;i++,j+=7)//i=Address,j=4;i<Address+Num;i++,j+=2
					{
						pTabTemp= Get_TimeTab_Time(i);
						*(pointer_2+j+0)=(uint8_t)(pTabTemp[0]>>8);//�ȷŸ�λ
						*(pointer_2+j+1)=(uint8_t)(pTabTemp[0]&0x00FF);//�ٷŵ�λ
						*(pointer_2+j+2)=(uint8_t)(pTabTemp[1]>>8);//�ȷŸ�λ
						*(pointer_2+j+3)=(uint8_t)(pTabTemp[1]&0x00FF);//�ٷŵ�λ
						*(pointer_2+j+4)=(uint8_t)(pTabTemp[2]>>8);//�ȷŸ�λ
						*(pointer_2+j+5)=(uint8_t)(pTabTemp[2]&0x00FF);//�ٷŵ�λ
						*(pointer_2+j+6)=(uint8_t)(pTabTemp[3]&0x00FF);//���ݸ����ŵ�λ
					}
					//д��У����						   
					SendKey=CRC16(pointer_2+1,*pointer_2-2);					
					//�����������У����װ��������ݻ�����
					*(pointer_2+(*pointer_2-1))=(uint8_t)(SendKey>>8);
					*(pointer_2+(*pointer_2))=(uint8_t)(SendKey&0x00FF);

					RS485_TX_EN();	
					/*��ʱ35uS,�ȴ�Max3485��DE/RE�����ź��ȶ�*/
					//Delay(50);
					/*	USART_IT_TXE ENABLE	*/ 
					USART_ITConfig(USART1,USART_IT_TXE,ENABLE);
					/*	USART_IT_RXNE DISABLE	*/ 
					USART_ITConfig(USART1,USART_IT_RXNE,DISABLE); 
			
			}
			else
			{
					ErrorHandle(3,pointer_2);//�����ȡ����
			}
	}
	else
	{
		Comu_Busy=0;
	}
}
 /* �������ܣ�����ȽϺ���
 	�������룺����ָ�룬
				pA������1��
			  pBָ������2��
			  num�����ݸ�����
	���������1����1��������2
	         2����1����������2
 */
uint8_t MyStrCmp(uint8_t *pA,uint16_t *pB)
{
	uint8_t *u8pB = 0;
	u8pB = (uint8_t *)pB;
	if(*pA == *(u8pB+1))
	{
		if(*(pA+1) == *u8pB)
		{
			if(*(pA+2) == *(u8pB+3))
			{
				if(*(pA+3) == *(u8pB+2))
				{
					if(*(pA+4) == *(u8pB+5))
					{
						if(*(pA+5) == *(u8pB+4))
						{
							return 0;
						}else return 1;
					}else return 1;
				}else return 1;
			}else return 1;
		}else return 1;
	}else return 1;
}
 /* �������ܣ���ȡĳʱ�������
 	�������룺����ָ�룬pointer_1ָ���������������Ϣ֡�����飬
			  pointer_2����ָ���������Ϣ֡�����飨��������ĵ�һ��Ԫ�ض����������Ϣ֡����Ч�ֽڸ�����
			  �����Ԫ�ذ���ModbusЭ����֯��														    
	����������ޡ�
 */
void ReadOneDataTab(uint8_t *pointer_1,uint8_t *pointer_2)//pointer_1�������룬pointer_2�������
{
	uint16_t SendKey;//Ҫ�������ݵ�У��ֵ
	uint16_t *pTabTemp = 0;
	uint16_t   Num_Temp_bf = 0;
//	uint8_t  *u8pTabTemp = 0;
	uint16_t Num=0;//Ҫ��ȡ�ļĴ�������
	uint8_t x=16;
	//����forѭ��
	uint8_t i=0;
	uint8_t j=0;
	extern uint16_t RecordWinCntNum;
	extern uint16_t SaveTimeJGTab[25];
	*(pointer_2+2)=0x42;//�������ֽ�Ϊ������
	if(*(pointer_1)==10)	  //������յ����ֽ������Ǹ�������һ������֡
	{
		for(i=0;i<25;i++)
		{
			pTabTemp = Get_TimeTab_Time(i);
			if(0==MyStrCmp(pointer_1+3,pTabTemp))
			{
				x = i;
				break;
			}
		}
		if(x != 16)
		{
			Num = Get_TimeTab_num(x);
			*(pointer_2)  = 10+Num*4+2;//��Ч�ֽڸ������ڴԻ���ַ+������+һ��ʱ���+CRCУ��
			pTabTemp= Get_TimeTab_Time(x);
			*(pointer_2+3)  =(uint8_t)(pTabTemp[0]>>8);//year
			*(pointer_2+4)  =(uint8_t)(pTabTemp[0]&0x00FF);//month
			*(pointer_2+5)  =(uint8_t)(pTabTemp[1]>>8);//date
			*(pointer_2+6)  =(uint8_t)(pTabTemp[1]&0x00FF);//hour
			*(pointer_2+7)  =(uint8_t)(pTabTemp[2]>>8);//min
			*(pointer_2+8)  =(uint8_t)(pTabTemp[2]&0x00FF);//serc
			*(pointer_2+9)  =SaveTimeJGTab[x];//x#ʱ��������ʱ����
			*(pointer_2+10) =Num;//x#ʱ�������ݸ���
			pTabTemp = NULL;
			
			Num_Temp_bf = 0;
			//���ʱ���֮ǰ�ж��ٸ�����
			for(i = 0; i < x; i++)
			{
				Num_Temp_bf += Get_TimeTab_num(i);
			}
			for(i=0,j=11;i<Num;i++,j+=4)//i=Address,j=4;i<Address+Num;i++,j+=2
			{
				pTabTemp= Get_DataTab(i + Num_Temp_bf);
				*(pointer_2+j+0)=(uint8_t)(pTabTemp[0]>>8);//0#ultrasonic_sound*100H
				*(pointer_2+j+1)=(uint8_t)(pTabTemp[0]&0x00FF);//0#ultrasonic_sound*100L
				*(pointer_2+j+2)=(uint8_t)(pTabTemp[1]>>8);//0#frequency*100(40KHz)H
				*(pointer_2+j+3)=(uint8_t)(pTabTemp[1]&0x00FF);//0#frequency*100(40KHz)L
			}
			//д��У����						   
			SendKey=CRC16(pointer_2+1,*pointer_2-2);					
			//�����������У����װ��������ݻ�����
			*(pointer_2+(*pointer_2-1))=(uint8_t)(SendKey>>8);
			*(pointer_2+(*pointer_2))=(uint8_t)(SendKey&0x00FF);

			RS485_TX_EN();	
			/*��ʱ35uS,�ȴ�Max3485��DE/RE�����ź��ȶ�*/
			Delay(50);
			/*	USART_IT_TXE ENABLE	*/ 
			USART_ITConfig(USART1,USART_IT_TXE,ENABLE);
			/*	USART_IT_RXNE DISABLE	*/ 
			USART_ITConfig(USART1,USART_IT_RXNE,DISABLE); 		
		}	else 
		{
				ErrorHandle(3,pointer_2);//�����ȡ����
		}
	}
	else
	{
		Comu_Busy=0;
	}
}
/* �������ܣ����������Ϣ֡���д������չ����벻ͬ�����ò�ͬ�ĺ�������
 	�������룺����ָ�룬pointer_1ָ���������������Ϣ֡�����飬
			  pointer_2����ָ���������Ϣ֡�����飨��������ĵ�һ��Ԫ�ض����������Ϣ֡����Ч�ֽڸ�����
			  �����Ԫ�ذ���ModbusЭ����֯��														    
	����������ޡ�                                                                                      */
void MessageHandle(uint8_t *pointer_in,uint8_t *pointer_out)		  
{
//	uint16_t* pDataTab;
//	uint16_t* pTimeTab;
	uint16_t year=0;
	uint8_t mon=0,day=0,hour=0,min=0,sec=0;
 	uint16_t CalKey;//���������У��ֵ
	uint16_t RcvKey;//���յ���У��ֵ
	uint16_t Address=0;
	HaveMes=0;//�����Ϣλ
	//��ȡ���յ���У��ֵ
	RcvKey=(uint16_t)*(pointer_in+(*pointer_in-1));
	RcvKey=RcvKey<<8;
	RcvKey=RcvKey|(uint16_t)*(pointer_in+(*pointer_in));

	CalKey=CRC16(pointer_in+1,*pointer_in-2);
	if(CalKey==RcvKey)
	{
		switch(*(pointer_in+2))//�ڶ����ֽ�Ϊ������
		{
			case 0x03:
			{
				HoldReg[0]=k_value;
				HoldReg[1]=RangeMAX;
				HoldReg[2]=SaveGearsValue;					
				HoldReg[3]=ConectFrqcyOnOff;
				HoldReg[4]=SaveTimeJG;
				HoldReg[5]=PowrOffTime;
//				HoldReg[6]=;
//				HoldReg[7]=;
				HoldReg[8]=calendar.w_year;					
				HoldReg[9]=calendar.w_month;					
				HoldReg[10]=calendar.w_date;					
				HoldReg[11]=calendar.hour;					
				HoldReg[12]=calendar.min;					
				HoldReg[13]=calendar.sec;
				HoldReg[14]=(uint16_t)(ultrasonic_sound*100);					
				HoldReg[15]=(uint16_t)(frequency*100);					
				
				ReadHoldingReg(pointer_in,pointer_out);//�����ּĴ���
				break;
			}

			case 0x06:
			{
				PresetSingleReg(pointer_in,pointer_out);//Ԥ�Ƶ����Ĵ���
				Address=(uint16_t)(*(pointer_in+3))*256+(*(pointer_in+4));//�ȵõ��Ĵ�����ַ

				switch	(Address)
				{
					case 0:
					{
						k_value=HoldReg[0];
						if(k_value>=1001)
							{
								k_value = 100;				//Ĭ��Kֵ
								bsp_WriteCpuFlash(SAVE_ADDR_k_value,(uint16_t *)&k_value, 1);
							}								
					}
					break;
					case 1:
					{
						RangeMAX=HoldReg[1];
						bsp_WriteCpuFlash(SAVE_ADDR_RangeMAX, (uint16_t *)&RangeMAX,1);							
					}
					break;
////////////////////////////////////////////////////////////////////////////////////////////////////
					case 2:
					{
						SaveGearsValue=HoldReg[2];
						if(SaveGearsValue >= 5)
						{
							SaveGearsValue  = 1;        //Ĭ�ϵ�λ
						}
						else if(SaveGearsValue <= 0)
						{
							SaveGearsValue  = 1;        //Ĭ�ϵ�λ
						}
						else if(SaveGearsValue == 4)	//�Զ���
						{
							AutoGearFlag = 1;
							
							if(WM_IsWindow(WinPara.hWinStatus)) 	//�жϴ����Ƿ���Ч
							{
								WM_SendMessageNoPara(WinPara.hWinStatus,MY_MSG_GAUTO);		
							}
							
						}
						else 
						{
							AutoGearFlag = 0;
							GearsValue = SaveGearsValue;
							SetGear(GearsValue);
							
							if(WM_IsWindow(WinPara.hWinStatus)) 	//�жϴ����Ƿ���Ч
							{
								WM_SendMessageNoPara(WinPara.hWinStatus,MY_MSG_GAUTO);		
							}
						}
						bsp_WriteCpuFlash(SAVE_ADDR_SaveGearsValue, (uint16_t *)&SaveGearsValue,1);		
					
					}
					break;

					case 3:
					{
						ConectFrqcyOnOff=HoldReg[3];
						bsp_WriteCpuFlash(SAVE_ADDR_ConectFrqcyOnOff, (uint16_t *)&ConectFrqcyOnOff,1);							
					}
					break;

					case 4:
					{
						SaveTimeJG=HoldReg[4];
						bsp_WriteCpuFlash(SAVE_ADDR_SaveTimeJG, (uint16_t *)&SaveTimeJG,1);	
					}
					break;

					case 5:
					{
						PowrOffTime=HoldReg[5];	
						bsp_WriteCpuFlash(SAVE_ADDR_PowrOffTime, (uint16_t *)&PowrOffTime,1);							
					}
					
					break;
////////////////////////////////////////////////////////////////////////////////////////////////////
					case 8:
					year = HoldReg[8];
					RTC_Set(year,mon,day,hour,min,sec); //����ʱ��
					break;
					case 9:
					mon  = HoldReg[9];					
					RTC_Set(year,mon,day,hour,min,sec); //����ʱ��
					break;
					case 10:
					day  = HoldReg[10];					
					RTC_Set(year,mon,day,hour,min,sec); //����ʱ��
					break;
					case 11:
					hour = HoldReg[11];					
					RTC_Set(year,mon,day,hour,min,sec); //����ʱ��
					break;
					case 12:
					min  = HoldReg[12];					
					RTC_Set(year,mon,day,hour,min,sec); //����ʱ��
					break;
					case 13:
					sec  = HoldReg[13];					
					RTC_Set(year,mon,day,hour,min,sec); //����ʱ��
					break;					
					default: 
					break;
				}
			}
			break;
			case 0x10:
			{
				PresetMulReg(pointer_in,pointer_out);//Ԥ�ƶ���Ĵ���
				year = HoldReg[8];
				mon  = HoldReg[9];
				day  = HoldReg[10];
				hour = HoldReg[11];
				min  = HoldReg[12];
				sec  = HoldReg[13];
				RTC_Set(year,mon,day,hour,min,sec); //����ʱ��
				
				break;
			}												 
			case 0x11://���豸ʶ����
			{
				ReadDeviceID(pointer_in,pointer_out);
				break;
			}												 
			case 0x41:
			{
				ReadAllTimeTab(pointer_in,pointer_out);
				break;
			}												 
			case 0x42:
			{
				ReadOneDataTab(pointer_in,pointer_out);
				break;
			}												 
			default: 
			{
				*(pointer_out+2)=*(pointer_in+2);//��ȡ������
				ErrorHandle(1,pointer_out);//��������� 
			}	
				break;
		}
	}
	else
	{
		Comu_Busy=0;
	} 		
}
 /* �������ܣ�Modbusר�ö�ʱ����TIM4��
 	�������룺�ޡ�														    
	����������ޡ�		
*/
void TIM4_IRQHandler(void)															  
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ
		Tim_Out=1;
		TIM_Cmd(TIM4,DISABLE);
		TIM_SetCounter(TIM4,0);
		Rcv_Complete=1;
		Rcv_Buffer[0]=Rcv_Num;
		
		if(HaveMes!=0&&Rcv_Num>3)
//		if(HaveMes!=0)
		{
		Comu_Busy=1;
		HaveMes=0;////��Ϣλ��Ϊ0����ʾ��������һ֡���ݣ��������ݵ�����7-19 14:49�޸ģ�
		PointToRcvBuf=Rcv_Buffer;
		PointToSendBuf=Send_Buffer;
//		if(*(PointToRcvBuf+1)==0)
//		ReadDeviceID(PointToRcvBuf,PointToSendBuf);
//		else
		MessageHandle(PointToRcvBuf,PointToSendBuf);
		}
	}	
}
void USART1_IRQHandler()
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		ModRcv();//����modbus��Ϣ����
	}
	if((USART_GetITStatus(USART1,USART_IT_TXE)!=RESET))
	{
		USART_ClearITPendingBit(USART1,USART_IT_TXE);
		ModSend();//����modbus��Ϣ����
	}
}
void Delay(volatile unsigned int nCount)
{
   for(; nCount != 0; nCount--);
}

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET){}; 
    USART_SendData(USART1,(uint8_t)ch);   
	return ch;
}
//__use_no_semihosting was requested, but _ttywrch was referenced, �������º���, ����2
void _ttywrch(int ch)
{
ch = ch;
}
#endif 



