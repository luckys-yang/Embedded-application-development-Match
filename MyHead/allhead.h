#ifndef __ALLHEAD_H
#define __ALLHEAD_H
#include <stdio.h>	//c库
#include <string.h>	//c库
#include <stdlib.h>	//c库
#include "stm32f4xx.h"	//stm32f4头文件
#include "delay.h"	//延时头文件
#include "infrared.h"	//红外发射头文件
#include "cba.h"	//核心板头文件
#include "ultrasonic.h"	//超声波头文件
#include "canp_hostcom.h"	//CAN数据交互头文件
#include "hard_can.h"	//CAN初始化与检测头文件
#include "bh1750.h"	//光度传感器头文件
#include "power_check.h"	//电量采集头文件
#include "can_user.h"	//CAN用户数据监视头文件(核心)
#include "data_base.h"	//命令数组定义头文件
#include "roadway_check.h"	//主车寻迹与路况检测头文件
#include "tba.h"	//任务板头文件
#include "swopt_drv.h"	//未知(没用到)
#include "uart_a72.h"	//车自带终端(没用到)
#include "Can_check.h"	//CAN总线检测头文件
#include "Timer.h"	//TIM10时间片轮询配置头文件
#include "Rc522.h"	//RFID头文件
#include "bkrc_voice.h"	//小创语音头文件
#include "drive.h"	//系统常用的函数头文件
#if 0	//不用到
#include "usart.h"	//串口1头文件
#endif
#include "sys.h"	//IO控制头文件
#include "data_filtering.h"	//滤波算法头文件
#include "fifo_drv.h"
#include "yang.h"	//我自定义的头文件(核心)


/************************************main.c的全局变量************************************/

extern uint8_t RFID_Flag;          	           // RFID检测标志位(默认0)
extern uint16_t distance;                      // 记录超声波数据(默认0)(默认0)
extern uint8_t gear_init;                      // 智能路灯初始档位(默认0)(默认0)
extern uint8_t coordinate;                         // 随机坐标点
extern uint8_t number;                         // 计数值(默认0)
extern uint8_t make;                           // 全自动驾驶标志位(默认0)
extern uint16_t random;                            // 随机数
//全自动执行函数设的值
extern uint8_t  Go_Speed;                    // 全局行进速度值(默认40)
extern uint8_t  wheel_Speed;                  // 全局转弯速度值(默认90)
extern uint8_t  RFID_Go_Speed;               // RFID路段的速度根据新车、旧车自行调节(默认35)
extern uint16_t Go_Temp;                     // 全局前进码盘值(默认260)
extern uint16_t wheel_Time;                  // 全局转45°时间(默认330)
//立体显示
extern uint8_t Stere1[6];		//显示 《前方学校，减速慢行》(0xFF,0x14,0x01,0x00,0x00,0x00)
extern uint8_t Stere2[6];		//显示 《前方学校，减速慢行》(0xFF,0x14,0x02,0x00,0x00,0x00)
extern uint8_t Stere3[6];		//显示 《前方学校，减速慢行》(0xFF,0x14,0x03,0x00,0x00,0x00)
extern uint8_t Stere4[6];		//显示 《前方学校，减速慢行》(0xFF,0x14,0x04,0x00,0x00,0x00)
//安卓二维码识别
extern uint8_t data1[3];		//告诉安卓识别二维码(0x55,0xA1,0x01)
extern uint8_t data2[3];		//告诉安卓识别二维码(0x55,0xA1,0x02)
extern uint8_t data3[3];		//告诉安卓识别二维码(0x55,0xA1,0x03)
extern uint8_t data4[3];		//告诉安卓识别二维码(0x55,0xA1,0x04)
extern uint8_t data5[3];		//告诉安卓识别二维码(0x55,0xA1,0x10)
extern uint8_t data6[3];		//告诉安卓识别二维码(0x55,0xA1,0x11)

extern uint8_t AutoFlag;//全自动执行开始标志位(默认0)

/****************************************************************************************/


/********************************ultrasonic.c的全局变量**********************************/

extern float Ultrasonic_Value;		//存储超声波计算后结果临时变量(默认0)
extern uint32_t Ultrasonic_Num;	//超声波计数值(默认0)
extern uint16_t dis;	//存储超声波计算后结果最终变量(默认0)

/****************************************************************************************/


/************************************bh1750.c的全局变量***********************************/

extern uint8_t  BUF[4]; //光度传感器接收数据缓存区(默认0)

