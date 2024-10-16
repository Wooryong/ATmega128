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

unsigned char img[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67};
//unsigned char img[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};	

// 	
//unsigned char img[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xD8, 0x80, 0x90};
//unsigned char img[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xD8, 0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E};

//int Num = 73;
//int Digit_1000, Digit_100, Digit_10, Digit_1;
//
//Digit_1000 = (Num / 1000);		// 7
//Digit_100 = (Num % 1000) / 100;	// 8
//Digit_10 = (Num % 100) / 10;	// 5
//Digit_1 = (Num % 10);			// 4
//
//int data[] = {Digit_1000, Digit_100, Digit_10, Digit_1};	
//char data[] = {1, 2, 3, 4};
	
int main(void)
{
	
	// PG4 SW ON
	StandBy();
  
	CDDR |= 0x0F; // 출력 : PB0 PB1 PB2 PB3
	IDDR = 0xFF; // 출력
   
    CPORT = 0x0F; // 모든 세그먼트 선택 ('1' : Digit ON, '0' : OFF)
	IPORT = ~(0xFF); // 숫자 8 + 소수점 
	
	// 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67(0x6F)
    // 0xC0, 0xF5, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xD8, 0x80, 0x90

    //while (1) 
    //{
		//for (int i = 0; i < 4; i++)
		//{
			//CPORT = _BV(i); // 1 << i;
			//IPORT = ~img[i +1];
			//_delay_ms(10);
		//}
    //}
	
	int Num;
	int Digit_1000, Digit_100, Digit_10, Digit_1;

	    while (1)
	    {
		    for(Num = 0; Num < 10000; Num++)
			{
				Digit_1000 = (Num / 1000);		// 7
				Digit_100 = (Num % 1000) / 100;	// 8
				Digit_10 = (Num % 100) / 10;	// 5
				Digit_1 = (Num % 10);			// 4
						
				int data[] = {Digit_1000, Digit_100, Digit_10, Digit_1};						
			
				for (int i = 0; i < 4; i++)
				{
					CPORT = _BV(i); // 1 << i;
					// 0x01 0x02 0x04 0x08
					IPORT = ~img[ data[i] ];
					_delay_ms(5);				
				}
				
				//_delay_ms(20);
			}
	    }
	
}

