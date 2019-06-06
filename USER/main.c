#include "includes.h"
#include "sys.h"

//*********************  S T A R T  *******************************//

float sum;  //距离的五次平均值

//debug
float Velocity;
int16_t Error,dError,pError; //local var


 int main(void)
 {
	 
        //float t=89312;
    //int8_t i;
    //int8_t j; 
   speed_ideal_left  = 5656;
   speed_ideal_right = 5151;
   speed_ideal0 = 500;
   speed_actual = 40;
     
     Proportion = 0.5;
     Integral = 0;
     Derivative = 0;
     
     LastError_L=0; // Error[-1]
     PrevError_L=0; // Error[-2]
     iError_L=0;    // Sums ofErrors
     
     LastError_R=0; // Error[-1]
     PrevError_R=0; // Error[-2]
     iError_R=0;    // Sums ofErrors
     
     dir=0;
     Turn_Pwm = 0;
     Balance_Pwm_L=speed_ideal_left;
     Balance_Pwm_R=speed_ideal_left;
		delay_init();	    //延时函数初始化
   	PWM_Init(7199,0);   //=====初始化PWM8 10KHZ，用于驱动电机 如需初始化电调接口，该设置下预装载值越大电平越高
	  delay_ms(300); 
	 
//***************Motor PWM 使用TIM8****************************//	=============TIM8 
//	 
	  TIM_SetCompare1(TIM8,speed_ideal_left); //PC6  接AIN1   
    TIM_SetCompare2(TIM8,speed_ideal0);     //PC7  接AIN2
    TIM_SetCompare3(TIM8,speed_ideal_right);//PC8  接BIN1
    TIM_SetCompare4(TIM8,speed_ideal0);     //PC9  接BIN2
	 
//***********************************************************//
	 
	  Encoder_Init_TIM2(); //=====编码器A接口 ===================================TIM2
	  Encoder_Init_TIM4(); //=====编码器B接口 ===================================TIM4
		TIM3_Cap_Init(0xffff,71);//  超声波模块 Trig--PB13 ; encho--PA6 ===========TIM3
	  TIM6_Int_Init(499,7199); //TIM6用于产生一个50ms的周期中断读取编码器数值====TIM6
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		uart_init(9600);     //uart初始化一定要放在TIM8初始化之后
		delay_ms(100);
		printf("Serial Test!  \r\n");
		printf("Serial OK!  \r\n");
		printf("Start!   \r\n");
		HC_SR04_Init(); //超声波模块初始化
    HC_SR04_Count(); //超声波模块测距
	  	 
//***************Motor PWM 使用TIM8****************************//
	 
//		TIM_SetCompare1(TIM8,5100);		//PC6  接AIN1
//		TIM_SetCompare2(TIM8,1100);	  //PC7  接AIN2
//		TIM_SetCompare3(TIM8,5100);		//PC8  接BIN1
//		TIM_SetCompare4(TIM8,1100);	  //PC9  接BIN2
	 
//***********************************************************//

	while(1)
	{ 	

//    balence();
		
		  HC_SR04_Count(); //超声波模块测距	
			TIM_SetCompare1(TIM8,speed_ideal_right);
			TIM_SetCompare3(TIM8,speed_ideal_left);
	
			printf("************* E n c o d e r **************** \r\n");
			printf("Encoder_Left: %d  ",Encoder_Left);
			printf("speed_ideal_left:%d\n",speed_ideal_left);           
			printf("Encoder_Right: %d  ",Encoder_Right);
			printf("speed_ideal_right:%d\n",speed_ideal_right);         
			printf("************* D O N E *****************\r\n");			
        			
			if((Encoder_Left<speed_actual-8)|(Encoder_Left>speed_actual+8))
					speed_ideal_left+=(speed_actual-Encoder_Left)>>1;
			if((Encoder_Right<speed_actual-8)|(Encoder_Right>speed_actual+8))
					speed_ideal_right+=(speed_actual-Encoder_Right)>>1;

	}

 
 }
 
//*********************************** E N D **************************************//
