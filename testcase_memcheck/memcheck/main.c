#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/boot.h>
#include <avr/interrupt.h>


#define PAGESIZE SPM_PAGESIZE
#define WORDSIZE 2
#define APP_FLASH_END 0x1FC00

static void write_word_to_page(const uint32_t page_addr, const uint16_t word);
static int page_is_filled_with_word(const uint32_t page_addr, const uint16_t word);


int main(void)
{
	DDRD = 0b00000111;
	PORTD = 0x01;  // indicates that memcheck started
	
	uint32_t page_addr = 0;
	int flash_is_ok = 0;
	
	while(page_addr < APP_FLASH_END){
		write_word_to_page(page_addr, 0x5555);
		flash_is_ok = page_is_filled_with_word(page_addr, 0x5555);
		if (!flash_is_ok){
			break;
		}
		
		write_word_to_page(page_addr, 0xAAAA);
		flash_is_ok = page_is_filled_with_word(page_addr, 0xAAAA);
		if (!flash_is_ok){
			break;
		}
		
		page_addr += PAGESIZE;
	}
	
	PORTD = (flash_is_ok)? 0b00000010: 0b00000100; // indicates failure or success
	while(1);
}


static void write_word_to_page(const uint32_t page_addr, const uint16_t word){
	uint8_t sreg = SREG;
	cli();
	eeprom_busy_wait ();
	
	boot_page_erase(page_addr);
	boot_spm_busy_wait();
	
	for (uint32_t word_addr = 0; word_addr < PAGESIZE ; word_addr += WORDSIZE){
		boot_page_fill(word_addr, word);
	}
	
	boot_page_write(page_addr); 
	boot_spm_busy_wait();
	
	boot_rww_enable ();
	sei();
	SREG = sreg;
}


static int page_is_filled_with_word(const uint32_t page_addr, const uint16_t word){
	for (uint32_t word_addr = page_addr; word_addr < page_addr + PAGESIZE ; word_addr += WORDSIZE){
		uint16_t res_word = pgm_read_word_far(word_addr);
		if(res_word != word){
			return 0;
		}
	}
	return 1;
}

