#include "allhead.h"
#include "yang.h"

/***************************************************************************
									宏定义区
****************************************************************************/
//立体车库标志物设备A/B
#define GARAGE_DEVICE_A	0x0D
#define GARAGE_DEVICE_B 0x05
//TFT标志物设备A/B
#define TFT_DEVICE_A	0x0B
#define TFT_DEVICE_B 	0x08
//智能交通灯标志物设备A/B
#define TRAFFIC_DEVICE_A	0x0E
#define TRAFFIC_DEVICE_B 	0x0F

/***************************************************************************
									全局变量区
****************************************************************************/

/*====================发送控制命令======================*/
//语音播报标志物发送控制命令存储数组
uint8_t Voice_TX_Arr[8] = {0x55, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBB};
//语音播报标志物3选1发送控制命令存储数组
uint8_t Voice_TX_3choose1_Arr[4] = {0xFD, 0x00, 0x01, 0x00};
//闸门标志物发送控制命令存储数组
uint8_t Gate_TX_Arr[8] = {0x55, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBB};
//LED显示标志物发送控制命令存储数组
uint8_t LED_TX_Arr[8] = {0x55, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBB};
//立体车库标志物发送控制命令存储数组
uint8_t Garage_TX_Arr[8] = {0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBB};
//烽火台报警器标志物发送控制命令存储数组
uint8_t Beacon_TX_Arr[8] = {0x55, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBB};
//烽火台报警器标志物发送控制命令存储数组(红外)
uint8_t Beacon_TX_Arr_Infrared[6];
//TFT显示标志物发送控制命令存储数组
uint8_t TFT_TX_Arr[8] = {0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBB};
//无线充电标志物发送控制命令存储数组
uint8_t WirelessCharging_TX_Arr[8] = {0x55, 0x0A, 0x01, 0x00, 0x00, 0x00, 0x00, 0xBB};
//ETC标志物发送控制命令存储数组
uint8_t ETC_TX_Arr[8] = {0x55, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBB};
//智能交通灯标志物发送控制命令存储数组
uint8_t SmartTrafficLights_TX_Arr[8] = {0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBB};
//特殊地形标志物发送控制命令存储数组
uint8_t SpecialTerrain_TX_Arr[8] = {0x55, 0x10, 0x10, 0x01, 0x00, 0x00, 0x00, 0xBB};
//智能路灯标志物发送控制命令存储数组(红外)
uint8_t StreetLight_TX_Arr[4] = {0x00, 0xFF, 0x00, 0x00};
//立体显示标志物发送控制命令存储数组(红外)
uint8_t ThreeD_TX_Arr[6] = {0xFF, 0x00, 0x00, 0x00, 0x00, 0x00};
//发送给从车的命令存储数组
uint8_t AGV_TX_Arr[8] = {0x55, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBB};



/*====================回传数据存储======================*/
//接收语音播报标志物请求回传的日期缓存数组
uint8_t Voice_RX_DateArr[3] = {0};
//接收语音播报标志物请求回传的时间缓存数组
uint8_t Voice_RX_TimeArr[3] = {0};
//接收语音播报标志物请求回传的天气与温度数据缓存数组
uint8_t Voice_RX_WeatherTempertureArr[2] = {0};
//立体车库标志物红外状态 0---都没触发 1---前侧触发后侧没触发 2---前侧没触发后侧触发 3---都触发
uint8_t Garage_Infrared_Flag = 0;
//烽火台报警器标志物回传的随机坐标点
uint8_t Beacon_Coordinate = 0;
//保存智能路灯标志物每档的光度值
uint16_t Light_temp[4] = {0, 0, 0, 0};
/*====================通用======================*/
//通用标志物状态标志位
uint8_t LandMark_Flag = 0;
//通用标志物超时计数
uint16_t LandMark_TimeA = 0;
uint16_t LandMark_TimeB = 0;




/*==================保存终端回传的=================*/
/* 定义终端发送给主车的格式
帧头第一位--0x55 帧头第二位--0xAA 主指令--0xA1~0xA9 0xB1~0xB9 副指令1--0x00 副指令2--识别结果等等 副指令3--备用(0x00) 校验和 帧尾--0xBB
*/
//接收APP识别交通灯颜色的值(1--红色 2--绿色 3--黄色)
uint8_t App_RX_TrafficColor;
//数组转字符串后存放的(下面车牌)
char App_RX_TFTCarCard[7] = "\0";
//接收APP识别TFT标志物车牌的结果(如果主指令是0x00表示识别错误或不是车牌图片 如果是0x01则是识别到一个车牌)
char App_RX_TFTarr[6];
//保存读取RFID提取的数字
uint8_t RFID_Number_Buf[16] = "92";

/*

LandMark_Flag 状态解释：

0x06 ---> ETC处于开启状态
0x07 ---> 智能交通灯识别模式成功
0x08 ---> 智能交通灯识别模式失败
0x31 ---> 通行成功方向A->B
0x32 ---> 通行成功方向B->A
0x33 ---> 通行失败

*/







/***************************************************************************
									语音标志物控制区
****************************************************************************/

/*
附录1：
帧头第一位--0x55,帧头第二位--0x06，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明(可能编号对应的跟播报的不一样以实际为准编号实际是00~06)
|--------|-----------|-----------|-----------|-----------------------------------
|        |	 0x01	 |           |           | 播报"民主路站"(编号01)
|        |-----------|           |           |-----------------------------------
|        |	 0x02    |	 	     |           | 播报"文明路站"(编号02)
|        |-----------|           |           |-----------------------------------
|        |   0x03    |	      	 |           | 播报"和谐路站"(编号03)
|        |-----------|           |           |-----------------------------------
|  0x10  |   0x04    |	  0x00   |     0x00  | 播报"爱国路站"(编号04)
|        |-----------|           |           |-----------------------------------
|        |   0x05    |           |           | 播报"敬业路站"(编号05)
|        |-----------|           |           |-----------------------------------
|        |	 0x06    |           |           |播报"友善路站"(编号06)
|        |-----------|           |           |-----------------------------------
|        |   0x00    |           |           |播报"富强路站"(编号00)
|        |-----------|           |           |-----------------------------------
--------------------------------------------------------------------------------
*/
/*
功能：播报指定语音命令(xx路站)
参数---需要播报的ID(详见通信协议)
*/
void Yang_Voice_Assign(uint8_t ID)
{
    uint8_t checksum;

    Voice_TX_Arr[2] = 0x10;	//主指令
    Voice_TX_Arr[3] = ID;	//副指令1
    checksum = Yang_CheckSum_1(Voice_TX_Arr[2], Voice_TX_Arr[3], Voice_TX_Arr[4], Voice_TX_Arr[5]);	//计算校验和
    Voice_TX_Arr[6] =  checksum;	//校验和
    Send_ZigbeeData_To_Fifo(Voice_TX_Arr, 8);	//通过ZigBee发送命令
    Yang_ArrInit(Voice_TX_Arr);	//还原数组
    delay_ms(100);	//延时100ms
}
/*
附录1：
帧头第一位--0x55,帧头第二位--0x06，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|-----------------------------------
|   0x20 |	 0x01	 |   0x00    |    0x00   | 播报随机播报语音编号 01~07
|--------|-----------|-----------|-----------|-----------------------------------
--------------------------------------------------------------------------------
*/
/*
功能：播报随机语音命令(xx路站)
*/
void Yang_Voice_Random(void)
{
    uint8_t checksum;

    Voice_TX_Arr[2] = 0x20;	//主指令
    Voice_TX_Arr[3] = 0x01;	//副指令1
    checksum = Yang_CheckSum_1(Voice_TX_Arr[2], Voice_TX_Arr[3], Voice_TX_Arr[4], Voice_TX_Arr[5]);	//计算校验和
    Voice_TX_Arr[6] =  checksum;	//校验和
    Send_ZigbeeData_To_Fifo(Voice_TX_Arr, 8);	//通过ZigBee发送命令
    Yang_ArrInit(Voice_TX_Arr);	//还原数组
    delay_ms(100);	//延时100ms
}
/*
附录1：
帧头第一位--0x55,帧头第二位--0x06，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|-----------------------------------
|   0x30 |	 0xXX	 |   0xXX    |    0xXX   | 0xXX格式是0x+直接想显示的日期
|--------|-----------|-----------|-----------|-----------------------------------
--------------------------------------------------------------------------------
*/
/*
功能：设置RTC起始日期(年月日)
参数1---年0x+十进制 参数2---月0x+十进制 参数3---日0x+十进制
*/
void Yang_Voice_SetRtcDate(uint8_t year, uint8_t month, uint8_t day)
{
    uint8_t checksum;

    Voice_TX_Arr[2] = 0x30;	//主指令
    Voice_TX_Arr[3] = year;	//副指令1---年
    Voice_TX_Arr[4] = month;	//副指令2---月
    Voice_TX_Arr[5] = day;	//副指令2---日
    checksum = Yang_CheckSum_1(Voice_TX_Arr[2], Voice_TX_Arr[3], Voice_TX_Arr[4], Voice_TX_Arr[5]);	//计算校验和
    Voice_TX_Arr[6] =  checksum;	//校验和
    Send_ZigbeeData_To_Fifo(Voice_TX_Arr, 8);	//通过ZigBee发送命令
    Yang_ArrInit(Voice_TX_Arr);	//还原数组
    delay_ms(100);	//延时100ms
}
/*
附录1：
帧头第一位--0x55,帧头第二位--0x06，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|-----------------------------------
|   0x40 |	 0xXX	 |   0xXX    |    0xXX   | 0xXX格式是0x+直接想显示的时间
|--------|-----------|-----------|-----------|-----------------------------------
--------------------------------------------------------------------------------
*/
/*
功能：设置RTC起始时间(时分秒)
参数1---时0x+十进制 参数2---分0x+十进制 参数3---秒0x+十进制
*/
void Yang_Voice_SetRtcTime(uint8_t hour, uint8_t min, uint8_t sec)
{
    uint8_t checksum;

    Voice_TX_Arr[2] = 0x40;	//主指令
    Voice_TX_Arr[3] = hour;	//副指令1---时
    Voice_TX_Arr[4] = min;	//副指令2---分
    Voice_TX_Arr[5] = sec;	//副指令2---秒
    checksum = Yang_CheckSum_1(Voice_TX_Arr[2], Voice_TX_Arr[3], Voice_TX_Arr[4], Voice_TX_Arr[5]);	//计算校验和
    Voice_TX_Arr[6] =  checksum;	//校验和
    Send_ZigbeeData_To_Fifo(Voice_TX_Arr, 8);	//通过ZigBee发送命令
    Yang_ArrInit(Voice_TX_Arr);	//还原数组
    delay_ms(100);	//延时100ms
}
/*
附录1：
帧头第一位--0x55,帧头第二位--0x06，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|-----------------------------------
|   0x31 |	 0x01	 |   0x00    |    0x00   | 查询 RTC 当前日期
|--------|-----------|-----------|-----------|-----------------------------------
|   0x41 |	 0x01	 |   0x00    |    0x00   | 查询 RTC 当前时间
|--------|-----------|-----------|-----------|-----------------------------------
|   0x02 |	 0xXX	 |   0xXX    |    0xXX   | 返回RTC日期年月日(0xXX格式是0x+日期十进制)(回传命令)
|--------|-----------|-----------|-----------|-----------------------------------
|   0x03 |	 0xXX	 |   0xXX    |    0xXX   | 返回RTC时间时分秒(0xXX格式是0x+时间十进制)(回传命令)
|--------|-----------|-----------|-----------|-----------------------------------
--------------------------------------------------------------------------------
*/
/*
功能：查询RTC当前日期(年月日)并存储(有回传)
*/
void Yang_Voice_InquireRtcDate(void)
{
    uint8_t checksum;
#if 0
    uint8_t arr[13];
#endif
    Voice_TX_Arr[2] = 0x31;	//主指令
    Voice_TX_Arr[3] = 0x01;	//副指令1
    checksum = Yang_CheckSum_1(Voice_TX_Arr[2], Voice_TX_Arr[3], Voice_TX_Arr[4], Voice_TX_Arr[5]);	//计算校验和
    Voice_TX_Arr[6] =  checksum;	//校验和
    Send_ZigbeeData_To_Fifo(Voice_TX_Arr, 8);	//通过ZigBee发送命令
    Yang_ArrInit(Voice_TX_Arr);	//还原数组
    delay_ms(100);	//一定要加延时因为执行速度太快了还没置1就已经结束这个函数了
    if(Zigbee_Rx_flag == 1)	 //zigbee返回信息
    {
        Zigbee_Rx_flag = 0;	//接收标志位清0
        if((Zigb_Rx_Buf[0] == 0x55) && (Zigb_Rx_Buf[1] == 0x06))	//判断是不是语音标志物发送过来的
        {
            if(Zigb_Rx_Buf[2] == 0x02)	//判断是不是RTC日期回传
            {
                Voice_RX_DateArr[0] = Zigb_Rx_Buf[3];	//年
                Voice_RX_DateArr[1] = Zigb_Rx_Buf[4];	//月
                Voice_RX_DateArr[2] = Zigb_Rx_Buf[5];	//日
#if 0
                sprintf((char *)arr, "20%x-%x-%x\n", Voice_RX_DateArr[0], Voice_RX_DateArr[1], Voice_RX_DateArr[2]);
                Send_InfoData_To_Fifo((char *)arr, 12);	//显示在Debug区
#endif
            }
        }
    }
    else
    {
        Send_InfoData_To_Fifo("RTCDate error\n", 15);	//异常显示
    }
    delay_ms(100);	//延时100ms
}
/*
功能：查询RTC当前时间(时分秒)并存储(有回传)
*/
void Yang_Voice_InquireRtcTime(void)
{
    uint8_t checksum;
#if 0
    uint8_t arr[11];
#endif
    Voice_TX_Arr[2] = 0x41;	//主指令
    Voice_TX_Arr[3] = 0x01;	//副指令1
    checksum = Yang_CheckSum_1(Voice_TX_Arr[2], Voice_TX_Arr[3], Voice_TX_Arr[4], Voice_TX_Arr[5]);	//计算校验和
    Voice_TX_Arr[6] =  checksum;	//校验和
    Send_ZigbeeData_To_Fifo(Voice_TX_Arr, 8);	//通过ZigBee发送命令
    Yang_ArrInit(Voice_TX_Arr);	//还原数组
    delay_ms(100);
    if(Zigbee_Rx_flag == 1)	 //zigbee返回信息
    {
        Zigbee_Rx_flag = 0;	//接收标志位清0
        if((Zigb_Rx_Buf[0] == 0x55) && (Zigb_Rx_Buf[1] == 0x06))	//判断是不是语音标志物发送过来的
        {
            if(Zigb_Rx_Buf[2] == 0x03)	//判断是不是RTC时间回传
            {
                Voice_RX_TimeArr[0] = Zigb_Rx_Buf[3];	//时
                Voice_RX_TimeArr[1] = Zigb_Rx_Buf[4];	//分
                Voice_RX_TimeArr[2] = Zigb_Rx_Buf[5];	//秒
#if 0
                sprintf((char *)arr, "%x-%x-%x\n", Voice_RX_TimeArr[0], Voice_RX_TimeArr[1], Voice_RX_TimeArr[2]);
                Send_InfoData_To_Fifo((char *)arr, 10);	//显示在Debug区
#endif
            }
        }
    }
    else
    {
        Send_InfoData_To_Fifo("RTCTime error\n", 15);	//异常显示
    }
    delay_ms(100);	//延时100ms
}
/*
附录1：
帧头第一位--0x55,帧头第二位--0x06，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|-----------------------------------
|        |	 0x00	 |           |           | 大风（0xXX格式是十进制度数的十六进制 比如25°C-->0x19）
|        |-----------|           |           |-----------------------------------
|        |	 0x01	 |           |           | 多云
|        |-----------|           |           |-----------------------------------
|        |	 0x02	 |           |           | 晴
|   0x42 |-----------|   0xXX    |    0x00   |-----------------------------------
|        |	 0x03	 |           |           | 小雪
|        |-----------|           |           |-----------------------------------
|        |	 0x04	 |           |           | 小雨
|        |-----------|           |           |-----------------------------------
|        |	 0x05	 |           |           | 阴天
|        |-----------|-----------|-----------|-----------------------------------
--------------------------------------------------------------------------------
*/
/*
设置天气数据与温度数据
参数1---天气十六进制 参数2---温度十六进制
*/
void Yang_Voice_SetWeatherTemperture(uint8_t weather, uint8_t temperture)
{
    uint8_t checksum;

    Voice_TX_Arr[2] = 0x42;	//主指令
    Voice_TX_Arr[3] = weather;	//副指令1
    Voice_TX_Arr[4] = temperture;	//副指令2
    checksum = Yang_CheckSum_1(Voice_TX_Arr[2], Voice_TX_Arr[3], Voice_TX_Arr[4], Voice_TX_Arr[5]);	//计算校验和
    Voice_TX_Arr[6] =  checksum;	//校验和
    Send_ZigbeeData_To_Fifo(Voice_TX_Arr, 8);	//通过ZigBee发送命令
    Yang_ArrInit(Voice_TX_Arr);	//还原数组
    delay_ms(100);	//延时100ms
}
/*
附录1：
帧头第一位--0x55,帧头第二位--0x06，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|-----------------------------------
|   0x43 |	 0x00	 |   0x00    |    0x00   | 请求回传天气数据与温度数据
|--------|-----------|-----------|-----------|-----------------------------------
|   0x04 |	 /	     |   /       |      /    | 返回天气数据与温度数据(除了主指令 其余位跟设置天气与温度的命令一致)(回传命令)
|--------|-----------|-----------|-----------|-----------------------------------
--------------------------------------------------------------------------------
*/
/*
功能：请求回传天气与温度数据(有回传)
*/
void Yang_Voice_InquireWeatherTemperture(void)
{
    uint8_t checksum;
#if 0
    uint8_t arr[12];
#endif

    Voice_TX_Arr[2] = 0x43;	//主指令
    checksum = Yang_CheckSum_1(Voice_TX_Arr[2], Voice_TX_Arr[3], Voice_TX_Arr[4], Voice_TX_Arr[5]);	//计算校验和
    Voice_TX_Arr[6] =  checksum;	//校验和
    Send_ZigbeeData_To_Fifo(Voice_TX_Arr, 8);	//通过ZigBee发送命令
    Yang_ArrInit(Voice_TX_Arr);	//还原数组
    delay_ms(100);
    if(Zigbee_Rx_flag == 1)	 //zigbee返回信息
    {
        Zigbee_Rx_flag = 0;	//接收标志位清0
        if((Zigb_Rx_Buf[0] == 0x55) && (Zigb_Rx_Buf[1] == 0x06))	//判断是不是语音标志物发送过来的
        {
            if(Zigb_Rx_Buf[2] == 0x04)	//判断是不是天气与温度的回传
            {
                Voice_RX_WeatherTempertureArr[0] = Zigb_Rx_Buf[3];	//天气
                Voice_RX_WeatherTempertureArr[1] = Zigb_Rx_Buf[4];	//温度
#if 0
                sprintf((char *)arr, "W-%x T-%x\n", Voice_RX_WeatherTempertureArr[0], Voice_RX_WeatherTempertureArr[1]);
                Send_InfoData_To_Fifo((char *)arr, 11);	//显示在Debug区
#endif
            }
        }
    }
    else
    {
        Send_InfoData_To_Fifo("Weather error\n", 15);	//异常显示
    }
    delay_ms(100);	//延时100ms
}
/*
附录1：
帧头--0xFD
语音合成命令
------------------------------------------------------------------------------------------------
| 数据区长度-H | 数据区长度-L | 数据区-命令字 | 数据区-文本编码 |	待合成文本 |	 说明
|-------------|-------------|--------------|----------------|-------------|---------------------
|             |	     	    |              |    0x00        |             |GB2312编码
|             |             |              |----------------|             |---------------------
|             |             |              |    0x01        |             |GBK编码(一般使用这个)
|     0xXX    |     0xXX    |      0x01    |----------------|    ...      |---------------------
|             |             |              |    0x02        |             |BIG5编码
|             |             |              |----------------|             |---------------------
|             |             |              |    0x03        |             |Unicode编码
|-------------|-------------|--------------|----------------|-------------|---------------------
------------------------------------------------------------------------------------------------
*/
/*
功能：播报自定义合成文本语音
参数--需要发送的字符串(比如你好)
注意--需要注意先发送命令再发送要播报的文本字符串
*/
void Yang_Voice_Customize(char *p)
{
    uint16_t p_len = strlen(p);             // 文本长度(不包括\0)

    YY_Init[1] = 0xff & ((p_len + 2) >> 8); // 数据区长度高八位
    YY_Init[2] = 0xff & (p_len + 2);        // 数据区长度低八位
    Send_ZigbeeData_To_Fifo(YY_Init, 5);	//先发送ZigBee命令
    Send_ZigbeeData_To_Fifo((uint8_t *)p, p_len);	//再发送文本内容和文本长度
    delay_ms(100);	//延时100ms
}
/*
附录1：
帧头--0xFD
------------------------------------------------------------
| 数据区长度-H | 数据区长度-L | 数据区-命令字 | 说明
|-------------|-------------|--------------|----------------
|             |	     	    |      0x02    | 停止合成语音
|             |             |--------------|----------------
|             |	     	    |      0x03    | 暂停合成语音
|    0x00     |    0x01     |--------------|----------------
|             |	     	    |      0x04    | 恢复合成语音
|-------------|-------------|--------------|----------------
------------------------------------------------------------------------------------------------
*/
/*
功能：停止/暂停/恢复合成语音
参数--- 0x02~0x04分别表示停止，暂停，恢复
*/
void Yang_Voice_3Choose1(uint8_t num)
{
    Voice_TX_3choose1_Arr[3] = num;	//命令字
    Send_ZigbeeData_To_Fifo(Voice_TX_3choose1_Arr, 4);	//发送ZigBee命令
    Yang_ArrInit(Voice_TX_3choose1_Arr);	//还原数组
    delay_ms(100);	//延时100ms
}
/*
附录1：
帧头第一位--0x55,帧头第二位--0x06，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|----------|----------|-----------------------------------
|        |	 0x4E	 |          |          | 语音忙碌，正在合成语音(回传命令)
|   0x01 |-----------|   0x00   |    0x00  |-----------------------------------
|        |	 0x4F    |          |          | 语音空闲(回传命令)
|--------|-----------|----------|----------|-----------------------------------
--------------------------------------------------------------------------------
*/
/*
待写
这个暂时不知道是自动回传还是怎么样没有请求的命令！
*/


