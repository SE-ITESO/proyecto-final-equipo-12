/*
 * configure.c
 *
 *  Created on: 8 Nov 2023
 *      Author: Alejandro De La Rosa A.
 */
#include <mixer.h>
#include "NVIC.h"
#include "stdint.h"
#include "fsl_port.h"
#include "GPIO.h"
#include "configure.h"
#include "wdog.h"

/* Variables */
uint16_t wdog_reset_count;
wdog_config_t config;

static WDOG_Type *wdog_base = WDOG;
static RCM_Type *rcm_base   = RCM;

/* Structure to set a pin with all the below configurations */
const port_pin_config_t button_config = {
		kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
		kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
		kPORT_PassiveFilterEnable,                              /* Passive filter is disabled */
		kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
		kPORT_HighDriveStrength,                                 /* High drive strength is configured */
		kPORT_MuxAsGpio,                                         /* Pin is configured as PTA4 */
		kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
	  };

/**
 * \brief
 * This function load the setting for all ports
 * and enables it´s interrupt, associated with a sound */
void configure_port(void)
{
	/* Go to "MANUAL MODE" */
	PORT_SetPinConfig(PORTD, 5U, &button_config);
	PORT_SetPinInterruptConfig(PORTD, 5U, kPORT_InterruptFallingEdge);

	/* Go to "FX MODE" */
	PORT_SetPinConfig(PORTD, 7U, &button_config);
	PORT_SetPinInterruptConfig(PORTD, 7U, kPORT_InterruptFallingEdge);

	///ESC main menu
	PORT_SetPinConfig(PORTB, 20U, &button_config);
	PORT_SetPinInterruptConfig(PORTB, 20U, kPORT_InterruptFallingEdge);

	/*Select PIANO*/
	/* Play Kick sound when pressed*/
	PORT_SetPinConfig(PORTC, 18U, &button_config);
	PORT_SetPinInterruptConfig(PORTC, 18U, kPORT_InterruptFallingEdge);

	/* Select GUITAR*/
	PORT_SetPinConfig(PORTD, 6U, &button_config);
	PORT_SetPinInterruptConfig(PORTD, 6U, kPORT_InterruptFallingEdge);

	/* Select BASS*/
	PORT_SetPinConfig(PORTD, 4U, &button_config);
	PORT_SetPinInterruptConfig(PORTD, 4U, kPORT_InterruptFallingEdge);

	/* DO*/
	PORT_SetPinConfig(PORTB, 18U, &button_config);
	PORT_SetPinInterruptConfig(PORTB, 18U, kPORT_InterruptFallingEdge);

	/* DO#*/
	PORT_SetPinConfig(PORTB, 19U, &button_config);
	PORT_SetPinInterruptConfig(PORTB, 19U, kPORT_InterruptFallingEdge);

	/* RE*/
	PORT_SetPinConfig(PORTC, 1U, &button_config);
	PORT_SetPinInterruptConfig(PORTC, 1U, kPORT_InterruptFallingEdge);

	/* RE#*/
	PORT_SetPinConfig(PORTC, 8U, &button_config);
	PORT_SetPinInterruptConfig(PORTC, 8U, kPORT_InterruptFallingEdge);

	/* MI*/
	PORT_SetPinConfig(PORTC, 9U, &button_config);
	PORT_SetPinInterruptConfig(PORTC, 9U, kPORT_InterruptFallingEdge);

	/* FA*/
	PORT_SetPinConfig(PORTC, 0U, &button_config);
	PORT_SetPinInterruptConfig(PORTC, 0U, kPORT_InterruptFallingEdge);

	/* FA#*/
	PORT_SetPinConfig(PORTC, 16U, &button_config);
	PORT_SetPinInterruptConfig(PORTC, 16U, kPORT_InterruptFallingEdge);

	/* SOL*/
	PORT_SetPinConfig(PORTC, 17U, &button_config);
	PORT_SetPinInterruptConfig(PORTC, 17U, kPORT_InterruptFallingEdge);

	/* SOL#*/
	PORT_SetPinConfig(PORTB, 9U, &button_config);
	PORT_SetPinInterruptConfig(PORTB, 9U, kPORT_InterruptFallingEdge);

	/* LA*/
	PORT_SetPinConfig(PORTA, 1U, &button_config);
	PORT_SetPinInterruptConfig(PORTA, 1U, kPORT_InterruptFallingEdge);

	/* LA#*/
	PORT_SetPinConfig(PORTB, 23U, &button_config);
	PORT_SetPinInterruptConfig(PORTB, 23U, kPORT_InterruptFallingEdge);

	/* SI*/
	PORT_SetPinConfig(PORTA, 2U, &button_config);
	PORT_SetPinInterruptConfig(PORTA, 2U, kPORT_InterruptFallingEdge);

	/* DO2*/
	PORT_SetPinConfig(PORTC, 2U, &button_config);
	PORT_SetPinInterruptConfig(PORTC, 2U, kPORT_InterruptFallingEdge);

}
/**
 * \brief
 * This function configures GPIO Pins as inputs */
