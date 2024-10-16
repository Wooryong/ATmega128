/*
 * Test03-Timer0.c
 *
 * Created: 2024-10-11 오후 2:33:04
 * Author : user
 */ 

// *** 전처리 ***
#define F_CPU 16000000L // 16 MHz CLK (설정 안할 시 1 MHz로 동작)
#include <avr/io.h> // AVR 기본 헤더파일
#define __DELAY_BACKWARD_COMPATIBLE__ // delay 함수의 인수로 변수 사용가능 (#include <util/delay.h> 위에 선언)
#include <util/delay.h> //delay를 실행하기 위한 헤더파일
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>

//
#define DDR		DDRG
#define PORT	PORTG 
#define LED0	PORTG0 // PORTG0 = 0 / ( 1 << 0 )
#define LED1	PORTG1 // PORTG1 = 1 / ( 1 << 1 )
#define LED2	PORTG2 // PORTG2 = 2 / ( 1 << 2 )

#define SW2		PORTD0 // PORTD0 = 0 / ( 1 << 0 )
#define SW3		PORTD1 // PORTD1 = 1 / ( 1 << 1 )


//
volatile int count = 0; // int cnt = 0;
volatile int state = 0;

volatile int o_count = 0; 
volatile int o_state = 0;

volatile int Interval = 500; // 초기 점멸간격 500 ms 
int PS_Ratio = 256; // 분주비 (Pre-scale Ratio)

double Period = 4.096; // (1 / 16000) * 256 * 256 // ms 단위
// double Period = (1 / 16000) * 256 * 256; // ms 단위 // <<  이거 안됨 4.096
// Period = (1 / 16000000) * (분주기) * 256 (8-bit Timer) 
// 분주비 256 > Overflow 발생주기 4.096 ms

volatile int max_count = (int)(500 / 4.096); // 초기값 상수로 설정 필요
// 500 / 4.096 = 122.07XX ~ 122

//int cnt1 = 0; 
//int cntEx = 5;


ISR(TIMER0_OVF_vect)
{
	count++;
	 
	if (count > max_count) // 
	{
		count = 0;
		if (state)
		{
			PORT |= _BV(LED0);
			state = 0;
		}
		else
		{
			PORT &= ~( _BV(LED0) );
			state = 1;
		}
	}
}


//ISR(TIMER0_OVF_vect)
//{
	//count++;
	//if (count > 25) // 약 100 ms 간격 
	//{
		//count = 0;
		//cnt1++;
		//if(cnt1 >= cntEx)
		//{
			//cnt1 = 0;
			//PORT |= _BV(LED0);
			//state = 0;
		//}
		//else
		//{
			//PORT &= ~( _BV(LED0) );
			//state = 1;
		//}
	//}
//}

ISR(TIMER0_COMP_vect)
{
	o_count++;
	
	//
	if (o_count > max_count) // if (o_count > 122)
	{
		o_count = 0;
		if (o_state)
		{
			PORT |= _BV(LED1);
			o_state = 0;
		}
		else
		{
			PORT &= ~( _BV(LED1) );
			o_state = 1;
		}
	}
	
	TCNT0 = 0; // OCR에서 초기화하기 때문에 Overflow Interrupt 발생하지 못함 
}


//
ISR(INT0_vect) // SW2(PD0) - 점점 느리게
{
	Interval += 100;
	if( Interval > 2000 ) Interval = 2000;
	max_count = (int)(Interval / Period);
	//cntEx++;
}

ISR(INT1_vect) // SW3(PD1) - 점점 빠르게
{
	Interval -= 100;
	if( Interval < 100 ) Interval = 100;
	max_count = (int)(Interval / Period);
	//cntEx--;
}

