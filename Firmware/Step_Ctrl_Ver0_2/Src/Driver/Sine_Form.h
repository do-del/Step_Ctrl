#ifndef SIN_FORM_H
#define SIN_FORM_H

#include <stdint.h>

//sin_pi_m2 (��Ӧԭʼsin����ͼ��0~pi*2)
#define sin_pi_m2_dpix			1024	//ˮƽ�ֱ���
#define sin_pi_m2_dpiybit		12		//��ֱ�ֱ���λ��(4096)
extern const int16_t sin_pi_m2[sin_pi_m2_dpix + 1];
	 
#endif
