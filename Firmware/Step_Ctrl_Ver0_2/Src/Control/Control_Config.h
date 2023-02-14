#ifndef CONTROL_CONFIG_H
#define CONTROL_CONFIG_H

/********************  Ӳ��������  ********************/
#define Current_Rated_Current		(2000)		//�����(mA)
#define Current_Cali_Current		(1000)		//У׼����(mA)

/********************  �˶�����������  ********************/
#define Move_Step_NUM				((int32_t)(200))																//(ʹ�õĵ����Ȧ����)(ÿ���ų���ת90��)
#define Move_Divide_NUM			((int32_t)(256))																//(ÿ�����Լ�����ϸ����)
#define Move_Pulse_NUM			((int32_t)(Move_Step_NUM * Move_Divide_NUM))		//(�����Ȧ������)
#define Move_Rated_Speed			((int32_t)(50 * Move_Pulse_NUM))							//(�ת��)(50תÿ��)
#define Move_Rated_UpAcc				((int32_t)(1000 * Move_Pulse_NUM))					//(�̼�����ټ��ٶ�)(1000r/ss)
#define Move_Rated_DownAcc			((int32_t)(1000 * Move_Pulse_NUM))					//(�̼�����ټ��ٶ�)(1000r/ss)
#define Move_Rated_UpCurrentRate		((int32_t)(20 * Current_Rated_Current))	//(�̼�������ݶ�)(20���/s)
#define Move_Rated_DownCurrentRate	((int32_t)(20 * Current_Rated_Current))	//(�̼�������ݶ�)(20���/s)

/****************************************  У׼��������  ****************************************/
#define CALI_Encode_Bit					((int32_t)(14))																//(������λ��)(14λ�������)
#define CALI_Encode_Res					((int32_t)((0x00000001U) << CALI_Encode_Bit))	//(�������ֱ���)(2^14 = 16384)(16k�ֱ���)(ռ��32kУ׼�ռ�)
#define CALI_Gather_Encode_Res	((int32_t)(CALI_Encode_Res / Move_Step_NUM))	//(У׼ÿ�ɼ����������ֱ���)

/****************************************  ������Ƶ��������  ****************************************/
#define CONTROL_FREQ_HZ			(20000)												//����Ƶ��_hz
#define CONTROL_PERIOD_US		(1000000 / CONTROL_FREQ_HZ)		//��������_us

#endif
