/*
 * @file DMA.h
 * @brief Prototipos, macros e tipos de dados referentes a DMA
 * @date Jan 29, 2023
 * @author Wu Shin-Ting
 */

#ifndef DMA_H_
#define DMA_H_

/**
 * @brief Associa fonte de sinais de requiicao de transferencia com o canal
 * @param[in] x canal DMA
 * @param[in] src fonte de sinais
 * @note Codigos das fontes na Tabela 3-20/p.64 do Manual
 */
void DMAMUX_setaReq (uint8_t x, uint8_t src);

/**
 * @brief Setar a fonte, o destino e o buffer circular usado na transferencia
 * @param[in] src endereco da fonte
 * @param[in] sinc indremento no endereco apos a transferencia
 * @param[in] ssize tamanho do dado da fonte por transferencia
 * @param[in] smod tamanho do buffer circular da fonte
 * @param[in] dst endereco do destino
 * @param[in] dinc incremento no endereco apos a transferencia
 * @param[in] dsize tamanho do dado no destino por transferencia
 * @param[in] dmod tamanho do buffer circular do destino
 */
void DMA_SAR_DAR (uint32_t *src, uint8_t sinc, uint8_t ssize, uint8_t smod, 
		uint32_t *dst, uint8_t dinc, uint8_t dsize, uint8_t dmod);

/**
 * @brief Setar o endereco-remetente SAR
 * @param[in] src endereco dentre os especificados no manual (p. 354)
 */
void DMA_setaSAR (uint32_t *src);

/**
 * @brief Setar o modo de transferencia dos dados
 * @pram[in] eint habilitar interrupcao quando finalia uma transferencia
 * @param[in] erq habilitar requisicao do periferico para iniciar uma transferencia
 * @param[in] cs habilitar roubo de alguns ciclos de relogio da CPU para transferencia
 * @param[in] aa habilitar auto-alinhamento
 * @param[in] eadreq habilitar requisicao DMA assincrona
 * @param[in] dreq habilitar desabilitacao automatica quando esgotam dados
 */
void DMA_setaTransfModo(uint8_t eint, uint8_t erq, uint8_t cs, uint8_t aa, uint8_t eadreq, 
		uint8_t dreq);

/**
 * @brief Configurar quantidade de bytes a serem transferidos num bloco
 * @param[in] tam quantidade de bytes.
 */
void DMA_setaTamBloco (uint32_t tam);

/**
 * @brief Habilitar IRQ em NVIC
 * @param[in] irq linha de solicitacao
 * @param[in] prioridade de atendimento
 */
void DMA_habilitaNVICIRQ(uint8_t irq, uint8_t prioridade);

/**
 * @brief Habilitar o canal x de DMA
 * @param[in] x canal (slot)
 */
void DMAMUX_habilitaCanal (uint8_t x);

/**
 * @brief Desabilitar o canal x de DMA
 * @param[in] x canal (slot)
 */
void DMAMUX_desabilitaCanal (uint8_t x);

/**
 * @brief Configuracao especifica memoria - TPM1CH1 via DMA0
 * @param[in] end endereco do bloco de dados a ser transferido 
 * @param[in] tam tamanho do bloco a ser transferido em bytes
 */
void DMA0_MemoTPM1CH1_config_especifica (uint32_t *end, uint32_t tam);

#endif /* DMA_H_ */
