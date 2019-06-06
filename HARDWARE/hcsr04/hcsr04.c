#include "includes.h"

u32 tim;
u16 cnt;
int t = 0;
float dis;
//float sum = 0;
	u32 temp=0; 
extern u8  TIM3CH1_CAPTURE_STA;		//���벶��״̬		    				
extern u16	TIM3CH1_CAPTURE_VAL;	//���벶��ֵ	
void HC_SR04_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;					
 	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		//PB12--ECHO
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		//��������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;			//PB13--TRIG	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;					//ģ�����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	GPIO_SetBits(GPIOB, GPIO_Pin_13);
			
}

void Trig_Pulse(void)
{
	Trig=1;
	delay_us(15);
	Trig=0;
}

void Echo_Count(void)
{
		
		delay_ms(10);	 		 
 		if(TIM3CH1_CAPTURE_STA&0X80)//�ɹ�������һ��������
		{
			temp=TIM3CH1_CAPTURE_STA&0X3F;
			temp*=65536;//���ʱ���ܺ�
			temp+=TIM3CH1_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
			dis=temp*0.017;
			//printf("\r\ndis=%f cm\n",dis);
			//printf("HIGH:%d us\r\n",temp);//��ӡ�ܵĸߵ�ƽʱ��
			TIM3CH1_CAPTURE_STA=0;//������һ�β���
		}
		if(t!=5)  //ȡ5��ƽ��ֵ
		{
			t++;
			sum=sum+dis;
		}
		else
		{
			t=0;
			sum=sum/5.0;  //ȡ5��ƽ��ֵ
			printf("************* S E N S O R **************** \r\n"); 
			printf("%f\r\n",sum);
		  printf("************ D O N E *****************\r\n");		
			sum=0;
//	  delay_ms(500);
		}
	
}
void HC_SR04_Count(void)
{
		Trig_Pulse();
		Echo_Count();
			}
