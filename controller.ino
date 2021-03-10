//Fabiolus 2021/02/23
//the_fabiolous@hotmail.com
#include <ezButton.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 myRadio(8, 9); // CE, CSN

//address through which two modules communicate.
const byte address[6] = "00001";

//data being sent to the receiver, joystick position, pot values and button count.
struct package
{
  int joyposX;
  int joyposY;
  int potValue1;
  int potValue2;
  int modeCount;
};


//define object for wieless communication
typedef struct package Package;
Package data;

// Define Joystick Connections
#define joyX A0
#define joyY A1
#define readPot1 A2
#define readPot2 A3
#define ledModeStatusCount 6

// Define Joystick Values - Start at 512 (middle position) this might change depending on batteries and motor type.
int joyposX = 512;
int joyposY = 512;
int potValue1 = 0;
int potValue2 = 0;


//initial value of animation1 and 2 //animation1 is to control counter to cycle throught Mode1(frontlight,backlight,left,rightlight)
//animation2 is a counter to cycle through the ALA predefined templates, 7 in totals that I chose based on current, the lowest the better.
int animation1Count = 0;
int animation2Count = 0;
int buttonJoystickCount = 0;
int modeCount;

//defining buttons using ezbutton library
ezButton modeButton(7);  // create ezButton object that attach to pin 7;
ezButton buttonJoystick(5);  // create ezButton object that attach to pin 5;
ezButton buttonForward(4);  // create ezButton object that attach to pin 4;
ezButton buttonBackward(3);  // create ezButton object that attach to pin 3;

void setup() {
  Serial.begin(9600);

  //define Radio as object
  myRadio.begin();

  //set the address
  myRadio.openWritingPipe(address);
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS );
  myRadio.setChannel(115);

  //Set module as transmitter
  myRadio.stopListening();
  pinMode(ledModeStatusCount, OUTPUT);
  digitalWrite(ledModeStatusCount, LOW);
  delay(10);


  //The below I use for debounce issue using the EZbutton library, had to adjust for the switch button on thumbjoystick
  modeButton.setDebounceTime(50); // set debounce time to 50 milliseconds
  buttonJoystick.setDebounceTime(80); // set debounce time to 80 milliseconds
  buttonForward.setDebounceTime(50); // set debounce time to 50 milliseconds
  buttonBackward.setDebounceTime(50); // set debounce time to 50 milliseconds
}
void loop() {

  modeButton.loop(); // MUST call the loop() function first
  buttonJoystick.loop(); // MUST call the loop() function first
  buttonForward.loop(); // MUST call the loop() function first
  buttonBackward.loop(); // MUST call the loop() function first

  //from EZbutton library to keep count of buttons count. Works great!
  unsigned long modeButtonCount = modeButton.getCount();
  unsigned long buttonJoystickCount = buttonJoystick.getCount();
  unsigned long buttonForwardCount = buttonForward.getCount();
  unsigned long buttonBackwardCount = buttonBackward.getCount();

  //prepare to capture data to pass wirelessly using data for identifier.
  myRadio.write(&data, sizeof(data));

  data.joyposX = analogRead(joyX);
  data.joyposY = analogRead(joyY);
  data.potValue1 = analogRead(readPot1);
  data.potValue2 = analogRead(readPot2);

//Serial.println(data.joyposX);
Serial.println(data.joyposY);
  //for some reason released works better then IsPressed
  if (modeButton.isReleased()) {

    if (modeButtonCount >= 4) modeButton.resetCount();


    for (int i = 0; i < modeButtonCount; i++) {
      digitalWrite(ledModeStatusCount, LOW);
      delay(400);
      digitalWrite(ledModeStatusCount, HIGH);
      delay(400);
      digitalWrite(ledModeStatusCount, LOW);
    }
    data.modeCount = modeButtonCount;
  }

}
