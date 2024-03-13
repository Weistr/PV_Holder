#ifndef _BSP_UART_H
#define _BSP_UART_H
#include "main.h"

#define use_uart1 
#define use_uart2
#define use_uart3
#ifdef use_uart1
extern uint8_t uart1_rx_buf[64];
extern uint8_t uart1_tx_buf[64];
extern uint8_t uart1_frame_data_len;
extern uint16_t uart1_timeout_cnt;
void uart1_init(void);
#endif
#ifdef use_uart2
extern uint8_t uart2_rx_buf[64];
extern uint8_t uart2_tx_buf[64];
extern uint8_t uart2_frame_data_len;
extern uint16_t uart2_timeout_cnt;
void uart2_init(void);
#endif
#ifdef use_uart3
extern uint8_t uart3_rx_buf[64];
extern uint8_t uart3_tx_buf[64];
extern uint8_t uart3_frame_data_len;
extern uint16_t uart3_timeout_cnt;
void uart3_init(void);
#endif
#endif
