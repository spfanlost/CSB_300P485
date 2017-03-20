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
  *	�ڲ�flash��дAPI
  ******************************************************************************
  */

#include "bsp.h"

/*
*********************************************************************************************************
*	�� �� ��: bsp_GetSector
*	����˵��: ���ݵ�ַ���������׵�ַ
*	��    �Σ���
*	�� �� ֵ: �����׵�ַ
*********************************************************************************************************
*/
uint32_t bsp_GetSector(uint32_t _ulWrAddr)
{
	uint32_t sector = 0;
	sector = _ulWrAddr & SECTOR_MASK;
	return sector;
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_ReadCpuFlash_HalfWord
*	����˵��: ��ȡCPU Flash������
*	��    �Σ�_ucpDst : Ŀ�껺����
*			 _ulFlashAddr : ��ʼ��ַ
*			 _ulSize : ���ݴ�С����λ�ǰ��֣�HalfWord
*	�� �� ֵ: 0=�ɹ���1=ʧ��
*********************************************************************************************************
*/
uint8_t bsp_ReadCpuFlash_HalfWord(uint32_t _ulFlashAddr,uint16_t *_ucpDst,uint32_t _ulSize)
{
	uint32_t i;
	/* ���ƫ�Ƶ�ַ����оƬ�������򲻸�д��������� */
	if (_ulFlashAddr + _ulSize > FLASH_BASE_ADDR + FLASH_SIZE)
	{
		return 1;
	}
	/* ����Ϊ0ʱ����������,������ʼ��ַΪ���ַ����� */
	if (_ulSize == 0)
	{
		return 1;
	}
	for( i =0; i < _ulSize; i++)
	{
		_ucpDst[ i ]=*(uint16_t*)_ulFlashAddr;
		_ulFlashAddr+=2;
	}
	return 0;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_CmpCpuFlash
*	����˵��: �Ƚ�Flashָ����ַ������.
*	��    ��: _ulFlashAddr : Flash��ַ
*			 _ucpBuf : ���ݻ�����
*			 _ulSize : ���ݴ�С����λ�ǰ��֣�HalfWord
*	�� �� ֵ:
*			FLASH_IS_EQU		0   Flash���ݺʹ�д���������ȣ�����Ҫ������д����
*			FLASH_REQ_WRITE		1	Flash����Ҫ������ֱ��д
*			FLASH_REQ_ERASE		2	Flash��Ҫ�Ȳ���,��д
*			FLASH_PARAM_ERR		3	������������
*********************************************************************************************************
*/
uint8_t bsp_CmpCpuFlash(uint32_t _ulFlashAddr, uint16_t *_ucpBuf, uint32_t _ulSize)
{
	uint32_t i;
	uint8_t ucIsEqu;	/* ��ȱ�־ */
	uint16_t ucByte;
	/* ���ƫ�Ƶ�ַ����оƬ�������򲻸�д��������� */
	if (_ulFlashAddr + _ulSize > FLASH_BASE_ADDR + FLASH_SIZE)
	{
		return FLASH_PARAM_ERR;		/*��������������*/
	}
	/* ����Ϊ0ʱ������ȷ */
	if (_ulSize == 0)
	{
		return FLASH_IS_EQU;		/* Flash���ݺʹ�д���������� */
	}
	ucIsEqu = 1;			/* �ȼ��������ֽںʹ�д���������ȣ���������κ�һ������ȣ�������Ϊ 0 */
	for (i = 0; i < _ulSize; i++)
	{
		ucByte = *(uint16_t *)_ulFlashAddr;
		if (ucByte != *_ucpBuf)
		{
			if (ucByte != 0xFFFF)
			{
				return FLASH_REQ_ERASE;		/* ��Ҫ��������д */
			}
			else
			{
				ucIsEqu = 0;	/* ����ȣ���Ҫд */
			}
		}
		_ulFlashAddr+=2;
		_ucpBuf+=2;
	}
	if (ucIsEqu == 1)
	{
		return FLASH_IS_EQU;	/* Flash���ݺʹ�д���������ȣ�����Ҫ������д���� */
	}
	else
	{
		return FLASH_REQ_WRITE;	/* Flash����Ҫ������ֱ��д */
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_WriteCpuFlash
*	����˵��: д���ݵ�CPU �ڲ�Flash��
*	��    ��: _ulFlashAddr : Flash��ַ
*			 _ucpSrc : ���ݻ�����
*			 _ulSize : ���ݴ�С����λ�ǰ��֣�HalfWord
*	�� �� ֵ: 0-�ɹ���1-���ݳ��Ȼ��ַ�����2-дFlash����(����Flash������)
*********************************************************************************************************
*/
uint8_t bsp_WriteCpuFlash(uint32_t _ulFlashAddr, uint16_t *_ucpSrc, uint32_t _ulSize)
{
	uint32_t i;
	uint8_t ucRet;
	FLASH_Status status = FLASH_COMPLETE;
	/* ���ƫ�Ƶ�ַ����оƬ�������򲻸�д��������� */
	if (_ulFlashAddr + _ulSize > FLASH_BASE_ADDR + FLASH_SIZE)
	{
		return 1;
	}
	/* ����Ϊ0 ʱ����������  */
	if (_ulSize == 0)
	{
		return 0;
	}
	ucRet = bsp_CmpCpuFlash(_ulFlashAddr, _ucpSrc, _ulSize);
	if (ucRet == FLASH_IS_EQU)
	{
		return 0;
	}
	__set_PRIMASK(1);  		/* ���ж� */
	/* FLASH ���� */
	FLASH_Unlock();
  	/* Clear pending flags (if any) */
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
	/* ��Ҫ���� */
	if (ucRet == FLASH_REQ_ERASE)
	{
		status = FLASH_ErasePage(bsp_GetSector(_ulFlashAddr));
		if (status != FLASH_COMPLETE)
		{
			return 2;
		}		
	}
	/* ������ģʽ��̣�Ϊ���Ч�ʣ����԰��ֱ�̣�һ��д��2�ֽڣ� */
	for (i = 0; i < _ulSize; i++)
	{
		//FLASH_ProgramByte(_ulFlashAddr++, *_ucpSrc++);		
//		usTemp = _ucpSrc[2 * i];
//		usTemp |= (_ucpSrc[2 * i + 1] << 8);
		status = FLASH_ProgramHalfWord(_ulFlashAddr, _ucpSrc[i]);
		if (status != FLASH_COMPLETE)
		{
			break;
		}
		_ulFlashAddr += 2;
	}
  	/* Flash ��������ֹдFlash���ƼĴ��� */
  	FLASH_Lock();
  	__set_PRIMASK(0);  		/* ���ж� */
	if (status == FLASH_COMPLETE)
	{
		return 0;
	}
	return 2;
}

extern uint16_t RefreshGear(void);
void Read_Stored_Data(void)
{
	extern uint16_t k_value;			//�洢 Kֵ
	extern uint16_t SaveGearsValue ;    //�洢 ��λ
	extern uint16_t GearsValue ;        //
	extern uint8_t  AutoGearFlag ;      //
	extern uint16_t ConectFrqcyOnOff;   //��ǿ����ʱ���Ƿ����Ƶ��
	extern uint16_t SaveTimeJG;         //
	extern uint16_t PowrOffTime ;       //
	extern uint16_t LogoOnOff ;         //
	extern uint16_t RangeMAX;           //�������� �Ͻ�Ϊ�㣡����
	extern uint16_t SaveTimeCnt;        //
	extern uint16_t ModbusDeviceID;        //
/////////////////////////////////////////////////////////////////////////////////////////////////
	bsp_ReadCpuFlash_HalfWord(SAVE_ADDR_k_value,(uint16_t *)&k_value, 1);
	if(k_value>=1001)
		{
			k_value = 100;			   //Ĭ��Kֵ
			bsp_WriteCpuFlash(SAVE_ADDR_k_value,(uint16_t *)&k_value, 1);
		}								
/////////////////////////////////////////////////////////////////////////////////////////////////		
	bsp_ReadCpuFlash_HalfWord(SAVE_ADDR_SaveGearsValue,(uint16_t *)&SaveGearsValue,1);	
	if(SaveGearsValue >= 5)
		{
			SaveGearsValue  = 1;      //Ĭ�ϵ�λ
			AutoGearFlag = 0;
			bsp_WriteCpuFlash(SAVE_ADDR_SaveGearsValue,(uint16_t *)&SaveGearsValue, 1);
		}
	else if((SaveGearsValue <= 3)&&(SaveGearsValue > 0)) //ǰ���� ���������ٵ�λ ���Ƕ��ٵ�λ
		{
			AutoGearFlag = 0;
			GearsValue = SaveGearsValue;
		}
	else
		{
			AutoGearFlag = 1;
			GearsValue = 1;           //���ĵ�������Ĭ�ϵ�һ��
			SetGear(GearsValue);
		}
/////////////////////////////////////////////////////////////////////////////////////////////////
	bsp_ReadCpuFlash_HalfWord(SAVE_ADDR_ConectFrqcyOnOff,(uint16_t *)&ConectFrqcyOnOff,1);	                   
	if(ConectFrqcyOnOff>=2)
		{
			ConectFrqcyOnOff=0;        //��ǿ����ʱ���Ƿ����Ƶ��
			bsp_WriteCpuFlash(SAVE_ADDR_ConectFrqcyOnOff,(uint16_t *)&ConectFrqcyOnOff, 1);
		}								
/////////////////////////////////////////////////////////////////////////////////////////////////
	bsp_ReadCpuFlash_HalfWord(SAVE_ADDR_SaveTimeJG,(uint16_t *)&SaveTimeJG,1);	
	if(SaveTimeJG>300)
		{
			SaveTimeJG=3;              //Ĭ�ϴ洢ʱ����s
			bsp_WriteCpuFlash(SAVE_ADDR_SaveTimeJG,(uint16_t *)&SaveTimeJG, 1);
		}								
/////////////////////////////////////////////////////////////////////////////////////////////////
	bsp_ReadCpuFlash_HalfWord(SAVE_ADDR_PowrOffTime,(uint16_t *)&PowrOffTime,1);	
	if(PowrOffTime>=60)
		{
			PowrOffTime=10;            //�ػ�ʱ��   Ĭ��10
			bsp_WriteCpuFlash(SAVE_ADDR_PowrOffTime,(uint16_t *)&PowrOffTime, 1);
		}		
/////////////////////////////////////////////////////////////////////////////////////////////////
	bsp_ReadCpuFlash_HalfWord(SAVE_ADDR_LogoOnOff,(uint16_t *)&LogoOnOff,1);
	if(LogoOnOff>=2)
		{
			LogoOnOff = 0;             //��ʾlogo   Ĭ�Ϲ�
			bsp_WriteCpuFlash(SAVE_ADDR_LogoOnOff,(uint16_t *)&LogoOnOff, 1);
		}								
/////////////////////////////////////////////////////////////////////////////////////////////////
	bsp_ReadCpuFlash_HalfWord(SAVE_ADDR_RangeMAX,(uint16_t *)&RangeMAX,1);	
	if((RangeMAX>=25400)||(RangeMAX<=10))
	{
		RangeMAX=254;                  //���� Ĭ��254
		bsp_WriteCpuFlash(SAVE_ADDR_RangeMAX,(uint16_t *)&RangeMAX, 1);
	}
/////////////////////////////////////////////////////////////////////////////////////////////////
	bsp_ReadCpuFlash_HalfWord(SAVE_ADDR_SaveTimeCnt,(uint16_t *)&SaveTimeCnt,1);
	if(SaveTimeCnt>=25)
	{
		SaveTimeCnt=0;              //Ĭ��0
		bsp_WriteCpuFlash(SAVE_ADDR_SaveTimeCnt,(uint16_t *)&SaveTimeCnt, 1);
	}
/////////////////////////////////////////////////////////////////////////////////////////////////
	bsp_ReadCpuFlash_HalfWord(SAVE_ADDR_ModbusDeviceID,(uint16_t *)&ModbusDeviceID,1);	
	if(ModbusDeviceID>=246)
		{
			ModbusDeviceID=1;              //   modbus��ַ Ĭ��1
			bsp_WriteCpuFlash(SAVE_ADDR_ModbusDeviceID,(uint16_t *)&ModbusDeviceID, 1);
		}
/////////////////////////////////////////////////////////////////////////////////////////////////
	GUI_Power_On();
/////////////////////////////////////////////////////////////////////////////////////////////////		

}
