#ifndef POWERBANK_H
#define	POWERBANK_H

// ------- Preamble -------- //

#define DDRLED_LAMP	DDRD
#define PRTLED_LAMP	PORTD
#define PN_LED_LAMP	PD2

#define DDRLED_INDICATOR	DDRD
#define PRTLED_INDICATOR	PORTD
#define PN_LED_INDICATOR	PD3

#define DDRSW_VOLT	DDRD
#define PRTSW_VOLT	PORTD
#define PINSW_VOLT	PIND
#define PN_SW_VOLT	PD4

#define DDRSW_CURRENT	DDRD
#define PRTSW_CURRENT	PORTD
#define PINSW_CURRENT	PIND
#define PN_SW_CURRENT	PD5

#define DDRBUTTON	DDRD
#define PRTBUTTON	PORTD
#define PINBUTTON	PIND
#define PN_BUTTON	PD6

#define UART_BAUD_RATE 9600

#define DEBOUNCE_TIME_US 1000 // debounce time in microseconds

#define LONG_PRESS_TIME_MS 1000 // long press time in milliseconds

// system states
#define START				0
#define DEFAULT				1
#define ENTER_TIMER_H0		2
#define ENTER_TIMER_L0		3
#define ENTER_TIMER_H1		4
#define ENTER_TIMER_L1		5
#define PRE_LAMP_TOGGLE		6
#define LAMP				7
#define PRE_ALWAYS_ON		8
#define ALWAYS_ON			9
#define POST_ALWAYS_ON		10

void fsm(void);

// END
#endif