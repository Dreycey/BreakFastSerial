/*
 * main.c - application entry point
 * 
 * Author Howdy Pierce, howdy.pierce@colorado.edu
 */
#include "sysclock.h"
#include "uart.h"
#include "stdio.h"
#include "test_circular_buffer.h"
#include "hexdump.h"
#include "command_processor.h"
#define BAUDRATE (38400)



int main(void)
{
	sysclock_init();

	// TODO: initialize the UART here
	Init_UART0(BAUDRATE);

	// testing circular buffer
	test_cbfifo();

	// First message
	printf("Welcome to BreakfastSerial \r\n");
	printf(" ? \n\r");

	// enter infinite loop
	while (1) {
		preprocess();
	}

	return 0 ;
}
