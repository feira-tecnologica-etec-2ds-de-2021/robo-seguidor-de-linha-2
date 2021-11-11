#define IN1 5
#define IN2 4
#define IN3 0
#define IN4 2

#define TCRT5000_A 12
#define TCRT5000_B 13

bool sensorA_state = 0;
bool sensorB_state = 0;

void setup() {
  pinMode(TCRT5000_A, INPUT);
  pinMode(TCRT5000_B, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(115200);
}

void loop() {
  sensorA_state = digitalRead(TCRT5000_A);
  sensorB_state = digitalRead(TCRT5000_B);

  printSensorStates();
  printMotorStates();

  if (!sensorA_state && !sensorB_state) {
    stopWheels();
  } else if (sensorA_state && sensorB_state) {
    setDirectionToCenter();
  } else if (!sensorA_state) {
    setDirectionToLeft();
  } else if (!sensorB_state) {
    setDirectionToRight();
  }
  delay(50);
}

void printSensorStates() {
  Serial.println();
  Serial.println("-----SENSORES-----");
  Serial.print("sensor A: ");
  Serial.println(sensorA_state);
  Serial.print("sensor B: ");
  Serial.println(sensorB_state);
}
void printMotorStates() {
  Serial.println();
  Serial.println("-----MOTORES-----");
  if (!sensorA_state && !sensorB_state) {
    Serial.println("Motor A: desligado");
    Serial.println("Motor B: desligado");
  } else if (sensorA_state && sensorB_state) {
    Serial.println("Motor A: ligado");
    Serial.println("Motor B: ligado");
  } else if (!sensorA_state) {
    Serial.println("Motor A: ligado");
    Serial.println("Motor B: desligado");
  } else if (!sensorB_state) {
    Serial.println("Motor A: desligado");
    Serial.println("Motor B: ligado");
  }
}

void setDirectionToLeft() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void setDirectionToRight() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
}

void setDirectionToCenter() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopWheels() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
}
