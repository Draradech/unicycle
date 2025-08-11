#include "stdinc.h"

#define CS_PIN 8
#define MISO_PIN 9
#define MOSI_PIN 7
#define SCK_PIN 6
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
  gIMU.setTempDLPF(ICM20948_DLPF_6);
}

float stage1 = 22.2f;
void loopSensors()
{
  float volt = (analogRead(1) + 270.0f) / 112.0f;
  stage1 = PT1(volt, stage1, 500);
  sensorData.voltage = PT1(stage1, sensorData.voltage, 500);
  gIMU.readSensor();
  gIMU.getGValues(&sensorData.acc);
  gIMU.getGyrValues(&sensorData.gyro);
  gIMU.getMagValues(&sensorData.magnet);
  sensorData.temp = gIMU.getTemperature();
}
