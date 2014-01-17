/*
	Ministat.cpp - Library for ___
*/

#include "Arduino.h"
#include "Ministat.h"



void Ministat::Init()
{
	_readPin = 0;
	_writePin = 0;
	_power = 200;

	// Default values
	_automationEnabled = false;
	_state = false;
	_pulseWidth = 50.0;
	_pulseFrequency = 4950.0;
	_pulseVolume = 30.0;
	_pulseRepeat = 100;
	_reactorVolume = 20.0;
	_reactorMass = 20.0;
	_reactorDensity = 1.0;
	_reactorDilutionRate = 0.693;
}


// -rpin
void Ministat::SetReadPin(int val)
{
	_readPin = val;
}
int Ministat::GetReadPin()
{
	return _readPin;
}

// -wpin
void Ministat::SetWritePin(int val)
{
	_writePin = val;
}
int Ministat::GetWritePin()
{
	return _writePin;
}

// - power
void Ministat::SetPower(int val)
{
	_power = val;
}
int Ministat::GetPower()
{
	return _power;
}

// -pwid
void Ministat::SetPulseWidth(float val)
{
	_pulseWidth = val;
}
float Ministat::GetPulseWidth()
{
	return _pulseWidth;
}

// -pfreq
void Ministat::SetPulseFrequency(float val)
{
	_pulseFrequency = val;
}
float Ministat::GetPulseFrequency()
{
	return _pulseFrequency;
}

// -pvol
void Ministat::SetPulseVolume(float val)
{
	_pulseVolume = val;
}
float Ministat::GetPulseVolume()
{
	return _pulseVolume;
}

// -prep
void Ministat::SetPulseRepeat(int val)
{
	_pulseRepeat = val;
}
int Ministat::GetPulseRepeat()
{
	return _pulseRepeat;
}

// cont
void Ministat::ContinuousPump()
{
	// Deactivate any automation before sending a continuous signal.
	Ministat::DeactivatePump();
	analogWrite(_writePin, _power);
}

// pulse
void Ministat::PulsePump()
{
	// Deactivate any automation before sending a pulsatile signal.
	Ministat::DeactivatePump();
	
	// Turn on for <_pulseWidth> milliseconds then off for 100 milliseconds, <_pulseRepeat> times.
	for(int i=0; i<_pulseRepeat; i++)
	{
		analogWrite(_writePin, _power);
		delay(_pulseWidth);
		analogWrite(_writePin, 0);
		delay(100);
	}
}

// auto
void Ministat::AutomatePump()
{
	Ministat::ActivatePump();
}

// -stop
void Ministat::StopPump()
{
	// Deactivate any automation that may be going on.
	Ministat::DeactivatePump();

	// Write low value.
	analogWrite(_writePin, 0);
}

// -rvol
void Ministat::SetReactorVolume(float val)
{
	_reactorVolume = val;
}
float Ministat::GetReactorVolume()
{
	return _reactorVolume;
}
// -rmass
void Ministat::SetReactorMass(float val)
{
	_reactorMass = val;
}
float Ministat::GetReactorMass()
{
	return _reactorMass;
}
// -rden
void Ministat::SetReactorDensity(float val)
{
	_reactorDensity = val;
}
float Ministat::GetReactorDensity()
{
	return _reactorDensity;
}
// -rdil
void Ministat::SetReactorDilutionRate(float val)
{
	_reactorDilutionRate = val;
}
float Ministat::GetReactorDilutionRate()
{
	return _reactorDilutionRate;
}

boolean Ministat::GetStatus()
{
	return _automationEnabled;
}


// PRIVATE
void Ministat::DeactivatePump()
{
	_automationEnabled = false;
}
void Ministat::ActivatePump()
{
	_automationEnabled = true;
}

