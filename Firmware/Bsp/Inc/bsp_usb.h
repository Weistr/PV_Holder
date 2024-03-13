#ifndef _BSP_USB_H 
#define _BSP_USB_H

#include "main.h"

uint8_t usb_userCMD_handle(uint16_t len);
void usb_printf(uint8_t *str);
void usb_txfinish_handle(void);

extern uint8_t UsbReceivedLen;
extern uint8_t UserRxBufferFS[];
extern uint8_t UserTxBufferFS[];

#endif
