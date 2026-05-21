/*
 * hubspoke.h
 *
 *  Created on: Jul 21, 2025
 *      Author: hansg
 */

#ifndef SRC_HUBSPOKE_H_
#define SRC_HUBSPOKE_H_

#include <stdint.h>
#include "stm32l0xx_hal.h"

static uint8_t heater_remap[32] = {24,23,19,20,22,21,18,17,30,29,27,28,32,31,25,26,1,2,3,4,5,6,7,8,9,10,11,12,13,14,16,15};
static uint8_t temp_remap[32] = {26,25,21,22,24,23,20,19,32,31,29,30,2,1,27,28,3,4,5,6,7,8,9,10,11,12,13,14,15,16,18,17};



extern ADC_HandleTypeDef hadc;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

void set_heater(uint8_t element, uint8_t value);
void select_temp(uint8_t element);
uint32_t read_temp(uint8_t element);
void read_all(uint32_t * buffer);
void heater_drive_on();
void heater_drive_off();

#endif /* SRC_HUBSPOKE_H_ */
