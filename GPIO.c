/*
 * GPIO.c
 *
 *  Created on: 29 Aug 2023
 *      Author: DanielG
 */
#include <stdint.h>
#include "fsl_gpio.h"
#include "GPIO.h"
#include <stdbool.h>


static Interrupt_Flags_t flag;
static Function_Flags_t function;
static switch_pressed_t switches = {0};
static uint8_t Mode_Flag = 0U;


/**
 * \brief
 * This functions are made to handle interruptions
 * at ports */
void PORTA_IRQHandler(void)
{
	flag.PORTA_F = true;

	if (PORTA->ISFR & (1 << PIN_1))
	{
		function.A = true;
		PORTA->ISFR |= (1 << PIN_1);
	}
	if (PORTA->ISFR & (1 << PIN_2))
	{
		function.B = true;
		PORTA->ISFR |= (1 << PIN_2);
	}

	GPIO_PortClearInterruptFlags(GPIOA, CLEAR_VALUE);
}

/**
 * \brief
 * This functions are made to handle interruptions
 * at port B, where most GPIO pins are connected to reproduce notes */
void PORTB_IRQHandler(void)
{
	flag.PORTB_F = true;

	if (PORTB->ISFR & (1 << PIN_19))
	{
		function.C_sharp = true;
		PORTB->ISFR |= (1 << PIN_19);
	}
	if (PORTB->ISFR & (1 << PIN_18))
	{
		function.C = true;
		PORTB->ISFR |= (1 << PIN_18);
	}
	if (PORTB->ISFR & (1 << PIN_23))
	{
		function.A_sharp = true;
		PORTB->ISFR |= (1 << PIN_23);
	}
	if (PORTB->ISFR & (1 << PIN_9))
	{
		function.G_sharp = true;
		PORTB->ISFR |= (1 << PIN_9);
	}
	if (PORTB->ISFR & (1 << PIN_20))
	{
		function.main_menu = true;
		PORTB->ISFR |= (1 << PIN_20);
	}

	GPIO_PortClearInterruptFlags(GPIOB, CLEAR_VALUE);

}

/**
 * \brief
 * This functions are made to handle interruptions
 * at port C, where most GPIO pins are connected to reproduce notes */
void PORTC_IRQHandler(void)
{

	flag.PORTC_F = true;
	if (PORTC->ISFR & (1 << PIN_1))
	{
		function.D = true;
		PORTC->ISFR |= (1 << PIN_1);
	}
	if (PORTC->ISFR & (1 << PINOCHO))
	{
		function.D_sharp = true;
		PORTC->ISFR |= (1 << PINOCHO);
	}
	if (PORTC->ISFR & (1 << PIN_9))
	{
		function.E = true;
		PORTC->ISFR |= (1 << PIN_9);
	}
	if (PORTC->ISFR & (1 << PIN_0))
	{
		function.F = true;
		PORTC->ISFR |= (1 << PIN_0);
	}
	if (PORTC->ISFR & (1 << PIN_16))
	{
		function.F_sharp = true;
		PORTC->ISFR |= (1 << PIN_16);
	}
	if (PORTC->ISFR & (1 << PIN_17))
	{
		function.G = true;
		PORTC->ISFR |= (1 << PIN_17);
	}
	if (PORTC->ISFR & (1 << PIN_2))
	{
		function.C2 = true;
		PORTC->ISFR |= (1 << PIN_2);
	}

	if (PORTC->ISFR & (1 << PIN_18))
	{
		function.piano = true;
		PORTC->ISFR |= (1 << PIN_18);

	}




	GPIO_PortClearInterruptFlags(GPIOC, CLEAR_VALUE);
}
/**
 * \brief
 * This functions are made to handle interruptions
 * at port D, where most GPIO pins are connected to switch between
 * menu modes */

