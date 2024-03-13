#include "XdriveCom.h"
#include "bsp_uart.h"
#include "usart.h"
#include "string.h"
#include "crc.h"
#include "bsp_delay.h"

#include "stdbool.h"
uint8_t xdriveMultiPos_finish = 0;
/**
 * 电机位置速度控制
*/

void motorPosControl(uint8_t id, int32_t pos, int32_t speed,uint8_t update)
{
  memset(uart1_tx_buf, 0, 32);
  uart1_tx_buf[0] = id;   // id
	if(update == 0)
		uart1_tx_buf[1] = 0x20+0x08; // mod
	else
		uart1_tx_buf[1] = 0x20; // mod
  memcpy(&uart1_tx_buf[2], (uint8_t *)&pos, 4);
  memcpy(&uart1_tx_buf[6], (uint8_t *)&speed, 4);
  uint16_t crc = crc16Cal_soft((uint8_t *)&uart1_tx_buf[0], 10);
  uart1_tx_buf[10] = BYTE1(crc);
  uart1_tx_buf[11] = BYTE0(crc);
  HAL_UART_Transmit_DMA(&huart1,(uint8_t *)&uart1_tx_buf[0], 12);
}

void motorSpeedControl(uint8_t id, int32_t speed,uint8_t update)
{
  uart1_tx_buf[0] = id;
	if(update == 0)
		uart1_tx_buf[1] = 0x21+0x08; // mod
	else
		uart1_tx_buf[1] = 0x21; // mod
  uart1_tx_buf[2] = BYTE0(speed);
  uart1_tx_buf[3] = BYTE1(speed);
  uart1_tx_buf[4] = BYTE2(speed);
  uart1_tx_buf[5] = BYTE3(speed);
  int16_t crc = crc16Cal_soft((uint8_t *)&uart1_tx_buf[0], 6);
  uart1_tx_buf[6] = BYTE1(crc);
  uart1_tx_buf[7] = BYTE0(crc);
  HAL_UART_Transmit_DMA(&huart1,(uint8_t *)&uart1_tx_buf[0], 8);
}

void motorCurrControl(uint8_t id, int16_t Curr)
{
  uart1_tx_buf[0] = id;
  uart1_tx_buf[1] = 0x22;
  uart1_tx_buf[2] = BYTE0(Curr);
  uart1_tx_buf[3] = BYTE1(Curr);
  int16_t crc = crc16Cal_soft((uint8_t *)&uart1_tx_buf[0], 4);
  uart1_tx_buf[4] = BYTE1(crc);
  uart1_tx_buf[5] = BYTE0(crc);
  HAL_UART_Transmit_DMA(&huart1,(uint8_t *)&uart1_tx_buf[0], 6);
}
void motorStop(uint8_t id)
{
  uart1_tx_buf[0] = id;
  uart1_tx_buf[1] = 0x10;
  int16_t crc = crc16Cal_soft((uint8_t *)&uart1_tx_buf[0], 2);
  uart1_tx_buf[2] = BYTE1(crc);
  uart1_tx_buf[3] = BYTE0(crc);
  HAL_UART_Transmit_DMA(&huart1,(uint8_t *)&uart1_tx_buf[0], 4);
}

void motorStartMotion(uint8_t id)
{
  uart1_tx_buf[0] = id;
  uart1_tx_buf[1] = 0x02;
  int16_t crc = crc16Cal_soft((uint8_t *)&uart1_tx_buf[0], 2);
  uart1_tx_buf[2] = BYTE1(crc);
  uart1_tx_buf[3] = BYTE0(crc);
  HAL_UART_Transmit_DMA(&huart1,(uint8_t *)&uart1_tx_buf[0], 4);
}

int32_t xdriveMultiPos_pos[totalMotorNum];
int32_t xdriveMultiPos_spd[totalMotorNum];
int32_t xdrivePosRcv[totalMotorNum];
int32_t xdriveSpdRcv[totalMotorNum];
uint8_t xdriveMultiPos_flag = 0;
uint8_t xdriveMultiPos_cnt = 0;

void xdrive_PosControl_multi()
{
  xdriveMultiPos_flag = 1;
  xdriveMultiPos_cnt = 0;
  motorPosControl(xdriveMultiPos_cnt+1, xdriveMultiPos_pos[xdriveMultiPos_cnt], xdriveMultiPos_spd[xdriveMultiPos_cnt],motor_no_update);
}




/******XDRIVE接收*********/
uint8_t xdRcvFinsh_Falg = 0;


void XdriveRcvTask(void)
{
	uint8_t idRcv;
			
  int16_t crcCal = crc16Cal_soft(uart1_rx_buf,10);
  int16_t crcRcv = (uart1_rx_buf[10]<<8) + uart1_rx_buf[11];
  if(crcCal == crcRcv)
  {
    xdRcvFinsh_Falg = 1;
    idRcv = uart1_rx_buf[0];
    if(idRcv > totalMotorNum) return;
    memcpy(&xdrivePosRcv[idRcv-1],&uart1_rx_buf[2],4);
    memcpy(&xdriveSpdRcv[idRcv-1],&uart1_rx_buf[6],4);

    if (xdriveMultiPos_flag == 1)//多重电机控制
		{
			xdriveMultiPos_cnt ++;
			if(xdriveMultiPos_cnt >= totalMotorNum)//发送完成
			{
				delay_us(20);
				motorStartMotion(motor_id_universal);//更新电机
				xdriveMultiPos_cnt = 0;
				xdriveMultiPos_flag = 0;
				xdriveMultiPos_finish = 1;
			}
			else//没发完
			{
				delay_us(20);
				motorPosControl(xdriveMultiPos_cnt+1, xdriveMultiPos_pos[xdriveMultiPos_cnt],xdriveMultiPos_spd[xdriveMultiPos_cnt], motor_no_update);
			}
		}


    
  }
			
		
}
