#ifndef __CTL_H
#define __CTL_H
#include <sys.h>

extern int16_t Encoder_Left;                     //左编码器的脉冲计数
extern int16_t Encoder_Right;                     //右编码器的脉冲计数

extern int16_t speed_ideal_left;
extern int16_t speed_ideal_right;
extern int16_t speed_ideal0;
extern int16_t speed_actual;

	
extern float Proportion; // 比例常数ProportionalConst
extern float Integral; // 积分常数IntegralConst
extern float Derivative; // 微分常数DerivativeConst

extern int16_t LastError_L; // Error[-1]
extern int16_t PrevError_L; // Error[-2]
extern int32_t iError_L;    // Sums ofErrors
               
extern int16_t LastError_R; // Error[-1]
extern int16_t PrevError_R; // Error[-2]
extern int32_t iError_R;    // Sums ofErrors

extern int16_t Balance_Pwm_L;
extern int16_t Balance_Pwm_R;
extern int16_t Turn_Pwm;


void Limit_range_Pwm(void);

int myabs(int a);

float PIDCalc_L(int16_t SetPoint, int16_t NextPoint);
float PIDCalc_R(int16_t SetPoint, int16_t NextPoint);

int16_t balance(int16_t Encoder);

int velocity(int16_t encoder_left,int16_t encoder_right);


#endif
