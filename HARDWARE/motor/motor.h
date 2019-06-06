#ifndef __MOTOR_H
#define __MOTOR_H	 
#include "sys.h"


#define PWM_R_F   TIM8->CCR1
#define PWM_L_F   TIM8->CCR3
#define PWM_R_B   TIM8->CCR2
#define PWM_L_B   TIM8->CCR4
extern int8_t dir;

void Set_Pwm(int16_t speed_ideal_left,int16_t speed_ideal_right);

void PWM_Init(u16 arr,u16 psc);

void stop(void);

void run(void);

#endif
