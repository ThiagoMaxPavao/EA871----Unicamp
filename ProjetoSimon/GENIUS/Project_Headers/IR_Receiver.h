/*
 * @file IR_Receiver.h
 * @brief Declaracao das funcoes de auxilio no uso do sensor receptor de IR (Infravermelho)
 * @author Thiago Pavao
 * @date 20/06/2023
 */

#ifndef IR_RECEIVER_H_
#define IR_RECEIVER_H_
//Inclusao dos tipos de dados padronizados pelo ISO C99
#include "stdint.h"

/**
 * @brief enumeracao das teclas do controle remoto
 */
typedef enum IR_tecla{
	POWER,
	OK,
	ARROW_LEFT,
	ARROW_RIGHT,
	ARROW_UP,
	ARROW_DOWN,
	NUMPAD0,
	NUMPAD1,
	NUMPAD2,
	NUMPAD3,
	NUMPAD4,
	NUMPAD5,
	NUMPAD6,
	NUMPAD7,
	NUMPAD8,
	NUMPAD9,
	UNMAPPED
}IR_tecla;

/**
 * @brief realiza a leitura do sinal recebido de acordo com os valores do buffer circular de ISR
 * @param[out] resultado endereco da variavel que deve receber o valor lido do sinal
 * @return codigo de erro, 1 se houve e 0 caso contrario
 */
int IR_Leitura (uint16_t *resultado);

/**
 * @brief Decodifica o valor capturado do controle remoto, de 16 bits, em uma tecla
 * @param[in] leitura do controle remoto
 * @return tecla equivalente, da enumeracao IR_tecla
 */
IR_tecla IR_decodifica(uint16_t leitura);

/**
 * @brief Converte uma tecla numerica para um numero, retornando um codigo de erro se a tecla enviada nao for numerica
 * @param[in] tecla que se deseja obter o valor numerico
 * @return valor da tecla, ou -1 caso a tecla nao seja numerica
 */
int IR_numero(IR_tecla tecla);

#endif /* IR_RECEIVER_H_ */
