/*
 * Test05-FND.c
 *
 * Created: 2024-10-16 오후 3:12:15
 * Author : user
 */ 

// *** 전처리 ***
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

//unsigned char img[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67};
unsigned char img[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67, 0x00}; // img[10] for Null	
//unsigned char img[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71}; // 0 ~ F

// 반전된 Version 	
//unsigned char img[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xD8, 0x80, 0x90};
//unsigned char img[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xD8, 0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E}; // 0 ~ F


//

volatile int mode = 0; // 0 : Ready, 1 : Up-Count, 2 : 정지, 3(0)

volatile char data[5] = {1, 2, 3, 4}; // 초기화
int digit(int num) // num 변수를 1000, 100, 10, 1 자릿수 추출하여 data[] 배열에 저장
{
	// if, 4자리 수 이상의 숫자가 입력되면?? ex) 50,001 
	// n4 = 50; > Indexing 오류 발생 
	if( num > 9999 ) return 0; // 인수 유효성 Check (범위 초과 시 0을 반환)
	
	//
	data[3] = num % 10; // 1의 자리 
	data[2] = (num / 10) % 10; //  10의 자리 / 자동으로 int 형변환
	data[1] = (num / 100) % 10; // 100의 자리
	data[0] = (num / 1000); // 1000의 자리	
	// data[5] = {1000의 자리, 100의 자리, 10의 자리, 1의 자리}
	
	//
	// 상위 Digit 0 대신 Null 처리
	//if ( num < 10 )
	//{
		//data[2] = 10;
		//data[1] = 10;
		//data[0] = 10;
	//}
	//else if ( num < 100 )
	//{
		//data[1] = 10;
		//data[0] = 10;
	//}
	//else if ( num < 1000 )
		//data[0] = 10;		
	//
	
	return 1;	
}
//

//

volatile int count = 0; // int cnt = 0;
volatile int num = 0;
ISR(TIMER0_COMP_vect) // 1 ms 간격
{	
	if( mode == 0 ) 
	{
		count = 0; num = 0;
	}
	else if( mode == 1 )
	{
		count++;
	
		if (count == 10) // 10 ms 간격
		{
			count = 0; num++;	
			if (num > 9999) num = 0;	
		}
		
	}
	
	TCNT0 = 0; //  TCNT0 초기화		
}
//

//
volatile int i = 0;
//volatile int num = 0;
ISR(TIMER2_OVF_vect) // FND Refresh // 4 ms 간격
{
	//
	//if ( i == 0 )
	//{
		//digit(num);
		//num++;
		//if (num > 9999) num = 0;
	//}
	//
	digit(num);
	CPORT = _BV(i); // 1 << i
	// (PB0 - 1st Digit) > (PB1 - 2nd Digit) > (PB2 - 3rd Digit) > (PB3 - 4th Digit)
	IPORT = ~( img[ data[ i ] ] + (i == 1) * 0x80 );
	// data[4] = {1000의 자리, 100의 자리, 10의 자리, 1의 자리}
	i++;
	if (i == 4) i = 0;
	//			
}
//

//
ISR(INT0_vect) // SW2(PD0) - 점점 느리게
{
	mode++;
	if (mode == 3) mode = 0;
	// mode 1 : START, mode 2 : STOP, mode 3(0) : Reset
}
//
	
