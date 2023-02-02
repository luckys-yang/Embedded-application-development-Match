#include "allhead.h"
#define __CAN_USER_C__
#include "can_user.h"


uint8_t Wifi_Rx_Buf[ WIFI_MAX_NUM ];	//WiFi接收命令数组(终端发送过来的)(数组大小200)
uint8_t Zigb_Rx_Buf[ ZIGB_RX_MAX ];	//ZigBee接收数组(标志物回传的命令)(数组大小200)
uint8_t Wifi_Rx_num ;//WiFi接收数组的索引(可判断接收了多少字节)
uint8_t Wifi_Rx_flag ;  //WiFi接收完成标志位
uint8_t Zigbee_Rx_num ;	//ZigBee接收数组的索引(可判断接收了多少字节)
uint8_t Zigbee_Rx_flag ;  //ZigBee接收完成标志位


uint8_t Host_AGV_Return_Flag = RESET;	//主车数据上传标志位（默认RESET）
uint8_t AGV_data_Falg = RESET;	//返回从车数据标志位（默认RESET）

uint32_t canu_wifi_rxtime = 0;	//时间片轮询WiFi接收需要时间（默认0）
uint32_t canu_zibe_rxtime = 0;	//时间片轮询ZigBee接收需要时间（默认0）

uint8_t GateStr[6];	//没用到




void Can_WifiRx_Save(uint8_t res)
{
	if(Wifi_Rx_flag == 0)
	{
		canu_wifi_rxtime = gt_get()+10;
		Wifi_Rx_num =0;
		Wifi_Rx_Buf[Wifi_Rx_num]=res;
		Wifi_Rx_flag = 1;
	}
	else if(Wifi_Rx_num < WIFI_MAX_NUM )	
	{
		Wifi_Rx_Buf[++Wifi_Rx_num]=res;	 
	}
}

uint8_t Rx_Flag ;

void Normal_data(void)	  // 正常接收8字节控制指令
{
	uint8_t sum=0;

	if(Wifi_Rx_Buf[7]==0xbb)	 // 判断包尾
	{									  
		//主指令与三位副指令左求和校验
		//注意：在求和溢出时应该对和做256取余。
		 sum=(Wifi_Rx_Buf[2]+Wifi_Rx_Buf[3]+Wifi_Rx_Buf[4]+Wifi_Rx_Buf[5])%256;
		if(sum == Wifi_Rx_Buf[6])
		{
			   Rx_Flag =1;
		}
		else Rx_Flag =0;
	}
}

void Abnormal_data(void)	  //数据异常处理
{
	uint8_t i,j;
	uint8_t sum=0;
	
	if(Wifi_Rx_num <8)			// 异常数据字节数小于8字节不做处理
	{
	   Rx_Flag =0;
	}
	else {
		for(i=0;i<=(Wifi_Rx_num -7);i++)  
		{
			if(Wifi_Rx_Buf[i]==0x55)	   // 寻找包头
			{
			   if(Wifi_Rx_Buf[i+7]==0xbb)	// 判断包尾
			   {
			   	    sum=(Wifi_Rx_Buf[i+2]+Wifi_Rx_Buf[i+3]+Wifi_Rx_Buf[i+4]+Wifi_Rx_Buf[i+5])%256;

		           if(sum==Wifi_Rx_Buf[i+6])	 // 判断求和
	              {
                     for(j=0;j<8;j++)
                     {
                        Wifi_Rx_Buf[j]=Wifi_Rx_Buf[j+i];	 // 数据搬移
                     }
					      Rx_Flag =1;
		          }
	  	         else Rx_Flag =0;
		       }
			}
	    }

	}
}

