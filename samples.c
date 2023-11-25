/*
 * samples.c
 *
 *  Created on: 3 Nov 2023
 *      Author: DanielG
 */

/**
 * \brief
 * This driver is concerned with loading and playing sound samples,
 * using functions and variables to manage and emulate drum components.*/

#include "stdint.h"
#include "spi_mem.h"
#include "samples.h"
#include "dma.h"


/* This array stores read byte
 * memory direction*/
uint8_t address_array[3];

/* This arrays contain sound samples
 * that model drum components */
uint8_t KICK[6704];
uint8_t SNARE[3562];
uint8_t BONGO[2068];
uint8_t HI_HAT[2410];
uint8_t GUIRO[7068];
uint8_t RIM_SHOT[808];
uint8_t TAMBOR[4650];
uint8_t CYNBAL[10076];
uint8_t COWBELL[2558];


/* Memory directions where sound samples are stored,
 * using static variables */
static uint32_t kick_address = 0x02C000;
static uint32_t snare_address = 0x037000;
static uint32_t bongo_address = 0x004000;
static uint32_t hi_hat_address = 0x025000;
static uint32_t guiro_address = 0x016000;
static uint32_t rim_shot_address = 0x031000;
static uint32_t tambor_address = 0x039000;
static uint32_t cynbal_address = 0x00E000;
static uint32_t cowbell_address = 0x00C000;

/**
 * \brief
 * This function load all the sound samples,
 * for each drum component with their respective
 * direction and length */
void load_instruments(void)
{
	read_instrument(KICK, kick_address, 6704);
	read_instrument(SNARE, snare_address, 3561);
	read_instrument(BONGO, bongo_address, 2067);
	read_instrument(HI_HAT, hi_hat_address, 2410);
	read_instrument(GUIRO, guiro_address, 7067);
	read_instrument(RIM_SHOT, rim_shot_address, 808);
	read_instrument(TAMBOR, tambor_address, 4650);
	read_instrument(CYNBAL, cynbal_address, 10075);
	read_instrument(COWBELL, cowbell_address, 2557);

}

/**
 * \brief
 * This function read the sound samples,
 * it receives a pointer to the drum component (instrument), the address and the length.
 * It reads the sound samples of an instrument
 * from memory using the given direction and length */
void read_instrument(uint8_t * instrument, uint32_t address, uint16_t length)
{


	for(uint16_t i = 0; i < length; i+=2)
	{
		address_array[0] = address >> 16U & 0xFF;
		address_array[1] = address >> 8U & 0xFF;
		address_array[2] = address & 0xFF;

		/* Using "memory_read_byte" function,
		 * we read the bytes from memory and then,
		 * we store those bytes in the instrument array*/
		instrument[i+1] = memory_read_byte(address_array);

		address++;

		address_array[0] = address >> 16U & 0xFF;
		address_array[1] = address >> 8U & 0xFF;
		address_array[2] = address & 0xFF;

		instrument[i] = memory_read_byte(address_array);

		address++;
	}

}

/**
 * \brief
 * This are the play functions,
 * each one of these, set and activate the DMA,
 * in order to play the samples of the corresponding
 * instrument*/
void play_kick(void)
{
	configure_dma(KICK, 6704);
}

void play_snare(void)
{
	configure_dma(SNARE, 3562);
}

void play_bongo(void)
{
	configure_dma(BONGO, 2068);
}

void play_hihat(void)
{
	configure_dma(HI_HAT, 2410);
}
void play_guiro(void)
{
	configure_dma(GUIRO, 7068);
}
void play_rimshot(void)
{
	configure_dma(RIM_SHOT, 808);
}
void play_tambor(void)
{
	configure_dma(TAMBOR, 4650);
}

void play_cynbal(void)
{
	configure_dma(CYNBAL, 10076);
}
void play_cowbell(void)
{
	configure_dma(COWBELL, 2558);
}

/**
 * \brief
 * Plays the sequence of a specific instrument
 * based on the given character.
 * It calls the corresponding play-back functions
 * based on the character ('K', 'S', 'B', 'H', 'G', 'R', 'T', 'C', 'c').*/
void play_sequence(uint8_t character)
{
	if('K' == character)
	{
		play_kick();
	}
	else if('S' == character)
	{
		play_snare();
	}
	else if('B' == character)
	{
		play_bongo();
	}
	else if('H' == character)
	{
		play_hihat();
	}
	else if('G' == character)
	{
		play_guiro();
	}
	else if('R' == character)
	{
		play_rimshot();
	}
	else if('T' == character)
	{
		play_tambor();
	}
	else if('C' == character)
	{
		play_cynbal();
	}
	else if('c' == character)
	{
		play_cowbell();
	}
}




