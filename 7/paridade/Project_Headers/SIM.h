/**
 * @file SIM.h
 * @brief Declaracao do prototipo, macros e tipos de dados relacionados com SIM
 * @date Jan 16, 2023
 * @author Ting
 */

#ifndef SIM_H_
#define SIM_H_
//Inclusao dos tipos de dados padronizados pelo ISO C99
#include "stdint.h"

/*!
 * @brief Seta o divisor de frequencia para sinal de barramento e de Flash
 * @param[in] OUTDIV4 divisor do sinal MCGOUTCLK
 */
void SIM_setaOUTDIV4 (uint8_t OUTDIV4);

/*!
 * @brief Selecionar o modo de geracao por laco de sincronismo
 * @param[in] pll 1==PLL/0==FLL
 */
void SIM_setaFLLPLL (uint8_t pll);

#endif /* SIM_H_ */
