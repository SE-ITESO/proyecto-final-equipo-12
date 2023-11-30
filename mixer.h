/*
 * state_machines.c
 *
 *  Created on: 3 Nov 2023
 *      Author: Daniel Gutierrez & Alejandro De La Rosa A.
 */

#ifndef STATE_MACHINES_C_
#define STATE_MACHINES_C_


#include "stdint.h"

/* enumerated type that contains
 * as members each of the states to use */
typedef enum {
	WELCOME,
	MAIN_MENU,
	SELECT_MODE,
	CLEAN_MODE,
	RYTHM_MODE,
	RYTHM_PLAYING,

}State_name_t;

typedef enum {
	NONE_LOADED,
	PIANO_LOADED,
	GUITAR_LOADED,
	BASS_LOADED,
}loaded_t;

/**
 * \brief
 * This is the main state machine, it controls each state in the mixer */
void state_machine_one(void);

/**
 * \brief
 * This function activates a flag to know when the PIT has interrupted */
void interrupt_pit(uint32_t flags);



#endif /* STATE_MACHINES_C_ */
