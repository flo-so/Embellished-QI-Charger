/*
  The Board: 
  - Digispark ATTiny 85
  The circuit:
  - TPO (Total Power Output) of QI-Charger connected to analog Input "sensorPin"
  - LED anode (long leg) attached to PWM Output "ledPin" (don't forget the resistor to control max. current)
    cathode (short leg) attached to ground
  - Debug LED onboard LED digital output "debugLedPin"
*/

//Debug
#define DEBUG 0
#ifdef DEBUG
  const int debugLedPin = 1;
#endif

//Konstanten -> Datensatz
const int sensorPin = 1;                  //P2 = Analog 1
const int ledPin = 0;                     //P0 = Digital 0
const int keinLadenGrenzwert = 64;
const int langsamLadenGrenzwert = 256;
const int langsamLadenPwmMin = 112;
const int langsamLadenPwmMax = 240;
const int langsamLadenDelay = 15;
const int fehlerLadenDelay = 300;
const int schnellLadenPwmMin = 16;
const int schnellLadenPwmMax = 208;
const int schnellLadenDelay = 10;

//Variable
int sensorValue;
bool laden = false;

void setup() {
    #ifdef DEBUG
      pinMode(debugLedPin, OUTPUT);
    #endif

  pinMode(sensorPin,INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  sensorValue = analogRead(sensorPin);
  
  if(sensorValue < keinLadenGrenzwert)                              //kein Laden
  {
      #ifdef DEBUG
        digitalWrite(debugLedPin, HIGH);
        delay(500);
        digitalWrite(debugLedPin, LOW);
        delay(500);
      #endif
    
    digitalWrite(ledPin, HIGH);
    laden = false;
  }
  else if(sensorValue < langsamLadenGrenzwert)
  {
    if(laden)                                                       //langsam Laden
    {
      #ifdef DEBUG
        digitalWrite(debugLedPin, HIGH);
        delay(500);
        digitalWrite(debugLedPin, LOW);
      #endif
      for(int i = langsamLadenPwmMax; i > langsamLadenPwmMin; i--)
      {
        analogWrite(ledPin, i);
        delay(langsamLadenDelay);
      }
      for (int i = langsamLadenPwmMin; i < langsamLadenPwmMax; i++) 
      {
        analogWrite(ledPin, i);
        delay(langsamLadenDelay);
      }
    }
    else                                                              //fehlerhaftes Laden
    {
      digitalWrite(ledPin, HIGH);
      delay(fehlerLadenDelay);
      digitalWrite(ledPin, LOW);
      delay(fehlerLadenDelay);
    }
  }
  else                                                                //schnelles Laden
  {
      #ifdef DEBUG
        digitalWrite(debugLedPin, HIGH);
        delay(200);
        digitalWrite(debugLedPin, LOW);
      #endif

    laden = true;
    for(int i = schnellLadenPwmMax; i > schnellLadenPwmMin; i--)
    {
      analogWrite(ledPin, i);
      delay(schnellLadenDelay);
    }
    for (int i = schnellLadenPwmMin; i < schnellLadenPwmMax; i++) 
    {
      analogWrite(ledPin, i);
      delay(schnellLadenDelay);
    }
  }
}
