/*
 * samples.h
 *
 *  Created on: 3 Nov 2023
 *      Author: DanielG
 */

#ifndef SAMPLES_H_
#define SAMPLES_H_

#include "stdint.h"

/**
 * \brief
 * This function reads form the memory */
void read_instrument(uint8_t * instrument, uint32_t address, uint16_t length);

/**
 * \brief
 * This function loads all the instruments to RAM memory */
void load_instruments(void);

/* plays instruments */
void play_kick(void);

void play_snare(void);

void play_bongo(void);

void play_hihat(void);
void play_guiro(void);
void play_rimshot(void);
void play_tambor(void);

void play_cynbal(void);
void play_cowbell(void);
///////////////////////
/**
 * \brief
 * This function plays an instrument in function of a character */
void play_sequence(uint8_t character);
#endif /* SAMPLES_H_ */
