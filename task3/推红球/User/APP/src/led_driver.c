#include "led_driver.h"
#include "systick.h"
#include "gd32f10x.h"

/*初始化LED，引脚是PB15 推挽输出 速度50MHZ*/
void led_init(void)
{
    /* enable the led clock */
    rcu_periph_clock_enable(RCU_GPIOB); // 使能GPIOB时钟
    /* configure led GPIO port */
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15); // 初始化LED对应的GPIO

    gpio_bit_set(GPIOB,GPIO_PIN_15);
}

/*LED打开*/
void led_on(void)
{
		gpio_bit_reset(GPIOB,GPIO_PIN_15);
}
/*LED关闭*/
void led_off(void)
{
		gpio_bit_set(GPIOB,GPIO_PIN_15);
}
/*LED状态反转*/
void led_toggle(void)
{
    gpio_bit_write(GPIOB, GPIO_PIN_15,
                   (bit_status)(1 - gpio_input_bit_get(GPIOB, GPIO_PIN_15)));
}
