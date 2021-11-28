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
 * Velocidade dos eixos dos 3 motores, dados do GPS, velocidade escalar do robô.
 */
typedef  struct
{
	uint16_t v1;
	uint16_t v2;
	uint16_t v3;
} Velocidade_Eixo;

typedef  struct
{
	int32_t x;
	int32_t y;
	int32_t z;
} GPS;

uint16_t velocidade_escalar;
Velocidade_Eixo v_eixo;
GPS gps;

/* Variável auxiliar para a conversão dos dados em string */
char str[32];


void start_rtos(void)
{
	/* Valores para Teste */
	v_eixo.v1 = 1;
	v_eixo.v2 = 2;
	v_eixo.v3 = 3;
	gps.x = -20.004;
	gps.y = 21.544;
	gps.z = 22.000;
	velocidade_escalar = 10;

	/* Inicializa o display ???? Deve ser incializado nessa parte do código? */
	ssd1306_Init();

	/* Cria a tarefa que irá imprimir os valores em tela */
	xTaskCreate(displayTask, "displayTask", 128,   NULL,  1,  &hdisplayTask);

	/* Inicia o gerenciado de tarefas */
	vTaskStartScheduler();

	while(1); /* Execution will never reach this line */
}


void displayTask(void *arg)
{

	while(1)
	{
		/* Procedimento para imprimir os dados */
		/* Velocidade Angular */
		ssd1306_SetCursor(8, 4);
		ssd1306_WriteString("Velocidade dos eixos", Font_6x8, White);

		sprintf(str, "%u", v_eixo.v1);
		ssd1306_SetCursor(20, 14);
		ssd1306_WriteString("V1 =", Font_6x8, White);
		ssd1306_SetCursor(26, 14);
		ssd1306_WriteString(str, Font_6x8, White);

		sprintf(str, "%u", v_eixo.v2);
		ssd1306_SetCursor(20, 24);
		ssd1306_WriteString("V2 =", Font_6x8, White);
		ssd1306_SetCursor(26, 24);
		ssd1306_WriteString(str, Font_6x8, White);

		sprintf(str, "%u", v_eixo.v3);
		ssd1306_SetCursor(20, 34);
		ssd1306_WriteString("V3 =", Font_6x8, White);
		ssd1306_SetCursor(26, 34);
		ssd1306_WriteString(str, Font_6x8, White);

		ssd1306_UpdateScreen(); /* Copia as infromações do Buffer para a tela */
		vTaskDelay(pdMS_TO_TICKS(3000)); /* 3s */
		ssd1306_Fill(Black);

		/* GPS */
		ssd1306_SetCursor(8, 4);
		ssd1306_WriteString("Localização", Font_6x8, White);

		sprintf(str, "%f", (float)gps.x);
		ssd1306_SetCursor(20, 14);
		ssd1306_WriteString("X =", Font_6x8, White);
		ssd1306_SetCursor(26, 14);
		ssd1306_WriteString(str, Font_6x8, White);

		sprintf(str, "%f", (float)gps.y);
		ssd1306_SetCursor(20, 24);
		ssd1306_WriteString("Y =", Font_6x8, White);
		ssd1306_SetCursor(26, 24);
		ssd1306_WriteString(str, Font_6x8, White);

		sprintf(str, "%f", (float)gps.z);
		ssd1306_SetCursor(20, 34);
		ssd1306_WriteString("Z =", Font_6x8, White);
		ssd1306_SetCursor(26, 34);
		ssd1306_WriteString(str, Font_6x8, White);

		ssd1306_UpdateScreen(); /* Copia as infromações do Buffer para a tela */
		vTaskDelay(pdMS_TO_TICKS(3000)); /* 3s */
		ssd1306_Fill(Black); /* Limpa o display */

		/* Velocidade escalar */
		ssd1306_SetCursor(8, 4);
		ssd1306_WriteString("Velocidade do Robô", Font_6x8, White);

		sprintf(str, "%u", velocidade_escalar);
		ssd1306_SetCursor(20, 14);
		ssd1306_WriteString("v =", Font_6x8, White);
		ssd1306_SetCursor(26, 14);
		ssd1306_WriteString(str, Font_6x8, White);

		ssd1306_UpdateScreen(); /* Copia as infromações do Buffer para a tela */
		vTaskDelay(pdMS_TO_TICKS(3000)); /* 3s */
		ssd1306_Fill(Black); /* Limpa o display */

	}
}
