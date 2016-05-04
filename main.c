#include "16F877.H"
#include "stdlib.h"
#include "stddef.h"
#include "ctype.h"
#use standard_io(B)
#use delay (clock=10000000)
#fuses XT
#use i2c(Master, sda=PIN_C4, scl=PIN_C3, force_hw)
#use rs232(BAUD=9600,xmit=PIN_C6,rcv=PIN_C7) 
#byte PORTA=0x05
#byte PORTB=0x06
#byte PORTC=0x07
#byte PORTD=0x08
#byte PORTE=0x09
#byte TRISA=0x85
#byte TRISB=0x86
#byte TRISC=0x87
#byte TRISD=0x88
#byte TRISE=0x89
//#byte OPTION_REG=0x81
//#byte INTCON=0x0B
#bit LED=PORTD.0

byte  TPA81_ID = 0xD0; // Standardadresse I2C f?r TPA81
byte  t[10], i, servo; 

void main(void)
{
    i2c_start(1);
    TRISD=0b00000000;
    LED=0;
    while(1)
    {
        i2c_start();
        i2c_write(0x10);
        i2c_read();
        //i2c_write();
        //i2c_write();
        //i2c_stop(); 
        //delay_ms(1000);
        //LED=~LED;
    }
}
void read_tpa81( byte slaveID ) {

   for ( i=0; i<10; i++) {

      i2c_start();
      i2c_write(slaveID);
      i2c_write(i);            // Leseregister
      i2c_start();
      i2c_write(slaveID+1);
      t[i] =  i2c_read(0);
      i2c_stop ();
      delay_ms(10);
   }
} 