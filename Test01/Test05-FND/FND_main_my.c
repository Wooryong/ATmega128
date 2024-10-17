///*
 //* FND_main_my.c
 //*
 //* Created: 2024-10-17 오후 12:21:08
 //*  Author: user
 //*/ 
//
//// *** 전처리 ***
//#define F_CPU 16000000L // 16 MHz CLK (설정 안할 시 1 MHz로 동작)
//#include <avr/io.h> // AVR 기본 헤더파일
//#define __DELAY_BACKWARD_COMPATIBLE__ // delay 함수의 인수로 변수 사용가능 (#include <util/delay.h> 위에 선언)
//#include <util/delay.h> //delay를 실행하기 위한 헤더파일
//#include <avr/sfr_defs.h>
//#include <avr/interrupt.h>
//
//
//#define CDDR	DDRB // Control Port : Port B
//#define IDDR	DDRA // Image Port : Port A
//
//#define CPORT	PORTB // Control Port : Port B
//#define IPORT	PORTA // Image Port : Port A
//
//
//#define LED0	PORTG0 // PORTG0 = 0 / ( 1 << 0 )
//#define LED1	PORTG1 // PORTG1 = 1 / ( 1 << 1 )
//#define LED2	PORTG2 // PORTG2 = 2 / ( 1 << 2 )
//
//#define SW2		PORTD0 // PORTD0 = 0 / ( 1 << 0 )
//#define SW3		PORTD1 // PORTD1 = 1 / ( 1 << 1 )
//
////unsigned char img[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67};
//unsigned char img[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67, 0x00}; // img[10] for Null
////unsigned char img[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x67, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71}; // 0 ~ F
//
//// 반전된 Version
////unsigned char img[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xD8, 0x80, 0x90};
////unsigned char img[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xD8, 0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E}; // 0 ~ F
//
////
//volatile char data[5] = {1, 2, 3, 4};
//int digit(int num) // num 변수를 1000, 100, 10, 1 자릿수 추출하여 data[] 배열에 저장
//{
	//// if, 4자리 수 이상의 숫자가 입력되면?? ex) 50,001
	//// n4 = 50; > Indexing 오류 발생
	//if( num > 9999 ) return 0; // 인수 유효성 Check (범위 초과 시 0을 반환)
	//
	////
	//data[3] = num % 10; // 1의 자리
	//data[2] = (num / 10) % 10; //  10의 자리 / 자동으로 int 형변환
	//data[1] = (num / 100) % 10; // 100의 자리
	//data[0] = (num / 1000); // 1000의 자리
	//// data[5] = {1000의 자리, 100의 자리, 10의 자리, 1의 자리}
	//
	//return 1;
//}	
//
//int main(void)
//{
	////
	////while (1)
	////{
	////for (int i = 0; i < 4; i++)
	////{
	////CPORT = _BV(i); // 1 << i;
	////IPORT = ~img[i +1];
	////_delay_ms(10);
	////}
	////}
	////
	//
	//
	////
	//// 7-Segment 연결 & 고장여부 확인
	////while (1)
	////{
	////for (int i = 0 ; i < 4 ; i++)
	////{
	////CPORT = _BV(i); // 1 << i;
	////
	////for (int j = 0 ; j < 10 ; j++)
	////{
	////IPORT = ~img[ j ];
	////_delay_ms(500);
	////}
	////}
	////}
	////	
	
	
		//
		// 7-Segment 연결 & 고장여부 확인
		//while (1)
		//{
		//for (int i = 0 ; i < 4 ; i++)
		//{
		//CPORT = _BV(i); // 1 << i;
		//
		//for (int j = 0 ; j < 10 ; j++)
		//{
		//IPORT = ~img[ j ];
		//_delay_ms(500);
		//}
		//}
		//}
		//
		
		//
		// 내가 쓴 코드 (0000 ~ 9999 사이 임의의 숫자 7-Segment에 출력)
		//int Num;
		//int Digit_1000, Digit_100, Digit_10, Digit_1;
		//
		//while (1)
		//{
		//for(Num = 0; Num < 10000; Num++)
		//{
		//Digit_1000 = (Num / 1000);		// 7
		//Digit_100 = (Num % 1000) / 100;	// 8
		//Digit_10 = (Num % 100) / 10;	// 5
		//Digit_1 = (Num % 10);			// 4
		//
		//// 상위 Digit 0 대신 Null 처리
		//if ( Num < 10 )
		//{
		//Digit_10 = 10;
		//Digit_100 = 10;
		//Digit_1000 = 10;
		//}
		//else if ( Num < 100 )
		//{
		//Digit_100 = 10;
		//Digit_1000 = 10;
		//}
		//else if ( Num < 1000 )
		////Digit_100 = Digit_100 + 0x80; //
		//Digit_1000 = 10;
		////
		//
		//
		//int data[] = {Digit_1000, Digit_100, Digit_10, Digit_1};
		//
		//for (int i = 0; i < 4; i++)
		//{
		//CPORT = _BV(i); // 1 << i; //
		//// 0x01 (PB0 - 1st Digit) > 0x02 (PB1 - 2nd Digit) > 0x04 (PB2 - 3rd Digit) > 0x08 (PB3 - 4th Digit)
		//IPORT = ~img[ data[i] ];
		//// IPORT = ~( img[ data[i] ] + dp[ data[i] ]
		//_delay_ms(5);
		//}
		//
		//}
		//}
		//
