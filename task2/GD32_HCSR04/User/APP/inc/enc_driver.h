#ifndef _ENC_DRIVER_H
#define _ENC_DRIVER_H

#include "stdint.h"

#define ENCODER1_TIMER_RCU RCU_TIMER1
#define ENCODER1_TIMER TIMER1

#define TIMER1_A_CH_RCU RCU_GPIOA
#define TIMER1_B_CH_RCU RCU_GPIOB
#define TIMER1_A_CH_PORT GPIOA
#define TIMER1_B_CH_PORT GPIOB
#define TIMER1_A_CH_PIN GPIO_PIN_15
#define TIMER1_B_CH_PIN GPIO_PIN_3

#define ENCODER2_TIMER_RCU RCU_TIMER2
#define ENCODER2_TIMER TIMER2

#define TIMER2_A_CH_RCU RCU_GPIOB
#define TIMER2_B_CH_RCU RCU_GPIOB
#define TIMER2_A_CH_PORT GPIOB
#define TIMER2_B_CH_PORT GPIOB
#define TIMER2_A_CH_PIN GPIO_PIN_4
#define TIMER2_B_CH_PIN GPIO_PIN_5


#define ENCODER3_TIMER_RCU RCU_TIMER3
#define ENCODER3_TIMER TIMER3

#define TIMER3_A_CH_RCU RCU_GPIOB
#define TIMER3_B_CH_RCU RCU_GPIOB
#define TIMER3_A_CH_PORT GPIOB
#define TIMER3_B_CH_PORT GPIOB
#define TIMER3_A_CH_PIN GPIO_PIN_6
#define TIMER3_B_CH_PIN GPIO_PIN_7

void encoder1_config(void);
void encoder2_config(void);
void encoder3_config(void);
int32_t ENC_Read(uint32_t timer_periph);

#endif
