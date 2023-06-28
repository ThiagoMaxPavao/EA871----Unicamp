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
	INICIO,
	LEITURA_INICIO,
	MOSTRA_MAPA,
	ESPERA_MAPA,
	LEITURA_MAPA,
	INTERPRETA_MAPAM,
	INICIALIZA_JOGO,
	MOSTRA_SEQUENCIA,
	ESPERA_JOGO,
	LEITURA_JOGO,
	INTERPRETA_JOGO,
	MOSTRAR_RESULTADO
}tipo_estado;

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

/**
 * @brief Inicializa o buffer circular de dados da leitura do sensor IR
 */
void ISR_inicializaBC();

/**
 * @brief Verifica se o buffer de dados esta vazio
 * @return 1 se estiver vazio e 0 caso contenha elementos
 */
uint8_t ISR_BufferVazio();

/**
 * @brief Remove o item lido a mais tempo do buffer circular
 * @return o item removido do buffer
 */
uint16_t ISR_BufferPop();

#endif /* ISR_H_ */
