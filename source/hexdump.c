/*
 * hexdump.c
 * ------------
 *
 * Description:
 *     This file includes the implementation for the
 *     the 'hexdump' methods and API.
 */
#include <stdio.h>
#include "hexdump.h"
#define STRIDE 16 // Howdy's def for size of stride.




// Defined in header
char int_to_hexchar(uint8_t input_num) {
    if (input_num == 10) {return 'A';}
    if (input_num == 11) {return 'B';}
    if (input_num == 12) {return 'C';}
    if (input_num == 13) {return 'D';}
    if (input_num == 14) {return 'E';}
    if (input_num == 15) {return 'F';}
    return input_num+'0';
}

// Defined in header file - author: Howdy (taken from class)
void *hexdump(const void *loc, size_t nbytes){


	const uint8_t *start_loc = (const uint8_t*) loc;

	for (int i=0; i<nbytes; i += STRIDE) {
		start_loc = start_loc + i;
		// print out address at the start.
		putchar(int_to_hexchar(((uint32_t)(start_loc) & 0xF0000000) >> 28));
		putchar(int_to_hexchar(((uint32_t)(start_loc) & 0x0F000000) >> 24));
		putchar(int_to_hexchar(((uint32_t)(start_loc) & 0x00F00000) >> 20));
		putchar(int_to_hexchar(((uint32_t)(start_loc) & 0x000F0000) >> 16));
		putchar('_');
		putchar(int_to_hexchar(((uint32_t)(start_loc) & 0x0000F000) >> 12));
		putchar(int_to_hexchar(((uint32_t)(start_loc) & 0x00000F00) >> 8));
		putchar(int_to_hexchar(((uint32_t)(start_loc) & 0x000000F0) >> 4));
		putchar(int_to_hexchar(((uint32_t)(start_loc) & 0x0000000F) >> 0));
		putchar(' ');
		putchar(' ');
		putchar(' ');

		/*
		 * prints out STRIDE-many bytes
		 */
		for (int j=0; j < STRIDE; j++) {
		    if ((i + j) > nbytes) {
		    	break;
		    }
			putchar(int_to_hexchar((start_loc[j] & 0xF0) >> 4));
			putchar(int_to_hexchar((start_loc[j] & 0x0F) >> 0));
			putchar(' ');
		}
		putchar('\r');
		putchar('\n');
	}
}
