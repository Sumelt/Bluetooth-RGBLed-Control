/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  MCD Application Team
  * @version V2.1.0
  * @date    18-November-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stm8s_it.h"
#include <string.h>
#include <stdlib.h>

/* Private defines -----------------------------------------------------------*/
#define LED_GPIOB_PORT  (GPIOB) //红
#define LED_GPIOC_PORT  (GPIOC)  //蓝
#define LED_GPIOD_PORT  (GPIOD)  //绿

#define LEDR_GPIO_PINS  (GPIO_PIN_5) //红
#define LEDG_GPIO_PINS  (GPIO_PIN_4) //绿10
#define LEDB_GPIO_PINS  (GPIO_PIN_3) //蓝08

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
#define UART_BUFF_SIZE 10
extern uint8_t uart_buff[UART_BUFF_SIZE];
extern volatile uint16_t uart_p;
unsigned int flag = 0;
/*------------------------main()---------------------------------*/
/**************延迟函数*****************/
static void delay(unsigned long nCount)
{
  while (nCount != 0) {
    nCount--;
  }
}

/*****************  GPIO_Init **********************/
static void GPIO_Config(GPIO_TypeDef *GPIO_PORT, GPIO_Pin_TypeDef GPIO_PINS)
{
  GPIO_Init(GPIO_PORT, GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST);
    
}

/*****************  发字符 **********************/
void uart_SendByte(uint8_t data)
{
  UART1_SendData8(data);
  //UART1_SendData9(data);
  delay(0xff);
}
/*****************  发字符串 **********************/
void uart_SendString(unsigned char Str[])
{
  unsigned char n = 0;
  while(Str[n])
    uart_SendByte(Str[n++]);
}

//清空数据
void clean_rebuff(void)
{
    uint16_t i=100+1;
    uart_p = 0;
      while(i)
	uart_buff[--i]=0;
}



