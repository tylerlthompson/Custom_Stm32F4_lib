/**
 * ECE4510 Lab9 Task 3
 *
 * @author      Tyler Thompson
 * @date        3/20/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Custom_STM32F4_lib\Character_LCD.h"
#include "Custom_STM32F4_lib\UltraSonicSensor.h"
#include "Custom_STM32F4_lib\DIP_Switch.h"


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  
 disp_init(); // LCD initialization
 
 dip_init(); // DIP switch initialization
 
 ultrasonic_init(); // ultra sonic sensor initialization
  
  while (1)
  {
   
  }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif