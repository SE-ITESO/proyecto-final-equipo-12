/*
 * state_machines.c
 *
 *  Created on: 3 Nov 2023
 *      Author: eldangus
 */
#include <mixer.h>
#include "stdint.h"
#include "GPIO.h"
#include "LCD_nokia.h"
#include <stdbool.h>
#include "samples.h"
#include "dma.h"
#include "PIT.h"
#include "LCD_nokia_images.h"
#include "wdog.h"

static uint8_t wdog = 0U;

static uint8_t sequence_array[20] = {0};
static uint8_t interrupt_pit_flag = 0U;

static uint8_t welcome_passed_flag = 0U;

State_name_t current_state = WELCOME;

uint8_t string_mode[]="MODOS";
uint8_t string_manual_mode[]="1)Manual";
uint8_t string_sequence_mode[]="2)Secuencia";

uint8_t string_modo_manual[]="MODO MANUAL";
uint8_t string_presione[] = "Presione";
uint8_t string_cualquier[]= "cualquier";
uint8_t string_boton[]= "boton";
uint8_t string_para_flow[]= "para flow";

uint8_t string_modo_secuencia[]="Secuencia1";
uint8_t string_play[] = "play   ";
uint8_t string_stop[] = "stopped";


void state_machine_one(void)
{
	static uint8_t counter = 0U;
	static uint8_t i = 0U;
	switch(current_state)
	{
	case WELCOME:

		if(0U == counter)
		{
			PIT_StartTimer(PIT_BASEADDR, PIT_CHANNEL_ONE);

			if(WDOG_RESET == wdog)
			{
				//shows ERROR
				wdog_sequence();
			}
			else
			{
				//shows the band image
				welcome_sequence();
				write_note();
				load_instruments();

			}

			NVIC_DisableIRQ(PORTB_IRQ);
			NVIC_DisableIRQ(PORTC_IRQ);
		}
		counter = 1U;
		if(1U == welcome_passed_flag)
		{
			current_state = MAIN_MENU;
			counter = 0U;
			PIT_StopTimer(PIT_BASEADDR, PIT_CHANNEL_ONE);
		}

		break;
	case MAIN_MENU:

		if(0U == counter)
		{
			//show the menu
			LCD_nokia_clear();
			LCD_nokia_goto_xy(5,0);
			LCD_nokia_send_string(string_mode);
			LCD_nokia_goto_xy(0,2);
			LCD_nokia_send_string(string_manual_mode);
			LCD_nokia_goto_xy(0,3);
			LCD_nokia_send_string(string_sequence_mode);

			for(uint8_t j = 0; j < 20; j++)
			{
				sequence_array[j] = 0;
			}

			//can't play anything without selecting the mode
			NVIC_DisableIRQ(PORTB_IRQ);
			NVIC_DisableIRQ(PORTC_IRQ);
		}
		counter = 1U;

		if(true == GPIO_getf(GPIO_D, PIN_4))
		{
			current_state = MANUAL_MODE;
			counter = 0U;
			GPIO_setf(GPIO_D, PIN_4);
		}
		else if(true == GPIO_getf(GPIO_D, PIN_5))
		{
			current_state = SEQUENCE_MODE;
			counter = 0U;
			GPIO_setf(GPIO_D, PIN_5);
		}

		break;

	case MANUAL_MODE:

		if(0U == counter)
		{
			//shows manual mode
			LCD_nokia_clear();
			LCD_nokia_goto_xy(0,0);
			LCD_nokia_send_string(string_modo_manual);
			LCD_nokia_goto_xy(0,1);
			LCD_nokia_send_string(string_presione);
			LCD_nokia_goto_xy(0,2);
			LCD_nokia_send_string(string_cualquier);
			LCD_nokia_goto_xy(0,3);
			LCD_nokia_send_string(string_boton);
			LCD_nokia_goto_xy(0,4);
			LCD_nokia_send_string(string_para_flow);

			NVIC_EnableIRQ(PORTB_IRQ);
			NVIC_EnableIRQ(PORTC_IRQ);
		}
		counter = 1U;

		if(true == GPIO_gets(GPIO_A))
		{
			current_state = MAIN_MENU;
			counter = 0U;
			GPIO_sets(GPIO_A);
		}
		if(true == GPIO_getf(GPIO_B, PIN_19))
		{
			//play_kick();
			PIT_StartTimer(PIT_BASEADDR, PIT_CHANNEL_THREE);
			GPIO_setf(GPIO_B, PIN_19);
		}
		if(true == GPIO_getf(GPIO_B, PIN_18))
		{
			play_snare();

			GPIO_setf(GPIO_B, PIN_18);
		}
		if(true == GPIO_getf(GPIO_C, PIN_1))
		{
			play_bongo();

			GPIO_setf(GPIO_C, PIN_1);
		}
		if(true == GPIO_getf(GPIO_C, PINOCHO))
		{
			play_hihat();

			GPIO_setf(GPIO_C, PINOCHO);
		}
		if(true == GPIO_getf(GPIO_B, PIN_23))
		{
			play_guiro();

			GPIO_setf(GPIO_B, PIN_23);
		}
		if(true == GPIO_getf(GPIO_C, PIN_9))
		{
			play_rimshot();

			GPIO_setf(GPIO_C, PIN_9);
		}
		if(true == GPIO_getf(GPIO_B, PIN_9))
		{
			play_tambor();

			GPIO_setf(GPIO_B, PIN_9);
		}
		if(true == GPIO_getf(GPIO_C, PIN_0))
		{
			play_cynbal();

			GPIO_setf(GPIO_C, PIN_0);
		}
		if(true == GPIO_getf(GPIO_C, PIN_16))
		{
			play_cowbell();

			GPIO_setf(GPIO_C, PIN_16);
		}
		break;

	case SEQUENCE_MODE:
		if(0U == counter)
		{
			LCD_nokia_clear();
			LCD_nokia_goto_xy(0,0);
			LCD_nokia_send_string(string_modo_secuencia);
			LCD_nokia_goto_xy(0,1);
			NVIC_EnableIRQ(PORTB_IRQ);
			NVIC_EnableIRQ(PORTC_IRQ);
		}
		counter = 1U;

		if(true == GPIO_gets(GPIO_A))
		{
			current_state = MAIN_MENU;
			counter = 0U;
			GPIO_sets(GPIO_A);
		}
		if(true == GPIO_getf(GPIO_B, PIN_19))
		{
			sequence_array[i] = 'K';
			LCD_nokia_send_char('K');
			if(20U > i)
			{
				i++;
			}
			else
			{
				i = 0;
			}
			GPIO_setf(GPIO_B, PIN_19);
		}
		if(true == GPIO_getf(GPIO_B, PIN_18))
		{
			sequence_array[i] = 'S';
			LCD_nokia_send_char('S');
			if(20U > i)
			{
				i++;
			}
			else
			{
				i = 0;
			}

			GPIO_setf(GPIO_B, PIN_18);
		}
		if(true == GPIO_getf(GPIO_C, PIN_1))
		{
			sequence_array[i] = 'B';
			LCD_nokia_send_char('B');
			if(20U > i)
			{
				i++;
			}
			else
			{
				i = 0;
			}

			GPIO_setf(GPIO_C, PIN_1);
		}
		if(true == GPIO_getf(GPIO_C, PINOCHO))
		{
			sequence_array[i] = 'H';
			LCD_nokia_send_char('H');
			if(20U > i)
			{
				i++;
			}
			else
			{
				i = 0;
			}

			GPIO_setf(GPIO_C, PINOCHO);
		}
		if(true == GPIO_getf(GPIO_B, PIN_23))
		{
			sequence_array[i] = 'G';
			LCD_nokia_send_char('G');
			if(20U > i)
			{
				i++;
			}
			else
			{
				i = 0;
			}

			GPIO_setf(GPIO_B, PIN_23);
		}
		if(true == GPIO_getf(GPIO_C, PIN_9))
		{
			sequence_array[i] = 'R';
			LCD_nokia_send_char('R');
			if(20U > i)
			{
				i++;
			}
			else
			{
				i = 0;
			}

			GPIO_setf(GPIO_C, PIN_9);
		}
		if(true == GPIO_getf(GPIO_B, PIN_9))
		{
			sequence_array[i] = 'T';
			LCD_nokia_send_char('T');
			if(20U > i)
			{
				i++;
			}
			else
			{
				i = 0;
			}

			GPIO_setf(GPIO_B, PIN_9);
		}
		if(true == GPIO_getf(GPIO_C, PIN_0))
		{
			sequence_array[i] = 'C';
			LCD_nokia_send_char('C');
			if(20U > i)
			{
				i++;
			}
			else
			{
				i = 0;
			}

			GPIO_setf(GPIO_C, PIN_0);
		}
		if(true == GPIO_getf(GPIO_C, PIN_16))
		{
			sequence_array[i] = 'c';
			LCD_nokia_send_char('c');
			if(20U > i)
			{
				i++;
			}
			else
			{
				i = 0;
			}

			GPIO_setf(GPIO_C, PIN_16);
		}

		if(true == GPIO_getf(GPIO_D, PIN_4))
		{
			current_state = PLAY_SEQUENCE;
			PIT_StartTimer(PIT_BASEADDR, PIT_CHANNEL_ONE);
			counter = 0U;
			GPIO_setf(GPIO_D, 4U);
			GPIO_setf(GPIO_D, 5U);
		}

		break;

	case PLAY_SEQUENCE:
		if(0U == counter)
		{
			LCD_nokia_goto_xy(0,5);
			LCD_nokia_send_string(string_play);
		}
		counter = 1U;
		if(0 != sequence_array[i] && 1U == interrupt_pit_flag)
		{
			play_sequence(sequence_array[i]);
			interrupt_pit_flag = 0U;
			i++;
		}
		else if(0 == sequence_array[i])
		{
			i = 0U;
		}

		if(true == GPIO_getf(GPIO_D, PIN_5))
		{
			current_state = STOP_SEQUENCE;
			counter = 0U;
			GPIO_setf(GPIO_D, PIN_5);
		}

		break;
	case STOP_SEQUENCE:
		if(0U == counter)
		{
			LCD_nokia_goto_xy(0,5);
			LCD_nokia_send_string(string_stop);

			PIT_StopTimer(PIT_BASEADDR, PIT_CHANNEL_ONE);
		}
		counter = 1U;

		if(true == GPIO_getf(GPIO_D, PIN_4))
		{
			current_state = PLAY_SEQUENCE;
			PIT_StartTimer(PIT_BASEADDR, PIT_CHANNEL_ONE);
			counter = 0U;
			GPIO_setf(GPIO_D, PIN_4);
		}
		if(true == GPIO_gets(GPIO_A))
		{
			current_state = MAIN_MENU;
			i = 0U;
			counter = 0U;
			GPIO_sets(GPIO_A);
		}

		break;

	}
}

void interrupt_pit(uint32_t flags)
{
	static uint8_t welcome_counter = 0U;
	interrupt_pit_flag = 1U;
	welcome_counter++;
	if (8U == welcome_counter)
	{
		welcome_passed_flag = 1U;
	}
}



void refresh_wdog(uint32_t flags)
{
	WDOG_Refresh(WDOG);
	PIT_StopTimer(PIT_BASEADDR, PIT_CHANNEL_TWO);
}

void wdog_reset(void)
{
	if(WDOG_NONRESET == wdog)
	{
		wdog = WDOG_RESET;
	}
	else
	{
		wdog = WDOG_NONRESET;
	}
}
