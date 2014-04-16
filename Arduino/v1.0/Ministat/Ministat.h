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
	// -pvol
	void SetPulseVolume(float val);
	float GetPulseVolume();
	// -prep
	void SetPulseRepeat(int val);
	int GetPulseRepeat();
	// -rvol
	void SetReactorVolume(float val);
	float GetReactorVolume();
	// -rmass
	void SetReactorMass(float val);
	float GetReactorMass();
	// -rden
	void SetReactorDensity(float val);
	float GetReactorDensity();
	// -rdil
	void SetReactorDilutionRate(float val);
	float GetReactorDilutionRate();

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
	float _pulseVolume;
	int _pulseRepeat;
	float _reactorVolume;
	float _reactorMass;
	float _reactorDensity;
	float _reactorDilutionRate;

	void ActivatePump();
	void DeactivatePump();
};

#endif
