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
	uint8_t paridade_requisitada;
	uint8_t digito;
	uint8_t erro;
	
	for(;;) {
		if(ISR_LeEstado() == MENSAGEM){
			ISR_EnviaString("Entre <P/p/I/i> <tipo><valor> (tipo: b/B/h/H)\n\r");
			ISR_EscreveEstado(EXPRESSAO);
		}
		else if(ISR_LeEstado() == TOKENS){
			ISR_extraiString(string);
			erro = extraiString2Tokens(string, tokens);
			
			char *strNumero;
			int base;
			if(erro == 0) {
				paridade_requisitada = (tokens[0][0] == 'I' || tokens[0][0] == 'i') ? 1 : 0;
				if(tokens[1][0] == 'B' || tokens[1][0] == 'b') {
					strNumero = tokens[1] + 1;
					base = 2;
				}
				else if(tokens[1][0] == 'H' || tokens[1][0] == 'h') {
					strNumero = tokens[1] + 1;
					base = 16;
				}
				else {
					strNumero = tokens[1];
					base = 10;
				}
				
				erro = ConvertStringtoUl32(strNumero, base, &numero);
			}
			
			if(erro != 0) ISR_EscreveEstado(ERRO);
			else ISR_EscreveEstado(COMPUTO);
			
		}
		else if(ISR_LeEstado() == ERRO){
			
			char *errorMsgs[] = {
				"Quantidade de tokens incorreta.\n\r",
				"Valor inteiro invalido.\n\r",
				"Tipo de paridade incorreto.\n\r"
			};
			
			ISR_EnviaString(errorMsgs[erro - 1]);
			ISR_EscreveEstado(EXPRESSAO);
		}
		else if(ISR_LeEstado() == COMPUTO){
			paridade_atual = paridade(numero);
			digito = paridade_requisitada ^ paridade_atual;
			ISR_EscreveEstado(RESULTADO);
		}
		else if(ISR_LeEstado() == RESULTADO){
			
			char *par_impar[] = {
				"par",
				"impar"
			};
			
			char resultado[100];
			
			ConvertUl32toBitString(numero, resultado);
			strcat(resultado, " tem uma quantidade ");
			strcat(resultado, par_impar[paridade_atual]);
			strcat(resultado, " de 1. O digito de paridade ");
			strcat(resultado, par_impar[paridade_requisitada]);
			strcat(resultado, ": ");
			
			char digito_str[] = " ";
			digito_str[0] = digito + '0';
			
			strcat(resultado, digito_str);
			strcat(resultado, "\n\r");
			
			ISR_EnviaString(resultado);
			ISR_EscreveEstado(MENSAGEM);
			
			
		}
		
	}
		
	return 0;
}
