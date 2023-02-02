#include "allhead.h"

/******** �궨���� *********/
#define debug_A72 0	//A72����ѡ��


/******** ȫ�־�̬������ *********/
static uint32_t Power_check_times;          // �����������
static uint32_t LED_twinkle_times;          // LED��˸����
static uint32_t WIFI_Upload_data_times;     // ͨ��Wifi�ϴ���������
static uint32_t RFID_Init_Check_times;      // RFID��ʼ�����ʱ������
static uint32_t task_times;                 // ����ʱ����ʱ������

/******** ȫ�ֱ����� *********/
uint8_t RFID_Flag = 0;          	           // RFID����־λ
uint16_t distance = 0;                      // ��¼����������
uint8_t gear_init = 0;                      // ����·�Ƴ�ʼ��λ
uint8_t coordinate;                         // ��������
uint8_t number = 0;                         // ����ֵ
uint8_t make = 0;                           // ȫ�Զ���ʻ��־λ
uint16_t random;                            // �����

uint8_t  Go_Speed  = 40;                    // ȫ���н��ٶ�ֵ
uint8_t  wheel_Speed = 90;                  // ȫ��ת���ٶ�ֵ
uint8_t  RFID_Go_Speed  = 35;               // RFID·�ε��ٶȸ����³����ɳ����е���
uint16_t Go_Temp = 260;                     // ȫ��ǰ������ֵ
uint16_t wheel_Time = 330;                  // ȫ��ת45��ʱ��


uint8_t Stere1[6]={0xFF,0x14,0x01,0x00,0x00,0x00};		// ������ʾ ��ʾ ǰ��ѧУ����������
uint8_t Stere2[6]={0xFF,0x14,0x02,0x00,0x00,0x00};		// ������ʾ ��ʾ ǰ��ѧУ����������
uint8_t Stere3[6]={0xFF,0x14,0x03,0x00,0x00,0x00};		// ������ʾ ��ʾ ǰ��ѧУ����������
uint8_t Stere4[6]={0xFF,0x14,0x04,0x00,0x00,0x00};		// ������ʾ ��ʾ ǰ��ѧУ����������

uint8_t data1[3]={0x55,0xA1,0x01};		//���߰�׿ʶ���ά��
uint8_t data2[3]={0x55,0xA1,0x02};		//���߰�׿ʶ���ά��
uint8_t data3[3]={0x55,0xA1,0x03};		//���߰�׿ʶ���ά��
uint8_t data4[3]={0x55,0xA1,0x04};		//���߰�׿ʶ���ά��
uint8_t data5[3]={0x55,0xA1,0x10};		//���߰�׿ʶ���ά��
uint8_t data6[3]={0x55,0xA1,0x11};		//���߰�׿ʶ���ά��

//uint8_t data3[3]={0x55,0xA1,0x10};		//���߰�׿ʶ���ά��

/******** ȫ�־�̬������ *********/
static void KEY_Check(void);                // ������⺯��
static void Hardware_Init(void);            // Ӳ����ʼ������


