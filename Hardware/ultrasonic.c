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
  * �����������API
  ******************************************************************************
*/

#include "bsp.h"
#include "stm32_dsp.h"
#include <math.h>

#define NPT 1024              //FFT����

/* ���ر������� ---------------------------------------------------------*/ 
volatile float   ultrasonic_sound        = 0.00f;//ȫ�ֱ���
volatile float   ultrasonic_sound_max    = 0.00f;//ȫ�ֱ���
volatile float   frequency               = 0.00f;//ȫ�ֱ���

static long       lBUFMAG[NPT+NPT/2];			   //�洢��ģ�������
static long       lBUFOUT[NPT];                    //FFT�������
static long       lBUFIN[NPT];                     //FFT����ϵ��

static uint16_t   frequency_ad[1024] = {0};
static uint16_t   ultrasonic_ad[100] = {0};

static float ultrasonic_last[4] = {0.0f};
static float Ultra_PowerTemp = 0.0f;

float	          GVotage            = 0.0f;		//������ǿ��ѹ�������㵵λ
uint16_t     	  ConectFrqcyOnOff   = 1;           //��ǿ����ʱ���Ƿ����Ƶ�� Ĭ�ϴ� //ȫ�ֱ���
uint16_t          RangeMAX = 2540;                   //RangeMAX�Ͻ�Ϊ�㣡����//ȫ�ֱ���
uint16_t          k_value = 100;                    //  //ȫ�ֱ���

/* �ⲿ�������� ---------------------------------------------------------*/ 
extern uint16_t GearsValue;

//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//������ģʽ->�洢��/16λ���ݿ��/�洢������ģʽ
/**
 * [myDMA_Config DMA1�ĸ�ͨ������]
 * @param DMA_CHx     [DMAͨ��CHx]
 * @param peri_add    [�����ַ]
 * @param memery_add  [�洢����ַ]
 * @param data_number [���ݴ�����]
 */
void myDMA_Config(DMA_Channel_TypeDef* DMA_CHx,uint32_t peri_add,uint32_t memery_add,uint16_t data_number)
{	  
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMAʱ��
	DMA_DeInit(DMA_CHx);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	DMA_InitStructure.DMA_PeripheralBaseAddr = peri_add;  //DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr     = memery_add;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;  //���ݴ��䷽�򣬴����跢�͵��ڴ�  DMA_CCRXλ4
	DMA_InitStructure.DMA_BufferSize         = data_number;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //�������ݿ��Ϊ16λ
	DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord; //�ڴ����ݿ��Ϊ16λ
	DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;  //������ѭ������ģʽ
	DMA_InitStructure.DMA_Priority           = DMA_Priority_High; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA_CHx, &DMA_InitStructure); 
	DMA_Cmd(DMA1_Channel1, DISABLE);        //����DMAͨ��  	
} 
//����һ��DMA����
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx, uint16_t DMA1_MEM_LEN)
{ 
	DMA_Cmd(DMA_CHx, DISABLE );  //�ر�ADC1 DMA1 ��ָʾ��ͨ��      
	DMA_SetCurrDataCounter(DMA_CHx,DMA1_MEM_LEN);//DMAͨ����DMA����Ĵ�С
	DMA_Cmd(DMA_CHx, ENABLE);  //ʹ��ADC1 DMA1 ��ָʾ��ͨ�� 
}

/*******************************************************************************
* ��������: quicksort()
* ��������: �����㷨
* �������: left��right��Ҫ������������Ҷ˵� ��a[n]����quicksort(0,n-1); 
* ���ز���: ��
********************************************************************************/
static void quicksort(int left,int right) 
{
    int i,j;
	uint32_t t,temp; 
    if(left>right) 
       return; 	
    temp=ultrasonic_ad[left]; //temp�д�ľ��ǻ�׼�� 
    i=left; 
    j=right; 
    while(i!=j) 
    { 
       //˳�����Ҫ��Ҫ�ȴ��ұ߿�ʼ�� 
       while(ultrasonic_ad[j]>=temp && i<j) 
                j--; 
       //�����ұߵ� 
       while(ultrasonic_ad[i]<=temp && i<j) 
                i++; 
       //�����������������е�λ�� 
       if(i<j) 
       { 
                t=ultrasonic_ad[i]; 
                ultrasonic_ad[i]=ultrasonic_ad[j]; 
                ultrasonic_ad[j]=t; 
       } 
    } 
    //���ս���׼����λ 
    ultrasonic_ad[left]=ultrasonic_ad[i]; 
    ultrasonic_ad[i]=temp; 
                             
    quicksort(left,i-1);//����������ߵģ�������һ���ݹ�Ĺ��� 
    quicksort(i+1,right);//���������ұߵ� ��������һ���ݹ�Ĺ��� 
} 


