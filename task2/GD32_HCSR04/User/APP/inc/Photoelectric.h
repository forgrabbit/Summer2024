#ifndef _PHOTOELECTRIC_H_
#define _PHOTOELECTRIC_H_

#include "stdint.h"
#define sensor1_PIN GPIO_PIN_4
#define sensor1_GPIO_PORT GPIOA
#define sensor1_GPIO_CLK RCU_GPIOA

#define sensor2_PIN GPIO_PIN_7
#define sensor2_GPIO_PORT GPIOA
#define sensor2_GPIO_CLK RCU_GPIOA

#define sensor3_PIN GPIO_PIN_6
#define sensor3_GPIO_PORT GPIOA
#define sensor3_GPIO_CLK RCU_GPIOA

#define sensor4_PIN GPIO_PIN_5
#define sensor4_GPIO_PORT GPIOA
#define sensor4_GPIO_CLK RCU_GPIOA



// π‚µÁ—°‘Ò
typedef enum
{
	
	sensor1 = 0,
	sensor2,
	sensor3,
	sensor4,
	
} sensor_num;

void sensor_init(void);
uint8_t Read_sensor(sensor_num num);


#endif
