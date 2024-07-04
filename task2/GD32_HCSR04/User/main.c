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
	int S_Value [4] = {0,0,0,0};
	int SD = 16;//if distance above SD, Straight forward, else rotate
	int BlACK_flag = 1;// if forward on the black way BLACK_flag = 0, else = 1
	int speed_slow = 500;
	int speed_medium = 1500;
	int speed_quick = 1500;
	int rotate = 1800;
	
	
	Timer_Init();
	encoder1_config();
	encoder2_config();
	encoder3_config();
	int32_t Enc1 = 0;
	int32_t Enc2 = 0;
	int32_t Enc3 = 0;
	
	

	uint16_t Dis = 0.0;
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
	OLED_P6x8Str(10, 0, "Test HCSR04 "); // 字符串
	delay_1ms(5);
	
	
	
	while (1)
	{
		
		Dis = Get_Distance();
		//sprintf(txt, "Dis=%3d cm", Dis);
		//OLED_P8x16Str(10, 3, txt); // 显示字符串
		//printf("Distance:%dcm\n", Dis);
		led_toggle();
		
		
		sensor_Value[0] = Read_sensor(sensor1);
		sensor_Value[1] = Read_sensor(sensor2);
		sensor_Value[2] = Read_sensor(sensor3);
		sensor_Value[3] = Read_sensor(sensor4);
		
		sprintf(txt, "%d %d %d %d", sensor_Value[0], sensor_Value[1], sensor_Value[2], sensor_Value[3]);
		//OLED_P6x8Str(0, 2, txt); // 字符串
		led_toggle();
		//delay_1ms(200);
		
		//OLED_P6x8Str(0, 1, "PWM_Test."); // 字符串
		
		OLED_P6x8Str(0, 1, "TIM_Test."); // 字符串
		Enc1 = ENC_Read(ENCODER1_TIMER);
		sprintf(TXT, "E1:%4d", Enc1);
		OLED_P6x8Str(0, 3, TXT); // 字符串

		Enc2 = ENC_Read(ENCODER2_TIMER);
		sprintf(TXT, "E2:%4d", Enc2);
		OLED_P6x8Str(0, 4, TXT); // 字符串

		Enc3 = ENC_Read(ENCODER3_TIMER);
		sprintf(TXT, "E3:%4d", Enc3);
		OLED_P6x8Str(0, 5, TXT); // 字符串
		
		//straight
		if (sensor_Value[1] == 1||
		sensor_Value[2] == 1||
		sensor_Value[0] == 1||
		sensor_Value[3] == 1
		)
		{
			BlACK_flag =0;
		}
		
		
		
		if ((Dis > SD || Dis == 1)&& BlACK_flag == 0)
		{
		if (sensor_Value[1] == 1&&
		sensor_Value[2] == 1&&
		sensor_Value[0]==sensor_Value[3]
		)
		{
			duty = speed_quick;
			MotorCtrl3W(0, -duty, duty);
			S_Value[0]=sensor_Value[0];
		  S_Value[1]=sensor_Value[1];
		  S_Value[2]=sensor_Value[2];
		  S_Value[3]=sensor_Value[3];//record the value of sensor at last time 
		}
		
		//left run
		else if (sensor_Value[0] == 1&&
		sensor_Value[1] == 1&&
		sensor_Value[2] == 1&&
		sensor_Value[3] == 0)
		{
			/*duty = rotate;*/
			MotorCtrl3W(0,-speed_quick, speed_medium );
			S_Value[0]=sensor_Value[0];
		  S_Value[1]=sensor_Value[1];
		  S_Value[2]=sensor_Value[2];
		  S_Value[3]=sensor_Value[3];//record the value of sensor at last time 
		}// same 1 = 1 2 = 1
		
		//right run
		else if (sensor_Value[0] == 0&&
		sensor_Value[1] == 1&&
		sensor_Value[2] == 1&&
		sensor_Value[3] == 1)
		{
			//duty = rotate;
			MotorCtrl3W(0,-speed_medium ,speed_quick );
			S_Value[0]=sensor_Value[0];
		  S_Value[1]=sensor_Value[1];
		  S_Value[2]=sensor_Value[2];
		  S_Value[3]=sensor_Value[3];//record the value of sensor at last time 
		}//same 1 ==1 2==1
		
		//left
		else if (sensor_Value[0] == 1&&
		sensor_Value[2] == 0&&
		sensor_Value[3] == 0)
		{
			MotorCtrl3W(0,-rotate, speed_slow );
			S_Value[0]=sensor_Value[0];
		  S_Value[1]=sensor_Value[1];
		  S_Value[2]=sensor_Value[2];
		  S_Value[3]=sensor_Value[3];//record the value of sensor at last time 
			
		}
		
		//right
		else if (sensor_Value[0] == 0&&
		sensor_Value[1] == 0&&
		sensor_Value[3] == 1)
		{
			MotorCtrl3W(0,-speed_slow ,rotate+300 );
			S_Value[0]=sensor_Value[0];
		  S_Value[1]=sensor_Value[1];
		  S_Value[2]=sensor_Value[2];
		  S_Value[3]=sensor_Value[3];//record the value of sensor at last time 
		}
		
		//pa
		else // black way
		{
			//delay_1ms(300);
			if((S_Value[1] == 1 && S_Value[2] == 1) || (S_Value[0] == 1 && S_Value[3] == 1))
			{
				BlACK_flag = 1;
			}
			else if(S_Value[0] == 1 && S_Value[3] == 0)
			{
			duty = -1000;
			MotorCtrl3W(duty , duty, duty);//left
			}
			else if(S_Value[3] == 1 && S_Value[0] == 0)
			{
			duty = 1000;
			MotorCtrl3W(duty , duty, duty);
			}
			
			else
			{
				duty = -1000;
				MotorCtrl3W(duty , duty, duty);
			}
		}
	}
		else if((Dis > SD || Dis == 1) && BlACK_flag == 1)// judge the right way by rotating 
		{

			duty = speed_quick;
			MotorCtrl3W(0, -duty, duty);// avoid obstruction 
			//
		}
		else if(Dis < SD)// rotate to the right way 
		{
			BlACK_flag =1;
			duty = -1000;
			MotorCtrl3W(duty , duty, duty);
			delay_1ms(150);
		}
		
		if(Enc1 == 0 && Enc2 == 0 && Enc3 == 0 )
		{
			duty = speed_quick;
			MotorCtrl3W(0, duty, -duty);
			delay_1ms(1000);
			MotorCtrl3W(duty , duty, duty);
			delay_1ms(100);
		}
		


		
		//sprintf(txt, "BlACK_flag:%05d", BlACK_flag);
		//OLED_P8x16Str(10, 5, txt); // 显示字符串
		led_toggle();
		delay_1ms(10);
		
	}
}

