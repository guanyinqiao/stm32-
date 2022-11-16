#include "led.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "remote.h"
#include <stdlib.h>

/***********************************************************************************************************************************************/ 
/****************************************************************����ң��̰����*****************************************************************/
/***********************************************************************************************************************************************/

												/******������********/
 int main(void)
 {	 
	/*******������ʼ��******/
	u8 key;
	u8 t=0;	
	u16 x_coor = 157;			//��ͷԭX����
	u16 y_coor = 302;			//��ͷԭY����
	u16 level = 1;				//�ؿ�
	u16 score = 0;				//�÷�
	u16 key_0 = 0;				//�ϴμ�ֵ
	u16 direction = 0;			//����,0Ϊ�ޣ�1��2��3��4��
	u16 direction0 = 0;			//ԭ����
	u16 length = 3;				//�߳���
	u16 n = 0;					//����ֵ�������������ֵ����ʵ�������
	u8 reinit = 0;				//��λ��־
	u8 clash=0;					//ʳ��λ�ú������ͻ��־λ
	 
	u16 i;
	u16 coor_x[100]={157,151,145};	//x��������
	u16 coor_y[100]={302,302,302};	//y��������
	u16 x_food,y_food ;				//ʳ������
	
	

	/*******�̼���ʼ��*******/	
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	LCD_Init();	
	Remote_Init();			//������ճ�ʼ�� 
 	POINT_COLOR=GREEN;		//��������Ϊ��ɫ 
	LCD_ShowString(50,30,200,24,24,"Gluttonous Snake");//����
	POINT_COLOR=RED;		//��������Ϊ��ɫ 
	LCD_ShowString(30,70,200,16,16,"Level");		//�ؿ�
	LCD_ShowChar(70,70,':',16,1);//ð��
	LCD_ShowString(200,70,200,16,16,"score");	//�÷�
	LCD_ShowChar(240,70,':',16,1);//ð��	
	LCD_DrawRectangle(1, 100, 318, 478);			//�����α߿�
	LCD_DrawRectangle(2, 101, 317, 477);			//�����α߿�
	for(i=0;i<3;i++)
	{
		LCD_Fill(coor_x[i]-2,coor_y[i]-2,coor_x[i]+2,coor_y[i]+2,i==0?RED:BLACK);	//������
	}
	x_food = rand()%50*6+7;			//ʳ��x����,0~50�������*6+7����һ��ʳ�����ǹ̶���
	y_food = rand()%60*6+104;		//ʳ��y����
	LCD_Fill(x_food-2,y_food-2,x_food+2,y_food+2,BLUE);	//��ʳ��
	
	/********��ѭ��*******/
	while(1)
	{
		key=Remote_Scan();	//����ֵ
		n==10?n=0:n++;		//n��0~10ѭ������
		if(key!=key_0)		//��ֵ��ͬ�ϴ���ִ������仯����
		{	 
			switch(key)
			{
				case 2:direction = 0;;break;	
				case 98:direction = 1;break;	    //�����־	
				case 168:direction = 9;break;	
				case 34:direction = 2;break;				
				case 194:direction = 8;break;	  				 
			}
			if(direction+direction0==10)
			{
				direction = direction0;		//��ֹ����
			}
		}
			
	/******��ʱ��˸LED*******/
		delay_ms(5);	  
		t++;
		if(t==160-level*16)
		{
			t=0;
			LED0=!LED0;
		}
		
	/*******�Զ�ǰ��********/
		if(t==0 && direction !=0 )			//ÿ����ѭ����ԭ�����Զ�ǰ��һ��
		{
			for(i=length;i>0;i--)
			{
				coor_x[i] = coor_x[i-1];
				coor_y[i] = coor_y[i-1];
			}
			switch(direction)
			{
				case 1:coor_y[0] -=6;break;
				case 9:coor_y[0] +=6;break;
				case 2:coor_x[0] -=6;break;
				case 8:coor_x[0] +=6;break;
			}
		}
		
	/********ײǽ��λ*******/
		if(coor_x[0] <=3||coor_x[0] >=316||coor_y[0] <=103||coor_y[0] >=476)					//ײǽ���
		{
			LCD_ShowString(100,250,200,24,24,"GAME OVER!");	//ײǽ
			reinit = 1;
		}
		
	/*******ҧβ����********/
		for(i=2;i<=length;i++)
		{
			if(coor_x[0]==coor_x[i] && coor_y[0]==coor_y[i])
			{
				LCD_ShowString(80,250,200,24,24,"GAME OVER!	");	//ҧβ����
				reinit = 1;	break;					
			}
		}
			
		
	/********��λ����*******/
		if(reinit)
		{
				do 
			{
				key=Remote_Scan();		//��ȡ��ֵ
			}
			while(key!=2);			//OK������ѭ��
			x_coor = 157;			//��ͷ��Xԭ����        //���³�ʼ��	
			y_coor = 302;			//��ͷ��Yԭ����
			level = 1;				//�ؿ�
			score = 0;				//�÷�
			key_0 = 0;				//�ϴμ�ֵ
			direction = 0;			//����,0Ϊ�ޣ�1��2��3��4��
			length = 3;				//�߳���
			n = 0;					//����ֵ�������������ֵ����ʵ�������
			reinit = 0;				//��λ��־
			clash=0;					//ʳ��λ�ú������ͻ��־λ
			LCD_Fill(3,102,316,476,WHITE  );		//�������
			coor_x[0]=157;coor_x[1]= 151;coor_x[2]=145;
			coor_y[0]=302;coor_y[1]=302;coor_y[2]=302;
			for(i=0;i<3;i++)
			{
				LCD_Fill(coor_x[i]-2,coor_y[i]-2,coor_x[i]+2,coor_y[i]+2,i==0?RED:BLACK);	//������
			}
			do
				{
					x_food = rand()%6*6*n+7;		//ʳ��x���꣬7~307֮������꣬ÿ����������6����
					y_food = rand()%7*6*n+104;		//ʳ��y���꣬104~464֮�������
					for(i=0;i<length;i++)
					{
						if(x_food==coor_x[i]&&y_food==coor_y[i])
						{
							clash = 1;break;
						}
						else
						{
							clash = 0;
						}
					}
				}
				while(clash);
			LCD_Fill(x_food-2,y_food-2,x_food+2,y_food+2,BLUE);	//��ʳ��
			reinit = 0;
		}
	
	/*******ˢ����ʾ*******/
		LCD_ShowNum(80,70,level,2,16);		//��ʾ�ؿ�	
		LCD_ShowNum(250,70,score,3,16);		//��ʾ�÷�
		if(coor_x[0] !=x_coor || coor_y[0] != y_coor)  //��������б仯��ˢ����ʾ
		{
			if(x_food==coor_x[0] && y_food==coor_y[0])		//��ͷ�����ʳ��������ͬ
			{
				do
				{
					x_food = rand()%6*6*n+7;		//ʳ��x���꣬7~307֮������꣬ÿ����������6����
					y_food = rand()%7*6*n+104;		//ʳ��y���꣬104~464֮�������
					for(i=0;i<length;i++)
					{
						if(x_food==coor_x[i]&&y_food==coor_y[i])
						{
							clash = 1;break;
						}
						else
						{
							clash = 0;
						}
					}
				}
				while(clash);								//��ʳ��λ�ú������ͻ��⣬�����ͻ�����²���λ��
				
				LCD_Fill(x_food-2,y_food-2,x_food+2,y_food+2,BLUE);	//��ʳ��
				length++;			//����+1
				score++;
				if(score==10)
				{
					score = 0;
					level++;
					if(level==10)
					{
						LCD_ShowString(80,250,200,24,24,"Congratulation!");	//ͨ��
						reinit = 1;
					}
				}
			}
			LCD_Fill(coor_x[0]-2,coor_y[0]-2,coor_x[0]+2,coor_y[0]+2,RED);	//����ͷ����
			LCD_Fill(coor_x[1]-2,coor_y[1]-2,coor_x[1]+2,coor_y[1]+2,BLACK);	//���ڶ��ڷ���
			LCD_Fill(coor_x[length]-2,coor_y[length]-2,coor_x[length]+2,coor_y[length]+2,WHITE  );		//���ĩ�˵���ʾ
		}
		
	/*********��ԭֵ********/
		key_0 = key;
		x_coor = coor_x[0] ;
		y_coor = coor_y[0];
		direction0 = direction;
		
	}
}
 





