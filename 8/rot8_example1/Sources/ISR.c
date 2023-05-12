/*
 * @file ISR.c
 * @brief Funcoes relacionadas com o processamento de interrupcoes ISR
 * @date Jan 27, 2023
 * @author Wu Shin Ting
 */

#include "derivative.h"

static uint16_t i16PulseWidth=0;

//void FTM0_IRQHandler () {
//	
//	//Uma implementacao dos 5 niveis de PWM mostrados pelas linhas vermelhas pontilhadas na Fig. 12-4.
//	uint16_t u16PWMDuty[5] = {480, 4320, 2400, 1440, 4320};
//	static uint8_t ind=0;
//
//	TPM0_C1V = u16PWMDuty[ind];
//	TPM0_C2V = u16PWMDuty[ind];
//
//	ind=(ind+1)%5;
//
//	TPM0_SC |= TPM_SC_TOF_MASK;
//}

void FTM2_IRQHandler () {
	
	//Uma implementacao dos 5 niveis de PWM mostrados pelas linhas vermelhas pontilhadas na Fig. 12-4.
	uint16_t u16PWMDuty[5] = {480, 4320, 2400, 1440, 4320};
	static uint8_t ind=0;

	TPM2_C0V = u16PWMDuty[ind];
	TPM2_C1V = u16PWMDuty[ind];

	ind=(ind+1)%5;

	TPM2_SC |= TPM_SC_TOF_MASK;
}

void FTM1_IRQHandler () {
	 int32_t tmp;
	
	 TPM1_SC |= TPM_SC_TOF_MASK;
	 tmp = (int32_t)TPM1_C1V;
	 tmp -= (int32_t)TPM1_C0V;
	 
	 if (tmp > 0) i16PulseWidth=(uint32_t)tmp;
	 
	 TPM1_C0SC |= TPM_CnSC_CHF_MASK;
	 TPM1_C1SC |= TPM_CnSC_CHF_MASK;
}


