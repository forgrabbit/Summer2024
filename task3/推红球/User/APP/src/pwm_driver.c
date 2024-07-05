#include "pwm_driver.h"
#include "gd32f10x.h"
#include "gd32f10x_libopt.h"
#include "systick.h"

void timer_config(void)
{
	timer_oc_parameter_struct timer_ocintpara;
	timer_parameter_struct timer_initpara;

	rcu_periph_clock_enable(RCU_TIMER0);

	timer_deinit(TIMER0);

	/* TIMER0 configuration */
	timer_initpara.prescaler = 0;
	timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
	timer_initpara.counterdirection = TIMER_COUNTER_UP;
	timer_initpara.period = 7199;
	timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
	timer_initpara.repetitioncounter = 0;

	timer_init(TIMER0, &timer_initpara);

	/* CH0,CH1,CH2 configuration in PWM model */
	timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
	timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
	timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
	timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
	timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
	timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

	timer_channel_output_config(TIMER0, TIMER_CH_0, &timer_ocintpara);
	timer_channel_output_config(TIMER0, TIMER_CH_1, &timer_ocintpara);
	timer_channel_output_config(TIMER0, TIMER_CH_2, &timer_ocintpara);

	/* CH0 configuration in PWM model */
	timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0, 499);
	timer_channel_output_mode_config(TIMER0, TIMER_CH_0, TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(TIMER0, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);

	/* CH1 configuration in PWM model */
	timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_1, 499);
	timer_channel_output_mode_config(TIMER0, TIMER_CH_1, TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(TIMER0, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);

	/* CH2 configuration in PWM model */
	timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_2, 499);
	timer_channel_output_mode_config(TIMER0, TIMER_CH_2, TIMER_OC_MODE_PWM0);
	timer_channel_output_shadow_config(TIMER0, TIMER_CH_2, TIMER_OC_SHADOW_DISABLE);

	timer_auto_reload_shadow_enable(TIMER0);

	timer_primary_output_config(TIMER0, ENABLE);

	timer_enable(TIMER0);
}

void PWM_Init(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_AF);

	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

	timer_config();
}

void Motor_Init(void)
{

	/* enable the led clock */
	rcu_periph_clock_enable(RCU_GPIOB); // 使能GPIOB时钟
	/* configure led GPIO port */
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12); // 初始化LED对应的GPIO
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13); // 初始化LED对应的GPIO
	gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_14); // 初始化LED对应的GPIO

	GPIO_BC(GPIOB) = GPIO_PIN_12;
	GPIO_BC(GPIOB) = GPIO_PIN_13;
	GPIO_BC(GPIOB) = GPIO_PIN_14;

	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_AF);

	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
	gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

	timer_config();
}

void MotorCtrl3W(int32_t motor1, int32_t motor2, int32_t motor3)
{
	if (motor1 > 0)
	{
		timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0, motor1);
		gpio_bit_reset(GPIOB,GPIO_PIN_12);
	}
	else
	{
		timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0, -motor1);
		gpio_bit_set(GPIOB,GPIO_PIN_12);		
	}

	if (motor2 > 0)
	{
		timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_1, motor2);
		gpio_bit_reset(GPIOB,GPIO_PIN_13);
	}
	else
	{
		timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_1, -motor2);
		gpio_bit_set(GPIOB,GPIO_PIN_13);	
	}
	if (motor3 > 0)
	{
		timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_2, motor3);
		gpio_bit_reset(GPIOB,GPIO_PIN_14);
	}
	else
	{
		timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_2, -motor3);
		gpio_bit_set(GPIOB,GPIO_PIN_14);	
	}
}
