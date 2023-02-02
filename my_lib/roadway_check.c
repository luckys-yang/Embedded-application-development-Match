#include "allhead.h"
#include "roadway_check.h"


uint8_t wheel_L_Flag = 0, wheel_R_Flag = 0;	//��ת��־λ����ת��־λ
uint8_t wheel_Nav_Flag = 0;	//������ת��־λ
uint8_t Go_Flag = 0, Back_Flag = 0;	//ǰ����־λ�����˱�־λ
uint8_t Track_Flag = 0;	//Ѱ����־λ
uint8_t Stop_Flag = 0;	//ֹͣ��־λ
uint8_t Line_Flag = 0;	//û�õ�
uint16_t count = 0;	//�෴��Ѱ�������õ��ļ���

extern uint8_t RFID_Flag;	// RFID����־λ
int LSpeed = 0, RSpeed = 0;	//���������ٶ� ���������ٶ�(��Ϊ�и���������uint16_t!!!)
int Car_Spend = 0;	//���ٶ���ʱ����(��������ʱ������)(��Ϊ�и���������uint16_t!!!)
uint16_t temp_MP = 0;	//����ֵ��ʱ����(��������ʱ������)
uint16_t temp_Nav = 0;	//������תֵ

uint32_t Wheel_flag = 0;	//ת�����ֵ

void Track_Correct(uint8_t gd);
void Back_Track(uint8_t gd);

//_________________________________________________________
int16_t Roadway_cmp;	//��������ֵ
extern int16_t CanHost_Mp;	//������ʱ����ֵ

/*
	����ͬ��
**/
void Roadway_mp_syn(void)
{
    Roadway_cmp = CanHost_Mp;
}

/*
	���̻�ȡ
**/
uint16_t Roadway_mp_Get(void)
{
    uint32_t ct;
    if(CanHost_Mp > Roadway_cmp)
        ct = CanHost_Mp - Roadway_cmp;
    else
        ct = Roadway_cmp - CanHost_Mp;
    if(ct > 0x8000)
        ct = 0xffff - ct;
    return ct;
}

uint16_t Roadway_Navig;
extern uint16_t CanHost_Navig;

/*
	�Ƕ�ͬ��
**/
void Roadway_nav_syn(void)
{
    Roadway_Navig = CanHost_Navig;
}

/*
	��ȡ�ǶȲ�ֵ
**/
uint16_t Roadway_nav_Get(void)
{
    uint16_t ct;
    if(CanHost_Navig > Roadway_Navig)
        ct = CanHost_Navig - Roadway_Navig;
    else
        ct = Roadway_Navig - CanHost_Navig;
    while(ct >= 36000)
        ct -= 36000;
    return ct;
}

//_______________________________________________________________
//���С�����еı�־λ
void Roadway_Flag_clean(void)
{
    wheel_L_Flag = 0;
    wheel_R_Flag = 0;
    Go_Flag = 0;
    Back_Flag = 0;
    Track_Flag = 0;
    Stop_Flag = 0;
    //	temp_MP = 0;
}

/**
	ǰ��/���˼��
*/
void Go_and_Back_Check(void)
{
    if(Go_Flag == 1)
    {
        if(temp_MP <= Roadway_mp_Get())
        {
            Go_Flag = 0;
            Stop_Flag = 3;
            Send_UpMotor(0, 0);		// ֹͣ
        }
    }
    else if(Back_Flag == 1)
    {
        if(temp_MP <= Roadway_mp_Get())
        {
            Back_Flag = 0;
            Stop_Flag = 3;
            Send_UpMotor(0, 0);
        }
    }
}

//���б�־λ�Ƿ�Ϊ0���(����ֹͣ��־λ����Ҫ�ж�)
uint8_t Roadway_GoBack_Check(void)
{
    return ((Go_Flag == 0) && (Back_Flag == 0) && (Track_Flag == 0) && (wheel_L_Flag == 0) && (wheel_R_Flag == 0)) ? 1 : 0;
}

