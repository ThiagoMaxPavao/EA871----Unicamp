/*!
 * @file ISR.c
 * @brief Este modulo contem as rotinas de servico do aplicativo
 * @author Wu Shin Ting
 * @date 05/03/2022
 */

#include "derivative.h"
#include "buffer_circular.h"
#include "ISR.h"

#define TAM_MAX 128
static uint8_t flag;
static BufferCirc_type bufferE;				//buffer de entrada
static BufferCirc_type buffer0;	//buffer de saida Terminal (0) e H5Pin2 (2)

/*!
 * @brief Rotina de serviço de UART0
 */
void UART0_IRQHandler()
{
	char item;

	if (UART0_S1 & UART0_S1_RDRF_MASK) {
		/*!
		 * Interrupcao solicitada pelo canal Rx
		 */
		item = UART0_D;

		if (flag != ESPERA && flag != MOSTRA) {
			//Liberar o canal para novas recepcoes
			return;
		}
		
		if (flag == MOSTRA) {
			// Parar o preenchimento de buffers circulares
			flag = LIBERA_BUFFER;
			return;
		} 
		
		//Ecoar o caractere
		UART0_D = item;
		//Adicionar no buffer circular de entrada
		if (item == '\r') {
			//inserir o terminador e avisar o fim de uma string
			BC_push (&bufferE, '\0');
			while (!(UART0_S1 & UART_S1_TDRE_MASK));
			UART0_D = '\n';
			flag = EXTRAI;
		} else {
			BC_push (&bufferE, item);
		}
	}
	else if (UART0_S1 & UART0_S1_TDRE_MASK) {
		/*!
		 * Interrupcao solicitada pelo canal Tx
		 */
		if (BC_isEmpty(&buffer0))
			UART0_C2 &= ~UART0_C2_TIE_MASK;     ///< desabilita Tx quando nao ha dado para envio
		else {
			BC_pop (&buffer0, &item);
			UART0_D = item;
		}
	} 
}

tipo_estado ISR_LeEstado () {
	return flag;
}

void ISR_escreveEstado (uint8_t novo) {
	flag = novo;
	return;
}

void ISR_inicializaBC () {
	/*!
	 * Inicializa um buffer circular de entrada
	 */
	BC_init (&bufferE, TAM_MAX);

	/*!
	 * Inicializa dois buffers circulares de saida
	 */
	BC_init(&buffer0, TAM_MAX);
}

void ISR_extraiString (char *string) {
	//Entrada de uma nova string
	uint8_t i=0;
	BC_pop (&bufferE, &string[i]);
	while (string[i] != '\0') {
		BC_pop (&bufferE, &string[++i]);				
	}
}

void ISR_EnviaString10x (char *string) {
	uint8_t i, j;
	
	for (j=0; j<10; j++) {
		while (BC_push( &buffer0, string[0])==-1);
		UART0_C2 |= UART0_C2_TIE_MASK;
		i=1;
		while (string[i] != '\0') {
			while (BC_push( &buffer0, string[i])==-1);
			i++;
		}
		while (BC_push( &buffer0, ' ')==-1);
	}
}

void ISR_EnviaString (char *string) {
	uint8_t i;
	
	while (BC_push( &buffer0, string[0])==-1);
	UART0_C2 |= UART0_C2_TIE_MASK;
	i=1;
	while (string[i] != '\0') {
		while (BC_push( &buffer0, string[i])==-1);
		i++;
	}
}

void ISR_Realinhamento() {
	while (BC_push( &buffer0, '\n')==-1); //newline
	while (BC_push( &buffer0, '\r')==-1); //carriage return
	UART0_C2 |= UART0_C2_TIE_MASK;
}

uint8_t ISR_BufferSaidaVazio () {
	return BC_isEmpty (&buffer0);
}
