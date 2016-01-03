// Drawbot

#include <Encoder.h>
#include <PID_v1.h>
#include <BricktronicsMotor.h>
#include <BricktronicsButton.h>
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <BricktronicsShield.h>

#define MAXSPEED 150
#define MINSPEED 60

BricktronicsMotor leftmotor(BricktronicsShield::MOTOR_1);
BricktronicsMotor rightmotor(BricktronicsShield::MOTOR_2);

BricktronicsButton button(BricktronicsShield::SENSOR_2);


void setup()
{
  BricktronicsShield::begin();  
  leftmotor.begin();
  rightmotor.begin();
  button.begin();

  // initialize random number generator
  randomSeed(analogRead(A0));
}

long timeout = -1;
bool prev_button_pressed = false;
bool button_pressed;

void loop()
{
  if(timeout >= 0 && timeout < millis())
  {
    timeout_expired();
  }

  button_pressed = button.isPressed();
  if(button_pressed && button_pressed != prev_button_pressed) {
    bumpers_hit();
  }
  prev_button_pressed = button_pressed;
}

void bumpers_hit()
{
  int right_speed = rightmotor.getFixedDrive() * -0.9;
  int left_speed = leftmotor.getFixedDrive() * -0.9;

  rightmotor.setFixedDrive(right_speed);
  leftmotor.setFixedDrive(left_speed);

  delay(500);
  if(abs(left_speed) < MINSPEED && abs(right_speed) < MINSPEED) {
    timeout_expired(); // gotten too slow; reset
  }
}

void timeout_expired()
{
  int right_speed = 0;
  int left_speed = 0;
  while(abs(left_speed) < MINSPEED && abs(right_speed) < MINSPEED) {
    left_speed = random(-MAXSPEED, MAXSPEED);
    right_speed = random(-MAXSPEED, MAXSPEED);
  }
  rightmotor.setFixedDrive(right_speed);
  leftmotor.setFixedDrive(left_speed);
  timeout = millis() + random(1,10)*1000;
}