/*******************************************************************************
* ��������: read_ultrasonic_once()
* ��������: ��ȡһ����ǿ����
* �������: 
* ���ز���: ��
********************************************************************************/
/**
***		��λ1ʵ����������
***		
***		�ⲿ�����ѹ		0 ---------- 5 ------------ 25 ---------- >50
***		
***		AD�����ѹ		0 ---------- 0 ---------- 1.187 --------- 3
***		
***		��ʾֵ��Χ		0 --------------------------------------- RangeMAX/100
***		
***						 
***		��λ2ʵ����������
***		
***		�ⲿ�����ѹ		0 ---------- 5 ---------- 25
***		
***		AD�����ѹ		0 --------- 0.67 ------- 2.93
***		
***		��ʾֵ��Χ		0 ---------------------- RangeMAX*0.5/100
***		
***						 
***		��λ3ʵ����������
***		
***		�ⲿ�����ѹ		0 ---------- 5
***		
***		AD�����ѹ		0 --------- 1.67		
***		
***		��ʾֵ��Χ		0 --------- RangeMAX*0.1/100
***		
**/

//void ult_delay_us(uint32_t us)
//{
//	us = us*72;
//	while(--us){}
//}
//ult_delay_us(10);
/*********������ǿֵ ultrasonic_sound************/
void update_ultrasonic(void)
{
	uint16_t i=0;
	uint16_t temp1=0;
	uint32_t Sum1=0;
	for(i=0;i<100;i++)
	{
		ultrasonic_ad[i] = ADC2_Channel_2_get();
	}
	quicksort(0,99);//�����㷨   
	for(i=15;i<85;i++)
	{
		Sum1 += ultrasonic_ad[i];
	}
//	Ultra_PowerTemp=(float)(Sum1/70)*(3.285/4096);
	Ultra_PowerTemp=(float)(Sum1/70)*(0.000802f);
	if(Ultra_PowerTemp <= 0.035f) Ultra_PowerTemp=0;  //��ѹ��ֵ
	if(Ultra_PowerTemp >= 3.285f) Ultra_PowerTemp=3.285f;  //��ѹ��ֵ
	GVotage = Ultra_PowerTemp;			//�������㵵λ
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	switch (GearsValue)
    {
    	case 1:
//			Ultra_PowerTemp=Ultra_PowerTemp*Ultra_PowerTemp*RangeMAX/��100*3*3*4��;	
		
//			Ultra_PowerTemp=Ultra_PowerTemp*RangeMAX/��100*3*4��;
			Ultra_PowerTemp=Ultra_PowerTemp*RangeMAX*0.000833f;
    		break;
    	case 2:
//			Ultra_PowerTemp=Ultra_PowerTemp*Ultra_PowerTemp*RangeMAX/��100*2*2.93*2.93*4��;	
		
//			Ultra_PowerTemp=Ultra_PowerTemp*RangeMAX/��100*2*2.93*4��;
			Ultra_PowerTemp=Ultra_PowerTemp*RangeMAX*0.000427f;
    		break;
    	case 3:
//			Ultra_PowerTemp=Ultra_PowerTemp*Ultra_PowerTemp*RangeMAX/��100*10*1.67*1.67*4��;	
		
//			Ultra_PowerTemp=Ultra_PowerTemp*RangeMAX/��100*10*1.67*4��;
			Ultra_PowerTemp=Ultra_PowerTemp*RangeMAX*0.00015f;
    		break;
    	default:
    		break;
    }	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(ConectFrqcyOnOff)              //����ʱ�����Ƶ��
	{
		Ultra_PowerTemp=Ultra_PowerTemp*(1+frequency*0.075f);
	}
	else
	{
		Ultra_PowerTemp=Ultra_PowerTemp*4;	
	}

	for(i=0;i<3;i++)                    //ѹջ
	{
		ultrasonic_last[3-i]=ultrasonic_last[2-i];
	}
		ultrasonic_last[0] = Ultra_PowerTemp;

	/*̽���ó� ����������ǿֵ*/
	if(ultrasonic_last[0]<0.05f)
	{
		Ultra_PowerTemp=0;
		/*̽���ó���������������� ��ֹ�ٴη���ʱ��������ǿֵ�������������*/
		for(i=0;i<4;i++)
		{
			ultrasonic_last[i]=0;
		}
	}
	else
	{
		Ultra_PowerTemp=ultrasonic_last[0]*0.5f+ultrasonic_last[1]*0.2f+ultrasonic_last[2]*0.2f+ultrasonic_last[3]*0.1f;					
	}

	Ultra_PowerTemp=Ultra_PowerTemp*k_value*0.01f;
	
	temp1=(uint16_t)(Ultra_PowerTemp*100);
	Ultra_PowerTemp=(float)(temp1*0.01f);     //����С�������λ ��������

	switch (GearsValue)
    {
    	case 1:
			if(Ultra_PowerTemp > (RangeMAX*0.01f))   //RangeMAX/100
			{
				Ultra_PowerTemp = RangeMAX*0.01f;			
				ultrasonic_sound_max = RangeMAX*0.01f;
			}
			else if(Ultra_PowerTemp<0.05f)  
			{
				Ultra_PowerTemp=0;	
				ultrasonic_sound_max=0;
			}
    		break;
    	case 2:
			if(Ultra_PowerTemp > RangeMAX*0.005f)   //RangeMAX/100/2
			{
				Ultra_PowerTemp = RangeMAX*0.005f;
				ultrasonic_sound_max = RangeMAX*0.005f;
			}
			else if(Ultra_PowerTemp<0.05f)  
			{
				Ultra_PowerTemp=0;	
				ultrasonic_sound_max=0;
			}
    		break;
    	case 3:
			if(Ultra_PowerTemp > RangeMAX*0.001f)   //RangeMAX/100/10
			{
				Ultra_PowerTemp = RangeMAX*0.001f;
				ultrasonic_sound_max = RangeMAX*0.001f;			
			}
			else if(Ultra_PowerTemp<0.05f)  
			{
				Ultra_PowerTemp=0;	
				ultrasonic_sound_max=0;
			}
    		break;
    	default:
    		break;
    }
	if(Ultra_PowerTemp>ultrasonic_sound_max)
	{
		ultrasonic_sound_max = Ultra_PowerTemp;
	}
	ultrasonic_sound = Ultra_PowerTemp;
}

