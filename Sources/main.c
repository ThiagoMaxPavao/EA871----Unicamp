/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "stdlib.h"
#include "string.h"

#include "derivative.h" /* include peripheral declarations */
#include "ISR.h"
#include "UART.h"
#include "SIM.h"
#include "MCG.h"


uint8_t ExtraiString2Tokens (char *str, uint8_t *i, char **tokens){
	
	char* op = strtok (str, " ");
	char* valor = strtok (NULL, " ");
	
	if(op == NULL || valor == NULL || strtok(NULL, " ") != NULL){
		return 1;
	}
	if((strcmp(op, "I") != 0) && (strcmp(op, "P") != 0)){
		return 3;
	}
	
	return 0;
	
}

int main(void)
{
	/*
	 * Habilitar MCGFLLCLK 20MHz
	 */
	MCG_initFLL20MH();
	
	SIM_setaFLLPLL (0);    //seta FLL
	
	/*
	 * Configurar sinais de relogio e pinos
	 */
	UART0_config_basica(0b1);
	
	/*
	 * Configura o modulo UART0
	 */
	//MCGFLLCLK em 20MHz
	UART0_config_especifica (20971520, 38400, 0x0B, 2);
		
	ISR_inicializaBC();
	
	for(;;) {	
		uint8_t c;
		
		while(!(UART0_S1&UART_S1_RDRF_MASK));
		c = UART0_D;
		while(!(UART0_S1&UART_S1_TDRE_MASK) && !(UART0_S1&UART_S1_TC_MASK));
		UART0_D = c;
	}
		
	return 0;
}