uint8_t AutoFlag = 0;//ȫ�Զ�ִ�п�ʼ��־λ
/* ȫ�Զ����к��� */
void Car_Thread(void)
{
	switch(make)
	{
      case 0x01:
      {
          //��������
           Send_ZigbeeData_To_Fifo(GarageA_To1, 8);		//���峵�⽵��1��
           delay_ms(500);
           Send_ZigbeeData_To_Fifo(GarageA_To1, 8);		//���峵�⽵��1��
           delay_ms(500);
           Send_ZigbeeData_To_Fifo(SEG_TimOpen, 8);		//������ʾ��ʼ��ʱ
           delay_ms(500);
           Send_ZigbeeData_To_Fifo(SEG_TimOpen, 8);		//������ʾ��ʼ��ʱ
           delay_ms(500);
           make = 0x02;
         break;
      }
      case 0x02:
      {
          //���������
           Car_Track(Go_Speed);    //������
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
           Ultrasonic_Ranging();   //��ȡ����������
           distance = dis;         //��¼����������
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
          //������ά��ʶ��
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
           delay_ms(200);//���н���D6��
           make=0x04;
         break;
      }
      case 0x04:
      {
          //ͨ��ETCϵͳ
           ETC_Get_Zigbee();//��ѯETCϵͳ�Ƿ��
           Car_Track(Go_Speed);
           Car_Go(Go_Speed,Go_Temp);
           delay_ms(200);
           make=0x05;
         break;
      }
      case 0x05:
      {  
         //����·�Ƶ�����2��
           gear_init = Light_Inf(2);
           delay_ms(500);
           
           Car_Left(wheel_Speed);
           delay_ms(200);
           make=0x06;
         break;
      }
      case 0x06:
      {
          //��ͨ��Aʶ��
           Send_ZigbeeData_To_Fifo(TrafficA_Open, 8);		// ͨ������ZigBee�����ý�ͨ�ƽ���ʶ��ģʽ
           delay_ms(500);
           delay_ms(500);
           delay_ms(500);
           delay_ms(500);
           delay_ms(500);
           delay_ms(500);
           Send_ZigbeeData_To_Fifo(TrafficA_Green, 8);		// ������ɫȷ��
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
            //����TFT��ʾ��־��A��ҳ
           Send_ZigbeeData_To_Fifo(TFT_PageDown, 8);		   // TFT��ҳ
           for(int a=0;a<7;a++)
           {
              delay_ms(500);
           }
           Send_ZigbeeData_To_Fifo(TFT_PageDown, 8);		   // TFT��ҳ
           for(int a=0;a<7;a++)
           {
              delay_ms(500);
           }
           Send_ZigbeeData_To_Fifo(TFT_PageDown, 8);		   // TFT��ҳ
           for(int a=0;a<7;a++)
           {
              delay_ms(500);
           }
           //ͨ��������α�־��
           Car_Left(wheel_Speed);
           Car_Left(wheel_Speed);
           delay_ms(200);
           Car_Track(Go_Speed);   //��⵽�������
           Car_Go(Go_Speed+10,900);  //ͨ���������
           Car_Track(Go_Speed);
           Car_Go(Go_Speed,Go_Temp);
           delay_ms(200);
           make=0x08;
         break;
      }
      case 0x08:
      {
         //����ʶ������
           BKRC_Voice_Extern(0);
           
           Car_Right(wheel_Speed);
           delay_ms(200);
           
           //�򿪵�բ
           Send_ZigbeeData_To_Fifo(Gate_Open, 8);		   // �򿪵�բ
           delay_ms(500);
           Send_ZigbeeData_To_Fifo(Gate_Open, 8);		   // �򿪵�բ
           delay_ms(500);
           delay_ms(500);
           
           Car_Track(Go_Speed);
           Car_Go(Go_Speed,Go_Temp);
           delay_ms(200);
           
           Car_Right(wheel_Speed);
           delay_ms(200);
           
           //��������̨
           Car_R45(wheel_Speed,wheel_Time);
           delay_ms(500);
           Infrared_Send(Alarm_Open,6);   //ͨ�����ⷢ�����ݿ������̨
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
         //����������ʾ��־��
           Car_R45(wheel_Speed,wheel_Time);
           Infrared_Send(Stere2,6);    //����������ʾ��־����ʾǰ��ѧУ��������
           delay_ms(500);
           
           Car_Left(wheel_Speed);
           delay_ms(200);
           Car_Track(Go_Speed);
           Car_Go(Go_Speed,Go_Temp);
           delay_ms(200);
           
           //�������
           Car_Right(wheel_Speed);
           delay_ms(200);
           Car_Track(Go_Speed);
           delay_ms(200);
           Car_Back(50,2000);
           Garage_Cont_Zigbee('A', 2);		                  //���峵��A����2��
           delay_ms(500);
           Send_ZigbeeData_To_Fifo(Charge_Open, 8);		   //�������߳��
           delay_ms(500);
           Send_ZigbeeData_To_Fifo(SMG_TimClose, 8);		   //�رռ�ʱ
           delay_ms(500);
           
           task_times = gt_get() + 90000; //���ó�ʱ����ʱ��Ϊ90��  
           make=0;
         break;
      }
      
      case 0x10:
      {
         //-------------��ʱ������-------------------
         if(gt_get_sub(task_times) == 0)      //��ʱ����
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

/* ������⺯�� */
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
	
    uint16_t Light_Value = 0;               // ��ǿ��ֵ
    uint16_t CodedDisk_Value = 0;           // ����ֵ
    Hardware_Init();                        // Ӳ����ʼ��
    LED_twinkle_times = gt_get() + 50;
    Power_check_times = gt_get() + 200;
    WIFI_Upload_data_times = gt_get() + 200;
    RFID_Init_Check_times = gt_get() + 200;
    Principal_Tab[0] = 0x55;                // ���������ϴ�ָ���ͷ
    Principal_Tab[1] = 0xAA;
    Follower_Tab[0] = 0x55;                 // �������䳵(�ӳ�)�����ϴ�ָ���ͷ
    Follower_Tab[1] = 0x02;
    Send_UpMotor(0, 0);	
    while(1)
    {
        KEY_Check();                                    // �������
#if 1
		if(AutoFlag)
		{
			Yang_Automatic_1();
		}
#endif		
#if 0
		//ѭ�����ʶ��ش�(��Ҫ�ж�ʶ��Ļش��ٴ�)���ڶ�ʱ���ǿ���
		Voice_Drive();	
#endif				
        Can_WifiRx_Check();                             // Wifi�������ݴ���(�ж�APP���͹�����)
        Can_ZigBeeRx_Check();                           // Zigbee�������ݴ���
		  Car_Thread();									        // ȫ�Զ����г���
        if(gt_get_sub(LED_twinkle_times) == 0)          // ����ָʾ��
        {
            LED_twinkle_times = gt_get() + 50;          // LED4״̬ȡ��
            LED4 = !LED4;
        }

        if(gt_get_sub(Power_check_times) == 0)          // ��ص������
        {
            Power_check_times = gt_get() + 200;
            Power_Check();
        }

#if 1
        if(gt_get_sub(RFID_Init_Check_times) == 0)      // RFID��ʼ�����
        {
            RFID_Init_Check_times =  gt_get() + 200;
            if(Rc522_GetLinkFlag() == 0)	//��ʼ��ʧ��
            {
                Readcard_daivce_Init();
                MP_SPK = !MP_SPK;	//�����������
            }
            else	//��ʾ��ʼ���ɹ�
            {
				MP_SPK = 0;
				LED1 = !LED1;
				Rc522_LinkTest();
            }
        }
#endif
        if(gt_get_sub(WIFI_Upload_data_times) == 0)         // �����ϴ�
        {
            WIFI_Upload_data_times =  gt_get() + 500;
            if(Host_AGV_Return_Flag == RESET)               // ���������ϴ�
            {
                Principal_Tab[2] = Stop_Flag;               // ����״̬
                Principal_Tab[3] = Get_tba_phsis_value();   // ����״̬

                Ultrasonic_Ranging();                       // ���������ݲɼ�
                Principal_Tab[4] = dis % 256;               // ���������ݵͰ�λ
                Principal_Tab[5] = dis / 256;               // ���������ݸ߰�λ

                Light_Value = Get_Bh_Value();               // ��ǿ�ȴ��������ݲɼ�
                Principal_Tab[6] = Light_Value % 256;       // ��ǿ�����ݵͰ�λ
                Principal_Tab[7] = Light_Value / 256;       // ��ǿ�����ݸ߰�λ

                CodedDisk_Value = CanHost_Mp;               // ����ֵ
                Principal_Tab[8] = CodedDisk_Value % 256;
                Principal_Tab[9] = CodedDisk_Value / 256;

                Principal_Tab[10] = coordinate;             //���������Ԯ�����
                Send_WifiData_To_Fifo(Principal_Tab, 13);   // ͨ��Wifi�ϴ���������(�������β�Ȼapp���Ӳ��˳�)
#if debug_A72				
                UartA72_TxClear();
                UartA72_TxAddStr(Principal_Tab, 13);        // ͨ�������ϴ���������
                UartA72_TxStart();
#endif				
				coordinate = 0 ;
            }
            else if((Host_AGV_Return_Flag == SET) && (AGV_data_Falg == SET))
            {
#if debug_A72
                UartA72_TxClear();
                UartA72_TxAddStr(Follower_Tab, 50);         // ͨ�������ϴ��ӳ�����
                UartA72_TxStart();
#endif				
                Send_WifiData_To_Fifo(Follower_Tab, 50);    // ͨ��Wifi�ϴ��ӳ�����
                AGV_data_Falg = 0;
            }
        }
    }
}

/* Ӳ����ʼ������ */
//û�õ��Դ��ն����Բ���ҪUartA72_Init
//	TIM1	TIM8~TIM11 ��ʱ��Ϊ 168M
//	TIM2~TIM7 	TIM12~TIM14 ��ʱ��Ϊ 84M
void Hardware_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);     // �жϷ���
    delay_init(168);                                    // ��ʱ��ʼ��
    Tba_Init();                                         // ������ʼ��
    Infrared_Init();                                    // �����ʼ��
    Cba_Init();                                         // ���İ��ʼ��
    Ultrasonic_Init();                                  // ��������ʼ��	(TIM6Ƶ��0.01ms,�ж����ȼ�0,8)+(�ⲿ�жϣ����ȼ�0,7)
    Hard_Can_Init();                                    // CAN���߳�ʼ��
    BH1750_Configure();                                 // BH1750��ʼ������
    BKRC_Voice_Init();									// С������ģ���ʼ��
#if debug_A72	
	UartA72_Init();										//�Դ��ն˵�(�ɲ�Ҫ)
#endif	
    Electricity_Init();                                 // ��������ʼ��
    Can_check_Init(7, 83);                              // CAN���߶�ʱ����ʼ��	(TIM7Ƶ��0.008ms,�ж����ȼ�0,4)
    roadway_check_TimInit(1999, 167);                   // ·�����	(TIM9Ƶ��2ms,�ж����ȼ�0,5)
    Timer_Init(999, 167);                               // ��������ͨѶʱ��֡	(TIM10Ƶ��1ms,�ж����ȼ�0,6)
    Readcard_daivce_Init();                         	// RFID��ʼ��
#if 0
	USART1_Configure(115200);	//����1���õ����Ե���ʱ�ſ�������ʱ����رղ�ȻӰ���������ܣ���
#endif
#if 1
	Timer2_Init(167,999);	//��ʱ��2(Ƶ��2ms���ж����ȼ�0,9)
#endif	
}

