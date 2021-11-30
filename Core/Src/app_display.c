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

/* Incluir as bibliotecas do Display */
#include "ssd1306.h"
#include "ssd1306_fonts.h"

TaskHandle_t hdisplayTask;
//TaskHandle_t hi2cTask;

/* Valores a serem mostrados no display.
 * Velocidade dos eixos dos 3 motores, dados do GPS, velocidade da base do robô.
 */

vEixo v_eixo;
GPS gps;
vBase v_base;

/* Variável auxiliar para a conversão dos dados em string */
char str[32];


void start_rtos(void)
{
	/* Valores para Teste
	v_eixo.w1 = 1;
	v_eixo.w2 = 2;
	v_eixo.w3 = 3;
	gps.x = -20.004;
	gps.y = 21.544;
	gps.angulo_teta = 22.000;
	v_base.vx = 10;
	v_base.vy = 10;
	v_base.w = 10;**/

	/* Cria a tarefa que irá imprimir os valores em tela */
	xTaskCreate(displayTask, "displayTask", 128,   NULL,  0,  &hdisplayTask);

	/* Inicia o gerenciado de tarefas */
	vTaskStartScheduler();

	while(1); /* Execution will never reach this line */
}

void displayTask(void *arg)
{
	uint8_t status;
	/* Inicializa o display */
	ssd1306_Init();
	status = 0;

	while(1)
	{
		switch (status)
		{
			case 0:
				/* Testa a conexão do display */
				if(ssd1306_IICTest() == HAL_OK){

					/* Velocidade Eixos */
					/* Leitura dos dados da fila */
					rDadosVEixo (v_eixo, 500, uxTaskPriorityGet(hdisplayTask));

					/* Procedimento para imprimir os dados */
					ssd1306_SetCursor(1, 2);
					ssd1306_WriteString("Velocidade dos eixos", Font_6x8, White);

					sprintf(str, "W1= %3.1f rpm", (float)v_eixo.w1);
					ssd1306_SetCursor(1, 14);
					ssd1306_WriteString(str, Font_6x8, White);

					sprintf(str, "W2= %3.1f rpm", (float)v_eixo.w2);
					ssd1306_SetCursor(1, 24);
					ssd1306_WriteString(str, Font_6x8, White);

					sprintf(str, "W3= %3.1f rpm", (float)v_eixo.w3);
					ssd1306_SetCursor(1, 34);
					ssd1306_WriteString(str, Font_6x8, White);

					ssd1306_UpdateScreen(); /* Copia as informações do Buffer para a tela */
					vTaskDelay(pdMS_TO_TICKS(3000)); /* 3s */
					ssd1306_Fill(Black);

					/* GPS */
					/* Leitura dos dados da fila */
					rDadosGps (gps, 500, uxTaskPriorityGet(hdisplayTask));

					ssd1306_SetCursor(1, 2);
					ssd1306_WriteString("GPS", Font_6x8, White);

					sprintf(str, "X= %3.2fm", (float)gps.x);
					ssd1306_SetCursor(1, 14);
					ssd1306_WriteString(str, Font_6x8, White);

					sprintf(str, "Y= %3.2fm", (float)gps.y);
					ssd1306_SetCursor(1, 24);
					ssd1306_WriteString(str, Font_6x8, White);

					sprintf(str, "t= %3.2fº", (float)gps.angulo_teta);
					ssd1306_SetCursor(1, 34);
					ssd1306_WriteString(str, Font_6x8, White);

					ssd1306_UpdateScreen(); /* Copia as informações do Buffer para a tela */
					vTaskDelay(pdMS_TO_TICKS(3000)); /* 3s */
					ssd1306_Fill(Black); /* Limpa o display */

					/* Velocidade escalar */
					/* Leitura dos dados da fila */
					rDadosVBase (v_base, 500, uxTaskPriorityGet(hdisplayTask));

					ssd1306_SetCursor(1, 2);
					ssd1306_WriteString("Velocidade da Base", Font_6x8, White);

					sprintf(str, "Vx= %3.3fm/s", (float)v_base.vx);
					ssd1306_SetCursor(0, 12);
					ssd1306_WriteString(str, Font_6x8, White);

					sprintf(str, "Vy= %3.3fm/s", (float)v_base.vy);
					ssd1306_SetCursor(0, 22);
					ssd1306_WriteString(str, Font_6x8, White);

					sprintf(str, "W= %3.2frpm", (float)v_base.w);
					ssd1306_SetCursor(0, 32);
					ssd1306_WriteString(str, Font_6x8, White);

					ssd1306_UpdateScreen(); /* Copia as informações do Buffer para a tela */
					vTaskDelay(pdMS_TO_TICKS(3000)); /* 3s */
					ssd1306_Fill(Black); /* Limpa o display */
				}
				else status = 1;
			break;
			case 1:
				if(ssd1306_IICTest() == HAL_OK){
					ssd1306_Init();
					status = 0;
				} else vTaskDelay(500);
			break;
		}

	}
}
