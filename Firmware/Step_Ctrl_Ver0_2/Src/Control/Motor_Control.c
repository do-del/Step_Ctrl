#include "Motor_Control.h"

//PID����
Control_PID_Typedef pid;
//DCE����
Control_DCE_Typedef dce;
Motor_Control_Debug_Typedef		mc_debug;	//���Ƶ���
Motor_Control_Typedef 				motor_control;				//�������ṹ

/****************************************  �������(��������)  ****************************************/
/****************************************  �������(��������)  ****************************************/
/**
  * @brief  �������
  * @param  current: �������
  * @retval NULL
**/
void Control_Cur_To_Electric(int16_t current)
{
	//���FOC����
	motor_control.foc_current = current;
	//���FOCλ��
	if(motor_control.foc_current > 0)				motor_control.foc_location = motor_control.est_location + Move_Divide_NUM;
	else if(motor_control.foc_current < 0)	motor_control.foc_location = motor_control.est_location - Move_Divide_NUM;
	else																		motor_control.foc_location = motor_control.est_location;
	//�����������
	HW_Elec_SetDivideElec(motor_control.foc_location, motor_control.foc_current);
	//CurrentControl_Out_FeedTrack(motor_control.foc_location, motor_control.foc_current, 0, 1);
}

/****************************************  PID����(�ٶȿ���)  ****************************************/
/****************************************  PID����(�ٶȿ���)  ****************************************/
Control_PID_Typedef pid;

/**
  * @brief  ��������
  * @param  _k
  * @retval NULL
**/
void Control_PID_SetKP(uint16_t _k)
{
	if(_k <= 1024){		pid.kp = _k;		pid.valid_kp = 1;		}
	else{															pid.valid_kp = 0;		}
}

/**
  * @brief  ��������
  * @param  _k
  * @retval NULL
**/
void Control_PID_SetKI(uint16_t _k)
{
	if(_k <= 1024){		pid.ki = _k;		pid.valid_ki = 1;		}
	else{															pid.valid_ki = 0;		}
}

/**
  * @brief  ��������
  * @param  _k
  * @retval NULL
**/
void Control_PID_SetKD(uint16_t _k)
{
	if(_k <= 1024){		pid.kd = _k;		pid.valid_kd = 1;		}
	else{															pid.valid_kd = 0;		}
}

/**
  * @brief  PID�����ָ�
  * @param  NULL
  * @retval NULL
**/
void Control_PID_Set_Default(void)
{
	Control_PID_SetKP(De_PID_KP);
	Control_PID_SetKI(De_PID_KI);
	Control_PID_SetKD(De_PID_KD);
}

/**
  * @brief  ������PID��ʼ��
  * @param  NULL
  * @retval NULL
**/
void Control_PID_Init(void)
{
	//ǰ��������Чʱ,����Ĭ������
	if(!pid.valid_kp)				{	Control_PID_SetKP(De_PID_KP);		}
	if(!pid.valid_ki)				{	Control_PID_SetKI(De_PID_KI);		}
	if(!pid.valid_kd)				{	Control_PID_SetKD(De_PID_KD);		}
	
	//���Ʋ���
	pid.v_error = 0;	pid.v_error_last = 0;
	pid.op = 0;				pid.oi = 0;			pid.od = 0;	
	pid.i_mut = 0;		pid.i_dec = 0;
	pid.out = 0;
}

/**
  * @brief  PID��������
  * @param  _speed    �����ٶ�
  * @retval NULL
**/
void Control_PID_To_Electric(int32_t _speed)
{
	//���
	pid.v_error_last = pid.v_error;
	pid.v_error = _speed - motor_control.est_speed;	//�ٶ����
	if(pid.v_error > ( 1024 * 1024))	pid.v_error = ( 1024 * 1024);
	if(pid.v_error < (-1024 * 1024))	pid.v_error = (-1024 * 1024);
	//op���
	pid.op = ((pid.kp) * (pid.v_error));
	//oi���
	pid.i_mut += ((pid.ki) * (pid.v_error));
	pid.i_dec  = (pid.i_mut >> 10);
	pid.i_mut -= (pid.i_dec << 10);
	pid.oi    += (pid.i_dec);
	if(pid.oi >      (  Current_Rated_Current << 10 ))	pid.oi = (  Current_Rated_Current << 10 );	//����Ϊ����� * 1024
	else if(pid.oi < (-(Current_Rated_Current << 10)))	pid.oi = (-(Current_Rated_Current << 10));	//����Ϊ����� * 1024
	//od���
	pid.od = (pid.kd) * (pid.v_error - pid.v_error_last);
	//�ۺ��������
	pid.out = (pid.op + pid.oi + pid.od) >> 10;
	if(pid.out > 			Current_Rated_Current)		pid.out =  Current_Rated_Current;
	else if(pid.out < -Current_Rated_Current)		pid.out = -Current_Rated_Current;
	
	//���FOC����
	motor_control.foc_current = pid.out;
	//���FOCλ��
	if(motor_control.foc_current > 0)				motor_control.foc_location = motor_control.est_location + Move_Divide_NUM;
	else if(motor_control.foc_current < 0)	motor_control.foc_location = motor_control.est_location - Move_Divide_NUM;
	else																		motor_control.foc_location = motor_control.est_location;
	//�����������
	HW_Elec_SetDivideElec(motor_control.foc_location, motor_control.foc_current);
	//CurrentControl_Out_FeedTrack(motor_control.foc_location, motor_control.foc_current, 0, 1);
}

