/**
 * timer.c
 *
 *  Created on: 04/09/2019
 *      Author: evandro
 */
#include "timer.h"

/**
 *
 */
#define TIMER_IRQ 			PIT_CH0_IRQHandler
#define TIMER_PIT_IRQ_ID 	PIT_CH0_IRQn

/**
 *
 */
void (*timer_task_irq)(void);

/**
 *
 */
void timer_init(uint32_t count)
{
    /* Structure of initialize PIT */
    pit_config_t pitConfig;

    /*
     * pitConfig.enableRunInDebug = false;
     */
    PIT_GetDefaultConfig(&pitConfig);

    /* Init pit module */
    PIT_Init(PIT, &pitConfig);

    /* Set timer period for channel 0 */
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(count, CLOCK_GetFreq(kCLOCK_BusClk)));

    /* Enable timer interrupts for channel 0 */
    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

    /* Enable at the NVIC */
    EnableIRQ(TIMER_PIT_IRQ_ID);
}

/**
 *
 */
void timer_callback_irq(void (*task)(void))
{
	if(task !=NULL)
	{
		timer_task_irq = task;
	}
}

/**
 *
 */
void timer_start(void)
{
	PIT_StartTimer(PIT, kPIT_Chnl_0);
}

/**
 *
 */
void timer_stop(void)
{
	PIT_StopTimer(PIT, kPIT_Chnl_0);
}

/**
 *
 */
void TIMER_IRQ(void)
{
	if(timer_task_irq != NULL)
	{
	    /* Clear interrupt flag.*/
	    PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
		timer_task_irq();
	}
}
