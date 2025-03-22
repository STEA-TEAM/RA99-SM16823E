//
// Created by CafuuChino on 24-12-6.
//

#include "argb_phy.h"
#include "argb_led.h"
#include "argb_platform.h"
#include "argb_common.h"

#include <malloc.h>


ARGB_PHY_Info g_ARGB_PHY_List[ARGB_PHY_TYPE_NUM] = {
        {ARGB_PHY_TIM_DMA, ARGB_TIM_HwInit, ARGB_TIM_DMA_SEND},
};

struct list_head g_ARGB_PHY_Parents; // manage phy parents


static ARGB_PHY_Info *get_phy_info(uint8_t phy_type){
    for (uint8_t i = 0; i < ARGB_LED_TYPE_NUM; i++){
        if (g_ARGB_PHY_List[i].phy_type == phy_type){
            return g_ARGB_PHY_List + i;
        }
    }
    return NULL;
}

static void add_phy_parent(void* phy_handle, ARGB_Typedef *hARGB){
    ARGB_PHY_PARENT *node = (ARGB_PHY_PARENT*)malloc(sizeof(ARGB_PHY_PARENT));
    node->phy_handle = phy_handle;
    node->hARGB = hARGB;
    list_add(&node->list, &g_ARGB_PHY_Parents);
}

static ARGB_Typedef* get_phy_parent(void *phy_handle){
    struct list_head *p;
    ARGB_PHY_PARENT *parent;
    list_for_each(p, &g_ARGB_PHY_Parents){
        parent = list_entry(p, ARGB_PHY_PARENT, list);
        if (parent->phy_handle == phy_handle){
            return parent->hARGB;
        }
    }
    return NULL;
}

void ARGB_Phy_Init(){
    INIT_LIST_HEAD(&g_ARGB_PHY_Parents);
}

uint8_t ARGB_Attach_Phy(ARGB_Typedef *hARGB, uint8_t phy_type, void* hphy){
    ARGB_PHY_Info *phy_info = get_phy_info(phy_type);
    if (phy_info == NULL){
        argb_perr("Get Phy Info NULL");
    }
    hARGB->p_phy_info = phy_info;
    hARGB->phy_handle = hphy;
    add_phy_parent(hphy, hARGB);
    return 0;
}

uint8_t ARGB_TIM_HwInit(ARGB_Typedef *hARGB){
    ARGB_TIM_SET_PRES((PHY_TIM*)hARGB->phy_handle, 0);
    uint16_t arr = (hARGB->p_led_info->period_ns) * (uint32_t)(TIM_CLK_US_PRESCALER-1) / 1000;
    ARGB_TIM_SET_ARR((PHY_TIM*)hARGB->phy_handle, arr-1);
    return 0;
}

uint8_t ARGB_TIM_DMA_SEND(ARGB_Typedef *hARGB){
    uint8_t ret = ARGB_TIM_PWM_DMA(hARGB);
    hARGB->tx_busy = 1;
    return ret;
}