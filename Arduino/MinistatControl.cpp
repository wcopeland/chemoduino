#include <stdio.h>
#include <SimpleTimer.h>
#include <Ministat.h>

/* Ministat Control
*
* Interface with an Arduino to control up to six peristaltic pumps configured in a
* ministat system.
*
*
* 
*
*
*/


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
    }
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
  // Note: This only checks for the existence of '-'. If the string is not
  //       properly formatted, you will get junk.
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
    //if(record && command[i] == '-')
    //  args[row] += command[i];
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
  else if(action == "-help")
  {
    Serial.println("========== Chemoduino Help ===========");
    Serial.println("Start Pump: -start <id> <cont | pulse | auto>");
    Serial.println("Stop Pump: -stop");
    Serial.println("Set Ministat Property: -set <id> -<property> <val>");
    Serial.println("\t\tDescription\tProperty\tValue");
    Serial.println("\t\tRead Pin\t-rpin\t\tinteger");
    Serial.println("\t\tWrite Pin\t-wpin\t\tinteger");
    Serial.println("\t\tPulse Width\t-pwid\t\tfloat");
    Serial.println("\t\tPulse Frequency\t-pfreq\t\tfloat");
    Serial.println("\t\t");
  }
  else
  {  
    for(int i=2; i<argCount; i+=2)
    {
      if(action == "-set")
      {
        if(args[i] == "-rpin")
        {
          int val = args[i+1].toInt();
          Chemostats[pumpId].SetReadPin(val);
          
          Serial.print("Read pin set to ");
          Serial.print(val);
          Serial.print(" [Ministat ");
          Serial.print(pumpId);
          Serial.println("]");
        }
        else if(args[i] == "-wpin")
        {
          int val = args[i+1].toInt();
          Chemostats[pumpId].SetWritePin(val);
          
          Serial.print("Write pin set to ");
          Serial.print(val);
          Serial.print(" [Ministat ");
          Serial.print(pumpId);
          Serial.println("]");
        }
        else if(args[i] == "-power")
        {
          int val = args[i+1].toInt();
          Chemostats[pumpId].SetPower(val);
          
          Serial.print("Power set to ");
          Serial.print(val);
          Serial.print(" [Ministat ");
          Serial.print(pumpId);
          Serial.println("]");
        }        
        else if(args[i] == "-pwid")
        {
          float val = args[i+1].toFloat();
          Chemostats[pumpId].SetPulseWidth(val);
          
          Serial.print("Pulse width set to ");
          Serial.print(val);
          Serial.print(" [Ministat ");
          Serial.print(pumpId);
          Serial.println("]");
        }
        else if(args[i] == "-pfreq")
        {
          float val = args[i+1].toFloat();
          Chemostats[pumpId].SetPulseFrequency(val);
          
          Serial.print("Pulse frequency set to ");
          Serial.print(val);
          Serial.print(" [Ministat ");
          Serial.print(pumpId);
          Serial.println("]");
        }
        else if(args[i] == "-pvol")
        {
          float val = args[i+1].toFloat();
          Chemostats[pumpId].SetPulseVolume(val);
          
          Serial.print("Pulse volume set to ");
          Serial.print(val);
          Serial.print(" [Ministat ");
          Serial.print(pumpId);
          Serial.println("]");
        }
        else if(args[i] == "-prep")
        {
          int val = args[i+1].toInt();
          Chemostats[pumpId].SetPulseRepeat(val);
          
          Serial.print("Pulse repetition set to ");
          Serial.print(val);
          Serial.print(" [Ministat ");
          Serial.print(pumpId);
          Serial.println("]");
        }
        else if(args[i] == "-rvol")
        {
          float val = args[i+1].toFloat();
          Chemostats[pumpId].SetReactorVolume(val);
          
          Serial.print("Reactor volume set to ");
          Serial.print(val);
          Serial.print(" [Ministat ");
          Serial.print(pumpId);
          Serial.println("]");
        }
        else if(args[i] == "-rmass")
        {
          float val = args[i+1].toFloat();
          Chemostats[pumpId].SetReactorMass(val);
          
          Serial.print("Reactor mass set to ");
          Serial.print(val);
          Serial.print(" [Ministat ");
          Serial.print(pumpId);
          Serial.println("]");
        }
        else if(args[i] == "-rden")
        {
          float val = args[i+1].toFloat();
          Chemostats[pumpId].SetReactorDensity(val);
          
          Serial.print("Reactor density set to ");
          Serial.print(val);
          Serial.print(" [Ministat ");
          Serial.print(pumpId);
          Serial.println("]");
        }
        else if(args[i] == "-rdil")
        {
          float val = args[i+1].toFloat();
          Chemostats[pumpId].SetReactorDilutionRate(val);
          
          Serial.print("Reactor dilution rate set to ");
          Serial.print(val);
          Serial.print(" [Ministat ");
          Serial.print(pumpId);
          Serial.println("]");
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
        if(args[i] == "-rpin")
        {
          Serial.print("Read pin is set to ");
          Serial.print(Chemostats[pumpId].GetReadPin());
          Serial.print(" [Ministat ");
          Serial.print(pumpId);
          Serial.println(" ]");
          break;
        }
        else if(args[i] == "-wpin")
        {
          Serial.print("Write pin is set to ");
          Serial.print(Chemostats[pumpId].GetWritePin());
          Serial.print(" [Ministat ");
          Serial.print(pumpId);
          Serial.println(" ]");
        }
        else if(args[i] == "-power")
        {
          Serial.print("Pin power is set to ");
          Serial.print(Chemostats[pumpId].GetPower());
          Serial.print(" [Ministat ");
          Serial.print(pumpId);
          Serial.println(" ]");
        }
        else if(args[i] == "-pwid")
        {
          Serial.print("Pulse width is set to ");
          Serial.print(Chemostats[pumpId].GetPulseWidth());
          Serial.print(" [Ministat ");
          Serial.print(pumpId);
          Serial.println(" ]");
        }
        else if(args[i] == "-pfreq")
        {
          Serial.print("Pulse frequency is set to ");
          Serial.print(Chemostats[pumpId].GetPulseFrequency());
          Serial.print(" [Ministat ");
          Serial.print(pumpId);
          Serial.println(" ]");
        }
        else if(args[i] == "-pvol")
        {
                    Serial.print("Pulse volume is set to ");
          Serial.print(Chemostats[pumpId].GetPulseVolume());
          Serial.print(" [Ministat ");
          Serial.print(pumpId);
          Serial.println(" ]");
        }
        else if(args[i] == "-prep")
        {
          Serial.print("Pulse repetition is set to ");
          Serial.print(Chemostats[pumpId].GetPulseRepeat());
          Serial.print(" [Ministat ");
          Serial.print(pumpId);
          Serial.println(" ]");
        }
        else if(args[i] == "-rvol")
        {
          Serial.print("Reactor volume is set to ");
          Serial.print(Chemostats[pumpId].GetReactorVolume());
          Serial.print(" [Ministat ");
          Serial.print(pumpId);
          Serial.println(" ]");
        }
        else if(args[i] == "-rmass")
        {
          Serial.print("Reactor mass is set to ");
          Serial.print(Chemostats[pumpId].GetReactorMass());
          Serial.print(" [Ministat ");
          Serial.print(pumpId);
          Serial.println(" ]");
        }
        else if(args[i] == "-rden")
        {
          Serial.print("Reactor density is set to ");
          Serial.print(Chemostats[pumpId].GetReactorDensity());
          Serial.print(" [Ministat ");
          Serial.print(pumpId);
          Serial.println(" ]");
        }
        else if(args[i] == "-rdil")
        {
          Serial.print("Reactor dilution rate is set to ");
          Serial.print(Chemostats[pumpId].GetReactorDilutionRate());
          Serial.print(" [Ministat ");
          Serial.print(pumpId);
          Serial.println(" ]");
        }
      }
      else if(action == "-start")
      {
        if(args[i] == "-cont")
        {
          Serial.print("Pump started in continuous mode. [Ministat ");
          Serial.print(pumpId);
          Serial.println("]");
          Chemostats[pumpId].ContinuousPump();
        }
        else if(args[i] == "-pulse")
        {
          Serial.print("Pump started in pulse mode. [Ministat ");
          Serial.print(pumpId);
          Serial.println("]");
          Chemostats[pumpId].PulsePump();
        }
        else if(args[i] == "-auto")
        {
          Serial.print("Pump started in automated mode. [Ministat ");
          Serial.print(pumpId);
          Serial.println("]");
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
