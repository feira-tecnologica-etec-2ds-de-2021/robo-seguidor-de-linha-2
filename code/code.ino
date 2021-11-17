#include <ESP8266WiFi.h>
#include <espnow.h>

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    int semaphoreState;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Int: ");
  Serial.println(myData.semaphoreState);
}

#define IN1 15
#define IN2 13
#define IN3 12
#define IN4 14

#define TCRT5000_A 5
#define TCRT5000_B 4

bool sensorA_state = 0;
bool sensorB_state = 0;

void setup() {
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

  
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
