/*!
 * @file main.c
 * @brief Este projeto demonstra o uso de ADC cuja amostragem eh iniciada por disparos por hardware (PIT)
 * @author Wu Shin Ting
 * @date 23/10/2021
 *
 */

#include "ADC.h"
#include "PIT.h"
#include "GPIO_H5.h"
#include "GPIO_latch_lcd.h"
#include "ISR.h"
#include "SIM.h"
#include "util.h"

#define MAX 0xff			///< resolucao 8 bits 

// Configuracao do ADC0
struct ADC_MemMap Master_Adc_Config = {
		.SC1[0]=AIEN_OFF 
		 | DIFF_SINGLE 
		 | ADC_SC1_ADCH(31),
		.SC1[1]=AIEN_OFF 
		 | DIFF_SINGLE 
		 | ADC_SC1_ADCH(31),
		.CFG1=ADLPC_NORMAL
		 | ADC_CFG1_ADIV(ADIV_4)
		 | ADLSMP_LONG
		 | ADC_CFG1_MODE(MODE_8)
		 | ADC_CFG1_ADICLK(ADICLK_BUS_2),   
		.CFG2=MUXSEL_ADCA					//select ADC0_SE9 (PTB1 potenciometro externo)
		 | ADACKEN_DISABLED
		 | ADHSC_HISPEED
		 | ADC_CFG2_ADLSTS(ADLSTS_20),
		.CV1=0x1234u,                                   
		.CV2=0x5678u,
		.SC2=ADTRG_HW //Hardware trigger
		 | ACFE_DISABLED
		 | ACFGT_LESS
		 | ACREN_DISABLED
		 | DMAEN_DISABLED
		 | ADC_SC2_REFSEL(REFSEL_EXT),                                    
		.SC3=CAL_OFF
		 | ADCO_SINGLE
		 | AVGE_ENABLED
		 | ADC_SC3_AVGS(AVGS_8),
};

int main (void)
{	
	/*
	 * Seta o divisor de frequencia do sinal do barramento
	 */ 
	SIM_setaOUTDIV4 (0b001);

	/*
	 * Configura os pinos do Latch 74751
	 */
	GPIO_ativaConLatchLCD ();

	/*
	 * Configura 4 pinos do header H5 para conectar com analisador logico
	 */
	GPIO_H5Pins_config_basica ();
	GPIO_H5Pins_config_especifica();
	
	/*
	 * Configura o timer PIT com habilitacao de IRQ 22 do NVIC
	 */
//	PIT_initTimer0(10485760, 1);            //10485760*2/20971520 = 1s
	PIT_initTimer0(20971520, 1);            //20971520*2/20971520 = 2s	

	/*
	 * Configura o modulo ADC
	 */
	// Ativa sinais de relogio e multiplexa pino para canal 9 (SE9) com disparo HW (PIT0)
	ADC_PTB1_config_basica(PIT0_TRG);

	// Configuracao especifica e calibracao do modulo ADC
	ADC_Config_Alt (ADC0_BASE_PTR, &Master_Adc_Config);	  //!!! configura via "drive ADC"
	ADC_Cal (ADC0_BASE_PTR);							  //!!! calibra (Secao 28.4.6/p.494 no Manual)
	ADC_Config_Alt (ADC0_BASE_PTR, &Master_Adc_Config);   //reconfigura
	
	//Habilita interrupcoes no NVIC
	ADC_habilitaNVICIRQ(2);					
	
	/*
	 * Setup
	 */
	uint8_t amostras[2];
	uint8_t estado;
	
	GPIO_escreveByteLatch(0x00); 				// apagar os leds vermelhos
	ADC_selecionaCanal (0b01001); 				// selecionar o canal do header H7 (PTB1/SE9)

	// Habilita interrupcao do ADC
	ADC_habilitaInterrupCOCO();

	/*!
	 * Ativa PIT
	 */
	PIT_ativaTimer0();

	//Reseta o estado
	amostras[0] = amostras[1] = 0;

	/*!
	 * Laco de interacoes 
	 */
	for(;;) 
	{
		/*!
		 * Polling pela mudanca de estados e valores.
		 * Eles podem estar defasados, mas sao os valores mais
		 * recentes amostrados.
		 */
		ISR_LeValoresAmostrados (amostras);
		estado = ISR_LeEstado();

		// visualiza os dois valores amostrados dos sensores
		if (estado == 3) {
			GPIO_escreveByteLatch(amostras[0]);
			//delay_10us(1000);
			espera_5us(120000);
			GPIO_escreveByteLatch(amostras[1]);
			//delay_10us(1000);
			espera_5us(120000);
		}
	}
	return 1;
}
