/*
 * @file DMA.c
 * @brief Funcoes referentes aos modulos DMA e DMAMUX
 * @date Jan 29, 2023
 * @author Wu Shin-Ting
 */

#include "derivative.h"

static DMAMUX_MemMapPtr DMAMUX=DMAMUX0_BASE_PTR;

void DMAMUX_setaReq (uint8_t x, uint8_t src) {
	
	DMAMUX->CHCFG[x] &= ~DMAMUX_CHCFG_ENBL_MASK;
	
	DMAMUX->CHCFG[x] |= DMAMUX_CHCFG_SOURCE(src);   // Tabela 3-20/pag. 64 do Manual
	
	DMAMUX->CHCFG[x] |= DMAMUX_CHCFG_ENBL_MASK;
	
	return;
}

void DMAMUX_habilitaCanal (uint8_t x) {
	DMAMUX->CHCFG[x] |= DMAMUX_CHCFG_ENBL_MASK;	
}

void DMAMUX_desabilitaCanal (uint8_t x) {
	DMAMUX->CHCFG[x] &= ~DMAMUX_CHCFG_ENBL_MASK;	
}

void DMA_SAR_DAR (uint32_t *src, uint8_t sinc, uint8_t ssize, uint8_t smod,
		uint32_t *dst, uint8_t dinc, uint8_t dsize, uint8_t dmod) {
	DMA_SAR0 = (uint32_t) src;
	DMA_DAR0 = (uint32_t) dst;
	
	if (sinc) {
		DMA_DCR0 |= DMA_DCR_SINC_MASK;
	}
	DMA_DCR0 |= DMA_DCR_SSIZE(ssize);
	
	if (dinc) {
		DMA_DCR0 |= DMA_DCR_DINC_MASK;
	}
	DMA_DCR0 |= DMA_DCR_DSIZE (dsize)		;
	
	DMA_DCR0 |= DMA_DCR_SMOD (smod);
			
	DMA_DCR0 |= DMA_DCR_DMOD (dmod);
	return;
}

void DMA_setaSAR (uint32_t *src) {
	DMA_SAR0 = (uint32_t) src;
}

void DMA_setaTransfModo(uint8_t eint, uint8_t erq, uint8_t cs, uint8_t aa, uint8_t eadreq, 
		uint8_t dreq) {
	if (eint) DMA_DCR0 |= DMA_DCR_EINT_MASK;
	if (erq) DMA_DCR0 |= DMA_DCR_ERQ_MASK;
	if (cs) DMA_DCR0 |= DMA_DCR_CS_MASK;
	if (aa) DMA_DCR0 |= DMA_DCR_AA_MASK;
	if (eadreq) DMA_DCR0 |= DMA_DCR_EADREQ_MASK;
	if (dreq)  DMA_DCR0 |= DMA_DCR_ERQ_MASK;
	
	return;
}

void DMA_setaTamBloco (uint32_t tam) {
	DMA_DSR_BCR0 |= DMA_DSR_BCR_BCR(tam);
	
	return;
}

void DMA_habilitaNVICIRQ(uint8_t irq, uint8_t prioridade) {
	/**
	 * Configura o modulo NVIC: habilita IRQ irq
	 */
	NVIC_ISER = NVIC_ISER_SETENA(1<<irq);	

	/**
	 * Configura o modulo NVIC: limpa pendencias IRQ irq
	 */
	NVIC_ICPR = NVIC_ICPR_CLRPEND(1<<irq);	

	/**
	 * Configura o modulo NVIC: seta prioriodade 3 para IRQ
	 * registrador NVIC_IPRn n = irq/4(Tabela 3-7/p. 53 do Manual)
	 * campo dentro do registrador: (irq%4)*8
	 */
	NVIC_IP_REG(NVIC_BASE_PTR,irq/4) |= (prioridade<<6) << (irq%4*8); //(Secao 3.3.2.1/p. 53 do Manual)
	
	return;
}

void DMA0_MemoTPM1CH1_config_especifica (uint32_t *end, uint32_t tam) {
	// DMAMUX_setaReq (0, 54);     	//TPM0 TOF
	DMAMUX_setaReq (0, 55);     	//TPM1 TOF

	// Configurar fonte, destino e buffer circular em transferencias DMA
//	DMA_SAR_DAR (end, 1, 2, 4, (uint32_t *)&TPM1_C1V, 0, 2, 0b0000);  //incrmento de 0xfe gera valor 
	DMA_SAR_DAR (end, 1, 2, 0b0000, (uint32_t *)&TPM1_C1V, 0, 2, 0b0000); //nao esperado. Sem o buffer
																		  //circular, incremento eh correto

	// Configurar o modo de tranferencia
	DMA_setaTransfModo(1, 1, 1, 0, 1, 0); 

	// Configurar o bloco de dados numa transferencia
	DMA_setaTamBloco (tam);		// sao transferidos 128 bytes

}
