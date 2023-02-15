#include "AT8236.h"
#include "Sine_Form.h"

//Ӳ������ʵ��
Coil_Typedef		coil_a;	//��������
Coil_Typedef		coil_b;	//��������

/**
  * @brief  12λ��׼��ѹ������
  * @param  elec_va: ����ͨ��a��׼
  * @param  elec_vb: ����ͨ��b��׼
  * @retval NULL
  */
static void CurrentControl_VREF_12Bit_MixOut(uint16_t elec_va, uint16_t elec_vb)
{
	Out_PWMtoDAC_A((elec_va >> 2));
	Out_PWMtoDAC_B((elec_vb >> 2));
}


void HW_Elec_Init(void)
{
	HAL_TIM_PWM_Start(&HW_ELEC_PWM_Get_HTIM, HW_ELEC_APWM_CHANNEL);
	HAL_TIM_PWM_Start(&HW_ELEC_PWM_Get_HTIM, HW_ELEC_BPWM_CHANNEL);
}

/**
  * @brief  ����˯��
  * @param  NULL
  * @retval NULL
  */
void HW_Elec_SetSleep(void)
{
	coil_a.dac_reg = 0;
	coil_b.dac_reg = 0;
	CurrentControl_VREF_12Bit_MixOut(coil_a.dac_reg, coil_b.dac_reg);
	Out_AP_L();	Out_AM_L();
	Out_BP_L();	Out_BM_L();
}

/**
  * @brief  ��������ɲ��
  * @param  NULL
  * @retval NULL
  */
void HW_Elec_SetBrake(void)
{
	coil_a.dac_reg = 0;
	coil_b.dac_reg = 0;
	CurrentControl_VREF_12Bit_MixOut(coil_a.dac_reg, coil_b.dac_reg);
	Out_AP_H();	Out_AM_H();
	Out_BP_H();	Out_BM_H();
}

/**
  * @brief  �������ϸ�ֵ���
  * @param  divide:  ϸ�� (0 ~ ϸ����)
  * @param  dac_reg: ���� (0 ~ 3300mA)
  * @retval NULL
  */
void HW_Elec_SetDivideElec(uint32_t divide, int32_t elec_ma)
{
	//��ϸ�����������ָ��
	coil_b.conver = (divide) & (0x000003FF);								//��1024ȡ��
	coil_a.conver = (coil_b.conver + (256)) & (0x000003FF);	//��1024ȡ��
	
	//������ָ������������(�ռ任ʱ�䷽��)
	coil_a.sin_data = sin_pi_m2[coil_a.conver];
	coil_b.sin_data = sin_pi_m2[coil_b.conver];

	//���������ݻ��DAC�Ĵ�������
	uint32_t dac_reg = D_ABS(elec_ma);								//��ѹ������ϵΪ1:1(��������Ϊ0.1ŷ)
	dac_reg = (uint32_t)(dac_reg * 5083) >> 12; 		//(dac_reg * 4095 / 3300)�ı���
	dac_reg = dac_reg & (0x00000FFF);								//(��4096ȡ��)(��Сȡ��)(��������λ)
	coil_a.dac_reg = (uint32_t)(dac_reg * D_ABS(coil_a.sin_data)) >> sin_pi_m2_dpiybit;	//(--- / sin_1024_dpiy)�ı���
	coil_b.dac_reg = (uint32_t)(dac_reg * D_ABS(coil_b.sin_data)) >> sin_pi_m2_dpiybit;	//(--- / sin_1024_dpiy)�ı���

	//DAC���
	CurrentControl_VREF_12Bit_MixOut(coil_a.dac_reg, coil_b.dac_reg);
	if(coil_a.sin_data > 0)				{	Out_AP_H();		Out_AM_L();		}
	else if(coil_a.sin_data < 0)	{	Out_AP_L();		Out_AM_H();		}
	else 													{	Out_AP_H();		Out_AM_H();		}
	
	if(coil_b.sin_data > 0)				{	Out_BP_H();		Out_BM_L();		}
	else if(coil_b.sin_data < 0) 	{	Out_BP_L();		Out_BM_H();		}
	else													{	Out_BP_H();		Out_BM_H();		}
}
