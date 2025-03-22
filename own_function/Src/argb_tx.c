//
// Created by watermalen on 25-3-8.
//

#include "argb_tx.h"

uint8_t argb_typedefine(argb_handle *argb_total, uint8_t type) {
  argb_total->argb_type = type;
  argb_total->UZCC_data = malloc(sizeof(UZCC_uint8_handel));
  switch (type) {
    case WS2812E: argb_total->per_data_len = 3;
      argb_total->ex_len_uint8 = 0;
      argb_total->UZCC_data->ex_data_num = argb_total->ex_len_uint8;
      argb_total->UZCC_data->target_clk_KHZ = 800;
      argb_total->UZCC_data->duty_0H = 0.2;
      break;
    case SM16823E: argb_total->per_data_len = 3;
      argb_total->ex_len_uint8 = 2;
      argb_total->UZCC_data->ex_data_num = argb_total->ex_len_uint8;
      argb_total->UZCC_data->target_clk_KHZ = 10000 / 12;
      argb_total->UZCC_data->duty_0H = 0.25;
      break;
    default: return 1;
  }
  return 0;
}

uint8_t argb_init(argb_handle *argb_total, uint8_t type, uint32_t number, TIM_HandleTypeDef *htim, uint32_t Channel,
                  uint32_t tim_clk_KHZ) {
  if (argb_typedefine(argb_total, type)) {
    return 1;
  }
  argb_total->argb_num = number;
  argb_total->clear_flag = 0;
  argb_total->UZCC_data->htim = htim;
  argb_total->UZCC_data->Channel = Channel;
  argb_total->UZCC_data->tim_clk_KHZ = tim_clk_KHZ;
  if (UZCC_TIM_init(argb_total->UZCC_data)) {
    return 1;
  }
  UZCC_CCR_init(argb_total->UZCC_data);
  if (UZCC_uint8_data_len_build(argb_total->UZCC_data,
                                number * argb_total->per_data_len, argb_total->ex_len_uint8)) {
    return 1;
  }
  return 0;
}

uint8_t argb_set_single_data(argb_handle *argb_total, uint32_t num_address, uint8_t *color) {
  if (UZCC_uint8_set_data(argb_total->UZCC_data, (num_address - 1) * argb_total->per_data_len + 1, color,
                          argb_total->per_data_len)) {
    return 1;
  }
  return 0;
}

uint8_t argb_set_all_data(argb_handle *argb_total,uint8_t *color) {
  if (argb_total->UZCC_data->data_num == 0) {
    return 1;
  }
  for (uint32_t i = 0; i < argb_total->argb_num; i++) {
    argb_set_single_data(argb_total,i+1,color);
  }
  return 0;
}

uint8_t argb_set_ex_data(argb_handle *argb_total, uint8_t *ex_data) {
  if (UZCC_uint8_set_ex_data(argb_total->UZCC_data, 1, ex_data, argb_total->ex_len_uint8)) {
    return 1;
  }
  return 0;
}

uint8_t argb_clear_data(argb_handle *argb_total) {
  if (argb_total->argb_num == 0 || argb_total->UZCC_data->data_num == 0 || argb_total->UZCC_data->tx_length == 0) {
    return 1;
  }
  argb_total->clear_flag = 1;
  for (uint32_t i = 0; i < argb_total->UZCC_data->data_num; i++) {
    UZCC_uint8_set_data(argb_total->UZCC_data, i + 1, 0, 1);
  }
  return 0;
}

uint8_t argb_clear_ex_data(argb_handle *argb_total) {
  if (argb_total->UZCC_data->ex_data_num == 0 || argb_total->UZCC_data->tx_length == 0) {
    return 1;
  }
  argb_total->clear_flag = 1;
  for (uint32_t i = 0; i < argb_total->UZCC_data->ex_data_num; i++) {
    UZCC_uint8_set_ex_data(argb_total->UZCC_data, i + 1, 0, 1);
  }
  return 0;
}



uint8_t argb_UZCC_uint8_send(argb_handle *argb_total) {
  if (argb_total->clear_flag) {
    argb_total->UZCC_data->update_num = argb_total->UZCC_data->data_num * 8;
    argb_total->UZCC_data->ex_update_num = argb_total->UZCC_data->ex_data_num * 8;
    argb_total->clear_flag = 0;
  }
  UZCC_uint8_tx_build(argb_total->UZCC_data);
  if (UZCC_uint8_DMA_send(argb_total->UZCC_data)) {
    return 1;
  }
  return 0;
}
