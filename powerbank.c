// ------- Preamble -------- //
#include <stdio.h>
#include <avr/io.h> /* Defines pins, ports, etc */
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h> /* Functions to waste time */
#include "uart/uart.h"

#include "powerbank.h"

uint8_t sys_state = START;
uint16_t task_button_time = LONG_PRESS_TIME_MS;
uint8_t status_lamp = 0;
uint8_t status_indicator = 0;

// -------- Inits --------- //
uint8_t state_button = 0;
uint8_t state_sw_volt = 0;
uint8_t state_sw_current = 0;

uint8_t debounce_button(void) {
	if ((PINBUTTON & (1 << PN_BUTTON)) == 0) {
		_delay_us(DEBOUNCE_TIME_US);
		if ((PINBUTTON & (1 << PN_BUTTON)) == 0) {
			return (1);
		}
	}
	return (0);
}

uint8_t debounce_sw_volt(void) {
	if ((PINSW_VOLT & (1 << PN_SW_VOLT)) == 0) {
		_delay_us(DEBOUNCE_TIME_US);
		if ((PINSW_VOLT & (1 << PN_SW_VOLT)) == 0) {
			return (1);
		}
	}
	return (0);
}

uint8_t debounce_sw_current(void) {
	if ((PINSW_CURRENT & (1 << PN_SW_CURRENT)) == 0) {
		_delay_us(DEBOUNCE_TIME_US);
		if ((PINSW_CURRENT & (1 << PN_SW_CURRENT)) == 0) {
			return (1);
		}
	}
	return (0);
}

void set_lamp(void) {
	if (status_lamp == 1) {
		PRTLED_LAMP |= (1 << PN_LED_LAMP);
	}
	else
	{
		PRTLED_LAMP &= ~(1 << PN_LED_LAMP);
	}
}

void set_indicator(void) {
	if (status_indicator == 1) {
		PRTLED_INDICATOR |= (1 << PN_LED_INDICATOR);
	}
	else
	{
		PRTLED_INDICATOR &= ~(1 << PN_LED_INDICATOR);
	}
}

int main(void) {
	
	char serial_buffer[100];
	DDRLED_LAMP |= (1 << PN_LED_LAMP);
	DDRLED_INDICATOR |= (1 << PN_LED_INDICATOR);
	//
	DDRBUTTON &= ~(1 << PN_BUTTON);
	PRTBUTTON |= (1 << PN_BUTTON);
	
	DDRSW_VOLT &= ~(1 << PN_SW_VOLT);
	PRTSW_VOLT |= (1 << PN_SW_VOLT);
	
	DDRSW_CURRENT &= ~(1 << PN_SW_CURRENT);
	PRTSW_CURRENT |= (1 << PN_SW_CURRENT);
	
	// set up timer 0 for 1 mSec ticks
	TIMSK2 = (1<<OCIE2A); // turn on timer 0 cmp match ISR
	OCR2A = 249; // set the compare reg to 250 time ticks
	TCCR2A = (1<<WGM21); // turn on clear-on-match
	TCCR2B = (1<<CS22) | (0<<CS21) | (0<<CS20);	// clock prescalar to 64
	
	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );
	sei(); // serial library Fleury harus pakai interrupt
	// ------ Event loop ------ //
	while (1) {
		
		// Polling untuk push button dan switches
		if (debounce_button()) {
			if (state_button == 0) {
				state_button = 1;
			}
		}
		else {
			state_button = 0;
		}
		
		if (debounce_sw_volt()) {
			if (state_sw_volt == 0) {
				state_sw_volt = 1;
			}
		}
		else {
			state_sw_volt = 0;
		}
		
		if (debounce_sw_current()) {
			if (state_sw_current == 0) {
				state_sw_current = 1;
			}
		}
		else {
			state_sw_current = 0;
		}
		
		fsm();
		
		set_indicator();
		set_lamp();
		
		// Serial Untuk Debugging
		sprintf(serial_buffer, "%d\t%d\t%d\t%d\tstate: %d\ttime:\t%u", state_button, state_sw_volt, state_sw_current, status_indicator, sys_state, (unsigned)task_button_time);
		uart_puts(serial_buffer);
		uart_puts("\r\n");
		
		_delay_ms(20); /* wait */
	} /* End event loop */
	
	
	return (0); /* This line is never reached */
}

ISR(TIMER2_COMPA_vect) {
	// decrement debouncing task time
	if (task_button_time > 0) {
		task_button_time--;
	}
}