#include <ESP8266WiFi.h>
#include <espnow.h>

#define IN1 5
#define IN2 4
#define IN3 0
#define IN4 2
#define ENA_ENB 13

#define TCRT5000_A 14
#define TCRT5000_B 12

#define DUTY_CYCLE 130

#define SEMAPHORE_RED 3
#define SEMAPHORE_YELLOW 2
#define SEMAPHORE_GREEN 1

typedef struct struct_message {
  int state;
} struct_message;

struct_message semaphore = {
  .state = SEMAPHORE_RED
};

bool sensorA_state = 0;
bool sensorB_state = 0;

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&semaphore, incomingData, sizeof(semaphore));
  Serial.print("Semaforo esta ");
  Serial.println((semaphore.state == SEMAPHORE_RED) ? "vermelho" : ((semaphore.state == SEMAPHORE_YELLOW) ? "amarelo" : "verde"));
}

void initESPNOW(){
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
}

void showMACAddress() {
  Serial.print("ESP8266 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
}

void setup() {
  Serial.begin(115200);
  initESPNOW();
  
  pinMode(TCRT5000_A, INPUT);
  pinMode(TCRT5000_B, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA_ENB, OUTPUT);

  analogWrite(ENA_ENB, DUTY_CYCLE);
}

void loop() {
  sensorA_state = digitalRead(TCRT5000_A);
  sensorB_state = digitalRead(TCRT5000_B);

  if (isSemaphoreClosed() && isOnPedestrianCrossing()){
    stopMotors();
  } else if (sensorA_state && sensorB_state) {
    moveMotors();
  } else if (!sensorA_state) {
    moveMotorB();
  } else if (!sensorB_state) {
    moveMotorA();
  }

  showLogs();
  delay(500);
}

void showLogs(){
  Serial.println("-----SENSORES-----");
  Serial.print("Sensor A: ");
  Serial.println((sensorA_state) ? "livre" : "linha");
  Serial.print("Sensor B: ");
  Serial.println((sensorB_state) ? "livre" : "linha");
  Serial.println("-----MOTORES-----");
  Serial.print("Motor A: ");
  Serial.println((!sensorB_state) ? "ligado" : "desligado");
  Serial.print("Motor B: ");
  Serial.println((!sensorA_state) ? "ligado" : "desligado");
  Serial.println("\n\n");
}

bool isSemaphoreClosed(){
  return semaphore.state == SEMAPHORE_RED;
}

bool isOnPedestrianCrossing(){
  return (!sensorA_state && !sensorB_state);
}

void moveMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void moveMotorA() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void moveMotorB() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}