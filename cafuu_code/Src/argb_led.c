//
// Created by CafuuChino on 24-11-19.
//

#include "argb_led.h"
#include "argb_common.h"
#include "argb_platform.h"
#include <memory.h>
#include <malloc.h>



#define WS2812_RST_TICK 280
#define WS2812_H 116
#define WS2812_L 57

ARGB_Typedef *g_hARGB=NULL;

ARGB_LED_Info g_ARGB_LED_List[ARGB_LED_TYPE_NUM] = {
        {ARGB_LED_WS2812, 3, 0, 0, 1200,ARGB_WS2812_BuildTxBuf},
        {ARGB_LED_SK6812_4C, 4, 0, 0, 1200,NULL},
        {ARGB_LED_SM16823E, 3, 2, 0, 1200,ARGB_SM16823_BuildTxBuf},
        {ARGB_LED_TM1814, 4, 0, 0, 1200,NULL},
};


ARGB_LED_Info *ARGB_Get_LED_Info(uint8_t led_type){
    for (uint8_t i = 0; i < ARGB_LED_TYPE_NUM; i++){
        if (g_ARGB_LED_List[i].led_type == led_type){
            return g_ARGB_LED_List + i;
        }
    }
    argb_perr("Cannot find led info, type %d", led_type);
    return NULL;
}

uint8_t ARGB_WS2812_BuildTxBuf(ARGB_Typedef* hARGB){
    if (hARGB->update_num == 0) return 1;
    uint8_t color_num = hARGB->p_led_info->color_num;
    uint16_t color_data_len = (hARGB->update_num) * color_num;
    uint16_t send_len = 8 * color_data_len + 1;
    hARGB->tx_buffer_len = send_len;
    for (uint16_t i = 0; i < hARGB->update_num; i++){
        uint8_t mask = 0x80;
        for (uint8_t j = 0; j < 8; j++){
            hARGB->pc_tx_buf[24*i + 16 + j] = (hARGB->pc_color[i*3 + 2] & mask) ? WS2812_H : WS2812_L; // B
            hARGB->pc_tx_buf[24*i +  8 + j] = (hARGB->pc_color[i*3    ] & mask) ? WS2812_H : WS2812_L; // R
            hARGB->pc_tx_buf[24*i +      j] = (hARGB->pc_color[i*3 + 1] & mask) ? WS2812_H : WS2812_L; // G
            mask >>= 1;
        }
    }
    hARGB->pc_tx_buf[hARGB->tx_buffer_len - 1] = 0;
    hARGB->update_num = 0;
    return 0;
}

#define SM16823_RST_TICK 280
#define SM16823_PERIOD 336
#define SM16823_H (uint16_t)(TIM_CLK_US_PRESCALER * 0.67)
#define SM16823_L (uint16_t)(TIM_CLK_US_PRESCALER * 0.33)

uint8_t ARGB_SM16823_BuildTxBuf(ARGB_Typedef* hARGB){
    if (hARGB->update_num == 0) return 1;
    uint8_t color_num = hARGB->p_led_info->color_num;
    uint8_t current_byte = hARGB->p_led_info->current_bytes;
    //uint8_t ext_bit = hARGB->p_led_info->ext_bit;
    uint16_t color_data_len = hARGB->update_num * color_num;
//    uint16_t send_len = 8 * (color_data_len + current_byte) + 2;
//    hARGB->tx_buffer_len = send_len;
    hARGB->pc_tx_buf[0] = 0;
    for (uint16_t i = 0; i < hARGB->led_num; i++){
        uint8_t mask = 0x80;
        for (uint8_t j = 0; j < 8; j++){
            hARGB->pc_tx_buf[1 + 24*i +      j] = (hARGB->pc_color[i*3    ] & mask) ? SM16823_H : SM16823_L; // R
            hARGB->pc_tx_buf[1 + 24*i +  8 + j] = (hARGB->pc_color[i*3 + 1] & mask) ? SM16823_H : SM16823_L; // G
            hARGB->pc_tx_buf[1 + 24*i + 16 + j] = (hARGB->pc_color[i*3 + 2] & mask) ? SM16823_H : SM16823_L; // B
            mask >>= 1;
        }
    }
    uint16_t cur_pos = 8 * color_num * hARGB->led_num + 1;
    uint16_t cur_byte = (uint16_t)((hARGB->pc_cur_gain[0] & 0xF) << 12)
            | (uint16_t)((hARGB->pc_cur_gain[1] & 0xF) << 8)
            | (uint16_t)((hARGB->pc_cur_gain[2] & 0xF) << 4);

    uint16_t mask = 0x8000;
    for (uint16_t i = 0; i < 16; i++){
            hARGB->pc_tx_buf[cur_pos + i] = (cur_byte & mask) ? SM16823_H : SM16823_L;
            mask >>= 1;
    }
    hARGB->pc_tx_buf[hARGB->tx_buffer_len - 1] = 0;
    hARGB->update_num = 0;
    return 0;
}