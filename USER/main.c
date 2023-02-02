#include "allhead.h"

/******** 宏定义区 *********/
#define debug_A72 0	//A72屏蔽选择


/******** 全局静态变量区 *********/
static uint32_t Power_check_times;          // 电量检测周期
static uint32_t LED_twinkle_times;          // LED闪烁周期
static uint32_t WIFI_Upload_data_times;     // 通过Wifi上传数据周期
static uint32_t RFID_Init_Check_times;      // RFID初始化检测时间周期
static uint32_t task_times;                 // 任务时间检测时间周期

/******** 全局变量区 *********/
uint8_t RFID_Flag = 0;          	           // RFID检测标志位
uint16_t distance = 0;                      // 记录超声波数据
uint8_t gear_init = 0;                      // 智能路灯初始档位
uint8_t coordinate;                         // 随机坐标点
uint8_t number = 0;                         // 计数值
uint8_t make = 0;                           // 全自动驾驶标志位
uint16_t random;                            // 随机数

uint8_t  Go_Speed  = 40;                    // 全局行进速度值
uint8_t  wheel_Speed = 90;                  // 全局转弯速度值
uint8_t  RFID_Go_Speed  = 35;               // RFID路段的速度根据新车、旧车自行调节
uint16_t Go_Temp = 260;                     // 全局前进码盘值
uint16_t wheel_Time = 330;                  // 全局转45°时间


uint8_t Stere1[6]={0xFF,0x14,0x01,0x00,0x00,0x00};		// 立体显示 显示 前方学校，减速慢行
uint8_t Stere2[6]={0xFF,0x14,0x02,0x00,0x00,0x00};		// 立体显示 显示 前方学校，减速慢行
uint8_t Stere3[6]={0xFF,0x14,0x03,0x00,0x00,0x00};		// 立体显示 显示 前方学校，减速慢行
uint8_t Stere4[6]={0xFF,0x14,0x04,0x00,0x00,0x00};		// 立体显示 显示 前方学校，减速慢行

uint8_t data1[3]={0x55,0xA1,0x01};		//告诉安卓识别二维码
uint8_t data2[3]={0x55,0xA1,0x02};		//告诉安卓识别二维码
uint8_t data3[3]={0x55,0xA1,0x03};		//告诉安卓识别二维码
uint8_t data4[3]={0x55,0xA1,0x04};		//告诉安卓识别二维码
uint8_t data5[3]={0x55,0xA1,0x10};		//告诉安卓识别二维码
uint8_t data6[3]={0x55,0xA1,0x11};		//告诉安卓识别二维码

//uint8_t data3[3]={0x55,0xA1,0x10};		//告诉安卓识别二维码

/******** 全局静态函数区 *********/
static void KEY_Check(void);                // 按键检测函数
static void Hardware_Init(void);            // 硬件初始化函数


