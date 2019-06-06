#include "includes.h"

int8_t dir;
void PWM_Init(u16 arr,u16 psc)
{		 		
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);		//高级定时器TIM8使能
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);  //使能GPIO_C外设时钟使能
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //TIM8_CH1-4：PC6 PC7 PC8 PC9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	TIM_DeInit(TIM8);  
	
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;                            //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

  TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH1预装载使能，下次生效
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH2预装载使能
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH3预装载使能
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH4预装载使能	 
	
	TIM_ARRPreloadConfig(TIM8, ENABLE); //使能TI;Mx在ARR上的预装载寄存器
	
	TIM_CtrlPWMOutputs(TIM8,ENABLE);  //！！！！这句话必须加上！！！！
	
	TIM_Cmd(TIM8, ENABLE);  //使能TIM8
 } 

 void Set_Pwm(int16_t speed_ideal_left,int16_t speed_ideal_right)
{
	if(!dir)
	{
    PWM_L_F = speed_ideal_left;
    PWM_R_F = speed_ideal_right;
    PWM_L_B = speed_ideal0;
    PWM_R_B = speed_ideal0;
	}
	else
	{
    PWM_L_F = speed_ideal0;
    PWM_R_F = speed_ideal0;
    PWM_L_B = speed_ideal_left;
    PWM_R_B = speed_ideal_right;
		
	}
	
}

void stop(void)
{
	TIM8->CCER&=0XFFFE;  //CH1输出使能	   
	TIM8->CCER&=0XFEFF;  //CH3输出使能
}


