// mock_stm32f4xx_hal.h
#ifndef MOCK_STM32F4XX_HAL_H
#define MOCK_STM32F4XX_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Basic HAL types
typedef enum {
    HAL_OK       = 0x00U,
    HAL_ERROR    = 0x01U,
    HAL_BUSY     = 0x02U,
    HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

// ADC structures
typedef struct {
    uint32_t Channel;
    uint32_t Rank;
    uint32_t SamplingTime;
    uint32_t Offset;
} ADC_ChannelConfTypeDef;

typedef struct {
    void* Instance;
    void* Init;
    HAL_StatusTypeDef State;
    void* DMA_Handle;
    uint32_t ErrorCode;
} ADC_HandleTypeDef;

// UART structures
typedef struct {
    uint32_t BaudRate;
    uint32_t WordLength;
    uint32_t StopBits;
    uint32_t Parity;
    uint32_t Mode;
    uint32_t HwFlowCtl;
    uint32_t OverSampling;
} UART_InitTypeDef;

typedef struct {
    void* Instance;
    UART_InitTypeDef Init;
    uint8_t* pTxData;
    uint16_t TxXferSize;
    uint16_t TxXferCount;
    uint8_t* pRxData;
    uint16_t RxXferSize;
    uint16_t RxXferCount;
} UART_HandleTypeDef;

// GPIO structures
typedef struct {
    uint32_t Pin;
    uint32_t Mode;
    uint32_t Pull;
    uint32_t Speed;
    uint32_t Alternate;
} GPIO_InitTypeDef;

// Function declarations for ADC
HAL_StatusTypeDef HAL_ADC_Init(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef HAL_ADC_ConfigChannel(ADC_HandleTypeDef* hadc, ADC_ChannelConfTypeDef* sConfig);
HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef HAL_ADC_PollForConversion(ADC_HandleTypeDef* hadc, uint32_t Timeout);
uint32_t HAL_ADC_GetValue(ADC_HandleTypeDef* hadc);
HAL_StatusTypeDef HAL_ADC_Stop(ADC_HandleTypeDef* hadc);

// Function declarations for UART
HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef* huart);
HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef* huart, uint8_t* pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef* huart, uint8_t* pData, uint16_t Size, uint32_t Timeout);

// Function declarations for GPIO
HAL_StatusTypeDef HAL_GPIO_Init(void* GPIOx, GPIO_InitTypeDef* GPIO_Init);
void HAL_GPIO_WritePin(void* GPIOx, uint16_t GPIO_Pin, uint8_t PinState);
uint8_t HAL_GPIO_ReadPin(void* GPIOx, uint16_t GPIO_Pin);

// Clock related declarations
HAL_StatusTypeDef HAL_RCC_ClockConfig(void* RCC_ClkInitStruct, uint32_t FLatency);
HAL_StatusTypeDef HAL_RCC_OscConfig(void* RCC_OscInitStruct);

#ifdef __cplusplus
}
#endif

#endif // MOCK_STM32F4XX_HAL_H