/***************************************************************************
									道闸标志物控制区
****************************************************************************/

/*
附录1：
帧头第一位--0x55,帧头第二位--0x03，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|-----------------------------------
|        |	 0x01	 |           |           | 开启闸门
|   0x01 |-----------|    0x00   |    0x00   |-----------------------------------
|        |	 0x02	 |           |           | 关闭闸门
|--------|-----------|-----------|-----------|-----------------------------------
|        |	 0x01	 |           |           | 闸门上升(初始角度调节目的是控制道闸处于关闭状态时，闸门处于水平状态设备固件更新后仅需要设置一次,掉电保护)
|   0x09 |-----------|    0x00   |    0x00   |-----------------------------------
|        |	 0x02	 |           |           | 闸门下降
|--------|-----------|-----------|-----------|-----------------------------------
--------------------------------------------------------------------------------
*/
/*
功能：开启闸门/关闭闸门
参数：SET--开启  RESET--关闭
*/
void Yang_Gate_OffOrOn(uint8_t swch)
{
    uint8_t checksum;
    Gate_TX_Arr[2] = 0x01;	//主指令

    if(SET == swch)
    {
        Gate_TX_Arr[3] = 0x01;	//副指令1
        checksum = Yang_CheckSum_1(Gate_TX_Arr[2], Gate_TX_Arr[3], Gate_TX_Arr[4], Gate_TX_Arr[5]);	//计算校验和
        Gate_TX_Arr[6] = checksum;
        Send_ZigbeeData_To_Fifo(Gate_TX_Arr, 8);	//发送开启命令
        Yang_ArrInit(Gate_TX_Arr);	//还原数组
        delay_ms(100);	//延时100ms
        do	//判断闸门是否开启如果没开启则再发
        {
            LandMark_TimeA++;
            if((Yang_InquireGate_State()) == SET)	//如果是开启状态则直接退出
            {
                break;
            }
            else if((Yang_InquireGate_State()) == RESET)	//如果是关闭状态再发送命令
            {
                delay_ms(500);	//延时多点不然可能发送太快了
                Yang_Gate_OffOrOn(SET);
            }
            else if(5 == LandMark_TimeA)	//超时也退出
            {
                LandMark_TimeA = 0;
                break;
            }
        }
        while(1);
    }
    else if(RESET == swch)
    {
        Gate_TX_Arr[3] = 0x02;	//副指令1
        checksum = Yang_CheckSum_1(Gate_TX_Arr[2], Gate_TX_Arr[3], Gate_TX_Arr[4], Gate_TX_Arr[5]);	//计算校验和
        Gate_TX_Arr[6] = checksum;
        Send_ZigbeeData_To_Fifo(Gate_TX_Arr, 8);	//发送关闭命令
        Yang_ArrInit(Gate_TX_Arr);	//还原数组
        delay_ms(100);	//延时100ms
        do
        {
            LandMark_TimeA++;
            if((Yang_InquireGate_State()) == RESET)
            {
                break;
            }
            else if((Yang_InquireGate_State()) == SET)
            {
                delay_ms(500);	//延时多点不然可能发送太快了
                Yang_Gate_OffOrOn(RESET);
            }
            else if(5 == LandMark_TimeA)	//超时也退出
            {
                break;
            }
        }
        while(1);
    }
}
/*
附录1：
帧头第一位--0x55,帧头第二位--0x03，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|---------|-----------------------------------
|   0x10 |	 0xXX	 |   0xXX    |   0xXX  | 车牌前3位数据(ASCII)
|--------|-----------|-----------|---------|-----------------------------------
|   0x11 |	 0xXX	 |   0xXX    |   0xXX  | 车牌后三位数据(ASCII)
|--------|-----------|-----------|---------|-----------------------------------
--------------------------------------------------------------------------------
*/
/*
功能：发送车牌数据到闸门标志物
参数：字符串(ASCII，6个字符 比如"A12300")
注意--*(str+0)是取字符串的第0位数据，*(str+1)是第1位数据...以此类推
*/
void Yang_Gate_CarCard(char *str)
{
    uint8_t checksum;

    Gate_TX_Arr[2] = 0x10;	//主指令
    Gate_TX_Arr[3] = *(str + 0);     // 副指令1---车牌数据【1】
    Gate_TX_Arr[4] = *(str + 1);     // 副指令2---车牌数据【2】
    Gate_TX_Arr[5] = *(str + 2);     // 副指令3---车牌数据【3】
    checksum = Yang_CheckSum_1(Gate_TX_Arr[2], Gate_TX_Arr[3], Gate_TX_Arr[4], Gate_TX_Arr[5]);	//计算校验和
    Gate_TX_Arr[6] = checksum;
    Send_ZigbeeData_To_Fifo(Gate_TX_Arr, 8);     // 先发送车牌前3位Zigbee数据
    delay_ms(400);	//延时
    Gate_TX_Arr[2] = 0x11;	//主指令
    Gate_TX_Arr[3] = *(str + 3);     // 副指令1---车牌数据【4】
    Gate_TX_Arr[4] = *(str + 4);     // 副指令2---车牌数据【5】
    Gate_TX_Arr[5] = *(str + 5);     // 副指令3---车牌数据【6】
    checksum = Yang_CheckSum_1(Gate_TX_Arr[2], Gate_TX_Arr[3], Gate_TX_Arr[4], Gate_TX_Arr[5]);	//计算校验和
    Gate_TX_Arr[6] = checksum;
    Send_ZigbeeData_To_Fifo(Gate_TX_Arr, 8);     // 再发送车牌后3位Zigbee数据
    Yang_ArrInit(Gate_TX_Arr);	//还原数组
    delay_ms(100);	//延时
}
/*
附录1：
帧头第一位--0x55,帧头第二位--0x03，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|---------|-----------------------------------
|   0x20 |	 0x01	 |   0x00    |         | 请求回传道闸状态
|--------|-----------|-----------|  0x00   |------------------------------------
|   0x01 |	 0x00	 |   0x05    |         | 道闸处于开启状态(回传命令)道闸标志物处于关闭状态时请求回传状态，不会回传任何指令。
|--------|-----------|-----------|---------|-----------------------------------
--------------------------------------------------------------------------------
*/
/*
功能：请求回传闸门状态且判断回传值
返回值--SET表示是开启状态，RESET--表示关闭状态
*/
uint8_t Yang_InquireGate_State(void)
{
    uint8_t checksum;

    Gate_TX_Arr[2] = 0x20;	//主指令
    Gate_TX_Arr[3] = 0x01;	//副指令1
    checksum = Yang_CheckSum_1(Gate_TX_Arr[2], Gate_TX_Arr[3], Gate_TX_Arr[4], Gate_TX_Arr[5]);	//计算校验和
    Gate_TX_Arr[6] =  checksum;	//校验和
    Send_ZigbeeData_To_Fifo(Gate_TX_Arr, 8);	//通过ZigBee发送命令
    Yang_ArrInit(Gate_TX_Arr);	//还原数组
    delay_ms(100);
    if(Zigbee_Rx_flag == 1)	 //zigbee返回信息
    {
        Zigbee_Rx_flag = 0;	//接收标志位清0
        if((Zigb_Rx_Buf[0] == 0x55) && (Zigb_Rx_Buf[1] == 0x03))	//判断是不是闸门标志物发送过来的
        {
            if((Zigb_Rx_Buf[2] == 0x01) && (Zigb_Rx_Buf[4] == 0x05))	//判断是不是闸门状态回传
            {
                return SET;	//表示当前是开启状态
            }
        }
    }
    else
    {
        Send_InfoData_To_Fifo("Gate error\n", 12);	//异常显示
    }
    return RESET;	//表示当前是关闭状态
}

/***************************************************************************
									LED显示标志物控制区
****************************************************************************/

/*
附录1：
帧头第一位--0x55,帧头第二位--0x04，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|---------|-----------------------------------
|   0x01 |	 0xXX	 |   0xXX    |   0xXX  | 第一排数码管显示数据(0xXX格式为十六进制)显示时是直接显示XX
|--------|-----------|-----------|---------|-----------------------------------
|   0x02 |	 0xXX	 |   0xXX    |   0xXX  | 第二排数码管显示数据(0xXX格式为十六进制)显示时是直接显示XX
|--------|-----------|-----------|---------|-----------------------------------
--------------------------------------------------------------------------------
*/
/*
功能：LED显示标志物数码管第一排显示数据/第二排显示数据
参数1---需要显示的数据十六进制 参数2---需要显示的数据十六进制 参数3---需要显示的数据十六进制 参数4---选择第一排还是第二排显示0x01 or 0x02
*/
void Yang_LED_ShowData(uint8_t one, uint8_t two, uint8_t three, uint8_t choose)
{
    uint8_t checksum;

    LED_TX_Arr[2] = choose;	//主命令(0x01/0x02第一排/第二排)
    LED_TX_Arr[3] = one;	//副指令1
    LED_TX_Arr[4] = two;	//副指令2
    LED_TX_Arr[5] = three;	//副指令3
    checksum = Yang_CheckSum_1(LED_TX_Arr[2], LED_TX_Arr[3], LED_TX_Arr[4], LED_TX_Arr[5]);	//计算校验和
    LED_TX_Arr[6] = checksum;
    Send_ZigbeeData_To_Fifo(LED_TX_Arr, 8);	//发送数据
    Yang_ArrInit(LED_TX_Arr);	//还原数组
    delay_ms(500);	//如果需要连续发第一排第二排则需要延时500+，低于500可能会只改变一行另一行不会变
}
/*
附录1：
帧头第一位--0x55,帧头第二位--0x04，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|---------|-----------------------------------
|        |	 0x00	 |           |         | 计时关闭
|        |-----------|           |         |-----------------------------------
|  0x03  |	 0x01	 |    0x00   |  0x00   | 计时开启(计时到5分钟蜂鸣器会响)
|        |-----------|           |         |-----------------------------------
|        |	 0x02	 |           |         | 计时清0
|--------|-----------|-----------|---------|-----------------------------------
--------------------------------------------------------------------------------
*/
/*
功能：LED显示标志物第1排显示计时模式
参数---0表示计时关闭 1表示计时开启 2表示计时清0
*/
void Yang_LED_Timing(uint8_t choose)
{
    uint8_t checksum;

    LED_TX_Arr[2] = 0x03;	//主命令(0x01/0x02第一排/第二排)
    switch(choose)
    {
    case 0:
        LED_TX_Arr[3] = 0x00;
        break;	//副指令1
    case 1:
        LED_TX_Arr[3] = 0x01;
        break;	//副指令1
    case 2:
        LED_TX_Arr[3] = 0x02;
        break;	//副指令1
    }
    checksum = Yang_CheckSum_1(LED_TX_Arr[2], LED_TX_Arr[3], LED_TX_Arr[4], LED_TX_Arr[5]);	//计算校验和
    LED_TX_Arr[6] = checksum;
    Send_ZigbeeData_To_Fifo(LED_TX_Arr, 8);	//发送数据
    Yang_ArrInit(LED_TX_Arr);	//还原数组
    delay_ms(100);
}
/*
附录1：
帧头第一位--0x55,帧头第二位--0x04，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|---------|-----------------------------------
|        |           |           |         |
|        |           |           |         |
|  0x04  |    0x00   |    0x0X   |  0xXX   |“X”为要显示的距离值，单位为毫米，显示格式为十进制（显示范围只能0~999,超过的只取个十百位）
|        |           |           |         |注意---测量误差：±20mm
|        |	     	 |           |         |注意---LED屏显示格式：JL-xxx(xxx范围是0~999)
|--------|-----------|-----------|---------|-----------------------------------
--------------------------------------------------------------------------------
*/
/*
功能：LED显示标志物第2排显示距离
参数---距离值
返回值---距离值
*/
uint16_t Yang_LED_Removing(uint16_t pdis)
{
    uint8_t checksum;
    uint16_t Ultrasound_pv;	//距离显示的缓存变量

    Ultrasound_pv = pdis;
#if 1
    Ultrasonic_Ranging();   //获取超声波数据
    Ultrasound_pv = dis;  //记录超声波数据
#endif
    LED_TX_Arr[2] = 0x04;	//主指令
    LED_TX_Arr[4] = Ultrasound_pv / 100 % 10;	//获取高位
    LED_TX_Arr[5] = ((Ultrasound_pv / 10 % 10) * 16 + Ultrasound_pv % 10);	//剩余低位转化成十六进制
    checksum = Yang_CheckSum_1(LED_TX_Arr[2], LED_TX_Arr[3], LED_TX_Arr[4], LED_TX_Arr[5]);	//计算校验和
    LED_TX_Arr[6] = checksum;
    Send_ZigbeeData_To_Fifo(LED_TX_Arr, 8);	//发送数据
    Yang_ArrInit(LED_TX_Arr);	//还原数组
    delay_ms(100);
    return Ultrasound_pv;
}


