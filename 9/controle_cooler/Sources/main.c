/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */

#include "SIM.h"
#include "GPIO_latch_lcd.h"
#include "TPM.h"
#include "ADC.h"
#include "util.h"

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
		.CFG2=MUXSEL_ADCA //select ADC0_SE9 (PTB1 potenciometro externo)
		| ADACKEN_DISABLED
		| ADHSC_HISPEED
		| ADC_CFG2_ADLSTS(ADLSTS_20),
		.CV1=0x1234u,                                   
		.CV2=0x5678u,
		.SC2=ADTRG_HW //Software trigger
//		| ACFE_ENABLED
		| ACFE_DISABLED
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
	SIM_setaTPMSRC (0b01);
	SIM_setaFLLPLL (0); //Seta FLL como fonte de tpm
	SIM_setaOUTDIV4(0b000); //Seta divisor de frequência do clock do barramento como 1
	
	/*
	 * Inicializa LCD
	 */
	GPIO_ativaConLCD();
	GPIO_initLCD();
	
	TPM1TPM2_PTB0PTB18PTB19_config_basica();
	
	ADC_PTB1_config_basica(TPM2_TRG);

	ADC_Config_Alt (ADC0_BASE_PTR, &Master_Adc_Config);	  // configura via "drive ADC"
	ADC_Cal (ADC0_BASE_PTR);							  // calibra
	ADC_Config_Alt (ADC0_BASE_PTR, &Master_Adc_Config);   // reconfigura
	
	ADC_selecionaCanal(0b01001);
	
	TPM_config_especifica(1, 4095, 0b1111, 0, 0, 0, 0, 0, 0b0110);
	TPM_config_especifica(2, 65535, 0b1111, 0, 0, 0, 0, 0, 0b0110);
	
	TPM_CH_config_especifica(1, 0, 0b0000, 0); // TPM1_CH0 - PTB0  - cooler
	TPM_CH_config_especifica(2, 0, 0b0000, 0); // TPM2_CH0 - PTB18 - canal vermelho LED
	TPM_CH_config_especifica(2, 1, 0b0000, 0); // TPM2_CH1 - PTB19 - canal verde LED

	// TPM_CH_config_especifica(1, 0, 0b1010, 2000);
	
	char buffer[15];
	
	for(;;) {
		ftoa((float) 456.00, buffer, 2);
		GPIO_escreveStringLCD(0x0, (uint8_t *) buffer);
	}
	
	return 0;
}
