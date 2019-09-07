/*
 * timer.h
 *
 *  Created on: 04/09/2019
 *      Author: evandro
 */

#ifndef HAL_TIMER_H_
#define HAL_TIMER_H_


#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_pit.h"
#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"

void timer_init(uint32_t count);
void timer_callback_irq(void (*task)(void));
void timer_start(void);
void timer_stop(void);

#endif /* HAL_TIMER_H_ */
