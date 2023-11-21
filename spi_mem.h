/*
 * spi_mem.h
 *
 *  Created on: 28 Oct 2023
 *      Author: admin
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

uint8_t memory_read_byte(uint8_t * address);

void config_memory(void);
void memory_write_enable(void);
void memory_write_array(uint32_t address, uint8_t *data, uint32_t dataSize);
uint8_t memory_read_sr(void);
void memory_sector_erase(uint8_t address);


#endif /* SPI_MEM_H_ */
