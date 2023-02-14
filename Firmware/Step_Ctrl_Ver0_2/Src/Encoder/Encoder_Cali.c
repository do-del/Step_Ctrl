#include "Encoder_Cali.h"
#include "stdio.h"

//У׼��ʵ��
Encode_Cali_Typedef encode_cali;	//����һ��У׼��

//˽�к���
uint32_t CycleRem(uint32_t a,uint32_t b);									//ȡ��(������ʽʵ��,��ֹ�������Ż�)
int32_t CycleSub(int32_t a, int32_t b, int32_t cyc);			//ȡѭ����(������ʽʵ��,��ֹ�������Ż�)
int32_t CycleAverage(int32_t a, int32_t b, int32_t cyc);	//ȡѭ��ƽ��ֵ(������ʽʵ��,��ֹ�������Ż�)

void Calibration_Data_Check(void);		//У׼��ԭʼ���ݼ��

/**
  * ȡ��,����һ�����ڵĸ���(���Ŀ���-�����Զ�ת��Ϊ�޷�������)
  * @param  NULL
  * @retval NULL
**/
uint32_t CycleRem(uint32_t a,uint32_t b)
{
	return (a+b)%b;
}

/**
  * ȡѭ����
  * @param  NULL
  * @retval NULL
	* ��cycΪһ�����ڵ����
  * ����a = 16384, b = 1,cyc = 16384, sub_data = 16383>(16384/2),sub_data = sub_data - 16384 = -1
**/
int32_t CycleSub(int32_t a, int32_t b, int32_t cyc)
{
	int32_t sub_data;

	sub_data = a - b;
	if(sub_data > (cyc >> 1))		sub_data -= cyc;
	if(sub_data < (-cyc >> 1))	sub_data += cyc;
	return sub_data;
}

/**
  * ȡѭ��ƽ��ֵ
  * @param  NULL
  * @retval NULL
	* ie. a = 16384, b = 2, cyc = 16384;sub_data = 16384 - 1 = 16382 > (cyc/2),ave_data = (16384 + 2) / 2;��Ϊave_data >= cyc/2��ave_data = ave_data - cyc/2
**/
int32_t CycleAverage(int32_t a, int32_t b, int32_t cyc)
{
	int32_t sub_data;
	int32_t ave_data;

	sub_data = a - b;
	ave_data = (a + b) >> 1;

	if(D_ABS(sub_data) > (cyc >> 1))
	{
		if(ave_data >= (cyc >> 1))	ave_data -= (cyc >> 1);
		else												ave_data += (cyc >> 1);
	}
	return ave_data;
}

/**
  * ȡѭ��ƽ��ֵ
  * @param  NULL
  * @retval NULL
**/
int32_t CycleDataAverage(uint16_t *data, uint16_t length, int32_t cyc)
{
	int32_t sum_data = 0;	//���ֺ�
	int32_t sub_data;			//��
	int32_t	diff_data;		//΢��

	//���ص�һֵ
	sum_data += (int32_t)data[0];
	//���غ����ֵ
	for(uint16_t i=1; i<length; i++){
		diff_data = (int32_t)data[i];
		sub_data = (int32_t)data[i] - (int32_t)data[0];
		if(sub_data > (cyc >> 1))		diff_data = (int32_t)data[i] - cyc;
		if(sub_data < (-cyc >> 1))	diff_data = (int32_t)data[i] + cyc;
		sum_data += diff_data;
	}
	//����ƽ��ֵ
	sum_data = sum_data / length;
	//ƽ��ֵ��׼��
	if(sum_data < 0)		sum_data += cyc;
	if(sum_data > cyc)	sum_data -= cyc;
	return sum_data;
}

/**
  * @brief  У׼��ԭʼ���ݼ��
  * @param  NULL
  * @retval NULL
  */
