#include "rtthread.h"
#include "rtdevice.h"
#include "board.h"

TIM_HandleTypeDef beep_pwm_handle;

void beep_pwm_start(void)
{
    HAL_TIM_PWM_Start_IT(&beep_pwm_handle, TIM_CHANNEL_1);
}

void beep_pwm_stop(void)
{
    HAL_TIM_PWM_Stop_IT(&beep_pwm_handle, TIM_CHANNEL_1);
}

void beep_pwm_hw_init(void)
{
    TIM_OC_InitTypeDef sConfigOC = {0};
    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

    /* USER CODE BEGIN TIM16_Init 1 */

    /* USER CODE END TIM16_Init 1 */
    beep_pwm_handle.Instance = TIM16;
    beep_pwm_handle.Init.Prescaler = 64-1;
    beep_pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    beep_pwm_handle.Init.Period = 370;
    beep_pwm_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    beep_pwm_handle.Init.RepetitionCounter = 0;
    beep_pwm_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&beep_pwm_handle) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_PWM_Init(&beep_pwm_handle) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 50;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    if (HAL_TIM_PWM_ConfigChannel(&beep_pwm_handle, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
    sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
    sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
    sBreakDeadTimeConfig.DeadTime = 0;
    sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
    sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
    sBreakDeadTimeConfig.BreakFilter = 0;
    sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
    if (HAL_TIMEx_ConfigBreakDeadTime(&beep_pwm_handle, &sBreakDeadTimeConfig) != HAL_OK)
    {
        Error_Handler();
    }

    HAL_TIM_MspPostInit(&beep_pwm_handle);
    HAL_TIM_Base_Start_IT(&beep_pwm_handle);
}

void TIM16_IRQHandler(void)
{
  /* USER CODE BEGIN TIM16_IRQn 0 */

  /* USER CODE END TIM16_IRQn 0 */
  HAL_TIM_IRQHandler(&beep_pwm_handle);
  /* USER CODE BEGIN TIM16_IRQn 1 */

  /* USER CODE END TIM16_IRQn 1 */
}