/***************************************************************************
									立体车库标志物控制区
****************************************************************************/

/*
附录1：
帧头第一位--0x55,帧头第二位--设备A是0x0D 设备B是0x05，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|---------|-----------------------------------
|        |	 0x01	 |           |         | 到达第一层
|        |-----------|           |         |-----------------------------------
|  0x01  |	 0x02	 |           |         | 到达第二层
|        |-----------|    0x00   |  0x00   |-----------------------------------
|        |	 0x03	 |           |         | 到达第三层
|        |-----------|           |         |-----------------------------------
|        |	 0x04	 |           |         | 到达第四层
|--------|-----------|-----------|---------|-----------------------------------
*/
/*
功能：控制立体车库A/B到达第几层
参数1---设备A/B 参数2---到第几层(1~4)
注意---当前我们买的标志物是设备A（以实际为准）
*/
void Yang_GarageWent(uint8_t device, uint8_t floor)
{
    uint8_t checksum;
#if 1	//需要回传则打开	
    uint8_t temp;	//临时变量接收请求回传命令的返回值
    uint8_t i, j;	//i--循环索引，j--需要循环的次数

    temp = Yang_InquireGarage_Floor(device);	//先获得层数
#endif
    if('A' == device)	//设备A
    {
        Garage_TX_Arr[1] = GARAGE_DEVICE_A;	//帧头第2位(设备A)
    }
    else if('B' == device)	//设备B
    {
        Garage_TX_Arr[1] = GARAGE_DEVICE_B;	//帧头第2位(设备B)
    }
    Garage_TX_Arr[2] = 0x01;	//主指令
    Garage_TX_Arr[3] = floor;	//副指令1(层数)
    checksum = Yang_CheckSum_1(Garage_TX_Arr[2], Garage_TX_Arr[3], Garage_TX_Arr[4], Garage_TX_Arr[5]);	//计算校验和
    Garage_TX_Arr[6] = checksum;	//校验和
    Send_ZigbeeData_To_Fifo(Garage_TX_Arr, 8);	//发送控制命令
#if 1	//为了以防万一多发一次
    delay_ms(200);
    Send_ZigbeeData_To_Fifo(Garage_TX_Arr, 8);	//发送控制命令
#endif
    Yang_ArrInit(Garage_TX_Arr);	//还原数组
#if 1	//需要回传则打开
    switch(temp >= floor ? temp - floor : floor - temp)	//三目运算保证结果是正数
    {
    case 1:
        j = 6;	//从第1层到第3层至少6s
        break;
    case 2:
        j = 11;	//从第1层到第3层至少11s
        break;
    case 3:
        j = 17;	//从第1层到第4层至少17s
        break;
    default:
        j = 0;
        delay_ms(500);	//至少需要延时500ms才能再发命令不然下面的命令发送失败
        break;
    }
    for(i = 0; i < j; i++)
    {
        delay_ms(500);
        delay_ms(500);	//延时
    }
    temp = Yang_InquireGarage_Floor(device);
    if(temp == floor)
    {
        Send_InfoData_To_Fifo("Inquire and Send same!\n", 24);	//验证
    }
    else
    {
        Send_InfoData_To_Fifo("Inquire and Send Different!\n", 29);	//验证
    }
#endif
    delay_ms(100);	//延时
}



/*
附录1：
帧头第一位--0x55,帧头第二位--设备A是0x0D 设备B是0x05，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|----------------------------------
|  0x02  |   0x01    |     0x00  |     0x00  | 请求返回车库位于第几层
|--------|-----------|-----------|-----------|----------------------------------
|        |           |   0x01    |           | 返回车库位于第一层（回传命令）
|        |           |-----------|           |----------------------------------
|        |           |   0x02    |           | 返回车库位于第二层（回传命令）
|  0x03  |   0x01    |-----------|   0x00    |----------------------------------
|        |           |   0x03    |           | 返回车库位于第三层（回传命令）
|        |           |-----------|           |----------------------------------
|        |           |   0x04    |           | 返回车库位于第四层（回传命令）
|--------|-----------|-----------|-----------|----------------------------------
--------------------------------------------------------------------------------
*/
/*
功能：请求回传车库位于第几层
参数---设备A/B
返回值---对应层数 0表示异常
*/
uint8_t Yang_InquireGarage_Floor(uint8_t device)
{
    uint8_t checksum;
    uint8_t RXA_floor;	//存储设备A回传的层数临时变量
    uint8_t RXB_floor;	//存储设备A回传的层数临时变量
#if 0	//测试再开启
    uint8_t arr[21];
#endif

    if('A' == device)	//设备A
    {
        Garage_TX_Arr[1] = GARAGE_DEVICE_A;	//帧头第2位(设备A)
    }
    else if('B' == device)	//设备B
    {
        Garage_TX_Arr[1] = GARAGE_DEVICE_B;	//帧头第2位(设备B)
    }
    Garage_TX_Arr[2] = 0x02;	//主指令
    Garage_TX_Arr[3] = 0x01;	//副指令1
    checksum = Yang_CheckSum_1(Garage_TX_Arr[2], Garage_TX_Arr[3], Garage_TX_Arr[4], Garage_TX_Arr[5]);	//计算校验和
    Garage_TX_Arr[6] = checksum;	//校验和
    Send_ZigbeeData_To_Fifo(Garage_TX_Arr, 8);	//发送请求回传命令
    Yang_ArrInit(Garage_TX_Arr);	//还原数组
    delay_ms(90);	//延时不能太长也不能太短否则接收不到
    if(Zigbee_Rx_flag == 1)	 //zigbee返回信息
    {
        Zigbee_Rx_flag = 0;	//接收标志位清0
        if(Zigb_Rx_Buf[0] == 0x55)
        {
            switch(Zigb_Rx_Buf[1])
            {
            case GARAGE_DEVICE_A:	//设备A回传
            {
                if((Zigb_Rx_Buf[2] == 0x03) && (Zigb_Rx_Buf[3] == 0x01))	//判断是不是立体车库层数回传主指令
                {
                    RXA_floor = Zigb_Rx_Buf[4];
#if 0	//测试再开启
                    sprintf((char *)arr, "A--%d\n", RXA_floor);
                    Send_InfoData_To_Fifo((char *)arr, 10);
#endif
                    delay_ms(100);	//延时100ms
                    return RXA_floor;	//返回设备A层数
                }
                break;
            }
            case GARAGE_DEVICE_B:	//设备B回传
            {
                if((Zigb_Rx_Buf[2] == 0x03) && (Zigb_Rx_Buf[3] == 0x01))	//判断是不是立体车库层数回传主指令
                {
                    RXB_floor = Zigb_Rx_Buf[4];
                    delay_ms(100);	//延时100ms
                    return RXB_floor;	//返回设备B层数
                }
                break;
            }
            default:
                break;
            }
        }
    }
    else
    {
        Send_InfoData_To_Fifo("Garage_Floor error\n", 20);	//异常显示
    }
    delay_ms(100);	//延时100ms
    return 0;
}
/*
附录1：
帧头第一位--0x55,帧头第二位--设备A是0x0D 设备B是0x05，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|----------------------------------
|  0x02  |   0x02    |    0x00   |     0x00  |请求返回前后侧红外状态
|--------|-----------|-----------|-----------|----------------------------------
|        |           |0x01未触发  | 0x01未触发|
|  0x03  |    0x02   |0x02 触发  | 0x02 触发  | 返回车库前后侧红外对管状态（回传命令）这里跟通信协议相反协议是0x01触发0x02未触发
|        |           | - 前侧 -  | - 后侧 -   |
--------------------------------------------------------------------------------
*/
/*
功能：请求回传车库红外状态
参数---设备A/B
返回值---红外状态
*/
void Yang_Garage_Infrared(uint8_t device)
{
    uint8_t checksum;
#if 1	//测试再开启
    uint8_t arr[12];
#endif

    if('A' == device)	//设备A
    {
        Garage_TX_Arr[1] = GARAGE_DEVICE_A;	//帧头第2位(设备A)
    }
    else if('B' == device)	//设备B
    {
        Garage_TX_Arr[1] = GARAGE_DEVICE_B;	//帧头第2位(设备B)
    }
    Garage_TX_Arr[2] = 0x02;	//主指令
    Garage_TX_Arr[3] = 0x02;	//副指令1
    checksum = Yang_CheckSum_1(Garage_TX_Arr[2], Garage_TX_Arr[3], Garage_TX_Arr[4], Garage_TX_Arr[5]);	//计算校验和
    Garage_TX_Arr[6] = checksum;	//校验和
    Send_ZigbeeData_To_Fifo(Garage_TX_Arr, 8);	//发送请求回传命令
    Yang_ArrInit(Garage_TX_Arr);	//还原数组
    delay_ms(50);
    if(Zigbee_Rx_flag == 1)	 //zigbee返回信息
    {
        Zigbee_Rx_flag = 0;	//接收标志位清0
        if(Zigb_Rx_Buf[0] == 0x55)
        {
            if(Zigb_Rx_Buf[2] == 0x03)
            {
                if((Zigb_Rx_Buf[4] == 0x01) && (Zigb_Rx_Buf[5] == 0x01))
                {
                    Garage_Infrared_Flag = 3;
                }
                if((Zigb_Rx_Buf[4] == 0x02) && (Zigb_Rx_Buf[5] == 0x02))
                {
                    Garage_Infrared_Flag = 0;
                }
                if((Zigb_Rx_Buf[4] == 0x01) && (Zigb_Rx_Buf[5] == 0x02))
                {
                    Garage_Infrared_Flag = 1;
                }
                if((Zigb_Rx_Buf[4] == 0x02) && (Zigb_Rx_Buf[5] == 0x01))
                {
                    Garage_Infrared_Flag = 2;
                }
                sprintf((char *)arr, "Garage--%d\n", Garage_Infrared_Flag);
                Send_InfoData_To_Fifo((char *)arr, 11);
            }
        }
    }

#if 0	//在ZigBee接收函数里判断即可这里不用	
    delay_ms(50);	//延时
    if(Zigbee_Rx_flag == 1)	 //zigbee返回信息
    {
        Zigbee_Rx_flag = 0;	//接收标志位清0
        if(Zigb_Rx_Buf[0] == 0x55)
        {
            switch(Zigb_Rx_Buf[1])	//设备
            {
            case GARAGE_DEVICE_A:	//设备A回传
            {
                if((Zigb_Rx_Buf[2] == 0x03) && (Zigb_Rx_Buf[3] == 0x02))	//判断是不是立体车库前后红外回传主指令
                {
                    if((Zigb_Rx_Buf[4] == 0x01) && (Zigb_Rx_Buf[5] == 0x01))	//都不触发
                    {
                        Garage_Infrared_Flag = 3;
                    }
                    else if((Zigb_Rx_Buf[4] == 0x01) && (Zigb_Rx_Buf[5] == 0x02))	//前侧不触发后侧触发
                    {
                        Garage_Infrared_Flag = 1;
                    }
                    else if((Zigb_Rx_Buf[4] == 0x02) && (Zigb_Rx_Buf[5] == 0x01))	//前侧触发后侧不触发
                    {
                        Garage_Infrared_Flag = 2;
                    }
                    else	//都触发
                    {
                        Garage_Infrared_Flag = 0;
                    }
#if 1	//测试再开启
                    sprintf((char *)arr, "Garage--%d\n", Garage_Infrared_Flag);
                    Send_InfoData_To_Fifo((char *)arr, 11);
#endif
                }
                break;
            }
            case GARAGE_DEVICE_B:	//设备B回传
            {
                if((Zigb_Rx_Buf[2] == 0x03) && (Zigb_Rx_Buf[3] == 0x02))	//判断是不是立体车库前后红外回传主指令
                {
                    if((Zigb_Rx_Buf[4] == 0x01) && (Zigb_Rx_Buf[5] == 0x01))	//都不触发
                    {
                        Garage_Infrared_Flag = 3;
                    }
                    else if((Zigb_Rx_Buf[4] == 0x01) && (Zigb_Rx_Buf[5] == 0x02))	//前侧不触发后侧触发
                    {
                        Garage_Infrared_Flag = 1;
                    }
                    else if((Zigb_Rx_Buf[4] == 0x02) && (Zigb_Rx_Buf[5] == 0x01))	//前侧触发后侧不触发
                    {
                        Garage_Infrared_Flag = 2;
                    }
                    else	//都触发
                    {
                        Garage_Infrared_Flag = 0;
                    }
                }
                break;
            }
            default:
                break;
            }
        }
    }
    else
    {
        Send_InfoData_To_Fifo("Garage_Infrared error\n", 23);	//异常显示
    }
    delay_ms(100);	//延时100ms
#endif
}


/***************************************************************************
									烽火台报警标志物控制区
****************************************************************************/

/*
附录1：
帧头第一位--0x55,帧头第二位--0x07，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|----------------------------------
|  0x10  |   0xXX    |    0xXX   |    0xXX   |发送前3位开启码数据
|--------|-----------|-----------|-----------|----------------------------------
|  0x11  |   0xXX    |    0xXX   |    0xXX   | 发送后3位开启码数据
--------------------------------------------------------------------------------
*/
/*
功能：向烽火台报警器发送开启码(ZigBee发送)
参数---含开启码的数组(6字节)
*/
#if 0	//屏蔽不用管(工程师说用红外控制ZigBee控不了)
void Yang_Beacon_Open(uint8_t *temp)
{
    uint8_t checksum;

    Beacon_TX_Arr[2] = 0x10;	//主指令【前3位开启码】
    Beacon_TX_Arr[3] = temp[0];	//副指令1【开启码第1位】
    Beacon_TX_Arr[4] = temp[1];	//副指令2【开启码第2位】
    Beacon_TX_Arr[5] = temp[2];	//副指令3【开启码第3位】
    checksum = Yang_CheckSum_1(Beacon_TX_Arr[2], Beacon_TX_Arr[3], Beacon_TX_Arr[4], Beacon_TX_Arr[5]);	//计算校验和
    Beacon_TX_Arr[6] = checksum;	//校验和
    Send_ZigbeeData_To_Fifo(Beacon_TX_Arr, 8);	//通过ZigBee发送命令
    delay_ms(400);	//延时
    Beacon_TX_Arr[2] = temp[3];	//主指令【后3位开启码】
    Beacon_TX_Arr[3] = temp[4];	//副指令1【开启码第4位】
    Beacon_TX_Arr[4] = temp[5];	//副指令2【开启码第5位】
    Beacon_TX_Arr[5] = temp[6];	//副指令3【开启码第6位】
    checksum = Yang_CheckSum_1(Beacon_TX_Arr[2], Beacon_TX_Arr[3], Beacon_TX_Arr[4], Beacon_TX_Arr[5]);	//计算校验和
    Beacon_TX_Arr[6] = checksum;	//校验和
    Send_ZigbeeData_To_Fifo(Beacon_TX_Arr, 8);	//通过ZigBee发送命令
    Yang_ArrInit(Beacon_TX_Arr);	//还原数组
    delay_ms(100);
}
/*
附录1：
帧头第一位--0x55,帧头第二位--0x07，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|----------------------------------
|  0x09  |   0x00    |    0x00   |    0x00   |请求回传随机救援坐标
|--------|-----------|-----------|-----------|----------------------------------
|  0x01  |   0xXX    |    0x00   |    0x00   |回传的随机救援坐标点(回传命令)
|--------|-----------|-----------|-----------|----------------------------------
--------------------------------------------------------------------------------
*/
/*
功能：请求回传随机救援坐标(ZigBee发送)
*/
void Yang_Beacon_InquireCoordinate(void)
{
    uint8_t checksum;
#if 1	//测试再开启
    uint8_t arr[11];
#endif

    Beacon_TX_Arr[2] = 0x09;	//主指令
    checksum = Yang_CheckSum_1(Beacon_TX_Arr[2], Beacon_TX_Arr[3], Beacon_TX_Arr[4], Beacon_TX_Arr[5]);	//计算校验和
    Beacon_TX_Arr[6] = checksum;	//校验和
    Send_ZigbeeData_To_Fifo(Beacon_TX_Arr, 8);	//通过ZigBee发送命令
    delay_ms(90);
    if(Zigbee_Rx_flag == 1)	 //zigbee返回信息
    {
        Zigbee_Rx_flag = 0;	//接收标志位清0
        if((Zigb_Rx_Buf[0] == 0x55) && (Zigb_Rx_Buf[1] == 0x07))	//判断是不是烽火台报警器标志物发送过来的
        {
            if(Zigb_Rx_Buf[2] == 0x01)	//判断是不是随机坐标回传
            {
                Beacon_Coordinate = Zigb_Rx_Buf[3];	//存储随机坐标
#if 1	//测试再开启
                sprintf((char *)arr, "sj--%#x\n", Beacon_Coordinate);
                Send_InfoData_To_Fifo((char *)arr, 10);
#endif
            }
        }
    }
    else
    {
        Send_InfoData_To_Fifo("Beacon error\n", 14);	//异常显示
    }
    Yang_ArrInit(Beacon_TX_Arr);	//还原数组
    delay_ms(100);
}
#endif
/*
附录1：
----------------------------------------------------------------------------------------
| 数据1  |   数据2    |   数据3   |   数据4   |  数据5  |   数据6  |			说明
|--------|-----------|-----------|-----------|---------|---------|----------------------
|  0xXX  |   0xXX    |    0xXX   |    0xXX   |   0xXX  |   0xXX  |6字节开启码(默认开启码：0x03,0x05,0x14,0x45,0xDE,0x92 )
|        |           |           |           |         |         | 注意---开启码错误是不能报警的(以实际为准)
|--------|-----------|-----------|-----------|---------|---------|----------------------
----------------------------------------------------------------------------------------
*/
/*
功能：向烽火台报警器发送开启码(红外发送)
参数---6Byte数组
*/
void Yang_Beacon_Open2(uint8_t *temp)
{
    Beacon_TX_Arr_Infrared[0] = temp[0];	//【数据1】
    Beacon_TX_Arr_Infrared[1] = temp[1];	//【数据2】
    Beacon_TX_Arr_Infrared[2] = temp[2];	//【数据3】
    Beacon_TX_Arr_Infrared[3] = temp[3];	//【数据4】
    Beacon_TX_Arr_Infrared[4] = temp[4];	//【数据5】
    Beacon_TX_Arr_Infrared[5] = temp[5];	//【数据6】
    Infrared_Send(Beacon_TX_Arr_Infrared, 6);	//红外发送命令
#if 1	//以防万一再发一次
    delay_ms(500);
    Infrared_Send(Beacon_TX_Arr_Infrared, 6);	//红外发送命令
#endif
    delay_ms(100);	//延时
}

