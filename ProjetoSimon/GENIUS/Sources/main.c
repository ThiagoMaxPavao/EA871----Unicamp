/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#define NUM_MAPAS 3
#define NUM_NOTAS 35
#define MAX_SEQUENCIA 99

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

	/*
	 * Inicializa os pinos do spi e configura para comunicacao com a matriz de LEDs
	 */
	LEDM_init_pins();
	LEDM_init_SPI();
	
	/*
	 * Inicializa a matriz de LEDs, utilizando a conexao do SPI
	 */
	LEDM_init_matrix(0x5);
	LEDM_clear();
	
	/*
	 * Configura fonte de pulsos para contadores de TPMs
	 */
	SIM_setaTPMSRC (0b01);
	
	/*
	 * Configura os pinos e os TPMs do buzzer e do sensor de controle remoto
	 */
	TPM1TPM2_PTE20PTE22_config_basica();

	TPM_config_especifica(1, 65535, 0b1111, 0, 0, 0, 0, 0, 0b010); // periodo = 12,49980927 ms > 9 ms ok
	
	/*
	 * Inicializa o canal do sensor infravermelho MS_ELS = 0b0011 = Input capture - ambas as bordas
	 */
	TPM_CH_config_especifica(1, 0, 0b0011, 0);

	TPM_habilitaInterrupCH(1,0); // habilita interrupcao do canal do infravermelho
	TPM_habilitaNVICIRQ(18, 3);  // habilita interrupcao do TPM1
	
	ISR_inicializaBC(); // Inicializa buffer circular que armazena os dados lidos do sensor IR
	
	//Inicializa timer 0 do PIT desativado
	PIT_initTimer0(9611946, 1);
	PIT_desativaTimer0();
	
	ISR_EscreveEstado(APRESENTACAO);
	
	// controle da leitura do controle remoto
	uint16_t leitura;
	uint8_t numero_teclado;
	
	// controle do mapa
	uint8_t mapas[NUM_MAPAS][9] = {
			{2, 4, 6, 8},
			{2, 4, 5, 6, 8},
			{1, 2, 3, 4, 5, 6, 7, 8, 9}
	};
	uint8_t tamanho_mapa[NUM_MAPAS] = {4, 5, 9};
	uint8_t mapa_selecionado = 0;
	
	// controle da sequencia de posicoes
	uint8_t sequencia[MAX_SEQUENCIA];
	uint8_t tamanho_sequencia;
	uint8_t posicao_sequencia;
	
	// letras do incio/apresentacao
	char letra_tela[] = "GENIUS ";
	uint8_t posicao_letra = 0;

	// musica da apresentacao
	int index = 0;
	int nota;
	int musica[NUM_NOTAS] = {294, -1, 220, 294, -1, 370, -1, -1, -1, 294, -1, -1,
							 -1, 370, -1, 294, 370, -1, 440, -1, -1, -1, 370, -1,
							 -1, -1, 440, -1, 370, 440, -1, 554, -1, -1, -1}; // -1 significa manter a ultima nota
	
	// string de resultado (pontuacao)
	char resultado[3];
			
	for(;;) switch(ISR_LeEstado()) {
		case APRESENTACAO:
			nota = musica[index++];
			if(index == NUM_NOTAS) { // acabou a musica
				buzzer_desliga();
				LEDM_clear();
				espera_1ms(500);
				PIT_ativaTimer0();
				ISR_EscreveEstado(ESPERA_INICIO);
			}
			else {
				if(nota != -1) buzzer_liga_freq(nota);
				LEDM_escreve_char(letra_tela[(index*6)/NUM_NOTAS]);
				espera_1ms(120); // periodo basico da musica
			}
			break;
		case LEITURA_INICIO:
			if(IR_Leitura(&leitura)) ISR_EscreveEstado(ESPERA_INICIO);
			else {
				LEDM_clear();
				PIT_desativaTimer0();
				buzzer_toca_inicio();
				ISR_EscreveEstado(MOSTRA_MAPA);
			}
			break;
		case ATUALIZA_INICIO:
			LEDM_escreve_char(letra_tela[posicao_letra]);
			posicao_letra++;
			if(letra_tela[posicao_letra] == 0) posicao_letra = 0; // volta para o comeco da string se tiver terminado
			ISR_EscreveEstado(ESPERA_INICIO);
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
				buzzer_liga_freq(600);
				espera_1ms(250);
				buzzer_desliga();
				mapa_selecionado++;
				mapa_selecionado = mapa_selecionado % NUM_MAPAS;
				ISR_EscreveEstado(MOSTRA_MAPA);
				break;
			case ARROW_LEFT:
				LEDM_clear();
				buzzer_liga_freq(500);
				espera_1ms(250);
				buzzer_desliga();
				mapa_selecionado += NUM_MAPAS-1;
				mapa_selecionado = mapa_selecionado % NUM_MAPAS;
				ISR_EscreveEstado(MOSTRA_MAPA);
				break;
			case OK:
				ISR_EscreveEstado(INICIALIZA_JOGO);
				break;
			default: // tecla inesperada
				buzzer_toca_erro();
				ISR_EscreveEstado(ESPERA_MAPA);
				break;
			}
			break;
		case INICIALIZA_JOGO:
			LEDM_clear();
			buzzer_toca_sucesso();
			espera_1ms(500);
			LEDM_desenha_grade();
			espera_1ms(500);
			
			tamanho_sequencia = 0;
			sequencia[tamanho_sequencia++] = mapas[mapa_selecionado][geraNumeroAleatorio(0, tamanho_mapa[mapa_selecionado])];
			ISR_EscreveEstado(MOSTRA_SEQUENCIA);
		case MOSTRA_SEQUENCIA:
			for(index = 0; index < tamanho_sequencia; index++){
				LEDM_acende_posicao(sequencia[index]);
				buzzer_liga_pos(sequencia[index]);
				espera_1ms(500);
				LEDM_desenha_grade();
				buzzer_desliga();
				if(index != tamanho_sequencia-1) espera_1ms(400); // espera so se nao for o ultimo, se for ja pode receber entrada do usuario
			}
			posicao_sequencia = 0;
			ISR_EscreveEstado(ESPERA_JOGO);
			break;
		case LEITURA_JOGO:
			if(IR_Leitura(&leitura))	ISR_EscreveEstado(ESPERA_JOGO);
			else						ISR_EscreveEstado(INTERPRETA_JOGO);
			break;
		case INTERPRETA_JOGO:
			numero_teclado = IR_numero(IR_decodifica(leitura));
			if(numero_teclado == -1 || !includes(mapas[mapa_selecionado], tamanho_mapa[mapa_selecionado], numero_teclado)) {
				buzzer_toca_erro();
				ISR_EscreveEstado(ESPERA_JOGO); //tecla nao numerica ou nao faz parte do mapa atual
			}
			else if(numero_teclado == sequencia[posicao_sequencia]) { // acertou a proxima da sequencia
				LEDM_acende_posicao(numero_teclado);
				buzzer_liga_pos(numero_teclado);
				espera_1ms(500);
				LEDM_desenha_grade();
				buzzer_desliga();
				
				posicao_sequencia++;
				
				if(posicao_sequencia == tamanho_sequencia){ // terminou de inserir a sequencia corretamente
					espera_1ms(300);
					buzzer_toca_sucesso();
					espera_1ms(1000);
					if(tamanho_sequencia == MAX_SEQUENCIA) { // atingiu o maximo
						tamanho_sequencia++; // como se tivesse perdido ao inserir a proxima sequencia
						ISR_EscreveEstado(MOSTRA_RESULTADO);
					}
					else {
						sequencia[tamanho_sequencia++] = mapas[mapa_selecionado][geraNumeroAleatorio(0, tamanho_mapa[mapa_selecionado])];
						ISR_EscreveEstado(MOSTRA_SEQUENCIA);
					}
				} else { // ainda faltam posicoes para inserir na sequencia
					ISR_EscreveEstado(ESPERA_JOGO);
				}
			} else { // errou a sequencia
				LEDM_acende_posicao(numero_teclado);
				buzzer_toca_perdeu();
				ISR_EscreveEstado(MOSTRA_RESULTADO);
			}
			break;
		case MOSTRA_RESULTADO:
			LEDM_clear();
			espera_1ms(1000);
			int2alg_toString(resultado, tamanho_sequencia - 1);
			LEDM_escreve_string(resultado, 750);
			LEDM_clear();
			espera_1ms(500);
			posicao_letra = 0;
			PIT_ativaTimer0();
			ISR_EscreveEstado(ESPERA_INICIO);
			break;
		default:
			break;
	}
	
	return 0;
}
