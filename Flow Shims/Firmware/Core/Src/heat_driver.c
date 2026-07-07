/*
 * heat_driver.c
 *
 *  Created on: Jul 7, 2026
 *      Author: hans
 */

#include "heat_driver.h"
#include "tmp11826.h"
#include "stm32l0xx.h"
#include "stm32l0xx_hal.h"
#include <string.h>

char heater_mask[NUM_HEATERS] = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
int16_t setpoints[NUM_HEATERS];
int16_t integrals[NUM_HEATERS];
int16_t derivatives[NUM_HEATERS];

int16_t temps[NUM_HEATERS];
int16_t new_errors[NUM_HEATERS];
int16_t errors[NUM_HEATERS];
uint16_t outputs[NUM_HEATERS];

void init_heater_driver(){
	ADDR_PORT->MODER |= ADDR_PORT_MODER_VAL;
	ENX_0_PORT->MODER |= ENX_0_PORT_MODER_VAL;
	ENX_1_PORT->MODER |= ENX_1_PORT_MODER_VAL;
}

void set_all_heaters(uint16_t * outputs){
	for(int i = 0; i < NUM_HEATERS; i++){
		set_heater(i, outputs[i]);
	}
}

void set_heater(uint8_t element, uint16_t output){
	//Set DAC
	DAC->DHR12R1 = output;
	//Set address
	uint8_t address = (
			A0SHIFT(element) |
			A1SHIFT(element) |
			A2SHIFT(element) |
			A3SHIFT(element)
			);
	ADDR_PORT->BSRR = SET_SOME_PINS(address);

	//EN
	//	GETENPORT(x)->BSRR = 0;
	if(element < 25){
		ENX_0_PORT->BSRR = SET_SOME_PINS(GETEN(element));
	}else{
		ENX_1_PORT->BSRR = SET_SOME_PINS(GETEN(element));
	}

	//Wait
	delay_us(500);

	//Turn off element
	if(element < 25){
		ENX_0_PORT->BSRR = SETLOW(EN0_0P | EN1_0P);
	}else{
		ENX_1_PORT->BSRR = SETLOW(EN0_1P | EN1_1P);
	}
}

void step(){
	//Read in all temperatures
	read_all_temps(temps);
	for(int i = 0; i < NUM_HEATERS; i++) new_errors[i] = setpoints[i] - temps[i];
	for(int i = 0; i < NUM_HEATERS; i++) derivatives[i] = new_errors[i] - errors[i];
	memcpy(errors, new_errors, NUM_HEATERS);
	for(int i = 0; i < NUM_HEATERS; i++) integrals[i] = integrals[i] + errors[i];
	for(int i = 0; i < NUM_HEATERS; i++){
		int output = (P * errors[i] + I * integrals[i] + D * derivatives[i]) >> 8;
		if(output < 0) output = 0;
		if(output > 4095) output = 4095;
		outputs[i] = output;
	}
	set_all_heaters(outputs);
}


