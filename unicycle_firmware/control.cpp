#include "stdinc.h"

void setupControl()
{
  controlState.active = false;
  actuator.disabled = false;

  controlPara.iPartSpeed = 0.8f;
  controlPara.pPartSpeed = 10.0f;
  controlPara.pPartPitch = 2.0;
  controlPara.dPartPitch = 1.0;
  controlPara.pitchWP = 5.5f;
  controlPara.speedFilter = 110.0f;

  controlPara.iPartSpeedRoll = 20.0f;
  controlPara.pPartSpeedRoll = 5.0f;
  controlPara.pPartRoll = 3.0f;
  controlPara.dPartRoll = 0.4f;
  controlPara.rollWP = -3.7f;
  controlPara.speedRollFilter = 110.0f;
}

void loopControl()
{
  float newTargetSpeed = 0;
  if(joystickTimeout) newTargetSpeed += (joystickReport.y  / 127.0f - 1.0f) * -5.0f;
  if (newTargetSpeed > controlState.targetSpeed1) controlState.targetSpeed1 += 0.0025f;
  if (newTargetSpeed < controlState.targetSpeed1) controlState.targetSpeed1 -= 0.0025f;
  controlState.speedFilt1 = PT1(motor1.shaft_velocity, controlState.speedFilt1, controlPara.speedFilter);
  float speedDev = controlState.targetSpeed1 - motor1.shaft_velocity;
  controlState.targetPitchI += speedDev * controlPara.iPartSpeed / 100.0f;
  speedDev = controlState.targetSpeed1 - controlState.speedFilt1;
  controlState.targetPitch = controlState.targetPitchI + speedDev * controlPara.pPartSpeed / 10.0f;
  static float pitchDevLast = 0.0f;
  float pitchDev = controlPara.pitchWP + controlState.targetPitch - sensorData.pitchAngle;
  controlState.dynamicTorqueFactor1 = 1.0f / LIMIT(1.0f - (fabs(controlState.speedFilt1) / 40.0f), 0.25f, 1.0f);
  actuator.torque1 = pitchDev * controlPara.pPartPitch - (pitchDev - pitchDevLast) * controlPara.dPartPitch;
  actuator.torque1 *= controlState.dynamicTorqueFactor1;

  controlState.targetSpeed2 = 0;
  controlState.speedFilt2 = PT1(motor2.shaft_velocity, controlState.speedFilt2, controlPara.speedRollFilter);
  speedDev = controlState.targetSpeed2 - controlState.speedFilt2;
  controlState.targetRollI -= speedDev * controlPara.iPartSpeedRoll / 500000.0f;
  controlState.targetRoll = controlState.targetRollI - speedDev * controlPara.pPartSpeedRoll / 100.0f;
  static float rollDevLast = 0.0f;
  float rollDev = controlPara.rollWP + controlState.targetRoll - sensorData.rollAngle;
  controlState.dynamicTorqueFactor2 = 1.0f / LIMIT(1.0f - (fabs(controlState.speedFilt2) / 40.0f), 0.25f, 1.0f);
  actuator.torque2 = -rollDev * controlPara.pPartRoll + (pitchDev - pitchDevLast) * controlPara.dPartRoll;
  actuator.torque2 *= controlState.dynamicTorqueFactor2;

  if (!controlState.active && fabs(actuator.torque2) < 0.05f) controlState.active = true;
  if (fabs(sensorData.rollAngle - controlPara.rollWP) > 10.0f) controlState.active = false;
  if (fabs(sensorData.pitchAngle) > 45.0f) controlState.active = false;
  if (!controlState.active)
  {
    actuator.torque2 = 0;
    actuator.torque1 = 0;
    controlState.targetPitchI = 0;
    controlState.targetRollI = 0;
  }
}
