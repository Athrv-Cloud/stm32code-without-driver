#ifndef MOCK_STM32F1XX_H_
#define MOCK_STM32F1XX_H_

#include <stdint.h>

// Mock RCC structure
typedef struct {
    uint32_t APB2ENR;
} MockRCC_Type;

// Mock GPIO structure
typedef struct {
    uint32_t CRH;
} MockGPIO_Type;

// Declare mock objects
extern MockRCC_Type MockRCC;
extern MockGPIO_Type MockGPIOC;

// Map RCC and GPIOC to mock objects
#define RCC (&MockRCC)
#define GPIOC (&MockGPIOC)

// Define bit masks
#define RCC_APB2ENR_IOPCEN (1U << 4)
#define GPIO_CRH_MODE13    (3U << 20)
#define GPIO_CRH_MODE13_0  (1U << 20)
#define GPIO_CRH_MODE13_1  (1U << 21)
#define GPIO_CRH_CNF13     (3U << 22)
#define GPIO_CRH_CNF13_0   (1U << 22)
#define GPIO_CRH_CNF13_1   (1U << 23)

#endif /* MOCK_STM32F1XX_H_ */
