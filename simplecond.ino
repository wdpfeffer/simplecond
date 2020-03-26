#include <OneWire.h>
#include <DallasTemperature.h>

const float Vstart = 5.06; // volts
const float K=1.127;  //cell constant to be determined.
const float Tcoef = 0.02;
const int R1 = 1000;       //adjust based on actual R value
const int aIn = A0;

//Define pins
  //voltage sense of 1K resistor
#define Vout 5   //Pin 5 to turn on voltage for conductivity

//Setup onewire
#define ONE_WIRE_BUS 7 // Data wire For Temp Probe is plugged into pin 7 on the Arduino

//setup varialbes
int level =128;
float Voltage = 0;
float R2;
float i1;
float EC;
float EC25;
float Temperature;
float cV;


OneWire oneWire(ONE_WIRE_BUS);// Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);// Pass our oneWire reference to Dallas Temperature.





void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(Vout, OUTPUT);
  sensors.begin();
  delay(100);
}

void loop() {
  
  //Turn on probe
  digitalWrite(Vout, HIGH);
  cV = analogRead(aIn);
  cV = 0;
  delay(20);
  for (int i =0; i<10;i++){
    //average readings
    cV+=analogRead(aIn);
    delay(10);
  }

  //turn off probe
  digitalWrite(Vout, LOW);


  //Calculate the conductivity
  cV = cV/10.0;
  Voltage = (cV * Vstart)/ 1023.0;
  i1 = Voltage/R1;
  R2 = (Vstart-Voltage)/i1;

    //*********Reading Temperature Of Solution *******************//
  sensors.requestTemperatures();// Send the command to get temperatures
  Temperature=sensors.getTempCByIndex(0); //Stores Value in Variable

  EC=1000/(R2*K);
  EC25 = EC/(1+Tcoef*(Temperature-25.0));
  Serial.print("Temp ");
  Serial.print(Temperature);
  Serial.print("(C) ");
  Serial.print("R2 ");
  Serial.print(R2/1000.0);
  Serial.print("(Kohm) ");
  Serial.print("Conductivity (us/cm2) ");
  Serial.println(EC25*1000);

  //reset cV
  cV = 0;
  //delay the conductivity readings so we do not electrochemically break down the solution
  delay(5000);
}