/**
	����ת��
*/
void wheel_Nav_check(void)
{
    uint16_t Mp_Value = 0;

    if(wheel_Nav_Flag)
    {
        Mp_Value = Roadway_mp_Get();
        if(Mp_Value >= temp_Nav)
        {
            wheel_Nav_Flag = 0;
            Stop_Flag = 2;
            Send_UpMotor(0, 0);
        }
    }
}


/**
	����ѭ����ת�䣨��ת������ʵ�֣�
	�����Ĭ������ת���򶯵�
*/
//1����---���� 0����---����
uint32_t Mp_Value = 0;
void wheel_Track_check(void)
{
    uint16_t Track_Value = 0;

    if(wheel_L_Flag == 1)	//��ת
    {
        Track_Value = Get_Host_UpTrack(TRACK_H8);	//��ȡ������ѭ������ ����ֵ--Rt ѭ������
        if(!(Track_Value & 0X10))		// �ҵ�ѭ���ߣ�ֹͣ
        {
            if(Wheel_flag > 50)	//����������������������else if��һֱ++50
            {
                wheel_L_Flag = 0;
                Wheel_flag = 0;
                Stop_Flag = 2;
                Send_UpMotor(0, 0);	//���ֹͣת��
            }
        }
        else if(Track_Value == 0Xff)	// ѭ����ȫ��(�Ⱦ���ȫ���ⲽ��ִ�������ǲ�)
        {
            Wheel_flag++;
        }
    }
    else if(wheel_R_Flag == 1)	//��ת
    {
        Track_Value = Get_Host_UpTrack(TRACK_H8);

        if(!(Track_Value & 0X08)) //�ҵ�ѭ���ߣ�ֹͣ
        {
            if(Wheel_flag > 50)	//50
            {
                wheel_R_Flag = 0;
                Wheel_flag = 0;
                Stop_Flag = 2;
                Send_UpMotor(0, 0);
            }
        }
        else if( Track_Value == 0Xff)
        {
            Wheel_flag++;
        }
    }
}


/**
	ѭ����⣨Ѱ��������ʵ�֣�
*/
void Track_Check()
{
    if(Track_Flag == 1)	//Ѱ����������ͣ
    {
        Track_Correct(Get_Host_UpTrack(TRACK_H8));	//��ȡѰ������(��Ѱ���Ƶ�״̬)����ȥ��ȫ����ͣ��
    }	
	if(Yang_Track_Flag == 1)	//Ѱ����������ͣ���Լ���ӵģ�
	{
		Yang_Track_Correct_White(Get_Host_UpTrack(TRACK_H8));	//��ȡѰ������(��Ѱ���Ƶ�״̬)����ȥ��ȫ����ͣ��
	}
}

void Roadway_Check(void)
{
    Go_and_Back_Check();	//ǰ��/���˼��
    wheel_Track_check();	//����ѭ����ת����
    wheel_Nav_check();	//����ת����
    Track_Check();	//ѭ�����
}



/***************************************************************
** ���ܣ�     ������ƺ���
** ������	  L_Spend����������ٶ�
**            R_Spend����������ٶ�
** ����ֵ��   ��
�ٶȱ�д���� ��ת�����---100 ��С��----5  ��ת�ٶ���С��--- -5 �ٶ������--- -100
****************************************************************/
//���μ���(����0��ֱ�Ӳ���Send_UpMotor����)
//void Control(int L_Spend, int R_Spend)
//{
//	//����
//    if(L_Spend > 0)
//    {
//        if(L_Spend > 100)L_Spend = 100;
//        if(L_Spend < 5)L_Spend = 5;		//�����ٶȲ���
//    }
//    else if(L_Spend < 0)
//    {
//        if(L_Spend < -100)L_Spend = -100;
//        if(L_Spend > -5)L_Spend = -5;  //�����ٶȲ���
//    }
//	else if(L_Spend == 0)
//	{
//		L_Spend = 0;
//	}
//	//����
//    if(R_Spend > 0)
//    {
//        if(R_Spend > 100)R_Spend = 100;
//        if(R_Spend < 5)R_Spend = 5;		//�����ٶȲ���
//    }
//    else if(R_Spend < 0)
//    {
//        if(R_Spend < -100)R_Spend = -100;
//        if(R_Spend > -5)R_Spend = -5;		//�����ٶȲ���
//    }
//	else if(R_Spend == 0)
//	{
//		R_Spend = 0;
//	}
//    Send_UpMotor(L_Spend , R_Spend);
//}
//ԭ��
void Control(int L_Spend, int R_Spend)
{
    if(L_Spend >= 0)
    {
        if(L_Spend > 100)L_Spend = 100;
        if(L_Spend < 5)L_Spend = 5;		//�����ٶȲ���
    }
    else
    {
        if(L_Spend < -100)L_Spend = -100;
        if(L_Spend > -5)L_Spend = -5;  //�����ٶȲ���
    }
    if(R_Spend >= 0)
    {
        if(R_Spend > 100)R_Spend = 100;
        if(R_Spend < 5)R_Spend = 5;		//�����ٶȲ���
    }
    else
    {
        if(R_Spend < -100)R_Spend = -100;
        if(R_Spend > -5)R_Spend = -5;		//�����ٶȲ���
    }
    Send_UpMotor(L_Spend , R_Spend);
}

