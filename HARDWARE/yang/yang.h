#ifndef __YANG_H
#define __YANG_H
#include "allhead.h"


void Yang_Voice_Assign(uint8_t ID);	//����ָ����������(xx·վ)
void Yang_Voice_Random(void);	//���������������(xx·վ)
void Yang_Voice_SetRtcDate(uint8_t year,uint8_t month,uint8_t day);	//����RTC��ʼ����(������)
void Yang_Voice_SetRtcTime(uint8_t hour,uint8_t min,uint8_t sec);	//����RTC��ʼʱ��(ʱ����)
void Yang_Voice_InquireRtcDate(void);	//��ѯRTC��ǰ����(������)���洢(�лش�)
void Yang_Voice_InquireRtcTime(void);	//��ѯRTC��ǰʱ��(ʱ����)���洢(�лش�)
void Yang_Voice_SetWeatherTemperture(uint8_t weather,uint8_t temperture);	//���������������¶�����
void Yang_Voice_InquireWeatherTemperture(void);	//����ش��������¶�����(�лش�)
void Yang_Voice_Customize(char *p);	//�����Զ���ϳ��ı�����
void Yang_Voice_3Choose1(uint8_t num);	//ֹͣ/��ͣ/�ָ��ϳ�����
void Yang_Gate_OffOrOn(uint8_t swch);	//����բ��/�ر�բ��
void Yang_Gate_CarCard(char* str);	//���ͳ������ݵ�բ�ű�־��
uint8_t Yang_InquireGate_State(void);	//����ش�բ��״̬���жϻش�ֵ
void Yang_LED_ShowData(uint8_t one,uint8_t two,uint8_t three,uint8_t choose);	//LED��ʾ��־������ܵ�һ����ʾ����/�ڶ�����ʾ����
void Yang_LED_Timing(uint8_t choose);	//LED��ʾ��־���1����ʾ��ʱģʽ
uint16_t Yang_LED_Removing(uint16_t pdis);	//LED��ʾ��־���2����ʾ����
void Yang_GarageWent(uint8_t device,uint8_t floor);	//�������峵��A/B����ڼ���
uint8_t Yang_InquireGarage_Floor(uint8_t device);	//����ش�����λ�ڵڼ���
void Yang_Garage_Infrared(uint8_t device);	//����ش��������״̬
#if 0
void Yang_Beacon_Open(uint8_t* temp);	//����̨���������Ϳ�����(ZigBee����)
void Yang_Beacon_InquireCoordinate(void);	//����ش������Ԯ����(ZigBee����)
#endif
void Yang_Beacon_Open2(uint8_t* temp);	//����̨���������Ϳ�����(���ⷢ��)
void Yang_TFT_Assign(uint8_t device,uint8_t ID);	//����TFT��־��A/B��ʾָ��ͼƬ
void Yang_TFT_TurnPage(uint8_t device,uint8_t choose);	//����TFT��־��A/BͼƬ�ֶ���ҳ/�Զ���ҳ
void Yang_TFT_CarCard(uint8_t device,char* str);	//����TFT��־��A/B��ʾ����ģʽ
void Yang_TFT_Timing(uint8_t device,uint8_t choose);	//����TFT��־��A/B��ʱ��ʾģʽ
void Yang_TFT_HexShow(uint8_t device,uint8_t* temp);	//����TFT��־��A/B ��λHEX��ʽ������ʾ
void Yang_TFT_Removing(uint8_t device,uint16_t pdis);	//����TFT��־��A/B��ʾ����ģʽ
void Yang_TFT_TrafficSigns(uint8_t device,uint8_t ID);	//����TFT��־��A/B��ͨ��־��ʾģʽ
void Yang_WirelessCharging_OffOrOn(uint8_t swch);	//�������߳���־�￪��
void Yang_ETC_Inquire(void);	//��ѯETC�Ƿ��ѿ���
void Yang_Traffic_RecognitionPatterns(uint8_t device);	//����ʶ��ģʽ
void Yang_Traffic_Color(uint8_t device,uint8_t ID);	//ʶ��������ȷ��
void Yang_SpecialTerrain_check(void);	//������α�־���ѯ����ͨ��״̬
void Yang_SpecialTerrain_1(uint8_t speed);

