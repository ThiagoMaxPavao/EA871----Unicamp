/*
 * main implementation: use this 'C' sample to create your own application
 *
 */





#include "derivative.h" /* include peripheral declarations */



int main(void)
{
	/*
	 * Inicializa LCD
	 */
	GPIO_ativaConLCD();
	GPIO_initLCD();
	
	TPM1TPM2_PTB0PTB18PTB19_config_basica()();
	
	for(;;) {
		
	}
	
	return 0;
}
