#ifndef __YANG_H
#define __YANG_H
#include "allhead.h"


void Yang_Voice_Assign(uint8_t ID);	//播报指定语音命令(xx路站)
void Yang_Voice_Random(void);	//播报随机语音命令(xx路站)
void Yang_Voice_SetRtcDate(uint8_t year,uint8_t month,uint8_t day);	//设置RTC起始日期(年月日)
void Yang_Voice_SetRtcTime(uint8_t hour,uint8_t min,uint8_t sec);	//设置RTC起始时间(时分秒)
void Yang_Voice_InquireRtcDate(void);	//查询RTC当前日期(年月日)并存储(有回传)
void Yang_Voice_InquireRtcTime(void);	//查询RTC当前时间(时分秒)并存储(有回传)
void Yang_Voice_SetWeatherTemperture(uint8_t weather,uint8_t temperture);	//设置天气数据与温度数据
void Yang_Voice_InquireWeatherTemperture(void);	//请求回传天气与温度数据(有回传)
void Yang_Voice_Customize(char *p);	//播报自定义合成文本语音
void Yang_Voice_3Choose1(uint8_t num);	//停止/暂停/恢复合成语音
void Yang_Gate_OffOrOn(uint8_t swch);	//开启闸门/关闭闸门
void Yang_Gate_CarCard(char* str);	//发送车牌数据到闸门标志物
uint8_t Yang_InquireGate_State(void);	//请求回传闸门状态且判断回传值
void Yang_LED_ShowData(uint8_t one,uint8_t two,uint8_t three,uint8_t choose);	//LED显示标志物数码管第一排显示数据/第二排显示数据
void Yang_LED_Timing(uint8_t choose);	//LED显示标志物第1排显示计时模式
uint16_t Yang_LED_Removing(uint16_t pdis);	//LED显示标志物第2排显示距离
void Yang_GarageWent(uint8_t device,uint8_t floor);	//控制立体车库A/B到达第几层
uint8_t Yang_InquireGarage_Floor(uint8_t device);	//请求回传车库位于第几层
void Yang_Garage_Infrared(uint8_t device);	//请求回传车库红外状态
#if 0
void Yang_Beacon_Open(uint8_t* temp);	//向烽火台报警器发送开启码(ZigBee发送)
void Yang_Beacon_InquireCoordinate(void);	//请求回传随机救援坐标(ZigBee发送)
#endif
void Yang_Beacon_Open2(uint8_t* temp);	//向烽火台报警器发送开启码(红外发送)
void Yang_TFT_Assign(uint8_t device,uint8_t ID);	//控制TFT标志物A/B显示指定图片
void Yang_TFT_TurnPage(uint8_t device,uint8_t choose);	//控制TFT标志物A/B图片手动翻页/自动翻页
void Yang_TFT_CarCard(uint8_t device,char* str);	//控制TFT标志物A/B显示车牌模式
void Yang_TFT_Timing(uint8_t device,uint8_t choose);	//控制TFT标志物A/B计时显示模式
void Yang_TFT_HexShow(uint8_t device,uint8_t* temp);	//控制TFT标志物A/B 六位HEX格式数据显示
void Yang_TFT_Removing(uint8_t device,uint16_t pdis);	//控制TFT标志物A/B显示距离模式
void Yang_TFT_TrafficSigns(uint8_t device,uint8_t ID);	//控制TFT标志物A/B交通标志显示模式
void Yang_WirelessCharging_OffOrOn(uint8_t swch);	//控制无线充电标志物开关
void Yang_ETC_Inquire(void);	//查询ETC是否已开启
void Yang_Traffic_RecognitionPatterns(uint8_t device);	//进入识别模式
void Yang_Traffic_Color(uint8_t device,uint8_t ID);	//识别结果请求确认
void Yang_SpecialTerrain_check(void);	//特殊地形标志物查询车辆通行状态
void Yang_SpecialTerrain_1(uint8_t speed);

