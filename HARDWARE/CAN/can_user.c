#include "allhead.h"
#define __CAN_USER_C__
#include "can_user.h"


uint8_t Wifi_Rx_Buf[ WIFI_MAX_NUM ];	//WiFi������������(�ն˷��͹�����)(�����С200)
uint8_t Zigb_Rx_Buf[ ZIGB_RX_MAX ];	//ZigBee��������(��־��ش�������)(�����С200)
uint8_t Wifi_Rx_num ;//WiFi�������������(���жϽ����˶����ֽ�)
uint8_t Wifi_Rx_flag ;  //WiFi������ɱ�־λ
uint8_t Zigbee_Rx_num ;	//ZigBee�������������(���жϽ����˶����ֽ�)
uint8_t Zigbee_Rx_flag ;  //ZigBee������ɱ�־λ


uint8_t Host_AGV_Return_Flag = RESET;	//���������ϴ���־λ��Ĭ��RESET��
uint8_t AGV_data_Falg = RESET;	//���شӳ����ݱ�־λ��Ĭ��RESET��

uint32_t canu_wifi_rxtime = 0;	//ʱ��Ƭ��ѯWiFi������Ҫʱ�䣨Ĭ��0��
uint32_t canu_zibe_rxtime = 0;	//ʱ��Ƭ��ѯZigBee������Ҫʱ�䣨Ĭ��0��

uint8_t GateStr[6];	//û�õ�




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

void Normal_data(void)	  // ��������8�ֽڿ���ָ��
{
	uint8_t sum=0;

	if(Wifi_Rx_Buf[7]==0xbb)	 // �жϰ�β
	{									  
		//��ָ������λ��ָ�������У��
		//ע�⣺��������ʱӦ�öԺ���256ȡ�ࡣ
		 sum=(Wifi_Rx_Buf[2]+Wifi_Rx_Buf[3]+Wifi_Rx_Buf[4]+Wifi_Rx_Buf[5])%256;
		if(sum == Wifi_Rx_Buf[6])
		{
			   Rx_Flag =1;
		}
		else Rx_Flag =0;
	}
}

void Abnormal_data(void)	  //�����쳣����
{
	uint8_t i,j;
	uint8_t sum=0;
	
	if(Wifi_Rx_num <8)			// �쳣�����ֽ���С��8�ֽڲ�������
	{
	   Rx_Flag =0;
	}
	else {
		for(i=0;i<=(Wifi_Rx_num -7);i++)  
		{
			if(Wifi_Rx_Buf[i]==0x55)	   // Ѱ�Ұ�ͷ
			{
			   if(Wifi_Rx_Buf[i+7]==0xbb)	// �жϰ�β
			   {
			   	    sum=(Wifi_Rx_Buf[i+2]+Wifi_Rx_Buf[i+3]+Wifi_Rx_Buf[i+4]+Wifi_Rx_Buf[i+5])%256;

		           if(sum==Wifi_Rx_Buf[i+6])	 // �ж����
	              {
                     for(j=0;j<8;j++)
                     {
                        Wifi_Rx_Buf[j]=Wifi_Rx_Buf[j+i];	 // ���ݰ���
                     }
					      Rx_Flag =1;
		          }
	  	         else Rx_Flag =0;
		       }
			}
	    }

	}
}

