#include "Del_Hal.h"
#include <math.h>

#define BUTTON_UP_GPIO_Port 			(GPIOA)
#define BUTTON_UP_Pin					(GPIO_PIN_15)

#define BUTTON_DOWN_GPIO_Port			(GPIOB)
#define BUTTON_DOWN_Pin					(GPIO_PIN_2)

float kp=15.0;     
float ki=0.2;  
float kd=200; 

const float LPFA=0.9630; 
const float LPFB=0.0370;

const uint8_t UMAXCL=60;   
const uint8_t UMAXOP=30; 

float r=0.0;   //setpoint
float r_1=0.0;   //setpoint
uint8_t dir=1; 
float y=0.0;   // measured angle
float y_1=0.0;
float yw=0.0;  // "wrapped" angle (not limited to 0-360)
float yw_1=0.0;
int32_t wrap_count=0; 
float e=0.0;   // e = r-yw (error)
float p=0.0;   // proportional effort
float i=0.0;   // integral effort
float iterm=0;
float dterm=0;
float u=0.0;     //real control effort (not abs)
int32_t stepnumber=0;
float stepangle=0.0;

uint16_t hccount=0;
uint8_t closemode;
uint8_t enmode=1;

static void SetModeCheck(void);
static void OneStep(void);
static void CalibrateEncoder(void);

void Del_Init(void)
{
	HAL_Delay(100);
	Led_Init();
	MT6816_Init();
	
	if(mt6816.rectify_valid)
	{
		printf("encoder data is valid!\r\n");
	}
	else
	{
		printf("encoder data is invalid!\r\n");
	}
	
  HW_Elec_Init();
	SetModeCheck();
	HAL_TIM_Base_Start_IT(&htim3);
}

void Del_Update(void)
{

	//printf("e:%f-u:%f\r\n",e,u);
	HAL_Delay(10);
	if(HAL_GPIO_ReadPin(BUTTON_UP_GPIO_Port,BUTTON_UP_Pin)==GPIO_PIN_RESET)
	{
		HAL_Delay(20);
		while(HAL_GPIO_ReadPin(BUTTON_UP_GPIO_Port,BUTTON_UP_Pin)==GPIO_PIN_RESET);
		r += 200;
	}
	if(HAL_GPIO_ReadPin(BUTTON_DOWN_GPIO_Port,BUTTON_DOWN_Pin)==GPIO_PIN_RESET)
	{
		HAL_Delay(20);
		while(HAL_GPIO_ReadPin(BUTTON_DOWN_GPIO_Port,BUTTON_DOWN_Pin)==GPIO_PIN_RESET);
		r -= 200;
	}
}

int fputc(int ch,FILE *f)
{
	HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,0xFFFF);
	return (ch);
}

void SetModeCheck(void)
{
	
	MT6816_Get_AngleData();
	if(mt6816.state)
	{
		for(uint8_t m=0;m<6;m++)
		{
			GPIO_TOGGLE(Green_Led_Port,Green_Led_Pin);
			HAL_Delay(200);	
		} 
	}	
	if(HAL_GPIO_ReadPin(BUTTON_DOWN_GPIO_Port, BUTTON_DOWN_Pin) == GPIO_PIN_RESET)  
	{
		CalibrateEncoder();
	}
	
	stepangle=1.8/32;

	if(1)
	{
		MT6816_Get_AngleData();
		closemode=1;
		r=mt6816.rectify_angle;
		r=r*360.0/16384;
		y=r;
		y_1=y;
		yw=y;  
		yw_1=yw;
	}
	else
	{
		closemode=0;
	}
}

void OneStep(void)
{          
  if(dir) 
    stepnumber+=1;
  else 
    stepnumber-=1;
  
  HW_Output(1.8*stepnumber,80);
  HAL_Delay(10);
}

