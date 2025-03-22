//
// Created by watermalen on 25-1-7.
//

#ifndef BUTTON_INTERRUPT_PULLUP_H
#define BUTTON_INTERRUPT_PULLUP_H
#include <stdint.h>
#include "main.h"
#include "button_interrupt_pullup.h"
#include "stm32g4xx_hal.h"
typedef struct key_judge_ {
  uint32_t edge_time;
  uint8_t gpio_state_now;
  uint8_t edge_state_now;
} key_judge;

void key_judge_init(key_judge *data);
uint8_t Eliminate_dithering_judge(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, key_judge *last, uint32_t time_interval);
#endif //BUTTON_INTERRUPT_PULLUP_H
