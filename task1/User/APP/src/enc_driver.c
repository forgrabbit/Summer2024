#include "enc_driver.h"
#include "gd32f10x.h"

void encoder1_config(void)
{
	timer_parameter_struct timer_initpara;

	rcu_periph_clock_enable(ENCODER1_TIMER_RCU); // 使能对应定时器的时钟

	rcu_periph_clock_enable(TIMER1_A_CH_RCU); // 使能接口对应的时钟
	rcu_periph_clock_enable(TIMER1_B_CH_RCU); // 使能接口对应的时钟

	gpio_pin_remap_config(GPIO_TIMER1_FULL_REMAP, ENABLE);

	gpio_init(TIMER1_A_CH_RCU, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, TIMER1_A_CH_PIN); // 初始化对应的GPIO
	gpio_init(TIMER1_B_CH_RCU, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, TIMER1_B_CH_PIN); // 初始化对应的GPIO

	timer_deinit(ENCODER1_TIMER);

	/* TIMER1 configuration */
	timer_initpara.prescaler = 0;
	timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
	timer_initpara.counterdirection = TIMER_COUNTER_UP;
	timer_initpara.period = 65535;
	timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0;
	timer_init(ENCODER1_TIMER, &timer_initpara); // 写入定时器的配置

	// 配置定时器为正交解码模式
	timer_quadrature_decoder_mode_config(ENCODER1_TIMER, TIMER_QUAD_DECODER_MODE1, TIMER_IC_POLARITY_RISING, TIMER_IC_POLARITY_FALLING);

	/* auto-reload preload enable */
	timer_auto_reload_shadow_enable(ENCODER1_TIMER);

	/* TIMER1 counter enable */
	timer_enable(ENCODER1_TIMER);
}

void encoder2_config(void)
{
	timer_parameter_struct timer_initpara;

	rcu_periph_clock_enable(ENCODER2_TIMER_RCU); // 使能对应定时器的时钟

	rcu_periph_clock_enable(TIMER2_A_CH_RCU); // 使能接口对应的时钟
	rcu_periph_clock_enable(TIMER2_B_CH_RCU); // 使能接口对应的时钟

	gpio_pin_remap_config(GPIO_TIMER2_PARTIAL_REMAP, ENABLE);
	gpio_init(TIMER2_A_CH_RCU, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, TIMER2_A_CH_PIN); // 初始化对应的GPIO
	gpio_init(TIMER2_B_CH_RCU, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, TIMER2_B_CH_PIN); // 初始化对应的GPIO

	timer_deinit(ENCODER2_TIMER);

	/* TIMER2 configuration */
	timer_initpara.prescaler = 0;
	timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
	timer_initpara.counterdirection = TIMER_COUNTER_UP;
	timer_initpara.period = 65535;
	timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0;
	timer_init(ENCODER2_TIMER, &timer_initpara); // 写入定时器的配置

	// 配置定时器为正交解码模式
	timer_quadrature_decoder_mode_config(ENCODER2_TIMER, TIMER_QUAD_DECODER_MODE1, TIMER_IC_POLARITY_RISING, TIMER_IC_POLARITY_FALLING);

	/* auto-reload preload enable */
	timer_auto_reload_shadow_enable(ENCODER2_TIMER);

	/* TIMER2 counter enable */
	timer_enable(ENCODER2_TIMER);
}

void encoder3_config(void)
{
	timer_parameter_struct timer_initpara;

	rcu_periph_clock_enable(ENCODER3_TIMER_RCU); // 使能对应定时器的时钟

	rcu_periph_clock_enable(TIMER3_A_CH_RCU); // 使能接口对应的时钟
	rcu_periph_clock_enable(TIMER3_B_CH_RCU); // 使能接口对应的时钟

	gpio_init(TIMER3_A_CH_RCU, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, TIMER3_A_CH_PIN); // 初始化对应的GPIO
	gpio_init(TIMER3_B_CH_RCU, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, TIMER3_B_CH_PIN); // 初始化对应的GPIO

	timer_deinit(ENCODER3_TIMER);

	/* TIMER3 configuration */
	timer_initpara.prescaler = 0;
	timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
	timer_initpara.counterdirection = TIMER_COUNTER_UP;
	timer_initpara.period = 65535;
	timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0;
	timer_init(ENCODER3_TIMER, &timer_initpara); // 写入定时器的配置

	// 配置定时器为正交解码模式
	timer_quadrature_decoder_mode_config(ENCODER3_TIMER, TIMER_QUAD_DECODER_MODE1, TIMER_IC_POLARITY_RISING, TIMER_IC_POLARITY_FALLING);

	/* auto-reload preload enable */
	timer_auto_reload_shadow_enable(ENCODER3_TIMER);

	/* TIMER3 counter enable */
	timer_enable(ENCODER3_TIMER);
}

int32_t ENC_Read(uint32_t timer_periph)
{
	int32_t count_value = 0U;
	count_value = TIMER_CNT(timer_periph);
	timer_counter_value_config(timer_periph, 0);
	if (count_value > 65534/2)
		count_value = count_value - 65535;
	return (count_value);
}
