/**
 * Script to send acceleration signal to cart.
 * Calculates cart acceleration from sin curve.
 * Converts cart acceleration to angular acceleration of stepper motor.
 * Continously integrates angular acceleration to find instantaneous angular velocity.
 * Finds stepper timings from angular velocity.
 * 
 * @author Will Martin
 */
 
 // stepper pins
#define driverPUL 9 // pulse pin
#define driverDIR 8 // direction pin

// switch pins
#define goButtonPin 7

// switch state
int goButtonState = 0;

// steps per revolution
// 1.8 deg per step = 200 steps per revolution
const double motorStepsPerRev = 200;
const double micoStepSetting = 2;
const double stepsPerRev = motorStepsPerRev * micoStepSetting;
const double pulleyDiameter = .025;

// pendulum system state values
double xDDot = 0.0;

// stepper motor state values
double angularVel = 0.0;
double prevAngularVel = 0.0;
double angularAccel = 0.0;

// stepper motor delay time
double secPerStep = 0.0;

// timings
unsigned long motorUpdateTime = micros();
unsigned long sinSignalUpdateTime = micros();
unsigned long integrationTime = micros();

// input acceleration signal parameters
const double sinSignalUpdateFrequency = 1000; // Hz
const double sinAmplitude = 0.5;
const double sinFrequency = 5;

// stepper pulse
bool highlow = true;

// stepper direction
bool cw = true;

void setup() {
  Serial.begin(9600);

  // setup stepper
  pinMode (driverPUL, OUTPUT);
  pinMode (driverDIR, OUTPUT);
  digitalWrite(driverDIR, LOW); // LOW=CW

  // setup switch
  pinMode(goButtonPin, INPUT);
}

void loop() {
  // get switch state
  goButtonState = digitalRead(goButtonPin);
  
  if (goButtonState==HIGH) {

    // drive motor
    if (microsecondsToSeconds((double) (micros()-motorUpdateTime)) >= secPerStep) {
      motorUpdateTime = micros();
      
      if (highlow) {
          digitalWrite(driverPUL, HIGH);
          highlow = false;
      } else {
          digitalWrite(driverPUL, LOW);
          highlow = true;
      } 
      
    }

    // calculate time delays for stepper motor
    if (microsecondsToSeconds((double) (micros()-sinSignalUpdateTime)) >= (1/sinSignalUpdateFrequency)) {
      sinSignalUpdateTime = micros();

      // acceleration of cart based on sin wave
      xDDot = sinAmplitude * sin(sinFrequency * microsecondsToSeconds((double) micros()));

      // angular acceleration of stepper motor
      angularAccel = xDDot / (pulleyDiameter/2);

      // continously integrate angular acceleration to get angular velocity
      prevAngularVel = angularVel;
      angularVel += angularAccel*(microsecondsToSeconds((double) (micros()-integrationTime)));
      integrationTime = micros();

      // convert angular velocity to delay time between steps
      secPerStep = 1/(2*abs(angularVel)*(stepsPerRev/(2*PI)));

      // switch direction or rotation if velocity changes sign
      if ((prevAngularVel*angularVel) < 0) {
        if (cw) {
            digitalWrite(driverDIR, HIGH);
            cw = false;
        } else {
            digitalWrite(driverDIR, LOW);
            cw = true;
        }
      }
      
    }
    
  } else {
    // velocity of motor returns to zero
    angularVel = 0.0;
  }
}

/**
 * Convert seconds to microseconds
 */
double secondsToMicroseconds (double seconds) {
  return seconds * 1000000;
}

/**
 * Convert microseconds to seconds
 */
double microsecondsToSeconds(double microseconds) {
  return microseconds / 1000000;
}
