/*
 * Test01.c
 *
 * Created: 2024-10-02 오후 12:17:17
 * Author : user
 */ 

#define F_CPU 16000000L
#include <avr/io.h> // AVR 기본 헤더파일
//#include <avr/delay.h> //delay를 실행하기 위한 헤더파일

// LED 연결 위치 : PG4


int main(void)
{
    // DDRG = 0x10; // G Port 5번째 Pin (PG4) 출력 설정 ('0' = 입력 / '1' = 출력)
	DDRG |= 0x10; // 해당 Pin 외 다른 Pin들은 OR '0' 연산 (변화 주지 않기 위해서)
	// XXX? ???? (Port G는 Pin이 총 5개)
    while (1) // 무한 루프 형태 (전원이 공급되는 한 main 함수는 종료되선 안된다)
    {
	//	PORTG = 0x10; // G Port 5번째 Pin (PG4) 출력 High
		PORTG |= 0x10;
		//_delay_ms(1000);
		
    }
}

