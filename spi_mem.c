/*
 * spi_mem.c
 *
 *  Created on: 28 Oct 2023
 *      Author: DanielG & Alejandro De La Rosa A.
 */
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "SPI.h"
#include "spi_mem.h"
#include "stdint.h"
#include "fsl_dspi.h"
#include "GPIO.h"

/**
 * \brief
 * This function performs the necessary configuration
 * for SPI communication with a memory device. */
void config_memory(void)
{
	/* Master DSPI Configuration structure*/
	dspi_master_config_t masterConfig;
	uint32_t srcClock_Hz;

	/* Clock gating setting for DSPI0 */
	CLOCK_EnableClock(kCLOCK_Spi0);
	CLOCK_EnableClock(kCLOCK_PortC);

	PORT_SetPinMux(PORTC,PIN3_IDX,kPORT_MuxAlt2);

	 /* Master configuration */
	masterConfig.whichCtar                                = kDSPI_Ctar1;
	masterConfig.ctarConfig.baudRate                      = TRANSFER_BAUDRATE;
	masterConfig.ctarConfig.bitsPerFrame                  = 8;                            /*Quantity of size of frame to transfer*/
	masterConfig.ctarConfig.cpol                          = kDSPI_ClockPolarityActiveLow; /*CPOL == 1*/
	masterConfig.ctarConfig.cpha                          = kDSPI_ClockPhaseSecondEdge;   /*CPHA == 1*/
	masterConfig.ctarConfig.direction                     = kDSPI_MsbFirst;

	/*---------------------------------------- Parameters to set ------------------------------------*/
	masterConfig.ctarConfig.pcsToSckDelayInNanoSec        = 0;
	masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec    = 0;
	masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 0;

	masterConfig.whichPcs           = kDSPI_Pcs1;		  /* Which chip enable is being selected, and what slave has this setting*/
	masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow; /* Chip enable active in low or high */

	masterConfig.enableContinuousSCK        = false;
	masterConfig.enableRxFifoOverWrite      = false;
	masterConfig.enableModifiedTimingFormat = false;
	masterConfig.samplePoint                = kDSPI_SckToSin0Clock;	 /* Set the moment where it takes the sample point */

	srcClock_Hz = CLOCK_GetFreq(DSPI0_CLK_SRC);
	DSPI_MasterInit(SPI0, &masterConfig, srcClock_Hz);
}
/**
 * \brief
 * This function performs a read operation of a byte
 * from a specific SPI memory */
uint8_t memory_read_byte(uint8_t * address)
{
	/* 4 Byte Read command */
	uint8_t command[4U];
	uint8_t rx_buffer[1U] = {0};

	/* First byte is the read Op-code*/
	command[0] = 0x03;

	/* Following bytes are the specified memory direction
	 * by "address" pointer */
	command[1] = address[0];
	command[2] = address[1];
	command[3] = address[2];

	/* Half duplex transfer*/
	dspi_half_duplex_transfer_t masterXfer;

	/* Reception & transmission data size,
	 * and other characteristics*/
	masterXfer.txData = command;
	masterXfer.rxData = rx_buffer;
	masterXfer.rxDataSize = 1;
	masterXfer.txDataSize = 4U;
	masterXfer.isPcsAssertInTransfer = true;
	masterXfer.isTransmitFirst = true;
	masterXfer.configFlags = kDSPI_MasterCtar1  | kDSPI_MasterPcs1 | kDSPI_MasterPcsContinuous;

	/* Start blocking DSPI Transfer */
	DSPI_MasterHalfDuplexTransferBlocking(SPI0, &masterXfer);

	/* Result return located in reception buffer,
	 * the first read byte is returned. */
	return rx_buffer[0];
}
