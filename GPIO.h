/*
 * GPIO.h
 *
 *  Created on: 29 Aug 2023
 *      Author: DanielG
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>
#include "MK64F12.h"

/* To set pin number and GPIO functionalities*/
#define CLEAR_VALUE 0xFFFF
#define PIN_0 0U
#define PIN_1 1U
#define PIN_2 2U
#define PIN_3 3U
#define PIN_4 4U
#define PIN_5 5U
#define PIN_6 6U
#define PIN_7 7U
#define PINOCHO 8U
#define PIN_9 9U
#define PIN_10 10U
#define PIN_11 11U
#define PIN_12 12U
#define PIN_19 19U
#define PIN_18 18U
#define PIN_15 15U
#define PIN_16 16U
#define PIN_17 17U
#define PIN_20 20U
#define PIN_23 23U
#define GPIO_STOPWATCH_MODE 1U
#define GPIO_CLOCK_MODE 0U
#define BUTTON_PRESSED 0U
#define SET_AS_OUTPUT 1
#define SET_AS_INPUT 0
#define SET 1

/* To evaluate sw state */
typedef struct
{
	uint8_t sw2;
	uint8_t sw3;

}switch_pressed_t;


/* Structure to contain GPIO functionalities */
typedef struct
{
	uint32_t PDOR;
	uint32_t PSOR;
	uint32_t PCOR;
	uint32_t PTOR;
	uint32_t PDIR;
	uint32_t PDDR;

}GPIO_t;

/* Structure to use flags on different ports */
typedef struct
{
	uint8_t PORTA_F;
	uint8_t PORTB_F;
	uint8_t PORTC_F;
	uint8_t PORTD_F;
	uint8_t PORTE_F;

}Interrupt_Flags_t;

/* Structure to set the flag according to the functionality */
typedef struct
{
	uint8_t main_menu;
	uint8_t piano;
	uint8_t bass;
	uint8_t guitar;
	uint8_t clean_mode_f;
	uint8_t fx_mode_f;
	uint8_t C;
	uint8_t C_sharp;
	uint8_t D;
	uint8_t D_sharp;
	uint8_t E;
	uint8_t F;
	uint8_t F_sharp;
	uint8_t G;
	uint8_t G_sharp;
	uint8_t A;
	uint8_t A_sharp;
	uint8_t B;
	uint8_t C2;

}Function_Flags_t;


/* Pointer to the base address of each GPIO */
#define GPIO_A_BASE_ADDR (((volatile uint32_t *) 0x400FF000))
#define GPIO_B_BASE_ADDR (((volatile uint32_t *) 0x400FF040))
#define GPIO_C_BASE_ADDR (((volatile uint32_t *) 0x400FF080))
#define GPIO_D_BASE_ADDR (((volatile uint32_t *) 0x400FF0C0))
#define GPIO_E_BASE_ADDR (((volatile uint32_t *) 0x400FF100))

#define GPIO_A ( (GPIO_t*) GPIO_A_BASE_ADDR)
#define GPIO_B ( (GPIO_t*) GPIO_B_BASE_ADDR)
#define GPIO_C ( (GPIO_t*) GPIO_C_BASE_ADDR)
#define GPIO_D ( (GPIO_t*) GPIO_D_BASE_ADDR)
#define GPIO_E ( (GPIO_t*) GPIO_E_BASE_ADDR)

/* To establish which interrupt handler each switch corresponds to */
#define SW3_INT PORTA_IRQHandler
#define SW2_INT PORTC_IRQHandler

/* Pin address (0 input, 1 output) */
void GPIO_set_direction(GPIO_t * BASE, uint32_t pin, uint8_t out);
/* Pin out*/
void GPIO_set_output(GPIO_t * BASE, uint32_t pin, uint8_t out);
/* Configure pin, 1, is set to 1, 0, does not change */
void GPIO_set_PSOR(GPIO_t * BASE, uint32_t pin, uint8_t out);
/* Clear pin with a 0 */
void GPIO_clear_pin(GPIO_t * BASE, uint32_t pin);
/* If the pin is at 1, set it to 0 and vice versa */
void GPIO_PTOR(GPIO_t * BASE, uint32_t pin, uint8_t out);
/* Read a specific input pin */
uint8_t GPIO_PDIR(GPIO_t * BASE, uint32_t pin);

/* gets and sets for interrupts */
uint8_t GPIO_gets(GPIO_t * BASE);
void GPIO_sets(GPIO_t * BASE);

/* gets and sets for switches */
uint8_t GPIO_getsw(GPIO_t * BASE);
void GPIO_setsw(GPIO_t * BASE);


/* Gets and sets for features */
uint8_t GPIO_getf(GPIO_t * BASE, uint8_t pin);
void GPIO_setf(GPIO_t * BASE, uint8_t pin);

/* Function to handle flag for the interrupt handler */
void GPIO_mode_flag_config (void);


#endif /* GPIO_H_ */
