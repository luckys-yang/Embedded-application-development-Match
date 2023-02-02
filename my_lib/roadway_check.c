#include "allhead.h"
#include "roadway_check.h"


uint8_t wheel_L_Flag = 0, wheel_R_Flag = 0;	//左转标志位，右转标志位
uint8_t wheel_Nav_Flag = 0;	//码盘旋转标志位
uint8_t Go_Flag = 0, Back_Flag = 0;	//前进标志位，后退标志位
uint8_t Track_Flag = 0;	//寻迹标志位
uint8_t Stop_Flag = 0;	//停止标志位
uint8_t Line_Flag = 0;	//没用到
uint16_t count = 0;	//相反的寻迹函数用到的计数

extern uint8_t RFID_Flag;	// RFID检测标志位
int LSpeed = 0, RSpeed = 0;	//最终左轮速度 最终右轮速度(因为有负数不能用uint16_t!!!)
int Car_Spend = 0;	//车速度临时变量(函数传参时赋给它)(因为有负数不能用uint16_t!!!)
uint16_t temp_MP = 0;	//码盘值临时变量(函数传参时赋给它)
uint16_t temp_Nav = 0;	//码盘旋转值

uint32_t Wheel_flag = 0;	//转弯计数值

void Track_Correct(uint8_t gd);
void Back_Track(uint8_t gd);

//_________________________________________________________
int16_t Roadway_cmp;	//最终码盘值
extern int16_t CanHost_Mp;	//码盘临时变量值

/*
	码盘同步
**/
void Roadway_mp_syn(void)
{
    Roadway_cmp = CanHost_Mp;
}

/*
	码盘获取
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
	角度同步
**/
void Roadway_nav_syn(void)
{
    Roadway_Navig = CanHost_Navig;
}

