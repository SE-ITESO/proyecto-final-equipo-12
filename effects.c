/*
 * effects.c
 *
 *  Created on: 18 Nov 2023
 *      Author: Daniel Gutierrez & Alejandro De La Rosa A.
 */
#include "effects.h"


static float REVERB[18000] = {0};



float audioBuffer[BUFFER_SIZE];

uint8_t readIndex = 0;
uint8_t writeIndex = 0;

void apply_reverb(uint16_t* input_buffer, uint16_t* output_buffer, uint16_t input_lenght, uint16_t output_lenght)
{
	REVERB[1] = 1;
	REVERB[4000] = 0.3;
	REVERB[17000] = 0.1;


    for (uint16_t i = 0; i < output_lenght; i++) {
        output_buffer[i] = 0;
        for (uint16_t j = 0; j < input_lenght; j++) {
            if (i - j >= 0 && i - j < 180) {
                output_buffer[i] += input_buffer[j] * REVERB[i - j];
            }
        }
    }
}
void apply_delay(void)
{

}
void apply_distortion(void)
{

}
