/*
 * heat_driver.h
 *
 *  Created on: Jul 7, 2026
 *      Author: hans
 */

#ifndef INC_HEAT_DRIVER_H_
#define INC_HEAT_DRIVER_H_

#include "stm32l0xx_hal.h"

#define NUM_HEATERS 50

//Constants are scaled up by 8 (1 -> 1/8)
#define P 1
#define I 1
#define D 1

#define A0P 3
#define A1P 1
#define A2P 5
#define A3P 6
#define EN0_0P 8
#define EN1_0P 10
#define EN0_1P 0
#define EN1_1P 1
#define ADDR_PORT GPIOA
#define ENX_0_PORT GPIOA
#define ENX_1_PORT GPIOB
#define ADDR_PORT_MODER_VAL (1<<A0P*2 | 1<<A1P*2 | 1<<A2P*2 | 1<<A3P*2)
#define ENX_0_PORT_MODER_VAL ((1<<EN0_0P*2) | (1<<EN1_0P*2))
#define ENX_1_PORT_MODER_VAL ((1<<EN0_1P*2) | (1<<EN1_1P*2))

#define A0SHIFT(x) ((x>>(A0P-0))&(1<<0))
#define A1SHIFT(x) ((x>>(A1P-1))&(1<<1))
#define A2SHIFT(x) ((x>>(A2P-2))&(1<<2))
#define A3SHIFT(x) ((x>>(A3P-3))&(1<<3))

#define SETLOW(x) x << 16
#define SETHIGH(x) x

#define GETEN(x) ((x < 25) ? (x < 16 ? EN0_0P : EN1_0P) : (x < 16 ? EN0_1P : EN1_1P))
#define GETENPORT(x) ((x < 25) ? ENX_0_PORT : ENX_1_PORT)

#define SET_SOME_PINS(x) (SETHIGH(x) | SETLOW(~x))

void init_heater_driver();
void set_all_heaters(uint16_t * outputs);
void set_heater(uint8_t element, uint16_t output);
void step();

#endif /* INC_HEAT_DRIVER_H_ */