void StreetLight_adjust(uint8_t ID);	//调节路灯的光源档位(红外发送)
void StreetLight_Test(uint8_t num);	//根据光度判断当前档位再根据需要调到规定档位(红外发送)
void Yang_ThreeD_CarCardAndxy(char* str,char x,char y);	//立体显示标志物显示车牌信息及坐标(红外发送)
void Yang_ThreeD_Removing(uint16_t pdis);	//立体显示标志物显示距离模式(红外发送)
void Yang_ThreeD_GraphShow(uint8_t ID);	//立体显示标志物图形信息显示模式(红外发送)
void Yang_ThreeD_Color(uint8_t ID);	//立体显示标志物颜色信息显示模式(红外发送)
void Yang_ThreeD_Warning(uint8_t ID);	//立体显示标志物交通警示牌信息显示模式(红外发送)
void Yang_ThreeD_Traffic(uint8_t ID);	//立体显示标志物交通标志信息显示模式(红外发送)
void Yang_ThreeD_Default(void);	//立体显示标志物显示默认信息(红外发送)
void Yang_ThreeD_TextColor(uint8_t R,uint8_t G,uint8_t B);	//立体显示标志物设置文字显示颜色(红外发送)
void Yang_ThreeD_Other(uint8_t swch);	//立体显示标志物其他功能
void Yang_ThreeD_CustomizeText(void);	//立体显示标志物自定义文本累加
void Yang_AGV_LED(void);	//测试

void Yang_Track_duoy_RFID(uint8_t speed);   //寻卡寻迹
void Yang_Track_duo_RFID(uint8_t speed,uint8_t card);//读卡函数   参数：循迹速度，需要读取的数据块
void Yang_Track_xie_RFID(uint8_t speed,uint8_t card);//写卡函数   参数：循迹速度，需要写取的数据块

void Yang_Automatic_1(void);	//小车全自动函数(模拟任务)
void Yang_Car_TrackAndGo(void);	//寻迹完前进一丢丢
void Yang_Left_Time(uint8_t speed,uint16_t time);	// 主车左转 参数1：速度 参数2：时间(慢慢调)
void Yang_Right_Time(uint8_t speed,uint16_t time);	// 主车右转 参数1：速度 参数2：时间(慢慢调)
void Yang_Track_Correct_White(uint8_t gd);	//寻迹到白线停的执行函数
void Yang_Car_Track_White(uint8_t speed);	//寻迹到白线停函数
void Yang_CarThread_Go(uint8_t speed, uint16_t time,uint8_t swch);	// 主车前进 参数1：速度  参数2：时间(慢慢调)写死了
void Yang_Car_L45(int8_t speed, uint16_t times);	//左旋转45度 参数1：旋转速度 参数2：旋转时间
void Yang_Car_R45(int8_t speed, uint16_t tims);	//右旋转45度 参数1：旋转速度 参数2：旋转时间
void Yang_Car_Back_Infrared(uint8_t speed,uint16_t time);	//倒车到红外后侧亮则停函数(也可以当后退函数)
void Yang_Car_Garage_return(void);	//倒车入库执行函数

uint8_t Yang_CheckSum_1(uint8_t main_command, uint8_t sub_command1, uint8_t sub_command2, uint8_t sub_command3);	//计算校验和(适用于含---"主指令""副指令1""副指令2""副指令3" 8Byte通信协议)
void Yang_ArrInit(uint8_t* arr);	//还原数组
void Yang_bubble_sort(uint16_t* arr,uint8_t len);	//冒泡排序
void Timer2_Init(uint16_t arr,uint16_t psc);	//定时器定时
void Yang_ArrToStr(char* arr,char* str,unsigned char size);	//数组转字符串(车牌数据用)
void Yang_DrawNumber(uint8_t* Read_buf);	//在一堆数据里提取数字(RFID用)
uint8_t Yang_NumberTo16(uint8_t* temp);	//把某两个数组合成一个0xXX的十六进制(不是十进制直接转成十六进制)

#endif






