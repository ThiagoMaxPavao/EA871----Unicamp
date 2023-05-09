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
#include "util.h"

uint8_t extraiString2Tokens (char *str, char **tokens){
	
	char* op = strtok(str, " ");
	char* valor = strtok(NULL, " ");
	tokens[0] = op;
	tokens[1] = valor;
	
	if(op == NULL || valor == NULL || strtok(NULL, " ") != NULL){
		return 1;
	}
	if((strcmp(op, "I") != 0) && (strcmp(op, "P") != 0) && (strcmp(op, "p") != 0) && (strcmp(op, "i") != 0)){
		return 3;
	}
	
	return 0;
	
}

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
	UART0_habilitaInterruptRxTerminal();
	
	ISR_EscreveEstado(MENSAGEM);
	
	char *tokens[2];
	char string[100];
	uint32_t numero;
	uint8_t paridade_atual;
	uint8_t digito;
	
	for(;;) {
		if(ISR_LeEstado() == MENSAGEM){
			ISR_EnviaString("Entre <P/p/I/i> <tipo><valor> (tipo: b/B/h/H)\n\r");
			ISR_EscreveEstado(EXPRESSAO);
		} else if(ISR_LeEstado() == TOKENS){
			ISR_extraiString(string);
			int erro = extraiString2Tokens(string, tokens);
			
			if(erro == 0) {
				int base = 10;
				if(tokens[1][0] == 'B' || tokens[1][0] == 'b') {
					tokens[1]++;
					base = 2;
				}
				if(tokens[1][0] == 'H' || tokens[1][0] == 'h') {
					tokens[1]++;
					base = 16;
				}
				
				erro = ConvertStringtoUl32(tokens[1], base, &numero);
			}
			
			if(erro == 1) ISR_EnviaString("Quantidade de tokens incorreta.\n\r");
			if(erro == 2) ISR_EnviaString("Valor inteiro invalido.\n\r");
			if(erro == 3) ISR_EnviaString("Tipo de paridade incorreto.\n\r");
			
			if(erro != 0) ISR_EscreveEstado(ERRO);
			else ISR_EscreveEstado(COMPUTO);
			
		} else if(estado == COMPUTO){
			
			paridade_atual = paridade(numero);
			digito = (tokens[1][0] == 'i' || tokens[1][0] == 'I') ? 1-paridade_atual : paridade_atual;
		}
		
		
	}
		
	return 0;
}
