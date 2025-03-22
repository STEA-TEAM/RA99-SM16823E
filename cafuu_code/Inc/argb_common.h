//
// Created by CafuuChino on 24-11-18.
//

#ifndef H7B0DEBUGER_ARGB_COMMON_H
#define H7B0DEBUGER_ARGB_COMMON_H

#include <stdint.h>


#define DBG_ENABLE 1
#if DBG_ENABLE
#include <stdio.h>
#define argb_pdbg(...) printf("[ARGB][DBG]: ");printf(__VA_ARGS__)
#define argb_pwarn(...) printf("[ARGB][WARN]: ");printf(__VA_ARGS__)
#define argb_perr(...) printf("[ARGB][ERR]: ");printf(__VA_ARGS__)
#else
#define argb_pdbg(...) (void)
#define argb_pwarn(...) (void)
#define argb_perr(...) (void)
#endif


typedef struct ARGB_Handle_StructDef ARGB_Typedef;

typedef struct{
    uint8_t phy_type;
    uint8_t (*hw_init)(ARGB_Typedef *);
    uint8_t (*tx)(ARGB_Typedef *);
}ARGB_PHY_Info;

typedef struct{
    uint8_t led_type;
    uint8_t color_num;
    uint8_t current_bytes;
    uint8_t ext_bit;
    uint16_t period_ns;
    uint8_t (*build_tx)(ARGB_Typedef*);
}ARGB_LED_Info;

struct ARGB_Handle_StructDef{
    uint8_t led_type;
    uint8_t phy_type;
    uint8_t tx_busy;
    uint16_t led_num;
    uint16_t update_num;
    uint16_t tx_buffer_len;
    uint8_t *pc_color;
    uint8_t *pc_cur_gain;
    uint16_t *pc_tx_buf;
    ARGB_LED_Info *p_led_info;
    ARGB_PHY_Info *p_phy_info;
    void* phy_handle;
};




void argb_test();
void argb_test_loop();

void ARGB_Init();

ARGB_Typedef* ARGB_New(uint8_t led_type, uint8_t phy_type, uint16_t led_num, void* phy_handle);
uint8_t ARGB_Update(ARGB_Typedef* hARGB);

uint8_t ARGB_Reset_Color(ARGB_Typedef *hARGB);
uint8_t ARGB_Set_Color(ARGB_Typedef *hARGB, uint16_t addr, uint8_t *color);
uint8_t ARGB_Set_SingleColor(ARGB_Typedef *hARGB, uint16_t addr, uint8_t channel, uint8_t color);
uint8_t ARGB_Set_RangeColor(ARGB_Typedef *hARGB, uint16_t addr_start, uint16_t num, uint8_t *color);
uint8_t ARGB_Set_CurGain(ARGB_Typedef *hARGB, uint8_t channel, uint8_t cur_gain);




uint8_t ARGB_TIM_DMA_SEND(ARGB_Typedef*);
void HSL2RGB(float H, float S, float L, uint8_t *R, uint8_t *G, uint8_t *B);

#endif //H7B0DEBUGER_ARGB_COMMON_H
