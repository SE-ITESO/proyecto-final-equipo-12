/*
 * SPI.h
 *
 *  Created on: 19 jun. 2021
 *      Author: jlpe
 *      Modified by: DanielG & Alejandro De La Rosa A.
 */

#ifndef SPI_H_
#define SPI_H_
#include "fsl_dspi.h"
#include "fsl_port.h"
#include "fsl_gpio.h"
#include "fsl_clock.h"
#include "clock_config.h"
#include "GPIO.h"


#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */
#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */

#define TRANSFER_SIZE     64U     /*! Transfer dataSize */
#define TRANSFER_BAUDRATE 1000000U /*! Transfer baudrate - 500k */
#define TRANSFER_BAUDRATE_MEM 2000000U /*! Transfer baudrate - 500k */

void SPI_config(void);


#endif /* SPI_H_ */
