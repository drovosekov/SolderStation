/*
 * onewire.h
 *
 *  Version 1.0.2
 */

#ifndef ONEWIRE_H_
#define ONEWIRE_H_


#include "main.h"

// для разных процессоров потребуется проверить функцию OW_Init
// на предмет расположения ножек USART
#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"

// выбираем, на каком USART находится 1-wire
#define OW_USART1
//#define OW_USART2
//#define OW_USART3
//#define OW_USART4


#ifdef OW_USART1

#undef OW_USART2
#undef OW_USART3
#undef OW_USART4

#define OW_USART 		USART1
#define OW_DMA_CH_RX 	DMA1_Channel5
#define OW_DMA_CH_TX 	DMA1_Channel4
#define OW_DMA_FLAG		DMA1_FLAG_TC5

#endif


#ifdef OW_USART2

#undef OW_USART1
#undef OW_USART3
#undef OW_USART4

#define OW_USART 		USART2
#define OW_DMA_CH_RX 	DMA1_Channel6
#define OW_DMA_CH_TX 	DMA1_Channel7
#define OW_DMA_FLAG		DMA1_FLAG_TC6

#endif


#ifdef OW_USART3

#undef OW_USART1
#undef OW_USART2
#undef OW_USART4

#define OW_USART 		USART3
#define OW_DMA_CH_RX 	DMA1_Channel1
#define OW_DMA_CH_TX 	DMA1_Channel2
#define OW_DMA_FLAG		DMA1_FLAG_TC1

#endif

// если нужно отдавать тики FreeRTOS, то раскомментировать
//#define OW_GIVE_TICK_RTOS

// первый параметр функции OW_Send
#define OW_SEND_RESET		1
#define OW_NO_RESET		2

// статус возврата функций
#define OW_OK			1
#define OW_ERROR		2
#define OW_NO_DEVICE	3

#define OW_NO_READ		0xff

#define OW_READ_SLOT	0xff

u8 OW_Init();
u8 OW_Send(u8 sendReset, u8 *command, u8 cLen, u8 *data, u8 dLen, u8 readStart);
void OW_out_set_as_Power_pin(void);
void OW_out_set_as_TX_pin(void);

#endif /* ONEWIRE_H_ */