void Calibration_Data_Check(void)
{
	uint32_t	count;			//���ڸ�������
	int32_t		sub_data;		//���ڸ������

	/******************** ���ƽ��ֵ�����Լ����� ********************/
	//���ƽ��ֵ����
	for(count=0; count<Move_Step_NUM+1; count++){
		encode_cali.coder_data_f[count] = (uint16_t)CycleAverage((int32_t)encode_cali.coder_data_f[count], (int32_t)encode_cali.coder_data_r[count], CALI_Encode_Res);
	}
	//ƽ��ֵ���ݼ��
	sub_data = CycleSub((int32_t)encode_cali.coder_data_f[0], (int32_t)encode_cali.coder_data_f[Move_Step_NUM-1], CALI_Encode_Res);
	if(sub_data == 0)	{	encode_cali.error_code = CALI_Error_Average_Dir; return;	}
	if(sub_data > 0)	{	encode_cali.dir = 1;	}
	if(sub_data < 0)	{	encode_cali.dir = 0;	}
	for(count=1; count<Move_Step_NUM; count++)
	{
		sub_data = CycleSub((int32_t)encode_cali.coder_data_f[count], (int32_t)encode_cali.coder_data_f[count-1], CALI_Encode_Res);
		if(D_ABS(sub_data) > (CALI_Gather_Encode_Res * 3 / 2))	{	encode_cali.error_code = CALI_Error_Average_Continuity;	encode_cali.error_data = count;	return;	}	//�������ݲ����
		if(D_ABS(sub_data) < (CALI_Gather_Encode_Res * 1 / 2))	{	encode_cali.error_code = CALI_Error_Average_Continuity;	encode_cali.error_data = count;	return;	}	//�������ݲ��С
		if(sub_data == 0)																			{	encode_cali.error_code = CALI_Error_Average_Dir;				encode_cali.error_data = count;	return;	}
		if((sub_data > 0) && (!encode_cali.dir))							{	encode_cali.error_code = CALI_Error_Average_Dir;				encode_cali.error_data = count;	return;	}
		if((sub_data < 0) && (encode_cali.dir))								{	encode_cali.error_code = CALI_Error_Average_Dir;				encode_cali.error_data = count;	return;	}
	}

	/******************** ȫ����У׼ ��ȫ��ϴ�������������ʵ����λ�Ƿ����Թ�ϵ ********************/
	//Ѱ�������±꼰��Ծ��ֵ
	uint32_t step_num = 0;
	if(encode_cali.dir){
		for(count=0; count<Move_Step_NUM; count++){
			sub_data = (int32_t)encode_cali.coder_data_f[CycleRem(count+1, Move_Step_NUM)] - (int32_t)encode_cali.coder_data_f[CycleRem(count, Move_Step_NUM)];
			if(sub_data < 0){
				step_num++;
				encode_cali.rcd_x = count;//ʹ������ǰ��
				encode_cali.rcd_y = (CALI_Encode_Res-1) - encode_cali.coder_data_f[CycleRem(encode_cali.rcd_x, Move_Step_NUM)];
			}
		}
		if(step_num != 1){
			encode_cali.error_code = CALI_Error_PhaseStep;
			return;
		}
	}
	else{
		for(count=0; count<Move_Step_NUM; count++){
			sub_data = (int32_t)encode_cali.coder_data_f[CycleRem(count+1, Move_Step_NUM)] - (int32_t)encode_cali.coder_data_f[CycleRem(count, Move_Step_NUM)];
			if(sub_data > 0){
				step_num++;
				encode_cali.rcd_x = count;//ʹ������ǰ��
				encode_cali.rcd_y = (CALI_Encode_Res-1) - encode_cali.coder_data_f[CycleRem(encode_cali.rcd_x+1, Move_Step_NUM)];
			}
		}
		if(step_num != 1){
			encode_cali.error_code = CALI_Error_PhaseStep;
			return;
		}
	}

	//У׼OK
	encode_cali.error_code = CALI_No_Error;
	printf("\r\nencode no error\r\n");
	return;
}

/**
  * @brief  У׼����ʼ��
  * @param  NULL
  * @retval NULL
  */
