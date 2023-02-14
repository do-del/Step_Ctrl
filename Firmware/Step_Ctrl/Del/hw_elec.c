#include "hw_elec.h"
#include "math_sin.h"

void HW_Elec_Init(void)
{
	HAL_TIM_PWM_Start(&HW_ELEC_PWM_Get_HTIM, HW_ELEC_APWM_CHANNEL);
	HAL_TIM_PWM_Start(&HW_ELEC_PWM_Get_HTIM, HW_ELEC_BPWM_CHANNEL);
}

void HW_Output(float theta,uint8_t effort) 
{	
  int16_t v_coil_A;
  int16_t v_coil_B;

  int16_t sin_coil_A;
  int16_t sin_coil_B;
	
  int16_t angle_1;
  int16_t angle_2;
		
  uint16_t phase_multiplier=500;
	
  angle_1=Mod(phase_multiplier*theta,3600);   
  angle_2=Mod(phase_multiplier*theta+900,3600);
  
  sin_coil_A=sin_1[angle_1];
  sin_coil_B=sin_1[angle_2];

  v_coil_A=effort*sin_coil_A>>10;
  v_coil_B=effort*sin_coil_B>>10;
	
  if(v_coil_A>=0)  
  {
    Out_PWMtoDAC_A(v_coil_A);  
		Out_AP_L();
		Out_AM_H();
  }
  else  
  {
    Out_PWMtoDAC_A(-v_coil_A);  
		Out_AP_H();
		Out_AM_L();
  } 
  if(v_coil_B>=0)  
  {
    Out_PWMtoDAC_B(v_coil_B);  
		Out_BP_H();
		Out_BM_L();  
  }
  else 
  {
    Out_PWMtoDAC_B(-v_coil_B); 
		Out_BP_L();
		Out_BM_H(); 
  }
}
