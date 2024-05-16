
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

long prevDistX;
long prevDistY;
long prevDistZ;
long newDistX;
long deltaDistX;
long deltaDistY;
long deltaDistZ;

int count = 0;
void loop()
{ // main logic loop that runs the car

  // initial wait
  if (count==0) {
    delay(10000);
    straightReset();
  }

  // each iteration of loop checks all distances
  distanceX = takeDataX(); // right sensor
  distanceY = takeDataY(); // forward sensor
  distanceZ = takeDataZ(); // left sensor

  // ASSUME THERE IS WALL ON RIGHT AT START
  // maybe write entrance function here

  // logic for following wall
  if (distanceX > 200)
  {
    // if gap in wall, must turn into gap
    turn(90);
    // go forward a bit to enter gap
    straight(1350);
    prevDistX = takeDataX();
    prevDistY = takeDataY();
    prevDistZ = takeDataZ();
  }
  else if (distanceY < 150)
  {
    // there is a corner in front of us
    turn(-90);
    prevDistX = takeDataX();
    prevDistY = takeDataY();
    prevDistZ = takeDataZ();
  }
  else
  {
    if(count%64 == 0)
    {
      // Straightening code
      deltaDistX = distanceX - prevDistX;
      deltaDistY = distanceY - prevDistY;
      deltaDistZ = distanceZ - prevDistZ;
      prevDistX = distanceX;
      prevDistY = distanceY;
      prevDistZ = distanceZ;
      // Use the deltaDistance to make a minor correction
      if (deltaDistX > 10)
      {
        // Turn left a bit based on the deltaDistance
        turn((int)(-deltaDistX/4));
        // Go straight
        ledcWrite(pwmChanLF, 175);
        ledcWrite(pwmChanRF, 175);
      }
      else if (deltaDistZ > 10)
      {
        // Turn right a bit based on the deltaDistance
        turn((int)(deltaDistZ/4));
        // Go straight
        ledcWrite(pwmChanLF, 175);
        ledcWrite(pwmChanRF, 175);
      }
      else
      {
        // Go straight
        ledcWrite(pwmChanLF, 175);
        ledcWrite(pwmChanRF, 175);
      }
    }
  }
  // default to go straight after making necessary turns
  ledcWrite(pwmChanLF, 175);
  ledcWrite(pwmChanRF, 175);
  count++;
}

void straightReset() {
  prevDistX = 0;
  newDistX = 0;
  deltaDistX = 0;
}

void straight(int time) {
  ledcWrite(pwmChanLF, 175);
  ledcWrite(pwmChanRF, 175);
  delay(time);
  ledcWrite(pwmChanLF, 0);
  ledcWrite(pwmChanRF, 0);
}

void back(int time) {
  ledcWrite(pwmChanLF, 0);
  ledcWrite(pwmChanRF, 0);
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

  delay(100); // wait for wheels to stop turning (0.1s)

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

  long totalDistanceX = 0;

  for (int i = 0; i < 5; i++) {
    digitalWrite(pingPinX, HIGH);
    delayMicroseconds(10);
    digitalWrite(pingPinX, LOW);
    long durationX = pulseIn(echoPinX, HIGH);
    long distanceX = durationX * 1.e-6 * c / 2;
    totalDistanceX += distanceX;
  }

  long averageDistanceX = totalDistanceX / 5;
  return averageDistanceX;
}

long takeDataY(){

  long totalDistanceY = 0;

  for (int i = 0; i < 5; i++) {
    digitalWrite(pingPinY, HIGH);
    delayMicroseconds(10);
    digitalWrite(pingPinY, LOW);
    long durationY = pulseIn(echoPinY, HIGH);
    long distanceY = durationY * 1.e-6 * c / 2;
    totalDistanceY += distanceY;
  }

  long averageDistanceY = totalDistanceY / 5;
  return averageDistanceY;
}

long takeDataZ(){

  long totalDistanceZ = 0;

  for (int i = 0; i < 5; i++) {
    digitalWrite(pingPinZ, HIGH);
    delayMicroseconds(10);
    digitalWrite(pingPinZ, LOW);
    long durationZ = pulseIn(echoPinZ, HIGH);
    long distanceZ = durationZ * 1.e-6 * c / 2;
    totalDistanceZ += distanceZ;
  }

  long averageDistanceZ = totalDistanceZ / 5;
  return averageDistanceZ;
}


