/*
 * tmp11826.c
 *
 *  Created on: Jul 1, 2026
 *      Author: hans
 */

#include "tmp11826.h"
#include "stm32l0xx.h"
#include "stm32l0xx_hal.h"
#include "delays.h"
#include "main.h"

uint64_t *device_addresses[MAX_DEVICES];
uint8_t num_devices = 0;

void tmp11826_init(){
	debug_printf("Initializing Temp Bus.\n\r");
	ONEWIRE_PORT->OTYPER |= ONEWIRE_PIN; //open drain
	ONEWIRE_PORT->PUPDR |= ONEWIRE_PIN_PUPDR_PULLUP; //open drain
	ONEWIRE_PORT->MODER |= ONEWIRE_PIN_MODER_OUTPUT; //output
	ONEWIRE_PORT->BSRR = ONEWIRE_PIN_SETHIGH; //Write output HIGH
}

void onewire_write_bit(char bit){
	if(bit){
		ONEWIRE_PORT->BSRR = ONEWIRE_PIN_SETLOW; //output low
		DELAY_TWR1L
		ONEWIRE_PORT->BSRR = ONEWIRE_PIN_SETHIGH; //output high
	}else{
		ONEWIRE_PORT->BSRR = ONEWIRE_PIN_SETLOW; //output low
		DELAY_TWR0L
		ONEWIRE_PORT->BSRR = ONEWIRE_PIN_SETHIGH; //output high
	}
	delay_us(20);
}

uint8_t onewire_read_bit(){
	ONEWIRE_PORT->BSRR = ONEWIRE_PIN_SETLOW; //output low
	DELAY_TRL
	ONEWIRE_PORT->BSRR = ONEWIRE_PIN_SETHIGH; //output high
	DELAY_TRL
	uint8_t value = (ONEWIRE_PORT->IDR & ONEWIRE_PIN) >> ONEWIRE_PIN_OFFSET;

	delay_us(20);
	return value;
}

uint8_t onewire_read_byte(){
	uint8_t byte = 0x00;
	for(uint8_t i = 0; i < 8; i++){
		byte |= onewire_read_bit() << i;
	}
	return byte;
}

void onewire_write_byte(uint8_t byte){
	for(uint8_t i = 0; i < 8; i++){
		onewire_write_bit((byte >> i) & 0x01);
	}
}

void onewire_bus_reset(){
	debug_printf("Resetting Bus.\n\r");
	ONEWIRE_PORT->BSRR = ONEWIRE_PIN_SETLOW; //output low
	DELAY_TRSTL
	ONEWIRE_PORT->BSRR = ONEWIRE_PIN_SETHIGH; //output low
	delay_us(100);
}

uint8_t onewire_search_pass(uint64_t* addr){
	onewire_bus_reset();
	onewire_write_byte(ONEWIRE_CMD_SEARCHADDR);
	*addr = 0;
	*addr |= ((uint64_t) onewire_read_byte());
	*addr |= ((uint64_t) onewire_read_byte()) << 8;
	*addr |= ((uint64_t) onewire_read_byte()) << 16;
	*addr |= ((uint64_t) onewire_read_byte()) << 24;
	*addr |= ((uint64_t) onewire_read_byte()) << 32;
	*addr |= ((uint64_t) onewire_read_byte()) << 40;
	*addr |= ((uint64_t) onewire_read_byte()) << 48;
	*addr |= ((uint64_t) onewire_read_byte()) << 56;

	debug_printf("Found address: %08lx%08lx\n\r",
             (unsigned long)(*addr >> 32),
             (unsigned long)(*addr & 0xFFFFFFFF));
	return *addr == 0xFFFFFFFFFFFFFFFFULL;
}

void enable_fast_arbitration(){
	onewire_bus_reset();
	//Enable fast arbitration
	onewire_write_byte(ONEWIRE_CMD_SKIPADDR);
	onewire_write_byte(ONEWIRE_CMD_SCRATCHPAD);
	onewire_write_byte(ONEWIRE_CONFIG1);
	onewire_write_byte(ONEWIRE_CONFIG2 | FAST_ARBITRATION);
	onewire_write_byte(ONEWIRE_SHORT_ADDR);
	onewire_write_byte(ONEWIRE_ALERT_LOW);
	onewire_write_byte(ONEWIRE_ALERT_LOW_MSB);
	onewire_write_byte(ONEWIRE_WRITE_HIGH_LSB);
	onewire_write_byte(ONEWIRE_WRITE_HIGH_MSB);
	onewire_write_byte(ONEWIRE_WRITE_TEMP_OFFSET_LSB);
	onewire_write_byte(ONEWIRE_WRITE_TEMP_OFFSET_MSB);
	uint8_t crc = onewire_read_byte(); //Read in and drop CRC
	debug_printf("CRC: %x\n\r", crc);
}

void disable_fast_arbitration(){
	onewire_bus_reset();
	//Enable fast arbitration
	onewire_write_byte(ONEWIRE_CMD_SKIPADDR);
	onewire_write_byte(ONEWIRE_CMD_SCRATCHPAD);
	onewire_write_byte(ONEWIRE_CONFIG1);
	onewire_write_byte(ONEWIRE_CONFIG2);
	onewire_write_byte(ONEWIRE_SHORT_ADDR);
	onewire_write_byte(ONEWIRE_ALERT_LOW);
	onewire_write_byte(ONEWIRE_ALERT_LOW_MSB);
	onewire_write_byte(ONEWIRE_WRITE_HIGH_LSB);
	onewire_write_byte(ONEWIRE_WRITE_HIGH_MSB);
	onewire_write_byte(ONEWIRE_WRITE_TEMP_OFFSET_LSB);
	onewire_write_byte(ONEWIRE_WRITE_TEMP_OFFSET_MSB);
	uint8_t crc = onewire_read_byte(); //Read in and drop CRC
	debug_printf("CRC: %x\n\r", crc);
}

uint8_t onewire_search(){

	disable_fast_arbitration();
	enable_fast_arbitration();
	onewire_bus_reset();
	num_devices = 0;

	uint64_t address;
	uint8_t result = 0;
	debug_printf("Search Result: %x\n\r", result);
	while(!result){
		result = onewire_search_pass(&address);
		debug_printf("Search Result: %x\n\r", result);
		device_addresses[num_devices] = address;
		num_devices ++;
	}

	debug_printf("Found %d devices.\n\r", num_devices);
	return num_devices;
}

static inline void delay_us(uint16_t us)
{
    TIM6->CNT = 0;
    TIM6->CR1 = TIM_CR1_CEN;

    while (TIM6->CNT < us*DEVICE_FCLK_MHZ) {
        /* spin */
    }

    TIM6->CR1 = 0;
}

static inline void delay_timer(uint16_t count)
{
    TIM6->CNT = 0;
    TIM6->CR1 = TIM_CR1_CEN;

    while (TIM6->CNT < count) {
        /* spin */
    }

    TIM6->CR1 = 0;
}