extern uint8_t RFID_Flag;
extern uint8_t Terrain_Flag;
/***************************************************************
** ���ܣ�     ѭ������
** ������	  �޲���
** ����ֵ��   ��
1����---���� 0����---����
****************************************************************/
void Track_Correct(uint8_t gd)
{
    if(gd == 0x00)	// ѭ����ȫ�� ֹͣ
    {
        Track_Flag = 0;
        Stop_Flag = 1;	//ֹͣ��־λ��1
        Send_UpMotor(0, 0);

    }
    else if(gd == 0xE7)	//E7---1110 0111��������ʻ��
    {
        LSpeed = Car_Spend;
        RSpeed = Car_Spend;
    }
    else if((gd == 0xF7) || (gd == 0xF3))	//F7---1111 0111 F3---1111 0011��ƫ��
    {
		//���ּ��٣����ּ���
        LSpeed = Car_Spend + 20;	
        RSpeed = Car_Spend - 40;
    }
    else if((gd == 0xFB) || (gd == 0xF9))	//FB---1111 1011 F9---1111 1001��ƫƫ��
    {
		//���ּ��٣����ּ���
        LSpeed = Car_Spend + 40;
        RSpeed = Car_Spend - 60;
    }
    else if((gd == 0xFD) || (gd == 0xFC))	//FD---1111 1101 FC---1111 1100��ƫƫƫ��
    {
		//���ּ��٣����ּ���
        LSpeed = Car_Spend + 60;
        RSpeed = Car_Spend - 90;
    }
    else if(gd == 0xFE)	//FE---1111 1110��ƫƫƫƫ��
    {
		//���ּ��٣����ּ���
        LSpeed = Car_Spend + 80;
        RSpeed = Car_Spend - 120;
    }
    else if((gd == 0xEF) || (gd == 0xCF))	//EF---1110 1111 CF---1100 1111��ƫ�ң�
    {
		//���ּ��٣����ּ���
        RSpeed = Car_Spend + 20;
        LSpeed = Car_Spend - 40;
    }
    else if((gd == 0xDF) || (gd == 0x9F))	//DF---1101 1111 9F---1001 1111��ƫƫ�ң�
    {
		//���ּ��٣����ּ���		
        RSpeed = Car_Spend + 40;
        LSpeed = Car_Spend - 60;
    }
    else if((gd == 0xBF) || (gd == 0x3F))	//BF---1011 1111 3F---0011 1111��ƫƫƫ�ң�
    {
		//���ּ��٣����ּ���		
        RSpeed = Car_Spend + 60;
        LSpeed = Car_Spend - 90;
    }
    else if(gd == 0x7F)	//7F---0111 1111��ƫƫƫƫ�ң�
    {
		//���ּ��٣����ּ���		
        RSpeed = Car_Spend + 80;
        LSpeed = Car_Spend - 120;
    }
    else	//������������ٶ�
    {
        LSpeed = Car_Spend;
        RSpeed = Car_Spend;
    }
    if(RFID_Flag == 1)	//RFID·��
    {
        if(((gd & 0x18) == 0x18) && (((gd & 0x80) == 0x00) || ((gd & 0x01) == 0x00)))	//18---0001 1000 80---1000 0000 01---0000 0001
        {
            Track_Flag = 0;
            Stop_Flag = 1;
            Send_UpMotor(0, 0);
        }
    }
    if(gd == 0xFF) //ѭ����ȫ��	FF---1111 1111
    {
        LSpeed = Car_Spend;
        RSpeed = Car_Spend;
        if(count > 1200)
        {
            count = 0;
            Send_UpMotor(0, 0);
            Track_Flag = 0;
            Stop_Flag = 4;
        }
        else
        {
            count++;
        }
    }
    else
    {
        count = 0;
    }

    if(Track_Flag != 0)
    {
        Control(LSpeed, RSpeed);
    }
}

