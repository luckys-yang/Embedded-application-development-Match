#ifndef __ALLHEAD_H
#define __ALLHEAD_H
#include <stdio.h>	//c��
#include <string.h>	//c��
#include <stdlib.h>	//c��
#include "stm32f4xx.h"	//stm32f4ͷ�ļ�
#include "delay.h"	//��ʱͷ�ļ�
#include "infrared.h"	//���ⷢ��ͷ�ļ�
#include "cba.h"	//���İ�ͷ�ļ�
#include "ultrasonic.h"	//������ͷ�ļ�
#include "canp_hostcom.h"	//CAN���ݽ���ͷ�ļ�
#include "hard_can.h"	//CAN��ʼ������ͷ�ļ�
#include "bh1750.h"	//��ȴ�����ͷ�ļ�
#include "power_check.h"	//�����ɼ�ͷ�ļ�
#include "can_user.h"	//CAN�û����ݼ���ͷ�ļ�(����)
#include "data_base.h"	//�������鶨��ͷ�ļ�
#include "roadway_check.h"	//����Ѱ����·�����ͷ�ļ�
#include "tba.h"	//�����ͷ�ļ�
#include "swopt_drv.h"	//δ֪(û�õ�)
#include "uart_a72.h"	//���Դ��ն�(û�õ�)
#include "Can_check.h"	//CAN���߼��ͷ�ļ�
#include "Timer.h"	//TIM10ʱ��Ƭ��ѯ����ͷ�ļ�
#include "Rc522.h"	//RFIDͷ�ļ�
#include "bkrc_voice.h"	//С������ͷ�ļ�
#include "drive.h"	//ϵͳ���õĺ���ͷ�ļ�
#if 0	//���õ�
#include "usart.h"	//����1ͷ�ļ�
#endif
#include "sys.h"	//IO����ͷ�ļ�
#include "data_filtering.h"	//�˲��㷨ͷ�ļ�
#include "fifo_drv.h"
#include "yang.h"	//���Զ����ͷ�ļ�(����)


/************************************main.c��ȫ�ֱ���************************************/

extern uint8_t RFID_Flag;          	           // RFID����־λ(Ĭ��0)
extern uint16_t distance;                      // ��¼����������(Ĭ��0)(Ĭ��0)
extern uint8_t gear_init;                      // ����·�Ƴ�ʼ��λ(Ĭ��0)(Ĭ��0)
extern uint8_t coordinate;                         // ��������
extern uint8_t number;                         // ����ֵ(Ĭ��0)
extern uint8_t make;                           // ȫ�Զ���ʻ��־λ(Ĭ��0)
extern uint16_t random;                            // �����
//ȫ�Զ�ִ�к������ֵ
extern uint8_t  Go_Speed;                    // ȫ���н��ٶ�ֵ(Ĭ��40)
extern uint8_t  wheel_Speed;                  // ȫ��ת���ٶ�ֵ(Ĭ��90)
extern uint8_t  RFID_Go_Speed;               // RFID·�ε��ٶȸ����³����ɳ����е���(Ĭ��35)
extern uint16_t Go_Temp;                     // ȫ��ǰ������ֵ(Ĭ��260)
extern uint16_t wheel_Time;                  // ȫ��ת45��ʱ��(Ĭ��330)
//������ʾ
extern uint8_t Stere1[6];		//��ʾ ��ǰ��ѧУ���������С�(0xFF,0x14,0x01,0x00,0x00,0x00)
extern uint8_t Stere2[6];		//��ʾ ��ǰ��ѧУ���������С�(0xFF,0x14,0x02,0x00,0x00,0x00)
extern uint8_t Stere3[6];		//��ʾ ��ǰ��ѧУ���������С�(0xFF,0x14,0x03,0x00,0x00,0x00)
extern uint8_t Stere4[6];		//��ʾ ��ǰ��ѧУ���������С�(0xFF,0x14,0x04,0x00,0x00,0x00)
//��׿��ά��ʶ��
extern uint8_t data1[3];		//���߰�׿ʶ���ά��(0x55,0xA1,0x01)
extern uint8_t data2[3];		//���߰�׿ʶ���ά��(0x55,0xA1,0x02)
extern uint8_t data3[3];		//���߰�׿ʶ���ά��(0x55,0xA1,0x03)
extern uint8_t data4[3];		//���߰�׿ʶ���ά��(0x55,0xA1,0x04)
extern uint8_t data5[3];		//���߰�׿ʶ���ά��(0x55,0xA1,0x10)
extern uint8_t data6[3];		//���߰�׿ʶ���ά��(0x55,0xA1,0x11)

