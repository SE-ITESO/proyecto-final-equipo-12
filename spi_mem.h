/*
 * spi_mem.h
 *
 *  Created on: 28 Oct 2023
 *      Author: DanielG & Alejandro De La Rosa A.
 */

#ifndef SPI_MEM_H_
#define SPI_MEM_H_

#include "fsl_gpio.h"
#include "fsl_port.h"
#include "SPI.h"
#include "spi_mem.h"
#include "stdint.h"
#include "fsl_dspi.h"
#include "GPIO.h"

/* Prototypes */
uint8_t memory_read_byte(uint8_t * address);
void config_memory(void);


#endif /* SPI_MEM_H_ */
