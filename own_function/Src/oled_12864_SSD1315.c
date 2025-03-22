//
// Created by CafuuChino on 2022/9/16.
//

#include <math.h>
#include "oled_12864_SSD1315.h"
#include "oled_12864_font.h"
#include <memory.h>
#include "i2c.h"

uint8_t CMD_Data[] = {
        0xAE, // [RESET 0xAE] Set display, 0xAE-OFF / 0xAF-ON
        0x00, // [RESET 0x00] Set lower column start Addr (Only for Page Addr Mode), Bit[3:0] is data
        0x10, // [RESET 0x10] Set higher column start Addr (Only for Page Addr Mode), Bit[2:0] is data
        0x40, // [RESET 0x40] Set display start line, Bit[5:0] is data
        //0xB0, // Set page start Addr (Only for Page Addr Mode), Bit[2:0] is data
        0x81, 0x7F, // [RESET 0x81, 0x7F] 2Byte CMD, Set contrast, Bit[7:0] is data
        0xA1, // [RESET 0xA0] Set segment remap, 0xA0-Col0 to SEG0 / 0xA1-Col127 to SEG0 (Vertical Mirror)
        //0xA6, // [RESET 0xA6] Set normal/inverse display, 0xA6-normal / 0xA7-inverse
        //0xA8, 0x3F, // 2Byte CMD, Set multiplex ratio, Bit[5:0] is data(set to N+1, 0-14 is invalid)
        0xC8, // [RESET 0xC0] Set COM output scan direction, C0-normal, from COM0 to COM[N-1] / C8-inverse(N is multiplex ratio)
        //0xD3, 0x00, // [RESET 0xD3, 0x00] 2Byte CMD, Set display offset, Bit[5:0] is data (Vertical Shift)
        //0xD5, 0x80, // [RESET 0xD5, 0x80] 2Byte CMD, Set display clock, Bit[3:0] is display clock divide ratio, Bit[7:4] is oscillator freq
        //0xD8, 0x05, //  Invalid in SSD1315
        0xD9, 0xF1, // [RESET 0xD9, 0x22] 2Byte CMD, Set pre-charge period, Bit[3:0] is Phase1 period, Bit[7:4] is Phase2 period(i.e. 2,4,6,...,30 0 is invalid)
        0xDA, 0b00010010, // 2Byte CMD, Set COM pins hardware configure
                                    // Bit[4] 0b0-Sequential COM pin configuration / 0b1-Alternative COM pin configuration
        0xDB, 0b00100000, // [RESET 0xD9, 0x20] 2Byte CMD, Set COM high level voltage, Bit[5:4] is data
                                    // 0b00 0.65 * Vcc
                                    // 0b01 0.71 * Vcc
                                    // 0b10 0.77 * Vcc
                                    // 0b11 0.83 * Vcc
        0x8D, 0b00010100, // [RESET 0x8D, 0x10] 2Byte CMD, Charge pump setting. Bit[7:0] is data
                                    // Bit[2] 0b0-disabled charge pump[RESET] / 0b1-enabled charge pump
                                    // Bit[7] Bit[0]: Charge pump mode: 0b00-7.5V[RESET] / 0b10-8.5V / 0b11 9.0V
        0xAF // [RESET 0xAE] Set display, 0xAE-OFF / 0xAF-ON
};




static void WriteCmd(void);

uint8_t OLED_ShowU16(uint8_t x, uint8_t row, uint16_t num, uint8_t width, uint8_t reverse, uint8_t mode){
    uint8_t x_pos = x;
    uint8_t digit;
    uint16_t origin_digit = num;
    uint8_t begin_ava = 0;
    for (uint8_t i = width; i > 0; i--){
        digit = origin_digit / (uint16_t)pow(10, i-1);

        origin_digit -= digit*(uint16_t)pow(10, i-1);
        // ava 0
        if (begin_ava){
            x_pos = OLED_ShowChar(x_pos, row, '0' + digit, (reverse == i || reverse == 0xFF));
        }
        else{
            if (digit){
                begin_ava = 1;
                x_pos = OLED_ShowChar(x_pos, row, '0' + digit, (reverse == i || reverse == 0xFF));
            }
            else{
                if (!mode && i>1){
                    x_pos = OLED_ShowChar(x_pos, row, ' ', (reverse == i || reverse == 0xFF));
                }
                else{
                    x_pos = OLED_ShowChar(x_pos, row, '0', (reverse == i || reverse == 0xFF));
                }

            }
        }
    }
    return x_pos;
}