void Calibration_Init(void)
{
	//�ź�
	encode_cali.trigger = 0;
	encode_cali.error_code = CALI_No_Error;
	encode_cali.error_data = 0;
	//��ȡ����
	encode_cali.state = CALI_Disable;
	encode_cali.out_location = 0;
		//coder_data_f[]
		//coder_data_r[]
		//dir
	//ȫ����У׼��������
	encode_cali.rcd_x = 0;
	encode_cali.rcd_y = 0;
	encode_cali.result_num = 0;
}


/**
  * @brief  У׼���жϻص�
  * @param  NULL
  * @retval NULL
  */
void Calibration_Interrupt_Callback(void)
{
#define AutoCali_Speed	2		//�Զ�У׼�ٶ�(��Ҫ���ڱ�����Ԥ���ݲɼ�)
#define Cali_Speed			1		//У׼�ٶ�(���ھ�ȷ�����ݲɼ�)

	//���ѵ�������
	//CurrentControl_OutWakeUp();
	
	//״̬�任
	switch(encode_cali.state)
	{
		//ʧ��״̬
		case CALI_Disable:
			//ELEC_Set_Sleep();
			if(encode_cali.trigger)
			{
				HW_Elec_SetDivideElec(encode_cali.out_location, Current_Cali_Current);
				//CurrentControl_Out_FeedTrack(encode_cali.out_location, Current_Cali_Current, true, true);
				encode_cali.out_location = Move_Pulse_NUM;			//�����1Ȧλ��
				encode_cali.gather_count = 0;										//�ɼ�����
				encode_cali.state = CALI_Forward_Encoder_AutoCali;	//--->��������ת�Զ�У׼
				//��ʼ����־
				encode_cali.error_code = CALI_No_Error;
				encode_cali.error_data = 0;
			}
		break;
		//��������ת�Զ�У׼
		case CALI_Forward_Encoder_AutoCali://��ת��1Ȧ (1 * Motor_Pulse_NUM) -> (2 * Motor_Pulse_NUM)
			encode_cali.out_location += AutoCali_Speed;
			HW_Elec_SetDivideElec(encode_cali.out_location, Current_Cali_Current);
			//CurrentControl_Out_FeedTrack(encode_cali.out_location, Current_Cali_Current, true, true);
			if(encode_cali.out_location == 2 * Move_Pulse_NUM)
			{
				encode_cali.out_location = Move_Pulse_NUM;
				encode_cali.state = CALI_Forward_Measure;//--->�������
			}
		break;
		//�������
		case CALI_Forward_Measure://(Motor_Pulse_NUM) -> (2 * Motor_Pulse_NUM)
			if((encode_cali.out_location % Move_Divide_NUM) == 0)//ÿ����ɼ�ϸ������ɼ�һ������
			{
				//�ɼ�
				encode_cali.coder_data_gather[encode_cali.gather_count++] = mt6816.angle_data;
				if(encode_cali.gather_count == Gather_Quantity){
					//��¼����
					encode_cali.coder_data_f[(encode_cali.out_location - Move_Pulse_NUM) / Move_Divide_NUM]
						= CycleDataAverage(encode_cali.coder_data_gather, Gather_Quantity, CALI_Encode_Res);
					//�ɼ���������
					encode_cali.gather_count = 0;
					//�ƶ�λ��
					encode_cali.out_location += Cali_Speed;
				}
			}
			else{
				//�ƶ�λ��
				encode_cali.out_location += Cali_Speed;
			}	
			HW_Elec_SetDivideElec(encode_cali.out_location, Current_Cali_Current);
			//CurrentControl_Out_FeedTrack(encode_cali.out_location, Current_Cali_Current, true, true);
			if(encode_cali.out_location > (2 * Move_Pulse_NUM))
			{
				encode_cali.state = CALI_Reverse_Ret;//--->�������
			}
		break;
		//�������
		case CALI_Reverse_Ret://(2 * Motor_Pulse_NUM) -> (2 * Motor_Pulse_NUM + Motor_Divide_NUM * 20)
			encode_cali.out_location += Cali_Speed;
			HW_Elec_SetDivideElec(encode_cali.out_location, Current_Cali_Current);
			//CurrentControl_Out_FeedTrack(encode_cali.out_location, Current_Cali_Current, true, true);
			if(encode_cali.out_location == (2 * Move_Pulse_NUM + Move_Divide_NUM * 20))
			{
				encode_cali.state = CALI_Reverse_Gap;//--->��������
			}
		break;
		//��������
		case CALI_Reverse_Gap://(2 * Motor_Pulse_NUM + Motor_Divide_NUM * 20) -> (2 * Motor_Pulse_NUM)
			encode_cali.out_location -= Cali_Speed;
			HW_Elec_SetDivideElec(encode_cali.out_location, Current_Cali_Current);
			//CurrentControl_Out_FeedTrack(encode_cali.out_location, Current_Cali_Current, true, true);
			if(encode_cali.out_location == (2 * Move_Pulse_NUM))
			{
				encode_cali.state = CALI_Reverse_Measure;//--->�������
			}
		break;
		//�������
		case CALI_Reverse_Measure://(2 * Motor_Pulse_NUM) -> (Motor_Pulse_NUM)
			if((encode_cali.out_location % Move_Divide_NUM) == 0)//ÿ����ɼ�ϸ������ɼ�һ������
			{
				//�ɼ�
				encode_cali.coder_data_gather[encode_cali.gather_count++] = mt6816.angle_data;
				if(encode_cali.gather_count == Gather_Quantity){
					//��¼����
					encode_cali.coder_data_r[(encode_cali.out_location - Move_Pulse_NUM) / Move_Divide_NUM]
						= CycleDataAverage(encode_cali.coder_data_gather, Gather_Quantity, CALI_Encode_Res);
					//�ɼ���������
					encode_cali.gather_count = 0;
					//�ƶ�λ��
					encode_cali.out_location -= Cali_Speed;
				}
			}
			else{
				//�ƶ�λ��
				encode_cali.out_location -= Cali_Speed;
			}	
			HW_Elec_SetDivideElec(encode_cali.out_location, Current_Cali_Current);
			//CurrentControl_Out_FeedTrack(encode_cali.out_location, Current_Cali_Current, true, true);
			if(encode_cali.out_location < Move_Pulse_NUM)
			{
				encode_cali.state = CALI_Operation;//--->����
			}
		break;
		//����
		case CALI_Operation:
			//����У׼������
			HW_Elec_SetDivideElec(0, 0);
			//CurrentControl_Out_FeedTrack(0, 0, true, true);
		break;
		default:
		break;
	}
}

