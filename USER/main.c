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

      printf("\r\n\r\n");
      printf("************* distance **************** \r\n");
      printf("sum:%f\r\n",sum);

//    printf("************* E n c o d e r **************** \r\n");
//    printf("Turn_Pwm: %d\r\n",Turn_Pwm);
//    printf("Encoder_Left: %d \t Encoder_Right:%d \r\n",Encoder_Left,Encoder_Right);
//    printf("Encoder_Left: %d \t Encoder_Right:%d \r\n",Encoder_Left,Encoder_Right);
//    printf("speed_ideal_right:%d speed_ideal_left:%d\r\n",speed_ideal_right,speed_ideal_left);

      printf("************* E n c o d e r **************** \r\n");
      printf("Encoder_Left: %d  Encoder_Right:%d \r\n",Encoder_Left,Encoder_Right);
      printf("speed_ideal_right:%d speed_ideal_left:%d \r\n",speed_ideal_right,speed_ideal_left);
      
      printf("start balance\r\n");
            
      //static int32_t iError_L;     //define as global
      Error =speed_actual - Encoder_Left; //偏差
      //iError_L += Error; //积分
      //pError = Error;
      //dError =LastError_L - PrevError_L; //当前微分
      iError_L = Error;
      pError = Error - LastError_L;
      dError =Error -(LastError_L<<1) + PrevError_L; //当前微分
      PrevError_L =LastError_L;
      LastError_L =Error;
      Balance_Pwm_L =(speed_actual-Encoder_Left)>>1;//Proportion * pError+Integral * iError_L+Derivative * dError;

      //static int32_t iError_R;     //define as global
       Error =speed_actual - Encoder_Right; //偏差
       iError_R += Error; //积分
       pError = Error;
       dError =LastError_R - PrevError_R; //当前微分
//     iError_R = Error;
//     pError = Error - LastError_R;
//     dError =Error -(LastError_R<<1) + PrevError_R; //当前微分
        PrevError_R =LastError_R;
        LastError_R =Error;
        Balance_Pwm_R =(speed_actual-Encoder_Right)>>1;//Proportion * pError+Integral * iError_R+Derivative * dError;

            
        Turn_Pwm = 0;// velocity(speed_ideal_left,speed_ideal_right);
        printf("Balance_Pwm_L: %d\tTurn_Pwm: %d\r\n",Balance_Pwm_L,Turn_Pwm);
                printf("Balance_Pwm_R: %d\r\n",Balance_Pwm_R);
        
        printf("set value...\r\n");
        speed_ideal_left += Balance_Pwm_L+Turn_Pwm;
        speed_ideal_right += Balance_Pwm_R-Turn_Pwm;
        printf("speed_ideal_right:%d speed_ideal_left:%d \r\n",speed_ideal_right,speed_ideal_left);
                
        Limit_range_Pwm();
        printf("add limitation......\r\n");
        printf("speed_ideal_right:%d speed_ideal_left:%d \r\n",speed_ideal_right,speed_ideal_left);
        Set_Pwm(speed_ideal_left,speed_ideal_right);



        // TIM_SetCompare1(TIM8,speed_ideal_left);
        // TIM_SetCompare3(TIM8,speed_ideal_right);
        
        // printf("Encoder_Left: %d  ",Encoder_Left);
        // printf("speed_ideal_left:%d\n",speed_ideal_left);           
        // printf("Encoder_Right: %d  ",Encoder_Right);
        // printf("speed_ideal_right:%d\n",speed_ideal_right);         
        
        // if((Encoder_Left<speed_actual-4)|(Encoder_Left>speed_actual+4)|(Encoder_Right<speed_actual-4)|(Encoder_Right>speed_actual+4))
				// {
            // speed_ideal_left+=(speed_actual-Encoder_Left)>>0;
            // speed_ideal_right+=(speed_actual-Encoder_Right)>>0;
				// }

	}

 
 }
 
//*********************************** E N D **************************************//
