/*
 * @file main.c
 * @brief Edge Aligned PWM and Input Capture Mode (Exemplo 1 no Capitulo 12 em KLQRUG)
 * @date 26/01/2023
 * @author Wu Shin-Ting
 */

#include "derivative.h"
#include "TPM.h" /* include peripheral declarations */
#include "SIM.h"
#include "MCG.h"
#include "ISR.h"

int main(void)
{
	/*
	 * Habilitar MCGFLLCLK 48MHz
	 */
	MCG_initFLL48MH();
	SIM_setaFLLPLL (0);              //seta FLL	
	SIM_setaTPMSRC (01);			 //seta FLL como fonte de clock de TPMx
	
	/*
	 * Habilitar sinais de relogio e configurar pinos
	 */
	//TPM0TPM1_PTA12PTA13PTC2PTC3_config_basica();
	TPM1TPM2_PTE20PTE21PTE22PTE23_config_basica();
	
//	/*
//	 * TPM0 
//	 */
//	TPM_habilitaNVICIRQ(17, 3);		//Habilitar IRQ17 alocada para TPM0
//	
//	TPM_config_especifica (0, 0x12C0, 0b1111, 0, 0, 0, 0, 0, 0b000); //Configurar periodo do contador (4800*1/48000000)=0,1ms
//	TPM_habilitaInterrupTOF(0);
//	
//	TPM_CH_config_especifica (0, 1, 0b1010, u16PWMDuty); //EPWM polaridade alta 
//	TPM_CH_config_especifica (0, 2, 0b1001, u16PWMDuty); //EPWM polaridade baixa 

	TPM_habilitaNVICIRQ(19, 3);		//Habilitar IRQ19 alocada para TPM2
	
	TPM_config_especifica (2, 0x12C0, 0b1111, 0, 0, 0, 0, 0, 0b000); //Configurar periodo do contador (4800*1/48000000)=0,1ms
	TPM_habilitaInterrupTOF(2);
	
	TPM_CH_config_especifica (2, 0, 0b1010, 480); //EPWM polaridade alta 
	TPM_CH_config_especifica (2, 1, 0b1001, 480); //EPWM polaridade baixa 

	/*
	 * TPM1 
	 */
	TPM_habilitaNVICIRQ(18, 1);		//Habilitar IRQ18 alocada para TPM1

//	TPM_config_especifica (1, 0x2BC0, 0b1000, 0, 1, 1, 0, 0, 0b000); //(11200*1/48000000)=0,0023ms Trigger TPM0
	TPM_config_especifica (1, 0x2BC0, 0b1010, 0, 1, 1, 0, 0, 0b000); //(11200*1/48000000)=0,0023ms Trigger TPM2
	TPM_habilitaInterrupTOF(1);
	
	TPM_CH_config_especifica (1, 0, 0b0001, 0x00); //Capture on rising edge
	TPM_CH_config_especifica (1, 1, 0b0010, 0x00); //Capture on falling edge

	for(;;) {	   
	}
	
	return 0;
}