void configure_gpio(void)
{
	/* Switch between states action buttons */
	GPIO_set_direction(GPIO_D, 4U, INPUT);
	GPIO_set_direction(GPIO_D, 5U, INPUT);
	GPIO_set_direction(GPIO_A, 1U, INPUT);

	/* play sounds action buttons */
	GPIO_set_direction(GPIO_B, 19U, INPUT);
	GPIO_set_direction(GPIO_B, 18U, INPUT);
	GPIO_set_direction(GPIO_C, 1U, INPUT);
	GPIO_set_direction(GPIO_C, 8U, INPUT);
	GPIO_set_direction(GPIO_B, 23U, INPUT);
	GPIO_set_direction(GPIO_C, 9U, INPUT);
	GPIO_set_direction(GPIO_B, 9U, INPUT);
	GPIO_set_direction(GPIO_C, 0U, INPUT);
	GPIO_set_direction(GPIO_C, 16U, INPUT);
	GPIO_set_direction(GPIO_C, 17U, INPUT);
	GPIO_set_direction(GPIO_A, 2U, INPUT);
	GPIO_set_direction(GPIO_C, 2U, INPUT);
	GPIO_set_direction(GPIO_B, 20U, INPUT);
	GPIO_set_direction(GPIO_D, 7U, INPUT);
	GPIO_set_direction(GPIO_D, 6U, INPUT);
	GPIO_set_direction(GPIO_C, 18U, INPUT);

}

/**
 * \brief
 * This function configures the NVIC.
 * Sets the priority and enables interrupts at NVIC for the PORT
 * action buttons, and their relation & use in the state machines
 * (which doesn't use get and set functions) */
void configure_nvic(void)
{
	NVIC_enable_interrupt_and_priotity(PORTD_IRQ, 3);
	NVIC_enable_interrupt_and_priotity(PORTA_IRQ, 2);
	NVIC_enable_interrupt_and_priotity(PORTB_IRQ, 4);
	NVIC_enable_interrupt_and_priotity(PORTC_IRQ, 5);

	NVIC_global_enable_interrupts;
}

/**
 * \brief
 * This function configures the watchdog
 * based on the previous reset source and the number of watchdog resets. */
void configure_wdog(void)
{

	wdog_reset_count = 0;

	if (!(RCM_GetPreviousResetSources(rcm_base) & kRCM_SourceWdog))
	{
		WDOG_ClearResetCount(wdog_base);
	}
	wdog_reset_count = WDOG_GetResetCount(wdog_base);
	if (wdog_reset_count < 1)
	{
		WDOG_GetDefaultConfig(&config);
		config.timeoutValue     = TIMEOUT_VALUE_100mS;
		WDOG_Init(wdog_base, &config);
		WaitWctClose(wdog_base);
	}
	else if (wdog_reset_count >= 1)
	{
		WDOG_GetDefaultConfig(&config);
		config.timeoutValue     = TIMEOUT_VALUE_100mS;
		WDOG_Init(wdog_base, &config);
		WaitWctClose(wdog_base);
		wdog_reset();
	}

}
