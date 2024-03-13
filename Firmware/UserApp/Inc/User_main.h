#ifndef _USER_MAIN_H
#define _USER_MAIN_H
#include "main.h"

#define EventBit_SysInitOK   0x01
#define EventBit_UsbCmd1 0x01 << 1
#define EventBit_UsbCmd2 0x01 << 2
#define EventBit_UsbCmd3 0x01 << 3
#define EventBit_UsbCmd4 0x01 << 4

#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)		) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )
void user_main(void);

#endif
