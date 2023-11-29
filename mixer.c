/*
 * state_machines.c
 *
 *  Created on: 3 Nov 2023
 *      Author: Daniel Gutierrez
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

static uint8_t guitar_mode = 0U;
static uint8_t piano_mode = 0U;
static uint8_t bass_mode = 0U;

State_name_t current_state = WELCOME;


/* Strings to be deployed and shown in the LCD Screen */

uint8_t string_instruments[]="INSTRUMENTOS";
uint8_t string_piano[]="1)Piano";
uint8_t string_guitar[]="2)Guitarra";
uint8_t string_bass[]="3)Bajo";

uint8_t string_rythms[]="RITMOS";
uint8_t string_rock[]="1)Rock";
uint8_t string_hiphop[]="2)Hip-Hop";
uint8_t string_latino[]="3)Latino";

uint8_t string_PIANO[]="PIANO";
uint8_t string_GUITAR[]="GUITARRA";
uint8_t string_BASS[]="BAJO";

uint8_t string_loading[]="Cargando...";

uint8_t string_DO[]="DO  ";
uint8_t string_DO_s[]="DO#";
uint8_t string_RE[]="RE   ";
uint8_t string_RE_s[]="RE#";
uint8_t string_MI[]="MI   ";
uint8_t string_FA[]="FA   ";
uint8_t string_FA_s[]="FA#";
uint8_t string_SOL[]="SOL   ";
uint8_t string_SOL_s[]="SOL#";
uint8_t string_LA[]="LA   ";
uint8_t string_LA_s[]="LA#";
uint8_t string_SI[]="SI   ";
uint8_t string_DO2[]="DO 2   ";

uint8_t string_mode[]="MODOS";
uint8_t string_free_mode[]="1)Libre";
uint8_t string_rythm_mode[]="2)Ritmo";

uint8_t string_modo_manual[]="MODO MANUAL";
uint8_t string_presione[] = "Presione";
uint8_t string_cualquier[]= "cualquier";
uint8_t string_boton[]= "boton";
uint8_t string_para_flow[]= "para flow";

uint8_t string_modo_secuencia[]="Secuencia1";
uint8_t string_play[] = "play   ";
uint8_t string_stop[] = "stopped";

/**
 * \brief
 * This function models the main state machine,
 * controlling the behavior of what will be shown on the screen
 * depending on the action of the buttons using get and set */

