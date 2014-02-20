#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

typedef unsigned char u8;

// 2
struct {
    unsigned short datlen;
    u8 ledbuf[480];
} ramimg;

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
        ramimg.datlen = 0xffff;
        dp = (u8*) &ramimg;
        unsigned short tmp = 0;
// wait for SS
        while( PINB & SSBIT );
        do {
            USIDR = tmp;
            USISR = (1 << USIOIF);
            while ( !(USISR & (1 << USIOIF)) && !(PINB & SSBIT) ) 
                ;
            *dp++ = USIDR;
        }
        while( ++tmp < 2+ramimg.datlen && !(PINB & SSBIT) );

        u8 bithi = PORTB | NPBIT;
        u8 bitlo = bithi & ~NPBIT;
        dp = ramimg.ledbuf;
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
        } while (dp != ramimg.ledbuf + ramimg.datlen);
        //_delay_us(50);
    }
}
