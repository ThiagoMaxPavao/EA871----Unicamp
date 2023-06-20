/*
 * main implementation: use this 'C' sample to create your own application
 *
 */


#include "derivative.h" /* include peripheral declarations */

#include "SIM.h"
#include "LED_Matrix.h"
#include "util.h"

int main(void)
{	
	SIM_setaFLLPLL(0);
	
	LEDM_init_pins();
	LEDM_init_SPI();
	
	LEDM_init_matrix(0x6);
	LEDM_clear();
	
	// testes:
	
	espera_5us(200000);
	
	LEDM_desenha_grade();

	espera_5us(200000);
	
	int i;
	for(i = 1; i <= 9; i++) {
		LEDM_acende_posicao(i);
	
		espera_5us(100000);
	}

	LEDM_clear();
	
	espera_5us(200000);
	
	LEDM_escreve_string("Mapas:", 150000);
	
	LEDM_clear();
	
	espera_5us(200000);
	
	uint8_t posicoes1[] = {2,4,6,8};
	uint8_t posicoes2[] = {2,4,5,6,8};
	uint8_t posicoes3[] = {1,2,3,4,5,6,7,8,9};
	
	LEDM_acende_posicoes(posicoes1, 4);
	espera_5us(150000);
	LEDM_acende_posicoes(posicoes2, 5);
	espera_5us(150000);
	LEDM_acende_posicoes(posicoes3, 9);
	espera_5us(150000);

	LEDM_clear();

	espera_5us(250000);
	
	LEDM_escreve_string("Curintia ", 150000);
	
	for(;;) {
	}
	
	return 0;
}
