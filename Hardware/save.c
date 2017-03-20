
#include "bsp.h"
//	һ��ҳ��������2048���ֽ�
//	ǰ256���ֽڣ�25*5*2���洢ʱ������
//	��1792���ֽڣ�448*2*2���洢����

//////////////////////////////////////////////////////////////
//	ʱ������洢25��
//	ÿ��ʱ�������ռ��5��16λ���ݣ�10���ֽڣ�
//	ʱ��				���ʱ�������Ӧ�����ݸ���  ���ݶο�ʼλ��
//	16 08 02  12 02 15			015                    0
//	16 09 03  16 12 10			025                    16
//	16 10 10  08 07 15			100                    31
//	������						������
//////////////////////////////////////////////////////////////
//uint16_t Save_TimeTab[25][5]={{0}};//ʱ��-�洢����

////////////////////////////////////////////////////////
//	ÿһ��ʱ������洢����100����
//  ����100�����¼����һ��ʱ���
//	448��2��16λ���ݣ�4���ֽڣ�
//	��ǿ			Ƶ��
//	5.46			39.78
//	1.37			32.68
//	������			������
////////////////////////////////////////////////////////
//uint16_t Save_DataTab[448][2]={{0}};//��ǿ  Ƶ��

/* ���ر������� ---------------------------------------------------------*/ 
static uint16_t Save_DataTab[1024]={0};
//uint16_t Save_DataTab[1024]={0};

/* �ⲿ�������� ---------------------------------------------------------*/ 
extern volatile float frequency;
extern volatile float ultrasonic_sound;
extern uint16_t TimeTabItemNum;//25
extern uint16_t DataTabItemNum;//448
extern uint16_t SaveTimeCnt;


/*******************************************************************************
* ��������: Add_TimeTab_time()
* ��������: Add_TimeTab_time�����һ��ʱ�������
* �������: 1��tcnt ��tcnt��ʱ������ ��Χ 0-24
* ���ز���: ��
********************************************************************************/
void Add_TimeTab_time(uint8_t tcnt)
{
	if(tcnt>=TimeTabItemNum)tcnt=0;
	tcnt = tcnt*5;
	Save_DataTab[0 + tcnt] = (( uint16_t )((uint8_t)(calendar.w_year-2000))<<8)|( uint16_t )calendar.w_month;//year&month
	Save_DataTab[1 + tcnt] = (( uint16_t )(calendar.w_date)<<8)|( uint16_t )calendar.hour;//date&hour
	Save_DataTab[2 + tcnt] =  (( uint16_t )(calendar.min)<<8)|( uint16_t )calendar.sec;//min&serc
}
/*******************************************************************************
* ��������: Add_TimeTab_Dest()
* ��������: Add_TimeTab_Dest�����һ��ʱ���λ��
* �������: 1��tcnt ��tcnt��ʱ������ ��Χ 0-24
* �������: 2��Dest ��tcnt��ʱ�����ݿ�ʼλ�� ��Χ 0-447
* ���ز���: ��
********************************************************************************/
void Add_TimeTab_Dest(uint8_t tcnt,uint16_t Dest)
{
	if(tcnt>=TimeTabItemNum)tcnt=0;
	tcnt = tcnt*5;
	Save_DataTab[4 + tcnt] = Dest;//min&serc
}
/*******************************************************************************
* ��������: Clear_TimeTab()
* ��������: Clear_TimeTab�����һ��ʱ�������
* �������: 1��tcnt ��tcnt��ʱ������ ��Χ 0-24
* ���ز���: ��
********************************************************************************/
void Clear_TimeTab(uint8_t tcnt)
{
	if(tcnt>=TimeTabItemNum)tcnt=0;
	tcnt = tcnt*5;
	Save_DataTab[0 + tcnt] = 0;//����
	Save_DataTab[1 + tcnt] = 0;//��ʱ
	Save_DataTab[2 + tcnt] = 0;//����
	Save_DataTab[3 + tcnt] = 0;//Num
	Save_DataTab[4 + tcnt] = 0;//Dest
}