uint8_t Infrared_Tab[6];			//红外数据存放数组
extern uint8_t make;
void Can_WifiRx_Check(void)
{
	if(Wifi_Rx_flag)
	{	
		if(gt_get_sub(canu_wifi_rxtime) == 0)
		{
			if(Wifi_Rx_Buf[0]==0xFD)  
			{			
				Send_ZigbeeData_To_Fifo( Wifi_Rx_Buf , (Wifi_Rx_num +1)); 
			}
			else if(Wifi_Rx_Buf[0]==0x55)  
			{              
				Normal_data();  
			}
			else
			{
				Abnormal_data();      
			} 	
			Wifi_Rx_flag = 0;
		}
	}
	if(Rx_Flag ==1)	//这里处理APP发送过来的协议解析
	{    
		if(Wifi_Rx_Buf[1] == 0xAA) 	   
		{	
			switch(Wifi_Rx_Buf[2])
			{
			case 0x01:              	//停止
				Send_UpMotor(0 ,0);
				Roadway_Flag_clean();	//清除标志位状态	
				break;
			case 0x02:              //前进
				Roadway_mp_syn();	//码盘同步
				Stop_Flag = 0; Go_Flag = 1; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 0;
				Back_Flag = 0; Track_Flag = 0;
				temp_MP = Wifi_Rx_Buf[5];
				temp_MP <<=  8;
				temp_MP |= Wifi_Rx_Buf[4];
				Car_Spend = Wifi_Rx_Buf[3];
				//set_Test_Times();
				Control(Car_Spend ,Car_Spend);
				break;
			case 0x03:              //后退
				Roadway_mp_syn();	//码盘同步
				Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 0;
				Back_Flag = 1; Track_Flag = 0;
				temp_MP = Wifi_Rx_Buf[5];
				temp_MP <<=  8;
				temp_MP |= Wifi_Rx_Buf[4];
				Car_Spend = Wifi_Rx_Buf[3];
				Control(-Car_Spend ,-Car_Spend);					
				break;
			case 0x04:              //左转
				Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 1;wheel_R_Flag = 0;wheel_Nav_Flag = 0;
				Back_Flag = 0; Track_Flag = 0;
				Car_Spend = Wifi_Rx_Buf[3];				
				Control(-Car_Spend ,Car_Spend);
				break;
			case 0x05:              //右转
				Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 1;wheel_Nav_Flag = 0;
				Back_Flag = 0; Track_Flag = 0;
				Car_Spend = Wifi_Rx_Buf[3];
				Control(Car_Spend,-Car_Spend);
				break;
			case 0x06:              //循迹
				Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 0;
				Back_Flag = 0; Track_Flag = 1;
				Car_Spend = Wifi_Rx_Buf[3];
				//set_Test_Times();
				break;
			case 0x07:				//码盘清零
				
				break;
			case 0x08:				//左转弯--角度											
				Roadway_nav_syn();	//角度同步
				Roadway_mp_syn();	//码盘同步
				Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 1;
				Back_Flag = 0; Track_Flag = 0;
				temp_Nav = Wifi_Rx_Buf[5];
				temp_Nav <<=  8;
				temp_Nav |= Wifi_Rx_Buf[4];
				Car_Spend = Wifi_Rx_Buf[3];				
				Send_UpMotor(-Car_Spend ,Car_Spend);					
				break;
			case 0x09:				//右转弯--角度			
				Roadway_nav_syn();	//角度同步
				Roadway_mp_syn();	//码盘同步
				Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 1;
				Back_Flag = 0; Track_Flag = 0;
				temp_Nav = Wifi_Rx_Buf[5];
				temp_Nav <<=  8;
				temp_Nav |= Wifi_Rx_Buf[4];
				Car_Spend = Wifi_Rx_Buf[3];
				Send_UpMotor(Car_Spend,-Car_Spend);
				break;
			case 0x10:										//红外前三位数据
				Infrared_Tab[0]=Wifi_Rx_Buf[3];
				Infrared_Tab[1]=Wifi_Rx_Buf[4];
				Infrared_Tab[2]=Wifi_Rx_Buf[5];
				break;
			case 0x11:										//红外后三位数据
				Infrared_Tab[3]=Wifi_Rx_Buf[3];//数据第四位
				Infrared_Tab[4]=Wifi_Rx_Buf[4];//低位校验码
				Infrared_Tab[5]=Wifi_Rx_Buf[5];//高位校验码
				break;
			case 0x12:										//通知小车单片机发送红外线
				Infrared_Send(Infrared_Tab,6);
				
				break;									
			case 0x20:	//转向灯控制
				Set_tba_WheelLED(L_LED,Wifi_Rx_Buf[3]);
				Set_tba_WheelLED(R_LED,Wifi_Rx_Buf[4]);
				break;					
			case 0x30:
				Set_tba_Beep(Wifi_Rx_Buf[3]);				//蜂鸣器
				break;
			case 0x40:										//暂未使用
				
				break;
				case 0x50:  				//红外发射控制相片上翻 
				Infrared_Send(TFT_PageUp,4);
				break;
			case 0x51:    					//红外发射控制相片下翻 
				Infrared_Send(TFT_PageDown,4);
				break;
			case 0x61:    					//红外发射控制光源强度档位加1 
				Infrared_Send(Light_plus1,4);				
				break;
			case 0x62:   	 				//红外发射控制光源强度档位加2 
				Infrared_Send(Light_plus2,4);
				break;
			case 0x63:    					//红外发射控制光源强度档位加3 
				Infrared_Send(Light_plus3,4);
				break;
		   case 0x80:						//运动标志物数据返回允许位
			    Host_AGV_Return_Flag = Wifi_Rx_Buf[3];   //SET 允许 / RESET 禁止
				break;
			case 0xA0:						//自动驾驶
				make = 0x01;
				break;
#if 1	//以下是我添加的识别结果回传
			case 0xA1:	//交通灯识别
			{
				break;
			}
			case 0xA2:	//TFT标志物车牌并返回车牌前3位数据
			{
				App_RX_TFTarr[0] = Wifi_Rx_Buf[3];	//【车牌数据1】
				App_RX_TFTarr[1] = Wifi_Rx_Buf[4];	//【车牌数据2】
				App_RX_TFTarr[2] = Wifi_Rx_Buf[5];	//【车牌数据3】			
				break;
			}
			case 0xA3:	//TFT标志物车牌并返回车牌后3位数据
			{
				App_RX_TFTarr[3] = Wifi_Rx_Buf[3];	//【车牌数据4】
				App_RX_TFTarr[4] = Wifi_Rx_Buf[4];	//【车牌数据5】
				App_RX_TFTarr[5] = Wifi_Rx_Buf[5];	//【车牌数据6】
				Yang_ArrToStr(App_RX_TFTarr,App_RX_TFTCarCard,6);	//数组转字符串
				break;	
			}
			case 0xA4:	//TFT显示的不是车牌
			{
				break;	
			}
			case 0xA5:	//备用
			{
				Set_tba_WheelLED(L_LED,SET);
				break;
			}	
			case 0xA6:	//备用
			{
				break;
			}
			case 0xA7:	//识别到二维码提取有效的返回(应该是有效数字，返回类型为字符类型'')
			{
				break;
			}
			case 0xA8:	//备用
			{
				break;
			}
			case 0xA9:	//备用
			{
				break;
			}	
			case 0xB1:	//备用
			{
				break;
			}
			case 0xB2:	//备用
			{
				break;
			}
			case 0xB3:	//备用
			{
				break;
			}
			case 0xB4:	//备用
			{
				break;
			}
			case 0xB5:	//备用
			{
				break;
			}
			case 0xB6:	//备用
			{
				break;
			}			
#endif			
		   default:
				break;
			}
		}
		else  
		{
			Send_ZigbeeData_To_Fifo(Wifi_Rx_Buf,8); 
		}
		Rx_Flag = 0;
	}
}




