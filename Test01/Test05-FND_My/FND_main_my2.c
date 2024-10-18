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


volatile int mode = 0; // 0 : Ready, 1 : Up-Count, 2 : 정지, 3(0)
//
volatile int data[5] = {1, 2, 3, 4}; // 초기화
int digit(int num) // num 변수를 1000, 100, 10, 1 자릿수 추출하여 data[] 배열에 저장
{
	if( num > 9999 ) return 0; // 인수 유효성 Check (범위 초과 시 0을 반환)	
	//
	data[3] = num % 10; // Digit_1 
	data[2] = (num / 10) % 10; // Digit_10
	data[1] = (num / 100) % 10; // Digit_100
	data[0] = (num / 1000); // Digit_1000
	// 

	/*
	// 상위 Digit 0 대신 Null 처리
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
		
	return 1;	
} 
// int digit(int num)


// TIMER 0 OCM Interrupt ISR //
volatile int count = 0; // 
volatile int num = 0;
ISR(TIMER0_COMP_vect) // Interrupt 발생 주기 : 1 ms
{	
	//
	//if( mode == 0 ) // Initial or Reset
	//{
		//count = 0; num = 0; //
	//}
	//
	
	// else if( mode == 1 ) // Up-count
	if( mode == 1 ) // Up-count
	{
		count++;
	
		if (count == 10) // 10 ms 주기 num Update
		{
			count = 0; 
//			if () // Up-count
//			{
				num++; // Digit Update	
				if (num > 9999) num = 0;	
//			}				
//			if() // Down-count
//			{
//				num--; // Digit Update
//				if (num < 0) num = 0;
//			}
			
		}
			
	}
	
	TCNT0 = 0; // TCNT0 초기화			
} // ISR(TIMER0_COMP_vect)
// TIMER 0 OCM Interrupt ISR //


// TIMER 2 TOF Interrupt ISR //
volatile int i = 0;
//volatile int num = 0;
ISR(TIMER2_OVF_vect) // Interrupt 발생 주기 : 4 ms 
{
	// FND Refresh 
	// 약 4 ms 간격으로 한 자리씩 표시
	// 표시될 숫자 num은 TIMER0 OCM Interrupt 에서 갱신
	
	//
	//if ( i == 0 )
	//{
		//digit(num);
		//num++;
		//if (num > 9999) num = 0;
	//}
	
	//
	// digit(num); // main loop로?
		
	CPORT = _BV(i); 
	//
	if (mode == 0) 
		IPORT = ~( img[ 0 ] );
	else	
		IPORT = ~( img[ data[ i ] ] + (i == 1) * 0x80 ); // 소수점 표시 : "+ (i == 1) * 0x80 "
	//
	i++;
	if (i == 4) i = 0;
	//			
} // ISR(TIMER2_OVF_vect)
// TIMER 2 TOF Interrupt ISR //


// External Interrupt ISR //
ISR(INT0_vect) // SW (PD0) 
{
	mode++;
	if (mode == 3) mode = 0;
	// mode 0 : Ready, mode 1 : START, mode 2 : STOP (Wait), mode 3(0) : Reset
	
	
	
	
}
//
	
int main(void)
{	
	// *** GPIO Setting *** //
	DDRD &= ~(0x03); // PD0, PD1 입력 (SW)
	PORTD |= 0x03; // PD0, PD1 내부 Pull-up 저항 사용
	
	DDRG &= ~(0x10); // PG4 입력 (SW)
	  
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
	TCCR0 |= 0x04; // TIMER 0 분주비 64 > Overflow 발생주기 1.024 ms	
	OCR0 = 250; // OCI 발생주기 : 1.024 ms * (250 / 256) = 1 ms
	
	// TIMER 2 TOF Interrupt : FND Refresh
	TIMSK |= _BV(TOIE2); // TIMER2 Overflow Interrupt 활성화  
	TCCR2 |= 0x04; // TIMER 2 분주비	256 > Overflow 발생주기 4.096 ms
	// (TIMER0와 다르므로 데이터시트 참고)
	 
	// External Interrupt : Stopwatch Mode Change	
	EIMSK |= 0x03; // External Interrupt 0, 1 활성화	
	EICRA = ( EICRA & (0xF0) ) | (0x0A); // Falling Edge Active
	
	// Global Interrupt Activation
	sei();	
	
	// *** Interrupt Setting *** //
	
		
	//
	num = 0;
	digit(num);
 	// PG4 SW ON
 	StandBy();	
	//		
	while(1)
	{				
		// 0000 ~ 9999 사이 숫자 7-Segment 출력 //
		/*
		for( num = 0; num < 10000; num++)
		{
			digit(num);
			for( int i = 0 ; i < 4 ; i++ )
			{
				CPORT = _BV(i); // 1 << i
				// (PB0 - 1st Digit) > (PB1 - 2nd Digit) > (PB2 - 3rd Digit) > (PB3 - 4th Digit)
				IPORT = ~( img[ data[ i ] ] );
				// IPORT = ~( img[ data[ i ] ] + (i == 1) * 0x80 );
				// data[3] = {1000의 자리, 100의 자리, 10의 자리, 1의 자리}
				_delay_ms(5);
			}
			
			// _delay_ms(10); //
		}
		*/
		// 0000 ~ 9999 사이 숫자 7-Segment 출력 //
		
				
		// TIMER0 Output Compare Match Interrupt //
		/*
		//digit(num);
		//for( int i = 0 ; i < 4 ; i++ )
		//{
			//CPORT = _BV(i); // 1 << i
			////IPORT = ~img[ data[ i ] ];
			//IPORT = ~( img[ data[ i ] ] + (i == 1) * 0x80 );
			//_delay_ms(5);
		//}
		*/
		// TIMER0 Output Compare Match Interrupt //
		
		// digit(num); // 	
	
		//
		switch( mode )
		{
			case 0 : // Ready
			{
				count = 0; num = 0;		
				break;	
			}
			
			case 1 : // Up-count
			{
				digit(num);
				break;
			}
			case 2 : // Stop
			{
				break;
			}
			default : // 
			{
				break;
			}			
		}
		// switch
		
		
		
	}

}
