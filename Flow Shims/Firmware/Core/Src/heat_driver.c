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
#include "main.h"

char heater_mask[NUM_HEATERS] = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};
int16_t setpoints[NUM_HEATERS];
int16_t integrals[NUM_HEATERS];
int16_t derivatives[NUM_HEATERS];

int temps[NUM_HEATERS];
int16_t new_errors[NUM_HEATERS];
int16_t errors[NUM_HEATERS];
uint16_t outputs[NUM_HEATERS];

void array_off(){
	for(int i = 1; i <= NUM_HEATERS; i++){
		set_heater(i, 0);
		HAL_Delay(1);
	}
}

void array_on(){
	for(int i = 1; i <= NUM_HEATERS; i++){
		set_heater(i, 2500);
		HAL_Delay(1);
	}
}

void enable_vd(){
	HAL_GPIO_WritePin(VD_EN_GPIO_Port, VD_EN_Pin, GPIO_PIN_SET);
}

void disable_vd(){
	HAL_GPIO_WritePin(VD_EN_GPIO_Port, VD_EN_Pin, GPIO_PIN_RESET);
}

void init_heater_driver(){
	ADDR_PORT->MODER |= ADDR_PORT_MODER_VAL;
	ENX_0_PORT->MODER |= ENX_0_PORT_MODER_VAL;
	ENX_1_PORT->MODER |= ENX_1_PORT_MODER_VAL;
}

void set_all_heaters(uint16_t * outputs){
	for(int i = 1; i <= NUM_HEATERS; i++){
		set_heater(i, outputs[i]);
	}
}

void set_heater(uint8_t element, uint16_t output){
	//Set DAC
	DAC->DHR12R1 = output;
	//Set address
	uint16_t address = 0;
	if(element <= 25){
		address = (
				A0SHIFT((element - 1)) |
				A1SHIFT((element - 1)) |
				A2SHIFT((element - 1)) |
				A3SHIFT((element - 1))
				);
	}else{
		address = (
				A0SHIFT((element - 26)) |
				A1SHIFT((element - 26)) |
				A2SHIFT((element - 26)) |
				A3SHIFT((element - 26))
				);
	}

	debug_printf("%x\n\r", SET_ADDR_PINS(address));
	debug_printf("seten%x\n\r", SET_ENX_0_PINS(GETEN(element)));
	debug_printf("en%x\n\r", GETEN(element));

	ADDR_PORT->BSRR = SET_ADDR_PINS(address);

	if(element <= 25){
		ENX_0_PORT->BSRR = SET_ENX_0_PINS(GETEN(element));
	}else{
		ENX_1_PORT->BSRR = SET_ENX_1_PINS(GETEN(element));
	}


	//Wait
	delay_us(500);

	//Turn off element
	ENX_0_PORT->BSRR = SETLOW(ENX0_MASK);
	ENX_1_PORT->BSRR = SETLOW(ENX1_MASK);
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


