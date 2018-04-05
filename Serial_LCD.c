/**
 * Driver for serial LCD
 * Model: ST7920
 *
 * @author      Tyler Thompson
 * @date        3/27/2018
 */

/* Includes ------------------------------------------------------------------*/
#include "Serial_LCD.h"
#include "Timing_Delay.h"

__IO uint8_t tx_index;
__IO char tx_buffer[3] = { 0x0, 0x0, 0x0 };

/**
  * @brief  This function handles SPI interrupt request.
  * @param  None
  * @retval None
  */
void SLCD_SPI_IRQHANDEL(void)
{
  /* SPI in Tramitter mode */
  if (SPI_I2S_GetITStatus(SLCD_SPI, SPI_I2S_IT_TXE) == SET)
  { 
    if (tx_index < 3)
    {
      /* Send Transaction data */
      SPI_I2S_SendData(SLCD_SPI, tx_buffer[tx_index++]);   
    }
    else
    {
      /* Disable the Tx buffer empty interrupt */
      SPI_I2S_ITConfig(SLCD_SPI, SPI_I2S_IT_TXE, DISABLE);
    }
  }
}

/**
  * @brief  Send a string to the LCD
  * @param  *data - the string to send
  * @retval None
  */
void slcd_send_string(char *data) {
  slcd_clear();
  for(uint8_t i = 0; i < 32; i++) {
    if( data[i] == '\0' ) {
      return;
    }
    slcd_send_char(data[i]);
  }
}

/**
  * @brief  Send a single character to the LCD.
  * @param  data - The character to send.
  * @retval None
  */
void slcd_send_char(char data) {
  char higher_bits = data & 0xF0;
  char lower_bits = (data & 0x0F) << 4;
  slcd_send_data(0xFA, higher_bits, lower_bits);
}

/**
  * @brief  Clear the LCD
  * @param  None
  * @retval None
  */
void slcd_clear(void){ 
  slcd_send_data(0xF8, 0x0, 0x10); // clear
  slcd_send_data(0xF8, 0x0, 0x20); // return home
}

/**
  * @brief  Send raw instruction or data to the LCD.
  * @param  cmd_bits - Instruction bits
  * @param  higher_bits - Higher order bits
  * @param  lower_bits - Lower order bits
  * @retval None
  */
static void slcd_send_data(char cmd_bits, char higher_bits, char lower_bits) {
  tx_index = 0;
  tx_buffer[0] = cmd_bits;
  tx_buffer[1] = higher_bits;
  tx_buffer[2] = lower_bits;
  SPI_I2S_ITConfig(SLCD_SPI, SPI_I2S_IT_TXE, ENABLE);
   /* Waiting the end of Data transfer */
  while (tx_index < 3){}
  delay(1); // 1 millisecond delay
}

/**
  * @brief  Initialize the LCD
  * @param  None
  * @retval None
  */
void slcd_init(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

  /* Peripheral Clock Enable -------------------------------------------------*/
  /* Enable the SPI clock */
  SLCD_SPI_CLK_INIT(SLCD_SPI_CLK, ENABLE);
  
  /* Enable GPIO clocks */
  RCC_AHB1PeriphClockCmd(SLCD_GPIO_CLK, ENABLE);

  /* SPI GPIO Configuration --------------------------------------------------*/
  /* GPIO Deinitialisation */
  //GPIO_DeInit(SLCD_GPIO_PORT);
  
  /* Connect SPI pins to AF5 */  
  GPIO_PinAFConfig(SLCD_GPIO_PORT, SLCD_SCK_SOURCE, SLCD_SPI_AF);   
  GPIO_PinAFConfig(SLCD_GPIO_PORT, SLCD_MOSI_SOURCE, SLCD_SPI_AF);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = SLCD_SCK_PIN;
  GPIO_Init(SLCD_GPIO_PORT, &GPIO_InitStructure);  

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  SLCD_MOSI_PIN;
  GPIO_Init(SLCD_GPIO_PORT, &GPIO_InitStructure);
 
  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(SLCD_SPI);
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  
  SPI_Init(SLCD_SPI, &SPI_InitStructure);
  
  /* Configure the Priority Group to 1 bit */                
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* Configure the SPI interrupt priority */
  NVIC_InitStructure.NVIC_IRQChannel = SLCD_SPI_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable the SPI peripheral */
  SPI_Cmd(SLCD_SPI, ENABLE);
  
  /* Enable the Tx buffer empty interrupt */
  //SPI_I2S_ITConfig(SLCD_SPI, SPI_I2S_IT_TXE, ENABLE);
  
  tx_index = 0;

  delay_init();
  slcd_send_data(0xF8, 0x0, 0x10); // clear
  slcd_send_data(0xF8, 0x0, 0x20); // return home
  slcd_send_data(0xF8, 0x0, 0x60); // entry mode
  slcd_send_data(0xF8, 0x0, 0xC0); // display control
}

