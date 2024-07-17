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




uint8_t transmitter_buffer1[] = "HELLOWORLD";//���巢������
uint8_t receiver_buffer1[32];//�����������
#define ARRAYNUM1(arr_nanme)      (uint32_t)(sizeof(arr_nanme) / sizeof(*(arr_nanme)))
#define TRANSMIT_SIZE1   (ARRAYNUM1(transmitter_buffer1) - 1)//�����С
uint8_t transfersize1 = TRANSMIT_SIZE1;

uint8_t receivesize1 = 10;    //����
__IO uint8_t  txcount1 = 0; 
__IO uint16_t rxcount1 = 0; 

//-------------------------------------------------------------------------------------

 uint8_t  USART_Rx_Buf[USART_RECV_BUF_SIZE];  // ���ڱ��� RX ���յ�������
 uint16_t USART_Rx_Len = 0;                   // ���ڱ��� RX ���յ������ݵĸ���
 uint16_t USART_Rx_Count = 0;                 // ���ڱ��� RX ��������ʱ�Ѿ����յ������ݸ���
 uint8_t  USART_Tx_Buf[USART_SEND_BUF_SIZE];  // ���ڱ��� Tx Ҫ���͵�����
 uint16_t USART_Tx_Len = 0;                   // ���ڱ��� Tx Ҫ���͵����ݵĸ���
 uint16_t USART_Tx_Count = 0;                 // ���ڱ��� Tx Ҫ���͵�����ʱ�Ѿ����͵����ݸ���


/**
  * @brief �ض���c�⺯��printf��USART1
  * @retval None
  */
int fputc(int ch,FILE *p)  
{
	usart_data_transmit(USART1,(unsigned char)ch);
	while(usart_flag_get(USART1, USART_FLAG_TC)==RESET);
	return ch;
}

/**
  * @brief �ض���c�⺯��getchar,scanf��USART2
  * @retval None
  */
int fgetc(FILE *f)
{
    uint8_t ch2 = 0;
    ch2 = usart_data_receive(USART2);
    return ch2;
}


/********************************************************************
* ��������void UART1_init(void)
* ��  ��: uint32_t Baud_rate  ������
* ��  �ã�USART1_Init(115200);
* USART1��ʼ����ʹ��PA2(TX),PA3(RX)�ţ�115200�����ʣ���У�飬8λ���ݣ�1λֹͣ
********************************************************************/
void USART1_Init(uint32_t Baud_rate)
{
    rcu_periph_clock_enable(RCU_GPIOA);//ʹ��GPIOAʱ��
    rcu_periph_clock_enable(RCU_USART1);//ʹ��USART1ʱ��

    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_2);        //PA2  TX���
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ,GPIO_PIN_3);  //PA3  RX����
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
    usart_enable(USART1);//ʹ��USART1	
    
    nvic_irq_enable(USART1_IRQn, 0, 1);             //ʹ��USART1�ж�
	usart_interrupt_enable(USART1, USART_INT_RBNE); //�����жϴ�
}
/********************************************************************
* ��������void UART2_init(void)
* ��  ��: uint32_t Baud_rate  ������
* ��  �ã�USART2_Init(115200);
* USART2��ʼ����ʹ��PB10(TX),PB11(RX)�ţ�115200�����ʣ���У�飬8λ���ݣ�1λֹͣ
********************************************************************/
void USART2_Init(uint32_t Baud_rate)
{
    rcu_periph_clock_enable(RCU_GPIOB);//ʹ��GPIOAʱ��
    rcu_periph_clock_enable(RCU_USART2);//ʹ��USART1ʱ��

    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_10);        //PB10  TX���
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ,GPIO_PIN_11);  //PB11  RX����
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
    usart_enable(USART2);//ʹ��USART2
    
    nvic_irq_enable(USART2_IRQn, 0, 0);             //ʹ��USART2�жϣ���ռ���ȼ��ʹ����ȼ���ԽСԽ��
		usart_interrupt_enable(USART2, USART_INT_RBNE); //�շ��жϴ�
}



void USART_Sent_Byte(uint32_t usart_periph,uint8_t byte)
{
    usart_data_transmit(USART2, byte);	    					//ͨ��USART2����
    while(RESET == usart_flag_get(USART2, USART_FLAG_TBE));		//�жϻ������Ƿ��Ѿ�����
}


//����Ϊͨ��
void USART_Sent_Data(uint32_t usart_periph ,uint8_t *data)
{
    uint32_t len;
    len = sizeof(*data) / sizeof(data[0]);
    while(data && len)
	{
		USART_Sent_Byte(usart_periph,*data);
		data++;		//ָ��������һλ
        len--;
	}
}

void USART_Sent_String(uint32_t usart_periph,char *str)
{
	while(str && (*str))
     {
		USART_Sent_Byte(usart_periph,(uint8_t)*str);
		str++;		// ָ������һ���ƶ�
     }
}



