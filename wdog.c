/*
 * wdog.c
 *
 *  Created on: 28 Sept 2023
 *      Author: admin
 */

#include "fsl_wdog.h"
#include "fsl_rcm.h"
#include <stdio.h>
#include "MK64F12.h"
#include "wdog.h"


void WaitWctClose(WDOG_Type *base)
{
	/* Accessing register by bus clock */
	for (uint32_t i = 0; i < WDOG_WCT_INSTRUCITON_COUNT; i++)
	{
		(void)base->RSTCNT;
	}
}



