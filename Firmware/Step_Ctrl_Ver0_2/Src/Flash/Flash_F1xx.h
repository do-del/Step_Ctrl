#ifndef FLASH_F1XX_H
#define FLASH_F1XX_H

#include "stm32f1xx_hal.h"

#define STOCKPILE_APP_CALI_ADDR (0x08017C00)
#define STOCKPILE_APP_CALI_SIZE (0x00008000)	//32K

#define		STOCKPILE_APP_DATA_ADDR					(0x0801FC00)		//起始地址
#define		STOCKPILE_APP_DATA_SIZE					(0x00000400)		//Flash容量     1K

#define Stockpile_Page_Size		0x400U		//扇区大小(默认1024字节)
#if (Stockpile_Page_Size != FLASH_PAGE_SIZE)	//和HAL库获取的Flash页大小比较,检查配置是否有效
	#error "Stockpile_Page_Size Error !!!"		
#endif

/**
* Flash分区表结构体
**/
typedef struct{
	//配置
	uint32_t	begin_add;			//起始地址
	uint32_t	area_size;			//区域大小
	uint32_t	page_num;				//芯片实体页数量
	//过程量
	uint32_t	asce_write_add;	//写地址
}Stockpile_FLASH_Typedef;

extern Stockpile_FLASH_Typedef stockpile_quick_cali;
extern Stockpile_FLASH_Typedef stockpile_data;

#define Stockpile_Flash_Unlock()	{WRITE_REG(FLASH->KEYR, FLASH_KEY1);WRITE_REG(FLASH->KEYR, FLASH_KEY2);}
#define Stockpile_Flash_Lock()	{SET_BIT(FLASH->CR, FLASH_CR_LOCK);}

inline uint16_t Stockpile_Flash_Read_Data16(uint32_t faddr)
{
  return *(volatile uint16_t*)faddr;
}

void Stockpile_Flash_Data_Empty(Stockpile_FLASH_Typedef *stockpile);			//Flash数据清空
void Stockpile_Flash_Data_Begin(Stockpile_FLASH_Typedef *stockpile);			//Flash数据开始写入
void Stockpile_Flash_Data_End(Stockpile_FLASH_Typedef *stockpile);				//Flash数据结束写入
void Stockpile_Flash_Data_Set_Write_Add(Stockpile_FLASH_Typedef *stockpile, uint32_t write_add);					//Flash设置写地址
void Stockpile_Flash_Data_Write_Data16(Stockpile_FLASH_Typedef *stockpile, uint16_t *data, uint32_t num);	//Flash_16位数据写入
void Stockpile_Flash_Data_Write_Data32(Stockpile_FLASH_Typedef *stockpile, uint32_t *data, uint32_t num);	//Flash_32位数据写入
void Stockpile_Flash_Data_Write_Data64(Stockpile_FLASH_Typedef *stockpile, uint64_t *data, uint32_t num);	//Flash_64位数据写入

#endif
