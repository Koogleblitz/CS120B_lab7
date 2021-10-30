/*	Author: Richard Tobing, rlumb001@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #7  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding timer.h or example
 *	code, is my own original work.
	
	Demo Link: https://youtu.be/hfxVGOhdZ1o
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "io.h"
#endif
//----------------------------------------timer.h----------------------------------------------//
volatile unsigned char TimerFlag = 0; 
unsigned long _avr_timer_M = 1; 
unsigned long _avr_timer_cntcurr = 0; 
void TimerOn() {
  TCCR1B = 0x0B;
  OCR1A = 125;
  TIMSK1 = 0x02;
  TCNT1 = 0;
  _avr_timer_cntcurr = _avr_timer_M;
  SREG |= 0x80;
}
void TimerOff() {TCCR1B = 0x00;}
void TimerISR() {TimerFlag = 1;}
ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--; 
	if(_avr_timer_cntcurr == 0) { 
		TimerISR(); 
		_avr_timer_cntcurr = _avr_timer_M;
	}
}
void TimerSet(unsigned long M) {
	_avr_timer_M = M;                               
	_avr_timer_cntcurr = _avr_timer_M;
}
////----------------------------------------/timer.h----------------------------------------------////

















enum states{wait,cMin,cPlus,clear}state;
unsigned char A,prev;
signed long cnt = 0;
signed long i = 0;


void Tick(){
	 A = (~PINA) & 3;
	   

	    switch(state){
	    	case wait:
				prev = 0;
				if(A==3){state = clear; }
				else if(A==1){state = cPlus; }
				else if(A==2){state = cMin; }
				else{state = wait; }
				break;
		case cPlus:
				prev = 1;
				if(A==3){state = clear; }
				else if(A==1){state = cPlus; }
				else if(A==2){state = cMin; }
				else{state = wait; }
				break;
		case cMin:
				prev = 2;
				if(A==3){state = clear; }
				else if(A==1){state = cPlus; }
				else if(A==2){state = cMin; }
				else{state = wait; }
				break;
		case clear:
				prev = 4;
				if(A==0){state = wait; }
				else{state = clear; }
				break;
	    }

	    //-----------------------------------------------------------

	    switch(state){
		case wait:
			i = 0;
			break;

		case cPlus:
			if((prev!=1)&&(cnt<9)){cnt++; }
			if(prev == 1){cnt++; }
			break;

		case cMin:
			if((prev!=2)&&(cnt>0)){
				if(cnt ==0){cnt = 0; }
				else{cnt = cnt -1; }
			}
			if(prev == 2){cnt = cnt-1; }
			break;

		case clear:
			cnt = 0;
			break;
	    }


	    if(cnt > 9){cnt = 9; }
	    else if(cnt < 0){cnt = 0; }
	    PORTB = cnt; 

	   if (cnt == 0) {LCD_DisplayString(1, "0"); }
		else if (cnt == 1) {LCD_DisplayString(1, "1"); }
		else if (cnt == 2) { LCD_DisplayString(1, "2"); }
		else if (cnt == 3) { LCD_DisplayString(1, "3"); }
		else if (cnt == 4) { LCD_DisplayString(1, "4"); }
		else if (cnt == 5) { LCD_DisplayString(1, "5"); }
		else if (cnt == 6) { LCD_DisplayString(1, "6"); }
		else if (cnt == 7) { LCD_DisplayString(1, "7"); }
		else if (cnt == 8) { LCD_DisplayString(1, "8"); }
		else if (cnt == 9) { LCD_DisplayString(1, "9"); }
	    

	    
}


int main(void) {
	DDRA = 0x00; PORTA = 0xFF;   //when dealing with LCDs, ALL ports need to be used, or the output will look all funky
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	TimerSet(300);
	TimerOn();
	LCD_init();	//Dont forget to initialize the LCD

	state = wait;
		
	


    
	LCD_DisplayString(1, "1:03am");    //these messages cant be too long or the LCD will get confused
	LCD_ClearScreen();
	

    while (1) {

	
	 
	Tick();
	
	while (!TimerFlag);	
	TimerFlag = 0;

		    
    }
    //return 0;
}

/*Demo:
	1. increment to 9
	2. decrement to 0
	3. go to 4, then reset
	4. holdpress to 9
	5. holdpress to 0


	*/
