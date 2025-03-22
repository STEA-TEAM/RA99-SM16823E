//
// Created by CafuuChino on 24-11-18.
//

#include "argb_common.h"
#include "argb_platform.h"
#include "argb_led.h"
#include "argb_phy.h"
#include <malloc.h>
#include <memory.h>

#include "tim.h"

void argb_test(){
    ARGB_Phy_Init();
    PHY_TIM *g_hARGB_PHY = (PHY_TIM*)malloc(sizeof(PHY_TIM));
    g_hARGB_PHY->htim = &htim3;
    g_hARGB_PHY->tim_channel = TIM_CHANNEL_4;
    g_hARGB = ARGB_New(ARGB_LED_SM16823E, ARGB_PHY_TIM_DMA, 36, g_hARGB_PHY);
    /*if (g_hARGB == NULL){
        while(1){
            __NOP();
        }
    }*/
}

uint8_t addr = 0;

void argb_test_loop(){
    //return;
    uint32_t ret;
//    ret = HAL_TIM_PWM_Start_DMA(&htim17, TIM_CHANNEL_1, dma_buf, 8);
//    if (ret){
//        __NOP();
//    }0.140 0.076 0.099
    uint8_t color[3] = {15,0,15};
    ARGB_Reset_Color(g_hARGB);
    ARGB_Set_Color(g_hARGB, addr++, color);
    if (addr == 25) addr = 0;
    ret = ARGB_Update(g_hARGB);
    if (ret){
        __NOP();
    }
    HAL_Delay(10);
}


void ARGB_Init(){
    ARGB_Phy_Init();
}

ARGB_Typedef* ARGB_New(uint8_t led_type, uint8_t phy_type, uint16_t led_num, void* phy_handle){
    uint8_t ret = 0;

    ARGB_Typedef *new_ARGB = (ARGB_Typedef*)malloc(sizeof(ARGB_Typedef));
    new_ARGB->led_type = led_type;
    new_ARGB->phy_type = phy_type;

    new_ARGB->led_num = led_num;
    new_ARGB->p_led_info = ARGB_Get_LED_Info(led_type);
    if (new_ARGB->p_led_info == NULL){
        free (new_ARGB);
        return NULL;
    }

    ret = ARGB_Attach_Phy(new_ARGB, phy_type, phy_handle);
    if (ret){
        free (new_ARGB);
        return NULL;
    }
    new_ARGB->p_phy_info->hw_init(new_ARGB);

    new_ARGB->tx_busy = 0;
    uint16_t color_data_len = (new_ARGB->p_led_info->color_num * led_num);
    new_ARGB->pc_color = (uint8_t*)malloc(color_data_len);
    memset(new_ARGB->pc_color, 0, color_data_len);
    new_ARGB->pc_cur_gain = (uint8_t*)malloc(new_ARGB->p_led_info->color_num);
    memset(new_ARGB->pc_cur_gain, 0, new_ARGB->p_led_info->color_num);

    new_ARGB->update_num = 0;
    uint32_t send_len = 8 * (color_data_len + new_ARGB->p_led_info->current_bytes) + 2;
    new_ARGB->tx_buffer_len = send_len;
    new_ARGB->pc_tx_buf = (uint16_t*)malloc(2 * send_len); // if MCU has enough memory, to avoid frequent malloc, malloc max size in init;
    ARGB_Reset_Color(new_ARGB);
    ARGB_Update(new_ARGB);
    return new_ARGB;
}

uint8_t ARGB_Reset_Color(ARGB_Typedef *hARGB){
    uint16_t mem_len = hARGB->p_led_info->color_num * hARGB->led_num;
    memset(hARGB->pc_color, 0, mem_len);
    hARGB->update_num = hARGB->led_num;
    return 0;
}

