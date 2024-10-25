/*
 * Test07-UART.c
 *
 * Created: 2024-10-25 오전 9:41:17
 * Author : user
 */ 

// *** 전처리 *** //
#define F_CPU 16000000L // 16 MHz CLK (설정 안할 시 1 MHz로 동작)
#define __DELAY_BACKWARD_COMPATIBLE__ // delay 함수의 인수로 변수 사용가능 (#include <util/delay.h> 위에 선언)

#include <avr/io.h> // AVR 기본 헤더파일
#include <util/delay.h> //delay를 실행하기 위한 헤더파일
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>

void UART0_init() // UART0 Register Setting
{
    // *** UART0 Register Setting *** //
    // 9600 - N - 8 - 1
    // [1] Baud Rate : 9600
    UBRR0H = 0; UBRR0L = 207; //
    
    // [2] (비동기) 2배속 모드
    UCSR0A |= (1 << U2X0);
    
    // [3] UCSRnC Setting : 비동기, Parity, #Data Bit, #Stop Bit
    // UCSR0C |= 0x06;    
    UCSR0C &= ~(1 << UMSEL0); // 비동기 모드 (Default)
    
    UCSR0C &= ~( (1 << UPM01) | (1 << UPM00) ); // No Parity (Default) / '00' : None, '10' : Even, '11' : Odd
    
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); // 8-bit Data Bit (Default) / '011' : 8-bit, '111' : 9-bit
    UCSR0B &= ~(1 << UCSZ02); // 8-bit Data Bit (Default)
    
    UCSR0C &= ~(1 << USBS0); // 1-bit Stop Bit (Default) / '0' : 1-bit, '1' : 2-bit
    
    // [4] RX, TX 활성화
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	
	// *** UART0 Register Setting *** //
} // void UART0_init()


void UART0_putc(char c) // char c 받아서 UDR에 전달 
{
	while (1) // UDR 송신버퍼가 준비될 때까지
	{
		if( UCSR0A & (1 << UDRE0) ) break;
	};
	// while( !(UCSR0A & (1 << UDRE0) ) ); 
		
	UDR0 = c; // Send 1 Char		
					
} // void UART0_putc(char c)

void UART0_puts(char *str) // char string str 받아서 UART0 통해 송신
{
	while( *str ) 
	{
		UART0_putc(*str);
		str++;
	}	
} // void UART0_puts(char *str)


// *** External Interrupt 0 ISR *** //
char *str = "Button Pushed!!!\r\n"; // str ~ 배열명
int n = 0;
char buf[256]; // Buffer
ISR(INT0_vect) // BUT1 (PD0)
{
	n++; 
	sprintf(buf, "Button Pushed %d times. \r\n", n);
	UART0_puts(buf);	
} // ISR(INT0_vect)
// *** External Interrupt 0 ISR *** //


int main(void)
{
	UART0_init(); // Initialize

	UART0_puts("\033[2J"); // Screen Clear (Escape Sequence) - ESC[2J 
	UART0_puts("\033[0;0H"); // Move Cursor to Home Position (Left-top) (Escape Sequence) - ESC[0;0H // ESC[(y좌표);(x좌표)H 
	// ESC = \033 (\ + 8진수 표현)
	
	UART0_puts("\033[47m");
	
	// *** Interrupt Setting *** //
	// External Interrupt
	EIMSK |= (1 << INT0); // External Interrupt 0 활성화
	EICRA |= 0x02; // INT0 Falling Edge Active

	// Global Interrupt Activation
	sei();	
	// *** Interrupt Setting *** //
	
    while (1) 
    {
					
    }
}