int main(void)
{	
	DDRD &= ~(0x03); // PD0, PD1 입력 (SW)
	PORTD |= 0x03; // PD0, PD1 내부 Pull-up 저항 사용
	
	DDRG &= ~(0x10);
	  
	CDDR |= 0x0F; // 출력 : PB0 PB1 PB2 PB3
	IDDR = 0xFF; // 출력
	
    CPORT = 0x0F; // 모든 세그먼트 선택 ('1' : Digit ON, '0' : OFF)
	IPORT = ~(0x3F); // 0 모두 표시 
	// IPORT = ~img[ 0 ];

 	// PG4 SW ON
 	// StandBy();
		
	// TIMER0 : 100 ms Scale Stop-watch 용도
	TIMSK |= _BV(OCIE0); // TIMER0 Output Compare Match Interrupt 활성화
	//TIMSK |= 0x02;
			
	// TIMER0 분주비 설정
	TCCR0 |= 0x04; 
	//TCCR0 |= 0x07;
	// TCCR0 [2:0] = '111' // 분주비 1024 > 주기 64 us > Overflow 발생주기 16.384 ms
	// TCCR0 [2:0] = '110' // 분주비 256 > 주기 16 us > Overflow 발생주기 4.096 ms
	// TCCR0 [2:0] = '100' // 분주비 64 > 주기 4 us > Overflow 발생주기 1.024 ms
	
	OCR0 = 250; // (0 ~ 156) 
	// TCCR0 [2:0] = '111' // 분주비 1024 > 주기 64 us > Overflow 발생주기 16.384 ms	
	// if OCR0 = 156, OCI 발생주기 : 16.384 ms * (156 / 256) = 9.984 ms
	// 7번 OCI 발생 시, 14.272 ms * 7 = 99.904 ms
	// TCCR0 [2:0] = '110' // 분주비 256 > 주기 16 us > Overflow 발생주기 4.096 ms
	// if OCR0 = 250, OCI 발생주기 : 4.096 ms * (250 / 256) = 4 ms
	// TCCR0 [2:0] = '100' // 분주비 64 > 주기 4 us > Overflow 발생주기 1.024 ms
	// if OCR0 = 250, OCI 발생주기 : 1.024 ms * (250 / 256) = 1 ms

	
	// TIMER2 : FND Refresh
	TIMSK |= _BV(TOIE2); // TIMER2 Overflow Interrupt 활성화  
	//TIMSK |= 0x40;
	
	// TIMER2 분주비 설정 (TIMER0와 다르므로 데이터시트 참고)
	TCCR2 |= 0x04; 
	// TCCR2 [2:0] = '011' // 분주비 64 > 주기 4 us > Overflow 발생주기 1.024 ms
	// TCCR2 [2:0] = '100' // 분주비 256 > 주기 16 us > Overflow 발생주기 4.096 ms
	// TCCR2 [2:0] = '101' // 분주비 1024 > 주기 64 us > Overflow 발생주기 16.384 ms
	
				
	// Mask Register : EIMSK
	EIMSK |= 0x03; // INT1, INT0
	// EIMSK = 0x03; ==> EIMSK = 0000_0011
		
	// Create Register : EICRA
	EICRA = ( EICRA & (0xF0) ) | (0x0A); // 해당범위 먼저 0으로 만들고 1 덮어씌우기	
	// SW OFF > ON == H > L (하강엣지) / SW ON > OFF == L > H (상승엣지)
	// ISC11 = 1, ISC10 = 0 >> INT1 하강엣지 / ISC11 = 1, ISC10 = 1 >> INT1 상승엣지
	// ISC01 = 1, ISC00 = 0 >> INT0 하강엣지 / ISC01 = 1, ISC00 = 1 >> INT0 상승엣지
	// '10' - 하강엣지, '11' - 상승엣지	
	
	// 전역 인터럽트 활성화
	sei();	
		
	//
	// 교수님 코드 (0000 ~ 9999 사이 임의의 숫자 7-Segment에 출력)
	//num = 0; // int num = 1234;
	//digit(num); // digit 함수 실행 > num의 자릿수 추출하여 배열에 저장

	num = 0;
	digit(num);
 	// PG4 SW ON
 	StandBy();	
			
	while(1)
	{
//		for( num = 0; num < 10000; num++)
//		{
//			digit(num);
			//for( int i = 0 ; i < 4 ; i++ )
			//{
				//CPORT = _BV(i); // 1 << i
				//// (PB0 - 1st Digit) > (PB1 - 2nd Digit) > (PB2 - 3rd Digit) > (PB3 - 4th Digit)
				//IPORT = ~( img[ data[ i ] ] + (i == 1) * 0x80 );
				//// data[3] = {1000의 자리, 100의 자리, 10의 자리, 1의 자리}
				//_delay_ms(5);
			//}
			//_delay_ms(10); //
//		}

		// TIMER0 Output Compare Match Interrupt
		//digit(num);
		//for( int i = 0 ; i < 4 ; i++ )
		//{
			//CPORT = _BV(i); // 1 << i
			////IPORT = ~img[ data[ i ] ];
			//IPORT = ~( img[ data[ i ] ] + (i == 1) * 0x80 );
			//_delay_ms(5);
		//}
	
		//switch( mode )
		//{
			//case 0 : // Ready
			//
			//case 1 : // Up-count
			//
			//case 2 : // Stop
			//
			//default : // 
			//
		//}
	}
}
