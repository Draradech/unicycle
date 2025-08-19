#include "stdinc.h"

void setup()
{
  setupSensors();
  setupControl();
  setupMotor();
  setupUI();
  setupBLE();
  setupServo();
  setupSystem();
}

void loop2ms()
{
  loopSensors();
  loopControl();
  loopServo();
  loopMotor();
  loopUI();
  loopInputOutput();
  loopOta();
}

void loop250us()
{
  fastLoopMotor();
}

void loop()
{
  loopBLE();
}