/****************************************  DCE������(λ�ÿ���)  ****************************************/
/****************************************  DCE������(λ�ÿ���)  ****************************************/

/**
  * @brief  ��������
  * @param  _k
  * @retval NULL
**/
void Control_DCE_SetKP(uint16_t _k)
{
	if(_k <= 1024){		dce.kp = _k;		dce.valid_kp = 1;		}
	else{															dce.valid_kp = 0;		}
}

/**
  * @brief  ��������
  * @param  _k
  * @retval NULL
**/
void Control_DCE_SetKI(uint16_t _k)
{
	if(_k <= 1024){		dce.ki = _k;		dce.valid_ki = 1;		}
	else{															dce.valid_ki = 0;		}
}

/**
  * @brief  ��������
  * @param  _k
  * @retval NULL
**/
void Control_DCE_SetKV(uint16_t _k)
{
	if(_k <= 1024){		dce.kv = _k;		dce.valid_kv = 1;		}
	else{															dce.valid_kv = 0;		}
}

/**
  * @brief  ��������
  * @param  _k
  * @retval NULL
**/
void Control_DCE_SetKD(uint16_t _k)
{
	if(_k <= 1024){		dce.kd = _k;		dce.valid_kd = 1;		}
	else{															dce.valid_kd = 0;		}
}

/**
  * @brief  DCE�����ָ�
  * @param  NULL
  * @retval NULL
**/
void Control_DCE_Set_Default(void)
{
	Control_DCE_SetKP(De_DCE_KP);
	Control_DCE_SetKI(De_DCE_KI);
	Control_DCE_SetKV(De_DCE_KV);
	Control_DCE_SetKD(De_DCE_KD);
}

/**
  * @brief  ������DCE��ʼ��
  * @param  NULL
  * @retval NULL
**/
void Control_DCE_Init(void)
{
	//ǰ��������Чʱ,����Ĭ������
	if(!dce.valid_kp)				{	Control_DCE_SetKP(De_DCE_KP);		}
	if(!dce.valid_ki)				{	Control_DCE_SetKI(De_DCE_KI);		}
	if(!dce.valid_kv)				{	Control_DCE_SetKV(De_DCE_KV);		}
	if(!dce.valid_kd)				{	Control_DCE_SetKD(De_DCE_KD);		}
	
	//���Ʋ���(��������)
	dce.p_error = 0;	dce.v_error = 0;
	dce.op = 0;				dce.oi = 0;			dce.od = 0;	
	dce.i_mut = 0;		dce.i_dec = 0;
	dce.out = 0;
}

/**
  * @brief  DCE��������
  * @param  _location ����λ��
  * @param  _speed    �����ٶ�
  * @retval NULL
**/
void Control_DCE_To_Electric(int32_t _location, int32_t _speed)
{
	//���
	dce.p_error = _location - motor_control.est_location;
	dce.v_error = (_speed - motor_control.est_speed) >> 7;	//�ٶ������С��1/128
	if(dce.p_error > ( 3200))	dce.p_error = ( 3200);				//����λ�������1/16Ȧ��(51200/16)
	if(dce.p_error < (-3200))	dce.p_error = (-3200);
	if(dce.v_error > ( 4000))	dce.v_error = ( 4000);				//�����ٶ������10r/s��(51200*10/128)
	if(dce.v_error < (-4000))	dce.v_error = (-4000);
	//op�������
	dce.op     = ((dce.kp) * (dce.p_error));
	//oi�������
	dce.i_mut += ((dce.ki) * (dce.p_error));
	dce.i_mut += ((dce.kv) * (dce.v_error));
	dce.i_dec  = (dce.i_mut >> 7);
	dce.i_mut -= (dce.i_dec << 7);
	dce.oi    += (dce.i_dec);
	if(dce.oi >      (  Current_Rated_Current << 10 ))	dce.oi = (  Current_Rated_Current << 10 );	//����Ϊ����� * 1024
	else if(dce.oi < (-(Current_Rated_Current << 10)))	dce.oi = (-(Current_Rated_Current << 10));	//����Ϊ����� * 1024
	//od�������
	dce.od = ((dce.kd) * (dce.v_error));
	//�ۺ��������
	dce.out = (dce.op + dce.oi + dce.od) >> 10;
	if(dce.out > 			Current_Rated_Current)		dce.out =  Current_Rated_Current;
	else if(dce.out < -Current_Rated_Current)		dce.out = -Current_Rated_Current;

	//���FOC����
	motor_control.foc_current = dce.out;
	//���FOCλ��
	if(motor_control.foc_current > 0)			motor_control.foc_location = motor_control.est_location + Move_Divide_NUM;
	else if(motor_control.foc_current < 0)	motor_control.foc_location = motor_control.est_location - Move_Divide_NUM;
	else																		motor_control.foc_location = motor_control.est_location;
	//�����������
	HW_Elec_SetDivideElec(motor_control.foc_location, motor_control.foc_current);
	//CurrentControl_Out_FeedTrack(motor_control.foc_location, motor_control.foc_current, 0, 1);
}

