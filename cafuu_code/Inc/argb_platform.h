//
// Created by CafuuChino on 24-11-18.
//

#ifndef H7B0DEBUGER_ARGB_PLATFORM_H
#define H7B0DEBUGER_ARGB_PLATFORM_H

#include "argb_common.h"


//#include "stm32h7xx_hal.h"
#include "stm32g4xx_hal.h"
#include "tim.h"

#define TIM_CLK_US_PRESCALER 120

typedef struct{
    TIM_HandleTypeDef *htim;
    uint32_t tim_channel;
}PHY_TIM;

void ARGB_TIM_SET_PRES(PHY_TIM *tim, uint16_t pres);
void ARGB_TIM_SET_ARR(PHY_TIM *tim, uint16_t arr);

uint8_t ARGB_TIM_PWM_DMA(ARGB_Typedef *hARGB);

#endif //H7B0DEBUGER_ARGB_PLATFORM_H