/**
  * @brief  У׼��������ص�
  * @param  NULL
  * @retval NULL
  */
void Calibration_Loop_Callback(void)
{
	int32_t		data_i32;
	uint16_t	data_u16;
//	uint16_t	data_u8;
	
	//�ǽ���̬�˳�
	if(encode_cali.state != CALI_Operation)
		return;
	
	//PWM���˥��̬
	HW_Elec_SetSleep();

	//У׼��ԭʼ���ݼ��
	Calibration_Data_Check();
	
	/**********  ���п��ٱ�����  **********/
	if(encode_cali.error_code == CALI_No_Error)
	{
		//���ݽ���
		/******************** ȫ����У׼ ��ȫ��ϴ�������������ʵ����λ�Ƿ����Թ�ϵ ********************/
		int32_t step_x, step_y;
		encode_cali.result_num = 0;
		Stockpile_Flash_Data_Empty(&stockpile_quick_cali);		//����������
		Stockpile_Flash_Data_Begin(&stockpile_quick_cali);		//��ʼд������
		if(encode_cali.dir){
			for(step_x = encode_cali.rcd_x; step_x < encode_cali.rcd_x + Move_Step_NUM + 1; step_x++){
				data_i32 = CycleSub(	encode_cali.coder_data_f[CycleRem(step_x+1, Move_Step_NUM)],
															encode_cali.coder_data_f[CycleRem(step_x,   Move_Step_NUM)],
															CALI_Encode_Res);
				if(step_x == encode_cali.rcd_x){//��ʼ��Ե
					for(step_y = encode_cali.rcd_y; step_y < data_i32; step_y++){
						data_u16 = CycleRem(	Move_Divide_NUM * step_x + Move_Divide_NUM * step_y / data_i32,
																	Move_Pulse_NUM);
						Stockpile_Flash_Data_Write_Data16(&stockpile_quick_cali, &data_u16, 1);
						encode_cali.result_num++;
					}
				}
				else if(step_x == encode_cali.rcd_x + Move_Step_NUM){//������Ե
					for(step_y = 0; step_y < encode_cali.rcd_y; step_y++){
						data_u16 = CycleRem(	Move_Divide_NUM * step_x + Move_Divide_NUM * step_y / data_i32,
																	Move_Pulse_NUM);
						Stockpile_Flash_Data_Write_Data16(&stockpile_quick_cali, &data_u16, 1);
						encode_cali.result_num++;
					}
				}
				else{//�м�
					for(step_y = 0; step_y < data_i32; step_y++){
						data_u16 = CycleRem(	Move_Divide_NUM * step_x + Move_Divide_NUM * step_y / data_i32,
																	Move_Pulse_NUM);
						Stockpile_Flash_Data_Write_Data16(&stockpile_quick_cali, &data_u16, 1);
						encode_cali.result_num++;
					}
				}
			}
		}
		else
		{
			for(step_x = encode_cali.rcd_x + Move_Step_NUM; step_x > encode_cali.rcd_x - 1; step_x--)
			{
				data_i32 = CycleSub(	encode_cali.coder_data_f[CycleRem(step_x, Move_Step_NUM)],
															encode_cali.coder_data_f[CycleRem(step_x+1, Move_Step_NUM)],
															CALI_Encode_Res);
				if(step_x == encode_cali.rcd_x+Move_Step_NUM){//��ʼ��Ե
					for(step_y = encode_cali.rcd_y; step_y < data_i32; step_y++){
						data_u16 = CycleRem(	Move_Divide_NUM * (step_x+1) - Move_Divide_NUM * step_y / data_i32,
																	Move_Pulse_NUM);
						Stockpile_Flash_Data_Write_Data16(&stockpile_quick_cali, &data_u16, 1);
						encode_cali.result_num++;
					}
				}
				else if(step_x == encode_cali.rcd_x){//������Ե
					for(step_y = 0; step_y < encode_cali.rcd_y; step_y++){
						data_u16 = CycleRem(	Move_Divide_NUM * (step_x+1) - Move_Divide_NUM * step_y / data_i32,
																	Move_Pulse_NUM);
						Stockpile_Flash_Data_Write_Data16(&stockpile_quick_cali, &data_u16, 1);
						encode_cali.result_num++;
					}
				}
				else{//�м�
					for(step_y = 0; step_y < data_i32; step_y++){
						data_u16 = CycleRem(	Move_Divide_NUM * (step_x+1) - Move_Divide_NUM * step_y / data_i32,
																	Move_Pulse_NUM);
						Stockpile_Flash_Data_Write_Data16(&stockpile_quick_cali, &data_u16, 1);
						encode_cali.result_num++;
					}
				}
			}
		}
		Stockpile_Flash_Data_End(&stockpile_quick_cali);	//����д������
		
		if(encode_cali.result_num != CALI_Encode_Res)
			encode_cali.error_code = CALI_Error_Analysis_Quantity;
	}

	//ȷ��У׼���
	if(encode_cali.error_code == CALI_No_Error){
		mt6816.rectify_valid = 1;
	}
	else{
		mt6816.rectify_valid = 0;
		Stockpile_Flash_Data_Empty(&stockpile_quick_cali);	//���У׼������
	}
	
	//�˶����ø���
	//Signal_MoreIO_Capture_Goal();			//��ȡ�źŶ˿����������У׼�ڼ�������ź�
	//motor_control.stall_flag = 1;	//������ת����,��У׼������˶�����
	
	//����У׼�ź�
	encode_cali.state = CALI_Disable;
	encode_cali.trigger =0;			//���У׼����
}