/*******************************************************************************
* ��������: TimeTabHaveData()
* ��������: TimeTabHaveData��ʱ���λ���Ƿ���ֵ
* �������: 1��tcnt ��tcnt��ʱ������ ��Χ 0-24
* ���ز���: TRUE����ֵ����FALSE����ֵ��
********************************************************************************/
uint8_t TimeTabHaveData(uint8_t tcnt)
{
	if(tcnt>=TimeTabItemNum)tcnt=0;
	tcnt = tcnt*5;
	if(Save_DataTab[3 + tcnt] != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;	
	}
}
/*******************************************************************************
* ��������: Add_TimeTab_num()
* ��������: Add_TimeTab_num�����һ��ʱ�������
* �������: 1��tcnt ��tcnt��ʱ������ ��Χ 0-24
* �������: 2��num  ��tcnt��ʱ��������cnt������ ��Χ 0-447
* ���ز���: ��
********************************************************************************/
void Add_TimeTab_num(uint8_t tcnt, uint16_t num)
{
	if(tcnt>=TimeTabItemNum)tcnt=0;
	if(num>=448)num=0;
	tcnt = tcnt*5;
	Save_DataTab[3 + tcnt] = num;
}
/*******************************************************************************
* ��������: Get_TimeTab_Time()
* ��������: Get_TimeTab_Time����ȡһ��ʱ���
* �������: 1��tcnt ��tcnt��ʱ������ ��Χ 0-24
* ���ز���: ʱ������ݸ���
********************************************************************************/
uint16_t* Get_TimeTab_Time(uint8_t tcnt)
{
	if(tcnt>TimeTabItemNum)tcnt=0;
	tcnt = tcnt*5;
	return  &Save_DataTab[tcnt];
}
/*******************************************************************************
* ��������: Get_TimeTab_num()
* ��������: Get_TimeTab_num����ȡһ��ʱ������ݸ���
* �������: 1��tcnt ��tcnt��ʱ������ ��Χ 0-24
* ���ز���: ʱ������ݸ���
********************************************************************************/
uint16_t Get_TimeTab_num(uint8_t tcnt)
{
	if(tcnt>=TimeTabItemNum)tcnt=0;
	tcnt = tcnt*5;
	return Save_DataTab[3 + tcnt];
}
/*******************************************************************************
* ��������: Get_TimeTab_Dest()
* ��������: Get_TimeTab_Dest����ȡһ��ʱ������ݸ���
* �������: 1��tcnt ��tcnt��ʱ������ ��Χ 0-24
* ���ز���: ʱ������ݿ�ʼλ��
********************************************************************************/
uint16_t Get_TimeTab_Dest(uint8_t tcnt)
{
	if(tcnt>=TimeTabItemNum)tcnt=0;
	tcnt = tcnt*5;
	return Save_DataTab[4 + tcnt];
}

/*******************************************************************************
* ��������: Add_DataTab()
* ��������: Add_DataTab�����һ��ʱ�������
* �������: 1��dat ��ӵ�dat���ݱ� ��Χ 0-447
* ���ز���: ��
********************************************************************************/
void Add_DataTab(uint16_t dat)
{
	if(dat>=DataTabItemNum) dat=0;
	dat = dat*2 + 128;
	Save_DataTab[0 + dat] = (uint16_t)(ultrasonic_sound*100);
	Save_DataTab[1 + dat] = (uint16_t)(frequency*100);
//	Save_DataTab[0 + dat] = (uint16_t)(13.14*100);
//	Save_DataTab[1 + dat] = (uint16_t)(5.27*100);
}
/*******************************************************************************
* ��������: Get_DataTab()
* ��������: Get_DataTab�����һ��ʱ�������
* �������: 1��dat ��ӵ�dat���ݱ� ��Χ 0-447
* ���ز���: ��
********************************************************************************/
uint16_t* Get_DataTab(uint16_t dat)
{
	if(dat>=DataTabItemNum) dat=0;
	dat = dat*2 + 128;
	return  &Save_DataTab[dat];
}
/*******************************************************************************
* ��������: Clear_DataTab()
* ��������: Clear_DataTab,���һ��ʱ�������
* �������: 1��dat �����dat���ݱ� ��Χ 0-447
* ���ز���: ��
********************************************************************************/
void Clear_DataTab(uint16_t dat)
{
	if(dat>=DataTabItemNum) dat=0;
	dat = dat*2 + 128;
	Save_DataTab[0 + dat] = 0;
	Save_DataTab[1 + dat] = 0;
}

/*******************************************************************************
* ��������: Clear_TimeDataTab()
* ��������: Clear_TimeDataTab,�����n��ʱ��������������
* �������: 1��n ��n��ʱ���
* ���ز���: ��
********************************************************************************/
void Clear_TimeDataTab(uint16_t n)
{
	uint16_t i = 0;
	uint16_t Num = 0;
	uint16_t Dest = 0;
	
	Num = Get_TimeTab_num(n);
	Dest = Get_TimeTab_Dest(n);
	for(i=Dest; i<(Num+Dest); i++)
	{
		Clear_DataTab(i);	
	}
	Clear_TimeTab(n);
}

