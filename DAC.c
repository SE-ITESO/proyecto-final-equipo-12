/*
 * DAC.c
 *
 *  Created on: 15 Sept 2023
 *      Author: Daniel Gutierrez & Alejandro De La Rosa
 */


#include "fsl_dac.h"

#include "DAC.h"


/**
 * \brief
 * This function configures the DAC operation
 * */

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

void DAC1_configure(void)
{
	dac_config_t dacConfigStruct;
    /* Configure the DAC. */
    /*
     * dacConfigStruct.referenceVoltageSource = kDAC_ReferenceVoltageSourceVref2;
     * dacConfigStruct.enableLowPowerMode = false;
     */
    DAC_GetDefaultConfig(&dacConfigStruct);
    DAC_Init(DAC1, &dacConfigStruct);
    DAC_Enable(DAC1, true);             /* Enable output. */
    //DAC_SetBufferReadPointer(DAC0, 0U); /* Make sure the read pointer to the start. */
                                                     /*
                                                      * The buffer is not enabled, so the read pointer can not move automatically. However, the buffer's read pointer
                                                      * and itemss can be written manually by user.
                                                      */

}