void Test_UASRT1(void)
{
	//USART�������
	USART1_Init(115200);                //��ʼ��USART 115200 ʹ�� �����������жϽ���
	USART2_Init(115200);                //��ʼ��USART 115200 ʹ�� �����������жϽ���

  OLED_Init();                         // OLED��ʼ��
  OLED_P6x8Str(10, 0,"Test USART1");  // �ַ���
	delay_1ms(50);
	printf(" USART1 Init ok \n");
	
	while(1)
	{
		if(rxcount1 >= receivesize1)//������10���ֽڣ���USART0_IRQHandler�����м���
		{
			rxcount1=0;
			txcount1=0;
			usart_interrupt_enable(USART1, USART_INT_TBE);//�����жϴ�
			while(txcount1 < transfersize1);//�ȴ�������ɣ���USART0_IRQHandler�����м���
			while (RESET == usart_flag_get(USART1, USART_FLAG_TC));//��������ж�
			usart_interrupt_enable(USART1, USART_INT_RBNE);//�����жϴ�
			printf("\nReceive1 OK \n");
		}
	}
}

unsigned int receive_data;
unsigned char receive_data_last_time_rotate = '1';
unsigned char receive_data_last_time = '1';
int accelerate = 1000; 
int increased_rotate = 0;
int goal_accel = 0;

void Test_UASRT2(void)
{
	char txt[64];
	
	int duty = 1500;

	int balance = 600;
	
	int increased_balance = 1000;
	
	USART2_Init(115200);                //��ʼ��USART2 115200 ʹ�� �����������жϽ���
   
  //OLED_Init();                         // OLED��ʼ��
  //OLED_P8x16Str(10, 0,"Test USART2");  // �ַ���
	//delay_1ms(50);
	//printf("USART2 Init OK \n");
  //USART_Sent_String(USART2,"USART2 Init OK\n");
	
	//while(1)
	{
		//�����жϷ�������ʲô��ʲô
		sprintf(txt, "RD:%c %c %c",receive_data, receive_data_last_time_rotate, receive_data_last_time);
		OLED_P8x16Str(0, 4, txt);
		sprintf(txt, "speed: %d ",accelerate+duty+goal_accel);
		OLED_P8x16Str(0, 2, txt);
		led_toggle();
		
		
		if(receive_data == '4')//nearer
		{
			accelerate = 0;
			goal_accel = goal_accel + 100;
			if(goal_accel >= 1500)
				goal_accel = 1500;
			receive_data = receive_data_last_time;
		}
		if(receive_data == 'a')//far
		{
			accelerate = 2000;
			receive_data = receive_data_last_time;
			goal_accel = 0;
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
		   MotorCtrl3W(0,-duty - accelerate-goal_accel, duty + accelerate + goal_accel);
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
			MotorCtrl3W(-1500 , -goal_accel-800 , 800+goal_accel );
			receive_data_last_time = receive_data;
			receive_data_last_time_rotate = '2';
		}
		else if(receive_data == '7')//left
		{
			MotorCtrl3W(1500 , -goal_accel-800 , 800+goal_accel );
			receive_data_last_time = receive_data;
			receive_data_last_time_rotate = '3';
		}
		else if(receive_data == '8')// left
		{
			MotorCtrl3W( -increased_rotate, -duty-500-goal_accel-increased_rotate , duty-300+goal_accel);
			receive_data_last_time = receive_data;
			receive_data_last_time_rotate = '3';
		}
		else if(receive_data == '9')// right 
		{
			MotorCtrl3W( increased_rotate , -duty+300-goal_accel, duty+500+goal_accel+increased_rotate);
			receive_data_last_time = receive_data;
			receive_data_last_time_rotate = '2';
		}
		else if(receive_data == '5')
		{
			receive_data_last_time = receive_data;
			MotorCtrl3W(0, 2000, -2000);
			delay_1ms(250);
			MotorCtrl3W(0, -2000, 2000);
			delay_1ms(200);
			
			
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
			delay_1ms(400);
		}
		else if(receive_data == 'd') // right
		{
			MotorCtrl3W(-1500 , -1000 , 2000 );
			delay_1ms(500);
		}
		else if(receive_data == 'e') // left
		{
			MotorCtrl3W(1500 , -2000 , 1000 );
			delay_1ms(500);
		}
		else if(receive_data == 'f') //  postion tag1
		{
			MotorCtrl3W(1500 , -1500 , 0 );
			delay_1ms(500);
		}
		else if(receive_data == 'g') // position tag2
		{
			MotorCtrl3W(-1500 , 0 , 1500 );
			delay_1ms(500);
		}
		else
		{
			receive_data_last_time = receive_data;
			MotorCtrl3W(800, 800, 800);
		}
		
	}
}

void Data_parsing()   //���ݽ���
{
	//֡��ʽ��0xFA xx xx xx 0xAF
	
	;
}





