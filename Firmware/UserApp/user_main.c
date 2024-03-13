#include "user_main.h"
#include "usart.h"
//OS

//bsp
#include "bsp_key.h"
#include "bsp_led.h"
#include "bsp_power.h"
#include "bsp_adc.h"
#include "bsp_uart.h"
//module

#include "xdrivecom.h"

//app
#include "kinematics.h"
#include "chassis.h"
#include "robot_behavior.h"
/*********************事件************************/
/*********************信号量************************/
/***************************************************/
/*********************TASK************************/
/***************************************************/
//========================================================================
// 函数: basic  task
// 描述: .
// 参数: None.
// 返回: None.
//========================================================================
void basicTask_entry(void *argument)
{

		led_sta_update_20ms();
		
		bsp_keyScan_20ms();
		
	//	power_key_handle_20ms();
		
		adc_handle_20ms();	
}



/***************************************************/
/*********************初始化************************/
/***************************************************/



void user_main()
{
	//taskENTER_CRITICAL();
	Output3V3_ENABLE();
	
	adc_init();		
	uart1_init();
	uart2_init();
	uart3_init();
}









