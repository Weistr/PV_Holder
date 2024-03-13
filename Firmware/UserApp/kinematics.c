#include "kinematics.h"
#include "math.h"
#include "xdrivecom.h"

/**
 * 加减速波形发生器
*/
void accMotiom_Init(accMotionTypedef* accType)
{
    #define accp accType->accp
    #define accn  accType->accn
    #define spdPss  accType->spdPss
    #define route  accType->route
    float vmax_acchive;
    //三角速度
     vmax_acchive = sqrtf(2.0f*route*accp*accn/(accp+accn));
    if (vmax_acchive <= spdPss)//三角速度
    {
        accType->tpoint_accn = (2.0f*route/vmax_acchive)*accn/(accp+accn);//减速转折时间点
        accType->xpoint_accn = vmax_acchive*vmax_acchive/(2.0f*accp);//减速转折位置点
        accType->xLen_accn = vmax_acchive*vmax_acchive/(2.0f*accn);
        accType->T_total = vmax_acchive/accp + vmax_acchive/accn;

        accType->flag_needConstV=0;//无需匀速
    }
    else//梯形速度
    {
        accType->tpoint_const = (float)spdPss/accp;//匀速转折时间点
        accType->xpoint_const =  spdPss*spdPss/(2.0f*accp);//匀速转折位置点
		accType->xpoint_accn = route-spdPss*spdPss/(2.0f*accn); 	//减速转折位置点  
        accType->xLen_accn = spdPss*spdPss/(2.0f*accn);
        accType->tpoint_accn = (float)route/spdPss-spdPss/(2.0f*accn)-spdPss/(2.0f*accp) + accType->tpoint_const;//减速转折时间点
		accType->T_total = accType->tpoint_accn + (float)spdPss/accn;
        accType->flag_needConstV=1;//需要匀速
    }
    accType->vmax_acchive = vmax_acchive;
}

uint8_t accMotion_Cal(accMotionTypedef* accType,float t)
{
    float tt = t*t;
	#define flag_needConstV accType->flag_needConstV
    #define tpoint_const 	accType->tpoint_const
		float outPos=0;
		float outSpeed=0;
    //#define outPos accType->outPos
    //#define outSpeed accType->outSpeed
    #define tpoint_accn accType->tpoint_accn
    #define xpoint_accn accType->xpoint_accn
    #define xpoint_const accType->xpoint_const
    #define T_total accType->T_total
    #define xLen_accn accType->xLen_accn
    #define vmax_acchive accType->vmax_acchive
    if (flag_needConstV == 1)//梯形加速
    {
        if (t< tpoint_const)//加速
        {
            outPos = (accp)*tt/2;
            outSpeed = t*accp;
        }
        else if(t< tpoint_accn)//匀速
        {
            outPos = xpoint_const + spdPss*(t- tpoint_const);
            outSpeed = spdPss;
        }
        else//减速
        {
            float tleast = t- T_total;
            outPos = xpoint_accn + xLen_accn - accn*tleast*tleast/2;
            outSpeed = spdPss-((t-tpoint_accn)*accn);
						if((outPos >= route) || (outSpeed <=0))return 1;
        }
    }
    else if (flag_needConstV == 0)//三角加速
    {
         if (t<tpoint_accn)//加速
        {
            outPos = accp*tt/2;
            outSpeed = t*accp;
        }      
        else//减速
        {
            float tleast = t-T_total;
            outPos = xpoint_accn + xLen_accn - accn*tleast*tleast/2;
            outSpeed = vmax_acchive-((t-tpoint_accn)*accn);
						if((outPos >= route) || (outSpeed <=0))return 1;
        } 
    }
		accType->outPos = outPos;
		accType->outSpeed = outSpeed;
		return 0;
}


/**
 * 底盘运动学
*/
chassisMoveL_Typedef chassis_Struct;
float cosRad,sinRad;


float route_offset=0;

void chassis_moveL_Start()
{
    
    float rad = degToRad(chassis_Struct.deg);
    cosRad = cosf(rad);
    sinRad = sinf(rad);
    chassis_Struct.t = 0;
    chassis_StartMove_Bit = 1;
    accMotiom_Init(&chassis_Struct.accKeinematic);

	
}
float mToXdrive =203718.327f;
int32_t chassis_motor_offset[4];
uint8_t chassis_moveL_Cal()
{
    
    if(accMotion_Cal(&chassis_Struct.accKeinematic, chassis_Struct.t) == 1)//已经到达
    {
        return 1;
    }
    //chassis_Struct.pos += chassis_Struct.accKeinematic.outPos;//底盘位置累加


    float posOut_m_cosRad = chassis_Struct.accKeinematic.outPos *cosRad;
    float posOut_m_sinRad = chassis_Struct.accKeinematic.outPos *sinRad;


    xdriveMultiPos_pos[0] = (posOut_m_cosRad + posOut_m_sinRad)*mToXdrive + chassis_motor_offset[0];
    xdriveMultiPos_pos[1] = (-posOut_m_cosRad + posOut_m_sinRad)*mToXdrive + chassis_motor_offset[1];
    xdriveMultiPos_pos[2] = (-posOut_m_cosRad - posOut_m_sinRad)*mToXdrive + chassis_motor_offset[2];
    xdriveMultiPos_pos[3] = (posOut_m_cosRad - posOut_m_sinRad)*mToXdrive + chassis_motor_offset[3];
	return 0;
}


/**
 * 机械臂运动
*/

arm20sffTypedef arm20sffStruct;

void arm20sff_ik(arm20sffTypedef* st)
{
    float x = st->tXYZ.x;
    float y = st->tXYZ.y;
    float z = st->tXYZ.z;
    
    float l1 = st->armval.j1L;
    float l2 = st->armval.j2L;

    float a = sqrtf(x*x + y*y);
    float c = sqrtf(a*a);

    float th2 = acosf((l1*l1 + l2*l2 - c*c)/(2.0f*l1*l2));
    float th3 = asinf(l2*sin(th2)/(float)c);
    float th4 = acosf((c*c + a*a -z*z)/(2*a*c));
    float th1 = th3+th4;
    float th5 = acosf((x*x +a*a - y*y)/(2*a*x));


}
void arm20sff_moveJ()
{

}