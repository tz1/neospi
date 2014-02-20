#include <SPI.h>
unsigned x = 0;
#define SELPIN 2
void setup()
{
  pinMode(SELPIN,OUTPUT);
  digitalWrite(SELPIN,1);
    Serial.begin(115200);
    SPI.begin();
    SPI.setDataMode(SPI_MODE0);
    SPI.setClockDivider(SPI_CLOCK_DIV128);
    SPI.setBitOrder(MSBFIRST);
}

void loop()
{
  byte r,g,b,rr,gr,br;
  x++;
  delay(500);
  digitalWrite(SELPIN,0);

  if( x&1 )
    r = 15;
  else
    r = 0;
  if( x&2 )
    g = 15;
  else
    g = 0;
  if( x&4 )
    b = 15;
  else
    b = 0;
  
  gr=SPI.transfer(12);
  rr=SPI.transfer(0);
  Serial.print(gr);
  Serial.print(' ');
  Serial.print(rr);
  Serial.print(' ');
  
  gr=SPI.transfer(g);
  rr=SPI.transfer(r);
  br=SPI.transfer(b);
  
  Serial.print(gr);
  Serial.print(' ');
  Serial.print(rr);
  Serial.print(' ');
  Serial.print(br);
  Serial.print(' ');

  gr=SPI.transfer(15-g);
  rr=SPI.transfer(15-r);
  br=SPI.transfer(15-b);
  
  Serial.print(gr);
  Serial.print(' ');
  Serial.print(rr);
  Serial.print(' ');
  Serial.print(br);
  Serial.print(' ');
  gr=SPI.transfer(7+g);
  rr=SPI.transfer(7+r);
  br=SPI.transfer(7+b);
  
  Serial.print(gr);
  Serial.print(' ');
  Serial.print(rr);
  Serial.print(' ');
  Serial.print(br);
  Serial.print(' ');
  gr=SPI.transfer(32-g);
  rr=SPI.transfer(32-r);
  br=SPI.transfer(32-b);
  Serial.print(gr);
  Serial.print(' ');
  Serial.print(rr);
  Serial.print(' ');
  Serial.print(br);
  Serial.print(' ');
  digitalWrite(SELPIN,1);

  Serial.println(x);
  
}

