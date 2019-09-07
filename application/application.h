/*
 * application.h
 *
 *  Created on: 01/09/2019
 *      Author: evandro
 */

#ifndef APPLICATION_APPLICATION_H_
#define APPLICATION_APPLICATION_H_

#include <stdio.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "fsl_gpio.h"
#include "fsl_debug_console.h"

#include "../modbus/include/mb.h"
#include "../modbus/include/mbport.h"
#include "../modbus/include/mbutils.h"

void modbus_task_init(void);
void app_task_init(void);

#endif /* APPLICATION_APPLICATION_H_ */
