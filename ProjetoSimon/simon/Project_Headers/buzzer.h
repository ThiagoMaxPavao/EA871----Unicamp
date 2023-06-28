/**
 * @file buzzer.h
 * @brief Funcoes utilitarias para o buzzer
 * @author Thiago Pavao
 * @date 20/06/2023
 */

#ifndef BUZZER_H_
#define BUZZER_H_

/*
 * @brief Liga o buzzer com o tom de acordo com o valor passado como parametro
 * @param[in] tone do som, valor de 1 a 9, frequencia varia linearmente dependendo do valor dado.
 */
void liga_buzzer(uint8_t tone);

/*
 * @brief Desliga o buzzer, desativando o canal.
 */
void desliga_buzzer();

#endif /* BUZZER_H_ */
