//
// Created by CafuuChino on 24-11-18.
//

#include "argb_platform.h"
#include "argb_common.h"



void ARGB_TIM_SET_PRES(PHY_TIM *tim, uint16_t pres){
    __HAL_TIM_SET_PRESCALER(tim->htim, pres);
}

void ARGB_TIM_SET_ARR(PHY_TIM *tim, uint16_t arr){
    __HAL_TIM_SET_AUTORELOAD(tim->htim, arr);
}

uint8_t ARGB_TIM_PWM_DMA(ARGB_Typedef *hARGB){
    TIM_HandleTypeDef *htim = ((PHY_TIM*)hARGB->phy_handle)->htim;
    uint32_t tim_channel = ((PHY_TIM*)hARGB->phy_handle)->tim_channel;
    return HAL_TIM_PWM_Start_DMA(htim, tim_channel, (uint32_t*)hARGB->pc_tx_buf, hARGB->tx_buffer_len);
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){
    if (htim == &htim3){
        //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_2);
        HAL_TIM_PWM_Stop_DMA(&htim3, TIM_CHANNEL_4);
//        uint32_t OC1M = htim17.Instance->CCMR1;
//        OC1M = OC1M & 0xF;
//        OC1M |= 0x00000040;
//        htim17.Instance->CCMR1 = OC1M;
    }
}