/*
	获取角度差值
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
//清除小车所有的标志位
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
	前进/后退监测
*/
void Go_and_Back_Check(void)
{
    if(Go_Flag == 1)
    {
        if(temp_MP <= Roadway_mp_Get())
        {
            Go_Flag = 0;
            Stop_Flag = 3;
            Send_UpMotor(0, 0);		// 停止
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

//所有标志位是否为0检测(除了停止标志位不需要判断)
uint8_t Roadway_GoBack_Check(void)
{
    return ((Go_Flag == 0) && (Back_Flag == 0) && (Track_Flag == 0) && (wheel_L_Flag == 0) && (wheel_R_Flag == 0)) ? 1 : 0;
}

/**
	码盘转弯
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
	根据循迹线转弯（左转是在这实现）
	电机是默认是左转方向动的
*/
//1白线---灯亮 0黑线---灯灭
uint32_t Mp_Value = 0;
void wheel_Track_check(void)
{
    uint16_t Track_Value = 0;

    if(wheel_L_Flag == 1)	//左转
    {
        Track_Value = Get_Host_UpTrack(TRACK_H8);	//获取、处理循迹数据 返回值--Rt 循迹数据
        if(!(Track_Value & 0X10))		// 找到循迹线，停止
        {
            if(Wheel_flag > 50)	//如果不大于这个数则在下面else if那一直++50
            {
                wheel_L_Flag = 0;
                Wheel_flag = 0;
                Stop_Flag = 2;
                Send_UpMotor(0, 0);	//电机停止转动
            }
        }
        else if(Track_Value == 0Xff)	// 循迹灯全亮(先经过全亮这步才执行上面那步)
        {
            Wheel_flag++;
        }
    }
    else if(wheel_R_Flag == 1)	//右转
    {
        Track_Value = Get_Host_UpTrack(TRACK_H8);

        if(!(Track_Value & 0X08)) //找到循迹线，停止
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
	循迹监测（寻迹是在这实现）
*/
void Track_Check()
{
    if(Track_Flag == 1)	//寻迹到黑线则停
    {
        Track_Correct(Get_Host_UpTrack(TRACK_H8));	//获取寻迹数据(即寻迹灯的状态)传过去（全黑则停）
    }	
	if(Yang_Track_Flag == 1)	//寻迹到白线则停（自己添加的）
	{
		Yang_Track_Correct_White(Get_Host_UpTrack(TRACK_H8));	//获取寻迹数据(即寻迹灯的状态)传过去（全白则停）
	}
}

void Roadway_Check(void)
{
    Go_and_Back_Check();	//前进/后退检测
    wheel_Track_check();	//根据循迹线转弯检测
    wheel_Nav_check();	//码盘转弯检测
    Track_Check();	//循迹监测
}



/***************************************************************
** 功能：     电机控制函数
** 参数：	  L_Spend：电机左轮速度
**            R_Spend：电机右轮速度
** 返回值：   无
速度被写死了 正转最大是---100 最小是----5  反转速度最小是--- -5 速度最大是--- -100
****************************************************************/
//屏蔽即可(对于0可直接操作Send_UpMotor函数)
//void Control(int L_Spend, int R_Spend)
//{
//	//左轮
//    if(L_Spend > 0)
//    {
//        if(L_Spend > 100)L_Spend = 100;
//        if(L_Spend < 5)L_Spend = 5;		//限制速度参数
//    }
//    else if(L_Spend < 0)
//    {
//        if(L_Spend < -100)L_Spend = -100;
//        if(L_Spend > -5)L_Spend = -5;  //限制速度参数
//    }
//	else if(L_Spend == 0)
//	{
//		L_Spend = 0;
//	}
//	//右轮
//    if(R_Spend > 0)
//    {
//        if(R_Spend > 100)R_Spend = 100;
//        if(R_Spend < 5)R_Spend = 5;		//限制速度参数
//    }
//    else if(R_Spend < 0)
//    {
//        if(R_Spend < -100)R_Spend = -100;
//        if(R_Spend > -5)R_Spend = -5;		//限制速度参数
//    }
//	else if(R_Spend == 0)
//	{
//		R_Spend = 0;
//	}
//    Send_UpMotor(L_Spend , R_Spend);
//}
//原版
void Control(int L_Spend, int R_Spend)
{
    if(L_Spend >= 0)
    {
        if(L_Spend > 100)L_Spend = 100;
        if(L_Spend < 5)L_Spend = 5;		//限制速度参数
    }
    else
    {
        if(L_Spend < -100)L_Spend = -100;
        if(L_Spend > -5)L_Spend = -5;  //限制速度参数
    }
    if(R_Spend >= 0)
    {
        if(R_Spend > 100)R_Spend = 100;
        if(R_Spend < 5)R_Spend = 5;		//限制速度参数
    }
    else
    {
        if(R_Spend < -100)R_Spend = -100;
        if(R_Spend > -5)R_Spend = -5;		//限制速度参数
    }
    Send_UpMotor(L_Spend , R_Spend);
}

extern uint8_t RFID_Flag;
extern uint8_t Terrain_Flag;
/***************************************************************
** 功能：     循迹函数
** 参数：	  无参数
** 返回值：   无
1白线---灯亮 0黑线---灯灭
****************************************************************/
void Track_Correct(uint8_t gd)
{
    if(gd == 0x00)	// 循迹灯全灭 停止
    {
        Track_Flag = 0;
        Stop_Flag = 1;	//停止标志位置1
        Send_UpMotor(0, 0);

    }
    else if(gd == 0xE7)	//E7---1110 0111（正常行驶）
    {
        LSpeed = Car_Spend;
        RSpeed = Car_Spend;
    }
    else if((gd == 0xF7) || (gd == 0xF3))	//F7---1111 0111 F3---1111 0011（偏左）
    {
		//左轮加速，右轮减速
        LSpeed = Car_Spend + 20;	
        RSpeed = Car_Spend - 40;
    }
    else if((gd == 0xFB) || (gd == 0xF9))	//FB---1111 1011 F9---1111 1001（偏偏左）
    {
		//左轮加速，右轮减速
        LSpeed = Car_Spend + 40;
        RSpeed = Car_Spend - 60;
    }
    else if((gd == 0xFD) || (gd == 0xFC))	//FD---1111 1101 FC---1111 1100（偏偏偏左）
    {
		//左轮加速，右轮减速
        LSpeed = Car_Spend + 60;
        RSpeed = Car_Spend - 90;
    }
    else if(gd == 0xFE)	//FE---1111 1110（偏偏偏偏左）
    {
		//左轮加速，右轮减速
        LSpeed = Car_Spend + 80;
        RSpeed = Car_Spend - 120;
    }
    else if((gd == 0xEF) || (gd == 0xCF))	//EF---1110 1111 CF---1100 1111（偏右）
    {
		//右轮加速，左轮减速
        RSpeed = Car_Spend + 20;
        LSpeed = Car_Spend - 40;
    }
    else if((gd == 0xDF) || (gd == 0x9F))	//DF---1101 1111 9F---1001 1111（偏偏右）
    {
		//右轮加速，左轮减速		
        RSpeed = Car_Spend + 40;
        LSpeed = Car_Spend - 60;
    }
    else if((gd == 0xBF) || (gd == 0x3F))	//BF---1011 1111 3F---0011 1111（偏偏偏右）
    {
		//右轮加速，左轮减速		
        RSpeed = Car_Spend + 60;
        LSpeed = Car_Spend - 90;
    }
    else if(gd == 0x7F)	//7F---0111 1111（偏偏偏偏右）
    {
		//右轮加速，左轮减速		
        RSpeed = Car_Spend + 80;
        LSpeed = Car_Spend - 120;
    }
    else	//其余情况正常速度
    {
        LSpeed = Car_Spend;
        RSpeed = Car_Spend;
    }
    if(RFID_Flag == 1)	//RFID路线
    {
        if(((gd & 0x18) == 0x18) && (((gd & 0x80) == 0x00) || ((gd & 0x01) == 0x00)))	//18---0001 1000 80---1000 0000 01---0000 0001
        {
            Track_Flag = 0;
            Stop_Flag = 1;
            Send_UpMotor(0, 0);
        }
    }
    if(gd == 0xFF) //循迹灯全亮	FF---1111 1111
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
** 功能：     循迹函数
** 参数：	  无参数
** 返回值：   无
这个跟上面相反
1---灯灭 0---灯亮
****************************************************************/
void Track_Roadway(uint8_t gd)
{
    if(gd == 0x00)	// 循迹灯全灭 停止
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

    if(gd == 0xFF) //循迹灯全亮
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
//TIM9定时器初始化
//频率2ms
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
//定时器中断访问函数
void TIM1_BRK_TIM9_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM9, TIM_IT_Update) == SET)
    {
        Roadway_Check();		// 路况检测
    }
    TIM_ClearITPendingBit(TIM9, TIM_IT_Update);
}


//end file
