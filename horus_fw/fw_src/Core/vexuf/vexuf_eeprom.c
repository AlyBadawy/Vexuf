/*
 * 93CXX.c
 *
 *  Created on: Jul 27, 2024
 *      Author: Aly Badawy
 */


#include "vexuf_eeprom.h"

#define EEPROM_CMD_READ  		0x1800
#define EEPROM_CMD_WRITE 		0x1400
#define EEPROM_CMD_ERASE 		0x1C00
#define EEPROM_CMD_WREN  		0x1300
#define EEPROM_CMD_WRDI  		0x1000
#define EEPROM_CMD_ERASE_ALL 	0x1200

#define MEM_DelayNUM			2
#define MEM_DeLay()				for(uint16_t i=0;i<MEM_DelayNUM;i++)asm("NOP")
#define MEM_Timeout				1000


int16_t MEMrData[1];
int16_t MEMsData[1];


void EEPROM_CS_LOW(void);
void EEPROM_CS_HIGH(void);
bool EEPROM_IsBusy(void);
void EEPROM_SendCommand(uint16_t command);
void EEPROM_WriteEnable(void);
void EEPROM_WriteDisable(void);



// SPI handle (assuming it is defined and initialized elsewhere)
extern SPI_HandleTypeDef hspi1;

void EEPROM_CS_LOW(void) {
    HAL_GPIO_WritePin(EEPROM_CS_GPIO_Port, EEPROM_CS_Pin, GPIO_PIN_RESET);
}

void EEPROM_CS_HIGH(void) {
    HAL_GPIO_WritePin(EEPROM_CS_GPIO_Port, EEPROM_CS_Pin, GPIO_PIN_SET);
}


bool EEPROM_IsBusy(void) {
    EEPROM_CS_HIGH();
    bool isBusy = (HAL_GPIO_ReadPin(SPI_MISO_GPIO_Port, SPI_MISO_Pin) == GPIO_PIN_RESET);
    EEPROM_CS_LOW();
    return isBusy;
}

uint8_t SPI_TransmitReceive(uint8_t data) {
    uint8_t receivedData;
    HAL_SPI_TransmitReceive(&hspi1, &data, &receivedData, 1, 150);
    return receivedData;
}


void EEPROM_SendCommand(uint16_t command) {
    uint8_t cmdHigh = (command >> 8) & 0xFF;
    uint8_t cmdLow = command & 0xFF;
    SPI_TransmitReceive(cmdHigh);
    SPI_TransmitReceive(cmdLow);
}

uint16_t EEPROM_Read(uint16_t address) {
	while (EEPROM_IsBusy());
    uint16_t command = EEPROM_CMD_READ | (address & 0x03FF); // Ensure address is 10 bits
    uint16_t data = 0;
    uint8_t temp = 0;

    EEPROM_CS_HIGH();
    EEPROM_SendCommand(command);

	// Receive the first byte which contains the dummy bit and the high part of the data
	temp = SPI_TransmitReceive(0x00);
	data |= (temp & 0x7F) << 9; // Mask out the dummy bit and shift left to align with the 16-bit data

	// Receive the second byte and combine it with the first byte's data
	data |= SPI_TransmitReceive(0x00) << 1;

	// Receive the remaining 7 bits of data
	temp = SPI_TransmitReceive(0x00);
    data |= (temp >> 7);
    HAL_Delay(1); // Ensure some delay as per datasheet
    EEPROM_CS_LOW();
    return data;
}

void EEPROM_WriteEnable(void) {
	while (EEPROM_IsBusy());
    EEPROM_CS_HIGH();
    EEPROM_SendCommand(EEPROM_CMD_WREN);
    HAL_Delay(2); // Ensure some delay as per datasheet
    EEPROM_CS_LOW();
}

void EEPROM_WriteDisable(void) {
	while (EEPROM_IsBusy());
    EEPROM_CS_HIGH();
    EEPROM_SendCommand(EEPROM_CMD_WRDI);
    HAL_Delay(1);
    EEPROM_CS_LOW();
}

