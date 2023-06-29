/**
 * @file util.c
 * @brief Definicao das funcoes uteis do projeto
 * @date Jan 11, 2023
 * @author Wu Shin-Ting
 * @author Thiago Pavao
 * @author Vinicius Mantovani   
 */

//Inclusao dos tipos de dados padronizados pelo ISO C99
#include "stdint.h"
#include "MKL25Z4.h"

void espera_5us (uint32_t multiplos)
{
	// r0 ja tem o parametro esperado
	asm(
		"cmp	r0, #0 \n\t"
		"beq	fim \n"
		"iteracao: \n\t"
		"mov	r2, #4 \n" // carregue a qtde de iteracoes
		"laco: \n\t"
		"mov	r3, #5 \n\t" // 5 + 16 operacoes para "gastar" tempo
		"orr	r3, r0 \n\t"
		"and	r3, r0 \n\t"
		"lsr	r3, #1 \n\t"
		"asr	r3, #1 \n\t"
			
		"mov	r3, #5 \n\t"
		"orr	r3, r0 \n\t"
		"and	r3, r0 \n\t"
		"lsr	r3, #1 \n\t"
		"asr	r3, #1 \n\t"
			
		"mov	r3, #5 \n\t"
		"orr	r3, r0 \n\t"
		"and	r3, r0 \n\t"
		"lsr	r3, #1 \n\t"
		"asr	r3, #1 \n\t"
			
		"mov	r3, #5 \n\t"
		"orr	r3, r0 \n\t"
		"and	r3, r0 \n\t"
		"lsr	r3, #1 \n\t"
		"asr	r3, #1 \n\t"
			
		"asr	r3, #1 \n\t"
			
		"sub	r2, #1 \n\t"
		"cmp	r2, #0 \n\t"
		"bne	laco \n\t"  // condicional NE (nao eh igual a zero) para desvio
							//enquanto nao atingir 0, PC <- <iteracao>
		"rev	r3,r3 \n\t"
		"lsl	r3,#0 \n\t"
		"sub	r0,#1 \n\t"
		"cmp	r0, #0 \n\t"
		"bne	iteracao \n"
		"fim: \n\t"
	);
}

void espera_1ms (uint32_t multiplos){
	espera_5us(200*multiplos);
}

void espera (uint32_t valor)
{
	while (valor) valor--;
}

void UlToStr(char *s, uint32_t bin, uint8_t n)
{
    s += n;
    *s = '\0';

    while (n--)
    {
        *--s = (bin % 10) + '0';
        bin /= 10;
    }
}

uint32_t geraNumeroAleatorio(uint32_t min, uint32_t max) {
	return min + TPM1_CNT % (max-min);
}

int includes(uint8_t *vetor, uint8_t n, uint8_t x){
	int i;
	for(i = 0; i < n; i++){
		if(vetor[i] == x) return 1;
	}
	return 0;
}

void int2alg_toString(char* resultado, uint8_t entrada){
	resultado[0] = entrada/10 + '0';
	resultado[1] = entrada%10 + '0';
	resultado[2] = 0;

}