extern uint8_t AutoFlag;//ȫ�Զ�ִ�п�ʼ��־λ(Ĭ��0)

/****************************************************************************************/


/********************************ultrasonic.c��ȫ�ֱ���**********************************/

extern float Ultrasonic_Value;		//�洢���������������ʱ����(Ĭ��0)
extern uint32_t Ultrasonic_Num;	//����������ֵ(Ĭ��0)
extern uint16_t dis;	//�洢����������������ձ���(Ĭ��0)

/****************************************************************************************/


/************************************bh1750.c��ȫ�ֱ���***********************************/

extern uint8_t  BUF[4]; //��ȴ������������ݻ�����(Ĭ��0)

/****************************************************************************************/


/************************************power_check.c��ȫ�ֱ���******************************/

extern uint8_t pwr_ck_l,pwr_ck_r;  // ��������(Ĭ��0)
extern uint8_t Electric_Buf[2];	//(Ĭ��0)

/*****************************************************************************************/


/**********************************************rc522.c��ȫ�ֱ���**************************/

extern uint8_t CT[2];		// ������
extern uint8_t SN[4];		// ����
extern uint8_t READ_RFID[16];		// ���RFID 
extern uint8_t WRITE_RFID[16];	//(Ĭ��"0123456789ABCDEF")
extern uint8_t KEY_A[6];   // A��Կ(Ĭ�� 0xff,0xff,0xff,0xff,0xff,0xff)
extern uint8_t KEY_B[6];	// B��Կ(Ĭ�� 0xff,0xff,0xff,0xff,0xff,0xff)
extern uint8_t ADDR_Str[14];	//Ĭ��"RFID_ADDR:01\n"

/******************************************************************************************/


/************************************bkrc_voice.c��ȫ�ֱ���**********************************/

extern uint8_t uart6_data;			// USART6 ����������ʱ����(Ĭ��0)
extern uint8_t uart6_flag;			// USART6 ��������ʱ��(Ĭ��0)
extern uint8_t UART6_RxData[8];		// USART6 �����������ջ���
extern uint8_t voice_falg;		// ����ģ�鷵��״̬(Ĭ��0��0x01--��ʾ����һ֡�������,0x00--û���յ�)
extern uint8_t YY_Init[5];	//��������������־�ﲥ��ָ���ı���Ϣ����(Ĭ�� 0xFD, 0x00, 0x00, 0x01, 0x01)(֡ͷ�����������ֽڣ����������ֽڣ������֣�GBK��ʽ)
extern uint8_t Zigbee[8];           // Zigbee�������ݻ���(8Byte���������xx·վ)
extern uint8_t start_voice_dis[5];	//���Ѵ�(����� Ĭ�� 0xFA, 0xFA, 0xFA, 0xFA, 0xA1)
extern uint8_t bkrc_voice_Flag;           // SYN7318����ʶ������ID���(Ĭ��0)

/********************************************************************************************/


/**********************************data_base.c��ȫ�ֱ���**************************************/

extern uint8_t Principal_Tab[Principal_Length];	//���������ϴ�**********************ָ���ͷ �������С50��
extern uint8_t Follower_Tab[Follower_Length];	//�������䳵(�ӳ�)�����ϴ�ָ���ͷ�������С50��

/********************************************************************************************/


/********************************************roadway_check.c��ȫ�ֱ���************************/

