#include "vcom.h"
#include "vcom_interface.h"
#include "stm32f1xx_hal.h"
#include "main.h"


uint8_t vcom_interface_timer_init(void)
{
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};

    /* USER CODE BEGIN TIM21_Init 1 */

    /* USER CODE END TIM21_Init 1 */
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 1875;
    htim3.Init.CounterMode = TIM_COUNTERMODE_DOWN;
    htim3.Init.Period = 1;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
    {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }

    return 0;
}

uint8_t vcom_interface_timer_deint(void)
{
    if (HAL_TIM_Base_DeInit(&htim3) == HAL_OK) {
        return 1;
    } else {
        return 0;
    }
}

uint8_t vcom_interface_timer_start(void)
{
    HAL_TIM_Base_Start_IT(&htim3);
    return 0;
}

uint8_t vcom_interface_timer_stop(void)
{
    HAL_TIM_Base_Stop_IT(&htim3);
    return 0;
}

uint8_t vcom_interface_tx_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(VCOM_TX_GPIO_Port, VCOM_TX_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin : VCOM_TX_Pin */
    GPIO_InitStruct.Pin = VCOM_TX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(VCOM_TX_GPIO_Port, &GPIO_InitStruct);

    return 0;
}


uint8_t vcom_interface_tx_gpio_deinit(void)
{
    HAL_GPIO_DeInit(VCOM_TX_GPIO_Port, VCOM_TX_Pin);
    return 0;
}


uint8_t vcom_interface_tx_gpio_wirte(uint8_t level)
{
    if (level == 0) {
        HAL_GPIO_WritePin(VCOM_TX_GPIO_Port, VCOM_TX_Pin, GPIO_PIN_RESET);
    } else {
        HAL_GPIO_WritePin(VCOM_TX_GPIO_Port, VCOM_TX_Pin, GPIO_PIN_SET);
    }
    return 0;
}
