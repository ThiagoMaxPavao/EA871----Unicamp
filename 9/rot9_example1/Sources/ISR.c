/*
 * @file ISR.c
 * @brief Funcoes relativas ao tratamento de solicitacoes de interrupcoes
 * @date Feb 5, 2023
 * @author Wu Shin-Ting
 */

#include "derivative.h"
#include "ADC.h"

static uint16_t result0A;
static float exponentially_filtered_result=0.0;
static uint8_t cycle_flags;

void ADC0_IRQHandler () {
	if(( ADC0_SC1A & ADC_SC1_COCO_MASK ) == ADC_SC1_COCO_MASK)
	{
		result0A = ADC0_RA; // this will clear the COCO bit that is also the interrupt flag

		// Begin exponential filter code for Potentiometer setting for demonstration of filter effect
		exponentially_filtered_result += result0A;
		exponentially_filtered_result /= 2;

		cycle_flags |= ADC0A_DONE;

		GPIOB_PTOR = (1<<19);			// toggle green led instead of orenge one
		/*
		 * Caso queira ver o tempo de amostragem e conversao no analisador, basta descomentar
		 * as instrucoes abaixo
		 */
//		GPIOE_PTOR = (1<<21);
	}
}

/*
 * Caso queira ver o periodo configurado para LPTMR0 
 */
//void  LPTimer_IRQHandler () {
//	GPIOE_PTOR = (1<<21);
//	LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;
//}

uint8_t ISR_leCycleFlags() {
	return cycle_flags;
}

void ISR_escreveCycleFlags(uint8_t estado) {
	cycle_flags = estado;
}
