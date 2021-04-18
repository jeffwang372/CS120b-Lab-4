/*	Author: jwang619
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum pt1States {SMStart, Wait1, S1, Wait2, S2} pt1State;

void pt1SM() {
	unsigned char inputA = 0x00;
	unsigned char outputB = 0x00;

	outputB = PORTB;
	inputA = PINA;
	switch(pt1State) {
		case SMStart:
			pt1State = Wait1;
			break;
		case Wait1:
			if((inputA & 0x01) == 0x01) {
				pt1State = S1;
			}
			break;
		case S1:
			if((inputA & 0x01) == 0x00) {
				pt1State = Wait2;
			}
			break;
		case Wait2:
			if((inputA & 0x01) == 0x01) {
				pt1State = S2;
			}
			break;
		case S2:
			if((inputA & 0x01) == 0x00) {
				pt1State = Wait1;
			}
			break;
		default:
			pt1State = SMStart;
			break;

	}
	switch(pt1State) {
		case SMStart:
			outputB = 0x01;
			break;
		case Wait1:
			break;
		case S1:
			outputB = 0x02;
			break;
		case Wait2:
			break;
		case S2:
			outputB = 0x01;
			break;
		default:
			break;
	}
	PORTB = outputB;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
	pt1State = SMStart;
	
    while (1) {
	pt1SM();		
    }
    return 0;
}