/***************************************************************************
									智能TFT显示标志物控制区
****************************************************************************/

/*
附录1：
帧头第一位--0x55,帧头第二位设备A是0x0B；设备B是0x08，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|-----------------------------------
|  0x10  |	 0x00	 | 0x01~0x20 |   0x00    | 由第二副指令指定显示那张图片
|--------|-----------|-----------|-----------|-----------------------------------
*/
/*
功能：控制TFT标志物A/B显示指定图片
参数1---设备A/B 参数2---要显示的图片编号(0x01~0x20)
*/
void Yang_TFT_Assign(uint8_t device, uint8_t ID)
{
    uint8_t checksum;

    if('A' == device)	//设备A
    {
        TFT_TX_Arr[1] = TFT_DEVICE_A;	//帧头第2位(设备A)
    }
    else if('B' == device)	//设备B
    {
        TFT_TX_Arr[1] = TFT_DEVICE_B;	//帧头第2位(设备B)
    }
    TFT_TX_Arr[2] = 0x10;	//主指令
    TFT_TX_Arr[4] = ID;	//副指令2(图片ID)
    checksum = Yang_CheckSum_1(TFT_TX_Arr[2], TFT_TX_Arr[3], TFT_TX_Arr[4], TFT_TX_Arr[5]);	//计算校验和
    TFT_TX_Arr[6] = checksum;	//校验和
    Send_ZigbeeData_To_Fifo(TFT_TX_Arr, 8);	//发送控制命令
#if 1	//多发一次稳点
    delay_ms(50);
    Send_ZigbeeData_To_Fifo(TFT_TX_Arr, 8);	//发送控制命令
#endif
    Yang_ArrInit(TFT_TX_Arr);	//还原数组
    delay_ms(100);
}
/*
附录1：
帧头第一位--0x55,帧头第二位设备A是0x0B；设备B是0x08，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|-----------------------------------
|        |	 0x01    |	     	 |           | 图片向上翻页
|        |-----------|           |           |-----------------------------------
|  0x10  |   0x02    |    0x00   |   0x00    | 图片向下翻页
|        |-----------|           |           |-----------------------------------
|		 |   0x03    |           |           | 图片自动向下翻页显示，间隔时间 10S
|--------|-----------|-----------|-----------|-----------------------------------
*/
/*
功能：控制TFT标志物A/B图片手动翻页/自动翻页
参数1---设备A/B 参数2---功能选择(1---向上翻 2---向下翻 3---自动向下翻)
注意---自动翻页没效果
*/
void Yang_TFT_TurnPage(uint8_t device, uint8_t choose)
{
    uint8_t checksum;

    if('A' == device)	//设备A
    {
        TFT_TX_Arr[1] = TFT_DEVICE_A;	//帧头第2位(设备A)
    }
    else if('B' == device)	//设备B
    {
        TFT_TX_Arr[1] = TFT_DEVICE_B;	//帧头第2位(设备B)
    }
    TFT_TX_Arr[2] = 0x10;	//主指令
    TFT_TX_Arr[3] = choose;	//副指令2(功能)
    checksum = Yang_CheckSum_1(TFT_TX_Arr[2], TFT_TX_Arr[3], TFT_TX_Arr[4], TFT_TX_Arr[5]);	//计算校验和
    TFT_TX_Arr[6] = checksum;	//校验和
    Send_ZigbeeData_To_Fifo(TFT_TX_Arr, 8);	//发送控制命令
    Yang_ArrInit(TFT_TX_Arr);	//还原数组
    delay_ms(100);
}
/*
附录1：
帧头第一位--0x55,帧头第二位设备A是0x0B；设备B是0x08，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|-----------------------------------
|  0x20  |	 0xXX    |	 0xXX	 |   0xXX	 | 车牌前三位数据（ASCII）
|--------|-----------|-----------|-----------|-----------------------------------
|  0x21  |	 0xXX	 |	 0xXX	 |   0xXX	 | 车牌后三位数据（ASCII）
--------------------------------------------------------------------------------
*/
/*
功能：控制TFT标志物A/B显示车牌模式
参数1---设备A/B 参数2---字符串(ASCII，6个字符 比如"A12300")
注意---自动翻页没效果
*/
void Yang_TFT_CarCard(uint8_t device, char *str)
{
    uint8_t checksum;

    if('A' == device)	//设备A
    {
        TFT_TX_Arr[1] = TFT_DEVICE_A;	//帧头第2位(设备A)
    }
    else if('B' == device)	//设备B
    {
        TFT_TX_Arr[1] = TFT_DEVICE_B;	//帧头第2位(设备B)
    }
    TFT_TX_Arr[2] = 0x20;	//主指令
    TFT_TX_Arr[3] = *(str + 0);     // 副指令1---车牌数据【1】
    TFT_TX_Arr[4] = *(str + 1);     // 副指令2---车牌数据【2】
    TFT_TX_Arr[5] = *(str + 2);     // 副指令3---车牌数据【3】
    checksum = Yang_CheckSum_1(TFT_TX_Arr[2], TFT_TX_Arr[3], TFT_TX_Arr[4], TFT_TX_Arr[5]);	//计算校验和
    TFT_TX_Arr[6] = checksum;
    Send_ZigbeeData_To_Fifo(TFT_TX_Arr, 8);     // 先发送车牌前3位Zigbee数据
#if 1	//多发一次稳点
    delay_ms(50);
    Send_ZigbeeData_To_Fifo(TFT_TX_Arr, 8);     // 先发送车牌前3位Zigbee数据
#endif
    delay_ms(200);	//延时
    TFT_TX_Arr[2] = 0x21;	//主指令
    TFT_TX_Arr[3] = *(str + 3);     // 副指令1---车牌数据【4】
    TFT_TX_Arr[4] = *(str + 4);     // 副指令2---车牌数据【5】
    TFT_TX_Arr[5] = *(str + 5);     // 副指令3---车牌数据【6】
    checksum = Yang_CheckSum_1(TFT_TX_Arr[2], TFT_TX_Arr[3], TFT_TX_Arr[4], TFT_TX_Arr[5]);	//计算校验和
    TFT_TX_Arr[6] = checksum;
    Send_ZigbeeData_To_Fifo(TFT_TX_Arr, 8);     // 再发送车牌后3位Zigbee数据
#if 1	//多发一次稳点
    delay_ms(50);
    Send_ZigbeeData_To_Fifo(TFT_TX_Arr, 8);     // 先发送车牌前3位Zigbee数据
#endif
    Yang_ArrInit(TFT_TX_Arr);	//还原数组
    delay_ms(100);	//延时
}
/*
附录1：
帧头第一位--0x55,帧头第二位设备A是0x0B；设备B是0x08，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|-----------------------------------
|        |	 0x00	 |	     	 |       	 | 计时显示模式关闭
|        |-----------|           |           |-----------------------------------
|	0x30 |	 0x01	 |   0x00    |   0x00    | 计时显示模式打开
|        |-----------|           |           |-----------------------------------
| 		 |	 0x02	 |           |           | 计时显示模式清零
--------------------------------------------------------------------------------
*/
/*
功能：控制TFT标志物A/B计时显示模式
参数1---设备A/B 参数2---0表示计时关闭 1表示计时开启 2表示计时清0
*/
void Yang_TFT_Timing(uint8_t device, uint8_t choose)
{
    uint8_t checksum;

    if('A' == device)	//设备A
    {
        TFT_TX_Arr[1] = TFT_DEVICE_A;	//帧头第2位(设备A)
    }
    else if('B' == device)	//设备B
    {
        TFT_TX_Arr[1] = TFT_DEVICE_B;	//帧头第2位(设备B)
    }
    TFT_TX_Arr[2] = 0x30;	//主指令
    TFT_TX_Arr[3] = choose;	//副指令1(选择模式)
    checksum = Yang_CheckSum_1(TFT_TX_Arr[2], TFT_TX_Arr[3], TFT_TX_Arr[4], TFT_TX_Arr[5]);	//计算校验和
    TFT_TX_Arr[6] = checksum;
    Send_ZigbeeData_To_Fifo(TFT_TX_Arr, 8);     //发送Zigbee数据
#if 1	//多发一次稳点
    delay_ms(20);
    Send_ZigbeeData_To_Fifo(TFT_TX_Arr, 8);     //发送Zigbee数据
#endif
    Yang_ArrInit(TFT_TX_Arr);
    delay_ms(100);
}
/*
附录1：
帧头第一位--0x55,帧头第二位设备A是0x0B；设备B是0x08，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|-----------------------------------
|        |	    	 |	         |      	 | 六位显示数据（HEX格式）
|  0x40  |   0xXX    |    0xXX   |   0xXX    |注意---0x+你想要显示的十进制即可(比如显示102299->0x10 0x22 0x99)
|		 |     		 |			 |			 |如果是十进制数则自动转为16进制显示
|--------|-----------|-----------|-----------|-----------------------------------
--------------------------------------------------------------------------------
*/
/*
功能：控制TFT标志物A/B 六位HEX格式数据显示
参数1---设备A/B 参数2--- 3Byte数组
*/
void Yang_TFT_HexShow(uint8_t device, uint8_t *temp)
{
    uint8_t checksum;

    if('A' == device)	//设备A
    {
        TFT_TX_Arr[1] = TFT_DEVICE_A;	//帧头第2位(设备A)
    }
    else if('B' == device)	//设备B
    {
        TFT_TX_Arr[1] = TFT_DEVICE_B;	//帧头第2位(设备B)
    }
    TFT_TX_Arr[2] = 0x40;	//主指令
    TFT_TX_Arr[3] = temp[0];	//副指令1【第一位HEX】
    TFT_TX_Arr[4] = temp[1];	//副指令2【第二位HEX】
    TFT_TX_Arr[5] = temp[2];	//副指令3【第三位HEX】
    checksum = Yang_CheckSum_1(TFT_TX_Arr[2], TFT_TX_Arr[3], TFT_TX_Arr[4], TFT_TX_Arr[5]);	//计算校验和
    TFT_TX_Arr[6] = checksum;
    Send_ZigbeeData_To_Fifo(TFT_TX_Arr, 8);     //发送Zigbee数据
#if 1	//多发一次稳点
    delay_ms(5);
    Send_ZigbeeData_To_Fifo(TFT_TX_Arr, 8);     //发送Zigbee数据
#endif
    Yang_ArrInit(TFT_TX_Arr);
    delay_ms(100);
}
/*
附录1：
帧头第一位--0x55,帧头第二位设备A是0x0B；设备B是0x08，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|-----------------------------------
|        |	     	 |	    	 |      	 |距离显示模式（十进制）“X”为要显示的距离值，单位为毫米，
|  0x50  |   0x00    |   0x0X    |    0xXX   |显示格式为十进制（显示范围只能0~999,超过的只取个十百位）
|        |           |           |           |注意---测量误差：±20mm注意---LED屏显示格式：JL-xxx(xxx范围是0~999)
|        |           |           |           |注意---跟LED显示一样
--------------------------------------------------------------------------------
*/
/*
功能：控制TFT标志物A/B显示距离模式
参数1---设备A/B 参数2---要显示的距离
*/
void Yang_TFT_Removing(uint8_t device, uint16_t pdis)
{
    uint8_t checksum;
    uint16_t Ultrasound_pv;	//距离显示的缓存变量

    Ultrasound_pv = pdis;
    if('A' == device)	//设备A
    {
        TFT_TX_Arr[1] = TFT_DEVICE_A;	//帧头第2位(设备A)
    }
    else if('B' == device)	//设备B
    {
        TFT_TX_Arr[1] = TFT_DEVICE_B;	//帧头第2位(设备B)
    }
#if 0
    Ultrasonic_Ranging();   //获取超声波数据
    Ultrasound_pv = dis;  //记录超声波数据
#endif
    TFT_TX_Arr[2] = 0x50;	//主指令
    TFT_TX_Arr[4] = Ultrasound_pv / 100 % 10;	//获取高位
    TFT_TX_Arr[5] = ((Ultrasound_pv / 10 % 10) * 16 + Ultrasound_pv % 10);	//剩余低位转化成十六进制
    checksum = Yang_CheckSum_1(TFT_TX_Arr[2], TFT_TX_Arr[3], TFT_TX_Arr[4], TFT_TX_Arr[5]);	//计算校验和
    TFT_TX_Arr[6] = checksum;
    Send_ZigbeeData_To_Fifo(TFT_TX_Arr, 8);	//发送数据
#if 1	//多发一次稳点
    delay_ms(5);
    Send_ZigbeeData_To_Fifo(TFT_TX_Arr, 8);     //发送Zigbee数据
#endif
    Yang_ArrInit(TFT_TX_Arr);	//还原数组
    delay_ms(100);
}
/*
附录1：
帧头第一位--0x55,帧头第二位设备A是0x0B；设备B是0x08，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|-----------------------------------
|        |	 0x01	 |	     	 |       	 | 显示交通标志“直行”
|        |-----------|           |           |-----------------------------------
|	     |	 0x02	 |           |           | 显示交通标志“左转”
|        |-----------|           |           |-----------------------------------
| 		 |	 0x03	 |           |           | 显示交通标志“右转”
|  0x60  |-----------|   0x00    |   0x00    |-----------------------------------
| 		 |	 0x04	 |           |           | 显示交通标志“掉头”
|        |-----------|           |           |-----------------------------------
| 		 |	 0x05	 |           |           | 显示交通标志“禁止直行”
|        |-----------|           |           |-----------------------------------
| 		 |	 0x06	 |           |           | 显示交通标志“禁止通行”
--------------------------------------------------------------------------------
*/
/*
功能：控制TFT标志物A/B交通标志显示模式
参数1---设备A/B 参数2---要显示的编号ID(1~6)
*/
void Yang_TFT_TrafficSigns(uint8_t device, uint8_t ID)
{
    uint8_t checksum;

    if('A' == device)	//设备A
    {
        TFT_TX_Arr[1] = TFT_DEVICE_A;	//帧头第2位(设备A)
    }
    else if('B' == device)	//设备B
    {
        TFT_TX_Arr[1] = TFT_DEVICE_B;	//帧头第2位(设备B)
    }
    TFT_TX_Arr[2] = 0x60;	//主指令
    TFT_TX_Arr[3] = ID;	//副指令1(编号ID)
    checksum = Yang_CheckSum_1(TFT_TX_Arr[2], TFT_TX_Arr[3], TFT_TX_Arr[4], TFT_TX_Arr[5]);	//计算校验和
    TFT_TX_Arr[6] = checksum;
    Send_ZigbeeData_To_Fifo(TFT_TX_Arr, 8);	//发送数据
#if 1	//多发一次稳点
    delay_ms(5);
    Send_ZigbeeData_To_Fifo(TFT_TX_Arr, 8);     //发送Zigbee数据
#endif
    Yang_ArrInit(TFT_TX_Arr);	//还原数组
    delay_ms(100);
}

/***************************************************************************
									无线充电标志物控制区
****************************************************************************/

/*
附录1：
帧头第一位--0x55,帧头第二位是0x0A，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|-----------------------------------
|        |	 0x01	 |	     	 |       	 |开启无线充电 (无线充电标志物开启后持续 10 秒)
|  0x08  |-----------|-----------|   0x00    |-----------------------------------
|	     |	 0x02	 |           |           | 关闭无线充电 (期间可使用关闭指令控制标志物关闭无线充电，若 10 秒内未收到关闭指令，则无线充电自动关闭)
--------------------------------------------------------------------------------
*/
/*
功能：控制无线充电标志物开关
参数---SET表示开，RESET表示关
*/
void Yang_WirelessCharging_OffOrOn(uint8_t swch)
{
    uint8_t checksum;

    if(SET == swch)
    {
        WirelessCharging_TX_Arr[3] = 0x01;	//副指令1(开)
    }
    else if(RESET == swch)
    {
        WirelessCharging_TX_Arr[3] = 0x02;	//副指令1(关)
    }
    checksum = Yang_CheckSum_1(WirelessCharging_TX_Arr[2], WirelessCharging_TX_Arr[3], WirelessCharging_TX_Arr[4], WirelessCharging_TX_Arr[5]);	//计算校验和
    WirelessCharging_TX_Arr[6] = checksum;
    Send_ZigbeeData_To_Fifo(WirelessCharging_TX_Arr, 8);	//发送数据
#if 1	//多发一次稳点
    delay_ms(80);
    Send_ZigbeeData_To_Fifo(WirelessCharging_TX_Arr, 8);     //发送Zigbee数据
#endif
    Yang_ArrInit(WirelessCharging_TX_Arr);	//还原数组
    delay_ms(100);
}