/****************************************  Motor_Contro_Debug  ****************************************/
/****************************************  Motor_Contro_Debug  ****************************************/
Motor_Control_Debug_Typedef		mc_debug;	//���Ƶ���

/**
  * @brief  �๦�ܵ����ٶ�
  * @param  NULL
  * @retval NULL
**/
void Motor_MultiDebug_Location(void)
{
	mc_debug.mut += mc_debug.speed * 420;
	mc_debug.dec  = mc_debug.mut >> 23;
	mc_debug.mut -= mc_debug.dec << 23;
	
	motor_control.soft_location += mc_debug.dec;
	motor_control.soft_speed = mc_debug.speed;
}

/**
  * @brief  �๦�ܵ����ٶ�
  * @param  NULL
  * @retval NULL
**/
void Motor_MultiDebug_Speed(void)
{
	motor_control.soft_speed = mc_debug.speed;
}

/****************************************  Motor_Control  ****************************************/
/****************************************  Motor_Control  ****************************************/

/**
  * @brief  ���ģʽ����
  * @param  _mode
  * @retval NULL
**/
void Motor_Control_SetMotorMode(Motor_Mode _mode)
{
	motor_control.mode_order = _mode;
	motor_control.valid_mode = 1;
}

/**
  * @brief  ��ת��������
  * @param  _switch
  * @retval NULL
**/
void Motor_Control_SetStallSwitch(uint8_t _switch)
{
	motor_control.stall_switch = _switch;
	motor_control.valid_stall_switch = 1;
}

/**
  * @brief  ����ģʽ�����ָ�
  * @param  NULL
  * @retval NULL
**/
void Motor_Control_SetDefault(void)
{
	Motor_Control_SetMotorMode(De_Motor_Mode);
	Motor_Control_SetStallSwitch(De_Motor_Stall);
}

/**
  * @brief  д��Ŀ��λ��
  * @param  NULL
  * @retval NULL
**/
void Motor_Control_Write_Goal_Location(int32_t value)
{
	motor_control.goal_location = value;
}
	
/**
  * @brief  д��Ŀ���ٶ�
  * @param  NULL
  * @retval NULL
**/
void Motor_Control_Write_Goal_Speed(int32_t value)
{
	if((value >= -Move_Rated_Speed) && (value <= Move_Rated_Speed))
	{
		motor_control.goal_speed = value;
	}
}

/**
  * @brief  д��Ŀ�����
  * @param  NULL
  * @retval NULL
**/
void Motor_Control_Write_Goal_Current(int16_t value)
{
	if((value >= -Current_Rated_Current) && (value <= Current_Rated_Current))
	{
		motor_control.goal_current = value;
	}
}

/**
  * @brief  д��Ŀ��ʧ��
  * @param  NULL
  * @retval NULL
**/
void Motor_Control_Write_Goal_Disable(uint16_t value)
{
	motor_control.goal_disable = value;
}

/**
  * @brief  д��Ŀ��ɲ��
  * @param  NULL
  * @retval NULL
**/
void Motor_Control_Write_Goal_Brake(uint16_t value)
{
	motor_control.goal_brake = value;
}

