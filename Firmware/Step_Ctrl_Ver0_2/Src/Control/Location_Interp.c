/*****
  ** @file     : Location_Interp.c/h
  ** @brief    : 位置插补器
  ** @versions : 1.1.7
  ** @time     : 2019/12/06
  ** @reviser  : unli (JiXi XuanCheng China)
  ** @explain  : null
*****/

//Oneself
#include "Location_Interp.h"


/****************************************  位置补插器  ****************************************/
/****************************************  位置补插器  ****************************************/
/****************************************  位置补插器  ****************************************/
//Location_Interp实例
Location_Interp_Typedef location_interp;

/**
  * 位置补插器初始化
  * @param  interp	位置插补器实例
  * @retval NULL
**/
void Location_Interp_Init(void)
{
	//源信号数据
	location_interp.record_location = 0;
	location_interp.record_location_last = 0;
	location_interp.est_location = 0;
	location_interp.est_speed_mut = 0;
	location_interp.est_speed = 0;
	//输出跟踪控制量
	location_interp.go_location = 0;
	location_interp.go_speed = 0;
}

/**
  * 位置补插器开始新任务
  * @param  interp			位置插补器实例
  * @param  real_location	实时位置
  * @param  real_speed		实时速度
  * @retval NULL
**/
void Location_Interp_NewTask(int32_t real_location, int32_t real_speed)
{
	//更新源信号数据
	location_interp.record_location = real_location;
	location_interp.record_location_last = real_location;
	location_interp.est_location = real_location;
	location_interp.est_speed = real_speed;
}

/**
  * 位置补插器获得立即位置和立即速度
  * @param  interp			位置插补器实例
  * @param  goal_location	目标位置
  * @param  NULL
  * @retval NULL
**/
void Location_Interp_Capture_Goal(int32_t goal_location)
{
	//记录源信号
	location_interp.record_location_last = location_interp.record_location;
	location_interp.record_location = goal_location;

	//估计源信号速度
	location_interp.est_speed_mut += (	((location_interp.record_location - location_interp.record_location_last) * CONTROL_FREQ_HZ)
							  + ((int32_t)(location_interp.est_speed  << 6) - (int32_t)(location_interp.est_speed))
							 );
	location_interp.est_speed 	   = (location_interp.est_speed_mut >> 6);							//(对64取整)(向0取整)(保留符号位)
	location_interp.est_speed_mut  = (location_interp.est_speed_mut - (location_interp.est_speed << 6));	//(对64取余)(向0取整)(保留符号位)
	//估计源信号位置
	location_interp.est_location = location_interp.record_location;

	//输出
	location_interp.go_location = location_interp.est_location;
	location_interp.go_speed = location_interp.est_speed;
}



