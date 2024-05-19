#include "pid.h"
PidObject pitMotorPidSt; //pitch轴电机pid结构体
PidObject yawMotorPidSt;//yaw轴电机pid结构体


/**************************************************************
 *批量复位PID函数
 * @param[in] 
 * @param[out] 
 * @return     
 ***************************************************************/	
void pidRest(PidObject **pid,const uint8_t len)
{
	uint8_t i;
	for(i=0;i<len;i++)
	{
	  	pid[i]->integ = 0;
	    pid[i]->prevError = 0;
	    pid[i]->out = 0;
		pid[i]->offset = 0;
	}
}

/**************************************************************
 * Update the PID parameters.
 *
 * @param[in] pid         A pointer to the pid object.
 * @param[in] measured    The measured value
 * @param[in] updateError Set to TRUE if error should be calculated.
 *                        Set to False if pidSetError() has been used.
 * @return PID algorithm output
 ***************************************************************/	
void pidUpdate(PidObject* pid,const float dt)
{
	 float error;
	 float deriv;
	
    error = pid->desired - pid->measured; //当前角度与实际角度的误差

    pid->integ += error * dt;	 //误差积分累加值
	
		if(pid->integ > pid->ilimt)//进行积分限幅
			pid->integ = pid->ilimt;
		if(pid->integ < (-pid->ilimt))
			pid->integ = -(pid->ilimt);
		
    deriv = (error - pid->prevError)/dt;  //前后两次误差做微分
	
    pid->out = pid->kp * error + pid->ki * pid->integ + pid->kd * deriv;//PID输出
	
		
    pid->prevError = error;  //更校?律洗蔚奈蟛?
		
}




void pid_param_Init(void)
{
//默认pid参数/////////////////////////////////////////////	
	pitMotorPidSt.kp = 5.1f;
	pitMotorPidSt.ki = 0.0f;
	pitMotorPidSt.kd = 0.24f;
	pitMotorPidSt.ilimt = 30.0f;

	yawMotorPidSt.kp = 5.1f;
	yawMotorPidSt.ki = 0.0f;
	yawMotorPidSt.kd = 0.24f;
	yawMotorPidSt.ilimt = 30.0f;

}

