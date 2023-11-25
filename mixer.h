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
	FX_MODE,

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
