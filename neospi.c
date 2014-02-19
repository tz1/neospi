#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

typedef unsigned char u8;

// 2
unsigned short volatile datlen = 12;
// 432
u8 ledbuf[144*3] = {
16,0,0, 
0,16,0, 
0,0,16, 
8,8,8 }; 


#define PORTBIT (_BV(4))
int main(void)
{
    DDRB |=PORTBIT | _BV(1); // SPI DO
    PORTB |= 7; //; set pullups,  DO=H
    PORTB &= ~PORTBIT;
    _delay_us(50);
    for(;;) {
        u8 bithi = PORTB | PORTBIT;
        u8 bitlo = bithi & ~PORTBIT;
        u8 *dp = ledbuf;
        do {
                u8 curbyte = *dp++;
                asm volatile (
                                "       ldi   %0,8  \n\t" 
                                "loop%=:            \n\t" 
                                "       out   %2,%3 \n\t"     //  '1' [01] '0' [01] - re
                                "       nop\n\t"
                                "       sbrs  %1,7  \n\t"     //  '1' [03] '0' [02]
                                "       out   %2,%4 \n\t"     //  '1' [--] '0' [03] - fe-low
                                "       lsl   %1    \n\t"     //  '1' [04] '0' [04]
                                "       nop\n\t"
                                "       out   %2,%4 \n\t"     //  '1' [+1] '0' [+1] - fe-high
                                "       dec   %0    \n\t"     //  '1' [+2] '0' [+2]
                                "       brne  loop%=\n\t"     //  '1' [+3] '0' [+4]
                                :"=&d"(curbyte)
                                :"r"(curbyte), "I"(_SFR_IO_ADDR(PORTB)), "r"(bithi), "r"(bitlo)
                              );

        } while( dp != ledbuf + datlen );



        _delay_us(50);


        USICR = (1<<USIWM0)|(1<<USICS1);//|(1<<USICS0);

        dp = ledbuf;
        u8 curbyte = 12;
        while( curbyte--) {
                USIDR = curbyte;
                USISR = (1<<USIOIF);
                // wait for SS, not for now...
                while( !(USISR & (1<<USIOIF) ))
                        ;
                *dp++ = USIDR;
        }

#if 0

memset( ledbuf, 0, 12);

if( USIDR & 1 )
        ledbuf[1] = 16;
if( USIDR & 2 )
        ledbuf[2] = 16;
if( USIDR & 4 )
        ledbuf[4] = 16;
if( USIDR & 8 )
        ledbuf[5] = 16;
if( USIDR & 16 )
        ledbuf[7] = 16;
if( USIDR & 32 )
        ledbuf[8] = 16;
if( USIDR & 64 )
        ledbuf[10] = 16;
if( USIDR & 128 )
        ledbuf[11] = 16;
#endif

#if 0

        dp = ledbuf;
        dp[12] = dp[0];
        dp[13] = dp[1];
        dp[14] = dp[2];
        for( curbyte = 0; curbyte < 12; dp++, curbyte++ )
            dp[0] = dp[3];
#endif
    }
}
