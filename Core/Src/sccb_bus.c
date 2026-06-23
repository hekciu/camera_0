/**
  * @file    sccb_bus.c
  * @author  Arducam
  * @version V0.1
  * @date    2018.06.22
  * @brief   sccb bus
  */
#include "sccb_bus.h"
#include "delay.h"

#include <stdint.h>
 uint32_t ntime;



void SCCB_SIC_H() { HAL_GPIO_WritePin(GPIOA, SIO_C_Pin, GPIO_PIN_SET); } // SCL H
void SCCB_SIC_L() { HAL_GPIO_WritePin(GPIOA, SIO_C_Pin, GPIO_PIN_RESET); }	 	//SCL L
void SCCB_SID_H() { HAL_GPIO_WritePin(GPIOA, SIO_D_Pin, GPIO_PIN_SET); }   //SDA	H
void SCCB_SID_L() { HAL_GPIO_WritePin(GPIOA, SIO_D_Pin, GPIO_PIN_RESET); }  //SDA	L


#define SCCB_SIC_H() { GPIOA->BSRR = SIO_C_Pin; } // SCL H
#define SCCB_SIC_L() { GPIOA->BSRR = (SIO_C_Pin >> 8); }	 	//SCL L
#define SCCB_SID_H() { GPIOA->BSRR = SIO_D_Pin; }   //SDA	H
#define SCCB_SID_L() { GPIOA->BSRR = (SIO_D_Pin >> 8); }  //SDA	L


void SCCB_DATA_IN()
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = SIO_D_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void SCCB_DATA_OUT()
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = SIO_D_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

//GPIO_PinState SCCB_SID_STATE() { return HAL_GPIO_ReadPin(GPIOA, SIO_D_Pin); }

#define SCCB_SID_STATE() (GPIOA->IDR & SIO_D_Pin)

//#define SCCB_SID_STATE() (1)

/**
  * @brief  init i2c bus
  * @param  None
  * @retval None
  */
void sccb_bus_init(void)

{
	SCCB_DATA_OUT();

    SCCB_SID_H();
    SCCB_SIC_H();
}




void sccb_bus_start(void)
{
	SCCB_DATA_OUT();

    SCCB_SID_H();
    delay_us(I2C_TIM);
//    SCCB_SIC_H();
//    delay_us(I2C_TIM);
//    SCCB_SID_L();
//    delay_us(I2C_TIM);
//    SCCB_SIC_L();
//    delay_us(I2C_TIM);
}


void sccb_bus_stop(void)
{
    SCCB_SID_L();
    delay_us(I2C_TIM);
    SCCB_SIC_H();
    delay_us(I2C_TIM);
    SCCB_SID_H();
    delay_us(I2C_TIM);
}


void sccb_bus_send_noack(void)
{
	SCCB_SID_H();
	delay_us(I2C_TIM);
	SCCB_SIC_H();
	delay_us(I2C_TIM);
	SCCB_SIC_L();
	delay_us(I2C_TIM);
	SCCB_SID_L();
	delay_us(I2C_TIM);
}

void sccb_bus_send_ack(void)
{
	SCCB_SID_L();
	delay_us(I2C_TIM);
	SCCB_SIC_L();
	delay_us(I2C_TIM);
	SCCB_SIC_H();
	delay_us(I2C_TIM);
	SCCB_SIC_L();
	delay_us(I2C_TIM);
	SCCB_SID_L();
	delay_us(I2C_TIM);
}

uint8_t sccb_bus_write_byte(uint8_t data)
{
	uint32_t i;
	uint8_t tem;

	for(i = 0; i < 8; i++)
	{
		if((data<<i) & 0x80)
		{
			SCCB_SID_H();
		}
		else
		{
			SCCB_SID_L();
		}
		delay_us(I2C_TIM);
		SCCB_SIC_H();
		delay_us(I2C_TIM);
		SCCB_SIC_L();

	}
	SCCB_DATA_IN();
	delay_us(I2C_TIM);
	SCCB_SIC_H();
	delay_us(I2C_TIM);
	if(SCCB_SID_STATE())
	{
		tem = 0;
	}
	else
	{
		tem = 1;
	}

	SCCB_SIC_L();
	delay_us(I2C_TIM);
	SCCB_DATA_OUT();
	return tem;
}

uint8_t sccb_bus_read_byte(void)
{
	uint32_t i;
	uint8_t read = 0;

	SCCB_DATA_IN();
	delay_us(I2C_TIM);
	for(i = 8; i > 0; i--)
	{
		delay_us(I2C_TIM);
		SCCB_SIC_H();
		delay_us(I2C_TIM);
		read = read << 1;
		if(SCCB_SID_STATE())
		{
			read += 1;
		}
		SCCB_SIC_L();
		delay_us(I2C_TIM);
	}
    SCCB_DATA_OUT();
	return read;
}
