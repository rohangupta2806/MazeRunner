int pwmChanLF = 0;
int pwmChanLB = 1;
int pwmChanRF = 2;
int pwmChanRB = 3;
int freq = 5000;
int res = 8;

void setup() {
  // put your setup code here, to run once:
  ledcSetup(pwmChanLF, freq, res);
  ledcAttachPin(A0, pwmChanLF);
  ledcSetup(pwmChanLB, freq, res);
  ledcAttachPin(A1, pwmChanLB);

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

    go = 1;
  }
}
