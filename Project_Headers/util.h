/**
 * @file util.h
 * @brief Prototipos, macros e tipos de dados de util.c
 * @date Jan 11, 2023
 * @author Wu Shin-Ting
 */

#ifndef UTIL_H_
#define UTIL_H_

#define GPIO_PIN(x)  ((1)<<(x))

typedef enum boolean_tag {
	OFF,     /**< falso/apaga/desativa/liga */
	ON      /**< verdadeiro/acende/ativa/fecha */
} tipo_booleano;

/**
 * @brief espera em multiplos de 5us
 * @param[in] multiplo de 5us
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
 * @brief Converte segundos em dias
 * @param[in] n segundos
 * @param[out] dias
 * @param[out] horas
 * @param[out] minutos
 * @param[out] segundos
 * 
 * @note https://www.geeksforgeeks.org/converting-seconds-into-days-hours-minutes-and-seconds/
 */
void ConvertSectoDay(uint32_t n, uint32_t *dias, uint32_t *horas, uint32_t *minutos, uint32_t *segundos);


/**
 * @brief Converte segundos em dias no formato HH:MM:SS
 * @param[in] n segundos
 * @return string no formato HH:MM:SS
 * 
 */
char *ConvertSectoDayString (uint32_t seconds, char *string);

/**
 * @brief Converte dias em segundos
 * @param[in] dias
 * @param[in] horas
 * @param[in] minutos
 * @param[in] segundos
 * @param[out] n segundos
 * 
 */
void ConvertDaytoSec(uint32_t days, uint32_t hours, uint32_t minutes, uint32_t seconds, uint32_t *n);

/**
 * @brief Converte string de numero binario em inteiro
 * @param[in] str string a ser convertida
 * @param[in] base do numero na escrita
 * @param[out] valor retorno do numero convertido
 * @return 2 em caso de string com caracteres discordantes com a base, 0 caso nao ocorra erros
 */
uint8_t ConvertStringtoUl32 (char *str, uint8_t base, uint32_t *valor);

/**
 * @brief Calcula em que paridade se encontra o dado de 32 bits passado como parametro
 * @param[in] x numero que se deseja definir a paridade
 * @return 0 se o dado tiver um numero par de 1's e 1 se tiver um numero impar de 1's
 */
uint32_t paridade(uint32_t x);

/**
 * @brief Converte um valor inteiro em uma string de caracteres 0's e 1's, escrevendo a representacao binaria do numero com 32 bits, completando com zeros a esquerda.
 * @param[in] valor inteiro a ser convertido em string binaria
 * @param[out] bin string de retorno do numero convertido
 * @return endereco da string convertida, igual ao parametro bin
 */
char *ConvertUl32toBitString (uint32_t valor, char *bin);

#endif /* UTIL_H_ */
