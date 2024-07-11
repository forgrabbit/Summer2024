#include "gd32f10x.h"
#include "systick.h"
#include <stdio.h>
#include "led_driver.h"
#include "key_driver.h"
#include "oled_driver.h"
#include "tim_driver.h"
#include "enc_driver.h"
#include "uart_driver.h"
#include "pwm_driver.h"
#include "Photoelectric.h"
#include "hcsr04_driver.h"

/*!
	\brief      main function
	\param[in]  none
	\param[out] none
	\retval     none
*/

int main(void)
{
	uint8_t sensor_Value[4];
	char txt[20];
	char TXT[50];
	systick_config();
	rcu_ahb_clock_config(RCU_AHB_CKSYS_DIV1); // AHB主频是1分频
	systick_config();						  // 系统主频72MHZ,采用外部晶振
	rcu_periph_clock_enable(RCU_AF);		  // 管脚复用时钟alternate function clock使能

	led_init(); // LED初始化
	key_init();
	OLED_Init(); // OLED初始化
	
	delay_1ms(200);
	OLED_CLS();
	sensor_init();
	Motor_Init();	
  int32_t duty = 0;	//调整占空比的变量	
	
	
	Timer_Init();
	encoder1_config();
	encoder2_config();
	encoder3_config();
	int32_t Enc1 = 0;
	int32_t Enc2 = 0;
	int32_t Enc3 = 0;
	
	
	systick_config();
	rcu_ahb_clock_config(RCU_AHB_CKSYS_DIV1); // AHB主频是1分频
	systick_config();						  // 系统主频72MHZ,采用外部晶振,由两个宏决定(__SYSTEM_CLOCK_72M_PLL_HXTAL与HXTAL_VALUE)
	rcu_periph_clock_enable(RCU_AF);		  // 管脚复用时钟alternate function clock使能

	led_init(); // LED初始化
	key_init();
	OLED_Init(); // OLED初始化
	delay_1ms(200);
	OLED_CLS();

	Ultrasonic_Init();
	OLED_Init();						 // OLED初始化
	OLED_P6x8Str(10, 2, "Test red ball "); // 字符串
	
	delay_1ms(5);

	USART2_Init(115200);                //初始化USART2 115200 使能 并开启串口中断接受
   
  OLED_Init();                         // OLED初始化
  OLED_P8x16Str(10, 0,"Test USART2");  // 字符串
	delay_1ms(50);
	printf("USART2 Init OK \n");
  USART_Sent_String(USART2,"USART2 Init OK\n");
	
	
	
	
	while(1)
	{
		
		Push_Red_Ball();
		
	}
	while(1)
	{
	delay_1ms(100);
	}
}

