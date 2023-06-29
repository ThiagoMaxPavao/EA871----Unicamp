/**
 * @file util.h
 * @brief Prototipos, macros e tipos de dados de util.c
 * @date Jan 11, 2023
 * @author Wu Shin-Ting
 * @author Thiago Pavao
 * @author Vinicius Mantovani
 */

#ifndef UTIL_H_
#define UTIL_H_

#define GPIO_PIN(x)  ((1)<<(x))

/**
 * enum de estados booleanos, uso geral
 */
typedef enum boolean_tag {
	OFF,     /**< falso/apaga/desativa/liga */
	ON      /**< verdadeiro/acende/ativa/fecha */
} tipo_booleano;

/**
 * @brief espera em multiplos de 5us
 * @param[in] multiplos de 5us
 */
void espera_5us (uint32_t multiplos);

/**
 * @brief espera em decrementos de valores ate 0
 * @param[in] valor de decremento
 */
void espera (uint32_t valor);

/**
 * @brief Converte um valor inteiro para uma string de caracteres ASCII
 * @param[out] s string
 * @param[in] bin valor inteiro
 * @param[in] n quantidade de digitos
 * 
 * @note https://community.arm.com/support-forums/f/keil-forum/17118/conversion-of-integer-to-ascii-for-display
 */
void UlToStr(char *s, uint32_t bin, uint8_t n);


/**
 * @brief espera 1 ms vezes um multiplo
 * @param[in] multiplos numero que multiplica o 1 ms
 */
void espera_1ms (uint32_t multiplos);

/**
 * @brief Converte um float em uma string
 * @param[in] n numero a ser convertido
 * @param[out] res array em que se armazena a string resultante da conversao
 * @param[in] afterpoint numero de casas decimais a serem consideradas
 */
void ftoa(float n, char *res, int afterpoint);

/**
 * @brief Gera um numero aleatorio acessando o contador de TPM1 no momento da chamada da função
 * @param[in] min valor minimo do numero a ser gerado (incluso)
 * @param[in] max valor maximo do numero (não incluso)
 */
uint32_t geraNumeroAleatorio(uint32_t min, uint32_t max);

/**
 * @brief Confere se um determinado valor esta presente no vetor
 * @param[in] vetor a ser conferido
 * @param[in] n numero de elementos do vetor
 * @param[in] x valor que se quer conferir
 */
int includes(uint8_t *vetor, uint8_t n, uint8_t x);

/**
 * @brief converte um valor inteiro em string
 * @param[out] resultado string que contem o inteiro convertido em string
 * @param[in] entrada valor inteiro a ser convertido para string
 */
void int2alg_toString(char* resultado, uint8_t entrada);

#endif /* UTIL_H_ */
