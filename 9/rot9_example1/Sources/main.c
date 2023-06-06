/*
 * @file main.c
 * @brief Exemplo de configuracao dado no Cap. 11 em KLRUG.
 * @date 04/02/2023
 * @author Wu Shin-Ting
 *
 */

#include "OSC.h"
#include "LPTMR.h"
#include "ADC.h"
#include "GPIO_ledRGB.h"
#include "ISR.h"

#include "derivative.h" /* include peripheral declarations */

//Uso das estruturas pre-definidas no IDE CodeWarrior (MKL25Z4.h)
//	Master_Adc_Config.CONFIG1 = ADLPC_NORMAL
//	 | ADC_CFG1_ADIV(ADIV_4)
//	 | ADLSMP_LONG
//	 | ADC_CFG1_MODE(MODE_16)
//	 | ADC_CFG1_ADICLK(ADICLK_BUS);
//	Master_Adc_Config.CONFIG2 = MUXSEL_ADCB//select ADC0_SE4B
//	 | ADACKEN_DISABLED
//	 | ADHSC_HISPEED
//	 | ADC_CFG2_ADLSTS(ADLSTS_20) ;
//	Master_Adc_Config.COMPARE1 = 0x1234u;
//	Master_Adc_Config.COMPARE2 = 0x5678u;
//	Master_Adc_Config.STATUS2 = ADTRG_HW //Hardware trigger
//	 | ACFE_DISABLED
//	 | ACFGT_GREATER
//	 | ACREN_ENABLED
//	 | DMAEN_DISABLED
//	 | ADC_SC2_REFSEL(REFSEL_EXT);
//	Master_Adc_Config.STATUS3 = CAL_OFF
//	 | ADCO_SINGLE
//	 | AVGE_ENABLED
//	 | ADC_SC3_AVGS(AVGS_32);
//	Master_Adc_Config.STATUS1A = AIEN_OFF | DIFF_SINGLE | ADC_SC1_ADCH(31);
//	Master_Adc_Config.STATUS1B = AIEN_OFF | DIFF_SINGLE | ADC_SC1_ADCH(31); 

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
		| ADC_CFG1_MODE(MODE_16)
		| ADC_CFG1_ADICLK(ADICLK_BUS),   
		.CFG2=MUXSEL_ADCA					//select ADC0_SE9 (PTB1 potenciometro externo)
		| ADACKEN_DISABLED
		| ADHSC_HISPEED
		| ADC_CFG2_ADLSTS(ADLSTS_20),
		.CV1=0x1234u,                                   
		.CV2=0x5678u,
		.SC2=ADTRG_HW //Hardware trigger
		| ACFE_ENABLED
//		| ACFE_DISABLED
		| ACFGT_GREATER
		| ACREN_ENABLED
		| DMAEN_DISABLED
		| ADC_SC2_REFSEL(REFSEL_EXT),                                    
		.SC3=CAL_OFF
		| ADCO_SINGLE
		| AVGE_ENABLED
		| ADC_SC3_AVGS(AVGS_32),
};


int main(void)
{
	//Configurar a fonte de sinais osciladores para RTC/LPTMR
	//OSC_RTCCLKIN ();					
	OSC_LPO1kHz ();							//no lugar de RTCCLK_IN (PTC1) ja alocado para LCD, usa-se LPO1kHz

	//Habilita sinais de relogio e configura pinos para os modulos
	//Em FRDMKL25Z nao ha on-board potenciometro. Substituimos 
	//por um externo conectado no pino PTB1
	ADC_PTB1_config_basica(LPTMR0_TRG);   // ADC com PTB1 multiplexado para SE9 no lugar de 
	// PTE29 (SE4B)

	LPTMR_config_basica ();				  // LPTMR0 sem pino de saida

	GPIO_initLedG();					  // !!! Led G para sinalizacao do fim de uma conversao

	/*
	 * Caso queira ver o periodo configurado para LPTMR0 no analisador, basta descomentar
	 * as instrucoes abaixo e LPTimer_IRQHandler
	 */
	SIM_SCGC5 |= (SIM_SCGC5_LPTMR_MASK
			| SIM_SCGC5_PORTE_MASK
	);

	PORTE_PCR21 |= (PORT_PCR_MUX(0b1)
			| PORT_PCR_DSE_MASK
	);
	GPIOE_PDDR |= (1<<21);
	GPIOE_PSOR = (1<<21);
	LPTMR0_habilitaInterrupcao();
	LPTMR0_habilitaNVICIRQ(0);	//!!! se nao eh necessario tratamento especifico

	/*
	 * Caso queira ver o tempo de amostragem e conversao, basta descomentar
	 * uma instrucao em ADC0_IRQHandler
	 */

	//Configura os modulos
	LPTMR_config_especifica (0b0000, 0b01, 250);  // so temos LPO (0b01) disponivel
	// 0b10 = ERCLK32K (Secao 3.8.3.3/p.90); 
	// CMR = 0.5*1000/2 = 250

	ADC_Config_Alt (ADC0_BASE_PTR, &Master_Adc_Config);	  //!!! configura via "drive ADC"
	ADC_Cal (ADC0_BASE_PTR);							  //!!! calibra (Secao 28.4.6/p.494 no Manual)
	ADC_Config_Alt (ADC0_BASE_PTR, &Master_Adc_Config);   //reconfigura

	//Habilita interrupcoes no NVIC
	ADC_habilitaNVICIRQ(0);							// 0 na inicializacao

	//!!! Ativa o canal de ADC
	ADC_selecionaCanal(0b01001);

	//!!! Habilita interrupcao do ADC
	ADC_habilitaInterrupCOCO();

	ISR_escreveCycleFlags(!ADC0A_DONE);

	LPTMR0_ativaCNR ();

	for(;;) {
		if (ISR_leCycleFlags()) {
			// print ADC conversion results
			ISR_escreveCycleFlags(!ADC0A_DONE);
		}
	}

	return 0;
}
