/**
 * @file GPIO_ledRGB.h
 * @brief Prototipos, macros e tipos de dados de GPIO_ledRGB.c
 * @date Jan 11, 2023
 * @author Wu Shin TIng
 */

#ifndef GPIO_LEDRGB_H_
#define GPIO_LEDRGB_H_

//Inclusao dos tipos de dados de util
#include "util.h"

/**
 * @brief inicilizar em 1 (apagado) o pino de controle de G do LED RGB, conectadas nos pinos
 * PTB18, PTB19 e PTD1
 */
void GPIO_initLedG();

/**
 * @brief inicilizar em 1 (apagado) os pinos de controle de R, G e B do LED RGB, conectadas nos pinos
 * PTB18, PTB19 e PTD1
 */
void GPIO_initLedRGB();

/**
 * @brief seta o estado do LED G componente ledRGB
 * @param[in] estadoG ON/OFF led G
 */
void GPIO_ledG(tipo_booleano estadoG);

/**
 * @brief seta o estado do componente ledRGB
 * @param[in] estadoR ON/OFF led R
 * @param[in] estadoG ON/OFF led G
 * @param[in] estadoB ON/OFF led B
 */
void GPIO_ledRGB(tipo_booleano estadoR, tipo_booleano estadoG, tipo_booleano estadoB);

/**
 * @brief alterna o estado do canal G do LED RGB
 */
void GPIO_ledG_t();

/**
 * @brief alterna o estado do LED RGB
 */
void GPIO_ledRGB_t();

#endif /* GPIO_LEDRGB_H_ */
