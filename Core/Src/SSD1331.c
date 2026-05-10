/*
 * SSD1331.c
 *
 *  Created on: May 10, 2026
 *      Author: jakub
 */

#include <stdint.h>

#include "main.h"

#include "SSD1331.h"


static void SSD1331_Reset_Low() {
	HAL_GPIO_WritePin(RES_DISPLAY_GPIO_Port, RES_DISPLAY_Pin, GPIO_PIN_RESET);
}

static void SSD1331_Reset_High() {
	HAL_GPIO_WritePin(RES_DISPLAY_GPIO_Port, RES_DISPLAY_Pin, GPIO_PIN_SET);
}

static void SSD1331_Select() {
	HAL_GPIO_WritePin(CS_DISPLAY_GPIO_Port, CS_DISPLAY_Pin, GPIO_PIN_RESET);
}

static void SSD1331_Deselect() {
	HAL_GPIO_WritePin(CS_DISPLAY_GPIO_Port, CS_DISPLAY_Pin, GPIO_PIN_SET);
}

void SSD1331_Send_Command(SPI_HandleTypeDef* hspi, uint8_t data) {
	HAL_GPIO_WritePin(DC_DISPLAY_GPIO_Port, DC_DISPLAY_Pin, GPIO_PIN_RESET);
	SSD1331_Select();

	HAL_SPI_Transmit(hspi, &data, 1, HAL_MAX_DELAY);

	SSD1331_Deselect();

}

void SSD1331_Send_Data(SPI_HandleTypeDef* hspi, uint8_t* data, uint16_t len) {
	HAL_GPIO_WritePin(DC_DISPLAY_GPIO_Port, DC_DISPLAY_Pin, GPIO_PIN_SET);

	SSD1331_Select();

	HAL_SPI_Transmit(hspi, data, len, HAL_MAX_DELAY);

	SSD1331_Deselect();
}

void SSD1331_Set_Contrast_A(SPI_HandleTypeDef* hspi, uint8_t value) {
	SSD1331_Send_Command(hspi, 0x81); SSD1331_Send_Command(hspi, value);
}

void SSD1331_Set_Contrast_B(SPI_HandleTypeDef* hspi, uint8_t value) {
	SSD1331_Send_Command(hspi, 0x82); SSD1331_Send_Command(hspi, value);
}

void SSD1331_Set_Contrast_C(SPI_HandleTypeDef* hspi, uint8_t value) {
	SSD1331_Send_Command(hspi, 0x83); SSD1331_Send_Command(hspi, value);
}

void SSD1331_Initialize(SPI_HandleTypeDef* hspi) {
	SSD1331_Reset_Low(hspi);
	HAL_Delay(10);

	SSD1331_Reset_High(hspi);
	HAL_Delay(50);

	// Initialization sequence
	SSD1331_Send_Command(hspi, 0xAE);              // Display off
	SSD1331_Send_Command(hspi, 0xA0); SSD1331_Send_Command(hspi, 0x72); // Set Remap & Color Depth
	SSD1331_Send_Command(hspi, 0xA1); SSD1331_Send_Command(hspi, 0x00); // Set Display Start Line
	SSD1331_Send_Command(hspi, 0xA2); SSD1331_Send_Command(hspi, 0x00); // Set Display Offset
	SSD1331_Send_Command(hspi, 0xA4);              // Normal Display (not all-on)
	SSD1331_Send_Command(hspi, 0xA8); SSD1331_Send_Command(hspi, 0x3F); // Set Multiplex Ratio
	SSD1331_Send_Command(hspi, 0xAD); SSD1331_Send_Command(hspi, 0x8E); // Master Config
	SSD1331_Send_Command(hspi, 0xB0); SSD1331_Send_Command(hspi, 0x0B); // Power Save Mode
	SSD1331_Send_Command(hspi, 0xB1); SSD1331_Send_Command(hspi, 0x31); // Phase Period Adjust
	SSD1331_Send_Command(hspi, 0xB3); SSD1331_Send_Command(hspi, 0xF0); // Clock Divider / Oscillator
	SSD1331_Send_Command(hspi, 0x8A); SSD1331_Send_Command(hspi, 0x64); // Precharge A
	SSD1331_Send_Command(hspi, 0x8B); SSD1331_Send_Command(hspi, 0x78); // Precharge B
	SSD1331_Send_Command(hspi, 0x8C); SSD1331_Send_Command(hspi, 0x64); // Precharge C
	SSD1331_Send_Command(hspi, 0xBB); SSD1331_Send_Command(hspi, 0x3A); // Precharge Level
	SSD1331_Send_Command(hspi, 0xBE); SSD1331_Send_Command(hspi, 0x3E); // VCOMH
	SSD1331_Send_Command(hspi, 0x87); SSD1331_Send_Command(hspi, 0x06); // Master Current
	SSD1331_Set_Contrast_A(hspi, 0x91);
	SSD1331_Set_Contrast_B(hspi, 0x50);
	SSD1331_Set_Contrast_C(hspi, 0x7D);
	SSD1331_Send_Command(hspi, 0xAF);              // Display ON
}

void SSD1331_Draw_Pixel(SPI_HandleTypeDef* hspi, uint8_t x, uint8_t y, uint16_t color) {
	// set column
	SSD1331_Send_Command(hspi, 0x15);
	SSD1331_Send_Command(hspi, x);
	SSD1331_Send_Command(hspi, x);

	// set row
	SSD1331_Send_Command(hspi, 0x75);
	SSD1331_Send_Command(hspi, y);
	SSD1331_Send_Command(hspi, y);

	// send pixel color (in RGB565 format)
	uint8_t data[2] = {
	    color >> 8,
		color & 0xFF
	};

	SSD1331_Send_Data(hspi, data, 2);
}

void SSD1331_Draw_Whole_Image(SPI_HandleTypeDef* hspi, uint8_t* data) {
	for (size_t w = 0; w < SSD1331_WIDTH; w++) {
		 for (size_t h = 0; h < SSD1331_HEIGHT; h++){
			size_t index = w * SSD1331_WIDTH + h;

			uint8_t pixel_low_byte = *(data + (index * 2));
			uint8_t pixel_high_byte = *(data + (index * 2 + 1));

			uint16_t pixel = ((uint16_t)pixel_high_byte << 8) + (uint16_t)pixel_low_byte;

			SSD1331_Draw_Pixel(hspi, w, h, pixel);
		}
	}
}
