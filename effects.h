/*
 * effects.h
 *
 *  Created on: 18 Nov 2023
 *      Author: admin
 */

#ifndef EFFECTS_H_
#define EFFECTS_H_

#define BUFFER_SIZE 1024

#include "stdint.h"

void apply_reverb(uint16_t* input_buffer, uint16_t* output_buffer, uint16_t input_lenght, uint16_t output_lenght);
void apply_delay(void);
void apply_distortion(void);


#endif /* EFFECTS_H_ */
