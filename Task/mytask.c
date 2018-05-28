#include "mytask.h"
#include "urat.h"

#define START_TASK_PRIO		3
#define START_STK_SIZE 		128
OS_TCB StartTaskTCB;
CPU_STK START_TASK_STK[START_STK_SIZE];
void start_task(void *p_arg);

#define TASK1_TASK_PRIO		4
#define TASK1_STK_SIZE 		128
OS_TCB Task1_TaskTCB;
CPU_STK TASK1_TASK_STK[TASK1_STK_SIZE];
void task1_task(void *p_arg);

#define TASK2_TASK_PRIO		5
#define TASK2_STK_SIZE 		128
OS_TCB Task2_TaskTCB;
CPU_STK TASK2_TASK_STK[TASK2_STK_SIZE];
void task2_task(void *p_arg);

#define TASK3_TASK_PRIO		2
#define TASK3_STK_SIZE 		128
OS_TCB Task3_TaskTCB;
CPU_STK TASK3_TASK_STK[TASK3_STK_SIZE];
void task3_task(void *p_arg);

#define TASK4_TASK_PRIO		5
#define TASK4_STK_SIZE 		128
OS_TCB TASK4_TaskTCB;
CPU_STK TASK4_TASK_STK[TASK4_STK_SIZE];
void task4_task(void *p_arg);



#define  SystemDatasBroadcast_PRIO            12 // 统计任务优先级最低，我这里是12，已经低于其他任务的优先级了  
#define  SystemDatasBroadcast_STK_SIZE       100 // 任务的堆栈大小，做统计一般够了，统计结果出来后不够再加..  
OS_TCB  SystemDatasBroadcast_TCB;        // 定义统计任务的TCB  
CPU_STK SystemDatasBroadcast_STK [SystemDatasBroadcast_STK_SIZE];// 开辟数组作为任务栈给任务使用  
void SystemDatasBroadcast(void *p_arg);

extern uint8_t temp;
extern UART_HandleTypeDef huart2;

int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();//关闭全部中断
	
	OSInit(&err);		    //初始化UCOSIII
	OS_CRITICAL_ENTER();	//进入临界区			 

	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
				 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区	 
	OSStart(&err);      //开启UCOSIII
}


//开始任务任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	
	HAL_Init();
	SystemClock_Config();
	init_function();
	//BSP_Init();
	CPU_Init();
	
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif
	
	OS_CRITICAL_ENTER();	//进入临界区
	//创建TASK1任务
//	HAL_UART_Receive_IT(&huart2,&temp,1);
						 
	OSTaskCreate((OS_TCB 	* )&Task1_TaskTCB,		
				 (CPU_CHAR	* )"Task1 task", 		
                 (OS_TASK_PTR )task1_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )TASK1_TASK_PRIO,     
                 (CPU_STK   * )&TASK1_TASK_STK[0],	
                 (CPU_STK_SIZE)TASK1_STK_SIZE/10,	
                 (CPU_STK_SIZE)TASK1_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);				
				 
	//创建TASK2任务
	OSTaskCreate((OS_TCB 	* )&Task2_TaskTCB,		
				 (CPU_CHAR	* )"task2 task", 		
                 (OS_TASK_PTR )task2_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )TASK2_TASK_PRIO,     	
                 (CPU_STK   * )&TASK2_TASK_STK[0],	
                 (CPU_STK_SIZE)TASK2_STK_SIZE/10,	
                 (CPU_STK_SIZE)TASK2_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);		

	OSTaskCreate((OS_TCB 	* )&Task3_TaskTCB,		
				 (CPU_CHAR	* )"task3 task", 		
                 (OS_TASK_PTR )task3_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )TASK3_TASK_PRIO,     	
                 (CPU_STK   * )&TASK3_TASK_STK[0],	
                 (CPU_STK_SIZE)TASK3_STK_SIZE/10,	
                 (CPU_STK_SIZE)TASK3_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);	

	 OSTaskCreate (( OS_TCB   *  )  &TASK4_TaskTCB,          //TCB控制块  
                  ( CPU_CHAR *  )  "task4 task",            //任务名称  
                  ( OS_TASK_PTR )   task4_task,             //任务函数  
                  ( void     *  )   0,                      //任务函数参数  
                  ( OS_PRIO     )   TASK4_TASK_PRIO,        //优先级  
                  ( CPU_STK  *  )   TASK4_TASK_STK,         //任务堆栈  
                  ( CPU_STK_SIZE)   TASK4_STK_SIZE/10,      //栈深  
                  ( CPU_STK_SIZE)   TASK4_STK_SIZE,         //堆栈大小  
                  ( OS_MSG_QTY  )   0,                      //任务内建消息队列  
                  ( OS_TICK     )   0,                      //时间片轮转  
                  ( void     *  )   0,                      //数据扩展区  
                  ( OS_OPT      )   OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,//选项 堆栈设置 清零  
                  ( OS_ERR   *  )   &err);  	//存放该函数错误时的返回值
	

	OSTaskCreate( (OS_TCB     *)&SystemDatasBroadcast_TCB,  
				(CPU_CHAR   *)"SystemDatasBroadcast",  
				(OS_TASK_PTR ) SystemDatasBroadcast,  
				(void       *) 0,  
				(OS_PRIO     ) SystemDatasBroadcast_PRIO,  
				(CPU_STK    *)&SystemDatasBroadcast_STK[0],  
				(CPU_STK_SIZE) SystemDatasBroadcast_STK_SIZE/10,/*栈溢出临界值我设置在栈大小的90%处*/  
				(CPU_STK_SIZE) SystemDatasBroadcast_STK_SIZE,  
				(OS_MSG_QTY  ) 0,  
				(OS_TICK     ) 0,  
				(void       *) 0,  
				(OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),   
				(OS_ERR     *) &err);     
				  
	OS_CRITICAL_EXIT();	//退出临界区
				 
	OSTaskDel((OS_TCB*)0,&err);	//删除start_task任务自身
}


