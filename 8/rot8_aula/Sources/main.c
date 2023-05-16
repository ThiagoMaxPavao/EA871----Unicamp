/**
 * @file main.c
 * @brief Este aplicativo demonstra a funcao IC e OC do temporizador TPM
 * @author Wu Shin Ting
 * @date 16/10/2021
 *
 */

#include "TPM.h"
#include "GPIO_H5.h"
#include "SIM.h"

int main( void)
{	
	/*
	 * Configurar MCGFLLCLK 20971520Hz com fonte de sinais de relogio
	 */
	SIM_setaFLLPLL (0);
	
	/*
	 * Configura fonte de pulsos para contadores de TPMs
	 */
	SIM_setaTPMSRC (0b01);
	
	/*
	 * Habilitar sinais de relogio e alocar os pinos
	 * Inicializar os pinos de monitoramento via analisador logico
	 */
	TPM0TPM1_PTA4PTE21_config_basica ();
	GPIO_H5Pin2_config_basica ();
	 
	/*
	 *  Configurar TPM0 e TPM1
	 */
	TPM_config_especifica (0, 65535, 0b1111, 0, 0, 0, 0, 0, 0b111);  // (65535*128)/20971520 = 0.3999
	TPM_config_especifica (1, 65535, 0b1111, 0, 0, 0, 0, 0, 0b111);  // (65535*128)/20971520 = 0.3999

	TPM_CH_config_especifica (0, 1, 0b0010, 0);                //IC na borda de descida
	TPM_CH_config_especifica (0, 2, 0b0110, 0);				   //OC reseta saida em 0 ao igualar
	
	TPM_CH_config_especifica (1, 1, 0b0110, 0);				   //OC reseta saida em 0 ao igualar
	
	/*
	 * Configurar o pino H5Pin2
	 */
	GPIO_H5Pin2_config_especifica();
	
	/*
	 * Habilitar interrupcoes NVIC
	 */
	TPM_habilitaNVICIRQ(17, 1);
	
	/*
	 * Habilitar interrupcoes dos canais
	 */
	TPM_habilitaInterrupCH(0, 1);
	//TPM_habilitaInterrupCH(0, 2);
	 
	for(;;) 
	{
	}
	return 1;
}
