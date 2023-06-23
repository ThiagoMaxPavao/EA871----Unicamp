/*
 * @file IR_Receiver.c
 * @brief Este arquivo contem as funcoes necessarias para a funcionalidade do sensor IR
 * @author Thiago Pavao
 * @date 20/06/2023
 */

#include "derivative.h"
#include "ISR.h"
#include "IR_Receiver.h"

uint8_t verifica_tempo(float tempo_medido, float tempo_esperado) {
	float margem = 150e-6;
	return (tempo_medido > tempo_esperado - margem) && (tempo_medido < tempo_esperado + margem);
}

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
			if(!verifica_tempo(tempo, 9e-3)) erro = 1; // sinal estranho
		}

		else if(tempos_lidos == 2) { // segundo pulso -> deve ser aproximadamente 4.5 ms
			if(!verifica_tempo(tempo, 4.5e-3)) erro = 1; // sinal estranho
		}
		
		else if(tempos_lidos == 67) { // termino de leitura com sucesso -> deve ser aproximadamente 562.5 us
			if(!verifica_tempo(tempo, 562.5e-6)) erro = 1; // sinal estranho
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
			if(!verifica_tempo(tempo, 562.5e-6)) erro = 1; // sinal estranho
		}
		
		else if((tempos_lidos%2) == 0) { // envio de bit -> deve ser aproximadamente 562.2 us (bit 0) ou 1.687 ms (bit 1)
			leitura = leitura << 1;
			if(verifica_tempo(tempo, 1.687e-3)) leitura += 1;
			else if(!verifica_tempo(tempo, 562.5e-6)) erro = 1; // sinal estranho
		}
	}
	
	return erro;
}


IR_tecla IR_decodifica(uint16_t leitura) {
	switch(leitura) {
	case 0x2010: return POWER;
	case 0x2022: return OK;
	case 0x20e0: return ARROW_LEFT;
	case 0x2060: return ARROW_RIGHT;
	case 0x2002: return ARROW_UP;
	case 0x2082: return ARROW_DOWN;
	case 0x2008: return NUMPAD0;
	case 0x2088: return NUMPAD1;
	case 0x2048: return NUMPAD2;
	case 0x20c8: return NUMPAD3;
	case 0x2028: return NUMPAD4;
	case 0x20a8: return NUMPAD5;
	case 0x2068: return NUMPAD6;
	case 0x20e8: return NUMPAD7;
	case 0x2018: return NUMPAD8;
	case 0x2098: return NUMPAD9;
	default: 	 return UNMAPPED;
	}
}

int IR_numero(IR_tecla tecla) {
	switch(tecla) {
	case NUMPAD0: return 0;
	case NUMPAD1: return 1;
	case NUMPAD2: return 2;
	case NUMPAD3: return 3;
	case NUMPAD4: return 4;
	case NUMPAD5: return 5;
	case NUMPAD6: return 6;
	case NUMPAD7: return 7;
	case NUMPAD8: return 8;
	case NUMPAD9: return 9;
	default: 	  return -1;
	}
}
