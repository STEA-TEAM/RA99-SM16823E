//
// Created by watermalen on 25-3-4.
//

#include "set_pwm.h"


/*uint32_t get_percentage_step(TIM_HandleTypeDef const *htim,double_t const *set_step,uint8_t const length) {
  uint32_t const ARR=htim->Instance->ARR;
  double_t percentage_step[length];
  for(uint8_t i=0;i<length;i++) {
    percentage_step[i]=*(set_step+i)*ARR;
  }
  return (uint32_t) percentage_step[length];
}

void change_pwm_step_duty(TIM_HandleTypeDef const *htim,uint32_t Channel,uint32_t step,uint8_t direction) {
  if(direction==1) {
    switch(Channel) {
      case TIM_CHANNEL_1:htim->Instance->CCR1=(htim->Instance->CCR1+step)%htim->Instance->ARR; break;
      case TIM_CHANNEL_2:htim->Instance->CCR2=(htim->Instance->CCR1+step)%htim->Instance->ARR; break;
      case TIM_CHANNEL_3:htim->Instance->CCR3=(htim->Instance->CCR1+step)%htim->Instance->ARR; break;
      case TIM_CHANNEL_4:htim->Instance->CCR4=(htim->Instance->CCR1+step)%htim->Instance->ARR; break;
      case TIM_CHANNEL_5:htim->Instance->CCR5=(htim->Instance->CCR1+step)%htim->Instance->ARR; break;
      case TIM_CHANNEL_6:htim->Instance->CCR6=(htim->Instance->CCR1+step)%htim->Instance->ARR; break;
      default: break;
    }
  }else {
    switch(Channel) {
      case TIM_CHANNEL_1:htim->Instance->CCR1=htim->Instance->CCR1-step; break;
      case TIM_CHANNEL_2:htim->Instance->CCR2=htim->Instance->CCR2-step; break;
      case TIM_CHANNEL_3:htim->Instance->CCR3=htim->Instance->CCR3-step; break;
      case TIM_CHANNEL_4:htim->Instance->CCR4=htim->Instance->CCR4-step; break;
      case TIM_CHANNEL_5:htim->Instance->CCR5=htim->Instance->CCR5-step; break;
      case TIM_CHANNEL_6:htim->Instance->CCR6=htim->Instance->CCR6-step; break;
      default: break;
    }
  }
}*/
double_t step_value[]={0.1,0.01,0.001,0.0001,0.00001};/*max=1*/
uint32_t step_length=get_length(step_value);
double_t duty=0;
double_t pwm_step_duty(double_t const step_duty ,uint32_t const direction, double_t const duty_before) {
  double duty_now ;
  if (direction==1) {
    duty_now = duty_before+step_duty;
    if (duty_now>1) {
      duty_now = 1;
    }
  }else {
    duty_now = duty_before-step_duty;
    if (duty_now<0) {
      duty_now = 0;
    }
  }
  return duty_now;
}

void CCR_pwm_set_duty(TIM_HandleTypeDef const *htim,uint32_t const Channel,double_t const duty) {
  uint32_t const ARR=htim->Instance->ARR;
  uint32_t const CCR=ARR * duty;
  switch(Channel) {
    case TIM_CHANNEL_1:htim->Instance->CCR1=CCR; break;
    case TIM_CHANNEL_2:htim->Instance->CCR2=CCR; break;
    case TIM_CHANNEL_3:htim->Instance->CCR3=CCR; break;
    case TIM_CHANNEL_4:htim->Instance->CCR4=CCR; break;
    case TIM_CHANNEL_5:htim->Instance->CCR5=CCR; break;
    case TIM_CHANNEL_6:htim->Instance->CCR6=CCR; break;
    default: break;
  }
}
