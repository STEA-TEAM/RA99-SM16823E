//
// Created by CafuuChino on 24-12-6.
//

#ifndef MOTOR_4CH_ARGB_PHY_H
#define MOTOR_4CH_ARGB_PHY_H

#include "argb_common.h"
#include "chain_list.h"

#define ARGB_PHY_TYPE_NUM 2
#define ARGB_PHY_TIM_DMA 0
#define ARGB_PHY_SPI_DMA 1

typedef struct{
    void* phy_handle;
    ARGB_Typedef *hARGB;
    struct list_head list;
}ARGB_PHY_PARENT;

void ARGB_Phy_Init();

uint8_t ARGB_Attach_Phy(ARGB_Typedef *hARGB, uint8_t phy_type, void* hphy);


uint8_t ARGB_TIM_HwInit(ARGB_Typedef *hARGB);
uint8_t ARGB_TIM_DMA_SEND(ARGB_Typedef *hARGB);

#endif //MOTOR_4CH_ARGB_PHY_H
