/**
 * @file ISR.h
 * @brief 
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

#endif /* ISR_H_ */
