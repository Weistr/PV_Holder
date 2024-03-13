#ifndef _BSP_POWER_H
#define _BSP_POWER_H
#include "main.h"

#define Output3V3_ENABLE() HAL_GPIO_WritePin(EN_3V3_GPIO_Port, EN_3V3_Pin, GPIO_PIN_RESET)
#define Output3V3_DISABLE() HAL_GPIO_WritePin(EN_3V3_GPIO_Port, EN_3V3_Pin, GPIO_PIN_SET)
#define OutputBATT_ENABLE() HAL_GPIO_WritePin(EN_BTO_GPIO_Port, EN_BTO_Pin, GPIO_PIN_SET)
#define OutputBATT_DISABLE() HAL_GPIO_WritePin(EN_BTO_GPIO_Port, EN_BTO_Pin, GPIO_PIN_RESET)
#define readBATT_ENpin() HAL_GPIO_ReadPin(EN_BTO_GPIO_Port, EN_BTO_Pin)

#define BATT_LOW_PROTECTION 0

void power_key_handle_20ms(void);
void bsp_powerOFF(void);
void mcu_standby(void);

#endif
