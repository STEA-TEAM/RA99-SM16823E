//
// Created by watermalen on 25-3-5.
//

#ifndef UNIPOLAR_ZEROING_CODE_COMMUNICATION_H
#define UNIPOLAR_ZEROING_CODE_COMMUNICATION_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
/* typedef ------------------------------------------------------------------*/
typedef struct {
  uint32_t CCR_0;
  uint32_t CCR_1;
} UZCC_CCR;

typedef struct {
  TIM_HandleTypeDef *htim;
  uint32_t Channel;
  uint32_t tim_clk_KHZ;
  double_t target_clk_KHZ;
  double_t duty_0H;
  UZCC_CCR CCR_data;
  uint32_t data_num;
  uint8_t *data_buffer;
  uint32_t update_num;
  uint32_t ex_data_num;
  uint8_t *ex_data_buffer;
  uint32_t ex_update_num;
  uint32_t tx_length;
  uint16_t *tx_buffer;
}UZCC_uint8_handel;

//extern UZCC_uint8_handel *UZCC_data1;
/* function ------------------------------------------------------------------*/
//void UZCC_data1_init();
uint8_t UZCC_TIM_init(UZCC_uint8_handel *g_handel);
void UZCC_CCR_init(UZCC_uint8_handel *g_handel);
uint8_t UZCC_uint8_data_build(UZCC_uint8_handel *g_handel, uint8_t *data, uint32_t data_length, uint8_t *ex_data,
                              uint32_t ex_data_length);
uint8_t UZCC_uint8_data_len_build(UZCC_uint8_handel *g_handel, uint32_t data_length,uint32_t ex_data_length);
uint8_t UZCC_uint8_set_data(UZCC_uint8_handel *g_handel, uint32_t data_address, uint8_t *data, uint32_t data_length);
uint8_t UZCC_uint8_set_ex_data(UZCC_uint8_handel *g_handel, uint32_t data_address, uint8_t *ex_data, uint32_t ex_data_length);
void UZCC_uint8_tx_build(UZCC_uint8_handel *g_handel);
uint8_t UZCC_uint8_all_zeros(UZCC_uint8_handel *g_handel);
uint8_t UZCC_uint8_DMA_send(UZCC_uint8_handel *g_handel);

#endif //UNIPOLAR_ZEROING_CODE_COMMUNICATION_H
