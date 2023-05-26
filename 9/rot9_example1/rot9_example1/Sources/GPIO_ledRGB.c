/**
 * @file GPIO_ledRGB.c
 * @brief Implementacao das funcoes declaradas em GPIO_ledRGB.h
 * @author Wu Shin Ting
 * @date 06/09/2020
 * 
 */

//Declaracao de macros e funcoes do componente GPIO_ledRGB
#include "derivative.h"
#include "GPIO_ledRGB.h"

void GPIO_initLedRGB() {
	// Modulo SIM: habilita o clock dos modulos PORTB e PORTD
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK |
			SIM_SCGC5_PORTD_MASK; 
	
	// Modulo PORT: configure o pino PTB18 para GPIO (vermelho)
	PORTB_PCR18 &= ~PORT_PCR_MUX (0b111); // mascarar os bits 8, 9 e 10
	PORTB_PCR18 |= PORT_PCR_MUX (0b001); // mascarar os bits 8, 9 e 10
	
	// Modulo PORT: configure o pino PTB19 para GPIO (verde)
	//	((struct PORT_MemMap volatile *)0x4004A000u)->PCR[19] &= ~PORT_PCR_MUX (0b111); // mascarar os bits 8, 9 e 10
	//	((PORT_MemMapPtr)0x4004A000u)->PCR[19] |= PORT_PCR_MUX (0b001); // mascarar os bits 8, 9 e 10
	PORTB_PCR19 &= ~PORT_PCR_MUX (0b111); // mascarar os bits 8, 9 e 10
	PORTB_PCR19 |= PORT_PCR_MUX (0b001); // mascarar os bits 8, 9 e 10

	// Modulo PORT: configure o pino PTD1 para GPIO (azul)
	PORTD_PCR1 &= ~PORT_PCR_MUX (0b111); // mascarar os bits 8, 9 e 10
	PORTD_PCR1 |= PORT_PCR_MUX (0b001); // mascarar os bits 8, 9 e 10

	// Modulo GPIO: configure o sentido do sinal nos pinos PTB18, PTB19 e PTD1
	GPIOB_PDDR |= GPIO_PDDR_PDD(0b11<<18);    // Seta pinos 18 e 19 do PORTB como saida 	
	GPIOD_PDDR |= GPIO_PDDR_PDD(0b1<<1);      // Seta pino 1 do PORTD como saida 	
	
	// Modulo GPIO: inicializar PTB18, PTB19 e PTD1 em 1 (ativo baixo)
	GPIOB_PSOR = GPIO_PSOR_PTSO(0b11<<18);    // Resetar pinos 18, 19 do PORTB
	GPIOD_PSOR = GPIO_PSOR_PTSO(0b1<<1);      // Resetar pino 1 do PORTD
}

void GPIO_initLedG() {
	// Modulo SIM: habilita o clock do modulo PORTB
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; 
	
	// Modulo PORT: configure o pino PTB19 para GPIO (vermelho)
	PORTB_PCR19 &= ~PORT_PCR_MUX (0b111); // mascarar os bits 8, 9 e 10
	
	PORTB_PCR19 |= (PORT_PCR_ISF_MASK |	  // restar o bit de estado  	
			PORT_PCR_MUX(0b001)| 		  // mascarar os bits 8, 9 e 10		
			PORT_PCR_DSE_MASK);			  // aumentar a corrente 
	
	// Modulo GPIO: configure o sentido do sinal no pino PTB19
	GPIOB_PDDR |= GPIO_PDDR_PDD(0b1<<19);    // Seta pino 19 do PORTB como saida 	
	
	// Modulo GPIO: inicializar PTB18 em 1 (ativo baixo)
	GPIOB_PSOR = GPIO_PSOR_PTSO(0b1<<19);    // Resetar pino 19 do PORTB
}

void GPIO_ledG(tipo_booleano estado) {
	switch (estado) {
	case OFF:
		// Escrever 1 (Set) no pino PTB19 onde a G esta ligada
		GPIOB_PSOR = GPIO_PSOR_PTSO(1<<19); 
		break;
	case ON:
	 	// Escrever 0 (Clear) no pino PTB19 onde a G esta ligada
		GPIOB_PCOR = GPIO_PCOR_PTCO(1<<19);
		break;
	}	
}

void GPIO_ledRGB(tipo_booleano estadoR, tipo_booleano estadoG, tipo_booleano estadoB) {
	uint8_t tmp = (estadoR << 0 | estadoG << 1);
	
	//Apagar os leds
	GPIOB_PSOR = GPIO_PSOR_PTSO(0b11<<18);
	GPIOD_PSOR = GPIO_PSOR_PTSO(0b1<<1);

	//Setar os novos estados
	GPIOB_PCOR =  GPIO_PCOR_PTCO(tmp<<18);
	GPIOD_PCOR =  GPIO_PCOR_PTCO(estadoB<<1);
}

void GPIO_ledG_t() {
	GPIOB_PTOR = GPIO_PTOR_PTTO(0b1<<19);	
}

void GPIO_ledRGB_t() {
	GPIOB_PTOR = GPIO_PTOR_PTTO(0b11<<18);	
	GPIOD_PTOR = GPIO_PTOR_PTTO(1<<1);		
}
