/**
 * @file  maincode.h
 * @date  25-November-2021
 * @brief Main application function.
 *
 * @author
 * @author
 */

#include <stdint.h>
#include "main.h"
#include "cmsis_os.h"
//display and fonts library
#include "ssd1306.h"
#include "ssd1306_fonts.h"

TaskHandle_t hdisplayTask; /* Task handles for LED blink task */
TaskHandle_t hi2cTask;

void displayTask(void *arg);
void i2cTask(void *arg);


void start_rtos(void)
{

	xTaskCreate(displayTask, "displayTask", 128,   NULL,  1,  &hdisplayTask);

	/* Start the scheduler */
	vTaskStartScheduler();

	while(1); /* Execution will never reach this line */
}


void displayTask(void *arg)
{
	while(1)
	{



	}
}
