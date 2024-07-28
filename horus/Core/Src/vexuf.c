/*
 * vexuf.c
 *
 *  Created on: Jul 27, 2024
 *      Author: Aly Badawy
 */


#include "vexuf.h"

extern I2C_HandleTypeDef hi2c1;

static const char custom_base32_alphabet[] = "23456789ABCDEFGHJKLMNPQRSTUVWXYZ";

float cToF(float c) {
	return (c * (9.0 / 5.0)) + 32.0;
}

float fToC(float f) {
	return (f - 32.0) * (5.0 / 9.0);
}

void base32_encode(const uint8_t *data, size_t length, char *output) {
    int index, bitbuffer, bits_in_buffer;
    size_t i;
    const uint8_t *current_byte;

    index = 0;
    bitbuffer = 0;
    bits_in_buffer = 0;
    current_byte = data;

    for (i = 0; i < length; i++) {
        bitbuffer = (bitbuffer << 8) | *current_byte++;
        bits_in_buffer += 8;

        while (bits_in_buffer >= 5) {
            output[index++] = custom_base32_alphabet[(bitbuffer >> (bits_in_buffer - 5)) & 0x1F];
            bits_in_buffer -= 5;
        }
    }

    if (bits_in_buffer > 0) {
        output[index++] = custom_base32_alphabet[(bitbuffer << (5 - bits_in_buffer)) & 0x1F];
    }

    output[index] = '\0';
}

void generate_serial_number(char *serial_number) {
	char serial[20];
	uint32_t uid[3];
    uint8_t uid_bytes[12];

    uid[2] = HAL_GetUIDw0();
    uid[1] = HAL_GetUIDw1();
    uid[0] = HAL_GetUIDw2();

    for (int i = 0; i < 3; i++) {
    	uid_bytes[4*i] = (uid[i] >> 24) & 0xFF;
    	uid_bytes[4*i + 1] = (uid[i] >> 16) & 0xFF;
    	uid_bytes[4*i + 2] = (uid[i] >> 8) & 0xFF;
    	uid_bytes[4*i + 3] = uid[i] & 0xFF;
	}

    base32_encode(uid_bytes, 12, serial);

    int i, j;
    int length = strlen(serial);
    for (i = 0, j = 0; i < length; i++) {
        if (i > 0 && i % 5 == 0) {
        	serial_number[j++] = '-';
        }
        serial_number[j++] = serial[i];
    }

    serial_number[j] = '\0';
}

void SerialNumber_test(void) {
	char serial_number[25];
	generate_serial_number(serial_number);
	printf("===================================================\r\n\r\n");
	printf("   VexUF:Horus\r\n\r\n");
	printf("   Serial Number: %s\r\n\r\n", serial_number);
	printf("===================================================\r\n");
}

void temperatureInternal_Test(void) {
	if (AHT20_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}

	float temperature = 0.0f;
	float humidity = 0.0f;
	// Read temperature and humidity from AHT20
	printf("Internal AHT20 sensor:\r\n");
	if (AHT20_ReadTemperatureHumidity(&hi2c1, &temperature, &humidity) == HAL_OK) {
		printf("  Temperature Internal C: %0.2f\r\n", temperature);
		printf("  Temperature Internal F: %0.2f\r\n", cToF(temperature));
		printf("  Humidity Internal %%: %0.2f\r\n", humidity);
	} else {
		printf("  Error reading from AHT20\n");
}

}
