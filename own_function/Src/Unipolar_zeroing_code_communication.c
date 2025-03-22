//
// Created by watermalen on 25-3-5.
//

#include "Unipolar_zeroing_code_communication.h"

#include "tim.h"

//UZCC_data1 init
/*UZCC_uint8_handel *UZCC_data1;

void UZCC_data1_init() {
  UZCC_data1->htim = &htim1;
  UZCC_data1->Channel = TIM_CHANNEL_3;
  UZCC_data1->tim_clk_KHZ = 280 * 1000;
  UZCC_data1->target_clk_KHZ = 10000 / 12;
  UZCC_data1->duty_0H = 0.25;
  UZCC_TIM_init(UZCC_data1);
  UZCC_CCR_init(UZCC_data1);
}*/

//end

uint8_t UZCC_TIM_init(UZCC_uint8_handel *g_handel) {
  if (g_handel->tim_clk_KHZ <= g_handel->target_clk_KHZ) {
    return 1;
  }
  __HAL_TIM_SET_PRESCALER(g_handel->htim, 0);
  __HAL_TIM_SET_AUTORELOAD(g_handel->htim, (uint32_t) (g_handel->tim_clk_KHZ / g_handel->target_clk_KHZ));
  __HAL_TIM_SET_COMPARE(g_handel->htim, g_handel->Channel, 0);
  __HAL_TIM_SET_COUNTER(g_handel->htim, 0);
  return 0;
}

void UZCC_CCR_init(UZCC_uint8_handel *g_handel) {
  uint32_t ARR;
  ARR = (uint32_t) (g_handel->tim_clk_KHZ / g_handel->target_clk_KHZ);
  g_handel->CCR_data.CCR_0 = (uint32_t) (ARR * g_handel->duty_0H);
  g_handel->CCR_data.CCR_1 = ARR - g_handel->CCR_data.CCR_0;
}

uint8_t UZCC_uint8_data_build(UZCC_uint8_handel *g_handel, uint8_t *data, uint32_t data_length, uint8_t *ex_data,
                              uint32_t ex_data_length) {
  if (data_length) {
    g_handel->data_buffer = data;
    g_handel->data_num = data_length;
    g_handel->ex_data_buffer = ex_data;
    g_handel->ex_data_num = ex_data_length;
    g_handel->update_num = (data_length + ex_data_length) * 8;
    g_handel->ex_update_num = (ex_data_length + ex_data_length) * 8;
    return 0;
  }
  return 1;
}

uint8_t UZCC_uint8_data_len_build(UZCC_uint8_handel *g_handel, uint32_t data_length, uint32_t ex_data_length) {
  if (data_length) {
    g_handel->data_buffer = (uint8_t *) malloc(sizeof(uint8_t) * data_length);
    memset(g_handel->data_buffer, 0, data_length);
    g_handel->data_num = data_length;
    g_handel->ex_data_buffer = (uint8_t *) malloc(sizeof(uint8_t) * ex_data_length);
    memset(g_handel->ex_data_buffer, 0, ex_data_length);
    g_handel->ex_data_num = ex_data_length;
    g_handel->tx_length = (data_length + ex_data_length) * 8 + 1;
    g_handel->tx_buffer = (uint16_t *) malloc(sizeof(uint16_t) * g_handel->tx_length);
    g_handel->update_num = 0;
    g_handel->ex_update_num = 0;
    return 0;
  }
  return 1;
}

uint8_t UZCC_uint8_set_data(UZCC_uint8_handel *g_handel, uint32_t data_address, uint8_t *data, uint32_t data_length) {
  if ((data_address > g_handel->data_num) || (data_address + data_length - 1 > g_handel->data_num)) {
    return 1;
  }
  for (uint32_t i = 0; i < data_length; i++) {
    *(g_handel->data_buffer + data_address - 1 + i) = *(data + i);
  }
  g_handel->update_num = (data_address - 1 + data_length) * 8;
  return 0;
}

uint8_t UZCC_uint8_set_ex_data(UZCC_uint8_handel *g_handel, uint32_t data_address, uint8_t *ex_data,
                               uint32_t ex_data_length) {
  if (data_address > g_handel->ex_data_num || data_address + ex_data_length - 1 > g_handel->ex_data_num) {
    return 1;
  }
  for (uint32_t i = 0; i < ex_data_length; i++) {
    *(g_handel->ex_data_buffer + data_address - 1 + i) = *(ex_data + i);
  }
  g_handel->ex_update_num = (data_address - 1 + ex_data_length) * 8;
  return 0;
}

void UZCC_uint8_tx_build(UZCC_uint8_handel *g_handel) {
  uint8_t mask, j;
  uint32_t i;
  for (i = 0; i < (g_handel->update_num / 8); i++) {
    mask = 0b10000000;
    for (j = 0; j < 8; j++) {
      *(g_handel->tx_buffer + i * 8 + j) = ((*(g_handel->data_buffer + i) & mask) != 0)
                                             ? g_handel->CCR_data.CCR_1
                                             : g_handel->CCR_data.CCR_0;
      mask >>= 1;
    }
  }
  for (i = 0; i < (g_handel->ex_update_num / 8); i++) {
    mask = 0b10000000;
    for (j = 0; j < 8; j++) {
      *(g_handel->tx_buffer + g_handel->update_num + i * 8 + j) = ((*(g_handel->ex_data_buffer + i) & mask) != 0)
                                                                    ? g_handel->CCR_data.CCR_1
                                                                    : g_handel->CCR_data.CCR_0;
      mask >>= 1;
    }
  }
  *(g_handel->tx_buffer + g_handel->update_num + g_handel->ex_update_num) = 0;
}

uint8_t UZCC_uint8_all_zeros(UZCC_uint8_handel *g_handel) {
  if (g_handel->data_num == 0||g_handel->tx_length == 0) {
    return 1;
  }
  memset(g_handel->data_buffer, 0, g_handel->data_num);
  UZCC_uint8_tx_build(g_handel);
  return 0;
}

uint8_t UZCC_uint8_DMA_send(UZCC_uint8_handel *g_handel) {
  if (g_handel->update_num || g_handel->ex_update_num) {
    HAL_TIM_PWM_Start_DMA(g_handel->htim, g_handel->Channel, (uint32_t *) g_handel->tx_buffer,
                          (g_handel->update_num + g_handel->ex_update_num + 1));
    //g_handel->update_num=0;
    //g_handel->ex_update_num=0;
    return 0;
  }
  return 1;
}

