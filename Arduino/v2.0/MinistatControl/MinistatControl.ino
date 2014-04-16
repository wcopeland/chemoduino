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
  }
  
  Chemostats[0].SetWritePin(13);
  Chemostats[1].SetWritePin(11);
  Chemostats[2].SetWritePin(10);
  Chemostats[3].SetWritePin(9);
  Chemostats[4].SetWritePin(6);
  Chemostats[5].SetWritePin(5);  
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
    Serial.print(") Pump deactived and stopped. Please wait ");
    Serial.print(Chemostats[pumpId].GetPulseFrequency());
    Serial.println(" seconds before restarting.");
  }
  else
  {  
    for(int i=2; i<argCount; i+=2)
    {
      if(action == "-set")
      {
        Serial.println("Value set.");
        if(args[i] == "-rpin")
        {
          int val = args[i+1].toInt();
          Chemostats[pumpId].SetReadPin(val);
        }
        else if(args[i] == "-wpin")
        {
          int val = args[i+1].toInt();
          Chemostats[pumpId].SetWritePin(val);
        }
        else if(args[i] == "-power")
        {
          int val = args[i+1].toInt();
          Chemostats[pumpId].SetPower(val);
        }        
        else if(args[i] == "-pwid")
        {
          float val = args[i+1].toFloat();
          Chemostats[pumpId].SetPulseWidth(val);
        }
        else if(args[i] == "-pfreq")
        {
          float val = args[i+1].toFloat();
          Chemostats[pumpId].SetPulseFrequency(val);
        }
        else if(args[i] == "-prep")
        {
          int val = args[i+1].toInt();
          Chemostats[pumpId].SetPulseRepeat(val);
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
        else if(args[i] == "-prep")
        {
          Serial.print(" pulse repeat: ");
          Serial.println(Chemostats[pumpId].GetPulseRepeat());
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
            Automate_0_ON();
          }          
          else if(pumpId==1)
          {
            Automate_1_ON();
          }
          else if(pumpId==2)
          {
            Automate_2_ON();
          }
          else if(pumpId==3)
          {
            Automate_3_ON();
          }
          else if(pumpId==4)
          {
            Automate_4_ON();
          }
          else if(pumpId==5)
          {
            Automate_5_ON();
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


void Automate_0_ON()
{
  if(Chemostats[0].GetStatus())
  {
    long t = (long) Chemostats[0].GetPulseWidth();
    Timers[0].setTimeout(t, Automate_0_OFF);
    analogWrite(Chemostats[0].GetWritePin(), Chemostats[0].GetPower());
  }
}
void Automate_1_ON()
{
  if(Chemostats[1].GetStatus())
  {
    long t = (long) Chemostats[1].GetPulseWidth();
    Timers[1].setTimeout(t, Automate_1_OFF);
    analogWrite(Chemostats[1].GetWritePin(), Chemostats[1].GetPower());
  }
}
void Automate_2_ON()
{
  if(Chemostats[2].GetStatus())
  {
    long t = (long) Chemostats[2].GetPulseWidth();
    Timers[2].setTimeout(t, Automate_2_OFF);
    analogWrite(Chemostats[2].GetWritePin(), Chemostats[2].GetPower());
  }
}
void Automate_3_ON()
{
  if(Chemostats[3].GetStatus())
  {
    long t = (long) Chemostats[3].GetPulseWidth();
    Timers[3].setTimeout(t, Automate_3_OFF);
    analogWrite(Chemostats[3].GetWritePin(), Chemostats[3].GetPower());
  }
}
void Automate_4_ON()
{
  if(Chemostats[4].GetStatus())
  {
    long t = (long) Chemostats[4].GetPulseWidth();
    Timers[4].setTimeout(t, Automate_4_OFF);
    analogWrite(Chemostats[4].GetWritePin(), Chemostats[4].GetPower());
  }
}
void Automate_5_ON()
{
  if(Chemostats[5].GetStatus())
  {
    long t = (long) Chemostats[5].GetPulseWidth();
    Timers[5].setTimeout(t, Automate_5_OFF);
    analogWrite(Chemostats[5].GetWritePin(), Chemostats[5].GetPower());
  }
}



void Automate_0_OFF()
{
  if(Chemostats[0].GetStatus())
  {
    long t = (long) Chemostats[0].GetPulseFrequency();
    Timers[0].setTimeout(t, Automate_0_ON);
    analogWrite(Chemostats[0].GetWritePin(), 0);
  }
}
void Automate_1_OFF()
{
  if(Chemostats[1].GetStatus())
  {
    long t = (long) Chemostats[1].GetPulseFrequency();
    Timers[1].setTimeout(t, Automate_1_ON);
    analogWrite(Chemostats[1].GetWritePin(), 0);
  }
}
void Automate_2_OFF()
{
  if(Chemostats[2].GetStatus())
  {
    long t = (long) Chemostats[2].GetPulseFrequency();
    Timers[2].setTimeout(t, Automate_2_ON);
    analogWrite(Chemostats[2].GetWritePin(), 0);
  }
}
void Automate_3_OFF()
{
  if(Chemostats[3].GetStatus())
  {
    long t = (long) Chemostats[3].GetPulseFrequency();
    Timers[3].setTimeout(t, Automate_3_ON);
    analogWrite(Chemostats[3].GetWritePin(), 0);
  }
}
void Automate_4_OFF()
{
  if(Chemostats[4].GetStatus())
  {
    long t = (long) Chemostats[4].GetPulseFrequency();
    Timers[4].setTimeout(t, Automate_4_ON);
    analogWrite(Chemostats[4].GetWritePin(), 0);
  }
}
void Automate_5_OFF()
{
  if(Chemostats[5].GetStatus())
  {
    long t = (long) Chemostats[5].GetPulseFrequency();
    Timers[5].setTimeout(t, Automate_5_ON);
    analogWrite(Chemostats[5].GetWritePin(), 0);
  }
}

