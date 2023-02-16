#include "Del_Can.h"
#include "stdio.h"

static CAN_TxHeaderTypeDef TxMessage; //CAN���͵���Ϣ����Ϣͷ
static CAN_RxHeaderTypeDef RxMessage; //CAN���յ���Ϣ����Ϣͷ

void CANFilter_Config(void)
{
	CAN_FilterTypeDef  sFilterConfig;

	sFilterConfig.FilterBank = 0;                       //CAN��������ţ���Χ0-27
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;   //CAN������ģʽ������ģʽ���б�ģʽ
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;  //CAN�������߶ȣ�16λ��32λ
	sFilterConfig.FilterIdHigh = 0x000 << 5;			//32λ�£��洢Ҫ����ID�ĸ�16λ
	sFilterConfig.FilterIdLow = 0x0000;					//32λ�£��洢Ҫ����ID�ĵ�16λ
	sFilterConfig.FilterMaskIdHigh = 0x0000;			//����ģʽ�£��洢��������
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = 0;				//����ͨ����������ƥ��󣬴洢���ĸ�FIFO
	sFilterConfig.FilterActivation = ENABLE;    		//���������
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
	TxMessage.IDE = CAN_ID_STD;     //����ID����
	TxMessage.StdId = 0x112;        //����ID��
	TxMessage.RTR = CAN_RTR_DATA;   //���ô�������֡
	TxMessage.DLC = 4;              //�������ݳ���
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


