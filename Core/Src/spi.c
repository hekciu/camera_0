#include "spi.h"
#include "ArduCAM.h"

#include <stdint.h>
#include <stdbool.h>

#define BMPIMAGEOFFSET 66
#define pgm_read_byte(x)        (*((char *)x))
const char bmp_header[BMPIMAGEOFFSET] =
{
  0x42, 0x4D, 0x36, 0x58, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0x00, 0x28, 0x00,
  0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x00, 0x03, 0x00,
  0x00, 0x00, 0x00, 0x58, 0x02, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0xC4, 0x0E, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0xE0, 0x07, 0x00, 0x00, 0x1F, 0x00,
  0x00, 0x00
};

uint8_t	*picbuf = 0;
bool receive_OK = false;
bool USB_sendOK = true;
bool send_OK = true;
uint32_t sendlen = 0;
uint32_t haveRev = 0;
uint32_t noRev = 0;
uint8_t  EP2_SendFinish = 1;
uint8_t	Buf1[BUFFER_MAX_SIZE]={0}, Buf2[BUFFER_MAX_SIZE]={0};
extern uint16_t NumPackage;

extern SPI_HandleTypeDef* camera_spi;


void SPI_CAMERA_Init(void)
{
//	SPI_Cmd(SPI1, ENABLE);
	SPI_CAMERA_ReadWriteByte(0xff);
}

uint8_t SPI_CAMERA_ReadWriteByte(uint8_t TxData)
{
	uint8_t out = 0;

	HAL_SPI_TransmitReceive(camera_spi, &TxData, &out, 1, 3000);

	return out;
}

void DMA1_RX(uint8_t *p , uint32_t len)
{
	CS_LOW();
	set_fifo_burst();

	HAL_SPI_Receive_DMA(camera_spi, p, len);
}
//
//void DMA1_SendtoUsart(uint8_t *p , uint32_t len)
//{
//	DMA1_Channel7->CMAR = (u32)p;
//	DMA1_Channel7->CNDTR = len;
//	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
//	DMA_Cmd(DMA1_Channel7, ENABLE);
//}


void SingleCapTransfer(uint8_t* out, uint32_t out_bfr_size)
{
	flush_fifo();
	clear_fifo_flag();
	start_capture();
	while(!get_bit(ARDUCHIP_TRIG , CAP_DONE_MASK)){;}
	//printf("ACK CMD capture done\r\n");
	length= read_fifo_length();
	//printf("ACK CMD the length is %d\r\n",length);
	sendlen = (length>=BUFFER_MAX_SIZE) ? BUFFER_MAX_SIZE : length;
//	picbuf = Buf1;
	haveRev = 0;

	DMA1_RX(out, out_bfr_size);
}

void StartBMPcapture(void)
{
//	flush_fifo();
//	clear_fifo_flag();
//	start_capture();
//	while(!get_bit(ARDUCHIP_TRIG , CAP_DONE_MASK)){;}
//	//printf("ACK CMD capture done\r\n");
//	length= read_fifo_length();
////	printf("ACK CMD the length is %d\r\n",length);
//	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
//	USART_SendData(USART1, 0xff);
//	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
//	USART_SendData(USART1, 0xaa);
//	for(int temp = 0; temp < BMPIMAGEOFFSET; temp++)
//	{
//		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
//		USART_SendData(USART1, pgm_read_byte(&bmp_header[temp]));
//
//	}
//	CS_LOW();
//	set_fifo_burst();
//	char VH, VL;
//	int i = 0, j = 0;
//	for (i = 0; i < 240; i++)
//	{
//		for (j = 0; j < 320; j++)
//		{
//			VH = SPI1_ReadWriteByte(0x00);
//			VL = SPI1_ReadWriteByte(0x00);
//			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
//			USART_SendData(USART1, VL);
//			delay_us(15);
//			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
//			USART_SendData(USART1, VH);
//			delay_us(15);
//		}
//	}
//	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
//	USART_SendData(USART1, 0xbb);
//	delay_us(12);
//	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
//	USART_SendData(USART1, 0xcc);
//	CS_HIGH();
}

//void PB_SPI_DMA_IRQHandler(void)
//{
//	if(DMA_GetITStatus(DMA1_IT_TC2))
//	{
//		DMA_ClearITPendingBit(DMA1_IT_GL4 | DMA1_IT_TC2 | DMA1_IT_GL5 | DMA1_IT_TC5);
//		DMA_Cmd(PB_SPI_RX_DMA_Channel, DISABLE);
//		DMA_Cmd(DMA1_Channel3, DISABLE);
//		SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx | SPI_I2S_DMAReq_Tx, DISABLE);
//		CS_HIGH();
//		receive_OK =1;
//	}
//}
//
//void DMA1_Channel7_IRQHandler(void)
//{
//	if(DMA_GetITStatus(DMA1_IT_TC7))
//	{
//		DMA_ClearITPendingBit(DMA1_IT_GL7 | DMA1_IT_TC7);
//		DMA_Cmd(DMA1_Channel7, DISABLE);
//		USART_DMACmd(USART2, USART_DMAReq_Tx , DISABLE);
//	}
//}
