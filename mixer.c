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
#include "PIT.h"


/**
 * \brief
 * This static variables are used for flag handling,
 * instead of using more states, we use flags to determine which
 * instrument we are about to play, rythm, or mode  */

static uint8_t interrupt_pit_flag = 0U;
static uint8_t welcome_passed_flag = 0U;

static uint8_t guitar_mode = 0U;
static uint8_t piano_mode = 0U;
static uint8_t bass_mode = 0U;

static uint8_t rock_rythm = 0U;
static uint8_t hiphop_rythm = 0U;
static uint8_t latino_rythm = 0U;

State_name_t current_state = WELCOME;
loaded_t loaded_instrument = NONE_LOADED;

/* Strings to be deployed and shown in the LCD Screen */
uint8_t string_instruments[]="INSTRUMENTOS";
uint8_t string_piano[]="1)Piano";
uint8_t string_guitar[]="2)Guitarra";
uint8_t string_bass[]="3)Bajo";

uint8_t string_rythms[]="RITMOS";
uint8_t string_rock[]="1)Rock";
uint8_t string_hiphop[]="2)Hip-Hop";
uint8_t string_latino[]="3)Latino";

uint8_t string_rockear[]="A rockear!";
uint8_t string_rapear[]="A rapear!";
uint8_t string_bailar[]="A bailar!";

uint8_t string_PIANO[]="PIANO";
uint8_t string_GUITAR[]="GUITARRA";
uint8_t string_BASS[]="BAJO";

uint8_t string_loading[]="Cargando...";

uint8_t string_DO[]="DO  ";
uint8_t string_DO_s[]="DO#  ";
uint8_t string_RE[]="RE   ";
uint8_t string_RE_s[]="RE#   ";
uint8_t string_MI[]="MI   ";
uint8_t string_FA[]="FA   ";
uint8_t string_FA_s[]="FA#   ";
uint8_t string_SOL[]="SOL   ";
uint8_t string_SOL_s[]="SOL#";
uint8_t string_LA[]="LA   ";
uint8_t string_LA_s[]="LA#  ";
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
 * that execute specific interrupts, such as sounds,
 * and screen printing of characters and strings.
 * Everything controlled using states and specific flags*/
