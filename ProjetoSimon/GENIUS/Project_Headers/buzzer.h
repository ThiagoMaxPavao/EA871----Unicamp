/**
 * @file buzzer.h
 * @brief Funcoes utilitarias para o buzzer
 * @author Thiago Pavao
 * @date 20/06/2023
 */

#ifndef BUZZER_H_
#define BUZZER_H_

/**
 * @brief Liga o buzzer com a frequencia passada como parametro
 * @param[in] f frequencia do som que sera emitido
 */
void buzzer_liga_freq(int f);

/**
 * @brief Desliga o buzzer, desativando o canal.
 */
void buzzer_desliga();

/**
 * @brief Liga o buzzer com o tom respectivo a posicao no mapa (de 1 a 9)
 * @param[in] pos posicao no mapa cuja frequencia referente sera tocada
 */
void buzzer_liga_pos(int pos);

/**
 * @brief toca o som que indica erro do usuario
 */
void buzzer_toca_erro();

/**
 * @brief toca o som que indica sucesso/confirmacao do usuario
 */
void buzzer_toca_sucesso();

/**
 * @brief toca o som que indica perda do usuario
 */
void buzzer_toca_perdeu();

/**
 * @brief toca o som que indica inicio da escolha de mapas
 */
void buzzer_toca_inicio();

#endif /* BUZZER_H_ */
