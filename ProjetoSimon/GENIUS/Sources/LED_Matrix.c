/*
 * @file LED_Matrix.c
 * @brief Este arquivo contem as funcoes necessarias para comunicacao com a matriz de LEDs
 * @author Thiago Pavao
 * @author Vinicius Mantovani
 * @date 20/06/2023
 */

#include "derivative.h"
#include "util.h"
#include "font.h"
#include "MAX7219.h"


void LEDM_init_pins() {
	// Habilita o clock do modulo PORTE
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK; //habilita o clock de port E

	PORTE_PCR1 |= PORT_PCR_MUX(0x2);  //habilita PTE1 para SPI1_MOSI

	PORTE_PCR2 |= PORT_PCR_MUX(0x2);  //habilita PTE2 para SPI1_SCK

	PORTE_PCR4 |= PORT_PCR_MUX(0x2);  //habilita PTE4 para SPI1_PCS0

}

void LEDM_init_SPI(){

	SIM_SCGC4 |= SIM_SCGC4_SPI1_MASK; //Habilita o clock de SPI1
	
	SPI1_C1 |= SPI_C1_SPE_MASK; //Habilita o módulo SPI
	
	SPI1_C1 |= SPI_C1_MSTR_MASK; //Configura o módulo SPI como um dispositivo mestre
	
	SPI1_C1 |= SPI_C1_CPOL_MASK; //Seta a polaridade do clock como ativo alto
	
	SPI1_C1 |= SPI_C1_SSOE_MASK; //Configura o módulo para que, junto com o valor de MODFEN, a funcao do pino SS/ seja SS/ automatico 
	
	SPI1_C2 |= SPI_C2_MODFEN_MASK; //Determina a forma como o pino SS/ é usado, atuando, neste caso, como slave select output
	
	SPI1_C2 |= SPI_C2_BIDIROE_MASK; //Determina o uso do pino I/O de SPI como output
	
	SPI1_C2 |= SPI_C2_SPISWAI_MASK; //o clock do modulo SPI para qunado MCU entra em modo de espera 

	SPI1_BR |= SPI_BR_SPPR(0b111); //seta o divisor prescaler como 8 (divide o clock por 8)
	
}

void LEDM_escreve(uint8_t addrs, uint8_t dado){
	while(!(SPI1_S && SPI_S_SPTEF_MASK));
	SPI1_D |= SPI_D_Bits(addrs);
	
	while(!(SPI1_S && SPI_S_SPTEF_MASK));
	SPI1_D |= SPI_D_Bits(dado);
	
	espera_5us(10); // espera processamento da transmissao
}

void LEDM_init_matrix(uint8_t brilho) {
	LEDM_escreve(MAX7219_REG_DECODE_MODE_ADDRESS,
				 MAX7219_REG_DECODE_MODE_NO_DECODE);			// Sem decodificacao, pois nao sao digitos
	
	LEDM_escreve(MAX7219_REG_INTENSITY_ADDRESS,
				 brilho);										// Brightness (max = 0xf)
	
	LEDM_escreve(MAX7219_REG_SCAN_LIMIT_ADDRESS,
				 MAX7219_REG_SCAN_LIMIT_DISPLAY_DIGIT_0_TO_7);	// Scanlimit completo
	
	LEDM_escreve(MAX7219_REG_SHUTDOWN_ADDRESS,
				 MAX7219_REG_SHUTDOWN_MODE_NORMAL_OPERATION);	// normal mode (not shutdown)
	
	LEDM_escreve(MAX7219_REG_DISPLAY_TEST_ADDRESS,
				 MAX7219_REG_DISPLAY_TEST_NORMAL_OPERATION);	// normal mode (not display test)
}

void LEDM_clear() {
	uint8_t i;
	for(i = 1; i <= 8; i++)
		LEDM_escreve(i, 0x00);
}

void LEDM_escreve_char(char caractere) {
	int i;
	for(i = 1; i <= 8; i++)
		LEDM_escreve(i, font[(int)caractere][i-1]);
}

void LEDM_escreve_string(char* string, int delay) {
	char c, cant = 0;
	while((c = *(string++)) != 0) {
		if(c == cant) { // se repetir o caractere, apaga a tela e imprime denovo para mostrar que repetiu
			LEDM_clear();
			espera_1ms(200);
		}
		LEDM_escreve_char(c);
		espera_1ms(delay);
		cant = c;
	}
}

void LEDM_desenha_grade() {
	int i;
	for(i = 1; i <= 8; i++)
		LEDM_escreve(i, i%3 == 0 ? 0xff : 0x24);
}

void LEDM_acende_posicao(uint8_t posicao) {
	int i;
	for(i = 1; i <= 8; i++) {
		if(i%3 == 0) LEDM_escreve(i, 0xff); // linha horizontal da grade
		else {
			uint8_t byte = 0b00100100; // grade
			if((i-1)/3 == (posicao-1)/3) { // se posicao estiver na linha atual
				// posiciona 2 bits 1, dependendo do resto da divisao de (posicao-1) por tres
				byte |= 0b11 << ((2-(posicao-1)%3) * 3);
			}
			LEDM_escreve(i, byte);
		}
	}
}

void LEDM_acende_posicoes(uint8_t* posicoes, uint8_t num) {
	int i, j;
	for(i = 1; i <= 8; i++) {
		if(i%3 == 0) LEDM_escreve(i, 0x00); // linha horizontal da grade
		else {
			uint8_t byte = 0;
			for(j = 0; j < num; j++) {
				if((i-1)/3 == (posicoes[j]-1)/3) { // se posicao estiver na linha atual
					byte |= 0b11 << ((2-(posicoes[j]-1)%3) * 3); // mesma logica de cima
				}
			}
			LEDM_escreve(i, byte);
		}
	}
}
