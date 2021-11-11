#define IN1 0 
#define IN2 0
#define IN3 0 
#define IN4 0

#define TCRT5000_A 12
#define TCRT5000_B 13

bool sensorA_state = 0;
bool sensorB_state = 0;

void setup(){
  pinMode(TCRT5000_A, INPUT);
  pinMode(TCRT5000_B, INPUT);
  
  Serial.begin(115200);
}
 
void loop() {
  sensorA_state = digitalRead(TCRT5000_A);
  sensorB_state = digitalRead(TCRT5000_B);
  
  printSensorsState();
  printMotorStates();
  
  delay(2000);
}

void printSensorsState(){
  Serial.println();
  Serial.println("-----SENSORES-----");
  Serial.print("sensor A: ");
  Serial.println(sensorA_state);
  Serial.print("sensor B: ");
  Serial.println(sensorB_state);
}
void printMotorStates(){ 
  Serial.println();
  Serial.println("-----MOTORES-----");
  if (!sensorA_state && !sensorB_state){
    Serial.println("Motor A: desligado");
    Serial.println("Motor B: desligado");
  } else if (sensorA_state && sensorB_state){
    Serial.println("Motor A: ligado");
    Serial.println("Motor B: ligado");
  } else if (!sensorA_state){
    Serial.println("Motor A: ligado");
    Serial.println("Motor B: desligado");
  } else if (!sensorB_state){
    Serial.println("Motor A: desligado");
    Serial.println("Motor B: ligado");
  }
}

void setDirectionToLeft(){
  
}
 
void setDirectionToRight(){
  
}
 
void setDirectionToCenter(){
  
}
 
void stopWheels(){
    
}