/***************************************************************
** ���ܣ�     ѭ������
** ������	  �޲���
** ����ֵ��   ��
����������෴
1---���� 0---����
****************************************************************/
void Track_Roadway(uint8_t gd)
{
    if(gd == 0x00)	// ѭ����ȫ�� ֹͣ
    {
        Track_Flag = 0;
        Stop_Flag = 1;
        Send_UpMotor(0, 0);

    }
    else if(gd == 0x18)	//18--- 0001 1000
    {
        LSpeed = Car_Spend;
        RSpeed = Car_Spend;

    }
    else if((gd == 0x08) || (gd == 0x0C))	//08--- 0000 1000 0C--- 0000 1100
    {
        LSpeed = Car_Spend + 20;
        RSpeed = Car_Spend - 40;
    }
    else if((gd == 0x04) || (gd == 0x06))	//04--- 0000 0100 06--- 0000 0110
    {
        LSpeed = Car_Spend + 40;
        RSpeed = Car_Spend - 60;
    }
    else if((gd == 0x02) || (gd == 0x03))	//02--- 0000 0010 03--- 0000 0011
    {
        LSpeed = Car_Spend + 60;
        RSpeed = Car_Spend - 90;

    }
    else if(gd == 0x01)	//01--- 0000 0001
    {
        LSpeed = Car_Spend + 80;
        RSpeed = Car_Spend - 120;

    }
    else if((gd == 0x10) || (gd == 0x30))	//10--- 0001 0000 30--- 0011 0000
    {
        RSpeed = Car_Spend + 20;
        LSpeed = Car_Spend - 40;

    }
    else if((gd == 0x20) || (gd == 0x60))	//20--- 0010 0000 60--- 0110 0000
    {
        RSpeed = Car_Spend + 40;
        LSpeed = Car_Spend - 60;

    }
    else if((gd == 0x40) || (gd == 0xC0))	//40--- 0100 0000 C0--- 1100 0000
    {
        RSpeed = Car_Spend + 60;
        LSpeed = Car_Spend - 90;

    }
    else if(gd == 0X80)	//80--- 1000 0000
    {
        RSpeed = Car_Spend + 80;
        LSpeed = Car_Spend - 120;

    }
    else
    {
        LSpeed = Car_Spend;
        RSpeed = Car_Spend;
    }

    if(gd == 0xFF) //ѭ����ȫ��
    {
        if(count > 1000)
        {
            count = 0;
            Send_UpMotor(0, 0);
            Track_Flag = 0;
            Stop_Flag = 4;
        }
        else
        {
            count++;
        }
    }
    else
    {
        count = 0;
    }

    if(Track_Flag != 0)
    {
        Control(LSpeed, RSpeed);
    }
}
//TIM9��ʱ����ʼ��
//Ƶ��2ms
void roadway_check_TimInit(uint16_t arr, uint16_t psc)
{
    TIM_TimeBaseInitTypeDef TIM_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);

    TIM_InitStructure.TIM_Period = arr;
    TIM_InitStructure.TIM_Prescaler = psc;
    TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_InitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM9, &TIM_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ITConfig(TIM9, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM9, ENABLE);
}
//��ʱ���жϷ��ʺ���
void TIM1_BRK_TIM9_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM9, TIM_IT_Update) == SET)
    {
        Roadway_Check();		// ·�����
    }
    TIM_ClearITPendingBit(TIM9, TIM_IT_Update);
}


//end file
