/*
 * @file  filas_rtos.c
 * @date  29-November-2021
 * @brief Aplicação para comunicação em tarefas.
 */

#include <stdio.h>
#include <stdint.h>
#include "main.h"
#include "cmsis_os.h"
#include "queue.h"
#include "app_display.h"
#include "filas_rtos.h"

/* Incluir as bibliotecas do Display */
#include "ssd1306.h"
#include "ssd1306_fonts.h"

xQueueHandle vEixoQueue, gpsQueue, vBaseQueue;

void criar_filas(void) {

	/* Inicializa o display */
	ssd1306_Init();

	vBaseQueue = xQueueCreate(5, sizeof(struct vBase *));
	gpsQueue = xQueueCreate(5, sizeof(struct GPS *));
	vEixoQueue = xQueueCreate(5, sizeof(struct vEixo *));
	if ((vBaseQueue == NULL) || (gpsQueue == NULL) || (vEixoQueue == NULL))
	{
		ssd1306_SetCursor(1, 2);
		ssd1306_WriteString("Erro: QueueCreate", Font_6x8, White);
		ssd1306_UpdateScreen();
		while(1);
	}
}

int rDadosVEixo (vEixo dados){
	if (vBaseQueue != NULL)
	{
		if (xQueueReceive(vBaseQueue, &(dados), (TickType_t) 500) == pdPASS)
			return 1;
		else return 0;
	}
	else return -1;
}

int rDadosVBase (vBase dados){
	if (vEixoQueue != NULL)
	{
		if (xQueueReceive(vEixoQueue, &(dados), (TickType_t) 500) == pdPASS)
			return 1;
		else return 0;
	}
	else return -1;
}

int rDadosGps (GPS dados){
	if (gpsQueue != NULL)
	{
		if (xQueueReceive(gpsQueue, &(dados), (TickType_t) 500) == pdPASS)
			return 1;
		else return 0;
	}
	else return -1;
}

int wDadosVEixo (vEixo dados){
	if (vBaseQueue != NULL)
	{
		if (xQueueSend(vBaseQueue, &(dados), (TickType_t) 500) == pdPASS)
			return 1;
		else return 0;
	}
	else return -1;
}

int wDadosVBase (vBase dados){
	if (vEixoQueue != NULL)
	{
		if (xQueueSend(vEixoQueue, &(dados), (TickType_t) 500) == pdPASS)
			return 1;
		else return 0;
	}
	else return -1;
}

int wDadosGps (GPS dados){
	if (gpsQueue != NULL)
	{
		if (xQueueSend(gpsQueue, &(dados), (TickType_t) 500) == pdPASS)
			return 1;
		else return 0;
	}
	else return -1;
}
