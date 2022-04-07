/*
 * command_processor.h
 *
 *  Created on: Apr 5, 2022
 *      Author: dreyceyalbin
 */

#ifndef COMMAND_PROCESSOR_H_
#define COMMAND_PROCESSOR_H_


/*
 * Function: author_method(const char *args);
 * ---------------
 *
 * Description:
 *     This helper method is used to print the authors name. Dreycey.
 * Input:
 *     1. args
 */
void author_method(const char *args);

/*
 * Function: author_method(const char *args);
 * ---------------
 *
 * Description:
 *     This helper method is used to print the hex of a specified
 *     memory locationn
 * Notes:
 * 		useful site - https://www.tutorialspoint.com/c_standard_library/c_function_sscanf.htm
 * Input:
 *     1. args
 */
void hexdump_method(const char *args);

/*
 * Function: help_method(const char *args);
 * ---------------
 *
 * Description:
 *     This helper method is used to print useful information out about
 *     all avialable commands.
 * Input:
 *     1. args
 */
void help_method(const char *args);

/*
 * Function: info_method(const char *args);
 * ---------------
 *
 * Description:
 *     This helper method is used to print software information out.
 * Input:
 *     1. args
 */
void info_method(const char *args);

/*
 * Function: preprocess()
 * ---------------
 *
 * Description:
 *     This method parses the input commands, getting rid of backspaces and enters
 * Input:
 *     1. N/A - void
 */
void preprocess();

/*
 * Function: command_processor(char *parsed_characters)
 * ---------------
 *
 * Description:
 *     This method chooses the correct helper method based on the initial argument.
 * Input:
 *     1. N/A - void
 */
void command_processor(char *parsed_characters);

#endif /* COMMAND_PROCESSOR_H_ */
