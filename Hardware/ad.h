#ifndef __AD_H
#define __AD_H 			   

void ADC1_Channel_1_init(void);
void ADC2_Channel_2_init(void);
void ADC2_Channel_3_init(void);
void ADC2_Channel_10_init(void);

uint16_t ADC1_Channel_1_get(void);//�벨��Ƶ               
uint16_t ADC2_Channel_2_get(void);//��ǿ����
uint16_t ADC2_Channel_3_get(void);//ȫ����Ƶ
uint16_t ADC2_Channel_10_get(void);//��ص�ѹ����
uint16_t Get_Bat_Average(void);//��ص�ѹ��ֵ����

#endif