extern uint8_t wheel_L_Flag, wheel_R_Flag;	//��ת��־λ����ת��־λ��Ĭ��0��
extern uint8_t wheel_Nav_Flag;	//������ת��־λ��Ĭ��0��
extern uint8_t Go_Flag, Back_Flag;	//ǰ����־λ�����˱�־λ��Ĭ��0��
extern uint8_t Track_Flag;	//Ѱ����־λ��Ĭ��0��
extern uint8_t Stop_Flag;	//ֹͣ��־λ��Ĭ��0��
extern uint16_t count;	//�෴��Ѱ�������õ��ļ�����Ĭ��0��
extern int LSpeed, RSpeed;	//���������ٶ� ���������ٶȣ�Ĭ��0��
extern int Car_Spend;	//���ٶ���ʱ����(��������ʱ������)��Ĭ��0��
extern uint16_t temp_MP;	//����ֵ��ʱ����(��������ʱ������)��Ĭ��0��
extern uint16_t temp_Nav;	//������תֵ��Ĭ��0��
extern uint32_t Wheel_flag;	//ת�����ֵ��Ĭ��0��
extern int16_t Roadway_cmp;	//��������ֵ

/*********************************************************************************************/


/************************************can_user.c��ȫ�ֱ���**************************************/

extern uint8_t Wifi_Rx_Buf[ WIFI_MAX_NUM ];	//WiFi������������(�ն˷��͹�����)(�����С200)
extern uint8_t Zigb_Rx_Buf[ ZIGB_RX_MAX ];	//ZigBee��������(��־��ش�������)(�����С200)
extern uint8_t Wifi_Rx_num ;	//WiFi�������������(���жϽ����˶����ֽ�)
extern uint8_t Wifi_Rx_flag ;  //WiFi������ɱ�־λ
extern uint8_t Zigbee_Rx_num ;	//ZigBee�������������(���жϽ����˶����ֽ�)
extern uint8_t Zigbee_Rx_flag ;  //ZigBee������ɱ�־λ
extern uint8_t Host_AGV_Return_Flag;	//���������ϴ���־λ��Ĭ��RESET��
extern uint8_t AGV_data_Falg;	//���شӳ����ݱ�־λ��Ĭ��RESET��
extern uint32_t canu_wifi_rxtime;	//ʱ��Ƭ��ѯWiFi������Ҫʱ�䣨Ĭ��0��
extern uint32_t canu_zibe_rxtime;	//ʱ��Ƭ��ѯZigBee������Ҫʱ�䣨Ĭ��0��

/*********************************************************************************************/


/****************************************yang.c��ȫ�ֱ���**************************************/
//����������־�﷢�Ϳ�������洢����
extern uint8_t Voice_TX_Arr[8];//Ĭ��(0x55, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBB)
//��������������־������ش������ڻ�������
extern uint8_t Voice_RX_DateArr[3];	//(Ĭ��0)
//��������������־������ش���ʱ�仺������
extern uint8_t Voice_RX_TimeArr[3];	//(Ĭ��0)
//��������������־������ش����������¶����ݻ�������
extern uint8_t Voice_RX_WeatherTempertureArr[2];	//(Ĭ��0)
//����������־��3ѡ1����洢����
extern uint8_t Voice_TX_3choose1_Arr[4];	//Ĭ��(0xFD,0x00,0x01,0x00)
//բ�ű�־�﷢�Ϳ�������洢����
extern uint8_t Gate_TX_Arr[8];	//Ĭ��(0x55,0x03,0x00,0x00,0x00,0x00,0x00,0xBB)
//LED��ʾ��־�﷢�Ϳ�������洢����
extern uint8_t LED_TX_Arr[8];	//Ĭ��(0x55,0x04,0x00,0x00,0x00,0x00,0x00,0xBB)

//��־��״̬��־λ
extern uint8_t LandMark_Flag; //��Ĭ��0��
//��־�ﳬʱ����
extern uint16_t LandMark_TimeA; //��Ĭ��0��
extern uint16_t LandMark_TimeB;	//��Ĭ��0��

