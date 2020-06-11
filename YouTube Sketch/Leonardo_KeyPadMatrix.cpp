/*
 * Simple demo sketch to use a membrane key matrix ($2 each) to send back
 * numeric keypad characters so we can use it for ALT characters (often
 * used as shortcut keys for applications or for special ASCII characters
 * like the µ character (ALT-230)
 */

#include "Leonardo_KeyPadMatrix.h"
#include <Keypad.h>
#include <Keyboard.h>

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
							{ '1', '2', '3' },
							{ '4', '5', '6' },
							{ '7', '8', '9' },
							{ '*', '0', '#' }
};
byte rowPins[ROWS] = { 10, 16, 14, 15 }; //connect to the row pinouts of the keypad
byte colPins[COLS] = { 9, 8, 7 }; //connect to the column pinouts of the keypad

// Key Pad Keys 0 - 9 (different to top row of standard keyboard)
uint8_t keyPadKey[] = { 234, 225, 226, 227, 228, 229, 230, 231, 232, 233 };

#define ledPin A3
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

#define buzzPin 4

void beep() {
	digitalWrite(buzzPin,HIGH);
	delay(100);
	digitalWrite(buzzPin,LOW);
}

void setup()
{
	// LED to show when in ALT mode
	pinMode(ledPin, OUTPUT);
	digitalWrite(ledPin, HIGH);
	delay(100);
	digitalWrite(ledPin, LOW);

	// Beeper
	pinMode(buzzPin,OUTPUT);
	beep();

	// This does nothing but is for backwards compatibility only
	Keyboard.begin();
	Serial.begin(9600);
}

void loop()
{
	// Has key been pressed?
	char key = keypad.getKey();

	// We have a keypress...
	if (key)
	{
		// Debugging with beep so we know we have pressed it
		Serial.print("Key:");
		Serial.println(key);
		beep();

		switch (key)
		{
		case '*':
			// Set ALT key mode
			digitalWrite(ledPin, HIGH);
			Keyboard.press(KEY_LEFT_ALT);
			break;
		case '#':
			// Cancel ALT key mode and send all key presses to PC
			digitalWrite(ledPin, LOW);
			Keyboard.releaseAll();
			break;
		default:
			// Convert all standard numeric keys (ASCII value) to KeyPad keys
			uint8_t keyPadValue = (uint8_t)key - 48;

			Serial.print("Key Pad Value:");
			Serial.println(keyPadValue);
			Serial.println(keyPadKey[keyPadValue]);

			// 'Press' the alternative (keypad) key
			Keyboard.press(keyPadKey[keyPadValue]);
			delay(50);

			// Release the key pressed (key up)
			Keyboard.release(keyPadKey[keyPadValue]);
		}
	}
}
