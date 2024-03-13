#ifndef _KINEMATCS_H
#define _KINEMATCS_H
#include "User_main.h"

typedef struct
{
    float tpoint_accn;//减速转折时间点
    float tpoint_const;//匀速转折时间点
    float xpoint_accn;//减速转折位置点
    float xpoint_const;//匀速转折位置点
    float vmax_acchive;//能达到的最大速度
    float xLen_accn;//减速段位移
    float T_total;
    float accp,accn,spdPss,route;
    uint8_t flag_needConstV; 

    float outSpeed;
    float outPos;
    
}accMotionTypedef;
typedef struct 
{
    float x;
    float y;
    float z;
}fXYZTypedef;
typedef struct 
{
    float j1;
    float j2;
    float j3;
}jointdegTypedef;
typedef struct 
{
    float j1L;
    float j2L;
    float j3L;
    float j1offset;
    float j2offset;
    float j3offset;
}armvalTypedef;
typedef struct 
{
    float th1;
    float th2;
    float th5;
}ikoutTypedef;
/**底盘******/
typedef struct
{
    float deg;
    float t;
    float pos;
    uint8_t mod;
    accMotionTypedef accKeinematic;
}chassisMoveL_Typedef;
void chassis_moveL_Start(void);
uint8_t chassis_moveL_Cal(void);
extern uint8_t chassis_StartMove_Bit;
extern chassisMoveL_Typedef chassis_Struct;
/*******END**********/
typedef struct 
{
    fXYZTypedef tXYZ;
    jointdegTypedef ik_jointdeg;
    jointdegTypedef r_jointdeg;
    armvalTypedef armval;
    accMotionTypedef arm20sffACC;
}arm20sffTypedef;

void accMotiom_Init(accMotionTypedef* accType);

uint8_t accMotion_Cal(accMotionTypedef* accType,float t);


#define degToRad(N) (N*0.01745329252f)
#define pi 3.1415926535898f
#define pi_d2 1.5707963268f

extern arm20sffTypedef arm20sffStruct;
#endif
