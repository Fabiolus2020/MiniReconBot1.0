//Fabiolus 2021/02/23
//the_fabiolous@otmail.com

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
  int modeCount;
  int buttonJoystickCount;
  int buttonForwardCount;
  int animation1Count;
  int animation2Count;
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

// Set initial motor direction as forward and speed at 0
//int motorSpeedA = 0;
//int motorSpeedB = 0;
//int motordirection = 0;
int MotorSpeed1 = 0;
int MotorSpeed2 = 0;

void modeOne()
{
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


void modeTwo()
{

}

void modeThree()
{


}

void modeFour()
{
  
}

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

    if (data.modeCount == 1)
    {

      modeOne();
    }
    if (data.modeCount == 2)
    {
      modeTwo();
    }
    if (data.modeCount == 3)
    {
      modeThree();
    }
    if (data.modeCount == 4)
    {
      modeFour();
    }

  }

}