uint8_t ARGB_Set_Color(ARGB_Typedef *hARGB, uint16_t addr, uint8_t *color){
    if (addr >= hARGB->led_num){
        return 1;
    }
    uint16_t start = addr * hARGB->p_led_info->color_num;
    if (addr + 1 > hARGB->update_num){
        hARGB->update_num = addr + 1;
    }
    memcpy(hARGB->pc_color+start, color, hARGB->p_led_info->color_num);
    return 0;
}

uint8_t ARGB_Set_SingleColor(ARGB_Typedef *hARGB, uint16_t addr, uint8_t channel, uint8_t color){
    if (addr >= hARGB->led_num){
        return 1;
    }
    uint16_t pos = addr * hARGB->p_led_info->color_num + channel;
    hARGB->pc_color[pos] = color;
    if (addr + 1 > hARGB->update_num){
        hARGB->update_num = addr + 1;
    }
    return 0;
}

uint8_t ARGB_Set_RangeColor(ARGB_Typedef *hARGB, uint16_t addr_start, uint16_t num, uint8_t *color){
    if (addr_start >= hARGB->led_num || addr_start + num > hARGB->led_num){
        return 1;
    }
    uint16_t start = addr_start * hARGB->p_led_info->color_num;
    for (uint16_t i = 0; i < num; i++){
        memcpy(hARGB->pc_color + start, color, hARGB->p_led_info->color_num);
        start += hARGB->p_led_info->color_num;
    }
    if (addr_start + num + 1 > hARGB->update_num){
        hARGB->update_num = addr_start + num + 1;
    }
    return 0;
}

uint8_t ARGB_Set_CurGain(ARGB_Typedef *hARGB, uint8_t channel, uint8_t cur_gain){
    if (channel >= hARGB->p_led_info->color_num){
        return 1;
    }
    hARGB->pc_cur_gain[channel] = cur_gain;
    return 0;
}

uint8_t ARGB_Update(ARGB_Typedef* hARGB) {
    uint8_t ret = 0;
    ret = hARGB->p_led_info->build_tx(hARGB);
    if (ret){
        return 1;
    }
    ret = hARGB->p_phy_info->tx(hARGB);
    if (ret){
        return 2;
    }
    return 0;

}













void HSL2RGB(float H, float S, float L, uint8_t *R, uint8_t *G, uint8_t *B){
    // H must be [0, 360]
    // S must be [0, 1]
    // L must be [0, 1]
    if(S == 0){
        *R = *G = *B = (uint8_t)(L * 255.0f);
    }
    else{
        float q = (L<0.5f)?(L * (1.0f+S)):(L+S - (L*S));
        float p = (2.0f * L) - q;
        float Hk = H/360.0f;
        float T[3];
        T[0] = Hk + 0.3333333f; // Tr 0.3333333f=1.0/3.0
        T[1] = Hk;        // Tb
        T[2] = Hk - 0.3333333f; // Tg
        for(int i=0; i<3; i++)
        {
            if(T[i] < 0) T[i] += 1.0f;
            if(T[i] > 1) T[i] -= 1.0f;
            if((T[i]*6) < 1){
                T[i] = p + ((q-p)*6.0f*T[i]);
            }
            else if((T[i]*2.0f) < 1){ //(1.0/6.0)<=T[i] && T[i]<0.5
                T[i] = q;
            }
            else if((T[i]*3.0f) < 2){ // 0.5<=T[i] && T[i]<(2.0/3.0)
                T[i] = p + (q-p) * ((2.0f/3.0f) - T[i]) * 6.0f;
            }
            else T[i] = p;
        }
        T[0] = T[0]*255.0f;
        T[1] = T[1]*255.0f;
        T[2] = T[2]*255.0f;
        *R = (uint8_t)((T[0]>255)? 255 : ((T[0]<0)?0 : T[0]));
        *G = (uint8_t)((T[1]>255)? 255 : ((T[1]<0)?0 : T[1]));
        *B = (uint8_t)((T[2]>255)? 255 : ((T[2]<0)?0 : T[2]));
    }
}