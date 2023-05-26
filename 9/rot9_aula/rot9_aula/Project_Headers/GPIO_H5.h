/*!
 * @file GPIO_H5.h
 * @brief Este modulo contem declaracoes relacionadas com o modulo GPIO_H5.c 
 * @author Wu Shin Ting
 * @date 01/03/2022
 */

#ifndef GPIO_H5_H_
#define GPIO_H5_H_

/**
 * @brief Habilitar os sinais de relogio para PTE e multiplexar pinos de H5 para GPIO
 */
void GPIO_H5Pins_config_basica ();

/**
 * @brief Configurar os  pinos do header H5
 */
void GPIO_H5Pins_config_especifica();

/**
 * @brief Habilitar os sinais de relogio para PTE e multiplexar pino 2 de H5 para GPIO
 */
void GPIO_H5Pin2_config_basica ();

/**
 * @brief Configurar o  pino 2 do header H5
 */
void GPIO_H5Pin2_config_especifica();

#endif /* GPIO_ANALYZERTIMERS_H_ */
