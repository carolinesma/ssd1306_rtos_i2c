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

/* Incluir as bibliotecas do Display */
#include "ssd1306.h"
#include "ssd1306_fonts.h"

TaskHandle_t hdisplayTask;
//TaskHandle_t hi2cTask;

/* Declaração dos valores a serem mostrados no display.
 * Velocidade dos eixos dos 3 motores, dados do GPS, velocidade da base do robô.
 */
typedef  struct
{
	uint32_t w1;
	uint32_t w2;
	uint32_t w3;
} Velocidade_Eixo;

typedef  struct
{
	int32_t x;
	int32_t y;
	int32_t angulo_teta;
} GPS;

typedef  struct
{
	uint32_t vx;
	uint32_t vy;
	uint32_t w;
} Velocidade_Base;

Velocidade_Eixo v_eixo;
GPS gps;
Velocidade_Base v_base_1;
Velocidade_Base v_base_2;
Velocidade_Base v_base_3;

/* Variável auxiliar para a conversão dos dados em string */
char str[32];


void start_rtos(void)
{
	/* Valores para Teste */
	v_eixo.w1 = 1;
	v_eixo.w2 = 2;
	v_eixo.w3 = 3;
	gps.x = -20.004;
	gps.y = 21.544;
	gps.angulo_teta = 22.000;
	v_base_1.vx = 10;
	v_base_1.vy = 10;
	v_base_1.w = 10;



	/* Cria a tarefa que irá imprimir os valores em tela */
	xTaskCreate(displayTask, "displayTask", 128,   NULL,  1,  &hdisplayTask);

	/* Inicia o gerenciado de tarefas */
	vTaskStartScheduler();

	while(1); /* Execution will never reach this line */
}


void displayTask(void *arg)
{
	/* Inicializa o display */
	ssd1306_Init();

	while(1)
	{
		/* Testa a conexão do display */
		if(ssd1306_IICTest() == HAL_OK){

			/* Procedimento para imprimir os dados */
			/* Velocidade Angular */
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
			ssd1306_SetCursor(1, 2);
			ssd1306_WriteString("Velocidade da Base", Font_6x8, White);

			sprintf(str, "%3.3f %3.3f %3.3f", (float)v_base_1.vx, (float)v_base_2.vx, (float)v_base_3.vx);
			ssd1306_SetCursor(0, 12);
			ssd1306_WriteString(str, Font_6x8, White);

			sprintf(str, "%3.3f %3.3f %3.3f", (float)v_base_1.vy, (float)v_base_2.vy, (float)v_base_3.vy);
			ssd1306_SetCursor(0, 22);
			ssd1306_WriteString(str, Font_6x8, White);

			sprintf(str, "%3.2f %3.2f %3.2f", (float)v_base_1.w, (float)v_base_2.w, (float)v_base_3.w);
			ssd1306_SetCursor(0, 32);
			ssd1306_WriteString(str, Font_6x8, White);

			ssd1306_UpdateScreen(); /* Copia as informações do Buffer para a tela */
			vTaskDelay(pdMS_TO_TICKS(3000)); /* 3s */
			ssd1306_Fill(Black); /* Limpa o display */
		}
		else
		{
			if(ssd1306_IICTest() == HAL_OK)
				ssd1306_Init();
			else
				vTaskDelay(500);
		}
	}
}
