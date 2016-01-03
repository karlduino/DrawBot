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

int time=200;
int speed=MAXSPEED/2;
int turn_time = 250;
bool prev_button_pressed = false;
bool button_pressed;

void loop()
{
  speed += random(-50, 50);
  if(speed > MAXSPEED) speed = MAXSPEED;
  if(speed < MINSPEED) speed = MINSPEED;

  forward(random(5, 50), speed);

  time += random(-40, 40);
  if(time < 0) time = 50;
  left(time, speed);


  forward(random(5, 50), speed);

  if(button.isPressed()) bumpers_hit();

  time += random(-40, 40);
  if(time < 0) time = 50;
  right(time, speed);

  if(button.isPressed()) bumpers_hit();

}

void bumpers_hit()
{
  backward(100, MAXSPEED*0.8);
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
        return();
      }
      delay(10);
  }
}

