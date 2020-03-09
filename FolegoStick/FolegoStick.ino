//Programa:   FôlegoStick usando 
//            acelerômetro MPU 6050 
//            sensor de pressão HX711
//            arduino micro
//Autores:    Samuel Vitório e William Soares
                      
//Adaptação dos códigos de: 
//  Saulo Alexandre - Autocore Robotica;
//  Bodge - GitHub user.

#include "HX711.h"
#include<Wire.h>;

////////////////////////////////////////////////
//  Fiação do circuito do HX711               //
//  6 e 7 são pinos digitais do arduino       //
//  Scale é a Variavel de leitura das pressões//
//  Assim como a pastReading e reading        //
const int LOADCELL_DOUT_PIN = 7; 
const int LOADCELL_SCK_PIN = 6;
HX711 scale; 
long pastReading = (-200000), reading;

////////////////////////////////////////////////////
// Endereco I2C do MPU6050                        //
// Variaveis para armazenar valores dos sensores  //
// Up e down são valores para definir o giro do CI//
const int MPU=0x68;   
int AcX,AcY,AcZ,up = 12000,down = (-12000);


void setup() {
  //Inicializa o sensor de pressão
  Serial.begin(57600);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  //Inicializa o MPU
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
   
  //Inicializa o acelerômetro
  Wire.write(0); 
  Wire.endTransmission(true);
}

void loop() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  
  //Solicita os dados do acelerômetro
  Wire.requestFrom(MPU,14,true);  
  
  //Armazena o valor dos sensores nas variaveis correspondentes
  AcX=Wire.read()<<8|Wire.read();      
  AcY=Wire.read()<<8|Wire.read(); 
  AcZ=Wire.read()<<8|Wire.read();
  
  //Teste de posições do CI para definir a direção
  if (AcZ > up) {
     Serial.println("Normal");
  } else if (AcY < down ){
     Serial.println("Para Esquerda");
  } else if (AcY > up){
     Serial.println("Para Direita");
  } else if (AcX < down){
     Serial.println("Para Frente");
  } else if (AcX > up){
     Serial.println("Para Tras");
  } else {
     Serial.println("De cabeça para baixo");
  }
  
  //Leitura da pressão
  if (scale.is_ready()) {
    reading = scale.read_average(5);
  } else {
    Serial.println("HX711 not found.");
  }
  if((pastReading - reading)< (-5000)){
    Serial.println("Assoprou");
  } else{
    Serial.println("Não Assoprou");
  }
  pastReading = reading;
  //Aguarda 1s antes de reiniciar o processo
  delay(1000);
}
