/**
 * This is template for main module created by New Kinetis SDK 2.x Project Wizard. Enjoy!
 **/

#include <string.h>
#include <stdio.h>
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "../application/application.h"

/* Task priorities. */
#define hello_task_PRIORITY (configMAX_PRIORITIES - 1)

/*!
 * @brief Task responsible for printing of "Hello world." message.
 */
static void hello_task(void *pvParameters)
{
//	PRINTF("\n\r\n\rCreat Task Hello world");
	modbus_task_init();
	app_task_init();

	for (;;)
	{
//		PRINTF("\n\rSuspend Task Hello world");
		/* Add your code here */
		vTaskSuspend(NULL);
	}
}

/*!
 * @brief Application entry point.
 */
int main(void)
{
  /* Init board hardware. */
  //BOARD_InitPins();
  BOARD_BootClockRUN();
  //BOARD_InitDebugConsole();

  /* Add your code here */

  /* Create RTOS task */
  xTaskCreate(hello_task,"Hello_task",configMINIMAL_STACK_SIZE,NULL,hello_task_PRIORITY,NULL);
  vTaskStartScheduler();

  for(;;)
  {
	  /* Infinite loop to avoid leaving the main function */
    __asm("NOP"); /* something to use as a breakpoint stop while looping */
  }
}



