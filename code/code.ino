#include <ESP8266WiFi.h>
#include <espnow.h>

#define IN1 5
#define IN2 4
#define IN3 0
#define IN4 2

#define velocity 200

#define TCRT5000_A 14
#define TCRT5000_B 12

#define SEMAPHORE_RED "vermelho"
#define SEMAPHORE_YELLOW "amarelo"
#define SEMAPHORE_GREEN "verde"

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    String state;
} struct_message;

// Create a struct_message called myData
struct_message semaphore = {
  .state = SEMAPHORE_RED
};

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&semaphore, incomingData, sizeof(semaphore));
}

bool sensorA_state = 0;
bool sensorB_state = 0;

void setup() {
  Serial.begin(115200);
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

  digitalWrite(IN1, 255);
  digitalWrite(IN2, 255);
  digitalWrite(IN3, 255);
  digitalWrite(IN4, 255);
}

void loop() {
  sensorA_state = digitalRead(TCRT5000_A);
  sensorB_state = digitalRead(TCRT5000_B);

  showLogs();

  if (semaphore.state == SEMAPHORE_GREEN  || semaphore.state == SEMAPHORE_YELLOW){
    moveWheels();
  } else if (semaphore.state == SEMAPHORE_RED){
    if (!sensorA_state && !sensorB_state){
      stopWheels();
    } else {
      moveWheels();
    }
  }
  
  delay(20);
}
void showLogs(){
  Serial.println("-----------------------");
  Serial.println("--------SEMAFORO-------");
  Serial.print("Semaforo esta ");
  Serial.println(semaphore.state);
  Serial.println();
  Serial.println("-----SENSORES-----");
  Serial.print("Sensor A: ");
  Serial.println((sensorA_state) ? "livre" : "linha");
  Serial.print("Sensor B: ");
  Serial.println((sensorB_state) ? "livre" : "linha");
  Serial.println();
  Serial.println("-----MOTORES-----");
  Serial.print("Motor A: ");
  Serial.println((sensorB_state) ? "ligado" : "desligado");
  Serial.print("Motor B: ");
  Serial.println((sensorA_state) ? "ligado" : "desligado");
}

void moveWheels(){
  if (sensorA_state && sensorB_state) {
    setDirectionToCenter();
  } else if (!sensorA_state) {
    setDirectionToLeft();
  } else if (!sensorB_state) {
    setDirectionToRight();
  }
}

void setDirectionToLeft() {
  analogWrite(IN1, velocity); 
  analogWrite(IN2, velocity);
  analogWrite(IN3, 255); 
  analogWrite(IN4, 255);
}

void setDirectionToRight() {
  analogWrite(IN1, 255);
  analogWrite(IN2, 255);
  analogWrite(IN3, velocity);
  analogWrite(IN4, velocity);
}

void setDirectionToCenter() {
  analogWrite(IN1, velocity);
  analogWrite(IN2, velocity);
  analogWrite(IN3, velocity); 
  analogWrite(IN4, velocity);
}

void stopWheels() {
  analogWrite(IN1, 255);
  analogWrite(IN2, 255);
  analogWrite(IN3, 255);
  analogWrite(IN4, 255);
}
