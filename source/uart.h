/* Author: Dreycey Albin
 *
 * UART.h
 *
 *  Created on: Apr 1, 2022
 */

#ifndef UART_H_
#define UART_H_
#include "circular_buffer.h"
#include <stdint.h>


/*
 * the Tx/Rx circular buffers involved.
 * using similiar pattern to Dean, with modification suggested for incapsulation.
 */
extern tCircularBuffer Tx_buffer, Rx_buffer;
#define UART_OVERSAMPLE_RATE 	(16)
#define SYS_CLOCK				(24e6)


/*
 * Function: uart_init()
 * ---------------
 *
 * Description:
 *     This method initializes the UART0 on the FRDM KL25Z
 *     - Heavily adapted from Code listing 8.8, p. 231 (Dean Alexander)
 * Input:
 *     1. N/A - void
 * Returns (non-Error):
 *     1. N/A - void
 * Returns (Error Codes):
 *     1. the read that empties the shift register/(Rx for UART0_D)
 */
void Init_UART0(uint32_t baud_rate);

/*
 * Function: UART0_Read_Nonblocking()
 * ---------------
 *
 * Description:
 *     This method was taken from PES lecture 20; Howdy Pierce
 * Input:
 *     1. N/A - void
 * Returns (non-Error):
 *     1. Returns one byte if it's available to be read;
 *        Returns -1 if no bytes available
 * Returns (Error Codes):
 *     1. N/A
 */
uint32_t UART0_Read_Nonblocking();

/*
 * Function: UART0_Send_Blocking(const char *msg)
 * ---------------
 *
 * Description:
 *     This method allows for sending a message
 *     through UART, a null-terminated string
 * Input:
 *     1. N/A - void
 * Returns (non-Error):
 *     1. a null-terminated string
 *        Function won't return until all bytes sent
 * Returns (Error Codes):
 *     1. N/A
 */
void UART0_Send_Blocking(const char *msg);

/*
 * Function: __sys_write(int handle, char *input_buffer, int size)
 * ---------------
 *
 * Description:
 *     This method allows for sending a message
 *     over UART to the host PC.
 * Input:
 *     1. handle of 0 or 1 (doesn't matter here)
 *     2. an input buffer to hold info
 *     3. the size of the message
 * Returns (non-Error):
 *     1. 0  : if works
 * Returns (Error Codes):
 *     1. -1 : if error
 */
int __sys_write(int handle, char *input_buffer, int size);

/*
 * Function: __sys_readc(void)
 * ---------------
 *
 * Description:
 *     This method allows for reading a message
 *     over UART from the host PC.
 * Input:
 *     1. void - NA
 * Returns (non-Error):
 *     1. buffer with message
 * Returns (Error Codes):
 *     1. -1 : if error
 */
int __sys_readc(void);

#endif /* UART_H_ */
