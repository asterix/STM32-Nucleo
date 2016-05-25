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

#ifndef _SERIALLEDS_MAIN_H_
#define _SERIALLEDS_MAIN_H_


/* Includes */
#include "stm32f3xx_hal.h"
#include "stm32f3xx_nucleo_32.h"
#include "stdlib.h"


/* New types */
typedef struct
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
}pixel;

/* Prototypes */
void config_system_clock(void);

void init_all(void);

void strip_set_pixel_color(pixel *leds, int pixel_index, uint8_t r, uint8_t g, uint8_t b);

void strip_display_all(pixel *leds);

int random(int min, int max);

void random_color(pixel *pix, int min, int max);

void led_dance(void);

int mode_randomize(int p);

void switch_mode(void);

void randomize_vars(void);

void check_all_bounds(void);

void stay_stationary(int time);

#endif /* _SERIALLEDS_MAIN_H_ */
