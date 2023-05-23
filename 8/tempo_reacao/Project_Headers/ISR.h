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
 * @brief enum de estados do programa
 * 
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
 * @brief o resumo
 */
uint8_t funcaoExemplo();

/**
 * @brief altera o estado atual para novo_estado
 * @param[in] novo_estado estado ao qual se transitarah
 */
void ISR_EscreveEstado(tipo_estado novo_estado);

/**
 * @brief altera o ponteiro tempo para armazenar o valor do tempo_reacao estatico de ISR.c
 * @param[out] tempo ponteiro de float que armazenarah o tempo_reacao
 */
void GET_TempoReacao(float *tempo);

/**
 * @brief armazena o argumento passado em valor na variavel counter
 * @param[in] valor inteiro armazenado na variavel estatica counter 
 * @return 0
 */
uint8_t SET_Counter(uint32_t valor);

/**
 * @brief retorna o estado atual
 * @return estado variavel estatica do tipo tipo_estado
 */
tipo_estado ISR_LeEstado();


#endif /* ISR_H_ */
