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
// #include <avr/sfr_defs.h>
#include <avr/interrupt.h>

// 
# define LED1	PORTG1 // LED - PG1 / PORTG1 = 1 / ( 1 << 1 )
# define SW2	PORTD0 // SW2 - PD0 / PORTD0 = 0 / ( 1 << 0 )
# define SW3	PORTD1 // SW3 - PD1 / PORTD1 = 1 / ( 1 << 1 )

# define SW		PORTG4 // SW - PG4 / PORTG4 = 4 / ( 1 << 4 )

// Port 선언은 main 함수 위에 

// 외부 인터럽트 Pin : PD0 (INT0), PD1 (INT1)

// SW ON = '0' & SW OFF = '1'	
// #define _BV(bit) (1 << (bit))

void StandBy() // PG4 Pin을 프로그램의 시작 스위치로 연결 
{
	DDRG &= ~(0x10); // PG4 : 입력 
	PORTG |= 0x10; // PG4 내부 Pull-up 저항 사용 설정
	 
	// PING의 초기값 N/A (외부 Pull-up 저항 연결되어 있지만 초기값 '1' 로 확신할 수 없음)
	// 초기값이 '1' 인지 검증하는 과정 필요
	while ( (PING & 0x10 ) == 0 ); // PING0의 초기값이 '1'이여야 탈출 (Pull-up 저항 SW)	
	// while (1)
	// {
	//		 if ( (PING & 0x10 ) == 1 ) 
	//			break;
	// }
		
	// PG4의 초기값 검증 이후 
	while(1)
	{
		if ( (PING & 0x10 ) == 0 ) // SW ON 되면 탈출 
			break;		
	}		
	// while( (PING & 0x10 ) == 1 )
}

 	volatile int ival = 1000; // 초기 Delay 1000 ms 설정 
	// 

ISR(INT0_vect) // 점점 느리게
{
	ival += 100;
}

ISR(INT1_vect) // 점점 빠르게
{
	ival -= 100;
	if(ival < 100)
		ival = 100;
}

int main(void)
{
	
	DDRD &= ~(0x03); // XXXX_XX00	
	PORTD |= 0x03;
		
	// DDRG &= ~(0x0C); // XXXX_00XX
	// PG3, PG2 입력 설정
	// PORTG |= 0x0C; // XXXX_11XX / 내부 Pull-up 저항
			
	DDRG |= 0x03; // XXXX_XX11
	// PG1 PG0 출력 설정
	 
	PORTG |= _BV(LED1); // 1 << LED1 = 0x02; / LED ON
	
	StandBy();
	
	PORTG &= ~( _BV(LED1) ); 
	// PORTG &= ~(0x02);
	
	// Mask Register : EIMSK
	EIMSK |= 0x03; // INT1, INT0
	// XXXX_XX11
	
	// Create Register : EICRA
	EICRA |= 0x0A; // XXXX_1010
	// ISC01 = 1, ISC00 = 0
	// ISC11 = 1, ISC10 = 0
	// SW OFF > ON == H > L (하강엣지)
	
	sei();
			
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
					
		PORTG |= _BV(LED1); // LED ON
		_delay_ms(ival); // ms 단위의 지연시간 설정
		// ival = delay 조절
			
		PORTG &= ~( _BV(LED1) ); // LED OFF 
		_delay_ms(ival);
				
    }
	
	
}

