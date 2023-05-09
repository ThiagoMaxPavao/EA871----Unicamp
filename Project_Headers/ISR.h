/*!
 * @file ISR.h
 * @brief Este modulo contem declaracoes relacionadas com o modulo ISR.c
 * @author Wu Shin Ting
 * @author Thiago Pavao
 * @author Vinicius Mantovani
 * @date 05/03/2022
 */

#ifndef ISR_H_
#define ISR_H_

typedef enum estado_tag {
	MENSAGEM,
	RESULTADO,
	EXPRESSAO,
	ERRO,
	COMPUTO,
	TOKENS
} tipo_estado;

/*!
 * @brief Le o estado do aplicativo
 */
tipo_estado ISR_LeEstado ();

/*!
 * @brief Atualiza o estado do aplicativo
 * @param[in] novo estado
 */
void ISR_EscreveEstado (tipo_estado novo);

/*!
 * @brief Inicializa buffers circulares para o aplicativo
 */
void ISR_inicializaBC ();

/*!
 * @brief Extrai do buffer de entrada uma linha de caracteres.
 * @param[out] string contendo a linha terminada com '\0'.
 */
void ISR_extraiString (char *string);

/*!
 * @brief Envia uma string para o Terminal.
 * @param[in] string a ser enviado para Terminal via buffer circular
 */
void ISR_EnviaString (char *string);

/*!
 * @brief Verificar se o buffer de saida esta vazio
 * @return estado vazio do buffer
 */
uint8_t ISR_BufferSaidaVazio ();

#endif /* ISR_H_ */
