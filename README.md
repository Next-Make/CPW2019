# VU Meter

*Next Make Solder-Your-Own Circuit-Board Event*

*CPW 2019 / REX 2019*

## Prerequisites
You will need some tools to build the code and load it onto the ATtiny. All code for this project was developed on Windows 10 and is not guaranteed to work with other OS's. 

### Cross-Compiler
* Windows: [WinAVR](http://winavr.sourceforge.net/index.html)
* Mac: [CrossPack](http://www.obdev.at/products/crosspack/download-de.html)
* Linux: Install GCC-AVR package with whatever package manager you're using. For Debian/Ubuntu:
```bash
sudo apt-get update
sudo apt-get upgrade all
# Required packages:
sudo apt-get install gcc-avr binutils-avr avr-libc
# Optional debugger
sudo apt-get install gdb-avr
```
 
 ### Programmer
AVRdude should be bundled with the packages mentioned in the previous section. Run ```avrdude -h``` in the terminal to test it. If this throws an error, make sure that the above packages were installed correctly and (especially on Windows) that your path variables are correct.

## Wiring Notes
* The arrow on the programmer socket indicates pin 1.
* The pin directly across from pin 1 is pin 2. The other pin directly adjacent to pin 1 is pin 3. 

## Programming
1. Navigate to AVR_test The makefile is set to compile the code and also run AVRdude for convenience. To set up this functionality, ensure the correct parameters are set in ```makefile```. Specifically:
```
MCU = attiny261
AVRDUDE_PROGRAMMER = usbtiny
AVRDUDE_PORT = usb
```
2. Line up the pogo pins with the pads on the board. The square pad is pin 1. Ensure solid contact.

3. In the program directory, run ```make all``` to generate the .hex file and program the board. AVRdude should say "Thank you" if it programs successfully.

Alternatively, if you have a pre-compiled .hex file, you can run the programmer directly without rebuilding:
```
avrdude -P usb -c usbtiny -p t26 -U flash:w:main.hex
```

## Troubleshooting
* If you see ```avr-gcc: _spawnvp: No such file or directory```, you will need to include the ```msys-1.0.dll``` library included in this repository. This should be placed in PATH/TO/WINAVR/utils/bin. Replace the existing ```msys-1.0.dll``` with this new one.