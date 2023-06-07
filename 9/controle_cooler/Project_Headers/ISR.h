/*!
 * @file ISR.h
 * @brief Prototipos, macros e tipos de dados relacionados com ISR
 * @date Feb 5, 2023
 * @author Wu Shin Ting
 */

#ifndef ISR_H_
#define ISR_H_


/**
 * @brief enumeracao de estados da maquina de estados do programa
 */
typedef enum estado_tag{
	AMOSTRA_VOLT, // amostra a tensão do potenicometro
	AMOSTRA_TEMP, // amostra o sensor de temperatura
	ATUALIZACAO // mostra os resultados
}tipo_estado;

/**
 * Copia os componentes do vetor valores no vetor que entra como parametro
 * @param[out] v vetor que recebe os componentes de valores
 */
void ISR_LeValoresAmostrados (uint16_t *v);

/**
 * @brief altera o estado atual para novo_estado
 * @param[in] novo_estado estado ao qual se transitarah
 */
void ISR_EscreveEstado(tipo_estado novo_estado);

/**
 * @brief retorna o estado atual
 * @return estado variavel estatica do tipo tipo_estado
 */
tipo_estado ISR_LeEstado();

#endif /* ISR_H_ */
