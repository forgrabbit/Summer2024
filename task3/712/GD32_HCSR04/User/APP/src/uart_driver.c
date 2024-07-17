#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "gd32f10x.h"
#include "systick.h"
#include "led_driver.h"
#include "key_driver.h"
#include "oled_driver.h"
#include "uart_driver.h"
#include "pwm_driver.h"




uint8_t transmitter_buffer1[] = "HELLOWORLD";//定义发送数组
uint8_t receiver_buffer1[32];//定义接收数组
#define ARRAYNUM1(arr_nanme)      (uint32_t)(sizeof(arr_nanme) / sizeof(*(arr_nanme)))
#define TRANSMIT_SIZE1   (ARRAYNUM1(transmitter_buffer1) - 1)//计算大小
uint8_t transfersize1 = TRANSMIT_SIZE1;

uint8_t receivesize1 = 10;    //接收
__IO uint8_t  txcount1 = 0; 
__IO uint16_t rxcount1 = 0; 

//-------------------------------------------------------------------------------------

 uint8_t  USART_Rx_Buf[USART_RECV_BUF_SIZE];  // 用于保存 RX 接收到的数据
 uint16_t USART_Rx_Len = 0;                   // 用于保存 RX 接收到的数据的个数
 uint16_t USART_Rx_Count = 0;                 // 用于保存 RX 接收数据时已经接收到的数据个数
 uint8_t  USART_Tx_Buf[USART_SEND_BUF_SIZE];  // 用于保存 Tx 要发送的数据
 uint16_t USART_Tx_Len = 0;                   // 用于保存 Tx 要发送的数据的个数
 uint16_t USART_Tx_Count = 0;                 // 用于保存 Tx 要发送的数据时已经发送的数据个数


/**
  * @brief 重定向c库函数printf到USART1
  * @retval None
  */
int fputc(int ch,FILE *p)  
{
	usart_data_transmit(USART1,(unsigned char)ch);
	while(usart_flag_get(USART1, USART_FLAG_TC)==RESET);
	return ch;
}

/**
  * @brief 重定向c库函数getchar,scanf到USART2
  * @retval None
  */
int fgetc(FILE *f)
{
    uint8_t ch2 = 0;
    ch2 = usart_data_receive(USART2);
    return ch2;
}


/********************************************************************
* 函数名：void UART1_init(void)
* 参  数: uint32_t Baud_rate  波特率
* 调  用：USART1_Init(115200);
* USART1初始化，使用PA2(TX),PA3(RX)脚，115200波特率，无校验，8位数据，1位停止
********************************************************************/
void USART1_Init(uint32_t Baud_rate)
{
    rcu_periph_clock_enable(RCU_GPIOA);//使能GPIOA时钟
    rcu_periph_clock_enable(RCU_USART1);//使能USART1时钟

    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_2);        //PA2  TX输出
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ,GPIO_PIN_3);  //PA3  RX输入
    /* USART configure */
    usart_deinit(USART1);
    usart_baudrate_set(USART1,Baud_rate);
	usart_word_length_set(USART1, USART_WL_8BIT);
    usart_stop_bit_set(USART1, USART_STB_1BIT);
    usart_parity_config(USART1, USART_PM_NONE);
	usart_hardware_flow_rts_config(USART1, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART1, USART_CTS_DISABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_enable(USART1);//使能USART1	
    
    nvic_irq_enable(USART1_IRQn, 0, 1);             //使能USART1中断
	usart_interrupt_enable(USART1, USART_INT_RBNE); //接收中断打开
}
/********************************************************************
* 函数名：void UART2_init(void)
* 参  数: uint32_t Baud_rate  波特率
* 调  用：USART2_Init(115200);
* USART2初始化，使用PB10(TX),PB11(RX)脚，115200波特率，无校验，8位数据，1位停止
********************************************************************/
void USART2_Init(uint32_t Baud_rate)
{
    rcu_periph_clock_enable(RCU_GPIOB);//使能GPIOA时钟
    rcu_periph_clock_enable(RCU_USART2);//使能USART1时钟

    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_10);        //PB10  TX输出
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ,GPIO_PIN_11);  //PB11  RX输入
    /* USART configure */
    usart_deinit(USART2);
    usart_baudrate_set(USART2,Baud_rate);
		usart_word_length_set(USART2, USART_WL_8BIT);
    usart_stop_bit_set(USART2, USART_STB_1BIT);
    usart_parity_config(USART2, USART_PM_NONE);
		usart_hardware_flow_rts_config(USART2, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART2, USART_CTS_DISABLE);
    usart_transmit_config(USART2, USART_TRANSMIT_ENABLE);
    usart_receive_config(USART2, USART_RECEIVE_ENABLE);
    usart_enable(USART2);//使能USART2
    
    nvic_irq_enable(USART2_IRQn, 0, 0);             //使能USART2中断，抢占优先级和次优先级，越小越高
		usart_interrupt_enable(USART2, USART_INT_RBNE); //收发中断打开
}



