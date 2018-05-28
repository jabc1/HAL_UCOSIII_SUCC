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



#define  SystemDatasBroadcast_PRIO            12 // ͳ���������ȼ���ͣ���������12���Ѿ�����������������ȼ���  
#define  SystemDatasBroadcast_STK_SIZE       100 // ����Ķ�ջ��С����ͳ��һ�㹻�ˣ�ͳ�ƽ�������󲻹��ټ�..  
OS_TCB  SystemDatasBroadcast_TCB;        // ����ͳ�������TCB  
CPU_STK SystemDatasBroadcast_STK [SystemDatasBroadcast_STK_SIZE];// ����������Ϊ����ջ������ʹ��  
void SystemDatasBroadcast(void *p_arg);

extern uint8_t temp;
extern UART_HandleTypeDef huart2;

int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();//�ر�ȫ���ж�
	
	OSInit(&err);		    //��ʼ��UCOSIII
	OS_CRITICAL_ENTER();	//�����ٽ���			 

	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���	 
	OSStart(&err);      //����UCOSIII
}


//��ʼ����������
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
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif
	
	OS_CRITICAL_ENTER();	//�����ٽ���
	//����TASK1����
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
				 
	//����TASK2����
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

	 OSTaskCreate (( OS_TCB   *  )  &TASK4_TaskTCB,          //TCB���ƿ�  
                  ( CPU_CHAR *  )  "task4 task",            //��������  
                  ( OS_TASK_PTR )   task4_task,             //������  
                  ( void     *  )   0,                      //����������  
                  ( OS_PRIO     )   TASK4_TASK_PRIO,        //���ȼ�  
                  ( CPU_STK  *  )   TASK4_TASK_STK,         //�����ջ  
                  ( CPU_STK_SIZE)   TASK4_STK_SIZE/10,      //ջ��  
                  ( CPU_STK_SIZE)   TASK4_STK_SIZE,         //��ջ��С  
                  ( OS_MSG_QTY  )   0,                      //�����ڽ���Ϣ����  
                  ( OS_TICK     )   0,                      //ʱ��Ƭ��ת  
                  ( void     *  )   0,                      //������չ��  
                  ( OS_OPT      )   OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,//ѡ�� ��ջ���� ����  
                  ( OS_ERR   *  )   &err);  	//��Ÿú�������ʱ�ķ���ֵ
	

	OSTaskCreate( (OS_TCB     *)&SystemDatasBroadcast_TCB,  
				(CPU_CHAR   *)"SystemDatasBroadcast",  
				(OS_TASK_PTR ) SystemDatasBroadcast,  
				(void       *) 0,  
				(OS_PRIO     ) SystemDatasBroadcast_PRIO,  
				(CPU_STK    *)&SystemDatasBroadcast_STK[0],  
				(CPU_STK_SIZE) SystemDatasBroadcast_STK_SIZE/10,/*ջ����ٽ�ֵ��������ջ��С��90%��*/  
				(CPU_STK_SIZE) SystemDatasBroadcast_STK_SIZE,  
				(OS_MSG_QTY  ) 0,  
				(OS_TICK     ) 0,  
				(void       *) 0,  
				(OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),   
				(OS_ERR     *) &err);     
				  
	OS_CRITICAL_EXIT();	//�˳��ٽ���
				 
	OSTaskDel((OS_TCB*)0,&err);	//ɾ��start_task��������
}


//task1������
void task1_task(void *p_arg)
{
	OS_ERR err;
//	CPU_SR_ALLOC();
	p_arg = p_arg;

	while(1)
	{
		//printf_test();
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1,(GPIO_PinState)!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1));
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ1s
	}
}

//task2������
void task2_task(void *p_arg)
{
	OS_ERR err;
//	CPU_SR_ALLOC();
	p_arg = p_arg;
	while(1)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,(GPIO_PinState)!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5));
		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ1s
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
		OSTimeDlyHMSM(0,0,0,400,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ1s
	}
}

void  SystemDatasBroadcast (void *p_arg)  
{  
  OS_ERR err;  
  CPU_STK_SIZE free,used;  
  (void)p_arg;  
  while(0)  
  {  
    OSTaskStkChk (&SystemDatasBroadcast_TCB,&free,&used,&err);//  ��ͳ��������Ķ�ջʹ����Ҳ��ӡ����  
   
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


