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

enum SMStates {SMStart, Init, Wait, Inc, Reset, Dec, Wait2} SMState;

void SMFunc() {
        unsigned char inputA = 0x00;
        unsigned char outputC = 0x00;

        outputC = PORTC;
        inputA = PINA;
        switch(SMState) {
                case SMStart:
                        SMState = Init;
                        break;
                case Init:
                        SMState = Wait;
                        break;
                case Wait:
                        if((inputA & 0x03) == 0x01) {
                                SMState = Inc;
                        }
                        if((inputA & 0x03) == 0x03) {
                                SMState = Reset;
                        }
                        if((inputA & 0x03) == 0x02) {
                                SMState = Dec;
                        }
                        break;
                case Inc:
                        SMState = Wait2;
                        break;
                case Reset:
                        if((inputA & 0x03) == 0x00) {
                                SMState = Wait;
                        }
                        break;
                case Dec:
                        SMState = Wait2;
                        break;
		case Wait2:
			if((inputA & 0x03) == 0x03) {
                                SMState = Reset;
                        }
                        if((inputA & 0x03) == 0x00) {
                                SMState = Wait;
                        }
			break;
                default:
                        SMState = SMStart;
                        break;

        }
        switch(SMState) {
                case SMStart:
                        break;
                case Init:
                        outputC = 0x07;
                        break;
                case Wait:
                        break;
                case Inc:
			if(outputC < 0x09) {
                                outputC = outputC + 1;
                        }
                        break;
                case Reset:
                        outputC = 0x00;
                        break;
                case Dec:
                        if(outputC > 0x00) {
                                outputC = outputC - 1;
                        }
                        break;
		case Wait2:
			break;
                default:
                        break;
        }
        PORTC = outputC;
}

int main(void) {
    /* Insert DDR and PORT initializations */
        DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
        DDRC = 0xFF; PORTC = 0x00;
    /* Insert your solution below */
        SMState = SMStart;

    while (1) {
        SMFunc();
    }
    return 0;
}