void state_machine_one(void)
{
	static uint8_t counter = 0U;
	switch(current_state)
	{
	case WELCOME:

		if(0U == counter)
		{
			PIT_StartTimer(PIT_BASEADDR, PIT_CHANNEL_ONE);

			/* shows the NXP image */
			welcome_sequence();


		}
		counter = 1U;
		if(1U == welcome_passed_flag)
		{
			current_state = MAIN_MENU;
			counter = 0U;
			PIT_StopTimer(PIT_BASEADDR, PIT_CHANNEL_ONE);
		}

		break;

	/* "MAIN MENU" state, here, you can transition to more states,
	 * depending on the button selection,
	 * (GPIOC pin 18, GPIOD pin 6, and GPIOD pin 4, go to "SELECT MODE" state) */
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

			/* can't play anything without
			selecting the mode */
		}
		counter = 1U;

		/* Here, using a flag, if we press GPIOC18, button, we load an instrument
		 * form SPI memory to K64 RAM memory*/
		if(true == GPIO_getf(GPIO_C, PIN_18))
		{
			piano_mode = 1U;
			guitar_mode = 0U;
			bass_mode = 0U;
			LCD_nokia_clear();
			LCD_nokia_goto_xy(10,2);
			LCD_nokia_send_string(string_loading);
			if(loaded_instrument != PIANO_LOADED)
			{
				load_piano();
			}
			/*Then, transitions to corresponding state */
			loaded_instrument = PIANO_LOADED;
			current_state = SELECT_MODE;
			counter = 0U;
			GPIO_setf(GPIO_C, PIN_18);
		}

		/* Otherwise, if we choose GPIOD6 Pin, we also transition
		 * to SELECT MODE State */
		else if(true == GPIO_getf(GPIO_D, PIN_6))
		{
			guitar_mode = 1U;
			piano_mode = 0U;
			bass_mode = 0U;
			LCD_nokia_clear();
			LCD_nokia_goto_xy(10,2);
			LCD_nokia_send_string(string_loading);
			if(loaded_instrument != GUITAR_LOADED)
			{
				load_guitar();
			}
			loaded_instrument = GUITAR_LOADED;
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
			if(loaded_instrument != BASS_LOADED)
			{
				load_bass();
			}
			loaded_instrument = BASS_LOADED;
			current_state = SELECT_MODE;
			counter = 0U;
			GPIO_setf(GPIO_D, PIN_4);
		}


		break;

	/* If you press button associated to GPIOD pin 4, pin 6 or GPIOC pin 18,
	 * you should transition to this state, here, you can choose between 2
	 * more states: CLEAN MODE or RYTHM MODE.*/

	/* That means that, if the program goes to the "CLEAN MODE" state,
		 * the get and set of the GPIO pin corresponding to each note,
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

		/* Program goes to this state when chosen */
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
		GPIO_setf(GPIO_C, PIN_18);
		GPIO_setf(GPIO_D, PIN_6);
		GPIO_setf(GPIO_D, PIN_4);
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
	 * is main menu, and the "Rythm mode" GPIO
	 * associated button is pressed.
	 */

	/* Now, in the "Rythm mode" state, when you press one of the
	 * 3 associated buttons, a drum kit rythm will play, being 3 main
	 * already preloaded sequences that will play directly from RAM.
	 */

	/* Aditionally, the sequence will play depending on the GPIO Button selection,
	 * But we can still play an instrument (PIANO, GUITAR, BASS GUITAR), Over the playing rythm.
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

		if(true == GPIO_getf(GPIO_B, PIN_20))
		{
			/* We can go back here to the main menu screen if we press GPIO associated button*/
			current_state = MAIN_MENU;
			counter = 0U;
			GPIO_setf(GPIO_B, PIN_20);
		}

		/* If we chosse GPIOC18, the state will change to RYTHM Playing, to display
		 * corresponding string, aditionally in this condition, the function that plays
		 * the rythm is called, and plays it
		 * using the PIT interrupt*/
		if(true == GPIO_getf(GPIO_C, PIN_18))
		{
			rock_rythm = 1U;
			hiphop_rythm = 0U;
			latino_rythm = 0U;
			current_state = RYTHM_PLAYING;
			counter = 0U;

			PIT_callback_init(PIT_CHANNEL_TWO, play_rock_rythm);
			PIT_StartTimer(PIT_BASEADDR, PIT_CHANNEL_TWO);
			GPIO_setf(GPIO_C, PIN_18);
		}

		/* If we chosse GPIOD6, the state will change to RYTHM Playing, to display
		 * corresponding string, aditionally in this condition, the function that plays
		 * the rythm is called, and plays it
		 * using the PIT interrupt*/
		else if(true == GPIO_getf(GPIO_D, PIN_6))
		{
			rock_rythm = 0U;
			hiphop_rythm = 1U;
			latino_rythm = 0U;
			current_state = RYTHM_PLAYING;
			counter = 0U;

			PIT_callback_init(PIT_CHANNEL_TWO, play_hiphop_rythm);
			PIT_StartTimer(PIT_BASEADDR, PIT_CHANNEL_TWO);

			GPIO_setf(GPIO_D, PIN_6);
		}

		/* If we chosse GPIOC18, the state will change to RYTHM Playing, to display
		 * corresponding string, aditionally in this condition, the function that plays
		 * the rythm is called, and plays it
		 * using the PIT interrupt*/
		else if(true == GPIO_getf(GPIO_D, PIN_4))
		{
			rock_rythm = 0U;
			hiphop_rythm = 0U;
			latino_rythm = 1U;
			current_state = RYTHM_PLAYING;
			counter = 0U;
			PIT_callback_init(PIT_CHANNEL_TWO, play_latino_rythm);
			PIT_StartTimer(PIT_BASEADDR, PIT_CHANNEL_TWO);
			GPIO_setf(GPIO_D, PIN_4);
		}


		break;

		/* The program goes here if any of the rythms are chosen*/
	case RYTHM_PLAYING:

		if(0U == counter)
		{
			/* show the menu */
			LCD_nokia_clear();
			LCD_nokia_goto_xy(0,0);
			if(1 == rock_rythm)
			{
				LCD_nokia_send_string(string_rockear);
			}
			else if(1 == hiphop_rythm)
			{
				LCD_nokia_send_string(string_rapear);
			}
			else if(1 == latino_rythm)
			{
				LCD_nokia_send_string(string_bailar);
			}


		}

		/* If we chose to go back, then it stops PIT timer
		 * and we go back to previous state, that means it stops playing
		 * a rythm */
		counter = 1U;

		if(true == GPIO_getf(GPIO_B, PIN_20))
		{
			PIT_StopTimer(PIT_BASEADDR, PIT_CHANNEL_TWO);
			current_state = RYTHM_MODE;
			counter = 0U;
			GPIO_setf(GPIO_B, PIN_20);
		}

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

		break;

	}
}

/* This function is called when the welcome screen
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

