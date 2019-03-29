// Attiny-26
// Miles Dai
// Next Make

#include<avr/io.h>

#define NUM_SAMPLES 100

#define LOWER_BOUND 0
#define UPPER_BOUND 100

int acdRead();
int findPeak();

int main() {

	// Setup code
	DDRA = 0x01; // On port A, set pin 1 to input and rest to output
	DDRB = 0x00; // Set port B to output (drive LEDs)

	/*
	ADC MUX Register:
	REFS[1:0] = 00 (AVcc)
	ADLAR = 1 (left-justified)
	MUX[4:0] = 00000 (ADC0)
	*/
	ADMUX = 0x20 // 00100000

	/*
	ADC Control and Status Register:
	ADEN = 1 (enable)
	ADSC = 1 (start conversion - will run a dummy conversion to configure everything)
	ADFR = 1 (free-running mode)
	ADIF = X (interrupt flag)
	ADIE = 1 (interrupt enable)
	ADPS[2:0] = 011 (divide by 8 - assuming a 1 MHz internal clock) <- will need to check fuse bits for this
	*/
	ADCSR = ; 0xEB // 11101011

	int readings[NUM_SAMPLES];
	int peak;

	while(1) {
		for(int i = 0; i < NUM_SAMPLES; i++) {
			readings[i] = acdRead();
		}
		peak = findPeak(readings, NUM_SAMPLES);

		lightLeds(scalePeak(peak));
	}
	return 0;
}

int acdRead() {
	while((ADCSRA & (1 << ADIF)) == 0); // Check ADC interrupt
	return (int)ADCH * 256; // read top byte (discard lower bytes)
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
	return (n - LOWER_BOUND) / (UPPER_BOUND - LOWER_BOUND);
}

void lightLeds(int n) {
	if(n < 9) {

	} else {

	}
}