#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#include "../Inc/command_processor.h"
#include "../Inc/cbfifo.h"
#include "fake_robot_actions.h"

void test_forward(void) {
	fake_robot_actions_reset();
	command_status_t status = run_command("w", 20);
	assert(status == CMD_STATUS_OK);
	assert(fake_forward_called == true);
	assert(fake_forward_speed == 20);
}

void test_backward(void) {
	fake_robot_actions_reset();
	command_status_t status = run_command("b", 30);
	assert(status == CMD_STATUS_OK);
	assert(fake_backward_called == true);
	assert(fake_backward_speed == 30);
}

void test_stop(void) {
	fake_robot_actions_reset();
	command_status_t status = run_command("q", 0);
	assert(status == CMD_STATUS_OK);
	assert(fake_stop_called == true);
	assert(fake_stop_speed == 0);
}

void test_bad_command(void) {
	command_status_t status = run_command("bad", 0);
	assert(status == CMD_STATUS_UNKNOWN);
}

void test_empty_command(void) {
	command_status_t status = run_command("", 0);
	assert(status == CMD_STATUS_EMPTY);
}

void test_bad_argument(void) {
	command_status_t status = run_command("", 250);
	assert(status == CMD_STATUS_BAD_ARGUMENT);
}

int main(void) {
	test_forward();
	test_backward();
	test_stop();
	test_bad_command();
	test_empty_command();
	test_bad_argument();
}
