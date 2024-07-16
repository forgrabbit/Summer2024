#include "Photoelectric.h"
#include "gd32f10x.h"

void sensor_init(void)
{
    /* 打开sensor1的时钟 */
    // rcu_periph_clock_enable(sensor1_GPIO_PORT);
    rcu_periph_clock_enable(sensor1_GPIO_CLK);
    /* 配置sensor1的GPIO模式 */
    gpio_init(sensor1_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, sensor1_PIN);

    /* 打开sensor2的时钟 */
    // rcu_periph_clock_enable(sensor2_GPIO_PORT);
    rcu_periph_clock_enable(sensor2_GPIO_CLK);
    /* 配置sensor2的GPIO模式 */
    gpio_init(sensor2_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, sensor2_PIN);

    /* 打开sensor3的时钟 */
    // rcu_periph_clock_enable(sensor3_GPIO_PORT);
    rcu_periph_clock_enable(sensor3_GPIO_CLK);
    /* 配置sensor1的GPIO模式 */
    gpio_init(sensor3_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, sensor3_PIN);

    /* 打开sensor4的时钟 */
    // rcu_periph_clock_enable(sensor4_GPIO_PORT);
    rcu_periph_clock_enable(sensor1_GPIO_CLK);
    /* 配置sensor1的GPIO模式 */
    gpio_init(sensor4_GPIO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, sensor4_PIN);
}

uint8_t Read_sensor(sensor_num num)
{
    switch (num)
    {
    case 0:
        if (gpio_input_bit_get(sensor1_GPIO_PORT, sensor1_PIN) == RESET)
            return 1;
        break;
    case 1:
        if (gpio_input_bit_get(sensor2_GPIO_PORT, sensor2_PIN) == RESET)
            return 1;
        break;
    case 2:
        if (gpio_input_bit_get(sensor3_GPIO_PORT, sensor3_PIN) == RESET)
            return 1;
        break;
    case 3:
        if (gpio_input_bit_get(sensor4_GPIO_PORT, sensor4_PIN) == RESET)
            return 1;
        break;
        }
    return 0;
}
