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
	return 0;
}
