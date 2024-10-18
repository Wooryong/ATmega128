/*
 * Test05-FND.c
 *
 * Created: 2024-10-16 오후 3:12:15
 * Author : user
 */ 

// *** 전처리 *** //
#define F_CPU 16000000L // 16 MHz CLK (설정 안할 시 1 MHz로 동작)
#include <avr/io.h> // AVR 기본 헤더파일
#define __DELAY_BACKWARD_COMPATIBLE__ // delay 함수의 인수로 변수 사용가능 (#include <util/delay.h> 위에 선언)
#include <util/delay.h> //delay를 실행하기 위한 헤더파일
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>


#define CDDR	DDRB // Control Port : Port B
#define IDDR	DDRA // Image Port : Port A

#define CPORT	PORTB // Control Port : Port B
#define IPORT	PORTA // Image Port : Port A

#define LED0	PORTG0 // PORTG0 = 0 / ( 1 << 0 )
#define LED1	PORTG1 // PORTG1 = 1 / ( 1 << 1 )
#define LED2	PORTG2 // PORTG2 = 2 / ( 1 << 2 )

#define SW2		PORTD0 // PORTD0 = 0 / ( 1 << 0 )
#define SW3		PORTD1 // PORTD1 = 1 / ( 1 << 1 )
// *** 전처리 *** //


//unsigned char img[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67};
unsigned char img[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67, 0x00}; // img[10] for Null	
//unsigned char img[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71}; // 0 ~ F

// 반전된 Version 	
//unsigned char img[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xD8, 0x80, 0x90};
//unsigned char img[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xD8, 0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E}; // 0 ~ F


volatile int opt_mode, watch_mode, timer_mode = 0; 
// [opt_mode] 0 : Watch 모드, 1 : Timer 모드
// [watch_mode] 0 : Default / 1 : Time Setting (Initial Minute) / 2 : Time Setting End (Initial Minute) / 3 : Watch Operation Start
// [timer_mode] 0 : Default / 1 : Time Setting (Target Second) / 2 : Time Setting End (Target Second) / 3 : Timer Operation Start

//
volatile int data[5] = {1, 2, 3, 4}; // 초기화
int digit(int num) // num 변수로부터 1000, 100, 10, 1 자릿수 추출하여 data[] 배열에 저장
{
	if( num > 9999 ) return 0; // 인수 유효성 Check (범위 초과 시 0을 반환)	
	// 최대 표현 범위 99:99
	data[3] = num % 10; // Digit_1 (10 ms)
	data[2] = (num / 10) % 10; // Digit_10 (100 ms)
	data[1] = (num / 100) % 10; // Digit_100 (1 s)
	data[0] = (num / 1000); // Digit_1000 (10 s)
	// 
	// 상위 Digit 0 대신 Null 처리
	/*	
	if ( num < 10 )
	{
		data[2] = 10; data[1] = 10; data[0] = 10;
	}
	else if ( num < 100 )
	{
		data[1] = 10; data[0] = 10;
	}
	else if ( num < 1000 )
		data[0] = 10;
	*/
	// 상위 Digit 0 대신 Null 처리
			
	return 1;	
} 
// int digit(int num)

//
volatile int time_data[5] = {1, 2, 3, 4}; // 초기화
int time_digit (int time_num) // time_num 변수로부터 10분, 1분, 10초, 1초 자릿수 추출하여 time_data[] 배열에 저장
{
	if( time_num > 5999 ) return 0; // 인수 유효성 Check (범위 초과 시 0을 반환)		
	// 최대 표현 범위 99:59 (99분 59초 이므로 5940(60분) + 59 = 5,999)
	int sec = time_num % 60; // 00 ~ 59
	int minute = time_num / 60;	// 00 ~ 99
	//
	time_data[3] = sec % 10; // Digit_1 (1 s)
	time_data[2] = sec / 10; // Digit_10 (10 s)
	time_data[1] = minute % 10; // Digit_100 (1 min.)
	time_data[0] = minute / 10;// Digit_1000 (10 min.)
	//		
	return 1;	
} 
// 


