
#include "stdint.h"
#include "SPI.h"
#include "spi_mem.h"
#include "clock.h"
#include "configure.h"


uint8_t status_reg[2] = {0xF};
uint8_t array_to_write[6]= {23, 44, 53, 24, 89, 14 };
uint8_t instrument_xd[6] = {0};
uint8_t address_array_ekisde[3] = {0x00,0x00,0x00};
uint8_t KICK[20] = {0};
uint8_t address_array_kick[3] = {0x02, 0xC0, 0x00};
int main(void)
{

	static uint32_t address = 0x000000;
	config_clock();

	CLOCK_EnableClock(kCLOCK_PortA);
	CLOCK_EnableClock(kCLOCK_PortB);
	CLOCK_EnableClock(kCLOCK_PortC);
	CLOCK_EnableClock(kCLOCK_PortD);

	configure_port();
	configure_gpio();


	SPI_config();
	config_memory();

	for(uint8_t i = 0; i < 20; i++)
	{
		KICK[i] = memory_read_byte(address_array_kick);
	}

	//status_reg[0] = memory_read_sr();
	memory_sector_erase(address);
	SDK_DelayAtLeastUs(100000, 100000000);
	memory_write_array(0x000000, array_to_write, 6);
	SDK_DelayAtLeastUs(100000, 100000000);

	for(uint8_t i = 0; i < 6; i++)
	{
	    address_array_ekisde[0] = (address >> 16) & 0xFF;
	    address_array_ekisde[1] = (address >> 8) & 0xFF;
	    address_array_ekisde[2] = address & 0xFF;

		instrument_xd[i] = memory_read_byte(address_array_ekisde);
		address++;
	}

	while(1) {




	}

	return 0;
}
