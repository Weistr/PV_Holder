#include "bsp_key.h"

//========================================================================
// 函数: bsp_keyScan_20ms 
// 描述: .
// 参数: None.
// 返回: None.
//========================================================================
bool key0Val=0;//按键按下置1，松开置0

bool key0OneClicFlag=0;//按键单击标志位  触发取反
bool key0TwoClicFlag=0;//按键双击标志位  触发取反
bool key0LonClicFlag=0;//按键长按标志位  触发取反
uint8_t clicTimeSpan_x20ms = 500/20;//双击时间间隔 默认500ms
uint8_t long_push_x20ms = 1500/20;//长按1000ms
uint8_t clicCnt=0;
void bsp_keyScan_20ms()
{
	static bool key0ValPrevious=0;
	static uint8_t timeOutCnt=0,push_time_x20ms=0;
	key0Val = HAL_GPIO_ReadPin(KEY0_GPIO_Port,KEY0_Pin);
	
	if(key0Val == 1)
	{
		 push_time_x20ms++;
		if(push_time_x20ms > long_push_x20ms)
		{
			push_time_x20ms = 0;
			key0LonClicFlag = !key0LonClicFlag;
		}
	}
	else
	{
		push_time_x20ms=0;
	}
	
	if(key0ValPrevious < key0Val)//按键按下
	{
		clicCnt++;
		timeOutCnt=0;
		key0OneClicFlag =! key0OneClicFlag;//单击
		if(clicCnt % 2 == 0)//双击
			key0TwoClicFlag =! key0TwoClicFlag;
		if(clicCnt % 3 == 0)//三击
		{}
		
	}
	else
	{
		timeOutCnt++;
		if(timeOutCnt > clicTimeSpan_x20ms)
		{
			timeOutCnt = 0;
			clicCnt = 0;
		}
	}
	key0ValPrevious = key0Val;
}

