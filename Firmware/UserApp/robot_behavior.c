#include "robot_behavior.h"
#include "kinematics.h"
#include "xdrivecom.h"
#include "math.h"
#include "string.h"
#include "bsp_uart.h"
#include "bsp_delay.h"


/**
 * robot init
 * 
*/
void robotInit_task(void *argument)
{ 
    for(;;)
    {    
        /**chssis*/
        chassis_Struct.t = 0;
        chassis_Struct.deg = 0;
        chassis_Struct.accKeinematic.accn = 0.4;
        chassis_Struct.accKeinematic.accp = 0.5;
        chassis_Struct.accKeinematic.spdPss = 0.5;
        chassis_Struct.accKeinematic.route = 0.4;

      
        /**arm*/
        arm20sffStruct.armval.j1L = 0.158f;
        arm20sffStruct.armval.j2L = 0.158f;

    }
}

/**
 * 200HZ心跳任务
*/


uint8_t chassis_StartMove_Bit = 0;
uint8_t robotLock = 1;
extern int32_t chassis_motor_offset[];


void robot_heart_task(void *argument)
{
	vTaskDelay(1500);
    for(;;)
    {
        if (robotLock == 1)
        {
            static uint8_t cnt=0;
            motorStop(cnt+1);
            cnt++;
            if (cnt >= totalMotorNum)cnt=0;
            chassis_motor_offset[cnt] = xdrivePosRcv[cnt];
						xdriveMultiPos_pos[cnt] = xdrivePosRcv[cnt];
        }  
        else
        {
            if (chassis_StartMove_Bit == 1)
            {
                if (chassis_Struct.t < chassis_Struct.accKeinematic.T_total)
                {
                    chassis_Struct.t += 0.01f;
                    chassis_moveL_Cal();      
                }
                else
                {
                    chassis_StartMove_Bit = 0; 
                    chassis_Struct.t = 0;
                    for(uint8_t i=0; i<4;i++)
                    chassis_motor_offset[i] = xdriveMultiPos_pos[i];
                }
                
            }
            xdrive_PosControl_multi();//启动发送
        }

        
        	
    }
}
/****************END*************************************/
uint8_t deb1=0,deb1pre=0;
void robotBusiness_task(void *argument)
{
    for (;;)
    {
        if (deb1 != deb1pre)
        {
            deb1pre = deb1;
            chassis_moveL_Start();         
        }
        
 
        vTaskDelay(100);
        //xSemaphoreTake(chssisFinishMove_Semaphore,portMAX_DELAY);
        vTaskDelay(100);

    }
    
}
