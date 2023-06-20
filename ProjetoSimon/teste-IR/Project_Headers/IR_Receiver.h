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
 * @brief realiza a leitura do sinal recebido de acordo com os valores do buffer circular de ISR
 * @param[in] resultado endereco da variavel que deve receber o valor lido do sinal
 * @return codigo de erro, 1 se houve e 0 caso contrario
 */
int IR_Leitura (uint16_t *resultado);

#endif /* IR_RECEIVER_H_ */
