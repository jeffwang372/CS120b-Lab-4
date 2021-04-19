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

enum SMStates {SMStart, Init, Wait, Unlock1, WaitUnlock, Unlock2, Lock} SMState;

void SMFunc() {
        unsigned char inputA = 0x00;
        unsigned char outputC = 0x00;
	unsigned char outputB = 0x00;	

	outB = PORTB;
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
                        if((inputA & 0x80) == 0x80) {
                                SMState = Lock;
                        }
                        if((inputA & 0x04) == 0x04) {
                                SMState = Unlock1;
                        }
                        break;
                case Unlock1:
			if((inputA & 0xFF) == 0x00) {
                                SMState = WaitUnlock;
                        }
			else if((inputA & 0x80) == 0x80) {
                                SMState = Lock;
                        }
			else if((inputA & 0x04) != 0x04) {
                                SMState = Wait;
                        }
                        break;
                case WaitUnlock:
                        if((inputA & 0x80) == 0x80) {
                                SMState = Lock;
                        }
			else if((inputA & 0x02) == 0x02) {
                                SMState = Unlock2;
                        }
			else if((inputA & 0xFF) != 0x00) {
                                SMState = Wait;
                        }
                        break;
                case Unlock2:
                        if((inputA & 0x80) == 0x80) {
                                SMState = Lock;
                        }
			else {
				SMState = Wait;
			}
                        break;
		case Lock:
			if((inputA & 0x80) != 0x80) {
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
		case WaitInc:
			break;
		case WaitReset:
			break;
		case WaitDec:
			break;
                default:
                        break;
        }
        PORTC = outputC;
}

int main(void) {
    /* Insert DDR and PORT initializations */
        DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00;
        DDRC = 0xFF; PORTC = 0x00;
    /* Insert your solution below */
        SMState = SMStart;

    while (1) {
        SMFunc();
    }
    return 0;
}
