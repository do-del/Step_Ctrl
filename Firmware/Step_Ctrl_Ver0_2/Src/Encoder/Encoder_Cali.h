#ifndef ENCODER_CALI_H
#define ENCODER_CALI_H

#include "MT6816.h"
#include "AT8236.h"
#include "Control_Config.h"

//校准数据检查报告
typedef enum{
	//无错误
	CALI_No_Error = 0x00,						//数据无错误
	//原始数据出错
	CALI_Error_Average_Dir,					//平均值方向错误
	CALI_Error_Average_Continuity,	//平均值连续性错误
	CALI_Error_PhaseStep,						//阶跃次数错误
	//解析数据出错
	CALI_Error_Analysis_Quantity,		//解析数据量错误
}CALI_Error;

//校准状态
typedef enum{
	CALI_Disable = 0x00,						//失能
	CALI_Forward_Encoder_AutoCali,	//编码器正转自动校准
	CALI_Forward_Measure,						//正向测量
	CALI_Reverse_Ret,								//反向回退
	CALI_Reverse_Gap,								//反向消差
	CALI_Reverse_Measure,						//反向测量
	CALI_Operation,									//解算
}CALI_State;

/**
  * Control校准结构体定义
**/
typedef struct{
	//信号
	uint8_t			trigger;			//触发校准
	CALI_Error	error_code;		//校准错误代码
	uint32_t		error_data;		//校准错误数据
	//读取过程
	CALI_State	state;					//校准状态
	uint32_t		out_location;		//输出位置
	#define			Gather_Quantity	16	//每个采集点采集数量
	uint16_t		gather_count;												//采集计数
	uint16_t		coder_data_gather[Gather_Quantity];	//采集点每次数据
	uint16_t 		coder_data_f[Move_Step_NUM+1];	//校准读取的数据
	uint16_t 		coder_data_r[Move_Step_NUM+1];	//校准读取的数据
	uint8_t			dir;		//校准数据方向
	//全段域校准过程数据
	int32_t		rcd_x, rcd_y;			//寻找区间下标及阶跃差值
	uint32_t	result_num;				//统计数量
}Encode_Cali_Typedef;
extern Encode_Cali_Typedef encode_cali;

void Calibration_Init(void);								//校准器初始化
void Calibration_Interrupt_Callback(void);	//校准器中断回调(稳定中断调用)
void Calibration_Loop_Callback(void);				//校准器主程序回调(非中断调用)

#endif
