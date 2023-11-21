/*
 * configure.c
 *
 *  Created on: 8 Nov 2023
 *      Author: Alex
 */

#include "stdint.h"
#include "fsl_port.h"
#include "GPIO.h"
#include "configure.h"



uint16_t wdog_reset_count;
static WDOG_Type *wdog_base = WDOG;
static RCM_Type *rcm_base   = RCM;

const port_pin_config_t button_config = {
		kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
		kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
		kPORT_PassiveFilterEnable,                              /* Passive filter is disabled */
		kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
		kPORT_HighDriveStrength,                                 /* High drive strength is configured */
		kPORT_MuxAsGpio,                                         /* Pin is configured as PTA4 */
		kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
	  };


void configure_port(void)
{
	PORT_SetPinConfig(PORTA, 1U, &button_config);
	PORT_SetPinInterruptConfig(PORTA, 1U, kPORT_InterruptFallingEdge);

	PORT_SetPinConfig(PORTD, 4U, &button_config);
	PORT_SetPinInterruptConfig(PORTD, 4U, kPORT_InterruptFallingEdge);

	PORT_SetPinConfig(PORTD, 5U, &button_config);
	PORT_SetPinInterruptConfig(PORTD, 5U, kPORT_InterruptFallingEdge);
//KICK BUTTOWSKI
	PORT_SetPinConfig(PORTB, 19U, &button_config);
	PORT_SetPinInterruptConfig(PORTB, 19U, kPORT_InterruptFallingEdge);
//snare
	PORT_SetPinConfig(PORTB, 18U, &button_config);
	PORT_SetPinInterruptConfig(PORTB, 18U, kPORT_InterruptFallingEdge);
//bongo
	PORT_SetPinConfig(PORTC, 1U, &button_config);
	PORT_SetPinInterruptConfig(PORTC, 1U, kPORT_InterruptFallingEdge);
//Hi-hat
	PORT_SetPinConfig(PORTC, 8U, &button_config);
	PORT_SetPinInterruptConfig(PORTC, 8U, kPORT_InterruptFallingEdge);
//guiro
	PORT_SetPinConfig(PORTB, 23U, &button_config);
	PORT_SetPinInterruptConfig(PORTB, 23U, kPORT_InterruptFallingEdge);
//rimshot
	PORT_SetPinConfig(PORTC, 9U, &button_config);
	PORT_SetPinInterruptConfig(PORTC, 9U, kPORT_InterruptFallingEdge);
//tambor
	PORT_SetPinConfig(PORTB, 9U, &button_config);
	PORT_SetPinInterruptConfig(PORTB, 9U, kPORT_InterruptFallingEdge);
//cynbal
	PORT_SetPinConfig(PORTC, 0U, &button_config);
	PORT_SetPinInterruptConfig(PORTC, 0U, kPORT_InterruptFallingEdge);
//cowbell
	PORT_SetPinConfig(PORTC, 16U, &button_config);
	PORT_SetPinInterruptConfig(PORTC, 16U, kPORT_InterruptFallingEdge);

}
void configure_gpio(void)
{
	GPIO_set_direction(GPIO_D, 4U, INPUT);
	GPIO_set_direction(GPIO_D, 5U, INPUT);
	GPIO_set_direction(GPIO_A, 1U, INPUT);


	GPIO_set_direction(GPIO_B, 19U, INPUT);
	GPIO_set_direction(GPIO_B, 18U, INPUT);
	GPIO_set_direction(GPIO_C, 1U, INPUT);
	GPIO_set_direction(GPIO_C, 8U, INPUT);
	GPIO_set_direction(GPIO_B, 23U, INPUT);
	GPIO_set_direction(GPIO_C, 9U, INPUT);
	GPIO_set_direction(GPIO_B, 9U, INPUT);
	GPIO_set_direction(GPIO_C, 0U, INPUT);
	GPIO_set_direction(GPIO_C, 16U, INPUT);

}

