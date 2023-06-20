/*
 * main implementation: use this 'C' sample to create your own application
 *
 */


#include "derivative.h" /* include peripheral declarations */

void initPins() {
	// Habilita o clock do modulo PORTE
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK ;    		

	PORTE_PCR1 |= PORT_PCR_MUX(0x2);  

	PORTE_PCR2 |= PORT_PCR_MUX(0x2);  

	PORTE_PCR4 |= PORT_PCR_MUX(0x2);  

}

void initSPI(){

	SPI1_C1 |= SPI_C1_SPE_MASK;
	
	SPI1_C1 |= SPI_C1_MSTR_MASK;
	
	SPI1_C1 |= SPI_C1_CPOL_MASK;
	
	SPI1_C1 |= SPI_C1_SSOE_MASK;
	
	SPI1_C2 |= SPI_C2_MODFEN_MASK;
	
	SPI1_C2 |= SPI_C2_BIDIROE_MASK;
	
	SPI1_C2 |= SPI_C2_SPISWAI_MASK;

	SPI1_BR |= SPI_BR_SPPR(0b111);
	//SPI1_C1 |= SPI_C1_SPIE_MASK; //interrupt
	
	
	//5. In the master, read SPIx_S while SPTEF = 1, and then write to the transmit data register (SPIx_D) to begin transfer

	/*if(SPI1_S && SPI_S_SPTEF_MASK ){
		SPI1_D |= SPI_D_Bits(0b10101010);
	}*/
}


void SIM_setaFLLPLL (uint8_t pll) {
	/*
	 * Selecionar o modo de geracao por laco de sincronismo
	 */
	if (pll) {
		SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK; // setar PLL
	} else {
		SIM_SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK; // setar FLL
	}
}

void escreveLED(uint8_t addrs, uint8_t dado){
	while(!(SPI1_S && SPI_S_SPTEF_MASK));
		SPI1_D |= SPI_D_Bits(addrs);
	while(!(SPI1_S && SPI_S_SPTEF_MASK));
		SPI1_D |= SPI_D_Bits(dado);
}

int main(void)
{	
	initPins();
	
	SIM_setaFLLPLL(0);
	SIM_SCGC4 |= SIM_SCGC4_SPI1_MASK;
	
	initSPI();
	
	for(;;) {	   
		
	}
	
	return 0;
}
