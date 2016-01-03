// Drawbot

#include <Encoder.h>
#include <PID_v1.h>
#include <BricktronicsMotor.h>
#include <BricktronicsButton.h>
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <BricktronicsShield.h>

#define MAXSPEED 200
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

int time=2000;
int speed=MAXSPEED/2;
int turn_time = 2500;
bool prev_button_pressed = false;
bool button_pressed;

void loop()
{
  speed += random(-50, 50);
  if(speed > MAXSPEED) speed = MAXSPEED;
  if(speed < MINSPEED) speed = MINSPEED;

  forward(random(50, 500), speed);

  if(button.isPressed()) bumpers_hit();

  time += random(-400, 400);
  if(time < 0) time = 500;
  left(time, speed);

  if(button.isPressed()) bumpers_hit();

  forward(random(50, 500), speed);

  if(button.isPressed()) bumpers_hit();

  time += random(-400, 400);
  if(time < 0) time = 500;
  right(time, speed);

  if(button.isPressed()) bumpers_hit();

}

void bumpers_hit()
{
  backward(1000, MAXSPEED*0.8);
  if(random(0, 1) < 0.5)
    right(turn_time, MAXSPEED*0.8);
  else
    left(turn_time, MAXSPEED*0.8);
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
  delay(amount);
  stop();
}

void forward(int amount, int speed)
{
  rightmotor.setFixedDrive(-speed);
  leftmotor.setFixedDrive(-speed);
  delay(amount);
  stop();
}


void right(int amount, int speed)
{
  rightmotor.setFixedDrive(speed);
  leftmotor.setFixedDrive(-speed);
  delay(amount);
  stop();
}

void left(int amount, int speed)
{
  rightmotor.setFixedDrive(-speed);
  leftmotor.setFixedDrive(speed);
  delay(amount);
  stop();
}
