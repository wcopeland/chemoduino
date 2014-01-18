#include <stdio.h>
#include <SimpleTimer.h>
#include <Ministat.h>


const int NUMBER_OF_CHEMOSTATS = 6;
Ministat Chemostats[NUMBER_OF_CHEMOSTATS];
SimpleTimer Timers[NUMBER_OF_CHEMOSTATS];

void setup()
{
  Serial.begin(9600);
  
  // Load defaults
  for(int i=0; i<NUMBER_OF_CHEMOSTATS; i++)
  {
    Chemostats[i].Init();
    
    /*
    if(i==0)
    {
      Chemostats[i].SetWritePin(13);
    }
    else if(i==1)
    {
      Chemostats[i].SetWritePin(11);
    }
    else if(i==2)
    {
      Chemostats[i].SetWritePin(10);
    }
    else if(i==3)
    {
      Chemostats[i].SetWritePin(9);
    }
    else if(i==4)
    {
      Chemostats[i].SetWritePin(6);
    }
    else if(i==5)
    {
      Chemostats[i].SetWritePin(5);
    }*/
  }
}

void loop()
{
  // Run Timers
  for(int i=0; i<NUMBER_OF_CHEMOSTATS; i++)
  {
    Timers[i].run();
  }
  
  // Initialized empty command.
  String command = "";
  
  // Check for command.
  delay(100);
  while(Serial.available())
  {
    char c = Serial.read();
    if(c == '\n')
    {
        break;
    }
    command += c;
  }
  if(command.length() <= 0)
    return;
    
  // Check for valid arguements.
  int argCount = 0;
  for(int i=0; i<command.length(); i++)
  {
    if(command[i] == '-')
      argCount += 2;
  }
  if(argCount <= 0)
    return;
  
  // Separate command arguements.
  String args[argCount];
  int row = 0;
  boolean record = true;
  for(int i=0; i<command.length(); i++)
  {
    if(record && command[i] != ' ')
      args[row] += command[i]; 
    else if(record && command[i] == ' ')
      record = false;
    else if (!record && command[i] != ' ')
    {
       record = true;
       row++;
       args[row] += command[i]; 
    }
  }
  
  // Execute arguments
  String action = args[0];
  int pumpId = args[1].toInt();
  if(action == "-stop")
  {
    Chemostats[pumpId].StopPump();
    Serial.print("(Ministat ");
    Serial.print(pumpId);
    Serial.println(") Pump deactived and stopped.");
  }
  else
  {  
    for(int i=2; i<argCount; i+=2)
    {
      if(action == "-set")
      {
        Serial.println("Value has beem set.");
        
        if(args[i] == "-rpin")
        {
          int val = args[i+1].toInt();
          Chemostats[pumpId].SetReadPin(val);
          //Serial.println("Read pin has been set.");
        }
        else if(args[i] == "-wpin")
        {
          int val = args[i+1].toInt();
          Chemostats[pumpId].SetWritePin(val);
          //Serial.println("Write pin has been set.");
        }
        else if(args[i] == "-power")
        {
          int val = args[i+1].toInt();
          Chemostats[pumpId].SetPower(val);
          //Serial.println("Power has been set.");
        }        
        else if(args[i] == "-pwid")
        {
          float val = args[i+1].toFloat();
          Chemostats[pumpId].SetPulseWidth(val);
          //Serial.println("Pulse width has been set.");
        }
        else if(args[i] == "-pfreq")
        {
          float val = args[i+1].toFloat();
          Chemostats[pumpId].SetPulseFrequency(val);
          //Serial.println("Pulse frequency has been set.");
        }
        else if(args[i] == "-pvol")
        {
          float val = args[i+1].toFloat();
          Chemostats[pumpId].SetPulseVolume(val);
          //Serial.println("Pulse volume has been set.");
        }
        else if(args[i] == "-prep")
        {
          int val = args[i+1].toInt();
          Chemostats[pumpId].SetPulseRepeat(val);
          //Serial.println("Pulse repeat has been set.");
        }
        else if(args[i] == "-rvol")
        {
          float val = args[i+1].toFloat();
          Chemostats[pumpId].SetReactorVolume(val);
          //Serial.println("Reactor volume has been set.");
        }
        else if(args[i] == "-rmass")
        {
          float val = args[i+1].toFloat();
          Chemostats[pumpId].SetReactorMass(val);
          //Serial.println("Reactor mass has been set.");
        }
        else if(args[i] == "-rden")
        {
          float val = args[i+1].toFloat();
          Chemostats[pumpId].SetReactorDensity(val);
          //Serial.println("Reactor volume has been set.");
        }
        else if(args[i] == "-rdil")
        {
          float val = args[i+1].toFloat();
          Chemostats[pumpId].SetReactorDilutionRate(val);
          //Serial.println("Reactor dilution rate has been set.");
        }
        else
        {
          Serial.print("Invalid command: ");
          Serial.println(args[i]);
          break;
        }
      }
      else if(action == "-get")
      {
        Serial.print("Pump ");
        Serial.print(pumpId);

        if(args[i] == "-rpin")
        {
          Serial.print(" read pin: ");
          Serial.println(Chemostats[pumpId].GetReadPin());
          break;
        }
        else if(args[i] == "-wpin")
        {
          Serial.print(" write pin: ");
          Serial.println(Chemostats[pumpId].GetWritePin());
        }
        else if(args[i] == "-power")
        {
          Serial.print(" power: ");
          Serial.println(Chemostats[pumpId].GetPower());
        }
        else if(args[i] == "-pwid")
        {
          Serial.print(" pulse width: ");
          Serial.println(Chemostats[pumpId].GetPulseWidth());
        }
        else if(args[i] == "-pfreq")
        {
          Serial.print(" pulse frequency: ");
          Serial.println(Chemostats[pumpId].GetPulseFrequency());
        }
        else if(args[i] == "-pvol")
        {
          Serial.print(" - Pulse Volume: ");
          Serial.println(Chemostats[pumpId].GetPulseVolume());
        }
        else if(args[i] == "-prep")
        {
          Serial.print(" pulse repeat: ");
          Serial.println(Chemostats[pumpId].GetPulseRepeat());
        }
        else if(args[i] == "-rvol")
        {
          Serial.print(" reactor volume: ");
          Serial.println(Chemostats[pumpId].GetReactorVolume());
        }
        else if(args[i] == "-rmass")
        {
          Serial.print(" reactor mass: ");
          Serial.println(Chemostats[pumpId].GetReactorMass());
        }
        else if(args[i] == "-rden")
        {
          Serial.print(" reactor density: ");
          Serial.println(Chemostats[pumpId].GetReactorDensity());
        }
        else if(args[i] == "-rdil")
        {
          Serial.print(" reactor dilution: ");
          Serial.println(Chemostats[pumpId].GetReactorDilutionRate());
        }
      }
      else if(action == "-start")
      {
        if(args[i] == "-cont")
        {
          Serial.print("(Ministat ");
          Serial.print(pumpId);
          Serial.println(") Pump started in continuous mode.");
          Chemostats[pumpId].ContinuousPump();
        }
        else if(args[i] == "-pulse")
        {
          Serial.print("(Ministat ");
          Serial.print(pumpId);
          Serial.println(") Pump started in pulse mode.");
          Chemostats[pumpId].PulsePump();
        }
        else if(args[i] == "-auto")
        {
          Serial.print("Ministat ");
          Serial.print(pumpId);
          Serial.println(") Pump started in automated mode.");
          Chemostats[pumpId].AutomatePump();
          
          if(pumpId==0)
          {
            Pump0_ON();
          }          
          else if(pumpId==1)
          {
            Pump1_ON();
          }
          else if(pumpId==2)
          {
            Pump2_ON();
          }
          else if(pumpId==3)
          {
            Pump3_ON();
          }
          else if(pumpId==4)
          {
            Pump4_ON();
          }
          else if(pumpId==5)
          {
            Pump5_ON();
          }
        }
      }
      else
      {
        Serial.println("Unknown command received.");
      }
    }
  }
}


