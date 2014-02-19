#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

typedef unsigned char u8;

// 2
unsigned short volatile datlen = 12;
// 432
u8 ledbuf[144 * 3] = {
    16, 0, 0,
    0, 16, 0,
    0, 0, 16,
    8, 8, 8
};

#define PORTBIT (_BV(3))
int main(void)
{
    DDRB |= PORTBIT | _BV(1);   // SPI DO
    PORTB |= 7;                 //; set pullups,  DO=H
    PORTB &= ~PORTBIT;
    _delay_us(50);
    for (;;) {
        u8 bithi = PORTB | PORTBIT;
        u8 bitlo = bithi & ~PORTBIT;
        u8 *dp = ledbuf;
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
        } while (dp != ledbuf + datlen);

        _delay_us(50);

        USICR = (1 << USIWM0) | (1 << USICS1);  //|(1<<USICS0);

        dp = ledbuf;
        u8 tmp = 12;
        while (tmp--) {
            USIDR = tmp;
            USISR = (1 << USIOIF);
            // wait for SS, not for now...
            while (!(USISR & (1 << USIOIF)));
            *dp++ = USIDR;
        }
    }
}
