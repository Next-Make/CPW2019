// Attiny-26
// Miles Dai, Alex Cuellar
// Next Make

/* Pin Mappings
PORTB [7:0] -> leds [7:0]
PORTA [2:1] -> leds [9:8]
*/

#include<avr/io.h>
#include<util/delay.h>

#define NUM_SAMPLES 700

#define LOWER_BOUND 0
#define UPPER_BOUND 100

int adcRead(void);
int findMin(int data[], int size);
void lightBinary(unsigned int n);
void lightLeds(int n);
int scalePeak(int n);
void showVolume(void);
void testBinary(void);
void testLEDs(void);


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
	ADCSR = 1 << ADEN | 3 << ADPS0 | 1 << ADIE;

	int result;

	while(1) {
		result = adcRead();
		// lightBinary((int)result);
		_delay_ms(100);
		lightLeds(scalePeak(result));
		// testLEDs();

	}
	return 1;
}

void showVolume(void) {
	// ADC clk speed is 1 MHz/8 = 125kHz
	// For a 100Hz wave, this requires 625 samples to get half the wave
	// Will get 700 samples and take the smallest 

	int minSample = 0;
	for(int i = 0; i < NUM_SAMPLES; i++) {
		int sample = adcRead();
		minSample = (sample < minSample) ? sample : minSample;
	}

	lightLeds(10 - scalePeak(minSample));
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
	while((ADCSR & (1 << ADIF)) == 0); // Check ADC interrupt

	unsigned int adcHigh = ADCH;

	return (int)(adcHigh << 2); // read top byte (discard lower bytes)
}

int findMin(int data[], int size) {
	int min = data[0];
	for(int i = 0; i < size; i++) {
		min = (min > data[i]) ? data[i] : min;
	}
	return min;
}

// scale the peak value from 0 to 10 inclusive to indicated number of led bars
int scalePeak(int n) {
	int res = n * 10 / 1024;
	res = (res > 10) ? 10 : res;
	res = (res < 0) ? 0 : res;
	return res;
}

void lightLeds(int n) {

	if(n < 8) {
		PORTB = (1<<n) - 1;
		PORTA = 0x00;
	} else {
		PORTB = (1<<7) - 1;
		switch(n) {
			case 8:
				PORTA = 0b10;
				break;
			case 9:
				PORTA = 0b110;
				break;
			case 10:
				PORTA = 0b10110;
				break;
			default:
				PORTA = 0b10110;
		}
	}
}

void lightBinary(unsigned int n) {
	unsigned int lowerBits = n & 0x3FF;
	PORTB = n & 0x7F;
	PORTA = (n & (1 << 7)) >> 6 | (n & (1 << 8)) >> 6 | (n & (1 << 9)) >> 5;
}

void testBinary(void) {
	for(int i = 0; i < 1024; i++) {
		lightBinary(i);
		_delay_ms(20);
	}
}