uint8_t AutoFlag = 0;//全自动执行开始标志位
/* 全自动运行函数 */
void Car_Thread(void)
{
	switch(make)
	{
      case 0x01:
      {
          //主车启动
           Send_ZigbeeData_To_Fifo(GarageA_To1, 8);		//立体车库降至1层
           delay_ms(500);
           Send_ZigbeeData_To_Fifo(GarageA_To1, 8);		//立体车库降至1层
           delay_ms(500);
           Send_ZigbeeData_To_Fifo(SEG_TimOpen, 8);		//立体显示开始计时
           delay_ms(500);
           Send_ZigbeeData_To_Fifo(SEG_TimOpen, 8);		//立体显示开始计时
           delay_ms(500);
           make = 0x02;
         break;
      }
      case 0x02:
      {
          //超声波测距
           Car_Track(Go_Speed);    //调整车
           Car_Go(Go_Speed,Go_Temp);
           delay_ms(200);
           Car_Left(wheel_Speed);
           delay_ms(200);
           Car_Back(Go_Speed-10,1000);
           delay_ms(500);
           Car_Track(Go_Speed);
           delay_ms(500);
           Car_Back(Go_Speed-10,600);
           delay_ms(500);
           Car_Track(Go_Speed);
           delay_ms(500);
           Car_Back(Go_Speed-10,600);
           delay_ms(500);
           Car_Track(30);
           delay_ms(500);
           Car_Back(Go_Speed-10,350);
           delay_ms(500);
           Car_Track(20);
           delay_ms(500);
           Ultrasonic_Ranging();   //获取超声波数据
           distance = dis;         //记录超声波数据
           delay_ms(200);  
           LED_Dis_Zigbee(distance);
           delay_ms(500);  
           Car_Go(Go_Speed,Go_Temp);
           delay_ms(200);
           Car_Right(wheel_Speed);
           make=0x03;
         break;
      }
      case 0x03:
      {
          //主车二维码识别
           Car_R45(wheel_Speed,wheel_Time);
           delay_ms(500);
           delay_ms(500);
           delay_ms(500);
           delay_ms(500);
           delay_ms(500);
           delay_ms(500);
           
           Car_Right(wheel_Speed);
           delay_ms(200);
           
           Car_Track(Go_Speed);    
           Car_Go(Go_Speed,Go_Temp);
           delay_ms(200);//已行进至D6处
           make=0x04;
         break;
      }
      case 0x04:
      {
          //通过ETC系统
           ETC_Get_Zigbee();//查询ETC系统是否打开
           Car_Track(Go_Speed);
           Car_Go(Go_Speed,Go_Temp);
           delay_ms(200);
           make=0x05;
         break;
      }
      case 0x05:
      {  
         //智能路灯调节至2挡
           gear_init = Light_Inf(2);
           delay_ms(500);
           
           Car_Left(wheel_Speed);
           delay_ms(200);
           make=0x06;
         break;
      }
      case 0x06:
      {
          //交通灯A识别
           Send_ZigbeeData_To_Fifo(TrafficA_Open, 8);		// 通过发送ZigBee数据让交通灯进入识别模式
           delay_ms(500);
           delay_ms(500);
           delay_ms(500);
           delay_ms(500);
           delay_ms(500);
           delay_ms(500);
           Send_ZigbeeData_To_Fifo(TrafficA_Green, 8);		// 发送绿色确认
           delay_ms(500);
           Car_Track(Go_Speed);
           Car_Go(Go_Speed,Go_Temp);
           delay_ms(200);
           Car_Right(wheel_Speed);
           delay_ms(200);
           make=0x07;
         break;
      }
      case 0x07:
      {
            //智能TFT显示标志物A翻页
           Send_ZigbeeData_To_Fifo(TFT_PageDown, 8);		   // TFT翻页
           for(int a=0;a<7;a++)
           {
              delay_ms(500);
           }
           Send_ZigbeeData_To_Fifo(TFT_PageDown, 8);		   // TFT翻页
           for(int a=0;a<7;a++)
           {
              delay_ms(500);
           }
           Send_ZigbeeData_To_Fifo(TFT_PageDown, 8);		   // TFT翻页
           for(int a=0;a<7;a++)
           {
              delay_ms(500);
           }
           //通过特殊地形标志物
           Car_Left(wheel_Speed);
           Car_Left(wheel_Speed);
           delay_ms(200);
           Car_Track(Go_Speed);   //检测到特殊地形
           Car_Go(Go_Speed+10,900);  //通过特殊地形
           Car_Track(Go_Speed);
           Car_Go(Go_Speed,Go_Temp);
           delay_ms(200);
           make=0x08;
         break;
      }
      case 0x08:
      {
         //语音识别任务
           BKRC_Voice_Extern(0);
           
           Car_Right(wheel_Speed);
           delay_ms(200);
           
           //打开道闸
           Send_ZigbeeData_To_Fifo(Gate_Open, 8);		   // 打开道闸
           delay_ms(500);
           Send_ZigbeeData_To_Fifo(Gate_Open, 8);		   // 打开道闸
           delay_ms(500);
           delay_ms(500);
           
           Car_Track(Go_Speed);
           Car_Go(Go_Speed,Go_Temp);
           delay_ms(200);
           
           Car_Right(wheel_Speed);
           delay_ms(200);
           
           //开启报警台
           Car_R45(wheel_Speed,wheel_Time);
           delay_ms(500);
           Infrared_Send(Alarm_Open,6);   //通过红外发送数据开启烽火台
           delay_ms(500);
           delay_ms(500);
           
           Car_Left(wheel_Speed);
           delay_ms(200);
           Car_Track(Go_Speed);
           Car_Go(Go_Speed,Go_Temp);
           delay_ms(200);
           make=0x09;
         break;
      }
      case 0x09:
      {
         //控制立体显示标志物
           Car_R45(wheel_Speed,wheel_Time);
           Infrared_Send(Stere2,6);    //控制立体显示标志物显示前方学校减速慢行
           delay_ms(500);
           
           Car_Left(wheel_Speed);
           delay_ms(200);
           Car_Track(Go_Speed);
           Car_Go(Go_Speed,Go_Temp);
           delay_ms(200);
           
           //倒车入库
           Car_Right(wheel_Speed);
           delay_ms(200);
           Car_Track(Go_Speed);
           delay_ms(200);
           Car_Back(50,2000);
           Garage_Cont_Zigbee('A', 2);		                  //立体车库A升至2层
           delay_ms(500);
           Send_ZigbeeData_To_Fifo(Charge_Open, 8);		   //开启无线充电
           delay_ms(500);
           Send_ZigbeeData_To_Fifo(SMG_TimClose, 8);		   //关闭计时
           delay_ms(500);
           
           task_times = gt_get() + 90000; //设置超时处理时间为90秒  
           make=0;
         break;
      }
      
      case 0x10:
      {
         //-------------超时处理函数-------------------
         if(gt_get_sub(task_times) == 0)      //超时处理
         {
           make = 0x11;
         } 
         else if(Zigbee_Rx_flag == 1)	       
         {
            Zigbee_Rx_flag =0;
            if((Zigb_Rx_Buf[0] == 0x55) && (Zigb_Rx_Buf[1] == 0x02))     // 55 02 0D
            {
               if(Zigb_Rx_Buf[2] == 0x0D)
               {
                  make =0x11;
                  
               }
            }
         }           
         break;
      }
      case 0x11:
      {
         
      }
      default : 
       break;		
	}
}

