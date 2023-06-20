/*
 * @file IR_Receiver.c
 * @brief Este arquivo contem as funcoes necessarias para a funcionalidade do sensor IR
 * @author Thiago Pavao
 * @date 20/06/2023
 */

#define VERIFICA_TEMPO(tempo_medido, tempo_esperado) ((tempo_medido) > (tempo_esperado) - 150e-6 && (tempo_medido) < (tempo_esperado) + 150e-6)

#include "derivative.h"
#include "ISR.h"

int IR_Leitura (uint16_t *resultado) {
	float tempo;
	uint32_t leitura = 0;
	uint8_t erro = 0;
	uint8_t tempos_lidos = 0;
	uint8_t fim = 0;
	
	while(!erro && !fim) {
		while(ISR_BufferVazio());
		tempo = (ISR_BufferPop() / 65535.0) * 12.5e-3;
		
		tempos_lidos++;

		if(tempos_lidos == 1) { // primeiro pulso -> deve ser aproximadamente 9 ms
			if(!VERIFICA_TEMPO(tempo, 9e-3)) erro = 1; // sinal estranho
		}

		else if(tempos_lidos == 2) { // segundo pulso -> deve ser aproximadamente 4.5 ms
			if(!VERIFICA_TEMPO(tempo, 4.5e-3)) erro = 1; // sinal estranho
		}
		
		else if(tempos_lidos == 67) { // termino de leitura com sucesso -> deve ser aproximadamente 562.5 us
			if(!VERIFICA_TEMPO(tempo, 562.5e-6)) erro = 1; // sinal estranho
			else {
				uint8_t byte_1 		 = leitura >> (8*3),
						byte_1_check = leitura >> (8*2),
						byte_2		 = leitura >> (8*1),
						byte_2_check = leitura >> (8*0);
				
				if(byte_1 + byte_1_check != 0xFF || byte_2 + byte_2_check != 0xFF) erro = 1; 
				else {
					*resultado = (byte_1 << 8) + byte_2;
					fim = 1;
				}
			}
		}
		
		else if((tempos_lidos%2) == 1) { // preparo de bit -> deve ser aproximadamente 562.2 us
			if(!VERIFICA_TEMPO(tempo, 562.5e-6)) erro = 1; // sinal estranho
		}
		
		else if((tempos_lidos%2) == 0) { // envio de bit -> deve ser aproximadamente 562.2 us (bit 0) ou 1.687 ms (bit 1)
			leitura = leitura << 1;
			if(VERIFICA_TEMPO(tempo, 1.687e-3)) leitura += 1;
			else if(!VERIFICA_TEMPO(tempo, 562.5e-6)) erro = 1; // sinal estranho
		}
	}
	
	return erro;
}
