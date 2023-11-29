/*
 * spi.c
 *
 *  Created on: 28 Oct 2023
 *      Author: DanielG & Alejandro De La Rosa A.
 */

#include "SPI.h"
#include "fsl_dspi.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
#include "clock_config.h"
#include "GPIO.h"


/**
 * \brief
 * This function performs the necessary configuration
 * for communication with the LCD SPI Screen device. */
void SPI_config(void)
{
	/* Master DSPI Configuration structure*/
	dspi_master_config_t masterConfig;
	uint32_t srcClock_Hz;

	/* Clock gating setting for DSPI0 */
	CLOCK_EnableClock(kCLOCK_Spi0);
	CLOCK_EnableClock(kCLOCK_PortD);

	/* Pin setting at PORTD to work as SPI interface pins */
	PORT_SetPinMux(PORTD, PIN0_IDX, kPORT_MuxAlt2);
	PORT_SetPinMux(PORTD, PIN1_IDX, kPORT_MuxAlt2);
	PORT_SetPinMux(PORTD, PIN2_IDX, kPORT_MuxAlt2);
	PORT_SetPinMux(PORTD, PIN3_IDX, kPORT_MuxAlt2);

	/* To enable more current output capacity*/
	PORT_SetPinDriveStrength(PORTD, PIN0_IDX, kPORT_HighDriveStrength);
	PORT_SetPinDriveStrength(PORTD, PIN1_IDX, kPORT_HighDriveStrength);
	PORT_SetPinDriveStrength(PORTD, PIN2_IDX, kPORT_HighDriveStrength);
	PORT_SetPinDriveStrength(PORTD, PIN3_IDX, kPORT_HighDriveStrength);

	/* Master config */
	/* Setting are:
	 * - Baud-rate Speed
	 * - Bits per frame
	 * - Clock polarity
	 * - Clock phase
	 * - Bits order (MSB first)
	 * - Delays
	 * - Temporized format
	 * - Select PCS (Chip select & its polarity)
	 * - Clock continuity
	 * - Reception FIFO Overwrite & modified temporized format
	 */
	masterConfig.whichCtar 							= kDSPI_Ctar0;
	masterConfig.ctarConfig.baudRate 				= TRANSFER_BAUDRATE;
	masterConfig.ctarConfig.bitsPerFrame 			= 8;
	masterConfig.ctarConfig.cpol					= kDSPI_ClockPolarityActiveHigh;
	masterConfig.ctarConfig.cpha					= kDSPI_ClockPhaseFirstEdge;
	masterConfig.ctarConfig.direction				= kDSPI_MsbFirst;
	masterConfig.ctarConfig.pcsToSckDelayInNanoSec 	= 0;
	masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec = 0;
	masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 0;

	masterConfig.whichPcs = kDSPI_Pcs0;
	masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow;

	masterConfig.enableContinuousSCK = false;
	masterConfig.enableRxFifoOverWrite = false;
	masterConfig.enableModifiedTimingFormat = false;
	masterConfig.samplePoint = kDSPI_SckToSin0Clock;

	srcClock_Hz = CLOCK_GetFreq(DSPI0_CLK_SRC);

	/* Call this function to initialize DSPI Master with the specified configuration*/
	DSPI_MasterInit(SPI0, &masterConfig, srcClock_Hz);
}


