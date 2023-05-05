/**
 * @file util.c
 * @brief Definicao das funcoes uteis do projeto
 * @date Jan 11, 2023
 * @author Wu Shin-Ting   
 */
//Inclusao dos tipos de dados padronizados pelo ISO C99
#include "stdint.h"
#include "string.h"

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

uint8_t charToDigit(char c) {
	if(c >= '0' && c <= '9') return c - '0';
	if(c >= 'a' && c <= 'f') return c - 'a' + 10;
	if(c >= 'A' && c <= 'F') return c - 'A' + 10;
	return -1;
}

uint8_t ConvertStringtoUl32 (char *str, uint8_t base, uint32_t *valor) {
	uint32_t aux = 0;
	uint8_t digit, i = 0;
	
	while(str[i] != 0) {
		digit = charToDigit(str[i++]);
		if(digit == -1 || digit >= base) return 2;
		aux = base * aux + digit;
	}
	
	*valor = aux;
	return 0;
}

uint32_t paridade(uint32_t x)
{
    // divide recursivamentemente o inteiro (32 bits) em dois
    // reduz pela metade e pega seu XOR até restar apenas 1 bit
 
    x = (x & 0x0000FFFF) ^ (x >> 16);
    x = (x & 0x000000FF) ^ (x >> 8);
    x = (x & 0x0000000F) ^ (x >> 4);
    x = (x & 0x00000003) ^ (x >> 2);
    x = (x & 0x00000001) ^ (x >> 1);
 
    // retorna 1 se o último bit estiver definido; caso contrário, retorne 0
    return x & 1;
}

char *ConvertUl32toBitString (uint32_t valor, char *bin) {
	char* numeros;

	uint32_t i = 0;
	
	while(valor > 0){
		
		numeros[i] = valor%2 + '0';
		valor = valor/2;
		i++;
		
	}
	
	int j;
	
	for(j = strlen(numeros) - 1; j >= 0; j--){
		bin[strlen(numeros) - j] = numeros[j];
	}
	
}
