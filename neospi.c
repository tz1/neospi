#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

typedef unsigned char u8;

// 2
u8 rambuf[500];

#define NPBIT (_BV(3))
#define SSBIT (_BV(4))
int main(void)
{
    DDRB = NPBIT | _BV(1);   // SPI DO
    PORTB = ~NPBIT;
    USICR = (1 << USIWM0) | (1 << USICS1);  //|(1<<USICS0);

    u8 *dp; 
    for (;;) {
        dp = rambuf;
// wait for SS
        while( PINB & SSBIT );
        do {
            USIDR = *dp;
            USISR = (1 << USIOIF);
            while ( !(USISR & (1 << USIOIF)) && !(PINB & SSBIT) ) 
                ;
            *dp++ = USIDR;
        }
        while( !(PINB & SSBIT) );
        dp = rambuf+2;
        u8 bithi = 0xff;
        u8 bitlo = ~NPBIT;
        u8 *ep = dp + *((unsigned short *)rambuf);
        do {
            u8 tmp = *dp++;
            asm volatile (" ldi  %0,8\n"
                "loop%=:\n"
                " out  %2,%3\n"
                " nop\n"
                " sbrs %1,7\n"
                " out  %2,%4\n"
                " lsl  %1\n"
                " nop\n"
                " out  %2,%4\n"
                " dec  %0\n"
                " brne loop%=\n"
                :"=&d" (tmp):"r"(tmp), "I"(_SFR_IO_ADDR(PORTB)), "r"(bithi), "r"(bitlo)
            );
        } while (dp != ep); 
        //_delay_us(50);
    }
}
