// Projeto SunFlower - Placa Principal (Master)
// Arthur Castro
// Janeiro de 2018

#include <Wire.h>
#include<avr/io.h>
#include<util/delay.h>

#define horario 0
#define anti_horario 1
#define graus 30
#define END_Aux 2

void passo_m1(int passos, int dir);
void busca_zero(void);

boolean sensorE, sensorC, sensorD, sensorB, sensorP;
void lerSensor(), envioI2C(), pedidoI2C();

uint8_t m1_move[4] = {0x03,0x06,0x0c,0x09};
int m1_offset = 0;
int direcao = 1;
int n_passos = 5;
int passos = 0;


void setup() {
  Wire.begin(1);
  Serial.begin(9600);
  pedidoI2C(101,0);
  envioI2C(103,45);
/*  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);
  pinMode(6,INPUT);
  pinMode(7,INPUT);
  */
  
}

void loop() {
// -------------------- Leitura de Sensores -------------------- 
  sensorE = lerSensor('E');
  sensorC = lerSensor('C');
  sensorD = lerSensor('D');
  sensorB = lerSensor('B');
  sensorP = lerSensor('P');

// ------------------- LDRs no Serial Monitor -------------------
  
  boolean LDR1 = (sensorE && sensorC);
  boolean LDR2 = (sensorC);
  boolean LDR3 = (sensorD && sensorC);
  boolean LDR4 = (sensorE);
  boolean LDR5 = (sensorP);
  boolean LDR6 = (sensorD);
  boolean LDR7 = (sensorE && sensorB);
  boolean LDR8 = (sensorB);
  boolean LDR9 = (sensorD && sensorB);

  Serial.println("      C      ");
  Serial.print("   ");
  Serial.print(LDR1);
  Serial.print("  ");
  Serial.print(LDR2);
  Serial.print("  ");
  Serial.print(LDR3);
  Serial.println("   ");
  Serial.print("E  ");
  Serial.print(LDR4);
  Serial.print("  ");
  Serial.print(LDR5);
  Serial.print("  ");
  Serial.print(LDR6);
  Serial.println("  D");
  Serial.print("   ");
  Serial.print(LDR7);
  Serial.print("  ");
  Serial.print(LDR8);
  Serial.print("  ");
  Serial.print(LDR9);
  Serial.println("   ");
  Serial.println("      B      ");
  Serial.println(" ");

  delay(1000);
  
  for(int x=0; x<50; x++){ Serial.println();}

// -------------------- Laço -------------------- 
//  while(1){
//  }
}

boolean lerSensor(char sensor){
const int pinE=3, pinC=4, pinD=5, pinB=6, pinP=7;
boolean estadoE=0, estadoC=0, estadoD=0, estadoB=0, estadoP=0;

  switch(sensor){
    case 'E':
      estadoE = digitalRead(pinE);
      return estadoE;
      break;
    case 'C':
      estadoC = digitalRead(pinC);
      return estadoC;
      break;
    case 'D':
      estadoD = digitalRead(pinD);
      return estadoD;
      break;
    case 'B':
      estadoB = digitalRead(pinB);
      return estadoB;
      break;
    case 'P':
      estadoP = digitalRead(pinP);
      return estadoP;
      break;
    default:
      Serial.println("Insira um parametro valido (E, C, D, B ou P)...");
  }
}

void envioI2C(int codigo0, int codigo1){
  unsigned int codigo[2];

  codigo[0] = codigo0;
  codigo[1] = codigo1;
   
  Wire.beginTransmission(2);
    for(int i=0; i<=1; i++){
      Wire.write(codigo[i]);
      Serial.print("Enviado: ");
      Serial.println(codigo[i]);
    }
  Wire.endTransmission();
}

int pedidoI2C(int codigo0, int codigo1){
  unsigned int codigo[2];
  char textoRecebido[12];
  int ang, j;
  
  codigo[0] = codigo0;
  codigo[1] = codigo1;
  Wire.beginTransmission(2);
    for(int i=0; i<=1; i++){
    Wire.write(codigo[i]);
    Serial.print("Enviado: ");
    Serial.println(codigo[i]);
  }
  if(codigo[0] == 104){
    Wire.requestFrom(2, 3);    // request 6 bytes from slave device #2
    ang = Wire.read();
    Serial.print("Angulo Recebido: ");
    Serial.println(ang);
  }
  if(codigo[0] == 101){
    Wire.requestFrom(2, 12);    // request 6 bytes from slave device #2
      while (Wire.available()) { // slave may send less than requested
        j = 0;
        textoRecebido[j] = Wire.read(); // receive a byte as character
        Serial.print(j);         // print the character
        j++;
      }
      Serial.println();
  }
  Wire.endTransmission();
}





/*
#include<avr/io.h>
#include<util/delay.h>
//#include<math.h>

#define horario 0
#define anti_horario 1
#define graus 30
#define END_Aux 2

int main(void);
void passo_m1(int passos, int dir);
void busca_zero(void);

void TWI_init(void);
void TWI_start(void);
void TWI_stop(void);
void TWI_write(uint8_t dado);
uint8_t TWI_ack(void);
uint8_t TWI_nack(void);
uint8_t TWI_get_status(void);

unsigned char m1_move[4] = {0x03,0x06,0x0c,0x09};
int m1_offset = 0;
int direcao = 1;
int n_passos = 5;
int passos = 0;

int main(){
 DDRD = 0xf8;
  PORTD = 0x00;
  DDRB = 0x0f;
  PORTB |= 0x00;
  TWI_init();

  while(1){ 
    
    busca_zero();
    _delay_ms(1000);
    //while(bit_is_clear(PIND,2)){}
    //while(bit_is_set(PIND,2)){}
    direcao = horario;
    //while(bit_is_clear(PIND,2)){
    passo_m1(90, direcao );
    _delay_ms(1000);
    
      
    
    TWI_init();
    TWI_start();
    TWI_write(0x02);
    TWI_write(103);
    //TWI_start();
    TWI_write(45);
    TWI_stop();
    _delay_ms(2000);
    
    
  }
  return 0;
}
void passo_m1(int passos, int dir){
  passos *= 29;
  do{ 
    
    if(dir == 1){
      m1_offset++;
      if (m1_offset == 5) m1_offset = 1;
      PORTB = m1_move[(m1_offset - 1)];
      n_passos += 1;
    }
    else if(dir == 0){
      m1_offset--;
      if (m1_offset <= 0) m1_offset = 4;
      PORTB = m1_move[(m1_offset - 1)];
      n_passos -= 1;
    }
    _delay_ms(10);
    passos -= 1;
    
  }while(passos != 0);
}
void busca_zero(void){
  while(bit_is_clear(PIND,2)){
  passo_m1(1,1);
  _delay_ms(2);
  }
  _delay_ms(500);
  n_passos = 0;
}
*/
