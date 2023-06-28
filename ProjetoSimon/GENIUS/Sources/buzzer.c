/*!
 * @file buzzer.c
 * @brief Este modulo contem funcoes uteis para o controle do buzzer
 * @author Thiago Pavao
 * @date 20/06/2023
 */

#include "derivative.h"
#include "TPM.h"

void desliga_buzzer() {
	TPM_CH_config_especifica(2, 0, 0b0000, 0);
}

void liga_buzzer_freq(int f) {
	uint16_t mod = (uint16_t) (20971520/(32*f));
	TPM_config_especifica(2, mod, 0b1111, 0, 0, 0, 0, 0, 0b101);
	TPM_CH_config_especifica(2, 0, 0b1001, mod/2);
}

void liga_buzzer_pos(int pos) {
	int min = 400;
	int max = 800;
	float f = min + (pos - 1.0) * (max - min) / (9 - 1);
	liga_buzzer_freq(f);
}

void liga_buzzer_erro() {
	liga_buzzer_freq(250);
}

liga_buzzer_sucesso() {
	espera_1ms(300);
	liga_buzzer_freq(1000);
	espera_1ms(100);
	desliga_buzzer();
	espera_1ms(100);
	liga_buzzer_freq(1000);
	espera_1ms(100);
	desliga_buzzer();
}

void toca_buzzer_perdeu() {
	liga_buzzer_pos(3);
	espera_1ms(300);
	liga_buzzer_pos(1);
	espera_1ms(300);
	liga_buzzer_pos(-1);
	espera_1ms(600);
	desliga_buzzer();
}

void toca_buzzer_inicio() {
	liga_buzzer_pos(7);
	espera_1ms(300);
	liga_buzzer_pos(9);
	espera_1ms(300);
	liga_buzzer_pos(11);
	espera_1ms(600);
	desliga_buzzer();
}
