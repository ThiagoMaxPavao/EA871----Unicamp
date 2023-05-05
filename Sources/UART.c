/*!
 * @file UART.c
 * @brief Este modulo contem interface dos módulos UARTx.
 * @author Wu Shin Ting
 * @date 05/03/2022
 */

#include "derivative.h"
#include "UART.h"

uint16_t UART0_SBR (uint32_t clock, uint32_t baud_rate) {
	uint16_t sbr;
	uint8_t osr;
	
	osr = (UART0_C4 & UART0_C4_OSR_MASK) >> UART0_C4_OSR_SHIFT;
	
	sbr = (uint16_t) ((clock*1.0)/(baud_rate * (osr+1)));
	
	if (((clock*1.0)/(baud_rate * (osr+1))) > sbr) sbr++;	
	
	return sbr;
}

void UART0_config_basica(uint8_t uart0src) {
	/*
	 * Ativa sinal de relogio do modulo UART0
	 */
	SIM_SCGC4 |= SIM_SCGC4_UART0_MASK ;
		
	SIM_SOPT2 &= ~SIM_SOPT2_UART0SRC(0b11); // configurar a fonte de relógio (20.971520MHz)
	SIM_SOPT2 |= SIM_SOPT2_UART0SRC(uart0src);	

	/*
	 * Configura funcao RxTx dos pinos PTA1 e PTA2 conectados ao Terminal
	 */ 
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
	
	PORTA_PCR1 |= PORT_PCR_MUX(0x2);        // UART0_RX
	PORTA_PCR2 |= PORT_PCR_MUX(0x2);        // UART0_TX
	
	return;
}

void UART0_config_especifica (uint32_t clock, uint32_t baud_rate, uint8_t osr) {

	/*
	 * Desabilitar canais RX e TX para configuracoes
	 */ 
	UART0_C2 &= ~(UART0_C2_RE_MASK |           
			UART0_C2_TE_MASK);

	// Valores fora do intervalo 4 a 32 implicam em setar 16
	UART0_C4 |= (0b11111 << UART0_C4_OSR_SHIFT);
	UART0_C4 &= ((osr << UART0_C4_OSR_SHIFT) | ~UART0_C4_OSR_MASK);

	/*
	 * Setar baud rate
	 */ 
	uint16_t sbr = UART0_SBR (clock, baud_rate);
	
	sbr = sbr & 0x1FFF;		// Mascarar os bits invalidos

	UART0_BDH &= ~UART0_BDH_SBR(0x1F);                             
	UART0_BDL &= ~UART0_BDL_SBR(0xFF);            
	UART0_BDH |= UART0_BDH_SBR(sbr >> 8);                             
	UART0_BDL |= UART0_BDL_SBR(sbr & 0x00FF);            
	
	/*
	 * Configurar o registrador C1
	 */ 
	UART0_C1 &= ~(UART0_C1_LOOPS_MASK |
			UART0_C1_DOZEEN_MASK |
			UART0_C1_RSRC_MASK |
			UART0_C1_M_MASK |                 
			UART0_C1_WAKE_MASK |
			UART0_C1_ILT_MASK |
			UART0_C1_PE_MASK |			
			UART0_C1_PT_MASK
	);

	/*
	 * Configurar o registrador C3
	 */ 
	UART0_C3 &= ~(UART0_C3_R8T9_MASK |
			UART0_C3_R9T8_MASK |
			UART0_C3_TXDIR_MASK |
			UART0_C3_TXINV_MASK 
	);

	/*
	 * Configurar MA1 
	 */
	UART0_MA1 &= ~UART0_MA1_MA(0xFF);
	
	/*
	 * Configurar MA2
	 */
	UART0_MA2 &= ~UART0_MA2_MA(0xFF);
	
	/*
	 *  w1c dos bits de estado
	 */
	UART0_S1 |= (UART0_S1_IDLE_MASK
			| UART0_S1_OR_MASK
			| UART0_S1_NF_MASK
			| UART0_S1_FE_MASK
			| UART0_S1_PF_MASK);

	/*
	 * w1c dos bits de estado
	 */ 
	UART0_S2 |= (UART0_S2_LBKDIF_MASK |
			UART0_S2_RXEDGIF_MASK);

	/*
	 * Habilitar os canais RX e TX
	 */
	UART0_C2 |= (UART0_C2_RE_MASK |         // habilita o canal receptor 
			UART0_C2_TE_MASK);	  			// habilita o canal transmissor  

	return;
}

void UART0_habilitaNVICIRQ12(uint8_t prioridade) {
	/**
	 * Configura o modulo NVIC: habilita IRQ 12
	 */
	NVIC_ISER = NVIC_ISER_SETENA(1<<12);	

	/**
	 * Configura o modulo NVIC: limpa pendencias IRQ 12
	 */
	NVIC_ICPR = NVIC_ICPR_CLRPEND(1<<12);	

	/**
	 * Configura o modulo NVIC: seta prioriodade 3 para IRQ12
	 * registrador NVIC_IPR3 (Tabela 3-7/p. 54 do Manual)
	 */
	NVIC_IPR3 |= NVIC_IP_PRI_12(prioridade<<6); 	//(Secao 3.3.2.1/p. 52 do Manual)

	
	return;
}

void UART0_habilitaInterruptRxTerminal() {
	UART0_C2 |= UART0_C2_RIE_MASK;	
}

void UART0_habilitaInterruptTxTerminal() {
	UART0_C2 |= UART0_C2_TIE_MASK;	
}

void UART0_desabilitaInterruptRxTerminal() {
	UART0_C2 &= ~UART0_C2_RIE_MASK;	
}

void UART0_desabilitaInterruptTxTerminal() {
	UART0_C2 &= ~UART0_C2_TIE_MASK;	
}
