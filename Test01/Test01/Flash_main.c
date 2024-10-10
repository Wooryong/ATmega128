/*
 * Test01.c
 *
 * Created: 2024-10-02 오후 12:17:17
 * Author : user
 */ 

// *** 전처리 ***
#define F_CPU 16000000L // 1초동안 생성되는 클록의 개수 (16 MHz 동작속도)
#include <avr/io.h> // AVR 기본 헤더파일
#include <util/delay.h> //delay를 실행하기 위한 헤더파일

// LED 연결 위치 : PG4 (PortG 5번째 Pin)
// SW 연결 위치 : PG3 (PortG 4번째 Pin) - Pull-up 방식 (SW OFF > '1' / SW ON > '0')

// SW0 (PG0 Pin) - ON) 점멸 시작/중지 / OFF) 이전 상태 유지
// 	(한번 누르면 켜지고, 두번 누르면 꺼짐)
// SW1 (PG1 Pin) - ON) Delay 감소 / OFF) 이전 상태 유지
// SW2 (PG2 Pin) - ON) Delay 증가 / OFF) 이전 상태 유지

int LED_ONOFF = 0; // 전역변수 선언 / 초기상태 : LED OFF
int count = 10; // 전역변수 선언 / 초기 Delay 500 ms
// int count = 10;

void SW_Check()
{
	int SW0 = PING & 0x01; // ON = '0' / OFF = '1'
	int SW1 = PING & 0x02; // ON = '0' / OFF = '1'
	int SW2 = PING & 0x04; // ON = '0' / OFF = '1'

	if (SW0 == 0) // SW0 누를 때마다 LED ON/OFF 설정
		LED_ONOFF = !(LED_ONOFF); //  '0' > '1' / '1' > '0'

	if (SW1 == 0) // SW1 누를 때마다 LED 점멸 Delay 감소 (더 빠르게 깜박임)
	{
		if (count > 2) // 최소 Delay 100 ms
			count--;
	}

	if (SW2 == 0) // SW2 누를 때마다 LED 점멸 Delay 증가 (더 느리게 깜박임)
	{
		if (count < 20) // 최대 Delay 1000 ms
			count++;
	}
}

int main(void)
{
	DDRG |= 0x10; // PG4 : 출력
	DDRG &= ~(0x07); // PG0 PG1 PG2 : 입력
	
	PORTG &= ~(0x10); // 초기상태 : LED OFF

	int i;

	// 1. SW 상태 확인하는 함수실행 (SW_Check)
	// 2. 내부 상태 (LED ONOFF 여부, Delay 설정) 업데이트 (SW Check 함수 내에서)
	// 3. LED_ONOFF 값에 따라

	while(1)
	{
		SW_Check(); // SW 상태 확인하는 함수실행 > 내부 상태 업데이트
		
		if ( LED_ONOFF == 0 ) // SW0가 눌리지 않았다면 or LED ON 상태에서 또 눌렀다면
		{
			PORTG &= ~(0x10); // LED OFF
			count = 10;
		}
		else // ( LED_ONOFF == 1 )
		{
			PORTG |= 0x10;
			for(i = 0; i < count; i++)
			{
				_delay_ms(50);
				SW_Check();
				if ( LED_ONOFF == 0 ) break;
			}

			PORTG &= ~(0x10);
			for(i = 0; i < count; i++)
			{
				_delay_ms(50);
				SW_Check();
				if ( LED_ONOFF == 0 ) break;
			}
		}
	}
}