void USART_Sent_Byte(uint32_t usart_periph,uint8_t byte)
{
    usart_data_transmit(USART2, byte);	    					//通过USART2发送
    while(RESET == usart_flag_get(USART2, USART_FLAG_TBE));		//判断缓冲区是否已经空了
}


//测试为通过
void USART_Sent_Data(uint32_t usart_periph ,uint8_t *data)
{
    uint32_t len;
    len = sizeof(*data) / sizeof(data[0]);
    while(data && len)
	{
		USART_Sent_Byte(usart_periph,*data);
		data++;		//指针往后移一位
        len--;
	}
}

void USART_Sent_String(uint32_t usart_periph,char *str)
{
	while(str && (*str))
     {
		USART_Sent_Byte(usart_periph,(uint8_t)*str);
		str++;		// 指针往下一个移动
     }
}



void Test_UASRT1(void)
{
	//USART相关配置
	USART1_Init(115200);                //初始胡USART 115200 使能 并开启串口中断接受
	USART2_Init(115200);                //初始胡USART 115200 使能 并开启串口中断接受

  OLED_Init();                         // OLED初始化
  OLED_P6x8Str(10, 0,"Test USART1");  // 字符串
	delay_1ms(50);
	printf(" USART1 Init ok \n");
	
	while(1)
	{
		if(rxcount1 >= receivesize1)//接收满10个字节，在USART0_IRQHandler函数中计数
		{
			rxcount1=0;
			txcount1=0;
			usart_interrupt_enable(USART1, USART_INT_TBE);//发送中断打开
			while(txcount1 < transfersize1);//等待发送完成，在USART0_IRQHandler函数中计数
			while (RESET == usart_flag_get(USART1, USART_FLAG_TC));//发送完成判断
			usart_interrupt_enable(USART1, USART_INT_RBNE);//接收中断打开
			printf("\nReceive1 OK \n");
		}
	}
}

unsigned int receive_data;
unsigned char receive_data_last_time_rotate = '1';
unsigned char receive_data_last_time = '1';
int accelerate = 1000; 

