#include "16F877.H"					//Definition des PIC16F877
#use standard_io(B)              				//Definition der Standart Ein- und Ausgängen
#use delay (clock=10000000)      				//Definition der Pausenzeiten
#fuses XT                        				//Definition, dass ein extrener Clock verwendet wird
void main(void)					//Hauptprogramm
{
		while (1)				//Endlosschlaufe
		{ 
		output_high(PIN_D0);		//Pin D0 setzen
		delay_ms(1000);			//Pause 1s
		output_low(PIN_D0);			//Pin D0 löschen
		delay_ms(1000);			//Pause 1s
		}
}


/*#include "16F877.H"
//#include "stdlib.h"
//#include "stddef.h"
//#include "ctype.h"
//#use standard_io(B)
#use delay (clock=10000000)
#fuses XT
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
#bit LED=PORTD.0;

void main(void)
{
    TRISD=0b00000000;
    LED=0;
    while(1)
    {
        delay_ms(1000);
        LED=~LED;
    }
}*/
