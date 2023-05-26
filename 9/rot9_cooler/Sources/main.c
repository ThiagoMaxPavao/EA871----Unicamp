/*
 * @file main.c
 * @brief Programa para teste de operacao do cooler
 * @date 09/02/2023
 * @author Wu Shin Ting
 *
 */

#include "derivative.h" /* include peripheral declarations */

static TPM_MemMapPtr TPM[] = TPM_BASE_PTRS;

void SIM_setaTPMSRC (uint8_t src) {
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(src) ;
}

void SIM_setaFLLPLL (uint8_t pll) {
	/*
	 * Selecionar o modo de geracao por laco de sincronismo
	 */
	if (pll) {
		SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK; // setar PLL
	} else {
		SIM_SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK; // setar FLL
	}
}

void TPM1_PTB0_config_basica() {
	
	/*
	 * Habilita os sinais de relógio para TPM1
	 */
	SIM_SCGC6 |= (SIM_SCGC6_TPM1_MASK);			

	/*
	 * Configura pinos PTB0
	 */ 
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;				//habilita sinais de relogio

	PORTB_PCR0 = (PORT_PCR_ISF_MASK |				//TPM1_CH0
			PORT_PCR_MUX(0x3)| 
			PORT_PCR_DSE_MASK);						//acionamento maior na corrente de saida
	
	return;
}

void TPM_config_especifica (uint8_t x, uint16_t mod, uint8_t trigger, uint8_t crot, uint8_t csoo, uint8_t csot,
		uint8_t dma, uint8_t cpwms, uint8_t ps) {

	/*
	 * Configurar disparo/recarga do contador CNT
	 */
	uint8_t temp = trigger & 0b1111;
	
	if (temp != 0b0010 && temp != 0b0011 && temp != 0b0110 &&
			temp != 0b0111 && temp != 0b1011 && temp != 0b1111) {
		TPM[x]->CONF &= ~((0b1111<<24)|(0b111<<16));
		TPM[x]->CONF |= TPM_CONF_TRGSEL(trigger);		//Tabela 3-38/p. 86
		if (crot) TPM[x]->CONF |= TPM_CONF_CROT_MASK;           //Resetar CNT no disparo
		if (csoo) TPM[x]->CONF |= TPM_CONF_CSOO_MASK;			//CNT para em TOF 
		if (csot) TPM[x]->CONF |= TPM_CONF_CSOT_MASK;			//Ativar contador no disparo
	}
	/*
	 * Resetar o contador fazendo um acesso de escrita
	 */
	TPM[x]->CNT = 0;
	
	/*
	 * Configurar periodo do contador T = PS*MOD/freq.
	 */
	TPM[x]->SC &= ~TPM_SC_PS(0b111);
	TPM[x]->SC |= TPM_SC_PS(ps);
	if (dma) TPM[x]->SC |= TPM_SC_DMA_MASK;
	if (cpwms) TPM[x]->SC |= TPM_SC_CPWMS_MASK;
	
	/**
	 * Configurar a contagem maxima
	 */
	TPM[x]->MOD = TPM_MOD_MOD(mod);
	
	TPM[x]->SC |= TPM_SC_CMOD(1);			// ativar o contador LPTPM
	return;
}

void TPM_CH_config_especifica (uint8_t x, uint8_t n, uint8_t MS_ELS, uint16_t valor) {
    /*
     * Configurar o modo de operacao do canal
     */
	TPM[x]->CONTROLS[n].CnSC &= ~(0b1111<<2);
	TPM[x]->CONTROLS[n].CnSC |= (MS_ELS<<2);
	
	/*
	 * Inicializar com valor 0 no contador
	 */
	TPM[x]->CONTROLS[n].CnV = TPM_CnV_VAL(valor);	
			
	return;
}

int main(void)
{
	/*
	 * Configura fonte de pulsos para contadores de TPMs (MCGFLLCLK)
	 */
	SIM_setaTPMSRC (0b01);

	/*
	 * Configurar MCGFLLCLK 20971520Hz com fonte de sinais de relogio
	 */
	SIM_setaFLLPLL (0);
	
	TPM1_PTB0_config_basica();
	
	TPM_config_especifica (1, 4095, 0b1111, 0, 0, 0, 0, 0, 0b110);

	TPM_CH_config_especifica (1, 0, 0b1010, (3*4095)/4.);          

	for(;;) {	   
	}
	
	return 0;
}
