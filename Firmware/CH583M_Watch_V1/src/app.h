/*
 * app.h
 *
 *  Created on: Oct 11, 2023
 *      Author: w
 */

#ifndef SRC_APP_H_
#define SRC_APP_H_

#include "CH58x_common.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "math.h"

#include "oled.h"
#include "key.h"
#include "fifo.h"

#define TASK_NUM    5

//The buffer length should be a power of 2
#define APP_UART_TX_BUFFER_LENGTH    512U
#define APP_UART_RX_BUFFER_LENGTH    2048U

extern uint8_t app_uart_tx_buffer[APP_UART_TX_BUFFER_LENGTH];
extern uint8_t app_uart_rx_buffer[APP_UART_RX_BUFFER_LENGTH];

extern app_drv_fifo_t app_uart_tx_fifo;
extern app_drv_fifo_t app_uart_rx_fifo;

extern uint8_t uart_rx_flag;
extern uint8_t for_uart_rx_black_hole;

#define START_BYTE 0xAA
#define WRITE_OP   0x01
#define READ_OP    0x02

typedef struct {
    uint8_t start;
    uint8_t id;
    uint8_t op;
    uint8_t *data;
    uint8_t checksum;
} Packet;

void app_uart_tx_data(uint8_t *data, uint16_t length);
void app_uart_process(void);
uint16_t crc16_calc(uint8_t *_pBuf, uint16_t _usLen);
void createPacket(uint8_t id, uint8_t op, uint8_t *data, uint8_t dataLength, uint8_t *packet, uint8_t *packetLength);
uint8_t parsePacket(uint8_t *packet, uint8_t packetLength, uint8_t *id, uint8_t *op, uint8_t *data, uint8_t *dataLength);

void HAL_IncTick(void);
void main_init(void);
void main_loop(void);

#endif /* SRC_APP_H_ */
