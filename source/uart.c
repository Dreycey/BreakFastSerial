/* Author: Dreycey Albin
 *
 * UART.h
 *
 *  Created on: Apr 1, 2022
 */
#include <MKL25Z4.h>
#include "uart.h"
#include "stdio.h"
#define STOPBIT (0) // 2 stop bits
#define PARITY (0) // no parity
#define DATA_LENGTH (0) // 0 == 8 data bits


volatile int g_INTERUPT_COUNTER = 0;
volatile int g_INTERUPT_COUNTER_1 = 0;
volatile int g_INTERUPT_COUNTER_2 = 0;
volatile int g_INTERUPT_COUNTER_3 = 0;
tCircularBuffer Tx_buffer, Rx_buffer;


void Init_UART0(uint32_t baud_rate) {
	uint16_t sbr;
	uint8_t temp;

	// Enable clock gating for UART0 and Port A
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;


	// Make sure transmitter and receiver are disabled before init
	UART0->C2 &= ~UART0_C2_TE_MASK & ~UART0_C2_RE_MASK;

	// Set UART clock to 48 MHz clock
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);//mcg or mcg/2 clock
	//SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;//mcg/2 clock

	// Set pins to UART0 Rx and Tx
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Rx
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // Tx

	// Set baud rate and oversampling ratio
	sbr = (uint16_t)((SYS_CLOCK)/(baud_rate * UART_OVERSAMPLE_RATE));
	UART0->BDH &= ~UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART0_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(UART_OVERSAMPLE_RATE-1);

	// Disable interrupts for RX active edge and LIN break detect, select two stop bit
	UART0->BDH |= UART0_BDH_RXEDGIE(0) | UART0_BDH_SBNS(STOPBIT) | UART0_BDH_LBKDIE(0);

	// Don't enable loopback mode, use 8 data bit mode, don't use parity
	UART0->C1 = UART0_C1_LOOPS(0) | UART0_C1_M(DATA_LENGTH) | UART0_C1_PE(PARITY);
	// Don't invert transmit data, don't enable interrupts for errors
	UART0->C3 = UART0_C3_TXINV(0) | UART0_C3_ORIE(0)| UART0_C3_NEIE(0) | UART0_C3_FEIE(0) | UART0_C3_PEIE(0);

	// Clear error flags
	UART0->S1 = UART0_S1_OR(1) | UART0_S1_NF(1) | UART0_S1_FE(1) | UART0_S1_PF(1);

	// Send LSB first, do not invert received data
	UART0->S2 = UART0_S2_MSBF(0) | UART0_S2_RXINV(0);

	// Enable interrupts. Listing 8.11 on p. 234
	initialize_cbfifo(&Tx_buffer);
	initialize_cbfifo(&Rx_buffer);

	NVIC_SetPriority(UART0_IRQn, 2); // 0, 1, 2, or 3
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);

	// Enable receive interrupts but not transmit interrupts yet
	UART0->C2 |= UART_C2_RIE(1);
	UART0->C2 |= UART_C2_TIE(1);//enable for test-dont do this here

	// Enable UART receiver and transmitter
	UART0->C2 |= UART0_C2_RE(1) | UART0_C2_TE(1);

	// Clear the UART RDRF flag
	temp = UART0->D;
	if (temp) {}
	UART0->S1 &= ~UART0_S1_RDRF_MASK;

}


//Defined in header
void UART0_IRQHandler(void) {
	uint8_t Rx_character;
	uint8_t Tx_character;

	if (UART0->S1 & (UART_S1_OR_MASK |UART_S1_NF_MASK | UART_S1_FE_MASK | UART_S1_PF_MASK)) {
			// clear the error flags
			UART0->S1 |= UART0_S1_OR_MASK | UART0_S1_NF_MASK | UART0_S1_FE_MASK | UART0_S1_PF_MASK;
			// read the data register to clear RDRF
			Rx_character = UART0->D;
	}
	if (UART0->S1 & UART0_S1_RDRF_MASK) {
		Rx_character = UART0->D;
		if (cbfifo_length(&Rx_buffer) <= cbfifo_capacity(&Rx_buffer)){
			cbfifo_enqueue(&Rx_buffer,&Rx_character,1);
		}

	}
	if ( (UART0->C2 & UART0_C2_TIE_MASK) && (UART0->S1 & UART0_S1_TDRE_MASK) ) {
		g_INTERUPT_COUNTER++;
		if(cbfifo_dequeue(&Tx_buffer, &Tx_character, 1) == 1){
			g_INTERUPT_COUNTER_1++;
			 UART0->D = Tx_character;
		} else {
			g_INTERUPT_COUNTER_2++;
			UART0->C2 &= ~UART0_C2_TIE_MASK;
		}
	}
}

// Defined in header
int __sys_write(int handle, char *buf, int size){

	/*
	 * Wait till Tx_buffer gets room
	 */
	while(cbfifo_length(&Tx_buffer) == cbfifo_capacity(&Tx_buffer));

	/*
	 * move string into Tx
	 */
	while (1) {
		if (*buf == '\0')
			break;
		cbfifo_enqueue(&Tx_buffer,buf,1);
		buf++;
	}

	/*
	 * Trigger interupt.
	 */
	if (!(UART0->C2 & UART0_C2_TIE_MASK)) {
		g_INTERUPT_COUNTER_3++;
		UART0->C2 |= UART0_C2_TIE(1);
	}

	return 0;
}

// Defined in header
int __sys_readc(void){

	int rx_char;

	while(cbfifo_length(&Rx_buffer) == 0); // wait if empty

	if(cbfifo_dequeue(&Rx_buffer, &rx_char, 1) == 1){
		return rx_char;
	} else {
		return -1;
	}
}

