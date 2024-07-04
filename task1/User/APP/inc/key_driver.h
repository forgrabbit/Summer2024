
#ifndef _KEY_DRIVER_H
#define _KEY_DRIVER_H
#include "systick.h"

#define KEY1_PIN 					GPIO_PIN_0
#define KEY1_GPIO_PORT 		GPIOB
#define KEY1_GPIO_CLK 		RCU_GPIOB

#define KEY2_PIN 					GPIO_PIN_1
#define KEY2_GPIO_PORT 		GPIOB
#define KEY2_GPIO_CLK 		RCU_GPIOB

#define KEY3_PIN 					GPIO_PIN_2
#define KEY3_GPIO_PORT 		GPIOB
#define KEY3_GPIO_CLK 		RCU_GPIOB

#define KEY4_PIN 					GPIO_PIN_11
#define KEY4_GPIO_PORT 		GPIOA
#define KEY4_GPIO_CLK 		RCU_GPIOA

#define KEY5_PIN 					GPIO_PIN_12
#define KEY5_GPIO_PORT 		GPIOA
#define KEY5_GPIO_CLK 		RCU_GPIOA

// 按键选择
typedef enum
{

	KEY1 = 0,
	KEY2,
	KEY3,
	KEY4,
	KEY5
} key_num;

void key_init(void);		   // 按键初始化
uint8_t Read_key(key_num num); // 读取按键状态

#endif
