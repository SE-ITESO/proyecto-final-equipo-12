/*
 * PIT.h
 *
 *  Created on: 15 Sept 2023
 *      Author: Alejandro De La Rosa & Daniel Gutierrez
 */

#ifndef PIT_H_
#define PIT_H_

#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_pit.h"

/* PIT Defines to clarification */
#define PIT_BASEADDR PIT
#define PIT_CHANNEL kPIT_Chnl_0
#define PIT_HANDLER PIT0_IRQHandler
#define PIT_IRQ_ID PIT0_IRQn

/* Source clock to PIT controller */
#define PIT_SOURCE_CLOCK CLOCK_GetFreq(kCLOCK_BusClk)


/* defines for PIT channel ONE */
#define PIT_HANDLER_ONE   PIT1_IRQHandler
#define PIT_CHANNEL_ONE  kPIT_Chnl_1
#define PIT_IRQ_ID_ONE     PIT1_IRQn

/* defines for PIT channel TWO */
#define PIT_HANDLER_TWO   PIT2_IRQHandler
#define PIT_CHANNEL_TWO  kPIT_Chnl_2
#define PIT_IRQ_ID_TWO     PIT2_IRQn

/* defines for PIT channel THREE */
#define PIT_HANDLER_THREE   PIT3_IRQHandler
#define PIT_CHANNEL_THREE  kPIT_Chnl_3
#define PIT_IRQ_ID_THREE     PIT3_IRQn

/* Prototypes */
void PIT_configure(void);
void PIT_callback_init(pit_chnl_t channel, void (*handler)(uint32_t flags));

#endif /* PIT_H_ */
