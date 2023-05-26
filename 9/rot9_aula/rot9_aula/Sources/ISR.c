/*!
 * @file ISR.c
 * @brief Este modulo contem as rotinas de servico do aplicativo
 * @author Wu Shin Ting
 * @date 06/03/2022
 */
#include "derivative.h"
#include "ISR.h"
#include "ADC.h"

/*!
 * Declarar variaveis GLOBAIS de controle
 */
static uint8_t valor[2];		///< valor convertido do sensor LM61
static uint8_t flag=0;

/**
 * @brief Rotina de servico do IRQ 15 (ADC0)
 */
void ADC0_IRQHandler(void) {
	if( ADC0_SC1A & ADC_SC1_COCO_MASK )
	{
		if (flag == 1) {
			GPIOE_PCOR = GPIO_PIN(21);			//!< \li \l pos conversao disparada por hardware
			valor[0] = ADC0_RA;					//!< \li \l guardar o valor amostrado
			ADC0_SC2 &= ~ADC_SC2_ADTRG_MASK;	//!< \li \l chavear para trigger por software
			ADC_selecionaCanal (0b11010); 			//!< \li \l selecionar o canal do sensor AN3031
			GPIOE_PSOR = GPIO_PIN(21);			//!< trigger software enviado para ADC0
			flag = 2;
		} else if (flag == 2) {
			GPIOE_PCOR = GPIO_PIN(21);			//!< \li \l pos conversao disparada por software
			valor[1] = ADC0_RA;					//!< \li \l guardar o valor amostrado
			ADC0_SC2 |= ADC_SC2_ADTRG_MASK;		//!< \li \l chavear para trigger por hardware
			ADC_selecionaCanal (0b01001); 			//!< \li \l selecionar o canal do header H7
			flag = 3;
		}
	}
}

/**
 * @brief Rotina de servico para eventos de interrupcao gerados pelo modulo PIT
 */
void PIT_IRQHandler(void) {	
	GPIOE_PSOR = GPIO_PIN(21);			//!< \li \l trigger hardware enviado para ADC0
	GPIOE_PTOR = GPIO_PIN(22);			//!< \li \l alternar o canal 2 do analisador logico
	
	flag = 1;					//!< \li \l estado; amostrar LM61

	PIT_TFLG0 |= PIT_TFLG_TIF_MASK;     //!< \li \l w1c (limpa pendencias)
}

uint8_t ISR_LeEstado() {
	return flag;
}

void ISR_LeValoresAmostrados (uint8_t *v) {
	v[0] = valor[0];
	v[1] = valor[1];
	
	return;
}