// TIMER 2 OCM Interrupt ISR //
volatile int i = 0;
int dp = 1; // 두번째 자리 옆에 소수점 표시 
ISR(TIMER2_COMP_vect) // Interrupt 발생 주기 : 4 ms 
{
	// FND Refresh //	
	CPORT = _BV(i); 
	//
	if ( opt_mode == 0 )
	{
		if ( watch_mode == 0 )
			IPORT = ~( img[ 0 ] );
		else // (mode != 0)
		{
			if( i == dp )
				IPORT = ~( ( img[ time_data[ i ] ] ) | (0x80) ); // 두번째 자리에 소수점 표시
			else
				IPORT = ~( img[ time_data[ i ] ] );	
		}
	}
	//
	if( opt_mode == 1 )
	{
		if ( timer_mode == 0 ) 
			IPORT = ~( img[ 0 ] );
		else // (mode != 0)
		{
			if( i == dp ) 	
				IPORT = ~( ( img[ data[ i ] ] ) | (0x80) ); // 두번째 자리에 소수점 표시
			else
				IPORT = ~( img[ data[ i ] ] );	
		}
	}	
	//
	i++;
	if (i == 4) i = 0;
	//		
	TCNT2 = 0; //		
} // ISR(TIMER2_COMP_vect) 
// TIMER 2 OCM Interrupt ISR //


// TIMER 0 OCM Interrupt ISR //
volatile int tick, Stick = 0; //
ISR(TIMER0_COMP_vect) // Interrupt 발생 주기 : 10 ms (정확히는 9.984 ms)
{
	if ( timer_mode == 3 )
	{
		if (tick < Stick) tick++;		
		if( tick > 9999 ) tick = 0;
	}
	TCNT0 = 0; // TCNT0 초기화
} // ISR(TIMER0_COMP_vect)
// TIMER 0 OCM Interrupt ISR //


// TIMER 1 OCMA Interrupt ISR //
volatile int time_tick, time_Stick = 0;
ISR(TIMER1_COMPA_vect) // Interrupt 발생 주기 : 1 s 
{
	if ( watch_mode == 3 ) 
	{
		time_tick++;		
		if( time_tick > 5999 ) time_tick = 0; // (표현가능 범위 초과 시 0으로)
	}
			
	TCNT1 = 0; // TCNT1 초기화
} // ISR(TIMER1_COMPA_vect)
// TIMER 1 OCM Interrupt ISR //


// External Interrupt ISR //
ISR(INT0_vect) // BUT1 (PD0) 
{
	if ( opt_mode == 0 ) // Watch
	{	
		if (watch_mode < 3) watch_mode++;
		if ( watch_mode == 2 ) time_Stick = time_tick;
	}
	//	
	if ( opt_mode == 1 ) // Timer
	{
		if( timer_mode == 0 ) timer_mode = 1;
		else if( timer_mode == 1 ) 
		{
			timer_mode = 2;
			Stick = tick; tick = 0;
		}										
	}
	/*
	switch( ++timer_mode )
	{
		case 0 : 
		{			
			break;
		}
		case 1 : // Start Up-count
		{
			tick = 0;
			break;		
		}
		case 2 : // Stop
		{
			Stick = tick;
			break;
		}
		default : // mode > 2
		{
			timer_mode = 0; tick = 0; Stick = 0;
			break;
		}
	}
	*/
	//		
}
// External Interrupt ISR //


// External Interrupt ISR //
// volatile int target_minute_watch = 0;
// volatile int target_second_timer = 0;
ISR(INT1_vect) // BUT2 (PD1)
{
	//
	if ( watch_mode == 1 )
	{
		time_tick = time_tick + 60; // 1분(60초)씩 증가		
		if (time_tick > 5940) time_tick = 0; // 표현 가능한 최대 분 범위 99분 이후 00분
	}
	//
	if ( watch_mode == 3 )
	{
		watch_mode = 0; time_tick = 0; time_Stick = 0;
	}
	//	
	if ( timer_mode == 1 )
	{
		tick = tick + 100; // 1초(1000 ms)씩 증가			
		if (tick > 9999) tick = 0; // 표현 가능한 범위 99.99초
	}
	//
	if ( timer_mode == 2 ) timer_mode = 3;				
	//
	if ( (timer_mode == 3) && (tick == Stick) ) // Timer Reset
	{
		timer_mode = 0; tick = 0; Stick = 0;	
	}
	//
} 
// External Interrupt ISR //


