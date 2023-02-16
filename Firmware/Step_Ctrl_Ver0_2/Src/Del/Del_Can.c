#include "Del_Can.h"
#include "stdio.h"

static CAN_TxHeaderTypeDef TxMessage; //CAN发送的消息的消息头
static CAN_RxHeaderTypeDef RxMessage; //CAN接收的消息的消息头

void CANFilter_Config(void)
{
	CAN_FilterTypeDef  sFilterConfig;

	sFilterConfig.FilterBank = 0;                       //CAN过滤器编号，范围0-27
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;   //CAN过滤器模式，掩码模式或列表模式
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;  //CAN过滤器尺度，16位或32位
	sFilterConfig.FilterIdHigh = 0x000 << 5;			//32位下，存储要过滤ID的高16位
	sFilterConfig.FilterIdLow = 0x0000;					//32位下，存储要过滤ID的低16位
	sFilterConfig.FilterMaskIdHigh = 0x0000;			//掩码模式下，存储的是掩码
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = 0;				//报文通过过滤器的匹配后，存储到哪个FIFO
	sFilterConfig.FilterActivation = ENABLE;    		//激活过滤器
	sFilterConfig.SlaveStartFilterBank = 0;

	if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK) 
	{
		Error_Handler();
	}
}

void CAN_Send_Test(void)
{
	uint32_t TxMailbox;
	uint8_t data[4] = {0x01, 0x02, 0x03, 0x04};
	TxMessage.IDE = CAN_ID_STD;     //设置ID类型
	TxMessage.StdId = 0x112;        //设置ID号
	TxMessage.RTR = CAN_RTR_DATA;   //设置传送数据帧
	TxMessage.DLC = 4;              //设置数据长度
	if (HAL_CAN_AddTxMessage(&hcan, &TxMessage, data, &TxMailbox) != HAL_OK)
	{
		Error_Handler();
	}	
}

void CAN_Start_Init(void)
{
	
	CANFilter_Config();
	
	if (HAL_CAN_Start(&hcan) != HAL_OK) 
	{
		Error_Handler();
	}

	/* 3. Enable CAN RX Interrupt */
	if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) !=  HAL_OK) {
		Error_Handler();
	}
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan_haldle)
{
	uint8_t  data[8];
	HAL_StatusTypeDef	status;

	if (hcan_haldle->Instance == CAN1) {	
		status = HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &RxMessage, data);
		if (HAL_OK == status){                              			
			printf("--->Data Receieve!\r\n");
			printf("RxMessage.StdId is %#x\r\n",  RxMessage.StdId);
			printf("data[0] is 0x%02x\r\n", data[0]);
			printf("data[1] is 0x%02x\r\n", data[1]);
			printf("data[2] is 0x%02x\r\n", data[2]);
			printf("data[3] is 0x%02x\r\n", data[3]);
			printf("<---\r\n");
				
		}
	}
}


