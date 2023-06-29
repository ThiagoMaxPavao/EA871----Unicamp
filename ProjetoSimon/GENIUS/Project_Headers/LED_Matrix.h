/*
 * @file LED_Matrix.h
 * @brief Declaracao das funcoes de auxilio no uso da matriz de LEDs
 * @author Thiago Pavao
 * @author Vinicius Mantovani
 * @date 20/06/2023
 */

#ifndef LED_MATRIX_H_
#define LED_MATRIX_H_
//Inclusao dos tipos de dados padronizados pelo ISO C99
#include "stdint.h"

/** 
 * @brief inicializa os pinos do PORTE que serao utilizados na comunicacao com a matriz de LEDs
 * PTE1 - SPI1_MOSI, PTE2 - SPI1_SCK, PTE4 - SPI1_PCS0
 */
void LEDM_init_pins();

/** 
 * @brief inicializa o modulo SPI, para comunicacao com MAX7219, da matriz de LEDs
 */
void LEDM_init_SPI();

/** 
 * @brief Envia um dado para ser escrito em um dos registradores de MAX7219. Dois bytes sao enviados na comunicacao
 * @param[in] addrs endereco do registrador que se deseja escrever o dado
 * @param[in] dado a ser escrito no registrador
 */
void LEDM_escreve(uint8_t addrs, uint8_t dado);

/** 
 * @brief Inicializa a matriz de LEDs, configurando tipo de decodificacao, brilho e etc. Isto so pode ser feito apos a incializacao de SPI
 * @param[in] brilho dos leds, valor entre 0x0 e 0xf (inclusive)
 */
void LEDM_init_matrix(uint8_t brilho);

/** 
 * @brief Limpa a tela da matriz, apagando todos os LEDs.
 */
void LEDM_clear();

/** 
 * @brief Escreve um caractere na matriz de LEDs
 * @param[in] caractere a ser escrito. Aceita qualquer caractere, pega os simbolos do arquivo font.h
 */
void LEDM_escreve_char(char caractere);

/** 
 * @brief Escreve uma string na matriz de LEDs, piscando os caracteres na ordem com um delay entre eles
 * @param[in] string a ser escrita
 * @param[in] delay entre as letras, em multiplos de 1ms
 */
void LEDM_escreve_string(char* string, int delay);

/** 
 * @brief Desenha a grade do jogo, mostrando as separacoes entre as casas
 */
void LEDM_desenha_grade();

/** 
 * @brief Acende uma posicao na tela, especificada por um numero de 1 a 9. A grade tambem eh acessa
 * @param[in] posicao que deve ser acendida
 */
void LEDM_acende_posicao(uint8_t posicao);

/** 
 * @brief Acende varias posicoes, baseado no vetor enviado. A grade nao eh acessa junto, apenas as posicoes sao mostradas.
 * @param[in] posicoes vetor de posicoes que devem ser acessas
 * @param[in] numero de posicoes no vetor
 */
void LEDM_acende_posicoes(uint8_t* posicoes, uint8_t n);

#endif /* LED_MATRIX_H_ */