/****************************************************************************************/


/************************************power_check.c的全局变量******************************/

extern uint8_t pwr_ck_l,pwr_ck_r;  // 电量数据(默认0)
extern uint8_t Electric_Buf[2];	//(默认0)

/*****************************************************************************************/


/**********************************************rc522.c的全局变量**************************/

extern uint8_t CT[2];		// 卡类型
extern uint8_t SN[4];		// 卡号
extern uint8_t READ_RFID[16];		// 存放RFID 
extern uint8_t WRITE_RFID[16];	//(默认"0123456789ABCDEF")
extern uint8_t KEY_A[6];   // A密钥(默认 0xff,0xff,0xff,0xff,0xff,0xff)
extern uint8_t KEY_B[6];	// B密钥(默认 0xff,0xff,0xff,0xff,0xff,0xff)
extern uint8_t ADDR_Str[14];	//默认"RFID_ADDR:01\n"

/******************************************************************************************/


/************************************bkrc_voice.c的全局变量**********************************/

extern uint8_t uart6_data;			// USART6 接收数据临时缓存(默认0)
extern uint8_t uart6_flag;			// USART6 接收数据时序(默认0)
extern uint8_t UART6_RxData[8];		// USART6 接收数据最终缓存
extern uint8_t voice_falg;		// 语音模块返回状态(默认0；0x01--表示接收一帧命令完成,0x00--没接收到)
extern uint8_t YY_Init[5];	//控制语音播报标志物播报指定文本信息命令(默认 0xFD, 0x00, 0x00, 0x01, 0x01)(帧头，数据区高字节，数据区低字节，命令字，GBK格式)
extern uint8_t Zigbee[8];           // Zigbee发送数据缓存(8Byte的命令，用于xx路站)
extern uint8_t start_voice_dis[5];	//唤醒词(无需管 默认 0xFA, 0xFA, 0xFA, 0xFA, 0xA1)
extern uint8_t bkrc_voice_Flag;           // SYN7318语音识别命令ID编号(默认0)

/********************************************************************************************/


/**********************************data_base.c的全局变量**************************************/

extern uint8_t Principal_Tab[Principal_Length];	//主车数据上传**********************指令包头 （数组大小50）
extern uint8_t Follower_Tab[Follower_Length];	//智能运输车(从车)数据上传指令包头（数组大小50）

/********************************************************************************************/


/********************************************roadway_check.c的全局变量************************/

extern uint8_t wheel_L_Flag, wheel_R_Flag;	//左转标志位，右转标志位（默认0）
extern uint8_t wheel_Nav_Flag;	//码盘旋转标志位（默认0）
extern uint8_t Go_Flag, Back_Flag;	//前进标志位，后退标志位（默认0）
extern uint8_t Track_Flag;	//寻迹标志位（默认0）
extern uint8_t Stop_Flag;	//停止标志位（默认0）
extern uint16_t count;	//相反的寻迹函数用到的计数（默认0）
extern int LSpeed, RSpeed;	//最终左轮速度 最终右轮速度（默认0）
extern int Car_Spend;	//车速度临时变量(函数传参时赋给它)（默认0）
extern uint16_t temp_MP;	//码盘值临时变量(函数传参时赋给它)（默认0）
extern uint16_t temp_Nav;	//码盘旋转值（默认0）
extern uint32_t Wheel_flag;	//转弯计数值（默认0）
extern int16_t Roadway_cmp;	//最终码盘值

/*********************************************************************************************/


/************************************can_user.c的全局变量**************************************/

extern uint8_t Wifi_Rx_Buf[ WIFI_MAX_NUM ];	//WiFi接收命令数组(终端发送过来的)(数组大小200)
extern uint8_t Zigb_Rx_Buf[ ZIGB_RX_MAX ];	//ZigBee接收数组(标志物回传的命令)(数组大小200)
extern uint8_t Wifi_Rx_num ;	//WiFi接收数组的索引(可判断接收了多少字节)
extern uint8_t Wifi_Rx_flag ;  //WiFi接收完成标志位
extern uint8_t Zigbee_Rx_num ;	//ZigBee接收数组的索引(可判断接收了多少字节)
extern uint8_t Zigbee_Rx_flag ;  //ZigBee接收完成标志位
extern uint8_t Host_AGV_Return_Flag;	//主车数据上传标志位（默认RESET）
extern uint8_t AGV_data_Falg;	//返回从车数据标志位（默认RESET）
extern uint32_t canu_wifi_rxtime;	//时间片轮询WiFi接收需要时间（默认0）
extern uint32_t canu_zibe_rxtime;	//时间片轮询ZigBee接收需要时间（默认0）

