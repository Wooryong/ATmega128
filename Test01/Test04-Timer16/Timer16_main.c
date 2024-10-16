/*
 * Test04-Timer16.c
 *
 * Created: 2024-10-16 오전 11:38:08
 * Author : user
 */ 

// *** 전처리 ***
#define F_CPU 16000000L // 16 MHz CLK (설정 안할 시 1 MHz로 동작)
#include <avr/io.h> // AVR 기본 헤더파일
#define __DELAY_BACKWARD_COMPATIBLE__ // delay 함수의 인수로 변수 사용가능 (#include <util/delay.h> 위에 선언)
#include <util/delay.h> //delay를 실행하기 위한 헤더파일
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>

// *** 
#define DDR		DDRG
#define PORT	PORTG
#define LED0	PORTG0 // PORTG0 = 0 / ( 1 << 0 )
#define LED1	PORTG1 // PORTG1 = 1 / ( 1 << 1 )
#define LED2	PORTG2 // PORTG2 = 2 / ( 1 << 2 )

#define SW2		PORTD0 // PORTD0 = 0 / ( 1 << 0 )
#define SW3		PORTD1 // PORTD1 = 1 / ( 1 << 1 )


volatile int state_1 = 0;
volatile int state_3 = 0;

ISR(TIMER1_OVF_vect)
{
	
	if (state_1) // if (state == 1)
	{
		PORT |= _BV(LED0);
		state_1 = 0;
	}
	
	else // (state == 0)
	{
		PORT &= ~( _BV(LED0) );
		state_1 = 1;
	}

}


ISR(TIMER3_OVF_vect)
{
	
	if (state_3) // if (state == 1)
	{
		PORT |= _BV(LED1);
		state_3 = 0;
	}
	
	else // (state == 0)
	{
		PORT &= ~( _BV(LED1) );
		state_3 = 1;
	}

}


int main(void)
{
	DDR |= 0x07;
	// DDR |= ( _BV(LED0) | _BV(LED1) | _BV(LED2) );
	// DDR |= (1 << LED0) | (1 << LED1) | (1 << LED2);
	
	// PORT |= ( _BV(LED0) | _BV(LED1) | _BV(LED2) );
	// PORT |= (1 << LED0) | (1 << LED1) | (1 << LED2);
	// PORT |= _BV(LED0); // PORTG |= 0x01; // LED ON
	// PORT |= _BV(LED1); // PORTG |= 0x01; // LED ON
	// PORT |= _BV(LED2); // PORTG |= 0x01; // LED ON
		
	// StandBy(); // PG4에 연결된 SW로 프로그램의 시작
		
	//
	// PORT &= ~( _BV(LED0) ); // PORTG &= ~(0x01); // LED OFF
	// PORT &= ~( _BV(LED1) ); // PORTG &= ~(0x02); // LED OFF
	// PORT &= ~( _BV(LED2) ); // PORTG &= ~(0x04); // LED OFF
	
	
	// TIMER1 Overflow Interrupt 활성화
	TIMSK |= _BV(TOIE1);
	
	// TIMER1 분주비 설정
	TCCR1B |= 0x04; // TCCR1B[2:0] = CS32 CS31 CS30
		
	// TIMER3 Overflow Interrupt 활성화
	ETIMSK |= _BV(TOIE3);
		
	// TIMER3 분주비 설정
	TCCR3B |= 0x03; // TCCR3B[2:0] = CS32 CS31 CS30
	// 16-bit TIMER Overflow 주기 : (1 / 16000000) * (분주비) * 65535 (16-bit)
	// TCCR3B[2:0] = '011' > 분주비 64 > Overflow 발생주기 ~250 ms 	
	// 분주비 256 설정 시, 약 1초마다 Overflow 발생 - (1 / 16000000) * (256) * 65535 (16-bit) ~ 1.04856 s
	
	// 전역 인터럽트 활성화
	sei();	
	
	
    /* Replace with your application code */
    while (1) 
    {
    }
}

