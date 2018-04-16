// Projeto SunFlower - Placa Auxiliar - Arthuíno (Slave)
// Arthur Castro
// Janeiro de 2018

#include <Wire.h>

#define enderecoI2C 2
#define tempoMotor 5

#define LEDAUX 3

#define CR_encontrar90 101
#define CR_MoverDir 102
#define CR_MoverEsq 103
#define CR_EnviarAng 104

#define in1 8
#define in2 9
#define in3 10
#define in4 11

#define pinChave 7

int contEnvAng;

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(LEDAUX, OUTPUT); //TESTE

  pinMode(pinChave, INPUT_PULLUP);

  Wire.begin(enderecoI2C);
  Wire.onReceive(ComandoRecebido);
  Wire.onRequest(enviarAng);
  Serial.begin(9600);
}

void loop() {
  delay(100);
}

// ======================================= LEITURA E REDIRECIONAMENTO DE COMANDO RECEBIDO =======================================

void ComandoRecebido(){
// ------------------------------ Leitura do Código Recebido ------------------------------
  int codigoRecebido = Wire.read();
  Serial.print("Codigo Recebido: ");
  Serial.println(codigoRecebido);

// ------------------------------ Redirecionamento ------------------------------
  switch(codigoRecebido){
    case CR_encontrar90:
      encontrar90();
      break;
    case CR_MoverDir:
      moverDir();
      break;
    case CR_MoverEsq:
      moverEsq();
      break;
    case CR_EnviarAng:
      enviarAng();
      break;
    default:
      break;
  }
}

// ================================================= FUNÇÕES PRINCIPAIS =================================================
void encontrar90(){
  Serial.println(" ----- Funcao Encontrar 90 ----- ");
  Serial.println("");

  boolean estadoChave;
  int aux = 1;

  estadoChave = digitalRead(pinChave);
  
  while(estadoChave == HIGH){
    estadoChave = digitalRead(pinChave);
    giro(1,aux++);
    Serial.println(estadoChave);
    Serial.println("Buscando 90");
    digitalWrite(LEDAUX, LOW);  //TESTE   
  }
  Serial.println("90 encontrado");
  digitalWrite(LEDAUX, HIGH);     //TESTE
  contEnvAng = 90;
}

void moverDir(){
  Serial.println(" ----- Funcao Mover para a Direita ----- ");
  Serial.println("");
  int ang = Wire.read();
  Serial.print("Angulo: ");
  Serial.println(ang);

  giro(1,ang);
}

void moverEsq(){
  Serial.println(" ----- Funcao Mover para a Esquerda ----- ");
  Serial.println("");
  int ang = Wire.read();
  Serial.print("Angulo: ");
  Serial.println(ang);

  giro(0,ang);
}

void enviarAng(){
  Serial.println(" ----- Funcao Enviar Angulo ----- ");
  Serial.println("");
  Wire.write(contEnvAng);
  Serial.print("Angulo atual enviado: ");
  Serial.println(contEnvAng);
}

// ================================================= FUNÇÕES AUXILIARES =================================================
void giro(int sentido, float angulo){
  int passo[4], passoH[4] = {0x03, 0x06, 0x0C, 0x09}, passoAH[4] = {0x09, 0x0C, 0x06, 0x03}, i, j, k, steps, resto;
  
  if(sentido == 0){
  contEnvAng = contEnvAng - angulo;
    for(k=0 ; k<=3 ; k++){
      passo[k] = passoAH[k];
    }
  } else {
  contEnvAng = contEnvAng + angulo;
    for(k=0 ; k<=3 ; k++){
      passo[k] = passoH[k];
    }  
  }
    
  steps = convAngPassos(angulo);    // Retorna um número inteiro de Passos referente ao ângulo
  resto = steps % 4;                // Retorna o resto da divisão
  
  if(resto == 0){
    for(j=1; j<=(steps/4); j++){
      for(i=0; i<=3; i++){
        PORTB = passo[i];
        delay(tempoMotor);
        Serial.println("DEBUG 1");
      }
    }
  } else {
    for(j=1; j<=(steps/4); j++){
      for(i=0; i<=3; i++){
        PORTB = passo[i];
        delay(tempoMotor);
        Serial.println("DEBUG 2");
      }
    }
    for(i=0; i<=(resto-1); i++){
      PORTB = passo[i];
      delay(tempoMotor);
      Serial.println("DEBUG 3");
    }
  }
}

int convAngPassos(float ang){
  float qtdPassosF;
  int qtdPassosI;
  
  qtdPassosF = 5.69 * ang;            // (2048passos / 360graus) * angulo
  qtdPassosI = round(qtdPassosF);     // Arredondamento para Inteiro mais próximo

  return qtdPassosI;
}