/***************************************************************************
									ETC标志物控制区
****************************************************************************/

/*
附录1：
帧头第一位--0x55,帧头第二位是0x0C，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|-----------------------------------
|        |	 0x01	 |   0x01 	 |       	 | 控制 ETC 系统处于关闭状态时，
|        | (左侧上升) |(右侧上升)  |           |
|  0x08  |-----------|-----------|  0x00     | 左右两侧闸门处于水平状态(掉电不丢失)
|	     |	 0x02	 |   0x01    |           |
|        | (左侧下降) | (右侧下降)|           |
--------------------------------------------------------------------------------
*/

/*
附录1：
帧头第一位--0x55,帧头第二位是0x0C，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|-----------------------------------
|  0x01  |	 0x01	 |	0x06     |  0x00     |ETC 系统标志物接收天线在有效区域内感应到小车的磁条，闸门自动开启，
|		 |  		 |			 |			 |同时自动回传闸门开启状态指令，闸门开启 10 秒后自动关闭(自动回传命令)
--------------------------------------------------------------------------------
*/
/*
功能：查询ETC是否已开启
*/

void Yang_ETC_Inquire(void)
{
    LandMark_Flag = 0;	//标志位清0
		
//	Yang_CarThread_Go(20,700,SET);	//前进到ETC面前(走多少距离看实际摆放调试)	
    while(LandMark_Flag != 0x06)
    {
        LandMark_TimeA++;
        delay_ms(1);
        if(LandMark_TimeA >= 400)
        {
            LandMark_TimeA = 0;
            LandMark_TimeB++;
#if 1
            Yang_Car_Back_Infrared(20, 300);	//后退
            delay_ms(100);
            Yang_Car_Back_Infrared(20, 350);	//小车前进
#endif
            if(Zigbee_Rx_flag == 1)	 //zigbee返回信息(开启了才会回传不然不会回传)
            {
                Zigbee_Rx_flag = 0;	//接收标志位清0
                if(Zigb_Rx_Buf[0] == 0x55)
                {
                    if((Zigb_Rx_Buf[1] == 0x0C) && (Zigb_Rx_Buf[2] == 0x01))
                    {
                        LandMark_Flag = Zigb_Rx_Buf[4];	//接收状态(开启状态)
                        USART6_Send_Byte(0x06);
						break;
                    }
                }
            }
        }
        if(LandMark_TimeB >= 4)	//超时退出
        {
            LandMark_TimeB = 0;
            break;
        }
    }
}

//通过ETC测试函数
void Yang_ETC_Go(void)
{
	
}


/***************************************************************************
									智能交通灯标志物控制区
****************************************************************************/

/*
附录1：
帧头第一位--0x55,帧头第二位设备A是0x0E；设备B是0x0F，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1]  | 副指令[2]  | 副指令[3]  |			说明
|--------|-----------|-----------|-----------|-----------------------------------
|  0x01  |	0x00     |	0x00  	 |    0x00	 |进入识别模式
|--------|-----------|-----------|-----------|-----------------------------------
|        |	0x01     |	0x07  	 |       	 |进入识别模式成功(自动回传命令)
| 0x01   |-----------|-----------|   0x00    |-----------------------------------
|        |	0x01     |	0x08  	 |       	 |进入识别模式(自动回传命令)
--------------------------------------------------------------------------------
进入识别模式成功后10s内要发送识别结果，超出时间则全部灯亮表示失败
*/
/*
功能：进入识别模式
参数---设备A/B
*/
void Yang_Traffic_RecognitionPatterns(uint8_t device)
{
    uint8_t checksum;
#if 1	//测试再打开
    uint8_t arr[16];
#endif
    if('A' == device)	//设备A
    {
        SmartTrafficLights_TX_Arr[1] = TRAFFIC_DEVICE_A;	//帧头第2位(设备A)
    }
    else if('B' == device)	//设备B
    {
        SmartTrafficLights_TX_Arr[1] = TRAFFIC_DEVICE_B;	//帧头第2位(设备B)
    }
    SmartTrafficLights_TX_Arr[2] = 0x01;	//主指令
    checksum = Yang_CheckSum_1(SmartTrafficLights_TX_Arr[2], SmartTrafficLights_TX_Arr[3], SmartTrafficLights_TX_Arr[4], SmartTrafficLights_TX_Arr[5]);	//计算校验和
    SmartTrafficLights_TX_Arr[6] = checksum;
    LandMark_Flag = 0;	//标志位清0
    while(LandMark_Flag != 0x07)	//如果回传失败则继续发送
    {
        LandMark_TimeA++;
        if(LandMark_TimeA >= 7)	//发送7次
        {
            LandMark_TimeA = 0;	//清0
            Send_InfoData_To_Fifo("Send LongTime\n", 15);
            break;	//退出程序
        }
        Send_ZigbeeData_To_Fifo(SmartTrafficLights_TX_Arr, 8);	//发送数据
#if 1	//判断回传
        delay_ms(90);
        if(Zigbee_Rx_flag == 1)	 //zigbee返回信息
        {
            Zigbee_Rx_flag = 0;	//接收标志位清0
            if(Zigb_Rx_Buf[0] == 0x55)
            {
                switch(Zigb_Rx_Buf[1])
                {
                case TRAFFIC_DEVICE_A:
                {
                    if((Zigb_Rx_Buf[2] == 0x01) && (Zigb_Rx_Buf[3] == 0x01))	//判断是不是进入识别模式的回传
                    {
                        LandMark_Flag = Zigb_Rx_Buf[4];	//结果是0x07--成功 0x08--失败
#if 1	//测试再打开
                        sprintf((char *)arr, "Traffic--%#x\n", LandMark_Flag);
                        Send_InfoData_To_Fifo((char *)arr, 16);
#endif
                    }
                    break;
                }
                case TRAFFIC_DEVICE_B:
                {
                    if((Zigb_Rx_Buf[2] == 0x01) && (Zigb_Rx_Buf[3] == 0x01))	//判断是不是进入识别模式的回传
                    {
                        LandMark_Flag = Zigb_Rx_Buf[4];	//结果是0x07--成功 0x08--失败
                    }
                    break;
                }
                default:
                    break;
                }
            }
        }

#endif
    }
    Yang_ArrInit(SmartTrafficLights_TX_Arr);	//还原数组
    delay_ms(100);
}
/*
附录1：
帧头第一位--0x55,帧头第二位设备A是0x0E；设备B是0x0F，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1]  | 副指令[2]  | 副指令[3]  |			说明
|--------|-----------|-----------|-----------|-----------------------------------
|        |	0x01     |	     	 |      	 |“红色”识别结果请求确认
|        |-----------|           |           |-----------------------------------
|  0x02  |	0x02     |   0x00    |   0x00    |“绿色”识别结果请求确认
|        |-----------|           |           |-----------------------------------
|        |	0x03     |           |       	 |“黄色”识别结果请求确认
--------------------------------------------------------------------------------
注意---识别成功则停留在那个灯上，识别错误则全部灯亮
*/
/*
功能：识别结果请求确认
参数1---设备A/B 参数2---请求的颜色ID(1~3)
*/
void Yang_Traffic_Color(uint8_t device, uint8_t ID)
{
    uint8_t checksum;

    if('A' == device)	//设备A
    {
        SmartTrafficLights_TX_Arr[1] = TRAFFIC_DEVICE_A;	//帧头第2位(设备A)
    }
    else if('B' == device)	//设备B
    {
        SmartTrafficLights_TX_Arr[1] = TRAFFIC_DEVICE_B;	//帧头第2位(设备B)
    }
    SmartTrafficLights_TX_Arr[2] = 0x02;	//主指令
    switch(ID)
    {
    case 1:
        SmartTrafficLights_TX_Arr[3] = 0x01;	//副指令1【红色】
        break;
    case 2:
        SmartTrafficLights_TX_Arr[3] = 0x02;	//副指令1【绿色】
        break;
    case 3:
        SmartTrafficLights_TX_Arr[3] = 0x03;	//副指令1【黄色】
        break;
    default:
        break;
    }
    checksum = Yang_CheckSum_1(SmartTrafficLights_TX_Arr[2], SmartTrafficLights_TX_Arr[3], SmartTrafficLights_TX_Arr[4], SmartTrafficLights_TX_Arr[5]);	//计算校验和
    SmartTrafficLights_TX_Arr[6] = checksum;
    Send_ZigbeeData_To_Fifo(SmartTrafficLights_TX_Arr, 8);	//发送数据
    Yang_ArrInit(SmartTrafficLights_TX_Arr);	//还原数组
    delay_ms(100);
}


/***************************************************************************
									特殊地形标志物控制区
****************************************************************************/

/*
附录1：
帧头第一位--0x55,帧头第二位是0x10，帧尾--0xBB
--------------------------------------------------------------------------------
| 主指令 | 副指令[1]  | 副指令[2]  | 副指令[3]  |			说明
|--------|-----------|-----------|-----------|-----------------------------------
|        |	         |	  0x00   |      	 |查询车辆通行状态
|        |           |-----------|           |-----------------------------------
|        |           |    0x31   |           |车辆顺利通过，通行方向(A->B)(回传命令)
| 0x10   |  0x01     |-----------|  0x00     |-----------------------------------
|        |           |    0x32   |           |车辆顺利通过，通行方向(B->A)(回传命令)
|        |           |-----------|           |-----------------------------------
|        |           |    0x33   |       	 |车辆未顺利通过(回传命令)
--------------------------------------------------------------------------------
注意：特殊地形标志物不主动回传通行状态，需通过指令请求回传
*/
/*
功能：特殊地形标志物查询车辆通行状态
*/
void Yang_SpecialTerrain_check(void)
{
    uint8_t checksum;
#if 1	//测试再开启
    uint8_t arr[10];
#endif
#if 1
    if(LandMark_TimeA >= 3)
    {
        LandMark_TimeA = 0;
        return;	//强制退出函数
    }
#endif
    LandMark_Flag = 0;	//标志位清0
    checksum = Yang_CheckSum_1(SpecialTerrain_TX_Arr[2], SpecialTerrain_TX_Arr[3], SpecialTerrain_TX_Arr[4], SpecialTerrain_TX_Arr[5]);	//计算校验和
    SpecialTerrain_TX_Arr[6] = checksum;	//校验和
    Send_ZigbeeData_To_Fifo(SpecialTerrain_TX_Arr, 8);	//发送数据
    delay_ms(90);
    if(Zigbee_Rx_flag == 1)	 //zigbee返回信息
    {
        Zigbee_Rx_flag = 0;	//接收标志位清0
        if((Zigb_Rx_Buf[0] == 0x55) && (Zigb_Rx_Buf[1] == 0x10))	//判断是不是特殊地形帧头
        {
            if((Zigb_Rx_Buf[2] == 0x10) && (Zigb_Rx_Buf[3] == 0x01) && (Zigb_Rx_Buf[4] != 0x00))	//判断是不是回传命令类型
            {
                switch(Zigb_Rx_Buf[4])
                {
                case 0x31:	//A->B
                {
                    LandMark_Flag = 0x31;
                    break;
                }
                case 0x32:	//B->A
                {
                    LandMark_Flag = 0x32;
                    break;
                }
                case 0x33:	//未顺利通过
                {
                    LandMark_Flag = 0x33;
                    break;
                }
                default:
                    break;
                }
#if 1	//测试再开启				
                sprintf((char *)arr, "LM-%#x\n", LandMark_Flag);
                Send_InfoData_To_Fifo((char *)arr, 9);
#endif
            }

        }
    }
    else	//回调
    {
        Yang_SpecialTerrain_check();
        LandMark_TimeA++;
    }
    delay_ms(100);
}

//穿过特殊地形硬闯
//1白线---灯亮 0黑线---灯灭
//参数--速度
//标志位Yang_Track_Flag
void Yang_SpecialTerrain_1(uint8_t speed)
{
    Yang_Car_Track_White(20);	//寻迹到白线停
    delay_ms(700);	//延时

    Control(speed, speed);  // 电机驱动函数
    delay_ms(700);	//调试即可
    delay_ms(100);
    Car_Track(Yang_Go_Speed);	//寻迹到黑色
    delay_ms(Yang_Delay_big);	//延时
}


/***************************************************************************
									智能路灯标志物控制区
****************************************************************************/

/*
附录1：
--------------------------------------------------------------------------------
| 帧头第1位 | 帧头第2位 | 数据  | 数据反码 |			说明
|----------|-----------|------|----------|-----------------------------------
|          |	       | 0x0c |   0xF3   |光源挡位加1档(红外)
|          |           |------|----------|-----------------------------------
|  0x00    |    0xFF   | 0x18 |   0xE7   |光源挡位加2档(红外)
|          |           |------|----------|-----------------------------------
|          |	       | 0x5E |   0xA1   |光源挡位加3档(红外)
--------------------------------------------------------------------------------
*/
/*
功能：调节路灯的光源档位
参数---需要加多少档（1~3）
*/
void StreetLight_adjust(uint8_t ID)
{
    switch(ID)
    {
    case 1:	//光源档位+1
    {
        StreetLight_TX_Arr[2] = 0x0C;	//数据
        StreetLight_TX_Arr[3] = 0xF3;	//数据反码
        break;
    }
    case 2:	//光源档位+2
    {
        StreetLight_TX_Arr[2] = 0x18;	//数据
        StreetLight_TX_Arr[3] = 0xE7;	//数据反码
        break;
    }
    case 3:	//光源档位+3
    {
        StreetLight_TX_Arr[2] = 0x5E;	//数据
        StreetLight_TX_Arr[3] = 0xA1;	//数据反码
        break;
    }
    default:
        break;
    }
    Infrared_Send(StreetLight_TX_Arr, 4);	//红外发送数据
    delay_ms(100);
}

/*
功能：根据光度判断当前档位再根据需要调到规定档位
参数---需要到达的档位(1~4)
*/
void StreetLight_Test(uint8_t num)
{
    uint16_t temp;	//临时变量保存初始状态的光度
    uint8_t i;
    uint8_t Old_Gear, New_Gear = 0;	//初始档位,最后档位

#if 0	//测试再打开
    uint8_t arr[40], j;
#endif

    Light_temp[0] = Get_Bh_Value();	//读取第一次光度值
    temp = Light_temp[0];
    StreetLight_adjust(1);	//+1档
    delay_ms(500);	//至少延时1s太短会接收不到
    delay_ms(500);
    Light_temp[1] = Get_Bh_Value();	//读取第二次光度值
    StreetLight_adjust(1);	//+1档
    delay_ms(500);
    delay_ms(500);
    Light_temp[2] = Get_Bh_Value();	//读取第三次光度值
    StreetLight_adjust(1);	//+1档
    delay_ms(500);
    delay_ms(500);
    Light_temp[3] = Get_Bh_Value();	//读取第四次光度值
    StreetLight_adjust(1);	//+1档回到最初位置

#if 0	//测试再打开	
    sprintf((char *)arr, "1--%d 2--%d 3--%d 4--%d\n", Light_temp[0], Light_temp[1], Light_temp[2], Light_temp[3]);
    Send_InfoData_To_Fifo((char *)arr, 34);
#endif

    Yang_bubble_sort(Light_temp, 4);	//冒泡排序从小到大
    for(i = 0; i < 4; i++)	//确认档位
    {
        if(Light_temp[i] == temp)	//如果相等
        {
            Old_Gear = i + 1;	//最小档位是1最大是4

#if 0	//测试再打开	
            sprintf((char *)arr, "1--%d 2--%d 3--%d 4--%d D--%d \n", Light_temp[0], Light_temp[1], Light_temp[2], Light_temp[3], Old_Gear);
            Send_InfoData_To_Fifo((char *)arr, 38);
#endif

        }
    }
    delay_ms(500);
    delay_ms(500);
    //设置到规定的档位
    switch(num)
    {
    case 1:	//档位调到1档处
    {
        if(1 == Old_Gear)
        {
            ;	//无需处理
        }
        else if(2 == Old_Gear)
        {
            StreetLight_adjust(3);	//+3档
            New_Gear = 2;
        }
        else if(3 == Old_Gear)
        {
            StreetLight_adjust(2);	//+2档
        }
        else if(4 == Old_Gear)
        {
            StreetLight_adjust(1);	//+1档
        }
        New_Gear = 1;
        break;
    }
    case 2:	//档位调到2档处
    {
        if(1 == Old_Gear)
        {
            StreetLight_adjust(1);	//+1档
        }
        else if(2 == Old_Gear)
        {
            ;	//无需处理
        }
        else if(3 == Old_Gear)
        {
            StreetLight_adjust(3);	//+3档
        }
        else if(4 == Old_Gear)
        {
            StreetLight_adjust(2);	//+2档
        }
        New_Gear = 2;
        break;
    }
    case 3:	//档位调到3档处
    {
        if(1 == Old_Gear)
        {
            StreetLight_adjust(2);	//+2档
        }
        else if(2 == Old_Gear)
        {
            StreetLight_adjust(1);	//+1档
        }
        else if(3 == Old_Gear)
        {
            ;	//无需处理
        }
        else if(4 == Old_Gear)
        {
            StreetLight_adjust(3);	//+3档
        }
        New_Gear = 3;
        break;
    }
    case 4:	//档位调到4档处
    {
        if(1 == Old_Gear)
        {
            StreetLight_adjust(3);	//+3档
        }
        else if(2 == Old_Gear)
        {
            StreetLight_adjust(2);	//+2档
        }
        else if(3 == Old_Gear)
        {
            StreetLight_adjust(1);	//+1档
        }
        else if(4 == Old_Gear)
        {
            ;	//无需处理
        }
        New_Gear = 4;
        break;
    }
    default:
        break;
    }

#if 0	//测试再打开	
    sprintf((char *)arr, "New_Gear = %d\n", New_Gear);
    Send_InfoData_To_Fifo((char *)arr, 14);
#endif
}