/* 按键检测函数 */
void KEY_Check(void)
{
	//uint8_t i;
    if(S1 == 0)
    {
        delay_ms(10);
        if(S1 == 0)
        {
            LED1 = !LED1;
            while(!S1);	
			AutoFlag = 1;		
			
        }
    }
    if(S2 == 0)
    {
        delay_ms(10);
        if(S2 == 0)
        {
            LED2 = !LED2;
            while(!S2);
//			Yang_Traffic_RecognitionPatterns('A');
        }
    }
    if(S3 == 0)
    {
        delay_ms(10);
        if(S3 == 0)
        {
			
            LED3 = !LED3;
            while(!S3);
			Yang_ETC_Inquire();
        }
    }
    if(S4 == 0)
    {
        delay_ms(10);
        if(S4 == 0)
		   {
            while(!S4);
        }
    }
}

int main(void)
{
	
    uint16_t Light_Value = 0;               // 光强度值
    uint16_t CodedDisk_Value = 0;           // 码盘值
    Hardware_Init();                        // 硬件初始化
    LED_twinkle_times = gt_get() + 50;
    Power_check_times = gt_get() + 200;
    WIFI_Upload_data_times = gt_get() + 200;
    RFID_Init_Check_times = gt_get() + 200;
    Principal_Tab[0] = 0x55;                // 主车数据上传指令包头
    Principal_Tab[1] = 0xAA;
    Follower_Tab[0] = 0x55;                 // 智能运输车(从车)数据上传指令包头
    Follower_Tab[1] = 0x02;
    Send_UpMotor(0, 0);	
    while(1)
    {
        KEY_Check();                                    // 按键检测
#if 1
		if(AutoFlag)
		{
			Yang_Automatic_1();
		}
#endif		
#if 0
		//循环检测识别回传(需要判断识别的回传再打开)可在定时器那开启
		Voice_Drive();	
#endif				
        Can_WifiRx_Check();                             // Wifi交互数据处理(判断APP发送过来的)
        Can_ZigBeeRx_Check();                           // Zigbee交互数据处理
		  Car_Thread();									        // 全自动运行程序
        if(gt_get_sub(LED_twinkle_times) == 0)          // 运行指示灯
        {
            LED_twinkle_times = gt_get() + 50;          // LED4状态取反
            LED4 = !LED4;
        }

        if(gt_get_sub(Power_check_times) == 0)          // 电池电量检测
        {
            Power_check_times = gt_get() + 200;
            Power_Check();
        }

#if 1
        if(gt_get_sub(RFID_Init_Check_times) == 0)      // RFID初始化检测
        {
            RFID_Init_Check_times =  gt_get() + 200;
            if(Rc522_GetLinkFlag() == 0)	//初始化失败
            {
                Readcard_daivce_Init();
                MP_SPK = !MP_SPK;	//蜂鸣器间断响
            }
            else	//表示初始化成功
            {
				MP_SPK = 0;
				LED1 = !LED1;
				Rc522_LinkTest();
            }
        }
#endif
        if(gt_get_sub(WIFI_Upload_data_times) == 0)         // 数据上传
        {
            WIFI_Upload_data_times =  gt_get() + 500;
            if(Host_AGV_Return_Flag == RESET)               // 主车数据上传
            {
                Principal_Tab[2] = Stop_Flag;               // 运行状态
                Principal_Tab[3] = Get_tba_phsis_value();   // 光敏状态

                Ultrasonic_Ranging();                       // 超声波数据采集
                Principal_Tab[4] = dis % 256;               // 超声波数据低八位
                Principal_Tab[5] = dis / 256;               // 超声波数据高八位

                Light_Value = Get_Bh_Value();               // 光强度传感器数据采集
                Principal_Tab[6] = Light_Value % 256;       // 光强度数据低八位
                Principal_Tab[7] = Light_Value / 256;       // 光强度数据高八位

                CodedDisk_Value = CanHost_Mp;               // 码盘值
                Principal_Tab[8] = CodedDisk_Value % 256;
                Principal_Tab[9] = CodedDisk_Value / 256;

                Principal_Tab[10] = coordinate;             //返回随机救援坐标点
                Send_WifiData_To_Fifo(Principal_Tab, 13);   // 通过Wifi上传主车数据(不能屏蔽不然app连接不了车)
#if debug_A72				
                UartA72_TxClear();
                UartA72_TxAddStr(Principal_Tab, 13);        // 通过串口上传主车数据
                UartA72_TxStart();
#endif				
				coordinate = 0 ;
            }
            else if((Host_AGV_Return_Flag == SET) && (AGV_data_Falg == SET))
            {
#if debug_A72
                UartA72_TxClear();
                UartA72_TxAddStr(Follower_Tab, 50);         // 通过串口上传从车数据
                UartA72_TxStart();
#endif				
                Send_WifiData_To_Fifo(Follower_Tab, 50);    // 通过Wifi上传从车数据
                AGV_data_Falg = 0;
            }
        }
    }
}

