//
// Created by watermalen on 25-3-8.
//

#ifndef ARGB_TX_H
#define ARGB_TX_H
/*define---------------------------------------------------------------------*/
#define WS2812E 0
#define SM16823E 1
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Unipolar_zeroing_code_communication.h"
/* typedef ------------------------------------------------------------------*/

typedef struct {
  uint8_t argb_type;
  uint8_t per_data_len;
  uint8_t ex_len_uint8;
  uint8_t clear_flag;
  uint32_t argb_num;
  UZCC_uint8_handel *UZCC_data;
}argb_handle;

/*function-------------------------------------------------------------------*/
uint8_t argb_typedefine(argb_handle *argb_total, uint8_t type);
uint8_t argb_init(argb_handle *argb_total, uint8_t type, uint32_t number, TIM_HandleTypeDef *htim, uint32_t Channel,
                  uint32_t tim_clk_KHZ);
uint8_t argb_set_single_data(argb_handle *argb_total, uint32_t num_address, uint8_t *color);
uint8_t argb_set_all_data(argb_handle *argb_total,uint8_t *color);
uint8_t argb_set_ex_data(argb_handle *argb_total, uint8_t *ex_data);
uint8_t argb_clear_data(argb_handle *argb_total);
uint8_t argb_clear_ex_data(argb_handle *argb_total);
uint8_t argb_UZCC_uint8_send(argb_handle *argb_total);
/*others--------------------------------------------------------------------*/
extern argb_handle *argb1;


#endif //ARGB_TX_H
