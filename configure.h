/*
 * configure.h
 *
 *  Created on: 8 Nov 2023
 *      Author: Alejandro De La Rosa A.
 */

#ifndef CONFIGURE_H_
#define CONFIGURE_H_

#include "NVIC.h"
#include "stdint.h"
#include "fsl_port.h"
#include "GPIO.h"

#define INPUT 0U
#define OUTPUT 1U

/* Prototypes */
void configure_port(void);
void configure_gpio(void);
void configure_nvic(void);
void configure_wdog(void);

#endif /* CONFIGURE_H_ */
