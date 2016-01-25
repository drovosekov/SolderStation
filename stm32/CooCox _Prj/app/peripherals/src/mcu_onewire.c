/*
 * onewire.c
 *
 *  Created on: 25/05/2015
 *      Author: 2San
 */

#include "main.h"


// Буфер для приема/передачи по 1-wire
uint8_t ow_buf[8];

#define OW_0	0x00
#define OW_1	0xff
#define OW_R_1	0xff

//#define FULL_DUPLEX



//-----------------------------------------------------------------------------
// функция преобразует один байт в восемь, для передачи через USART
// ow_byte - байт, который надо преобразовать
// ow_bits - ссылка на буфер, размером не менее 8 байт
//-----------------------------------------------------------------------------
void OW_toBits(uint8_t ow_byte, uint8_t *ow_bits) {
	uint8_t i;
	for (i = 0; i < 8; i++) {
		if (ow_byte & 0x01) {
			*ow_bits = OW_1;
		} else {
			*ow_bits = OW_0;
		}
		ow_bits++;
		ow_byte = ow_byte >> 1;
	}
}

//-----------------------------------------------------------------------------
// обратное преобразование - из того, что получено через USART опять собирается байт
// ow_bits - ссылка на буфер, размером не менее 8 байт
//-----------------------------------------------------------------------------
uint8_t OW_toByte(uint8_t *ow_bits) {
	uint8_t ow_byte, i;
	ow_byte = 0;
	for (i = 0; i < 8; i++) {
		ow_byte = ow_byte >> 1;
		if (*ow_bits == OW_R_1) {
			ow_byte |= 0x80;
		}
		ow_bits++;
	}

	return ow_byte;
}

#ifdef FULL_DUPLEX
//-----------------------------------------------------------------------------
// инициализирует USART и DMA
//-----------------------------------------------------------------------------
uint8_t OW_Init() {
	#ifndef OW_USART
		return OW_ERROR;
	#endif

	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

#ifdef OW_USART1
		// USART TX
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;

		GPIO_Init(GPIOA, &GPIO_InitStruct);

		// USART RX
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;

		GPIO_Init(GPIOA, &GPIO_InitStruct);

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
#endif
#ifdef OW_USART2
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;

		GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;

		GPIO_Init(GPIOA, &GPIO_InitStruct);

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
#endif

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

	USART_Init(OW_USART, &USART_InitStructure);
	USART_Cmd(OW_USART, ENABLE);
	return OW_OK;
}
#endif

#ifndef FULL_DUPLEX
//-----------------------------------------------------------------------------
// инициализирует USART и DMA в полудуплексном режиме
// используется только одна нога МК
//-----------------------------------------------------------------------------
uint8_t OW_Init() {
	#ifndef OW_USART
		return OW_ERROR;
	#endif

	//GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStructure;

	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

	//GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;

#ifdef OW_USART1
	// USART TX
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	//GPIO_Init(GPIOA, &GPIO_InitStruct);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
#endif
#ifdef OW_USART2
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	//GPIO_Init(GPIOA, &GPIO_InitStruct);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
#endif
#ifdef OW_USART3
	//GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	//GPIO_Init(GPIOB, &GPIO_InitStruct);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
#endif

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

	USART_Init(OW_USART, &USART_InitStructure);
	USART_Cmd(OW_USART, ENABLE);

	// Здесь вставим разрешение работы USART в полудуплексном режиме
	USART_HalfDuplexCmd(OW_USART, ENABLE);

	return OW_OK;
}
#endif

//-----------------------------------------------------------------------------
// осуществляет сброс и проверку на наличие устройств на шине
//-----------------------------------------------------------------------------
uint8_t OW_Reset() {
	uint8_t ow_presence;
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(OW_USART, &USART_InitStructure);

	// отправляем 0xf0 на скорости 9600
	USART_ClearFlag(OW_USART, USART_FLAG_TC);
	USART_SendData(OW_USART, 0xf0);
	while (USART_GetFlagStatus(OW_USART, USART_FLAG_TC) == RESET) {
#ifdef OW_GIVE_TICK_RTOS
		taskYIELD();
#endif
	}

	ow_presence = USART_ReceiveData(OW_USART);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(OW_USART, &USART_InitStructure);

	if (ow_presence != 0xf0) {
		return OW_OK;
	}

	return OW_NO_DEVICE;
}


