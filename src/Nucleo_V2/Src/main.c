
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f7xx_hal.h"
#include "COBS.h"
#include "protocol.h"
#include <stdint.h>

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi3;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI3_Init(void);
char checkReceivedData(uint8_t *arr);

int checkCorrect(protocol_u *pro)
{
	protocol_u protocolstream = *pro;
	if(protocolstream.protocol_s.startByte == 0x01 && protocolstream.protocol_s.angle_pitch == 1 && protocolstream.protocol_s.test_int_5 == 8 && protocolstream.protocol_s.test_int_12 == 15 && protocolstream.protocol_s.test_u16_1 == 16 && protocolstream.protocol_s.test_u16_2 == 17)
		return 1;
	else
		return 0;
}

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init(); //Master
  MX_SPI3_Init(); //Slave

  char state = 0;

  uint8_t input_slave[num_sum+2];
  uint8_t output_slave[4];//Maximum Buffer size

  uint8_t COBS_len = 0;

  protocol_u protocolstream;

 volatile uint32_t successful_protocol_real = 0;
 volatile uint32_t successful_protocol_unreal = 0;
 volatile uint32_t successful_real = 0;
 volatile uint32_t unsuccessful_COBS = 0;
 volatile uint32_t unsuccessful_checksum = 0;
 volatile uint32_t unsuccessful_real = 0;



  while (1)
  {
	  switch(state)
	  {
	  case 0: /* --- Start / Initializaiton --- */
		  state = 1;
		  //HAL_SPI_Receive(&hspi3, input_slave, 4, 0xFF); //Clean Input FIFO
		  //HAL_SPI_Transmit(&hspi3, output_slave, 4, 0xFF); //Clean Output FIFO
		  break;

	  case 1: /* ---Two-Way-Handshake --- */

		  /* --- Waiting for request --- */
		  input_slave[0] = 0xFF; // Default Values
		  input_slave[1] = 0xFF; // Default Values
		  char offset = 0;
		  volatile uint32_t handC = 0;
		  while(!((input_slave[0] == 0x00 && input_slave[1] == 0x01) || (input_slave[0] == 0x01 && input_slave[1] == 0x00)))
		  {
			  if(offset == 0) //Switching Offset between '0' and '1' to receive starting sequence immediately
				  offset = 1;
			  else
				  offset = 0;

			  HAL_SPI_Receive(&hspi3, input_slave + offset, 1, 0xFF);
			  handC++;
		  }

		  /* --- Sending ACK --- */
		  output_slave[0] = 0xFF;
		  output_slave[1] = 0xFE;
		  HAL_SPI_Transmit(&hspi3, output_slave, 2, 0xFF);

		  state = 2;
		  break;

	  case 2: /* --- Read PDU--- */

		  /*
		  HAL_SPI_Receive(&hspi3, input_slave, 1, 0xFF);
		  uint16_t pos = 1;
		  while(input_slave[pos-1] != 0x00) //Reading until '0x00' is received (COBS - boundary), ToDo: Timeout when '0x00' is never received
		  {
			  HAL_SPI_Receive(&hspi3, input_slave + pos, 1, 0xFF); //Read Byte for Byte until '0x00'
			  pos++;
		  }
		  COBS_len = pos; //Store number of received Bytes for COBS decoding

		  state = 3;
		  break;

		   */
		  // --- Alternative Read if 'HAL_SPI_Receive(&hspi3, input_slave + pos, 1, 0xFF)' isn't working by higher transfer speeds ---

		  HAL_SPI_Receive(&hspi3, input_slave, num_sum+2, 0xFF); //Read complete PDU

		  //Check for '0x00' position (COBS - boundary)
		  uint16_t pos = 0;
		  while(input_slave[pos++] != 0x00);
		  pos--;

		  //Check if Position of '0x00' is how expected
			  COBS_len = pos; //Store number of received Bytes for COBS decoding
			  //ToDo: Reaction when COBS's Zero is found at wrong position

		  state = 3;
		  break;








	  case 3: /* --- Decode the input (COBS) --- */
		  if(num_sum == decode_COBS(input_slave, COBS_len, protocolstream.bytestream))
		  {
			  changeMSB_LSB(&protocolstream);
			  if(checkChecksum(&protocolstream))
			  {
				  if(checkCorrect(&protocolstream))
					  successful_protocol_real++;
				  else
					  successful_protocol_unreal++;
			  } else
			  {
				unsuccessful_checksum++;
			  }
		  } else
			unsuccessful_COBS++;

		  //Check for real successful_real (ToDo: Use Hexa Code)
//			if(protocolstream.protocol_s.startByte == 0x01 && protocolstream.protocol_s.test_int_1 == 1000 && protocolstream.protocol_s.angle_yaw == -300 && protocolstream.protocol_s.test_int_11 == 11000)

			if(checkCorrect(&protocolstream))
				successful_real++;
			else
				unsuccessful_real++;

			  //Sensor data is now transmitted and successfully stored in protocolstream struct for easy access like 'protocolstream.protocolstream.protocol_s.angle_pitch'
			  state = 1;
		  break;

	  }

  }

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* SPI3 init function */
static void MX_SPI3_Init(void)
{

  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_SLAVE;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_HARD_INPUT;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 7;
  hspi3.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi3.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin : SPI1 Pins (MASTER) */
  GPIO_InitStruct.Pin       = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7; //SCK | MISO | MOSI
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin  = GPIO_PIN_4; //NSS
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI3 Pins (SLAVE) */
  GPIO_InitStruct.Pin       = GPIO_PIN_11; //MISO
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI3;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin  = GPIO_PIN_10; //SCK
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin  = GPIO_PIN_2; //MOSI
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin  = GPIO_PIN_15; //NSS
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/** Check for expected result of received Data.
 * @return Array as expected (0: false)
*/
char checkReceivedData(uint8_t *arr)
{
	for(int i = 0; i < 4; i++)
	{
		if(arr[i] != 2*(i+1))
			return 0;
	}
	return 1;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
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
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
