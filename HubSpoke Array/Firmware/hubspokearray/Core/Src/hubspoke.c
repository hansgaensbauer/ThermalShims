

#include "hubspoke.h"
#include "main.h"


void select_temp(uint8_t element){

	HAL_GPIO_WritePin(CS_TEMP_GPIO_Port, CS_TEMP_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(WR_GPIO_Port, WR_Pin, GPIO_PIN_RESET);

	uint8_t address = temp_remap[element];
	HAL_GPIO_WritePin(A0_GPIO_Port, A0_Pin, address & 0x01);
	HAL_GPIO_WritePin(A1_GPIO_Port, A1_Pin, address & 0x02);
	HAL_GPIO_WritePin(A2_GPIO_Port, A2_Pin, address & 0x04);
	HAL_GPIO_WritePin(A3_GPIO_Port, A3_Pin, address & 0x08);
	HAL_GPIO_WritePin(A4_GPIO_Port, A4_Pin, address & 0x10);

	HAL_Delay(1);
	HAL_GPIO_WritePin(WR_GPIO_Port, WR_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(CS_TEMP_GPIO_Port, CS_TEMP_Pin, GPIO_PIN_SET);
}

void set_heater(uint8_t element, uint8_t value){
	HAL_GPIO_WritePin(CS_DRIVE_GPIO_Port, CS_DRIVE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(WR_GPIO_Port, WR_Pin, GPIO_PIN_RESET);

	uint8_t address = heater_remap[element];
	HAL_GPIO_WritePin(A0_GPIO_Port, A0_Pin, address & 0x01);
	HAL_GPIO_WritePin(A1_GPIO_Port, A1_Pin, address & 0x02);
	HAL_GPIO_WritePin(A2_GPIO_Port, A2_Pin, address & 0x04);
	HAL_GPIO_WritePin(A3_GPIO_Port, A3_Pin, address & 0x08);
	HAL_GPIO_WritePin(A4_GPIO_Port, A4_Pin, address & 0x10);

	HAL_GPIO_WritePin(DAC_GPIO_Port, DAC_Pin, value);
//	HAL_NVIC_DisableIRQ(USART1_IRQn);
//	HAL_NVIC_DisableIRQ(USART2_IRQn);
	heater_drive_on();
	HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DAC_GPIO_Port, DAC_Pin, value);
	HAL_Delay(1);
	HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_SET);
	heater_drive_off();
//	HAL_NVIC_EnableIRQ(USART1_IRQn);
//	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_GPIO_WritePin(CS_DRIVE_GPIO_Port, CS_DRIVE_Pin, GPIO_PIN_SET);
}

void heater_drive_off(){
	GPIO_InitTypeDef Gate_HighZStruct = {0};

	// Assume GPIOA, Pin 5 for example
	Gate_HighZStruct.Pin = DAC_Pin;
	Gate_HighZStruct.Mode = GPIO_MODE_INPUT;
	Gate_HighZStruct.Pull = GPIO_NOPULL;

	HAL_GPIO_Init(DAC_GPIO_Port, &Gate_HighZStruct);
}

void heater_drive_on(){
	GPIO_InitTypeDef Gate_DriveStruct = {0};

	// Assume GPIOA, Pin 5 for example
	Gate_DriveStruct.Pin = DAC_Pin;
	Gate_DriveStruct.Mode = GPIO_MODE_OUTPUT_PP;
	Gate_DriveStruct.Pull = GPIO_NOPULL;
	Gate_DriveStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(DAC_GPIO_Port, &Gate_DriveStruct);
}

uint32_t read_temp(uint8_t element){
	select_temp(element);
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
	return(HAL_ADC_GetValue(&hadc));
}

void read_all(uint32_t * buffer){

	HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_RESET);
	for(uint8_t i = 0; i < 32; i++){
		buffer[i] = read_temp(i);
	}
#ifdef MAIN_BOARD
	HAL_UART_Transmit(&huart1, (uint8_t *)"rt\n", 3, HAL_MAX_DELAY); //Trigger a read from the other board
	HAL_UART_Receive(&huart1, (uint8_t *) &buffer[32], 4*32, 1000);
#endif
	HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_SET);
}
