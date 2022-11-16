#include "led.h"
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "remote.h"
#include <stdlib.h>

/***********************************************************************************************************************************************/ 
/****************************************************************红外遥控贪吃蛇*****************************************************************/
/***********************************************************************************************************************************************/

												/******主函数********/
 int main(void)
 {	 
	/*******变量初始化******/
	u8 key;
	u8 t=0;	
	u16 x_coor = 157;			//蛇头原X坐标
	u16 y_coor = 302;			//蛇头原Y坐标
	u16 level = 1;				//关卡
	u16 score = 0;				//得分
	u16 key_0 = 0;				//上次键值
	u16 direction = 0;			//方向,0为无，1上2下3左4右
	u16 direction0 = 0;			//原方向
	u16 length = 3;				//蛇长度
	u16 n = 0;					//计数值，用于随机数基值，以实现真随机
	u8 reinit = 0;				//复位标志
	u8 clash=0;					//食物位置和蛇体冲突标志位
	 
	u16 i;
	u16 coor_x[100]={157,151,145};	//x坐标数组
	u16 coor_y[100]={302,302,302};	//y坐标数组
	u16 x_food,y_food ;				//食物坐标
	
	

	/*******固件初始化*******/	
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 	LED_Init();			     //LED端口初始化
	LCD_Init();	
	Remote_Init();			//红外接收初始化 
 	POINT_COLOR=GREEN;		//设置字体为红色 
	LCD_ShowString(50,30,200,24,24,"Gluttonous Snake");//标题
	POINT_COLOR=RED;		//设置字体为红色 
	LCD_ShowString(30,70,200,16,16,"Level");		//关卡
	LCD_ShowChar(70,70,':',16,1);//冒号
	LCD_ShowString(200,70,200,16,16,"score");	//得分
	LCD_ShowChar(240,70,':',16,1);//冒号	
	LCD_DrawRectangle(1, 100, 318, 478);			//画矩形边框
	LCD_DrawRectangle(2, 101, 317, 477);			//画矩形边框
	for(i=0;i<3;i++)
	{
		LCD_Fill(coor_x[i]-2,coor_y[i]-2,coor_x[i]+2,coor_y[i]+2,i==0?RED:BLACK);	//画方块
	}
	x_food = rand()%50*6+7;			//食物x坐标,0~50的随机数*6+7，第一个食物总是固定的
	y_food = rand()%60*6+104;		//食物y坐标
	LCD_Fill(x_food-2,y_food-2,x_food+2,y_food+2,BLUE);	//画食物
	
	/********主循环*******/
	while(1)
	{
		key=Remote_Scan();	//读键值
		n==10?n=0:n++;		//n从0~10循环计数
		if(key!=key_0)		//键值不同上次则执行坐标变化动作
		{	 
			switch(key)
			{
				case 2:direction = 0;;break;	
				case 98:direction = 1;break;	    //方向标志	
				case 168:direction = 9;break;	
				case 34:direction = 2;break;				
				case 194:direction = 8;break;	  				 
			}
			if(direction+direction0==10)
			{
				direction = direction0;		//防止逆行
			}
		}
			
	/******延时闪烁LED*******/
		delay_ms(5);	  
		t++;
		if(t==160-level*16)
		{
			t=0;
			LED0=!LED0;
		}
		
	/*******自动前进********/
		if(t==0 && direction !=0 )			//每几次循环按原方向自动前进一格
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
		
	/********撞墙复位*******/
		if(coor_x[0] <=3||coor_x[0] >=316||coor_y[0] <=103||coor_y[0] >=476)					//撞墙检测
		{
			LCD_ShowString(100,250,200,24,24,"GAME OVER!");	//撞墙
			reinit = 1;
		}
		
	/*******咬尾程序********/
		for(i=2;i<=length;i++)
		{
			if(coor_x[0]==coor_x[i] && coor_y[0]==coor_y[i])
			{
				LCD_ShowString(80,250,200,24,24,"GAME OVER!	");	//咬尾结束
				reinit = 1;	break;					
			}
		}
			
		
	/********复位程序*******/
		if(reinit)
		{
				do 
			{
				key=Remote_Scan();		//读取键值
			}
			while(key!=2);			//OK键跳出循环
			x_coor = 157;			//蛇头的X原坐标        //重新初始化	
			y_coor = 302;			//蛇头的Y原坐标
			level = 1;				//关卡
			score = 0;				//得分
			key_0 = 0;				//上次键值
			direction = 0;			//方向,0为无，1上2下3左4右
			length = 3;				//蛇长度
			n = 0;					//计数值，用于随机数基值，以实现真随机
			reinit = 0;				//复位标志
			clash=0;					//食物位置和蛇体冲突标志位
			LCD_Fill(3,102,316,476,WHITE  );		//清除画面
			coor_x[0]=157;coor_x[1]= 151;coor_x[2]=145;
			coor_y[0]=302;coor_y[1]=302;coor_y[2]=302;
			for(i=0;i<3;i++)
			{
				LCD_Fill(coor_x[i]-2,coor_y[i]-2,coor_x[i]+2,coor_y[i]+2,i==0?RED:BLACK);	//画方块
			}
			do
				{
					x_food = rand()%6*6*n+7;		//食物x坐标，7~307之间的坐标，每个坐标点相隔6像素
					y_food = rand()%7*6*n+104;		//食物y坐标，104~464之间的坐标
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
			LCD_Fill(x_food-2,y_food-2,x_food+2,y_food+2,BLUE);	//画食物
			reinit = 0;
		}
	
	/*******刷新显示*******/
		LCD_ShowNum(80,70,level,2,16);		//显示关卡	
		LCD_ShowNum(250,70,score,3,16);		//显示得分
		if(coor_x[0] !=x_coor || coor_y[0] != y_coor)  //如果坐标有变化则刷新显示
		{
			if(x_food==coor_x[0] && y_food==coor_y[0])		//蛇头坐标和食物坐标相同
			{
				do
				{
					x_food = rand()%6*6*n+7;		//食物x坐标，7~307之间的坐标，每个坐标点相隔6像素
					y_food = rand()%7*6*n+104;		//食物y坐标，104~464之间的坐标
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
				while(clash);								//新食物位置和蛇身冲突检测，如果冲突则重新产生位置
				
				LCD_Fill(x_food-2,y_food-2,x_food+2,y_food+2,BLUE);	//画食物
				length++;			//长度+1
				score++;
				if(score==10)
				{
					score = 0;
					level++;
					if(level==10)
					{
						LCD_ShowString(80,250,200,24,24,"Congratulation!");	//通关
						reinit = 1;
					}
				}
			}
			LCD_Fill(coor_x[0]-2,coor_y[0]-2,coor_x[0]+2,coor_y[0]+2,RED);	//画蛇头方块
			LCD_Fill(coor_x[1]-2,coor_y[1]-2,coor_x[1]+2,coor_y[1]+2,BLACK);	//画第二节方块
			LCD_Fill(coor_x[length]-2,coor_y[length]-2,coor_x[length]+2,coor_y[length]+2,WHITE  );		//清除末端的显示
		}
		
	/*********存原值********/
		key_0 = key;
		x_coor = coor_x[0] ;
		y_coor = coor_y[0];
		direction0 = direction;
		
	}
}
 





