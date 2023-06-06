/*
 * @file ISR.h
 * @brief Prototipos, macros e tipos de dados relacionados com ISR
 * @date Feb 5, 2023
 * @author Wu Shin Ting
 */

#ifndef ISR_H_
#define ISR_H_

/**
 * @brief Le o estado da conversao
 * @return estado da conversao (completa == 1)
 */
uint8_t ISR_leCycleFlags();

/**
 * @brief Seta o estado da conversao
 * @param[in] estado de conversao (completa/incompleta)
 */
void ISR_escreveCycleFlags (uint8_t estado);

#endif /* ISR_H_ */
