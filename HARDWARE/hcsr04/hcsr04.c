#include "includes.h"

u32 tim;
u16 cnt;
int t = 0;
float dis;
//float sum = 0;
	u32 temp=0; 
extern u8  TIM3CH1_CAPTURE_STA;		//输入捕获状态		    				
extern u16	TIM3CH1_CAPTURE_VAL;	//输入捕获值	
void HC_SR04_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;					
 	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		//PB12--ECHO
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		//浮空输入
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;			//PB13--TRIG	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;					//模拟输出
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
 		if(TIM3CH1_CAPTURE_STA&0X80)//成功捕获到了一次上升沿
		{
			temp=TIM3CH1_CAPTURE_STA&0X3F;
			temp*=65536;//溢出时间总和
			temp+=TIM3CH1_CAPTURE_VAL;//得到总的高电平时间
			dis=temp*0.017;
			//printf("\r\ndis=%f cm\n",dis);
			//printf("HIGH:%d us\r\n",temp);//打印总的高点平时间
			TIM3CH1_CAPTURE_STA=0;//开启下一次捕获
		}
		if(t!=5)  //取5次平均值
		{
			t++;
			sum=sum+dis;
		}
		else
		{
			t=0;
			sum=sum/5.0;  //取5次平均值
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
