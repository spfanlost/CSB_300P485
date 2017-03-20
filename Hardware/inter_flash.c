/*********************************Copyright (c)*********************************                                      
  *                                             
  ******************************************************************************
  * 文件名称：GearsSwitch.C
  * 描    述：文件
  * 创 建 者: 余萌
  * 创建日期: 
  * 修 改 者:             
  * 修改日期: 
  * 版    本: V1.0.0
  ******************************************************************************
  * attention
  *	内部flash读写API
  ******************************************************************************
  */

#include "bsp.h"

/*
*********************************************************************************************************
*	函 数 名: bsp_GetSector
*	功能说明: 根据地址计算扇区首地址
*	形    参：无
*	返 回 值: 扇区首地址
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
*	函 数 名: bsp_ReadCpuFlash_HalfWord
*	功能说明: 读取CPU Flash的内容
*	形    参：_ucpDst : 目标缓冲区
*			 _ulFlashAddr : 起始地址
*			 _ulSize : 数据大小（单位是半字）HalfWord
*	返 回 值: 0=成功，1=失败
*********************************************************************************************************
*/
uint8_t bsp_ReadCpuFlash_HalfWord(uint32_t _ulFlashAddr,uint16_t *_ucpDst,uint32_t _ulSize)
{
	uint32_t i;
	/* 如果偏移地址超过芯片容量，则不改写输出缓冲区 */
	if (_ulFlashAddr + _ulSize > FLASH_BASE_ADDR + FLASH_SIZE)
	{
		return 1;
	}
	/* 长度为0时不继续操作,否则起始地址为奇地址会出错 */
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
*	函 数 名: bsp_CmpCpuFlash
*	功能说明: 比较Flash指定地址的数据.
*	形    参: _ulFlashAddr : Flash地址
*			 _ucpBuf : 数据缓冲区
*			 _ulSize : 数据大小（单位是半字）HalfWord
*	返 回 值:
*			FLASH_IS_EQU		0   Flash内容和待写入的数据相等，不需要擦除和写操作
*			FLASH_REQ_WRITE		1	Flash不需要擦除，直接写
*			FLASH_REQ_ERASE		2	Flash需要先擦除,再写
*			FLASH_PARAM_ERR		3	函数参数错误
*********************************************************************************************************
*/
uint8_t bsp_CmpCpuFlash(uint32_t _ulFlashAddr, uint16_t *_ucpBuf, uint32_t _ulSize)
{
	uint32_t i;
	uint8_t ucIsEqu;	/* 相等标志 */
	uint16_t ucByte;
	/* 如果偏移地址超过芯片容量，则不改写输出缓冲区 */
	if (_ulFlashAddr + _ulSize > FLASH_BASE_ADDR + FLASH_SIZE)
	{
		return FLASH_PARAM_ERR;		/*　函数参数错误　*/
	}
	/* 长度为0时返回正确 */
	if (_ulSize == 0)
	{
		return FLASH_IS_EQU;		/* Flash内容和待写入的数据相等 */
	}
	ucIsEqu = 1;			/* 先假设所有字节和待写入的数据相等，如果遇到任何一个不相等，则设置为 0 */
	for (i = 0; i < _ulSize; i++)
	{
		ucByte = *(uint16_t *)_ulFlashAddr;
		if (ucByte != *_ucpBuf)
		{
			if (ucByte != 0xFFFF)
			{
				return FLASH_REQ_ERASE;		/* 需要擦除后再写 */
			}
			else
			{
				ucIsEqu = 0;	/* 不相等，需要写 */
			}
		}
		_ulFlashAddr+=2;
		_ucpBuf+=2;
	}
	if (ucIsEqu == 1)
	{
		return FLASH_IS_EQU;	/* Flash内容和待写入的数据相等，不需要擦除和写操作 */
	}
	else
	{
		return FLASH_REQ_WRITE;	/* Flash不需要擦除，直接写 */
	}
}

/*
*********************************************************************************************************
*	函 数 名: bsp_WriteCpuFlash
*	功能说明: 写数据到CPU 内部Flash。
*	形    参: _ulFlashAddr : Flash地址
*			 _ucpSrc : 数据缓冲区
*			 _ulSize : 数据大小（单位是半字）HalfWord
*	返 回 值: 0-成功，1-数据长度或地址溢出，2-写Flash出错(估计Flash寿命到)
*********************************************************************************************************
*/
uint8_t bsp_WriteCpuFlash(uint32_t _ulFlashAddr, uint16_t *_ucpSrc, uint32_t _ulSize)
{
	uint32_t i;
	uint8_t ucRet;
	FLASH_Status status = FLASH_COMPLETE;
	/* 如果偏移地址超过芯片容量，则不改写输出缓冲区 */
	if (_ulFlashAddr + _ulSize > FLASH_BASE_ADDR + FLASH_SIZE)
	{
		return 1;
	}
	/* 长度为0 时不继续操作  */
	if (_ulSize == 0)
	{
		return 0;
	}
	ucRet = bsp_CmpCpuFlash(_ulFlashAddr, _ucpSrc, _ulSize);
	if (ucRet == FLASH_IS_EQU)
	{
		return 0;
	}
	__set_PRIMASK(1);  		/* 关中断 */
	/* FLASH 解锁 */
	FLASH_Unlock();
  	/* Clear pending flags (if any) */
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
	/* 需要擦除 */
	if (ucRet == FLASH_REQ_ERASE)
	{
		status = FLASH_ErasePage(bsp_GetSector(_ulFlashAddr));
		if (status != FLASH_COMPLETE)
		{
			return 2;
		}		
	}
	/* 按半字模式编程（为提高效率，可以按字编程，一次写入2字节） */
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
  	/* Flash 加锁，禁止写Flash控制寄存器 */
  	FLASH_Lock();
  	__set_PRIMASK(0);  		/* 开中断 */
	if (status == FLASH_COMPLETE)
	{
		return 0;
	}
	return 2;
}

extern uint16_t RefreshGear(void);
void Read_Stored_Data(void)
{
	extern uint16_t k_value;			//存储 K值
	extern uint16_t SaveGearsValue ;    //存储 档位
	extern uint16_t GearsValue ;        //
	extern uint8_t  AutoGearFlag ;      //
	extern uint16_t ConectFrqcyOnOff;   //声强计算时候是否关联频率
	extern uint16_t SaveTimeJG;         //
	extern uint16_t PowrOffTime ;       //
	extern uint16_t LogoOnOff ;         //
	extern uint16_t RangeMAX;           //量程上限 严禁为零！！！
	extern uint16_t SaveTimeCnt;        //
	extern uint16_t ModbusDeviceID;        //
/////////////////////////////////////////////////////////////////////////////////////////////////
	bsp_ReadCpuFlash_HalfWord(SAVE_ADDR_k_value,(uint16_t *)&k_value, 1);
	if(k_value>=1001)
		{
			k_value = 100;			   //默认K值
			bsp_WriteCpuFlash(SAVE_ADDR_k_value,(uint16_t *)&k_value, 1);
		}								
/////////////////////////////////////////////////////////////////////////////////////////////////		
	bsp_ReadCpuFlash_HalfWord(SAVE_ADDR_SaveGearsValue,(uint16_t *)&SaveGearsValue,1);	
	if(SaveGearsValue >= 5)
		{
			SaveGearsValue  = 1;      //默认档位
			AutoGearFlag = 0;
			bsp_WriteCpuFlash(SAVE_ADDR_SaveGearsValue,(uint16_t *)&SaveGearsValue, 1);
		}
	else if((SaveGearsValue <= 3)&&(SaveGearsValue > 0)) //前三档 读出来多少档位 就是多少档位
		{
			AutoGearFlag = 0;
			GearsValue = SaveGearsValue;
		}
	else
		{
			AutoGearFlag = 1;
			GearsValue = 1;           //第四档开机就默认第一档
			SetGear(GearsValue);
		}
/////////////////////////////////////////////////////////////////////////////////////////////////
	bsp_ReadCpuFlash_HalfWord(SAVE_ADDR_ConectFrqcyOnOff,(uint16_t *)&ConectFrqcyOnOff,1);	                   
	if(ConectFrqcyOnOff>=2)
		{
			ConectFrqcyOnOff=0;        //声强计算时候是否关联频率
			bsp_WriteCpuFlash(SAVE_ADDR_ConectFrqcyOnOff,(uint16_t *)&ConectFrqcyOnOff, 1);
		}								
/////////////////////////////////////////////////////////////////////////////////////////////////
	bsp_ReadCpuFlash_HalfWord(SAVE_ADDR_SaveTimeJG,(uint16_t *)&SaveTimeJG,1);	
	if(SaveTimeJG>300)
		{
			SaveTimeJG=3;              //默认存储时间间隔s
			bsp_WriteCpuFlash(SAVE_ADDR_SaveTimeJG,(uint16_t *)&SaveTimeJG, 1);
		}								
/////////////////////////////////////////////////////////////////////////////////////////////////
	bsp_ReadCpuFlash_HalfWord(SAVE_ADDR_PowrOffTime,(uint16_t *)&PowrOffTime,1);	
	if(PowrOffTime>=60)
		{
			PowrOffTime=10;            //关机时间   默认10
			bsp_WriteCpuFlash(SAVE_ADDR_PowrOffTime,(uint16_t *)&PowrOffTime, 1);
		}		
/////////////////////////////////////////////////////////////////////////////////////////////////
	bsp_ReadCpuFlash_HalfWord(SAVE_ADDR_LogoOnOff,(uint16_t *)&LogoOnOff,1);
	if(LogoOnOff>=2)
		{
			LogoOnOff = 0;             //显示logo   默认关
			bsp_WriteCpuFlash(SAVE_ADDR_LogoOnOff,(uint16_t *)&LogoOnOff, 1);
		}								
/////////////////////////////////////////////////////////////////////////////////////////////////
	bsp_ReadCpuFlash_HalfWord(SAVE_ADDR_RangeMAX,(uint16_t *)&RangeMAX,1);	
	if((RangeMAX>=25400)||(RangeMAX<=10))
	{
		RangeMAX=254;                  //量程 默认254
		bsp_WriteCpuFlash(SAVE_ADDR_RangeMAX,(uint16_t *)&RangeMAX, 1);
	}
/////////////////////////////////////////////////////////////////////////////////////////////////
	bsp_ReadCpuFlash_HalfWord(SAVE_ADDR_SaveTimeCnt,(uint16_t *)&SaveTimeCnt,1);
	if(SaveTimeCnt>=25)
	{
		SaveTimeCnt=0;              //默认0
		bsp_WriteCpuFlash(SAVE_ADDR_SaveTimeCnt,(uint16_t *)&SaveTimeCnt, 1);
	}
/////////////////////////////////////////////////////////////////////////////////////////////////
	bsp_ReadCpuFlash_HalfWord(SAVE_ADDR_ModbusDeviceID,(uint16_t *)&ModbusDeviceID,1);	
	if(ModbusDeviceID>=246)
		{
			ModbusDeviceID=1;              //   modbus地址 默认1
			bsp_WriteCpuFlash(SAVE_ADDR_ModbusDeviceID,(uint16_t *)&ModbusDeviceID, 1);
		}
/////////////////////////////////////////////////////////////////////////////////////////////////
	GUI_Power_On();
/////////////////////////////////////////////////////////////////////////////////////////////////		

}
