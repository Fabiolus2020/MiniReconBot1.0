//Fabiolus
//the_fabiolous@hotmail.com
//2020/09/28

// Library: TMRh20/RF24, https://github.com/tmrh20/RF24/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 myRadio(8, 9); // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00001";


struct package
{
  int joyposX;
  int joyposY;
  int potValue1;
  int potValue2;
  int modeNow;
};

typedef struct package Package;
Package data;

// global variables
#define ain1  5  // Motor A input 1
#define ain2  6  // Motor A input 2
#define bin1  10 // Motor B input 1
#define bin2  3 // Motor B input 2
#define slp  4  // DRV8833 sleep/enable
#define flt  2   // overcurrent/overheat fault

// Define Joystick Connections
//#define joyX A0
//#define joyY A1

//int joyposX = 512;
//int joyposY = 512;

int MotorSpeed1 = 0;
int MotorSpeed2 = 0;

// Speed control potentiometers
//int SpeedControl = A2;


void setup() {

  while (!Serial);

  Serial.begin(9600);
  myRadio.begin();
  //set the address
  myRadio.openReadingPipe(0, address);
  myRadio.setChannel(115);
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS );
  //Set module as receiver
  myRadio.startListening();

  pinMode(ain1, OUTPUT);
  pinMode(ain2, OUTPUT);
  pinMode(bin1, OUTPUT);
  pinMode(bin2, OUTPUT);
  pinMode(slp, OUTPUT);
  pinMode(flt, INPUT_PULLUP);
  digitalWrite(ain1, LOW);
  digitalWrite(ain2, LOW);
  digitalWrite(bin1, LOW);
  digitalWrite(bin2, LOW);
}

void loop() {

  if ( myRadio.available())
  {
    myRadio.read( &data, sizeof(data) );
    // put your main code here, to run repeatedly:
    //  joyposX = analogRead(joyX);
    //  joyposY = analogRead(joyY);
    //  MotorSpeed1 = analogRead(SpeedControl);
    // MotorSpeed2 = analogRead(SpeedControl);


    Serial.println(data.joyposX);
   // Serial.println(data.joyposY);
   // Serial.println(data.potValue1);
    //Serial.println(joyposY);

    // check for a fault
    if (digitalRead(flt) != LOW) {




      if (data.joyposX > 510)
      {
        // enable DRV8833
        digitalWrite(slp, HIGH);
        MotorSpeed1 = map(data.joyposX, 510, 1023, 0, 255);
        MotorSpeed2 = map(data.joyposX, 510, 1023, 0, 255);
        // This is forward
        digitalWrite(ain1, LOW);
        analogWrite(ain2, MotorSpeed1);
        analogWrite(bin1, MotorSpeed2);
        digitalWrite(bin2, LOW);

      }

      else if (data.joyposX < 480)
      {
        // enable DRV8833
        digitalWrite(slp, HIGH);
        MotorSpeed1 = map(data.joyposX, 480, 0, 0, 255);
        MotorSpeed2 = map(data.joyposX, 480, 0, 0, 255);

        // This is forward
        analogWrite(ain1, MotorSpeed1);
        digitalWrite(ain2, LOW);
        digitalWrite(bin1, LOW);
        analogWrite(bin2, MotorSpeed2);

      }
      else  if (data.joyposY < 505)
      {
        // enable DRV8833
        digitalWrite(slp, HIGH);
        MotorSpeed1 = map(data.joyposY, 505, 0, 0, 255);
        MotorSpeed2 = map(data.joyposY, 505, 0, 0, 255);

        // This is forward
        analogWrite(ain1, MotorSpeed1);
        digitalWrite(ain2, LOW);
        analogWrite(bin1, MotorSpeed2);
        digitalWrite(bin2, LOW);

      }
      else  if (data.joyposY > 520)
      {
        // enable DRV8833
        digitalWrite(slp, HIGH);
        MotorSpeed1 = map(data.joyposY, 520, 1023, 0, 255);
        MotorSpeed2 = map(data.joyposY, 520, 1023, 0, 255);

        // This is forward
        digitalWrite(ain1, LOW);
        analogWrite(ain2, MotorSpeed1);
        digitalWrite(bin1, LOW);
        analogWrite(bin2, MotorSpeed2);

      }
      else {
        digitalWrite(ain1, LOW);
        digitalWrite(ain2, LOW);
        digitalWrite(bin1, LOW);
        digitalWrite(bin2, LOW);
        // disable DRV8833, conserve power
        digitalWrite(slp, LOW);
      }
    }
    //else {

    //  digitalWrite(ain1, LOW);
    //  digitalWrite(ain2, LOW);
    //  digitalWrite(bin1, LOW);
    //  digitalWrite(bin2, LOW);
    // disable DRV8833, conserve power
    // digitalWrite(slp, LOW);
    //}

    delay(20);
  }
}
