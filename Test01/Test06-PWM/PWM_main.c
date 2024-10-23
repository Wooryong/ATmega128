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

#define DDR		DDRG	//
#define PORT	PORTG	//

volatile int Period = 500; 
volatile int Duty_rate = 50;


// *** External Interrupt 0 ISR *** //
ISR(INT0_vect) // SW1 (PD0)
{
	OCR1A -= 3000;
	if (OCR1A < 0) OCR1A = 65535;
	
	//OCR1A += 10;
	//if (OCR1A > 255) OCR1A = 50;
	
} // ISR(INT0_vect)
// *** External Interrupt 0 ISR *** //


// *** External Interrupt 1 ISR *** //
ISR(INT1_vect) // SW2 (PD1)
{
	OCR1B -= 3000;
	if (OCR1B < 0) OCR1B = 32767;
	
} // ISR(INT1_vect)
// *** External Interrupt 1 ISR *** //


// *** External Interrupt 2 ISR *** //
// volatile int Max_count = 1;
ISR(INT2_vect) // SW3 (PD2)
{
	OCR1C += 3000;
	if (OCR1C > 65535) OCR1C = 0;	

	/*
	OCR0 += 10;
	if (OCR0 > 255) OCR0 = 150;		
	
	Max_count--;		
	if ( Max_count < 0 ) Max_count = 50;
	*/	
} // ISR(INT2_vect)
// *** External Interrupt 2 ISR *** //


/*
// *** TIMER 0 OCM Interrupt ISR *** //
volatile int LED_G_state = 0;
volatile int count = 0;
ISR(TIMER0_COMP_vect) // 10 ms 주기로 발생
{
	
	count++;
	if( count > Max_count ) // 10 ms * Max_count 간격으로 LED_G 점멸
	{	
		LED_G_state = ~LED_G_state;
		if (LED_G_state)	PORT |=	(1 << LED_G);
		else				PORT &=	~(1 << LED_G); 
		
		count = 0;
	}
	
	// TCNT0 = 0;
} // ISR(TIMER0_COMP_vect)
// *** TIMER 0 OCM Interrupt ISR *** //
*/

/*
// *** TIMER 1 OCMA Interrupt ISR *** //
ISR(TIMER1_COMPA_vect) // Interrupt 발생 주기 : 1s
{

} // ISR(TIMER1_COMPA_vect)
// *** TIMER 1 OCMA Interrupt ISR *** //


// *** TIMER 1 OCMB Interrupt ISR *** //
ISR(TIMER1_COMPB_vect) // Interrupt 발생 주기 : 1s
{

} // ISR(TIMER1_COMPB_vect)
// *** TIMER 1 OCMB Interrupt ISR *** //


// *** TIMER 1 OCMC Interrupt ISR *** //
ISR(TIMER1_COMPC_vect) // Interrupt 발생 주기 : 1s
{

} // ISR(TIMER1_COMPC_vect)
// *** TIMER 1 OCMC Interrupt ISR *** //
*/


int main(void)
{
	DDR |= (1 << LED_R) | (1 << LED_Y) | (1 << LED_G); // PG0, PG1, PG2 출력 (LED)
	// DDR |= 0x07;  
	
	// DDRB |= 0x10; // PB4 (OC0; TIMER 0 OCM)
	DDRB |= 0xE0; // PB5(OC1A), PB6(OC1B), PB7(OC1C)
		
	// *** Interrupt Setting *** //
	/*
	// TIMER 0 OCM Interrupt 		 
	TIMSK |= (1 << OCIE0); // TIMER 0 Output Compare Match Interrupt 활성화
	// TIMSK |= _BV(OCIE0);
	TCCR0 |= 0x07; // TIMER 0 분주비 1024 > Overflow 발생주기 16.384 ms
	OCR0 = 157; // OCM 발생주기 : 16.384 ms * (157 / 256) = ~10 ms
	
	// TCCR0 |= (0 << WGM01) | (0 << WGM00); // TCCR0 |= 0x48 // X1XX_1XXX
	// [WGM01 : WGM00] 00 : 정상모드 / 01 : 위상교정 PWM / 10 : CTC / 11 : 고속 PWM
	// TCCR0 |= (1 << COM01) | (0 << COM00); // TCCR0 |= 0x20 // XX10_XXXX
	// [COM01 : COM00] 정상, CTC 모드  
	// 00 : OC0 X / 01 : OC0 반전 / 10 : OC0 'L' (비반전; Clear) / 11 : OC0 'H' (반전; Set)
	
	TCCR0 |= (1 << WGM01) | (1 << WGM00); // TCCR0 |= 0x48 // X1XX_1XXX
	// 고속 PWM 모드
	TCCR0 |= (1 << COM01) | (0 << COM00); // TCCR0 |= 0x20 // XX10_XXXX
	// 비반전 모드 : 비교일치 발생 > OCn = 'L', Overflow 발생 > OCn = 'H'	
	*/	

	
	// TIMER 1 OCMA Interrupt : 1 sec Unit Time Update
	// TIMSK |= (1 << OCIE1A) | (1 << OCIE1B); // TIMER 1 Output Compare Match (A, B) Interrupt 활성화
	// ETIMSK |= (1 << OCIE1C); // TIMER 1 Output Compare Match (C) Interrupt 활성화
	TCCR1B |= 0x01; // TIMER 1 분주비 64 > Overflow 발생주기 (4.096 * 64) ~ 32 ms
	// 32 ms - 깜박임 보임 
	OCR1A = 65535; // Start 100% Duty
	OCR1B = 32767; // Start 50% Duty
	OCR1C = 0; // Start 0% Duty

	ICR1 = 65535;
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);
	// 고속 PWM (TOP : ICR1) WGM1n = '1110'
	// 8-bit 고속 PWM (TOP : 0x00FF) WGM1n = '0101' >> 주기 짧다. (분주비 1024 -> ~4 ms)
	// 고속 PWM (TOP : OCR1A) WGM1n = '1111'

	TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << COM1C1);
	// 00 : OCn X / 01 : OCn X (15번 고속 PWM모드 제외) / 10 : OC0 'L' (비반전; Clear) / 11 : OC0 'H' (반전; Set)

	// External Interrupt	
	EIMSK |= (1 << INT0) | (1 << INT1) | (1 << INT2); 
	// EIMSK |= (1 << INT0) | (1 << INT1) | (1 << INT2); // External Interrupt 0, 1, 2 활성화
	// EIMSK |= 0x07; 
	EICRA = 0x2A; // Falling Edge Active
	// EICRA = XX10_1010;

	// Global Interrupt Activation
	sei(); 
 

    while (1) 
    {
		/*
		PORT |= (1 << LED_R) | (1 << LED_Y); // LED ON
		_delay_ms( Period * Duty_rate / 100 ); // T_H
		// (Duty_rate / 100) 사용 X - 정수로 처리하여 0이 된다.
		
		PORT &= ~( (1 << LED_R) | (1 << LED_Y) ); // LED OFF
		_delay_ms( Period * (100 - Duty_rate) / 100 ); // T_L
		*/
    }
}

