//Definição dos pinos de controle do motor
#define IN1 2 // Pino_Velocidade 1º Motor ( 0 a 255)_ Porta ATV_A ponte H;
#define IN2 3 //Pino_Velocidade 2º Motor ( 0 a 255) _ Porta ATV_B ponte H;
#define IN3 4 //Pino_Direção do 1º Motor: Para frente / Para trás (HIGH ou LOW)_ porta IN1 ponte H;
#define IN4 5 //Pino_Direção do 2º Motor: Para frente / Para trás (HIGH ou LOW)_ porta IN3 ponte H;
 
//Definição dos pinos dos sensores
#define pinLed 12
#define pin_S1 9
#define pin_S2 8
#define sensorInBlack 1
#define sensorInWhite 0
bool Sensor1 = 0;
bool Sensor2 = 0;
 
//semaforo
#define semaphoreGreen 1
#define semaphoreRed 2
#define semaphoreYellow 3
 
//variável responsável por controlar a velocidade dos motores
#define velocidadeMotores 200   
void setup(){
  //Motor DC
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
 
  //sensor TCRT5000
  pinMode(pin_S1, INPUT);
  pinMode(pin_S2, INPUT);
  pinMode(pin_S2, INPUT);
  pinMode(pinLed, OUTPUT);
  digitalWrite(pinLed, LOW);

  //motor setup
  digitalWrite(IN1, 255);
  digitalWrite(IN2, 255);
  digitalWrite(IN3, 255);
  digitalWrite(IN4, 255);
  
  Serial.begin(115200);
}
 
void loop() {
  //Neste processo armazenamos o valor lido pelo sensor na variável que armazena tais dados.
  Sensor1 = digitalRead(pin_S1);
  Sensor2 = digitalRead(pin_S2);
 
  if (getSemaphoreState() == semaphoreRed){
    stopWheels();
  } else {
    if ((Sensor1 == sensorInWhite) && (Sensor2 == sensorInWhite)){
      setDirectionToCenter();
    }
    if ((Sensor1 == sensorInBlack) && (Sensor2 == sensorInWhite)){
      setDirectionToRight();
    }
    if ((Sensor1 == sensorInWhite) && (Sensor2 == sensorInBlack)){ 
      setDirectionToLeft();
    }
  }
}
 
void setDirectionToLeft(){
  analogWrite(IN4, velocidadeMotores);
  analogWrite(IN3, velocidadeMotores);
  analogWrite(IN2, 255);
  analogWrite(IN1, 255);
}
 
void setDirectionToRight(){
  analogWrite(IN4, 255);
  analogWrite(IN3, 255); 
  analogWrite(IN2, velocidadeMotores);
  analogWrite(IN1, velocidadeMotores); 
}
 
void setDirectionToCenter(){
  analogWrite(IN1, velocidadeMotores);
  analogWrite(IN2, velocidadeMotores);
  analogWrite(IN3, velocidadeMotores); 
  analogWrite(IN4, velocidadeMotores);
}
 
void stopWheels(){
    // Motor lado esquerdo parado
    analogWrite(IN1, 255);
    analogWrite(IN2, 255);
 
    // Motor lado direito parado
    analogWrite(IN3, 255);
    analogWrite(IN4, 255);
}
 
byte getSemaphoreState(){
    
  /* aqui ficará a implementação da comunicação com o farol */
  
  return byte(semaphoreGreen); //simulando que o semaforo esta verde

}
