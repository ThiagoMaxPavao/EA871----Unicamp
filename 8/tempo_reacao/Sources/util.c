/**
 * @file util.c
 * @brief Definicao das funcoes uteis do projeto
 * @date Jan 11, 2023
 * @author Wu Shin-Ting   
 */
//Inclusao dos tipos de dados padronizados pelo ISO C99
#include "stdint.h"
#include "MKL25Z4.h"

void espera_5us (uint32_t multiplos)
{
		__asm__ (
			// substitua pelo seu codigo do roteiro 3
			"NOP \n\t"
		);
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

void ConvertSectoDay(uint32_t n, uint32_t *days, uint32_t *hours, uint32_t *minutes, uint32_t *seconds)
{
    *days = n / (24 * 3600);
 
    n = n % (24 * 3600);
    *hours = n / 3600;
 
    n %= 3600;
    *minutes = n / 60 ;
 
    n %= 60;
    *seconds = n;     
}

char *ConvertSectoDayString (uint32_t seconds, char *string)
{
	uint32_t dd, hh, mm, ss;

	ConvertSectoDay (seconds, &dd, &hh, &mm, &ss);

	string[2] = string[5] = ':';    // inserir os espacadores
	string[8] = '\0';				// terminador

	if (hh > 23) {
		//!< horario invalido: FF:FF:FF
		string[0] = string[1] = string[3] = string[4] = string[6] = string[7] = 'F';
	}

	string[0] = (hh < 10)? '0': (hh/10)+'0';
	string[1] = hh%10+'0';

	string[3] = (mm < 10)? '0': (mm/10)+'0';
	string[4] = mm%10+'0';

	string[6] = (ss < 10)? '0': (ss/10)+'0';
	string[7] = ss%10+'0';

	return string;
}

void ConvertDaytoSec(uint32_t days, uint32_t hours, uint32_t minutes, uint32_t seconds, uint32_t *n)
{
	*n = days*86400+hours*3600+minutes*60+seconds;
}

void ftoa(float n, char *res, int afterpoint){
	int aux = 0;
	int aux1 = 0;
	char string_invertida[100];
	char dec_invertido[100];
	aux = n;
	aux1 = n;
	int cont = 0;
	int cont_dec = 0;
	
	while(aux > 0){
		
		string_invertida[cont] = aux%10;
		cont++;
		aux = aux/10;	
	}
	
	int i = 0;
	for(i = 0; i < afterpoint; i++){
		n = n*10;
		aux1 = aux1*10;
	}
	aux1 = n - aux1;
	
	while(aux1 > 0){
		dec_invertido[cont_dec] = aux1%10;
		cont_dec++;
		aux1 = aux1/10;
	}
	
	aux = 0;
	while(cont > 0){
		res[aux] = string_invertida[cont - 1] + 48;
		aux++;
		cont--;
	}
	
	res[aux] = '.';
	
	aux1 = 1;
	while(afterpoint > 0){
		res[aux1 + aux] = dec_invertido[afterpoint - 1] + 48;
		aux1++;
		afterpoint--;
	}
	res[aux + aux1] = '\0';
}

uint32_t geraNumeroAleatorio(uint32_t min, uint32_t max) {
	return min + TPM0_CNT % (max-min);
}

