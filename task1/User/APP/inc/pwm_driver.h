#ifndef _PWM_DRIVER_H_
#define _PWM_DRIVER_H_
#include "stdint.h"


void PWM_Init(void);
void Motor_Init(void);
void MotorCtrl3W(int32_t motor1, int32_t motor2, int32_t motor3);



#endif


