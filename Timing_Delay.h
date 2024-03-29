/**
 * Various timing delay functions
 *
 * @author      Tyler Thompson
 * @date        3/21/18
 */

#define __TIMING_DELAY_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;
static uint8_t systick_initialized;

/* Private functions ---------------------------------------------------------*/
void TimingDelay_Decrement(void);

/* Public functions ---------------------------------------------------------*/
void delay_init(void);
void delay(__IO uint32_t nTime);   
void delay_micro(__IO uint32_t micro_time);


