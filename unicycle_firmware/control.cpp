#include "stdinc.h"

void setupControl()
{
  controlPara.current = 0.1f;
  controlPara.detents = 16;
  controlPara.d = 0;
  actuator.disabled = false;
}

void loopControl()
{
  if(joystickTimeout)
  {
    joystickTimeout--;
    actuator.torque = ((float)joystickReport.y - 127) / 127 * 0.8f;
  }
  else if (controlPara.detents > 0.01f)
  {
    actuator.torque = controlPara.current * sin(motor.shaft_angle * controlPara.detents) + motor.shaft_velocity / 500 * controlPara.d;
  }
  else
  {
    actuator.torque = controlPara.current * (controlPara.reverse > 0.01f ? -1 : 1);
  }
}
