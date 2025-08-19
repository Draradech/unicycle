#include "stdinc.h"

#define CHANNEL 0
#define PIN 15
#define RESBIT 14
#define MAXVAL (1 << RESBIT)
#define FREQUENCY 50
#define PERIODUS (1000000 / FREQUENCY)
#define CYCLES(us) ((int32_t)(us) * MAXVAL / PERIODUS)


void setupServo()
{
  ledcAttach(PIN, FREQUENCY, RESBIT);
}

static int steps;
void loopServo()
{
  if(!(steps++ == 8)) return;
  steps =  0;
  int setvalTarget = 0;
  if (joystickTimeout) setvalTarget = ((int)joystickReport.z - 127);
  float speedlim = 5.0f - fabs(motor1.shaft_velocity);
  speedlim = MAX(0.0f, speedlim) * 1.5f + 7.5f;
  if (controlState.servoSetval > 0 && fabs(motor2.shaft_velocity) > speedlim) setvalTarget = 50;
  if (controlState.servoSetval < 0 && fabs(motor2.shaft_velocity) > speedlim) setvalTarget = -50;
  if (setvalTarget > 5) controlState.servoSetval = MAX(controlState.servoSetval, 50);
  if (setvalTarget < -5) controlState.servoSetval = MIN(controlState.servoSetval, -50);
  if(setvalTarget > controlState.servoSetval) controlState.servoSetval++;
  if(setvalTarget < controlState.servoSetval) controlState.servoSetval--;
  ledcWrite(PIN, CYCLES(1487 + controlState.servoSetval));
}