#include <Servo.h>

Servo gate;

const int TRIG1 = 2;
const int ECHO1 = 3;
const int TRIG2 = 4;
const int ECHO2 = 5;
const int SERVO_PIN = 6;
const int BUZZER = 7;
const int LED_PIN = 8;

bool gateClosed = false;
bool trainAtExit = false;

int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration =
      pulseIn(echoPin, HIGH, 30000);

  if (duration == 0) {
    return 999;
  }

  return duration * 0.034 / 2;
}

void setup() {
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);

  pinMode(BUZZER, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  gate.attach(SERVO_PIN);

  // First code: Gate close and open test
  gate.write(0);
  delay(2000);
  gate.write(90);
  delay(2000);

  // Start with gate open
  gate.write(90);

  Serial.begin(9600);
}

void loop() {
  int distance1 = getDistance(TRIG1, ECHO1);
  delay(60);
  int distance2 = getDistance(TRIG2, ECHO2);

  // Train approaching: Close the gate
  if (distance1 < 20 && !gateClosed) {
    gate.write(0);
    gateClosed = true;
    trainAtExit = false;

    digitalWrite(LED_PIN, HIGH);
    tone(BUZZER, 1000);

    Serial.println("Train approaching");
    Serial.println("Gate CLOSED");
  }

  // Train reaches the exit sensor
  if (gateClosed && distance2 < 20) {
    trainAtExit = true;
  }

  // Train moves away from the exit sensor
  if (gateClosed && trainAtExit &&
      distance2 >= 20) {
    noTone(BUZZER);
    delay(1500);

    gate.write(90);
    gateClosed = false;
    trainAtExit = false;

    digitalWrite(LED_PIN, LOW);

    Serial.println("Train passed");
    Serial.println("Gate OPEN");
  }

  delay(100);
}