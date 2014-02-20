#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

typedef unsigned char u8;

// 2
u8 ledbuf[500];

#define NPBIT (_BV(3))
#define SSBIT (_BV(4))
int main(void)
{
    u8 *dp; 
    DDRB |= NPBIT | _BV(1);   // SPI DO
    PORTB = 0xff;                 //; set pullups,  DO=H
    PORTB &= ~NPBIT;
    USICR = (1 << USIWM0) | (1 << USICS1);  //|(1<<USICS0);

    for (;;) {
        USIDR = 0xa5;
        dp = ledbuf;
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
        u8 bithi = PORTB | NPBIT;
        u8 bitlo = bithi & ~NPBIT;
        dp = ledbuf;
        unsigned short datlen = *dp++;
        datlen |= *dp++ << 8;
        do {
            u8 tmp = *dp++;
            asm volatile (" ldi  %0,8\n"
                "loop%=:\n"
                " out  %2,%3\n"
                " nop\n\t"
                " sbrs %1,7\n"
                " out  %2,%4\n"
                " lsl  %1\n"
                " nop\n\t"
                " out  %2,%4\n"
                " dec  %0\n"
                " brne loop%=\n\t":"=&d" (tmp)
                :"r"(tmp), "I"(_SFR_IO_ADDR(PORTB)), "r"(bithi), "r"(bitlo)
            );
        } while (dp != ledbuf + datlen+2);
        //_delay_us(50);
    }
}
