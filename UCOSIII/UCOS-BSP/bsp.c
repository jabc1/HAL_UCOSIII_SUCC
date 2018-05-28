/**
  ******************************************************************************
  * 文件名程: bsp.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: 基于uCOSIII的硬件底层实现
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-F1Pro使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */
/* 包含头文件 ----------------------------------------------------------------*/
#include "includes.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
#define  DWT_CR      *(CPU_REG32 *)0xE0001000
#define  DWT_CYCCNT  *(CPU_REG32 *)0xE0001004
#define  DEM_CR      *(CPU_REG32 *)0xE000EDFC
#define  DBGMCU_CR   *(CPU_REG32 *)0xE0042004
 
 
/*
 ***********************************************************************
 *                            REGISTER BITS
 ***********************************************************************
 */
#define  DEM_CR_TRCENA                   (1 << 24)
#define  DWT_CR_CYCCNTENA                (1 <<  0)

/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/

/*
 ***********************************************************************
 *                           REGISTERS
 ***********************************************************************
 */
 

/*
 ***********************************************************************
 *                             BSP_CPU_ClkFreq()
 *
 * Description : Read CPU registers to determine the CPU clock frequency of the chip.
 *
 * Argument(s) : none.
 *
 * Return(s)   : The CPU clock frequency, in Hz.
 *
 * Caller(s)   : Application.
 *
 * Note(s)     : none.
 ***********************************************************************
 */
 
CPU_INT32U  BSP_CPU_ClkFreq (void)
{
  CPU_INT32U    hclk_freq;
  hclk_freq = HAL_RCC_GetHCLKFreq();//获取HCLK时钟
  return (hclk_freq);//返HCLK时钟
}

/*
 *********************************************************************
 *                           BSP_Tick_Init()
 *
 * Description : Initialize all the peripherals that required OS Tick services (OS initialized)
 *
 * Argument(s) : none.
 *
 * Return(s)   : none.
 *
 * Caller(s)   : Application.
 *
 * Note(s)     : none.
 *********************************************************************
 */
void BSP_Tick_Init(void)
{
	CPU_INT32U  cpu_clk_freq;
  CPU_INT32U  cnts;	
  cpu_clk_freq = BSP_CPU_ClkFreq(); 
  /* Determine SysTick reference freq. */ 
#if (OS_VERSION >= 30000u)
  cnts  = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;       
	/* Determine nbr SysTick increments. */
#else
  cnts  = cpu_clk_freq / (CPU_INT32U)OS_TICKS_PER_SEC;       
  /* Determine nbr SysTick increments. */
#endif 
  OS_CPU_SysTickInit(cnts);                              
//  SysTick_Config(cnts);  	
}


/**
  * 函数功能: 各硬件模块初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void BSP_Init(void)
{

  /* 系统滴答初始化 */
  BSP_Tick_Init();
  
  
}

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrInit (void)
{
	CPU_INT32U  cpu_clk_freq_hz;
  
  DEM_CR         |= (CPU_INT32U)DEM_CR_TRCENA;                
	/* Enable Cortex-M3's DWT CYCCNT reg. */
  DWT_CYCCNT      = (CPU_INT32U)0u;
  DWT_CR         |= (CPU_INT32U)DWT_CR_CYCCNTENA;
 
  cpu_clk_freq_hz = BSP_CPU_ClkFreq();
  CPU_TS_TmrFreqSet(cpu_clk_freq_hz);
}
#endif
 
#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR  CPU_TS_TmrRd (void)
{
  return ((CPU_TS_TMR)DWT_CYCCNT);
}
#endif
 
 
#if (CPU_CFG_TS_32_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS32_to_uSec (CPU_TS32  ts_cnts)
{
	CPU_INT64U  ts_us;
  CPU_INT64U  fclk_freq;

 
  fclk_freq = BSP_CPU_ClkFreq();
  ts_us     = ts_cnts / (fclk_freq / DEF_TIME_NBR_uS_PER_SEC);

  return (ts_us);
}
#endif
 
 
#if (CPU_CFG_TS_64_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS64_to_uSec (CPU_TS64  ts_cnts)
{
	CPU_INT64U  ts_us;
	CPU_INT64U  fclk_freq;


  fclk_freq = BSP_CPU_ClkFreq();
  ts_us     = ts_cnts / (fclk_freq / DEF_TIME_NBR_uS_PER_SEC);
	
  return (ts_us);
}
#endif

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
