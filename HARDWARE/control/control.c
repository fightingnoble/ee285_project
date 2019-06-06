#include "includes.h"


int16_t Encoder_Left;              //����������������
int16_t Encoder_Right;             //�ұ��������������

int16_t speed_ideal_left;   //�����ٶȵ�Ŀ��ֵ��Ӧ��װ��ֵ
int16_t speed_ideal_right;  //�����ٶȵ�Ŀ��ֵ��Ӧ��װ��ֵ
int16_t speed_ideal0;   //  ������һ�������������װ��ֵ
int16_t speed_actual;   
int16_t step;   

int16_t Balance_Pwm_L;
int16_t Balance_Pwm_R;
int16_t Turn_Pwm;
	
float Proportion; // ��������ProportionalConst
float Integral; // ���ֳ���IntegralConst
float Derivative; // ΢�ֳ���DerivativeConst

int16_t LastError_L; // Error[-1]
int16_t PrevError_L; // Error[-2]
int32_t iError_L;    // Sums ofErrors
        
int16_t LastError_R; // Error[-1]
int16_t PrevError_R; // Error[-2]
int32_t iError_R;    // Sums ofErrors

void Limit_range_Pwm(void)
{	
	  int Amplitude=6000;    //===PWM������7200 ������6000
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
    Error =SetPoint - NextPoint; //ƫ��
    //iError_L += Error; //����
	  //pError = Error;
    //dError =LastError_L - PrevError_L; //��ǰ΢��
	  iError_L = Error;
	  pError = Error - LastError_L;
	  dError =Error -(LastError_L<<1) + PrevError_L; //��ǰ΢��
    PrevError_L =LastError_L;
    LastError_L =Error;
    return(Proportion * pError //������
    +Integral * iError_L //������
    +Derivative * dError //΢���� 
		);
}

float PIDCalc_R(int16_t SetPoint, int16_t NextPoint)
{
    int16_t Error,dError,pError; //local var
	  //static int32_t iError_R;     //define as global
    Error =SetPoint - NextPoint; //ƫ��
    //iError_R += Error; //����
	  //pError = Error;
    //dError =LastError_R - PrevError_R; //��ǰ΢��
	  iError_R = Error;
	  pError = Error - LastError_R;
	  dError =Error -(LastError_R<<1) + PrevError_R; //��ǰ΢��
    PrevError_R =LastError_R;
    LastError_R =Error;
    return(Proportion * pError //������
    +Integral * iError_R //������
    +Derivative * dError //΢���� 
		);
}


//float PIDCalc(int16_t NextPoint)
//{
//    double dError, Error;
//    Error =pp->SetPoint - NextPoint; //ƫ��
//    pp->SumError += Error; //����
//    dError =pp->LastError - pp->PrevError; //��ǰ΢��
//    pp->PrevError =pp->LastError;
//    pp->LastError =Error;
//    return(pp->Proportion * Error //������
//    +pp->Integral * pp->SumError //������
//    +pp->Derivative * dError //΢���� );
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
		Encoder_Least =(Encoder_Left - Encoder_Right)-0;  //===��ȡ�����ٶ�ƫ��==�����ٶȣ����ұ�����֮�ͣ�-Ŀ���ٶȣ��˴�Ϊ�㣩 
		Encoder *= 0.7;		                         //===һ�׵�ͨ�˲���       
		Encoder += Encoder_Least*0.3;	             //===һ�׵�ͨ�˲���    
		Encoder_Integral +=Encoder;                                  //===���ֳ�λ�� ����ʱ�䣺10ms
		Encoder_Integral=Encoder_Integral-Movement;                  //===����ң�������ݣ�����ǰ������
	
		if(Encoder_Integral>1000)  	Encoder_Integral=1000;         //===�����޷�
		if(Encoder_Integral<-1000)	Encoder_Integral=-1000;         //===�����޷�	
	
	  if(Encoder_Least>0) Velocity=-(Encoder*kp+Encoder_Integral*ki); //===�ٶȿ���	
	  else if (Encoder_Least<0) Velocity=Encoder*kp+Encoder_Integral*ki;
    else Velocity = 0;
	
		if((TIM8->CCER&0X0001)|(TIM8->CCER&0X0100))   Encoder_Integral=0;    //===����رպ��������
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
// int backward_step()  //���˺���
//{
//    
//    
//    return 1;
//}
// 