void PORTD_IRQHandler(void)
{
	flag.PORTD_F = true;

	if (PORTD->ISFR & (1 << PIN_4))
	{
		function.bass = true;
		PORTD->ISFR |= (1 << PIN_4);
	}
	if (PORTD->ISFR & (1 << PIN_5))
	{
		function.clean_mode_f = true;
		PORTD->ISFR |= (1 << PIN_5);
	}
	if (PORTD->ISFR & (1 << PIN_6))
	{
		function.guitar = true;
		PORTD->ISFR |= (1 << PIN_6);
	}
	if (PORTD->ISFR & (1 << PIN_7))
	{
		function.fx_mode_f = true;
		PORTD->ISFR |= (1 << PIN_7);
	}

	GPIO_PortClearInterruptFlags(GPIOD, CLEAR_VALUE);

}

void PORTE_IRQHandler(void)
{
	flag.PORTE_F = true;
	GPIO_PortClearInterruptFlags(GPIOE, CLEAR_VALUE);

}


/* Get and set functions in order to replace callbacks */
uint8_t GPIO_gets(GPIO_t * BASE)
{
	/* status flag*/
	uint8_t s = 0u;
	if (GPIO_A == BASE){
		s = flag.PORTA_F;

	}
	if (GPIO_B == BASE){
		s = flag.PORTB_F;
	}
	if (GPIO_C == BASE){
		s = flag.PORTC_F;
	}
	if (GPIO_D == BASE){
		s = flag.PORTD_F;
	}
	if (GPIO_E == BASE){
		s = flag.PORTE_F;
	}

	return s;

}



void GPIO_sets(GPIO_t * BASE)
{
	if (GPIO_A == BASE){
		flag.PORTA_F = false;

	}
	if (GPIO_B == BASE){
		flag.PORTB_F = false;
	}
	if (GPIO_C == BASE){
		flag.PORTC_F = false;

	}
	if (GPIO_D == BASE){
		flag.PORTD_F = false;
	}
	if (GPIO_E == BASE){
		flag.PORTE_F = false;
	}

}

/* Here, we control each button and associate it
 * to a functionality, in this case, a sound. */
uint8_t GPIO_getf(GPIO_t * BASE, uint8_t pin)
{

	uint8_t f = 0u;
	if (GPIO_A == BASE){

		if (PIN_1 == pin)
		{
			f = function.A;
		}
		if (PIN_2 == pin)
		{
			f = function.B;
		}

	}
	if (GPIO_B == BASE){
		if (PIN_19 == pin)
		{
			f = function.C_sharp;
		}
		if (PIN_18 == pin)
		{
			f = function.C;
		}
		if (PIN_23 == pin)
		{
			f = function.A_sharp;
		}
		if (PIN_9 == pin)
		{
			f = function.G_sharp;
		}
		if (PIN_20 == pin)
		{
			f = function.main_menu;
		}

	}
	if (GPIO_C == BASE){
		if (PIN_1 == pin)
		{
			f = function.D;
		}
		if (PINOCHO == pin)
		{
			f = function.D_sharp;
		}
		if (PIN_9 == pin)
		{
			f = function.E;
		}
		if (PIN_0 == pin)
		{
			f = function.F;
		}
		if (PIN_16 == pin)
		{
			f = function.F_sharp;
		}
		if (PIN_17 == pin)
		{
			f = function.G;
		}
		if (PIN_2 == pin)
		{
			f = function.C2;
		}

		if (PIN_18 == pin)
		{
			f = function.piano;
		}

	}
	if (GPIO_D == BASE){
		if (PIN_5 == pin)
		{
			f = function.clean_mode_f;
		}
		if (PIN_7 == pin)
		{
			f = function.fx_mode_f;
		}
		if (PIN_4 == pin)
		{
			f = function.bass;
		}
		if (PIN_6 == pin)
		{
			f = function.guitar;
		}
	}
	if (GPIO_E == BASE){

	}

	return f;

}