void WriteCmd() {
    HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, CMD_Data, sizeof(CMD_Data), 0xFF);
}

void OLED_WR_CMD(uint8_t cmd) {
    HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 0x01);
}

void OLED_WR_DATA(uint8_t data) {
    HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x40, I2C_MEMADD_SIZE_8BIT, &data, 1, 0x01);
}

void OLED_WR_DATA_ARRAY(uint8_t* pData, uint16_t data_len) {
    HAL_I2C_Mem_Write(&hi2c1, 0x78, 0x40, I2C_MEMADD_SIZE_8BIT, pData, data_len, 50);
}

void OLED_Init(void) {
    HAL_Delay(100);
    WriteCmd();
    OLED_Clear();
}

void OLED_Clear() {
    uint8_t i;
    uint8_t zero[128];
    memset(zero, 0, 128);
    for (i = 0; i < 8; i++) {
        OLED_WR_CMD(0xb0 + i);
        OLED_WR_CMD(0x00);
        OLED_WR_CMD(0x10);
        OLED_WR_DATA_ARRAY(zero, 128);
    }
}

void OLED_Set_Pos(uint8_t x, uint8_t y) {
    OLED_WR_CMD(0xb0 + y);
    OLED_WR_CMD(((x & 0xf0) >> 4) | 0x10);
    OLED_WR_CMD(x & 0x0f);
}

uint8_t OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t mode) {
    uint8_t c, i;
    c = chr - ' ';
    if (x > 127) {
        x = 0;
        y = y + 2;
    }
    OLED_Set_Pos(x, y);
    for (i = 0; i < 8; i++) {
        if (mode) {
            OLED_WR_DATA(~F8x16[c * 16 + i]);
        } else {
            OLED_WR_DATA(F8x16[c * 16 + i]);
        }
    }
    OLED_Set_Pos(x, y + 1);
    for (i = 0; i < 8; i++) {
        if (mode) {
            OLED_WR_DATA(~F8x16[c * 16 + i + 8]);
        } else {
            OLED_WR_DATA(F8x16[c * 16 + i + 8]);
        }
    }
    return x + 8;
}


uint8_t OLED_ShowArray(uint8_t x, uint8_t y, char *str, uint8_t chr_num, uint8_t mode, uint8_t align) {
    uint8_t disp_num = 0;
    uint8_t x_pos = x;
    uint8_t row = y;
    uint8_t col = 0;
    if (align == 1){
        uint16_t width = 8 * chr_num;
        while (width > 128){
            for (uint8_t i = 0; i < 16; i++){
                OLED_ShowChar(8*i, row, str[disp_num], mode);
                disp_num ++;
            }
            width -= 128;
            row += 2;
        }
        col = (128 - width) / 2;
        while(disp_num < chr_num){
            OLED_ShowChar(col, row, str[disp_num], mode);
            disp_num ++;
            col += 8;
        }
        return col;
    }
    else{
        while (disp_num < chr_num) {
            OLED_ShowChar(col+x_pos, row, str[disp_num], mode);
            disp_num ++;
            col += 8;
            if (col > 120) {
                col = 0;
                row += 2;
            }
        }
        return row;
    }
}

uint8_t OLED_ShowString(uint8_t x, uint8_t y, char *str, uint8_t mode, uint8_t align) {
    uint8_t chr_num = 0;
    while (str[chr_num] != '\0') {
        chr_num++;
    }
    OLED_ShowArray(x, y, str, chr_num, mode, align);
    return 0;
}
