#include "stdinc.h"

void setup()
{
  setupSensors();
  setupControl();
  setupMotor();
  setupUI();
  setupBLE();
  setupSystem();
}

void loop2ms()
{
  loopSensors();
  loopControl();
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
