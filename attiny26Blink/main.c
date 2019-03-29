#include <avr/io.h>
#include <util/delay.h>
 
 
int main (void)
{
    // set PB3 to be output
	DDRA |= (1 << 0);
    while (1) {

        // set PB3 high
        PORTA ^= (1<<0);
        _delay_ms(800);
        // set PB3 low
        PORTA ^= (1<<0);
        _delay_ms(200);
    }
    
    return 1;
}