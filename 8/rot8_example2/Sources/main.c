/*
 * @file main.c
 * @brief TPM functionality in low power stop mode (Exemplo 2 do Capitulo 12 KLQRUG)
 * @date 29/01/2023
 * @author Wu Shin Ting
 *
 */

#include "stdint.h"
#include "MCG.h" 
#include "SIM.h"
#include "TPM.h"
#include "DMA.h"
#include "ISR.h"
#include "GPIO_switches.h"

#include "derivative.h"

int main(void)
{
	const uint16_t au16PwmBuffSin[64] = \
			{ 2000, 2199, 2396, 2589, 2776, 2956, 3126, 3285, 3431, 3563, 3680, 3779, 3861,\
		3925, 3969, 3990, 3990, 3990, 3949, 3895, 3823, 3732, 3623, 3499, 3360, 3207,\
		3042, 2867, 2684, 2493, 2298, 2099, 1900, 1701, 1506, 1315, 1132, 957, 792, \
		639, 500, 376, 267, 176, 104, 50, 15, 15, 15, 30, 74, 138, 220, 319, 436, \
		568, 714, 873, 1043, 1223, 1410, 1603, 1800, 2000};

	//	const uint16_t au16PwmBuffTrg[64] = 
	//			{ 0, 133, 266, 399, 533, 667, 800, 933, 1066, 1200, 1333, 1466, 1600, 1733, 
	//		1866, 2000, 2133, 2266, 2400, 2533, 2667, 2800, 2933, 3067, 3200, 3333, 
	//		3466, 3600, 3733, 3867, 4000, 0, 133, 266, 399, 533, 667, 800, 933, 1066, 
	//		1200, 1333, 1466, 1600, 1733, 1866, 2000, 2133, 2266, 2400, 2533, 2667, 
	//		2800, 2933, 3067, 3200, 3333, 3466, 3600, 3733, 3867, 4000};
	const uint16_t au16PwmBuffTrg[64] = \
			{ 0, 129, 258, 387, 516, 645, 774, 903, 1032, 1161, 1290, 1419, 1548, 1677, \
		1806, 1935, 2064, 2193, 2322, 2451, 2580, 2709, 2838, 2967, 3096, 3225, \
		3354, 3483, 3612, 3741, 3870, 4000, 0, 129, 258, 387, 516, 645, 774, 903, \
		1032, 1161, 1290, 1419, 1548, 1677, 1806, 1935, 2064, 2193, 2322, 2451, \
		2580, 2709, 2838, 2967, 3096, 3225, 3354, 3483, 3612, 3741, 3870, 4000};

	const uint16_t au16PwmBuffSqr[64] = \
			{ 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000,\
		4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000, 4000,\
		4000, 4000, 4000, 4000, 4000, 4000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,\
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	uint16_t au16PwmDuties[64];

	/*
	 * Inicializar os registradores de SIM antes de TPM e DMA
	 */
	SIM_reseta();

	/*
	 * Setar fonte de TPMx
	 */
	SIM_setaTPMSRC (3);

	/*
	 * Configurar MCGIRCLK 4MHz para modo de operacao VLPS
	 */
	MCG_initIR4MHFLL20MH();

	/*
	 * Habilitar os sinais de relogio e setar o modo de multiplexacao de PTA4 e PTE21
	 */
	TPM1DMA_PTE21_config_basica();
	GPIO_initSwitchNMI (9, 1);

	/*
	 * Configurar transferencias diretas entre TPM1CH1 e memoria via DMA0 sob disparos TPM1 TOF
	 */
	//TPM_config_especifica (0, 0x0FA0, 0b1111, 0, 0, 0, 1, 1, 0b000);	//periodo = 4000/4000000 = 0.001s
	TPM_config_especifica (1, 0x07D0, 0b1111, 0, 0, 0, 1, 1, 0b000);	//periodo = 2000/4000000 = 0.0005s

	//TPM_CH_config_especifica (0, 2, 0b1010, 0x00);
	TPM_CH_config_especifica (1, 1, 0b1010, 0x00);		//CPWM

	// Habilitar a interrupcao IRQ0: enable_irq(0); set_irq_priority(0, 2)
	DMA_habilitaNVICIRQ(0, 2);
	
	//DMA0 config_especifica
	//DMA0_MemoTPM1CH1_config_especifica((uint32_t *)au16PwmDuties, 64000); // tamanho eh 128 bytes
	DMA0_MemoTPM1CH1_config_especifica((uint32_t *)au16PwmDuties, 64*2);
	
	//!!! Instrucoes adicionais para codigo funcionar
	ISR_setaBlockSize (64*2);
	ISR_setaSAR ((uint32_t *)au16PwmDuties);
	
	/*
	 * Habilitar o modo VLPx
	 */
	SMC_PMPROT |= SMC_PMPROT_AVLP_MASK;

	uint8_t i, anterior;
	uint16_t * p_au16PwmDuties;

	//Inicializa o bloco de dados a ser transferido
	ISR_setaBufferSelect (3);
	anterior = 3;
	
	p_au16PwmDuties = (uint16_t *)au16PwmBuffSqr;
	
	for (i = 0; i < 64; i++)
	{
		au16PwmDuties[i] = p_au16PwmDuties[i]>>1;
	}

	for(;;) {	   
		TPM_setaCnV (1, 1, 0);
		
		DMAMUX_desabilitaCanal (0);

		//Otimizacao ... evitar processamentos redundantes
		uint8_t atual = ISR_leBufferSelect();
		
		if (atual != anterior) {
			switch(atual) 
			{
			case 1: p_au16PwmDuties = (uint16_t *)au16PwmBuffSin; break;
			case 2: p_au16PwmDuties = (uint16_t *)au16PwmBuffTrg; break;
			case 3: p_au16PwmDuties = (uint16_t *)au16PwmBuffSqr; break;
			default: break;
			}

			for (i = 0; i < 64; i++)
			{
				au16PwmDuties[i] = p_au16PwmDuties[i]>>1;
			}

			anterior = atual;
		}
		
		DMAMUX_habilitaCanal (0);

		SMC_PMCTRL |= SMC_PMCTRL_STOPM(2);     	//Very-Low-Power Stop 
		SCB_SCR |= SCB_SCR_SLEEPDEEP_MASK;		//Habilitar deep sleep (System Control Block (B3.2.7/p. 270) em ARMv6)
		asm("WFI");

	}

	return 0;
}
