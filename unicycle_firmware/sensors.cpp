#include "stdinc.h"

#define CS_PIN 4
#define MOSI_PIN 5
#define SCK_PIN 6
#define MISO_PIN 7
bool spi = true;

SPIClass SPI3(HSPI);
ICM20948_WE gIMU = ICM20948_WE(&SPI3, CS_PIN, MOSI_PIN, MISO_PIN, SCK_PIN, spi);

void setupSensors()
{
  analogSetAttenuation(ADC_11db);
  sensorData.voltage=22.2f;
  gIMU.init();
  gIMU.initMagnetometer();
  gIMU.setAccDLPF(ICM20948_DLPF_7);
  gIMU.setAccSampleRateDivider(0);
  gIMU.setGyrDLPF(ICM20948_DLPF_7);
  gIMU.setGyrSampleRateDivider(0);
  gIMU.setGyrRange(ICM20948_GYRO_RANGE_500);
  gIMU.setTempDLPF(ICM20948_DLPF_6);
}

float stage1 = 22.2f;
void loopSensors()
{
  float volt = (analogRead(1) + 61.4f) / 108.6f;
  stage1 = PT1(volt, stage1, 100);
  sensorData.voltage = PT1(stage1, sensorData.voltage, 100);
  gIMU.readSensor();
  gIMU.getGValues(&sensorData.acc);
  gIMU.getGyrValues(&sensorData.gyro);
  gIMU.getMagValues(&sensorData.magnet);
  sensorData.temp = gIMU.getTemperature();
  float pitchAcc = atan2(sensorData.acc.x, -sensorData.acc.z) * 180.0f / 3.1416f;
  sensorData.pitchAngleAcc = PT1(pitchAcc, sensorData.pitchAngleAcc, 1000);
  float rollAcc = atan2(sensorData.acc.y, -sensorData.acc.z) * 180.0f / 3.1416f;
  sensorData.rollAngleAcc = PT1(rollAcc, sensorData.rollAngleAcc, 1000);
  sensorData.pitchAngle += sensorData.gyro.y * LOOP_S;
  sensorData.rollAngle -= sensorData.gyro.x * LOOP_S;
  sensorData.pitchAngle = PT1(sensorData.pitchAngleAcc, sensorData.pitchAngle, 1000);
  sensorData.rollAngle = PT1(sensorData.rollAngleAcc, sensorData.rollAngle, 1000);
}