//
int main(void)
{
	// 외부 인터럽트 Pin : (PD0 PD1 PD2 PD3) (PE4 PE5 PE6 PE7)
//	DDRD &= ~(0x03); // PD0, PD1 입력 (SW)
//	PORTD |= 0x03; // PD0, PD1 내부 Pull-up 저항 사용


	// StandBy 함수 실행 		
	// DDR |= _BV( LED0 | LED1 | LED2 );
//	DDR |= _BV( LED0 );
	DDR |= ( _BV(LED0) | _BV(LED1) | _BV(LED2) );
	// DDR |= (1 << LED0) | (1 << LED1) | (1 << LED2);
	// DDR |= _BV(LED0); // DDRG |= 0x01; // PG0 출력 (LED)	
	// DDR |= _BV(LED1); // DDRG |= 0x02; // PG1 출력 (LED)	
	// DDR |= _BV(LED2); // DDRG |= 0x04; // PG2 출력 (LED)
	
//	PORT |= _BV( LED0 );
	PORT |= ( _BV(LED0) | _BV(LED1) | _BV(LED2) );
	// PORT |= _BV( LED0 | LED1 | LED2 );
	// PORT |= (1 << LED0) | (1 << LED1) | (1 << LED2);
	// PORT |= _BV(LED0); // PORTG |= 0x01; // LED ON
 	// PORT |= _BV(LED1); // PORTG |= 0x01; // LED ON
	// PORT |= _BV(LED2); // PORTG |= 0x01; // LED ON
 
 	StandBy(); // PG4에 연결된 SW로 프로그램의 시작
	 
	//  
	PORT &= ~( _BV(LED0) ); // PORTG &= ~(0x01); // LED OFF
	PORT &= ~( _BV(LED1) ); // PORTG &= ~(0x02); // LED OFF
	PORT &= ~( _BV(LED2) ); // PORTG &= ~(0x04); // LED OFF
	
	// Timer Interrupt 
	TIMSK |= 0x03; // TOIE0 (TIMER0 Overflow Interrupt) 활성화
	//TIMSK |= (1 << TOIE0);
	// TIMSK |= 0x02; // OCIE0 (TIMER0 Output Compare Match Interrupt) 활성화
	//TIMSK |= (1 << OCIE0);
	// TIMSK |= 0x03; 
	OCR0 = 128; // (0 ~ 255) Overflow 발생간격의 절반
	
	TCCR0 |= 0x06; 
	// TCCR0 [2:0] = '111' // 분주비 1024 > 주기 64 us > Overflow 발생주기 16.384 ms
	// TCCR0 [2:0] = '110' // 분주비 256 > 주기 16 us > Overflow 발생주기 4.096 ms
	// 1s 간격 >> 1000 ms / (16.384 ms) ~ 61회 Overflow 발생 
	// 500 ms 간격 >> 500 ms / (16.384 ms) ~ 30회 Overflow 발생

		
	// Mask Register : EIMSK
	EIMSK |= 0x03; // INT1, INT0
	// EIMSK = 0x03; ==> EIMSK = 0000_0011
		
		
	// Create Register : EICRA
	EICRA = ( EICRA & (0xF0) ) | (0x0A); // 해당범위 먼저 0으로 만들고 1 덮어씌우기
	// EICRA |= 0x0A; // XXXX_1X1X
	// EICRA &= ~(0x05); // XXXX_X0X0
		
	//	EICRA |= 0x0A; // XXXX_1X1X //	EICRA &= ~(0x05); // XXXX_X0X0
	// SW OFF > ON == H > L (하강엣지) / SW ON > OFF == L > H (상승엣지)
	// ISC11 = 1, ISC10 = 0 >> INT1 하강엣지 / ISC11 = 1, ISC10 = 1 >> INT1 상승엣지
	// ISC01 = 1, ISC00 = 0 >> INT0 하강엣지 / ISC01 = 1, ISC00 = 1 >> INT0 상승엣지
	// '10' - 하강엣지, '11' - 상승엣지
	
		
	// 전역 인터럽트 활성화
	sei();
	 
	 
    while (1) 
    {
    }
}