// External Interrupt ISR //
ISR(INT2_vect) // BUT3 (PD1)
{
	opt_mode++;	
	if (opt_mode > 1) opt_mode = 0; // 0 : 시계 모드, 1 : Timer 모드
	
	watch_mode = 0; timer_mode = 0;	
	time_tick = 0; time_Stick = 0;
	tick = 0; Stick = 0;
}
// External Interrupt ISR //

	
int main(void)
{	
	// *** GPIO Setting *** //
	DDRD &= ~(0x07); // PD0, PD1, PD2 입력 (SW)
	PORTD |= 0x07; // PD0, PD1, PD2 내부 Pull-up 저항 사용
	
	DDRG &= ~(0x10); // PG4 입력 (SW)
	
	DDRG |= 0x01; // PG0 출력 (LED)
	PORTG &= ~(0x01); // Initially, PG0 LED OFF
		  
	CDDR |= 0x0F; // 출력 : PB0 PB1 PB2 PB3
	IDDR = 0xFF; // 출력
	// *** GPIO Setting *** //	
	//	
    CPORT = 0x0F; // 모든 세그먼트 선택 ('1' : Digit ON, '0' : OFF)
	IPORT = ~(0x3F); // 0 모두 표시 
	// IPORT = ~(0xFF);

 	// PG4 SW ON
 	// StandBy();
	//
	
	
	// *** Interrupt Setting *** // 	
	// TIMER 0 OCM Interrupt : 10 ms Unit Time Update 
	TIMSK |= _BV(OCIE0); // TIMER0 Output Compare Match Interrupt 활성화	
	TCCR0 |= 0x07; // TIMER 0 분주비 1024 > Overflow 발생주기 16.384 ms	
	OCR0 = 156; // OCM 발생주기 : 16.384 ms * (156 / 256) = 9.984 ms	
	
		
	// TIMER 2 TOF Interrupt : FND Refresh
	// TIMSK |= _BV(TOIE2); // TIMER2 Overflow Interrupt 활성화 
	//
	TIMSK |= _BV(OCIE2); 
	OCR2 = 250; // OCM 발생주기 : 4.096 ms * (250 / 256) = 4 ms
	//
	TCCR2 |= 0x04; // TIMER 2 분주비	256 > Overflow 발생주기 4.096 ms
	// (TIMER0와 다르므로 데이터시트 참고)
	 
	 
	// TIMER 1 OCM Interrupt : 1 sec Unit Time Update
	TIMSK |= _BV(OCIE1A); 
	TCCR1B |= 0x04;
	// TCCR1B [2:0] = '100' // 분주비 256 > Overflow 발생주기 1.048576 s
	OCR1A = 62500; // OCM 발생주기 : (1.048576 s) * (62500 / 65536) = 1 s
	//  
		
		 
	// External Interrupt	
	EIMSK |= 0x07; // External Interrupt 0, 1, 2 활성화	
	EICRA |= 0x2A; // Falling Edge Active

	// Global Interrupt Activation
	sei();	
	
	// *** Interrupt Setting *** //		
	//
	digit(0);
 	// PG4 SW ON
 	StandBy();	
	//		
	while(1)
	{	
		//			
		if ( opt_mode == 0 ) // watch_mode
		{
			switch( watch_mode )
			{
				case 0 : // Ready
				{
					time_digit(0); break;					
				}				
				case 1 : // Time Setting Start (Initial Minute)
				{
					time_digit(time_tick); break;
				}
				case 2 : // Time Setting End (Initial Minute) 
				{
					time_digit(time_Stick); break;
				}
				case 3 : // Watch Start
				{
					time_digit(time_tick); break;
				}							
			} 
		} // watch_mode						
		else // opt_mode == 1 (timer_mode)
		{
			switch( timer_mode )
			{
				case 0 : // Ready
				{
					digit(0); 
					PORTG &= ~(0x01); // PG0 LED OFF
					break;		
				}			
				case 1 : // Time Setting Start (Target Second)
				{
					digit(tick); break;
				}
				case 2 : // Time Setting End (Target Second)
				{
					digit(Stick); break;
				}
				case 3 : // Timer Start
				{
					digit(tick); 
					//
					if ( tick >= Stick ) PORTG |= 0x01; // LED ON	
					//
					break;
				}		
			}
		} // timer_mode
	}

}
