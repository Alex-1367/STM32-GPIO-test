#include "stm32f1xx_hal.h"

void SystemClock_Config(void);

void SysTick_Handler(void) {
    HAL_IncTick();
}

int main(void) {
    HAL_Init();
    SystemClock_Config();

    // Enable clocks: GPIOA, GPIOC
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN;

    // Configure PC13 as output push-pull (LED)
    GPIOC->CRH &= ~(0x0F << 20);
    GPIOC->CRH |= (0x02 << 20);   // 0x2 = Output push-pull, 2MHz

    // Configure PA9 as ordinary push-pull output, 2MHz
    GPIOA->CRH &= ~(0x0F << 4);   // Clear PA9 config bits
    GPIOA->CRH |= (0x02 << 4);    // 0x2 = Output push-pull, 2MHz

    // Ensure PA9 starts low
    GPIOA->ODR &= ~GPIO_ODR_ODR9;

    while (1) {
        // Toggle LED (PC13)
        GPIOC->ODR ^= GPIO_ODR_ODR13;

        // Toggle PA9 as a plain GPIO
        GPIOA->ODR ^= GPIO_ODR_ODR9;

        HAL_Delay(500);
    }
}

void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
}