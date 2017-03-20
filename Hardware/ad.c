/*********************************Copyright (c)*********************************                                      
  *                                             
  ******************************************************************************
  * �ļ����ƣ�ad.C
  * ��    ����ad�����ļ�
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

#define ADC1_DR_Address    ((uint32_t)0x4001244C)

/*******************************************************************************
  * @��������	ADC1_Channel_1_init
  * @����˵��  �벨��Ƶ��ѹ�ɼ�ͨ��
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void ADC1_Channel_1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1,ENABLE); 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); 						
	//�벨��Ƶ�����IO�ڳ�ʼ��PA1_ADC-1_IN1
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;		//ģ����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;     //ADC1  work mode as independent
	ADC_InitStructure.ADC_ScanConvMode       = DISABLE;                  //enable scan mode
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                   //ADC work mode as continuous conversion
	ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;//sofware set start conversion
	ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;      //data align as right align
	ADC_InitStructure.ADC_NbrOfChannel       = 1;   					 //1 channel
	ADC_Init(ADC1, &ADC_InitStructure); //init ADC1 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_7Cycles5 ); //ADC1,ADCͨ��,����ʱ��Ϊ����	  			    
	ADC_DMACmd(ADC1, ENABLE);   //enable adc1  dma  transmit
	ADC_Cmd(ADC1, ENABLE); 			//enable ADC1	
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));  //wait for reset ADC1 calibration falg as reset
	ADC_StartCalibration(ADC1); 	 //start adc1 calibration
	while(ADC_GetCalibrationStatus(ADC1)); 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//�������ADת��
}
/*******************************************************************************
  * @��������	ADC2_Channel_2_init
  * @����˵��  ��ǿ����
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void ADC2_Channel_2_init(void)
{
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC2, ENABLE );	  //ʹ��ADC2ͨ��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	//��ǿ���������IO�ڳ�ʼ��PA2_ADC-2_IN2
	GPIO_InitStructure.GPIO_Speed            = GPIO_Speed_50MHz;                        
	GPIO_InitStructure.GPIO_Pin              = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode             = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	ADC_DeInit(ADC2);  //��λADC,������ ADC ��ȫ���Ĵ�������Ϊȱʡֵ
	ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode       = DISABLE;	      //ģ��ת�������ڶ�ͨ��ģʽ ����ת��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel       = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC2, &ADC_InitStructure);            //����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   
	ADC_Cmd(ADC2, ENABLE);	//ʹ��ADC2
	ADC_ResetCalibration(ADC2);	//ʹ�ܸ�λУ׼  
	while(ADC_GetResetCalibrationStatus(ADC2));	//�ȴ���λУ׼����
	ADC_StartCalibration(ADC2);	 //����ADУ׼
	while(ADC_GetCalibrationStatus(ADC2));	 //�ȴ�У׼����
	ADC_RegularChannelConfig(ADC2, ADC_Channel_2, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
	ADC_SoftwareStartConvCmd(ADC2, ENABLE);		                        //ʹ��ָ����ADC1�����ת����������	
	while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC ));                   //�ȴ�ת������
  }
	
/*******************************************************************************
  * @��������	ADC2_Channel_3_init
  * @����˵��   ȫ����Ƶ����ʱû�ã�
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void ADC2_Channel_3_init(void)
{
	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC2	, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	//ȫ����Ƶ�����IO�ڳ�ʼ��PA3_ADC-2_IN3
	GPIO_InitStructure.GPIO_Speed            = GPIO_Speed_50MHz;                        
	GPIO_InitStructure.GPIO_Pin              = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode             = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	ADC_DeInit(ADC2);  //��λADC1,������ ADC2 ��ȫ���Ĵ�������Ϊȱʡֵ
	ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode       = DISABLE;	      //ģ��ת�������ڶ�ͨ��ģʽ ����ת��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel       = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC2, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   
	ADC_Cmd(ADC2, ENABLE);	//ʹ��ADC2
	ADC_ResetCalibration(ADC2);	//ʹ�ܸ�λУ׼  
	while(ADC_GetResetCalibrationStatus(ADC2));	//�ȴ���λУ׼����
	ADC_StartCalibration(ADC2);	 //����ADУ׼
	while(ADC_GetCalibrationStatus(ADC2));	 //�ȴ�У׼����
}

	
/*******************************************************************************
  * @��������	ADC2_Channel_10_init
  * @����˵��  ����ѹ����
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
void ADC2_Channel_10_init(void)
{
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_ADC2	, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	//��ص�ѹ���������IO�ڳ�ʼ��PC0_ADC-2_IN10
	GPIO_InitStructure.GPIO_Speed            = GPIO_Speed_50MHz;  
	GPIO_InitStructure.GPIO_Pin              = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode             = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	ADC_DeInit(ADC2);  //��λADC1,������ ADC2 ��ȫ���Ĵ�������Ϊȱʡֵ
	ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode       = DISABLE;	      //ģ��ת�������ڶ�ͨ��ģʽ ����ת��
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel       = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC2, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   
	ADC_Cmd(ADC2, ENABLE);	//ʹ��ADC2
	ADC_ResetCalibration(ADC2);	//ʹ�ܸ�λУ׼  
	while(ADC_GetResetCalibrationStatus(ADC2));	//�ȴ���λУ׼����
	ADC_StartCalibration(ADC2);	 //����ADУ׼
	while(ADC_GetCalibrationStatus(ADC2));	 //�ȴ�У׼����
  }

/*******************************************************************************
  * @��������	ADC2_Channel_2_get
  * @����˵��  ��ǿ���� 
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
uint16_t ADC2_Channel_2_get(void)//��ǿ����
{
    //����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC2, ADC_Channel_2, 1, ADC_SampleTime_239Cycles5 );	//ADC,ADCͨ��,����ʱ��Ϊ239.5����	  			    
	ADC_SoftwareStartConvCmd(ADC2, ENABLE);		                        //ʹ��ָ����ADC1�����ת����������	
	while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC ));                   //�ȴ�ת������
	return ADC_GetConversionValue(ADC2);	                            //�������һ��ADC1�������ת�����
}

/*******************************************************************************
  * @��������	ADC1_Channel_1_get
  * @����˵��   ȫ����Ƶ
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
//uint16_t ADC2_Channel_3_get(void)//ȫ����Ƶ
//{
//    //����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
//	ADC_RegularChannelConfig(ADC2, ADC_Channel_3, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
//	ADC_SoftwareStartConvCmd(ADC2, ENABLE);		                        //ʹ��ָ����ADC1�����ת����������	
//	while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC ));                   //�ȴ�ת������
//	return ADC_GetConversionValue(ADC2);	                            //�������һ��ADC1�������ת�����
//}

/*******************************************************************************
  * @��������	ADC2_Channel_10_get
  * @����˵��  ����ѹ����  
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
uint16_t ADC2_Channel_10_get(void)//����ѹ����
{
    //����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC2, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
	ADC_SoftwareStartConvCmd(ADC2, ENABLE);		                        //ʹ��ָ����ADC1�����ת����������	
	while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC ));                     //�ȴ�ת������
	return ADC_GetConversionValue(ADC2);	                            //�������һ��ADC1�������ת�����
}

/*******************************************************************************
  * @��������	Get_Bat_Average
  * @����˵��  ������ֵ 
  * @�������   ��
  * @�������   ��
  * @���ز���   ��
*******************************************************************************/
uint16_t Get_Bat_Average(void)
{
//��λֵƽ���˲������ֳƷ��������ƽ���˲�����
	const int FILTER_N = 100;

	int i, j;
	uint32_t filter_temp, filter_sum = 0;
	int filter_buf[FILTER_N];
	for(i = 0; i < FILTER_N; i++) {
	filter_buf[i] = ADC2_Channel_10_get();
	}
	// ����ֵ��С�������У�ð�ݷ���
	for(j = 0; j < FILTER_N - 1; j++) {
		for(i = 0; i < FILTER_N - 1 - j; i++) {
		  if(filter_buf[i] > filter_buf[i + 1]) {
			filter_temp = filter_buf[i];
			filter_buf[i] = filter_buf[i + 1];
			filter_buf[i + 1] = filter_temp;
		  }
		}
	}
	// ȥ�������С��ֵ����ƽ��
	for(i = 10; i < FILTER_N - 10; i++) filter_sum += filter_buf[i];
	return filter_sum / (FILTER_N - 20);
} 	 




