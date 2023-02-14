#ifndef SIN_FORM_H
#define SIN_FORM_H

#include <stdint.h>

//sin_pi_m2 (对应原始sin函数图形0~pi*2)
#define sin_pi_m2_dpix			1024	//水平分辨率
#define sin_pi_m2_dpiybit		12		//垂直分辨率位数(4096)
extern const int16_t sin_pi_m2[sin_pi_m2_dpix + 1];
	 
#endif
