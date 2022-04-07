/*
 * circular_buffer.c
 *
 *  Created on: Apr 1, 2022
 *      Author: dreyceyalbin
 */
#include "circular_buffer.h"
#include <stdio.h>
#include <stdlib.h>


//Defined in header
void initialize_cbfifo(tCircularBuffer *data) {
    data->write = 0;
    data->read = 0;
    data->length = 0;
}

//Defined in header
size_t cbfifo_enqueue(tCircularBuffer *cb, void *buf, size_t nbyte) {
    if (nbyte < 0) {return (CB_SIZE-1);}
    if (!buf) {return (CB_SIZE-1);}

    uint32_t masking_state = __get_PRIMASK();
    __disable_irq();
    int num_bytes_added = 0;
    uint8_t * ptr_to_buffer = buf;
    for (int i=0; i < nbyte; i++){
        if (cb->length >= CB_SIZE) {break;}
        cb->array[cb->write] = (*(uint8_t *)ptr_to_buffer) & (0xFF);
        cb->length++;
        num_bytes_added++;
        ptr_to_buffer++;

        // update the write pointer - resets to zero once == CB_SIZE
        cb->write = (cb->write + 1) & (CB_SIZE - 1);
    }
    __set_PRIMASK(masking_state);
    return num_bytes_added;
}

//Defined in header
size_t cbfifo_dequeue(tCircularBuffer *cb, void *buf, size_t nbyte) {

    uint8_t * ptr_cpy = (uint8_t *)buf;
    int num_bytes_removed = 0;
    if (cb->length <= 0) {return num_bytes_removed;}

    uint32_t masking_state = __get_PRIMASK();
    __disable_irq();
    for (int i=0; i<nbyte; i++){
        if (cb->length <= 0) {break;} // if empty, return
        ptr_cpy[i] = cb->array[cb->read];
        cb->length--;
        num_bytes_removed++;

        // update the write pointer - resets to zero once == CB_SIZE
        cb->read= (cb->read + 1) & (CB_SIZE - 1);
    }
    __set_PRIMASK(masking_state);
    return num_bytes_removed;
}

//Defined in header
size_t cbfifo_capacity() {
    return CB_SIZE;
}

//Defined in header
size_t cbfifo_length(tCircularBuffer *cb) {
    return cb->length;
}
