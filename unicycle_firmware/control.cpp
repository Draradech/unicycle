#include "stdinc.h"

void setupControl()
{
  controlPara.current = 2.0f;
  controlPara.detents = 8;
  controlPara.d = 0;
  actuator.disabled = false;
  controlPara.iPartSpeed = 1.0f;
  controlPara.pPartSpeed = 1.2f;
  controlPara.pPartPitch = 1.0f;
  controlPara.dPartPitch = 0.7f;
  controlPara.iPartSpeedRoll = 4.0;
  controlPara.pPartSpeedRoll = 6.0f;
  controlPara.pPartRoll = 3.5f;
  controlPara.dPartRoll = 0;
  controlPara.rollWP = 2.7f;
  controlState.rollActive = false;
}

void loopControl()
{
  if(controlMode == 0)
  {
    if(joystickTimeout)
    {
      joystickTimeout--;
      actuator.torque1 = (joystickReport.y  / 127.0f - 1.0f) * controlPara.current;
      actuator.torque2 = (joystickReport.rz / 127.0f - 1.0f) * controlPara.current;
    }
    else
    {
      actuator.torque1 = 0;
      actuator.torque2 = 0;
    }
  }
  else if (controlMode == 1)
  {
    actuator.torque1 = controlPara.current * sin(motor1.shaft_angle * controlPara.detents) / controlPara.detents;
    actuator.torque2 = controlPara.current * sin(motor2.shaft_angle * controlPara.detents) / controlPara.detents;
  }
  else if (controlMode == 2)
  {
    actuator.torque1 = controlPara.current * sin(motor1.shaft_angle - motor2.shaft_angle);
    actuator.torque2 = controlPara.current * sin(motor2.shaft_angle - motor1.shaft_angle);
  }
  else
  {
    static float speedDevFilt1;
    actuator.torque2 = 0;
    float targetSpeed = 0;
    if(joystickTimeout) targetSpeed = (joystickReport.y  / 127.0f - 1.0f) * -5.0f;
    float speedDev = targetSpeed - motor1.shaft_velocity;
    speedDevFilt1 = PT1(speedDev, speedDevFilt1, 100);
    controlState.targetPitchI += speedDev * controlPara.iPartSpeed / 100.0f;
    controlState.targetPitch = controlState.targetPitchI + speedDevFilt1 * controlPara.pPartSpeed / 10.0f;
    static float pitchDevLast = 0.0f;
    float pitchDev = controlState.targetPitch - sensorData.pitchAngle;
    actuator.torque1 = pitchDev * controlPara.pPartPitch - (pitchDev - pitchDevLast) * controlPara.dPartPitch;

    static float speedDevFilt2;
    targetSpeed = 0;
    speedDev = targetSpeed - motor2.shaft_velocity;
    speedDevFilt2 = PT1(speedDev, speedDevFilt2, 100);
    controlState.targetRollI -= speedDev * controlPara.iPartSpeedRoll / 500000.0f;
    controlState.targetRoll = controlState.targetRollI - speedDevFilt2 * controlPara.pPartSpeedRoll / 100.0f;
    static float rollDevLast = 0.0f;
    float rollDev = controlPara.rollWP + controlState.targetRoll - sensorData.rollAngle;
    actuator.torque2 = -rollDev * controlPara.pPartRoll + (pitchDev - pitchDevLast) * controlPara.dPartRoll;
    if (!controlState.rollActive && fabs(actuator.torque2) < 0.05f) controlState.rollActive = true;
    if (fabs(sensorData.rollAngle - controlPara.rollWP) > 10.0f) controlState.rollActive = false;
    if (!controlState.rollActive)
    {
      actuator.torque2 = 0;
      controlState.targetRollI = 0;
    }
  }
}
