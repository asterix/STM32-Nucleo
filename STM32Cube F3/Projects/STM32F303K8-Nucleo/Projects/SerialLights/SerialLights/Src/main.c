/*-----------------------------------------------------------------------------------
*  Author   : xfrings
*  Created  : 10-Dec-2015
*  Hardware : STM32F303K8 Nucleo 
*  Copyright (c) 2015 xfrings


*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to deal
*  in the Software without restriction, including without limitation the rights
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*  copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:

*  The above copyright notice and this permission notice shall be included in
*  all copies or substantial portions of the Software.

*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*  THE SOFTWARE.
-----------------------------------------------------------------------------------*/

#include "main.h"


/* Defines */
#define NUM_PIXELS 150
#define PIXEL_PORT GPIOB
#define PIXEL_PIN  GPIO_PIN_0
#define WS2812B 1



/* Globals */
/* Strip of all LEDs */
pixel strip[NUM_PIXELS];
static GPIO_InitTypeDef  GPIO_Initr;



/* Main */
int main(void)
{
	
	/* Initialize HAL */
  HAL_Init();

  /* SYSCLK = 64 MHz, HSI */
  config_system_clock();
  
  /* Initialize everything */
  init_all();
	
  /* Standard peripheral allowance */	
  HAL_Delay(100);
	
	for(int i = 0; i < NUM_PIXELS; i++)
	{
		strip_set_pixel_color(strip, i, 222, 255, 0);
	}
	
	while(1)
	{
	  strip_display_all(strip);
	}
	
	

}


void strip_set_pixel_color(pixel *leds, int pixel_index, uint8_t r, uint8_t g, uint8_t b)
{
	leds[pixel_index].r = r;
	leds[pixel_index].g = g;
	leds[pixel_index].b = b;
}



void strip_display_all(pixel *leds)
{
	int i, j;
	uint8_t current_r, current_g, current_b;
	uint16_t high = 0, low = 0;
	uint32_t current_pixel_grb, mask = 0x80000000;
	
	/* Maintain other bits of PORTB */
	high = PIXEL_PORT->ODR | (PIXEL_PIN);
	low = PIXEL_PORT->ODR & (~PIXEL_PIN);
	
	/* Over all pixels */
	for(i = 0; i < NUM_PIXELS; i++)
	{
		current_g = leds[i].g;
		current_r = leds[i].r;
		current_b = leds[i].b;
		
		current_pixel_grb  = (current_g << 24);
		current_pixel_grb |= (current_r << 16);
		current_pixel_grb |= (current_b << 8);
		
		/* 24 bits in G-R-B */
		for(j = 0; j < 24; j++)
		{
			if((mask >> j) & current_pixel_grb)
			{
				/* Bit is 1 */
				PIXEL_PORT->ODR = high;
      	
      	/* Nominal WS2812B - T1H = 700ns */
				/* Actual T1H = 750ns */
      	__asm volatile(
      		"mov r0, r0; nop; nop; nop; nop;\
      		nop; nop; nop; nop; nop;\
      		nop; nop; nop; nop; nop;\
      		nop; nop; nop; nop; nop;\
				  nop; nop; nop; nop; nop;\
				  nop; nop; nop; nop; nop;\
      		nop; nop; nop; nop; nop;\
				  nop;");
				
				/* WS2812B - T1L = 600ns */
				/* Actual T1L = 520ns */				
				PIXEL_PORT->ODR = low;
				
        __asm volatile(
      		"mov r0, r0; nop; nop; nop;\
				  nop; nop; nop; nop; nop;");
			}
			else
			{
				/* Bit is 0 */
				
				/* Nominal WS2812B - T0H = 350ns */
				/* Actual T0H = 350ns */
				PIXEL_PORT->ODR = high;
				
        __asm volatile(
      		"mov r0, r0; nop; nop; nop; nop;\
      		nop; nop; nop; nop; nop;\
      		nop; nop; nop; nop; nop;\
				  nop;");
				
      	/* Nominal WS2812B - T0L = 800ns */
				/* Actual T0L = 910ns */
				PIXEL_PORT->ODR = low;
				
      	__asm volatile(
      		"mov r0, r0; nop; nop; nop; nop;\
      		nop; nop; nop; nop; nop;\
      		nop; nop; nop; nop; nop;\
      		nop; nop; nop; nop; nop;\
				  nop; nop; nop; nop; nop;\
				  nop; nop; nop; nop; nop;\
				  nop; nop;");
				
			}
		}
	}
	
	/* Reset pulse > 50us */
	PIXEL_PORT->ODR = low;
	HAL_Delay(5);

}
	

/*--------------------------------------------------------------------
  init_all()
----------------------------------------------------------------------
-  Generic init routine
--------------------------------------------------------------------*/
void init_all()
{
	/* GPIO PORT-B Clock Enable */
  __GPIOB_CLK_ENABLE();

  /* Set I/O directions */
  GPIO_Initr.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_Initr.Pull  = GPIO_PULLUP;
  GPIO_Initr.Speed = GPIO_SPEED_HIGH;

	/* LED3 pin */
  GPIO_Initr.Pin = GPIO_PIN_3;
  HAL_GPIO_Init(GPIOB, &GPIO_Initr);
	
	/* PORT-B0 pin */
	GPIO_Initr.Pin = GPIO_PIN_0;
  HAL_GPIO_Init(GPIOB, &GPIO_Initr);
}


/*--------------------------------------------------------------------
  config_system_clock()
----------------------------------------------------------------------
-  System Clock source  = PLL (HSI)
-  SYSCLK(Hz)           = 64000000
-  HCLK(Hz)             = 64000000
-  AHB Prescaler        = 1
-  APB1 Prescaler       = 2
-  APB2 Prescaler       = 1
-  PLLMUL               = RCC_PLL_MUL16 (16)
-  Flash Latency(WS)    = 2
--------------------------------------------------------------------*/
void config_system_clock(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
	/* System Clock */
  /* HSI - On (8MHz), PLL - turn on with source = HSI/2 (4MHz), PLL Multiplier = 16 (64MHz) */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
  RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL     = RCC_PLL_MUL16;
	
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    /* Catch Error */
    while(1); 
  }

	/* Peripheral Clocks */
	/* AHB = SYSCLK/1 (64MHz), APB1 = AHB/2 (PCLK1 = 32MHz), APB2 = AHB/1 (64MHz) */
  RCC_ClkInitStruct.ClockType      = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    /* Catch Error */
    while(1); 
  }
}

