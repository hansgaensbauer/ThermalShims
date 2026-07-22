/*
 * tmp11826.c
 *
 *  Created on: Jul 1, 2026
 *      Author: hans
 */

#include "tmp11826.h"
#include "stm32l0xx.h"
#include "stm32l0xx_hal.h"
#ifdef USE_STANDARD_MODE
#include "delays_standardmode.h"
#else
#include "delays.h"
#endif

#include "main.h"

const uint8_t sensor_map[50] = {
		30,40,4,17,25,38,43,44,27,39,32,36,13,9,12,49,1,47,31,24,41,21,10,29,5,
		45,2,23,19,8,20,11,7,14,50,46,6,35,33,22,16,42,37,34,18,26,3,28,48,15
};

const uint64_t device_addresses[MAX_DEVICES] = {
		0x5d5080286a252226,
		0x765160286a252226,
		0xb15090286a252226,
		0xf05f90286a252226,
		0x2a50d0286a252226,
		0xc45070286a252226,
		0x2b5088286a252226,
		0x9f5048286a252226,
		0x5851b8286a252226,
		0xb25078286a252226,
		0x665084286a252226,
		0xda5284286a252226,
		0xd25044286a252226,
		0x6251e4286a252226,
		0xff5074286a252226,
		0x10508c286a252226,
		0x515f8c286a252226,
		0xa4504c286a252226,
		0x8b50cc286a252226,
		0x42522c286a252226,
		0x16515c286a252226,
		0x89507c286a252226,
		0xcc5082286a252226,
		0xc851e2286a252226,
		0x615f92286a252226,
		0xbb50d2286a252226,
		0x555072286a252226,
		0xba508a286a252226,
		0x06528a286a252226,
		0xfb5f8a286a252226,
		0x0e504a286a252226,
		0x2150ca286a252226,
		0x7152da286a252226,
		0x23507a286a252226,
		0xf75086286a252226,
		0x4b5286286a252226,
		0x435046286a252226,
		0x6e5076286a252226,
		0x81508e286a252226,
		0xc05f8e286a252226,
		0x1a50ce286a252226,
		0xd3522e286a252226,
		0x87515e286a252226,
		0x18507e286a252226,
		0x185020296a252226,
		0x6b5110296a252226,
		0x135208296a252226,
		0x235024296a252226,
		0xcf5206296a252226,
		0x76513e296a252226
};
uint8_t num_devices = 50;

void tmp11826_init(){
	debug_printf("Initializing Temp Bus.\n\r");
	ONEWIRE_PORT->OTYPER |= ONEWIRE_PIN; //open drain
	ONEWIRE_PORT->PUPDR |= ONEWIRE_PIN_PUPDR_PULLUP; //open drain
	ONEWIRE_PORT->MODER |= ONEWIRE_PIN_MODER_OUTPUT; //output
	ONEWIRE_PORT->BSRR = ONEWIRE_PIN_SETHIGH; //Write output HIGH
}

//TODO
void read_all_temps(int16_t* sensor_temps){
  for(int i = 0; i < num_devices; i++){
	  sensor_temps[i] = tmp11826_get_temp(sensor_map[i]-1);
  }
}

int tmp11826_get_temp(uint8_t sensor_index){
	uint64_t address = device_addresses[sensor_index];
	onewire_bus_reset();
	//SKIPADDR
	onewire_write_byte(ONEWIRE_CMD_SKIPADDR);
	//CONVERTEMP
	onewire_write_byte(ONEWIRE_CMD_CONVERTEMP);
	//Wait
	HAL_Delay(5);
	//Reset
	onewire_bus_reset();
	//MATCHADDR
	onewire_write_byte(ONEWIRE_CMD_MATCHADDR);
	//SEND ADDRESS
	onewire_write_byte(address & 0xFF);
	onewire_write_byte((address >> 8) & 0xFF);
	onewire_write_byte((address >> 16) & 0xFF);
	onewire_write_byte((address >> 24) & 0xFF);
	onewire_write_byte((address >> 32) & 0xFF);
	onewire_write_byte((address >> 40) & 0xFF);
	onewire_write_byte((address >> 48) & 0xFF);
	onewire_write_byte((address >> 56) & 0xFF);
	//READSCRATCHPAD
	onewire_write_byte(ONEWIRE_CMD_READ_SCRATCHPAD);
	//Read temp
	uint8_t byte_low = onewire_read_byte();
	uint16_t byte_high = onewire_read_byte();
	onewire_bus_reset();

	//return temp
	int tmp = (((int16_t) byte_high << 8 | byte_low) * 625) / 100;
	return tmp;
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
	delay_us(200);
}

uint8_t onewire_read_bit(){
	ONEWIRE_PORT->BSRR = ONEWIRE_PIN_SETLOW; //output low
	DELAY_TRL
	ONEWIRE_PORT->BSRR = ONEWIRE_PIN_SETHIGH; //output high
	DELAY_TRL
	uint8_t value = (ONEWIRE_PORT->IDR & ONEWIRE_PIN) >> ONEWIRE_PIN_OFFSET;

	delay_us(200);
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
	HAL_Delay(1);
}

void onewire_bus_reset(){
//	debug_printf("Resetting Bus.\n\r");
	ONEWIRE_PORT->BSRR = ONEWIRE_PIN_SETLOW; //output low
	DELAY_TRSTL
	ONEWIRE_PORT->BSRR = ONEWIRE_PIN_SETHIGH; //output low
	delay_us(500);
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

//	debug_printf("Found address: %08lx%08lx\n\r",

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
//	debug_printf("CRC: %x\n\r", crc);
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
//	debug_printf("CRC: %x\n\r", crc);
}

/*
uint8_t onewire_search(){

	disable_fast_arbitration();
	enable_fast_arbitration();
	onewire_bus_reset();
	num_devices = 0;

	uint64_t address;
	uint8_t result = onewire_search_pass(&address);
	while(!result){
		device_addresses[num_devices] = address;
		num_devices ++;
		result = onewire_search_pass(&address);
	}

	debug_printf("Found %d devices.\n\r", num_devices);
	disable_fast_arbitration();
	return num_devices;
}
*/

void delay_us(uint16_t us)
{
    TIM6->CNT = 0;
    TIM6->CR1 = TIM_CR1_CEN;

    while (TIM6->CNT < us*DEVICE_FCLK_MHZ) {
        /* spin */
    }

    TIM6->CR1 = 0;
}