void StreetLight_adjust(uint8_t ID);	//����·�ƵĹ�Դ��λ(���ⷢ��)
void StreetLight_Test(uint8_t num);	//���ݹ���жϵ�ǰ��λ�ٸ�����Ҫ�����涨��λ(���ⷢ��)
void Yang_ThreeD_CarCardAndxy(char* str,char x,char y);	//������ʾ��־����ʾ������Ϣ������(���ⷢ��)
void Yang_ThreeD_Removing(uint16_t pdis);	//������ʾ��־����ʾ����ģʽ(���ⷢ��)
void Yang_ThreeD_GraphShow(uint8_t ID);	//������ʾ��־��ͼ����Ϣ��ʾģʽ(���ⷢ��)
void Yang_ThreeD_Color(uint8_t ID);	//������ʾ��־����ɫ��Ϣ��ʾģʽ(���ⷢ��)
void Yang_ThreeD_Warning(uint8_t ID);	//������ʾ��־�ｻͨ��ʾ����Ϣ��ʾģʽ(���ⷢ��)
void Yang_ThreeD_Traffic(uint8_t ID);	//������ʾ��־�ｻͨ��־��Ϣ��ʾģʽ(���ⷢ��)
void Yang_ThreeD_Default(void);	//������ʾ��־����ʾĬ����Ϣ(���ⷢ��)
void Yang_ThreeD_TextColor(uint8_t R,uint8_t G,uint8_t B);	//������ʾ��־������������ʾ��ɫ(���ⷢ��)
void Yang_ThreeD_Other(uint8_t swch);	//������ʾ��־����������
void Yang_ThreeD_CustomizeText(void);	//������ʾ��־���Զ����ı��ۼ�
void Yang_AGV_LED(void);	//����

void Yang_Track_duoy_RFID(uint8_t speed);   //Ѱ��Ѱ��
void Yang_Track_duo_RFID(uint8_t speed,uint8_t card);//��������   ������ѭ���ٶȣ���Ҫ��ȡ�����ݿ�
void Yang_Track_xie_RFID(uint8_t speed,uint8_t card);//д������   ������ѭ���ٶȣ���Ҫдȡ�����ݿ�

void Yang_Automatic_1(void);	//С��ȫ�Զ�����(ģ������)
void Yang_Car_TrackAndGo(void);	//Ѱ����ǰ��һ����
void Yang_Left_Time(uint8_t speed,uint16_t time);	// ������ת ����1���ٶ� ����2��ʱ��(������)
void Yang_Right_Time(uint8_t speed,uint16_t time);	// ������ת ����1���ٶ� ����2��ʱ��(������)
void Yang_Track_Correct_White(uint8_t gd);	//Ѱ��������ͣ��ִ�к���
void Yang_Car_Track_White(uint8_t speed);	//Ѱ��������ͣ����
void Yang_CarThread_Go(uint8_t speed, uint16_t time,uint8_t swch);	// ����ǰ�� ����1���ٶ�  ����2��ʱ��(������)д����
void Yang_Car_L45(int8_t speed, uint16_t times);	//����ת45�� ����1����ת�ٶ� ����2����תʱ��
void Yang_Car_R45(int8_t speed, uint16_t tims);	//����ת45�� ����1����ת�ٶ� ����2����תʱ��
void Yang_Car_Back_Infrared(uint8_t speed,uint16_t time);	//����������������ͣ����(Ҳ���Ե����˺���)
void Yang_Car_Garage_return(void);	//�������ִ�к���

uint8_t Yang_CheckSum_1(uint8_t main_command, uint8_t sub_command1, uint8_t sub_command2, uint8_t sub_command3);	//����У���(�����ں�---"��ָ��""��ָ��1""��ָ��2""��ָ��3" 8Byteͨ��Э��)
void Yang_ArrInit(uint8_t* arr);	//��ԭ����
void Yang_bubble_sort(uint16_t* arr,uint8_t len);	//ð������
void Timer2_Init(uint16_t arr,uint16_t psc);	//��ʱ����ʱ
void Yang_ArrToStr(char* arr,char* str,unsigned char size);	//����ת�ַ���(����������)
void Yang_DrawNumber(uint8_t* Read_buf);	//��һ����������ȡ����(RFID��)
uint8_t Yang_NumberTo16(uint8_t* temp);	//��ĳ��������ϳ�һ��0xXX��ʮ������(����ʮ����ֱ��ת��ʮ������)

#endif






