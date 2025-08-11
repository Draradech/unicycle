#include "stdinc.h"

MagneticSensorSPI sensor = MagneticSensorSPI(AS5048_SPI, 10);
BLDCMotor motor = BLDCMotor(7, 15, 30);
BLDCDriver3PWM driver = BLDCDriver3PWM(34, 35, 36, 37);
SPIClass SPI2(FSPI);

void setupMotor()
{
  sensor.init(&SPI2);
  motor.linkSensor(&sensor);

  driver.voltage_power_supply = 24;
  driver.init();
  motor.linkDriver(&driver);

  motor.foc_modulation = FOCModulationType::SpaceVectorPWM;
  motor.controller = MotionControlType::torque;

  motor.init();
  motor.zero_electric_angle = 3.0;
  motor.sensor_direction = Direction::CW;
  motor.initFOC();
}

void loopMotor()
{
  if (actuator.disabled)
  {
    motor.move(0);
  }
  else
  {
    motor.move(actuator.torque);
  }
}

static uint8_t tgl = 0;
void fastLoopMotor()
{
  tgl = !tgl;
  if (tgl)
  {
    motor.loopFOC();
  }
  else
  {
    // second motor
    ets_delay_us(50);
  }
}
