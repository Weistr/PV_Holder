#include "bsp_adc.h"
#include "adc.h"
#include "bsp_adc.h"
#include "bsp_power.h"


uint16_t u16adc_buf[5];
uint32_t u32adc_buf[5];
uint16_t battVot_mv ;
const uint16_t vref_mV = 1200;
uint16_t vref_adc = 1509;
void adc_init()
{
	HAL_ADCEx_Calibration_Start(&hadc1);
}
	uint8_t cnt1=0,cnt2=0,cnt3=50;

void adc_handle_20ms()
{
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)&u16adc_buf[0],2);
	
//vref
	if(cnt1 < 50)
	{
		cnt1++;
		u32adc_buf[0] += u16adc_buf[0];
	}
	else
	{
		vref_adc = u32adc_buf[0] / cnt1;
		u32adc_buf[0]=0;
		cnt1 = 0;
	}
//battery
	//if(readBATT_ENpin())//µÍµçÁ¿
	{	
		if(cnt2 < 20)
		{
			cnt2++;
			u32adc_buf[1] += u16adc_buf[1];
		}
		else
		{
			battVot_mv = ((u32adc_buf[1] / cnt2) * vref_mV / vref_adc) * 2;
			cnt2=0;
			u32adc_buf[1]=0;
		}

		
		if(battVot_mv < 1900)
		{
			if(cnt3 > 0)
			{
				cnt3--;
			}
			else
			{
				cnt3 = 20;//400ms
				#if BATT_LOW_PROTECTION == 1			
				bsp_powerOFF();
				mcu_standby();
				#endif
			}
		}
		else
		cnt3=10;
	}
}




