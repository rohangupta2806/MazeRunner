
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
  ledcAttachPin(A1, pwmChanLF);
  ledcSetup(pwmChanLB, freq, res);
  ledcAttachPin(A0, pwmChanLB);
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
void loop()
{ // main logic loop that runs the car
  // each iteration of loop checks all distances
  distanceX = takeDataX();
  distanceY = takeDataY();
  distanceZ = takeDataZ();

  if (count == 0)
  {
    // go forward a bit
    // turn right, go forward a bit, find wall
    delay(10000);
    prevDistanceX = distanceX;
    prevDistanceY = distanceY;
    prevDistanceZ = distanceZ;
  }
  // Check the absolute distance to the wall
  if (distanceY < 200)
  {
    // Check the right wall distance
    if (distanceX < 100)
    {
      // Turn left
      turn(-90);
      // Go straight for 1 second before taking more data
      straight(1000);
      // Reset the prevDistance by retaking the data
    }
    else
    {
      if (distanceZ > distanceX)
      {
        // Turn right, move forward a bit, then turn left
        turn(90);
        straight(500);
        turn(-90);
        // Reset the prevDistance
        prevDistanceX = takeDataX();
        prevDistanceY = takeDataY();
        prevDistanceZ = takeDataZ();
      }
      else
      {
        // Turn left
        (-90);
        // Go straight for 1 second before taking more data
        straight(1000);
        // Reset the prevDistance by retaking the data
        prevDistanceX = takeDataX();
        prevDistanceY = takeDataY();
        prevDistanceZ = takeDataZ();
      }
    }
  }
  else
  {
    if (count%64 == 0)
    {
      deltaDistanceX = distanceX - prevDistanceX;
      deltaDistanceY = distanceY - prevDistanceY;
      deltaDistanceZ = distanceZ - prevDistanceZ;
      prevDistanceX = distanceX;
      prevDistanceY = distanceY;
      prevDistanceZ = distanceZ;
      // Use the deltaDistance to make a minor correction
      if (deltaDistanceX > 10)
      {
        // Turn right a bit based on the deltaDistance
        turn((int)(-deltaDistanceX/4));
      }
      else if (deltaDistanceZ > 10)
      {
        // Turn left a bit based on the deltaDistance
        turn((int)(deltaDistanceZ/4));
      }
      else
      {
        // Go straight
        ledcWrite(pwmChanLF, 175);
        ledcWrite(pwmChanRF, 175);
      }
    }
    count++;
    ledcWrite(pwmChanLF, 175);
    ledcWrite(pwmChanRF, 175);
  }
}

void straight(int time) {
  ledcWrite(pwmChanLF, 175);
  ledcWrite(pwmChanRF, 175);
  delay(time);
  ledcWrite(pwmChanLF, 0);
  ledcWrite(pwmChanRF, 0);
}

void back(int time) {
  ledcWrite(pwmChanLB, 175);
  ledcWrite(pwmChanRB, 175);
  delay(time);
  ledcWrite(pwmChanLB, 0);
  ledcWrite(pwmChanRB, 0);
}

void turn(int degrees) {
  // first, stop all wheel motion
  ledcWrite(pwmChanLF, 0);
  ledcWrite(pwmChanLB, 0);
  ledcWrite(pwmChanRF, 0);
  ledcWrite(pwmChanRB, 0);

  delay(1000); // wait for wheels to stop turning (1s)

  // calculate duration of turn based on angle
  int dur90 = 1500;
  float ratio = (abs(degrees) / 90) * dur90;
  int turndur = (int)ratio;


  if (degrees > 0) {
    // turn right
    ledcWrite(pwmChanLF, 255);
    ledcWrite(pwmChanRB, 255);
    delay(turndur);
    // stop turn
    ledcWrite(pwmChanLF, 0);
    ledcWrite(pwmChanRB, 0);

  } else if (degrees < 0) {
    // turn left
    ledcWrite(pwmChanRF, 255);
    ledcWrite(pwmChanLB, 255);
    delay(turndur);
    // stop turn
    ledcWrite(pwmChanRF, 0);
    ledcWrite(pwmChanLB, 0);

  } else {
    // do nothing
    return;
  }
}

long takeDataX(){
  digitalWrite(pingPinX, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPinX, LOW);
  long durationX = pulseIn(echoPinX, HIGH);
  long distanceX = durationX * 1.e-6 * c / 2;
  return distanceX;
}

long takeDataY(){
  digitalWrite(pingPinY, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPinY, LOW);
  long durationY = pulseIn(echoPinY, HIGH);
  long distanceY = durationY * 1.e-6 * c / 2;
  return distanceY;
}

long takeDataZ(){
  digitalWrite(pingPinZ, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPinZ, LOW);
  long durationZ = pulseIn(echoPinZ, HIGH);
  long distanceZ = durationZ * 1.e-6 * c / 2;
  return distanceZ;
}


