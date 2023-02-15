#include "MT6816.H"
#include "stdio.h"

uint16_t *Read_QuickCali_DATA = (uint16_t*)STOCKPILE_APP_CALI_ADDR;

MT6816_SPI_Signal_Typedef	mt6816_spi;

/**
  * @brief  MT6816_SPI采集初始化
  * @param  NULL
  * @retval NULL
**/
void MT6816_SPI_Signal_Init(void)
{
	//采集数据
	mt6816_spi.sample_data = 0;
	//输出数据
	mt6816_spi.angle = 0;
	
	//配置外设
	MT6816_SPI_CS_H();
}

/**
  * @brief  MT6816_SPI采集获取角度数据
  * @param  NULL
  * @retval NULL
**/
void MT6816_SPI_Get_AngleData(void)
{
	uint16_t data_t[2];
	uint16_t data_r[2];
	uint8_t h_count;
	
	data_t[0] = (0x80 | 0x03) << 8;
	data_t[1] = (0x80 | 0x04) << 8;
	
	for(uint8_t i=0; i<3; i++){
		//读取SPI数据
		MT6816_SPI_CS_L();
		HAL_SPI_TransmitReceive(&MT6816_SPI, (uint8_t*)&data_t[0], (uint8_t*)&data_r[0], 1, HAL_MAX_DELAY);
		MT6816_SPI_CS_H();
		MT6816_SPI_CS_L();
		HAL_SPI_TransmitReceive(&MT6816_SPI, (uint8_t*)&data_t[1], (uint8_t*)&data_r[1], 1, HAL_MAX_DELAY);
		MT6816_SPI_CS_H();
		mt6816_spi.sample_data = ((data_r[0] & 0x00FF) << 8) | (data_r[1] & 0x00FF);
		
		//奇偶校验
		h_count = 0;
		for(uint8_t j=0; j<16; j++){
			if(mt6816_spi.sample_data & (0x0001 << j))
				h_count++;
		}
		if(h_count & 0x01){
			mt6816_spi.pc_flag = 0;
		}
		else{
			mt6816_spi.pc_flag = 1;
			break;
		}
	}
	
	if(mt6816_spi.pc_flag){
		mt6816_spi.angle = mt6816_spi.sample_data >> 2;
		mt6816_spi.no_mag_flag = (mt6816_spi.sample_data & (0x0001 << 1));
	}
}

MT6816_Typedef	mt6816;

/**
  * @brief  MT6816初始化
  * @param  NULL
  * @retval NULL
**/
void MT6816_Init(void)
{
	MT6816_SPI_Signal_Init();		//初始化SPI接口

	//初始化阶段获取一次角度数据(过滤错误数据)(暂未查明复位后第一次读取数据丢失的原因)
	MT6816_Get_AngleData();
	delay(10);
	MT6816_Get_AngleData();
	delay(10);
	MT6816_Get_AngleData();
	delay(10);
	MT6816_Get_AngleData();
	delay(10);
	MT6816_Get_AngleData();
	
	//检查校准区数据是否有效
	mt6816.rectify_valid = 1;
	for(uint16_t i=0; i<(MT6816_Encoder_Res); i++){
		if(Read_QuickCali_DATA[i] == 0xFFFF)
		{
			mt6816.rectify_valid = 0;
			return;
		}
	}
	printf("Flash data is valid!\r\n");
}

/**
  * @brief  MT6816获取角度数据
  * @param  NULL
  * @retval NULL
**/
void MT6816_Get_AngleData(void)
{
	MT6816_SPI_Get_AngleData();
	mt6816.angle_data = mt6816_spi.angle;
	mt6816.rectify_angle = Read_QuickCali_DATA[mt6816.angle_data];
}
