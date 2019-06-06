#include "includes.h"
#include "sys.h"

//*********************  S T A R T  *******************************//

int Encoder_Left;              //����������������
int Encoder_Right;             //�ұ��������������
float sum;  //��������ƽ��ֵ
float step;
int16_t speed_ideal_left;   //�����ٶȵ�Ŀ��ֵ��Ӧ��װ��ֵ
int16_t speed_ideal_right;  //�����ٶȵ�Ŀ��ֵ��Ӧ��װ��ֵ
int16_t speed_ideal0;   //  ������һ�������������װ��ֵ
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

//    balence();
		
		  HC_SR04_Count(); //������ģ����	
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
 
 
 int backward_step()  //���˺���
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