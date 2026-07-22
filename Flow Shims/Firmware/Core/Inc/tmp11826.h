/*
 * tmp11826.h
 *
 *  Created on: Jul 1, 2026
 *      Author: hans
 */

#ifndef INC_TMP11826_H_
#define INC_TMP11826_H_

#include "stm32l0xx_hal.h"

#define USE_STANDARD_MODE

#define DEVICE_FCLK_MHZ 16
#define MAX_DEVICES 64

#define ONEWIRE_PORT GPIOA
#define ONEWIRE_PIN_OFFSET 9
#define ONEWIRE_PIN (1<<ONEWIRE_PIN_OFFSET)
#define ONEWIRE_PIN_SETLOW (1<<(ONEWIRE_PIN_OFFSET+16))
#define ONEWIRE_PIN_SETHIGH (1<<ONEWIRE_PIN_OFFSET)
#define ONEWIRE_PIN_MODER_OUTPUT (1<<2*ONEWIRE_PIN_OFFSET)
#define ONEWIRE_PIN_PUPDR_PULLUP (1<<2*ONEWIRE_PIN_OFFSET)

#define ONEWIRE_CMD_SKIPADDR 0xCC
#define ONEWIRE_CMD_SCRATCHPAD 0x4E
#define ONEWIRE_CMD_SEARCHADDR 0xF0
#define ONEWIRE_CMD_CONVERTEMP 0x44
#define ONEWIRE_CMD_MATCHADDR 0x55
#define ONEWIRE_CMD_READ_SCRATCHPAD 0xBE

#define ONEWIRE_CONFIG1 0x70
#define ONEWIRE_CONFIG2 0x80
#define ONEWIRE_SHORT_ADDR 0x00
#define ONEWIRE_ALERT_LOW 0x00
#define ONEWIRE_ALERT_LOW_MSB 0x00
#define ONEWIRE_WRITE_HIGH_LSB 0xF0
#define ONEWIRE_WRITE_HIGH_MSB 0x07
#define ONEWIRE_WRITE_TEMP_OFFSET_LSB 0x00
#define ONEWIRE_WRITE_TEMP_OFFSET_MSB 0x00

#define FAST_ARBITRATION (0b11 << 3)

extern TIM_HandleTypeDef htim6;
#define US_DELAY_TIM htim6;

void read_all_temps(int16_t* sensor_temps);
void onewire_write_bit(char bit);
uint8_t onewire_read_bit();
uint8_t onewire_read_byte();
void onewire_write_byte(uint8_t byte);
void delay_us(uint16_t us);
void tmp11826_init();
void disable_fast_arbitration();
void enable_fast_arbitration();
int tmp11826_get_temp(uint8_t sensor_index);
void onewire_bus_reset();
uint8_t onewire_search_pass(uint64_t* addr);
uint8_t onewire_search();

#endif /* INC_TMP11826_H_ */
