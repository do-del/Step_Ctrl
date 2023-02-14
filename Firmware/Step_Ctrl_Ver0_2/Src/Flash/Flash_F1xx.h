#ifndef FLASH_F1XX_H
#define FLASH_F1XX_H

#include "stm32f1xx_hal.h"

#define STOCKPILE_APP_CALI_ADDR (0x08017C00)
#define STOCKPILE_APP_CALI_SIZE (0x00008000)	//32K

#define		STOCKPILE_APP_DATA_ADDR					(0x0801FC00)		//��ʼ��ַ
#define		STOCKPILE_APP_DATA_SIZE					(0x00000400)		//Flash����     1K

#define Stockpile_Page_Size		0x400U		//������С(Ĭ��1024�ֽ�)
#if (Stockpile_Page_Size != FLASH_PAGE_SIZE)	//��HAL���ȡ��Flashҳ��С�Ƚ�,��������Ƿ���Ч
	#error "Stockpile_Page_Size Error !!!"		
#endif

/**
* Flash������ṹ��
**/
typedef struct{
	//����
	uint32_t	begin_add;			//��ʼ��ַ
	uint32_t	area_size;			//�����С
	uint32_t	page_num;				//оƬʵ��ҳ����
	//������
	uint32_t	asce_write_add;	//д��ַ
}Stockpile_FLASH_Typedef;

extern Stockpile_FLASH_Typedef stockpile_quick_cali;
extern Stockpile_FLASH_Typedef stockpile_data;

#define Stockpile_Flash_Unlock()	{WRITE_REG(FLASH->KEYR, FLASH_KEY1);WRITE_REG(FLASH->KEYR, FLASH_KEY2);}
#define Stockpile_Flash_Lock()	{SET_BIT(FLASH->CR, FLASH_CR_LOCK);}

inline uint16_t Stockpile_Flash_Read_Data16(uint32_t faddr)
{
  return *(volatile uint16_t*)faddr;
}

void Stockpile_Flash_Data_Empty(Stockpile_FLASH_Typedef *stockpile);			//Flash�������
void Stockpile_Flash_Data_Begin(Stockpile_FLASH_Typedef *stockpile);			//Flash���ݿ�ʼд��
void Stockpile_Flash_Data_End(Stockpile_FLASH_Typedef *stockpile);				//Flash���ݽ���д��
void Stockpile_Flash_Data_Set_Write_Add(Stockpile_FLASH_Typedef *stockpile, uint32_t write_add);					//Flash����д��ַ
void Stockpile_Flash_Data_Write_Data16(Stockpile_FLASH_Typedef *stockpile, uint16_t *data, uint32_t num);	//Flash_16λ����д��
void Stockpile_Flash_Data_Write_Data32(Stockpile_FLASH_Typedef *stockpile, uint32_t *data, uint32_t num);	//Flash_32λ����д��
void Stockpile_Flash_Data_Write_Data64(Stockpile_FLASH_Typedef *stockpile, uint64_t *data, uint32_t num);	//Flash_64λ����д��

#endif
