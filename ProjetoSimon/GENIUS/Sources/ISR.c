/*!
 * @file ISR.c
 * @brief Este modulo contem as rotinas de servico do aplicativo
 * @author Thiago Pavao
 * @author Vinicius Mantovani
 * @date 19/05/2023
 */

#define TAM_MAX 70 // suficiente para uma transmissão completa
#include "derivative.h"
#include "TPM.h"
#include "ISR.h"
#include "buffer_circular.h"

static tipo_estado estado;

static uint16_t CT1;
static uint8_t seguranca;
static BufferCirc_type buffer_leitura;

void FTM1_IRQHandler () {
	
	if (TPM1_STATUS & TPM_STATUS_TOF_MASK) {
			
		if(estado == LEITURA_INICIO || estado == LEITURA_MAPA || estado == LEITURA_JOGO) {
			if(seguranca) { // indica que nao houveram pulsos em um tempo longo demais, maior do que o maior esperado (9ms),
							// pois houveram dois overflows antes de qualquer borda no sinal (de subida ou descida).
				BC_push (&buffer_leitura, 0xffff);
			} else {
				seguranca = 1;
			}
		}		
		else {
			TPM1_SC &= ~TPM_SC_TOIE_MASK; // desabilitar a interrupcao por overflow
		}
		
		TPM1_SC |= TPM_SC_TOF_MASK;
	}
	
	if (TPM1_STATUS & TPM_STATUS_CH0F_MASK) {
		if(estado == INICIO || estado == ESPERA_MAPA || estado == ESPERA_JOGO) {
			CT1 = TPM1_C0V;
			BC_free (&buffer_leitura);
			seguranca = 0;
			// habilita overflow para detectar corte no sinal
			TPM1_SC |= (TPM_SC_TOF_MASK |	// resetar flag
						TPM_SC_TOIE_MASK);	// habilitar a interrupcao TOF
			switch(estado){
			case INICIO:
				estado = LEITURA_INICIO;
				break;
			case ESPERA_MAPA:
				estado = LEITURA_MAPA;
				break;
			case ESPERA_JOGO:
				estado = LEITURA_JOGO;
				break;
			default:
				break;
			}
		}
		
		else if(estado == LEITURA_INICIO || estado == LEITURA_MAPA || estado == LEITURA_JOGO) {
			uint16_t CT2 = TPM1_C0V;
			
			if(CT2 >= CT1)	BC_push (&buffer_leitura, CT2 - CT1);
			else			BC_push (&buffer_leitura, 65535 - CT1 + CT2);
			
			seguranca = 0;
			CT1 = CT2;
		}
		
		TPM1_C0SC |= TPM_CnSC_CHF_MASK;
	}
}

tipo_estado ISR_LeEstado() {
	return estado;
}

void ISR_EscreveEstado(tipo_estado novo_estado) {
	estado = novo_estado;
}

void ISR_inicializaBC() {
	BC_init (&buffer_leitura, TAM_MAX);
}

uint8_t ISR_BufferVazio() {
	return BC_isEmpty (&buffer_leitura);
}

uint16_t ISR_BufferPop() {
	uint16_t leitura;
	BC_pop (&buffer_leitura, &leitura);
	return leitura;
}