void GPIO_setf(GPIO_t * BASE, uint8_t pin)
{
	if (GPIO_A == BASE){
		if (PIN_1 == pin)
		{
			function.A = false;
		}
		if (PIN_2 == pin)
		{
			function.B = false;
		}

	}
	if (GPIO_B == BASE){
		if (PIN_19 == pin)
		{
			function.C_sharp = false;
		}
		if (PIN_18 == pin)
		{
			function.C = false;
		}
		if (PIN_23 == pin)
		{
			function.A_sharp = false;
		}
		if (PIN_9 == pin)
		{
			function.G_sharp = false;
		}
		if (PIN_20 == pin)
		{
			function.main_menu = false;
		}


	}
	if (GPIO_C == BASE){
		if (PIN_1 == pin)
		{
			function.D = false;
		}
		if (PINOCHO == pin)
		{
			function.D_sharp = false;
		}
		if (PIN_9 == pin)
		{
			function.E = false;
		}
		if (PIN_0 == pin)
		{
			function.F = false;
		}
		if (PIN_16 == pin)
		{
			function.F_sharp = false;
		}
		if (PIN_17 == pin)
		{
			function.G = false;
		}
		if (PIN_2 == pin)
		{
			function.C2 = false;
		}

		if (PIN_18 == pin)
		{
			function.piano = false;
		}
	}
	if (GPIO_D == BASE){
		if (PIN_4 == pin)
		{
			function.bass = false;
		}
		if (PIN_5 == pin)
		{
			function.clean_mode_f = false;
		}
		if (PIN_6 == pin)
		{
			function.guitar = false;
		}
		if (PIN_7 == pin)
		{
			function.fx_mode_f = false;
		}
	}
	if (GPIO_E == BASE){

	}

}




/* To read switches status */

uint8_t GPIO_getsw(GPIO_t * BASE)
{
	uint8_t sw = 0u;
	if (GPIO_A == BASE){
		sw = switches.sw3;

	}

	if (GPIO_C == BASE){
		sw = switches.sw2;
	}
	return sw;
}

void GPIO_setsw(GPIO_t * BASE)
{
	if (GPIO_A == BASE){
		switches.sw3 = 0;

		}

	if (GPIO_C == BASE){
		switches.sw2 = 0;
		}

}


/* Own GPIO functions */
void GPIO_set_direction(GPIO_t * BASE, uint32_t pin, uint8_t out)
{
	//BASE->PDDR = 0x00000000;
	BASE->PDDR = (out << pin) | BASE->PDDR;

}
void GPIO_set_output(GPIO_t * BASE, uint32_t pin, uint8_t out)
{
	//BASE->PDDR = 0xFFFFFFFF;
	BASE->PDOR = (out << pin) | BASE->PDOR;
}
void GPIO_set_PSOR(GPIO_t * BASE, uint32_t pin, uint8_t out)
{
	if(1U == out){
	 BASE->PSOR = (1U << pin) | BASE->PSOR;
	}

}
void GPIO_clear_pin(GPIO_t * BASE, uint32_t pin)
{

	BASE->PCOR = (1U << pin) | BASE->PCOR;


}
void GPIO_PTOR(GPIO_t * BASE, uint32_t pin, uint8_t out)
{
	if (1U == out){
	uint8_t result = (BASE ->PTOR >> (pin));
		if (1U == result){
			BASE->PTOR = (0U << pin) & BASE->PTOR;
		}
		else{
			BASE->PTOR = (1U << pin) | BASE->PTOR;
		}

	}

}

uint8_t GPIO_PDIR(GPIO_t * BASE, uint32_t pin){

	uint8_t result = (BASE ->PDIR >> (pin));
	return result;
}

/* Unused */
void GPIO_mode_flag_config (void)
{
	if(GPIO_CLOCK_MODE == Mode_Flag)
	{
		Mode_Flag = GPIO_STOPWATCH_MODE;
	}
	else
	{
		Mode_Flag = GPIO_CLOCK_MODE;
	}
}
