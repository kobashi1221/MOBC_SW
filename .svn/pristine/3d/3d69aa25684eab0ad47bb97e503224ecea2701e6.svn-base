#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

#include <stdint.h> //for uint8_t
#include <stdbool.h> //for boolean
#include <string.h> // for memcpy
#include "../../TlmCmd/user_packet_handler.h"
#include "src_core/System/TimeManager/time_manager.h"


typedef struct{
	uint8_t data[TCP_MAX_LEN];
} Buffer;

typedef struct{
	uint32_t head;
	uint32_t tail;
	uint8_t length;
	Buffer* buffer;
}Queue_LIST;

typedef struct{
	uint16_t byte_count;
	uint8_t receive_buf[TCP_MAX_LEN];
	uint8_t byte_buf;
	size_t len;
	ObcTime first_rcv_time;
}inturrupt_driver;

typedef struct{
	Queue_LIST list;
	inturrupt_driver dri;
}Queue;

typedef enum
{
	QUEUE_OK,
	QUEUE_INVALID,
	QUEUE_IS_FULL,
	QUEUE_IS_EMPTY,
} QUEUE_ACK;

void queue_init(Queue* queue, size_t length, Buffer* buf);
void queue_reset(Queue_LIST* list);
void queue_count_reset(Queue* queue);
QUEUE_ACK enqueue(Queue_LIST* list, uint8_t* tcp);
QUEUE_ACK dequeue(Queue_LIST* list, TCP* tcp);
QUEUE_ACK isFull(Queue_LIST* list);
QUEUE_ACK isEmpty(Queue_LIST* list);
void Callback_(Queue* queue);
void Callback_Packet(Queue* queue, size_t Size);
void Spi_Callback_Packet(Queue* queue, size_t Size);
size_t get_packet_len(uint8_t* packet);

#endif
