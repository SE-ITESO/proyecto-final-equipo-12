
#include "SPI.h"
#include "fsl_dspi.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
#include "clock_config.h"
#include "GPIO.h"

void SPI_config(void)
{
	dspi_master_config_t masterConfig;
	uint32_t srcClock_Hz;

	CLOCK_EnableClock(kCLOCK_Spi0);
	CLOCK_EnableClock(kCLOCK_PortD);	//habilita el reloj en los pines D

	PORT_SetPinMux(PORTD, PIN0_IDX, kPORT_MuxAlt2);		//los pines de los cuales disponemos para usar el SPI
	PORT_SetPinMux(PORTD, PIN1_IDX, kPORT_MuxAlt2);
	PORT_SetPinMux(PORTD, PIN2_IDX, kPORT_MuxAlt2);
	PORT_SetPinMux(PORTD, PIN3_IDX, kPORT_MuxAlt2);
	PORT_SetPinDriveStrength(PORTD, PIN0_IDX, kPORT_HighDriveStrength);
	PORT_SetPinDriveStrength(PORTD, PIN1_IDX, kPORT_HighDriveStrength);
	PORT_SetPinDriveStrength(PORTD, PIN2_IDX, kPORT_HighDriveStrength);
	PORT_SetPinDriveStrength(PORTD, PIN3_IDX, kPORT_HighDriveStrength);

/* Master config */
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
	DSPI_MasterInit(SPI0, &masterConfig, srcClock_Hz);
}


