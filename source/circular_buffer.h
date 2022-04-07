/*
 * circular_buffer.h
 *
 *  Created on: Apr 1, 2022
 *      Author: dreyceyalbin
 */

#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

#include <stdlib.h>  // for size_t
#include <stdint.h>
#include <MKL25Z4.h>
#define CB_SIZE 256 // size of buffer

/*
 * circular buffer
 * from Ch. 6 White
 */
struct sCircularBuffer {
  uint8_t array[CB_SIZE];    // must be a power of two
  int read;    // holds current read position: 0 to (CB_SIZE-1)
  int write;   // holds current write position: 0 to (CB_SIZE-1)
  int length; // Default is 0, changed to true once full.
};

/*
 * Creating a typedef for the circular buffer
 */
typedef struct sCircularBuffer tCircularBuffer;


/*
 * Function: initialize_cbfifo(tCircularBuffer *data)
 * ---------------
 *
 * Description:
 *     This method initializes all of the attributes of the
 *     cbfifo to zero. Otherwise the length and other attributes
 *     are given random starting values.
 * Input:
 *     1. pointer to circular buffer
 * Returns (non-Error):
 *     1. N/A - void
 * Returns (Error Codes):
 *     1. NONE
 */
void initialize_cbfifo(tCircularBuffer *data);

/*
 * Enqueues data onto the FIFO, up to the limit of the available FIFO
 * capacity.
 *
 * Parameters:
 *   buf      Pointer to the data
 *   nbyte    Max number of bytes to enqueue
 *
 * Returns:
 *   The number of bytes actually enqueued, which could be 0. In case
 * of an error, returns (size_t) -1.
 */
size_t cbfifo_enqueue(tCircularBuffer *cb, void *buf, size_t nbyte);

/*
 * Attempts to remove ("dequeue") up to nbyte bytes of data from the
 * FIFO. Removed data will be copied into the buffer pointed to by buf.
 *
 * Parameters:
 *   buf      Destination for the dequeued data
 *   nbyte    Bytes of data requested
 *
 * Returns:
 *   The number of bytes actually copied, which will be between 0 and
 * nbyte.
 *
 * To further explain the behavior: If the FIFO's current length is 24
 * bytes, and the caller requests 30 bytes, cbfifo_dequeue should
 * return the 24 bytes it has, and the new FIFO length will be 0. If
 * the FIFO is empty (current length is 0 bytes), a request to dequeue
 * any number of bytes will result in a return of 0 from
 * cbfifo_dequeue.
 */
size_t cbfifo_dequeue(tCircularBuffer *cb, void *buf, size_t nbyte);


/*
 * Returns the number of bytes currently on the FIFO.
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   Number of bytes currently available to be dequeued from the FIFO
 */
size_t cbfifo_length();


/*
 * Returns the FIFO's capacity
 *
 * Parameters:
 *   none
 *
 * Returns:
 *   The capacity, in bytes, for the FIFO
 */
size_t cbfifo_capacity();


#endif /* CIRCULAR_BUFFER_H_ */
