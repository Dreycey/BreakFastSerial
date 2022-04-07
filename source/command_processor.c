/*
 * command_processor.c
 *
 *  Created on: Apr 5, 2022
 *      Author: dreyceyalbin
 */
#include "command_processor.h"
#include "hexdump.h"
#include "uart.h"
#include <stdio.h>
#define KEYBOARD_ENTER (0X0D)


/*
 * Command methods.
 */
typedef void (*command_method)(const char *args);

void author_method(const char *args) {
	printf("Author: Dreycey Albin");
}

/*
 * https://www.tutorialspoint.com/c_standard_library/c_function_sscanf.htm
 */
void hexdump_method(const char *args) {
	uint32_t start_location;
	size_t hexdump_size;
	sscanf(args,"dump %j %k",&start_location, &hexdump_size);
	hexdump(&start_location, hexdump_size);
}

/*
 * Structures for holding command methods.
 */
typedef struct {
	const char *command_arg;
	const char *command_info;
    command_method method;
} arg_command;

static arg_command arg_command_arr[] = {
		{"author", "Returns the amazing authors name.", author_method},
		{"dump", "Returns the hexdump for a start and end location.", hexdump_method},
};


/*
 * Processing UART.
 */

// Defined in header
void preprocess() {
	char temp_char = 0;
	char character_array[300];
	int ca_index = 0;

	while (1) {
		while((cbfifo_dequeue(&Rx_buffer, &temp_char, 1) != 1));
		printf("%c", temp_char);

		if ((temp_char == '\n' ) || (temp_char == '\r')) {
			break;
		}
		if (temp_char == KEYBOARD_ENTER) {
			character_array[ca_index] = '\0';
			break;
		}
		else if (temp_char == '\b') { //remove backspace.
			ca_index--;
			printf(" \b");
		}
		else {
			character_array[ca_index] = temp_char;
			ca_index++;
		}
	}
	character_array[ca_index] = '\0';
	printf(character_array);
	printf("\r\n");

	/*
	 * Further process the commands.
	 */
	command_processor(character_array); // Call command_processor
}





// Defined in header
void command_processor(char *parsed_characters) {
	int method_count = 2;
	int not_recognized = 1;
	char *argument[10];
	char *buffer[10];

	sscanf(parsed_characters, "%s %[^\t\n]", &argument, buffer);

	/*
	* Searching for matching method
	*/
	for (int j = 0; j < method_count; j++){
		if (strcasecmp(argument, arg_command_arr[j].command_arg) == 0)  {
			arg_command_arr[j].method(parsed_characters);
		    not_recognized = 0;
		    break;
		}
	}

	 /*
	  * If no similiar commands
	  */
	 if(not_recognized){
		 printf("Unknown Command\r\n");
		 printf ("?");
	 }

}











