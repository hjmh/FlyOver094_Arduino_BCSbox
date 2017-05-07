/*
 *  VRiD_VRcontrol_v0021_2.h  --> runs with Arduino v0021
 *  Library for various functions used to control the VR via serial 
 *  communication from the VR and Data PC
 *
 *  Created by Hannah Haberkern on 4/5/12.
 *  Copyright 2012 ETH Zürich. All rights reserved.
 *
 *  Updated 19/11/2015 by Hannah
 *
 */

#ifndef VRiD_VRcontrol_h
#define VRiD_VRcontrol_h
#include <SPI.h>// include the SPI library code
#include <VRiD_VRsetup_v0021_2.h>

class VRiD_VRcontrol
	{
	public:
	  //variables
		 
	  //functions
		
		VRiD_VRcontrol(long timeOutInterval, int dacScreenLED, int dacBasler, int dacIRLaser, int calibrationPeriod_Hz, int calibrationPulseTime, int dac2pin);
		
		void initializeDacVariables(int dacScreenLED, int dacBasler, int dacIRLaser);
		
		void performCalibration();
		
		void beginRecordingTrialVideo(int trialVideoFramerate_Hz, int videoRecoding_ms);
		
		//void stopRecordingTrialVideo();
		
		int screensOn();
		int screensOff();
		
		int baslerOn();
		int baslerOff();
		
		int laserOn(int dutyCycle);
		int laserOff();
		
		//int punishZone(uint8_t zoneSpec, int zoneDutyCycle);
		
		//int checkTimeOut();
		//int startTimeOutTimer();
		//int resetTimeOutTimer();
		
		void dac2Write(uint8_t dac, int16_t value);
		
	private:
	  //variables
		int _dacScreenLED;
		int _dacBasler;
		int _dacIRLaser;

		int _dac2pin;
		
		long _timeOutInterval;
		long _timeOutTimer;
		int _timeOutStarted;
		
		int _calibrationPeriod_Hz;
		int _calibrationPulseTime;
		
	  //functions
		int initiateReward(int pin, int dutyValue);
		
		void TTLpulseWhile();
		
		int startCameraTrigger(int trialVideoFramerate_Hz, int videoRecoding_ms);
		
	};
#endif