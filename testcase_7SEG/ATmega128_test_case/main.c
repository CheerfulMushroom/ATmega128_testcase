#include <avr/io.h>

#define F_CPU 1000000UL
#include <util/delay.h>

const unsigned char LED_CHARS[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

int main(void)
{
	DDRD = 0b01111111;
	short int char_index = 0;

	while (1)
	{
		PORTD = LED_CHARS[char_index];
		char_index = (char_index + 1) % 10;
		_delay_ms(1000);

	}

	return 0;
}
