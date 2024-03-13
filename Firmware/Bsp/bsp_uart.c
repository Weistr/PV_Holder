#include "usart.h"
#include "bsp_uart.h"
#include "bsp_delay.h"


#include "xdrivecom.h"

//========================================================================
// 函数: 
// 描述: 串口相关.
// 参数: None.
// 返回: None.
//========================================================================
#ifdef use_uart1
uint8_t uart1_rx_buf[64];
uint8_t uart1_tx_buf[64];
uint8_t uart1_frame_data_len;
uint16_t uart1_timeout_cnt;
void uart1_init()
{
	
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1,uart1_rx_buf,64);
	
	__HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);
}

#endif
#ifdef use_uart2
uint8_t uart2_rx_buf[64];
uint8_t uart2_tx_buf[64];
uint8_t uart2_frame_data_len;
uint16_t uart2_timeout_cnt;
void uart2_init()
{
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2,uart2_rx_buf,64);
	__HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE);
}
#endif
#ifdef use_uart3
uint8_t uart3_rx_buf[64];
uint8_t uart3_tx_buf[64];
uint8_t uart3_frame_data_len;
uint16_t uart3_timeout_cnt;
void uart3_init()
{
	HAL_UARTEx_ReceiveToIdle_DMA(&huart3,uart3_rx_buf,64);
	__HAL_UART_ENABLE_IT(&huart3,UART_IT_IDLE);
}
#endif

//串口接收完成中断回调函数
uint8_t uartid =0;
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{

	#ifdef use_uart1
	if (huart->Instance == USART1)
	{
 		HAL_UARTEx_ReceiveToIdle_DMA(huart, uart1_rx_buf, 64);
		uart1_frame_data_len = Size;
		uart1_timeout_cnt=0;
		uartid =1;
		XdriveRcvTask();
	}
	#endif
	#ifdef use_uart2
	if (huart->Instance == USART2)
	{
		HAL_UARTEx_ReceiveToIdle_DMA(huart, uart2_rx_buf, 64);
		uart2_frame_data_len = Size;
		uart2_timeout_cnt=0;
		uartid=2;
	}	
	#endif
	#ifdef use_uart3
	if (huart->Instance == USART3)
	{
		HAL_UARTEx_ReceiveToIdle_DMA(huart, uart3_rx_buf, 64);
		uart3_frame_data_len = Size;
		uart3_timeout_cnt=0;
		uartid=3;
	}	
	#endif

}












