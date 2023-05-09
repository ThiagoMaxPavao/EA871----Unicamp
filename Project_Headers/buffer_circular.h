/**
 * @file buffer_circular.h
 * @brief Arquivo-cabecalho de buffer_circular.c
 * @author Wu Shin-Ting
 * @author Thiago Pavao
 * @author Vinicius Mantovani
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
	char * dados;  	 // buffer de dados
	unsigned int tamanho;	// quantidade total de elementos
	unsigned int leitura;       // indice de leitura (tail)  
	unsigned int escrita;       // indice de escrita (head)
} BufferCirc_type;

/*!
 * @brief Inicializa um buffer circular (BC), configuarando os campos da struct e alocando memoria para os dados
 * @param[in] buffer endereco da struct que armazena o BC
 * @param[in] tamanho do vetor de dados do BC
 */
void BC_init(BufferCirc_type *buffer, unsigned int tamanho);

/*!
 * @brief Reinicia o buffer circular, limpando os dados. (A memoria nao e apagada e nem desalocada, mas e desconsiderada apos a alteracao feita nos campos "leitura" e "escrita")
 * @param[in] buffer endereco da struct que armazena o BC
 */
void BC_reset(BufferCirc_type *buffer);

/*!
 * @brief Libera a memoria que foi alocada na inicializacao do buffer. O buffer nao consegue mais armazenar dados apos a execucao.
 * @param[in] buffer endereco da struct que armazena o BC
 */
void BC_free(BufferCirc_type *buffer);

/*!
 * @brief Insere um item no buffer circular, no final da fila. Se o buffer estiver cheio, retorna um codigo de erro.
 * @param[in] buffer endereco da struct que armazena o BC
 * @param[in] item a ser inserido no buffer
 * return -1 se o buffer estava cheio. 0 caso nao ocorra erro
 */
int BC_push (BufferCirc_type *buffer, char item);

/*!
 * @brief Remove um item do buffer circular, do incio da fila. Se o buffer estiver vazio, retorna um codigo de erro.
 * @param[in] buffer endereco da struct que armazena o BC
 * @param[out] item endereco de memoria para escrever o valor que foi removido do buffer
 * return -1 se o buffer estava vazio. 0 caso nao ocorra erro
 */
int BC_pop (BufferCirc_type *buffer, char *item);

/*!
 * @brief Calcula a quantidade de itens armazenados no buffer circular
 * @param[in] buffer endereco da struct que armazena o BC
 * return numero de itens armazenados
 */
unsigned int BC_elements (BufferCirc_type *buffer);

/*!
 * @brief Verifica se o buffer circular esta cheio
 * @param[in] buffer endereco da struct que armazena o BC
 * return 1 se estiver cheio, 0 se nao estiver
 */
uint8_t BC_isFull (BufferCirc_type *buffer);

/*!
 * @brief Verifica se o buffer circular esta vazio
 * @param[in] buffer endereco da struct que armazena o BC
 * return 1 se estiver vazio, 0 se nao estiver
 */
uint8_t BC_isEmpty (BufferCirc_type *buffer);

#endif /* BUFFER_CIRCULAR_H_ */
