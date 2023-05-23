/*
 * main implementation: use this 'C' sample to create your own application
 *
 */


#include <stdint.h>

#include "derivative.h" /* include peripheral declarations */
#include "GPIO_latch_lcd.h"
#include "SIM.h"
#include "TPM.h"
#include "util.h"
#include "ISR.h"


int main(void)
{	
	/*
	 * Inicializa LCD
	 */
	GPIO_ativaConLCD();
	GPIO_initLCD();

	/*
	 * Seleciona sinais de relógio para contadores TPM
	 */
	SIM_setaTPMSRC(1);
	SIM_setaFLLPLL(0);
	
	/*
	 * Configurar sinais de relogio e pinos para TPM e seus canais
	 */
	TPM0TPM1_PTA4PTA12PTE21_config_basica();
	
	/*
	 * Configura modulos TPM com periodo .25s e frequencia 440Hz para TPM0 e TPM1, respectivamente
	 */
	TPM_config_especifica(0, 40960, 0b1111, 0, 0, 0, 0, 0, 0b111);
	TPM_config_especifica(1, 1500, 0b1111, 0, 0, 0, 0, 0, 0b101);
	
	/*
	 * Desativa todos os canais que serao utilizados
	 */
	TPM_CH_config_especifica(1, 0, 0b0000, 0); // TPM1_CH0
	TPM_CH_config_especifica(1, 1, 0b0000, 0); // TPM1_CH1
	TPM_CH_config_especifica(0, 1, 0b0000, 0); // TPM0_CH1
	TPM_CH_config_especifica(0, 4, 0b0000, 0); // TPM0_CH4

	TPM_habilitaInterrupTOF(1);
	
	/*
	 * Habilita interrupcoes dos canais que necessitam, elas permaneceram habilitadas
	 * porem isto nao gera interrupcoes desnecessarias, pois os canais sao desabilitados
	 * (configurando com MS_ELS = 0b0000) quando nao sao mais necessarios.
	 */
	TPM_habilitaInterrupCH(1,0); // botoeira IRQA12
	TPM_habilitaInterrupCH(0,1); // botoeira NMI
	TPM_habilitaInterrupCH(0,4); // interrupcoes do output compare para contar numero de periodos completos (para calculo do tempo de reacao)
	
	/*
	 * Habilita IRQS dos modulos TPM0 e TPM1, TPM0 com maior prioridade para uma melhor a resposta ao pressionamento em NMI
	 */
	TPM_habilitaNVICIRQ(17, 1);
	TPM_habilitaNVICIRQ(18, 3);

	TPM_CH_config_especifica(1, 0, 0b0010, 0); // ativa botoeira IRQA12
	
	ISR_EscreveEstado(PREPARA_INICIO);
	
	char buffer_saida[25] = "";
	int tempo_reacao;
	
	for(;;) switch( ISR_LeEstado() ) {
		case PREPARA_INICIO:
			GPIO_escreveStringLCD(0, (uint8_t *) "Pressione IRQA12");
			ISR_EscreveEstado(INICIO);
			break;
		case INICIO:
			break;
		case PREPARA_AUDITIVO:
			GPIO_escreveStringLCD(0, (uint8_t *) " Teste Auditivo ");
			ISR_EscreveEstado(ESPERA_ESTIMULO_AUDITIVO);
			break;
		case ESPERA_ESTIMULO_AUDITIVO: // espera soar o buzzer
			break;
		case ESPERA_REACAO_AUDITIVA:
			break;
		case RESULTADO:
			tempo_reacao = (int) GET_TempoReacao();
			// montar string de saida
			ISR_EscreveEstado(LEITURA);
			SET_Counter(12);
			TPM_CH_config_especifica(0, 4, 0b0100, TPM0_CNT);
			break;
		case LEITURA:
			break;
		default:
			break;
	}

	return 0;
}
