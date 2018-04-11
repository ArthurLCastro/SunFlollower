#include<avr/io.h>
#include<util/delay.h>


void MoveServo(int pos);
void MoveEsquerda(int pos_para_esquerda);
void MoveDireita(int pos_para_direita);

int servo = 1200;

int main(){
	DDRB = 0x02;
	DDRD = 0x00;
	PORTD = 0xF8;
	TCCR1A |= 1<<WGM11 | 1<<COM1A1;
	TCCR1B |= 1<<WGM12 | 1<<WGM13 | 1<<CS10;
	ICR1 = 19999;
	OCR1A = 1200;
	const uint8_t esquerda = 0x08;
	const uint8_t direita = 0x20;
	const uint8_t centro = 0x80;
	while(1){
	
		while((PIND & 0xF8)==centro){
			if((PIND & 0xF8)==esquerda){
				MoveEsquerda(servo);
				_delay_ms(200);
			}
			else if((PIND & 0xF8)==direita){
				MoveDireita(servo);
				_delay_ms(200);
			}
			else{
				continue;
			}
		}
		
	}
	return 0;
}
void MoveServo(int pos){
	OCR1A = pos;
}
void MoveEsquerda(int pos_para_esquerda){
	pos_para_esquerda+=10;
	servo = pos_para_esquerda;
	MoveServo(servo);
}
void MoveDireita(int pos_para_direita){
	pos_para_direita-=10;
	servo = pos_para_direita;
	MoveServo(servo);
}