uint8_t Infrared_Tab[6];			//�������ݴ������
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
	if(Rx_Flag ==1)	//���ﴦ��APP���͹�����Э�����
	{    
		if(Wifi_Rx_Buf[1] == 0xAA) 	   
		{	
			switch(Wifi_Rx_Buf[2])
			{
			case 0x01:              	//ֹͣ
				Send_UpMotor(0 ,0);
				Roadway_Flag_clean();	//�����־λ״̬	
				break;
			case 0x02:              //ǰ��
				Roadway_mp_syn();	//����ͬ��
				Stop_Flag = 0; Go_Flag = 1; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 0;
				Back_Flag = 0; Track_Flag = 0;
				temp_MP = Wifi_Rx_Buf[5];
				temp_MP <<=  8;
				temp_MP |= Wifi_Rx_Buf[4];
				Car_Spend = Wifi_Rx_Buf[3];
				//set_Test_Times();
				Control(Car_Spend ,Car_Spend);
				break;
			case 0x03:              //����
				Roadway_mp_syn();	//����ͬ��
				Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 0;
				Back_Flag = 1; Track_Flag = 0;
				temp_MP = Wifi_Rx_Buf[5];
				temp_MP <<=  8;
				temp_MP |= Wifi_Rx_Buf[4];
				Car_Spend = Wifi_Rx_Buf[3];
				Control(-Car_Spend ,-Car_Spend);					
				break;
			case 0x04:              //��ת
				Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 1;wheel_R_Flag = 0;wheel_Nav_Flag = 0;
				Back_Flag = 0; Track_Flag = 0;
				Car_Spend = Wifi_Rx_Buf[3];				
				Control(-Car_Spend ,Car_Spend);
				break;
			case 0x05:              //��ת
				Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 1;wheel_Nav_Flag = 0;
				Back_Flag = 0; Track_Flag = 0;
				Car_Spend = Wifi_Rx_Buf[3];
				Control(Car_Spend,-Car_Spend);
				break;
			case 0x06:              //ѭ��
				Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 0;
				Back_Flag = 0; Track_Flag = 1;
				Car_Spend = Wifi_Rx_Buf[3];
				//set_Test_Times();
				break;
			case 0x07:				//��������
				
				break;
			case 0x08:				//��ת��--�Ƕ�											
				Roadway_nav_syn();	//�Ƕ�ͬ��
				Roadway_mp_syn();	//����ͬ��
				Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 1;
				Back_Flag = 0; Track_Flag = 0;
				temp_Nav = Wifi_Rx_Buf[5];
				temp_Nav <<=  8;
				temp_Nav |= Wifi_Rx_Buf[4];
				Car_Spend = Wifi_Rx_Buf[3];				
				Send_UpMotor(-Car_Spend ,Car_Spend);					
				break;
			case 0x09:				//��ת��--�Ƕ�			
				Roadway_nav_syn();	//�Ƕ�ͬ��
				Roadway_mp_syn();	//����ͬ��
				Stop_Flag = 0; Go_Flag = 0; wheel_L_Flag = 0;wheel_R_Flag = 0;wheel_Nav_Flag = 1;
				Back_Flag = 0; Track_Flag = 0;
				temp_Nav = Wifi_Rx_Buf[5];
				temp_Nav <<=  8;
				temp_Nav |= Wifi_Rx_Buf[4];
				Car_Spend = Wifi_Rx_Buf[3];
				Send_UpMotor(Car_Spend,-Car_Spend);
				break;
			case 0x10:										//����ǰ��λ����
				Infrared_Tab[0]=Wifi_Rx_Buf[3];
				Infrared_Tab[1]=Wifi_Rx_Buf[4];
				Infrared_Tab[2]=Wifi_Rx_Buf[5];
				break;
			case 0x11:										//�������λ����
				Infrared_Tab[3]=Wifi_Rx_Buf[3];//���ݵ���λ
				Infrared_Tab[4]=Wifi_Rx_Buf[4];//��λУ����
				Infrared_Tab[5]=Wifi_Rx_Buf[5];//��λУ����
				break;
			case 0x12:										//֪ͨС����Ƭ�����ͺ�����
				Infrared_Send(Infrared_Tab,6);
				
				break;									
			case 0x20:	//ת��ƿ���
				Set_tba_WheelLED(L_LED,Wifi_Rx_Buf[3]);
				Set_tba_WheelLED(R_LED,Wifi_Rx_Buf[4]);
				break;					
			case 0x30:
				Set_tba_Beep(Wifi_Rx_Buf[3]);				//������
				break;
			case 0x40:										//��δʹ��
				
				break;
				case 0x50:  				//���ⷢ�������Ƭ�Ϸ� 
				Infrared_Send(TFT_PageUp,4);
				break;
			case 0x51:    					//���ⷢ�������Ƭ�·� 
				Infrared_Send(TFT_PageDown,4);
				break;
			case 0x61:    					//���ⷢ����ƹ�Դǿ�ȵ�λ��1 
				Infrared_Send(Light_plus1,4);				
				break;
			case 0x62:   	 				//���ⷢ����ƹ�Դǿ�ȵ�λ��2 
				Infrared_Send(Light_plus2,4);
				break;
			case 0x63:    					//���ⷢ����ƹ�Դǿ�ȵ�λ��3 
				Infrared_Send(Light_plus3,4);
				break;
		   case 0x80:						//�˶���־�����ݷ�������λ
			    Host_AGV_Return_Flag = Wifi_Rx_Buf[3];   //SET ���� / RESET ��ֹ
				break;
			case 0xA0:						//�Զ���ʻ
				make = 0x01;
				break;
#if 1	//�����������ӵ�ʶ�����ش�
			case 0xA1:	//��ͨ��ʶ��
			{
				break;
			}
			case 0xA2:	//TFT��־�ﳵ�Ʋ����س���ǰ3λ����
			{
				App_RX_TFTarr[0] = Wifi_Rx_Buf[3];	//����������1��
				App_RX_TFTarr[1] = Wifi_Rx_Buf[4];	//����������2��
				App_RX_TFTarr[2] = Wifi_Rx_Buf[5];	//����������3��			
				break;
			}
			case 0xA3:	//TFT��־�ﳵ�Ʋ����س��ƺ�3λ����
			{
				App_RX_TFTarr[3] = Wifi_Rx_Buf[3];	//����������4��
				App_RX_TFTarr[4] = Wifi_Rx_Buf[4];	//����������5��
				App_RX_TFTarr[5] = Wifi_Rx_Buf[5];	//����������6��
				Yang_ArrToStr(App_RX_TFTarr,App_RX_TFTCarCard,6);	//����ת�ַ���
				break;	
			}
			case 0xA4:	//TFT��ʾ�Ĳ��ǳ���
			{
				break;	
			}
			case 0xA5:	//����
			{
				Set_tba_WheelLED(L_LED,SET);
				break;
			}	
			case 0xA6:	//����
			{
				break;
			}
			case 0xA7:	//ʶ�𵽶�ά����ȡ��Ч�ķ���(Ӧ������Ч���֣���������Ϊ�ַ�����'')
			{
				break;
			}
			case 0xA8:	//����
			{
				break;
			}
			case 0xA9:	//����
			{
				break;
			}	
			case 0xB1:	//����
			{
				break;
			}
			case 0xB2:	//����
			{
				break;
			}
			case 0xB3:	//����
			{
				break;
			}
			case 0xB4:	//����
			{
				break;
			}
			case 0xB5:	//����
			{
				break;
			}
			case 0xB6:	//����
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
�������ܣ�����ZigBee����
��    ��: ��
�� �� ֵ����
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

extern uint8_t coordinate;              //��������
/**
�������ܣ�ZigBee���ݼ��
��    ������
�� �� ֵ����
*/
void Can_ZigBeeRx_Check(void)
{
	if(Zigbee_Rx_flag)
	{
		if(gt_get_sub(canu_zibe_rxtime) == 0)
		{
			if(Zigb_Rx_Buf[1] == 0x03) 			// ��բ 
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
			else if((Zigb_Rx_Buf[1]==0x0E)&&(Zigb_Rx_Buf[2]==0x01))	//��ͨ�Ʊ�־��					
			{
//				Stop_Flag = Zigb_Rx_Buf[4] ;	  						// 0x07����ʶ��ģʽ 0x08 δ����ʶ��ģʽ
			} 
			else if((Zigb_Rx_Buf[1]==0x10) && (Zigb_Rx_Buf[2]==0x10)) //������α�־��
			{
				if(Zigb_Rx_Buf[3]==0x01)
				{
//					Stop_Flag = Zigb_Rx_Buf[4];
				}
			}
			else if((Zigb_Rx_Buf[1]==0x0D)&&(Zigb_Rx_Buf[2]==0x03)) //���峵���־��
			{
				if(Zigb_Rx_Buf[3] == 0x01)							//��ȡ���峵�⵱ǰ����
				{
//					switch(Zigb_Rx_Buf[4])							
//					{
//						case 1:
//							Stop_Flag = 0x09;						//��һ��
//							break;
//						case 2:
//							Stop_Flag = 0x0A;						//�ڶ���
//							break;
//						case 3:
//							Stop_Flag = 0x0B;						//������
//							break;
//						case 4:
//							Stop_Flag = 0x0C;						//���Ĳ�
//							break;
//					}
				} 
				else if(Zigb_Rx_Buf[3] == 0x02) 					//��ȡ���峵�⵱ǰ����״̬
				{
					if((Zigb_Rx_Buf[4] == 0x01) && (Zigb_Rx_Buf[5] == 0x01))
					{
                        Garage_Infrared_Flag = 3;
//						Stop_Flag = 0x11;							//ǰ����ⴥ���������ⴥ��
					} 
					else if((Zigb_Rx_Buf[4] == 0x02) && (Zigb_Rx_Buf[5] == 0x02))
					{
                        Garage_Infrared_Flag = 0;						
//						Stop_Flag = 0x22;							//ǰ�����δ������������δ����
					}
					else if((Zigb_Rx_Buf[4] == 0x01) && (Zigb_Rx_Buf[5] == 0x02))
					{
                        Garage_Infrared_Flag = 1;						
//						Stop_Flag = 0x12;							//ǰ����ⴥ����������δ����
					}
					else if((Zigb_Rx_Buf[4] == 0x02) && (Zigb_Rx_Buf[5] == 0x01))
					{
                        Garage_Infrared_Flag = 2;	
//						Stop_Flag = 0x21;							//ǰ�����δ�����������ⴥ��
					}
//						sprintf((char *)arr, "Garage--%d\n", Garage_Infrared_Flag);
//						Send_InfoData_To_Fifo((char *)arr, 11);					
				}
			}else if((Zigb_Rx_Buf[1]==0x07)&&(Zigb_Rx_Buf[2]==0x01)) //����̨��־��������������
             {         
                    coordinate = Zigb_Rx_Buf[3];             
             }
#if 0	//���մӳ���(��ʱ����)		 
			else if((Zigb_Rx_Buf[0] == 0x55)&&(Zigb_Rx_Buf[1] == 0x02))		//���شӳ�����
			{		
					memcpy(Follower_Tab,Zigb_Rx_Buf,50);
					AGV_data_Falg = SET;
			} 
#endif		
			else if((Zigb_Rx_Buf[0] == 0x55)&&(Zigb_Rx_Buf[1] == 0x01))	//������յ��ӳ�������
			{
				switch(Zigb_Rx_Buf[2])	//�ж���ָ��
				{
					case 0xA0:	//������ת�����
					{
						Set_tba_WheelLED(R_LED,SET);	//��ת�����
						break;
					}
					default:break;
				}
			}
			else if(Zigb_Rx_Buf[1]==0x06)	 //������������
			{
				if(Zigb_Rx_Buf[2]==0x01)
				{
						Stop_Flag=Zigb_Rx_Buf[3] ;	  // ����оƬ״̬����
				}			
			}
			Zigbee_Rx_flag = 0;
		}	
	}
}


/** ��δʹ��
�������ܣ�����ѭ���ϴ�����ʱ��
��    ������
�� �� ֵ����
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
�������ܣ�CAN��ѯ�����ͽ��ռ��
��    ������
�� �� ֵ����
*/
void Canuser_main(void)
{
	CanP_Host_Main();
	//CanP_CanTx_Check();				//CAN���߷������ݼ��
	CanP_CanTx_Check_fIrq();
}


//end file
