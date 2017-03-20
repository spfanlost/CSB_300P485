#ifndef __key
#define __key 

#define KEY_COUNT    7	   					/* �������� */

/* ����Ӧ�ó���Ĺ��������������� */
#define KEY_DOWN_K1		KEY_1_DOWN		/*ESC*/
#define KEY_UP_K1			KEY_1_UP
#define KEY_LONG_K1		KEY_1_LONG

#define KEY_DOWN_K2		KEY_2_DOWN		/*OK*/
#define KEY_UP_K2			KEY_2_UP
#define KEY_LONG_K2		KEY_2_LONG

#define JOY_DOWN_U		KEY_3_DOWN		/* �� */
#define JOY_UP_U			KEY_3_UP
#define JOY_LONG_U		KEY_3_LONG

#define JOY_DOWN_D		KEY_4_DOWN		/* �� */
#define JOY_UP_D			KEY_4_UP
#define JOY_LONG_D		KEY_4_LONG

#define JOY_DOWN_L		KEY_5_DOWN		/* �� */
#define JOY_UP_L			KEY_5_UP
#define JOY_LONG_L		KEY_5_LONG

#define JOY_DOWN_R		KEY_6_DOWN		/* �� */
#define JOY_UP_R			KEY_6_UP
#define JOY_LONG_R		KEY_6_LONG


#define KEY_DOWN_K1D	KEY_9_DOWN		/* K1 K2 ��ϼ� */
#define KEY_UP_K1D	    KEY_9_UP
#define KEY_LONG_K1D	KEY_9_LONG

/* ����ID, ��Ҫ����bsp_KeyState()��������ڲ��� */
typedef enum
{
	KID_ESC = 0,
	KID_OK,
	KID_U,
	KID_D,
	KID_L,
	KID_R,
}KEY_ID_E;

/*
	�����˲�ʱ��50ms, ��λ10ms��
	ֻ��������⵽50ms״̬�������Ϊ��Ч����������Ͱ��������¼�
	��ʹ������·����Ӳ���˲������˲�����Ҳ���Ա�֤�ɿ��ؼ�⵽�����¼�
*/
#define KEY_FILTER_TIME   5
#define KEY_LONG_TIME     100			/* ��λ20ms�� ���� �룬��Ϊ�����¼� */

/*
	�����ֵ����, ���밴���´���ʱÿ�����İ��¡�����ͳ����¼�

	�Ƽ�ʹ��enum, ����#define��ԭ��
	(1) ����������ֵ,�������˳��ʹ���뿴���������
	(2) �������ɰ����Ǳ����ֵ�ظ���
*/
typedef enum
{
	KEY_NONE = 0,			/* 0 ��ʾ�����¼� */

	KEY_1_DOWN,				/* 1������ */
	KEY_1_UP,					/* 1������ */
	KEY_1_LONG,				/* 1������ */

	KEY_2_DOWN,				/* 2������ */
	KEY_2_UP,					/* 2������ */
	KEY_2_LONG,				/* 2������ */

	KEY_3_DOWN,				/* 3������ */
	KEY_3_UP,					/* 3������ */
	KEY_3_LONG,				/* 3������ */

	KEY_4_DOWN,				/* 4������ */
	KEY_4_UP,					/* 4������ */
	KEY_4_LONG,				/* 4������ */

	KEY_5_DOWN,				/* 5������ */
	KEY_5_UP,					/* 5������ */
	KEY_5_LONG,				/* 5������ */

	KEY_6_DOWN,				/* 6������ */
	KEY_6_UP,					/* 6������ */
	KEY_6_LONG,				/* 6������ */

	/* ��ϼ� */
	KEY_9_DOWN,				/* 9������ */
	KEY_9_UP,				/* 9������ */
	KEY_9_LONG,				/* 9������ */

}KEY_ENUM;

/*
	ÿ��������Ӧ1��ȫ�ֵĽṹ�������
*/
typedef struct
{
	/* ������һ������ָ�룬ָ���жϰ����ַ��µĺ��� */
	uint8_t (*IsKeyDownFunc)(void); /* �������µ��жϺ���,1��ʾ���� */

	uint8_t  Count;			/* �˲��������� */
	uint16_t LongCount;		/* ���������� */
	uint16_t LongTime;		/* �������³���ʱ��, 0��ʾ����ⳤ�� */
	uint8_t  State;			/* ������ǰ״̬�����»��ǵ��� */
	uint8_t  RepeatSpeed;	/* ������������ */
	uint8_t  RepeatCount;	/* �������������� */
}KEY_T;

/* ����FIFO�õ����� */
#define KEY_FIFO_SIZE	6
typedef struct
{
	uint8_t Buf[KEY_FIFO_SIZE];		/* ��ֵ������ */
	uint8_t Read;					/* ��������ָ��1 */
	uint8_t Write;					/* ������дָ�� */
	uint8_t Read2;					/* ��������ָ��2 */
}KEY_FIFO_T;

/* ���ⲿ���õĺ������� */
void bsp_Init_Key(void);
void bsp_KeyScan(void);
void bsp_PutKey(uint8_t _KeyCode);
uint8_t bsp_GetKey(void);
uint8_t bsp_GetKey2(void);
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID);
void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed);
void bsp_ClearKey(void);

void bsp_Power_init(void);
void bsp_OnOffKey_init(void);
void GUI_KeyTask(void);
void BacklightOn(void);
void BacklightOff(void);

#endif
