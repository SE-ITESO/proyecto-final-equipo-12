/*
 * samples.h
 *
 *  Created on: 3 Nov 2023
 *      Author: DanielG
 */

#ifndef SAMPLES_H_
#define SAMPLES_H_

#include "stdint.h"


#define KICK_SIZE 2000U
#define SNARE_SIZE 1500U
#define HIHAT_SIZE 2000U
#define STICKS_SIZE 2000U
#define NOTE_SIZE 12000U

/**
 * \brief
 * This function reads form the memory */
void read_instrument(uint8_t * instrument, uint32_t address, uint16_t length);

/**
 * \brief
 * This function loads all the instruments to RAM memory */
void load_instruments(void);
void load_guitar(void);
void load_piano(void);
void load_bass(void);

/* plays instruments */
void play_kick(uint32_t flags);

void play_snare(uint32_t flags);

void play_sticks(uint32_t flags);

void play_hihat(uint32_t flags);

///////////////////////


void play_rock_rythm(uint32_t flags);
void play_hiphop_rythm(uint32_t flags);
void play_latino_rythm(uint32_t flags);

void play_do(void);
void play_do_s(void);
void play_re(void);
void play_re_s(void);
void play_mi(void);
void play_fa(void);
void play_fa_s(void);
void play_sol(void);
void play_sol_s(void);
void play_la(void);
void play_la_s(void);
void play_si(void);
void play_do2(void);


#endif /* SAMPLES_H_ */
