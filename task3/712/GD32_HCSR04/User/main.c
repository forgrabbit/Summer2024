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

	
	while(1)
	{
		OLED_P6x8Str(0, 1, "TIM_Test."); // 字符串
		Enc1 = ENC_Read(ENCODER1_TIMER);
		Enc2 = ENC_Read(ENCODER2_TIMER);
		Enc3 = ENC_Read(ENCODER3_TIMER);
		//sprintf(TXT, "E:%4d %4d %4d ", Enc1, Enc2, Enc3);
		
		OLED_P6x8Str(0, 2, TXT);
		delay_1ms(100);
		
		
		//if(Enc1 <= 3 && Enc3 <= 3 && Enc1 >= -3 && Enc3 >= -3)
		//{
		//	MotorCtrl3W(1800, 1800, 1800);
	//		delay_1ms(100);
	//		MotorCtrl3W(-1800, -1800, -1800);
		//}
		//else
		{
			Test_UASRT2();
		}
		delay_1ms(10);
	}
}

