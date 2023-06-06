/*
 * main implementation: use this 'C' sample to create your own application
 *
 */





#include "derivative.h" /* include peripheral declarations */



int main(void)
{
	
	
	
	SIM_setaTPMSRC (0b01);
	SIM_setaFLLPLL (0); //Seta FLL como fonte de tpm
	SIM_setaOUTDIV4(0b000); //Seta divisor de frequência do clock do barramento como 1

	for(;;) {	   
		
	}
	
	return 0;
}
