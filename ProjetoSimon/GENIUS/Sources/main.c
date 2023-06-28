/*
 * main implementation: use this 'C' sample to create your own application
 *
 */


#define NUM_MAPAS 3


#include "derivative.h" /* include peripheral declarations */
#include "buzzer.h"
#include "buffer_circular.h"
#include "IR_Receiver.h"
#include "LED_Matrix.h"
#include "SIM.h"
#include "TPM.h"
#include "util.h"
#include "ISR.h"


int main(void)
{
	/*
	 * Configurar MCGFLLCLK 20971520Hz com fonte de sinais de relogio
	 */
	SIM_setaFLLPLL (0);
	
	LEDM_init_pins();
	LEDM_init_SPI();
	
	LEDM_init_matrix(0x5);
	LEDM_clear();
	/*
	 * Configura fonte de pulsos para contadores de TPMs
	 */
	SIM_setaTPMSRC (0b01);

	TPM1TPM2_PTE20PTE22_config_basica();

	TPM_config_especifica(1, 65535, 0b1111, 0, 0, 0, 0, 0, 0b010); // periodo = 12,49980927 ms > 9 ms ok

	TPM_CH_config_especifica(1, 0, 0b0011, 0); // Input capture - ambas as bordas	

	TPM_habilitaInterrupCH(1,0); // habilita interrupcao do canal
	
	TPM_habilitaNVICIRQ(18, 3); // habilita interrupcao do TPM1
	
	ISR_EscreveEstado(INICIO);
	
	ISR_inicializaBC();		
		
	uint16_t leitura;
	
	uint8_t mapas[NUM_MAPAS][10] = {
			{2, 4, 6, 8},
			{2, 4, 5, 6, 8},
			{1, 2, 3, 4, 5, 6, 7, 8, 9}
	};
	
	uint8_t tamanho_mapa[NUM_MAPAS] = {4, 5, 9}; 
	
	uint8_t mapa_selecionado = 0;
				
	uint8_t sequencia[100];
	
	uint8_t tamanho_sequencia;
	
	uint8_t posicao_sequencia;
	
	uint8_t i;
	
	uint8_t numero_teclado;
		
	for(;;)switch(ISR_LeEstado()){
	
		case LEITURA_INICIO:
			if(IR_Leitura(&leitura))	ISR_EscreveEstado(INICIO);
			else						ISR_EscreveEstado(MOSTRA_MAPA);
			break;
		case MOSTRA_MAPA:
			LEDM_acende_posicoes(mapas[mapa_selecionado], tamanho_mapa[mapa_selecionado]);
			ISR_EscreveEstado(ESPERA_MAPA);
			break;
		case LEITURA_MAPA:
			if(IR_Leitura(&leitura))	ISR_EscreveEstado(ESPERA_MAPA);
			else						ISR_EscreveEstado(INTERPRETA_MAPA);
			break;	
		case INTERPRETA_MAPA:
			switch(IR_decodifica(leitura)) {
			case ARROW_RIGHT:
				LEDM_clear();
				espera_1ms(250);
				mapa_selecionado++;
				mapa_selecionado = mapa_selecionado%NUM_MAPAS;
				ISR_EscreveEstado(MOSTRA_MAPA);
				break;
			case ARROW_LEFT:
				LEDM_clear();
				espera_1ms(300);
				mapa_selecionado += NUM_MAPAS-1;	
				mapa_selecionado = mapa_selecionado%NUM_MAPAS;
				ISR_EscreveEstado(MOSTRA_MAPA);
				break;
			case OK:
				ISR_EscreveEstado(INICIALIZA_JOGO);
				break;
			default:
				liga_buzzer_erro();
				espera_1ms(300);
				desliga_buzzer();
				ISR_EscreveEstado(ESPERA_MAPA);
				break;
			}
			break;
		
		case INICIALIZA_JOGO:
			LEDM_clear();
			espera_1ms(500);
			LEDM_desenha_grade();
			espera_1ms(500);
			
			tamanho_sequencia = 0;
			sequencia[tamanho_sequencia++] = mapas[mapa_selecionado][geraNumeroAleatorio(0, tamanho_mapa[mapa_selecionado])];
			ISR_EscreveEstado(MOSTRA_SEQUENCIA);
		case MOSTRA_SEQUENCIA:
			for(i = 0; i < tamanho_sequencia; i++){
				LEDM_acende_posicao(sequencia[i]);
				liga_buzzer_pos(sequencia[i]);
				espera_1ms(500);
				LEDM_desenha_grade();
				desliga_buzzer();
				if(i != tamanho_sequencia-1) espera_1ms(400); // espera so se nao for o ultimo, se for ja pode receber entrada do usuario
			}
			posicao_sequencia = 0;
			ISR_EscreveEstado(ESPERA_JOGO);
			break;
		case LEITURA_JOGO:
			if(IR_Leitura(&leitura))	ISR_EscreveEstado(ESPERA_JOGO);
			else						ISR_EscreveEstado(INTERPRETA_JOGO);
			break;
		case INTERPRETA_JOGO:
			numero_teclado = IR_numero(IR_decodifica(leitura)) ;
			if(numero_teclado == -1){
				ISR_EscreveEstado(ESPERA_JOGO); //tecla nao numerica
			}
			else if(!includes(mapas[mapa_selecionado], tamanho_mapa[mapa_selecionado], numero_teclado)) { // nao faz parte do mapa atual
				liga_buzzer_erro();
				espera_1ms(300);
				desliga_buzzer();
				ISR_EscreveEstado(ESPERA_JOGO);
			}
			else if(numero_teclado == sequencia[posicao_sequencia]) { // acertou a proxima da sequencia
				LEDM_acende_posicao(numero_teclado);
				liga_buzzer_pos(numero_teclado);
				espera_1ms(500);
				LEDM_desenha_grade();
				desliga_buzzer();
				
				posicao_sequencia++;
				
				if(posicao_sequencia == tamanho_sequencia){ // terminou de inserir a sequencia corretamente
					sequencia[tamanho_sequencia++] = mapas[mapa_selecionado][geraNumeroAleatorio(0, tamanho_mapa[mapa_selecionado])];
					liga_buzzer_sucesso();
					ISR_EscreveEstado(MOSTRA_SEQUENCIA);
					espera_1ms(1000);
				} else { // ainda faltam posicoes para inserir na sequencia
					ISR_EscreveEstado(ESPERA_JOGO);
				}
			} else { // errou a sequencia
				LEDM_acende_posicao(numero_teclado);
				toca_buzzer_perdeu();
				ISR_EscreveEstado(MOSTRA_RESULTADO);
			}
			break;
		case MOSTRA_RESULTADO:
			LEDM_escreve_string("Perdeu parca", 750);

			ISR_EscreveEstado(INICIO);
			break;
		default:
			break;
	}
	
	return 0;
}