void Test_UASRT2(void)
{
	char txt[64];
	
	int duty = 2000;

	int balance = 600;
	
	int increased_balance = 1000;
	
	USART2_Init(115200);                //初始化USART2 115200 使能 并开启串口中断接受
   
  //OLED_Init();                         // OLED初始化
  //OLED_P8x16Str(10, 0,"Test USART2");  // 字符串
	//delay_1ms(50);
	//printf("USART2 Init OK \n");
  //USART_Sent_String(USART2,"USART2 Init OK\n");
	
	//while(1)
	{
		//串口中断服务中收什么发什么
		sprintf(txt, "RD:%c %c %c",receive_data, receive_data_last_time_rotate, receive_data_last_time);
		OLED_P8x16Str(0, 4, txt);
		sprintf(txt, "a: %d ",accelerate);
		OLED_P8x16Str(0, 2, txt);
		led_toggle();
		
		
		if(receive_data == '4')//nearer
		{
			accelerate = 0;
			receive_data = receive_data_last_time;
		}
		if(receive_data == 'a')//far
		{
			accelerate = 1000;
			receive_data = receive_data_last_time;
		}
		if(receive_data == '3')//turn left
		{
			MotorCtrl3W(0 , -duty , duty - balance);
			receive_data_last_time = receive_data;
			receive_data_last_time_rotate = receive_data;		
		}
		else if(receive_data == 'c')//turn left
		{
			MotorCtrl3W(0 , -duty , duty - increased_balance);
			receive_data_last_time = receive_data;
			receive_data_last_time_rotate = '3';			
		}
		else if (receive_data == '1')//straight forward
		{
		   MotorCtrl3W(0,-duty - accelerate, duty + accelerate);
			 receive_data_last_time = receive_data;
		}
		else if (receive_data == '2')//turn right
		{
		  MotorCtrl3W(0 , -duty + balance, duty);
			receive_data_last_time = receive_data;
			receive_data_last_time_rotate = receive_data;
		}
		else if (receive_data == 'b')//turn right
		{
		  MotorCtrl3W(0 , -duty + increased_balance, duty);
			receive_data_last_time = receive_data;
			receive_data_last_time_rotate = '2';
		}
		else if (receive_data == '0')//rotate
		{
			if(receive_data_last_time_rotate ==  '3')// turn right
			{
				MotorCtrl3W(-800 , -800, -800);
				receive_data = '0';
				receive_data_last_time = receive_data;
			}
			else if(receive_data_last_time_rotate ==  '2')// turn left 
			{
				MotorCtrl3W(800 , 800, 800);
				receive_data = '0';
				receive_data_last_time = receive_data;
			}
			else// turn left 
			{
				MotorCtrl3W(-800 , -800, -800);
				receive_data = '0';
				receive_data_last_time = receive_data;
			}
		}
		/*else if(receive_data == '4')//rotate
		{
			if(receive_data_last_time ==  '2')// turn right
			{
				MotorCtrl3W(1200 , 0 , 0 );
			}
			else if(receive_data_last_time ==  '3')// turn left 
			{
				MotorCtrl3W(-1200 , 0 , 0 );
			}
			else 
			{
				MotorCtrl3W(-1200 , 0 , 0 );
			}
		}*/
		else if(receive_data == '6')// right
		{
			MotorCtrl3W(-1500 , -800 , 800 );
			receive_data_last_time = receive_data;
			receive_data_last_time_rotate = '2';
		}
		else if(receive_data == '7')//left
		{
			MotorCtrl3W(1500 , -800 , 800 );
			receive_data_last_time = receive_data;
			receive_data_last_time_rotate = '3';
		}
		else if(receive_data == '8')//
		{
			MotorCtrl3W( 0, -2000 , 1200 );
			receive_data_last_time = receive_data;
			receive_data_last_time_rotate = '2';
		}
		else if(receive_data == '9')//
		{
			MotorCtrl3W( 0 , -1200, 2000 );
			receive_data_last_time = receive_data;
			receive_data_last_time_rotate = '3';
		}
		else if(receive_data == '5')
		{
			receive_data_last_time = receive_data;
			MotorCtrl3W(0, -2000, 2000);
			delay_1ms(150);
			MotorCtrl3W(0, 2000, -2000);
			delay_1ms(250);
			MotorCtrl3W(1000, 1000, 1000);
			delay_1ms(640);
		}
		else
		{
			receive_data_last_time = receive_data;
			MotorCtrl3W(800, 800, 800);
		}
		
	}
}

void Data_parsing()   //数据解析
{
	//帧格式，0xFA xx xx xx 0xAF
	
	;
}





