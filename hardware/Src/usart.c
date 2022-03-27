/**
 ******************************************************************************
 * @file    usart.c
 * @brief   This file provides code for the configuration
 *          of the USART instances.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include <string.h>
#include "vlog.h"
#include "message.h"

uint8_t rDataBuffer[256];
/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}

void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (uartHandle->Instance == USART1)
  {
    /* USER CODE BEGIN USART1_MspInit 0 */

    /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 2, 2);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    /* USER CODE BEGIN USART1_MspInit 1 */

    /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle)
{

  if (uartHandle->Instance == USART1)
  {
    /* USER CODE BEGIN USART1_MspDeInit 0 */

    /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
    /* USER CODE BEGIN USART1_MspDeInit 1 */

    /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

void readyRecv_IT(uint16_t size)
{
  while (HAL_UART_Receive_IT(&huart1, rDataBuffer, size) != HAL_OK)
    ;
}

extern Message_t message;
extern bool isGetNewFrame;
bool checkFrame(const MessageSerial_t *pMessageSerial)
{
  if ((uint8_t)(BeginFlag >> 8) != pMessageSerial->beginFlag[0])
  {
    Log(W, "%s::%d frame error l:%02X r:%02X", __func__, __LINE__, (uint8_t)(BeginFlag >> 8), pMessageSerial->beginFlag[0]);
    return false;
  }
  if ((uint8_t)(BeginFlag & 0XFF) != pMessageSerial->beginFlag[1])
  {
    Log(W, "%s::%d frame error", __func__, __LINE__);
    return false;
  }
  if ((uint8_t)(EndFlag >> 8) != pMessageSerial->endFlag[0])
  {
    Log(W, "%s::%d frame error", __func__, __LINE__);
    return false;
  }
  if ((uint8_t)(EndFlag & 0XFF) != pMessageSerial->endFlag[1])
  {
    Log(W, "%s::%d frame error", __func__, __LINE__);
    return false;
  }
  return true;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  // printf("HAL_UART_RxCpltCallback:rDataBuffer -> ");
  // for (size_t i = 0; i < 42; i++)
  // {
  //   printf("%02X ", rDataBuffer[i]);
  // }

  // print("\r\n");
  if (huart == &huart1)
  {
    MessageSerial_t *pMessageSerial = (MessageSerial_t *)(rDataBuffer);
    if (checkFrame(pMessageSerial))
    {
      // Log(I, "get new frame");
      isGetNewFrame = true;
      memcpy(&message, &(pMessageSerial->message), sizeof(Message_t));
    }
    else
    {
      // Log(W, "frame error");
    }
    while (HAL_UART_Receive_IT(&huart1, rDataBuffer, sizeof(MessageSerial_t)) != HAL_OK)
      ;
  }
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/