void Pump0_ON()
{
  int PID = 0;
  if(Chemostats[PID].GetStatus())
  {
    long t = (long) Chemostats[PID].GetPulseWidth();
    Timers[PID].setTimer(t, Pump0_OFF, 1);
    analogWrite(Chemostats[PID].GetWritePin(), Chemostats[PID].GetPower());
  }
}
void Pump1_ON()
{
  int PID = 1;
  if(Chemostats[PID].GetStatus())
  {
    long t = (long) Chemostats[PID].GetPulseWidth();
    Timers[PID].setTimer(t, Pump1_OFF, 1);
    analogWrite(Chemostats[PID].GetWritePin(), Chemostats[PID].GetPower());
  }
}
void Pump2_ON()
{
  int PID = 2;
  if(Chemostats[PID].GetStatus())
  {
    long t = (long) Chemostats[PID].GetPulseWidth();
    Timers[PID].setTimer(t, Pump2_OFF, 1);
    analogWrite(Chemostats[PID].GetWritePin(), Chemostats[PID].GetPower());
  }
}
void Pump3_ON()
{
  int PID = 3;
  if(Chemostats[PID].GetStatus())
  {
    long t = (long) Chemostats[PID].GetPulseWidth();
    Timers[PID].setTimer(t, Pump3_OFF, 1);
    analogWrite(Chemostats[PID].GetWritePin(), Chemostats[PID].GetPower());
  }
}
void Pump4_ON()
{
  int PID = 4;
  if(Chemostats[PID].GetStatus())
  {
    long t = (long) Chemostats[PID].GetPulseWidth();
    Timers[PID].setTimer(t, Pump4_OFF, 1);
    analogWrite(Chemostats[PID].GetWritePin(), Chemostats[PID].GetPower());
  }
}
void Pump5_ON()
{
  int PID = 5;
  if(Chemostats[PID].GetStatus())
  {
    long t = (long) Chemostats[PID].GetPulseWidth();
    Timers[PID].setTimer(t, Pump5_OFF, 1);
    analogWrite(Chemostats[PID].GetWritePin(), Chemostats[PID].GetPower());
  }
}



