/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */

#include "TPM.h"
#include "SIM.h"
#include "ISR.h"
#include "util.h"

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

int main(void)
{
	/*
	 * Configurar MCGFLLCLK 20971520Hz com fonte de sinais de relogio
	 */
	SIM_setaFLLPLL (0);

	/*
	 * Configura fonte de pulsos para contadores de TPMs
	 */
	SIM_setaTPMSRC (0b01);
	
	TPM1_PTE20_config_basica();
	TPM2_PTE22_config_basica();
	
	TPM_config_especifica(1, 65535, 0b1111, 0, 0, 0, 0, 0, 0b010); // periodo = 12,49980927 ms > 9 ms ok

	TPM_CH_config_especifica(1, 0, 0b0011, 0); // Input capture - ambas as bordas	

	TPM_habilitaInterrupCH(1,0); // habilita interrupcao do canal
	
	TPM_habilitaNVICIRQ(18, 3); // habilita interrupcao do TPM1
	
	ISR_EscreveEstado(ESPERA);
	
	ISR_inicializaBC();
	
	uint16_t resultado;
	int tone;
	
	for(;;) switch(ISR_LeEstado()) {
	case LEITURA:
		if(IR_Leitura(&resultado))	ISR_EscreveEstado(ERRO);
		else						ISR_EscreveEstado(RESULTADO);
		break;
	case RESULTADO:
		switch(resultado) {
		case 0x2088:
			tone = 1;
			break;
		case 0x2048:
			tone = 2;
			break;
		case 0x20c8:
			tone = 3;
			break;
		case 0x2028:
			tone = 4;
			break;
		case 0x20a8:
			tone = 5;
			break;
		case 0x2068:
			tone = 6;
			break;
		case 0x20e8:
			tone = 7;
			break;
		case 0x2018:
			tone = 8;
			break;
		case 0x2098:
			tone = 9;
			break;
		}
		liga_buzzer(tone);
		espera_5us(200000 * .3);
		desliga_buzzer();
		ISR_EscreveEstado(ESPERA);
		break;
	case ERRO:
		ISR_EscreveEstado(ESPERA);
		break;
	default:
		break;
	}
	
	return 0;
}

/*
 * 0x2010 - POWER
 * 0x2022 - OK
 * 0x20e0 - Seta esquerda
 * 0x2060 - Seta direita
 * 0x2002 - Seta cima
 * 0x2082 - Seta baixo
 * 0x2088 - NUMPAD1
 * 0x2048 - NUMPAD2
 * 0x20c8 - NUMPAD3
 * 0x2028 - NUMPAD4
 * 0x20a8 - NUMPAD5
 * 0x2068 - NUMPAD6
 * 0x20e8 - NUMPAD7
 * 0x2018 - NUMPAD8
 * 0x2098 - NUMPAD9
 * 0x2008 - NUMPAD0
 */
