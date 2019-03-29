// Attiny-26
// Miles Dai, Alex Cuellar
// Next Make

/* Pin Mappings
PORTB [7:0] -> leds [7:0]
PORTA [2:1] -> leds [9:8]
*/

#include<avr/io.h>
#include<util/delay.h>

#define NUM_SAMPLES 100

#define LOWER_BOUND 0
#define UPPER_BOUND 100

int acdRead(void);
int findPeak(int data[], int size);
int scalePeak(int n);
void lightLeds(int n);

int main(void) {

	// Setup code
	DDRA = 0xFE; // On port A, set pin 1 to input and rest to output
	DDRB = 0b01111111;// 0xFF; // Set port B to output (drive LEDs)

	/*
	ADC MUX Register:
	REFS[1:0] = 00 (AVcc)
	ADLAR = 1 (left-justified)
	MUX[4:0] = 00000 (ADC0)
	*/
	ADMUX = 1 << ADLAR; //0x20; // 00100000

	/*
	ADC Control and Status Register:
	ADEN = 1 (enable)
	ADSC = 0 (start conversion - will run a dummy conversion to configure everything)
	ADFR = 0 (free-running mode)
	ADIF = X (interrupt flag)
	ADIE = 1 (interrupt enable)
	ADPS[2:0] = 011 (divide by 8 - assuming a 1 MHz internal clock) <- will need to check fuse bits for this
	*/
	ADCSR = 1 << ADEN | 3 << ADPS0;

	// int readings[NUM_SAMPLES];
	// int peak;
	int result;

	while(1) {

		// for(int i = 0; i < NUM_SAMPLES; i++) {
		// 	readings[i] = adcRead();
		// }
		result = adcRead();
		lightLeds(scalePeak(result));

		// peak = findPeak(readings, NUM_SAMPLES);

	}
	return 1;
}

void testLEDs(void) {
	for(int i = 0; i < 11; i++) {
		lightLeds(i);
		_delay_ms(50);
	}
	for(int i = 9; i > -1; i--) {
		lightLeds(i);
		_delay_ms(50);
	}
}

int adcRead(void) {
	ADCSR = ADCSR | (1 << ADSC); // start ADC conversion
	// while((ADCSR & (1 << ADIF)) == 0); // Check ADC interrupt
	while(ADCSR & 1<<ADSC);

	unsigned char adcHigh = ADCH;

	return (int)(adcHigh << 8); // read top byte (discard lower bytes)
}

int findPeak(int data[], int size) {
	int max = data[0];
	for(int i = 0; i < size; i++) {
		max = (max < data[i]) ? data[i] : max;
	}
	return max;
}

// scale the peak value from 0 to 10 inclusive to indicated number of led bars
int scalePeak(int n) {
	int res = n * 10 / 1024;
	res = (res > 10) ? 10 : res;
	res = (res < 0) ? 0 : res;
	return res;
}

void lightLeds(int n) {
	switch(n) {
		case 0:
			PORTB = 0;
			PORTA = 0;
			break;
		case 1:
			PORTB = 0b00000001;
			PORTA = 0X00;
			break;
		case 2:
			PORTB = 0b00000011;
			PORTA = 0X00;
			break;
		case 3:
			PORTB = 0b00000111;
			PORTA = 0x00;
			break;
		case 4:
			PORTB = 0b00001111;
			PORTA = 0x00;
			break;
		case 5:
			PORTB = 0b00011111;
			PORTA = 0x00;
			break;
		case 6:
			PORTB = 0b00111111;
			PORTA = 0x00;
			break;
		case 7:
			PORTB = 0b01111111;
			PORTA = 0x00;
			break;
		case 8:
			PORTB = 0b01111111;
			PORTA = 0b00000010;
			break;
		case 9:
			PORTB = 0b01111111;
			PORTA = 0b00000110;
			break;
		case 10:
			PORTB = 0b01111111;
			PORTA = 0b00010110;
			break;
		default:
			PORTB = 0b01111111;
			PORTA = 0b00010110;
	}
}