/* 硬件初始化函数 */
//没用到自带终端所以不需要UartA72_Init
//	TIM1	TIM8~TIM11 的时钟为 168M
//	TIM2~TIM7 	TIM12~TIM14 的时钟为 84M
void Hardware_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);     // 中断分组
    delay_init(168);                                    // 延时初始化
    Tba_Init();                                         // 任务板初始化
    Infrared_Init();                                    // 红外初始化
    Cba_Init();                                         // 核心板初始化
    Ultrasonic_Init();                                  // 超声波初始化	(TIM6频率0.01ms,中断优先级0,8)+(外部中断，优先级0,7)
    Hard_Can_Init();                                    // CAN总线初始化
    BH1750_Configure();                                 // BH1750初始化配置
    BKRC_Voice_Init();									// 小创语音模块初始化
#if debug_A72	
	UartA72_Init();										//自带终端的(可不要)
#endif	
    Electricity_Init();                                 // 电量检测初始化
    Can_check_Init(7, 83);                              // CAN总线定时器初始化	(TIM7频率0.008ms,中断优先级0,4)
    roadway_check_TimInit(1999, 167);                   // 路况检测	(TIM9频率2ms,中断优先级0,5)
    Timer_Init(999, 167);                               // 串行数据通讯时间帧	(TIM10频率1ms,中断优先级0,6)
    Readcard_daivce_Init();                         	// RFID初始化
#if 0
	USART1_Configure(115200);	//串口1（用到电脑调试时才开启不用时必须关闭不然影响其他功能！）
#endif
#if 1
	Timer2_Init(167,999);	//定时器2(频率2ms，中断优先级0,9)
#endif	
}

