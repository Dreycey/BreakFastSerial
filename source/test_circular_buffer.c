/*
 * circular_buffer_test.c
 *
 *  Created on: Apr 1, 2022
 *      Author: dreyceyalbin
 */
#include "test_circular_buffer.h"




// Description in header
int test_cbfifo() {

	// create buffer and initialize
	tCircularBuffer c_buff;
	initialize_cbfifo(&c_buff);

    // Testing my name
    char val_to_store[] = {'D', 'r', 'e', 'y', 'c', 'e', 'y', '\0'};
    size_t number_added = cbfifo_enqueue(&c_buff, val_to_store, sizeof(val_to_store));
    char my_name[sizeof(val_to_store)];
    cbfifo_dequeue(&c_buff, my_name, sizeof(val_to_store));
    assert(strcmp(my_name, val_to_store) == 0);

    // testing a dequeue with an empty queue
    assert(cbfifo_dequeue(&c_buff, my_name, sizeof(val_to_store)) == 0);

    // Add (max) PLUS ONE
    char single_char[] = {'A'};
    int max_size = CB_SIZE;
    for (int i=1; i<=max_size; i++) {
        number_added = cbfifo_enqueue(&c_buff, single_char, 1);
    }
    number_added = cbfifo_enqueue(&c_buff, single_char, 1);
    assert(number_added == 0);

    printf("CBFIFO: All test cases pass! 3/3 \n\n\n\r");
    return 0;
}