//���峵���־�﷢�Ϳ�������洢����
extern uint8_t Garage_TX_Arr[8];	//Ĭ��(0x55,0x00,0x00,0x00,0x00,0x00,0x00,0xBB)
//���峵���־�����״̬ 0---��û���� 1---ǰ�ഥ�����û���� 2---ǰ��û������ഥ�� 3---������
extern uint8_t Garage_Infrared_Flag; //��Ĭ��-1��
//���̨��������־�﷢�Ϳ�������洢����
extern uint8_t Beacon_TX_Arr[8];	//Ĭ��(0x55,0x07,0x00,0x00,0x00,0x00,0x00,0xBB)
//���̨��������־��ش�����������
extern uint8_t Beacon_Coordinate; //��Ĭ��0��
//���̨��������־�﷢�Ϳ�������洢����(����)
extern uint8_t Beacon_TX_Arr_Infrared[6];
//TFT��ʾ��־�﷢�Ϳ�������洢����
extern uint8_t TFT_TX_Arr[8];	//Ĭ��(0x55,0x00,0x00,0x00,0x00,0x00,0x00,0xBB)
//���߳���־�﷢�Ϳ�������洢����
extern uint8_t WirelessCharging_TX_Arr[8];	//ģʽ(0x55,0x0A,0x01,0x00,0x00,0x00,0x00,0xBB)
//ETC��־�﷢�Ϳ�������洢����
extern uint8_t ETC_TX_Arr[8];	//Ĭ��(0x55,0x0C,0x00,0x00,0x00,0x00,0x00,0xBB)
//���ܽ�ͨ�Ʊ�־�﷢�Ϳ�������洢����
extern uint8_t SmartTrafficLights_TX_Arr[8];	//Ĭ��(0x55,0x00,0x00,0x00,0x00,0x00,0x00,0xBB)
//������α�־�﷢�Ϳ�������洢����
extern uint8_t SpecialTerrain_TX_Arr[8];	//Ĭ��(0x55, 0x10, 0x10, 0x01, 0x00, 0x00, 0x00, 0xBB)
//����·�Ʊ�־�﷢�Ϳ�������洢����(����)
extern uint8_t StreetLight_TX_Arr[4];	//Ĭ��(0x00,0xFF,0x00,0x00)
//��������·�Ʊ�־��ÿ���Ĺ��ֵ
extern uint16_t Light_temp[4];	//(Ĭ��0)
//������ʾ��־�﷢�Ϳ�������洢����(����)
extern uint8_t ThreeD_TX_Arr[6];	//Ĭ��(0xFF,0x00,0x00,0x00,0x00,0x00)
//���͸��ӳ�������洢����
extern uint8_t AGV_TX_Arr[8];	//Ĭ��(0x55,0x02,0x00,0x00,0x00,0x00,0x00,0xBB)

extern uint16_t Yang_Go_Speed;	//Ѱ�����������ߵ��ٶ�(Ĭ��40)
extern uint16_t Yang_LR_Speed;	//����ת�ٶ�(�Ƕ�)(Ĭ��90)
extern uint16_t Yang_Removing;	//ǰ��ʱ�ľ���(Ĭ��180)
extern uint16_t L_SPEED;	//��ת�ٶ�
extern uint16_t L_TIME;	//��תʱ��
extern uint16_t R_SPEED;	//��ת�ٶ�
extern uint16_t R_TIME;	//��תʱ��
extern uint16_t Go_TIME;	//ǰ��320��ʱ�ľ���
extern uint16_t Yang_wheel_Time; // ȫ��ת45��ʱ��
extern uint16_t Yang_Delay_big;	//����ʱ
extern uint16_t Yang_Delay_small;	//С��ʱ

//����ȫ�Զ�״̬λ
extern uint8_t Automatic_Falg;	//Ĭ��(0x01)
extern uint16_t Yang_wheel_Time; // ȫ��ת45��ʱ��(Ĭ��330)

