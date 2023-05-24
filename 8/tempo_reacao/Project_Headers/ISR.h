/**
 * @file ISR.h
 * @brief Este modulo contem declaracoes relacionadas com o modulo ISR.c
 * @author Thiago Pavao
 * @author Vinicius Mantovani
 * @date 19/05/2023
 */

#ifndef ISR_H_
#define ISR_H_


/**
 * @brief enumeracao de estados da maquina de estados do programa
 */
typedef enum estado_tag{
	PREPARA_INICIO,
	INICIO,
	PREPARA_AUDITIVO,
	ESPERA_ESTIMULO_AUDITIVO,
	ESPERA_REACAO_AUDITIVA,
	LARGADA_QUEIMADA,
	RESULTADO,
	LEITURA
}tipo_estado;

/**
 * @brief altera o estado atual para novo_estado
 * @param[in] novo_estado estado ao qual se transitarah
 */
void ISR_EscreveEstado(tipo_estado novo_estado);

/**
 * @brief pegar o valor do tempo_reacao estatico de ISR.c, calculado na transicao para o estado RESULTADO
 * @return tempo armazenado em tempo_reacao de ISR.c
 */
float GET_TempoReacao();

/**
 * @brief armazena o argumento passado em valor na variavel counter
 * @param[in] valor inteiro a ser armazenado em counter 
 * @return 0
 */
uint8_t SET_Counter(uint32_t valor);

/**
 * @brief retorna o estado atual
 * @return estado variavel estatica do tipo tipo_estado
 */
tipo_estado ISR_LeEstado();

#endif /* ISR_H_ */
