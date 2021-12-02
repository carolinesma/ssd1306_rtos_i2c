/**
 * @file  app_display.h
 * @date  25-November-2021
 * @brief Main application function.
 *
 * @author
 * @author
 */

#include <stdint.h>
#include <stdio.h>
#include "main.h"
#include "cmsis_os.h"
#include "app_display.h"
#include "filas_rtos.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"

/**** Unidades de medida das variáveis ****/

#define V_EIXO "rpm"
#define GPS_X_Y "m"
#define GPS_ANGULO "graus"
#define V_BASE "m/s"
#define V_BASE_W "rpm"

/**** Estados para verificação de falha no display ****/

typedef enum {
    DISPLAY_OK = 0x00,
    DISPLAY_ERR = 0x01
} Display_Error;


TaskHandle_t hdisplayTask;

vEixo v_eixo;
GPS gps;
vBase v_base;

void start_rtos(void)
{
	/* Cria a tarefa que irá imprimir os valores em tela */
	xTaskCreate(displayTask, "displayTask", 128,   NULL,  0,  &hdisplayTask);

	/* Inicia o gerenciado de tarefas */
	vTaskStartScheduler();

	while(1); /* Execution will never reach this line */
}

void displayTask(void *arg)
{
	Display_Error status = DISPLAY_OK; // Variável para verificar o status do display
	char str[32]; // Variável auxiliar para a conversão dos dados em string

	ssd1306_Init();

	while(1) {
		switch (status) {
			case DISPLAY_OK:
				/* Testa a conexão do display */
				if(ssd1306_IICTest() == HAL_OK){

					/* Velocidade Eixos */
					/* Leitura dos dados da fila */
					rDadosVEixo (v_eixo, (TickType_t)500, uxTaskPriorityGet(hdisplayTask));

					/* Procedimento para imprimir os dados */
					ssd1306_SetCursor(1, 2);
					ssd1306_WriteString("Velocidade dos eixos", Font_6x8, White);

					sprintf(str, "W1= %3.1f %s", (float)v_eixo.w1, V_EIXO);
					ssd1306_SetCursor(1, 14);
					ssd1306_WriteString(str, Font_6x8, White);

					sprintf(str, "W2= %3.1f %s", (float)v_eixo.w2, V_EIXO);
					ssd1306_SetCursor(1, 24);
					ssd1306_WriteString(str, Font_6x8, White);

					sprintf(str, "W3= %3.1f %s", (float)v_eixo.w3, V_EIXO);
					ssd1306_SetCursor(1, 34);
					ssd1306_WriteString(str, Font_6x8, White);

					ssd1306_UpdateScreen(); /* Copia as informações do Buffer para a tela */
					vTaskDelay(pdMS_TO_TICKS(3000)); /* 3s */
					ssd1306_Fill(Black);

					/* GPS */

					rDadosGps (gps, (TickType_t)500, uxTaskPriorityGet(hdisplayTask));

					ssd1306_SetCursor(1, 2);
					ssd1306_WriteString("GPS", Font_6x8, White);

					sprintf(str, "X= %3.2f %s", (float)gps.x, GPS_X_Y);
					ssd1306_SetCursor(1, 14);
					ssd1306_WriteString(str, Font_6x8, White);

					sprintf(str, "Y= %3.2f %s", (float)gps.y, GPS_X_Y);
					ssd1306_SetCursor(1, 24);
					ssd1306_WriteString(str, Font_6x8, White);

					sprintf(str, "t= %3.2f %s", (float)gps.angulo_teta, GPS_ANGULO);
					ssd1306_SetCursor(1, 34);
					ssd1306_WriteString(str, Font_6x8, White);

					ssd1306_UpdateScreen(); /* Copia as informações do Buffer para a tela */
					vTaskDelay(pdMS_TO_TICKS(3000)); /* 3s */
					ssd1306_Fill(Black); /* Limpa o display */

					/* Velocidade da base */

					rDadosVBase (v_base, (TickType_t)500, uxTaskPriorityGet(hdisplayTask));

					ssd1306_SetCursor(1, 2);
					ssd1306_WriteString("Velocidade da Base", Font_6x8, White);

					sprintf(str, "Vx= %3.3f %s", (float)v_base.vx, V_BASE);
					ssd1306_SetCursor(0, 12);
					ssd1306_WriteString(str, Font_6x8, White);

					sprintf(str, "Vy= %3.3f %s", (float)v_base.vy, V_BASE);
					ssd1306_SetCursor(0, 22);
					ssd1306_WriteString(str, Font_6x8, White);

					sprintf(str, "W= %3.2f %s", (float)v_base.w, V_BASE_W);
					ssd1306_SetCursor(0, 32);
					ssd1306_WriteString(str, Font_6x8, White);

					ssd1306_UpdateScreen(); /* Copia as informações do Buffer para a tela */
					vTaskDelay(pdMS_TO_TICKS(3000)); /* 3s */
					ssd1306_Fill(Black); /* Limpa o display */
				}
				else status = DISPLAY_ERR;
			break;
			case DISPLAY_ERR:
				if(ssd1306_IICTest() == HAL_OK){
					ssd1306_Init();
					status = DISPLAY_OK;
				} else vTaskDelay(500);
			break;
		}
	}
}
