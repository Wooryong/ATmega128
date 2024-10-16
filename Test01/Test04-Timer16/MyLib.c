/*
 * MyLib.c
 *
 * Created: 2024-10-11 오후 2:38:15
 *  Author: user
 */ 

#include <avr/io.h> // AVR 기본 헤더파일
// Test 함수 (레지스터와 비트 마스크를 받아서 결과를 반환)
// 레지스터 = 포인터 ex) PING = PING의 Address와 연결
// int Test_Bit(char *PIN, char Mask)
int Test_Bit(char PIN, char Mask) // PINX 레지스터 값의 특정 Mask bit 판별
{
	if ( ( PIN & Mask ) != 0 ) return 1;
	else					   return 0;
};

void StandBy() // PG4 - 프로그램의 시작 SW
{
	// 초기값이 '1' 인지 검증하는 과정 필요
	// PING의 초기값 N/A (외부 Pull-up 저항 연결되어 있지만 초기값 '1' 로 확신할 수 없음)
	while( !( Test_Bit( PING, 0x10 ) ) ); // PG4 입력이 '1' 로 안정될 때까지 무한루프
	//while(1)
	//{
	//if( Test_Bit( PING, 0x10 ) ) break; // Test_Bit 함수를 이용
	//}
	
	// PG4 초기값 검증 이후 SW 눌려야 동작 시작
	while( ( Test_Bit( PING, 0x10 ) ) ); // PG4 입력이 '0' (SW ON) 될 때까지 무한루프
	//while(1)
	//{
	//if( !( Test_Bit( PING, 0x10 ) ) ) break; // Test_Bit 함수를 이용
	//}
}