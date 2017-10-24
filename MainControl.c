/*programa do controlador principal  em desenvolvimento...
*-implementação da leitura dos sensores e atuação através de servo motor em um eixo.
*
*/
#include<avr/io.h>
#include<util/delay.h>


void MoveServo(int pos);
void MoveEsquerda(int pos_para_esquerda);
void MoveDireita(int pos_para_direita);

int servo = 1200;

int main(){
	DDRB = 0x02;
	DDRC = 0x00;
	PORTC = 0xFF;
	TCCR1A |= 1<<WGM11 | 1<<COM1A1;
	TCCR1B |= 1<<WGM12 | 1<<WGM13 | 1<<CS10;
	ICR1 = 19999;
	//OCR1A = 1200;
	while(1){
	
		
			/*if(PINC==0x01){
				MoveEsquerda(servo);
				_delay_ms(500);
			}*/
			if(PINC==0xE2){
				MoveDireita(servo);
				_delay_ms(500);
			}
			else{
				MoveServo(servo);
				_delay_ms(500);
			}
		
	}
	return 0;
}
void MoveServo(int pos){
	OCR1A = pos;
}
void MoveEsquerda(int pos_para_esquerda){
	pos_para_esquerda+=100;
	servo = pos_para_esquerda;
	MoveServo(servo);
}
void MoveDireita(int pos_para_direita){
	pos_para_direita-=100;
	servo = pos_para_direita;
	MoveServo(servo);
}
