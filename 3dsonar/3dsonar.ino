const int pingPinY = 15;
const int echoPinY = 32;
const int pingPinX = 13;
const int echoPinX = 12;
const int pingPinZ = 27;
const int echoPinZ = 33;

const float c = 3.43e5;

long durationY;
long durationX;
long durationZ;

void setup() {
  // put your setup code here, to run once:
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
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(pingPinY, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPinY, LOW);
  durationY = pulseIn(echoPinY, HIGH);
  Serial.print(durationY * 1.e-6 * c / 2);
  Serial.println(" mm in Y");

  digitalWrite(pingPinX, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPinX, LOW);
  durationX = pulseIn(echoPinX, HIGH);
  Serial.print(durationX * 1.e-6 * c / 2);
  Serial.println(" mm in X");

  digitalWrite(pingPinZ, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPinZ, LOW);
  durationZ = pulseIn(echoPinZ, HIGH);
  Serial.print(durationZ * 1.e-6 * c / 2);
  Serial.println(" mm in Z");

  delay(1000);
}
