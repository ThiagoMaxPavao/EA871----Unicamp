/*
 * @file ISR.c
 * @brief Funcoes relativas ao tratamento de solicitacoes de interrupcoes
 * @date Feb 5, 2023
 * @author Wu Shin-Ting
 */

#include "derivative.h"
#include "ADC.h"
#include "ISR.h"

static uint16_t valor[2];
static tipo_estado estado;

void ADC0_IRQHandler(void) {
	if( ADC0_SC1A & ADC_SC1_COCO_MASK ) {
		uint16_t leitura = ADC0_RA; // le o valor e limpa a flag automaticamente
		if (estado == AMOSTRA_VOLT) {
			valor[0] = leitura;
			ADC0_SC2 &= ~ADC_SC2_ADTRG_MASK;// chavear para trigger por software
			ADC_selecionaCanal (0b11010);	// muda para canal do sensor AN3031, causando o trigger de conversão
			estado = AMOSTRA_TEMP;
		} else if (estado == AMOSTRA_TEMP) {
			valor[1] = leitura;
			ADC0_SC2 |= ADC_SC2_ADTRG_MASK;	// chavear para trigger por hardware
			ADC_selecionaCanal (0b01001);	// seleciona o canal do header H7, nao causa trigger, pois agora esta selecionado por hardware
			estado = ATUALIZACAO;
		}
	}
}

void ISR_LeValoresAmostrados (uint16_t *v) {
	v[0] = valor[0];
	v[1] = valor[1];
	return;
}

tipo_estado ISR_LeEstado() {
	return estado;
}

void ISR_EscreveEstado(tipo_estado novo_estado) {
	estado = novo_estado;
}