//-----------------------------------------------------------------------------
// процедура общения с шиной 1-wire
// sendReset - посылать RESET в начале общения.
// 		OW_SEND_RESET или OW_NO_RESET
// command - массив байт, отсылаемых в шину. Если нужно чтение - отправляем OW_READ_SLOTH
// cLen - длина буфера команд, столько байт отошлется в шину
// data - если требуется чтение, то ссылка на буфер для чтения
// dLen - длина буфера для чтения. Прочитается не более этой длины
// readStart - с какого символа передачи начинать чтение (нумеруются с 0)
//		можно указать OW_NO_READ, тогда можно не задавать data и dLen
//-----------------------------------------------------------------------------
u8 OW_Send(u8 sendReset, u8 *command, u8 cLen, u8 *data, u8 dLen, u8 readStart) {

	// если требуется сброс - сбрасываем и проверяем на наличие устройств
	if (sendReset == OW_SEND_RESET) {
		if (OW_Reset() == OW_NO_DEVICE) {
			return OW_NO_DEVICE;
		}
	}

	while (cLen > 0) {
		OW_toBits(*command, ow_buf);
		command++;
		cLen--;

		DMA_InitTypeDef DMA_InitStructure;

		// DMA на чтение
		DMA_DeInit(OW_DMA_CH_RX);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(OW_USART->DR);
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) ow_buf;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		DMA_InitStructure.DMA_BufferSize = 8;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
		DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		DMA_Init(OW_DMA_CH_RX, &DMA_InitStructure);

		// DMA на запись
		DMA_DeInit(OW_DMA_CH_TX);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) &(OW_USART->DR);
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) ow_buf;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
		DMA_InitStructure.DMA_BufferSize = 8;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
		DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		DMA_Init(OW_DMA_CH_TX, &DMA_InitStructure);

		// старт цикла отправки
		USART_ClearFlag(OW_USART, USART_FLAG_RXNE | USART_FLAG_TC | USART_FLAG_TXE);
		USART_DMACmd(OW_USART, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);
		DMA_Cmd(OW_DMA_CH_RX, ENABLE);
		DMA_Cmd(OW_DMA_CH_TX, ENABLE);

		// Ждем, пока не примем 8 байт
		while (DMA_GetFlagStatus(OW_DMA_FLAG) == RESET){
#ifdef OW_GIVE_TICK_RTOS
			taskYIELD();
#endif
		}

		// отключаем DMA
		DMA_Cmd(OW_DMA_CH_TX, DISABLE);
		DMA_Cmd(OW_DMA_CH_RX, DISABLE);
		USART_DMACmd(OW_USART, USART_DMAReq_Tx | USART_DMAReq_Rx, DISABLE);

		// если прочитанные данные кому-то нужны - выкинем их в буфер
		if (readStart == 0 && dLen > 0) {
			*data = OW_toByte(ow_buf);
			data++;
			dLen--;
		} else {
			if (readStart != OW_NO_READ) {
				readStart--;
			}
		}
	}

	return OW_OK;
}


void OW_out_set_as_TX_pin(void){
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

#ifdef OW_USART1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
		// USART TX
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif
#ifdef OW_USART2
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;

		GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif
#ifdef OW_USART3
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
		GPIO_Init(GPIOB, &GPIO_InitStruct);
#endif
}

void OW_out_set_as_Power_pin(void){
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

#ifdef OW_USART1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
		// GPIO
		GPIO_SetBits(GPIOA, GPIO_Pin_9);
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif
#ifdef OW_USART2
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_SetBits(GPIOA , GPIO_Pin_2);
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
		GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif
#ifdef OW_USART3
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_SetBits(GPIOB , GPIO_Pin_10);
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
		GPIO_Init(GPIOB, &GPIO_InitStruct);
#endif
}
