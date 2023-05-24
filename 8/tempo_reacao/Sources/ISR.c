/*!
 * @file ISR.c
 * @brief Este modulo contem as rotinas de servico do aplicativo
 * @author Thiago Pavao
 * @author Vinicius Mantovani
 * @date 19/05/2023
 */

#include "derivative.h"
#include "TPM.h"
#include "ISR.h"
#include "util.h"

static tipo_estado estado;
static uint32_t counter;
static float tempo_reacao;

void FTM0_IRQHandler () {

	if (TPM0_STATUS & TPM_STATUS_CH1F_MASK) {
		
		if (estado == PREPARA_AUDITIVO || estado == ESPERA_ESTIMULO_AUDITIVO) {
			estado = LARGADA_QUEIMADA;
		}
		
		if (estado == ESPERA_REACAO_AUDITIVA) {
			/*
			 * Computo do tempo de reacao
			 */
			uint16_t CT1 = TPM0_C4V;
			uint16_t CT2 = TPM0_C1V;
			float max = TPM0_MOD; // definido como float para forçar divisao nao inteira
			if(CT2 >= CT1)
				tempo_reacao = (counter + ((CT2 - CT1) / max)) * 0.25;
			else
				tempo_reacao = (counter + (((max - CT1) + CT2) / max)) * 0.25;
			
			/*
			 * desabilitar buzzer, NMI e TPM0_CH4
			 */
			TPM_CH_config_especifica(1, 1, 0b0000, 0); // buzzer
			TPM_CH_config_especifica(0, 1, 0b0000, 0); // NMI
			TPM_CH_config_especifica(0, 4, 0b0000, 0); // TPM0_CH4
			
			estado = RESULTADO;
		}

		TPM0_C1SC |= TPM_CnSC_CHF_MASK;
		
	}
	if (TPM0_STATUS & TPM_STATUS_CH4F_MASK) {
		
		if (estado == ESPERA_REACAO_AUDITIVA) {
			counter++; // contagem de estouros de periodos completos enquanto espera pressionamento em NMI
		}
		
		if (estado == LEITURA) {
			counter--;
			
			if(!counter) { // terminou espera de 3s para Leitura
				estado = PREPARA_INICIO;
				TPM_CH_config_especifica(0, 4, 0b0000, 0); // desativa canal
				TPM_CH_config_especifica(1, 0, 0b0010, 0); // ativa botoeira IRQA12
			}
		}

		TPM0_C4SC |= TPM_CnSC_CHF_MASK;
	} 
}

void FTM1_IRQHandler () {

	if (TPM1_STATUS & TPM_STATUS_TOF_MASK) {

		if (estado == PREPARA_AUDITIVO || estado == ESPERA_ESTIMULO_AUDITIVO) {
			counter--;
			
			if(!counter) {
				/*
				 * Ativa buzzer
				 */
				TPM_CH_config_especifica(1, 1, 0b1001, 750);
				
				/*
				 * Configura TPM0_CH4 para contar o numero de periodos completos até que NMI seja pressionada.
				 * Isto é feito com output compare e interrupcoes sempre que a contagem se iguala ao que é atualmente, momento em que o buzzer foi ligado.
				 */
				TPM_CH_config_especifica(0, 4, 0b0100, TPM0_CNT);
				
				/*
				 * Desabilita interrupcoes por overflow, pois ja terminou o periodo aleatorio
				 */
				TPM_desabilitaInterrupTOF(1);

				// counter = 0; Nao e necessario pois ele ja e igual a zero, caso contrario nao teria entrado no if
				
				estado = ESPERA_REACAO_AUDITIVA;
			}
		}
		
		TPM1_SC |= TPM_SC_TOF_MASK;
	}
	if (TPM1_STATUS & TPM_STATUS_CH0F_MASK) {
		
		if (estado == INICIO) {
			TPM_CH_config_especifica(1, 0, 0b0000, 0); // desativa IRQA12
			counter = geraNumeroAleatorio(440, 2200);
			
			/*
			 * Habilita interrupcoes de overflow para contar o tempo de acordo com o numero aleatorio gerado
			 */
			TPM_habilitaInterrupTOF(1);

			/*
			 * Ativa botoeira NMI
			 */
			TPM_CH_config_especifica(0, 1, 0b0010, 0);
			
			estado = PREPARA_AUDITIVO;
		}

		TPM1_C0SC |= TPM_CnSC_CHF_MASK;
	}
	
}

tipo_estado ISR_LeEstado() {
	return estado;
}

void ISR_EscreveEstado(tipo_estado novo_estado) {
	estado = novo_estado;
}

float GET_TempoReacao() {
	return tempo_reacao;
}

uint8_t SET_Counter(uint32_t valor) {
	if(estado != LEITURA) return -1;
	counter = valor;
	return 0;
}
