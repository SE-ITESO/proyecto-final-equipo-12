/*
 * wdog.c
 *
 *  Created on: 28 Sept 2023
 *      Author: DanielG
 */

#include "fsl_wdog.h"
#include "fsl_rcm.h"
#include <stdio.h>
#include "MK64F12.h"
#include "wdog.h"

/**
 * \brief
 * This function waits until WDOG Timer closes,
 * The function uses a for loop to execute a read operation
 *  of the WDOG module's RSTCNT register multiple times.*/

/* The purpose of this operation is to access the WDOG RSTCNT register
 * multiple times to ensure that the timer closing process is completed.*/
void WaitWctClose(WDOG_Type *base)
{
	/* Accessing register by bus clock */
	for (uint32_t i = 0; i < WDOG_WCT_INSTRUCITON_COUNT; i++)
	{
		(void)base->RSTCNT;
	}
}



