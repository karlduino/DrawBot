// Drawbot

#include <Encoder.h>
#include <PID_v1.h>
#include <BricktronicsMotor.h>
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <BricktronicsShield.h>

#define MAXSPEED 170
#define MINSPEED 60

BricktronicsMotor leftmotor(BricktronicsShield::MOTOR_1);
BricktronicsMotor rightmotor(BricktronicsShield::MOTOR_2);

int time1 = 3000, time2 = 1000;
int lspeed1=120, rspeed1 = 140;
int lspeed2=-130, rspeed2 = 150;
int accel_steps = 20;
int accel_time = 10;

// drive at a fixed speed for a given length of time
void drive(int left_speed, int right_speed,
           int travel_time)
{
    leftmotor.setFixedDrive(left_speed);
    rightmotor.setFixedDrive(right_speed);
    delay(travel_time);
}

// accelerate from one speed to another in a fixed number of steps
void accel(int left_start, int right_start,
           int left_end, int right_end,
           int time_increment, int steps)
{
    float left_step = (float)(left_end - left_start)/(float)steps;
    float right_step = (float)(right_end - right_start)/(float)steps;

    for(int i=0; i<steps; i++) {
        leftmotor.setFixedDrive(i*left_step + left_start);
        rightmotor.setFixedDrive(i*right_step + right_start);
        delay(time_increment);
    }
    leftmotor.setFixedDrive(left_end);
    rightmotor.setFixedDrive(right_end);
}


int limit_value(int number, int low, int high)
{
  if(number < low) return low;
  if(number > high) return high;
  return number;
}

void setup()
{
    BricktronicsShield::begin();
    leftmotor.begin();
    rightmotor.begin();

    // initialize random number generator
    randomSeed(analogRead(A0));

    // accel to 1st speed
    accel(0, 0, lspeed1, rspeed1, accel_time, accel_steps);
}


void loop()
{
    // drive at 1st speed
    drive(lspeed1, rspeed1, time1);

    // accel to 2nd speed
    accel(lspeed1, rspeed1, lspeed2, rspeed2, accel_time, accel_steps);

    // drive at 2nd speed
    drive(lspeed2, rspeed2, time2);

    // accel back to 1st speed
    accel(lspeed2, rspeed2, lspeed1, rspeed1, accel_time, accel_steps);
}



