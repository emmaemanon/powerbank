#include <stdio.h>

#include "powerbank.h"

extern uint8_t sys_state;
extern uint16_t task_button_time;
extern uint8_t status_lamp;
extern uint8_t status_indicator;

// -------- Inits --------- //
extern uint8_t state_button;
extern uint8_t state_sw_volt;
extern uint8_t state_sw_current;


void fsm(void) {
	// State Machine Sistem
	switch (sys_state) {
		case START:
			task_button_time = LONG_PRESS_TIME_MS;
			status_lamp = 0;
			status_indicator = 0;
			sys_state = DEFAULT;
		break;
		case DEFAULT:
			task_button_time = LONG_PRESS_TIME_MS;
			if ((state_button == 1) && (state_sw_volt == 1)) { sys_state = ENTER_TIMER_H0; }
			if ((state_sw_current == 1) && (state_sw_volt == 1)) { sys_state = PRE_ALWAYS_ON; }
			if (state_sw_volt == 0) { sys_state = START; }
		break;
		case ENTER_TIMER_H0:
			if ((task_button_time > 0) && (state_button == 0)) { sys_state = ENTER_TIMER_L0; }
			else if ((task_button_time == 0) && (state_button == 1)) { sys_state = PRE_LAMP_TOGGLE; }
			if ((status_indicator == 0) && (state_sw_current == 1)) { sys_state = PRE_ALWAYS_ON; }
			if (state_sw_volt == 0) { sys_state = START; }
		break;
		case ENTER_TIMER_L0:
			if ((task_button_time > 0) && (state_button == 1)) { sys_state = ENTER_TIMER_H1; }
			else if (task_button_time == 0) { sys_state = DEFAULT; }
			if (state_sw_current == 1) { sys_state = PRE_ALWAYS_ON; }
			if (state_sw_volt == 0) { sys_state = START; }
		break;
		case ENTER_TIMER_H1:
			if ((task_button_time > 0) && (state_button == 0) && (state_sw_current == 0)) { sys_state = ENTER_TIMER_L1; }
			else if (task_button_time == 0) { sys_state = DEFAULT; }
			if (state_sw_current == 1) { sys_state = PRE_ALWAYS_ON; }
			if (state_sw_volt == 0) { sys_state = START; }
		break;
		case ENTER_TIMER_L1:
			status_indicator = !status_indicator;
			sys_state = DEFAULT;
		break;
		case PRE_LAMP_TOGGLE:
			status_lamp = !status_lamp;
			sys_state = LAMP;
		break;
		case LAMP:
			if ((state_button == 0) && (state_sw_volt == 1)) { sys_state = DEFAULT; }
			if ((state_sw_current == 1) && (state_sw_volt == 1)) { sys_state = PRE_ALWAYS_ON; }
			if (state_sw_volt == 0) { sys_state = START; }
		break;
		case PRE_ALWAYS_ON:
			status_indicator = 1;
			sys_state = ALWAYS_ON;
		break;
		case ALWAYS_ON:
			task_button_time = LONG_PRESS_TIME_MS;
			if (state_sw_current == 0) { sys_state = POST_ALWAYS_ON; }
			if ((state_button == 1) && (state_sw_volt == 1)) { sys_state = ENTER_TIMER_H0; }
			if (state_sw_volt == 0) { sys_state = START; }
		break;
		case POST_ALWAYS_ON:
			status_indicator = 0;
			sys_state = DEFAULT;
		break;
		
		default: break;
	}
}