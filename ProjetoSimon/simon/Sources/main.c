/*
 * main implementation: use this 'C' sample to create your own application
 *
 */





#include "derivative.h" /* include peripheral declarations */
#include "buzzer.h"
#include "buffer_circular.h"
#include "IR_Receiver.h"
#include "LED_Matrix.h"
#include "MAX7219.h"
#include "SIM.h"
#include "TPM.h"
#include "util.h"
#include "ISR.h"


int main(void)
{
	/*
	 * Configurar MCGFLLCLK 20971520Hz com fonte de sinais de relogio
	 */
	SIM_setaFLLPLL (0);
	
	LEDM_init_pins();
	LEDM_init_SPI();
	
	LEDM_init_matrix(0x5);
	LEDM_clear();
	/*
	 * Configura fonte de pulsos para contadores de TPMs
	 */
	SIM_setaTPMSRC (0b01);

	TPM1TPM2_PTE20PTE22_config_basica();

	TPM_config_especifica(1, 65535, 0b1111, 0, 0, 0, 0, 0, 0b010); // periodo = 12,49980927 ms > 9 ms ok

	TPM_CH_config_especifica(1, 0, 0b0011, 0); // Input capture - ambas as bordas	

	TPM_habilitaInterrupCH(1,0); // habilita interrupcao do canal
	
	TPM_habilitaNVICIRQ(18, 3); // habilita interrupcao do TPM1
	
	//ISR_EscreveEstado(ESPERA);
	
	ISR_inicializaBC();	
	int a = 1;

	for(;;){
	
	//TESTE BUZZER:
	liga_buzzer(a);
	espera_5us(200000 * .3); // 1 segundo = 200000
	desliga_buzzer();
	
	a = (a == 9 ? 1 : a+1);
	
	//TESTE INFRA VERMELHO:
		/*case LEITURA:
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
			break;*/
	}
	
	return 0;
}
