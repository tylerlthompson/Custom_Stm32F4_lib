/**
 * Driver for 16x2 Character LCD
 * Model: NHD-0216BZ-RN-YBW
 *
 * @author      Tyler Thompson
 * @date        3/19/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Character_LCD.h"


/**
  * @brief  Convert a single digit number to a char for LCD
  * @param  None
  * @retval None
  */
char int_to_char(uint8_t number) {
  return (number | 0x30);
}

void disp_write_string(char *display_string) {
  disp_clear();
  
  for(uint8_t i = 0; i < 32; i++){
    
    if (i == 16 ) {
      disp_command(0xC0); // set to second line
    }
    
    if(display_string[i] == '\0'){
      return;
    //  disp_write(0x00);
    }
      disp_write(display_string[i]);
      
  }
}

void disp_clear(void) {
  disp_command(0x01);
  disp_command(0x0C);
}

void disp_init(void){
  delay_init();
  disp_GPIO_Setup();
  GPIO_ResetBits(DISP_CONTROL_PORT, DISP_E_PIN); // E-> LOW
  delay(DISP_NYB_DELAY * 100);
  disp_command(0x30);
  delay(DISP_NYB_DELAY * 30);
  disp_command(0x30);
  delay(DISP_NYB_DELAY * 10);
  disp_command(0x30);;
  delay(DISP_NYB_DELAY * 10);
  disp_command(0x38);
  disp_command(0x10);
  disp_command(0x0F);
  disp_command(0x06);
  disp_clear();
}

void disp_write(char i) {
  DISP_DATA_PORT->ODR = i;
  GPIO_SetBits(DISP_CONTROL_PORT, DISP_RS_PIN); // D/I->HIGH send data
  disp_nybble();
}

void disp_command(char i) {
  DISP_DATA_PORT->ODR = i;
  GPIO_ResetBits(DISP_CONTROL_PORT, DISP_RS_PIN); // D/I->LOW send instruction
  disp_nybble();
}

static void disp_nybble(void){
  GPIO_SetBits(DISP_CONTROL_PORT, DISP_E_PIN); // E-> HIGH
  delay(DISP_NYB_DELAY);
  GPIO_ResetBits(DISP_CONTROL_PORT, DISP_E_PIN); // E-> LOW
}


static void disp_GPIO_Setup(void) {
  GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIOD clock enable */
  RCC_AHB1PeriphClockCmd(DISP_DATA_CLK, ENABLE);
  
  /* GPIOD Configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(DISP_DATA_PORT, &GPIO_InitStructure);

  /* GPIOA clock enable */
  RCC_AHB1PeriphClockCmd(DISP_CONTROL_CLK, ENABLE);
  
  /* GPIOA Configuration */
  GPIO_InitStructure.GPIO_Pin = DISP_E_PIN | DISP_RS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(DISP_CONTROL_PORT, &GPIO_InitStructure);
}