/***************************************************************************
									立体显示标志物控制区
****************************************************************************/

/*
附录1：
帧头---0xFF
-----------------------------------------------------------------------
| 主指令    | 数据1  | 数据2| 数据3 | 数据4 |			说明
|----------|--------|------|------|-------|----------------------------
|   0x20   |  0xXX	| 0xXX | 0xXX | 0xXX  |前4位车牌信息显示模式（ASCII）
|----------|--------|------|------|-------|----------------------------
|   0x10   |  0xXX  | 0xXX |横坐标 |纵坐标 |后2位车牌及坐标(x,y)信息显示模式（ASCII）
-----------------------------------------------------------------------
*/
/*
功能：立体显示标志物显示车牌信息及坐标
参数1---车牌字符串 参数2---横坐标x 参数3---纵坐标y(比如 车牌:"A12345" x坐标:'C' y坐标:'1')
*/
void Yang_ThreeD_CarCardAndxy(char *str, char x, char y)
{
    ThreeD_TX_Arr[1] = 0x20;	//主指令
    ThreeD_TX_Arr[2] = *(str + 0);	//【车牌数据1】
    ThreeD_TX_Arr[3] = *(str + 1);	//【车牌数据2】
    ThreeD_TX_Arr[4] = *(str + 2);	//【车牌数据3】
    ThreeD_TX_Arr[5] = *(str + 3);	//【车牌数据4】
    Infrared_Send(ThreeD_TX_Arr, 6);	//红外发送
    delay_ms(500);	//延时
    ThreeD_TX_Arr[1] = 0x10;	//主指令
    ThreeD_TX_Arr[2] = *(str + 4);	//【车牌数据1】
    ThreeD_TX_Arr[3] = *(str + 5);	//【车牌数据2】
    ThreeD_TX_Arr[4] = x;	//【横坐标x】
    ThreeD_TX_Arr[5] = y;	//【纵坐标y】
    Infrared_Send(ThreeD_TX_Arr, 6);	//红外发送

    Yang_ArrInit(ThreeD_TX_Arr);	//还原数组
    delay_ms(100);

}
/*
附录1：
帧头---0xFF
-----------------------------------------------------------------------
| 主指令    | 数据1  | 数据2| 数据3 | 数据4 |			说明
|----------|--------|------|------|-------|----------------------------
|   0x11   |  十位	| 个位 | 0x00 | 0x00  |显示距离
-----------------------------------------------------------------------
*/
/*
功能：立体显示标志物显示距离模式
参数---需要显示的数据十进制(比如123)
*/
void Yang_ThreeD_Removing(uint16_t pdis)
{
    uint16_t Ultrasound_pv;	//距离显示的缓存变量

    Ultrasound_pv = pdis;
#if 0
    Ultrasonic_Ranging();   //获取超声波数据
    Ultrasound_pv = dis;  //记录超声波数据
#endif
    ThreeD_TX_Arr[1] = 0x011;	//主指令(显示距离模式)
    ThreeD_TX_Arr[2] = 0x30 + (uint8_t)(Ultrasound_pv / 100 % 10);	//距离十位--cm
    ThreeD_TX_Arr[3] = 0x30 + (uint8_t)(Ultrasound_pv / 10 % 10);	//距离个位--cm
    Infrared_Send(ThreeD_TX_Arr, 6);	//发送
#if 1	//以防万一再发一次
	delay_ms(100);
	Infrared_Send(ThreeD_TX_Arr, 6);	//发送
#endif	
    Yang_ArrInit(ThreeD_TX_Arr);	//还原数组
    delay_ms(100);
}
/*
附录1：
帧头---0xFF
-----------------------------------------------------------------------
| 主指令    | 数据1  | 数据2| 数据3 | 数据4 |			说明
|----------|--------|------|------|-------|----------------------------
|          |  0x01	|      |      |       |矩形（以下都是显示的是字不是图案）
|          |--------|      |      |       |----------------------------
|          |  0x02  |      |      |       |圆形
|          |--------|      |      |       |----------------------------
|   0x12   |  0x03  | 0x00 | 0x00 | 0x00  |三角形
|          |--------|      |      |       |----------------------------
|          |  0x04  |      |      |       |菱形
|          |--------|      |      |       |----------------------------
|          |  0x05  |      |      |       |五角星
------------------------------------------------------------------------
*/
/*
功能：立体显示标志物图形信息显示模式
参数---需要显示的图案ID(1~5)
*/
void Yang_ThreeD_GraphShow(uint8_t ID)
{
    ThreeD_TX_Arr[1] = 0x12;	//主命令
    switch(ID)
    {
    case 1:
    {
        ThreeD_TX_Arr[2] = 0x01;	//数据1【矩形】
        break;
    }
    case 2:
    {
        ThreeD_TX_Arr[2] = 0x02;	//数据1【圆形】
        break;
    }
    case 3:
    {
        ThreeD_TX_Arr[2] = 0x03;	//数据1【三角形】
        break;
    }
    case 4:
    {
        ThreeD_TX_Arr[2] = 0x04;	//数据1【菱形】
        break;
    }
    case 5:
    {
        ThreeD_TX_Arr[2] = 0x05;	//数据1【五角星】
        break;
    }
    }
    Infrared_Send(ThreeD_TX_Arr, 6);	//发送
    Yang_ArrInit(ThreeD_TX_Arr);	//还原数组
    delay_ms(100);
}
/*
附录1：
帧头---0xFF
-----------------------------------------------------------------------
| 主指令    | 数据1  | 数据2| 数据3 | 数据4 |			说明
|----------|--------|------|------|-------|----------------------------
|          |  0x01	|      |      |       |红色(255,0,0)（以下都是显示的是字）
|          |--------|      |      |       |----------------------------
|          |  0x02  |      |      |       |绿色(0,255,0)
|          |--------|      |      |       |----------------------------
|          |  0x03  |      |      |       |蓝色(0,0,255)
|          |--------|      |      |       |----------------------------
|          |  0x04  |      |      |       |黄色(255,255,0)
|   0x13   |--------| 0x00 | 0x00 | 0x00  |----------------------------
|          |  0x05  |      |      |       |品色(255,0,255)
|          |--------|      |      |       |----------------------------
|          |  0x06  |      |      |       |青色(0,255,255)
|          |--------|      |      |       |----------------------------
|          |  0x07  |      |      |       |黑色(0,0,0)
|          |--------|      |      |       |----------------------------
|          |  0x08  |      |      |       |白色(255,255,255)
------------------------------------------------------------------------
*/
/*
功能：立体显示标志物颜色信息显示模式
参数---需要显示的颜色ID(1~8)
*/
void Yang_ThreeD_Color(uint8_t ID)
{
    ThreeD_TX_Arr[1] = 0x13;	//主指令
    switch(ID)
    {
    case 1:
    {
        ThreeD_TX_Arr[2] = 0x01;	//红色
        break;
    }
    case 2:
    {
        ThreeD_TX_Arr[2] = 0x02;	//绿色
        break;
    }
    case 3:
    {
        ThreeD_TX_Arr[2] = 0x03;	//蓝色
        break;
    }
    case 4:
    {
        ThreeD_TX_Arr[2] = 0x04;	//黄色
        break;
    }
    case 5:
    {
        ThreeD_TX_Arr[2] = 0x05;	//品色
        break;
    }
    case 6:
    {
        ThreeD_TX_Arr[2] = 0x06;	//青色
        break;
    }
    case 7:
    {
        ThreeD_TX_Arr[2] = 0x07;	//黑色
        break;
    }
    case 8:
    {
        ThreeD_TX_Arr[2] = 0x08;	//白色
        break;
    }
    }
    Infrared_Send(ThreeD_TX_Arr, 6);	//发送命令
    Yang_ArrInit(ThreeD_TX_Arr);
    delay_ms(100);
}
/*
附录1：
帧头---0xFF
-----------------------------------------------------------------------
| 主指令    | 数据1  | 数据2| 数据3 | 数据4 |			说明
|----------|--------|------|------|-------|----------------------------
|          |  0x01	|      |      |       |前方学校 减速慢行
|          |--------|      |      |       |----------------------------
|          |  0x02  |      |      |       |前方施工 禁止通行
|          |--------|      |      |       |----------------------------
|          |  0x03  |      |      |       |塌方路段 注意安全
|          |--------|      |      |       |----------------------------
|          |  0x04  |      |      |       |追尾危险 保持车距
|   0x14   |--------| 0x00 | 0x00 | 0x00  |----------------------------
|          |  0x05  |      |      |       |严禁 酒后驾车！
|          |--------|      |      |       |----------------------------
|          |  0x06  |      |      |       |严禁 乱扔垃圾！
------------------------------------------------------------------------
*/
/*
功能：立体显示标志物交通警示牌信息显示模式
参数---需要显示的交通警示牌信息ID(1~6)
*/
void Yang_ThreeD_Warning(uint8_t ID)
{
    ThreeD_TX_Arr[1] = 0x14;	//主指令
    switch(ID)
    {
    case 1:
    {
        ThreeD_TX_Arr[2] = 0x01;	//前方学校 减速慢行
        break;
    }
    case 2:
    {
        ThreeD_TX_Arr[2] = 0x02;	//前方施工 禁止通行
        break;
    }
    case 3:
    {
        ThreeD_TX_Arr[2] = 0x03;	//塌方路段 注意安全
        break;
    }
    case 4:
    {
        ThreeD_TX_Arr[2] = 0x04;	//追尾危险 保持车距
        break;
    }
    case 5:
    {
        ThreeD_TX_Arr[2] = 0x05;	//严禁 酒后驾车！
        break;
    }
    case 6:
    {
        ThreeD_TX_Arr[2] = 0x06;	//严禁 乱扔垃圾！
        break;
    }
    }
    Infrared_Send(ThreeD_TX_Arr, 6);	//发送命令
    Yang_ArrInit(ThreeD_TX_Arr);
    delay_ms(100);
}
/*
附录1：
帧头---0xFF
-----------------------------------------------------------------------
| 主指令    | 数据1  | 数据2| 数据3 | 数据4 |			说明
|----------|--------|------|------|-------|----------------------------
|          |  0x01	|      |      |       |直行
|          |--------|      |      |       |----------------------------
|          |  0x02  |      |      |       |左转
|          |--------|      |      |       |----------------------------
|          |  0x03  |      |      |       |右转
|          |--------|      |      |       |----------------------------
|          |  0x04  |      |      |       |掉头
|   0x15   |--------| 0x00 | 0x00 | 0x00  |----------------------------
|          |  0x05  |      |      |       |禁止直行
|          |--------|      |      |       |----------------------------
|          |  0x06  |      |      |       |禁止通行
------------------------------------------------------------------------
*/
/*
功能：立体显示标志物交通标志信息显示模式
参数---需要显示的交通标志信息ID(1~6)
*/
void Yang_ThreeD_Traffic(uint8_t ID)
{
    ThreeD_TX_Arr[1] = 0x15;	//主指令
    switch(ID)
    {
    case 1:
    {
        ThreeD_TX_Arr[2] = 0x01;	//直行
        break;
    }
    case 2:
    {
        ThreeD_TX_Arr[2] = 0x02;	//左转
        break;
    }
    case 3:
    {
        ThreeD_TX_Arr[2] = 0x03;	//右转
        break;
    }
    case 4:
    {
        ThreeD_TX_Arr[2] = 0x04;	//掉头
        break;
    }
    case 5:
    {
        ThreeD_TX_Arr[2] = 0x05;	//禁止直行
        break;
    }
    case 6:
    {
        ThreeD_TX_Arr[2] = 0x06;	//禁止通行
        break;
    }
    }
    Infrared_Send(ThreeD_TX_Arr, 6);	//发送命令
    Yang_ArrInit(ThreeD_TX_Arr);
    delay_ms(100);
}
/*
附录1：
帧头---0xFF
-----------------------------------------------------------------------
| 主指令    | 数据1  | 数据2| 数据3 | 数据4 |			说明
|----------|--------|------|------|-------|----------------------------
|   0x16   |  0x01	| 0x00 | 0x00 |  0x00 |默认显示信息“百科荣创（北京）科技发展有限公司“
------------------------------------------------------------------------
*/
/*
功能：立体显示标志物显示默认信息
*/
void Yang_ThreeD_Default(void)
{
    ThreeD_TX_Arr[1] = 0x16;	//主指令
    ThreeD_TX_Arr[2] = 0x01;	//数据1
    Infrared_Send(ThreeD_TX_Arr, 6);	//发送命令
    Yang_ArrInit(ThreeD_TX_Arr);	//还原数组
    delay_ms(100);
}
/*
附录1：
帧头---0xFF
-----------------------------------------------------------------------
| 主指令    | 数据1  |  数据2  |  数据3  |   数据4   |			说明
|----------|--------|---------|---------|----------|-------------------
|   0x17   |  0x01	| 0xXX(R) | 0xXX(G) |  0xXX(B) |设置文字显示颜色
-----------------------------------------------------------------------
*/
/*
功能：立体显示标志物设置文字显示颜色(十六进制格式 比如红色-->0xFF,0x00,0x00)
参数1---R 参数2---G 参数3---B
注意：是RGB格式
*/
void Yang_ThreeD_TextColor(uint8_t R, uint8_t G, uint8_t B)
{
    ThreeD_TX_Arr[1] = 0x17;	//主指令
    ThreeD_TX_Arr[2] = 0x01;	//数据1
    ThreeD_TX_Arr[3] = R;	//数据2【R】
    ThreeD_TX_Arr[4] = G;	//数据3【G】
    ThreeD_TX_Arr[5] = B;	//数据4【B】
    Infrared_Send(ThreeD_TX_Arr, 6);	//发送命令
    Yang_ArrInit(ThreeD_TX_Arr);	//还原数组
    delay_ms(100);
}
/*
附录1：
帧头---0xFF
-----------------------------------------------------------------------
| 主指令    | 数据1  | 数据2| 数据3 | 数据4 |			说明
|----------|--------|------|------|-------|----------------------------
|          | 0x01   |      |      |       |结束累加不清除现有显示内容
|   0x32   |------  | 0x00 | 0x00 | 0x00  |----------------------------
|          | 0x02   |      |      |       |结束累加并清除现有显示内容
------------------------------------------------------------------------
*/
/*
功能：立体显示标志物其他功能
参数---SET表示结束累加不清除现有显示内容 RESET表示结束累加并清除现有显示内容
*/
void Yang_ThreeD_Other(uint8_t swch)
{
    ThreeD_TX_Arr[1] = 0x32;	//主指令
    if(SET == swch)
    {
        ThreeD_TX_Arr[2] = 0x01;	//数据1【结束累加不清除现有显示内容】
    }
    else if(RESET == swch)
    {
        ThreeD_TX_Arr[2] = 0x02;	//数据1【结束累加并清除现有显示内容】
    }
    Infrared_Send(ThreeD_TX_Arr, 6);	//发送命令
    Yang_ArrInit(ThreeD_TX_Arr);	//还原数组
    delay_ms(100);
}
/*
附录1：
帧头---0xFF
------------------------------------------------------------------------------------------
| 主指令    | 数据1  |       数据2   |      数据3     |       数据4        | 说明
|----------|--------|---------------|----------------|-------------------|----------------
|   0x31   |  0x01	| 0xXX(文本信息) | 0xXX(文本信息) |  0x55(文字结束)    |自定义文本累加显示
------------------------------------------------------------------------------------------
*/
/*
功能：立体显示标志物自定义文本累加
*/
void Yang_ThreeD_CustomizeText(void)
{
    ThreeD_TX_Arr[1] = 0x31;	//主指令
    ThreeD_TX_Arr[2] = 0x01;	//数据1
    ThreeD_TX_Arr[3] = 0xBD;	//数据2【文本信息】
    ThreeD_TX_Arr[4] = 0x22;	//数据3【文本信息】
    ThreeD_TX_Arr[5] = 0x55;	//数据4【文字结束】
    Infrared_Send(ThreeD_TX_Arr, 6);	//发送命令
    Yang_ArrInit(ThreeD_TX_Arr);	//还原数组
    delay_ms(100);
}

/***************************************************************************
									静态标志物控制区(二维码)
****************************************************************************/


/***************************************************************************
									RFID相关
****************************************************************************/

void Yang_Track_duoy_RFID(uint8_t speed)   //寻卡寻迹
{
    Stop_Flag = 0;          // 运行状态标志位
    Go_Flag = 0;            // 前进标志位
    wheel_L_Flag = 0;       // 左转标志位
    wheel_R_Flag = 0;       // 右转标志位
    wheel_Nav_Flag = 0;     // 码盘旋转标志位
    Back_Flag = 0;          // 后退标志位
    Track_Flag = 1;         // 循迹标志位
    Car_Spend = speed;      // 速度值
    Control(Car_Spend, Car_Spend);  // 电机驱动函数
}

//块地址（范围：0~63）
void Yang_Track_duo_RFID(uint8_t speed, uint8_t card) //读卡函数   参数：循迹速度，需要读取的数据块
{
    Yang_Track_duoy_RFID(speed);
    delay_ms(300);
    while(Stop_Flag != 0x01)
    {
        if(PcdRequest(PICC_REQALL, CT) == MI_OK)  //是寻到RFID
        {
            Send_UpMotor(0 , 0);
            wheel_L_Flag = 0;
            wheel_R_Flag = 0;
            Go_Flag = 0;
            Back_Flag = 0;
            Track_Flag = 0;
            Stop_Flag = 0;
            delay_ms(500);
            for(char i = 0; i < 10; i++)
            {
                MP_SPK = 1;
                delay_ms(80);
                MP_SPK = 0;
                delay_ms(10);
            }
            delay_ms(500);
            RC522(card, RFID_Read); //读卡
            delay_ms(400);
            delay_ms(400);
#if 0	//测试再打开		 
            Stop_Flag = 0x01;
#endif
            Yang_Track_duoy_RFID(speed);
        }
    }
}

//块地址（范围：0~63）一共16个扇区，一共扇区4块 16*4=64
void Yang_Track_xie_RFID(uint8_t speed, uint8_t card) //写卡函数   参数：循迹速度，需要写取的数据块
{
    Yang_Track_duoy_RFID(speed);
    delay_ms(300);
    while(Stop_Flag != 0x01)
    {
        if(PcdRequest(PICC_REQALL, CT) == MI_OK)  //是寻到RFID
        {
            Send_UpMotor(0 , 0);
            wheel_L_Flag = 0;
            wheel_R_Flag = 0;
            Go_Flag = 0;
            Back_Flag = 0;
            Track_Flag = 0;
            Stop_Flag = 0;
            delay_ms(500);
            for(char i = 0; i < 10; i++)
            {
                MP_SPK = 1;
                delay_ms(80);
                MP_SPK = 0;
                delay_ms(10);
            }
            delay_ms(500);
            RC522(card, RFID_Write); //写卡
            delay_ms(400);
            delay_ms(400);
            Yang_Track_duoy_RFID(speed);
        }
    }
}






/***************************************************************************
									终端与主车通信区
****************************************************************************/
/* 定义终端发送给主车的格式
帧头第一位--0x55 帧头第二位--0xAA 主指令--0xA1~0xA9 0xB1~0xB9 副指令1--0xXX 副指令2--0xXX 副指令3--0xXX 校验和 帧尾--0xBB
*/
//终端发给主车的命令大全
/*
附录1：
帧头第一位--0x55,帧头第二位--0xAA(主车)，帧尾--0xBB
//交通灯标志物识别
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|-----------------------------------
|        |	 0x0E	 |   0x01    | 0x00      |交通灯识别结果为红色
|		 |   (A)	 |-----------|-----------|-----------------------------------
| 0xA1   |---------- |	 0x02	 | 0x00      |交通灯识别结果为绿色
|        |	0x0F     |-----------|-----------|-----------------------------------
|        |  (B)      |	 0x03    | 0x00      |交通灯识别结果为黄色
--------------------------------------------------------------------------------

//TFT标志物车牌识别(有A/B设备但是协议不够长不判断了)
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA2   |	 0xXX	 |   0xXX    | 0xXX      |识别到车牌并返回车牌前3位数据(返回类型为字符类型'')
|--------|---------- |-----------|-----------|-----------------------------------
| 0xA3   |	 0xXX	 |   0xXX    | 0xXX      |识别到车牌并返回车牌后3位数据(返回类型为字符类型'')
|--------|---------- |-----------|-----------|-----------------------------------
| 0xA4   |   0x00    |	 0x00    | 0x01      |TFT显示的不是车牌
|--------|---------- |-----------|-----------|-----------------------------------
| 0xA5   |   0x00    |	 0x00    | 0x00      |(备用)
|--------|---------- |-----------|-----------|-----------------------------------
| 0xA6   |   0x00    |	 0x00    | 0x00      |(备用)
--------------------------------------------------------------------------------

//二维码标志物识别
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA7   |	 0xXX	 |   0x00    | 0x00      |识别到二维码提取有效的返回(应该是有效数字，返回类型为字符类型'')
|--------|---------- |-----------|-----------|-----------------------------------
| 0xA8   |	 0x00	 |   0x00    | 0x00      |(备用)
|--------|---------- |-----------|-----------|-----------------------------------
| 0xA9   |   0x00    |	 0x00    | 0x00      |(备用)
--------------------------------------------------------------------------------

//备用
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|-----------------------------------
| 0xB1   |	 0x00	 |   0x00    | 0x00      |(备用)
|--------|---------- |-----------|-----------|-----------------------------------
| 0xB2   |	 0x00	 |   0x00    | 0x00      |(备用)
|--------|---------- |-----------|-----------|-----------------------------------
| 0xB3   |   0x00    |	 0x00    | 0x00      |(备用)
|--------|---------- |-----------|-----------|-----------------------------------
| 0xB4   |   0x00    |	 0x00    | 0x00      |(备用)
|--------|---------- |-----------|-----------|-----------------------------------
| 0xB5   |   0x00    |	 0x00    | 0x00      |(备用)
|--------|---------- |-----------|-----------|-----------------------------------
| 0xB6   |   0x00    |	 0x00    | 0x00      |(备用)
--------------------------------------------------------------------------------
*/




/***************************************************************************
									从车与主车通信区
****************************************************************************/

/* 定义主车发送给从车的格式
帧头第一位--0x55 帧头第二位--0x02 主指令--0xA0~0xA9 0xB0~0xB9 副指令1--0xXX 副指令2--0xXX 副指令3--0xXX 校验和 帧尾--0xBB
*/
/* 主车接收从车的格式(即从车发送给主车的格式)
帧头第一位--0x55 帧头第二位--0x01 主指令--0xA0~0xA9 0xB0~0xB9 副指令1--0xXX 副指令2--0xXX 副指令3--0xXX 校验和 帧尾--0xBB
*/
/*
附录1：
帧头第一位--0x55,帧头第二位--0x02(从车)，帧尾--0xBB
//交通灯标志物识别
--------------------------------------------------------------------------------
| 主指令 | 副指令[1] | 副指令[2] | 副指令[3] |			说明
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA0   |	0x00     |   0x00    | 0x00      |测试从车转向灯亮
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA1   |	0xXX     |   0xXX    | 0xXX      |备用
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA2   |	0xXX     |   0xXX    | 0xXX      |备用
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA3   |	0xXX     |   0xXX    | 0xXX      |备用
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA4   |	0xXX     |   0xXX    | 0xXX      |备用
|--------|-----------|-----------|-----------|-----------------------------------
| 0xA5   |	0xXX     |   0xXX    | 0xXX      |备用
|--------|-----------|-----------|-----------|-----------------------------------
--------------------------------------------------------------------------------
*/
//测试
void Yang_AGV_LED(void)
{
    uint8_t checksum;
    AGV_TX_Arr[2] = 0xA0;	//主指令
    checksum = Yang_CheckSum_1(AGV_TX_Arr[2], AGV_TX_Arr[3], AGV_TX_Arr[4], AGV_TX_Arr[5]);	//计算校验和
    AGV_TX_Arr[6] = checksum;	//校验和
    Send_ZigbeeData_To_Fifo(AGV_TX_Arr, 8);
    Yang_ArrInit(AGV_TX_Arr);
    delay_ms(100);
}



/***************************************************************************
									通用函数区
****************************************************************************/

/*
功能：计算校验和(适用于含---"主指令""副指令1""副指令2""副指令3" 8Byte通信协议)
参数1---主指令十六进制 参数2---副指令1十六进制 参数3---副指令2十六进制 参数3---副指令3十六进制
返回值：uint8_t类型
*/
uint8_t Yang_CheckSum_1(uint8_t main_command, uint8_t sub_command1, uint8_t sub_command2, uint8_t sub_command3)
{
    uint8_t Sum;
    Sum = (main_command + sub_command1 + sub_command2 + sub_command3) % 256;	//主+3个副指令求和再对256求余数
    return Sum;	//返回计算结果
}
/*
功能：还原数组
参数---哪个数组
*/
void Yang_ArrInit(uint8_t *arr)
{
    if(arr == Voice_TX_Arr)
    {
        //语音播报标志物发送控制命令存储数组
        Voice_TX_Arr[0] = 0x55;
        Voice_TX_Arr[1] = 0x06;
        Voice_TX_Arr[2] = 0x00;
        Voice_TX_Arr[3] = 0x00;
        Voice_TX_Arr[4] = 0x00;
        Voice_TX_Arr[5] = 0x00;
        Voice_TX_Arr[6] = 0x00;
        Voice_TX_Arr[7] = 0xBB;
    }
    else if(arr == Voice_RX_DateArr)
    {
        //接收语音播报标志物请求回传的日期缓存数组
        Voice_RX_DateArr[0] = 0;
        Voice_RX_DateArr[1] = 0;
        Voice_RX_DateArr[2] = 0;
    }
    else if(arr == Voice_RX_TimeArr)
    {
        //接收语音播报标志物请求回传的时间缓存数组
        Voice_RX_TimeArr[0] = 0;
        Voice_RX_TimeArr[1] = 0;
        Voice_RX_TimeArr[2] = 0;
    }
    else if(arr == Voice_RX_WeatherTempertureArr)
    {
        //接收语音播报标志物请求回传的天气与温度数据缓存数组
        Voice_RX_WeatherTempertureArr[0] = 0;
        Voice_RX_WeatherTempertureArr[1] = 0;
    }
    else if(arr == Voice_TX_3choose1_Arr)
    {
        //语音播报标志物3选1命令存储数组
        Voice_TX_3choose1_Arr[0] = 0xFD;
        Voice_TX_3choose1_Arr[0] = 0x00;
        Voice_TX_3choose1_Arr[0] = 0x01;
        Voice_TX_3choose1_Arr[0] = 0x00;
    }
    else if(arr == Gate_TX_Arr)
    {
        //闸门标志物发送控制命令存储数组
        Gate_TX_Arr[0] = 0x55;
        Gate_TX_Arr[1] = 0x03;
        Gate_TX_Arr[2] = 0x00;
        Gate_TX_Arr[3] = 0x00;
        Gate_TX_Arr[4] = 0x00;
        Gate_TX_Arr[5] = 0x00;
        Gate_TX_Arr[6] = 0x00;
        Gate_TX_Arr[7] = 0xBB;
    }
    else if(arr == LED_TX_Arr)
    {
        //LED显示标志物发送控制命令存储数组
        LED_TX_Arr[0] = 0x55;
        LED_TX_Arr[1] = 0x04;
        LED_TX_Arr[2] = 0x00;
        LED_TX_Arr[3] = 0x00;
        LED_TX_Arr[4] = 0x00;
        LED_TX_Arr[5] = 0x00;
        LED_TX_Arr[6] = 0x00;
        LED_TX_Arr[7] = 0xBB;
    }
    else if(arr == Beacon_TX_Arr)
    {
        //烽火台报警器标志物发送控制命令存储数组
        Beacon_TX_Arr[0] = 0x55;
        Beacon_TX_Arr[1] = 0x07;
        Beacon_TX_Arr[2] = 0x00;
        Beacon_TX_Arr[3] = 0x00;
        Beacon_TX_Arr[4] = 0x00;
        Beacon_TX_Arr[5] = 0x00;
        Beacon_TX_Arr[6] = 0x00;
        Beacon_TX_Arr[7] = 0xBB;
    }
    else if(arr == TFT_TX_Arr)
    {
        //TFT显示标志物发送控制命令存储数组
        Beacon_TX_Arr[0] = 0x55;
        Beacon_TX_Arr[1] = 0x00;
        Beacon_TX_Arr[2] = 0x00;
        Beacon_TX_Arr[3] = 0x00;
        Beacon_TX_Arr[4] = 0x00;
        Beacon_TX_Arr[5] = 0x00;
        Beacon_TX_Arr[6] = 0x00;
        Beacon_TX_Arr[7] = 0xBB;
    }
    else if(arr == WirelessCharging_TX_Arr)
    {
        //无线充电标志物发送控制命令存储数组
        WirelessCharging_TX_Arr[0] = 0x55;
        WirelessCharging_TX_Arr[1] = 0x0A;
        WirelessCharging_TX_Arr[2] = 0x01;
        WirelessCharging_TX_Arr[3] = 0x00;
        WirelessCharging_TX_Arr[4] = 0x00;
        WirelessCharging_TX_Arr[5] = 0x00;
        WirelessCharging_TX_Arr[6] = 0x00;
        WirelessCharging_TX_Arr[7] = 0xBB;
    }
    else if(arr == ETC_TX_Arr)
    {
        //ETC标志物发送控制命令存储数组
        ETC_TX_Arr[0] = 0x55;
        ETC_TX_Arr[1] = 0x0C;
        ETC_TX_Arr[2] = 0x00;
        ETC_TX_Arr[3] = 0x00;
        ETC_TX_Arr[4] = 0x00;
        ETC_TX_Arr[5] = 0x00;
        ETC_TX_Arr[6] = 0x00;
        ETC_TX_Arr[7] = 0xBB;
    }
    else if(arr == SmartTrafficLights_TX_Arr)
    {
        //智能交通灯标志物发送控制命令存储数组
        SmartTrafficLights_TX_Arr[0] = 0x55;
        SmartTrafficLights_TX_Arr[1] = 0x00;
        SmartTrafficLights_TX_Arr[2] = 0x00;
        SmartTrafficLights_TX_Arr[3] = 0x00;
        SmartTrafficLights_TX_Arr[4] = 0x00;
        SmartTrafficLights_TX_Arr[5] = 0x00;
        SmartTrafficLights_TX_Arr[6] = 0x00;
        SmartTrafficLights_TX_Arr[7] = 0xBB;
    }
    else if(arr == ThreeD_TX_Arr)
    {
        //立体显示标志物发送控制命令存储数组(红外)
        ThreeD_TX_Arr[0] = 0xFF;
        ThreeD_TX_Arr[1] = 0x00;
        ThreeD_TX_Arr[2] = 0x00;
        ThreeD_TX_Arr[3] = 0x00;
        ThreeD_TX_Arr[4] = 0x00;
        ThreeD_TX_Arr[5] = 0x00;
    }
    else if(arr == AGV_TX_Arr)
    {
        //发送给从车的命令存储数组
        AGV_TX_Arr[0] = 0x55;
        AGV_TX_Arr[1] = 0x02;
        AGV_TX_Arr[2] = 0x00;
        AGV_TX_Arr[3] = 0x00;
        AGV_TX_Arr[4] = 0x00;
        AGV_TX_Arr[5] = 0x00;
        AGV_TX_Arr[6] = 0x00;
        AGV_TX_Arr[7] = 0xBB;
    }
}

//冒泡排序
void Yang_bubble_sort(uint16_t *arr, uint8_t len)
{
    uint8_t count = 1; //判断是否已经排序好
    uint16_t i, j;
    //一共要冒n-1次
    for(i = 0; i < len - 1; i++)
    {
        count = 0;
        for(j = 0; j < len - 1 - i; j++)
        {
            if(arr[j] > arr[j + 1])
            {
                uint16_t temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                count = 1;
            }
        }
        if(count == 0) //如果已经是排好则不需要再执行后面直接退出
            break;
    }
}
//数组转字符串（车牌数据用）
//如果数组元素是字符类型则打印用%c  不是则%d
void Yang_ArrToStr(char *arr, char *str, unsigned char size)
{
    //数组转字符串
    int j;
    for(j = 0; j < size; j++)
    {
        str[j] = arr[j];
    }
}

//在一堆数据里连续提取数字(RFID用)
void Yang_DrawNumber(uint8_t *Read_buf)
{
    uint8_t i;
    static uint8_t count = 0;	//因为是有两张卡所以下次继续保存到下一个下标，一张卡则无所谓

    uint8_t len = strlen((const char *)Read_buf);	//计算长度(不包括\0)
    for(i = 0; i < 17; i++)
    {
        if((Read_buf[i] >= '0') && (Read_buf[i] <= '9'))	//提取数字
        {
            RFID_Number_Buf[count] = *(Read_buf + i);
            count++;
        }
    }
    Send_InfoData_To_Fifo((char *)RFID_Number_Buf, count - 1);	//需要减1
    Send_InfoData_To_Fifo("\n", 2);
}

//把某两个数组合成一个0xXX的十六进制(不是十进制直接转成十六进制)
//适用于字符串或者数组第0,1位是字符型数字的情况
uint8_t Yang_NumberTo16(uint8_t *temp)
{
#if 0	//测试再打开	
    uint8_t arr[10];
#endif
    uint8_t Number1;

    Number1 = (temp[0] * 10 + temp[1]) - 16;	//比如9和2组合起来就是92(需要减16)
    Number1 = ((Number1 / 10 % 10) * 16 + Number1 % 10);	//这步就是把92变成0x92
#if 0	//测试再打开	
    sprintf((char *)arr, "%#x", Number1);
    Send_InfoData_To_Fifo((char *)arr, 5);
#endif
    return Number1;
}


