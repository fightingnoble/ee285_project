#include "includes.h"
#include "sys.h"

//*********************  S T A R T  *******************************//

float sum;  //��������ƽ��ֵ

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
	 
	delay_init();	    //��ʱ������ʼ��
   	PWM_Init(7199,0);   //=====��ʼ��PWM8 10KHZ������������� �����ʼ������ӿڣ���������Ԥװ��ֵԽ���ƽԽ��
	  delay_ms(300); 
	 
//***************Motor PWM ʹ��TIM8****************************//	=============TIM8 
//	 
	  TIM_SetCompare1(TIM8,speed_ideal_left); //PC6  ��AIN1   
    TIM_SetCompare2(TIM8,speed_ideal0);     //PC7  ��AIN2
    TIM_SetCompare3(TIM8,speed_ideal_right);//PC8  ��BIN1
    TIM_SetCompare4(TIM8,speed_ideal0);     //PC9  ��BIN2
	 
//***********************************************************//
	 
	  Encoder_Init_TIM2(); //=====������A�ӿ� ===================================TIM2
	  Encoder_Init_TIM4(); //=====������B�ӿ� ===================================TIM4
		TIM3_Cap_Init(0xffff,71);//  ������ģ�� Trig--PB13 ; encho--PA6 ===========TIM3
	  TIM6_Int_Init(499,7199); //TIM6���ڲ���һ��50ms�������ж϶�ȡ��������ֵ====TIM6
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		uart_init(9600);     //uart��ʼ��һ��Ҫ����TIM8��ʼ��֮��
		delay_ms(100);
		printf("Serial Test!  \r\n");
		printf("Serial OK!  \r\n");
		printf("Start!   \r\n");
		HC_SR04_Init(); //������ģ���ʼ��
    HC_SR04_Count(); //������ģ����
	  	 
//***************Motor PWM ʹ��TIM8****************************//
	 
//		TIM_SetCompare1(TIM8,5100);		//PC6  ��AIN1
//		TIM_SetCompare2(TIM8,1100);	  //PC7  ��AIN2
//		TIM_SetCompare3(TIM8,5100);		//PC8  ��BIN1
//		TIM_SetCompare4(TIM8,1100);	  //PC9  ��BIN2
	 
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
      Error =speed_actual - Encoder_Left; //ƫ��
      //iError_L += Error; //����
      //pError = Error;
      //dError =LastError_L - PrevError_L; //��ǰ΢��
      iError_L = Error;
      pError = Error - LastError_L;
      dError =Error -(LastError_L<<1) + PrevError_L; //��ǰ΢��
      PrevError_L =LastError_L;
      LastError_L =Error;
      Balance_Pwm_L =(speed_actual-Encoder_Left)>>1;//Proportion * pError+Integral * iError_L+Derivative * dError;

      //static int32_t iError_R;     //define as global
       Error =speed_actual - Encoder_Right; //ƫ��
       iError_R += Error; //����
       pError = Error;
       dError =LastError_R - PrevError_R; //��ǰ΢��
//     iError_R = Error;
//     pError = Error - LastError_R;
//     dError =Error -(LastError_R<<1) + PrevError_R; //��ǰ΢��
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
