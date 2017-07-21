/*
 *  VRiD_VRcontrol_v0021_2.h
 *  
 *
 *  Created by Hannah Haberkern on 4/5/12.
 *  Copyright 2012 ETH Zürich. All rights reserved.
 *  Updated 19/11/2015 by Hannah
 *
 */

#ifndef VRiD_VRcontrol_cpp
#define VRiD_VRcontrol_cpp
#include <SPI.h>// include the SPI library code
#include <VRiD_VRcontrol_v0021_2.h>
#include <VRiD_VRsetup_v0021_2.h>
#endif

//===============
//Constructor
VRiD_VRcontrol::VRiD_VRcontrol(long timeOutInterval, int dacScreenLED, int dacBasler, int dacIRLaser, int calibrationPeriod_Hz, int calibrationPulseTime, int dac2pin)
{
	_timeOutInterval = timeOutInterval; //interval w/o input after which outputs are truned off
	_timeOutTimer = 0;
	_timeOutStarted = 0;
	
	_dacScreenLED = dacScreenLED;
	_dacBasler = dacBasler;
	_dacIRLaser = dacIRLaser;
	
	_calibrationPeriod_Hz = calibrationPeriod_Hz;
	_calibrationPulseTime = calibrationPulseTime;

	_dac2pin = dac2pin;
}


//===============
//Public functions
/*
Serial initializePCcommunication(long baudrate)
{
	Serial.begin(baudrate);
	Serial.println("FlyOver for VRiD");
	Serial.println();  
	
	//return serial
	return Serial;
}
*/

void VRiD_VRcontrol::initializeDacVariables(int dacScreenLED, int dacBasler, int dacIRLaser)
{
	_dacScreenLED = dacScreenLED;
	_dacBasler = dacBasler;
	_dacIRLaser = dacIRLaser;
}


//---------------

void VRiD_VRcontrol::performCalibration()
{
	VRiD_VRcontrol::TTLpulseWhile();
}

//---------------

void VRiD_VRcontrol::beginRecordingTrialVideo(int trialVideoFramerate_Hz, int videoRecoding_ms)
{
	int stateCameraTrigger = VRiD_VRcontrol::startCameraTrigger(trialVideoFramerate_Hz, videoRecoding_ms);
}

//---------------

int VRiD_VRcontrol::screensOn()
{
	dac2Write(_dacScreenLED, 32767);// set the DAC output to 5V
	//lock3 = STATE_CLOSED;//close the lock
	return 1; // state SCREENS_ON
}



int VRiD_VRcontrol::screensOff()
{
	dac2Write(_dacScreenLED, 0);// set the DAC output to 0V
	return 0; // state SCREENS_OFF
}

//---------------

int VRiD_VRcontrol::baslerOn()
{
	dac2Write(_dacBasler, 32767);// set the DAC output to 5V
	return 1; // state BASLER_ON
}

int VRiD_VRcontrol::baslerOff()
{
	dac2Write(_dacBasler, 0);// set the DAC output to 0V
	return 0; // state BASLER_OFF
}

//---------------

int VRiD_VRcontrol::laserOn(int dutyCycle)
{
	// range of laser duty cycle is from 0 - 255
	//int PMWval = (int) (dutyCycle*255)/100.0; //The variable dutyCycle defines the duty cycle in %
	int PMWval = (int) dutyCycle;
	analogWrite(_dacIRLaser, PMWval);//Set the PWM to high, pretty much 5 volts
	
	return 1; //state LASER=ON
}

int VRiD_VRcontrol::laserOff()
{
	analogWrite(_dacIRLaser, 0);
	return 0; //state LASER_OFF	
}

//---------------

//int VRiD_VRcontrol::punishZone (uint8_t zoneSpec, int zoneDutyCycle)
//{
//	int reward_state = initiateReward(_dacIRLaser, zoneDutyCycle);//Call reward function (previous value: 254)
//	
//	return reward_state; //should return 2
//}

//---------------

