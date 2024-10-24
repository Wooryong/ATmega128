/*
 * Test06-PWM_My.c
 *
 * Created: 2024-10-24 오전 10:31:40
 * Author : user
 */ 

#include <avr/io.h>

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
	
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);
	// 고속 PWM (TOP : ICR1) WGM1n = '1110'
	// 8-bit 고속 PWM (TOP : 0x00FF) WGM1n = '0101' >> 주기 짧다. (분주비 1024 -> ~4 ms)
	// 고속 PWM (TOP : OCR1A) WGM1n = '1111'

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

// *** External Interrupt 0 ISR *** //
ISR(INT0_vect) // SW1 (PD0)
{
	// OCR1A -= 3000;
	// if (OCR1A < 0) OCR1A = 65535;
	
} // ISR(INT0_vect)
// *** External Interrupt 0 ISR *** //


// *** External Interrupt 1 ISR *** //
ISR(INT1_vect) // SW2 (PD1)
{
	// OCR1B -= 3000;
	// if (OCR1B < 0) OCR1B = 32767;
	
} // ISR(INT1_vect)
// *** External Interrupt 1 ISR *** //


// *** External Interrupt 2 ISR *** //
// volatile int Max_count = 1;
ISR(INT2_vect) // SW3 (PD2)
{
	// OCR1C += 3000;
	// if (OCR1C > 65535) OCR1C = 0;	

	/*
	Max_count--;		
	if ( Max_count < 0 ) Max_count = 50;
	*/	
} // ISR(INT2_vect)
// *** External Interrupt 2 ISR *** //


int main(void)
{
	// External Interrupt
	// EIMSK |= (1 << INT0) | (1 << INT1) | (1 << INT2);
	// EIMSK |= (1 << INT0) | (1 << INT1) | (1 << INT2); // External Interrupt 0, 1, 2 활성화
	// EIMSK |= 0x07;
	// EICRA = 0x2A; // Falling Edge Active
	// EICRA = XX10_1010;	 
 
 
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

