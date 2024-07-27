/*
 * 93CXX.c
 *
 *  Created on: Jul 27, 2024
 *      Author: Aly Badawy
 */


#include "93CXX.h"


int16_t MEMrData[1];
int16_t MEMsData[1];

// SPI handle (assuming it is defined and initialized elsewhere)
extern SPI_HandleTypeDef hspi1;

void EEPROM_CS_LOW(void) {
    HAL_GPIO_WritePin(EEPROM_CS_GPIO_Port, EEPROM_CS_Pin, GPIO_PIN_RESET);
}

void EEPROM_CS_HIGH(void) {
    HAL_GPIO_WritePin(EEPROM_CS_GPIO_Port, EEPROM_CS_Pin, GPIO_PIN_SET);
}


int EEPROM_IsBusy(void) {
    EEPROM_CS_HIGH();
    HAL_Delay(1); // Small delay to ensure CS low is registered
    int isBusy = (HAL_GPIO_ReadPin(SPI_MISO_GPIO_Port, SPI_MISO_Pin) == GPIO_PIN_RESET);
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
    HAL_Delay(30); // Ensure some delay as per datasheet
    return data;
}

void EEPROM_WriteEnable(void) {
	while (EEPROM_IsBusy());
    EEPROM_CS_HIGH();
    EEPROM_SendCommand(EEPROM_CMD_WREN);
    HAL_Delay(30); // Ensure some delay as per datasheet
    EEPROM_CS_LOW();
}

void EEPROM_WriteDisable(void) {
	while (EEPROM_IsBusy());
    EEPROM_CS_LOW();
    EEPROM_SendCommand(EEPROM_CMD_WRDI);
    HAL_Delay(1);
    EEPROM_CS_HIGH();
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

    HAL_Delay(25); // Wait for write cycle to complete (25 clock cycles)
    EEPROM_WriteDisable();
}

void EEPROM_Erase(uint16_t address) {
	while (EEPROM_IsBusy());
    uint16_t command = EEPROM_CMD_ERASE | (address & 0x03FF); // Ensure address is 10 bits

    EEPROM_WriteEnable();
    EEPROM_CS_HIGH();
    EEPROM_SendCommand(command);
    EEPROM_CS_LOW();
    HAL_Delay(9); // Wait for erase cycle to complete (9 clock cycles)
    EEPROM_WriteDisable();
}

void EEPROM_EraseAll(void) {
	while (EEPROM_IsBusy());
    EEPROM_WriteEnable();
    EEPROM_CS_HIGH();
    EEPROM_SendCommand(EEPROM_CMD_ERASE_ALL);
    EEPROM_CS_LOW();
    HAL_Delay(9); // Wait for erase cycle to complete (9 clock cycles)
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


