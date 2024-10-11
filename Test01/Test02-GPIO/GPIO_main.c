/*
 * Test02-GPIO.c
 *
 * Created: 2024-10-10 오전 9:35:49
 * Author : user
 */ 

// *** 전처리 ***
#define F_CPU 16000000L // 16 MHz CLK (설정 안할 시 1 MHz로 동작)
#include <avr/io.h> // AVR 기본 헤더파일
# define __DELAY_BACKWARD_COMPATIBLE__ // delay 함수의 인수로 변수 사용가능 (#include <util/delay.h> 위에 선언)
#include <util/delay.h> //delay를 실행하기 위한 헤더파일
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>
//#include "MyLib.c"

// 
# define LED1	PORTG1 // PORTG1 = 1 / ( 1 << 1 )
# define SW2	PORTD0 // PORTD0 = 0 / ( 1 << 0 )
# define SW3	PORTD1 // PORTD1 = 1 / ( 1 << 1 )
//
# define SW		PORTG4 // PORTG4 = 4 / ( 1 << 4 )


// 외부 인터럽트 Pin : PD0 (INT0), PD1 (INT1), PD2 (INT2), PD3 (INT3)

// SW ON = '0' & SW OFF = '1'	
// #define _BV(bit) (1 << (bit))


 volatile int ival = 1000; // 초기 Delay 1000 ms 설정 
 
// *** ISR 정의 ***
ISR(INT0_vect) // SW2(PD0) - 점점 느리게
{
	ival += 100;
	if( ival > 2000 ) ival = 2000;
}

ISR(INT1_vect) // SW3(PD1) - 점점 빠르게
{
	ival -= 100;
	if( ival < 100 ) ival = 100;
}

//
int main(void)
{	
	DDRD &= ~(0x03); // PD0, PD1 입력 (SW)
	PORTD |= 0x03; // PD0, PD1 내부 Pull-up 저항 사용
		
	DDRG |= 0x03; // PG0, PG1 출력 (LED)
	DDRG &= ~(0x10); // PG4 : 입력		
	PORTG |= 0x10; // PG4 내부 Pull-up 저항 사용 설정 
	 
	PORTG |= _BV(LED1); // 1 << LED1 = 0x02; / LED ON
	
	StandBy();
	
	PORTG &= ~( _BV(LED1) ); // LED OFF
	// PORTG &= ~(0x02);
	
	// Mask Register : EIMSK
	EIMSK |= 0x03; // INT1, INT0
	// EIMSK = 0x03; ==> EIMSK = 0000_0011
	
	// Create Register : EICRA
	EICRA = ( EICRA & (0xF0) ) | (0x0A); // 해당범위 먼저 0으로 만들고 1 덮어씌우기
	//	EICRA |= 0x0A; // XXXX_1X1X
	//	EICRA &= ~(0x05); // XXXX_X0X0	
	// SW OFF > ON == H > L (하강엣지) / SW ON > OFF == L > H (상승엣지)
	// ISC11 = 1, ISC10 = 0 >> INT1 하강엣지 / ISC11 = 1, ISC10 = 1 >> INT1 상승엣지
	// ISC01 = 1, ISC00 = 0 >> INT0 하강엣지 / ISC01 = 1, ISC00 = 1 >> INT0 상승엣지
	// '10' - 하강엣지, '11' - 상승엣지
	
	sei(); // SREG |= 0x80;
			
    while (1) 
    {
//		if ( (PING & _BV(SW2) ) == 0 ) // SW2 (PG1) ON
//		// if ( (PING & 0x02 ) == 0 )
//			ival += 500; // Slower
//				
//		else if ( (PING & _BV(SW3) ) == 0 )	// SW3 (PG2) ON	
//		// else if ( (PING & 0x04 ) == 0 )
//			ival -= 500; // Faster
//			
//		if(ival < 10)
//			ival = 100;	
		
		// Delay 변화를 Interrupt로 처리 			
		PORTG |= _BV(LED1); // LED ON
		_delay_ms(ival); // ms 단위의 지연시간 설정
		// ival = delay 조절
			
		PORTG &= ~( _BV(LED1) ); // LED OFF 
		_delay_ms(ival);
				
    }
	
	
}