/*********************************************************************************************/


/****************************************yang.c的全局变量**************************************/
//语音播报标志物发送控制命令存储数组
extern uint8_t Voice_TX_Arr[8];//默认(0x55, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBB)
//接收语音播报标志物请求回传的日期缓存数组
extern uint8_t Voice_RX_DateArr[3];	//(默认0)
//接收语音播报标志物请求回传的时间缓存数组
extern uint8_t Voice_RX_TimeArr[3];	//(默认0)
//接收语音播报标志物请求回传的天气与温度数据缓存数组
extern uint8_t Voice_RX_WeatherTempertureArr[2];	//(默认0)
//语音播报标志物3选1命令存储数组
extern uint8_t Voice_TX_3choose1_Arr[4];	//默认(0xFD,0x00,0x01,0x00)
//闸门标志物发送控制命令存储数组
extern uint8_t Gate_TX_Arr[8];	//默认(0x55,0x03,0x00,0x00,0x00,0x00,0x00,0xBB)
//LED显示标志物发送控制命令存储数组
extern uint8_t LED_TX_Arr[8];	//默认(0x55,0x04,0x00,0x00,0x00,0x00,0x00,0xBB)

//标志物状态标志位
extern uint8_t LandMark_Flag; //（默认0）
//标志物超时计数
extern uint16_t LandMark_TimeA; //（默认0）
extern uint16_t LandMark_TimeB;	//（默认0）

//立体车库标志物发送控制命令存储数组
extern uint8_t Garage_TX_Arr[8];	//默认(0x55,0x00,0x00,0x00,0x00,0x00,0x00,0xBB)
//立体车库标志物红外状态 0---都没触发 1---前侧触发后侧没触发 2---前侧没触发后侧触发 3---都触发
extern uint8_t Garage_Infrared_Flag; //（默认-1）
//烽火台报警器标志物发送控制命令存储数组
extern uint8_t Beacon_TX_Arr[8];	//默认(0x55,0x07,0x00,0x00,0x00,0x00,0x00,0xBB)
//烽火台报警器标志物回传的随机坐标点
extern uint8_t Beacon_Coordinate; //（默认0）
//烽火台报警器标志物发送控制命令存储数组(红外)
extern uint8_t Beacon_TX_Arr_Infrared[6];
//TFT显示标志物发送控制命令存储数组
extern uint8_t TFT_TX_Arr[8];	//默认(0x55,0x00,0x00,0x00,0x00,0x00,0x00,0xBB)
//无线充电标志物发送控制命令存储数组
extern uint8_t WirelessCharging_TX_Arr[8];	//模式(0x55,0x0A,0x01,0x00,0x00,0x00,0x00,0xBB)
//ETC标志物发送控制命令存储数组
extern uint8_t ETC_TX_Arr[8];	//默认(0x55,0x0C,0x00,0x00,0x00,0x00,0x00,0xBB)
//智能交通灯标志物发送控制命令存储数组
extern uint8_t SmartTrafficLights_TX_Arr[8];	//默认(0x55,0x00,0x00,0x00,0x00,0x00,0x00,0xBB)
//特殊地形标志物发送控制命令存储数组
extern uint8_t SpecialTerrain_TX_Arr[8];	//默认(0x55, 0x10, 0x10, 0x01, 0x00, 0x00, 0x00, 0xBB)
//智能路灯标志物发送控制命令存储数组(红外)
extern uint8_t StreetLight_TX_Arr[4];	//默认(0x00,0xFF,0x00,0x00)
//保存智能路灯标志物每档的光度值
extern uint16_t Light_temp[4];	//(默认0)
//立体显示标志物发送控制命令存储数组(红外)
extern uint8_t ThreeD_TX_Arr[6];	//默认(0xFF,0x00,0x00,0x00,0x00,0x00)
//发送给从车的命令存储数组
extern uint8_t AGV_TX_Arr[8];	//默认(0x55,0x02,0x00,0x00,0x00,0x00,0x00,0xBB)

