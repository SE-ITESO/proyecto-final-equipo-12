/*
 * PIT.c
 *
 *  Created on: 15 Sept 2023
 *      Author: Alejandro De La Rosa & Daniel Gutierrez
 */


#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_pit.h"
#include "PIT.h"
#include "NVIC.h"


static void (*PIT0_callback)(uint32_t flags) = 0;
static void (*PIT1_callback)(uint32_t flags_1) = 0;
static void (*PIT2_callback)(uint32_t flags_2) = 0;
static void (*PIT3_callback)(uint32_t flags_3) = 0;

/* PIT Channel 0 interrupt handler */
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

/* PIT Channel 1 interrupt handler */
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

/* PIT Channel 2 interrupt handler */
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

/* PIT Channel 3 interrupt handler */
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


/* PIT Configuration, initialization and enable settings */
void PIT_configure(void){

	pit_config_t pitConfig;


    PIT_GetDefaultConfig(&pitConfig);

    /* PIT module enable */
    PIT_Init(PIT_BASEADDR, &pitConfig);

    /* First PIT period setting */
    PIT_SetTimerPeriod(PIT_BASEADDR,PIT_CHANNEL, USEC_TO_COUNT(62U, PIT_SOURCE_CLOCK));



    /* Second PIT configuration for sound delay */
    PIT_SetTimerPeriod(PIT_BASEADDR,PIT_CHANNEL_ONE, USEC_TO_COUNT(250000U, PIT_SOURCE_CLOCK));

    PIT_EnableInterrupts(PIT_BASEADDR, PIT_CHANNEL_ONE, kPIT_TimerInterruptEnable);

    EnableIRQ(PIT_IRQ_ID_ONE);

    /* Second PIT configuration */
    PIT_SetTimerPeriod(PIT_BASEADDR,PIT_CHANNEL_TWO, USEC_TO_COUNT(125000U, PIT_SOURCE_CLOCK));

    PIT_EnableInterrupts(PIT_BASEADDR, PIT_CHANNEL_TWO, kPIT_TimerInterruptEnable);

    EnableIRQ(PIT_IRQ_ID_TWO);


    /* Third PIT configuration for sample rate */
    PIT_SetTimerPeriod(PIT_BASEADDR,PIT_CHANNEL_THREE, USEC_TO_COUNT(41U, PIT_SOURCE_CLOCK));

    PIT_EnableInterrupts(PIT_BASEADDR, PIT_CHANNEL_THREE, kPIT_TimerInterruptEnable);

    EnableIRQ(PIT_IRQ_ID_THREE);



}

/* PIT callback initialization function, contain all PIT used channels*/
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
