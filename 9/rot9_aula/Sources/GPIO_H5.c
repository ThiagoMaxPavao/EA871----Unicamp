/*!
 * @file GPIO_H5.c
 * @brief Este modulo contem interface com os pinos do header H5 do shield FEEC871
 * @author Wu Shin Ting
 * @date 27/02/2022
 */
#include "derivative.h"
#include "util.h"

void GPIO_H5Pins_config_basica () {
	// Habilita o clock do modulo PORTE
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK ;    		

	// Configure pinos 1, 2, 3, 4 com nivel 0
	// Funcao GPIO
	PORTE_PCR20 |= PORT_PCR_MUX(0x1);  

	PORTE_PCR21 |= PORT_PCR_MUX(0x1);  

	PORTE_PCR22 |= PORT_PCR_MUX(0x1); 

	PORTE_PCR23 |= PORT_PCR_MUX(0x1);  

	return;
}

void GPIO_H5Pins_config_especifica() {
	
	// Sentido do sinal: saida
	GPIOE_PDDR |= GPIO_PDDR_PDD(GPIO_PIN(20)) |
			GPIO_PDDR_PDD(GPIO_PIN(21)) |
			GPIO_PDDR_PDD(GPIO_PIN(22)) |
			GPIO_PDDR_PDD(GPIO_PIN(23));
	
	// Limpar os bits	
	GPIOE_PCOR = GPIO_PCOR_PTCO(GPIO_PIN(20)) |
			GPIO_PCOR_PTCO(GPIO_PIN(21)) |
			GPIO_PCOR_PTCO(GPIO_PIN(22)) |
			GPIO_PCOR_PTCO(GPIO_PIN(23)) ;	
}

void GPIO_H5Pin2_config_basica () {
	// Habilita o clock do modulo PORTE
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK ;    		

	// Funcao GPIO
	PORTE_PCR22 |= PORT_PCR_MUX(0x1); 

	return;
}

void GPIO_H5Pin2_config_especifica() {
	// Sentido do sinal: saida
	GPIOE_PDDR |= GPIO_PDDR_PDD(GPIO_PIN(22));
	
	// Limpar os bits	
	GPIOE_PCOR = GPIO_PCOR_PTCO(GPIO_PIN(22)) ;
}
