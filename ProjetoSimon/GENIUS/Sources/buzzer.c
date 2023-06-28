/*!
 * @file buzzer.c
 * @brief Este modulo contem funcoes uteis para o controle do buzzer
 * @author Thiago Pavao
 * @date 20/06/2023
 */

#include "derivative.h"
#include "TPM.h"

void liga_buzzer(uint8_t tone) {
	int min = 400;
	int max = 1000;
	float f = min + (tone - 1.0) * (max - min) / (9 - 1);
	uint16_t mod = (uint16_t) (20971520/(32*f));
	
	TPM_config_especifica(2, mod, 0b1111, 0, 0, 0, 0, 0, 0b101);
	TPM_CH_config_especifica(2, 0, 0b1001, mod/2);
}

void desliga_buzzer() {
	TPM_CH_config_especifica(2, 0, 0b0000, 0);
}
