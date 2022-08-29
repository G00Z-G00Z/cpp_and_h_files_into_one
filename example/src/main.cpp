#include <Arduino.h>
#include <Keypad.h>
#include <Led.h>
#include <Servo.h>
#include <LockStateMachine.h>
#include <LockStates.h>

/**************************************************/
// key pad set up
const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad lockKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
/**************************************************/

Led openLed = Led(22);
Led closeLed = Led(23);

/**************************************************/
const int servoPin = 30;
Servo doorServo;
/**************************************************/

/**************************************************/
LockContext *electronicLock = nullptr;
/**************************************************/

void setup()
{
  Serial.begin(9600);

  doorServo.attach(servoPin);

  electronicLock = new LockContext(
      &openLed,
      &closeLed,
      &doorServo,
      &lockKeypad,
      nullptr);

  electronicLock->transitionTo(new ChangePasswordState(true));
}

void loop()
{
  electronicLock->update();
}