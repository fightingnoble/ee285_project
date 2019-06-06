#include "includes.h"


int16_t Encoder_Left;              //左编码器的脉冲计数
int16_t Encoder_Right;             //右编码器的脉冲计数

int16_t speed_ideal_left;   //左轮速度的目标值对应的装载值
int16_t speed_ideal_right;  //右轮速度的目标值对应的装载值
int16_t speed_ideal0;   //  左右轮一样的用于做差的装载值
int16_t speed_actual;   
int16_t step;   

int16_t Balance_Pwm_L;
int16_t Balance_Pwm_R;
int16_t Turn_Pwm;
	
float Proportion; // 比例常数ProportionalConst
float Integral; // 积分常数IntegralConst
float Derivative; // 微分常数DerivativeConst

int16_t LastError_L; // Error[-1]
int16_t PrevError_L; // Error[-2]
int32_t iError_L;    // Sums ofErrors
        
int16_t LastError_R; // Error[-1]
int16_t PrevError_R; // Error[-2]
int32_t iError_R;    // Sums ofErrors

void Limit_range_Pwm(void)
{	
	  int Amplitude=6000;    //===PWM满幅是7200 限制在6000
    if(speed_ideal_left<speed_ideal0) speed_ideal_left=speed_ideal0;	
		if(speed_ideal_left>Amplitude)  speed_ideal_left=Amplitude;	
	  if(speed_ideal_right<speed_ideal0) speed_ideal_right=speed_ideal0;	
		if(speed_ideal_right>Amplitude)  speed_ideal_right=Amplitude;		
	
}
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}
int16_t balance(int16_t Encoder)
{ 
	float Velocity;
	printf("start balance\r\n");
	Velocity =PIDCalc_L(speed_actual,Encoder);
  return (int16_t)Velocity;
}

float PIDCalc_L(int16_t SetPoint, int16_t NextPoint)
{
    int16_t Error,dError,pError; //local var
	  //static int32_t iError_L;     //define as global
    Error =SetPoint - NextPoint; //偏差
    //iError_L += Error; //积分
	  //pError = Error;
    //dError =LastError_L - PrevError_L; //当前微分
	  iError_L = Error;
	  pError = Error - LastError_L;
	  dError =Error -(LastError_L<<1) + PrevError_L; //当前微分
    PrevError_L =LastError_L;
    LastError_L =Error;
    return(Proportion * pError //比例项
    +Integral * iError_L //积分项
    +Derivative * dError //微分项 
		);
}

float PIDCalc_R(int16_t SetPoint, int16_t NextPoint)
{
    int16_t Error,dError,pError; //local var
	  //static int32_t iError_R;     //define as global
    Error =SetPoint - NextPoint; //偏差
    //iError_R += Error; //积分
	  //pError = Error;
    //dError =LastError_R - PrevError_R; //当前微分
	  iError_R = Error;
	  pError = Error - LastError_R;
	  dError =Error -(LastError_R<<1) + PrevError_R; //当前微分
    PrevError_R =LastError_R;
    LastError_R =Error;
    return(Proportion * pError //比例项
    +Integral * iError_R //积分项
    +Derivative * dError //微分项 
		);
}


//float PIDCalc(int16_t NextPoint)
//{
//    double dError, Error;
//    Error =pp->SetPoint - NextPoint; //偏差
//    pp->SumError += Error; //积分
//    dError =pp->LastError - pp->PrevError; //当前微分
//    pp->PrevError =pp->LastError;
//    pp->LastError =Error;
//    return(pp->Proportion * Error //比例项
//    +pp->Integral * pp->SumError //积分项
//    +pp->Derivative * dError //微分项 );
//}

//int turn(int16_t encoder_left,int16_t encoder_right)
//{
//	  static float Turn_Target,Turn,Encoder_temp,Turn_Convert=0.9,Turn_Count;
//	  float Turn_Amplitude=40,Kp=0.5,Kd=0;   
//	  
//  	if(encoder_left!=encoder_right)                      
//		{
//			if(++Turn_Count==1)
//			Encoder_temp=myabs(encoder_left-encoder_right);
//			Turn_Convert=50/Encoder_temp;
//			if(Turn_Convert<0.6)Turn_Convert=0.6;
//			if(Turn_Convert>3)Turn_Convert=3;
//		}	
//	  else
//		{
//			Turn_Convert=0.9;
//			Turn_Count=0;
//			Encoder_temp=0;
//		}			
//	Turn_Target = Turn_Convert;
//    if(Turn_Target>Turn_Amplitude)  Turn_Target=Turn_Amplitude;
//	  if(Turn_Target<-Turn_Amplitude) Turn_Target=-Turn_Amplitude;
//	Turn=-Turn_Target*Kp;                 
//	  return Turn;
//}

int velocity(int16_t encoder_left,int16_t encoder_right)
{  
    static float Velocity,Encoder_Least,Encoder,Movement;
	  static float Encoder_Integral;
	  float kp=80,ki=0.40;
	  Movement=0;	
		Encoder_Least =(Encoder_Left - Encoder_Right)-0;  //===获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此处为零） 
		Encoder *= 0.7;		                         //===一阶低通滤波器       
		Encoder += Encoder_Least*0.3;	             //===一阶低通滤波器    
		Encoder_Integral +=Encoder;                                  //===积分出位移 积分时间：10ms
		Encoder_Integral=Encoder_Integral-Movement;                  //===接收遥控器数据，控制前进后退
	
		if(Encoder_Integral>1000)  	Encoder_Integral=1000;         //===积分限幅
		if(Encoder_Integral<-1000)	Encoder_Integral=-1000;         //===积分限幅	
	
	  if(Encoder_Least>0) Velocity=-(Encoder*kp+Encoder_Integral*ki); //===速度控制	
	  else if (Encoder_Least<0) Velocity=Encoder*kp+Encoder_Integral*ki;
    else Velocity = 0;
	
		if((TIM8->CCER&0X0001)|(TIM8->CCER&0X0100))   Encoder_Integral=0;    //===电机关闭后清除积分
	  return Velocity;
}


// int forward_step()
//{
//    float dis_temp =dis;
//    while(1)
//    {
//        //float delta= dis_temp-dis;
//        TIM_SetCompare1(TIM3,speed_ideal_left);
//        TIM_SetCompare3(TIM3,speed_ideal_right);
//			
//			  printf("\r\n\r\n");
//			  printf("************* E n c o d e r **************** \r\n");        
//        printf("Encoder_Left: %d  ",Encoder_Left);
//        printf("speed_ideal_left:%d\n",speed_ideal_left);           
//        printf("Encoder_Right: %d  ",Encoder_Right);
//        printf("speed_ideal_right:%d\n",speed_ideal_right);         
//        printf("************* D O N E *****************\r\n");	
//        printf("\r\n\r\n");			
//                
//        if((Encoder_Left<step-0.01)|(Encoder_Left>step-0.01))
//            speed_ideal_left+=(speed_actual-Encoder_Left)>>1;
//        else if((Encoder_Right<speed_actual-8)|(Encoder_Right>speed_actual+8))
//            speed_ideal_right+=(speed_actual-Encoder_Right)>>1;
//        else
//            break;
//            }
//    return 1;
//}
// 
// 
// int backward_step()  //后退函数
//{
//    
//    
//    return 1;
//}
// 
