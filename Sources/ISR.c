/*!
 * @file ISR.c
 * @brief Este modulo contem as rotinas de servico do aplicativo
 * @author Wu Shin Ting
 * @date 05/03/2022
 */

#include "derivative.h"
#include "buffer_circular.h"
#include "ISR.h"

#define TAM_MAX 100
static BufferCirc_type bufferE; //buffer de entrada
static BufferCirc_type bufferS;	//buffer de saida Terminal (0)
static tipo_estado estado;

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
		
		if(estado != EXPRESSAO) return; // apenas processa o caractere recebido se esta no estado de receber a expressao
		
		//Ecoar o caractere
		UART0_D = item;
		//Adicionar no buffer circular de entrada
		if (item == '\r') {
			//inserir o terminador e avisar o fim de uma string
			BC_push (&bufferE, '\0');
			while (!(UART0_S1 & UART_S1_TDRE_MASK));
			UART0_D = '\n';
			estado = TOKENS;
		} else {
			BC_push (&bufferE, item);
		}
	}
	else if (UART0_S1 & UART0_S1_TDRE_MASK) {
		/*!
		 * Interrupcao solicitada pelo canal Tx
		 */
		if (BC_isEmpty(&bufferS)){
			UART0_C2 &= ~UART0_C2_TIE_MASK;     ///< desabilita Tx quando nao ha dado para envio
		}	
		else {
			BC_pop (&bufferS, &item);
			UART0_D = item;
		}
	} 
}

tipo_estado ISR_LeEstado () {
	return estado;
}

void ISR_EscreveEstado (uint8_t novo) {
	estado = novo;
	return;
}

void ISR_inicializaBC () {
	/*!
	 * Inicializa o buffer circular de entrada
	 */
	BC_init (&bufferE, TAM_MAX);

	/*!
	 * Inicializa o buffer circulare de saida
	 */
	BC_init(&bufferS, TAM_MAX);
}

void ISR_extraiString (char *string) {
	//Entrada de uma nova string
	uint8_t i=0;
	BC_pop (&bufferE, &string[i]);
	while (string[i] != '\0') {
		BC_pop (&bufferE, &string[++i]);
	}
}

void ISR_EnviaString (char *string) {
	uint8_t i;
	
	while (BC_push( &bufferS, string[0])==-1);
	UART0_C2 |= UART0_C2_TIE_MASK;
	i=1;
	while (string[i] != '\0') {
		while (BC_push( &bufferS, string[i])==-1);
		i++;
	}
}

uint8_t ISR_BufferSaidaVazio () {
	return BC_isEmpty (&bufferS);
}
