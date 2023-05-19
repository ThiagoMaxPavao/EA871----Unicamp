/*
 * main implementation: use this 'C' sample to create your own application
 *
 */





#include "derivative.h" /* include peripheral declarations */
#include "GPIO_latch_lcd.h"
#include "SIM.h"
#include "TPM.h"


int main(void)
{	
	/*
	 * Inicializa LCD
	 */
	GPIO_ativaConLCD();
	GPIO_initLCD();

	/*
	 * Seleciona sinais de relógio para contadores TPM
	 */
	SIM_setaTPMSRC(1);
	SIM_setaFLLPLL(0);
	
	/*
	 * Configurar sinais de relogio e pinos para TPM e seus canais
	 */
	TPM0TPM1_PTA4PTA12PTE21_config_basica();
	
	TPM_config_especifica(0, 40960, 0b1111, 0, 0, 0, 0, 0, 0b111);
	TPM_config_especifica(1, 1500, 0b1111, 0, 0, 0, 0, 0, 0b101);
	
	TPM_CH_config_especifica(1, 0, 0b0000, 0); // TPM1_CH0
	TPM_CH_config_especifica(1, 1, 0b1001, 750); // TPM1_CH1
	TPM_CH_config_especifica(0, 1, 0b0000, 0); // TPM0_CH1
	TPM_CH_config_especifica(0, 4, 0b0000, 0); // TPM0_CH4
	
	for(;;) {
		
	}

	return 0;
}
