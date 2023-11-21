/*
 * PIT.c
 *
 *  Created on: 15 Sept 2023
 *      Author: admin
 */

//includes del ejemplo del PITer parker
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_pit.h"
#include "PIT.h"
#include "NVIC.h"
//#include "fsl_common_arm.h"

static void (*PIT0_callback)(uint32_t flags) = 0;
static void (*PIT1_callback)(uint32_t flags_1) = 0;
static void (*PIT2_callback)(uint32_t flags_2) = 0;
static void (*PIT3_callback)(uint32_t flags_3) = 0;

// Manejador de interrupciones del canal 1 del PIT
void PIT_HANDLER(void)
{
	uint32_t irq_status = 0;
	irq_status = PIT_GetStatusFlags(PIT_BASEADDR, PIT_CHANNEL);

	if(PIT0_callback)
	{
		PIT0_callback(irq_status);
	}

    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT_BASEADDR, PIT_CHANNEL, kPIT_TimerFlag);
    /* Added for, and affects, all PIT handlers. For CPU clock which is much larger than the IP bus clock,
     * CPU can run out of the interrupt handler before the interrupt flag being cleared, resulting in the
     * CPU's entering the handler again and again. Adding DSB can prevent the issue from happening.
     */
    __DSB();
}

// Manejador de interrupciones del canal 2 del PIT
void PIT_HANDLER_ONE(void)
{
	uint32_t irq_status_1 = 0;
	irq_status_1 = PIT_GetStatusFlags(PIT_BASEADDR, PIT_CHANNEL_ONE);

	if(PIT1_callback)
	{
		PIT1_callback(irq_status_1);
	}

    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT_BASEADDR, PIT_CHANNEL_ONE, kPIT_TimerFlag);
    /* Added for, and affects, all PIT handlers. For CPU clock which is much larger than the IP bus clock,
     * CPU can run out of the interrupt handler before the interrupt flag being cleared, resulting in the
     * CPU's entering the handler again and again. Adding DSB can prevent the issue from happening.
     */
    __DSB();
}

// Manejador de interrupciones del canal 3 del PIT
void PIT_HANDLER_TWO(void)
{
	uint32_t irq_status_2 = 0;
	irq_status_2 = PIT_GetStatusFlags(PIT_BASEADDR, PIT_CHANNEL_TWO);

	if(PIT2_callback)
	{
		PIT2_callback(irq_status_2);
	}

    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT_BASEADDR, PIT_CHANNEL_TWO, kPIT_TimerFlag);
    /* Added for, and affects, all PIT handlers. For CPU clock which is much larger than the IP bus clock,
     * CPU can run out of the interrupt handler before the interrupt flag being cleared, resulting in the
     * CPU's entering the handler again and again. Adding DSB can prevent the issue from happening.
     */
    __DSB();
}

// Manejador de interrupciones del canal 4 del PIT
void PIT_HANDLER_THREE(void)
{
	uint32_t irq_status_3 = 0;
	irq_status_3 = PIT_GetStatusFlags(PIT_BASEADDR, PIT_CHANNEL_THREE);

	if(PIT3_callback)
	{
		PIT3_callback(irq_status_3);
	}

    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT_BASEADDR, PIT_CHANNEL_THREE, kPIT_TimerFlag);
    /* Added for, and affects, all PIT handlers. For CPU clock which is much larger than the IP bus clock,
     * CPU can run out of the interrupt handler before the interrupt flag being cleared, resulting in the
     * CPU's entering the handler again and again. Adding DSB can prevent the issue from happening.
     */
    __DSB();
}


// Configuracion del PIT
void PIT_configure(void){

	pit_config_t pitConfig;


    PIT_GetDefaultConfig(&pitConfig);

    // Se habilita el modulo del PIT
    PIT_Init(PIT_BASEADDR, &pitConfig);

    // Se establece el periodo para el primer PIT
    PIT_SetTimerPeriod(PIT_BASEADDR,PIT_CHANNEL, USEC_TO_COUNT(62U, PIT_SOURCE_CLOCK));



//config del segundo PIT para el retardo de sonidos
    PIT_SetTimerPeriod(PIT_BASEADDR,PIT_CHANNEL_ONE, USEC_TO_COUNT(250000U, PIT_SOURCE_CLOCK));

    PIT_EnableInterrupts(PIT_BASEADDR, PIT_CHANNEL_ONE, kPIT_TimerInterruptEnable);

    EnableIRQ(PIT_IRQ_ID_ONE);

//config del tercer PIT para el watchdog
    PIT_SetTimerPeriod(PIT_BASEADDR,PIT_CHANNEL_TWO, USEC_TO_COUNT(90000U, PIT_SOURCE_CLOCK));

    PIT_EnableInterrupts(PIT_BASEADDR, PIT_CHANNEL_TWO, kPIT_TimerInterruptEnable);

    EnableIRQ(PIT_IRQ_ID_TWO);


    //config del tercer PIT para el watchdog
    PIT_SetTimerPeriod(PIT_BASEADDR,PIT_CHANNEL_THREE, USEC_TO_COUNT(82U, PIT_SOURCE_CLOCK));

    PIT_EnableInterrupts(PIT_BASEADDR, PIT_CHANNEL_THREE, kPIT_TimerInterruptEnable);

    EnableIRQ(PIT_IRQ_ID_THREE);




}


void PIT_callback_init(pit_chnl_t channel, void (*handler)(uint32_t flags))
{
	if(PIT_CHANNEL == channel)
	{
		PIT0_callback = handler;
	}

	else if (PIT_CHANNEL_ONE == channel)
	{
		PIT1_callback = handler;
	}

	else if (PIT_CHANNEL_TWO == channel)
	{
		PIT2_callback = handler;
	}

	else if (PIT_CHANNEL_THREE == channel)
	{
		PIT3_callback = handler;
	}


}
