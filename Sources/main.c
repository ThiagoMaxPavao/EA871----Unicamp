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



int main(void)
{
	SIM_setaOUTDIV4 (0b000);
	
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

	/*!
	 * Habilita IRQ
	 */
	UART0_habilitaNVICIRQ12(0);
	
	/*
	 * Inicializa os buffers circulares
	 */
	ISR_inicializaBC();

	/*!
	 * Habilita a interrupcao do Rx do UART0
	 */
//	UART0_habilitaInterruptRxTerminal();
	
	ISR_EnviaString("EA871 � LE30: teste\n\r");
	
	for(;;) {

	}
	
	return 0;
}
