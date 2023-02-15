#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include "stm32f1xx_hal.h"
#include "Control_Config.h"
#include "AT8236.h"
#include "MT6816.h"
#include "Speed_Tracker.h"
#include "Move_Reconstruct.h"
#include "Location_Tracker.h"
#include "Location_Interp.h"
#include "Current_Tracker.h"

/****************************************  ֱ�ӿ���(��������)  ****************************************/
/****************************************  ֱ�ӿ���(��������)  ****************************************/
void Control_Cur_To_Electric(int16_t current);		//�������

/****************************************  PID����(�ٶȿ���)  ****************************************/
/****************************************  PID����(�ٶȿ���)  ****************************************/
typedef struct{
	//����
	#define De_PID_KP	5	//5		//Ĭ��KP
	#define De_PID_KI 30	//30	//Ĭ��KI
	#define De_PID_KD	0		//Ĭ��KD
	uint8_t		valid_kp, valid_ki, valid_kd;	//������Ч��־
	int32_t	kp, ki, kd;		//����
	//���Ʋ���
	int32_t		v_error, v_error_last;	//����¼
	int32_t		op, oi, od;							//���
	int32_t		i_mut, i_dec;						//С���ִ���
	int32_t		out;										//���
}Control_PID_Typedef;
extern Control_PID_Typedef pid;

//��������
void Control_PID_SetKP(uint16_t _k);		//KP��������
void Control_PID_SetKI(uint16_t _k);		//KI��������
void Control_PID_SetKD(uint16_t _k);		//KD��������
void Control_PID_Set_Default(void);			//PID�����ָ�
//��ʼ��
void Control_PID_Init(void);
void Control_PID_To_Electric(int32_t _speed);

/****************************************  DCE����(λ�ÿ���)  ****************************************/
/****************************************  DCE����(λ�ÿ���)  ****************************************/
extern int32_t goal_debug;
extern int32_t goal_c_debug;
extern int32_t goal_v_debug;
typedef struct{
	//����
	#define De_DCE_KP	200	//200		//Ĭ��KP
	#define De_DCE_KI 80	//80		//Ĭ��KI
	#define De_DCE_KV	300	//300		//Ĭ��KIV
	#define De_DCE_KD	250	//250		//Ĭ��KD
	uint8_t		valid_kp, valid_ki, valid_kv, valid_kd;	//������Ч��־
	int32_t	kp, ki, kv, kd;		//����
	//���Ʋ���(��������)
	int32_t		p_error, v_error;		//����¼
	int32_t last_error;
	int32_t		op, oi, od;					//���
	int32_t		i_mut, i_dec;				//С���ִ���
	int32_t		out;								//���
}Control_DCE_Typedef;
extern Control_DCE_Typedef dce;

//��������
void Control_DCE_SetKP(uint16_t _k);		//KP��������
void Control_DCE_SetKI(uint16_t _k);		//KI��������
void Control_DCE_SetKV(uint16_t _k);		//KV��������
void Control_DCE_SetKD(uint16_t _k);		//KD��������
void Control_DCE_Set_Default(void);			//DCE�����ָ�
//��ʼ��
void Control_DCE_Init(void);
void Control_DCE_To_Electric(int32_t _location, int32_t _speed);

/****************************************  Motor_Contro_Debug  ****************************************/
/****************************************  Motor_Contro_Debug  ****************************************/
/**
  * Motor_Control��ṹ�嶨��
**/
typedef struct{
	//��������
	int32_t		mut;		//����λ��
	int32_t		dec;		//����λ��
	int32_t		speed;	//�����ٶ�
}Motor_Control_Debug_Typedef;
extern Motor_Control_Debug_Typedef mc_debug;

//����
void Motor_MultiDebug_Location(void);	//�๦�ܵ���λ��
void Motor_MultiDebug_Speed(void);		//�๦�ܵ����ٶ�

/****************************************  Motor_Control  ****************************************/
/****************************************  Motor_Control  ****************************************/
/**
  * ������״̬
**/
typedef enum{
	Control_State_Stop				= 0x00,	//ֹͣ
	Control_State_Finish			= 0x01,	//�������
	Control_State_Running			= 0x02,	//����ִ����
	Control_State_Overload		= 0x03,	//����
	Control_State_Stall				= 0x04,	//��ת
}Motor_State;

