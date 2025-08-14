#include "stdinc.h"

void setupControl()
{
  actuator.disabled = false;
  controlPara.iPartSpeed = 1.0f;
  controlPara.pPartSpeed = 10.0f;
  controlPara.pPartPitch = 1.5f;
  controlPara.dPartPitch = 0.3f;
  controlPara.iPartSpeedRoll = 6.0f;
  controlPara.pPartSpeedRoll = 3.0f;
  controlPara.pPartRoll = 3.0f;
  controlPara.dPartRoll = 0;
  controlPara.rollWP = -3.7f;
  controlState.rollActive = false;
}

void loopControl()
{
  static float targetSpeed;
  static float speedDevFilt1;
  float newTargetSpeed = 0;
  if(joystickTimeout) newTargetSpeed = (joystickReport.y  / 127.0f - 1.0f) * -5.0f;
  targetSpeed = PT1(newTargetSpeed, targetSpeed, 1000);
  float speedDev = targetSpeed - motor1.shaft_velocity;
  speedDevFilt1 = PT1(speedDev, speedDevFilt1, 100);
  controlState.targetPitchI += speedDev * controlPara.iPartSpeed / 100.0f;
  controlState.targetPitch = controlState.targetPitchI + speedDevFilt1 * controlPara.pPartSpeed / 10.0f;
  static float pitchDevLast = 0.0f;
  float pitchDev = controlState.targetPitch - sensorData.pitchAngle;
  actuator.torque1 = pitchDev * controlPara.pPartPitch - (pitchDev - pitchDevLast) * controlPara.dPartPitch;

  static float speedDevFilt2;
  float targetSpeed2 = 0;
  speedDev = targetSpeed2 - motor2.shaft_velocity;
  speedDevFilt2 = PT1(speedDev, speedDevFilt2, 100);
  controlState.targetRollI -= speedDev * controlPara.iPartSpeedRoll / 500000.0f;
  controlState.targetRoll = controlState.targetRollI - speedDevFilt2 * controlPara.pPartSpeedRoll / 100.0f;
  static float rollDevLast = 0.0f;
  float rollDev = controlPara.rollWP + controlState.targetRoll - sensorData.rollAngle;
  actuator.torque2 = -rollDev * controlPara.pPartRoll + (pitchDev - pitchDevLast) * controlPara.dPartRoll;
  if (!controlState.rollActive && fabs(actuator.torque2) < 0.05f) controlState.rollActive = true;
  if (fabs(sensorData.rollAngle - controlPara.rollWP) > 10.0f) controlState.rollActive = false;
  if (fabs(sensorData.pitchAngle) > 45.0f) controlState.rollActive = false;
  if (!controlState.rollActive)
  {
    actuator.torque2 = 0;
    actuator.torque1 = 0;
    controlState.targetPitchI = 0;
    controlState.targetRollI = 0;
  }
}
