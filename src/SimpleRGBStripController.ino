/*****************************************************************************
	Simple RGB Strip Controller - Arduino based RS-232 control of RGB LEDs
	Copyright (C) 2019  David Wyban

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*****************************************************************************/

#define PIN_R	5
#define PIN_G	3
#define PIN_B	6

#include <SerialCommand.h> // https://github.com/kroimon/Arduino-SerialCommand

// Types
typedef union ulongToARGB_t {
	unsigned long int UlongNum;
	struct ARGB_t {
		uint8_t B;
		uint8_t G;
		uint8_t R;
		uint8_t A;
	} ARGB;
} ulongToARGB_t;


// Globals
SerialCommand sc;
float fFadeTime;
ulongToARGB_t curColor;

void setup()
{
	// Configure PWM outputs for RGB
	pinMode(PIN_R, OUTPUT);
	pinMode(PIN_G, OUTPUT);
	pinMode(PIN_B, OUTPUT);

	// Intial output is off
	digitalWrite(PIN_R, LOW);
	digitalWrite(PIN_G, LOW);
	digitalWrite(PIN_B, LOW);
	
	analogWrite(PIN_R, 0);
	analogWrite(PIN_G, 0);
	analogWrite(PIN_B, 0);

	// Configure the PWM on Pin 3 to run closer to 1 KHz
	//--------------------------------------------------
	// Timer2 controls the PWM frequency on pin 3
	// Set to Fast PWM Mode. (Arduino Core sets to Phase-Correct PWM)
	TCCR2A |= _BV(WGM21) | _BV(WGM20);

	// Prescaler to /64
	TCCR2B = _BV(CS22);

	
	Serial.begin(115200);

	// Setup commands
	sc.addCommand("C", cmdColor);
	sc.addCommand("CF", cmdColorFade);
	sc.addCommand("FT", cmdFadeTime);
	sc.addDefaultHandler(cmdUnknown);
	
	fFadeTime = 1.0F;
	curColor.UlongNum = 0;
	
	Serial.println("Ready");
}

void loop()
{
	// Run cmd parser
	sc.readSerial();
}

// Command functions
//------------------
void cmdColor()
{
	char *arg;
	arg = sc.next();
	if (arg != NULL)
	{
		// Test that parameter is string of hex digits
		if (IsHexString(arg))
		{
			ulongToARGB_t temp;
			temp.UlongNum = strtoul(arg, NULL, 16);

			analogWrite(PIN_R, temp.ARGB.R);
			analogWrite(PIN_G, temp.ARGB.G);
			analogWrite(PIN_B, temp.ARGB.B);

			errorNoError();
		}
		else
		{
			errorParamInvalid();
		}
	}
	else
	{
		errorParamInvalid();
	}
}

void cmdColorFade()
{
	char *arg;
	arg = sc.next();
	if (arg != NULL)
	{
		// Test that parameter is string of hex digits
		if (IsHexString(arg))
		{
			ulongToARGB_t newColor, fadeColor;
			newColor.UlongNum = strtoul(arg, NULL, 16);

			uint16_t fadeSteps = (uint16_t)(fFadeTime / 0.016);
			float fadePos = 0;
			float fadeStepSize = 1.0F / (float)(fadeSteps - 1);

			for (uint16_t i=0; i < fadeSteps; i++)
			{
				fadePos = fadeStepSize * (float)i;
				fadeColor = CalcFadeColor(curColor, newColor, fadePos);
				analogWrite(PIN_R, fadeColor.ARGB.R);
				analogWrite(PIN_G, fadeColor.ARGB.G);
				analogWrite(PIN_B, fadeColor.ARGB.B);
			}
			curColor = newColor;
			errorNoError();
		}
		else
		{
			errorParamInvalid();
		}
	}
	else
	{
		errorParamInvalid();
	}
}

void cmdFadeTime()
{
	char *arg;
	arg = sc.next();
	if (arg != NULL)
	{
		// Convert to float and save
		float val = atof(arg);
		if (val != 0.0F)
		{
			fFadeTime = val;
		}
		else
		{
			errorParamInvalid();
		}
	}
	else
	{
		errorParamInvalid();
	}
	errorNoError();
}

void cmdUnknown()
{
	Serial.println("Error,Command unrecognized");
}

// Standardized Error Output functions
//------------------------------------

void errorNoError()
{
	Serial.println("OK,Success");
}
void errorParamInvalid()
{
	Serial.println("Error,Parameter invalid");
}
void errorParamMissing()
{
	Serial.write("Error,Parameter missing");
}



// Utility functions
//------------------
bool IsHexString(char *s)
{
	uint8_t len = strlen(s);
	for (uint8_t i = 0; i < len; i++)
	{
		if (!isHexadecimalDigit(s[i]))
			return false;
	}
	return true;
}

ulongToARGB_t CalcFadeColor(ulongToARGB_t startColor, ulongToARGB_t endColor, float fadePosition)
{
	if (fadePosition <= 0)
		return startColor;

	if (fadePosition >= 1)
		return endColor;
	
	ulongToARGB_t fadeColor;
	fadeColor.ARGB.R = (uint8_t)((1.0F - fadePosition) * (float)startColor.ARGB.R + fadePosition * (float)endColor.ARGB.R);
	fadeColor.ARGB.G = (uint8_t)((1.0F - fadePosition) * (float)startColor.ARGB.G + fadePosition * (float)endColor.ARGB.G);
	fadeColor.ARGB.B = (uint8_t)((1.0F - fadePosition) * (float)startColor.ARGB.B + fadePosition * (float)endColor.ARGB.B);
	return fadeColor;
}
