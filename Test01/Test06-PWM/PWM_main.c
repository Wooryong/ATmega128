/*
 * Test06-PWM.c
 *
 * Created: 2024-10-23 오전 9:45:56
 * Author : user
 */ 

// *** 전처리 *** //
#define F_CPU 16000000L // 16 MHz CLK (설정 안할 시 1 MHz로 동작)
#include <avr/io.h> // AVR 기본 헤더파일
#define __DELAY_BACKWARD_COMPATIBLE__ // delay 함수의 인수로 변수 사용가능 (#include <util/delay.h> 위에 선언)
#include <util/delay.h> //delay를 실행하기 위한 헤더파일
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>


#define LED_R	PORTG0 // PORTG0 = 0 / ( 1 << 0 )
#define LED_Y	PORTG1 // PORTG1 = 1 / ( 1 << 1 )
#define LED_G	PORTG2 // PORTG2 = 2 / ( 1 << 2 )

#define BUZZER	PORTB6 // PB6 BUZZER

#define DDR		DDRG	//
#define PORT	PORTG	//

volatile int Period = 500; 
volatile int Duty_rate = 50;

//
enum {	DO = 130,
		Do = 138,
		RE = 146,
		Re = 155,
		MI = 164,
		FA = 174,
		Fa = 185,
		SL = 196,
		Sl = 207,
		LA = 220,
		La = 233,
		SI = 246
		}; // 3옥타브 음계 주파수 리스트 : 도 도# 레 레# 미 파 파# 솔 솔# 라 라# 시
		
unsigned int song[] = {	SL, SL, LA, LA,
				SL, SL, MI,
				SL, SL, MI, MI, RE,
				SL, SL, LA, LA,
				SL, SL, MI,
				SL, MI, RE, MI, DO
				}; // 음계 주파수 		

unsigned int TOP[24]; // 각 음계 주파수에 해당하는 TOP(ICR1) 값 계산
// Large ICR1 > Long Period > Small Frequency
//
int CS = 1024; // 분주비 1024
void conv()
{
	for( int i = 0 ; i < 25 ; i++ )	
	{
		// song1[i] = F_CPU / ( CS * song[i] );		
		// song1[i] = F_CPU / CS / song[i];	
		// song1[i] = F_CPU / (long)( CS * song[i] ); // 계산과정 중 int형 변수 Overflow 막기 위해서 	
		TOP[i] = F_CPU / (long)( CS * song[i] ) / 4; // 3옥타브 > 5옥타브 (BUZZER의 동작주파수 범위 고려) 
		// Target Frequency = 16 MHz / { (분주비) * (TOP) }
		// TOP = { 16 MHz / (분주비) } / (Target Frequency) = (15.625 kHz) / (Target Frequency) // 분주비 1024	
		// ex) 도 : ICR = 120 / 시 : ICR = 63
		// 
		// 분주비 64 : (분주비 8 * 5) 보다 더 고주파수
	}		
}
//

// *** TIMER 1 OCMA Interrupt ISR *** //
/*
volatile int count = 0;
volatile int order = 0;
ISR(TIMER1_COMPB_vect) // 
{

	count++;
	
	if (count > 500)
	{
		count = 0;
		
		order++;
		if (order > 23) order = 0;			
	}
	// 도 : TOP 120 > 7.68 ms 마다 발생 
	// 시 : TOP 63 > 4 ms
	
} // ISR(TIMER1_COMPB_vect)
// *** TIMER 1 OCMB Interrupt ISR *** //
*/


int main(void)
{
	DDR |= (1 << LED_R) | (1 << LED_Y) | (1 << LED_G); // PG0, PG1, PG2 출력 (LED)
	// DDR |= 0x07;  
	
	// DDRB |= 0x10; // PB4 (OC0; TIMER 0 OCM)
	DDRB |= 0xE0; // PB5(OC1A), PB6(OC1B), PB7(OC1C)
		
	// *** Interrupt Setting *** //
	// TIMER 1 OCMA Interrupt 
	// TIMSK |= (1 << OCIE1A) | (1 << OCIE1B); // TIMER 1 Output Compare Match (A, B) Interrupt 활성화
	// TIMSK |= (1 << OCIE1B); // TIMER 1 Output Compare Match (B) Interrupt 활성화
	// ETIMSK |= (1 << OCIE1C); // TIMER 1 Output Compare Match (C) Interrupt 활성화
	TCCR1B |= 0x05; // TIMER 1 분주비 1024 > Overflow 발생주기 (4.096 * 1024) ~ 4 s
	// '010' : 분주비 8 / '011' : 분주비 64 / '100' : 분주비 256 / '101' : 분주비 1024
	// 32 ms - 깜박임 보임 
	// OCR1A = 65535; // Start 100% Duty
	// OCR1B = 32767; // Start 50% Duty
	// OCR1C = 0; // Start 0% Duty

	// ICR1 = 65535; // 고속 PWM (TOP : ICR1)
	// Target Frequency = 16 MHz / { (분주비) * (TOP) }
	// TOP = { 16 MHz / (분주비) } / (Target Frequency) = (15.625 kHz) / (Target Frequency) // 분주비 1024
	// Period = (4.096 ms) * (TOP / 65536)
	
	TCCR1A |= (1 << WGM11); 
	TCCR1B |= (1 << WGM12) | (1 << WGM13);
	// 고속 PWM (TOP : ICR1) WGM1n = '1110' / 고속 PWM (TOP : OCR1A) WGM1n = '1111'

	StandBy();	
	TCCR1A |= (1 << COM1B1);
	// TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << COM1C1);	
	// 00 : OCn X / 01 : OCn X (15번 고속 PWM모드 제외) / 10 : OC0 'L' (비반전; Clear) / 11 : OC0 'H' (반전; Set)


	// Global Interrupt Activation
	sei();
 	

	conv();	
	int order = 0; //
    while (1) 
    {		
		ICR1 = TOP[order];			
		OCR1B = ICR1 / 2;
		_delay_ms(500);
		order++;
//		OCR1B = OCR1B * 2;
//		_delay_ms(5);
		
		if (order > 23) order = 0;
    }
}

