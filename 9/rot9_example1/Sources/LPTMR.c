/*
 * @file LPTMR.c
 * @brief Funcoes relativas ao modulo LPTRM
 * @date Feb 5, 2023
 * @author Wu Shin-Ting
 */

#include "derivative.h"

void LPTMR_config_basica () {
	
	SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;
}

void LPTMR_config_especifica (uint8_t prescaler, uint8_t clock_source, uint16_t count) {
	LPTMR0_PSR = (LPTMR_PSR_PRESCALE(prescaler) // 0000 is div 2
	 | LPTMR_PSR_PBYP_MASK
	 | LPTMR_PSR_PCS(clock_source)) ;	//Secao 3.8.3.3/p. 90 em Manual
	
	LPTMR0_CMR = LPTMR_CMR_COMPARE(count); //Set compare value
	
//	LPTMR0_CSR = (LPTMR_CSR_TCF_MASK // Clear any pending interrupt
//	 | LPTMR_CSR_TIE_MASK // LPT interrupt enabled
//	 | LPTMR_CSR_TPS(0) //TMR pin select
//	 |!LPTMR_CSR_TPP_MASK //TMR Pin polarity
//	 |!LPTMR_CSR_TFC_MASK // Timer Free running counter is reset whenever TMR
//							//	counter equals compare
//	 |!LPTMR_CSR_TMS_MASK //LPTMR0 as Timer
//	 );

	LPTMR0_CSR &= ~(LPTMR_CSR_TPP_MASK //TMR Pin polarity
			| LPTMR_CSR_TFC_MASK // Timer Free running counter is reset whenever TMR
								  //counter equals compare
			| LPTMR_CSR_TMS_MASK //LPTMR0 as Timer
			| LPTMR_CSR_TEN_MASK //!!! LPTMR0 disabled (para que os disparos HW acontecam 
								 //depois da conclusao de configuracao do sistema)
	 	 	);

	LPTMR0_CSR |= (LPTMR_CSR_TCF_MASK // Clear any pending interrupt
//	 | LPTMR_CSR_TIE_MASK //!!! nao eh necessario se nao precisarmos que evento tenha
						//tratamento especifico
	 | LPTMR_CSR_TPS(0) // CMP0_OUT pin select (Secao 3.8.3.2/p. 89 em Manual)
	 );
}

void LPTMR0_habilitaNVICIRQ(uint8_t prioridade) {
	
	/**
	 * Configura o modulo NVIC: habilita IRQ irq==28
	 */
	NVIC_ISER = NVIC_ISER_SETENA(1<<28);	

	/**
	 * Configura o modulo NVIC: limpa pendencias IRQ irq
	 */
	NVIC_ICPR = NVIC_ICPR_CLRPEND(1<<28);	

	/**
	 * Configura o modulo NVIC: seta prioriodade 3 para IRQ
	 * registrador NVIC_IPRn n = irq/4(Tabela 3-7/p. 53 do Manual)
	 * campo dentro do registrador: (irq%4)*8
	 */
	NVIC_IP_REG(NVIC_BASE_PTR,28/4) |= (prioridade<<6) << (28%4*8); //(Secao 3.3.2.1/p. 53 do Manual)
	
	return;

}

void LPTMR0_ativaCNR () {
	LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;
}
