#include "urat.h"
#include "string.h"
#define test
#ifdef test
uint8_t temp,Rxbuff[256],len,flag;
UART_HandleTypeDef huart2;
void MX_USART2_UART_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	
	if (HAL_UART_Init(&huart2) != HAL_OK)
	{
		;//_Error_Handler(__FILE__, __LINE__);
	}

	HAL_UART_Receive_IT(&huart2,&temp,1);
	HAL_NVIC_SetPriority(USART2_IRQn, 7, 0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	if(uartHandle->Instance==USART2)
	{
		/* USART2 clock enable */
		__HAL_RCC_USART2_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**USART2 GPIO Configuration    
		PA2     ------> USART2_TX
		PA3     ------> USART2_RX 
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_2;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_3;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}
void uart2_IRQHandler(void);
void USART2_IRQHandler(void)
{
	//HAL_UART_IRQHandler(&huart2);	//调用HAL库中断处理公用函数
	//testSemaphore();
	uart2_IRQHandler();
}
#define START	1
#define	STATR1	2

typedef struct 
{
	uint8_t flag;
	uint8_t over;
	uint8_t temp;
}Rx;
Rx	UartRx;
void uart2_IRQHandler()
{
	if((__HAL_UART_GET_FLAG(&huart2,UART_FLAG_RXNE)!=RESET))//有数据来
	{
		__HAL_UART_CLEAR_FLAG(&huart2,UART_FLAG_RXNE);
		UartRx.temp = (uint8_t)(huart2.Instance->DR);
		if(UartRx.flag == START)
		{
			UartRx.flag = STATR1;
			len = 0;
			Rxbuff[len++] = UartRx.temp;
		}
		else if(UartRx.flag == STATR1)
		{
			Rxbuff[len++] = UartRx.temp;		
		}
	}
	if((__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE)!=RESET))  //进入空闲中断
	{
		__HAL_UART_CLEAR_IDLEFLAG(&huart2);
		if(UartRx.flag == STATR1)
		{
			UartRx.over = 0x01;
		}
	}

}
extern OS_TCB Task2_TaskTCB;
void test_task()
{
	OS_ERR err;
	if(Rxbuff[0] == 'R')
	{
		OSTaskSuspend ((OS_TCB  *)&Task2_TaskTCB, 
                       (OS_ERR  *)&err);
	}
	if(Rxbuff[0] == 'W')
	{
		OSTaskResume ((OS_TCB  *)&Task2_TaskTCB, 
                      (OS_ERR  *)&err);
	}
}
void init_t()
{
	UartRx.over = 0x00;
	len = 0;
	UartRx.flag = START;
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);  //打开空闲中断
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_PE);
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_ERR);
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
}
void printf_test()
{
	//printf("test打印!");
	if((UartRx.over) == 0x01)
	{
		printf("=%s\r\n",Rxbuff);
		//HAL_UART_Transmit(&huart2,Rxbuff,strlen((char *)Rxbuff),0xff);
		test_task();
		
		
		memset(Rxbuff,0,256);
		UartRx.over = 0x00;
		UartRx.flag = START;
	}

}
#endif

#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART2->SR&0X40)==0);//循环发送,直到发送完毕   
	USART2->DR = (u8) ch;      
	return ch;
}
#endif

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if(huart->Instance == USART2)
//	{
//		HAL_UART_Receive_IT(&huart2,&temp,1);
//		HAL_UART_Transmit_IT(&huart2,&temp,1);
////		if(temp == 'R')
////		{
////			len = 0;
////		}
////		Rxbuff[len++] = temp;
////		if(temp == '\n')
////		{
////			Rxbuff[len] = temp;
////			flag = 1;
////		}
//		
//	}
//}





