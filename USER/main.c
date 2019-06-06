#include "includes.h"
#include "sys.h"

//*********************  S T A R T  *******************************//

int Encoder_Left;              //左编码器的脉冲计数
int Encoder_Right;             //右编码器的脉冲计数
float sum;  //距离的五次平均值
float step;
int16_t speed_ideal_left;   //左轮速度的目标值对应的装载值
int16_t speed_ideal_right;  //右轮速度的目标值对应的装载值
int16_t speed_ideal0;   //  左右轮一样的用于做差的装载值
int16_t speed_actual;   
//int8_t i;
//int8_t j; 
int balance();

 int main(void)
 {
	 
		//float t=89312;
//	  int8_t i;
//		int8_t j; 
    int16_t speed_ideal_left  = 5656;
    int16_t speed_ideal_right = 5651;
    int16_t speed_ideal0 = 500;
    int16_t speed_actual = 120;
	 
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
			TIM_SetCompare1(TIM8,speed_ideal_left);
			TIM_SetCompare3(TIM8,speed_ideal_right);
	
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
 
 int forward_step()
{
    float dis_temp =dis;
    while(1)
    {
        //float delta= dis_temp-dis;
        TIM_SetCompare1(TIM3,speed_ideal_left);
        TIM_SetCompare3(TIM3,speed_ideal_right);
			
			  printf("\r\n\r\n");
			  printf("************* E n c o d e r **************** \r\n");        
        printf("Encoder_Left: %d  ",Encoder_Left);
        printf("speed_ideal_left:%d\n",speed_ideal_left);           
        printf("Encoder_Right: %d  ",Encoder_Right);
        printf("speed_ideal_right:%d\n",speed_ideal_right);         
        printf("************* D O N E *****************\r\n");	
        printf("\r\n\r\n");			
                
        if((Encoder_Left<step-0.01)|(Encoder_Left>step-0.01))
            speed_ideal_left+=(speed_actual-Encoder_Left)>>1;
        else if((Encoder_Right<speed_actual-8)|(Encoder_Right>speed_actual+8))
            speed_ideal_right+=(speed_actual-Encoder_Right)>>1;
        else
            break;
            }
    return 1;
}
 
 
 int backward_step()  //后退函数
{
    
    
    return 1;
}
 
 int balance()    
{
    while(1)
    {
        TIM_SetCompare1(TIM8,speed_ideal_left);
        TIM_SetCompare3(TIM8,speed_ideal_right);
			
			  printf("\r\n\r\n");
		    printf("************* E n c o d e r **************** \r\n");
        printf("Encoder_Left: %d  ",Encoder_Left);
        printf("speed_ideal_left:%d\n",speed_ideal_left);           
        printf("Encoder_Right: %d  ",Encoder_Right);
        printf("speed_ideal_right:%d\n",speed_ideal_right);  
        printf("************* D O N E *****************\r\n");	
        printf("\r\n\r\n");			
        
        if((Encoder_Left<speed_actual-8)|(Encoder_Left>speed_actual+8))
            speed_ideal_left+=(speed_actual-Encoder_Left)>>1;
        else if((Encoder_Right<speed_actual-8)|(Encoder_Right>speed_actual+8))
            speed_ideal_right+=(speed_actual-Encoder_Right)>>1;
        else
            break;
    }
    return 1;
}
//*********************************** E N D **************************************//