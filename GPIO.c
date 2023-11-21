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



//hanlders de interrupcion de puertos
void PORTA_IRQHandler(void)
{
	switches.sw3++;
	flag.PORTA_F = true;
	GPIO_PortClearInterruptFlags(GPIOA, CLEAR_VALUE);
}


void PORTB_IRQHandler(void)
{
	flag.PORTB_F = true;

	if (PORTB->ISFR & (1 << PIN_19))
	{
		function.kick = true;
		PORTB->ISFR |= (1 << PIN_19);
	}
	if (PORTB->ISFR & (1 << PIN_18))
	{
		function.snare = true;
		PORTB->ISFR |= (1 << PIN_18);
	}
	if (PORTB->ISFR & (1 << PIN_23))
	{
		function.guiro = true;
		PORTB->ISFR |= (1 << PIN_23);
	}
	if (PORTB->ISFR & (1 << PIN_9))
	{
		function.tambor = true;
		PORTB->ISFR |= (1 << PIN_9);
	}

	GPIO_PortClearInterruptFlags(GPIOB, CLEAR_VALUE);

}

void PORTC_IRQHandler(void)
{

	flag.PORTC_F = true;
	if (PORTC->ISFR & (1 << PIN_1))
	{
		function.bongo = true;
		PORTC->ISFR |= (1 << PIN_1);
	}
	if (PORTC->ISFR & (1 << PINOCHO))
	{
		function.hihat = true;
		PORTC->ISFR |= (1 << PINOCHO);
	}
	if (PORTC->ISFR & (1 << PIN_9))
	{
		function.rimshot = true;
		PORTC->ISFR |= (1 << PIN_9);
	}
	if (PORTC->ISFR & (1 << PIN_0))
	{
		function.cynbal = true;
		PORTC->ISFR |= (1 << PIN_0);
	}
	if (PORTC->ISFR & (1 << PIN_16))
	{
		function.cowbell = true;
		PORTC->ISFR |= (1 << PIN_16);
	}




	GPIO_PortClearInterruptFlags(GPIOC, CLEAR_VALUE);
}


void PORTD_IRQHandler(void)
{
	flag.PORTD_F = true;

	if (PORTD->ISFR & (1 << PIN_4))
	{
		function.manual_mode_f = true;
		PORTD->ISFR |= (1 << PIN_4);
	}
	if (PORTD->ISFR & (1 << PIN_5))
	{
		function.sequence_mode_f = true;
		PORTD->ISFR |= (1 << PIN_5);
	}

	GPIO_PortClearInterruptFlags(GPIOD, CLEAR_VALUE);

}

void PORTE_IRQHandler(void)
{
	flag.PORTE_F = true;
	GPIO_PortClearInterruptFlags(GPIOE, CLEAR_VALUE);

}



uint8_t GPIO_gets(GPIO_t * BASE)
{
	uint8_t s = 0u;//status
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

// Gets para las funciones del menu

uint8_t GPIO_getf(GPIO_t * BASE, uint8_t pin)
{
	// Funcion
	uint8_t f = 0u;
	if (GPIO_A == BASE){


	}
	if (GPIO_B == BASE){
		if (PIN_19 == pin)
		{
			f = function.kick;
		}
		if (PIN_18 == pin)
		{
			f = function.snare;
		}
		if (PIN_23 == pin)
		{
			f = function.guiro;
		}
		if (PIN_9 == pin)
		{
			f = function.tambor;
		}

	}
	if (GPIO_C == BASE){
		if (PIN_1 == pin)
		{
			f = function.bongo;
		}
		if (PINOCHO == pin)
		{
			f = function.hihat;
		}
		if (PIN_9 == pin)
		{
			f = function.rimshot;
		}
		if (PIN_0 == pin)
		{
			f = function.cynbal;
		}
		if (PIN_16 == pin)
		{
			f = function.cowbell;
		}

	}
	if (GPIO_D == BASE){
		if (PIN_4 == pin)
		{
			f = function.manual_mode_f;
		}
		if (PIN_5 == pin)
		{
			f = function.sequence_mode_f;
		}
	}
	if (GPIO_E == BASE){

	}

	return f;

}

void GPIO_setf(GPIO_t * BASE, uint8_t pin)
{
	if (GPIO_A == BASE){


	}
	if (GPIO_B == BASE){
		if (PIN_19 == pin)
		{
			function.kick = false;
		}
		if (PIN_18 == pin)
		{
			function.snare = false;
		}
		if (PIN_23 == pin)
		{
			function.guiro = false;
		}
		if (PIN_9 == pin)
		{
			function.tambor = false;
		}


	}
	if (GPIO_C == BASE){
		if (PIN_1 == pin)
		{
			function.bongo = false;
		}
		if (PINOCHO == pin)
		{
			function.hihat = false;
		}
		if (PIN_9 == pin)
		{
			function.rimshot = false;
		}
		if (PIN_0 == pin)
		{
			function.cynbal = false;
		}
		if (PIN_16 == pin)
		{
			function.cowbell = false;
		}
	}
	if (GPIO_D == BASE){
		if (PIN_4 == pin)
		{
			function.manual_mode_f = false;
		}
		if (PIN_5 == pin)
		{
			function.sequence_mode_f = false;
		}
	}
	if (GPIO_E == BASE){

	}

}




// Para leer el estado de los switches

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