/**
  * @brief  ������Ƴ�ʼ��
  * @param  NULL
  * @retval NULL
**/
void Motor_Control_Init(void)
{	
	//ǰ��������Чʱ,����Ĭ������
	if(!motor_control.valid_mode)					{		Motor_Control_SetMotorMode(De_Motor_Mode);		}
	if(!motor_control.valid_stall_switch)	{		Motor_Control_SetStallSwitch(De_Motor_Stall);	}

	//ģʽ
	motor_control.mode_run = Control_Mode_Stop;
	//��ȡ
	motor_control.real_lap_location = 0;
	motor_control.real_lap_location_last = 0;
	motor_control.real_location = 0;
	motor_control.real_location_last = 0;
	//����
	motor_control.est_speed_mut = 0;
	motor_control.est_speed = 0;
	motor_control.est_lead_location = 0;
	motor_control.est_location = 0;
	motor_control.est_error = 0;
	//ӲĿ��
	motor_control.goal_location = 0;
	motor_control.goal_speed = 0;
	motor_control.goal_current = 0;
	motor_control.goal_disable = 0;
	motor_control.goal_brake = 0;
	//��Ŀ��
	motor_control.soft_location = 0;
	motor_control.soft_speed = 0;
	motor_control.soft_current = 0;
	motor_control.soft_disable = 0;
	motor_control.soft_brake = 0;
	motor_control.soft_new_curve = 0;
	//���
	motor_control.foc_location = 0;
	motor_control.foc_current = 0;
	//��תʶ��
	motor_control.stall_time_us = 0;
	motor_control.stall_flag = 0;
	//����ʶ��
	motor_control.overload_time_us = 0;
	motor_control.overload_flag = 0;
	//״̬
	motor_control.state = Control_State_Stop;		
	
	/**********  �����㷨��ʼ��  **********/
	Control_PID_Init();
	Control_DCE_Init();
	
	/********** �켣�滮 **********/
	Location_Tracker_Init();	//λ�ø�������ʼ��
	Speed_Tracker_Init();			//�ٶȸ�������ʼ��
	Current_Tracker_Init();		//������������ʼ��
	Move_Reconstruct_Init();	//�˶��ع�����ʼ��
	Location_Interp_Init();		//λ�ò岹����ʼ��
}

