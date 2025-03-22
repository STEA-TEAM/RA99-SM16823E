//
// Created by watermalen on 25-1-7.
//

#include "button_interrupt_pullup.h"
#include "main.h"


#define UP_EDGE 0u
#define DOWN_EDGE 1u


void key_judge_init(key_judge *data) {
  data->edge_state_now=UP_EDGE;
  data->edge_time=0;
}

void Get_key_judge(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, key_judge *data) {
  data->edge_time = HAL_GetTick();
  data->gpio_state_now = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
  data->edge_state_now =( (data->gpio_state_now != GPIO_PIN_RESET) ? UP_EDGE : DOWN_EDGE);
}

uint8_t Compare_key_judge(key_judge *now, key_judge *last, uint32_t time_interval) {
  if (now->gpio_state_now != GPIO_PIN_RESET && now->gpio_state_now != last->gpio_state_now) {
    if (now->edge_state_now == UP_EDGE && now->edge_state_now != last->edge_state_now) {
      if (now->edge_time - last->edge_time >= time_interval) {
        return 1;
      }
    }
  }
  return 0;
}

uint8_t Eliminate_dithering_judge(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, key_judge *last, uint32_t time_interval) {
  key_judge now;
  Get_key_judge(GPIOx, GPIO_Pin, &now);
  const uint8_t result = Compare_key_judge(&now, last, time_interval);
  last->edge_time = now.edge_time;
  last->gpio_state_now = now.gpio_state_now;
  last->edge_state_now = now.edge_state_now;
  return result;
}

