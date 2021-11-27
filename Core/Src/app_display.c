/**
 * @file  app_display.h
 * @date  25-November-2021
 * @brief Main application function.
 *
 * @author
 * @author
 */

#include <stdint.h>
#include "main.h"
#include "cmsis_os.h"
#include "app_display.h"
//display and fonts library
#include "ssd1306.h"
#include "ssd1306_fonts.h"

TaskHandle_t hdisplayTask; /* Task handles for LED blink task */
TaskHandle_t hi2cTask;

void start_rtos(void)
{
	ssd1306_Init(); /* Inicializa o display ???? Deve ser incializado nessa parte do código? */

	//criar tarefas
	xTaskCreate(displayTask, "displayTask", 128,   NULL,  1,  &hdisplayTask);

	/* Start the scheduler */
	vTaskStartScheduler();

	while(1); /* Execution will never reach this line */
}


void displayTask(void *arg)
{

	while(1)
	{
		/* Velocidade */
		ssd1306_SetCursor(8, 4);
		ssd1306_WriteString("Velocidade dos motores", Font_6x8, White);

		ssd1306_SetCursor(20, 14);
		ssd1306_WriteString("v1", Font_6x8, White);
		ssd1306_SetCursor(20, 24);
		ssd1306_WriteString("V2", Font_6x8, White);
		ssd1306_SetCursor(20, 34);
		ssd1306_WriteString("V3", Font_6x8, White);

		ssd1306_UpdateScreen(); /* Copia as infromações do Buffer para a tela */
		vTaskDelay(pdMS_TO_TICKS(3000)); /* 3s */
		ssd1306_Fill(Black);

		/* GPS */
		ssd1306_SetCursor(8, 4);
		ssd1306_WriteString("Localização", Font_6x8, White);

		ssd1306_SetCursor(20, 14);
		ssd1306_WriteString("x = ", Font_6x8, White);
		ssd1306_SetCursor(20, 24);
		ssd1306_WriteString("y = ", Font_6x8, White);
		ssd1306_SetCursor(20, 34);
		ssd1306_WriteString("z = ", Font_6x8, White);

		ssd1306_UpdateScreen(); /* Copia as infromações do Buffer para a tela */
		vTaskDelay(pdMS_TO_TICKS(3000)); /* 3s */
		ssd1306_Fill(Black); /* Limpa o display */

		/* GPS */
		ssd1306_SetCursor(8, 4);
		ssd1306_WriteString("Velocidade do Robô", Font_6x8, White);

		ssd1306_SetCursor(20, 14);
		ssd1306_WriteString("V = ", Font_6x8, White);

		ssd1306_UpdateScreen(); /* Copia as infromações do Buffer para a tela */
		vTaskDelay(pdMS_TO_TICKS(3000)); /* 3s */
		ssd1306_Fill(Black); /* Limpa o display */

	}
}
