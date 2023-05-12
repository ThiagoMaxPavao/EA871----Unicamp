/**
 * @file ISR.h
 * @brief Prototipos, macros e tipos de dados relacionados com tratamento de interrupcoes
 * @date Jan 27, 2023
 * @author Wu Shin Ting
 */

#ifndef ISR_H_
#define ISR_H_

/**
 * @brief Le o buffer de dados currente (1, 2 ou 3) 
 */
uint8_t ISR_leBufferSelect();

/**
 * @brief Seta o buffer de dados
 * @param[in] i 1, 2, ou 3
 */
void ISR_setaBufferSelect(uint8_t i);

/*
 * @brief Seta o tamanho do bloco a ser transferido
 * @param[in] size tamanho em bytes
 */
void ISR_setaBlockSize(uint32_t size);

/*
 * @brief Seta o endereco inicial do bloco a ser transferico
 * @param[in] end endereco
 */
void ISR_setaSAR (uint32_t *end);

#endif /* ISR_H_ */
