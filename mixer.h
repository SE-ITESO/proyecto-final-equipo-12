/*
 * state_machines.c
 *
 *  Created on: 3 Nov 2023
 *      Author: DanGoose y EL ALex
 */

#ifndef STATE_MACHINES_C_
#define STATE_MACHINES_C_


#include "stdint.h"


typedef enum {
	WELCOME,
	MAIN_MENU,
	MANUAL_MODE,
	SEQUENCE_MODE,
	PLAY_SEQUENCE,
	STOP_SEQUENCE,
}State_name_t;

/**
 * \brief
 * This function controls the state of the mixer */
void state_machine_one(void);

/**
 * \brief
 * This function activates a flag to know when the PIT has interrupted */
void interrupt_pit(uint32_t flags);

/**
 * \brief
 * This function refreshes the WDOG */
void refresh_wdog(uint32_t flags);

/**
 * \brief
 * This function lets the program know a WDOG reset happened */
void wdog_reset(void);




#endif /* STATE_MACHINES_C_ */
