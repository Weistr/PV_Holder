#ifndef _XDRIVECOM_H
#define _XDRIVECOM_H
#include "user_main.h"
#include "stdbool.h"
void motorPosControl(uint8_t id, int32_t pos, int32_t speed,uint8_t update);
void motorSpeedControl(uint8_t id, int32_t speed,uint8_t update);
void motorCurrControl(uint8_t id, int16_t Curr);
void motorStop(uint8_t id);
void motorRcvStatusUpdate(uint8_t id);
void motorStartMotion(uint8_t id);
void xdrive_PosControl_multi(void);
void XdriveRcvTask(void);
#define motor_id_universal 0x00//Õ®”√id
#define motor_no_update 0
#define motor_update_now 1
#define  totalMotorNum  4




extern int32_t xdriveMultiPos_pos[totalMotorNum];
extern int32_t xdriveMultiPos_spd[totalMotorNum];
extern int32_t xdrivePosRcv[totalMotorNum];
extern int32_t xdriveSpdRcv[totalMotorNum];
extern uint8_t xdriveMultiPos_flag;
extern uint8_t xdriveMultiPos_cnt;
extern uint8_t xdriveMultiPos_finish ;
extern uint8_t xdrive_mutiControl_mode;

#endif