/***************************************************************************
									用户TIM相关区
****************************************************************************/
//定时器2 定时2ms
void Timer2_Init(uint16_t arr, uint16_t psc)
{
    TIM_TimeBaseInitTypeDef TIM_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_InitStructure.TIM_Period = arr;
    TIM_InitStructure.TIM_Prescaler = psc;
    TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_InitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 9;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
}
//TIM2中断服务函数
void TIM2_IRQHandler(void)
{
    static uint16_t count = 0;
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        count++;

        if(count >= 10)	//20ms扫描一次
        {
            count = 0;
            Voice_Drive();	//回传命令解析
        }
    }
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}


/***************************************************************************
									行走函数区
****************************************************************************/

/*====================行走控制的全局变量======================*/
uint16_t Yang_Go_Speed = 40;	//寻迹，正常前进/后退行走的速度(默认40)
uint16_t Yang_LR_Speed = 90;	//左右转速度(角度)(默认90)
uint16_t Yang_Removing = 150;	//前进时的距离(码盘值)(暂时不用了已经全改成根据延时来决定多远)(默认150)
uint16_t Yang_wheel_Time = 330; // 全局转45°时间(默认330)
uint16_t Yang_Delay_big = 700;	//大延时(默认700)
uint16_t Yang_Delay_small = 600;	//小延时(默认600)

//以下时间根据实际调整
uint16_t L_SPEED = 90;	//左转速度(默认90)
uint16_t L_TIME = 660;	//左转时间(默认650)
uint16_t R_SPEED = 90;	//右转速度(默认90)
uint16_t R_TIME = 660;	//右转时间(默认650)
uint16_t Go_TIME = 320;	//前进320延时的距离(默认320)

//主车全自动状态位
uint8_t Automatic_Falg = 0x01;
//主车寻迹标志位自定义(寻迹白线用的)
uint8_t Yang_Track_Flag = 0;
//主车倒车入库结束标志位
uint8_t Back_Car_Flag = 0;

//寻迹完前进一丢丢(已延时)
void Yang_Car_TrackAndGo(void)
{
    delay_ms(Yang_Delay_big);
    Car_Track(Yang_Go_Speed);	//寻迹
    delay_ms(Yang_Delay_big);
    Yang_CarThread_Go(Yang_Go_Speed, Go_TIME,SET);	//前进
    delay_ms(Yang_Delay_big);
}
// 主车前进 参数1：速度  参数2：时间(慢慢调)写死了 参数3：最后是否要延时(SET表示要  RESET表示不要)
void Yang_CarThread_Go(uint8_t speed, uint16_t time,uint8_t swch)
{
    Stop_Flag = 0;          // 运行状态标志位
    Go_Flag = 0;            // 前进标志位
    wheel_L_Flag = 0;       // 左转标志位
    wheel_R_Flag = 0;       // 右转标志位
    wheel_Nav_Flag = 0;     // 码盘旋转标志位
    Back_Flag = 0;          // 后退标志位
    Track_Flag = 0;         // 循迹标志位
    Car_Spend = speed;      // 速度值
    Control(Car_Spend, Car_Spend);  // 电机驱动函数
    delay_ms(time);	//延时多久
    Send_UpMotor(0, 0);	//电机停止转动
	if(SET == swch)
	{
		delay_ms(Yang_Delay_big);   //延时		
	}
	else if(RESET == swch)
	{
		delay_ms(1);
	}

}
// 主车左转 参数1：速度 参数2：时间(慢慢调)写死了
void Yang_Left_Time(uint8_t speed, uint16_t time)
{
    delay_ms(100);
    Stop_Flag = 0;          // 运行状态标志位
    Go_Flag = 0;            // 前进标志位
    wheel_L_Flag = 0;       // 左转标志位
    wheel_R_Flag = 0;       // 右转标志位
    wheel_Nav_Flag = 0;     // 码盘旋转标志位
    Back_Flag = 0;          // 后退标志位
    Track_Flag = 0;         // 循迹标志位
    Car_Spend = speed;      // 速度值
    Control(-Car_Spend, Car_Spend); // 电机驱动函数
    delay_ms(time);	//延时多久
    Send_UpMotor(0, 0);	//电机停止转动
    delay_ms(Yang_Delay_big);   //延时
}
// 主车右转 参数1：速度 参数2：时间(慢慢调)
void Yang_Right_Time(uint8_t speed, uint16_t time)
{
    delay_ms(100);
    Stop_Flag = 0;          // 运行状态标志位
    Go_Flag = 0;            // 前进标志位
    wheel_L_Flag = 0;       // 左转标志位
    wheel_R_Flag = 0;       // 右转标志位
    wheel_Nav_Flag = 0;     // 码盘旋转标志位
    Back_Flag = 0;          // 后退标志位
    Track_Flag = 0;         // 循迹标志位
    Car_Spend = speed;      // 速度值
    Control(Car_Spend, -Car_Spend); // 电机驱动函数
    delay_ms(time);	//延时多久
    Send_UpMotor(0, 0);	//电机停止转动
    delay_ms(Yang_Delay_big);   //延时
}

//循迹函数(寻迹到白线停的执行函数)1白线---灯亮 0黑线---灯灭
void Yang_Track_Correct_White(uint8_t gd)
{
    if(gd == 0xFF)	// 循迹灯全亮 停止
    {
        Yang_Track_Flag = 0;	//自定义寻迹白线标志位
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
            Yang_Track_Flag = 0;
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
            Yang_Track_Flag = 0;
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

    if(Yang_Track_Flag != 0)
    {
        Control(LSpeed, RSpeed);
    }
}

//寻迹到白线则停函数（主要判断8个灯状态全亮即可）
void Yang_Car_Track_White(uint8_t speed)
{
#if 0	//自己加
    Roadway_mp_syn();       // 码盘同步

#endif
    Stop_Flag = 0;          // 运行状态标志位
    Go_Flag = 0;            // 前进标志位
    wheel_L_Flag = 0;       // 左转标志位
    wheel_R_Flag = 0;       // 右转标志位
    wheel_Nav_Flag = 0;     // 码盘旋转标志位
    Back_Flag = 0;          // 后退标志位
    Track_Flag = 0;         // 循迹标志位
    Yang_Track_Flag = 1;	//寻迹到白线停标志位
    Car_Spend = speed;      // 速度值
    Control(Car_Spend, Car_Spend);  // 电机驱动函数
    while(Stop_Flag != 0x01);       // 等待循迹完成
}
//左旋转45度 参数1：旋转速度 参数2：旋转时间
void Yang_Car_L45(int8_t speed, uint16_t times)
{
    delay_ms(100);
    Send_UpMotor(-speed , speed);
    delay_ms(times);
    Send_UpMotor(0 , 0);
    delay_ms(Yang_Delay_big);	//延时
}
//右旋转45度 参数1：旋转速度 参数2：旋转时间
void Yang_Car_R45(int8_t speed, uint16_t tims)
{
    delay_ms(100);
    Send_UpMotor(speed, -speed);		// 电机驱动函数
    delay_ms(tims);					// 延时作角度
    Send_UpMotor(0 , 0);				// 停车
    delay_ms(Yang_Delay_big);	//延时
}



//通过获取车库红外状态来决定是否停
//如果车库是正常，有字的在外则判断后侧红外是否触发，如果触发则车停
//如果车库是不正常，有字的在里面则判断前侧红外是否触发，如果触发则车停
/*
Garage_Infrared_Flag == 1 表示后侧触发前侧未触发
Garage_Infrared_Flag == 2 表示前侧触发后侧未触发
Garage_Infrared_Flag == 3 表示都未触发
Garage_Infrared_Flag == 0 表示都触发
*/

//倒车寻迹到红外后侧亮则停函数(也可以当后退函数)
//参数1：速度 参数2：需要延时多久(以实际调试为准)
void Yang_Car_Back_Infrared(uint8_t speed, uint16_t time)
{
    Stop_Flag = 0;          // 运行状态标志位
    Go_Flag = 0;            // 前进标志位
    wheel_L_Flag = 0;       // 左转标志位
    wheel_R_Flag = 0;       // 右转标志位
    wheel_Nav_Flag = 0;     // 码盘旋转标志位
    Back_Flag = 0;          // 后退标志位
    Track_Flag = 0;         // 循迹标志位
    Car_Spend = speed;      // 速度值
    Control(-Car_Spend, -Car_Spend);  // 电机驱动函数
    delay_ms(time);
    Send_UpMotor(0, 0);	//电机停止转动
}


//倒车入库执行函数
void Yang_Car_Garage_return(void)
{
    uint8_t i;
	
	Set_tba_WheelLED(L_LED,SET);	//打开双闪开始倒车
	Set_tba_WheelLED(R_LED,SET);
	
    Yang_Garage_Infrared('A');
    if(Garage_Infrared_Flag != 1)
    {
        Garage_Infrared_Flag = 0;
        Yang_Car_Back_Infrared(Yang_Go_Speed, 700); //倒车
    }
    for(i = 0; i < 5; i++)
    {
        delay_ms(300);
        Yang_Garage_Infrared('A');
        if(Garage_Infrared_Flag != 1)
        {
            Yang_Car_Back_Infrared(Yang_Go_Speed, 200); //倒车
        }
        else
        {
			Yang_CarThread_Go(Yang_Go_Speed,80,SET);	//前进60
#if 1	//可要可不要		
			Set_tba_WheelLED(L_LED,RESET);	//入库完成关闭双闪
			Set_tba_WheelLED(R_LED,RESET);
			Set_tba_Beep(SET);	//打开蜂鸣器
			delay_ms(300);
			Set_tba_Beep(RESET);	//关闭蜂鸣器
#endif			
            break;
        }
    }
}





////ETC识别然后前进函数(适用于ETC放在寻迹线中间)
//void Yang_ETC_Go(void)
//{
//	Yang_
//}


/*
主车全自动函数1(根据任务文档)
*/
void Yang_Automatic_1(void)
{
    static uint16_t JL_temp = 0;	//保存距离
    LandMark_Flag = 0;
    LandMark_TimeA = 0;
    LandMark_TimeB = 0;
    switch(Automatic_Falg)
    {
    case 0x01:	//任务1--主车启动
    {
        Yang_LED_Timing(1);	//开启计时
        Yang_CarThread_Go(Yang_Go_Speed, Go_TIME,RESET);	//前进
        Yang_Car_TrackAndGo();	//寻迹前进
        Yang_Left_Time(L_SPEED, L_TIME);	// 左转
        Automatic_Falg = 0x02;	//下一个任务标志
        break;
    }
    case 0x02:	//任务2--主车交通灯识别
    {
        Yang_Traffic_RecognitionPatterns('A');	//交通灯A进入识别模式
#if 1 //接收APP识别的结果待写
        App_RX_TrafficColor = 1;	//接收返回的识别结果
#endif
        delay_ms(500);	//延时1s
        delay_ms(500);
        Yang_Traffic_Color('A', App_RX_TrafficColor);	//颜色识别结果发送
        Yang_Car_TrackAndGo();	//寻迹前进
        Yang_Car_L45(Yang_LR_Speed, Yang_wheel_Time);	//左转45度
        Automatic_Falg = 0x03;	//下一个任务标志
        break;
    }
    case 0x03:	//任务3--主车识别车牌
    {
#if 0	//向APP发送识别车牌命令，识别失败则翻页继续

#endif
        while(LandMark_Flag != 0xA3)	//直到返回后3位识别结果才退出或者超时也退出
        {
            Yang_TFT_TurnPage('A', 2);	//向下翻页(1则向上翻)
            delay_ms(Yang_Delay_small);	//延时1s
            delay_ms(Yang_Delay_small);
            LandMark_TimeA++;
#if 0	//向APP发送识别车牌命令，识别失败则翻页继续	

#endif
            delay_ms(60);	//延时一下
            //这里在Can_WifiRx_Check函数里进行判断(如果识别到则LandMark_Flag置0xA3且保存车牌数据)
            if(LandMark_TimeA >= 20)	//超时
            {
                break;	//强制退出
            }
        }
        Yang_Car_R45(Yang_LR_Speed, Yang_wheel_Time);	//右转45度
        Automatic_Falg = 0x04;	//下一个任务标志
        break;
    }
    case 0x04:	//任务4--主车测距
    {
        Car_Track(Yang_Go_Speed);	//寻迹
        delay_ms(Yang_Delay_big);	//延时
		Yang_CarThread_Go(Yang_Go_Speed, Go_TIME,SET);	//前进再测因为误差需要在+-20（范围100mm~400mm）
        JL_temp = Yang_LED_Removing(1);	//函数会获取值这里随便传个参即可,结果存起来后面会用到
        Automatic_Falg = 0x05;	//下一个任务标志
        break;
    }
    case 0x05:	//任务5--二维码识别
    {
        while(LandMark_Flag != 0xA7)	//直到返回二维码有效数据或者超时退出
        {
            delay_ms(Yang_Delay_small);
            LandMark_TimeA++;
#if 0	//向APP发送识别二维码命令			

#endif
            delay_ms(60);	//延时一下
            //这里在Can_WifiRx_Check函数里进行判断(如果识别到则LandMark_Flag置0xA3且保存车牌数据)
            if(LandMark_TimeA >= 2)	//超时
            {
                break;	//强制退出
            }
        }
        Yang_Right_Time(R_SPEED, R_TIME);	//右转90度
        Automatic_Falg = 0x06;	//下一个任务标志
        break;
    }
    case 0x06:	//任务6--主车通过道闸
    {
        uint8_t i = 4;
        uint8_t swch;
        while(1)	//循环发送4次
        {
            Yang_Gate_CarCard(App_RX_TFTCarCard);	//发送车牌到闸门
            delay_ms(300);
            swch = Yang_InquireGate_State();	//获取闸门状态
            if(RESET == swch)	//如果返回RESET表示是关闭的
            {
                i--;
            }
            else if(SET == swch)
            {
                delay_ms(300);
                break;	//如果是SET则表示已经打开闸门
            }
            else if(0 == i)	//表示上面都没效则强制打开闸门
            {
                Yang_Gate_OffOrOn(SET);
                delay_ms(300);
                break;
            }
        }
        Yang_Car_TrackAndGo();	//寻迹前进
        Automatic_Falg = 0x07;	//下一个任务标志
        break;
    }
    case 0x07:	//任务7--主车控制立体显示标志物显示
    {
        Yang_Car_R45(Yang_LR_Speed, Yang_wheel_Time);	//右转45度
        Yang_ThreeD_Removing(JL_temp);	//控制立体标志物显示距离
        Yang_Car_R45(Yang_LR_Speed, Yang_wheel_Time);	//右转45度
        Automatic_Falg = 0x08;	//下一个任务标志
        break;
    }
    case 0x08:	//任务8--主车获取RFID数据(写入数据9和2)
    {
        Yang_Track_duo_RFID(40, 6);	//读取第2扇区第2个数据块数据(即4+2=6)
        Yang_CarThread_Go(Yang_Go_Speed, Go_TIME,SET);	//前进
        Yang_Track_duo_RFID(40, 6);	//读取第2扇区第2个数据块数据(即4+2=6)
        Yang_CarThread_Go(Yang_Go_Speed, Go_TIME,SET);	//前进
        Yang_Left_Time(L_SPEED, L_TIME);	//左转90度
        Automatic_Falg = 0x09;	//下一个任务标志
        break;
    }
    case 0x09:	//任务9--主车调光
    {
        uint8_t num;

        num = 1 % 4 + 1;	//这是任务5得到的这里暂时用1(这里结果是2)
        Yang_Car_TrackAndGo();	//寻迹前进
        Yang_Right_Time(R_SPEED, R_TIME);	//右转90度
        StreetLight_Test(num);	//路灯调节到num档
        delay_ms(Yang_Delay_small);	//延时
        Yang_Left_Time(L_SPEED, L_TIME);	//左转90度
        Yang_Car_L45(Yang_LR_Speed, Yang_wheel_Time);	//左转45度
        Automatic_Falg = 0x10;	//下一个任务标志
        break;
    }
    case 0x10:	//任务10--主车开启烽火报警
    {
        uint8_t num2;
        num2 = Yang_NumberTo16(RFID_Number_Buf);	//进行数据换算
        uint8_t Beacon_temp[6] = {0x03, 0x05, 0x14, 0x45, 0xDE, num2};	//发送开启码
        Yang_Beacon_Open2(Beacon_temp);	//发送开启码
        delay_ms(Yang_Delay_small);	//延时
        Yang_Car_L45(Yang_LR_Speed, Yang_wheel_Time);	//左转45度
        Automatic_Falg = 0x11;	//下一个任务标志
        break;
    }
    case 0x11:	//任务11-主车经过特殊地形
    {
        Yang_SpecialTerrain_1(60);	//穿过特殊地形来到十字路口
        Yang_CarThread_Go(Yang_Go_Speed, Go_TIME,SET);	//前进
        Yang_Left_Time(L_SPEED, L_TIME);	//左转90度
        Automatic_Falg = 0x12;	//下一个任务标志
        break;
    }
    case 0x12:	//任务12--主车倒车入库
    {
        if(Yang_InquireGarage_Floor('A') != 1)	//如果层数不是1则降到第1层
        {
            Yang_GarageWent('A', 1);	//降到第1层
        }
        delay_ms(500);
		Yang_Car_Garage_return();	//倒车入库完成
		Yang_GarageWent('A',2);	//到达第2层
		Yang_WirelessCharging_OffOrOn(SET);	//开启无线充电
		delay_ms(300);
		Yang_LED_Timing(0);	//关闭LED计时
		delay_ms(300);
		Yang_LED_Timing(0);	//关闭LED计时
        Automatic_Falg = 0x13;	//下一个任务标志
        break;
    }
    case 0x13:
    {
        AutoFlag = 0;	//全自动标志位置0退出
    }

    }
}





