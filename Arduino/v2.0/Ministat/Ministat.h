/*
Ministat.h - Library for controling miniature chemostats via Arduino.
Created by Wilbert B. Copeland, August 29, 2013.
Laboratory of Herbert Sauro, University of Washington.
*/

#ifndef Ministat_h
#define Ministat_h
//#include <stdio.h>


class Ministat
{
public:

	//Ministat(int read_pin, int write_pin);	
	void Init();

	// -rpin
	void SetReadPin(int val);
	int GetReadPin();
	// -wpin
	void SetWritePin(int val);
	int GetWritePin();
	// -power
	void SetPower(int val);
	int GetPower();
	// -pwid
	void SetPulseWidth(float val);
	float GetPulseWidth();
	// -pfreq
	void SetPulseFrequency(float val);
	float GetPulseFrequency();
	// -prep
	void SetPulseRepeat(int val);
	int GetPulseRepeat();

	// -cont
	void ContinuousPump();
	// -pulse
	void PulsePump();
	// -stop
	void StopPump();
	// -auto
	void AutomatePump();

	boolean GetStatus();
	


private:
	int _readPin;
	int _writePin;
	int _power;
	boolean _automationEnabled;
	boolean _state;
	float _pulseWidth;
	float _pulseFrequency;
	int _pulseRepeat;

	void ActivatePump();
	void DeactivatePump();
};

#endif
