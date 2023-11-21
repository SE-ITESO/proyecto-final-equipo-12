/*
 * dma.h
 *
 *  Created on: 3 Nov 2023
 *      Author: admin
 */

#ifndef DMA_H_
#define DMA_H_
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


#define ARR_SIZE (16u)
#define BUFFER_LENGTH       8
#define TCD_QUEUE_SIZE      1U

#define DMA_SOURCE_GPIO (51u)
#define DMA_MUX_ALWAYS (58u)

/**
 * \brief
 * This function initiates the dma to make a transference values */
void configure_dma(uint8_t * src_buffer, uint16_t length);
#endif /* DMA_H_ */