void Pump0_OFF()
{
  int PID = 0;
  if(Chemostats[PID].GetStatus())
  {
    long t = (long) Chemostats[PID].GetPulseFrequency();
    Timers[PID].setTimer(t, Pump0_ON, 1);
    analogWrite(Chemostats[PID].GetWritePin(), 0);
  }
}
void Pump1_OFF()
{
  int PID = 1;
  if(Chemostats[PID].GetStatus())
  {
    long t = (long) Chemostats[PID].GetPulseFrequency();
    Timers[PID].setTimer(t, Pump1_ON, 1);
    analogWrite(Chemostats[PID].GetWritePin(), 0);
  }
}
void Pump2_OFF()
{
  int PID = 2;
  if(Chemostats[PID].GetStatus())
  {
    long t = (long) Chemostats[PID].GetPulseFrequency();
    Timers[PID].setTimer(t, Pump2_ON, 1);
    analogWrite(Chemostats[PID].GetWritePin(), 0);
  }
}
void Pump3_OFF()
{
  int PID = 3;
  if(Chemostats[PID].GetStatus())
  {
    long t = (long) Chemostats[PID].GetPulseFrequency();
    Timers[PID].setTimer(t, Pump3_ON, 1);
    analogWrite(Chemostats[PID].GetWritePin(), 0);
  }
}
void Pump4_OFF()
{
  int PID = 4;
  if(Chemostats[PID].GetStatus())
  {
    long t = (long) Chemostats[PID].GetPulseFrequency();
    Timers[PID].setTimer(t, Pump4_ON, 1);
    analogWrite(Chemostats[PID].GetWritePin(), 0);
  }
}
void Pump5_OFF()
{
  int PID = 5;
  if(Chemostats[PID].GetStatus())
  {
    long t = (long) Chemostats[PID].GetPulseFrequency();
    Timers[PID].setTimer(t, Pump5_ON, 1);
    analogWrite(Chemostats[PID].GetWritePin(), 0);
  }
}

