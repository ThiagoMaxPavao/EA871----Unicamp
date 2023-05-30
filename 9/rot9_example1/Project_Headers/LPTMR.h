/*
 * @file LPTMR.h
 * @brief Prototipos, macros e tipos de dados relacionados com LPTMR
 * @date Feb 5, 2023
 * @author Wu Shin-Ting
 */

#ifndef LPTMR_H_
#define LPTMR_H_

#include "stdint.h"

void LPTMR_config_basica ();

/**
 * @brief Configura o modulo LPTMR
 * @param[in] prescaler divisor 
 * @param[in] clock_source fonte de relogio
 * @param[in] count referencia maxima de contagem de LPTMRx_CNR
 */
void LPTMR_config_especifica (uint8_t prescaler, uint8_t clock_source, uint16_t count);

/**
 * @brief Habilita IRQ28 (LPTMR0) no NVIC 
 * @param[in] prioridade de atendimento
 */
void LPTMR0_habilitaNVICIRQ(uint8_t prioridade);

/**
 * @brief Habilita solicitacao de interrupcao do LPTMR0
 */
void LPTMR0_habilitaInterrupcao();

/**
 * @brief Ativa o modulo LPTMR0
 * 
 */
void LPTMR0_ativaCNR ();

#endif /* LPTMR_H_ */