void EEPROM_Write(uint16_t address, uint16_t data) {
	while (EEPROM_IsBusy());
    uint16_t command = EEPROM_CMD_WRITE | (address & 0x03FF); // Ensure address is 10 bits

    EEPROM_WriteEnable();
    EEPROM_CS_HIGH();
    EEPROM_SendCommand(command);

    // Send 16-bit data
    SPI_TransmitReceive((data >> 8) & 0xFF); // Send high byte
    SPI_TransmitReceive(data & 0xFF); // Send low byte
    EEPROM_CS_LOW();

    HAL_Delay(2); // Wait for write cycle to complete
    EEPROM_WriteDisable();
}

void EEPROM_Erase(uint16_t address) {
	while (EEPROM_IsBusy());
    uint16_t command = EEPROM_CMD_ERASE | (address & 0x03FF); // Ensure address is 10 bits

    EEPROM_WriteEnable();
    EEPROM_CS_HIGH();
    EEPROM_SendCommand(command);
    EEPROM_CS_LOW();
    HAL_Delay(2); // Wait for erase cycle to complete (9 clock cycles)
    EEPROM_WriteDisable();
}

void EEPROM_EraseAll(void) {
	while (EEPROM_IsBusy());
    EEPROM_WriteEnable();
    EEPROM_CS_HIGH();
    EEPROM_SendCommand(EEPROM_CMD_ERASE_ALL);
    EEPROM_CS_LOW();
    HAL_Delay(2); // Wait for erase cycle to complete
    EEPROM_WriteDisable();
}

void EEPROM_ReadMultipleWords(uint16_t startAddress, uint16_t* buffer, uint16_t length) {
    for (uint16_t i = 0; i < length; i++) {
        buffer[i] = EEPROM_Read(startAddress + i);
    }
}

void EEPROM_WriteMultipleWords(uint16_t startAddress, uint16_t* buffer, uint16_t length) {
    for (uint16_t i = 0; i < length; i++) {
        EEPROM_Write(startAddress + i, buffer[i]);
    }
}

void EEPROM_WriteBool(uint16_t address, bool value) {
	uint16_t data = value ? 1 : 0; // Convert bool to uint16_t
	EEPROM_Write(address, data);
}

bool EEPROM_ReadBool(uint16_t address) {
	return EEPROM_Read(address) != 0;
}

void EEPROM_Test(void) {
	// EEPROM READ TEST
	// Example usage
	uint16_t data = EEPROM_Read(0xFFEE);
	EEPROM_Write(0xFFEE, 0xABCD);
	data = EEPROM_Read(0xFFEE);
	printf("EEprom says: %04X\r\n", data);



	uint16_t buffer[0x3FF];
	for (uint16_t address = 0; address < 0x3FF; address++) {
		buffer[address] = EEPROM_Read(address);
	}
	for (uint16_t address = 0; address < 0x3FF; address += 16) {
		printf("%10X: ", address);
		// Print first column (8 words)
		for (uint16_t i = 0; i < 8; i++) {
			printf("%04X ", buffer[address + i]);
		}

		// Print separator
		printf("    ");

		// Print second column (8 words)
		for (uint16_t i = 8; i < 16; i++) {
			printf("%04X ", buffer[address + i]);
		}

		// New line
		printf("\r\n");



	}

	char writeData[25] = "Hello, EEPROM! Testing";
	char readData[25];
	uint16_t writeBuffer[13]; // 25 characters will be stored in 13 words (16-bit each)
	uint16_t readBuffer[13];

	// Convert char array to uint16_t array
	for (int i = 0; i < 12; i++) {
		writeBuffer[i] = (writeData[2 * i] << 8) | writeData[2 * i + 1];
	}
	// Handle the last character (if array size is odd)
	writeBuffer[12] = writeData[24];

	// Write to EEPROM
	EEPROM_WriteMultipleWords(0x0000, writeBuffer, 13);

	// Read from EEPROM
	EEPROM_ReadMultipleWords(0x0000, readBuffer, 13);

	// Convert uint16_t array back to char array
	for (int i = 0; i < 12; i++) {
		readData[2 * i] = (readBuffer[i] >> 8) & 0xFF;
		readData[2 * i + 1] = readBuffer[i] & 0xFF;
	}
	// Handle the last character (if array size is odd)
	readData[24] = readBuffer[12] & 0xFF;

	// Print the read data
	printf("Read Data: %s\r\n", readData);
}