/**
函数功能：保存ZigBee数据
参    数: 无
返 回 值：无
*/
void Can_ZigBeeRx_Save(uint8_t res)
{
	if(Zigbee_Rx_flag == 0)
	{
		canu_zibe_rxtime = gt_get()+10;
		Zigbee_Rx_num =0;
		Zigb_Rx_Buf[Zigbee_Rx_num]=res;
		Zigbee_Rx_flag = 1;
	}
	else if(Zigbee_Rx_num < ZIGB_RX_MAX )	
	{
		Zigb_Rx_Buf[++Zigbee_Rx_num]=res;	 
	}
		
}

extern uint8_t coordinate;              //随机坐标点
/**
函数功能：ZigBee数据监测
参    数：无
返 回 值：无
*/
void Can_ZigBeeRx_Check(void)
{
	if(Zigbee_Rx_flag)
	{
		if(gt_get_sub(canu_zibe_rxtime) == 0)
		{
			if(Zigb_Rx_Buf[1] == 0x03) 			// 道闸 
			{
//			   if(Zigb_Rx_Buf[2]==0x01)
//			    {				
//					Stop_Flag=Zigb_Rx_Buf[4];	  			 																		
//			    }		
			} 
			else if(Zigb_Rx_Buf[1]==0x0c) 		// ETC
			{
//				if(Zigb_Rx_Buf[2]==0x01)
//			    {
//					if(Zigb_Rx_Buf[3]==0x01)
//					{
//						Stop_Flag=Zigb_Rx_Buf[4] ;	  			 											
//					}							
//			    }		
			}
			else if((Zigb_Rx_Buf[1]==0x0E)&&(Zigb_Rx_Buf[2]==0x01))	//交通灯标志物					
			{
//				Stop_Flag = Zigb_Rx_Buf[4] ;	  						// 0x07进入识别模式 0x08 未进入识别模式
			} 
			else if((Zigb_Rx_Buf[1]==0x10) && (Zigb_Rx_Buf[2]==0x10)) //特殊地形标志物
			{
				if(Zigb_Rx_Buf[3]==0x01)
				{
//					Stop_Flag = Zigb_Rx_Buf[4];
				}
			}
			else if((Zigb_Rx_Buf[1]==0x0D)&&(Zigb_Rx_Buf[2]==0x03)) //立体车库标志物
			{
				if(Zigb_Rx_Buf[3] == 0x01)							//获取立体车库当前层数
				{
//					switch(Zigb_Rx_Buf[4])							
//					{
//						case 1:
//							Stop_Flag = 0x09;						//第一层
//							break;
//						case 2:
//							Stop_Flag = 0x0A;						//第二层
//							break;
//						case 3:
//							Stop_Flag = 0x0B;						//第三层
//							break;
//						case 4:
//							Stop_Flag = 0x0C;						//第四层
//							break;
//					}
				} 
				else if(Zigb_Rx_Buf[3] == 0x02) 					//获取立体车库当前红外状态
				{
					if((Zigb_Rx_Buf[4] == 0x01) && (Zigb_Rx_Buf[5] == 0x01))
					{
                        Garage_Infrared_Flag = 3;
//						Stop_Flag = 0x11;							//前侧红外触发、后侧红外触发
					} 
					else if((Zigb_Rx_Buf[4] == 0x02) && (Zigb_Rx_Buf[5] == 0x02))
					{
                        Garage_Infrared_Flag = 0;						
//						Stop_Flag = 0x22;							//前侧红外未触发、后侧红外未触发
					}
					else if((Zigb_Rx_Buf[4] == 0x01) && (Zigb_Rx_Buf[5] == 0x02))
					{
                        Garage_Infrared_Flag = 1;						
//						Stop_Flag = 0x12;							//前侧红外触发、后侧红外未触发
					}
					else if((Zigb_Rx_Buf[4] == 0x02) && (Zigb_Rx_Buf[5] == 0x01))
					{
                        Garage_Infrared_Flag = 2;	
//						Stop_Flag = 0x21;							//前侧红外未触发、后侧红外触发
					}
//						sprintf((char *)arr, "Garage--%d\n", Garage_Infrared_Flag);
//						Send_InfoData_To_Fifo((char *)arr, 11);					
				}
			}else if((Zigb_Rx_Buf[1]==0x07)&&(Zigb_Rx_Buf[2]==0x01)) //报警台标志物随机返回坐标点
             {         
                    coordinate = Zigb_Rx_Buf[3];             
             }
#if 0	//接收从车的(暂时屏蔽)		 
			else if((Zigb_Rx_Buf[0] == 0x55)&&(Zigb_Rx_Buf[1] == 0x02))		//返回从车数据
			{		
					memcpy(Follower_Tab,Zigb_Rx_Buf,50);
					AGV_data_Falg = SET;
			} 
#endif		
			else if((Zigb_Rx_Buf[0] == 0x55)&&(Zigb_Rx_Buf[1] == 0x01))	//如果接收到从车的命令
			{
				switch(Zigb_Rx_Buf[2])	//判断主指令
				{
					case 0xA0:	//测试左转向灯亮
					{
						Set_tba_WheelLED(R_LED,SET);	//左转向灯亮
						break;
					}
					default:break;
				}
			}
			else if(Zigb_Rx_Buf[1]==0x06)	 //语音播报返回
			{
				if(Zigb_Rx_Buf[2]==0x01)
				{
						Stop_Flag=Zigb_Rx_Buf[3] ;	  // 语音芯片状态返回
				}			
			}
			Zigbee_Rx_flag = 0;
		}	
	}
}


/** 暂未使用
函数功能：设置循迹上传更新时间
参    数：无
返 回 值：无
*/
void Canuser_upTrackTime(void)
{
	static uint8_t run_mode = 0;
	
	if(gt_get() < 1000)
		run_mode = 0;
	else if(run_mode == 0)
	{
		run_mode = 1;
		Host_Set_UpTrack(50);
	}
}

#define UART_or_CAN    0   //  0---UART  1---CAN

/**
函数功能：CAN查询、发送接收检测
参    数：无
返 回 值：无
*/
void Canuser_main(void)
{
	CanP_Host_Main();
	//CanP_CanTx_Check();				//CAN总线发送数据监测
	CanP_CanTx_Check_fIrq();
}


//end file

