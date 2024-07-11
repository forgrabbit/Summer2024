#include "key_driver.h"
#include "led_driver.h"
#include "gd32f10x.h"

/*KEY初始化*/
void key_init(void)
{
	/* 打开KEY1的时钟 */
	// rcu_periph_clock_enable(KEY1_GPIO_PORT);
	rcu_periph_clock_enable(KEY1_GPIO_CLK);
	/* 配置KEY1的GPIO模式 */
	gpio_init(KEY1_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, KEY1_PIN);

	/* 打开KEY2的时钟 */
	// rcu_periph_clock_enable(KEY2_GPIO_PORT);
	rcu_periph_clock_enable(KEY2_GPIO_CLK);
	/* 配置KEY2的GPIO模式 */
	gpio_init(KEY2_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, KEY2_PIN);

	/* 打开KEY3的时钟 */
	// rcu_periph_clock_enable(KEY3_GPIO_PORT);
	rcu_periph_clock_enable(KEY3_GPIO_CLK);
	/* 配置KEY1的GPIO模式 */
	gpio_init(KEY3_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, KEY3_PIN);

	/* 打开KEY4的时钟 */
	// rcu_periph_clock_enable(KEY4_GPIO_PORT);
	rcu_periph_clock_enable(KEY1_GPIO_CLK);
	/* 配置KEY1的GPIO模式 */
	gpio_init(KEY4_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, KEY4_PIN);

	/* 打开KEY5的时钟 */
	// rcu_periph_clock_enable(KEY5_GPIO_PORT);
	rcu_periph_clock_enable(KEY5_GPIO_CLK);
	/* 配置KEY1的GPIO模式 */
	gpio_init(KEY5_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, KEY5_PIN);
}

/*
@函数名称：Read_key
@功能说明：判断相关按键是否按下
@参数说明：num :按键编号
@函数返回：状态 1：按下 0：没按下

@调用方法：if(Read_key(KEY0)) KEY0是否按下
@备    注：无
*/

uint8_t Read_key(key_num num)
{
	switch (num)
	{
	case 0:
		if (gpio_input_bit_get(KEY1_GPIO_PORT, KEY1_PIN) == RESET)
		{
			if (gpio_input_bit_get(KEY1_GPIO_PORT, KEY1_PIN) == RESET)
				return 1;
		}
		break;

	case 1:
		if (gpio_input_bit_get(KEY2_GPIO_PORT, KEY2_PIN) == RESET)
		{
			if (gpio_input_bit_get(KEY2_GPIO_PORT, KEY2_PIN) == RESET)
				return 1;
		}
		break;

	case 2:
		if (gpio_input_bit_get(KEY3_GPIO_PORT, KEY3_PIN) == RESET)
		{
			if (gpio_input_bit_get(KEY3_GPIO_PORT, KEY3_PIN) == RESET)
				return 1;
		}
		break;
	case 3:
		if (gpio_input_bit_get(KEY4_GPIO_PORT, KEY4_PIN) == RESET)
		{
			if (gpio_input_bit_get(KEY4_GPIO_PORT, KEY4_PIN) == RESET)
				return 1;
		}
		break;

	case 4:
		if (gpio_input_bit_get(KEY5_GPIO_PORT, KEY5_PIN) == RESET)
		{
			if (gpio_input_bit_get(KEY5_GPIO_PORT, KEY5_PIN) == RESET)
				return 1;
		}
		break;
	}
	return 0;
}
