/*!
 * @brief Este projeto implementa um programa para ler o valor de um potenciometro e do sensor de temperatura do nucleo: AN3031, a saida e mostrada via LED RGB, LCD e cooler.
 * @author Thiago Pavao
 * @author Vinicius Mantovani
 * @date 06/06/2023
 */

#include "derivative.h" /* include peripheral declarations */

#include "SIM.h"
#include "GPIO_latch_lcd.h"
#include "TPM.h"
#include "ADC.h"
#include "util.h"
#include "ISR.h"

// Configuracao do ADC0
struct ADC_MemMap Master_Adc_Config = {
		.SC1[0]=AIEN_OFF 
		 | DIFF_SINGLE
		 | ADC_SC1_ADCH(31),
		.SC1[1]=AIEN_OFF 
		 | DIFF_SINGLE 
		 | ADC_SC1_ADCH(31),
		.CFG1=ADLPC_NORMAL
		 | ADC_CFG1_ADIV(ADIV_8)
		 | ADLSMP_SHORT
		 | ADC_CFG1_MODE(MODE_16)
		 | ADC_CFG1_ADICLK(ADICLK_BUS_2),   
		.CFG2=MUXSEL_ADCA					//select ADC0_SE9 (PTB1 potenciometro externo)
		 | ADACKEN_DISABLED
		 | ADHSC_NORMAL
		 | ADC_CFG2_ADLSTS(ADLSTS_20),
		.CV1=0x0u,                                   
		.CV2=0x0u,
		.SC2=ADTRG_HW //Hardware trigger
		 | ACFE_DISABLED
		 | ACFGT_LESS
		 | ACREN_DISABLED
		 | DMAEN_DISABLED
		 | ADC_SC2_REFSEL(REFSEL_EXT),                                    
		.SC3=CAL_OFF
		 | ADCO_SINGLE
		 | AVGE_ENABLED
		 | ADC_SC3_AVGS(AVGS_16),
};

int main(void) {
	SIM_setaOUTDIV4(0b000); //Seta divisor de frequência do clock do barramento como 1
	SIM_setaFLLPLL (0); //Seta FLL como fonte de tpm
	SIM_setaTPMSRC (0b01);
	
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
	
	ADC_selecionaCanal(0b01001); // potenciometro

	//Habilita interrupcoes no NVIC
	ADC_habilitaNVICIRQ(2);

	// Habilita interrupcao do ADC
	ADC_habilitaInterrupCOCO();
	
	TPM_config_especifica(1, 65535, 0b1111, 0, 0, 0, 0, 0, 0b0110);
	TPM_config_especifica(2, 65535, 0b1111, 0, 0, 0, 0, 0, 0b0110);
	
	// inicializa is canais do LED desativados e o do cooler em PWM com largura de pulso igual a zero.
	TPM_CH_config_especifica(1, 0, 0b1010, 0); // TPM1_CH0 - PTB0  - cooler
	TPM_CH_config_especifica(2, 0, 0b0000, 0); // TPM2_CH0 - PTB18 - canal vermelho LED
	TPM_CH_config_especifica(2, 1, 0b0000, 0); // TPM2_CH1 - PTB19 - canal verde LED

	TPM_CH_config_especifica(1, 0, 0b1010, 0);
	
	char bolinha[] = {0x06,0x09,0x9,0x06,0x00,0x0,0x0,0x00};
	GPIO_escreveBitmapLCD (0x01, (uint8_t *)bolinha);


	GPIO_escreveStringLCD(0x00, (uint8_t*) " DUTY:          ");
	GPIO_escreveStringLCD(0x40, (uint8_t*) " TEMP:       C  ");
	GPIO_escreveStringLCD(0x4C, (uint8_t*) "\x01");

	char buffer[6];
	uint16_t valores[2];
	
	ISR_EscreveEstado(AMOSTRA_VOLT);
	
	for(;;) {
		if(ISR_LeEstado() == ATUALIZACAO){
			ISR_LeValoresAmostrados(valores);
			uint16_t pot_value  = valores[0],
					 temp_value = valores[1];
			
			float duty = 100.0 * pot_value / 65535,
				  temp = AN3031_Celsius(temp_value);
			
			// Aciona os led com cor e intensidade dependendo da temperatura
			if(temp > 25){
				TPM_CH_config_especifica(2, 0, 0b1010, (uint16_t) (temp - 25)*65535/25);// canal vermelho
				TPM_CH_config_especifica(2, 1, 0b0000, 0);								// canal verde
			} else{
				TPM_CH_config_especifica(2, 0, 0b0000, 0);								// canal vermelho
				TPM_CH_config_especifica(2, 1, 0b1010, (uint16_t) (25 - temp)*65535/25);// canal verde
			}
			
			// LCD
			GPIO_escreveStringLCD(0x07, (uint8_t*) "      "); // limpa posicoes dos numeros
			GPIO_escreveStringLCD(0x47, (uint8_t*) "     ");
			ftoa(duty, buffer, 2);
			GPIO_escreveStringLCD(0x07, (uint8_t*) buffer); // atualiza duty
			ftoa(temp, buffer, 2);
			GPIO_escreveStringLCD(0x47, (uint8_t*) buffer); // atualiza temperatura
			
			// Cooler
			TPM_CH_set_V(1,0,pot_value);
			
			ISR_EscreveEstado(AMOSTRA_VOLT);
		}
	}
	
	return 0;
}
