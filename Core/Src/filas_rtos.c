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

/* Bibliotecas do Display */
#include "ssd1306.h"
#include "ssd1306_fonts.h"

/*Filas para receber os dados:
 * Velocidade do eixo: vEixoQueue
 * GPS: gpsQueue
 * Velocidade da Base: vBaseQueue
 */
xQueueHandle vEixoQueue, gpsQueue, vBaseQueue;

/* Rotina para criar e verificar as filas
 * Caso ocorra erro na criação retorna no display
 * e fica preso no while(1)
 */
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

/* Rotinas para ler informações nas filas, testa a prioridade para excluir
 * apenas os dados lidos pelas tarefas de menor prioridade
 *
 * @param vEixo dados, TickType_t tempo, UBaseType_t uxPriority
 * @return 1 se a leitura ocorreu com sucesso
 * 		   0 se ocorreu erro na leitura
 * 		   -1 se a fila não foi localizada
 */
int8_t rDadosVEixo (vEixo dados, TickType_t tempo, UBaseType_t uxPriority){
	if (vEixoQueue != NULL) {
		switch (uxPriority)
		{
		case 0:
			if (xQueueReceive(vEixoQueue, &(dados), tempo) == pdPASS)
				return 1;
			else return 0;
		break;
		default:
			if(xQueuePeek( vEixoQueue, &(dados), tempo ) == pdPASS)
				return 1;
			else return 0;
		}
	}
	else
		return -1;
}

int8_t rDadosVBase (vBase dados, TickType_t tempo, UBaseType_t uxPriority){
	if (vBaseQueue != NULL) {
		switch (uxPriority)
		{
		case 0:
			if (xQueueReceive(vBaseQueue, &(dados), tempo) == pdPASS)
				return 1;
			else return 0;
		break;
		default:
			if(xQueuePeek( vBaseQueue, &(dados), tempo ) == pdPASS)
				return 1;
			else return 0;
		}
	}
	else
		return -1;
}

int8_t rDadosGps (GPS dados, TickType_t tempo, UBaseType_t uxPriority){
	if (gpsQueue != NULL) {
		switch (uxPriority)
		{
		case 0:
			if (xQueueReceive(gpsQueue, &(dados), tempo) == pdPASS)
				return 1;
			else return 0;
		break;
		default:
			if(xQueuePeek( gpsQueue, &(dados), tempo ) == pdPASS)
				return 1;
			else return 0;
		}
	}
	else
		return -1;
}

/* Rotinas para escrever nas filas
 *
 * @param vEixo dados, TickType_t tempo
 * @return 1 se a escrita ocorreu com sucesso
 * 		   0 se ocorreu erro na escrita
 * 		   -1 se a fila não foi localizada
 */
int8_t wDadosVEixo (vEixo dados, TickType_t tempo){
	if (vBaseQueue != NULL)
	{
		if (xQueueSend(vBaseQueue, &(dados), tempo) == pdPASS)
			return 1;
		else return 0;
	}
	else return -1;
}

int8_t wDadosVBase (vBase dados, TickType_t tempo){
	if (vEixoQueue != NULL)
	{
		if (xQueueSend(vEixoQueue, &(dados), tempo) == pdPASS)
			return 1;
		else return 0;
	}
	else return -1;
}

int8_t wDadosGps (GPS dados, TickType_t tempo){
	if (gpsQueue != NULL)
	{
		if (xQueueSend(gpsQueue, &(dados), tempo) == pdPASS)
			return 1;
		else return 0;
	}
	else return -1;
}