/**
  * @brief  ����������ص�
  * @param  NULL
  * @retval NULL
**/
void Motor_Control_Callback(void)
{
	/************************************ �״ν�����ƻص� ************************************/
	/************************************ �״ν�����ƻص� ************************************/
	static uint8_t first_call = 1;
	if(first_call)
	{
		//��ȡ(Ϊ�˷��㽫XDrive������ֲ�����������,��λ�ö�ȡ��ʼ������ȫ�������ڴ˴�����)
		motor_control.real_lap_location				= mt6816.rectify_angle;
		motor_control.real_lap_location_last	= mt6816.rectify_angle;
		motor_control.real_location						= mt6816.rectify_angle;
		motor_control.real_location_last			= mt6816.rectify_angle;
		//��һ������ǿ���˳�
		first_call = 0;
		return;
	}
	
	/************************************ ���ݲɼ� ************************************/
	/************************************ ���ݲɼ� ************************************/
	int32_t		sub_data;		//���ڸ������
	//��ȡ��Ȧλ��
	motor_control.real_lap_location_last = motor_control.real_lap_location;
	motor_control.real_lap_location = mt6816.rectify_angle;
	//�ػ����
	sub_data = motor_control.real_lap_location - motor_control.real_lap_location_last;
	if(sub_data > (Move_Pulse_NUM >> 1))				sub_data -= Move_Pulse_NUM;
	else if(sub_data < -(Move_Pulse_NUM >> 1))	sub_data += Move_Pulse_NUM;
	//��ȡλ��
	motor_control.real_location_last = motor_control.real_location;
	motor_control.real_location += sub_data;
	
	/************************************ ���ݹ��� ************************************/
	/************************************ ���ݹ��� ************************************/
	//�����ٶ�
	motor_control.est_speed_mut += (	((motor_control.real_location - motor_control.real_location_last) * (CONTROL_FREQ_HZ))
																	+ ((int32_t)(motor_control.est_speed  << 5) - (int32_t)(motor_control.est_speed))
																	);
	motor_control.est_speed      = (motor_control.est_speed_mut >> 5);																	//(ȡ��)(��0ȡ��)(��������λ)
	motor_control.est_speed_mut  = ((motor_control.est_speed_mut) - ((motor_control.est_speed << 5)));	//(ȡ��)(��0ȡ��)(��������λ)
	//����λ��
	motor_control.est_lead_location = Motor_Control_AdvanceCompen(motor_control.est_speed);
	motor_control.est_location = motor_control.real_location + motor_control.est_lead_location;
	//�������
	motor_control.est_error = motor_control.soft_location - motor_control.est_location;
	
	/************************************ �˶����� ************************************/
	/************************************ �˶����� ************************************/
	if(0){}
	//�������(���������IO,�ָ������Ҫ��������)
	else if(
		 (motor_control.stall_flag)			//��ת��־��λ
	|| (motor_control.soft_disable)		//��Ŀ��_ʧ��ָ��
	|| ((!mt6816.rectify_valid))			//������У׼����Ч
	){
		Motor_Control_Clear_Integral();		//�������
		motor_control.foc_location = 0;		//��FOCλ��
		motor_control.foc_current = 0;		//��FOC����
		HW_Elec_SetSleep();					//��������
		//CurrentControl_OutSleep();			//XDrive����Ӳ���߼���������,�Զ�����
	}
	//���ɲ��
	else if(
		 (motor_control.soft_brake)			//��Ŀ��_ɲ��ָ��
	){
		Motor_Control_Clear_Integral();		//�������
		motor_control.foc_location = 0;		//��FOCλ��
		motor_control.foc_current = 0;		//��FOC����
		HW_Elec_SetBrake();					//����ɲ��
		//CurrentControl_OutBrake();			//XDrive����Ӳ���߼���������,�Զ�ɲ��
	}
	else{
		//����ģʽ��֧
		switch(motor_control.mode_run)
		{
			//����
			case Motor_Mode_Debug_Location:		Control_DCE_To_Electric(motor_control.soft_location, motor_control.soft_speed);				break;
			case Motor_Mode_Debug_Speed:			Control_PID_To_Electric(motor_control.soft_speed);																		break;
			//ֹͣ
			case Control_Mode_Stop:						HW_Elec_SetSleep();																															break;
			//DIG(CAN/RS485)
			case Motor_Mode_Digital_Location:	Control_DCE_To_Electric(motor_control.soft_location, motor_control.soft_speed);				break;
			case Motor_Mode_Digital_Speed:		Control_PID_To_Electric(motor_control.soft_speed);																		break;
			case Motor_Mode_Digital_Current:	Control_Cur_To_Electric(motor_control.soft_current);																	break;
			case Motor_Mode_Digital_Track:		Control_DCE_To_Electric(motor_control.soft_location, motor_control.soft_speed);				break;
			//MoreIO(PWM/PUL)
			case Motor_Mode_PWM_Location:			Control_DCE_To_Electric(motor_control.soft_location, motor_control.soft_speed);				break;
			case Motor_Mode_PWM_Speed:				Control_PID_To_Electric(motor_control.soft_speed);																		break;
			case Motor_Mode_PWM_Current:			Control_Cur_To_Electric(motor_control.soft_current);																	break;
			case Motor_Mode_PULSE_Location:		Control_DCE_To_Electric(motor_control.soft_location, motor_control.soft_speed);				break;
			//�����Ƿ�ģʽ
			default:	break;
		}
	}

	/************************************ ģʽ��� ************************************/
	/************************************ ģʽ��� ************************************/
	//���
	if(motor_control.mode_run != motor_control.mode_order)
	{
		motor_control.mode_run = motor_control.mode_order;
		switch(motor_control.mode_run){
			//����
			case Motor_Mode_Debug_Location:
				//Signal_MoreIO_Config(MoreIO_Mode_Disable);
				motor_control.soft_new_curve = 1;
				break;
			case Motor_Mode_Debug_Speed:
				//Signal_MoreIO_Config(MoreIO_Mode_Disable);
				motor_control.soft_new_curve = 1;
				break;
			//ֹͣ
			case Control_Mode_Stop:
				//Signal_MoreIO_Config(MoreIO_Mode_Disable);
				motor_control.soft_new_curve = 1;
				break;
			//DIG(CAN/RS485)
			case Motor_Mode_Digital_Location:
				//Signal_MoreIO_Config(MoreIO_Mode_Disable);
				motor_control.soft_new_curve = 1;
				break;
			case Motor_Mode_Digital_Speed:
				//Signal_MoreIO_Config(MoreIO_Mode_Disable);
				motor_control.soft_new_curve = 1;
				break;
			case Motor_Mode_Digital_Current:
				//Signal_MoreIO_Config(MoreIO_Mode_Disable);
				motor_control.soft_new_curve = 1;
				break;
			case Motor_Mode_Digital_Track:
				//Signal_MoreIO_Config(MoreIO_Mode_Disable);
				motor_control.soft_new_curve = 1;
				break;
			//MoreIO(PWM/PUL)
			case Motor_Mode_PWM_Location:
				//Signal_MoreIO_Config(MoreIO_Mode_PWM_Location);
				motor_control.soft_new_curve = 1;
				break;
			case Motor_Mode_PWM_Speed:
				//Signal_MoreIO_Config(MoreIO_Mode_PWM_Speed);
				motor_control.soft_new_curve = 1;
				break;
			case Motor_Mode_PWM_Current:
				//Signal_MoreIO_Config(MoreIO_Mode_PWM_Current);
				motor_control.soft_new_curve = 1;
				break;
			case Motor_Mode_PULSE_Location:
				//Signal_MoreIO_Config(MoreIO_Mode_PULSE_Locatioon);
				motor_control.soft_new_curve = 1;
				break;
			//�����Ƿ�ģʽ
			default:	break;
		}
	}

	/************************************ ӲĿ����ȡ ************************************/
	/************************************ ӲĿ����ȡ ************************************/
	//��ȡ
	switch(motor_control.mode_run){
		//����
		case Motor_Mode_Debug_Location:		/*��Ŀ�����ȡ*/			break;
		case Motor_Mode_Debug_Speed:			/*��Ŀ�����ȡ*/			break;
		//ֹͣ
		case Control_Mode_Stop:						/*��Ŀ�����ȡ*/			break;
		//DIG(CAN/RS485)
		case Motor_Mode_Digital_Location:	/*ӲĿ������ͨѶ��������*/	break;
		case Motor_Mode_Digital_Speed:		/*ӲĿ������ͨѶ��������*/	break;
		case Motor_Mode_Digital_Current:	/*ӲĿ������ͨѶ��������*/	break;
		case Motor_Mode_Digital_Track:		/*ӲĿ������ͨѶ��������*/	break;
		//MoreIO(PWM/PUL)
		case Motor_Mode_PWM_Location:
		case Motor_Mode_PWM_Speed:
		case Motor_Mode_PWM_Current:
			/*
			Signal_MoreIO_Capture_Goal();	//MoreIO�ӿڻ�ȡ����
			motor_control.goal_location	= signal_moreio.goal_location;	//��ȡĿ��λ��
			motor_control.goal_speed	  = signal_moreio.goal_speed;			//��ȡĿ���ٶ�
			motor_control.goal_current  = signal_moreio.goal_current;		//��ȡĿ�����
			motor_control.goal_disable	= signal_moreio.goal_disable;		//��ȡĿ��ʧ��
			motor_control.goal_brake		= signal_moreio.goal_brake;			//��ȡĿ��ɲ��
			*/
			break;
		case Motor_Mode_PULSE_Location:
			
			//Signal_MoreIO_Capture_Goal();	//MoreIO�ӿڻ�ȡ����
			motor_control.goal_location	+= 5;	//��ȡĿ��λ��(Countģʽ����Ŀ��λ�ô��Ŀ��λ������)
			//motor_control.goal_disable	= signal_moreio.goal_disable;		//��ȡĿ��ʧ��
			//motor_control.goal_brake		= signal_moreio.goal_brake;			//��ȡĿ��ɲ��
			
			break;
			//�����Ƿ�ģʽ
		default:
			break;
	}
	//����
	if(motor_control.goal_speed					>  Move_Rated_Speed)			motor_control.goal_speed		=  Move_Rated_Speed;
	else if(motor_control.goal_speed		< -Move_Rated_Speed)			motor_control.goal_speed		= -Move_Rated_Speed;
	if(motor_control.goal_current				>  Current_Rated_Current)	motor_control.goal_current	=  Current_Rated_Current;
	else if(motor_control.goal_current	< -Current_Rated_Current)	motor_control.goal_current	= -Current_Rated_Current;

	/************************************ �����źŷ������¶��� ************************************/
	/************************************ �����źŷ������¶��� ************************************/
	//����Ĵ����·�����
	if(	 ((motor_control.soft_disable) && (!motor_control.goal_disable))	//ʧ��ָ��ر�
		|| ((motor_control.soft_brake)   && (!motor_control.goal_brake))		//ɲ��ָ��ر�
	){
		motor_control.soft_new_curve = 1;
	}
	//�źŷ�����ˢ��
	if(motor_control.soft_new_curve){
		motor_control.soft_new_curve = 0;
		//�������غ͹���ģ�黽��
		Motor_Control_Clear_Integral();	//������ƻ�����Ŀ
		Motor_Control_Clear_Stall();		//�����תʶ��
		//CurrentControl_OutWakeUp();		//XDrive����Ӳ���߼���������,�Զ�����
		//CurrentControl_OutRunning();	//XDrive����Ӳ���߼���������,�Զ�����
		switch(motor_control.mode_run){
			//����
			case Motor_Mode_Debug_Location:		break;
			case Motor_Mode_Debug_Speed:			break;
			//ֹͣ
			case Control_Mode_Stop:						break;
			//DIG(CAN/RS485)
			case Motor_Mode_Digital_Location:	Location_Tracker_NewTask(	motor_control.est_location,	motor_control.est_speed);	break;
			case Motor_Mode_Digital_Speed:		Speed_Tracker_NewTask(		motor_control.est_speed);															break;
			case Motor_Mode_Digital_Current:	Current_Tracker_NewTask(	motor_control.foc_current);														break;
			case Motor_Mode_Digital_Track:		Move_Reconstruct_NewTask(	motor_control.est_location,	motor_control.est_speed);	break;
			//MoreIO(PWM/PUL)
			case Motor_Mode_PWM_Location:			Location_Tracker_NewTask(	motor_control.est_location,	motor_control.est_speed);	break;
			case Motor_Mode_PWM_Speed:				Speed_Tracker_NewTask(		motor_control.est_speed);															break;
			case Motor_Mode_PWM_Current:			Current_Tracker_NewTask(	motor_control.foc_current);														break;
			case Motor_Mode_PULSE_Location:		Location_Interp_NewTask(	motor_control.est_location,	motor_control.est_speed);
											/** ����λ�û�ȡ���ֵ,��Ҫ��ʼ�� **/	motor_control.goal_location = motor_control.est_location;		break;
			//�����Ƿ�ģʽ
			default:	break;
		}
	}

	/************************************ ��Ŀ����ȡ ************************************/
	/************************************ ��Ŀ����ȡ ************************************/
	//��ȡ(��λ��,���ٶ�,�����)
	switch(motor_control.mode_run){
		//����
		case Motor_Mode_Debug_Location:		Motor_MultiDebug_Location();	break;
		case Motor_Mode_Debug_Speed:			Motor_MultiDebug_Speed();			break;
		//ֹͣ
		case Control_Mode_Stop:						break;
		//DIG(CAN/RS485)
		case Motor_Mode_Digital_Location:	Location_Tracker_Capture_Goal(motor_control.goal_location);
																			motor_control.soft_location = location_tck.go_location;
																			motor_control.soft_speed    = location_tck.go_speed;
																			break;
		case Motor_Mode_Digital_Speed:		Speed_Tracker_Capture_Goal(motor_control.goal_speed);
																			motor_control.soft_speed    = speed_tck.go_speed;
																			break;
		case Motor_Mode_Digital_Current:	Current_Tracker_Capture_Goal(motor_control.goal_current);
																			motor_control.soft_current  = current_tck.go_current;
																			break;
		case Motor_Mode_Digital_Track:		Move_Reconstruct_Capture_Goal(motor_control.goal_location, motor_control.goal_speed);
																			motor_control.soft_location = move_reco.go_location;
																			motor_control.soft_speed    = move_reco.go_speed;
																			break;
		//MoreIO(PWM/PUL)
		case Motor_Mode_PWM_Location:			Location_Tracker_Capture_Goal(motor_control.goal_location);
																			motor_control.soft_location = location_tck.go_location;
																			motor_control.soft_speed    = location_tck.go_speed;
																			break;			
		case Motor_Mode_PWM_Speed:				Speed_Tracker_Capture_Goal(motor_control.goal_speed);
																			motor_control.soft_speed    = speed_tck.go_speed;
																			break;
		case Motor_Mode_PWM_Current:			Current_Tracker_Capture_Goal(motor_control.goal_current);
																			motor_control.soft_current  = current_tck.go_current;
																			break;
		case Motor_Mode_PULSE_Location:		Location_Interp_Capture_Goal(motor_control.goal_location);
																			motor_control.soft_location = location_interp.go_location;
																			motor_control.soft_speed    = location_interp.go_speed;
																			break;
		//�����Ƿ�ģʽ
		default:	break;
	}
	//��ȡ(��ʧ��,��ɲ��)
	motor_control.soft_disable = motor_control.goal_disable;
	motor_control.soft_brake = motor_control.goal_brake;
	
	/************************************ ״̬ʶ�� ************************************/
	/************************************ ״̬ʶ�� ************************************/
	int32_t abs_out_electric = D_ABS(motor_control.foc_current);
	//��ת���
	if( ((motor_control.mode_run == Motor_Mode_Digital_Current) || (motor_control.mode_run == Motor_Mode_PWM_Current))	//����ģʽ
	 && (abs_out_electric != 0)																																													//���������
	 && (D_ABS(motor_control.est_speed) < (Move_Pulse_NUM/5))																															//����1/5ת/s
	){
		if(motor_control.stall_time_us >= (1000 * 1000))	motor_control.stall_flag = 1;
		else																							motor_control.stall_time_us += CONTROL_PERIOD_US;
	}
	else if( (abs_out_electric == Current_Rated_Current)						//�����
				&& (D_ABS(motor_control.est_speed) < (Move_Pulse_NUM/5))		//����1/5ת/s
	){
		if(motor_control.stall_time_us >= (1000 * 1000))	motor_control.stall_flag = 1;
		else																							motor_control.stall_time_us += CONTROL_PERIOD_US;
	}
	else{
		motor_control.stall_time_us = 0;
		//��ת��־�������������Ҫ�ⲿָ��������
	}

	//���ؼ��
	if(abs_out_electric == Current_Rated_Current){		//�����
		if(motor_control.overload_time_us >= (1000 * 1000))	motor_control.overload_flag = 1;
		else																								motor_control.overload_time_us += CONTROL_PERIOD_US;
	}
	else{
		motor_control.overload_time_us = 0;
		motor_control.overload_flag = 0;//���ر�־�������
	}
	
	/************************************ ״̬��¼ ************************************/
	/************************************ ״̬��¼ ************************************/
	//ͳһ�ĵ��״̬
	if(motor_control.mode_run == Control_Mode_Stop)	//ֹͣģʽ
		motor_control.state = Control_State_Stop;
	else if(motor_control.stall_flag)								//��ת��־��λ
		motor_control.state = Control_State_Stall;
	else if(motor_control.overload_flag)						//���ر�־��λ
		motor_control.state = Control_State_Overload;
	else
	{
		if(motor_control.mode_run == Motor_Mode_Digital_Location){
			if( (motor_control.soft_location == motor_control.goal_location)
			 && (motor_control.soft_speed == 0))
				motor_control.state = Control_State_Finish;		//��ӲĿ��ƥ��
			else
				motor_control.state = Control_State_Running;
		}
		else if(motor_control.mode_run == Motor_Mode_Digital_Speed){
			if(motor_control.soft_speed == motor_control.goal_speed)
				motor_control.state = Control_State_Finish;		//��ӲĿ��ƥ��
			else
				motor_control.state = Control_State_Running;
		}
		else if(motor_control.mode_run == Motor_Mode_Digital_Current){
			if(motor_control.soft_current == motor_control.goal_current)
				motor_control.state = Control_State_Finish;		//��ӲĿ��ƥ��
			else
				motor_control.state = Control_State_Running;
		}
		else{
			motor_control.state = Control_State_Finish;			//��ӲĿ��ƥ��
		}
	}
}

