/*
 * main implementation: use this 'C' sample to create your own application
 *
 */





#include "derivative.h" /* include peripheral declarations */
#include "GPIO_latch_lcd.h"


int main(void)
{
	GPIO_ativaConLCD();
	GPIO_initLCD();
	SIM_setaTPMSRC(1);
	SIM_setaFLLPLL(0);
	
	TPM_config_especifica(0, 40960, 0b1111, 0, 0, 0, 0, 0, 0, 0b111);
	TPM_CH_config_especifica();
	
	TPM_config_especifica(1, 1500, 0b1111, 0, 0, 0, 0, 0, 0, 0b101);
	TPM_CH_config_especifica();
	return 0;
}
