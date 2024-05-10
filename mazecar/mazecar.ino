
// set up PWM for leftside and rightside wheels
int pwmChanLF = 0;
int pwmChanLB = 1;
int pwmChanRF = 2;
int pwmChanRB = 3;
int freq = 5000;
int res = 8;

const float c = 3.43e5; // the speed of sounds (m/s)

// set up X, Y, Z ultrasonic sensors
const int pingPinY = 15;
const int echoPinY = 32;
const int pingPinX = 13;
const int echoPinX = 12;
const int pingPinZ = 27;
const int echoPinZ = 33;
// Y is forwards facing sensor
// X should be first right wall
// Z shouldn't be used

void setup() {  // set up sensors and begin
  // set up L and R wheel PWM for forward and back
  ledcSetup(pwmChanLF, freq, res);
  ledcAttachPin(A0, pwmChanLF);
  ledcSetup(pwmChanLB, freq, res);
  ledcAttachPin(A1, pwmChanLB);
  ledcSetup(pwmChanRF, freq, res);
  ledcAttachPin(21, pwmChanRF);
  ledcSetup(pwmChanRB, freq, res);
  ledcAttachPin(A5, pwmChanRB);

  // set up three ultrasonic sensors
  pinMode(pingPinY, OUTPUT);
  pinMode(echoPinY, INPUT);
  digitalWrite(pingPinY, LOW);
  pinMode(pingPinX, OUTPUT);
  pinMode(echoPinX, INPUT);
  digitalWrite(pingPinX, LOW);
  pinMode(pingPinZ, OUTPUT);
  pinMode(echoPinZ, INPUT);
  digitalWrite(pingPinZ, LOW);

  Serial.begin(115200);
  Serial.println("STARTING");

}

long durationY;
long durationX;
long durationZ;
long distanceY;
long distanceX;
long distanceZ;
long prevDistanceY;
long prevDistanceX;
long prevDistanceZ;
long deltaDistanceY;
long deltaDistanceX;
long deltaDistanceZ;

int count = 0;
void loop() { // main logic loop that runs the car
  // each iteration of loop checks all distances
  digitalWrite(pingPinY, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPinY, LOW);
  durationY = pulseIn(echoPinY, HIGH);
  distanceY = durationY * 1.e-6 * c / 2;
  //Serial.print(durationY * 1.e-6 * c / 2);
  //Serial.println(" mm in Y");

  digitalWrite(pingPinX, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPinX, LOW);
  durationX = pulseIn(echoPinX, HIGH);
  distanceX = durationX * 1.e-6 * c / 2;
  //Serial.print(durationX * 1.e-6 * c / 2);
  //Serial.println(" mm in X");

  digitalWrite(pingPinZ, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPinZ, LOW);
  durationZ = pulseIn(echoPinZ, HIGH);
  distanceZ = durationZ * 1.e-6 * c / 2;
  //Serial.print(durationZ * 1.e-6 * c / 2);
  //Serial.println(" mm in Z");

  // check forwards distance,
  int block = 0;

  // check left wall distance
  int gap = 0;
  if (count == 0)
  {
    // go forward a bit
    // turn right, go forward a bit, find wall
    prevDistanceX = distanceX;
    prevDistanceY = distanceY;
    prevDistanceZ = distanceZ;
  }

  if (count%512 == 0)
  {
    deltaDistanceX = distanceX - prevDistanceX;
    deltaDistanceY = distanceY - prevDistanceY;
    deltaDistanceZ = distanceZ - prevDistanceZ;
    prevDistanceX = distanceX;
    prevDistanceY = distanceY;
    prevDistanceZ = distanceZ;
    // Use the deltaDistance to make a minor correction
    if (deltaDistanceX > 50)
    {
      // Turn right a bit based on the deltaDistance
      turn((int)(deltaDistanceX/10));
    }
    else if (deltaDistanceZ > 50)
    {
      // Turn left a bit based on the deltaDistance
      turn((int)(-deltaDistanceZ/10));
    }
    else
    {
      // Do nothing
    }
  }
  count++;
}

void turn(int degrees) { 
  // first, stop all wheel motion
  ledcWrite(pwmChanLF, 0);
  ledcWrite(pwmChanLB, 0);
  ledcWrite(pwmChanRF, 0);
  ledcWrite(pwmChanRB, 0);

  // calculate duration of turn based on angle
  int dur90 = 10000; 
  float ratio = (abs(degrees) / 90) * dur90;
  int turndur = (int)ratio;


  if (degrees > 0) {
    // turn right
    ledcWrite(pwmChanLF, 255);
    ledcWrite(pwmChanRB, 150);
    delay(turndur);
    // stop turn
    ledcWrite(pwmChanLF, 0);
    ledcWrite(pwmChanRB, 0);

  } else if (degrees < 0) {
    // turn left
    ledcWrite(pwmChanRF, 150);
    ledcWrite(pwmChanLB, 150);
    delay(turndur);
    // stop turn
    ledcWrite(pwmChanRF, 0);
    ledcWrite(pwmChanLB, 0);

  } else {
    // do nothing
    return;
  }





