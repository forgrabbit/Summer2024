#include "hcsr04_driver.h"
#include <stdio.h>
#include <stdint.h>
#include "gd32f10x.h"
#include "systick.h"
#include "led_driver.h"
#include "key_driver.h"
#include "oled_driver.h"
#include "uart_driver.h"

/*
1.接线:
	  超声波模块与开发板用杜邦线相连，板子PB9接Trig, PB8接Echo, GND接GND 5V接VCC。
2.实验:
	  下载程序，全速运行，安装OLED屏幕，屏幕显示距离值。说明实验成功
*/
/*-----------------------------------------------------------
定时器初始化 初始化定时器2 定时时间为10us 用来给超声波计时
系统主频72MHz
-----------------------------------------------------------*/
/**************************************************
 * 函数名: TIM2_Init(u16 arr,u16 psc)
 * 功  能: 定时器2初始化参数配置
 * 返回值: 装载值arr，预分频系数值psc
 ***************************************************/
void TIM2_Init(uint16_t arr, uint16_t psc)
{
	/* TIMER2 configuration */
	timer_parameter_struct Timer_initpara;
	rcu_periph_clock_enable(RCU_TIMER2); // 使能RCU_TIMER2时钟

	timer_deinit(TIMER2);

	/* initialize TIMER init parameter struct */
	timer_struct_para_init(&Timer_initpara);

	Timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
	Timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
	Timer_initpara.counterdirection = TIMER_COUNTER_UP;
	Timer_initpara.period = arr;	// 9;
	Timer_initpara.prescaler = psc; // 77;
	Timer_initpara.repetitioncounter = 0;
	timer_init(TIMER2, &Timer_initpara);

	// 手动清除一下更新中断标志位，避免刚初始化完成就进入中断
	timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);

	/* TIMER2 channel control update interrupt enable */
	timer_interrupt_enable(TIMER2, TIMER_INT_UP);

	/* enable and set key EXTI interrupt to the specified priority */
	nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
	nvic_irq_enable(TIMER2_IRQn, 1U, 1U);

	timer_enable(TIMER2);
}

void Ultrasonic_Init()
{
	/*--------------------------------------------------------------------
	初始化 超声波模块引脚
	--------------------------------------------------------------------*/
	rcu_periph_clock_enable(RCU_GPIOB);										// 使能GPIOB时钟
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);		// Trig - PB11输出
	gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_8); // Echo - PB10输入
	/*预先拉低Trig引脚*/
	gpio_bit_write(GPIOB, GPIO_PIN_9, RESET);

	TIM2_Init(9, 77); // 用于计数
}

uint32_t HalTime1=0, HalTime2=0;
uint32_t Get_Distance(void) // 传入时间单位10us
{
	uint32_t Distance = 0;
	/*将Trig引脚拉高十秒*/
	gpio_bit_write(GPIOB, GPIO_PIN_9, SET);
	delay_us(10);
	gpio_bit_write(GPIOB, GPIO_PIN_9, RESET);
	//如果Echo引脚是低电平则一直等待，直到为Echo引脚为高电平，为高电平就说明接受到了返回的信号
	while (gpio_input_bit_get(GPIOB, GPIO_PIN_8) == 0);
	//记录下此时的时间值（10us定时器中累加的变量，所以时间单位为10us）
	HalTime1 = TimeCounter;
	//等待高电平时间结束
	while (gpio_input_bit_get(GPIOB, GPIO_PIN_8) == 1);

	//计算时间 判断时间变量是不是从头开始累加了（变量超过范围时会从0开始累加）
	if (TimeCounter > HalTime1)
	{
		//计算高电平的时间长度
		HalTime2 = TimeCounter - HalTime1;
		// 距离单位cm,声速340M/S，时间*速度/2=距离
		Distance = (uint32_t)(((float)HalTime2 * 17) / 100);
	}
	return Distance;
}

void Test_Ultrasonic()
{
	uint16_t Dis = 0.0;
	char txt[32];
	Ultrasonic_Init();
	OLED_Init();						 // OLED初始化
	OLED_P6x8Str(10, 0, "Test HCSR04 "); // 字符串
	delay_1ms(5);
	while (1)
	{
		Dis = Get_Distance();
		sprintf(txt, "Dis=%3d cm", Dis);
		OLED_P8x16Str(10, 2, txt); // 显示字符串
		printf("Distance:%dcm\n", Dis);
		led_toggle();
		delay_1ms(100);
	}
}
