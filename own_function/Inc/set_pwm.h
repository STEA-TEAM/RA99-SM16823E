//
// Created by watermalen on 25-3-4.
//

#ifndef SET_PWM_H
#define SET_PWM_H

#endif //SET_PWM_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32g4xx_hal_tim.h"
#include <math.h>
#include "tim.h"
/* typedef ------------------------------------------------------------------*/
#define get_length(a) (uint32_t)(sizeof(a)/sizeof(a[0]))
extern double_t step_value[];/*max=1*/
extern uint32_t step_length;
extern double_t duty;
/*function ------------------------------------------------------------------*/
double_t pwm_step_duty(double_t const step_duty ,uint32_t const direction, double_t const duty_before);
void CCR_pwm_set_duty(TIM_HandleTypeDef const *htim,uint32_t const Channel,double_t const duty);
