/*
 * SSD1331.h
 *
 *  Created on: May 10, 2026
 *      Author: hekciu
 */

#ifndef INC_SSD1331_H_
#define INC_SSD1331_H_


#define SSD1331_WIDTH 96
#define SSD1331_HEIGHT 64


void SSD1331_Send_Command(SPI_HandleTypeDef* hspi, uint8_t data);

void SSD1331_Send_Data(SPI_HandleTypeDef* hspi, uint8_t* data, uint16_t len);

void SSD1331_Set_Contrast_A(SPI_HandleTypeDef* hspi, uint8_t value);

void SSD1331_Set_Contrast_B(SPI_HandleTypeDef* hspi, uint8_t value);

void SSD1331_Set_Contrast_C(SPI_HandleTypeDef* hspi, uint8_t value);

void SSD1331_Initialize(SPI_HandleTypeDef* hspi);

void SSD1331_Draw_Pixel(SPI_HandleTypeDef* hspi, uint8_t x, uint8_t y, uint16_t color);

void SSD1331_Draw_Whole_Image(SPI_HandleTypeDef* hspi, uint8_t* data);


#endif /* INC_SSD1331_H_ */
