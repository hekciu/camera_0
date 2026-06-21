#ifndef __SPI_H
#define __SPI_H
#include "main.h"
#include <stdint.h>
#include <stdbool.h>
#define  BUFFER_MAX_SIZE 4096
extern uint32_t sendlen ;
extern uint32_t haveRev ;
extern uint32_t noRev;
extern uint8_t *picbuf	;
extern bool receive_OK;
extern bool send_OK ;
extern uint8_t	Buf1[BUFFER_MAX_SIZE];
extern uint8_t	Buf2[BUFFER_MAX_SIZE];
extern uint8_t  EP2_SendFinish;

uint8_t DMA_CAMERA_Init(void);
void SPI_CAMERA_Init(void);
uint8_t SPI_CAMERA_ReadWriteByte(uint8_t TxData);
void SingleCapTransfer(uint8_t* out, uint32_t out_bfr_size);
void StartBMPcapture(void);
#endif
