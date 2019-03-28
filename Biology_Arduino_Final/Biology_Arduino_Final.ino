#include<ArduinoJson.h>

const int  buttonPin = 7;    // the pin that the pushbutton is attached to
const int  buttonPin2 = 6;    // the pin that the pushbutton is attached to
const long waitDebounce = 1000;  // waitDebounce agengst bounce
int counterOne = 0;   // counter for the number of button presses
int counterTwo = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int buttonState2 = 0;         // current state of the button
float volumeGas = 7.9;
int lastButtonState = 0;     // previous state of the button
int lastButtonState2 = 0;     // previous state of the button
unsigned long previousMillis = 0;        // will store last time switch was on
unsigned long previousMillis2 = 0;        // will store last time switch was on

// used for themperature sensor
const int sensorpH = A0;
const int sensorTemp = A1;

float temperature = 0;
// Used for pH sensor
float pH;
float voltage;

void setup()
{
  pinMode(buttonPin, INPUT);
  pinMode(buttonPin2, INPUT);
  // initialize serial communication:
  Serial.begin(9600);
}

float abstemp_cma(float value)   // value is referring to the ADC reading 0-1023
{
  float Rntc, Vout;
  float Rfixed = 20000.0;      //20k
  float Vin = 5.0;             //5000mV.
  float T_0 = 273.15;          //Celsius to Kelvin offset
  float K0 = 0.00102119;       //constant specific for the temperature sensor
  float K1 = 0.000222468;      //constant specific for the temperature sensor
  float K2 = 0.000000133342;   //constant specific for the temperature sensor

  float T, logR;
  //
  //dertermine Vout
  //
  Vout = Vin * value / 1024.0;
  //
  // now calculate Rntc from Vout, Vin and Rfixed
  //
  Rntc = (Vout * Rfixed) / (Vin - Vout);
  //
  logR = log(Rntc);
  T = 1 / (K0 + (K1 * logR) + (K2 * logR * logR * logR));
  //
  return float(10 * (T - T_0));
}

//Smoothing pH readings
float getVoltage() {
  float tempVoltage = 0;
  int window = 10;

  for (int i = 0; i < window; i++) {
    // Read light sensor data.
    tempVoltage = tempVoltage + analogRead(sensorpH);

    // 1ms pause adds more stability between reads.
    delay(1);
  }
  tempVoltage = tempVoltage / window;

  return tempVoltage;
}

void loop() {
  buttonState = digitalRead(buttonPin);
  unsigned long currentMillis = millis();
  // compare the buttonState to its previous state
  if (buttonState != lastButtonState)
  {
    // if the state has changed, increment the counter
    if (buttonState == HIGH && currentMillis - previousMillis >= waitDebounce)
    {
      // if the current state is HIGH then the button went from off to on:
      counterOne++;
      previousMillis = currentMillis;
    }
  }
  lastButtonState = buttonState;  // save the current state as the last state, for next time through the loop


  buttonState2 = digitalRead(buttonPin2);
  unsigned long currentMillis2 = millis();
  // compare the buttonState to its previous state
  if (buttonState2 != lastButtonState2)
  {
    // if the state has changed, increment the counter
    if (buttonState2 == HIGH && currentMillis2 - previousMillis2 >= waitDebounce)
    {
      // if the current state is HIGH then the button went from off to on:
      counterTwo++;
      previousMillis2 = currentMillis2;
    }
  }
  lastButtonState2 = buttonState2;  // save the current state as the last state, for next time through the loop
  
  float voltage = getVoltage();
  sendToSerial(voltage);
  delay(5000);  //testing purpose of delay
}

void sendToSerial(float voltage) {
  // Sending pH sensor reading
  pH = (-0.01926 * voltage + 15.50);
  Serial.println(pH);

  // Sending temperature sensor reading
  temperature = (abstemp_cma(analogRead(sensorTemp)) / 10);
  Serial.println(temperature);

  // Sending volume of gas produced
  Serial.println(counterOne * volumeGas);

  Serial.println(counterTwo * volumeGas);
}

void createJson(){
  DynamicJsonDocument doc(256);
}