//����Ѱ����־λ�Զ���(Ѱ�������õ�)
extern uint8_t Yang_Track_Flag;	//(Ĭ��0)
//����������������־λ
extern uint8_t Back_Car_Flag;//(Ĭ��0)

//����APPʶ��ͨ����ɫ��ֵ(1--��ɫ 2--��ɫ 3--��ɫ)
extern uint8_t App_RX_TrafficColor;
//����ת�ַ������ŵ�(���泵��)
extern char App_RX_TFTCarCard[7];	//Ĭ��("\0")
//����APPʶ��TFT��־�ﳵ�ƵĽ��(�����ָ����0x00��ʾʶ�������ǳ���ͼƬ �����0x01����ʶ��һ������)
extern char App_RX_TFTarr[6];
//�����ȡRFID��ȡ������
extern uint8_t RFID_Number_Buf[16];



/*********************************************************************************************/


/************************************���õ�ȫ�ֺ���********************************************/

extern void Infrared_Send(uint8_t *s, int n);	//�����������ͺ���
extern void Set_tba_Beep(uint8_t swch);	//����������(������SET--�� RESET--��)
extern void Set_tba_WheelLED(uint8_t LorR, uint8_t swch);	//ת��ƿ���(������SET--�� RESET--��)
extern void Ultrasonic_Ranging(void);	//���������(���������ⲿ�жϽ���)
extern uint16_t Get_Bh_Value(void);	//��ȡ��ȴ�������ֵ
extern void RC522(uint8_t card_addr,uint8_t mode);	//RFID��д���ƺ���
extern char PcdRequest(unsigned char req_code,unsigned char *pTagType);	//ѰRFID��
extern char PcdAuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr);	//��֤��Ƭ����
extern char PcdRead(unsigned char addr,unsigned char *pData);	//��ȡM1��һ������
extern char PcdWrite(unsigned char addr,unsigned char *pData);	//д���ݵ�M1��һ��
extern void USART6_Send_Byte(uint8_t byte);	//USART6����1���ֽ�(�����ڷ��Ͳ������ID)
extern uint8_t Voice_Drive(void);	//����ʶ��ش������������(�ɶԷ���ֵ���в���)
extern void Roadway_mp_syn(void);	//����ͬ��
extern uint16_t Mean_Filter(uint16_t m);	//��ֵ������
extern uint16_t Smoothing_Filtering(uint16_t value);	//ƽ���˲�
extern void Send_UpMotor( int x1, int x2);	//���͵���ٶ�
extern void Send_ZigbeeData_To_Fifo( uint8_t *p ,uint8_t len);	//�����ݷ�����ZigBeeģ�飬����ZigBee����ʾ
extern void Send_WifiData_To_Fifo( uint8_t *p ,uint8_t len);	//�����ݷ�����WIFIģ�飬����WiFi����ʾ
extern void Send_InfoData_To_Fifo(char *p ,uint8_t len);	//��Debug����ʾ��������ʾ����
extern void Can_WifiRx_Check(void);	//Wifi���յ����ݽ���
extern void Normal_data(void);	//wifi���յ�8Byte�������


/*===============���ߺ���==================*/
extern uint16_t  Get_Host_UpTrack( uint8_t mode);	//��ȡѰ��������(����������ǰ7����8�����е�)
extern void Car_Go(uint8_t speed, uint16_t temp);   // ����ǰ�� �������ٶ�/����
extern void Car_Back(uint8_t speed, uint16_t temp); // �������� �������ٶ�/����
extern void Car_Left(uint8_t speed);       // ������ת �������ٶ�
extern void Car_Right(uint8_t speed);       // ������ת �������ٶ�
extern void Car_Track(uint8_t speed);   // ����ѭ�� �������ٶ�
extern void Car_L45(int8_t speed, uint16_t times);		// ����ת ��������תʱ�� 
extern void Car_R45(int8_t speed, uint16_t tims);		// ����ת ��������תʱ��
/**********************************************************************************************/

#endif