/**
  * @brief  ���������
  * @param  NULL
  * @retval NULL
**/
void Motor_Control_Clear_Integral(void)
{
	//�ڴ˴�����������ƶδ����еĻ�����
	
	//PID
	pid.i_mut = 0;
	pid.i_dec = 0;
	pid.oi = 0;
	
	//DCE
	dce.i_mut = 0;
	dce.i_dec = 0;
	dce.oi = 0;
	
	//Debug
	mc_debug.mut = 0;
	mc_debug.dec = 0;
}

/**
  * @brief  �����תʶ��
  * @param  NULL
  * @retval NULL
**/
void Motor_Control_Clear_Stall(void)
{
	motor_control.stall_time_us = 0;		//��ת��ʱ��
	motor_control.stall_flag = 0;		//��ת��־
}

/**
  * @brief  ��ǰ�ǲ���
  * @param  _speed:�����ٶ�
  * @retval �����Ƕ�
**/
int32_t Motor_Control_AdvanceCompen(int32_t _speed)
{
	/******************** !!!!! ��Ҫ1������������ȡ��DPSϵ�д���                                                  !!!!! ********************/
	/******************** !!!!! ��Ҫ2������Դ���������������ݣ�������������ȫ�ʺ�TLE5012��MT6816                !!!!! ********************/
	/******************** !!!!! ��Ҫ3�����ڲ�������������Ѳ�������ʮ�ַ�ʱ���������߲���׼���ڽ��ڽ���У׼����� !!!!! ********************/

	int32_t compen;
	if(_speed < 0){
		if(_speed > -100000)				compen = 0;
		else if(_speed > -1300000)	compen = (((_speed +  100000) * 262) >> 20) -   0;
		else if(_speed > -2200000)	compen = (((_speed + 1300000) * 105) >> 20) - 300;
		else												compen = (((_speed + 2200000) *  52) >> 20) - 390;
		if(compen < -430)						compen = -430;
	}
	else{
		if(_speed < 100000)					compen = 0;																					//(      0,  0) ~ ( 100000,  0)
		else if(_speed <  1300000)	compen = (((_speed -  100000) * 262) >> 20) +   0;	//( 100000,  0) ~ (1300000,300)
		else if(_speed <  2200000)	compen = (((_speed - 1300000) * 105) >> 20) + 300;	//(1300000,300) ~ (2200000,390)
		else												compen = (((_speed - 2200000) *  52) >> 20) + 390;	//(2200000,390) ~ 
		if(compen > 430)						compen = 430;
	}
	return compen;
}
