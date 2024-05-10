int pwmChanLF = 0;
int pwmChanLB = 1;
int pwmChanRF = 2;
int pwmChanRB = 3;
int freq = 5000;
int res = 8;

void setup() {
  // put your setup code here, to run once:
  ledcSetup(pwmChanLF, freq, res);
  ledcAttachPin(A1, pwmChanLF);
  ledcSetup(pwmChanLB, freq, res);
  ledcAttachPin(A0, pwmChanLB);

  ledcSetup(pwmChanRF, freq, res);
  ledcAttachPin(21, pwmChanRF);
  ledcSetup(pwmChanRB, freq, res);
  ledcAttachPin(A5, pwmChanRB);

  Serial.begin(115200);

}

int go = 0;
void loop() {
  // put your main code here, to run repeatedly:
  if (go == 0) {
    delay(5000);
    straight(10000);
    delay(5000);
    turn(90);
    delay(5000);
    straight(10000);
    go = 1;
  }
}

void test4motors() {
  ledcWrite(pwmChanLB, 150);
  delay(5000);
  ledcWrite(pwmChanLB, 0);
  delay(5000);
  ledcWrite(pwmChanLF, 150);
  delay(5000);
  ledcWrite(pwmChanLF, 0);
  delay(5000);
  ledcWrite(pwmChanRB, 150);
  delay(5000);
  ledcWrite(pwmChanRB, 0);
  delay(5000);
  ledcWrite(pwmChanRF, 150);
  delay(5000);
  ledcWrite(pwmChanRF, 0);
  return;
}

void straight(int time) {
  ledcWrite(pwmChanLF, 255);
  ledcWrite(pwmChanRF, 255);
  delay(time);
  ledcWrite(pwmChanLF, 0);
  ledcWrite(pwmChanRF, 0);
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

}
