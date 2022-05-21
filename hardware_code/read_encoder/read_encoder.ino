/**
 * Read encoder measurements for inverted pendulum.
 * Convert encoder reading to theta value in range (-pi, pi).
 * Measure instantaneous theta dot value.
 * Calculate smoothed theta dot value using mean filter.
 * 
 * @author Will Martin
 */

// encoder pins
#define encoderPinA  3
#define encoderPinB  2

// raw encoder position
volatile int encoderPos = 0;

// state values
double theta = 0;
double thetaDot = 0;

// raw theta values
double thetaAbs = 0;
double thetaAbsPrev = 0;
double thetaDotEstimate = 0;

// time for theta dot estimate
unsigned long measurementTime = micros();

// array for theta dot mean filter
const int memorySize = 3;
double thetaDotArray[memorySize];
int memoryCnt = 0;

void setup() {
  Serial.begin(9600);
  
  // encoder
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  attachInterrupt(0, doEncoderA, CHANGE);
  attachInterrupt(1, doEncoderB, CHANGE);
}

void loop() {
  // find theta in range (-2*pi, 2*pi)
  thetaAbs = (encoderPos * ((2*3.1415) / (2048*4)));
  theta = fmod(thetaAbs, 2*3.14);

  // constrain theta to range (0, 2*pi)
  if (theta < 0) {
    theta += 2*PI;
  }

  // shift range to (-pi, pi) with correct direction
  theta = -(theta - PI);

  // get estimate of theta dot
  thetaDotEstimate = (thetaAbs-thetaAbsPrev) / ((double) (micros()-measurementTime) / 1000000);
  
  // update current measurement time and prev measurement value
  measurementTime = micros();
  thetaAbsPrev = thetaAbs;

  // add theta dot estimates to array
  if (memoryCnt == memorySize) {
    for (int i=0; i < memorySize-1; i++) {
      thetaDotArray[i] = thetaDotArray[i+1];
    }
    memoryCnt--;
  }
  thetaDotArray[memoryCnt] = thetaDotEstimate;

  // get theta dot using mean filter
  thetaDot = getAvg(memoryCnt, thetaDotArray);
  memoryCnt++;

  // print (open Serial Plotter to visualize)
  Serial.print(theta);
  Serial.print('\t');
  Serial.print(thetaDotEstimate);
  Serial.print('\t');
  Serial.println(thetaDot);
}

/** 
 *  Encoder interupt callback a.
 */
void doEncoderA() {
  if (digitalRead(encoderPinA) == digitalRead(encoderPinB)) {
    encoderPos++;
  } else {
    encoderPos--;
  }
}

/** 
 *  Encoder interupt callback b.
 */
void doEncoderB() {
  if (digitalRead(encoderPinA) != digitalRead(encoderPinB)) {
    encoderPos++;
  } else {
    encoderPos--;
  }
}

/** 
 *  Get average of array.
 *  
 *  @param cnt Index of last item to be averaged in array.
 *  @param valArray Array of values.
 *  
 *  @return average of items in array
 */
double getAvg(int cnt, double valArray[]){
  double sum = 0.0;
  for (int i=0; i < cnt+1; i++) {
    sum += valArray[i];
  }
  return sum/(cnt+1);
}
