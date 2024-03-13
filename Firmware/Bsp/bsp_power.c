#include "bsp_power.h"
#include "bsp_key.h"
#include "bsp_led.h"

void bsp_powerOFF()
{
	
	Output3V3_DISABLE();
//	OutputBATT_DISABLE();
	LED_MODE = LED0constOFF;
}
void mcu_standby()
{
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);
	HAL_PWR_EnterSTANDBYMode();
}
void power_key_handle_20ms()
{
	static uint8_t step=0;
	static uint8_t cnt = 0;
	static bool key0LonClicFlag_pre=0;
	switch(step)
	{
		case 0:
			if(key0LonClicFlag != key0LonClicFlag_pre)//按键长按标志
			{	
				key0LonClicFlag_pre = key0LonClicFlag;//清除标志位
				bsp_powerOFF();
				step++;
			}
			break;
		case 1:
			if(key0Val == 0)//按键松开
			{
				step++;
			}
			break;
		case 2:
			cnt++;
			if(cnt > 5)//100ms
			{
				mcu_standby();
			}
			break;
	}
}

void battery_mon()
{
	
}









