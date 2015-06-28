#ifndef ARDUINOEMU_ARDUINO_H
#define ARDUINOEMU_ARDUINO_H

const bool HIGH = true;
const bool LOW = false;

void digitalWrite(int pin, bool value);


const int OUTPUT = 1;

void pinMode(int pin, int mode);

#endif
