/*
 * hexdump.h
 *
 *  Created on: Apr 3, 2022
 *      Author: dreyceyalbin
 */

#ifndef HEXDUMP_H_
#define HEXDUMP_H_

#include <stdint.h>

/*
* int_to_hexchar: helper function for num to hex
* Input:
*   given start/end, it returns the hex value for that position.
*
* Parameters:
*   1. char string : char *
*   2. start location : uint8_t
*   3. stop location : uint8_t
*
* Returns:
*   The hex character of a given binary segment.
*/
char int_to_hexchar(uint8_t input_num);

/*
* hexdump: Creates a hexdump for an memory start and end location
*        - This one was taken from lecture.
*
* Parameters:
*   -> 1. starting point in memory : const char *
*   -> 2. number of bytes to traverse : size_t
*
* Returns:
*   Returns the str with the modifications
*/
void *hexdump(const void *loc, size_t nbytes);

#endif /* HEXDUMP_H_ */