void CalibrateEncoder(void) 
{   
	int32_t encoderReading=0;    
	int32_t currentencoderReading=0;
	int32_t lastencoderReading=0;        

	int32_t iStart=0;     //encoder zero position index
	int32_t jStart=0;
	int32_t stepNo=0;

	int32_t fullStepReadings[200];
	int32_t ticks=0;	

	uint16_t lookupAngle;
	uint32_t count = 0;
	
	dir=0; 
	HW_Output(0.0,80);
	for(uint8_t m=0;m<6;m++)
	{
		GPIO_TOGGLE(Red_Led_Port,Red_Led_Pin);
		HAL_Delay(200);	
	} 
	for(uint16_t x=0;x<200;x++) 
	{    
		encoderReading=0;
		MT6816_Get_AngleData();
		lastencoderReading=mt6816.angle_data;
		HAL_Delay(10);
		for(uint8_t reading=0;reading<10;reading++) 
		{ 
			MT6816_Get_AngleData();
			currentencoderReading=mt6816.angle_data;
			
			if(currentencoderReading-lastencoderReading<-8192)
				currentencoderReading+=16384;
			else if(currentencoderReading-lastencoderReading>8192)
				currentencoderReading-=16384;
			printf("angle:%d\r\n",currentencoderReading);
			encoderReading+=currentencoderReading;
			HAL_Delay(10);
			lastencoderReading=currentencoderReading;
		}
		encoderReading=encoderReading/10;
		if(encoderReading>16384)
			encoderReading-=16384;
		else if(encoderReading<0)
			encoderReading+=16384;
		fullStepReadings[x]=encoderReading;
		
		OneStep();
		HAL_Delay(3); 
	}
	
	Out_PWMtoDAC_A(0);  
	Out_PWMtoDAC_B(0); 

	for(uint8_t i=0;i<200;i++) 
	{
		ticks=fullStepReadings[(i+1)%200]-fullStepReadings[i%200];
		if(ticks<-15000){
			ticks+=16384;
		}
		else if(ticks>15000)
		{
			ticks-=16384;
		}
		for(int32_t j=0;j<ticks;j++) 
		{
			stepNo=(fullStepReadings[i]+j)%16384;
			if(stepNo==0) 
			{
				iStart=i;
				jStart=j;
			}
		}
	}
	
	Stockpile_Flash_Data_Empty(&stockpile_quick_cali);
	Stockpile_Flash_Data_Begin(&stockpile_quick_cali);
	for(int32_t i=iStart;i<(iStart+200+1);i++) 
	{
		ticks=fullStepReadings[(i+1)%200]-fullStepReadings[i%200];
		if(ticks<-15000) 
			ticks+=16384;
		if(i==iStart) 
		{
			for(int32_t j=jStart;j<ticks;j++) 
			{
				lookupAngle=(8192*i+8192*j/ticks)%1638400/100;
				count++;
				Stockpile_Flash_Data_Write_Data16(&stockpile_quick_cali,&lookupAngle, 1);
			}
		}
		else if(i==(iStart+200)) 
		{
			for(int32_t j=0;j<jStart;j++) 
			{
				lookupAngle=((8192*i+8192*j/ticks)%1638400)/100;
				count++;
				Stockpile_Flash_Data_Write_Data16(&stockpile_quick_cali,&lookupAngle, 1);
			}
		}
		else 
		{                        //this is the general case
			for(int32_t j=0;j<ticks;j++) 
			{
				lookupAngle=((8192*i+8192*j/ticks)%1638400)/100;
				count++;
				Stockpile_Flash_Data_Write_Data16(&stockpile_quick_cali,&lookupAngle, 1);
			}
		}
	}
	Stockpile_Flash_Data_End(&stockpile_quick_cali);
	
	printf("Cali Completed!\r\n");
	MT6816_Flash_Check();
	if(mt6816.rectify_valid)
	{
		printf("encoder data is valid!\r\n");
	}
	else
	{
		while(1)
		{
			printf("encoder data is invalid!\r\n");
			HAL_Delay(1000);
		}	
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == TIM3){
		if(enmode==1)
		{
			if(closemode==1) 
			{
				MT6816_Get_AngleData();
				y=mt6816.rectify_angle;
				y=y*360.0/16384;
				if(y-y_1>180.0) 
					wrap_count--;      
				else if(y-y_1<-180.0) 
					wrap_count++; 
				yw=y+360.0*wrap_count;             
				e=r-yw; 
				if((e>1.8)||(e<-1.8))
					Red_Led_On();
				else
					Red_Led_Off();			
				iterm+=ki*e;
				if(iterm>UMAXCL)
					iterm=UMAXCL;
				else if(iterm<-UMAXCL) 
					iterm=-UMAXCL;          
				
				dterm=LPFA*dterm-LPFB*kd*(yw-yw_1);          
				u=kp*e+iterm+dterm; 	

				y_1=y;  //copy current value of y to previous value (y_1) for next control cycle before PA angle added   
				yw_1=yw;

				if(u>0)             //You can experiment with "Phase Advance" by increasing PA when operating at high speeds
				{
					y+=1.5;
				}
				else 
				{
					y-=1.5;
					u=-u;
				}
				if(u>UMAXCL)     // limit control effort
					u=UMAXCL;       //saturation limits max current command	                  //digitalWrite(ledPin, LOW);
				
				HW_Output(-y,roundf(u));    // update phase currents
			}          
			else 
			{
				if(r==r_1)
				{
					hccount++;
					if(hccount>=1000)
						hccount=1000;
				}
				else
					hccount=0;

				if(hccount>=1000)
					HW_Output(r,UMAXOP/2);
				else
					HW_Output(r,UMAXOP);
				r_1=r;
			}
		}
	}
}
