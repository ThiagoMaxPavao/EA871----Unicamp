/*
 * @file MCG.c
 * @brief Funcoes referentes a MCG
 * @date Jan 23, 2023
 * @author Wu Shin-Ting
 */

#include "derivative.h"

void MCG_initFLL20MH() {

  MCG_C1 |= (MCG_C1_CLKS(0b00) 				//fonte de MCGOUTCLK = FLL/PLL
		  | MCG_C1_FRDIV(0b000)             	//divisor de frequencia para referencia externa
		  | MCG_C1_IREFS_MASK               //sinal de relogio de referencia para FLL = referencia interna lenta
          | MCG_C1_IRCLKEN_MASK);				//habilita MCGIRCLK
  
  //Frequencia FLL no intervalo de baixas frequencias (FLL=20971520Hz)
  MCG_C4 &= ~MCG_C4_DRST_DRS(0x03);
  MCG_C4 &= ~MCG_C4_DMX32_MASK;  
  
  //Espera recomendada pelo fabricante
  while((MCG_S & MCG_S_IREFST_MASK) == 0x00U);	  /* Check that the source of the FLL reference clock is the internal reference clock. */
  
  while((MCG_S & 0x0CU) != 0x00U);	  /* Wait until output of the FLL is selected */

}

