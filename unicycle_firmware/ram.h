#ifdef _DEFINE_RAM_VARS_
#define globl
#else
#define globl extern
#endif

enum tt
{
  ttmin,
  ttmax,
  ttsum,
  ttnum,
  ttavg,
  tt_size
};

enum charge
{
  chg_normal,
  chg_low,
  chg_critical,
  chg_empty
};

typedef struct
{
  xyzFloat acc;
  xyzFloat gyro;
  xyzFloat magnet;
  float temp;
  uint16_t taskTime[2][tt_size];
  float voltage;
  int8_t chargeState;
  float pitchAngleAcc;
  float rollAngleAcc;
  float pitchAngle;
  float rollAngle;
} SSensorData;

typedef struct
{
  float torque1;
  float torque2;
  bool disabled;
} SActuator;

typedef struct
{
  float iPartSpeed;
  float pPartSpeed;
  float pPartPitch;
  float dPartPitch;
  float pitchWP;
  float speedFilter;
  float iPartSpeedRoll;
  float pPartSpeedRoll;
  float pPartRoll;
  float dPartRoll;
  float rollWP;
  float speedRollFilter;
} SControlPara;

typedef struct
{
  float targetSpeed1;
  float speedFilt1;
  float targetSpeed2;
  float speedFilt2;
  float targetPitchI;
  float targetPitch;
  float targetRollI;
  float targetRoll;
  bool active;
  float dynamicTorqueFactor1;
  float dynamicTorqueFactor2;
  int servoSetval;
} SControlState;

typedef struct __attribute__((__packed__)) {
  uint8_t hat;
  uint8_t x;
  uint8_t y;
  uint8_t z;
  uint8_t rz;
  uint8_t brake;
  uint8_t accel;
  uint32_t buttons;
} joystick_t;

globl joystick_t joystickReport;
globl uint8_t joystickTimeout;
globl bool wifiOn;
globl uint8_t oscillate;
globl uint8_t correlate;
globl SSensorData sensorData;
globl SActuator actuator;
globl SControlPara controlPara;
globl SControlState controlState;
