/*
 * spi_mem.c
 *
 *  Created on: 28 Oct 2023
 *      Author: admin
 */
#include "fsl_gpio.h"
#include "fsl_port.h"
#include "SPI.h"
#include "spi_mem.h"
#include "stdint.h"
#include "fsl_dspi.h"
#include "GPIO.h"

void config_memory(void)
{
	dspi_master_config_t masterConfig;
	uint32_t srcClock_Hz;
	CLOCK_EnableClock(kCLOCK_Spi0);
	CLOCK_EnableClock(kCLOCK_PortC);

	PORT_SetPinMux(PORTC,PIN3_IDX,kPORT_MuxAlt2);

	 /* Master config */
	masterConfig.whichCtar                                = kDSPI_Ctar1;
	masterConfig.ctarConfig.baudRate                      = TRANSFER_BAUDRATE;
	masterConfig.ctarConfig.bitsPerFrame                  = 8; //Cantidad o tamaño de frame de transferencia
	masterConfig.ctarConfig.cpol                          = kDSPI_ClockPolarityActiveLow; //CPOL == 1
	masterConfig.ctarConfig.cpha                          = kDSPI_ClockPhaseSecondEdge;//CPHA == 1
	masterConfig.ctarConfig.direction                     = kDSPI_MsbFirst;

	//----------------------------------------Parametros a configurar------------------------------------//
	masterConfig.ctarConfig.pcsToSckDelayInNanoSec        = 0;
	masterConfig.ctarConfig.lastSckToPcsDelayInNanoSec    = 0;
	masterConfig.ctarConfig.betweenTransferDelayInNanoSec = 0;

	masterConfig.whichPcs           = kDSPI_Pcs1;	//Cual chip enable se selecciona, que esclavo tiene esta configuración
	masterConfig.pcsActiveHighOrLow = kDSPI_PcsActiveLow; //Chip enable activo en bajo o alto

	masterConfig.enableContinuousSCK        = false;
	masterConfig.enableRxFifoOverWrite      = false;
	masterConfig.enableModifiedTimingFormat = false;
	masterConfig.samplePoint                = kDSPI_SckToSin0Clock;	 //configura el momento donde se toma el punto de muestreo

	srcClock_Hz = CLOCK_GetFreq(DSPI0_CLK_SRC);
	DSPI_MasterInit(SPI0, &masterConfig, srcClock_Hz);
}
uint8_t memory_read_byte(uint8_t * address)
{
	uint8_t command[4U];
	uint8_t rx_buffer[1U] = {0};
	command[0] = 0x03; //lectura
	command[1] = address[0];
	command[2] = address[1];
	command[3] = address[2];
	dspi_half_duplex_transfer_t masterXfer;

	masterXfer.txData = command;
	masterXfer.rxData = rx_buffer;
	masterXfer.rxDataSize = 1;
	masterXfer.txDataSize = 4U;
	masterXfer.isPcsAssertInTransfer = true;
	masterXfer.isTransmitFirst = true;
	masterXfer.configFlags = kDSPI_MasterCtar1  | kDSPI_MasterPcs1 | kDSPI_MasterPcsContinuous;

	DSPI_MasterHalfDuplexTransferBlocking(SPI0, &masterXfer);

	return rx_buffer[0];
}

uint8_t memory_read_sr(void)
{
	uint8_t command[1];
	uint8_t rx_buffer[2] = {0};
	command[0] = 0x05; //leer sr1

	dspi_half_duplex_transfer_t masterXfer;

	masterXfer.txData = command;
	masterXfer.rxData = rx_buffer;
	masterXfer.rxDataSize = 1;
	masterXfer.txDataSize = 4U;
	masterXfer.isPcsAssertInTransfer = true;
	masterXfer.isTransmitFirst = true;
	masterXfer.configFlags = kDSPI_MasterCtar1  | kDSPI_MasterPcs1 | kDSPI_MasterPcsContinuous;

	DSPI_MasterHalfDuplexTransferBlocking(SPI0, &masterXfer);

	return rx_buffer[0];
}

void memory_write_enable(void)
{
    uint8_t command = 0x06;
	uint8_t rx_buffer[1U] = {0};
    dspi_transfer_t masterXfer;
    uint8_t masterBuffer[1];

    masterBuffer[0] = command;

    masterXfer.txData = masterBuffer;
    masterXfer.dataSize = sizeof(masterBuffer);
	masterXfer.rxData = rx_buffer;
    masterXfer.configFlags = kDSPI_MasterPcsContinuous | kDSPI_MasterCtar1 | kDSPI_MasterPcs1;

    DSPI_MasterTransferBlocking(SPI0, &masterXfer);
}
void memory_sector_erase(uint8_t address)
{
    memory_write_enable();
	uint8_t rx_buffer[1U] = {0};
    uint8_t command = 0x20;

    dspi_transfer_t masterXfer;
    uint8_t masterBuffer[4];

    masterBuffer[0] = command;
    masterBuffer[1] = (address >> 16) & 0xFF;
    masterBuffer[2] = (address >> 8) & 0xFF;
    masterBuffer[3] = address & 0xFF;



    masterXfer.txData = masterBuffer;
    masterXfer.dataSize = sizeof(masterBuffer);
	masterXfer.rxData = rx_buffer;
    masterXfer.configFlags = kDSPI_MasterPcsContinuous | kDSPI_MasterCtar1 | kDSPI_MasterPcs1;

    DSPI_MasterTransferBlocking(SPI0, &masterXfer);
}
void memory_write_array(uint32_t address, uint8_t *data, uint32_t dataSize)
{
    memory_write_enable();
	uint8_t rx_buffer[1U] = {0};
    uint8_t command = 0x02;

    dspi_transfer_t masterXfer;
    uint8_t masterBuffer[4 + dataSize];

    masterBuffer[0] = command;
    masterBuffer[1] = (address >> 16) & 0xFF;
    masterBuffer[2] = (address >> 8) & 0xFF;
    masterBuffer[3] = address & 0xFF;

    // Copy data to buffer
    for (uint32_t i = 0; i < dataSize; ++i) {
        masterBuffer[4 + i] = data[i];
    }

    masterXfer.txData = masterBuffer;
    masterXfer.dataSize = sizeof(masterBuffer);
	masterXfer.rxData = rx_buffer;
    masterXfer.configFlags = kDSPI_MasterPcsContinuous | kDSPI_MasterCtar1 | kDSPI_MasterPcs1;

    DSPI_MasterTransferBlocking(SPI0, &masterXfer);

}