//int VRiD_VRcontrol::checkTimeOut ()
//{
//	/* Checks if _timeOutInterval time has elapsed without any input...
//	 Measured using a time that is started in any first loop in which no new input arrives.
//	 */
//	
//	if(_timeOutStarted && (millis()-_timeOutTimer)>_timeOutInterval){
//		dac2Write(_dacScreenLED, 0);// set the DAC output to 0V
//		dac2Write(_dacBasler, 0);// set the DAC output to 0V
//		_timeOutStarted = 0;
//		_timeOutInterval = 0;
//		return 0; //reset timeout state
//	}
//	return 1; //timeout still going on
//	
//}
//
//int VRiD_VRcontrol::startTimeOutTimer()
//{
//	_timeOutStarted = 1;
//	_timeOutTimer = millis();
//	return 1; //start timeout
//}
//
//int VRiD_VRcontrol::resetTimeOutTimer()
//{
//	_timeOutStarted = 0; 
//	_timeOutInterval = 0;
//	return 0; //reset timeout
//}


void VRiD_VRcontrol::dac2Write(uint8_t dac, int16_t value)
{
	// AD5754 DAC write routine
	// dac = dac output channel, 0 - 3
	// value = 16 bit output value
	digitalWrite(_dac2pin, LOW);
	SPI.transfer(dac);
	SPI.transfer((value & 0xff00)>>8);
	SPI.transfer((value & 0x00ff));
	digitalWrite(_dac2pin, HIGH);
	digitalRead(_dac2pin);
}

//==============
//private

int VRiD_VRcontrol::initiateReward(int pin, int dutyValue)
{
	/*This function is called when a reward is initiated
	 pin  -  connection to IR laser
	 value  -  duty cycle (minimum 0 -> 0% duty cycle, maximum 255 -> 100% duty cycle)
	 */
	
    analogWrite(pin, dutyValue); 
	
	return 2;
}

//---------------
void VRiD_VRcontrol::TTLpulseWhile()
{
	//This function sends a TTL when 'c' is sent to the Arduino
	//Can be depreciated if I can find the other PWM on the board

	long startTimeTTL = millis();
	float halfPeriod = 0.5*(1000.0/_calibrationPeriod_Hz);
	
	while(millis()-startTimeTTL < _calibrationPulseTime) { // do this for 10 seconds (10000), or 1 s (1000)
        
        digitalWrite(_dacBasler, HIGH);
        delay(halfPeriod);
        digitalWrite(_dacBasler, LOW);
        delay(halfPeriod)
        
        /*
		dac2Write(2, 32767);// set the DAC output to high (TTL_PULSE_HIGH = 32767 --> about 5V)
        long startPeriod1=millis();
        
		while(millis()- startPeriod1 < halfPeriod){
			//wait for 1/2 calibrationPeriod
		};
		
        dac2Write(2, 0);// set the DAC output to low (TTL_PULSE_LOW = 0 --> 0V)
		long startPeriod2=millis();
		
        while(millis()-startPeriod2 < halfPeriod){
			//wait for 1/2 calibrationPeriod
		};
         */
	};
}


int VRiD_VRcontrol::startCameraTrigger(int trialVideoFramerate_Hz, int videoRecoding_ms)
{
	/*
	This function generates a TTL pulse signal to trigger a camera to capture single frames during a VR trial.
	*/
	
	int stateTrialVideo = 1;
	
	long startTimeTTL = millis();
	float halfPeriod = ((1/trialVideoFramerate_Hz)*500);
	
	while(millis()-startTimeTTL < videoRecoding_ms) { // do this for 10 seconds (10000), or 1 s (1000)
        
		dac2Write(2, 32767);// set the DAC output to high (TTL_PULSE_HIGH = 32767 --> about 5V)
        long startPeriod1=millis();
        
		while(millis()- startPeriod1 < halfPeriod){
			//wait for 1/2 calibrationPeriod
		};
		
        dac2Write(2, 0);// set the DAC output to low (TTL_PULSE_LOW = 0 --> 0V)
		long startPeriod2=millis();
		
        while(millis()-startPeriod2 < halfPeriod){
			//wait for 1/2 calibrationPeriod
		};
	};
	stateTrialVideo = 0;
	
	return stateTrialVideo;
}
