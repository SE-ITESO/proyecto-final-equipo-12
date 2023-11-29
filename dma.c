/*
 * DMA.c
 *
 *  Created on: 10 Sept 2023
 *      Author: JLPE
 *      Modified By: Daniel Gutierrez & Alejandro De La Rosa
 */

#include <stdio.h>
#include "bits.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_edma.h"
#include "NVIC.h"
#include "GPIO.h"
#include "fsl_dmamux.h"
#include "DAC.h"
#include "PIT.h"
#include "dma.h"
#include "MK64F12.h"


/* This variable is set to true when a DMA transfer is completed */
volatile bool g_Transfer_Done = false;


/* Macro used to align memory into tranfer */
AT_QUICKACCESS_SECTION_DATA_ALIGN(edma_tcd_t tcdMemoryPoolPtr[TCD_QUEUE_SIZE + 1], sizeof(edma_tcd_t));

static edma_handle_t g_EDMA_Handle;

/**
 * \brief
 * This function is executed when a DMA tranfer is completed.
 * It stops the PIT timer and sets "g_tranfer_Done" to true. */
void EDMA_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
    if (transferDone)
    {
        g_Transfer_Done = true;
        PIT_StopTimer(PIT_BASEADDR, PIT_CHANNEL);
    }
}


/**
 * \brief
 * This function takes as arguemntes the source bufer,
 * and the transfer length, it initializes DAC using the respective function,
 * enables DMAMUX, and its controller.
 * Also, it configures & prepares the transfer,
 * descriptor, request and interrupts */
void configure_dma(uint8_t * src_buffer, uint16_t length) {

		edma_transfer_config_t transferConfig;
	    edma_config_t userConfig;

		DAC_configure();

	    DMAMUX_Init(DMAMUX0);

	    DMAMUX_SetSource(DMAMUX0, 0, DMA_MUX_ALWAYS);
	    DMAMUX_EnablePeriodTrigger(DMAMUX0, 0);
	    DMAMUX_EnableChannel(DMAMUX0, 0);


	    EDMA_GetDefaultConfig(&userConfig);
	    EDMA_Init(DMA0, &userConfig);
	    EDMA_CreateHandle(&g_EDMA_Handle, DMA0, 0);
	    EDMA_SetCallback(&g_EDMA_Handle, EDMA_Callback, NULL);
	    EDMA_ResetChannel(g_EDMA_Handle.base, g_EDMA_Handle.channel);

	    EDMA_PrepareTransferConfig(&transferConfig,
	    		   &src_buffer[0],
				   2,
				   2, /* source offset */
				   (void*)DAC0_BASE,
				   2,
				   0, /* dest offset */
				   2,
				   length);
	   EDMA_TcdSetMajorOffsetConfig(
			    		   tcdMemoryPoolPtr,  	/* param tcd A point to the TCD structure. */
			    		   0, 					/* param sourceOffset source address offset. */
						   0);					/* destOffset destination address offset. */

	   EDMA_TcdSetTransferConfig(tcdMemoryPoolPtr, &transferConfig, NULL);

	   EDMA_TcdEnableInterrupts(&tcdMemoryPoolPtr[0], kEDMA_MajorInterruptEnable);
	   EDMA_TcdEnableAutoStopRequest(&tcdMemoryPoolPtr[0], false);
	   EDMA_InstallTCD(DMA0, 0, &tcdMemoryPoolPtr[0]);
	   EDMA_EnableChannelRequest(DMA0, 0);

	   NVIC_enable_interrupt_and_priotity(DMA_CH0_IRQ, PRIORITY_4);
	   NVIC_global_enable_interrupts;

	   PIT_StartTimer(PIT_BASEADDR, PIT_CHANNEL);

}




