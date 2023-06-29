/**
 * @file buffer_circular.h
 * @brief Arquivo-cabecalho de buffer_circular.c
 * @author Wu Shin-Ting
 * @date 21/06/2021
 */
#include "stdint.h"

#ifndef BUFFER_CIRCULAR_H_
#define BUFFER_CIRCULAR_H_

//=====================================
// Estrutura de dados: buffer circular
//=====================================
typedef struct BufferCirc_tag
{
	uint16_t * dados;  	 // buffer de dados
	unsigned int tamanho;	// quantidade total de elementos
	unsigned int leitura;       // indice de leitura (tail)  
	unsigned int escrita;       // indice de escrita (head)
} BufferCirc_type;

/**
 * @brief incializa um buffer circular
 * @param[out] buffer a ser inicializado
 * @param[in] tamanho do buffer
 */
void BC_init(BufferCirc_type *buffer, unsigned int tamanho);

/**
 * @brief reseta um buffer circular
 * @param[out] buffer a ser resetado
 */
void BC_reset(BufferCirc_type *buffer);

/**
 * @brief limpa um buffer circular e, limpa a memória
 * @param[in] buffer a ser limpo
 */
void BC_free(BufferCirc_type *buffer);

/**
 * @brief coloca um item em buffer circular
 * @param[out] buffer
 * @param[in] item a ser colocado no buffer
 */
int BC_push (BufferCirc_type *buffer, uint16_t item);

/**
 * @brief tira um item em buffer circular
 * @param[out] buffer circular
 * @param[in] item a ser tirado no buffer
 */
int BC_pop (BufferCirc_type *buffer, uint16_t *item);

/**
 * @brief conta e devolve o numero de elementos do buffer circular
 * @param[out] buffer circular
 */
unsigned int BC_elements (BufferCirc_type *buffer);

/**
 * @brief verifica se o buffer circular está cheio (retorna 1 caso esteja)
 * @param[out] buffer circular
 */
uint8_t BC_isFull (BufferCirc_type *buffer);

/**
 * @brief verifica se o buffer circular está vazio (retorna 1 caso esteja)
 * @param[out] buffer circular
 */
uint8_t BC_isEmpty (BufferCirc_type *buffer);

#endif /* BUFFER_CIRCULAR_H_ */
