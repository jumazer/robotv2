#include <stdio.h>
#include <assert.h>

#include "../Inc/cbfifo.h"

static cbfifo test_cbfifo;


void fill_queue(char c) {
	for(int i = 0; i < QUEUE_SIZE; i++) {
		int ret = cb_enqueue(&test_cbfifo, c);
		assert(ret != -1);
	}
}

void empty_queue(void) {
	for(int i = 0; i < QUEUE_SIZE; i++) {
		if(cb_length(&test_cbfifo) == 0)
			break;

		cb_dequeue(&test_cbfifo);
	}
}


void test_init(void) {
	cb_init(&test_cbfifo);
	assert(cb_empty(&test_cbfifo) == true);
}

void test_enqueue(void) {
	int ret = cb_enqueue(&test_cbfifo, 'a');
	assert(ret != -1);

	int length = cb_length(&test_cbfifo);
	assert(length == 1);

	assert(test_cbfifo.tail == 1);
}

void test_dequeue(void) {
	char c = cb_dequeue(&test_cbfifo);
	assert(c == 'a');

	assert(test_cbfifo.head == 1);
}

void test_dequeue_empty(void) {
	char c = cb_dequeue(&test_cbfifo);
	assert(c == 0);
}

void test_enqueue_full(void) {
	int ret;
	fill_queue('a');

	ret = cb_enqueue(&test_cbfifo, 'a');
	assert(ret == -1);

	empty_queue();

	int length = cb_length(&test_cbfifo);
	assert(length == 0);
}

void test_head_tail_ptr(void) {
	fill_queue('a');
	uint8_t tail_index = test_cbfifo.tail;
	assert(tail_index == 0);

	uint8_t head_index = test_cbfifo.head;
	assert(head_index == 0);

	char c;
	for(int i = 0; i < QUEUE_SIZE - 1; i++) {
		c = cb_dequeue(&test_cbfifo);
	}
	assert(c == 'a');
	head_index = test_cbfifo.head;
	assert(head_index == QUEUE_SIZE - 1);
}

int main(void) {
	test_init();
	test_enqueue();
	test_dequeue();


	test_init();
	test_dequeue_empty();
	test_enqueue_full();

	test_init();
	test_head_tail_ptr();

	test_init();

}
