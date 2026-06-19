/*
 * cbfifo.c
 *
 *  Created on: Jun 14, 2026
 *      Author: julius
 */


#include "cbfifo.h"
#include <string.h>

bool cb_empty(cbfifo *cb) {
	if(cb->length == 0)
		return true;
	else
		return false;
}

bool cb_full(cbfifo *cb) {
	if(cb->length == QUEUE_SIZE)
		return true;
	else
		return false;
}

int cb_length(cbfifo *cb) {
	return cb->length;
}

void cb_init(cbfifo *cb) {
	cb->head = 0;
	cb->tail = 0;
	cb->length = 0;
	memset(cb->buffer, 0, sizeof(cb->buffer));
}

int cb_enqueue(cbfifo *cb, unsigned char c) {
	int ret = -1;
	if(!cb_full(cb)) {
		cb->buffer[cb->tail] = c;
		cb->tail = (cb->tail + 1) & (QUEUE_SIZE - 1);
		cb->length++;
		ret = 1;
	}

	return ret;
}

unsigned char cb_dequeue(cbfifo *cb) {
	unsigned char c = 0;
	if(!cb_empty(cb)) {
		c = cb->buffer[cb->head];
		cb->head = (cb->head + 1) & (QUEUE_SIZE - 1);
		cb->length--;
	}
	return c;
}