/* changes state by going to other sub-menus and calling functions
 * that execute specific interrupts, such as sounds, buffering,
 * and screen printing of characters and strings.
 * Everything controlled using states*/
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
				/* shows ERROR */
				wdog_sequence();
			}
			else
			{
				/* shows the band image */
				welcome_sequence();
			}

		}
		counter = 1U;
		if(1U == welcome_passed_flag)
		{
			current_state = MAIN_MENU;
			counter = 0U;
			PIT_StopTimer(PIT_BASEADDR, PIT_CHANNEL_ONE);
		}

		break;

	/* "MAIN MENU" state, here, you can transition to 2 more states,
	 * depending on the button selection,
	 * (GPIOD, pin 4 goes to "MANUAL MODE" state), and
	 * (GPIOD, pin 5 goes to "SEQUENCE MODE" state) */
	case MAIN_MENU:

		if(0U == counter)
		{
			/* show the menu */
			LCD_nokia_clear();
			LCD_nokia_goto_xy(0,0);
			LCD_nokia_send_string(string_instruments);
			LCD_nokia_goto_xy(0,2);
			LCD_nokia_send_string(string_piano);
			LCD_nokia_goto_xy(0,3);
			LCD_nokia_send_string(string_guitar);
			LCD_nokia_goto_xy(0,4);
			LCD_nokia_send_string(string_bass);

			for(uint8_t j = 0; j < 20; j++)
			{
				sequence_array[j] = 0;
			}

			/* can't play anything without
			selecting the mode */
		}
		counter = 1U;

		if(true == GPIO_getf(GPIO_C, PIN_18))
		{
			piano_mode = 1U;
			guitar_mode = 0U;
			bass_mode = 0U;
			LCD_nokia_clear();
			LCD_nokia_goto_xy(10,2);
			LCD_nokia_send_string(string_loading);
			load_piano();

			current_state = SELECT_MODE;
			counter = 0U;
			GPIO_setf(GPIO_C, PIN_18);
		}
		else if(true == GPIO_getf(GPIO_D, PIN_6))
		{
			guitar_mode = 1U;
			piano_mode = 0U;
			bass_mode = 0U;
			LCD_nokia_clear();
			LCD_nokia_goto_xy(10,2);
			LCD_nokia_send_string(string_loading);
			load_guitar();
			current_state = SELECT_MODE;
			counter = 0U;
			GPIO_setf(GPIO_D, PIN_6);
		}
		else if(true == GPIO_getf(GPIO_D, PIN_4))
		{
			bass_mode = 1U;
			piano_mode = 0U;
			guitar_mode = 0U;
			LCD_nokia_clear();
			LCD_nokia_goto_xy(10,2);
			LCD_nokia_send_string(string_loading);
			load_bass();
			current_state = SELECT_MODE;
			counter = 0U;
			GPIO_setf(GPIO_D, PIN_4);
		}


		break;

	/* If you press button associated to GPIOD, pin 4,
	 * you should transition to this state, here, you can press
	 * any sound-related-button, any time without any memory restriction.*/

	/* That means that, if the program goes to the "MANUAL MODE" state,
		 * the get and set of the GPIO pin corresponding to each sound,
		 * and where depending on which one has been pressed,
		 * the function that contains the sound to be played
		 * will be called
		 */

	case SELECT_MODE:
		if(0U == counter)
		{
			/* show the menu */
			LCD_nokia_clear();
			LCD_nokia_goto_xy(5,0);
			LCD_nokia_send_string(string_mode);
			LCD_nokia_goto_xy(0,2);
			LCD_nokia_send_string(string_free_mode);
			LCD_nokia_goto_xy(0,3);
			LCD_nokia_send_string(string_rythm_mode);


		}
		counter = 1U;

		if(true == GPIO_getf(GPIO_D, PIN_5))
		{

			current_state = CLEAN_MODE;
			counter = 0U;
			GPIO_setf(GPIO_D, PIN_5);
		}
		else if(true == GPIO_getf(GPIO_D, PIN_7))
		{
			current_state = RYTHM_MODE;
			counter = 0U;

			GPIO_setf(GPIO_D, PIN_7);
		}



		break;
	case CLEAN_MODE:

		if(0U == counter)
		{
			/* shows manual mode */
			LCD_nokia_clear();
			LCD_nokia_goto_xy(0,0);
			if(1 == piano_mode)
			{
				LCD_nokia_send_string(string_PIANO);
			}
			else if(1 == guitar_mode)
			{
				LCD_nokia_send_string(string_GUITAR);
			}
			else if(1 == bass_mode)
			{
				LCD_nokia_send_string(string_BASS);
			}

			GPIO_setf(GPIO_B, PIN_20);
		}
		counter = 1U;

		/* During MANUAL MODE state, the program will go
		 * to the corresponding "if" condition, which is directly
		 * related to a GPIO action (a pressed button)*/

		if(true == GPIO_getf(GPIO_B, PIN_18))
		{
			LCD_nokia_goto_xy(38,3);
			LCD_nokia_send_string(string_DO);
			play_do();

			GPIO_setf(GPIO_B, PIN_18);
		}
		if(true == GPIO_getf(GPIO_B, PIN_19))
		{
			LCD_nokia_goto_xy(38,3);
			LCD_nokia_send_string(string_DO_s);

			play_do_s();

			GPIO_setf(GPIO_B, PIN_19);
		}
		if(true == GPIO_getf(GPIO_C, PIN_1))
		{
			LCD_nokia_goto_xy(38,3);
			LCD_nokia_send_string(string_RE);
			play_re();
			GPIO_setf(GPIO_C, PIN_1);
		}
		if(true == GPIO_getf(GPIO_C, PINOCHO))
		{
			LCD_nokia_goto_xy(38,3);
			LCD_nokia_send_string(string_RE_s);
			play_re_s();
			GPIO_setf(GPIO_C, PINOCHO);
		}
		if(true == GPIO_getf(GPIO_C, PIN_9))
		{
			LCD_nokia_goto_xy(38,3);
			LCD_nokia_send_string(string_MI);
			play_mi();
			GPIO_setf(GPIO_C, PIN_9);
		}
		if(true == GPIO_getf(GPIO_C, PIN_0))
		{
			LCD_nokia_goto_xy(38,3);
			LCD_nokia_send_string(string_FA);
			play_fa();
			GPIO_setf(GPIO_C, PIN_0);
		}
		if(true == GPIO_getf(GPIO_C, PIN_16))
		{
			LCD_nokia_goto_xy(38,3);
			LCD_nokia_send_string(string_FA_s);
			play_fa_s();
			GPIO_setf(GPIO_C, PIN_16);
		}
		if(true == GPIO_getf(GPIO_C, PIN_17))
		{
			LCD_nokia_goto_xy(38,3);
			LCD_nokia_send_string(string_SOL);
			play_sol();
			GPIO_setf(GPIO_C, PIN_17);
		}
		if(true == GPIO_getf(GPIO_B, PIN_9))
		{
			LCD_nokia_goto_xy(38,3);
			LCD_nokia_send_string(string_SOL_s);
			play_sol_s();
			GPIO_setf(GPIO_B, PIN_9);
		}

		if(true == GPIO_getf(GPIO_A, PIN_1))
		{
			LCD_nokia_goto_xy(38,3);
			LCD_nokia_send_string(string_LA);
			play_la();
			GPIO_setf(GPIO_A, PIN_1);
		}

		if(true == GPIO_getf(GPIO_B, PIN_23))
		{
			LCD_nokia_goto_xy(38,3);
			LCD_nokia_send_string(string_LA_s);
			play_la_s();
			GPIO_setf(GPIO_B, PIN_23);
		}

		if(true == GPIO_getf(GPIO_A, PIN_2))
		{
			LCD_nokia_goto_xy(38,3);
			LCD_nokia_send_string(string_SI);
			play_si();
			GPIO_setf(GPIO_A, PIN_2);
		}

		if(true == GPIO_getf(GPIO_C, PIN_2))
		{
			LCD_nokia_goto_xy(38,3);
			LCD_nokia_send_string(string_DO2);
			play_do2();
			GPIO_setf(GPIO_C, PIN_2);
		}

		if(true == GPIO_getf(GPIO_B, PIN_20))
		{

			current_state = MAIN_MENU;
			counter = 0U;
			GPIO_setf(GPIO_B, PIN_20);
		}

		break;

	/* The program goes here if the current state
	 * is main menu, and the "Sequence" GPIO
	 * associated button is pressed.
	 */

	/* Now, in the "sequence mode" state, when you press each button,
	 * nothing will sound, because the function is not called,
	 * but rather the sound is saved in a buffer, and it waits
	 * until there is another GPIO set. , in this case,
	 * of the GPIO pin associated with "PLAY sequence"
	 */

	/* Aditionally, instead of playing a sound,
	 * When a button is pressed, a char corresponding to the initial
	 * of the supposed sound to be played on the screen is displayed
	 */
	case RYTHM_MODE:
		if(0U == counter)
		{
			LCD_nokia_clear();
			LCD_nokia_goto_xy(5,0);
			LCD_nokia_send_string(string_rythms);
			LCD_nokia_goto_xy(0,2);
			LCD_nokia_send_string(string_rock);
			LCD_nokia_goto_xy(0,3);
			LCD_nokia_send_string(string_hiphop);
			LCD_nokia_goto_xy(0,4);
			LCD_nokia_send_string(string_latino);

			GPIO_setf(GPIO_B, PIN_20);
		}
		counter = 1U;


		break;

	/* If "SEQUENCE MODE" state has at least 1 stored sound,
	 * the program will go to this state and will play this sound
	 * repeatedly until you change state to "STOP SEQUENCE".*/

	}
}

/* This function is called when the band welcome screen
 * is passed, and we start the main manu,
 * using the state machine*/
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

/* This function is responsible for feeding the dog,
 * it uses a PIT to achieve this*/
void refresh_wdog(uint32_t flags)
{
	WDOG_Refresh(WDOG);
	PIT_StopTimer(PIT_BASEADDR, PIT_CHANNEL_TWO);
}

/* This function controls what WDOG will do in the event
 * of behavior that crashes the system,
 * and what it will do during correct operation.*/
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