//fft���ֵ
static void powerMag(long nfill)
{
	int32_t lX,lY;
	uint32_t i;
	for (i=0; i < nfill; i++) 
	{
		lX= (lBUFOUT[i]<<16)>>16; /* sine_cosine --> cos */ 
		lY= (lBUFOUT[i] >> 16);   /* sine_cosine --> sin */     
		{
			float X=  NPT*((float)lX)/32768; 
			float Y = NPT*((float)lY)/32768; 
			float Mag = sqrt(X*X+ Y*Y)/nfill;  // ��ƽ����,�ٿ��� 
			lBUFMAG[i] = (long)(Mag*65536); 
		}
	} 
}

/* 
********************************************************************************************************* 
˵��:(���ٸ���Ҷ�任)��ȡƵ��
********************************************************************************************************* 
*/ 
void update_frequence(void)
{
	uint16_t    i=0;
	uint16_t    j=0;
	uint32_t    temp=0;
	long    	temp1=0;
	float 		FrequencyTemp=0.0f;
	myDMA_Config(DMA1_Channel1,(uint32_t)&ADC1->DR,(uint32_t)&frequency_ad,NPT);   //ÿ����������  ���ڴ��ַָ��ƫ�Ƶ��׵�ַ		
	MYDMA_Enable(DMA1_Channel1,NPT); //����һ��DMA����
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	while(1)
	{
		if(DMA_GetFlagStatus(DMA1_FLAG_TC1)!=RESET)	//�ж�ͨ��1�������
		{
			DMA_ClearFlag(DMA1_FLAG_TC1);//���ͨ��1������ɱ�־
			break; 
		}				
	}			    
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for(i=0;i<NPT;i++)
	{
		lBUFIN[i]=frequency_ad[i]<<16;					 
	}
	cr4_fft_1024_stm32(lBUFOUT,lBUFIN,NPT);
	powerMag(NPT);                      
	temp1=0;				
	for(i=10;i<500;i++)                    //��20��ʼ ��Ϊ��ʱ���⵽�ǳ�����Ƶ��Ƶ��  ����������
	{
		if(lBUFMAG[i]>temp1)
		{
			temp1=lBUFMAG[i];
			j=i;
		}
	}		
//	FrequencyTemp=((float)(j*0.65149f)-0.02f);	   //����Ƶ��56M,ADC_SampleTime_1Cycles5
	FrequencyTemp=(float)(j*0.586f);	           //����Ƶ��72M,ADC_SampleTime_7Cycles5
	temp=(uint32_t)(FrequencyTemp*100);
	FrequencyTemp=(float)(temp*0.01f);    				 	//����С�������λ ��������
	if(FrequencyTemp>290.0f)                         //��������
	{
		FrequencyTemp=290.0f;                 
	}	
	if(FrequencyTemp<12.0f)                         //��������
	{
		FrequencyTemp=0;                 
	}
	frequency=FrequencyTemp;
}


/******************end file*********************/





