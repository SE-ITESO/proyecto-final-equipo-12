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

// Para establecer numeros de pin y funcionalidades en GPIOs
#define CLEAR_VALUE 0xFFFF
#define PIN_0 0U
#define PIN_1 1U
#define PIN_2 2U
#define PIN_3 3U
#define PIN_4 4U
#define PIN_5 5U
#define PIN_6 6U
#define PINOCHO 8U
#define PIN_9 9U
#define PIN_10 10U
#define PIN_11 11U
#define PIN_19 19U
#define PIN_18 18U
#define PIN_16 16U
#define PIN_23 23U
#define GPIO_STOPWATCH_MODE 1U
#define GPIO_CLOCK_MODE 0U
#define BUTTON_PRESSED 0U
#define SET_AS_OUTPUT 1
#define SET_AS_INPUT 0
#define SET 1

// Estructura para evaluar estado de switches
typedef struct
{
	uint8_t sw2;
	uint8_t sw3;

}switch_pressed_t;


// Estructura para contener funcionalidades de los GPIOs
typedef struct
{
	uint32_t PDOR;
	uint32_t PSOR;
	uint32_t PCOR;
	uint32_t PTOR;
	uint32_t PDIR;
	uint32_t PDDR;

}GPIO_t;

// Estructura para utilizar banderas en distintos puertos
typedef struct
{
	uint8_t PORTA_F;
	uint8_t PORTB_F;
	uint8_t PORTC_F;
	uint8_t PORTD_F;
	uint8_t PORTE_F;

}Interrupt_Flags_t;

// Estructura para establecer la bandera segun la funcionalidad
typedef struct
{
	uint8_t manual_mode_f;
	uint8_t sequence_mode_f;
	uint8_t kick;
	uint8_t snare;
	uint8_t bongo;
	uint8_t hihat;
	uint8_t guiro;
	uint8_t rimshot;
	uint8_t tambor;
	uint8_t cynbal;
	uint8_t cowbell;

}Function_Flags_t;


// Apuntador a la direccion base de cada GPIO
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

// Para establecer a que manejador de interrupcion le corresponde cada switch
#define SW3_INT PORTA_IRQHandler
#define SW2_INT PORTC_IRQHandler

// Direccion del pin (0 entrada, 1 salida)
void GPIO_set_direction(GPIO_t * BASE, uint32_t pin, uint8_t out);
// Salida del pin
void GPIO_set_output(GPIO_t * BASE, uint32_t pin, uint8_t out);
// Configurar el pin, 1, se pone en 1, 0, no cambia
void GPIO_set_PSOR(GPIO_t * BASE, uint32_t pin, uint8_t out);
// Poner en 0 el pin
void GPIO_clear_pin(GPIO_t * BASE, uint32_t pin);
// Si el pin esta en 1, ponerlo en 0 y viceversa
void GPIO_PTOR(GPIO_t * BASE, uint32_t pin, uint8_t out);
// Leer un pin de entrada especifico
uint8_t GPIO_PDIR(GPIO_t * BASE, uint32_t pin);

//gets y sets para interrupciones
uint8_t GPIO_gets(GPIO_t * BASE);
void GPIO_sets(GPIO_t * BASE);

//gets y sets para switches
uint8_t GPIO_getsw(GPIO_t * BASE);
void GPIO_setsw(GPIO_t * BASE);

//interrupt handlers
//void PORTA_IRQHandler(void);
//void PORTB_IRQHandler(void);
//void PORTC_IRQHandler(void);
//void PORTD_IRQHandler(void);
//void PORTE_IRQHandler(void);

// Gets y sets para las funcionalidades
uint8_t GPIO_getf(GPIO_t * BASE, uint8_t pin);
void GPIO_setf(GPIO_t * BASE, uint8_t pin);

// Funcion para manejar bandera para el handler de interrupcion
void GPIO_mode_flag_config (void);


#endif /* GPIO_H_ */
