/*
 * DAC.c
 *
 *  Created on: 15 Sept 2023
 *      Author: admin
 */


#include "fsl_dac.h"

#include "DAC.h"



void DAC_configure(void)
{
	dac_config_t dacConfigStruct;
    /* Configure the DAC. */
    /*
     * dacConfigStruct.referenceVoltageSource = kDAC_ReferenceVoltageSourceVref2;
     * dacConfigStruct.enableLowPowerMode = false;
     */
    DAC_GetDefaultConfig(&dacConfigStruct);
    DAC_Init(DAC0, &dacConfigStruct);
    DAC_Enable(DAC0, true);             /* Enable output. */
    //DAC_SetBufferReadPointer(DAC0, 0U); /* Make sure the read pointer to the start. */
                                                     /*
                                                      * The buffer is not enabled, so the read pointer can not move automatically. However, the buffer's read pointer
                                                      * and itemss can be written manually by user.
                                                      */

}