extern uint16_t Yang_Go_Speed;	//寻迹，正常行走的速度(默认40)
extern uint16_t Yang_LR_Speed;	//左右转速度(角度)(默认90)
extern uint16_t Yang_Removing;	//前进时的距离(默认180)
extern uint16_t L_SPEED;	//左转速度
extern uint16_t L_TIME;	//左转时间
extern uint16_t R_SPEED;	//右转速度
extern uint16_t R_TIME;	//右转时间
extern uint16_t Go_TIME;	//前进320延时的距离
extern uint16_t Yang_wheel_Time; // 全局转45°时间
extern uint16_t Yang_Delay_big;	//大延时
extern uint16_t Yang_Delay_small;	//小延时

//主车全自动状态位
extern uint8_t Automatic_Falg;	//默认(0x01)
extern uint16_t Yang_wheel_Time; // 全局转45°时间(默认330)

//主车寻迹标志位自定义(寻迹白线用的)
extern uint8_t Yang_Track_Flag;	//(默认0)
//主车倒车入库结束标志位
extern uint8_t Back_Car_Flag;//(默认0)

//接收APP识别交通灯颜色的值(1--红色 2--绿色 3--黄色)
extern uint8_t App_RX_TrafficColor;
//数组转字符串后存放的(下面车牌)
extern char App_RX_TFTCarCard[7];	//默认("\0")
//接收APP识别TFT标志物车牌的结果(如果主指令是0x00表示识别错误或不是车牌图片 如果是0x01则是识别到一个车牌)
extern char App_RX_TFTarr[6];
//保存读取RFID提取的数字
extern uint8_t RFID_Number_Buf[16];



/*********************************************************************************************/


/************************************常用的全局函数********************************************/

extern void Infrared_Send(uint8_t *s, int n);	//红外控制命令发送函数
extern void Set_tba_Beep(uint8_t swch);	//蜂鸣器开关(参数：SET--开 RESET--关)
extern void Set_tba_WheelLED(uint8_t LorR, uint8_t swch);	//转向灯开关(参数：SET--开 RESET--关)
extern void Ultrasonic_Ranging(void);	//超声波测距(计算结果在外部中断进行)
extern uint16_t Get_Bh_Value(void);	//读取光度传感器的值
extern void RC522(uint8_t card_addr,uint8_t mode);	//RFID读写控制函数
extern char PcdRequest(unsigned char req_code,unsigned char *pTagType);	//寻RFID卡
extern char PcdAuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr);	//验证卡片密码
extern char PcdRead(unsigned char addr,unsigned char *pData);	//读取M1卡一块数据
extern char PcdWrite(unsigned char addr,unsigned char *pData);	//写数据到M1卡一块
extern void USART6_Send_Byte(uint8_t byte);	//USART6发送1个字节(可用于发送播报编号ID)
extern uint8_t Voice_Drive(void);	//语音识别回传命令解析函数(可对返回值进行操作)
extern void Roadway_mp_syn(void);	//码盘同步
extern uint16_t Mean_Filter(uint16_t m);	//均值过滤器
extern uint16_t Smoothing_Filtering(uint16_t value);	//平滑滤波
extern void Send_UpMotor( int x1, int x2);	//发送电机速度
extern void Send_ZigbeeData_To_Fifo( uint8_t *p ,uint8_t len);	//将数据发送至ZigBee模块，并在ZigBee区显示
extern void Send_WifiData_To_Fifo( uint8_t *p ,uint8_t len);	//将数据发送至WIFI模块，并在WiFi区显示
extern void Send_InfoData_To_Fifo(char *p ,uint8_t len);	//在Debug区显示，不能显示中文
extern void Can_WifiRx_Check(void);	//Wifi接收的数据解析
extern void Normal_data(void);	//wifi接收到8Byte接收相关


/*===============行走函数==================*/
extern uint16_t  Get_Host_UpTrack( uint8_t mode);	//获取寻迹灯数据(参数可以是前7，后8，所有灯)
extern void Car_Go(uint8_t speed, uint16_t temp);   // 主车前进 参数：速度/码盘
extern void Car_Back(uint8_t speed, uint16_t temp); // 主车后退 参数：速度/码盘
extern void Car_Left(uint8_t speed);       // 主车左转 参数：速度
extern void Car_Right(uint8_t speed);       // 主车右转 参数：速度
extern void Car_Track(uint8_t speed);   // 主车循迹 参数：速度
extern void Car_L45(int8_t speed, uint16_t times);		// 左旋转 参数：旋转时间 
extern void Car_R45(int8_t speed, uint16_t tims);		// 右旋转 参数：旋转时间
/**********************************************************************************************/

#endif




