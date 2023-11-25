/*
 * LCD_nokia_images.c
 *
 *  Created on: 29/09/2023
 *      Author: DanielG & Alejandro De La Rosa A.
 *
 */

#include <mixer.h>
#include "LCD_nokia.h"
#include "LCD_nokia_images.h"
#include "stdint.h"
#include "SPI.h"
#include "spi_mem.h"
#include "clock.h"
#include "NVIC.h"
#include "PIT.h"
#include "configure.h"
#include "samples.h"

/**
 * \brief
 * Main function
 * Here, we load all the initial configuration, resources
 * and parameters to be used, before we enter while loop
 * and start the program */
int main(void)
{
	/* Load initial configuration:
	 * Clocks
	 * MAL API (PORTs, interrupts, Timers, and memories) */
	config_clock();

	CLOCK_EnableClock(kCLOCK_PortA);
	CLOCK_EnableClock(kCLOCK_PortB);
	CLOCK_EnableClock(kCLOCK_PortC);
	CLOCK_EnableClock(kCLOCK_PortD);

	configure_port();
	configure_gpio();
	configure_nvic();


	PIT_configure();
	PIT_callback_init(PIT_CHANNEL_ONE, interrupt_pit);
	PIT_callback_init(PIT_CHANNEL_TWO, refresh_wdog);

	SPI_config();

	LCD_nokia_init();   /*! Configuration function for the LCD */
	config_memory();    /*! Prepare memory and initial settings */
	LCD_nokia_clear();  /*! Initialize screen by clearing undesired chars */
	//load_instruments(); /*! Load sound arrays to memory */

	//configure_wdog();   /*! Load WDOG configuration functions */

	while(1) {

		/* Main menu & start program */
		PIT_StartTimer(PIT_BASEADDR, PIT_CHANNEL_TWO);
		state_machine_one();


	}
	
	return 0;
}

