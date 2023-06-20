/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */

#include "SIM.h"
#include "TPM.h"
#include "util.h"

void liga_buzzer(uint8_t tone) {
	int base = 500;
	int range = 2000 - base;
	float step = range / (9 - 1);
	float f = base + (tone - 1) * step;
	uint16_t mod = (uint16_t) (20971520/(32*f));
	
	TPM_config_especifica(2, mod, 0b1111, 0, 0, 0, 0, 0, 0b101);
	TPM_CH_config_especifica(2, 0, 0b1001, mod/2);
}

void desliga_buzzer() {
	TPM_CH_config_especifica(2, 0, 0b0000, 0);
}

int main(void)
{
	SIM_setaTPMSRC(1);
	SIM_setaFLLPLL(0);

	TPM2_PTE22_config_basica();

	int a = 1;

	for(;;) {
		liga_buzzer(a);
		espera_5us(200000); // 1 segundo = 200000
		desliga_buzzer();
		
		a = (a == 9 ? 1 : a+1);
	}

//	int mod = 0;
//	 
//	for(;;) {
//
//		TPM_config_especifica(2, mod, 0b1111, 0, 0, 0, 0, 0, 0b101);
//		TPM_CH_config_especifica(2, 0, 0b1001, mod/2);
//		espera_5us(200000/50);
//		desliga_buzzer();
//		
//		mod+=1;
//	}
	return 0;
}