void main(void)
{
  enableInterrupts(); //开启中断
  
  GPIO_Config(LED_GPIOB_PORT,LEDR_GPIO_PINS);
  GPIO_Config(LED_GPIOD_PORT,LEDG_GPIO_PINS);
  GPIO_Config(LED_GPIOC_PORT,LEDB_GPIO_PINS);
  
  GPIO_WriteHigh(LED_GPIOB_PORT,LEDR_GPIO_PINS);
  GPIO_WriteHigh(LED_GPIOD_PORT,LEDG_GPIO_PINS);
  GPIO_WriteHigh(LED_GPIOC_PORT,LEDB_GPIO_PINS);
  
  UART1_DeInit();  
  
  //波特率9600，1个停止位，无奇偶校验位，不需要同步时钟,启用发送和接收
  UART1_Init((u32)9600, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, 
  UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);//UART1_MODE_TX_ENABLE);
  
  UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE); //启用接收数据的中断
  
  
  while (1)
 {
      if(strcmp((const char *)uart_buff,"R&")==0)
        {
           uart_SendString("OK");
           GPIO_WriteReverse(LED_GPIOB_PORT, LEDR_GPIO_PINS);
           clean_rebuff(); 
        }
    
   
      else if(strcmp((const char *)uart_buff,"G&")==0)
	 {
	    uart_SendString("OK");
	    GPIO_WriteReverse(LED_GPIOD_PORT, LEDG_GPIO_PINS);
	    clean_rebuff(); 
	 }
	   
      else if(strcmp((const char *)uart_buff,"B&")==0)
	 {
            uart_SendString("OK");
            GPIO_WriteReverse(LED_GPIOC_PORT, LEDB_GPIO_PINS);
            clean_rebuff(); 
	 }
	   
		/*if(uart_buff[uart_p]=='&')
		{
		  uart_SendString(uart_buff); 
		  clean_rebuff();
		}*/
		//clean_rebuff();
		
      else if(strcmp((const char *)uart_buff,"S&")==0)
	{
      
	    if((GPIOB->ODR&0x20)==0||(GPIOD->ODR&0x10)==0||(GPIOC->ODR&0x08)==0)
			  
	      {
	          clean_rebuff();
			
		    while(1)
		    {
					  //GPIO_WriteReverse(LED_GPIOB_PORT, LEDR_GPIO_PINS);
					  //delay(0xfff);
					  //delay(0xff);
		        if((GPIOB->ODR&0x20)==0) //red b
			  {        
			    delay(0xfff);
                            delay(0xff);
                            GPIO_WriteHigh(LED_GPIOB_PORT,LEDR_GPIO_PINS);
                            delay(0xfff);
                            delay(0xff);
                            GPIO_WriteLow(LED_GPIOB_PORT,LEDR_GPIO_PINS);
			  }
					  //GPIOC->ODR&0x18==0||GPIOC->ODR&0x08==0||
			  if((GPIOD->ODR&0x10)==0)  // green b
			    {        
				delay(0xfff);
				delay(0xff);
			        GPIO_WriteHigh(LED_GPIOD_PORT,LEDG_GPIO_PINS);
				delay(0xfff);
			        delay(0xff);
			        GPIO_WriteLow(LED_GPIOD_PORT,LEDG_GPIO_PINS);
			    }
					  
			    if((GPIOC->ODR&0x08)==0)  //blue c
			      {        
				delay(0xfff);
				delay(0xff);
				GPIO_WriteHigh(LED_GPIOC_PORT,LEDB_GPIO_PINS);
				delay(0xfff);
				delay(0xff);
				GPIO_WriteLow(LED_GPIOC_PORT,LEDB_GPIO_PINS);
			      }
		/*******多闪********/
		if(((GPIOB->ODR&0x20)==0&&(GPIOC->ODR&0x10)==0)||((GPIOB->ODR&0x20)==0&&(GPIOD->ODR&0x08)==0)||((GPIOD->ODR&0x10)==0&&(GPIOC->ODR&0x08)==0)) 
						 
		{        
		    if((GPIOB->ODR&0x20)==0&&(GPIOC->ODR&0x08)==0)
			{
                          delay(0xfff);
                          delay(0xff);
                          GPIO_WriteHigh(LED_GPIOC_PORT,LEDB_GPIO_PINS);
                          delay(0xfff);
                          delay(0xff);
                          GPIO_WriteLow(LED_GPIOC_PORT,LEDB_GPIO_PINS);
			}
					  
		    if((GPIOB->ODR&0x20)==0&&(GPIOD->ODR&0x10)==0)
			{
                          delay(0xfff);
                          delay(0xff);
                          GPIO_WriteHigh(LED_GPIOD_PORT,LEDG_GPIO_PINS);
                          delay(0xfff);
                          delay(0xff);
                          GPIO_WriteLow(LED_GPIOD_PORT,LEDG_GPIO_PINS);
			}
		    if((GPIOD->ODR&0x10)==0&&(GPIOC->ODR&0x08)==0)
			{
						  
                          delay(0xfff);
                          delay(0xff);
                          GPIO_WriteHigh(LED_GPIOD_PORT,LEDG_GPIO_PINS);
                          delay(0xfff);
                          delay(0xff);
                          GPIO_WriteLow(LED_GPIOD_PORT,LEDG_GPIO_PINS);                   
			}
		    if((GPIOD->ODR&0x10)==0&&(GPIOC->ODR&0x08)==0&&(GPIOB->ODR&0x20)==0)
			{
			  delay(0xfff);
                          delay(0xff);
                          GPIO_WriteHigh(LED_GPIOB_PORT,LEDR_GPIO_PINS);
                          delay(0xfff);
                          delay(0xff);
                          GPIO_WriteLow(LED_GPIOB_PORT,LEDR_GPIO_PINS);
						  
                          delay(0xffff);
                          delay(0xff);
		          GPIO_WriteHigh(LED_GPIOD_PORT,LEDG_GPIO_PINS);
		          delay(0xfff);
                          delay(0xff);
                          GPIO_WriteLow(LED_GPIOD_PORT,LEDG_GPIO_PINS);
									
			}
					   
		  }
					  
		    if(strcmp((const char *)uart_buff,"S&")==0)
			{
			  if((GPIOB->ODR&0x20)==0)  
				GPIO_WriteHigh(LED_GPIOB_PORT,LEDR_GPIO_PINS);
											
			  if((GPIOD->ODR&0x10)==0)  
				GPIO_WriteHigh(LED_GPIOD_PORT,LEDG_GPIO_PINS);
											
			  if((GPIOC->ODR&0x08)==0)  
				GPIO_WriteHigh(LED_GPIOC_PORT,LEDB_GPIO_PINS);
						   
			  clean_rebuff();
			  break;
			} 
                        clean_rebuff();
	     }
	 }
     }
  }
}
#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/