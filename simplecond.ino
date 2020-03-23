#include <OneWire.h>
#include <DallasTemperature.h>

const int CS = 10;
const float Vstart = 5.06; // volts
const float K=0.0341;  //cell constant to be determined.
const float Tcoef = 0.02;

const int aIn = A0;   //Voltage drop across a 1K resistor.

int RawVoltage = 0;
int level =128;
float Voltage = 0;
int R1;
float R2;
int Rc=45; //resistance of digital pot.
float EC;
float EC25;
float Temperature;
float cv;

//Setup onewire
#define ONE_WIRE_BUS 7 // Data wire For Temp Probe is plugged into pin 7 on the Arduino

OneWire oneWire(ONE_WIRE_BUS);// Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);// Pass our oneWire reference to Dallas Temperature.





void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(CS, OUTPUT);
  pinMode(Vout, OUTPUT);
  sensors.begin();
  delay(100);
}

void loop() {
  
  //Turn on probe
  digitalWrite(Vout, HIGH);
  delay(20);
  for (int i =0; i<10;i++){
    //average readings
    cV+=analogRead(aIn);
    delay(10);
  }

  //turn off probe
  analogWrite(Vout, LOW);


  //Calculate the conductivity
  cV = cV/10.0;
  Voltage = (cV * Vstart)/ 1023.0;
  R1=level*Rstep+Rc+Rstep;
  R2=Voltage*R1/(Vstart-Voltage);
    //*********Reading Temperature Of Solution *******************//
  sensors.requestTemperatures();// Send the command to get temperatures
  Temperature=sensors.getTempCByIndex(0); //Stores Value in Variable

  EC=1000/(R2*K);
  EC25 = EC/(1+Tcoef*(Temperature-25.0));
  Serial.print("Temp  ");
  Serial.print(Temperature);
  Serial.print(" ");
  Serial.print("R1 ");
  Serial.print(" ");
  Serial.print(R1);
  Serial.print(" ");
  Serial.print("R2 ");
  Serial.print(R2);
  Serial.print(" ");
  Serial.print("Conductivity (ms) ");
  Serial.println(EC25);
  delay(5000);
}

void MCP41010Write(byte value)
{
  // Note that the integer vale passed to this subroutine
  // is cast to a byte
  
  digitalWrite(CS,LOW);
  SPI.transfer(B00010001); // This tells the chip to set the pot
  SPI.transfer(value); // This tells it the pot position
  digitalWrite(CS,HIGH);
}