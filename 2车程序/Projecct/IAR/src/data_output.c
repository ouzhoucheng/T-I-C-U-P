#include "car_global.h"
#include "data.h"
void data_output()

{

    if (mycar.status)
    {
        if (outdata.left_pwm >= 0)
        {
            pwm_duty(PWM1_MODULE3_CHB_C17, outdata.left_pwm);
            pwm_duty(PWM1_MODULE3_CHA_C16, 0);
        }
        else
        {
            pwm_duty(PWM1_MODULE3_CHA_C16, -outdata.left_pwm);
            pwm_duty(PWM1_MODULE3_CHB_C17, 0);
        }
        if (outdata.right_pwm >= 0)
        {
            pwm_duty(PWM2_MODULE3_CHA_C18, outdata.right_pwm);
            pwm_duty(PWM2_MODULE3_CHB_C19, 0);
        }
        else
        {
            pwm_duty(PWM2_MODULE3_CHB_C19, -outdata.right_pwm);
            pwm_duty(PWM2_MODULE3_CHA_C18, 0);
        }
    }
    else
    {
        // pwm_duty(PWM1_MODULE3_CHA_C16, 0);
        // pwm_duty(PWM2_MODULE3_CHB_C19, 0);
        pwm_duty(PWM1_MODULE3_CHB_C17, setpara.testL > 0 ? setpara.testL : 0);
        pwm_duty(PWM1_MODULE3_CHA_C16, setpara.testL < 0 ? -setpara.testL : 0);
        pwm_duty(PWM2_MODULE3_CHA_C18, setpara.testR > 0 ? setpara.testR : 0);
        pwm_duty(PWM2_MODULE3_CHB_C19, setpara.testR < 0 ? -setpara.testR : 0);
    }
    /////////旺仔牛奶
    //蜂鸣器
    // pwm_duty(PWM2_MODULE2_CHA_C10, _LIMIT(setpara.Buzz*500,0,50000));
    // //激光
    // pwm_duty(PWM2_MODULE2_CHB_C11, _LIMIT(setpara.Laser*500,0,50000));
    // //舵机
    // pwm_duty(PWM2_MODULE1_CHA_C8, _LIMIT((setpara.Servo1*29+1050),1050,6350));
    // pwm_duty(PWM2_MODULE1_CHB_C9, _LIMIT((setpara.Servo2*29+1050),1050,6350));
    // pwm_duty(PWM2_MODULE2_CHB_C11, outdata.steer_pwm);
}