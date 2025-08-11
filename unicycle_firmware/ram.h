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

typedef struct
{
  xyzFloat acc;
  xyzFloat gyro;
  xyzFloat magnet;
  float temp;
  uint16_t taskTime[2][tt_size];
  float voltage;
  int8_t button;
} SSensorData;

typedef struct
{
  float torque;
  bool disabled;
} SActuator;

typedef struct
{
  float current;
  float detents;
  float d;
  float reverse; 
} SControlPara;

typedef struct
{
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
globl SSensorData sensorData;
globl SActuator actuator;
globl SControlPara controlPara;
globl SControlState controlState;
globl bool wifiOn;
