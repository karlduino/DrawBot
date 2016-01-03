// Drawbot

#include <Encoder.h>
#include <PID_v1.h>
#include <BricktronicsMotor.h>
#include <BricktronicsButton.h>
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <BricktronicsShield.h>

#define MAXSPEED 170
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

int time=200;
int speed=150;
int turn_time = 25;
bool prev_button_pressed = false;
bool button_pressed;

void loop()
{
  speed = limit_value(speed + random(-50, 50), MINSPEED, MAXSPEED);
  forward(random(5, 50), speed);

  time = limit_value(time + random(-40, 40), 50, 500);
  left(time, speed);

  forward(random(5, 50), speed);

  time = limit_value(time + random(-40, 40), 50, 500);
  right(time, speed);
}

void bumpers_hit()
{
  rightmotor.setFixedDrive(MAXSPEED*0.8);
  leftmotor.setFixedDrive(MAXSPEED*0.8);
  delay(500);
  if(random(0, 1) < 0.5) {
    rightmotor.setFixedDrive(MAXSPEED*0.8);
    leftmotor.setFixedDrive(-MAXSPEED*0.8);
  }
  else {
    rightmotor.setFixedDrive(MAXSPEED*0.8);
    leftmotor.setFixedDrive(-MAXSPEED*0.8);
  }
  delay(1250);
}

void stop()
{
  rightmotor.setFixedDrive(0);
  leftmotor.setFixedDrive(0);
}

void backward(int amount, int speed)
{
  rightmotor.setFixedDrive(speed);
  leftmotor.setFixedDrive(speed);
  look_for_button(amount);
  stop();
}

void forward(int amount, int speed)
{
  rightmotor.setFixedDrive(-speed);
  leftmotor.setFixedDrive(-speed);
  look_for_button(amount);
  stop();
}


void right(int amount, int speed)
{
  rightmotor.setFixedDrive(speed);
  leftmotor.setFixedDrive(-speed);
  look_for_button(amount);
  stop();
}

void left(int amount, int speed)
{
  rightmotor.setFixedDrive(-speed);
  leftmotor.setFixedDrive(speed);
  look_for_button(amount);
  stop();
}

void look_for_button(int amount)
{
  for(int i=0; i<amount; i++) {
      if(button.isPressed()) {
        bumpers_hit();
        return;
      }
      delay(10);
  }
}

int limit_value(int number, int low, int high)
{
  if(number < low) return low;
  if(number > high) return high;
  return number;
}

