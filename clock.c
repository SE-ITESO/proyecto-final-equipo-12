/*
 * clock.c
 *
 *  Created on: 3 Nov 2023
 *      Author: Daniel Gutierrez
 */


#include <stdio.h>
#include "MK64F12.h"
#include "fsl_clock.h"
#include "clock.h"

/**
 * \brief
 * This function loads and sets the configuration
 * in order to bypass MCG clock */
void config_clock(void) {
	mcg_pll_config_t pll0Config =
	{
			.enableMode = MCG_PLL_DISABLE, /* MCGPLLCLK disabled */
			.prdiv = 0xeU, /* PLL Reference divider:	divided by 15 */
			.vdiv = 0x6U, /* VCO divider: multiplied by 36 */
	};
	const osc_config_t osc_config = {
			.freq = 50000000U, /* Oscillator frequency: 50000000Hz */
			.capLoad = (OSC_CAP0P), /* Oscillator capacity load: 0pF */
			.workMode = kOSC_ModeExt, /* Use external clock */
			.oscerConfig = {
					.enableMode =
					kOSC_ErClkEnable, /* Enable external reference clock, disable
										external reference clock in STOP mode */
					}
	};
/**
 * \brief
 * This functions are used to provide a safe frequency change,
 * then, we set the desired frequency and change it */
	CLOCK_SetSimSafeDivs();
	CLOCK_InitOsc0(&osc_config);

	CLOCK_SetXtal0Freq(osc_config.freq);

	CLOCK_BootToPeeMode(kMCG_OscselOsc, kMCG_PllClkSelPll0, &pll0Config);


}
