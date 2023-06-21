/*
 * main implementation: use this 'C' sample to create your own application
 *
 */


#include "derivative.h" /* include peripheral declarations */

#include "SIM.h"
#include "LED_Matrix.h"
#include "util.h"
#include "TPM.h" // import necessario apenas por que quis gerar numeros aleatorios aqui, nao eh necessario para controlar os LEDs

int main(void)
{	
	SIM_setaFLLPLL(0);
	
	LEDM_init_pins();
	LEDM_init_SPI();
	
	LEDM_init_matrix(0x5);
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
	
	uint8_t posicoes[] = {2,4,5,6,8};
	
	LEDM_acende_posicoes(posicoes, 5);
	
	espera_5us(200000);

	LEDM_clear();

	espera_5us(200000);
	
	LEDM_escreve_string("Vamo jantar!", 150000);

	LEDM_clear();

	espera_5us(200000);
	
	// As linhas a seguir iniciam o TPM1, (mesmo modulo que sera utilizado para IR_Receiver) apenas para gerar numeros aleatorios
	// Dado que os numeros aleatorios sao gerados lendo o valor no contador em um momento inesperado
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(0b01) ;
	SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK;
	TPM_config_especifica(1, 65535, 0b1111, 0, 0, 0, 0, 0, 0b010);
	
	for(;;) {
		LEDM_acende_posicao(geraNumeroAleatorio(1,10));
		espera_5us(150000);
		LEDM_desenha_grade();
		espera_5us(50000);
	}
	
	return 0;
}