/**
  * ģʽ
**/
typedef enum{
	/*****************************************************�ⲿ������***********�źŷ�����(�ɿص��˶�����)**����״̬(���Ű汾)*****/
	//����
	Motor_Mode_Debug_Location		= 0x00,	//����λ��				NULL									�๦�ܵ����ź�Դ						���԰汾��������
	Motor_Mode_Debug_Speed			= 0x01,	//�����ٶ�				NULL									�๦�ܵ����ź�Դ						���԰汾��������
	//ֹͣ
	Control_Mode_Stop						= 0x10,	//ֹͣ						NULL									NULL												NULL
	//DIG(CAN/RS485)
	Motor_Mode_Digital_Location	= 0x20,	//DIGλ��					Ŀ��λ��							λ�ø�����(�ٶ�,˫���ٶ�)		Դ�뿪��(20191101)
	Motor_Mode_Digital_Speed		= 0x21,	//DIG�ٶ�					Ŀ���ٶ�							�ٶȸ�����(˫���ٶ�)				Դ�뿪��(20191101)
	Motor_Mode_Digital_Current	= 0x22,	//DIG����					Ŀ�����							����������(˫���ݶ�)				Դ�뿪��(20191101)
	Motor_Mode_Digital_Track		= 0x23,	//DIG�켣					��ָ��								�˶��ع���(�˶���Ѱ)				Դ�뿪��(20191101)
	//MoreIO(PWM/PUL)
	Motor_Mode_PWM_Location			= 0x30,	//PWMλ��_���		Ŀ��λ��							λ�ø�����(�ٶ�,˫���ٶ�)		Դ�뿪��(20191101)
	Motor_Mode_PWM_Speed				= 0x31,	//PWM�ٶ�_���ٻ�	Ŀ���ٶ�							�ٶȸ�����(˫���ٶ�)				Դ�뿪��(20191101)
	Motor_Mode_PWM_Current			= 0x32,	//PWM����_ֱ����	Ŀ�����							����������(˫���ݶ�)				Դ�뿪��(20191101)
	Motor_Mode_PULSE_Location		= 0x33,	//PULSEλ��				Ŀ��λ��							λ�ò岹��(��)							Դ�뿪��(20200803)
	//Action
	Motor_Mode_QCode						= 0x40,	//Qָ��						NULL									����												�ڲ�����()
	//Demos
	Motor_Mode_Music						= 0x50,	//����						NULL									NULL												�ڲ�����()
}Motor_Mode;

/**
  * Motor_Control��ṹ�嶨��
**/
typedef struct{
	//����(ģʽ)
	#define			De_Motor_Mode		Motor_Mode_PWM_Location //Motor_Mode_PULSE_Location	//Ĭ������
	uint8_t				valid_mode;		//��Ч��־
	Motor_Mode	mode_order;		//���ģʽ_��ָ���
	//����(��ת)
	#define			De_Motor_Stall	1		//Ĭ�϶�ת��������
	uint8_t				valid_stall_switch;			//��ת����������Ч��־
	uint8_t				stall_switch;						//��ת��������
	//ģʽ
	Motor_Mode	mode_run;			//���ģʽ_�����е�
	//��ȡ
	int32_t		real_lap_location;				//��ȡ��Ȧλ��
	int32_t		real_lap_location_last;		//��ȡ��Ȧλ��
	int32_t		real_location;						//��ȡλ��
	int32_t		real_location_last;				//��ȡλ��
	//����
	int32_t		est_speed_mut;						//�����ٶȱ�ֵ(�Ŵ�n��)(mut�л�������,�����о�����ʧ)
	int32_t		est_speed;								//�����ٶ�
	int32_t		est_lead_location;				//����λ�ó�ǰλ��
	int32_t		est_lead_location_debug;	//����λ�ó�ǰλ��
	int32_t		est_location;							//����λ��
	int32_t		est_error;								//����λ�����
	//ӲĿ��
	int32_t		goal_location;	//Ŀ��λ��(���ź�����)
	int32_t		goal_speed;			//Ŀ���ٶ�(���ź�����)
	int16_t		goal_current;		//Ŀ�����(���ź�����)
	uint8_t			goal_disable;		//Ŀ��ʧ��(���ź�����)
	uint8_t			goal_brake;			//Ŀ��ɲ��(���ź�����)
	//��Ŀ��
	int32_t		soft_location;	//��λ��(�� ������/�ع���/�岹��/Ӳ���� �õ�)
	int32_t		soft_speed;			//���ٶ�(�� ������/�ع���/�岹��/Ӳ���� �õ�)
	int16_t		soft_current;		//�����(�� ������/�ع���/�岹��/Ӳ���� �õ�)
	uint8_t			soft_disable;		//��ʧ��
	uint8_t			soft_brake;			//��ɲ��
	uint8_t			soft_new_curve;	//����Ŀ������
	//���
	int32_t		foc_location;		//FOCʸ��λ��
	int32_t		foc_current;		//FOCʸ����С
	//��תʶ��
	uint32_t	stall_time_us;	//��ת��ʱ��
	uint8_t			stall_flag;			//��ת��־
	//����ʶ��
	uint32_t	overload_time_us;	//���ؼ�ʱ��
	uint8_t			overload_flag;		//���ر�־
	//״̬
	Motor_State		state;			//ͳһ�ĵ��״̬
}Motor_Control_Typedef;
extern Motor_Control_Typedef motor_control;

//��������
void Motor_Control_SetMotorMode(Motor_Mode _mode);	//����ģʽ
void Motor_Control_SetStallSwitch(uint8_t _switch);		//��ת��������
void Motor_Control_SetDefault(void);								//����ģʽ�����ָ�

//����д��
void Motor_Control_Write_Goal_Location(int32_t value);//д��Ŀ��λ��
void Motor_Control_Write_Goal_Speed(int32_t value);		//д��Ŀ���ٶ�
void Motor_Control_Write_Goal_Current(int16_t value);	//д��Ŀ�����
void Motor_Control_Write_Goal_Disable(uint16_t value);//д��Ŀ��ʧ��
void Motor_Control_Write_Goal_Brake(uint16_t value);	//д��Ŀ��ɲ��

//����ִ��
void Motor_Control_Init(void);											//������Ƴ�ʼ��
void Motor_Control_Callback(void);									//����������ص�
void Motor_Control_Clear_Integral(void);						//�������
void Motor_Control_Clear_Stall(void);								//�����ת����
int32_t Motor_Control_AdvanceCompen(int32_t _speed);//��ǰ�ǲ���

#endif
