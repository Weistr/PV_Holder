#include "bsp_tim.h"
#include "tim.h"
#include "bsp_adc.h"
#include "remote_data.h"
#include "bsp_key.h"
#include "bsp_led.h"
#include "bsp_power.h"
#include "ECB02S2_BLE.h"
#include "usart.h"
#include "string.h"
#include "bsp_uart.h"
//========================================================================
// 函数: pwm_init
// 描述: pwm初始化.
// 参数: None.
// 返回: None.
//========================================================================
void bsp_timInit()
{
	//切记!!!!
	//一次只能配置1个通道!!!!
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
	__HAL_TIM_SetAutoreload(&htim4,999);
	__HAL_TIM_PRESCALER(&htim4,71);
	__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,0);
	HAL_TIM_Base_Start_IT(&htim1);

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		static uint8_t cnt=0;
		static uint8_t flag=0;
		cnt++;
		if(cnt > 255)cnt=0;
		//10ms
		adc_handle_10ms();//adc采样滤波
		//20ms
		if(cnt % 2 == 0)
		{
			led_sta_update_20ms();
			bsp_keyScan_20ms();
			power_key_handle_20ms();
			RC_liner_data_handle();//遥控器数据处理
		}
		if(cnt % 5 == 0)
		{
			if(IS_BLE_Connected())
			{
				if(flag==0)
				{
					flag =1;
					__HAL_TIM_DISABLE(&htim1);
					HAL_Delay(2000);
					__HAL_TIM_ENABLE(&htim1);
				}
				
				uint8_t i=0;
				uint16_t sum=0;
				uint8_t* pbuf = (uint8_t*)&RC_Control.THROTTLE;
				for(i=0;i<12;i++)
					uart2_tx_buf[i] = pbuf[i];
				
//				strlcpy((char*)uart2_tx_buf,(char*)&RC_Control.THROTTLE,12);
				for(i=0;i<12;i++)
				{
					sum += uart2_tx_buf[i];
				}
				uart2_tx_buf[i] = sum&0xFF;
				HAL_UART_Transmit_DMA(&huart2,uart2_tx_buf,13);
			}
			else
			{
				flag=0;
			}
			
		}
	}
}
