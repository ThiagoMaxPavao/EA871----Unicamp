/*!
 * @file buzzer.c
 * @brief Este modulo contem funcoes uteis para o controle do buzzer
 * @author Thiago Pavao
 * @date 20/06/2023
 */

#include "derivative.h"
#include "TPM.h"
#include "util.h"

void buzzer_desliga() {
	TPM_CH_config_especifica(2, 0, 0b0000, 0);
}

void buzzer_liga_freq(int f) {
	uint16_t mod = (uint16_t) (20971520/(32*f));
	TPM_config_especifica(2, mod, 0b1111, 0, 0, 0, 0, 0, 0b101);
	TPM_CH_config_especifica(2, 0, 0b1001, mod/2);
}

void buzzer_liga_pos(int pos) {
	int min = 400;
	int max = 800;
	float f = min + (pos - 1.0) * (max - min) / (9 - 1);
	buzzer_liga_freq(f);
}

void buzzer_toca_erro() {
	buzzer_liga_freq(250);
	espera_1ms(300);
	buzzer_desliga();
}

void buzzer_toca_sucesso() {
	buzzer_liga_freq(1000);
	espera_1ms(100);
	buzzer_desliga();
	espera_1ms(100);
	buzzer_liga_freq(1000);
	espera_1ms(100);
	buzzer_desliga();
}

void buzzer_toca_perdeu() {
	buzzer_liga_pos(3);
	espera_1ms(300);
	buzzer_liga_pos(1);
	espera_1ms(300);
	buzzer_liga_pos(-1);
	espera_1ms(900);
	buzzer_desliga();
}

void buzzer_toca_inicio() {
	buzzer_liga_pos(6);
	espera_1ms(150);
	buzzer_liga_pos(8);
	espera_1ms(150);
	buzzer_liga_pos(10);
	espera_1ms(350);
	buzzer_desliga();
}
