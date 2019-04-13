/*
* testcase_button_counter.c
*
* Created: 13.04.2019 23:26:08
* Author : Alexandr
*/

#include <avr/io.h>

#define BUTTON_PIN PD0

const unsigned char LED_CHARS[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

void show_num(int count);

int main(void)
{
	//input
	DDRA = 0x00;
	PORTA = 0xFF;
	
	//output
	DDRD = 0b11111111;
	DDRE = 0b01111111;
	PORTD = 0b00000000;
	PORTE = 0b00000000;
	
	int button_was_pressed = 0;
	int count = 0;
	show_num(count);
	
	while (1)
	{
		if (!(PINA & (1 << BUTTON_PIN)) && (!button_was_pressed)){
			button_was_pressed = 1;
		}
		
		if ((PINA & (1 << BUTTON_PIN)) && button_was_pressed){
			button_was_pressed = 0;
			count = (count +1) % 100;
			show_num(count);
		}
	}
}

void show_num(int count){
	PORTD = LED_CHARS[count / 10];
	PORTE = LED_CHARS[count % 10];
}
