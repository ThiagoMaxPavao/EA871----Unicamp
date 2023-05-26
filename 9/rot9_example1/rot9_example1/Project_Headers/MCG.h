/*
 * @file MCG.h
 * @brief Prototipos, macros e tipos de dados relativos a MCG.
 * @date Jan 23, 2023
 * @author Wu Shin Ting
 */

#ifndef MCG_H_
#define MCG_H_

/**
 * @brief Configurar MCG para gerar MCGFLLCLK a frequencia de 48MHz
 */
void MCG_initFLL48MH();

/**
 * @brief Configurar MCG para gerar MCGIRCLK (estatico) a frequencia de 4MHz (unica no modo de
 * operacao de baixo consumo - VLPS)
 */
void MCG_initIR4MHFLL20MH();

#endif /* MCG_H_ */
