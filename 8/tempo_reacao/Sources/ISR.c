/*!
 * @file ISR.c
 * @brief 
 * @author Thiago Pavao
 * @author Vinicius Mantovani
 * @date 19/05/2023
 */

#include "derivative.h"
#include "TPM.h"
#include "ISR.h"

tipo_estado estado = INICIO;

/**
 * @brief Rotina de servico
 */
void FTM0_IRQHandler () {
	static uint16_t counter;
	static uint8_t vezes=0;
	uint16_t valor1, valor2;

	if (TPM0_STATUS & TPM_STATUS_CH1F_MASK) {
		valor1 = TPM0_C1V;
		valor2 = TPM1_CNT + 5; 
		if (vezes == 0) {
			TPM_CH_config_especifica (1, 1, 0b0111, valor2); // set o canal TPM1_CH1 (H5Pin3, led R) com contagem em TPM1_CNT+5
			GPIOE_PSOR = (1 << 22);					// ativa o led G
			/*!
			 * ativar interrupcao do TPM0_CH2
			 */
			TPM0_C2SC |= TPM_CnSC_CHF_MASK; 	    // baixar bandeira (w1c)
			TPM0_C2SC |= TPM_CnSC_CHIE_MASK; 	    // habilitar interrupcao do TPM0_CH2
			TPM0_C2V = TPM_CnV_VAL(valor1);   		// seta o valor de match em TPM0_CH2
			counter = 0;							//  contador de ciclos intervalo
			vezes++;
		} else if (vezes == 1) {
			/*!
			 * desativa interrupcao do TPM0_CH2
			 */
			TPM0_C2SC &= ~TPM_CnSC_CHIE_MASK; 	    // desabilitar a interrupcao do TPM0_C2
			vezes = 0;
			GPIOE_PCOR = (1 << 22);			    	// apaga o led G
			TPM_CH_config_especifica (1, 1, 0b0110, valor2); //clear o canal TPM1_CH1 (H5Pin3, led R) com contagem em TPM1_CNT+5
		}

		TPM0_C1SC |= TPM_CnSC_CHF_MASK;     	// limpar solicitacao da chave     
	} else if (TPM0_STATUS & TPM_STATUS_CH2F_MASK) {
		counter++;								// incrementar counter
		GPIOE_PTOR = (1 << 22);			    	// alternar o sinal indicador (led G) de periodo do TPM0
		TPM0_C2SC |= TPM_CnSC_CHF_MASK;     	// limpar solicitacao de Output Compare    
	} 
}


