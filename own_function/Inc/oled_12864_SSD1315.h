//
// Created by CafuuChino on 2022/9/16.
//

#ifndef SC_TIMER_OLED_H
#define SC_TIMER_OLED_H
#include "main.h"



void OLED_Init(void);
void OLED_Clear(void);
uint8_t OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr, uint8_t mode);
uint8_t OLED_ShowString(uint8_t x,uint8_t y,char *chr, uint8_t mode, uint8_t align);
uint8_t OLED_ShowArray(uint8_t x, uint8_t y, char *str, uint8_t chr_num, uint8_t mode, uint8_t align);
uint8_t OLED_ShowU16(uint8_t x, uint8_t row, uint16_t num, uint8_t width, uint8_t reverse, uint8_t mode);

void OLED_Set_Pos(uint8_t x, uint8_t y);
void OLED_WR_DATA(uint8_t data);
void OLED_WR_CMD(uint8_t cmd);
void OLED_WR_DATA_ARRAY(uint8_t* pData, uint16_t data_len);
void OLED_RD_DATA_ARRAY(uint8_t* pData, uint16_t data_len);
#endif //SC_TIMER_OLED_H
