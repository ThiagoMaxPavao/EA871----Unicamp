/*
 * @file OSC.c
 * @brief Funcoes relacionadas com o modulo de Oscilador OSC
 * @date Feb 5, 2023
 * @author Wu Shin-Ting
 */

#include "derivative.h"

void OSC_LPO1kHz () {
	SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL(0b11); // LPO1kHz is RTC and LPTMR OSC CLOCK
}

void OSC_RTCCLKIN () {
	SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL(0b10); // RTC_CLKIN is RTC and LPTMR OSC CLOCK
	
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;  //Habilitar o sinal de relogio de PORTC
	PORTC_PCR1 |= PORT_PCR_MUX(1);      //Multiplexar o pino PTC1 para RTC_CLKIN
}


