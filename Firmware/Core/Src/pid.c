#include "pid.h"
PidObject pitMotorPidSt; //pitch����pid�ṹ��
PidObject yawMotorPidSt;//yaw����pid�ṹ��


/**************************************************************
 *������λPID����
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
	
    error = pid->desired - pid->measured; //��ǰ�Ƕ���ʵ�ʽǶȵ����

    pid->integ += error * dt;	 //�������ۼ�ֵ
	
		if(pid->integ > pid->ilimt)//���л����޷�
			pid->integ = pid->ilimt;
		if(pid->integ < (-pid->ilimt))
			pid->integ = -(pid->ilimt);
		
    deriv = (error - pid->prevError)/dt;  //ǰ�����������΢��
	
    pid->out = pid->kp * error + pid->ki * pid->integ + pid->kd * deriv;//PID���
	
		
    pid->prevError = error;  //��У?��ϴε���?
		
}




void pid_param_Init(void)
{
//Ĭ��pid����/////////////////////////////////////////////	
	pitMotorPidSt.kp = 5.1f;
	pitMotorPidSt.ki = 0.0f;
	pitMotorPidSt.kd = 0.24f;
	pitMotorPidSt.ilimt = 30.0f;

	yawMotorPidSt.kp = 5.1f;
	yawMotorPidSt.ki = 0.0f;
	yawMotorPidSt.kd = 0.24f;
	yawMotorPidSt.ilimt = 30.0f;

}

