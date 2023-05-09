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

	char *errorMsgs[] = {
		"Quantidade de tokens incorreta.\n\r",
		"Valor inteiro invalido.\n\r",
		"Tipo de paridade incorreto.\n\r"
	};
	char *par_impar[] = {
		"par",
		"impar"
	};
	
	char *tokens[2];
	char string[100];
	uint32_t numero;
	uint8_t paridade_atual;
	uint8_t paridade_requisitada;
	uint8_t digito;
	uint8_t erro;
	
	for(;;) switch(ISR_LeEstado ()) {
		case MENSAGEM:
			ISR_EnviaString("Entre <P/p/I/i> <tipo><valor> (tipo: b/B/h/H)\n\r");
			ISR_EscreveEstado(EXPRESSAO);
			break;
			
		case TOKENS:
			ISR_extraiString(string);
			erro = extraiString2Tokens(string, tokens);
			
			char *strNumero; // endereco do primeiro caractere do numero na string. Varia se ouver a letra indicadora da base no inicio do token.
			int base;
			if(erro == 0) { // caso não tenha tido erro na extracao dos tokens (quantidade de tokens e tipo de paridade corretos)
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
			else 		  ISR_EscreveEstado(COMPUTO);
			break;
			
		case ERRO:
			ISR_EnviaString(errorMsgs[erro - 1]);
			ISR_EscreveEstado(EXPRESSAO);
			break;

		case COMPUTO:
			paridade_atual = paridade(numero);
			digito = paridade_requisitada ^ paridade_atual; // se a paridade atual e a requisitada forem iguais, nao e necessario bit de paridade (=0)
															// caso contrario, precisa de bit de paridade (=1); Isto e feito pela operacao XOR
			ISR_EscreveEstado(RESULTADO);
			break;

		case RESULTADO:
			ConvertUl32toBitString(numero, string); // modifica a string escrevendo o numero e inserindo um '\0' no final dele
			strcat(string, " tem uma quantidade ");
			strcat(string, par_impar[paridade_atual]);
			strcat(string, " de 1. O digito de paridade ");
			strcat(string, par_impar[paridade_requisitada]);
			strcat(string, ": ");
			
			char digito_str[] = " ";
			digito_str[0] = digito + '0';
			
			strcat(string, digito_str);
			strcat(string, "\n\r");
			
			ISR_EnviaString(string);
			ISR_EscreveEstado(MENSAGEM);
			break;
		
		default:
			break;
	}
		
	return 0;
}
