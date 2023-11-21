/*
 * wdog.h
 *
 *  Created on: 28 Sept 2023
 *      Author: admin
 */

#ifndef WDOG_H_
#define WDOG_H_

#include "fsl_wdog.h"
#include "fsl_rcm.h"
#include <stdio.h>
#include "MK64F12.h"

#define TIMEOUT_VALUE_100mS 0x64

#define WDOG_NONRESET 0U
#define WDOG_RESET 1U

#define WDOG_WCT_INSTRUCITON_COUNT (256U)

void WaitWctClose(WDOG_Type *base);
inline uint32_t GetTimerOutputValue(WDOG_Type *base)
{
	return (uint32_t)((((uint32_t)base->TMROUTH) << 16U) | (base->TMROUTL));
}


#endif /* WDOG_H_ */
