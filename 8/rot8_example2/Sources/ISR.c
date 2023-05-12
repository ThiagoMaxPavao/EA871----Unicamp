/*
 * @file ISR.c
 * @brief Funcoes relacionadas com o processamento de interrupcoes ISR
 * @date Jan 27, 2023
 * @author Wu Shin Ting
 */

#include "derivative.h"

static uint8_t u8BuffSelect=3;
static uint32_t BlockSize;
static uint32_t *end_SAR;

void PORTA_IRQHandler () {
	if (PORTA_PCR4 & PORT_PCR_ISF_MASK) {
		u8BuffSelect--; 

		if (!u8BuffSelect)
		{
			u8BuffSelect = 3;
		}

		PORTA_ISFR |= PORT_ISFR_ISF (1<<4);
	}
}

void DMA0_IRQHandler () {
	DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE_MASK;
	
//	DMA_DSR_BCR0 |= DMA_DSR_BCR_BCR(64000); // tamanho do bloco a ser transferido em bytes ...
	DMA_DSR_BCR0 |= DMA_DSR_BCR_BCR(BlockSize);
	
	DMA_SAR0 = (uint32_t) end_SAR; //!!! eh necessario reiniciar o endereco inicial do bloco
	
	u8BuffSelect--;

	//!!! Manter os valores de u8BuffSelect validos
	if (!u8BuffSelect)
	{
		u8BuffSelect = 3;
	}

}

uint8_t ISR_leBufferSelect() {
	return u8BuffSelect;
}

void ISR_setaBufferSelect(uint8_t i) {
	u8BuffSelect = i;
	return;
}

void ISR_setaBlockSize(uint32_t size) {
	BlockSize = size;
	return;
}

void ISR_setaSAR (uint32_t *end) {
	end_SAR = end;
	return;
}
