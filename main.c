

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

uint8_t status_reg[2] = {0xF};
uint8_t array_to_write[6]= {23, 44, 5, 204, 33, 14 };
uint8_t instrument_xd[6] = {0};
uint8_t address_array_ekisde[3] = {0x00,0x00,0x00};

int main(void)
{

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
	PIT_callback_init(PIT_CHANNEL_THREE, play_do_piano_reverbed);

	SPI_config();

	LCD_nokia_init(); /*! Configuration function for the LCD */
	config_memory();
	LCD_nokia_clear();

	memory_write_enable();
	memory_sector_erase(address_array_ekisde);
	//load_instruments();
	status_reg[0] = memory_read_sr();
	memory_write_array(address_array_ekisde, array_to_write);
	SDK_DelayAtLeastUs(1000000, 100000000);

	read_instrument(instrument_xd, 0x000000, 6);
	while(1) {


		PIT_StartTimer(PIT_BASEADDR, PIT_CHANNEL_TWO);
		state_machine_one();


	}
	
	return 0;
}

