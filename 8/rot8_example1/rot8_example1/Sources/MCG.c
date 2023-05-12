/*
 * @file MCG.c
 * @brief Funcoes referentes a MCG
 * @date Jan 23, 2023
 * @author Wu Shin-Ting
 */

#include "derivative.h"

void MCG_initFLL48MH() {
  
  /*
   * Habilitar fonte de relogio interna
   */	
  MCG_C1 |= MCG_C1_IREFS_MASK;               //sinal de relogio de referencia para FLL = referencia interna lenta

  /*
   * Definir o intervalo das frequencias para FLL
   */
  MCG_C4 &= ~MCG_C4_DRST_DRS(0x03);
  MCG_C4 |= MCG_C4_DRST_DRS(0x01);
  MCG_C4 |= MCG_C4_DMX32_MASK;

  /*
   * Espera recomendada no Manual (secao 4.1.3/p.36)
   */
  while((MCG_S & MCG_S_IREFST_MASK) == 0x00U) { 
	  /* Check that the source of the FLL reference clock is the internal reference clock. */
  }
  while((MCG_S & 0x0CU) != 0x00U) {    
	  /* Wait until output of the FLL is selected */
  }
}

void MCG_initIR4MHFLL20MH() {

  MCG_C1 |= (MCG_C1_CLKS(0b00) 				//fonte de MCGOUTCLK = FLL/PLL
		  | MCG_C1_FRDIV(0b000)             	//divisor de frequencia para referencia externa
		  | MCG_C1_IREFS_MASK               //sinal de relogio de referencia para FLL = referencia interna lenta
          | MCG_C1_IRCLKEN_MASK				//habilita MCGIRCLK
          | MCG_C1_IREFSTEN_MASK);			//habilita IR no modo Stop	

  MCG_C2 |= MCG_C2_IRCS_MASK;				// sinal de referencia interna rapida
  
  //Frequencia FLL no intervalo de baixas frequencias (FLL=20971520Hz)
  MCG_C4 &= ~MCG_C4_DRST_DRS(0x03);
  MCG_C4 &= ~MCG_C4_DMX32_MASK;

  MCG_C4 |= MCG_C4_FCTRIM(0xA);
  
  MCG_SC |= (MCG_SC_ATME_MASK               //habilita auto trim machine
		  | MCG_SC_ATMS_MASK); 				//seleciona auto trim machine: 4MHz
  
  MCG_SC &= ~MCG_SC_FCRDIV(0b111);          //divisor do sinal IR rapida em 1
  
  
  //Espera recomendada pelo fabricante
  while((MCG_S & MCG_S_IREFST_MASK) == 0x00U) { 
	  /* Check that the source of the FLL reference clock is the internal reference clock. */
  }
  while((MCG_S & 0x0CU) != 0x00U) {    
	  /* Wait until output of the FLL is selected */
  }
  while (!(MCG_S & MCG_S_IRCST_MASK)) {
	  /* Wait until output of the IR is selected */
  }
  
  return;
}

