/*
 *  VRiD_VRsetup_v0021_2.h  --> runs with Arduino v0021
 *  Library for various functions used
 *	to set up the Arduino-mediated comunication.
 *
 *  Created by Hannah Haberkern on 4/5/12.
 *  Copyright 2012 ETH Zürich. All rights reserved.
 *
 */

#ifndef VRiD_VRsetup_h
#define VRiD_VRsetup_h
#include <SPI.h>// include the SPI library code
//#include <Arduino.h>

class VRiD_VRsetup
	{
	public:
		
		VRiD_VRsetup(int lowestPin, int highestPin, int dac2pin);
		
		void initializePins(int LEDpin);
		
		//initialize SPI; set dac variables & control register, set all dac outputs to 0
		void initializeSPI();
		
		//communication with LCD display (clear, write something)
		void LCDcommunication(String message);
		
		void dac2Write(uint8_t dac, int16_t value);
		
	private:
		int _lowestPin;
		int _highestPin;
		int _dac2pin;
	};
#endif