/*******************************************************************************
* ��������: MoveUp_TimeDataTab()
* ��������: MoveUp_TimeDataTab,�����n��ʱ��������������
* �������: 1��n ��n��ʱ���
* ���ز���: ��
********************************************************************************/
void MoveUp_TimeDataTab(uint16_t n)
{
	uint16_t Num = 0, i = 0;
	uint16_t Dest = 0;
	uint16_t temp = 0;
	uint16_t* ptime = 0;
	
	temp = n + 1;
	if(temp < TimeTabItemNum)
	{
		Num = Get_TimeTab_num(temp);
		//���´������ݣ����Ҳ������һλ����λ
		if(Num > 0)
		{
			Dest = Get_TimeTab_Dest(temp);
			ptime = Get_TimeTab_Time(temp);
			
			//��һ��ʱ�������ƶ�һλ
			temp = temp*5;
			Save_DataTab[0 + temp - 1] = ptime[0];//����
			Save_DataTab[1 + temp - 1] = ptime[1];//��ʱ
			Save_DataTab[2 + temp - 1] = ptime[2];//����
			Save_DataTab[3 + temp - 1] = Num;//Num
			Save_DataTab[4 + temp - 1] = Dest;//Dest
			
			//��һ�����������ƶ�Numλ
			for(i=Dest; i<(Num+Dest); i++)
			{
				if(i>=DataTabItemNum) i=0;
				i = i*2 + 128;
				Save_DataTab[0 + i - Num] = Save_DataTab[0 + i];
				Save_DataTab[1 + i - Num] = Save_DataTab[1 + i];
			}
			
			//�����һ����������
			Clear_TimeDataTab(temp);	
		}
	}
}


/*******************************************************************************
* ��������: DataTabHaveData()
* ��������: DataTabHaveData�����ݱ�λ���Ƿ���ֵ
* �������: 1��tcnt ��tcnt���������� ��Χ 0-447
* ���ز���: TRUE����ֵ����FALSE����ֵ��
********************************************************************************/
uint8_t DataTabHaveData(uint16_t dat)
{
	if(dat>=DataTabItemNum) dat=0;
	dat = dat*2 + 128;
	if((Save_DataTab[0 + dat] != 0)&&(Save_DataTab[1 + dat] != 0))
	{
		return TRUE;
	}
	else
	{
		return FALSE;	
	}
}


/*******************************************************************************
* ��������: SaveTabRead()
* ��������: SaveTabRead��
* �������: 
* ���ز���: 
********************************************************************************/
void SaveTabRead(void)
{
	uint16_t i = 0;
	bsp_ReadCpuFlash_HalfWord(SAVE_ADDR_Save_DataTab, Save_DataTab, 1024);//��ȡ�洢������
	if( (Save_DataTab[0]==0xffff)||(Save_DataTab[3]==0xffff)||(Save_DataTab[5]==0xffff)||(Save_DataTab[8]==0xffff)||
		(Save_DataTab[128]==0xffff)||(Save_DataTab[129]==0xffff)||(Save_DataTab[130]==0xffff)||(Save_DataTab[131]==0xffff) )
	{
		for(i=0;i<1024;i++)//ʱ�����������
		{
			Save_DataTab[i] = 0;
		}
		SaveTimeCnt = 0;
		bsp_WriteCpuFlash(SAVE_ADDR_SaveTimeCnt, &SaveTimeCnt, 1);	
		bsp_WriteCpuFlash(SAVE_ADDR_Save_DataTab, Save_DataTab, 1024); //������		
	}
///////////////////////////////////////////////////
////����
//	SaveTimeCnt = 24;
//	for(i = 0; i < 24; i++)
//	{
//		Add_TimeTab_time(i);
//		Add_TimeTab_num(i,3);
//		Add_TimeTab_Dest(i,(3*i));
//	}
//	for(i = 0; i < 440; i++)
//	{
//		Add_DataTab(i);
//	}
/////////////////////////////////////////////////////
//	for(i=0;i<1024;i++)
//	{
//		printf("0x%08X=%04X\r\n",SAVE_ADDR_Save_DataTab+(2*i),Save_DataTab[i]); //������
//	}

}

void PowerOffSave(void)
{
	GUI_Power_Off();
	bsp_WriteCpuFlash(SAVE_ADDR_SaveTimeCnt, &SaveTimeCnt, 1);	
	bsp_WriteCpuFlash(SAVE_ADDR_Save_DataTab, Save_DataTab, 1024); //������		
}
