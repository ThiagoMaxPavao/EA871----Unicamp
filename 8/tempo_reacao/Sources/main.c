/*
 * @brief Este projeto implementa a um programa para capturar, calcular e exibir o tempo de resposta de um usuário por meio da emissão de um som e do conseguinte pressionamento da botoeira.
 * @author Thiago Pavao
 * @author Vinicius Mantovani
 * @date 23/05/2023
 */


#include <stdint.h>

#include "derivative.h" /* include peripheral declarations */
#include "GPIO_latch_lcd.h"
#include "SIM.h"
#include "TPM.h"
#include "util.h"
#include "ISR.h"
#include "string.h"


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
	
	// Cria bitmaps 
	char c_com_cedilha[8]={0x0E,0x11,0x10,0x10,0x10,0x0E,0x04,0x08};
	char a_com_tio[] = {0x0D,0x12,0x0E,0x11,0x11,0x1F,0x11,0x00};
	
	GPIO_escreveBitmapLCD (0x01, (uint8_t *)c_com_cedilha);
	GPIO_escreveBitmapLCD (0x02, (uint8_t *)a_com_tio);
	
	char buffer_saida_1[25] = {'R', 'e', 'a', 0x01, 0x02, 'o', ' ', 'e', 'm', ' ', ' ',' ', ' ', ' ', ' ', ' ', ' ', '\0'};
	char buffer_saida_2[25] = "";
	float tempo_racao;
	
	for(;;) switch( ISR_LeEstado() ) {
		case PREPARA_INICIO:
			GPIO_escreveStringLCD(0, (uint8_t *) "Pressione IRQA12");
			GPIO_escreveStringLCD(0x42, (uint8_t *) "                   ");
			ISR_EscreveEstado(INICIO);
			break;
		case INICIO:
			break;
		case LARGADA_QUEIMADA:
			GPIO_escreveStringLCD(0, (uint8_t *) "Largada Queimada");
		    ISR_EscreveEstado(LEITURA);
		    SET_Counter(12);
			TPM_CH_config_especifica(0, 4, 0b0100, TPM0_CNT);
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
			GET_TempoReacao(&tempo_racao);			
			ftoa(tempo_racao, buffer_saida_2, 2);
			strcat(buffer_saida_2, " segundos");
			GPIO_escreveStringLCD(0x1, (uint8_t *) buffer_saida_1);
			GPIO_escreveStringLCD(0x42, (uint8_t *) buffer_saida_2);
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
