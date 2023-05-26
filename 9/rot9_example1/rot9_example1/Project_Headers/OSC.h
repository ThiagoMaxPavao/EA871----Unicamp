/*
 * @file OSC.h
 * @brief Prototipos, macros e tipos de dados relacionados com OSC
 * @date Feb 5, 2023
 * @author Wu Shin-Ting
 */

#ifndef OSC_H_
#define OSC_H_

/**
 * @brief Selecionar LOP 1kHz como a fonte do oscliador para o sinal ERCLK32K. 
 */
void OSC_LPO1kHz ();

/**
 * @brief Selecionar RTC_CLKIN com a fonte do oscliador para o sinal ERCLK32K.
 */
void OSC_RTCCLKIN ();

#endif /* OSC_H_ */
