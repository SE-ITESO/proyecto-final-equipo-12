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

static uint8_t NOTE[20000];

static uint8_t DO[12000];
static uint8_t DO_s[12000];
static uint8_t RE[12000];
static uint8_t RE_S[12000];
static uint8_t MI[12000];
static uint8_t FA[12000];
static uint8_t FA_S[12000];
static uint8_t SOL[12000];
static uint8_t SOL_S[12000];
static uint8_t LA[12000];
static uint8_t LA_S[12000];
static uint8_t SI[12000];
static uint8_t DO_2[12000];




/**
 * \brief
 * This function load all the sound samples,
 * for each drum component with their respective
 * direction and length */

/**
 * \brief
 * This function read the sound samples,
 * it receives a pointer to the drum component (instrument), the address and the length.
 * It reads the sound samples of an instrument
 * from memory using the given direction and length */

void load_guitar(void)
{
	read_instrument(DO, 0x045200, 12000);
	read_instrument(DO_s, 0x04A100,12000);
	read_instrument(RE, 0x04F000, 12000);
	read_instrument(RE_S, 0x053F00,12000);
	read_instrument(MI, 0x058E00, 12000);
	read_instrument(FA, 0x05DD00,12000);
	read_instrument(FA_S, 0x062C00,12000);
	read_instrument(SOL, 0x067B00,12000);
	read_instrument(SOL_S, 0x06CA00,12000);
	read_instrument(LA, 0x071900,12000);
	read_instrument(LA_S, 0x076800,12000);
	read_instrument(SI, 0x07B700,12000);
	read_instrument(DO_2, 0x080600,12000);
}

void load_piano(void)
{
	read_instrument(DO, 0x000000, 12000);
	read_instrument(DO_s, 0x004F00,12000);
	read_instrument(RE, 0x009E00, 12000);
	read_instrument(RE_S, 0x00ED00,12000);
	read_instrument(MI, 0x013E00, 12000);
	read_instrument(FA, 0x018B00,12000);
	read_instrument(FA_S, 0x01DA00,12000);
	read_instrument(SOL, 0x022900,12000);
	read_instrument(SOL_S, 0x02C700,12000);
	read_instrument(LA, 0x031600,12000);
	read_instrument(LA_S, 0x036500,12000);
	read_instrument(SI, 0x03B400,12000);
	read_instrument(DO_2, 0x040300,12000);
}

void load_bass(void)
{
	read_instrument(DO, 0x085500, 12000);
	read_instrument(DO_s, 0x08A400,12000);
	read_instrument(RE, 0x08F300, 12000);
	read_instrument(RE_S, 0x094200,12000);
	read_instrument(MI, 0x099100, 12000);
	read_instrument(FA, 0x09E000,12000);
	read_instrument(FA_S, 0x0A2F00,12000);
	read_instrument(SOL, 0x0A7E00,12000);
	read_instrument(SOL_S, 0x0ACD00,12000);
	read_instrument(LA, 0x0B1C00,12000);
	read_instrument(LA_S, 0x0B6B00,12000);
	read_instrument(SI, 0x0BBA00,12000);
	read_instrument(DO_2, 0x0C0900,12000);
}


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
void play_do(void)
{
	configure_dma(DO, 12000);
}
void play_do_s(void)
{
	configure_dma(DO_s, 12000);
}
void play_re(void)
{
	configure_dma(RE, 12000);
}
void play_re_s(void)
{
	configure_dma(RE_S, 12000);
}
void play_mi(void)
{
	configure_dma(MI, 12000);
}
void play_fa(void)
{
	configure_dma(FA, 12000);
}
void play_fa_s(void)
{
	configure_dma(FA_S, 12000);
}
void play_sol(void)
{
	configure_dma(SOL, 12000);
}

void play_sol_s(void)
{
	configure_dma(SOL_S, 12000);
}
void play_la(void)
{
	configure_dma(LA, 12000);
}
void play_la_s(void)
{
	configure_dma(LA_S, 12000);
}
void play_si(void)
{
	configure_dma(SI, 12000);
}
void play_do2(void)
{
	configure_dma(DO_2, 12000);
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




