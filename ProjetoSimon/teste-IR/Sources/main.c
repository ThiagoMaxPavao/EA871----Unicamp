/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */

#include "TPM.h"
#include "SIM.h"
#include "ISR.h"

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
	
	TPM_config_especifica(1, 65535, 0b1111, 0, 0, 0, 0, 0, 0b010); // periodo = 12,49980927 ms > 9 ms ok

	TPM_CH_config_especifica(1, 0, 0b0011, 0); // Input capture - ambas as bordas	

	TPM_habilitaInterrupCH(1,0); // habilita interrupcao do canal
	
	TPM_habilitaNVICIRQ(18, 3); // habilita interrupcao do TPM1
	
	ISR_EscreveEstado(ESPERA);
	
	ISR_inicializaBC();
	
	uint16_t resultado;
	
	for(;;) switch(ISR_LeEstado()) {
	case LEITURA:
		if(IR_Leitura(&resultado))	ISR_EscreveEstado(ERRO);
		else						ISR_EscreveEstado(RESULTADO);
		break;
	case RESULTADO:
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
