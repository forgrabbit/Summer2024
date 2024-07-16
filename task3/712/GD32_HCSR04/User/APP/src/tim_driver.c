#include "tim_driver.h"
#include "gd32f10x.h"

/*-----------------------------------------------------------
定时器初始化 初始化定时器2 定时时间为1000000us=1000ms=1s 
系统主频72MHz
-----------------------------------------------------------*/
void Timer_Init(void)
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
    Timer_initpara.period = 7199;
    Timer_initpara.prescaler = 9999;
    Timer_initpara.repetitioncounter = 0;
    timer_init(TIMER2, &Timer_initpara);

    // 手动把更新中断标志位清除一下，避免刚初始化完成就进入中断
    timer_interrupt_flag_clear(TIMER2, TIMER_INT_FLAG_UP);

    /* TIMER2 channel control update interrupt enable */
    timer_interrupt_enable(TIMER2, TIMER_INT_UP);

    /* enable and set key EXTI interrupt to the specified priority */
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    nvic_irq_enable(TIMER2_IRQn, 1U, 1U);

    timer_enable(TIMER2);
}
