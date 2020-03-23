#include <SPI.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const int CS = 10;
const int Vout = 8;  //Powers the Potentiometer
const int Rstep =39; //ohms
const float Vstart = 5.06; // volts
const float K=0.0341;  //cell constant to be determined.
const float Tcoef = 0.02;

int PWV = A0;   //Pot wiper voltage
int RawVoltage = 0;
int level =128;
float Voltage = 0;
int R1;
float R2;
int Rc=45; //resistance of digital pot.
float EC;
float EC25;
float Temperature;

//Setup onewire
#define ONE_WIRE_BUS 7 // Data wire For Temp Probe is plugged into pin 7 on the Arduino

OneWire oneWire(ONE_WIRE_BUS);// Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);// Pass our oneWire reference to Dallas Temperature.


const int aIn = A0;
int cV =0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(CS, OUTPUT);
  pinMode(Vout, OUTPUT);
  SPI.begin();
  sensors.begin();
  MCP41010Write(level);
  delay(100);
}

void loop() {
  // put your main code here, to run repea// Get level from read serial
  if (Serial.available()>0){
    level=Serial.readString().toInt();
  }
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
  cV = int(cV/10);
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
