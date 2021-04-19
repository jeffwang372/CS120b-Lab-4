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

enum SMStates {SMStart, Init, Wait, Check, Reset, Unlock, Lock} SMState;
unsigned char i = 0x00;
unsigned char code[4] = {0x04, 0x01, 0x02, 0x01};

void SMFunc() {
        unsigned char inputA = 0x00;
        unsigned char outputC = 0x00;
	unsigned char outputB = 0x00;	

	outputB = PORTB;
        outputC = PORTC;
        inputA = PINA;
        switch(SMState) {
                case SMStart:
                        SMState = Init;
                        outputC = 0x00;
                        break;
                case Init:
                        SMState = Wait;
                        outputC = 0x01;
                        break;
                case Wait:
                        if((inputA & 0xFF) == 0x00) {
                                SMState = Wait;
                        }
                        else if((inputA & 0x0F) == code[i]) {
                                SMState = Check;
                        }
                        else if((inputA & 0x80) == 0x80) {
                                SMState = Lock;
                        }
                        else if((inputA & 0x0F) != code[i]) {
                                SMState = Reset;
                        }
                        outputC = 0x02;
                        break;
                case Check:
                        if(i == 0x03) {
                                SMState = Unlock;
                        }
                        else if ((inputA & 0xFF) == 0x00) {
                                SMState = Wait;
				++i;
                        }
                        outputC = 0x03;
                        break;
                case Reset:
                        if((inputA & 0xFF) == 0x00) {
                                SMState = Wait;
                        }
                        i = 0x00;
                        outputC = 0x04;
                        break;
                case Unlock:
                        if((inputA & 0xFF) == 0x00) {
                                SMState = Wait;
                        }
                        i = 0x00;
                        outputC = 0x05;
                        break;
                case Lock:
                        if((inputA & 0x80) != 0x80) {
                                SMState = Wait;
                        }
			i = 0x00;
                        outputC = 0x06;
                        break;
                default:
                        SMState = SMStart;
                        break;

        }

        switch(SMState) {
                case SMStart:
                        break;
                case Init:
                        outputB = 0x00;
                        break;
                case Wait:
                        break;
                case Check:
                        break;
                case Reset:
                        break;
                case Unlock:
                        outputB = 0x01;
                        break;
		case Lock:
			outputB = 0x00;
			break;
                default:
                        break;
        }
        PORTC = outputC;
	PORTB = outputB;
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
