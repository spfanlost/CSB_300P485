#ifndef __USART_H
#define __USART_H

void bsp_usart_init(uint32_t bound);

uint16_t CRC16(uint8_t *puchMsgg,uint8_t usDataLen);//puchMsgg��Ҫ����CRCУ�����Ϣ��usDataLen����Ϣ���ֽ��� 
uint8_t   Uart1_PutChar(uint8_t ch);
void Uart1_PutString(uint8_t* buf , uint8_t len);
void TIM4_Configuration(void);
void ModInit(uint8_t Id);

#endif


