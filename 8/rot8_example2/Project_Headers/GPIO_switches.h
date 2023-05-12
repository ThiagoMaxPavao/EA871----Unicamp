/*
 * @file GPIO_switchhes.h
 * @brief Prototipos, macros e tipos de dados de GPIO_switches
 * @date Jan 13, 2023
 * @author Wu Shin-Ting
 */

#ifndef GPIO_SWITCHES_H_
#define GPIO_SWITCHES_H_
//Inclusao dos tipos de dados padronizados pelo ISO C99
#include "stdint.h"

/**
 * @brief Inicializa botoeira NMI
 * @param[in] IRQC modo de interrupcao
 * @param[in] prioridade de atendimento
 */
void GPIO_initSwitchNMI (uint8_t IRQC, uint8_t prioridade);

/**
 * @brief Inicializa botoeira IRQA5
 * @param[in] IRQC modo de interrupcao
 * @param[in] prioridade de atendimento
 */
void GPIO_initSwitchIRQA5 (uint8_t IRQC, uint8_t prioridade);

/**
 * @brief Desativa IRQ30, sem limpar as pendencias
 */
void GPIO_desativaSwitchesNVICInterrupt ();

/**
 * @brief Reativa IRQ30, sem limpar as pendencias
 */
void GPIO_reativaSwitchesNVICInterrupt ();

/**
 * @bief Desativa IRQC da botoeira IRQA5
 */
void GPIO_desativaSwitchesIRQA5Interrupt ();

/**
 * @brief Reativa IRQC da botoeira IRQA5 no modo de interrupcao especificado
 * @param[in] IRQA5_IRQC
 */
void GPIO_reativaSwitchesIRQA5Interrupt (uint8_t IRQA5_IRQC);

#endif /* GPIO_SWITCHES_H_ */
