#include <ESP8266WiFi.h>
#include <espnow.h>

#define SEMAPHORE_RED 3
#define SEMAPHORE_YELLOW 2
#define SEMAPHORE_GREEN 1

typedef struct struct_message {
  int state;
} struct_message;

struct_message semaphore = {
  .state = SEMAPHORE_GREEN
};

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&semaphore, incomingData, sizeof(semaphore));
  Serial.print("Semaforo esta ");
  Serial.println((semaphore.state == SEMAPHORE_RED) ? "vermelho" : ((semaphore.state == SEMAPHORE_YELLOW) ? "amarelo" : "verde"));
}

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
}

void loop() {

}

void showMACAddress() {
  Serial.print("ESP8266 Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
}
