//
// Created by CafuuChino on 24-11-19.
//

#ifndef H7B0DEBUGER_ARGB_WS2812_H
#define H7B0DEBUGER_ARGB_WS2812_H

#include "argb_common.h"

#define ARGB_LED_TYPE_NUM 4

#define ARGB_LED_WS2812 0
#define ARGB_LED_SK6812_4C 1
#define ARGB_LED_SM16823E 2
#define ARGB_LED_TM1814 3

extern ARGB_Typedef *g_hARGB;

ARGB_LED_Info *ARGB_Get_LED_Info(uint8_t led_type);

uint8_t ARGB_WS2812_BuildTxBuf(ARGB_Typedef*);
uint8_t ARGB_SM16823_BuildTxBuf(ARGB_Typedef*);
uint8_t ARGB_TM1814_BuildTxBuf(ARGB_Typedef*);


#endif //H7B0DEBUGER_ARGB_WS2812_H
