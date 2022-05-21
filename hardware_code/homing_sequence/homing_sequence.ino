/**
 * Basic homing script for inverted pendulum.
 * Homing sequence is carried out while "go" switch is pressed and held.
 * Cart travels to left limit switch, right limit switch, and finally the middle of the rail.
 * If "go" switch is released and repressed, homing sequence will restart.
 * 
 * @author Will Martin
 */

// stepper pins
#define driverPUL 9 // pulse pin
#define driverDIR 8 // direction pin

// switch pins
#define goButtonPin 7
#define leftButtonPin 6
#define rightButtonPin 5

// switch states
int goButtonState = 0;
int leftButtonState = 0;
int rightButtonState = 0;

// calculate linear distance per step
const double motorStepsPerRev = 200;
const double micoStepSetting = 2;
const double stepsPerRev = motorStepsPerRev * micoStepSetting;
const double pulleyDiameter = .025;
const double distancePerStep = (0.5)*((1/stepsPerRev)*3.14*pulleyDiameter);

// stepper pulse
bool highlow = true;

// stepper direction
bool cw = true;

// homing booleans
bool homing = true;
bool leftHome = true;
bool rightHome = true;

// homing position values
double left = 0.0;
double right = 0.0;

// current relative position
double x = 0.0;

void setup() {
  Serial.begin(9600);

  // setup stepper
  pinMode (driverPUL, OUTPUT);
  pinMode (driverDIR, OUTPUT);
  digitalWrite(driverDIR, LOW); // LOW=CW

  // setup switches
  pinMode(goButtonPin, INPUT);
  pinMode(leftButtonPin, INPUT);
  pinMode(rightButtonPin, INPUT);
}

void loop() {
  // get switch states
  goButtonState = digitalRead(goButtonPin);
  leftButtonState = digitalRead(leftButtonPin);
  rightButtonState = digitalRead(rightButtonPin);
  
  if (goButtonState == HIGH) {
    
    if (homing) {

      // drive motor
      if (highlow) {
          digitalWrite(driverPUL, HIGH);
          highlow = false;
          delay(1);
      } else {
          digitalWrite(driverPUL, LOW);
          highlow = true;
          delay(1);
      }

      // update current position
      if (cw) {
        x += distancePerStep;
      } else {
        x -= distancePerStep;
      }

      // left and right limit switches
      // record relative position
      // switch direction on stepper motor
      if (leftButtonState == HIGH) {
        left = x;
        digitalWrite(driverDIR, HIGH);
        leftHome = false;
        cw = false;
      }
      if (rightButtonState == HIGH) {
        right = x;
        digitalWrite(driverDIR, LOW);
        rightHome = false;
        cw = true;
      }

      // check to see if both limit switches have been toggled and at middle of rail
      if ((leftHome == false) && (rightHome == false) && (x > (left+right)/2)) {
        homing = false;
      }
    } 
    
  } else {
    // prepare to restart homing next time go button is pressed
    homing = true;
    leftHome = true;
    rightHome = true;
  }
  
}