//task1任务函数
void task1_task(void *p_arg)
{
	OS_ERR err;
//	CPU_SR_ALLOC();
	p_arg = p_arg;

	while(1)
	{
		//printf_test();
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,(GPIO_PinState)!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1));
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); //延时1s
	}
}

//task2任务函数
void task2_task(void *p_arg)
{
	OS_ERR err;
//	CPU_SR_ALLOC();
	p_arg = p_arg;
	while(1)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,(GPIO_PinState)!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5));
		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err); //延时1s
	}
}

extern uint8_t flag,len;
void task3_task(void *p_arg)
{
	OS_ERR err;
//	CPU_SR_ALLOC();
	p_arg = p_arg;
	init_t();
	while(1)
	{
		//HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,(GPIO_PinState)!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7));
		printf_test();
		OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}

void task4_task(void *p_arg)
{
	OS_ERR err;
//	CPU_SR_ALLOC();
	p_arg = p_arg;

	while(1)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,(GPIO_PinState)!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_3));
		OSTimeDlyHMSM(0,0,0,400,OS_OPT_TIME_HMSM_STRICT,&err); //延时1s
	}
}

void  SystemDatasBroadcast (void *p_arg)  
{  
  OS_ERR err;  
  CPU_STK_SIZE free,used;  
  (void)p_arg;  
  while(0)  
  {  
    OSTaskStkChk (&SystemDatasBroadcast_TCB,&free,&used,&err);//  把统计任务本身的堆栈使用量也打印出来  
   
    printf("SystemDatasBroadcast  used/free:%d/%d  usage:%%%d\r\n",used,free,(used*100)/(used+free));  
          
    OSTaskStkChk (&Task1_TaskTCB,&free,&used,&err);  
    printf("Task1_TaskTCB             used/free:%d/%d  usage:%%%d\r\n",used,free,(used*100)/(used+free));  
          
    OSTaskStkChk (&Task2_TaskTCB,&free,&used,&err);  
    printf("Task2_TaskTCB             used/free:%d/%d  usage:%%%d\r\n",used,free,(used*100)/(used+free));  
          
    OSTaskStkChk (&TASK4_TaskTCB,&free,&used,&err);  
    printf("TASK4_TaskTCB              used/free:%d/%d  usage:%%%d\r\n",used,free,(used*100)/(used+free));  
          
    OSTaskStkChk (&Task3_TaskTCB,&free,&used,&err);  
    printf("Task3_TaskTCB          used/free:%d/%d  usage:%%%d\r\n",used,free,(used*100)/(used+free));  
          
    printf("\r\n\r\n\r\n");  
    OSTimeDlyHMSM(0,0,5,0,(OS_OPT)OS_OPT_TIME_DLY,(OS_ERR*)&err);  
   }  
}  


