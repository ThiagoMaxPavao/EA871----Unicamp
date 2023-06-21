/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */

#include "TPM.h"
#include "SIM.h"
#include "ISR.h"
#include "IR_Receiver.h"

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
	IR_tecla tecla;
	int numero;
	
	for(;;) switch(ISR_LeEstado()) {
	case LEITURA:
		if(IR_Leitura(&resultado))	ISR_EscreveEstado(ERRO);
		else						ISR_EscreveEstado(RESULTADO);
		break;
	case RESULTADO:
		tecla = IR_decodifica(resultado);
		numero = IR_numero(tecla);
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
