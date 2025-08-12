#include "stdinc.h"

MagneticSensorSPI sensor1 = MagneticSensorSPI(AS5048_SPI, 9);
BLDCMotor motor1 = BLDCMotor(7, 15, 30);
BLDCDriver3PWM driver1 = BLDCDriver3PWM(33, 34, 35, 43);
MagneticSensorSPI sensor2 = MagneticSensorSPI(AS5048_SPI, 10);
BLDCMotor motor2 = BLDCMotor(7, 15, 30);
BLDCDriver3PWM driver2 = BLDCDriver3PWM(36, 37, 38, 44);
SPIClass SPI2(FSPI);

void setupMotor()
{
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);

  sensor1.init(&SPI2);
  motor1.linkSensor(&sensor1);
  driver1.voltage_power_supply = 24;
  driver1.init();
  motor1.linkDriver(&driver1);
  motor1.foc_modulation = FOCModulationType::SpaceVectorPWM;
  motor1.controller = MotionControlType::torque;
  motor1.init();
  motor1.zero_electric_angle = 2.24;
  motor1.sensor_direction = Direction::CCW;
  motor1.initFOC();

  sensor2.init(&SPI2);
  motor2.linkSensor(&sensor2);
  driver2.voltage_power_supply = 24;
  driver2.init();
  motor2.linkDriver(&driver2);
  motor2.foc_modulation = FOCModulationType::SpaceVectorPWM;
  motor2.controller = MotionControlType::torque;
  motor2.init();
  motor2.zero_electric_angle = 2.36;
  motor2.sensor_direction = Direction::CW;
  motor2.initFOC();
}

void loopMotor()
{
  driver1.voltage_power_supply = sensorData.voltage;
  driver2.voltage_power_supply = sensorData.voltage;
  if (actuator.disabled)
  {
    motor1.move(0);
    motor2.move(0);
  }
  else
  {
    motor1.move(actuator.torque1);
    motor2.move(actuator.torque2);
  }
}

static uint8_t tgl = 0;
void fastLoopMotor()
{
  tgl = !tgl;
  if (tgl)
  {
    motor1.loopFOC();
  }
  else
  {
    motor2.loopFOC();
  }
}
